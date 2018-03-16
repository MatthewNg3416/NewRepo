#if !defined(AFX_AUTOMODESELECTDLG_H__8750162C_2A40_4813_8577_6EA270F9FE97__INCLUDED_)
#define AFX_AUTOMODESELECTDLG_H__8750162C_2A40_4813_8577_6EA270F9FE97__INCLUDED_

#include "btnenh.h"
#include "MSystemData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoModeSelectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoModeSelectDlg dialog
class MPanelData;

const int DEF_MAX_USE_MODE	= 4;				

class CAutoModeSelectDlg : public CDialog
{
// Construction
public:
	CAutoModeSelectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoModeSelectDlg)
	enum { IDD = IDD_AUTO_MODE_SELECT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CBtnEnh m_BtnMode[DEF_MAX_USE_MODE];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoModeSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CAutoModeSelectDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnAutoRun();
	afx_msg void OnPassRun();
	afx_msg void OnIdleRun();
	afx_msg void OnRepairRun();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	ERunMode			m_eRunMode;
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additiontal declarations immediately before the previous line.

#endif // !defined(AFX_AUTOMODESELECTDLG_H__8750162C_2A40_4813_8577_6EA270F9FE97__INCLUDED_)
