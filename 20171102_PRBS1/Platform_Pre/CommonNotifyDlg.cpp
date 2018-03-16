// CommonNotifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "CommonNotifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonNotifyDlg dialog


CCommonNotifyDlg::CCommonNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonNotifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonNotifyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(WHITE);
}


void CCommonNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonNotifyDlg)
	DDX_Control(pDX, IDC_EDIT_LABEL, m_ctrlLblMsg);
	DDX_Control(pDX, IDC_PICTURE, m_ctrlLblPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonNotifyDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonNotifyDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonNotifyDlg message handlers

BEGIN_EVENTSINK_MAP(CCommonNotifyDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommonNotifyDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCommonNotifyDlg::OnInitDialog() 
{
	CString	strTemp;
	WCHAR	pBuf[MAX_PATH] = {0};
	
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	::GetCurrentDirectory(sizeof(pBuf), pBuf);
	strTemp.Format(_T("%s\\res\\infoicon.bmp"), pBuf);
	m_ctrlLblPicture.SetPicture(strTemp);
	m_ctrlLblPicture.SetPicturePosition(10);
	m_ctrlLblPicture.SetPictureKeepRatio(FALSE);
	
	m_ctrlLblMsg.SetCaption(m_strMsg);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonNotifyDlg::SetMessage(CString strMsg)
{
	m_strMsg = strMsg;
	m_ctrlLblMsg.SetCaption(strMsg);
}

void CCommonNotifyDlg::SetTitle(CString strTitle)
{
	SetWindowText(strTitle);
}

void CCommonNotifyDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(),&m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

/**	Message pumping function that can either be used to pump messages during
 *	long operations. This version will only pass messages to this window (and
 *	all child windows). (Thanks to Michael <mbh-ep@post5.tele.dk> for this)
 */
void CCommonNotifyDlg::PeekAndPump()
{
	//SetFocus();

    MSG msg;
    while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) 
    {
		if (!AfxGetApp()->PumpMessage()) 
        {
			::PostQuitMessage(0);
			return;
		} 
    }
}
