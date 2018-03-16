/* 
 * Axis Component
 *
 * Copyright 2006 by Mechatronics & Manufacturing Technology Center, 
 * Samsung Electronics, Inc., All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#include "stdafx.h"
#include "math.h"
#include "IIO.h"
#include "MRS_NMC_Setup.h"
#include "MRSNMCAxis.h"
#include "DefMotionValue.h"
#include "common.h"
#include "DefAxisPreBonder.h"
#include "IMotionLibNMC.h"
#include "MRS_NMCLib.h"

// Doolin kwangilshin 2017. 08. 27.
//
#include "MRS_NMC_Setup.h"
// End.

#include "MPlatformOlbSystem.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**  
 * MRSNMC.cpp  : This file defines the class of single-axis and multi-axis.
 *				     �� ������ ���� ������ ���� Class�� �����Ѵ�.
 *
 * @version $Revision$
 */
extern MPlatformOlbSystem	MOlbSystemPre;

MRSNMC::MRSNMC()
{
	/** 1�� ���� member variable �ʱ�ȭ */
	m_SaxAxis.iAxisID			= 0;
	memset(m_SaxAxis.szName, 0, sizeof(m_SaxAxis.szName));
	m_SaxAxis.iCPhaseUseType	= DEF_USE_SWITCH_ONLY;
	m_SaxAxis.bOriginDir		= FALSE;
	m_SaxAxis.bOriginFineDir	= FALSE;
	m_SaxAxis.dScale			= 1.0;
	m_SaxAxis.iOriginPriority	= 0;
	m_SaxAxis.bAbsEncoder		= FALSE;

	/** SERCOS ���� */
	m_SaxAxis.iRingNo			= 0;
	m_SaxAxis.iNodeNo			= 0;
	m_SaxAxis.iServoType		= 0;

	/** IO Component */
	m_plnkIO = NULL;

	/** Interlock IO Address */
	m_usiInterlockIOAddr		= 0;

	/** Interlock IO Check Value */
	m_bInterlockJudge			= 0;

	/** Timer ��ü ���� */
	m_bOriginFlag				= FALSE;
	m_iOriginStep				= DEF_ORIGIN_START_STEP;
	m_iOriginPrevStep			= DEF_ORIGIN_START_STEP;
	m_iOriginError				= ERR_MOTION_SUCCESS;
	m_ttOriginTimer				= MTickTimer();

	/** For Simulation */
	m_dCurrentAxisPos			= 0.0;
	m_dTargetAxisPos			= 0.0;
	m_dRMoveDistance			= 0.0;
	m_dVelocity					= 0.0;
	m_iAccelerate				= 0;
	m_iDecelerate				= 0;
	m_bRMoveStart				= FALSE;
	m_MotionTimer.StopTimer();
	m_dToHomeDistance			= 0.0;

	// Doolin kwangilshin 2017. 08. 27.
	//
	m_pRS_NMCSetup				= NULL;
	// End.
}

/**
 * Constructor : Set data, set inner object
 *				 ���޹��� �μ��� Data�� �����ϰ�, ���� ��ü�� �����Ѵ�.
 *
 * @param	iObjectID		: Object ID
 * @param	iBoardType		: Motion Board Type (1=MMC Board, 2=MEI board, 3=PPC Board, ...)
 * @param	saxAxis			: 1�� ���� ����
 * @param	iErrorBase		: Error Code Start Number
 * @param	strFullFileName	: Log File Path & File Name
 * @param	ucLevel			: Log Level (None, Normal, Warning, Error)
 * @param	iDays			: Log ���� �� ��
 * @stereotype constructor
 */
MRSNMC::MRSNMC(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO, ISercosSetup* pSercosSetup)
: ICommon(commonData)
{
	int iResult;

	/** 1�� ���� member variable �ʱ�ȭ */
	SetData(&datComponent);

	/** IO Component */
	m_plnkIO			= pIO;
	m_plnkSercosSetup	= pSercosSetup;
	m_pMotionLib = NULL;

	/** Interlock IO Address */
	m_usiInterlockIOAddr = 0;

	/** Interlock IO Check Value */
	m_bInterlockJudge	= 0;

	/** Timer ��ü ���� */
	m_bOriginFlag		= FALSE;
	m_iOriginStep		= DEF_ORIGIN_START_STEP;
	m_iOriginPrevStep	= DEF_ORIGIN_START_STEP;
	m_iOriginError		= ERR_MOTION_SUCCESS;
	m_ttOriginTimer		= MTickTimer();
	
	/** Component �ʱ�ȭ */
	if ((iResult = Initialize (commonData, 0, m_SaxAxis)) != ERR_MOTION_SUCCESS)
	{
		// Error ó��
	}

	/** Timer ��ü ���� */
	m_ttOriginTimer = MTickTimer();

	/** For Simulation */
	m_dCurrentAxisPos	= 0.0;
	m_dTargetAxisPos	= 0.0;
	m_dRMoveDistance	= 0.0;
	m_dVelocity			= 0.0;
	m_iAccelerate		= 0;
	m_iDecelerate		= 0;
	m_bRMoveStart		= FALSE;
	m_bInitOrigin		= FALSE;
	m_MotionTimer.StopTimer();
}

/**
 * Destructor : Delete inner opbject
 *				���� ��ü�� �Ҹ��Ѵ�.
 * @stereotype destructor
 */
MRSNMC::~MRSNMC()
{
	/** Axis All Stop */
	/** Servo All Off */
	Stop();
	SetAmpEnable(FALSE);
	m_pMotionLib->SetUseAxis(GetRingID(),GetNodeID(), FALSE);
}

int MRSNMC::SetData(SSMotionData *pData)
{
	m_strFileName	= pData->m_strFileName;	//	����Ÿ ���� �̸�

	swprintf(m_SaxAxis.szName, _T("%s"), pData->m_sAxisVal.szName);
//	sprintf(m_SaxAxis.szName, "%s", pData->m_sAxisVal.szName);

	// �����ڸ� ���� �ý��ۿ��� ���� ����Ÿ�� SAxis1 ���·� ��ȯ�Ѵ�.
	m_SaxAxis.iAxisID			= pData->m_sAxisVal.iAxisID;
	m_SaxAxis.iOriginPriority	= pData->m_sAxisVal.iOriginPriority;
 	m_SaxAxis.bOriginDir		= pData->m_sAxisVal.bOriginDir;
	m_SaxAxis.bOriginFineDir	= pData->m_sAxisVal.bOriginFineDir;
 	m_SaxAxis.iCPhaseUseType	= pData->m_sAxisVal.iCPhaseUseType;
	m_SaxAxis.dScale			= pData->m_sAxisVal.dScale;
	m_SaxAxis.iRoleOfHome		= pData->m_sAxisVal.iRoleOfHome;
	m_SaxAxis.bAbsEncoder		= pData->m_sAxisVal.bAbsEncoder;

	// Serco ���� 
	m_SaxAxis.iRingNo			= pData->m_sAxisVal.iRingNo;
	m_SaxAxis.iNodeNo			= pData->m_sAxisVal.iNodeNo;
	m_SaxAxis.iServoType		= pData->m_sAxisVal.iServoType;
	
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::GetData(SSMotionData *pData)
{
	swprintf(pData->m_sAxisVal.szName, _T("%s"), m_SaxAxis.szName);
//	sprintf(pData->m_sAxisVal.szName, "%s", m_SaxAxis.szName);

	// �����ڸ� ���� �ý��ۿ��� ���� ����Ÿ�� SAxis1 ���·� ��ȯ�Ѵ�.
	pData->m_sAxisVal.iAxisID			= m_SaxAxis.iAxisID;
	pData->m_sAxisVal.iOriginPriority	= m_SaxAxis.iOriginPriority;	
 	pData->m_sAxisVal.bOriginDir		= m_SaxAxis.bOriginDir;		
	pData->m_sAxisVal.bOriginFineDir	= m_SaxAxis.bOriginFineDir;	
 	pData->m_sAxisVal.iCPhaseUseType	= m_SaxAxis.iCPhaseUseType;		
	pData->m_sAxisVal.dScale			= m_SaxAxis.dScale;			
	pData->m_sAxisVal.bAbsEncoder		= m_SaxAxis.bAbsEncoder;				

	// Serco ���� 
	pData->m_sAxisVal.iRingNo			= m_SaxAxis.iRingNo;
	pData->m_sAxisVal.iNodeNo			= m_SaxAxis.iNodeNo;
	pData->m_sAxisVal.iServoType		= m_SaxAxis.iServoType;

	return ERR_MOTION_SUCCESS;
}

int	MRSNMC::LoadParameter()
{
	int iResult = ERR_MOTION_SUCCESS;
	MAxisData	fileData(_T(""),m_strFileName);

	// ���Ͽ��� ����Ÿ�� �д´�.
	fileData.SetSection(m_SaxAxis.szName);	// Section�� ���� �Ѵ�.

	BOOL bReadSuccess	= fileData.ReadData();
	m_AxisParameter		= fileData;	// Component ���·� �����Ѵ�.
	// �ý��ۿ��� ���� ����Ÿ�� �ε��� ����Ÿ�� ������ Component�� �ʱ�ȭ �Ѵ�.

	if (!bReadSuccess)
		return generateErrorCode(107908);	// ERR_MAXES_PARAMETER_LOAD_FAIL;

	return iResult;
}

// Component�� ����Ÿ�� �����Ѵ�. �̶� ���Ͽ� ���� �����Ѵ�.

int MRSNMC::SaveParameter(MAxisData *pData)
{
	int	iResult = ERR_MOTION_SUCCESS;

	MAxisData	fileData("",m_strFileName);

	// ���Ͽ� ���� �Ѵ�.
	m_AxisParameter = *pData;

	fileData = m_AxisParameter;	// ������ ����Ÿ�� �����Ѵ�.

	fileData.SetSection(m_SaxAxis.szName);	// Section�� ���� �Ѵ�.
	if (fileData.WriteData() != TRUE)
		// 107909 = it's Failed to Save parameter.
		iResult = generateErrorCode(107909);	// ERR_MAXES_PARAMETER_SAVE_FAIL

	// hongju ����. SetAxisData�� MMC2 Manager���� ��� ó��
	// ����Ÿ�� Component ������ �����Ѵ�.
//	iResult = SetAxisData();

	return iResult;
}

/** �� Parameter(DataManager)�� ��´�. */
void MRSNMC::GetParameter(MAxisData* pData)
{
	*pData	= m_AxisParameter;
}

/**
 * ���� �̸��� Return �Ѵ�.
 * @return : Axis Name
 */
CString MRSNMC::GetAxisName()
{
	CString strName;

	strName.Format(_T("%s"), m_SaxAxis.szName);
	return strName;
}

/**
 * Motion ������ ��� ������ Interlock ���ǿ� �ش�Ǵ� IO Address�� ���ذ��� �����Ѵ�.
 *
 * @param	usiIOAddress : ������ Interlock IO Address
 * @param	bJudgeValue	: Interlock �Ǵ��� ��
 * @return	Error Code : 0 = SUCCESS, �׿� = Error
 */
void MRSNMC::SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue)
{
	/** Interlock IO Address */
	m_usiInterlockIOAddr	= usiIOAddress;

	/** Interlock IO Check Value */
	m_bInterlockJudge		= bJudgeValue;
}

/**
 * Motion Component�� �ʱ�ȭ�Ѵ�.
 *
 * 1. Motion Board ������ �´� Motion Library ��ü�� �����Ѵ�.
 * 2. �� ���� ������ �����Ѵ�.
 * 3. �� ������ �����Ѵ�.
 *
 * @param	iObjectID		: Object ID
 * @param	iBoardType		: Motion Board Type (1=MMC Board, 2=MEI board, 3=PPC Board, ...)
 * @param	saxAxis			: 1�� ���� ����
 * @param	iErrorBase		: Error Code Start Number
 * @param	strFullFileName	: Log File Path & File Name
 * @param	ucLevel			: Log Level (None, Normal, Warning, Error)
 * @param	iDays			: Log ���� �� ��
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXES NUMBER (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis)
{
	int iResult = ERR_MOTION_SUCCESS;
	BOOL bUse;
	CString strLogMsg;

	if (m_pMotionLib != NULL)
	{
		delete m_pMotionLib;
		m_pMotionLib = NULL;
	}
	
	m_pMotionLib = new MRS_NMCLib(commonData);

	
	// Doolin kwangilshin 2017. 08. 27.
	//
	m_pRS_NMCSetup = MOlbSystemPre.GetRSNMCSetupComponent();
	// End.
	
	/** Data �ϳ��� ���� �˻��ϸ� ���� */
	if ((iResult = m_pMotionLib->GetUseAxis(saxAxis.iNodeNo, &bUse)) != ERR_MOTION_SUCCESS)
		return iResult;

	if (bUse == TRUE)
		return generateErrorCode(106020);	// ERR_MAXES_USED_AXIS_ID

	/** �� ������� ���� */
	if ((iResult = m_pMotionLib->SetUseAxis(saxAxis.iRingNo, saxAxis.iNodeNo, TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	if (m_SaxAxis.iRoleOfHome != HOME_N_NONE)
		SetHomeSensorEvent(DEF_E_STOP_EVENT);
	else
		SetHomeSensorEvent(DEF_NO_EVENT);

	SetPositiveSensorEvent(DEF_E_STOP_EVENT);
	SetNegativeSensorEvent(DEF_E_STOP_EVENT);

	// Motion Sampling Rater�� 4msec�� ����
	m_pMotionLib->SetControlTimerForAx(saxAxis.iNodeNo, DEF_SAMPLING_4MSEC);
	m_pMotionLib->SetControlTimerForAx(saxAxis.iNodeNo, DEF_SAMPLING_4MSEC, TRUE);


	return iResult;
}
/**
 * �� 1���� ���� Data�� �����Ѵ�. (����ü)
 *
 *	(1) �� ���� ������ �־�� �Ѵ�.
 *	(2) ���� �̹� ���ǰ� ������ �ȵȴ�.
 *
 * @param   ax1Data         : ������ �� ���� ���� Data
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = USED AXIS ID (MULTIAXES)
 *							  xx = NO EXIST AXIS PARAMETER AREA (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetAxisData()
{
	int		iResult = ERR_MOTION_SUCCESS;
	double	dPos;
	BOOL	bLevel;

	if ((iResult = checkAxisParameter(m_SaxAxis)) != ERR_MOTION_SUCCESS)
		return iResult;

	m_pMotionLib->GetInPosition(m_SaxAxis.iNodeNo, &dPos, &bLevel);
	m_pMotionLib->SetInPosition(m_SaxAxis.iNodeNo, m_AxisParameter.dTolerance * m_SaxAxis.dScale, bLevel);
	return ERR_MOTION_SUCCESS;
}
/**
 * �� 1���� ���� Data�� �д´�. (����ü)
 *
 * @param   *pax1Data       : ������ �� ���� ���� Data
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::GetAxisData(SAxis1 *pax1Data)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(pax1Data, TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	*pax1Data = m_SaxAxis;
	
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::SetAutoCP(int iBoardNo, BOOL* pbAutoSet)
{
	int iResult = ERR_MOTION_SUCCESS;
	short siTotalBoardNum;

	if ((iResult = checkBoardExistArgument(iBoardNo,(void*)pbAutoSet, TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Board ���� ������ŭ ���� */
	if (iBoardNo == DEF_ALL_MOTION_BD_ID)	// ��ü Board�� ���
	{
		/** �Ҵ�� �޸𸮿� ���� */
		if ((iResult = m_pMotionLib->GetBoardNum(&siTotalBoardNum)) != ERR_MOTION_SUCCESS)
			return iResult;

		for (int i = 0; i < siTotalBoardNum; i++)
		{
			if ((iResult = m_pMotionLib->SetSplAutoOff(i, pbAutoSet[i])) != ERR_MOTION_SUCCESS)
				return iResult;
		}
	}
	else	// �� �ϳ��� ���
	{
		if ((iResult = m_pMotionLib->SetSplAutoOff(iBoardNo, *pbAutoSet)) != ERR_MOTION_SUCCESS)
				return iResult;
	}

	return ERR_MOTION_SUCCESS;
}
/** 
 * Board�� ���� �ڵ� ��, ���� ��뿩�θ� �д´�.
 *(Library�� ��ȸ ����� ���� ����� SetAutpCP() ���ÿ��� ��ȸ�� ���������� ��ġ��)
 *
 * @param   iBoardNo        : MMC Board ��ȣ 0 ~ 7, -1 = All Board
 * @param   *pbAutoSet      : �ڵ� ��,���� ��������, TRUE : ����, FALSE : �ڵ�, iBoardNo=-1�̸� �迭�� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID MOTION BOARD ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::GetAutoCP(int iBoardNo, BOOL* pbAutoSet)
{
	int iResult = ERR_MOTION_SUCCESS;
	short siTotalBoardNum;

	if ((iResult = checkBoardExistArgument(iBoardNo,(void*)pbAutoSet, TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Board ���� ������ŭ ���� */
	if (iBoardNo == DEF_ALL_MOTION_BD_ID)	// ��ü Board�� ���
	{
		/** �Ҵ�� �޸𸮿��� Data �б� */
		if ((iResult = m_pMotionLib->GetBoardNum(&siTotalBoardNum)) != ERR_MOTION_SUCCESS)
			return iResult;

		for (int i = 0; i < siTotalBoardNum; i++)
		{
			if ((iResult = m_pMotionLib->GetSplAutoOff(i,(pbAutoSet + i))) != ERR_MOTION_SUCCESS)
				return iResult;
		}
	}
	else	// �� �ϳ��� ���
	{
		if ((iResult = m_pMotionLib->GetSplAutoOff(iBoardNo, pbAutoSet)) != ERR_MOTION_SUCCESS)
				return iResult;
	}

	return ERR_MOTION_SUCCESS;
}


EVelocityMode MRSNMC::GetVelocityMode()
{
	return m_AxisParameter.eVelocityMode;	
}

int MRSNMC::GetAxisID()
{
	return m_SaxAxis.iAxisID;	
}

int MRSNMC::GetRingID()
{
	return m_SaxAxis.iRingNo;	
}

int MRSNMC::GetNodeID()
{
	return m_SaxAxis.iNodeNo;	
}

double MRSNMC::GetHomeOffset()
{
	return m_AxisParameter.dHomeOffset;	
}

double MRSNMC::GetNegativeSWLimit()
{
	return m_AxisParameter.dCCWSWLimit;	
}

double MRSNMC::GetPositiveSWLimit()
{
	return m_AxisParameter.dCWSWLimit;	
}

double MRSNMC::GetSlowMovingVelocity()
{
	return m_AxisParameter.dSlowRunVelocity;
}

double MRSNMC::GetMovingVelocity()
{
	return m_AxisParameter.dRunVelocity;
}

int MRSNMC::GetMovingAccelerate()
{
	return m_AxisParameter.iRunAccelerate;
}

double MRSNMC::GetCoarseVelocity()
{
	return m_AxisParameter.dHomeFastVelocity;	
}

int MRSNMC::GetCoarseAccelerate()
{
	return m_AxisParameter.iHomeAccelerate;
}

double MRSNMC::GetFineVelocity()
{
	return m_AxisParameter.dHomeSlowVelocity;
}

int MRSNMC::GetFineAccelerate()
{
	return m_AxisParameter.iHomeAccelerate;
}

double MRSNMC::GetJogSlow()
{
	return m_AxisParameter.dJogSlow;	
}

double MRSNMC::GetJogFast()
{
	return m_AxisParameter.dJogFast;
}

BOOL MRSNMC::GetSign()
{
	return m_SaxAxis.bSign;
}

BOOL MRSNMC::IsAbsEncoder() 
{
	return m_SaxAxis.bAbsEncoder;
}

BOOL MRSNMC::GetOriginDir()
{
	return m_SaxAxis.bOriginDir;
}

BOOL MRSNMC::GetOriginFineDir()
{
	return m_SaxAxis.bOriginFineDir;
}

int MRSNMC::GetCPhaseUseType()
{
	return m_SaxAxis.iCPhaseUseType;
}

double MRSNMC::GetScale()
{
	return m_SaxAxis.dScale;
}

double MRSNMC::GetMoveTime()
{
	return m_AxisParameter.dLimitTime;
}

double MRSNMC::GetMoveAfterTime()
{
	return m_AxisParameter.dStabilityTime;
}

double MRSNMC::GetTolerance()
{
	return m_AxisParameter.dTolerance;
}

double MRSNMC::GetPassOffset()
{
	return m_AxisParameter.dPassOffset;
}

double MRSNMC::GetOriginWaitTime()
{
	return m_AxisParameter.dOriginLimitTime;
}

BOOL MRSNMC::IsOriginReturn()
{
	return m_bOriginFlag;
}

void MRSNMC::SetOriginFlag()
{
	m_bOriginFlag = TRUE;
}

int MRSNMC::ResetOrigin(BOOL *pbReturn)
{
	int iResult			= ERR_MOTION_SUCCESS;
	
	iResult = checkAxisExistArgument(TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	BOOL bResult		= TRUE;

	m_bOriginFlag		= FALSE;
	m_iOriginStep		= 100;
	m_iOriginPrevStep	= 100;
	m_iOriginError		= ERR_MOTION_SUCCESS;

	if (pbReturn != NULL)
		*pbReturn = m_bOriginFlag;
	
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::StopReturnOrigin(void)
{
	int iResult = ERR_MOTION_SUCCESS;
	
	/** Thread ���� */
	TerminateThread(m_hOriginThread, 0);

	/** �� ��ο� ���� VStop ��� ���� */
	iResult = VStop();

	m_iOriginStep = DEF_ORIGIN_ERROR_STEP;
	//WriteErrorLog("StopReturnOrigin(): Origin Stop By User");
	// 107900 = Axis Origin is Stopped by User.
	//m_iOriginError = generateErrorCode(107900);	// ERR_MAXES_ORIGIN_STOP_BY_USER //170717 JSH

	return iResult;
}

/**
 * ���� ������ǥ�� �д´�.
 * 
 * @return   double   : ���� ��ǥ��
 * @param   bType                : ���� ��ġ ����, FALSE=������ġ, TRUE=��ǥ��ġ 
 */
double MRSNMC::GetCurrentPosition(BOOL bCommandType)
{	
	double dCurrentPosition = 0.0;

#ifdef SIMULATION
	dCurrentPosition = m_dCurrentAxisPos;
	return dCurrentPosition;
#endif

	double iPos;

	m_pMotionLib->GetPosition(GetNodeID(), bCommandType, &iPos);

	dCurrentPosition = double(iPos / GetScale());

	return dCurrentPosition;
}


int MRSNMC::SetCurrentPosition(double dCurrentPosition, BOOL bType)
{
	int iResult = ERR_MOTION_SUCCESS;

#ifdef SIMULATION
	return iResult;
#endif

	/** �� ���� ���� ���� */
	iResult = checkAxisExistArgument(TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	double dPos;

	// hongju ����...Sercos�� bType �ʿ� ����.
	dPos = dCurrentPosition * GetScale();

	/** ���� ��ǥ��ġ ���� */
	iResult = m_pMotionLib->SetPosition(GetNodeID(), bType, dPos);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	//SJ_YYK 160317 Add..
	iResult = m_pMotionLib->SetOriginComplete(GetNodeID());
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
 * 
 * @param	bPosOpt              : ���� ��ġ ����, FALSE=������ġ, TRUE=Motion�� ��ǥ��ġ
 * @param   dTargetPosition      : ���� ��ǥ�� 
 * @param   *pdPermission        : (OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
 * @param   *pdDeviation         : (OPTION = NULL) �� ���̰�
 * @return   BOOL	             : �񱳰��
 */
BOOL MRSNMC::ComparePosition(BOOL bPosOpt, double dTargetPosition,
								  double *pdPermission, double *pdDeviation)
{	
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;

	/** ���� ��ġ �б� */
#ifdef SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition();

#endif

	/** ��ġ ���� ��� */
	dDevPosition = dTargetPosition - dCurPosition;
	if (pdDeviation != NULL)
		*pdDeviation = dDevPosition;

	/** Judge ���� */
	if (pdPermission != NULL)
	{
		if (fabs(dDevPosition) <= *pdPermission)
			return TRUE;		
	}
	else
	{
		if (fabs(dDevPosition) <= GetTolerance())
			return TRUE;		
	}
	
	return FALSE;
}

/**
 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
 * 
 * @param   dTargetPosition      : ���� ��ǥ��	 
 * @param   *pdPermission        : (OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
 * @return  BOOL	             : �񱳰��, �� ��ǥ�� ���� ��ǥ - Tolerance ���� ������ True, �ƴϸ� False
 */
BOOL MRSNMC::IsLessThanTargetPosition(double dTargetPosition,
								 double *pdPermission)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;

	/** ���� ��ġ �б� */
#ifdef SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition();
#endif

	/** Judge ���� */
	if (pdPermission != NULL)
	{
		/** ��ġ ���� ��� */
		dDevPosition = dTargetPosition - *pdPermission - dCurPosition;
		if (dDevPosition >= 0)
			return TRUE;		
	}
	else
	{
		dDevPosition = dTargetPosition - GetTolerance() - dCurPosition;
		if (dDevPosition >= 0)
			return TRUE;		
	}
	
	return FALSE;
}

/**
 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
 * 
 * @param   dTargetPosition      : ���� ��ǥ��	 
 * @param   *pdPermission        : (OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
 * @return  BOOL	             : �񱳰��, ���� ��ǥ�� ���� ��ǥ + Tolerance ���� ũ�� True, �ƴϸ� False
 */
BOOL MRSNMC::IsGreaterThanTargetPosition(double dTargetPosition, double *pdPermission)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;

	/** ���� ��ġ �б� */
#ifdef SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition();
#endif

	/** Judge ���� */
	if (pdPermission != NULL)
	{
		/** ��ġ ���� ��� */
		dDevPosition = dTargetPosition + *pdPermission - dCurPosition;
		if (dDevPosition <= 0)
			return TRUE;		
	}
	else
	{
		dDevPosition = dTargetPosition + GetTolerance() - dCurPosition;
		if (dDevPosition <= 0)
			return TRUE;		
	}
	
	return FALSE;
}

/**
 * �� �̵� (�Ѱ��� �࿡ ���� �̵�) - �̵� �Ϸ�� �� return
 * 
 * @param   dPosition        : �̵��� ��ġ
 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param	iMoveType        : (OPTION=1) �̵� Type,
 *											0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
 *											1=S-Curve �ӵ� Profile, ������ǥ �̵�
 *											4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
 *											5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
 * MoveType ���� ����. ? 						
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = INVALID MOVE-TYPE (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::Move(double dPosition, double dVelocity, int iAccelerate, int iDecelerate, int iMoveType)
{
	int iResult = ERR_MOTION_SUCCESS;
	CString strLogMsg;

	if ((iMoveType == DEF_MOVE_DISTANCE) || (iMoveType == DEF_SMOVE_DISTANCE)
		 || (iMoveType == DEF_TMOVE_DISTANCE) || (iMoveType == DEF_TSMOVE_DISTANCE))
		// 107901 = Motion Function is Abnormal..
		return generateErrorCode(107901);	// ERR_MAXES_INVALID_MOVE_TYPE

	if ((iResult = StartMove(dPosition, dVelocity, iAccelerate, iDecelerate, iMoveType)) != ERR_MOTION_SUCCESS)
		return iResult;

	if ((iResult = Wait4Done()) != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * �� �̵� (�Ѱ��� �࿡ ���� �̵�) - �̵� ��� �� �ٷ� return
 * 
 * @param   dPosition        : �̵��� ��ġ
 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param	iMoveType        : (OPTION=1) �̵� Type,
 *											0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
 *											1=S-Curve �ӵ� Profile, ������ǥ �̵�
 *											4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
 *											5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = INVALID MOVE-TYPE (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::StartMove(double dPosition, double dVelocity,
							int iAccelerate, int iDecelerate, int iMoveType)
{
#ifdef SIMULATION
	m_MotionTimer.StartTimer();
	m_dTargetAxisPos	= dPosition;
	m_dVelocity			= dVelocity;
	m_iAccelerate		= iAccelerate;
	m_iDecelerate		= iDecelerate;
	m_bRMoveStart		= FALSE;
	m_dRMoveDistance	= 0.0;
	return ERR_MOTION_SUCCESS;
#endif
	m_dTargetAxisPos	= dPosition;

	int		iResult = ERR_MOTION_SUCCESS;
	double	dPos, dVel, iAccel, iDecel;
	CString	strLogMsg;
	INT32U	iJerk;

	/** �� ���� ���� ���� */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}

	/** �̵� �� �������� ���� Ȯ�� */
	if (!IsOriginReturn())
		// 107906 = Axis is not Origin.
		return generateErrorCode(107906);
	
	/** �� ���� ���� */
	if ((iResult = checkAxisState(GetNodeID())) != ERR_MOTION_SUCCESS)
		return iResult;

	dPos = GetScale() * dPosition;

	/** �̵� �ӵ� ó�� */
	if (dVelocity == 0.0)
	{
		if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
			dVel = GetScale() * GetMovingVelocity();
		else
			dVel = GetScale() * GetSlowMovingVelocity();

		if (0.0 == dVel)
			// 107418 = Velocity Value is Zero.
			return generateErrorCode(107418);
	}
	else
		dVel =  GetScale() * dVelocity;

	/** �̵� ���ӵ� ó�� */
	if (dVelocity == 0.0)
	{
		if (iAccelerate == 0)
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iAccel = (int)((GetScale() * GetMovingVelocity())/((double)GetMovingAccelerate()/1000));
			else
				iAccel = (int)((GetScale() * GetSlowMovingVelocity())/((double)GetMovingAccelerate()/1000));
		}
		else
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iAccel = (int)((GetScale() * GetMovingVelocity())/((double)iAccelerate/1000));
			else
				iAccel = (int)((GetScale() * GetSlowMovingVelocity())/((double)iAccelerate/1000));
		}
	}
	else
	{
		if (iAccelerate == 0)
			iAccel = (int)((GetScale() * dVelocity)/((double)GetMovingAccelerate()/1000));
		else
			iAccel = (int)((GetScale() * dVelocity)/((double)iAccelerate/1000));
	}

	/** �̵� ���ӵ� ó�� */
	if (dVelocity == 0.0)
	{
		if (iDecelerate == 0)
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iDecel = (int)((GetScale() * GetMovingVelocity())/((double)GetMovingAccelerate()/1000));
			else
				iDecel = (int)((GetScale() * GetSlowMovingVelocity())/((double)GetMovingAccelerate()/1000));
		}
		else
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iDecel = (int)((GetScale() * GetMovingVelocity())/((double)iDecelerate/1000));
			else
				iDecel = (int)((GetScale() * GetSlowMovingVelocity())/((double)iDecelerate/1000));
		}
	}
	else
	{
		if (iDecelerate == 0)
			iDecel = (int)((GetScale() * dVelocity)/((double)GetMovingAccelerate()/1000));
		else
			iDecel = (int)((GetScale() * dVelocity)/((double)iDecelerate/1000));
	}


	/** SW Limit Check */
	if ((iResult = checkSWLimit(dPosition)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	if ((iMoveType == DEF_SMOVE_DISTANCE) || (iMoveType ==DEF_TSMOVE_DISTANCE))
	{
		/* 20090203 jdy : jerk �� ���� ���� ���� ���� �ݿ� �ȵ�. RAK ���� Jerk ������ ���� API �߰� �����ϱ�� ����.
		                  20090119 ���뱹K �� ��ȭ �Ϸ�. - RAK ���� �� API ���� ���� �شٰ� ��.
						  �Ʒ� �ּ� ������ jerk ������� ����.
		// 20 �̶�� ��ġ�� ����(or ����) ������ �� 20% (���� ���� �� 10%, ���� ���� �� 10%) ������
		// ���ؼ� jerk �� �����Ѵٴ� �ǹ̶�� ��.
		// ����, iJerk ���� ����� ���� ��,���� �� ���� ���� ���� ������ ����ϴ� �� �´�.
		*/
		if (iAccel < iDecel)
		{
			iJerk = INT32U(iAccel * 20);
		}
		else
		{
			iJerk = INT32U(iDecel * 20);
		}
	}
	else
	{
		iJerk = INT32U(0);			// Curve ���¸� �����Ѵ�. (��ٸ���)          
	}
	
	/** No-wait �̵� */
	iResult = m_pMotionLib->Move(GetNodeID(), dPos, dVel, iAccel, iDecel, iMoveType);
	if(iResult != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}

	return ERR_MOTION_SUCCESS;
}

/**
 * �� �̵� (�Ѱ��� �࿡ ���� �����ġ �̵�) - �̵� �Ϸ�� �� return
 * 
 * @param   dDistance        : �̵��� �Ÿ�
 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param	iMoveType        : (OPTION=3) �̵� Type,
 *											2=��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
 *											3=S-Curve �ӵ� Profile, ���Ÿ� �̵�
 *											6=���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
 *											7=���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�
 * @param	bClearOpt        : (OPTION=FALSE) �̵� ���� �Ŀ� Encoder ���� Clear�ϴ� ���� ��� ���� (TRUE:���, FALSE:�̻��)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::RMove(double dDistance, double dVelocity, int iAccelerate, int iDecelerate,
						int iMoveType, BOOL bClearOpt)
{
	int iResult = ERR_MOTION_SUCCESS;
	CString	strLogMsg;

	/** �� ���� ���� ���� */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult != ERR_MOTION_SUCCESS)
		return iResult;

	/** �̵� �� �������� ���� Ȯ�� */
	if (!IsOriginReturn())
		//WriteErrorLog("RMove(): Not Origin Returned ");
		return generateErrorCode(107906);

	if ((iMoveType == DEF_MOVE_POSITION) || (iMoveType == DEF_SMOVE_POSITION)
		 || (iMoveType == DEF_TMOVE_POSITION) || (iMoveType == DEF_TSMOVE_POSITION))
		return generateErrorCode(107901);	// ERR_MAXES_INVALID_MOVE_TYPE

	if ((iResult = StartRMove(dDistance, dVelocity, iAccelerate,
							 iDecelerate, iMoveType, bClearOpt)) != ERR_MOTION_SUCCESS)
		return iResult;

	if ((iResult = Wait4Done(FALSE,FALSE,TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * �� �̵� (�Ѱ��� �࿡ ���� �����ġ �̵�) - �̵� ��� �� �ٷ� return
 * 
 * @param   dDistance        : �̵��� �Ÿ�
 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param	iMoveType        : (OPTION=3) �̵� Type,
 *											2=��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
 *											3=S-Curve �ӵ� Profile, ���Ÿ� �̵�
 *											6=���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
 *											7=���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�
 * @param	bClearOpt        : (OPTION=FALSE) �̵� ���� �Ŀ� Encoder ���� Clear�ϴ� ���� ��� ���� (TRUE:���, FALSE:�̻��)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::StartRMove(double dDistance, double dVelocity, int iAccelerate, int iDecelerate, 
							 int iMoveType, BOOL bClearOpt)
{
#ifdef SIMULATION
	m_MotionTimer.StartTimer();
	m_bRMoveStart		= TRUE;
	m_dRMoveDistance	= dDistance;
	m_dVelocity			= dVelocity;
	m_iAccelerate		= iAccelerate;
	m_iDecelerate		= iDecelerate;
	return ERR_MOTION_SUCCESS;
#endif
	
	int		iResult = ERR_MOTION_SUCCESS;
	double	dPos;
	double	dVel	= 0.0;
	double	iAccel	= 0;
	double	iDecel	= 0;
	m_dTargetAxisPos = GetCurrentPosition() + dDistance;	//@151217.KKY
	CString strLogMsg;
	
	INT32U	iJerk;

	iResult = checkAxisExistArgument(TRUE);
	if(iResult != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}


	if ((iMoveType == DEF_MOVE_POSITION) || (iMoveType == DEF_SMOVE_POSITION)
		 || (iMoveType == DEF_TMOVE_POSITION) || (iMoveType == DEF_TSMOVE_POSITION))
		return generateErrorCode(107901);	// ERR_MAXES_INVALID_MOVE_TYPE

	/** �� ���� ���� */
	if ((iResult = checkAxisState(GetNodeID())) != ERR_MOTION_SUCCESS)
		return iResult;


	/** ��ġ ���� */
	dPos = GetScale() * dDistance;

	/** �̵� �ӵ� ó�� */
	if (dVelocity == 0.0)
	{
		if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
			dVel = GetScale() * GetMovingVelocity();
		else
			dVel = GetScale() * GetSlowMovingVelocity();

		if (0.0 == dVel)
			// 107418 = Velocity Value is Zero.
			return generateErrorCode(107418);
	}
	else
		dVel = GetScale() * dVelocity;

	/** �̵� ���ӵ� ó�� */
	if (dVelocity == 0.0)
	{
		if (iAccelerate == 0)
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iAccel = (int)((GetScale() * GetMovingVelocity())/((double)GetMovingAccelerate()/1000));
			else
				iAccel = (int)((GetScale() * GetSlowMovingVelocity())/((double)GetMovingAccelerate()/1000));
		}
		else
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iAccel = (int)((GetScale() * GetMovingVelocity())/((double)iAccelerate/1000));
			else
				iAccel = (int)((GetScale() * GetSlowMovingVelocity())/((double)iAccelerate/1000));
		}
	}
	else
	{
		if (iAccelerate == 0)
			iAccel = (int)((GetScale() * dVelocity)/((double)GetMovingAccelerate()/1000));
		else
			iAccel = (int)((GetScale() * dVelocity)/((double)iAccelerate/1000));
	}
	
	/** �̵� ���ӵ� ó�� */
	if (dVelocity == 0.0)
	{
		if (iAccelerate == 0)
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iDecel = (int)((GetScale() * GetMovingVelocity())/((double)GetMovingAccelerate()/1000));
			else
				iDecel = (int)((GetScale() * GetSlowMovingVelocity())/((double)GetMovingAccelerate()/1000));
		}
		else
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iDecel = (int)((GetScale() * GetMovingVelocity())/((double)iDecelerate/1000));
			else
				iDecel = (int)((GetScale() * GetSlowMovingVelocity())/((double)iDecelerate/1000));
		}
	}
	else
	{
		if (iAccelerate == 0)
			iDecel = (int)((GetScale() * dVelocity)/((double)GetMovingAccelerate()/1000));
		else
			iDecel = (int)((GetScale() * dVelocity)/((double)iDecelerate/1000));
	}

	//SW Limit Check
	if ((iResult = checkSWLimit(dDistance)) != ERR_MOTION_SUCCESS)
		return iResult;

	// Jerk ����.
	if ((iMoveType == DEF_SMOVE_DISTANCE) || (iMoveType ==DEF_TSMOVE_DISTANCE))
	{
		/* 20090203 jdy : jerk �� ���� ���� ���� ���� �ݿ� �ȵ�. RAK ���� Jerk ������ ���� API �߰� �����ϱ�� ����.
		                  20090119 ���뱹K �� ��ȭ �Ϸ�. - RAK ���� �� API ���� ���� �شٰ� ��.
						  �Ʒ� �ּ� ������ jerk ������� ����.
		// 20 �̶�� ��ġ�� ����(or ����) ������ �� 20% (���� ���� �� 10%, ���� ���� �� 10%) ������
		// ���ؼ� jerk �� �����Ѵٴ� �ǹ̶�� ��.
		// ����, iJerk ���� ����� ���� ��,���� �� ���� ���� ���� ������ ����ϴ� �� �´�.
		*/
		if (iAccel < iDecel)
		{
			iJerk = INT32U(iAccel * 20);
		}
		else
		{
			iJerk = INT32U(iDecel * 20);
		}
	}
	else
	{
		iJerk = INT32U(0);			// Curve ���¸� �����Ѵ�. (��ٸ���)          
	}  	

	// Doolin kwangilshin 2017. 08. 27.
	//
	BOOL bPLimit = FALSE;
	BOOL bNLimit = FALSE;

	if (IsOnPositiveSensor())
	{
		if (dDistance <= 0)
		{
			bPLimit = TRUE;
			SetPositiveSensorEvent(DEF_NO_EVENT);
		}
	}
	else if (IsOnNegativeSensor())
	{
		if (dDistance >= 0)
		{
			bNLimit = TRUE;
			SetNegativeSensorEvent(DEF_NO_EVENT);
		}
	}
	// End.

	/** No-wait �̵� */
	iResult = m_pMotionLib->Move(GetNodeID(), dPos, dVel, iAccel, iDecel, iMoveType, bClearOpt);

	if (bPLimit)	SetPositiveSensorEvent(DEF_E_STOP_EVENT);		
	if (bNLimit)	SetPositiveSensorEvent(DEF_E_STOP_EVENT);

	return iResult;
}
/**
 * �� �̵� (�Ѱ��� �࿡ ���� ��� �̵�, ��� ��ġ���� ���� �̵���) 
 * 
 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
 * @param   bDir             : (OPTION=TRUE) �̵��� ����, TRUE:(+), FALSE:(-), �����ϸ� (+�������� �̵�
 *                             dVelocity�� ���� �־��ָ� bDir�� �����ؼ� ����ϸ� �ȴ�.
 *                             �� ���� dVelocity�� ��ȣ�� ���� �̵� ������ �����ȴ�.
 *                             dVelocity�� 0.0�� �־� ������ �ӵ��� ����ϴ� ����
 *                             bDir�� (+/-) ������ ������ �� �ִ�.
 *                             ����, dVelocity�� ���� ���� ��� bDir�� ������ �ϰ� �Ǹ�
 *                             ������ dVelocuty, dAccelerate�� bDir�� �ݿ��Ǿ� �̵��� �ϰ� �ȴ�.
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::VMove(double dVelocity, int iAccelerate, BOOL bDir)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	int iResult = ERR_MOTION_SUCCESS;
	double dVel = 0.0;
	int iAccel = 0;
	int dDir;
	INT32S	iVel;
	INT32U	iAcc, iDec, iJerk;

//	if (TRUE == IsInMotion())
//		return ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	iResult = checkAxisExistArgument(TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;


	/** �� ���� ���� */
	iResult = checkAxisState(GetNodeID());
	if(iResult != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}

	/** �̵� �ӵ� ó�� */
	if (dVelocity == 0.0)
	{
		if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
			dVel = GetScale() * GetMovingVelocity();
		else
			dVel = GetScale() * GetSlowMovingVelocity();

		if (0.0 == dVel)
			// 107418 = Velocity Value is Zero.
			return generateErrorCode(107418);
	}
	else
		dVel = GetScale() * dVelocity;

	/** �̵� ���ӵ� ó�� */
	if (dVelocity == 0.0)
	{
		if (iAccelerate == 0)
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iAccel = (int)((GetScale() * GetMovingVelocity())/((double)GetMovingAccelerate()/1000));
			else
				iAccel = (int)((GetScale() * GetSlowMovingVelocity())/((double)GetMovingAccelerate()/1000));
		}
		else
		{
			if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
				iAccel = (int)((GetScale() * GetMovingVelocity())/((double)iAccelerate/1000));
			else
				iAccel = (int)((GetScale() * GetSlowMovingVelocity())/((double)iAccelerate/1000));
		}
	}
	else
	{
		if (iAccelerate == 0)
			iAccel = (int)((GetScale() * dVelocity)/((double)GetMovingAccelerate()/1000));
		else
			iAccel = (int)((GetScale() * dVelocity)/((double)iAccelerate/1000));
	}
	
	iVel = INT32S(dVel);			// mm/sec -> Pulse
	iAcc = INT32U(iAccel);	        // ���ӱ��� �ð� -> mm/s^2
	iDec = INT32U(iAccel);	
	iJerk = INT32U(10);				// msec                    // �� ���� �ϱ� 

	/** (+) ���� */
	if (bDir == TRUE)
		dDir = 1;
	/** (-) ���� */
	else
		dDir = -1;

	// ��ȣ ���� 

	iVel = dDir * iVel;

	// Doolin kwangilshin 2017. 08. 27.
	//
	BOOL bPLimit = FALSE;
	BOOL bNLimit = FALSE;
	
	if (IsOnPositiveSensor() && dDir == 1)
	{	
		return ERR_MOTION_SUCCESS;		
	}
	else if (IsOnNegativeSensor() && dDir == -1)
	{	
		return ERR_MOTION_SUCCESS;	
	}
	else
	{
		iResult = m_pMotionLib->VMove(GetNodeID(), iVel, iAccel, iAccel);
		if(iResult != ERR_MOTION_SUCCESS)
		{
			return iResult;
		}
	}
	// End.

	return ERR_MOTION_SUCCESS;	
}

/**
 * �� �̵� �� �ϷḦ Ȯ���Ѵ�. (�Ѱ��� �࿡ ���� �Ϸ� Ȯ��) 
 * 
 * @param   bSkipMode        : (OPTION=FALSE) ��ġ Ȯ�� ���, �̵� �� ����ȭ �ð� ���� ��� ����
 * @param	bUseInterlock    : (OPTION=FALSE) Interlock�� ���� ���� ���� ��� ��� ���� (FALSE:������, TRUE:�����)
 * @param	bRMove		     : (OPTION=FALSE) RMove�� ��� ��� ���� (FALSE:������, TRUE:�����)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = TIMEOUT MOVE-TIME (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::Wait4Done (BOOL bSkipMode,BOOL bUseInterlock, BOOL bRMove)
{
#ifdef SIMULATION
	double dTime = calculateMotionTime();

	if (m_MotionTimer.IsTimerStarted())
	{
		while (m_MotionTimer.LessThan(dTime))
		{
			if (FALSE == m_bRMoveStart && m_dCurrentAxisPos == m_dTargetAxisPos)
			{
				break;
			}
			
			if (TRUE == m_bRMoveStart && 0 == m_dRMoveDistance)
			{
				break;
			}
			
			if (m_MotionTimer.MoreThan(m_AxisParameter.dLimitTime))
				return generateErrorCode(107007);
			
			Sleep(5);
		}
		
		if (FALSE == m_bRMoveStart)
		{
			m_dCurrentAxisPos = m_dTargetAxisPos;			
		}
		else
		{
			m_dCurrentAxisPos += m_dRMoveDistance;
			m_bRMoveStart = FALSE;
		}
		
		m_MotionTimer.StopTimer();
	}

	return ERR_MOTION_SUCCESS;
#endif

	BOOL bVal;
	int iResult = ERR_MOTION_SUCCESS;
	/*
	// 2010.08.22 KBC ---------------------------------------------------
	// Y, T���� Inposition signal�̵����� �ʾƼ� �ӽ� ��ġ
	MTickTimer T_TimeOut;
	if (m_SaxAxis.iAxisID == DEF_AXIS_SERCOS_STAGE_Y ||
		m_SaxAxis.iAxisID == DEF_AXIS_SERCOS_STAGE_T) {
		while (true) {
			Sleep(5);
			if (fabs(GetCurrentPosition() - m_dTargetAxisPos) < 0.010)  {
				if (!IsAxisDone()) {
					Sleep(500);
					EStop();
				}
				break;
			}

			if (T_TimeOut.MoreThan(m_AxisParameter.dLimitTime)) {
				ClearFrames();
				return generateErrorCode(105064);
			}
		}

	}
	else {
		iErr = smmc_in_position_event(GetRingID(), GetNodeID(), (int)m_AxisParameter.dLimitTime * 1000, &iState);
		if (iErr != ERR_MOTION_SUCCESS)
		{
			ClearFrames();
			return (iErr);
		}
	}
	// --------------------------------------------------------------------
	*/

	/** Timer Start */
	m_ttMovingTimer.StartTimer();

	while (!IsAxisDone())
	{
		/** Interlock Check */
		if ((m_usiInterlockIOAddr != 0) && (bUseInterlock == TRUE))
		{
			if (m_bInterlockJudge == TRUE)
				bVal = m_plnkIO->IsOn(m_usiInterlockIOAddr);
			else
				bVal = m_plnkIO->IsOff(m_usiInterlockIOAddr);

			if (bVal == TRUE)
			{
				//-> Motion ����
				Stop();
				return generateErrorCode(106066);	// ERR_MAXES_INTERLOCK_CHECKED
			}
		}

		//�� ���� �б�
		if (IsEStopEvent())//170713 JSH
		{
			// E-Stop Event �߻�
			EStop();
			return generateErrorCode(106058);
		}

		//Amp Fault Ȯ��
		if (IsAmpFault())
		{
			//Amp Fault �߻�
			EStop();
			return generateErrorCode(106008);
		}

		if (m_ttMovingTimer.MoreThan(m_AxisParameter.dLimitTime))
		{
			//-> Motion ����
			Stop();				
			return generateErrorCode(106021);	// ERR_MAXES_TIMEOUT_MOVE_TIME
		}
		Sleep(5);
	}

	// Ȯ�� �ʿ� : �̵� �� Servo Off �ŵ� Error display ����. Servo Off �Ǹ� in position ��ȣ�� �ߴ� �� ����. ��� ���� ��.
	// ���� ���⼭ Amp Falut, Amp Enable ���� �˻�.
	iResult = checkAxisState(GetNodeID());
	if (iResult)
		return iResult;

	if (FALSE == bSkipMode)
		Sleep((DWORD)(m_AxisParameter.dStabilityTime * 1000));

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� ���� �Ϸ� ���θ� Return �Ѵ�. 
 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ�� �Ǵ� ���Ҿ� In-Position ���α��� Ȯ���Ѵ�.
 *
 * @return	: TRUE = AXIS_DONE
 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
 */
BOOL MRSNMC::IsAxisDone()
{
#ifdef SIMULATION
	if (Wait4Done() == ERR_MOTION_SUCCESS)
		return TRUE;

	if (m_dCurrentAxisPos == m_dTargetAxisPos)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
#endif	

	return m_pMotionLib->IsAxisDone(GetNodeID());
}

/**
 *	���� �̵� �� (�ӵ� > 0) ������ Return �Ѵ�.
 *
 *	@return : TRUE  = �� �̵� ��.
 *            FALSE = �� ���� ����
 */
BOOL MRSNMC::IsInMotion()
{
#ifdef SIMULATION
	if (Wait4Done() == ERR_MOTION_SUCCESS)
		return TRUE;

	double dTime = calculateMotionTime();

	if (m_MotionTimer.IsTimerStarted()
		&& m_MotionTimer.GetElapseTime() < dTime)
	{
		return TRUE;
	}
	
	return FALSE;
#endif

	BOOL bStatus = FALSE;

	if (m_pMotionLib->InCommand(GetNodeID(), &bStatus, 1/*DEF_INMOTION*/) > 0)
		return TRUE;
	
	if(bStatus == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * ���� ���� �Ϸ� ���θ� Return �Ѵ�. 
 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ���Ѵ�.  In-Position Ȯ�ξ���
 *
 * @return	: TRUE = AXIS_DONE
 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
 */
BOOL MRSNMC::IsMotionDone()
{
#ifdef SIMULATION
	if (Wait4Done() == ERR_MOTION_SUCCESS)
		return TRUE;

	if (m_dCurrentAxisPos == m_dTargetAxisPos)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
#endif

	return m_pMotionLib->IsMotionDone(GetNodeID());
}

/**
 * �������� �̵��Ѵ�.
 * 
 * @param   iMaxPoint        : �������� �̵����� ����
 * @param   *pdPosition      : �������� �̵����� ����, iMaxPoint��ŭ ����
 * @param   *pdVelocity      : �̵� �� �ӵ�, iMaxPoint��ŭ ���� (Scale�� ������� �ʴ´�.)
 * @param   *piAccelerate    : �̵� �� ���ӵ�, iMaxPoint��ŭ ����
 * @param	bAutoFlag        : Auto CP ����, FALSE=AutoCP ������, TRUE=AutoCP �����  /// ��� ���� 
 * @param	bWaitOpt         : (OPTION=TRUE)�� �̵� �Ϸ� ��� ����, FALSE=��� ����, TRUE=�����
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = NO EXIST AXIS ID TO MOVE (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::MoveSplineLine(int iMaxPoint, double *pdPosition, double *pdVelocity, int *piAccelerate,
								 BOOL bAutoFlag, BOOL bWaitOpt)
{
	int		iResult = ERR_MOTION_SUCCESS;
	INT32S	rgdPos[DEF_SPLINE_MOVE_PATH_MAX_NO];
	INT32U	rgdVel[DEF_SPLINE_MOVE_PATH_MAX_NO];
	INT32U	rgdAcc[DEF_SPLINE_MOVE_PATH_MAX_NO];
	INT32U	iJerk;
	double	dScale;
	CString	strLogMsg;

	/** �� ���� ���� ���� */
	iResult = checkAxisExistArgument((void*)pdPosition, (void*)pdVelocity, (void*)piAccelerate, TRUE);
	if(iResult)	return iResult;

	/** �̵� �� �������� ���� Ȯ�� */
	if (!IsOriginReturn())
		return generateErrorCode(107906);

	/** �̵� �� �� Event State Ȯ�� */
	if ((iResult = checkAxisState(GetNodeID())) != ERR_MOTION_SUCCESS)
		return iResult;

	/** ����� �� ���� */
	dScale = GetScale();

	/** �̵� Point�� ���� */
	for (int i = 0; i < iMaxPoint; i++)
	{
		/** ��ġ ���� */
		rgdPos[i]	= INT32S(dScale * pdPosition[i]);
		rgdVel[i]	= INT32U(dScale * pdVelocity[i]);
		rgdAcc[i]	= INT32U((dScale * pdVelocity[i]) / ((double)piAccelerate[i]/1000));
		iJerk		= INT32U(10);					// msec                  //  �� ���� �ϱ� 
		
		/** SW Limit Check */
		if ((iResult = checkSWLimit(pdPosition[i])) != ERR_MOTION_SUCCESS)
			return iResult;
	}

	// �̵� Point�� ���� 
	for (int i = 0; i < iMaxPoint; i++)
	{
		if(i == 0)
		{
			iResult = m_pMotionLib->SplAutoLineMove(GetNodeID(), rgdPos[i], rgdVel[i], rgdAcc[i], rgdAcc[i], 0);	// 0:mcAborting
			if(iResult)	return iResult;
		}
		else
		{
			iResult = m_pMotionLib->SplAutoLineMove(GetNodeID(), rgdPos[i], rgdVel[i], rgdAcc[i], rgdAcc[i], 4);	// 4:mcBlendingNext
			if(iResult)	return iResult;
		}
		Sleep(10);
	}
	
	if (bWaitOpt == TRUE)
	{
		/** Spline Line �̵� �Ϸ�� ������ ��� */
		if (m_pMotionLib->WaitForDone(GetNodeID()) != ERR_MOTION_SUCCESS)
			return iResult;
	}
	
	return ERR_MOTION_SUCCESS;
}

/**
 * Jog Slow�� ���� �̵��Ѵ�.
 * 
 * @param   bDir             : �̵��� ����, TRUE:(+), FALSE:(-)
 * @param   dSlow           : (OPTION = 0.0) �̵��� �Ÿ�, 0.0 = ������ Slow�Ÿ� ���
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::JogMoveSlow(BOOL bDir, double dSlow)
{
	int iResult = ERR_MOTION_SUCCESS;
	double dJogSlow = 0.0;
	int dDir;
	INT32S iVel;
	INT32U iAcc, iDec, iJerk;
	double iAccel;
	double iDecel;

//	if (!IsAxisDone()) return ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult)	return iResult;

	/** �̵� �ӵ� ó�� */
	if (0.0 == dSlow)
		dJogSlow = GetScale() * m_AxisParameter.dJogSlow;
	else
		dJogSlow = GetScale() * dSlow;

	/** �̵� ���ӵ� ó�� */
	iAccel = INT32U((dJogSlow) / ((double)GetMovingAccelerate()/1000));

	/** �̵� ���ӵ� ó�� */
	iDecel = iAccel;
	
	iVel = INT32S(dJogSlow);				// mm/sec -> Pulse
	iAcc = INT32U(iAccel);		
	iDec = INT32U(iDecel);	
	iJerk = INT32U(0);						// msec                    // �� ���� �ϱ� 

	/** (+) ���� */
	if (bDir == TRUE)
		dDir = 1;
	/** (-) ���� */
	else
		dDir = -1;

	// ��ȣ ���� 
	iVel = dDir * (INT32S)dJogSlow;

	if ((iResult = CheckAxisStateForJog(bDir)) != ERR_MOTION_SUCCESS)
	{
		VStop();
		return iResult;
	}

	JogStop();

	iResult = m_pMotionLib->VMove(GetNodeID(), iVel, iAcc, iDec);
	if(iResult)	return iResult;

	if (IsAmpFault())
	{
		VStop();
		return generateErrorCode(107803);
	}

	return ERR_MOTION_SUCCESS;
}

/**
 * Jog Fast�� ���� �̵��Ѵ�.
 * 
 * @param   bDir		: �̵��� ����, TRUE:(+), FALSE:(-)
 * @param   dFast		: (OPTION = 0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
 * @return	Error Code	: 0 = SUCCESS
 *					 	  xx = INVALID AXIS ID (MULTIAXES)
 *					 	  xx = INVALID POINTER (MULTIAXES)
 *						  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::JogMoveFast(BOOL bDir, double dFast)
{
	int iResult = ERR_MOTION_SUCCESS;
	double dJogFast = 0.0;
	int dDir;
	INT32S iVel;
	INT32U iAcc, iDec, iJerk;
	double iAccel;
	double iDecel;

//	if (!IsAxisDone()) return ERR_MOTION_SUCCESS;
	/** �� ���� ���� ���� */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult)	return iResult;

	/** �̵� �ӵ� ó�� */
	if (0.0 == dFast)
		dJogFast = GetScale() * m_AxisParameter.dJogFast;
	else
		dJogFast = GetScale() * dFast;

	/** �̵� ���ӵ� ó�� */
	iAccel = INT32U((dJogFast) / ((double)GetMovingAccelerate()/1000));

	/** �̵� ���ӵ� ó�� */
	iDecel = iAccel;
	
	iVel = INT32S(dJogFast);				// mm/sec -> Pulse
	iAcc = INT32U(iAccel);		
	iDec = INT32U(iDecel);	
	iJerk = INT32U(0);						// msec                    // �� ���� �ϱ� 

	/** (+) ���� */
	if (bDir == TRUE)
		dDir = 1;
	/** (-) ���� */
	else
		dDir = -1;

	// ��ȣ ���� 
	iVel = dDir * (INT32S)dJogFast;

	if ((iResult = CheckAxisStateForJog(bDir)) != ERR_MOTION_SUCCESS)
	{
		VStop();
		return iResult;
	}

	JogStop();

	iResult = m_pMotionLib->VMove(GetNodeID(), iVel, iAcc, iDec);
	if(iResult)	return iResult;
	
	if (IsAmpFault())
	{
		VStop();
		return generateErrorCode(107803);
	}

	return ERR_MOTION_SUCCESS;

}

/**
 * Jog �̵��� ���� ��Ų��.
 * 	 
 * @return	Error Code		 : 0 = SUCCESS
 *							  �� �� =  Error Code
 */
int MRSNMC::JogStop()
{
	int iResult = ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult)	return iResult;

	double dDecel;
	dDecel = (m_SaxAxis.dScale * m_AxisParameter.dRunVelocity) / (((double)m_AxisParameter.iRunAccelerate / 1000));

	iResult = m_pMotionLib->SetStop(GetNodeID(), DEF_VSTOP, dDecel, 0);
	if(iResult)	return iResult;

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� �����Ѵ�. (�Ѱ��� �࿡ ���� ����)
 * 
 * @param   *pbStatus      : (OPTION = NULL) ���� Stop ����
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::Stop(BOOL *pbStatus)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	
	INT32S	iErr;

	iErr = checkAxisExistArgument(TRUE);
	if(iErr)	return iErr;

	double dDecel;
	dDecel = (m_SaxAxis.dScale * m_AxisParameter.dRunVelocity) / ((double)m_AxisParameter.iRunAccelerate / 1000);

	iErr = m_pMotionLib->SetStop(GetNodeID(), DEF_STOP, m_AxisParameter.iRunAccelerate, 0);
	if (iErr != ERR_MOTION_SUCCESS)
		return (iErr);

	/** ��� ��� */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� ����̵��� ���� �����Ѵ�. (�Ѱ��� ���� ����̵��� ���� ����)
 * 
 * @param   *pbState         : (OPTION = NULL) ���� VStop ����
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::VStop(BOOL *pbStatus)
{
	CString strLog;
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	/** �� ���� ���� ���� */
	int iResult = checkAxisExistArgument(TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;	

	double dDecel;
	//dDecel = (m_SaxAxis.dScale * m_AxisParameter.dJogFast) / (((double)m_AxisParameter.iRunAccelerate / 1000));
	dDecel = (m_SaxAxis.dScale * m_AxisParameter.dSlowRunVelocity) / (((double)m_AxisParameter.iRunAccelerate / 1000));
	
	iResult = m_pMotionLib->SetStop(GetNodeID(), DEF_VSTOP, dDecel, 0);
	if(iResult)	return iResult;

	iResult = m_pMotionLib->WaitForDone(GetNodeID());
	if(iResult)	return iResult;

	/** VStop �� ����ȭ �ð� ���� */
	//Sleep((ULONG)(m_AxisParameter.dStabilityTime));

	/** ��� ��� */
	if(pbStatus != NULL)
		*pbStatus = TRUE;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * ���� ��������Ѵ�. (�Ѱ��� �࿡ ���� �������)
 * 
 * @param   *pbStatus        : (OPTION = NULL) ���� EStop ����
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::EStop(BOOL *pbStatus)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	
	/** �� ���� ���� ���� */
	int iResult = checkAxisExistArgument(TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	/** E-STOP */
	double dDecel;
	dDecel = (m_SaxAxis.dScale * m_AxisParameter.dSlowRunVelocity) / (((double)m_AxisParameter.iRunAccelerate / 1000));
	
	iResult = m_pMotionLib->SetStop(GetNodeID(), DEF_ESTOP, dDecel, 0);
	if(iResult)	return iResult;

	/** ��� ��� */
	if (pbStatus != NULL)
		*pbStatus = TRUE;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * ���� Servo�� On �Ѵ�. (�Ѱ��� �࿡ ���� Servo On ����)
 * 
 * @param   *pbStatus        : (OPTION = NULL) ���� Servo ON ����
 * @param   bLinearAxis        : (OPTION = FALSE) ���� Linear Type�̸� TRUE, �׿� FALSE
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
// Doolin kwangilshin 2017. 08. 27.
// Modify - Tact Time ����
//
int MRSNMC::ServoOn(BOOL *pbStatus, BOOL bIsCheck)
{
	int			iResult		= ERR_MOTION_SUCCESS;
	BOOL		bEnable		= DEF_DISABLE;

	MTickTimer	clsTimer;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}

	/** AMP Fault Reset */
	if (IsAmpFault() || IsEStopEvent())
	{
		if ((iResult = ResetAmpFault(TRUE)) != ERR_MOTION_SUCCESS)
		{
			return iResult;
		}

		Sleep(100);
	}	

	/** AMP Enable */
	m_pMotionLib->GetAmpEnable(GetNodeID(), &bEnable);
	
	if (!bEnable)
	{
		if ((iResult = m_pMotionLib->SetAmpEnable(GetNodeID(), TRUE)) != ERR_MOTION_SUCCESS)
			return iResult;

		clsTimer.StartTimer();
		while (bIsCheck)
		{
			if ((iResult = m_pMotionLib->GetAmpEnable(GetNodeID(), &bEnable)) != ERR_MOTION_SUCCESS)
				return iResult;

			if (bEnable)
			{
				bIsCheck = FALSE;
				break;
			}
			else if (clsTimer.MoreThan(2))
			{
				return generateErrorCode(106123);
			}
			Sleep(5);
		}		
	}
	
	if (pbStatus != NULL)
	{
		*pbStatus = bEnable;
	}
	
	return ERR_MOTION_SUCCESS;
}
// End.

/**
 * ���� Servo�� Off �Ѵ�. (�Ѱ��� �࿡ ���� Servo Off ����)
 * 
 * @param   *pbStatus        : (OPTION = NULL) ���� Servo OFF ����
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
// Doolin kwangilshin 2017. 08. 27.
// Modify - Tact Time ����
//
int MRSNMC::ServoOff(BOOL *pbStatus, BOOL bIsCheck)
{
	int			iResult		= ERR_MOTION_SUCCESS;
	BOOL		bEnable		= DEF_ENABLE;
	
	MTickTimer	clsTimer;
	
	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}
	
	/** AMP Fault Reset */
	if (IsAmpFault() || IsEStopEvent())
	{
		if ((iResult = ResetAmpFault(TRUE)) != ERR_MOTION_SUCCESS)
		{
			return iResult;
		}

		Sleep(10);
	}	
	
	/** AMP Disable */
	m_pMotionLib->GetAmpEnable(GetNodeID(), &bEnable);
	
	if (bEnable)
	{
		if ((iResult = m_pMotionLib->SetAmpEnable(GetNodeID(), FALSE)) != ERR_MOTION_SUCCESS)
			return iResult;
	
		clsTimer.StartTimer();
		while (bIsCheck)
		{
			if ((iResult = m_pMotionLib->GetAmpEnable(GetNodeID(), &bEnable)) != ERR_MOTION_SUCCESS)
				return iResult;
			
			if (!bEnable)
			{
				bIsCheck = FALSE;
				break;
			}
			else if (clsTimer.MoreThan(2))
			{
				return generateErrorCode(106124);
			}
			Sleep(5);
		}		
	}
	
	ResetOrigin();
	
	if (pbStatus != NULL)
	{
		*pbStatus = bEnable;
	}
	
	return ERR_MOTION_SUCCESS;
}
// End.

/**
 * ���� Home Sensor ���¸� �д´�. (�Ѱ��� �࿡ ���� �����б�)
 * 
 * @return   BOOL        : ���� Home Sensor ���� 
 */
BOOL MRSNMC::IsOnHomeSensor()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	ASSERT(m_pMotionLib != NULL);

	BOOL bStatus = FALSE;

	/** Home Sensor ���� �б� */
	m_pMotionLib->GetSensorStatus(GetNodeID(), DEF_HOME_SENSOR, &bStatus);
	
	return bStatus;	
}

/**
 * ���� Positive Sensor ���¸� �д´�. (�Ѱ��� �࿡ ���� �����б�)
 * 
 * @return   BOOL        : ���� Positive Sensor ���� 
 */
BOOL MRSNMC::IsOnPositiveSensor()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	ASSERT(m_pMotionLib != NULL);

	BOOL bStatus = FALSE;

	/** Positive Sensor ���� �б� */
	m_pMotionLib->GetSensorStatus(GetNodeID(), DEF_POSITIVE_SENSOR, &bStatus);
	
	return bStatus;	
}

/**
 * ���� Negative Sensor ���¸� �д´�. (�Ѱ��� �࿡ ���� �����б�)
 * 
 * @return   BOOL        : ���� Negative Sensor ���� 
 */
