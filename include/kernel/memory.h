/* $Id: memory.h,v 1.8 2002/09/13 23:13:03 pavlovskii Exp $ */
#ifndef __KERNEL_MEMORY_H
#define __KERNEL_MEMORY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <sys/types.h>
#include <kernel/arch.h>
#include <kernel/kernel.h>

/*!
 *	\ingroup	kernel
 *	\defgroup	mem	Physical Memory
 *	@{
 */

#if 0
typedef struct page_pool_t page_pool_t;
/*!	Describes a stack of physical pages */
struct page_pool_t
{
    spinlock_t sem;
    /*! Stack of addresses, describing each page in the pool */
    addr_t *pages;
    /*! The number of pages in memory, i.e. memory_top / PAGE_SIZE */
    unsigned num_pages;
    /*! The number of free pages in the stack */
    unsigned free_pages;
};

extern page_pool_t pool_all, pool_low;
#endif

#define LOW_MEMORY		0x100000
/* pool_low contains all the pages below 1MB */
#define NUM_LOW_PAGES	(LOW_MEMORY >> PAGE_BITS)

#define PAGE_ALIGN(addr)	((addr) & -PAGE_SIZE)
#define PAGE_ALIGN_UP(addr)	(((addr) + PAGE_SIZE - 1) & -PAGE_SIZE)

typedef struct page_array_t page_array_t;
struct page_array_t
{
    unsigned refs;
    unsigned num_pages;
    size_t mod_first_page;
    addr_t pages[1];
};

typedef unsigned pfn_t;
#define PHYS_TO_PFN(p)          ((p) >> PAGE_BITS)
#define PFN_TO_PHYS(p)          ((p) << PAGE_BITS)
#define PFN_NULL                ((pfn_t) -1)

typedef struct page_phys_t page_phys_t;
struct page_phys_t
{
    uint8_t flags;

    union
    {
        struct
        {
            pfn_t prev, next;
        } list;

        struct
        {
            struct vm_desc_t *desc;
            addr_t offset;
        } alloc_vm;
    } u;
};

#define PAGE_STATE_MASK         0x03
#define PAGE_STATE_FREE         0x00
#define PAGE_STATE_ZERO         0x01
#define PAGE_STATE_ALLOCATED    0x02
#define PAGE_STATE_RESERVED     0x03

#define PAGE_FLAG_MASK          0x0C
#define PAGE_FLAG_LOW           0x04
#define PAGE_FLAG_LARGE         0x08

typedef struct pfn_list_t pfn_list_t;
struct pfn_list_t
{
    spinlock_t spin;
    pfn_t first, last;
    unsigned num_pages;
};

extern pfn_list_t mem_free, mem_free_low, mem_zero;

addr_t	MemAlloc(void);
void	MemFree(addr_t block);
addr_t	MemAllocLow(void);
void	MemFreeLow(addr_t block);
addr_t	MemAllocLowSpan(size_t pages);
addr_t  MemAllocFromList(pfn_list_t *list);
bool	MemMap(addr_t virt, addr_t phys, addr_t virt_end, uint16_t priv);
bool    MemSetPageState(const void *virt, uint16_t state);
uint32_t MemTranslate(const void *address);
uint16_t MemGetPageState(const void *address);
void *	MemMapTemp(const addr_t *phys, unsigned num_pages, uint8_t priv);
void	MemUnmapTemp(void);
addr_t	MemAllocPageDir(void);
bool	MemVerifyBuffer(const void *buf, size_t bytes);
bool	MemLockPages(addr_t phys, unsigned pages, bool do_lock);
bool    MemIsPageLocked(addr_t phys);
/*page_array_t *MemCopyPageArray(unsigned num_pages, size_t mod_first_page, 
                               const addr_t *pages);*/
page_array_t *MemCopyPageArray(page_array_t *a);
page_array_t *MemDupPageArray(unsigned num_pages, size_t mod_first_page, 
                              const addr_t *pages);
page_array_t *MemCreatePageArray(const void *buf, size_t bytes);
void    MemDeletePageArray(page_array_t *array);
void *  MemMapPageArray(page_array_t *array, uint16_t state);
page_phys_t *MemLookupAllocatedPage(struct vm_desc_t *desc, addr_t offset);
void    MemZeroPageThread(void);

/*! @} */

#ifdef __cplusplus
}
#endif

#endif