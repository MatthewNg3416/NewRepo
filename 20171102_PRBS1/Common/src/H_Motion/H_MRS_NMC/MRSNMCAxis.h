/* 
 * Axis Component
 *
 * Copyright 2006 by Mechatronics & Manufacturing Technology Center, 
 * Samsung Electronics, Inc., All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc.("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MRSNMC.h : This file defines the class of single-axis and multi-axis.
 *				  �� ������ ���� ������ ���� Class�� �����Ѵ�.
 *
 * author Ji-Sung Kim(mickeykj.kim@samsung.com)
 * @version $Revision$
 */

#ifndef MRSNMC_H
#define MRSNMC_H

#include "IAxis.h"							// IAxis Interface Class Header File
#include "MTickTimer.h"						// MTickTimer Class Header File
#include "MAxisData.h"

#include "SMMCDEF.h"

class IIO;
class ISercosSetup;
class IMotionLibNMC;
class MRS_NMCSetup;

/**
 * This class is defined class of single-axis.
 * �� Class�� ���� �� ����(Stage) ������ ���� Class�� �����Ѵ�.
 * 
 * �� Class�� ��ü�� ���� ���� ������ �� ������ŭ�� �� ���� ������ �⺻���� ���´�.
 * �� Class�� �⺻ ����� IAxis Interface���� �����ϴ� Operation�� ���� ��ü �����̸�,
 * ��� Motion Library API �Լ����� ���������� �ʴ´�.
 * ���⼭ �����ϴ� ��ɵ��� ���� ��������������� ����ϴ� �Լ���� ���� Unit���� ���� ��
 * ������ ����ϴ� ������ �Լ����� �����ϸ�, Board ������ Unit�� �ٸ� ���� ��� ����ϴ� 
 * ��ɵ� �����Ѵ�.
 *
 * author Ji-Sung Kim(mickeykj.kim@samsung.com)
 * @version $Revision$
 */
class MRSNMC : public IAxis
{

private:	// attribute

    /** 1�� ���� */
	SAxis1				m_SaxAxis;

	/** IO Component */
	IIO*				m_plnkIO;
	ISercosSetup*		m_plnkSercosSetup;

	/** Interlock IO Address */
	unsigned short		m_usiInterlockIOAddr;

	/** Interlock IO Check Value */
	BOOL				m_bInterlockJudge;

	MAxisData			m_AxisParameter;

	CString				m_strFileName;

    /** �������� ���� ����(TRUE:������������) */
	BOOL				m_bOriginFlag;
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	BOOL				m_bInitOrigin;
	// End.
	/**
     * Motion Library ��ü
     * @link aggregation
     * @supplierCardinality 1
     */
    IMotionLibNMC*			m_pMotionLib;

	// Doolin kwangilshin 2017. 08. 27.
	//
	MRS_NMCSetup			*m_pRS_NMCSetup;
	// End.

	/** �������� Step */
	int					m_iOriginStep;
	int					m_iOriginPrevStep;

	/** �������� Error Code */
	int					m_iOriginError;

	/** �������� �̵� �ð� Timer */
	MTickTimer			m_ttOriginTimer;

	/** Motion �̵� �ð� Timer */
	MTickTimer			m_ttMovingTimer;

	/** �������� thread Handle */
	HANDLE				m_hOriginThread;	
	
	double				m_dCurrentAxisPos;

	double				m_dTargetAxisPos;
	
	/** Simulation ���۽ÿ� Motion ���� �ð� Check Timer*/
	MTickTimer			m_MotionTimer;

	/** Simulation ���� �ÿ� R Motion �̵��Ÿ� */
	double				m_dRMoveDistance;

	/** Simulation ���۽ÿ� R Motion �ӵ� */
	double				m_dVelocity;

	/** Simulation ���۽ÿ� R Motion ���ӵ� */
	int					m_iAccelerate;

	/** Simulation ���۽ÿ� R Motion ���ӵ� */
	int					m_iDecelerate;

	/** RMove ���� �ƴ��� Ȯ�� */
	BOOL				m_bRMoveStart;

	/** ������ġ�������� Ȩ���� ������ �Ÿ�. */
	double				m_dToHomeDistance;

	/** ErrorSting */
//	char	m_cpErrString[ERROR_STRING_LENGTH];


public:		// Constructor & Destructor

