// Date:   6.6.2007
// Author: Janez Puhan

#include "adc_dac.h"

// Configure adc0
// adc0_pins0 ... pins p0.0 to p0.15 configured as adc0 inputs [any combination of
//                0x00000000, adc0_6 and adc0_7]
// adc0_pins1 ... pins p0.16 to p0.31 configured as adc0 inputs [any combination of
//                0x00000000, adc0_0, adc0_1, adc0_2, adc0_3, adc0_4 and adc0_5]
// mode       ... type of conversion, if mode = 0 conversion is performed when
//                triggered, otherwise conversion is performed continuously and mode
//                defines number of cycles (and accuracy) needed for each conversion
//                [0, 4, 5, 6, 7, 8, 9, 10 or 11]
void adc0_init(int adc0_pins0, int adc0_pins1, int mode)
{
	int clkdiv, sel = 0, clk = 12 * ((PLLSTAT & msel) + 1), divider = 4;
	PINSEL0 = (PINSEL0 & ~adc0_pins0) | (pinsel0_adc_mask & adc0_pins0);
	PINSEL1 = (PINSEL1 & ~adc0_pins1) | (pinsel1_adc_mask & adc0_pins1);
	if(adc0_pins1 & adc0_0) sel = sel | adc_0;
	if(adc0_pins1 & adc0_1) sel = sel | adc_1;
	if(adc0_pins1 & adc0_2) sel = sel | adc_2;
	if(adc0_pins1 & adc0_3) sel = sel | adc_3;
	if(adc0_pins1 & adc0_4) sel = sel | adc_4;
	if(adc0_pins1 & adc0_5) sel = sel | adc_5;
	if(adc0_pins0 & adc0_6) sel = sel | adc_6;
	if(adc0_pins0 & adc0_7) sel = sel | adc_7;
	if(VPBDIV) divider = VPBDIV;
	clkdiv = 2 * clk / (9 * divider);
	if(!(2 * clk % (9 * divider))) clkdiv = clkdiv - 1;
	clkdiv = clkdiv << 8;
	if(mode) mode = ((11 - mode) << 17) | burst;
	AD0CR = sel | clkdiv | mode | pdn;
}

// Configure adc1
// adc1_pins0 ... pins p0.0 to p0.15 configured as adc1 inputs [any combination of
//                0x00000000, adc1_0, adc1_1, adc1_2, adc1_3, adc1_4 and adc1_5]
// adc1_pins1 ... pins p0.16 to p0.31 configured as adc1 inputs [any combination of
//                0x00000000, adc1_6 and adc1_7]
// mode       ... type of conversion, if mode = 0 conversion is performed when
//                triggered, otherwise conversion is performed continuously and mode
//                defines number of cycles (and accuracy) needed for each conversion
//                [0, 4, 5, 6, 7, 8, 9, 10 or 11]
void adc1_init(int adc1_pins0, int adc1_pins1, int mode)
{
	int clkdiv, sel = 0, clk = 12 * ((PLLSTAT & msel) + 1), divider = 4;
	PINSEL0 = (PINSEL0 & ~adc1_pins0) | (pinsel0_adc_mask & adc1_pins0);
	PINSEL1 = (PINSEL1 & ~adc1_pins1) | (pinsel1_adc_mask & adc1_pins1);
	if(adc1_pins0 & adc1_0) sel = sel | adc_0;
	if(adc1_pins0 & adc1_1) sel = sel | adc_1;
	if(adc1_pins0 & adc1_2) sel = sel | adc_2;
	if(adc1_pins0 & adc1_3) sel = sel | adc_3;
	if(adc1_pins0 & adc1_4) sel = sel | adc_4;
	if(adc1_pins0 & adc1_5) sel = sel | adc_5;
	if(adc1_pins1 & adc1_6) sel = sel | adc_6;
	if(adc1_pins1 & adc1_7) sel = sel | adc_7;
	if(VPBDIV) divider = VPBDIV;
	clkdiv = 2 * clk / (9 * divider);
	if(!(2 * clk % (9 * divider))) clkdiv = clkdiv - 1;
	clkdiv = clkdiv << 8;
	if(mode) mode = ((11 - mode) << 17) | burst;
	AD1CR = sel | clkdiv | mode | pdn;
}

// Configure dac
void dac_init()
{
	PINSEL1 = (PINSEL1 & ~dac_pins) | dac_value;
}
