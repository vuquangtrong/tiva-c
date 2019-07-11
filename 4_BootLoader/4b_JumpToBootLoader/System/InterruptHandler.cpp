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
#include "Logger/Logger.h"
#include "Utils/CpuUsage.h"

void SysTickInterruptHandler()
{
    static uint32_t __tick_count = 0;

    __tick_count++;
    if(__tick_count == SYSTICKS_PER_SECOND)
    {
        __tick_count = 0;
        LedIndicator::getIntance().outputHeartBeatSignal();
        Logger::getIntance().printf("%3d%%\n\r", CpuUsage::getIntance().getCurrentUsage());
    }

    UserInput::getIntance().checkInput();
}
