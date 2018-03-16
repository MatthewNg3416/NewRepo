//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_BADTABTEACHDLG_H__4567EF30_3997_4458_B0D2_130C01A84124__INCLUDED_)
#define AFX_BADTABTEACHDLG_H__4567EF30_3997_4458_B0D2_130C01A84124__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BadTabTeachDlg.h : header file
//
#include "DefTabFeeder.h"

class MTabFeeder;
class MCtrlTabFeeder;
class MSystemData;
class MManageOpPanel;
class MPreBonderData;

/////////////////////////////////////////////////////////////////////////////
// CTeachBadTabDetectorDlg dialog

class CTeachBadTabDetectorDlg : public CDialog
{
// Construction
private:
	int		m_iSelUnit;
	UINT	m_uiTimerID;
	int		m_iGroupNo;
	int		m_iSelectedPos;

	/** 
	 * Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	 */
	double			m_dDisplayOffsetZ;

	// 전체 Position
	double			m_rgdModelPosData[DEF_BADTABDETECTOR_MAX_POSITION];
	double			m_rgdOffsetPosData[DEF_BADTABDETECTOR_MAX_POSITION];

	CBtnEnh			m_lblGroup;
	CBtnEnh			m_BtnPos[DEF_BADTABDETECTOR_MAX_POSITION];
	CBtnEnh			m_sCoord[5];

	CBtnEnh			m_BtnAxis[DEF_BADTABDETECTOR_MAX_AXIS];

	CBtnEnh			m_sTensionOffset;

	CBtnEnh	m_btnReelDown;
	CBtnEnh	m_btnReelUp;
	CBtnEnh	m_btnReelClampDown;
	CBtnEnh	m_btnReelClampUp;

	STeachingInfo	m_sTeachingInfo;

	MTabFeeder*		m_plnkTabFeeder;
	MCtrlTabFeeder*	m_plnkCtrlTabFeeder;
	MSystemData*	m_plnkSystemData;
	MPreBonderData* m_plnkPreBonderData;
	MManageOpPanel*	m_plnkC_MngOpPanel;

	int		m_iSp1JogSel;
	int		m_iSp2JogSel;

public:
	CTeachBadTabDetectorDlg(int iFixModel, int iGroupNo, CWnd* pParent = NULL);   // standard constructor
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
	//{{AFX_DATA(CTeachBadTabDetectorDlg)
	enum { IDD = IDD_TEACH_BADTABDETECTOR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachBadTabDetectorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachBadTabDetectorDlg)
	afx_msg void OnJog();
	afx_msg void OnZ1();
	afx_msg void OnBadtabPos0();
	afx_msg void OnBadtabPos1();
	afx_msg void OnCmMove();
	afx_msg void OnPosSet();
	afx_msg void OnSave();
	afx_msg void OnExit();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickCmSprocket1Homing();
	afx_msg void OnClickCmSprocket2Homing();
	afx_msg void OnCheckCuttingLength();
	afx_msg void OnClickTabCuttingOffset();
	afx_msg void OnClickReelUp();
	afx_msg void OnClickReelDown();
	afx_msg void OnClickReelClampUp();
	afx_msg void OnClickReelClampDown();
	afx_msg void OnClickCmSprocket1Jog();
	afx_msg void OnClickCmSprocket2Jog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	double getUserOffsetX_withDisplay();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BADTABTEACHDLG_H__4567EF30_3997_4458_B0D2_130C01A84124__INCLUDED_)
