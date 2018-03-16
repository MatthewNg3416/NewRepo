#ifndef MRS_NMCSETUP_H
#define MRS_NMCSETUP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NMC_Motion.h"

#include <afxmt.h>

#define DEF_NMC_MASTER_BOARD_ID 0

class MRS_NMCSetup
{
public:

	/**
	 * Constructor : initialize inner object, 내부 개체들을 초기화한다.
	 * @stereotype constructor
	 */	
	MRS_NMCSetup();

	/**
	 * Destructor : Delete inner object
	 *				내부 개체를 소멸한다.
	 * @stereotype destructor
	 */
	~MRS_NMCSetup();

	/**
	 * Board의 상태를 초기화 한다. 
	 *
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	int BoardInit();
	int ECatRun();		//이더캣통신 시작.
	int ECatStop();		//이더캣통신 정지.
	int ECatReset();	//이더캣 통신을 리셋한다.

	BOOL IsMasterRun();			//마스터가 Run 상태인지 체크함.
	int GetMasterMode();		//마스터의 현재 상태를 읽어들인다.
	
	// Doolin kwangilshin 2017. 08. 27.
	//
	BOOL IsECatInitializing();
	BOOL IsSlaveOpMode(int nAxisIdx);
	// End.

	BOOL IsSlaveOpModeAll();	//전체 슬레이브의 상태가 Op모드인지 체크.
	
	// 정의되지 않은 Error를 NMC Library에서 읽어온다
	CString ReturnCurrentMotionError();

private:
	char chGetErrorMessage[128];

	BOOL	m_bOriginReturn;

    /**  Board 초기화 여부 (TRUE:Board 초기화 했음) */
	BOOL	m_bInitialized;

	// Doolin kwangilshin 2017. 08. 27.
	//
	BOOL				m_bIsInitializing;
	CCriticalSection	m_csLock;
	// End.

	int GetBoardID(int iAxisID);
	int GetAxisID(int iAxisID);

	int	MakeRS_NMC_Error(MC_STATUS mcStatus);
};

#endif //
