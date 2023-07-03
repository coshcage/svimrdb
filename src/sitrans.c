/*
 * Name:        sitrans.c
 * Description: Transaction control.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0702231427D0000000000L00000
 * License:     GPLv2.
 */

#include <stdio.h>  /* Using function printf. */
#include <stdlib.h> /* Using function malloc, free. */
#include <string.h> /* Using function strdup. */
#include <stdarg.h>
#include "svimrdb.h"
#include "StoneValley/src/svqueue.h"

typedef enum en_OPType
{
	OT_NONE = 0,

} OPType;

typedef struct st_OP
{
	OPType type;
	P_TABLE ptbl;
	union un_OPDATA
	{
		P_CELL prev;
		P_CELL after;
	} opdata;
} OP, * P_OP;

DEQUE_DL qOperationList = { 0 };

void siBeginTransaction()
{

}

void siCommitTransaction()
{

}

void siRollbackTransaction()
{

}
