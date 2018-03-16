/* 
 * Axis Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc.("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * IAxis.h : This file defines the Motion interface class of single-axis.
 *				  �� ������ ���� ������ ���� Class�� ���� Interface Class�� �����Ѵ�.
 *
 * @author Jun-young Park(jy21.park@samsung.com )
 * @version $Revision$
 */

#ifndef IAxis_H
#define IAxis_H

#include "DefMotionStruct.h"

#include "ICommon.h"

class MAxisData;

/** This class is defined Motion component interface class of single-axis as a abstract class.
 * �� Class�� ���� ������ ���� Class�� ���� Interface Class�� �����Ѵ�.
 * 
 * ���� �⺻���� �̵� ��ɵ�� �� ���� ��ȸ �� ���� ����� �����Ѵ�.
 * ������ �� �ִ� ���� ������ 1������ �����ϸ�, �� Interface�� ��ü�� ����� �� �ִ� ����
 * �ִ� �� ���� �����ϴ�. ����, �� �࿡ ���� ���� �ٸ� ��ü���� ������ �� ����.
 * 
 * �� Class���� �������� �ʴ� Operation���� IMotionLib Interface�� ����Ͽ� Library Level��
 * Operation�� ���� ����Ͽ��� �Ѵ�.(IAxis�� ���� ��� ���� ��� ����)
 *
 * @author Jun-young Park(jy21.park@samsung.com )
 * @version $Revision$
 *
 * @interface
 */
class IAxis  : public virtual ICommon
{

public:

	/**  ����Ÿ�� ���� �Ѵ�. */
	virtual int	SetData(SSMotionData* pData) = 0;

	/** ���� ����Ÿ�� ��ȸ �Ѵ�. */
	virtual int	GetData(SSMotionData* pData) = 0;

	/**  ���Ͽ��� ����Ÿ�� �ε� �Ѵ�. */
	virtual int LoadParameter() = 0;

	/**  ���Ͽ� ����Ÿ�� �����Ѵ�.  */
	virtual int SaveParameter(MAxisData* pData)  = 0;

	/** �� Parameter(DataManager)�� ��´�. */
	virtual void GetParameter(MAxisData* pData) = 0 ;

	/**
	 * ���� �̸��� Return �Ѵ�.
	 * @return : Axis Name
	 */
	virtual CString GetAxisName() = 0;

	/**
	 * Motion Component�� �ʱ�ȭ�Ѵ�.
	 *
	 * 1. Motion Board ������ �´� Motion Library ��ü�� �����Ѵ�.
	 * 2. �� ���� ������ �����Ѵ�.
	 * 3. �� ������ �����Ѵ�.
	 *
	 * @param	iBoardType		: Motion Board Type(1=MMC Board, 2=MEI board, 3=PPC Board, ...)
	 * @param	saxAxis			: 1�� ���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXES NUMBER(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis) = 0;

	/**
	 * Motion ������ ��� ������ Interlock ���ǿ� �ش�Ǵ� IO Address�� ���ذ��� �����Ѵ�.
	 *
	 * @param	usiIOAddress : ������ Interlock IO Address
	 * @param	bJudgeValue	: Interlock �Ǵ��� ��
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
    virtual void SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;

	/**
	 * �� 1���� ���� Data�� �����Ѵ�.(����ü)
	 *
	 *	(1) �� ���� ������ �־�� �Ѵ�.
	 *	(2) ���� �̹� ���ǰ� ������ �ȵȴ�.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  xx = USED AXIS ID(MULTIAXES)
	 *							  xx = NO EXIST AXIS PARAMETER AREA(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAxisData() = 0;

    /**
	 * �� 1���� ���� Data�� �д´�.(����ü)
	 *
	 * @param   *ax1Data        : ������ ���� ���� Data
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAxisData(SAxis1* ax1Data) = 0;

	virtual int SetAutoCP(int iBoardNo, BOOL* pbAutoSet) = 0;

	virtual int GetAutoCP(int iBoardNo, BOOL* pbAutoSet) = 0;

	/** 
	 * �� 1���� ���� Axis ID�� �д´�.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetAxisID() = 0;

	/** 
	 * �� 1���� ���� Ring ID�� �д´�.
	 *
	 * @return	Error Code		: Ring ID
	 */
	virtual int GetRingID() = 0;

