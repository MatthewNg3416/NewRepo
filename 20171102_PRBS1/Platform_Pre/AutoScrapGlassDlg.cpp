// ScrapUnScrapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "AutoScrapGlassDlg.h"
#include "MPlatformOlbSystem.h"
//#include "MRollerConveyor.h"
#include "MPanelTransfer.h"
#include "MPanelAligner.h"
#include "MHandler.h" //170123 SJ_YSH
#include "MTrsAutoManager.h"
#include "MTrsLoadConveyor.h"
#include "MTrsPanelTransfer.h"
#include "MTrsPanelTransferCenter.h"
#include "MTrsPanelTransferOut.h"
#include "MTrsPanelAligner.h"
#include "MTrsUnloadConveyor.h"
#include "MTrsTHandler.h" //170123 SJ_YSH

#include "MNetH.h"
#include "MLCNetData.h"
#include "MyProgressWnd.h"
// selsk_jdy
#include "AutoMainForm.h"
#include "IOpPanel.h"

//20110830 SJ_HJG
#include "DefIOAddrPreBonder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CAutoScrapGlassDlg dialog

CAutoScrapGlassDlg::CAutoScrapGlassDlg(int iScrapModeSelect, CWnd* pParent /*=NULL*/)
	: CDialog(CAutoScrapGlassDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoScrapGlassDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iScrapModeSelect		= iScrapModeSelect;
	m_plnkTrsAutoManager	= MOlbSystemPre.GetTrsAutoManagerComponent();
	m_plnkMelsecIF			= MOlbSystemPre.GetMelsecComponent();
	m_iUnit					= -1;

	// selsk_jdy
	m_plnkAutoMainForm		= (CAutoMainForm*)pParent;
}

void CAutoScrapGlassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoScrapGlassDlg)
	DDX_Control(pDX, IDB_BATCH_ID, m_btnBatchID);
	DDX_Control(pDX, IDB_DEVICE_ID, m_btnDeviceID);
	DDX_Control(pDX, IDB_LOT_ID, m_btnLotID);
	DDX_Control(pDX, IDB_SAVE, m_btnSave);
	DDX_Control(pDX, IDB_UNIT, m_btnUnit);
	DDX_Control(pDX, IDS_OP_TITLE, m_lblOpTitle);
	DDX_Control(pDX, IDS_BATCH_ID, m_lblBatchID);
	DDX_Control(pDX, IDS_DEVICE_ID, m_lblDeviceID);
	DDX_Control(pDX, IDS_LOT_ID, m_lblLotID);
	DDX_Control(pDX, IDS_UNIT, m_lblUnit);
	DDX_Control(pDX, IDB_LOAD_CONVEYOR, m_btnLoadConveyor);
	DDX_Control(pDX, IDB_PANEL_TRANSFER, m_btnPanelTransfer);
	DDX_Control(pDX, IDB_PANEL_ALIGNER, m_btnPanelAligner);
	DDX_Control(pDX, IDB_UNLOAD_CONVEYOR, m_btnUnloadConveyor);
	DDX_Control(pDX, IDB_PRODUCT_DEVICE_ID, m_btnProductDeviceID);
	DDX_Control(pDX, IDB_PRODUCT_BATCH_ID, m_btnProductBatchID);
	DDX_Control(pDX, IDB_PANEL_PASS_WORD_ASSIGN, m_btnPanelPassWordAssign);
	DDX_Control(pDX, IDB_TURN_HANDLER, m_btnTurnHandler); //170123 SJ_YSH 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoScrapGlassDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoScrapGlassDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoScrapGlassDlg message handlers

