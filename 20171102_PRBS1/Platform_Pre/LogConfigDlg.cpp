// LogConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "LogConfigDlg.h"
#include "MPlatformOlbSystem.h"
#include "MLog.h"

// TT_Alarm
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre; 

/////////////////////////////////////////////////////////////////////////////
// CLogConfigDlg dialog

CLogConfigDlg::CLogConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_ucLevel = DEF_MLOG_NONE_LOG_LEVEL;
	if (m_plnkSystemData->m_bLogLevelError)
		m_ucLevel |= DEF_MLOG_ERROR_LOG_LEVEL;
	if (m_plnkSystemData->m_bLogLevelMonitoring)
		m_ucLevel |= DEF_MLOG_MONITORING_LOG_LEVEL;
	if (m_plnkSystemData->m_bLogLevelNormal)
		m_ucLevel |= DEF_MLOG_NORMAL_LOG_LEVEL;
//170427 JSH.s
	if (m_plnkSystemData->m_bLogLevelEqTactTime)
		m_ucLevel |= DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL;
//170427 JSH.e

	// TT_Alarm
//	if (m_plnkSystemData->m_bLogLevelTactTime)
//		m_ucLevel |= DEF_MLOG_TACT_TIME_LOG_LEVEL;
}


void CLogConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogConfigDlg)
	DDX_Control(pDX, IDB_NORMAL, m_ctrlBtnNormal);
	DDX_Control(pDX, IDB_TACT_TIME, m_ctrlBtnTactTime);
	DDX_Control(pDX, IDB_WARNING, m_ctrlBtnWarning);
	DDX_Control(pDX, IDB_ERROR, m_ctrlBtnError);
//170427 JSH.s
	DDX_Control(pDX, IDB_EQ_TACT_TIME, m_ctrlBtnEqTactTime);
//170427 JSH.e
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CLogConfigDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogConfigDlg message handlers

void CLogConfigDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CLogConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_plnkSystemData->m_bLogLevelError)
		m_ctrlBtnError.SetValue(TRUE);
	if (m_plnkSystemData->m_bLogLevelMonitoring)
		m_ctrlBtnWarning.SetValue(TRUE);
	if (m_plnkSystemData->m_bLogLevelNormal)
		m_ctrlBtnNormal.SetValue(TRUE);
//170427 JSH.s
	if (m_plnkSystemData->m_bLogLevelEqTactTime)
		m_ctrlBtnEqTactTime.SetValue(TRUE);
//170427 JSH.e

	// TT_Alarm
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsTactTimeLogSave())
		m_ctrlBtnTactTime.SetValue(TRUE);
//	if (m_plnkSystemData->m_bLogLevelTactTime)
//		m_ctrlBtnTactTime.SetValue(TRUE);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	
	CString szTextTemp;
	ReadDisplayMessage(SUB_DLG, _T("LogConfigDlg_1"), &szTextTemp);	
//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogConfigDlg_2"), &szTextTemp);	
//	저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogConfigDlg_3"), &szTextTemp);	
//	Tact Time
	GetDlgItem(IDB_TACT_TIME)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogConfigDlg_4"), &szTextTemp);	
//	에러
	GetDlgItem(IDB_ERROR)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogConfigDlg_5"), &szTextTemp);	
//	경고
	GetDlgItem(IDB_WARNING)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogConfigDlg_6"), &szTextTemp);	
//	정상
	GetDlgItem(IDB_NORMAL)->SetWindowText(szTextTemp);
//170427 JSH.s
	ReadDisplayMessage(SUB_DLG, _T("LogConfigDlg_7"), &szTextTemp);	
//	EQ_Tact Time
	GetDlgItem(IDB_TACT_TIME)->SetWindowText(szTextTemp);
//170427 JSH.e

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CLogConfigDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLogConfigDlg)
	ON_EVENT(CLogConfigDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CLogConfigDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CLogConfigDlg, IDB_TACT_TIME, -600 /* Click */, OnTactTime, VTS_NONE)
	ON_EVENT(CLogConfigDlg, IDB_ERROR, -600 /* Click */, OnError, VTS_NONE)
	ON_EVENT(CLogConfigDlg, IDB_WARNING, -600 /* Click */, OnWarning, VTS_NONE)
	ON_EVENT(CLogConfigDlg, IDB_NORMAL, -600 /* Click */, OnNormal, VTS_NONE)
//170427 JSH.s
	ON_EVENT(CLogConfigDlg, IDB_EQ_TACT_TIME, -600 /* Click */, OnEqTactTime, VTS_NONE)
//170427 JSH.e
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CLogConfigDlg::OnExit() 
{
	CDialog::OnCancel();
}

void CLogConfigDlg::OnSave() 
{
	// TT_Alarm
//	m_plnkSystemData->m_bLogLevelTactTime	= m_ucLevel & DEF_MLOG_TACT_TIME_LOG_LEVEL;
	m_plnkSystemData->m_bLogLevelError		= m_ucLevel & DEF_MLOG_ERROR_LOG_LEVEL;
	m_plnkSystemData->m_bLogLevelMonitoring	= m_ucLevel & DEF_MLOG_MONITORING_LOG_LEVEL;
	m_plnkSystemData->m_bLogLevelNormal		= m_ucLevel & DEF_MLOG_NORMAL_LOG_LEVEL;
//170427 JSH.s
	m_plnkSystemData->m_bLogLevelEqTactTime	= m_ucLevel & DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL;
//170427 JSH.e

	m_plnkSystemData->WriteData();
	MOlbSystemPre.SetLogAttribute(m_ucLevel, m_plnkSystemData->m_iLogKeepingDays);

	// TT_Alarm
	if (m_ucLevel & DEF_MLOG_TACT_TIME_LOG_LEVEL)
		MOlbSystemPre.GetTrsAutoManagerComponent()->SetTactTimeLogSave(TRUE);
	else
		MOlbSystemPre.GetTrsAutoManagerComponent()->SetTactTimeLogSave(FALSE);
}

void CLogConfigDlg::OnTactTime() 
{
	if (m_ucLevel & DEF_MLOG_TACT_TIME_LOG_LEVEL)
		m_ucLevel &= ~DEF_MLOG_TACT_TIME_LOG_LEVEL;
	else
		m_ucLevel |= DEF_MLOG_TACT_TIME_LOG_LEVEL;
}
//170427 JSH.s
void CLogConfigDlg::OnEqTactTime() 
{
	if (m_ucLevel & DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL)
		m_ucLevel &= ~DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL;
	else
		m_ucLevel |= DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL;
}

//170427 JSH.e

void CLogConfigDlg::OnError() 
{
	if (m_ucLevel & DEF_MLOG_ERROR_LOG_LEVEL)
		m_ucLevel &= ~DEF_MLOG_ERROR_LOG_LEVEL;
	else
		m_ucLevel |= DEF_MLOG_ERROR_LOG_LEVEL;
}

void CLogConfigDlg::OnWarning()
{
	if (m_ucLevel & DEF_MLOG_MONITORING_LOG_LEVEL)
		m_ucLevel &= ~DEF_MLOG_MONITORING_LOG_LEVEL;
	else
		m_ucLevel |= DEF_MLOG_MONITORING_LOG_LEVEL;
}

void CLogConfigDlg::OnNormal() 
{
	if (m_ucLevel & DEF_MLOG_NORMAL_LOG_LEVEL)
		m_ucLevel &= ~DEF_MLOG_NORMAL_LOG_LEVEL;
	else
		m_ucLevel |= DEF_MLOG_NORMAL_LOG_LEVEL;
}
