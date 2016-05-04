// Date:   9.11.2011
// Author: Janez Puhan

// Usage example:
//	#define size 500
//	char system_memory[size];
//	vic_init(...);	// Do not use IRQ slot0 and timer0 interrupt.
//	rtos2init(system_memory, size, ...);	// Do not call from task.
//	enable_os();	// Always call these functions in pairs.
//	disable_os();

#include "vpbdiv.h"
#include "pll.h"
#include "timer.h"
#include "vic.h"
#include "rtos2.h"

int rtos2recursive_counter;
struct rtos2mem rtos2system_memory;	// System memory region structure.

// Disable operating system interrupt.
// If an interrupt request is received during resetting of corresponding bit in
// VICIntEnable register then the interrupt is disabled but taken. In such a case
// just disabled interrupt starts its execution. Since the interrupt is disabled,
// VIC cannot properly set the VICVectAddr register. VICDefVectAddr is taken
// instead, which can lead to unwanted consequences. To avoid described situation
// irq interrupts are temporary disabled. Also notice that disable_os() function
// works properly only if called from a privileged mode.
void disable_os()
{
	int intr = disable_intr(irq_mask);	// Disable irq interrupts.
	VICIntEnClear = timer0;	// Disable operating system interrupt.
	rtos2recursive_counter = rtos2recursive_counter - 1;
	enable_intr(intr);	// Enable irq interrupts.
}

// Enable operating system interrupt.
void enable_os()
{
	rtos2recursive_counter = rtos2recursive_counter + 1;
	if(rtos2recursive_counter > 0) VICIntEnable = timer0;
}

// CPU timer0 interrupt service routine with operating system core.
// Interrupt nesting is allowed, which enable priority task nesting.
// On irq interrupt current cprs is automatically saved to sprs_irq and return
// address is copied to lr_irq. Thus prior enabling irq interrupt nesting sprs_irq
// has to be stored to irq stack. The same cannot be done for link register, since
// it is constantly in use (function calls). Therefore a switch to another mode (in
// our case system mode) with another link register is needed. Original lr in system
// mode has to be preserved and is stored to system stack.
void rtos2timer0_isr()
{
	VICIntEnClear = timer0;	// Disable operating system interrupt.
	rtos2recursive_counter = rtos2recursive_counter - 1;
	T0IR = mr0_interrupt;	// Ready for the next interrupt.
	VICVectAddr = 0;
	asm("stmfd sp!,    {r4-r10}");	// Enable interrupt nesting.
	asm("mrs   r4,     spsr");
	asm("stmfd sp!,    {r4}");
	asm("mrs   r5,     cpsr");
	asm("and   r5,     r5, #0x7f");
	asm("orr   r5,     r5, #0x1f");
	asm("msr   cpsr_c, r5");
	asm("stmfd sp!,    {lr}");
	rtos2sched();
	asm("ldmfd sp!,    {lr}");	// Disable interrupt nesting.
	asm("mrs   r5,     cpsr");
	asm("and   r5,     r5, #0xf2");
	asm("orr   r5,     r5, #0x92");
	asm("msr   cpsr_c, r5");
	asm("ldmfd sp!,    {r4}");
	asm("msr   spsr,   r4");
	asm("ldmfd sp!,    {r4-r10}");
	enable_os();	// Enable operating system interrupt.
}

// Initialise operating system.
//  memory ... memory available for system dynamic memory allocation
//  size   ... size of available memory in bytes
//  slice  ... timeslice in microseconds
// Initialise VIC before starting the operating system. Do not use IRQ slot0 and
// timer0 interrupt. Do not call this function from task.
void rtos2init(char *memory, unsigned int size, unsigned int slice)
{
	int prescale = 4, match[4] = {0, 0, 0, 0};
	rtos2recursive_counter = 0;
	rtos2running_task = 0;
	rtos2ready_tasks = 0;
	rtos2interrupted_tasks = 0;
	rtos2mem_create(&rtos2system_memory, memory, size);
	slice_quantum = slice;
	if(VPBDIV) prescale = VPBDIV;
	prescale = 12 * ((PLLSTAT & msel) + 1) / prescale - 1;
	match[0] = slice_quantum;
	timer0_init(prescale, match, mr0i, timer);	// timer0 counts microseconds.
	VICVectAddr0 = (int)rtos2timer0_isr;
	VICVectCntl0 = 0x24;
	VICVectAddr = 0;
	T0TCR = counter_enable;
}
