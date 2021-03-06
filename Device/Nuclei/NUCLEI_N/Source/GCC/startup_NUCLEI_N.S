/*
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
 * \file     startup_NUCLEI_N.S
 * \brief    NMSIS Nuclei N/NX Class Core based Core Device Startup File for
 *           Device NUCLEI_N
 * \version  V1.00
 * \date     18. Dec 2019
 *
 ******************************************************************************/

#include "riscv_encoding.h"

.macro DECLARE_INT_HANDLER  INT_HDL_NAME
#if defined(__riscv_xlen) && (__riscv_xlen == 32)
    .word \INT_HDL_NAME
#else
    .dword \INT_HDL_NAME
#endif
.endm

/*
 * === Different Download and Running Mode ===
 * flashxip: Program will to be download into flash and run directly in Flash
 * flash: Program will be download into flash, when running, program will be copied to ilm/ram and run in ilm/ram
 * ilm: Program will be download into ilm/ram and run directly in ilm/ram, program lost when poweroff
 * ddr: Program will be download into ddr and run directly in ddr, program lost when poweroff
 */

/*** Vector Table Code Section ***/
    /*
     * Put the interrupt vectors in this section according to the run mode:
     * This startup code and link script file are configured for FlashXIP mode:
     * FlashXIP: .vtable
     * ILM: .vtable
     * DDR: .vtable
     * Flash: .vtable_ilm
     */
    .section .vtable

    .weak  eclic_msip_handler
    .weak  eclic_mtip_handler
    .weak  eclic_irq19_handler
    .weak  eclic_irq20_handler
    .weak  eclic_irq21_handler
    .weak  eclic_irq22_handler
    .weak  eclic_irq23_handler
    .weak  eclic_irq24_handler
    .weak  eclic_irq25_handler
    .weak  eclic_irq26_handler
    .weak  eclic_irq27_handler
    .weak  eclic_irq28_handler
    .weak  eclic_irq29_handler
    .weak  eclic_irq30_handler
    .weak  eclic_irq31_handler

    .globl vector_base
vector_base:
    j _start                                                /* 0: Reserved, Jump to _start when reset for ILM/FlashXIP mode.*/
    .align LOG_REGBYTES                                     /*    Need to align 4 byte for RV32, 8 Byte for RV64 */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 1: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 2: Reserved */
    DECLARE_INT_HANDLER     eclic_msip_handler              /* 3: Machine software interrupt */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 4: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 5: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 6: Reserved */
    DECLARE_INT_HANDLER     eclic_mtip_handler              /* 7: Machine timer interrupt */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 8: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 9: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 10: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 11: Reserved */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 12: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 13: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 14: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 15: Reserved */

    DECLARE_INT_HANDLER     default_intexc_handler          /* 16: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 17: Reserved */
    DECLARE_INT_HANDLER     default_intexc_handler          /* 18: Reserved */
    DECLARE_INT_HANDLER     eclic_irq19_handler             /* 19: Interrupt 19 */

    DECLARE_INT_HANDLER     eclic_irq20_handler             /* 20: Interrupt 20 */
    DECLARE_INT_HANDLER     eclic_irq21_handler             /* 21: Interrupt 21 */
    DECLARE_INT_HANDLER     eclic_irq22_handler             /* 22: Interrupt 22 */
    DECLARE_INT_HANDLER     eclic_irq23_handler             /* 23: Interrupt 23 */

    DECLARE_INT_HANDLER     eclic_irq24_handler             /* 24: Interrupt 24 */
    DECLARE_INT_HANDLER     eclic_irq25_handler             /* 25: Interrupt 25 */
    DECLARE_INT_HANDLER     eclic_irq26_handler             /* 26: Interrupt 26 */
    DECLARE_INT_HANDLER     eclic_irq27_handler             /* 27: Interrupt 27 */

    DECLARE_INT_HANDLER     eclic_irq28_handler             /* 28: Interrupt 28 */
    DECLARE_INT_HANDLER     eclic_irq29_handler             /* 29: Interrupt 29 */
    DECLARE_INT_HANDLER     eclic_irq30_handler             /* 30: Interrupt 30 */
    DECLARE_INT_HANDLER     eclic_irq31_handler             /* 31: Interrupt 31 */


/*** Startup Code Section ***/
    .section .init

    .globl _start
    .type _start,@function

/**
 * Reset Handler called on controller reset
 */
_start:
    /* ===== Startup Stage 1 ===== */
    /* Disable Global Interrupt */
    csrc CSR_MSTATUS, MSTATUS_MIE

    /* Initialize GP and Stack Pointer SP */
    .option push
    .option norelax
    la gp, __global_pointer$

    .option pop
    la sp, _sp

    /*
     * Set the the NMI base mnvec to share
     * with mtvec by setting CSR_MMISC_CTL
     * bit 9 NMI_CAUSE_FFF to 1
     */
    li t0, MMISC_CTL_NMI_CAUSE_FFF
    csrs CSR_MMISC_CTL, t0

    /*
     * Intialize ECLIC vector interrupt
     * base address mtvt to vector_base
     */
    la t0, vector_base
    csrw CSR_MTVT, t0

    /*
     * Set ECLIC non-vector entry to be controlled
     * by mtvt2 CSR register.
     * Intialize ECLIC non-vector interrupt
     * base address mtvt2 to irq_entry.
     */
    la t0, irq_entry
    csrw CSR_MTVT2, t0
    csrs CSR_MTVT2, 0x1

    /*
     * Set Exception Entry MTVEC to exc_entry
     * Due to settings above, Exception and NMI
     * will share common entry.
     */
    la t0, exc_entry
    csrw CSR_MTVEC, t0

    /* Set the interrupt processing mode to ECLIC mode */
    li t0, 0x3f
    csrc CSR_MTVEC, t0
    csrs CSR_MTVEC, 0x3

    /* ===== Startup Stage 2 ===== */

#ifdef __riscv_flen
    /* Enable FPU */
    li t0, MSTATUS_FS
    csrs mstatus, t0
    csrw fcsr, x0
#endif

    /* Enable mcycle and minstret counter */
    csrci CSR_MCOUNTINHIBIT, 0x5

    /*
     * Call vendor defined SystemInit to
     * initialize the micro-controller system.
     */
    call SystemInit

    /* ===== Startup Stage 3 ===== */
    /*
     * Load code section from FLASH to ILM
     * when code LMA is different with VMA
     */
    la a0, _ilm_lma
    la a1, _ilm
    /* If the ILM phy-address same as the logic-address, then quit */
    beq a0, a1, 2f
    la a2, _eilm
    bgeu a1, a2, 2f

1:
    /* Load code section if necessary */
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Load data section */
    la a0, _data_lma
    la a1, _data
    la a2, _edata
    bgeu a1, a2, 2f
1:
    lw t0, (a0)
    sw t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Clear bss section */
    la a0, __bss_start
    la a1, _end
    bgeu a0, a1, 2f
1:
    sw zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:

    /* Call global constructors */
    la a0, __libc_fini_array
    call atexit
    /* Call C/C++ constructor start up code */
    call __libc_init_array

    /* do pre-init steps before main */
    call _premain_init
    /* ===== Call Main Function  ===== */
    /* argc = argv = 0 */
    li a0, 0
    li a1, 0
    call main
    /* do post-main steps after main */
    call _postmain_fini

1:
    j 1b
