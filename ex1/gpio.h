#ifndef _GPIO_H_INCLUDED_
#define _GPIO_H_INCLUDED_

// Register P0 GPIO pin masks
#define P0_0	0x00000001
#define P0_1	0x00000002
#define P0_2	0x00000004
#define P0_3	0x00000008
#define P0_4	0x00000010
#define P0_5	0x00000020
#define P0_6	0x00000040
#define P0_7	0x00000080
#define P0_8	0x00000100
#define P0_9	0x00000200
#define P0_10	0x00000400
#define P0_11	0x00000800
#define P0_12	0x00001000
#define P0_13	0x00002000
#define P0_14	0x00004000
#define P0_15	0x00008000
#define P0_16	0x00010000
#define P0_17	0x00020000
#define P0_18	0x00040000
#define P0_19	0x00080000
#define P0_20	0x00100000
#define P0_21	0x00200000
#define P0_22	0x00400000
#define P0_23	0x00800000
#define P0_25	0x02000000
#define P0_26	0x04000000
#define P0_27	0x08000000
#define P0_28	0x10000000
#define P0_29	0x20000000
#define P0_30	0x40000000

// Register addresses
#define IO0PIN	(*((volatile int *)0xe0028000))	// Pin Value
#define IO0SET	(*((volatile int *)0xe0028004))	// Output Set
#define IO0DIR	(*((volatile int *)0xe0028008))	// Direction
#define IO0CLR	(*((volatile int *)0xe002800c))	// Output Clear
#define PINSEL0	(*((volatile int *)0xe002c000))	// Pin Function Select P0_0-P0_15
#define PINSEL1	(*((volatile int *)0xe002c004)) // Pin Function Select P0_16-P0_31

#endif
