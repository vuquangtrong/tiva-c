/*
 * BMP280.h
 *
 *  Created on: Jul 24, 2019
 *      Author: vqtrong
 */

#ifndef SENSOR_BMP280_H_
#define SENSOR_BMP280_H_

#pragma once

#include <stdint.h>
#include "BuildConfig.h"
#if USE_KALMAN_FILTER
#include "SimpleKalmanFilter/SimpleKalmanFilter.h"
#endif
/* BMP280 is set as ULTRA LOW POWER mode as default */

class BMP280
{
public:
    enum Settings
    {
        I2C_ADDRESS = 0x76,
        I2C_ADDRESS_ALT = 0x77,
        CHIP_ID = 0x58,
        REG_CALS_START = 0x88,
        REG_CALS_END = 0xBE,
        REG_CHIP_ID = 0xD0,
        REG_SOFT_RESET = 0xE0,
        REG_STATUS = 0xF3,
        REG_CONTROL_MEASUREMENT = 0xF4, /* T_Sampling << 5 | P_Sampling << 2 | Mode */
        REG_CONFIG = 0xF5, /* Standby << 5 | Filter << 2 |  Spi3w */
        REG_PRESSURE_DATA = 0xF7,
        REG_TEMPUERATURE_DATA = 0xFA,
        REQUEST_SOFT_RESET = 0xB6,
        FILTER_BUFFER = 5,
        THRESHOLD = 1200
    };

    enum T_Sampling
    {
        T_SAMPLING_NONE = 0x00,
        T_SAMPLING_X1 = 0x01,
        T_SAMPLING_X2 = 0x02,
        T_SAMPLING_X4 = 0x03,
        T_SAMPLING_X8 = 0x04,
        T_SAMPLING_X16 = 0x05
    };

    enum P_Sampling
    {
        P_SAMPLING_NONE = 0x00,
        P_SAMPLING_X1 = 0x01,
        P_SAMPLING_X2 = 0x02,
        P_SAMPLING_X4 = 0x03,
        P_SAMPLING_X8 = 0x04,
        P_SAMPLING_X16 = 0x05
    };

    enum Mode
    {
        MODE_SLEEP = 0x00, MODE_FORCED = 0x01, MODE_NORMAL = 0x03
    };

    enum Filter
    {
        FILTER_OFF = 0x00,
        FILTER_X2 = 0x01,
        FILTER_X4 = 0x02,
        FILTER_X8 = 0x03,
        FILTER_X16 = 0x04
    };

    enum Standby
    {
        STANDBY_MS_1 = 0x00,
        STANDBY_MS_63 = 0x01,
        STANDBY_MS_125 = 0x02,
        STANDBY_MS_250 = 0x03,
        STANDBY_MS_500 = 0x04,
        STANDBY_MS_1000 = 0x05,
        STANDBY_MS_2000 = 0x06,
        STANDBY_MS_4000 = 0x07
    };

    enum Spi3w
    {
        SPI3W_DISABLED = 0x00, SPI3W_ENABLED = 0x01
    };

    enum State
    {
        IDLE, READ_TEMPURATURE, READ_PRESSURE
    };

    typedef struct
    {
        uint16_t dig_T1;
        int16_t dig_T2;
        int16_t dig_T3;
        uint16_t dig_P1;
        int16_t dig_P2;
        int16_t dig_P3;
        int16_t dig_P4;
        int16_t dig_P5;
        int16_t dig_P6;
        int16_t dig_P7;
        int16_t dig_P8;
        int16_t dig_P9;
    } CalibrationSettings;

    BMP280();
    void init(uint32_t i2cBase, uint8_t addr = I2C_ADDRESS);
#if PROBE_SENSOR_DATA
    int32_t scan();
#else
    void scan();
#endif
    void setControl(T_Sampling tsam, P_Sampling psam, Mode mode);
    void setConfig(Standby stb, Filter filter, Spi3w spi3w);
    void requestTemperature();
    int32_t getTemperature();
    void requestPressure();
    int32_t getPressure();
#if USE_MIDI_UART
    void setNotes(uint8_t blow, uint8_t draw);
#endif
    int32_t getFiltered()
    {
        return _pressureFiltered;
    }
    uint32_t getRaw()
    {
        return _pressureRaw;
    }
#if USE_KALMAN_FILTER
    int32_t getKalman();
#endif
    int32_t getMin();
    int32_t getMax();

private:
    uint32_t _addr;
    uint32_t _i2cBase;
    State _state;
    uint8_t _state_count;
    int32_t __min;
    int32_t __max;
    int32_t __avg;
    int32_t __filter[FILTER_BUFFER];
    int32_t __filterIndex;
    int32_t __filterSum;

    int32_t __calibration_cnt;
    uint8_t _calRawVals[24];
    CalibrationSettings _calVals;
    uint8_t _temperatureRawVal[3];
    uint8_t _pressureRawVal[3];
    int32_t _t_fine;
    int32_t _temperature;
    int32_t _pressureFiltered;
    int32_t _pressureRaw;
#if USE_MIDI_UART
    uint8_t _blowNote;
    uint8_t _drawNote;
    bool __isBlowing;
    bool __isDrawing;
    void playNote(int32_t value);
#endif
#if USE_KALMAN_FILTER
    SimpleKalmanFilter _kalman_filter;
#endif

    void getRawTemperature();
    void getRawPressure();
    int32_t filter(int32_t input);

    void read(uint8_t reg_addr, uint8_t* result, uint8_t length);
    void write(uint8_t reg_addr, uint8_t data);
};

#endif /* SENSOR_BMP280_H_ */
