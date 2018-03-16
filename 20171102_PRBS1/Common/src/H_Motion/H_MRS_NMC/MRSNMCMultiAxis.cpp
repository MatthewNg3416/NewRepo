// MRSNMCMultiAxis.cpp: implementation of the MRSNMCMultiAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MRSNMCMultiAxis.h"
#include <math.h>

#include "common.h"
#include "NMC_Motion.h"
#include "MRSNMCAxis.h"
#include "DefMotionError.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


MRSNMCMultiAxis::MRSNMCMultiAxis(SCommonAttribute commonData, IAxis** pAxis, int iAxisNum, int iGroupID)
: ICommon(commonData)
{
	ASSERT(pAxis != NULL);
//	ASSERT(iAxisNum >=0 && iAxisNum<=DEF_MAX_SERCOS_AXIS_NO);
	
	m_iAxisNum			= iAxisNum;
	m_plnkAxis			= new MRSNMC*[m_iAxisNum];
	m_plnkRingID		= new INT32S[m_iAxisNum];
	m_plnkNodeID		= new INT32S[m_iAxisNum];

	m_pMoveVelocity		= new double[m_iAxisNum];
	m_pMoveSlowVelocity	= new double[m_iAxisNum];
	m_pAccelerate		= new int[m_iAxisNum];
	m_pScale			= new double[m_iAxisNum];
	m_pAxisID			= new int[m_iAxisNum];
	m_iGroupID			= iGroupID;
	
	int i = 0;
	for (int i = 0; i < m_iAxisNum; i++)
	{
		m_plnkAxis[i]			= (MRSNMC*)pAxis[i];
		m_plnkRingID[i]			= m_plnkAxis[i]->GetRingID();
		m_plnkNodeID[i]			= m_plnkAxis[i]->GetNodeID();

		m_pMoveVelocity[i]		= m_plnkAxis[i]->GetMovingVelocity();
		m_pMoveSlowVelocity[i]	= m_plnkAxis[i]->GetSlowMovingVelocity();
		m_pAccelerate[i]		= m_plnkAxis[i]->GetMovingAccelerate();
		m_pScale[i]				= m_plnkAxis[i]->GetScale();
		m_pAxisID[i]			= m_plnkAxis[i]->GetAxisID();
	}

	// Limit, Statble Time은 각 Axis 중 가장 큰 값으로 설정.
	m_dStabilityTime	= 0.0;
	m_dLimitTime		= 0.0;
	
	for (int i = 0; i < m_iAxisNum; i++)
	{
		if (m_dStabilityTime < m_plnkAxis[i]->GetMoveAfterTime())
			m_dStabilityTime = m_plnkAxis[i]->GetMoveAfterTime();

		if (m_dLimitTime < m_plnkAxis[i]->GetMoveTime())
			m_dLimitTime = m_plnkAxis[i]->GetMoveTime();
	}
}

/**
 * Destructor : Delete inner opbject
 *				내부 object를 소멸한다.
 * @stereotype destructor
 */
MRSNMCMultiAxis::~MRSNMCMultiAxis()
{
	delete []m_plnkAxis;
	delete []m_plnkRingID;
	delete []m_plnkNodeID;

	delete []m_pMoveVelocity;
	delete []m_pMoveSlowVelocity;
	delete []m_pAccelerate;
	delete []m_pScale;
	delete []m_pAxisID;
}

