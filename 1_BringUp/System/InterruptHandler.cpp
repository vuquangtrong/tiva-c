/*
 * InterruptHandler.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include "InterruptHandler.h"
#include "LedIndicator/LedIndicator.h"
#include "UserInput/UserInput.h"

void SysTickInterruptHandler()
{
    LedIndicator::getIntance().outputAliveSignal();
    UserInput::getIntance().checkInput();
}
