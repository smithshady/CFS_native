/*******************************************************************************
  USART0 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_usart0.c

  Summary:
    USART0 PLIB Implementation File

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

#include <string.h>

#include "device.h"
#include "plib_usart0.h"
#include "communication/comms/comms_utils/header/message_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: USART0 Implementation
// *****************************************************************************
// *****************************************************************************

static USART_CALLBACK prev_rxCallback = NULL;
static USART_CALLBACK prev_txCallback = NULL;

USART_OBJECT usart0Obj;

static void USART0_ISR_RX_Handler(void) {

    static bool message_start = false;
    static bool message_complete = false;
    static bool message_skip = false;

    while ((US_CSR_USART_RXRDY_Msk == (USART0_REGS->US_CSR& US_CSR_USART_RXRDY_Msk))) {

        uint8_t c = (USART0_REGS->US_RHR& US_RHR_RXCHR_Msk);
        usart0Obj.rx_data_buffer[usart0Obj.rxProcessedSize] = c;
        usart0Obj.rxProcessedSize++;

        if (message_skip){
            message_skip = false;
            continue;
        }
        
        if (c == 0xC0) {
            if (message_start) {
                message_complete = true;
            } else {
                message_start = true;
            }
        }
        
        if (c == 0xDB) {
            message_skip = true;
        }

    }

    if (message_complete) {
        
        message_complete = false;
        message_start = false;
        
        /* Reset message parameters */
        usart0Obj.rxBusyStatus = false;

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        USART0_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);

        /* Call callback function */
        if(usart0Obj.rxCallback != NULL) {
            usart0Obj.rxCallback(usart0Obj.rxContext);
        }
        
    }

}

