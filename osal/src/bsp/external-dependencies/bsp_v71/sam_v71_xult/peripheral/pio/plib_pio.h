/*******************************************************************************
  PIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_pio.h

  Summary:
    PIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (PIO) module.

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

#ifndef PLIB_PIO_H
#define PLIB_PIO_H

#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for GPIO_PD0 pin ***/
#define GPIO_PD0_Set()               (PIOD_REGS->PIO_SODR = (1<<0))
#define GPIO_PD0_Clear()             (PIOD_REGS->PIO_CODR = (1<<0))
#define GPIO_PD0_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<0))
#define GPIO_PD0_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<0))
#define GPIO_PD0_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<0))
#define GPIO_PD0_Get()               ((PIOD_REGS->PIO_PDSR >> 0) & 0x1)
#define GPIO_PD0_PIN                  PIO_PIN_PD0

/*** Macros for GPIO_PE0 pin ***/
#define GPIO_PE0_Set()               (PIOE_REGS->PIO_SODR = (1<<0))
#define GPIO_PE0_Clear()             (PIOE_REGS->PIO_CODR = (1<<0))
#define GPIO_PE0_Toggle()            (PIOE_REGS->PIO_ODSR ^= (1<<0))
#define GPIO_PE0_OutputEnable()      (PIOE_REGS->PIO_OER = (1<<0))
#define GPIO_PE0_InputEnable()       (PIOE_REGS->PIO_ODR = (1<<0))
#define GPIO_PE0_Get()               ((PIOE_REGS->PIO_PDSR >> 0) & 0x1)
#define GPIO_PE0_PIN                  PIO_PIN_PE0

/*** Macros for GPIO_PE1 pin ***/
#define GPIO_PE1_Set()               (PIOE_REGS->PIO_SODR = (1<<1))
#define GPIO_PE1_Clear()             (PIOE_REGS->PIO_CODR = (1<<1))
#define GPIO_PE1_Toggle()            (PIOE_REGS->PIO_ODSR ^= (1<<1))
#define GPIO_PE1_OutputEnable()      (PIOE_REGS->PIO_OER = (1<<1))
#define GPIO_PE1_InputEnable()       (PIOE_REGS->PIO_ODR = (1<<1))
#define GPIO_PE1_Get()               ((PIOE_REGS->PIO_PDSR >> 1) & 0x1)
#define GPIO_PE1_PIN                  PIO_PIN_PE1

/*** Macros for GPIO_PE2 pin ***/
#define GPIO_PE2_Set()               (PIOE_REGS->PIO_SODR = (1<<2))
#define GPIO_PE2_Clear()             (PIOE_REGS->PIO_CODR = (1<<2))
#define GPIO_PE2_Toggle()            (PIOE_REGS->PIO_ODSR ^= (1<<2))
#define GPIO_PE2_OutputEnable()      (PIOE_REGS->PIO_OER = (1<<2))
#define GPIO_PE2_InputEnable()       (PIOE_REGS->PIO_ODR = (1<<2))
#define GPIO_PE2_Get()               ((PIOE_REGS->PIO_PDSR >> 2) & 0x1)
#define GPIO_PE2_PIN                  PIO_PIN_PE2

/*** Macros for GPIO_PE3 pin ***/
#define GPIO_PE3_Set()               (PIOE_REGS->PIO_SODR = (1<<3))
#define GPIO_PE3_Clear()             (PIOE_REGS->PIO_CODR = (1<<3))
#define GPIO_PE3_Toggle()            (PIOE_REGS->PIO_ODSR ^= (1<<3))
#define GPIO_PE3_OutputEnable()      (PIOE_REGS->PIO_OER = (1<<3))
#define GPIO_PE3_InputEnable()       (PIOE_REGS->PIO_ODR = (1<<3))
#define GPIO_PE3_Get()               ((PIOE_REGS->PIO_PDSR >> 3) & 0x1)
#define GPIO_PE3_PIN                  PIO_PIN_PE3

/*** Macros for GPIO_PC31 pin ***/
#define GPIO_PC31_Set()               (PIOC_REGS->PIO_SODR = (1<<31))
#define GPIO_PC31_Clear()             (PIOC_REGS->PIO_CODR = (1<<31))
#define GPIO_PC31_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<31))
#define GPIO_PC31_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<31))
#define GPIO_PC31_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<31))
#define GPIO_PC31_Get()               ((PIOC_REGS->PIO_PDSR >> 31) & 0x1)
#define GPIO_PC31_PIN                  PIO_PIN_PC31
#define GPIO_PC31_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<31))
#define GPIO_PC31_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<31))

/*** Macros for GPIO_PC30 pin ***/
#define GPIO_PC30_Set()               (PIOC_REGS->PIO_SODR = (1<<30))
#define GPIO_PC30_Clear()             (PIOC_REGS->PIO_CODR = (1<<30))
#define GPIO_PC30_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<30))
#define GPIO_PC30_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<30))
#define GPIO_PC30_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<30))
#define GPIO_PC30_Get()               ((PIOC_REGS->PIO_PDSR >> 30) & 0x1)
#define GPIO_PC30_PIN                  PIO_PIN_PC30

