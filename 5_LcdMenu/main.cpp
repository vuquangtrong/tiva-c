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

/* Resource and Pin map

 |-PA0 uart0_rx
 |-PA1 uart0_tx
 |-PA2 lcd_clk  oled_sck
 |-PA3 lcd_ce   oled_cs
 |-PA4 lcd_bl
 |-PA5 lcd_din  oled_sda
 |-PA6 lcd_dc   oled_dc
 |-PA7 lcd_rst  oled_rst

 |-PF0 btn_left
 |-PF1 led_red
 |-PF2 led_blue
 |-PF3 led_green
 |-PF4 btn_right

 |-PD4 usb0_pd
 |-PD5 usb0_pm

 */

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which config the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
    Logger::getInstance().print("\033[2J"); // clear screen
#if USE_OLED_SH1106
    Logger::getInstance().println("\n\rMenu on OLED SH1106");
#elif USE_LCD_NOKIA5110
    Logger::getInstance().println("\n\rMenu on LCD NOKIA5110");
#else
    Logger::getInstance().println("\n\rNo Display is configured! Exit!");
    return;
#endif
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

