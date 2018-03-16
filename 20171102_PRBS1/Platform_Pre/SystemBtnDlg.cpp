// SystemBtnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "SystemBtnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SystemBtnDlg dialog


SystemBtnDlg::SystemBtnDlg(CString strTitle, CString strOne, CString strTwo, CString strThree, CString strFour, CString strFive, CWnd* pParent /*=NULL*/)
	: CDialog(SystemBtnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SystemBtnDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_strTitle = strTitle;
	m_strOne = strOne;
	m_strTwo = strTwo;
	m_strThree = strThree;
	m_strFour = strFour;
	m_strFive = strFive;
	
}


void SystemBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SystemBtnDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SystemBtnDlg, CDialog)
	//{{AFX_MSG_MAP(SystemBtnDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SystemBtnDlg message handlers

BEGIN_EVENTSINK_MAP(SystemBtnDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(SystemBtnDlg)
	ON_EVENT(SystemBtnDlg, IDC_FIVE_BUTTON1, -600 /* Click */, OnClickFiveButton1, VTS_NONE)
	ON_EVENT(SystemBtnDlg, IDC_FIVE_BUTTON2, -600 /* Click */, OnClickFiveButton2, VTS_NONE)
	ON_EVENT(SystemBtnDlg, IDC_FIVE_BUTTON3, -600 /* Click */, OnClickFiveButton3, VTS_NONE)
	ON_EVENT(SystemBtnDlg, IDC_FIVE_BUTTON4, -600 /* Click */, OnClickFiveButton4, VTS_NONE)
	ON_EVENT(SystemBtnDlg, IDC_FIVE_BUTTON5, -600 /* Click */, OnClickFiveButton5, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void SystemBtnDlg::OnClickFiveButton1() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void SystemBtnDlg::OnClickFiveButton2() 
{
	// TODO: Add your control notification handler code here
	EndDialog(1);
}

void SystemBtnDlg::OnClickFiveButton3() 
{
	// TODO: Add your control notification handler code here
	EndDialog(2);
}

void SystemBtnDlg::OnClickFiveButton4() 
{
	// TODO: Add your control notification handler code here
	EndDialog(3);
}

void SystemBtnDlg::OnClickFiveButton5() 
{
	// TODO: Add your control notification handler code here
	EndDialog(4);
}

BOOL SystemBtnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_FIVE_BUTTON1)->SetWindowText(m_strOne);
	GetDlgItem(IDC_FIVE_BUTTON2)->SetWindowText(m_strTwo);
	GetDlgItem(IDC_FIVE_BUTTON3)->SetWindowText(m_strThree);
	GetDlgItem(IDC_FIVE_BUTTON4)->SetWindowText(m_strFour);
	GetDlgItem(IDC_FIVE_BUTTON5)->SetWindowText(m_strFive);

	SetWindowText(m_strTitle);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
