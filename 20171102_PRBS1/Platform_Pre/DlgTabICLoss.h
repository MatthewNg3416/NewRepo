//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGTABICLOSS_H__694DD143_C703_4C63_9334_032BEE896629__INCLUDED_)
#define AFX_DLGTABICLOSS_H__694DD143_C703_4C63_9334_032BEE896629__INCLUDED_

#include "ListCtrlEx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTabICLoss.h : header file
//

class CDBTabLoss;
class MSystemData;
/////////////////////////////////////////////////////////////////////////////
// CDlgTabICLoss dialog

class CDlgTabICLoss : public CDialog
{
private:
	CString		m_strResult;
	CString		m_strTitle;
	CString		m_strTrayID;
	CDBTabLoss*	m_pDBTabLoss;
	MSystemData*		m_plnkSystemData;

	int		m_iColumnCnt;
private:
	void	itemDisplay(int iCol, int iRow, CString strData, COLORREF crBackground = NULL);
// Construction
public:
	CDlgTabICLoss(int iWorkGroup, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTabICLoss)
	enum { IDD = IDD_DLGTABICLOSS_DIALOG };
//	CListCtrl	m_listResult;
	CListCtrlEx m_listResult;
	CDateTimeCtrl	m_dtEndDay;
	CDateTimeCtrl	m_dtStartDay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTabICLoss)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTabICLoss)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickSearchPcblog();
	afx_msg void OnClickLogPath();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnClickSearchPcblogShift();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTABICLOSS_H__694DD143_C703_4C63_9334_032BEE896629__INCLUDED_)
