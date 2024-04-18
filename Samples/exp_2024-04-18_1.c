//
//  exp_2024-04-18_1.c
//  This file demonstrates usage of svimrdb and svregex.
//  Created by cosh.cage#hotmail.com on 04/18/24.
//  Licence:  GPLv2.
//  Platform: Unix|GNU/Linux
//
#include "svimrdb.h"
#include "svregex.h"
#include <string.h>
#include <stdio.h>

BOOL cbfsel(P_CELL * pitem, size_t param)
{
	size_t i, j = 1, l;

	for (i = 0; '\0' != i[(char *)(*(pitem + 1))->pdata]; ++i)
	{
		j = NextState((P_DFA)param, j, i[(char *)(*(pitem + 1))->pdata]);
		strGetValueMatrix(&l, (P_DFA)param, j, 0, sizeof(size_t));
		if (l & SIGN)
		{
			return TRUE;
		}
	}

	return FALSE;
}

int main()
{
	size_t i;
	P_TRANS ptrans;
	P_TABLE ptbl;
	P_ARRAY_Z parrhdr;
	P_MATRIX pm, pn;
	P_DFA dfa;

	parrhdr = strCreateArrayZ(2, sizeof(TBLHDR));

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->cr = CR_PRIMARY_KEY;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->strname = "number";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->ct = CT_STRING;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->strname = "name";

	ptrans = siBeginTransaction();

	ptbl = siCreateTable(ptrans, "Student", parrhdr);

	while (TRUE != siTrylock(ptrans, ptbl, LT_S)) // Share lock.
		;

	while (TRUE != siTrylock(ptrans, ptbl, LT_X)) // Write lock.
		;

	dfa = CompileRegex2DFA(L"(A|B)(A|B)*");

	siInsertIntoTable(ptrans, ptbl, NULL, 1, "AAAA");
	siInsertIntoTable(ptrans, ptbl, NULL, 2, "CCCC");
	siInsertIntoTable(ptrans, ptbl, NULL, 3, "BBBB");
	siInsertIntoTable(ptrans, ptbl, NULL, 4, "DDDD");

	pn = siCreateViewOfTable(ptbl);
	
	pm = siCreateSelectView(NULL, pn, cbfsel, (size_t)dfa);

	DestroyDFA(dfa);

	siPrintView(pm);

	strDeleteMatrix(pm);

	strDeleteMatrix(pn);

	siCommitTransaction(ptrans);

	siReleaseAllTransaction();

	strDeleteArrayZ(parrhdr);

	return 0;
}
