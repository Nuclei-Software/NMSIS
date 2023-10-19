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
 * @version  V2.1.0
 * @date     19. Dec 2023
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

/** \brief SoC Download mode definition */
/* TODO: device vendor can extend more download modes */
typedef enum {
    DOWNLOAD_MODE_FLASHXIP = 0,         /*!< Flashxip download mode */
    DOWNLOAD_MODE_FLASH = 1,            /*!< Flash download mode */
    DOWNLOAD_MODE_ILM = 2,              /*!< ilm download mode */
    DOWNLOAD_MODE_DDR = 3,              /*!< ddr download mode */
    DOWNLOAD_MODE_SRAM = 4,             /*!< sram download mode */
    DOWNLOAD_MODE_MAX,
} DownloadMode_Type;

/** \brief CPU Internal Region Information */
typedef struct IRegion_Info {
    unsigned long iregion_base;         /*!< Internal region base address */
    unsigned long eclic_base;           /*!< eclic base address */
    unsigned long systimer_base;        /*!< system timer base address */
    unsigned long smp_base;             /*!< smp base address */
    unsigned long idu_base;             /*!< idu base address */
} IRegion_Info_Type;

/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn {
/* ========================================  Nuclei N/NX Specific Interrupt Numbers  ========================================= */

/* TODO: use this N/NX interrupt numbers if your device is a Nuclei N/NX device */
    Reserved0_IRQn            =   0,              /*!<  Internal reserved */
    Reserved1_IRQn            =   1,              /*!<  Internal reserved */
    Reserved2_IRQn            =   2,              /*!<  Internal reserved */
    SysTimerSW_IRQn           =   3,              /*!<  System Timer SW interrupt for both M/S mode in ECLIC */
    Reserved3_IRQn            =   4,              /*!<  Internal reserved */
    Reserved4_IRQn            =   5,              /*!<  Internal reserved */
    Reserved5_IRQn            =   6,              /*!<  Internal reserved */
    SysTimer_IRQn             =   7,              /*!<  System Timer Interrupt for both M/S mode in ECLIC */
    Reserved6_IRQn            =   8,              /*!<  Internal reserved */
    Reserved7_IRQn            =   9,              /*!<  Internal reserved */
    Reserved8_IRQn            =  10,              /*!<  Internal reserved */
    Reserved9_IRQn            =  11,              /*!<  Internal reserved */
    Reserved10_IRQn           =  12,              /*!<  Internal reserved */
    Reserved11_IRQn           =  13,              /*!<  Internal reserved */
    Reserved12_IRQn           =  14,              /*!<  Internal reserved */
    Reserved13_IRQn           =  15,              /*!<  Internal reserved */
    InterCore_IRQn            =  16,              /*!<  CIDU Inter Core Interrupt */
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
    SmodeEcall_EXCn          =   9,              /*!<  Environment call from S-mode */
    MmodeEcall_EXCn          =  11,              /*!<  Environment call from Machine mode */
    InsPageFault_EXCn        =  12,              /*!<  Instruction page fault */
    LdPageFault_EXCn         =  13,              /*!<  Load page fault */
    StPageFault_EXCn         =  15,              /*!<  Store or AMO page fault */
    StackOverflow_EXCn       =  24,              /*!<  Stack overflow fault */
    StackUnderflow_EXCn      =  25,              /*!<  Stack underflow fault */
    NMI_EXCn                 =  0xfff,           /*!<  NMI interrupt */
} EXCn_Type;

/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */
extern volatile IRegion_Info_Type SystemIRegionInfo;
/* ===========================  Configuration of the Nuclei N/NX Processor and Core Peripherals  =========================== */
/* TODO: set the defines according your Device */
/* TODO: define the correct core revision
 *       __NUCLEI_N_REV if your device is a Nuclei-N Class device
 *       __NUCLEI_NX_REV if your device is a Nuclei-NX Class device
 */
