.. _nmsis_introduction:

Nuclei MCU Software Interface Standard(NMSIS)
=============================================

.. _about_nmsis:
About NMSIS
-----------

The **NMSIS** is a vendor-independent hardware abstraction layer for
micro-controllers that are based on Nuclei processors.

The **NMSIS** defines generic tool interfaces and enables consistent
device support. It provides simple software interfaces to the processor
and the peripherals, simplifying software re-use, reducing the learning
curve for micro-controller developers, and reducing the time to market
for new devices.

.. _nmsis_compoments:

NMSIS Components
----------------

NMSIS CORE
  All |nuclei_core| Standardized API for the Nuclei processor core and peripherals.

NMSIS DSP
  All |nuclei_core| DSP library collection with a lot of functions for various data
  types: fixed-point (fractional q7, q15, q31) and single precision floating-point (32-bit). 
  Implementations optimized for the Nuclei Processors which has RISC-V SIMD instruction set.

NMSIS NN
  All |nuclei_core| Collection of efficient neural network 
  kernels developed to maximize the performance and minimize the memory 
  footprint Nuclei processor cores.

.. _nmsis_design:

NMSIS Design
------------

**NMSIS** is designed to help the |nuclei_core| processors in
standardization. It enables consistent software layers and device
support across a wide range of development tools and micro-controllers.

**NMSIS** is a lightweight software interface layer that tried to
standardize common Nuclei processor-based SOC, and it didn't define any
standard peripherals. The silicon industry can therefore support the
wide variations of Nuclei processor-based devices with this common
standard.

.. image:: /asserts/images/NMSIS_Overview.png
    :alt: NMSIS Design Diagram
    :scale: 80%


We can achieve the following benefits of **NMSIS**:

-  **NMSIS** reduces the learning curve, development costs, and
   time-to-market. Developers can write software quicker through a
   variety of easy-to-use, standardized software interfaces.

-  Consistent software interfaces improve the software portability and
   re-usability. Generic software libraries and interfaces provide
   consistent software framework.

-  It provides interfaces for debug connectivity, debug peripheral
   views, software delivery, and device support to reduce time-to-market
   for new micro-controller deployment.

-  Being a compiler independent layer, it allows to use the compiler of
   your choice. Thus, it is supported by mainstream compilers.

-  It enhances program debugging with peripheral information for
   debuggers.

.. _nmsis_how_to_access:

How to Access
-------------

If you want to access the code of **NMSIS**, you can visit our opensource
`NMSIS Github Repository`_.

.. _nmsis_coding_rules:

Coding Rules
------------

The **NMSIS** uses the following essential coding rules and conventions:

-  Compliant with ANSI C (C99) and C++ (C++03).

-  Uses ANSI C standard data types defined in **stdint.h**.

-  Variables and parameters have a complete data type.

-  Expressions for *#define* constants are enclosed in parenthesis.

In addition, the **NMSIS** recommends the following conventions for
identifiers:

-  **CAPITAL** names to identify Core Registers, Peripheral Registers,
   and CPU Instructions.

-  **CamelCase** names to identify function names and interrupt
   functions.

-  **Namespace\_** prefixes avoid clashes with user identifiers and
   provide functional groups (i.e. for peripherals, RTOS, or DSP
   Library).

The **NMSIS** is documented within the source files with:

-  Comments that use the C or C++ style.

-  Doxygen compliant comments, which provide:
   -  brief function, variable, macro overview.
   -  detailed description of the function, variable, macro.
   -  detailed parameter explanation.
   -  detailed information about return values.

.. _nmsis_validation:

Validation
----------

Nuclei uses RISC-V GCC Compiler in the various tests of **NMSIS**, and
if more compiler is added, it could be easily supported by following the
**NMSIS** compiler independent layer. For each component, the section
**Validation** describes the scope of the various verifications.

**NMSIS** components are compatible with a range of C and C++ language
standards. 

As **NMSIS** defines API interfaces and functions that scale to a wide
range of processors and devices, the scope of the run-time test coverage
is limited. However, several components are validated using dedicated
test suites.

.. _nmsis_licence:

License
-------

This **NMSIS** is modified based on open-source project **CMSIS** to match Nuclei requirements.

This **NMSIS** is provided free of charge by Nuclei under the `Apache 2.0 License`_.


.. _RISC-V ELF psABI specification: https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md
.. _Apache 2.0 License: http://www.apache.org/licenses/LICENSE-2.0
.. _NMSIS Github Repository: https://github.com/Nuclei-Software/NMSIS
