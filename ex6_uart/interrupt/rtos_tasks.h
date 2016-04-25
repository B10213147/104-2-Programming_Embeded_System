// Date:   18.9.2013
// Author: Janez Puhan

#ifndef RTOS_TASKS_H_INCLUDED
#define RTOS_TASKS_H_INCLUDED

#include "vic.h"
#include "gpio.h"

extern voidfuncptr priv_task;
extern voidfuncptr sch_tab[];
extern int sch_tab_size;

// Priviliged and orinary scheduled tasks
extern void empty_task();

// LCD related stuff
extern char *lcd_string;
extern void lcd_driver();

#endif                           
