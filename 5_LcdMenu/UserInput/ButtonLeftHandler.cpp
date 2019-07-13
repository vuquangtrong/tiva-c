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
#include "MainMenu/MainMenu.h"
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
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA,
    GPIO_PIN_TYPE_STD_WPU);

    // assign button
    _button.setGPIO(GPIO_PORTF_BASE, GPIO_PIN_4);
    _button.setButtonHandler(this);
}
void ButtonLeftHandler::checkState()
{
    _button.checkState();
}

bool ButtonLeftHandler::getRawState()
{
    return _button.getRawState();
}

void ButtonLeftHandler::onClick()
{
    MainMenu::getIntance().hanldeAction(MainMenu::ACTION_UP);
}
void ButtonLeftHandler::onDoubleClick()
{
    MainMenu::getIntance().hanldeAction(MainMenu::ACTION_INC);
}
void ButtonLeftHandler::onHold()
{
    MainMenu::getIntance().hanldeAction(MainMenu::ACTION_ENTER);
}
void ButtonLeftHandler::onRelease()
{
}
