//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALHANDLERDLG_H__A61AB104_EA14_4432_AB70_FCED72625C80__INCLUDED_)
#define AFX_MANUALHANDLERDLG_H__A61AB104_EA14_4432_AB70_FCED72625C80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualHandlerDlg.h : header file
//

#include "MHandler.h"
#include "MPanelData.h"
#include "MSystemData.h"

/////////////////////////////////////////////////////////////////////////////
// CManualHandlerDlg dialog

class CManualHandlerDlg : public CDialog
{
// Construction
public:
	CManualHandlerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualHandlerDlg)
	enum { IDD = IDD_MANUAL_HANDLER };
	CBtnEnh	m_ctrlBtnUnloadHandlerVacOn;
	CBtnEnh	m_ctrlBtnUnloadHandlerVacOff;
	CBtnEnh	m_ctrlBtnUnloadHandlerVacuumOn1;
	CBtnEnh	m_ctrlBtnUnloadHandlerVacuumOff1;
	CBtnEnh	m_ctrlBtnUnloadHandlerVacuumOn2;
	CBtnEnh	m_ctrlBtnUnloadHandlerVacuumOff2;
	CBtnEnh	m_ctrlBtnUnloadHandlerVacuumOn3;
	CBtnEnh	m_ctrlBtnUnloadHandlerVacuumOff3;
	CBtnEnh m_bCancel;
	//}}AFX_DATA

	UINT	m_uiTimerID;

	MHandler*		m_plnkUnloadHandler;	
	MPanelData*		m_plnkPanelData;
	MSystemData*	m_plnkSystemData;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualHandlerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush	m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CManualHandlerDlg)
	afx_msg void OnClickExit();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickUnloadHandlerVacOn1();
	afx_msg void OnClickUnloadHandlerVacOff1();
	afx_msg void OnClickUnloadHandlerVacOn2();
	afx_msg void OnClickUnloadHandlerVacOff2();
	afx_msg void OnClickUnloadHandlerVacOn3();
	afx_msg void OnClickUnloadHandlerVacOff3();
	afx_msg void OnClickUnloadHandlerVacOnAll();
	afx_msg void OnClickUnloadHandlerVacOffAll();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALHANDLERDLG_H__A61AB104_EA14_4432_AB70_FCED72625C80__INCLUDED_)
