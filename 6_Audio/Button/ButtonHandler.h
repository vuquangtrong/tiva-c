/*
 * ButtonHandler.h
 *
 *  Created on: Jul 9, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef BUTTON_BUTTONHANDLER_H_
#define BUTTON_BUTTONHANDLER_H_

#pragma once

class ButtonHandler
{
public:
    virtual void checkState() = 0;
    virtual void onClick() = 0;
    virtual void onDoubleClick() = 0;
    virtual void onHold() = 0;
    virtual void onRelease() = 0;
    virtual bool getRawState() = 0;
};

#endif /* BUTTON_BUTTONHANDLER_H_ */
