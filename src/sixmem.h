/*
 * Name:        sixmem.h
 * Description: SI external memory interface.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0714231153G0000000000L00000
 * License:     GPLv2.
 */
#ifndef _SIXMEM_H_
#define _SIXMEM_H_

#include "svimrdb.h"
#include <stdio.h>

typedef struct st_XCELL
{
	CellType ct;
	long     fpos;
} XCELL, * P_XCELL;

#endif

