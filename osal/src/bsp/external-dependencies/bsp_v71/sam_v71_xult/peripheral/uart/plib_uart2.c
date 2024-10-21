/*******************************************************************************
  UART2 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart2.c

  Summary:
    UART2 PLIB Implementation File

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
#include "plib_uart2.h"

// *****************************************************************************
// *****************************************************************************
// Section: UART2 Implementation
// *****************************************************************************
// *****************************************************************************

static uint16_t error_count = 0;
static UART_ERROR uart_error = UART_ERROR_NONE;
static uint32_t error_status = 0x00;

static UART_CALLBACK prev_rxCallback = NULL;
static UART_CALLBACK prev_txCallback = NULL;

UART_OBJECT uart2Obj;

static void UART2_ISR_RX_Handler( void ) {

    static bool message_start = false;
    static bool message_complete = false;
    static bool message_skip = false;
    
    /* While message in register */
    while (UART_SR_RXRDY_Msk == (UART2_REGS->UART_SR& UART_SR_RXRDY_Msk)) {

        /* Obtain byte from buffer and increment buffer size*/
        uint8_t c = (UART2_REGS->UART_RHR& UART_RHR_RXCHR_Msk);
        if(255 <= uart2Obj.rxProcessedSize) {
            memset(uart2Obj.rx_data_buffer, 0, 256); 
            uart2Obj.rxProcessedSize = 0;
            message_start = false;
            message_complete = false;
        }
        uart2Obj.rx_data_buffer[uart2Obj.rxProcessedSize] = c;
        uart2Obj.rxProcessedSize++;
                
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
        uart2Obj.rxBusyStatus = false;

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART2_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        /* Call callback function */
        if(uart2Obj.rxCallback != NULL) {
            uart2Obj.rxCallback(uart2Obj.rxContext);
        }
        
    }

    UART2_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);

}

