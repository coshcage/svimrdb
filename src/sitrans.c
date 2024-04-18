/*
 * Name:        sitrans.c
 * Description: Transaction control.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0702231427D0418240134L00367
 * License:     GPLv2.
 */

#include <stdio.h>  /* Using macro BUFSIZ. */
#include <string.h> /* Using function memcpy, memmove. */
#include <stdlib.h> /* Using function malloc, free. */
#include "svimrdb.h"

/* A set of transactions. */
P_SET_T psetTrans = NULL;
pthread_mutex_t mtxPST = PTHREAD_MUTEX_INITIALIZER;

/* Compare function declaration. */
extern int _grpCBFCompareInteger(const void * px, const void * py);

/* Function name: siBeginTransaction
 * Description:   Starts a transaction.
 * Parameter:     N/A.
 * Return value:  Pointer to a new transaction.
 * Caution:       N/A.
 * Tip:           N/A.
 */
P_TRANS siBeginTransaction()
{
	P_TRANS pt = (P_TRANS) malloc(sizeof(TRANS));
	if (NULL == pt)
		return NULL;
	queInitDL(&pt->qoprlst);
	setInitT(&pt->setlock);

	pthread_mutex_lock(&mtxPST);

	if (NULL == psetTrans)
		psetTrans = setCreateT();
	setInsertT(psetTrans, &pt, sizeof(P_TRANS), _grpCBFCompareInteger);

	pthread_mutex_unlock(&mtxPST);

	return pt;
}

