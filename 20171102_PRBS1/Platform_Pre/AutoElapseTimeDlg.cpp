// AutoElapseTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "ICylinder.h"
#include "MPlatformOlbSystem.h"
#include "AutoElapseTimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int DEF_RAW_NUM = 36;

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// AutoElapseTimeDlg dialog

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

AutoElapseTimeDlg::AutoElapseTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AutoElapseTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(AutoElapseTimeDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
		m_plnkCylinder[i] = MOlbSystemPre.GetCylinderComponent(i);
}


void AutoElapseTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AutoElapseTimeDlg)
	//DDX_Control(pDX, IDC_MSFLEXGRID_CYLINDER_TIME, m_TimeElapse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AutoElapseTimeDlg, CDialog)
	//{{AFX_MSG_MAP(AutoElapseTimeDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AutoElapseTimeDlg message handlers

void AutoElapseTimeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int iRow1, iCol1, iRow2, iCol2;
	CString strTime;
	for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
	{
		if (i < DEF_RAW_NUM)
		{
			iRow1 = 1 + i;	iCol1 = 1;
			iRow2 = 1 + i;	iCol2 = 2;
		}
		else
		{
			iRow1 = 1 + i - DEF_RAW_NUM;	iCol1 = 4;
			iRow2 = 1 + i - DEF_RAW_NUM;	iCol2 = 5;
		}
		strTime.Format(_T("%.2f"), m_plnkCylinder[i]->GetUpElapseTime());
		//m_TimeElapse.SetTextMatrix(iRow1, iCol1, strTime); 
		strTime.Format(_T("%.2f"), m_plnkCylinder[i]->GetDownElapseTime());
		//m_TimeElapse.SetTextMatrix(iRow2, iCol2, strTime); 
	}
	
	CDialog::OnTimer(nIDEvent);
}

void AutoElapseTimeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL AutoElapseTimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// 반투명하게 만드는 루틴.
	SLWA pSetLayeredWindowAttributes = NULL;			// 함수포인터 선언, 초기화.
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL"));	// 인스턴스 얻음.
	pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes"); // 함수포인터 얻음.
	HWND hwnd = this->m_hWnd; // 다이얼로그의 핸들 얻음.
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | DEF_WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * 70) / 90, DEF_LWA_ALPHA);	// 다이얼로그 반투명하게 만듬.

	SetWindowPos(NULL, 30, 265, 0, 0,SWP_NOZORDER|SWP_NOSIZE);	// 투명윈도우의 위치 지정
	
	int iRow, iCol;
	for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
	{
		if (i < DEF_RAW_NUM)
		{
			iRow = 1 + i;	iCol = 0;
		}
		else
		{
			iRow = 1 + i - DEF_RAW_NUM;	iCol = 3;
		}
		//m_TimeElapse.SetTextMatrix(iRow, iCol, m_plnkCylinder[i]->GetCylinderName()); 
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;

//실린더 동작 시간 표시
	ReadDisplayMessage(MAIN_AUTO, _T("AutoElapseTimeDlg_1"), &szTextTemp);
	SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(AutoElapseTimeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(AutoElapseTimeDlg)
	ON_EVENT(AutoElapseTimeDlg, IDC_MSFLEXGRID_CYLINDER_TIME, -600 /* Click */, OnHide, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void AutoElapseTimeDlg::StartTimer()
{
	m_uiTimerID = SetTimer(1, 1000, NULL);
}

void AutoElapseTimeDlg::OnHide() 
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_HIDE);
	KillTimer(m_uiTimerID);
}
