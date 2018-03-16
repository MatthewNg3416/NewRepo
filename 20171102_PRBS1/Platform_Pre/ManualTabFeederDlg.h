//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALTABFEEDERDLG_H__61EE1483_767F_4AE2_B5EC_6E7EA231B2F7__INCLUDED_)
#define AFX_MANUALTABFEEDERDLG_H__61EE1483_767F_4AE2_B5EC_6E7EA231B2F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualTabFeederDlg.h : header file
//
//#include "DefSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CManualTabFeederDlg dialog

class MTabFeeder;
class MCtrlTabFeeder;
class MPreBonderData;
class IOpPanel;
class IIO;

const int DEF_MAX_CMD				= 3; // Command Button ¼ö
const int DEF_CMD_HOME_SPROCKET1	= 0; 
const int DEF_CMD_HOME_SPROCKET2	= 1; 
const int DEF_CMD_REVERSE			= 2; 

class CManualTabFeederDlg : public CDialog 
{
// Construction
private:
	int m_iGroupNo;

// Construction
public:
	CManualTabFeederDlg(int iWorkSide, CWnd* pParent = NULL);   // standard constructor
	
	MTabFeeder*			m_plnkTabFeeder;
	MCtrlTabFeeder*		m_plnkCtrlTabFeeder;
	MPreBonderData*		m_plnkPreBonderData;
	IOpPanel*			m_plnkOpPanel;
	IIO*				m_plnkIO;

	UINT	m_uiTimerID;

	BOOL	m_rgbCmdState[DEF_MAX_CMD];
	BOOL	m_bOnWithdrawingReel;

	CBtnEnh m_BtnCmd[DEF_MAX_CMD];

