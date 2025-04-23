.. _changelog:

Changelog
=========

V1.4.0
------

This is the version of ``V1.4.0`` which is still in development.

* **NMSIS-Core**

  - Fix wrong macro ``PLIC_GetThreshold`` & ``PLIC_GetThreshold_S`` implementation for ``core_feature_plic.h``
  - Add ``MTIME_SRW_CTRL`` bitfields in **SysTimer_Type** structure for ``core_feature_timer.h``

* **NMSIS-NN**

  - Sync with CMSIS-NN v7.0.0
  - Add new testcases for NN library testing
  - Add script to generate ``reflib`` by renaming symbols
  - Fix data overflow when ``rhs_offset=128`` in ``riscv_nn_vec_mat_mult_t_s8``
  - Add new test framework for NN library testing, see ``NMSIS/NN/Tests/UnitTest/``

* **NMSIS-DSP**

  - Fix undefined ``blkCnt`` when ``LOOPUNROLL=OFF``

V1.3.1
------


This is the version of ``V1.3.1`` release.


* **NMSIS-Core**

  - Fix Cache CCM related API compile fail using c++ compiler
  - **mfp16mode** csr is renamed to **mmisc_ctl1** due to hw changes

* **NMSIS-NN**

  - Update get started guide for build NMSIS-NN library
  - Fix testcases in NN/Tests/Cases/full: ``riscv_elementwise_mul_acc_s16`` and ``riscv_elementwise_mul_s16_batch_offset``
  - Sync cmake file changes
  - Update IAR prebuilt NN library and example projects

* **NMSIS-DSP**

  - Optimize fredsum, use less fredsum and more fmacc for rvv implementation
  - Sync cmake file changes
  - Update IAR prebuilt DSP library and example projects


V1.3.0
------

This is the version of ``V1.3.0`` release.

* **NMSIS-Core**

  - Add more ECC related macros for ``milm_ctl/mdlm_ctl/mcache_ctl`` csr
  - Add plic related API support in ``core_feature_plic.h``
  - Fix ``core_feature_cidu.h`` which wrongly included ``math.h`` which is not required

* **Build System**

  - Change ``TOOLCHAIN`` to ``LIBTYPE`` in ``Makefile.build`` and ``Makefile.nmsis`` located in ``NMSIS/Scripts/Build/``
* **NMSIS-DSP**

  - Sync with CMSIS-DSP Library v1.16.2
  - Add initial version of NMSIS DSP Benchmark
  - Fix bug found for ComplexMathFunctions when RISCV_MATH_LOOPUNROLL not defined

* **NMSIS-NN**

  - Sync with CMSIS-NN library v6.0.0
  - Add initial version of NMSIS NN benchmark
  - Roll back p-ext optimized version for function ``riscv_nn_vec_mat_mult_t_svdf_s8``

* **Nuclei AI Library**

  - Introduced RVV optimized AI operators implementation which can be found in https://github.com/Nuclei-Software/nuclei-ai-library

V1.2.1
------

This is the version ``V1.2.1`` release.

* **NMSIS-Core**

  - Fix and update many CSR type define structure defined in ``core_feature_base.h``
  - Fix protection type error in PMP/sPMP ``__set_PMPENTRYx/__set_sPMPENTRYx`` API
  - Add SMPU support in ``core_feature_spmp.h``
  - Fix wrong CLIC macros defined in ``core_feature_eclic.h``
  - Update RISC-V DSP intrinsic API and its comments

* **NMSIS-DSP**

  - Sync with CMSIS-DSP library(CMSIS-DSP v1.15.0, commit id:8f8bb8d5)
  - Add a few Nuclei DSP N2 intrinsic APIs in ``core_feature_dsp.h``, they are: ``__RV_DPACK32``, ``__RV_DSUNPKD810``, ``__RV_DSUNPKD820``,
    ``__RV_DSUNPKD830``, ``__RV_DSUNPKD831``, ``__RV_DSUNPKD832``, ``__RV_DZUNPKD810``, ``__RV_DZUNPKD820``, ``__RV_DZUNPKD830``,
    ``__RV_DZUNPKD831``, ``__RV_DZUNPKD832``. And change ``__RV_DPKBB32`` to ``__RV_DPACK32`` for better performance
  - Unified instruction naming style in ``core_feature_dsp.h``, such as: change ``DSMMULU`` to ``DSMMUL.u``, change ``DKWMMULU`` to ``DKWMMUL.u``,
    change ``DKMMACU`` to ``DKMMAC.u``, change ``DKMMSBU`` to ``DKMMSB.u``, change ``DSMAQASU`` to ``DSMAQA.SU``, change ``DDSMAQASU`` to ``DDSMAQA.SU``
  - Optimize the rvv implementation of some MatMult and Complex MatMult functions
  - Fixed some doc erros in ``core_feature_dsp.h``
  - Fixed ``riscv_lms_q31.c`` rvv bug

