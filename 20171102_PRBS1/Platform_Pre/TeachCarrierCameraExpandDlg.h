//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHCARRIERCAMERAEXPANDDLG_H__86CAE334_B371_4D33_AA90_D6D486985299__INCLUDED_)
#define AFX_TEACHCARRIERCAMERAEXPANDDLG_H__86CAE334_B371_4D33_AA90_D6D486985299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeachTransferDlg.h : header file
//

#include "MTabCarrier.h"
#include "DefSystem.h"
#include "DefTabCarrier.h"

class MManageOpPanel;
class MSystemData;
class IAxis;

/////////////////////////////////////////////////////////////////////////////
// CTeachCarrierCameraExpandDlg dialog

const int DEF_DLG_CAMDRA_EXPAND_POS_MAX_BTN		= 2;
const int DEF_DLG_CAMDRA_EXPAND_COMMAND_MAX_BTN	= 10; //Command Button ¼ö

const int DEF_DLG_CAMDRA_EXPAND_AXIS_TITLE					= 1; //Ãà¼ö
const int DEF_DLG_CAMDRA_EXPAND_TEACH_MAX_COORDINATE_TYPE	= 6; //ÁÂÇ¥Á¾·ù(Ex. °íÁ¤ÁÂÇ¥, º¯ÁÂÇ¥, Offset, ...)

const int ERR_CAMDRA_EXPAND_TEACH_DLG_SUCCESS				= 0;

class CTeachCarrierCameraExpandDlg : public CDialog
{
public:

	// Doolin kwangilshin 2017. 08. 22.
	//
	enum
	{
		eAxisX	= 0,
		eAxisEnd
	};
	// End.

private:
	const int m_iFixModel;
	int m_iExpandID;
	const int m_iGroupID;//SJ_YYK 150817 Add...
	int m_iExpandOldID; //SJ_YYK 150817 Add...
	int m_iJogSelect; //SJ_YYK 150817 Add...

	MTabCarrier* m_plnkTabCarrier;
	MManageOpPanel*	m_plnkC_MngOpPanel;
	MSystemData* m_plnkSystemData;

	double m_dDisplayExpandOffset;
	
	double	m_rgdExpandFixedPosData[DEF_TABCARRIER_EXPAND_MAX_POS];
	double	m_rgdExpandModelPosData[DEF_TABCARRIER_EXPAND_MAX_POS];
	double	m_rgdExpandOffsetPosData[DEF_TABCARRIER_EXPAND_MAX_POS];

	CBtnEnh m_sCoordTitle[DEF_DLG_CAMDRA_EXPAND_TEACH_MAX_COORDINATE_TYPE];
	CBtnEnh m_sCoord[DEF_DLG_CAMDRA_EXPAND_TEACH_MAX_COORDINATE_TYPE][DEF_DLG_CAMDRA_EXPAND_AXIS_TITLE];
	CBtnEnh m_BtnPos[DEF_DLG_CAMDRA_EXPAND_POS_MAX_BTN];
	CBtnEnh m_BtnCmd[DEF_DLG_CAMDRA_EXPAND_COMMAND_MAX_BTN];
	CBtnEnh	m_btnNextExpand; //SJ_YYK 150817 Add...
	CBtnEnh	m_btnPrevExpand; //SJ_YYK 150817 Add...
	CBtnEnh		m_lblWorkerNo; //SJ_YYK 150817 Add...

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

	void SetWorkExpand(const int iExpandID);

// Construction
public:
	CTeachCarrierCameraExpandDlg(int iFixModel, int iGroupID, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeachCarrierCameraExpandDlg)
	enum { IDD = IDD_TEACH_CARRIERCAMERAEXPAND };
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
	//{{AFX_VIRTUAL(CTeachCarrierCameraExpandDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachCarrierCameraExpandDlg)
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
	afx_msg void OnClickSelectPrevios();
	afx_msg void OnClickSelectNext();
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
