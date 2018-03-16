// StepDispDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "AutoStepDispDlg.h"
#include "MPlatformOlbSystem.h"
#include "MPanelData.h"
#include "MTabData.h"
#include "MProcessData.h"
#include "MCtrlPanelAligner.h"
#include "MTrsPanelAligner.h"
#include "MTrsPanelTransfer.h"
#include "MTrsPanelTransferCenter.h"
#include "MTrsPanelTransferOut.h"
//#include "MTrsPanelTransferCentor.h"
//#include "MTrsUnloadConveyor.h"
#include "MTrsTabCarrier.h"
#include "MTrsTabFeeder.h"	
#include "MTrsTabMounter.h"
#include "MTrsInspectionCamera.h"
#include "MTrsAutoManager.h"
#include "MTrsTHandler.h" //SJ_YYK 161104 Add...

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

/////////////////////////////////////////////////////////////////////////////
// CAutoStepDispDlg dialog

extern	MPlatformOlbSystem	MOlbSystemPre;

CAutoStepDispDlg::CAutoStepDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoStepDispDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoStepDispDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iSelColor = 0;
	m_uiTimerID = 0;
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	for (int i = 0; i < DEF_MAX_WORKSIDE_PER_EQUIP_UNIT; i++)
	{
		m_rgeWorkSide[i] = WORKING_SIDE_UNKNOWN;
		m_rgiMaxTabQtyPerWorkSide[i] = 0;
	}
}


void CAutoStepDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoStepDispDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PROCESS_NAME0,		m_rglblProcessName[0]);
	DDX_Control(pDX, IDC_PROCESS_NAME1,		m_rglblProcessName[1]);
	DDX_Control(pDX, IDC_PROCESS_NAME2,		m_rglblProcessName[2]);
	DDX_Control(pDX, IDC_PROCESS_NAME3,		m_rglblProcessName[3]);
	DDX_Control(pDX, IDC_PROCESS_NAME4,		m_rglblProcessName[4]);
	DDX_Control(pDX, IDC_PROCESS_NAME5,		m_rglblProcessName[5]);
	DDX_Control(pDX, IDC_PROCESS_NAME6,		m_rglblProcessName[6]);
	DDX_Control(pDX, IDC_PROCESS_NAME7,		m_rglblProcessName[7]);
	DDX_Control(pDX, IDC_PROCESS_NAME8,		m_rglblProcessName[8]);
	DDX_Control(pDX, IDC_PROCESS_NAME9,		m_rglblProcessName[9]);
	DDX_Control(pDX, IDC_PROCESS_NAME10,	m_rglblProcessName[10]);
	DDX_Control(pDX, IDC_PROCESS_NAME11,	m_rglblProcessName[11]);
	DDX_Control(pDX, IDC_PROCESS_NAME12,	m_rglblProcessName[12]);
	DDX_Control(pDX, IDC_PROCESS_NAME13,	m_rglblProcessName[13]);
	DDX_Control(pDX, IDC_PROCESS_NAME14,	m_rglblProcessName[14]);
	DDX_Control(pDX, IDC_PROCESS_NAME15,	m_rglblProcessName[15]);
	DDX_Control(pDX, IDC_PROCESS_NAME16,	m_rglblProcessName[16]);
	DDX_Control(pDX, IDC_PROCESS_NAME17,	m_rglblProcessName[17]);
	DDX_Control(pDX, IDC_PROCESS_NAME18,	m_rglblProcessName[18]);
	DDX_Control(pDX, IDC_PROCESS_NAME19,	m_rglblProcessName[19]);
	DDX_Control(pDX, IDC_PROCESS_NAME20,	m_rglblProcessName[20]);
	DDX_Control(pDX, IDC_PROCESS_NAME21,	m_rglblProcessName[21]);
	DDX_Control(pDX, IDC_PROCESS_NAME22,	m_rglblProcessName[22]);
	DDX_Control(pDX, IDC_PROCESS_NAME23,	m_rglblProcessName[23]);
	DDX_Control(pDX, IDC_PROCESS_NAME24,	m_rglblProcessName[24]);
	DDX_Control(pDX, IDC_PROCESS_NAME25,	m_rglblProcessName[25]);
	DDX_Control(pDX, IDC_PROCESS_NAME26,	m_rglblProcessName[26]);
	DDX_Control(pDX, IDC_PROCESS_NAME27,	m_rglblProcessName[27]);
	DDX_Control(pDX, IDC_PROCESS_NAME28,	m_rglblProcessName[28]);
	DDX_Control(pDX, IDC_PROCESS_NAME29,	m_rglblProcessName[29]);
	DDX_Control(pDX, IDC_PROCESS_NAME30,	m_rglblProcessName[30]);
	DDX_Control(pDX, IDC_PROCESS_NAME31,	m_rglblProcessName[31]);
	
	DDX_Control(pDX, IDC_PROCESS_STEP0,		m_rglblProcessStep[0]);
	DDX_Control(pDX, IDC_PROCESS_STEP1,		m_rglblProcessStep[1]);
	DDX_Control(pDX, IDC_PROCESS_STEP2,		m_rglblProcessStep[2]);
	DDX_Control(pDX, IDC_PROCESS_STEP3,		m_rglblProcessStep[3]);
	DDX_Control(pDX, IDC_PROCESS_STEP4,		m_rglblProcessStep[4]);
	DDX_Control(pDX, IDC_PROCESS_STEP5,		m_rglblProcessStep[5]);
	DDX_Control(pDX, IDC_PROCESS_STEP6,		m_rglblProcessStep[6]);
	DDX_Control(pDX, IDC_PROCESS_STEP7,		m_rglblProcessStep[7]);
	DDX_Control(pDX, IDC_PROCESS_STEP8,		m_rglblProcessStep[8]);
	DDX_Control(pDX, IDC_PROCESS_STEP9,		m_rglblProcessStep[9]);
	DDX_Control(pDX, IDC_PROCESS_STEP10,	m_rglblProcessStep[10]);
	DDX_Control(pDX, IDC_PROCESS_STEP11,	m_rglblProcessStep[11]);
	DDX_Control(pDX, IDC_PROCESS_STEP12,	m_rglblProcessStep[12]);
	DDX_Control(pDX, IDC_PROCESS_STEP13,	m_rglblProcessStep[13]);
	DDX_Control(pDX, IDC_PROCESS_STEP14,	m_rglblProcessStep[14]);
	DDX_Control(pDX, IDC_PROCESS_STEP15,	m_rglblProcessStep[15]);
	DDX_Control(pDX, IDC_PROCESS_STEP16,	m_rglblProcessStep[16]);
	DDX_Control(pDX, IDC_PROCESS_STEP17,	m_rglblProcessStep[17]);
	DDX_Control(pDX, IDC_PROCESS_STEP18,	m_rglblProcessStep[18]);
	DDX_Control(pDX, IDC_PROCESS_STEP19,	m_rglblProcessStep[19]);
	DDX_Control(pDX, IDC_PROCESS_STEP20,	m_rglblProcessStep[20]);
	DDX_Control(pDX, IDC_PROCESS_STEP21,	m_rglblProcessStep[21]);
	DDX_Control(pDX, IDC_PROCESS_STEP22,	m_rglblProcessStep[22]);
	DDX_Control(pDX, IDC_PROCESS_STEP23,	m_rglblProcessStep[23]);
	DDX_Control(pDX, IDC_PROCESS_STEP24,	m_rglblProcessStep[24]);
	DDX_Control(pDX, IDC_PROCESS_STEP25,	m_rglblProcessStep[25]);
	DDX_Control(pDX, IDC_PROCESS_STEP26,	m_rglblProcessStep[26]);
	DDX_Control(pDX, IDC_PROCESS_STEP27,	m_rglblProcessStep[27]);
	DDX_Control(pDX, IDC_PROCESS_STEP28,	m_rglblProcessStep[28]);
	DDX_Control(pDX, IDC_PROCESS_STEP29,	m_rglblProcessStep[29]);
	DDX_Control(pDX, IDC_PROCESS_STEP30,	m_rglblProcessStep[30]);
	DDX_Control(pDX, IDC_PROCESS_STEP31,	m_rglblProcessStep[31]);

	DDX_Control(pDX, IDC_PROCESS_ITI0,		m_rglblProcessITI[0]);
	DDX_Control(pDX, IDC_PROCESS_ITI1,		m_rglblProcessITI[1]);
	DDX_Control(pDX, IDC_PROCESS_ITI2,		m_rglblProcessITI[2]);
	DDX_Control(pDX, IDC_PROCESS_ITI3,		m_rglblProcessITI[3]);
	DDX_Control(pDX, IDC_PROCESS_ITI4,		m_rglblProcessITI[4]);
	DDX_Control(pDX, IDC_PROCESS_ITI5,		m_rglblProcessITI[5]);
	DDX_Control(pDX, IDC_PROCESS_ITI6,		m_rglblProcessITI[6]);
	DDX_Control(pDX, IDC_PROCESS_ITI7,		m_rglblProcessITI[7]);
	DDX_Control(pDX, IDC_PROCESS_ITI8,		m_rglblProcessITI[8]);
	DDX_Control(pDX, IDC_PROCESS_ITI9,		m_rglblProcessITI[9]);
	DDX_Control(pDX, IDC_PROCESS_ITI10,		m_rglblProcessITI[10]);
	DDX_Control(pDX, IDC_PROCESS_ITI11,		m_rglblProcessITI[11]);
	DDX_Control(pDX, IDC_PROCESS_ITI12,		m_rglblProcessITI[12]);
	DDX_Control(pDX, IDC_PROCESS_ITI13,		m_rglblProcessITI[13]);
	DDX_Control(pDX, IDC_PROCESS_ITI14,		m_rglblProcessITI[14]);
	DDX_Control(pDX, IDC_PROCESS_ITI15,		m_rglblProcessITI[15]);
	DDX_Control(pDX, IDC_PROCESS_ITI16,		m_rglblProcessITI[16]);
	DDX_Control(pDX, IDC_PROCESS_ITI17,		m_rglblProcessITI[17]);
	DDX_Control(pDX, IDC_PROCESS_ITI18,		m_rglblProcessITI[18]);
	DDX_Control(pDX, IDC_PROCESS_ITI19,		m_rglblProcessITI[19]);
	DDX_Control(pDX, IDC_PROCESS_ITI20,		m_rglblProcessITI[20]);
	DDX_Control(pDX, IDC_PROCESS_ITI21,		m_rglblProcessITI[21]);
	DDX_Control(pDX, IDC_PROCESS_ITI22,		m_rglblProcessITI[22]);
	DDX_Control(pDX, IDC_PROCESS_ITI23,		m_rglblProcessITI[23]);
	DDX_Control(pDX, IDC_PROCESS_ITI24,		m_rglblProcessITI[24]);
	DDX_Control(pDX, IDC_PROCESS_ITI25,		m_rglblProcessITI[25]);
	DDX_Control(pDX, IDC_PROCESS_ITI26,		m_rglblProcessITI[26]);
	DDX_Control(pDX, IDC_PROCESS_ITI27,		m_rglblProcessITI[27]);
	DDX_Control(pDX, IDC_PROCESS_ITI28,		m_rglblProcessITI[28]);
	DDX_Control(pDX, IDC_PROCESS_ITI29,		m_rglblProcessITI[29]);
	DDX_Control(pDX, IDC_PROCESS_ITI30,		m_rglblProcessITI[30]);
	DDX_Control(pDX, IDC_PROCESS_ITI31,		m_rglblProcessITI[31]);

	DDX_Control(pDX, IDC_WORKSIDE0,		m_rglblWorkSideName[0]);
	DDX_Control(pDX, IDC_WORKSIDE1,		m_rglblWorkSideName[1]);

	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE0,	m_rglblProcessDataValue[0][0]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE1,	m_rglblProcessDataValue[0][1]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE2,	m_rglblProcessDataValue[0][2]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE3,	m_rglblProcessDataValue[0][3]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE4,	m_rglblProcessDataValue[0][4]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE5,	m_rglblProcessDataValue[0][5]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE6,	m_rglblProcessDataValue[0][6]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE7,	m_rglblProcessDataValue[0][7]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE8,	m_rglblProcessDataValue[0][8]);

	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE0_1,	m_rglblProcessDataValue[1][0]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE1_1,	m_rglblProcessDataValue[1][1]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE2_1,	m_rglblProcessDataValue[1][2]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE3_1,	m_rglblProcessDataValue[1][3]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE4_1,	m_rglblProcessDataValue[1][4]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE5_1,	m_rglblProcessDataValue[1][5]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE6_1,	m_rglblProcessDataValue[1][6]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE7_1,	m_rglblProcessDataValue[1][7]);
	DDX_Control(pDX, IDC_PROCESS_DATA_VALUE8_1,	m_rglblProcessDataValue[1][8]);

	DDX_Control(pDX, IDC_PREARRANGED_PANEL_VALUE, m_lblNextPanelTabICLoadStart);
}


