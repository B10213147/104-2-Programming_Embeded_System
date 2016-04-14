// Date:   18.9.2013
// Author: Janez Puhan

#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include "timer.h"
#include "vpbdiv.h"
#include "pll.h"

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
#define P0_31	0x80000000

// Bit masks in PINSEL0 register
#define P0_0_PINSEL0	0xfffffffc
#define P0_1_PINSEL0	0xfffffff3
#define P0_2_PINSEL0	0xffffffcf
#define P0_3_PINSEL0	0xffffff3f
#define P0_4_PINSEL0	0xfffffcff
#define P0_5_PINSEL0	0xfffff3ff
#define P0_6_PINSEL0	0xffffcfff
#define P0_7_PINSEL0	0xffff3fff
#define P0_8_PINSEL0	0xfffcffff
#define P0_9_PINSEL0	0xfff3ffff
#define P0_10_PINSEL0	0xffcfffff
#define P0_11_PINSEL0	0xff3fffff
#define P0_12_PINSEL0	0xfcffffff
#define P0_13_PINSEL0	0xf3ffffff
#define P0_14_PINSEL0	0xcfffffff
#define P0_15_PINSEL0	0x3fffffff

// Bit masks in PINSEL1 register
#define P0_16_PINSEL1	0xfffffffc
#define P0_17_PINSEL1	0xfffffff3
#define P0_18_PINSEL1	0xffffffcf
#define P0_19_PINSEL1	0xffffff3f
#define P0_20_PINSEL1	0xfffffcff
#define P0_21_PINSEL1	0xfffff3ff
#define P0_22_PINSEL1	0xffffcfff
#define P0_23_PINSEL1	0xffff3fff
#define P0_25_PINSEL1	0xfff3ffff
#define P0_26_PINSEL1	0xffcfffff
#define P0_27_PINSEL1	0xff3fffff
#define P0_28_PINSEL1	0xfcffffff
#define P0_29_PINSEL1	0xf3ffffff
#define P0_30_PINSEL1	0xcfffffff
#define P0_31_PINSEL1	0x3fffffff

// Led diode masks for older S-Arm boards (Serial number < 50000)
// #define LED0 0x00000100	// Equivalent to P0_8
// #define LED1 0x00000200	// Equivalent to P0_9
// #define LED2 0x00000400	// Equivalent to P0_10
// #define LED3 0x00000800	// Equivalent to P0_11

// Led diode masks for newer S-Arm boards (Serial number > 49999)
#define LED0 0x00000010	// Equivalent to P0_4
#define LED1 0x00000020	// Equivalent to P0_5
#define LED2 0x00000040	// Equivalent to P0_6
#define LED3 0x00000080	// Equivalent to P0_7

// Push button masks
#define T0 0x00001000	// Equivalent to P0_12
#define T1 0x00002000	// Equivalent to P0_13
#define T2 0x00004000	// Equivalent to P0_14
#define T3 0x00008000	// Equivalent to P0_15

// LCD pin masks
#define DB4	0x00010000	// Equivalent to P0_16
#define DB5	0x00020000	// Equivalent to P0_17
#define DB6	0x00040000	// Equivalent to P0_18
#define DB7	0x00080000	// Equivalent to P0_19
#define RS	0x00100000	// Equivalent to P0_20
#define E	0x00200000	// Equivalent to P0_21
#define RW	0x00400000	// Equivalent to P0_22

// LCD commands
#define CLEAR_DISPLAY			0x01
#define RETURN_HOME				0x02
#define MODE					0x04
#define CURSOR_DIRECTION_RIGHT	0x02	// MODE command flag
#define CURSOR_DIRECTION_LEFT	0x00	// MODE command flag
#define DISPLAY_SHIFT_ON		0x01	// MODE command flag
#define DISPLAY_SHIFT_OFF		0x00	// MODE command flag
#define CONTROL					0x08
#define DISPLAY_ON				0x04	// CONTROL command flag
#define DISPLAY_OFF				0x00	// CONTROL command flag
#define CURSOR_ON				0x02	// CONTROL command flag
#define CURSOR_OFF				0x00	// CONTROL command flag
#define BLINK_ON				0x01	// CONTROL command flag
#define BLINK_OFF				0x00	// CONTROL command flag
#define SHIFT					0x10
#define DISPLAY					0x08	// SHIFT command flag
#define CURSOR					0x00	// SHIFT command flag
#define RIGHT					0x04	// SHIFT command flag
#define LEFT					0x00	// SHIFT command flag
#define SET						0x20
#define BITS_8					0x10	// SET command flag
#define BITS_4					0x00	// SET command flag
#define LINES_2					0x08	// SET command flag
#define LINE_1					0x00	// SET command flag
#define DOTS_5_10				0x04	// SET command flag
#define DOTS_5_8				0x00	// SET command flag
#define CGRAM					0x40
#define DDRAM					0x80

// Register addresses
#define IO0PIN	(*((volatile int *)0xe0028000))	// Pin Value
#define IO0SET	(*((volatile int *)0xe0028004))	// Output Set
#define IO0DIR	(*((volatile int *)0xe0028008))	// Direction
#define IO0CLR	(*((volatile int *)0xe002800c))	// Output Clear
#define PINSEL0	(*((volatile int *)0xe002c000))	// Pin Function Select
#define PINSEL1	(*((volatile int *)0xe002c004))

// Function declarations
extern void set_pin_direction(int input, int output);
extern void set_pin_value(int pin, int value);
extern int get_pin_value(int pin);
extern int get_keys();
extern int lcd_get_cursor();
extern void lcd_write_comm(int com);
extern void lcd_write_data(int data);
extern void lcd_init();

#endif
