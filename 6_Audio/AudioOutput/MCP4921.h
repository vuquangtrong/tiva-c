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
     * Vdd = Vbus
     * Vss = GND
     * Vref = 3.3 : max Vout = 3.3*G
     * ~LDAC = GND: push data on CS=High every bit
     */

    enum SETTINGS
    {
        DAC_RESOLUTION = 12,
        PIN_DIN = GPIO_PIN_3,
        PIN_CE = GPIO_PIN_1,
        PIN_CLK = GPIO_PIN_0,
        ACTIVE = 0x7000, /* BUF = 1, ~G=1, ~SHUTDOWN=1 */
        SHUTDOWN = 0x6000 /* BUF = 1, ~G=1, ~SHUTDOWN=0 */
    };

    static MCP4921& getIntance()
    {
        static MCP4921 __intance;
        return __intance;
    }

    void output(uint16_t voltage);
    void setActive(bool state);
    uint16_t makeData(uint16_t data);

private:
    bool _isActive;
    MCP4921();
    void initMCP4921();

};

#endif /* AUDIOOUTPUT_MCP4921_H_ */

