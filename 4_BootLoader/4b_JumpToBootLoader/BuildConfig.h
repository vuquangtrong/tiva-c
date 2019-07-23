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

#define HW_VERSION "HW: 0.00"
#define SW_VERSION "SW: 0.44"
// SysTick timer is a 24 bits, therefore, the minimum heart-beat rate is 5 Hz when running at 80MHz
// (80MHz / (2^24)) = 5
#define SYSTICKS_PER_SECOND 32
#define USE_SLEEP_MODE 1

#endif /* BUILDCONFIG_H_ */
