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

#define HW_VERSION "0.0"
#define SW_VERSION "0.3"
#define SYSTICKS_PER_SECOND 30 // for heart-beat service
                               // SysTick timer is a 24 bits, therefore, the minimum heart-beat rate is 5 Hz when running at 80MHz
                               // (80MHz / (2^24)) = 5
#define USE_SLEEP_MODE

#endif /* BUILDCONFIG_H_ */
