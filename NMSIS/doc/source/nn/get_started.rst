.. _nn_get_started:

Using NMSIS-NN
==============

Here we will describe how to run the nmsis nn examples in Nuclei QEMU.

Preparation
-----------

* Nuclei SDK, ``master`` branch(== 0.4.1 release)
* Nuclei RISCV GNU Toolchain 2022.12
* Nuclei QEMU 2022.12
* CMake >= 3.14
* Python 3 and pip package requirements located in

  * <nuclei-sdk>/tools/scripts/requirements.txt
  * <NMSIS>/NMSIS/Scripts/requirements.txt

Tool Setup
----------

1. Export **PATH** correctly for ``qemu`` and ``riscv-nuclei-elf-gcc``

.. code-block::

    export PATH=/path/to/qemu/bin:/path/to/riscv-nuclei-elf-gcc/bin/:$PATH

Build NMSIS NN Library
----------------------

1. Download or clone NMSIS source code into **NMSIS** directory.
2. cd to `NMSIS/NMSIS/` directory
3. Build NMSIS NN library optimized with Nuclei DSP N1 extension and strip debug information using ``make NUCLEI_DSP=N1 gen_nn_lib``

   - Possible values of **NUCLEI_DSP** are ``NO, N1, N2, N3``.
   - ``NUCLEI_DSP=NO`` means will not enable Nuclei N1/N2/N3 DSP extension to optimize library.
   - ``NUCLEI_DSP=N1`` means will enable extra Nuclei N1 DSP extension to optimize library.
   - ``NUCLEI_DSP=N2`` means will enable extra Nuclei N1/N2 DSP extension to optimize library.
   - ``NUCLEI_DSP=N3`` means will enable extra Nuclei N1/N2/N3 DSP extension to optimize library.

4. The nn library will be generated into ``./Library/NN/GCC`` folder
5. The nn libraries will be look like this:

.. code-block::

    $ ls -lh Library/NN/GCC/
    total 36M
    -rw-r--r-- 1 hqfang hqfang 513K Dec 30 12:45 libnmsis_nn_rv32imac.a
    -rw-r--r-- 1 hqfang hqfang 500K Dec 30 12:45 libnmsis_nn_rv32imacb.a
    -rw-r--r-- 1 hqfang hqfang 829K Dec 30 12:45 libnmsis_nn_rv32imacbp.a
    -rw-r--r-- 1 hqfang hqfang 842K Dec 30 12:45 libnmsis_nn_rv32imacp.a
    -rw-r--r-- 1 hqfang hqfang 514K Dec 30 12:45 libnmsis_nn_rv32imafc.a
    -rw-r--r-- 1 hqfang hqfang 507K Dec 30 12:45 libnmsis_nn_rv32imafcb.a
    -rw-r--r-- 1 hqfang hqfang 841K Dec 30 12:45 libnmsis_nn_rv32imafcbp.a
    -rw-r--r-- 1 hqfang hqfang 880K Dec 30 12:45 libnmsis_nn_rv32imafcbpv.a
    -rw-r--r-- 1 hqfang hqfang 778K Dec 30 12:45 libnmsis_nn_rv32imafcbv.a
    -rw-r--r-- 1 hqfang hqfang 853K Dec 30 12:45 libnmsis_nn_rv32imafcp.a
    -rw-r--r-- 1 hqfang hqfang 890K Dec 30 12:45 libnmsis_nn_rv32imafcpv.a
    -rw-r--r-- 1 hqfang hqfang 795K Dec 30 12:45 libnmsis_nn_rv32imafcv.a
    -rw-r--r-- 1 hqfang hqfang 514K Dec 30 12:45 libnmsis_nn_rv32imafdc.a
    -rw-r--r-- 1 hqfang hqfang 507K Dec 30 12:45 libnmsis_nn_rv32imafdcb.a
    -rw-r--r-- 1 hqfang hqfang 842K Dec 30 12:45 libnmsis_nn_rv32imafdcbp.a
    -rw-r--r-- 1 hqfang hqfang 880K Dec 30 12:45 libnmsis_nn_rv32imafdcbpv.a
    -rw-r--r-- 1 hqfang hqfang 781K Dec 30 12:45 libnmsis_nn_rv32imafdcbv.a
    -rw-r--r-- 1 hqfang hqfang 854K Dec 30 12:45 libnmsis_nn_rv32imafdcp.a
    -rw-r--r-- 1 hqfang hqfang 890K Dec 30 12:45 libnmsis_nn_rv32imafdcpv.a
    -rw-r--r-- 1 hqfang hqfang 798K Dec 30 12:45 libnmsis_nn_rv32imafdcv.a
    -rw-r--r-- 1 hqfang hqfang 734K Dec 30 12:45 libnmsis_nn_rv64imac.a
    -rw-r--r-- 1 hqfang hqfang 712K Dec 30 12:45 libnmsis_nn_rv64imacb.a
    -rw-r--r-- 1 hqfang hqfang 1.2M Dec 30 12:45 libnmsis_nn_rv64imacbp.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imacp.a
    -rw-r--r-- 1 hqfang hqfang 741K Dec 30 12:45 libnmsis_nn_rv64imafc.a
    -rw-r--r-- 1 hqfang hqfang 719K Dec 30 12:45 libnmsis_nn_rv64imafcb.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imafcbp.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imafcbpv.a
    -rw-r--r-- 1 hqfang hqfang 1.1M Dec 30 12:45 libnmsis_nn_rv64imafcbv.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imafcp.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imafcpv.a
    -rw-r--r-- 1 hqfang hqfang 1.2M Dec 30 12:45 libnmsis_nn_rv64imafcv.a
    -rw-r--r-- 1 hqfang hqfang 741K Dec 30 12:45 libnmsis_nn_rv64imafdc.a
    -rw-r--r-- 1 hqfang hqfang 718K Dec 30 12:45 libnmsis_nn_rv64imafdcb.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imafdcbp.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imafdcbpv.a
    -rw-r--r-- 1 hqfang hqfang 1.1M Dec 30 12:45 libnmsis_nn_rv64imafdcbv.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imafdcp.a
    -rw-r--r-- 1 hqfang hqfang 1.3M Dec 30 12:45 libnmsis_nn_rv64imafdcpv.a
    -rw-r--r-- 1 hqfang hqfang 1.2M Dec 30 12:45 libnmsis_nn_rv64imafdcv.a

