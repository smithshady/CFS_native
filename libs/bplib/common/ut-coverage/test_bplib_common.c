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

/*
 * Includes
 */
#include "test_bplib_common.h"

void UtTest_Setup(void)
{
    UtTest_Add(TestBplibCommon_CRC_Execute, TestBplibCommon_CRC_Setup, NULL, "CRC");

    /* The RBT setup is only needed once, and serves for all RBT tests */
    UtTest_Add(NULL, TestBplibCommon_RBT_Setup, NULL, "RB Tree Setup");
    UtTest_Add(TestBplibCommon_RBT_Basics, NULL, NULL, "RB Tree Basic");
    UtTest_Add(TestBplibCommon_RBT_LeafNodeInsertDelete, NULL, NULL, "RB Tree Leaf Node Insert/Delete");
    UtTest_Add(TestBplibCommon_RBT_NonLeafDelete, NULL, NULL, "RB Tree Non Leaf Delete");
    UtTest_Add(TestBplibCommon_RBT_Unique, NULL, NULL, "RB Tree Unique Indices");
    UtTest_Add(TestBplibCommon_RBT_NonUnique, NULL, NULL, "RB Tree NonUnique Indices");
    UtTest_Add(TestBplibCommon_RBT_Iterator, NULL, NULL, "RB Tree Iterator");
}
