// MyWaitWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MyWnd.h"

#include <string.h>
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
#define IDC_MESSAGE			10
#define IDC_ERRNUM			11
#define IDC_MESSAGETITLE	12
#define IDC_ERRNUMTITLE		13

/////////////////////////////////////////////////////////////////////////////
// CMyWnd

CMyWnd::CMyWnd(int dshow)
{
	m_hbrush = NULL;
	CommonConstruct();
    m_strTitle = "Error Message";
	m_CancelFlag = FALSE;
	m_OkFlag = FALSE;
	m_Show = dshow;
    Create(NULL);
}

CMyWnd::CMyWnd(CWnd* pParent)
{
	m_hbrush = NULL;
	CommonConstruct();
    m_strTitle = "Error Message";
    Create(pParent);
}

void CMyWnd::CommonConstruct()
{
	BkColor = RGB(255,0,255);
    m_strTitle = _T("Error Message");
}

CMyWnd::~CMyWnd()
{
	if( m_hbrush ) ::DeleteObject(m_hbrush);

   DestroyWindow();
}

BEGIN_MESSAGE_MAP(CMyWnd, CWnd)
	//{{AFX_MSG_MAP(CMyWnd)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyWnd message handlers

//BOOL CMyWnd::Create(CWnd* pParent, LPCTSTR pszTitle)
BOOL CMyWnd::Create(CWnd* pParent)
{
	// TODO: Add your specialized code here and/or call the base class
    BOOL bSuccess;

    // Register window class
    CString csClassName = AfxRegisterWndClass(CS_OWNDC|CS_HREDRAW|CS_VREDRAW,
                                              ::LoadCursor(NULL, IDC_WAIT),
                                              CBrush(::GetSysColor(COLOR_BTNFACE)));


    // If no parent supplied then try and get a pointer to it anyway
    if (!pParent) pParent = AfxGetMainWnd();

    // Create popup window
    bSuccess = CreateEx(WS_EX_DLGMODALFRAME,            // Extended style
                        csClassName,                    // Classname
//                      pszTitle,                       // Title
						      m_strTitle,
                        WS_POPUP|WS_BORDER|WS_CAPTION,  // style
                        0,0,                            // position - updated soon.
                        390,130,                        // Size - updated soon
                        pParent->GetSafeHwnd(),         // handle to parent
                        0,                              // No menu
                        NULL);    
    if (!bSuccess) return FALSE;

    // Now create the controls
    CRect TempRect(0,0,10,10);

    bSuccess = m_Message.Create( _T(""), WS_CHILD|WS_VISIBLE|SS_NOPREFIX|SS_LEFTNOWORDWRAP, 
								 TempRect, this, IDC_MESSAGE );

    if (!bSuccess) return FALSE;

    bSuccess = m_ErrNum.Create( _T(""), WS_CHILD|WS_VISIBLE|SS_NOPREFIX,//_LEFTNOWORDWRAP, 
							    TempRect, this, IDC_ERRNUM );

    if (!bSuccess) return FALSE;

    bSuccess = m_MessageTitle.Create( _T(""), WS_CHILD|WS_VISIBLE|SS_NOPREFIX,//_LEFTNOWORDWRAP, 
									 TempRect, this, IDC_MESSAGETITLE );

    if (!bSuccess) return FALSE;

    bSuccess = m_ErrNumTitle.Create( _T(""), WS_CHILD|WS_VISIBLE|SS_NOPREFIX,//_LEFTNOWORDWRAP, 
									 TempRect, this, IDC_ERRNUMTITLE );

    if (!bSuccess) return FALSE;

    GetWindowRect(TempRect); 
	TempRect.right = 415;
	TempRect.bottom = 190;
	MoveWindow(TempRect);

    TempRect.SetRect( 10, 50, 110, 70 );
    m_MessageTitle.MoveWindow(TempRect);

    TempRect.SetRect( 10, 75, 400, 145 );
    m_Message.MoveWindow(TempRect);

	TempRect.SetRect( 10, 10, 110, 30);
    m_ErrNumTitle.MoveWindow(TempRect);

	TempRect.SetRect( 120, 10, 400, 30);
    m_ErrNum.MoveWindow(TempRect);

	m_ErrNumTitle.SetWindowText(_T("Error Number :"));
	m_MessageTitle.SetWindowText(_T("Contents :"));

    // Center and show window
	if(m_Show)
	{
	    CenterWindow();
		Show();
	}

    return TRUE;
}

void CMyWnd::DoGradientFill(CDC* pDC, CRect* pRect)
{
    CBrush* pBrush[64];

    for (int i=0; i<64; i++)
        pBrush[i] = new CBrush(RGB (0, 255 - (i * 4), 255 - (i * 4)));

    int nWidth = pRect->Width();
    int nHeight = pRect->Height();
    CRect rect;

    for (int i=0; i<nHeight; i++) 
	{
        rect.SetRect (0, i, nWidth, i + 1);
        pDC->FillRect (&rect, pBrush[(i * 63) / nHeight]);
    }

    for (int i=0; i<64; i++)
        delete pBrush[i];
}

void CMyWnd::UpdateDisplay(COLORREF bkcolor)
{
	CRect mrect;
	m_Message.GetWindowRect(&mrect);
    mrect.InflateRect(2, 2);
    ScreenToClient(&mrect);
    CClientDC dc(this);
	dc.SetBkColor(bkcolor);
    dc.DrawEdge(mrect, EDGE_SUNKEN, BF_RECT);

	m_ErrNum.GetWindowRect(&mrect);
    mrect.InflateRect(2, 2);
    ScreenToClient(&mrect);
	dc.SetBkColor(bkcolor);
    dc.DrawEdge(mrect, EDGE_SUNKEN, BF_RECT);
}

void CMyWnd::Show()
{
    if (!::IsWindow(GetSafeHwnd()))
        return;

    if ((::GetWindowLong(GetSafeHwnd(), GWL_STYLE) & WS_VISIBLE) == 0)
    {
        ModifyStyle(0, WS_VISIBLE);
        ShowWindow(SW_SHOWNA);
        RedrawWindow(NULL,NULL,RDW_ERASE|RDW_FRAME|RDW_INVALIDATE|RDW_ALLCHILDREN);
    }
//	SetFocus();
}

void CMyWnd::DoSolidFill(CDC* pDC, CRect* pRect)
{
	CBrush pBrush( BkColor );
	pDC->FillRect( pRect,&pBrush );
}

BOOL CMyWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CBrush backBrush(GetSysColor(COLOR_BTNFACE));
    CBrush* pOldBrush = pDC->SelectObject(&backBrush);
    CRect rect;
    pDC->GetClipBox(&rect);     // Erase the area needed
    pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOldBrush);

//	DoGradientFill(pDC, &rect);
	DoSolidFill(pDC, &rect);

    return TRUE;
}

void CMyWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	UpdateDisplay(RGB(255,0,255));
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH CMyWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	HWND hwnd = pWnd->GetSafeHwnd();
	if(hwnd==GetDlgItem(IDC_MESSAGE)->m_hWnd)
	{
		COLORREF textColor = RGB(0,0,0);		// TEXT Color
		COLORREF backColor = RGB(255,255,0);	// Background Color

		if( m_hbrush ) ::DeleteObject(m_hbrush);

		pDC->SetTextColor(textColor);
		pDC->SetBkColor(backColor);
		pDC->SetBkMode(TRANSPARENT);			// 글자를 쓸때 배경이 변경 안되게..

		m_hbrush = ::CreateSolidBrush(backColor);	// Control의 바탕색

		return m_hbrush;
	}
	if(hwnd==GetDlgItem(IDC_ERRNUM)->m_hWnd)
	{
		COLORREF textColor = RGB(255,255,0);		// TEXT Color
		COLORREF backColor = RGB(0,0,255);	// Background Color

		if( m_hbrush ) ::DeleteObject(m_hbrush);

		pDC->SetTextColor(textColor);
		pDC->SetBkColor(backColor);
		pDC->SetBkMode(TRANSPARENT);			// 글자를 쓸때 배경이 변경 안되게..

		m_hbrush = ::CreateSolidBrush(backColor);	// Control의 바탕색

		return m_hbrush;
	}
	else if(hwnd==GetDlgItem(IDC_MESSAGETITLE)->m_hWnd)
	{
		COLORREF textColor = RGB(0,0,0);		// TEXT Color
		COLORREF backColor = BkColor;	// Background Color

		if( m_hbrush ) ::DeleteObject(m_hbrush);

		pDC->SetTextColor(textColor);
		pDC->SetBkColor(backColor);
		pDC->SetBkMode(TRANSPARENT);			// 글자를 쓸때 배경이 변경 안되게..

		m_hbrush = ::CreateSolidBrush(backColor);	// Control의 바탕색

		return m_hbrush;
	}
	else if(hwnd==GetDlgItem(IDC_ERRNUMTITLE)->m_hWnd)
	{
		COLORREF textColor = RGB(0,0,0);		// TEXT Color
		COLORREF backColor = BkColor;	// Background Color

		if( m_hbrush ) ::DeleteObject(m_hbrush);

		pDC->SetTextColor(textColor);
		pDC->SetBkColor(backColor);
		pDC->SetBkMode(TRANSPARENT);			// 글자를 쓸때 배경이 변경 안되게..

		m_hbrush = ::CreateSolidBrush(backColor);	// Control의 바탕색

		return m_hbrush;
	}
	
	return hbr; // TODO: Return a different brush if the default is not desired
}


void CMyWnd::SetErrorNumber(LPCTSTR fmt, ...)
{
    if ( !::IsWindow(GetSafeHwnd()) ) return;

    va_list args;

    va_start(args, fmt);
    _vstprintf(m_cErrNum, fmt, args);
    va_end(args);
	m_ErrNum.SetWindowText(m_cErrNum);
}

void CMyWnd::SetMessage(LPCTSTR fmt, ...)
{
    if ( !::IsWindow(GetSafeHwnd()) ) return;

    va_list args;

    va_start(args, fmt);
    _vstprintf(m_cMessage, fmt, args);
    va_end(args);

    m_Message.SetWindowText(m_cMessage);
}

void CMyWnd::PeekAndPump()
{
	MSG msg;

//	SetFocus();

	while( ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) )
	{
		if( msg.message == WM_CHAR )
		{
			switch(msg.wParam)
			{
				case VK_ESCAPE : m_CancelFlag = TRUE; break;
				case VK_BACK :
				case VK_F12 : m_OkFlag = TRUE; break;
				default : break;
			}
		}

		if(!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			return;
		}
	}
}
