.. _nn_get_started:

Using NMSIS-NN
==============

Here we will describe how to run the nmsis nn examples in Nuclei QEMU.

Preparation
-----------

* Nuclei SDK, ``master`` branch(>= 0.5.0 release)
* Nuclei RISC-V GNU Toolchain 2023.10
* Nuclei QEMU 2023.10
* CMake >= 3.14
* Python 3 and pip package requirements located in

  * <nuclei-sdk>/tools/scripts/requirements.txt
  * <NMSIS>/NMSIS/Scripts/requirements.txt

Tool Setup
----------

1. Export **PATH** correctly for ``qemu`` and ``riscv64-unknown-elf-gcc``

.. code-block:: shell

    export PATH=/path/to/qemu/bin:/path/to/gcc/bin:$PATH

Build NMSIS NN Library
----------------------

1. Download or clone NMSIS source code into **NMSIS** directory.
2. cd to `NMSIS/NMSIS/` directory
3. Build NMSIS NN library and strip debug information using ``make gen_nn_lib``
4. The nn library will be generated into ``./Library/NN/GCC`` folder
5. The nn libraries will be look like this:

.. code-block::

    $ ls -lhgG Library/NN/GCC/
    total 64M
    -rw-rw-r-- 1 619K Oct 20 11:55 libnmsis_nn_rv32imac.a
    -rw-rw-r-- 1 983K Oct 20 11:55 libnmsis_nn_rv32imac_xxldsp.a
    -rw-rw-r-- 1 986K Oct 20 11:55 libnmsis_nn_rv32imac_xxldspn1x.a
    -rw-rw-r-- 1 989K Oct 20 11:55 libnmsis_nn_rv32imac_xxldspn2x.a
    -rw-rw-r-- 1 992K Oct 20 11:55 libnmsis_nn_rv32imac_xxldspn3x.a
    -rw-rw-r-- 1 607K Oct 20 11:55 libnmsis_nn_rv32imac_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 961K Oct 20 11:55 libnmsis_nn_rv32imac_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 964K Oct 20 11:55 libnmsis_nn_rv32imac_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 966K Oct 20 11:55 libnmsis_nn_rv32imac_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 969K Oct 20 11:55 libnmsis_nn_rv32imac_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 620K Oct 20 11:55 libnmsis_nn_rv32imafc.a
    -rw-rw-r-- 1 984K Oct 20 11:55 libnmsis_nn_rv32imafc_xxldsp.a
    -rw-rw-r-- 1 987K Oct 20 11:55 libnmsis_nn_rv32imafc_xxldspn1x.a
    -rw-rw-r-- 1 990K Oct 20 11:55 libnmsis_nn_rv32imafc_xxldspn2x.a
    -rw-rw-r-- 1 993K Oct 20 11:55 libnmsis_nn_rv32imafc_xxldspn3x.a
    -rw-rw-r-- 1 608K Oct 20 11:55 libnmsis_nn_rv32imafc_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 962K Oct 20 11:55 libnmsis_nn_rv32imafc_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 965K Oct 20 11:55 libnmsis_nn_rv32imafc_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 968K Oct 20 11:55 libnmsis_nn_rv32imafc_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 970K Oct 20 11:55 libnmsis_nn_rv32imafc_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 716K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f.a
    -rw-rw-r-- 1 848K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_xxldsp.a
    -rw-rw-r-- 1 851K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_xxldspn1x.a
    -rw-rw-r-- 1 854K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_xxldspn2x.a
    -rw-rw-r-- 1 856K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_xxldspn3x.a
    -rw-rw-r-- 1 695K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 825K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 828K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 831K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 834K Oct 20 11:55 libnmsis_nn_rv32imafc_zve32f_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 621K Oct 20 11:55 libnmsis_nn_rv32imafdc.a
    -rw-rw-r-- 1 985K Oct 20 11:55 libnmsis_nn_rv32imafdc_xxldsp.a
    -rw-rw-r-- 1 988K Oct 20 11:55 libnmsis_nn_rv32imafdc_xxldspn1x.a
    -rw-rw-r-- 1 991K Oct 20 11:55 libnmsis_nn_rv32imafdc_xxldspn2x.a
    -rw-rw-r-- 1 994K Oct 20 11:55 libnmsis_nn_rv32imafdc_xxldspn3x.a
    -rw-rw-r-- 1 609K Oct 20 11:55 libnmsis_nn_rv32imafdc_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 963K Oct 20 11:55 libnmsis_nn_rv32imafdc_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 966K Oct 20 11:55 libnmsis_nn_rv32imafdc_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 969K Oct 20 11:55 libnmsis_nn_rv32imafdc_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 972K Oct 20 11:55 libnmsis_nn_rv32imafdc_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 718K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f.a
    -rw-rw-r-- 1 847K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_xxldsp.a
    -rw-rw-r-- 1 850K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_xxldspn1x.a
    -rw-rw-r-- 1 852K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_xxldspn2x.a
    -rw-rw-r-- 1 855K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_xxldspn3x.a
    -rw-rw-r-- 1 697K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 824K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 827K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 830K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 832K Oct 20 11:55 libnmsis_nn_rv32imafdc_zve32f_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 852K Oct 20 11:55 libnmsis_nn_rv64imac.a
    -rw-rw-r-- 1 1.4M Oct 20 11:55 libnmsis_nn_rv64imac_xxldsp.a
    -rw-rw-r-- 1 826K Oct 20 11:55 libnmsis_nn_rv64imac_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 1.4M Oct 20 11:55 libnmsis_nn_rv64imac_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 854K Oct 20 11:55 libnmsis_nn_rv64imafc.a
    -rw-rw-r-- 1 1.4M Oct 20 11:55 libnmsis_nn_rv64imafc_xxldsp.a
    -rw-rw-r-- 1 827K Oct 20 11:55 libnmsis_nn_rv64imafc_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 1.4M Oct 20 11:55 libnmsis_nn_rv64imafc_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 965K Oct 20 11:55 libnmsis_nn_rv64imafc_zve64f.a
    -rw-rw-r-- 1 1.2M Oct 20 11:55 libnmsis_nn_rv64imafc_zve64f_xxldsp.a
    -rw-rw-r-- 1 932K Oct 20 11:55 libnmsis_nn_rv64imafc_zve64f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 1.2M Oct 20 11:55 libnmsis_nn_rv64imafc_zve64f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 855K Oct 20 11:55 libnmsis_nn_rv64imafdc.a
    -rw-rw-r-- 1 972K Oct 20 11:55 libnmsis_nn_rv64imafdcv.a
    -rw-rw-r-- 1 1.2M Oct 20 11:55 libnmsis_nn_rv64imafdcv_xxldsp.a
    -rw-rw-r-- 1 939K Oct 20 11:55 libnmsis_nn_rv64imafdcv_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 1.2M Oct 20 11:55 libnmsis_nn_rv64imafdcv_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 1.4M Oct 20 11:55 libnmsis_nn_rv64imafdc_xxldsp.a
    -rw-rw-r-- 1 828K Oct 20 11:55 libnmsis_nn_rv64imafdc_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 1.4M Oct 20 11:55 libnmsis_nn_rv64imafdc_zba_zbb_zbc_zbs_xxldsp.a

