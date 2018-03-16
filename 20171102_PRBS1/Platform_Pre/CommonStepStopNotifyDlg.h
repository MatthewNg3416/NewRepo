//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMONSTEPSTOPNOTIFYDLG_H__B07776D7_A6C7_438F_9309_F4FCABF81E73__INCLUDED_)
#define AFX_COMMONSTEPSTOPNOTIFYDLG_H__B07776D7_A6C7_438F_9309_F4FCABF81E73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonStepStopNotifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommonStepStopNotifyDlg dialog

class CCommonStepStopNotifyDlg : public CDialog
{
// Construction
public:
	void SetMessage(CString strMsg);
	CCommonStepStopNotifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommonStepStopNotifyDlg)
	enum { IDD = IDD_COMMON_STEPSTOP_NOTIFY };
	CBtnEnh	m_Msg;
	CBtnEnh	m_Icon;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonStepStopNotifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonStepStopNotifyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONSTEPSTOPNOTIFYDLG_H__B07776D7_A6C7_438F_9309_F4FCABF81E73__INCLUDED_)