* **NMSIS-NN**

  - Sync with CMSIS-NN library(CMSIS-NN v5.0.0, commit id:bfc54edb)
  - Optimize some functions with Nuclei DSP N3 instruction(such as: riscv_convolve_s8, riscv_fully_connected_s8, etc.)
  - Optimize the rvv implementation of fullyconnect s8 function
  - Fixed ``riscv_nn_softmax_common_s8.c`` rvv bug

* **CI**

  - Nuclei SDK use 0.6.0 release version, SOC still use evalsoc.

* Documentation

  - Update sphinx and doxygen document version to 1.2.1


V1.2.0
------

This is the version ``V1.2.0`` release.

.. note::

    - This 1.2.0 version will no longer support old gcc 10 verison, and it now only support Nuclei Toolchain 2023.10
      (gcc13 and clang17) or later. The major changes that can be felt are as follows:

    - The prefix of toolchain has changed from ``riscv-nuclei-elf-`` to ``riscv64-unknown-elf-``
    - The ``-march`` option has changed a lot, see https://github.com/riscv-non-isa/riscv-toolchain-conventions/pull/26, for examples:

      - ``b`` extension changed to ``_zba_zbb_zbc_zbs`` extension,
      - ``p`` extension changed to ``_xxldsp`` , ``_xxldspn1x`` , ``_xxldspn2x`` , ``_xxldspn3x`` extensions which means
        stardard DSP extension, Nuclei N1, N2, N3 DSP extensions
      - ``v`` extension changed to ``v``, ``_zve32f``, ``_zve64f`` extensions

      These extensions also can be combined in a certain order, please be cautious

    - The name of Libraries has changed with ``-march``, for examples, the library named ``libnmsis_dsp_rv32imacb.a`` is now named
      ``libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs.a`` since ``b`` extension changed to ``_zba_zbb_zbc_zbs``

  - NMSIS v1.2.0 should be used with Nuclei SDK v0.5.0 or later
  - RVV intrinsic APIs is update to v0.12.0, please visit `rvv-intrinsic-doc <https://github.com/riscv-non-isa/rvv-intrinsic-doc/releases/tag/v0.12.0>`_


* **NMSIS-Core**

  - Add more Nuclei DSP N1/N2/N3 intrinsic APIs and fix some intrinsic API definition and descriptions in ``core_feature_dsp.h``
  - Add basic IAR support for NMSIS Core header files and device template, for sample usage, see Nuclei SDK 0.5.0 release
  - Fix missing break in ``__set_hpm_event`` function API in core_feature_base.h, which affected the nmsis_bench.h
  - Use IAR custom instruction and IAR P-ext 0.5.0 support to support Nuclei DSP extension based on P-ext 0.5.4, see changes maded in ``core_feature_dsp.h``, only Xxldsp is supported, no N1/N2/N3 supported, and some instructions can't be supported using custom instruction, but in future, we will cooperate with IAR to do full Nuclei DSP support
  - Add more CSRs definition according to Nuclei ISA updates such as Zc/stack check
  - No more bitmanip extension intrinsic header <rvintrin.h> for gcc13
  - Fix __RV_CLAMP macro and add __MACHINE/SUPERVISOR/USER_INTERRUPT macros
  - Add __get_hart_index and SysTimer_GetHartID and modify __get_hart_id API
  - In <Device.h>, we introduced __HARTID_OFFSET and __SYSTIMER_HARTID macro to represent timer hart index relation with cpu hartid for AMP SoC
  - Clean compiler warning of NMSIS-Core header files
  - Fix Cache CCM API missing return value in some case

