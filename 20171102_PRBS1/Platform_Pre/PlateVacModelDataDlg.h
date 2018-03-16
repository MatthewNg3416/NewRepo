//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_PLATEVACMODELDATADLG_H__F78CD50A_8C6A_46FA_95FE_0D6A3E552193__INCLUDED_)
#define AFX_PLATEVACMODELDATADLG_H__F78CD50A_8C6A_46FA_95FE_0D6A3E552193__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlateVacModelDataDlg.h : header file
//

#include "DefSystem.h"

const int DEF_MAX_MANUAL_WORKCNT	= 2;
/////////////////////////////////////////////////////////////////////////////
// CPlateVacModelDataDlg dialog
class MPanelData;

class CPlateVacModelDataDlg : public CDialog
{
// Construction
public:
	CPlateVacModelDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlateVacModelDataDlg)
	enum { IDD = IDD_PLATE_VACUUM_MODEL_DATA };
	CBtnEnh	m_btnSave;
	CBtnEnh	m_btnExit;
	//}}AFX_DATA

	CBtnEnh	m_btnPlateVacBtn[DEF_MAX_MANUAL_WORKCNT][DEF_MAX_AIRVACUUM];
	CBtnEnh	m_lblPlateVac[DEF_MAX_MANUAL_WORKCNT][DEF_MAX_AIRVACUUM];

	CBtnEnh	m_btnTHandlerVac[DEF_MAX_THANDLER_VAC];
	CBtnEnh	m_lblTHandlerVac[DEF_MAX_THANDLER_VAC];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlateVacModelDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CPlateVacModelDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnClickPlateVac0();
	afx_msg void OnClickPlateVac1();
	afx_msg void OnClickPlateVac2();
	afx_msg void OnClickPlateVac3();
	afx_msg void OnClickPlateVac4();
	afx_msg void OnClickPlateVac5();
	afx_msg void OnClickPlateVac6();
	afx_msg void OnClickPlateVac7();
	afx_msg void OnClickPlateVac8();
	afx_msg void OnClickPlateVac9();
	afx_msg void OnClickPlateVac10();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	MPanelData*	m_plnkPanelData;

	int				m_iWCnt;
	int				m_iAirVac;

	void UpdateDisplay(int iWCnt, int iAirVac);
	void ChangeParameterValue(int iWCnt, int iAirVac);

	void changeParamTHandlerVac(int iVacID);
	void updateDisplayTHandlerVac(int iVacID);
	
	int m_bACFOnOff[DEF_MAX_MANUAL_WORKCNT][DEF_MAX_AIRVACUUM];

	BOOL	m_bUseTHandlerVac[DEF_MAX_THANDLER_VAC];

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLATEVACMODELDATADLG_H__F78CD50A_8C6A_46FA_95FE_0D6A3E552193__INCLUDED_)
