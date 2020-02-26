.. _core_overview:

Overview
========

.. _core_introduction:

Introduction
------------

**NMSIS-Core** implements the basic run-time system for a |nuclei_core| 
based device and gives the user access to the processor core and
the device peripherals. In detail it defines:

-  **Hardware Abstraction Layer (HAL)** for Nuclei processor registers
   with standardized definitions for the **CSR Registers**, **TIMER**,
   **ECLIC**, **PMP Registers**, **DSP Registers**, **FPU registers**,
   and **Core Access Functions**.

-  **Standard core exception/interrupt names** to interface to system
   exceptions or interrupts without having compatibility issues.

-  **Methods to organize header files** that makes it easy to learn new
   Nuclei micro-controller products and improve software portability.
   This includes naming conventions for device-specific interrupts.

-  **Methods for system initialization** to be used by each Device vendor.
   For example, the standardized :cpp:func:`SystemInit` function is essential for
   configuring the clock system of the device.

-  **Intrinsic functions** used to generate CPU instructions that are
   not supported by standard C functions.

-  A variable :cpp:var:`SystemCoreClock` to determine the **system clock frequency**
   which simplifies the setup the timer.

The following sections provide details about the **NMSIS-Core**:

-  :ref:`core_get_started` describes the project
   setup and shows a simple program example

-  :ref:`core_device_templates` describes the files of the
   :ref:`nmsis_core` in detail and explains how to adapt template files
   provided by Nuclei to silicon vendor devices.

-  :ref:`nmsis_core_api` describe the features and functions of the
   :ref:`core_template_device_header` in detail.

-  :ref:`core_api_register_type` describe the data structures of the
   :ref:`core_template_device_header` in detail.

.. _core_processor_support:

Processor Support
-----------------

**NMSIS** have provided support for all the |nuclei_core|.

**Nuclei ISA Spec:**

- `Nuclei Process Core Instruction Set Architecture Spec`_

**Nuclei N Class Processor Reference Manuals:**

- `N200 series`_

- `N300 series`_

- `N600 series`_

**Nuclei NX Class Processor Reference Manuals:**

- `NX600 series`_


.. _core_toolchain_support:

Toolchain Support
-----------------

The :ref:`core_device_templates` provided by Nuclei have been tested and
verified using these toolchains:

- GNU Toolchain for RISC-V modified by Nuclei


.. _N100 series: https://www.nucleisys.com/product.php?site=n100
.. _N200 series: https://www.nucleisys.com/product.php?site=n200
.. _N300 series: https://www.nucleisys.com/product.php?site=n300
.. _N600 series: https://www.nucleisys.com/product.php?site=n600
.. _NX600 series: https://www.nucleisys.com/product.php?site=nx600
.. _Nuclei Process Core Instruction Set Architecture Spec: https://doc.nucleisys.com/nuclei_spec
