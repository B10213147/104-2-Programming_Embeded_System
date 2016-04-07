// Date:   15.2.2007
// Author: Janez Puhan

#include "timer.h"

// Reset and configure timer0
// prescale ... maximum prescale counter value
// match    ... array of match values
// control  ... match control [any combination of 0x00000000, mrxi, mrxr and mrxs,
//              where x = 0, 1, 2 or 3]
// count    ... count control [timer, counter_rising | capx, counter_falling | capx
//              or counter_both | capx, where x = 0, 1, 2 or 3]
void timer0_init(int prescale, int *match, int control, int count)
{
	T0TCR = counter_reset;
	T0IR = mr0_interrupt | mr1_interrupt | mr2_interrupt | mr3_interrupt |
		cr0_interrupt | cr1_interrupt | cr2_interrupt | cr3_interrupt;
	T0PR = prescale;
	T0MR0 = match[0];
	T0MR1 = match[1];
	T0MR2 = match[2];
	T0MR3 = match[3];
	T0MCR = control;
	T0CTCR = count;
}

// Reset and configure timer1
// prescale ... maximum prescale counter value
// match    ... array of match values
// control  ... match control [any combination of 0x00000000, mrxi, mrxr and mrxs,
//              where x = 0, 1, 2 or 3]
// count    ... count control [timer, counter_rising | capx, counter_falling | capx
//              or counter_both | capx, where x = 0, 1, 2 or 3]
void timer1_init(int prescale, int *match, int control, int count)
{
	T1TCR = counter_reset;
	T1IR = mr0_interrupt | mr1_interrupt | mr2_interrupt | mr3_interrupt |
		cr0_interrupt | cr1_interrupt | cr2_interrupt | cr3_interrupt;
	T1PR = prescale;
	T1MR0 = match[0];
	T1MR1 = match[1];
	T1MR2 = match[2];
	T1MR3 = match[3];
	T1MCR = control;
	T1CTCR = count;
}
