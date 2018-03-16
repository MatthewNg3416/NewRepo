//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATADISTANCEDLG_H__1622A85D_3A05_49A1_8775_5B2CF2687020__INCLUDED_)
#define AFX_DATADISTANCEDLG_H__1622A85D_3A05_49A1_8775_5B2CF2687020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataPanelAlignerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataPanelAlignerDlg dialog
const int DEF_PANEL_ALIGNER_MAX_LABEL = 10;

class CDataPanelAlignerDlg : public CDialog
{
// Construction
public:
	CDataPanelAlignerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataPanelAlignerDlg)
	enum { IDD = IDD_DATA_PANEL_ALIGNER };
	//}}AFX_DATA
	CBtnEnh m_lblPanelAlignParameter[DEF_PANEL_ALIGNER_MAX_LABEL];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataPanelAlignerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	MSystemData* m_plnkSystemData;

	WCHAR m_szOldValue[100];
	WCHAR m_szNewValue[100];

	double m_dData[DEF_PANEL_ALIGNER_MAX_LABEL];		

	// Generated message map functions
	//{{AFX_MSG(CDataPanelAlignerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnPanelAlignData0();
	afx_msg void OnPanelAlignData1();
	afx_msg void OnPanelAlignData2();
	afx_msg void OnPanelAlignData3();
	afx_msg void OnPanelAlignData4();
	afx_msg void OnPanelAlignData5();
	afx_msg void OnPanelAlignData6();
	afx_msg void OnPanelAlignData7();
	afx_msg void OnClickSystem8();
	afx_msg void OnClickSystem9();
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void updateDisplay();

	void getNewValue(int iIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATADISTANCEDLG_H__1622A85D_3A05_49A1_8775_5B2CF2687020__INCLUDED_)
