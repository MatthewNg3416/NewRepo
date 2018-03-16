//{{AFX_INCLUDES()
#include "btnenh.h"
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_MOLDZIGINFO_H__839E6BA6_19DA_4E60_8DC9_573430C165B1__INCLUDED_)
#define AFX_MOLDZIGINFO_H__839E6BA6_19DA_4E60_8DC9_573430C165B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoldZigInfo.h : header file
//
#include "MPreBonderData.h"
#include "MLCNetData.h"

/////////////////////////////////////////////////////////////////////////////
// CMoldZigInfo dialog

class IComThread;
class MCtrlTabFeeder;
class MLCNet;
class MSystemData;
class MRFID;

class CMoldZigInfo : public CDialog
{
private:
	IComThread*		m_plnkSerialBarCode;
	MCtrlTabFeeder*	m_plnkCtrlTabFeeder;
	MLCNet*			m_plnkLCNet;
	MSystemData*	m_plnkSystemData;
	MRFID* m_pRFID;

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
	CMoldZigInfo(int iFeederSelect, EMatEvent eMode, CWnd* pParent = NULL);   // standard constructor
	CBrush m_brBckColor;
	EMatEvent m_eMode;
	int m_iFeederSelect;
	int m_iReelSelect;
	UINT m_uiTimerID;
	void enableBtn();
	void setFocusBtn(int iIndex);
	CString RemoveSpecialChar(CString sRcvStr);
	void PeekAndPump();

// Dialog Data
	//{{AFX_DATA(CMoldZigInfo)
	enum { IDD = IDD_MOLD_ZIG_INFO };
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
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoldZigInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMoldZigInfo)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickDockIn();
	afx_msg void OnClickCancelDockIn();
	afx_msg void OnClickCheckDockIn();
	afx_msg void OnClickExit();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOLDZIGINFO_H__839E6BA6_19DA_4E60_8DC9_573430C165B1__INCLUDED_)
