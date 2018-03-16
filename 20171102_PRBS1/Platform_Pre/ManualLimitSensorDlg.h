//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_LIMITSENSORDLG_H__DCBCEC59_ABF2_4A20_A3AD_7D2032851AFF__INCLUDED_)
#define AFX_LIMITSENSORDLG_H__DCBCEC59_ABF2_4A20_A3AD_7D2032851AFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LimitSensorDlg.h : header file
//
#include "DefAxisPreBonder.h"

/////////////////////////////////////////////////////////////////////////////
// CManualLimitSensorDlg dialog

//@const int DEF_LIMIT_SENSOR_MAX_PAGE	= 5;
const int DEF_LIMIT_SENSOR_MAX_PAGE	= 6;
const int DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE = 16;
const int DEF_LIMIT_SENSOR_MAX_MOTION_NUM = DEF_AXIS_MAX_AXIS;
const int DEF_SUCCESS = 0;		

class IAxis;

class CManualLimitSensorDlg : public CDialog
{
// Construction
public:
	void ServoOnOff(int iIndex, BOOL bOnOff);
	void UpdateButton();
	void UpdateDisplay();
	CManualLimitSensorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualLimitSensorDlg)
	enum { IDD = IDD_MANUAL_LIMIT_SENSOR };
	CBtnEnh	m_bNext;
	CBtnEnh	m_bPrev;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualLimitSensorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;
	CBtnEnh m_bServoOn[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	CBtnEnh m_bServoOff[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	CBtnEnh m_lNegSensor[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	CBtnEnh m_lHomeSensor[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	CBtnEnh m_lPosSensor[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	CBtnEnh m_lAxisName[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	CBtnEnh m_lAxisStatus[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	CBtnEnh m_lAxisNum[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];

	IAxis*	m_plnkAxis[DEF_LIMIT_SENSOR_MAX_MOTION_NUM];

	BOOL	m_bOldOnOffStatus[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	BOOL	m_bOldNegSensor[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	BOOL	m_bOldHomeSensor[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];
	BOOL	m_bOldPosSensor[DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE];

	int m_iCurrentPage;

	UINT	m_uiTimerID;

	// Generated message map functions
	//{{AFX_MSG(CManualLimitSensorDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnServoOn0();
	afx_msg void OnServoOn1();
	afx_msg void OnServoOn2();
	afx_msg void OnServoOn3();
	afx_msg void OnServoOn4();
	afx_msg void OnServoOn5();
	afx_msg void OnServoOn6();
	afx_msg void OnServoOn7();
	afx_msg void OnServoOn8();
	afx_msg void OnServoOn9();
	afx_msg void OnServoOn10();
	afx_msg void OnServoOn11();
	afx_msg void OnServoOn12();
	afx_msg void OnServoOn13();
	afx_msg void OnServoOn14();
	afx_msg void OnServoOn15();
	afx_msg void OnServoOff0();
	afx_msg void OnServoOff1();
	afx_msg void OnServoOff2();
	afx_msg void OnServoOff3();
	afx_msg void OnServoOff4();
	afx_msg void OnServoOff5();
	afx_msg void OnServoOff6();
	afx_msg void OnServoOff7();
	afx_msg void OnServoOff8();
	afx_msg void OnServoOff9();
	afx_msg void OnServoOff10();
	afx_msg void OnServoOff11();
	afx_msg void OnServoOff12();
	afx_msg void OnServoOff13();
	afx_msg void OnServoOff14();
	afx_msg void OnServoOff15();
	afx_msg void OnAllServoOn();
	afx_msg void OnAllServoOff();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIMITSENSORDLG_H__DCBCEC59_ABF2_4A20_A3AD_7D2032851AFF__INCLUDED_)
