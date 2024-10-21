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
 * @file
 *   This is the dispatch hdr file for the Command Ingest lab application.
 */
#ifndef CI_LAB_DECODE_H
#define CI_LAB_DECODE_H

/*
** Required header files...
*/
#include "common_types.h"
#include "cfe_sb_api_typedefs.h"

CFE_Status_t CI_LAB_GetInputBuffer(void **BufferOut, size_t *SizeOut);
CFE_Status_t CI_LAB_DecodeInputMessage(void *SourceBuffer, size_t SourceSize, CFE_SB_Buffer_t **DestBufferOut);

#endif
