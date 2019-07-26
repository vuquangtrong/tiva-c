/*
 * Sensors.h
 *
 *  Created on: Jul 24, 2019
 *      Author: vqtrong
 */

#ifndef SENSOR_SENSORS_H_
#define SENSOR_SENSORS_H_

#pragma once

#include <stdint.h>
#include "BuildConfig.h"

#if USE_BMP180
#include "BMP180.h"
#elif USE_BMP280
#include "BMP280.h"
#endif

class Sensors
{
public:
    static Sensors& getInstance()
    {
        static Sensors __instance;
        return __instance;
    }

    void readSensors();

private:
#if USE_BMP180
    BMP180 _bmp180;
#elif USE_BMP280
#if USE_SENSOR_ADDR
    BMP280 _bmp280a;
    BMP280 _bmp280b;
#else
    BMP280 _bmp280s[2];
#endif
#endif
    Sensors();

};

#endif /* SENSOR_SENSORS_H_ */
