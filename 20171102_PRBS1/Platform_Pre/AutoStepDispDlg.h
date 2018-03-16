#if !defined(AFX_STEPDISPDLG_H__28BA00BC_493E_409D_AD47_5148E0CAF453__INCLUDED_)
#define AFX_STEPDISPDLG_H__28BA00BC_493E_409D_AD47_5148E0CAF453__INCLUDED_

#include "btnenh.h"
#include "DefSystem.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StepDispDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoStepDispDlg dialog

const int DEF_MAX_DISP_PROCESS_UNIT_NUM		= 32;
const int DEF_MAX_STEP_DISP_UNIT_NUM		= DEF_MAX_PROCESS_INSTANCE;
const int DEF_MAX_DATA_DISPLAY_ITEM_NUM		= 9;
const int DEF_MAX_WORKSIDE_PER_EQUIP_UNIT	= 2;

class MProcessData;
class MCtrlPanelAligner;		

class MTrsPanelAligner;
class MTrsPanelTransfer;
class MTrsPanelTransferCenter;
class MTrsPanelTransferOut;
//class MTrsLoadConveyor;
//class MTrsUnloadConveyor;
class MTrsTabCarrier;
class MTrsTabFeeder;	
class MTrsTabMounter;
class MTrsInspectionCamera;
class MTrsAutoManager;	
class MTrsTHandler;

class CAutoStepDispDlg : public CDialog
{
// Construction
public:
	void StartDlgTimer();
	void StopDlgTimer();

	CAutoStepDispDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoStepDispDlg)
	enum { IDD = IDD_AUTO_STEP_DISP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CBtnEnh m_rglblProcessName[DEF_MAX_DISP_PROCESS_UNIT_NUM];
	CBtnEnh m_rglblProcessStep[DEF_MAX_DISP_PROCESS_UNIT_NUM];
	CBtnEnh m_rglblProcessITI[DEF_MAX_DISP_PROCESS_UNIT_NUM];

	CBtnEnh m_rglblProcessDataValue[DEF_MAX_WORKSIDE_PER_EQUIP_UNIT][DEF_MAX_DATA_DISPLAY_ITEM_NUM];
	CBtnEnh m_rglblWorkSideName[DEF_MAX_WORKSIDE_PER_EQUIP_UNIT];

	CBtnEnh m_lblNextPanelTabICLoadStart;

	EWorkingSide m_rgeWorkSide[DEF_MAX_WORKSIDE_PER_EQUIP_UNIT];
	int m_rgiMaxTabQtyPerWorkSide[DEF_MAX_WORKSIDE_PER_EQUIP_UNIT];
	UINT m_uiTimerID;

	MTrsPanelAligner*	m_plnkTrsPanelAligner;
	MTrsPanelTransferCenter* m_plnkTrsPanelTransferC;
	MTrsPanelTransfer*	m_plnkTrsPanelTransfer;
	MTrsPanelTransferOut*	m_plnkTrsPanelTransferOut;
	MTrsTabCarrier*		m_plnkTrsTabCarrier[DEF_MAX_TABCARRIER];
	MTrsTabCarrier*		m_plnkTrsRTabCarrier[DEF_MAX_TABCARRIER];
	MTrsTabFeeder*		m_plnkTrsTabFeeder[DEF_MAX_TABFEEDER];
	MTrsTabMounter*		m_plnkTrsTabMounter[DEF_MAX_TABMOUNTER];
	MTrsTabMounter*		m_plnkTrsRTabMounter[DEF_MAX_TABMOUNTER];
	MTrsInspectionCamera* m_plnkTrsInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTrsInspectionCamera* m_plnkTrsRInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTrsAutoManager*	m_plnkTrsAutoManager;
	MTrsTHandler*		m_plnkTrsTurnHandler; //SJ_YYK 161104 Add.

	MCtrlPanelAligner*	m_plnkCtrlPanelAligner;
	MProcessData*		m_plnkProcessData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoStepDispDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void updateDisplay();
	void updateButton();

protected:
	CBrush m_brBckColor;
	int m_iSelColor;

	// Generated message map functions
	//{{AFX_MSG(CAutoStepDispDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeColor();
	afx_msg void OnExit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEPDISPDLG_H__28BA00BC_493E_409D_AD47_5148E0CAF453__INCLUDED_)
