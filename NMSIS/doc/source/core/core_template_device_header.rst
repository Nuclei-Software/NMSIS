.. _core_template_device_header:

Device Header File <device.h>
=============================

The :ref:`core_template_device_header` contains the following sections that are device specific:
  - :ref:`core_interrupt_num_def` provides interrupt numbers (IRQn) for all exceptions and interrupts of the device.
  - :ref:`nmsis_core_configuration` reflect the features of the device.
  - :ref:`core_device_periph_access` provides definitions for the :ref:`core_api_periph_access` to all device peripherals. It contains all data structures and the address mapping for device-specific peripherals.
  - **Access Functions for Peripherals (optional)** provide additional helper functions for peripherals that are useful for programming of these peripherals. Access Functions may be provided as inline functions or can be extern references to a device-specific library provided by the silicon vendor.

:ref:`nmsis_core_api` describes the standard features and functions of the :ref:`core_template_device_header` in detail.

.. _core_interrupt_num_def:

Interrupt Number Definition
---------------------------

:ref:`core_template_device_header` contains the enumeration :cpp:any:`IRQn_Type` that defines all exceptions and interrupts of the device.
  - Negative IRQn values represent processor core exceptions (internal interrupts).
  - Positive IRQn values represent device-specific exceptions (external interrupts). The first device-specific interrupt has the IRQn value 0.
    The IRQn values needs extension to reflect the device-specific interrupt vector table in the :ref:`core_template_startup_device_asm`.


The following example shows the extension of the interrupt vector table for the GD32VF103 device family.

.. code-block:: c
    :linenos:

    typedef enum IRQn {
        /******  N200 Processor Exceptions Numbers *********************************************/
        Reserved0_IRQn               =   0,     /*!<  Internal reserved                        */
        Reserved1_IRQn               =   1,     /*!<  Internal reserved                        */
        Reserved2_IRQn               =   2,     /*!<  Internal reserved                        */
        SysTimerSW_IRQn              =   3,     /*!<  System Timer SW interrupt                */
        Reserved3_IRQn               =   4,     /*!<  Internal reserved                        */
        Reserved4_IRQn               =   5,     /*!<  Internal reserved                        */
        Reserved5_IRQn               =   6,     /*!<  Internal reserved                        */
        SysTimer_IRQn                =   7,     /*!<  System Timer Interrupt                   */
        Reserved6_IRQn               =   8,     /*!<  Internal reserved                        */
        Reserved7_IRQn               =   9,     /*!<  Internal reserved                        */
        Reserved8_IRQn               =  10,     /*!<  Internal reserved                        */
        Reserved9_IRQn               =  11,     /*!<  Internal reserved                        */
        Reserved10_IRQn              =  12,     /*!<  Internal reserved                        */
        Reserved11_IRQn              =  13,     /*!<  Internal reserved                        */
        Reserved12_IRQn              =  14,     /*!<  Internal reserved                        */
        Reserved13_IRQn              =  15,     /*!<  Internal reserved                        */
        Reserved14_IRQn              =  16,     /*!<  Internal reserved                        */
        HardFault_IRQn               =  17,     /*!<  Hard Fault, storage access error         */
        Reserved15_IRQn              =  18,     /*!<  Internal reserved                        */
    
        /******  GD32VF103 Specific Interrupt Numbers ******************************************/
        WWDGT_IRQn                   =  19,     /*!< window watchDog timer interrupt           */
        LVD_IRQn                     =  20,     /*!< LVD through EXTI line detect interrupt    */
        TAMPER_IRQn                  =  21,     /*!< tamper through EXTI line detect           */
                     :       :
                     :       :
        CAN1_EWMC_IRQn               =  85,     /*!< CAN1 EWMC interrupt                       */
        USBFS_IRQn                   =  86,     /*!< USBFS global interrupt                    */
    } IRQn_Type;


.. _nmsis_core_configuration:

Configuration of the Processor and Core Peripherals
---------------------------------------------------

