#ifndef RTOS_TASKS_H_INCLUDED
#define RTOS_TASKS_H_INCLUDED

#include "vic.h"
#include "gpio.h"
#include "rtos_pipe.h"

extern voidfuncptr priv_task;
extern voidfuncptr sch_tab[];
extern int sch_tab_size;

// dummy task
extern void empty_task();

//LCD driver stuff
extern void lcd_driver();
extern char *lcd_string;

extern void keys_driver();
extern void keys2lcd();
extern void generator();


#endif
