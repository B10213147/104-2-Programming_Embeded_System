#ifndef RTOS_TASKS_H_INCLUDED
#define RTOS_TASKS_H_INCLUDED

#include "vic.h"
#include "gpio.h"

extern voidfuncptr priv_task;
extern voidfuncptr sch_tab[];
extern int sch_tab_size;
extern int time;

// Priviliged and orinary scheduled tasks
extern void empty_task();
extern void lcd_driver();
extern char *lcd_string;
extern void mainCounter();
extern void prepareOutput();
extern void keyCheck();

void modify_Array(int digit, char* temp)
void str2lcd(int position, char *str);
int num2str(int x, char* str);


#endif
