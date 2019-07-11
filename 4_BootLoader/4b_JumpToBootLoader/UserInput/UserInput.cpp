/*
 * UserInput.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include "UserInput.h"
#include "Logger/Logger.h"
#include "ButtonLeftHandler.h"
#include "ButtonRightHandler.h"

UserInput::UserInput()
{
    initUserInput();
}

void UserInput::initUserInput()
{
    for (int i = 0; i < BUTTONS_MAX; i++)
    {
        _buttons[i] = 0;
    }

    // add buttons here
    ButtonLeftHandler* buttonLeftHandler = new ButtonLeftHandler();
    _buttons[0] = buttonLeftHandler;

    ButtonRightHandler* buttonRightHandler = new ButtonRightHandler();
    _buttons[1] = buttonRightHandler;
}

void UserInput::checkInput()
{
    for (int i = 0; i < BUTTONS_MAX; i++)
    {
        if (_buttons[i] != 0)
        {
            _buttons[i]->checkState();
        }
    }
}

bool UserInput::isPressed(uint8_t btnId){
    if(btnId >=BUTTONS_MAX) {
        return false;
    }

    return (_buttons[btnId]->getRawState());
}
