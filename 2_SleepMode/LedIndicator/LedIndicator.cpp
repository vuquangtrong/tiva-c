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
#include "LedIndicator.h"
#include "Logger/Logger.h"

LedIndicator::LedIndicator()
{
    // LED of port F, we must enable port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    // set output direction on LEDs' pins
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
    GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void LedIndicator::outputHeartBeatSignal()
{
    GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, LED_BLUE);
    SysCtlDelay(1000);
    GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, LED_OFF);
}
