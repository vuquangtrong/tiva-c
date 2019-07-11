Tiva C TM4C123G LaunchPad FW Update via USB DFU
===============================================

If you want to update your device firmware, you just need to do below steps:
- hook an update request by checking a special case
- disable all interrupts
- setup USB port and PLL
- call to `ROM_UpdateUSB()`
- using LM Flash Programmer and choose USB DFU device to flash

**Hardware**

- [Texas Instruments Tiva C Series TM4C123G LaunchPad](http://www.ti.com/tool/ek-tm4c123gxl)
- USB Cable, of course


**Software**

- Install [TI's Code Composer Studio](http://www.ti.com/tool/ccstudio)
- Install [TivaWare for C Series](http://www.ti.com/tool/sw-tm4c)
- Include your Tivaware folder into project, you can use the macro **TI_TIVAWARE_ROOT** in _vars.ini_
- Change the heap size as you want
- To use _ROM_function()_ you must define part number of this kit __TARGET_IS_TM4C123_RB1__
- You can flash to the board via CCS or TI's [LM Flash Programmer](http://www.ti.com/tool/lmflashprogrammer)

**Licensing**

The MIT License applies to this software and its supporting documentation:
