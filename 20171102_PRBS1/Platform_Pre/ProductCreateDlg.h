#if !defined(AFX_PRODUCTCREATEDLG_H__22417C13_EBA2_4352_8C4F_825F35184559__INCLUDED_)
#define AFX_PRODUCTCREATEDLG_H__22417C13_EBA2_4352_8C4F_825F35184559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProductCreateDlg.h : header file
//
#include "BtnEnh.h"
/////////////////////////////////////////////////////////////////////////////
// CProductCreateDlg dialog
class CProductPlanDlg;

class CProductCreateDlg : public CDialog
{
// Construction
public:
	CProductCreateDlg(int nProductID, CProductPlanDlg *pProductPlan,  int nSel, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProductCreateDlg)
	enum { IDD = IDD_PRODUCT_CREATE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CProductPlanDlg* m_plnkProductPlan;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProductCreateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	int m_nCurrentSel, m_nProductID;

	CBtnEnh		m_lblProductData[6];
	CBtnEnh		m_lblWorkOrder;
	CBtnEnh		m_lblDeviceID;
	CBtnEnh		m_lblInitProductPlanCnt;
	CBtnEnh		m_lblLastProductPlanCnt;
	CBtnEnh		m_lblBatchStateCnt;
	CBtnEnh		m_lblPPID;

	CString		m_strWorkOrder;
	CString		m_strDeviceID;
	CString		m_strInitProductPlanCnt;
	CString		m_strLastProductPlanCnt;
	CString		m_strBatchStateCnt;
	CString		m_strPPID;

	int m_nBatchStateCnt;

	void updateDisplay();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProductCreateDlg)
	afx_msg void OnClickSave();
	afx_msg void OnClickExit();
	afx_msg void OnClickWorkOrder();
	afx_msg void OnClickFirstProductOrder();
	afx_msg void OnClickFinalProductOrder();
	afx_msg void OnClickBatchState();
	afx_msg void OnClickPpid();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickDevideId();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODUCTCREATEDLG_H__22417C13_EBA2_4352_8C4F_825F35184559__INCLUDED_)
