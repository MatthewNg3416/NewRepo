//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_ERRORNOTSENDDLG_H__898F46EC_C074_4336_9595_C29CB43A89DD__INCLUDED_)
#define AFX_ERRORNOTSENDDLG_H__898F46EC_C074_4336_9595_C29CB43A89DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorNotSendDlg.h : header file
//

const int DEF_DATA_ERROR_DLG_ALL_ITEM = 30;

/////////////////////////////////////////////////////////////////////////////
// CErrorNotSendDlg dialog
class MSystemData;

class CErrorNotSendDlg : public CDialog
{
private:
	CBrush m_brBckColor;
	CBtnEnh m_btnErrorNo[DEF_DATA_ERROR_DLG_ALL_ITEM];
	CBtnEnh m_lblErrorNo[DEF_DATA_ERROR_DLG_ALL_ITEM];

	MSystemData* m_plnkSystemData;

	double m_dData[DEF_DATA_ERROR_DLG_ALL_ITEM];
	CString m_sData[DEF_DATA_ERROR_DLG_ALL_ITEM];
	CString m_strErrorNo[DEF_DATA_ERROR_DLG_ALL_ITEM];

	WCHAR m_cBufGet[50];
	WCHAR m_cBuf[50];

// Construction
public:
	CErrorNotSendDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CErrorNotSendDlg)
	enum { IDD = IDD_ERROR_NOT_SEND_DLG };
	CBtnEnh	m_bExit;
	CBtnEnh	m_bSave;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CErrorNotSendDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void displayItemLabel(BOOL bUpdate = FALSE);
	void getUserInput(int iItemNo);

protected:

	// Generated message map functions
	//{{AFX_MSG(CErrorNotSendDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnPaint();
	afx_msg void OnClickErrorNo0();
	afx_msg void OnClickErrorNo1();
	afx_msg void OnClickErrorNo2();
	afx_msg void OnClickErrorNo3();
	afx_msg void OnClickErrorNo4();
	afx_msg void OnClickErrorNo5();
	afx_msg void OnClickErrorNo6();
	afx_msg void OnClickErrorNo7();
	afx_msg void OnClickErrorNo8();
	afx_msg void OnClickErrorNo9();
	afx_msg void OnClickErrorNo10();
	afx_msg void OnClickErrorNo11();
	afx_msg void OnClickErrorNo12();
	afx_msg void OnClickErrorNo13();
	afx_msg void OnClickErrorNo14();
	afx_msg void OnClickErrorNo15();
	afx_msg void OnClickErrorNo16();
	afx_msg void OnClickErrorNo17();
	afx_msg void OnClickErrorNo18();
	afx_msg void OnClickErrorNo19();
	afx_msg void OnClickErrorNo20();
	afx_msg void OnClickErrorNo21();
	afx_msg void OnClickErrorNo22();
	afx_msg void OnClickErrorNo23();
	afx_msg void OnClickErrorNo24();
	afx_msg void OnClickErrorNo25();
	afx_msg void OnClickErrorNo26();
	afx_msg void OnClickErrorNo27();
	afx_msg void OnClickErrorNo28();
	afx_msg void OnClickErrorNo29();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORNOTSENDDLG_H__898F46EC_C074_4336_9595_C29CB43A89DD__INCLUDED_)