BEGIN_EVENTSINK_MAP(CAutoScrapGlassDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoScrapGlassDlg)
	ON_EVENT(CAutoScrapGlassDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_LOT_ID, -600 /* Click */, OnClickLotId, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_DEVICE_ID, -600 /* Click */, OnClickDeviceId, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_BATCH_ID, -600 /* Click */, OnClickBatchId, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_LOAD_CONVEYOR, -600 /* Click */, OnClickLoadConveyor, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_PANEL_TRANSFER, -600 /* Click */, OnClickPanelTransfer, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_PANEL_ALIGNER, -600 /* Click */, OnClickPanelAligner, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_UNLOAD_CONVEYOR, -600 /* Click */, OnClickUnloadConveyor, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_PRODUCT_DEVICE_ID, -600 /* Click */, OnClickProductDeviceId, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_PRODUCT_BATCH_ID, -600 /* Click */, OnClickProductBatchId, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_PANEL_PASS_WORD_ASSIGN, -600 /* Click */, OnClickPanelPassWordAssign, VTS_NONE)
	ON_EVENT(CAutoScrapGlassDlg, IDB_TURN_HANDLER, -600 /* Click */, OnClickTurnHandler, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CAutoScrapGlassDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_iScrapModeSelect == DEF_SCRAP_GLASS)
	{
		SetWindowText(_T("Scrap Glass Operation"));		
		m_lblOpTitle.SetCaption(_T("Scrap"));
	}
	else if (m_iScrapModeSelect == DEF_UNSCRAP_GLASS)
	{
		SetWindowText(_T("Unscrap Glass Operation"));
		m_lblOpTitle.SetCaption(_T("Unscrap"));
	}

	enablePanelDataButton(FALSE);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛


	CString szTextTemp;
	ReadDisplayMessage(MAIN_AUTO, _T("AutoScrapGlassDlg_1"), &szTextTemp);
