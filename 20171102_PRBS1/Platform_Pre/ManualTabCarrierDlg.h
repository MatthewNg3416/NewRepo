//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALTABCARRIER_H__23010790_B067_4F0E_BD64_7C6E82E7378A__INCLUDED_)
#define AFX_MANUALTABCARRIER_H__23010790_B067_4F0E_BD64_7C6E82E7378A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualTabCarrierDlg.h : header file
//
#include "DefSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CManualTabCarrierDlg dialog
const int DEF_MANUAL_TABCARRIER_MAX_BUTTON = 4;

class MTabCarrier;


class CManualTabCarrierDlg : public CDialog
{
// Construction
private:
	int m_iGroupNo;

public:
	CManualTabCarrierDlg(int iWorkSide, CWnd* pParent = NULL);   // standard constructor

	MTabCarrier*		m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	
	UINT	m_uiTimerID;

// Dialog Data
	//{{AFX_DATA(CManualTabCarrierDlg)
	enum { IDD = IDD_MANUAL_TABCARRIER };
	CBtnEnh m_bVac1On;
	CBtnEnh m_bVac1Off;
	CBtnEnh m_bVac2On;
	CBtnEnh m_bVac2Off;
	CBtnEnh	m_bVac3Off;
	CBtnEnh	m_bVac3On;
	CBtnEnh	m_bVac4Off;
	CBtnEnh	m_bVac4On;
	
	CBtnEnh m_bHandUp;
	CBtnEnh m_bHandDown;
	CBtnEnh m_bHand2Up;
	CBtnEnh m_bHand2Down;
	CBtnEnh	m_bHandUp2;
	CBtnEnh	m_bHandDown2;

	CBtnEnh m_bForDumpBasket;
	CBtnEnh m_bBackDumpBasket;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualTabCarrierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CManualTabCarrierDlg)
	afx_msg void OnClickVac1On();
	afx_msg void OnClickVac1Off();
	afx_msg void OnClickVac2On();
	afx_msg void OnClickVac2Off();
	afx_msg void OnClickVac3On();
	afx_msg void OnClickVac3Off();
	afx_msg void OnClickVac4On();
	afx_msg void OnClickVac4Off();
	afx_msg void OnClickHandUp();
	afx_msg void OnClickHandDown();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickExit();
	afx_msg void OnPaint();
	afx_msg void OnClickForward();
	afx_msg void OnClickBackward();
	afx_msg void OnClickForwardDumpBasket();
	afx_msg void OnClickBackwardDumpBasket();
	afx_msg void OnClickUp2();
	afx_msg void OnClickDown2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALTABCARRIER_H__23010790_B067_4F0E_BD64_7C6E82E7378A__INCLUDED_)
