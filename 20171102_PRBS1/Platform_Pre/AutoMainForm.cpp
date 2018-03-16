/**
 * T7 OLB PreBonder System
 *
 * Copyright 2005 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * AutoMainForm.cpp: This file implements the AutoMainForm View.
 *
 * @author Dooyoung Jeong (dy20.jung@samsung.com)
 * @version $Revision$
 */
  
#include "stdafx.h"
#include <Tlhelp32.h>	// ACS Spii plus Kill Reload
//SESL_LKH_090706
#include "DefSystemConfig.h"
#include "DefIOAddrPreBonder.h"

#include "MProductInfoData.h"
#include "MPreBonderData.h"
#include "MPanelData.h" //150527 SJ_YSH
#include "MPlatformOlb.h"
#include "MainFrm.h"
#include "IVision.h"
#include "IAxis.h"
#include "MPlatformOlbSystem.h"
#include "BottomForm.h"
#include "AutoMainForm.h"
#include "AutoStepDispDlg.h"
#include "AutoModeSelectDlg.h"
#include "AutoMotorOriginDlg.h"
#include "MProcessData.h"

#include "AutoStepDispDlg.h"
#include "AutoMaterialInfoDlg.h"
#include "AutoScrapGlassDlg.h"
#include "AutoMaterialCmdGroupDlg.h"
#include "AutoOperationCmdGroupDlg.h"
#include "MaterialChangeDlg.h" //101119 SJ_YYK
#include "MNetH.h"
#include "MLCNet.h"

#include "MPanelAligner.h"
#include "MTabMounter.h"
#include "MTabCarrier.h"
#include "MTabFeeder.h"
#include "MTabAligner.h"
#include "MCameraCarrier.h"
#include "MPanelTransfer.h" // 2009.08.29 CYJ
#include "MModelChange.h" //150527 SJ_YSH
#include "MHandler.h"
// selsk_jdy
#include "IOpPanel.h"

#include "DBTabLoss.h"

#ifdef SIMULATION
#	include "MManageOpPanel.h"
#endif
#include "MCtrlTabFeeder.h"

#include "MTrsAutoManager.h"
#include "MTrsPanelTransfer.h"
#include "MTrsPanelTransferCenter.h"
#include "MTrsPanelAligner.h"
#include "MTrsPanelTransferOut.h"
#include "MTrsPanelAligner.h"
#include "MTrsLoadConveyor.h"
#include "MTrsUnloadConveyor.h"
#include "MTrsTabMounter.h"
#include "MTrsInspectionCamera.h"
#include "MTrsTabCarrier.h"
#include "MTrsTabFeeder.h"
#include "MTrsTHandler.h" //SJ_YYK 161104 Add...

// jdy sesl for test
#include "atlconv.h"

// syr add..090402
#include "AutoDoorStateDlg.h"
#include <math.h> // 2009.04.15 SESL_CYJ

//NSMC KJS
#include "AutoNSMCInfoDlg.h"
#include "DataMatchingTableDlg.h"
#include "SystemThreeBtnDlg.h"

#include "EqStopCodeDlg.h"
#include "PMFullDisplay.h"

#include "MLCNet.h"
#include "ModelNDeviceIDListDlg.h"

#include "DlgSheetTactTime.h"
#include "DlgTabICLoss.h"

//110929 SJ_KJS
#include "ICylinder.h"
#include "MCtrlTabFeeder.h"
#include "MDebug.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

int CAutoMainForm::sm_iRemoveTabICMode = FALSE;//0->버리지 않음. 1->Mounter만, 2->Mounter & Carrier 모두
int CAutoMainForm::sm_rgiTabMounterInitResult[DEF_MAX_WORKER_GROUP];
int CAutoMainForm::sm_rgiInspectionCameraInitResult[DEF_MAX_WORKER_GROUP];
int CAutoMainForm::sm_rgiTabCarrierInitResult;
int CAutoMainForm::sm_rgiRTabCarrierInitResult;
int CAutoMainForm::sm_rgiTabFeederInitResult;
int CAutoMainForm::sm_rgiRTabFeederInitResult;
//int CAutoMainForm::sm_rgiConveyorInitResult[DEF_MAX_CONVEYOR];
int CAutoMainForm::sm_rgiPanelTransferCenterInitResult;
int CAutoMainForm::sm_rgiPanelTransferOutInitResult;
int CAutoMainForm::sm_rgiTurnHandlerInitResult; //SJ_YYK 161104 Add..

int CAutoMainForm::sm_rgbTabMounterInitComplete[DEF_MAX_WORKER_GROUP];
int CAutoMainForm::sm_rgbInspectionCameraInitComplete[DEF_MAX_WORKER_GROUP];
int CAutoMainForm::sm_rgbTabCarrierInitComplete;
int CAutoMainForm::sm_rgbRTabCarrierInitComplete;
int CAutoMainForm::sm_rgbTabFeederInitComplete;
int CAutoMainForm::sm_rgbRTabFeederInitComplete;
//int CAutoMainForm::sm_rgbConveyorInitComplete[DEF_MAX_CONVEYOR];
int CAutoMainForm::sm_rgbPanelTransferCenterInitComplete;
int CAutoMainForm::sm_rgbPanelTransferOutInitComplete;
int CAutoMainForm::sm_rgbTurnHandlerInitComplete;  //SJ_YYK 161104 Add..

int CAutoMainForm::sm_rgiRemoveResult[2];
int CAutoMainForm::sm_rgbRemoveComplete[2];

//NSMC KJS
#include "CommonSelectUserGroupDlg.h"

IMPLEMENT_DYNCREATE(CAutoMainForm, CFormView)

/*
 * CAutoMainForm 생성자
 */
CAutoMainForm::CAutoMainForm()
	: CFormView(CAutoMainForm::IDD)
{
	//{{AFX_DATA_INIT(CAutoMainForm)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	//_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	//::_CrtIsValid

	m_StepDispFlag	= FALSE;
	m_pDlg			= new CAutoStepDispDlg;
	
	m_bTimeDispFlag	= FALSE;
	m_pTimeDlg		= new AutoElapseTimeDlg;

	//110621.KKY_____
	m_pDlgMaterial = NULL;
	//_______________
	
	m_pDlgProductPlan = NULL;//110624

	m_pDlgFixedVision = NULL; //161102

	for (int i = 0; i < DEF_MAX_FIXED_VIEW; i++)
	{
		m_rgiViewID[i] = -1;
	}

	//NSMC_KJS : NSMCdlg 추가
	m_bNSMCInfoFlag			= FALSE;
	m_pNSMCInfoDlg			= new CAutoNSMCInfoDlg;	
	
	m_plnkLCNet = MOlbSystemPre.GetLCNetComponent();
	
	//120201.kms________
	m_AFlgTransferCenter = FALSE;
	m_AFlgTurnHandler = FALSE; //170123 SJ_YSH Add.
	m_AFlgTransferIn = FALSE;
	m_AFlgPanelAligner = FALSE;
	m_AFlgTransferOut = FALSE;
	//__________________
}

/*
 * CAutoMainForm 소멸자
 */
CAutoMainForm::~CAutoMainForm()
{
	m_StepDispFlag	= FALSE;
	if (m_pDlg != NULL)
		delete m_pDlg;

	m_bTimeDispFlag	= FALSE;
	if (m_pTimeDlg != NULL)
		delete m_pTimeDlg;

	//110621__________________
	if (m_pDlgMaterial != NULL)
		delete m_pDlgMaterial;
	//________________________
	//110624__________________
	if (m_pDlgProductPlan != NULL)
		delete m_pDlgProductPlan;
	//________________________
	
	//161102
	if (m_pDlgFixedVision != NULL)
		delete m_pDlgFixedVision;

	//NSMC_KJS : NSMCdlg 추가
	m_bNSMCInfoFlag	= FALSE;
	if (m_pNSMCInfoDlg != NULL)
		delete m_pNSMCInfoDlg;
}

void CAutoMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoMainForm)
	DDX_Control(pDX, IDS_TERMINAL_DISPLAY, m_ListTerminalMsg);
	DDX_Control(pDX, IDB_RUN_MODE, m_RunMode);
	DDX_Control(pDX, IDB_ORIGIN_RETURN, m_OriginReturn);
	DDX_Control(pDX, IDC_READY, m_btnReady);
	DDX_Control(pDX, IDB_UNIT_INITIAL, m_bUnitInitial);
	DDX_Control(pDX, IDB_STEP_DISP, m_bStepDisp);
	DDX_Control(pDX, IDC_G_OPERATE_MODE, m_ctrlLblOpMode);
	DDX_Control(pDX, IDC_MODEL_NAME, m_ctrlLblModelName);
	DDX_Control(pDX, IDC_EQ_TACT_TIME, m_ctrlLblEqTact);
	DDX_Control(pDX, IDC_LINE_TACT_TIME, m_ctrlLblLineTact);
	DDX_Control(pDX, IDC_PRODUCT_COUNT, m_ctrlLblProductQty);
	DDX_Control(pDX, IDC_PANEL_IN_QTY, m_ctrlLbPanelInQty);
	DDX_Control(pDX, IDC_PANEL_SCRAP_QTY, m_ctrlLbPanelScrapQty);
	DDX_Control(pDX, IDC_S_OPERATE_MODE, m_ctrlLblOpStatus);
	DDX_Control(pDX, IDC_PREBONDING_TIME, m_ctrlLblPreBondingTime);
	DDX_Control(pDX, IDC_EQ_STATE, m_LblEqState);
	DDX_Control(pDX, IDC_EQP_STATE, m_LblEqpState);
	DDX_Control(pDX, IDB_OPERATION_GROUP, m_btnOperationGroup);
	DDX_Control(pDX, IDB_JIG_CHANGE_CARRIER_UNLOAD, m_btnJigChangeCarrierUnload);
	DDX_Control(pDX, IDB_MATERIAL_GROUP, m_btnMaterialGroup);
	DDX_Control(pDX, IDC_FEEDER1, m_ctrlLblFeeder1);
	DDX_Control(pDX, IDC_FEEDER2, m_ctrlLblFeeder2);
	DDX_Control(pDX, IDC_MOLD1, m_ctrlMold1);
	DDX_Control(pDX, IDC_MOLD2, m_ctrlMold2);
	DDX_Control(pDX, IDC_PRODUCT_INFO, m_lblProductInfo);
	DDX_Control(pDX, IDC_CTR_INFO, m_lblRunningInfo);
	DDX_Control(pDX, IDC_UNIT_INFO, m_lblUnitInfo);
	DDX_Control(pDX, IDC_INTERLOCK_MODE, m_lblInterlockMode);
	DDX_Control(pDX, IDB_REELREADY1, m_btnFeeder1Ready);
	DDX_Control(pDX, IDB_REELREADY2, m_btnFeeder2Ready);
	DDX_Control(pDX, IDB_LCNET_IF, m_BtnLCnet);
	DDX_Control(pDX, IDC_MODEL_DEVICE_ID, m_ctrlLblDeviceID);
	DDX_Control(pDX, IDB_AUTO_ALIGN_SYSTEM, m_ctrlLblAutoAlign);

	

	//}}AFX_DATA_MAP

	// NSMC

#ifdef DEF_SOURCE_SYSTEM
	DDX_Control(pDX, IDC_LOAD_CONVEYOR, m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_CENTER_PANEL]); //SJ_YYK 161104 Add..
	DDX_Control(pDX, IDC_TURN_HANDLER, m_rgLblWorkerPanelID[DEF_THANDLER_PANEL]);

	DDX_Control(pDX, IDC_PANEL_TRANSFER, m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_PANEL]);
	DDX_Control(pDX, IDC_PANEL_ALIGNER, m_rgLblWorkerPanelID[DEF_PANEL_ALIGNER_PANEL]);
	DDX_Control(pDX, IDC_UNLOAD_CONVEYOR, m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_OUT_PANEL]);
#else
	DDX_Control(pDX, IDC_PANEL_ALIGNER, m_rgLblWorkerPanelID[DEF_PANEL_ALIGNER_PANEL]);
	DDX_Control(pDX, IDC_UNLOAD_CONVEYOR, m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_OUT_PANEL]);
#endif
	DDX_Control(pDX, IDB_AUTO_MATERIAL_CHANGE, m_btnNSMCTabICChange);
}


BEGIN_MESSAGE_MAP(CAutoMainForm, CFormView)
	//{{AFX_MSG_MAP(CAutoMainForm)
	ON_WM_PAINT()
	ON_MESSAGE(WM_AUTO_STATUS, OnAutoStatus)
	ON_MESSAGE(WM_DISP_RUN_MODE, OnDisplayRunMode)
	ON_MESSAGE(WM_DISP_MODEL_NAME, OnDisplayModelName)
	ON_MESSAGE(WM_DISP_MODEL_DEVICEID, OnDisplayDeviceID)
	ON_MESSAGE(WM_DISP_EQ_TACK_TIME, OnDisplayEqTact)
	ON_MESSAGE(WM_DISP_LINE_TACK_TIME, OnDisplayLineTact)
	ON_MESSAGE(WM_DISP_PRODUCT_QTY, OnDisplayProductQty)

	// Doolin kwangilshin 2017. 09. 12.
	//
	ON_MESSAGE(WM_DISP_PANEL_IN_QTY, OnDisplayPanelInQty)
	ON_MESSAGE(WM_DISP_PANEL_SCRAP_QTY, OnDisplayPanelScrapQty)
	// End.

	ON_MESSAGE(WM_DISP_TABFEEDER1_INFO, OnDisplayTabFeederInfo1)
	ON_MESSAGE(WM_DISP_TABFEEDER2_INFO, OnDisplayTabFeederInfo2)
	ON_MESSAGE(WM_DISP_OPCALL_MSG, OnDisplayOPCallMsg)
	ON_MESSAGE(WM_DISP_TERMINAL_MSG, OnDisplayTerminalMsg)
	ON_MESSAGE(WM_DISP_EQ_STATE, OnDisplayEqState)
	ON_MESSAGE(WM_DISP_EQP_STATE, OnDisplayEqpState)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_DISP_PANEL_ID, OnDisplayPanelID)
	ON_MESSAGE(WM_DISP_PANEL_SCRAP_DLG, OnDisplayPanelScrapDlg)
	ON_MESSAGE(WM_DISP_PANEL_UNSCRAP_DLG, OnDisplayPanelUnscrapDlg)
	ON_MESSAGE(WM_DISP_MATERIAL_INFO_DLG, OnDisplayMaterialInfoDlg)
	ON_MESSAGE(WM_START_TAB_FEEDER_AUTO_HOMING_REQ, OnStartTabFeederAutoHoming)
	ON_MESSAGE(WM_MOVE_TAB_MOUNTER_REQ, OnMoveTabMounter)
	ON_MESSAGE(WM_REMOVE_TABIC_REQ, OnRemoveTabIC)
	ON_MESSAGE(WM_DISP_CYLINDER_MOVE_TIME_DLG, OnDisplayCylinderMoveTimeDlg)
	ON_MESSAGE(WM_MOVE_INSPECTION_CAMERA_REQ, OnMoveInspectionCamera)
	ON_MESSAGE(WM_DISP_MULTI_LANGUAGE_DLG_REQ, OnDisplayLanguage)
	ON_MESSAGE(WM_ALL_INSPECT_CAM_MOVE_ALIGN, OnAllInspectCamMoveAlign)
	ON_MESSAGE(WM_AUTO_UNIT_INITIALIZE, OnAutoUnitInitialize)			//110324

	ON_MESSAGE(WM_SHOW_MATERIAL_CODEINFO_DLG, OnShowMaterialCodeInfoDlg)
	ON_MESSAGE(WM_SHOW_PRODUCT_PLAN_DLG, OnShowProductPlanDlg)

	ON_MESSAGE(WM_DISP_PRODUCT_PLAN_MSG, OnDispProductPlan)
	ON_MESSAGE(WM_DISP_MATERIAL_CODEINFO, OnDispMaterialCodeInfo)

	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoMainForm diagnostics

#ifdef _DEBUG
void CAutoMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CAutoMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAutoMainForm message handlers

BOOL CAutoMainForm::PreTranslateMessage(MSG* pMsg) 
{
#ifdef SIMULATION
	if (pMsg->message == WM_KEYDOWN)
    {
		switch (pMsg->wParam)
		{
		case VK_F4:
			m_plnkManageOpPanel->SetSimulationStartSW(TRUE);
			TRACE0("\nKey Down : Simulation Start SW\n");
			break;
		case VK_F5:
			m_plnkManageOpPanel->SetSimulationStopSW(TRUE);
			TRACE0("\nKey Down : Simulation Stop SW\n");
			break;
		case VK_F6:
			m_plnkManageOpPanel->SetSimulationResetSW(TRUE);
			TRACE0("\nKey Down : Simulation Reset SW\n");
			break;
		default:
			break;
		}
	}
	else
	{
		m_plnkManageOpPanel->SetSimulationStartSW(FALSE);
		m_plnkManageOpPanel->SetSimulationStopSW(FALSE);
		m_plnkManageOpPanel->SetSimulationResetSW(FALSE);
	}
#endif
	return CFormView::PreTranslateMessage(pMsg);
}

