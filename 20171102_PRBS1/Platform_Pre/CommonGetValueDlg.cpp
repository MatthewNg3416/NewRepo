// GetValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "CommonGetValueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonGetValueDlg dialog


CCommonGetValueDlg::CCommonGetValueDlg(int maxchar, WCHAR* getnum, WCHAR* title,CWnd* pParent, BOOL pwd, BOOL btndisable)
	: CDialog(CCommonGetValueDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonGetValueDlg)
	//}}AFX_DATA_INIT
	m_intMaxChar = maxchar;
	m_strGetVal = getnum;
	m_strWndText = title;
	m_bpwd = pwd;
	m_bbtndisable = btndisable;
}


void CCommonGetValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonGetValueDlg)
	DDX_Control(pDX, IDC_DISP_VAL, m_sDispValue);
	DDX_Control(pDX, IDB_KEY_DOT, m_bDot);
	DDX_Control(pDX, IDB_KEY_EQUAL, m_bEqual);
	DDX_Control(pDX, IDB_KEY_COLON, m_bColon);
	DDX_Control(pDX, IDB_KEY_WON, m_bWon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonGetValueDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonGetValueDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonGetValueDlg message handlers

BEGIN_EVENTSINK_MAP(CCommonGetValueDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommonGetValueDlg)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_OK, -600 /* Click */, OnKeyOk, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_ESC, -600 /* Click */, OnKeyEsc, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_BACK, -600 /* Click */, OnKeyBack, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_CLEAR, -600 /* Click */, OnKeyClear, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_1, -600 /* Click */, OnKey1, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_2, -600 /* Click */, OnKey2, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_3, -600 /* Click */, OnKey3, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_4, -600 /* Click */, OnKey4, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_5, -600 /* Click */, OnKey5, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_6, -600 /* Click */, OnKey6, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_7, -600 /* Click */, OnKey7, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_8, -600 /* Click */, OnKey8, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_9, -600 /* Click */, OnKey9, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_0, -600 /* Click */, OnKey0, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_Q, -600 /* Click */, OnKeyQ, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_W, -600 /* Click */, OnKeyW, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_E, -600 /* Click */, OnKeyE, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_R, -600 /* Click */, OnKeyR, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_T, -600 /* Click */, OnKeyT, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_Y, -600 /* Click */, OnKeyY, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_U, -600 /* Click */, OnKeyU, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_I, -600 /* Click */, OnKeyI, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_O, -600 /* Click */, OnKeyO, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_P, -600 /* Click */, OnKeyP, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_A, -600 /* Click */, OnKeyA, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_S, -600 /* Click */, OnKeyS, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_D, -600 /* Click */, OnKeyD, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_F, -600 /* Click */, OnKeyF, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_G, -600 /* Click */, OnKeyG, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_H, -600 /* Click */, OnKeyH, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_J, -600 /* Click */, OnKeyJ, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_K, -600 /* Click */, OnKeyK, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_L, -600 /* Click */, OnKeyL, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_Z, -600 /* Click */, OnKeyZ, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_X, -600 /* Click */, OnKeyX, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_C, -600 /* Click */, OnKeyC, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_V, -600 /* Click */, OnKeyV, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_B, -600 /* Click */, OnKeyB, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_N, -600 /* Click */, OnKeyN, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_M, -600 /* Click */, OnKeyM, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_EQUAL, -600 /* Click */, OnKeyEqual, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_WON, -600 /* Click */, OnKeyWon, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_COLON, -600 /* Click */, OnKeyColon, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_DOT, -600 /* Click */, OnKeyDot, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_DASH, -600 /* Click */, OnKeyDash, VTS_NONE)
	ON_EVENT(CCommonGetValueDlg, IDB_KEY_UNDER_BAR, -600 /* Click */, OnKeyUnderBar, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCommonGetValueDlg::BtnDisable()
{
	// TODO: Add your control notification handler code here
	m_bDot.EnableWindow(FALSE);
	m_bColon.EnableWindow(FALSE);
	m_bWon.EnableWindow(FALSE);
	m_bEqual.EnableWindow(FALSE);
}

void CCommonGetValueDlg::OnKeyOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CCommonGetValueDlg::OnKeyEsc() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CCommonGetValueDlg::OnKeyBack() 
{
	// TODO: Add your control notification handler code here
	int len = m_strGetVal.GetLength();
	if (len)
	{
		m_strGetVal.SetAt(len-1,' ');
		m_strGetVal.TrimRight();
		UpdateDisplay();
	}
}

void CCommonGetValueDlg::OnKeyClear() 
{
	// TODO: Add your control notification handler code here
	m_strGetVal = "";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey1() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "1";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey2() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "2";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey3() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "3";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey4() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "4";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey5() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "5";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey6() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "6";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey7() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "7";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey8() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "8";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey9() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "9";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKey0() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "0";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyQ() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "Q";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyW() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "W";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyE() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "E";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyR() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "R";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyT() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "T";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyY() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "Y";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyU() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "U";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyI() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "I";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyO() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "O";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyP() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "P";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyA() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "A";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyS() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "S";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyD() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "D";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyF() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "F";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyG() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "G";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyH() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "H";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyJ() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "J";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyK() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "K";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyL() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "L";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyZ() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "Z";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyX() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "X";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyC() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "C";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyV() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "V";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyB() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "B";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyN() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "N";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyM() 
{
	// TODO: Add your control notification handler code here
	if (!VerifyMaxChar()) return;

	m_strGetVal += "M";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyEqual() 
{
	// TODO: Add your control notification handler code here
	if (m_bpwd) return;

	if (!VerifyMaxChar()) return;

	m_strGetVal += "=";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyWon() 
{
	// TODO: Add your control notification handler code here
	if (m_bpwd) return;

	if (!VerifyMaxChar()) return;

	m_strGetVal += "\\";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyColon() 
{
	// TODO: Add your control notification handler code here
	if (m_bpwd) return;

	if (!VerifyMaxChar()) return;

	m_strGetVal += ":";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyDot() 
{
	// TODO: Add your control notification handler code here
	if (m_bpwd) return;

	if (!VerifyMaxChar()) return;

	m_strGetVal += ".";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyDash() 
{
	// TODO: Add your control notification handler code here
	if (m_bpwd) return;

	if (!VerifyMaxChar()) return;

	m_strGetVal += "-";	
	UpdateDisplay();
}

void CCommonGetValueDlg::OnKeyUnderBar() 
{
	// TODO: Add your control notification handler code here
	if (m_bpwd) return;

	if (!VerifyMaxChar()) return;

	m_strGetVal += "_";	
	UpdateDisplay();
}

void CCommonGetValueDlg::UpdateDisplay(COLORREF bkcolor)
{
/*    CClientDC dc (this);
    dc.DrawEdge(m_rect, EDGE_SUNKEN, BF_RECT);

    CFont* pOldFont = dc.SelectObject(GetFont());
    CSize size = dc.GetTextExtent(m_strGetVal);

    CRect rect = m_rect;
    rect.InflateRect(-2, -2);
//    int x = rect.right - size.cx - m_cxChar;		// Right Align
    int x = rect.left + 2;							// Left Align
    int y = rect.top + ((rect.Height() - m_cyChar) / 2);

	dc.SetBkColor(bkcolor);

    dc.ExtTextOut(x, y, ETO_OPAQUE, rect, m_strGetVal, NULL);
    dc.SelectObject(pOldFont);
*/
	int len;
	CString bufstr;

	if (m_bpwd)
	{
		len = m_strGetVal.GetLength();
		if (len <= 0) 
		{
			bufstr = "";
			m_sDispValue.SetCaption(bufstr);
			return;
		}
		for (int i = 0; i < len; i++)
			bufstr += "*";

		m_sDispValue.SetCaption(bufstr);
	}
	else m_sDispValue.SetCaption(m_strGetVal);
}

BOOL CCommonGetValueDlg::VerifyMaxChar()
{
	if (m_strGetVal.GetLength() >= m_intMaxChar)
		return FALSE;
	else
		return TRUE;
}

void CCommonGetValueDlg::InitStaticDispWnd()
{
    CStatic* pRect = (CStatic*)GetDlgItem(IDC_DISP_VAL);
    pRect->GetWindowRect(&m_rect);
    pRect->DestroyWindow();
    ScreenToClient(&m_rect);

    TEXTMETRIC tm;
    CClientDC dc(this);
    dc.GetTextMetrics(&tm);
    m_cxChar = tm.tmAveCharWidth;
    m_cyChar = tm.tmHeight - tm.tmDescent;
}

CString CCommonGetValueDlg::GetStringValue()
{
	return m_strGetVal;
}

void CCommonGetValueDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	UpdateDisplay();
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CCommonGetValueDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_intMaxChar <= 0)
		m_intMaxChar = 1;

	if (m_bbtndisable) 
		BtnDisable();

	SetWindowText(m_strWndText);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
