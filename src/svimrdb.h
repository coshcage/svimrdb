/*
 * Name:        svimrdb.h
 * Description: StoneValley in-memory relational database (aka SI).
 * Author:      cosh.cage#hotmail.com
 * File ID:     0628231947A0504240117L00215
 * License:     GPLv2.
 */
#ifndef _SVIMRDB_H_
#define _SVIMRDB_H_

/* Include StoneValley headers. */
#include "svstring.h"
#include "svtree.h"
#include "svset.h"
#include "svqueue.h"
#include "svhash.h"
#include <wchar.h>  /* Using function wcslen, wcscmp, wprintf, wcscmp. */
#include <pthread.h>

/* Cell type. */
typedef enum en_CellType
{
	CT_NULL,
	CT_CHAR,
	CT_SHORT,
	CT_INTEGER,
	CT_LONG,
	CT_FLOAT,
	CT_DOUBLE,
	CT_STRING,
	CT_WSTRING
} CellType;

/* A cell. */
typedef struct st_CELL
{
	CellType ct;  /* Cell data type. */
	void * pdata; /* Cell data pointer. */
} CELL, * P_CELL;

/* Column restriction. */
typedef enum en_ColRstct
{
	CR_NONE,
	CR_NOT_NULL,
	CR_UNIQUE,
	CR_PRIMARY_KEY = CR_NOT_NULL + CR_UNIQUE /* Not null unique. */
} ColRstct;

/* Table header element. */
typedef struct st_TBLHDR
{
	CellType   ct;      /* Data type. */
	char *     strname; /* Column name. */
	ColRstct   cr;      /* Restriction. */
	P_HSHTBL_C phsh;    /* Hash table for searching. */
} TBLHDR, * P_TBLHDR;

/* Selection function. */
typedef BOOL(*SICBF_SELECT)(P_CELL * pitem, size_t param);

/* Table augmentation function. */
typedef size_t(*SICBF_TBLAUG)(size_t curln, size_t ln);

/* Table structure. */
typedef struct st_TABLE
{
	char *  tblname; /* Table name. */
	ARRAY_Z header;  /* Table header. */
	size_t  curln;   /* Current line. */
	MATRIX  tbldata; /* Table data. */
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
	void * pobj; /* Locking object. */
	LockType lt; /* Lock type. */
} LOCK, * P_LOCK;

/* Alteration type. */
typedef enum en_AltType
{
	AT_ALTER_CELL = 1,
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
	AltType at;   /* Alteration type. */
	P_TABLE ptbl; /* Target table. */
	union un_DATA
	{
		struct st_DACELL
		{
			size_t ln;
			size_t col;
			P_CELL before;
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
	DEQUE_DL qoprlst; /* Operation list. */
	SET_T    setlock; /* Lock set. */
} TRANS, * P_TRANS;

/* Table reference. */
typedef struct st_TBLREF
{
	P_TABLE pold;
	P_TABLE pnew;
} TBLREF, * P_TBLREF;

/* Misc functions. */
char * siStrLCase(char * str);
size_t siHashChar(const void * pkey);
size_t siHashShort(const void * pkey);
size_t siHashInt(const void * pkey);
size_t siHashLong(const void * pkey);
size_t siHashFloat(const void * pkey);
size_t siHashDouble(const void * pkey);
size_t siHashString(const void * pkey);
size_t siHashWString(const void * pkey);
P_NODE_S hshSearchCPlusA(P_HSHTBL_C pht, CBF_HASH cbfhsh, const void * pkey, size_t size);
P_NODE_S hshSearchCPlusW(P_HSHTBL_C pht, CBF_HASH cbfhsh, const void * pkey, size_t size);
size_t siPlatformSize(void);
void siPrintSystemVersion(void);
size_t sicbftaDefaultIncrease(size_t curln, size_t ln);
size_t sicbftaDefaultDecrease(size_t curln, size_t ln);
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
void siDestroyView(P_MATRIX pmtx);
void siPrintView(P_MATRIX pmtx);
ptrdiff_t siGetColumnByString(P_TABLE ptbl, char * strname);
P_MATRIX siCreateViewOfTable(P_TABLE ptbl);
P_TABLE siCreateTable(P_TRANS ptrans, char * tblname, P_ARRAY_Z parrhdr);
P_TABLE siCopyTable(P_TRANS ptrans, P_TABLE ptbl);
void siDeleteTable(P_TRANS ptrans, P_TABLE ptbl);
BOOL siInsertIntoTableBase(P_TRANS ptrans, P_TABLE ptbl, SICBF_TBLAUG cbfta, P_ARRAY_Z parrarg);
BOOL siInsertIntoTable(P_TRANS ptrans, P_TABLE ptbl, SICBF_TBLAUG cbfta, ...);
BOOL siDeleteFromTable(P_TRANS ptrans, P_TABLE ptbl, SICBF_TBLAUG cbfta, size_t ln);
BOOL siUpdateTableCell(P_TRANS ptrans, P_TABLE ptbl, void * pval, size_t ln, size_t col);
BOOL siAddTableColumn(P_TRANS ptrans, P_TABLE ptbl, P_TBLHDR phdr);
BOOL siDropTableColumn(P_TRANS ptrans, P_TABLE ptbl, size_t col);
/* Functions for transactions. */
P_TRANS siBeginTransaction();
void siCommitTransaction(P_TRANS ptrans);
void siRollbackTransaction(P_ARRAY_Z * pparr, P_TRANS ptrans);
void siReleaseAllTransaction();
/* Lock functions. */
BOOL siTrylock(P_TRANS ptrans, void * pobj, LockType lt);
void siUnlock(P_TRANS ptrans, void * pobj, LockType lt);

#define BKSNUM         1021 /* 1021 is a prime. */
#define TBL_LN_BUF_SIZ 512

// #define strdup _strdup /* POSIX compliant for visual C compiler. */

/* The following macros are for single thread debugging. */
/*
#define pthread_mutex_lock(abc)
#define pthread_mutex_unlock(def)
#define pthread_mutex_t int
#define PTHREAD_MUTEX_INITIALIZER 0
*/

/* System version. */
#define SYS_VER "1.0.2"

#endif
