.. _dsp_overview:

Overview
========

Introduction
------------

This user manual describes the NMSIS DSP software library,
a suite of common signal processing functions for use on |nuclei_core| based devices.

The library is divided into a number of functions each covering a specific category:

- Basic math functions
- Fast math functions
- Complex math functions
- Filters
- Matrix functions
- Transform functions
- Motor control functions
- Statistical functions
- Support functions
- Interpolation functions

The library has separate functions for operating on 8-bit integers, 16-bit integers,
32-bit integer and 32-bit floating-point values.

Using the Library
-----------------

The library functions are declared in the public file ``riscv_math.h`` which is placed in the *NMSIS/DSP/Include* folder.

Simply include this file and link the appropriate library in the application and begin calling the library functions.

The Library supports single public header file ``riscv_math.h`` for |nuclei_core| cores with little endian.
Same header file will be used for floating point unit(FPU) variants.

Examples
--------

The library ships with :ref:`a number of examples<nmsis_dsp_api_examples>` which demonstrate how to use the library functions.

Toolchain Support
-----------------

The library has been developed and tested with RISCV GCC Toolchain.

The library is being tested in GCC toolchain and updates on this activity will be made available shortly.

Building the Library
--------------------

The library installer contains a **Makefile** to rebuild libraries on Nuclei RISCV GCC toolchain in the **NMSIS/** folder.

The libraries can be built by run ``make gen_dsp_lib``, it will build and install DSP library into **Library/DSP/GCC** folder. 

Preprocessor Macros
-------------------

Each library project have different preprocessor macros.

RISCV_MATH_MATRIX_CHECK:
  Define macro RISCV_MATH_MATRIX_CHECK for checking on the input and output sizes of matrices

RISCV_MATH_ROUNDING:
  Define macro RISCV_MATH_ROUNDING for rounding on support functions

RISCV_MATH_LOOPUNROLL:
  Define macro RISCV_MATH_LOOPUNROLL to enable manual loop unrolling in DSP functions