* **NMSIS-DSP**

  - Defined ``NUCLEI_DSP_DEFAULT``, ``NUCLEI_DSP_N1``, ``NUCLEI_DSP_N2``, ``NUCLEI_DSP_N3`` in ``riscv_math_types.h``
    according to gcc options. This means that if compile with ``--march=rv32imafc_xxldspn1x``, the ``NUCLEI_DSP_N1`` will defined,
    if compile with ``--march=rv32imafc_xxldspn2x``, the ``NUCLEI_DSP_N1`` and ``NUCLEI_DSP_N2`` will defined, and so on
  - Optimize some functions with DSP N1/N2/N3 (such as FilteringFunctions, TransformFunctions, ComplexMathFunctions)
  - RVV intrinsic APIs is update to v0.12.0
  - Add f16 support(include f16 rvv extension support)
  - Fix the use of expd80 instruction(Nuclei default dsp instruction)
  - Fix some testcases bugs(such as MatrixFunctions, TransformFunctions)

* **NMSIS-NN**

  - Defined ``NUCLEI_DSP_DEFAULT``, ``NUCLEI_DSP_N1``, ``NUCLEI_DSP_N2``, ``NUCLEI_DSP_N3`` in ``riscv_nn_math_types.h``
    according to gcc options
  - RVV intrinsic APIs is update to v0.12.0
  - Clean code(nnref lib)

* **Build System**

  - Toolchain change to gcc13, The prefix of toolchain has changed to ``riscv64-unknown-elf-``, old gcc10 ``riscv-nuclei-elf-gcc`` changed to gcc 13 ``riscv64-unknown-elf-gcc``
  - Add ci configurations to support different instruction combinations, please check ``Scripts/Build/nmsis_dsp.json`` and ``Scripts/Build/nmsis_nn.json``
  - Library naming scheme changed due to march changes, which means the library name will not be compatiable with previous release, check dsp/nn get started guide for details
  - F16 library build is supported now when zfh/zvfh extension enabled

* **CI**

  - Change NMSIS to use Nuclei SDK evalsoc as ci run target, demosoc is removed in 0.5.0 Nuclei SDK release.
  - Spilt DSP and NN test jobs to reduce ci running time
  - Build DSP/NN library in one job now, since N1/N2/N3 library naming are different, and library build speed for risc-v vector increased now

V1.1.1
------

This is the version ``V1.1.1`` release.

* **NMSIS-Core**

  - Add CIDU support via core_feature_cidu.h, and ``__CIDU_PRESENT`` macro is required in ``<Device>.h`` to represent CIDU present or not
  - Add macros of HPM m/s/u event enable, events type, events idx
  - Fix define error of HPM_INIT macro
  - Update systimer/pmp/spmp/eclic API comment and implementation
  - Add Cache ECC related APIs
  - Due to mhartid csr update, two new API added called ``__get_hart_id`` and ``__get_cluster_id``

    - mhartid in Nuclei RISC-V processor are now used to present cluster id and hart id
    - bit 0-7 is used for hart id in current cluster
    - bit 8-15 is used for cluster id of current cluster

* **NMSIS-DSP**

  - Sync with CMSIS-DSP library(branch:main, commit id:1d9e38a, after CMSIS-DSP v1.14.4)
  - Optimize some functions with RVV(such as: ComplexMathFunctions, FilteringFunctions, MatrixFunctions, StatisticsFunctions, etc.)
  - Some bugfix(riscv_mat_inverse_f32.c rvv fix, riscv_offset_q15.c p fix, riscv_fir_q15.c rvv fix etc.)

* **NMSIS-NN**

  - Sync with CMSIS-NN library(branch:main, commit id:61d1bb6, CMSIS-NN v4.1.0)
  - Compile independent, no longer depend on NMSIS-DSP
  - Optimize some functions with RVV(such as: ActivationFunctions, FullyConnectedFunctions, PoolingFunctions, etc.)
  - Some bugfix that make tflite-micro test successfully

* Documentation

  - Update sphinx and doxygen document version to 1.1.1
  - Use mathjax to render latex formulas instead of latex, which can avoid strange compile error
  - Change dsp/nn sphinx rst document structure to match dsp/nn doxygen documentation update

* NPK

  - Add ``nmsis_dsp_nn`` choice for ``nmsislibsel``, it will select nmsis dsp and nn library
  - ``nmsis_nn`` library no longer select dsp library, since now it can live without dsp library

V1.1.0
------

This is the version ``V1.1.0`` release of Nuclei MCU Software Interface Standard(NMSIS).

* **NMSIS-Core**

  - Add ``nmsis_bench.h`` for benchmark and hpm helper functions.
  - Add hpm related API
  - Update ``riscv_encoding.h`` for latest riscv changes.
  - Add ``core_feature_spmp.h`` for TEE/sPMP unit.
  - Add more Nuclei DSP N1/N2/N3 intrinsic APIs in ``core_feature_dsp.h``
  - Bring SMP/AMP support in ``core_feature_eclic.h`` and ``core_feature_timer.h``

