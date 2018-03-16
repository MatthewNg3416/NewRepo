#if !defined(AFX_DATATABMOUNTERDLG_H__0B607592_373C_4503_9069_73161A0393CD__INCLUDED_)
#define AFX_DATATABMOUNTERDLG_H__0B607592_373C_4503_9069_73161A0393CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataTabMounterDlg.h : header file
//
#include "btnenh.h"

/*
const int DEF_DATA_TABMOUNTER_DLG_ALL_ITEM			= 22;
const int DEF_DATA_TABMOUNTER_MAX_ROW_PER_COLUMN	= 12;
const int DEF_DATA_TABMOUNTER_DLG_ALL_COLUMN_ITEM	= 4;
*/

const int DEF_DATA_TABMOUNTER_DLG_TABMOUNTER_ITEM		= 10;
const int DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM			= 12;
const int DEF_DATA_TABMOUNTER_DLG_CAMERA_COLUMN_ITEM	= 3;

class MSystemData;
/////////////////////////////////////////////////////////////////////////////
// CDataTabMounterDlg dialog

class CDataTabMounterDlg : public CDialog
{
private:
	CBrush m_brBckColor;
	
	/*
	CBtnEnh m_lblItem[DEF_DATA_TABMOUNTER_DLG_ALL_ITEM + 3 * DEF_DATA_TABMOUNTER_MAX_ROW_PER_COLUMN];
	CBtnEnh m_btnItem[DEF_DATA_TABMOUNTER_DLG_ALL_ITEM];
	CBtnEnh m_btnColumnItem[DEF_DATA_TABMOUNTER_DLG_ALL_COLUMN_ITEM];
	int m_iSelectedItemNo;
	double m_dData[DEF_DATA_TABMOUNTER_DLG_ALL_ITEM + 3 * DEF_DATA_TABMOUNTER_MAX_ROW_PER_COLUMN];
	*/

	MSystemData* m_plnkSystemData;

	CBtnEnh	m_btnItem_TabMounter[DEF_DATA_TABMOUNTER_DLG_TABMOUNTER_ITEM];
	CBtnEnh	m_lblItem_TabMounter[DEF_DATA_TABMOUNTER_DLG_TABMOUNTER_ITEM];
	double m_dData_TabMounter[DEF_DATA_TABMOUNTER_DLG_TABMOUNTER_ITEM];

	CBtnEnh	m_btnItem_CameraColumn[DEF_DATA_TABMOUNTER_DLG_CAMERA_COLUMN_ITEM];
	CBtnEnh	m_btnItem_Camera[DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM];
	CBtnEnh m_lblItem_Camera[DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM][DEF_DATA_TABMOUNTER_DLG_CAMERA_COLUMN_ITEM];
	double m_dData_Camera[DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM][DEF_DATA_TABMOUNTER_DLG_CAMERA_COLUMN_ITEM];
	
	int m_iSelectedItemNo;
	int m_iSelectedColumnNo;

	WCHAR m_cBufGet[50];
	WCHAR m_cBuf[50];

// Construction
public:
	CDataTabMounterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataTabMounterDlg)
	enum { IDD = IDD_DATA_TABMOUNTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataTabMounterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void displayItemTitle();
	void displayItemLabel(BOOL bUpdate = FALSE);
	void getUserInput(int iColumnNo);
	void getUserInputTabMounter(int iItemNo);

protected:

	// Generated message map functions
	//{{AFX_MSG(CDataTabMounterDlg)
	afx_msg void OnSave();
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnColumn0();
	afx_msg void OnColumn1();
	afx_msg void OnColumn2();
	afx_msg void OnSystem0();
	afx_msg void OnSystem1();
	afx_msg void OnSystem2();
	afx_msg void OnSystem3();
	afx_msg void OnSystem4();
	afx_msg void OnSystem5();
	afx_msg void OnSystem6();
	afx_msg void OnSystem7();
	afx_msg void OnSystem8();
	afx_msg void OnSystem9();
	afx_msg void OnSystem10();
	afx_msg void OnSystem11();
	afx_msg void OnSystem12();
	afx_msg void OnSystem13();
	afx_msg void OnSystem14();
	afx_msg void OnSystem15();
	afx_msg void OnSystem16();
	afx_msg void OnSystem17();
	afx_msg void OnSystem18();
	afx_msg void OnSystem19();
	afx_msg void OnClickSystem20();
	afx_msg void OnClickSystem21();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATATABMOUNTERDLG_H__0B607592_373C_4503_9069_73161A0393CD__INCLUDED_)
