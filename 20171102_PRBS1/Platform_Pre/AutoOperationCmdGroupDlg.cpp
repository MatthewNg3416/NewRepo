// AutoOperationCmdGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "AutoOperationCmdGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoOperationCmdGroupDlg dialog

// selsk_jdy CAutoOperationCmdGroupDlg::CAutoOperationCmdGroupDlg(HWND hWnd, CWnd* pParent /*=NULL*/)
CAutoOperationCmdGroupDlg::CAutoOperationCmdGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoOperationCmdGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoOperationCmdGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// selsk_jdy m_hAutoMainWnd = hWnd;

	// selsk_jdy
	m_hAutoMainWnd = pParent->m_hWnd;
}


void CAutoOperationCmdGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoOperationCmdGroupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoOperationCmdGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoOperationCmdGroupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoOperationCmdGroupDlg message handlers

BEGIN_EVENTSINK_MAP(CAutoOperationCmdGroupDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoOperationCmdGroupDlg)
	ON_EVENT(CAutoOperationCmdGroupDlg, IDB_MOVE_TABMOUNTER, -600 /* Click */, OnMoveTabMounter, VTS_NONE)
	ON_EVENT(CAutoOperationCmdGroupDlg, IDB_REMOVE_TABIC, -600 /* Click */, OnRemoveTabIC, VTS_NONE)
	ON_EVENT(CAutoOperationCmdGroupDlg, IDB_TIME_DISPLAY, -600 /* Click */, OnCylinderMoveTimeDisplay, VTS_NONE)
	ON_EVENT(CAutoOperationCmdGroupDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CAutoOperationCmdGroupDlg, IDB_MOVE_INSPECTION_CAMERA, -600 /* Click */, OnMoveInspectionCamera, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAutoOperationCmdGroupDlg::OnMoveTabMounter() 
{
	::PostMessage(m_hAutoMainWnd, WM_MOVE_TAB_MOUNTER_REQ, TRUE, NULL);
	EndDialog(0);
}

void CAutoOperationCmdGroupDlg::OnMoveInspectionCamera() 
{
	::PostMessage(m_hAutoMainWnd, WM_MOVE_INSPECTION_CAMERA_REQ, NULL, NULL);
	EndDialog(0);
}

void CAutoOperationCmdGroupDlg::OnRemoveTabIC() 
{
	::PostMessage(m_hAutoMainWnd, WM_REMOVE_TABIC_REQ, TRUE, FALSE);
	EndDialog(0);
}

void CAutoOperationCmdGroupDlg::OnCylinderMoveTimeDisplay() 
{
	::PostMessage(m_hAutoMainWnd, WM_DISP_CYLINDER_MOVE_TIME_DLG, NULL, NULL);
	EndDialog(0);
}

void CAutoOperationCmdGroupDlg::OnExit() 
{
	EndDialog(0);
}

BOOL CAutoOperationCmdGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString szTextTemp;

//TabMounter Ready 위치이동
	ReadDisplayMessage(MAIN_AUTO, _T("AutoOperationCmdGroupDlg_1"), &szTextTemp);
	GetDlgItem(IDB_MOVE_TABMOUNTER)->SetWindowText(szTextTemp);
//Gantry 작업 위치
	ReadDisplayMessage(MAIN_AUTO, _T("AutoOperationCmdGroupDlg_2"), &szTextTemp);
	GetDlgItem(IDB_MOVE_INSPECTION_CAMERA)->SetWindowText(szTextTemp);
//TabIC 제거
	ReadDisplayMessage(MAIN_AUTO, _T("AutoOperationCmdGroupDlg_3"), &szTextTemp);
	GetDlgItem(IDB_REMOVE_TABIC)->SetWindowText(szTextTemp);
//나가기
	ReadDisplayMessage(MAIN_AUTO, _T("AutoOperationCmdGroupDlg_4"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
