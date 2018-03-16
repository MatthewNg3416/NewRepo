//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_TITLEFORM_H__008DC046_8307_42F6_833F_C0F0F10A7E1E__INCLUDED_)
#define AFX_TITLEFORM_H__008DC046_8307_42F6_833F_C0F0F10A7E1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TitleForm.h : header file
//

#include "CommonEStopNotifyDlg.h"
#include "CommonPendantNotifyDlg.h"
#include "CommonStepStopNotifyDlg.h" //syr..090508
#include "MyMsgWnd.h"
#include "MLog.h"
#include "CommonNotifyDlg.h"
#include "EqStopCodeDlg.h"   //100825.KMS

/////////////////////////////////////////////////////////////////////////////
// CTitleForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class MTrsAutoManager;
class MManageOpPanel;
// 2009.11.01 Park, YK
class MSystemData;

class CTitleForm : public CFormView
{
protected:
	CTitleForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTitleForm)

// Form Data
public:
	//{{AFX_DATA(CTitleForm)
	enum { IDD = IDD_TITLE_FORM };
	CBtnEnh	m_LblTimer;
	CBtnEnh	m_LblTitleMsg;
	CBtnEnh	m_SimulEStop;
	CBtnEnh	m_SimulStart;
	CBtnEnh	m_SimulStepStop;
	CBtnEnh	m_btnLogin;
	CBtnEnh	m_btnBuzzerOff;
	CBtnEnh	m_btnTitleLogo;
	CBtnEnh m_btnKeyBoard;
	//}}AFX_DATA

// Attributes
private:
	UINT	m_TimerID;
	CString	m_strCurrentTime;
	CString	m_strVer;

	//110817 SJ_KJS
	CString m_strLCConnected;
	//_______________________
	CTime	m_tCurrentTime;
	BOOL	m_bActiveAutoView;
	BOOL	m_bStepStopState; //syr..090508

	MLog*	m_pLogMng;

	MTrsAutoManager* m_plnkTrsAutoManager;
	MManageOpPanel* m_plnkManageOpPanel;
	// 2009.11.01 Park, YK
	MSystemData*	m_plnkSystemData;

	CCommonEStopNotifyDlg	m_EStopNotifyDlg;
	CCommonPendantNotifyDlg	m_PendantNotifyDlg;
	CCommonStepStopNotifyDlg	m_StepStopNotifyDlg;//syr..090508

	CMyMsgWnd				m_MyMessageDlg;

	// NSMC
	CCommonNotifyDlg		m_ProgressDlgForAutoMC;

	// 100825 설비 유실 보고 화면
	CEqStopCodeDlg			m_dlgEqStopCode;

	// 2009.11.01 Park, YK
	UINT m_uiLanguageSelect;

// Operations
private:
	void dispLogInInfo();
	void peekNpump();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTitleForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTitleForm)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnClickLogin();
	afx_msg void OnClickTitleLogo();
	afx_msg void OnClickKeyBoard();
	afx_msg void OnClickInfo();
	afx_msg LRESULT OnAlarmMsg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayEStopStatus(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayTPConnectingStatus(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayRailUnfoldRequest(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayMaterialExhaustPreAlarm(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayMyMessage(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDispPanelMarkManualSearchDlg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT	OnDispInspectionJudgeDlg(UINT wParam, LPARAM lParam);
	afx_msg LRESULT OnDispMatCheckDlg(UINT wParam, LPARAM lParam);
	afx_msg void OnBuzzerOff();
	afx_msg LRESULT OnDisplayEqStopMessage(WPARAM wParam, LPARAM lParam);   //100825.KMS
	afx_msg LRESULT OnDisplayOperaterChangeMessage(WPARAM wParam, LPARAM lParam);   //120508.KMS
	afx_msg LRESULT	OnDisplayManualSearchRequest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisplayManualSearchPoint(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLanguageChange();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnDisplayProgressDlgForAutoMC(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLEFORM_H__008DC046_8307_42F6_833F_C0F0F10A7E1E__INCLUDED_)
