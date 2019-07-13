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
#include "MainMenu/MainMenu.h"
#include "Helper/CpuUsage.h"

/* Resource and Pin map */
/*
                  +---------------+
    btn_left  PF0-|               |-PA0 uart_rx
    led_red   PF1-|               |-PA1 uart_tx
    led_blue  PF2-|               |-PA2 lcd_clk
    led_green PF2-|               |-PA3 lcd_ce
    btn_right PF4-|               |-PA4 lcd_bl
                 -|               |-PA5 lcd_din
                 -|               |-PA6 lcd_dc
                 -|               |-PA7 lcd_rst
    usb_pd    PD4-|               |-
    usb_pm    PD5-|               |-
                  +---------------+
 */

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which config the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
    //Logger::getIntance().print("\033[2J"); // clear screen
    Logger::getIntance().println("\n\rLcdMenu");
    Logger::getIntance().print("HW: ");
    Logger::getIntance().println(HW_VERSION);
    Logger::getIntance().print("SW: ");
    Logger::getIntance().println(SW_VERSION);

    // if 2 buttons are pressed during startup, go to boot loader
    if (UserInput::getIntance().isPressed(0)
            && UserInput::getIntance().isPressed(1))
    {
        Logger::getIntance().println("Enter Update Mode!");
        JumpToUsbUpdate();
    }

    // start CPU Usage
    CpuUsage::getIntance().init();

    // run main program by enable interruptions
    EnableInterrupts();
    while (true)
    {
        // put CPU to low power mode, and wait for an interruption
        WaitForInterrupt();
    }
}

