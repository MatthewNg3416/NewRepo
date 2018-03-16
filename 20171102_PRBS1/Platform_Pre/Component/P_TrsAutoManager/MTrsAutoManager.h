/* 
 * MTrsAutoManage Component
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

/**
 * MTrsAutoManage.h : Headerfile of MTrsAutoManage component.
 */

#ifndef MTRSAUTOMANAGE_H
#define MTRSAUTOMANAGE_H

#include "ICommon.h"
#include "MTickTimer.h"
#include "DefSystem.h"
#include "MNetH.h"
#include "MLCNet.h"
#include "DefAxisPreBonder.h"

class MSystemData;
class MProcessData;
class MWorkSchedule;
class MSystemInfo;
class MProductInfoData;
class MPanelData;
class MAutoAlignData; //SJ_YYK 150318 Add..

class IIO;
class IAxis;
class IOpPanel;
class MPanelAligner;
class MPanelTransfer;
class MPanelTransferCenter;
class MPanelTransferOut;
class MTabFeeder;
//class MRollerConveyor;
class MManageOpPanel;
class MCtrlTabFeeder;
class MTrsPanelAligner;
class MTrsPanelTransfer;
class MTrsPanelTransferOut;
class MTrsPanelTransferCenter;
class MTrsLoadConveyor;
class MTrsUnloadConveyor;
class MTrsTabCarrier;
class MTrsTabFeeder;
class MTrsTabMounter;
class MTrsInspectionCamera;
class MTrsTHandler; //SJ_YYK 161104 Add..

#define PROCESS_ALARM(errorInfo)	m_plnkTrsAutoManager->ProcessAlarm(errorInfo);
//#define PROCESS_AUTOMANAGER_ALARM(iResult)	ProcessAlarm(GetObjectID(),iResult, GetErrorLastLogItem(),__LINE__);

//110808 SJ_KJS
//syr..SESL 090610 m_iModelChangeDisableState ����� ���� Define..
#define DEF_NOT_USE			-1
#define DEF_T2_STATE		 0
#define DEF_NORMAL_STATE	 1
#define DEF_DOWN_STATE		 2

// Error Code Definition
const int ERR_TRS_AUTO_MANAGER_SUCCESS		= 0;

// Panel Data Error
const int ERR_NO_BATCHID_DATA				= 1;
const int ERR_NO_DEVICEID_DATA				= 2;
const int ERR_NO_PANELID_DATA				= 3;
const int ERR_SAME_PANELID_IN_LAST_5TIME	= 4;
const int ERR_NO_PANEL_DATA					= 5;

//110630_____
const int ERR_NO_DEVICEID_MCODE_REQUEST		= 6;	//���� ������ DeviceID�� ��� �ֽ��ϴ�
const int ERR_NO_PRODUCT_PLAN_DATA			= 7;	//�����ȹ ����
const int ERR_DISCORD_DEVICE_ID				= 8;	//��ȹ������ DeviceID�� ����Panel�� DeviceID�� ����ġ

const int ERR_COMPLETE_PRODUCT_PLAN			= 9;	//���� ��ȹ �����
const int ERR_CNTOVER_PRODUCT_PLAN			= 10;	//End������� �ʰ�������

const int ERR_MCODE_NOT_EXIST				= 11;	//��ϵ� ���������� ��ġ�ϴ� ���簡 �����ϴ�.

const int 	ERR_HPANELID					= 21;
const int 	ERR_EPANELID					= 22;
const int 	ERR_BATCHID						= 23;
const int 	ERR_PRODTYPE					= 24;
const int 	ERR_PRODKIND					= 25;
const int 	ERR_DEVICEID					= 26;
const int 	ERR_STEPID						= 27;
const int 	ERR_PPID						= 28;
const int 	ERR_THICKNESS					= 29;
const int	ERR_INSFLAG						= 30;
const int	ERR_PANELSIZE					= 31;
const int	ERR_PANELPOSITION				= 32;
const int	ERR_COUNT1						= 33;
const int	ERR_COUNT2						= 34;
const int	ERR_GRADE						= 35;
const int	ERR_COMMENT						= 36;
const int	ERR_COMPSIZE					= 37;
const int	ERR_READINGFLAG					= 38;
const int	ERR_PANELSTATE					= 39;
const int	ERR_JUDGEMENT					= 40;
const int	ERR_CODE						= 41;
const int	ERR_RUNLINE						= 42;
const int	ERR_UNIQUEID					= 43;
const int	ERR_PAIRHPANELID				= 44;
const int	ERR_PAIREPANELID				= 45;
const int	ERR_PAIRGRADE					= 46;
const int	ERR_PAIRUNIQUEID				= 47;
const int	ERR_FLOWRECIPE					= 48;
const int	ERR_RESERVED0					= 49;
const int	ERR_BITSSIGNALS					= 50;
const int	ERR_REFERDATA					= 51;
//_______________________________________

