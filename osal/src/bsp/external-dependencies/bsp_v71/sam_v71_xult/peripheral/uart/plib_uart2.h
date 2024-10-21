/*******************************************************************************
  UART2 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart2.h

  Summary:
    UART2 PLIB Header File

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

#ifndef PLIB_UART2_H
#define PLIB_UART2_H

#include "plib_uart_common.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

#define UART2_FrequencyGet()    (uint32_t)(75000000UL)

/****************************** UART2 API *********************************/

void UART2_Initialize( void );

void UART2_Stop( void );

bool UART2_IsEnabled( void );

UART_ERROR UART2_ErrorGet( void );

bool UART2_Write( void *buffer, const size_t size );

uint16_t UART2_Read( void *buffer, const size_t size );

bool UART2_WriteIsBusy( void );

bool UART2_ReadIsBusy( void );

size_t UART2_WriteCountGet( void );

size_t UART2_ReadCountGet( void );

bool UART2_ReadAbort( void );

void UART2_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context );

void UART2_ReadCallbackRegister( UART_CALLBACK callback );

// DOM-IGNORE-END
#endif // PLIB_UART2_H
