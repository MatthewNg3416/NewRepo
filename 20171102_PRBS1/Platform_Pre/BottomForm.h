#if !defined(AFX_BOTTOMFORM_H__DE5D9729_144B_4F2F_BBE3_55E0A0B84AAE__INCLUDED_)
#define AFX_BOTTOMFORM_H__DE5D9729_144B_4F2F_BBE3_55E0A0B84AAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BottomForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBottomForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "btnenh.h"

const int DEF_MAX_BOTTOM_BUTTON	= 6;

class MTickTimer;
class MSystemData;
class MTrsAutoManager;

class CBottomForm : public CFormView
{
protected:
	CBottomForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBottomForm)

// Form Data
public:
	//{{AFX_DATA(CBottomForm)
	enum { IDD = IDD_BOTTOM_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	MSystemData* m_plnkSystemData;
	MTrsAutoManager*		m_pTrsAutoManager;

// Operations
private:
	BOOL checkPassword();
	int m_uiTimerID;

public:
	void OnAutoRun();
	void OnManual();	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBottomForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush		m_brBckColor;
	CBtnEnh		m_btnBottom[DEF_MAX_BOTTOM_BUTTON];
	CBtnEnh		m_lblDoorLock;
	CBtnEnh		m_lblDoorLock2;
	CBtnEnh		m_lblDoorLock3;
	CBtnEnh		m_lblDoorLock4;

	//syr..SESL 090613 ¹öÆ°¿¡ Flag ¸¸µê.
	BOOL		m_flgManualSelect;
	BOOL		m_flgDataSelect;
	BOOL		m_flgTeachSelect;	
	BOOL		m_flgAutoSelect;

	enum
	{
		DEF_AUTO_BTN = 0,
		DEF_MANUAL_BTN,
		DEF_DATA_BTN,
		DEF_TEACH_BTN,
		DEF_LOG_BTN,
		DEF_HELP_BTN,
	};

	virtual ~CBottomForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBottomForm)
	afx_msg void OnPaint();
	afx_msg void OnClickAuto();
	afx_msg void OnClickManual();
	afx_msg void OnClickData();
	afx_msg void OnClickTeach();
	afx_msg void OnClickLog();
	afx_msg void OnClickHelp();
	afx_msg void OnClickDoorLock();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickDoorLock2();
	afx_msg void OnClickDoorLock3();
	afx_msg void OnClickDoorLock4();
	afx_msg void OnClickVerDisp();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOTTOMFORM_H__DE5D9729_144B_4F2F_BBE3_55E0A0B84AAE__INCLUDED_)
