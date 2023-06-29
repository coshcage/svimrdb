/*
 * Name:        sitable.c
 * Description: SI functions for tables.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0628231947C0000000000L00000
 * License:     GPLv2.
 */

#include <stdio.h> /* Using function printf. */
#include "svimrdb.h"

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
