#if !defined(AFX_AUTOOPERATIONCMDGROUPDLG_H__582330FA_D4A1_4FF0_890A_D74C1A456560__INCLUDED_)
#define AFX_AUTOOPERATIONCMDGROUPDLG_H__582330FA_D4A1_4FF0_890A_D74C1A456560__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoOperationCmdGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoOperationCmdGroupDlg dialog

class CAutoOperationCmdGroupDlg : public CDialog
{
// Construction
public:
	CAutoOperationCmdGroupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoOperationCmdGroupDlg)
	enum { IDD = IDD_AUTO_OPERATION_CMD_GROUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoOperationCmdGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	HWND m_hAutoMainWnd;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoOperationCmdGroupDlg)
	afx_msg void OnMoveTabMounter();
	afx_msg void OnRemoveTabIC();
	afx_msg void OnCylinderMoveTimeDisplay();
	afx_msg void OnExit();
	afx_msg void OnMoveInspectionCamera();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOOPERATIONCMDGROUPDLG_H__582330FA_D4A1_4FF0_890A_D74C1A456560__INCLUDED_)
