//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATAMODELDLG_H__EAAF04CA_73C4_4393_AFCB_ADEDE6F32B37__INCLUDED_)
#define AFX_DATAMODELDLG_H__EAAF04CA_73C4_4393_AFCB_ADEDE6F32B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelDataDlg.h : header file
//

#include "DefSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CDataModelDlg dialog

class CDataModelDlg : public CDialog
{
// Construction
public:
	CDataModelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataModelDlg)
	enum { IDD = IDD_DATA_MODEL };
	CListBox	m_lboxModelList;
	CBtnEnh	m_lblCurrentModel;
	CBtnEnh	m_lblSelectModel;
	//110304.kms__________________________
	//Reference Model 추가 관련
	CBtnEnh	m_btnReferenceOfSelectCreateModel;
	CBtnEnh	m_btnReferenceOfSelectDeleteModel;
	CBtnEnh	m_btnModelModify;
	CListBox	m_lboxSelectModelReferenceList;
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataModelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush	m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CDataModelDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnModelCreate();
	afx_msg void OnModelDelete();
	afx_msg void OnModelLoad();
	afx_msg void OnUp();
	afx_msg void OnDown();
	afx_msg void OnPageUp();
	afx_msg void OnPageDown();
	afx_msg void OnSelchangeModelList();
	afx_msg void OnClickPaneldata();
	afx_msg void OnClickTabdata();
	afx_msg void OnSelchangeSelectReferenceModelList();
	afx_msg void OnClickSelectDeviceModelCreate();
	afx_msg void OnClickSelectDeviceModelDelete();
	afx_msg void OnModelModify();
	afx_msg void OnClickSelectDeviceModelSelect();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	WCHAR	m_szTmp[100];
	int		m_iModelCount;
	int		m_iCurrentSel;
	CString	m_strCurrentModelName;
	CString	m_strSelectedModelName;
	CString	m_strModelList[DEF_MAX_MODEL];

	void	updateModelListBox();
	int		InitializeModelListBox();

	//110114.kms________
	CString m_strDeviceIdList[DEF_MAX_MODEL];

	//110304.kms__________________________
	//Reference Model 추가 관련
	int		m_iCurrentSelForDeviceModelList;
	void	updateSelectModelReferenceListBox();
	//____________________________________

	BOOL m_bReferanceSelect;

public:
	//110304.kms__________________________
	//Reference Model 추가 관련
	CString m_rgdReferenceModel[DEF_MAX_REFERENCE_MODEL];
	//____________________________________
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELDATADLG_H__EAAF04CA_73C4_4393_AFCB_ADEDE6F32B37__INCLUDED_)
