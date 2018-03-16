// AutoRepairTabSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "AutoRepairTabSelectDlg.h"
#include "MPanelData.h"
#include "MProcessData.h"
#include "MPlatformOlbSystem.h"
#include "MTrsPanelAligner.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CAutoRepairTabSelectDlg dialog

CAutoRepairTabSelectDlg::CAutoRepairTabSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoRepairTabSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoRepairTabSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_plnkPanelData = MOlbSystemPre.GetPanelData();
	m_plnkProcessData = MOlbSystemPre.GetProcessData();

	m_plnkITIPanelAligner = dynamic_cast<ITI_PanelAligner*>(MOlbSystemPre.GetTrsPanelAlignerComponent());

	m_uiTimerID = 0;
	m_iWorkSide = WORKING_SIDE_UNKNOWN;
}


void CAutoRepairTabSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoRepairTabSelectDlg)
	DDX_Control(pDX, IDB_TAB_NEXT, m_btnNext);
	DDX_Control(pDX, IDB_REPAIR_RUN_CONTINUE, m_btnContinue);
	//}}AFX_DATA_MAP
	int i = 0;
	DDX_Control(pDX, PDB_ON_1,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_2,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_3,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_4,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_5,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_6,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_7,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_8,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_9,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_10,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_11,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_12,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_13,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_14,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_15,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_16,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_17,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_18,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_19,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_20,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_21,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_22,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_23,		m_btnSelect[i++]);
	DDX_Control(pDX, PDB_ON_24,		m_btnSelect[i++]);
}


BEGIN_MESSAGE_MAP(CAutoRepairTabSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoRepairTabSelectDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoRepairTabSelectDlg message handlers

BEGIN_EVENTSINK_MAP(CAutoRepairTabSelectDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoRepairTabSelectDlg)
	ON_EVENT(CAutoRepairTabSelectDlg, IDB_TAB_NEXT, -600 /* Click */, OnClickTabNext, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_1, -600 /* Click */, OnClickOn1, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_2, -600 /* Click */, OnClickOn2, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_3, -600 /* Click */, OnClickOn3, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_4, -600 /* Click */, OnClickOn4, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_5, -600 /* Click */, OnClickOn5, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_6, -600 /* Click */, OnClickOn6, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_7, -600 /* Click */, OnClickOn7, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_8, -600 /* Click */, OnClickOn8, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_9, -600 /* Click */, OnClickOn9, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_10, -600 /* Click */, OnClickOn10, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_11, -600 /* Click */, OnClickOn11, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_12, -600 /* Click */, OnClickOn12, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_13, -600 /* Click */, OnClickOn13, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_14, -600 /* Click */, OnClickOn14, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_15, -600 /* Click */, OnClickOn15, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_16, -600 /* Click */, OnClickOn16, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, IDB_REPAIR_RUN_CONTINUE, -600 /* Click */, OnClickRepairRunContinue, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_17, -600 /* Click */, OnClickOn17, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_18, -600 /* Click */, OnClickOn18, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_19, -600 /* Click */, OnClickOn19, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_20, -600 /* Click */, OnClickOn20, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_21, -600 /* Click */, OnClickOn21, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_22, -600 /* Click */, OnClickOn22, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_23, -600 /* Click */, OnClickOn23, VTS_NONE)
	ON_EVENT(CAutoRepairTabSelectDlg, PDB_ON_24, -600 /* Click */, OnClickOn24, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

int CAutoRepairTabSelectDlg::getNewWorkSide(int iNextWorkSide)
{
	ASSERT(iNextWorkSide > WORKING_SIDE_UNKNOWN && iNextWorkSide < DEF_MAX_WORK_SIDE);

	BOOL rgbUseWorkSide[DEF_MAX_WORK_SIDE];
	rgbUseWorkSide[WORKING_SIDE_X_UP] = m_plnkPanelData->m_bUseFlag_Xu;
	rgbUseWorkSide[WORKING_SIDE_X_DOWN] = m_plnkPanelData->m_bUseFlag_Xd;
	rgbUseWorkSide[WORKING_SIDE_Y_LEFT] = m_plnkPanelData->m_bUseFlag_Yl;
	rgbUseWorkSide[WORKING_SIDE_Y_RIGHT] = m_plnkPanelData->m_bUseFlag_Yr;

	switch (MOlbSystemPre.GetSystemData()->m_eSystemType)
	{
	case SYSTEM_TYPE_SOURCE:
		rgbUseWorkSide[WORKING_SIDE_Y_LEFT] = FALSE;
		rgbUseWorkSide[WORKING_SIDE_Y_RIGHT] = FALSE;
		break;
	case SYSTEM_TYPE_GATE:
		rgbUseWorkSide[WORKING_SIDE_X_UP] = FALSE;
		rgbUseWorkSide[WORKING_SIDE_X_DOWN] = FALSE;
		break;
	}

	int iNewWorkSide = WORKING_SIDE_UNKNOWN;
	if (FALSE == rgbUseWorkSide[iNextWorkSide])
	{
		if (rgbUseWorkSide[WORKING_SIDE_X_UP])
			iNewWorkSide = WORKING_SIDE_X_UP;
		else if (rgbUseWorkSide[WORKING_SIDE_X_DOWN]) 
			iNewWorkSide = WORKING_SIDE_X_DOWN;
		else if (rgbUseWorkSide[WORKING_SIDE_Y_LEFT]) 
			iNewWorkSide = WORKING_SIDE_Y_LEFT;
		else if (rgbUseWorkSide[WORKING_SIDE_Y_RIGHT])
			iNewWorkSide = WORKING_SIDE_Y_RIGHT;
	}
	else
		iNewWorkSide = iNextWorkSide;

	return iNewWorkSide;
}

void CAutoRepairTabSelectDlg::OnClickTabNext() 
{
	int iNextWorkSide = m_iWorkSide + 1;
	iNextWorkSide %= DEF_MAX_WORK_SIDE;

	int iNewWorkSide = getNewWorkSide(iNextWorkSide);
	if (m_iWorkSide != iNewWorkSide)
	{
		m_iWorkSide = iNewWorkSide;
		
		switch (m_iWorkSide)
		{
		case WORKING_SIDE_X_UP:
			GetDlgItem(IDC_TAB_MODE)->SetWindowText(_T("Xu"));
			break;
			
		case WORKING_SIDE_X_DOWN:
			GetDlgItem(IDC_TAB_MODE)->SetWindowText(_T("Xd"));
			break;
			
		case WORKING_SIDE_Y_LEFT:
			GetDlgItem(IDC_TAB_MODE)->SetWindowText(_T("Yl"));
			break;
			
		case WORKING_SIDE_Y_RIGHT:
			GetDlgItem(IDC_TAB_MODE)->SetWindowText(_T("Yr"));
			break;
		}
		for (int i = 0; i < DEF_MAX_TAB; i++)
		{
			m_btnSelect[i].SetValue(FALSE);
			m_btnSelect[i].EnableWindow(FALSE);
			m_btnSelect[i].ShowWindow(SW_HIDE);
		}
	}

	int iMaxQty = m_plnkProcessData->GetMaxTabQty((EWorkingSide)m_iWorkSide);
	for (int i = 0; i < iMaxQty; i++)
	{
		m_btnSelect[i].ShowWindow(SW_SHOW);
		m_btnSelect[i].EnableWindow(TRUE);
		
//		m_btnSelect[i].SetValue(m_plnkProcessData->IsRepairUseTab((EWorkingSide)m_iWorkSide, i));
	}
}

void CAutoRepairTabSelectDlg::changeTabSelect(int iSel)
{
//	m_plnkProcessData->SetRepairUseTab((EWorkingSide)m_iWorkSide, iSel, !(m_plnkProcessData->IsRepairUseTab((EWorkingSide)m_iWorkSide, iSel)));
//	m_btnSelect[iSel].SetValue(m_plnkProcessData->IsRepairUseTab((EWorkingSide)m_iWorkSide, iSel));
}

void CAutoRepairTabSelectDlg::OnClickOn1() 
{
	changeTabSelect(0);
}

void CAutoRepairTabSelectDlg::OnClickOn2() 
{
	changeTabSelect(1);
}

void CAutoRepairTabSelectDlg::OnClickOn3() 
{
	changeTabSelect(2);
}

void CAutoRepairTabSelectDlg::OnClickOn4() 
{
	changeTabSelect(3);
}

void CAutoRepairTabSelectDlg::OnClickOn5() 
{
	changeTabSelect(4);
}

void CAutoRepairTabSelectDlg::OnClickOn6() 
{
	changeTabSelect(5);
}

void CAutoRepairTabSelectDlg::OnClickOn7() 
{
	changeTabSelect(6);
}

void CAutoRepairTabSelectDlg::OnClickOn8() 
{
	changeTabSelect(7);
}

void CAutoRepairTabSelectDlg::OnClickOn9() 
{
	changeTabSelect(8);
}

void CAutoRepairTabSelectDlg::OnClickOn10() 
{
	changeTabSelect(9);
}

void CAutoRepairTabSelectDlg::OnClickOn11() 
{
	changeTabSelect(10);
}

void CAutoRepairTabSelectDlg::OnClickOn12() 
{
	changeTabSelect(11);
}

void CAutoRepairTabSelectDlg::OnClickOn13() 
{
	changeTabSelect(12);
}

void CAutoRepairTabSelectDlg::OnClickOn14() 
{
	changeTabSelect(13);
}

void CAutoRepairTabSelectDlg::OnClickOn15() 
{
	changeTabSelect(14);
}

void CAutoRepairTabSelectDlg::OnClickOn16() 
{
	changeTabSelect(15);
}

void CAutoRepairTabSelectDlg::OnClickOn17() 
{
	// TODO: Add your control notification handler code here
	changeTabSelect(16);
}

void CAutoRepairTabSelectDlg::OnClickOn18() 
{
	// TODO: Add your control notification handler code here
	changeTabSelect(17);
}

void CAutoRepairTabSelectDlg::OnClickOn19() 
{
	// TODO: Add your control notification handler code here
	changeTabSelect(18);	
}

void CAutoRepairTabSelectDlg::OnClickOn20() 
{
	// TODO: Add your control notification handler code here
	changeTabSelect(19);
}

void CAutoRepairTabSelectDlg::OnClickOn21() 
{
	// TODO: Add your control notification handler code here
	changeTabSelect(20);
}

void CAutoRepairTabSelectDlg::OnClickOn22() 
{
	// TODO: Add your control notification handler code here
	changeTabSelect(21);
}

void CAutoRepairTabSelectDlg::OnClickOn23() 
{
	// TODO: Add your control notification handler code here
	changeTabSelect(22);
}

void CAutoRepairTabSelectDlg::OnClickOn24() 
{
	// TODO: Add your control notification handler code here
	changeTabSelect(23);
}

void CAutoRepairTabSelectDlg::Start()
{
//	m_plnkProcessData->ClearAllRepairFlag();

	GetDlgItem(IDC_TAB_MODE)->SetWindowText(_T("Panel Work Side"));
	
//다음 작업변
	SetTitleCB(&m_btnNext, MAIN_AUTO, _T("AutoRepairTabSelectDlg_1"));
//Repair 계속
	SetTitleCB(&m_btnContinue, MAIN_AUTO, _T("AutoRepairTabSelectDlg_2"));

	m_uiTimerID = SetTimer(10, 500, NULL);

	OnClickTabNext();
}

void CAutoRepairTabSelectDlg::Terminate()
{
	if (m_uiTimerID)
	{
		KillTimer(m_uiTimerID);
		m_uiTimerID = 0;
	}

	m_iWorkSide = WORKING_SIDE_UNKNOWN;
}

void CAutoRepairTabSelectDlg::OnClickRepairRunContinue() 
{
//	m_plnkProcessData->SetWorkStartAfterUnitInitialization();
	m_plnkITIPanelAligner->SetRepairRunContinue();
}

void CAutoRepairTabSelectDlg::OnTimer(UINT nIDEvent) 
{
	if (m_uiTimerID == nIDEvent)
	{		
		if (m_btnContinue.GetValue() != m_plnkITIPanelAligner->IsRepairRunContinue())
			m_btnContinue.SetValue(m_plnkITIPanelAligner->IsRepairRunContinue());
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CAutoRepairTabSelectDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);
}

BOOL CAutoRepairTabSelectDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
    {
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			(CMainFrame*)(AfxGetApp()->GetMainWnd())->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;

		default:
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CAutoRepairTabSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
//다음 작업변
	SetTitleCB(&m_btnNext, MAIN_AUTO, _T("AutoRepairTabSelectDlg_1"));
//Repair 계속
	SetTitleCB(&m_btnContinue, MAIN_AUTO, _T("AutoRepairTabSelectDlg_2"));

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
