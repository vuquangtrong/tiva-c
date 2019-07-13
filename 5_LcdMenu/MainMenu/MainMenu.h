/*
 * MainMenu.h
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#ifndef MAINMENU_MAINMENU_H_
#define MAINMENU_MAINMENU_H_

#pragma once

#include <stdint.h>
#include "Menu/Menu.h"
#include "Menu/MenuItem.h"
#include "Properties/Label.h"
#include "Properties/Number.h"
#include "Properties/Switch.h"
#include "MainMenu.h"

class MainMenu
{
public:
    enum ACTIONS
    {
        ACTION_NONE,
        ACTION_ENTER,
        ACTION_EXIT,
        ACTION_UP,
        ACTION_DOWN,
        ACTION_INC,
        ACTION_DEC
    };

    static MainMenu& getIntance()
    {
        static MainMenu __intance;
        return __intance;
    }

    void hanldeAction(MainMenu::ACTIONS action);
    void drawMenu();

private:
    MainMenu();
    bool _isChanged;
    Menu* _currentMenu;
    uint8_t _currentRow;

    Menu _root;
    Menu _menu_settings;
    Menu _menu_about;

    MenuItem _menuItem_settings;
    Label _label_settings;

    MenuItem _menuItem_about;
    Label _label_about;

    MenuItem _menuItem_setting_volume;
    Number _number_setting_volume;

    MenuItem _menuItem_setting_balance;
    Number _number_setting_balance;

    MenuItem _menuItem_setting_backlight;
    Switch _switch_setting_backlight;

    MenuItem _menuItem_about_hw_ver;
    Label _label_about_hw_ver;

    MenuItem _menuItem_about_sw_ver;
    Label _label_about_sw_ver;
};

#endif /* MAINMENU_MAINMENU_H_ */