/* Function name: siCommitTransaction
 * Description:   Commit transaction.
 * Parameter:
 *    ptrans Pointer to a transaction.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siCommitTransaction(P_TRANS ptrans)
{
	if (NULL == ptrans)
		return;
	while (!queIsEmptyDL(&ptrans->qoprlst))
	{
		DATALT da;
		size_t i;
		queEjectDL(&da, sizeof(DATALT), &ptrans->qoprlst);

		switch (da.at)
		{
		case AT_ALTER_CELL:
			siDeleteCell(&da.data.dacell.before);
			break;
		case AT_ADD_TUPLE:

			break;
		case AT_DEL_TUPLE:
			for (i = 0; i < strLevelArrayZ(&da.data.datpl.tupledat); ++i)
				siDeleteCell((P_CELL *)strLocateItemArrayZ(&da.data.datpl.tupledat, sizeof(P_CELL), i));
			strFreeArrayZ(&da.data.datpl.tupledat);
			break;
		case AT_ADD_COLUMN:

			break;
		case AT_DEL_COLUMN:
			for (i = 0; i < strLevelArrayZ(&da.data.dacol.coldat); ++i)
				siDeleteCell((P_CELL *)strLocateItemArrayZ(&da.data.dacol.coldat, sizeof(P_CELL), i));
			free(da.data.dacol.hdr.strname);
			strFreeArrayZ(&da.data.dacol.coldat);
			break;
		case AT_ADD_TABLE:

			break;
		case AT_DEL_TABLE:
			siDeleteTable(NULL, da.data.datbl);
			break;
		}
	}
	pthread_mutex_lock(&mtxPST);

	setRemoveT(psetTrans, &ptrans, sizeof(P_TRANS), _grpCBFCompareInteger);

	pthread_mutex_unlock(&mtxPST);
	setFreeT(&ptrans->setlock);
	free(ptrans);
}

static int _cbftvsAlterTargetTable(void * pitem, size_t param)
{
	if (((P_DATALT)((P_NODE_D)pitem)->pdata)->ptbl == (P_TABLE)0[(size_t *)param])
		((P_DATALT)((P_NODE_D)pitem)->pdata)->ptbl = (P_TABLE)01[(size_t *)param];
	return CBF_CONTINUE;
}

/* Function name: siRollbackTransaction
 * Description:   Rollback transaction.
 * Parameter:
 *     pparr Pointer to an array that used to contain altered table pointers.
 *           Each element of this array is a TBLREF structure.
 *           Input NULL to dismiss this array.
 *    ptrans Pointer to a transaction.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siRollbackTransaction(P_ARRAY_Z * pparr, P_TRANS ptrans)
{
	size_t a[2];
	size_t m = 0;
	P_TABLE ptr = NULL;
	if (NULL == ptrans)
		return;
	while (! queIsEmptyDL(&ptrans->qoprlst))
	{
		size_t i;
		DATALT da;
		P_CELL * ppc;
		P_TBLHDR pt;
		queEjectDL(&da, sizeof(DATALT), &ptrans->qoprlst);

		switch (da.at)
		{
		case AT_ALTER_CELL:
			
			siUpdateTableCell(NULL, da.ptbl, da.data.dacell.before->pdata, da.data.dacell.ln, da.data.dacell.col);
			
			siDeleteCell(&da.data.dacell.before);
			break;
		case AT_ADD_TUPLE:
			siDeleteFromTable(NULL, da.ptbl, NULL, da.data.datpl.sizln);
			break;
		case AT_DEL_TUPLE:
			i = sicbftaDefaultIncrease(da.ptbl->curln + 1, da.ptbl->tbldata.ln);

			if (i != da.ptbl->tbldata.ln)
				strResizeMatrix(&da.ptbl->tbldata, i, da.ptbl->tbldata.col, sizeof(P_CELL));

			ppc = (P_CELL *)malloc(sizeof(P_CELL) * da.ptbl->tbldata.col);
			if (ppc)
			{
				for (i = 0; i < strLevelArrayZ(&da.data.datpl.tupledat); ++i)
				{
					P_CELL pc;
					pc = *(P_CELL *)strLocateItemArrayZ(&da.data.datpl.tupledat, sizeof(P_CELL), i);

					pt = strLocateItemArrayZ(&da.ptbl->header, sizeof(TBLHDR), i);

					if (NULL == pc)
						i[ppc] = NULL;
					else
						i[ppc] = siCreateCell(pc->pdata, pc->ct);

					if (NULL != pt->phsh)
					{
						switch (pt->cr)
						{
						case CR_UNIQUE:
						case CR_PRIMARY_KEY:
							switch (pt->ct)
							{
							case CT_CHAR:
								hshInsertC(pt->phsh, siHashChar, pc->pdata, sizeof(char));
								break;
							case CT_SHORT:
								hshInsertC(pt->phsh, siHashShort, pc->pdata, sizeof(short));
								break;
							case CT_INTEGER:
								hshInsertC(pt->phsh, siHashInt, pc->pdata, sizeof(int));
								break;
							case CT_LONG:
								hshInsertC(pt->phsh, siHashLong, pc->pdata, sizeof(long));
								break;
							case CT_FLOAT:
								hshInsertC(pt->phsh, siHashFloat, pc->pdata, sizeof(float));
								break;
							case CT_DOUBLE:
								hshInsertC(pt->phsh, siHashDouble, pc->pdata, sizeof(double));
								break;
							case CT_STRING:
								hshInsertC(pt->phsh, siHashString, &pc->pdata, sizeof(char *));
								break;
							case CT_WSTRING:
								hshInsertC(pt->phsh, siHashWString, &pc->pdata, sizeof(wchar_t *));
								break;
							}
							break;
						}
					}
				}

				++da.ptbl->curln;

				memmove
				(
					&da.ptbl->tbldata.arrz.pdata[(da.data.datpl.sizln + 1) * sizeof(P_CELL) * da.ptbl->tbldata.col],
					&da.ptbl->tbldata.arrz.pdata[da.data.datpl.sizln * sizeof(P_CELL) * da.ptbl->tbldata.col],
					(da.ptbl->curln - 1 - da.data.datpl.sizln) * sizeof(P_CELL) * da.ptbl->tbldata.col
				);

				memcpy(&da.ptbl->tbldata.arrz.pdata[da.data.datpl.sizln * sizeof(P_CELL) * da.ptbl->tbldata.col], ppc, sizeof(P_CELL) * da.ptbl->tbldata.col);
				free(ppc);

			}

			for (i = 0; i < strLevelArrayZ(&da.data.datpl.tupledat); ++i)
				siDeleteCell((P_CELL *)strLocateItemArrayZ(&da.data.datpl.tupledat, sizeof(P_CELL), i));

			strFreeArrayZ(&da.data.datpl.tupledat);
			break;
		case AT_ADD_COLUMN:
			siDropTableColumn(NULL, da.ptbl, da.data.dacol.sizcol);
			break;
		case AT_DEL_COLUMN:
			/* Add table header first. */
			strResizeArrayZ(&da.ptbl->header, strLevelArrayZ(&da.ptbl->header) + 1, sizeof(TBLHDR));
			strInsertItemArrayZ(&da.ptbl->header, &da.data.dacol.hdr, sizeof(TBLHDR), da.data.dacol.sizcol);
			pt = strLocateItemArrayZ(&da.ptbl->header, sizeof(TBLHDR), da.data.dacol.sizcol);
			pt->strname = strdup(da.data.dacol.hdr.strname);

			switch (da.data.dacol.hdr.cr)
			{
			case CR_UNIQUE:
			case CR_PRIMARY_KEY:
				pt->phsh = hshCreateC(BKSNUM);
				break;
			}
			/* Alter table next. */
			if (NULL != strResizeMatrix(&da.ptbl->tbldata, da.ptbl->tbldata.ln, da.ptbl->tbldata.col + 1, sizeof(P_CELL)))
			{
				size_t i;
				for (i = 0; i < da.ptbl->curln; ++i)
				{
					P_CELL pc;
					memmove
					(
						(P_CELL *)strGetValueMatrix(NULL, &da.ptbl->tbldata, i, da.data.dacol.sizcol + 1, sizeof(P_CELL)),
						(P_CELL *)strGetValueMatrix(NULL, &da.ptbl->tbldata, i, da.data.dacol.sizcol, sizeof(P_CELL)),
						sizeof(P_CELL) * (da.ptbl->tbldata.col - 1 - da.data.dacol.sizcol)
					);

					pc = *(P_CELL *)strLocateItemArrayZ(&da.data.dacol.coldat, sizeof(P_CELL), i);
					if (NULL != pc)
						pc = siCreateCell(pc->pdata, pc->ct);
					strSetValueMatrix(&da.ptbl->tbldata, i, da.data.dacol.sizcol, &pc, sizeof(P_CELL));

					if (NULL != pt->phsh)
					{
						switch (pt->cr)
						{
						case CR_UNIQUE:
						case CR_PRIMARY_KEY:
							switch (pt->ct)
							{
							case CT_CHAR:
								hshInsertC(pt->phsh, siHashChar, pc->pdata, sizeof(char));
								break;
							case CT_SHORT:
								hshInsertC(pt->phsh, siHashShort, pc->pdata, sizeof(short));
								break;
							case CT_INTEGER:
								hshInsertC(pt->phsh, siHashInt, pc->pdata, sizeof(int));
								break;
							case CT_LONG:
								hshInsertC(pt->phsh, siHashLong, pc->pdata, sizeof(long));
								break;
							case CT_FLOAT:
								hshInsertC(pt->phsh, siHashFloat, pc->pdata, sizeof(float));
								break;
							case CT_DOUBLE:
								hshInsertC(pt->phsh, siHashDouble, pc->pdata, sizeof(double));
								break;
							case CT_STRING:
								hshInsertC(pt->phsh, siHashString, &pc->pdata, sizeof(char *));
								break;
							case CT_WSTRING:
								hshInsertC(pt->phsh, siHashWString, &pc->pdata, sizeof(wchar_t *));
								break;
							}
							break;
						}
					}
				}
			}
			
			for (i = 0; i < strLevelArrayZ(&da.data.dacol.coldat); ++i)
				siDeleteCell((P_CELL *)strLocateItemArrayZ(&da.data.dacol.coldat, sizeof(P_CELL), i));

			free(da.data.dacol.hdr.strname);
			strFreeArrayZ(&da.data.dacol.coldat);
			break;
		case AT_ADD_TABLE:
			siDeleteTable(NULL, da.ptbl);
			break;
		case AT_DEL_TABLE:
			ptr = siCopyTable(NULL, da.data.datbl);
			a[0] = (size_t)da.ptbl;
			a[1] = (size_t)ptr;

			strTraverseLinkedListDC_X(ptrans->qoprlst.pfirst, NULL, _cbftvsAlterTargetTable, (size_t)a, FALSE);

			if (NULL != pparr)
			{
				if (NULL == *pparr)
					*pparr = strCreateArrayZ(BUFSIZ, sizeof(TBLREF));
				(*(P_TBLREF)strLocateItemArrayZ(*pparr, sizeof(P_TABLE), m)).pold = da.ptbl;
				(*(P_TBLREF)strLocateItemArrayZ(*pparr, sizeof(P_TABLE), m)).pnew = ptr;
				++m;
				if (m >= strLevelArrayZ(*pparr))
				{
					if (NULL == strResizeArrayZ(*pparr, strLevelArrayZ(*pparr) + BUFSIZ, sizeof(TBLREF)))
					{
						m = 0;
						break;
					}
				}
			}

			siDeleteTable(NULL, da.data.datbl);
			break;
		}
	}
	if (NULL != pparr && 0 != m)
		strResizeArrayZ(*pparr, m, sizeof(TBLREF));

	pthread_mutex_lock(&mtxPST);

	setRemoveT(psetTrans, &ptrans, sizeof(P_TRANS), _grpCBFCompareInteger);

	pthread_mutex_unlock(&mtxPST);

	setFreeT(&ptrans->setlock);
	free(ptrans);
}

/* Function name: siReleaseAllTransaction
 * Description:   Release all transaction.
 * Parameter:     N/A.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           Call this function before program terminated to release memory.
 */
void siReleaseAllTransaction()
{
	pthread_mutex_lock(&mtxPST);

	setDeleteT(psetTrans);
	psetTrans = NULL;

	pthread_mutex_unlock(&mtxPST);
}
