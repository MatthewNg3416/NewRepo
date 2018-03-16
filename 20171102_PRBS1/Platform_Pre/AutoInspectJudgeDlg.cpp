// AutoInspectJudgeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlbSystem.h"
#include "AutoInspectJudgeDlg.h"

#include "IVision.h"
#include "MManageOpPanel.h"
#include "MTrsAutoManager.h"
#include "IOpPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CAutoInspectJudgeDlg dialog


CAutoInspectJudgeDlg::CAutoInspectJudgeDlg(int iCamNo, CWnd* pParent /*=NULL*/)
	: CDialog(CAutoInspectJudgeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoInspectJudgeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_plnkTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
	m_plnkVision = MOlbSystemPre.GetVisionComponent();
	m_plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
	BOOL bBuzzerMode = MOlbSystemPre.GetSystemData()->m_bUseBuzzer;
	m_plnkManageOpPanel->SetBuzzerMode(bBuzzerMode);
	
	m_iCamNo = iCamNo;
	if (m_iCamNo % 2 == 0)
	{
		m_iCamNo1 = m_iCamNo;
		m_iCamNo2 = m_iCamNo + 1;
	}
	else
	{
		m_iCamNo1 = m_iCamNo - 1;
		m_iCamNo2 = m_iCamNo;
	}
	switch (m_iCamNo)
	{
	case DEF_INSPECTION_CAMERA_1:
	case DEF_INSPECTION_CAMERA_2:
		m_iCamCarrierNo = 0;
		break;
	case DEF_INSPECTION_CAMERA_3:
	case DEF_INSPECTION_CAMERA_4:
		m_iCamCarrierNo = 1;
		break;
#ifdef DEF_SOURCE_SYSTEM
	case DEF_INSPECTION_CAMERA_5:
	case DEF_INSPECTION_CAMERA_6:
		m_iCamCarrierNo = 2;
		break;
	case DEF_INSPECTION_CAMERA_7:
	case DEF_INSPECTION_CAMERA_8:
		m_iCamCarrierNo = 3;
		break;
#endif
	}
}


void CAutoInspectJudgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoInspectJudgeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoInspectJudgeDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoInspectJudgeDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoInspectJudgeDlg message handlers

BEGIN_EVENTSINK_MAP(CAutoInspectJudgeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoInspectJudgeDlg)
	ON_EVENT(CAutoInspectJudgeDlg, IDB_BUZZER_OFF, -600 /* Click */, OnClickBuzzerOff, VTS_NONE)
	ON_EVENT(CAutoInspectJudgeDlg, IDB_RUN_STOP, -600 /* Click */, OnClickRunStop, VTS_NONE)
	ON_EVENT(CAutoInspectJudgeDlg, IDB_RETRY_RECOG, -600 /* Click */, OnClickRetryRecog, VTS_NONE)
	ON_EVENT(CAutoInspectJudgeDlg, IDB_TAB_RELOAD, -600 /* Click */, OnClickTabReload, VTS_NONE)
	ON_EVENT(CAutoInspectJudgeDlg, IDB_ALIGN_CAMERA1, -600 /* Click */, OnClickAlignCamera1, VTS_NONE)
	ON_EVENT(CAutoInspectJudgeDlg, IDB_ALIGN_CAMERA2, -600 /* Click */, OnClickAlignCamera2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CAutoInspectJudgeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	RECT rect;

	CString strCamCarrierNo;
	strCamCarrierNo.Format(_T("Inspect Camera Carrier No. %d"), m_iCamCarrierNo+1);
	SetWindowText(strCamCarrierNo);

	CWnd* pWnd = GetDlgItem(IDC_MONITOR1);
	pWnd->GetWindowRect(&rect);
	CRect rectView1(rect.left, rect.top, rect.right, rect.bottom);
	ScreenToClient(&rectView1);
	m_iViewID1 = m_plnkVision->CreateTemporaryView(&rectView1, this);

	m_plnkVision->HaltVideo(m_iCamNo);
	m_plnkVision->ClearOverlay(m_iCamNo);
	m_plnkVision->LiveVideo(m_iCamNo);
	

	/*
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
	*/

	m_iTimerID = SetTimer(1, 200, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoInspectJudgeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(),&m_brBckColor);	
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CAutoInspectJudgeDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_iTimerID)
		KillTimer(m_iTimerID);

#ifndef SIMULATION
//	m_plnkManageOpPanel->SetJogUnit(-1);
#endif /* SIMULATION */

	m_plnkVision->HaltVideo(m_iCamNo1);
	m_plnkVision->ClearOverlay(m_iCamNo1);

	m_plnkVision->HaltVideo(m_iCamNo2);
	m_plnkVision->ClearOverlay(m_iCamNo2);

	m_plnkManageOpPanel->SetBuzzerMode(FALSE);
	m_plnkTrsAutoManager->SetDisplayErrorImage(FALSE);

	m_plnkVision->DestroyView(m_iViewID1);
//	m_plnkVision->DestroyView(m_iViewID2);	
}

void CAutoInspectJudgeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CAutoInspectJudgeDlg::OnClickBuzzerOff() 
{
	// TODO: Add your control notification handler code here
	m_plnkManageOpPanel->SetBuzzerMode(FALSE);	
}

void CAutoInspectJudgeDlg::OnClickRunStop() 
{
	// TODO: Add your control notification handler code here
	EndDialog(DEF_TABINSPECTION_RUN_STOP);
}

void CAutoInspectJudgeDlg::OnClickRetryRecog() 
{
	// TODO: Add your control notification handler code here
	EndDialog(DEF_TABINSPECTION_CURTAB_RETRY);	
}

void CAutoInspectJudgeDlg::OnClickTabReload() 
{
	// TODO: Add your control notification handler code here
	EndDialog(DEF_TABINSPECTION_TAB_RELOAD);		
}

void CAutoInspectJudgeDlg::OnClickAlignCamera1() 
{
	// TODO: Add your control notification handler code here
	m_plnkVision->HaltVideo(m_iCamNo1);
	m_plnkVision->ClearOverlay(m_iCamNo1);
	m_plnkVision->LiveVideo(m_iCamNo1);
}

void CAutoInspectJudgeDlg::OnClickAlignCamera2() 
{
	// TODO: Add your control notification handler code here
	m_plnkVision->HaltVideo(m_iCamNo2);
	m_plnkVision->ClearOverlay(m_iCamNo2);
	m_plnkVision->LiveVideo(m_iCamNo2);
}
