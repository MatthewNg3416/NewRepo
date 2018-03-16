#if !defined(AFX_VISIONVIEWDLG_H__8EF7F4B8_C534_4407_BCF0_631383F7430C__INCLUDED_)
#define AFX_VISIONVIEWDLG_H__8EF7F4B8_C534_4407_BCF0_631383F7430C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisionViewDlg.h : header file
//

class IVision;

/////////////////////////////////////////////////////////////////////////////
// CVisionViewDlg dialog

class CVisionViewDlg : public CDialog
{
private:
	IVision* m_plnkVision;
	int m_iViewID;

// Construction
public:
	CVisionViewDlg(IVision* plnkVision, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVisionViewDlg)
	enum { IDD = IDD_VISION_VIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionViewDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVisionViewDlg)
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONVIEWDLG_H__8EF7F4B8_C534_4407_BCF0_631383F7430C__INCLUDED_)
