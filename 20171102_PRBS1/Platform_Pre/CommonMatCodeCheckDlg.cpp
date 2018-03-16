// CommonMatCodeCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "CommonMatCodeCheckDlg.h"
#include "common.h"
#include "MPlatformOlbSystem.h"
#include "MCtrlTabFeeder.h"
#include "MLCNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonMatCodeCheckDlg dialog

extern	MPlatformOlbSystem	MOlbSystemPre;

CCommonMatCodeCheckDlg::CCommonMatCodeCheckDlg(int iTabFeederNo, int iSelectedReel, CWnd* pParent /*=NULL*/)
	: CDialog(CCommonMatCodeCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonMatCodeCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iTabFeederNo = iTabFeederNo;
	m_iSelectedReel = iSelectedReel;
	m_plnkCtrlTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(m_iTabFeederNo);
}


void CCommonMatCodeCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonMatCodeCheckDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonMatCodeCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonMatCodeCheckDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonMatCodeCheckDlg message handlers

BEGIN_EVENTSINK_MAP(CCommonMatCodeCheckDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommonMatCodeCheckDlg)
	ON_EVENT(CCommonMatCodeCheckDlg, IDB_USER_ID, -600 /* Click */, OnClickUserId, VTS_NONE)
	ON_EVENT(CCommonMatCodeCheckDlg, IDB_MAT_CANCEL, -600 /* Click */, OnClickMatCancel, VTS_NONE)
	ON_EVENT(CCommonMatCodeCheckDlg, IDB_MAT_START, -600 /* Click */, OnClickMatStart, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCommonMatCodeCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDS_MAT_VALIDATION_3)->SetWindowText(_T(""));
	GetDlgItem(IDS_MAT_VALIDATION_4)->SetWindowText(_T(""));
	GetDlgItem(IDS_MAT_VALIDATION_5)->SetWindowText(_T(""));
	GetDlgItem(IDS_MAT_VALIDATION_6)->SetWindowText(_T(""));

	CString strTabFeederNo;
	CString strUsingReelInfo;
	CString strBatchID;

	if (m_iTabFeederNo == DEF_TABFEEDER1)
		strTabFeederNo.Format(_T("[TabFeeder1]"));
	else	
		strTabFeederNo.Format(_T("[TabFeeder2]"));

	if (m_iSelectedReel == DEF_USING_REEL)
		strUsingReelInfo.Format(_T("[Using Reel]"));
	else
		strUsingReelInfo.Format(_T("[Spare Reel]"));
	
	strBatchID.Format(_T("[BatchID] %s"), m_plnkCtrlTabFeeder->m_matTabICInfo[m_iSelectedReel].m_sM_BATCHID);

	CString strStandardInfo;
	strStandardInfo.Format(_T("%s %s %s"), strTabFeederNo, strUsingReelInfo, strBatchID);
	GetDlgItem(IDS_MAT_VALIDATION_3)->SetWindowText(strStandardInfo);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonMatCodeCheckDlg::OnClickUserId() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDS_MAT_VALIDATION_6)->SetWindowText(_T(""));

	WCHAR getbuf[128];
	if (!GetValueBox(getbuf, 10, _T(""), _T("Input Your ID. (Max 10)"), FALSE))
		return;

	m_strUserID = (CString)getbuf;

	GetDlgItem(IDS_MAT_VALIDATION_5)->SetWindowText((LPCTSTR)(getbuf));
}

void CCommonMatCodeCheckDlg::OnClickMatCancel() 
{
	// TODO: Add your control notification handler code here
	m_plnkCtrlTabFeeder->m_matTabICInfo[m_iSelectedReel].m_nM_STATE = eMatStateHold;

	CDialog::OnOK();	
}

void CCommonMatCodeCheckDlg::OnClickMatStart() 
{
	// TODO: Add your control notification handler code here
	if (m_strUserID.GetLength() == 0)
	{
		GetDlgItem(IDS_MAT_VALIDATION_6)->SetWindowText(_T("You must enter your ID"));
		return;
	}

	m_plnkCtrlTabFeeder->m_matTabICInfo[m_iSelectedReel].m_nM_STATE = eMatStateEnable;

	m_plnkCtrlTabFeeder->m_matTabICInfo[m_iSelectedReel].m_sPORT_STATE = m_strUserID;
	MOlbSystemPre.GetLCNetComponent()->MaterialReport(eMatUserCheck, &(m_plnkCtrlTabFeeder->m_matTabICInfo[m_iSelectedReel]));

	CDialog::OnOK();	
}



void CCommonMatCodeCheckDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_plnkCtrlTabFeeder->m_matTabICInfo[m_iSelectedReel].m_nM_STATE != eMatStateEnable)
		m_plnkCtrlTabFeeder->m_matTabICInfo[m_iSelectedReel].m_nM_STATE = eMatStateHold;
}
