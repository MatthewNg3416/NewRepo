/*
 * S/W Standardization for T7 Module Line Equipments
 * 
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc.("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license aggrement
 * you entered into with Samsung.
 */

/*
 * MGQue.cpp : This file implements the General Queue used in C/C++ 
 * environment.
 *
 * @ author : 
 * @ version :
 * @ compiler Used :
 * @ support tools : 
 * @ update history 
 *		- 04/03/02 : Major, Created
 * @ see also : 
*/

#include <StdAfx.h>
#include <windows.h>
#include "MGQue.h"
#include <memory.h>

static int _GQExpand(LPGQUE lpGQ)
{
	int nBufSize = (lpGQ->nBufSize-lpGQ->nElementSize) * 2 + lpGQ->nElementSize;
	char * pBuf = new char[ nBufSize ];
	
	if ( !pBuf ) 
		return FALSE;
	int n = lpGQ->iTail - lpGQ->iHead;
	int nTail = n;
	

	if ( nTail < 0 ) 
		nTail += lpGQ->nBufSize;
	
	if ( n >= 0 )
	{
		memcpy( pBuf, lpGQ->pBuf+lpGQ->iHead,  lpGQ->iTail - lpGQ->iHead + lpGQ->nElementSize );
	}
	else
	{
		int siz = lpGQ->nBufSize - lpGQ->iHead;
		memcpy( pBuf, lpGQ->pBuf+lpGQ->iHead, siz );
		memcpy( pBuf+siz, lpGQ->pBuf, lpGQ->iTail+lpGQ->nElementSize );
	}
	delete lpGQ->pBuf;
	lpGQ->pBuf = pBuf;
	lpGQ->iHead = 0;
	lpGQ->iTail = nTail;
	lpGQ->nBufSize = nBufSize;

	return TRUE;
}

int GQInit(LPGQUE lpGQ, int nQSize, int nElementSize, int bAutoExp)
{
	if ( nQSize <= 0 || nElementSize < 1 ) 
		return FALSE;
	
	lpGQ->nElementSize = nElementSize;
	lpGQ->nBufSize = (nQSize+1)*nElementSize;
	lpGQ->pBuf = new char[ lpGQ->nBufSize ];
	lpGQ->iHead = nElementSize;
	lpGQ->iTail = 0;
	lpGQ->bAutoExp = bAutoExp;

	InitializeCriticalSection(&lpGQ->Sync);

	
	if ( !lpGQ->pBuf ) 
		return FALSE;
	
	return TRUE;
}

void GQClose(LPGQUE lpGQ)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( lpGQ->pBuf )
	{
		delete [] lpGQ->pBuf;
		lpGQ->pBuf = 0;
	}
	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	

}

int GQPut(LPGQUE lpGQ, void *pBuf)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( GQIsFull(lpGQ) && (!lpGQ->bAutoExp || !_GQExpand(lpGQ)) )
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);
		return FALSE;
	}
	lpGQ->iTail = (lpGQ->iTail+lpGQ->nElementSize) % lpGQ->nBufSize;
	memcpy( lpGQ->pBuf+lpGQ->iTail, pBuf, lpGQ->nElementSize );
	
	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);
	return TRUE;
}


int GQPut(LPGQUE lpGQ, int pBuf)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( GQIsFull(lpGQ) && (!lpGQ->bAutoExp || !_GQExpand(lpGQ)) )
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);
		return FALSE;
	}
	lpGQ->iTail = (lpGQ->iTail+lpGQ->nElementSize) % lpGQ->nBufSize;
	lpGQ->pBuf[lpGQ->iTail] = pBuf;

	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);
	return TRUE;
}

int GQPush(LPGQUE lpGQ, void *pBuf)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( GQIsFull(lpGQ) && (!lpGQ->bAutoExp || !_GQExpand(lpGQ)) )
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);
		return FALSE;
	}
	lpGQ->iHead = (lpGQ->iHead - lpGQ->nElementSize + lpGQ->nBufSize)
			% lpGQ->nBufSize;
	memcpy( lpGQ->pBuf+lpGQ->iHead, pBuf, lpGQ->nElementSize );

	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	return TRUE;
}