/*** Macros for GPIO_PC29 pin ***/
#define GPIO_PC29_Set()               (PIOC_REGS->PIO_SODR = (1<<29))
#define GPIO_PC29_Clear()             (PIOC_REGS->PIO_CODR = (1<<29))
#define GPIO_PC29_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<29))
#define GPIO_PC29_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<29))
#define GPIO_PC29_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<29))
#define GPIO_PC29_Get()               ((PIOC_REGS->PIO_PDSR >> 29) & 0x1)
#define GPIO_PC29_PIN                  PIO_PIN_PC29

/*** Macros for GPIO_PC15 pin ***/
#define GPIO_PC15_Set()               (PIOC_REGS->PIO_SODR = (1<<15))
#define GPIO_PC15_Clear()             (PIOC_REGS->PIO_CODR = (1<<15))
#define GPIO_PC15_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<15))
#define GPIO_PC15_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<15))
#define GPIO_PC15_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<15))
#define GPIO_PC15_Get()               ((PIOC_REGS->PIO_PDSR >> 15) & 0x1)
#define GPIO_PC15_PIN                  PIO_PIN_PC15

/*** Macros for GPIO_PC13 pin ***/
#define GPIO_PC13_Set()               (PIOC_REGS->PIO_SODR = (1<<13))
#define GPIO_PC13_Clear()             (PIOC_REGS->PIO_CODR = (1<<13))
#define GPIO_PC13_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<13))
#define GPIO_PC13_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<13))
#define GPIO_PC13_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<13))
#define GPIO_PC13_Get()               ((PIOC_REGS->PIO_PDSR >> 13) & 0x1)
#define GPIO_PC13_PIN                  PIO_PIN_PC13

/*** Macros for GPIO_PA18 pin ***/
#define GPIO_PA18_Set()               (PIOA_REGS->PIO_SODR = (1<<18))
#define GPIO_PA18_Clear()             (PIOA_REGS->PIO_CODR = (1<<18))
#define GPIO_PA18_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<18))
#define GPIO_PA18_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<18))
#define GPIO_PA18_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<18))
#define GPIO_PA18_Get()               ((PIOA_REGS->PIO_PDSR >> 18) & 0x1)
#define GPIO_PA18_PIN                  PIO_PIN_PA18

/*** Macros for GPIO_PA17 pin ***/
#define GPIO_PA17_Set()               (PIOA_REGS->PIO_SODR = (1<<17))
#define GPIO_PA17_Clear()             (PIOA_REGS->PIO_CODR = (1<<17))
#define GPIO_PA17_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<17))
#define GPIO_PA17_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<17))
#define GPIO_PA17_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<17))
#define GPIO_PA17_Get()               ((PIOA_REGS->PIO_PDSR >> 17) & 0x1)
#define GPIO_PA17_PIN                  PIO_PIN_PA17

/*** Macros for GPIO_PB2 pin ***/
#define GPIO_PB2_Set()               (PIOB_REGS->PIO_SODR = (1<<2))
#define GPIO_PB2_Clear()             (PIOB_REGS->PIO_CODR = (1<<2))
#define GPIO_PB2_Toggle()            (PIOB_REGS->PIO_ODSR ^= (1<<2))
#define GPIO_PB2_OutputEnable()      (PIOB_REGS->PIO_OER = (1<<2))
#define GPIO_PB2_InputEnable()       (PIOB_REGS->PIO_ODR = (1<<2))
#define GPIO_PB2_Get()               ((PIOB_REGS->PIO_PDSR >> 2) & 0x1)
#define GPIO_PB2_PIN                  PIO_PIN_PB2

/*** Macros for GPIO_PB3 pin ***/
#define GPIO_PB3_Set()               (PIOB_REGS->PIO_SODR = (1<<3))
#define GPIO_PB3_Clear()             (PIOB_REGS->PIO_CODR = (1<<3))
#define GPIO_PB3_Toggle()            (PIOB_REGS->PIO_ODSR ^= (1<<3))
#define GPIO_PB3_OutputEnable()      (PIOB_REGS->PIO_OER = (1<<3))
#define GPIO_PB3_InputEnable()       (PIOB_REGS->PIO_ODR = (1<<3))
#define GPIO_PB3_Get()               ((PIOB_REGS->PIO_PDSR >> 3) & 0x1)
#define GPIO_PB3_PIN                  PIO_PIN_PB3

/*** Macros for GPIO_PA8 pin ***/
#define GPIO_PA8_Set()               (PIOA_REGS->PIO_SODR = (1<<8))
#define GPIO_PA8_Clear()             (PIOA_REGS->PIO_CODR = (1<<8))
#define GPIO_PA8_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<8))
#define GPIO_PA8_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<8))
#define GPIO_PA8_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<8))
#define GPIO_PA8_Get()               ((PIOA_REGS->PIO_PDSR >> 8) & 0x1)
#define GPIO_PA8_PIN                  PIO_PIN_PA8

/*** Macros for GPIO_PA22 pin ***/
#define GPIO_PA22_Set()               (PIOA_REGS->PIO_SODR = (1<<22))
#define GPIO_PA22_Clear()             (PIOA_REGS->PIO_CODR = (1<<22))
#define GPIO_PA22_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<22))
#define GPIO_PA22_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<22))
#define GPIO_PA22_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<22))
#define GPIO_PA22_Get()               ((PIOA_REGS->PIO_PDSR >> 22) & 0x1)
#define GPIO_PA22_PIN                  PIO_PIN_PA22

