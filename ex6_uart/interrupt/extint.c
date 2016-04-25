// Date:   18.9.2013
// Author: Janez Puhan

#include "extint.h"

// Configure and reset external interrupts
// pins        ... external input pins [any combination of P0_1_EINT0, P0_3_EINT1,
//                 P0_7_EINT2, P0_9_EINT3, P0_14_EINT1, P0_15_EINT2, P0_16_EINT0,
//                 P0_20_EINT3 and P0_30_EINT3]
// wake        ... wake up allow mask [any combination of EXTWAKE0, EXTWAKE1,
//                 EXTWAKE2, EXTWAKE, BODWAKE and RTCWAKE]
// sensitivity ... edge sensitivity mask [any combination of #define EXTMODE0,
//                 EXTMODE1, EXTMODE2 and EXTMODE3]
// trig        ... high level (rising edge) trig mask [any combination of EXTPOLAR0,
//                 EXTPOLAR1, EXTPOLAR2 and EXTPOLAR3]
void extint_init(int pins, int wake, int sensitivity, int trig)
{
	if((pins & P0_1_EINT0) != 0) PINSEL0 = (PINSEL0 & P0_1_PINSEL0) | P0_1_EINT0;
	if((pins & P0_3_EINT1) != 0) PINSEL0 = (PINSEL0 & P0_3_PINSEL0) | P0_3_EINT1;
	if((pins & P0_7_EINT2) != 0) PINSEL0 = (PINSEL0 & P0_7_PINSEL0) | P0_7_EINT2;
	if((pins & P0_9_EINT3) != 0) PINSEL0 = (PINSEL0 & P0_9_PINSEL0) | P0_9_EINT3;
	if((pins & P0_14_EINT1) != 0) PINSEL0 = (PINSEL0 & P0_14_PINSEL0) | P0_14_EINT1;
	if((pins & P0_15_EINT2) != 0) PINSEL0 = (PINSEL0 & P0_15_PINSEL0) | P0_15_EINT2;
	if((pins & P0_16_EINT0) != 0) PINSEL1 = (PINSEL0 & P0_16_PINSEL1) | P0_16_EINT0;
	if((pins & P0_20_EINT3) != 0) PINSEL1 = (PINSEL0 & P0_20_PINSEL1) | P0_20_EINT3;
	if((pins & P0_30_EINT3) != 0) PINSEL1 = (PINSEL0 & P0_30_PINSEL1) | P0_30_EINT3;
	INTWAKE = wake;
	EXTMODE = sensitivity;
	EXTPOLAR = trig;
	EXTINT = EINT0 | EINT1 | EINT2 | EINT3;
}