7. library name with extra ``_xxldsp`` ``_xxldspn1x`` ``_xxldspn2x`` ``_xxldspn3x`` is built with RISC-V DSP enabled

   The examples are as follows:

   * ``libnmsis_dsp_rv32imac.a``: Build for **RISCV_ARCH=rv32imac** without DSP
   * ``libnmsis_dsp_rv32imac_xxldsp.a``: Build for **RISCV_ARCH=rv32imac_xxldsp** with Nuclei DSP enabled
   * ``libnmsis_dsp_rv32imac_xxldspn1x.a``: Build for **RISCV_ARCH=rv32imac_xxldspn1x** with Nuclei N1 DSP extension enabled
   * ``libnmsis_dsp_rv32imac_xxldspn2x.a``: Build for **RISCV_ARCH=rv32imac_xxldspn2x** with Nuclei N1/N2 DSP extension enabled
   * ``libnmsis_dsp_rv32imac_xxldspn3x.a``: Build for **RISCV_ARCH=rv32imac_xxldspn3x** with Nuclei N1/N2/N3 DSP extension enabled

8. library name with extra ``_zve32f`` ``_zve64f`` ``v`` is built with RISC-V Vector enabled

   The examples are as follows:

   * ``libnmsis_dsp_rv32imafc_zve32f.a``: Build for **RISCV_ARCH=rv32imafc_zve32f** with Vector enabled
   * ``libnmsis_dsp_rv32imafdc_zve32f.a``: Build for **RISCV_ARCH=rv32imafdc_zve32f** with Vector enabled
   * ``libnmsis_dsp_rv64imafc_zve64f.a``: Build for **RISCV_ARCH=rv64imafc_zve64f** with Vector enabled
   * ``libnmsis_dsp_rv64imafdcv.a``: Build for **RISCV_ARCH=rv64imafdcv** with Vector enabled

