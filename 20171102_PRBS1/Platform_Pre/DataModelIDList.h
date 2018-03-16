#if !defined(AFX_DATAMODELIDLIST_H__B2BCB2D3_FF8D_4604_B431_0CE38B7DF7D5__INCLUDED_)
#define AFX_DATAMODELIDLIST_H__B2BCB2D3_FF8D_4604_B431_0CE38B7DF7D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataModelIDList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataModelIDList dialog

class CDataModelIDList : public CDialog
{
// Construction
public:
	CDataModelIDList(CString strModelFileDir, CString* pstrModelID, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataModelIDList)
	enum { IDD = IDD_DATA_MODEL_ID_LIST };
	CListBox	m_lboxModelList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataModelIDList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	CString m_strModelFileDir;
	CString* m_pstrModelID;

	int m_iModelCount;
	int m_iCurrentSel;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataModelIDList)
	afx_msg void OnExit();
	afx_msg void OnPageUp();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnPageDown();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeModelList();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMODELIDLIST_H__B2BCB2D3_FF8D_4604_B431_0CE38B7DF7D5__INCLUDED_)
