//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MATERIALCHANGEDLG_H__D602B86E_B432_4648_A7FB_473FF3F7AFB4__INCLUDED_)
#define AFX_MATERIALCHANGEDLG_H__D602B86E_B432_4648_A7FB_473FF3F7AFB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaterialChangeDlg.h : header file
//
#include "DefTabFeeder.h"

/////////////////////////////////////////////////////////////////////////////
// CMaterialChangeDlg dialog
class MTabFeeder;
class MCtrlTabFeeder;
class MPreBonderData;
class IOpPanel;
class MManageOpPanel;
class MSystemData;

const int DEF_MAX_CMD				= 3; // Command Button ¼ö
const int DEF_CMD_HOME_SPROCKET1	= 0; 
const int DEF_CMD_HOME_SPROCKET2	= 1; 
const int DEF_CMD_REVERSE			= 2; 


class CMaterialChangeDlg : public CDialog
{
// Construction

private:
	int m_iGroupNo;

	// ÀüÃ¼ Position
	double			m_rgdModelPosData[DEF_BADTABDETECTOR_MAX_POSITION];
	double			m_rgdOffsetPosData[DEF_BADTABDETECTOR_MAX_POSITION];

public:
	CMaterialChangeDlg(int iGroupNo, CWnd* pParent = NULL);   // standard constructor
	
	MTabFeeder*			m_plnkTabFeeder;
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder;
	MPreBonderData*		m_plnkPreBonderData;
	IOpPanel*			m_plnkOpPanel;
	MSystemData*        m_plnkSystemData;

	MManageOpPanel*	m_plnkC_MngOpPanel;

	UINT	m_uiTimerID;

	BOOL	m_rgbCmdState[DEF_MAX_CMD];
	BOOL	m_bOnWithdrawingReel;

	CBtnEnh m_BtnCmd[DEF_MAX_CMD];

    int iCutTestRealCount;
    int iCutTestSetCount;



// Dialog Data
	//{{AFX_DATA(CMaterialChangeDlg)
	enum { IDD = IDD_MATERIAL_CHANGE_DLG };
	CBtnEnh	m_btnFeederSprocketUp;
	CBtnEnh	m_btnFeederSprocketDown;
	CBtnEnh	m_btnTensionSprocketDown;
	CBtnEnh	m_btnTensionSprocketUp;
	CBtnEnh m_btnRecoverySprocketDown;
	CBtnEnh m_btnRecoverySprocketUp;
	CBtnEnh	m_btnSprocketAllDown;
	CBtnEnh	m_btnSprocketAllUp;
	CBtnEnh	m_btnSupplierChuckLock;
	CBtnEnh	m_btnSupplierChuckUnlock;
	CBtnEnh	m_btnSupplierTensionSmall;
	CBtnEnh	m_btnSupplierTensionMotorOff;
	CBtnEnh	m_btnRecoveryMotorOn;
	CBtnEnh	m_btnRecoveryMotorOff;
	CBtnEnh	m_btnPReelMotorOn;
	CBtnEnh	m_btnPReelMotorOff;
	CBtnEnh	m_btnPReelReverseMotorOn;
	CBtnEnh	m_btnBrushMotorOn;
	CBtnEnh	m_btnBrushMotorOff;
	CBtnEnh	m_btnReelDown;
	CBtnEnh	m_btnReelUp;
	CBtnEnh	m_btnReelClampDown;
	CBtnEnh	m_btnReelClampUp;
	CBtnEnh	m_btnSprocketClutchOn;
	CBtnEnh	m_btnSprocketClutchOff;
	//}}AFX_DATA
	CBtnEnh			m_sTensionOffset;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaterialChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;
	// Generated message map functions
	//{{AFX_MSG(CMaterialChangeDlg)
	afx_msg void OnClickTensionSprocketUp();
	afx_msg void OnClickTensionSprocketDown();
	afx_msg void OnClickFeederSprocketUp();
	afx_msg void OnClickFeederSprocketDown();
	afx_msg void OnClickRecoverySprocketUp();
	afx_msg void OnClickRecoverySprocketDown();
	afx_msg void OnClickSprocketAllUp();
	afx_msg void OnClickSprocketAllDown();
	afx_msg void OnClickSupplierChuckLock();
	afx_msg void OnClickSupplierChuckUnlock();
	afx_msg void OnClickSupplierTensionSmall();
	afx_msg void OnClickSupplierTensionMotorOff();
	afx_msg void OnClickRecoveryMotorOn();
	afx_msg void OnClickRecoveryMotorOff();
	afx_msg void OnClickPReelMotorOn();
	afx_msg void OnClickPReelMotorOff();
	afx_msg void OnClickPReelReverseMotorOn();
	afx_msg void OnClickBrushMotorOn();
	afx_msg void OnClickBrushMotorOff();
	afx_msg void OnClickHomeSprocket1();
	afx_msg void OnClickHomeSprocket2();
	afx_msg void OnClickForcedHoming();
	afx_msg void OnClickOnetab();
	afx_msg void OnClickOnePitch();
	afx_msg void OnClickTentab();
	afx_msg void OnClickTenPitch();
	afx_msg void OnClickFeedingReverse();
	afx_msg void OnClickExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickReelUp();
	afx_msg void OnClickReelDown();
	afx_msg void OnClickReelClampUp();
	afx_msg void OnClickReelClampDown();
	afx_msg void OnClickSprocket1ClutchOn();
	afx_msg void OnClickSprocket1ClutchOff();
	afx_msg void OnClickTabicLoadCarrierChange();
	afx_msg void OnClickTabCuttingOffsetChange();
	afx_msg void OnClickRegisterMarkChange();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	double getUserOffsetX_withDisplay();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATERIALCHANGEDLG_H__D602B86E_B432_4648_A7FB_473FF3F7AFB4__INCLUDED_)