//20100212.kms_______________________________
const	int		DEF_STOP_BTN_NONE		= 0;
const   int		DEF_STOP_BTN_FRONT		= 1;
const   int		DEF_STOP_BTN_BACK		= 2;
//___________________________________________

/** TrsAutoManage Component�� ������ Component List */
typedef struct tagSTrsAutoManageRefCompList
{
	MManageOpPanel*		m_plnkManageOpPanel;
	IOpPanel*			m_plnkOpPanel;
	IAxis**				m_plnkMotion;
	MSystemInfo*		m_plnkSystemInfo;
	IIO*				m_plnkIO;
	MNetH*				m_plnkMelsecIF;
	MNetH*				m_plnkMelsecAOCIF; //SJ_YYK 150821 Add..
	MLCNet*				m_plnkLCNet;
	MPanelAligner*      m_plnkPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	MPanelTransfer*		m_plnkPanelTransferCenter;
	MPanelTransfer*		m_plnkPanelTransfer;
#endif
	MPanelTransfer*		m_plnkPanelTransferOut;
	MTabFeeder*			m_plnkTabFeeder[DEF_MAX_TABFEEDER];
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder[DEF_MAX_TABFEEDER];
	MTrsTHandler*		m_plnkTrsTHandler; //SJ_YYK 161104 Add.
} STrsAutoManageRefCompList;

/** TrsAutoManage Data */
typedef struct tagSTrsAutoManageData
{
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORKER_GROUP];
	MWorkSchedule*		m_plnkWorkScheduleSecond[DEF_MAX_WORKER_GROUP];
	MProductInfoData*	m_plnkProductInfoData;
	MPanelData*			m_plnkPanelData;
	MAutoAlignData*		m_plnkAutoAlignData; //SJ_YYK 150318 Add..

	unsigned short		m_rgusUpperIN[DEF_MAX_INTERFACE_IO];
	unsigned short		m_rgusUpperOUT[DEF_MAX_INTERFACE_IO];
	unsigned short		m_rgusLowerIN[DEF_MAX_INTERFACE_IO];
	unsigned short		m_rgusLowerOUT[DEF_MAX_INTERFACE_IO];

} STrsAutoManageData;

/** Process Layer List */
typedef struct  tagSTrsAutoManagerRefProcess
{
	MTrsPanelAligner*	m_plnkTrsPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	MTrsPanelTransferCenter*	m_plnkTrsPanelTransferCenter;
	MTrsPanelTransfer*	m_plnkTrsPanelTransfer;
#endif
	MTrsPanelTransferOut*	m_plnkTrsPanelTransferOut;
	MTrsTabCarrier*		m_plnkTrsTabCarrier[DEF_MAX_TABCARRIER];
	MTrsTabCarrier*		m_plnkTrsRTabCarrier[DEF_MAX_TABCARRIER];
	MTrsTabFeeder*		m_plnkTrsTabFeeder[DEF_MAX_TABFEEDER];	
	MTrsInspectionCamera* m_plnkTrsInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTrsTabMounter*		m_plnkTrsTabMounter[DEF_MAX_TABMOUNTER];
	MTrsInspectionCamera* m_plnkTrsRInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTrsTabMounter*		m_plnkTrsRTabMounter[DEF_MAX_TABMOUNTER];
	MTrsTHandler*		m_plnkTrsTHandler; //SJ_YYK 161104 Add..

} STrsAutoManagerRefProcess;

//20131115 SJ_HJG
typedef struct  tagSProcessParameter
{
	double			m_dMountInspectionDown;
	double			m_dMountInspectionTime;
	double			m_dMountInspectionUp;
	double			m_dMountMove;
	double			m_dMountBondingDown;
	double			m_dMountBondingTime;
	double			m_dMountBondingUp;
	
} SProcessParameter;

class MTrsAutoManager : public virtual ICommon
{
public:
	/** Glass Data �� ������ Pointer �迭
	 *	index	0 : Load Conveyor
	 *			1 : Panel Transfer
	 *			2 : Panel Aligner
	 *			3 : Unload Conveyor
	 */
	MGlassDatas*		m_pGlassData[DEF_MAX_PANEL_UNIT];

	BOOL					m_bErrorStopState; //100121 SJ_YYK

	//___________________________________________*/
	//101204.kms__________________
	BOOL				m_bDoorCheck;
	//____________________________

	//110721. SJ_YTH____
	BOOL				m_bDoorLockCheck;
	//__________________

	//20110914 SJ_HJG
	BOOL		m_bLCConnected;
	BOOL		m_bAirCVBlowOff;
	//�ٸ� ������ ��� �ϱ� ���� ��ġ �̵�
	CString				m_strNewDeviceID;

	BOOL				m_bFeederChange;

private:
/**********
 *  Reference Components
 */
	MManageOpPanel*		m_plnkManageOpPanel;
	IOpPanel*			m_plnkOpPanel;
	IAxis**				m_plnkMotion;
	MSystemInfo*		m_plnkSystemInfo;
	IIO*				m_plnkIO;
	MNetH*				m_plnkMelsecIF;
	MNetH*				m_plnkMelsecAOCIF; //SJ_YYK 150821 Add..
	MLCNet*				m_plnkLCNet;
	MPanelAligner*      m_plnkPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	MPanelTransfer*     m_plnkPanelTransferCenter;
#	endif
	MPanelTransfer*     m_plnkPanelTransfer;
#endif
	MPanelTransfer*     m_plnkPanelTransferOut;
	MTabFeeder*			m_plnkTabFeeder[DEF_MAX_TABFEEDER];
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder[DEF_MAX_TABFEEDER];

/**********
 *  Data
 */
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORKER_GROUP];
	MWorkSchedule*		m_plnkWorkScheduleSecond[DEF_MAX_WORKER_GROUP];
	MProductInfoData*	m_plnkProductInfoData;
	MPanelData*			m_plnkPanelData;
	MAutoAlignData*		m_plnkAutoAlignData; //SJ_YYK 150318 Add..

	unsigned short		m_rgusUpperIN[DEF_MAX_INTERFACE_IO];
	unsigned short		m_rgusUpperOUT[DEF_MAX_INTERFACE_IO];
	unsigned short		m_rgusLowerIN[DEF_MAX_INTERFACE_IO];
	unsigned short		m_rgusLowerOUT[DEF_MAX_INTERFACE_IO];

/**********
 *  System Threads
 */
	MTrsPanelAligner*			m_plnkTrsPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	MTrsPanelTransferCenter*	m_plnkTrsPanelTransferCenter;
#	endif 
	MTrsPanelTransfer*			m_plnkTrsPanelTransfer;
#endif
	MTrsPanelTransferOut*		m_plnkTrsPanelTransferOut;
	MTrsTabCarrier*				m_plnkTrsTabCarrier[DEF_MAX_TABCARRIER];
	MTrsTabCarrier*				m_plnkTrsRTabCarrier[DEF_MAX_TABCARRIER];
	MTrsTabFeeder*				m_plnkTrsTabFeeder[DEF_MAX_TABFEEDER];
	MTrsInspectionCamera*		m_plnkTrsInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTrsTabMounter*				m_plnkTrsTabMounter[DEF_MAX_TABMOUNTER];
	MTrsInspectionCamera*		m_plnkTrsRInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTrsTabMounter*				m_plnkTrsRTabMounter[DEF_MAX_TABMOUNTER];
#ifdef DEF_USE_TURN_HANDLER
	MTrsTHandler*				m_plnkTrsTHandler; //SJ_YYK 161104 Add..
#endif
	/** Critical Section */
	CRITICAL_SECTION	m_csAlarmProcess;

	/** Critical Section : Monitoring Log */
	CRITICAL_SECTION	m_csMonitoringLog;

	CRITICAL_SECTION	m_csMonitoringLogOp;
	//20111205 SJ_HJG
	CRITICAL_SECTION	m_csAutoAlignLog;


	//120116.kms__________
	/** Critical Section */
	CRITICAL_SECTION	m_csUseFeeder4Mount;

	/** Thread ���� Flag*/
	BOOL				m_bThreadLife;

	/** Thread Handle */
	CWinThread*			m_pThread;

	// Real Time Thread
	// : �׽� ����Ǿ�� �ϴ� �׸� ó�� - Message Box ������ ���࿡ ������ �־�� �ȵȴ�.
	CWinThread*			m_pRealTimeThread;

	// Monitoring Thread
	CWinThread*			m_pMonitoringThread;

	CWinThread*			m_pDoorMonitoringThread;	//@DoorLock

	CWinThread*			m_pAutoAlignThread; //SJ_YYK 150318 Add...

	BOOL				m_bAutoReady;
	BOOL				m_bErrorProcessing;
	BOOL				m_bEStopPressed;
	BOOL				m_bTPConnected;
	BOOL				m_bStepStopPressed; //syr..090508
	
	EOpMode				m_eOPMode;
	EOPStatus			m_eOPStatus;

	//EQ_STOP Report Flag �߰� 100121 SJ_YYK
	BOOL					m_bStepStopState;

	//20100212.kms_________
	int m_iStopBtnOnStatus;
	//_____________________

	/** Auto Main View Window Handle */
	HWND				m_hAutoViewWnd;
	HWND				m_hManualViewWnd;
	HWND				m_hDataViewWnd;
	HWND				m_hTeachViewWnd;
	
	/** Title View Window Handle */
	HWND				m_hTitleViewWnd;

	/** Log Main View Window Handle */
	HWND				m_hLogViewWnd;

	/** ���� ��ü Ȯ�� �÷��� */
	BOOL				m_bMaterialExchange;

	/** ���� ���� Panel �� �ִ��� ���� */
	BOOL				m_bEmptyPanel;
	BOOL				m_bManufacturingEmpty;

	BOOL				m_bFullPanel;//2009.05.15 CYJ

	/** Operator Call Flag */
	BOOL				m_bOperatorCall;

	/** Operator Call State
	 *	0 : Feeder1 Op Call State
	 *	1 : Feeder2 Op Call State
	 *	2 : Panel Aligner Op Call State
	 */
	int					m_iSelectState[DEF_MAX_OP_CALL_SELECT];

	/** PM Mode Flag */
	BOOL				m_bPMMode;

	/** AutoRun ���� �� Error Msg �� L/C �� �ø��� �ʴ� �ð� ������ ����ϴ� Timer */
	MTickTimer			m_timerSkipError;
	MTickTimer			m_timerManufacturingEmpty;

	//120228.kms_________________
	// Model Change ���� �����ð����� Error Report���� �ʱ����� Timer
	MTickTimer m_tChangeModelTimer;

	/** Vision Error �߻� �� Error Image Display ���� */
	BOOL				m_bDisplayErrorImage;

	// Manual Mode �� ��ȯ ���̶�� Message �� ǥ���ϰ� �ִ��� �ƴ��� ���θ� ����.
	BOOL				m_bDisplayManualModeChangeMsg;

	/** ������ �߻��� ���� �ð� */
	CTime				m_tmLastError;

	/** ������ �߻��� ���� ��ȣ */
	int					m_iLastErrorNo;

	// Melsec Monitoring
	BOOL				m_bMelsecSwitch;
	int					m_iMelsecAlarmCount;
	int					m_iMonitoringCount;

	/* selsk_jdy
	// Tact Timer
	MTickTimer			m_EQTactTimer;

	// Tact Time
	double				m_dEQTactTime;
	*/

	// �ӽ� - Monitoring Log �м��ؼ� Vacuum ���� �κ� ã�Ƽ� ��ġ �Ŀ� ������ ��.
	// Vacuum Error Detect Counter
	int					m_iVacuumErrorCounter;

	// NSMC
	BOOL				m_bInputLastGlass;
	CString				m_strNewModelFolderName;
//	CString				m_strNewDeviceID;
	BOOL				m_bModelChangeStart;
	BOOL				m_bPanelInStopBeforeAutoMC;
	BOOL				m_bPanelInStopNSMC;
	MModelChangeWordDatas*			m_pNSMCLocalData;
	MModelChangeWordDatas*			m_pNSMCControlData;
	BOOL				m_bOldModelChangeDisable;
	BOOL				m_bNSMCTabICChange;
	BOOL				m_iNSMCControlNo;
	BOOL				m_bNSMCWaitingCheckMaterial;

	//110324_____
	int					m_iFeederUseMode;	//0->Feeder1, 1->Feeder2, 2:All Feeder
	BOOL				m_b1FeederAllMounterInitStart;
	//___________

	//110623.Panel ���� ���� ���� ���_____
	BOOL	m_bManualPlanMode;
	//_____________________________________
	
	//110624_______________________________
	BOOL	m_bLastPanelCurProductPlan;
	//_____________________________________

	//120116.kms_____
	BOOL				m_bMountAutoChangeInitStart;

	//120216.kms_____
	int					OldUseFeeder; //4MountAll ���� �ʱ�ȭ �� ���� UseFeeder�� ����Ѵ�.

	// 091028 ���� Key Parameter ����
	BOOL				m_bMeasureProcessData;
	int					m_iMeasProcessData[50];
	int					m_iMeasParamError;
	//20121004 SJ_HJG Ⱦ����
	double		m_dTabEndMarkDistance1;
	double		m_dTabEndMarkDistance2;

	//@DoorLock____________
	BOOL		m_bAllDoorOpen;
	BOOL		m_bFrontDoorOpen;
	BOOL		m_bRearDoorOpen;
	BOOL		m_bFrontMaterialDoorOpen;
	BOOL		m_bRearMaterialDoorOpen;
	//@____________________

/***********************************************************************************************
/*
/*	Private Method
/*
/************************************************************************************************/
private:
	int	processOpPanel();

	int processNetState();

	void threadJob();

	void threadRealTimeJob();

	void threadMonitoringJob();

	static UINT threadFunction(LPVOID pParam);

	static UINT threadRealTimeFunction(LPVOID pParam);

	static UINT threadMonitoringFunction(LPVOID pParam);

	//@DoorLock____________
	static UINT threadDoorMonitoringFunction(LPVOID pParam);
	void		threadDoorMonitoringJob();
	//@____________________

	static UINT threadAutoAlignFunction(LPVOID pParam); //SJ_YYK 150318 Add..
	void threadAutoAlignMonitoringJob(); //SJ_YYK 150318 Add..

	BOOL isAllAutoRun();

	BOOL isAllStepStop();
	
	BOOL isAllModelChangeReady();
	
	//110324_____
	BOOL isAll1FeederAllMountInitReady();
	//___________

	//120116.kms_______
	BOOL isMountAutoChangeReady();
	//_________________

	BOOL isAllInitialized();

	void resetAllInitialized();

	int checkStartRunCondition();

	BOOL isErrorReportCode(int nErrorCode);

	int checkHeaterAlarm();

	/** ���� ���� ���� */
	CString	getCurrentEquipState();

	// jdy 12pm
	BOOL isNoPanelInEquipment();

//	BOOL isFullPanelInEquipment();//2009.05.15 CYJ

	// TT_Alarm
	BOOL				m_bTactTimeLogSave;
	//  T/T�� ��� �ð�
	MTickTimer			m_timerTactTimeLogSave;
	BYTE				m_ucLevel;
	
	//2009.05.15 CYJ
	// Ư���ð� Panel�� Full���� Ȯ���ϴ� Ÿ�̸�
	MTickTimer			m_eqpFullCheckTimer;
	BOOL				m_bEqpFullCheckTimerStarted;

	//syr..SESL 090610
	// Ư���ð� Panel�� Empty���� Ȯ���ϴ� Ÿ�̸�
	MTickTimer			m_eqpEmptyCheckTimer;
	BOOL				m_bEqpEmptyCheckTimerStarted;

	//syr..SESL 090610
	int m_iModelChangeDisableState;

	//SESL_LKH_090713 : Heater Alarm Counter 
	int					m_iHeaterAlarmCount;

	//SESL_�䱸���׹ݿ�
	int					m_iMoldCount1;
	int					m_iOldMoldCount1;

	//2009.08.04 CYJ : Mold Count Over Pre Alarm Counter 1
	int					m_iMoldCountAlarmCount1;

	int					m_iMoldCount2;
	int					m_iOldMoldCount2;

	//2009.08.04 CYJ : Mold Count Over Pre Alarm Counter 2
	int					m_iMoldCountAlarmCount2;

	//111022 sj_kjs
	MTickTimer			m_TimerMachineStopTime;
	//_______________________________
	// 2011.11.05 Park, YK - //ELLAS_111103
	//ELLAS_110630 ���� ���� ���� ���� Ȯ��
	void checkMelecFlowState();
	BOOL m_bOldUpperSendAble;
	BOOL m_bNewUpperSendAble;
	BOOL m_bOldLowerReceiveAble;
	BOOL m_bNewLowerReceiveAble;
	//ELLAS_110712 ������ ���� Ȯ��, Network���� Ȯ��
	void checkUpperEquipPause();
	DWORD m_lOldTick;
	DWORD m_lNewTick;

/***********************************************************************************************
/*
/* ������ & �Ҹ���
/*
/************************************************************************************************/
public:
	MTrsAutoManager(SCommonAttribute commonData, STrsAutoManageRefCompList listRefComponents, STrsAutoManageData datComponent);
    ~MTrsAutoManager();   
	
/***********************************************************************************************
/*
/* Component ���� ���� Interface
/*
/************************************************************************************************/ 

	int	SetData(STrsAutoManageData datcomponent);
	
	int	AssignComponents(STrsAutoManageRefCompList listRefComponents);

/***********************************************************************************************
/*
/* Process Assign Interface
/*
/************************************************************************************************/

	void AssignProcess(STrsAutoManagerRefProcess listRefTrs);

/***********************************************************************************************
/*
/* Thread���� ���� Interface
/*
/************************************************************************************************/ 

	void ThreadRun();

	void ThreadStop();

/***********************************************************************************************
/*
/* Mode,Status ���� Interface
/*
/************************************************************************************************/ 

	EOpMode GetOPMode();

	EOPStatus GetOPStatus();

	void SetOPMode(EOpMode eOPMode);

	void SetOPStatus(EOPStatus eOPStatus);
	
	int	EStopAllAxis();

	BOOL IsAutoReady();

	CString GetObjectName(int iErrorCode);

	BOOL IsPanelEmpty();

	BOOL IsPanelFull();

	BOOL isFullPanelInEquipment();//2009.05.15 CYJ

	BOOL IsOperatorCall();
	
// sesl_jdy	void SetOperatorCall(BOOL bState, CString strReceive, BOOL bBuzzerMode = DEF_DEFAULT);
	void SetOperatorCall(BOOL bState, CString strReceive, BOOL bBuzzerMode = DEF_DEFAULT, BOOL bDisplayOpDlg = TRUE);

	void SetSelectState(int iSelect, int iMode);

	int	GetSelectState(int iSelect);

	void SetDisplayErrorImage(BOOL bState);

	BOOL IsDisplayErrorImage();

	// sesl_jdy
	BOOL IsOffInterlockInvalidationSignal();

	BOOL IsStepStopSWPressed();//syr..090508

	//SESL_LKH_090711 : ���� �Լ� ������ ���� �߰�
	BOOL	IsDoorOpen();

	//@DoorLock________________
	/*
	BOOL	IsDoorOpenFront();
	BOOL	IsDoorOpenRear();
	*/
	//@________________________

	//131111. SJ_YTH
	BOOL	IsAllDoorOpen();
	BOOL	IsFrontDoorOpen();
	BOOL	IsRearDoorOpen();
	BOOL	IsFrontMeterialKeyAutoMode();
	BOOL	IsRearMeterialKeyAutoMode();
	BOOL	IsMeterialDoorOpenFront();
	BOOL	IsMeterialDoorOpenRear();
	int		LockAllDoor();
	int		LockMaterialFrontDoor();
	int		LockMaterialRearDoor();
	int		UnlockAllDoor();
	int		UnlockMaterialFrontDoor();
	int		UnlockMeterialRearDoor();	
	//________________________


	BOOL	IsAllMotorStop(BOOL bMotorStop[DEF_AXIS_MAX_AXIS]);
/***********************************************************************************************
/*
/* Operation Interface
/*
/************************************************************************************************/

	void SetWindowHandle(HWND hWnd);
	HWND GetAutoViewHandle();
	
	void SetManualViewHandle(HWND hWnd);
	HWND GetManualViewHandle();

	void SetDataViewHandle(HWND hWnd);
	HWND GetDataViewHandle();
	
	void SetTeachViewHandle(HWND hWnd);
	HWND GetTeachViewHandle();

	void SetLogViewHandle(HWND hWnd);
	HWND GetLogViewHandle();

	void SetTitleWindowHandle(HWND hWnd);
	HWND GetTitleViewHandle();

	int ProcessAlarm(int iProcessID, int iErrorCode, SLogItem LogItem, int iLineNo);
	
	int ResetOriginAllAxis();

	void ResetUnitInitialFlag();
	
	int ResetAllMotorEvent();

	int	AutoReady();
	
	void SetMaterialExchange(BOOL bFlag);

	BOOL IsMaterialExchange();


	/** ���� ���¸� ���� �ֱ�� ����Ѵ�. Ư���� �̺�Ʈ�� �߻����� �� ����Ѵ�.*/
	BOOL WriteMonitoringLog(CString strMsg);

	//20111028 SJ_HJG
	BOOL WritePanelIDLog(CString strMsg);

	/** OpStatus, OpMode ����� ����Ѵ�.(Normal Run, DryRun Mode, PassRun ����) */
	BOOL WriteMonitoringLogOp(CString strMsg);

	//20111205 SJ_HJG
	BOOL WriteAutoAlignLog(CString strMsg);

	void GetThreadAutoRunStatus(BOOL* prgStatus);


/***********************************************************************************************
/*
/* For Panel Data
/*
/************************************************************************************************/

	void ManageGlassData(EPanelEvent eEventType, int iUnitID);

	void TransferGlassData(int iPrevUnitID, int iNextUnitID);

	void AssignGlassData(int iUnitID);

	//121204.kms_______________
	void	AssignPassData(int iUnitID);   //121204.kms
	void	ResetPassData(int iUnitID);
	BOOL 	GetAssignPassData(int iUnitID);   //121204.kms
	//_________________________

	int CheckPanelValidation(int iUnitNo);

	int	CheckPanelValidationItem(int iUnitNo);
	
	int ConvertValidationCodeToErrorCode(int iValidationCode);
	
	CString GetGlassID(int iUnitID);

/***********************************************************************************************
/*
/* ��Ÿ
/*
/************************************************************************************************/ 

	/* selsk_jdy trs panel aligner ���� ó��
	// Tact Timer Start
	void		StartEQTactTimer();

	// Tact Timer Stop
	void		StopEQTactTimer();

	// Tact Time
	double		GetEQTactTime();
	*/

	int	SafetyModeInvalidate(BOOL bOnOff);

	void GetMonitoringLogResult();

	// TT_Alarm
	void	SetTactTimeLogSave(BOOL bTactTimeLogSave);
	BOOL	IsTactTimeLogSave();

	// hongju_SESL   //Load Conveyor�� Panel Stop �������� �˷��ش�..
//	BOOL		IsPanelInStopLoadConveyor();
	// hongju_MC : Auto NSMC_INFO���� �Է¹��� New Model ������ Setting ���ִ� �κ�..
	CString		m_strNewModelName;

	//110114.kms__________
	CString		m_strNewDeviceName;
	CString		m_strEquipModelName;

	//120228.kms_________________
	// Model Change ���� �����ð����� Error Report���� �ʱ����� Timer����
	void		StartChangeModelTimer();

	BOOL		IsPanelInStopPanelTransferCenter();

	//NSMC
	MModelChangeWordDatas*	GetCurrentNSMCInfo();
	void		SetCurrentNSMCInfo(MModelChangeWordDatas nsmcData);
	BOOL		IsLastGlassInput();
	BOOL		IsLastGlassOnWorker(int iWorkerID);
	BOOL		IsStartModelChange();
	void		ResetAutoModelChagneFlag();
	CString		GetNewModelDataFolderName(CString strDeviceID);
	void		SetPanelInStopNSMC(BOOL bPanelInStopNSMC);
	BOOL		IsPanelInStopNSMC();
	CString		GetNewModelFolderName() { return m_strNewModelFolderName; }
	CString		GetNewDeviceID() { return m_strNewDeviceID; }
	BOOL		GetModelChangeStart() { return m_bModelChangeStart; }
	BOOL		GetNSMCTabICChange() { return m_bNSMCTabICChange; }
	void		SetNSMCTabICChange(BOOL bState) { m_bNSMCTabICChange = bState; }
	int			PrepareAfterMC();
	BOOL		GetModelChangeReserveBit();
	void		SetNSMCControlNo(int iControlNo) { m_iNSMCControlNo = iControlNo; }
	void		CheckModelChangeDisable(); //syr..SESL 090610
	void		CheckMaterialValidation(int nTabFeederNo, int nSelectedReel, BOOL bAutoModelChange = FALSE);
	BOOL		IsWaitingCheckMaterial() { return m_bNSMCWaitingCheckMaterial; }

	//110324_____
	void		SetFeederUseMode(int iMode) { m_iFeederUseMode = iMode; }
	int			GetFeederUseMode() { return m_iFeederUseMode; }
	BOOL		Get1FeederAllMounterInitStart() { return m_b1FeederAllMounterInitStart; }
	void		Reset1FeederAllMounterInitStart() { m_b1FeederAllMounterInitStart = FALSE; }
	//___________

	//120116.kms_________
	BOOL		GetMountAutoChangeInitStart() { return m_bMountAutoChangeInitStart; }
	void		ResetMountAutoChangeInitStart() { m_bMountAutoChangeInitStart = FALSE; }
	//___________________

	// hongju_MC : Auto NSMC_INFO���� �Է¹��� New Model ������ Setting ���ִ� �κ�..
//110114.kms__________
//	void		SetNewModelName(CString strNewModelName);
	void		SetNewModelName(CString strNewModelName, CString strNewDeviceName = "NONE");
	CString		GetNewModelName();

	//SESL_�䱸���׹ݿ�
#ifdef DEF_SESL_SITE
	int			ReturnMold1Count() { return m_iMoldCount1; }
	void		ResetMold1Count() { m_iMoldCount1 = 0; }
	int			ReturnMold2Count() { return m_iMoldCount2; }
	void		ResetMold2Count() { m_iMoldCount2 = 0; }
#endif

private:
	double		getSecondsFromTimeString(CString strTime);
	double		getElapsedTime(CString strStart, CString strEnd);
	BOOL		stringExists(CString strSource, CString strTag);

private:
	// Add. 110525 �������� ���� Bit �߰�
	BOOL				m_bModelReserveStart;
	MTickTimer			m_timerModelReserve;

public:
	BOOL		GetNewModelReserveBit();	// Add. 110525 �������� ���� Bit �߰�
	
	//SESL_�䱸���׹ݿ�
#ifdef DEF_SESL_SITE
	void		checkMoldCount();
#endif

public:
	//100807.ECID__________________
	//
	//ECid�� ���� �����ϸ�  ECid�� �ش��ϴ� parameter���� MEqConstItem �о�´�
	int GetEqConstData(CString strECID, MEqConstant *Data);

	// EC Data�� RecodeSet�� �߰��Ѵ�.
	int InsertEqConstData(MEqConstant *pData);

	// �ϳ��� ECID �� ���Ͽ� EC Data�� Update�ϰ� ����� L/C�� �����Ѵ�.
	int	UpdateEqConstData(MEqConstant *pData);

	// �������� ECID �� ���Ͽ� EC Data�� Update�ϰ� ����� L/C�� �����Ѵ�.
	int UpdateEqConstDataList();

	// ��ü EC Data List�� L/C�� �����Ѵ�.
	int ReplyEqConstList();
	//_____________________________

	//Transfer���Ժο� �г��� �ִ���


	
	BOOL IsPanelExistInTransfer();
	//�ռ��񿡼� �Ѱ��� �г��� �ִ���
	BOOL IsPanelExistInPrevMachine();

	//SJ_YYK 101124 Add..
	void SetMaterialDoorCheck(BOOL bFlag, int iWorkGroup);
	//__________________________________*/

	int	CheckFeederEnable();

	BOOL GetStartSWStatus();
	BOOL GetStopSWStatus();
	BOOL GetResetSWStatus();
	BOOL GetEStopSWStatus();

	//110324_____
	BOOL IsAll1FeederAllMount_MountInitReady();
	BOOL IsAll1FeederAllMount_PanelAlignerInitReady();
	//___________

	//110808 SJ_KJS
	//110620.KKY_____
	void SetManualPlanMode(BOOL bOption)	{ m_bManualPlanMode = bOption; }
	BOOL IsManualPlanMode()					{ return m_bManualPlanMode; }

	int CheckProductValidation();
	int CheckProductPlanCnt();
	int CreateProductPlan();
	int UpdateProductPlan();
	int CreateMCodeInfo(MMaterialCodeInfo* pInfo);
	int MaterialCodeInfoRequest(int iPlanType, EMaterialStep eStep);
	//_______________

	//110624.KKY.Add_______________
	int		CheckExistMCode(int iPlanCode, EMaterialStep eStep, CString strMCode);
	void	SetLastPanelCurProductPlan(BOOL bLast);
	BOOL	GetLastPanelCurProductPlan();
	int		CheckLastPanelMCode();//Panel ����� ���� ���������� ������ Panel�̸� ��������(MCode)�� Transfer��Ų��.
	//_____________________________

	void UpdateProductInfoData();
	//_____________________________________---
	//111022 sj_kjs
	void IonaizerBlowOnOff(BOOL bSet);
	BOOL IsIonaizerBlowOnOff();
	//___________________________

	//120113.KMS________
	BOOL m_bModelChangeFlg;
	void SetModelChangeAlarmReport(BOOL bChange);
	BOOL GetModelChangeAlarmReport();
	//120116.kms_____________
	BOOL m_bMountAutoChangeFlg;
	void SetMountAutoChange(BOOL bChange);
	BOOL GetMountAutoChange();
	//_______________________


	void		SetMeasureProcessData(EWorkingSide eWorkSide,int iTabNo,double dAlignX,double dAlignY);
	double		m_dMeasProcessData[1000];

	// 091028 SJ_Chi, ���� Key Parameter ����
	//void		SetMeasureProcessData(BOOL bState){ m_bMeasureProcessData = bState; }
	BOOL		IsMeasureProcessData(){ return m_bMeasureProcessData; }
	int			MeasureProcessParameter();
	void		ProcessParamReport();
	void		SetMountBondingParameter(EWorkingSide eWorkSide,int iTabNo,SProcessParameter sProcessParameter);

	//120515.kms_________
	BOOL m_bAssemReportFlg;
	void SetAssemReport(BOOL bASReport);
	BOOL GetAssemReport();
	//20121004 SJ_HJG Ⱦ����
	void SetTabEndMarkDistance(int iWorkGroupNo, double dTabEndMarkDistance);
	double GetTabEndMarkDistance(int iWorkGroupNo);
	//___________________


	//@DoorLock____________
	BOOL IsFrontKeyAutoMode();
	BOOL IsRearKeyAutoMode();
	//150406 SJ_YSH
	BOOL IsFrontMaterialKeyAutoMode();
	BOOL IsRearMaterialKeyAutoMode();
	//_____________
	int	 LockFrontDoor();
	int	 LockRearDoor();
	int	 UnlockFrontDoor();
	int	 UnlockRearDoor();
	//@____________________

	//@___________________________
	BOOL	IsPanelEmpty_PrevMCUnload();				//@�ռ��� Unloading�ο� Panel�� ����ִ���
	void	SetPanelEmptyToPrevMC_In_MyLoading(BOOL bEmpty);	//@������ Loading�ο� Panel�� ����ִ���
	void	SetPanelEmptyToPrevMC_In_MyWorking(BOOL bEmpty);	//@������ Working�ο� Panel�� ����ִ���

	BOOL	IsPanelEmpty_NextMCLoad();
	void	SetPanelEmptyToNextMCLoad(BOOL bEmpty);
	

	BOOL	IsDoorClose_PrevMCUnload();
	void	SetDoorCloseToPrevMCUnload(BOOL bDoorClose);

	BOOL	IsDoorClose_NextMCLoad();
	void	SetDoorCloseToNextMCLoad(BOOL bDoorClose);

	BOOL	IsEnableToMovePrevMCWithPanel();
	BOOL	IsEnableToMoveNextMCWithPanel();

	//170928 JSH.s
	BOOL	IsOnPIO_PrevMC(EIfIOAddr eIfIO);
	BOOL	IsOffPIO_PrevMC(EIfIOAddr eIfIO);
	BOOL	IsOnPIO_NextMC(EIfIOAddr eIfIO);	
	BOOL	IsOffPIO_NextMC(EIfIOAddr eIfIO);
	//170928 JSH.e
	//@___________________________

	BOOL	IsDoorOpen_AOC();	//20171003 KDW Add
	//SJ_YYK 150109 Add.
	double  m_dTabCarrierAlignResult[DEF_MAX_WORKER_GROUP][DEF_MAX_WORKER_PER_GROUP];
	void	WriteTabCarrierAlignResult(int iGroup, int iWorkinstance, double dVal) {m_dTabCarrierAlignResult[iGroup][iWorkinstance] = dVal;}
	double	ReadTabCarrierAlignResult(int iGroup, int iWorkinstance) {return m_dTabCarrierAlignResult[iGroup][iWorkinstance];}

	BOOL		m_bAutoAlignDataUpdate; //SJ_YYK 150318 Add..
	BOOL		m_bSetAutoAlignPanelInStop; //SJ_YYK 150318 Add..

	void MatAOCAssembleReport(int nWorkGroup); //SJ_YYK 150821 Add..

	int getAssembleQty(int nWorkGroup); //SJ_YYK 150911 Add..
};
#endif //MTRSAUTOMANAGE_H
