/*******************************************************************************
  USART0 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_usart0.h

  Summary:
    USART0 PLIB Header File

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

#ifndef PLIB_USART0_H
#define PLIB_USART0_H

#include "plib_usart_common.h"

// *****************************************************************************
// *****************************************************************************
// Section: Interface
// *****************************************************************************
// *****************************************************************************

#define USART0_FrequencyGet()    (uint32_t)(75000000UL)

/****************************** USART0 API *********************************/

void USART0_Initialize( void );

void USART0_Stop( void );

bool USART0_IsEnabled( void );

USART_ERROR USART0_ErrorGet( void );

bool USART0_SerialSetup( USART_SERIAL_SETUP *setup, uint32_t srcClkFreq );

bool USART0_Write( void *buffer, const size_t size );

uint8_t USART0_Read( void *buffer, const size_t size );

bool USART0_WriteIsBusy( void );

bool USART0_ReadIsBusy( void );

size_t USART0_WriteCountGet( void );

size_t USART0_ReadCountGet( void );

bool USART0_ReadAbort( void );

void USART0_WriteCallbackRegister( USART_CALLBACK callback, uintptr_t context );

void USART0_ReadCallbackRegister( USART_CALLBACK callback, uintptr_t context );

// DOM-IGNORE-END
#endif // PLIB_USART0_H
