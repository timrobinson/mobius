/* $Id: strrchr.c,v 1.2 2001/11/06 01:29:38 pavlovskii Exp $ */

/* Copyright (C) 1996 DJ Delorie, see COPYING.DJ for details */
/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <string.h>

char *
strrchr(const char *s, int c)
{
  char cc = c;
  const char *sp=(char *)0;
  while (*s)
  {
    if (*s == cc)
      sp = s;
    s++;
  }
  if (cc == 0)
    sp = s;
  return (char*) sp;
}