/*** Macros for GPIO_PA16 pin ***/
#define GPIO_PA16_Set()               (PIOA_REGS->PIO_SODR = (1<<16))
#define GPIO_PA16_Clear()             (PIOA_REGS->PIO_CODR = (1<<16))
#define GPIO_PA16_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<16))
#define GPIO_PA16_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<16))
#define GPIO_PA16_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<16))
#define GPIO_PA16_Get()               ((PIOA_REGS->PIO_PDSR >> 16) & 0x1)
#define GPIO_PA16_PIN                  PIO_PIN_PA16

/*** Macros for GPIO_PA23 pin ***/
#define GPIO_PA23_Set()               (PIOA_REGS->PIO_SODR = (1<<23))
#define GPIO_PA23_Clear()             (PIOA_REGS->PIO_CODR = (1<<23))
#define GPIO_PA23_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<23))
#define GPIO_PA23_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<23))
#define GPIO_PA23_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<23))
#define GPIO_PA23_Get()               ((PIOA_REGS->PIO_PDSR >> 23) & 0x1)
#define GPIO_PA23_PIN                  PIO_PIN_PA23

/*** Macros for GPIO_PD27 pin ***/
#define GPIO_PD27_Set()               (PIOD_REGS->PIO_SODR = (1<<27))
#define GPIO_PD27_Clear()             (PIOD_REGS->PIO_CODR = (1<<27))
#define GPIO_PD27_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<27))
#define GPIO_PD27_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<27))
#define GPIO_PD27_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<27))
#define GPIO_PD27_Get()               ((PIOD_REGS->PIO_PDSR >> 27) & 0x1)
#define GPIO_PD27_PIN                  PIO_PIN_PD27

/*** Macros for GPIO_PC7 pin ***/
#define GPIO_PC7_Set()               (PIOC_REGS->PIO_SODR = (1<<7))
#define GPIO_PC7_Clear()             (PIOC_REGS->PIO_CODR = (1<<7))
#define GPIO_PC7_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<7))
#define GPIO_PC7_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<7))
#define GPIO_PC7_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<7))
#define GPIO_PC7_Get()               ((PIOC_REGS->PIO_PDSR >> 7) & 0x1)
#define GPIO_PC7_PIN                  PIO_PIN_PC7

/*** Macros for GPIO_PA15 pin ***/
#define GPIO_PA15_Set()               (PIOA_REGS->PIO_SODR = (1<<15))
#define GPIO_PA15_Clear()             (PIOA_REGS->PIO_CODR = (1<<15))
#define GPIO_PA15_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<15))
#define GPIO_PA15_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<15))
#define GPIO_PA15_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<15))
#define GPIO_PA15_Get()               ((PIOA_REGS->PIO_PDSR >> 15) & 0x1)
#define GPIO_PA15_PIN                  PIO_PIN_PA15

/*** Macros for GPIO_PA14 pin ***/
#define GPIO_PA14_Set()               (PIOA_REGS->PIO_SODR = (1<<14))
#define GPIO_PA14_Clear()             (PIOA_REGS->PIO_CODR = (1<<14))
#define GPIO_PA14_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<14))
#define GPIO_PA14_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<14))
#define GPIO_PA14_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<14))
#define GPIO_PA14_Get()               ((PIOA_REGS->PIO_PDSR >> 14) & 0x1)
#define GPIO_PA14_PIN                  PIO_PIN_PA14

/*** Macros for GPIO_PA24 pin ***/
#define GPIO_PA24_Set()               (PIOA_REGS->PIO_SODR = (1<<24))
#define GPIO_PA24_Clear()             (PIOA_REGS->PIO_CODR = (1<<24))
#define GPIO_PA24_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<24))
#define GPIO_PA24_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<24))
#define GPIO_PA24_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<24))
#define GPIO_PA24_Get()               ((PIOA_REGS->PIO_PDSR >> 24) & 0x1)
#define GPIO_PA24_PIN                  PIO_PIN_PA24

/*** Macros for GPIO_PD23 pin ***/
#define GPIO_PD23_Set()               (PIOD_REGS->PIO_SODR = (1<<23))
#define GPIO_PD23_Clear()             (PIOD_REGS->PIO_CODR = (1<<23))
#define GPIO_PD23_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<23))
#define GPIO_PD23_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<23))
#define GPIO_PD23_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<23))
#define GPIO_PD23_Get()               ((PIOD_REGS->PIO_PDSR >> 23) & 0x1)
#define GPIO_PD23_PIN                  PIO_PIN_PD23

/*** Macros for GPIO_PC5 pin ***/
#define GPIO_PC5_Set()               (PIOC_REGS->PIO_SODR = (1<<5))
#define GPIO_PC5_Clear()             (PIOC_REGS->PIO_CODR = (1<<5))
#define GPIO_PC5_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<5))
#define GPIO_PC5_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<5))
#define GPIO_PC5_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<5))
#define GPIO_PC5_Get()               ((PIOC_REGS->PIO_PDSR >> 5) & 0x1)
#define GPIO_PC5_PIN                  PIO_PIN_PC5