7. library name with extra ``p`` is build with RISCV DSP enabled.

   * ``libnmsis_nn_rv32imac.a``: Build for **RISCV_ARCH=rv32imac** without DSP enabled.
   * ``libnmsis_nn_rv32imacp.a``: Build for **RISCV_ARCH=rv32imac** with DSP enabled.

8. library name with extra ``v`` is build with RISCV Vector enabled, only valid for RISC-V 64bit processor.

   * ``libnmsis_nn_rv64imac.a``: Build for **RISCV_ARCH=rv64imac** without Vector.
   * ``libnmsis_nn_rv64imacv.a``: Build for **RISCV_ARCH=rv64imac** with Vector enabled.

.. note::

    * You can also directly build both DSP and NN library using ``make gen``
    * You can strip the generated DSP and NN library using ``make strip``
    * DSP and Vector extension can be combined, such as ``p``, ``v`` and ``pv``
    * Vector extension currently enabled for RISC-V 32/64 bit processor
    * RV32 Vector support are experimental, not stable, take care

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

3. Let us take ``cifar10`` for example,
  ``cd $NUCLEI_SDK_NMSIS/NN/Examples/RISCV/cifar10/`` to first

4. Run with RISCV DSP enabled and Vector enabled NMSIS-NN library for CORE ``nx900fd``

.. code-block::

    # Clean project
    make ARCH_EXT=pv CORE=nx900fd clean
    # Build project
    make ARCH_EXT=pv CORE=nx900fd all
    # Run application using qemu
    make ARCH_EXT=pv CORE=nx900fd run_qemu


5. Run with RISCV DSP disabled and Vector disabled NMSIS-NN library for CORE ``nx900fd``

.. code-block:: shell

    make ARCH_EXT= CORE=nx900fd clean
    make ARCH_EXT= CORE=nx900fd all
    make ARCH_EXT= CORE=nx900fd run_qemu

.. note::

    * You can easily run this example in your hardware,
      if you have enough memory to run it, just modify the
      ``SOC`` to the one your are using in step 1.
