//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALCAMERAEXPSHRDLG_H__188329C3_7683_4E74_9D60_771736A11986__INCLUDED_)
#define AFX_MANUALCAMERAEXPSHRDLG_H__188329C3_7683_4E74_9D60_771736A11986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ManualCameraExpShrDlg.h : header file
//

#include "DefSystem.h"

//class MTabAligner;
class MTabCarrier;
class MCameraCarrier;

const int	DEF_MAX_BTN_CMCESDLG	=	6;
const int	DEF_MAX_MANUAL_INSPECT_CAM_CARRIER	= 4;
/////////////////////////////////////////////////////////////////////////////
// CManualCameraExpShrDlg dialog

class CManualCameraExpShrDlg : public CDialog
{
private:
	int	m_iSelectGroup;
	BOOL m_bChangeGroup;

// Construction
public:
	CManualCameraExpShrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualCameraExpShrDlg)
	enum { IDD = IDD_MANUAL_CAMERA_ES };
	//}}AFX_DATA
	CBtnEnh	m_btnExpand[DEF_MAX_BTN_CMCESDLG];
	CBtnEnh m_btnShrink[DEF_MAX_BTN_CMCESDLG];
	CBtnEnh	m_btnSelectGroup[DEF_MAX_GROUP];
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualCameraExpShrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_uiTimerID;
	CBrush m_brBckColor;

//	MTabAligner* m_rgpTabAligner[DEF_MAX_TABALIGNER];
	MTabCarrier*	m_rgpTabCarrier[DEF_MAX_GROUP];
	MCameraCarrier* m_rgpInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

	// Generated message map functions
	//{{AFX_MSG(CManualCameraExpShrDlg)
	afx_msg void OnPaint();
	afx_msg void OnExpandTabAligner();
	afx_msg void OnShrinkTabAligner();
	afx_msg void OnExpandInspection();
	afx_msg void OnShrinkInspection();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnClickExpand3();
	afx_msg void OnClickShrink3();
	afx_msg void OnClickExpand4();
	afx_msg void OnClickShrink4();
	afx_msg void OnClickExpand5();
	afx_msg void OnClickShrink5();
	afx_msg void OnClickExpand6();
	afx_msg void OnClickShrink6();
	afx_msg void OnClickFrontGroup();
	afx_msg void OnClickRearGroup();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALCAMERAEXPSHRDLG_H__188329C3_7683_4E74_9D60_771736A11986__INCLUDED_)
