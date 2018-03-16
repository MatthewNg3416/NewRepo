
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

#ifndef IRSNMCMULTIAXIS_H
#define IRSNMCMULTIAXIS_H

#include "MRSNMCAxis.h"	

	
/**
 * MSercosMultiAxis.h : MPI�� �̿��� �������� Class�� InterFace Class
 */

class IRSNMCMultiAxis :public virtual ICommon	//**1222
{


public:		


	/**
	 * �� �̵�  - �̵� �Ϸ�� �� return
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
		double* dDecelerate = NULL, int bIsTrapMove = FALSE,  int iMoveType = DEF_MOVE_POSITION) = 0;

	/**
	 * �� �̵�  - �̵� ��� �� �ٷ� return
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
	virtual ErrorInfo StartMove (double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE) = 0;

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
	virtual ErrorInfo Wait4Done (BOOL bSkipMode = FALSE, int iMoveType = DEF_MOVE_POSITION, BOOL bUseInterlock = FALSE) = 0 ;

	/**
	 * �� �̵�  - �̵� �Ϸ�� �� return
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
		double* dDecelerate = NULL, int bIsTrapMove = FALSE,  int iMoveType = DEF_MOVE_POSITION) = 0;

	/**
	 * �� �̵�  - �̵� ��� �� �ٷ� return
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
	virtual ErrorInfo StartRMove (double * dPosition, double * dVelocity = NULL, double* dAccelerate = NULL,
		double* dDecelerate = NULL, int bIsTrapMove = FALSE) = 0;

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
	virtual ErrorInfo JogMoveSlow (BOOL bDir, double dSlow = 0.0) = 0;

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
	virtual ErrorInfo JogMoveFast (BOOL bDir, double dFast = 0.0) = 0;

	/**
	 * Jog �̵��� ���� ��Ų��.
	 * 	 
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  �� �� =  Error Code
	 */
	virtual ErrorInfo JogStop() = 0;

	virtual ErrorInfo Stop() = 0;

	virtual ErrorInfo GroupDisable() = 0;

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
	virtual ErrorInfo MoveSplineLine2 (int iMaxPoint, double *pdPosition, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE) = 0;
};

#endif // !defined(MSercosMultiAxis_H)
