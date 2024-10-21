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
 * \ingroup freertos 
 *
 * Purpose: This file contains definitions that are shared across the FreeRTOS 
 *          OSAL implementation.  This file is private to the FreeRTOS port and it
 *          may contain FreeRTOS-specific definitions.
 */

#ifndef OS_FREERTOS_H
#define OS_FREERTOS_H

/****************************************************************************************
                                    COMMON INCLUDE FILES
 ***************************************************************************************/

// FreeRTOS headers
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

#include "os-shared-common.h"
#include "os-shared-task.h"
#include "os-shared-queue.h"
#include "os-shared-printf.h"
#include "os-shared-timebase.h"
#include "os-shared-mutex.h"
#include "os-shared-countsem.h"
#include "os-shared-binsem.h"
#include "os-shared-globaldefs.h"

#include "osapi.h"
#include "common_types.h"

/****************************************************************************************
                                     DEFINES
 ***************************************************************************************/

/****************************************************************************************
                                    TYPEDEFS
 ***************************************************************************************/

/****************************************************************************************
                                   GLOBAL DATA
 ***************************************************************************************/

/****************************************************************************************
                       RTEMS IMPLEMENTATION FUNCTION PROTOTYPES
 ************************************************************************************/

int32 OS_FreeRTOS_TaskAPI_Impl_Init(void);
int32 OS_FreeRTOS_QueueAPI_Impl_Init(void);
int32 OS_FreeRTOS_BinSemAPI_Impl_Init(void);
int32 OS_FreeRTOS_CountSemAPI_Impl_Init(void);
int32 OS_FreeRTOS_MutexAPI_Impl_Init(void);
int32 OS_FreeRTOS_TimeBaseAPI_Impl_Init(void);
int32 OS_FreeRTOS_ModuleAPI_Impl_Init(void);
int32 OS_FreeRTOS_StreamAPI_Impl_Init(void);
int32 OS_FreeRTOS_DirAPI_Impl_Init(void);
int32 OS_FreeRTOS_FileSysAPI_Impl_Init(void);

int32 OS_FreeRTOS_TableMutex_Init(osal_objtype_t idtype);

#endif /* OS_FREERTOS_H */