BOOL MRSNMC::IsOnNegativeSensor()
{	
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	ASSERT(m_pMotionLib != NULL);

	BOOL bStatus = FALSE;

	m_pMotionLib->GetSensorStatus(GetNodeID(), DEF_NEGATIVE_SENSOR, &bStatus);
	
	return bStatus;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * ��ɾ��� 
 * ���� ����(Source)�� �д´�. 
 * 
 * @return   SUCCESS
 */
//int MRSNMC::GetAxisSource()
int MRSNMC::GetAxisSource(int *piReturn)
{
	short iStatus;

	m_pMotionLib->GetAxisRunStop(0, &iStatus);

	/** �� Source ���� �б� */
	int iResult = m_pMotionLib->GetAxisSource(GetNodeID(), &iStatus);
	if (iResult)
		return iResult;

	*piReturn = (int)iStatus;
	return ERR_MOTION_SUCCESS;
}

/**
 * ��ɾ��� 
 * ���� ����(State)�� �д´�. 
 * 
 * @return   SUCCESS
 */
//int MRSNMC::GetAxisState(int *piReturn)
int MRSNMC::GetAxisState(UINT32 *piReturn) //170713 JSH
{
	//short iStatus;
	UINT32 iStatus;
	/** �� Source ���� �б� */
	int iResult = m_pMotionLib->GetAxisState(GetNodeID(), &iStatus);
	if (iResult)
		return iResult;
	
	//*piReturn = (int)iStatus;
	*piReturn = iStatus; //170713 JSH
	return ERR_MOTION_SUCCESS;
}

/**
 * ���� AMP Enable ���¸� �д´�. 
 * 
 * @param   BOOL        : �� �ϳ��� ���� AMP���� (AMP Enable : TRUE, �׿� : FALSE) 
 */
BOOL MRSNMC::IsAmpEnable()
{	

#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	BOOL bStatus = FALSE;

	/** AMP Enable ���� �б� */
	m_pMotionLib->GetAmpEnable(GetNodeID(), &bStatus);

	return bStatus;
}

/**
 * ���� AMP Enable/Disable ���·� �����.
 * 
 * @param   bEnable      : AMP Enable ���·� ���� : TRUE
 *						   AMP Disable ���·� ���� : FALSE
 * @param   bLinearAxis  : (OPTION = FALSE) ���� Linear Type�̸� TRUE, �׿� FALSE
 * @return	Error Code	 : 0 = SUCCESS
 *						   xx = INVALID AXIS ID (MULTIAXES)
 *						   xx = INVALID POINTER (MULTIAXES)
 *						   �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetAmpEnable(BOOL bEnable)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	
	INT32S	iErr;

	/** �� ���� ���� ���� */
	if ((iErr = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iErr;

	/** AMP Fault ���� ���� */
	if ((iErr = m_pMotionLib->SetAmpEnable(GetNodeID(), bEnable)) != ERR_MOTION_SUCCESS)
		return iErr;

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� AMP Fault ���¸� �д´�. 
 * 
 * @return   BOOL         : �࿡ ���� AMP Fault���� (AMP Fault:TRUE, Fault�ƴϸ�:FALSE)
 */
BOOL MRSNMC::IsAmpFault()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	BOOL bStatus;

	// AMP Fault ���� �б�
	m_pMotionLib->GetAmpFaultStatus(GetNodeID(), &bStatus);
	
	return bStatus;
}

/**
 * ���� AMP Fault ���¸� Reset �Ѵ�. 
 * 
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::ResetAmpFault(BOOL bIsCheck)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	int			iResult		= ERR_MOTION_SUCCESS;
	MTickTimer	clsTimer;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** AMP Fault ���� ���� */
	if ((iResult = m_pMotionLib->SetAmpFaultEnable(GetNodeID(), FALSE)) != ERR_MOTION_SUCCESS)
		return iResult;

	clsTimer.StartTimer();
	while (bIsCheck)
	{
		if (!IsAmpFault() && !IsEStopEvent())
		{
			bIsCheck = FALSE;
			break;		
		}
		else if (clsTimer.MoreThan(1))
		{
			bIsCheck = FALSE;
			return generateErrorCode(106122);
		}
		Sleep(5);
	}

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� AMP Fault ���¸� Clear/Enable �Ѵ�. 
 * 
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetAmpFaultEnable()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	int iResult = ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** AMP Fault ���� ���� */
	if ((iResult = m_pMotionLib->SetAmpFaultEnable(GetNodeID(), TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� ���¸� �ʱ�ȭ �Ѵ�. (�Ѱ��� �࿡ ���� �ʱ�ȭ)
 *  Clear Status & Clear Frames
 * 
 * @param   *pbStatus        : (OPTION = NULL) ���� �ʱ�ȭ ����
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::ClearAxis(BOOL *pbStatus)
{
#ifdef SIMULATION
	if (NULL != pbStatus)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
#endif

	int iResult = ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** �� ���� �ʱ�ȭ */
	if ((iResult = m_pMotionLib->ClearStatus(GetNodeID())) != ERR_MOTION_SUCCESS)
	{
		/** ��� ��� */
		if (pbStatus != NULL)
			*pbStatus = FALSE;

		return iResult;
	}

	/** �� Frame �ʱ�ȭ */
	if ((iResult = m_pMotionLib->ClearFrames(GetNodeID())) != ERR_MOTION_SUCCESS)
	{
		/** ��� ��� */
		if (pbStatus != NULL)
			*pbStatus = FALSE;

		return iResult;
	}

	/** ��� ��� */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

/** 
 * �������� �켱 ������ Return �Ѵ�.
 *
 * @return	�켱 ����		: 0~63
 */
int MRSNMC::GetOriginPriority()
{
	return m_SaxAxis.iOriginPriority;
}

/** 
 * �������� ���� ���� �Լ�(�ѹ� ȣ�⿡ �� Step�� ����)
 * ���� ���� Step�� �Ϸ� Step�� �� �� ���� ��� ȣ�� �ؾ���
 *
 * @param   iOriginMoveType :	DEF_ORIGIN_RETURN_NONE_MOVE (0) -> Motion ���� ���� ���� ��ġ�� �������� �����ϴ� ���.
 *								DEF_ORIGIN_RETURN_ALL_MOVE	(1) -> ���� ���� Offset �̵� ������ ������ ��� ���� ���� ���� Motion ������ �����ϴ� ���.
 *								DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE	(2) -> ���� ���� Offset �̵� ������ ������ ���� ���� ���� Motion ������ �����ϴ� ���.
 * @return	OriginStep
 */
// Doolin kwangilshin 2017. 08. 27.
// Function Modify - Tact Time ����.
// 
int MRSNMC::OriginReturn(int iOriginMoveType)
{
#ifdef SIMULATION
	m_bOriginFlag		= TRUE;
	m_iOriginStep		= 1000;
	m_dCurrentAxisPos	= 0.0;
	m_dTargetAxisPos	= 0.0;
	m_dRMoveDistance	= 0.0;
	m_bRMoveStart		= FALSE;
	return m_iOriginStep;
#endif

	INT32S	nErr;
	int		iResult			= 0;
	
	BOOL	bHomingState	= FALSE;
	BOOL	bHomeEnd		= FALSE;

	INT32U	iHommingStatus = 0;
	

	if (!m_bInitOrigin)
	{
		m_ttOriginTimer.StartTimer();

		if ((nErr = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		{
			m_iOriginError = nErr;
			SetOriginStep(DEF_ORIGIN_ERROR_STEP);
			return nErr;
		}
// 		else if (!m_pRS_NMCSetup->IsMasterRun() || !m_pRS_NMCSetup->IsSlaveOpModeAll())
// 		{
// 			if (!m_pRS_NMCSetup->IsECatInitializing())
// 			{
// 				if ((nErr = m_pRS_NMCSetup->BoardInit()) != ERR_MOTION_SUCCESS)
// 				{
// 					m_iOriginError = nErr;
// 					SetOriginStep(DEF_ORIGIN_ERROR_STEP);
// 					SetErrorLevel(_T("MMC return Origin"), 3, __FILE__, __LINE__);
// 					return nErr;
// 				}
// 			}
// 		}
		else
		{
			if (IsAmpFault() || IsEStopEvent())
			{
				if ((nErr = ResetAmpFault(TRUE)) != ERR_MOTION_SUCCESS)
				{
					m_iOriginError = nErr;
					SetOriginStep(DEF_ORIGIN_ERROR_STEP);
					return nErr;
				}
			}
			
			if (!IsAmpEnable())
			{
				if ((nErr = ServoOn(NULL, TRUE)) != ERR_MOTION_SUCCESS)
				{
					m_iOriginError = nErr;
					SetOriginStep(DEF_ORIGIN_ERROR_STEP);
					return nErr;
				}
			}
			
			if (IsInMotion())
			{
				if ((nErr = Stop()) != ERR_MOTION_SUCCESS)
				{
					m_iOriginError = nErr;
					SetOriginStep(DEF_ORIGIN_ERROR_STEP);
					return nErr;
				}
			}

			m_bInitOrigin = TRUE;
		}
	}

	if (m_bInitOrigin)
	{
		switch (m_iOriginStep)
		{
		case 100:
			{
				if (IsOriginComplete())
				{
					SetOriginStep(1000);
				}
				else
				{
					m_dToHomeDistance	= GetCurrentPosition();
					m_iOriginError		= 0;
					m_bOriginFlag		= FALSE;

					SetOriginStep(150);
				}
			}
			break;


		case 150:
			{
				if (IsAbsEncoder()) 
				{
					m_ttOriginTimer.StartTimer();
					SetOriginStep(730);
				}	
				else
				{
					SetOriginStep(700);
				}
			}
			break;			  


		case 700:  
			{
				if ((iResult = m_pMotionLib->IsHoming(GetNodeID(), &bHomingState)) != ERR_MOTION_SUCCESS)
				{
					m_iOriginError = iResult;
					SetOriginStep(998);
					break;
				}

				if (bHomingState == FALSE)
				{
					double	dFastVelocity	= m_SaxAxis.dScale * m_AxisParameter.dHomeFastVelocity;
					double	dSlowVelocity	= m_SaxAxis.dScale * m_AxisParameter.dHomeSlowVelocity;
					double	dAccel			= dFastVelocity / ((double)m_AxisParameter.iHomeAccelerate / 1000);
					double	dDecel			= dFastVelocity / ((double)m_AxisParameter.iHomeAccelerate / 1000);
					double	dHomeOffset		= m_SaxAxis.dScale * m_AxisParameter.dHomeOffset;
					
					if (DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE == iOriginMoveType)
					{
						dHomeOffset = 0.0;
					}

					if ((iResult = m_pMotionLib->HomeSetConfig(GetNodeID(), m_SaxAxis.iCPhaseUseType, m_SaxAxis.bOriginDir)) != ERR_MOTION_SUCCESS) 	
					{
						m_iOriginError = iResult;
						SetOriginStep(998);
						break;
					}
					
					Sleep(100);

					if ((iResult = m_pMotionLib->HomeSetVelocity(GetNodeID(), dFastVelocity, dSlowVelocity, dAccel, dDecel, dHomeOffset)) != ERR_MOTION_SUCCESS)	
					{
						m_iOriginError = iResult;
						SetOriginStep(998);
						break;
					}
					
					Sleep(100);
					
					if ((iResult = m_pMotionLib->HomingStart(GetNodeID())) != ERR_MOTION_SUCCESS)	
					{
						m_iOriginError = iResult;
						SetOriginStep(998);
						break;
					}
				}

				m_ttOriginTimer.StartTimer();
			
				SetOriginStep(710);
			}
			break;


		case 710:
		{	
				if ((iResult = m_pMotionLib->IsHoming(GetNodeID(), &bHomingState)) != ERR_MOTION_SUCCESS)
				{
					m_iOriginError = iResult;
					SetOriginStep(998);
					break;
				}

				if ((iResult = m_pMotionLib->IsHomeEnd(GetNodeID(), &bHomeEnd)) != ERR_MOTION_SUCCESS)
				{	
					m_iOriginError = iResult;
					SetOriginStep(998);
					break;
				}

				if (bHomingState || bHomeEnd)
				{
					SetOriginStep(720);
					m_ttOriginTimer.StartTimer();
				}
				else if (m_ttOriginTimer.MoreThan(5))
				{
					m_iOriginError	= generateErrorCode(106122);
					SetOriginStep(998);
				}
			}
			break;


		case 720:
			{
				if ((iResult = m_pMotionLib->IsHomeEnd(GetNodeID(), &bHomeEnd)) != ERR_MOTION_SUCCESS)
				{	
					m_iOriginError = iResult;
					SetOriginStep(998);
					break;
				}
				
				if (bHomeEnd)
				{
					SetOriginStep(730);
					m_ttOriginTimer.StartTimer();
				}
				else if (m_ttOriginTimer.MoreThan(m_AxisParameter.dOriginLimitTime))
				{
					m_iOriginError	= generateErrorCode(106123);
					SetOriginStep(998);
				}
			}
			break;

		
		case 730:
			{
				if (IsMotionDone())
				{
					SetOriginStep(750);
				}
				else if (m_ttOriginTimer.MoreThan(5))
				{
					m_iOriginError	= generateErrorCode(106124);
					SetOriginStep(998);
				}
			}
			break;


		case 750: // IsHoming ���� Ȯ��  
			{
				if (!MOlbSystemPre.IsFunctionModuleAxis(GetAxisID()) && GetAxisID() != DEF_AXIS_SERCOS_MAX_AXIS) 
				{
					if ((iResult = m_pMotionLib->SetOriginComplete(GetNodeID())) != ERR_MOTION_SUCCESS)
					{
						m_iOriginError = iResult;
						SetOriginStep(998);
						break;
					}
				}
				
				SetOriginStep(1000);
			}
			break;

		case 998:
			{
				m_bOriginFlag	= FALSE;
				m_bInitOrigin	= FALSE;
			
				StopReturnOrigin();
				SetOriginStep(999);
			}
			break;


		case 999:
			{
			}
			break;


		case 1000:
			{
				m_bOriginFlag = TRUE;
				m_bInitOrigin = FALSE;
			}
			break;

		default:
			{
				m_iOriginError = generateErrorCode(107903);
				SetOriginStep(998);
			}
			break;
		}
	}
	else if (m_ttOriginTimer.MoreThan(5))
	{

	}
		
	return m_iOriginStep;
}

//100712.KKY.OriginReturn�Լ��� ������ �ȵǴ� ���� ��������_____
int MRSNMC::OriginReturnByUser()
{
#ifdef SIMULATION
	m_bOriginFlag		= TRUE;
	m_iOriginStep		= 1000;
	m_dCurrentAxisPos	= 0.0;
	m_dTargetAxisPos	= 0.0;
	m_dRMoveDistance	= 0.0;
	m_bRMoveStart		= FALSE;
	return m_iOriginStep;
#endif

	INT32S nErr;

	if (TRUE == IsOriginComplete())
	{

		// �࿡ ���� ���� üũ �� ó��.
		if (TRUE == IsEStopEvent())
		{
			nErr = SetPositiveSensorEvent(DEF_NO_EVENT);
			if (nErr != ERR_MOTION_SUCCESS)
			{
				m_iOriginError = nErr;
				SetOriginStep(DEF_ORIGIN_ERROR_STEP);
				return nErr;
			}
			nErr = SetNegativeSensorEvent(DEF_NO_EVENT);
			if (nErr != ERR_MOTION_SUCCESS)
			{
				m_iOriginError = (nErr);
				SetOriginStep(DEF_ORIGIN_ERROR_STEP);
				return nErr;
			}

			nErr = ClearAxis();
			if (nErr != ERR_MOTION_SUCCESS)
			{
				m_iOriginError = (nErr);
				SetOriginStep(DEF_ORIGIN_ERROR_STEP);
				return nErr;
			}
			
			m_iOriginError = generateErrorCode(107416);
			SetOriginStep(DEF_ORIGIN_ERROR_STEP);
			
			// 107416 = Event ���¸� ������ �ּ���.
			return generateErrorCode(107416);
		}
		
		// �׾� �ִ� ���� ���� ó�� ���ʿ�.
		// Amp Fault ����
		if (TRUE == IsAmpFault())
		{
			nErr = ResetAmpFault();
			if (nErr)
			{
				m_iOriginError = nErr;
				SetOriginStep(DEF_ORIGIN_ERROR_STEP);
				return nErr;
			}
			
			if (TRUE == IsAmpFault())
			{
				m_iOriginError = generateErrorCode(107417);
				SetOriginStep(DEF_ORIGIN_ERROR_STEP);
				
				// 107417 = Fault Reset �ȵ�.
				return generateErrorCode(107417);
			}
		}
		
		nErr = SetPositiveSensorEvent(E_STOP_EVENT);
		if (nErr != ERR_MOTION_SUCCESS)
		{
			m_iOriginError = (nErr);
			SetOriginStep(DEF_ORIGIN_ERROR_STEP);
			return nErr;
		}
		//110623.KKY.Modify_____
		/*
#ifdef DEF_GATE_SYSTEM
		if(m_SaxAxis.iAxisID != 13)//DEF_AXIS_SERCOS_MULTITOOL_CHANGE
#else
		if(m_SaxAxis.iAxisID != DEF_AXIS_SERCOS_MULTITOOL_CHANGE)//DEF_AXIS_SERCOS_MULTITOOL_CHANGE
#endif
		*/

#ifdef DEF_GATE_SYSTEM
		//@������� MultiToolChange ��뿩�ο� ���� ���� �ʿ� 
		/*
		if(m_SaxAxis.iAxisID != DEF_AXIS_SERCOS_MULTITOOL_CHANGE ||
		   m_SaxAxis.iAxisID != DEF_AXIS_SERCOS_MULTITOOL_CHANGE2)//DEF_AXIS_SERCOS_MULTITOOL_CHANGE
		{
			nErr = SetNegativeSensorEvent(E_STOP_EVENT);
			if (nErr != ERR_MOTION_SUCCESS)
			{
				SetErrorLevel(_T("MMC return Origin"), 3, __FILE__, __LINE__);
				m_iOriginError = nErr;
				SetOriginStep(DEF_ORIGIN_ERROR_STEP);
				return makeSercosError(nErr);
			}
		}
		*/
#else
		/*/
		if(m_SaxAxis.iAxisID != DEF_AXIS_SERCOS_MULTITOOL_CHANGE)
		{
			nErr = SetNegativeSensorEvent(E_STOP_EVENT);
			if (nErr != ERR_MOTION_SUCCESS)
			{
				SetErrorLevel(_T("MMC return Origin"), 3, __FILE__, __LINE__);
				m_iOriginError = makeSercosError(nErr);
				SetOriginStep(DEF_ORIGIN_ERROR_STEP);
				return makeSercosError(nErr);
			}
		}
		/*/
#endif

		nErr = ServoOn();
		if (nErr)
		{
			m_iOriginError = nErr;
			SetOriginStep(DEF_ORIGIN_ERROR_STEP);
			return nErr;
		}

		m_bOriginFlag = TRUE;
		SetOriginStep(DEF_ORIGIN_FINISH_STEP);
		return ERR_MOTION_SUCCESS;
	}

	int iResult = 0;

//	INT32U iHomingVel;
//	INT32U iHomingAcc;
	INT32U iHommingStatus = 0;

	INT32U nConfig = 0;	// home use, marker not use
	INT32U nDir = 0;	// home=negtive, marker=negative

	double dPos = 0;

	// C�� ��� ���� �� ���� ����
	if (DEF_USE_SWITCH_ONLY == m_SaxAxis.iCPhaseUseType)
	{
		nConfig	= HOMING_USE_SWITCH;						// home use, marker not use
	}
	else if (DEF_USE_SWITCH_N_MARKER == m_SaxAxis.iCPhaseUseType)
	{
		nConfig	= HOMING_USE_SWITCH | HOMING_USE_MARKER;	// home use, marker use
	}
	else
	{
		nConfig	= HOMING_USE_MARKER;						// marker use
	}

	// Origin Dir
	if (FALSE == m_SaxAxis.bOriginDir)
	{
		nDir	= HOMING_DIR_SWITCH_NEG;
	}
	else
	{
		nDir	= HOMING_DIR_SWITCH_POS;
	}

	if (FALSE == m_SaxAxis.bOriginFineDir)
	{
		nDir	|= HOMING_DIR_EDGE_NEG | HOMING_DIR_MARKER_NEG ;
	}
	else
	{
		nDir	|= HOMING_DIR_EDGE_POS | HOMING_DIR_MARKER_POS ;
	}

	// �������� ���ѽð��� ���� �Ÿ� ����.
	INT32S iOriginLimitTime	= (int)m_AxisParameter.dOriginLimitTime*1000;	
	INT32U iHomingDistance	= 20000000;

	INT32U iVel, iAccel, iDecel;
	INT32S iPos;

	switch (m_iOriginStep)
	{
	case 100: // Amp On/Off

		m_iOriginError	= 0;
		//ResetOrigin
		m_bOriginFlag	= FALSE;

		if (FALSE == IsAbsEncoder())
		{
			nErr = SetPositiveSensorEvent(DEF_NO_EVENT);
			if (nErr != ERR_MOTION_SUCCESS)
			{
				m_iOriginError = nErr;
				SetOriginStep(998);
				break;
			}
			nErr = SetNegativeSensorEvent(DEF_NO_EVENT);
			if (nErr != ERR_MOTION_SUCCESS)
			{
				m_iOriginError = nErr;
				SetOriginStep(998);
				break;
			}
		}

		iResult = SetAmpEnable(DEF_ENABLE);		
		if (iResult != ERR_MOTION_SUCCESS)
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}
		m_ttOriginTimer.StartTimer();
		SetOriginStep(110);
		break;

	//Clear Axis
	case 110:
		if (m_ttOriginTimer.MoreThan(0.2))
		{
			ClearAxis();
			SetOriginStep(120);
		}
		break;

	//Home���� ���� �Ǿ� �ִ��� �Ǵ�
	case 120:
		if (IsOnHomeSensor())
		{
			SetOriginStep(160);
		}
		else
		{
			SetOriginStep(130);
		}
		break;

	//Home���� �̰����� -�������� �̵�
	case 130:
		iResult = VMove(GetCoarseVelocity(), GetCoarseAccelerate(), FALSE);
		if ((iResult != ERR_MOTION_SUCCESS))
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}
		m_ttOriginTimer.StartTimer();
		SetOriginStep(140);
		break;

	//-�������� �̵��� Home���� Ȯ��
	case 140:
		if (IsOnHomeSensor())
		{
			EStop();
			m_ttOriginTimer.StartTimer();
			SetOriginStep(150);
			break;
		}
		if (m_ttOriginTimer.MoreThan(m_AxisParameter.dOriginLimitTime))
		{
			EStop();
			// �������� �̵��ð��� �ʰ��߽��ϴ�.
			m_iOriginError = generateErrorCode(107415);
			SetOriginStep(998);	
		}
		break;

	//Clear Axis
	case 150:
		if (m_ttOriginTimer.MoreThan(0.2))
		{
			m_ttOriginTimer.StartTimer();
			ClearAxis();
			SetOriginStep(160);
		}
		break;

	//Home�������� Ȯ��
	case 160:
		if (!IsOnHomeSensor())
		{
			m_iOriginError = generateErrorCode(107415);
			SetOriginStep(998);	
			break;
		}
		if (m_ttOriginTimer.MoreThan(0.2))
		{
			SetOriginStep(170);
		}
		break;

	//Home���� ������ +�������� �̵�
	case 170:
		iResult = VMove(5.0/*GetFineVelocity()*5.0*/, GetCoarseAccelerate(), TRUE);
		if ((iResult != ERR_MOTION_SUCCESS))
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}
		SetOriginStep(180);
		m_ttOriginTimer.StartTimer();
		break;

	//Home���� ������� Ȯ��
	case 180:
		if (!IsOnHomeSensor())
		{
			EStop();
			m_ttOriginTimer.StartTimer();
			SetOriginStep(190);
			break;
		}
		if (m_ttOriginTimer.MoreThan(10.0))
		{
			// �������� �̵��ð��� �ʰ��߽��ϴ�.
			m_iOriginError = generateErrorCode(107415);
			EStop();
			SetOriginStep(998);	
		}
		break;

	//Clear Axis
	case 190:
		if (m_ttOriginTimer.MoreThan(0.2))
		{
			ClearAxis();
			m_ttOriginTimer.StartTimer();
			SetOriginStep(200);
		}
		break;
	
	//-�������� �̵�
	case 200:
		if (m_ttOriginTimer.MoreThan(0.2))
		{
			iResult = VMove(GetFineVelocity(), GetCoarseAccelerate(), FALSE);
			if ((iResult != ERR_MOTION_SUCCESS))
			{
				m_iOriginError = iResult;
				SetOriginStep(998);
				break;
			}
			m_ttOriginTimer.StartTimer();
			SetOriginStep(210);
		}
		break;

	//Home���� ���� Ȯ��
	case 210:
		if (IsOnHomeSensor())
		{
			EStop();
			m_dToHomeDistance = GetCurrentPosition() - m_dToHomeDistance;
			m_ttOriginTimer.StartTimer();
			SetOriginStep(220);
		}
		if (m_ttOriginTimer.MoreThan(30.0))
		{
			EStop();
			// �������� �̵��ð��� �ʰ��߽��ϴ�.
			m_iOriginError = generateErrorCode(107415);
			SetOriginStep(998);	
		}
		break;

	case 220:
		if (m_ttOriginTimer.MoreThan(0.2))
		{
			ClearAxis();
			m_ttOriginTimer.StartTimer();
			SetOriginStep(230);
		}
		break;
			
	case 230:
		if (m_ttOriginTimer.MoreThan(0.2))
		{
			iResult = m_pMotionLib->SetPosition(GetNodeID(), FALSE, 0.0);
			if ((iResult != ERR_MOTION_SUCCESS))
			{
				m_iOriginError = (iResult);
				SetOriginStep(998);
				break;
			}
			SetOriginStep(755);
		}
		break;

	case 755:
		if (m_AxisParameter.dHomeOffset == 0.0)
		{
			SetOriginStep(780);
		}
		else
			SetOriginStep(760);

		break;

	case 760:

		iVel = (INT32S)(GetScale() * GetCoarseVelocity());		
		iAccel = (INT32S)((GetScale() * GetCoarseVelocity())/((double)GetCoarseAccelerate()/1000));
		iDecel = iAccel;
		
	
		iPos = (int)(GetScale() * m_AxisParameter.dHomeOffset);
		
//		if (iPos < 0)
//			SetNegativeSensorEvent(DEF_E_STOP_EVENT);
//		else
			SetPositiveSensorEvent(DEF_E_STOP_EVENT);

		iResult = m_pMotionLib->Move(GetNodeID(), iPos, iVel, iAccel, iDecel, DEF_MOVE_POSITION);
		if ((iResult != ERR_MOTION_SUCCESS))
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}		

		SetOriginStep(770);
		m_ttOriginTimer.StartTimer();
		break;

	case 765:
	
		SetOriginStep(770);
		m_ttOriginTimer.StartTimer();
		break;

	case 770:
		if (TRUE == IsAxisDone())
		{
#ifndef SIMULATION
			if (IsEStopEvent()) //170713 JSH
			{
				SetErrorLevel(_T("MMC return Origin"), 3, __FILE__, __LINE__);
				// 107902 = E-Stop Event is Occurred.
				m_iOriginError = generateErrorCode(107902);
				SetOriginStep(998);
				break;
			}
#endif

			iResult = m_pMotionLib->SetPosition(GetNodeID(), FALSE, 0.0);
			if ((iResult != ERR_MOTION_SUCCESS))
			{
				m_iOriginError = (iResult);
				SetOriginStep(998);
				break;
			}
			
			SetOriginStep(780);
			m_ttOriginTimer.StartTimer();
			break;
		}

		if (m_ttOriginTimer.MoreThan(m_AxisParameter.dOriginLimitTime))
		{
			// �������� �̵��ð��� �ʰ��߽��ϴ�.
			m_iOriginError = generateErrorCode(107415);
			EStop();
			SetOriginStep(998);	
			break;
		}
		break;

// Position Set �ڵ� ����.
	case 780:
		SetOriginStep(781);
		break;

	case 781:
		SetPositiveSensorEvent(DEF_E_STOP_EVENT);
		//110623.KKY.Modify_____
//		SetNegativeSensorEvent(DEF_E_STOP_EVENT);
		SetNegativeSensorEvent(DEF_NO_EVENT);
		//______________________		
		m_bOriginFlag = TRUE;
		SetOriginStep(1000);
		break;

	case 998:
		HomingStop();
		SetOriginStep(999);
		break;
	case 999:
		break;
	case 1000:
		break;

	default:
		// 107903 = Axis Origin Step is Invalid.
		m_iOriginError = generateErrorCode(107903);
		SetOriginStep(998);
		break;
	}

	return m_iOriginStep;
}
//______________________________________________________________


/**
 * �������� Step�� �����Ѵ�. (�Ѱ��� ��)
 * 
 * @param   iStep            : ������ (0:����, 999:����, 1000:�Ϸ�, �׿�:������)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID ORIGIN STEP (<0) (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetOriginStep(int iStep)
{
	m_iOriginPrevStep = m_iOriginStep;
	m_iOriginStep = iStep;

	return ERR_MOTION_SUCCESS;
}

/**
 * �������� Step�� �д´�. (�Ѱ��� ��)
 * 
 * @return	Origin Step
 */
int MRSNMC::GetOriginStep()
{
	return m_iOriginStep;
}

/**
 * �������� Error�� �ʱ�ȭ�Ѵ�. (�Ѱ��� ��)
 * 
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::ClearOriginError()
{
	int iResult = ERR_MOTION_SUCCESS;

	m_iOriginError = ERR_MOTION_SUCCESS;

	return iResult;
}

/**
 * �������� Error�� �д´�. (�Ѱ��� ��)
 * 
 * @param   *piError         : �߻��� ���� Code
* @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::GetOriginError(int *piError)
{
	int iResult = ERR_MOTION_SUCCESS;

	*piError = m_iOriginError;

	return iResult;
}

/**
 * Home Sensor�� ������ ��ȯ �Ѵ�.
 *  	 
 * @return	ERoleOfHome		HOME_N_NONE : Home Sensor ����
							HOME_N_POSITIVE_LIMIT : Home, Positive Sensor ����
							HOME_N_NEGATIVE_LIMIT : Home, Negative Sensor ����
 */
ERoleOfHome MRSNMC::GetRoleOfHomeSensor()
{
	return m_SaxAxis.iRoleOfHome;
}

//170713 JSH.s
BOOL MRSNMC::IsEStopEvent()
{
	UINT32 Status;
	int iResult = GetAxisState(&Status);
	if (iResult)
	{
		return TRUE;
	}
	
	if (Status & mcErrorStop)
	{
		return TRUE;
	}

	return FALSE;
}
//170713 JSH.e

/**
 * SAxis1 Data�� ���� ���� ���˰� ��ü ���縦 �����Ѵ�.
 *
 * @param	srcAx			: ���� ������ ���� Data
 * @param	*pdstAx			: (OPTION=NULL) ���纻 Data
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID MOTION BOARD ID (MULTIAXES)
 *							  xx = INVALID MOTION AXIS ID (MULTIAXES)
 *							  xx = INVALID MOVE-PRIORITY NUMBER (MULTIAXES)
 *							  xx = INVALID ORIGIN-PRIORITY NUMBER (MULTIAXES)
 */
int MRSNMC::checkAxisParameter(SAxis1 srcAx)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	CString strLogMsg;

	/** �� ID ���� ���� */
	if ((srcAx.iAxisID < -1) || (srcAx.iAxisID > (MAX_NODE_NUM - 1)))
		// 107907 = Axis ID is Abnormal.
		return generateErrorCode(107907);	// ERR_MAXES_INVALID_AXIS_ID

	return ERR_MOTION_SUCCESS;
}

int MRSNMC::checkAxisState(int iAxisID)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	int iResult = ERR_MOTION_SUCCESS;
	//short siState, siSource; //170713

	short siSource;
//	UINT32 siState;
//	if ((iResult = m_pMotionLib->GetAxisState(iAxisID, &siState)) != ERR_MOTION_SUCCESS)
//		return iResult;

	if ((m_bOriginFlag == TRUE)/* && (siState != DEF_NO_EVENT)*/) //170713 JSH.test
	{
		if ((iResult = m_pMotionLib->GetAxisSource(iAxisID, &siSource)) != ERR_MOTION_SUCCESS)
			return iResult;

 		if ((siSource & DEF_ST_HOME_SWITCH) && (m_SaxAxis.iRoleOfHome != HOME_N_NONE))
		{
			//if((iAxisID != DEF_AXIS_NMC_HOLD_CST_Z2+1) && (iAxisID != DEF_AXIS_NMC_PBU_Y2+1))
			{
				m_bOriginFlag = FALSE;
				return generateErrorCode(106036);	// ERR_MAXES_HOME_SWITCH_AXIS_SOURCE;
			}			
		}

		else if (siSource & DEF_ST_POS_LIMIT)
		{
			// Doolin kwangilshin 2017. 08. 24.
			//
			double dbTgtPos		= 0.0;
			double dbCurrPos	= 0.0;

			dbTgtPos	= m_dTargetAxisPos;
			dbCurrPos	= GetCurrentPosition();

			// End.
			return ERR_MOTION_SUCCESS;
			//if((iAxisID != DEF_AXIS_NMC_HOLD_CST_Z2+1) && (iAxisID != DEF_AXIS_NMC_PBU_Y2+1))
			{
				m_bOriginFlag = FALSE;
				return generateErrorCode(106037);	// ERR_MAXES_POSITIVE_LIMIT_AXIS_SOURCE
			}
		}

		else if (siSource & DEF_ST_NEG_LIMIT)
		{
			return ERR_MOTION_SUCCESS;
			//if((iAxisID != DEF_AXIS_NMC_HOLD_CST_Z2+1) && (iAxisID != DEF_AXIS_NMC_PBU_Y2+1))
			{
				m_bOriginFlag = FALSE;
				return generateErrorCode(106038);	// ERR_MAXES_NEGATIVE_LIMIT_AXIS_SOURCE
			}
		}

		else if (siSource & DEF_ST_AMP_FAULT)
		{
			m_bOriginFlag = FALSE;
			return generateErrorCode(106039);	// ERR_MAXES_AMP_FAULT_AXIS_SOURCE
		}

		else if (siSource & DEF_ST_A_LIMIT)
			return generateErrorCode(106040);	// ERR_MAXES_ACCELERATE_LIMIT_AXIS_SOURCE

		else if (siSource & DEF_ST_V_LIMIT)
			return generateErrorCode(106041);	// ERR_MAXES_VELOCITY_LIMIT_AXIS_SOURCE

		else if (siSource & DEF_ST_X_NEG_LIMIT)
			return generateErrorCode(106042);	// ERR_MAXES_X_NEGATIVE_LIMIT_AXIS_SOURCE

		else if (siSource & DEF_ST_X_POS_LIMIT)
			return generateErrorCode(106043);	// ERR_MAXES_X_POSITIVE_LIMIT_AXIS_SOURCE

		else if (siSource & DEF_ST_ERROR_LIMIT)
			return generateErrorCode(106044);	// ERR_MAXES_ERROR_LIMIT_AXIS_SOURCE

		else if (siSource & DEF_ST_PC_COMMAND)
			return generateErrorCode(106045);	// ERR_MAXES_PC_COMMAND_AXIS_SOURCE

		else if (siSource & DEF_ST_OUT_OF_FRAMES)
			return generateErrorCode(106046);	// ERR_MAXES_OUT_OF_FRAMES_AXIS_SOURCE

		else if (siSource & DEF_ST_AMP_POWER_ONOFF)
			return generateErrorCode(106047);	// ERR_MAXES_AMP_POWER_ON_OFF_AXIS_SOURCE

		else if (siSource & DEF_ST_RUN_STOP_COMMAND)
			return generateErrorCode(106048);	// ERR_MAXES_RUN_STOP_COMMAND_AXIS_SOURCE

		else if (siSource & DEF_ST_COLLISION_STATE)
			return generateErrorCode(106049);	// ERR_MAXES_COLLISION_STATE_AXIS_SOURCE

		else if (siSource & DEF_ST_NONE)
		{
			if ((iResult = m_pMotionLib->ClearStatus(iAxisID)) != ERR_MOTION_SUCCESS)
				return iResult;

			Sleep(400);
			return ERR_MOTION_SUCCESS;
		}
		else if (siSource & DEF_ST_INPOSITION_STATUS)
		{
			if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
				return iResult;

			Sleep(400);
			return ERR_MOTION_SUCCESS;
		}
		else if (siSource & DEF_ST_ABS_COMM_ERROR)
		{
			if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
				return iResult;

			Sleep(400);
			return ERR_MOTION_SUCCESS;
		}
		else if (siSource != ERR_MOTION_SUCCESS)
		{
			if(siSource != DEF_ST_HOME_SWITCH)
				return generateErrorCode(106056);	// ERR_MAXES_UNKNOWN_AXIS_SOURCE
		}
		else
			m_pMotionLib->ClearStatus(iAxisID);
	}

	return ERR_MOTION_SUCCESS;
}

int MRSNMC::CheckAxisStateForJog(BOOL bDir)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	int iResult = ERR_MOTION_SUCCESS;
	int iSource;
	static BOOL bOriginEventSet = FALSE;

//	iResult = m_pMotionLib->GetAxisState(GetNodeID(), &siState);
//	if(iResult)	return iResult;		

//if(siState != DEF_NO_EVENT)
	{
		iResult = GetAxisSource(&iSource);
		if(iResult)	return iResult;

		if ((iSource & DEF_ST_HOME_SWITCH) 
			 && (m_SaxAxis.iRoleOfHome == HOME_N_NEGATIVE_LIMIT) && !bDir)
			return generateErrorCode(106036);	// ERR_MAXES_HOME_SWITCH_AXIS_SOURCE;
		else if ((iSource & DEF_ST_HOME_SWITCH) 
			      && (m_SaxAxis.iRoleOfHome == HOME_N_POSITIVE_LIMIT) && bDir)
			return generateErrorCode(106036);	// ERR_MAXES_HOME_SWITCH_AXIS_SOURCE;
		else if ((iSource & DEF_ST_POS_LIMIT) && bDir)
			return generateErrorCode(106037);	// ERR_MAXES_POSITIVE_LIMIT_AXIS_SOURCE

		else if ((iSource & DEF_ST_NEG_LIMIT) && !bDir)
			return generateErrorCode(106038);	// ERR_MAXES_NEGATIVE_LIMIT_AXIS_SOURCE

		else if (iSource & DEF_ST_AMP_FAULT)
			return generateErrorCode(106039);	// ERR_MAXES_AMP_FAULT_AXIS_SOURCE

		else if (iSource & DEF_ST_A_LIMIT)
			return generateErrorCode(106040);	// ERR_MAXES_ACCELERATE_LIMIT_AXIS_SOURCE

		else if (iSource & DEF_ST_V_LIMIT)
			return generateErrorCode(106041);	// ERR_MAXES_VELOCITY_LIMIT_AXIS_SOURCE

		else if (iSource & DEF_ST_X_NEG_LIMIT)
			return generateErrorCode(106042);	// ERR_MAXES_X_NEGATIVE_LIMIT_AXIS_SOURCE

		else if (iSource & DEF_ST_X_POS_LIMIT)
			return generateErrorCode(106043);	// ERR_MAXES_X_POSITIVE_LIMIT_AXIS_SOURCE

		else if (iSource & DEF_ST_ERROR_LIMIT)
			return generateErrorCode(106044);	// ERR_MAXES_ERROR_LIMIT_AXIS_SOURCE

		else if (iSource & DEF_ST_OUT_OF_FRAMES)
			return generateErrorCode(106046);	// ERR_MAXES_OUT_OF_FRAMES_AXIS_SOURCE

		else if (iSource & DEF_ST_AMP_POWER_ONOFF)
			return generateErrorCode(106047);	// ERR_MAXES_AMP_POWER_ON_OFF_AXIS_SOURCE

		else if (iSource & DEF_ST_COLLISION_STATE)
			return generateErrorCode(106049);	// ERR_MAXES_COLLISION_STATE_AXIS_SOURCE
		else if (iSource & DEF_ST_INPOSITION_STATUS)
		{
			if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
				return iResult;

			Sleep(400);
			return ERR_MOTION_SUCCESS;
		}
		else if (iSource & DEF_ST_ABS_COMM_ERROR)
		{
			if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
				return iResult;

			Sleep(400);
			return ERR_MOTION_SUCCESS;
		}
		else
		{
			if (iSource & DEF_ST_HOME_SWITCH)
			{
				// Home���� E-Stop �߻� ���� �� �ݴ� Jog ������ ���� Event ����
				SetHomeSensorEvent(DEF_NO_EVENT);				

				bOriginEventSet = TRUE;
			}
						
			if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
				return iResult;
			
			return ERR_MOTION_SUCCESS;
		}
	}

	if (bOriginEventSet)
	{
		//Home���� E-Stop �߻� ���� �� �ݴ� Jog ������ ���� Event ���� �� ��
		//Home Sensor�� ����� �ٽ� E-Stop Event�� �ɱ� ����...
		//
		iResult = GetAxisSource(&iSource);
		if(iResult)	return iResult;

		if (!(iSource & DEF_ST_HOME_SWITCH))
		{
			if (m_SaxAxis.iRoleOfHome != HOME_N_NONE)
				SetHomeSensorEvent(DEF_E_STOP_EVENT);

			bOriginEventSet = FALSE;
		}
	}

	// ���� ���� ���̸� �����̺��൵ üũ�Ѵ�.
	/*
	if(MPCBSystem.IsSyncMasterAxis(m_SaxAxis.iAxisID-1) == TRUE)
	{
		iResult = m_pMotionLib->GetAxisState(m_SaxAxis.iAxisID+1, &siState);
		if(iResult)	return iResult;

		if (siState != DEF_NO_EVENT)
		{
			iResult = m_pMotionLib->GetAxisSource(m_SaxAxis.iAxisID, &siSource);
			if(iResult)	return iResult;

			if (siSource & DEF_ST_AMP_FAULT)
			{
				WriteErrorLog("checkAxisState() : AMP_FAULT_AXIS_SOURCE");
				return generateErrorCode(106039);	// ERR_MAXES_AMP_FAULT_AXIS_SOURCE
			}
			else if (siSource & DEF_ST_AMP_POWER_ONOFF)
			{
				WriteErrorLog("checkAxisState() : AMP_POWER_ON_OFF_AXIS_SOURCE");
				return generateErrorCode(106047);	// ERR_MAXES_AMP_POWER_ON_OFF_AXIS_SOURCE
			}

			else if (siSource & DEF_ST_COLLISION_STATE)
			{
				WriteErrorLog("checkAxisState() : COLLISION_STATE_AXIS_SOURCE");
				return generateErrorCode(106049);	// ERR_MAXES_COLLISION_STATE_AXIS_SOURCE
			}
			else if (siSource & DEF_ST_INPOSITION_STATUS)
			{
				if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
					return iResult;

				Sleep(400);
				return ERR_MOTION_SUCCESS;
			}
			else if (siSource & DEF_ST_ABS_COMM_ERROR)
			{
				if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
					return iResult;

				Sleep(400);
				return ERR_MOTION_SUCCESS;
			}
			else
			{
				if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
					return iResult;
				
				return ERR_MOTION_SUCCESS;
			}
		}
	}
	*/

	return ERR_MOTION_SUCCESS;
}

/**
 * �̵��� ��ġ�� SW Limit�� ������� Ȯ���Ѵ�.
 * 
 * @param   dPosition		: �˻��� ��ġ
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = OVER SW POSITIVE LIMIT (MULTIAXES)
 *							  xx = OVER SW NEGATIVE LIMIT (MULTIAXES)
 */
int	MRSNMC::checkSWLimit(double dPosition)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	if (dPosition > m_AxisParameter.dCWSWLimit)
		// 107904 = Axis Exceeded S/W + Limit.
		return generateErrorCode(107904);	// ERR_MAXES_OVER_SW_POSITIVE_LIMIT

	if (dPosition < m_AxisParameter.dCCWSWLimit)
		//	107905 = Axis Exceeded S/W - Limit.
		return generateErrorCode(107905);	// ERR_MAXES_OVER_SW_NAGATIVE_LIMIT

	return ERR_MOTION_SUCCESS;
}

