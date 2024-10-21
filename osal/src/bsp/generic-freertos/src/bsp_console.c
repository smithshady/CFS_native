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
 * File:  bsp_console.c
 *
 * Purpose:
 *   OSAL BSP debug console abstraction
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "generic_freertos_bsp_internal.h"
#include "bsp-impl.h"

/****************************************************************************************
                    BSP CONSOLE IMPLEMENTATION FUNCTIONS
 ****************************************************************************************/

extern void PSP_Console_Write(char* buffer);

/*----------------------------------------------------------------
   OS_BSP_ConsoleOutput_Impl
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleOutput_Impl(const char *Str, size_t DataLen)
{
    while (DataLen > 0)
    {
        PSP_Console_Write((char*) Str);
    }
}

/*----------------------------------------------------------------
   OS_BSP_ConsoleSetMode_Impl() definition
   See full description in header
 ------------------------------------------------------------------*/
void OS_BSP_ConsoleSetMode_Impl(uint32 ModeBits)
{
    /* nop */
}
