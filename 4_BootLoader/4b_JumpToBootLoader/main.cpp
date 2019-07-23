/*
 * main.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#define TARGET_IS_TM4C123_RB1

#include <stdint.h>
#include "BuildConfig.h"
#include "System/SystemControl.h"
#include "UserInput/UserInput.h"
#include "Utils/CpuUsage.h"
#include "Logger/Logger.h"

/* README
 *
 * THIS APPLICATION IS WORK WITH FLASH-BASED BOOTLOADER
 * Modified points:
 * - in tm4c123gh6pm.cmd:
 *      add APP_BASE and RAM_BASE, re-config flash, .intvers
 * TEST:
 * - flash BOOT LOADER first
 * - flash APPLCATION later using LM Flash Programmer only, don't use CCS flasher, start address is 0x2800
 * - press and hold 2 buttons, press reset to enter bootloader
 * - in device list, you will see Stellaris DFU device with the name Vu Quang Trong
 */

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which config the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
    Logger::getInstance().print("\033[2J"); // clear screen
    Logger::getInstance().println("\n\rBootLoader");
    Logger::getInstance().println(HW_VERSION);
    Logger::getInstance().println(SW_VERSION);

    // if 2 buttons are pressed during startup, go to boot loader
    if (UserInput::getInstance().isAllButtonsPressed())
    {
        Logger::getInstance().println("Enter Update Mode!");
        JumpToUpdateOnBootLoader();
    }

    // start CPU Usage
    CpuUsage::getInstance();

    // run main program by enable interruptions
    EnableInterrupts();

    while (true)
    {
        // put CPU to low power mode, and wait for an interruption
        WaitForInterrupt();
    }
}

