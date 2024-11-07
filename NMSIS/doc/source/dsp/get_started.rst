.. _dsp_get_started:

Using NMSIS-DSP
===============

Here we will describe how to run the nmsis dsp examples in Nuclei QEMU.

Preparation
-----------

* Nuclei SDK, ``master`` branch(>= 0.7.0 release)
* Nuclei RISC-V GNU Toolchain 2024.06
* Nuclei QEMU 2024.06
* CMake >= 3.14
* Python 3 and pip package requirements located in

  * <nuclei-sdk>/tools/scripts/requirements.txt
  * <NMSIS>/NMSIS/Scripts/requirements.txt

Tool Setup
----------

1. Export **PATH** correctly for ``qemu`` and ``riscv64-unknown-elf-gcc``

.. code-block:: shell

    export PATH=/path/to/qemu/bin:/path/to/gcc/bin:$PATH

Build NMSIS DSP Library
-----------------------

1. Download or clone NMSIS source code into **NMSIS** directory.
2. cd to `NMSIS/NMSIS/` directory, if you want to add or remove more arches to be build,
   you can modify ``Scripts/Build/nmsis_dsp.json`` file
3. Build NMSIS DSP library and strip debug information using ``make gen_dsp_lib``
4. The dsp library will be generated into ``./Library/DSP/GCC`` folder
5. The dsp libraries will be look like this:

.. code-block::

    $ ls -lhgG Library/DSP/GCC/
    total 361M
    -rw-rw-r-- 1 3.8M Oct 20 11:52 libnmsis_dsp_rv32imac.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv32imac_xxldsp.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv32imac_xxldspn1x.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv32imac_xxldspn2x.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv32imac_xxldspn3x.a
    -rw-rw-r-- 1 3.8M Oct 20 11:52 libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 3.5M Oct 20 11:52 libnmsis_dsp_rv32imafc.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_xxldsp.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_xxldspn1x.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_xxldspn2x.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_xxldspn3x.a
    -rw-rw-r-- 1 3.5M Oct 20 11:52 libnmsis_dsp_rv32imafc_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 3.9M Oct 20 11:52 libnmsis_dsp_rv32imafc_zfh.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafc_zfh_zvfh_zve32f.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_zfh_zvfh_zve32f_xxldsp.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_zfh_zvfh_zve32f_xxldspn1x.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafc_zfh_zvfh_zve32f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_zfh_zvfh_zve32f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafc_zfh_zvfh_zve32f_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_xxldsp.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_xxldspn1x.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_xxldspn2x.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_xxldspn3x.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafc_zve32f_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 3.5M Oct 20 11:52 libnmsis_dsp_rv32imafdc.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_xxldsp.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_xxldspn1x.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_xxldspn2x.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_xxldspn3x.a
    -rw-rw-r-- 1 3.4M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 3.7M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 3.8M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zfh.a
    -rw-rw-r-- 1 3.5M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zfh_zvfh_zve32f.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zfh_zvfh_zve32f_xxldsp.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zfh_zvfh_zve32f_xxldspn1x.a
    -rw-rw-r-- 1 3.5M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zfh_zvfh_zve32f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zfh_zvfh_zve32f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 3.6M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zfh_zvfh_zve32f_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 3.1M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_xxldsp.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_xxldspn1x.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_xxldspn2x.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_xxldspn3x.a
    -rw-rw-r-- 1 3.1M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_zba_zbb_zbc_zbs_xxldspn1x.a
    -rw-rw-r-- 1 3.2M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_zba_zbb_zbc_zbs_xxldspn2x.a
    -rw-rw-r-- 1 3.3M Oct 20 11:52 libnmsis_dsp_rv32imafdc_zve32f_zba_zbb_zbc_zbs_xxldspn3x.a
    -rw-rw-r-- 1 5.0M Oct 20 11:52 libnmsis_dsp_rv64imac.a
    -rw-rw-r-- 1 5.4M Oct 20 11:52 libnmsis_dsp_rv64imac_xxldsp.a
    -rw-rw-r-- 1 4.9M Oct 20 11:52 libnmsis_dsp_rv64imac_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 5.4M Oct 20 11:52 libnmsis_dsp_rv64imac_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 4.6M Oct 20 11:52 libnmsis_dsp_rv64imafc.a
    -rw-rw-r-- 1 5.0M Oct 20 11:52 libnmsis_dsp_rv64imafc_xxldsp.a
    -rw-rw-r-- 1 4.5M Oct 20 11:52 libnmsis_dsp_rv64imafc_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 5.0M Oct 20 11:52 libnmsis_dsp_rv64imafc_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 5.1M Oct 20 11:52 libnmsis_dsp_rv64imafc_zfh.a
    -rw-rw-r-- 1 4.5M Oct 20 11:52 libnmsis_dsp_rv64imafc_zfh_zvfh_zve64f.a
    -rw-rw-r-- 1 4.7M Oct 20 11:52 libnmsis_dsp_rv64imafc_zfh_zvfh_zve64f_xxldsp.a
    -rw-rw-r-- 1 4.5M Oct 20 11:52 libnmsis_dsp_rv64imafc_zfh_zvfh_zve64f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 4.7M Oct 20 11:52 libnmsis_dsp_rv64imafc_zfh_zvfh_zve64f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv64imafc_zve64f.a
    -rw-rw-r-- 1 4.1M Oct 20 11:52 libnmsis_dsp_rv64imafc_zve64f_xxldsp.a
    -rw-rw-r-- 1 4.0M Oct 20 11:52 libnmsis_dsp_rv64imafc_zve64f_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 4.1M Oct 20 11:52 libnmsis_dsp_rv64imafc_zve64f_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 4.5M Oct 20 11:52 libnmsis_dsp_rv64imafdc.a
    -rw-rw-r-- 1 3.9M Oct 20 11:52 libnmsis_dsp_rv64imafdcv.a
    -rw-rw-r-- 1 4.1M Oct 20 11:52 libnmsis_dsp_rv64imafdcv_xxldsp.a
    -rw-rw-r-- 1 3.9M Oct 20 11:52 libnmsis_dsp_rv64imafdcv_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 4.1M Oct 20 11:52 libnmsis_dsp_rv64imafdcv_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 4.5M Oct 20 11:52 libnmsis_dsp_rv64imafdcv_zfh_zvfh.a
    -rw-rw-r-- 1 4.6M Oct 20 11:52 libnmsis_dsp_rv64imafdcv_zfh_zvfh_xxldsp.a
    -rw-rw-r-- 1 4.4M Oct 20 11:52 libnmsis_dsp_rv64imafdcv_zfh_zvfh_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 4.6M Oct 20 11:52 libnmsis_dsp_rv64imafdcv_zfh_zvfh_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 4.9M Oct 20 11:52 libnmsis_dsp_rv64imafdc_xxldsp.a
    -rw-rw-r-- 1 4.4M Oct 20 11:52 libnmsis_dsp_rv64imafdc_zba_zbb_zbc_zbs.a
    -rw-rw-r-- 1 4.9M Oct 20 11:52 libnmsis_dsp_rv64imafdc_zba_zbb_zbc_zbs_xxldsp.a
    -rw-rw-r-- 1 5.0M Oct 20 11:52 libnmsis_dsp_rv64imafdc_zfh.a

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

