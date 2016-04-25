// Date:   18.9.2013
// Author: Janez Puhan

#ifndef VPBDIV_H_INCLUDED
#define VPBDIV_H_INCLUDED

// VPB divider values in VPBDIV register
#define cclk_4	0x00	// pclk = cclk / 4
#define cclk	0x01	// pclk = cclk
#define cclk_2	0x02	// pclk = cclk / 2

// Register address
#define VPBDIV (*((volatile int *)0xe01fc100))	// VPB Divider

// Function declaration
extern void set_vpbdiv(int div);

#endif
