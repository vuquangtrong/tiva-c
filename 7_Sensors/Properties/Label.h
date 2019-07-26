/*
 * Settings.h
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#ifndef SYSTEM_MAINMENU_LABEL_H_
#define SYSTEM_MAINMENU_LABEL_H_

#pragma once

#include "Menu/Menu.h"
#include "Property.h"
#include "Helper/ustdlib.h"

class Label: public Property
{
public:
    Label()
    {
    }

    void setText(const char *label)
    {
        ustrncpy(_label, label, Menu::MAX_CHARS_PER_ROW);
        uint8_t y = ustrlen(_label);
        while (y < Menu::MAX_CHARS_PER_ROW)
        {
            _label[y] = ' ';
            y++;
        }
        _label[Menu::MAX_CHARS_PER_ROW] = 0;
    }

    const char* getText()
    {
        return _label;
    }

    void valueUp()
    {
    }

    void valueDown()
    {
    }

    char _label[Menu::MAX_CHARS_PER_ROW + 1];
};

#endif /* SYSTEM_MAINMENU_LABEL_H_ */
