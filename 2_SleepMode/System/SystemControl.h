/*
 * SystemSettings.h
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#ifndef SYSTEM_SYSTEMCONTROL_H_
#define SYSTEM_SYSTEMCONTROL_H_

#pragma once

void InitSystem();
void DisableInterrupts();
void EnableInterrupts();
void WaitForInterrupt();

#endif /* SYSTEM_SYSTEMCONTROL_H_ */
