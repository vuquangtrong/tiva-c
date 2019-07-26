/*
 * Nokia5110.cpp
 *
 *  Created on: Jul 11, 2019
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
#include "Nokia5110.h"
#include "Nokia5110font.h"

Nokia5110::Nokia5110()
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
    command(COMMAND, 0x21); // enable chip Enable chip (PD=0), Horizontal addressing (V=0), Extended instructions (H=1)
    command(COMMAND, LCD_CONTRAST); // contrast
    command(COMMAND, 0x04); // temp coeff
    command(COMMAND, 0x13); // lcd bias, 0x13 or 0x14
    command(COMMAND, 0x20); // switch to basic command mode
    command(COMMAND, DISPLAY_MODE); // normal display, not inverse

    clear();
}

void Nokia5110::command(uint8_t type, uint8_t data)
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

void Nokia5110::setCursor(uint8_t col, uint8_t row)
{
    if (col > WIDTH - 1 || row > ((HEIGHT / 8) - 1))
    {
        return;
    }
    command(COMMAND, 0x80 | (col));    // setting bit 7 updates X-position
    command(COMMAND, 0x40 | (row));    // setting bit 6 updates Y-position
}

void Nokia5110::putChar(uint8_t chr)
{
    uint8_t i;
    for (i = 0; i < 5; i++)
    {
        command(DATA, NOKIA5110_ASCII[chr - 0x20][i]);
    }
}

void Nokia5110::print(uint8_t col, uint8_t row, const char *msg)
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

void Nokia5110::testFillFullImage(const uint8_t *img)
{
    uint16_t i;
    uint16_t imax = WIDTH * HEIGHT / 8;

    for (i = 0; i < imax; i++)
    {
        command(DATA, img[i]);
    }
}

void Nokia5110::clear()
{

    uint16_t i;
    uint16_t imax = WIDTH * HEIGHT / 8;

    for (i = 0; i < imax; i++)
    {
        command(DATA, 0x00);
    }
}

void Nokia5110::setBacklight(bool on)
{
    if (on)
    {
        GPIOPinWrite(GPIO_PORTA_BASE, PIN_BL, 0);
    }
    else
    {
        GPIOPinWrite(GPIO_PORTA_BASE, PIN_BL, PIN_BL);
    }
}

void Nokia5110::setBrightness(uint8_t val)
{

}