	/** 
	 * �� 1���� ���� Node ID�� �д´�.
	 *
	 * @return	Error Code		: Node ID
	 */
	virtual int GetNodeID() = 0;

	/** 
	 * �� 1���� ���� Home ��ġ(����������ġ)�� �д´�.
	 *
	 * @return  double		 : ���� dHomePosition	 
	 */
	virtual double GetHomeOffset() = 0;

	/** 
	 * �� 1���� ���� Negative Limit ��ġ�� �д´�.
	 *
	 * @return   double     : ���� dNegativeLimit Position	 
	 */
	virtual double GetNegativeSWLimit() = 0;

	/** 
	 * �� 1���� ���� Positive Limit ��ġ�� �д´�.
	 *
	 * @return   double     : ���� dPositiveLimit Position	 
	 */
	virtual double GetPositiveSWLimit() = 0;

	/** 
	 * �� 1���� ���� Slow Moving�ӵ�(Manual Run Velocity)�� �д´�.
	*
	* @return  double  : ���� dSlowMovingVelocity
	*/
	virtual double GetSlowMovingVelocity() = 0;
	
	/** 
	 * �� 1���� ���� Moving�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return  double  : ���� dMovingVelocity	 
	 */
	virtual double GetMovingVelocity() = 0;

	/** 
	 * �� 1���� ���� Moving�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return double  : ���� iMovingAccelerate	 
	 */
	virtual int GetMovingAccelerate() = 0;

	/** 
	 * �� 1���� ���� Coarse�ӵ�, ���ӵ��� �д´�.
	 *
	 * return   double      : ���� dCoarseVelocity	 
	 */
	virtual double GetCoarseVelocity() = 0;

	/** 
	 * �� 1���� ���� Coarse�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return   int    : ���� iCoarseAccelerate	 
	 */
	virtual int GetCoarseAccelerate() = 0;

	/** 
	 * �� 1���� ���� Fine�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return  double     : ���� dFineVelocity	 
	 */
	virtual double GetFineVelocity() = 0;

	/** 
	 * �� 1���� ���� Fine�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return   int    : ���� iFineAccelerate	 
	 */
	virtual int GetFineAccelerate() = 0;

	/** 
	 * �� 1���� ���� Jog Move�� Slow, �ӵ��� �д´�.
	 *
	 * @return   double         : ���� dJogSlow	 
	 */
	virtual double GetJogSlow() = 0;

	/** 
	 * �� 1���� ���� Jog Move�� Fast, �ӵ��� �д´�.
	 *
	 * @return   double     : ���� dJogFast	 
	 */
	virtual double GetJogFast() = 0;

	/** 
	 * �� 1���� ���� Sign�� �д´�.
	 *
	 * @return   BOOL          : ���� bSign	 
	 */
	virtual BOOL GetSign() = 0;

	/** 
	 * �� 1���� ���� Abs Encoder ��� ���θ� �д´�.
	 *
	 * @return   BOOL          : ���� bAbsData
	 */
	virtual BOOL IsAbsEncoder() = 0;

	/** 
	 * �� 1���� ���� �������� ��������� �д´�.
	 *   Limit Sensor ������ ���� �������� �ʱ� ��������� ���� �� �ְ� �Ѵ�.
	 *
	 * @return   BOOL           : ������ bOriginDir(TRUE : +����, FALSE : -����)	 
	 */
	virtual BOOL GetOriginDir() = 0;

	/** 
	 * �� 1���� ���� �������� ����(Fine����)������ �д´�.
	 *   Fine �ӵ� �������� �ʱ� ��������� ���� �� �ְ� �Ѵ�.
	 *
	 * @return   BOOL           : ������ bOriginDir(TRUE : +����, FALSE : -����)	 
	 */
	virtual BOOL GetOriginFineDir() = 0;

	/** 
	 * �� 1���� ���� C�� ��� Type �� �д´�.
	 *
	 * @param   int           : ���� C �� ��� type
	 */
	virtual int GetCPhaseUseType() = 0;

	/** 
	 * �� 1���� ���� Scale�� �д´�.
	 *
	 * @return   double         : ���� dScale	 
	 */
	virtual double GetScale() = 0;

