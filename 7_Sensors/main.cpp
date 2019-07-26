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
#include "Sensor/Sensors.h"

/* Resource and Pin map

 |-PA0 UART0RX      uart0_rx
 |-PA1 UART0TX      uart0_tx
 |-PA2 SSI0CLK      lcd_clk     oled_sck
 |-PA3 SSI0FSS      lcd_ce      oled_cs
 |-PA4 GPIO_D_OUT   lcd_bl
 |-PA5 SSI0TX       lcd_din     oled_sda
 |-PA6 GPIO_D_OUT   lcd_dc      oled_dc
 |-PA7 GPIO_D_OUT   lcd_rst     oled_rst

 |-PF0 GPIO_D_IN    btn_left
 |-PF1 GPIO_D_OUT   led_red
 |-PF2 GPIO_D_OUT   led_blue
 |-PF3 GPIO_D_OUT   led_green
 |-PF4 GPIO_D_IN    btn_right

 |-PD0 SSI1CLK      dac_clk
 |-PD1 SSI1FSS      dac_ce
 |-PD2
 |-PD3 SSI1TX       dac_din
 |-PD4 USB0PD       usb_pd
 |-PD5 USB0PM       usb_pm

 |-PB2 I2C0SCL      bmp180_scl  bmp280_scl
 |-PB3 I2C0SDA      bmp180_sda  bmp280_sda

 */

void main()
{
    // should disable system interrupt while configuring our interrupts
    DisableInterrupts();

    // MUST initialize system first, which config the SYSTEM CLOCK to 80MHz
    InitSystem();

    // start to run
#if !USE_MIDI_UART
    Logger::getInstance().print("\033[2J"); // clear screen
#if USE_OLED_SH1106
    Logger::getInstance().println("\n\rSensor on OLED SH1106");
#elif USE_LCD_NOKIA5110
    Logger::getInstance().println("\n\rSensor on LCD NOKIA5110");
#else
    Logger::getInstance().println("\n\rNo Display is configured! Exit!");
#endif
    Logger::getInstance().println(HW_VERSION);
    Logger::getInstance().println(SW_VERSION);
#endif
    // if 2 buttons are pressed during startup, go to boot loader
    if (UserInput::getInstance().isAllButtonsPressed())
    {
        Logger::getInstance().println("Enter Update Mode!");
        JumpToUsbUpdate();
    }

//#if !PROBE_SENSOR_DATA
//    // start CPU Usage
//    CpuUsage::getInstance();
//#endif

#if TEST_AUDIO
    // Play Audio Sample
    AudioOutput::getInstance().playSampleAudio();
#else
    AudioOutput::getInstance();
#endif

    // Start sensors
    Sensors::getInstance();

    // run main program by enable interruptions
    EnableInterrupts();

    while (true)
    {
        // put CPU to low power mode, and wait for an interruption
        WaitForInterrupt();
    }
}

