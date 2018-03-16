//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_COMMONERRMSGDLG_H__DFAEE19E_CD1B_46E9_80A0_895D87CCA587__INCLUDED_)
#define AFX_COMMONERRMSGDLG_H__DFAEE19E_CD1B_46E9_80A0_895D87CCA587__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommonErrMsgDlg.h : header file
//

const int	ML_OK		= 0;	// OK Button만 필요할 때
const int	ML_YESNO	= 1;	// YES, NO가 필요할 때
const int	ML_ERROR	= 2;	// Error Display일 때

const int	MLOK		= 1;
const int	MLYES		= 2;
const int	MLNO		= 3;

#define		DEF_SYSTEM_ERROR_RECOVERY_FILE		".\\Info\\ErrorRecovery.dat"

/////////////////////////////////////////////////////////////////////////////
// CCommonErrMsgDlg dialog

class MManageOpPanel;

class CCommonErrMsgDlg : public CDialog
{
// Construction
public:
	CBrush m_brBckColor;
	CCommonErrMsgDlg(CWnd* pParent = NULL);   // standard constructor
	void CloseWnd();
	int iButton;
	void SetTextMsg(WCHAR* strProcess, WCHAR* strUnit, WCHAR* strMsg, WCHAR *strInfo, int iErrorCode);

// Dialog Data
	//{{AFX_DATA(CCommonErrMsgDlg)
	enum { IDD = IDD_COMMON_ERROR_MSG };
	CBtnEnh	m_bInfo;
	CBtnEnh	m_bOK;
	CBtnEnh	m_sMessage;
	CBtnEnh	m_sMessageTitle;
	CBtnEnh	m_sProcess;
	CBtnEnh	m_sProcessTitle;
	CBtnEnh	m_sUnit;
	CBtnEnh	m_sUnitTitle;
	CBtnEnh	m_sErrorInfoTitle;
	CBtnEnh m_sErrorRecoveryTitle;
	CBtnEnh m_sErrorRecovery;
	CBtnEnh	m_btnBuzzerOff;
	CString	m_sErrorLevel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommonErrMsgDlg)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommonErrMsgDlg)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickInfo();
	afx_msg void OnClickBuzzerOff();
	afx_msg void OnDestroy();
	afx_msg void OnClickCreateReport();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	displayMessage();
	void	getErrorRecovery();
	void	resizeWindoow();
	void	extendWindow();

private:
	WCHAR m_MsgProcess[80];
	WCHAR m_MsgUnit[80];
	WCHAR m_Msg[200];
	WCHAR m_MsgInfo[200];
	WCHAR m_MsgRecovery[200];

	int m_iProcessObjectID;
	int m_iUnitErrorCode;

	BOOL m_bIsRecoveryExist;

	BOOL m_bBuzzerMode;

	MManageOpPanel* m_plnkManageOpPanel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMONERRMSGDLG_H__DFAEE19E_CD1B_46E9_80A0_895D87CCA587__INCLUDED_)
