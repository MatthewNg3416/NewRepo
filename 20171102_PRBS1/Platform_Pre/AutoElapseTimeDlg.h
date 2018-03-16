//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_AUTOELAPSETIMEDLG_H__E66917DA_4F1B_46F9_9FBB_7C47E86B88EE__INCLUDED_)
#define AFX_AUTOELAPSETIMEDLG_H__E66917DA_4F1B_46F9_9FBB_7C47E86B88EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoElapseTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AutoElapseTimeDlg dialog
#include "DefActuatorPreBonder.h"

class ICylinder;

class AutoElapseTimeDlg : public CDialog
{
// Construction
public:
	AutoElapseTimeDlg(CWnd* pParent = NULL);   // standard constructor
	void StartTimer();
// Dialog Data
	//{{AFX_DATA(AutoElapseTimeDlg)
	enum { IDD = IDD_AUTO_ELAPSE_TIME };
	//CMSFlexGrid	m_TimeElapse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AutoElapseTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;
	UINT m_uiTimerID;
	ICylinder* m_plnkCylinder[DEF_CYL_MAX_CYLINDER];

	// Generated message map functions
	//{{AFX_MSG(AutoElapseTimeDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnHide();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOELAPSETIMEDLG_H__E66917DA_4F1B_46F9_9FBB_7C47E86B88EE__INCLUDED_)
