#if !defined(AFX_SYSTEMBTNDLG_H__1A03416B_8C3A_43AE_9E9D_19D77DAAE386__INCLUDED_)
#define AFX_SYSTEMBTNDLG_H__1A03416B_8C3A_43AE_9E9D_19D77DAAE386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemBtnDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SystemBtnDlg dialog

class SystemBtnDlg : public CDialog
{
// Construction

public:
	CBrush m_brBckColor;
	SystemBtnDlg(CString strTitle, CString strOne, CString strTwo, CString strThree, CString strFour, CString strFive, CWnd* pParent = NULL);   // standard constructor
	CString m_strTitle;
	CString m_strOne;
	CString m_strTwo;
	CString m_strThree;
	CString m_strFour;
	CString	m_strFive;
// Dialog Data
	//{{AFX_DATA(SystemBtnDlg)
	enum { IDD = IDD_DATA_FIVEBTN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SystemBtnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SystemBtnDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickFiveButton1();
	afx_msg void OnClickFiveButton2();
	afx_msg void OnClickFiveButton3();
	afx_msg void OnClickFiveButton4();
	afx_msg void OnClickFiveButton5();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMBTNDLG_H__1A03416B_8C3A_43AE_9E9D_19D77DAAE386__INCLUDED_)


