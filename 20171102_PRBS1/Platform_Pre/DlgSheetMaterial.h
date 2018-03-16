#if !defined(AFX_DLGSHEETMATERIAL_H__5FFCFFAA_6A51_4F53_B676_6084B5D992D8__INCLUDED_)
#define AFX_DLGSHEETMATERIAL_H__5FFCFFAA_6A51_4F53_B676_6084B5D992D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSheetMaterial.h : header file
//

#include "PageMaterialInfo.h"
#include "PageMCodeInfo.h"
#include "PageMCodeInfo2ND.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSheetMaterial dialog

class CDlgSheetMaterial : public CDialog
{
private:	
	HWND m_hAutoMainWnd;

	CPropertySheet	m_sheet;
	CPageMaterialInfo*	m_pPageMaterialInfo;
	CPageMCodeInfo*		m_pPageMCodeInfo;
	CPageMCodeInfo2ND*	m_pPageMCodeInfo2ND;

	void				createSheet();

public:
	void				RenewMaterialCode();
// Construction
public:
	CDlgSheetMaterial(CWnd* pParent = NULL);   // standard constructor
	~CDlgSheetMaterial();
// Dialog Data
	//{{AFX_DATA(CDlgSheetMaterial)
	enum { IDD = IDD_DLGSHEETMATERIAL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSheetMaterial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSheetMaterial)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickExit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHEETMATERIAL_H__5FFCFFAA_6A51_4F53_B676_6084B5D992D8__INCLUDED_)
