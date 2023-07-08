/*
 * Name:        sitable.c
 * Description: SI functions for tables.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0628231947C0708231121L00657
 * License:     GPLv2.
 */

#include <stdio.h>  /* Using function printf, macro BUFSIZ. */
#include <stdlib.h> /* Using function malloc, free, qsort. */
#include <string.h> /* Using function strdup, memmove. */
#include <math.h>   /* Using function round, roundf. */
#include <stdarg.h>
#include "svimrdb.h"

static size_t sizSVTarget = 0; /* Variable for sorting column of a table. */
static BOOL   bAscend = TRUE;  /* Ascend or descend for TRUE or FALSE respectively. */

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _sicbfcmpSV
 * Description:   This function is used to compare cells of a view.
 * Parameters:
 *         px Pointer to pointer of P_CELL.
 *         py Pointer to pointer of P_CELL.
 * Return value:  > 0, < 0 and ==0 (Represents equal).
 * Caution:   This function is not reentrant.
 */
static int _sicbfcmpSV(const void * px, const void * py)
{
	int r = 0;
	P_CELL pcx, pcy;
	pcx = *((P_CELL *)px + sizSVTarget);
	pcy = *((P_CELL *)py + sizSVTarget);
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
		}
	}
	else if (NULL == pcx && NULL != pcy)
		return -1;
	else if (NULL != pcx && NULL == pcy)
		return 1;
	if (bAscend)
		return r;
	else
		return -1 * r;
}

/* Function name: siSortView
 * Description:   Sort view.
 * Parameter:
 *      pmtx Pointer to a view.
 *       col Column you want to sort. Starts from 0.
 *      ascd TRUE for ascend. FALSE for descend.
 * Return value:  N/A.
 * Caution:       Parameter pmtx must be allocated first.
 * Tip:           N/A.
 */
void siSortView(P_MATRIX pmtx, size_t col, BOOL ascd)
{
	sizSVTarget = col;
	bAscend = ascd;
	qsort(pmtx->arrz.pdata, pmtx->ln, sizeof(P_CELL) * pmtx->col, _sicbfcmpSV);
}

/* Function name: siInstantiateView
 * Description:   Initialize a view and its cell.
 * Parameter:
 *      pmtx Pointer to a view.
 * Return value:  A new allocated view with all initialized cell.
 * Caution:       N/A.
 * Tip:           N/A.
 */
P_MATRIX siInstantiateView(P_MATRIX pmtx)
{
	if (NULL != pmtx)
	{
		P_MATRIX pmr = strCreateMatrix(pmtx->ln, pmtx->col, sizeof(P_CELL));
		if (NULL != pmr)
		{
			size_t i, j;
			for (i = 0; i < pmtx->ln; ++i)
			{
				for (j = 0; j < pmtx->col; ++j)
				{
					P_CELL pcold, pcnew;
					strGetValueMatrix(&pcold, pmtx, i, j, sizeof(P_CELL));
					if (NULL != pcold)
						pcnew = siCreateCell(pcold->pdata, pcold->ct);
					else
						pcnew = NULL;
					strSetValueMatrix(pmr, i, j, &pcnew, sizeof(P_CELL));
				}
			}
		}
		return pmr;
	}
	return NULL;
}

