#if !defined(AFX_DLGVIEWWORKSCHEDULE_H__0D920946_3D95_4B6E_B8C9_FB01EB0B6A0B__INCLUDED_)
#define AFX_DLGVIEWWORKSCHEDULE_H__0D920946_3D95_4B6E_B8C9_FB01EB0B6A0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewWorkSchedule.h : header file
//
#include "BtnEnh.h"
#include "MProcessData.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgViewWorkSchedule dialog
const int DEF_MAX_TAB_MANUAL = 4;

class CDlgViewWorkSchedule : public CDialog
{
private:
	int					m_iScheduleListNo;
	MWorkSchedule		*m_plnkWorkSchedule;
	MVarList<MTabInfo>	*m_pWorkList;
	MVarList<MInspectCarrierInfo> m_listMounter;
	MVarList<MInspectCarrierInfo> m_listInspect;
	MListWorkTab		m_listWorkTab;
	MCameraCarrier*		m_plnkCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTabMounter*		m_plnkTabMounter[DEF_MAX_TABMOUNTER];

	double				m_dInspectCamTgPos[DEF_MAX_INSPECTION_CAM_CARRIER];
	double				m_dTabMounterTgPos[DEF_MAX_TABMOUNTER];

	int m_iInspectSelecPos;

private:
	void				displayScheduleInfo();
	void				assignComponent();
// Construction
public:
	CDlgViewWorkSchedule(MWorkSchedule *pSchedule, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewWorkSchedule)
	enum { IDD = IDD_DLGVIEWWORKSCHEDULE_DIALOG };
		
	CBtnEnh m_ctrlLblScheduleNo;
	CBtnEnh	m_ctrlLblWorkGroup;
	CBtnEnh	m_ctrlLblWorkTabNo;
	CBtnEnh	m_ctrlBtnInspectPosSelect;
	//}}AFX_DATA
	CBtnEnh	m_ctrlLblTabNo[DEF_MAX_TAB_MANUAL];
	CBtnEnh	m_ctrlLblInspectNo[DEF_MAX_TAB_MANUAL];
	CBtnEnh	m_ctrlLblMounterNo[DEF_MAX_TAB_MANUAL];
	CBtnEnh	m_ctrlLblInspectPos[DEF_MAX_TAB_MANUAL];
	CBtnEnh	m_ctrlLblMounterPos[DEF_MAX_TAB_MANUAL];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewWorkSchedule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewWorkSchedule)
	afx_msg void OnClickSelectPrevios();
	afx_msg void OnClickSelectNext();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickExit();
	afx_msg void OnClickMoveInspectCarrier();
	afx_msg void OnClickInspectPosSelect();
	afx_msg void OnClickMoveTabMounter();
	afx_msg void OnClickMoveTabMountZCheck();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIEWWORKSCHEDULE_H__0D920946_3D95_4B6E_B8C9_FB01EB0B6A0B__INCLUDED_)
