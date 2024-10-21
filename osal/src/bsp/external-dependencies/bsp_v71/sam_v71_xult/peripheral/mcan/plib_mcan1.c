/*******************************************************************************
  Controller Area Network (MCAN) Peripheral Library Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_mcan1.c

  Summary:
    MCAN peripheral library interface.

  Description:
    This file defines the interface to the MCAN peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_mcan1.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************
#define MCAN_STD_ID_Msk        0x7FF
#define MCAN_CALLBACK_TX_INDEX 3

static MCAN_RX_MSG mcan1RxMsg[2][32];
static MCAN_CALLBACK_OBJ mcan1CallbackObj[4];
static MCAN_OBJ mcan1Obj;

uint8_t Can1MessageRAM[MCAN1_MESSAGE_RAM_CONFIG_SIZE] __attribute__((aligned (32)))__attribute__((space(data), section (".ram_nocache")));

/******************************************************************************
Local Functions
******************************************************************************/

static uint8_t MCANDlcToLengthGet(uint8_t dlc)
{
    uint8_t msgLength[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
    return msgLength[dlc];
}

// *****************************************************************************
// *****************************************************************************
// MCAN1 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void MCAN1_Initialize(void)

   Summary:
    Initializes given instance of the MCAN peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void MCAN1_Initialize(void)
{
    /* Start MCAN initialization */
    MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_ENABLED;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) != MCAN_CCCR_INIT_Msk);

    /* Set CCE to unlock the configuration registers */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_CCE_Msk;

    /* Set Nominal Bit timing and Prescaler Register */
    MCAN1_REGS->MCAN_NBTP  = MCAN_NBTP_NBRP(19) | MCAN_NBTP_NSJW(0) | MCAN_NBTP_NTSEG1(1) | MCAN_NBTP_NTSEG2(1);

    /* Global Filter Configuration Register */
    MCAN1_REGS->MCAN_GFC = MCAN_GFC_ANFS_RX_FIFO_0 | MCAN_GFC_ANFE_RX_FIFO_0;

    /* Set the operation mode */
    MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_DISABLED;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk);

    /* Select interrupt line */
    MCAN1_REGS->MCAN_ILS = 0x0;

    /* Enable interrupt line 0 */
    MCAN1_REGS->MCAN_ILE = MCAN_ILE_EINT0_Msk;

    /* Enable MCAN Bus_Off Status interrupt */
    MCAN1_REGS->MCAN_IE = MCAN_IE_BOE_Msk;

    // Initialize the MCAN PLib Object
    mcan1Obj.txBufferIndex = 0;
    mcan1Obj.rxBufferIndex1 = 0;
    mcan1Obj.rxBufferIndex2 = 0;
    memset((void *)mcan1RxMsg, 0x00, sizeof(mcan1RxMsg));
    memset((void*)&mcan1Obj.msgRAMConfig, 0x00, sizeof(MCAN_MSG_RAM_CONFIG));
    mcan1Obj.isEnabled = true;

    MCAN1_ErrorGet();
    MCAN1_MessageRAMConfigSet(Can1MessageRAM);
    
}

// *****************************************************************************
/* Function:
    void MCAN1_Stop(void)

   Summary:
    Disables given instance of the MCAN peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void MCAN1_Stop(void)
{
    /* Mark MCAN1 PLib Object as disabled */
    mcan1Obj.isEnabled = false;

    /* Disable and clear status of MCAN Bus_Off Status Interrupt */
    MCAN1_REGS->MCAN_IE = (MCAN_IE_BOE(0x00));
    MCAN1_InterruptClear(MCAN_INTERRUPT_BO_MASK);

    /* Disable interrupt line 0 */
    MCAN1_REGS->MCAN_ILE = (MCAN_ILE_EINT0(0x00));

}

// *****************************************************************************
/* Function:
    void MCAN1_IsEnabled(void)

   Summary:
    Returns the enabled status of MCAN1 PLib Object

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    Status of MCAN1 PLib Object.
    true - MCAN1 is enabled and running.
    false - MCAN1 is disabled.
*/
bool MCAN1_IsEnabled(void)
{
    return (mcan1Obj.isEnabled);
}

