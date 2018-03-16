#if !defined(AFX_AUTOMATERIALCMDGROUPDLG_H__DF954259_FBF3_4FA3_98A4_B850FC1C55F7__INCLUDED_)
#define AFX_AUTOMATERIALCMDGROUPDLG_H__DF954259_FBF3_4FA3_98A4_B850FC1C55F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoMaterialCmdGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoMaterialCmdGroupDlg dialog

class CAutoMaterialCmdGroupDlg : public CDialog
{
// Construction
public:
	CAutoMaterialCmdGroupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoMaterialCmdGroupDlg)
	enum { IDD = IDD_AUTO_MATERIAL_CMD_GROUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoMaterialCmdGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	HWND m_hAutoMainWnd;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoMaterialCmdGroupDlg)
	afx_msg void OnPanelScrap();
	afx_msg void OnPanelUnscrap();
	afx_msg void OnMaterialinfo();
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMaterialchange();
	afx_msg void OnClickMaterialinfoView();
	afx_msg void OnClickProductPlanView();
	afx_msg void OnClickAutoHoming();
	afx_msg void OnClickAutoHomingR();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOMATERIALCMDGROUPDLG_H__DF954259_FBF3_4FA3_98A4_B850FC1C55F7__INCLUDED_)
