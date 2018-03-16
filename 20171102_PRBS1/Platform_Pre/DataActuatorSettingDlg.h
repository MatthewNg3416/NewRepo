//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MOTORSETTINGDLG_H__F17B9570_DA99_40D0_824F_91FE2E4AE764__INCLUDED_)
#define AFX_MOTORSETTINGDLG_H__F17B9570_DA99_40D0_824F_91FE2E4AE764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotorSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataActuatorSettingDlg dialog
#include "DefActuatorPreBonder.h"
#include "DefAxisPreBonder.h"
#include "MAxisData.h"
#include "MCylinderData.h"
#include "MVacuumData.h"

const int DEF_DLG_AXIS_BTN						= 16;

const int DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE	= 16;
const int DEF_DLG_MOTOR_SET_MAX_TITLE_NO		= 4;

//@const int DEF_DLG_MAX_PAGE_NO					= 5;
const int DEF_DLG_MAX_PAGE_NO					= 6;

const int DEF_DLG_VEL_ACC						= 0;
const int DEF_DLG_ORG_VEL_ACC					= 1;
const int DEF_DLG_LIMIT							= 2;
const int DEF_DLG_JOG_PITCH						= 3;
const int DEF_DLG_TOLERANCE						= 4;
const int DEF_DLG_MTR_DLY						= 5;
const int DEF_DLG_CYL_DLY						= 6;
const int DEF_DLG_VAC_DLY						= 7;

const int DEF_DLG_SETTING_MAX_BUFFER = DEF_DLG_MAX_PAGE_NO * DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE;

class ICylinder;
class IVacuum;
class IAxis;
class IMMCAxis;

class CDataActuatorSettingDlg : public CDialog
{
// Construction
public:
	CDataActuatorSettingDlg(int ButtonSel, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataActuatorSettingDlg)
	enum { IDD = IDD_DATA_ACTUATOR_SETTING };
//	CBtnEnh	m_lblVelMode;
	CBtnEnh	m_btnPrev;
	CBtnEnh	m_btnNext;
	CBtnEnh	m_btnAllIn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataActuatorSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void axisSelect(int iSel);
	void displayTable(int iUnitLimit, CString* pstrUnitName, int iDecimalColumn = -1);

private:
	// 100807. ECID ________
	CUIntArray	m_iaChangedItem;
	void		InitialDatabase();
	void		UpdateDatabase();
	void		InsertMotionData();
	void		InsertActuatorData();
	//______________________

protected:
	CBrush m_brBckColor;

	CBtnEnh m_btnTitle[DEF_DLG_MOTOR_SET_MAX_TITLE_NO];
	CBtnEnh m_btnAxis[DEF_DLG_AXIS_BTN];
	CBtnEnh m_lblValue[DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE][DEF_DLG_MOTOR_SET_MAX_TITLE_NO];
	
	ICylinder	*m_plnkCylinder[DEF_CYL_MAX_CYLINDER];
	IVacuum		*m_plnkVacuum[DEF_VAC_MAX_VACUUM];
	IAxis		*m_plnkMotion[DEF_AXIS_MAX_AXIS];

	MAxisData		m_rgsMotorParameter[DEF_AXIS_MAX_AXIS];
	MCylinderData	m_rgsCylParameter[DEF_CYL_MAX_CYLINDER];
	MVacuumData		m_rgsVacParameter[DEF_VAC_MAX_VACUUM];

	CString	m_strAxisName[DEF_AXIS_MAX_AXIS];
	CString	m_strCylinderName[DEF_CYL_MAX_CYLINDER];
	CString	m_strVacuumName[DEF_VAC_MAX_VACUUM];
	int		m_iCylinderIndex[DEF_CYL_MAX_CYLINDER];
	int		m_iVacuumIndex[DEF_VAC_MAX_VACUUM];	
	int		m_iCurrentPage;
	int		m_iSelectedPage;
	int		m_iPageNum;
	int		m_iColumnNum;
	int		m_iRowNum;
	int		m_iButtonSel;
	int		m_iSel;
	int		m_iOldSel;
	
	BOOL	m_bAllInputSelect;

	double	m_dTempBuf[DEF_DLG_SETTING_MAX_BUFFER][DEF_DLG_MOTOR_SET_MAX_TITLE_NO];
	bool	m_bAxisBtnStatus[DEF_DLG_MAX_PAGE_NO][DEF_DLG_AXIS_BTN];
	bool	m_bLblValueStatus[DEF_DLG_MAX_PAGE_NO][DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE][DEF_DLG_MOTOR_SET_MAX_TITLE_NO];
	
	void TitleSelect(int iSel);
	void UpdateDisplay();
	void InitialButton();

	// Generated message map functions
	//{{AFX_MSG(CDataActuatorSettingDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnAllInput();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnTitle0();
	afx_msg void OnTitle1();
	afx_msg void OnTitle2();
	afx_msg void OnTitle3();
	afx_msg void OnAxis0();
	afx_msg void OnAxis1();
	afx_msg void OnAxis2();
	afx_msg void OnAxis3();
	afx_msg void OnAxis4();
	afx_msg void OnAxis5();
	afx_msg void OnAxis6();
	afx_msg void OnAxis7();
	afx_msg void OnAxis8();
	afx_msg void OnAxis9();
	afx_msg void OnAxis10();
	afx_msg void OnAxis11();
	afx_msg void OnAxis12();
	afx_msg void OnAxis13();
	afx_msg void OnAxis14();
	afx_msg void OnAxis15();
	afx_msg void OnClickCreateEcdata();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTORSETTINGDLG_H__F17B9570_DA99_40D0_824F_91FE2E4AE764__INCLUDED_)
