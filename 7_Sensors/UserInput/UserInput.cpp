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

UserInput::UserInput()
{
}

void UserInput::checkInput()
{
    _buttonLeft.checkState();
    _buttonRight.checkState();
}

bool UserInput::isAllButtonsPressed()
{
    return _buttonLeft.isPressed() && _buttonRight.isPressed();
}
