//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MODULEPORTIDDLG_H__3E005D6A_15AD_4911_88B7_CD0C1CA111E2__INCLUDED_)
#define AFX_MODULEPORTIDDLG_H__3E005D6A_15AD_4911_88B7_CD0C1CA111E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModulePortIDDlg1.h : header file
//

const int DEF_MAX_MODULE_NUMBER = 12;

class MSystemData;

/////////////////////////////////////////////////////////////////////////////
// CModulePortIDDlg dialog

class CModulePortIDDlg : public CDialog
{
// Construction
public:
	CModulePortIDDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModulePortIDDlg)
	enum { IDD = IDD_MODULE_PORT_ID_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	MSystemData*	m_plnkSystemData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModulePortIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CBtnEnh  m_bEQPID;
	CBtnEnh  m_bPortid[DEF_MAX_MODULE_NUMBER];

	CBtnEnh  m_sEQPID;
	CBtnEnh  m_lblPortid[DEF_MAX_MODULE_NUMBER];

	CString m_strEQPID;
	CString m_strPortID[DEF_MAX_MODULE_NUMBER];

	//20110811 SJ_HJG
	CString		m_strModuleID;
	CString		m_strModuleID2;
	CString		m_strModuleID3_1;
	CString		m_strModuleID3_2;
	CString		m_strModuleID3_3;
	CString		m_strModuleID3_4;
	UINT		m_iReportLevel;

	CBtnEnh	m_bReportLevel;
	CBtnEnh	m_bModuleID;
	CBtnEnh	m_bModuleID2;
	CBtnEnh	m_bModuleID3_1;
	CBtnEnh	m_bModuleID3_2;
	CBtnEnh	m_bModuleID3_3;
	CBtnEnh	m_bModuleID3_4;

	CBtnEnh	m_sReportLevel;
	CBtnEnh	m_sModuleID;
	CBtnEnh	m_sModuleID2;
	CBtnEnh	m_sModuleID3_1;
	CBtnEnh	m_sModuleID3_2;
	CBtnEnh	m_sModuleID3_3;
	CBtnEnh	m_sModuleID3_4;

	//____________________________
	WCHAR m_szOldValue[100];
	WCHAR m_szNewValue[100];

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModulePortIDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnClickModuleId();
	afx_msg void OnClickPortId();
	afx_msg void OnClickPortId2();
	afx_msg void OnClickPortId3();
	afx_msg void OnClickPortId4();
	afx_msg void OnClickPortId5();
	afx_msg void OnClickPortId6();
	afx_msg void OnClickPortId7();
	afx_msg void OnClickPortId8();
	afx_msg void OnClickPortId9();
	afx_msg void OnClickPortId10();
	afx_msg void OnClickModuleid();
	afx_msg void OnClickModuleid2();
	afx_msg void OnClickModuleid3();
	afx_msg void OnClickModuleid4();
	afx_msg void OnClickModuleid5();
	afx_msg void OnClickModuleid6();
	afx_msg void OnClickReportLevel();
	afx_msg void OnClickPortId11();
	afx_msg void OnClickPortId12();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULEPORTIDDLG_H__3E005D6A_15AD_4911_88B7_CD0C1CA111E2__INCLUDED_)
