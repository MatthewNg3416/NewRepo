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

	// Limit, Statble Time�� �� Axis �� ���� ū ������ ����.
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
 *				���� object�� �Ҹ��Ѵ�.
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

	/** �̵� �� �������� ���� Ȯ��, �� ���� ���� */
	for (int i = 0; i < m_iAxisNum; i++)
	{
		/** �̵� �� �������� ���� Ȯ�� */
		if (!m_plnkAxis[i]->IsOriginReturn())
		{
			iResult = generateErrorCode(107906);
			goto RETURN;
		}

		/** �� ���� ���� */
		iResult = m_plnkAxis[i]->checkAxisState(m_pAxisID[i]);
		if (iResult)
			goto RETURN;

		/** SW Limit Check */
		if (0 == i) // �����ุ S/W Limit üũ.
		{
			iResult = m_plnkAxis[0]->checkSWLimit(dPosition[0]);
			if (iResult)
				goto RETURN;
		}

		// Profile ����.

		dPos[i] = m_pScale[i] * dPosition[i];
		iPos[i]	= (int)dPos[i];

		/** �̵� �ӵ� ó�� */
		if (dVelocity == NULL)
		{
			if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
				dVel[i] = m_pScale[i] * m_pMoveVelocity[i];
			else
				dVel[i] = m_pScale[i] * m_pMoveSlowVelocity[i];
		}
		else
			dVel[i] =  m_pScale[i] * dVelocity[i];
		
		/** �̵� ���ӵ� ó�� */
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
		
		/** �̵� ���ӵ� ó�� */
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
			/* 20090203 jdy : jerk �� ���� ���� ���� ���� �ݿ� �ȵ�. RAK ���� Jerk ������ ���� API �߰� �����ϱ�� ����.
		                  20090119 ���뱹K �� ��ȭ �Ϸ�. - RAK ���� �� API ���� ���� �شٰ� ��.
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
			iJerk[i] = INT32U(0);	// Curve ���¸� �����Ѵ�. (��ٸ���)          
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
		/** �̵� �� �������� ���� Ȯ�� */
		if (!m_plnkAxis[i]->IsOriginReturn())
		{
			iResult = generateErrorCode(107906);
			goto RETURN;
		}
		
		/** �� ���� ���� */
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

		/** �̵� �ӵ� ó�� */
		if (dVelocity == NULL)
		{
			if (m_plnkAxis[i]->GetVelocityMode() == VELOCITY_MODE_NORMAL)
				dVel[i] = m_pScale[i] * m_pMoveVelocity[i];
			else
				dVel[i] = m_pScale[i] * m_pMoveSlowVelocity[i];
		}
		else
			dVel[i] =  m_pScale[i] * dVelocity[i];
		
		/** �̵� ���ӵ� ó�� */
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
		
		/** �̵� ���ӵ� ó�� */
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
			/* 20090203 jdy : jerk �� ���� ���� ���� ���� �ݿ� �ȵ�. RAK ���� Jerk ������ ���� API �߰� �����ϱ�� ����.
		                  20090119 ���뱹K �� ��ȭ �Ϸ�. - RAK ���� �� API ���� ���� �شٰ� ��.
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
			iJerk[i] = INT32U(0);			// Curve ���¸� �����Ѵ�. (��ٸ���)          
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
* �� �̵� �� �ϷḦ Ȯ���Ѵ�. (Polling������ Ȯ��)  
* "DEF_DONE_EVENT_TYPE"�� Define�Ǹ� Event��Ŀ����� Done�� ����Ѵ�.
*
* @param   bSkipStableTime  : (OPTION=FALSE) ����ȭ�ð��� ������� �ʴ´�.
* @param   eDoneType		:
*                             - DEF_IN_POSITION_FINE_DONE : InPosition ��ȣ (���� ��������) ���� ��ٸ���.
*							  - DEF_IN_POSITION_COARSE_DONE : CoarseDone ��ȣ (����ڰ� ���� �߰��� ��������)���� ��ٸ��� Return�Ѵ�.
*							  - DEF_AT_TARGET_DONE : AtTarget ��ȣ (Command����� ��������) ���� ��ٸ��� Return�Ѵ�.
* @param	bUseInterlock    : (OPTION=FALSE) Interlock�� ���� ���� ���� ��� ��� ���� (FALSE:������, TRUE:�����)
* @return	Error Code		 : 0 = SUCCESS
*							  �� �� = Ÿ �Լ� Return Error
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
 * Jog Slow�� ���� �̵��Ѵ�.
 * 
 * @param   bDir             : �̵��� ����, TRUE:(+), FALSE:(-)
 * @param   dSlow           : (OPTION = 0.0) �̵��� �Ÿ�, 0.0 = ������ Slow�Ÿ� ���
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** (+) ���� */
	if (bDir == TRUE)
		iDir = 1;
	/** (-) ���� */
	else
		iDir = -1;

	for (int i = 0; i < m_iAxisNum; i++)
	{	
		// �̵� �ӵ� ó��.
		if (0.0 == dSlow)
			dVel[i] = m_pScale[i] * m_plnkAxis[i]->GetJogSlow();
		else 
			dVel[i] = m_pScale[i] * dSlow;
		
		// �̵� ������ ó��.
		dAccel[i] = dVel[i] / ((double)m_pAccelerate[i]/1000);
		dDecel[i] = dAccel[i];
		
		// S-Curve ���� ����.
		iJerk[i] = INT32U(0);			// Curve ���¸� �����Ѵ�. (��ٸ���)          
		
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
 * Jog Fast�� ���� �̵��Ѵ�.
 * 
 * @param   bDir             : �̵��� ����, TRUE:(+), FALSE:(-)
 * @param   dFast        : (OPTION = 0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
 * @return	Error Code		 : 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MULTIAXES)
 *							  xx = INVALID POINTER (MULTIAXES)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** (+) ���� */
	if (bDir == TRUE)
		iDir = 1;
	/** (-) ���� */
	else
		iDir = -1;

	for (int i = 0; i < m_iAxisNum; i++)
	{	
		// �̵� �ӵ� ó��.
		if (0.0 == dFast)
			dVel[i] = m_pScale[i] * m_plnkAxis[i]->GetJogFast();
		else 
			dVel[i] = m_pScale[i] * dFast;
		
		// �̵� ������ ó��.
		dAccel[i]	= dVel[i] / ((double)m_pAccelerate[i]/1000);
		dDecel[i]	= dAccel[i];
		
		// S-Curve ���� ����.
		iJerk[i]	= INT32U(0);			// Curve ���¸� �����Ѵ�. (��ٸ���)          
		
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
 * Jog �̵��� ���� ��Ų��.
 * 	 
 * @return	Error Code		 : 0 = SUCCESS
 *							  �� �� =  Error Code
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

	// ���ุ ���߸� �׿� ��� ���� ����
	iErr = smmc_set_stop(m_plnkRingID[0], m_plnkNodeID[0]);
	if (iErr != ERR_MOTION_SUCCESS)
		m_plnkAxis[0]->makeSercosError(iErr);

	return iResult;
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
		/** �̵� �� �������� ���� Ȯ�� */
		if (!m_plnkAxis[i]->IsOriginReturn())
			return generateErrorCode(107906);
		
		/** �� ���� ���� */
		if ((iResult = m_plnkAxis[i]->checkAxisState(m_pAxisID[i])) != ERR_MOTION_SUCCESS)
			return iResult;
	}

	/** SW Limit Check */ // �����ุ SW üũ ����...
	if ((iResult = m_plnkAxis[0]->checkSWLimit(pdPosition[0])) != ERR_MOTION_SUCCESS)
		return iResult;

	if ((iResult = m_plnkAxis[1]->checkSWLimit(pdPosition2[0])) != ERR_MOTION_SUCCESS)
		return iResult;

	// �̵� Point�� ���� 
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
		/** Spline Line �̵� �Ϸ�� ������ ��� */
		if ((iResult = Wait4Done()) != ERR_MOTION_SUCCESS)
			return iResult;
	}
	
	return ERR_MOTION_SUCCESS;
}