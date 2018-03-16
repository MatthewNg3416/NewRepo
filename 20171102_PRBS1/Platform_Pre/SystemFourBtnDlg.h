#if !defined(AFX_SYSTEMFOURBTNDLG_H__11501CCE_3C72_4B79_A696_2F5C852565D3__INCLUDED_)
#define AFX_SYSTEMFOURBTNDLG_H__11501CCE_3C72_4B79_A696_2F5C852565D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemFourBtnDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemFourBtnDlg dialog

class CSystemFourBtnDlg : public CDialog
{
// Construction
public:
	CSystemFourBtnDlg(CString strTitle, CString strOne, CString strTwo, CString strThree, CString strFour, CWnd* pParent = NULL);   // standard constructor
	CBrush m_brBckColor;
	CString m_strTitle;
	CString m_strOne;
	CString m_strTwo;
	CString m_strThree;
	CString m_strFour;

// Dialog Data
	//{{AFX_DATA(CSystemFourBtnDlg)
	enum { IDD = IDD_SYSTEMFOURBTNDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemFourBtnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSystemFourBtnDlg)
	afx_msg void OnClickButton1();
	afx_msg void OnClickButton2();
	afx_msg void OnClickButton3();
	afx_msg void OnClickButton4();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMFOURBTNDLG_H__11501CCE_3C72_4B79_A696_2F5C852565D3__INCLUDED_)
