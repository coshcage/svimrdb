/*
 * Name:        svimrdb.h
 * Description: StoneValley in-memory relational database (aka SI).
 * Author:      cosh.cage#hotmail.com
 * File ID:     0628231947A0000000000L00063
 * License:     GPLv2.
 */
#ifndef _SVIMRDB_H_
#define _SVIMRDB_H_

#include "StoneValley/src/svstring.h"
#include "StoneValley/src/svtree.h"
#include "StoneValley/src/svset.h"

/* Cell type. */
typedef enum en_CellType
{
	CT_NONE,
	CT_CHAR,
	CT_SHORT,
	CT_INTEGER,
	CT_LONG,
	CT_FLOAT,
	CT_DOUBLE,
	CT_STRING
} CellType;

/* A cell. */
typedef struct st_CELL
{
	CellType ct;
	void * pdata;
} CELL, * P_CELL;

/* Table header element. */
typedef struct st_TBLHDR
{
	CellType ct;
	char * strname;
} TBLHDR, * P_TBLHDR;

/* Selection function. */
typedef BOOL(*SICBF_SELECT)(P_CELL * pitem, size_t param);

/* Table structure. */
typedef struct st_TABLE
{
	char * tblname;
	ARRAY_Z header;
	MATRIX tbldata;
} TABLE, * P_TABLE;

/* Misc functions. */
char * strlowercase(char * str);
/* Relational algebraic functions. */
P_MATRIX siCreateUniqueView(P_MATRIX pmtx);
P_MATRIX siCreateUnionView(P_MATRIX pmtxa, P_MATRIX pmtxb);
P_MATRIX siCreateIntersectionView(P_MATRIX pmtxa, P_MATRIX pmtxb);
P_MATRIX siCreateDifferenceView(P_MATRIX pmtxa, P_MATRIX pmtxb);
P_MATRIX siCreateCartesianProductView(P_MATRIX pma, P_MATRIX pmb);
P_MATRIX siCreateSelectView(P_ARRAY_Z * pparr, P_MATRIX pmtx, SICBF_SELECT cbfsel, size_t param);
P_MATRIX siCreateProjectView(P_MATRIX pmtx, P_ARRAY_Z parrz);
/* Cell operation functions. */
P_CELL siCreateCell(void * pitem, CellType ct);
void siDeleteCell(P_CELL * ppcell);
/* Table and view functions. */
void siSortView(P_MATRIX pmtx, size_t col);
P_MATRIX siInstantiateView(P_MATRIX pmtx);
void siDestoryView(P_MATRIX pmtx);
void siPrintView(P_MATRIX pmtx);
P_TABLE siCreateTable(char * tblname, P_ARRAY_Z parrhdr);
void siDeleteTable(P_TABLE ptbl);
BOOL siInsertIntoTable(P_TABLE ptbl, ...);
BOOL siDeleteFromTable(P_TABLE ptbl, size_t col);


#endif
