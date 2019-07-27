/*
 * Sensors.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"
#include "BuildConfig.h"
#include "Sensors.h"
#include "BMP180.h"
#include "Logger/Logger.h"

Sensors::Sensors()
{
    // we use I2C on port B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }

#if USE_SLEEP_MODE_SENSORS
    // enable the peripheral in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOB);
#else
    SysCtlPeripheralSleepDisable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralDeepSleepDisable(SYSCTL_PERIPH_GPIOB);
#endif

    // tell the system to enable SSI0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C0))
    {
    }

#if USE_SLEEP_MODE_SENSORS
    // enable the peripheral in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_I2C0);
#else
    SysCtlPeripheralSleepDisable(SYSCTL_PERIPH_I2C0);
    SysCtlPeripheralDeepSleepDisable(SYSCTL_PERIPH_I2C0);
#endif

    // we change the pin's purpose
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);

    // Select the I2C function for these pins.  This function will also
    // configure the GPIO pins for I2C operation, setting them to
    // open-drain operation with weak pull-ups.  Consult the data sheet
    // to see which functions are allocated per pin.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // enable I2C0 as Master
    I2CMasterEnable(I2C0_BASE);

    // configure I2C0
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);

    SysCtlDelay(SysCtlClockGet() / 3 / 100);

    // we controls I2C slaves on port E
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE))
    {
    }

#if USE_SLEEP_MODE_SENSORS
    // enable the peripheral in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOE);
#else
    SysCtlPeripheralSleepDisable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralDeepSleepDisable(SYSCTL_PERIPH_GPIOE);
#endif

    // set output direction on LEDs' pins
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    GPIOPinWrite(GPIO_PORTE_BASE,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
                 0);

    // setup sensors
#if USE_BMP180
    _bmp180.init(I2C0_BASE);
#elif USE_BMP280
#if USE_SENSOR_ADDR
    _bmp280s[0].init(I2C0_BASE);
    _bmp280s[1].init(I2C0_BASE, BMP280::I2C_ADDRESS_ALT);
#else
    // array of sensors shared an I2C bus
    for (uint8_t i = 0; i < 2; i++)
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, 1 << i);
        _bmp280s[i].init(I2C0_BASE);
    }
#if USE_MIDI_UART
    _bmp280s[0].setNotes(72, 74);
    _bmp280s[1].setNotes(76, 77);
#endif
#endif
#endif
}

void Sensors::readSensors()
{
#if USE_BMP180
#if PROBE_SENSOR_DATA
    Logger::getInstance().printf("%d\n\r", _bmp180.scan());
#else
    _bmp180.scan();
#endif
#elif USE_BMP280
#if PROBE_SENSOR_DATA | USE_MIDI_UART
#if USE_SENSOR_ADDR
    Logger::getInstance().printf("%d %d %d %d %d %d\n\r",
            _bmp280s[0].getMin(),
            _bmp280s[0].getMax(),
            _bmp280s[0].scan(),
            _bmp280s[1].getMin(),
            _bmp280s[1].getMax(),
            _bmp280s[1].scan());
#else
    for (uint8_t i = 0; i < 2; i++)
    {
        GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1, 1 << i);
#if PROBE_SENSOR_DATA
#if USE_KALMAN_FILTER
        Logger::getInstance().printf("%d %d %d ", _bmp280s[i].scan(),
                _bmp280s[i].getRaw(),
                _bmp280s[i].getKalman());
#else
        Logger::getInstance().printf("%d ", _bmp280s[i].scan());
#endif
#else
        _bmp280s[i].scan();
#endif
    }
#if PROBE_SENSOR_DATA
    Logger::getInstance().printf("\n\r");
#endif
#endif
#else
    _bmp280a.scan();
    _bmp280b.scan();
#endif
#endif

}