//Panel 정보
	GetDlgItem(IDS_TITLE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoScrapGlassDlg_2"), &szTextTemp);
//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoScrapGlassDlg_3"), &szTextTemp);
//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	

	//m_btnLoadConveyor.EnableWindow(FALSE);	//@ //170123 SJ_YSH Del.

#ifdef DEF_GATE_SYSTEM
	m_btnPanelTransfer.EnableWindow(FALSE);
#endif
#ifndef DEF_USE_TRANSFER_CENTER
	m_btnLoadConveyor.EnableWindow(FALSE);
#endif
#ifndef DEF_USE_TURN_HANDLER
	m_btnTurnHandler.EnableWindow(FALSE);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoScrapGlassDlg::enablePanelDataButton(BOOL bState)
{
	if (bState == TRUE)
	{
//		MGlassDatas pGlassData;
//		m_plnkMelsecIF->GetGlassData((MToLowerEqs*)(&pGlassData));
	
//		m_strBatchID = (pGlassData.m_strBatchID.GetLength() > 1) ? pGlassData.m_strBatchID : "-";
//		m_strDeviceID = (pGlassData.m_strDeviceID.GetLength() > 1) ? pGlassData.m_strDeviceID : "-";
//		m_lblBatchID.SetCaption(m_strBatchID);
//		m_lblDeviceID.SetCaption(m_strDeviceID);
		m_lblLotID.SetCaption(_T("-"));
	}
	m_btnUnit.EnableWindow(bState);
	m_btnLotID.EnableWindow(bState);
	m_btnDeviceID.EnableWindow(bState);
	m_btnBatchID.EnableWindow(bState);
	m_btnProductDeviceID.EnableWindow(bState);
	m_btnProductBatchID.EnableWindow(bState);
	m_btnPanelPassWordAssign.EnableWindow(bState);
	
	m_btnSave.EnableWindow(bState);
}

void CAutoScrapGlassDlg::OnExit() 
{
	CDialog::OnCancel();
}

void CAutoScrapGlassDlg::OnClickSave() 
{
//입력된 LOT ID, Device ID, Batch ID를 적용 하시겠습니까?
	int iResult = MyMessageBox(MY_MESSAGE_BOX, 20400, _T("Confirm"), M_ICONQUESTION|M_YESNO);
	if (iResult == IDNO)
		return;

	MGlassDatas pGlassData;
	pGlassData.ResetData();//2009.04.06 SESL_CYJ
	if(m_iScrapModeSelect == 2)
		m_plnkMelsecIF->GetGlassData((MToLowerEqs*)(&pGlassData)); 

	pGlassData.m_strEPanelID = m_strLotID;
	pGlassData.m_strHPanelID = m_strLotID;

	if (m_strDeviceID.GetLength() > 1)
		pGlassData.m_strDeviceID = m_strDeviceID;
	
	if (m_strBatchID.GetLength() > 1)
		pGlassData.m_strBatchID = m_strBatchID;

	pGlassData.m_strJudgement = "OK";
	pGlassData.m_strInsFlag = "ON";
	pGlassData.m_strProdKind = "US";

	// Glass ID 입력 & 해당 Unit Initialize
	m_plnkTrsAutoManager->m_pGlassData[m_iUnit]->CopyData(pGlassData);
	m_plnkTrsAutoManager->ManageGlassData(ePanelUnScrap, m_iUnit);

	BOOL bIsPanel = FALSE;
	switch (m_iUnit)
	{
		/*
	case DEF_LOAD_CONVEYOR_PANEL:
		bIsPanel = MOlbSystemPre.GetRollerConveyorComponent(DEF_LOAD_CONVEYOR)->IsPanelDetected();
		break;
	case DEF_PANEL_TRANSFER_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelTransferComponent()->IsPanelAbsorbed();
		break;
	case DEF_PANEL_ALIGNER_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed();
		break;
	case DEF_UNLOAD_CONVEYOR_PANEL:
		bIsPanel = MOlbSystemPre.GetRollerConveyorComponent(DEF_UNLOAD_CONVEYOR)->IsPanelDetected();
		break;
		*/
		//170123 SJ_YSH Add.
#ifdef DEF_USE_TRANSFER_CENTER
	case DEF_PANEL_TRANSFER_CENTER_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelTransferCenterComponent()->IsPanelAbsorbed();
		break;
#endif
#ifdef DEF_USE_TURN_HANDLER
	case DEF_THANDLER_PANEL:
		bIsPanel = MOlbSystemPre.GetTurnHandlerComponent()->IsAbsorbPanel();
		break;
#endif
		//__________________
#ifdef DEF_SOURCE_SYSTEM
	case DEF_PANEL_TRANSFER_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelTransferComponent()->IsPanelAbsorbed();
		break;
#endif
	case DEF_PANEL_ALIGNER_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed();
		break;
	case DEF_PANEL_TRANSFER_OUT_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelTransferOutComponent()->IsPanelAbsorbed();
		break;
	default:
		ASSERT(FALSE);
		return;
	}

	if (FALSE == bIsPanel)
	{
//	Panel 정보 저장이 완료되었습니다.\r\n계속해서 자동운전 초기화를 진행합니다.\r\nPanel 투입을 확인하고 계속 진행하세요.
		MyMessageBox(MY_MESSAGE_BOX, 20401, _T("Panel Information save"));
	}
	
	switch (m_iUnit)
	{
		/*
	case DEF_LOAD_CONVEYOR_PANEL:
		iResult = MOlbSystemPre.GetTrsLoadConveyorComponent()->Initialize();
		break;
	case DEF_PANEL_TRANSFER_PANEL:
		iResult = MOlbSystemPre.GetTrsPanelTransferComponent()->Initialize();
		break;
	case DEF_PANEL_ALIGNER_PANEL:
		iResult = MOlbSystemPre.GetTrsPanelAlignerComponent()->Initialize(FALSE);
		break;
	case DEF_UNLOAD_CONVEYOR_PANEL:
		iResult = MOlbSystemPre.GetTrsUnloadConveyorComponent()->Initialize();
		break;
		*/
//@	case DEF_PANEL_TRANSFER_CENTER_PANEL:
//@		bIsPanel = MOlbSystemPre.GetPanelTransferCenterComponent()->Initialize();
//@		break;
#ifdef DEF_SOURCE_SYSTEM
	case DEF_PANEL_TRANSFER_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelTransferComponent()->Initialize();
		break;
#endif
	case DEF_PANEL_ALIGNER_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelAlignerComponent()->Initialize();
		break;
	case DEF_PANEL_TRANSFER_OUT_PANEL:
		bIsPanel = MOlbSystemPre.GetPanelTransferOutComponent()->Initialize();
		break;
	default:
		return;
	}
	if (bIsPanel)
	{
// jdy sesl		MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(bIsPanel);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
//Panel Unscrap 작업이 완료되었습니다.
	MyMessageBox(MY_MESSAGE_BOX, 20402, _T("Information"));	

	m_iUnit = -1;
	enablePanelDataButton(FALSE);
}

void CAutoScrapGlassDlg::OnClickLotId() 
{
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	// jdy sesl    memset(szTemp,0,sizeof(char)*100);
	memset(szTemp, 0, sizeof(szTemp));

	swprintf(szBuf, _T(""));
	GetValueBox(szTemp, 20, szBuf, _T("Input Lot ID"));

	m_strLotID.Format(_T("%s"), szTemp);
	m_lblLotID.SetCaption(szTemp);
}

void CAutoScrapGlassDlg::OnClickDeviceId() 
{
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	// jdy sesl    memset(szTemp,0,sizeof(char)*100);
	memset(szTemp, 0, sizeof(szTemp));

	swprintf(szBuf, _T(""));
	GetValueBox(szTemp, 20, szBuf, _T("Input Device ID"));

	m_strDeviceID.Format(_T("%s"), szTemp);
	m_lblDeviceID.SetCaption(szTemp);
}

void CAutoScrapGlassDlg::OnClickBatchId() 
{
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	// jdy sesl    memset(szTemp,0,sizeof(char)*100);
	memset(szTemp, 0, sizeof(szTemp));

	swprintf(szBuf, _T(""));
	GetValueBox(szTemp, 20, szBuf, _T("Input Batch ID"));

	m_strBatchID.Format(_T("%s"), szTemp);
	m_lblBatchID.SetCaption(szTemp);	
}

void CAutoScrapGlassDlg::OnClickLoadConveyor() 
{
#ifdef DEF_USE_TRANSFER_CENTER
//	m_iUnit = DEF_LOAD_CONVEYOR_PANEL;
	m_iUnit = DEF_PANEL_TRANSFER_CENTER_PANEL;
	if (m_iScrapModeSelect == DEF_SCRAP_GLASS)
	{
		// 이전 설비 Step Stop 으로 전환 요청
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(TRUE);

		// 현재 Unit 정보 Display
		//m_lblUnit.SetCaption(_T("Load Conveyor"));
		m_lblUnit.SetCaption(_T("Transfer Center"));

		// 현재 Panel 정보 Display
		MGlassDatas* pGlassData = m_plnkTrsAutoManager->m_pGlassData[m_iUnit];
		m_lblLotID.SetCaption(pGlassData->m_strEPanelID);

		// 패널 정보 제거 확인 메세지 출력
		//	PanelTransfer Center 에 있는 Panel 에 대한 정보를 삭제하시겠습니까?
		int iResult = MyMessageBox(MY_MESSAGE_BOX, 20403, _T("Question"), M_ICONQUESTION|M_YESNO);
		if (iResult == IDNO)
		{
			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);
			m_lblLotID.SetCaption(_T("-"));
			return;
		}

		/*/
		//20110830 SJ_HJG
		MOlbSystemPre.GetIOComponent()->OutputOff(OUT_DOOR_LOCK_OPENCLOSE);
		MOlbSystemPre.GetIOComponent()->OutputOff(OUT_FRONT_MATERIALCHANGE_DOOR_LOCK);
		MOlbSystemPre.GetIOComponent()->OutputOff(OUT_REAR_MATERIALCHANGE_DOOR_LOCK);
		//________
		/*/

		m_lblUnit.SetCaption(_T("-"));

		//iResult = MOlbSystemPre.GetTrsLoadConveyorComponent()->ScrapPanel();
		iResult = MOlbSystemPre.GetTrsPanelTransferCenterComponent()->ScrapPanel();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);
			return;
		}

		// 이전 설비 Step Stop 으로 전환 요청 해제.
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);

//	Panel Scrap 완료.
		MyMessageBox(MY_MESSAGE_BOX, 20404, _T("Information"));
	}
	else if (m_iScrapModeSelect == DEF_UNSCRAP_GLASS)
	{
//		if (FALSE == MOlbSystemPre.GetRollerConveyorComponent(DEF_LOAD_CONVEYOR)->IsPanelDetected())
		if (FALSE == MOlbSystemPre.GetPanelTransferCenterComponent()->IsPanelAbsorbed())
		{
//	Load Conveyor에 Panel이 감지 되지 않습니다.\r\nUnscrap 작업을 계속 하시겠습니까?
			if (MyMessageBox(MY_MESSAGE_BOX, 20405, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
				return;
		}
		
		//m_lblUnit.SetCaption(_T("Load Conveyor"));
		m_lblUnit.SetCaption(_T("Transfer Center"));
		enablePanelDataButton(TRUE);
	}
#endif
}

void CAutoScrapGlassDlg::OnClickPanelTransfer()
{
//	m_iUnit = DEF_PANEL_TRANSFER_PANEL;
	m_iUnit = DEF_PANEL_TRANSFER_PANEL;
	CString szTextTemp;
	if (m_iScrapModeSelect == DEF_SCRAP_GLASS)
	{
		// 이전 설비 Step Stop 으로 전환 요청
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(TRUE);

		// 현재 Unit 정보 Display
		m_lblUnit.SetCaption(_T("Panel Transfer"));

		// 현재 Panel 정보 Display
		MGlassDatas* pGlassData = m_plnkTrsAutoManager->m_pGlassData[m_iUnit];
		m_lblLotID.SetCaption(pGlassData->m_strEPanelID);

		// 패널 정보 제거 확인 메세지 출력
//	Panel Transfer 에 있는 Panel 에 대한 정보를 삭제하시겠습니까?
		int iResult = MyMessageBox(MY_MESSAGE_BOX, 20406, _T("Question : Delete"), M_ICONQUESTION|M_YESNO);
		if (iResult == IDNO)
		{
			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);
			m_lblLotID.SetCaption(_T("-"));
			return;
		}

		//20110830 SJ_HJG
//@		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN);
//@		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN);
		//________

		m_lblUnit.SetCaption(_T("-"));

// selsk_jdy
//@#ifdef DEF_GATE_SYSTEM
		iResult = SUCCESS;
		::SendMessage(m_plnkAutoMainForm->m_hWnd, WM_MOVE_TAB_MOUNTER_REQ, FALSE, (LPARAM)&iResult);
		if (iResult)
		{
			if (ERR_SKIP_ERROR_DISPLAY != iResult)
			{
// jdy sesl				MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			}

			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
			return;
		}
//@#endif

		iResult = MOlbSystemPre.GetTrsPanelTransferComponent()->ScrapPanel();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);
			return;
		}

		// selsk_jdy
//@#ifdef DEF_GATE_SYSTEM
//SESL_LKH_090711 : 변수명 통일
//		if (TRUE == MOlbSystemPre.GetSystemData()->m_bSafetySensor
//			&& TRUE == MOlbSystemPre.GetOpPanelComponent()->GetSafeDoorStatus())
		if (TRUE == m_plnkTrsAutoManager->IsDoorOpen())
		{
			int iCount = 0;

//		Door Open 상태입니다. 닫아주세요.
			ReadDisplayMessage(MAIN_AUTO, _T("AutoScrapGlassDlg_4"), &szTextTemp);

			do
			{
				MyMessageBox(szTextTemp, _T("Error"), M_ICONERROR);
				Sleep(100);

				if (iCount > 10)
				{
//				Door Sensor 이상.
					MyMessageBox(MY_MESSAGE_BOX, 20407, _T("Error"), M_ICONERROR);

					m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
					return;
				}

				iCount++;

			//} while (TRUE == MOlbSystemPre.GetOpPanelComponent()->GetSafeDoorStatus());
			} while (TRUE == m_plnkTrsAutoManager->IsDoorOpen());
		}

		m_plnkAutoMainForm->UnitInitialize(FALSE);
//@#endif

		// 이전 설비 Step Stop 으로 전환 요청 해제.
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);

//	Panel Scrap 완료.
		MyMessageBox(MY_MESSAGE_BOX, 20408, _T("Information"));
	}
	else if (m_iScrapModeSelect == DEF_UNSCRAP_GLASS)
	{
		if (FALSE == MOlbSystemPre.GetPanelTransferComponent()->IsPanelAbsorbed())
		{
//		Panel Transfer 에 Panel 이 흡착되어 있지 않습니다.\r\nUnscrap 작업을 계속 하시겠습니까?
			if (MyMessageBox(MY_MESSAGE_BOX, 20409, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
				return;
		}
		
		m_lblUnit.SetCaption(_T("Panel Transfer"));
		
		enablePanelDataButton(TRUE);
	}
}

void CAutoScrapGlassDlg::OnClickPanelAligner() 
{
	//m_iUnit = DEF_PANEL_ALIGNER_PANEL;
	m_iUnit = DEF_PANEL_ALIGNER_PANEL;
	if (m_iScrapModeSelect == DEF_SCRAP_GLASS)
	{
		// 다음 설비 Step Stop 으로 전환 요청
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(TRUE);

		// 현재 Unit 정보 Display
		m_lblUnit.SetCaption(_T("Panel Aligner"));

		// 현재 Panel 정보 Display
		MGlassDatas* pGlassData = m_plnkTrsAutoManager->m_pGlassData[m_iUnit];
		m_lblLotID.SetCaption(pGlassData->m_strEPanelID);

		// 패널 정보 제거 확인 메세지 출력
//	Panel Aligner 에 있는 Panel 에 대한 정보를 삭제하시겠습니까?
		int iResult = MyMessageBox(MY_MESSAGE_BOX, 20410, _T("Question"), M_ICONQUESTION|M_YESNO);
		if (iResult == IDNO)
		{
			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
			m_lblLotID.SetCaption(_T("-"));
			return;
		}

		//20110830 SJ_HJG
//@		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_FRONT_DOOR_LOCK_OPEN);
//@		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN);
		//________

		m_lblUnit.SetCaption(_T("-"));

		// selsk_jdy
//@#ifdef DEF_GATE_SYSTEM
		iResult = SUCCESS;
		::SendMessage(m_plnkAutoMainForm->m_hWnd, WM_MOVE_TAB_MOUNTER_REQ, FALSE, (LPARAM)&iResult);
		if (iResult)
		{
			if (ERR_SKIP_ERROR_DISPLAY != iResult)
			{
// jdy sesl				MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			}

			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
			return;
		}
//@#endif

		iResult = MOlbSystemPre.GetTrsPanelAlignerComponent()->ScrapPanel();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
			return;
		}

		// selsk_jdy
//@#ifdef DEF_GATE_SYSTEM
//SESL_LKH_090711 : 변수명 통일
//		if (TRUE == MOlbSystemPre.GetSystemData()->m_bSafetySensor
//			&& TRUE == MOlbSystemPre.GetOpPanelComponent()->GetSafeDoorStatus())
		if (TRUE == m_plnkTrsAutoManager->IsDoorOpen())
		{
			int iCount = 0;
//		Door Open 상태입니다. 닫아주세요.
			CString szTextTemp;
			ReadDisplayMessage(MAIN_AUTO, _T("AutoScrapGlassDlg_5"), &szTextTemp);

			do
			{
				MyMessageBox(szTextTemp, _T("Error"), M_ICONERROR);
				Sleep(100);

				if (iCount > 10)
				{
//				Door Sensor 이상.
					MyMessageBox(MY_MESSAGE_BOX, 20411, _T("Error"), M_ICONERROR);

					m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
					return;
				}

				iCount++;

			//} while (TRUE == MOlbSystemPre.GetOpPanelComponent()->GetSafeDoorStatus());
			} while (TRUE == m_plnkTrsAutoManager->IsDoorOpen());
		}

		m_plnkAutoMainForm->UnitInitialize(FALSE);
//@#endif

		// 다음 설비 Step Stop 으로 전환 요청 해제.
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);

//	Panel Scrap 완료.
		MyMessageBox(MY_MESSAGE_BOX, 20412, _T("Information"));
	}
	else if (m_iScrapModeSelect == DEF_UNSCRAP_GLASS)
	{
		if (FALSE == MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed())
		{
//		Panel Aligner 에 Panel 이 흡착되어 있지 않습니다.\r\nUnscrap 작업을 계속 하시겠습니까?
			if (MyMessageBox(MY_MESSAGE_BOX, 20413, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
				return;
		}
		
		m_lblUnit.SetCaption(_T("Panel Aligner"));
		
		enablePanelDataButton(TRUE);
	}
}

void CAutoScrapGlassDlg::OnClickUnloadConveyor() 
{
	//m_iUnit = DEF_UNLOAD_CONVEYOR_PANEL;
	m_iUnit = DEF_PANEL_TRANSFER_OUT_PANEL;
	if (m_iScrapModeSelect == DEF_SCRAP_GLASS)
	{
		// 다음 설비 Step Stop 으로 전환 요청
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(TRUE);

		// 현재 Unit 정보 Display
		//m_lblUnit.SetCaption(_T("Unload Conveyor"));
		m_lblUnit.SetCaption(_T("Transfer Out"));

		// 현재 Panel 정보 Display
		MGlassDatas* pGlassData = m_plnkTrsAutoManager->m_pGlassData[m_iUnit];
		m_lblLotID.SetCaption(pGlassData->m_strEPanelID);

		// 패널 정보 제거 확인 메세지 출력
//	Unload Conveyor 에 있는 Panel 에 대한 정보를 삭제하시겠습니까?
		int iResult = MyMessageBox(MY_MESSAGE_BOX, 20414, _T("Question : Delete"), M_ICONQUESTION|M_YESNO);
		if (iResult == IDNO)
		{
			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
			m_lblLotID.SetCaption(_T("-"));
			return;
		}

		//20110830 SJ_HJG
//@		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_FRONT_DOOR_LOCK_OPEN);
//@		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN);
		//________

		m_lblUnit.SetCaption(_T("-"));

		//iResult = MOlbSystemPre.GetTrsUnloadConveyorComponent()->ScrapPanel();
		iResult = MOlbSystemPre.GetTrsPanelTransferOutComponent()->ScrapPanel();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
			return;
		}

		// 다음 설비 Step Stop 으로 전환 요청 해제.
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);

//	Panel Scrap 완료.
		MyMessageBox(MY_MESSAGE_BOX, 20415, _T("Information"));
	}
	else if (m_iScrapModeSelect == DEF_UNSCRAP_GLASS)
	{
		//if (FALSE == MOlbSystemPre.GetRollerConveyorComponent(DEF_UNLOAD_CONVEYOR)->IsPanelDetected())
		if (FALSE == MOlbSystemPre.GetPanelTransferOutComponent()->IsPanelAbsorbed())
		{
//		Unload Conveyor 에 Panel 이 감지되지 않습니다.\r\nUnscrap 작업을 계속 하시겠습니까?
			if (MyMessageBox(MY_MESSAGE_BOX, 20416, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
				return;
		}
		
		//m_lblUnit.SetCaption(_T("Unload Conveyor"));
		m_lblUnit.SetCaption(_T("Transfer Out"));
		
		enablePanelDataButton(TRUE);
	}
}

void CAutoScrapGlassDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CAutoScrapGlassDlg::OnClickProductDeviceId() 
{
	// TODO: Add your control notification handler code here
	MGlassDatas pGlassData;
//	m_plnkMelsecIF->GetGlassData((MToLowerEqs*)(&pGlassData));
//	m_strDeviceID = (pGlassData.m_strDeviceID.GetLength() > 1) ? pGlassData.m_strDeviceID : "-";
	m_strDeviceID = MOlbSystemPre.GetLCNetComponent()->m_CurProductPlan.m_sDeviceID;
	m_lblDeviceID.SetCaption(m_strDeviceID);
}

void CAutoScrapGlassDlg::OnClickProductBatchId() 
{
	// TODO: Add your control notification handler code here
	MGlassDatas pGlassData;
//	m_plnkMelsecIF->GetGlassData((MToLowerEqs*)(&pGlassData));
//	m_strBatchID = (pGlassData.m_strBatchID.GetLength() > 1) ? pGlassData.m_strBatchID : "-";
	m_strBatchID = MOlbSystemPre.GetLCNetComponent()->m_CurProductPlan.m_sBatchID;
	m_lblBatchID.SetCaption(m_strBatchID);
}

void CAutoScrapGlassDlg::OnClickPanelPassWordAssign() 
{
	// TODO: Add your control notification handler code here
	//현재 선택 된 Unit의 Comment Word 에 "OS"를 입력 한다.
	if (MyMessageBox(_T("Do you want to assign OLB SKIP to Melsec Comment Word Data of Panel of the currently specified Unit?\r\n현재 지정한 Unit 의 Panel의 Melsec Comment Word Data 에 OLB SKIP을 Assign 하시겠습니까?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkTrsAutoManager->AssignPassData(m_iUnit);

	CDialog::OnCancel();
}

//170123 SJ_YSH
void CAutoScrapGlassDlg::OnClickTurnHandler() 
{

#ifdef DEF_USE_TURN_HANDLER
	m_iUnit = DEF_THANDLER_PANEL;
	CString szTextTemp;
	if (m_iScrapModeSelect == DEF_SCRAP_GLASS)
	{
		// 이전 설비 Step Stop 으로 전환 요청
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(TRUE);

		// 현재 Unit 정보 Display
		m_lblUnit.SetCaption(_T("Turn Handler"));

		// 현재 Panel 정보 Display
		MGlassDatas* pGlassData = m_plnkTrsAutoManager->m_pGlassData[m_iUnit];
		m_lblLotID.SetCaption(pGlassData->m_strEPanelID);

		// 패널 정보 제거 확인 메세지 출력
//	Panel Transfer 에 있는 Panel 에 대한 정보를 삭제하시겠습니까?
		int iResult = MyMessageBox(MY_MESSAGE_BOX, 20417, _T("Question : Delete"), M_ICONQUESTION|M_YESNO);
		if (iResult == IDNO)
		{
			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);
			m_lblLotID.SetCaption(_T("-"));
			return;
		}

		//20110830 SJ_HJG
//@		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN);
//@		MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN);
		//________

		m_lblUnit.SetCaption(_T("-"));

// selsk_jdy
//@#ifdef DEF_GATE_SYSTEM
		iResult = SUCCESS;
		/*/
		::SendMessage(m_plnkAutoMainForm->m_hWnd, WM_MOVE_TAB_MOUNTER_REQ, FALSE, (LPARAM)&iResult);
		if (iResult)
		{
			if (ERR_SKIP_ERROR_DISPLAY != iResult)
			{
// jdy sesl				MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			}

			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
			return;
		}
		/*/
//@#endif


		iResult = MOlbSystemPre.GetTrsTHandlerComponent()->ScrapPanel();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Initial Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

			m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);
			return;
		}

		// selsk_jdy
//@#ifdef DEF_GATE_SYSTEM
//SESL_LKH_090711 : 변수명 통일
//		if (TRUE == MOlbSystemPre.GetSystemData()->m_bSafetySensor
//			&& TRUE == MOlbSystemPre.GetOpPanelComponent()->GetSafeDoorStatus())
		if (TRUE == m_plnkTrsAutoManager->IsDoorOpen())
		{
			int iCount = 0;

//		Door Open 상태입니다. 닫아주세요.
			ReadDisplayMessage(MAIN_AUTO, _T("AutoScrapGlassDlg_4"), &szTextTemp);

			do
			{
				MyMessageBox(szTextTemp, _T("Error"), M_ICONERROR);
				Sleep(100);

				if (iCount > 10)
				{
//				Door Sensor 이상.
					MyMessageBox(MY_MESSAGE_BOX, 20407, _T("Error"), M_ICONERROR);

					m_plnkMelsecIF->SetImmediatelyPauseRequest_ToLowerEqBit(FALSE);
					return;
				}

				iCount++;

			//} while (TRUE == MOlbSystemPre.GetOpPanelComponent()->GetSafeDoorStatus());
			} while (TRUE == m_plnkTrsAutoManager->IsDoorOpen());
		}

		m_plnkAutoMainForm->UnitInitialize(FALSE);
//@#endif

		// 이전 설비 Step Stop 으로 전환 요청 해제.
		m_plnkMelsecIF->SetImmediatelyPauseRequest_ToUpperEqBit(FALSE);

//	Panel Scrap 완료.
		MyMessageBox(MY_MESSAGE_BOX, 20408, _T("Information"));
	}
	else if (m_iScrapModeSelect == DEF_UNSCRAP_GLASS)
	{
		if (FALSE == MOlbSystemPre.GetTurnHandlerComponent()->IsAbsorbPanel())
		{
//		Panel Transfer 에 Panel 이 흡착되어 있지 않습니다.\r\nUnscrap 작업을 계속 하시겠습니까?
			if (MyMessageBox(MY_MESSAGE_BOX, 20409, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
				return;
		}
		
		m_lblUnit.SetCaption(_T("Turn Handler"));
		
		enablePanelDataButton(TRUE);
	}
#endif
}
//_____________