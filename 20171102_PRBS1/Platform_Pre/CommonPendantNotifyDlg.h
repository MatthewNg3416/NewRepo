//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMONPENDANTNOTIFYDLG_H__A76DEE8B_0012_4D87_96B2_C15D8E1039C0__INCLUDED_)
#define AFX_COMMONPENDANTNOTIFYDLG_H__A76DEE8B_0012_4D87_96B2_C15D8E1039C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonPendantNotifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommonPendantNotifyDlg dialog

class CCommonPendantNotifyDlg : public CDialog 
{
// Construction
public:
	CCommonPendantNotifyDlg(CWnd* pParent = NULL);   // standard constructor
	void SetMessage(CString strMsg);
	CBrush m_brBckColor;

// Dialog Data
	//{{AFX_DATA(CCommonPendantNotifyDlg)
	enum { IDD = IDD_COMMON_PENDANT_NOTIFY };
	CBtnEnh	m_Msg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonPendantNotifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonPendantNotifyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONPENDANTNOTIFYDLG_H__A76DEE8B_0012_4D87_96B2_C15D8E1039C0__INCLUDED_)
