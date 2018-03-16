//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMONOPCALLDLG_H__6B5C5B10_C61A_4AB9_942F_328F653EAB42__INCLUDED_)
#define AFX_COMMONOPCALLDLG_H__6B5C5B10_C61A_4AB9_942F_328F653EAB42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonOPCallDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommonOPCallDlg dialog

class CCommonOPCallDlg : public CDialog
{
// Construction
public:
	CCommonOPCallDlg(CWnd* pParent = NULL);   // standard constructor
	void SetMessage(CString str);
	CStringArray	m_strArray;
	CBrush m_brBckColor; 
	int m_iSelect;
	void SetSelect(int iMode);

// Dialog Data
	//{{AFX_DATA(CCommonOPCallDlg)
	enum { IDD = IDD_COMMON_OPCALL_DLG };
	CBtnEnh	m_editLabl;
	CBtnEnh	m_MyIcon;
	CBtnEnh	m_bSelect1;
	CBtnEnh	m_bSelect2;
	CBtnEnh	m_bOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonOPCallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonOPCallDlg)
	afx_msg void OnClickMyok();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickSelect1();
	afx_msg void OnClickSelect2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONOPCALLDLG_H__6B5C5B10_C61A_4AB9_942F_328F653EAB42__INCLUDED_)
