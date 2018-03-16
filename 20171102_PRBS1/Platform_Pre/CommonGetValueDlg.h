//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_GETVALUEDLG_H__2DCECE2D_026B_45B0_BD81_51D582C2AD25__INCLUDED_)
#define AFX_GETVALUEDLG_H__2DCECE2D_026B_45B0_BD81_51D582C2AD25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetValueDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommonGetValueDlg dialog

class CCommonGetValueDlg : public CDialog
{
// Construction
public:
	CString m_strGetVal;
	void BtnDisable();
	CString GetStringValue();
	CCommonGetValueDlg(int maxchar = 10, WCHAR* getnum = _T(""), WCHAR* title = _T("Input the Value."), CWnd* pParent = NULL,
			     BOOL pwd = FALSE, BOOL btndisable = FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommonGetValueDlg)
	enum { IDD = IDD_COMMON_GET_VALUE };
	CBtnEnh	m_sDispValue;
	CBtnEnh	m_bDot;
	CBtnEnh	m_bEqual;
	CBtnEnh	m_bColon;
	CBtnEnh	m_bWon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonGetValueDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bpwd;
	BOOL m_bbtndisable;
	int m_intMaxChar;

	CString m_strWndText;
	
	void InitStaticDispWnd();
	BOOL VerifyMaxChar();

	// Generated message map functions
	//{{AFX_MSG(CCommonGetValueDlg)
	afx_msg void OnKeyOk();
	afx_msg void OnKeyEsc();
	afx_msg void OnKeyBack();
	afx_msg void OnKeyClear();
	afx_msg void OnKey1();
	afx_msg void OnKey2();
	afx_msg void OnKey3();
	afx_msg void OnKey4();
	afx_msg void OnKey5();
	afx_msg void OnKey6();
	afx_msg void OnKey7();
	afx_msg void OnKey8();
	afx_msg void OnKey9();
	afx_msg void OnKey0();
	afx_msg void OnKeyQ();
	afx_msg void OnKeyW();
	afx_msg void OnKeyE();
	afx_msg void OnKeyR();
	afx_msg void OnKeyT();
	afx_msg void OnKeyY();
	afx_msg void OnKeyU();
	afx_msg void OnKeyI();
	afx_msg void OnKeyO();
	afx_msg void OnKeyP();
	afx_msg void OnKeyA();
	afx_msg void OnKeyS();
	afx_msg void OnKeyD();
	afx_msg void OnKeyF();
	afx_msg void OnKeyG();
	afx_msg void OnKeyH();
	afx_msg void OnKeyJ();
	afx_msg void OnKeyK();
	afx_msg void OnKeyL();
	afx_msg void OnKeyZ();
	afx_msg void OnKeyX();
	afx_msg void OnKeyC();
	afx_msg void OnKeyV();
	afx_msg void OnKeyB();
	afx_msg void OnKeyN();
	afx_msg void OnKeyM();
	afx_msg void OnKeyEqual();
	afx_msg void OnKeyWon();
	afx_msg void OnKeyColon();
	afx_msg void OnKeyDot();
	afx_msg void OnKeyDash();
	afx_msg void OnKeyUnderBar();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	int m_cxChar;
	int m_cyChar;
	CRect m_rect;
	void UpdateDisplay(COLORREF bkcolor=RGB(255,255,255));
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETVALUEDLG_H__2DCECE2D_026B_45B0_BD81_51D582C2AD25__INCLUDED_)
