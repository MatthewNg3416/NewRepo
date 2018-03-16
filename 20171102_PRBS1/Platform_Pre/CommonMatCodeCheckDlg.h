#if !defined(AFX_COMMONMATCODECHECKDLG_H__68AF6036_E6A4_4803_8D92_55E3BDCEC22E__INCLUDED_)
#define AFX_COMMONMATCODECHECKDLG_H__68AF6036_E6A4_4803_8D92_55E3BDCEC22E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonMatCodeCheckDlg.h : header file
//

class MCtrlTabFeeder;

/////////////////////////////////////////////////////////////////////////////
// CCommonMatCodeCheckDlg dialog

class CCommonMatCodeCheckDlg : public CDialog
{
// Construction
public:
	CCommonMatCodeCheckDlg(int nTabFeederNo, int nSelectedReel, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCommonMatCodeCheckDlg)
	enum { IDD = IDD_COMMON_MATERIAL_CODE_CHECK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	int m_iTabFeederNo;
	int m_iSelectedReel;
	MCtrlTabFeeder* m_plnkCtrlTabFeeder;
	CString m_strUserID;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonMatCodeCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonMatCodeCheckDlg)
	afx_msg void OnClickUserId();
	afx_msg void OnClickMatCancel();
	afx_msg void OnClickMatStart();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONMATCODECHECKDLG_H__68AF6036_E6A4_4803_8D92_55E3BDCEC22E__INCLUDED_)
