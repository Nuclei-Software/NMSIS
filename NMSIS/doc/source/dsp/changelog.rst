.. dsp_changelog:

Changelog
=========

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
