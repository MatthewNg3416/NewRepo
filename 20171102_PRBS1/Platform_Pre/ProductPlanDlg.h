#if !defined(AFX_PRODUCTPLANDLG_H__F4A3E0B8_49A5_4D31_8A7B_CD1BC5473F61__INCLUDED_)
#define AFX_PRODUCTPLANDLG_H__F4A3E0B8_49A5_4D31_8A7B_CD1BC5473F61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProductPlanDlg.h : header file
//
#include "BtnEnh.h"
#include "DefSystem.h"
/////////////////////////////////////////////////////////////////////////////
// CProductPlanDlg dialog

class MLCNet;
class MProductInfoData;
class MSystemData;

class CProductPlanDlg : public CDialog
{
// Construction
public:
	CProductPlanDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProductPlanDlg)
	enum { IDD = IDD_PRODUCT_PLAN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void OnDisplayProductPlan(UINT iPlanCode);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProductPlanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support;
	//}}AFX_VIRTUAL
public:
	CString	m_strWorkOrderNext;
	CString	m_strDeviceIDNext;
	CString	m_strInitProductPlanCntNext;
	CString	m_strLastProductPlanCntNext;
	CString	m_strBatchStateCntNext;
	CString	m_strPPIDNext;

	int		m_nBatchStateCntNext;

private:
	//110629_________
	UINT m_uiTimerID;
	MLCNet*		m_plnkLCNet;
	void clearDisplay();
	void updateDisplay();
	void OnDisplayLanguage();

	CBtnEnh		m_lblNext[5];

	CBtnEnh		m_lblWorkOrderCur;
	CBtnEnh		m_lblDeviceIDCur;
	CBtnEnh		m_lblInitProductPlanCntCur;
	CBtnEnh		m_lblLastProductPlanCntCur;
	CBtnEnh		m_lblBatchStateCntCur;
	CBtnEnh		m_lblPPIDCur;

	CBtnEnh		m_lblWorkOrderNext[DEF_MAX_PRODUCT_PLAN_CNT];
	CBtnEnh		m_lblDeviceIDNext[DEF_MAX_PRODUCT_PLAN_CNT];
	CBtnEnh		m_lblInitProductPlanCntNext[DEF_MAX_PRODUCT_PLAN_CNT];
	CBtnEnh		m_lblPPIDNext[DEF_MAX_PRODUCT_PLAN_CNT];
	CBtnEnh		m_lblLastProductPlanCntNext[DEF_MAX_PRODUCT_PLAN_CNT];
	CBtnEnh		m_lblBatchStateCntNext[DEF_MAX_PRODUCT_PLAN_CNT];

	CBtnEnh		m_lblWorkOrderEnd[DEF_MAX_PRODUCT_PLAN_CNT];
	CBtnEnh		m_lblDeviceIDEnd[DEF_MAX_PRODUCT_PLAN_CNT];
	CBtnEnh		m_lblLastProductPlanCntEnd[DEF_MAX_PRODUCT_PLAN_CNT];
	CBtnEnh		m_lblPPIDEnd[DEF_MAX_PRODUCT_PLAN_CNT];
	//_______________


	//110614.kms____________________
	//생산 계획정보 관련 추가 항목 
	MProductInfoData* m_lnkProductInfoData;
	MSystemData*      m_plnkSystemData;

	void UpdateSelectPlan(int sel);
	void UpdateSelectButton(int sel);
	void UpdateCreatePlan(int sel);
	BOOL	m_bSelNextPlan[5];
	long m_lNextButtonRGB;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProductPlanDlg)
	afx_msg void OnClickExit();
	afx_msg void OnClickJobPlanRequest();
	afx_msg void OnClickJobPlanRegist();
	afx_msg void OnClickJobPlanRemove();
	afx_msg void OnClickJobPlanModify();
	afx_msg void OnClickValidationCheckItem();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickNext1();
	afx_msg void OnClickNext2();
	afx_msg void OnClickNext3();
	afx_msg void OnClickNext4();
	afx_msg void OnClickNext5();
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODUCTPLANDLG_H__F4A3E0B8_49A5_4D31_8A7B_CD1BC5473F61__INCLUDED_)
