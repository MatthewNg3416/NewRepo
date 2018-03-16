//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHTABMOUNTERDLG_H__9AD35060_2889_4B97_90D4_C6457CA623D4__INCLUDED_)
#define AFX_TEACHTABMOUNTERDLG_H__9AD35060_2889_4B97_90D4_C6457CA623D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeachTabMounterDlg.h : header file
//
#include "DefSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CTeachTabMounterDlg dialog
const int ERR_TABMOUNTER_TEACH_DLG_SUCCESS			= 0;
const int ERR_TABMOUNTER_TEACH_DLG_CANCEAL			= 1;

const int DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE		= 3;	//2;
const int DEF_TABMOUNTER_TEACH_MAX_COORDINATE_TYPE	= 7;

const int DEF_TABMOUNTER_TEACH_MAX_VISION_COMMAND	= 2;
const int DEF_TABMOUNTER_TEACH_MAX_CAMMAND			= 4;
const int DEF_TABMOUNTER_TEACH_MAX_POS_BTN			= 9;
const int DEF_TABMOUNTER_TEACH_MAX_POS				= DEF_TABMOUNTER_TEACH_MAX_POS_BTN;

class MTabMounter;
class MCameraCarrier;
class MManageOpPanel;
class MSystemData;
class MProcessData;
class MWorkSchedule;
class IAxis;

class CTeachTabMounterDlg : public CDialog
{
public:
	// Doolin kwangilshin 2017. 08. 22.
	//
	enum
	{
		eAxisTabMount	= 0,
		eAxisTabMountZ	= 1,
		eAxisGantry		= 2,
		eAxisEnd
	};
	// End.

private:
	CBtnEnh	m_lblCoord[DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE][DEF_TABMOUNTER_TEACH_MAX_COORDINATE_TYPE];
	CBtnEnh	m_btnPos[DEF_TABMOUNTER_TEACH_MAX_POS_BTN];
	CBtnEnh	m_btnVision[DEF_TABMOUNTER_TEACH_MAX_VISION_COMMAND];
	CBtnEnh	m_btnCmd[DEF_TABMOUNTER_TEACH_MAX_CAMMAND];
	CBtnEnh	m_lblWorkerNo;
	CBtnEnh	m_btnAxis[DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE];
	CBtnEnh m_btnGantryEnable;

	
	// Doolin kwangilshin 2017. 08. 24.
	// Change Array Type.
	//
	CBtnEnh	m_ctrlBtnAxis[eAxisEnd];
	// End.
	
	// Doolin kwangilshin 2017. 08. 24.
	//
	//CMSFlexGrid	m_grd_AxisSnsState[eAxisEnd];
	// End.


	CBtnEnh	m_lblSelTabNo;
	CBtnEnh	m_lblScheduleNo;

	int m_iGroupNo;
	int	m_iTabMounterID;
	int	m_iTabMounterOldID;
	int m_iFixModel;
	
//	int m_iGantryLoadPosID;

	UINT m_uiTimerID;

	int m_iJogSelect;
	int m_iSelectedAxis;
	int	m_iSelectedPos;
	int m_iSelectedTabNo;

	int	m_iWorkScheduleNo;

	BOOL m_bVacSts;
	BOOL m_bCylSts;

	BOOL m_bGantryMoveSts;

	CBrush m_brBckColor;

	// Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	double m_rgdDisplayOffset[DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE];

	// TabMounter 전체 Position
	double	m_rgdFixedPosData[DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE][DEF_TABMOUNTER_TEACH_MAX_POS];
	double	m_rgdOffsetPosData[DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE][DEF_TABMOUNTER_TEACH_MAX_POS];
	double	m_rgdModelPosData[DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE][DEF_TABMOUNTER_TEACH_MAX_POS];

	MTabMounter*	m_plnkTabMounter;
	MCameraCarrier*	m_plnkCameraCarrier;
	MManageOpPanel*	m_plnkC_MngOpPanel;
	MSystemData*	m_plnkSystemData;
	MProcessData*	m_plnkProcessData;
//@	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];	
	
// Construction
public:
	CTeachTabMounterDlg(int iFixModel, int iTabMounterGroupNo, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeachTabMounterDlg)
	enum { IDD = IDD_TEACH_TAB_MOUNTER };
	CBtnEnh	m_btnNextTabMounter;
	CBtnEnh	m_btnPrevTabMounter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachTabMounterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachTabMounterDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnClickChangeAxis();
	afx_msg void OnAxis1();
	afx_msg void OnAxis2();
	afx_msg void OnAxis3();
	afx_msg void OnClickPos0();
	afx_msg void OnClickPos1();
	afx_msg void OnClickPos2();
	afx_msg void OnClickPos3();
	afx_msg void OnClickPos4();
	afx_msg void OnClickVisionCalib();
	afx_msg void OnClickRegisterMark();
	afx_msg void OnClickCmMove();
	afx_msg void OnClickPosSet();
	afx_msg void OnClickCmUpdown();
	afx_msg void OnClickCmOnoff();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickSelectPrevios();
	afx_msg void OnClickSelectNext();
	afx_msg void OnGetOriginOffset();
	afx_msg void OnClickPos5();
	afx_msg void OnClickPos6();
	afx_msg void OnClickPos7();
	afx_msg void OnClickPos8();
	afx_msg void OnGantryEnable();
	afx_msg void OnClickCmViewSchedule();
	afx_msg void OnClickCmMoveAll();
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
	void setWorkTabMounter(const int iTabMounterID);

	BOOL isEnabledToModifyPosValue();
	void setTabMounterPosOffset(const int iTargetTabMounterID, const int iFixModel, const int iPosID, const double dValue);

	int getWorkTabNo();
	int getWorkCameraNo();

	void setEnableAllMoveBtn();

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

#endif // !defined(AFX_TEACHTABMOUNTERDLG_H__9AD35060_2889_4B97_90D4_C6457CA623D4__INCLUDED_)
