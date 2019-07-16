Tiva C TM4C123G LaunchPad Audio and DMA
=======================================

This project shows you how to use uDMA and DAC to make sound
- MCP4921 is connected to SSI1 on TM4C123, running at 44.1kHz x 16b
- uDMA transfer data from audio buffer to SSI1, in ping-pong mode
- You can play a piece of audio, please see AUDIO_TEST
- You can generate wave sound with Audio Generator

**Hardware**

- [Texas Instruments Tiva C Series TM4C123G LaunchPad](http://www.ti.com/tool/ek-tm4c123gxl)
- MCP4921 or and any DAC with SPI protocol
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
