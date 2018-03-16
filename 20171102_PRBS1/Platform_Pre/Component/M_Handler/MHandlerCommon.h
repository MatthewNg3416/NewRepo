/* 
 * Handler Component
 *
 * Copyright 2005 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * HandlerCommmon.h : Headerfile of HanlderCommon Class.
 */

#ifndef MHANDLER_COMMON_H
#define MHANDLER_COMMON_H

#include <math.h>
#include "IStateHandler.h"
//#include "MHandler.h" 
#include "MTeachingPosData.h"

class MHandlerCommon : public virtual ICommon, public IStateHandler
{
protected:   

/**********
 *  Reference Components
 */
	/** I/O Object */
	IIO*			m_plnkIO;
	/** Up/Down Cylinder Object */
	// kss 1. cylinder ���� ���� 

	/** Vacuum Line Object - Panel Vacuum (single : 1 - In) */
//@	IVacuum*		m_plnkVac1;
	/** Vacuum Line Object - Panel Vacuum (single : 2 - Out) */
//@	IVacuum*		m_plnkVac2;
	/** Vacuum Line Object - Panel Vacuum (single : 3 - Out) */
//@	IVacuum*		m_plnkVac3;
	
	IVacuum*		m_plnkVac[DEF_MAX_THANDLER_VAC];

	MNetH*			m_plnkMelsecIF;
/**********
 *  Data
 */

	// Handler Type
	int				m_iHandlerType;

	/** Panel ������ Sensor Address */
	unsigned short	m_usiIAddrPanelDetect;

	/** Panel Detect Sensor Address */
//@	unsigned short	m_usiIAddrArmViolation;

	/** Panel Detect Sensor Address */
//@	unsigned short	m_usiOAddrArmViolation;

	/** Panel Detect Sensor Address */
//@	unsigned short	m_usiIAddrMoveInhibit;

	/** Panel Detect Sensor Address */
//@	unsigned short	m_usiOAddrMoveInhibit;

	unsigned short	m_usOAddrIFMoveEnable;

	unsigned short	m_usOAddrPanelBlow1;
	unsigned short	m_usOAddrPanelBlow2;

	// kss 5. ���ʿ� 
	/** System Data ���� Class */
	MSystemData*	m_plnkSystemData;
	
	/** Panel Data ���� Class */
	MPanelData*		m_plnkPanelData;

	// kss 6 Handler.h�� �Űܾ� �Ѵ�. ��������� �ƴ�. 

	/** ������ǥ Data�� ����� File Name */
	CString			m_strPosFileName;
	
	/** Offset ��ǥ Data �� ����� File Name */
	CString			m_strOffsetPosFileName;
	
	/** Offset ��ǥ Data�� ����� File Path */
	CString			m_strOffsetPosFilePath;


/**********
 * ��ǥ
 */
  
	//20060503 ysb
	/** Handler - ������ǥ   */
	// kss 7. ���� �ʿ� Handler�� ���� ���� Data��. 
/************************************************************************/
/*                             Position                                 */
/************************************************************************/

	// kss 8 �ּ���Stage���� Handler�� �ٲ۴�. 
	/** Handler X, Y, T �� Position ���� */
    STeachingInfo	m_sXYTTeachingInfo;

	/** Handler Z �� Position ���� */
	STeachingInfo	m_sZTeachingInfo;



	/** Handler XYT�� ������ǥ�� */
	MPos_XYT	m_rgsXYTFixedPos[DEF_HANDLER_MAX_POS];
	/** Handler XYT�� ����ǥ�� */
	MPos_XYT	m_rgsXYTModelPos[DEF_HANDLER_MAX_POS];
	/** Handler XYT�� Offset ��ǥ�� */
	MPos_XYT	m_rgsXYTOffsetPos[DEF_HANDLER_MAX_POS];

	// jdy 3. Handler �� EtcOffset �ʿ� ����. ����.
	/** Handler Etc Offset ��ǥ */

	// kss 9. ���� �ʿ� Handler�� �ʿ���� Data��. 

	/** Handler Z�� ���� ��ǥ */
	double		m_rgdZFixedPos[DEF_HANDLER_MAX_Z_POS];
	
	/** Handler Z�� Offset ��ǥ */
	double		m_rgdZOffsetPos[DEF_HANDLER_MAX_Z_POS];

	/** Handler Z�� �� ��ǥ */
	double		m_rgdZModelPos[DEF_HANDLER_MAX_Z_POS];

/**********
 * ��Ÿ �޼���
 */
	/** Log Message	*/
	char	m_szLogMsg[250];

	CString m_strTTLog;

/************************************************************************/
/*                             Motion                                   */
/************************************************************************/
	/** Handler X Motion Object */
	IAxis*		m_plnkXAxis;

	/** Handler Y Motion Object */
	IAxis*		m_plnkYAxis;
	
	/** Handler T Motion Object */
	IAxis*		m_plnkTAxis;

	/** Handler Z Motion Object */
	IAxis*		m_plnkZAxis;

public:

/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/

 
	MHandlerCommon(SCommonAttribute commonData, SHandlerRefCompList listRefComponent);

	virtual ~MHandlerCommon();

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	int		AssignComponents(SHandlerRefCompList sCompList);

	IAxis*	GetMotionComponent(int nAxisID);


/***********************************************************************************************
/*
/* Teaching ���� ���� Interface
/*
/************************************************************************************************/ 

/**********
 * Handler X,Y,T,Z Position
 */

  
	int LoadXYTFixedPosParameter(void);

	int LoadXYTOffsetPosParameter(void);

	int LoadZFixedPosParameter(void);

	int LoadZOffsetPosParameter(void);

	int SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos);

	int SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos);

	int SaveZFixedPosParameter(double* rgdFixedPos);

	int SaveZOffsetPosParameter(double* rgdOffsetPos);

	int GetXYTPosParameter(	MPos_XYT* rgsFixedPos = NULL,
							MPos_XYT* rgsOffsetPos = NULL,
							MPos_XYT* rgsModelPos = NULL);
	
	MPos_XYT GetXYTModelPosParameter(int iPos);

	MPos_XYT GetXYTOffsetPosParameter(int iPos); 

	int GetZPosParameter(double* rgdFixedPos, 
		                 double* rgdOffsetPos,
						 double* rgdModelPos);
	
	STeachingInfo GetXYTTeachingInfo();

	STeachingInfo GetZTeachingInfo();

/***********************************************************************************************
/*
/* Operation Interface
/*
/************************************************************************************************/ 

/**********
 * Vacuum
 */
	int AbsorbPanel(BOOL bSkipSensor = FALSE);

	int ReleasePanel(BOOL bSkipSensor = FALSE);

	int CheckVacuum();

	int AbsorbPanel(int iVacID, BOOL bSkipSensor);

	int ReleasePanel(int iVacID, BOOL bSkipSensor);


    //Mamual ���۽� ���
//@	int VacuumOn1(BOOL bSkipSensor = FALSE);
//@	int VacuumOn2(BOOL bSkipSensor = FALSE);
//@	int VacuumOn3(BOOL bSkipSensor = FALSE);
//@	int VacuumOff1(BOOL bSkipSensor = FALSE);
//@	int VacuumOff2(BOOL bSkipSensor = FALSE);
//@	int VacuumOff3(BOOL bSkipSensor = FALSE);

//@	BOOL IsVacOn1();
//@	BOOL IsVacOn2();
//@	BOOL IsVacOn3();
//@	BOOL IsVacOff1();
//@	BOOL IsVacOff2();
//@	BOOL IsVacOff3();

/**********
 * Move
 */
	
	int SafeMoveXYTPos(int iPosID, BOOL bUseAuto = FALSE);

	int SafeMoveXPos(int iPosID, BOOL bUseAuto = FALSE);

	int SafeMoveZPos(int iPosID, BOOL bUseAuto = FALSE);

	virtual int MoveXYTPos(MPos_XYT posTarget, int iPosID = -1, BOOL bUseAuto = TRUE) = 0;

	virtual int MoveXPos(double dTarget, int iPosID = -1, BOOL bUseAuto = TRUE) = 0;

	virtual int MoveZPos(double dTarget, int iPosID, BOOL bUseAuto = TRUE) = 0;
	
	virtual int MoveXYTurnPos()=0;
	

private:


public:

/***********************************************************************************************
/*
/* Operation State Interface
/*
/************************************************************************************************/

/**********
 * Vacuum
 */
	BOOL IsAbsorbPanel(void);

	BOOL IsReleasePanel(void);

	BOOL IsAbsorbPanel(int iVacID);

	BOOL IsReleasePanel(int iVacID);

	BOOL IsAbsorbPanelOneMore();

/**********
 * Sensor
 */
	BOOL IsPanelDetected(void);

/**********
 * Position
 */
	// jdy 4. CheckOrigin() ���� XYT ���� ����ϴ��� ǥ���� �ִ���(CheckOriginXYT()), �ƴϸ� Z �� CheckOrigin() ������ �ְ�, ���ÿ� ���� �������� ���θ� ���� ���� ���� ��. (������ XYT �� �������� Z �������� ���δ� Ȯ�� �ؾ��ϹǷ� CheckOrigin() �ȿ� �ִ°� ���� ��.)

	int CheckXYTOrigin();

	BOOL CheckZOrigin();
 
	// jdy 5. �� ���� ������ (XYT - Z) �Ʒ� �Լ� ���� ���� & ��� �� ���� ����. �׳� GetXYTCurrentPos(), GetZCurrentPos() �� ���е� �� �׳� ���. (���� �Լ��� Handler (X - Y �� ����) �� ������)

	// kss 10. �μ��߿� ���� �ΰ��� �ʿ����. 
	// jdy 6. �� ���� ������ (XYT - Z) �Ʒ� �Լ� ���� ���� & ��� �� ���� ����. �׳� GetXYTTargetPos(), GetZTargetPos() �� ���е� �� �׳� ���. (���� �Լ��� Handler (X - Y �� ����) �� ������)

	// kss 17. ���ʿ� �Ʒ��� �� �ະ�� Ȯ���ϴ� �ڵ尡 ����. 

	// kss 11. �ٸ� �࿡ ���� AmpFault/AmpEnable�Լ��� �ʿ���. 
	BOOL IsXAxisAmpFault();

	BOOL IsXAxisAmpEnable();

	BOOL IsYAxisAmpFault();

	BOOL IsYAxisAmpEnable();

	BOOL IsTAxisAmpFault();

	BOOL IsTAxisAmpEnable();

	BOOL IsZAxisAmpFault();

	BOOL IsZAxisAmpEnable();

	/**********
     * Position
     */
	// kss 12. GetCurrentPos�� Command/encorder Type ���� ���� �����ϵ��� �� ������ ���� ������ ����
	// �ʿ� ����. 
	MPos_XYT GetXYTCurrentPos(BOOL bType = FALSE);

	double GetZCurrentPos(BOOL bType = FALSE);

	virtual MPos_XYT GetXYTTargetPos(int iPosID, int iTabNum = -1) = 0;

	double GetZTargetPos(int iPosID);
    
	BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL);

	BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL);

	BOOL IsInXPos(int iPosID, double* pdTolerance = NULL);

	BOOL IsInXPos(double dTargetPos, double* pdTolerance = NULL);

//@	BOOL IsInYPos(int iPosID, double* pdTolerance = NULL);

//@	BOOL IsInYPos(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInTPos(int iPosID, double* pdTolerance = NULL);

	BOOL IsInTPos(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInZPos(int iPosID, double* pdTolerance = NULL);

	BOOL IsInZPos(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInXYTMotion();

	// kss 13 �Ʒ� �Լ� 4���� MHandler.h���� �����ϸ� ��. �� �Լ����� �� �־�� �Ѵٴ� ���� ����. ���⼭�� ����.
	virtual int checkInterlockForZ(int iPosID) = 0;

	virtual int checkInterlockForXYT(int iPosID,BOOL bUseAuto = FALSE) = 0;

//	virtual int SetIOAfterHandlerDown() = 0;

//	virtual int SetIOAfterHandlerUp() = 0;

};

#endif // MHANDLER_COMMON_H
