//
//  exp_2023-07-08_1.c
//  This file demonstrates usage of svimrdb.
//  Created by cosh.cage#hotmail.com on 07/08/23.
//  Licence:  GPLv2.
//  Platform: Unix|GNU/Linux
//
#include "svimrdb.h"

int main()
{
	P_MATRIX pv;
	P_TRANS ptrans;
	P_TABLE ptbl;

	P_ARRAY_Z parrhdr;

	parrhdr = strCreateArrayZ(3, sizeof(TBLHDR));
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->strname = "number";
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->ct = CT_STRING;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->strname = "name";
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->ct = CT_STRING;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->strname = "Department";

	ptrans = siBeginTransaction();

	ptbl = siCreateTable(ptrans, "Student", parrhdr);

	while (TRUE != siTrylock(ptrans, ptbl, LT_S)) // Share lock.
		;

	while (TRUE != siTrylock(ptrans, ptbl, LT_X)) // Write lock.
		;


	siInsertIntoTable(ptrans, ptbl, 2, "Lisa", "CS");
	siInsertIntoTable(ptrans, ptbl, 1, "John", "LT");
	siInsertIntoTable(ptrans, ptbl, 4, "Amy",  "CS");
	siInsertIntoTable(ptrans, ptbl, 3, "Jack", "LT");

	siDeleteFromTable(ptrans, ptbl, 3);

	siDropTableColumn(ptrans, ptbl, 1);

	pv = siCreateViewOfTable(ptbl);

	siPrintView(pv);

	siDeleteTable(ptrans, ptbl);

	siUnlock(ptrans, ptbl, LT_X);
	siUnlock(ptrans, ptbl, LT_S);

	siCommitTransaction(ptrans);
	
	//siRollbackTransaction(&parrtblref, ptrans);

	strDeleteMatrix(pv);
	strDeleteArrayZ(parrhdr);
	siReleaseAllTransaction();

	return 0;
}
