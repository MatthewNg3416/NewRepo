//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_DATAINDEXOFFSET_H__1F616A33_0B4C_4A86_ABC1_CA395C6133C7__INCLUDED_)
#define AFX_DATAINDEXOFFSET_H__1F616A33_0B4C_4A86_ABC1_CA395C6133C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataIndexOffset.h : header file
//

const int DEF_MAX_DISPLAY_ITEM	= 14;
const int DEF_TYPE_TABMOUNTER	= 0;
const int DEF_TYPE_TABALIGNER	= 1;

/////////////////////////////////////////////////////////////////////////////
// CDataToolOffsetDlg dialog

class MSystemData;

class CDataToolOffsetDlg : public CDialog
{
private:
	CBtnEnh			m_btnOffsetMounter[DEF_MAX_DISPLAY_ITEM];
	CBtnEnh			m_lblOffsetMounter[DEF_MAX_DISPLAY_ITEM];

	CBtnEnh			m_btnOffsetTabAligner[DEF_MAX_DISPLAY_ITEM];
	CBtnEnh			m_lblOffsetTabAligner[DEF_MAX_DISPLAY_ITEM];

	MSystemData*	m_plnkSystemData;
	double			m_dOffsetMounter[DEF_MAX_DISPLAY_ITEM];
	double			m_dOffsetTabAligner[DEF_MAX_DISPLAY_ITEM];

	void			UpdateDisplay();
	void			ChangeVal(int iType, int iSel);
	void			getBatchInput(int iAxis);

// Construction
public:
	CBrush m_brBckColor;
	CDataToolOffsetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataToolOffsetDlg)
	enum { IDD = IDD_DATA_TOOL_OFFSET };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataToolOffsetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataToolOffsetDlg)
	afx_msg void OnSave();
	afx_msg void OnBatchxInput();
	afx_msg void OnBatchyInput();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickUse1x();
	afx_msg void OnClickUse2x();
	afx_msg void OnClickUse3x();
	afx_msg void OnClickUse4x();
	afx_msg void OnClickUse5x();
	afx_msg void OnClickUse6x();
	afx_msg void OnClickUse7x();
	afx_msg void OnClickUse8x();
	afx_msg void OnClickUse9x();
	afx_msg void OnClickUse10x();
	afx_msg void OnClickUse11x();
	afx_msg void OnClickUse12x();
	afx_msg void OnClickUse13x();
	afx_msg void OnClickUse14x();
	afx_msg void OnClickUse1y();
	afx_msg void OnClickUse2y();
	afx_msg void OnClickUse3y();
	afx_msg void OnClickUse4y();
	afx_msg void OnClickUse5y();
	afx_msg void OnClickUse6y();
	afx_msg void OnClickUse7y();
	afx_msg void OnClickUse8y();
	afx_msg void OnClickUse9y();
	afx_msg void OnClickUse10y();
	afx_msg void OnClickUse11y();
	afx_msg void OnClickUse12y();
	afx_msg void OnClickUse13y();
	afx_msg void OnClickUse14y();
	afx_msg void OnPaint();
	afx_msg void OnClickCancel();
	afx_msg void OnClickTaboffsetTable();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAINDEXOFFSET_H__1F616A33_0B4C_4A86_ABC1_CA395C6133C7__INCLUDED_)
