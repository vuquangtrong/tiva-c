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
#include "AudioOutput/AudioOutput.h"

/*                Resource and Pin map                                  */
/*
                   +----------------+
     btn_left  PF0-|                |-PA0 uart_rx
     led_red   PF1-|                |-PA1 uart_tx
     led_blue  PF2-|                |-PA2=SSI0Clk lcd_clk
     led_green PF3-|                |-PA3=SSI0Fss lcd_ce
     btn_right PF4-|     Tiva C     |-PA4=SSI0Rx  lcd_bl  (not use SSI0Rx)
                  -|  TM4C123GH6PM  |-PA5=SSI0Tx  lcd_din
                  -|                |-PA6         lcd_dc
                  -|                |-PA7         lcd_rst
                  -|                |-
                  -|                |-PD0=SSI1Clk dac_clk
                  -|                |-PD1=SSI1Fss dac_ce
                  -|                |-PD2=SSI1Rx  NC
                  -|                |-PD3=SSI1Tx  dac_din
                  -|                |-PD4=USB0PM  usb_pm
                  -|                |-PD5=USB0PD  usb_pd
                  -|                |-
                  -|                |-
                   +----------------+
 */

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which config the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
    //Logger::getIntance().print("\033[2J"); // clear screen
    Logger::getIntance().println("\n\rAudio");
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

    // initialize all modules
    CpuUsage::getIntance();
    MainMenu::getIntance();
    AudioOutput::getIntance().test();

    // run main program by enable interruptions
    EnableInterrupts();
    while (true)
    {
        // put CPU to low power mode, and wait for an interruption
        WaitForInterrupt();
    }
}

