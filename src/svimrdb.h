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
#include "StoneValley/src/svqueue.h"

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

/* Lock type. */
typedef enum en_LockType
{
	LT_S = 0,
	LT_X,
	LT_IS,
	LT_IX,
	LT_SIX,
	LT_NOLOCK
} LockType;

/* Lock structure. */
typedef struct st_LOCK
{
	void * pobj;
	LockType lt;
} LOCK, * P_LOCK;

/* Alteration type. */
typedef enum en_AltType
{
	AT_NONE = 0,
	AT_CELL,
	AT_TUPLE,
	AT_COLUMN,
	AT_TABLE
} AltType;

/* Data alteration. */
typedef union un_DATALT
{
	struct st_DACELL
	{
		P_CELL brfore;
		P_CELL after;
	} dacell;
	ARRAY_Z datuple;
	struct st_DACOL
	{
		TBLHDR hdr;
		ARRAY_Z coldat;
	} dacol;
	struct st_DATBL
	{
		TABLE before;
		TABLE after;
	} datable;
} DATALT, * P_DATALT;

/* Transactions. */
typedef struct st_TRANS
{
	DEQUE_DL qoprlst;
	SET_T setlock;
} TRANS, * P_TRANS;

/* Misc functions. */
char * siStrLCase(char * str);
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
void siSortView(P_MATRIX pmtx, size_t col, BOOL ascd);
P_MATRIX siInstantiateView(P_MATRIX pmtx);
void siDestoryView(P_MATRIX pmtx);
void siPrintView(P_MATRIX pmtx);
P_TABLE siCreateTable(char * tblname, P_ARRAY_Z parrhdr);
void siDeleteTable(P_TABLE ptbl);
BOOL siInsertIntoTable(P_TABLE ptbl, ...);
BOOL siDeleteFromTable(P_TABLE ptbl, size_t col);
void siUpdateTableCell(P_TABLE ptbl, void * pval, CellType ct, size_t ln, size_t col);
BOOL siAddTableColumn(P_TABLE ptbl, P_TBLHDR phdr);
BOOL siDropTableColumn(P_TABLE ptbl, size_t col);

#endif
