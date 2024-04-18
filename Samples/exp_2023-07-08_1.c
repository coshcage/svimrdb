//
//  exp_2023-07-08_1.c
//  This file demonstrates usage of svimrdb.
//  Created by cosh.cage#hotmail.com on 07/08/23.
//  Licence:  GPLv2.
//  Platform: Unix|GNU/Linux
//
#define _CRT_SECURE_NO_WARNINGS 1
#include "svimrdb.h"
#include "sixmem.h"

void PrintMatrix(P_MATRIX pmtx)
{
	size_t i, j;
	for (i = 0; i < pmtx->ln; ++i)
	{
		for (j = 0; j < pmtx->col; ++j)
			printf("%p ", *(P_CELL *)strGetValueMatrix(NULL, pmtx, i, j, sizeof(P_CELL)));
		printf("\n");
	}
}

int main()
{
	P_MATRIX pv;
	P_TRANS ptrans;
	P_TABLE ptbl;
	P_ARRAY_Z parrhdr, pa = NULL;
	FILE * fp;

	TBLHDR tbh;

	tbh.cr = CR_NONE;
	tbh.phsh = NULL;
	tbh.ct = CT_INTEGER;
	tbh.strname = "degree";

	parrhdr = strCreateArrayZ(3, sizeof(TBLHDR));
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->cr = CR_PRIMARY_KEY;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->strname = "number";
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->ct = CT_STRING;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->strname = "name";
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->ct = CT_WSTRING;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->strname = "Department";

	ptrans = siBeginTransaction();

	ptbl = siCreateTable(ptrans, "Student", parrhdr);

	while (TRUE != siTrylock(ptrans, ptbl, LT_S)) // Share lock.
		;

	while (TRUE != siTrylock(ptrans, ptbl, LT_X)) // Write lock.
		;

	siInsertIntoTable(ptrans, ptbl, NULL, 2, "Lisa", L"CS");
	siInsertIntoTable(ptrans, ptbl, NULL, 2, "Lisa", L"CS");
	siInsertIntoTable(ptrans, ptbl, NULL, 1, "John", L"LT");
	siInsertIntoTable(ptrans, ptbl, NULL, 4, "Amy", L"CS");
	siInsertIntoTable(ptrans, ptbl, NULL, 3, "Jack", L"LT");

	

	fp = fopen("test.db", "wb");

	if (NULL != fp)
	{
		siSaveTable(fp, 0, ptbl);
		fclose(fp);
	}

	fp = fopen("test.db", "rb");

	if (NULL != fp)
	{
		ptbl = siLoadTable(fp, 0);

		pv = siCreateViewOfTable(ptbl);

		siSortView(pv, 1, TRUE);

		siPrintView(pv);

		strDeleteMatrix(pv);

		fclose(fp);
	}
	

	siDeleteFromTable(ptrans, ptbl, NULL, 0);

	siAddTableColumn(ptrans, ptbl, &tbh);

	siDropTableColumn(ptrans, ptbl, 0);

	siUpdateTableCell(ptrans, ptbl, "Name", 0, 0);

	pv = siCreateViewOfTable(ptbl);

	//PrintMatrix(pv);

	siPrintView(pv);

	strDeleteMatrix(pv);

	siDeleteTable(ptrans, ptbl);

	siRollbackTransaction(&pa, ptrans);

	strDeleteArrayZ(parrhdr);

	if (NULL != pa)
		strDeleteArrayZ(pa);

	siReleaseAllTransaction();

	return 0;
}
