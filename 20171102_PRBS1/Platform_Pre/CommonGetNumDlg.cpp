// GetNumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "CommonGetNumDlg.h"

#include "common.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonGetNumDlg dialog


CCommonGetNumDlg::CCommonGetNumDlg(int maxchar, WCHAR* getnum, WCHAR* title, CWnd* pParent, BOOL pwd)
	: CDialog(CCommonGetNumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonGetNumDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bCalu = FALSE;
	m_nCalu = 0;

	m_FirstFlag = TRUE;
	m_intMaxChar =  maxchar;
	m_bpwd = pwd;

	/*if( m_bpwd )
		m_strGetNum = "";
	else */ //modeless
		m_strGetNum = getnum;

	if( m_strGetNum.Find('-') == -1 )
	{
		m_numFlag=TRUE;
	}
	else
	{
		m_numFlag=FALSE;
	}

	if( m_strGetNum.Find('.') == -1 )
	{
		m_nPointNum = 0;
	}
	else
	{
		m_nPointNum = 6;
	}


	m_strWndText = title;

	m_strOrg = m_strGetNum;
	
	if(m_bpwd)BOOL Create(CWnd* pParentWnd) ;
}


void CCommonGetNumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonGetNumDlg)
	DDX_Control(pDX, IDC_PRESENT, m_sSetValue);
	DDX_Control(pDX, IDC_MODIFY, m_sDispNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonGetNumDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonGetNumDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonGetNumDlg message handlers

BEGIN_EVENTSINK_MAP(CCommonGetNumDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommonGetNumDlg)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_PLUS, -600 /* Click */, OnBtnPlus, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_MINUS, -600 /* Click */, OnBtnMinus, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_EQUAL, -600 /* Click */, OnBtnEqual, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_DOT, -600 /* Click */, OnBtnDot, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_SIGN, -600 /* Click */, OnBtnSign, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_BACK, -600 /* Click */, OnBtnBack, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_CLEAR, -600 /* Click */, OnBtnClear, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_OK, -600 /* Click */, OnBtnOk, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDB_BTN_ESC, -600 /* Click */, OnBtnEsc, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_1, -600 /* Click */, OnBtn1, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_2, -600 /* Click */, OnBtn2, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_3, -600 /* Click */, OnBtn3, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_4, -600 /* Click */, OnBtn4, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_5, -600 /* Click */, OnBtn5, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_6, -600 /* Click */, OnBtn6, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_7, -600 /* Click */, OnBtn7, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_8, -600 /* Click */, OnBtn8, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_9, -600 /* Click */, OnBtn9, VTS_NONE)
	ON_EVENT(CCommonGetNumDlg, IDC_BTN_0, -600 /* Click */, OnBtn0, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCommonGetNumDlg::OnBtnPlus() 
{
	// TODO: Add your control notification handler code here
	if( !m_FirstFlag ) return;

	m_bCalu = TRUE;
	m_nCalu = TRUE;

	m_FirstFlag = TRUE;

	m_strOrg = m_strGetNum;
	m_strGetNum = "";
	m_sSetValue.SetCaption(m_strOrg);
	UpdateDisplay();	
}

void CCommonGetNumDlg::OnBtnMinus() 
{
	// TODO: Add your control notification handler code here
	if( !m_FirstFlag ) return;

	m_bCalu = TRUE;
	m_nCalu = FALSE;

	m_FirstFlag = TRUE;

	m_strOrg = m_strGetNum;
	m_strGetNum = "";
	m_sSetValue.SetCaption(m_strOrg);
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtnEqual() 
{
	// TODO: Add your control notification handler code here
	double fi, se, rel;
	WCHAR buf[100];

	if( !m_bCalu ) return;

	fi = wcstod(m_strOrg, NULL);
	se = wcstod(m_strGetNum, NULL);

	if( m_nCalu )
		rel = (fi) + (se);
	else
		rel = (fi) - (se);

	if( !m_nPointNum )
		swprintf(buf, _T("%.0f"), rel);
	else
		swprintf(buf, _T("%.6f"), rel);

	m_strGetNum = buf;
	UpdateDisplay();

	m_FirstFlag = TRUE;
	m_bCalu = FALSE;
}

void CCommonGetNumDlg::OnBtnDot() 
{
	// TODO: Add your control notification handler code here
	if( m_bpwd ) return;

	if( !VerifyMaxChar() ) return;

	m_strGetNum += ".";	
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtnSign() 
{
	// TODO: Add your control notification handler code here
	if( m_bpwd ) return;

	if( !m_strGetNum.GetLength() ) return;

	if(m_numFlag == TRUE)
	{
		m_strGetNum.Insert(0,'-');
		m_numFlag=FALSE;
	}
	else
	{
		m_strGetNum.Remove('-');
		m_numFlag=TRUE;
	}

	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtnBack() 
{
	// TODO: Add your control notification handler code here
	m_FirstFlag = FALSE;

	int len = m_strGetNum.GetLength();
	if(len)
	{
		m_strGetNum.SetAt(len-1,' ');
		m_strGetNum.TrimRight();
		UpdateDisplay();
	}
}

void CCommonGetNumDlg::OnBtnClear() 
{
	// TODO: Add your control notification handler code here
	m_strGetNum = "";	
	m_numFlag=TRUE;
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
	if(m_bpwd) 	{
//		GetNum_rtn = TRUE;
//		lstrcpy(codebuf, m_strGetNum);
		DestroyWindow();
	}
	else CDialog::OnOK();
}

void CCommonGetNumDlg::OnBtnEsc() 
{
	// TODO: Add your control notification handler code here
	if(m_bpwd) {	
//		GetNum_rtn = TRUE;
//		lstrcpy(codebuf, "0");
		DestroyWindow();

	}
	else CDialog::OnCancel();
}

void CCommonGetNumDlg::OnBtn1() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "1";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn2() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "2";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn3() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "3";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn4() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "4";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn5() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "5";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn6() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "6";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn7() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "7";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn8() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "8";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn9() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "9";
	UpdateDisplay();
}

void CCommonGetNumDlg::OnBtn0() 
{
	// TODO: Add your control notification handler code here
	if( m_FirstFlag )
	{
		m_FirstFlag = FALSE;
		m_numFlag=TRUE;
		m_strGetNum = "";
	}

	if( !VerifyMaxChar() ) return;

	m_strGetNum += "0";
	UpdateDisplay();
}

void CCommonGetNumDlg::InitStaticDispWnd()
{
    CStatic* pRect = (CStatic*)GetDlgItem(IDC_MODIFY);
    pRect->GetWindowRect(&m_rect);
    pRect->DestroyWindow();
    ScreenToClient(&m_rect);

    TEXTMETRIC tm;
    CClientDC dc(this);
    dc.GetTextMetrics(&tm);
    m_cxChar = tm.tmAveCharWidth;
    m_cyChar = tm.tmHeight - tm.tmDescent;
}

BOOL CCommonGetNumDlg::VerifyMaxChar()
{
	if( m_strGetNum.GetLength() >= m_intMaxChar ) return FALSE;
	else return TRUE;
}

void CCommonGetNumDlg::UpdateDisplay(COLORREF bkcolor)
{
/*    CClientDC dc (this);
    dc.DrawEdge(m_rect, EDGE_SUNKEN, BF_RECT);

    CFont* pOldFont = dc.SelectObject( GetFont() );
    CSize size = dc.GetTextExtent(m_strGetNum);

    CRect rect = m_rect;
    rect.InflateRect(-2, -2);
    int x = rect.right - size.cx - m_cxChar;		// Right Align
//    int x = rect.left + 2;							// Left Align
    int y = rect.top + ((rect.Height() - m_cyChar) / 2);

	dc.SetBkColor(bkcolor);

    dc.ExtTextOut(x, y, ETO_OPAQUE, rect, m_strGetNum, NULL);
    dc.SelectObject(pOldFont);
*/

	CString szTextTemp;
	ReadDisplayMessage(SUB_DLG, _T("CommonGetNumDlg_1"), &szTextTemp);
	//«ˆ¿Á ∞™
	GetDlgItem(IDS_PRESENT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("CommonGetNumDlg_2"), &szTextTemp);
	//¿˚øÎ ∞™
	GetDlgItem(IDS_MODIFY)->SetWindowText(szTextTemp);

	int len, i;
	CString bufstr;

	i = 0; len = 0;
	/* 2003.02.04 √÷±§«ı...
	if( m_bpwd )
	{
		len = m_strGetNum.GetLength();
		if( len <= 0 )
		{
			bufstr = "";
			m_sDispNum.SetText(bufstr);
			return;
		}
		for (int i=0;i<len;i++) bufstr += "*";
		m_sDispNum.SetText(bufstr);
	}
	else m_sDispNum.SetText(m_strGetNum);
	*/
	m_sDispNum.SetCaption(m_strGetNum);
}

void CCommonGetNumDlg::SetWindowTitle(LPCTSTR title)
{
	m_strWndText = title;
}

void CCommonGetNumDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	UpdateDisplay();
	SetWindowText(m_strWndText);	
	
	// Do not call CDialog::OnPaint() for painting messages
}
