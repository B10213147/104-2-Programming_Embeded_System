// Date:   26.9.2007
// Author: Janez Puhan

#include "mam.h"

// Memory acceleration module (MAM) initialisation
// clock_mhz ... clock rate in MHz [12, 24, 36, 48 or 60]
void mam_init(int clock_mhz)
{
	MAMCR = mam_disable;
	switch(clock_mhz)
	{
	case 12:
		// MAMTIM = 1;
		// break;	// If MAMTIM=1 WinIdea gets confused at download.
	case 24:
	case 36:
		MAMTIM = 2;
		break;
	case 48:
	case 60:
		MAMTIM = 3;
	}
	MAMCR = mam_enable;
}
