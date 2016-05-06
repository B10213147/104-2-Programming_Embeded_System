// Date:   10.10.2012
// Author: Janez Puhan
// some standard input/output functions rewritten for student ARM board with
// LPC2138 microcontroller used in first-year programming course at the
// Faculty of Electrical Engineering, University of Ljubljana

#include "io.h"

char second_line[16];					// Second line of LCD needed for scrolling
struct rtos2pipe *keys_buffer = NULL;	// Ten places long keys buffer
unsigned int num_of_secs = 0;			// Number of seconds since start
int prev_tc = 0;						// Previous timer counter value

// LCD

// Overwrite second line with spaces
void clear_2nd_line()
{
	int i;
	for(i = 0; i < 16; i++) second_line[i] = ' ';
}

// LCD initialisation routine
// Note that timer0 is used for generating LCD signals
void _LCDInit()
{
	lcd_init();
	lcd_write_comm(CONTROL | DISPLAY_ON | CURSOR_ON | BLINK_ON);
	clear_2nd_line();
}

// Clear LCD
// Move cursor to the beginning of the first line
void clrscr()
{
	lcd_write_comm(CLEAR_DISPLAY);
	clear_2nd_line();
}

// Copy second line of LCD into the first one
// Clear second line
// Move cursor to the beginning of the second line
void scroll()
{
	int i;
	lcd_write_comm(DDRAM | 0x00);
	for(i = 0; i < 16; i++) lcd_write_data(second_line[i]);
	lcd_write_comm(DDRAM | 0x40);
	for(i = 0; i < 16; i++) lcd_write_data(' ');
	lcd_write_comm(DDRAM | 0x40);
	clear_2nd_line();
}

// Write character to LCD at cursor position
// At the end of the first line move cursor to the beginning of the second line
// At the end of the second line scroll up
// Implemented escape sequences:
//	'\b' backspace       ... move cursor back one position (non-destructive)
//	'\n' new line        ... move cursor to the first position of the next line
//	'\r' carriage return ... move cursor to the first position of the current line
//	'\t' horizontal tab  ... move cursor to the next horizontal tabular position
// Return displayed character
int putch(int character)
{
	int cursor = lcd_get_cursor();	// 0x00..0x0f first line, 0x40..0x4f second line
	switch(character)
	{
	case '\b':
		// If not beginning of line, go one position back
		if(cursor != 0x00 && cursor != 0x40) cursor = cursor - 1;
		break;
	case '\n':
		if(cursor >= 0x40) scroll();	// Second line
		cursor = 0x40;
		break;
	case '\r':
		if(cursor < 0x40) cursor = 0x00;	// First line
		else cursor = 0x40;	// Second line
		break;
	case '\t':	// one tab = four positions
		cursor = (cursor & 0xfc) + 0x04;	// Go to next tab position
		if(cursor == 0x10) cursor = 0x0f;	// Beyond first line
		if(cursor == 0x50) cursor = 0x4f;	// Beyond second line
		break;
	default:
		lcd_write_data(character);
		// If character is in the second line, update
		if(cursor >= 0x40) second_line[cursor - 0x40] = character;
		cursor = cursor + 1;
		if(cursor & 0x10)	// Beyond end of line
		{
			if(cursor & 0x40) scroll();	// Second line
			cursor = 0x40;
		}
	}
	lcd_write_comm(DDRAM | cursor);	// Set new cursor position
	return character;
}

int putchar(int character)
{
	return putch(character);
}

// Set LCD cursor appearance
void _setcursortype(int cursor_type)
{
	int cursor = CURSOR_OFF | BLINK_OFF;
	switch(cursor_type)
	{
		case _SOLIDCURSOR: cursor = BLINK_ON;
		case _NORMALCURSOR: cursor = cursor | CURSOR_ON;
	}
	lcd_write_comm(CONTROL | DISPLAY_ON | cursor);
}

// Set cursor position on 2×16 characters LCD
// If position (x, y) is invalid, nothing happens
void gotoxy(int x, int y)
{
	if(x < 1 || x > 16 || y < 1 || y > 2) return;	// Invalid position
	lcd_write_comm(DDRAM | (0x40 * (y - 1) + (x - 1)));	// Set new cursor position
}

// Writes the str pointed string to LCD
// Does not append a newline character ('\n') as standard ANSI C puts() function
// Return number of characters written
int puts(const char *str)
{
	int i;
	for(i = 0; str[i]; i++) putch(str[i]);
	return i;
}

