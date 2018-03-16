#if !defined(AFX_AUTONSMCINFO_H__74B9E45C_9AF1_4386_8305_1F6E800F7608__INCLUDED_)
#define AFX_AUTONSMCINFO_H__74B9E45C_9AF1_4386_8305_1F6E800F7608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CAutoNSMCInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoNSMCInfoDlg dialog
#define DEF_MAX_NSMC_INFO 16
#include "btnenh.h"
#include "DefTabFeeder.h"

class MTrsAutoManager;
class MSystemData;
class MModelChangeWordDatas;

class CAutoNSMCInfoDlg : public CDialog
{
// Construction
public:
	CAutoNSMCInfoDlg(CWnd* pParent = NULL);   // standard constructor
	void StartDlgTimer();

	UINT m_uiTimerID;
	CString m_rgstrNSMCInfo[DEF_MAX_NSMC_INFO];
	CBtnEnh	m_sNSMCInfo[DEF_MAX_NSMC_INFO];


	MTrsAutoManager* m_plnkTrsAutoManager;
	MSystemData* m_plnkSystemData;
	//NSMC KJS 임시
	//SOperationData* m_plnkTabFeederOperationData1;
	//SOperationData* m_plnkTabFeederOperationData2;
	//NSMC KJS
	MModelChangeWordDatas*			m_pNSMCData;

	// hongju_MC
	CString	m_strNewModelID;	// 선택 되어진 Model 이름..


	
// Dialog Data
	//{{AFX_DATA(CAutoNSMCInfoDlg)
	enum { IDD = IDD_AUTO_NSMC_INFO };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoNSMCInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoNSMCInfoDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickExit();
	afx_msg void OnDestroy();
	afx_msg void OnClickSelectNewModel();
	afx_msg void OnClickDeleteNewModel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTONSMCINFO_H__74B9E45C_9AF1_4386_8305_1F6E800F7608__INCLUDED_)
