/*
 * Name:        svimrdb.h
 * Description: SI core functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0628231947B0809230919L00742
 * License:     GPLv2.
 */
#include <stdio.h>  /* Using macro BUFSIZ. */
#include <stdlib.h> /* Using function malloc, free, realloc. */
#include <string.h> /* Using function memcpy. */
#include <math.h>   /* Using function roundf, round. */
#include <wchar.h>  /* Using function wcscmp, wcslen. */
#include "svimrdb.h"

/* size_t integer copmaration function. */
extern int _grpCBFCompareInteger(const void * px, const void * py);
static int _sicbfcmp(const void * px, const void * py);
static int _sicbftvsMergeView(void * pitem, size_t param);

/* Warning: this following variable reduces the reentrancibility of most relational algebra functions.
 * This variable is used to store the view column number.
 */
static size_t _sizCUTTarget;

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _sicbfcmp
 * Description:   This function is used to compare table contents.
 * Parameters:
 *         px Pointer to pointer of P_CELL.
 *         py Pointer to pointer of P_CELL.
 * Return value:  > 0, < 0 and ==0 (Represents equal).
 */
static int _sicbfcmp(const void * px, const void * py)
{
	int r = 0;
	size_t i;
	P_CELL pcx, pcy;


	for (i = 0; i < _sizCUTTarget; ++i)
	{
		pcx = *((*(P_CELL **)px) + i);
		pcy = *((*(P_CELL **)py) + i);
		if (NULL != pcx && NULL != pcy)
		{
			switch (pcx->ct)
			{
			case CT_CHAR:
				r = *(char *)pcx->pdata - *(char *)pcy->pdata;
				break;
			case CT_SHORT:
				r = *(short *)pcx->pdata - *(short *)pcy->pdata;
				break;
			case CT_INTEGER:
				r = *(int *)pcx->pdata - *(int *)pcy->pdata;
				break;
			case CT_LONG:
				r = *(long *)pcx->pdata - *(long *)pcy->pdata;
				break;
			case CT_FLOAT:
				r = (int)roundf(*(float *)pcx->pdata - *(float *)pcy->pdata);
				break;
			case CT_DOUBLE:
				r = (int)round(*(double *)pcx->pdata - *(double *)pcy->pdata);
				break;
			case CT_STRING:
				r = strcmp((char *)pcx->pdata, (char *)pcy->pdata);
				break;
			case CT_WSTRING:
				r = wcscmp((wchar_t *)pcx->pdata, (wchar_t *)pcy->pdata);
				break;
			}
			if (0 == r)
				continue;
			else
				return r;
		}
		else if (NULL == pcx && NULL != pcy)
			return -1;
		else if (NULL != pcx && NULL == pcy)
			return 1;
	}
	return r;
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _sicbftvsMergeView
 * Description:   This function is used to fill a new view.
 * Parameters:
 *      pitem Pointer to each P_BSTNODE in a BST.
 *      param Pointer to a size_t[2] array structure.
 * Return value:  CBF_CONTINUE only.
 */
static int _sicbftvsMergeView(void * pitem, size_t param)
{
	memcpy
	(
		((P_MATRIX)(0[(size_t *)param]))->arrz.pdata + 1[(size_t *)param]++ * _sizCUTTarget * sizeof(P_CELL),
		*(P_CELL *)(P2P_BSTNODE(pitem)->knot.pdata),
		_sizCUTTarget * sizeof(P_CELL)
	);
	return CBF_CONTINUE;
}

/* Function name: siCreateUniqueView
 * Description:   Create a unique view.
 * Parameter:
 *      pmtx Pointer to the original view.
 * Return value:  Pointer to a new allocated unique view.
 * Caution:       This function is not reentrant.
 * Tip:           N/A.
 */
P_MATRIX siCreateUniqueView(P_MATRIX pmtx)
{
	if (NULL == pmtx)
		return NULL;
	if (pmtx->ln && pmtx->col)
	{
		size_t a[2];
		P_MATRIX mtxr;
		SET_T set;
		size_t i, j;

		setInitT(&set);

		_sizCUTTarget = pmtx->col;

		for (j = i = 0; i < pmtx->ln; ++i)
		{
			P_CELL * ptuple = strGetValueMatrix(NULL, pmtx, i, 0, sizeof(P_CELL));

			if (!setIsMemberT(&set, &ptuple, _sicbfcmp))
			{
				set = treBSTInsertAA(set, &ptuple, sizeof(P_CELL *), _sicbfcmp);
				++j;
			}
		}

		mtxr = strCreateMatrix(j, pmtx->col, sizeof(P_CELL));
		a[0] = (size_t)mtxr;
		a[1] = 0;
		if (NULL != mtxr)
			setTraverseT(&set, _sicbftvsMergeView, (size_t)a, ETM_INORDER);

		setFreeT(&set);

		return mtxr;
	}
	return NULL;
}

/* Function name: siCreateUnionView
 * Description:   Create a union view.
 * Parameter:
 *     pmtxa Pointer to a view.
 *     pmtxb Pointer to another view.
 * Return value:  Pointer to a new allocated union view.
 * Caution:       N/A.
 * Tip:           N/A.
 */
P_MATRIX siCreateUnionView(P_MATRIX pmtxa, P_MATRIX pmtxb)
{
	P_CELL pc = NULL;
	
	P_MATRIX mtxr;
	size_t i, j;
	
	P_MATRIX pma = siCreateUniqueView(pmtxa);
	P_MATRIX pmb = siCreateUniqueView(pmtxb);

	if (NULL == pma && NULL == pmb)
	{
		return NULL;
	}
	else if (NULL != pma && NULL == pmb)
	{
		mtxr = strCreateMatrix(pma->ln, pma->col, sizeof(P_CELL));
		if (NULL != mtxr)
			strCopyArrayZ(&mtxr->arrz, &pma->arrz, sizeof(P_CELL));
	}
	else if (NULL == pma && NULL != pmb)
	{
		mtxr = strCreateMatrix(pmb->ln, pmb->col, sizeof(P_CELL));
		if (NULL != mtxr)
			strCopyArrayZ(&mtxr->arrz, &pmb->arrz, sizeof(P_CELL));
	}
	else
	{
		if (pma->col != pmb->col)
		{
			mtxr = strCreateMatrix(pma->ln + pmb->ln, pma->col > pmb->col ? pma->col : pmb->col, sizeof(P_CELL));

			strSetMatrix(mtxr, &pc, sizeof(P_CELL));

			for (i = 0; i < pma->ln; ++i)
			{
				memcpy
				(
					strGetValueMatrix(NULL, mtxr, i, 0, sizeof(P_CELL)),
					strGetValueMatrix(NULL, pma, i, 0, sizeof(P_CELL)),
					pma->col * sizeof(P_CELL)
				);
			}
			for (j = 0; j < pmb->ln; ++j, ++i)
			{
				memcpy
				(
					strGetValueMatrix(NULL, mtxr, i, 0, sizeof(P_CELL)),
					strGetValueMatrix(NULL, pmb, j, 0, sizeof(P_CELL)),
					pmb->col * sizeof(P_CELL)
				);
			}
		}
		else
		{
			SET_T set;
			size_t a[2];

			setInitT(&set);

			_sizCUTTarget = pma->col;

			for (j = i = 0; i < pma->ln; ++i)
			{
				P_CELL * ptuple = strGetValueMatrix(NULL, pma, i, 0, sizeof(P_CELL));

				if (!setIsMemberT(&set, &ptuple, _sicbfcmp))
				{
					set = treBSTInsertAA(set, &ptuple, sizeof(P_CELL *), _sicbfcmp);
					++j;
				}
			}

			for (i = 0; i < pmb->ln; ++i)
			{
				P_CELL * ptuple = strGetValueMatrix(NULL, pmb, i, 0, sizeof(P_CELL));

				if (!setIsMemberT(&set, &ptuple, _sicbfcmp))
				{
					set = treBSTInsertAA(set, &ptuple, sizeof(P_CELL *), _sicbfcmp);
					++j;
				}
			}

			mtxr = strCreateMatrix(j, pma->col, sizeof(P_CELL));
			a[0] = (size_t)mtxr;
			a[1] = 0;
			if (NULL != mtxr)
				setTraverseT(&set, _sicbftvsMergeView, (size_t)a, ETM_INORDER);

			setFreeT(&set);
		}
	}

	return mtxr;
}

/* Function name: siCreateIntersectionView
 * Description:   Create an intersection view.
 * Parameter:
 *     pmtxa Pointer to a view.
 *     pmtxb Pointer to another view.
 * Return value:  Pointer to a new allocated intersection view.
 * Caution:       N/A.
 * Tip:           N/A.
 */
P_MATRIX siCreateIntersectionView(P_MATRIX pmtxa, P_MATRIX pmtxb)
{
	if (NULL == pmtxa || NULL == pmtxb)
		return NULL;
	else
	{
		if (pmtxa->col != pmtxb->col)
			return NULL;
		else
		{
			P_MATRIX mtxr = NULL;
			size_t i;
			P_MATRIX pma = siCreateUniqueView(pmtxa);
			P_MATRIX pmb = siCreateUniqueView(pmtxb);
			SET_T seta, setb, * psetc;
			P_CELL * ptuple;
			size_t a[2];

			setInitT(&seta);
			setInitT(&setb);

			_sizCUTTarget = pma->col;

			for (i = 0; i < pma->ln; ++i)
			{
				ptuple = strGetValueMatrix(NULL, pma, i, 0, sizeof(P_CELL));
				seta = treBSTInsertAA(seta, &ptuple, sizeof(P_CELL *), _sicbfcmp);
			}

			for (i = 0; i < pma->ln; ++i)
			{
				ptuple = strGetValueMatrix(NULL, pmb, i, 0, sizeof(P_CELL));
				setb = treBSTInsertAA(setb, &ptuple, sizeof(P_CELL *), _sicbfcmp);
			}

			psetc = setCreateIntersectionT(&seta, &setb, sizeof(P_CELL), _sicbfcmp);
			if (NULL != psetc)
			{
				mtxr = strCreateMatrix(setSizeT(psetc), pma->col, sizeof(P_CELL));
				a[0] = (size_t)mtxr;
				a[1] = 0;
				if (NULL != mtxr)
					setTraverseT(psetc, _sicbftvsMergeView, (size_t)a, ETM_INORDER);
			}

			setFreeT(&seta);
			setFreeT(&setb);
			setDeleteT(psetc);

			return mtxr;
		}
	}
	return NULL;
}

/* Function name: siCreateDifferenceView
 * Description:   Create a difference view.
 * Parameter:
 *     pmtxa Pointer to a view.
 *     pmtxb Pointer to another view.
 * Return value:  Pointer to a new allocated difference view.
 * Caution:       N/A.
 * Tip:           N/A.
 */
P_MATRIX siCreateDifferenceView(P_MATRIX pmtxa, P_MATRIX pmtxb)
{
	P_MATRIX mtxr = NULL;
	P_MATRIX pma = siCreateUniqueView(pmtxa);
	P_MATRIX pmb = siCreateUniqueView(pmtxb);

	if (NULL == pmtxa)
		return NULL;
	else if (NULL == pmtxb)
	{
		mtxr = strCreateMatrix(pma->ln, pma->col, sizeof(P_CELL));
		if (NULL != mtxr)
			strCopyArrayZ(&mtxr->arrz, &pma->arrz, sizeof(P_CELL));
	}
	else
	{
		if (pma->col != pmb->col)
		{
			mtxr = strCreateMatrix(pma->ln, pma->col, sizeof(P_CELL));
			if (NULL != mtxr)
				strCopyArrayZ(&mtxr->arrz, &pma->arrz, sizeof(P_CELL));
		}
		else
		{
			size_t i;
			
			SET_T seta, setb, * psetc;
			P_CELL * ptuple;
			size_t a[2];

			setInitT(&seta);
			setInitT(&setb);

			_sizCUTTarget = pma->col;

			for (i = 0; i < pma->ln; ++i)
			{
				ptuple = strGetValueMatrix(NULL, pma, i, 0, sizeof(P_CELL));
				seta = treBSTInsertAA(seta, &ptuple, sizeof(P_CELL *), _sicbfcmp);
			}

			for (i = 0; i < pma->ln; ++i)
			{
				ptuple = strGetValueMatrix(NULL, pmb, i, 0, sizeof(P_CELL));
				setb = treBSTInsertAA(setb, &ptuple, sizeof(P_CELL *), _sicbfcmp);
			}

			psetc = setCreateDifferenceT(&seta, &setb, sizeof(P_CELL), _sicbfcmp);
			if (NULL != psetc)
			{
				mtxr = strCreateMatrix(setSizeT(psetc), pma->col, sizeof(P_CELL));
				a[0] = (size_t)mtxr;
				a[1] = 0;
				if (NULL != mtxr)
					setTraverseT(psetc, _sicbftvsMergeView, (size_t)a, ETM_INORDER);
			}

			setFreeT(&seta);
			setFreeT(&setb);
			setDeleteT(psetc);
		}
	}
	
	return mtxr;
}

/* Function name: siCreateCartesianProductView
 * Description:   Create a cartesian product view.
 * Parameter:
 *     pmtxa Pointer to a view.
 *     pmtxb Pointer to another view.
 * Return value:  Pointer to a new allocated cartesian product view.
 * Caution:       Parameter pma and pmb must be allocated first.
 * Tip:           N/A.
 */
P_MATRIX siCreateCartesianProductView(P_MATRIX pma, P_MATRIX pmb)
{
	size_t m, n;
	P_MATRIX pmr = strCreateMatrix(m = pma->ln * pmb->ln, n = pma->col + pma->col, sizeof(P_CELL));
	if (NULL != pmr)
	{
		REGISTER size_t i, x, y;
		for (i = 0, x = 0, y = 0; i < m; ++i)
		{
			/* Fill tuples of pma to pmr. */
			memcpy
			(
				strGetValueMatrix(NULL, pmr, i, 0, sizeof(P_CELL)),
				strGetValueMatrix(NULL, pma, x, 0, sizeof(P_CELL)),
				sizeof(P_CELL) * pma->col
			);
			/* Fill tuples of pmb to pmr. */
			memcpy
			(
				strGetValueMatrix(NULL, pmr, i, pma->col, sizeof(P_CELL)),
				strGetValueMatrix(NULL, pmb, y, 0, sizeof(P_CELL)),
				sizeof(P_CELL) * pmb->col
			);
			if (0 == (i + 1) % pmb->ln)
				++x;
			if (++y == pmb->ln)
				y = 0;
		}
	}
	return pmr;
}

/* Function name: siCreateSelectView
 * Description:   Create a restriction view.
 * Parameter:
 *     pparr A new created sized array contains each column number of selected tuple.
 *           Input NULL to dismiss this parameter.
 *      pmtx Pointer to a view.
 *    cbfsel Pointer a selection function
 *     param Pointer to an external parameter.
 * Return value:  Pointer to a new allocated view.
 * Caution:       N/A.
 * Tip:
 //BOOL cbftestsel(P_CELL * pitem, size_t param)
 //{
 //	if (strcmp((*pitem)->pdata, "b1") == 0)
 //		return TRUE;
 //	return FALSE;
 //}
 //
 //int main(int argc, char * argv[])
 //{
 //	P_CELL pc;
 //	P_MATRIX pmtx, p;
 //	pmtx = strCreateMatrix(3, 2, sizeof(P_CELL));
 //	pc = siCreateCell("a1", CT_STRING);
 //	strSetValueMatrix(pmtx, 0, 0, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("a2", CT_STRING);
 //	strSetValueMatrix(pmtx, 0, 1, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b1", CT_STRING);
 //	strSetValueMatrix(pmtx, 1, 0, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b2", CT_STRING);
 //	strSetValueMatrix(pmtx, 1, 1, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b1", CT_STRING);
 //	strSetValueMatrix(pmtx, 2, 0, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b2", CT_STRING);
 //	strSetValueMatrix(pmtx, 2, 1, &pc, sizeof(P_CELL));
 //	p = siCreateSelectView(NULL, pmtx, cbftestsel, 0);
 //	printf("%s\n", (char *)(*(P_CELL *)strGetValueMatrix(NULL, p, 0, 0, sizeof(P_CELL)))->pdata);
 //	printf("%s\n", (char *)(*(P_CELL *)strGetValueMatrix(NULL, p, 0, 1, sizeof(P_CELL)))->pdata);
 //	printf("%s\n", (char *)(*(P_CELL *)strGetValueMatrix(NULL, p, 1, 0, sizeof(P_CELL)))->pdata);
 //	printf("%s\n", (char *)(*(P_CELL *)strGetValueMatrix(NULL, p, 1, 1, sizeof(P_CELL)))->pdata);
 //	strDeleteMatrix(pmtx);
 //	strDeleteMatrix(p);
 //	return 0;
 //}
 */
P_MATRIX siCreateSelectView(P_ARRAY_Z * pparr, P_MATRIX pmtx, SICBF_SELECT cbfsel, size_t param)
{
	P_MATRIX pmtxr = NULL;
	if (NULL == pmtx)
		return NULL;
	else
	{
		size_t i, j;
		pmtxr = strCreateMatrix(pmtx->ln, pmtx->col, sizeof(P_CELL));
		if (NULL != pparr)
			*pparr = strCreateArrayZ(BUFSIZ, sizeof(size_t));
		if (NULL != pmtxr)
		{
			for (j = i = 0; i < pmtx->ln; ++i)
			{
				if (FALSE != cbfsel(strGetValueMatrix(NULL, pmtx, i, 0, sizeof(P_CELL)), param))
				{
					if (NULL != pparr)
						*(size_t *)strLocateItemArrayZ(*pparr, sizeof(size_t), j) = i;
					memcpy
					(
						strGetValueMatrix(NULL, pmtxr, j, 0, sizeof(P_CELL)),
						strGetValueMatrix(NULL, pmtx, i, 0, sizeof(P_CELL)),
						sizeof(P_CELL) * pmtx->col
					);
					++j;
					if (NULL != pparr)
					{
						if (j >= strLevelArrayZ(*pparr))
						{
							if (NULL == strResizeArrayZ(*pparr, strLevelArrayZ(*pparr) + BUFSIZ, sizeof(size_t)))
							{
								j = 0;
								break;
							}
						}
					}
				}
			}
			if (NULL != pparr)
				strResizeArrayZ(*pparr, j, sizeof(size_t));
			if (NULL == strResizeMatrix(pmtxr, j, pmtx->col, sizeof(P_CELL)))
				return NULL;
		}
	}
	return pmtxr;
}

/* Function name: siCreateProjectView
 * Description:   Create a projection view.
 * Parameter:
 *      pmtx Pointer to a view.
 *     parrz Pointer to an ARRAY_Z that contains columns of the original view.
 * Return value:  Pointer to a new allocated view.
 * Caution:       N/A.
 * Tip:
 //int main(int argc, char * argv[])
 //{
 //	P_CELL pc;
 //	P_MATRIX pmtxa, p;
 //	P_ARRAY_Z parrz;
 //	parrz = strCreateArrayZ(2, sizeof(size_t));
 //	*(size_t *)strLocateItemArrayZ(parrz, sizeof(size_t), 0) = 2;
 //	*(size_t *)strLocateItemArrayZ(parrz, sizeof(size_t), 1) = 1;
 //	pmtxa = strCreateMatrix(3, 3, sizeof(P_CELL));
 //	pc = siCreateCell("a1", CT_STRING);
 //	strSetValueMatrix(pmtxa, 0, 0, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("a2", CT_STRING);
 //	strSetValueMatrix(pmtxa, 0, 1, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("a3", CT_STRING);
 //	strSetValueMatrix(pmtxa, 0, 2, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b1", CT_STRING);
 //	strSetValueMatrix(pmtxa, 1, 0, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b2", CT_STRING);
 //	strSetValueMatrix(pmtxa, 1, 1, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b3", CT_STRING);
 //	strSetValueMatrix(pmtxa, 1, 2, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b1", CT_STRING);
 //	strSetValueMatrix(pmtxa, 2, 0, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b2", CT_STRING);
 //	strSetValueMatrix(pmtxa, 2, 1, &pc, sizeof(P_CELL));
 //	pc = siCreateCell("b3", CT_STRING);
 //	strSetValueMatrix(pmtxa, 2, 2, &pc, sizeof(P_CELL));
 //	p = siCreateProjectView(pmtxa, parrz); // 5,3
 //	printf("%s\n", (char *)(*(P_CELL *)strGetValueMatrix(NULL, p, 0, 0, sizeof(P_CELL)))->pdata);
 //	printf("%s\n", (char *)(*(P_CELL *)strGetValueMatrix(NULL, p, 0, 1, sizeof(P_CELL)))->pdata);	
 //	printf("%s\n", (char *)(*(P_CELL *)strGetValueMatrix(NULL, p, 1, 0, sizeof(P_CELL)))->pdata);
 //	printf("%s\n", (char *)(*(P_CELL *)strGetValueMatrix(NULL, p, 1, 1, sizeof(P_CELL)))->pdata);
 //	strDeleteMatrix(pmtxa);
 //	strDeleteMatrix(p);
 //	return 0;
 //}
 */
P_MATRIX siCreateProjectView(P_MATRIX pmtx, P_ARRAY_Z parrz)
{
	P_MATRIX pmtxt = strCreateMatrix(pmtx->ln, parrz->num, sizeof(P_CELL));

	if (NULL != pmtxt)
	{
		size_t i, j;
		P_MATRIX pmtxr;
		strSortArrayZ(parrz, sizeof(size_t), _grpCBFCompareInteger);
		for (i = 0; i < parrz->num; ++i)
		{
			size_t col;
			col = *(size_t *)strLocateItemArrayZ(parrz, sizeof(size_t), i);
			for (j = 0; j < pmtx->ln; ++j)
			{
				memcpy
				(
					strGetValueMatrix(NULL, pmtxt, j, i,sizeof(P_CELL)),
					strGetValueMatrix(NULL, pmtx, j, col, sizeof(P_CELL)),
					sizeof(P_CELL)
				);
			}
		}
		pmtxr = siCreateUniqueView(pmtxt);
		strDeleteMatrix(pmtxt);
		return pmtxr;
	}
	return NULL;
}

/* Function name: siCreateCell
 * Description:   Create a new cell of a table.
 * Parameter:
 *     pitem Pointer to the value which allows you to be copied.
 *        ct The type of cell value you want to assign.
 * Return value:  Pointer to a new allocated cell.
 * Caution:       N/A.
 * Tip:           N/A.
 */
P_CELL siCreateCell(void * pitem, CellType ct)
{
	if (NULL != pitem)
	{
		void * rtn;
		size_t l = sizeof(CELL), m = l;
		switch (ct)
		{
		case CT_CHAR:
			m = sizeof(char);
			l += m;
			break;
		case CT_SHORT:
			m = sizeof(short);
			l += m;
			break;
		case CT_INTEGER:
			m = sizeof(int);
			l += m;
			break;
		case CT_LONG:
			m = sizeof(long);
			l += m;
			break;
		case CT_FLOAT:
			m = sizeof(float);
			l += m;
			break;
		case CT_DOUBLE:
			m = sizeof(double);
			l += m;
			break;
		case CT_STRING:
			m = strlen((char *)pitem) + 1;
			l += m;
			break;
		case CT_WSTRING:
			m = sizeof(wchar_t) * (wcslen((wchar_t *)pitem) + 1);
			l += m;
			break;
		}
		rtn = malloc(l);
		if (NULL != rtn)
		{
			((P_CELL)rtn)->ct = ct;
			((P_CELL)rtn)->pdata = (PUCHAR)rtn + sizeof(CELL);
			memcpy(((P_CELL)rtn)->pdata, pitem, m);
		}
		return (P_CELL)rtn;
	}
	return NULL;
}

/* Function name: siDeleteCell
 * Description:   Delete ana allocated cell.
 * Parameter:
 *    ppcell Pointer to the P_CELL you want to deallocate.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siDeleteCell(P_CELL * ppcell)
{
	free(*ppcell);
	*ppcell = NULL;
}

/* Function name: siAlterCell
 * Description:   Reallocate a cell of a table.
 * Parameter:
 *       pcl Pointer to the cell you want to reallocate.
 *    newval Pointer to the new cell value.
 * Return value:  Pointer to a allocated cell.
 * Caution:       Parameter newval shall not be NULL.
 * Tip:           N/A.
 */
P_CELL siAlterCell(P_CELL pcl, void * newval)
{
	void * rtn = NULL;
	if (NULL != pcl)
	{
		size_t l = sizeof(CELL), m = l;
		switch (pcl->ct)
		{
		case CT_CHAR:
			m = sizeof(char);
			l += m;
			break;
		case CT_SHORT:
			m = sizeof(short);
			l += m;
			break;
		case CT_INTEGER:
			m = sizeof(int);
			l += m;
			break;
		case CT_LONG:
			m = sizeof(long);
			l += m;
			break;
		case CT_FLOAT:
			m = sizeof(float);
			l += m;
			break;
		case CT_DOUBLE:
			m = sizeof(double);
			l += m;
			break;
		case CT_STRING:
			m = strlen((char *)newval) + 1;
			l += m;
			break;
		case CT_WSTRING:
			m = wcslen((wchar_t *)newval) + 1;
			l += m * sizeof(wchar_t);
			break;
		}
		rtn = realloc(pcl, l);
		if (NULL != rtn)
		{
			((P_CELL)rtn)->ct = pcl->ct;
			((P_CELL)rtn)->pdata = (PUCHAR)rtn + sizeof(CELL);
			memcpy(((P_CELL)rtn)->pdata, newval, m);
		}
	}
	return (P_CELL)rtn;
}
