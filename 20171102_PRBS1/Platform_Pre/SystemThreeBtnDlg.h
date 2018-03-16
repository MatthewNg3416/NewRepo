#if !defined(AFX_SYSTEMTHREEBTNDLG_H__12855780_9B20_403B_916A_350040613B6E__INCLUDED_)
#define AFX_SYSTEMTHREEBTNDLG_H__12855780_9B20_403B_916A_350040613B6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemThreeBtnDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemThreeBtnDlg dialog

class CSystemThreeBtnDlg : public CDialog
{
// Construction
public:
	CBrush m_brBckColor;
	CSystemThreeBtnDlg(CString strTitle, CString strOne, CString strTwo, CString strThree,CWnd* pParent = NULL);   // standard constructor
	CString m_strTitle;
	CString m_strOne;
	CString m_strTwo;
	CString m_strThree;
// Dialog Data
	//{{AFX_DATA(CSystemThreeBtnDlg)
	enum { IDD = IDD_DATA_THREEBTN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemThreeBtnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemThreeBtnDlg)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTHREEBTNDLG_H__12855780_9B20_403B_916A_350040613B6E__INCLUDED_)
