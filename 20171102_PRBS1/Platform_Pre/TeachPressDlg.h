//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_TEACHPRESSDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_)
#define AFX_TEACHPRESSDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeederTeachDlg.h : header file
//
#include "MTabFeeder.h"
#include "MManageOpPanel.h"

class MSystemData;
class MRFID;

const int DEF_TEACH_PRESS_DLG_SUCCESS = 0;

/////////////////////////////////////////////////////////////////////////////
// CTeachPressDlg dialog
 
class CTeachPressDlg : public CDialog
{
private:
	int				m_iSelUnit;
	UINT			m_uiTimerID;
	int				m_iJogSelect;
	int				m_iSelectedPos;

	MRFID *m_pRFID;  //130503.kms___________

	/** 
	 * Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	 */
	double			m_dDisplayOffsetZ;

	// 전체 Position
	double			m_rgdFixedPosData[DEF_PRESS_MAX_POSITION];
	double			m_rgdOffsetPosData[DEF_PRESS_MAX_POSITION];

	CBtnEnh			m_lblGroup;
	CBtnEnh			m_BtnPos[DEF_PRESS_MAX_POSITION];
	CBtnEnh			m_sCoord[5];
	CBtnEnh			m_sStats[2];
//	CBtnEnh         m_sCamPosCal; //130424.kms
	CBtnEnh         m_sMoldRegCnt; //130424.kms

	//130503.kms________
	CBtnEnh         m_sCamPosData[31];
	CBtnEnh         m_sCamPosCal[31];
	CBtnEnh         m_sMoldRead;

//	double          m_dCamPosData;
	int             m_iCamPosData;
	double          m_dMoldRead;
	double          m_dMoldCalOffset;

	//_________________

	MTabFeeder*		m_plnkTabFeeder;
	STeachingInfo	m_sTeachingInfo;
	MManageOpPanel*	m_plnkC_MngOpPanel;

	CBrush m_brBckColor; 

	//130424.kms______
	MSystemData*    m_plnkSystemData;
	double m_dCalResult[31];  //130503.kms

	//	double m_dCalResult;
	double m_dMoldRegCnt;
	//________________

// Construction
public:
	CTeachPressDlg(int iSel, int iPressID, CWnd* pParent = NULL);   // standard constructor

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
	//{{AFX_DATA(CTeachPressDlg)
	enum { IDD = IDD_TEACH_PRESS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeachPressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTeachPressDlg)
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
	afx_msg void OnClickCamPosCal();
	afx_msg void OnClickMoldRegCnt();
	afx_msg void OnClickMoldRegRead();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEACHPRESSDLG_H__14341A06_977F_4DB8_9F7F_4F534F28273C__INCLUDED_)
