/* $Id: arch.c,v 1.17 2002/04/20 12:30:04 pavlovskii Exp $ */

#include <kernel/kernel.h>
#include <kernel/arch.h>
#include <kernel/thread.h>
#include <kernel/sched.h>
#include <kernel/proc.h>
#include <kernel/init.h>
#include <kernel/multiboot.h>

#include <stdio.h>

#include "wrappers.h"

extern char scode[], sbss[], _data_end__[];
extern thread_info_t idle_thread_info;
extern uint16_t con_attribs;
extern thread_t thr_idle;
extern uint8_t arch_df_stack[];

descriptor_t arch_gdt[11];
gdtr_t arch_gdtr;
descriptor_int_t arch_idt[_countof(_wrappers)];
idtr_t arch_idtr;
tss_t arch_tss;
struct tss0_t arch_df_tss;

void i386PitInit(unsigned hz);
void i386PicInit(uint8_t master_vector, uint8_t slave_vector);

#if 0

/*!    \brief Adjusts the value of a pointer so that it can be used before the 
 *    initial kernel GDT is set up.
 */
#define MANGLE_PTR(type, ptr)    ((type) ((char*) ptr - scode))

void ArchStartup(kernel_startup_t* s)
{
    descriptor_t *gdt = MANGLE_PTR(descriptor_t*, arch_gdt);
    gdtr_t *gdtr = MANGLE_PTR(gdtr_t*, &arch_gdtr);
    kernel_startup_t *startup = MANGLE_PTR(kernel_startup_t*, &kernel_startup);

    i386SetDescriptor(gdt + 0, 0, 0, 0, 0);
    /* Based kernel code = 0x8 */
    i386SetDescriptor(gdt + 1, startup->kernel_phys - (addr_t) scode, 
        0xfffff, ACS_CODE | ACS_DPL_0, ATTR_GRANULARITY | ATTR_DEFAULT);
    /* Based kernel data/stack = 0x10 */
    i386SetDescriptor(gdt + 2, startup->kernel_phys - (addr_t) scode, 
        0xfffff, ACS_DATA | ACS_DPL_0, ATTR_GRANULARITY | ATTR_BIG);
    /* Flat kernel code = 0x18 */
    i386SetDescriptor(gdt + 3, 0, 
        0xfffff, ACS_CODE | ACS_DPL_0, ATTR_DEFAULT | ATTR_GRANULARITY);
    /* Flat kernel data = 0x20 */
    i386SetDescriptor(gdt + 4, 0, 
        0xfffff, ACS_DATA | ACS_DPL_0, ATTR_BIG | ATTR_GRANULARITY);
    /* Flat user code = 0x28 */
    i386SetDescriptor(gdt + 5, 0, 
        0xfffff, ACS_CODE | ACS_DPL_3, ATTR_GRANULARITY | ATTR_DEFAULT);
    /* Flat user data = 0x30 */
    i386SetDescriptor(gdt + 6, 0, 
        0xfffff, ACS_DATA | ACS_DPL_3, ATTR_GRANULARITY | ATTR_BIG);
    /* TSS = 0x38 */
    i386SetDescriptor(gdt + 7, 0, 0, 0, 0);
    /* Thread info struct = 0x40 */
    i386SetDescriptor(gdt + 8, (addr_t) &idle_thread_info, 
        sizeof(idle_thread_info) - 1, ACS_DATA | ACS_DPL_3, ATTR_BIG);
    /* Double fault TSS = 0x48 */
    i386SetDescriptor(gdt + 9, 
        (addr_t) &arch_df_tss, 
        sizeof(arch_df_tss) - 1, 
        ACS_TSS, 
        ATTR_BIG);
    
    gdtr->base = (addr_t) gdt + startup->kernel_phys;
    gdtr->limit = sizeof(arch_gdt) - 1;
    __asm__("lgdt (%0)" : : "r" (gdtr));

    __asm__("mov %0,%%ds\n"
        "mov %0,%%ss\n" : : "r" (KERNEL_BASED_DATA));

    __asm__("mov %0,%%es\n"
        "mov %0,%%gs\n"
        "mov %0,%%fs" : : "r" (KERNEL_FLAT_DATA));
    __asm__("add $_scode, %%esp" : : : "esp");
    
    __asm__("ljmp %0,$_KernelMain"
        :
        : "i" (KERNEL_BASED_CODE));
}
#endif

