//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_AUTOPANELMARKMANUALSEARCHDLG_H__410BC1D9_F22B_4295_B9D3_8AB6758D9235__INCLUDED_)
#define AFX_AUTOPANELMARKMANUALSEARCHDLG_H__410BC1D9_F22B_4295_B9D3_8AB6758D9235__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoPanelMarkManualSearchDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoPanelMarkManualSearchDlg dialog
class IAxis;
class IVision;
class MTrsAutoManager;
class MManageOpPanel;

class CAutoPanelMarkManualSearchDlg : public CDialog
{
private:
	BOOL checkAxisState(IAxis* plnkAxis);

// Construction
public:
	CAutoPanelMarkManualSearchDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoPanelMarkManualSearchDlg)
	enum { IDD = IDD_AUTO_PANEL_MARK_MANUAL_SEARCH };
	CBtnEnh	m_lblYPlus;
	CBtnEnh	m_lblYMinus;
	CBtnEnh	m_lblXPlus;
	CBtnEnh	m_lblXMinus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoPanelMarkManualSearchDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoPanelMarkManualSearchDlg)
	afx_msg void OnClickBuzzerOff();
	afx_msg void OnClickPass();
	afx_msg void OnClickRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnClickAlignCamera1();
	afx_msg void OnClickAlignCamera2();
	afx_msg void OnRunStop();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int					m_iTimerID;
	CBrush				m_brBckColor;
	int					m_iViewID1;
	int					m_iViewID2;

	IAxis*				m_plnkX;
	IAxis*				m_plnkY;
	IAxis*				m_plnkY2;
	IVision*			m_plnkVision;
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MManageOpPanel*		m_plnkManageOpPanel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOPANELMARKMANUALSEARCHDLG_H__410BC1D9_F22B_4295_B9D3_8AB6758D9235__INCLUDED_)
