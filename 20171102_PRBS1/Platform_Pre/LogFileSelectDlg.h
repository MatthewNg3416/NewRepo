//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_LOGFILESELECTDLG_H__D62A122D_6546_446E_B843_890DC6DD8A44__INCLUDED_)
#define AFX_LOGFILESELECTDLG_H__D62A122D_6546_446E_B843_890DC6DD8A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogFileSelectDlg.h : header file
//

//#include "MinLabel.h"
#include "MSystemInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CLogFileSelectDlg dialog

class CLogMainForm;

class CLogFileSelectDlg : public CDialog
{
// Construction
public:
	CLogFileSelectDlg(CWnd* pParent = NULL);   // standard constructor

	void InitializeLogListBox();
	void InitializeLabel();
	void InitializeButton();

// Dialog Data
	//{{AFX_DATA(CLogFileSelectDlg)
	enum { IDD = IDD_LOG_FILE_SELECT };
	CListBox	m_listLogFile;
	CBtnEnh		m_valueSelectedLog;
	CBtnEnh		m_labelLogListTitle;
	CBtnEnh		m_btnLogListUp;
	CBtnEnh		m_btnLogListDown;
	CBtnEnh		m_btnLogListPgUp;
	CBtnEnh		m_btnLogListPgDown;
	CBtnEnh		m_btnLogSelection;
	CBtnEnh		m_btnExit;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogFileSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	CString			m_strSelectedFile;
	CString			m_strLogList[256];
	int				m_iCurrentSel;
	int				m_iNumList;
	CLogMainForm*	m_pParent;

	MSystemInfo		*m_pSystemInfo;
	SObjectInfo		m_sInfoObject[DEF_MAX_OBJECT_INFO_NO];
	// Generated message map functions
	//{{AFX_MSG(CLogFileSelectDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnPageUp();
	afx_msg void OnPageDown();
	afx_msg void OnList();
	afx_msg void OnSelchangeList();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGFILESELECTDLG_H__D62A122D_6546_446E_B843_890DC6DD8A44__INCLUDED_)
