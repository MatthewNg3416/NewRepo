//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_SYSTEMDATADLG_H__F710ADCB_F33D_4530_80F1_63C493CFF6C7__INCLUDED_)
#define AFX_SYSTEMDATADLG_H__F710ADCB_F33D_4530_80F1_63C493CFF6C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemDataDlg.h : header file
//

const int DEF_DATA_COMMON_DLG_ALL_ITEM = 15	;

/////////////////////////////////////////////////////////////////////////////
// CDataCommonDlg dialog
class MSystemData;

class CDataCommonDlg : public CDialog
{
private:
	CBrush m_brBckColor;
	CBtnEnh m_lblItem[DEF_DATA_COMMON_DLG_ALL_ITEM];
	CBtnEnh m_btnItem[DEF_DATA_COMMON_DLG_ALL_ITEM];

	MSystemData* m_plnkSystemData;

	double m_dData[DEF_DATA_COMMON_DLG_ALL_ITEM];
	CString m_strLineControllerIP;

	WCHAR m_cBufGet[50];
	WCHAR m_cBuf[50];

// Construction
public:
	CDataCommonDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataCommonDlg)
	enum { IDD = IDD_DATA_COMMON };
	CBtnEnh	m_bExit;
	CBtnEnh	m_bSave;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataCommonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void displayItemLabel(BOOL bUpdate = FALSE);
	void getUserInput(int iItemNo);

protected:

	// Generated message map functions
	//{{AFX_MSG(CDataCommonDlg)
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSystem0();
	afx_msg void OnSystem1();
	afx_msg void OnSystem2();
	afx_msg void OnSystem3();
	afx_msg void OnSystem4();
	afx_msg void OnSystem5();
	afx_msg void OnSystem6();
	afx_msg void OnSystem7();
	afx_msg void OnSystem8();
	afx_msg void OnSystem9();
	afx_msg void OnClickSystem22();
	afx_msg void OnClickSystem23();
	afx_msg void OnClickSystem24();
	afx_msg void OnClickSystem25();
	afx_msg void OnClickSystem26();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void ClickSystem10();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMDATADLG_H__F710ADCB_F33D_4530_80F1_63C493CFF6C7__INCLUDED_)
