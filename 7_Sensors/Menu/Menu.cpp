/*
 * Menu.cpp
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "Menu.h"
#include "MenuItem.h"

Menu::Menu() :
        _size(0)
{
}

void Menu::addMenuItem(MenuItem* child)
{
    _menuItems[_size] = child;
    _size++;
}

MenuItem* Menu::getMenuItem(uint8_t index)
{
    if (index >= _size)
    {
        return 0;
    }
    return _menuItems[index];
}

int Menu::getRowsCount()
{
    return _size;
}