void ArchStartup(multiboot_header_t *header, multiboot_info_t *info)
{
    descriptor_t *gdt = MANGLE_PTR(descriptor_t*, arch_gdt);
    gdtr_t *gdtr = MANGLE_PTR(gdtr_t*, &arch_gdtr);
    kernel_startup_t *startup = MANGLE_PTR(kernel_startup_t*, &kernel_startup);

    startup->memory_size = 0x100000 + info->mem_upper * 1024;
    startup->kernel_phys = KERNEL_PHYS;
    startup->kernel_size = sbss - scode;
    startup->multiboot_info = DEMANGLE_PTR(multiboot_info_t*, info);
    info->mods_addr = DEMANGLE_PTR(uint32_t, info->mods_addr);
    info->mmap_addr = DEMANGLE_PTR(uint32_t, info->mmap_addr);

    i386SetDescriptor(gdt + 0, 0, 0, 0, 0);
    /* Based kernel code = 0x8 */
    i386SetDescriptor(gdt + 1, startup->kernel_phys - (addr_t) scode, 
        0xfffff, ACS_CODE | ACS_DPL_0, ATTR_GRANULARITY | ATTR_DEFAULT);
    /* Based kernel data/stack = 0x10 */
    i386SetDescriptor(gdt + 2, startup->kernel_phys - (addr_t) scode, 
        0xfffff, ACS_DATA | ACS_DPL_0, ATTR_GRANULARITY | ATTR_BIG);
    /* Flat kernel code = 0x18 */
    i386SetDescriptor(gdt + 3, 0, 
        0xfffff, ACS_CODE | ACS_DPL_0, ATTR_DEFAULT | ATTR_GRANULARITY);
    /* Flat kernel data = 0x20 */
    i386SetDescriptor(gdt + 4, 0, 
        0xfffff, ACS_DATA | ACS_DPL_0, ATTR_BIG | ATTR_GRANULARITY);
    /* Flat user code = 0x28 */
    i386SetDescriptor(gdt + 5, 0, 
        0xfffff, ACS_CODE | ACS_DPL_3, ATTR_GRANULARITY | ATTR_DEFAULT);
    /* Flat user data = 0x30 */
    i386SetDescriptor(gdt + 6, 0, 
        0xfffff, ACS_DATA | ACS_DPL_3, ATTR_GRANULARITY | ATTR_BIG);
    /* TSS = 0x38 */
    i386SetDescriptor(gdt + 7, 0, 0, 0, 0);
    /* Thread info struct = 0x40 */
    i386SetDescriptor(gdt + 8, (addr_t) &idle_thread_info, 
        sizeof(idle_thread_info) - 1, ACS_DATA | ACS_DPL_3, ATTR_BIG);
    /* Double fault TSS = 0x48 */
    i386SetDescriptor(gdt + 9, 
        (addr_t) &arch_df_tss, 
        sizeof(arch_df_tss) - 1, 
        ACS_TSS, 
        ATTR_BIG);
    
    gdtr->base = (addr_t) gdt/* + startup->kernel_phys*/;
    gdtr->limit = sizeof(arch_gdt) - 1;
    __asm__("lgdt (%0)" : : "r" (gdtr));

    __asm__("mov %0,%%ds\n"
        "mov %0,%%ss\n" : : "r" (KERNEL_BASED_DATA));

    __asm__("mov %0,%%es\n"
        "mov %0,%%gs\n"
        "mov %0,%%fs" : : "r" (KERNEL_FLAT_DATA));
    __asm__("add %0, %%esp" : : "g" (scode - KERNEL_PHYS) : "esp");
    
    __asm__("ljmp %0,$_KernelMain"
        :
        : "i" (KERNEL_BASED_CODE));
}

static void i386CpuId(uint32_t level, cpuid_info *cpuid)
{
    __asm__(/*"mov %0, %%eax\n"*/
        "cpuid\n"
        /*"mov %%eax, %1\n"
        "mov %%ebx, %2\n"
        "mov %%edx, %3\n"
        "mov %%ecx, %4"*/
        : "=a" (cpuid->regs.eax), "=b" (cpuid->regs.ebx),
            "=d" (cpuid->regs.edx), "=c" (cpuid->regs.ecx)
        : "a" (level)
        /*: "eax", "ebx", "edx", "ecx"*/);
}

