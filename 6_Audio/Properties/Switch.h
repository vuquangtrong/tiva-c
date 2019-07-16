/*
 * Settings.h
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#ifndef SYSTEM_MAINMENU_SWITCH_H_
#define SYSTEM_MAINMENU_SWITCH_H_

#pragma once

#include "Menu/Menu.h"
#include "Property.h"
#include "Helper/ustdlib.h"
#include "../Display/Nokia5110.h"

class Switch: public Property
{
public:
    Switch()
    {
        _checked = true;
    }

    void setChecked(bool checked)
    {
        _checked = checked;
    }

    void setText(const char *label)
    {
        ustrncpy(_text, label, Menu::MAX_CHARS_PER_ROW);
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
        usprintf(_text, "%s%s", _text, _checked ? "[x]" : "[ ]");
        return _text;
    }

    void valueUp()
    {
        _checked = !_checked;
        Nokia5110::getIntance().setBacklight(_checked);

    }

    void valueDown()
    {
        _checked = !_checked;
        Nokia5110::getIntance().setBacklight(_checked);
    }

private:
    char _text[Menu::MAX_CHARS_PER_ROW + 1];
    bool _checked;
};

#endif /* SYSTEM_MAINMENU_SWITCH_H_ */
