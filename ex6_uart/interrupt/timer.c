// Date:   18.9.2013
// Author: Janez Puhan

#include "timer.h"

// Reset and configure timer0
// prescale ... maximum prescale counter value
// match    ... array of match values
// control  ... match control [any combination of 0x00000000, mrxi, mrxr and mrxs,
//              where x = 0, 1, 2 or 3]
// count    ... count control [timer, counter_rising, counter_falling, counter_both]
// pin      ... counter input pin number (used only when count != timer)
//              [P0_2_TIMER0_CAP0, P0_4_TIMER0_CAP1, P0_6_TIMER0_CAP2,
//              P0_16_TIMER0_CAP2, P0_22_TIMER0_CAP0, P0_27_TIMER0_CAP1,
//              P0_28_TIMER0_CAP2, P0_29_TIMER0_CAP3, P0_30_TIMER0_CAP0]
// Note: capture timer counter and external match output features are not supported
//       in this function.
void timer0_init(int prescale, int *match, int control, int count, int pin)
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
	T0CCR = 0;
	T0CTCR = count;
	if(count != timer) switch(pin)
	{
	case P0_2_TIMER0_CAP0:
		T0CTCR = count | cap0;
		PINSEL0 = (PINSEL0 & P0_2_PINSEL0) | P0_2_TIMER0_CAP0;
		break;
	case P0_4_TIMER0_CAP1:
		T0CTCR = count | cap1;
		PINSEL0 = (PINSEL0 & P0_4_PINSEL0) | P0_4_TIMER0_CAP1;
		break;
	case P0_6_TIMER0_CAP2:
		T0CTCR = count | cap2;
		PINSEL0 = (PINSEL0 & P0_6_PINSEL0) | P0_6_TIMER0_CAP2;
		break;
	case P0_16_TIMER0_CAP2:
		T0CTCR = count | cap2;
		PINSEL1 = (PINSEL1 & P0_16_PINSEL1) | P0_16_TIMER0_CAP2;
		break;
	case P0_22_TIMER0_CAP0:
		T0CTCR = count | cap0;
		PINSEL1 = (PINSEL1 & P0_22_PINSEL1) | ~P0_22_TIMER0_CAP0;
		break;
	case P0_27_TIMER0_CAP1:
		T0CTCR = count | cap1;
		PINSEL1 = (PINSEL1 & P0_27_PINSEL1) | P0_27_TIMER0_CAP1;
		break;
	case P0_28_TIMER0_CAP2:
		T0CTCR = count | cap2;
		PINSEL1 = (PINSEL1 & P0_28_PINSEL1) | P0_28_TIMER0_CAP2;
		break;
	case P0_29_TIMER0_CAP3:
		T0CTCR = count | cap3;
		PINSEL1 = (PINSEL1 & P0_29_PINSEL1) | P0_29_TIMER0_CAP3;
		break;
	case P0_30_TIMER0_CAP0:
		T0CTCR = count | cap0;
		PINSEL1 = (PINSEL1 & P0_30_PINSEL1) | P0_30_TIMER0_CAP0;
		break;
	}
}

// Reset and configure timer1
// prescale ... maximum prescale counter value
// match    ... array of match values
// control  ... match control [any combination of 0x00000000, mrxi, mrxr and mrxs,
//              where x = 0, 1, 2 or 3]
// count    ... count control [timer, counter_rising, counter_falling, counter_both]
// pin      ... counter input pin number (used only when count != timer)
//              [P0_10_TIMER1_CAP0, P0_11_TIMER1_CAP1, P0_17_TIMER1_CAP2,
//              P0_18_TIMER1_CAP3, P0_19_TIMER1_CAP2, P0_21_TIMER1_CAP3]
// Note: capture timer counter and external match output features are not supported
//       in this function.
void timer1_init(int prescale, int *match, int control, int count, int pin)
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
	T1CCR = 0;
	T1CTCR = count;
	if(count != timer) switch(pin)
	{
	case P0_10_TIMER1_CAP0:
		T1CTCR = count | cap0;
		PINSEL0 = (PINSEL0 & P0_10_PINSEL0) | P0_10_TIMER1_CAP0;
		break;
	case P0_11_TIMER1_CAP1:
		T1CTCR = count | cap1;
		PINSEL0 = (PINSEL0 & P0_11_PINSEL0) | P0_11_TIMER1_CAP1;
		break;
	case P0_17_TIMER1_CAP2:
		T1CTCR = count | cap2;
		PINSEL1 = (PINSEL1 & P0_17_PINSEL1) | P0_17_TIMER1_CAP2;
		break;
	case P0_18_TIMER1_CAP3:
		T1CTCR = count | cap3;
		PINSEL1 = (PINSEL1 & P0_18_PINSEL1) | P0_18_TIMER1_CAP3;
		break;
	case P0_19_TIMER1_CAP2:
		T1CTCR = count | cap2;
		PINSEL1 = (PINSEL1 & P0_19_PINSEL1) | P0_19_TIMER1_CAP2;
		break;
	case P0_21_TIMER1_CAP3:
		T1CTCR = count | cap3;
		PINSEL1 = (PINSEL1 & P0_21_PINSEL1) | P0_21_TIMER1_CAP3;
		break;
	}
}
