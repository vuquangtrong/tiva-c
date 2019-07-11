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
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

#ifdef USE_SLEEP_MODE
    // Enable peripherals to operate when CPU is in sleep.
    ROM_SysCtlPeripheralClockGating(true);
#endif

    // set SYSTICK to 30Hz for heart beat service
    ROM_SysTickPeriodSet(SysCtlClockGet() / SYSTICKS_PER_SECOND);
    SysTickIntRegister(SysTickInterruptHandler);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();

    // start CPU Usage
    CpuUsage::getIntance().init();
}

void JumpToUsbUpdate()
{
    // because we do not start with boot loader, therefore we have to setup USB port first
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // wait for it
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

    // setup USB pins: USB0PD, USB0PM
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);

    // disable all interrupt
    ROM_IntMasterDisable();
    ROM_SysTickIntDisable();
    ROM_SysTickDisable();

    // force disable all interrupts via hw
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;


    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }
    ROM_SysCtlPeripheralReset(SYSCTL_PERIPH_USB0);

    // USB must use PLL
    ROM_SysCtlUSBPLLEnable();
    ROM_SysCtlDelay(SysCtlClockGet() / 3);

    // interrupt for USB
    ROM_IntMasterEnable();

    // call to Update function in ROM
    ROM_UpdateUSB(0);
}

void JumpToUpdateOnBootLoader()
{
    // because we do not start with boot loader, therefore we have to setup USB port first
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    // wait for it
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
    {
    }

    // setup USB pins: USB0PD, USB0PM
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);

    // disable all interrupt
    ROM_IntMasterDisable();
    ROM_SysTickIntDisable();
    ROM_SysTickDisable();

    // force disable all interrupts via hw
    HWREG(NVIC_DIS0) = 0xffffffff;
    HWREG(NVIC_DIS1) = 0xffffffff;


    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    while (!ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }
    ROM_SysCtlPeripheralReset(SYSCTL_PERIPH_USB0);

    // USB must use PLL
    ROM_SysCtlUSBPLLEnable();
    ROM_SysCtlDelay(SysCtlClockGet() / 3);

    // interrupt for USB
    ROM_IntMasterEnable();

    // Return control to the boot loader.  This is a call to the SVC
    // handler in the boot loader.
    (*((void (*)(void))(*(uint32_t *)0x2c)))();
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