/*** Macros for GPIO_PA11 pin ***/
#define GPIO_PA11_Set()               (PIOA_REGS->PIO_SODR = (1<<11))
#define GPIO_PA11_Clear()             (PIOA_REGS->PIO_CODR = (1<<11))
#define GPIO_PA11_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<11))
#define GPIO_PA11_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<11))
#define GPIO_PA11_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<11))
#define GPIO_PA11_Get()               ((PIOA_REGS->PIO_PDSR >> 11) & 0x1)
#define GPIO_PA11_PIN                  PIO_PIN_PA11

/*** Macros for GPIO_PD18 pin ***/
#define GPIO_PD18_Set()               (PIOD_REGS->PIO_SODR = (1<<18))
#define GPIO_PD18_Clear()             (PIOD_REGS->PIO_CODR = (1<<18))
#define GPIO_PD18_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<18))
#define GPIO_PD18_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<18))
#define GPIO_PD18_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<18))
#define GPIO_PD18_Get()               ((PIOD_REGS->PIO_PDSR >> 18) & 0x1)
#define GPIO_PD18_PIN                  PIO_PIN_PD18

/*** Macros for GPIO_PA27 pin ***/
#define GPIO_PA27_Set()               (PIOA_REGS->PIO_SODR = (1<<27))
#define GPIO_PA27_Clear()             (PIOA_REGS->PIO_CODR = (1<<27))
#define GPIO_PA27_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<27))
#define GPIO_PA27_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<27))
#define GPIO_PA27_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<27))
#define GPIO_PA27_Get()               ((PIOA_REGS->PIO_PDSR >> 27) & 0x1)
#define GPIO_PA27_PIN                  PIO_PIN_PA27
#define GPIO_PA27_InterruptEnable()   (PIOA_REGS->PIO_IER = (1<<27))
#define GPIO_PA27_InterruptDisable()  (PIOA_REGS->PIO_IDR = (1<<27))

/*** Macros for GPIO_PC28 pin ***/
#define GPIO_PC28_Set()               (PIOC_REGS->PIO_SODR = (1<<28))
#define GPIO_PC28_Clear()             (PIOC_REGS->PIO_CODR = (1<<28))
#define GPIO_PC28_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<28))
#define GPIO_PC28_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<28))
#define GPIO_PC28_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<28))
#define GPIO_PC28_Get()               ((PIOC_REGS->PIO_PDSR >> 28) & 0x1)
#define GPIO_PC28_PIN                  PIO_PIN_PC28
#define GPIO_PC28_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<28))
#define GPIO_PC28_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<28))

/*** Macros for GPIO_PC9 pin ***/
#define GPIO_PC9_Set()               (PIOC_REGS->PIO_SODR = (1<<9))
#define GPIO_PC9_Clear()             (PIOC_REGS->PIO_CODR = (1<<9))
#define GPIO_PC9_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<9))
#define GPIO_PC9_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<9))
#define GPIO_PC9_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<9))
#define GPIO_PC9_Get()               ((PIOC_REGS->PIO_PDSR >> 9) & 0x1)
#define GPIO_PC9_PIN                  PIO_PIN_PC9
#define GPIO_PC9_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<9))
#define GPIO_PC9_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<9))

/*** Macros for GPIO_PB12 pin ***/
#define GPIO_PB12_Set()               (PIOB_REGS->PIO_SODR = (1<<12))
#define GPIO_PB12_Clear()             (PIOB_REGS->PIO_CODR = (1<<12))
#define GPIO_PB12_Toggle()            (PIOB_REGS->PIO_ODSR ^= (1<<12))
#define GPIO_PB12_OutputEnable()      (PIOB_REGS->PIO_OER = (1<<12))
#define GPIO_PB12_InputEnable()       (PIOB_REGS->PIO_ODR = (1<<12))
#define GPIO_PB12_Get()               ((PIOB_REGS->PIO_PDSR >> 12) & 0x1)
#define GPIO_PB12_PIN                  PIO_PIN_PB12
#define GPIO_PB12_InterruptEnable()   (PIOB_REGS->PIO_IER = (1<<12))
#define GPIO_PB12_InterruptDisable()  (PIOB_REGS->PIO_IDR = (1<<12))

/*** Macros for GPIO_PD13 pin ***/
#define GPIO_PD13_Set()               (PIOD_REGS->PIO_SODR = (1<<13))
#define GPIO_PD13_Clear()             (PIOD_REGS->PIO_CODR = (1<<13))
#define GPIO_PD13_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<13))
#define GPIO_PD13_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<13))
#define GPIO_PD13_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<13))
#define GPIO_PD13_Get()               ((PIOD_REGS->PIO_PDSR >> 13) & 0x1)
#define GPIO_PD13_PIN                  PIO_PIN_PD13
#define GPIO_PD13_InterruptEnable()   (PIOD_REGS->PIO_IER = (1<<13))
#define GPIO_PD13_InterruptDisable()  (PIOD_REGS->PIO_IDR = (1<<13))