LRESULT CAutoMainForm::OnAutoStatus(UINT wParam, LPARAM lParam)
{
	if (wParam == MANUAL_MODE)
		enableButton(TRUE);
	
	m_ctrlLblOpStatus.SetForeColor(BLACK);
	if (wParam == MANUAL_MODE)
		m_ctrlLblOpStatus.SetCaption(_T("MANUAL"));
	else
	{
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TURN_HANDLER
		if (TRUE == m_plnkTrsPanelTransferCenter->IsPanelInStop()
#	else
		if (TRUE == m_plnkTrsPanelTransfer->IsPanelInStop()
#	endif
#else
		if (TRUE == m_plnkTrsPanelAligner->IsPanelInStop()
#endif
			|| TRUE == m_plnkTrsAutoManager->IsPanelInStopNSMC())	//101007.KKY.NSMC
		{
			m_ctrlLblOpStatus.SetCaption(_T("Panel In Stop"));
			m_ctrlLblOpStatus.SetForeColor(RED);
		}
		else
			m_ctrlLblOpStatus.SetCaption(_T("AUTORUN"));
	}
	
	if (FALSE == m_plnkTrsAutoManager->GetNSMCTabICChange())
		m_btnNSMCTabICChange.SetBackColorInterior(BASIC);
	else
		m_btnNSMCTabICChange.SetBackColorInterior(CYAN);
	
	return 0;
}

void CAutoMainForm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);

#ifndef VISION_SIMULATION
	for (int i = 0; i < DEF_MAX_FIXED_VIEW; i++)
	{
		m_plnkVision->RedrawView(m_rgiViewID[i]);
	}
#endif

	// Do not call CFormView::OnPaint() for painting messages
}

BEGIN_EVENTSINK_MAP(CAutoMainForm, CFormView)
    //{{AFX_EVENTSINK_MAP(CAutoMainForm)
	ON_EVENT(CAutoMainForm, IDC_READY, -600 /* Click */, OnClickReady, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_RUN_MODE, -600 /* Click */, OnRunMode, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_ORIGIN_RETURN, -600 /* Click */, OnOriginReturn, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_UNIT_INITIAL, -600 /* Click */, OnUnitInitial, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_STEP_DISP, -600 /* Click */, OnStepDisp, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_PANEL_IN_SCRAP_QTY, -600 /* Click */, OnPanelInAndScrapCount, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_PRODUCT_COUNT, -600 /* Click */, OnProductCount, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_MELSEC_IF, -600 /* Click */, OnClickMelsecIf, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_LCNET_IF, -600 /* Click */, OnClickLcnetIf, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_MATERIAL_GROUP, -600 /* Click */, OnMaterialGroup, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_OPERATION_GROUP, -600 /* Click */, OnOperationGroup, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_PANEL_WAIT, -600 /* Click */, OnClickPanelWait, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_PM_MODE, -600 /* Click */, OnClickPmMode, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_REELREADY1, -600 /* Click */, OnClickReelready1, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_REELREADY2, -600 /* Click */, OnClickReelready2, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_MODEL_NAME, -600 /* Click */, OnClickModelName, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_MODEL_DEVICE_ID, -600 /* Click */, OnClickModelDeviceId, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_UNIT_TACT, -600 /* Click */, OnClickUnitTact, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_JIG_CHANGE_CARRIER_UNLOAD, -600 /* Click */, OnClickJigChangeCarrierUnload, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_TABLOSS_LOG_F, -600 /* Click */, OnClickTablossLogF, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_TABLOSS_LOG_R, -600 /* Click */, OnClickTablossLogR, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_OPERATION_GROUP2, -600 /* Click */, OnClickOperationGroup2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CAutoMainForm, IDB_INTERLOCK_MODE, DISPID_CLICK, CAutoMainForm::ClickInterlockMode, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_MATCHING_TABLE, DISPID_CLICK, CAutoMainForm::OnClickMatchingTable, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_NSMC_INFO, DISPID_CLICK, CAutoMainForm::OnClickNSMCInfo, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_AUTO_MATERIAL_CHANGE, DISPID_CLICK, CAutoMainForm::OnClickNSMCTabICChange, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_MOLD_COUNT1, DISPID_CLICK, CAutoMainForm::ClickMoldCount1, VTS_NONE)
	ON_EVENT(CAutoMainForm, IDB_MOLD_COUNT2, DISPID_CLICK, CAutoMainForm::ClickMoldCount2, VTS_NONE)
END_EVENTSINK_MAP()

void CAutoMainForm::enableButton(BOOL bFlag)
{
	// 자동운전으로 들어갈때 Bottom View를 Disable 시키기 위함
	CMainFrame*	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CBottomForm * pBottomView = (CBottomForm*)pMainFrame->m_wndSplitter.GetPane(2,0); 

	m_ListTerminalMsg.EnableWindow(bFlag);
	m_RunMode.EnableWindow(bFlag);
	m_OriginReturn.EnableWindow(bFlag);
	m_btnReady.EnableWindow(bFlag);
	m_bUnitInitial.EnableWindow(bFlag);
	m_btnOperationGroup.EnableWindow(bFlag);
	m_btnJigChangeCarrierUnload.EnableWindow(bFlag);
//@	m_btnOperationGroup.EnableWindow(FALSE);

	if (bFlag)
	{
		if (m_plnkSystemData->m_eRunMode != REPAIR_RUN_MODE)
			pBottomView->OnManual();
	}
	else
	{
		pBottomView->OnAutoRun();	// Bottom Navigiation View Disable
	}

//	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		m_btnFeeder1Ready.EnableWindow(bFlag);
		m_btnFeeder2Ready.EnableWindow(bFlag);
	}
}

void CAutoMainForm::OnClickReady()
{
// hongju_SESL
#ifndef VISION_SIMULATION
	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS1);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[0] = m_plnkVision->CreateFixedView(0, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS2);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[1] = m_plnkVision->CreateFixedView(1, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS3);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[2] = m_plnkVision->CreateFixedView(2, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS4);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[3] = m_plnkVision->CreateFixedView(3, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS5);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[4] = m_plnkVision->CreateFixedView(4, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS6);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[5] = m_plnkVision->CreateFixedView(5, &m_rect, this);
#endif
	
	m_pDlgFixedVision->CreateFixedView(); //161102 //171010 JSh

	//130218.kms_____________
	//Bad Tab Count Interlock
	/*
	if(MOlbSystemPre.GetCtrlTabFeederComponent(0)->GetBadTabCntInterlock() == TRUE)
	{
		MyMessageBox(_T("Front Feeder의 BadCount 가 계산값과 다릅니다. 확인해 주세요."),_T("Bad Count Error"), M_ICONERROR);
		return;
	}

	if(MOlbSystemPre.GetCtrlTabFeederComponent(1)->GetBadTabCntInterlock() == TRUE)
	{
		MyMessageBox(_T("Rear Feeder의 BadCount 가 계산값과 다릅니다. 확인해 주세요."),_T("Bad Count Error"), M_ICONERROR);
		return;
	}
	//_______________________*/

	
#ifndef DEF_SETUP

	//110817 SJ_KJS
	if (MOlbSystemPre.GetLCNetComponent()->m_strPauseMessage2ND == "Eq Pause")
//		|| m_pDoc->GetAutoManagerCOM()->GetLCNetComponent()->m_strPauseMessage2ND != "Eq Resume")
	{
		if (MOlbSystemPre.GetLCNetComponent()->m_strPauseMessage2ND != "NONE")
		{
			MyMessageBox("Not Start.\r\n"
				"Host로부터 가동에 대한 명령을 수신하지 못하였습니다.\nIt can't recive order about operation from Host.",
				"Error", M_OK);
			return;
		}

	}
	//__________________________________


	/*/
	if (MOlbSystemPre.GetIOComponent()->IsOff(OUT_DOOR_LOCK_OPENCLOSE)) //Source = 2023, Gate = 2062
	{
		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_DOOR_LOCK_OPENCLOSE);
			MOlbSystemPre.GetIOComponent()->OutputOn(OUT_FRONT_MATERIALCHANGE_DOOR_LOCK);
			MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_MATERIALCHANGE_DOOR_LOCK);

		Sleep(200);

		if (MOlbSystemPre.GetIOComponent()->IsOff(OUT_DOOR_LOCK_OPENCLOSE))
		{

			CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
			return;
		}
		m_lblDoorLock.SetBackColorInterior(RGB(170,255,85));
	}
	//SJ_YYK 111004 Add...
	if (MOlbSystemPre.GetIOComponent()->IsOff(OUT_FRONT_MATERIALCHANGE_DOOR_LOCK)) //Source = 2023, Gate = 2062
	{
		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_FRONT_MATERIALCHANGE_DOOR_LOCK);

		Sleep(200);

		if (MOlbSystemPre.GetIOComponent()->IsOff(OUT_FRONT_MATERIALCHANGE_DOOR_LOCK))
		{

			CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
			return;
		}
		m_lblDoorLock.SetBackColorInterior(RGB(170,255,85));
	}
	if (MOlbSystemPre.GetIOComponent()->IsOff(OUT_REAR_MATERIALCHANGE_DOOR_LOCK)) //Source = 2023, Gate = 2062
	{
		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_MATERIALCHANGE_DOOR_LOCK);

		Sleep(200);

		if (MOlbSystemPre.GetIOComponent()->IsOff(OUT_REAR_MATERIALCHANGE_DOOR_LOCK))
		{

			CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
			return;
		}
		m_lblDoorLock.SetBackColorInterior(RGB(170,255,85));
	}
	//____________________________________________
	/*/
	/*/
	if (m_plnkTrsAutoManager->IsDoorOpenFront())
	{
		MyMessageBox(_T("Front Door가 Open되어 있습니다."), _T("Confirm"));
		return;
	}
	if (m_plnkTrsAutoManager->IsDoorOpenRear())
	{
		MyMessageBox(_T("Rear Door가 Open되어 있습니다."), _T("Confirm"));
		return;
	}
	/*/
	if (TRUE == m_plnkSystemData->m_bSafetySensor
		&& m_plnkTrsAutoManager->IsDoorOpen())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		// hongju_SESL : Door Open 에러 발생하면..Door 화면 보여주기
		ClickInterlockMode();
		return;	
	}
	if (FALSE == MOlbSystemPre.IsAllDoorLock(DEF_FRONT_GROUP))
	{
		MyMessageBox(_T("The front door is not locked.\r\n전면 Door가 Lock되어 있지 않습니다."), _T("Confirm"));
		return;
	}

	if (FALSE == MOlbSystemPre.IsAllDoorLock(DEF_REAR_GROUP))
	{
		MyMessageBox(_T("The rear door is not locked.\r\n후면 Door가 Lock되어 있지 않습니다."), _T("Confirm"));
		return;
	}

	//131111. SJ_YTH
	if (FALSE == m_plnkTrsAutoManager->IsFrontKeyAutoMode())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300008000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("KeyAuto Error"), M_ICONINFORMATION);
		return;	
	}

	if (FALSE == m_plnkTrsAutoManager->IsRearKeyAutoMode())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300009000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("KeyAuto Error"), M_ICONINFORMATION);
		return;	
	}
	
		//170123 SJ_YSH
#ifndef DEF_SETUP
	// 자동운전 시작 전 전후 설비 Door Open 확인 하도록.  Setup간 목지훈D 요청 사항
	if(FALSE == m_plnkTrsAutoManager->IsDoorClose_PrevMCUnload())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300010000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Pre Machine Door Open"), M_ICONINFORMATION);
		return;	
	}
	
	if(FALSE == m_plnkTrsAutoManager->IsDoorClose_NextMCLoad())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300011000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Next Machine Door Open"), M_ICONINFORMATION);
		return;	
	}

	//20171004 KDW Start
		//170127 SJ_YSH
	// 자동운전 중 AOC 설비 Door Open 감지 시 설비 정지 하도록.  Setup간 이기환D 요청 사항
	//if(FALSE == MOlbSystemPre.GetMelsecComponent2()->IsOnUpperEqBit(eReserved5_UpperEqBit))
	//{
//		CString strErr = MOlbSystemPre.GetErrorMessage(300012000);
//		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("AOC Machine Door Open"), M_ICONINFORMATION);
//		return;	
 //	}
	Sleep(300);
	if (m_plnkTrsAutoManager->IsDoorOpen_AOC())
	{
		int iResult = 300013000;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("AOC Door Open Error"), M_ICONINFORMATION);
		return;
	}
	//20171004 KDW End
	//_____________

#endif




#endif
	//150527 SJ_YSH
