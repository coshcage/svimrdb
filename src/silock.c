/*
 * Name:        silock.c
 * Description: Lock functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0704231516F0708231202L00145
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

extern P_SET_T psetTrans; /* Transactions. */

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _sicmfcmpLock
 * Description:   This function is used to compare locks.
 * Parameters:
 *         px Pointer to a LOCK structure.
 *         py Pointer to another LOCK structure.
 * Return value:  > 0, < 0 and ==0 (Represents equal).
 * Caution:   N/A.
 */
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

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _sicbftvsFindLock
 * Description:   This function is used to find lock.
 * Parameters:
 *      pitem Pointer to each P_BSTNODE in a BST.
 *      param Pointer to a size_t[4] array structure.
 * Return value:  CBF_CONTINUE or CBF_TERMINATE.
 */
static int _sicbftvsFindLock(void * pitem, size_t param)
{
	P_LOCK pl = (P_LOCK)(P2P_BSTNODE(pitem)->knot.pdata);
	if ((size_t)pl->pobj == 0[(size_t *)param])
		if (N == bCompatibleMatrix[pl->lt][1[(size_t *)param]])
			return CBF_TERMINATE;
	return CBF_CONTINUE;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _sicbftvsTrans
 * Description:   This function is used traverse transactions.
 * Parameters:
 *      pitem Pointer to each P_BSTNODE in a BST.
 *      param Pointer to a size_t[4] array structure.
 * Return value:  CBF_CONTINUE or CBF_TERMINATE.
 */
static int _sicbftvsTrans(void * pitem, size_t param)
{
	P_TRANS pt = *(P_TRANS *)(P2P_BSTNODE(pitem)->knot.pdata);
	if ((size_t)pt == 2[(size_t *)param])
	{
		3[(size_t *)param] = FALSE;
		return CBF_CONTINUE;
	}
	if (!setIsEmptyT(&pt->setlock))
	{
		int r;
		r = setTraverseT(&pt->setlock, _sicbftvsFindLock, param, ETM_LEVELORDER);
		if (CBF_TERMINATE == r)
			3[(size_t *)param] = TRUE;
		return r;
	}
	3[(size_t *)param] = FALSE;
	return CBF_CONTINUE;
}

/* Function name: siTrylock
 * Description:   Test a lock.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      pobj Pointer to an object.
 *        lt Lock type.
 * Return value:  TRUE lock accuired. FALSE lock refused.
 * Caution:       N/A.
 * Tip:           N/A.
 */
BOOL siTrylock(P_TRANS ptrans, void * pobj, LockType lt)
{
	size_t a[4];
	LOCK l;
	l.pobj = pobj;
	l.lt = lt;
	a[0] = (size_t)pobj;
	a[1] = lt;
	a[2] = (size_t)ptrans;
	a[3] = FALSE;

	if (NULL != psetTrans)
	{
		setTraverseT(psetTrans, _sicbftvsTrans, (size_t)a, ETM_LEVELORDER);
		if (FALSE != a[3])
			return FALSE;
	}

	setInsertT(&ptrans->setlock, &l, sizeof(LOCK), _sicmfcmpLock);
	return TRUE;
}

/* Function name: siUnlock
 * Description:   Release a lock.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      pobj Pointer to an object.
 *        lt Lock type.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siUnlock(P_TRANS ptrans, void * pobj, LockType lt)
{
	LOCK l;
	l.pobj = pobj;
	l.lt = lt;
	setRemoveT(&ptrans->setlock, &l, sizeof(LOCK), _sicmfcmpLock);
}
