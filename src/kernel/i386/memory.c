/* $Id: memory.c,v 1.11 2002/02/25 18:42:09 pavlovskii Exp $ */
#include <kernel/kernel.h>
#include <kernel/memory.h>
#include <kernel/thread.h>
#include <kernel/proc.h>
#include <kernel/init.h>
#include <kernel/multiboot.h>

#include <string.h>

#define MEM_TEMP_START	0xf8000000

extern addr_t kernel_pagedir[];
extern char scode[], ebss[], edata[], _data_start__[];
extern process_t proc_idle;

page_pool_t pool_low, pool_all;
addr_t mem_temp_end = MEM_TEMP_START;
uint8_t *locked_pages;

bool mem_ready;

static addr_t MemAllocPool(page_pool_t *pool)
{
	addr_t addr;

	SemAcquire(&pool->sem);
	do
	{
		if (pool->free_pages <= 0)
			return NULL;
		
		addr = pool->pages[--pool->free_pages];
	} while (addr == (addr_t) -1);
	SemRelease(&pool->sem);

	return addr;
}

static void MemFreePool(page_pool_t *pool, addr_t block)
{
	SemAcquire(&pool->sem);
	pool->pages[pool->free_pages++] = block;
	SemRelease(&pool->sem);
}

static void MemFreePoolRange(page_pool_t *pool, addr_t start, addr_t end)
{
	SemAcquire(&pool->sem);

	start = PAGE_ALIGN(start);
	end = PAGE_ALIGN(end);
	for (; start < end; start += PAGE_SIZE)
		pool->pages[pool->free_pages++] = start;

	SemRelease(&pool->sem);
}

/*!
 * \brief	Allocates one physical page
 */
addr_t MemAlloc(void)
{
	return MemAllocPool(&pool_all);
}

/*!
 * \brief	Frees one physical page
 */
void MemFree(addr_t block)
{
	MemFreePool(&pool_all, block);
}

/*!
 * \brief	Allocates one physical page from the low memory pool
 */
addr_t MemAllocLow(void)
{
	return MemAllocPool(&pool_low);
}

/*!
 * \brief	Frees one low memory physical page
 */
void MemFreeLow(addr_t block)
{
	MemFreePool(&pool_low, block);
}

/*!
 * \brief	Allocates a contiguous span of low-memory pages
 */
addr_t MemAllocLowSpan(size_t pages)
{
	unsigned start, i;
	addr_t addr;
	
	SemAcquire(&pool_low.sem);
	for (start = 0; start < pool_low.num_pages; start++)
	{
		for (i = 0; i < pages; i++)
			if (pool_low.pages[start + i] == NULL ||
				(i > 0 && pool_low.pages[start + i] != 
					pool_low.pages[start + i - 1] + PAGE_SIZE))
				break;

		if (i >= pages)
		{
			addr = pool_low.pages[start];

			for (i = 0; i < pages; i++)
				pool_low.pages[start + i] = NULL;
			
			SemRelease(&pool_low.sem);
			return addr;
		}
	}

	SemRelease(&pool_low.sem);
	return NULL;
}

/*!
 * \brief	Creates a virtual-to-physical mapping in the current address space
 */
bool MemMap(addr_t virt, addr_t phys, addr_t virt_end, uint8_t priv)
{
	addr_t *pde;
	
	virt = PAGE_ALIGN(virt);
	phys = PAGE_ALIGN(phys);
	virt_end = PAGE_ALIGN(virt_end);
	
	for (; virt < virt_end; virt += PAGE_SIZE, phys += PAGE_SIZE)
	{
		pde = ADDR_TO_PDE(virt);

		/*if (mem_ready && virt < 0x400000)
			wprintf(L"MemMap: mapping %lx => %lx pde = %lx\n",
				virt, phys, *pde);*/

		if (*pde == NULL)
		{
			addr_t pt;

			pt = MemAlloc();

			if (pt == NULL)
				return false;

			*pde = pt | PRIV_WR | PRIV_USER | PRIV_PRES;
		}

		*ADDR_TO_PTE(virt) = phys | priv;
		__asm__("invlpg %0"
			:
			: "m" (virt));
	}

	return true;
}

/*!
 * \brief	Returns the physical address associated with a virtual page
 */
