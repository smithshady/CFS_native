/*******************************************************************************
  UART1 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart1.c

  Summary:
    UART1 PLIB Implementation File

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
#include "plib_uart1.h"

// *****************************************************************************
// *****************************************************************************
// Section: UART1 Implementation
// *****************************************************************************
// *****************************************************************************

static UART_CALLBACK prev_rxCallback = NULL;
static UART_CALLBACK prev_txCallback = NULL;

UART_OBJECT uart1Obj;

static void UART1_ISR_RX_Handler( void ) {
    
    static bool message_start = false;
    static bool message_complete = false;
    static bool message_skip = false;
    
    /* While message in register */
    while (UART_SR_RXRDY_Msk == (UART1_REGS->UART_SR& UART_SR_RXRDY_Msk)) {

        /* Obtain byte from buffer and increment buffer size*/
        uint8_t c = (UART1_REGS->UART_RHR& UART_RHR_RXCHR_Msk);
        if(255 <= uart1Obj.rxProcessedSize) {
            memset(uart1Obj.rx_data_buffer, 0, 256); 
            uart1Obj.rxProcessedSize = 0;
            message_start = false;
            message_complete = false;
        }
        uart1Obj.rx_data_buffer[uart1Obj.rxProcessedSize] = c;
        uart1Obj.rxProcessedSize++;
                
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
        uart1Obj.rxBusyStatus = false;

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART1_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        /* Call callback function */
        if(uart1Obj.rxCallback != NULL) {
            uart1Obj.rxCallback(uart1Obj.rxContext);
        }
        
    }

}

