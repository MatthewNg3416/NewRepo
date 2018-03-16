
#include "stdafx.h"
#include "math.h"
#include "MACSAxis.h"
#include "IIO.h"
#include "common.h"
#include "atlconv.h"

#include "ACSC.h"
#pragma comment(lib, "ACSCL")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL MACSAxis::m_rgbACSAxisUseFlag = FALSE;

SACSCommSet	MACSAxis::m_sCommSet;

MACSAxis::MACSAxis()
{
	m_sCommSet.Handle = ACSC_INVALID;
	m_sCommSet.strIPAddr = "10.0.0.100";		//default address
	m_sCommSet.m_bConnect = FALSE;

	/** For ACS_SIMULATION */
	m_dCurrentAxisPos = 0.0;
	m_dTargetAxisPos = 0.0;
	m_dRMoveDistance = 0.0;
	m_dVelocity = 0.0;
	m_iAccelerate = 0;
	m_iDecelerate = 0;
	m_bRMoveStart = FALSE;
	m_MotionTimer.StopTimer();
}

MACSAxis::MACSAxis(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO, SACSCommSet sCommSet)
: ICommon(commonData)
{
	int iResult;

	/** 1축 구성 member variable 초기화 */
	SetData(&datComponent, &sCommSet);

	/** IO Component */
	m_plnkIO = pIO;

	/** Interlock IO Address */
	m_usiInterlockIOAddr = 0;

	/** Interlock IO Check Value */
	m_bInterlockJudge = 0;

	/** Timer 개체 생성 */
	m_bOriginFlag = FALSE;
	m_iOriginStep = DEF_ORIGIN_START_STEP;
	m_iOriginPrevStep = DEF_ORIGIN_START_STEP;
	m_iOriginError = ERR_MOTION_SUCCESS;
	m_ttOriginTimer = MTickTimer();

	m_bOnOriginReturn = FALSE;

	/** Component 초기화 */
	if ((iResult = Initialize (commonData, 0, m_SaxAxis)) != ERR_MOTION_SUCCESS)
	{
		// Error 처리
	}

	/** Timer 개체 생성 */
	m_ttOriginTimer = MTickTimer();

	m_sCommSet.m_bConnect = FALSE;

	iResult = Connect();
	if (iResult != ERR_MOTION_SUCCESS)
	{
		// Error 처리
	}

	/** For ACS_SIMULATION */
	m_dCurrentAxisPos = 0.0;
	m_dTargetAxisPos = 0.0;
	m_dRMoveDistance = 0.0;
	m_dVelocity = 0.0;
	m_iAccelerate = 0;
	m_iDecelerate = 0;
	m_bRMoveStart = FALSE;
	m_MotionTimer.StopTimer();
}

MACSAxis::~MACSAxis()
{
}

int MACSAxis::Connect()
{
	int iConnectOption;
	CString strAddr;

	// 2008.12.18 CYJ
	USES_CONVERSION;
	WCHAR* pszAnsi1 = NULL;
	char* pszAnsi2 = NULL;

	// 2008.12.18 CYJ
	pszAnsi1 = (WCHAR *)LPCTSTR(m_sCommSet.strIPAddr);
	pszAnsi2 = W2A(pszAnsi1);

	iConnectOption = ACSC_SOCKET_DGRAM_PORT;//UDP
//	iConnectOption = ACSC_SOCKET_STREAM_PORT;//TCP//인캅스가 막음

//	Disconnect();

#ifdef ACS_SIMULATION
//	m_sCommSet.Handle = acsc_OpenCommDirect();
#else
	m_sCommSet.Handle = acsc_OpenCommEthernet(pszAnsi2, iConnectOption);

	if (m_sCommSet.Handle == ACSC_INVALID)
	{
		if (m_sCommSet.strIPAddr == "10.0.0.100")
		{
			SetErrorLevel(_T("ACS AXIS connect"),3, __FILE__, __LINE__);
			//MyMessageBox("PC와 Gantry 1 Controller간 통신 연결이 되지 않았습니다.", "확인");
			MyMessageBox(MY_MESSAGE_BOX,90000,_T("Confirm"));
			return generateErrorCode(117001);
		}
		else if (m_sCommSet.strIPAddr == "10.0.0.101")
		{
			SetErrorLevel(_T("ACS AXIS connect"),3, __FILE__, __LINE__);
			//MyMessageBox("PC와 Gantry 2 Controller간 통신 연결이 되지 않았습니다.", "확인");
			MyMessageBox(MY_MESSAGE_BOX,90001,_T("Confirm"));
			return generateErrorCode(117002);
		}
		else
		{
			SetErrorLevel(_T("ACS AXIS connect"),3, __FILE__, __LINE__);
			//MyMessageBox("PC와 Gantry Controller간 통신 연결이 되지 않았습니다.", "확인");
			MyMessageBox(MY_MESSAGE_BOX,90002,_T("Confirm"));
			return generateErrorCode(117000);
		}
	}
	else
	{
		m_sCommSet.m_bConnect = TRUE;
		acsc_OpenHistoryBuffer(m_sCommSet.Handle, 100000);
		ServoOff();
	}
#endif


	return ERR_MOTION_SUCCESS;
}

int MACSAxis::Disconnect()
{
	acsc_CloseHistoryBuffer(m_sCommSet.Handle);
	acsc_ReleaseComm(m_sCommSet.Handle);
	acsc_CloseComm(m_sCommSet.Handle);
	m_sCommSet.Handle = ACSC_INVALID;
	m_sCommSet.m_bConnect = FALSE;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetData(SSMotionData *pData)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetData(SSMotionData* pData, SACSCommSet *psCommSet)
{
	m_strFileName	= pData->m_strFileName;	//	데이타 파일 이름

	swprintf( m_SaxAxis.szName, _T("%s"), pData->m_sAxisVal.szName);

	// 생성자를 통해 시스템에서 받은 데이타를 SAxisParameter 형태로 변환한다.
	m_SaxAxis.iAxisID			= pData->m_sAxisVal.iAxisID;			// HC-MMC B'd전체 분배
	m_SaxAxis.iOriginPriority	= pData->m_sAxisVal.iOriginPriority;
	m_SaxAxis.bSign				= pData->m_sAxisVal.bSign;
 	m_SaxAxis.bOriginDir		= pData->m_sAxisVal.bOriginDir;
	m_SaxAxis.bOriginFineDir	= pData->m_sAxisVal.bOriginFineDir;
 	m_SaxAxis.iCPhaseUseType	= pData->m_sAxisVal.iCPhaseUseType;
	m_SaxAxis.dScale			= pData->m_sAxisVal.dScale;
	m_SaxAxis.iRoleOfHome		= pData->m_sAxisVal.iRoleOfHome;
	m_SaxAxis.bAbsEncoder		= pData->m_sAxisVal.bAbsEncoder;

	m_sCommSet.Handle			= psCommSet->Handle;
	m_sCommSet.strIPAddr		= psCommSet->strIPAddr;
	m_sCommSet.m_bConnect		= psCommSet->m_bConnect;

	m_iACSMotionID				= pData->m_iACSMotionID;	//@
	m_iACSMotionInstance		= pData->m_iACSMotionInstance;
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetData(SSMotionData* pData)
{
	swprintf( pData->m_sAxisVal.szName, _T("%s"), m_SaxAxis.szName);

	// 생성자를 통해 시스템에서 받은 데이타를 SAxisParameter 형태로 변환한다.
	pData->m_sAxisVal.iAxisID			= m_SaxAxis.iAxisID;				// HC-MMC B'd전체 분배
	pData->m_sAxisVal.iOriginPriority	= m_SaxAxis.iOriginPriority;	
	pData->m_sAxisVal.bSign				= m_SaxAxis.bSign;				
 	pData->m_sAxisVal.bOriginDir		= m_SaxAxis.bOriginDir;		
	pData->m_sAxisVal.bOriginFineDir	= m_SaxAxis.bOriginFineDir;	
 	pData->m_sAxisVal.iCPhaseUseType	= m_SaxAxis.iCPhaseUseType;		
	pData->m_sAxisVal.dScale			= m_SaxAxis.dScale;			
	pData->m_sAxisVal.bAbsEncoder		= m_SaxAxis.bAbsEncoder;				

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis)
{
	int iResult = ERR_MOTION_SUCCESS;
	BOOL bUse = FALSE;
	CString strLogMsg;

	// Data 하나씩 범위 검사하며 설정

	if (getUseAxis())		
		return generateErrorCode(117006);

	// 축 사용으로 설정
	if ((iResult=setUseAxis(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	//SJ_YYK 150908  Add...
	if(m_iACSMotionID == 0)
	{
		int NConnections;
		ACSC_CONNECTION_DESC Connections[100];

		if (!acsc_GetConnectionsList(Connections, 100, &NConnections))
		{
			return ERR_MOTION_SUCCESS;
		}
		
		for(int i=0; i<NConnections; i++)
		{
			if (!acsc_TerminateConnection(&(Connections[i])))
			{
				
			}
		}
	}
	//_______________________*/

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::setUseAxis (BOOL bState)
{
	/** 축 사용 여부 확인 */
	if (m_rgbACSAxisUseFlag == TRUE)
		return generateErrorCode(117006);
	else
		m_rgbACSAxisUseFlag = TRUE;
	
	return ERR_MOTION_SUCCESS;
}

BOOL MACSAxis::getUseAxis()
{
	return m_rgbACSAxisUseFlag;
}

int	MACSAxis::LoadParameter()
{
	int iResult = ERR_MOTION_SUCCESS;
	MAxisData	fileData("",m_strFileName);

	/** 파일에서 데이타를 읽고 사용자가 설정한 데이타를 적절히 섞어서 모션이 사용할 
	 * 파라미터 구조를 만들어 모션을 초기화 한다.
	 */

	/** 파일에서 데이타를 읽는다. */
	fileData.SetSection(m_SaxAxis.szName);	// Section을 지정 한다.

	BOOL bReadSuccess = fileData.ReadData();
	m_AxisParameter = fileData;	// Component 형태로 저장한다.

	if (!bReadSuccess)
		return generateErrorCode(117008);

	/** 시스템에서 받은 데이타와 로드한 데이타를 가지고 Component를 초기화 한다. */
	iResult = SetInPositionData();

	return iResult;
}

/** Component에 데이타를 저장한다. 이때 파일에 같이 저장한다.
 * @param   *pData       : 설정할 각 축의 설정 Data
 * @return	Error : 0 = Success, 그외=기타
 */
int MACSAxis::SaveParameter(MAxisData* pData)
{
	int	iResult = ERR_MOTION_SUCCESS;

	MAxisData	fileData("",m_strFileName);

	/** 파일에서 데이타를 읽고 사용자가 설정한 데이타를 적절히 섞어서 모션이 사용할 
	 * 파라미터 구조를 만들어 모션을 초기화 한다.
	 */

	/** 파일에 저장 한다. */
	m_AxisParameter = *pData;
	fileData = m_AxisParameter;	// 저장할 데이타를 복사한다.

	fileData.SetSection(m_SaxAxis.szName);	// Section을 지정 한다.
	if (fileData.WriteData() != TRUE)
		iResult = generateErrorCode(117009);

	/** 데이타를 Component 변수에 저장한다. */
	iResult = SetInPositionData();

	return iResult;
}

void MACSAxis::GetParameter(MAxisData* pData)
{
	*pData	= m_AxisParameter;
}

int MACSAxis::GetAxisData (SAxis1 *pax1Data)
{
	// 미사용
	ASSERT(pax1Data);
	*pax1Data = m_SaxAxis;
	
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetAxisConfig(SMotionAxis sMotionAxis)
{
	// 미사용
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetAxisConfig(SMotionAxis* psMotionAxis)
{
	// 미사용
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::Move (double dPosition, 
				 double dVelocity, 
				 int iAccelerate, 
				 int iDecelerate, 
				 int iMoveType)
{
	int iResult = ERR_MOTION_SUCCESS;
	CString strLogMsg;

	/** 이동 전 원점복귀 여부 확인 */
	if (!m_bOriginFlag)
		return generateErrorCode(117010);

	if ((iMoveType == DEF_MOVE_DISTANCE) 
		|| (iMoveType == DEF_SMOVE_DISTANCE)
		|| (iMoveType == DEF_TMOVE_DISTANCE) 
		|| (iMoveType == DEF_TSMOVE_DISTANCE))
		return generateErrorCode(117011);

	if ((iResult = StartMove(dPosition, dVelocity, iAccelerate,
							 iDecelerate, iMoveType)) != ERR_MOTION_SUCCESS)
		return iResult;

	if ((iResult = Wait4Done()) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::StartMove (double dPosition, 
					  double dVelocity, 
					  int iAccelerate, 
					  int iDecelerate, 
					  int iMoveType)
{
#ifdef ACS_SIMULATION
	m_MotionTimer.StartTimer();
	m_dTargetAxisPos = dPosition;
	m_dVelocity = dVelocity;
	m_iAccelerate = iAccelerate;
	m_iDecelerate = iDecelerate;
	m_bRMoveStart = FALSE;
	m_dRMoveDistance = 0.0;
	return ERR_MOTION_SUCCESS;
#endif

	int iResult = ERR_MOTION_SUCCESS;

	double dSign, dPos, dVel;
	int iAccel, iDecel;
	double dAccel, dDecel, dJerk, dKillDecel;

	/** 이동 전 원점복귀 여부 확인 */
	if (!m_bOriginFlag)
		return generateErrorCode(117010);

	if ((iMoveType == DEF_MOVE_DISTANCE) 
		|| (iMoveType == DEF_SMOVE_DISTANCE)
		|| (iMoveType == DEF_TMOVE_DISTANCE) 
		|| (iMoveType == DEF_TSMOVE_DISTANCE))
		return generateErrorCode(117011);

	/** 축 상태 점검 */
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
		return iResult;

	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	dPos = dSign * m_SaxAxis.dScale * dPosition;

	/** 이동 속도 처리 */
	if (dVelocity == 0.0)
	{
		if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)			
			dVel = m_SaxAxis.dScale * m_AxisParameter.dRunVelocity;
		else
			dVel = m_SaxAxis.dScale * m_AxisParameter.dSlowRunVelocity;
	}
	else
		dVel =  m_SaxAxis.dScale * dVelocity;

	/** 이동 가속도 처리 */
	if (iAccelerate == 0)
		iAccel = m_AxisParameter.iRunAccelerate;
	else
		iAccel = iAccelerate;

	/** 이동 감속도 처리 */
	if (iDecelerate == 0)
		iDecel = m_AxisParameter.iRunAccelerate;
	else
		iDecel = iDecelerate;

	/** 가속도 및 감속도 계산 */
	//170817 JSH.s
// 	dAccel = ConvertmSec2mmPerSquareSec(dVel, iAccel);
// 	dDecel = ConvertmSec2mmPerSquareSec(dVel, iDecel);
	dAccel = fabs(dVel) * 2 / iAccel * 1000;
	dDecel = dAccel;
	dJerk = dAccel / iAccel * 1000;
	dKillDecel = dJerk;
	//170817 JSH.e

	/** SW Limit Check */
	if ((iResult = checkSWLimit(dPosition)) != ERR_MOTION_SUCCESS)
		return iResult;

	/** Timer Start */
	m_ttMovingTimer.StartTimer();

	iResult = acsc_SetVelocity(m_sCommSet.Handle, m_iACSMotionID, dVel, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117021);

	iResult = acsc_SetAcceleration(m_sCommSet.Handle, m_iACSMotionID, dAccel, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117022);

	iResult = acsc_SetDeceleration(m_sCommSet.Handle, m_iACSMotionID, dDecel, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117023);

	//170817 JSH.s
	iResult = acsc_SetKillDeceleration(m_sCommSet.Handle, m_iACSMotionID, dKillDecel, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117053);
	
//	iResult = acsc_SetJerk(m_sCommSet.Handle, m_iACSMotionID, dJerk, NULL);
	iResult = acsc_SetJerk(m_sCommSet.Handle, m_iACSMotionID, 4500, NULL);	
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117053);
	//170817 JSH.e

	/** start_move */
	iResult = acsc_ToPoint(m_sCommSet.Handle, 0, m_iACSMotionID, dPos, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117024);

	double dElaspTime;
	CString strLog;
	dElaspTime = m_ttMovingTimer.GetElapseTime();
//	strLog.Format(_T("Start Move Communication Time : %f\n"), dElaspTime);
//	TRACE(strLog);

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::RMove (double dDistance, 
				  double dVelocity,
				  int iAccelerate,
				  int iDecelerate,
				  int iMoveType, 
				  BOOL bClearOpt)
{
	int iResult = ERR_MOTION_SUCCESS;

	/** 이동 전 원점복귀 여부 확인 */
	if (!m_bOriginFlag)
		return generateErrorCode(117010);

	if ((iMoveType == DEF_MOVE_POSITION)
		|| (iMoveType == DEF_SMOVE_POSITION)
		|| (iMoveType == DEF_TMOVE_POSITION)
		|| (iMoveType == DEF_TSMOVE_POSITION))
		return generateErrorCode(117011);

	if ((iResult = StartRMove(dDistance, dVelocity, iAccelerate,
							 iDecelerate, iMoveType, bClearOpt)) != ERR_MOTION_SUCCESS)
		return iResult;

	if ((iResult = Wait4Done(FALSE,FALSE,TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::StartRMove (double dDistance, 
					   double dVelocity, 
					   int iAccelerate,
					   int iDecelerate, 
					   int iMoveType, 
					   BOOL bClearOpt)
{
#ifdef ACS_SIMULATION
	m_MotionTimer.StartTimer();
	m_bRMoveStart = TRUE;
	m_dRMoveDistance = dDistance;
	m_dVelocity = dVelocity;
	m_iAccelerate = iAccelerate;
	m_iDecelerate = iDecelerate;
	return ERR_MOTION_SUCCESS;
#endif

	int iResult = ERR_MOTION_SUCCESS;

	double dSign, dDis, dVel;
	int iAccel, iDecel;
	double dAccel, dDecel, dJerk, dKillDecel;

	if ((iMoveType == DEF_MOVE_POSITION) 
		|| (iMoveType == DEF_SMOVE_POSITION)
		|| (iMoveType == DEF_TMOVE_POSITION)
		|| (iMoveType == DEF_TSMOVE_POSITION))
		return generateErrorCode(117011);

	/** 축 상태 점검 */
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
		return iResult;

	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	/** 위치 정리 */
	dDis = dSign * m_SaxAxis.dScale * dDistance;

	/** 이동 속도 처리 */
	if (dVelocity == 0.0)
	{
		if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
			dVel = dSign * m_SaxAxis.dScale * m_AxisParameter.dRunVelocity;
		else
			dVel = dSign * m_SaxAxis.dScale * m_AxisParameter.dSlowRunVelocity;
	}
	else
		dVel = dSign * m_SaxAxis.dScale * dVelocity;

	/** 이동 가속도 처리 */
	if (iAccelerate == 0)
		iAccel = m_AxisParameter.iRunAccelerate;
	else
		iAccel = iAccelerate;

	/** 이동 감속도 처리 */
	if (iDecelerate == 0)
		iDecel = m_AxisParameter.iRunAccelerate;
	else
		iDecel = iDecelerate;

	/** 가속도 및 감속도 계산 */
	//170817 JSH.s
// 	dAccel = ConvertmSec2mmPerSquareSec(dVel, iAccel);
// 	dDecel = ConvertmSec2mmPerSquareSec(dVel, iDecel);
	dAccel = fabs(dVel) * 2 / iAccel * 1000;
	dDecel = dAccel;
	dJerk = dAccel / iAccel * 1000;
	dKillDecel = dJerk;
	//170817 JSH.e

	/** Timer Start */
	m_ttMovingTimer.StartTimer();

	iResult = acsc_SetVelocity(m_sCommSet.Handle, m_iACSMotionID, dVel, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117021);

	iResult = acsc_SetAcceleration(m_sCommSet.Handle, m_iACSMotionID, dAccel, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117022);

	iResult = acsc_SetDeceleration(m_sCommSet.Handle, m_iACSMotionID, dDecel, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117023);

	//170817 JSH.s
	iResult = acsc_SetKillDeceleration(m_sCommSet.Handle, m_iACSMotionID, dKillDecel, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117053);
	
//	iResult = acsc_SetJerk(m_sCommSet.Handle, m_iACSMotionID, dJerk, NULL);
	iResult = acsc_SetJerk(m_sCommSet.Handle, m_iACSMotionID, 4500, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117053);
	//170817 JSH.e

	/** 현재 Command 값을 Clear하고 RMove 동작 수행 */
/*
	if (bClearOpt ==TRUE)
	{
		// 현재 위치와 목표 위치를 0.0으로 설정
		if ((iResult = SetCurrentPosition(0.0, FALSE)) != ERR_MOTION_SUCCESS)
			return iResult;		
	}
*/
	/** start_rs_move */
	iResult = acsc_ToPoint(m_sCommSet.Handle, ACSC_AMF_RELATIVE, m_iACSMotionID, dDis, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117024);

	double dElaspTime;
	CString strLog;
	dElaspTime = m_ttMovingTimer.GetElapseTime();
	strLog.Format(_T("Start R Move Communication Time : %f\n"), dElaspTime);
	TRACE(strLog);

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::VMove (double dVelocity, int iAccelerate, BOOL bDir)
{
	int iResult = ERR_MOTION_SUCCESS;

	double dSign, dDir, dVel;
	int iAccel;
	double dAccel, dDecel, dJerk, dKillDecel;
#ifndef ACS_SIMULATION
	int iState;
#endif
	
	/** (+) 방향 */
	if (bDir == TRUE) dDir = 1.0;
	/** (-) 방향 */
	else dDir = -1.0;

	/** 축 상태 점검 */
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
		return iResult;

	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	/** 이동 속도 처리 */
	if (dVelocity == 0.0)
	{
		if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
			dVel = dDir * dSign * m_SaxAxis.dScale * m_AxisParameter.dRunVelocity;
		else
			dVel = dDir * dSign * m_SaxAxis.dScale * m_AxisParameter.dSlowRunVelocity;
	}
	else
		dVel = dDir * dSign * m_SaxAxis.dScale * dVelocity;

	/** 이동 가속도 처리 */
	if (iAccelerate == 0)
		iAccel = m_AxisParameter.iRunAccelerate;
	else
		iAccel = iAccelerate;

	/** 가속도 계산 */
	//170817 JSH.s
//	dAccel = ConvertmSec2mmPerSquareSec(dVel, iAccel);
	dAccel = fabs(dVel) * 2 / iAccel * 1000;
	dDecel = dAccel;
	dJerk = dAccel / iAccel * 1000;
	dKillDecel = dJerk;
	//170817 JSH.e


#ifndef ACS_SIMULATION
	iResult = acsc_SetAcceleration(m_sCommSet.Handle, m_iACSMotionID, dAccel, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("VMove() : Setting Acceleration failed");
		SetErrorLevel(_T("ACS AXIS Vmove"),3, __FILE__, __LINE__);
		return generateErrorCode(117022);
	}

	//170817 JSH.s
	iResult = acsc_SetDeceleration(m_sCommSet.Handle, m_iACSMotionID, dDecel, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("StartMove() : Setting Deceleration failed");
		SetErrorLevel(_T("ACS AXIS start Move"),3, __FILE__, __LINE__);
		return generateErrorCode(117023);
	}
	
	iResult = acsc_SetKillDeceleration(m_sCommSet.Handle, m_iACSMotionID, dKillDecel, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("StartMove() : Setting Deceleration failed");
		SetErrorLevel(_T("ACS AXIS start Move"),3, __FILE__, __LINE__);
		return generateErrorCode(117053);
	}
	
	iResult = acsc_SetJerk(m_sCommSet.Handle, m_iACSMotionID, dJerk, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("StartMove() : Setting Deceleration failed");
		SetErrorLevel(_T("ACS AXIS start Move"),3, __FILE__, __LINE__);
		return generateErrorCode(117053);
	}
	//170817 JSH.e

	iResult = acsc_GetFault(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("VMove() : Fault Check failed");
		SetErrorLevel(_T("ACS AXIS Vmove"),3, __FILE__, __LINE__);
		return generateErrorCode(117025);
	}

	if ( ((iState & ACSC_SAFETY_RL) && dVel > 0) ||	// (+)Limit, (+)Dir
		((iState & ACSC_SAFETY_LL) && dVel < 0) )	// (-)Limit. (-)Dir
	{
		VStop();
		return ERR_MOTION_SUCCESS;
	}

	/** v_move */
	iResult = acsc_Jog(m_sCommSet.Handle, ACSC_AMF_VELOCITY, m_iACSMotionID, dVel, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("VMove() : VMove Start failed");
		SetErrorLevel(_T("ACS AXIS Vmove"),3, __FILE__, __LINE__);
		return generateErrorCode(117024);
	}
#endif

	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
	{
		VStop();
		return iResult;
	}

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::Wait4Done(BOOL bSkipMode, BOOL bUseInterlock, BOOL bRMove)
{
#ifdef ACS_SIMULATION
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
				return generateErrorCode(117012);
			
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

	int		iResult = ERR_MOTION_SUCCESS;
	BOOL	bWaitDone = FALSE;
	BOOL	bWaitDone1 = TRUE;
	DWORD	dwElasp = 0;
	BOOL	bVal;
	double	dCurrentPos, dTargetPos;
	
	/** Timer Start */
	m_ttMovingTimer.StartTimer();

	/** bSkipSensor == TRUE  : Motion 동작만 확인
	 * 	bSkipSensor == FALSE : Motion 동작 및 위치 확인 
	 */
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
				//WriteErrorLog("Wait4Done() : IO Interlock Checked");
				return generateErrorCode(117013);
			}
		}

		//Amp Fault 확인
		if (IsAmpFault())
		{
			//Amp Fault 발생
			EStop();
			//WriteErrorLog("Wait4Done() : Amp Fault");
			return generateErrorCode(117014);
		}

		if (m_ttMovingTimer.MoreThan(m_AxisParameter.dLimitTime))
			return generateErrorCode(117012);
		Sleep(5);
	}

	/** 축 상태 점검 */
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
		return iResult;

	if (!bSkipMode) 
		Sleep((DWORD)(m_AxisParameter.dStabilityTime * 1000));
	else 
		return ERR_MOTION_SUCCESS;

	/** 이동 완료 후 현재 위치가 목표위치와 다르면 Error 처리 
	 *  RMove 인 경우 계산하지 말아야 함
	 */
	if (m_bOriginFlag == TRUE && bRMove == FALSE)	// 원점복귀되어 있으면 위치 확인을 한다.
	{
		/* 1. Encoder 읽기 */
		dCurrentPos = GetCurrentPosition(DEF_POS_ENCODER);			
		/* 2. Command 읽기 */
		dTargetPos = GetCurrentPosition(DEF_POS_COMMAND);
		
		int iRetryCount = 0;

		/* 3. 현재 위치와 목표 위치 Tolerance값과 비교하기 */
		while (fabs(dCurrentPos - dTargetPos) > m_AxisParameter.dTolerance)
		{
			//정헌식 Retry 추가함....  ㅡ_ㅡ;;
			Sleep((DWORD)(m_AxisParameter.dStabilityTime * 1000));
			
			if (iRetryCount > 10)
				return generateErrorCode(117015);

			/* 1. Encoder 읽기 */
			dCurrentPos = GetCurrentPosition(DEF_POS_ENCODER);			
			/* 2. Command 읽기 */
			dTargetPos = GetCurrentPosition(DEF_POS_COMMAND);
			
			iRetryCount++;
		}
	}

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::MoveSplineLine (int iMaxPoint, 
						   double* pdPosition, 
						   double* pdVelocity, 
						   int* piAccelerate,
						   BOOL bAutoFlag, 
						   BOOL bWaitOpt)
{
	ASSERT(pdPosition);
	ASSERT(pdVelocity);
	ASSERT(piAccelerate);

	if (iMaxPoint < 2)
	{
		// Error 처리
		//iMaxPoint 2개 이상이어야 함
	}

	int iResult = ERR_MOTION_SUCCESS;

	double rgdPos[DEF_SPLINE_MOVE_PATH_MAX_NO];
	double rgdVel[DEF_SPLINE_MOVE_PATH_MAX_NO];
	double rgdAccel[DEF_SPLINE_MOVE_PATH_MAX_NO];
	double dSign;
	double dScale;
	double dCurPos;
	
	/** 사용할 축 추출 */
	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;
	dScale = m_SaxAxis.dScale;

	/** 이동 전 축 Event State 확인 */
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
		return iResult;

	// 이동 Point에 따라
	for (int i = 0; i < iMaxPoint; i++)
	{
		// 위치 정리
		rgdPos[i] = dSign * dScale * pdPosition[i];
		rgdVel[i] = dSign * dScale * pdVelocity[i];

		/** 가속도 및 감속도 계산 */
		rgdAccel[i] = ConvertmSec2mmPerSquareSec(rgdVel[i], piAccelerate[i]);
	}

	dCurPos = GetCurrentPosition(DEF_POS_ENCODER);

	for (int i = 0; i < iMaxPoint-1; i++)
	{
		if ( (rgdPos[i+1] - rgdPos[i]) > 0)
		{
			if (dCurPos > rgdPos[i])
				rgdPos[i] = dCurPos;
		}
		else if ( (rgdPos[i+1] - rgdPos[i]) < 0)
		{
			if (dCurPos < rgdPos[i])
				rgdPos[i] = dCurPos;
		}
	}

#ifndef ACS_SIMULATION
	/** Timer Start */
	m_ttMovingTimer.StartTimer();

	/** 이동 Point에 따라 */
	for (int i = 0; i < iMaxPoint-1; i++)
	{
		iResult = acsc_SetAcceleration(m_sCommSet.Handle, m_iACSMotionID, rgdAccel[i], NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			//WriteErrorLog("MoveSplineLine() : Setting Acceleration failed");
			SetErrorLevel(_T("ACS AXIS move Spline Line"),3, __FILE__, __LINE__);
			return generateErrorCode(117022);
		}

		iResult = acsc_ExtToPoint(m_sCommSet.Handle, ACSC_AMF_VELOCITY | ACSC_AMF_ENDVELOCITY, m_iACSMotionID, rgdPos[i], rgdVel[i], rgdVel[i+1], NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			//WriteErrorLog("MoveSplineLine() : Setting Spline Move Velocity failed");
			SetErrorLevel(_T("ACS AXIS move Spline Line"),3, __FILE__, __LINE__);
			return generateErrorCode(117026);
		}
	}

	iResult = acsc_SetVelocity(m_sCommSet.Handle, m_iACSMotionID, rgdVel[iMaxPoint-1], NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("MoveSplineLine() : Setting Velocity failed");
		SetErrorLevel(_T("ACS AXIS move Spline Line"),3, __FILE__, __LINE__);
		return generateErrorCode(117021);
	}

	iResult = acsc_SetAcceleration(m_sCommSet.Handle, m_iACSMotionID, rgdAccel[iMaxPoint-1], NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("MoveSplineLine() : Setting Acceleration failed");
		SetErrorLevel(_T("ACS AXIS move Spline Line"),3, __FILE__, __LINE__);
		return generateErrorCode(117022);
	}

	iResult = acsc_ToPoint(m_sCommSet.Handle, 0, m_iACSMotionID, rgdPos[iMaxPoint-1], NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("MoveSplineLine() : Spline Move Start failed");
		SetErrorLevel(_T("ACS AXIS move Spline Line"),3, __FILE__, __LINE__);
		return generateErrorCode(117024);
	}

	double dElaspTime;
	CString strLog;
	dElaspTime = m_ttMovingTimer.GetElapseTime();
	strLog.Format(_T("Spline Move Communication Time : %f\n"), dElaspTime);
	TRACE(strLog);

#endif

	if (bWaitOpt == TRUE)
	{
		/** Spline Line 이동 완료될 때까지 대기 */		
		if ((iResult = Wait4Done()) != ERR_MOTION_SUCCESS)
			return iResult;
	}

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::JogMoveSlow(BOOL bDir, double dSlow)// = 0.0)
{
	int iResult = ERR_MOTION_SUCCESS;
	double dJogSlow = 0.0;
	
	/** Jog Slow 처리 */
	if (dSlow == 0.0)
	{
		iResult = VMove(m_AxisParameter.dJogSlow, 0, bDir);
		if (ERR_MOTION_SUCCESS != iResult)
		{
			VStop();
			return iResult;
		}
	}
	else
	{
		iResult = VMove(dSlow, 0, bDir);
		if (ERR_MOTION_SUCCESS != iResult)
		{
			VStop();
			return iResult;
		}
	}
	
	if (IsAmpFault())
	{
		VStop();
		return generateErrorCode(117014);
	}

//	if ((iResult = checkAxisStateForJog(bDir)) != ERR_MOTION_SUCCESS)
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
	{
		VStop();
		return iResult;
	}

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::JogMoveFast(BOOL bDir, double dFast)// = 0.0)
{
	int iResult = ERR_MOTION_SUCCESS;
	double dJogVel = 0.0;

	/** Jog Fast 처리 */
	if (dFast == 0.0)
	{
		iResult = VMove(m_AxisParameter.dJogFast, 0, bDir);
		if (ERR_MOTION_SUCCESS != iResult)
		{
			VStop();
			return iResult;
		}
	}
	else
	{
		iResult = VMove(dFast, 0, bDir);
		if (ERR_MOTION_SUCCESS != iResult)
		{
			VStop();
			return iResult;
		}
	}
	
	if (IsAmpFault())
	{
		VStop();
		return generateErrorCode(117014);
	}

//	if ((iResult = checkAxisStateForJog(bDir)) != ERR_MOTION_SUCCESS)
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
	{
		VStop();
		return iResult;
	}
	

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetEStopRate()
{

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::JogStop()
{
	int iResult = ERR_MOTION_SUCCESS;

	iResult = VStop();

	return iResult;
}

int MACSAxis::Stop(BOOL *pbStatus)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;

	//원점 복귀중이었으면 Origin Return Buffer 실행을 정지시킨다.
	if (m_bOnOriginReturn)
	{
		iResult = acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			acsc_Halt(m_sCommSet.Handle, m_iACSMotionID, NULL);
			//WriteErrorLog("Stop() : StopBuffer failed");
			SetErrorLevel(_T("ACS AXIS stop"),3, __FILE__, __LINE__);
			return generateErrorCode(117027);
		}

		m_bOnOriginReturn = FALSE;
	}

	/** Stop 설정 */
	iResult = acsc_Halt(m_sCommSet.Handle, m_iACSMotionID, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("Stop() : Stop failed");
		SetErrorLevel(_T("ACS AXIS stop"),3, __FILE__, __LINE__);
		return generateErrorCode(117028);
	}
#endif

	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::VStop(BOOL *pbStatus)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	/** V-Stop 설정 */
	iResult = acsc_Kill(m_sCommSet.Handle, m_iACSMotionID, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("VStop() : VStop failed");
		SetErrorLevel(_T("ACS AXIS stop"),3, __FILE__, __LINE__);
		return generateErrorCode(117029);
	}
#endif	
	/** VStop 후 안정화 시간 적용 - 2004.9.12 반용석K */
	Sleep((ULONG)(m_AxisParameter.dStabilityTime));

	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::EStop(BOOL *pbStatus)// = NULL)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	/** EStop 설정 */
	iResult = acsc_Kill(m_sCommSet.Handle, m_iACSMotionID, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("EStop() : EStop failed");
		SetErrorLevel(_T("ACS AXIS Estop"),3, __FILE__, __LINE__);
		return generateErrorCode(117030);
	}
#endif

	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::ClearAxis(BOOL *pbStatus)// = NULL)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	/** 축 발생 Event 해제 */
	iResult = acsc_FaultClear(m_sCommSet.Handle, m_iACSMotionID, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		if (pbStatus != NULL)
			*pbStatus = FALSE;

		//WriteErrorLog("ClearAxis() : Fault Clear failed");
		SetErrorLevel(_T("ACS AXIS Estop"),3, __FILE__, __LINE__);
		return generateErrorCode(117031);
	}

#endif

	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetStopRate(short siRate)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetEStopRate(short siRate)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetEncoderDirection(BOOL bEncDir, BOOL bCoorDir)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	double dEncDir;

	dEncDir = (bEncDir) ? 1 : 0;

	/** Encoder 입력 방향 설정 */
	iResult = acsc_SetConf(m_sCommSet.Handle, ACSC_CONF_ENCODER_KEY, m_iACSMotionID, dEncDir, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("SetEncoderDirection() : Setting Encoder Direction failed");
		SetErrorLevel(_T("ACS AXIS set Encoder Direction"),3, __FILE__, __LINE__);
		return generateErrorCode(117032);
	}
#endif
	return ERR_MOTION_SUCCESS;
}


int MACSAxis::GetAxisSource()
{
	int iState;

#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	/** 축 Event 발생 내역 읽기 */
	iResult = acsc_GetMotorState(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("GetAxisSource() : Getting Motor State failed");
		SetErrorLevel(_T("ACS AXIS get Axis Source"),3, __FILE__, __LINE__);
		return generateErrorCode(117034);
	}
#else
	iState = ACSC_MST_ENABLE;
#endif
	
	return iState;
}

int MACSAxis::GetAxisState(int *piReturn)
{
	int iState;

#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	/** 축 Event 발생 내역 읽기 */
	iResult = acsc_GetFault(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("GetAxisState() : Fault Check failed");
		SetErrorLevel(_T("ACS AXIS get Axis State"),3, __FILE__, __LINE__);
		return generateErrorCode(117025);
	}
#else
	iState = 0x00;
#endif
	
	*piReturn = iState;
	return ERR_MOTION_SUCCESS;
}

BOOL MACSAxis::IsAxisDone()
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	int iState;

	iResult = acsc_GetMotorState(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("IsAxisDone() : Getting Motor State failed");
		SetErrorLevel(_T("ACS AXIS check Axis Done"),3, __FILE__, __LINE__);
		return generateErrorCode(117034);
	}

	if (!(iState & ACSC_MST_MOVE) && (iState & ACSC_MST_INPOS))
		return TRUE;
#else
	if (m_dCurrentAxisPos == m_dTargetAxisPos)
	{
		return TRUE;
	}
#endif

	return FALSE;
}

BOOL MACSAxis::IsInMotion()
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	int iState;

	iResult = acsc_GetMotorState(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		SetErrorLevel(_T("ACS AXIS check Axis is in Motion"),3, __FILE__, __LINE__);
		//WriteErrorLog("IsInMotion() : Getting Motor State failed");
		return generateErrorCode(117034);
	}

	if (iState & ACSC_MST_MOVE)
		return TRUE;
#else
	double dTime = calculateMotionTime();

	if (m_MotionTimer.IsTimerStarted()
		&& m_MotionTimer.GetElapseTime() < dTime)
	{
		return TRUE;
	}
#endif

	return FALSE;
}

BOOL MACSAxis::IsMotionDone()
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	int iState;

	iResult = acsc_GetMotorState(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("IsMotionDone() : Getting Motor State failed");
		SetErrorLevel(_T("ACS AXIS check Axis is in Motion Done"),3, __FILE__, __LINE__);
		return generateErrorCode(117034);
	}

	if (!(iState & ACSC_MST_MOVE))
		return TRUE;
#else
	if (m_dCurrentAxisPos == m_dTargetAxisPos)
	{
		return TRUE;
	}
#endif

	return FALSE;
}

int MACSAxis::GetComVelocity()
{
//	return get_com_velocity(m_SaxAxis.iAxisID);
	return ERR_MOTION_SUCCESS;
}

BOOL MACSAxis::IsEStopEvent()
{
	//수정해야함
/*
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	int iState;

	iResult = acsc_GetFault(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		WriteErrorLog("IsEStopEvent() : Fault Check failed");
		return generateErrorCode(117025);
	}

	if (iState & ACSC_SAFETY_ES)
		return TRUE;
#endif
*/
	return FALSE;
}

double MACSAxis::GetCurrentPosition(BOOL bCommandType)
{
#ifdef ACS_SIMULATION
	return m_dCurrentAxisPos;
#endif

	double dSign;
	double dCurrentPosition;

	int iResult = ERR_MOTION_SUCCESS;

	if (bCommandType == DEF_POS_ENCODER)
	{		
		/** 축의 실제위치 읽기 */
		iResult = acsc_GetFPosition(m_sCommSet.Handle, m_iACSMotionID, &dCurrentPosition, NULL);
		if (iResult == DEF_ACS_FAIL)
			return generateErrorCode(117036);
	}
	else	// DEF_POS_COMMAND
	{
		/** 축의 목표위치 읽기 */
		iResult = acsc_GetRPosition(m_sCommSet.Handle, m_iACSMotionID, &dCurrentPosition, NULL);
		if (iResult == DEF_ACS_FAIL)
			return generateErrorCode(117038);
	}

	/** Encoder가 CCW Type이면 값은 역상이 되어야 한다. */
	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	dCurrentPosition *= (dSign / m_SaxAxis.dScale);
	
	return dCurrentPosition;
}

int MACSAxis::SetCurrentPosition(double dCurrentPosition, BOOL bType)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	double dPos;
	double dSign;
	
	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	if (bType == DEF_POS_ENCODER)
	{
		double dEncDir = 1.0;

		/** Encoder가 CCW Type이면 값은 역상이 되어야 한다. */
		/** 축 Encoder 방향 설정 읽기 */
/*		iResult = acsc_GetConf(m_sCommSet.Handle, ACSC_CONF_ENCODER_KEY, m_iACSMotionID, &dEncDir, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			WriteErrorLog("SetCurrentPosition() : Getting Encoder Direction failed");
			return generateErrorCode(117033);
		}
*/
////////////////////////////////////////////////////////////////////////
//		dEncDir = (dEncDir == DEF_CORD_CW) ? -1.0 : 1.0;
//		dEncDir = (dEncDir < 0.0) ? -1.0 : 1.0;
////////////////////////////////////////////////////////////////////////

		dPos = dCurrentPosition * dEncDir * dSign * m_SaxAxis.dScale;

		/** 축의 실제위치 설정 */
		iResult = acsc_SetFPosition(m_sCommSet.Handle, m_iACSMotionID, dPos, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			//WriteErrorLog("SetCurrentPosition() : Setting Current Real Position failed");
			SetErrorLevel(_T("ACS AXIS set Current Position"),3, __FILE__, __LINE__);
			return generateErrorCode(117035);
		}
	}
	else	//DEF_POS_COMMAND
	{
		dPos = dCurrentPosition * dSign * m_SaxAxis.dScale;

		/** 축의 목표위치 설정 */
		iResult = acsc_SetRPosition(m_sCommSet.Handle, m_iACSMotionID, dPos, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			//WriteErrorLog("SetCurrentPosition() : Setting Current Target Position failed");
			SetErrorLevel(_T("ACS AXIS set Current Position"),3, __FILE__, __LINE__);
			return generateErrorCode(117037);
		}
	}
#endif
	return ERR_MOTION_SUCCESS;
}

BOOL MACSAxis::ComparePosition(BOOL bPosOpt, double dTargetPosition,
					 double *pdPermission, double *pdDeviation)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;
	
	/** 현재 위치 읽기 */
#ifdef ACS_SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition(bPosOpt);
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
		if (fabs(dDevPosition) <= m_AxisParameter.dTolerance)
			return TRUE;		
	}
	
	return FALSE;
}

BOOL MACSAxis::IsLessThanTargetPosition(double dTargetPosition,
					 double *pdPermission)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;
	
	/** 현재 위치 읽기 */
#ifdef ACS_SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition(FALSE);
#endif

	/** Judge 결정 */
	if (pdPermission != NULL)
	{
		/** 위치 차이 계산 */
		dDevPosition = dTargetPosition - *pdPermission - dCurPosition;
		if ( dDevPosition >= 0 )
			return TRUE;		
	}
	else
	{
		dDevPosition = dTargetPosition - m_AxisParameter.dTolerance - dCurPosition;
		if ( dDevPosition >= 0 )
			return TRUE;		
	}
	
	return FALSE;
}

BOOL MACSAxis::IsGreaterThanTargetPosition(double dTargetPosition,
					 double *pdPermission)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;
	
	/** 현재 위치 읽기 */
#ifdef ACS_SIMULATION
	dCurPosition = m_dCurrentAxisPos;
#else
	dCurPosition = GetCurrentPosition(FALSE);
#endif

	/** Judge 결정 */
	if (pdPermission != NULL)
	{
		/** 위치 차이 계산 */
		dDevPosition = dTargetPosition + *pdPermission - dCurPosition;
		if ( dDevPosition <= 0 )
			return TRUE;		
	}
	else
	{
		dDevPosition = dTargetPosition + m_AxisParameter.dTolerance - dCurPosition;
		if ( dDevPosition <= 0 )
			return TRUE;		
	}
	
	return FALSE;
}


int MACSAxis::SetAmpEnable(BOOL bEnable)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;

	/** 축의 AMP Enable/Disable 설정 */
	if (bEnable == TRUE)
	{
		iResult = acsc_Enable(m_sCommSet.Handle, m_iACSMotionID, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			//WriteErrorLog("SetAmpEnable() : Amp Enable failed");
			SetErrorLevel(_T("ACS AXIS set Amp Enable"),3, __FILE__, __LINE__);
			return generateErrorCode(117039);
		}
	}
	else
	{
		iResult = acsc_Disable(m_sCommSet.Handle, m_iACSMotionID, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			SetErrorLevel(_T("ACS AXIS set Amp Enable"),3, __FILE__, __LINE__);
			//WriteErrorLog("SetAmpEnable() : Amp Disable failed");
			return generateErrorCode(117040);
		}
	}
#endif

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetAmpEnableLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

BOOL MACSAxis::IsAmpEnable()
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	int iState;

	iResult = acsc_GetMotorState(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("IsAmpEnable() : Getting Motor State failed");
		SetErrorLevel(_T("ACS AXIS check Amp Enable"),3, __FILE__, __LINE__);
		return generateErrorCode(117034);
	}

	if (iState & ACSC_MST_ENABLE)
		return TRUE;
	else
		return FALSE;
#endif

	return TRUE;
}

BOOL MACSAxis::IsAmpFault()
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;
	int iState;

	iResult = acsc_GetFault(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		//WriteErrorLog("IsAmpFault() : Fault Check failed");
		SetErrorLevel(_T("ACS AXIS check Amp Fault"),3, __FILE__, __LINE__);
		return generateErrorCode(117025);
	}

	if (iState != 0)
		return TRUE;
	else
		return FALSE;
#endif

	return TRUE;
}

int MACSAxis::ResetAmpFault(BOOL bIsCheck)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;

	iResult = acsc_FaultClear(m_sCommSet.Handle, m_iACSMotionID, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		SetErrorLevel(_T("ACS AXIS reset Amp Fault"),3, __FILE__, __LINE__);
		//WriteErrorLog("ResetAmpFault() : Fault Clear failed");
		return generateErrorCode(117031);
	}
#endif

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetAmpResetLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetAmpFaultEnable()
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetAmpFaultLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::ServoOn(BOOL *pbStatus, BOOL bIsCheck)
{
#ifndef ACS_SIMULATION
	int iResult = ERR_MOTION_SUCCESS;

	// 통신 연결 확인 후 연결되지 않았으면 재연결
	// 원점 복귀도 다시 해야 할 것으로 생각됨

	// AMP Disable
	if ((iResult = SetAmpEnable(FALSE)) != ERR_MOTION_SUCCESS)
	{
		SetErrorLevel(_T("ACS AXIS on Servo"),3, __FILE__, __LINE__);
		return iResult;		
	}
	Sleep(10);

	// AMP Fault Reset
	if ((iResult = ResetAmpFault()) != ERR_MOTION_SUCCESS)
	{
		SetErrorLevel(_T("ACS AXIS on Servo"),3, __FILE__, __LINE__);
		return iResult;
	}
	Sleep(10);

	// Status Clear,  Frame Clear
	if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
	{
		SetErrorLevel(_T("ACS AXIS on Servo"),3, __FILE__, __LINE__);
		return iResult;
	}
	Sleep(10);

	// Gantry 로 설정
	// 원점 복귀 Buffer에서 설정

	// AMP Enable
	if ((iResult = SetAmpEnable(TRUE)) != ERR_MOTION_SUCCESS)
	{
		SetErrorLevel(_T("ACS AXIS on Servo"),3, __FILE__, __LINE__);
		return iResult;
	}

	if (pbStatus != NULL)
	{
		// AMP Enable 상태 읽기
		*pbStatus = IsAmpEnable();		
	}
#endif

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::ServoOff(BOOL *pbStatus, BOOL bIsCheck)
{
	int iResult = ERR_MOTION_SUCCESS;
		
	// 원점복귀 Flag Reest
	ResetOrigin();

	// AMP Disable
	if ((iResult = SetAmpEnable(FALSE)) != ERR_MOTION_SUCCESS)
		return iResult;		
	Sleep(10);

	// AMP Fault Reset
	if ((iResult = ResetAmpFault()) != ERR_MOTION_SUCCESS)
		return iResult;
	Sleep(10);

	// Frame Clear, Status Clear
	if ((iResult = ClearAxis()) != ERR_MOTION_SUCCESS)
		return iResult;	

	// ACS Controller Restart
	// 적용 예정

	if (pbStatus != NULL)
	{
		*pbStatus = IsAmpEnable();		
	}

	return ERR_MOTION_SUCCESS;
}


CString MACSAxis::GetAxisName()
{
	CString strName;
	
	strName.Format(_T("%s"), m_SaxAxis.szName);
	return strName;
}

void MACSAxis::SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue)
{
	m_usiInterlockIOAddr = usiIOAddress;
	m_bInterlockJudge = bJudgeValue;
}

int MACSAxis::SetInPositionData()
{
	int		iResult = ERR_MOTION_SUCCESS;

//	if ((iResult = checkAxisParameter(m_SaxAxis)) != ERR_MOTION_SUCCESS)
//		return iResult;

/*
	//축의 위치결정 완료값 설정
	if ((iResult = set_in_position(m_SaxAxis.iAxisID, m_AxisParameter.dTolerance*m_SaxAxis.dScale)) != ERR_MOTION_SUCCESS)
	{
		CString strLogMsg;
		strLogMsg.Format("SetInPositionData(): set_in_position(set 'in_position' value and save in memory) Fail[AxisID=%d] - Error# is %d", m_SaxAxis.iAxisID, siResult);
		WriteErrorLog(strLogMsg);
		return makeMMCError((int)siResult);
	}
*/	
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetAxisID()
{
	return m_SaxAxis.iAxisID;	
}

double MACSAxis::GetHomeOffset()
{
	return m_AxisParameter.dHomeOffset;	
}

double MACSAxis::GetNegativeSWLimit()
{
	return m_AxisParameter.dCCWSWLimit;	
}

double MACSAxis::GetPositiveSWLimit()
{
	return m_AxisParameter.dCWSWLimit;	
}

double MACSAxis::GetSlowMovingVelocity()
{
	return m_AxisParameter.dSlowRunVelocity;
}

double MACSAxis::GetMovingVelocity()
{
	return m_AxisParameter.dRunVelocity;
}

int MACSAxis::GetMovingAccelerate()
{
	return m_AxisParameter.iRunAccelerate;
}

double MACSAxis::GetCoarseVelocity()
{
	return m_AxisParameter.dHomeFastVelocity;	
}

int MACSAxis::GetCoarseAccelerate()
{
	return m_AxisParameter.iHomeAccelerate;
}

double MACSAxis::GetFineVelocity()
{
	return m_AxisParameter.dHomeSlowVelocity;
}

int MACSAxis::GetFineAccelerate()
{
	return m_AxisParameter.iHomeAccelerate;
}

double MACSAxis::GetJogSlow()
{
	return m_AxisParameter.dJogSlow;	
}

double MACSAxis::GetJogFast()
{
	return m_AxisParameter.dJogFast;
}

BOOL MACSAxis::GetSign()
{
	return m_SaxAxis.bSign;
}

BOOL MACSAxis::IsAbsEncoder()
{
	return m_SaxAxis.bAbsEncoder;
}

BOOL MACSAxis::GetOriginDir()
{
	return m_SaxAxis.bOriginDir;
}

BOOL MACSAxis::GetOriginFineDir()
{
	return m_SaxAxis.bOriginFineDir;
}

int MACSAxis::GetCPhaseUseType()
{
	return m_SaxAxis.iCPhaseUseType;
}

double MACSAxis::GetScale()
{
	return m_SaxAxis.dScale;
}

double MACSAxis::GetMoveTime()
{
	return m_AxisParameter.dLimitTime;
}

double MACSAxis::GetMoveAfterTime()
{
	return m_AxisParameter.dStabilityTime;
}

double MACSAxis::GetTolerance()
{
	return m_AxisParameter.dTolerance;
}

double MACSAxis::GetPassOffset()
{
	return m_AxisParameter.dPassOffset;
}

ERoleOfHome MACSAxis::GetRoleOfHomeSensor()
{
	return m_SaxAxis.iRoleOfHome;
}

double MACSAxis::GetOriginWaitTime()
{
	return m_AxisParameter.dOriginLimitTime;
}

BOOL MACSAxis::IsOriginReturn()
{
	return m_bOriginFlag;
}

void MACSAxis::SetOriginFlag()
{
	m_bOriginFlag = TRUE;
}

int MACSAxis::ResetOrigin(BOOL *pbReturn)
{
	int iResult = ERR_MOTION_SUCCESS;
	BOOL bResult = TRUE;

	m_bOriginFlag = FALSE;
	m_iOriginStep = 100;
	m_iOriginPrevStep = 100;
	m_iOriginError = ERR_MOTION_SUCCESS;

	if (pbReturn != NULL)
		*pbReturn = m_bOriginFlag;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetIndexRequired(BOOL *pbIndexReq)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetIndexRequired(BOOL bIndexReq)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetOriginPriority()
{
	return m_SaxAxis.iOriginPriority;
}

int MACSAxis::OriginReturn(int iOriginMoveType)
{
	// 확인 필요 : 
#ifdef ACS_SIMULATION
	m_bOriginFlag = TRUE;
	m_iOriginStep = 1000;

	m_dCurrentAxisPos = 0.0;
	m_dTargetAxisPos = 0.0;
	m_dRMoveDistance = 0.0;
	m_bRMoveStart = FALSE;
	return m_iOriginStep;
#endif

	int iResult = ERR_MOTION_SUCCESS;

	int iState;
	double dOriginFastVel[1] = { m_SaxAxis.dScale * m_AxisParameter.dHomeFastVelocity };	// 원점 복귀 Fast 속도
	double dOriginSlowVel[1] = { m_SaxAxis.dScale * m_AxisParameter.dHomeSlowVelocity };	// 원점 복귀 Slow 속도
	int iOriginReturned[1] = { 0 };	// 원점 복귀 완료 여부 1:완료, 0:미완료

	double dSign;
	double dPos, dVel;
	short iAccel, iDecel;
	double dAccel, dDecel;
	
	switch(m_iOriginStep)
	{
		//Amp Off
		//Amp On
		//속도 설정
		//원점 복귀 Buffer Stop -> Run
		//원점 복귀 종료 or 에러 감시
		//원점 복귀 변수 확인
		//원점 복귀 Offset 처리

	case 100:
		m_iOriginError	= 0;
		m_bOriginFlag	= FALSE;

		SetOriginStep(200);
		break;

	case 200: // Amp Off
		iResult = ServoOff();
		if (iResult != ERR_MOTION_SUCCESS)
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}
/*
// ACS 권장 : 원점 복귀 Buffer에서 Amp Off/On 실행
		SetOriginStep(300);
		break;

	case 300: // Amp On
		iResult = ServoOn();
		if (iResult != ERR_MOTION_SUCCESS)
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}
*/
		SetOriginStep(400);
		break;

	case 400: // 원점 복귀 Fast/Slow 속도 설정
		// 원점 복귀 Fast 속도 설정
		iResult = acsc_WriteReal( m_sCommSet.Handle, ACSC_NONE, "OriginFastVel", 0, 0, ACSC_NONE, ACSC_NONE, dOriginFastVel, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117041);
			SetOriginStep(998);
			break;
		}

		// 원점 복귀 Slow 속도 설정
		iResult = acsc_WriteReal( m_sCommSet.Handle, ACSC_NONE, "OriginSlowVel", 0, 0, ACSC_NONE, ACSC_NONE, dOriginSlowVel, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117042);
			SetOriginStep(998);
			break;
		}

		SetOriginStep(500);
		break;

	case 500: // 원점 복귀 Buffer Stop -> Run
		iResult = acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117027);
			SetOriginStep(998);
			break;
		}

		m_bOnOriginReturn = FALSE;

		iResult = acsc_RunBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
			EStop();
			m_iOriginError = generateErrorCode(117043);
			SetOriginStep(998);
			break;
		}

		m_bOnOriginReturn = TRUE;

		SetOriginStep(600);
		m_ttOriginTimer.StartTimer();
		break;

	case 600: // 원점 복귀 Buffer 동작 시작 대기
		if (m_ttOriginTimer.MoreThan(1.0))
			SetOriginStep(650);
		break;

	case 650: // 원점 복귀 종료 or 에러 감시
		iResult = acsc_GetProgramState(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], &iState, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
			EStop();
			m_iOriginError = generateErrorCode(117044);
			SetOriginStep(998);
			break;
		}

		// 원점복귀 종료 확인
		if (iState & ACSC_PST_RUN)
		{
			//원점 복귀 중
			if (m_ttOriginTimer.MoreThan(m_AxisParameter.dOriginLimitTime))
			{
				acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
				EStop();
				m_iOriginError = generateErrorCode(117045);
				SetOriginStep(998);
				break;
			}

			break;
		}

		// 에러 확인
		iResult = acsc_GetFault(m_sCommSet.Handle, m_iACSMotionID, &iState, NULL);
		if (iResult == DEF_ACS_FAIL)
			return generateErrorCode(117025);
		if (iState != 0x00)
		{
			acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
			EStop();
			m_iOriginError = generateErrorCode(117051);
			SetOriginStep(998);
			break;
		}

		SetOriginStep(700);
		break;

	case 700: // 원점 복귀 변수 확인
		if(m_iACSMotionInstance == 0)
			iResult = acsc_ReadInteger(m_sCommSet.Handle, ACSC_NONE, "GantryHome", 0, 0, ACSC_NONE, ACSC_NONE, iOriginReturned, NULL);
		else
			iResult = acsc_ReadInteger(m_sCommSet.Handle, ACSC_NONE, "GantryHome", 1, 1, ACSC_NONE, ACSC_NONE, iOriginReturned, NULL);

		if (iResult == DEF_ACS_FAIL)
		{
			acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
			EStop();
			m_iOriginError = generateErrorCode(117046);
			SetOriginStep(998);
			break;
		}

		if (iOriginReturned[0] == 1)	// 1:완료, 0:미완료
		{
			SetOriginStep(800);
			break;
		}
		else	// iOriginReturned[0] == 0
		{
			/** 원점복귀 실패했습니다. */
			acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
			EStop();
			m_iOriginError = generateErrorCode(117047);
			SetOriginStep(998);
			break;
		}

		break;

	case 800: // 원점 복귀 Offset 확인
		if (m_AxisParameter.dHomeOffset != 0.0
			&& DEF_ORIGIN_RETURN_ALL_MOVE == iOriginMoveType)
		{
			SetOriginStep(850);
			break;
		}

		// 0.0 으로 보낸다.
		dPos = 0.0;
		dVel =  m_SaxAxis.dScale * m_AxisParameter.dHomeSlowVelocity;
		iAccel = m_AxisParameter.iRunAccelerate;
		iDecel = m_AxisParameter.iRunAccelerate;

		dAccel = ConvertmSec2mmPerSquareSec(dVel, iAccel);
		dDecel = ConvertmSec2mmPerSquareSec(dVel, iDecel);

		iResult = acsc_SetVelocity(m_sCommSet.Handle, m_iACSMotionID, dVel, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117021);
			SetOriginStep(998);
			break;
		}

		iResult = acsc_SetAcceleration(m_sCommSet.Handle, m_iACSMotionID, dAccel, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117022);
			SetOriginStep(998);
			break;
		}

		iResult = acsc_SetDeceleration(m_sCommSet.Handle, m_iACSMotionID, dDecel, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117023);
			SetOriginStep(998);
			break;
		}

		//170426 JSH.s
		iResult = acsc_SetJerk(m_sCommSet.Handle, m_iACSMotionID, 4500, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117053);
			SetOriginStep(998);
			break;
		}
		//170426 JSH.e

		iResult = acsc_ToPoint(m_sCommSet.Handle, 0, m_iACSMotionID, dPos, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117024);
			SetOriginStep(998);
			break;
		}

		iResult = Wait4Done();
		if (iResult)
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}

		m_bOnOriginReturn = FALSE;
		m_bOriginFlag = TRUE;

		SetOriginStep(1000);
		break;

	case 850: // 원점 복귀 Offset 처리
		dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;
		dPos = dSign * m_SaxAxis.dScale * m_AxisParameter.dHomeOffset;
		dVel =  m_SaxAxis.dScale * m_AxisParameter.dHomeFastVelocity;
		iAccel = m_AxisParameter.iRunAccelerate;
		iDecel = m_AxisParameter.iRunAccelerate;

		dAccel = ConvertmSec2mmPerSquareSec(dVel, iAccel);
		dDecel = ConvertmSec2mmPerSquareSec(dVel, iDecel);

		iResult = acsc_SetVelocity(m_sCommSet.Handle, m_iACSMotionID, dVel, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117021);
			SetOriginStep(998);
			break;
		}

		iResult = acsc_SetAcceleration(m_sCommSet.Handle, m_iACSMotionID, dAccel, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117022);
			SetOriginStep(998);
			break;
		}

		iResult = acsc_SetDeceleration(m_sCommSet.Handle, m_iACSMotionID, dDecel, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117023);
			SetOriginStep(998);
			break;
		}

		//170426 JSH.s
		iResult = acsc_SetJerk(m_sCommSet.Handle, m_iACSMotionID, 4500, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117053);
			SetOriginStep(998);
			break;
		}
		//170426 JSH.e


		iResult = acsc_ToPoint(m_sCommSet.Handle, 0, m_iACSMotionID, dPos, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117024);
			SetOriginStep(998);
			break;
		}

		iResult = Wait4Done();
		if (iResult)
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}

		iResult = acsc_SetFPosition(m_sCommSet.Handle, m_iACSMotionID, 0.0, NULL);
		if (iResult == DEF_ACS_FAIL)
		{
			m_iOriginError = generateErrorCode(117035);
			SetOriginStep(998);
			break;
		}

		m_bOnOriginReturn = FALSE;
		m_bOriginFlag = TRUE;

		SetOriginStep(1000);
		break;

	case 998:
		acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
		EStop();
		m_bOriginFlag = FALSE;
		SetOriginStep(999);
		break;

	case 999:	//DEF_ORIGIN_ERROR_STEP
		break;
	case 1000:	//DEF_ORIGIN_FINISH_STEP
		break;

	default:
		m_iOriginError = generateErrorCode(117016);
		SetOriginStep(998);
		break;
	}

	return m_iOriginStep;
}

int MACSAxis::StopReturnOrigin(void)
{
	int iResult = ERR_MOTION_SUCCESS;

	// 축 모두에 대해 VStop 명령 수행
	iResult = VStop();

	//Origin Return Buffer 실행을 정지시킨다.
	iResult = acsc_StopBuffer(m_sCommSet.Handle, DEF_ACS_ORIGIN_RETURN_BUFFER[m_iACSMotionInstance], NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117027);

	m_bOnOriginReturn = FALSE;

	/** EStop */
	iResult = acsc_Kill(m_sCommSet.Handle, m_iACSMotionID, NULL);
	if (iResult == DEF_ACS_FAIL)
		return generateErrorCode(117030);

	m_iOriginStep = DEF_ORIGIN_ERROR_STEP;
	m_iOriginError = generateErrorCode(117017);

	return iResult;
}

int MACSAxis::SetOriginStep(int iStep)
{
	m_iOriginPrevStep = m_iOriginStep;
	m_iOriginStep = iStep;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetOriginStep()
{
	return m_iOriginStep;
}

int MACSAxis::ClearOriginError()
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetOriginError(int *piError)
{
	ASSERT(piError);

	*piError = m_iOriginError;

	return ERR_MOTION_SUCCESS;
}


BOOL MACSAxis::IsOnHomeSensor()
{
	//HOME 센서 없음 미사용
	return TRUE;
}

BOOL MACSAxis::IsOnPositiveSensor()
{
	return TRUE;
}

BOOL MACSAxis::IsOnNegativeSensor()
{
	return TRUE;
}

int MACSAxis::SetHomeSensorEvent(int iLimit)
{
	//HOME 센서 없음 미사용
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetHomeSensorLevel(BOOL bLevel)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetPositiveSensorEvent(int iLimit)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetPositiveSensorLevel(BOOL bLevel)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetNegativeSensorEvent(int iLimit)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetNegativeSensorLevel(BOOL bLevel)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::checkAxisParameter(SAxis1 srcAx)
{
	CString strLogMsg;

	/** 축 ID 범위 점검 */
	if ((srcAx.iAxisID < DEF_ACS_MIN_AXIS_NO) || (srcAx.iAxisID > (DEF_ACS_MAX_AXIS_NO - 1)))
		return generateErrorCode(117007);

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::checkAxisState()
{
	int iResult = ERR_MOTION_SUCCESS;
	int iState;

	iState = GetAxisSource();

	if (!(iState & ACSC_MST_ENABLE))
		return generateErrorCode(117018);

	iResult = GetAxisState(&iState); 
	if (iResult)
		return iResult;

	if ((m_bOriginFlag == TRUE) && (iState != 0x00))
	{
		if (iState & ACSC_SAFETY_RL)
		{
			m_bOriginFlag = FALSE;
			return generateErrorCode(117999);	// ERR_MAXES_POSITIVE_LIMIT_AXIS_SOURCE
		}
		else if (iState & ACSC_SAFETY_LL)		// Left Limit
		{
			m_bOriginFlag = FALSE;
			return generateErrorCode(117999);	// ERR_MAXES_NEGATIVE_LIMIT_AXIS_SOURCE
		}
		else if (iState & ACSC_SAFETY_HOT)		// Motor Overheat
			return generateErrorCode(117999);	// ERR_MAXES_VELOCITY_LIMIT_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_SRL)		// Software Right Limit
			return generateErrorCode(117999);	// ERR_MAXES_X_NEGATIVE_LIMIT_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_SLL)		// Software Left Limit
			return generateErrorCode(117999);	// ERR_MAXES_X_POSITIVE_LIMIT_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_ENCNC)	// Primary Encoder Not Connected
			return generateErrorCode(117999);	// ERR_MAXES_ERROR_LIMIT_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_ENC2NC)	// Secondary Encoder Not Connected
			return generateErrorCode(117999);	// ERR_MAXES_PC_COMMAND_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_DRIVE)	// Driver Alarm
			return generateErrorCode(117999);	// ERR_MAXES_OUT_OF_FRAMES_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_ENC)		// Primary Encoder Error
			return generateErrorCode(117999);	// ERR_MAXES_AMP_POWER_ON_OFF_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_ENC2)		// Secondary Encoder Error
			return generateErrorCode(117999);	// ERR_MAXES_RUN_STOP_COMMAND_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_PE)		// Position Error
			return generateErrorCode(117999);	// ERR_MAXES_COLLISION_STATE_AXIS_SOURCE
		else if (iState & ACSC_SAFETY_CPE)		// Critical Position Error
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_VL)		// Velocity Limit
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_AL)		// Acceleration Limit
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_CL)		// Current Limit
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_SP)		// Servo Processor Alarm
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_PROG)		// Program Error
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_MEM)		// Memory Overuse
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_TIME)		// Time Overuse
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_ES)		// Emergency Stop
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_INT)		// Servo Interrupt
			// Error 처리
			return generateErrorCode(117999);
		else if (iState & ACSC_SAFETY_INTGR)	// Integrity Violation
			// Error 처리
			return generateErrorCode(117999);
		else
		{
#ifndef ACS_SIMULATION
			/** 축 발생 Event 해제 */
			iResult = acsc_FaultClear(m_sCommSet.Handle, m_iACSMotionID, NULL);
			if (iResult == DEF_ACS_FAIL)
			{
				//WriteErrorLog("checkAxisState() : Fault Clear failed");
				SetErrorLevel(_T("ACS AXIS check Axis State"),3, __FILE__, __LINE__);
				return generateErrorCode(117031);
			}
#endif
		}
		
	}

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::CheckAxisStateForJog(BOOL bDir)
{
	// Limit 센서 감지 이벤트가 설정 될 수 있어야 한다..
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int	MACSAxis::checkSWLimit(double dPosition)
{
	if (dPosition > m_AxisParameter.dCWSWLimit)
		return generateErrorCode(117019);

	if (dPosition < m_AxisParameter.dCCWSWLimit)
		return generateErrorCode(117020);

	return ERR_MOTION_SUCCESS;
}

int MACSAxis::makeACSError()
{
	int iACSError;

	iACSError = acsc_GetLastError();

//	return (117000+iACSError)*1000+m_iInstanceNo;;
	return generateErrorCode(iACSError);
}

int MACSAxis::SetAxisData()
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetAutoCP(int iBoardNo, BOOL* pbAutoSet)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetAutoCP(int iBoardNo, BOOL* pbAutoSet)
{
	return ERR_MOTION_SUCCESS;
}

void MACSAxis::SetAxisPosReset()
{
	m_dCurrentAxisPos = 0.0;
	m_dTargetAxisPos = 0.0;
}

int MACSAxis::GetRingID()
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::GetNodeID()
{
	return ERR_MOTION_SUCCESS;
}

BOOL MACSAxis::CheckHomeSensor()
{
	return TRUE;
}

BOOL MACSAxis::CheckPositiveSensor()
{
	return TRUE;
}

BOOL MACSAxis::CheckNegativeSensor()
{
	return TRUE;
}

BOOL MACSAxis::GetAmpEnable()
{
	return TRUE;
}

BOOL MACSAxis::GetAmpFault()
{
	return TRUE;
}

int MACSAxis::checkAxisState(int iAxisID)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::ChangePhase(int Phase)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::NodeClear()
{
	char* cmd = "#HWRES#HWRES\r"; 
	
	// example of the waiting call of acsc_Transaction
	
	if (!acsc_Command( m_sCommSet.Handle,                       // communication handle
		
		cmd,    // pointer to the buffer that contains executed controller’s command
		
		strlen(cmd),        // size of this buffer
		
		NULL                                // waiting call
		
		))
	{
		return generateErrorCode(117052);
	}
	else	
		return ERR_MOTION_SUCCESS;
}

double MACSAxis::calculateMotionTime()
{
	double dMotionTime = 0.0;
	double dMovingDistance = 0.0;

	double dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	if (FALSE == m_bRMoveStart)	// PtoP Move
	{
		dMovingDistance = fabs(m_dCurrentAxisPos - dSign * m_dTargetAxisPos);
	}
	else	// R Move
	{
		dMovingDistance = fabs(m_dRMoveDistance);
	}

	double dVel;
	if (m_dVelocity == 0.0)
	{
		if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
			dVel = m_AxisParameter.dRunVelocity;
		else
			dVel = m_AxisParameter.dSlowRunVelocity;
	}
	else
		dVel = dSign * m_dVelocity;

	short iAccel = (m_iAccelerate == 0) ? m_AxisParameter.iRunAccelerate : m_iAccelerate;
	short iDecel = (m_iDecelerate == 0) ? m_AxisParameter.iRunAccelerate : m_iDecelerate;

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

double MACSAxis::ConvertmSec2mmPerSquareSec(double dVel, int iTimeAccel)
{	// a=dv/dt, 사다리꼴 속도 Profile을 사용하면 a=v/t
	// a: mm/sec^2, v: mm/sec, t:msec -> 1/t: 1000/sec
	double dMilliMeterPerSquareSecAccel = 300.0;

	if(iTimeAccel != 0)
		dMilliMeterPerSquareSecAccel = dVel / iTimeAccel * 1000;

	// 가속도값은 (-)가 없음
	dMilliMeterPerSquareSecAccel = fabs(dMilliMeterPerSquareSecAccel);

	return dMilliMeterPerSquareSecAccel;
}

// 미사용 원점 복귀 Buffer 에서 실행
/*
int MACSAxis::GroupGantry()
{
	// Gantry 로 설정
	// 반드시 Amp Off 상태에서 해야 함

	int iResult = ERR_MOTION_SUCCESS;

	iResult = acsc_RunBuffer(m_sCommSet.Handle, DEF_ACS_GANTRY_GROUPING_BUFFER, NULL, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		WriteErrorLog("GroupGantry() : RunBuffer failed");
		return generateErrorCode(117048);
	}

	Sleep(500);

	// Gantry 설정 완료 상태 확인
	int iGantryGroupDone[1] = { 0 };
	iResult = acsc_ReadInteger(m_sCommSet.Handle, ACSC_NONE, "MFLAGS", 0, 0, ACSC_NONE, ACSC_NONE, iGantryGroupDone, NULL);
	if (iResult == DEF_ACS_FAIL)
	{
		WriteErrorLog("GroupGantry() : Gantry Grouping Complete Check failed");
		return generateErrorCode(117049);
	}

	if (!(iGantryGroupDone[0] & 0x80000000))
	{
		WriteErrorLog("GroupGantry() : Gantry Grouping failed");
		return generateErrorCode(117050);
	}

	return ERR_MOTION_SUCCESS;
}
*/
int MACSAxis::HomingStop()
{
	//MMC2 함수
	return ERR_MOTION_SUCCESS;
}
BOOL MACSAxis::IsOriginComplete()
{
	return FALSE;
}
void MACSAxis::ResetOriginComplete()
{
}
// 현재위치에서 Home Sensor 까지의 거리.
double MACSAxis::DistanceToCurrentToHome()
{
	return 0.0;
}

int MACSAxis::SetHomingSwitchSource(int iSourceAxisNodeID)
{
	return ERR_MOTION_SUCCESS;
}

int MACSAxis::SetMotorScale(double dScale)
{
	return ERR_MOTION_SUCCESS;
}
