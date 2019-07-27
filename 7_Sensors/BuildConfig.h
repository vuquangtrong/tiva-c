/*
 * BuildConfig.h
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef BUILDCONFIG_H_
#define BUILDCONFIG_H_

#define ENABLED    (1)
#define DISABLED   (0)

#define HW_VERSION "HW: 0.00"
#define SW_VERSION "SW: 0.70"
// SysTick timer is a 24 bits, therefore, the minimum heart-beat rate is 5 Hz when running at 80MHz
// (80MHz / (2^24)) = 5
#define SYSTICKS_PER_SECOND     (100)
#define USE_SLEEP_MODE          (ENABLED)
#define USE_SLEEP_MODE_DISPLAY  (ENABLED)
#define USE_SLEEP_MODE_AUDIO    (ENABLED)
#define USE_SLEEP_MODE_SENSORS  (ENABLED)
#define USE_LCD_NOKIA5110       (DISABLED)
#define USE_OLED_SH1106         (ENABLED)
#define TEST_AUDIO              (DISABLED)
#define PROBE_AUDIO_WAVE        (DISABLED)
#define USE_BMP180              (DISABLED)
#define USE_BMP280              (ENABLED)
#define PROBE_SENSOR_DATA       (DISABLED)
#define USE_SENSOR_ADDR         (DISABLED)
#define USE_MIDI_UART           (ENABLED)
#define USE_KALMAN_FILTER       (ENABLED)
#endif /* BUILDCONFIG_H_ */