#define __NUCLEI_N#_REV           0x0100                /*!< Core Revision rXpY, version X.Y, change N# to N for Nuclei N class cores, change N# to NX for Nuclei NX cores */
/* TODO: define the correct core features for the <Device> */
/**
 * If your hart index is different to your hartid, you must define this __HARTID_OFFSET macro.
 * For example, if your cpu has 4 harts, and hartid start from 3, so the __HARTID_OFFSET should set to 3.
 * Which means hartid 3-6 means hart index 0-3, this is useful for the timer software interrupt and timer interrupt trigger register location
 */
#ifndef __HARTID_OFFSET
#define __HARTID_OFFSET           0
#endif
#define __ECLIC_PRESENT           1                     /*!< Set to 1 if ECLIC is present */
#define __ECLIC_BASEADDR          SystemIRegionInfo.eclic_base           /*!< Set to ECLIC baseaddr of your device */
#define __ECLIC_INTCTLBITS        8                     /*!< Set to 1 - 8, the number of hardware bits are actually implemented in the clicintctl registers. */
#define __ECLIC_INTNUM            64                    /*!< Set to 1 - 1024, total interrupt number of ECLIC Unit */
#define __SYSTIMER_PRESENT        1                     /*!< Set to 1 if System Timer is present */
#define __SYSTIMER_BASEADDR       SystemIRegionInfo.systimer_base          /*!< Set to SysTimer baseaddr of your device */
#define __CIDU_PRESENT            0                     /*!< Set to 1 if CIDU is present */
#define __CIDU_BASEADDR           SystemIRegionInfo.idu_base              /*!< Set to cidu baseaddr of your device */
#define __FPU_PRESENT             1                     /*!< Set to 0, 1, or 2, 0 not present, 1 single floating point unit present, 2 double floating point unit present */
#define __BITMANIP_PRESENT        1                     /*!< Set to 1 if Bitmainpulation extension is present */
#define __DSP_PRESENT             1                     /*!< Set to 1 if DSP is present */
#define __VECTOR_PRESENT          1                     /*!< Set to 1 if Vector extension is present */
#define __PMP_PRESENT             1                     /*!< Set to 1 if PMP is present */
#define __PMP_ENTRY_NUM           16                    /*!< Set to 8 or 16, the number of PMP entries */
#define __SPMP_PRESENT            1                     /*!< Set to 1 if SPMP is present */
#define __SPMP_ENTRY_NUM          16                    /*!< Set to 8 or 16, the number of SPMP entries */
#define __TEE_PRESENT             0                     /*!< Set to 1 if TEE is present */
#define __ICACHE_PRESENT          0                     /*!< Set to 1 if I-Cache is present */
#define __DCACHE_PRESENT          0                     /*!< Set to 1 if D-Cache is present */
#define __CCM_PRESENT             0                     /*!< Set to 1 if Cache Control and Mantainence Unit is present */
#define __INC_INTRINSIC_API       0                     /*!< Set to 1 if intrinsic api header files need to be included */
#define __Vendor_SysTickConfig    0                     /*!< Set to 1 if different SysTick Config is used */
#define __Vendor_EXCEPTION        0                     /*!< Set to 1 if vendor exception hander is present */

/** @} */ /* End of group Configuration_of_NMSIS */


#include <nmsis_core.h>
/* TODO: include your system_<Device>.h file
         replace '<Device>' with your device name */
#include "system_<Device>.h"                    /*!< <Device> System */


/* ========================================  Start of section using anonymous unions  ======================================== */