#ifdef DEF_SOURCE_SYSTEM
	//if(m_plnkPanelData->m_dPanelSize_X > (DEF_PANEL_SIZE_78INCH + 5))
	if(m_plnkPanelData->m_dPanelSize_Y > (DEF_PANEL_SIZE_78INCH_Y + 5))
	{
		if(MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->IsForAirRailFB1() == FALSE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(230016000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		if(MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->IsForAirRailFB2() == FALSE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(230018000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
	else
	{
		if(MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->IsBackAirRailFB1() == FALSE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(230017000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		if(MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->IsBackAirRailFB2() == FALSE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(230019000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
#endif
	//_____________
	
	//@130204.KKY________________
	int iResult = SUCCESS;
	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
		MOlbSystemPre.GetTabMounterComponent(i)->UpPusherCyl(TRUE);
		MOlbSystemPre.GetRTabMounterComponent(i)->UpPusherCyl(TRUE);
	}
	iResult = MOlbSystemPre.GetTabMounterComponent(0)->UpPusherCyl();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	m_plnkTrsAutoManager->SetMaterialDoorCheck(FALSE, DEF_FRONT_GROUP);
	m_plnkTrsAutoManager->SetMaterialDoorCheck(FALSE, DEF_REAR_GROUP);

	m_plnkTrsAutoManager->m_bFeederChange = FALSE;

	if (m_plnkSystemData->m_eRunMode == PASS_RUN_MODE)
	{
    //Pass Run Mode 로 진행합니다.계속 진행하시겠습니까?
		if (IDNO == MyMessageBox(MY_MESSAGE_BOX, 20000, _T("Confirm"), M_ICONQUESTION|M_YESNO))
			return;
	}

	//170927 JSH.s
	if(m_plnkSystemData->m_eRunMode != PASS_RUN_MODE && m_plnkSystemData->m_bUseBondingAfterImgLog == TRUE)
	{
		if (IDNO == MyMessageBox(MY_MESSAGE_BOX, 20020, _T("Confirm"), M_ICONQUESTION|M_YESNO))
			return;
	}
	//170927 JSH.e

	iResult = m_plnkTrsAutoManager->AutoReady();
	if (iResult != ERR_TRS_AUTO_MANAGER_SUCCESS)
	{
		// jdy sesl		MyMessageBox(iResult, _T("Start Ready"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	enableButton(FALSE);

	// jdy sesl 081230	CString szTextTemp;
	CString strMsg;
	//OP Panel의 START Key로 자동운전을 시작합니다.STOP Key로 자동운전 MODE를 해제합니다.
	//ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_1"), &szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_1"), &strMsg);
	/*
	CString strMsg = szTextTemp;
	
	WCHAR szMsg[512];
	lstrcpy(szMsg, strMsg.GetBuffer(strMsg.GetLength()));
	*/

	// selsk_jdy
	//::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_DISP_MY_MESSAGE, TRUE, (LPARAM)szMsg);
	::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(), WM_DISP_MY_MESSAGE, TRUE, (LPARAM)(strMsg.GetBuffer(strMsg.GetLength())));
}

void CAutoMainForm::OnRunMode() 
{
	ERunMode eOldRunMode = m_plnkSystemData->m_eRunMode;

	CAutoModeSelectDlg dlg;
	dlg.DoModal();
	OnPaint();

	if (eOldRunMode == m_plnkSystemData->m_eRunMode)
		return;

	CMainFrame*	pMainFrame;
	pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CBottomForm* pBottomView = (CBottomForm*)pMainFrame->m_wndSplitter.GetPane(2,0); 

	if (m_plnkSystemData->m_eRunMode != REPAIR_RUN_MODE)
	{
		m_dlgRepairTabSelect.Terminate();
		m_dlgRepairTabSelect.ShowWindow(SW_HIDE);

		pBottomView->OnManual();

/*		if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
		{
			m_btnFeeder1Ready.EnableWindow(TRUE);
			m_btnFeeder2Ready.EnableWindow(TRUE);
		}
		else
		{
			m_btnFeeder1Ready.EnableWindow(FALSE);
			m_btnFeeder2Ready.EnableWindow(FALSE);
		}
*/
		if (NORMAL_RUN_MODE == m_plnkSystemData->m_eRunMode)
		{
			enableButton(FALSE);

			// Pass Run 이거나 Dry Run 에서 Normal Run 으로 전환하는 경우는 TabIC 가 걸려 있지 않은 상황일 수도 있으므로
			// 자동운전 초기화를 바로 수행할 것인지 확인한다.
			// Repair Run 에서 Normal Run 으로 전환할 경우에는 무조건 자동운전 초기화를 수행한다.
			if (PASS_RUN_MODE == eOldRunMode || DRY_RUN_MODE == eOldRunMode)
			{
				//초기화를 진행하시겠습니까?
				int iResult = MyMessageBox(MY_MESSAGE_BOX, 20001, _T("Confirm"), M_ICONQUESTION|M_YESNO);
				if (IDNO == iResult)
				{
					enableButton(TRUE);
					return;
				}
			}

			// selsk_jdy unitInitialize();
			UnitInitialize();

			enableButton(TRUE);
		}
	}
	else
	{
		m_dlgRepairTabSelect.Start();
		m_dlgRepairTabSelect.ShowWindow(SW_SHOW);

		pBottomView->OnAutoRun();
		
		int iResult = removeAllTabIC();
		if (iResult && NOT_USE_WORKSIDE != iResult)
		{
			if (USER_STOP == iResult)
			{
			  CString szTextTemp;
				//사용자 요청에 의해 작업을 중단함.
				ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_2"), &szTextTemp);
				CString strMsg = szTextTemp;
				return;
			}
			else if (THERE_IS_NO_TAB_IC == iResult)
				return;

			CString strTemp;
			if (ERR_PC_REBOOTING_REQUEST == iResult)
			{
				CString szTextTemp;
				//PC 오동작 중.프로그램 종료 후 PC Rebooting 해 주세요.
				ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_3"), &szTextTemp);
				CString strTemp = szTextTemp;

				m_plnkTrsAutoManager->WriteMonitoringLog("Can not do Normalcy Thread Stop(During AutoRun Init).");
			}
			else
				strTemp = MOlbSystemPre.GetErrorMessage(iResult);

			MyMessageBox(strTemp, _T("Error"), M_ICONERROR);
			return;
		}
	}

	//170928 JSh.s
	// Doolin kwangilshin 2017. 09. 16.
	//
	if(DRY_RUN_MODE == eOldRunMode)
	{
		for (int nUnitCnt = 0; nUnitCnt < DEF_MAX_PANEL_UNIT; nUnitCnt++)
		{
			m_rgLblWorkerPanelID[nUnitCnt].SetBackColor(STATIC_BACKGROUND);
			m_plnkTrsAutoManager->m_pGlassData[nUnitCnt]->ResetData();
			::SendMessage(m_plnkTrsAutoManager->GetAutoViewHandle(), WM_DISP_PANEL_ID, (WPARAM)nUnitCnt, NULL);
		}
	}
	// End.
	//170928 JSh.e
}

void CAutoMainForm::OnOriginReturn() 
{
#ifndef DEF_SETUP
	if (TRUE == m_plnkTrsAutoManager->IsDoorOpen())
	//@___________________________
	{
		/* jdy sesl
		// 300003 = 문이 열린 상태 입니다. 
		MyMessageBox(300003, _T("Error"), M_ICONERROR);
		*/
		int iResult = 300003000;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;
	}
	//_______________________________________________-
	//20171004 KDW Start
	Sleep(300);
	if (m_plnkTrsAutoManager->IsDoorOpen_AOC())
	{
		int iResult = 300013000;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("AOC Door Open Error"), M_ICONINFORMATION);
		return;
	}
	//20171004 KDW End
#endif
	MOlbSystemPre.GetLCNetComponent()->EqProcessStateReport(eInit);
	OnDisplayEqpState((WPARAM)eInit, NULL);

	CAutoMotorOriginDlg dlg;
	dlg.DoModal();

	MOlbSystemPre.GetLCNetComponent()->EqProcessStateReport(ePause);
	OnDisplayEqpState((WPARAM)ePause, NULL);
}

void CAutoMainForm::OnUnitInitial() 
{
	// selsk_jdy
//MERGE_LKH_090704 : SESL_조재근S 버전 적용
//090701 shim 임시로
//	if (TRUE == MOlbSystemPre.GetOpPanelComponent()->GetSafeDoorStatus())
	//2009.07.14 CYJ
	
	//@___________________________
	/*/
	if (TRUE == m_plnkSystemData->m_bSafetySensor
		&& TRUE == MOlbSystemPre.GetOpPanelComponent()->GetSafeDoorStatus()
		&& TRUE == MOlbSystemPre.GetIOComponent()->IsOn(OUT_FRONT_DOOR_LOCK_OPEN)
		&& TRUE == MOlbSystemPre.GetIOComponent()->IsOn(OUT_REAR_DOOR_LOCK_OPEN))
	/*/

	m_plnkSystemData->m_bUseDryrunFeed = TRUE;
#ifndef DEF_SETUP
	if (TRUE == m_plnkTrsAutoManager->IsDoorOpen())
	//@___________________________
	{
		/* jdy sesl
		// 300003 = 문이 열린 상태 입니다. 
		MyMessageBox(300003, _T("Error"), M_ICONERROR);
		*/
		int iResult = 300003000;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;
	}
	//_______________________________________________-
	//20171004 KDW Start
	if (m_plnkTrsAutoManager->IsDoorOpen_AOC())
	{
		int iResult = 300013000;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("AOC Door Open Error"), M_ICONINFORMATION);
		return;
	}
	//20171004 KDW End
#endif

	// selsk_jdy : 부상 Plate 에서만 Panel 흡착하고 있는 경우 처리.
	MPanelAligner* plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
	if (TRUE == plnkPanelAligner->IsAirCVVacAllOn(DEF_NONE_WORK_SCHEDULE) && TRUE == plnkPanelAligner->IsPanelReleased())
	{
		//부상 Plate 가 Panel 을 흡착하고 있습니다. Panel Aligner 로 Panel 을 흡착한 후에 초기화를 진행하세요.
		MyMessageBox(MY_MESSAGE_BOX, 20002, _T("Error"), M_ICONERROR);
		return;
	}

	enableButton(FALSE);

	//초기화를 진행하시겠습니까?
	int iResult = MyMessageBox(MY_MESSAGE_BOX, 20003, _T("Confirm"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
	{
		enableButton(TRUE);
		return;
	}

	// NSMC
	if (TRUE == m_plnkTrsAutoManager->IsLastGlassInput()
		|| TRUE == m_plnkTrsAutoManager->IsStartModelChange())
	{
		MyMessageBox("Auto Model Change Cancel ", "Confirm", M_OK);
		m_plnkTrsAutoManager->ResetAutoModelChagneFlag();
	}

	// selsk_jdy unitInitialize();
	UnitInitialize();
	
	enableButton(TRUE);
}

// selsk_jdy
//void CAutoMainForm::unitInitialize()
void CAutoMainForm::UnitInitialize(BOOL bDisplayMsg)
{
	MOlbSystemPre.GetLCNetComponent()->EqProcessStateReport(eInit);
	OnDisplayEqpState((WPARAM)eInit, NULL);
	CString szTextTemp;

	int iResult = initializeAutoRun();
	if (iResult)
	{
		// selsk_jdy
		if (TRUE == bDisplayMsg)
		{
			CString strMsg;

			if (ERR_PC_REBOOTING_REQUEST == iResult)
			{
				//PC 오동작 중.프로그램 종료 후 PC Rebooting 해 주세요.
				ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_4"), &strMsg);
				m_plnkTrsAutoManager->WriteMonitoringLog("Can not do Normalcy Thread Stop(During AutoRun Init).");
			}
			else
				strMsg = MOlbSystemPre.GetErrorMessage(iResult);
			
			MyMessageBox(strMsg, _T("Unit Initialization"), M_ICONERROR);
		}
		enableButton(TRUE);
		return;
	}

	MOlbSystemPre.GetLCNetComponent()->EqProcessStateReport(ePause);
	OnDisplayEqpState((WPARAM)ePause, NULL);
	MOlbSystemPre.GetTrsAutoManagerComponent()->m_bErrorStopState = FALSE; //100121 SJ_YYK

	// selsk_jdy
	if (TRUE == bDisplayMsg)
	{
		//자동운전 초기화 완료.
		ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_5"), &szTextTemp);
		CString strMsg = szTextTemp;
//		MyMessageBox(strMsg, _T("Unit Initialization"), M_ICONERROR); // 090118_syr
		MyMessageBox(strMsg, _T("Unit Initialization"), M_ICONINFORMATION); // 2009.04.18 SESL_CYJ

		/*/
		// sesl_jdy
		if (NORMAL_RUN_MODE == m_plnkSystemData->m_eRunMode || REPAIR_RUN_MODE == m_plnkSystemData->m_eRunMode)
		{
			int iMessageDisplayRetryLimit = 0;
			for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
				m_plnkCtrlTabFeeder[i]->CheckAndRequestRailUnfold(iMessageDisplayRetryLimit);
		}
		/*/
	}
}

int CAutoMainForm::initializeAutoRun()
{
	int iResult = SUCCESS;
	int i = 0;
	
	// 초기화 작업 내용 확인.
	BOOL bSkipCurrentPanelWork = FALSE;
	m_iInitialMode = -1;
	sm_iRemoveTabICMode = 0;

	//170616 JSH.s Reset 추가
	MOlbSystemPre.GetTrsInspectionCameraComponent(0)->ResetInspectionTabBolb();
	//작업 전체초기화시 Inspection Bolb 검사중 실패 했을때만 TRUE
	BOOL bForceUnitAll_Initialize = FALSE;
	if(m_plnkSystemData->m_bUseInspectionTabBolb == TRUE)
		bForceUnitAll_Initialize = MOlbSystemPre.GetTrsInspectionCameraComponent(0)->IsForceUnitAll_Initialize();
	MOlbSystemPre.GetTrsInspectionCameraComponent(0)->ResetForceUnitAll_Initialize();
	
	//170616 JSH.e

#ifndef SIMULATION
	if (MOlbSystemPre.GetSystemData()->m_eRunMode != DRY_RUN_MODE
		&& MOlbSystemPre.GetSystemData()->m_eRunMode != PASS_RUN_MODE)
	{
		CString szTextTemp;
		if (MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed() == TRUE)
		{	//170616 JSH.s
//			ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_24"), &szTextTemp);
//			CSystemThreeBtnDlg dlg(szTextTemp, "All\r\nInitialize", "Keep\r\nCurrentWork", "Panel\r\nPass");
//			m_iInitialMode = dlg.DoModal();
			if(bForceUnitAll_Initialize == FALSE)
			{
				ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_24"), &szTextTemp);
				//20171031 KDW Start
				//CSystemThreeBtnDlg dlg(szTextTemp, "All\r\nInitialize", "Keep\r\nCurrentWork", "Panel\r\nPass");
				CSystemThreeBtnDlg dlg(szTextTemp, "All\r\nInitialize", "Keep\r\nCurrentWork", "");
				//20171031 KDW End
				m_iInitialMode = dlg.DoModal();
			}
			else
			{
				MyMessageBox(MY_MESSAGE_BOX, 20021 , _T("Confirm"), M_ICONINFORMATION); //171004 JSH
				m_iInitialMode = 0;
			}
			//170616 JSH.e

			if (m_iInitialMode == 2)
				bSkipCurrentPanelWork = TRUE;
				
			//170612 JSh.s
			if(m_iInitialMode == 0) //전체 초기화
			{
				MOlbSystemPre.GetTrsInspectionCameraComponent(0)->SetInspectionTabBolb();
				TRACE(_T("<<MOlbSystemPre.GetTrsInspectionCameraComponent(0)->SetInspectionTabBolb();>>\n"));
			}
			//170612 JSh.e
		}

		if (m_iInitialMode != 2)
		{
			if (MOlbSystemPre.IsExistRemoveTab_TabMounter(DEF_FRONT_GROUP) || MOlbSystemPre.IsExistRemoveTab_TabMounter(DEF_REAR_GROUP)
				|| MOlbSystemPre.IsExistRemoveTab_TabCarrier(DEF_FRONT_GROUP) || MOlbSystemPre.IsExistRemoveTab_TabCarrier(DEF_REAR_GROUP)
				)
			{
				ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_25"), &szTextTemp);
				CSystemThreeBtnDlg dlg(szTextTemp, "Not Remove", "Mounter\r\nTabIC", "All\r\nTabIC");
				sm_iRemoveTabICMode = dlg.DoModal();
			}
		}
	}
#endif
	
	CMyProgressWnd ProgWnd(this, _T("Unit Initialize Executing..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("...is executing the Unit Initialize...\nWait a moment..."));
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();
	
	m_plnkTrsAutoManager->WriteMonitoringLog("Starting AutoRun Init.");

	if (TRUE == MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed())
	{
		iResult = MOlbSystemPre.GetPanelAlignerComponent()->AirCVBlowOn();
		if (iResult)
			return iResult;
		iResult = MOlbSystemPre.GetPanelAlignerComponent()->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_MOVE_UP_POS);
		if (iResult)
			return iResult;
	}

	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		iResult = MOlbSystemPre.GetCameraCarrierComponent(i)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
		if (iResult)
			return iResult;
		iResult = MOlbSystemPre.GetRCameraCarrierComponent(i)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
		if (iResult)
			return iResult;
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Panel Align Camera - Panel Mark 인식 위치 이동.
	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		//100823.임시추가(모델에 따라 확장 축소)
//@		if (i==DEF_INSPECTION_CARRIER_1 || i==DEF_INSPECTION_CARRIER_4)
		if (i == DEF_INSPECTION_CARRIER_1)
		{
			iResult = MOlbSystemPre.GetCameraCarrierComponent(i)->ExpandShrinkForPanelAlign();
		}
		else
		{
			iResult = MOlbSystemPre.GetCameraCarrierComponent(i)->ExpandShrinkForTabAlign();
		}
		if (iResult)
			return iResult;
		//________________________________________
	}

	iResult = MOlbSystemPre.MoveInspectCameraPanelAlignPos();
	if (iResult)
		return iResult;

//@	if (m_plnkSystemData->m_nUseFeeder != DEF_TABFEEDER_2)
	if (MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP)->GetUseWorkSide())
	{
		iResult = MOlbSystemPre.GetTabCarrierComponent(0)->ExpandShrinkForPreAlign();
		if (iResult)
			return iResult;

		iResult = MOlbSystemPre.GetTabCarrierComponent(1)->ExpandShrinkForPreAlign();
		if (iResult)
			return iResult;
	}

//@	if (m_plnkSystemData->m_nUseFeeder != DEF_TABFEEDER_1)
	if (MOlbSystemPre.GetWorkSchedule(DEF_REAR_GROUP)->GetUseWorkSide())
	{
		iResult = MOlbSystemPre.GetRTabCarrierComponent(0)->ExpandShrinkForPreAlign();
		if (iResult)
			return iResult;

		iResult = MOlbSystemPre.GetRTabCarrierComponent(1)->ExpandShrinkForPreAlign();
		if (iResult)
			return iResult;
	}
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 자동운전 초기화 처리 Thread 시작.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	CWinThread* rgpTabMounterThread[DEF_MAX_WORKER_GROUP];
	CWinThread* rgpInspectionCameraThread[DEF_MAX_WORKER_GROUP];
	CWinThread* rgpTabCarrierThread;
	CWinThread* rgpRTabCarrierThread;
	CWinThread* rgpTabFeederThread;
	CWinThread* rgpRTabFeederThread;
//del	CWinThread* rgpConveyorThread[DEF_MAX_CONVEYOR];
	CWinThread* rgpPanelTransferCenterThread;
	CWinThread* rgpPanelTransferOutThread;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Motor Event Clear 작업은 시간이 많이 소요되지만,
	// Conveyor Initialize 작업 시간과 중첩 시킴으로써 전체적인 초기화 동작 시간을 단축할 수 있다.
	// (Conveyor 에는 Motor 가 사용되지 않기 때문에 시간 중첩 가능.)
	m_plnkTrsAutoManager->ResetAllMotorEvent();
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	// Other Unit 초기화 thread begin.
	///KKY.Del.Thread에서 초기화중 MyMessageBox가 호출되면 Holding현상 발생.
	//따라서 MessageBox호출되는 Unit은 직접 Initial 함수를 호출함
	/*
	sm_rgbPanelTransferOutInitComplete = FALSE;
	sm_rgiPanelTransferOutInitResult = SUCCESS;
	rgpPanelTransferOutThread = NULL;
	rgpPanelTransferOutThread = AfxBeginThread(initPanelTransferOut, this);
//	Sleep(5);
	Sleep(100);

	sm_rgbPanelTransferCenterInitComplete = FALSE;
	sm_rgiPanelTransferCenterInitResult = SUCCESS;
	rgpPanelTransferCenterThread = NULL;
	rgpPanelTransferCenterThread = AfxBeginThread(initPanelTransferCenter, this);
//	Sleep(5);
	*/
	MOlbSystemPre.SetTabEndMarkDistance(0, 0.0);
	MOlbSystemPre.SetTabEndMarkDistance(1, 0.0);

	sm_rgbTabCarrierInitComplete= FALSE;
	sm_rgiTabCarrierInitResult= SUCCESS;
	rgpTabCarrierThread = NULL;
	rgpTabCarrierThread = AfxBeginThread(initTabCarrier, (LPVOID)0);
	
	sm_rgbRTabCarrierInitComplete= FALSE;
	sm_rgiRTabCarrierInitResult= SUCCESS;
	rgpRTabCarrierThread = NULL;
	rgpRTabCarrierThread = AfxBeginThread(initRTabCarrier, (LPVOID)0);
	
	sm_rgbTabFeederInitComplete= FALSE;
	sm_rgiTabFeederInitResult= SUCCESS;
	rgpTabFeederThread = NULL;
	rgpTabFeederThread = AfxBeginThread(initTabFeeder, (LPVOID)0);
	
	sm_rgbRTabFeederInitComplete= FALSE;
	sm_rgiRTabFeederInitResult= SUCCESS;
	rgpRTabFeederThread = NULL;
	rgpRTabFeederThread = AfxBeginThread(initRTabFeeder, (LPVOID)0);

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		sm_rgbTabMounterInitComplete[i] = FALSE;
		sm_rgbInspectionCameraInitComplete[i] = FALSE;

		sm_rgiTabMounterInitResult[i] = SUCCESS;
		sm_rgiInspectionCameraInitResult[i] = SUCCESS;

		rgpTabMounterThread[i] = NULL;
		rgpInspectionCameraThread[i] = NULL;

		rgpTabMounterThread[i] = AfxBeginThread(initTabMounter, (LPVOID)i);
		Sleep(1);
		rgpInspectionCameraThread[i] = AfxBeginThread(initInspectionCamera, (LPVOID)i);
		Sleep(1);
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Lighting On : E-Stop 후 등 설비 운전 중 조명 꺼졌을 때 대비.
	MOlbSystemPre.SetLightValue(DEF_MAX_CAMERA_NO);
	MOlbSystemPre.LightDefult(DEF_MAX_CAMERA_NO); //171004 JSH
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Thread 사용하지 않는 자동운전 초기화 처리. : 작업 순서 주의.		// selsk_jdy
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//170216 SJ_YSH
	//Panel Aligner 전체 초기화 시 Panel Transfer In 의 Panel과 충돌 발생..
	//if (m_iInitialMode == 0 && MOlbSystemPre.GetPanelTransferComponent()->IsPanelAbsorbed() == TRUE)
	if (m_iInitialMode != 1 && MOlbSystemPre.GetPanelTransferComponent()->IsPanelAbsorbed() == TRUE)
	{
		iResult = MOlbSystemPre.GetPanelTransferComponent()->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS);
		if (iResult)
			return iResult;		
	}
	//_____________

	//@________________________
//@	int iPanelAlignerInitResult = MOlbSystemPre.GetTrsPanelAlignerComponent()->Initialize(bSkipCurrentPanelWork);
	int iPanelAlignerInitResult = 0;
	if (m_iInitialMode == 0)	//All Initialize
		iPanelAlignerInitResult = MOlbSystemPre.GetTrsPanelAlignerComponent()->Initialize(TRUE, bSkipCurrentPanelWork);
	else
		iPanelAlignerInitResult = MOlbSystemPre.GetTrsPanelAlignerComponent()->Initialize(FALSE, bSkipCurrentPanelWork);
	//@________________________
	

	int iPanelTransferInitResult = FAILURE;
#ifdef DEF_SOURCE_SYSTEM
	if (SUCCESS == iPanelAlignerInitResult)
	{
		iPanelTransferInitResult = MOlbSystemPre.GetTrsPanelTransferComponent()->Initialize();
	}
#else
	iPanelTransferInitResult = SUCCESS;
#endif
	sm_rgbPanelTransferOutInitComplete = FALSE;
	sm_rgiPanelTransferOutInitResult = SUCCESS;
	rgpPanelTransferOutThread = NULL;
	initPanelTransferOut(0);

	sm_rgbPanelTransferCenterInitComplete = FALSE;
	sm_rgiPanelTransferCenterInitResult = SUCCESS;
	rgpPanelTransferCenterThread = NULL;
#ifdef DEF_SOURCE_SYSTEM
	initPanelTransfer(0);
#else
	sm_rgbPanelTransferCenterInitComplete = TRUE;
#endif

	//SJ_YYK 161104 Add.
	sm_rgbTurnHandlerInitComplete = FALSE;		
	sm_rgiTurnHandlerInitResult = SUCCESS;

#ifdef DEF_USE_TURN_HANDLER
	initTurnHandler(0);
#endif
	//________________________*/


	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 자동운전 초기화 처리 Thread 종료 처리.
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	MTickTimer timerLimit, timerScan;
	timerLimit.StartTimer();
	timerScan.StartTimer();
	
//	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		while (TRUE)
		{
			if ((NULL == rgpTabMounterThread[0] || TRUE == sm_rgbTabMounterInitComplete[0])
				&& (NULL == rgpInspectionCameraThread[0] || TRUE == sm_rgbInspectionCameraInitComplete[0])
				&& (NULL == rgpTabCarrierThread || TRUE == sm_rgbTabCarrierInitComplete)
				&& (NULL == rgpRTabCarrierThread || TRUE == sm_rgbRTabCarrierInitComplete)
				&& (NULL == rgpTabFeederThread || TRUE == sm_rgbTabFeederInitComplete)
				&& (NULL == rgpRTabFeederThread || TRUE == sm_rgbRTabFeederInitComplete)
				&& (NULL == rgpPanelTransferCenterThread || TRUE == sm_rgbPanelTransferCenterInitComplete)
				&& (NULL == rgpPanelTransferOutThread || TRUE == sm_rgbPanelTransferOutInitComplete)
#ifdef DEF_USE_TURN_HANDLER
				&& (TRUE == sm_rgbTurnHandlerInitComplete)
#endif
				)
			{
				break;
			}

			Sleep(5);	// Message Pumping 은 하지 않음.
			if (timerScan.MoreThan(1.0))
			{
				PeekAndPump();
				timerScan.StartTimer();
			}

			//if (TRUE == timerLimit.MoreThan(10 * 60.0))
			if (TRUE == timerLimit.MoreThan(10 * 30.0))
			{
				// 발생할 수 없는 Case : 발생했다면 OS 문제로 봐야 함. [Thread 정상 종료 안됨.]
				// Thread 강제 종료는 하지 않음.
				return ERR_PC_REBOOTING_REQUEST;
			}
		}
	}

	if (sm_iRemoveTabICMode != 0)
	{
		iResult = removeAllTabIC();
		if (iResult)
		{
			sm_rgiTabMounterInitResult[DEF_FRONT_GROUP] = iResult;
			sm_rgbTabMounterInitComplete[DEF_FRONT_GROUP] = TRUE;
		}
	}

	initializeProcessData();

	m_plnkTrsAutoManager->WriteMonitoringLog("AutoRun Init Complete.");

	if (SUCCESS !=  sm_rgiPanelTransferCenterInitResult)
		return sm_rgiPanelTransferCenterInitResult;

	if (SUCCESS != sm_rgiPanelTransferOutInitResult)
		return sm_rgiPanelTransferOutInitResult;

	if (SUCCESS != sm_rgiTabCarrierInitResult)
		return sm_rgiTabCarrierInitResult;
	
	
	if (SUCCESS != sm_rgiRTabCarrierInitResult)
		return sm_rgiRTabCarrierInitResult;
	
	//8Mount사용 Mode가 아닐경우 Feeder초기화 되어 있지 않으면 에러
	if (FALSE == m_plnkSystemData->m_bUse1FeederAllMount)
	{
		if (SUCCESS != sm_rgiTabFeederInitResult)
			return sm_rgiTabFeederInitResult;
		
		if (SUCCESS != sm_rgiRTabFeederInitResult)
			return sm_rgiRTabFeederInitResult;
	}
	//8Mount사용 Mode일 경우 Feeder모두 초기화 되어 있지 않으면 에러(둘중 하나만 초기화 되면 됨)
	else
	{
		if (SUCCESS != sm_rgiTabFeederInitResult && SUCCESS != sm_rgiRTabFeederInitResult)
		{
			if (SUCCESS != sm_rgiTabFeederInitResult)
				return sm_rgiTabFeederInitResult;
			
			if (SUCCESS != sm_rgiRTabFeederInitResult)
				return sm_rgiRTabFeederInitResult;
		}
		MOlbSystemPre.GetTrsTabFeederComponent(DEF_TABFEEDER1)->SetInitializedOK();
		MOlbSystemPre.GetTrsTabFeederComponent(DEF_TABFEEDER2)->SetInitializedOK();
	}
	
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		if (SUCCESS != sm_rgiTabMounterInitResult[i])
			return sm_rgiTabMounterInitResult[i];

		if (SUCCESS != sm_rgiInspectionCameraInitResult[i])
			return sm_rgiInspectionCameraInitResult[i];
	}

#if FALSE
	for (int i = 0; i < DEF_MAX_CONVEYOR; i++)
	{
		if (SUCCESS != sm_rgiConveyorInitResult[i])
			return sm_rgiConveyorInitResult[i];
	}
#endif

	if (SUCCESS != iPanelAlignerInitResult)
		return iPanelAlignerInitResult;

	if (SUCCESS != iPanelTransferInitResult)
		return iPanelTransferInitResult;

	if (SUCCESS != sm_rgiTurnHandlerInitResult)
		return sm_rgiTurnHandlerInitResult;

	ProgWnd.DestroyWindow();
	return SUCCESS;
}

void CAutoMainForm::OnStepDisp() 
{
	if (m_StepDispFlag == FALSE)
	{
		m_pDlg->Create(IDD_AUTO_STEP_DISP, this);
		m_StepDispFlag = TRUE;
	}
	m_pDlg->ShowWindow(SW_SHOW);
	m_pDlg->StartDlgTimer();
}

// Doolin kwangilshin 2017. 09. 12.
//
void CAutoMainForm::OnPanelInAndScrapCount()
{
	CString strMsg;
	BOOL	bIsDataChange = FALSE;

	if (m_plnkProductInfoData->m_nPanelInputQty)
	{
		ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_28"), &strMsg);
		if (MyMessageBox(strMsg, _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDYES)
		{
			bIsDataChange = TRUE;
			m_plnkProductInfoData->m_nPanelInputQty = 0;
		}
	}
	
	if (m_plnkProductInfoData->m_nPanelScrapQty)
	{
		ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_29"), &strMsg);
		if (MyMessageBox(strMsg, _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDYES)
		{
			bIsDataChange = TRUE;
			m_plnkProductInfoData->m_nPanelScrapQty = 0;	
		}
	}

	if (bIsDataChange)
	{
		m_plnkProductInfoData->WriteData();
		m_plnkProductInfoData->WriteMaterialData();
		
		for (int i = 0; i < DEF_MAX_PANEL_UNIT; i++)
		{
			m_plnkProductInfoData->m_Glass[i] = *m_plnkTrsAutoManager->m_pGlassData[i];
		}
		
		m_plnkProductInfoData->WriteGlassData();
		
		this->SendMessage(WM_DISP_PANEL_IN_QTY);
		this->SendMessage(WM_DISP_PANEL_SCRAP_QTY);	
	}
}
// End.

void CAutoMainForm::OnProductCount() 
{
	//생산 수량을 초기화 하시겠습니까?
	CString strMsg;
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_6"), &strMsg);
	if (MyMessageBox(strMsg, _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDNO) 
		return;

	m_plnkProductInfoData->m_iProductCount = 0;
	m_plnkProductInfoData->WriteData();
	m_plnkProductInfoData->WriteMaterialData();

	for (int i = 0; i < DEF_MAX_PANEL_UNIT; i++)
		m_plnkProductInfoData->m_Glass[i] = *m_plnkTrsAutoManager->m_pGlassData[i];

	m_plnkProductInfoData->WriteGlassData();

	this->SendMessage(WM_DISP_PRODUCT_QTY);
}

LRESULT CAutoMainForm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_SWITCH_VIEW:
		if (TRUE == m_StepDispFlag)
		{
			m_pDlg->StopDlgTimer();
			m_pDlg->ShowWindow(SW_HIDE);
		}

		if (TRUE == m_bTimeDispFlag)
			m_pTimeDlg->ShowWindow(SW_HIDE);

		//110621.KKY_____
		if (m_pDlgMaterial != NULL)
			m_pDlgMaterial->ShowWindow(SW_HIDE);
		//_______________

		//110624_____
		if (m_pDlgProductPlan != NULL)
			m_pDlgProductPlan->ShowWindow(SW_HIDE);
		//___________

		break;
	}
	
	return CFormView::WindowProc(message, wParam, lParam);
}

void CAutoMainForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkPanelData = MOlbSystemPre.GetPanelData(); //150527 SJ_YSH
	m_plnkProductInfoData = MOlbSystemPre.GetProductInfoData();
	m_plnkVision = MOlbSystemPre.GetVisionComponent();
	m_plnkTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
//KKY.Del	m_plnkTrsLoadConveyor = MOlbSystemPre.GetTrsLoadConveyorComponent();
#ifdef DEF_USE_TRANSFER_CENTER
	m_plnkTrsPanelTransferCenter = MOlbSystemPre.GetTrsPanelTransferCenterComponent();
#endif

#ifdef DEF_SOURCE_SYSTEM
	m_plnkTrsPanelTransfer = MOlbSystemPre.GetTrsPanelTransferComponent();
#endif
	m_plnkTrsPanelAligner = MOlbSystemPre.GetTrsPanelAlignerComponent();
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
		m_plnkCtrlTabFeeder[i] = MOlbSystemPre.GetCtrlTabFeederComponent(i);

#ifdef SIMULATION
	m_plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
#endif
	
	/** set View Handle **/
	m_plnkTrsAutoManager->SetWindowHandle(m_hWnd);

// hongju_SESL
#ifndef VISION_SIMULATION
	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS1);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[0] = m_plnkVision->CreateFixedView(0, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS2);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[1] = m_plnkVision->CreateFixedView(1, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS3);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[2] = m_plnkVision->CreateFixedView(2, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS4);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[3] = m_plnkVision->CreateFixedView(3, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS5);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[4] = m_plnkVision->CreateFixedView(4, &m_rect, this);

	m_pViewWnd = GetDlgItem(IDC_VISION_CANVAS6);
	m_pViewWnd->GetWindowRect(&m_rect);
	ScreenToClient(&m_rect);
	m_rgiViewID[5] = m_plnkVision->CreateFixedView(5, &m_rect, this);
#endif

	//161102//171010 JSh
	if (m_pDlgFixedVision == NULL)
	{
		m_pDlgFixedVision = new CDlgFixedVision;
		m_pDlgFixedVision->Create(IDD_DLGFIXEDVISION_DIALOG, this);
	}

	m_pDlgFixedVision->ShowWindow(SW_SHOW);
	
	m_pDlgFixedVision->CreateFixedView();
	//______

	m_uiTimerID = SetTimer(1, 200, NULL);

	m_ctrlLblOpStatus.SetCaption(_T("MANUAL"));
	this->SendMessage(WM_DISP_RUN_MODE);
	this->SendMessage(WM_DISP_MODEL_NAME);
	this->SendMessage(WM_DISP_MODEL_DEVICEID);
	this->SendMessage(WM_DISP_PRODUCT_QTY);
	// Doolin kwangilshin 2017. 09. 12.
	//	
	this->SendMessage(WM_DISP_PANEL_IN_QTY);
	this->SendMessage(WM_DISP_PANEL_SCRAP_QTY);
	// End

	this->SendMessage(WM_DISP_TABFEEDER1_INFO);
	this->SendMessage(WM_DISP_TABFEEDER2_INFO);

	// NSMC
//@	this->SendMessage(WM_DISP_PANEL_ID, (WPARAM)DEF_PANEL_TRANSFER_CENTER_PANEL, NULL);
#ifdef DEF_SOURCE_SYSTEM
	//SJ_PJC 20170123
	this->SendMessage(WM_DISP_PANEL_ID, (WPARAM)DEF_PANEL_TRANSFER_CENTER_PANEL, NULL);
	this->SendMessage(WM_DISP_PANEL_ID, (WPARAM)DEF_THANDLER_PANEL, NULL);
	//_______________
	this->SendMessage(WM_DISP_PANEL_ID, (WPARAM)DEF_PANEL_TRANSFER_PANEL, NULL);
#endif
	this->SendMessage(WM_DISP_PANEL_ID, (WPARAM)DEF_PANEL_ALIGNER_PANEL, NULL);
	this->SendMessage(WM_DISP_PANEL_ID, (WPARAM)DEF_PANEL_TRANSFER_OUT_PANEL, NULL);

	this->SendMessage(WM_DISP_EQ_STATE);
	this->SendMessage(WM_DISP_EQP_STATE);

	m_dlgRepairTabSelect.Create(IDD_AUTO_REPAIR_TAB_SELECT, this);

	m_dlgOperatorCall.Create(IDD_COMMON_OPCALL_DLG, this);
	m_dlgOperatorCall.SetWindowText(_T("Operator Call"));

#ifdef DEF_GATE_SYSTEM
	GetDlgItem(IDB_PANEL_TRNASFER)->EnableWindow(FALSE);	
	GetDlgItem(IDC_PANEL_TRANSFER)->EnableWindow(FALSE);
#endif

	// ML_hongju
	this->SendMessage(WM_DISP_MULTI_LANGUAGE_DLG_REQ);

	// Doolin kwangilshin 2017. 09. 16.
	//
#ifndef DEF_USE_TRANSFER_CENTER
	GetDlgItem(IDB_LOAD_CONVEYOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOAD_CONVEYOR)->EnableWindow(FALSE);
#endif
#ifndef DEF_USE_TURN_HANDLER
	GetDlgItem(IDB_TURN_HANDLER)->EnableWindow(FALSE);
	GetDlgItem(IDC_TURN_HANDLER)->EnableWindow(FALSE);
#endif
	// End.
}

void CAutoMainForm::PeekAndPump()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			return;
		}
		Sleep(1);
	}
}

