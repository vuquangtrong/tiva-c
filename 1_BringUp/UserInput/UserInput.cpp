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

UserInput::UserInput()
{
}

void UserInput::checkInput()
{
    _buttonLeft.checkState();
    _buttonRight.checkState();
}
