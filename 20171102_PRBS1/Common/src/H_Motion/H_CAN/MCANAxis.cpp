#include "stdafx.h"
#include <math.h>
#include "MCANAxis.h"

#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


MCANAxis::MCANAxis()
{
	m_canNodeID = 1;                // CANopen node ID

	/** For CAN_SIMULATION */
	m_dCurrentAxisPos = 0.0;
	m_dTargetAxisPos = 0.0;
	m_dRMoveDistance = 0.0;
	m_dVelocity = 0.0;
	m_iAccelerate = 0;
	m_iDecelerate = 0;
	m_bRMoveStart = FALSE;
	m_MotionTimer.StopTimer();
}

MCANAxis::MCANAxis(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO, int canNodeID, MCANSetup* pCANSetup)
: ICommon(commonData)
{
	int iResult=0;
	double dScale=0.0;
	CString strErr;

	m_canNodeID = canNodeID; // CANopen node ID

	InitializeCriticalSection(&m_csProcess);

#ifndef CAN_SIMULATION
	// Initialize the amplifier using default settings
	//	Amp amp;
	const Error *err = m_amp.Init( pCANSetup->m_canOpen, m_canNodeID );
	if (err)
	{
		// Error 처리
	}
#endif

	// Count Per Unit, Count Per 1mm
	dScale = datComponent.m_sAxisVal.dScale;
	m_amp.SetCountsPerUnit(dScale);
	
	/** 1축 구성 member variable 초기화 */
	SetData(&datComponent);

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

	m_amp.ClearNodeGuardEvent();

	InitializeCriticalSection(&m_csProcess);

	/** Timer 개체 생성 */
	m_ttOriginTimer = MTickTimer();

	/** For CAN_SIMULATION */
	m_dCurrentAxisPos = 0.0;
	m_dTargetAxisPos = 0.0;
	m_dRMoveDistance = 0.0;
	m_dVelocity = 0.0;
	m_iAccelerate = 0;
	m_iDecelerate = 0;
	m_bRMoveStart = FALSE;
	m_MotionTimer.StopTimer();
}

MCANAxis::~MCANAxis()
{
}

int MCANAxis::SetData(SSMotionData* pData)
{
	m_strFileName	= pData->m_strFileName;	//	데이타 파일 이름

	swprintf( m_SaxAxis.szName, _T("%s"), pData->m_sAxisVal.szName);

	// 생성자를 통해 시스템에서 받은 데이타를 SAxisParameter 형태로 변환한다.
	m_SaxAxis.iAxisID			= pData->m_sAxisVal.iAxisID;			// CAN B'd전체 분배
	m_SaxAxis.iOriginPriority	= pData->m_sAxisVal.iOriginPriority;
	m_SaxAxis.bSign				= pData->m_sAxisVal.bSign;
 	m_SaxAxis.bOriginDir		= pData->m_sAxisVal.bOriginDir;
	m_SaxAxis.bOriginFineDir	= pData->m_sAxisVal.bOriginFineDir;
 	m_SaxAxis.iCPhaseUseType	= pData->m_sAxisVal.iCPhaseUseType;
	m_SaxAxis.dScale			= pData->m_sAxisVal.dScale;
	m_SaxAxis.iRoleOfHome		= pData->m_sAxisVal.iRoleOfHome;
	m_SaxAxis.bAbsEncoder		= pData->m_sAxisVal.bAbsEncoder;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::GetData(SSMotionData* pData)
{
	swprintf( pData->m_sAxisVal.szName, _T("%s"), m_SaxAxis.szName);

	// 생성자를 통해 시스템에서 받은 데이타를 SAxisParameter 형태로 변환한다.
	pData->m_sAxisVal.iAxisID			= m_SaxAxis.iAxisID;				// CAN B'd전체 분배
	pData->m_sAxisVal.iOriginPriority	= m_SaxAxis.iOriginPriority;	
	pData->m_sAxisVal.bSign				= m_SaxAxis.bSign;				
 	pData->m_sAxisVal.bOriginDir		= m_SaxAxis.bOriginDir;		
	pData->m_sAxisVal.bOriginFineDir	= m_SaxAxis.bOriginFineDir;	
 	pData->m_sAxisVal.iCPhaseUseType	= m_SaxAxis.iCPhaseUseType;		
	pData->m_sAxisVal.dScale			= m_SaxAxis.dScale;			
	pData->m_sAxisVal.bAbsEncoder		= m_SaxAxis.bAbsEncoder;				

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis)
{
	/* 아무 작업도 하지 않음. 이 코드 삭제할 것인지 확인 필요.
	int iResult = ERR_MOTION_SUCCESS;
	BOOL bUse = FALSE;
	CString strLogMsg;

	// Data 하나씩 범위 검사하며 설정

	if (getUseAxis())		
	{
		strLogMsg.Format("Initialize(): Already Used Axis ID (%d)", saxAxis.iAxisID);
		WriteErrorLog(strLogMsg);


		return generateErrorCode(117006);
	}

	// 축 사용으로 설정
	if ((iResult=setUseAxis(TRUE)) != ERR_MOTION_SUCCESS)
		return iResult;
	*/

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::setUseAxis (BOOL bState)
{
	return ERR_MOTION_SUCCESS;
}

BOOL MCANAxis::getUseAxis()
{
	return ERR_MOTION_SUCCESS; 
}

int	MCANAxis::LoadParameter()
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
		return generateErrorCode(119159);

	/** 시스템에서 받은 데이타와 로드한 데이타를 가지고 Component를 초기화 한다. */
	iResult = SetInPositionData();

	return iResult;
}

/** Component에 데이타를 저장한다. 이때 파일에 같이 저장한다.
 * @param   *pData       : 설정할 각 축의 설정 Data
 * @return	Error : 0 = Success, 그외=기타
 */
int MCANAxis::SaveParameter(MAxisData* pData)
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
		iResult = generateErrorCode(119160);

	/** 데이타를 Component 변수에 저장한다. */
	iResult = SetInPositionData();

	return iResult;
}

void MCANAxis::GetParameter(MAxisData* pData)
{
	*pData	= m_AxisParameter;
}

