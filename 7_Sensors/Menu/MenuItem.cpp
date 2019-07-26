/*
 * MenuItem.cpp
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "MenuItem.h"
#include "Properties/Property.h"

MenuItem::MenuItem() :
        _parentMenu(0), _childMenu(0)
{
}

void MenuItem::setParentMenu(Menu* parentMenu)
{
    _parentMenu = parentMenu;
}

Menu* MenuItem::getParentMenu()
{
    return _parentMenu;
}

void MenuItem::setChildMenu(Menu* childMenu)
{
    _childMenu = childMenu;
}

Menu* MenuItem::getChildMenu()
{
    return _childMenu;
}

void MenuItem::setProperty(Property* property)
{
    _property = property;
}

Property* MenuItem::getProperty()
{
    return _property;
}
