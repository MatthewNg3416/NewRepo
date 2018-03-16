//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_AUTOREPAIRTABSELECTDLG_H__4B634CC1_B9CF_4C68_A2FA_761B3B39FAE0__INCLUDED_)
#define AFX_AUTOREPAIRTABSELECTDLG_H__4B634CC1_B9CF_4C68_A2FA_761B3B39FAE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoRepairTabSelectDlg.h : header file
//
#include "DefSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoRepairTabSelectDlg dialog

class MPanelData;
class MProcessData;
class ITI_PanelAligner;

class CAutoRepairTabSelectDlg : public CDialog
{
private:
	UINT m_uiTimerID;
	CBtnEnh	m_btnSelect[DEF_MAX_TAB];
	int m_iWorkSide;

	MPanelData* m_plnkPanelData;
	MProcessData* m_plnkProcessData;

	ITI_PanelAligner* m_plnkITIPanelAligner;

	void changeTabSelect(int iSel);
	int getNewWorkSide(int iNextWorkSide);

// Construction
public:
	CAutoRepairTabSelectDlg(CWnd* pParent = NULL);   // standard constructor

	void Start();
	void Terminate();

// Dialog Data
	//{{AFX_DATA(CAutoRepairTabSelectDlg)
	enum { IDD = IDD_AUTO_REPAIR_TAB_SELECT };
	CBtnEnh	m_btnNext;
	CBtnEnh	m_btnContinue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoRepairTabSelectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoRepairTabSelectDlg)
	afx_msg void OnClickTabNext();
	afx_msg void OnClickOn1();
	afx_msg void OnClickOn2();
	afx_msg void OnClickOn3();
	afx_msg void OnClickOn4();
	afx_msg void OnClickOn5();
	afx_msg void OnClickOn6();
	afx_msg void OnClickOn7();
	afx_msg void OnClickOn8();
	afx_msg void OnClickOn9();
	afx_msg void OnClickOn10();
	afx_msg void OnClickOn11();
	afx_msg void OnClickOn12();
	afx_msg void OnClickOn13();
	afx_msg void OnClickOn14();
	afx_msg void OnClickOn15();
	afx_msg void OnClickOn16();
	afx_msg void OnClickRepairRunContinue();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickOn17();
	afx_msg void OnClickOn18();
	afx_msg void OnClickOn19();
	afx_msg void OnClickOn20();
	afx_msg void OnClickOn21();
	afx_msg void OnClickOn22();
	afx_msg void OnClickOn23();
	afx_msg void OnClickOn24();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOREPAIRTABSELECTDLG_H__4B634CC1_B9CF_4C68_A2FA_761B3B39FAE0__INCLUDED_)