void CAutoMainForm::OnTimer(UINT nIDEvent) 
{
	CString strInfo;

	/*/
	//@___________
	strInfo.Format(_T("TabM:%d"), MOlbSystemPre.GetTrsTabMounterComponent(0)->GetWorkScheduleNo(0));
	m_rgLblWorkerPanelID[DEF_PANEL_ALIGNER_PANEL].SetCaption(strInfo);
	strInfo.Format(_T("TabC:%d"), MOlbSystemPre.GetTrsTabCarrierComponent(DEF_MAX_TABCARRIER-1)->GetWorkScheduleNo(0));
	m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_OUT_PANEL].SetCaption(strInfo);
	//@___________
	/*/

	// Feeder Info
	strInfo.Format(_T("%d,%d"),m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->m_matTabICInfo[DEF_USING_REEL].m_nT_QTY,
						m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->m_matTabICInfo[DEF_USING_REEL].m_nR_QTY);
	m_ctrlLblFeeder1.SetCaption(strInfo);
//#ifdef DEF_GATE_SYSTEM
	strInfo.Format(_T("%d,%d"),m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->m_matTabICInfo[DEF_USING_REEL].m_nT_QTY,
						m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->m_matTabICInfo[DEF_USING_REEL].m_nR_QTY);
	m_ctrlLblFeeder2.SetCaption(strInfo);
//#endif

	strInfo.Format(_T("%d"),m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->m_matZigInfo.m_nU_QTY);
	m_ctrlMold1.SetCaption(strInfo);
	strInfo.Format(_T("%d"),m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->m_matZigInfo.m_nU_QTY);
	m_ctrlMold2.SetCaption(strInfo);

	// Bonding Info
	strInfo.Format(_T("%.2f s"),m_plnkSystemData->m_dBondTime);
	m_ctrlLblPreBondingTime.SetCaption(strInfo);

	// sesl_jdy
	// Equipment Interlock Mode Display
	if (m_plnkSystemData->m_bSafetySensor)
		m_lblInterlockMode.SetCaption(_T("Safety On"));
	else
		m_lblInterlockMode.SetCaption(_T("Safety Off"));

	/*/
// hongju 수정...20090325
	if (MOlbSystemPre.GetIOComponent()->IsOn(OUT_FRONT_DOOR_LOCK_OPEN)
		|| MOlbSystemPre.GetIOComponent()->IsOn(OUT_REAR_DOOR_LOCK_OPEN))
	{
		m_lblDoorLock.SetBackColorInterior(RGB(255,0,0));
		m_lblDoorLock.SetCaption(_T("Door Lock \n Off"));
	}
	else
	{
		m_lblDoorLock.SetBackColorInterior(RGB(170,255,85));
		m_lblDoorLock.SetCaption(_T("Door Lock \n On"));
	}
	/*/

	/*
	// 100210.kms.add
	// Prebonder일경우 MMT 에서 생산 n% 문제 발생시 설비 Panel in stop 요청
	//
#ifdef DEF_GATE_SYSTEM
	//if (TRUE == m_plnkTrsLoadConveyor->IsPanelInStop())
	if (TRUE == m_plnkTrsPanelTransferCenter->IsPanelInStop())
	{
		m_ctrlLblOpStatus.SetCaption(_T("Panel In Stop"));
		m_ctrlLblOpStatus.SetForeColor(RED);
	}
	else
	{
		SendMessage(WM_AUTO_STATUS, m_plnkTrsAutoManager->GetOPMode(), 0);
	}
#endif
	*/
	//_______________________________________________________________________

//	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		if (m_btnFeeder1Ready.IsWindowVisible())
		{
			BOOL bReady = m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->GetFeederReady();
			if (m_btnFeeder1Ready.GetValue() != bReady)
				m_btnFeeder1Ready.SetValue(bReady);
		}
		if (m_btnFeeder2Ready.IsWindowVisible())
		{
			BOOL bReady = m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->GetFeederReady();
			if (m_btnFeeder2Ready.GetValue() != bReady)
				m_btnFeeder2Ready.SetValue(bReady);
		}
	}

	//120201.KMS__________
	//현재 Panel  이 어디에 있는지 확인 하기 위한 ...............
	OnDisplayPanelAbsorbState();
	//____________________

	//110630.kms_______________________________________
	//Online 상태 확인하여 IDB_LCNET_IF Button 색 변경
	if(m_plnkLCNet->m_bConnected == TRUE)
	{
		m_BtnLCnet.SetBackColorInterior(DKBLUE);
		m_BtnLCnet.SetWindowText(_T("OnLine"));
	}
	else
	{
		m_BtnLCnet.SetBackColorInterior(DKRED);
		m_BtnLCnet.SetWindowText(_T("OffLine"));
	}
	//_________________________________________________
