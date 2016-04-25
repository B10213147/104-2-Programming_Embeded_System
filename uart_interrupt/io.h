// Date:   18.9.2013
// Author: Janez Puhan
// some standard input/output functions rewritten for student ARM board with
// LPC2138 microcontroller used in first-year programming course at the
// Faculty of Electrical Engineering, University of Ljubljana

#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

#include "init.h"
#include "adc_dac.h"
#include "pwm.h"
#include "rtos2.h"
#include <stdarg.h>

#define NULL				0x00000000
#define CLOCKS_PER_SEC		1000
#define MAX_PRINTF_STR_LEN	50

// IR settings
#define FREQUENCY_kHz	38
#define DUTY_CYCLE		0.5

// LCD cursor types
#define _NOCURSOR		0
#define _SOLIDCURSOR	1
#define _NORMALCURSOR	2

// Function declarations
extern void _LCDInit();
extern void clrscr();
extern void scroll();
extern int putch(int ch);
extern int putchar(int ch);
extern void _setcursortype(int cursor_type);
extern void gotoxy(int x, int y);
extern int puts(const char *str);
extern int printf(const char *format, ...);
extern void _KeyInit();
extern int _getkeys();
extern int kbhit();
extern int getch();
extern void _LEDInit();
extern void _setleds(int mask);
extern void _clrleds(int mask);
extern unsigned int clock();
extern void delay(unsigned int milliseconds);
extern void _ADCInit();
extern int _adconvert();
extern int _adconvertExt(int input);
extern void _DACInit();
extern void _dacwrite(int val);
extern void _setpindir(int pinid, int dir);
extern void outportp(int pinid, int value);
extern int inportp(int pinid);
extern void _IRInit();

#endif
