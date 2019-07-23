/*
 * SH1106Brightness.h
 *
 *  Created on: Jul 15, 2019
 *      Author: vqtrong
 */

#ifndef PROPERTIES_SH1106BRIGHTNESS_H_
#define PROPERTIES_SH1106BRIGHTNESS_H_

#pragma once

#include <stdint.h>
#include "Menu/Menu.h"
#include "Number.h"
#include "Helper/ustdlib.h"
#include "Display/SH1106.h"

class SH1106Brightness: public Number
{
public:
    SH1106Brightness()
    {
    }

    void valueUp()
    {
        if (_number < _max)
        {
            _number+=5;
            SH1106::getInstance().setBrightness((uint8_t) _number);
        }
    }

    void valueDown()
    {
        if (_number > _min)
        {
            _number-=5;
            SH1106::getInstance().setBrightness((uint8_t) _number);
        }
    }
};

#endif /* PROPERTIES_SH1106BRIGHTNESS_H_ */
