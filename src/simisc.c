/*
 * Name:        simisc.c
 * Description: Misc functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0703231856E0708231210L00030
 * License:     GPLv2.
 */

#include <ctype.h> /* Using function tolower. */
#include <stdint.h>
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

size_t siHashChar(const void * pkey)
{
	return (size_t) * (char *)pkey;
}

size_t siHashShort(const void * pkey)
{
	return (size_t) * (short *)pkey;
}

size_t siHashInt(const void * pkey)
{
	return (size_t) * (int *)pkey;
}

size_t siHashLong(const void * pkey)
{
	return (size_t) * (long *)pkey;
}

size_t siHashFloat(const void * pkey)
{
	return (size_t) * (uint32_t *)pkey;
}

size_t siHashDouble(const void * pkey)
{
	return (size_t) * (uint64_t *)pkey;
}
