#if !defined(AFX_DLGTABOFFSETTABLE_H__3B8FE176_CCBD_4D35_8357_39B6AA819F4A__INCLUDED_)
#define AFX_DLGTABOFFSETTABLE_H__3B8FE176_CCBD_4D35_8357_39B6AA819F4A__INCLUDED_
#include "btnenh.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTabOffsetTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTabOffsetTable dialog
class MSystemData;

const int DEF_MAX_ROW	 = 1;
const int DEF_MAX_COLUMN = 2;

class CDlgTabOffsetTable : public CDialog
{
// Construction
public:
	CDlgTabOffsetTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTabOffsetTable)
	enum { IDD = IDD_DATA_TAB_OFFSET_TABLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CBtnEnh m_lblRowData[DEF_MAX_ROW][DEF_MAX_COLUMN];

	MSystemData* m_plnkSystemData;

	WCHAR	m_szOldValue[100];
	WCHAR	m_szNewValue[100];

	int m_iSelectedRow;

	double  m_rgdTabOffsetData[DEF_MAX_ROW][DEF_MAX_COLUMN];

	void UpdateDisplay(int iColNo);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTabOffsetTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTabOffsetTable)
	afx_msg void OnClickButton1();
	afx_msg void OnClickButton2();
	afx_msg void OnClickTabDataSave();
	afx_msg void OnClickTabDataCancel();
	afx_msg void OnClickUseSide();
	afx_msg void OnClickButton3();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTABOFFSETTABLE_H__3B8FE176_CCBD_4D35_8357_39B6AA819F4A__INCLUDED_)
