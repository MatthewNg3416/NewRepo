// ManualTabMounterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "ManualTabMounterDlg.h"
#include "MPlatformOlbSystem.h"
#include "MSystemData.h"
#include "MTabMounter.h"
#include "MTickTimer.h"
#include "IVision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualTabMounterDlg dialog
extern	MPlatformOlbSystem	MOlbSystemPre; 

CManualTabMounterDlg::CManualTabMounterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualTabMounterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualTabMounterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_plnkTabMounter[i]= MOlbSystemPre.GetTabMounterComponent(i);
		m_plnkRTabMounter[i]= MOlbSystemPre.GetRTabMounterComponent(i);
	}
}


void CManualTabMounterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualTabMounterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	int i = 0;
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_ON1,	m_btnVacOn[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_ON2,	m_btnVacOn[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_ON3,	m_btnVacOn[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_ON4,	m_btnVacOn[i++]);
	i=0;
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_ON5,	m_btnRVacOn[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_ON6,	m_btnRVacOn[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_ON7,	m_btnRVacOn[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_ON8,	m_btnRVacOn[i++]);

	i = 0;
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_OFF1,	m_btnVacOff[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_OFF2,	m_btnVacOff[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_OFF3,	m_btnVacOff[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_OFF4,	m_btnVacOff[i++]);
	i = 0;
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_OFF5,	m_btnRVacOff[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_OFF6,	m_btnRVacOff[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_OFF7,	m_btnRVacOff[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_VAC_OFF8,	m_btnRVacOff[i++]);

	i = 0;
	DDX_Control(pDX, IDB_TABMOUNTER_UP1,		m_btnUp[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_UP2,		m_btnUp[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_UP3,		m_btnUp[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_UP4,		m_btnUp[i++]);
	i = 0;
	DDX_Control(pDX, IDB_TABMOUNTER_UP5,		m_btnRUp[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_UP6,		m_btnRUp[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_UP7,		m_btnRUp[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_UP8,		m_btnRUp[i++]);

	i = 0;
	DDX_Control(pDX, IDB_TABMOUNTER_DOWN1,		m_btnDown[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_DOWN2,		m_btnDown[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_DOWN3,		m_btnDown[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_DOWN4,		m_btnDown[i++]);
	i = 0;
	DDX_Control(pDX, IDB_TABMOUNTER_DOWN5,		m_btnRDown[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_DOWN6,		m_btnRDown[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_DOWN7,		m_btnRDown[i++]);
	DDX_Control(pDX, IDB_TABMOUNTER_DOWN8,		m_btnRDown[i++]);

	i = 0;
	DDX_Control(pDX, IDB_F_PUSHER_UP1,			m_btnUpPusher[i++]);
	DDX_Control(pDX, IDB_F_PUSHER_UP2,			m_btnUpPusher[i++]);
	DDX_Control(pDX, IDB_F_PUSHER_UP3,			m_btnUpPusher[i++]);
	DDX_Control(pDX, IDB_F_PUSHER_UP4,			m_btnUpPusher[i++]);
	i = 0;
	DDX_Control(pDX, IDB_R_PUSHER_UP1,			m_btnRUpPusher[i++]);
	DDX_Control(pDX, IDB_R_PUSHER_UP2,			m_btnRUpPusher[i++]);
	DDX_Control(pDX, IDB_R_PUSHER_UP3,			m_btnRUpPusher[i++]);
	DDX_Control(pDX, IDB_R_PUSHER_UP4,			m_btnRUpPusher[i++]);

	i = 0;
	DDX_Control(pDX, IDB_F_PUSHER_DOWN1,		m_btnDownPusher[i++]);
	DDX_Control(pDX, IDB_F_PUSHER_DOWN2,		m_btnDownPusher[i++]);
	DDX_Control(pDX, IDB_F_PUSHER_DOWN3,		m_btnDownPusher[i++]);
	DDX_Control(pDX, IDB_F_PUSHER_DOWN4,		m_btnDownPusher[i++]);
	i = 0;
	DDX_Control(pDX, IDB_R_PUSHER_DOWN1,		m_btnRDownPusher[i++]);
	DDX_Control(pDX, IDB_R_PUSHER_DOWN2,		m_btnRDownPusher[i++]);
	DDX_Control(pDX, IDB_R_PUSHER_DOWN3,		m_btnRDownPusher[i++]);
	DDX_Control(pDX, IDB_R_PUSHER_DOWN4,		m_btnRDownPusher[i++]);

	DDX_Control(pDX, IDB_FRONT_REPEAT_TEST,	m_btnFrontRepeatTest);
	DDX_Control(pDX, IDB_REAR_REPEAT_TEST,	m_btnRearRepeatTest);
}


BEGIN_MESSAGE_MAP(CManualTabMounterDlg, CDialog)
	//{{AFX_MSG_MAP(CManualTabMounterDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualTabMounterDlg message handlers

BEGIN_EVENTSINK_MAP(CManualTabMounterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualTabMounterDlg)
	ON_EVENT(CManualTabMounterDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_ON1, -600 /* Click */, OnTabMounterVacOn1, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_ON2, -600 /* Click */, OnTabMounterVacOn2, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_ON3, -600 /* Click */, OnTabMounterVacOn3, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_ON4, -600 /* Click */, OnTabMounterVacOn4, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_ON5, -600 /* Click */, OnTabMounterVacOn5, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_ON6, -600 /* Click */, OnTabMounterVacOn6, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_ON7, -600 /* Click */, OnTabMounterVacOn7, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_ON8, -600 /* Click */, OnTabMounterVacOn8, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_OFF1, -600 /* Click */, OnTabMounterVacOff1, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_OFF2, -600 /* Click */, OnTabMounterVacOff2, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_OFF3, -600 /* Click */, OnTabMounterVacOff3, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_OFF4, -600 /* Click */, OnTabMounterVacOff4, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_OFF5, -600 /* Click */, OnTabMounterVacOff5, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_OFF6, -600 /* Click */, OnTabMounterVacOff6, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_OFF7, -600 /* Click */, OnTabMounterVacOff7, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_VAC_OFF8, -600 /* Click */, OnTabMounterVacOff8, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_UP1, -600 /* Click */, OnTabMounterUp1, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_UP2, -600 /* Click */, OnTabMounterUp2, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_UP3, -600 /* Click */, OnTabMounterUp3, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_UP4, -600 /* Click */, OnTabMounterUp4, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_UP5, -600 /* Click */, OnTabMounterUp5, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_UP6, -600 /* Click */, OnTabMounterUp6, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_UP7, -600 /* Click */, OnTabMounterUp7, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_UP8, -600 /* Click */, OnTabMounterUp8, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_DOWN1, -600 /* Click */, OnTabMounterDown1, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_DOWN2, -600 /* Click */, OnTabMounterDown2, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_DOWN3, -600 /* Click */, OnTabMounterDown3, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_DOWN4, -600 /* Click */, OnTabMounterDown4, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_DOWN5, -600 /* Click */, OnTabMounterDown5, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_DOWN6, -600 /* Click */, OnTabMounterDown6, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_DOWN7, -600 /* Click */, OnTabMounterDown7, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_DOWN8, -600 /* Click */, OnTabMounterDown8, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_F_ALL_UP, -600 /* Click */, OnTabmounterFAllUp, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_F_ALL_DOWN, -600 /* Click */, OnTabmounterFAllDown, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_R_ALL_UP, -600 /* Click */, OnClickTabmounterRAllUp, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_TABMOUNTER_R_ALL_DOWN, -600 /* Click */, OnClickTabmounterRAllDown, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_PUSHER_F_ALL_UP, -600 /* Click */, OnClickPusherFAllUp, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_PUSHER_F_ALL_DOWN, -600 /* Click */, OnClickPusherFAllDown, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_F_PUSHER_UP1, -600 /* Click */, OnClickFPusherUp1, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_F_PUSHER_DOWN1, -600 /* Click */, OnClickFPusherDown1, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_F_PUSHER_UP2, -600 /* Click */, OnClickFPusherUp2, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_F_PUSHER_DOWN2, -600 /* Click */, OnClickFPusherDown2, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_F_PUSHER_UP3, -600 /* Click */, OnClickFPusherUp3, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_F_PUSHER_DOWN3, -600 /* Click */, OnClickFPusherDown3, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_F_PUSHER_UP4, -600 /* Click */, OnClickFPusherUp4, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_F_PUSHER_DOWN4, -600 /* Click */, OnClickFPusherDown4, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_PUSHER_R_ALL_UP, -600 /* Click */, OnClickPusherRAllUp, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_PUSHER_R_ALL_DOWN, -600 /* Click */, OnClickPusherRAllDown, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_R_PUSHER_UP1, -600 /* Click */, OnClickRPusherUp1, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_R_PUSHER_DOWN1, -600 /* Click */, OnClickRPusherDown1, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_R_PUSHER_UP2, -600 /* Click */, OnClickRPusherUp2, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_R_PUSHER_DOWN2, -600 /* Click */, OnClickRPusherDown2, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_R_PUSHER_UP3, -600 /* Click */, OnClickRPusherUp3, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_R_PUSHER_DOWN3, -600 /* Click */, OnClickRPusherDown3, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_R_PUSHER_UP4, -600 /* Click */, OnClickRPusherUp4, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_R_PUSHER_DOWN4, -600 /* Click */, OnClickRPusherDown4, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
//	ON_EVENT(CManualTabMounterDlg, IDC_MANUAL_TABCARRIER_UD, DISPID_CLICK, CManualTabMounterDlg::ClickManualTabcarrierUd, VTS_NONE)

	ON_EVENT(CManualTabMounterDlg, IDB_FRONT_REPEAT_TEST, DISPID_CLICK, CManualTabMounterDlg::ClickFrontRepeatTest, VTS_NONE)
	ON_EVENT(CManualTabMounterDlg, IDB_REAR_REPEAT_TEST, DISPID_CLICK, CManualTabMounterDlg::ClickRearRepeatTest, VTS_NONE)
END_EVENTSINK_MAP()

void CManualTabMounterDlg::OnClickExit() 
{
	CDialog::OnCancel();
}

void CManualTabMounterDlg::OnTabMounterVacOn1() 
{
	vacuumOn(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnTabMounterVacOn2() 
{
	vacuumOn(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnTabMounterVacOn3() 
{
	vacuumOn(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnTabMounterVacOn4() 
{
	vacuumOn(DEF_TABMOUNTER4);
}

void CManualTabMounterDlg::OnTabMounterVacOn5() 
{
	vacuumOnR(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnTabMounterVacOn6() 
{
	vacuumOnR(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnTabMounterVacOn7() 
{
	vacuumOnR(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnTabMounterVacOn8() 
{
	vacuumOnR(DEF_TABMOUNTER4);
}

void CManualTabMounterDlg::OnTabMounterVacOff1() 
{
	vacuumOff(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnTabMounterVacOff2() 
{
	vacuumOff(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnTabMounterVacOff3() 
{
	vacuumOff(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnTabMounterVacOff4() 
{
	vacuumOff(DEF_TABMOUNTER4);
}

void CManualTabMounterDlg::OnTabMounterVacOff5() 
{
	vacuumOffR(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnTabMounterVacOff6() 
{
	vacuumOffR(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnTabMounterVacOff7() 
{
	vacuumOffR(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnTabMounterVacOff8() 
{
	vacuumOffR(DEF_TABMOUNTER4);
}

void CManualTabMounterDlg::OnTabMounterUp1() 
{
	up(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnTabMounterUp2() 
{
	up(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnTabMounterUp3() 
{
	up(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnTabMounterUp4() 
{
	up(DEF_TABMOUNTER4);
}

void CManualTabMounterDlg::OnTabMounterUp5() 
{
	upR(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnTabMounterUp6() 
{
	upR(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnTabMounterUp7() 
{
	upR(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnTabMounterUp8() 
{
	upR(DEF_TABMOUNTER4);
}

void CManualTabMounterDlg::OnTabMounterDown1() 
{
	down(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnTabMounterDown2() 
{
	down(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnTabMounterDown3() 
{
	down(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnTabMounterDown4() 
{
	down(DEF_TABMOUNTER4);
}

void CManualTabMounterDlg::OnTabMounterDown5() 
{
	downR(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnTabMounterDown6() 
{
	downR(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnTabMounterDown7() 
{
	downR(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnTabMounterDown8() 
{
	downR(DEF_TABMOUNTER4);
}

void CManualTabMounterDlg::vacuumOn(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	pTabMounter = m_plnkTabMounter[iMounterNo];
#else
	if (iMounterNo < DEF_MAX_TABMOUNTER)
		pTabMounter = m_plnkTabMounter[iMounterNo];
	else
		pTabMounter = m_plnkRTabMounter[iMounterNo-DEF_MAX_TABMOUNTER];
#endif
	/*/
	pTabMounter = m_plnkTabMounter[iMounterNo];
	int iResult = pTabMounter->AbsorbTabIC();

	/*
	int iBtnNo = 0;
	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		if (iMounterNo < 6)
			iBtnNo = iMounterNo;
		else
			iBtnNo = iMounterNo + 2;
	}
	else
		iBtnNo = iMounterNo;
	*/
	int iBtnNo = iMounterNo;

	m_btnVacOn[iBtnNo].SetValue(pTabMounter->IsAbsorbTabIC());

	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("TabMounter"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::vacuumOnR(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	pTabMounter = m_plnkRTabMounter[iMounterNo];
	int iResult = pTabMounter->AbsorbTabIC();

	int iBtnNo = iMounterNo;

	m_btnRVacOn[iBtnNo].SetValue(pTabMounter->IsAbsorbTabIC());

	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::vacuumOff(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	pTabMounter = m_plnkTabMounter[iMounterNo];
#else
	if (iMounterNo < DEF_MAX_TABMOUNTER)
		pTabMounter = m_plnkTabMounter[iMounterNo];
	else
		pTabMounter = m_plnkRTabMounter[iMounterNo-DEF_MAX_TABMOUNTER];
#endif
	/*/
	pTabMounter = m_plnkTabMounter[iMounterNo];
	int iResult = pTabMounter->ReleaseTabIC();
	
	/*
	int iBtnNo = 0;
	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		if (iMounterNo < 6)
			iBtnNo = iMounterNo;
		else
			iBtnNo = iMounterNo + 2;
	}
	else
		iBtnNo = iMounterNo;
	*/
	int iBtnNo = iMounterNo;

	m_btnVacOff[iBtnNo].SetValue(pTabMounter->IsReleaseTabIC());

	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("TabMounter"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::vacuumOffR(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	pTabMounter = m_plnkRTabMounter[iMounterNo];
	int iResult = pTabMounter->ReleaseTabIC();
	
	int iBtnNo = iMounterNo;

	m_btnRVacOff[iBtnNo].SetValue(pTabMounter->IsReleaseTabIC());

	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::up(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	pTabMounter = m_plnkTabMounter[iMounterNo];
#else
	if (iMounterNo < DEF_MAX_TABMOUNTER)
		pTabMounter = m_plnkTabMounter[iMounterNo];
	else
		pTabMounter = m_plnkRTabMounter[iMounterNo-DEF_MAX_TABMOUNTER];
#endif
	/*/
	pTabMounter = m_plnkTabMounter[iMounterNo];
	int iResult = pTabMounter->UpCyl();

	/*
	int iBtnNo = 0;
	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		if (iMounterNo < 6)
			iBtnNo = iMounterNo;
		else
			iBtnNo = iMounterNo + 2;
	}
	else
		iBtnNo = iMounterNo;
	*/
	int iBtnNo = iMounterNo;


	m_btnUp[iBtnNo].SetValue(pTabMounter->IsUpCyl());

	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("TabMounter"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::upR(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	pTabMounter = m_plnkRTabMounter[iMounterNo];
	int iResult = pTabMounter->UpCyl();

	int iBtnNo = iMounterNo;

	m_btnRUp[iBtnNo].SetValue(pTabMounter->IsUpCyl());

	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::down(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	pTabMounter = m_plnkTabMounter[iMounterNo];
#else
	if (iMounterNo < DEF_MAX_TABMOUNTER)
		pTabMounter = m_plnkTabMounter[iMounterNo];
	else
		pTabMounter = m_plnkRTabMounter[iMounterNo-DEF_MAX_TABMOUNTER];
#endif
	/*/
	pTabMounter = m_plnkTabMounter[iMounterNo];
	int iResult = pTabMounter->DownCyl();

	/*
	int iBtnNo = 0;
	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		if (iMounterNo < 6)
			iBtnNo = iMounterNo;
		else
			iBtnNo = iMounterNo + 2;
	}
	else
		iBtnNo = iMounterNo;
	*/
	int iBtnNo = iMounterNo;


	m_btnDown[iBtnNo].SetValue(pTabMounter->IsDownCyl());

	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("TabMounter"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::downR(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	pTabMounter = m_plnkRTabMounter[iMounterNo];
	int iResult = pTabMounter->DownCyl();

	int iBtnNo = iMounterNo;

	m_btnRDown[iBtnNo].SetValue(pTabMounter->IsDownCyl());

	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::upPusher(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	pTabMounter = m_plnkTabMounter[iMounterNo];
	int iResult = pTabMounter->UpPusherCyl();

	int iBtnNo = iMounterNo;

	m_btnUpPusher[iBtnNo].SetValue(pTabMounter->IsUpPusherCyl());

	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::upRPusher(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	pTabMounter = m_plnkRTabMounter[iMounterNo];
	int iResult = pTabMounter->UpPusherCyl();

	int iBtnNo = iMounterNo;

	m_btnRUpPusher[iBtnNo].SetValue(pTabMounter->IsUpPusherCyl());

	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::downPusher(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	pTabMounter = m_plnkTabMounter[iMounterNo];
	int iResult = pTabMounter->DownPusherCyl();

	int iBtnNo = iMounterNo;

	m_btnDownPusher[iBtnNo].SetValue(pTabMounter->IsDownPusherCyl());

	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::downRPusher(int iMounterNo)
{
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	MTabMounter* pTabMounter = NULL;
	pTabMounter = m_plnkRTabMounter[iMounterNo];
	int iResult = pTabMounter->DownPusherCyl();

	int iBtnNo = iMounterNo;

	m_btnRDownPusher[iBtnNo].SetValue(pTabMounter->IsDownPusherCyl());

	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabMounterDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		// Vacuum On Button
		if (m_bVacOnSts[i] != m_plnkTabMounter[i]->IsAbsorbTabIC())
		{
			m_bVacOnSts[i] = m_plnkTabMounter[i]->IsAbsorbTabIC();
			m_btnVacOn[i].SetValue(m_bVacOnSts[i]);
		}
		// Vacuum Off Button
		if (m_bVacOffSts[i] != m_plnkTabMounter[i]->IsReleaseTabIC())
		{
			m_bVacOffSts[i] = m_plnkTabMounter[i]->IsReleaseTabIC();
			m_btnVacOff[i].SetValue(m_bVacOffSts[i]);
		}
		// Cylinder Up Button
		if (m_bUpSts[i] != m_plnkTabMounter[i]->IsUpCyl())
		{
			m_bUpSts[i] = m_plnkTabMounter[i]->IsUpCyl();
			m_btnUp[i].SetValue(m_bUpSts[i]);
		}
		// Cylinder Down Button
		if (m_bDownSts[i] != m_plnkTabMounter[i]->IsDownCyl())
		{
			m_bDownSts[i] = m_plnkTabMounter[i]->IsDownCyl();
			m_btnDown[i].SetValue(m_bDownSts[i]);
		}

		if (m_bUpPusherSts[i] != m_plnkTabMounter[i]->IsUpPusherCyl())
		{
			m_bUpPusherSts[i] = m_plnkTabMounter[i]->IsUpPusherCyl();
			m_btnUpPusher[i].SetValue(m_bUpPusherSts[i]);
		}

		if (m_bDownPusherSts[i] != m_plnkTabMounter[i]->IsDownPusherCyl())
		{
			m_bDownPusherSts[i] = m_plnkTabMounter[i]->IsDownPusherCyl();
			m_btnDownPusher[i].SetValue(m_bDownPusherSts[i]);
		}

		if (m_bRUpPusherSts[i] != m_plnkRTabMounter[i]->IsUpPusherCyl())
		{
			m_bRUpPusherSts[i] = m_plnkRTabMounter[i]->IsUpPusherCyl();
			m_btnRUpPusher[i].SetValue(m_bRUpPusherSts[i]);
		}

		if (m_bRDownPusherSts[i] != m_plnkRTabMounter[i]->IsDownPusherCyl())
		{
			m_bRDownPusherSts[i] = m_plnkRTabMounter[i]->IsDownPusherCyl();
			m_btnRDownPusher[i].SetValue(m_bRDownPusherSts[i]);
		}

		//@#ifdef DEF_GATE_SYSTEM
		// Vacuum On Button
		if (m_bRVacOnSts[i] != m_plnkRTabMounter[i]->IsAbsorbTabIC())
		{
			m_bRVacOnSts[i] = m_plnkRTabMounter[i]->IsAbsorbTabIC();
			m_btnRVacOn[i].SetValue(m_bRVacOnSts[i]);
		}
		// Vacuum Off Button
		if (m_bRVacOffSts[i] != m_plnkRTabMounter[i]->IsReleaseTabIC())
		{
			m_bRVacOffSts[i] = m_plnkRTabMounter[i]->IsReleaseTabIC();
			m_btnRVacOff[i].SetValue(m_bRVacOffSts[i]);
		}
		// Cylinder Up Button
		if (m_bRUpSts[i] != m_plnkRTabMounter[i]->IsUpCyl())
		{
			m_bRUpSts[i] = m_plnkRTabMounter[i]->IsUpCyl();
			m_btnRUp[i].SetValue(m_bRUpSts[i]);
		}
		// Cylinder Down Button
		if (m_bRDownSts[i] != m_plnkRTabMounter[i]->IsDownCyl())
		{
			m_bRDownSts[i] = m_plnkRTabMounter[i]->IsDownCyl();
			m_btnRDown[i].SetValue(m_bRDownSts[i]);
		}
//@#endif
	}

//	MTabMounter* plnkRefTabMounter = m_plnkTabMounter[DEF_TABMOUNTER1];
//	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
//	{
//		if (i == DEF_REAR_GROUP)
//		{
//			plnkRefTabMounter = m_plnkTabMounter[DEF_TABMOUNTER7];
//		}
//	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CManualTabMounterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int iHalfCnt = DEF_MANUAL_TABMOUNTER_MAX_BUTTON/2;
	

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_bVacOnSts[i] = m_plnkTabMounter[i]->IsAbsorbTabIC();
		m_bVacOffSts[i] = !m_plnkTabMounter[i]->IsAbsorbTabIC();
		m_bUpSts[i] = m_plnkTabMounter[i]->IsUpCyl();
		m_bDownSts[i] = m_plnkTabMounter[i]->IsDownCyl();
		m_bUpPusherSts[i] = m_plnkTabMounter[i]->IsUpPusherCyl();
		m_bDownPusherSts[i] = m_plnkTabMounter[i]->IsDownPusherCyl();

		m_btnVacOn[i].SetValue(m_bVacOnSts[i]);
		m_btnVacOff[i].SetValue(m_bVacOffSts[i]);
		m_btnUp[i].SetValue(m_bUpSts[i]);
		m_btnDown[i].SetValue(m_bDownSts[i]);
		m_btnUpPusher[i].SetValue(m_bUpPusherSts[i]);
		m_btnDownPusher[i].SetValue(m_bDownPusherSts[i]);

//@#ifdef DEF_GATE_SYSTEM
		m_bRVacOnSts[i] = m_plnkRTabMounter[i]->IsAbsorbTabIC();
		m_bRVacOffSts[i] = !m_plnkRTabMounter[i]->IsAbsorbTabIC();
		m_bRUpSts[i] = m_plnkRTabMounter[i]->IsUpCyl();
		m_bRDownSts[i] = m_plnkRTabMounter[i]->IsDownCyl();
		m_bRUpPusherSts[i] = m_plnkRTabMounter[i]->IsUpPusherCyl();
		m_bRDownPusherSts[i] = m_plnkRTabMounter[i]->IsDownPusherCyl();

		m_btnRVacOn[i].SetValue(m_bRVacOnSts[i]);
		m_btnRVacOff[i].SetValue(m_bRVacOffSts[i]);
		m_btnRUp[i].SetValue(m_bRUpSts[i]);
		m_btnRDown[i].SetValue(m_bRDownSts[i]);
		m_btnRUpPusher[i].SetValue(m_bRUpPusherSts[i]);
		m_btnRDownPusher[i].SetValue(m_bRDownPusherSts[i]);
//@#endif
	}

	for (int i=DEF_MAX_TABMOUNTER; i<DEF_MANUAL_TABMOUNTER_MAX_BUTTON; i++)
	{
		m_btnUp[i].EnableWindow(FALSE);
		m_btnDown[i].EnableWindow(FALSE);
		m_btnRUp[i].EnableWindow(FALSE);
		m_btnRDown[i].EnableWindow(FALSE);
		m_btnUpPusher[i].EnableWindow(FALSE);
		m_btnDownPusher[i].EnableWindow(FALSE);
		m_btnRUpPusher[i].EnableWindow(FALSE);
		m_btnRDownPusher[i].EnableWindow(FALSE);

		m_btnVacOn[i].EnableWindow(FALSE);
		m_btnVacOff[i].EnableWindow(FALSE);
		m_btnRVacOn[i].EnableWindow(FALSE);
		m_btnRVacOff[i].EnableWindow(FALSE);
	}

//@#ifdef DEF_GATE_SYSTEM
	CString strCaption;
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		strCaption.Format(_T("RTabMounter %d\r\nUp"), i+1);
		m_btnRUp[i].SetCaption(strCaption);
		strCaption.Format(_T("RTabMounter %d\r\nDown"), i+1);
		m_btnRDown[i].SetCaption(strCaption);
		strCaption.Format(_T("RTabMounter %d\r\nOn"), i+1);
		m_btnRVacOn[i].SetCaption(strCaption);
		strCaption.Format(_T("RTabMounter %d\r\nOff"), i+1);
		m_btnRVacOff[i].SetCaption(strCaption);
	}
//@#endif

	SetTimer(1, 200, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;

	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabMounterDlg_1"), &szTextTemp);
//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabMounterDlg_2"), &szTextTemp);
//	Cylinder (앞)
//	GetDlgItem(IDC_MANUAL_TABCARRIER_UD)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabMounterDlg_3"), &szTextTemp);
//	Cylinder (뒤)
//	GetDlgItem(IDC_REAR_TITLE)->SetWindowText(szTextTemp);

//	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabMounterDlg_4"), &szTextTemp);
//	Vacuum (앞)
//	GetDlgItem(IDC_MANUAL_TABCARERIER_VAC_ON)->SetWindowText(szTextTemp);
	
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabMounterDlg_5"), &szTextTemp);
//	Vacuum (뒤)
//	GetDlgItem(IDC_REAR_TITLE1)->SetWindowText(szTextTemp);

	//	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabMounterDlg_6"), &szTextTemp);
//	Backup (앞)	
//	GetDlgItem(IDC_TITLE)->SetWindowText(szTextTemp);

	//	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabMounterDlg_7"), &szTextTemp);
//	Backup (뒤)
//	GetDlgItem(IDC_REAR_TITLE2)->SetWindowText(szTextTemp);

	GetDlgItem(IDB_TABMOUNTER_R_ALL_UP)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_R_ALL_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_UP5)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_DOWN5)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_UP6)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_DOWN6)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_UP7)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_DOWN7)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_UP8)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_DOWN8)->EnableWindow(FALSE);

	GetDlgItem(IDB_TABMOUNTER_VAC_ON5)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_VAC_OFF5)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_VAC_ON6)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_VAC_OFF6)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_VAC_ON7)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_VAC_OFF7)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_VAC_ON8)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER_VAC_OFF8)->EnableWindow(FALSE);

	GetDlgItem(IDB_PUSHER_R_ALL_UP)->EnableWindow(FALSE);
	GetDlgItem(IDB_PUSHER_R_ALL_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDB_R_PUSHER_UP1)->EnableWindow(FALSE);
	GetDlgItem(IDB_R_PUSHER_DOWN1)->EnableWindow(FALSE);
	GetDlgItem(IDB_R_PUSHER_UP2)->EnableWindow(FALSE);
	GetDlgItem(IDB_R_PUSHER_DOWN2)->EnableWindow(FALSE);
	GetDlgItem(IDB_R_PUSHER_UP3)->EnableWindow(FALSE);
	GetDlgItem(IDB_R_PUSHER_DOWN3)->EnableWindow(FALSE);
	GetDlgItem(IDB_R_PUSHER_UP4)->EnableWindow(FALSE);
	GetDlgItem(IDB_R_PUSHER_DOWN4)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManualTabMounterDlg::OnTabmounterFAllUp() 
{
	int iCnt = DEF_MAX_TABMOUNTER;
//@#ifdef DEF_SOURCE_SYSTEM
//@	iCnt /= 2.0;
//@#endif

	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, iCnt + 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < iCnt; i++)
		m_plnkTabMounter[i]->UpCyl(TRUE);

	// 2009.03.10 CYJ
//////////////////////////////////////////////////////////////////////
	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	double UpElapseTime[DEF_MAX_TABMOUNTER];
	CString strLog;
	int ElapseTimeCheck[DEF_MAX_TABMOUNTER];
	for (int j=0; j < iCnt; j++)
	{
		UpElapseTime[j] = 0.0;
		ElapseTimeCheck[j] = FALSE;
	}

	int k;
	while(timerPressCylinder.LessThan(2.0))
	{
		for(k=0; k < iCnt; k++)
		{
			if((m_plnkTabMounter[k]->IsUpCyl())&&(FALSE == ElapseTimeCheck[k]))
			{
				UpElapseTime[k] = timerPressCylinder.GetElapseTime();
				strLog.Format(_T("Front TabMounter %d Up Manual Time :,%.2f"), k, UpElapseTime[k]);
				MOlbSystemPre.GetVisionComponent()->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);
				ElapseTimeCheck[k] = TRUE;
			}
		}
		Sleep(10);
	}

	timerPressCylinder.StopTimer();

////////////////////////////////////////////////////////////////////////

	Sleep(10);
	ProgWnd.StepIt();

	for (int i = 0; i < iCnt; i++)
	{
		int iResult = m_plnkTabMounter[i]->UpCyl();
		if (iResult)
		{
			ProgWnd.DestroyWindow();

			// jdy sesl			MyMessageBox(iResult,_T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}
	ProgWnd.DestroyWindow();

	strLog.Format(_T("Front Up Manual Time :,%.2f,%.2f,%.2f,%.2f"), 
		UpElapseTime[0],UpElapseTime[1],UpElapseTime[2],UpElapseTime[3]);

	MyMessageBox(strLog.GetBuffer(strLog.GetLength()), _T("Info"), M_ICONINFORMATION);
}

void CManualTabMounterDlg::OnTabmounterFAllDown() 
{
	int iCnt = DEF_MAX_TABMOUNTER;
//@#ifdef DEF_SOURCE_SYSTEM
//@	iCnt /= 2.0;
//@#endif

	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, iCnt + 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < iCnt; i++)
		m_plnkTabMounter[i]->DownCyl(TRUE);

	// 2009.03.10 CYJ
//////////////////////////////////////////////////////////////////////
	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	double Down2ndElapseTime[DEF_MAX_TABMOUNTER];
	CString strLog;
	BOOL ElapseTimeCheck[DEF_MAX_TABMOUNTER];
	for (int j=0; j < iCnt; j++)
	{
		Down2ndElapseTime[j] = 0.0;
		ElapseTimeCheck[j] = FALSE;
	}

	while(timerPressCylinder.LessThan(2.0))
	{
		for (int j=0; j < iCnt; j++)
		{
			if((m_plnkTabMounter[j]->IsDownCyl())&&(FALSE == ElapseTimeCheck[j]))
			{
				Down2ndElapseTime[j] = timerPressCylinder.GetElapseTime();
				strLog.Format(_T("Front TabMounter %d 2nd Down Manual Time :,%.2f"), j, Down2ndElapseTime[j]);
				MOlbSystemPre.GetVisionComponent()->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);
				ElapseTimeCheck[j] = TRUE;
			}
		}
		Sleep(10);
	}

	timerPressCylinder.StopTimer();
////////////////////////////////////////////////////////////////////////

	Sleep(10);
	ProgWnd.StepIt();

	for (int i = 0; i < iCnt; i++)
	{
		int iResult = m_plnkTabMounter[i]->DownCyl();
		if (iResult)
		{
			ProgWnd.DestroyWindow();

// jdy sesl			MyMessageBox(iResult,_T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}
	ProgWnd.DestroyWindow();

	strLog.Format(_T("Front 2nd Down Manual Time :,%.2f,%.2f,%.2f,%.2f"), 
		Down2ndElapseTime[0],Down2ndElapseTime[1],Down2ndElapseTime[2],Down2ndElapseTime[3]);
	MyMessageBox(strLog.GetBuffer(strLog.GetLength()), _T("Info"), M_ICONINFORMATION);
}

void CManualTabMounterDlg::OnClickTabmounterRAllUp() 
{
	// TODO: Add your control notification handler code here
	int iCnt = DEF_MAX_TABMOUNTER;
//@#ifdef DEF_SOURCE_SYSTEM
//@	iCnt /= 2.0;
//@#endif

	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, iCnt + 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < iCnt; i++)
	{
//@#ifdef DEF_SOURCE_SYSTEM
//@		m_plnkTabMounter[i+iCnt]->UpCyl(TRUE);
//@#else
		m_plnkRTabMounter[i]->UpCyl(TRUE);
//@#endif
	}

	// 2009.03.10 CYJ
//////////////////////////////////////////////////////////////////////
	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	double UpElapseTime[DEF_MAX_TABMOUNTER];
	CString strLog;
	int ElapseTimeCheck[DEF_MAX_TABMOUNTER];
	for (int j=0; j < iCnt; j++)
	{
		UpElapseTime[j] = 0.0;
		ElapseTimeCheck[j] = FALSE;
	}

	int k;
	while(timerPressCylinder.LessThan(2.0))
	{
		for(k=0; k < iCnt; k++)
		{
//@#ifdef DEF_SOURCE_SYSTEM
//@			if((m_plnkTabMounter[k+iCnt]->IsUpCyl())&&(FALSE == ElapseTimeCheck[k]))
//@#else
			if((m_plnkRTabMounter[k]->IsUpCyl())&&(FALSE == ElapseTimeCheck[k]))
//@#endif
			{
				UpElapseTime[k] = timerPressCylinder.GetElapseTime();
				strLog.Format(_T("Front TabMounter %d Up Manual Time :,%.2f"), k, UpElapseTime[k]);
				MOlbSystemPre.GetVisionComponent()->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);
				ElapseTimeCheck[k] = TRUE;
			}
		}
		Sleep(10);
	}

	timerPressCylinder.StopTimer();

////////////////////////////////////////////////////////////////////////

	Sleep(10);
	ProgWnd.StepIt();

	for (int i = 0; i < iCnt; i++)
	{
//@#ifdef DEF_SOURCE_SYSTEM
//@		int iResult = m_plnkTabMounter[i+iCnt]->UpCyl();
//@#else
		int iResult = m_plnkRTabMounter[i]->UpCyl();
//@#endif
		if (iResult)
		{
			ProgWnd.DestroyWindow();

			// jdy sesl			MyMessageBox(iResult,_T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}
	ProgWnd.DestroyWindow();

	strLog.Format(_T("Rear Up Manual Time :,%.2f,%.2f,%.2f,%.2f"), 
		UpElapseTime[0],UpElapseTime[1],UpElapseTime[2],UpElapseTime[3]);

	MyMessageBox(strLog.GetBuffer(strLog.GetLength()), _T("Info"), M_ICONINFORMATION);
	return;	
}

void CManualTabMounterDlg::OnClickTabmounterRAllDown() 
{
	// TODO: Add your control notification handler code here
	int iCnt = DEF_MAX_TABMOUNTER;
//@#ifdef DEF_SOURCE_SYSTEM
//@	iCnt /= 2.0;
//@#endif

	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, iCnt + 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < iCnt; i++)
	{
//@#ifdef DEF_SOURCE_SYSTEM
//@		m_plnkTabMounter[i+iCnt]->DownCyl(TRUE);
//@#else
		m_plnkRTabMounter[i]->DownCyl(TRUE);
//@#endif
	}

	// 2009.03.10 CYJ
//////////////////////////////////////////////////////////////////////
	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	double Down2ndElapseTime[DEF_MAX_TABMOUNTER];
	CString strLog;
	BOOL ElapseTimeCheck[DEF_MAX_TABMOUNTER];
	for (int j=0; j < iCnt; j++)
	{
		Down2ndElapseTime[j] = 0.0;
		ElapseTimeCheck[j] = FALSE;
	}

	while(timerPressCylinder.LessThan(2.0))
	{
		for (int j=0; j < iCnt; j++)
		{
//@#ifdef DEF_SOURCE_SYSTEM
//@			if((m_plnkTabMounter[j+iCnt]->IsDownCyl())&&(FALSE == ElapseTimeCheck[j]))
//@#else
			if((m_plnkRTabMounter[j]->IsDownCyl())&&(FALSE == ElapseTimeCheck[j]))
//@#endif
			{
				Down2ndElapseTime[j] = timerPressCylinder.GetElapseTime();
				strLog.Format(_T("Front TabMounter %d 2nd Down Manual Time :,%.2f"), j, Down2ndElapseTime[j]);
				MOlbSystemPre.GetVisionComponent()->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);
				ElapseTimeCheck[j] = TRUE;
			}
		}
		Sleep(10);
	}

	timerPressCylinder.StopTimer();
////////////////////////////////////////////////////////////////////////

	Sleep(10);
	ProgWnd.StepIt();

	for (int i = 0; i < iCnt; i++)
	{
//@#ifdef DEF_SOURCE_SYSTEM
//@		int iResult = m_plnkTabMounter[i+iCnt]->DownCyl();
//@#else
		int iResult = m_plnkRTabMounter[i]->DownCyl();
//@#endif
		if (iResult)
		{
			ProgWnd.DestroyWindow();

// jdy sesl			MyMessageBox(iResult,_T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}
	ProgWnd.DestroyWindow();

	strLog.Format(_T("Front 2nd Down Manual Time :,%.2f,%.2f,%.2f,%.2f"), 
		Down2ndElapseTime[0],Down2ndElapseTime[1],Down2ndElapseTime[2],Down2ndElapseTime[3]);
	MyMessageBox(strLog.GetBuffer(strLog.GetLength()), _T("Info"), M_ICONINFORMATION);
	return;	
}

void CManualTabMounterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CManualTabMounterDlg::ClickFrontRepeatTest()
{
#if FALSE
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WCHAR cAfterVal[100];
	double dNewRepeatTestNumber;
	int i,j,k,l;

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10))
		return;

	/** 목표위치 설정하기 */
	dNewRepeatTestNumber = wcstod(cAfterVal,NULL);

	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i=0;i<dNewRepeatTestNumber;i++)
	{
		for (int j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
			m_plnkTabMounter[j]->DownReady(TRUE);

		Sleep(10);

		for (int j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
		{
			int iResult = m_plnkTabMounter[j]->DownReady();
			if (iResult)
			{
				ProgWnd.DestroyWindow();
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}

		Sleep(10);
		ProgWnd.StepIt();

		for (k = 0; k < DEF_MAX_WORKER_PER_GROUP; k++)
			m_plnkTabMounter[k]->DownPress(TRUE);

		Sleep(10);

		for (k = 0; k < DEF_MAX_WORKER_PER_GROUP; k++)
		{
			int iResult = m_plnkTabMounter[k]->DownPress();
			if (iResult)
			{
				ProgWnd.DestroyWindow();
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}

		Sleep(10);
		ProgWnd.StepIt();

		for (l = 0; l < DEF_MAX_WORKER_PER_GROUP; l++)
			m_plnkTabMounter[l]->Up(TRUE);

		Sleep(10);

		for (l = 0; l < DEF_MAX_WORKER_PER_GROUP; l++)
		{
			int iResult = m_plnkTabMounter[l]->Up();
			if (iResult)
			{
				ProgWnd.DestroyWindow();
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}

		Sleep(10);
		ProgWnd.StepIt();
	}

	ProgWnd.DestroyWindow();
#endif
}

void CManualTabMounterDlg::ClickRearRepeatTest()
{
#if FALSE
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	WCHAR cAfterVal[100];
	double dNewRepeatTestNumber;
	int i,j,k,l;

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10))
		return;

	/** 목표위치 설정하기 */
	dNewRepeatTestNumber = wcstod(cAfterVal,NULL);

	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i=0;i<dNewRepeatTestNumber;i++)
	{
		for (int j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
			m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP + j]->DownReady(TRUE);

		Sleep(10);

		for (int j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
		{
			int iResult = m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP + j]->DownReady();
			if (iResult)
			{
				ProgWnd.DestroyWindow();
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}

		Sleep(10);
		ProgWnd.StepIt();

		for (k = 0; k < DEF_MAX_WORKER_PER_GROUP; k++)
			m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP + k]->DownPress(TRUE);

		Sleep(10);

		for (k = 0; k < DEF_MAX_WORKER_PER_GROUP; k++)
		{
			int iResult = m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP + k]->DownPress();
			if (iResult)
			{
				ProgWnd.DestroyWindow();
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}

		Sleep(10);
		ProgWnd.StepIt();

		for (l = 0; l < DEF_MAX_WORKER_PER_GROUP; l++)
			m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP + l]->Up(TRUE);

		Sleep(10);

		for (l = 0; l < DEF_MAX_WORKER_PER_GROUP; l++)
		{
			int iResult = m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP + l]->Up();
			if (iResult)
			{
				ProgWnd.DestroyWindow();
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}

		Sleep(10);
		ProgWnd.StepIt();
	}

	ProgWnd.DestroyWindow();
#endif
}

void CManualTabMounterDlg::OnClickPusherFAllUp()
{
	// TODO: Add your control notification handler code here
	int iCnt = DEF_MAX_TABMOUNTER;
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, iCnt + 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < iCnt; i++)
		m_plnkTabMounter[i]->UpPusherCyl(TRUE);

	// 2009.03.10 CYJ
//////////////////////////////////////////////////////////////////////
	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	double UpElapseTime[DEF_MAX_TABMOUNTER];
	CString strLog;
	int ElapseTimeCheck[DEF_MAX_TABMOUNTER];
	for (int j=0; j < iCnt; j++)
	{
		UpElapseTime[j] = 0.0;
		ElapseTimeCheck[j] = FALSE;
	}

	int k;
	while(timerPressCylinder.LessThan(2.0))
	{
		for(k=0; k < iCnt; k++)
		{
			if((m_plnkTabMounter[k]->IsUpPusherCyl()) && (FALSE == ElapseTimeCheck[k]))
			{
				UpElapseTime[k] = timerPressCylinder.GetElapseTime();
				strLog.Format(_T("Front TabMounter %d UpPusher Manual Time :,%.2f"), k, UpElapseTime[k]);
				MOlbSystemPre.GetVisionComponent()->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);
				ElapseTimeCheck[k] = TRUE;
			}
		}
		Sleep(10);
	}

	timerPressCylinder.StopTimer();

////////////////////////////////////////////////////////////////////////

	Sleep(10);
	ProgWnd.StepIt();

	for (int i = 0; i < iCnt; i++)
	{
		int iResult = m_plnkTabMounter[i]->UpPusherCyl();
		if (iResult)
		{
			ProgWnd.DestroyWindow();

			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}
	ProgWnd.DestroyWindow();

	strLog.Format(_T("Front Up Manual Time :,%.2f,%.2f,%.2f,%.2f"), 
		UpElapseTime[0],UpElapseTime[1],UpElapseTime[2],UpElapseTime[3]);

	MyMessageBox(strLog.GetBuffer(strLog.GetLength()), _T("Info"), M_ICONINFORMATION);	
}

void CManualTabMounterDlg::OnClickPusherFAllDown()
{
	// TODO: Add your control notification handler code here
	int iCnt = DEF_MAX_TABMOUNTER;
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, iCnt + 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < iCnt; i++)
		m_plnkTabMounter[i]->DownPusherCyl(TRUE);

	// 2009.03.10 CYJ
//////////////////////////////////////////////////////////////////////
	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	double Down2ndElapseTime[DEF_MAX_TABMOUNTER];
	CString strLog;
	BOOL ElapseTimeCheck[DEF_MAX_TABMOUNTER];
	for (int j=0; j < iCnt; j++)
	{
		Down2ndElapseTime[j] = 0.0;
		ElapseTimeCheck[j] = FALSE;
	}

	while(timerPressCylinder.LessThan(2.0))
	{
		for (int j=0; j < iCnt; j++)
		{
			if((m_plnkTabMounter[j]->IsDownPusherCyl())&&(FALSE == ElapseTimeCheck[j]))
			{
				Down2ndElapseTime[j] = timerPressCylinder.GetElapseTime();
				strLog.Format(_T("Front TabMounter %d 2nd Down Manual Time :,%.2f"), j, Down2ndElapseTime[j]);
				MOlbSystemPre.GetVisionComponent()->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);
				ElapseTimeCheck[j] = TRUE;
			}
		}
		Sleep(10);
	}

	timerPressCylinder.StopTimer();
////////////////////////////////////////////////////////////////////////

	Sleep(10);
	ProgWnd.StepIt();

	for (int i = 0; i < iCnt; i++)
	{
		int iResult = m_plnkTabMounter[i]->DownPusherCyl();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}
	ProgWnd.DestroyWindow();

	strLog.Format(_T("Front 2nd Down Manual Time :,%.2f,%.2f,%.2f,%.2f"), 
		Down2ndElapseTime[0],Down2ndElapseTime[1],Down2ndElapseTime[2],Down2ndElapseTime[3]);
	MyMessageBox(strLog.GetBuffer(strLog.GetLength()), _T("Info"), M_ICONINFORMATION);
}

void CManualTabMounterDlg::OnClickFPusherUp1()
{
	// TODO: Add your control notification handler code here
	upPusher(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnClickFPusherDown1()
{
	// TODO: Add your control notification handler code here
	downPusher(DEF_TABMOUNTER1);	
}

void CManualTabMounterDlg::OnClickFPusherUp2() 
{
	// TODO: Add your control notification handler code here
	upPusher(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnClickFPusherDown2() 
{
	// TODO: Add your control notification handler code here
	downPusher(DEF_TABMOUNTER2);
}

void CManualTabMounterDlg::OnClickFPusherUp3() 
{
	// TODO: Add your control notification handler code here
	upPusher(DEF_TABMOUNTER3);	
}

void CManualTabMounterDlg::OnClickFPusherDown3() 
{
	// TODO: Add your control notification handler code here
	downPusher(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnClickFPusherUp4() 
{
	// TODO: Add your control notification handler code here
	upPusher(DEF_TABMOUNTER4);	
}

void CManualTabMounterDlg::OnClickFPusherDown4() 
{
	// TODO: Add your control notification handler code here
	downPusher(DEF_TABMOUNTER4);	
}

void CManualTabMounterDlg::OnClickPusherRAllUp() 
{
	// TODO: Add your control notification handler code here
	int iCnt = DEF_MAX_TABMOUNTER;
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, iCnt + 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < iCnt; i++)
		m_plnkRTabMounter[i]->UpPusherCyl(TRUE);

	// 2009.03.10 CYJ
//////////////////////////////////////////////////////////////////////
	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	double UpElapseTime[DEF_MAX_TABMOUNTER];
	CString strLog;
	int ElapseTimeCheck[DEF_MAX_TABMOUNTER];
	for (int j=0; j < iCnt; j++)
	{
		UpElapseTime[j] = 0.0;
		ElapseTimeCheck[j] = FALSE;
	}

	int k;
	while(timerPressCylinder.LessThan(2.0))
	{
		for(k=0; k < iCnt; k++)
		{
			if((m_plnkRTabMounter[k]->IsUpPusherCyl()) && (FALSE == ElapseTimeCheck[k]))
			{
				UpElapseTime[k] = timerPressCylinder.GetElapseTime();
				strLog.Format(_T("Front TabMounter %d UpPusher Manual Time :,%.2f"), k, UpElapseTime[k]);
				MOlbSystemPre.GetVisionComponent()->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);
				ElapseTimeCheck[k] = TRUE;
			}
		}
		Sleep(10);
	}

	timerPressCylinder.StopTimer();

////////////////////////////////////////////////////////////////////////

	Sleep(10);
	ProgWnd.StepIt();

	for (int i = 0; i < iCnt; i++)
	{
		int iResult = m_plnkRTabMounter[i]->UpPusherCyl();
		if (iResult)
		{
			ProgWnd.DestroyWindow();

			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}
	ProgWnd.DestroyWindow();

	strLog.Format(_T("Front Up Manual Time :,%.2f,%.2f,%.2f,%.2f"), 
		UpElapseTime[0],UpElapseTime[1],UpElapseTime[2],UpElapseTime[3]);

	MyMessageBox(strLog.GetBuffer(strLog.GetLength()), _T("Info"), M_ICONINFORMATION);	
}

void CManualTabMounterDlg::OnClickPusherRAllDown() 
{
	// TODO: Add your control notification handler code here
	int iCnt = DEF_MAX_TABMOUNTER;
	CMyProgressWnd ProgWnd(NULL, _T("Processing"));
	ProgWnd.SetRange(0, iCnt + 1);
	ProgWnd.SetText(_T("TabMounter is Processing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < iCnt; i++)
		m_plnkRTabMounter[i]->DownPusherCyl(TRUE);

	// 2009.03.10 CYJ
//////////////////////////////////////////////////////////////////////
	MTickTimer timerPressCylinder;
	timerPressCylinder.StartTimer();

	double Down2ndElapseTime[DEF_MAX_TABMOUNTER];
	CString strLog;
	BOOL ElapseTimeCheck[DEF_MAX_TABMOUNTER];
	for (int j=0; j < iCnt; j++)
	{
		Down2ndElapseTime[j] = 0.0;
		ElapseTimeCheck[j] = FALSE;
	}

	while(timerPressCylinder.LessThan(2.0))
	{
		for (int j=0; j < iCnt; j++)
		{
			if((m_plnkRTabMounter[j]->IsDownPusherCyl())&&(FALSE == ElapseTimeCheck[j]))
			{
				Down2ndElapseTime[j] = timerPressCylinder.GetElapseTime();
				strLog.Format(_T("Front TabMounter %d 2nd Down Manual Time :,%.2f"), j, Down2ndElapseTime[j]);
				MOlbSystemPre.GetVisionComponent()->WriteLog(_T("..\\Log\\TabMounter DownPress Time_log.csv"), (LPCTSTR)strLog);
				ElapseTimeCheck[j] = TRUE;
			}
		}
		Sleep(10);
	}

	timerPressCylinder.StopTimer();
////////////////////////////////////////////////////////////////////////

	Sleep(10);
	ProgWnd.StepIt();

	for (int i = 0; i < iCnt; i++)
	{
		int iResult = m_plnkRTabMounter[i]->DownPusherCyl();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}
	ProgWnd.DestroyWindow();

	strLog.Format(_T("Front 2nd Down Manual Time :,%.2f,%.2f,%.2f,%.2f"), 
		Down2ndElapseTime[0],Down2ndElapseTime[1],Down2ndElapseTime[2],Down2ndElapseTime[3]);
	MyMessageBox(strLog.GetBuffer(strLog.GetLength()), _T("Info"), M_ICONINFORMATION);
}

void CManualTabMounterDlg::OnClickRPusherUp1() 
{
	// TODO: Add your control notification handler code here
	upRPusher(DEF_TABMOUNTER1);
}

void CManualTabMounterDlg::OnClickRPusherDown1() 
{
	// TODO: Add your control notification handler code here
	downRPusher(DEF_TABMOUNTER1);	
}

void CManualTabMounterDlg::OnClickRPusherUp2() 
{
	// TODO: Add your control notification handler code here
	upRPusher(DEF_TABMOUNTER2);	
}

void CManualTabMounterDlg::OnClickRPusherDown2() 
{
	// TODO: Add your control notification handler code here
	downRPusher(DEF_TABMOUNTER2);	
}

void CManualTabMounterDlg::OnClickRPusherUp3() 
{
	// TODO: Add your control notification handler code here
	upRPusher(DEF_TABMOUNTER3);	
}

void CManualTabMounterDlg::OnClickRPusherDown3() 
{
	// TODO: Add your control notification handler code here
	downRPusher(DEF_TABMOUNTER3);
}

void CManualTabMounterDlg::OnClickRPusherUp4() 
{
	// TODO: Add your control notification handler code here
	upRPusher(DEF_TABMOUNTER4);	
}

void CManualTabMounterDlg::OnClickRPusherDown4()
{
	// TODO: Add your control notification handler code here
	downRPusher(DEF_TABMOUNTER4);	
}
