.. _core_template_system_device:

System Configuration Files system_<device>.c and system_<device>.h
==================================================================

The **System Configuration Files system_<device>.c and system_<device>.h**
provides as a minimum the functions described under :ref:`core_api_system_device`.

These functions are device specific and need adaptations. In addition, the file might have
configuration settings for the device such as XTAL frequency or PLL prescaler settings,
necessary system initialization, vendor customized interrupt, exception and nmi handling code,
refer to :ref:`core_api_system_device` for more details.

For devices with external memory BUS the ``system_<device>.c`` also configures the BUS system.

The silicon vendor might expose other functions (i.e. for power configuration) in the ``system_<device>.c`` file.
In case of additional features the function prototypes need to be added to the ``system_<device>.h`` header file.

system_Device.c Template File
-----------------------------

Here we provided ``system_Device.c`` template file as below:

.. literalinclude:: ../../../../Device/_Template_Vendor/Vendor/Device/Source/system_Device.c
    :language: c
    :linenos:

system_Device.h Template File
-----------------------------

Here we provided ``system_Device.h`` template file as below:

.. literalinclude:: ../../../../Device/_Template_Vendor/Vendor/Device/Include/system_Device.h
    :language: c
    :linenos:
