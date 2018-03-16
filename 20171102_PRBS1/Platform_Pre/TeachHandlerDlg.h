//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHHANDLERDLG_H__E3488BAA_9D43_43F1_A914_E53F1D7FC6A7__INCLUDED_)
#define AFX_TEACHHANDLERDLG_H__E3488BAA_9D43_43F1_A914_E53F1D7FC6A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeachHANDLERDlg.h : header file
//
#include	"MHandler.h"
class		MWorkBench;

/////////////////////////////////////////////////////////////////////////////
// CTeachHandlerDlg dialog
const int DEF_LOAD_HANDLER_TEACH				=0;
const int DEF_UNLOAD_HANDLER_TEACH				=1;

const int ERR_HANDLER_TEACH_DLG_SUCCESS			= 0;

const int DEF_HANDLER_TEACH_XYT_AXIS_TITLE		= 3;
const int DEF_HANDLER_TEACH_MAX_AXIS_TITLE		= 4;
const int DEF_HANDLER_TEACH_MAX_COORDINATE_TYPE	= 6;

const int DEF_HANDLER_TEACH_MAX_VISION_COMMAND	= 2;
const int DEF_HANDLER_TEACH_MAX_VISION_CAMERA	= 2;
const int DEF_HANDLER_TEACH_MAX_CAMMAND			= 4;
const int DEF_HANDLER_TEACH_MAX_POS				= 11;

const int DEF_HANDLER_TEACH_MAX_POS_BTN			= 10;
const int DEF_HANDLER_TEACH_MAX_Z_POS_BTN		= 4;

class MPos_XYT;
class MSystemData;
//class MAcfData;
class MManageOpPanel;

class CTeachHandlerDlg : public CDialog
{
	
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

	// Construction
public:
	CTeachHandlerDlg(int iHandlerType, int iFixModel, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeachHandlerDlg)
	enum { IDD = IDD_TEACH_HANDLER };
	CBtnEnh	m_ctrlBtnHandlerX;
	CBtnEnh	m_ctrlBtnHandlerY;
	CBtnEnh	m_ctrlLblVision;
	CBtnEnh	m_ctrlBtnVisionCalib;
	CBtnEnh	m_ctrlBtnRegisterMark;
	CBtnEnh	m_ctrlBtnCamera1;
	CBtnEnh	m_ctrlBtnCamera2;
	CBtnEnh	m_ctrlBtnZSelect;

	// Doolin kwangilshin 2017. 08. 24.
	// Change Array Type.
	//
	CBtnEnh	m_ctrlBtnAxis[eAxisEnd];
	// End.
	
	// Doolin kwangilshin 2017. 08. 24.
	//
	//CMSFlexGrid	m_grd_AxisSnsState[eAxisEnd];
	// End.

	CBtnEnh m_ctrlBtnJog;
	CBtnEnh m_ctrlBtnSave;
	CBtnEnh m_ctrlBtnCancel;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachHandlerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachHandlerDlg)
	afx_msg void OnClickExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickPtuPos0();
	afx_msg void OnClickPtuPos1();
	afx_msg void OnClickPtuPos2();
	afx_msg void OnClickPtuPos3();
	afx_msg void OnClickHandlerPos4();
	afx_msg void OnClickHandlerPos5();
	afx_msg void OnClickHandlerPos6();
	afx_msg void OnClickHandlerPos7();
	afx_msg void OnClickHandlerPos8();
	afx_msg void OnClickHandlerPos9();
	afx_msg void OnClickSave();
	afx_msg void OnClickCmMove();
	afx_msg void OnClickPosSet();
	afx_msg void OnPaint();
	afx_msg void OnClickCmReserved0();
	afx_msg void OnClickCmReserved1();
	afx_msg void OnClickCamera1();
	afx_msg void OnClickCamera2();
	afx_msg void OnClickVisionCalib();
	afx_msg void OnClickRegisterMark();
	afx_msg void OnClickX();
	afx_msg void OnClickY();
	afx_msg void OnClickT();
	afx_msg void OnClickZ();
	afx_msg void OnClickZSelect();
	afx_msg void OnClickZPos0();
	afx_msg void OnClickZPos1();
	afx_msg void OnClickZPos2();
	afx_msg void OnClickZPos3();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBtnEnh		m_sCoordTitle[DEF_HANDLER_TEACH_MAX_COORDINATE_TYPE];
	CBtnEnh		m_sCoord[DEF_HANDLER_TEACH_MAX_COORDINATE_TYPE][DEF_HANDLER_TEACH_MAX_AXIS_TITLE];
	CBtnEnh		m_BtnPos[DEF_HANDLER_TEACH_MAX_POS_BTN];
	CBtnEnh		m_BtnZPos[DEF_HANDLER_TEACH_MAX_Z_POS_BTN];
	CBtnEnh		m_BtnVision[DEF_HANDLER_TEACH_MAX_VISION_COMMAND];
	CBtnEnh		m_BtnCamera[DEF_HANDLER_TEACH_MAX_VISION_CAMERA];
	CBtnEnh		m_BtnCmd[DEF_HANDLER_TEACH_MAX_CAMMAND];

	int	m_iHandlerID;
	int m_iFixModel;
	BOOL m_bVacSts;
	
	UINT m_uiTimerID;

	int	m_rgiIndexMapping[DEF_HANDLER_TEACH_MAX_POS_BTN];

	int m_iJogSelect;

	int	m_iSelectedPos;
	int m_iSelectedZPos;

	int m_iSelRealPos;
		
	BOOL m_bZPosSelected;

	BOOL m_bOnCheck;

	CBrush m_brBckColor;

	// Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	MPos_XYT	m_sDisplayOffsetXYT;
	double		m_dDisplayOffsetZ;

	// OLB Handler 전체 Position
	MPos_XYT	m_rgXYTFixedPosData[DEF_HANDLER_TEACH_MAX_POS];
	MPos_XYT	m_rgXYTOffsetPosData[DEF_HANDLER_TEACH_MAX_POS];
	MPos_XYT	m_rgXYTModelPosData[DEF_HANDLER_TEACH_MAX_POS];
	MPos_XYT	m_XYTMarkOffset;
	MPos_XYT	m_XYTEtcOffset;

	// OLB Handler Z 전체 Position
	double		m_rgdZFixedPosData[DEF_HANDLER_MAX_Z_POS];
	double		m_rgdZOffsetPosData[DEF_HANDLER_MAX_Z_POS];

	MHandler*		m_plnkHandler;
//@	MStage*			m_plnkStage;
	MWorkBench*		m_plnkWorkBench;
	MManageOpPanel*	m_plnkC_MngOpPanel;
	IIO*			m_plnkIO;

	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MAcfData*   	m_plnkAcfData;

	void updateDisplay();
	void updateZDisplay();
	void updateCoordinate();
	void updateZCoordinate();
	void updateButton();
	void selectXYTPosition(int iPosIndex);
	void selectZPosition(int iPosIndex);
	void disabledPositionIndex(int iPosIndex, int iTemp);
	void setTargetXYTPosValue(int iAxisID);


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

#endif // !defined(AFX_TEACHHANDLERDLG_H__E3488BAA_9D43_43F1_A914_E53F1D7FC6A7__INCLUDED_)
