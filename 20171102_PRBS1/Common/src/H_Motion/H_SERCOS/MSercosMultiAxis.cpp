// MSercosMultiAxis.cpp: implementation of the MSercosMultiAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSercosMultiAxis.h"
#include <math.h>

#include "common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


MSercosMultiAxis::MSercosMultiAxis(SCommonAttribute commonData, IAxis** pAxis, int iAxisNum)
: ICommon(commonData)
{
	ASSERT(pAxis != NULL);
//	ASSERT(iAxisNum >=0 && iAxisNum<=DEF_MAX_SERCOS_AXIS_NO);
	
	m_iAxisNum			= iAxisNum;
	m_plnkAxis			= new MSercosAxis*[m_iAxisNum];
	m_plnkRingID		= new INT32S[m_iAxisNum];
	m_plnkNodeID		= new INT32S[m_iAxisNum];

	m_pMoveVelocity		= new double[m_iAxisNum];
	m_pMoveSlowVelocity	= new double[m_iAxisNum];
	m_pAccelerate		= new int[m_iAxisNum];
	m_pScale			= new double[m_iAxisNum];
	m_pAxisID			= new int[m_iAxisNum];
	
	int i = 0;
	for (int i = 0; i < m_iAxisNum; i++)
	{
		m_plnkAxis[i]			= (MSercosAxis*)pAxis[i];
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
MSercosMultiAxis::~MSercosMultiAxis()
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

int MSercosMultiAxis::StartMove(double *dPosition, double *dVelocity,
							double *dAccelerate, double *dDecelerate, int bIsTrapMove)
{
	int i = 0;
	int iResult = ERR_MOTION_SUCCESS;

	double* dPos	= new double[m_iAxisNum];
	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iPos	= new int[m_iAxisNum];
	int*	iJerk	= new int[m_iAxisNum];

	/** 이동 전 원점복귀 여부 확인, 축 상태 점검 */
	for (int i = 0; i < m_iAxisNum; i++)
	{
		/** 이동 전 원점복귀 여부 확인 */
		if (!m_plnkAxis[i]->IsOriginReturn())
		{
			iResult = generateErrorCode(107906);
			goto RETURN;
		}

		/** 축 상태 점검 */
		iResult = m_plnkAxis[i]->checkAxisState(m_pAxisID[i]);
		if (iResult)
			goto RETURN;

		/** SW Limit Check */
		if (0 == i) // 기준축만 S/W Limit 체크.
		{
			iResult = m_plnkAxis[0]->checkSWLimit(dPosition[0]);
			if (iResult)
				goto RETURN;
		}

		// Profile 생성.

		dPos[i] = m_pScale[i] * dPosition[i];
		iPos[i]	= (int)dPos[i];

		/** 이동 속도 처리 */
		if (dVelocity == NULL)
		{
			if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
				dVel[i] = m_pScale[i] * m_pMoveVelocity[i];
			else
				dVel[i] = m_pScale[i] * m_pMoveSlowVelocity[i];
		}
		else
			dVel[i] =  m_pScale[i] * dVelocity[i];
		
		/** 이동 가속도 처리 */
		if (dVelocity == NULL)
		{
			if (dAccelerate == NULL)
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
			if (dAccelerate == NULL)
				dAccel[i] = (double)((m_pScale[i] * dVelocity[i])/((double)m_pAccelerate[i]/1000));
			else
				dAccel[i] = (double)((m_pScale[i] * dVelocity[i])/((double)dAccelerate[i]/1000));
		}
		
		/** 이동 감속도 처리 */
		if (dVelocity == NULL)
		{
			if (dDecelerate == NULL)
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
			if (dDecelerate == NULL)
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
		
		iResult = smmc_set_profile(m_plnkRingID[i], m_plnkNodeID[i], (int)*dVel, (int)*dAccel, (int)*dDecel, iJerk[i]);
		if (iResult != ERR_MOTION_SUCCESS)
		{
			iResult = m_plnkAxis[i]->makeSercosError(iResult);
			goto RETURN;
		}
	}

	iResult = smmc_move_n(m_iAxisNum, m_plnkAxis[0]->GetRingID(), m_plnkNodeID, iPos);
	if (iResult != ERR_MOTION_SUCCESS)
	{
		iResult = m_plnkAxis[0]->makeSercosError(iResult);
		goto RETURN;
	}

RETURN:
	delete[] dPos;
	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iPos;
	delete[] iJerk;

	return iResult;
}

int MSercosMultiAxis::Move(double * dPosition, double * dVelocity, double* dAccelerate, 
						   double* dDecelerate, int bIsTrapMove,  int iMoveType)
{
	int iResult = StartMove(dPosition, dVelocity, dAccelerate, dDecelerate, bIsTrapMove);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	iResult = Wait4Done(FALSE, iMoveType);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
}

int MSercosMultiAxis::StartRMove(double *dPosition, double *dVelocity,
							double *dAccelerate, double *dDecelerate, int bIsTrapMove)
{
	int i = 0;
	int iResult = ERR_MOTION_SUCCESS;

	double* dPos	= new double[m_iAxisNum];
	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iPos	= new int[m_iAxisNum];
	int*	iJerk	= new int[m_iAxisNum];

	for (int i = 0; i < m_iAxisNum; i++)
	{
		/** 이동 전 원점복귀 여부 확인 */
		if (!m_plnkAxis[i]->IsOriginReturn())
		{
			iResult = generateErrorCode(107906);
			goto RETURN;
		}
		
		/** 축 상태 점검 */
		iResult = m_plnkAxis[i]->checkAxisState(m_pAxisID[i]);
		if (iResult)
			goto RETURN;
		
		/** SW Limit Check */
		if (0 == i)
		{
			iResult = m_plnkAxis[0]->checkSWLimit(dPosition[0]);
			if (iResult != ERR_MOTION_SUCCESS)
				goto RETURN;
		}

		dPos[i] = m_pScale[i] * dPosition[i];
		iPos[i]	= (int)dPos[i];

		/** 이동 속도 처리 */
		if (dVelocity == NULL)
		{
			if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
				dVel[i] = m_pScale[i] * m_pMoveVelocity[i];
			else
				dVel[i] = m_pScale[i] * m_pMoveSlowVelocity[i];
		}
		else
			dVel[i] =  m_pScale[i] * dVelocity[i];
		
		/** 이동 가속도 처리 */
		if (dVelocity == NULL)
		{
			if (dAccelerate == NULL)
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
			if (dAccelerate == NULL)
				dAccel[i] = (double)((m_pScale[i] * dVelocity[i])/((double)m_pAccelerate[i]/1000));
			else
				dAccel[i] = (double)((m_pScale[i] * dVelocity[i])/((double)dAccelerate[i]/1000));
		}
		
		/** 이동 감속도 처리 */
		if (dVelocity == NULL)
		{
			if (dDecelerate == NULL)
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
			if (dDecelerate == NULL)
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
			iJerk[i] = INT32U(0);			// Curve 형태를 결정한다. (사다리꼴)          
		}         
		
		iResult = smmc_set_profile(m_plnkRingID[i], m_plnkNodeID[i], (int)*dVel, (int)*dAccel, (int)*dDecel, iJerk[i]);
		if (iResult != ERR_MOTION_SUCCESS)
		{
			iResult = m_plnkAxis[i]->makeSercosError(iResult);
			goto RETURN;
			
		}
	}

	if (2 == m_iAxisNum)
	{
		iResult = smmc_r_move_2(m_plnkAxis[0]->GetRingID(), m_plnkNodeID[0], m_plnkNodeID[1], iPos[0], iPos[1]);
	}
	else if (3 == m_iAxisNum)
	{
		iResult = smmc_r_move_3(m_plnkAxis[0]->GetRingID(), m_plnkNodeID[0], m_plnkNodeID[1], m_plnkNodeID[2], iPos[0], iPos[1], iPos[2]);
	}

	if (iResult != ERR_MOTION_SUCCESS)
	{
		iResult = m_plnkAxis[i]->makeSercosError(iResult);
		goto RETURN;
	}

RETURN:
	delete[] dPos;
	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iPos;
	delete[] iJerk;

	return iResult;
}

int MSercosMultiAxis::RMove(double * dPosition, double * dVelocity, double* dAccelerate, 
						   double* dDecelerate, int bIsTrapMove,  int iMoveType)
{
	int iResult = StartRMove(dPosition, dVelocity, dAccelerate, dDecelerate, bIsTrapMove);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;

	iResult = Wait4Done(FALSE, iMoveType);
	if (iResult != ERR_MOTION_SUCCESS)
		return iResult;
	
	return ERR_MOTION_SUCCESS;
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
int MSercosMultiAxis::Wait4Done(BOOL bSkipStableTime, int iMoveType, BOOL bUseInterlock)
{
#ifdef DEF_DONE_EVENT_TYPE
	return ERR_MOTION_SUCCESS;
#endif	
	
	int i = 0;
	int	iResult = ERR_MOTION_SUCCESS;

	for (int i = 0; i < m_iAxisNum; i++)
	{
		iResult = m_plnkAxis[i]->Wait4Done();
		if (iResult)
		{
			for (int j = 0; j < m_iAxisNum; j++)
				m_plnkAxis[j]->ClearFrames();
			return iResult;
		}
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
int MSercosMultiAxis::JogMoveSlow (BOOL bDir, double dSlow)
{
	int iResult = ERR_MOTION_SUCCESS;

	double* dPos	= new double[m_iAxisNum];
	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iPos	= new int[m_iAxisNum];
	int*	iJerk   = new int[m_iAxisNum];
	int		iDir;

	JogStop();

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
			dVel[i] = m_pScale[i] * m_plnkAxis[i]->GetJogSlow();
		else 
			dVel[i] = m_pScale[i] * dSlow;
		
		// 이동 가감속 처리.
		dAccel[i] = dVel[i] / ((double)m_pAccelerate[i]/1000);
		dDecel[i] = dAccel[i];
		
		// S-Curve 형태 설정.
		iJerk[i] = INT32U(0);			// Curve 형태를 결정한다. (사다리꼴)          
		
		iResult = smmc_v_move(m_plnkRingID[i], m_plnkNodeID[i], iDir * (INT32S)dVel[0], (INT32S)dAccel[0], (INT32S)dDecel[0], iJerk[i]);
		if (iResult != ERR_MOTION_SUCCESS)
		{
			delete[] dPos;
			delete[] dVel;
			delete[] dAccel;
			delete[] dDecel;
			delete[] iPos;
			delete[] iJerk;

			return m_plnkAxis[i]->makeSercosError(iResult);
		}
	}

	delete[] dPos;
	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iPos;
	delete[] iJerk;

	return iResult;
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
int MSercosMultiAxis::JogMoveFast (BOOL bDir, double dFast)
{
	int iResult = ERR_MOTION_SUCCESS;

	double* dPos	= new double[m_iAxisNum];
	double* dVel	= new double[m_iAxisNum];
	double* dAccel	= new double[m_iAxisNum];
	double* dDecel	= new double[m_iAxisNum];
	int*	iPos	= new int[m_iAxisNum];
	int*	iJerk   = new int[m_iAxisNum];
	int		iDir;

	JogStop();

	/** (+) 방향 */
	if (bDir == TRUE)
		iDir = 1;
	/** (-) 방향 */
	else
		iDir = -1;

	for (int i = 0; i < m_iAxisNum; i++)
	{	
		// 이동 속도 처리.
		if (0.0 == dFast)
			dVel[i] = m_pScale[i] * m_plnkAxis[i]->GetJogFast();
		else 
			dVel[i] = m_pScale[i] * dFast;
		
		// 이동 가감속 처리.
		dAccel[i]	= dVel[i] / ((double)m_pAccelerate[i]/1000);
		dDecel[i]	= dAccel[i];
		
		// S-Curve 형태 설정.
		iJerk[i]	= INT32U(0);			// Curve 형태를 결정한다. (사다리꼴)          
		
		iResult = smmc_v_move(m_plnkRingID[i], m_plnkNodeID[i], iDir * (INT32S)dVel[0], (INT32S)dAccel[0], (INT32S)dDecel[0], iJerk[i]);
		if (iResult != ERR_MOTION_SUCCESS)
		{
			delete[] dPos;
			delete[] dVel;
			delete[] dAccel;
			delete[] dDecel;
			delete[] iPos;
			delete[] iJerk;
			return m_plnkAxis[i]->makeSercosError(iResult);
		}
	}

	delete[] dPos;
	delete[] dVel;
	delete[] dAccel;
	delete[] dDecel;
	delete[] iPos;
	delete[] iJerk;

	return iResult;
	
}

/**
 * Jog 이동을 정지 시킨다.
 * 	 
 * @return	Error Code		 : 0 = SUCCESS
 *							  그 외 =  Error Code
 */
int MSercosMultiAxis::JogStop()
{
	int i = 0;
	int iResult = ERR_MOTION_SUCCESS;

	for (int i = 0; i < m_iAxisNum; i++)
	{	
		iResult = smmc_v_move_stop(m_plnkRingID[i], m_plnkNodeID[i]);
		if (iResult != ERR_MOTION_SUCCESS)
			m_plnkAxis[i]->makeSercosError(iResult);
	}

	for (int i = 0; i < m_iAxisNum; i++)
	{	
		int iState;
		iResult = smmc_in_position_event(m_plnkRingID[i], m_plnkNodeID[i], 1000, &iState);
	}
	
//	if (!IsAxisDone()) return ERR_MOTION_SUCCESS;

	return iResult;
}

int MSercosMultiAxis::Stop()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	
	INT32S	iErr;
	int iResult = ERR_MOTION_SUCCESS;

	// 한축만 멈추면 그외 모든 축이 멈춤
	iErr = smmc_set_stop(m_plnkRingID[0], m_plnkNodeID[0]);
	if (iErr != ERR_MOTION_SUCCESS)
		m_plnkAxis[0]->makeSercosError(iErr);

	return iResult;
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
int MSercosMultiAxis::MoveSplineLine2(int iMaxPoint, double *pdPosition, double *pdPosition2, double *pdVelocity, int *piAccelerate,
								 BOOL bAutoFlag, BOOL bWaitOpt)
{
	int		iResult = ERR_MOTION_SUCCESS;

	double	dPos;
	double  dPos2;
	double	dVel;
	double  dVel2;
	int		iAccel;
	int		iAccel2;
	int		iJerk;
	int		iJerk2;	

	for (int i = 0; i < m_iAxisNum; i++)
	{
		/** 이동 전 원점복귀 여부 확인 */
		if (!m_plnkAxis[i]->IsOriginReturn())
			return generateErrorCode(107906);
		
		/** 축 상태 점검 */
		if ((iResult = m_plnkAxis[i]->checkAxisState(m_pAxisID[i])) != ERR_MOTION_SUCCESS)
			return iResult;
	}

	/** SW Limit Check */ // 기준축만 SW 체크 하지...
	if ((iResult = m_plnkAxis[0]->checkSWLimit(pdPosition[0])) != ERR_MOTION_SUCCESS)
		return iResult;

	if ((iResult = m_plnkAxis[1]->checkSWLimit(pdPosition2[0])) != ERR_MOTION_SUCCESS)
		return iResult;

	// 이동 Point에 따라 
	for (int iPoint = 0; iPoint < iMaxPoint; iPoint++)
	{
		dPos =  m_pScale[0] * pdPosition[iPoint];
		dPos2 =  m_pScale[1] * pdPosition2[iPoint];

		dVel =  m_pScale[0] * pdVelocity[iPoint];
		dVel2 =  m_pScale[1] * pdVelocity[iPoint];

		iAccel = INT32U((m_pScale[0] * pdVelocity[iPoint]) / ((double)piAccelerate[iPoint]/1000));
		iAccel2 = INT32U((m_pScale[1] * pdVelocity[iPoint]) / ((double)piAccelerate[iPoint]/1000));

		iJerk = INT32U(iAccel * 20);
		iJerk2 = INT32U(iAccel2 * 20);

		iResult = smmc_set_profile(m_plnkRingID[0], m_plnkNodeID[0], (int)dVel, iAccel, iAccel, iJerk);
	
		if (iResult != ERR_MOTION_SUCCESS)
			return m_plnkAxis[0]->makeSercosError(iResult);
		
		iResult = smmc_set_profile(m_plnkRingID[0], m_plnkNodeID[1], (int)dVel2, iAccel2, iAccel2, iJerk2);
	
		if (iResult != ERR_MOTION_SUCCESS)
			return m_plnkAxis[1]->makeSercosError(iResult);

		iResult = smmc_spl_line_move2(m_plnkRingID[0], m_plnkNodeID[0], m_plnkNodeID[1], (int)dPos, (int)dPos2);
		if (iResult != ERR_MOTION_SUCCESS)
			return m_plnkAxis[0]->makeSercosError(iResult);
	}
	
	if (bWaitOpt == TRUE)
	{
		/** Spline Line 이동 완료될 때까지 대기 */
		if ((iResult = Wait4Done()) != ERR_MOTION_SUCCESS)
			return iResult;
	}
	
	return ERR_MOTION_SUCCESS;
}