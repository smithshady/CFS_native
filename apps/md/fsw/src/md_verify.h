/************************************************************************
 * NASA Docket No. GSC-18,922-1, and identified as “Core Flight
 * System (cFS) Memory Dwell Application Version 2.4.1”
 *
 * Copyright (c) 2021 United States Government as represented by the
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
 * @file
 *   Contains CFS Memory Dwell macros that run preprocessor checks
 *   on mission configurable parameters
 */
#ifndef MD_VERIFY_H
#define MD_VERIFY_H

#include "cfe_platform_cfg.h"
#include "md_platform_cfg.h"

/*************************************************************************
 * Macro Definitions
 *************************************************************************/

/*
 * Number of memory dwell tables.
 * Max limitation is restricted by size of telemetry variable (16 bits) used to
 * report dwell stream enable statuses, the size of the mask used for
 * start and stop commands, and the size of variables used in code.
 */
#if MD_NUM_DWELL_TABLES < 1
#error MD_NUM_DWELL_TABLES must be at least one.
#elif MD_NUM_DWELL_TABLES > 16
#error MD_NUM_DWELL_TABLES cannot be greater than 16.
#endif

/*
 * If MD_DWELL_TABLE_SIZE is too large, the table load structure
 *  will be too large for Table Services
 */
#if MD_DWELL_TABLE_SIZE < 1
#error MD_DWELL_TABLE_SIZE must be at least one.
#elif MD_DWELL_TABLE_SIZE > 65535
#error MD_DWELL_TABLE_SIZE cannot be greater than 65535.
#endif

#if (MD_ENFORCE_DWORD_ALIGN != 0) && (MD_ENFORCE_DWORD_ALIGN != 1)
#error MD_ENFORCE_DWORD_ALIGN must be 0 or 1.
#endif

#if (MD_SIGNATURE_OPTION != 0) && (MD_SIGNATURE_OPTION != 1)
#error MD_SIGNATURE_OPTION must be 0 or 1.
#endif

#if (MD_SIGNATURE_FIELD_LENGTH % 4) != 0
#error MD_SIGNATURE_FIELD_LENGTH should be longword aligned
#elif MD_SIGNATURE_FIELD_LENGTH < 4
#error MD_SIGNATURE_FIELD_LENGTH cannot be less than 4.
#endif

#endif
