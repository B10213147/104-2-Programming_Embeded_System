// Date:   18.9.2013
// Author: Janez Puhan

#ifndef RTOS_H_INCLUDED
#define RTOS_H_INCLUDED

// System status
#define task_completed	0
#define task_running	1

#include "vpbdiv.h"
#include "pll.h"
#include "timer.h"
#include "rtos_tasks.h"

// Function declarations
extern void sch_on(unsigned int slice);
extern void sch_int();
void sch_syn(int len);

#endif
