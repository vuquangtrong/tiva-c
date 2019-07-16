/*
 * Nokia5110.h
 *
 *  Created on: Jul 11, 2019
 *      Author: vqtrong
 */

#ifndef DISPLAY_NOKIA5110_H_
#define DISPLAY_NOKIA5110_H_

#pragma once

#include <stdint.h>
#include "driverlib/gpio.h"

class Nokia5110
{
public:
    enum SETTINGS
    {
        WIDTH = 84,
        HEIGHT = 48,
        LCD_CONTRAST = 0xA4,
        DISPLAY_MODE = 0x0C,
        PIN_RST = GPIO_PIN_7,
        PIN_DC = GPIO_PIN_6,
        PIN_DIN = GPIO_PIN_5,
        PIN_BL = GPIO_PIN_4,
        PIN_CE = GPIO_PIN_3,
        PIN_CLK = GPIO_PIN_2,
        COMMAND = 0,
        DATA = 1
    };

    static Nokia5110& getIntance()
    {
        static Nokia5110 __intance;
        return __intance;
    }

    void print(uint8_t col, uint8_t row, const char *msg);
    void clear();
    void testFillFullImage(const uint8_t *img);
    void setBacklight(bool state);

private:
    Nokia5110();
    void initNokia5110();
    void command(uint8_t type, uint8_t data);
    void setCursor(uint8_t x, uint8_t y);
    void putChar(uint8_t chr);
};

#endif /* DISPLAY_NOKIA5110_H_ */