uint32_t MemTranslate(const void *address)
{
	if (*ADDR_TO_PDE((addr_t) address))
		return *ADDR_TO_PTE((addr_t) address);
	else
		return 0;
}

/*!
 * \brief	Creates a temporary physical-to-virtual mapping
 */
void *MemMapTemp(const addr_t *phys, unsigned num_pages, uint8_t priv)
{
	unsigned i;
	void *ptr;

	if (num_pages == 1 &&
		*phys < 0x08000000)
		return PHYSICAL(*phys);

	ptr = (void*) mem_temp_end;
	for (i = 0; i < num_pages; i++)
	{
		if (!MemMap(mem_temp_end, phys[i], mem_temp_end + PAGE_SIZE, priv))
			return NULL;

		mem_temp_end += PAGE_SIZE;
	}

	return ptr;
}

/*!
 * \brief	Unmaps all temporary physical-to-virtual mappings
 */
void MemUnmapTemp(void)
{
	MemMap(MEM_TEMP_START, 0, mem_temp_end, 0);
	mem_temp_end = MEM_TEMP_START;
}

/*!
 * \brief	Allocates a page directory
 */
addr_t MemAllocPageDir(void)
{
	addr_t page_dir, *pd;
	/*unsigned i;
	bool fail;*/

	page_dir = MemAlloc();
	pd = MemMapTemp(&page_dir, 1, PRIV_KERN | PRIV_WR | PRIV_PRES);
	memset(pd, 0, 512 * sizeof(*pd));
	memcpy(pd + 512, kernel_pagedir + 512, 511 * sizeof(*pd));
	pd[1023] = page_dir | PRIV_KERN | PRIV_WR | PRIV_PRES;

	/*for (i = 0; i < 512; i++)
	{
		if (pd[i] != 0)
		{
			wprintf(L"MemAllocPageDir: pd[%d] == 0x%x\n", i, pd[i]);
			fail = true;
		}
	}

	assert(!fail);*/
	MemUnmapTemp();

	return page_dir;
}

/*!
 * \brief	Verifies that the whole of a virtual buffer is accessible
 */
bool MemVerifyBuffer(const void *buf, size_t bytes)
{
	addr_t virt;

	virt = PAGE_ALIGN((addr_t) buf);
	bytes = PAGE_ALIGN_UP(bytes);
	for (; bytes > 0; bytes -= PAGE_SIZE, virt += PAGE_SIZE)
		if (MemTranslate((const void*) virt) == NULL)
			return false;

	return true;
}

/*!
 * \brief	Locks or unlocks a region of physical memory
 */
bool MemLockPages(addr_t phys, unsigned pages, bool do_lock)
{
	unsigned index;
	int d;

	phys = PAGE_ALIGN(phys);
	index = phys / PAGE_SIZE;
	d = do_lock ? 1 : -1;
	
	for (; pages > 0; pages--, index++, phys += PAGE_SIZE)
	{
		/*if (phys > kernel_startup.memory_size)
			return false;*/

		assert(phys < kernel_startup.memory_size);
		locked_pages[index] += d;
	}

	return true;
}

/*!
 * \brief Initializes the physical memory manager
 *
 *	This is the first function called by \p KernelMain. Its purpose is to get
 *	the CPU into an environment suitable for running the rest of the kernel,
 *	and performs the following tasks:
 *
 *	- Sets aside physical page pools for main and low memory
 *	- Distinguishes between usable and unusable memory
 *	- Sets up the initial kernel page directory
 *	- Enables paging
 *	- Maps the bottom 128MB of memory into the region at 0xF0000000 
 *		(= \p PHYSMEM )
 *
 *	\return	\p true
 */
