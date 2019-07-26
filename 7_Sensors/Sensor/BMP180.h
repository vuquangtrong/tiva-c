/*
 * BMP180.h
 *
 *  Created on: Jul 24, 2019
 *      Author: vqtrong
 */

#ifndef SENSOR_BMP180_H_
#define SENSOR_BMP180_H_

#pragma once

#include <stdint.h>
#include "BuildConfig.h"

/* BMP180 is set as ULTRA LOW POWER mode as default */

class BMP180
{
public:
    enum Settings
    {
        I2C_ADDRESS = 0x77,
        CHIP_ID = 0x55,
        REG_CALS_START = 0xAA,
        REG_CALS_END = 0xBE,
        REG_CHIP_ID = 0xD0,
        REG_SOFT_RESET = 0xE0,
        REG_CONTROL = 0xF4,
        REG_DATA = 0xF6,
        REQUEST_SOFT_RESET = 0xB6,
        REQUEST_TEMPERATURE = 0x2E,
        REQUEST_PRESSURE = 0x34, /* DEFAULT: oss = 0x00 */
    };

    enum State
    {
        IDLE, READ_TEMPURATURE, READ_PRESSURE
    };

    typedef struct
    {
        int16_t ac1;
        int16_t ac2;
        int16_t ac3;
        uint16_t ac4;
        uint16_t ac5;
        uint16_t ac6;
        int16_t b1;
        int16_t b2;
        int16_t mb;
        int16_t mc;
        int16_t md;
    } CalibrationSettings;

    BMP180();
    void init(uint32_t i2cBase);
#if PROBE_SENSOR_DATA
    int32_t scan();
#else
    void scan();
#endif
    void setOSS(uint8_t oss);
    void requestTemperature();
    int32_t getTemperature();
    void requestPressure();
    int32_t getPressure();

private:
    uint32_t _i2cBase;
    State _state;
    uint8_t _state_count;
    uint8_t _calRawVals[22];
    CalibrationSettings _calVals;
    uint8_t _temperatureRawVal[2];
    uint8_t _pressureRawVal[3];
    uint8_t _oss;
    int32_t _temperature;
    int32_t _pressure;

    void getRawTemperature();
    void getRawPressure();
    void read(uint8_t reg_addr, uint8_t* result, uint8_t length);
    void write(uint8_t reg_addr, uint8_t data);
};

#endif /* SENSOR_BMP180_H_ */
