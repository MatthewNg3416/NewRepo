//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALPANELALIGNERDLG_H__D62E3AAB_4F16_400D_90C2_71DCB6E3D196__INCLUDED_)
#define AFX_MANUALPANELALIGNERDLG_H__D62E3AAB_4F16_400D_90C2_71DCB6E3D196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualPanelAlignerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManualPanelAlignerDlg dialog

class MPanelAligner;
class MPanelTransfer;
class MModelChange;
class IIO;

#include "MTickTimer.h"

class CManualPanelAlignerDlg : public CDialog
{
// Construction
public:
	CManualPanelAlignerDlg(CWnd* pParent = NULL);   // standard constructor

	MPanelAligner*		m_plnkPanelAligner;
	MPanelTransfer*		m_plnkPanelTransfer;
	MPanelTransfer*		m_plnkPanelTransferOut;
	MModelChange*		m_plnkModelChange;
	IIO*				m_plnkIIO;

	UINT	m_uiTimerID;

// Dialog Data
	//{{AFX_DATA(CManualPanelAlignerDlg)
	enum { IDD = IDD_MANUAL_PANEL_ALIGNER };
	CBtnEnh	m_btnPanelAlignerPickupUp;
	CBtnEnh	m_btnPanelAlignerPickupDown;
	CBtnEnh	m_btnPanelAlignerVacuumOn;
	CBtnEnh	m_btnPanelAlignerVacuumOff;
	CBtnEnh	m_btnPanelAlignerAirCVBlowOn;
	CBtnEnh	m_btnPanelAlignerAirCVBlowOff;
	CBtnEnh	m_btnPanelAlignerAirVacuumAllOn;
	CBtnEnh	m_btnPanelAlignerAirVacuumAllOff;
	CBtnEnh	m_btnPanelAlignerAirVacuumPlate1On;
	CBtnEnh	m_btnPanelAlignerAirVacuumPlate1Off;
	CBtnEnh	m_btnPanelAlignerAirVacuumPlate2On;
	CBtnEnh	m_btnPanelAlignerAirVacuumPlate2Off;
	CBtnEnh	m_btnPanelAlignerAirVacuumPlate3On;
	CBtnEnh	m_btnPanelAlignerAirVacuumPlate3Off;
	CBtnEnh	m_btnPanelAlignerAirVacuumPlate4On;
	CBtnEnh	m_btnPanelAlignerAirVacuumPlate4Off;
	CBtnEnh	m_btnPanelTransferUp;
	CBtnEnh	m_btnPanelTransferDown;
	CBtnEnh	m_btnPanelTransferVacuumOn;
	CBtnEnh	m_btnPanelTransferVacuumOff;
	CBtnEnh	m_btnPanelTransferOutUp;
	CBtnEnh	m_btnPanelTransferOutDown;
	CBtnEnh	m_btnPanelTransferOutVacuumOn;
	CBtnEnh	m_btnPanelTransferOutVacuumOff;
	CBtnEnh	m_LblPanelDetectTIn;
	CBtnEnh	m_LblPanelDetectTOut;
	
	CBtnEnh	m_btnModelChangeAirRailFor1;
	CBtnEnh	m_btnModelChangeAirRailBack1;
	CBtnEnh	m_btnModelChangeAirRailFor2;
	CBtnEnh	m_btnModelChangeAirRailBack2;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualPanelAlignerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;
	// Generated message map functions
	//{{AFX_MSG(CManualPanelAlignerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickPanelAlignerPickupUp();
	afx_msg void OnClickPanelAlignerPickupDown();
	afx_msg void OnClickPanelAlignerVacuumOn();
	afx_msg void OnClickPanelAlignerVacuumOff();
	afx_msg void OnClickPanelAlignerAirCVBlowOn();
	afx_msg void OnClickPanelAlignerAirCVBlowOff();
	afx_msg void OnClickPanelAlignerAirVacuumAllOn();
	afx_msg void OnClickPanelAlignerAirVacuumAllOff();
	afx_msg void OnClickPanelAlignerAirVacuumPlate1On();
	afx_msg void OnClickPanelAlignerAirVacuumPlate1Off();
	afx_msg void OnClickPanelAlignerAirVacuumPlate2On();
	afx_msg void OnClickPanelAlignerAirVacuumPlate2Off();
	afx_msg void OnClickPanelAlignerAirVacuumPlate3On();
	afx_msg void OnClickPanelAlignerAirVacuumPlate3Off();
	afx_msg void OnClickPanelAlignerAirVacuumPlate4On();
	afx_msg void OnClickPanelAlignerAirVacuumPlate4Off();
	afx_msg void OnClickPanelTransferUp();
	afx_msg void OnClickPanelTransferDown();
	afx_msg void OnClickPanelTransferVacuumOn();
	afx_msg void OnClickPanelTransferVacuumOff();
	afx_msg void OnClickExit();
	afx_msg void OnClickPanelTransferOutUp();
	afx_msg void OnClickPanelTransferOutDown();
	afx_msg void OnClickPanelTransferVacuumOutOn();
	afx_msg void OnClickPanelTransferVacuumOutOff();
	afx_msg void OnClickModelchangeAirrailFor1();
	afx_msg void OnClickModelchangeAirrailBack1();
	afx_msg void OnClickModelchangeAirrailFor2();
	afx_msg void OnClickModelchangeAirrailBack2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MTickTimer	m_AirTimer;

	WCHAR		m_szTimeValue[100];

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALPANELALIGNERDLG_H__D62E3AAB_4F16_400D_90C2_71DCB6E3D196__INCLUDED_)
