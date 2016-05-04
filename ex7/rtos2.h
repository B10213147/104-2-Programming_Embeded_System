// Date:   1.1.2012
// Author: Janez Puhan

#define irq_mask			0x00000080
#define irq_and_fiq_mask	0x000000c0

typedef void (* rtos2taskptr)(void *);	// Task's function pointer.

// Data structures
struct rtos2task
{
	struct rtos2task *next;	// Next task in the list.
	rtos2taskptr function;	// Task's function pointer.
	void *arg;				// Task's argument data.
	unsigned int priority;	// Task's priority.
	int remain_ticks;		// Remaining sleep timeslices.
	int interval;			// Number of timeslices between two calls.
	char delete_int;		// Delete interrupted task flag.
};

// List of tasks
struct rtos2task_list
{
	struct rtos2task *task;			// Pointer to task's structure.
	struct rtos2task_list *next;	// Next task in the list.
};

struct rtos2mem_block
{
	struct rtos2mem_block *next;	// Next memory block.
	unsigned int size;				// Size in bytes.
};

struct rtos2mem
{
	struct rtos2mem_block *free;	// List of free memory blocks.
	struct rtos2mem_block *used;	// List of used memory blocks.
};

struct rtos2pipe
{
	char *buffer;		// Pipe buffer.
	unsigned int size;	// Size of allocated pipe buffer.
	unsigned int head;	// Pipe write buffer position.
	unsigned int tail;	// Pipe read buffer position.
};

// Global variables
extern int slice_quantum;
extern int num_of_empty;
extern struct rtos2task *rtos2running_task;
extern struct rtos2task *rtos2ready_tasks;
extern struct rtos2task *rtos2interrupted_tasks;
extern struct rtos2mem rtos2system_memory;

// Function declarations
extern void disable_os();
extern void enable_os();
extern void rtos2init(char *memory, unsigned int size, unsigned int slice);
extern void rtos2sched();
extern void rtos2task_remove(struct rtos2task **list, struct rtos2task *task);
extern void rtos2task_insert(struct rtos2task **list, struct rtos2task *task);
extern void rtos2task_create(rtos2taskptr function, void *arg,
	unsigned int priority, int interval);
extern void rtos2task_delete(rtos2taskptr function);
extern void rtos2mem_create(struct rtos2mem *region, char *memory,
	unsigned int size);
extern void *rtos2mem_allocate(struct rtos2mem *region, unsigned int size);
extern void rtos2mem_free(struct rtos2mem *region, void *ptr);
extern struct rtos2pipe *rtos2pipe_create(unsigned int size);
extern void rtos2pipe_delete(struct rtos2pipe *pipe);
extern unsigned int rtos2pipe_write(struct rtos2pipe *pipe, char *data,
	unsigned int size);
extern unsigned int rtos2pipe_read(struct rtos2pipe *pipe, char *buffer,
	unsigned int size);
