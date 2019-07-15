/*
 * SH1106.cpp
 *
 *  Created on: Jul 15, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "BuildConfig.h"
#include "SH1106.h"
#include "SH1106Font.h"

SH1106::SH1106()
{
    initSH1106();
}

void SH1106::initSH1106()
{
    static bool __isSH1106Initialized = false;

    if (__isSH1106Initialized)
    {
        return;
    }

    // we use SSI0 on port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

#ifdef USE_SLEEP_MODE_DISPLAY
    // enable the peripheral in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOA);
#else
    SysCtlPeripheralSleepDisable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralDeepSleepDisable(SYSCTL_PERIPH_GPIOA);
#endif

    // we change the pin's purpose
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5);
    // set output direction on DC and RST pins
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,
    GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7);

    // tell the system to enable SSI0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0))
    {
    }

#if USE_IN_SLEEP_MODE_DISPLAY
    // enable the peripheral in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_SSI0);
#else
    SysCtlPeripheralSleepDisable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralDeepSleepDisable(SYSCTL_PERIPH_SSI0);
#endif

    SSIDisable(SSI0_BASE);
    SSIClockSourceSet(SSI0_BASE, SSI_CLOCK_SYSTEM);
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(),
    SSI_CR0_FRF_MOTO,
                       SSI_MODE_MASTER, 4000000, 8);
    SSIEnable(SSI0_BASE);

    SysCtlDelay(80000);
    // turn on back light
    GPIOPinWrite(GPIO_PORTA_BASE, PIN_BL, 0);

    // reset LCD
    GPIOPinWrite(GPIO_PORTA_BASE, PIN_RST, 0);
    SysCtlDelay(80000);
    GPIOPinWrite(GPIO_PORTA_BASE, PIN_RST, PIN_RST);
    SysCtlDelay(80000);

    // setup LCD
    command(COMMAND, 0xae); //--turn off oled panel
    command(COMMAND, 0xa8); //--set multiplex ratio(1 to 64)
    command(COMMAND, 0x3f); //--1/64 duty
    command(COMMAND, 0xd3); //--set display offset Shift Mapping RAM Counter (0x00~0x3F)
    command(COMMAND, 0x00); //--not offset
    command(COMMAND, 0x40); //--set start line address, set Mapping RAM Display Start Line (0x00~0x3F)
    command(COMMAND, 0xa0); //--Set SEG/Column Mapping
    command(COMMAND, 0xc0); //--Set Col/Row Scan Direction
    command(COMMAND, 0xda); //--set com pins hardware configuration
    command(COMMAND, 0x12);
    command(COMMAND, 0x81); //--set contrast control register
    command(COMMAND, 0x7f); // Set SEG Output Current Brightness
    command(COMMAND, 0xa4); // Disable Entire Display On (0xa4/0xa5)
    command(COMMAND, 0xa6); // Disable Inverse Display On (0xa6/a7)
    command(COMMAND, 0xd5); //--set display clock divide ratio/oscillator frequency
    command(COMMAND, 0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
    command(COMMAND, 0x8d); //--set Charge Pump enable/disable
    command(COMMAND, 0x14); //--set(0x10) disable
    command(COMMAND, 0xaf); //--turn on oled panel
    command(COMMAND, 0xd9); //--set pre-charge period
    command(COMMAND, 0xf8); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    command(COMMAND, 0xdb); //--set vcomh
    command(COMMAND, 0x40); //Set VCOM Deselect Level

    clear();

    __isSH1106Initialized = true;
}

void SH1106::command(uint8_t type, uint8_t data)
{
    while (SSIBusy(SSI0_BASE))
    {
    }
    if (type == COMMAND)
    {
        GPIOPinWrite(GPIO_PORTA_BASE, PIN_DC, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTA_BASE, PIN_DC, PIN_DC);
    }
    SSIDataPut(SSI0_BASE, data);
    while (SSIBusy(SSI0_BASE))
    {
    }
}

void SH1106::setCursor(uint8_t col, uint8_t row)
{
    if (col > WIDTH - 1 || row > ((HEIGHT / 8) - 1))
    {
        return;
    }
    command(COMMAND, 0xb0 + (row & 0x07)); // set page start address
    command(COMMAND, col & 0x0f); // set lower nibble of the column address
    command(COMMAND, ((col & 0xf0) >> 4) | 0x10); // set higher nibble of the column address
}

void SH1106::putChar(uint8_t chr)
{
    for (uint8_t i = 0; i < 6; i++)
    {
        command(DATA, SH1106_ASCII[chr - 0x20][i]);
    }
}

void SH1106::print(uint8_t col, uint8_t row, const char *msg)
{
    if (msg == 0)
    {
        return;
    }

    setCursor(col, row);
    while (*msg)
    {
        putChar(*msg);
        msg++;
    }
}

void SH1106::testFillFullImage(const uint8_t *img)
{
    uint16_t j = 0;
    for (uint8_t y = 0; y < (HEIGHT / 8); y++)
    {
        setCursor(0, y);
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            command(DATA, img[j++]);
        }
    }
}

void SH1106::clear()
{
    for (uint8_t y = 0; y < (HEIGHT / 8); y++)
    {
        setCursor(0, y);
        for (uint8_t x = 0; x < WIDTH; x++)
        {
            command(DATA, 0);
        }
    }
}

void SH1106::setBacklight(bool on)
{
}

void SH1106::setBrightness(uint8_t val)
{
    command(COMMAND, 0x81);
    command(COMMAND, val);
}