static void USART0_ISR_TX_Handler( void )
{
    if(usart0Obj.txBusyStatus == true)
    {
        while((US_CSR_USART_TXEMPTY_Msk == (USART0_REGS->US_CSR& US_CSR_USART_TXEMPTY_Msk)) && (usart0Obj.txSize > usart0Obj.txProcessedSize) )
        {
            USART0_REGS->US_THR|= usart0Obj.txBuffer[usart0Obj.txProcessedSize++];
        }

        /* Check if the buffer is done */
        if(usart0Obj.txProcessedSize >= usart0Obj.txSize)
        {
            usart0Obj.txBusyStatus = false;
            USART0_REGS->US_IDR = US_IDR_USART_TXEMPTY_Msk;

            if(usart0Obj.txCallback != NULL)
            {
                usart0Obj.txCallback(usart0Obj.txContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
        ;
    }
}

void USART0_InterruptHandler( void )
{
    /* Error status */
    uint32_t errorStatus = (USART0_REGS->US_CSR & (US_CSR_USART_OVRE_Msk | US_CSR_USART_FRAME_Msk | US_CSR_USART_PARE_Msk));

    if(errorStatus != 0)
    {
        /* Client must call USARTx_ErrorGet() function to clear the errors */

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        USART0_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);

        usart0Obj.rxBusyStatus = false;

    }

    /* Receiver status */
    if(US_CSR_USART_RXRDY_Msk == (USART0_REGS->US_CSR & US_CSR_USART_RXRDY_Msk))
    {
        USART0_ISR_RX_Handler();
    }

    /* Transmitter status */
    if(US_CSR_USART_TXRDY_Msk == (USART0_REGS->US_CSR & US_CSR_USART_TXRDY_Msk))
    {
        USART0_ISR_TX_Handler();
    }
}

static void USART0_ErrorClear( void )
{
    uint8_t dummyData = 0u;

    USART0_REGS->US_CR = US_CR_USART_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( US_CSR_USART_RXRDY_Msk == (USART0_REGS->US_CSR & US_CSR_USART_RXRDY_Msk) )
    {
        dummyData = (USART0_REGS->US_RHR & US_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;
}

void USART0_Initialize( void )
{

    /* Reset USART0 */
    USART0_REGS->US_CR = (US_CR_USART_RSTRX_Msk | US_CR_USART_RSTTX_Msk | US_CR_USART_RSTSTA_Msk);

    /* Enable USART0 */
    USART0_REGS->US_CR = (US_CR_USART_TXEN_Msk | US_CR_USART_RXEN_Msk);

    /* Configure USART0 mode */
    USART0_REGS->US_MR = (US_MR_USART_USCLKS_MCK | US_MR_USART_CHRL_8_BIT | US_MR_USART_PAR_NO | US_MR_USART_NBSTOP_1_BIT | (0 << US_MR_USART_OVER_Pos));

    /* Configure USART0 Baud Rate */
    USART0_REGS->US_BRGR = US_BRGR_CD(40);

    /* Initialize instance object */
    usart0Obj.rxBuffer = NULL;
    usart0Obj.rxSize = 0;
    usart0Obj.rxProcessedSize = 0;
    usart0Obj.rxBusyStatus = false;
    usart0Obj.rxCallback = prev_rxCallback;
    usart0Obj.txBuffer = NULL;
    usart0Obj.txSize = 0;
    usart0Obj.txProcessedSize = 0;
    usart0Obj.txBusyStatus = false;
    usart0Obj.txCallback = prev_txCallback;
    usart0Obj.isEnabled = true;

    /* Enable Read */
    USART0_REGS->US_IER = (US_IER_USART_RXRDY_Msk);

    /* Clear error during initialization */
    USART0_ErrorGet();
    USART0_Read(NULL, 1);
}

void USART0_Stop( void )
{
    /* Mark instance object as disabled */
    usart0Obj.isEnabled = false;

    /* Disable Read */
    USART0_REGS->US_IER = (US_IER_USART_RXRDY(0x00));

    /* Disable USART0 */
    USART0_REGS->US_CR = (US_CR_USART_TXDIS_Msk | US_CR_USART_RXDIS_Msk);

    /* Store previous RX/TX callback function pointers.
     * Necessary in order to restore upon initialization */
    prev_rxCallback = usart0Obj.rxCallback;
    prev_txCallback = usart0Obj.txCallback;
}

bool USART0_IsEnabled( void )
{
    return (usart0Obj.isEnabled);
}

USART_ERROR USART0_ErrorGet( void )
{
    USART_ERROR errors = USART_ERROR_NONE;
    uint32_t status = USART0_REGS->US_CSR;

    errors = (USART_ERROR)(status & (US_CSR_USART_OVRE_Msk | US_CSR_USART_PARE_Msk | US_CSR_USART_FRAME_Msk));

    if(errors != USART_ERROR_NONE)
    {
        USART0_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool USART0_SerialSetup( USART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{

    uint32_t brgVal = 0;
    uint32_t overSampVal = 0;
    uint32_t usartMode;
    bool status = false;

    if((usart0Obj.rxBusyStatus == true) || (usart0Obj.txBusyStatus == true))
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }

    if (setup != NULL)
    {
        uint32_t baud = setup->baudRate;
        if(srcClkFreq == 0)
        {
            srcClkFreq = USART0_FrequencyGet();
        }

        /* Calculate BRG value */
        if (srcClkFreq >= (16 * baud))
        {
            brgVal = (srcClkFreq / (16 * baud));
        }
        else if (srcClkFreq >= (8 * baud))
        {
            brgVal = (srcClkFreq / (8 * baud));
            overSampVal = US_MR_USART_OVER(1);
        }
        else
        {
            return false;
        }

        if (brgVal > 65535)
        {
            /* The requested baud is so low that the ratio of srcClkFreq to baud exceeds the 16-bit register value of CD register */
            return false;
        }

        /* Configure USART0 mode */
        usartMode = USART0_REGS->US_MR;
        usartMode &= ~(US_MR_USART_CHRL_Msk | US_MR_USART_MODE9_Msk | US_MR_USART_PAR_Msk | US_MR_USART_NBSTOP_Msk | US_MR_USART_OVER_Msk);
        USART0_REGS->US_MR = usartMode | ((uint32_t)setup->dataWidth | (uint32_t)setup->parity | (uint32_t)setup->stopBits | (uint32_t)overSampVal);

        /* Configure USART0 Baud Rate */
        USART0_REGS->US_BRGR = US_BRGR_CD(brgVal);
        status = true;
    }

    return status;
}

uint8_t USART0_Read( void *buffer, const size_t size )
{
    uint8_t length = 0;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Clear errors before submitting the request.
         * ErrorGet clears errors internally. */
        USART0_ErrorGet();

        /* Check if receive request is in progress */
        if(usart0Obj.rxBusyStatus == false) {
            length = usart0Obj.rxProcessedSize;
            usart0Obj.rxSize = size;
            usart0Obj.rxProcessedSize = 0;
            usart0Obj.rxBusyStatus = true;

            memcpy(buffer, usart0Obj.rx_data_buffer, MESSAGE_SIZE);

            /* Enable Read, Overrun, Parity and Framing error interrupts */
            USART0_REGS->US_IER = (US_IER_USART_RXRDY_Msk | US_IER_USART_FRAME_Msk | US_IER_USART_PARE_Msk | US_IER_USART_OVRE_Msk);
        }
    }

    return length;
}

bool USART0_Write( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Check if transmit request is in progress */
        if(usart0Obj.txBusyStatus == false)
        {
            usart0Obj.txBuffer = lBuffer;
            usart0Obj.txSize = size;
            usart0Obj.txProcessedSize = 0;
            usart0Obj.txBusyStatus = true;
            status = true;

            /* Initiate the transfer by sending first byte */
            if(US_CSR_USART_TXRDY_Msk == (USART0_REGS->US_CSR & US_CSR_USART_TXRDY_Msk))
            {
                USART0_REGS->US_THR = (US_THR_TXCHR(*lBuffer) & US_THR_TXCHR_Msk);
                usart0Obj.txProcessedSize++;
            }

            USART0_REGS->US_IER = US_IER_USART_TXEMPTY_Msk;
        }
    }

    return status;
}


void USART0_WriteCallbackRegister( USART_CALLBACK callback, uintptr_t context )
{
    usart0Obj.txCallback = callback;

    usart0Obj.txContext = context;
}

void USART0_ReadCallbackRegister( USART_CALLBACK callback, uintptr_t context )
{
    usart0Obj.rxCallback = callback;

    usart0Obj.rxContext = context;
}

bool USART0_WriteIsBusy( void )
{
    return usart0Obj.txBusyStatus;
}

bool USART0_ReadIsBusy( void )
{
    return usart0Obj.rxBusyStatus;
}

bool USART0_ReadAbort(void)
{
    if (usart0Obj.rxBusyStatus == true)
    {        
        /* Disable Read, Overrun, Parity and Framing error interrupts */
        USART0_REGS->US_IDR = (US_IDR_USART_RXRDY_Msk | US_IDR_USART_FRAME_Msk | US_IDR_USART_PARE_Msk | US_IDR_USART_OVRE_Msk);	
        
        usart0Obj.rxBusyStatus = false;                
        
        /* If required application should read the num bytes processed prior to calling the read abort API */        
        usart0Obj.rxSize = usart0Obj.rxProcessedSize = 0;
    }
    
    return true;
}

size_t USART0_WriteCountGet( void )
{
    return usart0Obj.txProcessedSize;
}

size_t USART0_ReadCountGet( void )
{
    return usart0Obj.rxProcessedSize;
}

