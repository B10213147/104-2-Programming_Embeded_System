#ifndef _GPIO_H_INCLUDED_
#define _GPIO_H_INCLUDED_

// Register addresses
#define IO0PIN	(*((volatile int *)0xe0028000))	// Pin Value
#define IO0SET	(*((volatile int *)0xe0028004))	// Output Set
#define IO0DIR	(*((volatile int *)0xe0028008))	// Direction
#define IO0CLR	(*((volatile int *)0xe002800c))	// Output Clear
#define PINSEL0	(*((volatile int *)0xe002c000))	// Pin Function Select P0_0-P0_15
#define PINSEL1	(*((volatile int *)0xe002c004)) // Pin Function Select P0_16-P0_31

#endif
