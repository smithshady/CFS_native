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
 */

#ifndef OS_IMPL_COUNTSEM_H
#define OS_IMPL_COUNTSEM_H

#include <osconfig.h>
#include "os-freertos.h"
#include "os-shared-idmap.h"
#include "os-shared-countsem.h"

// TODO verify value
// UBaseType_t is defined in portmacro.h
// Could be adjust smaller or large, currently limit of signed long
#define MAX_SEM_VALUE 0x7FFFFFFF

typedef struct
{
    SemaphoreHandle_t id;
} OS_impl_countsem_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_countsem_internal_record_t OS_impl_count_sem_table[OS_MAX_COUNT_SEMAPHORES];

#endif /* OS_IMPL_COUNTSEM_H */
