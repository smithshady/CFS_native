/*******************************************************************************
  NVIC PLIB Implementation

  Company:
    Microchip Technology Inc.

  File Name:
    plib_nvic.c

  Summary:
    NVIC PLIB Source File

  Description:
    None

*******************************************************************************/

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

#include "device.h"
#include "plib_nvic.h"


// *****************************************************************************
// *****************************************************************************
// Section: NVIC Implementation
// *****************************************************************************
// *****************************************************************************

void NVIC_Initialize( void )
{
    /* Priority 0 to 7 and no sub-priority. 0 is the highest priority */
    NVIC_SetPriorityGrouping( 0x04 );

    /* Enable NVIC Controller */
    __DMB();
    __enable_irq();

    /* Enable the interrupt sources and configure the priorities as configured
     * from within the "Interrupt Manager" of MHC. */
    NVIC_SetPriority(SysTick_IRQn, 7);
    NVIC_SetPriority(UART0_IRQn, 7);
    NVIC_EnableIRQ(UART0_IRQn);
    NVIC_SetPriority(UART1_IRQn, 7);
    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_SetPriority(UART3_IRQn, 7);
    NVIC_EnableIRQ(UART3_IRQn);
    NVIC_SetPriority(PIOA_IRQn, 7);
    NVIC_EnableIRQ(PIOA_IRQn);
    NVIC_SetPriority(PIOB_IRQn, 7);
    NVIC_EnableIRQ(PIOB_IRQn);
    NVIC_SetPriority(PIOC_IRQn, 7);
    NVIC_EnableIRQ(PIOC_IRQn);
    NVIC_SetPriority(USART0_IRQn, 7);
    NVIC_EnableIRQ(USART0_IRQn);
    NVIC_SetPriority(USART2_IRQn, 7);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(PIOD_IRQn, 7);
    NVIC_EnableIRQ(PIOD_IRQn);
    NVIC_SetPriority(MCAN1_INT0_IRQn, 7);
    NVIC_EnableIRQ(MCAN1_INT0_IRQn);
    NVIC_SetPriority(UART2_IRQn, 7);
    NVIC_EnableIRQ(UART2_IRQn);
    NVIC_SetPriority(UART3_IRQn, 7);
    NVIC_EnableIRQ(UART3_IRQn);
    NVIC_SetPriority(TC3_CH0_IRQn, 7);
    NVIC_EnableIRQ(TC3_CH0_IRQn);



}