// *****************************************************************************
/* Function:
    bool MCAN1_MessageTransmit(uint32_t id, uint8_t length, uint8_t* data, MCAN_MODE mode, MCAN_MSG_TX_ATTRIBUTE msgAttr)

   Summary:
    Transmits a message into CAN bus.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    id      - 11-bit / 29-bit identifier (ID).
    length  - length of data buffer in number of bytes.
    data    - pointer to source data buffer
    mode    - MCAN mode Classic CAN or CAN FD without BRS or CAN FD with BRS
    msgAttr - Data Frame or Remote frame using Tx FIFO or Tx Buffer

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_MessageTransmit(uint32_t id, uint8_t length, uint8_t* data, MCAN_MODE mode, MCAN_MSG_TX_ATTRIBUTE msgAttr)
{
    uint8_t tfqpi = 0;
    mcan_txbe_registers_t *fifo = NULL;
    static uint8_t messageMarker = 0;

    switch (msgAttr)
    {
        case MCAN_MSG_ATTR_TX_FIFO_DATA_FRAME:
        case MCAN_MSG_ATTR_TX_FIFO_RTR_FRAME:
            if (MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFQF_Msk)
            {
                /* The FIFO is full */
                return false;
            }
            tfqpi = (uint8_t)((MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFQPI_Msk) >> MCAN_TXFQS_TFQPI_Pos);
            fifo = (mcan_txbe_registers_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.txBuffersAddress + tfqpi * MCAN1_TX_FIFO_BUFFER_ELEMENT_SIZE);
            break;
        default:
            /* Invalid Message Attribute */
            return false;
    }

    /* If the id is longer than 11 bits, it is considered as extended identifier */
    if (id > MCAN_STD_ID_Msk)
    {
        /* An extended identifier is stored into ID */
        fifo->MCAN_TXBE_0 = (id & MCAN_TXBE_0_ID_Msk) | MCAN_TXBE_0_XTD_Msk;
    }
    else
    {
        /* A standard identifier is stored into ID[28:18] */
        fifo->MCAN_TXBE_0 = id << 18;
    }

    /* Limit length */
    if (length > 8)
        length = 8;
    fifo->MCAN_TXBE_1 = MCAN_TXBE_1_DLC(length);

    if (msgAttr == MCAN_MSG_ATTR_TX_BUFFER_DATA_FRAME || msgAttr == MCAN_MSG_ATTR_TX_FIFO_DATA_FRAME)
    {
        /* copy the data into the payload */
        memcpy((uint8_t *)&fifo->MCAN_TXBE_DATA, data, length);
    }
    else if (msgAttr == MCAN_MSG_ATTR_TX_BUFFER_RTR_FRAME || msgAttr == MCAN_MSG_ATTR_TX_FIFO_RTR_FRAME)
    {
        fifo->MCAN_TXBE_0 |= MCAN_TXBE_0_RTR_Msk;
    }

    fifo->MCAN_TXBE_1 |= ((++messageMarker << MCAN_TXBE_1_MM_Pos) & MCAN_TXBE_1_MM_Msk);

    MCAN1_REGS->MCAN_TXBTIE = 1U << tfqpi;

    /* request the transmit */
    MCAN1_REGS->MCAN_TXBAR = 1U << tfqpi;

    MCAN1_REGS->MCAN_IE |= MCAN_IE_TCE_Msk;
    return true;
}

