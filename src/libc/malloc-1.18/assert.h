/* $Id: assert.h,v 1.2 2002/05/12 00:13:20 pavlovskii Exp $ */
#ifndef __ASSERT_H__
#define __ASSERT_H__
#ifdef DEBUG
#define ASSERT(p, s) \
	((void)(!(p)?__m_botch((s),"",(univptr_t)0,0,__FILE__,__LINE__):0))
#define ASSERT_SP(p, s, s2, sp) \
	((void)(!(p)?__m_botch((s),(s2),(univptr_t)(sp),0,__FILE__,__LINE__):0))
#define ASSERT_EP(p, s, s2, ep) \
	((void)(!(p)?__m_botch((s),(s2),(univptr_t)(ep),1,__FILE__,__LINE__):0))

#else
#define ASSERT(p, s)
#define ASSERT_SP(p, s, s2, sp)
#define ASSERT_EP(p, s, s2, ep)
#endif

extern int __m_botch proto((const char *, const char *, univptr_t, int, \
			    const char *, int));

#endif /* __ASSERT_H__ */ /* Do not add anything after this line */
