// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__19589088_EC08_4392_B075_E651A1BF5F00__INCLUDED_)
#define AFX_MAINFRM_H__19589088_EC08_4392_B075_E651A1BF5F00__INCLUDED_

#include "MultiViewSplitter.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int DEF_MAX_PANE = 10;

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Operations
public:
	void SwitchMainViews(int viewNumber);
	void SetStatusText(int iOrder, CString st);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMultiViewSplitter	m_wndSplitter;
	int					m_nView[DEF_MAX_PANE];

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar		m_wndStatusBar;
	CRuntimeClass*	m_pTitleForm;
	CRuntimeClass*	m_pAutoMainForm;
	CRuntimeClass*	m_pManualMainForm;
	CRuntimeClass*	m_pDataMainForm;
	CRuntimeClass*	m_pTeachMainForm;
	CRuntimeClass*	m_pLogMainForm;
	CRuntimeClass*	m_pBottomForm;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__19589088_EC08_4392_B075_E651A1BF5F00__INCLUDED_)
