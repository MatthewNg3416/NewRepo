//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_PANELVALIDATIONCHECKDLG_H__12343030_53F3_439D_B049_ACE394E76ADD__INCLUDED_)
#define AFX_PANELVALIDATIONCHECKDLG_H__12343030_53F3_439D_B049_ACE394E76ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanelValidationCheckDlg.h : header file
//
#include "DefSystem.h"

const int DEF_MAX_CHECK_NUMBER	= 32;

/////////////////////////////////////////////////////////////////////////////
// CPanelValidationCheckDlg dialog
class MSystemData;

class CPanelValidationCheckDlg : public CDialog
{
private:
	BOOL m_CheckSelect[DEF_MAX_CHECK_NUMBER];
	CString m_sCheckItem[DEF_MAX_CHECK_NUMBER];

	void	selectPanelItem(int iIndex);
// Construction
public:
	CPanelValidationCheckDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPanelValidationCheckDlg)
	enum { IDD = IDD_PANEL_VALIDATION_CHECK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

//	BOOL m_CheckSelect[DEF_MAX_CHECK_NUMBER];

	MSystemData* m_plnkSystemData;

	UINT	m_uiTimerID;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelValidationCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CBtnEnh	m_bCheckSelect[DEF_MAX_CHECK_NUMBER];
	CBtnEnh	m_lblCheckItem[DEF_MAX_CHECK_NUMBER];
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPanelValidationCheckDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnClickCheckSelect1();
	afx_msg void OnClickCheckSelect2();
	afx_msg void OnClickCheckSelect3();
	afx_msg void OnClickCheckSelect4();
	afx_msg void OnClickCheckSelect5();
	afx_msg void OnClickCheckSelect6();
	afx_msg void OnClickCheckSelect7();
	afx_msg void OnClickCheckSelect8();
	afx_msg void OnClickCheckSelect9();
	afx_msg void OnClickCheckSelect10();
	afx_msg void OnClickCheckSelect11();
	afx_msg void OnClickCheckSelect12();
	afx_msg void OnClickCheckSelect13();
	afx_msg void OnClickCheckSelect14();
	afx_msg void OnClickCheckSelect15();
	afx_msg void OnClickCheckSelect16();
	afx_msg void OnClickCheckSelect17();
	afx_msg void OnClickCheckSelect18();
	afx_msg void OnClickCheckSelect19();
	afx_msg void OnClickCheckSelect20();
	afx_msg void OnClickCheckSelect21();
	afx_msg void OnClickCheckSelect22();
	afx_msg void OnClickCheckSelect23();
	afx_msg void OnClickCheckSelect24();
	afx_msg void OnClickCheckSelect25();
	afx_msg void OnClickCheckSelect26();
	afx_msg void OnClickCheckSelect27();
	afx_msg void OnClickCheckSelect28();
	afx_msg void OnClickCheckSelect29();
	afx_msg void OnClickCheckSelect30();
	afx_msg void OnClickCheckSelect31();
	afx_msg void OnClickCheckSelect32();
	afx_msg void OnClickCheckItem1();
	afx_msg void OnClickCheckItem2();
	afx_msg void OnClickCheckItem3();
	afx_msg void OnClickCheckItem4();
	afx_msg void OnClickCheckItem5();
	afx_msg void OnClickCheckItem6();
	afx_msg void OnClickCheckItem7();
	afx_msg void OnClickCheckItem8();
	afx_msg void OnClickCheckItem9();
	afx_msg void OnClickCheckItem10();
	afx_msg void OnClickCheckItem11();
	afx_msg void OnClickCheckItem12();
	afx_msg void OnClickCheckItem13();
	afx_msg void OnClickCheckItem14();
	afx_msg void OnClickCheckItem15();
	afx_msg void OnClickCheckItem16();
	afx_msg void OnClickCheckItem17();
	afx_msg void OnClickCheckItem18();
	afx_msg void OnClickCheckItem19();
	afx_msg void OnClickCheckItem20();
	afx_msg void OnClickCheckItem21();
	afx_msg void OnClickCheckItem22();
	afx_msg void OnClickCheckItem23();
	afx_msg void OnClickCheckItem24();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELVALIDATIONCHECKDLG_H__12343030_53F3_439D_B049_ACE394E76ADD__INCLUDED_)
