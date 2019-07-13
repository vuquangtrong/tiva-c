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

class Display
{
public:
    virtual void print(uint8_t col, uint8_t row, const char *msg) = 0;
    virtual void clear() = 0;
};

#endif /* DISPLAY_DISPLAY_H_ */
