/*
 * Display.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "BuildConfig.h"
#include "Display.h"
#include "DisplayInterface.h"
#if USE_LCD_NOKIA5110
#include "Nokia5110.h"
#elif USE_OLED_SH1106
#include "SH1106.h"
#endif

Display::Display()
{
    _display = 0;
#if USE_LCD_NOKIA5110
    _display = (DisplayInterface*) &Nokia5110::getInstance();
#elif USE_OLED_SH1106
    _display = (DisplayInterface*) &SH1106::getInstance();
#endif
}

void Display::print(uint8_t col, uint8_t row, const char *msg)
{
    if (_display)
    {
        _display->print(col, row, msg);
    }
}

void Display::clear()
{
    if (_display)
    {
        _display->clear();
    }
}

void Display::setBacklight(bool state)
{
    if (_display)
    {
        _display->setBacklight(state);
    }
}

void Display::setBrightness(uint8_t val)
{
    if (_display)
    {
        _display->setBrightness(val);
    }
}
