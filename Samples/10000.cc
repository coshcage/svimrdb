#include "svimrdb.h"
#include <stdio.h>

int main()
{
	int i, * p = &i;
	P_TRANS ptrans;
	P_TABLE ptbl;
	P_ARRAY_Z parrhdr;
	P_ARRAY_Z parrg;

	parrg = strCreateArrayZ(1, sizeof(void *));

	parrhdr = strCreateArrayZ(1, sizeof(TBLHDR));
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->cr = CR_PRIMARY_KEY;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->strname = "number";

	strInsertItemArrayZ(parrg, &p, sizeof(void *), 0);

	ptrans = siBeginTransaction();

	ptbl = siCreateTable(ptrans, "Student", parrhdr);

	while (TRUE != siTrylock(ptrans, ptbl, LT_S)) // Share lock.
		;

	while (TRUE != siTrylock(ptrans, ptbl, LT_X)) // Write lock.
		;

	for (i = 0; i < 10000; ++i)
	{
		if (TRUE != siInsertIntoTableBase(ptrans, ptbl, NULL, parrg))
		{
			printf("%s, %d\n", "failed!", i);
			siRollbackTransaction(NULL, ptrans);
			siReleaseAllTransaction();
			return 1;
		}
	}

	siCommitTransaction(ptrans);

	siReleaseAllTransaction();

	strDeleteArrayZ(parrg);

	return 0;
}
