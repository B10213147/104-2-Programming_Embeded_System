// Date:   17.10.2011
// Author: Janez Puhan

#include "init.h"

// MAM, VPB, PLL, pin and LCD initialisation
// If LCD is not present the lcd_init() function call must be commented out.
// clock_mhz ... clock rate in MHz [12, 24, 36, 48 or 60]
// div       ... divider value [cclk_4, cclk_2 or cclk]
// input     ... input pins mask (e.g. 0x0000f000)
// output    ... output pins mask (e.g. 0x000000f0)
// value     ... output pins value [0 or 1]
void init(int clock_mhz, int div, int input, int output, int value)
{
	int i, j;
	mam_init(clock_mhz);
	set_vpbdiv(div);
	pll_init(clock_mhz);
	set_pin_direction(input, output);
	for(i = 0, j = 1; i < 32; i = i + 1, j = j << 1) if(output & j)
		set_pin_value(i, value);
	lcd_init();	// Comment out if LCD not present.
}
