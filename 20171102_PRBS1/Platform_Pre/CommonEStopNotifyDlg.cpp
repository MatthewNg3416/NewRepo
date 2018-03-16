// CommonEStopNotifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "CommonEStopNotifyDlg.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonEStopNotifyDlg dialog

CCommonEStopNotifyDlg::CCommonEStopNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonEStopNotifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonEStopNotifyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(RGB(255,255,255));
}


void CCommonEStopNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonEStopNotifyDlg)
	DDX_Control(pDX, IDC_EDITLABEL, m_Msg);
	DDX_Control(pDX, IDC_PICTURE, m_Icon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonEStopNotifyDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonEStopNotifyDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonEStopNotifyDlg message handlers

BOOL CCommonEStopNotifyDlg::OnInitDialog() 
{
	CString	strTemp;
	WCHAR	pBuf[MAX_PATH] = {0};

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	::GetCurrentDirectory(sizeof(pBuf), pBuf);
	strTemp.Format(_T("%s\\res\\erricon.bmp"), pBuf);
	m_Icon.SetPicture(strTemp); 
	m_Icon.SetPicturePosition(10);
	m_Icon.SetPictureKeepRatio(FALSE);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	
	//비상 정지 경고
	ReadDisplayMessage(SUB_DLG, _T("CommonEStopNotifyDlg_1"), &szTextTemp);
	SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonEStopNotifyDlg::SetMessage(CString strMsg)
{
	m_Msg.SetCaption(strMsg);
}

void CCommonEStopNotifyDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);

	// Do not call CDialog::OnPaint() for painting messages
}
