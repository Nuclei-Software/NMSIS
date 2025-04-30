.. _core_template_linker_script:

Device Linker Script: gcc_<device>.ld
=====================================

.. caution::

    **Please be informed** that the **NMSIS-Core Device Templates** may not be updated in a timely manner and thus could become outdated.
    We suggest referring to the specific implementation of `evalsoc in the Nuclei SDK`_ for the latest reference template.
    This reference template may not be actively maintained in the future.

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

.. _evalsoc in the Nuclei SDK: https://github.com/Nuclei-Software/nuclei-sdk/tree/master/SoC/evalsoc/Common
