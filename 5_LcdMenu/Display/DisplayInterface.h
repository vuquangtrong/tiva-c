/*
 * DisplayInterface.h
 *
 *  Created on: Jul 22, 2019
 *      Author: vqtrong
 */

#ifndef DISPLAY_DISPLAYINTERFACE_H_
#define DISPLAY_DISPLAYINTERFACE_H_

#pragma once

#include <stdint.h>

class DisplayInterface
{
public:
    virtual void print(uint8_t col, uint8_t row, const char *msg) = 0;
    virtual void clear() = 0;
    virtual void setBacklight(bool on) = 0;
    virtual void setBrightness(uint8_t val) = 0;
};

#endif /* DISPLAY_DISPLAYINTERFACE_H_ */
