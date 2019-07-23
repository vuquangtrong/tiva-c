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

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which config the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
    Logger::getInstance().print("\033[2J"); // clear screen
    Logger::getInstance().println("\n\rFwUpdate");
    Logger::getInstance().println(HW_VERSION);
    Logger::getInstance().println(SW_VERSION);

    // if 2 buttons are pressed during startup, go to boot loader
    if (UserInput::getInstance().isAllButtonsPressed())
    {
        Logger::getInstance().println("Enter Update Mode!");
        JumpToUsbUpdate();
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