	/** 
	 * �� �̵� �� ���� �ð��� �д´�.
	 *
	 * @return   double          : ������ �̵� ���� �ð�(�ʴ���)	 
	 */
	virtual double GetMoveTime() = 0;

	/** 
	 * �� �̵� �� ����ȭ �ð��� �д´�.
	 *
	 * @return  double          : ������ �̵� �� ����ȭ �ð�(�ʴ���)	 
	 */
	virtual double GetMoveAfterTime() = 0;

	/** 
	 * �� ��ġ ��� ������ �д´�.
	 *
	 * @return   double     : ������ ��ġ ��� ����(mm����)	 
	 */
	virtual double GetTolerance() = 0;

	/** 
	 * �� ��ġ ��� ������ �д´�.
	 *
	 * @return   double    : ������ Offset(mm����)	 
	 */
	virtual double GetPassOffset() = 0;

	/** 
	 * �� �������� �Ϸ� ��� �ð�(��)�� �д´�.
	 *
	 * @return   double          : ������ �������� �Ϸ� ��� �ð�(�� ����)	 
	 */
	virtual double GetOriginWaitTime() = 0;

	/**
	 * ���� �������� �ƴ��� Ȯ���Ѵ�.(�Ѱ��� �࿡ ���� ����)
	 * Ȯ���ϰ��� �ϴ� �࿡ ���� ���� Code�� �о���� �Ѵ�.
	 * 
	 * @return	: TRUE = �������� �Ϸ�
	 *            FALSE = �������� �̿Ϸ�
	 */
	virtual BOOL IsOriginReturn () = 0;

	/**
	 * �������� Flag�� TRUE��..... (�Ѱ��� ��)
	 */
	virtual void SetOriginFlag() = 0;
	

	/**
	 * �� �������� �����ϱ�(�Ѱ��� �࿡ ���� ����)
	 * 
	 * @param   *pbReturn        :(OPTION = NULL) �������� ��� �б�
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ResetOrigin(BOOL* pbReturn = NULL) = 0;

	/**
	 * �� �������� ���� �����ϱ�(������ ��� �࿡ ���� ���� ���� ��� ����)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int StopReturnOrigin(void) = 0;

	/**
	 * ���� ������ǥ�� �д´�.
	 * 
	 * @return   double   : ���� ��ǥ��
	 * @param   bCommandType         : ���� ��ġ ����, FALSE=������ġ(Encode), TRUE=��ǥ��ġ(Command)     
	 */
	virtual double GetCurrentPosition(BOOL bCommandType = FALSE) = 0;

	/**
	 * ���� ������ǥ�� �����Ѵ�.
	 * 
	 * @param   dCurrentPosition     : ���� ��ǥ��
	 * @param   bType                : ���� ��ġ ����, FALSE=������ġ, TRUE=��ǥ��ġ
	 * @return	Error Code		     : 0 = SUCCESS
	 *								  xx = INVALID AXIS ID(MULTIAXES)
	 *								  xx = INVALID POINTER(MULTIAXES)
	 *								  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetCurrentPosition(double dCurrentPosition, BOOL bType = FALSE) = 0;

	/**
	 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
	 * 
	 * @param	bPosOpt              : ���� ��ġ ����, FALSE=������ġ, TRUE=Motion�� ��ǥ��ġ
	 * @param   dTargetPosition      : ���� ��ǥ��	 
	 * @param   *pdPermission        :(OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
	 * @param   *pdDeviation         :(OPTION = NULL) �� ���̰�     
	 * @return  BOOL	             : �񱳰��
	 */
	virtual BOOL ComparePosition(BOOL bPosOpt,
		                          double dTargetPosition,
								  double* pdPermission = NULL, 
								  double* pdDeviation = NULL) = 0;

	/**
	 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
	 * 
	 * @param   dTargetPosition      : ���� ��ǥ��	 
	 * @param   *pdPermission        :(OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
	 * @return  BOOL	             : �񱳰��, ���� ��ǥ + Tolerance ���� ������ True, �ƴϸ� False
	 */
	virtual BOOL IsLessThanTargetPosition(double dTargetPosition,
										   double* pdPermission = NULL) = 0;

