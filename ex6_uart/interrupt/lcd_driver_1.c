// Date:   10.11.2011
// Author: Janez Puhan

#include "init.h"

char *lcd_string;

void lcd_driver_1()
{
	int i;
	lcd_write_comm(DDRAM | 0x00);
	for(i = 0; i < 16; i++) lcd_write_data(lcd_string[i]);
	lcd_write_comm(DDRAM | 0x40);
	for(; i < 32; i++) lcd_write_data(lcd_string[i]);
}

