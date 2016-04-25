// Date:   28.7.2011
// Author: Janez Puhan

// Usage example:
//	#define size 10
//	char buffer[size];
//	unsigned int written, read;
//	struct rtos2pipe *pipe;
//	pipe = rtos2pipe_create(100);
//	written = rtos2pipe_write(pipe, buffer, size);
//	read = rtos2pipe_read(pipe, buffer, size);
//	rtos2pipe_delete(pipe);

#include "rtos2.h"

// Initialise operating system pipe.
//  size ... pipe buffer size in bytes
// Return pointer to created pipe.
struct rtos2pipe *rtos2pipe_create(unsigned int size)
{
	// int intr;	// Uncomment, if disable_intr()/enable_intr() are used.
	struct rtos2pipe *pipe;
	disable_os();	// Disable operating system interrupt.
	// Calls to disable_os()/enable_os() can be commented out, if disable_intr()/enable_intr() are used.
	// intr = disable_intr(...);	// Disable irq (and fiq) interrupts with argument = irq_mask (= irq_and_fiq_mask). Uncomment to enable pipes in irq (and fiq) interrupts.
	pipe = rtos2mem_allocate(&rtos2system_memory, sizeof(struct rtos2pipe));
	pipe->buffer = rtos2mem_allocate(&rtos2system_memory, size + 1);
	pipe->size = size;
	pipe->head = 0;
	pipe->tail = 0;
	enable_os();	// Enable operating system interrupt.
	// enable_intr(intr);	// Uncomment to enable pipes in interrupts.
	return pipe;
}

// Delete operating system pipe.
//  pipe ... pipe structure
void rtos2pipe_delete(struct rtos2pipe *pipe)
{
	// int intr;	// Uncomment, if disable_intr()/enable_intr() are used.
	disable_os();	// Disable operating system interrupt.
	// Calls to disable_os()/enable_os() can be commented out, if disable_intr()/enable_intr() are used.
	// intr = disable_intr(...);	// Disable irq (and fiq) interrupts with argument = irq_mask (= irq_and_fiq_mask). Uncomment to enable pipes in irq (and fiq) interrupts.
	rtos2mem_free(&rtos2system_memory, pipe->buffer);
	rtos2mem_free(&rtos2system_memory, pipe);
	enable_os();	// Enable operating system interrupt.
	// enable_intr(intr);	// Uncomment to enable pipes in interrupts.
}

// Write to operating system pipe.
//  pipe ... pipe structure
//  data ... data to be written
//  size ... size of data in bytes
// Return number of written bytes.
unsigned int rtos2pipe_write(struct rtos2pipe *pipe, char *data, unsigned int size)
{
	unsigned int i;
	// int intr;	// Uncomment, if disable_intr()/enable_intr() are used.
	disable_os();	// Disable operating system interrupt.
	// Calls to disable_os()/enable_os() can be commented out, if disable_intr()/enable_intr() are used.
	// intr = disable_intr(...);	// Disable irq (and fiq) interrupts with argument = irq_mask (= irq_and_fiq_mask). Uncomment to enable pipes in irq (and fiq) interrupts.
	for(i = 0; i < size; i++)
	{
		unsigned int index = pipe->head + 1;
		if(index > pipe->size) index = 0;
		if(index == pipe->tail) break;
		else
		{
			pipe->buffer[pipe->head] = data[i];
			pipe->head = index;
		}
	}
	enable_os();	// Enable operating system interrupt.
	// enable_intr(intr);	// Uncomment to enable pipes in interrupts.
	return i;
}

// Read from operating system pipe.
//  pipe   ... pipe structure
//  buffer ... buffer holding read data
//  size   ... size of buffer in bytes
// Return number of bytes read.
unsigned int rtos2pipe_read(struct rtos2pipe *pipe, char *buffer, unsigned int size)
{
	unsigned int i;
	// int intr;	// Uncomment, if disable_intr()/enable_intr() are used.
	disable_os();	// Disable operating system interrupt.
	// Calls to disable_os()/enable_os() can be commented out, if disable_intr()/enable_intr() are used.
	// intr = disable_intr(...);	// Disable irq (and fiq) interrupts with argument = irq_mask (= irq_and_fiq_mask). Uncomment to enable pipes in irq (and fiq) interrupts.
	for(i = 0; pipe->tail != pipe->head && i < size; i++)
	{
		buffer[i] = pipe->buffer[pipe->tail];
		pipe->tail = pipe->tail + 1;
		if(pipe->tail > pipe->size) pipe->tail = 0;
	}
	enable_os();	// Enable operating system interrupt.
	// enable_intr(intr);	// Uncomment to enable pipes in interrupts.
	return i;
}
