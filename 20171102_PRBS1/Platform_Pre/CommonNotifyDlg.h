//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMONNOTIFYDLG_H__12C315E1_8C73_48DC_A3B8_67E4DF6CCEAB__INCLUDED_)
#define AFX_COMMONNOTIFYDLG_H__12C315E1_8C73_48DC_A3B8_67E4DF6CCEAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonNotifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommonNotifyDlg dialog

class CCommonNotifyDlg : public CDialog
{
// Construction
public:
	CCommonNotifyDlg(CWnd* pParent = NULL);   // standard constructor

	void SetTitle(CString strTitle);
	void SetMessage(CString strMsg);
	void PeekAndPump();

// Dialog Data
	//{{AFX_DATA(CCommonNotifyDlg)
	enum { IDD = IDD_COMMON_NOTIFY };
	CBtnEnh	m_ctrlLblMsg;
	CBtnEnh	m_ctrlLblPicture;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonNotifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	CString m_strMsg;

	// Generated message map functions
	//{{AFX_MSG(CCommonNotifyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONNOTIFYDLG_H__12C315E1_8C73_48DC_A3B8_67E4DF6CCEAB__INCLUDED_)
