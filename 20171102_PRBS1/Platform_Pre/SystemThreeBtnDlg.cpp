// SystemThreeBtnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "SystemThreeBtnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemThreeBtnDlg dialog


CSystemThreeBtnDlg::CSystemThreeBtnDlg(CString strTitle, CString strOne, CString strTwo, CString strThree, CWnd* pParent /*=NULL*/)
	: CDialog(CSystemThreeBtnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemThreeBtnDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_strTitle = strTitle;
	m_strOne = strOne;
	m_strTwo = strTwo;
	m_strThree = strThree;
}

BOOL CSystemThreeBtnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_BUTTON1)->SetWindowText(m_strOne);
	GetDlgItem(IDC_BUTTON2)->SetWindowText(m_strTwo);
	//20171031 KDW Start
	GetDlgItem(IDC_BUTTON3)->SetWindowText(m_strThree);
	if(m_strThree == _T(""))
		GetDlgItem(IDC_BUTTON3)->EnableWindow(SW_HIDE);
	else
		GetDlgItem(IDC_BUTTON3)->EnableWindow(SW_SHOW);
	//20171031 KDW End

	SetWindowText(m_strTitle);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CSystemThreeBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemThreeBtnDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemThreeBtnDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemThreeBtnDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemThreeBtnDlg message handlers

BEGIN_EVENTSINK_MAP(CSystemThreeBtnDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSystemThreeBtnDlg)
	ON_EVENT(CSystemThreeBtnDlg, IDC_BUTTON1, -600 /* Click */, OnButton1, VTS_NONE)
	ON_EVENT(CSystemThreeBtnDlg, IDC_BUTTON2, -600 /* Click */, OnButton2, VTS_NONE)
	ON_EVENT(CSystemThreeBtnDlg, IDC_BUTTON3, -600 /* Click */, OnButton3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSystemThreeBtnDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CSystemThreeBtnDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	EndDialog(1);
}

void CSystemThreeBtnDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	EndDialog(2);
}


