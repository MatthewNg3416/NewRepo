//{{AFX_INCLUDES()
#include "btnenh.h"
#include <afxCmn.h>  
//}}AFX_INCLUDES
#if !defined(AFX_LOGMAINFORM_H__5E078EDC_7378_49A5_902B_E310AF324A89__INCLUDED_)
#define AFX_LOGMAINFORM_H__5E078EDC_7378_49A5_902B_E310AF324A89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogMainForm.h : header file
//


#include "MLog.h"
#include "ListCtrlEx.h"

/////////////////////////////////////////////////////////////////////////////
// CLogMainForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define DEF_MAX_LOG_ROW 21 //syr..Log

class CLogMainForm : public CFormView
{
protected:
	CLogMainForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLogMainForm)


	SLogViewItem*	m_pLogViewItem;

	int				m_iObjectID;
	BOOL			m_bFormInitialized;
	int				m_iLanguage;

	// Log Configuration parameter
	BYTE			m_ucLevel;
	int				m_iLogKeepingDays;
	int				m_iLogViewItems;
	int				m_iCurrentObjectID;
	int				m_iCurrentLogType;
// Form Data
	
public:
	//{{AFX_DATA(CLogMainForm)
	enum { IDD = IDD_LOG_MAIN_FORM };
	//CMSFlexGrid	m_flexgridLogList;
	//}}AFX_DATA

// Operations
public:
	void InitializeFlexGrid(int iLogType);
	void InitializeButton();
	void InitializeLabel();
	void SetCurrentObjectID(int iObjectID);	
	void displayTable(); //syr..Log
	void redrawRowColor(); //syr..Log

	void SetLogConfig( BYTE ucLevel, int iKeepingDays, int iViewItems );

	int	GetNumViewItem() const { return m_iLogViewItems; };

	void ChangeLogTable(int iLogType);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogMainForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush	m_brBckColor;
	
	virtual ~CLogMainForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLogMainForm)
	afx_msg void OnPaint();
	afx_msg void OnLogSetting();
	afx_msg void OnTacttimeLog();
	afx_msg void OnErrorLog();
	afx_msg void OnNormalLog();
	afx_msg LRESULT OnRefreshLogMainWindow(UINT wParam, LPARAM lParam);
	afx_msg void OnClickLineTactLog();
	afx_msg LRESULT OnDisplayLanguage(UINT wParam, LPARAM lParam);
	afx_msg void OnClickPageUp();
	afx_msg void OnClickPageDown();
	afx_msg void OnClickMachineTactLog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_ListControl;
//	afx_msg void OnLvnItemchangedListView(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLvnBegindragListView(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGMAINFORM_H__5E078EDC_7378_49A5_902B_E310AF324A89__INCLUDED_)
#include "afxcmn.h"
