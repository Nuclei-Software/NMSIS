.. _core_template_intexc_device:

Interrupt and Exception Handling File: intexc_<device>.S
========================================================
The **intexc File intexc_<device>.S** contains:
 - Macro to save caller register.
 - Macro to restore caller register.
 - Default Exception/NMI routine implementation.
 - Default Non-Vector Interrupt routine implementation.

Nuclei processors provide **NMI(Non-Maskable Interrupt)**, **Exception**,
**Vector Interrupt** and **Non-Vector Interrupt** features.

NMI(Non-Maskable Interrupt)
---------------------------

Click `NMI`_ to learn about Nuclei Processor Core NMI in Nuclei ISA Spec.

NMI is used for urgent external HW error. It can't be masked and disabled.

When NMI happened, bit 9 of CSR ``MMSIC_CTL`` will be checked.
If this bit value is 1, then NMI entry address will be the same as exception(CSR_MTVEC), and exception code for NMI will be 0xFFF, otherwise NMI entry will be same as reset_vector.

In NMSIS-Core, the bit 9 of CSR ``MMISC_CTL`` is set to 1 during core startup, so NMI will be treated as Exception and handled.


Exception
---------

Click `Exception`_ to learn about Nuclei Processor Core Exception in Nuclei ISA Spec.

For CPU exception, the entry for exception will be ``exc_entry``, in this entry code, it will call default exception handler :cpp:func:`core_exception_handler`.

In the common exception routine(``exc_entry``) to get more information like exception code.
Exception handle flow show as below picture:

.. _figure_template_intexc_1:

.. figure:: /asserts/images/NMSIS_exception.png
    :alt: Exception Handling Flow
    :width: 50%
    :align: center

    Exception Handling Flow

NMI and exception could support nesting. Two levels of NMI/Exception state save stacks are supported.

We support three nesting mode as below:

- NMI nesting exception
- Exception nesting exception
- Exception nesting NMI


For software, we have provided the common entry for NMI and exception. Silicon vendor only need adapt the interface defined in :ref:`core_api_intexc_nmi_handling`.

Context save and restore have been handled by ``exc_entry`` interface.

When exception exception return it will run the intruction which trigger the exception again. It will cause software dead loop. So in the exception handler for each exception code, we propose to set CSR ``MEPC`` to be ``MEPC+4``, then it will start from next instruction of MEPC.

Interrupt
---------

Click `Interrupt`_ to learn about Nuclei Processor Core Interrupt in Nuclei Spec.

Interrupt could be configured as **CLINT** mode or **ECILC** mode.

In NMSIS-Core, Interrupt has been configured as **ECLIC** mode during startup in *startup_<Devices>.S*, which is also recommended setting using Nuclei Processors.

ECLIC managed interrupt could configured as **vector** and **non-vector** mode.

Detail interrupt handling process as below picture:

.. _figure_template_intexc_2:

.. figure:: /asserts/images/NMSIS_intexc.png
    :alt: Interrupt Handling Flow
    :width: 80%
    :align: center

    Interrupt Handling Flow


To get highest priority interrupt we need compare the interrupt level first.If level is the same then compare the priority. High level interrupt could interrupt low level
ISR and trigger interrupt nesting. If different priority with same level interrupt pending higher priority will be served first. Interrupt could be configured as vector mode 
and non-vector mode by vendor. For non-vector mode interrupt handler entry get from MTVT2 and exception/NMI handler entry get from MTVEC. If Vendor need set non vector mode
interrupt handler entry from ``MTVVEC`` you need set ``MTVT2.BIT0`` as 0.


Non-Vector Interrupt SW handler
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For **non-vector** mode interrupt it will make the necessary CSR registers and context save and restore.
Non-vector mode software handle flow show as below pciture:

.. _figure_template_intexc_3:

.. figure:: /asserts/images/NMSIS_Interrupt_Nonvec_SW_flow.png
    :alt: Non-vector mode interrupt software handle flow
    :width: 50%
    :align: center

    Non-vector mode interrupt software handle flow

**Detail description for non-vector mode interrupt handler as below steps:**

1. Get non-vector mode handler entry from ``MTVT2`` if ``MTVT2.BIT0`` is 1(proposed configuration).

2. Context save to stack for cpu registers.

3. Save CSR registers ``MEPC/MCAUSE/MSUBM`` to stack.

