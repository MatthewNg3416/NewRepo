#if !defined(AFX_PAGEMATERIALINFO_H__C2D57CCD_EBBD_4E4C_911D_CD7DC0988595__INCLUDED_)
#define AFX_PAGEMATERIALINFO_H__C2D57CCD_EBBD_4E4C_911D_CD7DC0988595__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageMaterialInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageMaterialInfo dialog
const int DEF_MAX_MATERIAL_ITEM = 10;

#include "BtnEnh.h"
#include "DefSystem.h"

class MLCNet;
class MTrsAutoManager;
class MCtrlTabFeeder;

class CPageMaterialInfo : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageMaterialInfo)

private:
	HWND m_hAutoMainWnd;

	MLCNet* m_plnkLCNet;
	MTrsAutoManager* m_plnkAutoManager;
	MCtrlTabFeeder*	m_plnkCtrlFeeder[DEF_MAX_TABFEEDER];

public:
	void SetAutoMainWnd(HWND hMainView);
	void RenewMaterialCode();
	void ClearDisplay();
	void UpdateDisplay();
	
// Construction
public:
	CPageMaterialInfo();
	~CPageMaterialInfo();

// Dialog Data
	//{{AFX_DATA(CPageMaterialInfo)
	enum { IDD = IDD_PAGE_MATERIAL_INFO };
	//}}AFX_DATA

	CBtnEnh		m_lblMID[DEF_MAX_MATERIAL_ITEM];
	CBtnEnh		m_lblMCode[DEF_MAX_MATERIAL_ITEM];
	CBtnEnh		m_lblTQty[DEF_MAX_MATERIAL_ITEM];
	CBtnEnh		m_lblUQty[DEF_MAX_MATERIAL_ITEM];
	CBtnEnh		m_lblRQty[DEF_MAX_MATERIAL_ITEM];
	CBtnEnh		m_lblNQty[DEF_MAX_MATERIAL_ITEM];
	CBtnEnh		m_lblAQty[DEF_MAX_MATERIAL_ITEM];
	CBtnEnh		m_lblMStep[DEF_MAX_MATERIAL_ITEM];
	CBtnEnh		m_lblMState[DEF_MAX_MATERIAL_ITEM];


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageMaterialInfo)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageMaterialInfo)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEMATERIALINFO_H__C2D57CCD_EBBD_4E4C_911D_CD7DC0988595__INCLUDED_)
