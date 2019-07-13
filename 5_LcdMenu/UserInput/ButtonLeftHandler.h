/*
 * ButtonLeftHandler.h
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef USERINPUT_BUTTONLEFTHANDLER_H_
#define USERINPUT_BUTTONLEFTHANDLER_H_

#pragma once

#include "Button/ButtonHandler.h"
#include "Button/Button.h"

class ButtonLeftHandler: public ButtonHandler
{
    public:
        ButtonLeftHandler();
        void checkState();
        void onClick();
        void onDoubleClick();
        void onHold();
        void onRelease();
        bool getRawState();

    private:
        Button _button;
};

#endif /* USERINPUT_BUTTONLEFTHANDLER_H_ */
