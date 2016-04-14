// Date:   18.9.2013
// Author: Janez Puhan

#ifndef EXTINT_H_INCLUDED
#define EXTINT_H_INCLUDED

#include "gpio.h"

// Bit masks in PINSEL0 and PINSEL1 registers
#define P0_1_EINT0	0x0000000c
#define P0_3_EINT1	0x000000c0
#define P0_7_EINT2	0x0000c000
#define P0_9_EINT3	0x000c0000
#define P0_14_EINT1	0x20000000
#define P0_15_EINT2	0x80000000
#define P0_16_EINT0	0x00000001
#define P0_20_EINT3	0x00000300
#define P0_30_EINT3	0x20000000

// Bit masks in EXTINT register
#define EINT0		0x00000001
#define EINT1		0x00000002
#define EINT2		0x00000004
#define EINT3		0x00000008

// Bit masks in INTWAKE register
#define EXTWAKE0	0x00000001
#define EXTWAKE1	0x00000002
#define EXTWAKE2	0x00000004
#define EXTWAKE3	0x00000008
#define BODWAKE		0x00004000
#define RTCWAKE		0x00008000

// Bits in EXTMODE register
#define EXTMODE0	0x00000001
#define EXTMODE1	0x00000002
#define EXTMODE2	0x00000004
#define EXTMODE3	0x00000008

// Bits in EXTPOLAR register
#define EXTPOLAR0	0x00000001
#define EXTPOLAR1	0x00000002
#define EXTPOLAR2	0x00000004
#define EXTPOLAR3	0x00000008

// Register addresses
#define EXTINT		(*((volatile int *)0xe01fc140))	// External Interrupt Flag
#define INTWAKE		(*((volatile int *)0xe01fc144))	// Interrupt Wakeup
#define EXTMODE		(*((volatile int *)0xe01fc148))	// External Interrupt Mode
#define EXTPOLAR	(*((volatile int *)0xe01fc14c))	// External Interrupt Polarity

// Function declaration
extern void extint_init(int pins, int wake, int sensitivity, int trig);

#endif

