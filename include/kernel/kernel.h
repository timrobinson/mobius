/* $Id: kernel.h,v 1.2 2001/11/05 18:45:23 pavlovskii Exp $ */
#ifndef __KERNEL_KERNEL_H
#define __KERNEL_KERNEL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <os/defs.h>

typedef struct kernel_startup_t kernel_startup_t;
struct kernel_startup_t
{
	uint32_t memory_size;
	uint32_t kernel_size;
	uint32_t kernel_phys;
	uint32_t ramdisk_size;
	uint32_t ramdisk_phys;
	uint32_t kernel_data;
};

struct thread_t;

typedef struct semaphore_t semaphore_t;
struct semaphore_t
{
	uint32_t locks;
	struct thread_t *owner;
	uint32_t old_psw;
};

extern kernel_startup_t kernel_startup;

void	SemInit(semaphore_t *sem);
void	SemAcquire(semaphore_t *sem);
void	SemRelease(semaphore_t *sem);
void	MtxAcquire(semaphore_t *sem);
void	MtxRelease(semaphore_t *sem);

#define CASSERT(exp)	extern char __ERR##__LINE__[(exp)!=0]
#define PHYSICAL(addr)	((void*) ((char*) PHYSMEM + (addr_t) (addr)))

#define LIST_ADD(list, item) \
	if (list##_last != NULL) \
		list##_last->next = item; \
	item->prev = list##_last; \
	item->next = NULL; \
	list##_last = item; \
	if (list##_first == NULL) \
		list##_first = item;

#define LIST_REMOVE(list, item) \
	if (item->next != NULL) \
		item->next->prev = item->prev; \
	if (item->prev != NULL) \
		item->prev->next = item->next; \
	if (list##_first == item) \
		list##_first = item->next; \
	if (list##_last == item) \
		list##_last = item->prev; \
	item->next = item->prev = NULL;

#define FOREACH(item, list) \
	for (item = list##_first; item != NULL; item = item->next)

#ifdef __cplusplus
}
#endif

#endif