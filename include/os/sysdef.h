/* $Id: sysdef.h,v 1.4 2002/02/24 19:13:11 pavlovskii Exp $ */
#ifdef KERNEL

/* The kernel uses different names for some functions... */
#define ThrWaitHandle	SysThrWaitHandle
#define ThrSleep		SysThrSleep
#define EvtAlloc		SysEvtAlloc
#define EvtSignal		SysEvtSignal
#define EvtIsSignalled	SysEvtIsSignalled
#define HndClose		SysHndClose
#endif

#ifndef SYS_BEGIN_GROUP
#define SYS_BEGIN_GROUP(n)
#endif

#ifndef SYS_END_GROUP
#define SYS_END_GROUP(n)
#endif

/*!
 *	\ingroup	libsys
 *	\defgroup	sys	Syscall Interface
 *	@{
 */

#define SYS_DbgWrite			0x100
#define SYS_Hello				0x101
#define SYS_SysUpTime			0x102

#define SYS_ThrExitThread		0x200
#define SYS_ThrWaitHandle		0x201
#define SYS_ThrSleep			0x202

#define SYS_ProcExitProcess		0x300
#define SYS_ProcSpawnProcess	0x301

#define SYS_FsCreate			0x400
#define SYS_FsOpen				0x401
#define SYS_FsClose				0x402
#define SYS_FsRead				0x403
#define SYS_FsWrite				0x404
#define SYS_FsSeek				0x405
/*#define SYS_FsRequestSync		0x406*/
#define SYS_FsOpenSearch		0x406

#define SYS_VmmAlloc			0x500

#define SYS_EvtAlloc			0x600
#define SYS_HndClose			0x601
#define SYS_EvtSignal			0x602
#define SYS_EvtIsSignalled		0x603

/* 0 */
SYS_BEGIN_GROUP(0)
SYS_END_GROUP(0)

/* 1 */
SYS_BEGIN_GROUP(1)
SYSCALL(int, DbgWrite, 8, const wchar_t*, size_t)
SYSCALL(int, Hello, 8, int, int)
SYSCALL(unsigned, SysUpTime, 0, void)
SYS_END_GROUP(1)

/* 2 */
SYS_BEGIN_GROUP(2)
SYSCALL(void, ThrExitThread, 4, int)
SYSCALL(bool, ThrWaitHandle, 4, handle_t)
SYSCALL(void, ThrSleep, 4, unsigned)
SYS_END_GROUP(2)

/* 3 */
SYS_BEGIN_GROUP(3)
SYSCALL(void, ProcExitProcess, 4, int)
SYSCALL(handle_t, ProcSpawnProcess, 4, const wchar_t*)
SYS_END_GROUP(3)

/* 4 */
SYS_BEGIN_GROUP(4)
SYSCALL(handle_t, FsCreate, 8, const wchar_t*, uint32_t)
SYSCALL(handle_t, FsOpen, 8, const wchar_t*, uint32_t)
SYSCALL(bool, FsClose, 4, handle_t)
SYSCALL(bool, FsRead, 16, handle_t, void*, size_t, struct fileop_t*)
SYSCALL(bool, FsWrite, 16, handle_t, const void*, size_t, struct fileop_t*)
SYSCALL(addr_t, FsSeek, 8, handle_t, addr_t)
/*SYSCALL(bool, FsRequestSync, 8, handle_t, struct request_t*)*/
SYSCALL(handle_t, FsOpenSearch, 4, const wchar_t*)
SYS_END_GROUP(4)

/* 5 */
SYS_BEGIN_GROUP(5)
SYSCALL(void *, VmmAlloc, 12, size_t, addr_t, uint32_t)
SYS_END_GROUP(5)

/* 6 */
SYS_BEGIN_GROUP(6)
SYSCALL(handle_t, EvtAlloc, 0, void)
SYSCALL(bool, HndClose, 4, handle_t)
SYSCALL(void, EvtSignal, 4, handle_t)
SYSCALL(bool, EvtIsSignalled, 4, handle_t)
SYS_END_GROUP(6)

/*! @} */

#ifdef KERNEL
#undef ThrWaitHandle
#undef ThrSleep
#undef EvtAlloc
#undef EvtSignal
#undef EvtIsSignalled
#undef HndClose
#endif
