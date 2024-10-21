/*******************************************************************************
  UART3 PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_uart3.c

  Summary:
    UART3 PLIB Implementation File

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
#include "plib_uart3.h"

// *****************************************************************************
// *****************************************************************************
// Section: UART3 Implementation
// *****************************************************************************
// *****************************************************************************

static UART_CALLBACK prev_rxCallback = NULL;
static UART_CALLBACK prev_txCallback = NULL;

UART_OBJECT uart3Obj;

static void UART3_ISR_RX_Handler( void ) {

    uint8_t incoming_byte = 0x00;
    while(UART_SR_RXRDY_Msk == (UART3_REGS->UART_SR& UART_SR_RXRDY_Msk)) {
        incoming_byte = (UART3_REGS->UART_RHR& UART_RHR_RXCHR_Msk);
    }

    uart3Obj.rxBusyStatus = false;

    /* Disable Read, Overrun, Parity and Framing error interrupts */
    UART3_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

    if(uart3Obj.rxCallback != NULL) {
        uart3Obj.rxCallback(incoming_byte);
    }

    /* Enable Read, Overrun, Parity and Framing error interrupts */
    UART3_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);

}

static void UART3_ISR_TX_Handler( void )
{
    if(uart3Obj.txBusyStatus == true)
    {
        while((UART_SR_TXRDY_Msk == (UART3_REGS->UART_SR & UART_SR_TXRDY_Msk)) && (uart3Obj.txSize > uart3Obj.txProcessedSize) )
        {
            UART3_REGS->UART_THR|= uart3Obj.txBuffer[uart3Obj.txProcessedSize++];
        }

        /* Check if the buffer is done */
        if(uart3Obj.txProcessedSize >= uart3Obj.txSize)
        {
            uart3Obj.txBusyStatus = false;
            UART3_REGS->UART_IDR = UART_IDR_TXEMPTY_Msk;

            if(uart3Obj.txCallback != NULL)
            {
                uart3Obj.txCallback(uart3Obj.txContext);
            }
        }
    }
    else
    {
        /* Nothing to process */
        ;
    }
}

void UART3_InterruptHandler( void )
{
    /* Error status */
    uint32_t errorStatus = (UART3_REGS->UART_SR & (UART_SR_OVRE_Msk | UART_SR_FRAME_Msk | UART_SR_PARE_Msk));

    if(errorStatus != 0)
    {
        /* Client must call UARTx_ErrorGet() function to clear the errors */

        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART3_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        uart3Obj.rxBusyStatus = false;

        /* UART errors are normally associated with the receiver, hence calling
         * receiver callback */
        if( uart3Obj.rxCallback != NULL )
        {
            uart3Obj.rxCallback(0x00);
        }
    }

    /* Receiver status */
    if(UART_SR_RXRDY_Msk == (UART3_REGS->UART_SR & UART_SR_RXRDY_Msk))
    {
        UART3_ISR_RX_Handler();
    }

    /* Transmitter status */
    if(UART_SR_TXRDY_Msk == (UART3_REGS->UART_SR & UART_SR_TXRDY_Msk))
    {
        UART3_ISR_TX_Handler();
    }
}

static void UART3_ErrorClear( void )
{
    uint8_t dummyData = 0u;

    UART3_REGS->UART_CR = UART_CR_RSTSTA_Msk;

    /* Flush existing error bytes from the RX FIFO */
    while( UART_SR_RXRDY_Msk == (UART3_REGS->UART_SR & UART_SR_RXRDY_Msk) )
    {
        dummyData = (UART3_REGS->UART_RHR & UART_RHR_RXCHR_Msk);
    }

    /* Ignore the warning */
    (void)dummyData;
}

void UART3_Initialize( void )
{
    /* Reset UART3 */
    UART3_REGS->UART_CR = (UART_CR_RSTRX_Msk | UART_CR_RSTTX_Msk | UART_CR_RSTSTA_Msk);

    /* Enable UART3 */
    UART3_REGS->UART_CR = (UART_CR_TXEN_Msk | UART_CR_RXEN_Msk);

    /* Configure UART3 mode */
    UART3_REGS->UART_MR = ((UART_MR_BRSRCCK_PERIPH_CLK) | (UART_MR_PAR_NO) | (0 << UART_MR_FILTER_Pos));

    /* Configure UART3 Baud Rate */
    UART3_REGS->UART_BRGR = UART_BRGR_CD(40);

    /* Initialize instance object */
    uart3Obj.rxBuffer = NULL;
    uart3Obj.rxSize = 0;
    uart3Obj.rxProcessedSize = 0;
    uart3Obj.rxBusyStatus = false;
    uart3Obj.rxCallback = prev_rxCallback;
    uart3Obj.txBuffer = NULL;
    uart3Obj.txSize = 0;
    uart3Obj.txProcessedSize = 0;
    uart3Obj.txBusyStatus = false;
    uart3Obj.txCallback = prev_txCallback;
    uart3Obj.isEnabled = true;

    /* Enable Read, Overrun, Parity and Framing error interrupts */
    UART3_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);

}