// *****************************************************************************
/* Function:
    bool MCAN1_MessageReceive(uint32_t *id, uint8_t *length, uint8_t *data, uint16_t *timestamp,
                                              MCAN_MSG_RX_ATTRIBUTE msgAttr, MCAN_MSG_RX_FRAME_ATTRIBUTE *msgFrameAttr)

   Summary:
    Receives a message from CAN bus.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    id           - Pointer to 11-bit / 29-bit identifier (ID) to be received.
    length       - Pointer to data length in number of bytes to be received.
    data         - pointer to destination data buffer
    timestamp    - Pointer to Rx message timestamp, timestamp value is 0 if timestamp is disabled
    msgAttr      - Message to be read from Rx FIFO0 or Rx FIFO1 or Rx Buffer
    msgFrameAttr - Data frame or Remote frame to be received

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_MessageReceive(uint32_t *id, uint8_t *length, uint8_t *data, uint16_t *timestamp,
                                          MCAN_MSG_RX_ATTRIBUTE msgAttr, MCAN_MSG_RX_FRAME_ATTRIBUTE *msgFrameAttr)
{

    uint8_t bufferIndex = 0;
    bool status = false;

    switch (msgAttr)
    {
        case MCAN_MSG_ATTR_RX_FIFO0:
            bufferIndex = (uint8_t)((MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0GI_Msk) >> MCAN_RXF0S_F0GI_Pos);
            if (bufferIndex == 0) {
                bufferIndex = 0x1F;
            } else {
                bufferIndex--;
            }

            mcan1RxMsg[msgAttr][bufferIndex].rxId = id;
            mcan1RxMsg[msgAttr][bufferIndex].rxBuffer = data;
            mcan1RxMsg[msgAttr][bufferIndex].rxsize = length;
            mcan1RxMsg[msgAttr][bufferIndex].timestamp = timestamp;
            mcan1RxMsg[msgAttr][bufferIndex].msgFrameAttr = msgFrameAttr;
            MCAN1_REGS->MCAN_IE |= MCAN_IE_RF0NE_Msk;
            status = true;
            break;
        default:
            return status;
    }
    return status;
}

// *****************************************************************************
/* Function:
    bool MCAN1_TransmitEventFIFOElementGet(uint32_t *id, uint8_t *messageMarker, uint16_t *timestamp)

   Summary:
    Get the Transmit Event FIFO Element for the transmitted message.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    id            - Pointer to 11-bit / 29-bit identifier (ID) to be received.
    messageMarker - Pointer to Tx message message marker number to be received
    timestamp     - Pointer to Tx message timestamp to be received, timestamp value is 0 if Timestamp is disabled

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool MCAN1_TransmitEventFIFOElementGet(uint32_t *id, uint8_t *messageMarker, uint16_t *timestamp)
{
    mcan_txefe_registers_t *txEventFIFOElement = NULL;
    uint8_t txefgi = 0;
    bool status = false;

    /* Check if Tx Event FIFO Element available */
    if ((MCAN1_REGS->MCAN_TXEFS & MCAN_TXEFS_EFFL_Msk) != 0)
    {
        /* Get a pointer to Tx Event FIFO Element */
        txefgi = (uint8_t)((MCAN1_REGS->MCAN_TXEFS & MCAN_TXEFS_EFGI_Msk) >> MCAN_TXEFS_EFGI_Pos);
        txEventFIFOElement = (mcan_txefe_registers_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.txEventFIFOAddress + txefgi * sizeof(mcan_txefe_registers_t));

        /* Check if it's a extended message type */
        if (txEventFIFOElement->MCAN_TXEFE_0 & MCAN_TXEFE_0_XTD_Msk)
        {
            *id = txEventFIFOElement->MCAN_TXEFE_0 & MCAN_TXEFE_0_ID_Msk;
        }
        else
        {
            *id = (txEventFIFOElement->MCAN_TXEFE_0 >> 18) & MCAN_STD_ID_Msk;
        }

        *messageMarker = ((txEventFIFOElement->MCAN_TXEFE_1 & MCAN_TXEFE_1_MM_Msk) >> MCAN_TXEFE_1_MM_Pos);

        /* Get timestamp from transmitted message */
        if (timestamp != NULL)
        {
            *timestamp = (uint16_t)(txEventFIFOElement->MCAN_TXEFE_1 & MCAN_TXEFE_1_TXTS_Msk);
        }

        /* Ack the Tx Event FIFO position */
        MCAN1_REGS->MCAN_TXEFA = MCAN_TXEFA_EFAI(txefgi);

        /* Tx Event FIFO Element read successfully, so return true */
        status = true;
    }
    return status;
}

