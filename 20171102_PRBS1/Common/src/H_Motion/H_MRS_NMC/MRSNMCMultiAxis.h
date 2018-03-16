/* 
 * Motion Component
 *
 * Copyright 2006 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef MRSNMCMultiAxis_H
#define MRSNMCMultiAxis_H

#include "IRSNMCMultiAxis.h"
#include "NMC_Motion.h"		

/**
 * MRSNMCMultiAxis.h : MPI�� �̿��� �������� Class 
 *
 * Single Axis�� Handle�� �̿��Ͽ� MultiMotion�� �����Ѵ�..
 *
 */

class MRSNMCMultiAxis :public virtual IRSNMCMultiAxis, public virtual ICommon 	//**1222
{
protected:
	
	int			m_iAxisNum;
	MRSNMC	 ** m_plnkAxis;
	INT32S*		m_plnkRingID;
	INT32S*		m_plnkNodeID;
	INT32S		m_iGroupID;

	double*		m_pMoveVelocity;
	double*		m_pMoveSlowVelocity;
	int*		m_pAccelerate;
	double*		m_pScale;
	int*		m_pAxisID;

	double		m_dStabilityTime;
	double		m_dLimitTime;

	/** Motion �̵� �ð� Timer */
	MTickTimer	m_ttMovingTimer;

public:		
	/**
	 * Constructor : Clear data (default value), initialize inner object
	 *				 Data�� �ʱ�ȭ�ϰ�, ���� ��ü���� �ʱ�ȭ�Ѵ�.
	 * @stereotype constructor
	 */
	MRSNMCMultiAxis();    

	/* Constructor 
	 * @param   pAxis  : �� ������ IMPIAxis��ü �迭�� �ּҰ�.
	 * @param   iAxisNum  : �� ������ IMPIAxis��ü �迭�� ũ��
	            MS IMPIAxis ù��° ��ü�� MS�� �ڵ����εǰ�, �ڵ���ȯ�ȴ�.
				�׷��� ������ SuperVisor�� ���ν�ų��� ���ΰ� ������� �׻� ����Ͽ��� �Ѵ�.
	 */
	MRSNMCMultiAxis(SCommonAttribute commonData, IAxis** pAxis, int iAxisNum, int iGroupID);

	/**
	 * Destructor : Delete inner opbject
	 *				���� ��ü�� �Ҹ��Ѵ�.
	 * @stereotype destructor
	 */
	virtual ~MRSNMCMultiAxis();

	/**
	 * �� �̵� (�Ѱ��� �࿡ ���� �̵�) - �̵� �Ϸ�� �� return
	 * 
	 * @param   dPosition        : �̵��� ��ġ
	 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param	bIsTrapMove      : TRUE  - TRAPERZOIDAL
								   FALSE - S-Curve						
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo Move(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE,  int iMoveType = DEF_MOVE_POSITION);


	/**
	 * �� �̵� (�Ѱ��� �࿡ ���� �̵�) - �̵� ��� �� �ٷ� return
	 * 
	 * @param   dPosition        : �̵��� ��ġ
	 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param	bIsTrapMove      : TRUE  - TRAPERZOIDAL
								   FALSE - S-Curve						
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo StartMove(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE);

	/**
	 * �� �̵� �� �ϷḦ Ȯ���Ѵ�. (�Ѱ��� �࿡ ���� �Ϸ� Ȯ��) 
	 * 
	 * @param   bSkipMode        : (OPTION=FALSE) ��ġ Ȯ�� ���, �̵� �� ����ȭ �ð� ���� ��� ����
	 * @param	bUseInterlock    : (OPTION=FALSE) Interlock�� ���� ���� ���� ��� ��� ���� (FALSE:������, TRUE:�����)
   	 * @param	bRMove		     : (OPTION=FALSE) RMove�� ��� ��� ���� (FALSE:������, TRUE:�����)
	 * @param   eDoneType		 :  
	 *                            - DEF_IN_POSITION_FINE_DONE : InPosition ��ȣ (���� ��������) ���� ��ٸ���.
	 *							  - DEF_IN_POSITION_COARSE_DONE : CoarseDone ��ȣ (����ڰ� ���� �߰��� ��������)���� ��ٸ��� Return�Ѵ�.
	 *							  - DEF_AT_TARGET_DONE : AtTarget ��ȣ (Command����� ��������) ���� ��ٸ��� Return�Ѵ�.
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo Wait4Done(BOOL bSkipMode = FALSE, int iMoveType = DEF_MOVE_POSITION, BOOL bUseInterlock = FALSE) ;


	/**
	 * �� �̵� (�Ѱ��� �࿡ ���� �̵�) - �̵� �Ϸ�� �� return
	 * 
	 * @param   dPosition        : �̵��� ��ġ
	 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param	bIsTrapMove      : TRUE  - TRAPERZOIDAL
								   FALSE - S-Curve						
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo RMove(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE,  int iMoveType = DEF_MOVE_POSITION);


	/**
	 * �� �̵� (�Ѱ��� �࿡ ���� �̵�) - �̵� ��� �� �ٷ� return
	 * 
	 * @param   dPosition        : �̵��� ��ġ
	 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param	bIsTrapMove      : TRUE  - TRAPERZOIDAL
								   FALSE - S-Curve						
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo StartRMove(double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE);

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
	virtual ErrorInfo JogMoveSlow(BOOL bDir, double dSlow = 0.0);

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
	virtual ErrorInfo JogMoveFast(BOOL bDir, double dFast = 0.0);

	/**
	 * Jog �̵��� ���� ��Ų��.
	 * 	 
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  �� �� =  Error Code
	 */
	virtual ErrorInfo JogStop();

	virtual ErrorInfo Stop();

	virtual ErrorInfo GroupDisable();

	//170709_KDH Group �߰�
//	virtual int GroupEnable();
	//_________________

	/**
	 * �������� �̵��Ѵ�.
	 * 
	 * @param   iMaxPoint        : �������� �̵����� ����
	 * @param   *pdPosition      : �������� �̵����� ����, iMaxPoint��ŭ ����
	 * @param   *pdVelocity      : �̵� �� �ӵ�, iMaxPoint��ŭ ���� (Scale�� ������� �ʴ´�.)
	 * @param   *piAccelerate    : �̵� �� ���ӵ�, iMaxPoint��ŭ ����
	 * @param	bAutoFlag        : Auto CP ����, FALSE=AutoCP ������, TRUE=AutoCP �����
	 * @param	bWaitOpt         : (OPTION=TRUE)�� �̵� �Ϸ� ��� ����, FALSE=��� ����, TRUE=�����
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = NO EXIST AXIS ID TO MOVE (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual ErrorInfo MoveSplineLine2(int iMaxPoint, double *pdPosition, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE);

	ErrorInfo MakeRS_NMC_Error(MC_STATUS mcStatus);
	// �ݿø� Function
	double motionRoundValue(double dVal);
	ErrorInfo ErrorMessage(int iCode, char *pcDst);
};

#endif // !defined(MRSNMCMultiAxis_H)