	/**
	 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
	 * 
	 * @param   dTargetPosition      : ���� ��ǥ��	 
	 * @param   *pdPermission        :(OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
	 * @return  BOOL	             : �񱳰��, ���� ��ǥ - Tolerance ���� ũ�� True, �ƴϸ� False
	 */
	virtual BOOL IsGreaterThanTargetPosition(double dTargetPosition, 
											  double* pdPermission = NULL) = 0;

	/**
	 * �� �̵�(�Ѱ��� �࿡ ���� �̵�) - �̵� �Ϸ�� �� return
	 * 
	 * @param   dPosition        : �̵��� ��ġ
	 * @param   dVelocity        :(OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param   iDecelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param	iMoveType        :(OPTION=1) �̵� Type,
	 *											0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *											1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 *											4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *											5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Move(double dPosition, 
					  double dVelocity = 0.0, 
					  int iAccelerate = 0, 
					  int iDecelerate = 0,
					  int iMoveType = DEF_MOVE_POSITION) = 0;

	/**
	 * �� �̵�(�Ѱ��� �࿡ ���� �̵�) - �̵� ��� �� �ٷ� return
	 * 
	 * @param   dPosition        : �̵��� ��ġ
	 * @param   dVelocity        :(OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param   iDecelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param	iMoveType        :(OPTION=1) �̵� Type,
	 *											0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *											1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 *											4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *											5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int StartMove(double dPosition, 
						   double dVelocity = 0.0, 
						   int iAccelerate = 0,
						   int iDecelerate = 0, 
						   int iMoveType = DEF_MOVE_POSITION) = 0;

	/**
	 * �� �̵�(�Ѱ��� �࿡ ���� �����ġ �̵�) - �̵� �Ϸ�� �� return
	 * 
	 * @param   dDistance        : �̵��� �Ÿ�
	 * @param   dVelocity        :(OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param   iDecelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param	iMoveType        :(OPTION=3) �̵� Type,
	 *											2=��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	 *											3=S-Curve �ӵ� Profile, ���Ÿ� �̵�
	 *											6=���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	 *											7=���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�
	 * @param	bClearOpt        :(OPTION=FALSE) �̵� ���� �Ŀ� Encoder ���� Clear�ϴ� ���� ��� ����(TRUE:���, FALSE:�̻��)
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int RMove(double dDistance, 
					   double dVelocity = 0.0, 
					   int iAccelerate = 0, 
					   int iDecelerate = 0,
					   int iMoveType = DEF_SMOVE_DISTANCE, 
					   BOOL bClearOpt = TRUE) = 0;

    /**
	 * �� �̵�(�Ѱ��� �࿡ ���� �����ġ �̵�) - �̵� ��� �� �ٷ� return
	 * 
	 * @param   dDistance        : �̵��� �Ÿ�
	 * @param   dVelocity        :(OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param   iDecelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���	
	 * @param	iMoveType        :(OPTION=2) �̵� Type,
	 *											2=��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	 *											3=S-Curve �ӵ� Profile, ���Ÿ� �̵�
	 *											6=���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	 *											7=���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�
	 * @param	bClearOpt        :(OPTION=FALSE) �̵� ���� �Ŀ� Encoder ���� Clear�ϴ� ���� ��� ����(TRUE:���, FALSE:�̻��)
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int StartRMove(double dDistance, 
						    double dVelocity = 0.0, 
							int iAccelerate = 0, 
							int iDecelerate = 0,
							int iMoveType = DEF_SMOVE_DISTANCE, 
							BOOL bClearOpt = TRUE) = 0;

	/**
	 * �� �̵�(�Ѱ��� �࿡ ���� ��� �̵�, ��� ��ġ���� ���� �̵���) 
	 * 
	 * @param   dVelocity        :(OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      :(OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	 * @param   bDir             :(OPTION=TRUE) �̵��� ����, TRUE:(+), FALSE:(-), �����ϸ�(+)�������� �̵�
	 *                             dVelocity�� ���� �־��ָ� bDir�� �����ؼ� ����ϸ� �ȴ�.
	 *                             �� ���� dVelocity�� ��ȣ�� ���� �̵� ������ �����ȴ�.
	 *                             dVelocity�� 0.0�� �־� ������ �ӵ��� ����ϴ� ����
	 *                             bDir��(+/-) ������ ������ �� �ִ�.
	 *                             ����, dVelocity�� ���� ���� ��� bDir�� ������ �ϰ� �Ǹ�
	 *                             ������ dVelocuty, dAccelerate�� bDir�� �ݿ��Ǿ� �̵��� �ϰ� �ȴ�.
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int VMove(double dVelocity = 0.0, int iAccelerate = 0, BOOL bDir = TRUE) = 0;

	/**
	 * �� �̵� �� �ϷḦ Ȯ���Ѵ�.(�Ѱ��� �࿡ ���� �Ϸ� Ȯ��) 
	 * 
	 * @param   bSkipMode        :(OPTION=FALSE) ��ġ Ȯ�� ���, �̵� �� ����ȭ �ð� ���� ��� ����
	 * @param	bUseInterlock    :(OPTION=FALSE) Interlock�� ���� ���� ���� ��� ��� ����(FALSE:������, TRUE:�����)
   	 * @param	bRMove		     :(OPTION=FALSE) RMove�� ��� ��� ����(FALSE:������, TRUE:�����)
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  xx = TIMEOUT MOVE-TIME(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Wait4Done(BOOL bSkipMode = FALSE, BOOL bUseInterlock = FALSE, BOOL bRMove = FALSE) = 0;


	/**
	 * ���� ���� �Ϸ� ���θ� Return �Ѵ�.(����� : 2005.01.11)
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ�� �Ǵ� ���Ҿ� In-Position ���α��� Ȯ���Ѵ�.
	 *
	 * MMC Library : axis_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsAxisDone() = 0;

	/**
	 *	���� �̵� ��(�ӵ� > 0) ������ Return �Ѵ�.
	 *
	 *	MMC Library : in_motion(axis)
	 *
	 *	@return : TRUE  = �� �̵� ��.
	 *            FALSE = �� ���� ����
	 */
	virtual BOOL IsInMotion() = 0;

