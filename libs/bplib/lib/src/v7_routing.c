/*
 * NASA Docket No. GSC-18,587-1 and identified as “The Bundle Protocol Core Flight
 * System Application (BP) v6.5”
 *
 * Copyright © 2020 United States Government as represented by the Administrator of
 * the National Aeronautics and Space Administration. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/******************************************************************************
 INCLUDES
 ******************************************************************************/

#include "bplib.h"
#include "bplib_os.h"
#include "bplib_routing.h"
#include "v7_mpool.h"
#include "v7_mpool_bblocks.h"
#include "v7_mpool_flows.h"
#include "v7_mpool_ref.h"
#include "v7.h"
#include "v7_base_internal.h"

/**
 * @brief Minimum time between interface poll cycles
 *
 * Most timers in DTN are on the longer side (i.e. minutes/hours, not milliseconds)
 * so it is generally not necessary to be aggressive with polling.
 */
#define BPLIB_INTF_MIN_POLL_INTERVAL 500

#define BPLIB_INTF_AVAILABLE_FLAGS (BPLIB_INTF_STATE_OPER_UP | BPLIB_INTF_STATE_ADMIN_UP)

bplib_mpool_ref_t bplib_route_get_intf_controlblock(bplib_routetbl_t *tbl, bp_handle_t intf_id)
{
    bplib_mpool_block_t *blk;

    blk = bplib_mpool_block_from_external_id(tbl->pool, intf_id);
    return bplib_mpool_ref_create(blk);
}

void bplib_route_release_intf_controlblock(bplib_routetbl_t *tbl, bplib_mpool_ref_t refptr)
{
    bplib_mpool_ref_release(refptr);
}

bplib_mpool_flow_t *bplip_route_lookup_intf(const bplib_routetbl_t *tbl, bp_handle_t intf_id)
{
    bplib_mpool_block_t *blk;

    blk = bplib_mpool_block_from_external_id(tbl->pool, intf_id);
    return bplib_mpool_flow_cast(blk);
}

static inline const bplib_mpool_flow_t *bplip_route_lookup_intf_const(const bplib_routetbl_t *tbl, bp_handle_t intf_id)
{
    /* note that bplip_route_lookup_intf does not modify its argument itself */
    return bplip_route_lookup_intf(tbl, intf_id);
}

/******************************************************************************
 EXPORTED FUNCTIONS
 ******************************************************************************/

int bplib_route_ingress_to_parent(void *arg, bplib_mpool_block_t *subq_src)
{
    bplib_mpool_flow_t *curr_flow;
    bplib_mpool_flow_t *next_flow;
    uint32_t            queue_depth;

    /* This implements a simple ingress for sub-interfaces (of any kind) where the ingress
     * of the sub-interface nodes is simply funneled into the parent */

    curr_flow = bplib_mpool_flow_cast(bplib_mpool_get_block_from_link(subq_src));
    if (curr_flow == NULL)
    {
        bplog(NULL, BP_FLAG_DIAGNOSTIC, "Failed to cast flow block\n");
        return -1;
    }

    next_flow = bplib_mpool_flow_cast(bplib_mpool_dereference(curr_flow->parent));
    if (next_flow == NULL)
    {
        bplog(NULL, BP_FLAG_DIAGNOSTIC, "Failed to cast flow parent block\n");
        return -1;
    }

    /* Entire contents moved as a batch. */
    queue_depth = bplib_mpool_flow_try_move_all(&next_flow->ingress, &curr_flow->ingress, 0);

    return queue_depth;
}

