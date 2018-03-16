// CommonPendantNotifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "CommonPendantNotifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

/////////////////////////////////////////////////////////////////////////////
// CCommonPendantNotifyDlg dialog


CCommonPendantNotifyDlg::CCommonPendantNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonPendantNotifyDlg::IDD, pParent) 
{
	//{{AFX_DATA_INIT(CCommonPendantNotifyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(RGB(255,255,255));
}


void CCommonPendantNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonPendantNotifyDlg)
	DDX_Control(pDX, IDC_EDITLABEL, m_Msg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonPendantNotifyDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonPendantNotifyDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonPendantNotifyDlg message handlers

BOOL CCommonPendantNotifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//�������ϰ� ����� ��ƾ.
	SLWA pSetLayeredWindowAttributes = NULL;			// �Լ������� ����, �ʱ�ȭ.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));	// �ν��Ͻ� ����.
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
    //�Լ������� ����.
	HWND hwnd = this->m_hWnd; //���̾�α��� �ڵ� ����.
	SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | 0x00080000);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * 70) / 120, 0x00000002);	
	//���̾�α� �������ϰ� ����.
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonPendantNotifyDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CCommonPendantNotifyDlg::SetMessage(CString strMsg)
{
	m_Msg.SetCaption(strMsg);
}