	/**
	 * ���� ���� �Ϸ� ���θ� Return �Ѵ�.(����� : 2005.01.11)
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ���Ѵ�.  In-Position Ȯ�ξ���
	 *
	 * MMC Library : motion_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsMotionDone() = 0;

	virtual int GetComVelocity() = 0;

	/**
	 * �������� �̵��Ѵ�.
	 * 
	 * @param   iMaxPoint        : �������� �̵����� ����
	 * @param   *pdPosition      : �������� �̵����� ����, iMaxPoint��ŭ ����
	 * @param   *pdVelocity      : �̵� �� �ӵ�, iMaxPoint��ŭ ����(Scale�� ������� �ʴ´�.)
	 * @param   *piAccelerate    : �̵� �� ���ӵ�, iMaxPoint��ŭ ����
	 * @param	bAutoFlag        : Auto CP ����, FALSE=AutoCP ������, TRUE=AutoCP �����
	 * @param	bWaitOpt         :(OPTION=TRUE)�� �̵� �Ϸ� ��� ����, FALSE=��� ����, TRUE=�����
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  xx = NO EXIST AXIS ID TO MOVE(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int MoveSplineLine(int iMaxPoint, 
								double* pdPosition, 
								double* pdVelocity, 
								int* piAccelerate,
								BOOL bAutoFlag, 
								BOOL bWaitOpt = TRUE) = 0;

	/**
	 * Jog Slow�� ���� �̵��Ѵ�.
	 * 
	 * @param   bDir             : �̵��� ����, TRUE:(+), FALSE:(-)
	 * @param   dSlow           :(OPTION = 0.0) �̵��� �Ÿ�, 0.0 = ������ Slow�Ÿ� ���
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int JogMoveSlow(BOOL bDir, double dSlow = 0.0) = 0;

	/**
	 * Jog Fast�� ���� �̵��Ѵ�.
	 * 
	 * @param   bDir             : �̵��� ����, TRUE:(+), FALSE:(-)
	 * @param   dFast        :(OPTION = 0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int JogMoveFast(BOOL bDir, double dFast = 0.0) = 0;

	virtual int CheckAxisStateForJog(BOOL bDir) = 0;

	/**
	 * ���� �����Ѵ�.(�Ѱ��� �࿡ ���� ����)
	 * 
	 * @param   *pbStatus      :(OPTION = NULL) ���� Stop ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Stop(BOOL* pbStatus = NULL) = 0;

	/**
	 * ���� ����̵��� ���� �����Ѵ�.(�Ѱ� ���� ����̵��� ���� ����)
	 * 
	 * @param   *pbState         :(OPTION = NULL) �� ���� VStop ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int VStop(BOOL* pbStatus = NULL) = 0;

	/**
	 * ���� ��������Ѵ�.(�Ѱ��� �࿡ ���� �������)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) �� ���� EStop ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int EStop(BOOL* pbStatus = NULL) = 0;

	/**
	 * ���� Servo�� On �Ѵ�.(�Ѱ��� �࿡ ���� Servo On ����)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) �� ���� Servo ON ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ServoOn (BOOL* pbStatus = NULL, BOOL bIsCheck = FALSE) = 0;

	/**
	 * ���� Servo�� Off �Ѵ�.(�Ѱ��� �࿡ ���� Servo Off ����)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) �� ���� Servo OFF ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ServoOff(BOOL* pbStatus = NULL,  BOOL bIsCheck = FALSE) = 0;

	/**
	 * ���� Home Sensor ���¸� �д´�.(�Ѱ��� �࿡ ���� �����б�)
	 * 
	 * @return   BOOL        : �� ���� Home Sensor ����     
	 */
	virtual BOOL IsOnHomeSensor() = 0;