4. Run instruction ``csrrw ra, CSR_JALMNXTI, ra``. It will enable interrupt, check interrupt pending. If interrupt is pending then get highest priority interrupt and jump to interrupt handler entry in the vector table, otherwise it will go to step 6.

5. Execute the interrupt handler routine, when return from isr routine it will jump to step 4.

6. Global interrupt disable.

7. Restore CSR registers ``MEPC/MCAUSE/MSUBM``.

8. Context restore from stack for cpu registers.

9. Execute ``mret`` to return from handler.

For **non-vector** mode iterrupt it could support **interrupt nesting**. 

**Interrupt nesting** handle flow show as below picture:

.. _figure_template_intexc_4:

.. figure:: /asserts/images/NMSIS_Interrupt_Nonvec_Nesting_SW_flow.png
    :alt: Nesting interrupt handling flow
    :width: 80%
    :align: center

    Nesting interrupt handling flow


Vector Interrupt SW handler
^^^^^^^^^^^^^^^^^^^^^^^^^^^

If vector interrupt handler need support nesting or making function call Vector mode software handling flow show as below picture:

.. _figure_template_intexc_5:

.. figure:: /asserts/images/NMSIS_Nesting_Vector_Interrupt_SW_flow.png
    :alt: Vector mode nesting interrupt handling flow
    :width: 20%
    :align: center

    Vector mode nesting interrupt handling flow


**Detail description for nested vector mode interrupt handler as below steps:**

1. Get vector mode handler from address of vector table entry ``MTVT`` added offset.

2. Context save to stack for cpu registers, done in each vector interrupt handler via :c:macro:`__INTERRUPT`

3. Save CSR registers ``MEPC/MCAUSE/MSUBM`` to stack, done in each vector interrupt handler by read and save these CSRs into variables.

4. Execute the interrupt handling.

5. Restore CSR registers ``MEPC/MCAUSE/MSUBM`` from stack.

6. CSR registers restore from saved variables used in step 3.

7. Execute ``mret`` to return from handler

Here is sample code for above nested vector interrupt handling process:

.. code-block:: c
    :linenos:

    // Vector interrupt handler for on-board button
    __INTERRUPT void SOC_BUTTON_1_HANDLER(void)
    {
        // save mepc,mcause,msubm enable interrupts
        SAVE_IRQ_CSR_CONTEXT();

        printf("%s", "----Begin button1 handler----Vector mode\r\n");

        // Green LED toggle
        gpio_toggle(GPIO, SOC_LED_GREEN_GPIO_MASK);

        // Clear the GPIO Pending interrupt by writing 1.
        gpio_clear_interrupt(GPIO, SOC_BUTTON_1_GPIO_OFS, GPIO_INT_RISE);

        wait_seconds(1); // Wait for a while

        printf("%s", "----End button1 handler\r\n");

        // disable interrupts,restore mepc,mcause,msubm
        RESTORE_IRQ_CSR_CONTEXT();
    }

**Detail description for non-nested vector mode interrupt handler as below**

To improve the software response latency for vector mode vendor could remove context save/restore and **MEPC/MCAUSE/MSUBM** save/restore.

If so vector mode interrupt will not support nesting and interrupt handler can only be a leaf function which doesn't make any function calls.


**Then the vector mode interrupt software flow will be described as below:**

1. Get vector mode handler from address of vector table entry ``MTVT`` added offset.

2. Execute the interrupt handler(leaf function).

3. Execute ``mret`` to return from handler

Here is sample code for above non-nested vector interrupt handler which is a leaf function handling process:

.. code-block:: c
    :linenos:

    static uint32_t btn_pressed = 0;
    // Vector interrupt handler for on-board button
    // This function is an leaf function, no function call is allowed
    __INTERRUPT void SOC_BUTTON_1_HANDLER(void)
    {
        btn_pressed ++;
    }


intexc_Device.S Template File
-----------------------------

The file exists for each supported toolchain and is the only toolchain specific NMSIS file.

Normally this file needn't adapt for different device. If CPU CSR registers have done some
changes you may need some adaption.

Here we provided ``intexc_Device.S`` template file as below:

.. literalinclude:: ../../../../Device/_Template_Vendor/Vendor/Device/Source/GCC/intexc_Device.S
    :language: c
    :linenos:


.. _NMI: https://doc.nucleisys.com/nuclei_spec/isa/nmi.html
.. _Exception: https://doc.nucleisys.com/nuclei_spec/isa/exception.html
.. _Interrupt: https://doc.nucleisys.com/nuclei_spec/isa/interrupt.html
