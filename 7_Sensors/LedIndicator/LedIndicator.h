/*
 * LedIndicator.h
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef LEDINDICATOR_LEDINDICATOR_H_
#define LEDINDICATOR_LEDINDICATOR_H_

#pragma once

#include <stdint.h>
#include "driverlib/gpio.h"

class LedIndicator
{
public:
    static LedIndicator& getInstance()
    {
        static LedIndicator __instance;
        return __instance;
    }

    void outputHeartBeatSignal();

private:
    enum Settings
    {
        LED_OFF = 0x00,
        LED_RED = GPIO_PIN_1,
        LED_BLUE = GPIO_PIN_2,
        LED_GREEN = GPIO_PIN_3,
    };

    LedIndicator();
};

#endif /* LEDINDICATOR_LEDINDICATOR_H_ */