BEGIN_MESSAGE_MAP(CAutoStepDispDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoStepDispDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoStepDispDlg message handlers

void CAutoStepDispDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CAutoStepDispDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_plnkTrsPanelAligner		= MOlbSystemPre.GetTrsPanelAlignerComponent();
#ifdef DEF_USE_TRANSFER_CENTER
	m_plnkTrsPanelTransferC		= MOlbSystemPre.GetTrsPanelTransferCenterComponent();
#endif
#ifdef DEF_SOURCE_SYSTEM
	m_plnkTrsPanelTransfer		= MOlbSystemPre.GetTrsPanelTransferComponent();
#endif

	m_plnkTrsPanelTransferOut	= MOlbSystemPre.GetTrsPanelTransferOutComponent();
//	m_plnkTrsLoadConveyor	= MOlbSystemPre.GetTrsLoadConveyorComponent();
//	m_plnkTrsUnloadConveyor	= MOlbSystemPre.GetTrsUnloadConveyorComponent();
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_plnkTrsTabMounter[i] = MOlbSystemPre.GetTrsTabMounterComponent(i);
		m_plnkTrsRTabMounter[i] = MOlbSystemPre.GetTrsRTabMounterComponent(i);
	}
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_plnkTrsTabCarrier[i] = MOlbSystemPre.GetTrsTabCarrierComponent(i);
	}
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_plnkTrsRTabCarrier[i] = MOlbSystemPre.GetTrsRTabCarrierComponent(i);
	}
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		m_plnkTrsTabFeeder[i] = MOlbSystemPre.GetTrsTabFeederComponent(i);
	}
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_plnkTrsInspectionCamera[i] = MOlbSystemPre.GetTrsInspectionCameraComponent(i);
		m_plnkTrsRInspectionCamera[i] = MOlbSystemPre.GetTrsRInspectionCameraComponent(i);
	}
	m_plnkTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();

#ifdef DEF_USE_TURN_HANDLER
	m_plnkTrsTurnHandler = MOlbSystemPre.GetTrsTHandlerComponent();
#endif

	m_plnkCtrlPanelAligner = MOlbSystemPre.GetCtrlPanelAlignerComponent();

	m_plnkProcessData = MOlbSystemPre.GetProcessData();

	m_rglblProcessName[DEF_PROCESS_PANEL_ALIGNER].SetCaption(m_plnkTrsPanelAligner->GetObjectName().Mid(3));
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	m_rglblProcessName[DEF_PROCESS_PANEL_TRANSFER_CENTER].SetCaption(m_plnkTrsPanelTransferC->GetObjectName().Mid(3));
#	endif
	m_rglblProcessName[DEF_PROCESS_PANEL_TRANSFER_IN].SetCaption(m_plnkTrsPanelTransfer->GetObjectName().Mid(3));
#endif
	m_rglblProcessName[DEF_PROCESS_PANEL_TRANSFER_OUT].SetCaption(m_plnkTrsPanelTransferOut->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_TABMOUNTER1].SetCaption(m_plnkTrsTabMounter[DEF_TABMOUNTER1]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_TABMOUNTER2].SetCaption(m_plnkTrsTabMounter[DEF_TABMOUNTER2]->GetObjectName().Mid(3));
#ifdef DEF_SOURCE_SYSTEM
	m_rglblProcessName[DEF_PROCESS_TABMOUNTER3].SetCaption(m_plnkTrsTabMounter[DEF_TABMOUNTER3]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_TABMOUNTER4].SetCaption(m_plnkTrsTabMounter[DEF_TABMOUNTER4]->GetObjectName().Mid(3));
#endif
	m_rglblProcessName[DEF_PROCESS_R_TABMOUNTER1].SetCaption(m_plnkTrsRTabMounter[DEF_TABMOUNTER1]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_R_TABMOUNTER2].SetCaption(m_plnkTrsRTabMounter[DEF_TABMOUNTER2]->GetObjectName().Mid(3));
#ifdef DEF_SOURCE_SYSTEM
	m_rglblProcessName[DEF_PROCESS_R_TABMOUNTER3].SetCaption(m_plnkTrsRTabMounter[DEF_TABMOUNTER3]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_R_TABMOUNTER4].SetCaption(m_plnkTrsRTabMounter[DEF_TABMOUNTER4]->GetObjectName().Mid(3));
#endif
	m_rglblProcessName[DEF_PROCESS_TABCARRIER1].SetCaption(m_plnkTrsTabCarrier[DEF_TABCARRIER1]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_TABCARRIER2].SetCaption(m_plnkTrsTabCarrier[DEF_TABCARRIER2]->GetObjectName().Mid(3));
#ifdef DEF_SOURCE_SYSTEM
	m_rglblProcessName[DEF_PROCESS_TABCARRIER3].SetCaption(m_plnkTrsTabCarrier[DEF_TABCARRIER3]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_TABCARRIER4].SetCaption(m_plnkTrsTabCarrier[DEF_TABCARRIER4]->GetObjectName().Mid(3));
#endif
	m_rglblProcessName[DEF_PROCESS_R_TABCARRIER1].SetCaption(m_plnkTrsRTabCarrier[DEF_TABCARRIER1]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_R_TABCARRIER2].SetCaption(m_plnkTrsRTabCarrier[DEF_TABCARRIER2]->GetObjectName().Mid(3));