	/**
	 * Constructor : Clear data(default value), initialize inner object
	 *				 Data�� �ʱ�ȭ�ϰ�, ���� ��ü���� �ʱ�ȭ�Ѵ�.
	 * @stereotype constructor
	 */
	MRSNMC();    

	/**
	 * Constructor : Set data, set inner object
	 *				 ���޹��� �μ��� Data�� �����ϰ�, ���� ��ü�� �����Ѵ�.
	 *
	 * @param	iObjectID		: Object ID
	 * @param	iBoardType		: Motion Board Type(1=MMC Board, 2=MEI board, 3=PPC Board, ...)
	 * @param	saxAxis		    : 1�� ���� ����
	 * @param	iErrorBase		: Error Code Start Number
	 * @param	strFullFileName	: Log File Path & File Name
	 * @param	ucLevel			: Log Level(None, Normal, Warning, Error)
	 * @param	iDays			: Log ���� �� ��
	 * @stereotype constructor
	 */
	MRSNMC(SCommonAttribute commonData, SSMotionData datComponent, IIO* pIO, ISercosSetup* pSercosSetup);    

	/**
	 * Destructor : Delete inner opbject
	 *				���� ��ü�� �Ҹ��Ѵ�.
	 * @stereotype destructor
	 */
	~MRSNMC();

public:		// interface operation

	/**
	 * NMC Library�� �����ϴ� ���� �ڵ带 System Error���·� �����
	 *
	 * @param	iError : Error �ڵ带 �Է� 
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	int makeSercosError(int iError);

	/**
	 * ����Ÿ�� ���� �Ѵ�. 
	 *
	 * @param	*pData : Data Address
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	virtual int	SetData(SSMotionData *pData);

	/**
	 * ���� ����Ÿ�� ��ȸ�Ѵ�. 
	 *
	 * @param	*pData : Data Address
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	virtual int	GetData(SSMotionData *pData);

	/**  ���Ͽ��� ����Ÿ�� �ε� �Ѵ�. */
	virtual int LoadParameter();

	/**  ���Ͽ� ����Ÿ�� �����Ѵ�.  */
	virtual int SaveParameter(MAxisData *pData);

	/** �� Parameter(DataManager)�� ��´�. */
	void GetParameter(MAxisData* pData);

	/**
	 * ���� �̸��� Return �Ѵ�.
	 * @return : Axis Name
	 */
	CString GetAxisName();

	/**
	 * Single-axis Component�� �ʱ�ȭ�Ѵ�.
	 *
	 * 1. Motion Board ������ �´� Motion Library ��ü�� �����Ѵ�.
	 * 2. �� ���� ������ �����Ѵ�.
	 * 3. �� ������ �����Ѵ�.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXES NUMBER(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis);


	/**
	 * Motion ������ ��� ������ Interlock ���ǿ� �ش�Ǵ� IO Address�� ���ذ��� �����Ѵ�.
	 *
	 * @param	usiIOAddress : ������ Interlock IO Address
	 * @param	bJudgeValue	: Interlock �Ǵ��� ��
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
    virtual void SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue);


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
	virtual int SetAxisData();


    /**
	 * �� 1���� ���� Data�� �д´�.(����ü)
	 *
	 * @param   *ax1Data        : ������ ���� ���� Data
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetAxisData(SAxis1 *ax1Data);


	/** 
	* ���� ����� Run Mode�� �д´�.
	*
	* @return	Error Code		: Axis ID
	*/
	virtual EVelocityMode GetVelocityMode();

	/** 
	 * �� 1���� ���� Axis ID�� �д´�.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetAxisID();

	/** 
	 * �� 1���� ���� Axis ID�� �д´�.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetRingID();

	/** 
	 * �� 1���� ���� Axis ID�� �д´�.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetNodeID();

	/** 
	 * �� 1���� ���� Home ��ġ(����������ġ)�� �д´�.
	 *
	 * @return  double		 : ���� dHomePosition	 
	 */
	virtual double GetHomeOffset();

	/** 
	 * �� 1���� ���� Negative Limit ��ġ�� �д´�.
	 *
	 * @return   double     : ���� dNegativeLimit Position
	 */
	virtual double GetNegativeSWLimit();

	/** 
	 * �� 1���� ���� Positive Limit ��ġ�� �д´�.
	 *
	 * @return   double     : ���� dPositiveLimit Position	 
	 */
	virtual double GetPositiveSWLimit();

	/** 
	 * �� 1���� ���� Slow Moving�ӵ�(Manual Run Velocity)�� �д´�.
	 *
	* @return  double  : ���� dSlowMovingVelocity
	*/
	virtual double GetSlowMovingVelocity();
	
	/** 
	 * �� 1���� ���� Moving�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return  double  : ���� dMovingVelocity	 
	 */
	virtual double GetMovingVelocity();

	/** 
	 * �� 1���� ���� Moving�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return double  : ���� iMovingAccelerate	 
	 */
	virtual int GetMovingAccelerate();

	/** 
	 * �� 1���� ���� Coarse�ӵ�, ���ӵ��� �д´�.
	 *
	 * return   double      : ���� dCoarseVelocity	 
	 */
	virtual double GetCoarseVelocity();
	/** 
	 * �� 1���� ���� Coarse�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return   int    : ���� iCoarseAccelerate	 
	 */
	virtual int GetCoarseAccelerate();

	/** 
	 * �� 1���� ���� Fine�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return  double     : ���� dFineVelocity	 
	 */
	virtual double GetFineVelocity();

	/** 
	 * �� 1���� ���� Fine�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return   double    : ���� iFineAccelerate	 
	 */
	virtual int GetFineAccelerate();	

	/** 
	 * �� 1���� ���� Jog Move�� Slow, �ӵ��� �д´�.
	 *
	 * @return   double         : ���� dJogSlow	 
	 */
	virtual double GetJogSlow();
	
	/** 
	 * �� 1���� ���� Jog Move�� Fast, �ӵ��� �д´�.
	 *
	 * @return   double     : ���� dJogFast	 
	 */
	virtual double GetJogFast();	

	/** 
	 * �� 1���� ���� Sign�� �д´�.
	 *
	 * @return   BOOL          : ���� bSign	 
	 */
	virtual BOOL GetSign();

	/** 
	 * �� 1���� ���� Abs Encoder ��� ���θ� �д´�.
	 *
	 * @return   BOOL          : ���� bAbsData
	 */
	virtual BOOL IsAbsEncoder();

	/** 
	 * �� 1���� ���� �������� ��������� �д´�.
	 *   Limit Sensor ������ ���� �������� �ʱ� ��������� ���� �� �ְ� �Ѵ�.
	 *
	 * @return   BOOL           : ������ bOriginDir(TRUE : +����, FALSE : -����)	
	 */
	virtual BOOL GetOriginDir();

	/** 
	 * �� 1���� ���� �������� ����(Fine����)������ �д´�.
	 *   Fine �ӵ� �������� �ʱ� ��������� ���� �� �ְ� �Ѵ�.
	 *
	 * @return   BOOL           : ������ bOriginDir(TRUE : +����, FALSE : -����)	 
	 */
	virtual BOOL GetOriginFineDir();

	/** 
	 * �� 1���� ���� C�� ��� Type �� �д´�.
	 *
	 * @param   int           : ���� C �� ��� type
	 */
	virtual int GetCPhaseUseType();

	/** 
	 * �� 1���� ���� Scale�� �д´�.
	 *
	 * @return   double         : ���� dScale	 
	 */
	virtual double GetScale();

	/** 
	 * �� �̵� �� ���� �ð��� �д´�.
	 *
	 * @return   double          : ������ �̵� ���� �ð�(�ʴ���)	 
	 */
	virtual double GetMoveTime();

	/** 
	 * �� �̵� �� ����ȭ �ð��� �д´�.
	 *
	 * @return   double          : ������ �̵� �� ����ȭ �ð�(�ʴ���)
	 */
	virtual double GetMoveAfterTime();

	/** 
	 * �� ��ġ ��� ������ �д´�.
	 *
	 * @return   double     : ������ ��ġ ��� ����(mm����)	 
	 */
	virtual double GetTolerance();

	/** 
	 * �� ��ġ ��� ������ �д´�.
	 *
	 * @return   double    : ������ Offset(mm����)
	 */
	virtual double GetPassOffset();

	/** 
	 * �� �������� �Ϸ� ��� �ð�(��)�� �д´�.
	 *
	 * @return   double          : ������ �������� �Ϸ� ��� �ð�(�� ����)	 
	 */
	virtual double GetOriginWaitTime();


	/**
	 * ���� �������� �ƴ��� Ȯ���Ѵ�.(�Ѱ��� �࿡ ���� ����)
	 * 
	 * @return	: TRUE = �������� �Ϸ�
	 *            FALSE = �������� �̿Ϸ�
	 */
	virtual BOOL IsOriginReturn();	

	virtual void SetOriginFlag();	

	/**
	 * �� �������� �����ϱ�(�Ѱ��� �࿡ ���� ����)
	 * 
	 * @param   *pbReturn        :(OPTION = NULL) �������� ��� �б�
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ResetOrigin(BOOL *pbReturn = NULL);


	/**
	 * �� �������� ���� �����ϱ�(������ ��� �࿡ ���� ���� ���� ��� ����)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int StopReturnOrigin(void);


	/**
	 * ���� ������ǥ�� �д´�.
	 * 
	 * @return   double   : ���� ��ǥ��
	 * @param   bCommandType         : ���� ��ġ ����, FALSE=������ġ(Encode), TRUE=��ǥ��ġ(Command)     
	 */
	virtual double GetCurrentPosition(BOOL bCommandType = FALSE);


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
	virtual int SetCurrentPosition(double dCurrentPosition, BOOL bType = FALSE);
	void SetTargetPos(double dTargetPos) {
		m_dTargetAxisPos = dTargetPos;
	}

	/**
	 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
	 * 
	 * @param	bPosOpt              : ���� ��ġ ����, FALSE=������ġ, TRUE=Motion�� ��ǥ��ġ
	 * @param   dTargetPosition      : ���� ��ǥ��	 
	 * @param   *pdPermission        :(OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
	 * @param   *pdDeviation         :(OPTION = NULL) �� ���̰�     
	 * @return  BOOL             : �񱳰��
	 */
	virtual BOOL ComparePosition(BOOL bPosOpt, double dTargetPosition,
								 double *pdPermission = NULL, double *pdDeviation = NULL);


	/**
	 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
	 * 
	 * @param   dTargetPosition      : ���� ��ǥ��	 
	 * @param   *pdPermission        :(OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
	 * @return  BOOL	             : �񱳰��, ���� ��ǥ + Tolerance ���� ������ True, �ƴϸ� False
	 */
	virtual BOOL IsLessThanTargetPosition(double dTargetPosition,
								 double *pdPermission = NULL);


	/**
	 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
	 * 
	 * @param   dTargetPosition      : ���� ��ǥ��	 
	 * @param   *pdPermission        :(OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
	 * @return  BOOL	             : �񱳰��, ���� ��ǥ - Tolerance ���� ũ�� True, �ƴϸ� False
	 */
	virtual BOOL IsGreaterThanTargetPosition(double dTargetPosition,
								 double *pdPermission = NULL);


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
	virtual int Move(double dPosition, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
					  int iMoveType = DEF_MOVE_POSITION);


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
	virtual int StartMove(double dPosition, double dVelocity = 0.0, int iAccelerate = 0,
						   int iDecelerate = 0, int iMoveType = DEF_MOVE_POSITION);

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
	 int RMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
					   int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE);

 
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
	 int StartRMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
							int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE);


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
	virtual int VMove(double dVelocity = 0.0, int iAccelerate = 0, BOOL bDir = TRUE);

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
	virtual int Wait4Done(BOOL bSkipMode = FALSE,BOOL bUseInterlock = FALSE, BOOL bRMove = FALSE);

	/**
	 * ���� ���� �Ϸ� ���θ� Return �Ѵ�.(����� : 2005.01.11)
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ�� �Ǵ� ���Ҿ� In-Position ���α��� Ȯ���Ѵ�.
	 *
	 * MMC Library : axis_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsAxisDone();

	/**
	 *	���� �̵� ��(�ӵ� > 0) ������ Return �Ѵ�.
	 *
	 *	MMC Library : in_motion(axis)
	 *
	 *	@return : TRUE  = �� �̵� ��.
	 *            FALSE = �� ���� ����
	 */
	virtual BOOL IsInMotion();
	
	/**
	 * ���� ���� �Ϸ� ���θ� Return �Ѵ�.(����� : 2005.01.11)
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ���Ѵ�.  In-Position Ȯ�ξ���
	 *
	 * MMC Library : motion_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND(MOTIONLIB)
	 */
	virtual BOOL IsMotionDone();

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
	virtual int MoveSplineLine(int iMaxPoint, double *pdPosition, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE);

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
	virtual int JogMoveSlow(BOOL bDir, double dSlow = 0.0);

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
	virtual int JogMoveFast(BOOL bDir, double dFast = 0.0);

	/**
	 * Jog �̵��� ���� ��Ų��.
	 * 	 
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  �� �� =  Error Code
	 */
	virtual int JogStop();

	/**
	 * ���� �����Ѵ�.(�Ѱ��� �࿡ ���� ����)
	 * 
	 * @param   *pbStatus      :(OPTION = NULL) ���� Stop ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Stop(BOOL *pbStatus = NULL);


	/**
	 * ���� ����̵��� ���� �����Ѵ�.(�Ѱ� ���� ����̵��� ���� ����)
	 * 
	 * @param   *pbState         :(OPTION = NULL) �� ���� VStop ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int VStop(BOOL *pbStatus = NULL);


	/**
	 * ���� ��������Ѵ�.(�Ѱ��� �࿡ ���� �������)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) �� ���� EStop ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int EStop(BOOL *pbStatus = NULL);


	/**
	 * ���� Servo�� On �Ѵ�.(�Ѱ��� �࿡ ���� Servo On ����)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) �� ���� Servo ON ����
	 * @param   *bLinearAxis        :(OPTION = FALSE) ���� Linear Type�̸� TRUE, �׿� FALSE
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ServoOn(BOOL *pbStatus = NULL,  BOOL bSkipSensor = FALSE);


	/**
	 * ���� Servo�� Off �Ѵ�.(�Ѱ��� �࿡ ���� Servo Off ����)
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) �� ���� Servo OFF ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ServoOff(BOOL *pbStatus = NULL, BOOL bSkipSensor = FALSE);


	/**
	 * ���� Home Sensor ���¸� �д´�.(�Ѱ��� �࿡ ���� �����б�)
	 * 
	 * @return  BOOL        : �� ���� Home Sensor ����     
	 */
	virtual BOOL IsOnHomeSensor();


	/**
	 * ���� Positive Sensor ���¸� �д´�.(�Ѱ��� �࿡ ���� �����б�)
	 * 
	 * @return   BOOL        : �� ���� Positive Sensor ����     
	 */
	virtual BOOL IsOnPositiveSensor();


	/**
	 * ���� Negative Sensor ���¸� �д´�.(�Ѱ��� ���� �����б�)
	 * 
	 * @return   BOOL        : �� ���� Negative Sensor ����     
	 */
	virtual BOOL IsOnNegativeSensor();

	/**
	 * ���� ����(Source)�� �д´�. 
	 * 
	 * @return   int       : �� �ϳ��� ���� ����(Source)     
	 */
//	virtual int GetAxisSource();
	virtual int GetAxisSource(int *piReturn); //170713 JSH


	/**
	 * ���� ����(State)�� �д´�. 
	 * 
	 * @retunr  int         : �� �ϳ��� ���� ����(State)     
	 */
	//virtual int GetAxisState(int* piReturn);
	virtual int GetAxisState(UINT32* piReturn); //170713 JSH


	/**
	 * ���� AMP Enable ���¸� �д´�. 
	 * 
	 * @param   BOOL        : �࿡ ���� AMP����(AMP Enable : TRUE, �׿� : FALSE)     
	 */
	virtual BOOL IsAmpEnable();

	/**
	 * ���� AMP Enable/Disable ���·� �����.(����� : 2005.01.11)
	 * 
	 * @param   bEnable          : AMP Enable ���·� ���� : TRUE
	 *							   AMP Disable ���·� ���� : FALSE
	 * @param   bLinearAxis        :(OPTION = FALSE) ���� Linear Type�̸� TRUE, �׿� FALSE
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpEnable(BOOL bEnable);


	/**
	 * ���� AMP Fault ���¸� �д´�. 
	 * 
	 * @return   BOOL         : �࿡ ���� AMP Fault����(AMP Fault:TRUE, Fault�ƴϸ�:FALSE)
	 */
	virtual BOOL IsAmpFault();

	/**
	 * ���� AMP Fault ���¸� Reset �Ѵ�.(����� : 2005.01.11)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ResetAmpFault(BOOL bIsCheck = FALSE);


	/**
	 * ���� AMP Fault ���¸� Enable �Ѵ�. (����� ���� : 2005.01.11)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID POINTER(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpFaultEnable();

	/**
	 * ���� ���¸� �ʱ�ȭ �Ѵ�.(�Ѱ��� �࿡ ���� �ʱ�ȭ)
	 *  Clear Status & Clear Frames
	 * 
	 * @param   *pbStatus        :(OPTION = NULL) �� ���� �ʱ�ȭ ����
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ClearAxis(BOOL *pbStatus = NULL);

	/** 
	 * �������� �켱 ������ Return �Ѵ�.
	 *
	 * @return	�켱 ����		: 0~63
	 */
	virtual int GetOriginPriority();

	/** 
	 * �������� ���� ���� �Լ�(�ѹ� ȣ�⿡ �� Step�� ����)
	 * ���� ���� Step�� �Ϸ� Step�� �� �� ���� ��� ȣ�� �ؾ���
	 *
	 * @param   iOriginMoveType :	DEF_ORIGIN_RETURN_NONE_MOVE(0) -> Motion ���� ���� ���� ��ġ�� �������� �����ϴ� ���.
	 *								DEF_ORIGIN_RETURN_ALL_MOVE	(1) -> ���� ���� Offset �̵� ������ ������ ��� ���� ���� ���� Motion ������ �����ϴ� ���.
	 *								DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE	(2) -> ���� ���� Offset �̵� ������ ������ ���� ���� ���� Motion ������ �����ϴ� ���.
	 * @return	Origin Step		: 1000 = ORIGIN RETURN SUCCESS
	 *							  990 = ERROR
	 *							  xx = ���� ���� ��...
	 */
	virtual int OriginReturn(int iOriginMoveType);

	//100712.KKY_____
	virtual int OriginReturnByUser();
	//_______________

	/**
	 * �������� Step�� �����Ѵ�.(�Ѱ��� ��)
	 * 
	 * @param   iStep            : ������(0:����, 999:����, 1000:�Ϸ�, �׿�:������)
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = INVALID ORIGIN STEP(<0)(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetOriginStep(int iStep);


	/**
	 * �������� Step�� �д´�.(�Ѱ��� ��)
	 * 
	 * @return	Origin Step
	 */
	virtual int GetOriginStep();


	/**
	 * �������� Error�� �ʱ�ȭ�Ѵ�.(�Ѱ��� ��)
	 * 
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ClearOriginError();


	/**
	 * �������� Error�� �д´�.(�Ѱ��� ��)
	 * 
 	 * @param   *piError         : �߻��� ���� Code
     * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetOriginError(int *piError);
	
	/**
	 * Home Sensor�� ������ ��ȯ �Ѵ�.
	 *  	 
     * @return	ERoleOfHome		HOME_N_NONE : Home Sensor ����
								HOME_N_POSITIVE_LIMIT : Home, Positive Sensor ����
								HOME_N_NEGATIVE_LIMIT : Home, Negative Sensor ����
	 */
	virtual ERoleOfHome GetRoleOfHomeSensor();
	

	virtual BOOL IsEStopEvent();

	/*
	*
	*
	*/
	virtual int ChangePhase(int Phase);


	/*
	*
	*
	*/
	virtual int NodeClear();

	/**
	 * �̵��� ��ġ�� SW Limit�� ������� Ȯ���Ѵ�.
	 * 
	 * @param   dPosition		: �˻��� ��ġ
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = OVER SW POSITIVE LIMIT(MULTIAXES)
	 *							  xx = OVER SW NEGATIVE LIMIT(MULTIAXES)
	 */
	int	checkSWLimit(double dPosition);

	/**
	 * �������͵� �࿡ ���ؼ� Event�� �߻��� ��쿡 ���� ���� �� ��ġ�� �Ѵ�.
	 * 
	 * @param   iAxisID			: �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MULTIAXES)
	 *							  xx = HOME SWITCH AXIS SOURCE(MULTIAXES)
	 *							  xx = POSITIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = NEGATIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = AMP FAULT AXIS SOURCE(MULTIAXES)
	 *							  xx = ACCELERATE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = VELOCITY LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = X NEGATIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = X POSITIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = ERROR LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = PC COMMAND AXIS SOURCE(MULTIAXES)
	 *							  xx = OUT OF FRAMES AXIS SOURCE(MULTIAXES)
	 *							  xx = AMP POWER ON OFF AXIS SOURCE(MULTIAXES)
	 *							  xx = RUN STOP COMMAND AXIS SOURCE(MULTIAXES)
	 *							  xx = COLLISION STATE AXIS SOURCE(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	int checkAxisState(int iAxisID);

	int checkAxisExistArgument(BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer, BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer1, void* pPointer2, BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer1, void* pPointer2, void* pPointer3, BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer1, 
								void* pPointer2, 
								void* pPointer3,
								void* pPointer4,
								BOOL bAxisExist);

	int checkAxisExistArgument(void* pPointer1, 
								void* pPointer2, 
								void* pPointer3, 
								void* pPointer4,
								void* pPointer5,
								BOOL bAxisExist);

	int checkBoardExistArgument(int iBoardID, void* pPointer, BOOL bBoardExist);

	void SetAxisPosReset();

	virtual int SetAutoCP(int iBoardNo, BOOL* pbAutoSet);

	virtual int GetAutoCP(int iBoardNo, BOOL* pbAutoSet);

	virtual int GetComVelocity();

	virtual int SetHomeSensorEvent(int iLimit);

	virtual int SetHomeSensorLevel(BOOL bLevel);

	virtual int SetPositiveSensorEvent(int iLimit);

	virtual int SetPositiveSensorLevel(BOOL bLevel);

	virtual int SetNegativeSensorEvent(int iLimit);

	virtual int SetNegativeSensorLevel(BOOL bLevel);

	virtual int GetIndexRequired(BOOL* pbIndexReq);

	virtual int SetIndexRequired(BOOL bIndexReq);

	/**
	 * Jog������ Event�� �߻��� ��쿡 ���� ���� �� ��ġ�� �Ѵ�.
	 * 
	 * @param   iAxisID			: �� ID(0 ~ 63)
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = POSITIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = NEGATIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = AMP FAULT AXIS SOURCE(MULTIAXES)
	 *							  xx = ACCELERATE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = VELOCITY LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = X NEGATIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = X POSITIVE LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = ERROR LIMIT AXIS SOURCE(MULTIAXES)
	 *							  xx = PC COMMAND AXIS SOURCE(MULTIAXES)
	 *							  xx = OUT OF FRAMES AXIS SOURCE(MULTIAXES)
	 *							  xx = AMP POWER ON OFF AXIS SOURCE(MULTIAXES)
	 *							  xx = RUN STOP COMMAND AXIS SOURCE(MULTIAXES)
	 *							  xx = COLLISION STATE AXIS SOURCE(MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	int CheckAxisStateForJog(BOOL bDir);
	
	// Control ������ ���������� �ְų�, �ܺο��� Set_position���� �ִ��� ���� TRUE: �������� �Ϸ�. FALSE : ���� ���� �� �Ϸ�.
	BOOL IsOriginComplete();

	void ResetOriginComplete();

	double DistanceToCurrentToHome();

	int SetHomingSwitchSource(int iSourceAxisNodeID);

	int ClearFrames();

	//100712.KKY_____
	virtual int GetHomeSensorLevel(BOOL *bLevel);
	virtual int GetPositiveSensorLevel(BOOL *bLevel);
	virtual int GetNegativeSensorLevel(BOOL *bLevel);
	//_______________


private:	// internal operation

	/**
	 * SAxis1 Data�� ���� ���� ���� �����Ѵ�.
	 *
	 * @param	srcAx			: ���� ������ ���� Data
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID(MULTIAXES)
	 *							  xx = INVALID MOTION AXIS ID(MULTIAXES)
	 *							  xx = INVALID MOVE-PRIORITY NUMBER(MULTIAXES)
	 *							  xx = INVALID ORIGIN-PRIORITY NUMBER(MULTIAXES)
	 */
	int checkAxisParameter(SAxis1 srcAx);

	/**
	 * Move ���۽� ���� Pos���� Target Pos���� �̵� �ð��� ����Ѵ�. 
	 * 
	 * @return	 �̵� �ð� 
	 */
	double calculateMotionTime();

	/**
	 * RMove ���۽� ���� Pos���� Target Pos���� �̵� �ð��� ����Ѵ�. 
	 * 
	 * @return	 �̵� �ð� 
	 */
	
	/**
	 * Stop(), EStop() �Լ� ������� ���ӵ��� ����.
	* �������� Run ���ӵ� ���� 1/5 �� ����.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID(MOTIONLIB)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	int SetEStopRate();

	int HomingStop();

	// Motor�� Scale�� ������ �� �ִ�.
	int SetMotorScale(double dScale);	
};

#endif //MRSNMC_H

// End of File MRSNMC.h
