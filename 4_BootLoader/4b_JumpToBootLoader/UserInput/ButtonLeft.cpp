/*
 * ButtonLeftHandler.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include <UserInput/ButtonLeft.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "Logger/Logger.h"

ButtonLeft::ButtonLeft()
{
    init();
}
void ButtonLeft::init()
{
    // SW of port F, we must enable port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // wait for it
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    // set input direction
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);

    // assign button
    config(GPIO_PORTF_BASE, GPIO_PIN_4);
}

void ButtonLeft::onClick()
{
    Logger::getInstance().print("Left button Click\n\r");
}
void ButtonLeft::onDoubleClick()
{
    Logger::getInstance().print("Left button Double Click\n\r");
}
void ButtonLeft::onHold()
{
    Logger::getInstance().print("Left button Hold\n\r");
}
void ButtonLeft::onRelease()
{
    Logger::getInstance().print("Left button Release\n\r");
}
