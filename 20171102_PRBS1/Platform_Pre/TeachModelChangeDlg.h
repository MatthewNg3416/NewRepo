//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHMODELCHANGEDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_)
#define AFX_TEACHMODELCHANGEDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeederTeachDlg.h : header file
//
#include "MModelChange.h"
#include "MManageOpPanel.h"

class IAxis;

const int DEF_TEACH_MODEL_CHANGE_DLG_SUCCESS = 0;

/////////////////////////////////////////////////////////////////////////////
// CTeachModelChangeDlg dialog
 
class CTeachModelChangeDlg : public CDialog
{
	// Doolin kwangilshin 2017. 08. 22.
	//
	enum
	{
		eAxisY	= 0,
		eAxisEnd
	};
	// End.

private:
	int				m_iWorkGroup;
	int				m_iSelUnit;
	UINT			m_uiTimerID;
	int				m_iJogSelect;
	int				m_iSelectedPos;
	int				m_iFixModel;

	/** 
	 * Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	 */
	double			m_dDisplayOffsetZ;

	// 전체 Position
	double			m_rgdFixedPosData[DEF_MODEL_CHANGE_MAX_POSITION];
	double			m_rgdOffsetPosData[DEF_MODEL_CHANGE_MAX_POSITION];
	double			m_rgdModelPosData[DEF_MODEL_CHANGE_MAX_POSITION];

	CBtnEnh			m_BtnPos[DEF_MODEL_CHANGE_MAX_POSITION];
	CBtnEnh			m_sCoord[6];
	CBtnEnh			m_sStats[2];
	CBtnEnh			m_BtnCmd[2];

	MModelChange*	m_plnkModelChange;
	STeachingInfo	m_sTeachingInfo;
	MManageOpPanel*	m_plnkC_MngOpPanel;

	CBrush m_brBckColor; 

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

// Construction
public:
	CTeachModelChangeDlg(int iWorkGroup, int iSel, CWnd* pParent = NULL);   // standard constructor

	/**
	 * 목표위치, Mark Offset, 기타보정, 현재위치, 편차 Display (XYT축)
	 */

	void UpdateDisplay();
	/**
	 * 고정좌표, Offset좌표 Display (XYT축)
	 */

	void UpdateCoordinate();
	/**
	 * 선택 Position 변경 (XYT축)
	 */

	void SelectPosition(int iPosIndex);
	/**
	 * Command Button 상태 Update
	 */

	void UpdateButton();

// Dialog Data
	//{{AFX_DATA(CTeachModelChangeDlg)
	enum { IDD = IDD_TEACH_MODELCHANGE };

	// Doolin kwangilshin 2017. 08. 24.
	// Change Array Type.
	//
	CBtnEnh	m_ctrlBtnAxis[eAxisEnd];
	// End.
	
	// Doolin kwangilshin 2017. 08. 24.
	//
	//CMSFlexGrid	m_grd_AxisSnsState[eAxisEnd];
	// End.

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachModelChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachModelChangeDlg)
	afx_msg void OnJog();
	afx_msg void OnAxisY();
	afx_msg void OnFeederPos0();
	afx_msg void OnFeederPos1();
	afx_msg void OnFeederPos2();
	afx_msg void OnBmMove();
	afx_msg void OnPosSet();
	afx_msg void OnSave();
	afx_msg void OnExit();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickCmModelChangeExpand();
	afx_msg void OnClickCmModelChangeShrink();
	afx_msg void OnClickRegisterMark();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEACHMODELCHANGEDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_)
