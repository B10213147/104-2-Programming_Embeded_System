// Date:   21.11.2011
// Author: Janez Puhan

#include "gpio.h"

// Set direction for selected pins p0.0 to p0.31
// input  ... input pins mask (e.g. 0x0000f000)
// output ... output pins mask (e.g. 0x000000f0)
void set_pin_direction(int input, int output)
{
	unsigned int i, j;
	for(i = 1, j = 0xfffffffc; i < 0x00010000; i = i << 1, j = j << 2, j = j + 3)
		if(i & (input | output)) PINSEL0 = PINSEL0 & j;
	for(i = 0x00010000, j = 0xfffffffc; i > 0; i = i << 1, j = j << 2, j = j + 3)
		if(i & (input | output)) PINSEL1 = PINSEL1 & j;
	IO0DIR = IO0DIR | output;
	IO0DIR = IO0DIR & ~input;
}

// Set output pin value
// pin   ... pin number [0-31]
// value ... pin value [0 or 1]
void set_pin_value(int pin, int value)
{
	pin = 1 << pin;
	if(value) IO0SET = pin;
	else IO0CLR = pin;
}

// Get input or output pin value
// pin ... pin number [0-31]
int get_pin_value(int pin)
{
	pin = 1 << pin;
	return (IO0PIN & pin)!=0;
}

// Get all four keys state
int get_keys()
{
	return(~IO0PIN & (T0 | T1 | T2 | T3));
}

// Wait function
// delay ... wait time in microseconds (< 0x7ffffffe)
void wait(int delay)
{
	int start = T0TC;
	while(T0TC - start < delay + 1);
}

// Get LCD cursor position
int lcd_get_cursor()
{
	int position;
	IO0DIR = IO0DIR & ~(DB7 | DB6 | DB5 | DB4);
	IO0CLR = RS;
	IO0SET = RW;
	wait(1);
	IO0SET = E;
	wait(1);
	position = (IO0PIN & (DB6 | DB5 | DB4)) >> 12;
	IO0CLR = E;
	wait(1);
	IO0SET = E;
	wait(1);
	position = position | ((IO0PIN & (DB7 | DB6 | DB5 | DB4)) >> 16);
	IO0CLR = E;
	IO0DIR = IO0DIR | DB7 | DB6 | DB5 | DB4;
	return position;
}

// Wait for busy state of LCD to end
void busy()
{
	int busy_flag;
	IO0DIR = IO0DIR & ~(DB7 | DB6 | DB5 | DB4);
	IO0CLR = RS;
	IO0SET = RW;
	do
	{
		wait(1);
		IO0SET = E;
		wait(1);
		busy_flag = IO0PIN & DB7;
		IO0CLR = E;
		wait(1);
		IO0SET = E;
		wait(1);
		IO0CLR = E;
	} while(busy_flag);
	IO0DIR = IO0DIR | DB7 | DB6 | DB5 | DB4;
}

// LCD write high four bits
// byte ... data or command byte
void lcd_write_high(int byte)
{
	IO0CLR = RW;
	wait(1);
	IO0CLR = DB7 | DB6 | DB5 | DB4;
	IO0SET = E | ((byte << 12) & (DB7 | DB6 | DB5 | DB4));
	wait(1);
	IO0CLR = E;
}

// LCD write low four bits
// byte ... data or command byte
void lcd_write_low(int byte)
{
	wait(1);
	IO0CLR = DB7 | DB6 | DB5 | DB4;
	IO0SET = E | ((byte << 16) & (DB7 | DB6 | DB5 | DB4));
	wait(1);
	IO0CLR = E;
	busy();
}

// LCD write half command
// com ... command
void lcd_write_half_comm(int com)
{
	IO0CLR = RS;
	lcd_write_high(com);
}

// LCD write command
// com ... command
void lcd_write_comm(int com)
{
	lcd_write_half_comm(com);
	lcd_write_low(com);
}

// LCD write data
// data ... character
void lcd_write_data(int data)
{
	IO0SET = RS;
	lcd_write_high(data);
	lcd_write_low(data);
}

// LCD initialisation routine
void lcd_init()
{
	// Check if timer0 is already initialised as free running counter counting
	// microseconds (rtos)
	int prescale = 4;
	if(VPBDIV) prescale = VPBDIV;
	prescale = 12 * ((PLLSTAT & msel) + 1) / prescale - 1;
	if(!(T0TCR == counter_enable && T0PR == prescale && T0MR1 == 0 && T0MR2 == 0 &&
		T0MR3 == 0 && T0MCR == mr0i && T0CTCR == timer))
	{
		int match[4] = {0, 0, 0, 0};
		timer0_init(prescale, match, 0, timer);
		T0TCR = counter_enable;
	}
	set_pin_direction(0, RS | RW | E | DB7 | DB6 | DB5 | DB4);
	IO0CLR = E;                             
	wait(40000);
	lcd_write_half_comm(SET | BITS_8);
	wait(4100);
	lcd_write_half_comm(SET | BITS_8);
	wait(100);
	lcd_write_half_comm(SET | BITS_8);
	busy();
	lcd_write_half_comm(SET | BITS_4);
	wait(100);	// LCD initialisation sometimes hangs without this delay ???
	busy();
	lcd_write_comm(SET | BITS_4 | LINES_2 | DOTS_5_8);
	lcd_write_comm(CONTROL | DISPLAY_OFF | CURSOR_OFF | BLINK_OFF);
	lcd_write_comm(CLEAR_DISPLAY);
	lcd_write_comm(MODE | CURSOR_DIRECTION_RIGHT | DISPLAY_SHIFT_OFF);
	lcd_write_comm(CONTROL | DISPLAY_ON | CURSOR_OFF | BLINK_OFF);
}
