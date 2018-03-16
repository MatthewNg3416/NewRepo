//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MODELNDEVICEIDLISTDLG_H__8DB6F6EE_D8FB_4299_9455_99B497DF875F__INCLUDED_)
#define AFX_MODELNDEVICEIDLISTDLG_H__8DB6F6EE_D8FB_4299_9455_99B497DF875F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelNDeviceIDListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelNDeviceIDListDlg dialog

class CModelNDeviceIDListDlg : public CDialog
{
// Construction
public:
	CModelNDeviceIDListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelNDeviceIDListDlg)
	enum { IDD = IDD_MODEL_DEVICEID_LIST };
	CBtnEnh	m_btnModelNameDisplay;
	CBtnEnh	m_btnDeviceIDDisplay;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelNDeviceIDListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelNDeviceIDListDlg)
	afx_msg void OnClickExit();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELNDEVICEIDLISTDLG_H__8DB6F6EE_D8FB_4299_9455_99B497DF875F__INCLUDED_)
