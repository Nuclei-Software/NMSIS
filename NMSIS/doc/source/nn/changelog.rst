.. nn_changelog:

Changelog
=========

V1.2.0
------
.. note::
  -  This 1.2.0 version will no longer support old gcc 10 verison, and it now support Nuclei Toolchain 2023.10(
     gcc 13 and clang 17)

This is release ``1.2.0`` version of NMSIS-NN library.

* Defined ``NUCLEI_DSP_DEFAULT``, ``NUCLEI_DSP_N1``, ``NUCLEI_DSP_N2``, ``NUCLEI_DSP_N3`` according to gcc options in ``riscv_nn_math_types.h``
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