int MCANAxis::GetAxisData (SAxis1 *pax1Data)
{
	// 미사용
	ASSERT(pax1Data);
	*pax1Data = m_SaxAxis;
	
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetAxisConfig(SMotionAxis sMotionAxis)
{
	// 미사용
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::GetAxisConfig(SMotionAxis* psMotionAxis)
{
	// 미사용
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::Move (double dPosition, 
				 double dVelocity, 
				 int iAccelerate, 
				 int iDecelerate, 
				 int iMoveType)
{
	int iResult = ERR_MOTION_SUCCESS;
	CString strLogMsg;

	/** 이동 전 원점복귀 여부 확인 */
	if (!m_bOriginFlag)
		return generateErrorCode(119161);

	if ((iMoveType == DEF_MOVE_DISTANCE) 
		|| (iMoveType == DEF_SMOVE_DISTANCE)
		|| (iMoveType == DEF_TMOVE_DISTANCE) 
		|| (iMoveType == DEF_TSMOVE_DISTANCE))
		return generateErrorCode(119162);

	if ((iResult = StartMove(dPosition, dVelocity, iAccelerate,
							 iDecelerate, iMoveType)) != ERR_MOTION_SUCCESS)
		return iResult;

	if ((iResult = Wait4Done()) != ERR_MOTION_SUCCESS)
		return iResult;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::StartMove (double dPosition, 
					  double dVelocity, 
					  int iAccelerate, 
					  int iDecelerate, 
					  int iMoveType)
{
#ifdef CAN_SIMULATION
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

	//Move 시마다 EventClear를 해준다. 
	m_amp.ClearNodeGuardEvent();

	/** 이동 전 원점복귀 여부 확인 */
	if (!m_bOriginFlag)
		return generateErrorCode(119161);

	if ((iMoveType == DEF_MOVE_DISTANCE) 
		|| (iMoveType == DEF_SMOVE_DISTANCE)
		|| (iMoveType == DEF_TMOVE_DISTANCE) 
		|| (iMoveType == DEF_TSMOVE_DISTANCE))
		return generateErrorCode(119162);

	/** 축 상태 점검 */
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
	{
		SetErrorLevel(_T("CAN AXIS start Move"),3, __FILE__, __LINE__);
		return iResult;
	}

	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	dPos = dSign * dPosition;

	/** 이동 속도 처리 */
	if (dVelocity == 0.0)
	{
		if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)			
			dVel = m_AxisParameter.dRunVelocity;
		else
			dVel = m_AxisParameter.dSlowRunVelocity;
	}
	else
		dVel =  dVelocity;

	/** 이동 가속도 처리 */
	if (dVelocity == 0.0)
	{
		if (iAccelerate == 0)
		{
			if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
				iAccel = (int)(GetMovingVelocity()/((double)GetMovingAccelerate()/1000));
			else
				iAccel = (int)(GetSlowMovingVelocity()/((double)GetMovingAccelerate()/1000));
		}
		else
		{
			if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
				iAccel = (int)(GetMovingVelocity()/((double)iAccelerate/1000));
			else
				iAccel = (int)(GetSlowMovingVelocity()/((double)iAccelerate/1000));
		}
	}
	else
	{
		if (iAccelerate == 0)
			iAccel = (int)(dVelocity/((double)GetMovingAccelerate()/1000));
		else
			iAccel = (int)(dVelocity/((double)iAccelerate/1000));
	}

	/** 이동 감속도 처리 */
	if (dVelocity == 0.0)
	{
		if (iDecelerate == 0)
		{
			if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
				iDecel = (int)(GetMovingVelocity()/((double)GetMovingAccelerate()/1000));
			else
				iDecel = (int)(GetSlowMovingVelocity()/((double)GetMovingAccelerate()/1000));
		}
		else
		{
			if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
				iDecel = (int)(GetMovingVelocity()/((double)iDecelerate/1000));
			else
				iDecel = (int)(GetSlowMovingVelocity()/((double)iDecelerate/1000));
		}
	}
	else
	{
		if (iDecelerate == 0)
			iDecel = (int)(dVelocity/((double)GetMovingAccelerate()/1000));
		else
			iDecel = (int)(dVelocity/((double)iDecelerate/1000));
	}


	/** SW Limit Check */
	if ((iResult = checkSWLimit(dPosition)) != ERR_MOTION_SUCCESS)
	{
		SetErrorLevel(_T("CAN AXIS start Move"),3, __FILE__, __LINE__);
		return iResult;
	}

	::EnterCriticalSection(&m_csProcess);

	if (iMoveType == DEF_MOVE_POSITION)
	{
		// Move 함수 Parameter 설정을 하기위한 변수 
		ProfileConfigTrap ConfigTrap;
		const Error *err;
		
		// 사디리꼴 Profile 구성 
		ConfigTrap.acc = iAccel;
		ConfigTrap.dec = iDecel;
		ConfigTrap.pos = dPos;
		ConfigTrap.vel = dVel;
		
		/** Timer Start */
		m_ttMovingTimer.StartTimer();
		
		// Setup the move
		err = m_amp.DoMove( ConfigTrap );	
		if (err)
		{
			::LeaveCriticalSection(&m_csProcess);
			iResult = err->GetID();
			// Error 처리
			//		WriteErrorLog("VMove() : VMove Start failed");
			SetErrorLevel(_T("CAN AXIS start Move"),3, __FILE__, __LINE__);
			return makeCanVcmError(iResult);
		}
	}
	else
	{
		// Move 함수 Parameter 설정을 하기위한 변수 
		ProfileConfigScurve scurve;
		const Error *err;
		
		// S-Curve Profile 구성 
		scurve.acc = iAccel;
		scurve.jrk = iAccel;
		scurve.pos = dPos;
		scurve.vel = dVel;
		
		/** Timer Start */
		m_ttMovingTimer.StartTimer();
		
		// Setup the move
		err = m_amp.DoMove( scurve );
		if (err)
		{
			::LeaveCriticalSection(&m_csProcess);
			iResult = err->GetID();
			// Error 처리
			//		WriteErrorLog("VMove() : VMove Start failed");
			SetErrorLevel(_T("CAN AXIS start Move"),3, __FILE__, __LINE__);
			return makeCanVcmError(iResult);
		}
	}
	::LeaveCriticalSection(&m_csProcess);
	
	double dElaspTime;
	CString strLog;
	dElaspTime = m_ttMovingTimer.GetElapseTime();
//	strLog.Format(_T("Start Move Communication Time : %f\n"), dElaspTime);
//	TRACE(strLog);
	
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::RMove (double dDistance, 
				  double dVelocity,
				  int iAccelerate,
				  int iDecelerate,
				  int iMoveType, 
				  BOOL bClearOpt)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::StartRMove (double dDistance, 
					   double dVelocity, 
					   int iAccelerate,
					   int iDecelerate, 
					   int iMoveType, 
					   BOOL bClearOpt)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::VMove (double dVelocity, int iAccelerate, BOOL bDir)
{
	int iResult = ERR_MOTION_SUCCESS;
	const Error *err;

	double dSign, dDir, dVel;
	int iAccel;

	// 속도 설정을 하기위한 변수 
	ProfileConfigVel ConfigVel;

	/** 축 관련 오류 점검 */
	err = m_amp.CheckStateForMove();
	if (err)
	{
		SetErrorLevel(_T("CAN AXIS Vmove"),3, __FILE__, __LINE__);
		//WriteErrorLog("VMove() : VMove Start failed");
		return generateErrorCode(119175);
	}
   
	
	/** (+) 방향 */
	if (bDir == TRUE) dDir = 1.0;
	/** (-) 방향 */
	else dDir = -1.0;

	/** 축 상태 점검 */
//	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
//		return iResult;

	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	/** 이동 속도 처리 */
	if (dVelocity == 0.0)
	{
		if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
			dVel = m_AxisParameter.dRunVelocity;
		else
			dVel = m_AxisParameter.dSlowRunVelocity;
	}
	else
		dVel = dVelocity;

	/** 이동 가속도 처리 */
	if (dVelocity == 0.0)
	{
		if (iAccelerate == 0)
		{
			if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
				iAccel = (int)(GetMovingVelocity()/((double)GetMovingAccelerate()/1000));
			else
				iAccel = (int)(GetSlowMovingVelocity()/((double)GetMovingAccelerate()/1000));
		}
		else
		{
			if (m_AxisParameter.eVelocityMode == VELOCITY_MODE_NORMAL)
				iAccel = (int)(GetMovingVelocity()/((double)iAccelerate/1000));
			else
				iAccel = (int)(GetSlowMovingVelocity()/((double)iAccelerate/1000));
		}
	}
	else
	{
		if (iAccelerate == 0)
			iAccel = (int)(dVelocity/((double)GetMovingAccelerate()/1000));
		else
			iAccel = (int)(dVelocity/((double)iAccelerate/1000));
	}

	ConfigVel.dir = dDir;
	ConfigVel.vel = dVel;
	ConfigVel.acc = iAccel;
	ConfigVel.dec = iAccel;

   // Setup the move
   err = m_amp.SetupMove( ConfigVel );
	if (err)
	{
		iResult = err->GetID();
		// Error 처리
		//WriteErrorLog("VMove() : VMove Setup failed");
		SetErrorLevel(_T("CAN AXIS Vmove"),3, __FILE__, __LINE__);
		return makeCanVcmError(iResult);
	}

	err = m_amp.StartMove( false );
   
	if (err)
	{
		iResult = err->GetID();
		// Error 처리
	   //WriteErrorLog("VMove() : VMove Start failed");
		SetErrorLevel(_T("CAN AXIS Vmove"),3, __FILE__, __LINE__);
		return makeCanVcmError(iResult);
	}
      
   return ERR_MOTION_SUCCESS;
}

int MCANAxis::Wait4Done(BOOL bSkipMode, BOOL bUseInterlock, BOOL bRMove)
{
#ifdef CAN_SIMULATION
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
			{
				SetErrorLevel(_T("CAN AXIS wait for Done"),3, __FILE__, __LINE__);
				// 119163 = It exceeded action time limit.
				return generateErrorCode(119163);
			}
			
			Sleep(1);
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
	double	dCurrentPos, dTargetPos;
	const Error *err;
	
	err = m_amp.WaitMoveDone((int)m_AxisParameter.dLimitTime * 1000);
	if (err)
	{
		iResult = err->GetID();
		// Error 처리
	   //WriteErrorLog("Wait4Done() : Wait4Done failed");
		SetErrorLevel(_T("CAN AXIS wait for Done"),3, __FILE__, __LINE__);
		return makeCanVcmError(iResult);
	}
  
	/** 축 상태 점검 */
	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
	{
		SetErrorLevel(_T("CAN AXIS wait for Done"),3, __FILE__, __LINE__);
		return iResult;
	}

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
			{
				CString strTemp;
				//strTemp.Format(_T("Wait4Done() : Not In Position(Current : %lf, Command : %lf"), dCurrentPos, dTargetPos);
				//WriteErrorLog(strTemp);
				SetErrorLevel(_T("CAN AXIS wait for Done"),3, __FILE__, __LINE__);

				// 119166 = It is not In Position state after action completion.
				return generateErrorCode(119166);
			}

			/* 1. Encoder 읽기 */
			dCurrentPos = GetCurrentPosition(DEF_POS_ENCODER);			
			/* 2. Command 읽기 */
			dTargetPos = GetCurrentPosition(DEF_POS_COMMAND);
			
			iRetryCount++;
		}
	}

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::MoveSplineLine (int iMaxPoint, 
						   double* pdPosition, 
						   double* pdVelocity, 
						   int* piAccelerate,
						   BOOL bAutoFlag, 
						   BOOL bWaitOpt)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::JogMoveSlow(BOOL bDir, double dSlow)// = 0.0)
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
			SetErrorLevel(_T("CAN AXIS move Jog Slow"),3, __FILE__, __LINE__);
			return iResult;
		}
	}
	else
	{
		iResult = VMove(dSlow, 0, bDir);
		if (ERR_MOTION_SUCCESS != iResult)
		{
			VStop();
			SetErrorLevel(_T("CAN AXIS move Jog Slow"),3, __FILE__, __LINE__);
			return iResult;
		}
	}
	
	if (IsAmpFault())
	{
		VStop();
		//WriteErrorLog("JogMoveSlow() : JogMoveSlow Fault");
		SetErrorLevel(_T("CAN AXIS move Jog Slow"),3, __FILE__, __LINE__);
		return generateErrorCode(119165);
	}

	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
	{
		VStop();
		SetErrorLevel(_T("CAN AXIS move Jog Slow"),3, __FILE__, __LINE__);
		return iResult;
	}

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::JogMoveFast(BOOL bDir, double dFast)// = 0.0)
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
			SetErrorLevel(_T("CAN AXIS move Jog Fast"),3, __FILE__, __LINE__);
			return iResult;
		}
	}
	else
	{
		iResult = VMove(dFast, 0, bDir);
		if (ERR_MOTION_SUCCESS != iResult)
		{
			VStop();
			SetErrorLevel(_T("CAN AXIS move Jog Fast"),3, __FILE__, __LINE__);
			return iResult;
		}
	}
	
	if (IsAmpFault())
	{
		VStop();
		//WriteErrorLog("checkAxisState() : Amp Fault");
		SetErrorLevel(_T("CAN AXIS move Jog Fast"),3, __FILE__, __LINE__);
		return generateErrorCode(119165);
	}

	if ((iResult = checkAxisState()) != ERR_MOTION_SUCCESS)
	{
		VStop();
		SetErrorLevel(_T("CAN AXIS move Jog Fast"),3, __FILE__, __LINE__);
		return iResult;
	}
	

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetEStopRate()
{

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::JogStop()
{
	int iResult = ERR_MOTION_SUCCESS;

	iResult = VStop();
	SetErrorLevel(_T("CAN AXIS stop Jog"),3, __FILE__, __LINE__);
	return iResult;
}

int MCANAxis::Stop(BOOL *pbStatus)
{
	const Error *err = m_amp.HaltMove();
	if (err)
	{
		//WriteErrorLog("Stop() : Stop failed");
		SetErrorLevel(_T("CAN AXIS stop"),3, __FILE__, __LINE__);
		return generateErrorCode(119180);
	}
	
	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;
	
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::VStop(BOOL *pbStatus)
{
   const Error *err = m_amp.HaltMove();
	if (err)
	{
		//WriteErrorLog("VStop() : VStop failed");
		SetErrorLevel(_T("CAN AXIS Vstop"),3, __FILE__, __LINE__);
		return generateErrorCode(119180);
	}
	/** VStop 후 안정화 시간 적용 - 2004.9.12 반용석K */
	Sleep((ULONG)(m_AxisParameter.dStabilityTime));

	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::EStop(BOOL *pbStatus)// = NULL)
{
	const Error *err = m_amp.HaltMove();	
	if (err)
	{
		SetErrorLevel(_T("CAN AXIS Vstop"),3, __FILE__, __LINE__);
		//WriteErrorLog("VStop() : VStop failed");
		return generateErrorCode(119180);
	}
	/** VStop 후 안정화 시간 적용 - 2004.9.12 반용석K */
	Sleep((ULONG)(m_AxisParameter.dStabilityTime));
	
	/** 결과 기록 */
	if (pbStatus != NULL)
		*pbStatus = TRUE;
	
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::ClearAxis(BOOL *pbStatus)// = NULL)
{
	const Error *err;
	
	err = m_amp.ClearFaults();
	if (err)
	{
		SetErrorLevel(_T("CAN AXIS clear Axis"),3, __FILE__, __LINE__);
		//WriteErrorLog("ResetAmpFault() : Fault Reset failed");
		return generateErrorCode(119176);
	}
	
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetStopRate(short siRate)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetEStopRate(short siRate)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetEncoderDirection(BOOL bEncDir, BOOL bCoorDir)
{
//#ifndef CAN_SIMULATION
//	int iResult = ERR_MOTION_SUCCESS;
//	double dEncDir;

//	dEncDir = (bEncDir) ? 1 : 0;

	/** Encoder 입력 방향 설정 */
//	iResult = acsc_SetConf(m_sCommSet.Handle, ACSC_CONF_ENCODER_KEY, ACSC_AXIS_X, dEncDir, NULL);
//	if (iResult == DEF_ACS_FAIL)
//	{
//		WriteErrorLog("SetEncoderDirection() : Setting Encoder Direction failed");
//		return generateErrorCode(117032);
//	}
//#endif
	return ERR_MOTION_SUCCESS;
}


int MCANAxis::GetAxisSource()
{
//	AMP_EVENT eAmpstatus;

	return ERR_MOTION_SUCCESS;

}

int MCANAxis::GetAxisState(int *piReturn)
{
	EVENT_STATUS status;
	int iResult = 0;

	/** 축 Event 발생 내역 읽기 */
	const Error *err = m_amp.GetEventStatus(status);
	if (err)
	{
		iResult = err->GetID();
		// Error 처리
//	   WriteErrorLog("VMove() : VMove Start failed");
		SetErrorLevel(_T("CAN AXIS clear Axis"),3, __FILE__, __LINE__);
		return makeCanVcmError(iResult);
	}
  
/*	if (status == ESTAT_SHORT_CRCT) ///< Amplifier short circuit 
	{
		WriteErrorLog("GetAxisSource() : Getting Motor State failed");
		return generateErrorCode(117034);
	}
	else if (status == ESTAT_AMP_TEMP) ///< Amplifier over temperature	return status;
	{
	}
	else if (status == ESTAT_OVER_VOLT) ///< Amplifier over voltage}
	{
	}
	else if (status == ESTAT_UNDER_VOLT) ///< Amplifier under voltage
	{
	}
	else if (status == ESTAT_MTR_TEMP) ///< Motor over temperatureint 
	{
	}
	else if (status == ESTAT_ENCODER_PWR) ///< Encoder power error
	{
	}
	else if (status == ESTAT_PHASE_ERR) ///< Phasing error
	{
	}
	else if (status == ESTAT_CRNT_LIM) ///< Current limited
	{
	}
	else if (status == ESTAT_VOLT_LIM) ///< Voltage limited
	{
	}
	else if (status == ESTAT_POSLIM) ///< Positive limit switch triggered
	{
	}
	else if (status == ESTAT_NEGLIM) ///< Negative limit switch triggered
	{
	}
	else if (status == ESTAT_DISABLE_INPUT) ///< Enable input pin not set
	{
	}
	else if (status == ESTAT_SOFT_DISABLE) ///< Disabled due to software request
	{
	}
	else if (status == ESTAT_STOP) ///< Try to stop motor (after disable, before brake)
	{
	}
	else if (status == ESTAT_BRAKE) ///< Brake actuated
	{
	}
	else if (status == ESTAT_PWM_DISABLE) ///< PWM outputs disabled
	{
	}
	else if (status == ESTAT_SOFTLIM_POS) ///< Positive software limit reached
	{
	}
	else if (status == ESTAT_SOFTLIM_NEG) ///< Negative software limit reached
	{
	}
	else if (status == ESTAT_TRK_ERR) ///< Tracking error
	{
	}
	else if (status == ESTAT_TRK_WARN) ///< Tracking warning
	{
	}
	else if (status == ESTAT_RESET) ///< Amplifier has been reset
	{
	}
	else if (status == ESTAT_POSWRAP) ///< Encoder position wrapped (rotory) or hit limit (linear).
	{
	}
	else if (status == ESTAT_FAULT) ///< Latching fault in effect
	{
	}
	else if (status == ESTAT_VEL_LIMIT) ///< Velocity is at limit
	{
	}
	else if (status == ESTAT_ACC_LIMIT) ///< Acceleration is at limit
	{
	}
	else if (status == ESTAT_TRK_WIN) ///< Not in tracking window if set
	{
	}
	else if (status == ESTAT_HOME) ///< Home switch is active
	{
	}
	else if (status == ESTAT_MOVING) ///< Trajectory generator active OR not yet settled
	{
	}
	else if (status == ESTAT_VEL_WIN) ///< Velocity error outside of velocity window when set.
	{
	}
	else if (status == ESTAT_PHASE_INIT) ///< Set when using algorithmic phase init mode & phase not initialized.
	{
	}
*/
	
	*piReturn = status;

	return ERR_MOTION_SUCCESS;
}

BOOL MCANAxis::IsAxisDone()
{
	return FALSE;
}

BOOL MCANAxis::IsInMotion()
{
	return FALSE;
}

BOOL MCANAxis::IsMotionDone()
{
	return FALSE;
}

int MCANAxis::GetComVelocity()
{
	return ERR_MOTION_SUCCESS;
}

BOOL MCANAxis::IsEStopEvent()
{
	//수정해야함
	return FALSE;
}

double MCANAxis::GetCurrentPosition(BOOL bCommandType)
{
#ifdef CAN_SIMULATION
	return m_dCurrentAxisPos;
#endif

	double dSign;
	double dCurrentPosition;
	const Error *err;

	int iResult = ERR_MOTION_SUCCESS;

	if (bCommandType == DEF_POS_ENCODER)
	{		
		/** 축의 실제위치 읽기 */
		err = m_amp.GetPositionMotor(dCurrentPosition);
		if (err)
		{
			//WriteErrorLog("GetCurrentPosition() : Getting Current Real Position failed");
			SetErrorLevel(_T("CAN AXIS clear Axis"),3, __FILE__, __LINE__);
			return generateErrorCode(119187);
		}		
	}
	else	// DEF_POS_COMMAND
	{
		/** 축의 목표위치 읽기 */
		err = m_amp.GetPositionCommand(dCurrentPosition);
		if (err)
		{
			//WriteErrorLog("GetCurrentPosition() : Getting Current Target Position failed");
			SetErrorLevel(_T("CAN AXIS clear Axis"),3, __FILE__, __LINE__);
			return generateErrorCode(119189);
		}
	}

	/** Encoder가 CCW Type이면 값은 역상이 되어야 한다. */
	dSign = (m_SaxAxis.bSign) ? 1.0 : -1.0;

	dCurrentPosition *= (dSign);
	
	return dCurrentPosition;
}

int MCANAxis::SetCurrentPosition(double dCurrentPosition, BOOL bType)
{
	return ERR_MOTION_SUCCESS;
}

BOOL MCANAxis::ComparePosition(BOOL bPosOpt, double dTargetPosition,
					 double *pdPermission, double *pdDeviation)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;
	
	/** 현재 위치 읽기 */
#ifdef CAN_SIMULATION
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

BOOL MCANAxis::IsLessThanTargetPosition(double dTargetPosition,
					 double *pdPermission)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;
	
	/** 현재 위치 읽기 */
#ifdef CAN_SIMULATION
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

BOOL MCANAxis::IsGreaterThanTargetPosition(double dTargetPosition,
					 double *pdPermission)
{
	double dCurPosition = 0.0;
	double dDevPosition = 0.0;
	
	/** 현재 위치 읽기 */
#ifdef CAN_SIMULATION
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


int MCANAxis::SetAmpEnable(BOOL bEnable)
{
	const Error *err;

	/** 축의 AMP Enable/Disable 설정 */
	if (bEnable == TRUE)
	{
		err = m_amp.Enable();
		if (err)	
		{
			//WriteErrorLog("Servo SetAmpEnable() : Servo On failed");
			SetErrorLevel(_T("CAN AXIS set Amp Enable"),3, __FILE__, __LINE__);
			return generateErrorCode(119190);
		}		
	}
	else
	{
		err = m_amp.Disable();
		if (err)	
		{
			//WriteErrorLog("Servo SetAmpEnable() : Servi Off failed");
			SetErrorLevel(_T("CAN AXIS set Amp Enable"),3, __FILE__, __LINE__);
			return generateErrorCode(119191);
		}		
	}

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetAmpEnableLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

BOOL MCANAxis::IsAmpEnable()
{
	int iResult = ERR_MOTION_SUCCESS;


	return TRUE;
}

BOOL MCANAxis::IsAmpFault()
{
	AMP_FAULT eState;
	const Error *err;

	err = m_amp.GetFaults(eState);
	if (err)
	{
		//WriteErrorLog("IsAmpFault() : Fault Check failed");
		SetErrorLevel(_T("CAN AXIS check Amp Fault"),3, __FILE__, __LINE__);
		return generateErrorCode(119176);
	}

	if (eState != 0)
		return TRUE;
	else
		return FALSE;
}

int MCANAxis::ResetAmpFault(BOOL bIsCheck)
{
	const Error *err;
	
	err = m_amp.ClearFaults();
	if (err)
	{
		//WriteErrorLog("ResetAmpFault() : Fault Reset failed");
		SetErrorLevel(_T("CAN AXIS reset Amp Fault"),3, __FILE__, __LINE__);
		return generateErrorCode(119182);
	}
	
	return ERR_MOTION_SUCCESS;
}



int MCANAxis::SetAmpResetLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetAmpFaultEnable()
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetAmpFaultLevel(BOOL bLevel)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::ServoOn(BOOL *pbStatus, BOOL bIsCheck)
{
	int iResult = ERR_MOTION_SUCCESS;

	iResult = ClearAxis();
	if (iResult)
	{
		SetErrorLevel(_T("CAN AXIS on Servo"),3, __FILE__, __LINE__);
		return iResult;
	}

	iResult = SetAmpEnable(TRUE);
	if (iResult)
	{
		SetErrorLevel(_T("CAN AXIS on Servo"),3, __FILE__, __LINE__);
		return iResult;
	}

	Sleep(10);

	if (pbStatus != NULL)
	{
		// AMP Enable 상태 읽기
		*pbStatus = IsAmpEnable();		
	}

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::ServoOff(BOOL *pbStatus, BOOL bIsCheck)
{
	int iResult = ERR_MOTION_SUCCESS;
		
	// 원점복귀 Flag Reest
	ResetOrigin();

	iResult = SetAmpEnable(FALSE);
	if (iResult)
	{
		SetErrorLevel(_T("CAN AXIS off Servo"),3, __FILE__, __LINE__);
		return iResult;
	}

	Sleep(10);

	if (pbStatus != NULL)
	{
		// AMP Enable 상태 읽기
		*pbStatus = IsAmpEnable();		
	}

	return ERR_MOTION_SUCCESS;
}


CString MCANAxis::GetAxisName()
{
	CString strName;
	
	strName.Format(_T("%s"), m_SaxAxis.szName);
	return strName;
}

void MCANAxis::SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue)
{
	m_usiInterlockIOAddr = usiIOAddress;
	m_bInterlockJudge = bJudgeValue;
}

int MCANAxis::SetInPositionData()
{
	int		iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::GetAxisID()
{
	return m_SaxAxis.iAxisID;	
}

double MCANAxis::GetHomeOffset()
{
	return m_AxisParameter.dHomeOffset;	
}

double MCANAxis::GetNegativeSWLimit()
{
	return m_AxisParameter.dCCWSWLimit;	
}

double MCANAxis::GetPositiveSWLimit()
{
	return m_AxisParameter.dCWSWLimit;	
}

double MCANAxis::GetSlowMovingVelocity()
{
	return m_AxisParameter.dSlowRunVelocity;
}

double MCANAxis::GetMovingVelocity()
{
	return m_AxisParameter.dRunVelocity;
}

int MCANAxis::GetMovingAccelerate()
{
	return m_AxisParameter.iRunAccelerate;
}

double MCANAxis::GetCoarseVelocity()
{
	return m_AxisParameter.dHomeFastVelocity;	
}

int MCANAxis::GetCoarseAccelerate()
{
	return m_AxisParameter.iHomeAccelerate;
}

double MCANAxis::GetFineVelocity()
{
	return m_AxisParameter.dHomeSlowVelocity;
}

int MCANAxis::GetFineAccelerate()
{
	return m_AxisParameter.iHomeAccelerate;
}

double MCANAxis::GetJogSlow()
{
	return m_AxisParameter.dJogSlow;	
}

double MCANAxis::GetJogFast()
{
	return m_AxisParameter.dJogFast;
}

BOOL MCANAxis::GetSign()
{
	return m_SaxAxis.bSign;
}

BOOL MCANAxis::IsAbsEncoder()
{
	return m_SaxAxis.bAbsEncoder;
}

BOOL MCANAxis::GetOriginDir()
{
	return m_SaxAxis.bOriginDir;
}

BOOL MCANAxis::GetOriginFineDir()
{
	return m_SaxAxis.bOriginFineDir;
}

int MCANAxis::GetCPhaseUseType()
{
	return m_SaxAxis.iCPhaseUseType;
}

double MCANAxis::GetScale()
{
	return m_SaxAxis.dScale;
}

double MCANAxis::GetMoveTime()
{
	return m_AxisParameter.dLimitTime;
}

double MCANAxis::GetMoveAfterTime()
{
	return m_AxisParameter.dStabilityTime;
}

double MCANAxis::GetTolerance()
{
	return m_AxisParameter.dTolerance;
}

double MCANAxis::GetPassOffset()
{
	return m_AxisParameter.dPassOffset;
}

ERoleOfHome MCANAxis::GetRoleOfHomeSensor()
{
	return m_SaxAxis.iRoleOfHome;
}

double MCANAxis::GetOriginWaitTime()
{
	return m_AxisParameter.dOriginLimitTime;
}

BOOL MCANAxis::IsOriginReturn()
{
	return m_bOriginFlag;
}

void MCANAxis::SetOriginFlag()
{
	m_bOriginFlag = TRUE;
}

int MCANAxis::ResetOrigin(BOOL *pbReturn)
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

int MCANAxis::GetIndexRequired(BOOL *pbIndexReq)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetIndexRequired(BOOL bIndexReq)
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::GetOriginPriority()
{
	return m_SaxAxis.iOriginPriority;
}

int MCANAxis::OriginReturn(int iOriginMoveType)
{
	// 확인 필요 : 
#ifdef CAN_SIMULATION
	m_bOriginFlag = TRUE;
	m_iOriginStep = 1000;

	m_dCurrentAxisPos = 0.0;
	m_dTargetAxisPos = 0.0;
	m_dRMoveDistance = 0.0;
	m_bRMoveStart = FALSE;
	return m_iOriginStep;
#endif

	int iResult = ERR_MOTION_SUCCESS;

	int iState = -1;
	double dOriginFastVel = m_AxisParameter.dHomeFastVelocity;	// 원점 복귀 Fast 속도
	double dOriginSlowVel = m_AxisParameter.dHomeSlowVelocity;	// 원점 복귀 Slow 속도
	double dOriginAccel = m_AxisParameter.iHomeAccelerate;	// 원점 복귀 Slow 속도
	double dOriginOffset = m_AxisParameter.dHomeOffset;	// 원점 복귀 Slow 속도
	int iOriginReturned = 0;	// 원점 복귀 완료 여부 1:완료, 0:미완료

	HomeConfig cfg;
	cfg.method = CHM_NHOME;
	cfg.offset = dOriginOffset;
	cfg.velFast = dOriginFastVel;
	cfg.velSlow = dOriginSlowVel;
	cfg.accel = dOriginFastVel/(dOriginAccel/1000);

	const Error *err = NULL;
		
	switch (m_iOriginStep)
	{
		//Amp Off
		//Amp On
		//원점 복귀 GoHome
		//원점 복귀 종료 or 에러 감시

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

		SetOriginStep(500);
		break;


	case 500: // Amp On

		m_bOnOriginReturn = TRUE;

		iResult = ServoOn();
		if (iResult != ERR_MOTION_SUCCESS)
		{
			m_iOriginError = iResult;
			SetOriginStep(998);
			break;
		}

		SetOriginStep(700);
		break;

	case 700: // 원점 복귀

		//Move 시마다 EventClear를 해준다. 
		m_amp.ClearNodeGuardEvent();

		err = m_amp.GoHome(cfg);
		if (err)
		{
			iResult = err->GetID();
			// Error 처리
		   //WriteErrorLog("OriginReturn() : GoHome failed");
			SetErrorLevel(_T("CAN AXIS return Origin"),3, __FILE__, __LINE__);
			return makeCanVcmError(iResult);
		}

		err = m_amp.WaitMoveDone( int32(m_AxisParameter.dOriginLimitTime * 1000.0) );
		if (err)
		{
			iResult = err->GetID();
			// Error 처리
		   //WriteErrorLog("OriginReturn() : GoHome Wait failed");
			SetErrorLevel(_T("CAN AXIS return Origin"),3, __FILE__, __LINE__);
			return makeCanVcmError(iResult);
		}

		iOriginReturned=1;

		if (iOriginReturned == 1)	// 1:완료, 0:미완료
		{
			SetOriginStep(800);
			break;
		}
		else	// iOriginReturned == 0
		{
			/** 원점복귀 실패했습니다. */
			EStop();
			//WriteErrorLog("OriginReturn() : Origin Return failed");
			SetErrorLevel(_T("CAN AXIS return Origin"),3, __FILE__, __LINE__);
			m_iOriginError = generateErrorCode(119198);
			SetOriginStep(998);
			break;
		}

		break;

	case 800: 

		m_bOnOriginReturn = FALSE;
		m_bOriginFlag = TRUE;

		SetOriginStep(1000);
		break;


	case 998:
		EStop();
		m_bOriginFlag = FALSE;
		SetOriginStep(999);
		break;

	case 999:	//DEF_ORIGIN_ERROR_STEP
		break;
	case 1000:	//DEF_ORIGIN_FINISH_STEP
		break;

	default:
		SetErrorLevel(_T("CAN AXIS return Origin"),3, __FILE__, __LINE__);
		m_iOriginError = generateErrorCode(119167);
		SetOriginStep(998);
		break;
	}

	return m_iOriginStep;
}

int MCANAxis::StopReturnOrigin(void)
{
	int iResult = ERR_MOTION_SUCCESS;

	// 축 모두에 대해 VStop 명령 수행
	iResult = VStop();

	m_bOnOriginReturn = FALSE;

	m_iOriginStep = DEF_ORIGIN_ERROR_STEP;
	//WriteErrorLog("StopReturnOrigin(): Origin Stop By User");
	SetErrorLevel(_T("CAN AXIS stop Return Origin"),3, __FILE__, __LINE__);
	m_iOriginError = generateErrorCode(119168);

	return iResult;
}

int MCANAxis::SetOriginStep(int iStep)
{
	m_iOriginPrevStep = m_iOriginStep;
	m_iOriginStep = iStep;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::GetOriginStep()
{
	return m_iOriginStep;
}

int MCANAxis::ClearOriginError()
{
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::GetOriginError(int *piError)
{
	ASSERT(piError);

	*piError = m_iOriginError;

	return ERR_MOTION_SUCCESS;
}


BOOL MCANAxis::IsOnHomeSensor()
{
	UINT16 m_iInputSensor;

	m_amp.GetInputs(m_iInputSensor);

	// input 3번 (3번 bit) 에 Home Sensor 연결되어 있음.
	if(m_iInputSensor & 0x004)
		return TRUE;
	else
		return FALSE;
}

BOOL MCANAxis::IsOnPositiveSensor()
{
	UINT16 m_iInputSensor;

	m_amp.GetInputs(m_iInputSensor);

	// input 2번 (2번 bit) 에 + limit Sensor 연결되어 있음.
	if(m_iInputSensor & 0x002)
		return TRUE;
	else
		return FALSE;
}

BOOL MCANAxis::IsOnNegativeSensor()
{
	// Sensor가 없음.
	return FALSE;
}

int MCANAxis::SetHomeSensorEvent(int iLimit)
{
	//HOME 센서 없음 미사용
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetHomeSensorLevel(BOOL bLevel)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetPositiveSensorEvent(int iLimit)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetPositiveSensorLevel(BOOL bLevel)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetNegativeSensorEvent(int iLimit)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetNegativeSensorLevel(BOOL bLevel)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::checkAxisParameter(SAxis1 srcAx)
{
	CString strLogMsg;

	/** 축 ID 범위 점검 */
	if ((srcAx.iAxisID < DEF_AXIS_NON_ID) || (srcAx.iAxisID > (DEF_MAX_AXIS_NO - 1)))
	{
		//strLogMsg.Format(_T("checkAxisParameter() : Invalid Axis ID Arguement %d "), srcAx.iAxisID);
		//WriteErrorLog(strLogMsg);
		SetErrorLevel(_T("CAN AXIS check Axis Parameter"),3, __FILE__, __LINE__);
		return generateErrorCode(119199);
	}

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::checkAxisState()
{
	int iResult = ERR_MOTION_SUCCESS;
	int iState = -1;

//	iState = GetAxisSource();

/*	if (status == ESTAT_SHORT_CRCT) ///< Amplifier short circuit 
	{
		WriteErrorLog("GetAxisSource() : Getting Motor State failed");
		return generateErrorCode(117034);
	}
	else if (status == ESTAT_AMP_TEMP) ///< Amplifier over temperature	return status;
	{
	}
	else if (status == ESTAT_OVER_VOLT) ///< Amplifier over voltage}
	{
	}
	else if (status == ESTAT_UNDER_VOLT) ///< Amplifier under voltage
	{
	}
	else if (status == ESTAT_MTR_TEMP) ///< Motor over temperatureint 
	{
	}
	else if (status == ESTAT_ENCODER_PWR) ///< Encoder power error
	{
	}
	else if (status == ESTAT_PHASE_ERR) ///< Phasing error
	{
	}
	else if (status == ESTAT_CRNT_LIM) ///< Current limited
	{
	}
	else if (status == ESTAT_VOLT_LIM) ///< Voltage limited
	{
	}
	else if (status == ESTAT_POSLIM) ///< Positive limit switch triggered
	{
	}
	else if (status == ESTAT_NEGLIM) ///< Negative limit switch triggered
	{
	}
	else if (status == ESTAT_DISABLE_INPUT) ///< Enable input pin not set
	{
	}
	else if (status == ESTAT_SOFT_DISABLE) ///< Disabled due to software request
	{
	}
	else if (status == ESTAT_STOP) ///< Try to stop motor (after disable, before brake)
	{
	}
	else if (status == ESTAT_BRAKE) ///< Brake actuated
	{
	}
	else if (status == ESTAT_PWM_DISABLE) ///< PWM outputs disabled
	{
	}
	else if (status == ESTAT_SOFTLIM_POS) ///< Positive software limit reached
	{
	}
	else if (status == ESTAT_SOFTLIM_NEG) ///< Negative software limit reached
	{
	}
	else if (status == ESTAT_TRK_ERR) ///< Tracking error
	{
	}
	else if (status == ESTAT_TRK_WARN) ///< Tracking warning
	{
	}
	else if (status == ESTAT_RESET) ///< Amplifier has been reset
	{
	}
	else if (status == ESTAT_POSWRAP) ///< Encoder position wrapped (rotory) or hit limit (linear).
	{
	}
	else if (status == ESTAT_FAULT) ///< Latching fault in effect
	{
	}
	else if (status == ESTAT_VEL_LIMIT) ///< Velocity is at limit
	{
	}
	else if (status == ESTAT_ACC_LIMIT) ///< Acceleration is at limit
	{
	}
	else if (status == ESTAT_TRK_WIN) ///< Not in tracking window if set
	{
	}
	else if (status == ESTAT_HOME) ///< Home switch is active
	{
	}
	else if (status == ESTAT_MOVING) ///< Trajectory generator active OR not yet settled
	{
	}
	else if (status == ESTAT_VEL_WIN) ///< Velocity error outside of velocity window when set.
	{
	}
	else if (status == ESTAT_PHASE_INIT) ///< Set when using algorithmic phase init mode & phase not initialized.
	{
	}
*/
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::CheckAxisStateForJog(BOOL bDir)
{
	// Limit 센서 감지 이벤트가 설정 될 수 있어야 한다..
	int iResult = ERR_MOTION_SUCCESS;

	return ERR_MOTION_SUCCESS;
}

int	MCANAxis::checkSWLimit(double dPosition)
{
	if (dPosition > m_AxisParameter.dCWSWLimit)
	{
		//WriteErrorLog("checkSWLimit() : OVER_SW_POSITIVE_LIMIT");
		SetErrorLevel(_T("CAN AXIS check SW Limit"),3, __FILE__, __LINE__);
		return generateErrorCode(119170);
	}

	if (dPosition < m_AxisParameter.dCCWSWLimit)
	{
		//WriteErrorLog("checkSWLimit() : OVER_SW_NAGATIVE_LIMIT");
		SetErrorLevel(_T("CAN AXIS check SW Limit"),3, __FILE__, __LINE__);
		return generateErrorCode(119171);
	}

	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetAxisData()
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetAutoCP(int iBoardNo, BOOL* pbAutoSet)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::GetAutoCP(int iBoardNo, BOOL* pbAutoSet)
{
	return ERR_MOTION_SUCCESS;
}

void MCANAxis::SetAxisPosReset()
{
	m_dCurrentAxisPos = 0.0;
	m_dTargetAxisPos = 0.0;
}

int MCANAxis::GetRingID()
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::GetNodeID()
{
	return ERR_MOTION_SUCCESS;
}

BOOL MCANAxis::CheckHomeSensor()
{
	return TRUE;
}

BOOL MCANAxis::CheckPositiveSensor()
{
	return TRUE;
}

BOOL MCANAxis::CheckNegativeSensor()
{
	return TRUE;
}

BOOL MCANAxis::GetAmpEnable()
{
	return TRUE;
}

BOOL MCANAxis::GetAmpFault()
{
	return TRUE;
}

int MCANAxis::checkAxisState(int iAxisID)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::ChangePhase(int Phase)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::NodeClear()
{
	return ERR_MOTION_SUCCESS;
}

double MCANAxis::calculateMotionTime()
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


int MCANAxis::HomingStop()
{
	//MMC2 함수
	return ERR_MOTION_SUCCESS;
}
BOOL MCANAxis::IsOriginComplete()
{
	return FALSE;
}
void MCANAxis::ResetOriginComplete()
{
}
// 현재위치에서 Home Sensor 까지의 거리.
double MCANAxis::DistanceToCurrentToHome()
{
	return 0.0;
}

int MCANAxis::SetHomingSwitchSource(int iSourceAxisNodeID)
{
	return ERR_MOTION_SUCCESS;
}

int MCANAxis::SetMotorScale(double dScale)
{
	return ERR_MOTION_SUCCESS;
}

// Copley VCM Library가 리턴하는 에러 코드를 System Error형태로 만든다
int MCANAxis::makeCanVcmError(int iError)
{
	//  Error define
	switch(iError)
	{
	case CMLERR_Error_OK                     	:	iError = 0;			break; 
	case CMLERR_Error_Unknown                	:	iError = 1;			break; 
	case CMLERR_AmpError_NodeState           	:	iError = 2;			break; 
	case CMLERR_AmpError_pvtSegPos           	:	iError = 3;			break; 
	case CMLERR_AmpError_pvtSegVel           	:	iError = 4;			break; 
	case CMLERR_AmpError_pvtBufferFull       	:	iError = 5;			break; 
	case CMLERR_AmpError_badDeviceID         	:	iError = 6;			break; 
	case CMLERR_AmpError_badHomeParam        	:	iError = 7;			break; 
	case CMLERR_AmpError_badMoveParam        	:	iError = 8;			break; 
	case CMLERR_AmpError_InMotion            	:	iError = 9;			break; 
	case CMLERR_AmpError_GuardError          	:	iError = 10;		break; 
	case CMLERR_AmpError_Fault               	:	iError = 11;		break; 
	case CMLERR_AmpError_ShortCircuit        	:	iError = 12;		break; 
	case CMLERR_AmpError_AmpTemp             	:	iError = 13;		break; 
	case CMLERR_AmpError_MotorTemp           	:	iError = 14;		break; 
	case CMLERR_AmpError_OverVolt            	:	iError = 15;		break; 
	case CMLERR_AmpError_UnderVolt           	:	iError = 16;		break; 
	case CMLERR_AmpError_EncoderPower        	:	iError = 17;		break; 
	case CMLERR_AmpError_PhaseErr            	:	iError = 18;		break; 
	case CMLERR_AmpError_TrackErr            	:	iError = 19;		break; 
	case CMLERR_AmpError_PosLim              	:	iError = 20;		break; 
	case CMLERR_AmpError_NegLim              	:	iError = 21;		break; 
	case CMLERR_AmpError_PosSoftLim          	:	iError = 22;		break; 
	case CMLERR_AmpError_NegSoftLim          	:	iError = 23;		break; 
	case CMLERR_AmpError_TrackWarn           	:	iError = 24;		break; 
	case CMLERR_AmpError_Unknown             	:	iError = 25;		break; 
	case CMLERR_AmpError_Reset               	:	iError = 26;		break; 
	case CMLERR_AmpError_Disabled            	:	iError = 27;		break; 
	case CMLERR_AmpError_QuickStopMode       	:	iError = 28;		break; 
	case CMLERR_AmpError_NoUserUnits         	:	iError = 29;		break; 
	case CMLERR_AmpError_Abort               	:	iError = 30;		break; 
	case CMLERR_AmpError_pvtPosUnavail       	:	iError = 31;		break; 
	case CMLERR_AmpFault_Memory              	:	iError = 32;		break; 
	case CMLERR_AmpFault_ADC                 	:	iError = 33;		break; 
	case CMLERR_AmpFault_ShortCircuit        	:	iError = 34;		break; 
	case CMLERR_AmpFault_AmpTemp             	:	iError = 35;		break; 
	case CMLERR_AmpFault_MotorTemp             	:	iError = 36;		break; 
	case CMLERR_AmpFault_OverVolt              	:	iError = 37;		break; 
	case CMLERR_AmpFault_UnderVolt             	:	iError = 38;		break; 
	case CMLERR_AmpFault_EncoderPower          	:	iError = 39;		break; 
	case CMLERR_AmpFault_PhaseErr              	:	iError = 40;		break; 
	case CMLERR_AmpFault_TrackErr              	:	iError = 41;		break; 
	case CMLERR_AmpFault_Unknown               	:	iError = 42;		break; 
	case CMLERR_CanError_BadPortName           	:	iError = 43;		break; 
	case CMLERR_CanError_NotOpen               	:	iError = 44;		break; 
	case CMLERR_CanError_AlreadyOpen           	:	iError = 45;		break; 
	case CMLERR_CanError_BadParam              	:	iError = 46;		break; 
	case CMLERR_CanError_Driver                	:	iError = 47;		break; 
	case CMLERR_CanError_BadBaud               	:	iError = 48;		break; 
	case CMLERR_CanError_Timeout               	:	iError = 49;		break; 
	case CMLERR_CanError_Overflow              	:	iError = 50;		break; 
	case CMLERR_CanError_BusOff                	:	iError = 51;		break; 
	case CMLERR_CanError_InvalidID             	:	iError = 52;		break; 
	case CMLERR_CanError_Unknown               	:	iError = 53;		break; 
	case CMLERR_CanError_NoDriver              	:	iError = 54;		break; 
	case CMLERR_CanError_Alloc                 	:	iError = 55;		break; 
	case CMLERR_CanOpenError_ThreadStart       	:	iError = 56;		break; 
	case CMLERR_CanOpenError_BadParam          	:	iError = 57;		break; 
	case CMLERR_CanOpenError_SDO_Busy          	:	iError = 58;		break; 
	case CMLERR_CanOpenError_SDO_Timeout       	:	iError = 59;		break; 
	case CMLERR_CanOpenError_SDO_Unknown       	:	iError = 60;		break; 
	case CMLERR_CanOpenError_SDO_BadMuxRcvd    	:	iError = 61;		break; 
	case CMLERR_CanOpenError_SDO_BadMsgRcvd    	:	iError = 62;		break; 
	case CMLERR_CanOpenError_BadNodeID         	:	iError = 63;		break; 
	case CMLERR_CanOpenError_NotInitialized    	:	iError = 64;		break; 
	case CMLERR_CanOpenError_Initialized       	:	iError = 65;		break; 
	case CMLERR_CanOpenError_NotSupported      	:	iError = 66;		break; 
	case CMLERR_CanOpenError_MonitorRunning    	:	iError = 67;		break; 
	case CMLERR_CanOpenError_IllegalFieldCt    	:	iError = 68;		break; 
	case CMLERR_CanOpenError_RcvrNotFound      	:	iError = 69;		break; 
	case CMLERR_CanOpenError_RcvrPresent       	:	iError = 70;		break; 
	case CMLERR_CanOpenError_Closed          	:	iError = 71;		break; 
	case CMLERR_NodeError_GuardTimeout       	:	iError = 72;		break; 
	case CMLERR_SDO_Error_NoAbortCode        	:	iError = 73;		break; 
	case CMLERR_SDO_Error_Togglebit          	:	iError = 74;		break; 
	case CMLERR_SDO_Error_Timeout            	:	iError = 75;		break; 
	case CMLERR_SDO_Error_Bad_scs            	:	iError = 76;		break; 
	case CMLERR_SDO_Error_Block_size         	:	iError = 77;		break; 
	case CMLERR_SDO_Error_Block_seq          	:	iError = 78;		break; 
	case CMLERR_SDO_Error_Block_crc          	:	iError = 79;		break; 
	case CMLERR_SDO_Error_Memory             	:	iError = 80;		break; 
	case CMLERR_SDO_Error_Access             	:	iError = 81;		break; 
	case CMLERR_SDO_Error_Writeonly          	:	iError = 82;		break; 
	case CMLERR_SDO_Error_Readonly           	:	iError = 83;		break; 
	case CMLERR_SDO_Error_Bad_object         	:	iError = 84;		break; 
	case CMLERR_SDO_Error_Pdo_map            	:	iError = 85;		break; 
	case CMLERR_SDO_Error_Pdo_length         	:	iError = 86;		break; 
	case CMLERR_SDO_Error_Bad_param          	:	iError = 87;		break; 
	case CMLERR_SDO_Error_Incompatible       	:	iError = 88;		break; 
	case CMLERR_SDO_Error_Hardware           	:	iError = 89;		break; 
	case CMLERR_SDO_Error_Bad_length         	:	iError = 90;		break; 
	case CMLERR_SDO_Error_Too_long           	:	iError = 91;		break; 
	case CMLERR_SDO_Error_Too_short          	:	iError = 92;		break; 
	case CMLERR_SDO_Error_Subindex           	:	iError = 93;		break; 
	case CMLERR_SDO_Error_Param_range        	:	iError = 94;		break; 
	case CMLERR_SDO_Error_Param_high         	:	iError = 95;		break; 
	case CMLERR_SDO_Error_Param_low          	:	iError = 96;		break; 
	case CMLERR_SDO_Error_Min_max            	:	iError = 97;		break; 
	case CMLERR_SDO_Error_General            	:	iError = 98;		break; 
	case CMLERR_SDO_Error_Transfer           	:	iError = 99;		break; 
	case CMLERR_SDO_Error_Transfer_Local     	:	iError = 100;		break; 
	case CMLERR_SDO_Error_Transfer_State     	:	iError = 101;		break; 
	case CMLERR_SDO_Error_OD_Gen_Fail        	:	iError = 102;		break; 
	case CMLERR_SDO_Error_Unknown            	:	iError = 103;		break; 
	case CMLERR_IOError_BadID                	:	iError = 104;		break; 
	case CMLERR_IOError_BadIOCount           	:	iError = 105;		break; 
	case CMLERR_PDO_Error_MapFull             	:	iError = 106;		break; 
	case CMLERR_PDO_Error_BitOverflow         	:	iError = 107;		break; 
	case CMLERR_PDO_Error_BitSizeError        	:	iError = 108;		break; 
	case CMLERR_LinkError_BadAmpCount         	:	iError = 109;		break; 
	case CMLERR_LinkError_NetworkMismatch     	:	iError = 110;		break; 
	case CMLERR_LinkError_AlreadyInit         	:	iError = 111;		break; 
	case CMLERR_LinkError_AmpAlreadyLinked    	:	iError = 112;		break; 
	case CMLERR_LinkError_AxisCount           	:	iError = 113;		break; 
	case CMLERR_LinkError_AmpTrjOverflow      	:	iError = 114;		break; 
	case CMLERR_LinkError_AmpTrjInUse         	:	iError = 115;		break; 
	case CMLERR_LinkError_AmpTrjNotRunning    	:	iError = 116;		break; 
	case CMLERR_LinkError_NoActiveTrj         	:	iError = 117;		break; 
	case CMLERR_LinkError_BadMoveLimit        	:	iError = 118;		break; 
	case CMLERR_LinkError_UnknownAmpErr       	:	iError = 119;		break; 
	case CMLERR_LinkError_StartMoveTO         	:	iError = 120;		break; 
	case CMLERR_LinkError_NotSupported        	:	iError = 121;		break; 
	case CMLERR_ThreadError_Start             	:	iError = 122;		break; 
	case CMLERR_ThreadError_Running           	:	iError = 123;		break; 
	case CMLERR_ThreadError_Timeout           	:	iError = 124;		break; 
	case CMLERR_ThreadError_General           	:	iError = 125;		break; 
	case CMLERR_ThreadError_BadParam          	:	iError = 126;		break; 
	case CMLERR_ThreadError_Alloc             	:	iError = 127;		break; 
	case CMLERR_EventError_AlreadyOwned       	:	iError = 128;		break; 
	case CMLERR_EventError_NotMapped          	:	iError = 129;		break; 
	case CMLERR_AmpFileError_format           	:	iError = 130;		break; 
	case CMLERR_AmpFileError_tooOld           	:	iError = 131;		break; 
	case CMLERR_AmpFileError_noFileAccess     	:	iError = 132;		break; 
	case CMLERR_AmpFileError_fileOpen         	:	iError = 133;		break; 
	case CMLERR_AmpFileError_range            	:	iError = 134;		break; 
	case CMLERR_ScurveError_BadParam          	:	iError = 135;		break; 
	case CMLERR_ScurveError_NoCalc            	:	iError = 136;		break; 
	case CMLERR_ScurveError_InUse             	:	iError = 137;		break; 
	case CMLERR_ScurveError_NotInUse          	:	iError = 138;		break; 
	case CMLERR_CanError_Permission           	:	iError = 139;		break; 
	case CMLERR_FirmwareError_open            	:	iError = 140;		break; 
	case CMLERR_FirmwareError_read            	:	iError = 141;		break; 
	case CMLERR_FirmwareError_format          	:	iError = 142;		break; 
	case CMLERR_FirmwareError_crc             	:	iError = 143;		break; 
	case CMLERR_FirmwareError_alloc           	:	iError = 144;		break; 
	case CMLERR_PathError_BadVel              	:	iError = 145;		break; 
	case CMLERR_PathError_BadAcc              	:	iError = 146;		break; 
	case CMLERR_PathError_VelNotInit          	:	iError = 147;		break; 
	case CMLERR_PathError_AccNotInit          	:	iError = 148;		break; 
	case CMLERR_PathError_BadPoint            	:	iError = 149;		break; 
	case CMLERR_PathError_Alloc               	:	iError = 150;		break; 
	case CMLERR_PathError_BadLength           	:	iError = 151;		break; 
	case CMLERR_PathError_Empty               	:	iError = 152;		break; 
	case CMLERR_AmpFileError_axis             	:	iError = 153;		break; 
	case CMLERR_AmpFault_I2TLimit             	:	iError = 154;		break; 
	case CMLERR_AmpError_VelWin               	:	iError = 155;		break; 
	case CMLERR_CopleyNodeError_SerialMsgLen  	:	iError = 156;		break; 
	case CMLERR_CopleyNodeError_SerialError   	:	iError = 157;		break; 
	case CMLERR_TrjError_NoneAvailable        	:	iError = 158;		break; 
		
	default										:	iError = 999;		break; //			// Error Code 없음 
	}

	// XXX                             XXX         XXX
	// EObjectType(OBJ_HL_CAN_MOTION)  CanVcmError   인스턴스번호
	return (119000+iError)*1000+m_iInstanceNo;
}