/* Function name: siDestoryView
 * Description:   Uninitialize a view and its cell.
 * Parameter:
 *      pmtx Pointer to a view.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siDestoryView(P_MATRIX pmtx)
{
	if (NULL != pmtx)
	{
		size_t i, j;
		for (i = 0; i < pmtx->ln; ++i)
		{
			for (j = 0; j < pmtx->col; ++j)
			{
				P_CELL * ppcold = (P_CELL *)strGetValueMatrix(NULL, pmtx, i, j, sizeof(P_CELL));
				if (NULL != *ppcold)
					siDeleteCell(ppcold);
			}
		}
	}
}

/* Function name: siPrintView
 * Description:   Print a view to stdout.
 * Parameter:
 *      pmtx Pointer to a view.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siPrintView(P_MATRIX pmtx)
{
	if (NULL != pmtx)
	{
		size_t i, j;
		for (i = 0; i < pmtx->ln; ++i)
		{
			for (j = 0; j < pmtx->col; ++j)
			{
				P_CELL pc = NULL;
				strGetValueMatrix(&pc, pmtx, i, j, sizeof(P_CELL));
				if (NULL != pc)
				{
					switch (pc->ct)
					{
					case CT_CHAR:
						printf("%c\t", *(char *)pc->pdata);
						break;
					case CT_SHORT:
						printf("%d\t", *(short *)pc->pdata);
						break;
					case CT_INTEGER:
						printf("%d\t", *(int *)pc->pdata);
						break;
					case CT_LONG:
						printf("%ld\t", *(long *)pc->pdata);
						break;
					case CT_FLOAT:
						printf("%f\t", *(float *)pc->pdata);
						break;
					case CT_DOUBLE:
						printf("%lf\t", *(double *)pc->pdata);
						break;
					case CT_STRING:
						printf("%s\t", (char *)pc->pdata);
						break;
					}
				}
				else
					printf("%s\t", "null");
			}
			printf("%c", '\n');
		}
	}
}

/* Function name: siCreateViewOfTable
 * Description:   Create a view of table.
 * Parameter:
 *      ptbl Pointer to a table.
 * Return value:  Pointer to a view.
 * Caution:       Parameter ptbl must be allocated first..
 * Tip:           N/A.
 */
P_MATRIX siCreateViewOfTable(P_TABLE ptbl)
{
	P_MATRIX pmtx = strCreateMatrix(ptbl->tbldata.ln, ptbl->tbldata.col, sizeof(P_CELL));
	if (NULL != pmtx)
		strCopyMatrix(pmtx, &ptbl->tbldata, sizeof(P_CELL));
	return pmtx;
}

/* Function name: siCreateTable
 * Description:   Print a view to stdout.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *   tblname Pointer to a string that indicates table name.
 *   parrhdr Pointer to an ARRAY_Z that contains table header description.
 *           Each element of this ARRAY_Z is TBLHDR.
 * Return value:  Pointer to a table.
 * Caution:       Parameter parrhdr must be allocated first..
 * Tip:           N/A.
 */
P_TABLE siCreateTable(P_TRANS ptrans, char * tblname, P_ARRAY_Z parrhdr)
{
	P_TABLE ptbl = (P_TABLE)malloc(sizeof(TABLE));
	if (NULL != ptbl)
	{
		size_t i;

		ptbl->tblname = strdup(tblname);
		siStrLCase(ptbl->tblname);

		strInitArrayZ(&ptbl->header, parrhdr->num, sizeof(TBLHDR));
		strCopyArrayZ(&ptbl->header, parrhdr, sizeof(TBLHDR));

		for (i = 0; i < parrhdr->num; ++i)
		{
			P_TBLHDR pt = strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i);
			pt->strname = strdup(pt->strname);
			siStrLCase(pt->strname);
		}
		strInitMatrix(&ptbl->tbldata, 0, parrhdr->num, sizeof(P_CELL));
	}

	if (NULL != ptrans)
	{
		DATALT da;
		da.at = AT_ADD_TABLE;
		da.ptbl = ptbl;
		
		queInjectDL(&ptrans->qoprlst, &da, sizeof(DATALT));
	}

	return ptbl;
}

/* Function name: siCopyTable
 * Description:   Copy and recreate a table.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      ptbl Pointer to a table.
 * Return value:  Pointer to a new table.
 * Caution:       Parameter ptbl must be allocated first..
 * Tip:           N/A.
 */
P_TABLE siCopyTable(P_TRANS ptrans, P_TABLE ptbl)
{
	P_TABLE pr = siCreateTable(ptrans, ptbl->tblname, &ptbl->header);
	if (NULL != pr)
	{
		if (NULL != strResizeMatrix(&pr->tbldata, ptbl->tbldata.ln, ptbl->tbldata.col, sizeof(P_CELL)))
		{
			size_t i, j;
			pr->tbldata.ln = ptbl->tbldata.ln;
			pr->tbldata.col = ptbl->tbldata.col;
			for (i = 0; i < ptbl->tbldata.ln; ++i)
			{
				for (j = 0; j < ptbl->tbldata.col; ++j)
				{
					P_CELL pc;
					strGetValueMatrix(&pc, &ptbl->tbldata, i, j, sizeof(P_CELL));
					if (NULL != pc)
						pc = siCreateCell(pc->pdata, pc->ct);
					strSetValueMatrix(&pr->tbldata, i, j, &pc, sizeof(P_CELL));
				}
			}
		}
	}
	return pr;
}

/* Function name: siDeleteTable
 * Description:   Delete a table.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      ptbl Pointer to a table you want to delete.
 * Return value:  N/A.
 * Caution:       Parameter ptbl must be allocated first..
 * Tip:           N/A.
 */
void siDeleteTable(P_TRANS ptrans, P_TABLE ptbl)
{
	size_t i;

	if (NULL != ptrans)
	{
		DATALT da;
		da.at = AT_DEL_TABLE;
		da.ptbl = ptbl;
		da.data.datbl = siCopyTable(NULL, ptbl);

		queInjectDL(&ptrans->qoprlst, &da, sizeof(DATALT));
	}
	
	free(ptbl->tblname);
	
	for (i = 0; i < ptbl->header.num; ++i)
	{
		P_TBLHDR pt = strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i);
		free(pt->strname);
	}
	strFreeArrayZ(&ptbl->header);

	if (NULL != ptbl->tbldata.arrz.pdata)
	{
		siDestoryView(&ptbl->tbldata);
		strFreeMatrix(&ptbl->tbldata);
	}
	free(ptbl);
	
}

/* Function name: siInsertIntoTable
 * Description:   Insert a tuple into a table.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      ptbl Pointer to a table you want to insert.
 *       ... Parameters you want to insert.
 * Return value:  TRUE insertion succeeded. FALSE insertion failed.
 * Caution:       Parameter ptbl must be allocated first..
 * Tip:           N/A.
 */
BOOL siInsertIntoTable(P_TRANS ptrans, P_TABLE ptbl, ...)
{
	size_t i, j;
	j = ptbl->tbldata.ln;
	if (NULL != strResizeMatrix(&(ptbl->tbldata), ptbl->tbldata.ln + 1, ptbl->tbldata.col, sizeof(P_CELL)))
	{

		va_list arg;
		va_start(arg, ptbl);

		++ptbl->tbldata.ln;
		for (i = 0; i < ptbl->header.num; ++i)
		{
			union un_CellData
			{
				char   c;
				short  s;
				int    i;
				long   l;
				float  f;
				double d;
			} cd;
			P_CELL pc;
			P_TBLHDR pt = strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i);
			switch (pt->ct)
			{
			case CT_CHAR:
				cd.c = va_arg(arg, char);
				pc = siCreateCell(&cd.c, pt->ct);
				break;
			case CT_SHORT:
				cd.s = va_arg(arg, short);
				pc = siCreateCell(&cd.s, pt->ct);
				break;
			case CT_INTEGER:
				cd.i = va_arg(arg, int);
				pc = siCreateCell(&cd.i, pt->ct);
				break;
			case CT_LONG:
				cd.l = va_arg(arg, long);
				pc = siCreateCell(&cd.l, pt->ct);
				break;
			case CT_FLOAT:
				cd.f = va_arg(arg, float);
				pc = siCreateCell(&cd.f, pt->ct);
				break;
			case CT_DOUBLE:
				cd.d = va_arg(arg, double);
				pc = siCreateCell(&cd.d, pt->ct);
				break;
			case CT_STRING:
				pc = siCreateCell(va_arg(arg, char *), pt->ct);
				break;
			}
			strSetValueMatrix(&ptbl->tbldata, j, i, &pc, sizeof(P_CELL));
		}

		va_end(arg);

		if (NULL != ptrans)
		{
			DATALT da;
			da.at = AT_ADD_TUPLE;
			da.ptbl = ptbl;
			da.data.datpl.sizln = j;

			queInjectDL(&ptrans->qoprlst, &da, sizeof(DATALT));
		}

		return TRUE;
	}
	return FALSE;
}

/* Function name: siDeleteFromTable
 * Description:   Delete a tuple from a table.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      ptbl Pointer to a table you want to delete from.
 *        ln Row number you want to delete. Starts from 0.
 * Return value:  TRUE deletion succeeded. FALSE deletion failed.
 * Caution:       Parameter ptbl must be allocated first..
 * Tip:           N/A.
 */