// Print formatted string to LCD
int printf(const char *format, ...)
{
	int ret;
	char buffer[MAX_PRINTF_STR_LEN];
	va_list arglist;
	va_start(arglist, format);
	ret = vsnprintf(buffer, MAX_PRINTF_STR_LEN, format, arglist);
	va_end(arglist);
	puts(buffer);
}

// Keys (RTOS must be initialised and enabled)
//      (Warning: RTOS stops during debug mode, which temporarily disable keys)

// Key driver task, called by operating system
// Put newly pressed key into keys buffer
void keys_driver(void *arg)
{
	static char n[4] = {0, 0, 0, 0};
	char data;
	int i, mask[4] = {T0, T1, T2, T3}, keys = get_keys();
	for(i = 0; i < 4; i++) if(keys & mask[i])
	{
		if(n[i] == 0)
		{
			data = i + '0';
			rtos2pipe_write(keys_buffer, &data, 1);
		}
		n[i] = 1;
	} else n[i] = 0;
}

// Key pins and driver initialisation routine
void _KeyInit()
{
	unsigned int i, j;
	// This was already done at initialisation
	// Needed in case the pins were reconfigured and are no longer GPIO pins
	for(i = 1, j = 0xfffffffc; i < 0x00010000; i = i << 1, j = j << 2, j = j + 3)
		if(i & (T0 | T1 | T2 | T3)) PINSEL0 = PINSEL0 & j;
	IO0DIR = IO0DIR & ~(T0 | T1 | T2 | T3);
	rtos2task_delete(keys_driver);	// Needed in case of reinitialisation
	if(keys_buffer) rtos2pipe_delete(keys_buffer);
	keys_buffer = rtos2pipe_create(10);	// Create buffer
	rtos2task_create(keys_driver, NULL, 10, 10);	// Start driver
}

// Get instantaneous keys state in least significant four bits (b3=T0 ... b0=T3)
// Flush keys buffer
int _getkeys()
{
	char buffer[10];
	int i, keys1, keys2;
	do	// 100us bouncing filter
	{
		keys1 = get_keys();
		wait(100);
		keys2 = get_keys();
	} while(keys1 != keys2);
	keys1 = keys1 >> 12;
	for(i = 0, keys2 = 0; i < 4; i++)
	{
		keys2 = (keys2 << 1) | (keys1 % 2);
		keys1 = keys1 >> 1;
	}
	rtos2pipe_read(keys_buffer, (char *)(&buffer), 10);
	return keys2;
}

// Check if a key was pressed
int kbhit()
{
	char buffer[10];
	unsigned int n;
	rtos2task_delete(keys_driver);	// Temporary disable keys driver
	n = rtos2pipe_read(keys_buffer, (char *)(&buffer), 10);
	rtos2pipe_write(keys_buffer, (char *)(&buffer), n);
	rtos2task_create(keys_driver, NULL, 10, 10);	// Start driver
	if(n) return buffer[0];
	return 0;
}

// Wait for a pressed key and get it
int getch()
{
	char character;
	unsigned int n = 0;
	while(n == 0) n = rtos2pipe_read(keys_buffer, &character, 1);
	return character;
}

// LEDs

// LED pins initialisation
void _LEDInit()
{
	int i, j;
	// This was already done at initialisation
	// Needed in case the pins were reconfigured and are no longer GPIO pins
	for(i = 1, j = 0xfffffffc; i < 0x00010000; i = i << 1, j = j << 2, j = j + 3)
		if(i & (LED0 | LED1 | LED2 | LED3)) PINSEL0 = PINSEL0 & j;
	IO0DIR = IO0DIR | LED0 | LED1 | LED2 | LED3;	// Set LED pins as output
}

// Turn LEDs specified in mask's last four bits (b3 = LED3 ... b0 = LED0) on
void _setleds(int mask)
{
	int i, j;
	for(i = 0, j = LED0 >> 1; j; i = i + 1, j = j >> 1);	// Find LED0 pin
	IO0SET = (mask & 0x0000000f) << i;
}

// Turn LEDs specified in mask's last four bits (b3 = LED3 ... b0 = LED0) off
void _clrleds(int mask)
{
	int i, j;
	for(i = 0, j = LED0 >> 1; j; i = i + 1, j = j >> 1);	// Find LED0 pin
	IO0CLR = (mask & 0x0000000f) << i;
}