/*** Macros for GPIO_PC10 pin ***/
#define GPIO_PC10_Set()               (PIOC_REGS->PIO_SODR = (1<<10))
#define GPIO_PC10_Clear()             (PIOC_REGS->PIO_CODR = (1<<10))
#define GPIO_PC10_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<10))
#define GPIO_PC10_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<10))
#define GPIO_PC10_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<10))
#define GPIO_PC10_Get()               ((PIOC_REGS->PIO_PDSR >> 10) & 0x1)
#define GPIO_PC10_PIN                  PIO_PIN_PC10
#define GPIO_PC10_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<10))
#define GPIO_PC10_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<10))

/*** Macros for GPIO_PD12 pin ***/
#define GPIO_PD12_Set()               (PIOD_REGS->PIO_SODR = (1<<12))
#define GPIO_PD12_Clear()             (PIOD_REGS->PIO_CODR = (1<<12))
#define GPIO_PD12_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<12))
#define GPIO_PD12_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<12))
#define GPIO_PD12_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<12))
#define GPIO_PD12_Get()               ((PIOD_REGS->PIO_PDSR >> 12) & 0x1)
#define GPIO_PD12_PIN                  PIO_PIN_PD12

/*** Macros for GPIO_PA2 pin ***/
#define GPIO_PA2_Set()               (PIOA_REGS->PIO_SODR = (1<<2))
#define GPIO_PA2_Clear()             (PIOA_REGS->PIO_CODR = (1<<2))
#define GPIO_PA2_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<2))
#define GPIO_PA2_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<2))
#define GPIO_PA2_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<2))
#define GPIO_PA2_Get()               ((PIOA_REGS->PIO_PDSR >> 2) & 0x1)
#define GPIO_PA2_PIN                  PIO_PIN_PA2
#define GPIO_PA2_InterruptEnable()   (PIOA_REGS->PIO_IER = (1<<2))
#define GPIO_PA2_InterruptDisable()  (PIOA_REGS->PIO_IDR = (1<<2))

/*** Macros for GPIO_PD11 pin ***/
#define GPIO_PD11_Set()               (PIOD_REGS->PIO_SODR = (1<<11))
#define GPIO_PD11_Clear()             (PIOD_REGS->PIO_CODR = (1<<11))
#define GPIO_PD11_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<11))
#define GPIO_PD11_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<11))
#define GPIO_PD11_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<11))
#define GPIO_PD11_Get()               ((PIOD_REGS->PIO_PDSR >> 11) & 0x1)
#define GPIO_PD11_PIN                  PIO_PIN_PD11

/*** Macros for GPIO_PC16 pin ***/
#define GPIO_PC16_Set()               (PIOC_REGS->PIO_SODR = (1<<16))
#define GPIO_PC16_Clear()             (PIOC_REGS->PIO_CODR = (1<<16))
#define GPIO_PC16_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<16))
#define GPIO_PC16_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<16))
#define GPIO_PC16_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<16))
#define GPIO_PC16_Get()               ((PIOC_REGS->PIO_PDSR >> 16) & 0x1)
#define GPIO_PC16_PIN                  PIO_PIN_PC16

/*** Macros for GPIO_PD9 pin ***/
#define GPIO_PD9_Set()               (PIOD_REGS->PIO_SODR = (1<<9))
#define GPIO_PD9_Clear()             (PIOD_REGS->PIO_CODR = (1<<9))
#define GPIO_PD9_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<9))
#define GPIO_PD9_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<9))
#define GPIO_PD9_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<9))
#define GPIO_PD9_Get()               ((PIOD_REGS->PIO_PDSR >> 9) & 0x1)
#define GPIO_PD9_PIN                  PIO_PIN_PD9

/*** Macros for GPIO_PC18 pin ***/
#define GPIO_PC18_Set()               (PIOC_REGS->PIO_SODR = (1<<18))
#define GPIO_PC18_Clear()             (PIOC_REGS->PIO_CODR = (1<<18))
#define GPIO_PC18_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<18))
#define GPIO_PC18_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<18))
#define GPIO_PC18_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<18))
#define GPIO_PC18_Get()               ((PIOC_REGS->PIO_PDSR >> 18) & 0x1)
#define GPIO_PC18_PIN                  PIO_PIN_PC18

/*** Macros for GPIO_PA28 pin ***/
#define GPIO_PA28_Set()               (PIOA_REGS->PIO_SODR = (1<<28))
#define GPIO_PA28_Clear()             (PIOA_REGS->PIO_CODR = (1<<28))
#define GPIO_PA28_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<28))
#define GPIO_PA28_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<28))
#define GPIO_PA28_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<28))
#define GPIO_PA28_Get()               ((PIOA_REGS->PIO_PDSR >> 28) & 0x1)
#define GPIO_PA28_PIN                  PIO_PIN_PA28

/*** Macros for GPIO_PD8 pin ***/
#define GPIO_PD8_Set()               (PIOD_REGS->PIO_SODR = (1<<8))
#define GPIO_PD8_Clear()             (PIOD_REGS->PIO_CODR = (1<<8))
#define GPIO_PD8_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<8))
#define GPIO_PD8_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<8))
#define GPIO_PD8_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<8))
#define GPIO_PD8_Get()               ((PIOD_REGS->PIO_PDSR >> 8) & 0x1)
#define GPIO_PD8_PIN                  PIO_PIN_PD8

