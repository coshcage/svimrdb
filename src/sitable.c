/*
 * Name:        sitable.c
 * Description: SI functions for tables.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0628231947C0000000000L00000
 * License:     GPLv2.
 */

#include <stdio.h>  /* Using function printf. */
#include <stdlib.h> /* Using function malloc, free. */
#include <string.h> /* Using function strdup. */
#include <stdarg.h>
#include "svimrdb.h"

#define strdup _strdup

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

// TODO:
// siCreateTable (x)
// siDeleteTable (x)
// siAddColumn
// SiDropColumn
// siInsertInto (x)
// siDeleteFrom (x)
// siUpdateCell (x)

P_TABLE siCreateTable(char * tblname, P_ARRAY_Z parrhdr)
{
	P_TABLE ptbl = (P_TABLE)malloc(sizeof(TABLE));
	if (NULL != ptbl)
	{
		size_t i;
		ptbl->tblname = strdup(tblname);
		strCopyArrayZ(&(ptbl->header), parrhdr, sizeof(TBLHDR));
		for (i = 0; i < parrhdr->num; ++i)
		{
			P_TBLHDR pt = strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), i);
			pt->strname = strdup(pt->strname);
		}
		strInitMatrix(&ptbl->tbldata, 0, parrhdr->num, sizeof(P_CELL));
	}
	return ptbl;
}

void siDeleteTable(P_TABLE ptbl)
{
	size_t i;
	free(ptbl->tblname);
	
	for (i = 0; i < ptbl->header.num; ++i)
	{
		P_TBLHDR pt = strLocateItemArrayZ(&(ptbl->header), sizeof(TBLHDR), i);
		free(pt->strname);
	}
	strFreeArrayZ(&(ptbl->header));

	if (NULL != ptbl->tbldata.arrz.pdata)
	{
		siDestoryView(&(ptbl->tbldata));
		strFreeMatrix(&(ptbl->tbldata));
	}
}

BOOL siInsertIntoTable(P_TABLE ptbl, ...)
{
	size_t i, j;
	j = ptbl->tbldata.ln;
	if (NULL != strResizeMatrix(&(ptbl->tbldata), ptbl->tbldata.ln + 1, ptbl->tbldata.col, sizeof(P_CELL)))
	{

		va_list arg;
		va_start(arg, ptbl->header.num);

		++ptbl->tbldata.ln;
		for (i = 0; i < ptbl->header.num; ++i)
		{
			P_CELL pc;
			P_TBLHDR pt = strLocateItemArrayZ(&(ptbl->header), sizeof(TBLHDR), i);
			switch (pt->ct)
			{
			case CT_CHAR:
				pc = siCreateCell(va_arg(arg, char *), pt->ct);
				break;
			case CT_SHORT:
				pc = siCreateCell(va_arg(arg, short *), pt->ct);
				break;
			case CT_INTEGER:
				pc = siCreateCell(va_arg(arg, int *), pt->ct);
				break;
			case CT_LONG:
				pc = siCreateCell(va_arg(arg, long *), pt->ct);
				break;
			case CT_STRING:
				pc = siCreateCell(va_arg(arg, char **), pt->ct);
				break;
			}
			strSetValueMatrix(&(ptbl->tbldata), j, i, &pc, sizeof(P_CELL));
		}

		va_end(arg);
		return TRUE;
	}
	return FALSE;
}

BOOL siDeleteFromTable(P_TABLE ptbl, size_t col)
{
	if (col < ptbl->tbldata.col)
	{
		strRemoveItemArrayZ(&(ptbl->tbldata.arrz), sizeof(P_CELL) * ptbl->tbldata.col, col, TRUE);
		return TRUE;
	}
	return FALSE;
}

void siUpdateTableCell(P_TABLE ptbl, void * pval, CellType ct, size_t ln, size_t col)
{
	P_CELL pc = siCreateCell(pval, ct);
	siDeleteCell((P_CELL *)strGetValueMatrix(NULL, &ptbl->tbldata, ln, col, sizeof(P_CELL)));
	strSetValueMatrix(&ptbl->tbldata, ln, col, &pc, sizeof(P_CELL));
}

void siAddTableColumn()
{

}

void siDropTableColumn()
{

}
