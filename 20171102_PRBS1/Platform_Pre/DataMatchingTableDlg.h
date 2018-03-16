#if !defined(AFX_DATAMATCHINGTABLEDLG_H__560E34A8_4B04_4BCD_B08B_021352D3C412__INCLUDED_)
#define AFX_DATAMATCHINGTABLEDLG_H__560E34A8_4B04_4BCD_B08B_021352D3C412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMatchingTableDlg.h : header file
//

#include "msflexgrid.h"
#include "DefSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CDataMatchingTableDlg dialog

#define DEF_MAX_COL 15
#define DEF_MAX_PANEL_TYPE 5

class CDataMatchingTableDlg : public CDialog
{
// Construction
public:
	CDataMatchingTableDlg(CString strModelFileDir, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataMatchingTableDlg)
	enum { IDD = IDD_DATA_DEVICE_ID_MATCHING_DLG };
	//CMSFlexGrid	m_FlexGrid;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMatchingTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void redrawRowColor();
	void displayTable();
	void calculateTable();

private:
	CString m_strModelFileDir;

	int m_iTotalDeviceIDNum;
	//NSMC KJS
	int m_iSelectedPage;

	int m_iOldSelectedRow;
	int m_iSelectedRow;

	CStringArray m_strArray_DeviceID;
	CStringArray m_strArray_ModelID;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataMatchingTableDlg)
	afx_msg void OnClickExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickConfigMatchingData();
	afx_msg void OnClickMsflexgridMatchingTable();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnPageUp();
	afx_msg void OnPageDown();
	afx_msg void OnAddMatchingData();
	afx_msg void OnDeleteMatchingData();
	afx_msg void OnClickUpdateMaterial();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMATCHINGTABLEDLG_H__560E34A8_4B04_4BCD_B08B_021352D3C412__INCLUDED_)
