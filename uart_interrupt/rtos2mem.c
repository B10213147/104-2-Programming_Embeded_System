// Date:   11.5.2009
// Author: Janez Puhan

// Usage example:
//	#define size 500
//	void *ptr;
//	char memory[size];
//	struct rtos2mem region;
//	rtos2mem_create(&region, memory, size);
//	ptr = rtos2mem_alocate(&region, 100);
//	rtos2mem_free(&region, ptr);

#include "rtos2.h"

// Insert memory block into address ordered list.
//  list  ... first memory block in address ordered list
//  block ... memory block address
//  size  ... size of memory block without head in bytes
void rtos2mem_insert_block(struct rtos2mem_block **list,
	struct rtos2mem_block *block, unsigned int size)
{
	struct rtos2mem_block *prev, *current;
	block->size = size;
	if(!*list || *list > block)	// Lowest address.
	{
		block->next = *list;
		*list = block;
	} else	// Search the list.
	{
		for(prev = *list, current = (*list)->next; current && current < block;
			prev = current, current = current->next);
		block->next = current;
		prev->next = block;
	}
}

// Remove memory block from address ordered list.
//  list  ... first memory block in address ordered list
//  block ... memory block address
void rtos2mem_remove_block(struct rtos2mem_block **list,
	struct rtos2mem_block *block)
{
	struct rtos2mem_block *prev, *current;
	if(*list == block) *list = (*list)->next;	// First in the list.
	else	// Search the list.
	{
		for(prev = *list, current = (*list)->next; current != block;
			prev = current, current = current->next);
		prev->next = current->next;
	}
	block->next = 0;
}

// Initialise memory region for dynamic memory allocation.
//  region ... pointer to memory region
//  memory ... available memory
//  size   ... size of available memory in bytes
void rtos2mem_create(struct rtos2mem *region, char *memory, unsigned int size)
{
	while(((int)memory) % sizeof(int))
	{
		memory = memory + 1;
		size = size - 1;
	}
	while(size % sizeof(int)) size = size - 1;
	region->free = 0;
	region->used = 0;
	rtos2mem_insert_block(&region->free, (struct rtos2mem_block *)memory,
		size - sizeof(struct rtos2mem_block));
}

// Allocate memory. Best fitted memory block is used.
//  region ... pointer to memory region
//  size   ... number of bytes to allocate
// Return pointer to allocated memory, or zero if error occurs.
void *rtos2mem_allocate(struct rtos2mem *region, unsigned int size)
{
	struct rtos2mem_block *current, *best = 0;
	unsigned int delta = 0xffffffff;
	while(size % sizeof(int)) size += 1;
	for(current = region->free; current; current = current->next)
	{
		if(current->size >= size &&
			current->size <= size + sizeof(struct rtos2mem_block))
		{
			rtos2mem_remove_block(&region->free, current);
			rtos2mem_insert_block(&region->used, current, current->size);
			return((char *)current + sizeof(struct rtos2mem_block));
		}
		// Fit found.
		if(current->size > size + sizeof(struct rtos2mem_block) &&
			delta > current->size - size)
		{
			delta = current->size - size;
			best = current;
		}
	}
	if(best)
	{
		rtos2mem_remove_block(&region->free, best);
		rtos2mem_insert_block(&region->free, (struct rtos2mem_block *)
			((unsigned int)best + size + sizeof(struct rtos2mem_block)),
			best->size - size - sizeof(struct rtos2mem_block));
		rtos2mem_insert_block(&region->used, best, size);
		return((char *)best + sizeof(struct rtos2mem_block));
	}
	return 0;
}

// Free allocated memory.
//  region ... pointer to memory region
//  ptr    ... pointer to allocated memory
void rtos2mem_free(struct rtos2mem *region, void *ptr)
{
	struct rtos2mem_block *current, *block =
		(struct rtos2mem_block *)((char *)ptr - sizeof(struct rtos2mem_block));
	for(current = region->used; current && current != block;
		current = current->next);
	if(!current) return;
	rtos2mem_remove_block(&region->used, block);
	rtos2mem_insert_block(&region->free, block, block->size);
	if((unsigned int)block + block->size + sizeof(struct rtos2mem_block) ==
		(unsigned int)block->next)
	{
		block->size =
			block->size + block->next->size + sizeof(struct rtos2mem_block);
		rtos2mem_remove_block(&region->free, block->next);
	}
	if(region->free != block)
	{
		for(current = region->free; current->next != block;
			current = current->next);
		if((unsigned int)current + current->size + sizeof(struct rtos2mem_block) ==
			(unsigned int)block)
		{
			current->size =
				current->size + block->size + sizeof(struct rtos2mem_block);
			rtos2mem_remove_block(&region->free, block);
		}
	}
}