The :ref:`core_template_device_header` configures the |nuclei_core| and the core peripherals with
``#define`` that are set prior to including the file *nmsis_core.h*.

The following tables list the ``#define`` along with the possible values for |nmsis_support_cores|.
If these ``#define`` are missing default values are used.

nmsis_core.h
^^^^^^^^^^^^

+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| #define                 | Value Range | Default    | Description                                                                                                   |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __NUCLEI_N_REV  OR      | 0x0100 |    | 0x0104     | * For Nuclei N class device, define __NUCLEI_N_REV, for NX class device, define __NUCLEI_NX_REV.              |
| __NUCLEI_NX_REV         | 0x0104      |            | * Core revision number ([15:8] revision number, [7:0] patch number), 0x0100 -> 1.0, 0x0104 -> 1.4             |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __SYSTIMER_PRESENT      | 0 .. 1      | 1          | Define whether Priviate System Timer is present or not. This SysTimer is a Memory Mapped Unit.                |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __SYSTIMER_BASEADDR     | -           | 0x02000000 | Base address of the System Timer Unit.                                                                        |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __ECLIC_PRESENT         | 0 .. 1      | 1          | Define whether Enhanced Core Local Interrupt Controller (ECLIC) Unit is present or not                        |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __ECLIC_BASEADDR        | -           | 0x0C000000 | Base address of the ECLIC unit.                                                                               |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __ECLIC_INTCTLBITS      | 1 .. 8      | 1          | Define the number of hardware bits are actually implemented in the clicintctl registers.                      |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __ECLIC_INTNUM          | 1 .. 1024   | 1          | Define the total interrupt number(including the internal core interrupts) of ECLIC Unit                       |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __PMP_PRESENT           | 0 .. 1      | 0          | Define whether Physical Memory Protection (PMP) Unit is present or not.                                       |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __PMP_ENTRY_NUM         | 8 or 16     | 8          | Define the numbers of PMP entries.                                                                            |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __FPU_PRESENT           | 0 .. 2      | 0          | Define whether Floating Point Unit (FPU) is present or not.                                                   |
|                         |             |            |                                                                                                               |
|                         |             |            | * 0: Not present                                                                                              |
|                         |             |            | * 1: Single precision FPU present                                                                             |
|                         |             |            | * 2: Double precision FPU present                                                                             |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __DSP_PRESENT           | 0 .. 1      | 0          | Define whether Digital Signal Processing Unit (DSP) is present or not.                                        |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __ICACHE_PRESENT        | 0 .. 1      | 0          | Define whether I-Cache Unit is present or not.                                                                |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __DCACHE_PRESENT        | 0 .. 1      | 0          | Define whether D-Cache Unit is present or not.                                                                |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+
| __Vendor_SysTickConfig  | 0 .. 1      | 0          | If __SYSTIMER_PRESENT is 1, then the __Vendor_SysTickConfig  can be set to 0, otherwise it can only set to 1. |
|                         |             |            |                                                                                                               |
|                         |             |            | If this define is set to 1, then the default SysTick_Config function is excluded.                             |
|                         |             |            |                                                                                                               |
|                         |             |            | In this case, the file Device.h must contain a vendor specific implementation of this function.               |
+-------------------------+-------------+------------+---------------------------------------------------------------------------------------------------------------+

NMSIS Version and Processor Information
---------------------------------------

The following shows the defines in the *nmsis_core.h* file that may be used in the
:ref:`core_device_templates` to verify a minimum version or ensure that the right Nuclei N/NX class is used.

.. _core_device_periph_access:

Device Peripheral Access Layer
------------------------------
The :ref:`core_template_device_header` contains for each peripheral:
  - Register Layout Typedef
  - Base Address
  - Access Definitions

The section :ref:`core_api_periph_access` shows examples for peripheral definitions.


Device.h Template File
----------------------

Here we provided ``Device.h`` template file as below:

.. literalinclude:: ../../../../Device/_Template_Vendor/Vendor/Device/Include/Device.h
    :language: text
    :linenos:
