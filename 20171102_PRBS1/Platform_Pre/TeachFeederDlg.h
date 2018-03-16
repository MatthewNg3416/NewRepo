#if !defined(AFX_FEEDERTEACHDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_)
#define AFX_FEEDERTEACHDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeederTeachDlg.h : header file
//
#include "MTabFeeder.h"
#include "MCtrlTabFeeder.h"
#include "MSystemData.h"
#include "MPanelData.h"
#include "MPreBonderData.h"
#include "MManageOpPanel.h"

/////////////////////////////////////////////////////////////////////////////
// CTeachFeederDlg dialog
 
class CTeachFeederDlg : public CDialog
{
private:
	int m_iSelUnit;
	UINT m_uiTimerID;
	int m_iJogSelect;
	int	m_iSelectedPos;

	/** 
	 * Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	 */
	double			m_dDisplayOffsetZ;

	// 전체 Position
	double			m_rgdFixedPosData[DEF_PRESS_MAX_POSITION];
	double			m_rgdOffsetPosData[DEF_PRESS_MAX_POSITION];

	CBtnEnh			m_BtnPos[DEF_PRESS_MAX_POSITION];
	CBtnEnh			m_sCoord[5];
	CBtnEnh			m_sStats[2];

	IIO*			m_plnkIO;
	MTabFeeder*		m_plnkTabFeeder;
	MCtrlTabFeeder*	m_plnkCtrlTabFeeder;
	STeachingInfo	m_sTeachingInfo;
	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData*	m_plnkPreBonderData;

	MManageOpPanel*	m_plnkC_MngOpPanel;

// Construction
public:
	CTeachFeederDlg(int iSel, int iFeederID,CWnd* pParent = NULL);   // standard constructor
	CBrush m_brBckColor; 

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
	//{{AFX_DATA(CTeachFeederDlg)
	enum { IDD = IDD_TEACH_FEEDER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachFeederDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachFeederDlg)
	afx_msg void OnJog();
	afx_msg void OnX();
	afx_msg void OnFeederPos0();
	afx_msg void OnFeederPos1();
	afx_msg void OnFeederPos2();
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

#endif // !defined(AFX_FEEDERTEACHDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_)
