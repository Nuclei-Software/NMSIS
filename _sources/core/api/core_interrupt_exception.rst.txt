.. _core_api_interrupt_exception:

Interrupts and Exceptions
=========================

Description
-----------

This section explains how to use interrupts and exceptions and access functions for the
`Enhanced Core Local Interrupt Controller(ECLIC)`_.

Nuclei provides a template file startup_device for each supported compiler. 
The file must be adapted by the silicon vendor to include
interrupt vectors for all device-specific interrupt handlers.
Each interrupt handler is defined as a weak function to an dummy handler.
These interrupt handlers can be used directly in application software without being adapted by the programmer.

Click `Interrupt`_ to learn more about interrupt handling in Nuclei processor core.

NMI Interrupt
-------------

`NMI interrupt`_ entry is stored by **CSR_MNVEC**.
If CSR_MMSIC[9] is 1 then NMI entry is the same as **Exception** which get from CSR_MTVEC.
If CSR_MMSIC[9] is 1 NMI entry is reset vector.

Exception
---------

`Exception`_ has only 1 entry address which stored by CSR_MTVEC. All the exceptions will jump to the same entry ``exc_entry`` defined in ``intexc_<Device>.S``.

The table below lists the core exception code of the Nuclei N/NX processors.

.. _table_api_core_intexc_1:

.. table:: Core exception code of the Nuclei N/NX processors

   +---------------------+-------+---------------------------------------+
   | Exception Code      | Value | Description                           |
   +---------------------+-------+---------------------------------------+
   | InsUnalign_EXCn     | 0     | Instruction address misaligned        |
   +---------------------+-------+---------------------------------------+
   | InsAccFault_EXCn    | 1     | Instruction access fault              |
   +---------------------+-------+---------------------------------------+
   | IlleIns_EXCn        | 2     | Illegal instruction                   |
   +---------------------+-------+---------------------------------------+
   | Break_EXCn          | 3     | Beakpoint                             |
   +---------------------+-------+---------------------------------------+
   | LdAddrUnalign_EXCn  | 4     | Load address misaligned               |
   +---------------------+-------+---------------------------------------+
   | LdFault_EXCn        | 5     | Load access fault                     |
   +---------------------+-------+---------------------------------------+
   | StAddrUnalign_EXCn  | 6     | Store or AMO address misaligned       |
   +---------------------+-------+---------------------------------------+
   | StAccessFault_EXCn  | 7     | Store or AMO access fault             |
   +---------------------+-------+---------------------------------------+
   | UmodeEcall_EXCn     | 8     | Environment call from User mode       |
   +---------------------+-------+---------------------------------------+
   | SmodeEcall_EXCn     | 9     | Environment call from Supervisor Mode |
   +---------------------+-------+---------------------------------------+
   | MmodeEcall_EXCn     | 11    | Environment call from Machine mode    |
   +---------------------+-------+---------------------------------------+
   | InsPageFault_EXCn   | 12    | Instruction page fault                |
   +---------------------+-------+---------------------------------------+
   | LdPageFault_EXCn    | 13    | Load page fault                       |
   +---------------------+-------+---------------------------------------+
   | StPageFault_EXCn    | 15    | Store or AMO page fault               |
   +---------------------+-------+---------------------------------------+
   | StackOverflow_EXCn  | 24    | Stack overflow fault                  |
   +---------------------+-------+---------------------------------------+
   | StackUnderflow_EXCn | 25    | Stack overflow fault                  |
   +---------------------+-------+---------------------------------------+
   | NMI_EXCn            | 0xfff | NMI interrupt                         |
   +---------------------+-------+---------------------------------------+

Vector Table
------------

The Vector Table defines the entry addresses of the ECLIC managed interrupts.

It is typically located at the beginning of the program memory,
and you can modify CSR MTVT to reallocate the base address of this vector table,
but you need to take care of the base address alignment according to the number of interrupts.

.. _table_api_core_intexc_2:

.. table:: base address alignment according to the number of interrupts

   +---------------------+------------------------------------+
   | Number of Interrupt | Alignment Requirements of CSR MTVT |
   +---------------------+------------------------------------+
   | 0 to 16             | 64-byte                            |
   +---------------------+------------------------------------+
   | 17 to 32            | 128-byte                           |
   +---------------------+------------------------------------+
   | 33 to 64            | 256-byte                           |
   +---------------------+------------------------------------+
   | 65 to 128           | 512-byte                           |
   +---------------------+------------------------------------+
   | 129 to 256          | 1KB                                |
   +---------------------+------------------------------------+
   | 257 to 512          | 2KB                                |
   +---------------------+------------------------------------+
   | 513 to 1024         | 4KB                                |
   +---------------------+------------------------------------+

Interrupt number 0~18 is reserved by Nuclei Core.
19~1023 could be used by Silicon Vendor Device.

Below is an example interrupt allocated table:

.. code-block:: c
    :linenos:

    typedef enum IRQn {
        /******  Nuclei N/NX Processor Core Internal Interrupt Numbers *************************/
        Reserved0_IRQn               =   0,     /*!<  Internal reserved                        */
        Reserved1_IRQn               =   1,     /*!<  Internal reserved                        */
        Reserved2_IRQn               =   2,     /*!<  Internal reserved                        */
        SysTimerSW_IRQn              =   3,     /*!<  System Timer SW interrupt                */
        Reserved3_IRQn               =   4,     /*!<  Internal reserved                        */
        Reserved4_IRQn               =   5,     /*!<  Internal reserved                        */
        Reserved5_IRQn               =   6,     /*!<  Internal reserved                        */
        SysTimer_IRQn                =   7,     /*!<  System Timer Interrupt                   */
        Reserved6_IRQn               =   8,     /*!<  Internal reserved                        */
        Reserved7_IRQn               =   9,     /*!<  Internal reserved                        */
        Reserved8_IRQn               =  10,     /*!<  Internal reserved                        */
        Reserved9_IRQn               =  11,     /*!<  Internal reserved                        */
        Reserved10_IRQn              =  12,     /*!<  Internal reserved                        */
        Reserved11_IRQn              =  13,     /*!<  Internal reserved                        */
        Reserved12_IRQn              =  14,     /*!<  Internal reserved                        */
        Reserved13_IRQn              =  15,     /*!<  Internal reserved                        */
        Reserved14_IRQn              =  16,     /*!<  Internal reserved                        */
        HardFault_IRQn               =  17,     /*!<  Hard Fault, storage access error         */
        Reserved15_IRQn              =  18,     /*!<  Internal reserved                        */

        /******  GD32VF103 Specific External Interrupt Numbers *********************************/
        WWDGT_IRQn                   =  19,     /*!< window watchDog timer interrupt           */
        LVD_IRQn                     =  20,     /*!< LVD through EXTI line detect interrupt    */
        TAMPER_IRQn                  =  21,     /*!< tamper through EXTI line detect           */
                     :       :
                     :       :
        CAN1_EWMC_IRQn               =  85,     /*!< CAN1 EWMC interrupt                       */
        USBFS_IRQn                   =  86,     /*!< USBFS global interrupt                    */
        SOC_INT_MAX,                            /*!< Number of total Interrupts                */
    } IRQn_Type;

ECLIC API Definitions
---------------------

When macro ``NMSIS_ECLIC_VIRTUAL`` is defined, the ECLIC access functions in the table below must be implemented for virtualizing ECLIC access.

These functions should be implemented in a separate source module.
The original NMSIS-Core ``__ECLIC_xxx`` functions are always available independent of ``NMSIS_ECLIC_VIRTUAL`` macro.

.. _table_api_core_intexc_3:

.. table:: ECLIC Access Functions

   +----------------------------------+-------------------------------------+
   | ECLIC ACCESS FUNCTIONS           | NMSIS-CORE FUNCTIONS FOR ECLIC      |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_SetCfgNlbits`    | :cpp:func:`__ECLIC_SetCfgNlbits`    |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetCfgNlbits`    | :cpp:func:`__ECLIC_GetCfgNlbits`    |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetInfoVer`      | :cpp:func:`__ECLIC_GetInfoVer`      |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetInfoCtlbits`  | :cpp:func:`__ECLIC_GetInfoCtlbits`  |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetInfoNum`      | :cpp:func:`__ECLIC_GetInfoNum`      |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_SetMth`          | :cpp:func:`__ECLIC_SetMth`          |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetMth`          | :cpp:func:`__ECLIC_GetMth`          |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_EnableIRQ`       | :cpp:func:`__ECLIC_EnableIRQ`       |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetEnableIRQ`    | :cpp:func:`__ECLIC_GetEnableIRQ`    |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_DisableIRQ`      | :cpp:func:`__ECLIC_DisableIRQ`      |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_SetPendingIRQ`   | :cpp:func:`__ECLIC_SetPendingIRQ`   |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetPendingIRQ`   | :cpp:func:`__ECLIC_GetPendingIRQ`   |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_ClearPendingIRQ` | :cpp:func:`__ECLIC_ClearPendingIRQ` |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_SetTrigIRQ`      | :cpp:func:`__ECLIC_SetTrigIRQ`      |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetTrigIRQ`      | :cpp:func:`__ECLIC_GetTrigIRQ`      |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_SetShvIRQ`       | :cpp:func:`__ECLIC_SetShvIRQ`       |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetShvIRQ`       | :cpp:func:`__ECLIC_GetShvIRQ`       |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_SetCtrlIRQ`      | :cpp:func:`__ECLIC_SetCtrlIRQ`      |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetCtrlIRQ`      | :cpp:func:`__ECLIC_GetCtrlIRQ`      |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_SetLevelIRQ`     | :cpp:func:`__ECLIC_SetLevelIRQ`     |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetLevelIRQ`     | :cpp:func:`__ECLIC_GetLevelIRQ`     |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_SetPriorityIRQ`  | :cpp:func:`__ECLIC_SetPriorityIRQ`  |
   +----------------------------------+-------------------------------------+
   | :c:macro:`ECLIC_GetPriorityIRQ`  | :cpp:func:`__ECLIC_GetPriorityIRQ`  |
   +----------------------------------+-------------------------------------+

