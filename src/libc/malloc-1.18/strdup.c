/*  Author: Mark Moraes <moraes@csri.toronto.edu> */

/*LINTLIBRARY*/

#include "defs.h"

RCSID("$Id: strdup.c,v 1.1 2001/11/05 18:31:45 pavlovskii Exp $")

/* 
 *  makes a copy of a null terminated string in malloc'ed storage.
 *  returns null if it fails.
 */
char *
strdup(s)
const char *s;
{
	char *cp;

	if (s) {
		cp = (char *) malloc((unsigned) (strlen(s)+1));
		if (cp)
			(void) strcpy(cp, s);
	} else
		cp = (char *) NULL;
	return(cp);
}