void bplib_route_ingress_route_single_bundle(bplib_routetbl_t *tbl, bplib_mpool_block_t *pblk)
{
    bplib_mpool_bblock_primary_t *pri_block;
    bp_primary_block_t           *pri;
    bp_ipn_addr_t                 dest_addr;
    bp_handle_t                   next_hop;
    uint32_t                      req_flags;
    uint32_t                      flag_mask;

    /* is this routable right now? */
    pri_block = bplib_mpool_bblock_primary_cast(pblk);
    if (pri_block != NULL)
    {
        pri = bplib_mpool_bblock_primary_get_logical(pri_block);

        v7_get_eid(&dest_addr, &pri->destinationEID);

        /* the next hop must be "up" (both administratively and operationally) to be valid */
        /* Also, if this bundle has not yet been stored, and the delivery policy wants some form of acknowledgement,
         * then it should be directed to an interface that is storage-capable, even if the outgoing CLA is up */
        req_flags = BPLIB_INTF_STATE_ADMIN_UP | BPLIB_INTF_STATE_OPER_UP;
        flag_mask = req_flags;

        if (!bp_handle_is_valid(pri_block->data.delivery.storage_intf_id) &&
            pri_block->data.delivery.delivery_policy != bplib_policy_delivery_none)
        {
            /* not yet stored and needs to be, so next hop must be a storage */
            flag_mask |= BPLIB_MPOOL_FLOW_FLAGS_STORAGE;
            req_flags |= BPLIB_MPOOL_FLOW_FLAGS_STORAGE;
        }
        next_hop = bplib_route_get_next_intf_with_flags(tbl, dest_addr.node_number, req_flags, flag_mask);
        if (bp_handle_is_valid(next_hop) && bplib_route_push_egress_bundle(tbl, next_hop, pblk) == 0)
        {
            /* successfully routed */
            ++tbl->routing_success_count;
            pblk = NULL;
        }
    }

    /* if qblk is still set to non-null at this point, it means the block was not routable */
    if (pblk != NULL)
    {
        /* this should never happen, must discard the block because there is nowhere to put it */
        bplib_mpool_recycle_block(pblk);
        ++tbl->routing_error_count;
    }
}

int bplib_route_ingress_baseintf_forwarder(void *arg, bplib_mpool_block_t *subq_src)
{
    bplib_mpool_block_t *qblk;
    bplib_mpool_flow_t  *flow;
    int                  forward_count;

    flow = bplib_mpool_flow_cast(bplib_mpool_get_block_from_link(subq_src));
    if (flow == NULL)
    {
        bplog(NULL, BP_FLAG_DIAGNOSTIC, "Failed to cast flow block\n");
        return -1;
    }

    forward_count = 0;
    while (true)
    {
        qblk = bplib_mpool_flow_try_pull(&flow->ingress, 0);
        if (qblk == NULL)
        {
            /* no more bundles */
            break;
        }

        /* Increment the counter based off items shifted from the input queue -
         * even if it gets dropped after this (hopefully not) it still counts
         * as something moved/changed by this action */
        ++forward_count;

        /*
         * This call always puts the block somewhere -
         * if its unroutable, the block will be put into the recycle bin.
         */
        bplib_route_ingress_route_single_bundle(arg, qblk);
    }

    /* This should return 0 if it did no work and no errors.
     * Should return >0 if some work was done */
    return forward_count;
}

bplib_mpool_t *bplib_route_get_mpool(const bplib_routetbl_t *tbl)
{
    return tbl->pool;
}

bplib_routetbl_t *bplib_route_alloc_table(uint32_t max_routes, size_t cache_mem_size)
{
    size_t            complete_size;
    size_t            align;
    size_t            route_offset;
    size_t            bplib_mpool_offset;
    uint8_t          *mem_ptr;
    bplib_routetbl_t *tbl_ptr;
    struct routeentry_align
    {
        /* This byte only exists to check the offset of the following member */
        /* cppcheck-suppress unusedStructMember */
        uint8_t            byte;
        bplib_routeentry_t route_tbl_offset;
    };

    if (max_routes == 0)
    {
        return NULL;
    }

    complete_size = sizeof(bplib_routetbl_t);

    align         = offsetof(struct routeentry_align, route_tbl_offset) - 1;
    complete_size = (complete_size + align) & ~align;
    route_offset  = complete_size;
    complete_size += sizeof(bplib_routeentry_t) * max_routes;

    align = sizeof(void *) - 1;
    align |= sizeof(uintmax_t) - 1;
    complete_size      = (complete_size + align) & ~align;
    bplib_mpool_offset = complete_size;
    complete_size += cache_mem_size;

    complete_size = (complete_size + align) & ~align;

    tbl_ptr = (bplib_routetbl_t *)bplib_os_calloc(complete_size);
    mem_ptr = (uint8_t *)tbl_ptr;

    if (tbl_ptr != NULL)
    {
        tbl_ptr->pool = bplib_mpool_create(mem_ptr + bplib_mpool_offset, complete_size - bplib_mpool_offset);
        if (tbl_ptr->pool == NULL)
        {
            bplib_os_free(tbl_ptr);
            tbl_ptr = NULL;
        }
    }

    if (tbl_ptr != NULL)
    {
        tbl_ptr->activity_lock  = bplib_os_createlock();
        tbl_ptr->last_intf_poll = bplib_os_get_dtntime_ms();
        bplib_mpool_init_list_head(NULL, &tbl_ptr->flow_list);

        tbl_ptr->max_routes = max_routes;
        tbl_ptr->route_tbl  = (void *)(mem_ptr + route_offset);
    }

    return tbl_ptr;
}

