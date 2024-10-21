/*******************************************************************************
  SYS PORTS Static Functions for PORTS System Service

  Company:
    Microchip Technology Inc.

  File Name:
    plib_pio.c

  Summary:
    PIO function implementations for the PIO PLIB.

  Description:
    The PIO PLIB provides a simple interface to manage peripheral
    input-output controller.

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

#include "plib_pio.h"

#define PIO_MAX_NUM_OF_CHANNELS     5

/* Array to store callback objects of each configured interrupt */
PIO_PIN_CALLBACK_OBJ portPinCbObj[2 + 1 + 5 + 1 + 0];

/* Array to store number of interrupts in each PORT Channel + previous interrupt count */
uint8_t portNumCb[PIO_MAX_NUM_OF_CHANNELS + 1] = {0, 2, 3, 8, 9, 9};

/******************************************************************************
  Function:
    PIO_Initialize ( void )

  Summary:
    Initialize the PIO library.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_Initialize ( void )
{
    MATRIX_REGS->CCFG_SYSIO = 0x0;
    /* Selected System IO pins are configured as GPIO */
    MATRIX_REGS->CCFG_SYSIO |= 0x1c10;

    /************************ PIO A Initialization ************************/
    /* PORTA Peripheral Function Selection */
    ((pio_registers_t*)PIO_PORT_A)->PIO_ABCDSR[0]= 0x180000;
    ((pio_registers_t*)PIO_PORT_A)->PIO_ABCDSR[1]= 0x60;
    /* PORTA PIO Disable and Peripheral Enable*/
    ((pio_registers_t*)PIO_PORT_A)->PIO_PDR = 0x00380678;
    ((pio_registers_t*)PIO_PORT_A)->PIO_PER = ~0x00380678;
    ((pio_registers_t*)PIO_PORT_A)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTA Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_A)->PIO_PUDR = ~0x09000000;
    ((pio_registers_t*)PIO_PORT_A)->PIO_PUER = 0x09000000;
    /* PORTA Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_A)->PIO_PPDDR = ~0x20004704;
    ((pio_registers_t*)PIO_PORT_A)->PIO_PPDER = 0x20004704;
    /* PORTA Output Write Enable */
    ((pio_registers_t*)PIO_PORT_A)->PIO_OWER = PIO_OWER_Msk;
    /* PORTA Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_A)->PIO_OER = 0x29814901;
    ((pio_registers_t*)PIO_PORT_A)->PIO_ODR = ~0x29814901;
    /* PORTA Initial state High */
    ((pio_registers_t*)PIO_PORT_A)->PIO_ODSR = 0x0;
    /* PORTA Additional interrupt mode Enable */
    ((pio_registers_t*)PIO_PORT_A)->PIO_AIMER = 0x8000004;
    /* PORTA Rising Edge or High Level Interrupt Enable */
    ((pio_registers_t*)PIO_PORT_A)->PIO_REHLSR = 0x8000004;
    /* PORTA Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_A)->PIO_ISR;
    /* PORTA system level interrupt will be enabled by NVIC Manager */
    /* PORTA module level Interrupt for every pin has to be enabled by user
       by calling PIO_PinInterruptEnable() API dynamically as and when needed*/
    /* PORTA drive control */
    ((pio_registers_t*)PIO_PORT_A)->PIO_DRIVER = 0x0;

    /************************ PIO B Initialization ************************/
    /* PORTB Peripheral Function Selection */
    ((pio_registers_t*)PIO_PORT_B)->PIO_ABCDSR[0]= 0x10;
    ((pio_registers_t*)PIO_PORT_B)->PIO_ABCDSR[1]= 0x13;
    /* PORTB PIO Disable and Peripheral Enable*/
    ((pio_registers_t*)PIO_PORT_B)->PIO_PDR = 0x13;
    ((pio_registers_t*)PIO_PORT_B)->PIO_PER = ~0x13;
    ((pio_registers_t*)PIO_PORT_B)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTB Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_B)->PIO_PUDR = ~0x4;
    ((pio_registers_t*)PIO_PORT_B)->PIO_PUER = 0x4;
    /* PORTB Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_B)->PIO_PPDDR = ~0x1000;
    ((pio_registers_t*)PIO_PORT_B)->PIO_PPDER = 0x1000;
    /* PORTB Output Write Enable */
    ((pio_registers_t*)PIO_PORT_B)->PIO_OWER = PIO_OWER_Msk;
    /* PORTB Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_B)->PIO_OER = 0xc;
    ((pio_registers_t*)PIO_PORT_B)->PIO_ODR = ~0xc;
    /* PORTB Initial state High */
    ((pio_registers_t*)PIO_PORT_B)->PIO_ODSR = 0x0;
    /* PORTB Additional interrupt mode Enable */
    ((pio_registers_t*)PIO_PORT_B)->PIO_AIMER = 0x1000;
    /* PORTB Rising Edge or High Level Interrupt Enable */
    ((pio_registers_t*)PIO_PORT_B)->PIO_REHLSR = 0x1000;
    /* PORTB Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_B)->PIO_ISR;
    /* PORTB system level interrupt will be enabled by NVIC Manager */
    /* PORTB module level Interrupt for every pin has to be enabled by user
       by calling PIO_PinInterruptEnable() API dynamically as and when needed*/
    /* PORTB drive control */
    ((pio_registers_t*)PIO_PORT_B)->PIO_DRIVER = 0x0;

    /************************ PIO C Initialization ************************/
    /* PORTC Peripheral Function Selection */
    ((pio_registers_t*)PIO_PORT_C)->PIO_ABCDSR[0]= 0xc;
    ((pio_registers_t*)PIO_PORT_C)->PIO_ABCDSR[1]= 0xf005000;
    /* PORTC PIO Disable and Peripheral Enable*/
    ((pio_registers_t*)PIO_PORT_C)->PIO_PDR = 0xf00500c;
    ((pio_registers_t*)PIO_PORT_C)->PIO_PER = ~0xf00500c;
    ((pio_registers_t*)PIO_PORT_C)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTC Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_C)->PIO_PUDR = ~0x10000;
    ((pio_registers_t*)PIO_PORT_C)->PIO_PUER = 0x10000;
    /* PORTC Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_C)->PIO_PPDDR = ~0x80080620;
    ((pio_registers_t*)PIO_PORT_C)->PIO_PPDER = 0x80080620;
    /* PORTC Output Write Enable */
    ((pio_registers_t*)PIO_PORT_C)->PIO_OWER = PIO_OWER_Msk;
    /* PORTC Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_C)->PIO_OER = 0x408100a0;
    ((pio_registers_t*)PIO_PORT_C)->PIO_ODR = ~0x408100a0;
    /* PORTC Initial state High */
    ((pio_registers_t*)PIO_PORT_C)->PIO_ODSR = 0x0;
    /* PORTC Additional interrupt mode Enable */
    ((pio_registers_t*)PIO_PORT_C)->PIO_AIMER = 0x90080600;
    /* PORTC Rising Edge or High Level Interrupt Enable */
    ((pio_registers_t*)PIO_PORT_C)->PIO_REHLSR = 0x90080600;
    /* PORTC Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_C)->PIO_ISR;
    /* PORTC system level interrupt will be enabled by NVIC Manager */
    /* PORTC module level Interrupt for every pin has to be enabled by user
       by calling PIO_PinInterruptEnable() API dynamically as and when needed*/
    /* PORTC drive control */
    ((pio_registers_t*)PIO_PORT_C)->PIO_DRIVER = 0x0;

    /************************ PIO D Initialization ************************/
    /* PORTD Peripheral Function Selection */
    ((pio_registers_t*)PIO_PORT_D)->PIO_ABCDSR[0]= 0x718000;
    ((pio_registers_t*)PIO_PORT_D)->PIO_ABCDSR[1]= 0x6000000;
    /* PORTD PIO Disable and Peripheral Enable*/
    ((pio_registers_t*)PIO_PORT_D)->PIO_PDR = 0x56718000;
    ((pio_registers_t*)PIO_PORT_D)->PIO_PER = ~0x56718000;
    ((pio_registers_t*)PIO_PORT_D)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTD Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_D)->PIO_PUDR = ~0x1000;
    ((pio_registers_t*)PIO_PORT_D)->PIO_PUER = 0x1000;
    /* PORTD Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_D)->PIO_PPDDR = ~0x42805;
    ((pio_registers_t*)PIO_PORT_D)->PIO_PPDER = 0x42805;
    /* PORTD Output Write Enable */
    ((pio_registers_t*)PIO_PORT_D)->PIO_OWER = PIO_OWER_Msk;
    /* PORTD Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_D)->PIO_OER = 0x8001805;
    ((pio_registers_t*)PIO_PORT_D)->PIO_ODR = ~0x8001805;
    /* PORTD Initial state High */
    ((pio_registers_t*)PIO_PORT_D)->PIO_ODSR = 0x0;
    /* PORTD Additional interrupt mode Enable */
    ((pio_registers_t*)PIO_PORT_D)->PIO_AIMER = 0x2000;
    /* PORTD Rising Edge or High Level Interrupt Enable */
    ((pio_registers_t*)PIO_PORT_D)->PIO_REHLSR = 0x2000;
    /* PORTD Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_D)->PIO_ISR;
    /* PORTD system level interrupt will be enabled by NVIC Manager */
    /* PORTD module level Interrupt for every pin has to be enabled by user
       by calling PIO_PinInterruptEnable() API dynamically as and when needed*/
    /* PORTD drive control */
    ((pio_registers_t*)PIO_PORT_D)->PIO_DRIVER = 0x0;

    /************************ PIO E Initialization ************************/
    ((pio_registers_t*)PIO_PORT_E)->PIO_PER = 0xFFFFFFFF;
    ((pio_registers_t*)PIO_PORT_E)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTE Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_E)->PIO_PUDR = 0xFFFFFFFF;
    /* PORTE Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_E)->PIO_PPDDR = 0xFFFFFFFF;
    /* PORTE Output Write Enable */
    ((pio_registers_t*)PIO_PORT_E)->PIO_OWER = PIO_OWER_Msk;
    /* PORTE Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_E)->PIO_OER = 0xf;
    ((pio_registers_t*)PIO_PORT_E)->PIO_ODR = ~0xf;
    /* PORTE Initial state High */
    ((pio_registers_t*)PIO_PORT_E)->PIO_ODSR = 0x0;
    /* PORTE drive control */
    ((pio_registers_t*)PIO_PORT_E)->PIO_DRIVER = 0x0;

    uint32_t i;
    /* Initialize Interrupt Pin data structures */
    portPinCbObj[3 + 0].pin = PIO_PIN_PC31;
    
    portPinCbObj[0].pin = PIO_PIN_PA27;
    
    portPinCbObj[3 + 1].pin = PIO_PIN_PC28;

    portPinCbObj[3 + 2].pin = PIO_PIN_PC9;
    
    portPinCbObj[2 + 0].pin = PIO_PIN_PB12;
    
    portPinCbObj[8 + 0].pin = PIO_PIN_PD13;
    
    portPinCbObj[3 + 3].pin = PIO_PIN_PC10;
    
    portPinCbObj[1].pin = PIO_PIN_PA2;
    
    portPinCbObj[3 + 4].pin = PIO_PIN_PC19;
    
    for(i=0; i<9; i++)
    {
        portPinCbObj[i].callback = NULL;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: PIO APIs which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t PIO_PortRead ( PIO_PORT port )

  Summary:
    Read all the I/O lines of the selected port.

  Description:
    This function reads the live data values on all the I/O lines of the
    selected port.  Bit values returned in each position indicate corresponding
    pin levels.
    1 = Pin is high.
    0 = Pin is low.

    This function reads the value regardless of pin configuration, whether it is
    set as as an input, driven by the PIO Controller, or driven by a peripheral.

  Remarks:
    If the port has less than 32-bits, unimplemented pins will read as
    low (0).
    Implemented pins are Right aligned in the 32-bit return value.
*/
uint32_t PIO_PortRead(PIO_PORT port)
{
    return ((pio_registers_t*)port)->PIO_PDSR;
}

// *****************************************************************************
/* Function:
    void PIO_PortWrite (PIO_PORT port, uint32_t mask, uint32_t value);

  Summary:
    Write the value on the masked I/O lines of the selected port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortWrite(PIO_PORT port, uint32_t mask, uint32_t value)
{
    ((pio_registers_t*)port)->PIO_ODSR = (((pio_registers_t*)port)->PIO_ODSR & (~mask)) | (mask & value);
}

// *****************************************************************************
/* Function:
    uint32_t PIO_PortLatchRead ( PIO_PORT port )

  Summary:
    Read the latched value on all the I/O lines of the selected port.

  Remarks:
    See plib_pio.h for more details.
*/
uint32_t PIO_PortLatchRead(PIO_PORT port)
{
    return ((pio_registers_t*)port)->PIO_ODSR;
}

// *****************************************************************************
/* Function:
    void PIO_PortSet ( PIO_PORT port, uint32_t mask )

  Summary:
    Set the selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortSet(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_SODR = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortClear ( PIO_PORT port, uint32_t mask )

  Summary:
    Clear the selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortClear(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_CODR = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortToggle ( PIO_PORT port, uint32_t mask )

  Summary:
    Toggles the selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortToggle(PIO_PORT port, uint32_t mask)
{
    /* Write into Clr and Set registers */
    ((pio_registers_t*)port)->PIO_ODSR^= mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortInputEnable ( PIO_PORT port, uint32_t mask )

  Summary:
    Enables selected IO pins of a port as input.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortInputEnable(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_ODR = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortOutputEnable ( PIO_PORT port, uint32_t mask )

  Summary:
    Enables selected IO pins of a port as output(s).

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortOutputEnable(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_OER = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortInterruptEnable(PIO_PORT port, uint32_t mask)

  Summary:
    Enables IO interrupt on selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortInterruptEnable(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_IER = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortInterruptDisable(PIO_PORT port, uint32_t mask)

  Summary:
    Disables IO interrupt on selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortInterruptDisable(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_IDR = mask;
}

// *****************************************************************************
// *****************************************************************************
// Section: PIO APIs which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    bool PIO_PinInterruptCallbackRegister(
        PIO_PIN pin,
        const PIO_PIN_CALLBACK callback,
        uintptr_t context
    );

  Summary:
    Allows application to register callback for configured pin.

  Remarks:
    See plib_pio.h for more details.
*/
bool PIO_PinInterruptCallbackRegister(
    PIO_PIN pin,
    const PIO_PIN_CALLBACK callback,
    uintptr_t context
)
{
    uint8_t i;
    uint8_t portIndex;

    portIndex = pin >> 5;

    for(i = portNumCb[portIndex]; i < portNumCb[portIndex +1]; i++)
    {
        if (portPinCbObj[i].pin == pin)
        {
            portPinCbObj[i].callback = callback;
            portPinCbObj[i].context  = context;
            return true;
        }
    }
    return false;
}
// *****************************************************************************
// *****************************************************************************
// Section: Local Function Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void _PIO_Interrupt_Handler ( PIO_PORT port )

  Summary:
    Interrupt handler for a selected port.

  Description:
    This function defines the Interrupt handler for a selected port.

  Remarks:
	It is an internal function used by the library, user should not call it.
*/
void _PIO_Interrupt_Handler ( PIO_PORT port )
{
    uint32_t status;
    uint32_t i, portIndex;

    status  = ((pio_registers_t*)port)->PIO_ISR;
    status &= ((pio_registers_t*)port)->PIO_IMR;

    /* get the index of the port channel: PIO_PORT_A--> 0, PIO_PORT_B--> 1 ... */
    portIndex = (port - PIOA_BASE_ADDRESS) >> 9;

    /* Check pending events and call callback if registered */
    for(i = portNumCb[portIndex]; i < portNumCb[portIndex +1]; i++)
    {
        if((status & (1 << (portPinCbObj[i].pin & 0x1F))) && (portPinCbObj[i].callback != NULL))
        {
            portPinCbObj[i].callback (portPinCbObj[i].pin, portPinCbObj[i].context);
        }
    }

}
// *****************************************************************************
// *****************************************************************************
// Section: Interrupt Service Routine (ISR) Implementation(s)
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void PIOA_InterruptHandler (void)

  Summary:
    Interrupt handler for PORTA.

  Description:
    This function defines the Interrupt service routine for PORTA.
    This is the function which by default gets into Interrupt Vector Table.

  Remarks:
    User should not call this function.
*/
void PIOA_InterruptHandler(void)
{
    /* Local PIO Interrupt Handler */
    _PIO_Interrupt_Handler(PIO_PORT_A);
}
// *****************************************************************************
/* Function:
    void PIOB_InterruptHandler (void)

  Summary:
    Interrupt handler for PORTB.

  Description:
    This function defines the Interrupt service routine for PORTB.
    This is the function which by default gets into Interrupt Vector Table.

  Remarks:
    User should not call this function.
*/
void PIOB_InterruptHandler(void)
{
    /* Local PIO Interrupt Handler */
    _PIO_Interrupt_Handler(PIO_PORT_B);
}

// *****************************************************************************
/* Function:
    void PIOC_InterruptHandler (void)

  Summary:
    Interrupt handler for PORTC.

  Description:
    This function defines the Interrupt service routine for PORTC.
    This is the function which by default gets into Interrupt Vector Table.

  Remarks:
    User should not call this function.
*/
void PIOC_InterruptHandler(void)
{
    /* Local PIO Interrupt Handler */
    _PIO_Interrupt_Handler(PIO_PORT_C);
}

// *****************************************************************************
/* Function:
    void PIOD_InterruptHandler (void)

  Summary:
    Interrupt handler for PORTD.

  Description:
    This function defines the Interrupt service routine for PORTD.
    This is the function which by default gets into Interrupt Vector Table.

  Remarks:
    User should not call this function.
*/
void PIOD_InterruptHandler(void)
{
    /* Local PIO Interrupt Handler */
    _PIO_Interrupt_Handler(PIO_PORT_D);
}





/*******************************************************************************
 End of File
*/
