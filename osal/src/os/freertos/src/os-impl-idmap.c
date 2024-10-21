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
 * \ingroup  rtems
 * \author   joseph.p.hickey@nasa.gov
 *
 *      This file contains some of the OS APIs abstraction layer for RTEMS
 *      This has been tested against the current RTEMS 4.11 release branch
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

#include <os-shared-globaldefs.h>
#include "os-freertos.h"
#include "os-shared-idmap.h"
#include "os-impl-idmap.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                     GLOBALS
 ***************************************************************************************/
// TODO May want to switch SemaphoreHandle_t to static OS_impl_objtype_lock_t for portability
SemaphoreHandle_t OS_task_table_sem;
SemaphoreHandle_t OS_queue_table_sem;
SemaphoreHandle_t OS_bin_sem_table_sem;
SemaphoreHandle_t OS_mutex_table_sem;
SemaphoreHandle_t OS_count_sem_table_sem;
SemaphoreHandle_t OS_stream_table_mut;
SemaphoreHandle_t OS_dir_table_mut;
SemaphoreHandle_t OS_timebase_table_mut;
SemaphoreHandle_t OS_timecb_table_mut;
SemaphoreHandle_t OS_module_table_mut;
SemaphoreHandle_t OS_filesys_table_mut;
SemaphoreHandle_t OS_console_mut;

static SemaphoreHandle_t *const MUTEX_TABLE[] = {
    [OS_OBJECT_TYPE_UNDEFINED] = NULL,
    [OS_OBJECT_TYPE_OS_TASK] = &OS_task_table_sem,
    [OS_OBJECT_TYPE_OS_QUEUE] = &OS_queue_table_sem,
    [OS_OBJECT_TYPE_OS_COUNTSEM] = &OS_count_sem_table_sem,
    [OS_OBJECT_TYPE_OS_BINSEM] = &OS_bin_sem_table_sem,
    [OS_OBJECT_TYPE_OS_MUTEX] = &OS_mutex_table_sem,
    [OS_OBJECT_TYPE_OS_STREAM] = &OS_stream_table_mut,
    [OS_OBJECT_TYPE_OS_DIR] = &OS_dir_table_mut,
    [OS_OBJECT_TYPE_OS_TIMEBASE] = &OS_timebase_table_sem,
    [OS_OBJECT_TYPE_OS_TIMECB] = &OS_timecb_table_mut,
    [OS_OBJECT_TYPE_OS_MODULE] = &OS_module_table_mut,
    [OS_OBJECT_TYPE_OS_FILESYS] = &OS_filesys_table_mut,
    [OS_OBJECT_TYPE_OS_CONSOLE] = &OS_console_mut,
};

enum {
    MUTEX_TABLE_SIZE = (sizeof(MUTEX_TABLE) / sizeof(MUTEX_TABLE[0]))
};

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_Lock_Global_Impl(osal_objtype_t idtype)
{
    SemaphoreHandle_t *impl;
    
    if(idtype < MUTEX_TABLE_SIZE) {
        mut = MUTEX_TABLE[idtype];
    } else {
        mut = NULL;
    }
    
    if(mut == NULL) {
        return OS_ERROR;
    }
    
    if(xSemaphoreTake(*mut, portMAX_DELAY) != pdTRUE) {
        OS_DEBUG("OS_Lock_Global_Impl: xSemaphoreTake failed\n");
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
void OS_Unlock_Global_Impl(osal_objtype_t idtype)
{
    SemaphoreHandle_t *impl;
    if(idtype < MUTEX_TABLE_SIZE) {
        mut = MUTEX_TABLE[idtype];
    } else {
        mut = NULL;
    }
    if(mut == NULL) {
        return OS_ERROR;
    }
    if(xSemaphoreGive(*mut) != pdTRUE) {
        OS_DEBUG("OS_Unlock_Global_Impl: xSemaphoreGive failed\n");
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
void OS_WaitForStateChange_Impl(osal_objtype_t idtype, uint32 attempts)
{
    TickType_t wait_ticks;

    if (attempts <= 10)
    {
        wait_ticks = attempts * attempts;
    }
    else
    {
        wait_ticks = 100;
    }

    OS_Unlock_Global_Impl(idtype);
    vTaskDelay(wait_ticks);
    OS_Lock_Global_Impl(idtype);
}

/****************************************************************************************
                                INITIALIZATION FUNCTION
 ***************************************************************************************/

/*---------------------------------------------------------------------------------------
   Name: OS_FreeRtos_TableMutex_Init

   Purpose: Initialize the tables that the OS API uses to keep track of information
            about objects

   returns: OS_SUCCESS or OS_ERROR
---------------------------------------------------------------------------------------*/
int32 OS_FreeRTOS_TableMutex_Init(osal_objtype_t idtype)
{
    int32 return_code = OS_SUCCESS;
    if(idtype < MUTEX_TABLE_SIZE && MUTEX_TABLE[idtype] != NULL) {
        // TODO some tuning required for mutexes: could use xSemaphoreCreateMutexStatic to define specific RAM address
        // TODO could use binary semaphore insttead of mutex, if interrupts can access mutex
        *MUTEX_TABLE[idtype] = xSemaphoreCreateMutex();
        if(*MUTEX_TABLE[idtype] == NULL) {
            OS_DEBUG("Error: xSemaphoreCreateMutex failed: %d\n", idtype);
            return OS_ERROR;
        }
        if(xSemaphoreTake(*MUTEX_TABLE[idtype], (TickType_t) 0) == pdTRUE) {
            if(xSemaphoreGive(*MUTEX_TABLE[idtype]) != pdTRUE) {
                return OS_ERROR;
            }
        } else {
            return OS_ERROR;
        }
    }
    return return_code;
}
