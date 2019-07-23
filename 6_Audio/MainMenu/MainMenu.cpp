/*
 * MainMenu.cpp
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "BuildConfig.h"
#include "MainMenu.h"
#include "Menu/Menu.h"
#include "Menu/MenuItem.h"
#include "Helper/ustdlib.h"
#include "Display/Display.h"
#include "Logger/Logger.h"

MainMenu::MainMenu()
{
    // setup menus and theirs items

    _root.addMenuItem(&_menuItem_settings);
    _root.addMenuItem(&_menuItem_about);

    _menu_settings.addMenuItem(&_menuItem_setting_volume);
    _menu_settings.addMenuItem(&_menuItem_setting_balance);
#if USE_LCD_NOKIA5110
    _menu_settings.addMenuItem(&_menuItem_setting_backlight);
#elif USE_OLED_SH1106
    _menu_settings.addMenuItem(&_menuItem_setting_brightness);
#endif

    _menu_about.addMenuItem(&_menuItem_about_hw_ver);
    _menu_about.addMenuItem(&_menuItem_about_sw_ver);

    _menuItem_settings.setChildMenu(&_menu_settings);
    _menuItem_settings.setProperty(&_label_settings);
    _label_settings.setText("Settings");

    _menuItem_about.setChildMenu(&_menu_about);
    _menuItem_about.setProperty(&_label_about);
    _label_about.setText("About");

    _menuItem_setting_balance.setParentMenu(&_root);
    _menuItem_setting_balance.setProperty(&_number_setting_balance);
    _number_setting_balance.setText("Balance");

    _menuItem_setting_volume.setParentMenu(&_root);
    _menuItem_setting_volume.setProperty(&_number_setting_volume);
    _number_setting_volume.setText("Volume");
    _number_setting_volume.setNumber(5, 10, 0);

#if USE_LCD_NOKIA5110
    _menuItem_setting_backlight.setParentMenu(&_root);
    _menuItem_setting_backlight.setProperty(&_switch_setting_backlight);
    _switch_setting_backlight.setText("Backlight");
    _switch_setting_backlight.setChecked(true);
#elif USE_OLED_SH1106
    _menuItem_setting_brightness.setParentMenu(&_root);
    _menuItem_setting_brightness.setProperty(&_switch_setting_brightness);
    _switch_setting_brightness.setText("Brightness");
    _switch_setting_brightness.setNumber(0, 125, 0);
#endif

    _menuItem_about_hw_ver.setParentMenu(&_root);
    _menuItem_about_hw_ver.setProperty(&_label_about_hw_ver);
    _label_about_hw_ver.setText("Vu Quang Trong");

    _menuItem_about_sw_ver.setParentMenu(&_root);
    _menuItem_about_sw_ver.setProperty(&_label_about_sw_ver);
    _label_about_sw_ver.setText("SW: 0.66 - Audio");

    _currentMenu = &_root;
    _currentRow = 0;
    _isChanged = true;
}

void MainMenu::drawMenu()
{
    if (_isChanged)
    {
        char line[Menu::MAX_CHARS_PER_ROW + 2 + 1];
        for (uint8_t i = 0; i < Menu::MAX_ROWS; i++)
        {
            line[0] = 0;
            if (i < _currentMenu->getRowsCount())
            {
                usprintf(
                        line, "%c %s", (i == _currentRow ? '>' : ' '),
                        _currentMenu->getMenuItem(i)->getProperty()->getText());
            }
            uint8_t y = ustrlen(line);
            while (y < Menu::MAX_CHARS_PER_ROW + 2 + 1)
            {
                line[y] = ' ';
                y++;
            }
            line[Menu::MAX_CHARS_PER_ROW + 2] = 0;
            Display::getInstance().print(0, i, line);
        }
        _isChanged = false;
    }
}

void MainMenu::hanldeAction(MainMenu::ACTIONS action)
{
    Logger::getInstance().printf("action=%d", action);
    switch (action)
    {
    case MainMenu::ACTION_UP:
        if (_currentRow == 0)
        {
            _currentRow = _currentMenu->getRowsCount() - 1;
        }
        else
        {
            _currentRow--;
        }
        break;
    case MainMenu::ACTION_DOWN:
        if (_currentRow == _currentMenu->getRowsCount() - 1)
        {
            _currentRow = 0;
        }
        else
        {
            _currentRow++;
        }
        break;
    case MainMenu::ACTION_ENTER:
        if (_currentMenu->getMenuItem(_currentRow)->getChildMenu())
        {
            _currentMenu =
                    _currentMenu->getMenuItem(_currentRow)->getChildMenu();
            _currentRow = 0;
        }
        break;
    case MainMenu::ACTION_EXIT:
        if (_currentMenu->getMenuItem(_currentRow)->getParentMenu())
        {
            _currentMenu =
                    _currentMenu->getMenuItem(_currentRow)->getParentMenu();
            _currentRow = 0;
        }
        break;
    case MainMenu::ACTION_INC:
        _currentMenu->getMenuItem(_currentRow)->getProperty()->valueUp();
        break;
    case MainMenu::ACTION_DEC:
        _currentMenu->getMenuItem(_currentRow)->getProperty()->valueDown();
        break;
    }

    _isChanged = true;
}
