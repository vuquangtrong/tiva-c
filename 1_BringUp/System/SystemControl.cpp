/*
 * SystemSettings.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "SystemControl.h"
#include "InterruptHandler.h"

void InitSystem()
{
    // set clock at 80MHz, using MAIN XTAL 16MHz
    SysCtlClockSet(
    SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    // set SYSTICK
    SysTickPeriodSet(SysCtlClockGet() / SYSTICKS_PER_SECOND);
    SysTickIntRegister(SysTickInterruptHandler);
    SysTickIntEnable();
    SysTickEnable();
}

void DisableInterrupts()
{
    __asm(" CPSID  I\n");
}

void EnableInterrupts()
{
    __asm(" CPSIE  I\n");
}

void WaitForInterrupt()
{
    __asm(" WFI\n");
}

