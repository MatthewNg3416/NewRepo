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
 * �� ����ü�� Control TabFeeder�� ���� ����� Component List�� �����ϴ� ����ü�̴�.
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
 * �� ����ü�� Control TabFeeder Ư�� ������ �����ϴ� ����ü�̴�.
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

	/** Tension ������ ���� ���� */
	int					m_iTensionCount;
	// Supplier Tension ���� ����.
	BOOL				m_bIsOnTensionSensor;
	BOOL				m_bIsOnOldTensionSensor;

	/** Encoder���� ���� Ƚ����ŭ ���Ͽ� ������ */
	double				m_dEncoderSum;

	/** Encoder���� ��հ� */
	double				m_dEncoderAvg;

	//iTabPitchcount �� ���� �̷�� Pitch Count
	int					m_iTabPitchCount;
	//dOnePitchDist = �� Pitch�� �Ÿ�
	double				m_dOnePitchDist;
	// itabPitchCount * dOnePitchDist
	double				m_dOneTabDist;

	/** Sprocket1 Home�� �������� Ȯ�� */
	BOOL				m_bFlgSprocket1Home;
	/** Sprocket2 Home�� �������� Ȯ�� */
	BOOL				m_bFlgSprocket2Home;

	/** TabIC �� BadTab Sensor ��ġ�� �ִ��� ���� */
	BOOL				m_bFlgFindTabIC;

	/** Bad Tab Count */
	int					m_iBadICCount;

	/** cutting count in mold max */
	UINT				m_iMoldCount;

	/**  Tab IC Detect Flag */
	BOOL				m_bFlgTabIC[DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS]; 
	
	/** Title View Window Handle */
	HWND				m_hTitleViewWnd;

	// Feeding ���� �̸� �������� �� ����(Cylinder��)
	int					prepareBeforeFeeding(int iCheckSkipSprocketNo = -1);

	// TabFeeder ��ȣ�κ��� �ش� �۾����� �´� Bad Tab Count �� ��´�.
	int					getBadTabCount();

/**********
 *  P-Reel ȸ�� Thread
 */
	/** Thread ���� Flag */
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

	/** �� �۾��� �� ���� */
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
	 * @param	commonData			: ���� Component Data
	 * @param	listRefComponent	: CtrlTabFeeder Component���� ������ Component Pointer
	 * @param	datComponent		: CtrlTabFeeder Component Data
	 */
	MCtrlTabFeeder(SCommonAttribute commonData, SCtrlTabFeederRefCompList listRefComponent, SCtrlTabFeederData datComponent);

	/**
	 * Destructor
	 */
	~MCtrlTabFeeder();

/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 
public:   
	
	/**
	 * CtrlTabFeeder Component�� �ʱ�ȭ �Ѵ�.
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int Initialize(void);

	/**
	 * TabType�� �ʱ�ȭ �Ѵ�.
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int InitializeTabType(void);

	/**
	 * CtrlTabFeeder Component�� �ʱ�ȭ �Ѵ�.
	 *
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int InitializeTabFeeder();

	/**
	 * MCtrlTabFeeder�� Data Parameter�� �����Ѵ�. 
	 *
	 * @param	datcomponent : ������ MCtrlTabFeeder�� Parameter
	 * @return	int (0 = Success, Error Code = �� ��)
	 */
	int	SetData(SCtrlTabFeederData datcomponent);
	
    /**
	* MCtrlTabFeeder�� Component List�� �����Ѵ�.
	* @param	listRefCompList : ������ MCtrlPCBHandler�� Component List
	 * @return	int (0 = Success, Error Code = �� ��)
	*/
	int	AssignComponents(SCtrlTabFeederRefCompList listRefComponents);

/***********************************************************************************************
/*
/* Step Interface
/*
/************************************************************************************************/

	/** Reel Cyl Down -> Clamp Cyl Down -> Press Vac On */	
	int PressTapeStep1();

	/**	Reel Cyl Up -> Press Cyl Up�� ������ Feeding ���� ������ */
	int PressTapeStep2();

	/**
	*	Press Cyl UP -> Reel Cyl Up�� �Ѵ�.	
	*	Check Part()�� ���ؼ� ���� ������ Ȯ���Ѵ�.
	*	Bad Tab ���¸� Ȯ���Ͽ� MotorFeeding()�� �����Ѵ�.
	*/
	int FeedTabIC();

	/** Tab Feeding Control
	*	- Press Vac�� Off�Ѵ�.
	*	- Press Cyl�� Up�Ѵ�.
	*	- Reel Cyl�� Up�Ѵ�.
	*	- flag�� ���� ���� ���
	*	 + (+1) : +1 Pitch�̵�
	*	 + (-1) : -1 Pitch�̵�
	*	 + (+2) : +1 Tab �̵�
	*	 + (-2) : -1 Tab �̵�
	*/
	int FeedTabIC(int iSel);

	// Auto Homing  bHomingSprocket1 : Reel AutoChange�ÿ��� Homing�ÿ��� BadTabIC ã�� ������ ���� Back�� ���� �ʴ´�. 
	int ExecuteAutoHoming(BOOL bHomingSprocket1 = FALSE);

	/** ExecuteAutoHoming�� ������ �� TabIC�� �����Ͽ� �ʱ���ġ�� ã�� ����*/
	int ExecuteAutoFindIC();

	/** Reel Auto Change ����. */
	int ExecuteReelAutoChange();

	int ExecuteSprocket1Homing();

	void SetHomeFlagSprocket1(BOOL bFlag);			
	void SetHomeFlagSprocket2(BOOL bFlag);			
	BOOL GetHomeFlagSprocket1();					
	BOOL GetHomeFlagSprocket2();					

	// hongju ����.(Sprocket2�� �ʿ�)
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
	/** - Tab IC ���� ��ŭ Feeding ���͸�  ȸ���ؼ� TabIC�� ������.
	 *	- Tab IC ���� �� IC ���� ���� ���� 
	 */
	int feedMotor();

	/** Reel Auto Change ���� ���� �����ؾ� �� �͵�
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
