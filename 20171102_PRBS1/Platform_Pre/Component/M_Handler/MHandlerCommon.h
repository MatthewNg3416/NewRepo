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
	// kss 1. cylinder 없음 삭제 

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

	/** Panel 감지용 Sensor Address */
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

	// kss 5. 불필요 
	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;
	
	/** Panel Data 관리 Class */
	MPanelData*		m_plnkPanelData;

	// kss 6 Handler.h로 옮겨야 한다. 공통사항이 아님. 

	/** 고정좌표 Data가 저장될 File Name */
	CString			m_strPosFileName;
	
	/** Offset 좌표 Data 가 저장될 File Name */
	CString			m_strOffsetPosFileName;
	
	/** Offset 좌표 Data가 저장될 File Path */
	CString			m_strOffsetPosFilePath;


/**********
 * 좌표
 */
  
	//20060503 ysb
	/** Handler - 고정좌표   */
	// kss 7. 삭제 필요 Handler에 대한 예전 Data임. 
/************************************************************************/
/*                             Position                                 */
/************************************************************************/

	// kss 8 주석을Stage에서 Handler로 바꾼다. 
	/** Handler X, Y, T 축 Position 정보 */
    STeachingInfo	m_sXYTTeachingInfo;

	/** Handler Z 축 Position 정보 */
	STeachingInfo	m_sZTeachingInfo;



	/** Handler XYT축 고정좌표계 */
	MPos_XYT	m_rgsXYTFixedPos[DEF_HANDLER_MAX_POS];
	/** Handler XYT축 모델좌표계 */
	MPos_XYT	m_rgsXYTModelPos[DEF_HANDLER_MAX_POS];
	/** Handler XYT축 Offset 좌표계 */
	MPos_XYT	m_rgsXYTOffsetPos[DEF_HANDLER_MAX_POS];

	// jdy 3. Handler 는 EtcOffset 필요 없음. 삭제.
	/** Handler Etc Offset 좌표 */

	// kss 9. 삭제 필요 Handler에 필요없는 Data임. 

	/** Handler Z축 고정 좌표 */
	double		m_rgdZFixedPos[DEF_HANDLER_MAX_Z_POS];
	
	/** Handler Z축 Offset 좌표 */
	double		m_rgdZOffsetPos[DEF_HANDLER_MAX_Z_POS];

	/** Handler Z축 모델 좌표 */
	double		m_rgdZModelPos[DEF_HANDLER_MAX_Z_POS];

/**********
 * 기타 메세지
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
/* 생성자 & 소멸자
/*
/************************************************************************************************/

 
	MHandlerCommon(SCommonAttribute commonData, SHandlerRefCompList listRefComponent);

	virtual ~MHandlerCommon();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	int		AssignComponents(SHandlerRefCompList sCompList);

	IAxis*	GetMotionComponent(int nAxisID);


/***********************************************************************************************
/*
/* Teaching 관련 제공 Interface
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


    //Mamual 동작시 사용
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
	// jdy 4. CheckOrigin() 에서 XYT 축을 담당하는지 표시해 주던가(CheckOriginXYT()), 아니면 Z 도 CheckOrigin() 안으로 넣고, 선택에 따라 원점복귀 여부를 보는 것이 좋을 듯. (어차피 XYT 만 움직여도 Z 원점복귀 여부는 확인 해야하므로 CheckOrigin() 안에 넣는게 좋을 듯.)

	int CheckXYTOrigin();

	BOOL CheckZOrigin();
 
	// jdy 5. 축 배정 구조상 (XYT - Z) 아래 함수 구현 불편 & 사용 빈도 거의 없음. 그냥 GetXYTCurrentPos(), GetZCurrentPos() 로 구분된 것 그냥 사용. (현재 함수는 Handler (X - Y 축 구성) 의 잔재임)

	// kss 10. 인수중에 뒤의 두개는 필요없음. 
	// jdy 6. 축 배정 구조상 (XYT - Z) 아래 함수 구현 불편 & 사용 빈도 거의 없음. 그냥 GetXYTTargetPos(), GetZTargetPos() 로 구분된 것 그냥 사용. (현재 함수는 Handler (X - Y 축 구성) 의 잔재임)

	// kss 17. 불필요 아래에 각 축별로 확인하는 코드가 있음. 

	// kss 11. 다른 축에 대한 AmpFault/AmpEnable함수도 필요함. 
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
	// kss 12. GetCurrentPos시 Command/encorder Type 선택 여부 가능하도록 할 것인지 안할 것인지 결정
	// 필요 없음. 
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

	// kss 13 아래 함수 4개는 MHandler.h에서 선언하면 됨. 이 함수들이 꼭 있어야 한다는 법은 없다. 여기서는 삭제.
	virtual int checkInterlockForZ(int iPosID) = 0;

	virtual int checkInterlockForXYT(int iPosID,BOOL bUseAuto = FALSE) = 0;

//	virtual int SetIOAfterHandlerDown() = 0;

//	virtual int SetIOAfterHandlerUp() = 0;

};

#endif // MHANDLER_COMMON_H
