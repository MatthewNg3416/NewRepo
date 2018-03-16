//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MATERIALINFODLG_H__2D2E3ADA_61BA_4B66_BDA6_CBECDF1EC4E8__INCLUDED_)
#define AFX_MATERIALINFODLG_H__2D2E3ADA_61BA_4B66_BDA6_CBECDF1EC4E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MaterialInfoDlg.h : header file
//

class MLCNet;
class MCtrlTabFeeder;
class MSystemData;

/////////////////////////////////////////////////////////////////////////////
// CAutoMaterialInfoDlg dialog

class CAutoMaterialInfoDlg : public CDialog
{
// Construction
public:
	CBrush m_brBckColor;
	CAutoMaterialInfoDlg(CWnd* pParent = NULL);   // standard constructor
	MLCNet*			m_plnkLCNet;
	MSystemData*	m_plnkSystemData;
	MCtrlTabFeeder*	m_plnkCtrlTabFeeder1;
	MCtrlTabFeeder*	m_plnkCtrlTabFeeder2;

// Dialog Data
	//{{AFX_DATA(CAutoMaterialInfoDlg)
	enum { IDD = IDD_AUTO_MATERIAL_INFO };
	CBtnEnh	m_bTabInsert1;
	CBtnEnh	m_bTabRemove1;
	CBtnEnh	m_bTabRemoveComplete1;
	CBtnEnh	m_bTabInsert2;
	CBtnEnh	m_bTabRemove2;
	CBtnEnh	m_bTabRemoveComplete2;
	CBtnEnh	m_bTabInsert3;
	CBtnEnh	m_bTabRemove3;
	CBtnEnh	m_bTabRemoveComplete3;
	CBtnEnh	m_bTabInsert4;
	CBtnEnh	m_bTabRemove4;
	CBtnEnh	m_bTabRemoveComplete4;
	CBtnEnh	m_bZigInsert1;
	CBtnEnh	m_bZigRemove1;
	CBtnEnh	m_bZigRemoveComplete1;
	CBtnEnh	m_bZigInsert2;
	CBtnEnh	m_bZigRemove2;
	CBtnEnh	m_bZigRemoveComplete2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoMaterialInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoMaterialInfoDlg)
	afx_msg void OnTabInsert1();
	afx_msg void OnTabRemove1();
	afx_msg void OnClickTabRemoveComplete1();
	afx_msg void OnTabInsert2();
	afx_msg void OnTabRemove2();
	afx_msg void OnClickTabRemoveComplete2();
	afx_msg void OnTabInsert3();
	afx_msg void OnTabRemove3();
	afx_msg void OnClickTabRemoveComplete3();
	afx_msg void OnTabInsert4();
	afx_msg void OnTabRemove4();
	afx_msg void OnClickTabRemoveComplete4();
	afx_msg void OnZigInsert1();
	afx_msg void OnZigRemove1();
	afx_msg void OnClickZigRemoveComplete1();
	afx_msg void OnZigInsert2();
	afx_msg void OnZigRemove2();
	afx_msg void OnClickZigRemoveComplete2();
	afx_msg void OnExit();
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATERIALINFODLG_H__2D2E3ADA_61BA_4B66_BDA6_CBECDF1EC4E8__INCLUDED_)
