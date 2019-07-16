/*
 * Logger.cpp
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include <stdarg.h>
#include "inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "BuildConfig.h"
#include "Logger.h"

Logger::Logger()
{
    initLogger();
}

void Logger::initLogger()
{
    static bool __isLoggerInitialized = false;

    if (__isLoggerInitialized)
    {
        return;
    }

    // UART0 is on port A, must enable port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }

    // we change the pin's purpose
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // tell the system to enable UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0))
    {
    }

    // enable the peripheral in sleep mode
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_UART0);

    // setup UART0 clock, baud rate, data structure
    UARTConfigSetExpClk(
            UART0_BASE, SysCtlClockGet(), 115200,
            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);

    // turn it on
    UARTEnable(UART0_BASE);

    __isLoggerInitialized = true;
}

void Logger::print(const char *pcString)
{
    //
    // Check the arguments.
    //
    if (pcString == 0)
    {
        return;
    }

    while (*pcString)
    {
        UARTCharPut(UART0_BASE, *pcString);
        pcString++;
    }
}

void Logger::println(const char *pcString)
{
    //
    // Check the arguments.
    //
    if (pcString == 0)
    {
        return;
    }

    print(pcString);
    UARTCharPut(UART0_BASE, '\n');
    UARTCharPut(UART0_BASE, '\r');
}

void Logger::printf(const char *pcString, ...)
{
    va_list vaArgP;

    //
    // Start the varargs processing.
    //
    va_start(vaArgP, pcString);

    vprintf(pcString, vaArgP);

    //
    // We're finished with the varargs now.
    //
    va_end(vaArgP);
}

void Logger::vprintf(const char *pcString, va_list vaArgP)
{
    uint32_t ui32Idx, ui32Value, ui32Pos, ui32Count, ui32Base, ui32Neg;
    char *pcStr, pcBuf[16], cFill;

    //
    // Check the arguments.
    //
    if (pcString == 0)
    {
        return;
    }

    //
    // Loop while there are more characters in the string.
    //
    while (*pcString)
    {
        //
        // Find the first non-% character, or the end of the string.
        //
        for (ui32Idx = 0;
                (pcString[ui32Idx] != '%') && (pcString[ui32Idx] != '\0');
                ui32Idx++)
        {
        }

        //
        // Write this portion of the string.
        //
        write(pcString, ui32Idx);

        //
        // Skip the portion of the string that was written.
        //
        pcString += ui32Idx;

        //
        // See if the next character is a %.
        //
        if (*pcString == '%')
        {
            //
            // Skip the %.
            //
            pcString++;

            //
            // Set the digit count to zero, and the fill character to space
            // (in other words, to the defaults).
            //
            ui32Count = 0;
            cFill = ' ';

            //
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
            //
            again:

            //
            // Determine how to handle the next character.
            //
            switch (*pcString++)
            {
            //
            // Handle the digit characters.
            //
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                //
                // If this is a zero, and it is the first digit, then the
                // fill character is a zero instead of a space.
                //
                if ((pcString[-1] == '0') && (ui32Count == 0))
                {
                    cFill = '0';
                }

                //
                // Update the digit count.
                //
                ui32Count *= 10;
                ui32Count += pcString[-1] - '0';

                //
                // Get the next character.
                //
                goto again;
            }

                //
                // Handle the %c command.
                //
            case 'c':
            {
                //
                // Get the value from the varargs.
                //
                ui32Value = va_arg(vaArgP, uint32_t);

                //
                // Print out the character.
                //
                write((char *) &ui32Value, 1);

                //
                // This command has been handled.
                //
                break;
            }

                //
                // Handle the %d and %i commands.
                //
            case 'd':
            case 'i':
            {
                //
                // Get the value from the varargs.
                //
                ui32Value = va_arg(vaArgP, uint32_t);

                //
                // Reset the buffer position.
                //
                ui32Pos = 0;

                //
                // If the value is negative, make it positive and indicate
                // that a minus sign is needed.
                //
                if ((int32_t) ui32Value < 0)
                {
                    //
                    // Make the value positive.
                    //
                    ui32Value = -(int32_t) ui32Value;

                    //
                    // Indicate that the value is negative.
                    //
                    ui32Neg = 1;
                }
                else
                {
                    //
                    // Indicate that the value is positive so that a minus
                    // sign isn't inserted.
                    //
                    ui32Neg = 0;
                }

                //
                // Set the base to 10.
                //
                ui32Base = 10;

                //
                // Convert the value to ASCII.
                //
                goto convert;
            }

                //
                // Handle the %s command.
                //
            case 's':
            {
                //
                // Get the string pointer from the varargs.
                //
                pcStr = va_arg(vaArgP, char *);

                //
                // Determine the length of the string.
                //
                for (ui32Idx = 0; pcStr[ui32Idx] != '\0'; ui32Idx++)
                {
                }

                //
                // Write the string.
                //
                write(pcStr, ui32Idx);

                //
                // Write any required padding spaces
                //
                if (ui32Count > ui32Idx)
                {
                    ui32Count -= ui32Idx;
                    while (ui32Count--)
                    {
                        write(" ", 1);
                    }
                }

                //
                // This command has been handled.
                //
                break;
            }

                //
                // Handle the %u command.
                //
            case 'u':
            {
                //
                // Get the value from the varargs.
                //
                ui32Value = va_arg(vaArgP, uint32_t);

                //
                // Reset the buffer position.
                //
                ui32Pos = 0;

                //
                // Set the base to 10.
                //
                ui32Base = 10;

                //
                // Indicate that the value is positive so that a minus sign
                // isn't inserted.
                //
                ui32Neg = 0;

                //
                // Convert the value to ASCII.
                //
                goto convert;
            }

                //
                // Handle the %x and %X commands.  Note that they are treated
                // identically; in other words, %X will use lower case letters
                // for a-f instead of the upper case letters it should use.  We
                // also alias %p to %x.
                //
            case 'x':
            case 'X':
            case 'p':
            {
                //
                // Get the value from the varargs.
                //
                ui32Value = va_arg(vaArgP, uint32_t);

                //
                // Reset the buffer position.
                //
                ui32Pos = 0;

                //
                // Set the base to 16.
                //
                ui32Base = 16;

                //
                // Indicate that the value is positive so that a minus sign
                // isn't inserted.
                //
                ui32Neg = 0;

                //
                // Determine the number of digits in the string version of
                // the value.
                //
                convert: for (ui32Idx = 1;
                        (((ui32Idx * ui32Base) <= ui32Value)
                                && (((ui32Idx * ui32Base) / ui32Base) == ui32Idx));
                        ui32Idx *= ui32Base, ui32Count--)
                {
                }

                //
                // If the value is negative, reduce the count of padding
                // characters needed.
                //
                if (ui32Neg)
                {
                    ui32Count--;
                }

                //
                // If the value is negative and the value is padded with
                // zeros, then place the minus sign before the padding.
                //
                if (ui32Neg && (cFill == '0'))
                {
                    //
                    // Place the minus sign in the output buffer.
                    //
                    pcBuf[ui32Pos++] = '-';

                    //
                    // The minus sign has been placed, so turn off the
                    // negative flag.
                    //
                    ui32Neg = 0;
                }

                //
                // Provide additional padding at the beginning of the
                // string conversion if needed.
                //
                if ((ui32Count > 1) && (ui32Count < 16))
                {
                    for (ui32Count--; ui32Count; ui32Count--)
                    {
                        pcBuf[ui32Pos++] = cFill;
                    }
                }

                //
                // If the value is negative, then place the minus sign
                // before the number.
                //
                if (ui32Neg)
                {
                    //
                    // Place the minus sign in the output buffer.
                    //
                    pcBuf[ui32Pos++] = '-';
                }

                //
                // Convert the value into a string.
                //
                for (; ui32Idx; ui32Idx /= ui32Base)
                {
                    pcBuf[ui32Pos++] =
                            g_pcHex[(ui32Value / ui32Idx) % ui32Base];
                }

                //
                // Write the string.
                //
                write(pcBuf, ui32Pos);

                //
                // This command has been handled.
                //
                break;
            }

                //
                // Handle the %% command.
                //
            case '%':
            {
                //
                // Simply write a single %.
                //
                write(pcString - 1, 1);

                //
                // This command has been handled.
                //
                break;
            }

                //
                // Handle all other commands.
                //
            default:
            {
                //
                // Indicate an error.
                //
                write("ERROR", 5);

                //
                // This command has been handled.
                //
                break;
            }
            }
        }
    }
}

int Logger::write(const char *pcBuf, uint32_t ui32Len)
{
    unsigned int uIdx;

    //
    // Check for valid arguments.
    //
    if (pcBuf == 0)
    {
        return -1;
    }

    //
    // Send the characters
    //
    for (uIdx = 0; uIdx < ui32Len; uIdx++)
    {
        //
        // If the character to the UART is \n, then add a \r before it so that
        // \n is translated to \n\r in the output.
        //
        if (pcBuf[uIdx] == '\n')
        {
            UARTCharPut(UART0_BASE, '\r');
        }

        //
        // Send the character to the UART output.
        //
        UARTCharPut(UART0_BASE, pcBuf[uIdx]);
    }

    //
    // Return the number of characters written.
    //
    return (uIdx);
}
