.. _changelog:

Changelog
=========

V1.0.4-dev
----------

This is the version ``V1.0.4-dev`` release of Nuclei MCU Software Interface Standard(NMSIS), which is still in development.

* **NMSIS-Core**

  - add ``__CCM_PRESENT`` macro in NMSIS-Core, if CCM hardware unit is present in your CPU,
    ``__CCM_PRESENT`` macro need to be set to 1 in ``<Device>.h``
  - Fixed mtvec related api comment in ``core_feature_eclic.h``
  - Add safely write mtime/mtimecmp register for 32bit risc-v processor
  - rearrage #include header files for all NMSIS Core header files
  - removed some not good #pragma gcc diagnostic lines in ``nmsis_gcc.h``


V1.0.3
------

This is the official release version ``V1.0.3`` release of Nuclei MCU Software Interface Standard(NMSIS).

This release is only supported by Nuclei GNU Toolchain 2021.12 and its later version,
since it required intrinsic header files in RISC-V GCC for B/P/V extensions.

The following changes has been made since ``V1.0.2``.

* **Documentation**

  - Update NMSIS Core/DSP/NN related documentation

* **Device Templates**

  - Add ``__BITMANIP_PRESENT`` and ``__VECTOR_PRESENT`` in ``<Device>.h``
  - Add more REG/ADDR/BIT access macros in ``<Device>.h``
  - Update linker script for ``<Device>.ld`` for Nuclei C Runtime Library
  - Add tp register initialization and add early exception setup during startup in ``startup_<Device>.S``
  - Adding support for Nuclei C Runtime library

* **NMSIS-Core**

  - Update ``core_feature_eclic.h``, ``core_feature_timer.h`` and ``core_feature_dsp.h``
  - Added ``core_feature_vector.h`` and ``core_feature_bitmainp.h``
  - Add more nuclei customized csr in ``riscv_encoding.h``

* **NMSIS-DSP/NN**

   - Add support for Nuclei GNU Toolchain 2021.12
   - Add new build system to generate NMSIS DSP and NN library
   - Update cmake files for both DSP and NN library
   - No need to define ``__RISCV_FEATURE_DSP`` and ``__RISCV_FEATURE_VECTOR`` when using DSP or NN library,
     it will be defined in ``riscv_math_types.h`` via the predefined macros in Nuclei RISC-V gcc 10.2
   - Rename ``RISCV_VECTOR`` to ``RISCV_MATH_VECTOR``
   - Fix FLEN and XLEN mis-usage in library

V1.0.2
------

This is the official release version ``V1.0.2`` release of Nuclei MCU Software Interface Standard(NMSIS).

The following changes has been made since ``V1.0.1``.

* **Documentation**

  - Update NMSIS Core/DSP/NN related documentation

* **Device Templates**

   - DOWNLOAD_MODE_xxx macros are removed from riscv_encoding.h, it is now defined as enum in ``<Device.h>``, and
     can be customized by soc vendor.
   - startup code now don't rely on DOWNLOAD_MODE macro, instead it now rely on a new macro called VECTOR_TABLE_REMAPPED,
     when VECTOR_TABLE_REMAPPED is defined, it means the vector table's lma != vma, such as vector table need to be
     copied from flash to ilm when boot up
   - Add more customized csr of Nuclei RISC-V Core
   - Add **BIT**, **BITS**, **REG**, **ADDR** related macros in ``<Device.h>``

* **NMSIS-Core**

   - Nuclei Cache CCM operation APIs are now introduced in core_feature_cache.h
   - Update NMSIS-Core header files

* **NMSIS-DSP/NN**

   - Merged the official CMSIS 5.8.0 release, CMSIS-DSP 1.9.0, CMSIS-NN 3.0.0
   - RISC-V Vector extension and P-extension support for DSP/NN libraries are added

V1.0.2-RC2
----------

This is the release candidate version ``V1.0.2-RC2`` release of Nuclei MCU Software Interface Standard(NMSIS).

The following changes has been made since ``V1.0.2-RC1``.

* **Documentation**

  - Update NMSIS Core/DSP/NN related documentation

V1.0.2-RC1
----------

This is the release candidate version ``V1.0.2-RC1`` release of Nuclei MCU Software Interface Standard(NMSIS).

The following changes has been made since ``V1.0.1``.

* **Device Templates**

   - DOWNLOAD_MODE_xxx macros are removed from riscv_encoding.h, it is now defined as enum in ``<Device.h>``, and
     can be customized by soc vendor.
   - startup code now don't rely on DOWNLOAD_MODE macro, instead it now rely on a new macro called VECTOR_TABLE_REMAPPED,
     when VECTOR_TABLE_REMAPPED is defined, it means the vector table's lma != vma, such as vector table need to be
     copied from flash to ilm when boot up
   - Add **BIT**, **BITS**, **REG**, **ADDR** related macros in ``<Device.h>``

* **NMSIS-Core**

   - Nuclei Cache CCM operation APIs are now introduced in core_feature_cache.h

* **NMSIS-DSP/NN**

   - Merged the official CMSIS 5.8.0 release, CMSIS-DSP 1.9.0, CMSIS-NN 3.0.0
   - RISC-V Vector extension and P-extension support for DSP/NN libraries are added


