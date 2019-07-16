/*
 * Display.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "Display.h"

Display::Display()
{
    _nokia5110 = &Nokia5110::getIntance();
}

void Display::print(uint8_t col, uint8_t row, const char *msg)
{
    _nokia5110->print(col, row, msg);
}

void Display::clear()
{
    _nokia5110->clear();
}

void Display::setBacklight(bool state)
{
    _nokia5110->setBacklight(state);
}

