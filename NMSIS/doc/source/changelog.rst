.. _changelog:

Changelog
=========

V1.0.0-xxx
----------

Release tag is not yet decided.

- **NMSIS-Core**

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
