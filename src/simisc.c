/*
 * Name:        simisc.c
 * Description: Misc functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0703231856E0418240132L00330
 * License:     GPLv2.
 */

#include <ctype.h>  /* Using function tolower. */
#include <stdint.h> /* Using type uint32_t, uint64_t. */
#include <string.h> /* Using function strcmp. */
#include <stdio.h>  /* Using function printf. */
#include "svimrdb.h"

int _siCBFCompareNodeDataSPlusA(void * pitem, size_t param);
int _siCBFCompareNodeDataSPlusW(void * pitem, size_t param);
P_NODE_S _siSearchLinkedListSCPlusA(LIST_S list, const void * pitem, size_t size);
P_NODE_S _siSearchLinkedListSCPlusW(LIST_S list, const void * pitem, size_t size);

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

/* Function name: siPrintSystemVersion
 * Description:   Print system version.
 * Parameter:     N/A.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siPrintSystemVersion(void)
{
	(void)printf("%s", SYS_VER);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _siCBFCompareNodeDataSPlusA
 * Description:   Used to compare data of NODE_S for strings.
 * Parameters:
 *      pitem Pointer to a NODE_S.
 *      param Pointer to FindingInfo.
 * Return value:  If data matched, function would return value CBF_TERMINATE,
 *                otherwise function would return value CBF_CONTINUE.
 */
int _siCBFCompareNodeDataSPlusA(void * pitem, size_t param)
{
	/* The type of param is P_FindingInfo. */
	if 
	(
		0 == strcmp
		(
			*(char **)(((P_NODE_S)pitem)->pdata),
			*(char **)(((P_FindingInfo)param)->pitem)
		)
	)
	{
		((P_FindingInfo)param)->result = pitem;
		return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _siSearchLinkedListSCPlusA
 * Description:   Find pitem in a single linked-list list.
 * Parameters:
 *       list Pointer to the first NODE_S element you want to search in the linked-list.
 *      pitem Pointer to the item you want to search.
 *       size Size of data of pitem.
 * Return value:  Pointer to the NODE_S which contains the same data as pitem.
 * Caution:       Data in each node of linked-list must be in the same size.
 * Tip:           No dead cycles for circular linked-lists.
 */
P_NODE_S _siSearchLinkedListSCPlusA(LIST_S list, const void * pitem, size_t size)
{
	FindingInfo fi;
	fi.result = NULL;
	fi.pitem = pitem;
	fi.size = size;
	strTraverseLinkedListSC_X(list, NULL, _siCBFCompareNodeDataSPlusA, (size_t)&fi);
	return (P_NODE_S)fi.result;
}

/* Function name: hshSearchCPlusA
 * Description:   Search an element in a separate chaining hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *     cbfhsh Pointer to hash function.
 *            The same hash table should use the same hash function.
 *       pkey Pointer to an element. Casted into (const void *).
 *      param size Size of data of pkey.
 * Return value:  Pointer to a NODE_S that contains key value.
 * Caution:       Parameter pht Must Be Allocated first.
 */
P_NODE_S hshSearchCPlusA(P_HSHTBL_C pht, CBF_HASH cbfhsh, const void * pkey, size_t size)
{
	return _siSearchLinkedListSCPlusA(*(P_NODE_S *)(pht->pdata + (cbfhsh(pkey) % strLevelArrayZ(pht)) * sizeof(P_NODE_S)), pkey, size);
}


/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _siCBFCompareNodeDataSPlusA
 * Description:   Used to compare data of NODE_S for strings.
 * Parameters:
 *      pitem Pointer to a NODE_S.
 *      param Pointer to FindingInfo.
 * Return value:  If data matched, function would return value CBF_TERMINATE,
 *                otherwise function would return value CBF_CONTINUE.
 */
int _siCBFCompareNodeDataSPlusW(void * pitem, size_t param)
{
	/* The type of param is P_FindingInfo. */
	if
	(
		0 == wcscmp
		(
			*(wchar_t **)(((P_NODE_S)pitem)->pdata),
			*(wchar_t **)(((P_FindingInfo)param)->pitem)
		)
	)
	{
		((P_FindingInfo)param)->result = pitem;
		return CBF_TERMINATE;
	}
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _siSearchLinkedListSCPlusA
 * Description:   Find pitem in a single linked-list list.
 * Parameters:
 *       list Pointer to the first NODE_S element you want to search in the linked-list.
 *      pitem Pointer to the item you want to search.
 *       size Size of data of pitem.
 * Return value:  Pointer to the NODE_S which contains the same data as pitem.
 * Caution:       Data in each node of linked-list must be in the same size.
 * Tip:           No dead cycles for circular linked-lists.
 */
P_NODE_S _siSearchLinkedListSCPlusW(LIST_S list, const void * pitem, size_t size)
{
	FindingInfo fi;
	fi.result = NULL;
	fi.pitem = pitem;
	fi.size = size;
	strTraverseLinkedListSC_X(list, NULL, _siCBFCompareNodeDataSPlusW, (size_t)&fi);
	return (P_NODE_S)fi.result;
}

/* Function name: hshSearchCPlusA
 * Description:   Search an element in a separate chaining hash table.
 * Parameters:
 *        pht Pointer to the hash table you want to operate.
 *     cbfhsh Pointer to hash function.
 *            The same hash table should use the same hash function.
 *       pkey Pointer to an element. Casted into (const void *).
 *      param size Size of data of pkey.
 * Return value:  Pointer to a NODE_S that contains key value.
 * Caution:       Parameter pht Must Be Allocated first.
 */
P_NODE_S hshSearchCPlusW(P_HSHTBL_C pht, CBF_HASH cbfhsh, const void * pkey, size_t size)
{
	return _siSearchLinkedListSCPlusW(*(P_NODE_S *)(pht->pdata + (cbfhsh(pkey) % strLevelArrayZ(pht)) * sizeof(P_NODE_S)), pkey, size);
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
