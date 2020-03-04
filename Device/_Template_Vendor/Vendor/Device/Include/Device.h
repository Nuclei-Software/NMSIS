/*
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 * Copyright (c) 2019 Nuclei Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 * @file     <Device>.h
 * @brief    NMSIS Nuclei N/NX Core Peripheral Access Layer Header File for
 *           Device <Device>
 * @version  V1.00
 * @date     17. Dec 2019
 ******************************************************************************/

#ifndef __<Device>_H__     /* TODO: replace '<Device>' with your device name */
#define __<Device>_H__

#ifdef __cplusplus
extern "C" {
#endif

/* TODO: replace '<Vendor>' with vendor name; add your doxygen comment   */
/** @addtogroup <Vendor>
  * @{
  */


/* TODO: replace '<Device>' with device name; add your doxygen comment   */
/** @addtogroup <Device>
  * @{
  */


/** @addtogroup Configuration_of_NMSIS
  * @{
  */



/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn {
/* ========================================  Nuclei N/NX Specific Interrupt Numbers  ========================================= */

/* TODO: use this N/NX interrupt numbers if your device is a Nuclei N/NX device */
    Reserved0_IRQn            =   0,              /*!<  Internal reserved */
    Reserved1_IRQn            =   1,              /*!<  Internal reserved */
    Reserved2_IRQn            =   2,              /*!<  Internal reserved */
    SysTimerSW_IRQn           =   3,              /*!<  System Timer SW interrupt */
    Reserved3_IRQn            =   4,              /*!<  Internal reserved */
    Reserved4_IRQn            =   5,              /*!<  Internal reserved */
    Reserved5_IRQn            =   6,              /*!<  Internal reserved */
    SysTimer_IRQn             =   7,              /*!<  System Timer Interrupt */
    Reserved6_IRQn            =   8,              /*!<  Internal reserved */
    Reserved7_IRQn            =   9,              /*!<  Internal reserved */
    Reserved8_IRQn            =  10,              /*!<  Internal reserved */
    Reserved9_IRQn            =  11,              /*!<  Internal reserved */
    Reserved10_IRQn           =  12,              /*!<  Internal reserved */
    Reserved11_IRQn           =  13,              /*!<  Internal reserved */
    Reserved12_IRQn           =  14,              /*!<  Internal reserved */
    Reserved13_IRQn           =  15,              /*!<  Internal reserved */
    Reserved14_IRQn           =  16,              /*!<  Internal reserved */
    Reserved15_IRQn           =  17,              /*!<  Internal reserved */
    Reserved16_IRQn           =  18,              /*!<  Internal reserved */

/* ===========================================  <Device> Specific Interrupt Numbers  ========================================= */
/* TODO: add here your device specific external interrupt numbers. 19~1023 is reserved number for user. Maxmum interrupt supported
         could get from clicinfo.NUM_INTERRUPT. According the interrupt handlers defined in startup_Device.s
         eg.: Interrupt for Timer#1       eclic_tim0_handler   ->   TIM0_IRQn */
    <DeviceInterrupt>_IRQn    = 19,                /*!< Device Interrupt */

    SOC_INT_MAX,                                   /* Max SoC interrupt Number */
} IRQn_Type;

/* =========================================================================================================================== */
/* ================                                  Exception Code Definition                                ================ */
/* =========================================================================================================================== */

typedef enum EXCn {
/* =======================================  Nuclei N/NX Specific Exception Code  ======================================== */
    InsUnalign_EXCn          =   0,              /*!<  Instruction address misaligned */
    InsAccFault_EXCn         =   1,              /*!<  Instruction access fault */
    IlleIns_EXCn             =   2,              /*!<  Illegal instruction */
    Break_EXCn               =   3,              /*!<  Beakpoint */
    LdAddrUnalign_EXCn       =   4,              /*!<  Load address misaligned */
    LdFault_EXCn             =   5,              /*!<  Load access fault */
    StAddrUnalign_EXCn       =   6,              /*!<  Store or AMO address misaligned */
    StAccessFault_EXCn       =   7,              /*!<  Store or AMO access fault */
    UmodeEcall_EXCn          =   8,              /*!<  Environment call from User mode */
    MmodeEcall_EXCn          =  11,              /*!<  Environment call from Machine mode */
    NMI_EXCn                 = 0xfff,            /*!<  NMI interrupt*/
} EXCn_Type;

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ===========================  Configuration of the Nuclei N/NX Processor and Core Peripherals  =========================== */
/* TODO: set the defines according your Device */
/* TODO: define the correct core revision
 *       __NUCLEI_N_REV if your device is a Nuclei-N Class device
 *       __NUCLEI_NX_REV if your device is a Nuclei-NX Class device
 */
#define __NUCLEI_N#_REV           0x0100                /*!< Core Revision rXpY, version X.Y, change N# to N for Nuclei N class cores, change N# to NX for Nuclei NX cores */
/* TODO: define the correct core features for the <Device> */
#define __ECLIC_PRESENT           1                     /*!< Set to 1 if ECLIC is present */
#define __ECLIC_BASEADDR          0x0C000000UL          /*!< Set to ECLIC baseaddr of your device */
#define __ECLIC_INTCTLBITS        8                     /*!< Set to 1 - 8, the number of hardware bits are actually implemented in the clicintctl registers. */
#define __ECLIC_INTNUM            51                    /*!< Set to 1 - 1024, total interrupt number of ECLIC Unit */
#define __SYSTIMER_PRESENT        1                     /*!< Set to 1 if System Timer is present */
#define __SYSTIMER_BASEADDR       0x02000000UL          /*!< Set to SysTimer baseaddr of your device */
#define __FPU_PRESENT             1                     /*!< Set to 0, 1, or 2, 0 not present, 1 single floating point unit present, 2 double floating point unit present */
#define __DSP_PRESENT             1                     /*!< Set to 1 if DSP is present */
#define __PMP_PRESENT             1                     /*!< Set to 1 if PMP is present */
#define __PMP_ENTRY_NUM           16                    /*!< Set to 8 or 16, the number of PMP entries */
#define __ICACHE_PRESENT          0                     /*!< Set to 1 if I-Cache is present */
#define __DCACHE_PRESENT          0                     /*!< Set to 1 if D-Cache is present */
#define __Vendor_SysTickConfig    0                     /*!< Set to 1 if different SysTick Config is used */

/** @} */ /* End of group Configuration_of_NMSIS */


#include <nmsis_core.h>
/* TODO: include your system_<Device>.h file
         replace '<Device>' with your device name */
#include "system_<Device>.h"                    /*!< <Device> System */


/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */


/** @addtogroup Device_Peripheral_peripherals
  * @{
  */

/* TODO: add here your device specific peripheral access structure typedefs
         following is an example for UART */

/* =========================================================================================================================== */
/* ================                                  UART                                                     ================ */
/* =========================================================================================================================== */

/**
  * @brief UART (UART)
  */
typedef struct {                            /*!< (@ 0x40000000) UART Structure                  */
  __IOM uint32_t   TXFIFO;                  /*!< (@ 0x00000000) UART TX FIFO                    */
  __IM  uint32_t   RXFIFO;                  /*!< (@ 0x00000004) UART RX FIFO                    */
  __IOM uint32_t   TXCTRL;                  /*!< (@ 0x00000008) UART TX FIFO control            */
  __OM  uint32_t   RXCTRL;                  /*!< (@ 0x0000000C) UART RX FIFO control            */
  __IM  uint32_t   IE;                      /*!< (@ 0x00000010) UART Interrupt Enable flag      */
  __IM  uint32_t   IP;                      /*!< (@ 0x00000018) TART Interrupt Pending flag     */
  __IM  uint32_t   DIV;                     /*!< (@ 0x00000018) UART Baudrate Divider           */
} <DeviceAbbreviation>_UART_TypeDef;

/*@}*/ /* end of group <Device>_Peripherals */


/* =========================================  End of section using anonymous unions  ========================================= */
#if defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif


/* =========================================================================================================================== */
/* ================                          Device Specific Peripheral Address Map                           ================ */
/* =========================================================================================================================== */


/* TODO: add here your device peripherals base addresses
         following is an example for timer */
/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
  */

/* Peripheral and SRAM base address */
#define <DeviceAbbreviation>_FLASH_BASE         (0x00000000UL)                              /*!< (FLASH     ) Base Address */
#define <DeviceAbbreviation>_SRAM_BASE          (0x20000000UL)                              /*!< (SRAM      ) Base Address */
#define <DeviceAbbreviation>_PERIPH_BASE        (0x40000000UL)                              /*!< (Peripheral) Base Address */

/* Peripheral memory map */
#define <DeviceAbbreviation>UART0_BASE          (<DeviceAbbreviation>_PERIPH_BASE)          /*!< (UART 0  ) Base Address */
#define <DeviceAbbreviation>I2C_BASE            (<DeviceAbbreviation>_PERIPH_BASE + 0x0800) /*!< (I2C    ) Base Address */
#define <DeviceAbbreviation>GPIO_BASE           (<DeviceAbbreviation>_PERIPH_BASE + 0x1000) /*!< (GPIO    ) Base Address */

/** @} */ /* End of group Device_Peripheral_peripheralAddr */


/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */


/* TODO: add here your device peripherals pointer definitions
         following is an example for uart0 */
/** @addtogroup Device_Peripheral_declaration
  * @{
  */
#define <DeviceAbbreviation>_UART0              ((<DeviceAbbreviation>_TMR_TypeDef *) <DeviceAbbreviation>UART0_BASE)


/** @} */ /* End of group <Device> */

/** @} */ /* End of group <Vendor> */

#ifdef __cplusplus
}
#endif

#endif  /* __<Device>_H__ */
