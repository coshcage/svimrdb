/*
 * Name:        silock.c
 * Description: Lock functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0704231516F0000000000L00000
 * License:     GPLv2.
 */

#include "svimrdb.h"

#define Y TRUE
#define N FALSE

static BOOL const bCompatibleMatrix[][6] =
{
	{Y,N,Y,N,N,Y},
	{N,N,N,N,N,Y},
	{Y,N,Y,Y,Y,Y},
	{N,N,Y,Y,N,Y},
	{N,N,Y,N,N,Y},
	{Y,Y,Y,Y,Y,Y}
};

extern P_SET_T psetTrans;

static int _sicmfcmpLock(const void * px, const void * py)
{
	P_LOCK plx, ply;
	plx = (P_LOCK)px;
	ply = (P_LOCK)py;
	if (plx->pobj > ply->pobj)
		return 1;
	else if (plx->pobj < ply->pobj)
		return -1;
	if (plx->lt > ply->lt)
		return 1;
	else if (plx->lt < ply->lt)
		return -1;
	return 0;
}

static int _sicbftvsFindLock(void * pitem, size_t param)
{
	P_LOCK pl = (P_LOCK)(P2P_BSTNODE(pitem)->knot.pdata);
	if ((size_t)pl->pobj == 0[(size_t *)param])
		if (N == bCompatibleMatrix[pl->lt][1[(size_t *)param]])
			return CBF_TERMINATE;
	return CBF_CONTINUE;
}

static int _sicbftvsTrans(void * pitem, size_t param)
{
	P_TRANS pt = *(P_TRANS *)(P2P_BSTNODE(pitem)->knot.pdata);
	if ((size_t)pt == 2[(size_t *)param])
		return CBF_CONTINUE;
	if (! setIsEmptyT(&pt->setlock))
		return setTraverseT(&pt->setlock, _sicbftvsFindLock, param, ETM_LEVELORDER);
	return CBF_CONTINUE;
}

BOOL siTrylock(P_TRANS ptrans, void * pobj, LockType lt)
{
	size_t a[3];
	LOCK l;
	l.pobj = pobj;
	l.lt = lt;
	a[0] = (size_t)pobj;
	a[1] = lt;
	a[2] = (size_t)ptrans;

	if (NULL != psetTrans)
		if (CBF_TERMINATE == setTraverseT(psetTrans, _sicbftvsTrans, (size_t)a, ETM_LEVELORDER))
			return FALSE;

	setInsertT(&ptrans->setlock, &l, sizeof(LOCK), _sicmfcmpLock);
	return TRUE;
}

void siUnlock(P_TRANS ptrans, void * pobj, LockType lt)
{
	LOCK l;
	l.pobj = pobj;
	l.lt = lt;
	setRemoveT(&ptrans->setlock, &l, sizeof(LOCK), _sicmfcmpLock);
}
