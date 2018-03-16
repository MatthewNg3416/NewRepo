#if !defined(AFX_AUTOINSPECTJUDGEDLG_H__44E2F819_66E3_45B3_9C0D_DE5CD6E6A089__INCLUDED_)
#define AFX_AUTOINSPECTJUDGEDLG_H__44E2F819_66E3_45B3_9C0D_DE5CD6E6A089__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoInspectJudgeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoInspectJudgeDlg dialog
class IVision;
class MTrsAutoManager;
class MManageOpPanel;

class CAutoInspectJudgeDlg : public CDialog
{
// Construction
public:
	CAutoInspectJudgeDlg(int iCamNo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoInspectJudgeDlg)
	enum { IDD = IDD_AUTOINSPECTJUDGEDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoInspectJudgeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoInspectJudgeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickBuzzerOff();
	afx_msg void OnClickRunStop();
	afx_msg void OnClickRetryRecog();
	afx_msg void OnClickTabReload();
	afx_msg void OnClickAlignCamera1();
	afx_msg void OnClickAlignCamera2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int		m_iTimerID;
	CBrush	m_brBckColor;
	int		m_iViewID1;
	int		m_iViewID2;
	int		m_iCamNo;
	int		m_iCamNo1;
	int		m_iCamNo2;
	int		m_iCamCarrierNo;
	IVision*			m_plnkVision;
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MManageOpPanel*		m_plnkManageOpPanel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOINSPECTJUDGEDLG_H__44E2F819_66E3_45B3_9C0D_DE5CD6E6A089__INCLUDED_)
