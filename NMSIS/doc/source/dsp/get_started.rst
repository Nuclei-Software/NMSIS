.. _dsp_get_started:

Using NMSIS-DSP
===============

Here we will describe how to run the nmsis dsp examples in Nuclei Spike.

Preparation
-----------

* Nuclei Modified Spike
* Nuclei Modified PK
* Nuclei RISCV GNU Toolchain

Tool Setup
----------

1. Export **PATH** correctly for `spike` and `riscv-nuclei-elf-gcc`

.. code-block:: shell
    
    export PATH=/path/to/spike/bin:/path/to/riscv-nuclei-elf-gcc/bin/:$PATH


2. Export **PK** correctly for `spike` commond.

.. code-block:: shell
    
    export PK=/path/to/spike/riscv32-unknown-elf/bin/pk


Build NMSIS DSP Library
-----------------------

1. Download or clone NMSIS source code into **NMSIS** directory.
2. cd to `NMSIS/NMSIS/` directory
3. Build NMSIS DSP library using ``make gen_dsp_lib``
4. The dsp library will be generated into ``./Library/DSP/GCC`` folder
5. The dsp libraries will be look like this:

.. code-block::
    
    $ ll Library/DSP/GCC/
    total 50464
    -rw-r--r-- 1 hqfang nucleisys  7510076 Dec 26 10:30 libnmsis_dsp_rv32imac.a
    -rw-r--r-- 1 hqfang nucleisys 10010616 Dec 26 10:30 libnmsis_dsp_rv32imacp.a
    -rw-r--r-- 1 hqfang nucleisys  7355624 Dec 26 10:30 libnmsis_dsp_rv32imafc.a
    -rw-r--r-- 1 hqfang nucleisys  9725584 Dec 26 10:30 libnmsis_dsp_rv32imafcp.a
    -rw-r--r-- 1 hqfang nucleisys  7352364 Dec 26 10:30 libnmsis_dsp_rv32imafdc.a
    -rw-r--r-- 1 hqfang nucleisys  9707712 Dec 26 10:30 libnmsis_dsp_rv32imafdcp.a

6. library name with extra ``p`` is build with RISCV DSP enabled.
   
   * ``libnmsis_dsp_rv32imac.a``: Build for **RISCV_ARCH=rv32imac** without DSP enabled.
   * ``libnmsis_dsp_rv32imacp.a``: Build for **RISCV_ARCH=rv32imac** with DSP enabled.

How to run
----------

1. Let us take `./riscv_class_marks_example/` for example

2. `cd ./riscv_class_marks_example/`

3. Run with RISCV DSP enabled NMSIS-DSP library

.. code-block:: shell

    make DSP_ENABLE=ON -B run

4. Run with RISCV DSP disabled NMSIS-DSP library

.. code-block:: shell
    
    make DSP_ENABLE=OFF -B run
