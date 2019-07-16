Tiva C TM4C123G LaunchPad
=========================

This is my practice on an ARM Cortex-M4F processor ([Tiva LaunchPad TM4C123G](http://www.ti.com/tool/ek-tm4c123gxl))

Please kindly check sub-folders for more project details:

- `1_Bring Up` starts your board with system ticks and basic GPIOs, include LED, SW, UART
- `2_Sleep Mode` puts your board into low power mode, you can see a decease of CPU Usage
- `3_Firmware Update` shows you how to switch your board to USB Device Firmware Update mode
- `4_Boot Loader` requires 2 parts: `BootLoaderUsb` providing USB DFU, `JumpToBootLoader` hooks 2 SW to enter boot loader
- `5_LCD Menu` shows a menu to user with menu item and controllable settings
- `6_Audio` uses MCP4921 12b DAC to play sound. it also include Audio Generator to make waves. This project uses uDMA for faster data transfer.