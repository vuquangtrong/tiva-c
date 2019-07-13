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
#include "ButtonLeftHandler.h"
#include "ButtonRightHandler.h"

#define BUTTONS_MAX 2

class UserInput
{
public:
    static UserInput& getIntance()
    {
        static UserInput __intance;
        return __intance;
    }

    void checkInput();
    bool isPressed(uint8_t btnId);

private:
    ButtonLeftHandler buttonLeftHandler;
    ButtonRightHandler buttonRightHandler;

    UserInput();
};

#endif /* USERINPUT_USERINPUT_H_ */
