# Run NN Examples in Spike
Here we will describe how to run the nmsis nn examples in Nuclei Spike.

## Preparation
* Nuclei Modified Spike - `xl_spike`
* Nuclei SDK modified for `xl_spike` branch `dev_xlspike`
* Nuclei RISCV GNU Toolchain
* CMake >= 3.5

## Tool Setup

1. Export **PATH** correctly for `xl_spike` and `riscv-nuclei-elf-gcc`

    ~~~
    export PATH=/path/to/xl_spike/bin:/path/to/riscv-nuclei-elf-gcc/bin/:$PATH
    ~~~

## Build NMSIS NN Library

1. cd to `<NMSIS_ROOT>/NMSIS`
2. Build NMSIS DSP library using `make PARELLEL=1 gen_nn_lib`,
   the libraries will be generated into `<NMSIS_ROOT>/NMSIS/Library/NN/GCC`
3. Strip debug informations using `make strip_nn_lib` to make the generated
   library smaller.

## How to run

1. Set environment variables `NUCLEI_SDK_ROOT` and `NUCLEI_SDK_NMSIS`,
   and set Nuclei SDK SoC to `xlspike`

    ~~~shell
    export NUCLEI_SDK_ROOT=/path/to/nuclei_sdk
    export NUCLEI_SDK_NMSIS=/path/to/NMSIS/NMSIS
    export SOC=xlspike
    ~~~

2. Let us take `./cifar10/` for example

3. `cd ./cifar10/`

4. Run with RISCV DSP enabled NMSIS-NN library for CORE `n307`

    ~~~shell
    # Clean project
    make DSP_ENABLE=ON CORE=n307 clean
    # Build project
    make DSP_ENABLE=ON CORE=n307 all
    # Run application using xl_spike
    make DSP_ENABLE=ON CORE=n307 run
    ~~~

5. Run with RISCV DSP disabled NMSIS-NN library for CORE `n307`

    ~~~shell
    make DSP_ENABLE=OFF CORE=n307 clean
    make DSP_ENABLE=OFF CORE=n307 all
    make DSP_ENABLE=OFF CORE=n307 run
    ~~~

