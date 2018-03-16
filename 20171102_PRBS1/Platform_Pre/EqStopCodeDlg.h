//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_EQSTOPCODEDLG_H__20F5A0A0_2F3E_4228_9DD5_C12B31F29618__INCLUDED_)
#define AFX_EQSTOPCODEDLG_H__20F5A0A0_2F3E_4228_9DD5_C12B31F29618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EqStopCodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEqStopCodeDlg dialog
const int DEF_MAX_BUTTON	= 28;

class MSystemData;

class CEqStopCodeDlg : public CDialog
{
// Construction
public:
	CEqStopCodeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEqStopCodeDlg)
	enum { IDD = IDD_EQ_STOP };
	CBtnEnh		m_bSave;
	CBtnEnh		m_bInput;
	CEdit		m_EditItem;
	CBtnEnh		m_CodeItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEqStopCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEqStopCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickEqstopBtn0();
	afx_msg void OnClickEqstopBtn1();
	afx_msg void OnClickEqstopBtn2();
	afx_msg void OnClickEqstopBtn3();
	afx_msg void OnClickEqstopBtn4();
	afx_msg void OnClickEqstopBtn5();
	afx_msg void OnClickEqstopBtn6();
	afx_msg void OnClickEqstopBtn7();
	afx_msg void OnClickEqstopBtn8();
	afx_msg void OnClickEqstopBtn9();
	afx_msg void OnClickEqstopBtn10();
	afx_msg void OnClickEqstopBtn11();
	afx_msg void OnClickEqstopBtn12();
	afx_msg void OnClickEqstopBtn13();
	afx_msg void OnClickEqstopBtn14();
	afx_msg void OnClickEqstopBtn15();
	afx_msg void OnClickEqstopBtn16();
	afx_msg void OnClickEqstopBtn17();
	afx_msg void OnClickEqstopBtn18();
	afx_msg void OnClickEqstopBtn19();
	afx_msg void OnClickEqstopBtn20();
	afx_msg void OnClickEqstopBtn21();
	afx_msg void OnClickEqstopBtn22();
	afx_msg void OnClickEqstopBtn23();
	afx_msg void OnClickEqstopBtn24();
	afx_msg void OnClickEqstopBtn25();
	afx_msg void OnClickEqstopBtn26();
	afx_msg void OnClickEqstopBtn27();
	afx_msg void OnClickSave();
	afx_msg void OnClickReport();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MSystemData* m_plnkSystemData;

	CBtnEnh			m_btnButton[DEF_MAX_BUTTON];
	CBtnEnh			m_lblLabel[DEF_MAX_BUTTON];

	CString			m_nCode[DEF_MAX_BUTTON];

	CString			m_strItem[DEF_MAX_BUTTON];
	CString			m_strCodeItem[DEF_MAX_BUTTON];

	int				m_nSelectCode;
	BOOL			m_bState[DEF_MAX_BUTTON];
	int				m_nSelectitem;

	void CodeData();
	void ItemData();
	void UpDateDisplay(int m_nButton); 
	void OnDisplayLanguage();
	void UpDateCodeDisplay(int m_nButton); 

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQSTOPCODEDLG_H__20F5A0A0_2F3E_4228_9DD5_C12B31F29618__INCLUDED_)