BOOL siDeleteFromTable(P_TRANS ptrans, P_TABLE ptbl, size_t ln)
{
	if (ln < ptbl->tbldata.ln)
	{
		size_t i;
		if (NULL != ptrans)
		{
			DATALT da;
			da.at = AT_DEL_TUPLE;
			da.ptbl = ptbl;

			da.data.datpl.sizln = ln;

			strInitArrayZ(&da.data.datpl.tupledat, strLevelArrayZ(&ptbl->header), sizeof(P_CELL));
			for (i = 0; i < strLevelArrayZ(&ptbl->header); ++i)
			{
				P_CELL pc;
				strGetValueMatrix(&pc, &ptbl->tbldata, ln, i, sizeof(P_CELL));
				if (NULL == pc)
					*(P_CELL *)strLocateItemArrayZ(&da.data.datpl.tupledat, sizeof(P_CELL), i) = NULL;
				else
					*(P_CELL *)strLocateItemArrayZ(&da.data.datpl.tupledat, sizeof(P_CELL), i) = siCreateCell(pc->pdata, pc->ct);
			}

			queInjectDL(&ptrans->qoprlst, &da, sizeof(DATALT));
		}
		
		for (i = 1; i < ptbl->tbldata.col; ++i)
			siDeleteCell((P_CELL *)strGetValueMatrix(NULL, &ptbl->tbldata, ln, i, sizeof(P_CELL)));

		memmove
		(
			&ptbl->tbldata.arrz.pdata[ln * sizeof(P_CELL) * ptbl->tbldata.col],
			&ptbl->tbldata.arrz.pdata[(ln + 1) * sizeof(P_CELL) * ptbl->tbldata.col],
			(ptbl->tbldata.ln - 1 - ln) * sizeof(P_CELL) * ptbl->tbldata.col
		);

		strResizeMatrix(&ptbl->tbldata, ptbl->tbldata.ln - 1, ptbl->tbldata.col, sizeof(P_CELL));
		--ptbl->tbldata.ln;
		return TRUE;
	}
	return FALSE;
}

/* Function name: siUpdateTableCell
 * Description:   Update a cell.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      ptbl Pointer to a table that contains the certain cell.
 *      pval Pointer to the new cell data.
 *        ct Cell type.
 *        ln Cell row number. Starts from 0.
 *       col Cel column number. Starts from 0.
 * Return value:  N/A.
 * Caution:       Parameter ptbl must be allocated first..
 * Tip:           N/A.
 */
void siUpdateTableCell(P_TRANS ptrans, P_TABLE ptbl, void * pval, CellType ct, size_t ln, size_t col)
{
	P_CELL pc;

	strGetValueMatrix(&pc, &ptbl->tbldata, ln, col, sizeof(P_CELL));

	if (NULL != ptrans)
	{
		DATALT da;
		da.at = AT_ALTER_CELL;
		da.ptbl = ptbl;

		da.data.dacell.ln = ln;
		da.data.dacell.col = col;
		if (NULL != pc)
			da.data.dacell.before = siCreateCell(pc->pdata, pc->ct);
		else
			da.data.dacell.before = NULL;

		queInjectDL(&ptrans->qoprlst, &da, sizeof(DATALT));
	}

	if (NULL != pc)
		siDeleteCell((P_CELL *)strGetValueMatrix(NULL, &ptbl->tbldata, ln, col, sizeof(P_CELL)));
	pc = siCreateCell(pval, ct);
	strSetValueMatrix(&ptbl->tbldata, ln, col, &pc, sizeof(P_CELL));
}

/* Function name: siAddTableColumn
 * Description:   Add a column of a table.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      ptbl Pointer to a table you want to operate.
 *      phdr Pointer to a TBLHDR structure that contains new column description.
 * Return value:  TRUE adding succeeded. FALSE adding failed.
 * Caution:       Parameter ptbl must be allocated first..
 * Tip:           N/A.
 */