ErrorInfo MRSNMCMultiAxis::StartMove(double *dPosition, double *dVelocity,
							double *dAccelerate, double *dDecelerate, int bIsTrapMove)
{
	int i = 0;
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	MC_STATUS ms;
	UINT32 state = 0;

	double* dPos	= new double[m_iAxisNum];
	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iPos	= new int[m_iAxisNum];
	int*	iJerk	= new int[m_iAxisNum];

	try
	{
			/** 이동 전 원점복귀 여부 확인, 축 상태 점검 */
		for (int i = 0; i < m_iAxisNum; i++)
		{
			//SJ_YYK 160106 Add....
			m_pMoveVelocity[i] = m_plnkAxis[i]->GetMovingVelocity();
			m_pMoveSlowVelocity[i]	= m_plnkAxis[i]->GetSlowMovingVelocity();
			m_pAccelerate[i]		= m_plnkAxis[i]->GetMovingAccelerate();
				//___________________________________*/

			/** 이동 전 원점복귀 여부 확인 */
			if (!m_plnkAxis[i]->IsOriginReturn())
				throw generateErrorCode(107906);

			/** 축 상태 점검 */
			errorInfo = m_plnkAxis[i]->checkAxisState(m_plnkNodeID[i]);
			if (errorInfo.IsError())
				throw errorInfo;

			ms = MC_ReadAxisStatus(m_plnkRingID[i], m_plnkNodeID[i], &state);
			if (ms != MC_OK)
				throw MakeRS_NMC_Error(ms);

			if(state & mcStandStill)
			{
				;
			}
			//170720 kmh test 
			else if(state & mcErrorStop || state & mcDisabled)
				throw generateErrorCode(107916);

			/** SW Limit Check */
			if (0 == i) // 기준축만 S/W Limit 체크.
			{
				errorInfo = m_plnkAxis[0]->checkSWLimit(dPosition[0]);
				if (errorInfo.IsError())
					throw errorInfo;
			}

			// Profile 생성.
			dPos[i] = m_pScale[i] * dPosition[i];
			iPos[i]	= (int)dPos[i];

			/** 이동 속도 처리 */
			if (dVelocity == NULL || dVelocity[i] == 0.0)
			{
				if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
					dVel[i] = m_pScale[i] * m_pMoveVelocity[i];
				else
					dVel[i] = m_pScale[i] * m_pMoveSlowVelocity[i];
			}
			else
				dVel[i] =  m_pScale[i] * dVelocity[i];
			
			/** 이동 가속도 처리 */
			if (dVelocity == NULL || dVelocity[i] == 0.0)
			{
				if (dAccelerate == NULL || dAccelerate[i] == 0.0)
				{
					if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
						dAccel[i] = (double)((m_pScale[i] * m_pMoveVelocity[i])/((double)m_pAccelerate[i]/1000));
					else
						dAccel[i] = (double)((m_pScale[i] * m_pMoveSlowVelocity[i])/((double)m_pAccelerate[i]/1000));
				}
				else
				{
					if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
						dAccel[i] = (double)((m_pScale[i] * m_pMoveVelocity[i])/((double)dAccelerate[i]/1000));
					else
						dAccel[i] = (double)((m_pScale[i] * m_pMoveSlowVelocity[i])/((double)dAccelerate[i]/1000));
				}
			}
			else
			{
				if (dAccelerate == NULL || dAccelerate[i] == 0.0)
					dAccel[i] = (double)((m_pScale[i] * dVelocity[i])/((double)m_pAccelerate[i]/1000));
				else
					dAccel[i] = (double)((m_pScale[i] * dVelocity[i])/((double)dAccelerate[i]/1000));
			}
			
			/** 이동 감속도 처리 */
			if (dVelocity == NULL || dVelocity[i] == 0.0)
			{
				if (dDecelerate == NULL || dDecelerate[i] == 0.0)
				{
					if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
						dDecel[i] = (double)((m_pScale[i] * m_pMoveVelocity[i])/((double)m_pAccelerate[i]/1000));
					else
						dDecel[i] = (double)((m_pScale[i] * m_pMoveSlowVelocity[i])/((double)m_pAccelerate[i]/1000));
				}
				else
				{
					if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
						dDecel[i] = (double)((m_pScale[i] * m_pMoveVelocity[i])/((double)dDecelerate[i]/1000));
					else
						dDecel[i] = (double)((m_pScale[i] * m_pMoveSlowVelocity[i])/((double)dDecelerate[i]/1000));
				}
			}
			else
			{
				if (dDecelerate == NULL || dDecelerate[i] == 0.0)
					dDecel[i] = (double)((m_pScale[i] * dVelocity[i])/((double)m_pAccelerate[i]/1000));
				else
					dDecel[i] = (double)((m_pScale[i] * dVelocity[i])/((double)dDecelerate[i]/1000));
			}
			
			if ((bIsTrapMove == DEF_SMOVE_DISTANCE) || (bIsTrapMove ==DEF_TSMOVE_DISTANCE))
			{
				/* 20090203 jdy : jerk 에 대한 수정 내용 아직 반영 안됨. RAK 에서 Jerk 설정을 위한 API 추가 제공하기로 했음.
							  20090119 정용국K 와 통화 완료. - RAK 협의 후 API 만들어서 연락 준다고 함.
				*/
				if (dAccel[i] < dDecel[i])
				{
					iJerk[i] = INT32U(dAccel[i] * 20);
				}
				else
				{
					iJerk[i] = INT32U(dDecel[i] * 20);
				}
			}
			else
			{
				iJerk[i] = INT32U(0);	// Curve 형태를 결정한다. (사다리꼴)          
			}         
		}
	}
	catch(ErrorInfo thr_errInfo)
	{	
		delete[] dPos;
		delete[] dVel;
		delete[] dAccel;
		delete[] dDecel;
		delete[] iPos;
		delete[] iJerk;
		return thr_errInfo;
	}


	try
	{
		ms = MC_UngroupAllAxes(m_plnkRingID[0], m_iGroupID);
		if(ms != MC_OK)
		{
			MC_UngroupAllAxes(m_plnkRingID[0], m_iGroupID);
			MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
			throw MakeRS_NMC_Error(ms);
		}
		Sleep(10);
		
		for (int i = 0; i < m_iAxisNum; i++)
		{	
			ms = MC_AddAxisToGroup(m_plnkRingID[i], m_plnkNodeID[i], m_iGroupID, i);
			if(ms != MC_OK)
			{
				MC_UngroupAllAxes(m_plnkRingID[i], m_iGroupID);
				MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
				throw MakeRS_NMC_Error(ms);
			}
			
			Sleep(10);
		}
		//170713_KDH RS 측 문의 필요, 명령 한번에 안먹음
		for (int i = 0; i < 20; i++)
		{
			ms = MC_GroupEnable(m_plnkRingID[0], m_iGroupID);
			Sleep(10);
			if(ms == MC_OK)
				break;	
		}
		if(ms != MC_OK)
		{
			MC_UngroupAllAxes(m_plnkRingID[0], m_iGroupID);
			MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
			throw MakeRS_NMC_Error(ms);
		}
		//________________-
		
		ms = MC_MoveLinearAbsolute(m_plnkRingID[0], m_iGroupID, m_iAxisNum, dPos, motionRoundValue(dVel[0]), motionRoundValue(dAccel[0]), motionRoundValue(dDecel[0]), motionRoundValue(iJerk[0]), mcACS, mcAborting, mcTMNone, NULL, NULL);
		if(ms != MC_OK)
			throw MakeRS_NMC_Error(ms);
		
		Sleep(10);
	}
	catch(ErrorInfo thr_errInfo)
	{
		MC_UngroupAllAxes(m_plnkRingID[0], m_iGroupID);
		MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
		delete[] dPos;
		delete[] dVel;
		delete[] dAccel;
		delete[] dDecel;
		delete[] iPos;
		delete[] iJerk;	
		return thr_errInfo;
	}
	delete[] dPos;
	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iPos;
	delete[] iJerk;

	return NO_ERROR_SUCESS;
}

ErrorInfo MRSNMCMultiAxis::Move(double * dPosition, double * dVelocity, double* dAccelerate, 
						   double* dDecelerate, int bIsTrapMove,  int iMoveType)
{
	ErrorInfo errorInfo = StartMove(dPosition, dVelocity, dAccelerate, dDecelerate, bIsTrapMove);
	if (errorInfo.IsError())
		return errorInfo;
	
	errorInfo = Wait4Done(FALSE, iMoveType);
	if (errorInfo.IsError())
	{
		//170926 jsH.S
		int iErrorCode = errorInfo.GetErrorCode();
		if( iErrorCode == 106125 ||
			iErrorCode == 106126 ||
			iErrorCode == 106127
			)
		{
			errorInfo = StartMove(dPosition, dVelocity, dAccelerate, dDecelerate, bIsTrapMove);
			if (errorInfo.IsError())
				return errorInfo;
			
			errorInfo = Wait4Done(FALSE, iMoveType);
			if (errorInfo.IsError())
				return errorInfo;
		}
		else
			return errorInfo;
	}
	
	return NO_ERROR_SUCESS;
}


ErrorInfo MRSNMCMultiAxis::StartRMove(double *dPosition, double *dVelocity,
							double *dAccelerate, double *dDecelerate, int bIsTrapMove)
{
	int i = 0;
	ErrorInfo errorInfo;

	double* dCurrPos	= new double[m_iAxisNum];
	double* dTargetPos = new double[m_iAxisNum];

	for (int i = 0; i < m_iAxisNum; i++)
	{
		dCurrPos[i] = m_plnkAxis[i]->GetCurrentPosition();
	}
	for (int i = 0; i < m_iAxisNum; i++)
	{
		dTargetPos[i] = dCurrPos[i] + dPosition[i];
	}

	errorInfo = StartMove(dTargetPos, dVelocity, dAccelerate, dDecelerate, bIsTrapMove);

	delete[] dCurrPos;
	delete[] dTargetPos;
	
	return errorInfo;
}

ErrorInfo MRSNMCMultiAxis::RMove(double * dPosition, double * dVelocity, double* dAccelerate,
						   double* dDecelerate, int bIsTrapMove,  int iMoveType)
{
	ErrorInfo errorInfo = StartRMove(dPosition, dVelocity, dAccelerate, dDecelerate, bIsTrapMove);
	if (errorInfo.IsError())
		return errorInfo;

	errorInfo = Wait4Done(FALSE, iMoveType);
	if (errorInfo.IsError())
		return errorInfo;
	
	return NO_ERROR_SUCESS;
}


/**
* 축 이동 후 완료를 확인한다. (Polling에의한 확인)  
* "DEF_DONE_EVENT_TYPE"이 Define되면 Event방식에의한 Done을 사용한다.
*
* @param   bSkipStableTime  : (OPTION=FALSE) 안정화시간을 대기하지 않는다.
* @param   eDoneType		:
*                             - DEF_IN_POSITION_FINE_DONE : InPosition 신호 (축의 정지상태) 까지 기다린다.
*							  - DEF_IN_POSITION_COARSE_DONE : CoarseDone 신호 (사용자가 정한 중간의 임의지점)까지 기다리고 Return한다.
*							  - DEF_AT_TARGET_DONE : AtTarget 신호 (Command명령이 끝난시점) 까지 기다리고 Return한다.
* @param	bUseInterlock    : (OPTION=FALSE) Interlock에 의한 동작 정지 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
* @return	Error Code		 : 0 = SUCCESS
*							  그 외 = 타 함수 Return Error
*/
ErrorInfo MRSNMCMultiAxis::Wait4Done(BOOL bSkipStableTime, int iMoveType, BOOL bUseInterlock)
{
#ifdef DEF_DONE_EVENT_TYPE
	return NO_ERROR_SUCESS;
#endif	
	
	int i = 0;
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	MTickTimer m_Timer;
	m_Timer.StartTimer();
	MC_STATUS ms;
	CString strLogMsg;

	UINT32 state = 0;


	while(TRUE)
	{
		ms = MC_GroupReadStatus(m_plnkRingID[0], m_iGroupID, &state);
		if(ms != MC_OK)
		{
			MC_GroupDisable(m_plnkRingID[0], m_iGroupID);			
			return MakeRS_NMC_Error(ms);
		}
		
		//170812 JSH.s
// 		if(state & mcStandStill)
// 		{
// 			break;
// 		} 
		if(state & GroupErrorStop)
		{
			Stop();
			MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
			return generateErrorCode(106126);
		} 
		if(state & GroupDisabled)
		{
			Stop();
			MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
			return generateErrorCode(106127);
		} 
		if(state & InPosition)
		{
			break;
		} 
		//170812 JSH.e

		if(m_Timer.MoreThan(m_dLimitTime) == TRUE)
		{
			Stop();
			MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
			return generateErrorCode(106125);
		}

		Sleep(5);
	}

	ms = MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
	if(ms != MC_OK)
	{
		return MakeRS_NMC_Error(ms);
	}
	Sleep(10);

	return NO_ERROR_SUCESS;
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
ErrorInfo MRSNMCMultiAxis::JogMoveSlow (BOOL bDir, double dSlow)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	double* dPos	= new double[m_iAxisNum];
	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* GAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iPos	= new int[m_iAxisNum];
	int*	iJerk   = new int[m_iAxisNum];
	int		iDir;
	CString strLogMsg;
	MC_STATUS ms;

	JogStop();
	ms = MC_UngroupAllAxes(m_plnkRingID[0], m_iGroupID);
	if(ms != MC_OK)
	{
		MC_UngroupAllAxes(m_plnkRingID[0], m_iGroupID);
		MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
		return MakeRS_NMC_Error(ms);
	}
	Sleep(10);

	try
	{
		/** (+) 방향 */
		if (bDir == TRUE)
			iDir = 1;
		/** (-) 방향 */
		else
			iDir = -1;

		for (int i = 0; i < m_iAxisNum; i++)
		{	
			// 이동 속도 처리.
			if (0.0 == dSlow)
				dVel[i] = m_plnkAxis[i]->GetJogSlow();
			else 
				dVel[i] = dSlow;
			
			// 이동 가감속 처리.
			dAccel[i] = dVel[i] / ((double)m_pAccelerate[i]/1000);
			GAccel[i] = dVel[i] * m_pScale[i] / ((double)m_pAccelerate[i]/1000);
			dDecel[i] = dAccel[i];
			
			// S-Curve 형태 설정.
			iJerk[i] = 1000;			// Curve 형태를 결정한다. (사다리꼴)    		
		}
	
		ms = MC_GearIn(m_plnkRingID[0], m_plnkNodeID[0], m_plnkNodeID[1], 1, 1, mcSetValue, motionRoundValue(GAccel[0]), motionRoundValue(GAccel[0]), motionRoundValue(iJerk[0]), mcAborting);
		if(ms != MC_OK)
			throw MakeRS_NMC_Error(ms);

		Sleep(200);
		
		errorInfo = m_plnkAxis[0]->VMove(dVel[0], (int)dAccel[0], bDir);
		if(errorInfo.IsError())
		{
			MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
			throw errorInfo;
		}
	}
	catch(ErrorInfo thr_errInfo)
	{
		delete[] dPos;
		delete[] dVel;
		delete[] dAccel;
		delete[] dDecel;
		delete[] iPos;
		delete[] iJerk;
		
		return thr_errInfo;
	}
	delete[] dPos;
	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iPos;
	delete[] iJerk;

	return NO_ERROR_SUCESS;
	
}

/**
 * Jog Fast에 의한 이동한다.
 * 
 * @param   bDir             : 이동할 방향, TRUE:(+), FALSE:(-)
 * @param   dFast        : (OPTION = 0.0) 이동할 속도, 0.0 = 지정된 속도 사용
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRSNMCMultiAxis::JogMoveFast (BOOL bDir, double dFast)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	double* dPos	= new double[m_iAxisNum];
	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* GAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iPos	= new int[m_iAxisNum];
	int*	iJerk   = new int[m_iAxisNum];
	int		iDir;

	CString strLogMsg;
	MC_STATUS ms;

	JogStop();

	/** (+) 방향 */
	if (bDir == TRUE)
		iDir = 1;
	/** (-) 방향 */
	else
		iDir = -1;

	try
	{
		for (int i = 0; i < m_iAxisNum; i++)
		{	
			// 이동 속도 처리.
			if (0.0 == dFast)
				dVel[i] = m_plnkAxis[i]->GetJogFast();
			else 
				dVel[i] = dFast;
			
			// 이동 가감속 처리.
			dAccel[i]	= dVel[i] / ((double)m_pAccelerate[i]/1000);
			GAccel[i]	= dVel[i] * m_pScale[i] / ((double)m_pAccelerate[i]/1000);
			dDecel[i]	= dAccel[i];
			
			// S-Curve 형태 설정.
			iJerk[i]	= INT32U(0);			// Curve 형태를 결정한다. (사다리꼴)    
		}

		
		ms = MC_GearIn(m_plnkRingID[0], m_plnkNodeID[0], m_plnkNodeID[1], 1, 1, mcSetValue, motionRoundValue(GAccel[0]), motionRoundValue(GAccel[0]), motionRoundValue(iJerk[0]), mcAborting);
		if(ms != MC_OK)
			throw MakeRS_NMC_Error(ms);

		Sleep(200);
		
		errorInfo = m_plnkAxis[0]->VMove(dVel[0], (int)dAccel[0], bDir);
		if(errorInfo.IsError())
		{
			MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
			throw errorInfo;
		}
	}
	catch(ErrorInfo thr_errInfo)
	{
		delete[] dPos;
		delete[] dVel;
		delete[] dAccel;
		delete[] dDecel;
		delete[] iPos;
		delete[] iJerk;
	
		return thr_errInfo;
	}
	delete[] dPos;
	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iPos;
	delete[] iJerk;

	return NO_ERROR_SUCESS;
}

/**
 * Jog 이동을 정지 시킨다.
 * 	 
 * @return	Error Code		 : 0 = SUCCESS
 *							  그 외 =  Error Code
 */
ErrorInfo MRSNMCMultiAxis::JogStop()
{
	int i = 0;
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	CString strLogMsg;

	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* GAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iJerk   = new int[m_iAxisNum];

	for (int i = 0; i < m_iAxisNum; i++)
	{	
		dVel[i] = m_pScale[i] * m_plnkAxis[i]->GetJogFast();
		// 이동 가감속 처리.
		dAccel[i]	= dVel[i] / ((double)m_pAccelerate[i]/1000);
		GAccel[i] = dVel[i] * m_pScale[i] / ((double)m_pAccelerate[i]/1000);
		dDecel[i]	= dAccel[i];
		
		// S-Curve 형태 설정.
		iJerk[i]	= INT32U(0);			// Curve 형태를 결정한다. (사다리꼴) 
		
		errorInfo = m_plnkAxis[i]->VStop();
		if(errorInfo.IsError())
		{
			delete[] dVel;
			delete[] dAccel;
			delete[] dDecel;
			delete[] iJerk;
			return errorInfo;
		}
	}
	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iJerk;

	return NO_ERROR_SUCESS;
	/*
	ms = MC_GearIn(m_plnkRingID[0], m_plnkNodeID[0], m_plnkNodeID[1], 1, 1, mcSetValue, motionRoundValue(GAccel[0]), motionRoundValue(GAccel[0]), motionRoundValue(iJerk[0]), mcAborting);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("EnableSyncMotion(): MC_GearIn(return the error message) Fail[SlaveID : %d] - Error# is %d"), m_plnkNodeID[1], MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}

	Sleep(200);
	
	iResult = m_plnkAxis[0]->VStop();
	if(iResult)
	{
		MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
		return iResult;
	}

	Sleep(100);

	iResult = m_plnkAxis[0]->Wait4Done();
	if(iResult)
	{
		MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
		return iResult;
	}
	
	ms = MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("DisableSyncMotion(): MC_GearOut(return the error message) Fail[SlaveID : %d] - Error# is %d"), m_plnkNodeID[1], MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}

	iResult = m_plnkAxis[1]->VStop();
	if(iResult)
	{
		MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
		return iResult;
	}

	iResult = m_plnkAxis[1]->Wait4Done();
	if(iResult)
	{
		MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
		return iResult;
	}
	
	Sleep(100);

//	if (!IsAxisDone()) return NO_ERROR_SUCESS;

	return iResult;
	*/
}

