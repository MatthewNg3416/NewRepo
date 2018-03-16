//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATAECDBDLG_H__07681D8F_2E7F_440D_8E2A_2C094A868E06__INCLUDED_)
#define AFX_DATAECDBDLG_H__07681D8F_2E7F_440D_8E2A_2C094A868E06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataECDBDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataECDBDlg dialog

class CDataECDBDlg : public CDialog
{
private:

	BOOL m_flgEditMode;

	long m_RowSel;

	CString		m_strECID;		// Equipment Constant ID
	CString		m_strECNAME;	// Equipment Constant Name
	CString		m_strECDEF;		// Equipment Constant Set Value
	CString		m_strECSLL;		// Equipment Transfer Stop Low Limit
	CString		m_strECSUL;		// Equipment Transfer Stop Upper Limit
	CString		m_strECWLL;		// Equipment Warning Low Limit
	CString		m_strECWUL;		// Equipment Warning Upper Limit

// Construction
public:
	CDataECDBDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataECDBDlg)
	enum { IDD = IDD_DATA_ECDATA_DB };
		// NOTE: the ClassWizard will add data members here
		CBtnEnh		m_btnEditMode;
		//CMSFlexGrid	m_grdData;
		CEdit		m_editECID;
		CEdit		m_editECNAME;
		CEdit		m_editECDEF;
		CEdit		m_editECSLL;
		CEdit		m_editECSUL;
		CEdit		m_editECWLL;
		CEdit		m_editECWUL;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataECDBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitGrid();
	void UpdateECData();

	// Generated message map functions
	//{{AFX_MSG(CDataECDBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickInsertData();
	afx_msg void OnClickModifyData();
	afx_msg void OnClickDeleteData();
	afx_msg void OnClickEditMode();
	afx_msg void OnClickGrid();
	afx_msg void OnClickExit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAECDBDLG_H__07681D8F_2E7F_440D_8E2A_2C094A868E06__INCLUDED_)
