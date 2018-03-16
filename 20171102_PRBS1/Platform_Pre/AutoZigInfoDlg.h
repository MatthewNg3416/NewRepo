//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_AUTOZIGINFODLG_H__4D7957E1_04D8_45CC_8356_2141DF8F7418__INCLUDED_)
#define AFX_AUTOZIGINFODLG_H__4D7957E1_04D8_45CC_8356_2141DF8F7418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoZigInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoZigInfoDlg dialog

class MSystemData;
class MRFID;
class MTabFeeder;
class MCtrlTabFeeder;
class MLCNet;

class CAutoZigInfoDlg : public CDialog
{
// Construction
public:
	CAutoZigInfoDlg(int iFeederSel, CWnd* pParent = NULL);   // standard constructor

	int m_iFeederSel;
	CBrush m_brBckColor;
	int m_uiTimerID;
	MRFID* m_pRFID;
	MCtrlTabFeeder*	m_plnkCtrlTabFeeder;
	MTabFeeder*	m_plnkTabFeeder;
	MLCNet*	m_plnkLCNet;
	MSystemData* m_plnkSystemData;
	void DisplayRFIDInformation();

// Dialog Data
	//{{AFX_DATA(CAutoZigInfoDlg)
	enum { IDD = IDD_AUTO_ZIG_INFO };
	//CMSFlexGrid	m_FlexGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoZigInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoZigInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickExit();
	afx_msg void OnClickCheckDockIn();
	afx_msg void OnClickDataRead();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOZIGINFODLG_H__4D7957E1_04D8_45CC_8356_2141DF8F7418__INCLUDED_)
