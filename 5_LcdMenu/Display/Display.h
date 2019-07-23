/*
 * Display.h
 *
 *  Created on: Jul 11, 2019
 *      Author: vqtrong
 */

#ifndef DISPLAY_DISPLAY_H_
#define DISPLAY_DISPLAY_H_

#pragma once

#include <stdint.h>
#include "BuildConfig.h"
#include "DisplayInterface.h"

class Display
{
public:
    static Display& getInstance()
    {
        static Display __instance;
        return __instance;
    }

    void print(uint8_t col, uint8_t row, const char *msg);
    void clear();
    void setBacklight(bool state);
    void setBrightness(uint8_t val);

private:
    Display();
    DisplayInterface* _display;

};

#endif /* DISPLAY_DISPLAY_H_ */
