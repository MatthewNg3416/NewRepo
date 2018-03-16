//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMONMSGDLG_H__6DEA9D9B_CB6E_4FCC_87C6_137AB134E14D__INCLUDED_)
#define AFX_COMMONMSGDLG_H__6DEA9D9B_CB6E_4FCC_87C6_137AB134E14D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonMsgDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCommonMsgDlg dialog

class CCommonMsgDlg : public CDialog
{
// Construction
public:
	CBrush m_brBckColor;
	WCHAR m_strNo[50];
	WCHAR m_strYes[50];
	int m_textHeight;
	int m_cy;
	int m_cx;
	WCHAR m_title[100];
	WCHAR m_message[500];
	UINT m_icontype;
	UINT m_type;
	CCommonMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommonMsgDlg)
	enum { IDD = IDD_COMMON_MSG };
	CBtnEnh	m_bMyAbort;
	CBtnEnh	m_bMyCancel;
	CBtnEnh	m_bMyCancel3;
	CBtnEnh	m_bMyIgnore;
	CBtnEnh	m_bMyNo;
	CBtnEnh	m_bMyOk;
	CBtnEnh	m_bMyOk2;
	CBtnEnh	m_bMyRetry;
	CBtnEnh	m_bMyRetry3;
	CBtnEnh	m_bMyYes;
	CBtnEnh	m_MyIcon;
	CBtnEnh	m_lblMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonMsgDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonMsgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickMyabort();
	afx_msg void OnClickMyok();
	afx_msg void OnClickMyyes();
	afx_msg void OnClickMyretry3();
	afx_msg void OnClickMyok2();
	afx_msg void OnClickMyretry();
	afx_msg void OnClickMycancel();
	afx_msg void OnClickMycancel3();
	afx_msg void OnClickMyno();
	afx_msg void OnClickMyignore();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONMSGDLG_H__6DEA9D9B_CB6E_4FCC_87C6_137AB134E14D__INCLUDED_)
