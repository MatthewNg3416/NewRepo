#if !defined(AFX_PMFULLDISPLAY_H__12BBBB54_DCE4_4C5A_BF84_DB22F9AA02C5__INCLUDED_)
#define AFX_PMFULLDISPLAY_H__12BBBB54_DCE4_4C5A_BF84_DB22F9AA02C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PMFullDisplay.h : header file
//
#include "MSystemData.h"

class MLog;

/////////////////////////////////////////////////////////////////////////////
// CPMFullDisplay dialog

class CPMFullDisplay : public CDialog
{
// Construction
public:
	CPMFullDisplay(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPMFullDisplay)
	enum { IDD = IDD_PM_MODE_FULL_DISPLAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPMFullDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPMFullDisplay)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickExit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MSystemData* m_plnkSystemData;
	MLog* m_plnkLogMng;

	BOOL		checkPassword();

public:
	static CString DecodePassword(CString strCode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMFULLDISPLAY_H__12BBBB54_DCE4_4C5A_BF84_DB22F9AA02C5__INCLUDED_)
