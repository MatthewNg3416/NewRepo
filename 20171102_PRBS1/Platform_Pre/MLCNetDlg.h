//{{AFX_INCLUDES()
#include "xnet.h"
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MLCNETDLG_H__05DB6710_FF7E_46D2_8F08_AAEA3A02E382__INCLUDED_)
#define AFX_MLCNETDLG_H__05DB6710_FF7E_46D2_8F08_AAEA3A02E382__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MLCNetDlg.h : header file
//

#define WMU_RCV_SEND_MSG				(WM_USER + 500)	

#include <afxmt.h>
#include "MLCNet.h"

/////////////////////////////////////////////////////////////////////////////
// MLCNetDlg dialog

class MLCNetDlg : public CDialog
{
// Construction
public:
	MLCNetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MLCNetDlg)
	enum { IDD = IDD_MLCNET };
	CEdit	m_editPort;
	CEdit	m_editIP;
	CListBox	m_cLog;
	CXNet	m_cXNet;
	CBtnEnh	m_btnClose;
	CBtnEnh	m_btnStart;
	CBtnEnh	m_btnStop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MLCNetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MLCNetDlg)
	afx_msg void OnCommCloseXnet();
	afx_msg void OnCommEstablishXnet();
	afx_msg void OnReceiveXnet(long mesgType, long mesgID, long mesgLen);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickBtnStart();
	afx_msg void OnClickBtnStop();
	afx_msg void OnClickBtnClose();
	afx_msg LRESULT OnRecvSendMsg(WPARAM wParam, LPARAM lParam);             	// 2005/01/18, KJH
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void AddSendMsg(CString sMsg);
	void LogTrace(int nType, CString szLog);

public:
	MLCNet *m_pMLCNet;

private:
	CMutex m_mutex;
	CStringArray m_srgSendMsg;

	//110629.KKY.Add_____
	int	 m_iCurTime;
	void writeLog(CString strFolder, CString strLog);
	void deleteOldFilesInFolder(CString strFolder, int iKeepingDays);
	//___________________
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLCNETDLG_H__05DB6710_FF7E_46D2_8F08_AAEA3A02E382__INCLUDED_)