bp_handle_t bplib_route_register_generic_intf(bplib_routetbl_t *tbl, bp_handle_t parent_intf_id,
                                              bplib_mpool_block_t *flow_block)
{
    bplib_mpool_flow_t *flow;
    bplib_mpool_ref_t   fref;
    bp_handle_t         result;

    result = BP_INVALID_HANDLE;
    flow   = bplib_mpool_flow_cast(flow_block);
    if (flow == NULL)
    {
        bplog(NULL, BP_FLAG_OUT_OF_MEMORY, "Failed to cast flow block\n");
        return result;
    }

    fref = bplib_route_get_intf_controlblock(tbl, parent_intf_id);

    if (fref == NULL && bp_handle_is_valid(parent_intf_id))
    {
        /* this is an error */
        bplog(NULL, BP_FLAG_DIAGNOSTIC, "Parent intf not valid\n");
    }
    else
    {
        flow->parent = fref;
        result       = bplib_mpool_get_external_id(flow_block);

        /* keep track of all flows here in the flow_list - this counts as a ref so it
         * will not be auto-collected as long as its here in this list.  The ref is
         * intentionally _not_ released here, see bplib_route_del_intf */
        fref = bplib_mpool_ref_create(flow_block);
        if (fref != NULL)
        {
            bplib_os_lock(tbl->activity_lock);
            bplib_mpool_insert_before(&tbl->flow_list, flow_block);
            bplib_os_unlock(tbl->activity_lock);
        }
    }

    return result;
}

int bplib_route_register_handler_impl(bplib_routetbl_t *tbl, bp_handle_t intf_id, size_t func_position,
                                      bplib_mpool_callback_func_t new_func)
{
    bplib_mpool_flow_t          *ifp;
    uint8_t                     *base_ptr;
    bplib_mpool_subq_workitem_t *subq;

    ifp = bplip_route_lookup_intf(tbl, intf_id);
    if (ifp == NULL)
    {
        /* Not a valid handle */
        return -1;
    }

    base_ptr = (uint8_t *)ifp;

    subq = (bplib_mpool_subq_workitem_t *)(void *)(base_ptr + func_position);

    if (subq->job_header.handler == new_func)
    {
        /* already registered */
        return 0;
    }

    /* One or the other must be NULL or else this is an invalid registration */
    if (subq->job_header.handler != NULL && new_func != NULL)
    {
        return -1;
    }

    /* OK, now set it to the new value */
    subq->job_header.handler = new_func;
    return 0;
}

int bplib_route_register_forward_ingress_handler(bplib_routetbl_t *tbl, bp_handle_t intf_id,
                                                 bplib_mpool_callback_func_t ingress)
{
    return bplib_route_register_handler_impl(tbl, intf_id, offsetof(bplib_mpool_flow_t, ingress), ingress);
}

int bplib_route_register_forward_egress_handler(bplib_routetbl_t *tbl, bp_handle_t intf_id,
                                                bplib_mpool_callback_func_t egress)
{
    return bplib_route_register_handler_impl(tbl, intf_id, offsetof(bplib_mpool_flow_t, egress), egress);
}