bool MemInit(void)
{
	unsigned num_pages;
	addr_t *pages, pt1, pt2, phys;
	uint32_t entry;
	size_t kernel_code;
	unsigned i;
	/*memory_map_t *map;*/
	multiboot_module_t *mod;

	num_pages = kernel_startup.memory_size / PAGE_SIZE;
	
	/*
	 * Total kernel size composed of:
	 *	Kernel file image
	 *	bss
	 *	Page address stack
	 *	Page lock counts
	 */
	kernel_startup.kernel_data = kernel_startup.kernel_size 
		+ ebss - edata;
		/*+ num_pages * sizeof(addr_t)
		+ num_pages;*/
	kernel_startup.kernel_data = PAGE_ALIGN_UP(kernel_startup.kernel_data);

	/* One page of PTEs at one page per PTE = 4MB */
	assert(kernel_startup.kernel_data < PAGE_SIZE * PAGE_SIZE / sizeof(uint32_t));

	/* Page address stack goes after the bss */
	/*pages = (addr_t*) ebss;*/
	pages = DEMANGLE_PTR(addr_t*, 0x5000);
	/*memset(pages, 0, num_pages * sizeof(addr_t));*/
		
	/* Page lock counts go after the stack */
	locked_pages = (uint8_t*) (pages + num_pages);

	/* xxx - why does memset not work here? */
	/*memset(locked_pages, 0, num_pages);*/
	for (i = 0; i < num_pages; i++)
	{
		pages[i] = 0;
		locked_pages[i] = 0;
	}
	
	pool_all.num_pages = num_pages - NUM_LOW_PAGES;
	pool_all.pages = pages + NUM_LOW_PAGES;
	pool_all.free_pages = 0;

	pool_low.num_pages = NUM_LOW_PAGES;
	pool_low.pages = pages;
	pool_low.free_pages = 0;

	/* Memory from BDA=>ROMs */
	entry = 0x5000 + PAGE_ALIGN_UP(num_pages * sizeof(addr_t) + num_pages);
	assert(entry <= 0xA0000);
	MemFreePoolRange(&pool_low, entry, 0xA0000);
	/* Memory from top of ROMs to top of 'low memory' */
	MemFreePoolRange(&pool_low, 0x100000, LOW_MEMORY);

	MemLockPages(kernel_startup.kernel_phys, 
		PAGE_ALIGN_UP(kernel_startup.kernel_data) / PAGE_SIZE, 
		true);
	mod = (multiboot_module_t*) kernel_startup.multiboot_info->mods_addr;
	for (i = 0; i < kernel_startup.multiboot_info->mods_count; i++)
		MemLockPages(mod[i].mod_start, 
			PAGE_ALIGN_UP(mod[i].mod_end - mod[i].mod_start) / PAGE_SIZE,
			true);

	/*if (kernel_startup.multiboot_info->mmap_length > 0)
	{
		map = (memory_map_t*) kernel_startup.multiboot_info->mmap_addr;
		while ((uint32_t) map < kernel_startup.multiboot_info->mmap_addr + 
			kernel_startup.multiboot_info->mmap_length)
		{
			if (map->type != 1 &&
				map->length_low != 0 &&
				map->base_addr_high == 0)
				MemLockPages(map->base_addr_low, 
					PAGE_ALIGN_UP(map->length_low) / PAGE_SIZE, true);
			
			map = (memory_map_t*) ((uint8_t*) map + ((uint32_t*) map)[-1]);
		}
	}*/

	for (i = NUM_LOW_PAGES; i < num_pages; i++)
		if (locked_pages[i] == 0)
			MemFreePool(&pool_all, i * PAGE_SIZE);

	pool_low.num_pages = pool_low.free_pages;
	pool_all.num_pages = pool_all.free_pages;
	
	/*
	 * Allocate one page table and patch the entries in the page directory:
	 *	- at 0xC0000000 for scode
	 *	- at kernel_phys for identity mapping
	 */
	
	pt1 = MemAlloc();
	kernel_pagedir[PAGE_DIRENT((addr_t) scode)] = pt1 | PRIV_WR | PRIV_USER | PRIV_PRES;
	
	pt2 = MemAlloc();
	kernel_pagedir[PAGE_DIRENT(kernel_startup.kernel_phys)] = pt2 | PRIV_WR | PRIV_USER | PRIV_PRES;
	
	kernel_code = _data_start__ - scode;
	
	/* Map code read-only */
	for (phys = 0; 
		phys < kernel_code;  
		phys += PAGE_SIZE)
	{
		entry = PAGE_TABENT((addr_t) scode + phys);
		i386_lpoke32(pt1 + entry * 4, 
			(kernel_startup.kernel_phys + phys) | PRIV_KERN | PRIV_PRES);

		entry = PAGE_TABENT((addr_t) kernel_startup.kernel_phys + phys);
		i386_lpoke32(pt2 + entry * 4, 
			(kernel_startup.kernel_phys + phys) | PRIV_KERN | PRIV_PRES);
	}

	/* Map data read-write */
	for (; 
		phys < kernel_startup.kernel_data; 
		phys += PAGE_SIZE)
	{
		entry = PAGE_TABENT((addr_t) scode + phys);
		i386_lpoke32(pt1 + entry * 4, 
			(kernel_startup.kernel_phys + phys) | PRIV_KERN | PRIV_WR | PRIV_PRES);

		entry = PAGE_TABENT((addr_t) kernel_startup.kernel_phys + phys);
		i386_lpoke32(pt2 + entry * 4, 
			(kernel_startup.kernel_phys + phys) | PRIV_KERN | PRIV_WR | PRIV_PRES);
	}

	/* Calculate physical address of kernel page directory */
	phys = kernel_startup.kernel_phys 
		+ (addr_t) kernel_pagedir 
		- (addr_t) scode;
	proc_idle.page_dir_phys = phys;

	/* Set up the last PDE to point to the page directory itself */
	kernel_pagedir[1023] = phys | PRIV_WR | PRIV_KERN | PRIV_PRES;

	/*
	 * xxx - Chicken and egg situation here
	 * Need to map PHYSMEM, in order to access pool_all.pages, before we can 
	 *	call MemMap() later.
	 */
	entry = PAGE_TABENT(PHYSMEM);
	pt1 = MemAlloc();
	memset(DEMANGLE_PTR(void*, pt1), 0, PAGE_SIZE);
	kernel_pagedir[PAGE_DIRENT(PHYSMEM)] = pt1 | PRIV_WR | PRIV_USER | PRIV_PRES;

	/* Load CR3 */
	__asm__("mov %0, %%cr3" 
		: 
		: "r" (phys));

	/* Enable paging... */
	/* xxx - set 486 CE and WP bits */
	__asm__("mov %cr0, %eax\n"
		/*"or $0x80000000, %eax\n"*/
		"or $0xC0010000, %eax\n"
		"mov %eax, %cr0");

	/* Reload the flat selectors */
	__asm__("mov %0, %%ds\n"
		"mov %0, %%es\n"
		"mov %1, %%fs\n"
		"mov %0, %%gs\n"
		"mov %0, %%ss\n"
		:
		: "r" (KERNEL_FLAT_DATA),
			"r" (USER_THREAD_INFO));

	/* Reload CS with the new flat selector */
	__asm__("ljmp %0,$_paging\n"
		"_paging:"
		:
		: "i" (KERNEL_FLAT_CODE));

	mem_ready = true;

	/* Create a page table for the bottom 4MB */
	/*pt1 = MemAlloc();
	kernel_pagedir[0] = pt1 | PRIV_WR | PRIV_KERN | PRIV_PRES;
	*ADDR_TO_PTE(0xb8000) = 0xb8000 | PRIV_WR | PRIV_PRES;
	__asm__("invlpg %0"
		:
		: "m" (0xb8000));*/

	pool_all.pages = PHYSICAL(MANGLE_PTR(void*, pool_all.pages));
	pool_low.pages = PHYSICAL(MANGLE_PTR(void*, pool_low.pages));
	locked_pages = PHYSICAL(MANGLE_PTR(void*, locked_pages));

	/* Map at most 128MB of physical memory */
	MemMap(PHYSMEM, 
		0, 
		PHYSMEM + min(kernel_startup.memory_size, 0x08000000), 
		PRIV_WR | PRIV_PRES | PRIV_KERN);

	kernel_startup.multiboot_info = 
		PHYSICAL(MANGLE_PTR(void*, kernel_startup.multiboot_info));
	kernel_startup.multiboot_info->mods_addr = 
		(addr_t) PHYSICAL(MANGLE_PTR(void*, kernel_startup.multiboot_info->mods_addr));
	kernel_startup.multiboot_info->mmap_addr = 
		(addr_t) PHYSICAL(MANGLE_PTR(void*, kernel_startup.multiboot_info->mmap_addr));
	return true;
}
