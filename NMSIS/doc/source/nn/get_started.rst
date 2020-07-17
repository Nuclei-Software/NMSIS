.. _nn_get_started:

Using NMSIS-NN
==============

Here we will describe how to run the nmsis nn examples in Nuclei Spike.

Preparation
-----------

* Nuclei Modified Spike - ``xl_spike``
* Nuclei SDK modified for ``xl_spike`` branch ``dev_xlspike``
* Nuclei RISCV GNU Toolchain
* CMake >= 3.5


Tool Setup
----------

1. Export **PATH** correctly for `xl_spike` and `riscv-nuclei-elf-gcc`

.. code-block::

    export PATH=/path/to/xl_spike/bin:/path/to/riscv-nuclei-elf-gcc/bin/:$PATH

Build NMSIS NN Library
----------------------

1. Download or clone NMSIS source code into **NMSIS** directory.
2. cd to `NMSIS/NMSIS/` directory
3. Build NMSIS NN library using ``make gen_nn_lib``
4. Strip debug informations using ``make strip_nn_lib`` to make the generated
   library smaller
5. The nn library will be generated into ``./Library/NN/GCC`` folder
6. The nn libraries will be look like this:

.. code-block::

    $ ll Library/NN/GCC/
    total 3000
    -rw-r--r-- 1 hqfang nucleisys 128482 Jul 14 14:51 libnmsis_nn_rv32imac.a
    -rw-r--r-- 1 hqfang nucleisys 281834 Jul 14 14:51 libnmsis_nn_rv32imacp.a
    -rw-r--r-- 1 hqfang nucleisys 128402 Jul 14 14:51 libnmsis_nn_rv32imafc.a
    -rw-r--r-- 1 hqfang nucleisys 282750 Jul 14 14:51 libnmsis_nn_rv32imafcp.a
    -rw-r--r-- 1 hqfang nucleisys 128650 Jul 14 14:51 libnmsis_nn_rv32imafdc.a
    -rw-r--r-- 1 hqfang nucleisys 282978 Jul 14 14:51 libnmsis_nn_rv32imafdcp.a
    -rw-r--r-- 1 hqfang nucleisys 183918 Jul 14 14:51 libnmsis_nn_rv64imac.a
    -rw-r--r-- 1 hqfang nucleisys 418598 Jul 14 14:51 libnmsis_nn_rv64imacp.a
    -rw-r--r-- 1 hqfang nucleisys 184206 Jul 14 14:51 libnmsis_nn_rv64imafc.a
    -rw-r--r-- 1 hqfang nucleisys 418070 Jul 14 14:51 libnmsis_nn_rv64imafcp.a
    -rw-r--r-- 1 hqfang nucleisys 184454 Jul 14 14:51 libnmsis_nn_rv64imafdc.a
    -rw-r--r-- 1 hqfang nucleisys 419774 Jul 14 14:51 libnmsis_nn_rv64imafdcp.a


7. library name with extra ``p`` is build with RISCV DSP enabled.

   * ``libnmsis_nn_rv32imac.a``: Build for **RISCV_ARCH=rv32imac** without DSP enabled.
   * ``libnmsis_nn_rv32imacp.a``: Build for **RISCV_ARCH=rv32imac** with DSP enabled.

.. note::

    * You can also directly build both DSP and NN library using ``make gen``
    * You can strip the generated DSP and NN library using ``make strip``


How to run
----------

1. Set environment variables ``NUCLEI_SDK_ROOT`` and ``NUCLEI_SDK_NMSIS``,
   and set Nuclei SDK SoC to `xlspike`

.. code-block:: shell

    export NUCLEI_SDK_ROOT=/path/to/nuclei_sdk
    export NUCLEI_SDK_NMSIS=/path/to/NMSIS/NMSIS
    export SOC=xlspike

2. Let us take ``./cifar10/`` for example

2. ``cd ./cifar10/``

3. Run with RISCV DSP enabled NMSIS-NN library for CORE ``n307``

.. code-block::

    # Clean project
    make DSP_ENABLE=ON CORE=n307 clean
    # Build project
    make DSP_ENABLE=ON CORE=n307 all
    # Run application using xl_spike
    make DSP_ENABLE=ON CORE=n307 run


4. Run with RISCV DSP disabled NMSIS-NN library for CORE ``n307``

.. code-block:: shell

    make DSP_ENABLE=OFF CORE=n307 clean
    make DSP_ENABLE=OFF CORE=n307 all
    make DSP_ENABLE=OFF CORE=n307 run

.. note::

    * You can easily run this example in your hardware,
      if you have enough memory to run it, just modify the
      ``SOC`` to the one your are using in step 1.