/*** Macros for GPIO_PC19 pin ***/
#define GPIO_PC19_Set()               (PIOC_REGS->PIO_SODR = (1<<19))
#define GPIO_PC19_Clear()             (PIOC_REGS->PIO_CODR = (1<<19))
#define GPIO_PC19_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<19))
#define GPIO_PC19_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<19))
#define GPIO_PC19_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<19))
#define GPIO_PC19_Get()               ((PIOC_REGS->PIO_PDSR >> 19) & 0x1)
#define GPIO_PC19_PIN                  PIO_PIN_PC19
#define GPIO_PC19_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<19))
#define GPIO_PC19_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<19))

/*** Macros for GPIO_PD7 pin ***/
#define GPIO_PD7_Set()               (PIOD_REGS->PIO_SODR = (1<<7))
#define GPIO_PD7_Clear()             (PIOD_REGS->PIO_CODR = (1<<7))
#define GPIO_PD7_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<7))
#define GPIO_PD7_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<7))
#define GPIO_PD7_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<7))
#define GPIO_PD7_Get()               ((PIOD_REGS->PIO_PDSR >> 7) & 0x1)
#define GPIO_PD7_PIN                  PIO_PIN_PD7

/*** Macros for GPIO_PC20 pin ***/
#define GPIO_PC20_Set()               (PIOC_REGS->PIO_SODR = (1<<20))
#define GPIO_PC20_Clear()             (PIOC_REGS->PIO_CODR = (1<<20))
#define GPIO_PC20_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<20))
#define GPIO_PC20_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<20))
#define GPIO_PC20_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<20))
#define GPIO_PC20_Get()               ((PIOC_REGS->PIO_PDSR >> 20) & 0x1)
#define GPIO_PC20_PIN                  PIO_PIN_PC20

/*** Macros for GPIO_PD6 pin ***/
#define GPIO_PD6_Set()               (PIOD_REGS->PIO_SODR = (1<<6))
#define GPIO_PD6_Clear()             (PIOD_REGS->PIO_CODR = (1<<6))
#define GPIO_PD6_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<6))
#define GPIO_PD6_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<6))
#define GPIO_PD6_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<6))
#define GPIO_PD6_Get()               ((PIOD_REGS->PIO_PDSR >> 6) & 0x1)
#define GPIO_PD6_PIN                  PIO_PIN_PD6

/*** Macros for GPIO_PC21 pin ***/
#define GPIO_PC21_Set()               (PIOC_REGS->PIO_SODR = (1<<21))
#define GPIO_PC21_Clear()             (PIOC_REGS->PIO_CODR = (1<<21))
#define GPIO_PC21_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<21))
#define GPIO_PC21_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<21))
#define GPIO_PC21_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<21))
#define GPIO_PC21_Get()               ((PIOC_REGS->PIO_PDSR >> 21) & 0x1)
#define GPIO_PC21_PIN                  PIO_PIN_PC21

/*** Macros for GPIO_PC22 pin ***/
#define GPIO_PC22_Set()               (PIOC_REGS->PIO_SODR = (1<<22))
#define GPIO_PC22_Clear()             (PIOC_REGS->PIO_CODR = (1<<22))
#define GPIO_PC22_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<22))
#define GPIO_PC22_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<22))
#define GPIO_PC22_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<22))
#define GPIO_PC22_Get()               ((PIOC_REGS->PIO_PDSR >> 22) & 0x1)
#define GPIO_PC22_PIN                  PIO_PIN_PC22

/*** Macros for GPIO_PD5 pin ***/
#define GPIO_PD5_Set()               (PIOD_REGS->PIO_SODR = (1<<5))
#define GPIO_PD5_Clear()             (PIOD_REGS->PIO_CODR = (1<<5))
#define GPIO_PD5_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<5))
#define GPIO_PD5_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<5))
#define GPIO_PD5_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<5))
#define GPIO_PD5_Get()               ((PIOD_REGS->PIO_PDSR >> 5) & 0x1)
#define GPIO_PD5_PIN                  PIO_PIN_PD5

/*** Macros for GPIO_PD4 pin ***/
#define GPIO_PD4_Set()               (PIOD_REGS->PIO_SODR = (1<<4))
#define GPIO_PD4_Clear()             (PIOD_REGS->PIO_CODR = (1<<4))
#define GPIO_PD4_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<4))
#define GPIO_PD4_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<4))
#define GPIO_PD4_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<4))
#define GPIO_PD4_Get()               ((PIOD_REGS->PIO_PDSR >> 4) & 0x1)
#define GPIO_PD4_PIN                  PIO_PIN_PD4

/*** Macros for GPIO_PC23 pin ***/
#define GPIO_PC23_Set()               (PIOC_REGS->PIO_SODR = (1<<23))
#define GPIO_PC23_Clear()             (PIOC_REGS->PIO_CODR = (1<<23))
#define GPIO_PC23_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<23))
#define GPIO_PC23_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<23))
#define GPIO_PC23_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<23))
#define GPIO_PC23_Get()               ((PIOC_REGS->PIO_PDSR >> 23) & 0x1)
#define GPIO_PC23_PIN                  PIO_PIN_PC23

/*** Macros for GPIO_PD3 pin ***/
#define GPIO_PD3_Set()               (PIOD_REGS->PIO_SODR = (1<<3))
#define GPIO_PD3_Clear()             (PIOD_REGS->PIO_CODR = (1<<3))
#define GPIO_PD3_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<3))
#define GPIO_PD3_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<3))
#define GPIO_PD3_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<3))
#define GPIO_PD3_Get()               ((PIOD_REGS->PIO_PDSR >> 3) & 0x1)
#define GPIO_PD3_PIN                  PIO_PIN_PD3

