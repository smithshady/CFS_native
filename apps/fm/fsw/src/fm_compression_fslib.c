/************************************************************************
 * NASA Docket No. GSC-18,918-1, and identified as “Core Flight
 * Software System (cFS) File Manager Application Version 2.6.1”
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
 *  File Manager (FM) decompression facility via CFS "FS_Lib"
 *
 * Calls into CFS FS Lib to decompress file(s)
 * Compression is not implemented.
 */

#include <string.h>
#include <common_types.h>
#include <cfe_error.h>
// TODO fix cfs fs lib include
//#include <cfs_fs_lib.h>

#include "fm_app.h"
#include "fm_compression.h"

/**
 * @brief The state object for a compressor
 *
 * This is a wrapper around the object defined in CFS FS Lib
 */
struct FM_Decompressor_State
{
    uint8_t placeholder;
//    FS_LIB_Decompress_State_t LibState;
};

/**
 * @brief Instance of the decompressor state object
 *
 * A single instance is OK because this is only invoked
 * from a single thread.
 */
//static FM_Decompressor_State_t FM_FSLIB_DecompressState;

CFE_Status_t FM_CompressionService_Init(void)
{
    //memset(&FM_FSLIB_DecompressState, 0, sizeof(FM_FSLIB_DecompressState));
    //FM_GlobalData.DecompressorStatePtr = &FM_FSLIB_DecompressState;
    return CFE_SUCCESS;
}

CFE_Status_t FM_Decompress_Impl(FM_Decompressor_State_t *State, const char *SrcFileName, const char *DstFileName)
{
    return CFE_STATUS_NOT_IMPLEMENTED;//FS_LIB_Decompress(&State->LibState, SrcFileName, DstFileName);
}

CFE_Status_t FM_Compress_Impl(FM_Compressor_State_t *State, const char *SrcFileName, const char *DstFileName)
{
    return CFE_STATUS_NOT_IMPLEMENTED;
}
