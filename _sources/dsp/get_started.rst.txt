.. _dsp_get_started:

Using NMSIS-DSP
===============

Here we will describe how to run the nmsis dsp examples in Nuclei Spike.

Preparation
-----------

* Nuclei Modified Spike - ``xl_spike``
* Nuclei SDK modified for ``xl_spike`` branch ``dev_xlspike``
* Nuclei RISCV GNU Toolchain, riscv gcc 10 with vector extension is required.
* CMake >= 3.5

Tool Setup
----------

1. Export **PATH** correctly for `xl_spike` and `riscv-nuclei-elf-gcc`

.. code-block:: shell

    export PATH=/path/to/xl_spike/bin:/path/to/riscv-nuclei-elf-gcc/bin/:$PATH

Build NMSIS DSP Library
-----------------------

1. Download or clone NMSIS source code into **NMSIS** directory.
2. cd to `NMSIS/NMSIS/` directory
3. Build NMSIS DSP library using ``make gen_dsp_lib``
4. Strip debug informations using ``make strip_dsp_lib`` to make the generated
   library smaller
5. The dsp library will be generated into ``./Library/DSP/GCC`` folder
6. The dsp libraries will be look like this:

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
    * You can strip the generated DSP and NN library using ``make strip``
    * DSP and Vector extension can be combined, such as ``p``, ``v`` and ``pv``
    * Vector extension currently is only available with RISC-V 64 bit processor

How to run
----------

1. Set environment variables ``NUCLEI_SDK_ROOT`` and ``NUCLEI_SDK_NMSIS``,
   and set Nuclei SDK SoC to `xlspike`

.. code-block:: shell

    export NUCLEI_SDK_ROOT=/path/to/nuclei_sdk
    export NUCLEI_SDK_NMSIS=/path/to/NMSIS/NMSIS
    export SOC=xlspike

2. Let us take ``./riscv_class_marks_example/`` for example

3. ``cd ./riscv_class_marks_example/``

4. Run with RISCV DSP enabled and Vector enabled NMSIS-DSP library for CORE ``ux600``

.. code-block:: shell

    # Clean project
    make DSP_ENABLE=ON VECTOR_ENABLE=ON CORE=ux600 clean
    # Build project
    make DSP_ENABLE=ON VECTOR_ENABLE=ON CORE=ux600 all
    # Run application using xl_spike
    make DSP_ENABLE=ON VECTOR_ENABLE=ON CORE=ux600 run

5. Run with RISCV DSP disabled and Vector disabled NMSIS-DSP library for CORE ``ux600``

.. code-block:: shell

    make DSP_ENABLE=OFF VECTOR_ENABLE=OFF CORE=ux600 clean
    make DSP_ENABLE=OFF VECTOR_ENABLE=OFF CORE=ux600 all
    make DSP_ENABLE=OFF VECTOR_ENABLE=OFF CORE=ux600 run

.. note::

    * You can easily run this example in your hardware,
      if you have enough memory to run it, just modify the
      ``SOC`` to the one your are using in step 1.
