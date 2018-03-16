//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for T7-2 PCB
// MLCNetData.h: interface for the MLCNetData class.
//
//
//		Version		Updated		 Author		 Note
//      -------     -------      ------      ----
//		   1.0       2005/12/20  Grouchy    Create	
//		   1.1       2006/03/16  cha		
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MLCNETDATA_H__58421562_E755_4F43_8C2D_1A0E660F97B3__INCLUDED_)
#define AFX_MLCNETDATA_H__58421562_E755_4F43_8C2D_1A0E660F97B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DefSystemConfig.h"

//	User defined value
#define	_MAX_DISPLAY_MSG			50
#define _MAX_LENGTH					256

//	User defined messages
#define WMU_INIT_MEMBERS		(WM_USER + 1)	//	Initialize members
#define WMU_MLCNET_ONLINE		(WM_USER + 2)	//	OnOnlineMode
#define WMU_MLCNET_PROCESS		(WM_USER + 3)	//	OnProcessCmd
#define WMU_MLCNET_EQUIPMENT	(WM_USER + 4)	//	OnEquipmentCmd
#define WMU_MLCNET_BATCH		(WM_USER + 5)	//	OnProductPlan(Batch)
#define WMU_MLCNET_PARAMETER	(WM_USER + 6)	//	OnParameterChg
#define WMU_MLCNET_PPID			(WM_USER + 7)	//	OnPPIDExcute
#define WMU_MLCNET_MATERIAL		(WM_USER + 8)	//	OnMaterialCmd
#define WMU_MLCNET_PARSING_INFO	(WM_USER + 9)	//	OnBarCodeParsingInfo

#define WMU_DISPLAY_MSG			(WM_USER + 10)		


//New Online___________
#if defined(DEF_PLATFORM_ARP_SYSTEM)
//#define DEF_MCODE_STEP_S		_T("OLB_ACF")
//#define DEF_MCODE_STEP_G		_T("OLB_ACF")
#elif defined(DEF_PLATFORM_PREBONDER_SYSTEM)
#define DEF_MCODE_STEP_S		_T("TABICS")
#define DEF_MCODE_STEP_G		_T("TABICG")
#elif DEF_PLATFORM_FINALBONDER_SYSTEM
//#define DEF_MCODE_STEP_S		_T("OLB_FNB")
//#define DEF_MCODE_STEP_G		_T("OLB_FNB")
#endif
//_____________________


//	User defined enum variable
enum EPPIDModeType
{
	ePPIDNothing = 0,
	ePPIDCreate = 1,
	ePPIDModify,
	ePPIDDelete,
	ePPIDChange
};

enum EEditType
{
	eNothing = 0,
	eCreate = 1,
	eModify,
//110629.KKY.Del	eDelete,
	eChange
};

enum EProcCmd
{
	eProcCmdNothing = 0,
	eProcCmdStart = 1,
	eProcCmdCancel,
	eProcCmdAbort
};

enum EMatCmd
{
	eMatCmdNothing = 0,
	eMatCmdStart = 1,
//	eMatCmdAbort,
//	eMatCmdCancel,
	eMatCmdCancel,
	eMatCmdAbort,
	eMatCmdUpdate,//kjs
	eMatCmdQty//kjs
};

enum EEqCmd
{
	eEqCmdNothing = 0,
	eEqCmdPause = 51,
	eEqCmdResume = 52,
	eEqCmdChgPM = 53,
	eEqCmdChgNormal = 54,
	eEqCmdTerminalMsg = 81,
	eEqCmdOperatorCall = 82,
	eEqCmdLastGlassCancel = 90,
	eEqCmdJobStart = 91,  // 정상 셀이므로 투입 하라는 명령
	eEqCmdJobCancel = 92, // 비정상 셀이므로 셀 제거 하라는 명령
	eEqCmdRcvPanelData = 100   // PanelData 도착 했다는 case 번호
};

//100807.ECID__________
enum EEqConstCmd
{
	eRequestNothing = 0,
	eInitRequest = 1,
	eChangeRequest
};
//_____________________

enum EParamType
{
	eParamNothing = 0,
	eEqStatus = 1,
	eEqConstant,
	eEqOnline
};

enum EOnlineMode
{
	eOnNothing = 0,
	eOffline = 1,
	eLocal,
	eRemote
};

enum EOpModeForLCNet
{
	eOpNothing = 0,
	eAuto = 1,
	eManual
};


enum EEqState
{
	eEqNothing	= 0,
	eNormal		= 1,
	eFault		= 2,
	ePM			= 3
};

enum EEqProcState
{
	eEqPNothing = 0,
	eInit		= 1,
	eIdle		= 2,
	eSetup		= 3,
	eReady      = 4,
	eExecute    = 5,
	ePause      = 6
};

enum EMaterialPortState
{
	eMaterialPortNothing	= 0,
	eMEmpty					= 1,		
	eMIdle					= 2,
	eMWait					= 3,
	eMResrve				= 4,
	eMBusy					= 5,
	eMComplete				= 6,
	eMAbort					= 7,
	eMCancel				= 8,
	eMPause					= 9,
	eMDisable				= 10
	
};

enum EPanelEvent
{
	ePanelNothing = 0,
	ePanelRemove = 14, 
	ePanelUnScrap = 15, 
	ePanelIn = 16,
	ePanelOut = 17,
	ePanelVcrRead = 18,
	ePanelBcrRead = 19,
	ePanelTrackIn = 20,
	ePanelTrackOut = 21
};

enum EMatEvent
{
	eMatNothing 		= 0,
	eMatInfoRequest 	= 93,
	eMatSupplementReq 	= 301,
	eMatStockIn 		= 302,
	eMatDockIn 			= 303,//Reel 장착
	eMatConsumeStart 	= 304,//Roll의 첫번째 타발
	eMatAssemble 		= 305,//Assemble Event
	eMatConsumeEnd 		= 306,//Roll의 마지막 타발
	eMatDockOut 		= 307,//Reel 제거
	eMatStockOut 		= 308,
	eMatBatchCancel 	= 309,
	eMatNG 				= 310,
	eMatWarning 		= 311,//교체 알람
	eMatShortage 		= 312,//완전 소모 
	eMatLocationUpdate 	= 313,
	eMatUserCheck		= 314,//사용자 자재 확인
	eMatPrehand			= 320,
	eMatAutoChange		= 330,
};

enum EMatType
{
	eMatTypeNothing = 0,
	eSourceLeftTab = 1,
	eSourceRightTab,
	eGateLeftTab,
	eGateRightTab,
	eSourcePcb,
	eGatePcb
};

enum eMState
{
	eMatStateIdle = 1,
	eMatStateEnable,
	eMatStateRun,
	eMatStateHold,
	eMatStateDisable,
	eMatStateRemove 
};

//NSMC KJS
enum EMCState
{
	eMCStart = 54,
	eMCComplete = 55,
};

////////////////////////////////////////////////
enum EEqSpecialState 
{
	ePanelSupplyStopEnd = 11,
	ePanelSupplyStopStart,
	ePanelSupplyStopEndNSMC,
	ePanelSupplyStopStartNSMC,	
	eMaterialChangeStart,
	eMaterialChangeEnd,
	eProgramStart,
	eProgramEnd,
	eProcessConditionChangeStart, 
	eProcessConditionChangeEnd,
	eGlassIn,
	eGlassOut,
	eIrregularPM,
	eAlignCheck,
	eBondingCheck,
	eSiNotHardeningCheck,	//Si미경화 체크
	eSiNotAppliedCheck,		//Si미도포 체크
	eEqCleaning,
	ePCBSupplyStopStart,
	ePCBSupplyStopEnd,
	eUpperEquipPauseStart,
	eUpperEquipPauseEnd,
	eManualModeStart,
	eAutoRunModeStart,//34 X
	eUpperSendAbleStart,
	eUpperSendAbleEnd,
	eLowerReceiveAbleStart,
	eLowerReceiveAbleEnd,
	eOneTouchModelChange	//20111103 
};

enum EMaterialType
{
	eMaterial_Nothing = 10,
	eACF_ACF,
	eACF_SeperateTape,
	eACF_Sheet,
	ePre_Reel,
	ePre_Mold,
	eFinal_Sheet,
	ePCB_ACF,
	ePCB_PCB,
	ePCB_Sheet,
	ePCB_SeperateTape,

	eACF_ACF_Auto,
	ePre_Reel_Auto,
	ePCB_ACF_Auto
};

enum EProductCodeState
{
	ePanelReadFail = 111,
	ePanelMisMatch = 113,
	ePanelCodeRead = 133
};

enum EProductPlanType
{
	eProductPlanRequest = 91,
	eMaterialInfoRequest = 93,
	eProductPlan46CodeUpdate = 98,	//100614 SJ_Chi, OLB 4-6 Code, 모듈 속성정보 요청
};

enum EProductPlanEvent
{
	eProductPlanBatchStart = 21,
	eProductPlanBatchCancel = 22,
	eProductPlanBatchAbort = 23,
	eProductPlanBatchEnd = 24,
//	eProductPlan46CodeUpdate = 98 //SJ_YYK 100616 4-6 Code 추가..
};

enum EProductPlanState
{
	eProductPlanNone = -1,
	eProductPlanCreate = 0,
	eProductPlanBusy,
	eProductPlanEnd
};

enum EMaterialKind
{
	eMatKind_NONE				= -1,
	eMatKind_POL				= 11,
	eMatKind_JIG				= 12,
	eMatKind_TABIC				= 13,
	eMatKind_PCB				= 14,
	eMatKind_BL					= 15,
	eMatKind_TC					= 16,
	eMatKind_ACF				= 17,
	eMatKind_COG				= 18,
	eMatKind_ACFBondingSheet	= 19,
};

enum EMaterialStep
{
	eMatStep_S	= 1,
	eMatStep_G	= 2,
};
//_______________

class MProductPlanBatchData
{
public:
		MProductPlanBatchData();
		~MProductPlanBatchData();
public:
	//110627.KKY_____
	int			m_nPlanQty;
	//_______________
	bool		m_bRunFlag;
	CString		m_sModuleID;
	int			m_nPlanOrderID;
	int			m_nPriorty;
	CString		m_sProdKind;
	CString		m_sProdType;
	CString		m_sDeviceID;
	CString		m_sStepID;
	CString		m_sPPID;
	CString		m_sBatchID;
	int			m_nBatchSize;		//최초 수량
	int			m_nBatchSizeLast;	//New Online. 최종 수량 
	int			m_nBatchState;
	CString		m_sPanelMaker;
	int			m_nPanelThickness;
	CString		m_sFirstGlassID;
	CString		m_sCurGlassID;
	CString		m_sInputLineNo;
	int			m_nValidFlag;
	int			m_nCQTY;
	int			m_nOQTY;
	int			m_nRQTY;
	int			m_nNQTY;

	//110803 SJ_KJS
	int			m_nPlanOrderID2;
	int			m_nPriorty2;
	CString		m_sProdKind2;
	CString		m_sProdType2;
	CString		m_sDeviceID2;
	CString		m_sStepID2;
	CString		m_sPPID2;
	CString		m_sBatchID2;
	int			m_nBatchSize2;
	int			m_nBatchSizeLast2;	//SJ_HJG
	int			m_nBatchState2;
	CString		m_sPanelMaker2;
	int			m_nPanelThickness2;
	CString		m_sFirstGlassID2;
	CString		m_sCurGlassID2;
	CString		m_sInputLineNo2;
	int			m_nValidFlag2;
	int			m_nCQTY2;
	int			m_nOQTY2;
	int			m_nRQTY2;
	int			m_nNQTY2;
	//_______________________

public:
	void	SetData(MProductPlanBatchData* p);
	void	SetData2N1(MProductPlanBatchData* p); //110803 SJ_KJS
	void	Clear();
};

class MBatchInfo 
{
public:
	MBatchInfo();
	virtual ~MBatchInfo();
public:
	CString 	m_strPLCD;				// Plan Code
	int		 	m_nOrderNo;				// Plan Order ID
	CString		m_strMakerID;			// Material Maker ID
	CString		m_strProdType;			// PP/EE/MT/PC/TE
	CString		m_strProdKind;			// TF/CF/PD
	int		 	m_nThickness;			// Panel Thickness
	CString		m_strBatchID;			// Batch ID
	CString		m_strInputLIne;			// Input Line No(L1, L2)
	CString		m_strDeviceID;			// ProcessID or PartID
	CString		m_strStepID;			// Step ID
	CString		m_strPPID;				// Process Program ID
	int			m_nPlanCount;			// Plan Count
	int			m_nPriority;			// 1=High, 2=Middle, 3=Low
	int			m_nOutCount;			// Out Count
	int			m_nRemainCount;			// In Equipment Count
	int			m_nWorkFlag;			// 1=Run, 2=Wait, 3=Pause
	CString		m_strFirstPanelID;		// First Glass ID
	CString		m_strCurPanelID;		// Current Glass ID
	int			m_nValidFlag;			// 1=OK, 2=NG
};

class MDevice 
{
public:
	MDevice();
	virtual ~MDevice();
public:
	CString			m_strProcessId;		// Related Process ID with the Material
	CString			m_strPartId;		// Related Part ID with the Material
	CString			m_strStepID;		// Related Step ID with the Material
	CString			m_strPPID;			// Related PPID with the Material

public:
	void	CopyData(MDevice* p);
};

class MMaterial 
{
public:
	MMaterial();
	virtual ~MMaterial();
public:
	EMatEvent		m_eMatEvt;

	CString 		m_sPORTID;			// Material Port ID
	CString 		m_sEQP_STATE;		// Eq State
	CString			m_sPORT_STATE;		// Port State
	CString			m_sPORT_TYPE;		// Port Type(Input/Output/Both)
	CString			m_sCSTID;			// 자재 반송 매체 ID
	int				m_nMAP_STIF;		// Mapping 정보
	int				m_nCUR_STIF;		// UnLoading 시 Mapping 정보
	CString			m_sM_BATCHID;		// Material Batchid
	CString			m_sM_MAKER;			// Material Maker
	int				m_nM_KIND;			// Material Kind
	int				m_nM_TYPE;			// Material Type
	int				m_nM_STATE;			// Material State
	int				m_nT_QTY;			// 자재 전체 수량
	int				m_nU_QTY;			// 사용된 수량
	int				m_nR_QTY;			// 남은 수량
	int				m_nN_QTY;			// NG 누적 수량
	int				m_nA_QTY;			// Assemble  자재 수량
	int				m_nE_FLAG;			// 자재 완료되었을 때, 자투리 인지 유무
	CString			m_sC_CODE;			// Cancel Code
	CString			m_sM_STEP;			// 자재 종류 구분 
	int				m_nS_QTY;			// 자재 요청 수량
	CString			m_sM_CODE;			// Material Defect Code

public:
	void	CopyData(MMaterial* p);
	void	ResetData();
};

class MEqConstant 
{
public:
	MEqConstant();
	virtual ~MEqConstant();
public:
	//100807.ECID_________________________
	CString		m_strECID;				// Equipment Constant ID
	//int		m_nECID;				// Equipment Constant ID
	CString		m_strECName;			// Equipment Constant Name
	CString		m_strECDef;				// Equipment Constant Set Value
	CString		m_strECSLL;				// Equipment Transfer Stop Low Limit
	CString		m_strECSUL;				// Equipment Transfer Stop Upper Limit
	CString		m_strECWLL;				// Equipment Warning Low Limit
	CString		m_strECWUL;				// Equipment Warning Upper Limit

public:
	void	CopyData(MEqConstant* p);
	//100807.ECID_________________________
	void	ResetData();
};

class MEqOnlineParam 
{
public:
	MEqOnlineParam();
	virtual ~MEqOnlineParam();
public:
	int				m_nEOID;			// Equipment Online Parameter ID
	int				m_nEOMD;			// Equipment Online Parameter Mode
	int				m_nEOV;				// Equipment Online Parameter Value

public:
	void	CopyData(MEqOnlineParam* p);
};

class MStateVariable 
{
public:
	MStateVariable();
	virtual ~MStateVariable();
public:
	int			m_nSVID;				// State Variable ID
	CString		m_strSV;				// State Variable 
	CString		m_strSVName;			// State Variable Name

public:
	void	CopyData(MStateVariable* p);
};

class MPPBody 
{
public:
	MPPBody();
	virtual ~MPPBody();
public:
	CString			m_strPParamName;		// Process Parameter Name
	CString			m_strPParamValue;		// Process Parameter Value

public:
	void	CopyData(MPPBody* p);
};

class MCCode 
{
public:
	MCCode();
	virtual ~MCCode();
public:
	int				m_nCCode;				// Each Command Code
	int 			m_nPPBodyQty;			// Number of PPBody
	MPPBody*		m_paPPBoby;			// PPID Body Class

public:
	void	CopyData(MCCode* p);
	void	SetCount(int nItemCnt);
};

class MProcessProgram 
{
public:
	MProcessProgram();
	virtual ~MProcessProgram();
public:
	CString			m_strPPID;				// Process Program ID
	int 			m_nCCodeQty;			// Number of CCode
	MCCode*			m_paCCode;				// CCode Class

public:
	void	CopyData(MProcessProgram* p);
	void	SetCount(int nItemCnt);
};

class MPanel 
{
public:
	MPanel();
	virtual ~MPanel();
public:
	CString			m_strHPanelID;		// Ascil  12byte					
	CString			m_strEPanelID;		// Ascil  12byte
	CString			m_strBatchID;		// Ascil  12byte				
	CString			m_strProdType;		// Ascil  2byte
	CString			m_strProdKind;		// Ascil  2byte
	CString			m_strDeviceID;		// Ascil  18byte
	CString			m_strStepID;		// Ascil  10byte
	CString			m_strPPID;			// Ascil  16byte
	CString			m_strThickness;		// Binary 2byte
	CString			m_strInsFlag;		// Ascil  2byte
	CString			m_strPanelSize;		// Binary 4byte
	CString			m_strPanelPosition;	// Ascil  2byte
	CString			m_strCount1;		// Ascil  2byte
	CString			m_strCount2;		// Ascil  2byte
	CString			m_strGrade;			// Bits	  6byte
	CString			m_strComment;		// Ascil  16byte
	CString			m_strCompSize;		// Binary 2byte
	CString			m_strReadingFlag;	// Ascil  2byte
	CString			m_strPanelState;	// Binary 2byte
	CString			m_strJudgement;		// Ascil  4byte
	CString			m_strCode;			// Ascil  4byte
	CString			m_strRunLine;		// Binary 20byte
	CString			m_strUniqueId;		// Binary 4byte
	CString			m_strPairHPanelID;	// Ascil  12byte
	CString			m_strPairEPanelID;	// Ascil  12byte
	CString			m_strPairGrade;		// Bits   6byte
	CString			m_strPairUniqueId;	// Binary 4byte
	CString			m_strFlowRecipe;	// Ascil  2byte
	CString			m_strReserved0;		// Any    12byte
	CString			m_strBitsSignals;	// Bits   2byte
	CString			m_strReferData;		// Any    4byte
	
public:
	void	CopyData(MPanel *p);
	void	ResetData();
};

class MAlarm 
{
public:
	MAlarm();
	virtual ~MAlarm();
public:
	bool			m_bAlarmSet;		// Alarm Set/Reset				

	int				m_nAlarmId;			// Alarm ID
	int				m_nAlarmCode;		// Alarm Code
	CString			m_sAlarmText;		// Alarm Text

public:
	void	CopyData(MAlarm* p);
};

class MBarCodeParsingInfo 
{
public:
	MBarCodeParsingInfo();
	virtual ~MBarCodeParsingInfo();
public:
	CString			m_sMaker;				// Maker

	int				m_nStartIndex_ID;		// ID Start Position 
	int				m_nEndIndex_ID;			// ID End Position 
	int				m_nStartIndex_Qty;		// Qty Start Position
	int				m_nEndIndex_Qty;		// Qty End Position

public:
	void	CopyData(MBarCodeParsingInfo* p);
};


//110620.KKY_____
class MPlanRequestData
{
public:
	MPlanRequestData();
	virtual ~MPlanRequestData();

	CString		m_sLINE_NO;
	CString		m_sEQPID;
	CString		m_sCarrierID;
	CString		m_sDeviceID;
	int			m_nM_KIND;
	CString		m_sM_STEP;

public:
	void	CopyData(MPlanRequestData* p);
};


class MProductCodeData // Vericode, Barcode
{
public:
	MProductCodeData();
	virtual ~MProductCodeData();

public:
//	int			m_iUnitID; // 1:Loader, 2:Labeller
	CString		m_strCSTID; // Carrier ID : Current Batch ID
	int			m_iCSTTYPE;
	int			m_iReq_Qty;
	CString		m_strPanelID;

public:
	void	CopyData(MProductCodeData* p);
};


//________________________________________________________________________________
// 091028 SJ_Chi, Data Collection Report 추가
typedef struct 
{
	CString		m_strParamName;
	double		m_dSettingValue;
	double		m_dMeasureValue;
	int         m_iIonizerState;//120508.kms

	BOOL		m_bUseParam;
} SProcessParamInfo;

class MProcessParamInfo
{
public:
	MProcessParamInfo();
	virtual ~MProcessParamInfo();

public:
	int					m_iDataCount;				// Key Parameter Data Count
	SProcessParamInfo	m_rgsProcessParam[1000];		// Key Parameter Data Struct

//	char	m_rgszParamName[30][20];	// Key Parameter Data Name
//	int		m_rgiSettingValue[30];		// Key Parameter Data Setting 값
//	int		m_rgiMeasureValue[30];		// Key Parameter Data 측정 값

public:
	void	CopyData(MProcessParamInfo* p);

};
//_______________________________________________________________________________
class MMaterialCodeInfo
{
public:
	MMaterialCodeInfo();
	virtual ~MMaterialCodeInfo();
	
	int				m_nWorkOrder;
	CString			m_sDeviceID;
	EMaterialKind	m_nM_KIND;
	CString			m_sM_STEP;
	CStringArray	m_asM_CODE;
public:
	void	CopyData(MMaterialCodeInfo* p);
	void	ResetData();
};
//_______________

#endif // !defined(AFX_MLCNETDATA_H__58421562_E755_4F43_8C2D_1A0E660F97B3__INCLUDED_)
