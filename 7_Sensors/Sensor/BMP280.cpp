/*
 * BMP280.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "BuildConfig.h"
#include "BMP280.h"
#include "Logger/Logger.h"

BMP280::BMP280()
{
    _addr = I2C_ADDRESS;
    _i2cBase = 0;
    _state = IDLE;
    _state_count = 0;
    __min = 0x7FFFFFFF;
    __max = 0xFFFFFFFF;
    __avg = 0;
    __filterIndex = 0;
    __filterSum = 0;
    __calibration_cnt = 0;
#if USE_MIDI_UART
    _blowNote = 72; /* C5 */
    _drawNote = 74; /* D5 */
    __isBlowing = false;
    __isDrawing = false;
#endif
#if USE_KALMAN_FILTER
    _kalman_filter.setup(10, 10, 0.01);
#endif
}

void BMP280::init(uint32_t i2cBase, uint8_t addr)
{
    _i2cBase = i2cBase;
    _addr = addr;

    if (_i2cBase != 0 && (_addr == I2C_ADDRESS || _addr == I2C_ADDRESS_ALT))
    {
        uint8_t id;
        read(REG_CHIP_ID, &id, 1);
//        Logger::getInstance().printf("id = 0x%x\n\r", id);

        if (id == CHIP_ID)
        {
//            Logger::getInstance().printf("Found BMP280 at addr: 0x%x\n\r", _addr);

// read coefficients
            volatile uint8_t i;
            for (i = 0; i < 12; i++)
            {
                read(REG_CALS_START + 2 * i, &_calRawVals[2 * i], 2);
            }

            // BMP280 uses LE format
            _calVals.dig_T1 = (uint16_t) ((((uint16_t) _calRawVals[1]) << 8)
                    | ((uint16_t) _calRawVals[0]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_T1);
            _calVals.dig_T2 = (int16_t) ((((uint16_t) _calRawVals[3]) << 8)
                    | ((uint16_t) _calRawVals[2]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_T2);
            _calVals.dig_T3 = (int16_t) ((((uint16_t) _calRawVals[5]) << 8)
                    | ((uint16_t) _calRawVals[4]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_T3);
            _calVals.dig_P1 = (uint16_t) ((((uint16_t) _calRawVals[7]) << 8)
                    | ((uint16_t) _calRawVals[6]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P1);
            _calVals.dig_P2 = (int16_t) ((((uint16_t) _calRawVals[9]) << 8)
                    | ((uint16_t) _calRawVals[8]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P2);
            _calVals.dig_P3 = (int16_t) ((((uint16_t) _calRawVals[11]) << 8)
                    | ((uint16_t) _calRawVals[10]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P3);
            _calVals.dig_P4 = (int16_t) ((((uint16_t) _calRawVals[13]) << 8)
                    | ((uint16_t) _calRawVals[12]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P4);
            _calVals.dig_P5 = (int16_t) ((((uint16_t) _calRawVals[15]) << 8)
                    | ((uint16_t) _calRawVals[14]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P5);
            _calVals.dig_P6 = (int16_t) ((((uint16_t) _calRawVals[17]) << 8)
                    | ((uint16_t) _calRawVals[16]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P6);
            _calVals.dig_P7 = (int16_t) ((((uint16_t) _calRawVals[19]) << 8)
                    | ((uint16_t) _calRawVals[18]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P7);
            _calVals.dig_P8 = (int16_t) ((((uint16_t) _calRawVals[21]) << 8)
                    | ((uint16_t) _calRawVals[20]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P8);
            _calVals.dig_P9 = (int16_t) ((((uint16_t) _calRawVals[23]) << 8)
                    | ((uint16_t) _calRawVals[22]));
//            Logger::getInstance().printf("%d\n\r", _calVals.dig_P9);

            write(REG_SOFT_RESET, REQUEST_SOFT_RESET);
            SysCtlDelay(80000000 / 3 / 10);
//            Logger::getInstance().println("reset done!");

            setControl(T_SAMPLING_X1, P_SAMPLING_X1, MODE_NORMAL);
            setConfig(STANDBY_MS_1, FILTER_OFF, SPI3W_DISABLED);
        }
        else
        {
            Logger::getInstance().println("No BMP280 detected!");
            _i2cBase = 0;
        }
    }
}

void BMP280::setControl(T_Sampling tsam, P_Sampling psam, Mode mode)
{
    write(REG_CONTROL_MEASUREMENT, tsam << 5 | psam << 2 | mode);
}

void BMP280::setConfig(Standby stb, Filter filter, Spi3w spi3w)
{
    write(REG_CONFIG, stb << 5 | filter << 2 | spi3w);
}

void BMP280::read(uint8_t reg_addr, uint8_t* result, uint8_t length)
{
    if (_i2cBase != 0 && result != 0 && length > 0)
    {
        // write mode, set slave address
        I2CMasterSlaveAddrSet(_i2cBase, _addr, false);

        // set slave register
        I2CMasterDataPut(_i2cBase, reg_addr);

        // send request
        I2CMasterControl(_i2cBase, I2C_MASTER_CMD_SINGLE_SEND);
        while (I2CMasterBusy(_i2cBase))
        {
        }

        // change to read mode
        I2CMasterSlaveAddrSet(_i2cBase, _addr, true);

        // read one or a number of bytes
        for (int i = 0; i < length; i++)
        {
            if (length == 1)
            {
                I2CMasterControl(_i2cBase, I2C_MASTER_CMD_SINGLE_RECEIVE);
            }
            else
            {
                if (i == 0)
                {
                    I2CMasterControl(_i2cBase,
                    I2C_MASTER_CMD_BURST_RECEIVE_START);
                }
                else if (i == length - 1)
                {
                    I2CMasterControl(_i2cBase,
                    I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
                }
                else
                {
                    I2CMasterControl(_i2cBase,
                    I2C_MASTER_CMD_BURST_RECEIVE_CONT);
                }
            }
            while (I2CMasterBusy(_i2cBase))
            {
            }
            result[i] = (I2CMasterDataGet(_i2cBase) & 0xFF);
        }
    }
}

void BMP280::write(uint8_t reg_addr, uint8_t data)
{
    if (_i2cBase != 0)
    {
        // write mode, set slave address
        I2CMasterSlaveAddrSet(_i2cBase, _addr, false);

        // set slave register
        I2CMasterDataPut(_i2cBase, reg_addr);

        // send request
        I2CMasterControl(_i2cBase, I2C_MASTER_CMD_BURST_SEND_START);
        while (I2CMasterBusy(_i2cBase))
        {
        }

        // write data
        I2CMasterDataPut(_i2cBase, data);

        I2CMasterControl(_i2cBase, I2C_MASTER_CMD_BURST_SEND_FINISH);
        while (I2CMasterBusy(_i2cBase))
        {
        }
    }
}

void BMP280::requestTemperature()
{
    // normal mode, no need to request
}

void BMP280::getRawTemperature()
{
    read(REG_TEMPUERATURE_DATA, &_temperatureRawVal[0], 3);
}

int32_t BMP280::getTemperature()
{
    int32_t var1, var2;

    int32_t adc_T = (int32_t) ((((int32_t) _temperatureRawVal[0]) << 16)
            + (((int32_t) _temperatureRawVal[1]) << 8)
            + ((int32_t) _temperatureRawVal[2]));
    adc_T >>= 4;

    var1 = ((((adc_T >> 3) - ((int32_t) _calVals.dig_T1 << 1)))
            * ((int32_t) _calVals.dig_T2)) >> 11;

    var2 = (((((adc_T >> 4) - ((int32_t) _calVals.dig_T1))
            * ((adc_T >> 4) - ((int32_t) _calVals.dig_T1))) >> 12)
            * ((int32_t) _calVals.dig_T3)) >> 14;

    _t_fine = var1 + var2;

    int32_t T = (_t_fine * 5 + 128) >> 8;
    return T;
}

void BMP280::requestPressure()
{
    // normal mode, no need to request
}

void BMP280::getRawPressure()
{
    read(REG_PRESSURE_DATA, &_pressureRawVal[0], 3);
}

int32_t BMP280::getPressure()
{
    int64_t var1, var2, p;

//    // Must be done first to get the t_fine variable set up
//    getTemperature(); // done in state READ_TEMPERATURE

    int32_t adc_P = ((((int32_t) _pressureRawVal[0]) << 16)
            + (((int32_t) _pressureRawVal[1]) << 8)
            + ((int32_t) _pressureRawVal[2]));
    adc_P >>= 4;

    var1 = ((int64_t) _t_fine) - 128000;
    var2 = var1 * var1 * (int64_t) _calVals.dig_P6;
    var2 = var2 + ((var1 * (int64_t) _calVals.dig_P5) << 17);
    var2 = var2 + (((int64_t) _calVals.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t) _calVals.dig_P3) >> 8)
            + ((var1 * (int64_t) _calVals.dig_P2) << 12);
    var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) _calVals.dig_P1) >> 33;

    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t) _calVals.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t) _calVals.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t) _calVals.dig_P7) << 4);
    return (int32_t) p / 256;;
}
#if PROBE_SENSOR_DATA
int32_t BMP280::scan()
#else
void BMP280::scan()
#endif
{
    switch (_state)
    {
    case IDLE: // start here, but it will never go to idle again
        requestTemperature();
        _state = READ_TEMPURATURE;
        break;

    case READ_TEMPURATURE:
        getRawTemperature();
        _temperature = getTemperature();
        requestPressure();
        _state = READ_PRESSURE;
        break;

    case READ_PRESSURE:
        getRawPressure();
        _pressureRaw = getPressure();
        _pressureFiltered = filter(_pressureRaw);
        if (_state_count == SYSTICKS_PER_SECOND)
        {
            _state_count = 0;
            requestTemperature();
            _state = READ_TEMPURATURE;
        }
        else
        {
            _state_count++;
            requestPressure();
            _state = READ_PRESSURE;
        }
        break;
    }

#if PROBE_SENSOR_DATA
    return _pressureFiltered;
#endif
#if USE_MIDI_UART
    playNote(_pressureFiltered);
#endif
}

#if USE_KALMAN_FILTER
int32_t BMP280::getKalman()
{
    return _kalman_filter.updateEstimate(_pressureRaw);
}
#endif

int32_t BMP280::getMin()
{
    return __min;
}

int32_t BMP280::getMax()
{
    return __max;
}

int32_t BMP280::filter(int32_t input)
{
    int32_t output;

    if (__calibration_cnt <= 1000)
    {
        if (input < __min)
        {
            __min = input;
        }

        if (input > __max)
        {
            __max = input;
        }
        __avg = (__max + __min) / 2;

        __filterSum = __filterSum - __filter[__filterIndex];
        __filter[__filterIndex] = __avg;
        __filterSum = __filterSum + __filter[__filterIndex];

        __filterIndex++;
        if (__filterIndex == FILTER_BUFFER)
        {
            __filterIndex = 0;
        }

        __calibration_cnt++;
        output = __avg;
#if USE_MIDI_UART
        if (__calibration_cnt == 1000)
        {
            Logger::getInstance().setInstrument(0, 22);
        }
#endif
    }
    else
    {
#if USE_KALMAN_FILTER
        output = _kalman_filter.updateEstimate(input);
#else
        __filterSum = __filterSum - __filter[__filterIndex];

        if (input > __max || input < __min)
        {
            __filter[__filterIndex] = input;
        }
        else
        {
            __filter[__filterIndex] = __avg;
        }

        __filterSum = __filterSum + __filter[__filterIndex];

        __filterIndex++;
        if (__filterIndex == FILTER_BUFFER)
        {
            __filterIndex = 0;
        }

        if (__filterSum
                > FILTER_BUFFER * __avg + (FILTER_BUFFER / 2) * (__max - __avg))
        {
            output = __filterSum / FILTER_BUFFER;
        }
        else if (__filterSum
                < FILTER_BUFFER * __avg - (FILTER_BUFFER / 2) * (__avg - __min))
        {
            output = __filterSum / FILTER_BUFFER;
        }
        else
        {
            output = __avg;
        }
#endif
    }

    return output;
}

#if USE_MIDI_UART
void BMP280::setNotes(uint8_t blow, uint8_t draw)
{
    _blowNote = blow;
    _drawNote = draw;
}

void BMP280::playNote(int32_t value)
{
    uint8_t velocity;
    int32_t delta;

    if (value < __min)
    {
        delta = __min - value;

        if (delta >= THRESHOLD)
        {
            velocity = 0x7F;
        }
        else
        {
            velocity = (uint8_t) ((delta * 127.0f) / THRESHOLD);
        }

        if (__isDrawing)
        {
            Logger::getInstance().controlChange(0, 7, velocity);
        }
        else
        {
            Logger::getInstance().sendNoteOn(0, _drawNote, 127);
            Logger::getInstance().controlChange(0, 7, velocity);
            __isDrawing = true;
        }
    }
    else
    {
        if (__isDrawing)
        {
            Logger::getInstance().sendNoteOff(0, _drawNote, 64);
            __isDrawing = false;
        }
    }

    if (value > __max)
    {
        delta = value - __max;

        if (delta >= THRESHOLD)
        {
            velocity = 0x7F;
        }
        else
        {
            velocity = (uint8_t) ((delta * 127.0f) / THRESHOLD);
        }

        if (__isBlowing)
        {
            Logger::getInstance().controlChange(0, 7, velocity);
        }
        else
        {
            Logger::getInstance().sendNoteOn(0, _blowNote, 127);
            Logger::getInstance().controlChange(0, 7, velocity);
            __isBlowing = true;
        }
    }
    else
    {
        if (__isBlowing)
        {
            Logger::getInstance().sendNoteOff(0, _blowNote, 64);
            __isBlowing = false;
        }
    }
}
#endif
