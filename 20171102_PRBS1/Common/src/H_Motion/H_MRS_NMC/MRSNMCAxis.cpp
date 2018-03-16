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
 *				     이 파일은 단축 구성을 위한 Class를 정의한다.
 *
 * @version $Revision$
 */
extern MPlatformOlbSystem	MOlbSystemPre;

MRSNMC::MRSNMC()
{
	/** 1축 구성 member variable 초기화 */
	m_SaxAxis.iAxisID			= 0;
	memset(m_SaxAxis.szName, 0, sizeof(m_SaxAxis.szName));
	m_SaxAxis.iCPhaseUseType	= DEF_USE_SWITCH_ONLY;
	m_SaxAxis.bOriginDir		= FALSE;
	m_SaxAxis.bOriginFineDir	= FALSE;
	m_SaxAxis.dScale			= 1.0;
	m_SaxAxis.iOriginPriority	= 0;
	m_SaxAxis.bAbsEncoder		= FALSE;

	/** SERCOS 관련 */
	m_SaxAxis.iRingNo			= 0;
	m_SaxAxis.iNodeNo			= 0;
	m_SaxAxis.iServoType		= 0;

	/** IO Component */
	m_plnkIO = NULL;

	/** Interlock IO Address */
	m_usiInterlockIOAddr		= 0;

	/** Interlock IO Check Value */
	m_bInterlockJudge			= 0;

	/** Timer 개체 생성 */
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
 *				 전달받은 인수로 Data를 설정하고, 내부 개체를 설정한다.
 *
 * @param	iObjectID		: Object ID
 * @param	iBoardType		: Motion Board Type (1=MMC Board, 2=MEI board, 3=PPC Board, ...)
 * @param	saxAxis			: 1축 구성 정보
 * @param	iErrorBase		: Error Code Start Number
 * @param	strFullFileName	: Log File Path & File Name
 * @param	ucLevel			: Log Level (None, Normal, Warning, Error)
 * @param	iDays			: Log 유지 일 수
 * @stereotype constructor
 */
MRSNMC::MRSNMC(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO, ISercosSetup* pSercosSetup)
: ICommon(commonData)
{
	int iResult;

	/** 1축 구성 member variable 초기화 */
	SetData(&datComponent);

	/** IO Component */
	m_plnkIO			= pIO;
	m_plnkSercosSetup	= pSercosSetup;
	m_pMotionLib = NULL;

	/** Interlock IO Address */
	m_usiInterlockIOAddr = 0;

	/** Interlock IO Check Value */
	m_bInterlockJudge	= 0;

	/** Timer 개체 생성 */
	m_bOriginFlag		= FALSE;
	m_iOriginStep		= DEF_ORIGIN_START_STEP;
	m_iOriginPrevStep	= DEF_ORIGIN_START_STEP;
	m_iOriginError		= ERR_MOTION_SUCCESS;
	m_ttOriginTimer		= MTickTimer();
	
	/** Component 초기화 */
	if ((iResult = Initialize (commonData, 0, m_SaxAxis)) != ERR_MOTION_SUCCESS)
	{
		// Error 처리
	}

	/** Timer 개체 생성 */
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
 *				내부 개체를 소멸한다.
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
	m_strFileName	= pData->m_strFileName;	//	데이타 파일 이름

	swprintf(m_SaxAxis.szName, _T("%s"), pData->m_sAxisVal.szName);
//	sprintf(m_SaxAxis.szName, "%s", pData->m_sAxisVal.szName);

	// 생성자를 통해 시스템에서 받은 데이타를 SAxis1 형태로 변환한다.
	m_SaxAxis.iAxisID			= pData->m_sAxisVal.iAxisID;
	m_SaxAxis.iOriginPriority	= pData->m_sAxisVal.iOriginPriority;
 	m_SaxAxis.bOriginDir		= pData->m_sAxisVal.bOriginDir;
	m_SaxAxis.bOriginFineDir	= pData->m_sAxisVal.bOriginFineDir;
 	m_SaxAxis.iCPhaseUseType	= pData->m_sAxisVal.iCPhaseUseType;
	m_SaxAxis.dScale			= pData->m_sAxisVal.dScale;
	m_SaxAxis.iRoleOfHome		= pData->m_sAxisVal.iRoleOfHome;
	m_SaxAxis.bAbsEncoder		= pData->m_sAxisVal.bAbsEncoder;

	// Serco 관련 
	m_SaxAxis.iRingNo			= pData->m_sAxisVal.iRingNo;
	m_SaxAxis.iNodeNo			= pData->m_sAxisVal.iNodeNo;
	m_SaxAxis.iServoType		= pData->m_sAxisVal.iServoType;
	
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::GetData(SSMotionData *pData)
{
	swprintf(pData->m_sAxisVal.szName, _T("%s"), m_SaxAxis.szName);
//	sprintf(pData->m_sAxisVal.szName, "%s", m_SaxAxis.szName);

	// 생성자를 통해 시스템에서 받은 데이타를 SAxis1 형태로 변환한다.
	pData->m_sAxisVal.iAxisID			= m_SaxAxis.iAxisID;
	pData->m_sAxisVal.iOriginPriority	= m_SaxAxis.iOriginPriority;	
 	pData->m_sAxisVal.bOriginDir		= m_SaxAxis.bOriginDir;		
	pData->m_sAxisVal.bOriginFineDir	= m_SaxAxis.bOriginFineDir;	
 	pData->m_sAxisVal.iCPhaseUseType	= m_SaxAxis.iCPhaseUseType;		
	pData->m_sAxisVal.dScale			= m_SaxAxis.dScale;			
	pData->m_sAxisVal.bAbsEncoder		= m_SaxAxis.bAbsEncoder;				

	// Serco 관련 
	pData->m_sAxisVal.iRingNo			= m_SaxAxis.iRingNo;
	pData->m_sAxisVal.iNodeNo			= m_SaxAxis.iNodeNo;
	pData->m_sAxisVal.iServoType		= m_SaxAxis.iServoType;

	return ERR_MOTION_SUCCESS;
}

int	MRSNMC::LoadParameter()
{
	int iResult = ERR_MOTION_SUCCESS;
	MAxisData	fileData(_T(""),m_strFileName);

	// 파일에서 데이타를 읽는다.
	fileData.SetSection(m_SaxAxis.szName);	// Section을 지정 한다.

	BOOL bReadSuccess	= fileData.ReadData();
	m_AxisParameter		= fileData;	// Component 형태로 저장한다.
	// 시스템에서 받은 데이타와 로드한 데이타를 가지고 Component를 초기화 한다.

	if (!bReadSuccess)
		return generateErrorCode(107908);	// ERR_MAXES_PARAMETER_LOAD_FAIL;

	return iResult;
}

// Component에 데이타를 저장한다. 이때 파일에 같이 저장한다.

int MRSNMC::SaveParameter(MAxisData *pData)
{
	int	iResult = ERR_MOTION_SUCCESS;

	MAxisData	fileData("",m_strFileName);

	// 파일에 저장 한다.
	m_AxisParameter = *pData;

	fileData = m_AxisParameter;	// 저장할 데이타를 복사한다.

	fileData.SetSection(m_SaxAxis.szName);	// Section을 지정 한다.
	if (fileData.WriteData() != TRUE)
		// 107909 = it's Failed to Save parameter.
		iResult = generateErrorCode(107909);	// ERR_MAXES_PARAMETER_SAVE_FAIL

	// hongju 수정. SetAxisData는 MMC2 Manager에서 모두 처리
	// 데이타를 Component 변수에 저장한다.
//	iResult = SetAxisData();

	return iResult;
}

/** 축 Parameter(DataManager)를 얻는다. */
void MRSNMC::GetParameter(MAxisData* pData)
{
	*pData	= m_AxisParameter;
}

/**
 * 축의 이름을 Return 한다.
 * @return : Axis Name
 */
CString MRSNMC::GetAxisName()
{
	CString strName;

	strName.Format(_T("%s"), m_SaxAxis.szName);
	return strName;
}

/**
 * Motion 동작을 즉시 정지할 Interlock 조건에 해당되는 IO Address와 기준값을 설정한다.
 *
 * @param	usiIOAddress : 설정할 Interlock IO Address
 * @param	bJudgeValue	: Interlock 판단할 값
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */
void MRSNMC::SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue)
{
	/** Interlock IO Address */
	m_usiInterlockIOAddr	= usiIOAddress;

	/** Interlock IO Check Value */
	m_bInterlockJudge		= bJudgeValue;
}

/**
 * Motion Component를 초기화한다.
 *
 * 1. Motion Board 종류에 맞는 Motion Library 개체를 생성한다.
 * 2. 축 구성 개수를 설정한다.
 * 3. 축 정보를 설정한다.
 *
 * @param	iObjectID		: Object ID
 * @param	iBoardType		: Motion Board Type (1=MMC Board, 2=MEI board, 3=PPC Board, ...)
 * @param	saxAxis			: 1축 구성 정보
 * @param	iErrorBase		: Error Code Start Number
 * @param	strFullFileName	: Log File Path & File Name
 * @param	ucLevel			: Log Level (None, Normal, Warning, Error)
 * @param	iDays			: Log 유지 일 수
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXES NUMBER (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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
	
	/** Data 하나씩 범위 검사하며 설정 */
	if ((iResult = m_pMotionLib->GetUseAxis(saxAxis.iNodeNo, &bUse)) != ERR_MOTION_SUCCESS)
		return iResult;

	if (bUse == TRUE)
		return generateErrorCode(106020);	// ERR_MAXES_USED_AXIS_ID

	/** 축 사용으로 설정 */
	if ((iResult = m_pMotionLib->SetUseAxis(saxAxis.iRingNo, saxAxis.iNodeNo, TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	if (m_SaxAxis.iRoleOfHome != HOME_N_NONE)
		SetHomeSensorEvent(DEF_E_STOP_EVENT);
	else
		SetHomeSensorEvent(DEF_NO_EVENT);

	SetPositiveSensorEvent(DEF_E_STOP_EVENT);
	SetNegativeSensorEvent(DEF_E_STOP_EVENT);

	// Motion Sampling Rater를 4msec로 설정
	m_pMotionLib->SetControlTimerForAx(saxAxis.iNodeNo, DEF_SAMPLING_4MSEC);
	m_pMotionLib->SetControlTimerForAx(saxAxis.iNodeNo, DEF_SAMPLING_4MSEC, TRUE);


	return iResult;
}
/**
 * 축 1개에 대한 Data를 설정한다. (구조체)
 *
 *	(1) 축 정보 영역이 있어야 한다.
 *	(2) 축이 이미 사용되고 있으면 안된다.
 *
 * @param   ax1Data         : 설정할 각 축의 설정 Data
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = USED AXIS ID (MULTIAXES)
 *							  xx = NO EXIST AXIS PARAMETER AREA (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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
 * 축 1개에 대한 Data를 읽는다. (구조체)
 *
 * @param   *pax1Data       : 설정할 각 축의 설정 Data
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::GetAxisData(SAxis1 *pax1Data)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 축 관련 오류 점검 */
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

	/** Board 구성 개수만큼 설정 */
	if (iBoardNo == DEF_ALL_MOTION_BD_ID)	// 전체 Board인 경우
	{
		/** 할당된 메모리에 설정 */
		if ((iResult = m_pMotionLib->GetBoardNum(&siTotalBoardNum)) != ERR_MOTION_SUCCESS)
			return iResult;

		for (int i = 0; i < siTotalBoardNum; i++)
		{
			if ((iResult = m_pMotionLib->SetSplAutoOff(i, pbAutoSet[i])) != ERR_MOTION_SUCCESS)
				return iResult;
		}
	}
	else	// 축 하나인 경우
	{
		if ((iResult = m_pMotionLib->SetSplAutoOff(iBoardNo, *pbAutoSet)) != ERR_MOTION_SUCCESS)
				return iResult;
	}

	return ERR_MOTION_SUCCESS;
}
/** 
 * Board에 대한 자동 가, 감속 사용여부를 읽는다.
 *(Library에 조회 기능이 없는 관계로 SetAutpCP() 사용시에만 조회와 실제동작이 일치함)
 *
 * @param   iBoardNo        : MMC Board 번호 0 ~ 7, -1 = All Board
 * @param   *pbAutoSet      : 자동 가,감속 설정여부, TRUE : 수동, FALSE : 자동, iBoardNo=-1이면 배열로 구성
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID MOTION BOARD ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::GetAutoCP(int iBoardNo, BOOL* pbAutoSet)
{
	int iResult = ERR_MOTION_SUCCESS;
	short siTotalBoardNum;

	if ((iResult = checkBoardExistArgument(iBoardNo,(void*)pbAutoSet, TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Board 구성 개수만큼 설정 */
	if (iBoardNo == DEF_ALL_MOTION_BD_ID)	// 전체 Board인 경우
	{
		/** 할당된 메모리에서 Data 읽기 */
		if ((iResult = m_pMotionLib->GetBoardNum(&siTotalBoardNum)) != ERR_MOTION_SUCCESS)
			return iResult;

		for (int i = 0; i < siTotalBoardNum; i++)
		{
			if ((iResult = m_pMotionLib->GetSplAutoOff(i,(pbAutoSet + i))) != ERR_MOTION_SUCCESS)
				return iResult;
		}
	}
	else	// 축 하나인 경우
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
	
	/** Thread 종료 */
	TerminateThread(m_hOriginThread, 0);

	/** 축 모두에 대해 VStop 명령 수행 */
	iResult = VStop();

	m_iOriginStep = DEF_ORIGIN_ERROR_STEP;
	//WriteErrorLog("StopReturnOrigin(): Origin Stop By User");
	// 107900 = Axis Origin is Stopped by User.
	//m_iOriginError = generateErrorCode(107900);	// ERR_MAXES_ORIGIN_STOP_BY_USER //170717 JSH

	return iResult;
}

/**
 * 축의 현재좌표를 읽는다.
 * 
 * @return   double   : 현재 좌표값
 * @param   bType                : 읽을 위치 종류, FALSE=실제위치, TRUE=목표위치 
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

	/** 축 관련 오류 점검 */
	iResult = checkAxisExistArgument(TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	double dPos;

	// hongju 수정...Sercos는 bType 필요 없음.
	dPos = dCurrentPosition * GetScale();

	/** 축의 목표위치 설정 */
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
 * 축의 현재좌표와 특정좌표간의 수치에 의한 좌표차이를 비교한다.
 * 
 * @param	bPosOpt              : 비교할 위치 종류, FALSE=현재위치, TRUE=Motion의 목표위치
 * @param   dTargetPosition      : 비교할 좌표값 
 * @param   *pdPermission        : (OPTION = NULL) 비교허용 오차, NULL이면 내부 Tolerance값으로 비교한다.
 * @param   *pdDeviation         : (OPTION = NULL) 비교 차이값
 * @return   BOOL	             : 비교결과
 */
BOOL MRSNMC::ComparePosition(BOOL bPosOpt, double dTargetPosition,
								  double *pdPermission, double *pdDeviation)
{	
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;

	/** 현재 위치 읽기 */
#ifdef SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition();

#endif

	/** 위치 차이 계산 */
	dDevPosition = dTargetPosition - dCurPosition;
	if (pdDeviation != NULL)
		*pdDeviation = dDevPosition;

	/** Judge 결정 */
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
 * 축의 현재좌표와 특정좌표간의 수치에 의한 좌표차이를 비교한다.
 * 
 * @param   dTargetPosition      : 비교할 좌표값	 
 * @param   *pdPermission        : (OPTION = NULL) 비교허용 오차, NULL이면 내부 Tolerance값으로 비교한다.
 * @return  BOOL	             : 비교결과, 비교 좌표가 현재 좌표 - Tolerance 보다 작으면 True, 아니면 False
 */
BOOL MRSNMC::IsLessThanTargetPosition(double dTargetPosition,
								 double *pdPermission)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;

	/** 현재 위치 읽기 */
#ifdef SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition();
#endif

	/** Judge 결정 */
	if (pdPermission != NULL)
	{
		/** 위치 차이 계산 */
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
 * 축의 현재좌표와 특정좌표간의 수치에 의한 좌표차이를 비교한다.
 * 
 * @param   dTargetPosition      : 비교할 좌표값	 
 * @param   *pdPermission        : (OPTION = NULL) 비교허용 오차, NULL이면 내부 Tolerance값으로 비교한다.
 * @return  BOOL	             : 비교결과, 비교할 좌표가 현재 좌표 + Tolerance 보다 크면 True, 아니면 False
 */
BOOL MRSNMC::IsGreaterThanTargetPosition(double dTargetPosition, double *pdPermission)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;

	/** 현재 위치 읽기 */
#ifdef SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition();
#endif

	/** Judge 결정 */
	if (pdPermission != NULL)
	{
		/** 위치 차이 계산 */
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
 * 축 이동 (한개의 축에 대한 이동) - 이동 완료된 후 return
 * 
 * @param   dPosition        : 이동할 위치
 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
 * @param	iMoveType        : (OPTION=1) 이동 Type,
 *											0=사다리꼴 속도 Profile, 절대좌표 이동
 *											1=S-Curve 속도 Profile, 절대좌표 이동
 *											4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
 *											5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
 * MoveType 결정 없음. ? 						
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = INVALID MOVE-TYPE (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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
 * 축 이동 (한개의 축에 대한 이동) - 이동 명령 후 바로 return
 * 
 * @param   dPosition        : 이동할 위치
 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
 * @param	iMoveType        : (OPTION=1) 이동 Type,
 *											0=사다리꼴 속도 Profile, 절대좌표 이동
 *											1=S-Curve 속도 Profile, 절대좌표 이동
 *											4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
 *											5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = INVALID MOVE-TYPE (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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

	/** 축 관련 오류 점검 */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}

	/** 이동 전 원점복귀 여부 확인 */
	if (!IsOriginReturn())
		// 107906 = Axis is not Origin.
		return generateErrorCode(107906);
	
	/** 축 상태 점검 */
	if ((iResult = checkAxisState(GetNodeID())) != ERR_MOTION_SUCCESS)
		return iResult;

	dPos = GetScale() * dPosition;

	/** 이동 속도 처리 */
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

	/** 이동 가속도 처리 */
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

	/** 이동 감속도 처리 */
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
		/* 20090203 jdy : jerk 에 대한 수정 내용 아직 반영 안됨. RAK 에서 Jerk 설정을 위한 API 추가 제공하기로 했음.
		                  20090119 정용국K 와 통화 완료. - RAK 협의 후 API 만들어서 연락 준다고 함.
						  아래 주석 내용대로 jerk 적용되지 않음.
		// 20 이라는 수치는 가속(or 감속) 구간의 총 20% (가속 시작 시 10%, 가속 종료 시 10%) 구간에
		// 대해서 jerk 를 적용한다는 의미라고 함.
		// 따라서, iJerk 값을 계산할 때는 가,감속 중 작은 쪽의 값을 가지고 계산하는 게 맞다.
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
		iJerk = INT32U(0);			// Curve 형태를 결정한다. (사다리꼴)          
	}
	
	/** No-wait 이동 */
	iResult = m_pMotionLib->Move(GetNodeID(), dPos, dVel, iAccel, iDecel, iMoveType);
	if(iResult != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}

	return ERR_MOTION_SUCCESS;
}

/**
 * 축 이동 (한개의 축에 대한 상대위치 이동) - 이동 완료된 후 return
 * 
 * @param   dDistance        : 이동할 거리
 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
 * @param	iMoveType        : (OPTION=3) 이동 Type,
 *											2=사다리꼴 속도 Profile, 상대거리 이동
 *											3=S-Curve 속도 Profile, 상대거리 이동
 *											6=비대칭 사다리꼴 속도 Profile, 상대거리 이동
 *											7=비대칭 S-Curve 속도 Profile, 상대거리 이동
 * @param	bClearOpt        : (OPTION=FALSE) 이동 전과 후에 Encoder 값을 Clear하는 동작 사용 여부 (TRUE:사용, FALSE:미사용)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::RMove(double dDistance, double dVelocity, int iAccelerate, int iDecelerate,
						int iMoveType, BOOL bClearOpt)
{
	int iResult = ERR_MOTION_SUCCESS;
	CString	strLogMsg;

	/** 축 관련 오류 점검 */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult != ERR_MOTION_SUCCESS)
		return iResult;

	/** 이동 전 원점복귀 여부 확인 */
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
 * 축 이동 (한개의 축에 대한 상대위치 이동) - 이동 명령 후 바로 return
 * 
 * @param   dDistance        : 이동할 거리
 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용	
 * @param	iMoveType        : (OPTION=3) 이동 Type,
 *											2=사다리꼴 속도 Profile, 상대거리 이동
 *											3=S-Curve 속도 Profile, 상대거리 이동
 *											6=비대칭 사다리꼴 속도 Profile, 상대거리 이동
 *											7=비대칭 S-Curve 속도 Profile, 상대거리 이동
 * @param	bClearOpt        : (OPTION=FALSE) 이동 전과 후에 Encoder 값을 Clear하는 동작 사용 여부 (TRUE:사용, FALSE:미사용)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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

	/** 축 상태 점검 */
	if ((iResult = checkAxisState(GetNodeID())) != ERR_MOTION_SUCCESS)
		return iResult;


	/** 위치 정리 */
	dPos = GetScale() * dDistance;

	/** 이동 속도 처리 */
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

	/** 이동 가속도 처리 */
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
	
	/** 이동 감속도 처리 */
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

	// Jerk 설정.
	if ((iMoveType == DEF_SMOVE_DISTANCE) || (iMoveType ==DEF_TSMOVE_DISTANCE))
	{
		/* 20090203 jdy : jerk 에 대한 수정 내용 아직 반영 안됨. RAK 에서 Jerk 설정을 위한 API 추가 제공하기로 했음.
		                  20090119 정용국K 와 통화 완료. - RAK 협의 후 API 만들어서 연락 준다고 함.
						  아래 주석 내용대로 jerk 적용되지 않음.
		// 20 이라는 수치는 가속(or 감속) 구간의 총 20% (가속 시작 시 10%, 가속 종료 시 10%) 구간에
		// 대해서 jerk 를 적용한다는 의미라고 함.
		// 따라서, iJerk 값을 계산할 때는 가,감속 중 작은 쪽의 값을 가지고 계산하는 게 맞다.
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
		iJerk = INT32U(0);			// Curve 형태를 결정한다. (사다리꼴)          
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

	/** No-wait 이동 */
	iResult = m_pMotionLib->Move(GetNodeID(), dPos, dVel, iAccel, iDecel, iMoveType, bClearOpt);

	if (bPLimit)	SetPositiveSensorEvent(DEF_E_STOP_EVENT);		
	if (bNLimit)	SetPositiveSensorEvent(DEF_E_STOP_EVENT);

	return iResult;
}
/**
 * 축 이동 (한개의 축에 대한 등속 이동, 등속 위치까진 가속 이동함) 
 * 
 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용	
 * @param   bDir             : (OPTION=TRUE) 이동할 방향, TRUE:(+), FALSE:(-), 생략하면 (+방향으로 이동
 *                             dVelocity에 값을 넣어주면 bDir은 생략해서 사용하면 된다.
 *                             이 경우는 dVelocity의 부호에 의해 이동 방향이 결정된다.
 *                             dVelocity에 0.0을 넣어 지정된 속도를 사용하는 경우는
 *                             bDir로 (+/-) 방향을 설정할 수 있다.
 *                             만약, dVelocity에 값을 넣은 경우 bDir을 설정을 하게 되면
 *                             지정된 dVelocuty, dAccelerate에 bDir이 반영되어 이동을 하게 된다.
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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

	/** 축 관련 오류 점검 */
	iResult = checkAxisExistArgument(TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;


	/** 축 상태 점검 */
	iResult = checkAxisState(GetNodeID());
	if(iResult != ERR_MOTION_SUCCESS)
	{
		return iResult;
	}

	/** 이동 속도 처리 */
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

	/** 이동 가속도 처리 */
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
	iAcc = INT32U(iAccel);	        // 가속구간 시간 -> mm/s^2
	iDec = INT32U(iAccel);	
	iJerk = INT32U(10);				// msec                    // 값 설정 하기 

	/** (+) 방향 */
	if (bDir == TRUE)
		dDir = 1;
	/** (-) 방향 */
	else
		dDir = -1;

	// 부호 결정 

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
 * 축 이동 후 완료를 확인한다. (한개의 축에 대한 완료 확인) 
 * 
 * @param   bSkipMode        : (OPTION=FALSE) 위치 확인 대기, 이동 후 안정화 시간 지연 사용 여부
 * @param	bUseInterlock    : (OPTION=FALSE) Interlock에 의한 동작 정지 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
 * @param	bRMove		     : (OPTION=FALSE) RMove시 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = TIMEOUT MOVE-TIME (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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
	// Y, T축이 Inposition signal이들어오지 않아서 임시 조치
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
				//-> Motion 정지
				Stop();
				return generateErrorCode(106066);	// ERR_MAXES_INTERLOCK_CHECKED
			}
		}

		//축 상태 읽기
		if (IsEStopEvent())//170713 JSH
		{
			// E-Stop Event 발생
			EStop();
			return generateErrorCode(106058);
		}

		//Amp Fault 확인
		if (IsAmpFault())
		{
			//Amp Fault 발생
			EStop();
			return generateErrorCode(106008);
		}

		if (m_ttMovingTimer.MoreThan(m_AxisParameter.dLimitTime))
		{
			//-> Motion 정지
			Stop();				
			return generateErrorCode(106021);	// ERR_MAXES_TIMEOUT_MOVE_TIME
		}
		Sleep(5);
	}

	// 확인 필요 : 이동 중 Servo Off 돼도 Error display 없음. Servo Off 되면 in position 신호도 뜨는 것 같음. 계속 진행 됨.
	// 따라서 여기서 Amp Falut, Amp Enable 상태 검사.
	iResult = checkAxisState(GetNodeID());
	if (iResult)
		return iResult;

	if (FALSE == bSkipMode)
		Sleep((DWORD)(m_AxisParameter.dStabilityTime * 1000));

	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 동작 완료 여부를 Return 한다. 
 * 이동 및 속도 이동 완료 확인 또는 더불어 In-Position 여부까지 확인한다.
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
 *	축이 이동 중 (속도 > 0) 인지를 Return 한다.
 *
 *	@return : TRUE  = 축 이동 중.
 *            FALSE = 축 정지 상태
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
 * 축의 동작 완료 여부를 Return 한다. 
 * 이동 및 속도 이동 완료 확인한다.  In-Position 확인안함
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
 * 직선보간 이동한다.
 * 
 * @param   iMaxPoint        : 직선보간 이동구간 개수
 * @param   *pdPosition      : 직선보간 이동구간 지정, iMaxPoint만큼 설정
 * @param   *pdVelocity      : 이동 시 속도, iMaxPoint만큼 설정 (Scale은 적용되지 않는다.)
 * @param   *piAccelerate    : 이동 시 가속도, iMaxPoint만큼 설정
 * @param	bAutoFlag        : Auto CP 여부, FALSE=AutoCP 사용안함, TRUE=AutoCP 사용함  /// 사용 안함 
 * @param	bWaitOpt         : (OPTION=TRUE)축 이동 완료 대기 여부, FALSE=대기 안함, TRUE=대기함
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  xx = NO EXIST AXIS ID TO MOVE (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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

	/** 축 관련 오류 점검 */
	iResult = checkAxisExistArgument((void*)pdPosition, (void*)pdVelocity, (void*)piAccelerate, TRUE);
	if(iResult)	return iResult;

	/** 이동 전 원점복귀 여부 확인 */
	if (!IsOriginReturn())
		return generateErrorCode(107906);

	/** 이동 전 축 Event State 확인 */
	if ((iResult = checkAxisState(GetNodeID())) != ERR_MOTION_SUCCESS)
		return iResult;

	/** 사용할 축 추출 */
	dScale = GetScale();

	/** 이동 Point에 따라 */
	for (int i = 0; i < iMaxPoint; i++)
	{
		/** 위치 정리 */
		rgdPos[i]	= INT32S(dScale * pdPosition[i]);
		rgdVel[i]	= INT32U(dScale * pdVelocity[i]);
		rgdAcc[i]	= INT32U((dScale * pdVelocity[i]) / ((double)piAccelerate[i]/1000));
		iJerk		= INT32U(10);					// msec                  //  값 설정 하기 
		
		/** SW Limit Check */
		if ((iResult = checkSWLimit(pdPosition[i])) != ERR_MOTION_SUCCESS)
			return iResult;
	}

	// 이동 Point에 따라 
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
		/** Spline Line 이동 완료될 때까지 대기 */
		if (m_pMotionLib->WaitForDone(GetNodeID()) != ERR_MOTION_SUCCESS)
			return iResult;
	}
	
	return ERR_MOTION_SUCCESS;
}

/**
 * Jog Slow에 의한 이동한다.
 * 
 * @param   bDir             : 이동할 방향, TRUE:(+), FALSE:(-)
 * @param   dSlow           : (OPTION = 0.0) 이동할 거리, 0.0 = 지정된 Slow거리 사용
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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

	/** 축 관련 오류 점검 */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult)	return iResult;

	/** 이동 속도 처리 */
	if (0.0 == dSlow)
		dJogSlow = GetScale() * m_AxisParameter.dJogSlow;
	else
		dJogSlow = GetScale() * dSlow;

	/** 이동 가속도 처리 */
	iAccel = INT32U((dJogSlow) / ((double)GetMovingAccelerate()/1000));

	/** 이동 감속도 처리 */
	iDecel = iAccel;
	
	iVel = INT32S(dJogSlow);				// mm/sec -> Pulse
	iAcc = INT32U(iAccel);		
	iDec = INT32U(iDecel);	
	iJerk = INT32U(0);						// msec                    // 값 설정 하기 

	/** (+) 방향 */
	if (bDir == TRUE)
		dDir = 1;
	/** (-) 방향 */
	else
		dDir = -1;

	// 부호 결정 
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
 * Jog Fast에 의한 이동한다.
 * 
 * @param   bDir		: 이동할 방향, TRUE:(+), FALSE:(-)
 * @param   dFast		: (OPTION = 0.0) 이동할 속도, 0.0 = 지정된 속도 사용
 * @return	Error Code	: 0 = SUCCESS
 *					 	  xx = INVALID AXIS ID (MULTIAXES)
 *					 	  xx = INVALID POINTER (MULTIAXES)
 *						  그 외 = 타 함수 Return Error
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
	/** 축 관련 오류 점검 */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult)	return iResult;

	/** 이동 속도 처리 */
	if (0.0 == dFast)
		dJogFast = GetScale() * m_AxisParameter.dJogFast;
	else
		dJogFast = GetScale() * dFast;

	/** 이동 가속도 처리 */
	iAccel = INT32U((dJogFast) / ((double)GetMovingAccelerate()/1000));

	/** 이동 감속도 처리 */
	iDecel = iAccel;
	
	iVel = INT32S(dJogFast);				// mm/sec -> Pulse
	iAcc = INT32U(iAccel);		
	iDec = INT32U(iDecel);	
	iJerk = INT32U(0);						// msec                    // 값 설정 하기 

	/** (+) 방향 */
	if (bDir == TRUE)
		dDir = 1;
	/** (-) 방향 */
	else
		dDir = -1;

	// 부호 결정 
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
 * Jog 이동을 정지 시킨다.
 * 	 
 * @return	Error Code		 : 0 = SUCCESS
 *							  그 외 =  Error Code
 */
int MRSNMC::JogStop()
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 축 관련 오류 점검 */
	iResult = checkAxisExistArgument(TRUE);
	if(iResult)	return iResult;

	double dDecel;
	dDecel = (m_SaxAxis.dScale * m_AxisParameter.dRunVelocity) / (((double)m_AxisParameter.iRunAccelerate / 1000));

	iResult = m_pMotionLib->SetStop(GetNodeID(), DEF_VSTOP, dDecel, 0);
	if(iResult)	return iResult;

	return ERR_MOTION_SUCCESS;
}

/**
 * 축을 정지한다. (한개의 축에 대한 정지)
 * 
 * @param   *pbStatus      : (OPTION = NULL) 축의 Stop 상태
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
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

	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

/**
 * 축을 등속이동에 대해 정지한다. (한개의 축의 등속이동에 대한 정지)
 * 
 * @param   *pbState         : (OPTION = NULL) 축의 VStop 상태
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::VStop(BOOL *pbStatus)
{
	CString strLog;
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	/** 축 관련 오류 점검 */
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

	/** VStop 후 안정화 시간 적용 */
	//Sleep((ULONG)(m_AxisParameter.dStabilityTime));

	/** 결과 기록 */
	if(pbStatus != NULL)
		*pbStatus = TRUE;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * 축을 비상정지한다. (한개의 축에 대한 비상정지)
 * 
 * @param   *pbStatus        : (OPTION = NULL) 축의 EStop 상태
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::EStop(BOOL *pbStatus)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	
	/** 축 관련 오류 점검 */
	int iResult = checkAxisExistArgument(TRUE);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	/** E-STOP */
	double dDecel;
	dDecel = (m_SaxAxis.dScale * m_AxisParameter.dSlowRunVelocity) / (((double)m_AxisParameter.iRunAccelerate / 1000));
	
	iResult = m_pMotionLib->SetStop(GetNodeID(), DEF_ESTOP, dDecel, 0);
	if(iResult)	return iResult;

	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 Servo를 On 한다. (한개의 축에 대한 Servo On 수행)
 * 
 * @param   *pbStatus        : (OPTION = NULL) 축의 Servo ON 상태
 * @param   bLinearAxis        : (OPTION = FALSE) 축이 Linear Type이면 TRUE, 그외 FALSE
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
// Doolin kwangilshin 2017. 08. 27.
// Modify - Tact Time 개선
//
int MRSNMC::ServoOn(BOOL *pbStatus, BOOL bIsCheck)
{
	int			iResult		= ERR_MOTION_SUCCESS;
	BOOL		bEnable		= DEF_DISABLE;

	MTickTimer	clsTimer;

	/** 축 관련 오류 점검 */
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
 * 축의 Servo를 Off 한다. (한개의 축에 대한 Servo Off 수행)
 * 
 * @param   *pbStatus        : (OPTION = NULL) 축의 Servo OFF 상태
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
// Doolin kwangilshin 2017. 08. 27.
// Modify - Tact Time 개선
//
int MRSNMC::ServoOff(BOOL *pbStatus, BOOL bIsCheck)
{
	int			iResult		= ERR_MOTION_SUCCESS;
	BOOL		bEnable		= DEF_ENABLE;
	
	MTickTimer	clsTimer;
	
	/** 축 관련 오류 점검 */
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
 * 축의 Home Sensor 상태를 읽는다. (한개의 축에 대한 상태읽기)
 * 
 * @return   BOOL        : 축의 Home Sensor 상태 
 */
BOOL MRSNMC::IsOnHomeSensor()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	ASSERT(m_pMotionLib != NULL);

	BOOL bStatus = FALSE;

	/** Home Sensor 상태 읽기 */
	m_pMotionLib->GetSensorStatus(GetNodeID(), DEF_HOME_SENSOR, &bStatus);
	
	return bStatus;	
}

/**
 * 축의 Positive Sensor 상태를 읽는다. (한개의 축에 대한 상태읽기)
 * 
 * @return   BOOL        : 축의 Positive Sensor 상태 
 */
BOOL MRSNMC::IsOnPositiveSensor()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	ASSERT(m_pMotionLib != NULL);

	BOOL bStatus = FALSE;

	/** Positive Sensor 상태 읽기 */
	m_pMotionLib->GetSensorStatus(GetNodeID(), DEF_POSITIVE_SENSOR, &bStatus);
	
	return bStatus;	
}

