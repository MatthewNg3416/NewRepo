//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for T7-2 OLB
// MLCNetData.h: interface for the MLCNetData class.
//
//
//		Version		Updated		 Author		 Note
//      -------     -------      ------      ----
//		   1.0       2005/12/20  	Grouchy    		Create	
//		   1.1       2006/03/16  	cha        		Data 구조 Update
//		   1.2       2006/08/17  	GrouchyKim      LC<==>설비간 시간 동기화 부분 추가
//		   1.3       2007/10/15  	GrouchyKim      LC<==>설비간 Alarm 동기화 부분 추가
//                                                  RCode(PM Code) 처리코드 수정
//                                                  Tabic Roll Reserved Port 처리코드 추가
//		   1.4       2007/11/07  	GrouchyKim      - LC<==>설비간 시간 동기화 부분 수정
//                                                  - SendString() 함수 쓰레드 동기화 문제로 수정
//		   1.5       2007/11/13  	GrouchyKim      - RCode(PM Code) 처리코드 수정
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MLCNET_H__63CC80FD_7A91_489C_99A6_70F7FA5E3BF7__INCLUDED_)
#define AFX_MLCNET_H__63CC80FD_7A91_489C_99A6_70F7FA5E3BF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MLCNetData.h"
#include "ICommon.h"
#include "MNetHData.h"
#include "Afxmt.h"

class MLCNetDlg;
class MSystemData;

#define MAX_UNIT		10

/**
 * Error Code Define
 */
const	int	ERR_MNET_SUCCESS				= 0;

class MLCNet : public virtual ICommon
{
private:
	//110629.KKY.Add_____
	CCriticalSection	m_csLock;

	//___________________

public:
	MLCNet(CString szIP, int nPort);
	virtual ~MLCNet();

public:
	/**	Initial Variable for XNet */
	CString			m_szIP;
	int				m_nPort;
	BOOL			m_bActive;
	BOOL			m_bSpool;

	bool			m_bConnected;

	/**	Common Variable for XSECNetDlg */
	MLCNetDlg*		m_pDlg;		//	Dialog Box Handle
	CWnd*			m_pMainWnd;	//	Main Window Handle
	BOOL			m_bShow;		//	Show type(true:Visible, false:Invisible)
    MSystemData*    m_plnkSystemData;

	EOpModeForLCNet	m_eOpMode;
	EEqState		m_eEqState;
	EEqProcState	m_eEqProcState;
	EMaterialPortState m_eMaterialPortState;

	//SS chang
	EEqSpecialState	m_eEqSpecialState;

	int				m_nMaxPanel;
	int				m_nUnit;
	CString			m_szOnlineMode;
	MPanel			m_rgPanel[MAX_UNIT];

	/**	Equipment Variable */
	CPtrList*		m_pSVList;		//	Equipment Status Variable(State Variable)
	CPtrList*		m_pECList;		//	Equipment Constant Variable
	CPtrList*		m_pEOList;		//	Equipment Online Variable

	/** PPID(ProcessProgram) */
	CPtrList*		m_pPPList;
	/** Current Product Plan. */
	MProductPlanBatchData	m_CurProductPlan;   // 2005/01/21, KJH
	MProductPlanBatchData	m_NextProductPlan;
	MProductPlanBatchData	m_EndProductPlan;

	/** Material Information.  */         // 2005/01/21, KJH
	MMaterial     m_MaterialData;
	// Receive Panel Data to L/C.
	MGlassDatas*			m_pReceivedPanelData;    

	//110620.KKY_____________
	/** Next Product Plan. */
	CPtrArray				m_caNextProductPlan;	//110629.KKY

	/** End Product Plan. */
	CPtrArray				m_caEndProductPlan;		//110629.KKY

	MMaterialCodeInfo		m_CurMaterialCodeInfo_S;
	MMaterialCodeInfo		m_NextMaterialCodeInfo_S;
	MMaterialCodeInfo		m_EndMaterialCodeInfo_S;

	MMaterialCodeInfo		m_CurMaterialCodeInfo_G;
	MMaterialCodeInfo		m_NextMaterialCodeInfo_G;
	MMaterialCodeInfo		m_EndMaterialCodeInfo_G;

	MMaterialCodeInfo		m_CurMaterialCodeInfo_ZS;
	MMaterialCodeInfo		m_NextMaterialCodeInfo_ZS;
	MMaterialCodeInfo		m_EndMaterialCodeInfo_ZS;

	MMaterialCodeInfo		m_CurMaterialCodeInfo_ZG;
	MMaterialCodeInfo		m_NextMaterialCodeInfo_ZG;
	MMaterialCodeInfo		m_EndMaterialCodeInfo_ZG;
//_
	//_______________________

	/** Material Event Parameter.   */            // 2005/01/22, KJH
	int         m_nWarningEvtLimitQty_TABIC;    // Material Warning Event
	int         m_nPreHandEvtLimitQty_TABIC;    // Material preHand Event
	int         m_nWarningEvtLimitQty_ZIG;      // Material Warning Event
	int         m_nPreHandEvtLimitQty_ZIG;      // Material preHand Event
	CString		m_sCurModelName;
	int         m_nAssembleQty_Feeder1;    
	int         m_nAssembleQty_Feeder2;    
	CString     m_sFeeder1MaterialType;    
	CString     m_sFeeder2MaterialType;    


	/** 수량정보가 Barcode에 없는 업체리스트이다. */
	CStringArray  m_srgNoParsingInfoMaker;
  
	/** L/C 재연결시 설비<=>L/C간의 데이터 동기화를 위한 부분. */
	MMaterial   m_oMat_Feeder1;
	MMaterial   m_oMat_Feeder2;
	MMaterial   m_oMat_ZigFeeder1;
	MMaterial   m_oMat_ZigFeeder2;

	//2007.10.15 GrouchyKim Add.[v1.3]
	MMaterial   m_oMat_Feeder3;
	MMaterial   m_oMat_Feeder4;

	CString m_strPM_MSG;

	/** RequestEquipmentCmd */
	CUIntArray		m_iaEquipmentCmd;
	CStringArray	m_saEquipmentCmd;
	CStringArray	m_saMultiTerminalCmd;

	/** RequestProcessCmd n*/
	CUIntArray		m_iaProcessCmd;
	CStringArray	m_saProcessCmd;

	/** RequestMaterialCmd */
	CUIntArray		m_iaMaterialCmd;
	CStringArray	m_saMaterialCmd;

	/** RequestOnlineMode */
	CUIntArray		m_iaOnlineCmdMode;//EOnlineModes

	/** RequestParamChange */
	CUIntArray		m_iaChangeParamType;//EParamType
	CPtrArray		m_caChangeParamStateVariable;//MStateVariable

	/** RequestProcessProgram */
	CUIntArray		m_iaProcessProgramMode;
	CPtrArray		m_caProcessProgram;			//MProcessProgram

	/** RequestProductPlan */
	CUIntArray		m_iaProductPlanCode;
	CPtrArray		m_caProductPlanBatchData;	//MProductPlanBatchData

	/** RequestBarCodeParsingInfo */
	CPtrArray		m_caBarCodeParsingInfo;		//MBarCodeParsingInfo

	//100807.ECID_______________________________
	//
	/** RequestEqConstChange */
	CUIntArray		m_iaEqConstCmd;		// 100817 SJ_Chi, EC Data Report
	CPtrArray		m_caEqConstData;
	//__________________________________________

	//110621.KKY________________________________
	/** Material CodeInfo*/
	CPtrArray		m_caMaterialCodeInfo;	//MMaterialCodeInfo
	//__________________________________________

	/** Setting LC Date & Time */         // 2006.08.17 GrouchyKim Add.
	bool          m_fUpdateLCDateTime;
	CStringArray	m_saLCDateTime;
	void          UpdateLCDateTime();

	//110629.KKY_____
	void		Lock();
	void		Unlock();
	void		ManageNextProductPlanData();
	void		ManageEndProductPlanData();
	void		TransferCurToEndProductPlanData();
	void		TransferNextToCurProductPlanData();
	void		TrsnaferCurMCodeToEndMCode();
	void		TrsnaferCurMCodeToEndMCodeJIG();
	void		UpdateCurrentMCodePlan(MMaterialCodeInfo* pInfo);
	void		UpdateNextMCodePlan(MMaterialCodeInfo* pInfo);
	//_______________

	//110812 SJ_KJS
	int			m_iPlanQty;
	CString		m_strReportID;
	void		SetUnitReportID(CString ReportID);
	//____________________________________

	//110817 SJ_KJS
	CString		m_strPauseMessage;

	CString		m_strPauseMessage2ND;

	CString		m_strMaker;
	//________________________


public:
	/**	Initial Method */
	long	Create();
	long	Start();
	long	Stop();
	void	SetUnitNo(int nUnit = 1);
	void	SetMaxPanel(int nMaxPanel);
	void	SetParamEC(MEqConstant* pData);
	void	SetParamSV(MStateVariable* pData);
	void	SetParamEO(MEqOnlineParam* pData);
	void	SetProcessProgram(CString szPPID, MProcessProgram* pPP);

	/**	Processing Method */
	long	PanelIdTransfer(int nPrevPos, int nCurPos, CString szPanelId);
	long	PanelTransferReport(EPanelEvent eEventType, CString szPanelId = "");
	long	PanelTransferReport(EPanelEvent eEventType, MPanel* p);
	long	InspectionReport(MPanel* pData, 
							 CString szRawPath, 
							 CString szSumPath, 
							 CString szImagePath, 
							 CString szDisk);
	long	EqStateReport(EEqState eEqState, CString sPMCode="NONE", int nAlarmID = -1, CString szAlarmText = "");
	long	ProcessProgramReport(int nMode, CString szPPID, MProcessProgram* pPP);
	long	MaterialReport(EMatEvent eEventType, MMaterial* pData);
	long	AlarmDataReport(int nOnOff, int nId, int nCode, CString szText);
	//100807.ECID________
	long	ParamECChangeReport(EEqConstCmd eEqConstCmd, MEqConstant* pData, int nCount = 1);
	long	ParamECReportAll(EEqConstCmd eEqConstCmd, int nSize);
	void	RequestEqConstChange(int nCmdID, MEqConstant* pEC = NULL);
	// DEL
	//long	ParamECChangeReport(EEditType eEdit, MEqConstant* pData);
	//___________________
	long	ParamSVChangeReport(EEditType eEdit, MStateVariable* pData);
	long	ParamEOChangeReport(EEditType eEdit, MEqOnlineParam* pData);
	long	MaterialPortStateReport(EMaterialPortState eMaterialPortState);
	long	EqProcessStateReport(EEqProcState eEqProcState, int nAlarmID = -1, CString szAlarmText = "");
	//100121 SJ_YYK EQ_STOP_DATA
	long	EqProcessStopReport(EEqProcState eEqProcState, int m_nCode, CString m_strItem);

	//SS chang
	//SJ_YYK 110421 Modify. Ellas
	//long	EqSpecialStateReport(EEqSpecialState eEqSpecialState);
	long	EqSpecialStateReport(EEqSpecialState eEqSpecialState, EMaterialType eMaterialType = eMaterial_Nothing, CString csDataHistory = "");
	long	EqSpecialStateClear();

	/**	Request(Event) Method */
	void	RequestEquipmentCmd(int nCmdId, CString szText);
	void	RequestProcessCmd(int nCmdId, CString szPanelId);
	void	RequestMaterialCmd(int nCmdId, CString szBatchId);
	void	RequestOnlineMode(CString szOnlineMode);
	void	RequestParamChange(EParamType eType, LPVOID lpvData);
	void	RequestProcessProgram(int nMode, MProcessProgram* pPP);
	void	RequestProductPlan(int nPlanCode, MProductPlanBatchData* pBI);
	void	RequestBarCodeParsingInfo(MBarCodeParsingInfo* p);
	void	RequestMaterialCodeInfo(MMaterialCodeInfo* p);	//110621.KKY.Add

	void	RequestMultiTerminalCmd(CString szText);

	// 091028 SJ_Chi, Data Collection Report 추가
	long	ProcessParamReport(MProcessParamInfo* pData, CString szPanelId);

	/** 	Trace Method */
	void	LogTrace(int nType, CString szLog);


	void	ViewVisible(BOOL bView);
	long	SendString(CString szMsg);


	bool	GetSubItem(CString sMsg, CStringArray &asList, int nType=1);

	CString GetPMCode();

	//NSMC KJS
	long	ModelChangeReport(EMCState eSts, CString strReasonCode = "");



	//110620.KKY.Add_____
	//설비에서 생산계획을 L/C에 요청한다...
	long	ProductPlanRequest(EProductPlanType ePlanType, MPlanRequestData *pPR);
	///설비에서 생산 계획의 시작과 종료를 L/C에 보고한다...
	long	ProductPlanReport(EProductPlanEvent ePlanEvent, MProductPlanBatchData *pBI);
	//___________________
	/** Product Code Report : Unit#1 - Vericode, Unit#2 - Barcode */
	long	ProductCodeReport(EPanelEvent eCodeType, EProductCodeState eCodeState, MProductCodeData *pPC);

private:
	/** Message Parse Function(Message to received from L/C). XML Format */
	CString GetItem_XMLParse(CString sBody, CString sTag);
	BOOL	GetItems_XMLParse(CString sBody, CString sTag, CStringArray &srgItems);
	int		FindStartIndex_XMLParse(CString sBody, CString sTag);
	int		FindEndIndex_XMLParse(CString sBody, CString sTag);
	
public:
	/** Anaysis Message to receiveed from L/C */
	void  AnaysisMessage_SelfFormat(CString sRcvMsg);
	void  AnaysisMessage_XMLFormat(CString sRcvMsg);

	/** Connection event handler */
	void  OnConnectionEvent();

public:
	/** Get current ProductPlan(Batch). */
	int   GetCurProductPlan(MProductPlanBatchData* pBI);     // 2005/01/21, KJH

	/** Get receive Material Information. */
	int   GetRcvMaterialInfo(MMaterial* pMat);				// 2005/01/22, KJH

	/** Get current Event Limit Qty(Material Warning)
	 * Material Warning Evnet: 설비의 자재 전체 수량이 설정된 값이 될때 발생하는 이벤트
	 */
	int   GetWarningLimitQty_TABIC();
	int   GetWarningLimitQty_ZIG();
	int   GetWarningLimitQty_PCB();

	/** Get current Event Limit Qty(Material PreHand)
	 * Material PreHand Evnet: 설비의 한 포트 자재 수량이 설정된 값이 될때 발생하는 이벤트
	 */
	int   GetPreHandLimitQty_TABIC();
	int   GetPreHandLimitQty_ZIG();
	int   GetPreHandLimitQty_PCB();

	/** Set current Event Limit Qty(Material Warning) */
	int   SetWarningLimitQty_TABIC(int nQty);
	int   SetWarningLimitQty_ZIG(int nQty);
	int   SetWarningLimitQty_PCB(int nQty);

	/** Set current Event Limit Qty(Material PreHand) */
	int   SetPreHandLimitQty_TABIC(int nQty);
	int   SetPreHandLimitQty_ZIG(int nQty);
	int   SetPreHandLimitQty_PCB(int nQty);

	void  ModelDataReport(CString sModelName, 
						  int nAssmbleQty_Feeder1, 
						  int nAssmbleQty_Feeder2, 
						  CString sFeeder1MaterialType,
						  CString sFeeder2MaterialType);
	void  ModelDataReport(CString sModelName);

	BOOL  Split(CString sStrLine, TCHAR chSep, CStringArray &asItem);
	BOOL  IsNoParsingInfoMaker(CString sMaker);

    //2007.10.15 GrouchyKim Add.[v1.3]
	int		m_nLastAlarmOnOff; 
	int		m_nLastAlarmId;
	int		m_nLastAlarmCode;
	CString m_strLastAlarmText; 
};

#endif // !defined(AFX_MLCNET_H__63CC80FD_7A91_489C_99A6_70F7FA5E3BF7__INCLUDED_)