BOOL siAddTableColumn(P_TRANS ptrans, P_TABLE ptbl, P_TBLHDR phdr)
{
	size_t i, j;
	for (i = 0; i < strLevelArrayZ(&ptbl->header); ++i)
	{
		P_TBLHDR pt;
		siStrLCase(phdr->strname);
		pt = (P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i);
		if (strcmp(pt->strname, phdr->strname) == 0)
			return FALSE;
	}
	j = i = strLevelArrayZ(&ptbl->header);

	if (NULL != strResizeArrayZ(&ptbl->header, strLevelArrayZ(&ptbl->header) + 1, sizeof(TBLHDR)))
	{
		TBLHDR th;
		th.ct = phdr->ct;
		th.strname = strdup(phdr->strname);
		strInsertItemArrayZ(&ptbl->header, &th, sizeof(TBLHDR), i);
	}
	else
		return FALSE;

	strResizeMatrix(&ptbl->tbldata, ptbl->tbldata.ln, ptbl->tbldata.col + 1, sizeof(P_CELL));
	++ptbl->tbldata.col;
	for (i = 0; i < ptbl->tbldata.ln; ++i)
	{
		P_CELL pc = NULL;
		strSetValueMatrix(&ptbl->tbldata, i, ptbl->tbldata.col - 1, &pc, sizeof(P_CELL));
	}

	if (NULL != ptrans)
	{
		DATALT da;
		da.at = AT_ADD_COLUMN;
		da.ptbl = ptbl;
		da.data.dacol.sizcol = j;

		queInjectDL(&ptrans->qoprlst, &da, sizeof(DATALT));
	}
	
	return TRUE;
}

/* Function name: siDropTableColumn
 * Description:   Delete a column of a table.
 * Parameter:
 *    ptrans Pointer to a transaction.
 *      ptbl Pointer to a table you want to operate.
 *       col Column number of the table. Starts from 0.
 * Return value:  TRUE dropping succeeded. FALSE dropping failed.
 * Caution:       Parameter ptbl must be allocated first..
 * Tip:           N/A.
 */
BOOL siDropTableColumn(P_TRANS ptrans, P_TABLE ptbl, size_t col)
{
	size_t i, j;
	P_ARRAY_Z parrcol;
	P_MATRIX pview;

	if (col >= ptbl->tbldata.col)
		return FALSE;

	if (NULL != ptrans)
	{
		DATALT da;
		da.at = AT_DEL_COLUMN;
		da.ptbl = ptbl;
		da.data.dacol.sizcol = col;
		da.data.dacol.hdr = *(TBLHDR *)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), col);
		da.data.dacol.hdr.strname = strdup((*(TBLHDR *)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), col)).strname);

		strInitArrayZ(&da.data.dacol.coldat, ptbl->tbldata.ln, sizeof(P_CELL));
		for (i = 0; i < ptbl->tbldata.ln; ++i)
		{
			P_CELL pc;
			strGetValueMatrix(&pc, &ptbl->tbldata, i, col, sizeof(P_CELL));
			if (NULL == pc)
				*(P_CELL *)strLocateItemArrayZ(&da.data.dacol.coldat, sizeof(P_CELL), i) = NULL;
			else
				*(P_CELL *)strLocateItemArrayZ(&da.data.dacol.coldat, sizeof(P_CELL), i) = siCreateCell(pc->pdata, pc->ct);
		}

		queInjectDL(&ptrans->qoprlst, &da, sizeof(DATALT));
	}

	parrcol = strCreateArrayZ(BUFSIZ, sizeof(size_t));

	for (j = i = 0; i < ptbl->header.num; ++i)
	{
		if (i != col)
		{
			*(size_t *)strLocateItemArrayZ(parrcol, sizeof(size_t), j) = i;
			if (++j >= strLevelArrayZ(parrcol))
			{
				if (NULL == strResizeArrayZ(parrcol, strLevelArrayZ(parrcol) + BUFSIZ, sizeof(size_t)))
				{
					j = 0;
					break;
				}
			}
		}
	}
	if (NULL == strResizeArrayZ(parrcol, j, sizeof(size_t)))
		return FALSE;

	free(((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), col))->strname);
	strRemoveItemArrayZ(&ptbl->header, sizeof(TBLHDR), col, TRUE);

	pview = siCreateProjectView(&ptbl->tbldata, parrcol);

	if (NULL != pview)
	{
		for (i = 0; i < ptbl->tbldata.ln; ++i)
		{
			P_CELL * ppc;
			ppc = (P_CELL *)strGetValueMatrix(NULL, &ptbl->tbldata, i, col, sizeof(P_CELL));
			siDeleteCell(ppc);
		}

		strCopyArrayZ(&ptbl->tbldata.arrz, &pview->arrz, sizeof(P_CELL));
		--ptbl->tbldata.col;

		strDeleteMatrix(pview);

		return TRUE;
	}
	return FALSE;
}
