/*
 * ButtonLeftHandler.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "BuildConfig.h"
#include "ButtonLeftHandler.h"
#include "Logger/Logger.h"

ButtonLeftHandler::ButtonLeftHandler()
{
    // SW of port F, we must enable port F
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

    // set input direction
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // assign button
    _button.setGPIO(GPIO_PORTF_BASE, GPIO_PIN_4);
    _button.setButtonHandler(this);
}
void ButtonLeftHandler::checkState()
{
    _button.checkState();
}

void ButtonLeftHandler::onClick()
{
    Logger::getIntance().print("Left button Click\n\r");
}
void ButtonLeftHandler::onDoubleClick()
{
    Logger::getIntance().print("Left button Double Click\n\r");
}
void ButtonLeftHandler::onHold()
{
    Logger::getIntance().print("Left button Hold\n\r");
}
void ButtonLeftHandler::onRelease()
{
    Logger::getIntance().print("Left button Release\n\r");
}