When ``NMSIS_VECTAB_VIRTUAL`` macro is defined, the functions in the table below must be replaced to virtualize the API access functions to the interrupt vector table.

The ECLIC vector table API should be implemented in a separate source module.

This allows, for example, alternate implementations to relocate the vector table from flash to RAM on the first vector table update.

The original NMSIS-Core functions are always available, but prefixed with ``__ECLIC``.

.. _table_api_core_intexc_4:

.. table:: ECLIC Vector Access Functions

   +-----------------------------+--------------------------------+
   | ECLIC Vector Table Access   | NMSIS-CORE FUNCTIONS           |
   +-----------------------------+--------------------------------+
   | :c:macro:`ECLIC_SetVector`  | :cpp:func:`__ECLIC_SetVector`  |
   +-----------------------------+--------------------------------+
   | :c:macro:`ECLIC_GetVector`  | :cpp:func:`__ECLIC_GetVector`  |
   +-----------------------------+--------------------------------+

ECLIC Function Usage
--------------------

The code below shows the usage of various NMSIS ECLIC flow with an GD32VF103 device.

.. code-block:: c
    :linenos:
    :caption: gd32vf103_interrupt_example1.c

    #include "gd32vf103.h"
    
    // Vector interrupt which could be nested
    __INTERRUPT void eclic_button1_handler(void)
    {
        SAVE_IRQ_CSR_CONTEXT();                                           /* save mepc,mcause,msubm enable interrupts */

        GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << GREEN_LED_GPIO_OFFSET);        /* Green LED On */
        GPIO_REG(GPIO_RISE_IP) = (0x1 << BUTTON_1_GPIO_OFFSET);           /* Clear the GPIO Pending interrupt by writing 1. */

        RESTORE_IRQ_CSR_CONTEXT();                                        /* disable interrupts,restore mepc,mcause,msubm */
    }
    
    // Non-vector interrupt
    void eclic_button2_handler(void)
    {
        GPIO_REG(GPIO_OUTPUT_VAL) |= (1 << GREEN_LED_GPIO_OFFSET);        /* Green LED On */
        GPIO_REG(GPIO_RISE_IP) = (0x1 << BUTTON_2_GPIO_OFFSET);           /* Clear the GPIO Pending interrupt by writing 1. */
    }

    void eclic_global_initialize(void)
    {
        ECLIC_SetMth(0);
        ECLIC_SetCfgNlbits(3);
    }

    int eclic_register_interrupt(IRQn_Type IRQn, uint8_t shv, uint32_t trig_mode, uint32 lvl, uint32_t priority, void * handler)
    {
        ECLIC_SetShvIRQ(IRQn, shv);
        ECLIC_SetTrigIRQ(IRQn, trig_mode);
        ECLIC_SetLevelIRQ(IRQn, lvl);
        ECLIC_SetPriorityIRQ(IRQn, priority);
        ECLIC_SetVector(IRQn, (rv_csr_t)(handler));
        ECLIC_EnableIRQ(IRQn);
        return 0;
    }

    int main (void)
    {
        uint32_t returnCode;

        eclic_global_initialize();                                /* initialize ECLIC */

        GPIO_init();                                              /* initialize GPIO */

        returnCode = eclic_register_interrupt(BTN1_IRQn,1,2,1,0,Button1_IRQHandler);  /* register system button1 interrupt */
        returnCode = eclic_register_interrupt(BTN2_IRQn,0,2,2,0,Button2_IRQHandler);  /* register system button2 interrupt */

        __enable_irq();                                           /* enable global interrupt */

        if (returnCode != 0)  {                                   /* Check return code for errors */
          // Error Handling
        }

        while(1);
    }


Interrupt and Exception API
---------------------------

.. doxygengroup:: NMSIS_Core_IntExc
   :project: nmsis_core
   :outline:
   :content-only:

.. doxygengroup:: NMSIS_Core_IntExc
   :project: nmsis_core

.. _Enhanced Core Local Interrupt Controller(ECLIC): https://doc.nucleisys.com/nuclei_spec/isa/eclic.html
.. _NMI interrupt: https://doc.nucleisys.com/nuclei_spec/isa/nmi.html
.. _Exception: https://doc.nucleisys.com/nuclei_spec/isa/exception.html
.. _Interrupt: https://doc.nucleisys.com/nuclei_spec/isa/interrupt.html
