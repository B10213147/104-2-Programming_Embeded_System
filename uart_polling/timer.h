// Date:   18.9.2013
// Author: Janez Puhan

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include "gpio.h"

// Bit masks in T0IR and T1IR registers
#define mr0_interrupt	0x00000001
#define mr1_interrupt	0x00000002
#define mr2_interrupt	0x00000004
#define mr3_interrupt	0x00000008
#define cr0_interrupt	0x00000010
#define cr1_interrupt	0x00000020
#define cr2_interrupt	0x00000040
#define cr3_interrupt	0x00000080

// Bit masks in T0TCR and T1TCR registers
#define counter_enable	0x00000001
#define counter_reset	0x00000002

// Bit masks in T0MCR and T1MCR registers
#define mr0i			0x00000001
#define mr0r			0x00000002
#define mr0s			0x00000004
#define mr1i			0x00000008
#define mr1r			0x00000010
#define mr1s			0x00000020
#define mr2i			0x00000040
#define mr2r			0x00000080
#define mr2s			0x00000100
#define mr3i			0x00000200
#define mr3r			0x00000400
#define mr3s			0x00000800

// Bit masks in T0CTCR and T1CTCR registers
#define timer			0x00000000
#define counter_rising	0x00000001
#define counter_falling	0x00000002
#define counter_both	0x00000003
#define cap0			0x00000000
#define cap1			0x00000004
#define cap2			0x00000008
#define cap3			0x0000000c

// Bit masks in PINSEL0 and PINSEL1 registers
#define P0_2_TIMER0_CAP0	0x00000020
#define P0_4_TIMER0_CAP1	0x00000200
#define P0_6_TIMER0_CAP2	0x00002000
#define P0_10_TIMER1_CAP0	0x00200000
#define P0_11_TIMER1_CAP1	0x00800000
#define P0_16_TIMER0_CAP2	0x00000003
#define P0_17_TIMER1_CAP2	0x00000004
#define P0_18_TIMER1_CAP3	0x00000010
#define P0_19_TIMER1_CAP2	0x000000c0
#define P0_21_TIMER1_CAP3	0x00000c00
#define P0_22_TIMER0_CAP0	~0x00002000	// Negated, to differ from P0_6_TIMER0_CAP2.
#define P0_27_TIMER0_CAP1	0x00800000
#define P0_28_TIMER0_CAP2	0x02000000
#define P0_29_TIMER0_CAP3	0x08000000
#define P0_30_TIMER0_CAP0	0x30000000

// Register addresses
#define T0IR	(*((volatile int *)0xe0004000))	// Timer0 Interrupt
#define T0TCR	(*((volatile int *)0xe0004004))	// Timer0 Control
#define T0TC	(*((volatile int *)0xe0004008))	// Timer0 Counter
#define T0PR	(*((volatile int *)0xe000400c))	// Timer0 Prescale
#define T0MCR	(*((volatile int *)0xe0004014))	// Timer0 Match Control
#define T0MR0	(*((volatile int *)0xe0004018))	// Timer0 Matches
#define T0MR1	(*((volatile int *)0xe000401c))
#define T0MR2	(*((volatile int *)0xe0004020))
#define T0MR3	(*((volatile int *)0xe0004024))
#define T0CCR	(*((volatile int *)0xe0004028))	// Timer0 Capture Control
#define T0CTCR	(*((volatile int *)0xe0004070))	// Timer0 Count Control
#define T1IR	(*((volatile int *)0xe0008000))	// Timer1 Interrupt
#define T1TCR	(*((volatile int *)0xe0008004))	// Timer1 Control
#define T1TC	(*((volatile int *)0xe0008008))	// Timer1 Counter
#define T1PR	(*((volatile int *)0xe000800c))	// Timer1 Prescale
#define T1MCR	(*((volatile int *)0xe0008014))	// Timer1 Match Control
#define T1MR0	(*((volatile int *)0xe0008018))	// Timer1 Matches
#define T1MR1	(*((volatile int *)0xe000801c))
#define T1MR2	(*((volatile int *)0xe0008020))
#define T1MR3	(*((volatile int *)0xe0008024))
#define T1CCR	(*((volatile int *)0xe0008028))	// Timer1 Capture Control
#define T1CTCR	(*((volatile int *)0xe0008070))	// Timer1 Count Control

// Function declarations
extern void timer0_init(int prescale, int *match, int control, int count, int pin);
extern void timer1_init(int prescale, int *match, int control, int count, int pin);

#endif
