/*
 * Settings.h
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#ifndef SYSTEM_MAINMENU_NUMBER_H_
#define SYSTEM_MAINMENU_NUMBER_H_

#pragma once

#include <stdint.h>
#include "Menu/Menu.h"
#include "Property.h"
#include "Helper/ustdlib.h"

class Number: public Property
{
public:
    Number()
    {
        _number = 0;
        _max = 5;
        _min = -5;
    }

    void setNumber(int16_t number, int16_t max, int16_t min)
    {
        _number = number;
        _max = max;
        _min = min;
    }

    void setText(const char *label)
    {
        ustrncpy(_text, label, Menu::MAX_CHARS_PER_ROW - 3);
        uint8_t y = ustrlen(_text);
        while (y < Menu::MAX_CHARS_PER_ROW)
        {
            _text[y] = ' ';
            y++;
        }
        _text[Menu::MAX_CHARS_PER_ROW] = 0;
    }

    const char* getText()
    {
        _text[Menu::MAX_CHARS_PER_ROW - 3] = 0;
        usprintf(_text, "%s%d", _text, _number);
        return _text;
    }

    void valueUp()
    {
        if (_number < _max)
        {
            _number++;
        }
    }

    void valueDown()
    {
        if (_number > _min)
        {
            _number--;
        }
    }

    char _text[Menu::MAX_CHARS_PER_ROW + 1];
    int16_t _number;
    int16_t _max;
    int16_t _min;
};

#endif /* SYSTEM_MAINMENU_NUMBER_H_ */
