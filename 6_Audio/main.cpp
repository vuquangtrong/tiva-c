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

 |-PD0 SSI1Clk  dac_clk
 |-PD1 SSI1Fss  dac_ce
 |-PD2 SSI1Rx   NC
 |-PD3 SSI1Tx   dac_din
 |-PD4 usb_pd
 |-PD5 usb_pm

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
    Logger::getInstance().println("\n\rAudio on OLED SH1106");
#elif USE_LCD_NOKIA5110
    Logger::getInstance().println("\n\rAudio on LCD NOKIA5110");
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

#if TEST_AUDIO
    // Play Audio Sample
    AudioOutput::getInstance().playSampleAudio();
#else
    AudioOutput::getInstance();
#endif

    // run main program by enable interruptions
    EnableInterrupts();

    while (true)
    {
        // put CPU to low power mode, and wait for an interruption
        WaitForInterrupt();
    }
}

