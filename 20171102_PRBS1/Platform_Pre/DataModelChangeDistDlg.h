//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATAMODELCHANGEDISTDLG_H__0FA4E45A_D34A_40C8_A68E_5AA0BAD93091__INCLUDED_)
#define AFX_DATAMODELCHANGEDISTDLG_H__0FA4E45A_D34A_40C8_A68E_5AA0BAD93091__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataModelChangeDistDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataModelChangeDistDlg dialog
const int DEF_MODEL_CHANGE_DIST_MAX_LABEL = 15;

class CDataModelChangeDistDlg : public CDialog
{
// Construction
public:
	CDataModelChangeDistDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataModelChangeDistDlg)
	enum { IDD = IDD_DATA_MODEL_CHANGE_DISTANCE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CBtnEnh m_lblDistParameter[DEF_MODEL_CHANGE_DIST_MAX_LABEL];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataModelChangeDistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	MSystemData* m_plnkSystemData;
	MPanelData*	m_plnkPanelData;

	WCHAR m_szOldValue[100];
	WCHAR m_szNewValue[100];

	double m_dData[DEF_MODEL_CHANGE_DIST_MAX_LABEL];

	// Generated message map functions
	//{{AFX_MSG(CDataModelChangeDistDlg)
	afx_msg void OnSave();
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnModelChangeDist0();
	afx_msg void OnModelChangeDist1();
	afx_msg void OnModelChangeDist2();
	afx_msg void OnPaint();
	afx_msg void OnClickSystem4();
	afx_msg void OnClickSystem5();
	afx_msg void OnClickSystem6();
	afx_msg void OnClickSystem7();
	afx_msg void OnClickSystem8();
	afx_msg void OnClickSystem9();
	afx_msg void OnClickSystem10();
	afx_msg void OnClickSystem11();
	afx_msg void OnClickSystem12();
	afx_msg void OnClickSystem13();
	afx_msg void OnClickSystem14();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void updateDisplay();

	void getNewValue(int iIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMODELCHANGEDISTDLG_H__0FA4E45A_D34A_40C8_A68E_5AA0BAD93091__INCLUDED_)
