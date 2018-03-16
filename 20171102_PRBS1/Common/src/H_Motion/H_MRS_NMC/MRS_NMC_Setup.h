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
	 * Constructor : initialize inner object, ���� ��ü���� �ʱ�ȭ�Ѵ�.
	 * @stereotype constructor
	 */	
	MRS_NMCSetup();

	/**
	 * Destructor : Delete inner object
	 *				���� ��ü�� �Ҹ��Ѵ�.
	 * @stereotype destructor
	 */
	~MRS_NMCSetup();

	/**
	 * Board�� ���¸� �ʱ�ȭ �Ѵ�. 
	 *
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	int BoardInit();
	int ECatRun();		//�̴�Ĺ��� ����.
	int ECatStop();		//�̴�Ĺ��� ����.
	int ECatReset();	//�̴�Ĺ ����� �����Ѵ�.

	BOOL IsMasterRun();			//�����Ͱ� Run �������� üũ��.
	int GetMasterMode();		//�������� ���� ���¸� �о���δ�.
	
	// Doolin kwangilshin 2017. 08. 27.
	//
	BOOL IsECatInitializing();
	BOOL IsSlaveOpMode(int nAxisIdx);
	// End.

	BOOL IsSlaveOpModeAll();	//��ü �����̺��� ���°� Op������� üũ.
	
	// ���ǵ��� ���� Error�� NMC Library���� �о�´�
	CString ReturnCurrentMotionError();

private:
	char chGetErrorMessage[128];

	BOOL	m_bOriginReturn;

    /**  Board �ʱ�ȭ ���� (TRUE:Board �ʱ�ȭ ����) */
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
