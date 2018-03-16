//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALPANELTRANSFERDLG_H__1467EA06_9557_42CC_8CBD_345725B767BA__INCLUDED_)
#define AFX_MANUALPANELTRANSFERDLG_H__1467EA06_9557_42CC_8CBD_345725B767BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualPanelTransferDlg.h : header file
//
#include "IIO.h"
/////////////////////////////////////////////////////////////////////////////
// CManualPanelTransferDlg dialog

class MPanelTransfer;
//class IIO;

#include "MTickTimer.h"

class CManualPanelTransferDlg : public CDialog
{

private:
	const int m_iTransferID;
	UINT m_uiTimerID;

	MPanelTransfer*	m_plnkPanelTransfer;
	IIO* m_plnkIIO;

	MTickTimer m_AirTimer;
	WCHAR m_szTimeValue[100];

// Construction
public:
	CManualPanelTransferDlg(int iTransferID, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualPanelTransferDlg)
	enum { IDD = IDD_MANUAL_PANEL_TRANSFER };
	CBtnEnh	m_btnPanelTransferUp;
	CBtnEnh	m_btnPanelTransferDown;
	CBtnEnh	m_btnPanelTransferVacuumOn;
	CBtnEnh	m_btnPanelTransferVacuumOff;
	CBtnEnh	m_lblPanelDetect;
	CBtnEnh	m_btnTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualPanelTransferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManualPanelTransferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickPanelTransferUp();
	afx_msg void OnClickPanelTransferDown();
	afx_msg void OnClickPanelTransferVacuumOn();
	afx_msg void OnClickPanelTransferVacuumOff();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickExit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetDlgCaption();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALPANELTRANSFERDLG_H__1467EA06_9557_42CC_8CBD_345725B767BA__INCLUDED_)
