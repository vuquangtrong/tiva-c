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
#include "Logger/Logger.h"

/* README
 *
 * Set --heap_size to use dynamic memory allocation
 *
 */

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which config the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
    Logger::getIntance().println("Henry Demo");
    Logger::getIntance().print("HW: ");
    Logger::getIntance().println(HW_VERSION);
    Logger::getIntance().print("SW: ");
    Logger::getIntance().println(SW_VERSION);
    EnableInterrupts();

    while (true)
    {
        // put cpu to low power mode, and wait for an interruption
        WaitForInterrupt();
    }

}

