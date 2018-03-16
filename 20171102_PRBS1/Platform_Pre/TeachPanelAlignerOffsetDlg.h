//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHSTAGEOFFSETDLG_H__0E37EB15_591F_4432_93E3_DBFD16106A16__INCLUDED_)
#define AFX_TEACHSTAGEOFFSETDLG_H__0E37EB15_591F_4432_93E3_DBFD16106A16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeachPanelAlignerOffsetDlg.h : header file
//

#include "MPanelAligner.h"

/////////////////////////////////////////////////////////////////////////////
// CTeachPanelAlignerOffsetDlg dialog

const int DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE	= 12;
const int DEF_DLG_STAGE_OFFSET_AXIS_TITLE			= 3;

const int ERR_STAGE_OFFSET_DLG_SUCCESS				= 0;

class CTeachPanelAlignerOffsetDlg : public CDialog
{
// Construction
public:
	CTeachPanelAlignerOffsetDlg(int iUnitID, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTeachPanelAlignerOffsetDlg)
	enum { IDD = IDD_TEACH_PANEL_ALIGNER_OFFSET };
	CBtnEnh	m_btnAllInput;
	CBtnEnh	m_btnNext;
	CBtnEnh	m_btnPrev;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachPanelAlignerOffsetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	// Generated message map functions
	//{{AFX_MSG(CTeachPanelAlignerOffsetDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSave();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnAllInput();
	afx_msg void OnX();
	afx_msg void OnY();
	afx_msg void OnT();
	afx_msg void OnPos0();
	afx_msg void OnPos1();
	afx_msg void OnPos2();
	afx_msg void OnPos3();
	afx_msg void OnPos4();
	afx_msg void OnPos5();
	afx_msg void OnPos6();
	afx_msg void OnPos7();
	afx_msg void OnPos8();
	afx_msg void OnPos9();
	afx_msg void OnPos10();
	afx_msg void OnPos11();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int		m_iSelectedUnitID;
	int		m_iSelPos;
	int		m_iRealPos;
	BOOL	m_bAllInput;
	int		m_iCurrentPage;

	MPos_XYT	m_rgXYTOffsetPosData[DEF_PANEL_ALIGNER_MAX_POSITION];

	CBtnEnh m_sCoord[DEF_DLG_STAGE_OFFSET_AXIS_TITLE][DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE];
	CBtnEnh m_BtnPos[DEF_DLG_STAGE_OFFSET_POS_MAX_BTN_PER_PAGE];

	MPanelAligner*	m_plnkPanelAligner;

	/**
	 * 선택 Position 변경 (XYT축)
	 */
	void SelectPosition(int iPosIndex);

	/**
	 * 고정좌표, Offset좌표 Display (XYT축)
	 */
	void UpdateCoordinate();

	/**
	 * Position Button 상태 Update
	 */
	void UpdateButton();

	void SetJobID(int iJobID);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEACHSTAGEOFFSETDLG_H__0E37EB15_591F_4432_93E3_DBFD16106A16__INCLUDED_)