int bplib_route_register_event_handler(bplib_routetbl_t *tbl, bp_handle_t intf_id, bplib_mpool_callback_func_t event)
{
    bplib_mpool_flow_t *ifp;

    ifp = bplip_route_lookup_intf(tbl, intf_id);
    if (ifp == NULL)
    {
        /* Not a valid handle */
        return -1;
    }

    if (ifp->statechange_job.event_handler == event)
    {
        /* already registered */
        return 0;
    }

    /* One or the other must be NULL or else this is an invalid registration */
    if (ifp->statechange_job.event_handler != NULL && event != NULL)
    {
        return -1;
    }

    /* OK, now set it to the new value */
    ifp->statechange_job.event_handler = event;
    return 0;
}

int bplib_route_del_intf(bplib_routetbl_t *tbl, bp_handle_t intf_id)
{
    uint32_t            pos;
    bplib_routeentry_t *rp;
    bplib_mpool_ref_t   ref;
    bplib_mpool_flow_t *ifp;

    ref = bplib_route_get_intf_controlblock(tbl, intf_id);
    if (ref == NULL)
    {
        /* Not a valid handle */
        return -1;
    }

    ifp = bplib_mpool_flow_cast(bplib_mpool_dereference(ref));
    if (ifp != NULL)
    {
        /* before it can be deleted, should ensure it is not referenced */
        for (pos = 0; pos < tbl->registered_routes; ++pos)
        {
            rp = &tbl->route_tbl[pos];
            if (bp_handle_equal(rp->intf_id, intf_id))
            {
                --tbl->registered_routes;

                /* close the gap, if this left one */
                if (pos < tbl->registered_routes)
                {
                    memmove(&rp[0], &rp[1], sizeof(*rp) * (tbl->registered_routes - pos));
                }
                break;
            }
        }
    }

    /* remove the flow from the flow_list.  This releases the reference
     * that was created during bplib_route_register_generic_intf()  */
    bplib_os_lock(tbl->activity_lock);
    if (bplib_mpool_is_link_attached(bplib_mpool_dereference(ref)))
    {
        bplib_mpool_extract_node(bplib_mpool_dereference(ref));
    }
    bplib_os_unlock(tbl->activity_lock);

    /* release the local ref, this should make the refcount 0 again */
    bplib_mpool_ref_release(ref);

    /* just wipe it */
    memset(ifp, 0, sizeof(*ifp));
    return 0;
}

bp_handle_t bplib_route_get_next_intf_with_flags(const bplib_routetbl_t *tbl, bp_ipn_t dest, uint32_t req_flags,
                                                 uint32_t flag_mask)
{
    uint32_t                  pos;
    bplib_routeentry_t       *rp;
    bp_handle_t               intf;
    const bplib_mpool_flow_t *ifp;
    uint32_t                  intf_flags;

    intf = BP_INVALID_HANDLE;
    for (pos = 0; pos < tbl->registered_routes; ++pos)
    {
        rp = &tbl->route_tbl[pos];
        if (((rp->dest ^ dest) & rp->mask) == 0)
        {
            intf_flags = ~req_flags;
            if (flag_mask != 0)
            {
                ifp = bplip_route_lookup_intf_const(tbl, rp->intf_id);
                if (ifp != NULL)
                {
                    intf_flags = ifp->current_state_flags;
                }
            }
            if ((intf_flags & flag_mask) == req_flags)
            {
                intf = rp->intf_id;
                break;
            }
        }
    }

    return intf;
}

bp_handle_t bplib_route_get_next_avail_intf(const bplib_routetbl_t *tbl, bp_ipn_t dest)
{
    return bplib_route_get_next_intf_with_flags(tbl, dest, BPLIB_INTF_AVAILABLE_FLAGS, BPLIB_INTF_AVAILABLE_FLAGS);
}

int bplib_route_push_ingress_bundle(const bplib_routetbl_t *tbl, bp_handle_t intf_id, bplib_mpool_block_t *cb)
{
    bplib_mpool_flow_t *flow;
    int                 status;

    status = -1;
    flow   = bplip_route_lookup_intf(tbl, intf_id);
    if (flow != NULL && bplib_mpool_flow_try_push(&flow->ingress, cb, 0))
    {
        status = 0;
    }

    return status;
}