// *****************************************************************************
/* Function:
    MCAN_ERROR MCAN1_ErrorGet(void)

   Summary:
    Returns the error during transfer.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    None.

   Returns:
    Error during transfer.
*/
MCAN_ERROR MCAN1_ErrorGet(void)
{
    MCAN_ERROR error;
    uint32_t   errorStatus = MCAN1_REGS->MCAN_PSR;

    error = (MCAN_ERROR) ((errorStatus & MCAN_PSR_LEC_Msk) | (errorStatus & MCAN_PSR_EP_Msk) | (errorStatus & MCAN_PSR_EW_Msk)
            | (errorStatus & MCAN_PSR_BO_Msk) | (errorStatus & MCAN_PSR_DLEC_Msk) | (errorStatus & MCAN_PSR_PXE_Msk));

    if ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk)
    {
        MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_CCE_Msk;
        MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_DISABLED;
        while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk);
    }

    return error;
}

// *****************************************************************************
/* Function:
    void MCAN1_ErrorCountGet(uint8_t *txErrorCount, uint8_t *rxErrorCount)

   Summary:
    Returns the transmit and receive error count during transfer.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    txErrorCount - Transmit Error Count to be received
    rxErrorCount - Receive Error Count to be received

   Returns:
    None.
*/
void MCAN1_ErrorCountGet(uint8_t *txErrorCount, uint8_t *rxErrorCount)
{
    *txErrorCount = (uint8_t)(MCAN1_REGS->MCAN_ECR & MCAN_ECR_TEC_Msk);
    *rxErrorCount = (uint8_t)((MCAN1_REGS->MCAN_ECR & MCAN_ECR_REC_Msk) >> MCAN_ECR_REC_Pos);
}

// *****************************************************************************
/* Function:
    bool MCAN1_InterruptGet(MCAN_INTERRUPT_MASK interruptMask)

   Summary:
    Returns the Interrupt status.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    interruptMask - Interrupt source number

   Returns:
    true - Requested interrupt is occurred.
    false - Requested interrupt is not occurred.
*/
bool MCAN1_InterruptGet(MCAN_INTERRUPT_MASK interruptMask)
{
    return ((MCAN1_REGS->MCAN_IR & interruptMask) != 0x0);
}

// *****************************************************************************
/* Function:
    void MCAN1_InterruptClear(MCAN_INTERRUPT_MASK interruptMask)

   Summary:
    Clears Interrupt status.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    interruptMask - Interrupt to be cleared

   Returns:
    None
*/
void MCAN1_InterruptClear(MCAN_INTERRUPT_MASK interruptMask)
{
    MCAN1_REGS->MCAN_IR = interruptMask;
}

// *****************************************************************************
/* Function:
    bool MCAN1_TxFIFOIsFull(void)

   Summary:
    Returns true if Tx FIFO is full otherwise false.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    None

   Returns:
    true  - Tx FIFO is full.
    false - Tx FIFO is not full.
*/
bool MCAN1_TxFIFOIsFull(void)
{
    return (MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFQF_Msk);
}

