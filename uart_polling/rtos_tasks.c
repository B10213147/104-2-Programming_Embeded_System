// Date:   14.11.2011
// Author: Janez Puhan

#include "rtos_tasks.h"

// Priviliged task
voidfuncptr priv_task = empty_task;
             
// Scheduler
voidfuncptr sch_tab[] = {lcd_driver};

int sch_tab_size = sizeof(sch_tab);

void empty_task(){
}

//lcd related stuff
char lcd_buff[33] = "                                ";
char *lcd_string=lcd_buff;
void lcd_driver(){
	int i, done=0;
	lcd_write_comm(DDRAM | 0x00);  
	for(i = 0; i < 16; i++){
    if(lcd_string[i]=='\0')
      done=1;
    if(done)
      lcd_write_data(' ');
    else
      lcd_write_data(lcd_string[i]);
  }
	lcd_write_comm(DDRAM | 0x40);
	for(; i < 32; i++){
    if(lcd_string[i]=='\0')
      done=1;
    if(done)
      lcd_write_data(' ');
    else
      lcd_write_data(lcd_string[i]);    
  }
}
