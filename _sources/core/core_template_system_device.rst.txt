.. _core_template_system_device:

System Configuration Files system_<Device>.c and system_<Device>.h
==================================================================

.. caution::

    **Please be informed** that the **NMSIS-Core Device Templates** may not be updated in a timely manner and thus could become outdated.
    We suggest referring to the specific implementation of `evalsoc in the Nuclei SDK`_ for the latest reference template.
    This reference template may not be actively maintained in the future.

The **System Configuration Files system_<device>.c and system_<device>.h**
provides as a minimum the functions described under :ref:`core_api_system_device`.

These functions are device specific and need adaptations. In addition, the file might have
configuration settings for the device such as XTAL frequency or PLL prescaler settings,
necessary system initialization, vendor customized interrupt, exception and nmi handling code,
refer to :ref:`core_api_system_device` for more details.

For devices with external memory BUS the ``system_<Device>.c`` also configures the BUS system.

The silicon vendor might expose other functions (i.e. for power configuration) in the ``system_<Device>.c`` file.
In case of additional features the function prototypes need to be added to the ``system_<Device>.h`` header file.

system_Device.c Template File
-----------------------------

Here we provided ``system_Device.c`` template file as below:

Please check file content in https://github.com/Nuclei-Software/NMSIS/blob/master/Device/_Template_Vendor/Vendor/Device/Source/system_Device.c

system_Device.h Template File
-----------------------------

Here we provided ``system_Device.h`` template file as below:

.. literalinclude:: ../../../../Device/_Template_Vendor/Vendor/Device/Include/system_Device.h
    :language: c
    :linenos:

.. _evalsoc in the Nuclei SDK: https://github.com/Nuclei-Software/nuclei-sdk/tree/master/SoC/evalsoc/Common
