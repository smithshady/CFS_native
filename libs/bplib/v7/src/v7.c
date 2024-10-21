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

#include <string.h>

#include <time.h>

#include "bplib.h"
#include "bplib_os.h"
#include "crc.h"
#include "v7.h"
#include "v7_types.h"
#include "v7_codec.h"
#include "v7_mpool.h"

/******************************************************************************
 DEFINES
 ******************************************************************************/

/******************************************************************************
 TYPEDEFS
 ******************************************************************************/

typedef struct bp_blocks
{
    bplib_mpool_block_t pblk;
} bp_v7block_t;

/******************************************************************************
 FUNCTIONS
 ******************************************************************************/

void v7_init_ipn_eid(bp_endpointid_buffer_t *eid, bp_ipn_t node, bp_ipn_t service)
{
    eid->scheme                 = bp_endpointid_scheme_ipn;
    eid->ssp.ipn.node_number    = node;
    eid->ssp.ipn.service_number = service;
}

void v7_set_eid(bp_endpointid_buffer_t *eid, const bp_ipn_addr_t *bp_addr)
{
    eid->scheme                 = bp_endpointid_scheme_ipn;
    eid->ssp.ipn.node_number    = bp_addr->node_number;
    eid->ssp.ipn.service_number = bp_addr->service_number;
}

void v7_get_eid(bp_ipn_addr_t *bp_addr, const bp_endpointid_buffer_t *eid)
{
    if (eid->scheme == bp_endpointid_scheme_ipn)
    {
        bp_addr->node_number    = eid->ssp.ipn.node_number;
        bp_addr->service_number = eid->ssp.ipn.service_number;
    }
    else
    {
        bp_addr->node_number    = 0;
        bp_addr->service_number = 0;
    }
}

int v7_compare_ipn2eid(const bp_ipn_addr_t *ipn, const bp_endpointid_buffer_t *eid)
{
    int result;

    result = v7_compare_numeric(eid->scheme, bp_endpointid_scheme_ipn);
    if (result == 0)
    {
        result = v7_compare_numeric(ipn->node_number, eid->ssp.ipn.node_number);
        if (result == 0)
        {
            result = v7_compare_numeric(ipn->service_number, eid->ssp.ipn.service_number);
        }
    }

    return result;
}

int v7_compare_ipn2ipn(const bp_ipn_addr_t *ipn1, const bp_ipn_addr_t *ipn2)
{
    int result;

    result = v7_compare_numeric(ipn1->node_number, ipn2->node_number);
    if (result == 0)
    {
        result = v7_compare_numeric(ipn1->service_number, ipn2->service_number);
    }

    return result;
}

bp_dtntime_t v7_get_current_time(void)
{
    return bplib_os_get_dtntime_ms();
}