ErrorInfo MRSNMCMultiAxis::Stop()
{
#ifdef SIMULATION
	return NO_ERROR_SUCESS;
#endif
	
	int i = 0;
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	CString strLogMsg;
	MC_STATUS ms;

	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iJerk   = new int[m_iAxisNum];

	for (int i = 0; i < m_iAxisNum; i++)
	{
		dVel[i] = m_pScale[i] * m_plnkAxis[i]->GetJogFast();
		// 이동 가감속 처리.
		dAccel[i]	= dVel[i] / ((double)m_pAccelerate[i]/1000);
		dDecel[i]	= dAccel[i];
		
		// S-Curve 형태 설정.
		iJerk[i]	= INT32U(0);			// Curve 형태를 결정한다. (사다리꼴)    		
	}

	ms = MC_GroupHalt(m_plnkRingID[0], m_iGroupID, dDecel[0], iJerk[0], mcAborting);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	Sleep(100);

//	if (!IsAxisDone()) return NO_ERROR_SUCESS;

	return errorInfo;
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
ErrorInfo MRSNMCMultiAxis::MoveSplineLine2(int iMaxPoint, double *pdPosition, double *pdVelocity, int *piAccelerate,
								 BOOL bAutoFlag, BOOL bWaitOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iPos	= new int[m_iAxisNum];
	int*	iJerk   = new int[m_iAxisNum];


	CString strLogMsg;
	MC_STATUS ms;

	iJerk[0] = INT32U(0);	// Curve 형태를 결정한다. (사다리꼴)   

	ms = MC_GearIn(m_plnkRingID[0], m_plnkNodeID[0], m_plnkNodeID[1], 1, 1, mcSetValue, motionRoundValue(pdVelocity[0]), motionRoundValue(piAccelerate[0]), motionRoundValue(iJerk[0]), mcAborting);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	errorInfo = m_plnkAxis[0]->MoveSplineLine(iMaxPoint, pdPosition, pdVelocity, piAccelerate, bAutoFlag, bWaitOpt);
	if(errorInfo.IsError())
	{
		MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
		return errorInfo;
	}

	ms = MC_GearOut(m_plnkRingID[0], m_plnkNodeID[1]);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iPos;
	delete[] iJerk;
		
	return NO_ERROR_SUCESS;
}

// NMC Library가 리턴하는 에러 코드를 System Error형태로 만든다
ErrorInfo MRSNMCMultiAxis::MakeRS_NMC_Error(MC_STATUS mcStatus)
{
	int iReturnErrorCode;

	char chGetErrorMessage[128];
	memset(chGetErrorMessage, 0, sizeof(chGetErrorMessage));

	switch(mcStatus)
	{
	case MC_ERROR_HW_NOT_INSTALLED:
		iReturnErrorCode = ERR_RS_NMC_MC_ERROR_HW_NOT_INSTALLED;
		break;
	case MC_ERROR_DD_SEND_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_ERROR_DD_SEND_ERROR;
		break;
	case MC_ERROR_DD_READ_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_ERROR_DD_READ_ERROR;
		break;
	case MC_DD_ERROR_SEND:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_ERROR_SEND;
		break;
	case MC_DD_ERROR_RECV:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_ERROR_RECV;
		break;
	case MC_DD_OPEN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_OPEN_FAIL;
		break;
	case MC_DD_NOT_OPENED:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_NOT_OPENED;
		break;
	case MC_DD_CONN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_CONN_FAIL;
		break;
	case MC_DD_CLIENT_START_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_CLIENT_START_FAIL;
		break;
	case MC_DD_OK:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_OK;
		break;
	case MC_CN_NOT_CONNECTED:
		iReturnErrorCode = ERR_RS_NMC_MC_CN_NOT_CONNECTED;
		break;
	case MC_CN_CONNECTED:
		iReturnErrorCode = ERR_RS_NMC_MC_CN_CONNECTED;
		break;
	case MC_CN_CONNERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_CN_CONNERROR;
		break;
	case MC_INVALID_SYSTEM_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_SYSTEM_STATE;
		break;
	case MC_UNSUPPORT_CMD:
		iReturnErrorCode = ERR_RS_NMC_MC_UNSUPPORT_CMD;
		break;
	case MC_INVALID_AXIS_STATE_NOT_HOMING_MODE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_NOT_HOMING_MODE;
		break;
	case MC_INVALID_PARAM:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM;
		break;
	case MC_INVALID_PARAM_1:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_1;
		break;
	case MC_INVALID_PARAM_2:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_2;
		break;
	case MC_INVALID_PARAM_3:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_3;
		break;
	case MC_INVALID_PARAM_4:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_4;
		break;
	case MC_INVALID_PARAM_5:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_5;
		break;
	case MC_INVALID_PARAM_6:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_6;
		break;
	case MC_INVALID_PARAM_7:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_7;
		break;
	case MC_INVALID_PARAM_8:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_8;
		break;
	case MC_INVALID_PARAM_9:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_9;
		break;
	case MC_INVALID_PARAM_10:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_10;
		break;
	case MC_INVALID_SIZE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_SIZE;
		break;
	case MC_INVALID_AXIS_NUM:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_NUM;
		break;
	case MC_NOT_ENOUGH_RESOURCE:
		iReturnErrorCode = ERR_RS_NMC_MC_NOT_ENOUGH_RESOURCE;
		break;
	case MC_LIMIT_ERROR_PARAM:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM;
		break;
	case MC_LIMIT_ERROR_PARAM_1:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_1;
		break;
	case MC_LIMIT_ERROR_PARAM_2:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_2;
		break;
	case MC_LIMIT_ERROR_PARAM_3:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_3;
		break;
	case MC_LIMIT_ERROR_PARAM_4:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_4;
		break;
	case MC_LIMIT_ERROR_PARAM_5:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_5;
		break;
	case MC_LIMIT_ERROR_PARAM_6:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_6;
		break;
	case MC_LIMIT_ERROR_PARAM_7:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_7;
		break;
	case MC_LIMIT_ERROR_PARAM_8:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_8;
		break;
	case MC_LIMIT_ERROR_PARAM_9:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_9;
		break;
	case MC_LIMIT_ERROR_PARAM_10:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_10;
		break;
	case MC_INVALID_DEVICE_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_DEVICE_STATE;
		break;
	case MC_INVALID_DEVICE_STATE_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_DEVICE_STATE_ERROR;
		break;
	case MC_INVALID_AXIS_STATE_DISABLED:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_DISABLED;
		break;
	case MC_INVALID_AXIS_STATE_STANDSTILL:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_STANDSTILL;
		break;
	case MC_INVALID_AXIS_STATE_DISCRETE_MOTION:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_DISCRETE_MOTION;
		break;
	case MC_INVALID_AXIS_STATE_CONTINUOUS_MOTION:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_CONTINUOUS_MOTION;
		break;
	case MC_INVALID_AXIS_STATE_SYNC_MOTION:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_SYNC_MOTION;
		break;
	case MC_INVALID_AXIS_STATE_HOMING:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_HOMING;
		break;
	case MC_INVALID_AXIS_STATE_STOPPING:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_STOPPING;
		break;
	case MC_INVALID_AXIS_STATE_ERRORSTOP:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_ERRORSTOP;
		break;
	case MC_INVALID_AXIS_STATE_MODE_CHANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_MODE_CHANGE;
		break;
	case MC_INVALID_AXIS_CONFIG:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG;
		break;
	case MC_INVALID_AXIS_CONFIG_POS_LIMIT_SWITCH:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_POS_LIMIT_SWITCH;
		break;
	case MC_INVALID_AXIS_CONFIG_NEG_LIMIT_SWITCH:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_NEG_LIMIT_SWITCH;
		break;
	case MC_INVALID_AXIS_CONFIG_HOME_SWITCH:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOME_SWITCH;
		break;
	case MC_INVALID_AXIS_CONFIG_Z_PHASE_INPUT:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_Z_PHASE_INPUT;
		break;
	case MC_INVALID_AXIS_CONFIG_HOME_SENSOR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOME_SENSOR;
		break;
	case MC_INVALID_AXIS_CONFIG_MARK_PULSE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_MARK_PULSE;
		break;
	case MC_INVALID_AXIS_CONFIG_HOME_TYPE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOME_TYPE;
		break;
	case MC_INVALID_AXIS_CONFIG_HOME_FLAG_HANDLE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOME_FLAG_HANDLE;
		break;
	case MC_INVALID_AXIS_CONFIG_HOMING_MODE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOMING_MODE;
		break;
	case MC_GEARING_RULE_VIOLATION:
		iReturnErrorCode = ERR_RS_NMC_MC_GEARING_RULE_VIOLATION;
		break;
	case MC_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_POSITION_OVER;
		break;
	case MC_POS_HW_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_POS_HW_LIMIT_POSITION_OVER;
		break;
	case MC_NEG_HW_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_NEG_HW_LIMIT_POSITION_OVER;
		break;
	case MC_POS_SW_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_POS_SW_LIMIT_POSITION_OVER;
		break;
	case MC_NEG_SW_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_NEG_SW_LIMIT_POSITION_OVER;
		break;
	case MC_INVALID_AXES_GROUP_NUM:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXES_GROUP_NUM;
		break;
	case MC_AXIS_ALREADY_ASSIGNED:
		iReturnErrorCode = ERR_RS_NMC_MC_AXIS_ALREADY_ASSIGNED;
		break;
	case MC_IDENT_ALREADY_ASSIGNED:
		iReturnErrorCode = ERR_RS_NMC_MC_IDENT_ALREADY_ASSIGNED;
		break;
	case MC_AXES_GROUP_INVALID_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_AXES_GROUP_INVALID_STATE;
		break;
	case MC_GROUP_INVALID_STATE_MOVING:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_INVALID_STATE_MOVING;
		break;
	case MC_GROUP_INVALID_STATE_HOMING:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_INVALID_STATE_HOMING;
		break;
	case MC_GROUP_INVALID_STATE_STOPPING:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_INVALID_STATE_STOPPING;
		break;
	case MC_GROUP_INVALID_STATE_ERRORSTOP:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_INVALID_STATE_ERRORSTOP;
		break;
	case MC_AXIS_IN_SINGLE_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_AXIS_IN_SINGLE_MOTION_STATE;
		break;
	case MC_1ST_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_1ST_AXIS_IN_MOTION_STATE;
		break;
	case MC_2ND_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_2ND_AXIS_IN_MOTION_STATE;
		break;
	case MC_3RD_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_3RD_AXIS_IN_MOTION_STATE;
		break;
	case MC_4TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_4TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_5TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_5TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_6TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_6TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_7TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_7TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_8TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_8TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_GROUP_MEMBER_EMPTY:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_MEMBER_EMPTY;
		break;
	case MC_1ST_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_1ST_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_2ND_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_2ND_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_3RD_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_3RD_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_4TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_4TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_5TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_5TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_6TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_6TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_7TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_7TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_8TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_8TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_GROUP_CMD_SIZE_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_CMD_SIZE_ERROR;
		break;
	case MC_GROUP_CMD_PARAMETER_SIZE_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_CMD_PARAMETER_SIZE_ERROR;
		break;
	case MC_GROUP_MEMBER_NOT_ALLOCATED_X:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_MEMBER_NOT_ALLOCATED_X;
		break;
	case MC_GROUP_MEMBER_NOT_ALLOCATED_Y:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_MEMBER_NOT_ALLOCATED_Y;
		break;
	case MC_AXIS_IN_GROUP_MOTION:
		iReturnErrorCode = ERR_RS_NMC_MC_AXIS_IN_GROUP_MOTION;
		break;
	case MC_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_FAIL;
		break;
	case MC_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_ERROR;
		break;
	case MC_IOMAPING_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_IOMAPING_ERR;
		break;
	case MC_COMMINIT_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_COMMINIT_ERR;
		break;
	case MC_COMM_EVENT_INIT_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_COMM_EVENT_INIT_ERR;
		break;
	case MC_READ_ENI_NODE_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_READ_ENI_NODE_ERR;
		break;
	case MC_INVALID_AXIS_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_ERR;
		break;
	case MC_INVALID_BOARD_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_BOARD_ERR;
		break;
	case MC_XML_PARSING_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_XML_PARSING_ERR;
		break;
	case MC_XML_ITEM_COUNT_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_MC_XML_ITEM_COUNT_MISMATCH;
		break;
	case MC_NO_BOARD_INSTALLED:
		iReturnErrorCode = ERR_RS_NMC_MC_NO_BOARD_INSTALLED;
		break;
	case MC_INVALID_DOWNLOAD_FILE_TYPE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_DOWNLOAD_FILE_TYPE;
		break;
	case MC_OPEN_ENI_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_OPEN_ENI_ERR;
		break;
	case MC_FILE_OPEN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_FILE_OPEN_FAIL;
		break;
	case MC_NO_MATCHING_DOWNLOADINFORMATION:
		iReturnErrorCode = ERR_RS_NMC_MC_NO_MATCHING_DOWNLOADINFORMATION;
		break;
	case MC_NONE_OP:
		iReturnErrorCode = ERR_RS_NMC_MC_NONE_OP;
		break;
	case MC_FAIL_GEN_DOWNLOAD_FILE:
		iReturnErrorCode = ERR_RS_NMC_MC_FAIL_GEN_DOWNLOAD_FILE;
		break;
	case MC_REG_KEY_READ_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_REG_KEY_READ_FAIL;
		break;
	case MC_NOT_ALLOWED_IN_THIS_MASTER_MODE:
		iReturnErrorCode = ERR_RS_NMC_MC_NOT_ALLOWED_IN_THIS_MASTER_MODE;
		break;
	case MC_MASTERID_OUT_OF_RANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTERID_OUT_OF_RANGE;
		break;
	case MC_BOARDNO_OUT_OF_RANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_BOARDNO_OUT_OF_RANGE;
		break;
	case MC_AXISNO_OUT_OF_RANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_AXISNO_OUT_OF_RANGE;
		break;
	case MC_BOARDCNT_OUT_OF_RANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_BOARDCNT_OUT_OF_RANGE;
		break;
	case MC_RETURN_SIZE_NOT_EQUAL:
		iReturnErrorCode = ERR_RS_NMC_MC_RETURN_SIZE_NOT_EQUAL;
		break;
	case MC_MASTERID_DUPLICATION_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTERID_DUPLICATION_ERR;
		break;
	case MC_PARAM_ERROR_FILE_IS_NULL:
		iReturnErrorCode = ERR_RS_NMC_MC_PARAM_ERROR_FILE_IS_NULL;
		break;
	case MC_NO_MATCHING_BOARDID_FOUND:
		iReturnErrorCode = ERR_RS_NMC_MC_NO_MATCHING_BOARDID_FOUND;
		break;
	case MC_NOT_READY_NETWORK_CONFIGURATION:
		iReturnErrorCode = ERR_RS_NMC_MC_NOT_READY_NETWORK_CONFIGURATION;
		break;
	case MC_INVALID_MASTERID_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_MASTERID_ERR;
		break;
	case MC_MASTER_MODE_CHANGE_NOT_ALLOWED:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTER_MODE_CHANGE_NOT_ALLOWED;
		break;
	case MC_MASTER_REQUEST_PARAM_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTER_REQUEST_PARAM_ERROR;
		break;
	case MC_MASTER_INVALID_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTER_INVALID_STATE;
		break;
	case MC_NOT_MOTION_LIBRAY_INITIALIZED:
		iReturnErrorCode = ERR_RS_NMC_MC_NOT_MOTION_LIBRAY_INITIALIZED;
		break;
	case MC_IOMANAGER_NOT_RUNNING:
		iReturnErrorCode = ERR_RS_NMC_MC_IOMANAGER_NOT_RUNNING;
		break;
	case MC_ANOTHER_PROGRAM_IS_USING_NMC_LIBRARY:
		iReturnErrorCode = ERR_RS_NMC_MC_ANOTHER_PROGRAM_IS_USING_NMC_LIBRARY;
		break;
	case MC_SLAVE_ITEM_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_MC_SLAVE_ITEM_MISMATCH;
		break;
	case MC_SLAVE_ITEM_COUNT_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_MC_SLAVE_ITEM_COUNT_MISMATCH;
		break;
	case MC_PCICIP_GEN_10:
		iReturnErrorCode = ERR_RS_NMC_MC_PCICIP_GEN_10;
		break;
	case COMM_CONNECTION_ESTABLISHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_ESTABLISHED;
		break;
	case COMM_CONN_CONFIG_FAILED_INVALID_NETWORK_PATH:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONN_CONFIG_FAILED_INVALID_NETWORK_PATH;
		break;
	case COMM_CONN_CONFIG_FAILED_NO_RESPONSE:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONN_CONFIG_FAILED_NO_RESPONSE;
		break;
	case COMM_CONN_CONFIG_FAILED_ERROR_RESPONSE:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONN_CONFIG_FAILED_ERROR_RESPONSE;
		break;
	case COMM_CONNECTION_TIMED_OUT:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_TIMED_OUT;
		break;
	case COMM_CONNECTION_CLOSED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_CLOSED;
		break;
	case COMM_INCOMING_CONNECTION_RUN_IDLE_FLAG_CHANGED:
		iReturnErrorCode = ERR_RS_NMC_COMM_INCOMING_CONNECTION_RUN_IDLE_FLAG_CHANGED;
		break;
	case COMM_ASSEMBLY_NEW_INSTANCE_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ASSEMBLY_NEW_INSTANCE_DATA;
		break;
	case COMM_ASSEMBLY_NEW_MEMBER_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ASSEMBLY_NEW_MEMBER_DATA;
		break;
	case COMM_CONNECTION_NEW_INPUT_SCANNER_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_NEW_INPUT_SCANNER_DATA;
		break;
	case COMM_CONNECTION_VERIFICATION:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_VERIFICATION;
		break;
	case COMM_CONNECTION_RECONFIGURED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_RECONFIGURED;
		break;
	case COMM_REQUEST_RESPONSE_RECEIVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_REQUEST_RESPONSE_RECEIVED;
		break;
	case COMM_REQUEST_FAILED_INVALID_NETWORK_PATH:
		iReturnErrorCode = ERR_RS_NMC_COMM_REQUEST_FAILED_INVALID_NETWORK_PATH;
		break;
	case COMM_REQUEST_TIMED_OUT:
		iReturnErrorCode = ERR_RS_NMC_COMM_REQUEST_TIMED_OUT;
		break;
	case COMM_CLIENT_OBJECT_REQUEST_RECEIVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CLIENT_OBJECT_REQUEST_RECEIVED;
		break;
	case COMM_NEW_CLASS3_RESPONSE:
		iReturnErrorCode = ERR_RS_NMC_COMM_NEW_CLASS3_RESPONSE;
		break;
	case COMM_CLIENT_PCCC_REQUEST_RECEIVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CLIENT_PCCC_REQUEST_RECEIVED;
		break;
	case COMM_NEW_LIST_IDENTITY_RESPONSE:
		iReturnErrorCode = ERR_RS_NMC_COMM_NEW_LIST_IDENTITY_RESPONSE;
		break;
	case COMM_ID_RESET:
		iReturnErrorCode = ERR_RS_NMC_COMM_ID_RESET;
		break;
	case COMM_BACKPLANE_REQUEST_RECEIVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_BACKPLANE_REQUEST_RECEIVED;
		break;
	case COMM_OUT_OF_MEMORY:
		iReturnErrorCode = ERR_RS_NMC_COMM_OUT_OF_MEMORY;
		break;
	case COMM_UNABLE_INTIALIZE_WINSOCK:
		iReturnErrorCode = ERR_RS_NMC_COMM_UNABLE_INTIALIZE_WINSOCK;
		break;
	case COMM_UNABLE_START_THREAD:
		iReturnErrorCode = ERR_RS_NMC_COMM_UNABLE_START_THREAD;
		break;
	case COMM_ERROR_USING_WINSOCK:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_USING_WINSOCK;
		break;
	case COMM_ERROR_SETTING_SOCKET_TO_NONBLOCKING:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_SETTING_SOCKET_TO_NONBLOCKING;
		break;
	case COMM_ERROR_SETTING_TIMER:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_SETTING_TIMER;
		break;
	case COMM_SESSION_COUNT_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_SESSION_COUNT_LIMIT_REACHED;
		break;
	case COMM_CONNECTION_COUNT_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_COUNT_LIMIT_REACHED;
		break;
	case COMM_PENDING_REQUESTS_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_PENDING_REQUESTS_LIMIT_REACHED;
		break;
	case COMM_PENDING_REQUEST_GROUPS_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_PENDING_REQUEST_GROUPS_LIMIT_REACHED;
		break;
	case COMM_ERROR_UNABLE_START_MODBUS:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_UNABLE_START_MODBUS;
		break;
	case COMM_ERROR_HW_NOT_INSTALLED:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_HW_NOT_INSTALLED;
		break;
	case COMM_ERROR_DD_SEND_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DD_SEND_ERROR;
		break;
	case COMM_ERROR_DD_READ_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DD_READ_ERROR;
		break;
	case COMM_DD_ERROR_SEND:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_ERROR_SEND;
		break;
	case COMM_DD_ERROR_RECV:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_ERROR_RECV;
		break;
	case COMM_DD_OPEN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_OPEN_FAIL;
		break;
	case COMM_DD_NOT_OPENED:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_NOT_OPENED;
		break;
	case COMM_DD_CONN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_CONN_FAIL;
		break;
	case COMM_DD_CLIENT_START_FAIL:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_CLIENT_START_FAIL;
		break;
	case COMM_DD_OK:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_OK;
		break;
	case COMM_CN_NOT_CONNECTED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CN_NOT_CONNECTED;
		break;
	case COMM_CN_CONNECTED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CN_CONNECTED;
		break;
	case COMM_CN_CONNERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_CN_CONNERROR;
		break;
	case COMM_ERROR_SUCCESS:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_SUCCESS;
		break;
	case COMM_ERROR_FAILURE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_FAILURE;
		break;
	case COMM_EXT_ERR_DUPLICATE_FWD_OPEN:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_DUPLICATE_FWD_OPEN;
		break;
	case COMM_EXT_ERR_CLASS_TRIGGER_INVALID:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CLASS_TRIGGER_INVALID;
		break;
	case COMM_EXT_ERR_OWNERSHIP_CONFLICT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_OWNERSHIP_CONFLICT;
		break;
	case COMM_EXT_ERR_CONNECTION_NOT_FOUND:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CONNECTION_NOT_FOUND;
		break;
	case COMM_EXT_ERR_INVALID_CONN_TYPE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_CONN_TYPE;
		break;
	case COMM_EXT_ERR_INVALID_CONN_SIZE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_CONN_SIZE;
		break;
	case COMM_EXT_ERR_DEVICE_NOT_CONFIGURED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_DEVICE_NOT_CONFIGURED;
		break;
	case COMM_EXT_ERR_RPI_NOT_SUPPORTED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_RPI_NOT_SUPPORTED;
		break;
	case COMM_EXT_ERR_CONNECTION_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CONNECTION_LIMIT_REACHED;
		break;
	case COMM_EXT_ERR_VENDOR_PRODUCT_CODE_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_VENDOR_PRODUCT_CODE_MISMATCH;
		break;
	case COMM_EXT_ERR_PRODUCT_TYPE_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_PRODUCT_TYPE_MISMATCH;
		break;
	case COMM_EXT_ERR_REVISION_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_REVISION_MISMATCH;
		break;
	case COMM_EXT_ERR_INVALID_CONN_POINT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_CONN_POINT;
		break;
	case COMM_EXT_ERR_INVALID_CONFIG_FORMAT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_CONFIG_FORMAT;
		break;
	case COMM_EXT_ERR_NO_CONTROLLING_CONNECTION:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_NO_CONTROLLING_CONNECTION;
		break;
	case COMM_EXT_ERR_TARGET_CONN_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_TARGET_CONN_LIMIT_REACHED;
		break;
	case COMM_EXT_ERR_RPI_SMALLER_THAN_INHIBIT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_RPI_SMALLER_THAN_INHIBIT;
		break;
	case COMM_EXT_ERR_CONNECTION_TIMED_OUT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CONNECTION_TIMED_OUT;
		break;
	case COMM_EXT_ERR_UNCONNECTED_SEND_TIMED_OUT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_UNCONNECTED_SEND_TIMED_OUT;
		break;
	case COMM_EXT_ERR_PARAMETER_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_PARAMETER_ERROR;
		break;
	case COMM_EXT_ERR_MESSAGE_TOO_LARGE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_MESSAGE_TOO_LARGE;
		break;
	case COMM_EXT_ERR_UNCONN_ACK_WITHOUT_REPLY:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_UNCONN_ACK_WITHOUT_REPLY;
		break;
	case COMM_EXT_ERR_NO_BUFFER_MEMORY_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_NO_BUFFER_MEMORY_AVAILABLE;
		break;
	case COMM_EXT_ERR_BANDWIDTH_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_BANDWIDTH_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_TAG_FILTERS_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_TAG_FILTERS_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_REAL_TIME_DATA_NOT_CONFIG:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_REAL_TIME_DATA_NOT_CONFIG;
		break;
	case COMM_EXT_ERR_PORT_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_PORT_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_LINK_ADDR_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_LINK_ADDR_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_INVALID_SEGMENT_TYPE_VALUE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_SEGMENT_TYPE_VALUE;
		break;
	case COMM_EXT_ERR_PATH_CONNECTION_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_PATH_CONNECTION_MISMATCH;
		break;
	case COMM_EXT_ERR_INVALID_NETWORK_SEGMENT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_NETWORK_SEGMENT;
		break;
	case COMM_EXT_ERR_INVALID_LINK_ADDRESS:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_LINK_ADDRESS;
		break;
	case COMM_EXT_ERR_SECOND_RESOURCES_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_SECOND_RESOURCES_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_CONNECTION_ALREADY_ESTABLISHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CONNECTION_ALREADY_ESTABLISHED;
		break;
	case COMM_EXT_ERR_DIRECT_CONN_ALREADY_ESTABLISHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_DIRECT_CONN_ALREADY_ESTABLISHED;
		break;
	case COMM_EXT_ERR_MISC:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_MISC;
		break;
	case COMM_EXT_ERR_REDUNDANT_CONNECTION_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_REDUNDANT_CONNECTION_MISMATCH;
		break;
	case COMM_EXT_ERR_NO_MORE_CONSUMER_RESOURCES:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_NO_MORE_CONSUMER_RESOURCES;
		break;
	case COMM_EXT_ERR_NO_TARGET_PATH_RESOURCES:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_NO_TARGET_PATH_RESOURCES;
		break;
	case COMM_EXT_ERR_VENDOR_SPECIFIC:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_VENDOR_SPECIFIC;
		break;
	case COMM_ERROR_NO_RESOURCE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_NO_RESOURCE;
		break;
	case COMM_ERROR_INVALID_PARAMETER_VALUE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_PARAMETER_VALUE;
		break;
	case COMM_ERROR_INVALID_SEG_TYPE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_SEG_TYPE;
		break;
	case COMM_ERROR_INVALID_DESTINATION:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_DESTINATION;
		break;
	case COMM_ERROR_PARTIAL_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_PARTIAL_DATA;
		break;
	case COMM_ERROR_CONN_LOST:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_CONN_LOST;
		break;
	case COMM_ERROR_BAD_SERVICE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_BAD_SERVICE;
		break;
	case COMM_ERROR_BAD_ATTR_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_BAD_ATTR_DATA;
		break;
	case COMM_ERROR_ATTR_LIST_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ATTR_LIST_ERROR;
		break;
	case COMM_ERROR_ALREADY_IN_REQUESTED_MODE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ALREADY_IN_REQUESTED_MODE;
		break;
	case COMM_ERROR_OBJECT_STATE_CONFLICT:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_OBJECT_STATE_CONFLICT;
		break;
	case COMM_ERROR_OBJ_ALREADY_EXISTS:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_OBJ_ALREADY_EXISTS;
		break;
	case COMM_ERROR_ATTR_NOT_SETTABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ATTR_NOT_SETTABLE;
		break;
	case COMM_ERROR_PERMISSION_DENIED:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_PERMISSION_DENIED;
		break;
	case COMM_ERROR_DEV_IN_WRONG_STATE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DEV_IN_WRONG_STATE;
		break;
	case COMM_ERROR_REPLY_DATA_TOO_LARGE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_REPLY_DATA_TOO_LARGE;
		break;
	case COMM_ERROR_FRAGMENT_PRIMITIVE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_FRAGMENT_PRIMITIVE;
		break;
	case COMM_ERROR_NOT_ENOUGH_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_NOT_ENOUGH_DATA;
		break;
	case COMM_ERROR_ATTR_NOT_SUPPORTED:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ATTR_NOT_SUPPORTED;
		break;
	case COMM_ERROR_TOO_MUCH_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_TOO_MUCH_DATA;
		break;
	case COMM_ERROR_OBJ_DOES_NOT_EXIST:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_OBJ_DOES_NOT_EXIST;
		break;
	case COMM_ERROR_NO_FRAGMENTATION:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_NO_FRAGMENTATION;
		break;
	case COMM_ERROR_DATA_NOT_SAVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DATA_NOT_SAVED;
		break;
	case COMM_ERROR_DATA_WRITE_FAILURE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DATA_WRITE_FAILURE;
		break;
	case COMM_ERROR_REQUEST_TOO_LARGE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_REQUEST_TOO_LARGE;
		break;
	case COMM_ERROR_RESPONSE_TOO_LARGE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_RESPONSE_TOO_LARGE;
		break;
	case COMM_ERROR_MISSING_LIST_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_MISSING_LIST_DATA;
		break;
	case COMM_ERROR_INVALID_LIST_STATUS:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_LIST_STATUS;
		break;
	case COMM_ERROR_SERVICE_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_SERVICE_ERROR;
		break;
	case COMM_ERROR_VENDOR_SPECIFIC:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_VENDOR_SPECIFIC;
		break;
	case COMM_ERROR_INVALID_PARAMETER:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_PARAMETER;
		break;
	case COMM_ERROR_WRITE_ONCE_FAILURE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_WRITE_ONCE_FAILURE;
		break;
	case COMM_ERROR_INVALID_REPLY:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_REPLY;
		break;
	case COMM_ERROR_BAD_KEY_IN_PATH:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_BAD_KEY_IN_PATH;
		break;
	case COMM_ERROR_BAD_PATH_SIZE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_BAD_PATH_SIZE;
		break;
	case COMM_ERROR_UNEXPECTED_ATTR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_UNEXPECTED_ATTR;
		break;
	case COMM_ERROR_INVALID_MEMBER:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_MEMBER;
		break;
	case COMM_ERROR_MEMBER_NOT_SETTABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_MEMBER_NOT_SETTABLE;
		break;
	case COMM_ERROR_UNKNOWN_MODBUS_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_UNKNOWN_MODBUS_ERROR;
		break;
	case COMM_ERROR_HW_NOT_INSTALLED1:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_HW_NOT_INSTALLED1;
		break;
	case COMM_ERROR_ENCAP_PROTOCOL:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ENCAP_PROTOCOL;
		break;
	case COMM_ERROR_STILL_PROCESSING:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_STILL_PROCESSING;
		break;
	case MC_DOWNLOAD_FAIL_DUE_TO_ANOTHER_PROGRAM_IS_RUNNING:
		iReturnErrorCode = ERR_RS_NMC_MC_DOWNLOAD_FAIL_DUE_TO_ANOTHER_PROGRAM_IS_RUNNING;
		break;
	default:
		
		ErrorMessage(mcStatus, chGetErrorMessage);
		
		iReturnErrorCode = ERR_RS_NMC_UNKNOWN;
		break;
	}
	return generateErrorCode(iReturnErrorCode + 106000);
//	return ((iReturnErrorCode + 106000) * 1000 )+ m_iInstanceNo;
}

// 반올림 Function
double MRSNMCMultiAxis::motionRoundValue(double dVal)
{
	return (ceil(dVal + 0.5) - 1.0);
}

ErrorInfo MRSNMCMultiAxis::ErrorMessage(int iCode, char *pcDst)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

#ifndef SIMULATION
	MC_STATUS ms;
	ms = MC_GetErrorMessage(iCode, 128, pcDst);
	if (ms != MC_OK)
	{
		strLogMsg.Format(_T("ErrorMessage(): MC_GetErrorMessage(return the error message) Fail[ErrorCode=%d] - Error# is %d"), iCode, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#else
	//pcDst = _T("Simulation_test");
#endif

	return NO_ERROR_SUCESS;
}
ErrorInfo MRSNMCMultiAxis::GroupDisable()
{
	MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
	return NO_ERROR_SUCESS;
}
/*
//170709_KDH Group 추가
// 그룹 활성화
int MRSNMCMultiAxis::GroupEnable()
{
#ifdef SIMULATION
	return NO_ERROR_SUCESS;
#endif
	
	CString strLogMsg;
	MC_STATUS ms;
	int i = 0;
	
	
	for (int i = 0; i < m_iAxisNum; i++)
	{	
		ms = MC_AddAxisToGroup(m_plnkRingID[i], m_plnkNodeID[i], m_iGroupID, i);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Grouping2Ax(): MC_AddAxisToGroup(return the error message) Fail[GroupID=%d] - Error# is %d"), m_iGroupID, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			MC_UngroupAllAxes(m_plnkRingID[i], m_iGroupID);
			MC_GroupDisable(m_plnkRingID[0], m_iGroupID);
			return MakeRS_NMC_Error(ms);
		}
		
		Sleep(10);
	}


	ms = MC_GroupEnable(m_plnkRingID[0], m_iGroupID);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("GroupEnable(): MC_GroupEnable(return the error message) Fail[GroupID=%d] - Error# is %d"), m_iGroupID, MakeRS_NMC_Error(ms));
		WriteErrorLog(strLogMsg);
		return MakeRS_NMC_Error(ms);
	}
	
	Sleep(10);
	
	return NO_ERROR_SUCESS;
}
*/
//______________________________
