//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_EQSTOPDLG_H__97C6C7F6_2500_49BE_9791_810B1F4F1277__INCLUDED_)
#define AFX_EQSTOPDLG_H__97C6C7F6_2500_49BE_9791_810B1F4F1277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EqStopDlg1.h : header file
//
//#include "btnenh.h"

class MLCNet;

const int DEF_MAX_BUTTON	= 28;

/////////////////////////////////////////////////////////////////////////////
// CEqStopDlg dialog

class CEqStopDlg : public CDialog
{
// Construction
public:
	CEqStopDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEqStopDlg)
	enum { IDD = IDD_EQ_STOP1 };
	CBtnEnh			m_btnButton[DEF_MAX_BUTTON];
	CBtnEnh			m_lblText[DEF_MAX_BUTTON];
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqStopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	MLCNet*			m_plnkLCNet;
	// Generated message map functions
	//{{AFX_MSG(CEqStopDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();	
	afx_msg void OnButton16();
	afx_msg void OnButton17();
	afx_msg void OnButton18();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	afx_msg void OnButton21();
	afx_msg void OnButton22();
	afx_msg void OnButton23();
	afx_msg void OnButton24();
	afx_msg void OnButton25();	
	afx_msg void OnButton26();
	afx_msg void OnButton27();
	afx_msg void OnButton28();	
	afx_msg void OnInput();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSave();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	int				m_nSelectCode;

	void UpDateDisplay(); 
	
	void SelectButton(int iBtnNo);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQSTOPDLG_H__97C6C7F6_2500_49BE_9791_810B1F4F1277__INCLUDED_)