static void UART2_ISR_TX_Handler( void )
{
    if(uart2Obj.txBusyStatus == true)
    {
        while((UART_SR_TXRDY_Msk == (UART2_REGS->UART_SR & UART_SR_TXRDY_Msk)) && (uart2Obj.txSize > uart2Obj.txProcessedSize) )
        {
            UART2_REGS->UART_THR|= uart2Obj.txBuffer[uart2Obj.txProcessedSize++];
        }

        /* Check if the buffer is done */
        if(uart2Obj.txProcessedSize >= uart2Obj.txSize)
        {
            uart2Obj.txBusyStatus = false;
            UART2_REGS->UART_IDR = UART_IDR_TXEMPTY_Msk;

            if(uart2Obj.txCallback != NULL)
            {
                uart2Obj.txCallback(uart2Obj.txContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
        ;
    }
}

void UART2_InterruptHandler( void )
{
    /* Error status */
    uint32_t errorStatus = (UART2_REGS->UART_SR & (UART_SR_OVRE_Msk | UART_SR_FRAME_Msk | UART_SR_PARE_Msk));

    if(errorStatus != 0)
    {

        /* Client must call UARTx_ErrorGet() function to clear the errors */
        error_count++;
        error_status = errorStatus;

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART2_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);
        uart2Obj.rxBusyStatus = false;

        uart_error = UART2_ErrorGet();

        /* Enable Read, Overrun, Parity and Framing error interrupts */
        UART2_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);

    }

    /* Receiver status */
    if(UART_SR_RXRDY_Msk == (UART2_REGS->UART_SR & UART_SR_RXRDY_Msk))
    {
        UART2_ISR_RX_Handler();
    }

    /* Transmitter status */
    if(UART_SR_TXRDY_Msk == (UART2_REGS->UART_SR & UART_SR_TXRDY_Msk))
    {
        UART2_ISR_TX_Handler();
    }
}

static void UART2_ErrorClear( void )
{
    uint8_t dummyData = 0u;

    UART2_REGS->UART_CR = UART_CR_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( UART_SR_RXRDY_Msk == (UART2_REGS->UART_SR & UART_SR_RXRDY_Msk) )
    {
        dummyData = (UART2_REGS->UART_RHR & UART_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;
}

void UART2_Initialize( void )
{
    /* Reset UART2 */
    UART2_REGS->UART_CR = (UART_CR_RSTRX_Msk | UART_CR_RSTTX_Msk | UART_CR_RSTSTA_Msk);

    /* Enable UART2 */
    UART2_REGS->UART_CR = (UART_CR_TXEN_Msk | UART_CR_RXEN_Msk);

    /* Configure UART2 mode */
    UART2_REGS->UART_MR = ((UART_MR_BRSRCCK_PERIPH_CLK) | (UART_MR_PAR_NO) | (0 << UART_MR_FILTER_Pos));

    /* Configure UART2 Baud Rate */
    //! Keeping comment in due to possible hardware contraint
    // UART2_REGS->UART_BRGR = UART_BRGR_CD(40); 115200
    UART2_REGS->UART_BRGR = UART_BRGR_CD(488); // 9600

    /* Initialize instance object */
    uart2Obj.rxBuffer = NULL;
    uart2Obj.rxSize = 0;
    uart2Obj.rxProcessedSize = 0;
    uart2Obj.rxBusyStatus = false;
    uart2Obj.rxCallback = prev_rxCallback;
    uart2Obj.txBuffer = NULL;
    uart2Obj.txSize = 0;
    uart2Obj.txProcessedSize = 0;
    uart2Obj.txBusyStatus = false;
    uart2Obj.txCallback = prev_txCallback;
    uart2Obj.isEnabled = true;

    /* Enable Read */
    UART2_REGS->UART_IER = (UART_IER_RXRDY_Msk);

    /* Clear error during initialization */
    UART2_ErrorGet();
    
    /* Enable Read, Overrun, Parity and Framing error interrupts */
    UART2_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);

}

void UART2_Stop( void )
{
    /* Mark instance object as disabled */
    uart2Obj.isEnabled = false;

    /* Disable Read, Overrun, Parity and Framing error interrupts*/
    UART2_REGS->UART_IER = (0x00);
    
    /* Disable UART2 */
    UART2_REGS->UART_CR = (UART_CR_TXDIS_Msk | UART_CR_RXDIS_Msk);

    /* Store previous RX/TX callback function pointers.
     * Necessary in order to restore upon initialization */
    prev_rxCallback = uart2Obj.rxCallback;
    prev_txCallback = uart2Obj.txCallback;
}

bool UART2_IsEnabled( void )
{
    return (uart2Obj.isEnabled);
}

UART_ERROR UART2_ErrorGet( void )
{

    /* Reset UART2 */
    UART2_REGS->UART_CR = (UART_CR_RSTRX_Msk | UART_CR_RSTTX_Msk | UART_CR_RSTSTA_Msk);

    /* Enable UART2 */
    UART2_REGS->UART_CR = (UART_CR_TXEN_Msk | UART_CR_RXEN_Msk);

    UART_ERROR errors = UART_ERROR_NONE;
    uint32_t status = UART2_REGS->UART_SR;

    errors = (UART_ERROR)(status & (UART_SR_OVRE_Msk | UART_SR_PARE_Msk | UART_SR_FRAME_Msk));

    if(errors != UART_ERROR_NONE)
    {
        UART2_ErrorClear();
    }

    /* All errors are cleared, but send the previous error state */
    return errors;
}

uint16_t UART2_Read( void *buffer, const size_t size )
{

    error_count = 0;

    uint16_t length = 0;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer) {
        /* Clear errors before submitting the request.
         * ErrorGet clears errors internally. */
        UART2_ErrorGet();
        
        /* Check if receive request is in progress */
        if(uart2Obj.rxBusyStatus == false)
        {
            length = uart2Obj.rxProcessedSize;
            uart2Obj.rxSize = size;
            uart2Obj.rxProcessedSize = 0;
            uart2Obj.rxBusyStatus = true;

            memcpy(buffer, uart2Obj.rx_data_buffer, 256);

            /* Enable Read, Overrun, Parity and Framing error interrupts */
            UART2_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);
        }
    }
    
    return length;
}

bool UART2_Write( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Check if transmit request is in progress */
        if(uart2Obj.txBusyStatus == false)
        {
            uart2Obj.txBuffer = lBuffer;
            uart2Obj.txSize = size;
            uart2Obj.txProcessedSize = 0;
            uart2Obj.txBusyStatus = true;
            status = true;

            /* Initiate the transfer by sending first byte */
            if(UART_SR_TXRDY_Msk == (UART2_REGS->UART_SR & UART_SR_TXRDY_Msk))
            {
                UART2_REGS->UART_THR = (UART_THR_TXCHR(*lBuffer) & UART_THR_TXCHR_Msk);
                uart2Obj.txProcessedSize++;
            }

            UART2_REGS->UART_IER = UART_IER_TXEMPTY_Msk;
        }
    }

    return status;
}

void UART2_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart2Obj.txCallback = callback;

    uart2Obj.txContext = context;
}

void UART2_ReadCallbackRegister( UART_CALLBACK callback )
{
    uart2Obj.rxCallback = callback;
}

bool UART2_WriteIsBusy( void )
{
    return uart2Obj.txBusyStatus;
}

bool UART2_ReadIsBusy( void )
{
    return uart2Obj.rxBusyStatus;
}

size_t UART2_WriteCountGet( void )
{
    return uart2Obj.txProcessedSize;
}

size_t UART2_ReadCountGet( void )
{
    return uart2Obj.rxProcessedSize;
}

bool UART2_ReadAbort(void)
{
    if (uart2Obj.rxBusyStatus == true)
    {
        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART2_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        uart2Obj.rxBusyStatus = false;

        /* If required application should read the num bytes processed prior to calling the read abort API */
        uart2Obj.rxSize = uart2Obj.rxProcessedSize = 0;
    }

    return true;
}

