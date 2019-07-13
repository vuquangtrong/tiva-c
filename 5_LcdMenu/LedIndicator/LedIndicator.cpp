/*
 * LedIndicator.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "BuildConfig.h"
#include "LedIndicator.h"

LedIndicator::LedIndicator()
{
    initLedIndicator();
}

void LedIndicator::initLedIndicator()
{
    static bool __isLedIndicatorInitialized = false;

    if (__isLedIndicatorInitialized)
    {
        return;
    }

    // LED of port F, we must enable port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // wait for it
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

#ifdef USE_SLEEP_MODE
    // enable the peripheral in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOF);
#endif

    // set output direction on LEDs' pins
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    __isLedIndicatorInitialized = true;
}

void LedIndicator::outputHeartBeatSignal()
{
    GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, LED_BLUE);
    SysCtlDelay(1000);
    GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, LED_OFF);
}
