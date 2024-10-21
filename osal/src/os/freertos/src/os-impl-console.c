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

#include "os-freertos.h"
#include "os-shared-printf.h"
#include "os-shared-idmap.h"
#include "os-shared-common.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

#define OSAL_CONSOLE_STREAM stdout
/*
 * By default use the stdout stream for the console (OS_printf)
 */
#define OSAL_CONSOLE_FILENO STDOUT_FILENO

/*
 * By default the console output is always asynchronous
 * (equivalent to "OS_UTILITY_TASK_ON" being set)
 *
 * This option was removed from osconfig.h and now is
 * assumed to always be on.
 */
#define OS_CONSOLE_ASYNC          false // TODO check removed 
// TODO edit config gen for the below values
 //  /home/brett/dev/cFS-submoduled/roverlink/osal/default_config.cmake
#define OS_CONSOLE_TASK_PRIORITY  OS_UTILITYTASK_PRIORITY // TODO set to 2
#define OS_CONSOLE_TASK_STACKSIZE OS_UTILITYTASK_STACK_SIZE // TODO set to 1024

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/
/* Console device */
typedef struct
{
    bool is_async;
    SemaphoreHandle_t data_sem;
    TaskHandle_t task_handle;
} OS_impl_console_internal_record_t;

/* Tables where the OS object information is stored */
OS_impl_console_internal_record_t OS_impl_console_table[OS_MAX_CONSOLES];

/********************************************************************/
/*                 CONSOLE OUTPUT                                   */
/********************************************************************/

// TODO see OS_ConsoleOutput_Imple in os/portable/os-impl-console-bsp.c

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
void OS_ConsoleWakeup_Impl(const OS_object_token_t *token)
{
    OS_impl_console_internal_record_t *local;

    local = OS_OBJECT_TABLE_GET(OS_impl_console_table, *token);

    if(local->is_async) {
        xSemaphoreGive(local->data_sem);
    } else {
        OS_ConsoleOutput_Impl(token);
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Local helper routine, not part of OSAL API.
 *
 *-----------------------------------------------------------------*/
static void OS_ConsoleTask_Entry(void *pvParameters)
{
    // TODO token->obj_id passed as arg to match RTEMS impl
    // might be better to pass token as pvParameters param
    int32 arg = (int32) pvParameters;
    OS_object_token_t                  token;
    OS_impl_console_internal_record_t *local;

    if (OS_ObjectIdGetById(OS_LOCK_MODE_REFCOUNT, OS_OBJECT_TYPE_OS_CONSOLE, OS_ObjectIdFromInteger(arg), &token) ==
        OS_SUCCESS)
    {
        local = OS_OBJECT_TABLE_GET(OS_impl_console_table, token);

        /* Loop forever (unless shutdown is set) */
        while (OS_SharedGlobalVars.GlobalState != OS_SHUTDOWN_MAGIC_NUMBER)
        {
            OS_ConsoleOutput_Impl(&token);
            xSemaphoreTake(local->data_sem, portMAX_DELAY);
        }
        OS_ObjectIdRelease(&token);
    }
}

/*----------------------------------------------------------------
 *
 *  Purpose: Implemented per internal OSAL API
 *           See prototype for argument/return detail
 *
 *-----------------------------------------------------------------*/
int32 OS_ConsoleCreate_Impl(const OS_object_token_t *token)
{
    OS_impl_console_internal_record_t *local;
    OS_console_internal_record_t *     console;
    int32                              return_code;


    if (OS_ObjectIndexFromToken(token) != 0)
    {
        return OS_ERROR;
    }

    local   = OS_OBJECT_TABLE_GET(OS_impl_console_table, *token);
    local->is_async = OS_CONSOLE_ASYNC;
    
    // initialize BSP console support
    PSP_Console_Init();
    
    local->data_sem = xSemaphoreCreateBinary();
    xSemaphoreGive(local->data_sem);
    
    return_code = xTaskCreate(
        &OS_ConsoleTask_Entry,
        "console task",
        OS_CONSOLE_TASK_STACKSIZE,
        (void*) token->obj_id, // pvParameters
        OS_CONSOLE_TASK_PRIORITY,
        &local->task_handle // pxCreatedTask handle
    );

    // TODO FreeRTOS typical return is 1/pdTRUE, OSAL uses 0 for OS_SUCCESS, possible updates required
    return return_code;
}
