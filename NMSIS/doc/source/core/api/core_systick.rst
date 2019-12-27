.. _core_api_systick:

Systick Timer(SysTimer)
=======================

SysTimer API
------------

.. doxygengroup:: NMSIS_Core_SysTimer
   :project: nmsis_core
   :outline:
   :content-only:

.. doxygengroup:: NMSIS_Core_SysTimer
   :project: nmsis_core


.. _core_systick_code_example:

SysTick Code Example
--------------------

The code below shows the usage of the function :cpp:func:`SysTick_Config` with an GD32VF103.

.. code-block:: c
    :linenos:
    :caption: gd32vf103_systick_example.c

    #include "gd32vf103.h"

    volatile uint32_t msTicks = 0;                              /* Variable to store millisecond ticks */

    void SysTick_Handler(void) {                                /* SysTick interrupt Handler. */
      SysTimer_SetLoadValue(0);                                 /* Set 0 to Timer counter to reload timer. */
      msTicks++;                                                /* See startup file startup_GD32VF103.s for SysTick vector */
    }

    int main (void) {
      uint32_t returnCode;

      returnCode = SysTick_Config(SystemCoreClock / 1000);      /* Configure SysTick to generate an interrupt every millisecond */

      if (returnCode != 0) {                                    /* Check return code for errors */
        // Error Handling
      }

      while(1);
    }

.. _core_systimer_interrupt_code_example:

SysTimer Interrupt Code Example
-------------------------------

The code below shows the usage of various NMSIS Timer Interrupt functions with an GD32VF103 device.


.. code-block:: c
    :linenos:
    :caption: gd32vf103_timer_example1.c

    #include "gd32vf103.h"

    void eclic_mtip_handler(void)
    {
        uint64_t now =  SysTimer_GetLoadValue();
        SysTimer_SetCompareValue(now + TIMER_FREQ/100);
    }

    static uint32_t int_cnt = 0;
    void eclic_msip_handler(void)
    {
        SysTimer_ClearSWIRQ();
        int_cnt++;
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

    void setup_timer(void)
    {
        SysTimer_SetLoadValue(0);
        SysTimer_SetCompareValue(TIMER_FREQ/100);
    }

    int main (void)
    {
        uint32_t returnCode;

        eclic_global_initialize();                                /* initialize ECLIC */

        setup_timer();                                            /* initialize timer */

        returnCode = eclic_register_interrupt(SysTimer_IRQn,1,2,8,0,eclic_mtip_handler);  /* register system timer interrupt */

        returnCode = eclic_register_interrupt(SysTimerSW_IRQn,1,2,8,0,eclic_msip_handler);  /* register system timer SW interrupt */

        __enable_irq();                                           /* enable global interrupt */

        SysTimer_SetSWIRQ();                                      /* trigger timer SW interrupt */

        if (returnCode != 0) {                                    /* Check return code for errors */
          // Error Handling
        }

        while(1);
    }