#ifdef DEF_SOURCE_SYSTEM
	m_rglblProcessName[DEF_PROCESS_R_TABCARRIER3].SetCaption(m_plnkTrsRTabCarrier[DEF_TABCARRIER3]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_R_TABCARRIER4].SetCaption(m_plnkTrsRTabCarrier[DEF_TABCARRIER4]->GetObjectName().Mid(3));
#endif
	m_rglblProcessName[DEF_PROCESS_INSPECT_CAMERA1].SetCaption(m_plnkTrsInspectionCamera[DEF_INSPECTION_CARRIER_1]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_INSPECT_CAMERA2].SetCaption(m_plnkTrsInspectionCamera[DEF_INSPECTION_CARRIER_2]->GetObjectName().Mid(3));
#ifdef DEF_SOURCE_SYSTEM
	m_rglblProcessName[DEF_PROCESS_INSPECT_CAMERA3].SetCaption(m_plnkTrsInspectionCamera[DEF_INSPECTION_CARRIER_3]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_INSPECT_CAMERA4].SetCaption(m_plnkTrsInspectionCamera[DEF_INSPECTION_CARRIER_4]->GetObjectName().Mid(3));
#endif
	m_rglblProcessName[DEF_PROCESS_R_INSPECT_CAMERA1].SetCaption(m_plnkTrsRInspectionCamera[DEF_INSPECTION_CARRIER_1]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_R_INSPECT_CAMERA2].SetCaption(m_plnkTrsRInspectionCamera[DEF_INSPECTION_CARRIER_2]->GetObjectName().Mid(3));
#ifdef DEF_SOURCE_SYSTEM
	m_rglblProcessName[DEF_PROCESS_R_INSPECT_CAMERA3].SetCaption(m_plnkTrsRInspectionCamera[DEF_INSPECTION_CARRIER_3]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_R_INSPECT_CAMERA4].SetCaption(m_plnkTrsRInspectionCamera[DEF_INSPECTION_CARRIER_4]->GetObjectName().Mid(3));
#endif
	m_rglblProcessName[DEF_PROCESS_TABFEEDER1].SetCaption(m_plnkTrsTabFeeder[DEF_TABFEEDER1]->GetObjectName().Mid(3));
	m_rglblProcessName[DEF_PROCESS_TABFEEDER2].SetCaption(m_plnkTrsTabFeeder[DEF_TABFEEDER2]->GetObjectName().Mid(3));

	//SJ_YYK 161104 Add...
#ifdef DEF_USE_TURN_HANDLER
	m_rglblProcessName[DEF_PROCESS_TURN_HANDLER].SetCaption(m_plnkTrsTurnHandler->GetObjectName().Mid(3));
#endif

	for (int i = DEF_MAX_PROCESS_INSTANCE; i < DEF_MAX_DISP_PROCESS_UNIT_NUM; i++)
	{
		m_rglblProcessName[i].EnableWindow(FALSE);
		m_rglblProcessStep[i].EnableWindow(FALSE);
		m_rglblProcessITI[i].EnableWindow(FALSE);

		m_rglblProcessName[i].ShowWindow(SW_HIDE);
		m_rglblProcessStep[i].ShowWindow(SW_HIDE);
		m_rglblProcessITI[i].ShowWindow(SW_HIDE);
	}

	// 반투명하게 만드는 루틴.
	SLWA pSetLayeredWindowAttributes = NULL;			// 함수포인터 선언, 초기화.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));	// 인스턴스 얻음.
	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes"); // 함수포인터 얻음.
	HWND hwnd = this->m_hWnd; // 다이얼로그의 핸들 얻음.
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | DEF_WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * 70) / 90, DEF_LWA_ALPHA);	// 다이얼로그 반투명하게 만듬.

	SetWindowPos(NULL, 30, 365, 0, 0, SWP_NOZORDER|SWP_NOSIZE);	// 투명 윈도우의 위치 지정

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp; //유니코드 변환 위해 추가
	ReadDisplayMessage(MAIN_AUTO, _T("AutoStepDispDlg_1"), &szTextTemp);
//숨기기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CAutoStepDispDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoStepDispDlg)
	ON_EVENT(CAutoStepDispDlg, IDB_CHANGE_COLOR, -600 /* Click */, OnChangeColor, VTS_NONE)
	ON_EVENT(CAutoStepDispDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAutoStepDispDlg::OnChangeColor() 
{
	if (m_iSelColor)
	{
		for (int i = 0; i < DEF_MAX_STEP_DISP_UNIT_NUM; i++)
		{
			m_rglblProcessName[i].SetBackColor(CLOUDBLUE);
			m_rglblProcessName[i].SetForeColor(BLACK);
		}
		m_iSelColor = 0;
	}
	else
	{
		for (int i = 0; i < DEF_MAX_STEP_DISP_UNIT_NUM; i++)
		{
			m_rglblProcessName[i].SetBackColor(YELLOW);
			m_rglblProcessName[i].SetForeColor(RED);
		}
		m_iSelColor = 1;
	}
}

void CAutoStepDispDlg::OnExit() 
{
	if (m_uiTimerID)
	{
		KillTimer(m_uiTimerID);
		m_uiTimerID = 0;
		Sleep(100);
	}

	this->ShowWindow(SW_HIDE);	
}

void CAutoStepDispDlg::OnTimer(UINT nIDEvent) 
{
	if (m_uiTimerID == nIDEvent)
	{		
		updateButton();
		updateDisplay();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CAutoStepDispDlg::updateDisplay()
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Thread Step Display
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	static int srgiOldStep[DEF_MAX_DISP_PROCESS_UNIT_NUM]
		= {	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
#ifdef DEF_SOURCE_SYSTEM
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
#else
			-1, -1, -1, -1, -1,	-1,
#endif
			};

	WCHAR buf[100];
	int rgiCurrentStep[DEF_MAX_PROCESS_INSTANCE];

	memset(buf, 0x00, sizeof(buf));

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Process Step Display
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	rgiCurrentStep[DEF_PROCESS_PANEL_ALIGNER]			= m_plnkTrsPanelAligner->GetStep();
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	rgiCurrentStep[DEF_PROCESS_PANEL_TRANSFER_CENTER]	= m_plnkTrsPanelTransferC->GetStep();
#	endif
	rgiCurrentStep[DEF_PROCESS_PANEL_TRANSFER_IN]		= m_plnkTrsPanelTransfer->GetStep();
#endif
	rgiCurrentStep[DEF_PROCESS_PANEL_TRANSFER_OUT]		= m_plnkTrsPanelTransferOut->GetStep();
	rgiCurrentStep[DEF_PROCESS_TABMOUNTER1]				= m_plnkTrsTabMounter[DEF_TABMOUNTER1]->GetStep();
	rgiCurrentStep[DEF_PROCESS_TABMOUNTER2]				= m_plnkTrsTabMounter[DEF_TABMOUNTER2]->GetStep();
#ifdef DEF_SOURCE_SYSTEM
	rgiCurrentStep[DEF_PROCESS_TABMOUNTER3]				= m_plnkTrsTabMounter[DEF_TABMOUNTER3]->GetStep();
	rgiCurrentStep[DEF_PROCESS_TABMOUNTER4]				= m_plnkTrsTabMounter[DEF_TABMOUNTER4]->GetStep();
#endif
	rgiCurrentStep[DEF_PROCESS_R_TABMOUNTER1]			= m_plnkTrsRTabMounter[DEF_TABMOUNTER1]->GetStep();
	rgiCurrentStep[DEF_PROCESS_R_TABMOUNTER2]			= m_plnkTrsRTabMounter[DEF_TABMOUNTER2]->GetStep();
#ifdef DEF_SOURCE_SYSTEM
	rgiCurrentStep[DEF_PROCESS_R_TABMOUNTER3]			= m_plnkTrsRTabMounter[DEF_TABMOUNTER3]->GetStep();
	rgiCurrentStep[DEF_PROCESS_R_TABMOUNTER4]			= m_plnkTrsRTabMounter[DEF_TABMOUNTER4]->GetStep();
#endif
	rgiCurrentStep[DEF_PROCESS_TABCARRIER1]				= m_plnkTrsTabCarrier[DEF_TABCARRIER1]->GetStep();
	rgiCurrentStep[DEF_PROCESS_TABCARRIER2]				= m_plnkTrsTabCarrier[DEF_TABCARRIER2]->GetStep();
#ifdef DEF_SOURCE_SYSTEM
	rgiCurrentStep[DEF_PROCESS_TABCARRIER3]				= m_plnkTrsTabCarrier[DEF_TABCARRIER3]->GetStep();
	rgiCurrentStep[DEF_PROCESS_TABCARRIER4]				= m_plnkTrsTabCarrier[DEF_TABCARRIER4]->GetStep();
#endif
	rgiCurrentStep[DEF_PROCESS_R_TABCARRIER1]			= m_plnkTrsRTabCarrier[DEF_TABCARRIER1]->GetStep();
	rgiCurrentStep[DEF_PROCESS_R_TABCARRIER2]			= m_plnkTrsRTabCarrier[DEF_TABCARRIER2]->GetStep();
#ifdef DEF_SOURCE_SYSTEM
	rgiCurrentStep[DEF_PROCESS_R_TABCARRIER3]			= m_plnkTrsRTabCarrier[DEF_TABCARRIER3]->GetStep();
	rgiCurrentStep[DEF_PROCESS_R_TABCARRIER4]			= m_plnkTrsRTabCarrier[DEF_TABCARRIER4]->GetStep();
#endif
	rgiCurrentStep[DEF_PROCESS_INSPECT_CAMERA1]			= m_plnkTrsInspectionCamera[DEF_INSPECTION_CARRIER_1]->GetStep();
	rgiCurrentStep[DEF_PROCESS_INSPECT_CAMERA2]			= m_plnkTrsInspectionCamera[DEF_INSPECTION_CARRIER_2]->GetStep();
#ifdef DEF_SOURCE_SYSTEM
	rgiCurrentStep[DEF_PROCESS_INSPECT_CAMERA3]			= m_plnkTrsInspectionCamera[DEF_INSPECTION_CARRIER_3]->GetStep();
	rgiCurrentStep[DEF_PROCESS_INSPECT_CAMERA4]			= m_plnkTrsInspectionCamera[DEF_INSPECTION_CARRIER_4]->GetStep();
#endif
	rgiCurrentStep[DEF_PROCESS_R_INSPECT_CAMERA1]		= m_plnkTrsRInspectionCamera[DEF_INSPECTION_CARRIER_1]->GetStep();
	rgiCurrentStep[DEF_PROCESS_R_INSPECT_CAMERA2]		= m_plnkTrsRInspectionCamera[DEF_INSPECTION_CARRIER_2]->GetStep();
#ifdef DEF_SOURCE_SYSTEM
	rgiCurrentStep[DEF_PROCESS_R_INSPECT_CAMERA3]		= m_plnkTrsRInspectionCamera[DEF_INSPECTION_CARRIER_3]->GetStep();
	rgiCurrentStep[DEF_PROCESS_R_INSPECT_CAMERA4]		= m_plnkTrsRInspectionCamera[DEF_INSPECTION_CARRIER_4]->GetStep();
#endif
	rgiCurrentStep[DEF_PROCESS_TABFEEDER1]				= m_plnkTrsTabFeeder[DEF_TABFEEDER1]->GetStep();
	rgiCurrentStep[DEF_PROCESS_TABFEEDER2]				= m_plnkTrsTabFeeder[DEF_TABFEEDER2]->GetStep();

#ifdef DEF_USE_TURN_HANDLER
	rgiCurrentStep[DEF_PROCESS_TURN_HANDLER]			= m_plnkTrsTurnHandler->GetStep(); //SJ_YYK 161104 Add.
#endif

	for (int i = 0; i < DEF_MAX_PROCESS_INSTANCE; i++)
	{
		if (srgiOldStep[i] != rgiCurrentStep[i])
		{
			srgiOldStep[i] = rgiCurrentStep[i];
			swprintf(buf, _T("%d"), rgiCurrentStep[i]);
			m_rglblProcessStep[i].SetCaption(CString(buf));
		}
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Process ITI Display
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	CString rgstrITI[DEF_MAX_PROCESS_INSTANCE];
	for (int i = 0; i < DEF_MAX_PROCESS_INSTANCE; i++)
		rgstrITI[i].Empty();

	/*

	// PanelAligner ITI
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsReceiveAbleToPanelTransfer()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsReceiveStartToPanelTransfer()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += " ";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsSendStartToPanelTransferOut()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsSendCompleteToPanelTransferOut()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += " ";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsPanelInEquipToWorker()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsWorkerEnabletoWork()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsInspectorEnabletoInspect()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += " ";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsPanelTransferEnabletoStopperUp()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_ALIGNER] += (m_plnkTrsPanelAligner->IsPanelTransferEnabletoMoveUnloadPos()) ? "1" : "0";

	// Panel Transfer ITI
	rgstrITI[DEF_PROCESS_PANEL_TRANSFER_IN] += (m_plnkTrsPanelTransfer->IsReceiveAbleToPanelTransferCenter()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_TRANSFER_IN] += (m_plnkTrsPanelTransfer->IsReceiveStartToPanelTransferCenter()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_TRANSFER_IN] += " ";
	rgstrITI[DEF_PROCESS_PANEL_TRANSFER_IN] += (m_plnkTrsPanelTransfer->IsSendStartToPanelAligner()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_TRANSFER_IN] += (m_plnkTrsPanelTransfer->IsSendCompleteToPanelAligner()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_PANEL_TRANSFER_IN] += " ";


	// TabMounter ITI : Front Work Group
	CString strTabNo;
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
	{
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += (m_plnkTrsTabMounter[i]->IsTabICReceiveStartToTC()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += (m_plnkTrsTabMounter[i]->IsTabICReceiveCompleteToTC()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += " ";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += (m_plnkTrsTabMounter[i]->IsInspectionAbleToIC()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += (m_plnkTrsTabMounter[i]->IsInspectionWaitStartToIC()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += (m_plnkTrsTabMounter[i]->IsAllInspectionCompleteToPA()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += " ";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += (m_plnkTrsTabMounter[i]->IsAllMountCompleteToPA()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += " ";
		strTabNo.Format(_T("%3d"), m_plnkTrsTabMounter[i]->GetMountTabNo());
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += strTabNo;
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += " ";
		rgstrITI[DEF_PROCESS_TABMOUNTER1 + i] += (m_plnkTrsTabMounter[i]->IsCurrentWork()) ? "1" : "0";
	}

	// TabCarrier1 ITI
	CString strVacID;
	int iVacuumID = -1;
	int j;
	int k = 0;

	for ( k=0; k<DEF_MAX_TABCARRIER; k++)
	{
		rgstrITI[DEF_PROCESS_TABCARRIER1+k] += (m_plnkTrsTabCarrier[DEF_TABCARRIER1+k]->IsTabICReceiveAbleToTF()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABCARRIER1+k] += (m_plnkTrsTabCarrier[DEF_TABCARRIER1+k]->IsTabICReceiveStartToTF()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABCARRIER1+k] += (m_plnkTrsTabCarrier[DEF_TABCARRIER1+k]->IsTabICReceiveCompleteToTF()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABCARRIER1+k] += " ";
		rgstrITI[DEF_PROCESS_TABCARRIER1+k] += (m_plnkTrsTabCarrier[DEF_TABCARRIER1+k]->IsTabICSendAbleToTM()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABCARRIER1+k] += (m_plnkTrsTabCarrier[DEF_TABCARRIER1+k]->IsTabICSendStartToTM()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABCARRIER1+k] += "\r\n";
		/ *
		for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		{
			iVacuumID = m_plnkTrsTabCarrier[DEF_TABCARRIER1+k]->GetWorkTabICNo(i);
			if (-1 == iVacuumID)
				strVacID.Format(_T("-"));
			else
				strVacID.Format(_T("%d"), iVacuumID);

			rgstrITI[DEF_PROCESS_TABCARRIER1+k] += strVacID;
		}
		* /
		iVacuumID = m_plnkTrsTabCarrier[DEF_TABCARRIER1+k]->GetWorkTabICNo();
		if (-1 == iVacuumID)
			strVacID.Format(_T("-"));
		else
			strVacID.Format(_T("%d"), iVacuumID);

		rgstrITI[DEF_PROCESS_TABCARRIER1+k] += strVacID;
	}

	for (k=0; k<DEF_MAX_TABCARRIER; k++)
	{
		rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += (m_plnkTrsRTabCarrier[DEF_TABCARRIER1+k]->IsTabICReceiveAbleToTF()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += (m_plnkTrsRTabCarrier[DEF_TABCARRIER1+k]->IsTabICReceiveStartToTF()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += (m_plnkTrsRTabCarrier[DEF_TABCARRIER1+k]->IsTabICReceiveCompleteToTF()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += " ";
		rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += (m_plnkTrsRTabCarrier[DEF_TABCARRIER1+k]->IsTabICSendAbleToTM()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += (m_plnkTrsRTabCarrier[DEF_TABCARRIER1+k]->IsTabICSendStartToTM()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += "\r\n";
		/ *
		for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		{
			iVacuumID = m_plnkTrsRTabCarrier[DEF_TABCARRIER1+k]->GetWorkTabICNo(i);
			if (-1 == iVacuumID)
				strVacID.Format(_T("-"));
			else
				strVacID.Format(_T("%d"), iVacuumID);

			rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += strVacID;
		}
		* /
		iVacuumID = m_plnkTrsRTabCarrier[DEF_TABCARRIER1+k]->GetWorkTabICNo();
		if (-1 == iVacuumID)
			strVacID.Format(_T("-"));
		else
			strVacID.Format(_T("%d"), iVacuumID);

		rgstrITI[DEF_PROCESS_R_TABCARRIER1+k] += strVacID;
	}

	// TabFeeder1 ITI
	rgstrITI[DEF_PROCESS_TABFEEDER1] += (m_plnkTrsTabFeeder[DEF_TABFEEDER1]->IsSendAbleToTC()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_TABFEEDER1] += (m_plnkTrsTabFeeder[DEF_TABFEEDER1]->IsSendStartToTC()) ? "1" : "0";

	// TabFeeder1 ITI
	rgstrITI[DEF_PROCESS_TABFEEDER2] += (m_plnkTrsTabFeeder[DEF_TABFEEDER2]->IsSendAbleToTC()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_TABFEEDER2] += (m_plnkTrsTabFeeder[DEF_TABFEEDER2]->IsSendStartToTC()) ? "1" : "0";

	// Inspection Camera ITI : Front Work Group
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
	{
		for (j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
		{
			rgstrITI[DEF_PROCESS_INSPECT_CAMERA1 + i] += (m_plnkTrsInspectionCamera[i]->IsInspectionStartToTM(j)) ? "1" : "0";
		}

		rgstrITI[DEF_PROCESS_INSPECT_CAMERA1 + i] += " ";

		for (j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
		{
			rgstrITI[DEF_PROCESS_INSPECT_CAMERA1 + i] += (m_plnkTrsInspectionCamera[i]->IsInspectionCompleteToTM(j)) ? "1" : "0";
		}

		strTabNo.Format(_T("%6d"), m_plnkTrsInspectionCamera[i]->GetInspectTabNo());
		rgstrITI[DEF_PROCESS_INSPECT_CAMERA1 + i] += strTabNo;
	}

#ifdef DEF_GATE_SYSTEM
	// TabMounter ITI : Rear Work Group
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
	{
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += (m_plnkTrsTabMounter[i + DEF_TABMOUNTER7]->IsTabICReceiveStartToTC()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += (m_plnkTrsTabMounter[i + DEF_TABMOUNTER7]->IsTabICReceiveCompleteToTC()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += " ";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += (m_plnkTrsTabMounter[i + DEF_TABMOUNTER7]->IsInspectionAbleToIC()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += (m_plnkTrsTabMounter[i + DEF_TABMOUNTER7]->IsInspectionWaitStartToIC()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += (m_plnkTrsTabMounter[i + DEF_TABMOUNTER7]->IsAllInspectionCompleteToPA()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += " ";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += (m_plnkTrsTabMounter[i + DEF_TABMOUNTER7]->IsAllMountCompleteToPA()) ? "1" : "0";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += " ";
		strTabNo.Format(_T("%3d"), m_plnkTrsTabMounter[i + DEF_TABMOUNTER7]->GetMountTabNo());
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += strTabNo;
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += " ";
		rgstrITI[DEF_PROCESS_TABMOUNTER7 + i] += (m_plnkTrsTabMounter[i + DEF_TABMOUNTER7]->IsCurrentWork()) ? "1" : "0";
	}

	// TabCarrier2 ITI
	rgstrITI[DEF_PROCESS_TABCARRIER2] += (m_plnkTrsTabCarrier[DEF_TABCARRIER2]->IsTabICReceiveAbleToTF()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_TABCARRIER2] += (m_plnkTrsTabCarrier[DEF_TABCARRIER2]->IsTabICReceiveStartToTF()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_TABCARRIER2] += (m_plnkTrsTabCarrier[DEF_TABCARRIER2]->IsTabICReceiveCompleteToTF()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_TABCARRIER2] += " ";
	rgstrITI[DEF_PROCESS_TABCARRIER2] += (m_plnkTrsTabCarrier[DEF_TABCARRIER2]->IsTabICSendAbleToTM()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_TABCARRIER2] += (m_plnkTrsTabCarrier[DEF_TABCARRIER2]->IsTabICSendStartToTM()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_TABCARRIER2] += "\r\n";
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
	{
		iVacuumID = m_plnkTrsTabCarrier[DEF_TABCARRIER2]->GetWorkTabICNo(i);
		if (-1 == iVacuumID)
			strVacID.Format(_T("-"));
		else
			strVacID.Format(_T("%d"), iVacuumID);

		rgstrITI[DEF_PROCESS_TABCARRIER2] += strVacID;
	}

	// TabFeeder2 ITI
	rgstrITI[DEF_PROCESS_TABFEEDER2] += (m_plnkTrsTabFeeder[DEF_TABFEEDER2]->IsSendAbleToTC()) ? "1" : "0";
	rgstrITI[DEF_PROCESS_TABFEEDER2] += (m_plnkTrsTabFeeder[DEF_TABFEEDER2]->IsSendStartToTC()) ? "1" : "0";

	// Inspection Camera ITI : Rear Work Group
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
	{
		for (j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
		{
			rgstrITI[DEF_PROCESS_INSPECT_CAMERA4 + i] += (m_plnkTrsInspectionCamera[i + DEF_INSPECTION_CAMERA4]->IsInspectionStartToTM(j)) ? "1" : "0";
		}

		rgstrITI[DEF_PROCESS_INSPECT_CAMERA4 + i] += " ";
		
		for (j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
		{
			rgstrITI[DEF_PROCESS_INSPECT_CAMERA4 + i] += (m_plnkTrsInspectionCamera[i + DEF_INSPECTION_CAMERA4]->IsInspectionCompleteToTM(j)) ? "1" : "0";
		}

		strTabNo.Format(_T("%6d"), m_plnkTrsInspectionCamera[i + DEF_INSPECTION_CAMERA4]->GetInspectTabNo());
		rgstrITI[DEF_PROCESS_INSPECT_CAMERA4 + i] += strTabNo;
	}
#endif

	for (int i = 0; i < DEF_MAX_PROCESS_INSTANCE; i++)
	{
		m_rglblProcessITI[i].SetCaption(rgstrITI[i]);
	}
	*/

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Process Data Display
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	CString rgstrData[DEF_MAX_WORKSIDE_PER_EQUIP_UNIT][DEF_MAX_DATA_DISPLAY_ITEM_NUM];
	CString strNextPanelTabICLoadStart;
	/*
	int iSpaceIndex = 0;

	for (int i = 0; i < DEF_MAX_WORKSIDE_PER_EQUIP_UNIT; i++)
	{
		if (m_rgiMaxTabQtyPerWorkSide[i] < 7)
			iSpaceIndex = m_rgiMaxTabQtyPerWorkSide[i];
		else if (m_rgiMaxTabQtyPerWorkSide[i] < 13)
			iSpaceIndex = m_rgiMaxTabQtyPerWorkSide[i] / 2;
		else
			iSpaceIndex = m_rgiMaxTabQtyPerWorkSide[i] / 4;

		if (i == 0 && WORKING_SIDE_X_UP != m_rgeWorkSide[0] && WORKING_SIDE_Y_LEFT != m_rgeWorkSide[0])
		{
			continue;
		}

		if (i == 1 && WORKING_SIDE_Y_RIGHT != m_rgeWorkSide[1])
		{
			continue;
		}

		for (j = 0; j < DEF_MAX_DATA_DISPLAY_ITEM_NUM; j++)
		{
			rgstrData[i][j].Empty();
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][0] += (m_plnkProcessData->GetWorkSkip(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][0] += " ";
			}
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][1] += (m_plnkProcessData->IsRemoveTab(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][1] += " ";
			}
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][2] += (m_plnkProcessData->IsCarrierLoadCompleted(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][2] += " ";
			}
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][3] += (m_plnkProcessData->IsMounterLoadCompleted(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][3] += " ";
			}
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][4] += (m_plnkProcessData->IsTabPrealignCompleted(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][4] += " ";
			}
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][5] += (m_plnkProcessData->IsInspectionCompleted(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][5] += " ";
			}
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][6] += (m_plnkProcessData->IsMountCompleted(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][6] += " ";
			}
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][7] += (m_plnkProcessData->IsNextPanelTabICLoadComplete(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][7] += " ";
			}
		}

		for (k = 0; k < m_rgiMaxTabQtyPerWorkSide[i]; k++)
		{
			rgstrData[i][8] += (m_plnkProcessData->IsAfterInspectionCompleted(m_rgeWorkSide[i], k)) ? "1" : "0";
			
			if ((k + 1) % iSpaceIndex == 0)
			{
				rgstrData[i][8] += " ";
			}
		}

		for (j = 0; j < DEF_MAX_DATA_DISPLAY_ITEM_NUM; j++)
		{
			m_rglblProcessDataValue[i][j].SetCaption(rgstrData[i][j]);
		}

		strNextPanelTabICLoadStart += (TRUE == m_plnkProcessData->IsNextPanelTabICLoadStart(i)) ? "1" : "0";
	}
	m_lblNextPanelTabICLoadStart.SetCaption(strNextPanelTabICLoadStart);
	*/
}

void CAutoStepDispDlg::updateButton()
{
	static int bOldAutoRun[DEF_MAX_STEP_DISP_UNIT_NUM]
		= {	FALSE, };
	
	BOOL bAutoRun[DEF_MAX_STEP_DISP_UNIT_NUM];
	m_plnkTrsAutoManager->GetThreadAutoRunStatus(bAutoRun);
	
	for (int i = 0; i < DEF_MAX_STEP_DISP_UNIT_NUM; i++)
	{
		if (bOldAutoRun[i] != bAutoRun[i])
		{
			bOldAutoRun[i] = bAutoRun[i];
			if (bAutoRun[i])
			{
				m_rglblProcessName[i].SetBackColor(YELLOW);
				m_rglblProcessName[i].SetForeColor(RED);
			}
			else
			{
				m_rglblProcessName[i].SetBackColor(CLOUDBLUE);
				m_rglblProcessName[i].SetForeColor(BLACK);				
			}
		}		
	}

	// 확인 필요 : 아래 코드 계속 호출할 필요는 없는 부분임. 코드 정리해서 최적화 필요.
	// WorkSide Name Display
#ifdef DEF_SOURCE_SYSTEM
	m_rglblWorkSideName[1].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][0].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][1].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][2].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][3].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][4].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][5].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][6].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][7].EnableWindow(FALSE);
	m_rglblProcessDataValue[1][8].EnableWindow(FALSE);

	m_rglblWorkSideName[0].SetCaption(_T("Xu"));	// 확인 필요 : Xd
	m_rgeWorkSide[0] = WORKING_SIDE_X_UP;
	m_rgeWorkSide[1] = WORKING_SIDE_UNKNOWN;
	m_rgiMaxTabQtyPerWorkSide[0] = MOlbSystemPre.GetTabData()->m_iXuTabQty;
#else
	if (TRUE == MOlbSystemPre.GetPanelData()->m_bUseFlag_Yl)
	{
		m_rglblWorkSideName[0].SetCaption(_T("Yl"));
		m_rgeWorkSide[0] = WORKING_SIDE_Y_LEFT;
		m_rgiMaxTabQtyPerWorkSide[0] = MOlbSystemPre.GetTabData()->m_iYlTabQty;
	}
	else
	{
		m_rglblWorkSideName[0].SetCaption(_T("-"));
		m_rgeWorkSide[0] = WORKING_SIDE_UNKNOWN;
		m_rgiMaxTabQtyPerWorkSide[0] = 0;
	}

	if (TRUE == MOlbSystemPre.GetPanelData()->m_bUseFlag_Yr)
	{
		m_rglblWorkSideName[1].SetCaption(_T("Yr"));
		m_rgeWorkSide[1] = WORKING_SIDE_Y_RIGHT;
		m_rgiMaxTabQtyPerWorkSide[1] = MOlbSystemPre.GetTabData()->m_iYrTabQty;
	}
	else
	{
		m_rglblWorkSideName[1].SetCaption(_T("-"));
		m_rgeWorkSide[1] = WORKING_SIDE_UNKNOWN;
		m_rgiMaxTabQtyPerWorkSide[1] = 0;
	}
#endif
}

void CAutoStepDispDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);	
}

void CAutoStepDispDlg::StartDlgTimer()
{
	if (m_uiTimerID == 0)
		m_uiTimerID = SetTimer(10, 500, NULL);
}

void CAutoStepDispDlg::StopDlgTimer()
{
	if (m_uiTimerID)
	{
		KillTimer(m_uiTimerID);
		m_uiTimerID = 0;
	}
}
