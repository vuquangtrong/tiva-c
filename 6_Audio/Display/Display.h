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
#include "Nokia5110.h"

class Display
{
public:
    static Display& getIntance()
    {
        static Display __intance;
        return __intance;
    }

    void print(uint8_t col, uint8_t row, const char *msg);
    void clear();
    void setBacklight(bool state);

private:
    Display();
    Nokia5110* _nokia5110;

};

#endif /* DISPLAY_DISPLAY_H_ */
