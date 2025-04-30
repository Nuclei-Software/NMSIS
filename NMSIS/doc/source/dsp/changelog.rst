.. dsp_changelog:

Changelog
=========

.. note::

    NMSIS DSP Library changelog will not maintain here in future, please check :ref:`changelog` here.

latest
------

See :ref:`changelog`

V1.3.1
------

This is release ``1.3.1`` version of NMSIS-DSP library.

* Update get started guide for build NMSIS-DSP library
* Optimize fredsum, use less fredsum and more fmacc for rvv implementation
* Sync cmake file changes


V1.3.0
------

This is release ``1.3.0`` version of NMSIS-DSP library.

* Sync with CMSIS-DSP Library v1.16.2
* Add initial version of NMSIS DSP Benchmark
* Fix bug found for ComplexMathFunctions when RISCV_MATH_LOOPUNROLL not defined

V1.2.1
------

This is release ``1.2.1`` version of NMSIS-DSP library.

* Sync with CMSIS-DSP library(CMSIS-DSP v1.15.0, commit id:8f8bb8d5)
* Add a few Nuclei DSP N2 intrinsic APIs in ``core_feature_dsp.h``, they are: ``__RV_DPACK32``, ``__RV_DSUNPKD810``, ``__RV_DSUNPKD820``,
  ``__RV_DSUNPKD830``, ``__RV_DSUNPKD831``, ``__RV_DSUNPKD832``, ``__RV_DZUNPKD810``, ``__RV_DZUNPKD820``, ``__RV_DZUNPKD830``,
  ``__RV_DZUNPKD831``, ``__RV_DZUNPKD832``. And change ``__RV_DPKBB32`` to ``__RV_DPACK32`` for better performance
* Unified instruction naming style in ``core_feature_dsp.h``, such as: change ``DSMMULU`` to ``DSMMUL.u``, change ``DKWMMULU`` to ``DKWMMUL.u``,
  change ``DKMMACU`` to ``DKMMAC.u``, change ``DKMMSBU`` to ``DKMMSB.u``, change ``DSMAQASU`` to ``DSMAQA.SU``, change ``DDSMAQASU`` to ``DDSMAQA.SU``
*  Optimize the rvv implementation of some MatMult and Complex MatMult functions
* Fixed some doc erros in ``core_feature_dsp.h``
* Fixed ``riscv_lms_q31.c`` rvv bug

V1.2.0
------

.. note::

    - This 1.2.0 version will no longer support old gcc 10 verison, and it now only support Nuclei Toolchain
      2023.10(gcc13 and clang17) or later. The major changes that can be felt are as follows:

    - The prefix of toolchain has changed from ``riscv-nuclei-elf-`` to ``riscv64-unknown-elf-``
    - The ``-march`` option has changed a lot, for example:

      - ``b`` extension changed to ``_zba_zbb_zbc_zbs`` extension,
      - ``p`` extension changed to ``_xxldsp`` or ``_xxldspn1x`` or ``_xxldspn2x`` or ``_xxldspn3x`` extensions which means
        stardard DSP extension, Nuclei N1, N2, N3 DSP extensions
      - ``v`` extension changed to ``v`` or ``_zve32f`` or ``_zve64f`` extensions according to the riscv cpu isa used

      These extensions need be combined in a certain order to get a correct arch name to match the prebuilt library name, please be cautious

    - The name of libraries changed due to ``-march`` in gcc13 updated, for example, the library named ``libnmsis_dsp_rv32imacb.a`` is now named
      ``libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs.a`` since ``b`` extension changed to ``_zba_zbb_zbc_zbs``

This is release ``1.2.0`` version of NMSIS-DSP library.

* Defined ``NUCLEI_DSP_DEFAULT``, ``NUCLEI_DSP_N1``, ``NUCLEI_DSP_N2``, ``NUCLEI_DSP_N3`` in ``riscv_math_types.h``
  according to gcc options. This means that if compile with ``--march=rv32imafc_xxldspn1x``, the ``NUCLEI_DSP_N1`` will defined,
  if compile with ``--march=rv32imafc_xxldspn2x``, the ``NUCLEI_DSP_N1`` and ``NUCLEI_DSP_N2`` will defined, and so on
* Optimize some functions with DSP N1/N2/N3 (such as FilteringFunctions, TransformFunctions, ComplexMathFunctions)
* RVV intrinsic APIs is update to v0.12.0
* Add f16 support(include f16 rvv extension support)
* Fix the use of expd80 instruction(Nuclei default dsp instruction)
* Fix some testcases bugs(such as MatrixFunctions, TransformFunctions)

V1.1.1
------

This is release ``1.1.1`` version of NMSIS-DSP library.

* Sync changes from CMSIS DSP commit 1d9e38a, version after v1.14.4
* Optimized more for RVP/RVV
* Add support for RV32 Vector
* Some bugfix(riscv_mat_inverse_f32.c rvv fix, riscv_offset_q15.c p fix, riscv_fir_q15.c rvv fix etc.)

V1.1.0
------

This is release ``1.1.0`` version of NMSIS-DSP library.

* Sync changes from CMSIS 5.9.0 release
* Optimized more for RVP/RVV
* Add experimental support for RV32 Vector

V1.0.3
------

This is release ``1.0.3`` version of NMSIS-DSP library.

* Update build system for NMSIS-DSP library
* Rename ``RISCV_VECTOR`` to ``RISCV_MATH_VECTOR`` in header file and source code
* Using new python script to generate NMSIS-DSP library
* Fix riscv_float_to_q31 function for rv64imafcv target
* Change vfredsum to vfredusum when using vector intrinsic function due to vector spec 1.0
* Support Nuclei RISC-V GCC 10.2

V1.0.2
------

This is release ``1.0.2`` version of NMSIS-DSP library.

* Sync up to CMSIS DSP library 1.9.0
* Adding initial support for RISC-V vector extension support
* **Caution**: ``riscv_math.h`` is separated into several header files.
  Extra ``PrivateInclude`` folder is included as header folder.

V1.0.1
------

This is release ``V1.0.1`` version of NMSIS-DSP library.

* Both Nuclei RISC-V 32 and 64 bit cores are supported now.
* Libraries are optimized for RISC-V 32 and 64 bit DSP instructions.
* The NN examples are now using Nuclei SDK as running environment.

V1.0.0
------

This is the first version of NMSIS-DSP library.

We adapt the CMSIS-DSP v1.6.0 library to use RISCV DSP instructions, all the API names now are renamed from ``arm_xxx`` to ``riscv_xxx``.
