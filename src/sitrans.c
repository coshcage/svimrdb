/*
 * Name:        sitrans.c
 * Description: Transaction control.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0702231427D0000000000L00000
 * License:     GPLv2.
 */

#include <stdlib.h> /* Using function malloc, free. */
#include "svimrdb.h"


P_SET_T psetTrans = NULL;

extern int _grpCBFCompareInteger(const void * px, const void * py);

void EnqueueAlt()
{

}

P_TRANS siBeginTransaction()
{
	P_TRANS pt = (P_TRANS) malloc(sizeof(TRANS));
	if (NULL == pt)
		return NULL;
	queInitDL(&pt->qoprlst);
	setInitT(&pt->setlock);
	if (NULL == psetTrans)
		psetTrans = setCreateT();
	setInsertT(psetTrans, &pt, sizeof(P_TRANS), _grpCBFCompareInteger);
	return pt;
	
}

void siCommitTransaction()
{

}

void siRollbackTransaction()
{

}
