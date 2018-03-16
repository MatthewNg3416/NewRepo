//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_VISIONOPTIONDLG_H__EB094F5F_705A_42A8_A1A7_A91222443FD2__INCLUDED_)
#define AFX_VISIONOPTIONDLG_H__EB094F5F_705A_42A8_A1A7_A91222443FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisionOptionDlg.h : header file
//

#include "DefVisionExt.h"

/////////////////////////////////////////////////////////////////////////////
// CVisionOptionDlg dialog
class IVision;
class MPreBonderData;

class CVisionOptionDlg : public CDialog
{
private:
	IVision*	m_plnkVision;
	EMarkType	m_eMarkType;
	int			m_iCamNo;
	int			m_iModelNo;
	BOOL		m_bIsValidCamNo;

	int			m_rgiValue[4];

	CBtnEnh		m_rgCtrlBtn[4];
	CBtnEnh		m_rgCtrlLbl[4];

	MPreBonderData* m_plnkPreBonderData;

// Construction
public:
	CVisionOptionDlg(IVision* plnkVision, EMarkType eMarkType, int iCamNo, int iModelNo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVisionOptionDlg)
	enum { IDD = IDD_VISION_OPTION };
	CBtnEnh	m_ctrlLblCamNo;
	CBtnEnh	m_ctrlLblMarkName;
	CBtnEnh	m_lblIllumination1;
	CBtnEnh	m_lblIllumination2;
	CBtnEnh	m_btnillumination1;
	CBtnEnh	m_btnillumination2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void displayLightingValue();

protected:
	CBrush m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CVisionOptionDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnDirection();
	afx_msg void OnColor();
	afx_msg void OnThreshold();
	afx_msg void OnMaxNum();
	afx_msg void OnCamChangeTime();
	afx_msg void OnVisionWait();
	afx_msg void OnClickIllumination1();
	afx_msg void OnIllumination1OnOff();
	afx_msg void OnIllumination2OnOff();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONOPTIONDLG_H__EB094F5F_705A_42A8_A1A7_A91222443FD2__INCLUDED_)