9. library name with extra ``_zfh`` is built for float16

   The examples are as follows:

   * ``libnmsis_dsp_rv32imafc_zfh.a``: Build for **RISCV_ARCH=rv32imafc_zfh**.
   * ``libnmsis_dsp_rv32imafdc_zfh_zve32f.a``: Build for **RISCV_ARCH=rv32imafdc_zfh_zve32f** with Vector enabled.
   * ``libnmsis_dsp_rv64imafc_zfh_zvfh_zve64f.a``: Build for **RISCV_ARCH=rv64imafc_zfh_zvfh_zve64f** with Vector enabled.
   * ``libnmsis_dsp_rv64imafdcv_zfh_zvfh.a``: Build for **RISCV_ARCH=rv64imafdcv_zfh_zvfh** with Vector enabled

.. note::

    * This NMSIS 1.2.0 is a big change version, will no longer support old gcc 10 verison, and it now only support Nuclei Toolchain 2023.10.
      The ``--march`` option has changed a lot, such as:

      - ``b`` extension changed to ``_zba_zbb_zbc_zbs`` extension,
      - ``p`` extension changed to ``_xxldsp`` , ``_xxldspn1x`` , ``_xxldspn2x`` , ``_xxldspn3x`` extensions which means
        stardard DSP extension, Nuclei N1, N2, N3 DSP extensions
      - ``v`` extension changed to ``v``, ``_zve32f``, ``_zve64f`` extensions
    * The name of libraries has changed with ``-march``, for examples, the library named ``libnmsis_dsp_rv32imacb.a`` is now named
      ``libnmsis_dsp_rv32imac_zba_zbb_zbc_zbs.a`` since ``b`` extension changed to ``_zba_zbb_zbc_zbs``
    * ``_xxldspn1x`` ``_xxldspn2x`` ``_xxldspn3x`` only valid for RISC-V 32bit processor. ``_xxldsp`` is valid for RISC-V 32/64 bit processor
    * You can also directly build both DSP and NN library using ``make gen``
    * DSP and Vector extension can be combined, such as ``_xxldsp``, ``v`` and ``v_xxldsp``, should notice the extension order
    * Vector extension currently enabled for RISC-V 32/64 bit processor

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
    # For Nuclei SDK < 0.7.0
    sed -i "s/64K/512K/g" $NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/gcc_evalsoc_ilm.ld
    # For Nuclei SDK >= 0.7.0
    sed -i 's/\([ID]LM_MEMORY_SIZE\).*/\1 = 0x80000;/' $NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/evalsoc.memory
    export SOC=evalsoc

2. Due to many of the examples could not be placed in 64K ILM and 64K DLM, and
   we are running using qemu, the ILM/DLM size in it are set to be 32MB, so we can
   change ilm/dlm to 512K/512K in the link script
   ``$NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/gcc_evalsoc_ilm.ld``
   or ``$NUCLEI_SDK_ROOT/SoC/evalsoc/Board/nuclei_fpga_eval/Source/GCC/evalsoc.memory``

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

3. Let us take ``riscv_class_marks_example`` for example:

.. code-block:: shell

   cd $NUCLEI_SDK_NMSIS/DSP/Examples/RISCV/riscv_class_marks_example

4. Run with RISCV DSP enabled and Vector enabled NMSIS-DSP library for CORE ``nx900fd``

.. code-block:: shell

    # Clean project
    make ARCH_EXT=v_xxldsp CORE=nx900fd clean
    # Build project, enable ``v`` and ``xxldsp`` optimize
    make ARCH_EXT=v_xxldsp CORE=nx900fd all
    # Run application using qemu
    make ARCH_EXT=v_xxldsp CORE=nx900fd run_qemu

5. Run with RISCV DSP disabled and Vector disabled NMSIS-DSP library for CORE ``nx900fd``

.. code-block:: shell

    make ARCH_EXT= CORE=nx900fd clean
    make ARCH_EXT= CORE=nx900fd all
    make ARCH_EXT= CORE=nx900fd run_qemu

.. note::

    * You can easily run this example in your hardware,
      if you have enough memory to run it, just modify the
      ``SOC`` to the one your are using in step 1.
