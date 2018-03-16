// DlgFixedVision1.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "DlgFixedVision.h"
#include "MPlatformOlbSystem.h"
#include "IVision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

extern	MPlatformOlbSystem	MOlbSystemPre;
/////////////////////////////////////////////////////////////////////////////
// CManualLimitSensorDlg dialog

CDlgFixedVision::CDlgFixedVision(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFixedVision::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFixedVision)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	for (int i = 0; i < DEF_MAX_DLG_FIXED_VIEW; i++)
	{
		m_rgiViewID[i] = -1;
	}
}


void CDlgFixedVision::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFixedVision)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFixedVision, CDialog)
	//{{AFX_MSG_MAP(CDlgFixedVision)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_NOTLAYER, OnBtnNotlayer)
	ON_BN_CLICKED(IDC_BTN_LAYER, OnBtnLayer)
	ON_BN_CLICKED(IDC_BTN_LAYER2, OnBtnLayer2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedVision message handlers

BOOL CDlgFixedVision::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_plnkVision	= MOlbSystemPre.GetVisionComponent();

	CreateFixedView();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFixedVision::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
#ifndef VISION_SIMULATION
	for (int i = 0; i < DEF_MAX_DLG_FIXED_VIEW; i++)
	{
		m_plnkVision->RedrawView(m_rgiViewID[i]);
	}
#endif
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgFixedVision::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CRect rc;
	GetWindowRect(&rc);
	CPoint pt = rc.TopLeft();
	rc.OffsetRect(pt);
//	SetWindowPos(&CWnd::wndTopMost,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOSIZE);

	SLWA pSetLayeredWindowAttributes = NULL;			// 함수포인터 선언, 초기화.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));	// 인스턴스 얻음.
	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes"); // 함수포인터 얻음.
	HWND hwnd = this->m_hWnd; // 다이얼로그의 핸들 얻음.
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | DEF_WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (BYTE)(255 * double(100/100.0)), DEF_LWA_ALPHA);	// 다이얼로그 반투명하게 만듬.

	SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOSIZE);
}

void CDlgFixedVision::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	for (int i = 0; i < DEF_MAX_DLG_FIXED_VIEW; i++)
		m_plnkVision->DestroyView(m_rgiViewID[i]);	
}

void CDlgFixedVision::CreateFixedView()
{
#ifndef VISION_SIMULATION
	CWnd* pViewWnd;
	RECT  rect;

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS7);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[0] = m_plnkVision->CreateFixedView(6, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS8);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[1] = m_plnkVision->CreateFixedView(7, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS9);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[2] = m_plnkVision->CreateFixedView(8, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS10);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[3] = m_plnkVision->CreateFixedView(9, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS11);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[4] = m_plnkVision->CreateFixedView(10, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS12);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[5] = m_plnkVision->CreateFixedView(11, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS13);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[6] = m_plnkVision->CreateFixedView(12, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS14);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[7] = m_plnkVision->CreateFixedView(13, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS15);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[8] = m_plnkVision->CreateFixedView(14, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS16);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[9] = m_plnkVision->CreateFixedView(15, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS17);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[10] = m_plnkVision->CreateFixedView(16, &rect, this);

	pViewWnd = GetDlgItem(IDC_VISION_CANVAS18);
	pViewWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_rgiViewID[11] = m_plnkVision->CreateFixedView(17, &rect, this);
#endif
}

BOOL CDlgFixedVision::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN) 
			pMsg->wParam = NULL;
		if(pMsg->wParam == VK_ESCAPE)
			pMsg->wParam = NULL;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFixedVision::OnBtnNotlayer() 
{
	// TODO: Add your control notification handler code here
	SLWA pSetLayeredWindowAttributes = NULL;			// 함수포인터 선언, 초기화.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));	// 인스턴스 얻음.
	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes"); // 함수포인터 얻음.
	HWND hwnd = this->m_hWnd; // 다이얼로그의 핸들 얻음.
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | DEF_WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (BYTE)(255 * double(100/100.0)), DEF_LWA_ALPHA);	// 다이얼로그 반투명하게 만듬.

//	SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOSIZE);	
}

void CDlgFixedVision::OnBtnLayer()
{
	// TODO: Add your control notification handler code here
	// 반투명하게 만드는 루틴.
	SLWA pSetLayeredWindowAttributes = NULL;			// 함수포인터 선언, 초기화.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));	// 인스턴스 얻음.

	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes"); // 함수포인터 얻음.
	HWND hwnd = this->m_hWnd; // 다이얼로그의 핸들 얻음.
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | DEF_WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (BYTE)(255 * double(30/100.0)), DEF_LWA_ALPHA);	// 다이얼로그 반투명하게 만듬.

//	SetWindowPos(&CWnd::wndBottom,0,0,0,0,SWP_NOSIZE);
}

void CDlgFixedVision::OnBtnLayer2() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}