    int iCutTestRealCount;
    int iCutTestSetCount;

// Dialog Data
	//{{AFX_DATA(CManualTabFeederDlg)
	enum { IDD = IDD_MANUAL_TABFEEDER };
	CBtnEnh	m_btnPress1VacOn;
	CBtnEnh	m_btnPress1VacOff;
	CBtnEnh	m_btnNewReelVacOn;
	CBtnEnh	m_btnNewReelVacOff;
	CBtnEnh	m_btnReelDown;
	CBtnEnh	m_btnReelUp;
	CBtnEnh	m_btnReelClampDown;
	CBtnEnh	m_btnReelClampUp;
	CBtnEnh	m_btnSupplierChuckLock;
	CBtnEnh	m_btnSupplierChuckUnlock;
	CBtnEnh	m_btnCuttingSupportLeftDown;
	CBtnEnh	m_btnCuttingSupportLeftUp;
	CBtnEnh	m_btnCuttingSupportRightDown;
	CBtnEnh	m_btnCuttingSupportRightUp;
	CBtnEnh	m_btnFeederSprocketDown;
	CBtnEnh	m_btnFeederSprocketUp;
	CBtnEnh	m_btnTensionSprocketDown;
	CBtnEnh	m_btnTensionSprocketUp;
	CBtnEnh	m_btnRecoverySprocketDown;
	CBtnEnh	m_btnRecoverySprocketUp;
	CBtnEnh	m_btnSprocketAllDown;
	CBtnEnh	m_btnSprocketAllUp;
	CBtnEnh	m_btnAttachPressDown;
	CBtnEnh	m_btnAttachPressUp;
	CBtnEnh	m_btnAttachReelCutterUp;
	CBtnEnh	m_btnAttachReelCutterDown;
	CBtnEnh	m_btnAttachReelForward;
	CBtnEnh	m_btnAttachReelBackward;
	CBtnEnh	m_btnRecoveryMotorOn;
	CBtnEnh	m_btnRecoveryMotorOff;
	CBtnEnh	m_btnPReelMotorOn;
	CBtnEnh	m_btnPReelReverseMotorOn;
	CBtnEnh	m_btnPReelMotorOff;
	CBtnEnh	m_btnSupplierTensionMotorOff;
	CBtnEnh	m_btnSupplierTensionSmall;
	CBtnEnh	m_btnBrushMotorOn;
	CBtnEnh	m_btnBrushMotorOff;
	CBtnEnh	m_lTabPressMoldCheck;
	CBtnEnh m_lBadTabDetect1;
	CBtnEnh m_lBadTabDetect2;
	CBtnEnh	m_lTensionUpperLimit;
	CBtnEnh	m_lTensionUpper;
	CBtnEnh	m_lTensionLower;
	CBtnEnh	m_lTensionLowerLimit;
	CBtnEnh	m_LblRecoveryTapeDetect;
	CBtnEnh m_LblUnfoldDetect;
	CBtnEnh	m_LblCurrentReelDetect;
	CBtnEnh	m_LblNewReelDetect;
	CBtnEnh	m_bCutTABICTest;
	CBtnEnh m_bTabPressMoldCheck;
	CBtnEnh	m_btnWithdrawReel;
	CBtnEnh	m_btnSprocketClutchOn;
	CBtnEnh	m_btnSprocketClutchOff;
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualTabFeederDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;
	// Generated message map functions
	//{{AFX_MSG(CManualTabFeederDlg)
	afx_msg void OnClickPress1VacOn();
	afx_msg void OnClickPress1VacOff();
	afx_msg void OnClickNewReelVacOn();
	afx_msg void OnClickNewReelVacOff();
	afx_msg void OnClickReelUp();
	afx_msg void OnClickReelDown();
	afx_msg void OnClickReelClampUp();
	afx_msg void OnClickReelClampDown();
	afx_msg void OnClickSupplierChuckLock();
	afx_msg void OnClickSupplierChuckUnlock();
	afx_msg void OnClickCuttingSupportLeftUp();
	afx_msg void OnClickCuttingSupportLeftDown();
	afx_msg void OnClickCuttingSupportRightUp();
	afx_msg void OnClickCuttingSupportRightDown();
	afx_msg void OnClickFeederSprocketOpen();
	afx_msg void OnClickFeederSprocketClose();
	afx_msg void OnClickTensionSprocketOpen();
	afx_msg void OnClickTensionSprocketClose();
	afx_msg void OnClickRecoverySprocketOpen();
	afx_msg void OnClickRecoverySprocketClose();
	afx_msg void OnClickSprocketAllOpen();
	afx_msg void OnClickSprocketAllClose();
	afx_msg void OnClickAttachPressUp();
	afx_msg void OnClickAttachPressDown();
	afx_msg void OnClickAttachReelCutterOpen();
	afx_msg void OnClickAttachReelCutterCutting();
	afx_msg void OnClickAttachReelForward();
	afx_msg void OnClickAttachReelBackward();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickExit();
	afx_msg void OnClickSprocket1Home();
	afx_msg void OnClickSprocket2Home();
	afx_msg void OnClickOnetab();
	afx_msg void OnClickOnePitch();
	afx_msg void OnClickTentab();
	afx_msg void OnClickTenPitch();
	afx_msg void OnClickFeedingReverse();
	afx_msg void OnClickRecoveryMotorOn();
	afx_msg void OnClickRecoveryMotorOff();
	afx_msg void OnClickPReelMotorOn();
	afx_msg void OnClickPReelMotorOff();
	afx_msg void OnClickSupplierTensionMotorOff();
	afx_msg void OnClickSupplierTensionSmall();
	afx_msg void OnClickBrushMotorOn();
	afx_msg void OnClickBrushMotorOff();
	afx_msg void OnClickPressTabic();
	afx_msg void OnClickForcedHoming();
	afx_msg void OnRemoveMold();
	afx_msg void OnWithdrawReel();
	afx_msg void OnPaint();
	afx_msg void OnClickSprocket1ClutchOn();
	afx_msg void OnClickSprocket1ClutchOff();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int freeWorkState();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALTABFEEDERDLG_H__61EE1483_767F_4AE2_B5EC_6E7EA231B2F7__INCLUDED_)
