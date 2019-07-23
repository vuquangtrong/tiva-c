/*
 * ButtonRightHandler.h
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef USERINPUT_BUTTONRIGHT_H_
#define USERINPUT_BUTTONRIGHT_H_

#pragma once

#include "Button/Button.h"

class ButtonRight: public Button
{
public:
    ButtonRight();
    void init();
    void onClick();
    void onDoubleClick();
    void onHold();
    void onRelease();
};

#endif /* USERINPUT_BUTTONRIGHT_H_ */