	/**
	 * ���� Positive Sensor ���¸� �д´�.(�Ѱ��� �࿡ ���� �����б�)
	 * 
	 * @return   BOOL        : �� ���� Positive Sensor ����     
	 */
	virtual BOOL IsOnPositiveSensor() = 0;


	/**
	 * ���� Negative Sensor ���¸� �д´�.(�Ѱ��� ���� �����б�)
	 * 
	 * @return   BOOL        : �� ���� Negative Sensor ����     
	 */
	virtual BOOL IsOnNegativeSensor() = 0;

	virtual int SetHomeSensorEvent(int iLimit) = 0;

	virtual int SetHomeSensorLevel(BOOL bLevel) = 0;

	virtual int SetPositiveSensorEvent(int iLimit) = 0;

	virtual int SetNegativeSensorEvent(int iLimit) = 0;

	/**
	 * ���� ����(Source)�� �д´�. 
	 * 
	 * @return   int       : �� �ϳ��� ���� ����(Source)     
	 */
	virtual int GetAxisSource(void) { return 0; }; //170713 JSH

	virtual int GetAxisSource(int *piReturn) { return 0; }; //170713 JSH

	/**
	 * ���� ����(State)�� �д´�. 
	 * 
	 * @retunr  int         : �� �ϳ��� ���� ����(State)     
	 */
	virtual int GetAxisState(int *piReturn) { return 0; };
	virtual int GetAxisState(UINT32 *piReturn) { return 0; };

	/**
	 * ���� AMP Enable ���¸� �д´�. 
	 * 
	 * @param   BOOL        : �࿡ ���� AMP����(AMP Enable : TRUE, �׿� : FALSE)     
	 */
	virtual BOOL IsAmpEnable() = 0;

	/**
	 * ���� AMP Enable/Disable ���·� �����.(����� : 2005.01.11)
	 * 
	 * @param   bEnable          : AMP Enable ���·� ���� : TRUE
	 *							   AMP Disable ���·� ���� : FALSE
	 * @param   bLinearAxis        : (OPTION = FALSE) ���� Linear Type�̸� TRUE, �׿� FALSE
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpEnable(BOOL bEnable) = 0;

	/**
	 * ���� AMP Fault ���¸� �д´�. 
	 * 
	 * @return   BOOL         : �࿡ ���� AMP Fault����(AMP Fault:TRUE, Fault�ƴϸ�:FALSE)
	 */
	virtual BOOL IsAmpFault() = 0;