/*!
 *    \brief    Initializes the i386 features of the kernel
 *
 *    This function is called just after \p MemInit (so paging is enabled) and
 *    performs the following tasks:
 *    - Rebases GDTR now that paging is enabled
 *    - Sets up the interrupt handlers
 *    - Installs the double fault task gate
 *    - Sets up the user-to-kernel transition TSS
 *    - Sets up the double fault TSS
 *    - Reprograms the PIT to run at 100Hz
 *    - Reprograms the PIC to put IRQs where we want them
 *    - Enables interrupts
 *    - Clears the screen
 *    - Performs a CPUID
 *    - Initializes the \p gdb remote debugging stubs
 *
 *    \return    \p true
 */
bool ArchInit(void)
{
    unsigned i;
    /*uint32_t cpu;*/
    cpuid_info cpuid1, cpuid2;
    uint16_t *ptr;

    arch_gdtr.base = (addr_t) arch_gdt;
    arch_gdtr.limit = sizeof(arch_gdt) - 1;
    __asm__("lgdt %0" : : "m" (arch_gdtr));

    for (i = 0; i < _countof(_wrappers); i++)
        i386SetDescriptorInt(arch_idt + i, 
            KERNEL_FLAT_CODE, 
            (uint32_t) _wrappers[i], 
            ACS_INT | ACS_DPL_3, 
            ATTR_GRANULARITY | ATTR_BIG);
    
    /* Double fault task gate */
    i386SetDescriptorInt(arch_idt + 8, 
        KERNEL_DF_TSS,
        0,
        ACS_TASK | ACS_DPL_0, 
        0);

    /*
     * The value of tss.esp makes no difference until the first task switch is made:
     * - initially, CPU is in ring 0
     * - first timer IRQ occurs -- processor executes timer interrupt
     * - no PL switch so tss.esp0 is ignored
     * - current thread's esp is ignored
     * - when a task switch ocurs, tss.esp0 is updated
     */
    i386SetDescriptor(arch_gdt + 7, (addr_t) &arch_tss, sizeof(arch_tss), 
        ACS_TSS, ATTR_BIG);
    arch_tss.io_map_addr = offsetof(tss_t, io_map);
    arch_tss.ss0 = KERNEL_FLAT_DATA;
    arch_tss.esp0 = thr_idle.kernel_esp;

    arch_df_tss.io_map_addr = sizeof(arch_df_tss);
    arch_df_tss.ss = KERNEL_FLAT_DATA;
    arch_df_tss.esp = (uint32_t) arch_df_stack + PAGE_SIZE;
    arch_df_tss.cs = KERNEL_FLAT_CODE;
    arch_df_tss.eip = (uint32_t) exception_8;
    arch_df_tss.ds = 
        arch_df_tss.es =
        arch_df_tss.fs = 
        arch_df_tss.gs = KERNEL_FLAT_DATA;
    
    __asm__("mov %%cr3, %0"
        : "=r" (arch_df_tss.cr3));

    __asm__("ltr %0"
        :
        : "r" ((uint16_t) KERNEL_TSS));

    arch_idtr.base = (addr_t) arch_idt;
    arch_idtr.limit = sizeof(arch_idt) - 1;
    __asm__("lidt %0"
        :
        : "m" (arch_idtr));

    i386PitInit(100);
    i386PicInit(0x20, 0x28);
    enable();

    ptr = PHYSICAL(0xb8000);
    for (i = 0; i < 80 * 25; i++)
        ptr[i] = con_attribs | ' ';

    wprintf(L"ArchInit: kernel is %uKB (%uKB code)\n",
        kernel_startup.kernel_data / 1024, (_data_end__ - scode) / 1024);

    /*cpu = i386IdentifyCpu();
    switch (cpu)
    {
    case 3:
        wprintf(L"Detected i386 CPU\n");
        break;

    case 4:
        wprintf(L"Detected i486 CPU\n");
        break;

    case 5:*/
        i386CpuId(0, &cpuid1);
        if (cpuid1.eax_0.max_eax > 0)
            i386CpuId(1, &cpuid2);
        else
            memset(&cpuid2, 0, sizeof(cpuid2));

        wprintf(L"Detected %.12S CPU, %u:%u:%u\n",
            cpuid1.eax_0.vendorid, 
            cpuid2.eax_1.stepping, cpuid2.eax_1.model, cpuid2.eax_1.family);
        /*break;
    }*/

    return true;
}

