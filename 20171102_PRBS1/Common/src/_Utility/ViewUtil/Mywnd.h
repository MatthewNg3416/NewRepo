#if !defined(AFX_MYWAITWND_H__91392BC1_18C6_11D2_8D8F_00AA00A18C62__INCLUDED_)
#define AFX_MYWAITWND_H__91392BC1_18C6_11D2_8D8F_00AA00A18C62__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyWaitWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyWnd window

class CMyWnd : public CWnd
{
// Construction
public:
	HBRUSH m_hbrush;

	CMyWnd(int dshow = TRUE);
	CMyWnd(CWnd* pParent);
    BOOL Create(CWnd* pParent);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_OkFlag;
	int m_Show;
	BOOL m_CancelFlag;
	void PeekAndPump();
	void UpdateDisplay(COLORREF bkcolor);
	void SetMessage(LPCTSTR fmt, ...);
	void SetErrorNumber(LPCTSTR fmt, ...);
	void Show();
	virtual ~CMyWnd();

	// Generated message map functions
protected:
	COLORREF BkColor;
	CString m_strTitle;
	void CommonConstruct();
	CStatic m_Message;
	CStatic m_ErrNum;
	CStatic m_MessageTitle;
	CStatic m_ErrNumTitle;
	//{{AFX_MSG(CMyWnd)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	TCHAR m_cMessage[512];
	TCHAR m_cErrNum[20];
	void DoSolidFill(CDC* pDC, CRect* pRect);
	void DoGradientFill(CDC*, CRect*);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWAITWND_H__91392BC1_18C6_11D2_8D8F_00AA00A18C62__INCLUDED_)
