/*
 * Property.h
 *
 *  Created on: Jul 12, 2019
 *      Author: vqtrong
 */

#ifndef MENU_PROPERTY_H_
#define MENU_PROPERTY_H_

class Property
{
public:
    virtual const char* getText() = 0;
    virtual void valueUp() = 0;
    virtual void valueDown() = 0;
};

#endif /* MENU_PROPERTY_H_ */