.. note::

    * This NMSIS 1.2.0 is a big change version, will no longer support old gcc 10 verison, and it now only support Nuclei Toolchain 2023.10.
      The ``--march`` option has changed a lot, such as:

      - ``b`` extension changed to ``_zba_zbb_zbc_zbs`` extension,
      - ``p`` extension changed to ``_xxldsp`` , ``_xxldspn1x`` , ``_xxldspn2x`` , ``_xxldspn3x`` extensions which means
        stardard DSP extension, Nuclei N1, N2, N3 DSP extensions
      - ``v`` extension changed to ``v``, ``_zve32f``, ``_zve64f`` extensions
    * The name of Libraries has changed with ``-march``, for examples, the library named ``libnmsis_dsp_rv32imacb.a`` is now named
      ``libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs.a`` since ``b`` extension changed to ``_zba_zbb_zbc_zbs``
    * ``_xxldspn1x`` ``_xxldspn2x`` ``_xxldspn3x`` only valid for RISC-V 32bit processor. ``_xxldsp`` is valid for RISC-V 32/64 bit processor
    * You can also directly build both DSP and NN library using ``make gen``
    * You can strip the generated DSP and NN library using ``make strip``
    * DSP and Vector extension can be combined, such as ``_xxldsp``, ``v`` and ``v_xxldsp``, should notice the extension order
    * Vector extension currently enabled for RISC-V 32/64 bit processor
    * NN library has no float16 data type, so here have no need to build float16 library

How to run
----------

1. Set environment variables ``NUCLEI_SDK_ROOT`` and ``NUCLEI_SDK_NMSIS``,
   and set Nuclei SDK SoC to `evalsoc`, and change ilm/dlm size from 64K to 512K.

.. code-block:: shell

    export NUCLEI_SDK_ROOT=/path/to/nuclei_sdk
    export NUCLEI_SDK_NMSIS=/path/to/NMSIS/NMSIS
    # Setup SDK development environment
    cd $NUCLEI_SDK_ROOT
    source setup.sh
    cd -
    # !!!!Take Care!!!!
    # change this link script will make compiled example can only run on bitstream which has 512K ILM/DLM
    sed -i "s/64K/512K/g" $NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/gcc_evalsoc_ilm.ld
    export SOC=evalsoc

2. Due to many of the examples could not be placed in 64K ILM and 64K DLM, and
   we are running using qemu, the ILM/DLM size in it are set to be 32MB, so we can
   change ilm/dlm to 512K/512K in the link script
   ``$NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/gcc_evalsoc_ilm.ld``

.. code-block:: diff

    --- a/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/gcc_evalsoc_ilm.ld
    +++ b/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/gcc_evalsoc_ilm.ld
    @@ -30,8 +30,8 @@ __HEAP_SIZE  = 2K;

    MEMORY
    {
    -  ilm (rxa!w) : ORIGIN = 0x80000000, LENGTH = 64K
    -  ram (wxa!r) : ORIGIN = 0x90000000, LENGTH = 64K
    +  ilm (rxa!w) : ORIGIN = 0x80000000, LENGTH = 512K
    +  ram (wxa!r) : ORIGIN = 0x90000000, LENGTH = 512K
    }

3. Let us take ``cifar10`` for example:

.. code-block:: shell

  cd $NUCLEI_SDK_NMSIS/NN/Examples/RISCV/cifar10/

4. Run with RISCV DSP enabled and Vector enabled NMSIS-NN library for CORE ``nx900fd``

.. code-block:: shell

    # Clean project
    make ARCH_EXT=v_xxldsp CORE=nx900fd clean
    # Build project, enable ``v`` and ``xxldsp`` optimize
    make ARCH_EXT=v_xxldsp CORE=nx900fd all
    # Run application using qemu
    make ARCH_EXT=v_xxldsp CORE=nx900fd run_qemu


5. Run with RISCV DSP disabled and Vector disabled NMSIS-NN library for CORE ``nx900fd``

.. code-block:: shell

    make ARCH_EXT= CORE=nx900fd clean
    make ARCH_EXT= CORE=nx900fd all
    make ARCH_EXT= CORE=nx900fd run_qemu

.. note::

    * You can easily run this example in your hardware,
      if you have enough memory to run it, just modify the
      ``SOC`` to the one your are using in step 1.
