/*
 * Name:        simisc.c
 * Description: Misc functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0703231856E0000000000L00027
 * License:     GPLv2.
 */

#include <ctype.h> /* Using function tolower. */
#include "svimrdb.h"

/* Function name: strlowercase
 * Description:   Convert a zero terminated string to lower case..
 * Parameter:
 *       str Pointer to the original string.
 * Return value:  Pointer to the original string.
 * Caution:       N/A.
 * Tip:           N/A.
 */
char * strlowercase(char * str)
{
	char * s = str;
	while (*str++)
		*str = tolower(*str);
	return s;
}
