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
 * MGQue.h : This file defines the General Queue used in C/C++ 
 * environment.
 *
 * @ author : 
 * @ version :
 * @ compiler Used :
 * @ support tools : 
 * @ update History 
 *		- 04/03/02 : Major, Created
 * @ see also 
*/


#ifndef __MGQue_H
#define __MGQue_H

#ifndef	TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

typedef struct tagGQUE
{
	CRITICAL_SECTION	Sync;
	int		iHead;
	int		iTail;
	int		nElementSize;
	int		nBufSize;
	char	*pBuf;
	int		bAutoExp;			// flag: auto expand the buffer
} GQUE, *LPGQUE;

typedef const GQUE*	LPCGQUE;

int	GQInit(LPGQUE lpGQ, int nQSize, int nElementSize, int bAutoExp=TRUE);
void GQClose(LPGQUE lpGQ);
int GQPut(LPGQUE lpGQ, void *pBuf);
int GQPut(LPGQUE lpGQ, int pBuf);

int GQPush(LPGQUE lpGQ, void *pBuf);
int GQGet(LPGQUE lpGQ, void *pBuf);
int GQGet(LPGQUE lpGQ, int *pBuf);
int GQPop(LPGQUE lpGQ, void *pBuf);
int GQPeek(LPCGQUE lpGQ, void *pBuf);
int GQPeekAt(LPCGQUE lpGQ, int idx, void *pBuf);
int GQPeekEnd(LPCGQUE lpGQ, void *pBuf);
void GQClear(LPGQUE lpGQ);
void GQSetAutoExpand(LPGQUE lpGQ, int bAutoExp);

int GQGetEntrySize(LPCGQUE lpGQ);
int GQGetQueSize(LPCGQUE lpGQ);
int GQGetVacantSize(LPCGQUE lpGQ);
int GQIsEmpty(LPCGQUE lpGQ);
int GQIsFull(LPCGQUE lpGQ);
int GQGetAutoExpand(LPCGQUE lpGQ);


#ifdef __cplusplus
// define C++ template wrapper class


/**
 * FIFO Queue의 기능을 구현한 Class이다.
 * Template Library를 사용하였기 때문 Queue에 저장할 Item의 타입을 지정할 수가 있다. 
 */
template<class T>
class MGQue : protected GQUE {
public:
	/** 생성자
	  * @param nQSize : Queue의 사이즈, Default값 16
	  * @param bAutoExp : 자동확장 여부, Default값 TRUE */	  
	MGQue(int nQSize=16, int bAutoExp=TRUE)
	{ 
		GQInit(this, nQSize, sizeof(T), bAutoExp ); 
	}

	/** 소멸자 */
	~MGQue() { GQClose(this); }

	/** Queue의 끝에다 삽입 한다.
	  * @param pBuf : 작업할 Element의 포인터 */
	int Put(const T *pBuf) { return GQPut(this, (void *)pBuf); }

	int PutTemp( int pBuf){	return GQPut(this, pBuf); }

	/** Queue의 처음에다 삽입한다.
	  * @param pBuf : 작업할 Element의 포인터 */  

	int Push(const T *pBuf) { return GQPush(this, (void *)pBuf); }

	/** 꺼내지 않고 읽어 보기만 한다.
	  * @param pBuf : 작업할 Element의 포인터 */
	int Peek(T *pBuf) const { return GQPeek(this, (void *)pBuf); }

	/** Index를 지정 하여 peek 한다.
	  * @param pBuf : 작업할 Element의 포인터 */
	int PeekAt(int idx, T *pBuf) const { return GQPeekAt(this, idx, (void *)pBuf); }

	/** Queue의 제일 끝 Element를 peek 한다.
	  * @param pBuf : 작업할 Element의 포인터 */
	int PeekEnd(T *pBuf) const { return GQPeekEnd(this, (void *)pBuf); }

	/** Queue의 맨 처음 Element를 끄집어 낸다.
	  * @param pBuf : 작업할 Element의 포인터 */
	int Get(T *pBuf) { return GQGet(this, (void *)pBuf); }

	int GetTemp(int *pBuf) { return GQGet(this, (int *)pBuf); }

	/** Queue의 제일 마직 Element를 끄집어 낸다.
	  * @param pBuf : 작업할 Element의 포인터 */
	int Pop(const T *pBuf) { return GQPop(this, (void *)pBuf); }

	/** Queue를 Clear한다 */
	void Clear() { GQClear(this); }

	/** 자동 확장 여부를 설정 한다
	  * @param bAutoExp : TRUE - 자동확장, FLASE - 자동 확장 안함 */
	void SetAutoExpand(int bAutoExp) { GQSetAutoExpand(this, bAutoExp); }

	//--- accessors	
	int GetQueSize() const { return GQGetQueSize(this); }
	int GetEntrySize() const { return GQGetEntrySize(this); }
	int GetVacantSize() const { return GQGetVacantSize(this); }
	int IsEmpty() const { return GQIsEmpty(this); }
	int IsFull() const { return GQIsFull(this); }
	int GetAutoExpand() const { return GQGetAutoExpand(this); }

protected:
	void Close() { GQClose(this); }
};

#endif

#endif
