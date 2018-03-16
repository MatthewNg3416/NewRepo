//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHSTAGEDLG_H__ACC2EAB6_333A_4532_AEFD_EE6C8380256D__INCLUDED_)
#define AFX_TEACHSTAGEDLG_H__ACC2EAB6_333A_4532_AEFD_EE6C8380256D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeachPanelAlignerDlg.h : header file
//
#include "DefPanelAligner.h"
#include "DefPanelTransfer.h"
#include "MPos_XYT.h"
#include "MPos_XYTZ.h"

/////////////////////////////////////////////////////////////////////////////
// CTeachPanelAlignerDlg dialog
const int ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS			= 0;

const int DEF_PANEL_ALIGNER_TEACH_XYT_AXIS_TITLE		= 3;
const int DEF_PANEL_ALIGNER_TEACH_MAX_AXIS_TITLE		= 4;
const int DEF_PANEL_ALIGNER_TEACH_MAX_COORDINATE_TYPE	= 8;

const int DEF_PANEL_ALIGNER_TEACH_MAX_VISION_COMMAND	= 2;
const int DEF_PANEL_ALIGNER_TEACH_MAX_VISION_CAMERA		= 4;
const int DEF_PANEL_ALIGNER_TEACH_MAX_CAMMAND			= 12;
const int DEF_PANEL_ALIGNER_TEACH_MAX_POS				= DEF_PANEL_ALIGNER_MAX_POSITION;
const int DEF_PANEL_ALIGNER_TEACH_MAX_POS_BTN			= 16;
const int DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS				= DEF_PANEL_ALIGNER_Z_MAX_POSITION;
const int DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS_BTN			= 4;

class MPanelAligner;
class MPanelTransfer;
class MCtrlPanelAligner;
class MManageOpPanel;
class IAxis;

class CTeachPanelAlignerDlg : public CDialog
{
public:
	// Doolin kwangilshin 2017. 08. 22.
	//
	enum
	{
		eAxisX	= 0,
		eAxisY	= 1,
		eAxisT	= 2,
		eAxisZ	= 3,
		eAxisEnd
	};
	// End.

private:
	CBtnEnh		m_sCoordTitle[DEF_PANEL_ALIGNER_TEACH_MAX_COORDINATE_TYPE];
	CBtnEnh		m_sCoord[DEF_PANEL_ALIGNER_TEACH_MAX_COORDINATE_TYPE][DEF_PANEL_ALIGNER_TEACH_MAX_AXIS_TITLE];
	CBtnEnh		m_BtnPos[DEF_PANEL_ALIGNER_TEACH_MAX_POS_BTN];
	CBtnEnh		m_BtnZPos[DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS_BTN];
	CBtnEnh		m_BtnVision[DEF_PANEL_ALIGNER_TEACH_MAX_VISION_COMMAND];
	CBtnEnh		m_BtnCamera[DEF_PANEL_ALIGNER_TEACH_MAX_VISION_CAMERA];
	CBtnEnh		m_BtnCmd[DEF_PANEL_ALIGNER_TEACH_MAX_CAMMAND];
	
	int	m_iPanelAlignerID;
	int m_iFixModel;
	
	UINT m_uiTimerID;
	UINT m_uiTimerID3; //Camera 4, 8 연속 촬상을 위한 Timer. 

	int	m_rgiIndexMapping[DEF_PANEL_ALIGNER_TEACH_MAX_POS_BTN];

	int m_iJogSelect;

	int	m_iSelectedPos;
	int m_iSelectedZPos;

	int m_iSelRealPos;
		
	BOOL m_bZSelected;

	BOOL m_bPanelAlignerVacSts;
	BOOL m_bPanelAlignerVacOnCheck;

	BOOL m_bAirCVVacSts;
	BOOL m_bAirCVVacOnCheck;

	BOOL m_bAirCVBlowOnSts;
	BOOL m_bAirCVBlowOnCheck;

	CBrush m_brBckColor;

	int m_iCamNo;

	BOOL m_bMultiXJog;

	double m_dDisplayOffsetX2;
	/** 
	 * Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	 */
	MPos_XYT	m_sDisplayOffsetXYT;
	double		m_dDisplayOffsetStageZ;