// *****************************************************************************
/* Function:
    void MCAN1_MessageRAMConfigSet(uint8_t *msgRAMConfigBaseAddress)

   Summary:
    Set the Message RAM Configuration.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    msgRAMConfigBaseAddress - Pointer to application allocated buffer base address.
                              Application must allocate buffer from non-cached
                              contiguous memory and buffer size must be
                              MCAN1_MESSAGE_RAM_CONFIG_SIZE

   Returns:
    None
*/
void MCAN1_MessageRAMConfigSet(uint8_t *msgRAMConfigBaseAddress)
{
    uint32_t offset = 0;

    memset((void*)msgRAMConfigBaseAddress, 0x00, MCAN1_MESSAGE_RAM_CONFIG_SIZE);

    /* Set MCAN CCCR Init for Message RAM Configuration */
    MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_ENABLED;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) != MCAN_CCCR_INIT_Msk);

    /* Set CCE to unlock the configuration registers */
    MCAN1_REGS->MCAN_CCCR |= MCAN_CCCR_CCE_Msk;

    mcan1Obj.msgRAMConfig.rxFIFO0Address = (mcan_rxf0e_registers_t *)msgRAMConfigBaseAddress;
    offset = MCAN1_RX_FIFO0_SIZE;
    /* Receive FIFO 0 Configuration Register */
    MCAN1_REGS->MCAN_RXF0C = MCAN_RXF0C_F0S(32) | MCAN_RXF0C_F0WM(0) | MCAN_RXF0C_F0OM_Msk |
            MCAN_RXF0C_F0SA(((uint32_t)mcan1Obj.msgRAMConfig.rxFIFO0Address >> 2));

    mcan1Obj.msgRAMConfig.txBuffersAddress = (mcan_txbe_registers_t *)(msgRAMConfigBaseAddress + offset);
    offset += MCAN1_TX_FIFO_BUFFER_SIZE;
    /* Transmit Buffer/FIFO Configuration Register */
    MCAN1_REGS->MCAN_TXBC = MCAN_TXBC_TFQS(32) |
            MCAN_TXBC_TBSA(((uint32_t)mcan1Obj.msgRAMConfig.txBuffersAddress >> 2));

    mcan1Obj.msgRAMConfig.txEventFIFOAddress =  (mcan_txefe_registers_t *)(msgRAMConfigBaseAddress + offset);
    offset += MCAN1_TX_EVENT_FIFO_SIZE;
    /* Transmit Event FIFO Configuration Register */
    MCAN1_REGS->MCAN_TXEFC = MCAN_TXEFC_EFWM(0) | MCAN_TXEFC_EFS(32) |
            MCAN_TXEFC_EFSA(((uint32_t)mcan1Obj.msgRAMConfig.txEventFIFOAddress >> 2));

    /* Set 16-bit MSB of mcan1 base address */
    MATRIX_REGS->CCFG_SYSIO = (MATRIX_REGS->CCFG_SYSIO & ~CCFG_SYSIO_CAN1DMABA_Msk)
                            | CCFG_SYSIO_CAN1DMABA(((uint32_t)msgRAMConfigBaseAddress >> 16));

    /* Complete Message RAM Configuration by clearing MCAN CCCR Init */
    MCAN1_REGS->MCAN_CCCR = MCAN_CCCR_INIT_DISABLED;
    while ((MCAN1_REGS->MCAN_CCCR & MCAN_CCCR_INIT_Msk) == MCAN_CCCR_INIT_Msk);
}

// *****************************************************************************
/* Function:
    void MCAN1_TxCallbackRegister(MCAN_CALLBACK callback, uintptr_t contextHandle)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given MCAN's transfer events occur.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    callback - A pointer to a function with a calling signature defined
    by the MCAN_CALLBACK data type.

    contextHandle - A value (usually a pointer) passed (unused) into the function
    identified by the callback parameter.

   Returns:
    None.
*/
void MCAN1_TxCallbackRegister(MCAN_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    mcan1CallbackObj[MCAN_CALLBACK_TX_INDEX].callback = callback;
    mcan1CallbackObj[MCAN_CALLBACK_TX_INDEX].context = contextHandle;
}

// *****************************************************************************
/* Function:
    void MCAN1_RxCallbackRegister(MCAN_CALLBACK callback, uintptr_t contextHandle, MCAN_MSG_RX_ATTRIBUTE msgAttr)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given MCAN's transfer events occur.

   Precondition:
    MCAN1_Initialize must have been called for the associated MCAN instance.

   Parameters:
    callback - A pointer to a function with a calling signature defined
    by the MCAN_CALLBACK data type.

    contextHandle - A value (usually a pointer) passed (unused) into the function
    identified by the callback parameter.

    msgAttr   - Message to be read from Rx FIFO0 or Rx FIFO1 or Rx Buffer

   Returns:
    None.
*/
void MCAN1_RxCallbackRegister(MCAN_CALLBACK callback, uintptr_t contextHandle, MCAN_MSG_RX_ATTRIBUTE msgAttr)
{
    if (callback == NULL)
    {
        return;
    }

    mcan1CallbackObj[msgAttr].callback = callback;
    mcan1CallbackObj[msgAttr].context = contextHandle;
}

