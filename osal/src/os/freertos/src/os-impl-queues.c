/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * \file
 * \ingroup  freertos 
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 *
 *      NOTE: This uses only the "Classic" RTEMS API.  It is intended to
 *      work on RTEMS targets that do not provide the POSIX API, i.e.
 *      when "--disable-posix" is given during the configuration stage.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <os-shared-globaldefs.h>
#include "os-impl-queues.h"
#include "os-freertos.h"

#include "os-shared-queue.h"
#include "os-shared-idmap.h"
#include "os-shared-timebase.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/* Tables where the OS object information is stored */
OS_impl_queue_internal_record_t OS_impl_queue_table[OS_MAX_QUEUES];

/****************************************************************************************
                                MESSAGE QUEUE API
 ***************************************************************************************/

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
int32 OS_Rtems_QueueAPI_Impl_Init(void)
{
    memset(OS_impl_queue_table, 0, sizeof(OS_impl_queue_table));
    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueCreate_Impl(const OS_object_token_t *token, uint32 flags)
{
    OS_impl_queue_internal_record_t *impl;
    OS_queue_internal_record_t *     queue;

    impl  = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);
    queue = OS_OBJECT_TABLE_GET(OS_queue_table, *token);

    impl->id = xQueueCreate(
        queue->max_depth,
        queue->max_size
    );

    /*
    ** If the operation failed, report the error
    */
    if (impl->id == NULL)
    {
        OS_DEBUG("Unhandled xQueueCreate error\n");
        return OS_ERROR;
    } else {
        return OS_SUCCESS;
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueDelete_Impl(const OS_object_token_t *token)
{
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    vQueueDelete(impl->id);

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGet_Impl(const OS_object_token_t *token, void *data, size_t size, size_t *size_copied, int32 timeout)
{
    int32 return_code;
    int32 status;
    TickType_t ticks;

    OS_impl_queue_internal_record_t *impl;
    OS_queue_internal_record_t *queue;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);
    queue = OS_OBJECT_TABLE_GET(OS_queue_table, *token);

    /* Get Message From Message Queue */
    if (timeout == OS_PEND)
    {
        ticks      = portMAX_DELAY;
    } else if (timeout == OS_CHECK) {
        ticks      = 0;
    } else {
        /* msecs rounded to the closest system tick count */
        if (OS_Milli2Ticks(timeout, (int *) &ticks) != OS_SUCCESS) {
            return OS_ERROR;
        }
    }

    /*
     ** Pend until a message arrives.
     */
    status = xQueueReceive(
         impl->id,           /* pointer to message buffer */
         data,     /* returned size of message */
         ticks           /* timeout */
    );

    if (status == pdTRUE)
    {
        return_code = OS_SUCCESS;
    } else if (timeout == OS_CHECK) {
        return_code = OS_QUEUE_EMPTY;
    } else {
        return_code = OS_QUEUE_TIMEOUT;
    }
    // TODO message size check needs to be here
    OS_DEBUG("xQueueReceive buffer size of queue->max_size (may not be accurate)\n");
    *size_copied = queue->max_size; 

    return return_code;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueuePut_Impl(const OS_object_token_t *token, const void *data, size_t size, uint32 flags)
{
    int32 status;
    OS_impl_queue_internal_record_t *impl;

    impl = OS_OBJECT_TABLE_GET(OS_impl_queue_table, *token);

    int32 ticks_to_wait = 0;


    status = xQueueSend(
        impl->id,
        data,           /* pointer to message */
        ticks_to_wait            /* length of message */
    );

    if (status == errQUEUE_FULL) { 
        return OS_QUEUE_FULL;
    } else if(status != pdTRUE) {
        OS_DEBUG("xQueueSend error: %d\n", status);
        return OS_ERROR;
    }

    return OS_SUCCESS;
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_QueueGetInfo_Impl(const OS_object_token_t *token, OS_queue_prop_t *queue_prop)
{
    /* No extra info for queues in the OS implementation */
    return OS_SUCCESS;
}