/*** Macros for GPIO_PA29 pin ***/
#define GPIO_PA29_Set()               (PIOA_REGS->PIO_SODR = (1<<29))
#define GPIO_PA29_Clear()             (PIOA_REGS->PIO_CODR = (1<<29))
#define GPIO_PA29_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<29))
#define GPIO_PA29_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<29))
#define GPIO_PA29_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<29))
#define GPIO_PA29_Get()               ((PIOA_REGS->PIO_PDSR >> 29) & 0x1)
#define GPIO_PA29_PIN                  PIO_PIN_PA29

/*** Macros for GPIO_PD2 pin ***/
#define GPIO_PD2_Set()               (PIOD_REGS->PIO_SODR = (1<<2))
#define GPIO_PD2_Clear()             (PIOD_REGS->PIO_CODR = (1<<2))
#define GPIO_PD2_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<2))
#define GPIO_PD2_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<2))
#define GPIO_PD2_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<2))
#define GPIO_PD2_Get()               ((PIOD_REGS->PIO_PDSR >> 2) & 0x1)
#define GPIO_PD2_PIN                  PIO_PIN_PD2

/*** Macros for GPIO_PB9 pin ***/
#define GPIO_PB9_Set()               (PIOB_REGS->PIO_SODR = (1<<9))
#define GPIO_PB9_Clear()             (PIOB_REGS->PIO_CODR = (1<<9))
#define GPIO_PB9_Toggle()            (PIOB_REGS->PIO_ODSR ^= (1<<9))
#define GPIO_PB9_OutputEnable()      (PIOB_REGS->PIO_OER = (1<<9))
#define GPIO_PB9_InputEnable()       (PIOB_REGS->PIO_ODR = (1<<9))
#define GPIO_PB9_Get()               ((PIOB_REGS->PIO_PDSR >> 9) & 0x1)
#define GPIO_PB9_PIN                  PIO_PIN_PB9


// *****************************************************************************
/* PIO Port

  Summary:
    Identifies the available PIO Ports.

  Description:
    This enumeration identifies the available PIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    PIO_PORT_A = PIOA_BASE_ADDRESS,
    PIO_PORT_B = PIOB_BASE_ADDRESS,
    PIO_PORT_C = PIOC_BASE_ADDRESS,
    PIO_PORT_D = PIOD_BASE_ADDRESS,
    PIO_PORT_E = PIOE_BASE_ADDRESS
} PIO_PORT;

// *****************************************************************************
/* PIO Port Pins

  Summary:
    Identifies the available PIO port pins.

  Description:
    This enumeration identifies the available PIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    PIO_PIN_PA0 = 0,
    PIO_PIN_PA1 = 1,
    PIO_PIN_PA2 = 2,
    PIO_PIN_PA3 = 3,
    PIO_PIN_PA4 = 4,
    PIO_PIN_PA5 = 5,
    PIO_PIN_PA6 = 6,
    PIO_PIN_PA7 = 7,
    PIO_PIN_PA8 = 8,
    PIO_PIN_PA9 = 9,
    PIO_PIN_PA10 = 10,
    PIO_PIN_PA11 = 11,
    PIO_PIN_PA12 = 12,
    PIO_PIN_PA13 = 13,
    PIO_PIN_PA14 = 14,
    PIO_PIN_PA15 = 15,
    PIO_PIN_PA16 = 16,
    PIO_PIN_PA17 = 17,
    PIO_PIN_PA18 = 18,
    PIO_PIN_PA19 = 19,
    PIO_PIN_PA20 = 20,
    PIO_PIN_PA21 = 21,
    PIO_PIN_PA22 = 22,
    PIO_PIN_PA23 = 23,
    PIO_PIN_PA24 = 24,
    PIO_PIN_PA25 = 25,
    PIO_PIN_PA26 = 26,
    PIO_PIN_PA27 = 27,
    PIO_PIN_PA28 = 28,
    PIO_PIN_PA29 = 29,
    PIO_PIN_PA30 = 30,
    PIO_PIN_PA31 = 31,
    PIO_PIN_PB0 = 32,
    PIO_PIN_PB1 = 33,
    PIO_PIN_PB2 = 34,
    PIO_PIN_PB3 = 35,
    PIO_PIN_PB4 = 36,
    PIO_PIN_PB5 = 37,
    PIO_PIN_PB6 = 38,
    PIO_PIN_PB7 = 39,
    PIO_PIN_PB8 = 40,
    PIO_PIN_PB9 = 41,
    PIO_PIN_PB12 = 44,
    PIO_PIN_PB13 = 45,
    PIO_PIN_PC0 = 64,
    PIO_PIN_PC1 = 65,
    PIO_PIN_PC2 = 66,
    PIO_PIN_PC3 = 67,
    PIO_PIN_PC4 = 68,
    PIO_PIN_PC5 = 69,
    PIO_PIN_PC6 = 70,
    PIO_PIN_PC7 = 71,
    PIO_PIN_PC8 = 72,
    PIO_PIN_PC9 = 73,
    PIO_PIN_PC10 = 74,
    PIO_PIN_PC11 = 75,
    PIO_PIN_PC12 = 76,
    PIO_PIN_PC13 = 77,
    PIO_PIN_PC14 = 78,
    PIO_PIN_PC15 = 79,
    PIO_PIN_PC16 = 80,
    PIO_PIN_PC17 = 81,
    PIO_PIN_PC18 = 82,
    PIO_PIN_PC19 = 83,
    PIO_PIN_PC20 = 84,
    PIO_PIN_PC21 = 85,
    PIO_PIN_PC22 = 86,
    PIO_PIN_PC23 = 87,
    PIO_PIN_PC24 = 88,
    PIO_PIN_PC25 = 89,
    PIO_PIN_PC26 = 90,
    PIO_PIN_PC27 = 91,
    PIO_PIN_PC28 = 92,
    PIO_PIN_PC29 = 93,
    PIO_PIN_PC30 = 94,
    PIO_PIN_PC31 = 95,
    PIO_PIN_PD0 = 96,
    PIO_PIN_PD1 = 97,
    PIO_PIN_PD2 = 98,
    PIO_PIN_PD3 = 99,
    PIO_PIN_PD4 = 100,
    PIO_PIN_PD5 = 101,
    PIO_PIN_PD6 = 102,
    PIO_PIN_PD7 = 103,
    PIO_PIN_PD8 = 104,
    PIO_PIN_PD9 = 105,
    PIO_PIN_PD10 = 106,
    PIO_PIN_PD11 = 107,
    PIO_PIN_PD12 = 108,
    PIO_PIN_PD13 = 109,
    PIO_PIN_PD14 = 110,
    PIO_PIN_PD15 = 111,
    PIO_PIN_PD16 = 112,
    PIO_PIN_PD17 = 113,
    PIO_PIN_PD18 = 114,
    PIO_PIN_PD19 = 115,
    PIO_PIN_PD20 = 116,
    PIO_PIN_PD21 = 117,
    PIO_PIN_PD22 = 118,
    PIO_PIN_PD23 = 119,
    PIO_PIN_PD24 = 120,
    PIO_PIN_PD25 = 121,
    PIO_PIN_PD26 = 122,
    PIO_PIN_PD27 = 123,
    PIO_PIN_PD28 = 124,
    PIO_PIN_PD29 = 125,
    PIO_PIN_PD30 = 126,
    PIO_PIN_PD31 = 127,
    PIO_PIN_PE0 = 128,
    PIO_PIN_PE1 = 129,
    PIO_PIN_PE2 = 130,
    PIO_PIN_PE3 = 131,
    PIO_PIN_PE4 = 132,
    PIO_PIN_PE5 = 133,

    /* This element should not be used in any of the PIO APIs.
       It will be used by other modules or application to denote that none of the PIO Pin is used */
    PIO_PIN_NONE = -1

} PIO_PIN;

