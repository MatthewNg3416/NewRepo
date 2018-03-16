//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATAFUNCONOFFDLG_H__F0D089DD_19E7_4B51_BCB3_D02A37150F61__INCLUDED_)
#define AFX_DATAFUNCONOFFDLG_H__F0D089DD_19E7_4B51_BCB3_D02A37150F61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataFuncOnOffDlg1.h : header file
//
class IDataType;

const int DEF_MAX_FUNC_NUMBER = 33;

/////////////////////////////////////////////////////////////////////////////
// CDataFuncOnOffDlg dialog

class MSystemData;
class MPreBonderData;

class CDataFuncOnOffDlg : public CDialog
{
// Construction
public:
	CDataFuncOnOffDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataFuncOnOffDlg)
	enum { IDD = IDD_DATA_FUNC_ONOFF };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataFuncOnOffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataFuncOnOffDlg)
	afx_msg void OnExit();
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickOnOffButton0();
	afx_msg void OnClickOnOffButton1();
	afx_msg void OnClickOnOffButton2();
	afx_msg void OnClickOnOffButton3();
	afx_msg void OnClickOnOffButton4();
	afx_msg void OnClickOnOffButton5();
	afx_msg void OnClickOnOffButton6();
	afx_msg void OnClickOnOffButton7();
	afx_msg void OnClickOnOffButton8();
	afx_msg void OnClickOnOffButton9();
	afx_msg void OnClickOnOffButton10();
	afx_msg void OnClickOnOffButton11();
	afx_msg void OnClickOnOffButton12();
	afx_msg void OnClickOnOffButton13();
	afx_msg void OnClickOnOffButton14();
	afx_msg void OnClickOnOffButton15();
	afx_msg void OnPaint();
	afx_msg void OnClickOnOffButton16();
	afx_msg void OnClickOnOffButton17();
	afx_msg void OnClickOnOffButton18();
	afx_msg void OnClickOnOffButton19();
	afx_msg void OnClickOnOffButton20();
	afx_msg void OnClickOnOffButton21();
	afx_msg void OnClickOnOffButton22();
	afx_msg void OnClickOnOffButton23();
	afx_msg void OnClickOnOffButton24();
	afx_msg void OnClickOnOffButton25();
	afx_msg void OnClickOnOffButton26();
	afx_msg void OnClickOnOffButton27();
	afx_msg void OnClickOnOffButton28();
	afx_msg void OnClickOnOffButton29();
	afx_msg void OnClickOnOffButton30();
	afx_msg void OnClickOnOffButton31();
	afx_msg void OnClickOnOffButton32(); //161109 JSH Cam Add...
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush			m_brBckColor;
	MSystemData*	m_plnkSystemData;
	MPreBonderData*	m_plnkPreBonderData;

	int				m_iFuncID;

	CBtnEnh			m_sFuncOnOff[DEF_MAX_FUNC_NUMBER];
	int				m_iFuncOnOff[DEF_MAX_FUNC_NUMBER];
	int				m_iFuncOnOffOld[DEF_MAX_FUNC_NUMBER];

	IDataType*		m_Item[DEF_MAX_FUNC_NUMBER];
	int				m_ItemMax;

	BOOL			m_bOldUseECDataReport;

	void			UpdateDisplay(int iFuncID);
	void			ChangeParameterValue(int iFuncID);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAFUNCONOFFDLG_H__F0D089DD_19E7_4B51_BCB3_D02A37150F61__INCLUDED_)
