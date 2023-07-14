/*
 * Name:        sixmem.h
 * Description: SI external memory function.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0714231200H0000000000L00000
 * License:     GPLv2.
 */
#include "sixmem.h"
#include <string.h> /* Using funtion strdup. */
#include <stdlib.h> /* Using function malloc. */

void _siReadString(char * buf, FILE * fp)
{
	size_t i = 0;
	while ((i < BUFSIZ - 1) && (*buf = fgetc(fp)))
	{
		++buf;
		++i;
	}
	buf[i] = '\0';
}

void _siWriteString(char * str, FILE * fp)
{
	while (*str)
	{
		fputc(*str, fp);
		++str;
	}
	/* Put 0. */
	fputc(*str, fp);
}

void siSaveTable(FILE * fp, long lpos, P_TABLE ptbl)
{
	if (NULL != ptbl && NULL != fp)
	{
		size_t i, j;

		fseek(fp, lpos, SEEK_SET);

		/* Write magic number. */
		fputc('d', fp);
		fputc('b', fp);

		/* Write table header count. */
		fwrite(&ptbl->header.num, sizeof(size_t), 1, fp);

		/* Write table header. */
		for (i = 0; i < strLevelArrayZ(&ptbl->header); ++i)
		{
			j = ((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->ct;
			fwrite(&j, sizeof(size_t), 1, fp);
		}

		/* Write table column name. */
		for (i = 0; i < strLevelArrayZ(&ptbl->header); ++i)
			_siWriteString(((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->strname, fp);
			
		/* Write table size. */
		fwrite(&ptbl->tbldata.ln, sizeof(size_t), 1, fp);
		fwrite(&ptbl->tbldata.col, sizeof(size_t), 1, fp);

		// TODO:

	}
}

P_TABLE siLoadTable(FILE * fp, long lpos)
{
	if (NULL != fp)
	{
		P_TABLE ptbl = (P_TABLE) malloc(sizeof(TABLE));

		char magic[2] = { 0 };
		size_t i, j, l, m;

		fseek(fp, lpos, SEEK_SET);

		/* Read magic number. */
		fread(magic, sizeof(char), 2, fp);
		if ('d' == magic[0] && 'b' == magic[1] && NULL != ptbl)
		{
			long oldl;
			XCELL xc;
			P_CELL * ppc;
			char buf[BUFSIZ] = { 0 };
			/* Read table header count. */
			fread(&j, sizeof(size_t), 1, fp);

			strInitArrayZ(&ptbl->header, j, sizeof(TBLHDR));

			/* Read table header. */
			for (i = 0; i < j; ++i)
				fread(&((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->ct, sizeof(size_t), 1, fp);

			/* Read column names. */
			for (i = 0; i < j; ++i)
			{
				_siReadString(buf, fp);
				((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->strname = strdup(buf);
			}

			/* Read table size. */
			fread(&l, sizeof(size_t), 1, fp);
			fread(&m, sizeof(size_t), 1, fp);

			strResizeMatrix(&ptbl->tbldata, l, m, sizeof(P_CELL));
			ppc = (P_CELL *)strGetValueMatrix(NULL, &ptbl->tbldata, 0, 0, sizeof(P_CELL));

			for (i = 0; i < l * m; ++i)
			{
				P_CELL pc = NULL;
				union un_CellData
				{
					char   c;
					short  s;
					int    i;
					long   l;
					float  f;
					double d;
				} cd;
				fread(&xc, sizeof(XCELL), 1, fp);
				oldl = ftell(fp);
				fseek(fp, xc.fpos, SEEK_SET);
				switch (xc.ct)
				{
				case CT_NULL:
					pc = NULL;
					break;
				case CT_CHAR:
					fread(&cd.c, sizeof(char), 1, fp);
					pc = siCreateCell(&cd.c, xc.ct);
					break;
				case CT_SHORT:
					fread(&cd.s, sizeof(short), 1, fp);
					pc = siCreateCell(&cd.s, xc.ct);
					break;
				case CT_INTEGER:
					fread(&cd.i, sizeof(int), 1, fp);
					pc = siCreateCell(&cd.i, xc.ct);
					break;
				case CT_LONG:
					fread(&cd.l, sizeof(long), 1, fp);
					pc = siCreateCell(&cd.l, xc.ct);
					break;
				case CT_FLOAT:
					fread(&cd.f, sizeof(float), 1, fp);
					pc = siCreateCell(&cd.f, xc.ct);
					break;
				case CT_DOUBLE:
					fread(&cd.d, sizeof(double), 1, fp);
					pc = siCreateCell(&cd.d, xc.ct);
					break;
				case CT_STRING:
					_siReadString(buf, fp);
					pc = siCreateCell(buf, xc.ct);
					break;
				}
				*(ppc++) = pc;
				fseek(fp, oldl, SEEK_SET);
			}
			
		}
		return ptbl;
	}
	return NULL;
}