void ArchSetupContext(thread_t *thr, addr_t entry, bool isKernel, 
                      bool useParam, addr_t param, addr_t stack)
{
    context_t *ctx;
    
    thr->kernel_esp = 
        (addr_t) thr->kernel_stack + PAGE_SIZE * 2 - sizeof(context_t);
    thr->user_stack_top = stack - 4;

    ctx = ThrGetContext(thr);
    ctx->kernel_esp = thr->kernel_esp;
    /*ctx->ctx_prev = NULL;*/
    memset(&ctx->regs, 0, sizeof(ctx->regs));
    ctx->regs.eax = 0xaaaaaaaa;
    ctx->regs.ebx = 0xbbbbbbbb;
    ctx->regs.ecx = 0xcccccccc;
    ctx->regs.edx = 0xdddddddd;

    if (isKernel)
    {
        ctx->cs = KERNEL_FLAT_CODE;
        ctx->ds = ctx->es = ctx->gs = ctx->ss = KERNEL_FLAT_DATA;
        ctx->fs = USER_THREAD_INFO;
        ctx->esp = thr->kernel_esp + sizeof(context_t);
    }
    else
    {
        ctx->cs = USER_FLAT_CODE | 3;
        ctx->ds = ctx->es = ctx->gs = ctx->ss = USER_FLAT_DATA | 3;
        ctx->fs = USER_THREAD_INFO | 3;
        ctx->esp = thr->user_stack_top;
    }

#if 0
    /* xxx - this doesn't work */
    if (useParam)
    {
        /* parameter */
        /*ctx->esp -= 4;*/
        *(addr_t*) ctx->esp = param;
        
        /* return address */
        ctx->esp -= 4;
        *(addr_t*) ctx->esp = 0;
    }
#endif

    ctx->eflags = EFLAG_IF | 2;
    ctx->eip = entry;
    ctx->error = (uint32_t) -1;
    ctx->intr = 1;
}

void ArchProcessorIdle(void)
{
    __asm__("hlt");
}

void ArchMaskIrq(uint16_t enable, uint16_t disable)
{
    /*uint16_t mask;
    mask = (uint16_t) ~in(PORT_8259M + 1) 
        | (uint16_t) ~in(PORT_8259S + 1) << 8;
    mask = (mask | enable) & ~disable;
    out(PORT_8259M + 1, ~enable & 0xff);
    out(PORT_8259S + 1, (~enable & 0xff) >> 8);*/
}

void MtxAcquire(semaphore_t *sem)
{
    if (sem->owner != NULL &&
        sem->owner != current)
    {
        while (sem->locks)
            ArchProcessorIdle();
    }

    sem->owner = current;
    sem->locks++;
}

void MtxRelease(semaphore_t *sem)
{
    assert(sem->locks > 0);
    assert(sem->owner == current);

    sem->locks--;
    if (sem->locks == 0)
        sem->owner = NULL;
}

void ArchDbgBreak(void)
{
    __asm__("int3");
}

void ArchDbgDumpContext(const context_t* ctx)
{
    context_v86_t *v86;
    v86 = (context_v86_t*) ctx;
    while (ctx != NULL)
    {
        wprintf(L"---- Context at %p: ----\n", ctx);
        wprintf(L"  EAX %08x\t  EBX %08x\t  ECX %08x\t  EDX %08x\n",
            ctx->regs.eax, ctx->regs.ebx, ctx->regs.ecx, ctx->regs.edx);
        wprintf(L"  ESI %08x\t  EDI %08x\t  EBP %08x\n",
            ctx->regs.esi, ctx->regs.edi, ctx->regs.ebp);
        wprintf(L"   CS %08x\t   DS %08x\t   ES %08x\t   FS %08x\n",
            ctx->cs, ctx->ds, ctx->es, ctx->fs);
        wprintf(L"   GS %08x\t   SS %08x\n",
            ctx->gs, ctx->ss);
        wprintf(L"  EIP %08x\t eflg %08x\t ESP0 %08x\t ESP3 %08x\n", 
            ctx->eip, ctx->eflags, ctx->kernel_esp, ctx->esp);

        if (ctx->eflags & EFLAG_VM)
            wprintf(L"V86DS %08x\tV86ES %08x\tV86FS %08x\tV86GS %08x\n",
                v86->v86_ds, v86->v86_es, v86->v86_fs, v86->v86_gs);
        /*wprintf(L"Previous context: %p\n", ctx->ctx_prev);*/

        /*ArchProcessorIdle();*/
        ctx = ctx->ctx_prev;
    }

    if (ctx != NULL)
        wprintf(L"---- Next context at %p ----\n", ctx);
}

