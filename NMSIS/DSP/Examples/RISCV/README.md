# Run DSP Examples in Spike
Here we will describe how to run the nmsis dsp examples in Nuclei Spike.

## Preparation
* Nuclei Modified Spike
* Nuclei Modified PK
* Nuclei RISCV GNU Toolchain

## Tool Setup

1. Export **PATH** correctly for `spike` and `riscv-nuclei-elf-gcc`
~~~
export PATH=/path/to/spike/bin:/path/to/riscv-nuclei-elf-gcc/bin/:$PATH
~~~

2. Export **PK** correctly for `spike` commond.
~~~
export PK=/path/to/spike/riscv32-unknown-elf/bin/pk
~~~

## Build NMSIS DSP Library

Follow the NMSIS DSP documentation to build and install DSP library.

## How to run
1. Let us take `./riscv_class_marks_example/` for example

2. `cd ./riscv_class_marks_example/`

3. Run with RISCV DSP enabled NMSIS-DSP library
~~~
make DSP_ENABLE=ON -B run
~~~

4. Run with RISCV DSP disabled NMSIS-DSP library
~~~
make DSP_ENABLE=OFF -B run
~~~
