/*
 * UserInput.h
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef USERINPUT_USERINPUT_H_
#define USERINPUT_USERINPUT_H_

#pragma once

#include <stdint.h>
#include "Button/Button.h"
#include "ButtonLeft.h"
#include "ButtonRight.h"

class UserInput
{
public:
    static UserInput& getInstance()
    {
        static UserInput __instance;
        return __instance;
    }

    void checkInput();
    bool isAllButtonsPressed();

private:
    ButtonLeft _buttonLeft;
    ButtonRight _buttonRight;

    UserInput();
};

#endif /* USERINPUT_USERINPUT_H_ */
