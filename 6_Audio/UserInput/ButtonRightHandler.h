/*
 * ButtonRightHandler.h
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef USERINPUT_BUTTONRIGHTHANDLER_H_
#define USERINPUT_BUTTONRIGHTHANDLER_H_

#pragma once

#include "Button/ButtonHandler.h"
#include "Button/Button.h"

class ButtonRightHandler: public ButtonHandler
{
public:
    ButtonRightHandler();
    void checkState();
    void onClick();
    void onDoubleClick();
    void onHold();
    void onRelease();
    bool getRawState();

private:
    Button _button;
};

#endif /* USERINPUT_BUTTONRIGHTHANDLER_H_ */