/**
 * 축의 Negative Sensor 상태를 읽는다. (한개의 축에 대한 상태읽기)
 * 
 * @return   BOOL        : 축의 Negative Sensor 상태 
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
 * 기능없음 
 * 축의 상태(Source)를 읽는다. 
 * 
 * @return   SUCCESS
 */
//int MRSNMC::GetAxisSource()
int MRSNMC::GetAxisSource(int *piReturn)
{
	short iStatus;

	m_pMotionLib->GetAxisRunStop(0, &iStatus);

	/** 축 Source 상태 읽기 */
	int iResult = m_pMotionLib->GetAxisSource(GetNodeID(), &iStatus);
	if (iResult)
		return iResult;

	*piReturn = (int)iStatus;
	return ERR_MOTION_SUCCESS;
}

/**
 * 기능없음 
 * 축의 상태(State)를 읽는다. 
 * 
 * @return   SUCCESS
 */
//int MRSNMC::GetAxisState(int *piReturn)
int MRSNMC::GetAxisState(UINT32 *piReturn) //170713 JSH
{
	//short iStatus;
	UINT32 iStatus;
	/** 축 Source 상태 읽기 */
	int iResult = m_pMotionLib->GetAxisState(GetNodeID(), &iStatus);
	if (iResult)
		return iResult;
	
	//*piReturn = (int)iStatus;
	*piReturn = iStatus; //170713 JSH
	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 AMP Enable 상태를 읽는다. 
 * 
 * @param   BOOL        : 축 하나에 대한 AMP상태 (AMP Enable : TRUE, 그외 : FALSE) 
 */
BOOL MRSNMC::IsAmpEnable()
{	

#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	BOOL bStatus = FALSE;

	/** AMP Enable 상태 읽기 */
	m_pMotionLib->GetAmpEnable(GetNodeID(), &bStatus);

	return bStatus;
}

/**
 * 축의 AMP Enable/Disable 상태로 만든다.
 * 
 * @param   bEnable      : AMP Enable 상태로 변경 : TRUE
 *						   AMP Disable 상태로 변경 : FALSE
 * @param   bLinearAxis  : (OPTION = FALSE) 축이 Linear Type이면 TRUE, 그외 FALSE
 * @return	Error Code	 : 0 = SUCCESS
 *						   xx = INVALID AXIS ID (MULTIAXES)
 *						   xx = INVALID POINTER (MULTIAXES)
 *						   그 외 = 타 함수 Return Error
 */
int MRSNMC::SetAmpEnable(BOOL bEnable)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	
	INT32S	iErr;

	/** 축 관련 오류 점검 */
	if ((iErr = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iErr;

	/** AMP Fault 상태 변경 */
	if ((iErr = m_pMotionLib->SetAmpEnable(GetNodeID(), bEnable)) != ERR_MOTION_SUCCESS)
		return iErr;

	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 AMP Fault 상태를 읽는다. 
 * 
 * @return   BOOL         : 축에 대한 AMP Fault상태 (AMP Fault:TRUE, Fault아니면:FALSE)
 */
BOOL MRSNMC::IsAmpFault()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	BOOL bStatus;

	// AMP Fault 상태 읽기
	m_pMotionLib->GetAmpFaultStatus(GetNodeID(), &bStatus);
	
	return bStatus;
}

/**
 * 축의 AMP Fault 상태를 Reset 한다. 
 * 
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::ResetAmpFault(BOOL bIsCheck)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	int			iResult		= ERR_MOTION_SUCCESS;
	MTickTimer	clsTimer;

	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** AMP Fault 상태 설정 */
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
 * 축의 AMP Fault 상태를 Clear/Enable 한다. 
 * 
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::SetAmpFaultEnable()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	int iResult = ERR_MOTION_SUCCESS;

	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** AMP Fault 상태 설정 */
	if ((iResult = m_pMotionLib->SetAmpFaultEnable(GetNodeID(), TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 상태를 초기화 한다. (한개의 축에 대해 초기화)
 *  Clear Status & Clear Frames
 * 
 * @param   *pbStatus        : (OPTION = NULL) 축의 초기화 상태
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::ClearAxis(BOOL *pbStatus)
{
#ifdef SIMULATION
	if (NULL != pbStatus)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
#endif

	int iResult = ERR_MOTION_SUCCESS;

	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** 축 상태 초기화 */
	if ((iResult = m_pMotionLib->ClearStatus(GetNodeID())) != ERR_MOTION_SUCCESS)
	{
		/** 결과 기록 */
		if (pbStatus != NULL)
			*pbStatus = FALSE;

		return iResult;
	}

	/** 축 Frame 초기화 */
	if ((iResult = m_pMotionLib->ClearFrames(GetNodeID())) != ERR_MOTION_SUCCESS)
	{
		/** 결과 기록 */
		if (pbStatus != NULL)
			*pbStatus = FALSE;

		return iResult;
	}

	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

/** 
 * 원점복귀 우선 순위를 Return 한다.
 *
 * @return	우선 순위		: 0~63
 */
int MRSNMC::GetOriginPriority()
{
	return m_SaxAxis.iOriginPriority;
}

/** 
 * 원점복귀 동작 수행 함수(한번 호출에 한 Step씩 수행)
 * 원점 복귀 Step이 완료 Step이 될 때 까지 계속 호출 해야함
 *
 * @param   iOriginMoveType :	DEF_ORIGIN_RETURN_NONE_MOVE (0) -> Motion 동작 없이 현재 위치를 원점으로 설정하는 경우.
 *								DEF_ORIGIN_RETURN_ALL_MOVE	(1) -> 원점 복귀 Offset 이동 동작을 포함한 모든 원점 복귀 관련 Motion 동작을 수행하는 경우.
 *								DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE	(2) -> 원점 복귀 Offset 이동 동작을 제외한 원점 복귀 관련 Motion 동작을 수행하는 경우.
 * @return	OriginStep
 */
// Doolin kwangilshin 2017. 08. 27.
// Function Modify - Tact Time 개선.
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


		case 750: // IsHoming 상태 확인  
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

//100712.KKY.OriginReturn함수로 대응이 안되는 축의 원점복귀_____
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

		// 축에 대한 상태 체크 후 처리.
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
			
			// 107416 = Event 상태를 해제해 주세요.
			return generateErrorCode(107416);
		}
		
		// 죽어 있는 경우는 별도 처리 불필요.
		// Amp Fault 상태
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
				
				// 107417 = Fault Reset 안됨.
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
		//@수정요망 MultiToolChange 사용여부에 따라 수정 필요 
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

	// C상 사용 유무 및 방향 설정
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

	// 원점복귀 제한시간과 제한 거리 설정.
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

	//Home센서 감지 되어 있는지 판단
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

	//Home센서 미감지시 -방향으로 이동
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

	//-방향으로 이동중 Home센서 확인
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
			// 원점복귀 이동시간을 초과했습니다.
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

	//Home센서감지 확인
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

	//Home센서 감지후 +방향으로 이동
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

	//Home센서 벗어났는지 확인
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
			// 원점복귀 이동시간을 초과했습니다.
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
	
	//-방향으로 이동
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

	//Home센서 감지 확인
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
			// 원점복귀 이동시간을 초과했습니다.
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
			// 원점복귀 이동시간을 초과했습니다.
			m_iOriginError = generateErrorCode(107415);
			EStop();
			SetOriginStep(998);	
			break;
		}
		break;

// Position Set 코드 적용.
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
 * 원점복귀 Step을 설정한다. (한개의 축)
 * 
 * @param   iStep            : 설정값 (0:시작, 999:오류, 1000:완료, 그외:동작중)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID ORIGIN STEP (<0) (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::SetOriginStep(int iStep)
{
	m_iOriginPrevStep = m_iOriginStep;
	m_iOriginStep = iStep;

	return ERR_MOTION_SUCCESS;
}

/**
 * 원점복귀 Step을 읽는다. (한개의 축)
 * 
 * @return	Origin Step
 */
int MRSNMC::GetOriginStep()
{
	return m_iOriginStep;
}

/**
 * 원점복귀 Error를 초기화한다. (한개의 축)
 * 
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::ClearOriginError()
{
	int iResult = ERR_MOTION_SUCCESS;

	m_iOriginError = ERR_MOTION_SUCCESS;

	return iResult;
}

/**
 * 원점복귀 Error를 읽는다. (한개의 축)
 * 
 * @param   *piError         : 발생한 오류 Code
* @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::GetOriginError(int *piError)
{
	int iResult = ERR_MOTION_SUCCESS;

	*piError = m_iOriginError;

	return iResult;
}

/**
 * Home Sensor의 역할을 반환 한다.
 *  	 
 * @return	ERoleOfHome		HOME_N_NONE : Home Sensor 역할
							HOME_N_POSITIVE_LIMIT : Home, Positive Sensor 역할
							HOME_N_NEGATIVE_LIMIT : Home, Negative Sensor 역할
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
 * SAxis1 Data에 대한 오류 점검과 개체 복사를 수행한다.
 *
 * @param	srcAx			: 오류 점검할 원본 Data
 * @param	*pdstAx			: (OPTION=NULL) 복사본 Data
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

	/** 축 ID 범위 점검 */
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
				// Home에서 E-Stop 발생 했을 때 반대 Jog 동작을 위해 Event 해제
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
		//Home에서 E-Stop 발생 했을 때 반대 Jog 동작을 위해 Event 해제 한 후
		//Home Sensor가 벗어나면 다시 E-Stop Event를 걸기 위해...
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

	// 동기 제어 축이면 슬레이브축도 체크한다.
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
 * 이동할 위치가 SW Limit을 벗어나는지 확인한다.
 * 
 * @param   dPosition		: 검사할 위치
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

// NMC Library가 리턴하는 에러 코드를 System Error형태로 만든다
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

	default										:	iError = 999;		break; //			// Error Code 없음 
	}

	// XXX XXX      XXX
	//   7 NMCError 인스턴스번호
	return generateErrorCode(107000 + iError);
}


/**
 * 축의 동작중 속도명령값을 PULSE값으로 돌려준다.
 *
 * @return	해당 Axis의 ComVelocity
 */
int MRSNMC::GetComVelocity()
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 Home Sensor에 대한 Event를 설정한다.(한개의 축에 대한 상태 설정)
 * 
 * @param	iLimit           : 동작할 Event(0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::SetHomeSensorEvent(int iLimit)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 Home Sensor에 대한 Level을 설정한다.(한개의 축에 대한 상태 설정)
 * 
 * @param	bLevel           : 신호 Level(TRUE=HIGH, FLASE=LOW)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::SetHomeSensorLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Home Sensor Event/Level 설정하기 */
	if ((iResult = m_pMotionLib->SetSensorLevel(GetNodeID(), 
		                                        DEF_HOME_SENSOR, bLevel)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 Positive Sensor에 대한 Event를 설정한다.(한개의 축에 대한 상태읽기)
 * 
 * @param	iLimit           : 동작할 Event(0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::SetPositiveSensorEvent(int iLimit)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Home Sensor Event/Level 설정하기 */
	if ((iResult = m_pMotionLib->SetSensorEvent(GetNodeID(), 
		                                        DEF_POSITIVE_SENSOR, 
												(short)iLimit)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 Positive Sensor에 대한 Limit를 설정한다.(한개의 축에 대한 상태읽기)
 * 
 * @param	bLevel           : 신호 Level(TRUE=HIGH, FLASE=LOW)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::SetPositiveSensorLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Home Sensor Event/Level 설정하기 */
	if ((iResult = m_pMotionLib->SetSensorLevel(GetNodeID(), 
		                                        DEF_POSITIVE_SENSOR, 
												bLevel)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 Negative Sensor에 대한 Event 설정한다.(한개의 축에 대한 상태읽기)
 * 
 * @param	iLimit           : 동작할 Event(0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::SetNegativeSensorEvent(int iLimit)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** Home Sensor Event/Level 설정하기 */
	
	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Home Sensor Event/Level 설정하기 */
	if ((iResult = m_pMotionLib->SetSensorEvent(GetNodeID(), 
		                                        DEF_NEGATIVE_SENSOR, 
												(short)iLimit)) != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

/**
 * 축의 Negative Sensor에 대한 Level를 설정한다.(한개의 축에 대한 상태읽기)
 * 
 * @param	bLevel           : 신호 Level(TRUE=HIGH, FLASE=LOW)
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MULTIAXES)
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::SetNegativeSensorLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;
	
	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	iResult = m_pMotionLib->SetAmpEnable(GetNodeID(),FALSE);
	if(iResult)
		return iResult;

	/** Home Sensor Event/Level 설정하기 */
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

	//MMC용 함수
	
	return ERR_MOTION_SUCCESS;
}

int MRSNMC::SetIndexRequired(BOOL bIndexReq)
{
	int iResult = ERR_MOTION_SUCCESS;

	//MMC용 함수
	
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

	// 가감속 구간에서의 이동 거리
	double dAccDecelMovingDistance = dVel * dAccelateTime / 2 + dVel * dDecelateTime / 2;
	if (dAccDecelMovingDistance >= dMovingDistance)
	{
		// 확인 필요 : 임시 코드. 정확한 계산은 아님. 그냥 대충 가감속 시간만큼만 먹는다고 생각.

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
 * Stop(), EStop() 함수 실행시의 감속도를 설정.
 * 개별축의 Run 가속도 기준 1/3 로 설정.
 *
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID(MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
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
	// 원점 완료 체크
	iResult = m_pMotionLib->GetOriginCompleteFlag(GetNodeID(), &HomeFlag);
	if(iResult)	return FALSE;

	if(HomeFlag == 3)
		return TRUE;
	else return FALSE;
}

void MRSNMC::ResetOriginComplete()
{
	// 원점 복귀가 완료 되지 않은 걸로 인식해서 원점 복귀를 다시 하도록 함....
	//smmc_set_pos_redefine_status(GetRingID(), GetNodeID(), POS_REDEFINE_EXTERNAL_APP);
	//SJ_YYK 160317 Modify.
	m_pMotionLib->ResetOriginComplete(GetNodeID());	
}

// 현재위치에서 Home Sensor 까지의 거리.
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

	/** 축 관련 오류 점검 */
	if ((iResult = checkAxisExistArgument(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** 축 Frame 초기화 */
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
 * 축 구성에 대한 오류를 점검한다.(포인터 인수 없음)
 *  1. Motion Library 개체 생성 여부
 *  2. 축 ID가 정상범위를 벗어난 경우
 *  축이 구성되어있어야하는 경우
 *    3. 설정된 축이 없는 경우
 *    4. 축 ID가 구성된 축의 범위를 벗어난 경우
 *    5. 설정된 축에 대한 축 설정정보 개체 생성 여부
 *
 * @param   bAxisExist      : 축 구성되어있는지 점검 여부, TRUE=구성되어있어야한다.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = NO-EXIST MOTION LIBRARY OBJECT(MULTIAXES)
 *							  xx = INVALID MOTION AXIS ID(MULTIAXES)
 *							  xx = NO-EXIST MOTION AXIS CONFIG(MULTIAXES)
 *							  xx = OVER MOTION AXIS NUMBER(MULTIAXES)
 *							  xx = NO-EXIST MOTION AXIS AREA POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::checkAxisExistArgument(BOOL bAxisExist)
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif

	/** 1. Motion Library 미생성 오류 */
	if (m_pMotionLib == NULL)
		return generateErrorCode(106024);	// ERR_MAXES_NO_EXIST_MOTION_LIBRARY_OBJECT

	/** 축 구성되어있어야 한다. */
	if (bAxisExist == TRUE)
	{
		/** 2. 축이 사용하고 있어야 한다. */
		BOOL bStatus = FALSE;
		m_pMotionLib->GetUseAxis(GetNodeID(), &bStatus);
		if (bStatus == FALSE)
			return generateErrorCode(106026);	// ERR_MAXES_NO_EXIST_MOTION_AXIS_CONFIG
	}

	return ERR_MOTION_SUCCESS;
}
/** 
 * 축 구성에 대한 오류를 점검한다.(포인터 인수 1개)
 *  1. 전달된 포인터 인수 정상 여부
 *  2. 기타 오류 점검
 *
 * @param   *pPointer       : Pointer로 전달된 인수
 * @param   bAxisExist      : 축 구성되어있는지 점검 여부, TRUE=구성되어있어야한다.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer, BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. 인수 포인터 오류 */
	if (pPointer == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. 축 구성 Index ID 범위 오류 */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * 축 구성에 대한 오류를 점검한다.(포인터 인수 2개)
 *  1. 전달된 포인터 인수1 정상 여부
 *  2. 전달된 포인터 인수2 정상 여부
 *  3. 기타 오류 점검
 *
 * @param   *pPointer1      : Pointer로 전달된 인수1
 * @param   *pPointer2      : Pointer로 전달된 인수2
 * @param   bAxisExist      : 축 구성되어있는지 점검 여부, TRUE=구성되어있어야한다.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer1, void* pPointer2, BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. 인수 포인터1 오류 */
	if (pPointer1 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. 인수 포인터2 오류 */
	if (pPointer2 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 3. 축 구성 Index ID 범위 오류 */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * 축 구성에 대한 오류를 점검한다.(포인터 인수 3개)
 *  1. 전달된 포인터 인수1 정상 여부
 *  2. 전달된 포인터 인수2 정상 여부
 *  3. 전달된 포인터 인수3 정상 여부
 *  4. 기타 오류 점검
 *
 * @param   *pPointer1      : Pointer로 전달된 인수1
 * @param   *pPointer2      : Pointer로 전달된 인수2
 * @param   *pPointer3      : Pointer로 전달된 인수3
 * @param   bAxisExist      : 축 구성되어있는지 점검 여부, TRUE=구성되어있어야한다.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer1, 
								  void* pPointer2, 
								  void* pPointer3,
								  BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. 인수 포인터1 오류 */
	if (pPointer1 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. 인수 포인터2 오류 */
	if (pPointer2 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 3. 인수 포인터3 오류 */
	if (pPointer3 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 4. 축 구성 Index ID 범위 오류 */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * 축 구성에 대한 오류를 점검한다.(포인터 인수 4개)
 *  1. 전달된 포인터 인수1 정상 여부
 *  2. 전달된 포인터 인수2 정상 여부
 *  3. 전달된 포인터 인수3 정상 여부
 *  4. 전달된 포인터 인수4 정상 여부
 *  5. 기타 오류 점검
 *
 * @param   *pPointer1      : Pointer로 전달된 인수1
 * @param   *pPointer2      : Pointer로 전달된 인수2
 * @param   *pPointer3      : Pointer로 전달된 인수3
 * @param   *pPointer4      : Pointer로 전달된 인수4
 * @param   bAxisExist      : 축 구성되어있는지 점검 여부, TRUE=구성되어있어야한다.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer1, 
								  void* pPointer2, 
								  void* pPointer3,
								  void* pPointer4,
								  BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. 인수 포인터1 오류 */
	if (pPointer1 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. 인수 포인터2 오류 */
	if (pPointer2 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 3. 인수 포인터3 오류 */
	if (pPointer3 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 4. 인수 포인터4 오류 */
	if (pPointer4 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 5. 축 구성 Index ID 범위 오류 */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * 축 구성에 대한 오류를 점검한다.(포인터 인수 4개)
 *  1. 전달된 포인터 인수1 정상 여부
 *  2. 전달된 포인터 인수2 정상 여부
 *  3. 전달된 포인터 인수3 정상 여부
 *  4. 전달된 포인터 인수4 정상 여부
 *  5. 전달된 포인터 인수5 정상 여부
 *  6. 기타 오류 점검
 *
 * @param   *pPointer1      : Pointer로 전달된 인수1
 * @param   *pPointer2      : Pointer로 전달된 인수2
 * @param   *pPointer3      : Pointer로 전달된 인수3
 * @param   *pPointer4      : Pointer로 전달된 인수4
 * @param   *pPointer5      : Pointer로 전달된 인수5
 * @param   bAxisExist      : 축 구성되어있는지 점검 여부, TRUE=구성되어있어야한다.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER(MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
int MRSNMC::checkAxisExistArgument(void* pPointer1, 
								  void* pPointer2, 
								  void* pPointer3,
								  void* pPointer4, 
								  void* pPointer5, 
								  BOOL bAxisExist)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 1. 인수 포인터1 오류 */
	if (pPointer1 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 2. 인수 포인터2 오류 */
	if (pPointer2 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 3. 인수 포인터3 오류 */
	if (pPointer3 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 4. 인수 포인터4 오류 */
	if (pPointer4 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 5. 인수 포인터5 오류 */
	if (pPointer5 == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 6. 축 구성 Index ID 범위 오류 */
	if ((iResult = checkAxisExistArgument(bAxisExist)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}
/** 
 * 축 구성에 대한 오류를 점검한다.
 *  1. Motion Library 개체 생성 여부
 *  2. 전달된 포인터 인수 정상 여부
 *  3. 구성된 Board 개수가 정상범위를 벗어난 경우
 *  4. Board ID가 정상범위를 벗어난 경우
 *  Board가 구성되어있어야하는 경우
 *    5. 설정된 Board이 없는 경우
 *    6. Board ID가 구성된 Board의 범위를 벗어난 경우
 *
 * @param   iBoardID        : Board ID, -1 = All Board
 * @param   *pPointer       : Pointer로 전달된 인수
 * @param   bBoardExist     : Board 구성되어있는지 점검 여부, TRUE=구성되어있어야한다.
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

	/** 1. Motion Library 미생성 오류 */
	if (m_pMotionLib == NULL)
		return generateErrorCode(106024);	// ERR_MAXES_NO_EXIST_MOTION_LIBRARY_OBJECT

	/** 2. 인수 포인터 오류 */
	if (pPointer == NULL)
		return generateErrorCode(106028);	// ERR_MAXES_INVALID_POINTER

	/** 구성된 Board 수 읽기 */
	m_pMotionLib->GetBoardNum(&siBdNum);

	/** 3. 구성된 Board 개수 Data 범위 오류 */
	if ((siBdNum < DEF_NON_MOTION_BD) || (siBdNum > DEF_MAX_MOTION_BD))
		return generateErrorCode(106032);	// ERR_MAXES_INVALID_MOTION_BOARD_NUMBER

	/** 4. Board ID Data 범위 오류 */
	if ((iBoardID < DEF_NON_MOTION_BD - 1) || (iBoardID > DEF_MAX_MOTION_BD - 1))
		return generateErrorCode(106032);	// ERR_MAXES_INVALID_MOTION_BOARD_NUMBER

	if (bBoardExist == TRUE)
	{
		/** 5. Board 개수 설정 Data 미일치 - 설정할 Board 없음 */
		if (siBdNum == 0)
			return generateErrorCode(106025);	// ERR_MAXES_NO_EXIST_MOTION_BOARD_CONFIG

		/** 6. Board 개수 설정 Data 미일치 - 설치된 Board보다 큰 인수값 */
		if (iBoardID >(siBdNum - 1))
			return generateErrorCode(106022);	// ERR_MAXES_OVER_MOTION_BOARD_NUMBER
	}

	return ERR_MOTION_SUCCESS;
}
//__________________
/* End Of Code */