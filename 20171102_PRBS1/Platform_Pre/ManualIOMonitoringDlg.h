//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALIOMONITORINGDLG_H__2A41C545_2196_4ED5_92BA_47EA321A0FE4__INCLUDED_)
#define AFX_MANUALIOMONITORINGDLG_H__2A41C545_2196_4ED5_92BA_47EA321A0FE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualIOMonitoringDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManualIOMonitoringDlg dialog
#include "DefSystemConfig.h"
#include "IIO.h"


/*
//const int DEF_IOMONITOR_MAX_INPUT_PAGE		= 12;		
//const int DEF_IOMONITOR_MAX_OUTPUT_PAGE		= 11;		
const int DEF_IOMONITOR_MAX_INPUT_PAGE		= 30;//24;
const int DEF_IOMONITOR_MAX_OUTPUT_PAGE		= 31;//22;		
*/


#ifdef DEF_SOURCE_SYSTEM
const int DEF_IOMONITOR_MAX_INPUT_PAGE		= 17;	// 271 / 16 => 17
const int DEF_IOMONITOR_MAX_OUTPUT_PAGE		= 25;	// 399 / 16 => 25
#endif

#ifdef DEF_GATE_SYSTEM
const int DEF_IOMONITOR_MAX_INPUT_PAGE		= 17;	// 271 / 16 = 17
const int DEF_IOMONITOR_MAX_OUTPUT_PAGE		= 19;	// 303 / 16 = 19
#endif


//const int DEF_IOMONITOR_MAX_IO_PER_PAGE		= 32;
//const int DEF_IOMONITOR_MAX_TITLE_PER_PAGE	= 4;
const int DEF_IOMONITOR_MAX_IO_PER_PAGE		= 16;
const int DEF_IOMONITOR_MAX_TITLE_PER_PAGE	= 2;

const int DEF_IOMONITOR_MAX_IO_INPUT		= 512;
const int DEF_IOMONITOR_MAX_IO_OUTPUT		= 512;

class CManualIOMonitoringDlg : public CDialog
{
// Construction
public:
	int ToggleOutput(int iAddr);
	void UpdateButton();
	void UpdateDisplay_In();
	void UpdateDisplay_Out();
	void InitButtons();

	CManualIOMonitoringDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualIOMonitoringDlg)
	enum { IDD = IDD_MANUAL_IO_MONITOR };
	CBtnEnh	m_bNext;
	CBtnEnh	m_bPrev;
	CBtnEnh	m_bNext2;
	CBtnEnh	m_bPrev2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualIOMonitoringDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;
//	CBtnEnh	m_lblIOTitle[DEF_IOMONITOR_MAX_TITLE_PER_PAGE];
//	CBtnEnh	m_btnIO[DEF_IOMONITOR_MAX_IO_PER_PAGE];
	CBtnEnh	m_lblIOTitle_In[DEF_IOMONITOR_MAX_TITLE_PER_PAGE];
	CBtnEnh	m_btnIO_In[DEF_IOMONITOR_MAX_IO_PER_PAGE];
	CBtnEnh	m_lblIOTitle_Out[DEF_IOMONITOR_MAX_TITLE_PER_PAGE];
	CBtnEnh	m_btnIO_Out[DEF_IOMONITOR_MAX_IO_PER_PAGE];

//	int	m_iSelectIO;	
//	int	m_iCurrentPage;
//	int m_iMaxPage;
//	int m_iAddrBase;
	int	m_iSelectIO_In;	
	int	m_iCurrentPage_In;
	int m_iMaxPage_In;
	int m_iAddrBase_In;

	int	m_iSelectIO_Out;	
	int	m_iCurrentPage_Out;
	int m_iMaxPage_Out;
	int m_iAddrBase_Out;

	UINT m_uiTimerID;

	IIO	*m_plnkIO;

	/** IO Data */
	SIOTABLE m_IODataInput[DEF_IOMONITOR_MAX_IO_INPUT];
	SIOTABLE m_IODataOutput[DEF_IOMONITOR_MAX_IO_OUTPUT];

	// Generated message map functions
	//{{AFX_MSG(CManualIOMonitoringDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnTop();
	afx_msg void OnBottom();
	afx_msg void OnIo10();
	afx_msg void OnIo11();
	afx_msg void OnIo12();
	afx_msg void OnIo13();
	afx_msg void OnIo14();
	afx_msg void OnIo15();
	afx_msg void OnIo16();
	afx_msg void OnIo17();
	afx_msg void OnIo18();
	afx_msg void OnIo19();
	afx_msg void OnIo1A();
	afx_msg void OnIo1B();
	afx_msg void OnIo1C();
	afx_msg void OnIo1D();
	afx_msg void OnIo1E();
	afx_msg void OnIo1F();
	afx_msg void OnExit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickPrevious2();
	afx_msg void OnClickNext2();
	afx_msg void OnClickTop2();
	afx_msg void OnClickBottom2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALIOMONITORINGDLG_H__2A41C545_2196_4ED5_92BA_47EA321A0FE4__INCLUDED_)
