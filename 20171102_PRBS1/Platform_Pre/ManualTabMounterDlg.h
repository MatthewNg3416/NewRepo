//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALTABMOUNTERDLG_H__FD335009_0029_4E7E_9AA9_5A099DB26A62__INCLUDED_)
#define AFX_MANUALTABMOUNTERDLG_H__FD335009_0029_4E7E_9AA9_5A099DB26A62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualTabMounterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManualTabMounterDlg dialog
#include "DefSystem.h"

const int DEF_MANUAL_TABMOUNTER_MAX_BUTTON = 4;

class MSystemData;
class MTabMounter;

class CManualTabMounterDlg : public CDialog
{
private:
	CBtnEnh	m_btnVacOn[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnVacOff[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnUp[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnDown[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnUpPusher[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnDownPusher[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];

	CBtnEnh	m_btnRVacOn[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnRVacOff[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnRUp[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnRDown[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnRUpPusher[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	CBtnEnh	m_btnRDownPusher[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	
	CBtnEnh	m_btnFrontRepeatTest;
	CBtnEnh	m_btnRearRepeatTest;

	BOOL m_bVacOnSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bVacOffSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bUpSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bDownSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bUpPusherSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bDownPusherSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];

	BOOL m_bRVacOnSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bRVacOffSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bRUpSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bRDownSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bRUpPusherSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];
	BOOL m_bRDownPusherSts[DEF_MANUAL_TABMOUNTER_MAX_BUTTON];

	void vacuumOn(int iMounterNo);
	void vacuumOff(int iMounterNo);
	void up(int iMounterNo);
	void down(int iMounterNo);
	void upPusher(int iMounterNo);
	void downPusher(int iMounterNo);

	void vacuumOnR(int iMounterNo);
	void vacuumOffR(int iMounterNo);
	void upR(int iMounterNo);
	void downR(int iMounterNo);
	void upRPusher(int iMounterNo);
	void downRPusher(int iMounterNo);

	CBrush m_brBckColor;
	MSystemData* m_plnkSystemData;
	MTabMounter* m_plnkTabMounter[DEF_MAX_TABMOUNTER];
	MTabMounter* m_plnkRTabMounter[DEF_MAX_TABMOUNTER];

// Construction
public:
	CManualTabMounterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualTabMounterDlg)
	enum { IDD = IDD_MANUAL_TABMOUNTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualTabMounterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManualTabMounterDlg)
	afx_msg void OnClickExit();
	afx_msg void OnTabMounterVacOn1();
	afx_msg void OnTabMounterVacOn2();
	afx_msg void OnTabMounterVacOn3();
	afx_msg void OnTabMounterVacOn4();
	afx_msg void OnTabMounterVacOn5();
	afx_msg void OnTabMounterVacOn6();
	afx_msg void OnTabMounterVacOn7();
	afx_msg void OnTabMounterVacOn8();
	afx_msg void OnTabMounterVacOff1();
	afx_msg void OnTabMounterVacOff2();
	afx_msg void OnTabMounterVacOff3();
	afx_msg void OnTabMounterVacOff4();
	afx_msg void OnTabMounterVacOff5();
	afx_msg void OnTabMounterVacOff6();
	afx_msg void OnTabMounterVacOff7();
	afx_msg void OnTabMounterVacOff8();
	afx_msg void OnTabMounterUp1();
	afx_msg void OnTabMounterUp2();
	afx_msg void OnTabMounterUp3();
	afx_msg void OnTabMounterUp4();
	afx_msg void OnTabMounterUp5();
	afx_msg void OnTabMounterUp6();
	afx_msg void OnTabMounterUp7();
	afx_msg void OnTabMounterUp8();
	afx_msg void OnTabMounterDown1();
	afx_msg void OnTabMounterDown2();
	afx_msg void OnTabMounterDown3();
	afx_msg void OnTabMounterDown4();
	afx_msg void OnTabMounterDown5();
	afx_msg void OnTabMounterDown6();
	afx_msg void OnTabMounterDown7();
	afx_msg void OnTabMounterDown8();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnTabmounterFAllUp();
	afx_msg void OnTabmounterFAllDown();
	afx_msg void OnPaint();
	afx_msg void OnClickTabmounterRAllUp();
	afx_msg void OnClickTabmounterRAllDown();
	afx_msg void OnClickPusherFAllUp();
	afx_msg void OnClickPusherFAllDown();
	afx_msg void OnClickFPusherUp1();
	afx_msg void OnClickFPusherDown1();
	afx_msg void OnClickFPusherUp2();
	afx_msg void OnClickFPusherDown2();
	afx_msg void OnClickFPusherUp3();
	afx_msg void OnClickFPusherDown3();
	afx_msg void OnClickFPusherUp4();
	afx_msg void OnClickFPusherDown4();
	afx_msg void OnClickPusherRAllUp();
	afx_msg void OnClickPusherRAllDown();
	afx_msg void OnClickRPusherUp1();
	afx_msg void OnClickRPusherDown1();
	afx_msg void OnClickRPusherUp2();
	afx_msg void OnClickRPusherDown2();
	afx_msg void OnClickRPusherUp3();
	afx_msg void OnClickRPusherDown3();
	afx_msg void OnClickRPusherUp4();
	afx_msg void OnClickRPusherDown4();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	void ClickManualTabcarrierUd();
	void ClickFrontRepeatTest();
	void ClickRearRepeatTest();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALTABMOUNTERDLG_H__FD335009_0029_4E7E_9AA9_5A099DB26A62__INCLUDED_)
