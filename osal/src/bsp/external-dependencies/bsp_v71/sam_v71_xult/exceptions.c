/*******************************************************************************
  System Exceptions File

  File Name:
    exceptions.c

  Summary:
    This file contains a function which overrides the default _weak_ exception
    handlers provided by the interrupt.c file.

  Description:
    This file redefines the default _weak_  exception handler with a more debug
    friendly one. If an unexpected exception occurs the code will stop in a
    while(1) loop.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include "configuration.h"
#include "definitions.h"

#include "sram/sram.h"

// *****************************************************************************
// *****************************************************************************
// Section: Exception Handling Routine
// *****************************************************************************
// *****************************************************************************

/* Brief default interrupt handlers for core IRQs.*/

void __attribute__((noreturn)) NonMaskableInt_Handler(void)
{
#if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
    __builtin_software_breakpoint();
#endif

   SRAM_SetResetReason(RESET_NON_MASKABLE_INT);

    while (1) {
         printf("Non Maskable Int\r\n");
    }
}

void __attribute__((noreturn)) HardFault_Handler(void)
{
#if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
    __builtin_software_breakpoint();
#endif

   SRAM_SetResetReason(RESET_HARD_FAULT);

   while (1) {
      printf("------------------------------------Hard Fault------------------------------------\r\n");
      while(1); // Infinite wait
   }
}

void __attribute__((noreturn)) DebugMonitor_Handler(void)
{
#if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
   __builtin_software_breakpoint();
#endif

   SRAM_SetResetReason(RESET_DEBUG_MONITOR);

   while (1) {
      printf("Debug Monitor\r\n");
   }
}

void __attribute__((noreturn)) MemoryManagement_Handler(void)
{
#if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
   __builtin_software_breakpoint();
#endif

   SRAM_SetResetReason(RESET_MEMORY_MANAGEMENT);

   while (1) {
      printf("Memory Management\r\n");
   }
}

void __attribute__((noreturn)) BusFault_Handler(void)
{
#if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
   __builtin_software_breakpoint();
#endif

   SRAM_SetResetReason(RESET_BUS_FAULT);

   while (1) {
      printf("Bus Fault\r\n");
   }
}

void __attribute__((noreturn)) UsageFault_Handler(void)
{
#if defined(__DEBUG) || defined(__DEBUG_D) && defined(__XC32)
   __builtin_software_breakpoint();
#endif

   SRAM_SetResetReason(RESET_USAGE_FAULT);

   while (1) {
      printf("Usage Fault\r\n");
   }
}
/*******************************************************************************
 End of File
 */

