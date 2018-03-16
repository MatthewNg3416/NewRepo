/* 
 * Manage OpPanel Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MManageOpPanel.h : This file defines the implements class of manage oppanel component.
 *			  이 파일은 manage oppanel Class에 대한 implements class를 정의한다.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef MMANAGEOPPANEL_H
#define MMANAGEOPPANEL_H

#include "MTickTimer.h"
#include "DefManageOpPanel.h"
#include "ICommon.h"
//20110222 SJ_HJG
#include "MTowerLampData.h"

class MSystemData;
class IOpPanel;

/** This class is defined manage oppanel component implements class as a abstract class.
 * 이 Class는 manage oppanel Class에 대한 implements Class로 정의한다.
 * 
 */
class MManageOpPanel : public virtual ICommon
{
private:
/**********
 *  Reference Components
 */
	/** OpPanel Component Pointer Parameter */
	IOpPanel* m_plnkOpPanel;

/**********
 *  ManageOpPanel Data
 */
	/** System Data Link Pointer */
	MSystemData* m_plnkSystemData;	

	//20110222 SJ_HJG
	MTowerLampData* m_plnkTowerLampData;

/**********
 *  Jog
 */
	/** Jog로 이동할 Motion에 대한 정보 Index */
	int m_iJogIndex;

	/** Jog Key 이전 값 저장 용 변수 */
	int m_iPrevJogVal_X;
	int m_iPrevJogVal_Y;
	int m_iPrevJogVal_T;
	int m_iPrevJogVal_Z;

	BOOL m_bPrevDir_X;
	BOOL m_bPrevDir_Y;
	BOOL m_bPrevDir_T;
	BOOL m_bPrevDir_Z;

/**********
 *  Blink
 */
	/** Blink Rate */
	double m_dBlinkRate;

	/** Tower Lamp Blink 동작을 위한 Timer */
	MTickTimer m_BlinkTimer;

	/** Blink Interval 계산을 위한 변수 */
	BOOL m_bBlinkState;

	volatile BOOL m_bBuzzerMode;

#ifdef SIMULATION
/**********
 * Simulation Switch
 */
	BOOL m_bSimulationStartSW;
	BOOL m_bSimulationStopSW;
	BOOL m_bSimulationResetSW;
#endif

public:    
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

    /**
	 * 생성자
     * @stereotype constructor 
     */
    MManageOpPanel();

	MManageOpPanel(SCommonAttribute commonData, IOpPanel *pOpPanel, MSystemData *pSystemData);

    /**
	 * 소멸자
     * @stereotype destructor 
     */
    virtual ~MManageOpPanel();    

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

    /**
     * OpPanel Component Object를 설정한다. 
	 *
	 * @param	*pOpPanel : 설정할 OpPanel Component Object Pointer
     */
     void AssignComponents(IOpPanel *pOpPanel);

   /**
     *  운전 초기 상태로 설정된다.
	 *
	 * @return	Error Code : 0 = SUCCESS, 그 외 = Error
     */
    int Initialize();

/***********************************************************************************************
/*
/* 안전 센서 체크
/*
/************************************************************************************************/ 

	/**
	 * Door Sensor 점검여부를 설정한다.
	 *
	 * @param bFlag : 점검 여부 (TRUE:점검, FALSE:무시)
	 * @param iGroup : (OPTION=-1) Door Sensor Group 번호 (-1이면 모든 Group내 설정)
	 * @param iIndex : Door Snesor Group 내 Index 번호 (-1이면 Group내 모든 Index 설정)
	 */
	void SetDoorCheckFlag(BOOL bFlag, int iGourp = -1, int iIndex = -1);

	/**
	 * Door의 상태를 읽어온다.
	 *
	 * @return	: Door 상태 (TRUE:OPEN, FALSE:CLOSE)
	 */
	BOOL GetDoorSWStatus();


	/* 110411.KKY.Add. Material Change Door_____
	*/
	void SetMaterialChangeFlag(BOOL bFlag, int iGroup = -1, int iIndex = -1);	
/***********************************************************************************************
/*
/* Switch 상태 확인
/*
/************************************************************************************************/ 

	/**
	 * Start Switch의 눌린 상태 읽어온다.
	 *
	 * @param	*pbStatus : Start Switch 눌린 상태 (TRUE:ON, FALSE:OFF)
	 * @return	Error Code : 0=SUCCESS, 그외=Error
	 */
	BOOL GetStartSWStatus();

	/**
	 * Stop Switch의 눌린 상태 읽어온다.
	 *
	 * @param	*pbStatus : Stop Switch 눌린 상태 (TRUE:ON, FALSE:OFF)
	 * @return	Error Code : 0=SUCCESS, 그외=Error
	 */
	BOOL GetStopSWStatus();

	/**
	 * E-Stop Switch의 눌린 상태 읽어온다.
	 *
	 * @param	*pbStatus : E-Stop Switch 눌린 상태 (TRUE:ON, FALSE:OFF)
	 * @return	Error Code : 0=SUCCESS, 그외=Error
	 */
	BOOL GetEStopSWStatus();

	/**
	 * Teaching Pendant 사용 상태를 읽어온다.
	 *
	 * @return	Error Code : 0=SUCCESS, 그외=Error
	 */
	BOOL GetTPStatus();

	/**
	 * Reset Switch의 눌린 상태 읽어온다.
	 *
	 * @param	*pbStatus : Reset Switch 눌린 상태 (TRUE:ON, FALSE:OFF)
	 * @return	Error Code : 0=SUCCESS, 그외=Error
	 */
	BOOL GetResetSWStatus();

/***********************************************************************************************
/*
/* OpPanel (Operation Panel) 제어
/*
/************************************************************************************************/ 

	/**
	 * Tower Lamp Blink 속도 설정하기
	 *
	 * @param	dRate : (OPTION=0.5) 설정할 Blink 속도
	 */
	void SetBlinkRate(double dRate = 0.5);

	/**
	 * OpPanel의 Switch 및 Tower Lamp의 On/OIff를 지시한다.
	 *
	 * @param	iState : On/Off Mode
	 *		   Mode                     Start	Step	TowerR	TowerY	TowerG	Buzzer
	 *		1:Step Stop 완료 상태		  X		 O		   O	  X		  X		  X
	 *		2:Start(Run Ready) 상태		  O		 X		   X	  O		  O		  X
	 *		3:Run 상태					  O		 X		   X	  X		  O		  X
	 *		4:Step Stop 진행 상태		  X		 O		   B	  X		  X		  X
	 *		5:Error Stop 진행 상태		  O		 X		   B	  X		  B		  O
	 *		6:Cycle Stop 진행 상태		  O		 X		   X	  X		  B		  X
	 *		7:Parts Empty 상태			  O		 X		   X	  B		  O		  O
	 *		  (Operator Call 상태)
	 * @return	Error Code : 0=SUCCESS, 그외=Error
	 */
	ErrorInfo SetOpPanel(int iState);
	
	/** 
	 * Buzzer Mode를 Set 한다.
	 * @param BOOL : TRUE = Buzzer Mode, FALSE = Buzzer Off Mode
	 */
	void SetBuzzerMode(BOOL bMode);

	/** 
	 * Buzzer Mode를 반환 한다.
	 * @return BOOL : TRUE = Buzzer Mode, FALSE = Buzzer Off Mode
	 */
	BOOL IsBuzzerMode();

/***********************************************************************************************
/*
/* Jog Operation
/*
/************************************************************************************************/ 

	/**
	 * Jog Key 확인하여 해당 Jog 축을 이동/정지한다.
	 *
	 * @return	Error Code : 0=SUCCESS, 그외=Error
	 */
	int MoveJog(void);

	/**
	 * Jog에 사용할 Unit Index를 설정한다.
	 *
	 * @param	iUnitIndex : (OPTION=-1) Jog에 사용할 Unit Index
	 */
	void SetJogUnit(int iUnitIndex = DEF_MNGOPPANEL_JOG_NO_USE);

	/** 
	 * 설정된 Jog에 사용할 Unit Index를 읽는다.
	 *
	 * @return	Error Code : 0=SUCCESS, 그외=Error
	 */
	int GetJogUnit(void);

#ifdef SIMULATION
/***********************************************************************************************
/*
/* Simulation Operation
/*
/************************************************************************************************/ 
	void SetSimulationStartSW(BOOL bState);

	void SetSimulationStopSW(BOOL bState);
	
	void SetSimulationResetSW(BOOL bState);
#endif
};
#endif //MMANAGEOPPANEL_H
