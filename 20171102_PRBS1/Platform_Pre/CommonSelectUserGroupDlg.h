//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMONSELECTUSERGROUPDLG_H__F48E10AF_6422_49D9_9811_722B49BFF2A4__INCLUDED_)
#define AFX_COMMONSELECTUSERGROUPDLG_H__F48E10AF_6422_49D9_9811_722B49BFF2A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonSelectUserGroupDlg.h : header file
//

#include "MSystemData.h"

class MLog;

/////////////////////////////////////////////////////////////////////////////
// CCommonSelectUserGroupDlg dialog

class CCommonSelectUserGroupDlg : public CDialog
{
// Construction
public:
	CCommonSelectUserGroupDlg(MLog* pLogMng, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommonSelectUserGroupDlg)
	enum { IDD = IDD_COMMON_SELECT_USERGROUP_DIALOG };
	CBtnEnh	m_btnEngineer;
	CBtnEnh	m_btnMaker;
	CBtnEnh	m_btnOperator;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonSelectUserGroupDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonSelectUserGroupDlg)
	afx_msg void OnOperator();
	afx_msg void OnEngineer();
	afx_msg void OnMaker();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePassword();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MSystemData* m_plnkSystemData;
	MLog* m_plnkLogMng;
	HWND m_hTitleViewWnd;

	BOOL		checkPassword(EUserGroup eLoginGroup);
	BOOL		changePassword();
	CString		encodePassword(CString strCode);

	void		displayLogMessage(CString strUserGroup);

public:
	static CString DecodePassword(CString strCode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONSELECTUSERGROUPDLG_H__F48E10AF_6422_49D9_9811_722B49BFF2A4__INCLUDED_)
