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
    // we use SSI0 on port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

#if USE_SLEEP_MODE_DISPLAY
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

#if USE_SLEEP_MODE_DISPLAY
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
    command(COMMAND, 0xae); // turn off oled
//    command(COMMAND, 0xa8); // command set multiplex ration
//    command(COMMAND, 0x3f); // value set multiplex ration; 0x3f = 64
//    command(COMMAND, 0xd3); // command set display offset
//    command(COMMAND, 0x00); // value set display offset
//    command(COMMAND, 0x40); // set display start line = 0
    command(COMMAND, 0xa1); // rotate right, POR = 0xa0
    command(COMMAND, 0xc8); // set output scan direction, 0xc0: com0 to comN-1; 0xc8: comN-1 to com0
    command(COMMAND, 0xda); // command set common pads hw configs
    command(COMMAND, 0x12); // value set common pads hw config
    command(COMMAND, 0x81); // command set brightness
    command(COMMAND, 0x00); // value set brightness
//    command(COMMAND, 0xa4); // set normal entire display status
//    command(COMMAND, 0xa6); // normal display; 0xa7: reverse display
//    command(COMMAND, 0xd5); // command set display clock divide ratio
//    command(COMMAND, 0x50); // value set display clock divide ratio
    command(COMMAND, 0x8d);
    command(COMMAND, 0x14);
    command(COMMAND, 0xaf); // turn on oled
//    command(COMMAND, 0xd9); // command to set dis-charge/pre-charge period
//    command(COMMAND, 0x22); // value to set dis-charge/pre-charge period
//    command(COMMAND, 0xdb); // command set VCOM deselect level
//    command(COMMAND, 0x35); // value set VCOM deselect level

    clear();
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

