//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TABICINFORMDLG_H__4C98500B_AEDA_409A_9F35_B6E05A3CFFCC__INCLUDED_)
#define AFX_TABICINFORMDLG_H__4C98500B_AEDA_409A_9F35_B6E05A3CFFCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabICInformDlg.h : header file
//
#include "MPreBonderData.h"
#include "MLCNetData.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoTabICInfoDlg dialog

class IComThread;
class MCtrlTabFeeder;
class MLCNet;
class MSystemData;

//20111020 by kss
/////////////////////////////////////////////////////////
class MCtrlPanelAligner;
/////////////////////////////////////////////////////////

class CAutoTabICInfoDlg : public CDialog
{
private:
	IComThread*		m_plnkSerialBarCode;
	MCtrlTabFeeder*	m_plnkCtrlTabFeeder;
	MLCNet*			m_plnkLCNet;
	MSystemData*	m_plnkSystemData;
//20111020 by kss
/////////////////////////////////////////////////////////
	MCtrlPanelAligner*	m_plnkCtrlPanelAlinger;
/////////////////////////////////////////////////////////

	CString			m_strStart;
	CString			m_strFeeder;
	CString			m_strTabICType;
	CString			m_strMaker;
	CString			m_strTabICID;
	CString			m_strEnd;
	CString			m_strValidationCheck;

	BOOL			m_bCancelInput;
	CString			m_strTemp;


// Construction
public:
	CBrush m_brBckColor;
	EMatEvent m_eMode;
	int m_iFeederSelect;
	int m_iReelSelect;
	UINT m_uiTimerID;
	CAutoTabICInfoDlg(int iFeederSelect, int iReelSelect, EMatEvent eMode, CWnd* pParent = NULL);   // standard constructor
	void enableBtn();
	void setFocusBtn(int iIndex);
	CString RemoveSpecialChar(CString sRcvStr);
	void PeekAndPump();


// Dialog Data
	//{{AFX_DATA(CAutoTabICInfoDlg)
	enum { IDD = IDD_AUTO_TABIC_INFO };
	CBtnEnh	m_bExit;
	//CMSFlexGrid	m_FlexGrid;
	CBtnEnh	m_bStartEnd;
	CBtnEnh	m_bFeederNum;
	CBtnEnh	m_bTabICType;
	CBtnEnh	m_bMaker;
	CBtnEnh	m_bTabIcID;
	CBtnEnh	m_bDockIn;
	CBtnEnh	m_bValidationCheck;
	CBtnEnh	m_sValidationCheck;
	CBtnEnh	m_sStartEnd;
	CBtnEnh	m_sFeederNum;
	CBtnEnh	m_sTabICType;
	CBtnEnh	m_sMaker;
	CBtnEnh	m_sTabIcID;
	CBtnEnh	m_bCancelDockIn;
	CBtnEnh	m_bCheckDockIn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoTabICInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoTabICInfoDlg)
	afx_msg void OnDockIn();
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickCancelDockIn();
	afx_msg void OnClickCheckDockIn();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABICINFORMDLG_H__4C98500B_AEDA_409A_9F35_B6E05A3CFFCC__INCLUDED_)
