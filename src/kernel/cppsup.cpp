#include <kernel/kernel.h>

void *operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void *p)
{
	free(p);
}

extern "C" void *__get_eh_context(void)
{
	static unsigned int temp[2];
	return temp;
}
