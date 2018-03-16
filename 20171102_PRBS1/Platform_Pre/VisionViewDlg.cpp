// VisionViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "VisionViewDlg.h"
#include "IVision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisionViewDlg dialog


CVisionViewDlg::CVisionViewDlg(IVision* plnkVision, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionViewDlg::IDD, pParent), m_plnkVision(plnkVision), m_iViewID(-1)
{
	//{{AFX_DATA_INIT(CVisionViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVisionViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionViewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisionViewDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionViewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionViewDlg message handlers

BOOL CVisionViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// Vision Display Setting
	CWnd *pViewWnd = GetDlgItem(IDC_VIEW);
	RECT rect;
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_iViewID = m_plnkVision->CreateTemporaryView(&rect, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVisionViewDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionViewDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CVisionViewDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_plnkVision->DestroyView(m_iViewID);	
	return CDialog::DestroyWindow();
}
