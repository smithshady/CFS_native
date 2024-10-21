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

/* PSP coverage stub replacement for fppLib.h */
#ifndef PCS_FPPLIB_H
#define PCS_FPPLIB_H

#include "PCS_basetypes.h"

/* ----------------------------------------- */
/* constants normally defined in fppLib.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in fppLib.h */
/* ----------------------------------------- */

typedef struct
{
    unsigned long context[4];
} PCS_FP_CONTEXT;

/* ----------------------------------------- */
/* prototypes normally declared in fppLib.h */
/* ----------------------------------------- */
extern void PCS_fppSave(PCS_FP_CONTEXT *fpc);

#endif