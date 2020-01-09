.. _core_get_started:

Using NMSIS in Embedded Applications
====================================

.. _core_get_started_intro:

Introduction
------------

To use the **NMSIS-Core**, the following files are added to the embedded
application:

-  :ref:`core_template_startup_device_asm`, which provided asm startup code
   and vector table.

-  :ref:`core_template_intexc_device`, which provided general exception
   handling code for non-vector interrupts and exceptions.

-  :ref:`core_template_linker_script`, which provided linker script for the device.

-  :ref:`core_template_system_device`, which provided general device
   configuration (i.e. for clock and BUS setup).

-  :ref:`core_template_device_header` gives access to processor core and all
   peripherals.

.. Note::

   The files :ref:`core_template_startup_device_asm`, :ref:`core_template_intexc_device`,
   :ref:`core_template_linker_script` and :ref:`core_template_system_device` may require
   application specific adaptations and therefore should be copied into
   the application project folder prior configuration.

   The :ref:`core_template_device_header` is included in all source files that need
   device access and can be stored on a central include folder that is
   generic for all projects.

The :ref:`core_template_startup_device_asm` is executed right after device reset, it will do
necessary stack pointer initialization, exception and interrupt entry configuration, then
call :cpp:func:`SystemInit`, after system initialization, will return to assemble startup code
and do c/c++ runtime initialization which includes data, bss section initialization, c++ runtime
initialization, then it will call :cpp:func:`main` function in the application code.

In the :ref:`core_template_intexc_device`, it will contain all exception and interrupt vectors
and implements a default function for every interrupt.
It may also contain stack and heap configurations for the user application.

The :ref:`core_template_system_device` performs the setup for the processor
clock. The variable :cpp:var:`SystemCoreClock` indicates the CPU clock speed.
:ref:`core_api_systick` describes the minimum feature set. In addition
the file may contain functions for the memory BUS setup and clock re-configuration.

The :ref:`core_template_device_header` is the central include file that the
application programmer is using in the C source code. It provides the
following features:

-  :ref:`core_api_periph_access` provides a standardized register layout for all
   peripherals. Optionally functions for device-specific peripherals may
   be available.

-  :ref:`core_api_interrupt_exception` can be accessed with standardized
   symbols and functions for the **ECLIC** are provided.

-  :ref:`core_api_core_intrinsic` allow to access special instructions,
   for example for activating sleep mode or the NOP instruction.

-  :ref:`nmsis_core_api_intrinsic_functions_for_simd_instructions` provide access to the DSP-oriented instructions.

-  :ref:`core_api_systick` function to configure and start a periodic timer interrupt.

-  :ref:`core_api_csr_access` function to access the core csr registers.

-  :ref:`core_api_cache` to access the I-CACHE and D-CACHE unit

-  :ref:`core_api_fpu` to access the Floating point unit.

-  :ref:`core_api_pmp` to access the Physical Memory Protection unit

-  :ref:`core_api_version_control` which defines NMSIS release specific macros.

-  :ref:`core_api_compiler_control` is compiler agnostic `#define` symbols for generic
   C/C++ source code


.. image:: /asserts/images/NMSIS_CORE_Files_user.png
    :alt: NMSIS-Core User Files
    :scale: 50%


The NMSIS-Core system files are device specific.

In addition, the :ref:`core_template_startup_device_asm` is also compiler vendor specific, currently only GCC version is provided.
The versions provided by NMSIS are only generic templates. The adopted versions for a concrete device are typically provided by the device
vendor through the according device familiy package.

For example, the following files are provided by the **GD32VF103** device family pack:

+-----------------------------------------+---------------------------------------------------------+
|                **File**                 |                 **Description**                         |
+-----------------------------------------+---------------------------------------------------------+
| ./Device/Source/GCC/startup_gd32vf103.S | Startup File startup_<device>.S                         |
|                                         |                                                         |
|                                         | for the GD32VF103 device variants.                      |
+-----------------------------------------+---------------------------------------------------------+
| ./Device/Source/GCC/intexc_gd32vf103.S  | Exception and Interrupt Handling File intexc_<device>.S |
|                                         |                                                         |
|                                         | for the GD32VF103 device variants.                      |
+-----------------------------------------+---------------------------------------------------------+
| ./Device/Source/GCC/gcc_gd32vf103.ld    | Linker script File gcc_<device>.ld                      |
|                                         |                                                         |
|                                         | for the GD32VF103 device variants.                      |
+-----------------------------------------+---------------------------------------------------------+
| ./Device/Source/system_gd32vf103.c      | System Configuration File system_<device>.c             |
|                                         |                                                         |
|                                         | for the GD32VF103 device families                       |
+-----------------------------------------+---------------------------------------------------------+
| ./Device/Include/system_gd32vf103.h     | System Configuration File system_<device>.h             |
|                                         |                                                         |
|                                         | for the GD32VF103 device families                       |
+-----------------------------------------+---------------------------------------------------------+
| ./Device/Include/gd32vf103.h            | Device Header File <device.h>                           |
|                                         |                                                         |
|                                         | for the GD32VF103 device families.                      |
+-----------------------------------------+---------------------------------------------------------+