// Time (time() task must run in RTOS, timer0 counts microseconds)

// Seconds counter, called by operating system
void time(void *arg)
{
	int t0tc = T0TC;
	num_of_secs = (int)num_of_secs + (unsigned int)(t0tc - prev_tc) / 1000000;
	prev_tc = t0tc - (unsigned int)(t0tc - prev_tc) % 1000000;
}

// Get time elapsed since CPU start (seconds * CLOCKS_PER_SEC)
// Number of milliseconds since CPU start are returned for CLOCKS_PER_SEC = 1000
unsigned int clock()
{
	int i, t0tc = T0TC, ticks = 0,
		secs = (int)num_of_secs + (unsigned int)(t0tc - prev_tc) / 1000000;
	for(i = 0; i < CLOCKS_PER_SEC; i++) ticks = ticks + secs;
	i = 1000000 / CLOCKS_PER_SEC;
	ticks = ticks + (unsigned int)(t0tc - prev_tc) % 1000000 / i;
	return ticks;
}

// Wait for specified time
void delay(unsigned int milliseconds)
{
	while(milliseconds > 2147483)
	{
		wait(2147483000);
		milliseconds = milliseconds - 2147483;
	}
	wait(1000 * milliseconds);
}

// AD/DA conversion

// AD converter at p0.27 initialisation
void _ADCInit()
{
	adc0_init(0, adc0_0, 0);
}

// Convert voltage on p0.27 [0, 3.3V] and return corresponding 10-bit value
int _adconvert()
{
	int val;
	AD0CR = AD0CR | start_now;
	do val = AD0DR;
	while(!(val & done));
	return((val & ls) >> 6);
}

// Initialise AD converter and convert voltage on given pin
// Return corresponding 10-bit value
//  input    pin
//    0      p0.27
//    1      p0.28
//    2      p0.29
//    3      p0.30
//    4      p0.25
//    5      p0.26
//    6      p0.4
//    7      p0.5
int _adconvertExt(int input)
{
	switch(input)
	{
	case 0: adc0_init(0, adc0_0, 0); break;
	case 1: adc0_init(0, adc0_1, 0); break;
	case 2: adc0_init(0, adc0_2, 0); break;
	case 3: adc0_init(0, adc0_3, 0); break;
	case 4: adc0_init(0, adc0_4, 0); break;
	case 5: adc0_init(0, adc0_5, 0); break;
	case 6: adc0_init(adc0_6, 0, 0); break;
	case 7: adc0_init(adc0_7, 0, 0); break;
	default: return -1;
	}
	return _adconvert();
}

// DA converter at p0.25 initialisation
void _DACInit()
{
	dac_init();
}

// Convert 10-bit value into voltage on p0.25 [0, 3.3V]
void _dacwrite(int value)
{
	DACR = (value << 6) & ls;
}

// GPIO

// Set pin direction
//  pinid    pin            dir    pin direction
//    0      p0.0            0         input
//    1      p0.1            1        output
//    2      p0.2
//    .       .
//    .       .
//   31      p0.31
void _setpindir(int pinid, int dir)
{
	int mask = 1 << pinid;
	if(dir) set_pin_direction(0, mask);
	else set_pin_direction(mask, 0);
}

// Set output pin value
//  pinid    pin            value    voltage[V]
//    0      p0.0             0           0
//    1      p0.1             1         3.3
//    2      p0.2
//    .       .
//    .       .
//   31      p0.31
void outportp(int pinid, int value)
{
	if(pinid < 0 || pinid > 31) return;
	set_pin_value(pinid, value);
}

// Get pin value
//  pinid    pin
//    0      p0.0
//    1      p0.1
//    2      p0.2
//    .       .
//    .       .
//   31      p0.31
int inportp(int pinid)
{
	if(pinid < 0 || pinid > 31) return -1;
	if(get_pin_value(pinid)) return 1;
	return 0;
}

// Ramp

// IR initialisation
void _IRInit()
{
	int match[7] = {0, 0, 0, 0, 0, 0, 0};
	if(VPBDIV) match[0] = 12e3 * ((PLLSTAT & msel) + 1) / (VPBDIV * FREQUENCY_kHz);
	else match[0] = 3e3 * ((PLLSTAT & msel) + 1) / FREQUENCY_kHz;
	match[2] = (int)(DUTY_CYCLE * match[0]);
	pwm_init(pwmena2, 0, match, 0, 0);
	PWMTCR = pwm_enable;
}