int bplib_route_push_egress_bundle(const bplib_routetbl_t *tbl, bp_handle_t intf_id, bplib_mpool_block_t *cb)
{
    bplib_mpool_flow_t *flow;
    int                 status;

    status = -1;
    flow   = bplip_route_lookup_intf(tbl, intf_id);
    if (flow != NULL && bplib_mpool_flow_try_push(&flow->egress, cb, 0))
    {
        status = 0;
    }

    return status;
}

int bplib_route_add(bplib_routetbl_t *tbl, bp_ipn_t dest, bp_ipn_t mask, bp_handle_t intf_id)
{
    uint32_t            pos;
    uint32_t            insert_pos;
    bplib_routeentry_t *rp;

    if (tbl->registered_routes >= tbl->max_routes)
    {
        return -1;
    }

    /* Mask check: should have MSB's set, no gaps */
    if (((~mask + 1) & (~mask)) != 0)
    {
        return -1;
    }

    /* Find the position, the sequence should go from most specific to least specific mask */
    insert_pos = 0;
    for (pos = 0; pos < tbl->registered_routes; ++pos)
    {
        rp = &tbl->route_tbl[pos];
        if (rp->mask == mask && rp->dest == dest && bp_handle_equal(rp->intf_id, intf_id))
        {
            break;
        }
        if ((rp->mask & mask) == mask)
        {
            ++insert_pos; /* must come (at least) after this route */
        }
    }

    if (pos < tbl->registered_routes)
    {
        /* duplicate route */
        return -1;
    }

    /* If necessary, shift entries back to make a gap.
     * This is somewhat expensive, but route add/remove probably does not
     * happen that often */
    rp = &tbl->route_tbl[insert_pos];
    if (insert_pos < tbl->registered_routes)
    {
        memmove(&rp[1], &rp[0], sizeof(*rp) * (tbl->registered_routes - insert_pos));
    }

    rp->dest    = dest;
    rp->mask    = mask;
    rp->intf_id = intf_id;

    ++tbl->registered_routes;

    return 0;
}

int bplib_route_del(bplib_routetbl_t *tbl, bp_ipn_t dest, bp_ipn_t mask, bp_handle_t intf_id)
{
    uint32_t            pos;
    bplib_routeentry_t *rp;

    if (tbl->registered_routes == 0)
    {
        return -1;
    }

    /* Find the position, the sequence should go from most specific to least specific */
    for (pos = 0; pos < tbl->registered_routes; ++pos)
    {
        rp = &tbl->route_tbl[pos];
        if (rp->mask == mask && rp->dest == dest && bp_handle_equal(rp->intf_id, intf_id))
        {
            break;
        }
    }

    if (pos >= tbl->registered_routes)
    {
        /* route not found */
        return -1;
    }

    --tbl->registered_routes;

    /* If this was in the middle, close the gap */
    rp = &tbl->route_tbl[pos];
    if (pos < tbl->registered_routes)
    {
        memmove(&rp[0], &rp[1], sizeof(*rp) * (tbl->registered_routes - pos));
    }

    return 0;
}

int bplib_route_intf_set_flags(bplib_routetbl_t *tbl, bp_handle_t intf_id, uint32_t flags)
{
    bplib_mpool_ref_t flow_ref;

    flow_ref = bplib_route_get_intf_controlblock(tbl, intf_id);
    if (flow_ref != NULL)
    {
        if (bplib_mpool_flow_modify_flags(bplib_mpool_dereference(flow_ref), flags, 0))
        {
            bplib_route_set_maintenance_request(tbl);
        }
    }
    bplib_route_release_intf_controlblock(tbl, flow_ref);

    return 0;
}

int bplib_route_intf_unset_flags(bplib_routetbl_t *tbl, bp_handle_t intf_id, uint32_t flags)
{
    bplib_mpool_ref_t flow_ref;

    flow_ref = bplib_route_get_intf_controlblock(tbl, intf_id);
    if (flow_ref != NULL)
    {
        if (bplib_mpool_flow_modify_flags(bplib_mpool_dereference(flow_ref), 0, flags))
        {
            bplib_route_set_maintenance_request(tbl);
        }
    }
    bplib_route_release_intf_controlblock(tbl, flow_ref);

    return 0;
}

