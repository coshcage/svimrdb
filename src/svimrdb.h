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
	AT_ALTER_CELL,
	AT_ADD_TUPLE,
	AT_DEL_TUPLE,
	AT_ADD_COLUMN,
	AT_DEL_COLUMN,
	AT_ADD_TABLE,
	AT_DEL_TABLE
} AltType;

/* Data alteration. */
typedef struct st_DATALT
{
	AltType at;
	P_TABLE ptbl;
	union un_DATA
	{
		struct st_DACELL
		{
			size_t ln;
			size_t col;
			P_CELL before;
			P_CELL after;
		} dacell;
		struct st_DATPL
		{
			size_t sizln;
			ARRAY_Z tupledat;
		} datpl;
		struct st_DACOL
		{
			size_t sizcol;
			TBLHDR hdr;
			ARRAY_Z coldat;
		} dacol;
		P_TABLE datbl;
	} data;
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
P_MATRIX siCreateViewOfTable(P_TABLE ptbl);
P_TABLE siCreateTable(P_TRANS ptrans, char * tblname, P_ARRAY_Z parrhdr);
P_TABLE siCopyTable(P_TRANS ptrans, P_TABLE ptbl);
void siDeleteTable(P_TRANS ptrans, P_TABLE ptbl);
BOOL siInsertIntoTable(P_TRANS ptrans, P_TABLE ptbl, ...);
BOOL siDeleteFromTable(P_TRANS ptrans, P_TABLE ptbl, size_t col);
void siUpdateTableCell(P_TRANS ptrans, P_TABLE ptbl, void * pval, CellType ct, size_t ln, size_t col);
BOOL siAddTableColumn(P_TRANS ptrans, P_TABLE ptbl, P_TBLHDR phdr);
BOOL siDropTableColumn(P_TRANS ptrans, P_TABLE ptbl, size_t col);

#endif
