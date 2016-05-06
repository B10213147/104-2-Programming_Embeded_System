// Date:   26.9.2007
// Author: Janez Puhan

// Bit masks in PLLCON register
#define plle		0x00000001
#define pllc		0x00000002

// Bit masks in PLLSTAT register
#define msel		0x0000001f
#define psel		0x00000060
#define plle_stat	0x00000100
#define pllc_stat	0x00000200
#define plock		0x00000400

// Register addresses
#define PLLCON	(*((volatile int *)0xe01fc080))	// Control
#define PLLCFG	(*((volatile int *)0xe01fc084))	// Configuration
#define PLLSTAT	(*((volatile int *)0xe01fc088))	// Status

// Function declaration
extern void pll_init(int clock_mhz);
