Tiva C TM4C123G LaunchPad
=========================

This is my practice on an ARM Cortex-M4F processor ([Tiva LaunchPad TM4C123G](http://www.ti.com/tool/ek-tm4c123gxl))

Please kindly check sub-folders for more project details:

- `1_BringUp` starts your board with system ticks and basic GPIOs, include LED, SW, UART
- `2_SleepMode` puts your board into low power mode, you can see a decease of CPU Usage
- `3_FwUpdate` shows you how to switch your board to USB Device Firmware Update mode
- `4_BootLoader` requires 2 parts: `BootLoaderUsb` providing USB DFU, `JumpToBootLoader` hooks 2 SW to enter boot loader
