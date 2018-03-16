//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALMAINFORM_H__772D6228_CB88_490D_AC2E_1B64D54C2B4F__INCLUDED_)
#define AFX_MANUALMAINFORM_H__772D6228_CB88_490D_AC2E_1B64D54C2B4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualMainForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManualMainForm form view

const int ERR_MANUAL_MAINFORM_SUCCESS				= 0;
const int ERR_MANUAL_MAINFORM_FAILURE				= 1;
const int ERR_MANUAL_MAINFORM_SKIP_ERROR_DISPLAY	= 2;

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DefSystem.h"

class MSystemData;
class MProcessData;

class IVision;
class MTabCarrier;
class MPanelAligner;
class MCameraCarrier;

class MCtrlTabCarrier;
class MCtrlPanelAligner;

class MPos_XY;

class CManualMainForm : public CFormView
{
private:
	//101001.KKY_____
	BOOL			m_bInitialized;
	EOpMode			m_eOldOpMode;
	//_______________

	MSystemData*	m_plnkSystemData;
	MProcessData*	m_plnkProcessData;

	IVision*		m_plnkVision;

	MTabCarrier*	m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	MTabCarrier*	m_plnkRTabCarrier[DEF_MAX_TABCARRIER];
	MPanelAligner*	m_plnkPanelAligner;

	MCtrlTabCarrier*	m_plnkCtrlTabCarrier[DEF_MAX_TABCARRIER];
	MCtrlTabCarrier*	m_plnkCtrlRTabCarrier[DEF_MAX_TABCARRIER];
	MCtrlPanelAligner*	m_plnkCtrlPanelAligner;

	int m_iCamNo;

protected:
	CManualMainForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CManualMainForm)

// Form Data
public:
	//{{AFX_DATA(CManualMainForm)
	enum { IDD = IDD_MANUAL_MAIN_FORM };
	CBtnEnh	m_lblIOJob;
	CBtnEnh	m_lblInitJob;
	CBtnEnh	m_lblBatchJob;
	CBtnEnh	m_lblUnitJob;
	//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualMainForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush	m_brBckColor;
	
	virtual ~CManualMainForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CManualMainForm)
	afx_msg void OnPaint();
	afx_msg void OnIOCheck();
	afx_msg void OnLimitCheck();
	afx_msg void OnReturnOrigin();
	afx_msg void OnPanelAlign();
	afx_msg void OnClickRollerConveyor();
	afx_msg void OnClickTabmounter();
	afx_msg void OnClickTabCarrier();
	afx_msg void OnClickTabFeeder();
	afx_msg void OnPanelAlignerDlg();
	afx_msg void OnClickTabcarrierR();
	afx_msg void OnClickTabfeederR();
	afx_msg void OnMountTabIC();
	afx_msg void OnClickAutoChangeTabicReel();
	afx_msg void OnClickAutoChangeTabicReel2();
	afx_msg void OnHeaterPowerReset();
	afx_msg void OnSercosInitialize();
	afx_msg void OnClickCameraExpandShrink();
	afx_msg void OnCheckPunchRepeatability();
	afx_msg void OnClickSetupTest();
	afx_msg LRESULT OnDisplayLanguage(UINT wParam, LPARAM lParam);
	afx_msg void OnClickEngStop();
	afx_msg void OnClickPanelTransferCenter();
	afx_msg void OnClickPanelAligner3();
	afx_msg void OnClickTabicLoadCarrier();
	afx_msg void OnClickTiltingJob();
	afx_msg void OnClickFrontTabmounterMoveToLoadPos();
	afx_msg void OnClickRearTabmounterMoveToLoadPos();
	afx_msg void OnClickFrontTabcarrierMoveToReadyPos();
	afx_msg void OnClickRearTabcarrierMoveToReadyPos();
	afx_msg void OnClickFrontTabcarrierMoveToUnloadPos();
	afx_msg void OnClickRearTabcarrierMoveToUnloadPos();
	afx_msg void OnClickTabicLoadAllTabmounter();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClickTabicLoadAllTabmounterRear();
	afx_msg void OnClickTabicRemoveFront();
	afx_msg void OnClickTabicRemoveRear();
	afx_msg void OnClickManualTurnHandler();
	afx_msg void OnClickPanelTransferIn();
	afx_msg void OnClickFrontGantryMoveToCleanPos();
	afx_msg void OnClickRearGantryMoveToCleanPos();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	
	//101001.KKY_____
	void showButtonManual();
	void showButtonAuto();
	//_______________
	int getWorkOption(EWorkingSide* peWorkSide, int* piGroupNo, int* piTab);
	int alignPanel();
	int operateTabICInspection(EWorkingSide eWorkSide, int iGroupNo, int iWorkTabNo, int iTargetStatus, MPos_XY* prgAlignOffset = NULL);
	int autoChangeTabICReel(int iFeederNo);
	int moveGateInspectionCameraToSafePosition(EWorkingSide eWorkSide, int iGroupNo);
	int checkTabPunchRepeatability(EWorkingSide eWorkSide, MCameraCarrier* plnkCamCarrier, int iTabNo);
	int tabICLoadAllTabMounter(int iWorkGroup, int iCarrierWorkGroup);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALMAINFORM_H__772D6228_CB88_490D_AC2E_1B64D54C2B4F__INCLUDED_)
