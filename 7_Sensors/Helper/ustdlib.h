//*****************************************************************************
//
// ustdlib.h - Prototypes for simple standard library functions.
//
// Copyright (c) 2007-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the Tiva Utility Library.
//
//*****************************************************************************

#ifndef __USTDLIB_H__
#define __USTDLIB_H__

//*****************************************************************************
//
// Include the standard C headers upon which these replacements are based.
//
//*****************************************************************************
#include <stdarg.h>
#include <time.h>

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
void ulocaltime(time_t timer, struct tm *tm);
time_t umktime(struct tm *timeptr);
int urand(void);
int usnprintf(char * restrict s, size_t n, const char * restrict format, ...);
int usprintf(char * restrict s, const char * restrict format, ...);
void usrand(unsigned int seed);
int ustrcasecmp(const char *s1, const char *s2);
int ustrcmp(const char *s1, const char *s2);
size_t ustrlen(const char *s);
int ustrncasecmp(const char *s1, const char *s2, size_t n);
int ustrncmp(const char *s1, const char *s2, size_t n);
char *ustrncpy(char * restrict s1, const char * restrict s2, size_t n);
char *ustrstr(const char *s1, const char *s2);
float ustrtof(const char * restrict nptr, const char ** restrict endptr);
unsigned long int ustrtoul(const char * restrict nptr, const char ** restrict endptr, int base);
int uvsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg);

#endif // __USTDLIB_H__
