//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_LOGCONFIGDLG_H__3382A35D_B54A_415B_B8F2_8D65EF64E8DE__INCLUDED_)
#define AFX_LOGCONFIGDLG_H__3382A35D_B54A_415B_B8F2_8D65EF64E8DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogConfigDlg dialog
class MSystemData;

class CLogConfigDlg : public CDialog
{
// Construction
public:
	CLogConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogConfigDlg)
	enum { IDD = IDD_LOG_CONFIG };
	CBtnEnh	m_ctrlBtnNormal;
	CBtnEnh	m_ctrlBtnTactTime;
	CBtnEnh	m_ctrlBtnWarning;
	CBtnEnh	m_ctrlBtnError;
//170427 JSH.s
	CBtnEnh	m_ctrlBtnEqTactTime;
//170427 JSH.e
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CLogConfigDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnTactTime();
	afx_msg void OnError();
	afx_msg void OnWarning();
	afx_msg void OnNormal();
//170427 JSH.s
	afx_msg void OnEqTactTime();
//170427 JSH.e
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BYTE m_ucLevel;
	MSystemData* m_plnkSystemData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGCONFIGDLG_H__3382A35D_B54A_415B_B8F2_8D65EF64E8DE__INCLUDED_)
