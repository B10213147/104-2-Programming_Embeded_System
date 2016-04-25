// Date:   1.1.2012
// Author: Janez Puhan

#include "timer.h"
#include "rtos2.h"

int slice_quantum;		// Number of VPB cycles per timeslice.
int num_of_empty = 1;	// Number of timeslices from the last non-empty timeslice to
						// the next one.

// Skip passed timeslices. Empty timeslices without a ready to be scheduled task are
// included. Determine next timeslice with a ready to be scheduled task.
// Return list of ready to be scheduled tasks. Task with higher priority precedes.
// In case of the same priority task with larger delay from the scheduling moment
// precedes.
struct rtos2task_list *rtos2sched_task_update()
{
	int next = 0x7fffffff;	// Number of timeslices to the next non-empty timeslice.
	struct rtos2task *task;
	struct rtos2task_list *another, *cur, *prev, *list = 0;
	// Decrement number of remaining timeslices for tasks waiting to be scheduled.
	for(task = rtos2ready_tasks; task; task = task->next)
	{
		task->remain_ticks = task->remain_ticks - num_of_empty;
		if(task->remain_ticks > 0)	// Waiting time not expired yet.
		{
			if(next > task->remain_ticks) next = task->remain_ticks;
		} else	// Another ready to be scheduled task.
		{
			another = rtos2mem_allocate(&rtos2system_memory,
				sizeof(struct rtos2task_list));
			another->next = 0;
			another->task = task;
			// Insert into a list of ready tasks.
			for(prev = 0, cur = list; cur; prev = cur, cur = cur->next)
				if(task->priority < cur->task->priority ||
					task->priority == cur->task->priority &&
					task->remain_ticks < cur->task->remain_ticks)
			{
				if(prev) prev->next = another;
				else list = another;
				another->next = cur;
				break;
			}
			if(!cur) if(prev) prev->next = another;
			else list = another;
			if(next > task->remain_ticks + task->interval)
				next = task->remain_ticks + task->interval;
		}
	}
	// Decrement number of remaining timeslices for already scheduled tasks.
	if(rtos2running_task)
	{
		rtos2running_task->remain_ticks =
			rtos2running_task->remain_ticks - num_of_empty;
		if(next > rtos2running_task->remain_ticks + rtos2running_task->interval)
			next = rtos2running_task->remain_ticks + rtos2running_task->interval;
	}
	for(task = rtos2interrupted_tasks; task; task = task->next)
	{
		task->remain_ticks = task->remain_ticks - num_of_empty;
		if(next > task->remain_ticks + task->interval)
			next = task->remain_ticks + task->interval;
	}
	// If a task is ready to be scheduled again or there are no tasks then the next
	// timeslice is not empty.
	if(next < 1 || next == 0x7fffffff) next = 1;
	T0MR0 = T0MR0 + next * slice_quantum;	// Set next non-empty slice beginning.
	if(T0TC - T0MR0 > 0) T0TC = T0MR0 - slice_quantum;
	num_of_empty = next;
	return list;
}

// Run the task.
void rtos2sched_run(struct rtos2task *task)
{
	// Remove "ready to run" task form ready list.
	rtos2task_remove(&rtos2ready_tasks, task);
	rtos2running_task = task;
	enable_os();	// Enable operating system interrupt.
	rtos2running_task->function(rtos2running_task->arg);
	disable_os();	// Disable operating system interrupt.
	// While running, task can be deleted. If so, it cannot be inserted back to
	// ready tasks after resume.
	if(rtos2running_task->delete_int)
		rtos2mem_free(&rtos2system_memory, rtos2running_task);
	else
	{
		rtos2running_task->remain_ticks =
			rtos2running_task->remain_ticks + rtos2running_task->interval;
		rtos2task_insert(&rtos2ready_tasks, rtos2running_task);
	}
}

// Remove first task from ready to be scheduled list. Return removed task.
struct rtos2task *rtos2sched_remove(struct rtos2task_list **list)
{
	struct rtos2task *task = (*list)->task;
	struct rtos2task_list *next = (*list)->next;
	rtos2mem_free(&rtos2system_memory, *list);
	*list = next;
	return task;
}

// Operating system scheduler.
void rtos2sched()
{
	static struct rtos2task_list *list = 0;
	while(list) rtos2sched_remove(&list);	// Release current list of ready tasks.
	list = rtos2sched_task_update();	// Create new list of ready tasks.
	while(list) if(rtos2running_task) 	// Next task ready to run.
		// Previous task has not finished yet.
		if(list->task->priority < rtos2running_task->priority)	// Interrupt task.
	{
		// Insert currently running task into interrupted list.
		rtos2task_insert(&rtos2interrupted_tasks, rtos2running_task);
		rtos2sched_run(rtos2sched_remove(&list));
		rtos2running_task = rtos2interrupted_tasks; // Resume interrupted task.
		rtos2task_remove(&rtos2interrupted_tasks, rtos2running_task);
	} else break;
	else	// Previous task has finished.
	{
		rtos2sched_run(rtos2sched_remove(&list));
		rtos2running_task = 0;
	}
}
