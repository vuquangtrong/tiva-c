/*
 * ButtonRightHandler.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include <UserInput/ButtonRight.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "Logger/Logger.h"

ButtonRight::ButtonRight()
{
    init();
}
void ButtonRight::init()
{
    // SW of port F, we must enable port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
    {
    }

    //
    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    //
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // set input direction
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);

    // assign button
    config(GPIO_PORTF_BASE, GPIO_PIN_0);
}

void ButtonRight::onClick()
{
    Logger::getInstance().print("Right button Click\n\r");
}
void ButtonRight::onDoubleClick()
{
    Logger::getInstance().print("Right button Double Click\n\r");
}
void ButtonRight::onHold()
{
    Logger::getInstance().print("Right button Hold\n\r");
}
void ButtonRight::onRelease()
{
    Logger::getInstance().print("Right button Release\n\r");
}