void UART3_Stop( void )
{
    /* Mark instance object as disabled */
    uart3Obj.isEnabled = false;

    /* Disable Read, Overrun, Parity and Framing error interrupts */
    UART3_REGS->UART_IER = (0x00);
    
    /* Disable UART3 */
    UART3_REGS->UART_CR = (UART_CR_TXDIS_Msk | UART_CR_RXDIS_Msk);

    /* Store previous RX/TX callback function pointers.
     * Necessary in order to restore upon initialization */
    prev_rxCallback = uart3Obj.rxCallback;
    prev_txCallback = uart3Obj.txCallback;
}

bool UART3_IsEnabled( void )
{
    return (uart3Obj.isEnabled);
}

UART_ERROR UART3_ErrorGet( void )
{
    UART_ERROR errors = UART_ERROR_NONE;
    uint32_t status = UART3_REGS->UART_SR;

    errors = (UART_ERROR)(status & (UART_SR_OVRE_Msk | UART_SR_PARE_Msk | UART_SR_FRAME_Msk));

    if(errors != UART_ERROR_NONE)
    {
        UART3_ErrorClear();
    }

    UART3_REGS->UART_IER = (UART_IER_RXRDY_Msk | UART_IER_FRAME_Msk | UART_IER_PARE_Msk | UART_IER_OVRE_Msk);

    /* All errors are cleared, but send the previous error state */
    return errors;
}

bool UART3_SerialSetup( UART_SERIAL_SETUP *setup, uint32_t srcClkFreq )
{
    bool status = false;
    uint32_t brgVal = 0;
    uint32_t uartMode;

    if((uart3Obj.rxBusyStatus == true) || (uart3Obj.txBusyStatus == true))
    {
        /* Transaction is in progress, so return without updating settings */
        return false;
    }
    if (setup != NULL)
    {
        uint32_t baud = setup->baudRate;
        if(srcClkFreq == 0)
        {
            srcClkFreq = UART3_FrequencyGet();
        }

        /* Calculate BRG value */
        brgVal = srcClkFreq / (16 * baud);

        /* If the target baud rate is acheivable using this clock */
        if (brgVal <= 65535)
        {
            /* Configure UART3 mode */
            uartMode = UART3_REGS->UART_MR;
            uartMode &= ~UART_MR_PAR_Msk;
            UART3_REGS->UART_MR = uartMode | setup->parity ;

            /* Configure UART3 Baud Rate */
            UART3_REGS->UART_BRGR = UART_BRGR_CD(brgVal);

            status = true;
        }
    }

    return status;
}

bool UART3_Write( void *buffer, const size_t size )
{
    bool status = false;
    uint8_t * lBuffer = (uint8_t *)buffer;

    if(NULL != lBuffer)
    {
        /* Check if transmit request is in progress */
        if(uart3Obj.txBusyStatus == false)
        {
            uart3Obj.txBuffer = lBuffer;
            uart3Obj.txSize = size;
            uart3Obj.txProcessedSize = 0;
            uart3Obj.txBusyStatus = true;
            status = true;

            /* Initiate the transfer by sending first byte */
            if(UART_SR_TXRDY_Msk == (UART3_REGS->UART_SR & UART_SR_TXRDY_Msk))
            {
                UART3_REGS->UART_THR = (UART_THR_TXCHR(*lBuffer) & UART_THR_TXCHR_Msk);
                uart3Obj.txProcessedSize++;
            }

            UART3_REGS->UART_IER = UART_IER_TXEMPTY_Msk;
        }
    }

    return status;
}

void UART3_WriteCallbackRegister( UART_CALLBACK callback, uintptr_t context )
{
    uart3Obj.txCallback = callback;

    uart3Obj.txContext = context;
}

void UART3_ReadCallbackRegister( UART_CALLBACK callback )
{
    uart3Obj.rxCallback = callback;

    uart3Obj.rxContext = 0;
}

bool UART3_WriteIsBusy( void )
{
    return uart3Obj.txBusyStatus;
}

bool UART3_ReadIsBusy( void )
{
    return uart3Obj.rxBusyStatus;
}

size_t UART3_WriteCountGet( void )
{
    return uart3Obj.txProcessedSize;
}

size_t UART3_ReadCountGet( void )
{
    return uart3Obj.rxProcessedSize;
}

bool UART3_ReadAbort(void)
{
    if (uart3Obj.rxBusyStatus == true)
    {
        /* Disable Read, Overrun, Parity and Framing error interrupts */
        UART3_REGS->UART_IDR = (UART_IDR_RXRDY_Msk | UART_IDR_FRAME_Msk | UART_IDR_PARE_Msk | UART_IDR_OVRE_Msk);

        uart3Obj.rxBusyStatus = false;

        /* If required application should read the num bytes processed prior to calling the read abort API */
        uart3Obj.rxSize = uart3Obj.rxProcessedSize = 0;
    }

    return true;
}

