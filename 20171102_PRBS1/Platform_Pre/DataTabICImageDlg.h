//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATATABICIMAGEDLG_H__D42DC902_60E8_40FC_912B_2F50DDDE5940__INCLUDED_)
#define AFX_DATATABICIMAGEDLG_H__D42DC902_60E8_40FC_912B_2F50DDDE5940__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataTabICImageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataTabICImageDlg dialog

class CDataTabICImageDlg : public CDialog
{
// Construction
public:
	CDataTabICImageDlg(int iSel, CWnd* pParent = NULL);   // standard constructor
	int m_iWorkSide;

// Dialog Data
	//{{AFX_DATA(CDataTabICImageDlg)
	enum { IDD = IDD_DATA_TAB_IMAGE };
	CBtnEnh	m_sTabImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataTabICImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataTabICImageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTabDataCancel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATATABICIMAGEDLG_H__D42DC902_60E8_40FC_912B_2F50DDDE5940__INCLUDED_)