typedef  void (*PIO_PIN_CALLBACK) ( PIO_PIN pin, uintptr_t context);

void PIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: PIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t PIO_PortRead(PIO_PORT port);

void PIO_PortWrite(PIO_PORT port, uint32_t mask, uint32_t value);

uint32_t PIO_PortLatchRead ( PIO_PORT port );

void PIO_PortSet(PIO_PORT port, uint32_t mask);

void PIO_PortClear(PIO_PORT port, uint32_t mask);

void PIO_PortToggle(PIO_PORT port, uint32_t mask);

void PIO_PortInputEnable(PIO_PORT port, uint32_t mask);

void PIO_PortOutputEnable(PIO_PORT port, uint32_t mask);

void PIO_PortInterruptEnable(PIO_PORT port, uint32_t mask);

void PIO_PortInterruptDisable(PIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

typedef struct {

    /* target pin */
    PIO_PIN                 pin;

    /* Callback for event on target pin*/
    PIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} PIO_PIN_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: PIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void PIO_PinWrite(PIO_PIN pin, bool value)
{
    PIO_PortWrite((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), (uint32_t)(0x1) << (pin & 0x1f), (uint32_t)(value) << (pin & 0x1f));
}

static inline bool PIO_PinRead(PIO_PIN pin)
{
    return (bool)((PIO_PortRead((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5)))) >> (pin & 0x1F)) & 0x1);
}

static inline bool PIO_PinLatchRead(PIO_PIN pin)
{
    return (bool)((PIO_PortLatchRead((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5)))) >> (pin & 0x1F)) & 0x1);
}

static inline void PIO_PinToggle(PIO_PIN pin)
{
    PIO_PortToggle((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinSet(PIO_PIN pin)
{
    PIO_PortSet((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinClear(PIO_PIN pin)
{
    PIO_PortClear((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinInputEnable(PIO_PIN pin)
{
    PIO_PortInputEnable((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinOutputEnable(PIO_PIN pin)
{
    PIO_PortOutputEnable((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinInterruptEnable(PIO_PIN pin)
{
    PIO_PortInterruptEnable((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinInterruptDisable(PIO_PIN pin)
{
    PIO_PortInterruptDisable((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

bool PIO_PinInterruptCallbackRegister(
    PIO_PIN pin,
    const   PIO_PIN_CALLBACK callBack,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_PIO_H
