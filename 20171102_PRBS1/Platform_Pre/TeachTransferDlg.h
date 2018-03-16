//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHTRANSFERDLG_H__86CAE334_B371_4D33_AA90_D6D486985299__INCLUDED_)
#define AFX_TEACHTRANSFERDLG_H__86CAE334_B371_4D33_AA90_D6D486985299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeachTransferDlg.h : header file
//

#include "MPanelTransfer.h"
#include "DefSystem.h"
#include "DefPanelTransfer.h"

class MManageOpPanel;
class IAxis;

/////////////////////////////////////////////////////////////////////////////
// CTeachTransferDlg dialog

const int DEF_TRANSFER_DISPLAY_MAX_POS		= 3;
const int DEF_DLG_TRANSFER_POS_MAX_BTN		= 3;
const int DEF_DLG_TRANSFER_COMMAND_MAX_BTN	= 10; //Command Button ¼ö

const int DEF_DLG_TRANSFER_AXIS_TITLE					= 1; //Ãà¼ö
const int DEF_DLG_TRANSFER_TEACH_MAX_COORDINATE_TYPE	= 6; //ÁÂÇ¥Á¾·ù(Ex. °íÁ¤ÁÂÇ¥, º¯ÁÂÇ¥, Offset, ...)

const int ERR_TRANSFER_TEACH_DLG_SUCCESS				= 0;

class CTeachTransferDlg : public CDialog
{
	// Doolin kwangilshin 2017. 08. 22.
	//
	enum
	{
		eAxisXFirst		= 0,
		eAxisXSecond	= 1,
		eAxisEnd		= 1,
	};
	// End.

private:
	const int m_iFixModel;
	const int m_iTransferID;

	MPanelTransfer* m_plnkPanelTransfer;
	MManageOpPanel*	m_plnkC_MngOpPanel;

	double m_dDisplayTransferOffset;
	
	double	m_rgdTransferFixedPosData[DEF_PANEL_TRANSFER_MAX_POSITION];
	double	m_rgdTransferModelPosData[DEF_PANEL_TRANSFER_MAX_POSITION];
	double	m_rgdTransferOffsetPosData[DEF_PANEL_TRANSFER_MAX_POSITION];

	CBtnEnh m_sCoordTitle[DEF_DLG_TRANSFER_TEACH_MAX_COORDINATE_TYPE];
	CBtnEnh m_sCoord[DEF_DLG_TRANSFER_TEACH_MAX_COORDINATE_TYPE][DEF_DLG_TRANSFER_AXIS_TITLE];
	CBtnEnh m_BtnPos[DEF_DLG_TRANSFER_POS_MAX_BTN];
	CBtnEnh m_BtnCmd[DEF_DLG_TRANSFER_COMMAND_MAX_BTN];

	int	m_iSelPos;
	int m_iJogSelectIndex;	//0: X1, 1: X2, 2: Multi
	UINT m_uiTimerID;

	BOOL m_bPanelTransferVacSts;
	BOOL m_bPanelTransferVacOnCheck;

	BOOL m_bPanelTransferUpSts;
	BOOL m_bPanelTransferUpCheck;

	BOOL m_bAirCVVacSts;
	BOOL m_bAirCVVacOnCheck;

	BOOL m_bAirCVBlowOnSts;
	BOOL m_bAirCVBlowOnCheck;

// Construction
public:
	CTeachTransferDlg(int iFixModel, int iTransferID, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeachTransferDlg)
	enum { IDD = IDD_TEACH_TRANSFER };
	CBtnEnh	m_btnSave;
	CBtnEnh	m_btnJogSel;
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
	//{{AFX_VIRTUAL(CTeachTransferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachTransferDlg)
	afx_msg void OnClickExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickJog();
	afx_msg void OnClickAfuPos0();
	afx_msg void OnClickAfuPos1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickSave();
	afx_msg void OnClickAfuX();
	afx_msg void OnClickCmMove();
	afx_msg void OnClickPosSet();
	afx_msg void OnClickCmTransferUpDown();
	afx_msg void OnClickCmTransferVacuum();
	afx_msg void OnClickCmTransferUpvac();
	afx_msg void OnClickCmTransferReleasedown();
	afx_msg void OnClickCmAirCvOnoff();
	afx_msg void OnClickCmAirCvVacOnoff();
	afx_msg void OnClickAfuPos2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetDlgCaption();
	void UpdateDisplay();
	void UpdateCoordinate();
	void SelectPosition(int iPosIndex);
	void DisplayLanguage();
	void UpdateButton();
	void SetJogObject();

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

#endif // !defined(AFX_TEACHTRANSFERDLG_H__86CAE334_B371_4D33_AA90_D6D486985299__INCLUDED_)