* **NMSIS-DSP**

  - Sync with DSP library in CMSIS 5.9.0 release.
  - Add experimental RV32 Vector support.
  - Optimize with RVP/RVV for DSP library.

* **NMSIS-NN**

  - Sync with NN library in CMSIS 5.9.0 release.
  - Add experimental RV32 Vector support.
  - Optimize with RVP/RVV for NN library.

* **Build System**

  - **DSP64** is removed, and replaced by **NUCLEI_DSP_N1**, which means Nuclei DSP N1 extension present.
  - **NUCLEI_DSP_N2** and **NUCLEI_DSP_N3** are introduced to standard for Nuclei DSP N2/N3 extension present.
  - Now you build different DSP/NN library optimized Nuclei DSP N1/N2/N3 via command such as ``make NUCLEI_DSP=N1 gen``
  - Add ``nmsis_help`` make target to show help message to build nmsis dsp/nn library.
  - Add ``check_build`` and ``check_run`` make target for locally build or run on a small test suite configuration.
  - Add fpga related test script located in ``Scripts/Configs/fpga/``.
  - Fix bugs found in ``nlbuild.py`` script.

* **Device Tempates**

  - Update Device templates to support SMP/AMP and new linker script changes to align with Nuclei SDK 0.4.0

* **CI**

  - Misc changes for github and gitlab ci, see commit history
  - gitlab ci will now test ``NUCLEI_DSP=NO/N1/N2/N3`` cases and also check rv32 with VPU for DSP/NN test cases

* **Documentation**

  - Update Core/DSP/NN documentation

* **Misc**

  - Nuclei SDK 0.4.0 will use NMSIS 1.1.0

V1.0.4
------

This is the version ``V1.0.4`` release of Nuclei MCU Software Interface Standard(NMSIS).

* **NMSIS-Core**

  - add ``__CCM_PRESENT`` macro in NMSIS-Core, if CCM hardware unit is present in your CPU,
    ``__CCM_PRESENT`` macro need to be set to 1 in ``<Device>.h``
  - Fixed mtvec related api comment in ``core_feature_eclic.h``
  - Add safely write mtime/mtimecmp register for 32bit risc-v processor
  - rearrage #include header files for all NMSIS Core header files
  - removed some not good #pragma gcc diagnostic lines in ``nmsis_gcc.h``

* **NMSIS-DSP**

  - Add initial bitmainp extension support
  - Fix bug in riscv_cmplx_mult_cmplx_q15 function when XLEN=64

* **NMSIS-NN**

  - Add initial bitmainp extension support
  - Change riscv_maxpool_q7_HWC implementation for rvv
  - Re-org NN_Lib_Tests to Tests

* **Build System**

  - Change minimal version of cmake to 3.14
  - Add REBUILD=0 to reuse previous generated Makefile

* **Device Tempates**

  - Fix bss section lma and vma not aligned and tbss space not reserved

* **CI**

  - Change NMSIS to use Nuclei SDK demosoc as ci run target
  - only run ci on master/develop branch

* **Documentation**

  - Update get started guide for dsp/nn library

V1.0.3
------

This is the official release version ``V1.0.3`` release of Nuclei MCU Software Interface Standard(NMSIS).

This release is only supported by Nuclei GNU Toolchain 2022.01 and its later version,
since it required intrinsic header files in RISC-V GCC for B/P/V extensions.

The following changes has been made since ``V1.0.2``.

* **Documentation**

  - Update NMSIS Core/DSP/NN related documentation

* **Device Templates**

  - Add ``__INC_INTRINSIC_API``, ``__BITMANIP_PRESENT`` and ``__VECTOR_PRESENT`` in ``<Device>.h``
  - Add more REG/ADDR/BIT access macros in ``<Device>.h``
  - Update linker script for ``<Device>.ld`` for Nuclei C Runtime Library
  - Add tp register initialization and add early exception setup during startup in ``startup_<Device>.S``
  - Adding support for Nuclei C Runtime library

* **NMSIS-Core**

  - Update ``core_feature_eclic.h``, ``core_feature_timer.h`` and ``core_feature_dsp.h``
  - Added ``core_feature_vector.h`` and ``core_feature_bitmainp.h``
  - Add more nuclei customized csr in ``riscv_encoding.h``
  - Include rvb/rvp/rvv header files when __INC_INTRINSIC_API = 1

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
