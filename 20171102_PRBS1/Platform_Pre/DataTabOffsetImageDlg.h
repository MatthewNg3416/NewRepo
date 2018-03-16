#if !defined(AFX_DATATABOFFSETIMAGEDLG_H__D4A2676F_353D_4E63_99FA_1FFBFB8B00FA__INCLUDED_)
#define AFX_DATATABOFFSETIMAGEDLG_H__D4A2676F_353D_4E63_99FA_1FFBFB8B00FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataTabOffsetImageDlg.h : header file
//
#include "btnenh.h"

/////////////////////////////////////////////////////////////////////////////
// CDataTabOffsetImageDlg dialog

class CDataTabOffsetImageDlg : public CDialog
{
// Construction
public:
	CDataTabOffsetImageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataTabOffsetImageDlg)
	enum { IDD = IDD_DATATABOFFSETIMAGEDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	CBtnEnh	m_sTabImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataTabOffsetImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataTabOffsetImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickCancel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATATABOFFSETIMAGEDLG_H__D4A2676F_353D_4E63_99FA_1FFBFB8B00FA__INCLUDED_)
