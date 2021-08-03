.. dsp_changelog:

Changelog
=========

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
