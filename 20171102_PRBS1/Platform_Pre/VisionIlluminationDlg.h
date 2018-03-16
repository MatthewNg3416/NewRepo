//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_VISIONILLUMINATIONDLG_H__1569121B_3DE6_4E9D_B6C8_913634D2598B__INCLUDED_)
#define AFX_VISIONILLUMINATIONDLG_H__1569121B_3DE6_4E9D_B6C8_913634D2598B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisionIlluminationDlg.h : header file
//
#include "DefSystem.h"

class IVision;
class ILighting;
class MPreBonderData;

const int DEF_PANEL_LIGHT = 0;
const int DEF_TABIC_LIGHT = 1;

/////////////////////////////////////////////////////////////////////////////
// CVisionIlluminationDlg dialog

class CVisionIlluminationDlg : public CDialog
{
private:
	IVision*			m_plnkVision;
	ILighting*			m_plnkLighting[DEF_MAX_LIGHTING];
	MPreBonderData*		m_plnkPreBonderData;

// Construction
public:
	CVisionIlluminationDlg(int iCamNo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVisionIlluminationDlg)
	enum { IDD = IDD_VISION_ILLUMINATION };
	CBtnEnh	m_bLight1;
	CBtnEnh	m_bLight2;
	CBtnEnh	m_lblLightValue1;
	CBtnEnh	m_lblLightValue2;
	CBtnEnh	m_bDown1;
	CBtnEnh	m_bDown2;
	CBtnEnh	m_bUp1;
	CBtnEnh	m_bUp2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionIlluminationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;
	int m_iCamNo;
	int m_rgiLightening[DEF_MAX_LIGHTING];

	// Generated message map functions
	//{{AFX_MSG(CVisionIlluminationDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnClickLight1();
	afx_msg void OnClickLight2();
	afx_msg void OnClickUp1();
	afx_msg void OnClickDown1();
	afx_msg void OnClickUp2();
	afx_msg void OnClickDown2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int setLightingValue();
	void displayUpdate();
	void setLightValue(int iLightType);
	void changeLightValue(int iLightType, int iChangeValue);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONILLUMINATIONDLG_H__1569121B_3DE6_4E9D_B6C8_913634D2598B__INCLUDED_)
