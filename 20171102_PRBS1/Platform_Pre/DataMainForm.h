//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATAMAINFORM_H__3E5B8AC3_DA91_4FBE_99C9_4C5854148D34__INCLUDED_)
#define AFX_DATAMAINFORM_H__3E5B8AC3_DA91_4FBE_99C9_4C5854148D34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataMainForm1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataMainForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDataMainForm : public CFormView
{
private:
	//101001.KKY_____
	BOOL			m_bInitialized;
	EOpMode			m_eOldOpMode;

	void showButtonManual();
	void showButtonAuto();
	//_______________


protected:
	CDataMainForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDataMainForm)

// Form Data
public:
	//{{AFX_DATA(CDataMainForm)
	enum { IDD = IDD_DATA_MAIN_FORM };
	CBtnEnh	m_lblModel;
	CBtnEnh	m_lblDelay;
	CBtnEnh	m_lblMotion;
	CBtnEnh	m_lblProcess;
	//}}AFX_DATA

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataMainForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush	m_brBckColor;

	virtual ~CDataMainForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDataMainForm)
	afx_msg void OnPaint();
	afx_msg void OnModelData();
	afx_msg void OnPanelData();
	afx_msg void OnRunVelocity();
	afx_msg void OnSwLimit();
	afx_msg void OnTolerance();
	afx_msg void OnOriginReturn();
	afx_msg void OnJogPitch();
	afx_msg void OnMotorDelay();
	afx_msg void OnCylinderDelay();
	afx_msg void OnVacuumDelay();
	afx_msg void OnTabOffset();
	afx_msg void OnClickFundOnoff();
	afx_msg void OnClickTabData1();
	afx_msg void OnTabMounterData();
	afx_msg void OnPanelAlignerData();
	afx_msg void OnClickDataBackup();
	afx_msg void OnCommonData();
	afx_msg void OnClickToolOffsetData();
	afx_msg void OnConveyorSpeedData();
	afx_msg void OnClickMaterialData();
	afx_msg void OnClickMaterialData2();
	afx_msg void OnModelChangeDistData();
	afx_msg LRESULT OnDisplayLanguage(UINT wParam, LPARAM lParam);
	afx_msg void OnClickDataDbEdit();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickHomeTeaching();
	afx_msg void OnClickTowerLampData();
	afx_msg void OnClickAcfPlateVacuumModeldata();
	afx_msg void OnClickErrorData();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMAINFORM_H__3E5B8AC3_DA91_4FBE_99C9_4C5854148D34__INCLUDED_)