void bplib_route_do_timed_poll(bplib_routetbl_t *tbl)
{
    uint64_t                current_time;
    uint64_t                poll_time;
    uint32_t                set_flags;
    uint32_t                clear_flags;
    bool                    poll_cycle;
    bplib_mpool_list_iter_t iter;
    int                     status;

    current_time = bplib_os_get_dtntime_ms();
    set_flags    = 0;
    clear_flags  = 0;

    /* because the time is a 64-bit value and may not be atomic, it should
     * be sampled and updated inside of a lock section to ensure the value
     * is consistent */
    bplib_os_lock(tbl->activity_lock);
    poll_time = tbl->last_intf_poll + BPLIB_INTF_MIN_POLL_INTERVAL;
    if (current_time >= poll_time)
    {
        tbl->last_intf_poll = poll_time;
        ++tbl->poll_count;
        poll_cycle = (tbl->poll_count & 1);

        if (poll_cycle)
        {
            set_flags = BPLIB_MPOOL_FLOW_FLAGS_POLL;
        }
        else
        {
            clear_flags = BPLIB_MPOOL_FLOW_FLAGS_POLL;
        }

        status = bplib_mpool_list_iter_goto_first(&tbl->flow_list, &iter);
        while (status == BP_SUCCESS)
        {
            /* NOTE: this will end up taking the pool lock as well, when it schedules the
             * state change for processing.  This means this task will have two locks at
             * once (the tbl activity lock and the pool resource lock).  As long as the locks
             * are always taken in that order (and not the other way around) this should be OK
             * for now, but it needs to be ensured that locks are never taken in the opposite
             * order. */
            bplib_mpool_flow_modify_flags(iter.position, set_flags, clear_flags);
            status = bplib_mpool_list_iter_forward(&iter);
        }
    }
    bplib_os_unlock(tbl->activity_lock);
}

void bplib_route_set_maintenance_request(bplib_routetbl_t *tbl)
{
    tbl->maint_request_flag = true;
    bplib_os_broadcast_signal(tbl->activity_lock);
}

void bplib_route_maintenance_request_wait(bplib_routetbl_t *tbl)
{
    uint64_t poll_time;

    bplib_os_lock(tbl->activity_lock);

    /* because the time is a 64-bit value and may not be atomic, it should
     * be sampled and updated inside of a lock section to ensure the value
     * is consistent */
    poll_time = tbl->last_intf_poll + BPLIB_INTF_MIN_POLL_INTERVAL;

    while (!tbl->maint_request_flag && bplib_os_get_dtntime_ms() < poll_time)
    {
        bplib_os_wait_until_ms(tbl->activity_lock, poll_time);
    }

    tbl->maint_request_flag = false;
    tbl->maint_active_flag  = true;

    bplib_os_unlock(tbl->activity_lock);
}

void bplib_route_maintenance_complete_wait(bplib_routetbl_t *tbl)
{
    bplib_os_lock(tbl->activity_lock);

    while (tbl->maint_request_flag || tbl->maint_active_flag)
    {
        bplib_os_wait_until_ms(tbl->activity_lock, BP_DTNTIME_INFINITE);
    }

    bplib_os_unlock(tbl->activity_lock);
}

void bplib_route_process_active_flows(bplib_routetbl_t *tbl)
{
    bplib_mpool_job_run_all(tbl->pool, tbl);
}

void bplib_route_periodic_maintenance(bplib_routetbl_t *tbl)
{
    /* execute time-based interface polling for intfs that require it */
    bplib_route_do_timed_poll(tbl);

    /* now forward any bundles between interfaces, based on active flows */
    bplib_route_process_active_flows(tbl);

    /* do general pool garbage collection to make sure it was done at least once */
    bplib_mpool_maintain(tbl->pool);

    bplib_os_lock(tbl->activity_lock);
    tbl->maint_active_flag = false;
    bplib_os_broadcast_signal_and_unlock(tbl->activity_lock);
}