/*!
 *    \brief    Switches to a new execution context and address space
 *
 *    Performs the following tasks:
 *    - Switches address spaces (if necessary)
 *    - Updates the \p ESP0 field in the TSS
 *    - Updates the \p kernel_esp field in the thread's context structure
 *    - Switches the \p FS base address to the new thread's information structure
 *    - Runs any APCs queued on the new thread
 *
 *    If any APCs were run then the thread is paused and a reschedule will need 
 *    to take place (in which case \p ArchAttachToThread will return \p false ).
 *
 *    \param    thr    New thread to switch to
 *    \param    isNewAddressSpace    If \p true , then \p ArchAttachToThread will
 *        switch address spaces as well (thereby invoking a TLB flush)
 *    \return    \p true if the switch went ahead correctly, or \p false if 
 *        reschedule should occur.
 */
bool ArchAttachToThread(thread_t *thr, bool isNewAddressSpace)
{
    context_t *new;
    thread_apc_t *a, *next;
    
    current = thr;
    if (isNewAddressSpace)
        __asm__("mov %0, %%cr3"
            :
            : "r" (thr->process->page_dir_phys));
    
    new = ThrGetContext(thr);
    
    if (new->eflags & EFLAG_VM)
        arch_tss.esp0 = (uint32_t) ((context_v86_t*) new + 1);
    else
        arch_tss.esp0 = (uint32_t) (new + 1);
    
    new->kernel_esp = thr->kernel_esp;
    /*wprintf(L"(%u) %02lx:%08lx %08lx\r", 
        current->id, new->cs, new->eip, current->kernel_esp);*/

    i386SetDescriptor(arch_gdt + 8, 
        (uint32_t) thr->info, 
        sizeof(thread_info_t) - 1, 
        ACS_DATA | ACS_DPL_3, 
        0);

    /*DevRunHandlers();*/
    if (thr->apc_first)
    {
        for (a = thr->apc_first; a; a = next)
        {
            next = a->next;
            LIST_REMOVE(thr->apc, a);
            a->fn(a->param);
            free(a);
            ThrPause(current);
        }

        return false;
    }

    return true;
}

void ArchReboot(void)
{
    uint8_t temp;
    disable();

    /* flush the keyboard controller */
    do
    {
	temp = in(0x64);
	if (temp & 1)
	    in(0x60);
    } while(temp & 2);

    /* pulse the CPU reset line */
    out(0x64, 0xFE);

    ArchProcessorIdle();
}

static unsigned short ctc(void)
{
    union
    {
        unsigned char byte[2];
        signed short word;
    } ret_val;

    semaphore_t sem = { 0 };

    SemAcquire(&sem);
    disable();
    out(0x43, 0x00);               /* latch channel 0 value */
    ret_val.byte[0] = in(0x40);    /* read LSB */
    ret_val.byte[1] = in(0x40);    /* read MSB */
    SemRelease(&sem);
    return -ret_val.word;          /* convert down count to up count */
}

void ArchMicroDelay(unsigned microseconds)
{
    unsigned short curr, prev;
    union
    {
        unsigned short word[2];
        unsigned long dword;
    } end;

    prev = ctc();
    end.dword = prev + ((unsigned long)microseconds * 500) / 419;
    for (;;)
    {
        curr = ctc();
        if (end.word[1] == 0)
        {
            if (curr >= end.word[0])
                break;
        }
        if (curr < prev)                /* timer overflowed */
        {
            if (end.word[1] == 0)
                break;
            (end.word[1])--;
        }
        prev = curr;
    }
}
