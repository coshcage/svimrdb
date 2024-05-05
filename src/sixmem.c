/*
 * Name:        sixmem.c
 * Description: SI external memory function.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0714231200H0505240225L00389
 * License:     GPLv2.
 */
#include "sixmem.h"
#include "svimrdb.h" /* Using macro TBL_LN_BUF_SIZ. */
#include <string.h>  /* Using funtion strdup. */
#include <stdlib.h>  /* Using function malloc. */

/* DB Table file structure.
 * platform size for a size_t.
 * db for magical number.
 * 1 table header count.
 * CT_INTEGER table header.
 * number\0 table column name
 * xcell.ct CT_INTEGER XCELL.
 * xcell.fpos 0x00000000 XCELL.
 * 0x12345678 data.
 */

 /* Attention:     This Is An Internal Function. No Interface for Library Users.
  * Function name: _siReadString
  * Description:   Read string from file.
  * Parameters:
  *        buf Pointer to a string buffer.
  *         fp Pointer to a file.
  * Return value:  N/A.
  * Caution:       N/A.
  */
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

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _siReadWString
 * Description:   Read wide string from file.
 * Parameters:
 *        buf Pointer to a wide string buffer.
 *         fp Pointer to a file.
 * Return value:  N/A.
 * Caution:       N/A.
 */
void _siReadWString(wchar_t * buf, FILE * fp)
{
	fgetws(buf, BUFSIZ - 1, fp);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _siWriteString
 * Description:   Write string to a file.
 * Parameters:
 *        str Pointer to a string.
 *         fp Pointer to a file.
 * Return value:  N/A.
 * Caution:       N/A.
 */
void _siWriteString(char * str, FILE * fp)
{
	while (*str)
	{
		fputc(*str, fp);
		++str;
	}
	/* Put 0. */
	fputc('\0', fp);
}

/* Attention:     This Is An Internal Function. No Interface for Library Users.
 * Function name: _siWriteWString
 * Description:   Write wide string to a file.
 * Parameters:
 *        str Pointer to a wide string.
 *         fp Pointer to a file.
 * Return value:  N/A.
 * Caution:       N/A.
 */
void _siWriteWString(wchar_t * str, FILE * fp)
{
	while (*str)
	{
		fputwc(*str, fp);
		++str;
	}
	/* Put 0. */
	fputwc(L'\0', fp);
}

/* Function name: siSaveTable
 * Description:   Save table to a file.
 * Parameters:
 *         fp Pointer to a file.
 *       lpos Start position of the file.
 *       ptbl Pointer to a table.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
void siSaveTable(FILE * fp, long lpos, P_TABLE ptbl)
{
	if (NULL != ptbl && NULL != fp)
	{
		XCELL xc = { 0 };
		P_CELL * ppc;
		size_t i, j;
		long oldl, curl;

		fseek(fp, lpos, SEEK_SET);

		/* Write magic number. */
		fputc('d', fp);
		fputc('b', fp);

		/* Write platform size. */
		j = siPlatformSize();
		fwrite(&j, sizeof(size_t), 1, fp);

		/* Write table name. */
		_siWriteString(ptbl->tblname, fp);

		/* Write table header count. */
		fwrite(&ptbl->header.num, sizeof(size_t), 1, fp);

		/* Write table header. */
		for (i = 0; i < strLevelArrayZ(&ptbl->header); ++i)
		{
			j = ((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->ct;
			fwrite(&j, sizeof(size_t), 1, fp);
			j = ((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->cr;
			fwrite(&j, sizeof(size_t), 1, fp);
		}

		/* Write table column name. */
		for (i = 0; i < strLevelArrayZ(&ptbl->header); ++i)
			_siWriteString(((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->strname, fp);
			
		/* Write table size. */
		fwrite(&ptbl->curln, sizeof(size_t), 1, fp);
		fwrite(&ptbl->tbldata.col, sizeof(size_t), 1, fp);

		oldl = ftell(fp);

		/* Fill xcell. */
		for (i = 0; i < ptbl->curln * ptbl->tbldata.col; ++i)
			fwrite(&xc, sizeof(XCELL), 1, fp);

		/* Fill content. */
		ppc = (P_CELL *)strGetValueMatrix(NULL, &ptbl->tbldata, 0, 0, sizeof(P_CELL));
		for (i = 0; i < ptbl->curln * ptbl->tbldata.col; ++i)
		{
			curl = ftell(fp);

			fseek(fp, oldl + i * sizeof(XCELL), SEEK_SET);
			if (NULL == *ppc)
				xc.ct = CT_NULL;
			else
				xc.ct = (*ppc)->ct;
			xc.fpos = curl;
			fwrite(&xc, sizeof(XCELL), 1, fp);

			fseek(fp, curl, SEEK_SET);

			switch (xc.ct)
			{
			case CT_NULL:
				
				break;
			case CT_CHAR:
				fwrite((*ppc)->pdata, sizeof(char), 1, fp);
				break;
			case CT_SHORT:
				fwrite((*ppc)->pdata, sizeof(short), 1, fp);
				break;
			case CT_INTEGER:
				fwrite((*ppc)->pdata, sizeof(int), 1, fp);
				break;
			case CT_LONG:
				fwrite((*ppc)->pdata, sizeof(long), 1, fp);
				break;
			case CT_FLOAT:
				fwrite((*ppc)->pdata, sizeof(float), 1, fp);
				break;
			case CT_DOUBLE:
				fwrite((*ppc)->pdata, sizeof(double), 1, fp);
				break;
			case CT_STRING:
				_siWriteString((char *)(*ppc)->pdata, fp);
				break;
			case CT_WSTRING:
				_siWriteWString((wchar_t *)(*ppc)->pdata, fp);
				break;
			}

			++ppc;
		}
	}
}

/* Function name: siLoadTable
 * Description:   Load data to a table.
 * Parameters:
 *         fp Pointer to a file.
 *       lpos Start position of the file.
 * Return value:  N/A.
 * Caution:       N/A.
 * Tip:           N/A.
 */
P_TABLE siLoadTable(FILE * fp, long lpos)
{
	if (NULL != fp)
	{
		P_TABLE ptbl = NULL;

		char magic[2] = { 0 };
		size_t i, j, l, m;

		fseek(fp, lpos, SEEK_SET);

		/* Read magic number. */
		fread(magic, sizeof(char), 2, fp);
		/* Read platform size. */
		fread(&j, sizeof(size_t), 1, fp);

		if ('d' == magic[0] && 'b' == magic[1] && siPlatformSize() == j)
		{
			stdiv_t d;
			long oldl;
			XCELL xc;
			P_CELL * ppc;
			char buf[BUFSIZ] = { 0 };
			wchar_t wbuf[BUFSIZ] = { 0 };
			
			ptbl = (P_TABLE) malloc(sizeof(TABLE));
			
			if (NULL == ptbl)
				return NULL;

			/* Read table name. */
			_siReadString(buf, fp);
			ptbl->tblname = strdup(buf);

			/* Read table header count. */
			fread(&j, sizeof(size_t), 1, fp);

			strInitArrayZ(&ptbl->header, j, sizeof(TBLHDR));

			/* Read table header. */
			for (i = 0; i < j; ++i)
			{
				fread(&((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->ct, sizeof(size_t), 1, fp);
				fread(&((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->cr, sizeof(size_t), 1, fp);
				((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->phsh = NULL;
			}

			/* Read column names. */
			for (i = 0; i < j; ++i)
			{
				_siReadString(buf, fp);
				((P_TBLHDR)strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), i))->strname = strdup(buf);
			}

			/* Read table size. */
			fread(&l, sizeof(size_t), 1, fp);
			fread(&m, sizeof(size_t), 1, fp);

			ptbl->curln = l;
			d = stdiv(l, TBL_LN_BUF_SIZ);
			l = TBL_LN_BUF_SIZ * d.quot + (0 == d.rem ? 0 : TBL_LN_BUF_SIZ);

			strInitMatrix(&ptbl->tbldata, l, m, sizeof(P_CELL));
			ppc = (P_CELL *)strGetValueMatrix(NULL, &ptbl->tbldata, 0, 0, sizeof(P_CELL));

			for (i = 0; i < ptbl->curln * m; ++i)
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
				case CT_WSTRING:
					_siReadWString(wbuf, fp);
					pc = siCreateCell(wbuf, xc.ct);
					break;
				}
				*(ppc++) = pc;
				fseek(fp, oldl, SEEK_SET);
			}
			for (i = 0; i < ptbl->curln; ++i)
			{
				for (j = 0; j < ptbl->tbldata.col; ++j)
				{
					P_TBLHDR pt = strLocateItemArrayZ(&ptbl->header, sizeof(TBLHDR), j);
					P_CELL pc;
					strGetValueMatrix(&pc, &ptbl->tbldata, i, j, sizeof(P_CELL));
					switch (pt->cr)
					{
					case CR_UNIQUE:
					case CR_PRIMARY_KEY:
						pt->phsh = hshCreateC(BKSNUM);
						if (NULL != pc)
						{
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
						}
						break;
					}
				}
			}
		}
		return ptbl;
	}
	return NULL;
}