// *****************************************************************************
/* Function:
    void MCAN1_INT0_InterruptHandler(void)

   Summary:
    MCAN1 Peripheral Interrupt Handler.

   Description:
    This function is MCAN1 Peripheral Interrupt Handler and will
    called on every MCAN1 interrupt.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    The function is called as peripheral instance's interrupt handler if the
    instance interrupt is enabled. If peripheral instance's interrupt is not
    enabled user need to call it from the main while loop of the application.
*/
void MCAN1_INT0_InterruptHandler(void) {
    uint8_t rxgi = 0;
    uint8_t bufferIndex = 0;
    mcan_rxf0e_registers_t *rxf0eFifo = NULL;
    uint32_t ir = MCAN1_REGS->MCAN_IR;

    /* Check if error occurred */
    if (ir & MCAN_IR_BO_Msk) {
        MCAN1_REGS->MCAN_IR = MCAN_IR_BO_Msk;
    }

    /* New Message in Rx FIFO 0 */
    if (ir & MCAN_IR_RF0N_Msk) {

        MCAN1_REGS->MCAN_IR = MCAN_IR_RF0N_Msk;
        MCAN1_REGS->MCAN_IE &= (~MCAN_IE_RF0NE_Msk);

        rxgi = (uint8_t)((MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0GI_Msk) >> MCAN_RXF0S_F0GI_Pos);
        rxf0eFifo = (mcan_rxf0e_registers_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.rxFIFO0Address + rxgi * MCAN1_RX_FIFO0_ELEMENT_SIZE);

        if (MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0FL_Msk) {

            rxgi = (uint8_t)((MCAN1_REGS->MCAN_RXF0S & MCAN_RXF0S_F0GI_Msk) >> MCAN_RXF0S_F0GI_Pos);
            rxf0eFifo = (mcan_rxf0e_registers_t *) ((uint8_t *)mcan1Obj.msgRAMConfig.rxFIFO0Address + rxgi * MCAN1_RX_FIFO0_ELEMENT_SIZE);
            if (mcan1CallbackObj[MCAN_MSG_ATTR_RX_FIFO0].callback != NULL) {
                
                mcan1CallbackObj[MCAN_MSG_ATTR_RX_FIFO0].callback(
                    mcan1CallbackObj[MCAN_MSG_ATTR_RX_FIFO0].context, 
                    rxf0eFifo->MCAN_RXF0E_0 & MCAN_RXF0E_0_ID_Msk,
                    (uint8_t *)&rxf0eFifo->MCAN_RXF0E_DATA, 
                    MCANDlcToLengthGet(((rxf0eFifo->MCAN_RXF0E_1 & MCAN_RXF0E_1_DLC_Msk) >> MCAN_RXF0E_1_DLC_Pos))
                );

                MCAN1_REGS->MCAN_RXF0A = MCAN_RXF0A_F0AI(rxgi);

            }
        }

        MCAN1_REGS->MCAN_IE |= MCAN_IE_RF0NE_Msk;

    }

    /* TX Completed */
    if (ir & MCAN_IR_TC_Msk) {

        MCAN1_REGS->MCAN_IR = MCAN_IR_TC_Msk;
        MCAN1_REGS->MCAN_IE &= (~MCAN_IE_TCE_Msk);
        for (bufferIndex = 0; bufferIndex < (MCAN1_TX_FIFO_BUFFER_SIZE/MCAN1_TX_FIFO_BUFFER_ELEMENT_SIZE); bufferIndex++)
        {
            if ((MCAN1_REGS->MCAN_TXBTO & (1 << (bufferIndex & 0x1F))) &&
                (MCAN1_REGS->MCAN_TXBTIE & (1 << (bufferIndex & 0x1F))))
            {
                MCAN1_REGS->MCAN_TXBTIE &= ~(1 << (bufferIndex & 0x1F));
            }
        }
    }

    /* TX FIFO is empty */
    if (ir & MCAN_IR_TFE_Msk)
    {

        MCAN1_REGS->MCAN_IR = MCAN_IR_TFE_Msk;
        uint8_t getIndex = (uint8_t)((MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFGI_Msk) >> MCAN_TXFQS_TFGI_Pos);
        uint8_t putIndex = (uint8_t)((MCAN1_REGS->MCAN_TXFQS & MCAN_TXFQS_TFQPI_Msk) >> MCAN_TXFQS_TFQPI_Pos);
        for (uint8_t fifoIndex = getIndex; ; fifoIndex++)
        {
            if (fifoIndex >= putIndex)
            {
                break;
            }
        }
    }
}


/*******************************************************************************
 End of File
*/