	/**
	 * ���� AMP Fault ���¸� Reset �Ѵ�.(����� : 2005.01.11)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ResetAmpFault(BOOL bIsCheck = FALSE) = 0;

	/**
	 * ���� AMP Fault ���¸� Enable �Ѵ�.(����� ���� : 2005.01.11)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpFaultEnable() = 0;

	virtual int GetIndexRequired(BOOL* pbIndexReq) = 0;

	virtual int SetIndexRequired(BOOL bIndexReq) = 0;

	/**
	 * ���� ���¸� �ʱ�ȭ �Ѵ�.(�Ѱ��� �࿡ ���� �ʱ�ȭ)
	 *  Clear Status & Clear Frames
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) �� ���� �ʱ�ȭ ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ClearAxis(BOOL* pbStatus = NULL) = 0;

	/** 
	 * �������� �켱 ������ Return �Ѵ�.
	 *
	 * @return	�켱 ����		: 0~63
	 */
	virtual int GetOriginPriority() = 0;

	/** 
	 * �������� ���� ���� �Լ�(�ѹ� ȣ�⿡ �� Step�� ����)
	 * ���� ���� Step�� �Ϸ� Step�� �� �� ���� ��� ȣ�� �ؾ���
	 *
	 * @param   iOriginMoveType :	DEF_ORIGIN_RETURN_NONE_MOVE (0) -> Motion ���� ���� ���� ��ġ�� �������� �����ϴ� ���. ( == FALSE)
	 *								DEF_ORIGIN_RETURN_ALL_MOVE	(1) -> ���� ���� Offset �̵� ������ ������ ��� ���� ���� ���� Motion ������ �����ϴ� ���. ( == TRUE)
	 *								DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE	(2) -> ���� ���� Offset �̵� ������ ������ ���� ���� ���� Motion ������ �����ϴ� ���.
 
     * @return	Origin Step		: 1000 = ORIGIN RETURN SUCCESS
	 *							  990 = ERROR
	 *							  xx = ���� ���� ��...
	 */
	virtual int OriginReturn(int iOriginMoveType) = 0;

	/**
	 * �������� Step�� �����Ѵ�.(�Ѱ��� ��)
	 * 
	 * @param   iStep            : ������(0:����, 999:����, 1000:�Ϸ�, �׿�:������)
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID ORIGIN STEP(<0)(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetOriginStep(int iStep) = 0;

	/**
	 * �������� Step�� �д´�.(�Ѱ��� ��)
	 * 
	 * @return	Origin Setp
	 */
	virtual int GetOriginStep() = 0;

	/**
	 * �������� Error�� �ʱ�ȭ�Ѵ�.(�Ѱ��� ��)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ClearOriginError() = 0;

	/**
	 * �������� Error�� �д´�.(�Ѱ��� ��)
	 * 
 	 * @param   *piError         : �߻��� ���� Code
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetOriginError(int* piError) = 0;
	
	/**
	 * Home Sensor�� ������ ��ȯ �Ѵ�.
	 *  	 
     * @return	ESercosRoleOfHome		HOME_N_NONE : Home Sensor ����
										HOME_N_POSITIVE_LIMIT : Home, Positive Sensor ����
										HOME_N_NEGATIVE_LIMIT : Home, Negative Sensor ����
	 */
	virtual ERoleOfHome GetRoleOfHomeSensor() = 0;

	virtual int checkAxisState(int iAxisID) = 0;

	virtual BOOL IsEStopEvent() = 0;

	virtual int ChangePhase(int Phase) = 0;

	virtual int NodeClear() = 0;

	virtual void SetAxisPosReset() = 0;
	
	/**
	 * Jog �̵��� ���� ��Ų��.
	 * 	 
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  �� �� =  Error Code
	*/
	virtual int JogStop() = 0;

	/**
	* Stop(), EStop() �Լ� ������� ���ӵ��� ����.
	* �������� Run ���ӵ� ���� 1/5 �� ����.
	*
	* @return Error Code		: 0 = SUCCESS
	*							  xx = INVALID AXIS ID(MOTIONLIB)
	*							  �� �� = Ÿ �Լ� Return Error
	*/
	virtual int SetEStopRate() = 0;

	virtual int HomingStop() = 0;

	virtual	BOOL IsOriginComplete() = 0;
	virtual void ResetOriginComplete() = 0;
	virtual double DistanceToCurrentToHome() = 0;

	virtual int SetHomingSwitchSource(int iSourceAxisNodeID) = 0;

	// Motor�� Scale�� ������ �� �ִ�...���� MMC2�� ��밡��.20080602
	virtual int SetMotorScale(double dScale) = 0;
};

#endif //IAxis_H

// End of File IAxis.h
