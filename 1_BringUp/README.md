Tiva C TM4C123G LaunchPad Bringup
=================================

This bringup project shows you how to start the board as **pre-RTOS** in **C++**

**Feature**

- _SystemControl_ sets up the system clock and _SysTick_ interrupt which acts as your system heartbeat
- _InterruptHandler_ provides interruption handler
- _LedIndicator_ blinks the on-board LED 
- _Logger_ configs the UART0 port which connects to PC via virtual COM port, and provides _printf_ function
- _Button_ detects _click_, _doubleClick_, _hold_, _release_ events of an input button
- _UserInput_ shows you how to add and handle on-board SW events


**System Structure**

This bringup project use _SysTick_ interrupt as the main task which periodically check buttons states and drives LED.
It also be used to refresh LCD, read inputs, send data. You can add conditions before starting any action in SysTick routine.

In case you set SysTick to a higher frequency (increase _SYSTICK_PER_SECOND_), you should move "slow tasks" to an different timer routine or main loop. This project just shows you how to schedule a task using a simple timer.


**Hardware**

- [Texas Instruments Tiva C Series TM4C123G LaunchPad](http://www.ti.com/tool/ek-tm4c123gxl)
- USB Cable, of course


**Software**

- Install [TI's Code Composer Studio](http://www.ti.com/tool/ccstudio)
- Install [TivaWare for C Series](http://www.ti.com/tool/sw-tm4c)
- Edit file [vars.ini](/vars.ini) by changing the path to your installed TivaWare folder
- Import this *1_BringUp* project into CCS
- You can flash to the board via CCS or TI's [LM Flash Programmer](http://www.ti.com/tool/lmflashprogrammer)

**Licensing**

The MIT License applies to this software and its supporting documentation:
