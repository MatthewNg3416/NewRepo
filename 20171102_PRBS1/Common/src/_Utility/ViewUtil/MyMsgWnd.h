#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyWaitWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyMsgWnd window
class CMyMsgWnd : public CWnd
{
// Construction
public:
	HBRUSH m_hbrush;

	CMyMsgWnd(BOOL pos = TRUE);
	CMyMsgWnd(CWnd* pParent, BOOL pos = TRUE);
   BOOL Create(CWnd* pParent);			   

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyMsgWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_MostTopFlag;
	void ResetFlag();
	BOOL m_OkFlag;
	BOOL m_CancelFlag;
	void PeekAndPump();
	void UpdateDisplay(COLORREF bkcolor);
	void SetMessage(LPCTSTR fmt, ...);
	void Show();
	virtual ~CMyMsgWnd();

	// Generated message map functions
protected:
	BOOL m_pos;
	COLORREF BkColor;
	CString m_strTitle;
	void CommonConstruct();
	CStatic m_Message;
	//{{AFX_MSG(CMyMsgWnd)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	TCHAR m_cMessage[512];
	void DoSolidFill(CDC* pDC, CRect* pRect);
	void DoGradientFill(CDC*, CRect*);
};

/////////////////////////////////////////////////////////////////////////////
