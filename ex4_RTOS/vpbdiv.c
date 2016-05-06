// Date:   15.2.2007
// Author: Janez Puhan

#include "vpbdiv.h"

// Set VLSI peripherial bus (VPB) divider
// div ... divider value [cclk_4, cclk_2 or cclk]
void set_vpbdiv(int div)
{
	VPBDIV = div;
}
