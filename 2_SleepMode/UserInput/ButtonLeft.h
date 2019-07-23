/*
 * ButtonLeftHandler.h
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef USERINPUT_BUTTONLEFT_H_
#define USERINPUT_BUTTONLEFT_H_

#pragma once

#include "Button/Button.h"

class ButtonLeft: public Button
{
public:
    ButtonLeft();
    void init();
    void onClick();
    void onDoubleClick();
    void onHold();
    void onRelease();
};

#endif /* USERINPUT_BUTTONLEFT_H_ */
