/*
 * CpuUsage.cpp
 *
 *  Created on: Jul 10, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "BuildConfig.h"
#include "CpuUsage.h"
#include "Logger/Logger.h"

CpuUsage::CpuUsage()
{
    initCpuUsage();
}

void CpuUsage::initCpuUsage()
{
    _cpu_cycle_per_tick = SysCtlClockGet() / SYSTICKS_PER_SECOND;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER2))
    {
    }

    // Enable the timer while the processor is in run mode, but disable
    // it in sleep mode.  It will therefore count system clocks when the
    // processor is running but not when it is sleeping.
    SysCtlPeripheralSleepDisable(SYSCTL_PERIPH_TIMER2);
    SysCtlPeripheralDeepSleepDisable(SYSCTL_PERIPH_TIMER2);

    // Configure the timer for 32-bit periodic operation.
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);

    // Set the load value for the third timer to the maximum value.
    TimerLoadSet(TIMER2_BASE, TIMER_A, 0xffffffff);

    // Enable the third timer.  It will now count the number of system clocks
    // during which the processor is executing code.
    TimerEnable(TIMER2_BASE, TIMER_A);
}

uint32_t CpuUsage::getCurrentUsage()
{
    static uint32_t __last_value = 0xffffffff;
    uint32_t value, usage;

    // Get the current value of the timer.
    value = TimerValueGet(TIMER2_BASE, TIMER_A);

    // Calculate usage based on last_value and current value of between each SYSTICK
    // this method will not return correct value if timer is reloaded, but this fault cycle is very short
    usage = ((((__last_value - value) * 6400) / _cpu_cycle_per_tick) >> 6);

    __last_value = value;

    return usage;
}
