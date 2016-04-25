// Date:   1.1.2012
// Author: Janez Puhan

// Usage example:
//	char arg[10];
//	void task(void *data);
//	rtos2task_create(task, arg, 1, 100);
//	rtos2task_delete(task);

#include "timer.h"
#include "rtos2.h"

struct rtos2task *rtos2running_task;		// Currently running task.
struct rtos2task *rtos2ready_tasks;			// List of ready to run tasks.
struct rtos2task *rtos2interrupted_tasks;	// List of interrupted tasks.

// Remove task from list.
//  list ... list of tasks
//  task ... the task
void rtos2task_remove(struct rtos2task **list, struct rtos2task *task)
{
	struct rtos2task *prev, *current;
	if(*list == task) *list = (*list)->next;	// First in the list.
	else	// Search the list.
	{
		for(prev = *list, current = (*list)->next; current != task;
			prev = current, current = current->next);
		prev->next = current->next;
	}
	task->next = 0;
}

// Insert task to list.
//  list ... list of tasks
//  task ... the task
void rtos2task_insert(struct rtos2task **list, struct rtos2task *task)
{
	task->next = *list;
	*list = task;
}

// Initialise operating system task and insert it to ready tasks.
//  function ... task function
//  arg      ... task function argument
//  priority ... task's priority
//  interval ... number of task executions per second =
//               1 / (interval * timeslice in seconds)
void rtos2task_create(rtos2taskptr function, void *arg, unsigned int priority,
	int interval)
{
	int prev_match, new_num_of_empty;
	struct rtos2task *task;
	disable_os();	// Disable operating system interrupt.
	task = rtos2mem_allocate(&rtos2system_memory, sizeof(struct rtos2task));
	rtos2task_insert(&rtos2ready_tasks, task);
	task->function = function;
	task->arg = arg;
	task->priority = priority;
	task->interval = interval;
	task->delete_int = 0;
	// Break eventual series of empty timeslices for the first task execution.
	prev_match = T0MR0 - num_of_empty * slice_quantum;
	new_num_of_empty = (T0TC - prev_match) / slice_quantum + 2;
	if(new_num_of_empty < num_of_empty)
	{
		num_of_empty = new_num_of_empty;
		T0MR0 = prev_match + num_of_empty * slice_quantum;
	}
	task->remain_ticks = num_of_empty;
	enable_os();	// Enable operating system interrupt.
}

// Delete operating system task.
//  function ... task function
void rtos2task_delete(rtos2taskptr function)
{
	struct rtos2task *prev, *current;
	disable_os();	// Disable operating system interrupt.
	if(rtos2running_task && rtos2running_task->function == function)
		rtos2running_task->delete_int = 1;
	while(rtos2ready_tasks && rtos2ready_tasks->function == function)
	{
		current = rtos2ready_tasks;
		rtos2ready_tasks = current->next;
		rtos2mem_free(&rtos2system_memory, current);
	}
	if(rtos2ready_tasks)
		for(prev = rtos2ready_tasks, current = prev->next; current;)
			if(current->function == function)
	{
		prev->next = current->next;
		rtos2mem_free(&rtos2system_memory, current);
		current = prev->next;
	} else
	{
		prev = current;
		current = current->next;
	}
	for(current = rtos2interrupted_tasks; current; current = current->next)
		if(current->function == function) current->delete_int = 1;
	enable_os();	// Enable operating system interrupt.
}
