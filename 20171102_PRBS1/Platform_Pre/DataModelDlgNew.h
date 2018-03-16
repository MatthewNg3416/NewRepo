//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATAMODELDLGNEW_H__EAAF04CA_73C4_4393_AFCB_ADEDE6F32B37__INCLUDED_)
#define AFX_DATAMODELDLGNEW_H__EAAF04CA_73C4_4393_AFCB_ADEDE6F32B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataModelDlgNew1.h : header file
//

#include "DefSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CDataModelDlgNew dialog
const int DEF_MAX_DEVICEID = 1000;
struct SDeviceIDModel
{
	CString strDeviceID;
	CString strModel;
};

class CDataModelDlgNew : public CDialog
{
// Construction
public:
	CDataModelDlgNew(CWnd* pParent = NULL);   // standard constructor
	int		ModelLoad(CString strModelName);

// Dialog Data
	//{{AFX_DATA(CDataModelDlgNew)
	enum { IDD = IDD_DATA_MODEL_NEW };
	CListBox	m_lboxModelList;
	CListBox	m_lboxModelList2;
	CBtnEnh	m_lblCurrentModel;
	CBtnEnh	m_lblSelectModel;
	CBtnEnh		m_btnReferenceOfSelectCreateModel;
	CBtnEnh		m_btnReferenceOfSelectDeleteModel;
	CListBox	m_lboxSelectModelReferenceList;
	CBtnEnh	m_btnModelModify;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataModelDlgNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush	m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CDataModelDlgNew)
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
	afx_msg void OnSelchangeModelList2();
	afx_msg void OnClickPaneldata();
	afx_msg void OnClickSelectDeviceModelCreate();
	afx_msg void OnSelchangeSelectReferenceModelList();
	afx_msg void OnClickSelectDeviceModelDelete();
	afx_msg void OnClickModelModify();
	afx_msg void OnClickSelectDeviceModelSelect();
	afx_msg void OnClickSearchData();
	afx_msg void OnClickTabdata();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	void OnDisplayOperater(); 
	void OnDisplayEngMaker(); 

	DECLARE_MESSAGE_MAP()

private:
	WCHAR	m_szTmp[100];
	int		m_iModelCount;
	int		m_iCurrentSel;
	CString	m_strCurrentModelName;
	CString	m_strSelectedModelName;
	CString	m_strModelList[DEF_MAX_MODEL];

	//110114.kms________
	CString m_strDeviceIdList[DEF_MAX_MODEL];

	SDeviceIDModel m_sDeviceIDModel[DEF_MAX_DEVICEID];
	int		m_iDeviceIDCount;

	void updateModelListBox();
	int InitializeModelListBox();


	//110304.kms__________________________
	//Reference Model 추가 관련
	int		m_iCurrentSelForDeviceModelList;
	void	updateSelectModelReferenceListBox();
	//____________________________________

	int		m_iCurrentSel2;
	void	updateModelListBox2();
	CString m_strSelectedModelName2;

public:
	//110304.kms__________________________
	//Reference Model 추가 관련
	CString m_rgdReferenceModel[DEF_MAX_REFERENCE_MODEL];
	//____________________________________

	BOOL m_bReferanceSelect;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAMODELDLG_H__EAAF04CA_73C4_4393_AFCB_ADEDE6F32B37__INCLUDED_)
