// AutoModeSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "AutoModeSelectDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"
#include "MPanelData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CAutoModeSelectDlg dialog

CAutoModeSelectDlg::CAutoModeSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoModeSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoModeSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
}


void CAutoModeSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoModeSelectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	
	DDX_Control(pDX, IDB_AUTO_RUN,		m_BtnMode[0]);
	DDX_Control(pDX, IDB_PASS_RUN,		m_BtnMode[1]);
	DDX_Control(pDX, IDB_IDLE_RUN,		m_BtnMode[2]);
	DDX_Control(pDX, IDB_REPAIR_RUN,	m_BtnMode[3]);
}


BEGIN_MESSAGE_MAP(CAutoModeSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoModeSelectDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoModeSelectDlg message handlers

void CAutoModeSelectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);

	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CAutoModeSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_eRunMode = m_plnkSystemData->m_eRunMode;
	if(m_eRunMode == NORMAL_RUN_MODE)
		m_BtnMode[0].SetValue(TRUE);		
	else if(m_eRunMode == PASS_RUN_MODE)
		m_BtnMode[1].SetValue(TRUE);
	else if(m_eRunMode == DRY_RUN_MODE)
		m_BtnMode[2].SetValue(TRUE);
	else if(m_eRunMode == REPAIR_RUN_MODE)
		m_BtnMode[3].SetValue(TRUE);

	m_plnkPanelData = MOlbSystemPre.GetPanelData();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
//운영 모드 선택
	CString szTextTemp;
	ReadDisplayMessage(MAIN_AUTO, _T("AutoModeSelectDlg_1"), &szTextTemp);
	SetWindowText(szTextTemp);

//나가기
	ReadDisplayMessage(MAIN_AUTO, _T("AutoModeSelectDlg_2"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CAutoModeSelectDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoModeSelectDlg)
	ON_EVENT(CAutoModeSelectDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CAutoModeSelectDlg, IDB_AUTO_RUN, -600 /* Click */, OnAutoRun, VTS_NONE)
	ON_EVENT(CAutoModeSelectDlg, IDB_PASS_RUN, -600 /* Click */, OnPassRun, VTS_NONE)
	ON_EVENT(CAutoModeSelectDlg, IDB_IDLE_RUN, -600 /* Click */, OnIdleRun, VTS_NONE)
	ON_EVENT(CAutoModeSelectDlg, IDB_REPAIR_RUN, -600 /* Click */, OnRepairRun, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAutoModeSelectDlg::OnExit() 
{
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_RUN_MODE, NULL, NULL);					
	CDialog::OnCancel();
}

void CAutoModeSelectDlg::OnAutoRun() 
{
	m_eRunMode = NORMAL_RUN_MODE;
	m_plnkSystemData->m_eRunMode = m_eRunMode;
	m_plnkPanelData->m_eRunMode = m_eRunMode;
	m_plnkSystemData->WriteData();	
	m_plnkPanelData->WriteData();	
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_RUN_MODE, NULL, NULL);					
	OnExit();
}

void CAutoModeSelectDlg::OnPassRun() 
{
	m_eRunMode = PASS_RUN_MODE;	
	m_plnkSystemData->m_eRunMode = m_eRunMode;
	m_plnkPanelData->m_eRunMode = m_eRunMode;
	m_plnkSystemData->WriteData();	
	m_plnkPanelData->WriteData();	
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_RUN_MODE, NULL, NULL);					
	OnExit();
}

void CAutoModeSelectDlg::OnIdleRun() 
{
	m_eRunMode = DRY_RUN_MODE;	
	m_plnkSystemData->m_eRunMode = m_eRunMode;
	m_plnkPanelData->m_eRunMode = m_eRunMode;
	m_plnkSystemData->WriteData();	
	m_plnkPanelData->WriteData();	
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_RUN_MODE, NULL, NULL);					
	OnExit();
}

void CAutoModeSelectDlg::OnRepairRun() 
{
	m_eRunMode = REPAIR_RUN_MODE;	
	m_plnkSystemData->m_eRunMode = m_eRunMode;
	m_plnkPanelData->m_eRunMode = m_eRunMode;
	m_plnkSystemData->WriteData();
	m_plnkPanelData->WriteData();	
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_RUN_MODE, NULL, NULL);					
	OnExit();
}
