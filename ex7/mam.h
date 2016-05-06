// Date:   26.9.2007
// Author: Janez Puhan

// MAM mode in MAMCR register
#define mam_disable				0x00
#define mam_partially_enable	0x01
#define mam_enable				0x02

// Register addresses
#define MAMCR	(*((volatile int *)0xe01fc000))	// Control
#define MAMTIM	(*((volatile int *)0xe01fc004))	// Timing

// Function declaration
extern void mam_init(int clock_mhz);
