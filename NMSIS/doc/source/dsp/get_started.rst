.. _dsp_get_started:

Using NMSIS-DSP
===============

Here we will describe how to run the nmsis dsp examples in Nuclei QEMU.

Preparation
-----------

* Nuclei SDK, ``master`` branch(>= 0.3.9 release)
* Nuclei RISCV GNU Toolchain 2022.08
* Nuclei QEMU 2022.08
* CMake >= 3.14
* Python 3 and pip package requirements located in

  * <nuclei-sdk>/tools/scripts/requirements.txt
  * <NMSIS>/NMSIS/Scripts/requirements.txt

Tool Setup
----------

1. Export **PATH** correctly for ``qemu`` and ``riscv-nuclei-elf-gcc``

.. code-block:: shell

    export PATH=/path/to/qemu/bin:/path/to/riscv-nuclei-elf-gcc/bin/:$PATH

Build NMSIS DSP Library
-----------------------

1. Download or clone NMSIS source code into **NMSIS** directory.
2. cd to `NMSIS/NMSIS/` directory
3. Build NMSIS DSP library and strip debug information using ``make gen_dsp_lib``
4. The dsp library will be generated into ``./Library/DSP/GCC`` folder
5. The dsp libraries will be look like this:

.. code-block::

    $ ls -lh Library/DSP/GCC/
    total 52M
    -rw-rw-r-- 1 hqfang nucleisys 2.5M Aug  6 16:07 libnmsis_dsp_rv32imac.a
    -rw-rw-r-- 1 hqfang nucleisys 3.2M Aug  6 16:07 libnmsis_dsp_rv32imacp.a
    -rw-rw-r-- 1 hqfang nucleisys 2.3M Aug  6 16:07 libnmsis_dsp_rv32imafc.a
    -rw-rw-r-- 1 hqfang nucleisys 3.0M Aug  6 16:07 libnmsis_dsp_rv32imafcp.a
    -rw-rw-r-- 1 hqfang nucleisys 2.3M Aug  6 16:07 libnmsis_dsp_rv32imafdc.a
    -rw-rw-r-- 1 hqfang nucleisys 3.0M Aug  6 16:07 libnmsis_dsp_rv32imafdcp.a
    -rw-rw-r-- 1 hqfang nucleisys 3.1M Aug  6 16:07 libnmsis_dsp_rv64imac.a
    -rw-rw-r-- 1 hqfang nucleisys 4.3M Aug  6 16:07 libnmsis_dsp_rv64imacp.a
    -rw-rw-r-- 1 hqfang nucleisys 3.0M Aug  6 16:07 libnmsis_dsp_rv64imafc.a
    -rw-rw-r-- 1 hqfang nucleisys 4.0M Aug  6 16:07 libnmsis_dsp_rv64imafcp.a
    -rw-rw-r-- 1 hqfang nucleisys 3.9M Aug  6 16:08 libnmsis_dsp_rv64imafcpv.a
    -rw-rw-r-- 1 hqfang nucleisys 3.6M Aug  6 16:08 libnmsis_dsp_rv64imafcv.a
    -rw-rw-r-- 1 hqfang nucleisys 3.0M Aug  6 16:08 libnmsis_dsp_rv64imafdc.a
    -rw-rw-r-- 1 hqfang nucleisys 4.0M Aug  6 16:08 libnmsis_dsp_rv64imafdcp.a
    -rw-rw-r-- 1 hqfang nucleisys 3.9M Aug  6 16:08 libnmsis_dsp_rv64imafdcpv.a
    -rw-rw-r-- 1 hqfang nucleisys 3.6M Aug  6 16:08 libnmsis_dsp_rv64imafdcv.a

7. library name with extra ``p`` is build with RISCV DSP enabled.

   * ``libnmsis_dsp_rv32imac.a``: Build for **RISCV_ARCH=rv32imac** without DSP.
   * ``libnmsis_dsp_rv32imacp.a``: Build for **RISCV_ARCH=rv32imac** with DSP enabled.

8. library name with extra ``v`` is build with RISCV Vector enabled, only valid for RISC-V 64bit processor.

   * ``libnmsis_dsp_rv64imac.a``: Build for **RISCV_ARCH=rv64imac** without Vector.
   * ``libnmsis_dsp_rv64imacv.a``: Build for **RISCV_ARCH=rv64imac** with Vector enabled.

.. note::

    * You can also directly build both DSP and NN library using ``make gen``
    * DSP and Vector extension can be combined, such as ``p``, ``v`` and ``pv``
    * Vector extension currently is only available with RISC-V 64 bit processor

How to run
----------

1. Set environment variables ``NUCLEI_SDK_ROOT`` and ``NUCLEI_SDK_NMSIS``,
   and set Nuclei SDK SoC to `demosoc`, and change ilm/dlm size from 64K to 512K.

.. code-block:: shell

    export NUCLEI_SDK_ROOT=/path/to/nuclei_sdk
    export NUCLEI_SDK_NMSIS=/path/to/NMSIS/NMSIS
    # Setup SDK development environment
    cd $NUCLEI_SDK_ROOT
    source setup.sh
    cd -
    # !!!!Take Care!!!!
    # change this link script will make compiled example can only run on bitstream which has 512K ILM/DLM
    sed -i "s/64K/512K/g" $NUCLEI_SDK_ROOT/SoC/demosoc/Board/nuclei_fpga_eval/Source/GCC/gcc_demosoc_ilm.ld
    export SOC=demosoc

2. Due to many of the examples could not be placed in 64K ILM and 64K DLM, and
   we are running using qemu, the ILM/DLM size in it are set to be 32MB, so we can
   change ilm/dlm to 512K/512K in the link script
   ``$NUCLEI_SDK_ROOT/SoC/demosoc/Board/nuclei_fpga_eval/Source/GCC/gcc_demosoc_ilm.ld``

.. code-block:: diff

    --- a/SoC/demosoc/Board/nuclei_fpga_eval/Source/GCC/gcc_demosoc_ilm.ld
    +++ b/SoC/demosoc/Board/nuclei_fpga_eval/Source/GCC/gcc_demosoc_ilm.ld
    @@ -30,8 +30,8 @@ __HEAP_SIZE  = 2K;
    
    MEMORY
    {
    -  ilm (rxa!w) : ORIGIN = 0x80000000, LENGTH = 64K
    -  ram (wxa!r) : ORIGIN = 0x90000000, LENGTH = 64K
    +  ilm (rxa!w) : ORIGIN = 0x80000000, LENGTH = 512K
    +  ram (wxa!r) : ORIGIN = 0x90000000, LENGTH = 512K
    }

3. Let us take ``riscv_class_marks_example`` for example,
  ``cd $NUCLEI_SDK_NMSIS/DSP/Examples/RISCV/riscv_class_marks_example`` to first

4. Run with RISCV DSP enabled and Vector enabled NMSIS-DSP library for CORE ``nx900fd``

.. code-block:: shell

    # Clean project
    make ARCH_EXT=pv CORE=nx900fd clean
    # Build project
    make ARCH_EXT=pv CORE=nx900fd all
    # Run application using qemu
    make ARCH_EXT=pv CORE=nx900fd run_qemu

5. Run with RISCV DSP disabled and Vector disabled NMSIS-DSP library for CORE ``nx900fd``

.. code-block:: shell

    make ARCH_EXT= CORE=nx900fd clean
    make ARCH_EXT= CORE=nx900fd all
    make ARCH_EXT= CORE=nx900fd run_qemu

.. note::

    * You can easily run this example in your hardware,
      if you have enough memory to run it, just modify the
      ``SOC`` to the one your are using in step 1.