//	m_ctrlLblDeviceID.SetCaption(MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName);
//	m_ctrlLblDeviceID.SetCaption(m_plnkSystemData->m_strCurrentDeviceID);

#ifdef DEF_GATE_SYSTEM
	m_ctrlLblAutoAlign.ShowWindow(SW_HIDE);
#else
	//20120126 SJ_HJG
	if(m_plnkSystemData->m_bUseAutoAlignSystem == TRUE)
	{
		m_ctrlLblAutoAlign.SetBackColorInterior(YELLOW);
		m_ctrlLblAutoAlign.SetCaption(_T("Auto Align\n[ON]"));
	}
	else
	{
		m_ctrlLblAutoAlign.SetBackColorInterior(RED);
		m_ctrlLblAutoAlign.SetCaption(_T("Auto Align\n[OFF]"));
	}
#endif

	CFormView::OnTimer(nIDEvent);
}

void CAutoMainForm::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	for (int i = 0; i < DEF_MAX_FIXED_VIEW; i++)
		m_plnkVision->DestroyView(m_rgiViewID[i]);
}

//__________________________________________________________________________
// Msg Display 
LRESULT CAutoMainForm::OnDisplayRunMode(UINT wParam, LPARAM lParam)
{
	CString strRunMode;
	switch (m_plnkSystemData->m_eRunMode)
	{
	case NORMAL_RUN_MODE: //syr..SESL 090609 
		// jdy sesl 081229
//		if (FALSE == MOlbSystemPre.IsNoWorkInAutoRunMode())
//		{
			strRunMode.Format(_T("Normal Run"));
			m_ctrlLblOpMode.SetForeColor(BLUE);
			break;
//		}
		// Fall Through

	case PASS_RUN_MODE:
		// jdy sesl
//		if (TRUE == MOlbSystemPre.IsNoWorkInAutoRunMode())
//		{
//			strRunMode.Format(_T("Normal(Skip)"));
//			break;
//		}
		if(m_plnkSystemData->m_bUseAlignPass == TRUE)
		{
			strRunMode.Format(_T("Align Pass Run"));
		}
		else	strRunMode.Format(_T("Pass Run"));
		
		m_ctrlLblOpMode.SetForeColor(RED);
		break;
	case DRY_RUN_MODE:
		strRunMode.Format(_T("Dry Run"));
		m_ctrlLblOpMode.SetForeColor(DKGREEN);
		break;
	case REPAIR_RUN_MODE:
		strRunMode.Format(_T("Repair Run"));
		break;
	default:
		return 0;
	}

	m_ctrlLblOpMode.SetCaption(strRunMode);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayModelName(UINT wParam, LPARAM lParam)
{
	m_ctrlLblModelName.SetCaption(m_plnkSystemData->m_strModelFileName);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayDeviceID(UINT wParam, LPARAM lParam)
{
	m_ctrlLblDeviceID.SetCaption(MOlbSystemPre.GetSystemData()->m_strCurrentDeviceID);

	return 0;
}

LRESULT CAutoMainForm::OnDisplayEqTact(UINT wParam, LPARAM lParam)
{
	CString strEqTact;
	strEqTact.Format(_T("%.1f sec"), m_plnkProductInfoData->m_dEquipTact);
	m_ctrlLblEqTact.SetCaption(strEqTact);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayLineTact(UINT wParam, LPARAM lParam)
{
	CString strLineTact;
	strLineTact.Format(_T("%.1f sec"), m_plnkProductInfoData->GetMeanLineTact());
	m_ctrlLblLineTact.SetCaption(strLineTact);
	return 0;
}

// Doolin kwangilshin 2017. 09. 12.
//
LRESULT CAutoMainForm::OnDisplayPanelInQty(UINT wParam, LPARAM lParam)
{
	CString strPanelInQty;
	strPanelInQty.Format(_T("%d EA"), m_plnkProductInfoData->m_nPanelInputQty);
	m_ctrlLbPanelInQty.SetCaption(strPanelInQty);
	return S_OK;
}

LRESULT CAutoMainForm::OnDisplayPanelScrapQty(UINT wParam, LPARAM lParam)
{
	CString strPanelScrapQty;
	strPanelScrapQty.Format(_T("%d EA"), m_plnkProductInfoData->m_nPanelScrapQty);
	m_ctrlLbPanelScrapQty.SetCaption(strPanelScrapQty);
	return S_OK;
}
// End.

LRESULT CAutoMainForm::OnDisplayProductQty(UINT wParam, LPARAM lParam)
{
	CString strProductQty;
	strProductQty.Format(_T("%d EA"), m_plnkProductInfoData->m_iProductCount);

	m_plnkProductInfoData->WriteData();
	m_plnkProductInfoData->WriteMaterialData();

	m_ctrlLblProductQty.SetCaption(strProductQty);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayTabFeederInfo1(UINT wParam, LPARAM lParam)
{
/*	CString strFeederState;
	if (wParam==1)
	{
		switch (lParam)
		{
		case 0:
			strFeederState.Format("-");
			break;
		case 1:
			strFeederState.Format("X-TAB");
			break;
		case 2:
			strFeederState.Format("Y-TAB");
			break;
		default:
			strFeederState.Format("-");
			break;
		}
	}
	else
	{
		strFeederState.Format("-");
	}

	m_ctrlLblFeeder1.SetCaption(strFeederState);
*/
	return 0;
}

LRESULT CAutoMainForm::OnDisplayTabFeederInfo2(UINT wParam, LPARAM lParam)
{
/*	CString strFeederState;
	if (wParam==1)
	{
		switch (lParam)
		{
		case 0:
			strFeederState.Format("-");
			break;
		case 1:
			strFeederState.Format("X-TAB");
			break;
		case 2:
			strFeederState.Format("Y-TAB");
			break;
		default:
			strFeederState.Format("-");
			break;
		}
	}
	else
	{
		strFeederState.Format("-");
	}

	m_ctrlLblFeeder2.SetCaption(strFeederState);
*/
	return 0;
}

LRESULT CAutoMainForm::OnDisplayOPCallMsg(UINT wParam, LPARAM lParam)
{
	CString strOPCallMsg;
	CTime t = CTime::GetCurrentTime();
	CString strTime = t.Format("[%H:%M]");

	strOPCallMsg.Format(_T("%s %s"), strTime, *((CString*)wParam));
	m_dlgOperatorCall.SetMessage(strOPCallMsg);
	m_dlgOperatorCall.SetSelect((int)lParam);
	if(lParam == 0)
		m_dlgOperatorCall.ShowWindow(SW_SHOW);
	else
		m_dlgOperatorCall.ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayTerminalMsg(UINT wParam, LPARAM lParam)
{
	CString strTerminalMsg;
	CTime t = CTime::GetCurrentTime();
	CString strTime = t.Format("[%H:%M]");

	strTerminalMsg.Format(_T("%s %s"), strTime, *((CString*)wParam));
	
	m_ListTerminalMsg.InsertString(0,(LPCTSTR)strTerminalMsg);

	if (m_ListTerminalMsg.GetCount()>5)
		m_ListTerminalMsg.DeleteString(m_ListTerminalMsg.GetCount()-1);

	m_ListTerminalMsg.ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayEqState(UINT wParam, LPARAM lParam)
{
	CString strEqState;
	switch ((EEqState)wParam)
	{
	case eEqNothing:
		strEqState.Format(_T("Normal"));
		break;
	case eNormal:
		strEqState.Format(_T("Normal"));
		break;
	case eFault:
		strEqState.Format(_T("Fault"));
		break;
	case ePM:
		strEqState.Format(_T("PM"));
		break;
	default:
		break;
	}

	m_LblEqState.SetCaption(strEqState);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayEqpState(UINT wParam, LPARAM lParam)
{
	CString strEqpState;
	switch ((EEqProcState)wParam)
	{
	case eEqPNothing:
		strEqpState.Format(_T("Pause"));
		break;
	case eInit:
		strEqpState.Format(_T("Init"));
		break;
	case eIdle:
		strEqpState.Format(_T("Idle"));
		break;
	case eSetup:
		strEqpState.Format(_T("Setup"));
		break;
	case eReady:
		strEqpState.Format(_T("Ready"));
		break;
	case eExecute:
		strEqpState.Format(_T("Execute"));
		break;
	case ePause:
		strEqpState.Format(_T("Pause"));
		break;
	default:
		break;
	}

	m_LblEqpState.SetCaption(strEqpState);
	return 0;
}

void CAutoMainForm::OnClickMelsecIf() 
{
	// TODO: Add your control notification handler code here
	CString strTitle = _T("Select Melsec Unit");
	CSystemThreeBtnDlg dlg(strTitle, "Panel", "AOC", "Cancel");
	int iSel = dlg.DoModal();
	if (iSel == 0)
		MOlbSystemPre.GetMelsecComponent()->ViewVisible(TRUE);
	else 
		MOlbSystemPre.GetMelsecComponent2()->ViewVisible(TRUE);
}

void CAutoMainForm::OnClickLcnetIf() 
{
	// TODO: Add your control notification handler code here
	if (!MOlbSystemPre.GetLCNetComponent()->m_bShow)
		MOlbSystemPre.GetLCNetComponent()->ViewVisible(TRUE);
	else
	{
		MOlbSystemPre.GetLCNetComponent()->ViewVisible(FALSE);
		MOlbSystemPre.GetLCNetComponent()->ViewVisible(TRUE);
	}
	
	/*
	int iResult = 0;
	MTickTimer timer;
	timer.StartTimer();
	while(TRUE)
	{
	\	Sleep(10);
		
		iResult = MOlbSystemPre.GetRTabMounterComponent(0)->SafeMoveGantryPos(DEF_TABMOUNTER_LOAD_POS);
		if(iResult)
			break;

		Sleep(500);
		
		iResult = MOlbSystemPre.GetRTabMounterComponent(0)->SafeMoveGantryPos(DEF_TABMOUNTER_INSPECTION_POS);
		if(iResult)
			break;

		if(timer.MoreThan(3600 * 60 * 2))
			break;
	}
	*/
}

// NSMC
/*
LRESULT CAutoMainForm::OnDisplayLoadConveyorPanelInfo(UINT wParam, LPARAM lParam)
{
	CString strPanelInfo;
	strPanelInfo.Format(_T("%s"), m_plnkTrsAutoManager->m_pGlassData[DEF_LOAD_CONVEYOR_PANEL]->m_strEPanelID);

	m_LblLoadConveyorPanelInfo.SetCaption(strPanelInfo);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayPanelTransferPanelInfo(UINT wParam, LPARAM lParam)
{
	CString strPanelInfo;
	strPanelInfo.Format(_T("%s"), m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_TRANSFER_PANEL]->m_strEPanelID);

	m_LblPanelTransferPanelInfo.SetCaption(strPanelInfo);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayPanelAlignerPanelInfo(UINT wParam, LPARAM lParam)
{
	CString strPanelInfo;
	strPanelInfo.Format(_T("%s"), m_plnkTrsAutoManager->m_pGlassData[DEF_PANEL_ALIGNER_PANEL]->m_strEPanelID);

	m_LblPanelAlignerPanelInfo.SetCaption(strPanelInfo);
	return 0;
}

LRESULT CAutoMainForm::OnDisplayUnloadConveyorPanelInfo(UINT wParam, LPARAM lParam)
{
	CString strPanelInfo;
	strPanelInfo.Format(_T("%s"), m_plnkTrsAutoManager->m_pGlassData[DEF_UNLOAD_CONVEYOR_PANEL]->m_strEPanelID);

	m_LblUnloadConveyorPanelInfo.SetCaption(strPanelInfo);
	return 0;
}
*/

// NSMC
LRESULT CAutoMainForm::OnDisplayPanelID(UINT wParam, LPARAM lParam)
{
	int iWorkerID = (int)wParam;
	ASSERT(iWorkerID > -1 && iWorkerID < DEF_MAX_PANEL_UNIT);

	CString strGlassInfo;
	strGlassInfo = m_plnkTrsAutoManager->m_pGlassData[iWorkerID]->m_strHPanelID;
	m_rgLblWorkerPanelID[iWorkerID].SetCaption(strGlassInfo);

	// Doolin kwangilshin 2017. 09. 15.
	//
	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		if (m_rgLblWorkerPanelID[iWorkerID].GetCaption() == _T(""))
		{
			m_rgLblWorkerPanelID[iWorkerID].SetBackColor(STATIC_BACKGROUND);
		}
		else
		{
			m_rgLblWorkerPanelID[iWorkerID].SetBackColor(GREEN);
		}
	}
	// End.

	if (TRUE == m_plnkTrsAutoManager->IsLastGlassOnWorker(iWorkerID))
		m_rgLblWorkerPanelID[iWorkerID].SetForeColor(RED);
	else
		m_rgLblWorkerPanelID[iWorkerID].SetForeColor(BLACK);

	return 0;
}

LRESULT CAutoMainForm::OnDisplayPanelScrapDlg(UINT wParam, LPARAM lParam)
{
	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
		return 0;

	/*/
	//120223.kms_________
	MOlbSystemPre.GetIOComponent()->OutputOn(OUT_FRONT_DOOR_LOCK_OPEN);
	MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN);
	//__________________
	/*/

	/*/
	int iResult = m_plnkTrsAutoManager->UnlockFrontDoor();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return 0;
	}
	iResult = m_plnkTrsAutoManager->UnlockRearDoor();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return 0;
	}
	/*/

	// selsk_jdy CAutoScrapGlassDlg dlg(1);
	// selsk_jdy
	CAutoScrapGlassDlg dlg(1, this);
	dlg.DoModal();
	return 0;
}

LRESULT CAutoMainForm::OnDisplayPanelUnscrapDlg(UINT wParam, LPARAM lParam)
{
	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
		return 0;

	/*/
	//120223.kms_________
	MOlbSystemPre.GetIOComponent()->OutputOn(OUT_FRONT_DOOR_LOCK_OPEN);
	MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN);
	//__________________
	/*/

	/*/
	int iResult = m_plnkTrsAutoManager->UnlockFrontDoor();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return 0;
	}
	iResult = m_plnkTrsAutoManager->UnlockRearDoor();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return 0;
	}
	/*/

	// selsk_jdy CAutoScrapGlassDlg dlg(2);
	// selsk_jdy
	CAutoScrapGlassDlg dlg(2, this);
	dlg.DoModal();
	return 0;
}

LRESULT CAutoMainForm::OnDisplayMaterialInfoDlg(UINT wParam, LPARAM lParam)
{
	//2009.08.17 CYJ
//	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
//		return 0;

	CAutoMaterialInfoDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CAutoMainForm::OnStartTabFeederAutoHoming(UINT wParam, LPARAM lParam)
{
	//2009.06.17 CYJ
//	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
//		return 0;

	ASSERT(DEF_TABFEEDER1 == (int)wParam || DEF_TABFEEDER2 == (int)wParam);

  //새로운 Reel을 장착하고, TabIC 흡착 가능한 상태로 하였습니까?
//	if (MyMessageBox(MY_MESSAGE_BOX, 20004 , _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDNO) 
//		return 0;
	
	// selsk_jdy MOlbSystemPre.GetTabFeederComponent(wParam)->AbsorbNewReel(TRUE);
	MOlbSystemPre.GetTabFeederComponent((int)wParam)->AbsorbNewReel(TRUE);
	
//	흡착이 완료 되었습니까?
//	if (MyMessageBox(MY_MESSAGE_BOX, 20005 , _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDNO) 
//	return 0;
//MERGE_LKH_090704 : SESL_조재근S 버전 적용
//	m_plnkTrsAutoManager->SetMaterialExchange(FALSE);
	
//	양면테이프를 붙치고, TabIC를 Cutting 하세요. New Reel Vacuum은 항상 On 이어야 합니다.
//	if (MyMessageBox(MY_MESSAGE_BOX, 20006 , _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDNO) 
//	return 0;
	return 0;
}

LRESULT CAutoMainForm::OnMoveTabMounter(UINT wParam, LPARAM lParam)
{
	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
		return 0;

	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Move To Load Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Batch job is in progress.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	int iResult = SUCCESS;

	double tgPos[DEF_MAX_TABMOUNTER];
	double tgPosR[DEF_MAX_TABMOUNTER];
	
	/*/
#ifdef DEF_SOURCE_SYSTEM
	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
		if (i<DEF_MOUNTER_WORK_PER_GROUP)
			tgPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
		else
			tgPosR[DEF_MAX_TABMOUNTER-1-i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
	}
#else
	/*/
	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
		tgPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
		tgPosR[i] = MOlbSystemPre.GetRTabMounterComponent(i)->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
	}
//@#endif
	iResult = MOlbSystemPre.MoveAllTabMounter(tgPos, tgPosR, DEF_TABMOUNTER_LOAD_POS);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return 0;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return 0;
	}
	ProgWnd.DestroyWindow();

	// TabMounter Load 위치 이동 완료.
	MyMessageBox(MY_MESSAGE_BOX,20015, _T("Information"), M_ICONINFORMATION);
	return 0;
}

LRESULT CAutoMainForm::OnMoveInspectionCamera(UINT wParam, LPARAM lParam)
{
	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
		return 0;

	CMyProgressWnd ProgWnd(NULL, _T("Gantry Move To WorkPos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Batch job is in progress.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	int iResult = SUCCESS;

	iResult = MOlbSystemPre.GetTabMounterComponent(0)->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS, FALSE, FALSE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return 0;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return 0;
	}
	iResult = MOlbSystemPre.GetRTabMounterComponent(0)->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS, TRUE, FALSE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return 0;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return 0;
	}
	Sleep(50);
	iResult = MOlbSystemPre.GetTabMounterComponent(0)->Wait4Done();
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return 0;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return 0;
	}
	ProgWnd.DestroyWindow();	

	return 0;
}

LRESULT CAutoMainForm::OnRemoveTabIC(UINT wParam, LPARAM lParam)
{
	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
		return 0;

	sm_iRemoveTabICMode = 2;
	BOOL bAskQuestion = (BOOL)wParam;

	CSystemThreeBtnDlg dlg(_T("Select Remove Mode"), _T("Not Remove"), _T("Mounter\r\nTabIC"), _T("All\r\nTabIC"));
	if (TRUE == bAskQuestion)
	{
//@		//	설비가 흡착하고 있는 TabIC 를 모두 버리시겠습니까?
//@		if (MyMessageBox(MY_MESSAGE_BOX, 20009, _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
//@			return 0;
		
		//0->버리지 않음. 1->Mounter만, 2->Mounter & Carrier 모두
		sm_iRemoveTabICMode = dlg.DoModal();
		if (sm_iRemoveTabICMode == 0)
			return 0L;
	}

	int iResult = removeAllTabIC();
	if (iResult && NOT_USE_WORKSIDE != iResult)
	{
		CString strMsg;
		if (USER_STOP == iResult)
		{
//	  사용자 요청에 의해 작업을 중단함.
			MyMessageBox(MY_MESSAGE_BOX, 20010, _T("Information"), M_ICONINFORMATION);
			return 0;
		}
		else if (THERE_IS_NO_TAB_IC == iResult)
		{
			if (TRUE == (BOOL)lParam)
				return 0;

//		설비가 흡착하고 있는 TabIC 가 없음.
			MyMessageBox(MY_MESSAGE_BOX, 20011, _T("Information"), M_ICONINFORMATION);
			return 0;
		}

		if (ERR_PC_REBOOTING_REQUEST == iResult)
		{
//		PC 오동작 중. 프로그램 종료 후 PC Rebooting 해 주세요.
			MyMessageBox(MY_MESSAGE_BOX, 20012, _T("Error"));

			m_plnkTrsAutoManager->WriteMonitoringLog("Can not do Normalcy Thread Stop(During Remove TabIC).");
		}
		else
			strMsg = MOlbSystemPre.GetErrorMessage(iResult);
    
		MyMessageBox(strMsg, _T("Error"), M_ICONERROR);
		return 0;
	}

	if (TRUE == (BOOL)lParam)
		return 0;

//TabIC Remove 작업 완료.
	MyMessageBox(MY_MESSAGE_BOX, 20013, _T("Confirm"), M_ICONINFORMATION);
	return 0;
}

int CAutoMainForm::removeAllTabIC()
{
//	CMyProgressWnd ProgWnd(NULL, _T("Remove TabIC..."));
//	ProgWnd.SetRange(0, 2);
//	ProgWnd.SetText(_T("Batch job is in progress.\nWait a moment..."));
//	ProgWnd.PeekAndPump();
//	ProgWnd.StepIt();

	CWinThread* rgpThread[2];
	for (int i = 0; i < 2; i++)
	{
		sm_rgiRemoveResult[i] = SUCCESS;
		sm_rgbRemoveComplete[i] = FALSE;
		
		rgpThread[i] = AfxBeginThread(removeTabIC, (LPVOID)i);
		Sleep(1);
	}

	MTickTimer timerLimit;
	timerLimit.StartTimer();
	for (int i = 0; i < 2; i++)
	{
		while (TRUE)
		{
			if (NULL == rgpThread[i] || TRUE == sm_rgbRemoveComplete[i])
				break;

			Sleep(5);	// Message Pumping 은 하지 않음.

			if (TRUE == timerLimit.MoreThan(5 * 60.0))
			{
				// 발생할 수 없는 Case : 발생했다면 OS 문제로 봐야 함. [Thread 정상 종료 안됨.]
				// Thread 강제 종료는 하지 않음.
				return ERR_PC_REBOOTING_REQUEST;
			}
		}
	}

//	ProgWnd.DestroyWindow();

	for (int i = 0; i < 2; i++)
	{
		if (SUCCESS != sm_rgiRemoveResult[i])
			return sm_rgiRemoveResult[i];
	}

	return SUCCESS;
}

UINT CAutoMainForm::removeTabIC(LPVOID pParam)
{
	int iTabMounterGroupNo = (int)pParam;
	int i = 0;

//	if (TRUE == MOlbSystemPre.IsExistRemoveTab_TabMounter(iTabMounterGroupNo))
//	{
//		int iResult = MOlbSystemPre.RemoveTabIC(iTabMounterGroupNo, FALSE);
//		if (iResult)
//			sm_rgiRemoveResult[iTabMounterGroupNo] = iResult;
//	}
	BOOL bRemoveCarrier = FALSE;
	if (sm_iRemoveTabICMode == 2)
		bRemoveCarrier = TRUE;
	int iResult = MOlbSystemPre.RemoveTabIC(iTabMounterGroupNo, bRemoveCarrier);
	if (iResult)
		sm_rgiRemoveResult[iTabMounterGroupNo] = iResult;

	sm_rgbRemoveComplete[iTabMounterGroupNo] = TRUE;
	return 0;
}

LRESULT CAutoMainForm::OnDisplayCylinderMoveTimeDlg(UINT wParam, LPARAM lParam)
{
	//syr..SESL 090609 NSMC Test대신 원래의 기능으로 다시 복원시킴.

	//NSMC KJS 임시 Test 완료 후 제거
	/*WCHAR getbuf[128];
	if (!GetValueBox(getbuf, 10, _T(""), _T("Input Password. (Max 10)"), TRUE))
		return 0;
	
	CString szTemp = CCommonSelectUserGroupDlg::DecodePassword(m_plnkSystemData->m_strMakerPassWord);
	if (wcscmp(szTemp.GetBuffer(szTemp.GetLength()), getbuf))
	{
		//Operator Password 가 일치 하지 않습니다.
		MyMessageBox(MY_MESSAGE_BOX,60000, _T("Password Error"), M_ICONERROR);
		return 0;
	}
	if (MyMessageBox(_T("NSMC Test 모드를 진행하시겠습니까?"),_T("확인"),M_YESNO|M_ICONQUESTION) == IDYES)
		MOlbSystemPre.GetTrsAutoManagerComponent()->SetNSMCControlNo(2);
	else
		MOlbSystemPre.GetTrsAutoManagerComponent()->SetNSMCControlNo(1);

	return 0;*/

	if (m_bTimeDispFlag == FALSE)
	{
		m_pTimeDlg->Create(IDD_AUTO_ELAPSE_TIME, this);
		m_bTimeDispFlag = TRUE;
	}
	m_pTimeDlg->StartTimer();
	m_pTimeDlg->ShowWindow(SW_SHOW);
	return 0;
}

//110324________________
LRESULT CAutoMainForm::OnAutoUnitInitialize(WPARAM wParam, LPARAM lParam)
{
	int *iResult = (int*)lParam;
	*iResult = SUCCESS;
	int i = 0;

	// 초기화 작업 내용 확인.
	m_iInitialMode = -1;
#ifndef SIMULATION
	if (MOlbSystemPre.GetSystemData()->m_eRunMode != DRY_RUN_MODE
		&& MOlbSystemPre.GetSystemData()->m_eRunMode != PASS_RUN_MODE
		&& MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed() == TRUE)
	{
		m_iInitialMode = 1;
	}
	else if (MOlbSystemPre.GetSystemData()->m_eRunMode == DRY_RUN_MODE)
	{
		if (m_plnkTrsPanelAligner->IsDryRunPanelExist())
			m_iInitialMode = 1;
	}
#endif
	
	initializeProcessData();

	return 0;
}
//______________________

void CAutoMainForm::OnMaterialGroup() 
{
	// selsk_jdy CAutoMaterialCmdGroupDlg dlg(m_hWnd);
	CAutoMaterialCmdGroupDlg dlg(this);
	dlg.DoModal();
}

void CAutoMainForm::initializeProcessData()
{
	int i = 0;

	/*/
#ifdef DEF_SOURCE_SYSTEM
	BOOL bFeederReady = FALSE;
	bFeederReady = m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->GetFeederReady();
	MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP)->SetFeederReady(DEF_FRONT_GROUP, bFeederReady);
	bFeederReady = m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->GetFeederReady();
	MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP)->SetFeederReady(DEF_REAR_GROUP, bFeederReady);
#endif
	/*/

	int iScheduleNo[DEF_MAX_WORKER_GROUP] = {0, 0};
	
	MWorkSchedule* pWorkSchedule[DEF_MAX_WORKER_GROUP] = {NULL, NULL};
	MWorkSchedule* pWorkScheduleSec[DEF_MAX_WORKER_GROUP] = {NULL, NULL};
	for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
	{
		pWorkSchedule[i] = MOlbSystemPre.GetWorkSchedule(i);
#ifdef DEF_SOURCE_SYSTEM
		pWorkScheduleSec[i] = MOlbSystemPre.GetWorkScheduleSecond(i);
#endif
		if (FALSE == pWorkSchedule[i]->GetUseWorkSide())
			continue;

		if (i == 0)
			iScheduleNo[i] = MOlbSystemPre.GetTrsTabMounterComponent(0)->GetWorkScheduleNo(i);
		else
			iScheduleNo[i] = MOlbSystemPre.GetTrsRTabMounterComponent(0)->GetWorkScheduleNo(i);

		//120410.KMS________
		if (m_iInitialMode == 1)	//현재작업 유지
		{
			if (pWorkSchedule[i]->IsMoreWorkForCurrentPanel() == FALSE)
			{
#ifdef DEF_SOURCE_SYSTEM
				if (pWorkScheduleSec[i]->IsMoreWorkForCurrentPanel() == FALSE)	//@작업이 완료된 경우
				{
					m_iInitialMode = 2;	//PanelPass
					MOlbSystemPre.GetTrsPanelAlignerComponent()->Initialize(TRUE, TRUE);
					iScheduleNo[i] = 0;
				}
				else
				{
					iScheduleNo[i] = 1;
				}
#else
				m_iInitialMode = 2;	//PanelPass
				MOlbSystemPre.GetTrsPanelAlignerComponent()->Initialize(TRUE, TRUE);
				iScheduleNo[i] = 0;
#endif
			}
			else
				iScheduleNo[i] = 0;
			//__________________
		}
	}

	//@Front/Rear Schedule번호에서 작은번호를 구한다
	int iRealScheduleNo = 100;
	for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
	{
		if (TRUE == pWorkSchedule[i]->GetUseWorkSide())
		{
			if (iScheduleNo[i] < iRealScheduleNo)
				iRealScheduleNo = iScheduleNo[i];
		}
	}
	
	if (m_iInitialMode == 0)		//전체 초기화
	{
		iRealScheduleNo = 0;
		for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
		{
			iScheduleNo[i] = 0;
			pWorkSchedule[i]->ClearWorkSchedule();
			pWorkSchedule[i]->GenerateWorkSchedule();
#ifdef DEF_SOURCE_SYSTEM
			pWorkScheduleSec[i]->ClearWorkSchedule();
			pWorkScheduleSec[i]->GenerateWorkSchedule();
#endif
		}
		pWorkSchedule[0]->SetExistWorkPanel(TRUE);
	}
	else if (m_iInitialMode == 1)	//현재작업 유지
	{
		for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
		{
			if(TRUE == pWorkSchedule[i]->GetUseWorkSide())
			{
				pWorkSchedule[i]->GenerateWorkScheduleByPanel();
#ifdef DEF_SOURCE_SYSTEM
				pWorkScheduleSec[i]->GenerateWorkScheduleByPanel();
#endif
			}
		}
		pWorkSchedule[0]->SetExistWorkPanel(TRUE);
	}
	else if (m_iInitialMode == 2)	//PanelPass
	{
		iRealScheduleNo = 0;
		for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
		{
			iScheduleNo[i] = 0;
			pWorkSchedule[i]->ClearWorkSchedule();
#ifdef DEF_SOURCE_SYSTEM
			pWorkScheduleSec[i]->ClearWorkSchedule();
#endif
		}
		pWorkSchedule[0]->SetExistWorkPanel(FALSE);
	}
	else	//Panel이 없을경우
	{
		iRealScheduleNo = 0;
		for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
		{
			iScheduleNo[i] = 0;
			pWorkSchedule[i]->ClearWorkSchedule();
#ifdef DEF_SOURCE_SYSTEM
			pWorkScheduleSec[i]->ClearWorkSchedule();
#endif
		}
		pWorkSchedule[0]->SetExistWorkPanel(FALSE);
	}

//170719 JSH Trs Init할때로 변경
	/*/
	MOlbSystemPre.GetTrsPanelAlignerComponent()->SetWorkScheduleNo(iRealScheduleNo);
	for (int i=0; i<DEF_MAX_GROUP; i++)
	{
		for (int j=0; j<DEF_MAX_TABCARRIER; j++)
		{
			MOlbSystemPre.GetTrsTabCarrierComponent(j)->SetWorkScheduleNo(i, iScheduleNo[i]);
			MOlbSystemPre.GetTrsRTabCarrierComponent(j)->SetWorkScheduleNo(i, iScheduleNo[i]);
		}
		for (int j=0; j<DEF_MAX_TABMOUNTER; j++)
		{
			MOlbSystemPre.GetTrsTabMounterComponent(j)->SetWorkScheduleNo(i, iScheduleNo[i]);
			MOlbSystemPre.GetTrsRTabMounterComponent(j)->SetWorkScheduleNo(i, iScheduleNo[i]);
		}
		for (int j=0; j<DEF_MAX_INSPECTION_CAM_CARRIER; j++)
		{
			MOlbSystemPre.GetTrsInspectionCameraComponent(j)->SetWorkScheduleNo(i, iScheduleNo[i]);
			MOlbSystemPre.GetTrsRInspectionCameraComponent(j)->SetWorkScheduleNo(i, iScheduleNo[i]);
		}
	}
	/*/

	/*/
	int iScheduleNo = 0;
	MWorkSchedule* pWorkSchedule[DEF_MAX_WORK_SCHEDULE][DEF_MAX_WORKER_GROUP] =
	{
		{MOlbSystemPre.GetWorkSchedule(0), MOlbSystemPre.GetWorkSchedule(1)},
#ifdef DEF_SOURCE_SYSTEM
		{MOlbSystemPre.GetWorkScheduleSecond(0), MOlbSystemPre.GetWorkScheduleSecond(1)},
#endif
	};
	for (int i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
	{
		for (int j=0; j<DEF_MAX_WORKER_GROUP; j++)
		{
			if (FALSE == pWorkSchedule[i][j]->GetUseWorkSide())
			{
				pWorkSchedule[i][j]->ClearWorkSchedule();
				pWorkSchedule[i][j]->SetExistWorkPanel(FALSE);
				continue;
			}

			//120410.KMS________
			if (m_iInitialMode == 1)	//현재작업 유지
			{
				if (pWorkSchedule[i][j]->IsMoreWorkForCurrentPanel() == FALSE)
				{
#ifdef DEF_SOURCE_SYSTEM
					if (i == 0)	//첫번째 Schedule작업이 끝났으면
					{
						if (TRUE == pWorkSchedule[i+1][j]->IsMoreWorkForCurrentPanel())	//두번째 Schedule에 작업이 남아 있으면
							iScheduleNo = 1;
						else
							iScheduleNo = 0;
					}
#endif
					if (iScheduleNo == 0)
					{
						m_iInitialMode = 2;	//PanelPass
						MOlbSystemPre.GetTrsPanelAlignerComponent()->Initialize(TRUE, TRUE);
					}
					else
						MOlbSystemPre.GetTrsPanelAlignerComponent()->Initialize(FALSE, FALSE);
				}
				else
					iScheduleNo = 0;
				//__________________
			}

			if (m_iInitialMode == 0)		//전체 초기화
			{
				pWorkSchedule[i][j]->ClearWorkSchedule();
				pWorkSchedule[i][j]->GenerateWorkSchedule();
				pWorkSchedule[i][j]->SetExistWorkPanel(TRUE);
			}
			else if (m_iInitialMode == 1)	//현재작업 유지
			{
				if (i == 1)
				{
					if (iScheduleNo == 1)
					{
						pWorkSchedule[i][j]->GenerateWorkScheduleByPanel();
						pWorkSchedule[i][j]->SetExistWorkPanel(TRUE);
					}
				}
				else
				{
					pWorkSchedule[i][j]->GenerateWorkScheduleByPanel();
					pWorkSchedule[i][j]->SetExistWorkPanel(TRUE);
				}
			}
			else if (m_iInitialMode == 2)	//PanelPass
			{
				pWorkSchedule[i][j]->ClearWorkSchedule();
				pWorkSchedule[i][j]->SetExistWorkPanel(FALSE);
			}
			else	//Panel이 없을경우
			{
				pWorkSchedule[i][j]->ClearWorkSchedule();
				pWorkSchedule[i][j]->SetExistWorkPanel(FALSE);
			}
		}
	}
	/*/
}

void CAutoMainForm::OnOperationGroup() 
{
	// selsk_jdy CAutoOperationCmdGroupDlg dlg(m_hWnd);
	// selsk_jdy
	// Door Open 되어 이있으면 동작 안됨
	//
	/*
	if (m_plnkTrsAutoManager->IsAllDoorOpen())
	{
		MyMessageBox(_T("전체 Door가 Open되어 있습니다."), _T("Confirm"));
		ClickInterlockMode();
		return;
	}
	*/

	if (m_plnkTrsAutoManager->IsFrontDoorOpen())
	{
		MyMessageBox(_T("The front door is open.\r\nFront Door가 Open되어 있습니다."), _T("Confirm"));
		ClickInterlockMode();
		return;
	}

	if (m_plnkTrsAutoManager->IsRearDoorOpen())
	{
		MyMessageBox(_T("Rear Door is open.\r\nRear Door가 Open되어 있습니다."), _T("Confirm"));
		ClickInterlockMode();
		return;
	}

	if (m_plnkTrsAutoManager->IsMeterialDoorOpenFront())
	{
		MyMessageBox(_T("Front Material Door is opened.\r\nFront 자재 Door가 Open되어 있습니다."), _T("Confirm"));
		ClickInterlockMode();
		return;
	}
	if (m_plnkTrsAutoManager->IsMeterialDoorOpenRear())
	{
		MyMessageBox(_T("Rear Material Door is open.\r\nRear 자재 Door가 Open되어 있습니다."), _T("Confirm"));
		ClickInterlockMode();
		return;
	}

	CAutoOperationCmdGroupDlg dlg(this);
	dlg.DoModal();
}

void CAutoMainForm::OnClickPanelWait() 
{
	/*
	//For Test
	if(MOlbSystemPre.GetCtrlTabFeederComponent(0)->GetBadTabCntInterlock() == TRUE)
	{
		MyMessageBox(_T("BadCount 가  이상 합니다."),_T("Bad Count Error"), M_ICONERROR);
		return;
	}
	else
	{
		MyMessageBox(_T("BadCount 가  정상 합니다."),_T("Bad Count Error"), M_ICONERROR);
		return;
	}

	return;
	*/


	/*/_______________________
	MOlbSystemPre.GetDBTabLoss(0)->AppendFieldCount(eTP);
//@	MOlbSystemPre.GetDBTabLoss(0)->AppendFieldCount(eCA1);
	MOlbSystemPre.GetDBTabLoss(0)->AppendFieldCount(eCA2D);
	MOlbSystemPre.GetDBTabLoss(0)->AppendFieldCount(eTM3);

	MOlbSystemPre.GetDBTabLoss(1)->AppendFieldCount(eTP);
	MOlbSystemPre.GetDBTabLoss(1)->AppendFieldCount(eCA2);
//@	MOlbSystemPre.GetDBTabLoss(1)->AppendFieldCount(eCA3D);
	MOlbSystemPre.GetDBTabLoss(1)->AppendFieldCount(eTM4);
	________________________/*/

	//130410. SJ_YTH. Del...
//	m_plnkTrsAutoManager->ProcessParamReport();
//	return;
	//______________________________


#ifndef SIMULATION
	if (m_plnkTrsAutoManager->GetOPMode() == MANUAL_MODE
		//|| m_plnkSystemData->m_eRunMode == DRY_RUN_MODE
		|| m_plnkSystemData->m_eRunMode == REPAIR_RUN_MODE)
	{
		// 임시 TEST
#	if 0	//#ifdef DEF_SETUP
		int iResult = 0;
		int iCount = 0;
		CString strLog;
		enableButton(FALSE);

		CMyProgressWnd ProgWnd(NULL, "Inspection Camera Test...");
		ProgWnd.SetRange(0, 1);
		ProgWnd.SetText("If you want to stop, press Stop Switch.");
		ProgWnd.PeekAndPump();

		while (TRUE)
		{
			iCount++;

			iResult = m_plnkVision->Grab(DEF_F_INSPECTION_CAMERA_1);
			if (iResult)
			{
				strLog.Format("F_IC1 Grab Error[%d], Count[%d]", iResult, iCount);
				m_plnkVision->WriteLog("..\\Log\\ictest_log.txt", strLog.GetBuffer(strLog.GetLength()));
				break;
			}
			iResult = m_plnkVision->GrabWait(DEF_F_INSPECTION_CAMERA_1);
			if (iResult)
			{
				strLog.Format("F_IC1 Grab Wait Error[%d], Count[%d]", iResult, iCount);
				m_plnkVision->WriteLog("..\\Log\\ictest_log.txt", strLog.GetBuffer(strLog.GetLength()));
				break;
			}

			iResult = m_plnkVision->Grab(DEF_F_INSPECTION_CAMERA_2);
			if (iResult)
			{
				strLog.Format("F_IC2 Grab Error[%d], Count[%d]", iResult, iCount);
				m_plnkVision->WriteLog("..\\Log\\ictest_log.txt", strLog.GetBuffer(strLog.GetLength()));
				break;
			}
			iResult = m_plnkVision->GrabWait(DEF_F_INSPECTION_CAMERA_2);
			if (iResult)
			{
				strLog.Format("F_IC2 Grab Wait Error[%d], Count[%d]", iResult, iCount);
				m_plnkVision->WriteLog("..\\Log\\ictest_log.txt", strLog.GetBuffer(strLog.GetLength()));
				break;
			}

			if (TRUE == MOlbSystemPre.GetOpPanelComponent()->GetStopButtonStatus())
			{
				strLog.Format("User Break, Count[%d]", iCount);
				m_plnkVision->WriteLog("..\\Log\\ictest_log.txt", strLog.GetBuffer(strLog.GetLength()));
				break;
			}

			Sleep(10);
		}
		enableButton(TRUE);
#	endif
		return;
	}
#endif

	//BOOL bPanelInStop = !m_plnkTrsLoadConveyor->IsPanelInStop();
	//m_plnkTrsLoadConveyor->SetPanelInStop(bPanelInStop);
/*	BOOL bPanelInStop = !m_plnkTrsPanelTransferCenter->IsPanelInStop();
	m_plnkTrsPanelTransferCenter->SetPanelInStop(bPanelInStop);

	if (TRUE == bPanelInStop)
	{
		m_ctrlLblOpStatus.SetCaption(_T("Panel In Stop"));
		m_ctrlLblOpStatus.SetForeColor(RED);
	}
	else
		SendMessage(WM_AUTO_STATUS, m_plnkTrsAutoManager->GetOPMode(), 0);*/
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TURN_HANDLER
	BOOL bPanelInStop = !m_plnkTrsPanelTransferCenter->IsPanelInStop();
	m_plnkTrsPanelTransferCenter->SetPanelInStop(bPanelInStop);
#	else
	BOOL bPanelInStop = !m_plnkTrsPanelTransfer->IsPanelInStop();
	m_plnkTrsPanelTransfer->SetPanelInStop(bPanelInStop);
#	endif
	//_____________
#else
	BOOL bPanelInStop = !m_plnkTrsPanelAligner->IsPanelInStop();
	m_plnkTrsPanelAligner->SetPanelInStop(bPanelInStop);
#endif

	if (TRUE == bPanelInStop)
	{
		m_ctrlLblOpStatus.SetCaption(_T("Panel In Stop"));
	    m_ctrlLblOpStatus.SetForeColor(RED);
	}
	else
		SendMessage(WM_AUTO_STATUS, m_plnkTrsAutoManager->GetOPMode(), 0);
}

UINT CAutoMainForm::initTabMounter(LPVOID pParam)
{
	int i = -1;
	int iResult = SUCCESS;
	int iTabMounterGroupID = (int)pParam;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		iResult = MOlbSystemPre.GetTrsTabMounterComponent(i)->Initialize();
		if (iResult)
		{
			sm_rgiTabMounterInitResult[iTabMounterGroupID] = iResult;
			sm_rgbTabMounterInitComplete[iTabMounterGroupID] = TRUE;
			return 0;
		}
	}
	for (int i=DEF_MAX_TABMOUNTER-1; i>=DEF_MOUNTER_WORK_PER_GROUP; i--)
	{
		iResult = MOlbSystemPre.GetTrsTabMounterComponent(i)->Initialize();
		if (iResult)
		{
			sm_rgiTabMounterInitResult[iTabMounterGroupID] = iResult;
			sm_rgbTabMounterInitComplete[iTabMounterGroupID] = TRUE;
			return 0;
		}
	}

	iResult = MOlbSystemPre.GetTabMounterComponent(DEF_MOUNTER_WORK_PER_GROUP-1)->Wait4Done();
	if (iResult)
	{
		sm_rgiTabMounterInitResult[iTabMounterGroupID] = iResult;
		sm_rgbTabMounterInitComplete[iTabMounterGroupID] = TRUE;
	}

	iResult = MOlbSystemPre.GetTabMounterComponent(DEF_MOUNTER_WORK_PER_GROUP)->Wait4Done();
	if (iResult)
	{
		sm_rgiTabMounterInitResult[iTabMounterGroupID] = iResult;
		sm_rgbTabMounterInitComplete[iTabMounterGroupID] = TRUE;
	}
#else
	/*/

	double dTgPos[DEF_MOUNTER_WORK_PER_GROUP] = {0.0, };

	for (int i=DEF_MOUNTER_WORK_PER_GROUP-1; i>=0; i--)
	{
		if (DEF_FRONT_GROUP == iTabMounterGroupID)
		{
			iResult = MOlbSystemPre.GetTrsTabMounterComponent(i)->Initialize();
			dTgPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
		}
		else
		{
			iResult = MOlbSystemPre.GetTrsRTabMounterComponent(i)->Initialize();
			dTgPos[i] = MOlbSystemPre.GetRTabMounterComponent(i)->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
		}
		if (iResult)
		{
			sm_rgiTabMounterInitResult[iTabMounterGroupID] = iResult;
			sm_rgbTabMounterInitComplete[iTabMounterGroupID] = TRUE;
			return 0;
		}
	}

	//@130204.Modify___________________
	/*/
	if (DEF_FRONT_GROUP == iTabMounterGroupID)
		iResult = MOlbSystemPre.GetTabMounterComponent(0)->Wait4Done();
	else
		iResult = MOlbSystemPre.GetRTabMounterComponent(0)->Wait4Done();
	/*/
	if (DEF_FRONT_GROUP == iTabMounterGroupID)
		iResult = MOlbSystemPre.MoveAllFrontTabMounter(dTgPos, DEF_TABMOUNTER_READY_POS);
	else
		iResult = MOlbSystemPre.MoveAllRearTabMounter(dTgPos, DEF_TABMOUNTER_READY_POS);
	//@________________________________

	if (iResult)
	{
		sm_rgiTabMounterInitResult[iTabMounterGroupID] = iResult;
		sm_rgbTabMounterInitComplete[iTabMounterGroupID] = TRUE;
	}
	
//@#endif

	//170829 JSH.s
	{
		double dTarget;
		dTarget = MOlbSystemPre.GetTabMounterComponent(iTabMounterGroupID)->GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_LOAD_POS);
		iResult = MOlbSystemPre.GetTabMounterComponent(iTabMounterGroupID)->MoveGantryPos(dTarget, DEF_TABMOUNTER_GANTRY_LOAD_POS);
		
		if (iResult)
		{
			sm_rgiTabMounterInitResult[iTabMounterGroupID] = iResult;
			sm_rgbTabMounterInitComplete[iTabMounterGroupID] = TRUE;
		}
	}
	//170829 JSH.e
	
	sm_rgbTabMounterInitComplete[iTabMounterGroupID] = TRUE;

	return 0;
}

UINT CAutoMainForm::initInspectionCamera(LPVOID pParam)
{
	int iInspectionCameraGroupID = (int)pParam;
	if (DEF_FRONT_GROUP == iInspectionCameraGroupID)
	{
		int rgiFrontInspectionCamOrder[DEF_MAX_INSPECTION_CAM_PER_GROUP] = {
//#ifdef DEF_SOURCE_SYSTEM
//			DEF_INSPECTION_CAMERA1, DEF_INSPECTION_CAMERA2
//#else
//			DEF_INSPECTION_CAMERA1, DEF_INSPECTION_CAMERA3, DEF_INSPECTION_CAMERA2
//#endif

#ifdef DEF_SOURCE_SYSTEM
			DEF_INSPECTION_CARRIER_1, DEF_INSPECTION_CARRIER_2,	DEF_INSPECTION_CARRIER_3, DEF_INSPECTION_CARRIER_4,
#else
			DEF_INSPECTION_CARRIER_1, DEF_INSPECTION_CARRIER_2,
#endif
		};
		
		for (int i = 0; i < DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
		{
			int iResult = MOlbSystemPre.GetTrsInspectionCameraComponent(rgiFrontInspectionCamOrder[i])->Initialize();
			if (iResult)
			{
				sm_rgiInspectionCameraInitResult[iInspectionCameraGroupID] = iResult;
				sm_rgbInspectionCameraInitComplete[iInspectionCameraGroupID] = TRUE;//KKY.Add
				return 0;
			}
		}		
	}
	else
	{
		int rgiFrontInspectionCamOrder[DEF_MAX_INSPECTION_CAM_PER_GROUP] = {
//#ifdef DEF_SOURCE_SYSTEM
//			DEF_INSPECTION_CAMERA1, DEF_INSPECTION_CAMERA2
//#else
//			DEF_INSPECTION_CAMERA1, DEF_INSPECTION_CAMERA3, DEF_INSPECTION_CAMERA2
//#endif
#ifdef DEF_SOURCE_SYSTEM
			DEF_INSPECTION_CARRIER_1, DEF_INSPECTION_CARRIER_2,	DEF_INSPECTION_CARRIER_3, DEF_INSPECTION_CARRIER_4,
#else
			DEF_INSPECTION_CARRIER_1, DEF_INSPECTION_CARRIER_2,
#endif
		};
		
		for (int i = 0; i < DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
		{
			int iResult = MOlbSystemPre.GetTrsRInspectionCameraComponent(rgiFrontInspectionCamOrder[i])->Initialize();
			if (iResult)
			{
				sm_rgiInspectionCameraInitResult[iInspectionCameraGroupID] = iResult;
				sm_rgbInspectionCameraInitComplete[iInspectionCameraGroupID] = TRUE;//KKY.Add
				return 0;
			}
		}		
	}

	sm_rgbInspectionCameraInitComplete[iInspectionCameraGroupID] = TRUE;
	return 0;
}

UINT CAutoMainForm::initTabCarrier(LPVOID pParam)
{
	int iCarrierID = (int)pParam;
	/*
	int iResult = MOlbSystemPre.GetTrsTabCarrierComponent(iCarrierID)->Initialize();
	if (iResult)
		sm_rgiTabCarrierInitResult[iCarrierID] = iResult;

	sm_rgbTabCarrierInitComplete[iCarrierID] = TRUE;
	*/
	for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
	{
		int iResult = MOlbSystemPre.GetTrsTabCarrierComponent(i)->Initialize();
		if (iResult)
		{
			sm_rgiTabCarrierInitResult = iResult;
			sm_rgbTabCarrierInitComplete = TRUE;
			return 0;
		}
	}
	sm_rgbTabCarrierInitComplete = TRUE;
	return 0;
}

UINT CAutoMainForm::initRTabCarrier(LPVOID pParam)
{
	int iCarrierID = (int)pParam;
	/*
	int iResult = MOlbSystemPre.GetTrsTabCarrierComponent(iCarrierID)->Initialize();
	if (iResult)
		sm_rgiTabCarrierInitResult[iCarrierID] = iResult;

	sm_rgbTabCarrierInitComplete[iCarrierID] = TRUE;
	*/
	for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
	{
		int iResult = MOlbSystemPre.GetTrsRTabCarrierComponent(i)->Initialize();
		if (iResult)
		{
			sm_rgiRTabCarrierInitResult = iResult;
			sm_rgbRTabCarrierInitComplete = TRUE;
			return 0;
		}
	}
	sm_rgbRTabCarrierInitComplete = TRUE;
	return 0;
}

UINT CAutoMainForm::initTabFeeder(LPVOID pParam)
{
	int iTabFeederNo = (int)pParam;
//	int iResult = MOlbSystemPre.GetTrsTabFeederComponent(iTabFeederNo)->Initialize();
	int iResult = MOlbSystemPre.GetTrsTabFeederComponent(DEF_TABFEEDER1)->Initialize();
	if (iResult)
		sm_rgiTabFeederInitResult = iResult;
	
	sm_rgbTabFeederInitComplete = TRUE;
	return 0;
}

UINT CAutoMainForm::initRTabFeeder(LPVOID pParam)
{
	int iTabFeederNo = (int)pParam;
//	int iResult = MOlbSystemPre.GetTrsTabFeederComponent(iTabFeederNo)->Initialize();
	int iResult = MOlbSystemPre.GetTrsTabFeederComponent(DEF_TABFEEDER2)->Initialize();
	if (iResult)
		sm_rgiRTabFeederInitResult = iResult;
	
	sm_rgbRTabFeederInitComplete = TRUE;
	return 0;
}

UINT CAutoMainForm::initPanelTransfer(LPVOID pParam)
{
#ifdef DEF_USE_TRANSFER_CENTER
	//int iResult = MOlbSystemPre.GetTrsPanelTransferComponent()->Initialize();
	int iResult = MOlbSystemPre.GetTrsPanelTransferCenterComponent()->Initialize();
	if (iResult)
		sm_rgiPanelTransferCenterInitResult = iResult;

	sm_rgbPanelTransferCenterInitComplete = TRUE;
#endif
	return 0;
}

UINT CAutoMainForm::initPanelTransferOut(LPVOID pParam)
{
	int iResult = MOlbSystemPre.GetTrsPanelTransferOutComponent()->Initialize();
	if (iResult)
		sm_rgiPanelTransferOutInitResult = iResult;

	sm_rgbPanelTransferOutInitComplete = TRUE;
	return 0;
}

#ifdef DEF_USE_TURN_HANDLER
//SJ_YYK 161104 Add..
UINT CAutoMainForm::initTurnHandler(LPVOID pParam)
{
	int iResult = MOlbSystemPre.GetTrsTHandlerComponent()->Initialize();
	if (iResult)
		sm_rgiTurnHandlerInitResult = iResult;

	sm_rgbTurnHandlerInitComplete = TRUE;
	return 0;
}
#endif

LRESULT CAutoMainForm::OnDisplayLanguage(UINT wParam, LPARAM lParam)
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;

//	생산 정보
	SetTitleLT(&m_lblProductInfo, MAIN_AUTO, _T("AutoMainForm_7"));
//	운영 정보
	SetTitleLT(&m_lblRunningInfo, MAIN_AUTO, _T("AutoMainForm_8"));
//	유닛수행정보
	SetTitleLT(&m_lblUnitInfo, MAIN_AUTO, _T("AutoMainForm_9"));

//	생산 모델
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_10"), &szTextTemp);	
	GetDlgItem(IDB_MODEL_NAME)->SetWindowText(szTextTemp);
//	설비 Tact Time
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_11"), &szTextTemp);	
	GetDlgItem(IDB_EQ_TACT_TIME)->SetWindowText(szTextTemp);
//	라인 Tact Time
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_12"), &szTextTemp);	
	GetDlgItem(IDB_LINE_TACT_TIME)->SetWindowText(szTextTemp);
//	생산 수량
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_13"), &szTextTemp);	
	GetDlgItem(IDB_PRODUCT_COUNT)->SetWindowText(szTextTemp);

//	운전상태
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_14"), &szTextTemp);	
	GetDlgItem(IDB_OPERATION_STATUS)->SetWindowText(szTextTemp);
//	운전모드;
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_15"), &szTextTemp);	
	GetDlgItem(IDB_OPERATION_MODE)->SetWindowText(szTextTemp);
//	가압 시간
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_16"), &szTextTemp);	
	GetDlgItem(IDB_PREBONDING_TIME)->SetWindowText(szTextTemp);
//	Run Mode\r\n선택
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_17"), &szTextTemp);	
	GetDlgItem(IDB_RUN_MODE)->SetWindowText(szTextTemp);
//	축 원점복귀
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_18"), &szTextTemp);	
	GetDlgItem(IDB_ORIGIN_RETURN)->SetWindowText(szTextTemp);
//	유닛 초기화
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_19"), &szTextTemp);	
	GetDlgItem(IDB_UNIT_INITIAL)->SetWindowText(szTextTemp);
//	시스템 준비
	SetTitleCB(&m_btnReady, MAIN_AUTO, _T("AutoMainForm_20"));
//	Panel\r\n공급중단
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_21"), &szTextTemp);	
	GetDlgItem(IDB_PANEL_WAIT)->SetWindowText(szTextTemp);
//	자재 관련
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_22"), &szTextTemp);	
	GetDlgItem(IDB_MATERIAL_GROUP)->SetWindowText(szTextTemp);
//	설비 InterLock	
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_23"), &szTextTemp);	
	GetDlgItem(IDB_INTERLOCK_MODE)->SetWindowText(szTextTemp);

// Doolin YoonDaeRoh 2017.08.30.
//
//	EQ State.
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_26"), &szTextTemp);	
	GetDlgItem(IDB_EQ_STATE)->SetWindowText(szTextTemp);

//	EQP State.
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_27"), &szTextTemp);	
	GetDlgItem(IDB_EQP_STATE)->SetWindowText(szTextTemp);

// 투입/스크랩 수량
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_30"), &szTextTemp);	
	GetDlgItem(IDB_PANEL_IN_SCRAP_QTY)->SetWindowText(szTextTemp);

// End.

	return 0;
}

void CAutoMainForm::ClickInterlockMode()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CAutoDoorStateDlg dlgDoorState(0);
	dlgDoorState.DoModal();
}

void CAutoMainForm::OnClickMatchingTable()
{
	CDataMatchingTableDlg dlgMatchingTable(m_plnkSystemData->m_strModelFileDir);
	dlgMatchingTable.DoModal();
}

void CAutoMainForm::OnClickNSMCInfo()
{
	// TODO: Add your control notification handler code here
	if (m_bNSMCInfoFlag == FALSE)
	{
		m_pNSMCInfoDlg->Create(IDD_AUTO_NSMC_INFO, this);
		m_bNSMCInfoFlag = TRUE;
	}
	m_pNSMCInfoDlg->StartDlgTimer();
	m_pNSMCInfoDlg->ShowWindow(SW_SHOW);
}

void CAutoMainForm::OnClickNSMCTabICChange()
{
	if (TRUE == m_plnkTrsAutoManager->GetNSMCTabICChange())
	{
//		m_btnNSMCTabICChange.SetBackColorInterior(BASIC);
		m_plnkTrsAutoManager->SetNSMCTabICChange(FALSE);
	}
	else
	{
//		m_btnNSMCTabICChange.SetBackColorInterior(CYAN);
		m_plnkTrsAutoManager->SetNSMCTabICChange(TRUE);
	}

	SendMessage(WM_AUTO_STATUS, m_plnkTrsAutoManager->GetOPMode(), 0);
}

//SESL_요구사항적용 : Mold Count Reset
void CAutoMainForm::ClickMoldCount1()
{
#ifdef DEF_SESL_SITE
	if (m_plnkTrsAutoManager->GetOPMode() == MANUAL_MODE)
	{
		//Feeder 1의 Mold Count를 초기화 하시겠습니까?
		if( MyMessageBox(MY_MESSAGE_BOX,20016,_T("Reset Mold Count"),M_YESNO|M_ICONQUESTION) == IDYES )
		{
//			m_plnkTrsAutoManager->ResetMold1Count();
//			m_ctrlMold1.SetCaption(_T("0 / 300000"));

			//2009.08.11 CYJ
			m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->m_matZigInfo.m_nU_QTY = 0;
			m_plnkProductInfoData->m_matZigInfo[DEF_TABFEEDER1].m_nU_QTY = 0;
			m_plnkProductInfoData->WriteMaterialData();
		}
	}
#endif		
}


void CAutoMainForm::ClickMoldCount2()
{
#if defined DEF_GATE_SYSTEM && defined DEF_SESL_SITE
	if (m_plnkTrsAutoManager->GetOPMode() == MANUAL_MODE)
	{
		//Feeder 2의 Mold Count를 초기화 하시겠습니까?
		if( MyMessageBox(MY_MESSAGE_BOX,20017,_T("Reset Mold Count"),M_YESNO|M_ICONQUESTION) == IDYES )
		{
//			m_plnkTrsAutoManager->ResetMold2Count();
//			m_ctrlMold2.SetCaption(_T("0 / 300000"));

			//2009.08.11 CYJ
			m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->m_matZigInfo.m_nU_QTY = 0;
			m_plnkProductInfoData->m_matZigInfo[DEF_TABFEEDER2].m_nU_QTY = 0;
			m_plnkProductInfoData->WriteMaterialData();
		}
	}
#endif
}

LRESULT	CAutoMainForm::OnAllInspectCamMoveAlign(WPARAM wParam, LPARAM lParam)
{
	/*미사용 함수
	double tgPos[DEF_MAX_INSPECTION_CAM_CARRIER];
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		tgPos[i] = MOlbSystemPre.GetCameraCarrierComponent(i)->GetTargetPos(DEF_CAMERACARRIER_PANEL_MARK_POS);
	int iResult = MOlbSystemPre.MoveAllGroupCameraCarrier(tgPos);
	if (iResult)
		return iResult;
	*/
	return 0;
}

void CAutoMainForm::OnClickPmMode() 
{
	//MOlbSystemPre.GetCtrlTabFeederComponent(DEF_REAR_GROUP)->SetHomeFlagSprocket2(FALSE);	
	// TODO: Add your control notification handler code here
	if (MAKER_GROUP != MOlbSystemPre.GetCurrentUserGroup())
	{
		// 이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX,50300,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CPMFullDisplay Dlg;
	Dlg.DoModal();
}

void CAutoMainForm::OnClickReelready1()
{
	// TODO: Add your control notification handler code here
	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		BOOL bReady = !m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->GetFeederReady();
		m_plnkCtrlTabFeeder[DEF_TABFEEDER1]->SetFeederReady4DryRun(bReady);
	}
}

void CAutoMainForm::OnClickReelready2()
{
	// TODO: Add your control notification handler code here
	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		BOOL bReady = !m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->GetFeederReady();
		m_plnkCtrlTabFeeder[DEF_TABFEEDER2]->SetFeederReady4DryRun(bReady);
	}
}

