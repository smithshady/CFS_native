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
 *
 * Purpose:
 *   Header file for internal data to the freeRTOS BSP
 */

#ifndef GENERIC_FREERTOS_BSP_INTERNAL_H
#define GENERIC_FREERTOS_BSP_INTERNAL_H

/*
** OSAL includes
*/
#include "bsp-impl.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <semLib.h>

/*
** BSP types
*/
typedef struct
{
    SEM_ID AccessMutex;
    // TODO replace vx w/ appt. name
    VX_MUTEX_SEMAPHORE(AccessMutexMem);
} OS_BSP_GenericFreeRtosGlobalData_t;

/*
 * Global Data object
 */
extern OS_BSP_GenericFreeRtosGlobalData_t OS_BSP_GenericFreeRtosGlobal;

#endif /* GENERIC_FREERTOS_BSP_INTERNAL_H */
