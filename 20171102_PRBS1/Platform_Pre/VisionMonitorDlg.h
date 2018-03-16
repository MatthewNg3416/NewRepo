//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_VISIONMONITORDLG_H__0FFFD90B_F90E_4DB3_B1A4_587A8693661C__INCLUDED_)
#define AFX_VISIONMONITORDLG_H__0FFFD90B_F90E_4DB3_B1A4_587A8693661C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VisionMonitorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVisionMonitorDlg dialog
#include "DefVisionExt.h"

class IVision;
class MVisionCalibration;

//const int DEF_DLG_VISION_MONITIOR_MAX_CAM_BTN	= 12;
const int DEF_DLG_VISION_MONITIOR_MAX_CAM_BTN	= 24;

class CVisionMonitorDlg : public CDialog
{
// Construction
public:
	CVisionMonitorDlg(IVision* plnkVision, EMarkType eMarkType, int iCamNo = 0, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CVisionMonitorDlg)
	enum { IDD = IDD_VISION_MONITOR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVisionMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_KeyTimerID;
	CBtnEnh m_bDlgType[3];
	CBtnEnh m_rgCtrlCamBtn[DEF_DLG_VISION_MONITIOR_MAX_CAM_BTN];

	int		m_iViewID;
	EMarkType m_eMarkType;
	IVision* m_plnkVision;
	MVisionCalibration* m_plnkVisonCalibration;
	int		m_iSelectedMarkNo;
	int		m_iCurrentCamNo;
	BOOL	m_bIsOnLive;
	BOOL	m_bOnStillContinue;
	
	//110214.Add_____
	CBtnEnh	m_btnDrawPoint;
	CBtnEnh	m_btnMeasureAngle;
	void	displayMeasureDegree();
	CPoint	m_ptMeasure[2];
	BOOL	m_bMeasureAngle;
	//_______________

	// Generated message map functions
	//{{AFX_MSG(CVisionMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnPattern();
	afx_msg void OnBlob();
	afx_msg void OnCaliper();
	afx_msg void OnShowMark();
	afx_msg void OnDeleteMark();
	afx_msg void OnMakeMark();
	afx_msg void OnOptions();
	afx_msg void OnTest();
	afx_msg void OnLiveImage();
	afx_msg void OnStillImage();
	afx_msg void OnCam1();
	afx_msg void OnCam2();
	afx_msg void OnCam3();
	afx_msg void OnCam4();
	afx_msg void OnCam5();
	afx_msg void OnCam6();
	afx_msg void OnCam7();
	afx_msg void OnCam8();
	afx_msg void OnCam9();
	afx_msg void OnCam10();
	afx_msg void OnCam11();
	afx_msg void OnCam12();
	afx_msg void OnCam13();
	afx_msg void OnCam14();
	afx_msg void OnCam15();
	afx_msg void OnCam16();
	afx_msg void OnCam17();
	afx_msg void OnCam18();
	afx_msg void OnCam19();
	afx_msg void OnCam20();
	afx_msg void OnClickSearchArea();
	afx_msg void OnClickLoadImage();
	afx_msg void OnClickCaptureImage();
	afx_msg void OnSetReferenceLine();
	afx_msg void OnRemoveReferenceLine();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickMeasureAngle();
	afx_msg void OnClickDrawPoint();
	afx_msg void OnClickCam21();
	afx_msg void OnClickCam22();
	afx_msg void OnClickCam23();
	afx_msg void OnClickCam24();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	void selectCamera(int iCamNo);

	int	getBlobRefrenceModelNo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISIONMONITORDLG_H__0FFFD90B_F90E_4DB3_B1A4_587A8693661C__INCLUDED_)