.. Note::
  The silicon vendors create these device-specific NMSIS-Core files based on :ref:`core_device_templates` provided by Nuclei.

Thereafter, the functions described under :ref:`nmsis_core_api` can be used in the application.

.. _basic_nmsis_example:

Basic NMSIS Example
-------------------

A typical example for using the NMSIS layer is provided below. The example is based on a GD32VF103 Device.

.. code-block:: c
    :linenos:
    :caption: gd32vf103_example.c

    #include <gd32vf103.h>                           // File name depends on device used

    uint32_t volatile msTicks;                       // Counter for millisecond Interval
    #define SysTick_Handler     eclic_mtip_handler
    #define CONFIG_TICKS        (SOC_TIMER_FREQ / 1000)

    void SysTick_Handler (void) {                    // SysTick Interrupt Handler
      SysTick_Reload(CONFIG_TICKS);
      msTicks++;                                     // Increment Counter
    }

    void WaitForTick (void)  {
      uint32_t curTicks;

      curTicks = msTicks;                            // Save Current SysTick Value
      while (msTicks == curTicks)  {                 // Wait for next SysTick Interrupt
        __WFI ();                                    // Power-Down until next Event/Interrupt
      }
    }

    void TIMER0_UP_IRQHandler (void) {               // Timer Interrupt Handler
      ;                                              // Add user code here
    }

    void timer0_init(int frequency) {                // Set up Timer (device specific)
      ECLIC_SetPriorityIRQ (TIMER0_UP_IRQn, 1);      // Set Timer priority
      ECLIC_EnableIRQ (TIMER0_UP_IRQn);              // Enable Timer Interrupt
    }


    void Device_Initialization (void)  {             // Configure & Initialize MCU
      if (SysTick_Config (CONFIG_TICKS)) {
           ; // Handle Error
      }
      timer0_init ();                                // setup device-specific timer
    }

    // The processor clock is initialized by NMSIS startup + system file
    void main (void) {                               // user application starts here
      Device_Initialization ();                      // Configure & Initialize MCU
      while (1)  {                                   // Endless Loop (the Super-Loop)
        __disable_irq ();                            // Disable all interrupts
        Get_InputValues ();                          // Read Values
        __enable_irq ();                             // Enable all interrupts
        Calculation_Response ();                     // Calculate Results
        Output_Response ();                          // Output Results
        WaitForTick ();                              // Synchronize to SysTick Timer
      }
    }


.. _using_interrupt_and_exception:

Using Interrupt and Exception/NMI
---------------------------------

Nuclei processors provide **NMI(Non-Maskable Interrupt)**, **Exception**,
**Vector Interrupt** and **Non-Vector Interrupt** features.

.. _using_nmsis_with_general_nuclei_processors:

Using NMSIS with generic Nuclei Processors
------------------------------------------

Nuclei provides NMSIS-Core files for the supported Nuclei Processors and for various compiler vendors.
These files can be used when standard Nuclei processors should be used in a project.
The table below lists the folder and device names of the Nuclei processors.

+------------------------------+-----------+--------+--------------------------------------------------------------------------------------------------------+
| Folder                       | Processor | RISC-V | Description                                                                                            |
+------------------------------+-----------+--------+--------------------------------------------------------------------------------------------------------+
| ./Device/Nuclei/NUCLEI_N     | * N200    | RV32   | Contains Include and Source template files configured for the Nuclei N200/N300/N600 processor.         |
|                              | * N300    |        |                                                                                                        |
|                              | * N600    |        | The device name is NUCLEI_N and the name of the Device Header File <device.h> is <NUCLEI_N.h>.         |
+------------------------------+-----------+--------+--------------------------------------------------------------------------------------------------------+
| ./Device/Nuclei/NUCLEI_NX    | NX600     | RV64   | Contains Include and Source template files configured for the Nuclei NX600 processor.                  |
|                              |           |        |                                                                                                        |
|                              |           |        | The device name is NUCLEI_NX and the name of the Device Header File <device.h> is <NUCLEI_NX.h>.       |
+------------------------------+-----------+--------+--------------------------------------------------------------------------------------------------------+

.. _create_generic_libraries_with_nmsis:

Create generic Libraries with NMSIS
-----------------------------------

The NMSIS Processor and Core Peripheral files allow also to create generic libraries.
The **NMSIS-DSP** Libraries are an example for such a generic library.

To build a generic library set the define **__NMSIS_GENERIC** and include the 
*nmsis_core.h* NMSIS CPU & Core Access header file for the processor.

The define **__NMSIS_GENERIC** disables device-dependent features such as the **SysTick timer**
and the **Interrupt System**.

Example
^^^^^^^

The following code section shows the usage of the *nmsis_core.h* header files to build a generic
library for |nmsis_support_cores|.

One of these defines needs to be provided on the compiler command line.

By using this header file, the source code can access the functions for :ref:`core_api_csr_access`,
:ref:`core_api_core_intrinsic` and :ref:`nmsis_core_api_intrinsic_functions_for_simd_instructions`.


.. code-block:: c
    :linenos:
    :caption: core_generic.h

    #define __NMSIS_GENERIC   // Disable Eclic and Systick functions
    #include <nmsis_core.h>
