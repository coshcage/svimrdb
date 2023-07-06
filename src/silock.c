/*
 * Name:        silock.c
 * Description: Lock functions.
 * Author:      cosh.cage#hotmail.com
 * File ID:     0704231516F0000000000L00000
 * License:     GPLv2.
 */

#include "svimrdb.h"

#define Y TRUE
#define N FALSE

BOOL const bCompatibleMatrix[][6] =
{
	{Y,N,Y,N,N,Y},
	{N,N,N,N,N,Y},
	{Y,N,Y,Y,Y,Y},
	{N,N,Y,Y,N,Y},
	{N,N,Y,N,N,Y},
	{Y,Y,Y,Y,Y,Y}
};

void siTrylock(P_TRANS ptrans, void * pobj, LockType lt)
{

}

void siUnlock(P_TRANS ptrans, void * pobj, LockType lt)
{

}