void CAutoMainForm::OnClickModelName() 
{
	// TODO: Add your control notification handler code here
	/*/
	CString szTextTemp;
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMainForm_10"), &szTextTemp);
	if (FALSE == m_plnkSystemData->m_bUseDryrunFeed)
	{
		m_plnkSystemData->m_bUseDryrunFeed = TRUE;
		szTextTemp += _T("*");
	}
	else
	{
		m_plnkSystemData->m_bUseDryrunFeed = FALSE;
	}
	GetDlgItem(IDB_MODEL_NAME)->SetWindowText(szTextTemp);
	/*/
	CModelNDeviceIDListDlg dlg2(0);
	dlg2.DoModal();
}


void CAutoMainForm::OnDisplayPanelAbsorbState() 
{
	//120201.KMS__________
	//현재 Panel  이 어디에 있는지 확인 하기 위한 ...............
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER	
	if(MOlbSystemPre.GetPanelTransferCenterComponent()->IsPanelAbsorbed())
	{
		if(m_AFlgTransferCenter == FALSE)
		{
			m_AFlgTransferCenter = TRUE;
			m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_CENTER_PANEL].SetBackColor(GREEN);
		}
	}
	else
	{
		if(m_AFlgTransferCenter == TRUE)
		{
			m_AFlgTransferCenter = FALSE;
			m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_CENTER_PANEL].SetBackColor(STATIC_BACKGROUND);
		}
	}
