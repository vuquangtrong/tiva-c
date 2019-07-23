/*
 * main.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include "BuildConfig.h"
#include "System/SystemControl.h"
#include "Utils/CpuUsage.h"
#include "Logger/Logger.h"

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which set the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
    Logger::getInstance().print("\033[2J"); // clear screen
    Logger::getInstance().println("\n\rBring Up");
    Logger::getInstance().println(HW_VERSION);
    Logger::getInstance().println(SW_VERSION);

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

