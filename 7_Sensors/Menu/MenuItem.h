/*
 * MenuItem.h
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#ifndef MENU_MENUITEM_H_
#define MENU_MENUITEM_H_

#pragma once

#include "Properties/Property.h"

class Menu;

class MenuItem
{
public:
    MenuItem();
    void setParentMenu(Menu* parentMenu);
    void setChildMenu(Menu* childMenu);
    void setProperty(Property* property);
    Menu* getParentMenu();
    Menu* getChildMenu();
    Property* getProperty();

private:
    Menu *_parentMenu;
    Menu *_childMenu;
    Property* _property;

};

#endif /* MENU_MENUITEM_H_ */
