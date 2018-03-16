// LogFileSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "LogFileSelectDlg.h"
#include "LogMainForm.h"
#include "MPlatformOlbSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CLogFileSelectDlg dialog

CLogFileSelectDlg::CLogFileSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogFileSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogFileSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_pSystemInfo = MOlbSystemPre.GetSystemInfo();
	m_pParent = (CLogMainForm*)pParent;
}


void CLogFileSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogFileSelectDlg)
	DDX_Control(pDX, IDS_LIST, m_listLogFile);
	DDX_Control(pDX, IDS_CURRENT_LOG_FILE, m_valueSelectedLog);
	DDX_Control(pDX, IDS_LOG_FILE_LIST, m_labelLogListTitle);
	DDX_Control(pDX, IDB_UP, m_btnLogListUp);
	DDX_Control(pDX, IDB_DOWN, m_btnLogListDown);
	DDX_Control(pDX, IDB_PAGE_UP, m_btnLogListPgUp);
	DDX_Control(pDX, IDB_PAGE_DOWN, m_btnLogListPgDown);
	DDX_Control(pDX, IDB_EXIT, m_btnExit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogFileSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CLogFileSelectDlg)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDS_LIST, OnSelchangeList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogFileSelectDlg message handlers

void CLogFileSelectDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CLogFileSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitializeLogListBox();
	InitializeButton();
	InitializeLabel();

	CLogFileSelectDlg::InitializeButton();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CLogFileSelectDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLogFileSelectDlg)
	ON_EVENT(CLogFileSelectDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CLogFileSelectDlg, IDB_UP, -600 /* Click */, OnUp, VTS_NONE)
	ON_EVENT(CLogFileSelectDlg, IDB_DOWN, -600 /* Click */, OnDown, VTS_NONE)
	ON_EVENT(CLogFileSelectDlg, IDB_PAGE_UP, -600 /* Click */, OnPageUp, VTS_NONE)
	ON_EVENT(CLogFileSelectDlg, IDB_PAGE_DOWN, -600 /* Click */, OnPageDown, VTS_NONE)
	ON_EVENT(CLogFileSelectDlg, IDS_LIST, -600 /* Click */, OnList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CLogFileSelectDlg::InitializeLogListBox()
{

	m_listLogFile.ResetContent();
	m_iNumList = m_pSystemInfo->GetNumObjects();
	m_pSystemInfo->GetObjectInform(m_sInfoObject);

	for (int i = 1 ; i < m_iNumList ;i++)
		m_listLogFile.InsertString(-1,(LPCTSTR)m_sInfoObject[i].m_strObjectName);

	m_iCurrentSel = 0;
	m_listLogFile.SetCurSel(m_iCurrentSel);

	if (m_iNumList != 0)
		m_listLogFile.GetText(m_iCurrentSel,m_strSelectedFile);

	if (m_iNumList == 0) m_valueSelectedLog.SetWindowText(_T(""));
	else m_valueSelectedLog.SetWindowText(m_strSelectedFile);

	return;
}

void CLogFileSelectDlg::InitializeLabel()
{
	m_labelLogListTitle.SetForeColor(BASIC);
	m_labelLogListTitle.SetBackColor(BLACK);
	
	m_valueSelectedLog.SetCaption(m_strSelectedFile);
	m_valueSelectedLog.SetForeColor(BASIC);
	m_valueSelectedLog.SetBackColor(BLACK);
}

void CLogFileSelectDlg::InitializeButton()
{
	CString szTextTemp;
	ReadDisplayMessage(SUB_DLG, _T("LogFileSelectDlg_1"), &szTextTemp);	
	//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogFileSelectDlg_2"), &szTextTemp);	
	//	위
	GetDlgItem(IDB_UP)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogFileSelectDlg_3"), &szTextTemp);	
	//	아래
	GetDlgItem(IDB_DOWN)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogFileSelectDlg_4"), &szTextTemp);	
	//	페이지 위
	GetDlgItem(IDB_PAGE_UP)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogFileSelectDlg_5"), &szTextTemp);	
	//	페이지 아래
	GetDlgItem(IDB_PAGE_DOWN)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("LogFileSelectDlg_6"), &szTextTemp);
	//	로그 파일 리스트	
	GetDlgItem(IDS_LOG_FILE_LIST)->SetWindowText(szTextTemp);

}

void CLogFileSelectDlg::OnExit() 
{
	EndDialog(m_sInfoObject[m_iCurrentSel+1].m_iObjectBase + m_sInfoObject[m_iCurrentSel+1].m_iInstanceNo);
}

void CLogFileSelectDlg::OnUp() 
{
	m_iCurrentSel = m_listLogFile.GetCurSel();

	if (m_iCurrentSel <= 0) return;
	else m_iCurrentSel--;

	m_listLogFile.SetCurSel(m_iCurrentSel);
	m_listLogFile.GetText(m_iCurrentSel, m_strSelectedFile);
	m_valueSelectedLog.SetCaption(m_strSelectedFile);
//	m_pParent->SetCurrentObjectID(m_sInfoObject[m_iCurrentSel+1].m_iObjectID);	
}

void CLogFileSelectDlg::OnDown() 
{
	// TODO: Add your control notification handler code here
	m_iCurrentSel = m_listLogFile.GetCurSel();

	if (m_iCurrentSel >= (m_iNumList-2)) return;
	else m_iCurrentSel++;

	m_listLogFile.SetCurSel(m_iCurrentSel);
	m_listLogFile.GetText(m_iCurrentSel, m_strSelectedFile);
	m_valueSelectedLog.SetCaption(m_strSelectedFile);	
}

void CLogFileSelectDlg::OnPageUp() 
{
	// TODO: Add your control notification handler code here
	if (m_iCurrentSel - 17 <= 0) m_iCurrentSel = 0;
	else m_iCurrentSel -= 17;
	
	m_listLogFile.SetCurSel(m_iCurrentSel);
	m_listLogFile.GetText(m_iCurrentSel, m_strSelectedFile);
	m_valueSelectedLog.SetCaption(m_strSelectedFile);	
}

void CLogFileSelectDlg::OnPageDown() 
{
	// TODO: Add your control notification handler code here
	if (m_iCurrentSel + 17 >= m_iNumList - 1) m_iCurrentSel = m_iNumList - 2;
	else m_iCurrentSel += 17;
	
	m_listLogFile.SetCurSel(m_iCurrentSel);
	m_listLogFile.GetText(m_iCurrentSel, m_strSelectedFile);
	m_valueSelectedLog.SetCaption(m_strSelectedFile);	
}

void CLogFileSelectDlg::OnList() 
{
	 m_iCurrentSel = m_listLogFile.GetCurSel();

	if (m_iNumList != 0)
		m_listLogFile.GetText(m_iCurrentSel,m_strSelectedFile);

    m_valueSelectedLog.SetCaption(m_strSelectedFile);

	if (m_iNumList != 0)
		m_pParent->SetCurrentObjectID(m_sInfoObject[m_iCurrentSel+1].m_iObjectBase);	
}

void CLogFileSelectDlg::OnSelchangeList() 
{
	m_iCurrentSel = m_listLogFile.GetCurSel();

	m_listLogFile.SetCurSel(m_iCurrentSel);
	m_listLogFile.GetText(m_iCurrentSel, m_strSelectedFile);
	m_valueSelectedLog.SetCaption(m_strSelectedFile);	
}