/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */
/* Macros for memory access operations */
#define _REG8P(p, i)                        ((volatile uint8_t *) ((uintptr_t)((p) + (i))))
#define _REG16P(p, i)                       ((volatile uint16_t *) ((uintptr_t)((p) + (i))))
#define _REG32P(p, i)                       ((volatile uint32_t *) ((uintptr_t)((p) + (i))))
#define _REG64P(p, i)                       ((volatile uint64_t *) ((uintptr_t)((p) + (i))))
#define _REG8(p, i)                         (*(_REG8P(p, i)))
#define _REG16(p, i)                        (*(_REG16P(p, i)))
#define _REG32(p, i)                        (*(_REG32P(p, i)))
#define _REG64(p, i)                        (*(_REG64P(p, i)))
#define REG8(addr)                          _REG8((addr), 0)
#define REG16(addr)                         _REG16((addr), 0)
#define REG32(addr)                         _REG32((addr), 0)
#define REG64(addr)                         _REG64((addr), 0)

/* Macros for address type convert and access operations */
#define ADDR16(addr)                        ((uint16_t)(uintptr_t)(addr))
#define ADDR32(addr)                        ((uint32_t)(uintptr_t)(addr))
#define ADDR64(addr)                        ((uint64_t)(uintptr_t)(addr))
#define ADDR8P(addr)                        ((uint8_t *)(uintptr_t)(addr))
#define ADDR16P(addr)                       ((uint16_t *)(uintptr_t)(addr))
#define ADDR32P(addr)                       ((uint32_t *)(uintptr_t)(addr))
#define ADDR64P(addr)                       ((uint64_t *)(uintptr_t)(addr))

/* Macros for Bit Operations */
#if __riscv_xlen == 32
#define BITMASK_MAX                         0xFFFFFFFFUL
#define BITOFS_MAX                          31
#else
#define BITMASK_MAX                         0xFFFFFFFFFFFFFFFFULL
#define BITOFS_MAX                          63
#endif

// BIT/BITS only support bit mask for __riscv_xlen
// For RISC-V 32 bit, it support mask 32 bit wide
// For RISC-V 64 bit, it support mask 64 bit wide
#define BIT(ofs)                            (0x1UL << (ofs))
#define BITS(start, end)                    ((BITMASK_MAX) << (start) & (BITMASK_MAX) >> (BITOFS_MAX - (end)))
#define GET_BIT(regval, bitofs)             (((regval) >> (bitofs)) & 0x1)
#define SET_BIT(regval, bitofs)             ((regval) |= BIT(bitofs))
#define CLR_BIT(regval, bitofs)             ((regval) &= (~BIT(bitofs)))
#define FLIP_BIT(regval, bitofs)            ((regval) ^= BIT(bitofs))
#define WRITE_BIT(regval, bitofs, val)      CLR_BIT(regval, bitofs); ((regval) |= ((val) << bitofs) & BIT(bitofs))
#define CHECK_BIT(regval, bitofs)           (!!((regval) & (0x1UL<<(bitofs))))
#define GET_BITS(regval, start, end)        (((regval) & BITS((start), (end))) >> (start))
#define SET_BITS(regval, start, end)        ((regval) |= BITS((start), (end)))
#define CLR_BITS(regval, start, end)        ((regval) &= (~BITS((start), (end))))
#define FLIP_BITS(regval, start, end)       ((regval) ^= BITS((start), (end)))
#define WRITE_BITS(regval, start, end, val) CLR_BITS(regval, start, end); ((regval) |= ((val) << start) & BITS((start), (end)))
#define CHECK_BITS_ALL(regval, start, end)  (!((~(regval)) & BITS((start), (end))))
#define CHECK_BITS_ANY(regval, start, end)  ((regval) & BITS((start), (end)))

#define BITMASK_SET(regval, mask)           ((regval) |= (mask))
#define BITMASK_CLR(regval, mask)           ((regval) &= (~(mask)))
#define BITMASK_FLIP(regval, mask)          ((regval) ^= (mask))
#define BITMASK_CHECK_ALL(regval, mask)     (!((~(regval)) & (mask)))
#define BITMASK_CHECK_ANY(regval, mask)     ((regval) & (mask))

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
