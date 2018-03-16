#if !defined(AFX_DLGFIXEDVISION_H__73C3C5E1_1055_4B6E_893C_4C59D59D329C__INCLUDED_)
#define AFX_DLGFIXEDVISION_H__73C3C5E1_1055_4B6E_893C_4C59D59D329C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFixedVision1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFixedVision dialog
const	int	DEF_MAX_DLG_FIXED_VIEW = 12;

class	IVision;

class CDlgFixedVision : public CDialog
{
private:
	int m_rgiViewID[DEF_MAX_DLG_FIXED_VIEW];
	IVision*		m_plnkVision;

public:
	void CreateFixedView();

// Construction
public:
	CDlgFixedVision(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFixedVision)
	enum { IDD = IDD_DLGFIXEDVISION_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFixedVision)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFixedVision)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg void OnBtnNotlayer();
	afx_msg void OnBtnLayer();
	afx_msg void OnBtnLayer2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFIXEDVISION_H__73C3C5E1_1055_4B6E_893C_4C59D59D329C__INCLUDED_)
