//
//  exp_2023-08-09_1.c
//  This file demonstrates usage of svimrdb.
//  Created by cosh.cage#hotmail.com on 08/09/23.
//  Licence:  GPLv2.
//  Platform: Unix|GNU/Linux
//
#include <stdio.h>
#include "svimrdb.h"

P_TABLE CreateTableStudent(P_TRANS ptrans)
{
	P_ARRAY_Z parrhdr;
	P_TABLE r = NULL;

	parrhdr = strCreateArrayZ(5, sizeof(TBLHDR));
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->cr = CR_PRIMARY_KEY;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->strname = "sno";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->ct = CT_STRING;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->strname = "sname";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->ct = CT_CHAR;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->strname = "ssex";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 3))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 3))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 3))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 3))->strname = "sage";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 4))->ct = CT_STRING;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 4))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 4))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 4))->strname = "sdept";

	r = siCreateTable(ptrans, "student", parrhdr);

	strDeleteArrayZ(parrhdr);

	return r;
}

void InsertIntoTableStudent(P_TRANS ptrans, P_TABLE ptbl)
{
	siInsertIntoTable(ptrans, ptbl, 2023001, "Lisa",  'F', 20, "CS");
	siInsertIntoTable(ptrans, ptbl, 2023002, "John",  'M', 19, "CS");
	siInsertIntoTable(ptrans, ptbl, 2023003, "Alice", 'F', 18, "MA");
	siInsertIntoTable(ptrans, ptbl, 2023004, "Jack",  'M', 19, "IS");
}

P_TABLE CreateTableCourse(P_TRANS ptrans)
{
	P_ARRAY_Z parrhdr;
	P_TABLE r = NULL;

	parrhdr = strCreateArrayZ(4, sizeof(TBLHDR));
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->cr = CR_PRIMARY_KEY;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->strname = "cno";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->ct = CT_STRING;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->strname = "cname";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->strname = "cpno";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 3))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 3))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 3))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 3))->strname = "ccredit";

	r = siCreateTable(ptrans, "course", parrhdr);

	strDeleteArrayZ(parrhdr);

	return r;
}

void InsertIntoTableCourse(P_TRANS ptrans, P_TABLE ptbl)
{
	siInsertIntoTable(ptrans, ptbl, 1, "Database",         5, 4);
	siInsertIntoTable(ptrans, ptbl, 2, "Math",             0, 2);
	siInsertIntoTable(ptrans, ptbl, 3, "Informatics",      1, 4);
	siInsertIntoTable(ptrans, ptbl, 4, "Operating System", 6, 3);
	siInsertIntoTable(ptrans, ptbl, 5, "Data Structure",   7, 4);
	siInsertIntoTable(ptrans, ptbl, 6, "Data Processing",  0, 2);
	siInsertIntoTable(ptrans, ptbl, 7, "PASCAL",           6, 4);
}

P_TABLE CreateTableSC(P_TRANS ptrans)
{
	P_ARRAY_Z parrhdr;
	P_TABLE r = NULL;

	parrhdr = strCreateArrayZ(3, sizeof(TBLHDR));
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 0))->strname = "sno";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->ct = CT_INTEGER;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 1))->strname = "cno";

	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->ct = CT_FLOAT;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->phsh = NULL;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->cr = CR_NONE;
	((TBLHDR *)strLocateItemArrayZ(parrhdr, sizeof(TBLHDR), 2))->strname = "grade";

	r = siCreateTable(ptrans, "sc", parrhdr);

	strDeleteArrayZ(parrhdr);

	return r;
}

void InsertIntoTableSC(P_TRANS ptrans, P_TABLE ptbl)
{
	float f;
	f = 92.0f;
	siInsertIntoTable(ptrans, ptbl, 2023001, 1, &f);
	f = 85.0f;
	siInsertIntoTable(ptrans, ptbl, 2023001, 2, &f);
	f = 88.0f;
	siInsertIntoTable(ptrans, ptbl, 2023001, 3, &f);
	f = 90.0f;
	siInsertIntoTable(ptrans, ptbl, 2023002, 2, &f);
	f = 80.0f;
	siInsertIntoTable(ptrans, ptbl, 2023002, 3, &f);
}

int main()
{
	P_ARRAY_Z parr;
	P_MATRIX pv, pv2;
	P_TRANS ptrans;
	P_TABLE ptbl_student, ptbl_course, ptbl_sc;

	ptrans = siBeginTransaction();


	ptbl_student = CreateTableStudent(ptrans);

	while (TRUE != siTrylock(ptrans, ptbl_student, LT_S)) // Share lock.
		;
	while (TRUE != siTrylock(ptrans, ptbl_student, LT_X)) // Write lock.
		;
	InsertIntoTableStudent(ptrans, ptbl_student);


	ptbl_course = CreateTableCourse(ptrans);

	while (TRUE != siTrylock(ptrans, ptbl_course, LT_S)) // Share lock.
		;
	while (TRUE != siTrylock(ptrans, ptbl_course, LT_X)) // Write lock.
		;
	InsertIntoTableCourse(ptrans, ptbl_course);


	ptbl_sc = CreateTableSC(ptrans);

	while (TRUE != siTrylock(ptrans, ptbl_sc, LT_S)) // Share lock.
		;
	while (TRUE != siTrylock(ptrans, ptbl_sc, LT_X)) // Write lock.
		;
	InsertIntoTableSC(ptrans, ptbl_sc);

	// select * from student
	pv = siCreateViewOfTable(ptbl_student);
	printf("select * from student:\n%s\t%s\t%s\t%s\t%s\n", "sno", "sname", "ssex", "sage", "sdept"); // Print table header.
	siPrintView(pv);
	printf("\n");
	strDeleteMatrix(pv);

	// select * from course
	pv = siCreateViewOfTable(ptbl_course);
	printf("select * from course:\n%s\t%s\t%s\t%s\n", "cno", "cname", "cpno", "ccredit"); // Print table header.
	siPrintView(pv);
	printf("\n");
	strDeleteMatrix(pv);

	// select * from sc
	pv = siCreateViewOfTable(ptbl_sc);
	printf("select * from sc:\n%s\t%s\t%s\n", "sno", "cno", "grade"); // Print table header.
	siPrintView(pv);
	printf("\n");
	strDeleteMatrix(pv);

	/* _______
	 *  |  |
	 *  |  |
	 * /    \sdept(student)
	 * select sdept from student
	 */
	pv = siCreateViewOfTable(ptbl_student);
	parr = strCreateArrayZ(1, sizeof(size_t));
	0[(size_t *)strLocateItemArrayZ(parr, sizeof(size_t), 0)] = 4;
	pv2 = siCreateProjectView(pv, parr);
	printf("%s\n", "select sdept from student:"); // Print table header.
	siPrintView(pv2);
	strDeleteMatrix(pv);
	strDeleteMatrix(pv2);

	
	siDeleteTable(ptrans, ptbl_student);
	siDeleteTable(ptrans, ptbl_course);
	siDeleteTable(ptrans, ptbl_sc);
	

	siCommitTransaction(ptrans);

	siReleaseAllTransaction();

	return 0;
}