	// OLB PanelAligner 전체 Position
	MPos_XYT	m_rgXYTFixedPosData[DEF_PANEL_ALIGNER_TEACH_MAX_POS];
	MPos_XYT	m_rgXYTOffsetPosData[DEF_PANEL_ALIGNER_TEACH_MAX_POS];
	MPos_XYT	m_rgXYTModelPosData[DEF_PANEL_ALIGNER_TEACH_MAX_POS];
	MPos_XYT	m_XYTMarkOffset;
	MPos_XYT	m_XYTEtcOffset;

	// PanelAligner Z
	double		m_rgZFixedPosData[DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS];
	double		m_rgZOffsetPosData[DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS];
	double		m_rgZModelPosData[DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS];

	MPanelAligner*		m_plnkPanelAligner;
	MCtrlPanelAligner*	m_plnkCtrlPanelAligner;
	MManageOpPanel*		m_plnkC_MngOpPanel;

	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;
	MPreBonderData*			m_plnkPreBonderData;

// Construction
public:
	CTeachPanelAlignerDlg(int iFixModel, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeachPanelAlignerDlg)
	enum { IDD = IDD_TEACH_PANEL_ALIGNER };
	CBtnEnh	m_ctrlBtnZSelect;
	CBtnEnh	m_ctrlBtnSelectJog;
	
	// Doolin kwangilshin 2017. 08. 24.
	// Change Array Type.
	//
	CBtnEnh	m_ctrlBtnAxis[eAxisEnd];
	// End.
	
	// Doolin kwangilshin 2017. 08. 24.
	//
	//CMSFlexGrid	m_grd_AxisSnsState[eAxisEnd];
	// End.

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachPanelAlignerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachPanelAlignerDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnJog();
	afx_msg void OnX();
	afx_msg void OnY();
	afx_msg void OnT();
	afx_msg void OnZ();
	afx_msg void OnPanelAlignerPos0();
	afx_msg void OnPanelAlignerPos1();
	afx_msg void OnPanelAlignerPos2();
	afx_msg void OnPanelAlignerPos3();
	afx_msg void OnPanelAlignerPos4();
	afx_msg void OnPanelAlignerPos5();
	afx_msg void OnPanelAlignerPos6();
	afx_msg void OnPanelAlignerPos7();
	afx_msg void OnPanelAlignerPos8();
	afx_msg void OnPanelAlignerPos9();
	afx_msg void OnPanelAlignerPos10();
	afx_msg void OnPanelAlignerPos11();
	afx_msg void OnPanelAlignerPos12();
	afx_msg void OnPanelAlignerPos13();
	afx_msg void OnPanelAlignerPos14();
	afx_msg void OnPanelAlignerPos15();
	afx_msg void OnVisionCalib();
	afx_msg void OnRegisterMark();
	afx_msg void OnCmMove();
	afx_msg void OnCmMarkAlign();
	afx_msg void OnPosSet();
	afx_msg void OnCmVacuum();
	afx_msg void OnZSelect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickZPos0();
	afx_msg void OnClickZPos1();
	afx_msg void OnClickZPos2();
	afx_msg void OnClickZPos3();
	afx_msg void OnClickCmAirCvOnoff();
	afx_msg void OnClickCmAirCvVacOnoff();
	afx_msg void OnClickCmStageUpvac();
	afx_msg void OnClickCmStageReleasedown();
	afx_msg void OnClickCmResetAlignOffset();
	afx_msg void OnClickCmPanelSetStage();
	afx_msg void OnClickCmManualView();
	afx_msg void OnClickCmPanelTurn();
	afx_msg void OnClickCmPanelReturn();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SelectPosition(int iPosIndex);
	void SelectZPosition(int iPosIndex);

	void UpdateCoordinate();
	void UpdateZCoordinate();

	void UpdateDisplay();
	void UpdateZDisplay();

	BOOL disabledPositionIndex(int iPosIndex, int iTemp);
	void UpdateButton();

	// Doolin YoonDaeRoh 2017. 09. 05.
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

#endif // !defined(AFX_TEACHSTAGEDLG_H__ACC2EAB6_333A_4532_AEFD_EE6C8380256D__INCLUDED_)
