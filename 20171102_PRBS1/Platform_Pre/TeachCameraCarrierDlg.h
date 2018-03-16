//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHCAMERCARRIERDLG_H__48DD5553_3CDA_49F2_ACA6_6D46EB17FE70__INCLUDED_)
#define AFX_TEACHCAMERCARRIERDLG_H__48DD5553_3CDA_49F2_ACA6_6D46EB17FE70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CameraTeachDlg.h : header file
//
#include "MCameraCarrier.h"
#include "MSystemData.h"
#include "MManageOpPanel.h"

/////////////////////////////////////////////////////////////////////////////
//@#ifdef DEF_SOURCE_SYSTEM
//@	const int DEF_CAMERACARRIER_MAX_AXIS_TYPE = 3;
//@#else
//@//@	const int DEF_CAMERACARRIER_MAX_AXIS_TYPE = 2;
//@#endif


const int DEF_CAMERACARRIER_MAX_AXIS_TYPE = 3;

// CTeachCameraCarrierDlg dialog
const int ERR_CAMERACARRIER_TEACH_DLG_SUCCESS			= 0;
const int ERR_CAMERACARRIER_TEACH_DLG_CANCEAL			= 1;

const int DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE		= 3;
const int DEF_CAMERACARRIER_TEACH_MAX_COORDINATE_TYPE	= 6;

const int DEF_CAMERACARRIER_TEACH_MAX_VISION_COMMAND	= 2;
const int DEF_CAMERACARRIER_TEACH_MAX_CAMMAND			= 4;
const int DEF_CAMERACARRIER_TEACH_MAX_POS_BTN			= 9;
const int DEF_CAMERACARRIER_TEACH_MAX_POS				= DEF_CAMERACARRIER_TEACH_MAX_POS_BTN;

const int DEF_CAMERACARRIER_TEACH_CAMERA_Y_AXIS_TITLE	= 0;
const int DEF_CAMERACARRIER_TEACH_BACKUP_Z_AXIS_TITLE	= 1;
const int DEF_CAMERACARRIER_TEACH_EXPAND_AXIS_TITLE		= 2;

class MCameraCarrier;
class MPanelAligner;
class MManageOpPanel;
class MSystemData;
class MProcessData;
class MPreBonderData;
class MWorkSchedule;
class CVisionViewDlg;
class IAxis;

/*
const int DEF_DLG_CAMERACARRIER_TEACH_MAX_COORDINATE_TYPE	= 5;
const int DEF_DLG_CAMERACARRIER_POS_MAX_BTN					= 4;
*/

class CTeachCameraCarrierDlg : public CDialog
{
	// Doolin kwangilshin 2017. 08. 22.
	//
	enum
	{
		eAxisX		= 0,
		eAxisZ		= 1,
		eAxisExp	= 2,
		eAxisEnd
	};
	// End.

private:
	CBtnEnh	m_lblCoord[DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE][DEF_CAMERACARRIER_TEACH_MAX_COORDINATE_TYPE];
	CBtnEnh	m_btnPos[DEF_CAMERACARRIER_TEACH_MAX_POS_BTN];
	CBtnEnh	m_btnVision[DEF_CAMERACARRIER_TEACH_MAX_VISION_COMMAND];
	CBtnEnh	m_btnCmd[DEF_CAMERACARRIER_TEACH_MAX_CAMMAND];
	CBtnEnh	m_lblWorkerNo;
	CBtnEnh	m_lblSelTabNo;
	CBtnEnh	m_lblScheduleNo;

	int m_iGroupNo;
	int	m_iCameraCarrierID;
	int	m_iCameraCarrierOldID;
	int m_iFixModel;
	
	UINT m_uiTimerID;

	int m_iJogSelect;
	int m_iSelectedAxis;
	int	m_iSelectedPos;
	int m_iSelectedTabNo;

	int	m_iWorkScheduleNo;

	CBrush m_brBckColor;

	BOOL m_bTemporaryView;
	CVisionViewDlg* m_pdlgView;

	int m_rgiJog_Carrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	int m_rgiJog_BackupZ[DEF_MAX_INSPECTION_CAM_CARRIER];
	int m_rgiJog_BackupX;
	int m_rgiJog_Expand[DEF_MAX_INSPECTION_CAM_CARRIER];

	// Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	double m_rgdDisplayOffset[DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE];

	// CameraCarrier 전체 Position
	double	m_rgdFixedPosData[DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE][DEF_CAMERACARRIER_TEACH_MAX_POS];
	double	m_rgdOffsetPosData[DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE][DEF_CAMERACARRIER_TEACH_MAX_POS];
	double	m_rgdModelPosData[DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE][DEF_CAMERACARRIER_TEACH_MAX_POS];

	MCameraCarrier*	m_plnkCamCarrier;
	MPanelAligner*	m_plnkPanelAligner;
	MManageOpPanel*	m_plnkC_MngOpPanel;
	MSystemData*	m_plnkSystemData;
	MPreBonderData*	m_plnkPreBonderData;
	MProcessData*	m_plnkProcessData;
//@	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];	

// Construction
public:
	CTeachCameraCarrierDlg(int iFixModel, int iCameraCarrierGroupNo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeachCameraCarrierDlg)
	enum { IDD = IDD_TEACH_CAMERA_CARRIER };
	// Doolin kwangilshin 2017. 08. 24.
	// Change Array Type.
	//
	CBtnEnh	m_ctrlBtnAxis[eAxisEnd];
	// End.
	
	// Doolin kwangilshin 2017. 08. 24.
	//
	//CMSFlexGrid	m_grd_AxisSnsState[eAxisEnd];
	// End.
	

	CBtnEnh	m_btnNextCameraCarrier;
	CBtnEnh	m_btnPrevCameraCarrier;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachCameraCarrierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachCameraCarrierDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnClickChangeAxis();
	afx_msg void OnAxis1();
	afx_msg void OnAxis2();
	afx_msg void OnClickVisionCalib();
	afx_msg void OnClickRegisterMark();
	afx_msg void OnClickCmMove();
	afx_msg void OnClickPosSet();
	afx_msg void OnClickCmOneTabPlus();
	afx_msg void OnClickCmOneTabMinus();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickSelectPrevios();
	afx_msg void OnClickSelectNext();
	afx_msg void OnGetOriginOffset();
	afx_msg void OnClickPos0();
	afx_msg void OnClickPos1();
	afx_msg void OnClickPos2();
	afx_msg void OnClickPos3();
	afx_msg void OnClickPos4();
	afx_msg void OnClickPos5();
	afx_msg void OnClickPos6();
	afx_msg void OnClickPos7();
	afx_msg void OnClickPos8();
	afx_msg void OnTemporaryView();
	afx_msg void OnClickAxis3();
	afx_msg void OnClickSelectSchedulePrev();
	afx_msg void OnClickSelectScheduleNext();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void selectPosition(int iPosIndex, BOOL bAskTabNoSkip = FALSE);
	void updateCoordinate();
	void updateDisplay();
	void updateButton();

	void updatePosButton();
	void setWorkCameraCarrier(const int iCameraCarrierID);
	int getWorkTabNo();
	int moveCameraCarrier(int iPosID, int iTabNo);

	// Doolin kwangilshin 2017. 08. 24.
	//
	void Init_GridAxisStste();
	
	void updateAxisSnsSts();
	
	BOOL Check_AxisStste(IAxis *pclsAxis);
	
	void SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome);
	void SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn);
	void SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn);
	void SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable);
	
	// End.
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEACHCAMERCARRIERDLG_H__48DD5553_3CDA_49F2_ACA6_6D46EB17FE70__INCLUDED_)