int MRSNMC::ChangePhase(int iPhase)
{
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::NodeClear()
{
	return ERR_MOTION_SUCCESS;
}

// NMC Library�� �����ϴ� ���� �ڵ带 System Error���·� �����
int MRSNMC::makeSercosError(int iError)
{
	return generateErrorCode(107000 + 999);
	//  Error define
	switch (iError)
	{
//	case ERR_MOTION_SUCCESS						:	iError = 0;			break; //0x0000		// "no error"
	case WRONG_RING_NUMBER						:	iError = 1;			break; //0x0001		// wrong ring number
	case WRONG_NODE_NUMBER						:	iError = 2;			break; //0x0002		// wrong node number
	case WRONG_STAGE_NUMBER						:	iError = 3;			break; //0x0003		// wrong stage nmuber
	case WRONG_AXIS_NUMBER						:	iError = 4;			break; //0x0004		// wrong axis number
	case WRONG_RING_CONFIG						:	iError = 5;			break; //0x0005		// wrong ring configuration
	case WRONG_STAGE_CONFIG						:	iError = 6;			break; //0x0006		// wrong stage configuration
	case TIME_OUT_ERROR							:	iError = 7;			break; //0x0007		// time out error
	case CANNOT_READ_NODE_LIST					:	iError = 8;			break; //0x0008		// can not read a node number list
	case CANNOT_READ_STAGE_COUNT				:	iError = 9;			break; //0x0009		// can not read a stage count
	case CANNOT_CHANGE_PHASE0					:	iError = 10;		break; //0x000A		// can not change phase 0
	case CANNOT_CHANGE_PHASE1					:	iError = 11;		break; //0x000B		// can not change phase 1
	case CANNOT_CHANGE_PHASE2					:	iError = 12;		break; //0x000C		// can not change phase 2
	case CANNOT_CHANGE_PHASE3					:	iError = 13;		break; //0x000D		// can not change phase 3
	case CANNOT_CHANGE_PHASE4					:	iError = 14;		break; //0x000E		// can not change phase 4
	case THERE_IS_NO_NMC_BOARD					:	iError = 15;		break; //0x000F		// There is no nmc board
	case DRIVER_IS_NOT_READY					:	iError = 16;		break; //0x0010		// Driver is not ready to operate
	case THE_FIRST_PARAM_IS_WRONG				:	iError = 17;		break; //0x0011		// The first parameter is wrong
	case THE_SECOND_PARAM_IS_WRONG				:	iError = 18;		break; //0x0012		// The second parameter is wrong
	case THE_THIRD_PARAM_IS_WRONG				:	iError = 19;		break; //0x0013		// The third parameter is wrong
	case THE_FOURTH_PARAM_IS_WRONG				:	iError = 20;		break; //0x0014		// The fourth parameter is wrong
	case THE_FIFTH_PARAM_IS_WRONG				:	iError = 21;		break; //0x0015		// The fifth parameter is wrong
	case THE_SIXTH_PARAM_IS_WRONG				:	iError = 22;		break; //0x0016		// The sixth parameter is wrong
	case THE_SEVENTH_PARAM_IS_WRONG				:	iError = 23;		break; //0x0017		// The seventh parameter is wrong
	case THE_EIGHTH_PARAM_IS_WRONG				:	iError = 24;		break; //0x0018		// The eighth parameter is wrong
	case THE_NINTH_PARAM_IS_WRONG				:	iError = 25;		break; //0x0019		// The ninth parameter is wrong
	case THE_TENTH_PARAM_IS_WRONG				:	iError = 26;		break; //0x001A		// The tenth parameter is wrong
	case IDN_DATA_TYPE_IS_WRONG					:	iError = 27;		break; //0x001B		// The idn data type is not a two bytes long or four bytes long
	case CANNOT_SERVO_ON						:	iError = 28;		break; //0x001C		// Drive Cannot servo on
	case CANNOT_SERVO_OFF						:	iError = 29;		break; //0x001D		// Drive Cannot servo off
	case EVENT_WAIT_ABANDONED					:	iError = 30;		break; //0x001E		// In position event object wait abandoned
	case EVENT_WAIT_TIMEOUT						:	iError = 31;		break; //0x001F		// In position event object wait timeout
	case EVENT_WAIT_FAILED						:	iError = 32;		break; //0x0020		// In position event object wait failed
	case HOMING_NOT_STOPPED						:	iError = 33;		break; //0x0021		// Previous homing operation is running
	case API_PARAMETER_RANGE_ERROR				:	iError = 34;		break; //0x0022		// API Parameter range error
	case NOT_AVAILABLE_AMP_STATE				:	iError = 35;		break; //0x0023		// API cannot be done in this amp state  
	case UNDEFINED_SWITCH_STATUS				:	iError = 36;		break; //0x0024		// Undefined switch status 
		
	case NO_IDN_NUMBER							:	iError = 100;		break; //0x1001		// no ident number
	case WRONG_ACCESS_TO_ELEMENT1				:	iError = 101;		break; //0X1009		// wrong access to element 1
	case NO_NAME								:	iError = 102;		break; //0x2001		// no name
	case NAME_TRASMISSION_TOO_SHORT				:	iError = 103;		break; //0x2002		// name transmission too short
	case NAME_TRASMISSION_TOO_LONG				:	iError = 104;		break; //0x2003		// name transmission too long
	case NAME_CANNOT_BE_CHANGED					:	iError = 105;		break; //0x2004		// name cannot be changed
	case NAME_WRINTE_PROTECTED					:	iError = 106;		break; //0x2005		// name write protected at this time
	case ATTRIBUTE_TRANSMISSION_TOO_SHORT		:	iError = 107;		break; //0x3002		// attribute transmission too short
	case ATTRIBUTE_TRANSMISSION_TOO_LONG     	:	iError = 108;		break; //0x3003		// attribute transmission too long
	case ATTRIBUTE_CANNOT_BE_MODIFIED        	:	iError = 109;		break; //0x3004		// attribute cannot be modified
	case ATTRIBUTE_WRITE_PROTECTED           	:	iError = 110;		break; //0x3005		// attribute is write protected at this time
	case NO_UNIT		                       	:	iError = 111;		break; //0x4001		// no units
	case UNIT_TRANSMISSION_TOO_SHORT         	:	iError = 112;		break; //0x4002		// unit transmission too short
	case UNIT_TRANSMISSION_TOO_LONG		      	:	iError = 113;		break; //0x4003		// unit transmission too long
	case UNIT_CANNOT_BE_CHANGED              	:	iError = 114;		break; //0x4004		// unit cannot be changed
	case UNIT_WRITE_PROTECTED		          	:	iError = 115;		break; //0x4005		// unit is write protected at this time
	case NO_MINIMUM_VALUE					 	:	iError = 116;		break; //0x5001		// no minimum value
	case MINVALUE_TRANSMISSION_TOO_SHORT  		:	iError = 117;		break; //0x5002		// minimum value transmission too short
	case MINVALUE_TRANSMISSION_TOO_LONG		   	:	iError = 118;		break; //0X5003		// minimum value transmission too long
	case MINVALUE_CANNOT_BE_CHANGED          	:	iError = 119;		break; //0x5004		// minimum value cannot be changed
	case MINVALUE_WRITE_PROTECTED	        	:	iError = 120;		break; //0x5005		// minimum value is write protected at this time
	case NO_MAXMUM_VALUE                     	:	iError = 121;		break; //0x6001		// no maximum value
	case MAXVALUE_TRANSMISSION_TOO_SHORT	  	:	iError = 122;		break; //0x6002		// maximum value transmission too short
	case MAXVALUE_TRANSMISSION_TOO_LONG      	:	iError = 123;		break; //0x6003		// maximum value transmission too long
	case MAXVALUE_CANNOT_BE_CHANGED		      	:	iError = 124;		break; //0x6004		// maximum value cannot be changed
	case MAXVALUE_WRITE_PROTECTED            	:	iError = 125;		break; //0x6005		// maximum value is write protected at this time
	case DATA_TRANSMISSION_TOO_SHORT         	:	iError = 126;		break; //0x7002		// data transmission too short
	case DATA_TRANSMISSION_TOO_LONG          	:	iError = 127;		break; //0x7003		// data transmission too long
	case DATA_CANNOT_BE_CHANGED              	:	iError = 128;		break; //0x7004		// data cannot be changed
	case DATA_WRITE_PROTECTED                	:	iError = 129;		break; //0x7005		// data write protected at this time
	case DATA_SMALLER_THAN_MINVALUE		       	:	iError = 130;		break; //0x7006		// data smaller than the min. value
	case DATA_GREATER_THAN_MAXVALUE          	:	iError = 131;		break; //0x7007		// data greater than the max. value
	case DATA_NOT_CORRECT                    	:	iError = 132;		break; //0x7008		// data not correct
	case DATA_WRITE_PROTECTED_BY_PASSWORD    	:	iError = 133;		break; //0x7009		// data write protected by password
	case DATA_CYCLIC_CONFIGURED              	:	iError = 134;		break; //0x700a		// data cyclic configured at this time
	case DATA_OUTSIDE_THE_VALID_NUMBER_AREA  	:	iError = 135;		break; //0x700c		// data outside the valid number area
	case DATA_CANNOT_BE_CHANGED_IN_HOMING		:	iError = 136;		break; //0x7010     // data can not be changed during homing
	case MOTION_NOT_COMPLETED					:	iError = 137;		break; //0x7011     // user motion does not completed
		
	//Diagnostic Error Define
	case SERVICE_CHANNEL_PRESENTLY_BUSY      	:	iError = 200;		break; //0x8001		// service channel presently busy
	case PROBLEM_IN_SERVICE_CHANNEL          	:	iError = 201;		break; //0x8002		// problem in service channel
	case DRIVE_ADDRESS_NOT_CORRECT           	:	iError = 202;		break; //0x8005		// drive address not corrrect
	case HS_TIMEOUT                          	:	iError = 203;		break; //0x8006		// HS timeout
	case AT_FAILURE                          	:	iError = 204;		break; //0x8007		// double AT failure
	case FIBER_OPTIC_RING_INTERRUPT          	:	iError = 205;		break; //0x8009		// fiber optics cable disconnected
	case TRANSMISSION_ABORTED                	:	iError = 206;		break; //0x800b		// transmission aborted(higher priority)
	case ILLEGAL_ACCESS		                   	:	iError = 207;		break; //0x800c		// illegal access(service channel still active)
	case DIAG_STATUS_CMD_CH_NOT_ACTIVE		 	:	iError = 208;		break; //0xC003		// command channel presently not active
	case DIAG_STATUS_CLASS_1				 	:	iError = 209;		break; //0xD001		// drive error(class 1 diagnostics, S-0-0011)
	case DIAG_STATUS_CP23						:	iError = 210;		break; //0xD002		// changeover phase 2->3 not possible
	case DIAG_STATUS_CP34						:	iError = 211;		break; //0xD003		// changeover phase 3->4 not possible
	case DIAG_STATUS_CMD_CANNOT_EXECUTED		:	iError = 212;		break; //0xD004		// command in drive cannot be executed
	case DIAG_STATUS_CONFIG						:	iError = 213;		break; //0xF001		// configuration error(command/actual value channel)
	case DIAG_STATUS_TIME_SLOT					:	iError = 214;		break; //0xF002		// error in time slot calculation
	case DIAG_STATUS_INCORRECT_PHASE			:	iError = 215;		break; //0xF003		// incorrect phase from NC
	case DIAG_STATUS_INTERNAL					:	iError = 216;		break; //0xF004		// internal error
	case DIAG_STATUS_LIFE_COUNTER				:	iError = 217;		break; //0xF005		// lifecounter error
	case DIAG_STATUS_TIMES_TOO_LONG				:	iError = 218;		break; //0xF006		// copy times too long
	case DIAG_STATUS_CHECKSUM_ERR				:	iError = 219;		break; //0xF007		// checksum error(Y parameter)
	case DIAG_STATUS_DOUBLE_MST_OR_SYNCIN		:	iError = 220;		break; //0xF008		// double MST failure or input signal SYNCIN failed
		
	//Board to Device Driver Error
	case ENMC_CMD_TIMEOUT_SERCOS_COMM			:	iError = 300;		break; //0xFF96		// (-106): procedure command timeout
	case ENMC_INV_DATA_SERCOS_COMM				:	iError = 301;		break; //0xFF97		// (-105): invalid data
	case ENMC_NC_TIMEOUT_SERCOS_COMM			:	iError = 302;		break; //0xFF98		// (-104): NC timeout 
	case ENMC_MMI_TIMEOUT_SERCOS_COMM			:	iError = 303;		break; //0xFF99		// (-103): MMI timeout 
	case ENMC_NOINIT_SERCOS_COMM				:	iError = 304;		break; //0xFF9A		// (-102): board not initizlied 
	case ENMC_INIT_SERCOS_COMM					:	iError = 305;		break; //0xFF9B		// (-101): init error 
	case ENMC_TIMEOUT_SERCOS_COMM				:	iError = 306;		break; //0xFF9C		// (-100): timeout
		
	// Homing Error Define
//	case HOMING_NO_ERROR						:	iError = 400;		break; //0x0000		// No Homing Error
	case HOMING_ERROR_AMP_FAULT					:	iError = 401;		break; //0xFFF1		// Amp Fault while homing
	case HOMING_ERROR_EVENT_STATUS				:	iError = 402;		break; //0xFFF2		// EVENT Status is not cleared
	case HOMING_ERROR_EVENT_SETTING				:	iError = 403;		break; //0xFFF3		// EVENT does not set to NO_EVENT
	case HOMING_ERROR_USER_CANCEL				:	iError = 404;		break; //0xFFF4		// Homing User Cancel
	case HOMING_ERROR_SHORT_DISTANCE			:	iError = 405;		break; //0xFFF5		// Short Homing Distance
	case HOMING_ERROR_CMD_BUF					:	iError = 406;		break; //0xFFF6		// Motion Command Buffer not Empty.
	case HOMING_ERROR_DIRECTION					:	iError = 407;		break; //0xFFF7		// Limit Switch disagree with Moving Direction.
	case HOMING_NOT_FOUND_MARKER				:	iError = 408;		break; //0xFFF8		// Not found Marker in 1 turn.
	case HOMING_ERROR_SERVO_OFF					:	iError = 409;		break; //0xFFF9		// Servo off in homing procedure
	case HOMING_ERROR_MODULO_SETTING			:	iError = 410;		break; //0xFFFA		// Modulo Setting is Enable
	case HOMING_ABNORMAL_STATE					:	iError = 411;		break; //0xFFFB		// Homing Procedure State is abnormal.
	case HOMING_ERROR_SWITCH					:	iError = 412;		break; //0xFFFC		// Not found Switch or abnormal Switch status.
	case HOMING_NOT_FOUND_HOME					:	iError = 413;		break; //0xFFFD		// Not found Home Switch.
	case HOMING_ERROR_SERVO_ON					:	iError = 414;		break; //0xFFFE		// Not Servo on.
	case HOMING_WDT_TIME_OUT					:	iError = 415;		break; //0xFFFF		// Homing Watch dog time out.

	default										:	iError = 999;		break; //			// Error Code ���� 
	}

	// XXX XXX      XXX
	//   7 NMCError �ν��Ͻ���ȣ
	return generateErrorCode(107000 + iError);
}


/**
 * ���� ������ �ӵ���ɰ��� PULSE������ �����ش�.
 *
 * @return	�ش� Axis�� ComVelocity
 */
int MRSNMC::GetComVelocity()
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� Home Sensor�� ���� Event�� �����Ѵ�.(�Ѱ��� �࿡ ���� ���� ����)
 * 
 * @param	iLimit           : ������ Event(0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetHomeSensorEvent(int iLimit)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� Home Sensor�� ���� Level�� �����Ѵ�.(�Ѱ��� �࿡ ���� ���� ����)
 * 
 * @param	bLevel           : ��ȣ Level(TRUE=HIGH, FLASE=LOW)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetHomeSensorLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Home Sensor Event/Level �����ϱ� */
	if ((iResult = m_pMotionLib->SetSensorLevel(GetNodeID(), 
		                                        DEF_HOME_SENSOR, bLevel)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}

/**
 * ���� Positive Sensor�� ���� Event�� �����Ѵ�.(�Ѱ��� �࿡ ���� �����б�)
 * 
 * @param	iLimit           : ������ Event(0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetPositiveSensorEvent(int iLimit)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Home Sensor Event/Level �����ϱ� */
	if ((iResult = m_pMotionLib->SetSensorEvent(GetNodeID(), 
		                                        DEF_POSITIVE_SENSOR, 
												(short)iLimit)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * ���� Positive Sensor�� ���� Limit�� �����Ѵ�.(�Ѱ��� �࿡ ���� �����б�)
 * 
 * @param	bLevel           : ��ȣ Level(TRUE=HIGH, FLASE=LOW)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetPositiveSensorLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Home Sensor Event/Level �����ϱ� */
	if ((iResult = m_pMotionLib->SetSensorLevel(GetNodeID(), 
		                                        DEF_POSITIVE_SENSOR, 
												bLevel)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * ���� Negative Sensor�� ���� Event �����Ѵ�.(�Ѱ��� �࿡ ���� �����б�)
 * 
 * @param	iLimit           : ������ Event(0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetNegativeSensorEvent(int iLimit)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** Home Sensor Event/Level �����ϱ� */
	
	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Home Sensor Event/Level �����ϱ� */
	if ((iResult = m_pMotionLib->SetSensorEvent(GetNodeID(), 
		                                        DEF_NEGATIVE_SENSOR, 
												(short)iLimit)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * ���� Negative Sensor�� ���� Level�� �����Ѵ�.(�Ѱ��� �࿡ ���� �����б�)
 * 
 * @param	bLevel           : ��ȣ Level(TRUE=HIGH, FLASE=LOW)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetNegativeSensorLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;
	
	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	iResult = m_pMotionLib->SetAmpEnable(GetNodeID(),FALSE);
	if(iResult)
		return iResult;

	/** Home Sensor Event/Level �����ϱ� */
	if ((iResult = m_pMotionLib->SetSensorLevel(GetNodeID(), 
												DEF_NEGATIVE_SENSOR, 
												bLevel)) != ERR_MOTION_SUCCESS)
		return iResult;

	iResult = m_pMotionLib->SetAmpEnable(GetNodeID(),TRUE);
	if(iResult)
		return iResult;

	
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::GetIndexRequired(BOOL* pbIndexReq)
{
	int iResult = ERR_MOTION_SUCCESS;

	//MMC�� �Լ�
	
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::SetIndexRequired(BOOL bIndexReq)
{
	int iResult = ERR_MOTION_SUCCESS;

	//MMC�� �Լ�
	
	return ERR_MOTION_SUCCESS;
}


double MRSNMC::calculateMotionTime()
{
	double dMotionTime = 0.0;
	double dMovingDistance = 0.0;

	if (FALSE == m_bRMoveStart)	// PtoP Move
		dMovingDistance = fabs(m_dCurrentAxisPos - m_dTargetAxisPos);
	else						// R Move
		dMovingDistance = fabs(m_dRMoveDistance);

	double dVel;
	if (m_dVelocity == 0.0)
	{
		if (GetVelocityMode() == VELOCITY_MODE_NORMAL)
			dVel = GetMovingVelocity();
		else
			dVel = GetSlowMovingVelocity();
	}
	else
		dVel = m_dVelocity;

	short iAccel = (m_iAccelerate == 0) ? GetMovingAccelerate() : m_iAccelerate;
	short iDecel = (m_iDecelerate == 0) ? GetMovingAccelerate() : m_iDecelerate;

	double dAccelateTime = (double)iAccel / 1000;
	double dDecelateTime = (double)iDecel / 1000;

	// ������ ���������� �̵� �Ÿ�
	double dAccDecelMovingDistance = dVel * dAccelateTime / 2 + dVel * dDecelateTime / 2;
	if (dAccDecelMovingDistance >= dMovingDistance)
	{
		// Ȯ�� �ʿ� : �ӽ� �ڵ�. ��Ȯ�� ����� �ƴ�. �׳� ���� ������ �ð���ŭ�� �Դ´ٰ� ����.

		dMotionTime = dAccelateTime + dDecelateTime;
	}
	else
	{
		double dUniformMotiontime = (dMovingDistance - dAccDecelMovingDistance) / dVel;
		dMotionTime = dAccelateTime + dUniformMotiontime + dDecelateTime;
	}

	return dMotionTime;
}

void MRSNMC::SetAxisPosReset()
{
	m_dCurrentAxisPos	= 0.0;
	m_dTargetAxisPos	= 0.0;
}

/**
 * Stop(), EStop() �Լ� ������� ���ӵ��� ����.
 * �������� Run ���ӵ� ���� 1/3 �� ����.
 *
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::SetEStopRate()
{
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::HomingStop()
{
	int iResult = ERR_MOTION_SUCCESS;

	double dDecel;
	dDecel = (m_SaxAxis.dScale * m_AxisParameter.dRunVelocity) / (((double)m_AxisParameter.iRunAccelerate / 10000));
	
	iResult = m_pMotionLib->SetStop(GetNodeID(), DEF_STOP, dDecel, 0);
	if(iResult)	return iResult;

	m_bOriginFlag = FALSE;

	return ERR_MOTION_SUCCESS;
}

BOOL MRSNMC::IsOriginComplete()
{
	//SJ_YYK 160317 Modify.
	int iResult;
	short HomeFlag;
	// ���� �Ϸ� üũ
	iResult = m_pMotionLib->GetOriginCompleteFlag(GetNodeID(), &HomeFlag);
	if(iResult)	return FALSE;

	if(HomeFlag == 3)
		return TRUE;
	else return FALSE;
}

void MRSNMC::ResetOriginComplete()
{
	// ���� ���Ͱ� �Ϸ� ���� ���� �ɷ� �ν��ؼ� ���� ���͸� �ٽ� �ϵ��� ��....
	//smmc_set_pos_redefine_status(GetRingID(), GetNodeID(), POS_REDEFINE_EXTERNAL_APP);
	//SJ_YYK 160317 Modify.
	m_pMotionLib->ResetOriginComplete(GetNodeID());	
}

// ������ġ���� Home Sensor ������ �Ÿ�.
double MRSNMC::DistanceToCurrentToHome()
{
	return m_dToHomeDistance;
}

int MRSNMC::SetHomingSwitchSource(int iSourceAxisNodeID)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MRSNMC::ClearFrames()
{
	int iResult = ERR_MOTION_SUCCESS;

	/** �� ���� ���� ���� */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** �� Frame �ʱ�ȭ */
	if ((iResult = m_pMotionLib->ClearFrames(GetNodeID())) != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}
	
	return iResult;
}

int MRSNMC::SetMotorScale(double dScale)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	return ERR_MOTION_SUCCESS;
}

//100712.KKY__________
int MRSNMC::GetHomeSensorLevel(BOOL *bLevel)
{
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::GetPositiveSensorLevel(BOOL *bLevel)
{
	int iResult;

	iResult = m_pMotionLib->GetSensorLevel(GetNodeID(), DEF_POSITIVE_SENSOR, bLevel);
	if(iResult)
	{
		return iResult;
	}
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::GetNegativeSensorLevel(BOOL *bLevel)
{
	int iResult;

	iResult = m_pMotionLib->GetSensorLevel(GetNodeID(), DEF_NEGATIVE_SENSOR, bLevel);
	if(iResult)
	{
		return iResult;
	}
	return ERR_MOTION_SUCCESS;
}

/** 
 * �� ������ ���� ������ �����Ѵ�.(������ �μ� ����)
 *  1. Motion Library ��ü ���� ����
 *  2. �� ID�� ��������� ��� ���
 *  ���� �����Ǿ��־���ϴ� ���
 *    3. ������ ���� ���� ���
 *    4. �� ID�� ������ ���� ������ ��� ���
 *    5. ������ �࿡ ���� �� �������� ��ü ���� ����
 *
 * @param   bAxisExist      : �� �����Ǿ��ִ��� ���� ����, TRUE=�����Ǿ��־���Ѵ�.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = NO-EXIST MOTION LIBRARY OBJECT(MULTIAXES)
 *							  xx = INVALID MOTION AXIS ID(MULTIAXES)
 *							  xx = NO-EXIST MOTION AXIS CONFIG(MULTIAXES)
 *							  xx = OVER MOTION AXIS NUMBER(MULTIAXES)
 *							  xx = NO-EXIST MOTION AXIS AREA POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::checkAxisExistArgument(BOOL bAxisExist)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	/** 1. Motion Library �̻��� ���� */
	if (m_pMotionLib == NULL)
		return generateErrorCode(106024);	// ERR_MAXES_NO_EXIST_MOTION_LIBRARY_OBJECT

	/** �� �����Ǿ��־�� �Ѵ�. */
	if (bAxisExist == TRUE)
	{
		/** 2. ���� ����ϰ� �־�� �Ѵ�. */
		BOOL bStatus = FALSE;
		m_pMotionLib->GetUseAxis(GetNodeID(), &bStatus);
		if (bStatus == FALSE)
			return generateErrorCode(106026);	// ERR_MAXES_NO_EXIST_MOTION_AXIS_CONFIG
	}

	return ERR_MOTION_SUCCESS;
}
/** 
 * �� ������ ���� ������ �����Ѵ�.(������ �μ� 1��)
 *  1. ���޵� ������ �μ� ���� ����
 *  2. ��Ÿ ���� ����
 *
 * @param   *pPointer       : Pointer�� ���޵� �μ�
 * @param   bAxisExist      : �� �����Ǿ��ִ��� ���� ����, TRUE=�����Ǿ��־���Ѵ�.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer, BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. �μ� ������ ���� */
	if (pPointer == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. �� ���� Index ID ���� ���� */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * �� ������ ���� ������ �����Ѵ�.(������ �μ� 2��)
 *  1. ���޵� ������ �μ�1 ���� ����
 *  2. ���޵� ������ �μ�2 ���� ����
 *  3. ��Ÿ ���� ����
 *
 * @param   *pPointer1      : Pointer�� ���޵� �μ�1
 * @param   *pPointer2      : Pointer�� ���޵� �μ�2
 * @param   bAxisExist      : �� �����Ǿ��ִ��� ���� ����, TRUE=�����Ǿ��־���Ѵ�.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer1, void* pPointer2, BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. �μ� ������1 ���� */
	if (pPointer1 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. �μ� ������2 ���� */
	if (pPointer2 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 3. �� ���� Index ID ���� ���� */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * �� ������ ���� ������ �����Ѵ�.(������ �μ� 3��)
 *  1. ���޵� ������ �μ�1 ���� ����
 *  2. ���޵� ������ �μ�2 ���� ����
 *  3. ���޵� ������ �μ�3 ���� ����
 *  4. ��Ÿ ���� ����
 *
 * @param   *pPointer1      : Pointer�� ���޵� �μ�1
 * @param   *pPointer2      : Pointer�� ���޵� �μ�2
 * @param   *pPointer3      : Pointer�� ���޵� �μ�3
 * @param   bAxisExist      : �� �����Ǿ��ִ��� ���� ����, TRUE=�����Ǿ��־���Ѵ�.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer1, 
								  void* pPointer2, 
								  void* pPointer3,
								  BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. �μ� ������1 ���� */
	if (pPointer1 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. �μ� ������2 ���� */
	if (pPointer2 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 3. �μ� ������3 ���� */
	if (pPointer3 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 4. �� ���� Index ID ���� ���� */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * �� ������ ���� ������ �����Ѵ�.(������ �μ� 4��)
 *  1. ���޵� ������ �μ�1 ���� ����
 *  2. ���޵� ������ �μ�2 ���� ����
 *  3. ���޵� ������ �μ�3 ���� ����
 *  4. ���޵� ������ �μ�4 ���� ����
 *  5. ��Ÿ ���� ����
 *
 * @param   *pPointer1      : Pointer�� ���޵� �μ�1
 * @param   *pPointer2      : Pointer�� ���޵� �μ�2
 * @param   *pPointer3      : Pointer�� ���޵� �μ�3
 * @param   *pPointer4      : Pointer�� ���޵� �μ�4
 * @param   bAxisExist      : �� �����Ǿ��ִ��� ���� ����, TRUE=�����Ǿ��־���Ѵ�.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer1, 
								  void* pPointer2, 
								  void* pPointer3,
								  void* pPointer4,
								  BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. �μ� ������1 ���� */
	if (pPointer1 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. �μ� ������2 ���� */
	if (pPointer2 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 3. �μ� ������3 ���� */
	if (pPointer3 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 4. �μ� ������4 ���� */
	if (pPointer4 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 5. �� ���� Index ID ���� ���� */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * �� ������ ���� ������ �����Ѵ�.(������ �μ� 4��)
 *  1. ���޵� ������ �μ�1 ���� ����
 *  2. ���޵� ������ �μ�2 ���� ����
 *  3. ���޵� ������ �μ�3 ���� ����
 *  4. ���޵� ������ �μ�4 ���� ����
 *  5. ���޵� ������ �μ�5 ���� ����
 *  6. ��Ÿ ���� ����
 *
 * @param   *pPointer1      : Pointer�� ���޵� �μ�1
 * @param   *pPointer2      : Pointer�� ���޵� �μ�2
 * @param   *pPointer3      : Pointer�� ���޵� �μ�3
 * @param   *pPointer4      : Pointer�� ���޵� �μ�4
 * @param   *pPointer5      : Pointer�� ���޵� �μ�5
 * @param   bAxisExist      : �� �����Ǿ��ִ��� ���� ����, TRUE=�����Ǿ��־���Ѵ�.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer1, 
								  void* pPointer2, 
								  void* pPointer3,
								  void* pPointer4, 
								  void* pPointer5, 
								  BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. �μ� ������1 ���� */
	if (pPointer1 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. �μ� ������2 ���� */
	if (pPointer2 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 3. �μ� ������3 ���� */
	if (pPointer3 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 4. �μ� ������4 ���� */
	if (pPointer4 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 5. �μ� ������5 ���� */
	if (pPointer5 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 6. �� ���� Index ID ���� ���� */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * �� ������ ���� ������ �����Ѵ�.
 *  1. Motion Library ��ü ���� ����
 *  2. ���޵� ������ �μ� ���� ����
 *  3. ������ Board ������ ��������� ��� ���
 *  4. Board ID�� ��������� ��� ���
 *  Board�� �����Ǿ��־���ϴ� ���
 *    5. ������ Board�� ���� ���
 *    6. Board ID�� ������ Board�� ������ ��� ���
 *
 * @param   iBoardID        : Board ID, -1 = All Board
 * @param   *pPointer       : Pointer�� ���޵� �μ�
 * @param   bBoardExist     : Board �����Ǿ��ִ��� ���� ����, TRUE=�����Ǿ��־���Ѵ�.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = NO-EXIST MOTION LIBRARY OBJECT(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  xx = INVALID MOTION BOARD ID(MULTIAXES)
 *							  xx = INVALID MOTION BOARD NUMBER(MULTIAXES)
 *							  xx = NO-EXIST MOTION BOARD CONFIG(MULTIAXES)
 *							  xx = OVER MOTION BOARD NUMBER(MULTIAXES)
 */
int MRSNMC::checkBoardExistArgument(int iBoardID, void* pPointer, BOOL bBoardExist)
{
	short siBdNum;
	CString strLogMsg;

	/** 1. Motion Library �̻��� ���� */
	if (m_pMotionLib == NULL)
		return generateErrorCode(106024);	// ERR_MAXES_NO_EXIST_MOTION_LIBRARY_OBJECT

	/** 2. �μ� ������ ���� */
	if (pPointer == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** ������ Board �� �б� */
	m_pMotionLib->GetBoardNum(&siBdNum);

	/** 3. ������ Board ���� Data ���� ���� */
	if ((siBdNum < DEF_NON_MOTION_BD) || (siBdNum > DEF_MAX_MOTION_BD))
		return generateErrorCode(106032);	// ERR_MAXES_INVALID_MOTION_BOARD_NUMBER

	/** 4. Board ID Data ���� ���� */
	if ((iBoardID < DEF_NON_MOTION_BD - 1) || (iBoardID > DEF_MAX_MOTION_BD - 1))
		return generateErrorCode(106032);	// ERR_MAXES_INVALID_MOTION_BOARD_NUMBER

	if (bBoardExist == TRUE)
	{
		/** 5. Board ���� ���� Data ����ġ - ������ Board ���� */
		if (siBdNum == 0)
			return generateErrorCode(106025);	// ERR_MAXES_NO_EXIST_MOTION_BOARD_CONFIG

		/** 6. Board ���� ���� Data ����ġ - ��ġ�� Board���� ū �μ��� */
		if (iBoardID >(siBdNum - 1))
			return generateErrorCode(106022);	// ERR_MAXES_OVER_MOTION_BOARD_NUMBER
	}

	return ERR_MOTION_SUCCESS;
}
//__________________
/* End Of Code */