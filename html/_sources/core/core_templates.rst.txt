.. _core_device_templates:

NMSIS-Core Device Templates
===========================

Introduction
------------

Nuclei supplies NMSIS-Core device template files for the all supported |nuclei_core| and various compiler vendors.
Refer to the list of :ref:`supported toolchain<core_toolchain_support>` for compliance.

These NMSIS-Core device template files include the following:
 - Register names of the Core Peripherals and names of the Core Exception/Interrupt Vectors.
 - Functions to access core peripherals, special CPU instructions and SIMD instructions
 - Generic startup code and system configuration code.

The detailed file structure of the NMSIS-Core device templates is shown in the following picture.

.. image:: /asserts/images/NMSIS_CORE_Templates.png
    :alt: NMSIS-Core Device Templates
    :scale: 50%

NMSIS-Core Processor Files
--------------------------

The NMSIS-Core processor files provided by Nuclei are in the directory *NMSIS/Core/Include*.

These header files define all processor specific attributes do not need any modifications.

The *nmsis_core.h* defines the core peripherals and provides helper functions that access the core registers.

Device Examples
---------------

The NMSIS Software Pack defines several devices that are based on the Nuclei N/NX processors.

The device related NMSIS-Core files are in the directory *Device/Nuclei* and include
NMSIS-Core processor file explained before.

The following sample devices are defined as below:

+--------------+-----------------+------------------------------+
| **Family**   | **Device**      | **Description**              |
+--------------+-----------------+------------------------------+
| Nuclei N     | NUCLEI N Class  | Nuclei N Class based device  |
+--------------+-----------------+------------------------------+
| Nuclei NX    | NUCLEI NX Class | Nuclei NX Class based device |
+--------------+-----------------+------------------------------+

Template Files
--------------

To simplify the creation of NMSIS-Core device files, the following template files are provided
that should be extended by the silicon vendor to reflect the actual device and device peripherals.

Silicon vendors add to these template files the following information:
 - **Device Peripheral Access Layer** that provides definitions for device-specific peripherals.
 - **Access Functions for Peripherals** (optional) that provides additional helper functions to access device-specific peripherals.
 - **Interrupt vectors** in the startup file that are device specific.

+-------------------------------------------+----------------------------------------------------------------------------+
| **Template File**                         | **Description**                                                            |
|                                           |                                                                            |
| (Under ./Device/_Template_Vendor/Vendor/) |                                                                            |
+-------------------------------------------+----------------------------------------------------------------------------+
| Device/Source/GCC/startup_Device.S        | Startup file template for GNU GCC RISC-V Embedded Compiler.                |
+-------------------------------------------+----------------------------------------------------------------------------+
| Device/Source/GCC/gcc_Device.ld           | Link Script file template for GNU GCC RISC-V Embedded Compiler.            |
+-------------------------------------------+----------------------------------------------------------------------------+
| Device/Source/GCC/intexc_Device.S         | Exception and Interrupt handling file template                             |
|                                           |                                                                            |
|                                           | for GNU GCC RISC-V Embedded Compiler.                                      |
+-------------------------------------------+----------------------------------------------------------------------------+
| Device/Source/system_Device.c             | Generic system_Device.c file for system configuration                      |
|                                           |                                                                            |
|                                           | (i.e. processor clock and memory bus system).                              |
+-------------------------------------------+----------------------------------------------------------------------------+
| Device/Include/Device.h                   | Generic device header file.                                                |
|                                           |                                                                            |
|                                           | Needs to be extended with the device-specific peripheral registers.        |
|                                           |                                                                            |
|                                           | Optionally functions that access the peripherals can be part of that file. |
+-------------------------------------------+----------------------------------------------------------------------------+
| Device/Include/system_Device.h            | Generic system device configuration include file.                          |
+-------------------------------------------+----------------------------------------------------------------------------+

.. Note::
    The template files for silicon vendors are placed under *./Device/_Template_Vendor/Vendor/*.

    Please goto that folder to find the file list in the above table.

Adapt Template Files to a Device
--------------------------------

The following steps describe how to adopt the template files to a specific device or device family.

Copy the complete all files in the template directory and replace:
  - directory name ``Vendor`` with the abbreviation for the device vendor  e.g.: **GD**.
  - directory name ``Device`` with the specific device name e.g.: **GD32VF103**.
  - in the file names ``Device`` with the specific device name e.g.: **GD32VF103**.

Each template file contains comments that start with **TODO**: that describe a required modification.

The template files contain place holders:

+----------------------+-----------------------------------------------------------------+
| **Placeholder**      | **Replaced with**                                               |
+----------------------+-----------------------------------------------------------------+
| <Device>             | the specific device name or device family name; i.e. GD32VF103. |
+----------------------+-----------------------------------------------------------------+
| <DeviceInterrupt>    | a specific interrupt name of the device; i.e. TIM1 for Timer 1. |
+----------------------+-----------------------------------------------------------------+
| <DeviceAbbreviation> | short name or abbreviation of the device family; i.e. GD32VF.   |
+----------------------+-----------------------------------------------------------------+
| Nuclei-N#            | the specific Nuclei Class name; i.e. Nuclei N or Nuclei NX.     |
+----------------------+-----------------------------------------------------------------+

Device Templates Explaination
-----------------------------
The device configuration of the template files is described in detail on the following pages:

.. toctree::
    :maxdepth: 2

    core_template_startup_device.rst
    core_template_intexc.rst
    core_template_linker_device.rst
    core_template_system_device.rst
    core_template_device_header.rst

