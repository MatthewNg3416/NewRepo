//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHCARRIERDLG_H__A0992C5A_3189_4A86_B80C_E83BC2D04A8A__INCLUDED_)

#define AFX_TEACHCARRIERDLG_H__A0992C5A_3189_4A86_B80C_E83BC2D04A8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeachCarrierDlg.h : header file
//
#include "MTabCarrier.h"
#include "MCtrlTabCarrier.h"
#include "MManageOpPanel.h"

const int DEF_TABCARRIER_TEACH_MAX_POS_BTN = 10;
/////////////////////////////////////////////////////////////////////////////
// CTeachCarrierDlg dialog
class MSystemData;

class CTeachCarrierDlg : public CDialog
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
	void SetWorkCarrier(const int iCarrierID);
	UINT m_uiTimerID;
	int m_iSelUnit;
	int m_iFixModel;
	int m_iJogSelect;
	int	m_iSelectedPos;
	
	BOOL m_bZSelected; //SJ_YYK 150109 Add.

	BOOL m_bYAxisSel; //SJ_YYK 151021 Add..
	// Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	MPos_XYT		m_dDisplayOffset;

	double			m_dDisplayOffsetZ; //SJ_YYK 150109 Add..

	// 전체 Position
	MPos_XYT		m_rgdFixedPosData[DEF_TABCARRIER_MAX_POSITION];
	MPos_XYT		m_rgdOffsetPosData[DEF_TABCARRIER_MAX_POSITION];
	MPos_XYT		m_rgdModelPosData[DEF_TABCARRIER_MAX_POSITION];
	MPos_XYT		m_XYTMarkOffset;

	double			m_rgdFixedZPosData[DEF_TABCARRIER_Z_MAX_POS];
	double			m_rgdOffsetZPosData[DEF_TABCARRIER_Z_MAX_POS];
	double			m_rgdModelZPosData[DEF_TABCARRIER_Z_MAX_POS];


	CBtnEnh		m_lblWorkerNo;

	CBtnEnh		m_lblGroup;
	CBtnEnh		m_BtnPos[DEF_TABCARRIER_TEACH_MAX_POS_BTN];
	//CBtnEnh		m_sCoord[7][DEF_TABCARRIER_AXIS_MAX_NO]; //SJ_YYK 150109 Modify..
	CBtnEnh		m_sCoord[7][4];

	STeachingInfo	m_sTeachingInfo;
	STeachingInfo	m_sTeachingZInfo; //SJ_YYK 150109 Modify..

	MTabCarrier*	m_plnkTabCarrier;
	MCtrlTabCarrier*	m_plnkCtrlTabCarrier;
	MManageOpPanel*	m_plnkC_MngOpPanel;
	MSystemData*	m_plnkSystemData;

	//Variable=========================
	int			m_iGroupNo;
	int			m_iCarrierID;
	int			m_iCarrierOldID;

	//Function=========================
	void		updatePosButton();
	void		setCarrierPosOffset(const int iTargetCarrierID, const int iFixModel, const int iPosID, const MPos_XYT posValue);
	BOOL		isEnabledToModifyPosValue();
	int			getWorkCameraNo();

	//Control==========================
	CBtnEnh		m_btnNextCarrier;
	CBtnEnh		m_btnPrevCarrier;


// Construction
public:
	CBrush m_brBckColor; 
	CTeachCarrierDlg(int iFixModel, int iGroupNo, CWnd* pParent = NULL);   // standard constructor

	/**
	 * 목표위치, Mark Offset, 기타보정, 현재위치, 편차 Display (XYT축)
	 */
	void updateDisplay();
	/**
	 * 고정좌표, Offset좌표 Display (XYT축)
	 */
	void updateCoordinate();
	/**
	 * 선택 Position 변경 (XYT축)
	 */
	void SelectPosition(int iPosIndex);
	/**
	 * Command Button 상태 Update
	 */
	void updateButton();

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
	
// Dialog Data
	//{{AFX_DATA(CTeachCarrierDlg)
	enum { IDD = IDD_TEACH_TAB_CARRIER };

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
	//{{AFX_VIRTUAL(CTeachCarrierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachCarrierDlg)
	afx_msg void OnClickExit();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickJog();
	afx_msg void OnClickX();
	afx_msg void OnClickY();
	afx_msg void OnClickCmMove();
	afx_msg void OnClickPosSet();
	afx_msg void OnClickSave();
	afx_msg void OnClickCarrierPos0();
	afx_msg void OnClickCarrierPos1();
	afx_msg void OnClickCarrierPos2();
	afx_msg void OnClickCarrierPos3();
	afx_msg void OnClickCarrierPos4();
	afx_msg void OnClickCarrierPos5();
	afx_msg void OnClickCarrierPos6();
	afx_msg void OnClickCarrierPos7();
	afx_msg void OnClickCarrierPos8();
	afx_msg void OnClickCarrierPos9();
	afx_msg void OnClickCmCyl1();
	afx_msg void OnClickCmVac1();
	afx_msg void OnClickVisionCalib();
	afx_msg void OnClickRegisterMark();
	afx_msg void OnClickGetOriginOffset();
	afx_msg void OnClickSelectPrevios();
	afx_msg void OnClickSelectNext();
	afx_msg void OnClickT();
	afx_msg void OnClickTabAlign();
	afx_msg void OnClickZ();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEACHCARRIERDLG_H__A0992C5A_3189_4A86_B80C_E83BC2D04A8A__INCLUDED_)
