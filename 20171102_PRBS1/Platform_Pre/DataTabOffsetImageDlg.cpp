// DataTabOffsetImageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DefSystemConfig.h"
#include "mplatformolb.h"
#include "DataTabOffsetImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataTabOffsetImageDlg dialog


CDataTabOffsetImageDlg::CDataTabOffsetImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataTabOffsetImageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataTabOffsetImageDlg)
	//}}AFX_DATA_INIT
}


void CDataTabOffsetImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataTabOffsetImageDlg)
	DDX_Control(pDX, IDC_TAB_IMAGE, m_sTabImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataTabOffsetImageDlg, CDialog)
	//{{AFX_MSG_MAP(CDataTabOffsetImageDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTabOffsetImageDlg message handlers

BOOL CDataTabOffsetImageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	WCHAR szCurrentDirectory[255];
	::GetCurrentDirectory(255, szCurrentDirectory);

#ifdef DEF_SOURCE_SYSTEM
#	ifndef DEF_MIRROR_
	lstrcat(szCurrentDirectory, _T("\\res\\TabOffsetNormal.jpg"));
#	else
	lstrcat(szCurrentDirectory, _T("\\res\\TabOffsetMirror.jpg"));
#	endif
#else	//Gate
#	ifndef DEF_MIRROR_
	lstrcat(szCurrentDirectory, _T("\\res\\TabOffsetNormal_G.jpg"));
#	else
	lstrcat(szCurrentDirectory, _T("\\res\\TabOffsetMirror_G.jpg"));
#	endif
#endif
	m_sTabImage.SetPicture(szCurrentDirectory);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDataTabOffsetImageDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataTabOffsetImageDlg)
	ON_EVENT(CDataTabOffsetImageDlg, IDB_CANCEL, -600 /* Click */, OnClickCancel, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataTabOffsetImageDlg::OnClickCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
