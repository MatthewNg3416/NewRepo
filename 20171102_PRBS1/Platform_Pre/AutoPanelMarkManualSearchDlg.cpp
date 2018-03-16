// AutoPanelMarkManualSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlbSystem.h"
#include "AutoPanelMarkManualSearchDlg.h"

#include "IVision.h"
#include "MManageOpPanel.h"
#include "MTrsAutoManager.h"
#include "IOpPanel.h"
#include "IAxis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CAutoPanelMarkManualSearchDlg dialog


CAutoPanelMarkManualSearchDlg::CAutoPanelMarkManualSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoPanelMarkManualSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoPanelMarkManualSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_plnkTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
	m_plnkVision = MOlbSystemPre.GetVisionComponent();
	m_plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
	BOOL bBuzzerMode = MOlbSystemPre.GetSystemData()->m_bUseBuzzer;
	m_plnkManageOpPanel->SetBuzzerMode(bBuzzerMode);

	m_plnkX = MOlbSystemPre.GetMotionComponent(DEF_AXIS_SERCOS_STAGE_X);
	m_plnkY = MOlbSystemPre.GetMotionComponent(DEF_AXIS_SERCOS_STAGE_Y);
	m_plnkY2 = MOlbSystemPre.GetMotionComponent(DEF_AXIS_SERCOS_STAGE_T);
}


void CAutoPanelMarkManualSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoPanelMarkManualSearchDlg)
	DDX_Control(pDX, IDS_Y_PLUS, m_lblYPlus);
	DDX_Control(pDX, IDS_Y_MINUS, m_lblYMinus);
	DDX_Control(pDX, IDS_X_PLUS, m_lblXPlus);
	DDX_Control(pDX, IDS_X_MINUS, m_lblXMinus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoPanelMarkManualSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoPanelMarkManualSearchDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoPanelMarkManualSearchDlg message handlers

BEGIN_EVENTSINK_MAP(CAutoPanelMarkManualSearchDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoPanelMarkManualSearchDlg)
	ON_EVENT(CAutoPanelMarkManualSearchDlg, IDB_BUZZER_OFF, -600 /* Click */, OnClickBuzzerOff, VTS_NONE)
	ON_EVENT(CAutoPanelMarkManualSearchDlg, IDB_PASS, -600 /* Click */, OnClickPass, VTS_NONE)
	ON_EVENT(CAutoPanelMarkManualSearchDlg, IDB_REMOVE, -600 /* Click */, OnClickRemove, VTS_NONE)
	ON_EVENT(CAutoPanelMarkManualSearchDlg, IDB_ALIGN_CAMERA1, -600 /* Click */, OnClickAlignCamera1, VTS_NONE)
	ON_EVENT(CAutoPanelMarkManualSearchDlg, IDB_ALIGN_CAMERA2, -600 /* Click */, OnClickAlignCamera2, VTS_NONE)
	ON_EVENT(CAutoPanelMarkManualSearchDlg, IDB_RUN_STOP, -600 /* Click */, OnRunStop, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAutoPanelMarkManualSearchDlg::OnClickBuzzerOff() 
{
	m_plnkManageOpPanel->SetBuzzerMode(FALSE);	
}

void CAutoPanelMarkManualSearchDlg::OnClickPass() 
{
	/*/
	if (TRUE == checkAxisState(m_plnkX)	|| TRUE == checkAxisState(m_plnkY))
	{
		CString strMsg;
//	PanelAligner 축이 정상 상태가 아니므로, 자동운전을 정지합니다.
		MyMessageBox(MY_MESSAGE_BOX, 20300, _T("Information"), M_ICONINFORMATION);

		EndDialog(DEF_PANEL_ALIGN_RUN_STOP);
	}
	else
	{
		EndDialog(DEF_PANEL_ALIGN_PASS);
	}
	/*/
	EndDialog(DEF_PANEL_ALIGN_PASS);
}

void CAutoPanelMarkManualSearchDlg::OnClickRemove() 
{
	EndDialog(DEF_PANEL_ALIGN_PULL_OUT);
}

BOOL CAutoPanelMarkManualSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	RECT rect;

	m_plnkManageOpPanel->SetJogUnit(DEF_JOG_STAGE_XYT);

	CWnd* pWnd = GetDlgItem(IDC_MONITOR1);
	pWnd->GetWindowRect(&rect);
	CRect rectView1(rect.left, rect.top, rect.right, rect.bottom);
	ScreenToClient(&rectView1);
	m_iViewID1 = m_plnkVision->CreateTemporaryView(&rectView1, this);

	m_plnkVision->HaltVideo(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->LiveVideo(DEF_PANEL_ALIGN_CAMERA_1);
	

	CString szTextTemp;
	
	ReadDisplayMessage(MAIN_AUTO, _T("AutoPanelMarkManualSearchDlg_1"), &szTextTemp);
//Panel\r\n제거
	GetDlgItem(IDB_REMOVE)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("AutoPanelMarkManualSearchDlg_2"), &szTextTemp);
//진행
	GetDlgItem(IDB_PASS)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("AutoPanelMarkManualSearchDlg_3"), &szTextTemp);
//Jog를 움직여 Mark를 들어오게 한 후, 확인을 눌러주세요.
	GetDlgItem(IDB_BUZZER_OFF2)->SetWindowText(szTextTemp);	

	m_iTimerID = SetTimer(1, 100, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoPanelMarkManualSearchDlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_iTimerID)
		KillTimer(m_iTimerID);

#ifndef SIMULATION
	m_plnkManageOpPanel->SetJogUnit(-1);
#endif /* SIMULATION */

	m_plnkVision->HaltVideo(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_1);

	m_plnkVision->HaltVideo(DEF_PANEL_ALIGN_CAMERA_2);
	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_2);

	m_plnkManageOpPanel->SetBuzzerMode(FALSE);
	m_plnkTrsAutoManager->SetDisplayErrorImage(FALSE);

	m_plnkVision->DestroyView(m_iViewID1);
	m_plnkVision->DestroyView(m_iViewID2);
}

void CAutoPanelMarkManualSearchDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(),&m_brBckColor);	
	// Do not call CDialog::OnPaint() for painting messages
}

void CAutoPanelMarkManualSearchDlg::OnClickAlignCamera1() 
{
	m_plnkVision->HaltVideo(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_1);
	m_plnkVision->LiveVideo(DEF_PANEL_ALIGN_CAMERA_1);
}

void CAutoPanelMarkManualSearchDlg::OnClickAlignCamera2() 
{
	m_plnkVision->HaltVideo(DEF_PANEL_ALIGN_CAMERA_2);
	m_plnkVision->ClearOverlay(DEF_PANEL_ALIGN_CAMERA_2);
	m_plnkVision->LiveVideo(DEF_PANEL_ALIGN_CAMERA_2);
}

void CAutoPanelMarkManualSearchDlg::OnRunStop() 
{
	EndDialog(DEF_PANEL_ALIGN_RUN_STOP);
}

void CAutoPanelMarkManualSearchDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (TRUE == checkAxisState(m_plnkX))
	{
		if (TRUE == m_plnkX->IsOnNegativeSensor())
			m_lblXMinus.SetBackColor(RED);
		else
			m_lblXMinus.SetBackColor(WHITE);
		
		if (TRUE == m_plnkX->IsOnPositiveSensor())
			m_lblXPlus.SetBackColor(RED);
		else
			m_lblXPlus.SetBackColor(WHITE);

		if (FALSE == m_plnkX->IsOnNegativeSensor()
			&& FALSE == m_plnkX->IsOnPositiveSensor())
		{
			m_lblXMinus.SetBackColor(RED);
			m_lblXPlus.SetBackColor(RED);
		}
	}
	else
	{
		m_lblXMinus.SetBackColor(WHITE);
		m_lblXPlus.SetBackColor(WHITE);
	}

	if (TRUE == checkAxisState(m_plnkY))
	{
		if (TRUE == m_plnkY->IsOnNegativeSensor())
			m_lblYMinus.SetBackColor(RED);
		else
			m_lblYMinus.SetBackColor(WHITE);
		
		if (TRUE == m_plnkY->IsOnPositiveSensor())
			m_lblYPlus.SetBackColor(RED);
		else
			m_lblYPlus.SetBackColor(WHITE);

		if (FALSE == m_plnkY->IsOnNegativeSensor()
			&& FALSE == m_plnkY->IsOnPositiveSensor())
		{
			m_lblYMinus.SetBackColor(RED);
			m_lblYPlus.SetBackColor(RED);
		}
	}
	else
	{
		m_lblYMinus.SetBackColor(WHITE);
		m_lblYPlus.SetBackColor(WHITE);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CAutoPanelMarkManualSearchDlg::checkAxisState(IAxis* plnkAxis)
{
	if (TRUE == plnkAxis->IsOnNegativeSensor())
		return TRUE;

	if (TRUE == plnkAxis->IsOnPositiveSensor())
		return TRUE;

	if (FALSE == plnkAxis->IsAmpEnable())
		return TRUE;

	if (TRUE == plnkAxis->IsAmpFault())
		return TRUE;

	if (TRUE == plnkAxis->IsEStopEvent())
		return TRUE;

	if (FALSE == plnkAxis->IsOriginReturn())
		return TRUE;

	return FALSE;
}
