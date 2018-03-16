/* 
 * Control TabFeeder Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef MCTRLTABFEEDER_H
#define MCTRLTABFEEDER_H

#include "ICommon.h"
#include "DefSystem.h"
#include "MLCNetData.h"
#include "DefTabFeeder.h"
#include "MTickTimer.h"

class MSystemData;
class MPreBonderData;
class MProductInfoData;
class MPanelData;
class MTabData;
class MProcessData;
class MTabFeeder;
class IStateTabCarrier;
class MLCNet;
class IRFID;
class MManageOpPanel;
class MTrsAutoManager;
class CDBTabLoss;

const int DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS = 50;

/**
 * 이 구조체는 Control TabFeeder의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	MTabFeeder*			m_plnkTabFeeder;
	IStateTabCarrier*	m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	MLCNet*				m_plnkLCNet;
	IRFID*				m_plnkRFID;
	MManageOpPanel*		m_plnkManageOpPanel;
#if FALSE
	IStateTabCarrier*	m_plnkTabCarrier;
#endif
	//111014 SJ_KJS
	MTrsAutoManager*		m_plnkTrsAutoManager;
} SCtrlTabFeederRefCompList;

/**
 * This structure is defined data of Control TabFeeder.
 * 이 구조체는 Control TabFeeder 특성 정보를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct
{
	MSystemData*		m_plnkSystemData;
	MPreBonderData*		m_plnkPreBonderData;
	MTabData*			m_plnkTabData;
	MProductInfoData*	m_plnkProductInfoData;
	MPanelData*			m_plnkPanelData;
	MProcessData*		m_plnkProcessData;
	CDBTabLoss*			m_plnkDBTabLoss;

} SCtrlTabFeederData;

/**
 * Error Define
 */
const int ERR_CTRL_TABFEEDER_SUCCESS	= 0;

/**
 * Control TabFeeder Class Define
 */
class MCtrlTabFeeder : virtual public ICommon
{
private:	
	MTabFeeder*			m_plnkTabFeeder;
//	IStateTabCarrier*	m_plnkTabCarrier;
	IStateTabCarrier*	m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	MLCNet*				m_plnkLCNet;
	IRFID*				m_plnkRFID;
	MManageOpPanel*		m_plnkManageOpPanel;


	MSystemData*		m_plnkSystemData;
	MPreBonderData*		m_plnkPreBonderData;
	MTabData*			m_plnkTabData;
	MProductInfoData*	m_plnkProductInfoData;
	MPanelData*			m_plnkPanelData;
	MProcessData*		m_plnkProcessData;
	CDBTabLoss*			m_plnkDBTabLoss;

	/** Tension 조절을 위한 변수 */
	int					m_iTensionCount;
	// Supplier Tension 감지 센서.
	BOOL				m_bIsOnTensionSensor;
	BOOL				m_bIsOnOldTensionSensor;

	/** Encoder값을 일정 횟수만큼 더하여 저장함 */
	double				m_dEncoderSum;

	/** Encoder값의 평균값 */
	double				m_dEncoderAvg;

	//iTabPitchcount 한 탭을 이루는 Pitch Count
	int					m_iTabPitchCount;
	//dOnePitchDist = 한 Pitch의 거리
	double				m_dOnePitchDist;
	// itabPitchCount * dOnePitchDist
	double				m_dOneTabDist;

	/** Sprocket1 Home이 잡혔는지 확인 */
	BOOL				m_bFlgSprocket1Home;
	/** Sprocket2 Home이 잡혔는지 확인 */
	BOOL				m_bFlgSprocket2Home;

	/** TabIC 가 BadTab Sensor 위치에 있는지 여부 */
	BOOL				m_bFlgFindTabIC;

	/** Bad Tab Count */
	int					m_iBadICCount;

	/** cutting count in mold max */
	UINT				m_iMoldCount;

	/**  Tab IC Detect Flag */
	BOOL				m_bFlgTabIC[DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS]; 
	
	/** Title View Window Handle */
	HWND				m_hTitleViewWnd;

	// Feeding 전에 미리 움직여야 할 동작(Cylinder들)
	int					prepareBeforeFeeding(int iCheckSkipSprocketNo = -1);

	// TabFeeder 번호로부터 해당 작업변에 맞는 Bad Tab Count 를 얻는다.
	int					getBadTabCount();

/**********
 *  P-Reel 회수 Thread
 */
	/** Thread 구동 Flag */
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	CWinThread*			m_pThreadSupplyReel;//Mech Ver2

	BOOL				m_bRunTension;
	BOOL				m_bRunTensionReverse;
	BOOL				m_bStopTensionCompleted;
	BOOL				m_bManualOperation;

	BOOL				m_bFeederReady4DryRun;//110323.Add
	
	//Mech Ver2_____
	EFeederReelMoveType	m_eFeederReelMoveType;
	int					m_iFeederReelMoveErr;
	BOOL				m_bManualFind;
	CRITICAL_SECTION	m_csFindSensor;
	MTickTimer			m_tmrRecoveryTension;
	//______________
	//111014 SJ_KJS
	MTrsAutoManager*	m_plnkTrsAutoManager;

public:
	int m_iZigUsedQtyEquip;
	int m_iZigTotalQtyEquip;

	CString m_strZigBatchID;
	CString	m_strZigTotalQtyAfterGRD;
	CString	m_strZigUsedQtyAfterRGD;
	CString	m_strZigTotalQty;
	CString	m_strZigUsedQty;

	MMaterial m_matTabICInfo[DEF_MAX_REEL];
	MMaterial m_matZigInfo;

	BOOL m_bConsumeStart[DEF_MAX_REEL];
	BOOL m_bPreHandState;
	BOOL m_bWarningState;
	int	m_iSprocketFeedingCount;

	/** 총 작업할 탭 개수 */
	int m_iXAssemble;
	int m_iYAssemble;

	BOOL m_bZigConsumeStart;
	BOOL m_bZigWarningState;
	BOOL m_bZigRFIDState;

	int m_iSelectedReel;

public:  
	/**
	 * @stereotype constructor
	 *
	 * @param	commonData			: 공통 Component Data
	 * @param	listRefComponent	: CtrlTabFeeder Component에서 참조할 Component Pointer
	 * @param	datComponent		: CtrlTabFeeder Component Data
	 */
	MCtrlTabFeeder(SCommonAttribute commonData, SCtrlTabFeederRefCompList listRefComponent, SCtrlTabFeederData datComponent);

	/**
	 * Destructor
	 */
	~MCtrlTabFeeder();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
public:   
	
	/**
	 * CtrlTabFeeder Component를 초기화 한다.
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int Initialize(void);

	/**
	 * TabType을 초기화 한다.
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int InitializeTabType(void);

	/**
	 * CtrlTabFeeder Component를 초기화 한다.
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int InitializeTabFeeder();

	/**
	 * MCtrlTabFeeder에 Data Parameter를 설정한다. 
	 *
	 * @param	datcomponent : 설정할 MCtrlTabFeeder에 Parameter
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int	SetData(SCtrlTabFeederData datcomponent);
	
    /**
	* MCtrlTabFeeder에 Component List를 설정한다.
	* @param	listRefCompList : 설정할 MCtrlPCBHandler에 Component List
	 * @return	int (0 = Success, Error Code = 그 외)
	*/
	int	AssignComponents(SCtrlTabFeederRefCompList listRefComponents);

/***********************************************************************************************
/*
/* Step Interface
/*
/************************************************************************************************/

	/** Reel Cyl Down -> Clamp Cyl Down -> Press Vac On */	
	int PressTapeStep1();

	/**	Reel Cyl Up -> Press Cyl Up의 순으로 Feeding 동작 마무리 */
	int PressTapeStep2();

	/**
	*	Press Cyl UP -> Reel Cyl Up을 한다.	
	*	Check Part()를 통해서 자재 유무를 확인한다.
	*	Bad Tab 상태를 확인하여 MotorFeeding()을 실행한다.
	*/
	int FeedTabIC();

	/** Tab Feeding Control
	*	- Press Vac를 Off한다.
	*	- Press Cyl을 Up한다.
	*	- Reel Cyl을 Up한다.
	*	- flag에 따른 동작 방식
	*	 + (+1) : +1 Pitch이동
	*	 + (-1) : -1 Pitch이동
	*	 + (+2) : +1 Tab 이동
	*	 + (-2) : -1 Tab 이동
	*/
	int FeedTabIC(int iSel);

	// Auto Homing  bHomingSprocket1 : Reel AutoChange시에는 Homing시에는 BadTabIC 찾는 동작을 위한 Back을 하지 않는다. 
	int ExecuteAutoHoming(BOOL bHomingSprocket1 = FALSE);

	/** ExecuteAutoHoming을 동작한 후 TabIC를 검출하여 초기위치를 찾는 동작*/
	int ExecuteAutoFindIC();

	/** Reel Auto Change 실행. */
	int ExecuteReelAutoChange();

	int ExecuteSprocket1Homing();

	void SetHomeFlagSprocket1(BOOL bFlag);			
	void SetHomeFlagSprocket2(BOOL bFlag);			
	BOOL GetHomeFlagSprocket1();					
	BOOL GetHomeFlagSprocket2();					

	// hongju 수정.(Sprocket2만 필요)
	void SetFindTabICFlag(BOOL bFlag);
	BOOL GetFindTabICFlag();

	void SetMoldCount(int iMoldCount);
	int GetMoldCount();
		
	void SetConsumStart(int iReelSelect, BOOL bConsumeStart);
	void SetZigConsumStart(BOOL bConsumeStart);
	void SetZigWarningState(BOOL bWarningState);
	int ReadZigInfo();
	int WriteZigInfo();
	BOOL GetZigRFIDState();
	void InitAssembleCount();
	void ChangeSpareReelInfotoUsingReel();

	void SetSelectedReel(int iReelSelect);
	int GetSelectedReel();

	void SetTitleWindowHandle(HWND hWnd);

	int ChangeModel(void);

	int CheckAndRequestRailUnfold(int iRetryLimit = 3);

	void SetPReelTensionManualOperation(BOOL bState);

	void SetRunTension(BOOL bset);
	void SetRunTensionReverse(BOOL bSet);

	//110323.Add_____
	BOOL GetFeederReady();
	void SetFeederReady4DryRun(BOOL bReady);
	//_______________

	//Mech Ver2______
	void SetFeederReelMoveType(EFeederReelMoveType eType);
	EFeederReelMoveType	GetFeederReelMoveType();
	int GetFeederReelMoveErr();

	int	FindWeightUpSensor();
	int FindWeightDownSensor();
	int EscapeWeightUpSensor();
	int EscapeWeightDownSensor();
	//_______________

	//130218.kms________
	BOOL GetBadTabCntInterlock();
	//__________________
	
private:
	/** - Tab IC 길이 만큼 Feeding 모터를  회전해서 TabIC를 공급함.
	 *	- Tab IC 공급 후 IC 유무 여부 저장 
	 */
	int feedMotor();

	/** Reel Auto Change 전에 사전 동작해야 할 것들
	 */
	int prepareReelAutoChange();

	void threadRun();

	void threadStop();

	static UINT threadFunction(LPVOID pParam);
	
    void threadJob();

	//Mech Ver2_____
	static UINT threadFunctionSupplyFeeder(LPVOID pParam);
	void threadJobSupplyFeeder();

	int	find_WeightUpSensor();
	int find_WeightDownSensor();
	int escape_WeightUpSensor();
	int escape_WeightDownSensor();
	//______________

	//eMatNG 12.08.14 yh
public:
	int m_iNG_CurTab;
	int m_iNG_OldTab;
	int m_iNG_Count;

};

#endif //MCTRLTABFEEDER_H
