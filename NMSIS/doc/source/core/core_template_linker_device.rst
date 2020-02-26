.. _core_template_linker_script:

Device Linker Script: gcc_<device>.ld
=====================================
The Linker Script File **gcc_<device>.ld** contains:
 - Memory base address and size.
 - Code, data section, vector table etc. location.
 - Stack & heap location and size.

The file exists for each supported toolchain and is the only toolchain specific NMSIS file.

To adapt the file to a new device only when you need change the memory base address, size, data and code location etc.

gcc_Device.ld Template File
---------------------------

Here we provided ``gcc_Device.ld`` template file as below:

.. literalinclude:: ../../../../Device/_Template_Vendor/Vendor/Device/Source/GCC/gcc_Device.ld
    :language: c
    :linenos:
