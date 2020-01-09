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
 * @file     NUCLEI_N.h
 * @brief    NMSIS Nuclei Core Peripheral Access Layer Header File for
 *           Nuclei N Device
 * @version  V1.00
 * @date     18. Dec 2019
 ******************************************************************************/

#ifndef __NUCLEI_N_H__
#define __NUCLEI_N_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup Nuclei
  * @{
  */


/** @addtogroup NUCLEI_N
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

/* ===========================================  NUCLEI_N Specific Interrupt Numbers  ========================================= */
    Interrupt19_IRQn    = 19,                /*!< Device Interrupt*/
    Interrupt20_IRQn    = 20,                /*!< Device Interrupt*/
    Interrupt21_IRQn    = 21,                /*!< Device Interrupt*/
    Interrupt22_IRQn    = 22,                /*!< Device Interrupt*/
    Interrupt23_IRQn    = 23,                /*!< Device Interrupt*/
    Interrupt24_IRQn    = 24,                /*!< Device Interrupt*/
    Interrupt25_IRQn    = 25,                /*!< Device Interrupt*/
    Interrupt26_IRQn    = 26,                /*!< Device Interrupt*/
    Interrupt27_IRQn    = 27,                /*!< Device Interrupt*/
    Interrupt28_IRQn    = 28,                /*!< Device Interrupt*/
    Interrupt29_IRQn    = 29,                /*!< Device Interrupt*/
    Interrupt30_IRQn    = 30,                /*!< Device Interrupt*/
    Interrupt31_IRQn    = 31,                /*!< Device Interrupt*/
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
/*       __NUCLEI_N_REV if your device is a Nuclei-N Class device
 */
#define __NUCLEI_N_REV            0x0104                /*!< Nuclei N Core Revision r1p4, version 1.4 */
#define __ECLIC_PRESENT           1                     /*!< Set to 1 if ECLIC is present */
#define __ECLIC_BASEADDR          0x0C000000UL          /*!< Set to ECLIC baseaddr of your device */
#define __ECLIC_INTCTLBITS        8                     /*!< Set to 1 - 8, the number of hardware bits are actually implemented in the clicintctl registers. */
#define __ECLIC_INTNUM            32                    /*!< Set to 1 - 1024, total interrupt number of ECLIC Unit */
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
#include "system_NUCLEI_N.h"                    /*!< NUCLEI_N System */

/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__GNUC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/** @} */ /* End of group NUCLEI_N */

/** @} */ /* End of group Nuclei */

#ifdef __cplusplus
}
#endif

#endif  /* __NUCLEI_N_H__ */