static void UART1_ISR_TX_Handler( void )
{

    if(uart1Obj.txBusyStatus == true)
    {
        while((UART_SR_TXRDY_Msk == (UART1_REGS->UART_SR & UART_SR_TXRDY_Msk)) && (uart1Obj.txSize > uart1Obj.txProcessedSize) )
        {
            UART1_REGS->UART_THR|= uart1Obj.txBuffer[uart1Obj.txProcessedSize++];
        }

        /* Check if the buffer is done */
        if(uart1Obj.txProcessedSize >= uart1Obj.txSize)
        {
            uart1Obj.txBusyStatus = false;
            UART1_REGS->UART_IDR = UART_IDR_TXEMPTY_Msk;

            if(uart1Obj.txCallback != NULL)
            {
                uart1Obj.txCallback(uart1Obj.txContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
        ;
    }
}

void UART1_InterruptHandler( void ) {
    /* Error status */
    uint32_t errorStatus = (UART1_REGS->UART_SR & (UART_SR_OVRE_Msk | UART_SR_FRAME_Msk | UART_SR_PARE_Msk));

    if(errorStatus != 0)
    {
        /* Client must call UARTx_ErrorGet() function to clear the errors */

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART1_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        uart1Obj.rxBusyStatus = false;
        
        /* UART errors are normally associated with the receiver, hence calling
         * receiver callback */
        if( uart1Obj.rxCallback != NULL )
        {
            uart1Obj.rxCallback(uart1Obj.rxContext);
        }
    }

    /* Receiver status */
    if(UART_SR_RXRDY_Msk == (UART1_REGS->UART_SR & UART_SR_RXRDY_Msk))
    {
        UART1_ISR_RX_Handler();
    }

    /* Transmitter status */
    if(UART_SR_TXRDY_Msk == (UART1_REGS->UART_SR & UART_SR_TXRDY_Msk))
    {
        UART1_ISR_TX_Handler();
    }
}

static void UART1_ErrorClear( void ) {
    uint8_t dummyData = 0u;

    UART1_REGS->UART_CR = UART_CR_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( UART_SR_RXRDY_Msk == (UART1_REGS->UART_SR & UART_SR_RXRDY_Msk) )
    {
        dummyData = (UART1_REGS->UART_RHR & UART_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;
}

void UART1_Initialize( void ) {
    /* Reset UART1 */
    UART1_REGS->UART_CR = (UART_CR_RSTRX_Msk | UART_CR_RSTTX_Msk | UART_CR_RSTSTA_Msk);

    /* Enable UART1 */
    UART1_REGS->UART_CR = (UART_CR_TXEN_Msk | UART_CR_RXEN_Msk);

    /* Configure UART1 mode */
    UART1_REGS->UART_MR = ((UART_MR_BRSRCCK_PERIPH_CLK) | (UART_MR_PAR_NO) | (0 << UART_MR_FILTER_Pos));

    /* Configure UART1 Baud Rate */
    UART1_REGS->UART_BRGR = UART_BRGR_CD(40);

    /* Initialize instance object */
    uart1Obj.rxBuffer = NULL;
    uart1Obj.rxSize = 0;
    uart1Obj.rxProcessedSize = 0;
    uart1Obj.rxBusyStatus = false;
    uart1Obj.rxCallback = prev_rxCallback;
    uart1Obj.txBuffer = NULL;
    uart1Obj.txSize = 0;
    uart1Obj.txProcessedSize = 0;
    uart1Obj.txBusyStatus = false;
    uart1Obj.txCallback = prev_txCallback;
    uart1Obj.isEnabled = true;
    
    /* Enable Read */
    UART1_REGS->UART_IER = (UART_IER_RXRDY_Msk);

    /* Clear error during initialization */
    UART1_ErrorGet();
    UART1_Read(NULL, 1);
}

void UART1_Stop( void ) {

    /* Mark instance object as disabled */
    uart1Obj.isEnabled = false;

    /* Disable Read */
    UART1_REGS->UART_IER = (UART_IER_RXRDY(0x00));

    /* Disable UART1 */
    UART1_REGS->UART_CR = (UART_CR_TXDIS_Msk | UART_CR_RXDIS_Msk);

    /* Store previous RX/TX callback function pointers.
     * Necessary in order to restore upon initialization */
    prev_rxCallback = uart1Obj.rxCallback;
    prev_txCallback = uart1Obj.txCallback;
}

bool UART1_IsEnabled( void ) {
    return (uart1Obj.isEnabled);
}

UART_ERROR UART1_ErrorGet( void ) {

    /* Reset UART1 */
    UART1_REGS->UART_CR = (UART_CR_RSTRX_Msk | UART_CR_RSTTX_Msk | UART_CR_RSTSTA_Msk);

    /* Enable UART1 */
    UART1_REGS->UART_CR = (UART_CR_TXEN_Msk | UART_CR_RXEN_Msk);

    UART_ERROR errors = UART_ERROR_NONE;
    uint32_t status = UART1_REGS->UART_SR;

    errors = (UART_ERROR)(status & (UART_SR_OVRE_Msk | UART_SR_PARE_Msk | UART_SR_FRAME_Msk));

    if(errors != UART_ERROR_NONE)
    {
        UART1_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool UART1_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq ) {
    bool status = false;
    uint32_t brgVal = 0;
    uint32_t uartMode;

    if((uart1Obj.rxBusyStatus == true) || (uart1Obj.txBusyStatus == true))
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }
    if (setup != NULL)
    {
    	uint32_t baud = setup->baudRate;
        if(srcClkFreq == 0)
        {
            srcClkFreq = UART1_FrequencyGet();
        }

        /* Calculate BRG value */
        brgVal = srcClkFreq / (16 * baud);

        /* If the target baud rate is acheivable using this clock */
        if (brgVal <= 65535)
        {
            /* Configure UART1 mode */
            uartMode = UART1_REGS->UART_MR;
            uartMode &= ~UART_MR_PAR_Msk;
            UART1_REGS->UART_MR = uartMode | setup->parity ;

            /* Configure UART1 Baud Rate */
            UART1_REGS->UART_BRGR = UART_BRGR_CD(brgVal);

            status = true;
        }
    }

    return status;
}

uint8_t UART1_Read( void *buffer, const size_t size ) {
    
    uint8_t length = 0;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer) {
        /* Clear errors before submitting the request.
         * ErrorGet clears errors internally. */
        UART1_ErrorGet();
        
        /* Check if receive request is in progress */
        if(uart1Obj.rxBusyStatus == false)
        {
            length = uart1Obj.rxProcessedSize;
            uart1Obj.rxSize = size;
            uart1Obj.rxProcessedSize = 0;
            uart1Obj.rxBusyStatus = true;

            memcpy(buffer, uart1Obj.rx_data_buffer, 256);

            /* Enable Read, Overrun, Parity and Framing error interrupts */
            UART1_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);
        }
    }
    
    return length;
}

bool UART1_Write( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Check if transmit request is in progress */
        if(uart1Obj.txBusyStatus == false)
        {
            uart1Obj.txBuffer = lBuffer;
            uart1Obj.txSize = size;
            uart1Obj.txProcessedSize = 0;
            uart1Obj.txBusyStatus = true;
            status = true;

            /* Initiate the transfer by sending first byte */
            if(UART_SR_TXRDY_Msk == (UART1_REGS->UART_SR & UART_SR_TXRDY_Msk))
            {
                UART1_REGS->UART_THR = (UART_THR_TXCHR(*lBuffer) & UART_THR_TXCHR_Msk);
                uart1Obj.txProcessedSize++;
            }

            UART1_REGS->UART_IER = UART_IER_TXEMPTY_Msk;
        }
    }

    return status;
}

void UART1_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart1Obj.txCallback = callback;

    uart1Obj.txContext = context;
}

void UART1_ReadCallbackRegister( UART_CALLBACK callback, uintptr_t context, uintptr_t ctx )
{
    uart1Obj.rxCallback = callback;

    uart1Obj.rxContext = context;
    uart1Obj.ctx = ctx;
}

bool UART1_WriteIsBusy( void )
{
    return uart1Obj.txBusyStatus;
}

bool UART1_ReadIsBusy( void )
{
    return uart1Obj.rxBusyStatus;
}

size_t UART1_WriteCountGet( void )
{
    return uart1Obj.txProcessedSize;
}

size_t UART1_ReadCountGet( void )
{
    return uart1Obj.rxProcessedSize;
}

bool UART1_ReadAbort(void)
{
    if (uart1Obj.rxBusyStatus == true)
    {
        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART1_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        uart1Obj.rxBusyStatus = false;

        /* If required application should read the num bytes processed prior to calling the read abort API */
        uart1Obj.rxSize = uart1Obj.rxProcessedSize = 0;
    }

    return true;
}

uintptr_t UART1_GetCTX(void) {
    
    return uart1Obj.ctx;

}
