//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHMOLDPINDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_)
#define AFX_TEACHMOLDPINDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeederTeachDlg.h : header file
//
#include "MTabFeeder.h"
#include "MManageOpPanel.h"

const int DEF_TEACH_MOLDPIN_DLG_SUCCESS = 0;

/////////////////////////////////////////////////////////////////////////////
// CTeachMoldPinDlg dialog
 
class CTeachMoldPinDlg : public CDialog
{
private:
	int				m_iSelUnit;
	UINT			m_uiTimerID;
	int				m_iJogSelect;
	int				m_iSelectedPos;

	/** 
	 * Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	 */
	double			m_dDisplayOffsetZ;

	// 전체 Position
	double			m_rgdFixedPosData[DEF_MOLDPIN_MAX_POSITION];
	double			m_rgdOffsetPosData[DEF_MOLDPIN_MAX_POSITION];

	CBtnEnh			m_lblGroup;
	CBtnEnh			m_BtnPos[DEF_MOLDPIN_MAX_POSITION];
	CBtnEnh			m_sCoord[5];
	CBtnEnh			m_sStats[2];

	MTabFeeder*		m_plnkTabFeeder;
	STeachingInfo	m_sTeachingInfo;
	MManageOpPanel*	m_plnkC_MngOpPanel;

	CBrush m_brBckColor; 

// Construction
public:
	CTeachMoldPinDlg(int iSel, int iPressID, CWnd* pParent = NULL);   // standard constructor

	/**
	 * 목표위치, Mark Offset, 기타보정, 현재위치, 편차 Display (XYT축)
	 */

	void UpdateDisplay();
	/**
	 * 고정좌표, Offset좌표 Display (XYT축)
	 */

	void UpdateCoordinate();
	/**
	 * 선택 Position 변경 (XYT축)
	 */

	void SelectPosition(int iPosIndex);
	/**
	 * Command Button 상태 Update
	 */

	void UpdateButton();

// Dialog Data
	//{{AFX_DATA(CTeachMoldPinDlg)
	enum { IDD = IDD_TEACH_MOLDPIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachMoldPinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachMoldPinDlg)
	afx_msg void OnJog();
	afx_msg void OnX();
	afx_msg void OnFeederPos0();
	afx_msg void OnFeederPos1();
	afx_msg void OnBmMove();
	afx_msg void OnPosSet();
	afx_msg void OnSave();
	afx_msg void OnExit();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEACHMOLDPINDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_)
