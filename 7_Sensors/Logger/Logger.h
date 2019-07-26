/*
 * Logger.h
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef LOGGER_LOGGER_H_
#define LOGGER_LOGGER_H_

#pragma once

#include <stdint.h>
#include <stdarg.h>
#include "driverlib/gpio.h"
#include "BuildConfig.h"

class Logger
{
public:
    static Logger& getInstance()
    {
        static Logger __instance;
        return __instance;
    }
#if USE_MIDI_UART
    void sendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void sendNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    void controlChange(uint8_t channel, uint8_t controller, uint8_t value);
    void setInstrument(uint8_t channel, uint8_t type);
#endif
    void print(const char *pcString);
    void println(const char *pcString);
    void printf(const char *pcString, ...);

private:
    const char g_pcHex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                               'a', 'b', 'c', 'd', 'e', 'f' };

    Logger();
    void vprintf(const char *pcString, va_list vaArgP);
    int write(const char *pcBuf, uint32_t ui32Len);
};

#endif /* LOGGER_LOGGER_H_ */
