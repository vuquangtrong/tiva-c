/*
 * SystemSettings.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */
#define TARGET_IS_TM4C123_RB1

#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/rom.h"
#include "SystemControl.h"
#include "InterruptHandler.h"
#include "Utils/CpuUsage.h"

void InitSystem()
{
    // set clock at 80MHz, using MAIN XTAL 16MHz
    SysCtlClockSet(
    SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

#if USE_SLEEP_MODE
    // Enable peripherals to operate when CPU is in sleep.
    SysCtlPeripheralClockGating(true);
#endif

    // set SYSTICK
    SysTickPeriodSet(SysCtlClockGet() / SYSTICKS_PER_SECOND);
    SysTickIntRegister(SysTickInterruptHandler);
    SysTickIntEnable();
    SysTickEnable();
}

void JumpToUsbUpdate()
{
    // because we do not start with boot loader, therefore we have to setup USB port first
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

    // setup USB pins: USB0PD, USB0PM
    GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);

    // disable all interrupt
    ROM_IntMasterDisable();
    SysTickIntDisable();
    SysTickDisable();

    // force disable all interrupts via hw
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;

    // enable USB0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }

    // reset USB0 to default description
    SysCtlPeripheralReset(SYSCTL_PERIPH_USB0);

    // USB must use PLL
    SysCtlUSBPLLEnable();
    SysCtlDelay(SysCtlClockGet() / 3);

    // interrupt for USB
    ROM_IntMasterEnable();

    // call to Update function in ROM
    ROM_UpdateUSB(0);
}

void JumpToUpdateOnBootLoader()
{
    // because we do not start with boot loader, therefore we have to setup USB port first
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // wait for it
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

    // setup USB pins: USB0PD, USB0PM
    GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);

    // disable all interrupt
    ROM_IntMasterDisable();
    SysTickIntDisable();
    SysTickDisable();

    // force disable all interrupts via hw
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;

    // enable USB0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }

    // reset USB0 to default description
    SysCtlPeripheralReset(SYSCTL_PERIPH_USB0);

    // USB must use PLL
    SysCtlUSBPLLEnable();
    SysCtlDelay(SysCtlClockGet() / 3);

    // interrupt for USB
    ROM_IntMasterEnable();

    // Return control to the boot loader.  This is a call to the SVC
    // handler in the boot loader.
    (*((void (*)(void)) (*(uint32_t *) 0x2c)))();
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

