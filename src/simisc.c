/*
 * Name:        simisc.c
 * Description: Misc functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0703231856E0807260414L00296
 * License:     GPLv2.
 */

#include <ctype.h>  /* Using function tolower. */
#include <stdint.h> /* Using type uint32_t, uint64_t. */
#include <string.h> /* Using function strcmp. */
#include <stdio.h>  /* Using function printf. */
#include "svimrdb.h"

int _siCBFCompareNodeDataSPlusA(void * pitem, size_t param);
int _siCBFCompareNodeDataSPlusW(void * pitem, size_t param);
P_NODE_S _siSearchLinkedListSCPlusA(LIST_S list, const void * pitem);
P_NODE_S _siSearchLinkedListSCPlusW(LIST_S list, const void * pitem);

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
	return hshCBFHashString(*(char **)pkey);
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

/* Function name: siCBFMatchChar
 * Description:   Match chars.
 * Parameters:
 *         px Pointer to an element.
 *         py Pointer to another element.
 * Return value:  CBF_SET_EQUAL if pointers matched. true if pointers mismatched.
 */
int siCBFMatchChar(const void * px, const void * py)
{
	return (int)BOOLIZE(*(char *)px - *(char *)py);
}

/* Function name: siCBFMatchShort
 * Description:   Match short integers.
 * Parameters:
 *         px Pointer to an element.
 *         py Pointer to another element.
 * Return value:  CBF_SET_EQUAL if pointers matched. true if pointers mismatched.
 */
int siCBFMatchShort(const void * px, const void * py)
{
	return (int)BOOLIZE(*(short *)px - *(short *)py);
}

/* Function name: siCBFMatchInt
 * Description:   Match integers.
 * Parameters:
 *         px Pointer to an element.
 *         py Pointer to another element.
 * Return value:  CBF_SET_EQUAL if pointers matched. true if pointers mismatched.
 */
int siCBFMatchInt(const void * px, const void * py)
{
	return (int)BOOLIZE(*(int *)px - *(int *)py);
}

/* Function name: siCBFMatchLong
 * Description:   Match long integers.
 * Parameters:
 *         px Pointer to an element.
 *         py Pointer to another element.
 * Return value:  CBF_SET_EQUAL if pointers matched. true if pointers mismatched.
 */
int siCBFMatchLong(const void * px, const void * py)
{
	return (int)BOOLIZE(*(long *)px - *(long *)py);
}

/* Function name: siCBFMatchFloat
 * Description:   Match float numbers.
 * Parameters:
 *         px Pointer to an element.
 *         py Pointer to another element.
 * Return value:  CBF_SET_EQUAL if pointers matched. true if pointers mismatched.
 */
int siCBFMatchFloat(const void * px, const void * py)
{
	return (int)BOOLIZE(*(uint32_t *)px - *(uint32_t *)py);
}

/* Function name: siCBFMatchDouble
 * Description:   Match double floats.
 * Parameters:
 *         px Pointer to an element.
 *         py Pointer to another element.
 * Return value:  CBF_SET_EQUAL if pointers matched. true if pointers mismatched.
 */
int siCBFMatchDouble(const void * px, const void * py)
{
	return (int)BOOLIZE(*(uint64_t *)px - *(uint64_t *)py);
}

/* Function name: siCBFMatchString
 * Description:   Match strings.
 * Parameters:
 *         px Pointer to an element.
 *         py Pointer to another element.
 * Return value:  CBF_SET_EQUAL if pointers matched. true if pointers mismatched.
 */
int siCBFMatchString(const void * px, const void * py)
{
	return strcmp(*(char **)px, *(char **)py);
}

/* Function name: siCBFMatchWString
 * Description:   Match wide strings.
 * Parameters:
 *         px Pointer to an element.
 *         py Pointer to another element.
 * Return value:  CBF_SET_EQUAL if pointers matched. true if pointers mismatched.
 */
int siCBFMatchWString(const void * px, const void * py)
{
	return wcscmp(*(wchar_t **)px, *(wchar_t **)py);
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

/* Function name: siPrintSystemVersion
 * Description:   Print system version.
 * Parameter:     N/A.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siPrintSystemVersion(void)
{
	DISUSE(printf("%s", SYS_VER));
}

/* Function name: sicbftaDefaultIncrease
 * Description:   Default table increasing function.
 * Parameters:
 *      curln Input the current table length value.
 *         ln Input total table length value.
 * Return value:  New table length value.
 */
size_t sicbftaDefaultIncrease(size_t curln, size_t ln)
{
	if (curln == ln)
		return ln + TBL_LN_BUF_SIZ;
	else
		return ln;
}

/* Function name: sicbftaDefaultDecrease
 * Description:   Default table decreasing function.
 * Parameters:
 *      curln Input the current table length value.
 *         ln Input total table length value.
 * Return value:  New table length value.
 */
size_t sicbftaDefaultDecrease(size_t curln, size_t ln)
{
	if (ln - TBL_LN_BUF_SIZ > curln)
		return ln - TBL_LN_BUF_SIZ;
	else
		return ln;
}