int GQGet(LPGQUE lpGQ, void *pBuf)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( GQIsEmpty(lpGQ) ) 
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);
		return FALSE;
	}
	memcpy( pBuf, lpGQ->pBuf+lpGQ->iHead, lpGQ->nElementSize );
	lpGQ->iHead = (lpGQ->iHead + lpGQ->nElementSize ) % lpGQ->nBufSize;
	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	return TRUE;
}
int GQGet(LPGQUE lpGQ, int *pBuf)
{
	char buffer[250];
	memset(buffer, 0x00, sizeof(buffer));

	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( GQIsEmpty(lpGQ) ) 
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);
		return FALSE;
	}
	//memcpy( buffer, lpGQ->pBuf+lpGQ->iHead, lpGQ->nElementSize );
	memcpy( buffer, lpGQ->pBuf+lpGQ->iHead, 1 );
	lpGQ->iHead = (lpGQ->iHead + lpGQ->nElementSize ) % lpGQ->nBufSize;
	
	*pBuf = buffer[0];
	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	return TRUE;
}
int GQPop(LPGQUE lpGQ, void *pBuf)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( GQIsEmpty(lpGQ) ) 
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);
		return FALSE;
	}
	memcpy( pBuf, lpGQ->pBuf+lpGQ->iTail, lpGQ->nElementSize );
	lpGQ->iTail = (lpGQ->iTail-lpGQ->nElementSize) % lpGQ->nBufSize;

	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	return TRUE;
}

int GQPeek(LPCGQUE lpGQ, void *pBuf)
{
	
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( GQIsEmpty(lpGQ) ) 
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	
		return FALSE;
	}
	memcpy( pBuf, lpGQ->pBuf+lpGQ->iHead, lpGQ->nElementSize );

	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	

	return TRUE;
}

int GQPeekAt(LPCGQUE lpGQ, int idx, void *pBuf)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( idx < 0 ) 
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	
		return FALSE;
	}
	int ent = GQGetEntrySize( lpGQ );
	
	if ( idx >= ent ) 
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	
		return FALSE;
	}
	int offs = (lpGQ->iHead+lpGQ->nElementSize*idx) % lpGQ->nBufSize;
	memcpy( pBuf, lpGQ->pBuf+offs, lpGQ->nElementSize );

	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	

	return TRUE;
}

int GQPeekEnd(LPCGQUE lpGQ, void *pBuf)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	if ( GQIsEmpty(lpGQ) ) 
	{
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	
		return FALSE;
	}
	memcpy( pBuf, lpGQ->pBuf+lpGQ->iTail, lpGQ->nElementSize );

	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	

	return TRUE;
}

void GQClear(LPGQUE lpGQ)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	lpGQ->iHead = lpGQ->nElementSize;
	lpGQ->iTail = 0;

	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	

}

void GQSetAutoExpand(LPGQUE lpGQ, int bAutoExp)
{
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);

	lpGQ->bAutoExp = bAutoExp;

	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&lpGQ->Sync);	
}

int GQGetEntrySize(LPCGQUE lpGQ)
{
	if ( GQIsEmpty(lpGQ) ) 
		return 0;
	
	int nCount = ( ( lpGQ->iTail < lpGQ->iHead ) ?
		lpGQ->iTail+lpGQ->nBufSize : lpGQ->iTail )
		- lpGQ->iHead + lpGQ->nElementSize;
	
	return nCount / lpGQ->nElementSize;
}

int GQGetQueSize(LPCGQUE lpGQ)
{
	return (lpGQ->nBufSize / lpGQ->nElementSize) - 1;
}

int GQGetVacantSize(LPCGQUE lpGQ)
{

	return GQGetQueSize(lpGQ) - GQGetEntrySize(lpGQ);
}

int GQIsEmpty(LPCGQUE lpGQ)
{
	return (lpGQ->iTail + lpGQ->nElementSize) % lpGQ->nBufSize
		== lpGQ->iHead;
}

int GQIsFull(LPCGQUE lpGQ)
{
	return (lpGQ->iTail+lpGQ->nElementSize+lpGQ->nElementSize)
		% lpGQ->nBufSize == lpGQ->iHead;
}

int GQGetAutoExpand(LPCGQUE lpGQ)
{
	return lpGQ->bAutoExp;
}
