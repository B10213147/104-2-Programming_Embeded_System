// Date:   18.9.2013
// Author: Janez Puhan

#include "rtos.h"

// System variables
int sch_tst, sch_idx, slice_quantum;

// Enable IRQ interrupts
void enable_irq()
{
	asm("stmfd sp!, {r0}");
	asm("mrs   r0, cpsr");
	asm("bic   r0, r0, #0x80");
	asm("msr   cpsr_c, r0");
	asm("ldmfd sp!, {r0}");
}

// Disable IRQ interrupts
void disable_irq()
{
	asm("stmfd sp!, {r0}");
	asm("mrs   r0, cpsr");
	asm("orr   r0, r0, #0x80");
	asm("msr   cpsr_c, r0");
	asm("ldmfd sp!, {r0}");
}

// Real time operating system core
void sch_int()
{
	if(sch_tst == task_running) while(1);
	sch_tst = task_running;
	T0IR = mr0_interrupt;
	VICVectAddr = 0;
	T0MR0 = T0MR0 + slice_quantum;	// Set next slice beginning
	if(T0TC - T0MR0 > 0) T0TC = T0MR0 - slice_quantum;
	enable_irq();
	(*priv_task)();
	(*(sch_tab[sch_idx]))();
	sch_idx = sch_idx + 1;
	if(sch_idx == sch_tab_size / sizeof(voidfuncptr)) sch_idx = 0;
	disable_irq();
	sch_tst = task_completed;
}

// Start real time operating system
//  slice ... timeslice in microseconds
void sch_on(unsigned int slice)
{
	int prescale = 4, match[4] = {0, 0, 0, 0},
		interrupt[16] = {timer0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	voidfuncptr function[16] =
		{sch_int, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	sch_tst = task_completed;
	sch_idx = 0;
	slice_quantum = slice;
	if(VPBDIV) prescale = VPBDIV;
	prescale = 12 * ((PLLSTAT & msel) + 1) / prescale - 1;
	match[0] = slice_quantum;
	timer0_init(prescale, match, mr0i, timer, 0);	// timer0 counts microseconds
	vic_init(0, timer0, function, interrupt, 0);
	T0TCR = counter_enable;
}


// Synchronise with scheduler
// len ... critical code length = len * prescale_val VPB cycles
void sch_syn(int len) {
  while((int)(T0MR0 - len - T0TC) <= 0);
}

