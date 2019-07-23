/*
 * MCP4921.h
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#ifndef AUDIOOUTPUT_MCP4921_H_
#define AUDIOOUTPUT_MCP4921_H_

#pragma once

#include <stdint.h>
#include "driverlib/gpio.h"

class MCP4921
{
public:

    /*
     * PIN_1 = VDD, set to 3.3V
     * PIN_2 = CS
     * PIN_3 = SCK
     * PIN_4 = SDI
     * PIN_5 = LDAC, set to GND to push data on every CS=HIGH
     * PIN_6 = VREF, set to 3.3V, max Vout = 3.3*G
     * PIN_7 = VSS, set to GND
     * PIN_8 = VOUT
     */

    enum Settings
    {
        DAC_RESOLUTION = 12,
        PIN_DIN = GPIO_PIN_3,
        PIN_CE = GPIO_PIN_1,
        PIN_CLK = GPIO_PIN_0,
        ACTIVE = 0x7000, /* BUF = 1, ~G=1, ~SHUTDOWN=1 */
        SHUTDOWN = 0x6000 /* BUF = 1, ~G=1, ~SHUTDOWN=0 */
    };

    static MCP4921& getInstance()
    {
        static MCP4921 __instance;
        return __instance;
    }

    void output(uint16_t voltage);
    void setActive(bool state);
    uint16_t makeData(uint16_t data);

private:
    bool _isActive;

    MCP4921();
};

#endif /* AUDIOOUTPUT_MCP4921_H_ */

