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
#include "Helper/CpuUsage.h"
#include "MainMenu/MainMenu.h"
#include "Sensor/Sensors.h"

void SysTickInterruptHandler()
{
    static uint32_t __tick_count = 0;

    __tick_count++;
    if (__tick_count == SYSTICKS_PER_SECOND)
    {
        __tick_count = 0;
        LedIndicator::getInstance().outputHeartBeatSignal();
//#if !PROBE_SENSOR_DATA
//        Logger::getInstance().printf("%3d%%\n\r", CpuUsage::getInstance().getCurrentUsage());
//#endif
    }

    UserInput::getInstance().checkInput();
    MainMenu::getInstance().drawMenu();
    Sensors::getInstance().readSensors();
}
