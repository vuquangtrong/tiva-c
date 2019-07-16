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
#include "ButtonLeftHandler.h"
#include "ButtonRightHandler.h"

UserInput::UserInput()
{
}

void UserInput::checkInput()
{
    buttonLeftHandler.checkState();
    buttonRightHandler.checkState();
}

bool UserInput::isPressed(uint8_t btnId)
{
    switch (btnId)
    {
    case 0:
        return buttonLeftHandler.getRawState();
    case 1:
        return buttonRightHandler.getRawState();
    default:
        return false;
    }
}
