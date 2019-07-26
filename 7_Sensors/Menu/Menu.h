/*
 * Menu.h
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#ifndef MENU_MENU_H_
#define MENU_MENU_H_

#pragma once

#include <stdint.h>
#include "MenuItem.h"

class Menu
{
public:
    enum Settings
    {
        MAX_ROWS = 6, MAX_CHARS_PER_ROW = 14
    };

    Menu();
    void addMenuItem(MenuItem* child);
    MenuItem* getMenuItem(uint8_t index);
    int getRowsCount();

private:
    MenuItem* _menuItems[MAX_ROWS];
    uint8_t _size;
};

#endif /* MENU_MENU_H_ */
