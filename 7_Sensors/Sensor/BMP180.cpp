/*
 * BMP180.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "BuildConfig.h"
#include "BMP180.h"
#include "Logger/Logger.h"

BMP180::BMP180()
{
    _i2cBase = 0;
    _state = IDLE;
    _state_count = 0;
    _oss = 0;
    _temperature = 0;
    _pressure = 0;
}

void BMP180::init(uint32_t i2cBase)
{
    _i2cBase = i2cBase;

    if (_i2cBase != 0)
    {
        uint8_t id;
        read(REG_CHIP_ID, &id, 1);
        Logger::getInstance().printf("id = 0x%x\n\r", id);

        if (id == CHIP_ID)
        {
            Logger::getInstance().println("Found BMP180");

            // read coefficients
            volatile uint8_t i;
            for (i = 0; i < 11; i++)
            {
                read(REG_CALS_START + 2 * i, &_calRawVals[2 * i], 2);
            }

            _calVals.ac1 = (int16_t) ((((int16_t) _calRawVals[0]) << 8)
                    | ((int16_t) _calRawVals[1]));
            Logger::getInstance().printf("%d\n\r", _calVals.ac1);
            _calVals.ac2 = (int16_t) ((((int16_t) _calRawVals[2]) << 8)
                    | ((int16_t) _calRawVals[3]));
            Logger::getInstance().printf("%d\n\r", _calVals.ac2);
            _calVals.ac3 = (int16_t) ((((int16_t) _calRawVals[4]) << 8)
                    | ((int16_t) _calRawVals[5]));
            Logger::getInstance().printf("%d\n\r", _calVals.ac3);
            _calVals.ac4 = (uint16_t) ((((int16_t) _calRawVals[6]) << 8)
                    | ((int16_t) _calRawVals[7]));
            Logger::getInstance().printf("%d\n\r", _calVals.ac4);
            _calVals.ac5 = (uint16_t) ((((int16_t) _calRawVals[8]) << 8)
                    | ((int16_t) _calRawVals[9]));
            Logger::getInstance().printf("%d\n\r", _calVals.ac5);
            _calVals.ac6 = (uint16_t) ((((int16_t) _calRawVals[10]) << 8)
                    | ((int16_t) _calRawVals[11]));
            Logger::getInstance().printf("%d\n\r", _calVals.ac6);
            _calVals.b1 = (int16_t) ((((int16_t) _calRawVals[12]) << 8)
                    | ((int16_t) _calRawVals[13]));
            Logger::getInstance().printf("%d\n\r", _calVals.b1);
            _calVals.b2 = (int16_t) ((((int16_t) _calRawVals[14]) << 8)
                    | ((int16_t) _calRawVals[15]));
            Logger::getInstance().printf("%d\n\r", _calVals.b2);
            _calVals.mb = (int16_t) ((((int16_t) _calRawVals[16]) << 8)
                    | ((int16_t) _calRawVals[17]));
            Logger::getInstance().printf("%d\n\r", _calVals.mb);
            _calVals.mc = (int16_t) ((((int16_t) _calRawVals[18]) << 8)
                    | ((int16_t) _calRawVals[19]));
            Logger::getInstance().printf("%d\n\r", _calVals.mc);
            _calVals.md = (int16_t) ((((int16_t) _calRawVals[20]) << 8)
                    | ((int16_t) _calRawVals[21]));
            Logger::getInstance().printf("%d\n\r", _calVals.md);

            write(REG_SOFT_RESET, REQUEST_SOFT_RESET);
            SysCtlDelay(80000000 / 3 / 10);
            Logger::getInstance().println("reset done!");
        }
        else
        {
            Logger::getInstance().println("No BMP180 detected!");
            _i2cBase = 0;
        }
    }
}

void BMP180::setOSS(uint8_t oss)
{
    if (oss > 0x03)
    {
        _oss = 0x03;
    }
    else
    {
        _oss = oss;
    }
}

void BMP180::read(uint8_t reg_addr, uint8_t* result, uint8_t length)
{
    if (_i2cBase != 0 && result != 0 && length > 0)
    {
        // write mode, set slave address
        I2CMasterSlaveAddrSet(_i2cBase, I2C_ADDRESS, false);

        // set slave register
        I2CMasterDataPut(_i2cBase, reg_addr);

        // send request
        I2CMasterControl(_i2cBase, I2C_MASTER_CMD_SINGLE_SEND);
        while (I2CMasterBusy(_i2cBase))
        {
        }

        // change to read mode
        I2CMasterSlaveAddrSet(_i2cBase, I2C_ADDRESS, true);

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
            result[i] = I2CMasterDataGet(_i2cBase);
        }
    }
}

void BMP180::write(uint8_t reg_addr, uint8_t data)
{
    if (_i2cBase != 0)
    {
        // write mode, set slave address
        I2CMasterSlaveAddrSet(_i2cBase, I2C_ADDRESS, false);

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

void BMP180::requestTemperature()
{
    write(REG_CONTROL, REQUEST_TEMPERATURE);
}

void BMP180::getRawTemperature()
{
    read(REG_DATA, &_temperatureRawVal[0], 2);
}

int32_t BMP180::getTemperature()
{
    // Calculate UT
    int32_t UT = (int32_t) ((((int32_t) _temperatureRawVal[0]) << 8)
            + ((int32_t) _temperatureRawVal[1]));

    // Calculate X1
    int32_t X1 = (UT - (int32_t) _calVals.ac6) * ((int32_t) _calVals.ac5)
            / 32768;

    // Calculate X2
    int32_t X2 = ((int32_t) _calVals.mc * 2048) / (X1 + (int32_t) _calVals.md);

    // Calculate B5
    int32_t B5 = X1 + X2;

    return ((B5 + 8) / 16);
}

void BMP180::requestPressure()
{
    write(REG_CONTROL, (_oss << 6) | REQUEST_PRESSURE);
}

void BMP180::getRawPressure()
{
    read(REG_DATA, &_pressureRawVal[0], 3);
}

int32_t BMP180::getPressure()
{
    // Calculate UT
    int32_t UT = (int32_t) ((((int32_t) _temperatureRawVal[0]) << 8)
            + ((int32_t) _temperatureRawVal[1]));

    // Calculate UP
    int32_t UP = ((((int32_t) _pressureRawVal[0]) << 16)
            + (((int32_t) _pressureRawVal[1]) << 8)
            + ((int32_t) _pressureRawVal[2])) >> (8 - _oss);

    // Calculate X1
    int32_t X1 = (UT - (int32_t) _calVals.ac6) * ((int32_t) _calVals.ac5)
            / 32768;

    // Calculate X2
    int32_t X2 = ((int32_t) _calVals.mc * 2048) / (X1 + (int32_t) _calVals.md);

    // Calculate B5
    int32_t B5 = X1 + X2;

    // Calculate B6
    int32_t B6 = B5 - 4000;

    // Recalculate X1
    X1 = ((int32_t) _calVals.b2 * ((B6 * B6) / 4096)) / 2048;

    // Recalculate X2
    X2 = (int32_t) _calVals.ac2 * B6 / 2048;

    // Calculate X3
    int32_t X3 = X1 + X2;

    // Calculate B3
    int32_t B3 = ((((int32_t) _calVals.ac1 * 4 + X3)) + 2) / 4;

    // Recalculate X1
    X1 = (int32_t) _calVals.ac3 * B6 / 8192;

    // Recalculate X2
    X2 = ((int32_t) _calVals.b1 * ((B6 * B6) / 4096)) / 65536;

    // Recalculate X3
    X3 = ((X1 + X2) + 2) / 4;

    // Calculate B4
    uint32_t B4 = (uint32_t) _calVals.ac4 * (uint32_t) (X3 + 32768) / 32768;

    // Calculate B7
    uint32_t B7 = ((uint32_t) UP - B3) * (50000);

    // Calculate p
    int32_t p;
    if (B7 < 0x80000000)
    {
        p = (B7 * 2) / B4;
    }
    else
    {
        p = (B7 / B4) * 2;
    }

    // Recalculate X1
    X1 = (p / 256) * (p / 256);
    X1 = (X1 * 3038) / 65536;

    // Recalculate X2
    X2 = (-7357 * p) / 65536;

    // Recalculate p
    p = p + (X1 + X2 + 3791) / 16;

    return p;
}

#if PROBE_SENSOR_DATA
int32_t BMP180::scan()
#else
void BMP180::scan()
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
        _pressure = getPressure();
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
    return _pressure;
#endif
}
