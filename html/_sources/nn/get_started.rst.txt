.. _nn_get_started:

Using NMSIS-NN
==============

Here we will describe how to run the nmsis nn examples in Nuclei Spike.

Preparation
-----------

* Nuclei Modified Spike
* Nuclei Modified PK
* Nuclei RISCV GNU Toolchain
* Ubuntu 18.04

Tool Setup
----------

1. Export **PATH** correctly for `spike` and `riscv-nuclei-elf-gcc`

.. code-block::

    export PATH=/path/to/spike/bin:/path/to/riscv-nuclei-elf-gcc/bin/:$PATH

2. Export **PK** correctly for `spike` commond.

.. code-block::

    export PK=/path/to/spike/riscv32-unknown-elf/bin/pk

Build NMSIS NN Library
----------------------

1. Download or clone NMSIS source code into **NMSIS** directory.
2. cd to `NMSIS/NMSIS/` directory
3. Build NMSIS NN library using ``make gen_nn_lib``
4. The nn library will be generated into ``./Library/NN/GCC`` folder
5. The nn libraries will be look like this:

.. code-block::
    
    $ ll Library/NN/GCC/
    total 8632
    -rw-r--r-- 1 hqfang nucleisys 1175294 Dec 26 10:30 libnmsis_nn_rv32imac.a
    -rw-r--r-- 1 hqfang nucleisys 1766258 Dec 26 10:30 libnmsis_nn_rv32imacp.a
    -rw-r--r-- 1 hqfang nucleisys 1175530 Dec 26 10:30 libnmsis_nn_rv32imafc.a
    -rw-r--r-- 1 hqfang nucleisys 1768718 Dec 26 10:30 libnmsis_nn_rv32imafcp.a
    -rw-r--r-- 1 hqfang nucleisys 1175870 Dec 26 10:30 libnmsis_nn_rv32imafdc.a
    -rw-r--r-- 1 hqfang nucleisys 1769062 Dec 26 10:30 libnmsis_nn_rv32imafdcp.a

6. library name with extra ``p`` is build with RISCV DSP enabled.

   * ``libnmsis_nn_rv32imac.a``: Build for **RISCV_ARCH=rv32imac** without DSP enabled.
   * ``libnmsis_nn_rv32imacp.a``: Build for **RISCV_ARCH=rv32imac** with DSP enabled.

How to run
----------

1. Let us take `./cifar10/` for example

2. `cd ./cifar10/`

3. Run with RISCV DSP enabled NMSIS-NN library

.. code-block::

    make DSP_ENABLE=ON -B run


Here is sample output for above command:

.. code-block::
    
    Makefile:36: "enable dsp"
    riscv-nuclei-elf-gcc -march=rv32imafdc -mabi=ilp32d -O2 -g -D__RISCV_FEATURE_DSP=1 -D__RISCV_FEATURE_DSP=1 -DTEST_IMAGE=cat1  riscv_nnexamples_cifar10.c -L../../../../Library/DSP/GCC -L../../../../Library/NN/GCC -lnmsis_nn_rv32imafdcp -lnmsis_dsp_rv32imafdcp -lm -I. -I../../../../Core/Include -I../../../../DSP/Include -I../../../../NN/Include -o run.elf
    riscv-nuclei-elf-objcopy -O binary run.elf run.bin
    spike --isa=rv32gct /home/share/tools/riscv/32/riscv32-unknown-elf/bin/pk run.elf
    bbl loader
    start execution cnn to classify image cat1
    CSV, BENCH START, 404308
    CSV, extra_cost, 7
    CSV, preprocess, 21519
    CSV, riscv_convolve_HWC_q7_RGB, 3961749
    CSV, riscv_relu_q7, 40981
    CSV, riscv_maxpool_q7_HWC, 153919
    CSV, riscv_convolve_HWC_q7_fast, 3067538
    CSV, riscv_relu_q7, 5141
    CSV, riscv_maxpool_q7_HWC, 24151
    CSV, riscv_convolve_HWC_q7_fast, 768131
    CSV, riscv_relu_q7, 2582
    CSV, riscv_maxpool_q7_HWC, 9331
    CSV, riscv_fully_connected_q7_opt, 12163
    CSV, riscv_softmax_q7, 390
    label 0: 0, Plane, 0.00%
    label 1: 0, Car, 0.00%
    label 2: 1, Bird, 0.79%
    label 3: 33, Cat, 25.98%
    label 4: 16, Deer, 12.60%
    label 5: 67, Dog, 52.76%
    label 6: 0, Frog, 0.00%
    label 7: 8, Horse, 6.30%
    label 8: 0, Ship, 0.00%
    label 9: 0, Truck, 0.00%
    CSV, BENCH END, 8518029

4. Run with RISCV DSP disabled NMSIS-NN library

.. code-block::

    make DSP_ENABLE=OFF -B run
