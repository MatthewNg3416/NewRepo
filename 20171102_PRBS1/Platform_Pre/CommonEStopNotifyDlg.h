//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMONESTOPNOTIFYDLG_H__B07776D7_A6C7_438F_9309_F4FCABF81E73__INCLUDED_)
#define AFX_COMMONESTOPNOTIFYDLG_H__B07776D7_A6C7_438F_9309_F4FCABF81E73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonEStopNotifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommonEStopNotifyDlg dialog

class CCommonEStopNotifyDlg : public CDialog
{
// Construction
public:
	CCommonEStopNotifyDlg(CWnd* pParent = NULL);   // standard constructor
	void SetMessage(CString strMsg);
	CBrush m_brBckColor;
	
// Dialog Data
	//{{AFX_DATA(CCommonEStopNotifyDlg)
	enum { IDD = IDD_COMMON_ESTOP_NOTIFY };
	CBtnEnh	m_Msg;
	CBtnEnh	m_Icon;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonEStopNotifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonEStopNotifyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONESTOPNOTIFYDLG_H__B07776D7_A6C7_438F_9309_F4FCABF81E73__INCLUDED_)
