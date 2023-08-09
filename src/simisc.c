/*
 * Name:        simisc.c
 * Description: Misc functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0703231856E0809230924L00152
 * License:     GPLv2.
 */

#include <ctype.h>  /* Using function tolower. */
#include <stdint.h> /* Using type uint32_t, uint64_t. */
#include "svimrdb.h"

/* Function name: siStrLCase
 * Description:   Convert a zero terminated string to lower case.
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

/* Function name: siHashChar
 * Description:   Hash character.
 * Parameter:
 *      pkey Pointer to a character.
 * Return value:  Hash value.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siHashChar(const void * pkey)
{
	return (size_t) * (char *)pkey;
}

/* Function name: siHashChar
 * Description:   Hash character.
 * Parameter:
 *      pkey Pointer to a character.
 * Return value:  Hash value.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siHashShort(const void * pkey)
{
	return (size_t) * (short *)pkey;
}

/* Function name: siHashInt
 * Description:   Hash integer.
 * Parameter:
 *      pkey Pointer to an integer.
 * Return value:  Hash value.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siHashInt(const void * pkey)
{
	return (size_t) * (int *)pkey;
}

/* Function name: siHashLong
 * Description:   Hash long.
 * Parameter:
 *      pkey Pointer to a long integer.
 * Return value:  Hash value.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siHashLong(const void * pkey)
{
	return (size_t) * (long *)pkey;
}

/* Function name: siHashFloat
 * Description:   Hash float.
 * Parameter:
 *      pkey Pointer to a float value.
 * Return value:  Hash value.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siHashFloat(const void * pkey)
{
	return (size_t) * (uint32_t *)pkey;
}

/* Function name: siHashDouble
 * Description:   Hash double.
 * Parameter:
 *      pkey Pointer to a double value.
 * Return value:  Hash value.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siHashDouble(const void * pkey)
{
	return (size_t) * (uint64_t *)pkey;
}

/* Function name: siHashString
 * Description:   Hash string.
 * Parameter:
 *      pkey Pointer to a string.
 * Return value:  Hash value.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siHashString(const void * pkey)
{
	char * pstr = *(char **)pkey;
	return hshCBFHashString(pstr);
}

/* Function name: siHashWString
 * Description:   Hash wide string.
 * Parameter:
 *      pkey Pointer to a wide string.
 * Return value:  Hash value.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siHashWString(const void * pkey)
{
	wchar_t * pstr = *(wchar_t **)pkey;
	size_t hrtn = 0;
	while ('\0' != *pstr)
		hrtn = (hrtn << 13) + *pstr++;
	return hrtn;
}

/* Function name: siPlatformSize
 * Description:   Return sizeof size_t.
 * Parameter:     N/A.
 * Return value:  Size of size_t.
 * Caution:       N/A.
 * Tip:           N/A.
 */
size_t siPlatformSize(void)
{
	return sizeof(size_t);
}
