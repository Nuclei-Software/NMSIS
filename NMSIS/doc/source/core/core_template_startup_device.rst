.. _core_template_startup_device_asm:

Startup File startup_<device>.S
===============================

The **Startup File startup_<device>.S** contains:
 - The reset handler which is executed after CPU reset and typically calls the :cpp:func:`SystemInit` function.
 - The setup values for the stack pointer SP.
 - Exception vectors of the Nuclei Processor with weak functions that implement default routines.
 - Interrupt vectors that are device specific with weak functions that implement default routines.

The processer level start flow is implemented in the *startup_<device>.S*. Detail description as below picture:

.. _figure_template_startup_1:

.. image:: /asserts/images/NMSIS_startup.png
    :alt: NMSIS-Startup User Files
    :width: 80%
    :align: center

    NMSIS-Startup User Files

Stage1: Interrupt and Exception initialization
  * Disable Interrupt
  * Initialize GP, stack
  * Initialize NMI entry and set default NMI handler
  * Initialize Exception entry and set default exception handler
  * Initialize vector table entry and set default interrupt handler
  * Initialize Interrupt mode as ECLIC mode. (ECLIC mode is proposed. Default mode is CLINT mode)

Stage2: Hardware initialization
  * Enable FPU if necessary
  * Call user defined :cpp:func:`SystemInit` for system clock initialization.

Stage3: Section initialization
  * Copy section, e.g. data section, text section if necessary.
  * Clear Block Started by Symbol (BSS) section
  * Call ``__libc_fini_array`` and ``__libc_init_array`` functions to do C library initialization
  * Call ``_premain_init`` function to do initialization steps before main function
  * Jump Main


The file exists for each supported toolchain and is the only toolchain specific NMSIS file.

To adapt the file to a new device only the interrupt vector table needs to be extended with
the device-specific interrupt handlers.

The naming convention for the interrupt handler names are ``eclic_<interrupt_name>_handler``.

This table needs to be consistent with :cpp:enum:`IRQn_Type` that defines all the IRQ numbers for each interrupt.

The following example shows the extension of the interrupt vector table for the GD32VF103 device family.

.. code-block:: c
    :linenos:

        .section .vtable

        .weak  eclic_msip_handler
        .weak  eclic_mtip_handler
        .weak  eclic_pmaf_handler
        /* Adjusted for GD32VF103 interrupt handlers */
        .weak  eclic_wwdgt_handler
        .weak  eclic_lvd_handler
        .weak  eclic_tamper_handler
            :    :
            :    :
        .weak  eclic_can1_ewmc_handler
        .weak  eclic_usbfs_handler

        .globl vector_base
        .type vector_base, @object
    vector_base:
        /* Run in FlashXIP download mode */
        j _start                                                /* 0: Reserved, Jump to _start when reset for vector table not remapped cases.*/
        .align LOG_REGBYTES                                     /*    Need to align 4 byte for RV32, 8 Byte for RV64 */
        DECLARE_INT_HANDLER     default_intexc_handler          /* 1: Reserved */
        DECLARE_INT_HANDLER     default_intexc_handler          /* 2: Reserved */
        DECLARE_INT_HANDLER     eclic_msip_handler              /* 3: Machine software interrupt */
                        :          :
                        :          :
        /* Adjusted for Vendor Defined External Interrupts */
        DECLARE_INT_HANDLER     eclic_wwdgt_handler             /* 19: Window watchDog timer interrupt */

        DECLARE_INT_HANDLER     eclic_lvd_handler               /* 20: LVD through EXTI line detect interrupt */
        DECLARE_INT_HANDLER     eclic_tamper_handler            /* 21: tamper through EXTI line detect */
                        :          :
                        :          :
        DECLARE_INT_HANDLER     eclic_can1_ewmc_handler         /* 85: CAN1 EWMC interrupt */
        DECLARE_INT_HANDLER     eclic_usbfs_handler             /* 86: USBFS global interrupt */


startup_Device.S Template File
------------------------------

Here provided a riscv-gcc template startup assemble code template file as below.
The files for other compilers can slightly differ from this version.

.. literalinclude:: ../../../../Device/_Template_Vendor/Vendor/Device/Source/GCC/startup_Device.S
    :language: c
    :linenos:
