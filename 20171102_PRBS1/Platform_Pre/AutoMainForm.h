//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_AUTOMAINFORM_H__83801842_76D1_4184_BDAB_8B08832B0C89__INCLUDED_)
#define AFX_AUTOMAINFORM_H__83801842_76D1_4184_BDAB_8B08832B0C89__INCLUDED_

#include "MTickTimer.h"
#include "DefSystem.h"
#include "AutoRepairTabSelectDlg.h"
#include "AutoElapseTimeDlg.h"
#include "DlgSheetMaterial.h"
#include "ProductPlanDlg.h"
#include "DlgFixedVision.h" //171010 JSh


class MSystemData;
class MPanelData; //150527 SJ_YSH
class MProductInfoData;
class IVision;
class MManageOpPanel;
class MPanelTransfer;//2009.08.29 CYJ
class MTrsPanelTransferCenter;
class MTrsPanelAligner;
class MCtrlTabFeeder;
class MTrsAutoManager;
class MTrsLoadConveyor;
class MTrsPanelTransfer;
class CAutoStepDispDlg;
class CAutoNSMCInfoDlg;
class MLCNet;
//110929 SJ_KJS
class ICylinder;

#include "CommonOPCallDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoMainForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoMainForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//const int DEF_MAX_FIXED_VIEW		= 2;
const int DEF_MAX_FIXED_VIEW		= 6;
const int ERR_PC_REBOOTING_REQUEST	= 99;

class CAutoMainForm : public CFormView
{
protected:
	CAutoMainForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAutoMainForm)

// Form Data
public:
	//{{AFX_DATA(CAutoMainForm)
	enum { IDD = IDD_AUTO_MAIN_FORM };
	CListBox m_ListTerminalMsg;
	CBtnEnh	m_RunMode;
	CBtnEnh	m_OriginReturn;
	CBtnEnh	m_btnReady;
	CBtnEnh	m_bUnitInitial;
	CBtnEnh	m_bStepDisp;
	CBtnEnh	m_ctrlLblOpMode;
	CBtnEnh	m_ctrlLblModelName;
	CBtnEnh	m_ctrlLblEqTact;
	CBtnEnh	m_ctrlLblLineTact;
	CBtnEnh	m_ctrlLblProductQty;
	CBtnEnh	m_ctrlLbPanelInQty;
	CBtnEnh	m_ctrlLbPanelScrapQty;
	CBtnEnh	m_ctrlLblOpStatus;
	CBtnEnh	m_ctrlLblPreBondingTime;
	CBtnEnh	m_LblEqState;
	CBtnEnh	m_LblEqpState;
	CBtnEnh m_btnOperationGroup;
	CBtnEnh m_btnJigChangeCarrierUnload;
	CBtnEnh	m_btnMaterialGroup;
	CBtnEnh	m_ctrlLblFeeder1;
	CBtnEnh	m_ctrlLblFeeder2;
	CBtnEnh	m_ctrlMold1;
	CBtnEnh	m_ctrlMold2;
	CBtnEnh	m_lblProductInfo;
	CBtnEnh	m_lblRunningInfo;
	CBtnEnh	m_lblUnitInfo;
	CBtnEnh	m_lblInterlockMode;
	CBtnEnh m_btnFeeder1Ready;
	CBtnEnh m_btnFeeder2Ready;
	CBtnEnh m_BtnLCnet;
	CBtnEnh	m_ctrlLblDeviceID;
	CBtnEnh	m_ctrlLblAutoAlign;
	//}}AFX_DATA

	// NSMC
	CBtnEnh m_rgLblWorkerPanelID[DEF_MAX_PANEL_UNIT];
	CBtnEnh m_btnNSMCTabICChange;

// Attributes
public:

// Operations
public:
	void UnitInitialize(BOOL bDisplayMsg = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoMainForm)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;
	BOOL m_StepDispFlag;
	CAutoStepDispDlg* m_pDlg;
	CDlgSheetMaterial*	m_pDlgMaterial;//110621.KKY
	CProductPlanDlg*	m_pDlgProductPlan;//110624
	CDlgFixedVision*	m_pDlgFixedVision; //161102 //171010 JSh

	BOOL m_bTimeDispFlag;
	AutoElapseTimeDlg* m_pTimeDlg;
	int m_rgiViewID[DEF_MAX_FIXED_VIEW];

	// hongju_SESL
	CWnd *				m_pViewWnd;
	RECT				m_rect;

	MLCNet*				m_plnkLCNet;

	virtual ~CAutoMainForm();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CAutoMainForm)
	afx_msg void OnPaint();
	afx_msg LRESULT OnAutoStatus(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayRunMode(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayModelName(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayDeviceID(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayEqTact(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayLineTact(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayProductQty(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayTabFeederInfo1(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayTabFeederInfo2(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayOPCallMsg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayTerminalMsg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayEqState(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayEqpState(UINT wParam, LPARAM lParam);
	afx_msg void OnClickReady();
	afx_msg void OnRunMode();
	afx_msg void OnOriginReturn();
	afx_msg void OnUnitInitial();
	afx_msg void OnStepDisp();
	afx_msg void OnPanelInAndScrapCount();
	afx_msg void OnProductCount();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClickMelsecIf();
	afx_msg void OnClickMatchingTable();
	afx_msg void OnClickNSMCInfo();
	afx_msg LRESULT OnDisplayPanelID(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayPanelScrapDlg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayPanelUnscrapDlg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayMaterialInfoDlg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnStartTabFeederAutoHoming(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnMoveTabMounter(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnMoveInspectionCamera(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnRemoveTabIC(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayCylinderMoveTimeDlg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoUnitInitialize(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickLcnetIf();
	afx_msg void OnMaterialGroup();
	afx_msg void OnOperationGroup();
	afx_msg void OnClickPanelWait();
	afx_msg LRESULT OnDisplayLanguage(UINT wParam, LPARAM lParam);
	afx_msg LRESULT	OnAllInspectCamMoveAlign(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickNSMCTabICChange();
	afx_msg void OnClickPmMode();
	afx_msg void OnClickReelready1();
	afx_msg void OnClickReelready2();
	afx_msg LRESULT OnShowMaterialCodeInfoDlg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnShowProductPlanDlg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDispProductPlan(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDispMaterialCodeInfo(UINT wParam, LPARAM lParam);
	afx_msg void OnClickModelName();
	afx_msg void OnClickModelDeviceId();
	afx_msg void OnClickUnitTact();
	afx_msg void OnClickJigChangeCarrierUnload();
	afx_msg void OnClickTablossLogF();
	afx_msg void OnClickTablossLogR();
	afx_msg LRESULT OnDisplayPanelInQty(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayPanelScrapQty(UINT wParam, LPARAM lParam);
	afx_msg void OnClickOperationGroup2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int	m_iInitialMode;	
	int m_uiTimerID;
	MSystemData* m_plnkSystemData;
	MPanelData* m_plnkPanelData; //150527 SJ_YSH
	MProductInfoData* m_plnkProductInfoData;
	MTrsAutoManager* m_plnkTrsAutoManager;
//KKY.Del	MTrsLoadConveyor* m_plnkTrsLoadConveyor;
#ifdef DEF_USE_TURN_HANDLER
	MTrsPanelTransferCenter*	m_plnkTrsPanelTransferCenter;
#endif
	MTrsPanelTransfer*		m_plnkTrsPanelTransfer;
	MTrsPanelAligner*		m_plnkTrsPanelAligner;
	IVision* m_plnkVision;
	MCtrlTabFeeder* m_plnkCtrlTabFeeder[DEF_MAX_TABFEEDER];
#ifdef SIMULATION
	MManageOpPanel* m_plnkManageOpPanel;
#endif

	/** Repair Select Dlg */
	CAutoRepairTabSelectDlg m_dlgRepairTabSelect;

	/** Operator Call */
	CCommonOPCallDlg m_dlgOperatorCall;

	//NSMC_KJS : NSMCdlg 추가
	BOOL m_bNSMCInfoFlag;
	CAutoNSMCInfoDlg* m_pNSMCInfoDlg;

private:
	void PeekAndPump();
	void enableButton(BOOL bFlag);

	int initializeAutoRun();
	void initializeProcessData();
	void resetMoldCount(int iFeederNo);
	int removeAllTabIC();

	static int sm_iRemoveTabICMode;//0->버리지 않음. 1->Mounter만, 2->Mounter & Carrier 모두
	// 자동운전 초기화 Thread 종료 확인
	static int sm_rgiTabMounterInitResult[DEF_MAX_WORKER_GROUP];
	static int sm_rgiInspectionCameraInitResult[DEF_MAX_WORKER_GROUP];
	static int sm_rgiTabCarrierInitResult;
	static int sm_rgiRTabCarrierInitResult;
	static int sm_rgiTabFeederInitResult;
	static int sm_rgiRTabFeederInitResult;
//KKY.Del	static int sm_rgiConveyorInitResult[DEF_MAX_CONVEYOR];
	static int sm_rgiPanelTransferCenterInitResult;
	static int sm_rgiPanelTransferOutInitResult;
	static int sm_rgiTurnHandlerInitResult; //SJ_YYK 161104 Add.

	static int sm_rgbTabMounterInitComplete[DEF_MAX_WORKER_GROUP];
	static int sm_rgbInspectionCameraInitComplete[DEF_MAX_WORKER_GROUP];
	static int sm_rgbTabCarrierInitComplete;
	static int sm_rgbRTabCarrierInitComplete;
	static int sm_rgbTabFeederInitComplete;
	static int sm_rgbRTabFeederInitComplete;
//KKY.Del	static int sm_rgbConveyorInitComplete[DEF_MAX_CONVEYOR];
	static int sm_rgbPanelTransferCenterInitComplete;
	static int sm_rgbPanelTransferOutInitComplete;

	static int sm_rgbTurnHandlerInitComplete; //SJ_YYK 161104 Add.


	// 자동운전 초기화 Thread 생성 함수
	static UINT initTabMounter(LPVOID pParam);
	static UINT initInspectionCamera(LPVOID pParam);
	static UINT initTabCarrier(LPVOID pParam);
	static UINT initRTabCarrier(LPVOID pParam);
	static UINT initTabFeeder(LPVOID pParam);
	static UINT initRTabFeeder(LPVOID pParam);
	static UINT initPanelTransfer(LPVOID pParam);
	static UINT initPanelTransferOut(LPVOID pParam);
#ifdef DEF_USE_TURN_HANDLER
	static UINT initTurnHandler(LPVOID pParam); //SJ_YYK 161104 Add..
#endif
	// 기타
	static int sm_rgiRemoveResult[2];
	static int sm_rgbRemoveComplete[2];
	static UINT removeTabIC(LPVOID pParam);
public:
	void ClickInterlockMode();

	//SESL_요구사항적용
	void ClickMoldCount1();
	void ClickMoldCount2();

	//120201.kms________
	BOOL m_AFlgTransferCenter;
	BOOL m_AFlgTurnHandler;
	BOOL m_AFlgTransferIn;
	BOOL m_AFlgPanelAligner;
	BOOL m_AFlgTransferOut;
	void OnDisplayPanelAbsorbState();
	//__________________
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOMAINFORM_H__83801842_76D1_4184_BDAB_8B08832B0C89__INCLUDED_)
