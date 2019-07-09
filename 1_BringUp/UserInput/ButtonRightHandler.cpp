/*
 * ButtonRightHandler.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "ButtonRightHandler.h"
#include "Logger/Logger.h"

ButtonRightHandler::ButtonRightHandler()
{
    // SW of port F, we must enable port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // wait for it
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
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // assign button
    _button.setGPIO(GPIO_PORTF_BASE, GPIO_PIN_0);
    _button.setButtonHandler(this);
}
void ButtonRightHandler::checkState()
{
    _button.checkState();
}

void ButtonRightHandler::onClick()
{
    Logger::getIntance().print("Right button Click\n\r");
}
void ButtonRightHandler::onDoubleClick()
{
    Logger::getIntance().print("Right button Double Click\n\r");
}
void ButtonRightHandler::onHold()
{
    Logger::getIntance().print("Right button Hold\n\r");
}
void ButtonRightHandler::onRelease()
{
    Logger::getIntance().print("Right button Release\n\r");
}