V1.0.1
------

This is the official ``V1.0.1`` release of Nuclei MCU Software Interface Standard(NMSIS).

The following changes has been maded since ``V1.0.1-RC1``.

* **Device Templates**

  - I/D Cache enable assemble code in startup_<Device>.S are removed now
  - Cache control updates in System_<Device>.c

    - I-Cache will be enabled if __ICACHE_PRESENT = 1 defined in <Device.h>
    - D-Cache will be enabled if __DCACHE_PRESENT = 1 defined in <Device.h>


V1.0.1-RC1
----------

This is release candidate version ``V1.0.1-RC1`` of NMSIS.


* **NMSIS-Core**

  - Add RISC-V DSP 64bit intrinsic functions in ``core_feature_dsp.h``
  - Add more CSR definitions in ``riscv_encoding.h``
  - Update arm compatiable functions for RISC-V dsp instruction cases in ``core_compatiable.h``

* **NMSIS-DSP**

  - Optimize RISC-V 32bit DSP library implementation
  - Add support for Nuclei RISC-V 64bit DSP SIMD instruction for DSP library
  - Add test cases used for DSP library testing, mainly for internal usage
  - Change the examples and tests to use Nuclei SDK as running environment

* **NMSIS-NN**

  - Add support for Nuclei RISC-V 64bit DSP SIMD instruction for NN library
  - Change the examples and tests to use Nuclei SDK as running environment

* **Device Templates**

  - Add ``DDR DOWNLOAD_MODE`` in device templates
  - Modifications to ``startup_<Device>.S`` files

    - ``_premain_init`` is added to replace ``_init``
    - ``_postmain_fini`` is added to replace ``_fini``

  - If you have implemented your init or de-init functions through ``_init`` or ``_fini``,
    please use ``_premain_init`` and ``_postmain_fini`` functions defined ``system_<Device>.c`` now

V1.0.0-beta1
------------

Main changes in release **V1.0.0-beta1**.

- **NMSIS-Core**

  - Fix ``SysTick_Reload`` implementation
  - Update ``ECLIC_Register_IRQ`` implementation to allow handler == NULL
  - Fix ``MTH`` offset from 0x8 to 0xB, this will affect function of ``ECLIC_GetMth`` and ``ECLIC_SetMth``
  - Fix wrong macro check in cache function
  - Add missing ``SOC_INT_MAX`` enum definition in Device template
  - In ``System_<Device>.c``, ECLIC NLBits set to ``__ECLIC_INTCTLBITS``,
    which means all the bits are for level, no bits for priority


V1.0.0-beta
-----------

Main changes in release **V1.0.0-beta**.

- **NMSIS-Core**

  - Fix error typedef of ``CSR_MCAUSE_Type``
  - Change ``CSR_MCACHE_CTL_DE`` to future value ``0x00010000``
  - Fix names in CSR naming, ``CSR_SCRATCHCSW`` -> ``CSR_MSCRATCHCSW``,
    and ``CSR_SCRATCHCSWL`` -> ``CSR_MSCRATCHCSWL``
  - Add macros in ``riscv_encoding.h``: ``MSTATUS_FS_INITIAL``, ``MSTATUS_FS_CLEAN``, ``MSTATUS_FS_DIRTY``

- **Documentation**

  - Fix an typo in *core_template_intexc.rst*
  - Add cross references of Nuclei ISA Spec
  - Update appendix
  - Refines tables and figures


V1.0.0-alpha.1
--------------

API changes has been maded to system timer.

- Start from Nuclei N core version 1.4, MSTOP register is renamed to MTIMECTL to provide more features

- Changes made to NMSIS/Core/core_feature_timer.h

  - MSTOP register name changed to MTIMECTL due to core spec changes
  - SysTimer_SetMstopValue renamed to SysTimer_SetControlValue
  - SysTimer_GetMstopValue renamed to SysTimer_GetControlValue
  - Add SysTimer_Start and SysTimer_Stop to start or stop system timer counter
  - SysTick_Reload function is introduced to reload system timer
  - Macro names started with SysTimer_xxx are changed, please check in the code.

- Removed unused lines of code in DSP and NN library source code which has unused macros which will not work for RISCV cores.

- Fix some documentation issues, mainly typos and invalid cross references.


V1.0.0-alpha
------------

This is the ``V1.0.0-alpha`` release of Nuclei MCU Software Interface Standard(NMSIS).

In this release, we have release three main compoments:

* **NMSIS-Core**: Standardized API for the Nuclei processor core and peripherals.

* **NMSIS-DSP**: DSP library collection optimized for the Nuclei Processors which has RISC-V SIMD instruction set.

* **NMSIS-NN**: Efficient neural network
  library developed to maximize the performance and minimize the memory footprint Nuclei Processors which has RISC-V SIMD instruction set.

We also released totally new `Nuclei-SDK`_ which is an SDK implementation based on the **NMSIS-Core** for Nuclei N/NX evaluation cores running on HummingBird Evaluation Kit.


.. _Nuclei-SDK: https://github.com/Nuclei-Software/nuclei-sdk
