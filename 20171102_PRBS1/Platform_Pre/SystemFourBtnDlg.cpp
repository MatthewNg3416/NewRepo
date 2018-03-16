// SystemFourBtnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "SystemFourBtnDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemFourBtnDlg dialog


CSystemFourBtnDlg::CSystemFourBtnDlg(CString strTitle, CString strOne, CString strTwo, CString strThree, CString strFour, CWnd* pParent /*=NULL*/)
	: CDialog(CSystemFourBtnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemFourBtnDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_strTitle = strTitle;
	m_strOne = strOne;
	m_strTwo = strTwo;
	m_strThree = strThree;
	m_strFour = strFour;
}


void CSystemFourBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemFourBtnDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemFourBtnDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemFourBtnDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemFourBtnDlg message handlers

BEGIN_EVENTSINK_MAP(CSystemFourBtnDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSystemFourBtnDlg)
	ON_EVENT(CSystemFourBtnDlg, IDC_BUTTON1, -600 /* Click */, OnClickButton1, VTS_NONE)
	ON_EVENT(CSystemFourBtnDlg, IDC_BUTTON2, -600 /* Click */, OnClickButton2, VTS_NONE)
	ON_EVENT(CSystemFourBtnDlg, IDC_BUTTON3, -600 /* Click */, OnClickButton3, VTS_NONE)
	ON_EVENT(CSystemFourBtnDlg, IDC_BUTTON4, -600 /* Click */, OnClickButton4, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSystemFourBtnDlg::OnClickButton1() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CSystemFourBtnDlg::OnClickButton2() 
{
	// TODO: Add your control notification handler code here
	EndDialog(1);	
}

void CSystemFourBtnDlg::OnClickButton3() 
{
	// TODO: Add your control notification handler code here
	EndDialog(2);	
}

void CSystemFourBtnDlg::OnClickButton4() 
{
	// TODO: Add your control notification handler code here
	EndDialog(3);	
}

BOOL CSystemFourBtnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_BUTTON1)->SetWindowText(m_strOne);
	GetDlgItem(IDC_BUTTON2)->SetWindowText(m_strTwo);
	GetDlgItem(IDC_BUTTON3)->SetWindowText(m_strThree);
	GetDlgItem(IDC_BUTTON4)->SetWindowText(m_strFour);
	//20131218 SJ_HJG
	if(m_strFour == "ÀÛ¾÷Pass\n(OS)")
	{
		GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	}

	SetWindowText(m_strTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
