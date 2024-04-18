//
//  exp_2023-08-09_1.c
//  This file demonstrates usage of svimrdb.
//  Created by cosh.cage#hotmail.com on 01/30/24.
//  Licence:  GPLv2.
//  Platform: Unix|GNU/Linux
//
#include <unistd.h>
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
	siInsertIntoTable(ptrans, ptbl, NULL, 2023001, "Lisa",  'F', 20, "CS");
	siInsertIntoTable(ptrans, ptbl, NULL, 2023002, "John",  'M', 19, "CS");
	siInsertIntoTable(ptrans, ptbl, NULL, 2023003, "Alice", 'F', 18, "MA");
	siInsertIntoTable(ptrans, ptbl, NULL, 2023004, "Jack",  'M', 19, "IS");
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
	siInsertIntoTable(ptrans, ptbl, NULL, 1, "Database",         5, 4);
	siInsertIntoTable(ptrans, ptbl, NULL, 2, "Math",             0, 2);
	siInsertIntoTable(ptrans, ptbl, NULL, 3, "Informatics",      1, 4);
	siInsertIntoTable(ptrans, ptbl, NULL, 4, "Operating System", 6, 3);
	siInsertIntoTable(ptrans, ptbl, NULL, 5, "Data Structure",   7, 4);
	siInsertIntoTable(ptrans, ptbl, NULL, 6, "Data Processing",  0, 2);
	siInsertIntoTable(ptrans, ptbl, NULL, 7, "PASCAL",           6, 4);
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
	siInsertIntoTable(ptrans, ptbl, NULL, 2023001, 1, &f);
	f = 85.0f;
	siInsertIntoTable(ptrans, ptbl, NULL, 2023001, 2, &f);
	f = 88.0f;
	siInsertIntoTable(ptrans, ptbl, NULL, 2023001, 3, &f);
	f = 90.0f;
	siInsertIntoTable(ptrans, ptbl, NULL, 2023002, 2, &f);
	f = 80.0f;
	siInsertIntoTable(ptrans, ptbl, NULL, 2023002, 3, &f);
}

P_TABLE ptbl_student, ptbl_course, ptbl_sc;

void * foo(void * ptr)
{
	P_TRANS ptrans;

	ptrans = siBeginTransaction();


	ptbl_student = CreateTableStudent(ptrans);

	while (TRUE != siTrylock(ptrans, &ptbl_student, LT_S)) // Share lock.
	{
		printf("SLEEP 1\n");
		(void)sleep(1);
	}
	while (TRUE != siTrylock(ptrans, &ptbl_student, LT_X)) // Write lock.
	{
		printf("SLEEP 2\n");
		(void)sleep(1);
	}
	InsertIntoTableStudent(ptrans, ptbl_student);


	ptbl_course = CreateTableCourse(ptrans);

	while (TRUE != siTrylock(ptrans, &ptbl_course, LT_S)) // Share lock.
	{
		printf("SLEEP 3\n");
		(void)sleep(1);
	}
	while (TRUE != siTrylock(ptrans, &ptbl_course, LT_X)) // Write lock.
	{

		printf("SLEEP 4\n");
		(void)sleep(1);
	}
	InsertIntoTableCourse(ptrans, ptbl_course);


	ptbl_sc = CreateTableSC(ptrans);

	while (TRUE != siTrylock(ptrans, &ptbl_sc, LT_S)) // Share lock.
	{
		printf("SLEEP 5\n");
		(void)sleep(1);
	}
	while (TRUE != siTrylock(ptrans, &ptbl_sc, LT_X)) // Write lock.
	{
		printf("SLEEP 6\n");
		(void)sleep(1);
	}
	InsertIntoTableSC(ptrans, ptbl_sc);

	siCommitTransaction(ptrans);
}

void * bar(void * ptr)
{
	P_ARRAY_Z parr;
	P_MATRIX pv, pv2;
	P_TRANS ptrans;
	ptrans = siBeginTransaction();

	while (TRUE != siTrylock(ptrans, &ptbl_student, LT_S)) // Share lock.
	{
		printf("SLEEP 7\n");
		(void)sleep(1);
	}
	// select * from student
	pv = siCreateViewOfTable(ptbl_student);

	siUnlock(ptrans, &ptbl_student, LT_S);

	printf("select * from student:\n%s\t%s\t%s\t%s\t%s\n", "sno", "sname", "ssex", "sage", "sdept"); // Print table header.
	siPrintView(pv);
	printf("\n");
	strDeleteMatrix(pv);


	while (TRUE != siTrylock(ptrans, &ptbl_course, LT_S)) // Share lock.
	{

		printf("SLEEP 8\n");
		(void)sleep(1);
	}
	// select * from course
	pv = siCreateViewOfTable(ptbl_course);

	siUnlock(ptrans, &ptbl_course, LT_S);

	printf("select * from course:\n%s\t%s\t%s\t%s\n", "cno", "cname", "cpno", "ccredit"); // Print table header.
	siPrintView(pv);
	printf("\n");
	strDeleteMatrix(pv);


	while (TRUE != siTrylock(ptrans, &ptbl_sc, LT_S)) // Share lock.
	{
		printf("SLEEP 9\n");
		(void)sleep(1);
	}
	// select * from sc
	pv = siCreateViewOfTable(ptbl_sc);

	siUnlock(ptrans, &ptbl_sc, LT_S);

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
	while (TRUE != siTrylock(ptrans, &ptbl_student, LT_S)) // Share lock.
	{
		printf("SLEEP 10\n");
		(void)sleep(1);
	}
	pv = siCreateViewOfTable(ptbl_student);

	siUnlock(ptrans, &ptbl_student, LT_S);

	parr = strCreateArrayZ(1, sizeof(size_t));
	0[(size_t *)strLocateItemArrayZ(parr, sizeof(size_t), 0)] = 4;
	pv2 = siCreateProjectView(pv, parr);
	printf("%s\n", "select sdept from student:"); // Print table header.
	siPrintView(pv2);
	strDeleteMatrix(pv);
	strDeleteMatrix(pv2);
	strDeleteArrayZ(parr);
	

	siCommitTransaction(ptrans);
}

int main()
{
	pthread_t pt1, pt2;

	int r1, r2;

	r1 = pthread_create(&pt1, NULL, foo, (void *)NULL);
	r2 = pthread_create(&pt2, NULL, bar, (void *)NULL);

	pthread_join(pt1, NULL);
	pthread_join(pt2, NULL);

	printf("Thread 1 returns: %d\n", r1);
	printf("Thread 2 returns: %d\n", r2);

	siReleaseAllTransaction();

	return 0;
}
