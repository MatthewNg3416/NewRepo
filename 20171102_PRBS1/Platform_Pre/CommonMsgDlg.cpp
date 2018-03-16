// CommonMsgDlg.cpp: implementation file
//

#include "stdafx.h"
#include "CommonMsgDlg.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonMsgDlg dialog


CCommonMsgDlg::CCommonMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonMsgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(RGB(255,255,255));

	m_cx = 110;
	m_cy = 16;
}

void CCommonMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonMsgDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, ID_MYABORT, m_bMyAbort);
	DDX_Control(pDX, ID_MYCANCEL, m_bMyCancel);
	DDX_Control(pDX, ID_MYCANCEL3, m_bMyCancel3);
	DDX_Control(pDX, ID_MYIGNORE, m_bMyIgnore);
	DDX_Control(pDX, ID_MYNO, m_bMyNo);
	DDX_Control(pDX, ID_MYOK, m_bMyOk);
	DDX_Control(pDX, ID_MYOK2, m_bMyOk2);
	DDX_Control(pDX, ID_MYRETRY, m_bMyRetry);
	DDX_Control(pDX, ID_MYRETRY3, m_bMyRetry3);
	DDX_Control(pDX, ID_MYYES, m_bMyYes);
	DDX_Control(pDX, IDC_PICTURE, m_MyIcon);
	DDX_Control(pDX, IDC_EDITLABEL, m_lblMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonMsgDlg)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_DRAWITEM()
		ON_WM_PAINT()
		ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CCommonMsgDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommonMsgDlg)
	ON_EVENT(CCommonMsgDlg, ID_MYABORT, -600 /* Click */, OnClickMyabort, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYOK, -600 /* Click */, OnClickMyok, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYYES, -600 /* Click */, OnClickMyyes, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYRETRY3, -600 /* Click */, OnClickMyretry3, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYOK2, -600 /* Click */, OnClickMyok2, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYRETRY, -600 /* Click */, OnClickMyretry, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYCANCEL, -600 /* Click */, OnClickMycancel, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYCANCEL3, -600 /* Click */, OnClickMycancel3, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYNO, -600 /* Click */, OnClickMyno, VTS_NONE)
	ON_EVENT(CCommonMsgDlg, ID_MYIGNORE, -600 /* Click */, OnClickMyignore, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonMsgDlg message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CCommonMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strTemp;
	WCHAR pBuf[MAX_PATH] = {0};
	
	SetWindowText(m_title);
	
	m_bMyOk2.ShowWindow(SW_HIDE);
	m_bMyOk.ShowWindow(SW_HIDE);
	m_bMyCancel.ShowWindow(SW_HIDE);
	m_bMyYes.ShowWindow(SW_HIDE);
	m_bMyNo.ShowWindow(SW_HIDE);
	m_bMyAbort.ShowWindow(SW_HIDE);
	m_bMyRetry.ShowWindow(SW_HIDE);
	m_bMyIgnore.ShowWindow(SW_HIDE);
	m_bMyCancel3.ShowWindow(SW_HIDE);
	m_bMyRetry3.ShowWindow(SW_HIDE);
	
	switch (m_type)
	{
	case M_OK:
		m_bMyOk2.ShowWindow(SW_SHOW);
		break;
		
	case M_OKCANCEL:
		m_bMyOk.ShowWindow(SW_SHOW);
		m_bMyCancel.ShowWindow(SW_SHOW);
		break;
		
	case M_YESNO:
		m_bMyYes.SetCaption(m_strYes);
		m_bMyYes.ShowWindow(SW_SHOW);
		m_bMyNo.SetCaption(m_strNo);
		m_bMyNo.ShowWindow(SW_SHOW);
		break;
		
	case M_ABORTRETRYIGNORE:
		m_bMyAbort.ShowWindow(SW_SHOW);
		m_bMyRetry.ShowWindow(SW_SHOW);
		m_bMyIgnore.ShowWindow(SW_SHOW);
		break;
		
	case M_RETRYCANCEL:
		m_bMyCancel3.ShowWindow(SW_SHOW);
		m_bMyRetry3.ShowWindow(SW_SHOW);
		break;
		
	default:
		m_bMyOk2.ShowWindow(SW_SHOW);
		break;
	}
	
	switch (m_icontype)
	{
	case M_ICONINFORMATION:  
		::GetCurrentDirectory(sizeof(pBuf), pBuf);
		strTemp.Format(_T("%s\\res\\infoicon.bmp"), pBuf);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);
		break;
		
	case M_ICONQUESTION:
		::GetCurrentDirectory(sizeof(pBuf), pBuf);
		strTemp.Format(_T("%s\\res\\quesicon.bmp"), pBuf);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);
		break;
		
	case M_ICONSTOP:
		::GetCurrentDirectory(sizeof(pBuf), pBuf);
		strTemp.Format(_T("%s\\res\\stopicon.bmp"), pBuf);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);
		break;
		
	case M_ICONERROR:
		::GetCurrentDirectory(sizeof(pBuf), pBuf);
		strTemp.Format(_T("%s\\res\\erricon.bmp"), pBuf);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);
		break;
		
	default:
       	::GetCurrentDirectory(sizeof(pBuf), pBuf);
		strTemp.Format(_T("%s\\res\\infoicon.bmp"), pBuf);
		m_MyIcon.SetPicture(strTemp); 
		m_MyIcon.SetPicturePosition(10);
		m_MyIcon.SetPictureKeepRatio(FALSE);
		break;
	}
	
	m_lblMsg.SetCaption(m_message);  
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonMsgDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CCommonMsgDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

HBRUSH CCommonMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (m_icontype != M_ICONERROR)
		return hbr;
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CCommonMsgDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			pMsg->message = 0;
			pMsg->wParam = 0;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CCommonMsgDlg::OnClickMyabort() 
{
	EndDialog(IDABORT);
}

void CCommonMsgDlg::OnClickMyok() 
{
	EndDialog(IDOK);
}

void CCommonMsgDlg::OnClickMyyes() 
{
	EndDialog(IDYES);
}

void CCommonMsgDlg::OnClickMyretry3() 
{
	EndDialog(IDRETRY);
}

void CCommonMsgDlg::OnClickMyok2() 
{
	EndDialog(IDOK);
}

void CCommonMsgDlg::OnClickMyretry() 
{
	EndDialog(IDRETRY);
}

void CCommonMsgDlg::OnClickMycancel() 
{
	EndDialog(IDCANCEL);
}

void CCommonMsgDlg::OnClickMycancel3() 
{
	EndDialog(IDCANCEL);
}

void CCommonMsgDlg::OnClickMyno() 
{
	EndDialog(IDNO);
}

void CCommonMsgDlg::OnClickMyignore() 
{
	EndDialog(IDIGNORE);
}