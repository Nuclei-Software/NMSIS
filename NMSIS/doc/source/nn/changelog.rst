.. nn_changelog:

Changelog
=========

V1.2.1
------

This is release ``1.2.1`` version of NMSIS-NN library.

* Sync with CMSIS-NN library(CMSIS-NN v5.0.0, commit id:bfc54edb)
* Optimize some functions with Nuclei DSP N3 instruction(such as: riscv_convolve_s8, riscv_fully_connected_s8, etc.)
* Optimize the rvv implementation of fullyconnect s8 function
* Fixed ``riscv_nn_softmax_common_s8.c`` rvv bug

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

    - The name of libraries changed due to ``-march`` in gcc13 updated, for example, the library named ``libnmsis_nn_rv32imacb.a`` is now named
      ``libnmsis_nn_rv32imac_zba_zbb_zbc_zbs.a`` since ``b`` extension changed to ``_zba_zbb_zbc_zbs``

This is release ``1.2.0`` version of NMSIS-NN library.

* Defined ``NUCLEI_DSP_DEFAULT``, ``NUCLEI_DSP_N1``, ``NUCLEI_DSP_N2``, ``NUCLEI_DSP_N3`` in ``riscv_nn_math_types.h``
  according to gcc options now, no longer define it in cmake files.
* RVV intrinsic APIs is update to v0.12.0
* Clean code(nnref lib)

V1.1.1
------

This is release ``1.1.1`` version of NMSIS-NN library.

* Sync changes from CMSIS NN v4.1.0
* Optimized more for RVP/RVV
* Add support for RV32 Vector
* Some bugfix that make tflite-micro test successfully

V1.1.0
------

This is release ``1.1.0`` version of NMSIS-NN library.

* Sync changes from CMSIS 5.9.0 release
* Optimized more for RVP/RVV
* Add experimental support for RV32 Vector

V1.0.3
------

This is release ``1.0.3`` version of NMSIS-NN library.

* Update build system for NMSIS-NN library
* Rename ``RISCV_VECTOR`` to ``RISCV_MATH_VECTOR`` in header file and source code
* Using new python script to generate NMSIS-NN library
* Support Nuclei RISC-V GCC 10.2

V1.0.2
------

This is release ``1.0.2`` version of NMSIS-NN library.

* Sync up to CMSIS NN library 3.0.0
* Initial support for RISC-V  vector extension support


V1.0.1
------

This is release ``V1.0.1`` version of NMSIS-DSP library.

* Both Nuclei RISC-V 32 and 64 bit cores are supported now.
* Libraries are optimized for RISC-V 32 and 64 bit DSP instructions.
* The DSP examples are now using Nuclei SDK as running environment.


V1.0.0
------

This is the first version of NMSIS-NN library.

We adapt the CMSIS-NN v1.0.0 library to use RISCV DSP instructions, all the API names now are renamed from ``arm_xxx`` to ``riscv_xxx``.
