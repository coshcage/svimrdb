/*
 * Name:        simisc.c
 * Description: Misc functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0703231856E0708231210L00030
 * License:     GPLv2.
 */

#include <ctype.h> /* Using function tolower. */
#include "svimrdb.h"

/* Function name: siStrLCase
 * Description:   Convert a zero terminated string to lower case..
 * Parameter:
 *       str Pointer to the original string.
 * Return value:  Pointer to the original string.
 * Caution:       N/A.
 * Tip:           N/A.
 */
char * siStrLCase(char * str)
{
	char * s = str;
	while (*s)
	{
		*s = tolower(*s);
		++s;
	}
	return str;
}
