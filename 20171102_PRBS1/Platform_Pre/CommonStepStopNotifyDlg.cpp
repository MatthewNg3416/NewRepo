// CommonStepStopNotifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "CommonStepStopNotifyDlg.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonStepStopNotifyDlg dialog


CCommonStepStopNotifyDlg::CCommonStepStopNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonStepStopNotifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonStepStopNotifyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCommonStepStopNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonStepStopNotifyDlg)
	DDX_Control(pDX, IDC_EDITLABEL, m_Msg);
	DDX_Control(pDX, IDC_PICTURE, m_Icon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonStepStopNotifyDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonStepStopNotifyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonStepStopNotifyDlg message handlers

BOOL CCommonStepStopNotifyDlg::OnInitDialog() 
{
	CString	strTemp;
	WCHAR	pBuf[MAX_PATH] = {0};

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	::GetCurrentDirectory(sizeof(pBuf), pBuf);
	strTemp.Format(_T("%s\\res\\stopicon.bmp"), pBuf);
	m_Icon.SetPicture(strTemp); 
	m_Icon.SetPicturePosition(10);
	m_Icon.SetPictureKeepRatio(FALSE);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	
	CString szTextTemp;
	
	// 비상 정지 경고
	ReadDisplayMessage(SUB_DLG, _T("CommonStepStopNotifyDlg"), &szTextTemp);
	SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonStepStopNotifyDlg::SetMessage(CString strMsg)
{
	m_Msg.SetCaption(strMsg);
}
