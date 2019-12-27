# Explanation how to use the Device folder and template files

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
                  +-- GCC                                 Nuclei RISC-V GNU toolchain
                       +- startup_<Device>.s              ASM startup file for GNU GCC
                       +- intexc_<Device>.s               Interrupt and exception handling file for GNU GCC
                       +- gcc_<Device>.ld                 Linker script template for GNU GCC
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


**Note:**
> Template files (i.e. startup_Device.s, system_Device.c) are application
> specific and therefore expected to be copied into the application project
> folder prior to use!

