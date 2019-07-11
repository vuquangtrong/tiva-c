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
#include "Logger/Logger.h"

/* README
 *
 * Set --heap_size to use dynamic memory allocation, currently it is set to 512 bytes
 *
 */

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which config the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
    //Logger::getIntance().print("\033[2J"); // clear screen
    Logger::getIntance().println("\n\rFwUpdate");
    Logger::getIntance().print("HW: ");
    Logger::getIntance().println(HW_VERSION);
    Logger::getIntance().print("SW: ");
    Logger::getIntance().println(SW_VERSION);

    // if 2 buttons are pressed during startup, go to boot loader
    if (UserInput::getIntance().isPressed(0)
            && UserInput::getIntance().isPressed(0))
    {
        Logger::getIntance().println("Enter Update Mode!");
        JumpToUsbUpdate();
    }

    // run main program by enable interruptions
    EnableInterrupts();
    while (true)
    {
        // put CPU to low power mode, and wait for an interruption
        WaitForInterrupt();
    }
}

