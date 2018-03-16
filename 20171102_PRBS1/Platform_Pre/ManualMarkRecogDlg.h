//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALMARKRECOGDLG_H__214B160B_CAD7_42C6_99BC_C20C0909B849__INCLUDED_)
#define AFX_MANUALMARKRECOGDLG_H__214B160B_CAD7_42C6_99BC_C20C0909B849__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualMarkRecogDlg.h : header file
//

class IVision;
class MTrsAutoManager;
class MManageOpPanel;
class IIO;

/////////////////////////////////////////////////////////////////////////////
// CManualMarkRecogDlg dialog

class CManualMarkRecogDlg : public CDialog
{
// Construction
public:
	CManualMarkRecogDlg(int iCamNo, BOOL bSelectCam, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualMarkRecogDlg)
	enum { IDD = IDD_MANUAL_MARK_RECOG };
	CBtnEnh	m_btnExit;
	CBtnEnh	m_lblDescription;
	//}}AFX_DATA
	CBtnEnh	m_btnCamNo[2];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualMarkRecogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CBrush				m_brBckColor;
	BOOL				m_bIsOnLive;

//@	int					m_iCurrentCamNo;
	int					m_iCamIndex;
	int					m_iJogMode;
	int					m_iModelNo;
	int					m_iViewID;
	IVision*			m_plnkVision;
	IIO*				m_plnkIO;
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MManageOpPanel*		m_plnkManageOpPanel;
	
	UINT				m_vp_key;
	int					m_iCamNo[2];
	CPoint				m_MarkPoint[2];
	CRect				m_rectLimit;
	unsigned			m_nJogKeyValue;
	unsigned			m_nOldKeyValue;
	BOOL				m_bSelectCamera;

	void OnStillImage();
	void OnLiveImage();
	void selectCamera(int iCamNo);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManualMarkRecogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickExit();
	afx_msg void OnClickStart();
	afx_msg void OnClickCam1();
	afx_msg void OnClickCam2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void	Trace();
	unsigned checkKey();
	unsigned getJogKey();
	int tracePoint();

public:
	CPoint GetMarkPoint(int iCamIndex){ return m_MarkPoint[iCamIndex]; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALMARKRECOGDLG_H__214B160B_CAD7_42C6_99BC_C20C0909B849__INCLUDED_)