#	endif
#	ifdef DEF_USE_TURN_HANDLER
	if(MOlbSystemPre.GetTurnHandlerComponent()->IsAbsorbPanel())
	{
		if(m_AFlgTurnHandler == FALSE)
		{
			m_AFlgTurnHandler = TRUE;
			m_rgLblWorkerPanelID[DEF_THANDLER_PANEL].SetBackColor(GREEN);
		}
	}
	else
	{
		if(m_AFlgTurnHandler == TRUE)
		{
			m_AFlgTurnHandler = FALSE;
			m_rgLblWorkerPanelID[DEF_THANDLER_PANEL].SetBackColor(STATIC_BACKGROUND);
		}
	}
#	endif
	

	if(MOlbSystemPre.GetPanelTransferComponent()->IsPanelAbsorbed())
	{
		if(m_AFlgTransferIn == FALSE)
		{
			m_AFlgTransferIn = TRUE;
			m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_PANEL].SetBackColor(GREEN);
		}
	}
	else
	{
		if(m_AFlgTransferIn == TRUE)
		{
			m_AFlgTransferIn = FALSE;
			m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_PANEL].SetBackColor(STATIC_BACKGROUND);
		}
	}
#endif

	if(MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed())
	{
		if(m_AFlgPanelAligner == FALSE)
		{
			m_AFlgPanelAligner = TRUE;
			m_rgLblWorkerPanelID[DEF_PANEL_ALIGNER_PANEL].SetBackColor(GREEN);
		}
	}
	else
	{
		if(m_AFlgPanelAligner == TRUE)
		{
			m_AFlgPanelAligner = FALSE;
			m_rgLblWorkerPanelID[DEF_PANEL_ALIGNER_PANEL].SetBackColor(STATIC_BACKGROUND);
		}
	}

	if(MOlbSystemPre.GetPanelTransferOutComponent()->IsPanelAbsorbed())
	{
		if(m_AFlgTransferOut == FALSE)
		{
			m_AFlgTransferOut = TRUE;
			m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_OUT_PANEL].SetBackColor(GREEN);
		}
	}
	else
	{
		if(m_AFlgTransferOut == TRUE)
		{
			m_AFlgTransferOut = FALSE;
			m_rgLblWorkerPanelID[DEF_PANEL_TRANSFER_OUT_PANEL].SetBackColor(STATIC_BACKGROUND);
		}
	}
	//____________________
}
//110620.KKY_____
//생산 정보 Display화면
LRESULT CAutoMainForm::OnShowProductPlanDlg(UINT wParam, LPARAM lParam)
{
	if (m_pDlgProductPlan == NULL)
	{
		m_pDlgProductPlan = new CProductPlanDlg(this);
		m_pDlgProductPlan->Create(IDD_PRODUCT_PLAN, this);
	}
	m_pDlgProductPlan->CenterWindow(this);
	m_pDlgProductPlan->ShowWindow(SW_SHOW);

	return 0;
}

//자재 정보 Display화면
LRESULT CAutoMainForm::OnShowMaterialCodeInfoDlg(UINT wParam, LPARAM lParam)
{
//	CDlgSheetMaterial dlg(this);
//	dlg.DoModal();
	if (m_pDlgMaterial == NULL)
	{
		m_pDlgMaterial = new CDlgSheetMaterial(this);
		m_pDlgMaterial->Create(IDD_DLGSHEETMATERIAL_DIALOG, this);
	}
	m_pDlgMaterial->CenterWindow(this);
	m_pDlgMaterial->ShowWindow(SW_SHOW);
	m_pDlgMaterial->RenewMaterialCode();

	return 0;
}

//자재 정보 Display하기
LRESULT CAutoMainForm::OnDispMaterialCodeInfo(UINT wParam, LPARAM lParam)
{
	if (m_pDlgMaterial != NULL)
		m_pDlgMaterial->RenewMaterialCode();
	return 0;
}

LRESULT CAutoMainForm::OnDispProductPlan(UINT wParam, LPARAM lParam)
{
	if (m_pDlgProductPlan != NULL)
	{
		UINT iPlanCode = (UINT)wParam;
		m_pDlgProductPlan->OnDisplayProductPlan(iPlanCode);
	}
	return 0;
}
//_______________

void CAutoMainForm::OnClickModelDeviceId() 
{
	// TODO: Add your control notification handler code here
	CModelNDeviceIDListDlg Dlg2;
	Dlg2.DoModal();
	
}

void CAutoMainForm::OnClickUnitTact() 
{
	// TODO: Add your control notification handler code here
	CDlgSheetTactTime Dlg;
	Dlg.DoModal();
}



void CAutoMainForm::OnClickJigChangeCarrierUnload() 
{
	// TODO: Add your control notification handler code here
#ifdef DEF_SOURCE_SYSTEM
	if (MyMessageBox(MY_MESSAGE_BOX, 30393, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
#else
	if (MyMessageBox(MY_MESSAGE_BOX, 30397, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
#endif
		return;

	int iResult = 0;

	//전면 
	if (MyMessageBox(_T("Please select a feeder to replace the mold.\r\n금형 교체할 Feeder 를 선택 해 주세요."), _T("confirm"), M_ICONQUESTION|M_YESNO, _T("전면"), _T("후면")) == IDYES)
	{					
		MPos_XYT tgPos[DEF_MAX_TABCARRIER];
		int iPosID;

		iPosID = DEF_TABCARRIER_UNLOAD_POS;

		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{			
			tgPos[i] = MOlbSystemPre.GetTabCarrierComponent(i)->GetTargetXYTPos(iPosID);
		}
		iResult = MOlbSystemPre.MoveAllFrontTabCarrier(tgPos, iPosID);
		if (iResult)
		{
			if (ERR_SKIP_ERROR_DISPLAY == iResult)
				return;
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		
		//	Load 위치로 이동 작업 완료
		MyMessageBox(MY_MESSAGE_BOX, 30394, _T("confirm"), M_ICONINFORMATION);			
	}
	else  //후면 
	{			
		MPos_XYT tgPos[DEF_MAX_TABCARRIER];
		int iPosID;

		iPosID = DEF_TABCARRIER_UNLOAD_POS;

		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{			
			tgPos[i] = MOlbSystemPre.GetRTabCarrierComponent(i)->GetTargetXYTPos(iPosID);
		}
		iResult = MOlbSystemPre.MoveAllRearTabCarrier(tgPos, iPosID);
		if (iResult)
		{
			if (ERR_SKIP_ERROR_DISPLAY == iResult)
				return;
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		
		//	Load 위치로 이동 작업 완료
		MyMessageBox(MY_MESSAGE_BOX, 30394, _T("confirm"), M_ICONINFORMATION);	
	}
}


void CAutoMainForm::OnClickTablossLogF() 
{
	// TODO: Add your control notification handler code here
	CDlgTabICLoss dlg(0);
	dlg.DoModal();
}

void CAutoMainForm::OnClickTablossLogR() 
{
	// TODO: Add your control notification handler code here
	CDlgTabICLoss dlg(1);
	dlg.DoModal();
}

void CAutoMainForm::OnClickOperationGroup2() 
{
	// TODO: Add your control notification handler code here
	m_pDlgFixedVision->ShowWindow(SW_SHOW);
}
