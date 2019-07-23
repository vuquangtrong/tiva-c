/*
 * SH1106.h
 *
 *  Created on: Jul 15, 2019
 *      Author: vqtrong
 */

#ifndef DISPLAY_SH1106_H_
#define DISPLAY_SH1106_H_

#pragma once

#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "DisplayInterface.h"

class SH1106: public DisplayInterface
{
public:
    enum Settings
    {
        WIDTH = 128,
        HEIGHT = 64,
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

    static SH1106& getInstance()
    {
        static SH1106 __instance;
        return __instance;
    }

    void print(uint8_t col, uint8_t row, const char *msg);
    void clear();
    void testFillFullImage(const uint8_t *img);
    void setBacklight(bool on);
    void setBrightness(uint8_t val);

private:
    SH1106();
    void initSH1106();
    void command(uint8_t type, uint8_t data);
    void setCursor(uint8_t x, uint8_t y);
    void putChar(uint8_t chr);
};

#endif /* DISPLAY_SH1106_H_ */
