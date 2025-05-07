# Explanation how to use the Device folder and template files

> [!CAUTION]
> These device template files will be deprecated, please directly refer to `evalsoc` implementation
> in Nuclei SDK.

If you want to get a up to date and working example for NMSIS Device Template, you can refer to
[Nuclei SDK](https://github.com/Nuclei-Software/nuclei-sdk) `SoC/evalsoc` folder, this folder is
using NMSIS Device Template as base and modified to match Nuclei Evaluation SoC for Nuclei RISC-V
Core Evaluation.

> Device/Nuclei template is removed since NMSIS 1.2.0 release.

1. **Following directory structure and template files are given:**

~~~
  - <Vendor>
      |
      +-- <Device>
            |
            +-- Include
            |     +- <Device>.h                           header file
            |     +- system_Device.h                      system include file
            +-- Source
                  |
                  +- system_<Device>.c                    system source file
                  |
                  +-- GCC                                 Nuclei RISC-V toolchain
                  |    +- startup_<Device>.S              ASM startup file for GNU GCC/LLVM Clang
                  |    +- intexc_<Device>.S               Interrupt and exception handling file for GNU GCC/LLVM Clang
                  |    +- intexc_<Device>_s.S             S-Mode Interrupt and exception handling file for GNU GCC/LLVM Clang
                  |    +- gcc_<Device>.ld                 Linker script template for GNU GCC/LLVM Clang
                  +-- IAR                                 IAR Compiler
                       +- startup_<Device>.S              ASM startup file for IAR
                       +- intexc_<Device>.S               Interrupt and exception handling file for IAR
                       +- intexc_<Device>_s.S             S-Mode Interrupt and exception handling file for IAR
                       +- gcc_<Device>.ld                 Linker script template for IAR
~~~

2. **Copy the complete folder including files and replace:**
- folder name 'Vendor' with the abbreviation for the device vendor  e.g.: GD.
- folder name 'Device' with your specific device name e.g.: GD32VF103.
- in the filenames 'Device' with your specific device name e.g.: GD32VF103.

3. The template files contain comments starting with `TODO:`.

There it is described what you need to do.

The template files contain following placeholder:

~~~
  <Device>
  <Device> should be replaced with your specific device name.
   e.g.: GD32VF103

  <DeviceInterrupt>
  <DeviceInterrupt> should be replaced with a specific device interrupt name.
  e.g.: TIM1 for Timer#1 interrupt.

  <DeviceAbbreviation>
  <DeviceAbbreviation> should be replaced with a dedicated device family
  abbreviation (e.g.: GD for GD32VF103 device family)

  __NUCLEI_N#_REV
  __NUCLEI_N#_REV can be replaced with __NUCLEI_N_REV or __NUCLEI_NX_REV
  e.g.: __NUCLEI_N_REV for a Nuclei N class core based device
~~~


> [!NOTE]
> Template files (i.e. startup_Device.s, system_Device.c) are application
> specific and therefore expected to be copied into the application project
> folder prior to use!

