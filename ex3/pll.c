// Date:   15.2.2007
// Author: Janez Puhan

#include "pll.h"

// PLL feed sequence
void pll_feed()
{
	asm("stmfd sp!, {r0-r2}");
	asm("ldr   r0,  =0xe01fc08c");
	asm("mov   r1,  #0x000000aa");
	asm("mov   r2,  #0x00000055");
	asm("str   r1,  [r0]");
	asm("str   r2,  [r0]");
	asm("ldmfd sp!, {r0-r2}");
}

// Phase locked loop (PLL) initialisation
// clock_mhz ... clock rate in MHz [12, 24, 36, 48 or 60]
void pll_init(int clock_mhz)
{
	PLLCON = 0x00000000;
	pll_feed();
	switch(clock_mhz)
	{
	case 12:
		PLLCFG = 0x00000060;
		break;
	case 24:
		PLLCFG = 0x00000041;
		break;
	case 36:
		PLLCFG = 0x00000042;
		break;
	case 48:
		PLLCFG = 0x00000023;
		break;
	case 60:
		PLLCFG = 0x00000024;
		break;
	}
	PLLCON = plle;
	pll_feed();
	while(!(PLLSTAT & plock));
	PLLCON = PLLCON | pllc;
	pll_feed();
}
