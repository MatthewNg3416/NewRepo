//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_VISIONSEARCHAREADLG_H__66C534AF_DCCD_4959_8368_B5477A122750__INCLUDED_)
#define AFX_VISIONSEARCHAREADLG_H__66C534AF_DCCD_4959_8368_B5477A122750__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisionSearchAreaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVisionSearchAreaDlg dialog

class IVision;

class CVisionSearchAreaDlg : public CDialog
{
// Construction
private:
	int m_iMarkNo;
	int m_iCamNo;
	char* m_pszMarkName;

	int m_iCenterX;
	int m_iCenterY;
	int m_iWidth;
	int m_iHeight;

	CBtnEnh	m_btnCtrl;
	CBtnEnh	m_lblCenterX;
	CBtnEnh	m_lblCenterY;
	CBtnEnh	m_lblWidth;
	CBtnEnh	m_lblHeight;

	IVision* m_plnkVision;

public:
	CVisionSearchAreaDlg(IVision* plnkVision, int iCamNo, int iMarkNo, char* szMarkName, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVisionSearchAreaDlg)
	enum { IDD = IDD_VISION_SEARCH_AREA };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionSearchAreaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVisionSearchAreaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLeft();
	afx_msg void OnTop();
	afx_msg void OnRight();
	afx_msg void OnBottom();
	afx_msg void OnSave();
	afx_msg void OnExit();
	afx_msg void OnClickCenterX();
	afx_msg void OnClickCenterY();
	afx_msg void OnClickWidth();
	afx_msg void OnClickHeight();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONSEARCHAREADLG_H__66C534AF_DCCD_4959_8368_B5477A122750__INCLUDED_)
