//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATASEARCHMAJORSPEC_H__3725CBEE_47C5_40CF_BED0_1F40D0C63941__INCLUDED_)
#define AFX_DATASEARCHMAJORSPEC_H__3725CBEE_47C5_40CF_BED0_1F40D0C63941__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSearchMajorSpec.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataSearchMajorSpec dialog

class CDataSearchMajorSpec : public CDialog
{
// Construction
public:
	CDataSearchMajorSpec(CString strCurrent, CString strNext, CWnd* pParent = NULL);   // standard constructor
	CString m_strCurrent;
	CString m_strNext;

// Dialog Data
	//{{AFX_DATA(CDataSearchMajorSpec)
	enum { IDD = IDD_DATA_SEARCH_MAJOR_SPEC };
	CWebBrowser2	m_MyBrowser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSearchMajorSpec)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataSearchMajorSpec)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASEARCHMAJORSPEC_H__3725CBEE_47C5_40CF_BED0_1F40D0C63941__INCLUDED_)
