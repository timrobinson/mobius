/* $Id: console.c,v 1.1 2002/03/05 02:10:21 pavlovskii Exp $ */

#include <errno.h>

#include <os/rtl.h>
#include <os/syscall.h>

uint32_t ConReadKey(void)
{
    handle_t keyb;
    fileop_t op;
    uint32_t key;

    keyb = ProcGetProcessInfo()->std_in;

    op.event = keyb;
    if (!FsRead(keyb, &key, sizeof(key), &op))
    {

	errno = op.result;
	return -1;
    }

    if (op.result == SIOPENDING)
	ThrWaitHandle(op.event);

    if (op.result != 0 || op.bytes == 0)
    {
	errno = op.result;
	return -1;
    }
    
    return key;
}
