#if !defined(AFX_DLGSHEETTACTTIME_H__607817D3_BA5C_44A8_BB81_5A086CEF98BA__INCLUDED_)
#define AFX_DLGSHEETTACTTIME_H__607817D3_BA5C_44A8_BB81_5A086CEF98BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSheetTactTime.h : header file
//
#include "PageTabMounterTact.h"
#include "PageTabCarrierTact.h"
#include "PageEtcTact.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSheetTactTime dialog

class CDlgSheetTactTime : public CDialog
{
private:
	HWND m_hAutoMainWnd;

	CPropertySheet	m_sheet;
	CPageTabMounterTact*	m_pPageTabMounterTact;
	CPageTabCarrierTact*	m_pPageTabCarrierTact;
	CPageEtcTact*			m_pPageEtcTact;

	void				createSheet();

// Construction
public:
	void				RenewTactTimeCode();

public:
	CDlgSheetTactTime(CWnd* pParent = NULL);   // standard constructor
	~CDlgSheetTactTime();
// Dialog Data
	//{{AFX_DATA(CDlgSheetTactTime)
	enum { IDD = IDD_DLG_SHEET_TACT_TIMER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSheetTactTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSheetTactTime)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnClickExit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHEETTACTTIME_H__607817D3_BA5C_44A8_BB81_5A086CEF98BA__INCLUDED_)
