/*
 * MCP4921.cpp
 *
 *  Created on: Jul 13, 2019
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
#include "MCP4921.h"

MCP4921::MCP4921()
{
    _isActive = true;

    // use SSI1 on port D
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

#if USE_SLEEP_MODE_AUDIO
    // enable the peripheral in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOD);
#else
    SysCtlPeripheralSleepDisable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralDeepSleepDisable(SYSCTL_PERIPH_GPIOD);
#endif

    // enable SSI1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI1))
    {
    }

#if USE_SLEEP_MODE_AUDIO
    // allow it run in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_SSI1);
#else
    SysCtlPeripheralSleepDisable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralDeepSleepDisable(SYSCTL_PERIPH_SSI1);
#endif

    // setup pins
    GPIOPinConfigure(GPIO_PD0_SSI1CLK);
    GPIOPinConfigure(GPIO_PD1_SSI1FSS);
    GPIOPinConfigure(GPIO_PD3_SSI1TX);
    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3);

    // setup SSI
    SSIConfigSetExpClk(SSI1_BASE, SysCtlClockGet(),
    SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, 44100 * 16, 16);

    // turn SSI1 on
    SSIEnable(SSI1_BASE);
}

void MCP4921::output(uint16_t voltage)
{
    while (SSIBusy(SSI1_BASE))
    {
    }
    SSIDataPut(SSI1_BASE, (ACTIVE | (voltage & 0x0FFF)));
    while (SSIBusy(SSI1_BASE))
    {
    }
}

void MCP4921::setActive(bool state)
{
    _isActive = state;
}

uint16_t MCP4921::makeData(uint16_t data)
{
    return (_isActive ?
            (ACTIVE | (data & 0x0FFF)) : (SHUTDOWN | (data & 0x0FFF)));
}
