//{{AFX_INCLUDES()
#include "btnenh.h"
//}}AFX_INCLUDES
#if !defined(AFX_MOTORORIGINDLG_H__FE0BE927_C365_47E6_9442_29DCDC34275A__INCLUDED_)
#define AFX_MOTORORIGINDLG_H__FE0BE927_C365_47E6_9442_29DCDC34275A__INCLUDED_

#include "btnenh.h"
#include "DefAxisPreBonder.h"
#include "MTickTimer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MotorOriginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoMotorOriginDlg dialog

typedef struct
{
	void* pThisDlg;
	int* prgiOriginMoveType;

	int iOriginGroupNo;
	BOOL bGetPreMoveDist;

	int iStartAxisNo;
	int iEndAxisNo;
//	int iOriginAxisNo[DEF_AXIS_MAX_AXIS];

} SOriginGroupData;

// Doolin kwangilshin 2017. 08. 27.
//
#define	DEF_MOVE_CAP	80
// End.

const int ERR_DLG_ORIGIN_SUCCESS				= 0;
const int ERR_DLG_ORIGIN_FAIL					= -1;
const int ERR_DLG_ORIGIN_DETECT_E_STOP			= 2;
const int ERR_DLG_ORIGIN_DETECT_USER_STOP		= 4;
const int ERR_DLG_ORIGIN_DETECT_AMP_FAULT		= 8;
const int ERR_DLG_ORIGIN_DETECT_EQ_IF_SIGNAL	= 16;
const int ERR_DLG_ORIGIN_LIMIT_TIME_OVER		= 32;
const int ERR_DLG_ORIGIN_INTERLOCK				= 48;
const int ERR_DLG_ORIGIN_SERCOS_PHASE_ERROR		= 64;
const int ERR_DLG_ORIGIN_PC_ERROR				= 128;

//@const int DEF_MOTOR_ORIGIN_DLG_MAX_BUTTON		= 78;
const int DEF_MOTOR_ORIGIN_DLG_MAX_BUTTON		= 96;

//#ifdef DEF_USE_TRANSFER_CENTER
	const int DEF_MAX_ORIGIN_GROUP	= 21;
//#else
//	const int DEF_MAX_ORIGIN_GROUP	= 20;
//#endif
const int DEF_ORIGIN_GROUP_TAB_MOUNTER_Z	= 0;
const int DEF_ORIGIN_GROUP_TAB_MOUNTER_X	= 1;
const int DEF_ORIGIN_GROUP_R_TAB_MOUNTER_Z	= 2;
const int DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X	= 3;

const int DEF_ORIGIN_GROUP_BACKUP_Z			= 4;
const int DEF_ORIGIN_GROUP_INSPECTION_X		= 5;
const int DEF_ORIGIN_GROUP_R_BACKUP_Z		= 6;
const int DEF_ORIGIN_GROUP_R_INSPECTION_X	= 7;

const int DEF_ORIGIN_GROUP_F_TABCARRIER_X	= 8;
const int DEF_ORIGIN_GROUP_F_TABCARRIER_T	= 9;
const int DEF_ORIGIN_GROUP_F_TABCARRIER_Y	= 10;
const int DEF_ORIGIN_GROUP_R_TABCARRIER_X	= 11;
const int DEF_ORIGIN_GROUP_R_TABCARRIER_T	= 12;
const int DEF_ORIGIN_GROUP_R_TABCARRIER_Y	= 13;

const int DEF_ORIGIN_GROUP_F_TAB_PRESS		= 14;
const int DEF_ORIGIN_GROUP_R_TAB_PRESS		= 15;

const int DEF_ORIGIN_GROUP_PANEL_TRANSFER	= 16;

const int DEF_ORIGIN_GROUP_GANTRY			= 17;

const int DEF_ORIGIN_GROUP_F_CAMERA_EXPAND	= 18;
const int DEF_ORIGIN_GROUP_R_CAMERA_EXPAND	= 19;
//#ifdef DEF_USE_TRANSFER_CENTER
	const int DEF_ORIGIN_GROUP_HANDLER			= 20; //SJ_YYK 161104 Add.
//#endif
class IAxis;
class ISercosSetup;
class MManageOpPanel;
class MCANSetup;

class CAutoMotorOriginDlg : public CDialog
{
private:
	// Doolin kwangilshin 2017. 08. 24.
	// Add bIsSelect Flag.
	//
	void setAxisSelectFlag(int iSel, BOOL bIsSelected);
	// End.
	void axisSelect(int iSel);
	int checkBeforeOrigin(int iAxisId);
	int checkOnOrigin(int iAxisId);
	int checkAfterOrigin(int iAxisId);
	int checkCompleteOrigin(int iAxisId); //170922 JSH
	void clearAxisForIAMNode(BOOL bServoOnOff = FALSE);
	void getOriginGroupAxisOrder(int iGroupId, int* iAxisId); //161108 JSH Origin
	
	void checkAxisSelectionForGroup(int* rgiOriginMoveType);

	static UINT returnForcedOriginGroupAxis(LPVOID pParam);
	static UINT returnOriginNormalAxis(LPVOID pParam);

	void setData(SOriginGroupData* pSrc, SOriginGroupData* pDst);

	int doOriginReturnNormal(SOriginGroupData* pData);
	int doOriginReturnSingleAxis(SOriginGroupData* pData, int iAxisNo, int iPriority);

	BOOL isAxisGroupOriginComplete(SOriginGroupData* pData);
	BOOL isAxisGroupOriginComplete(SOriginGroupData* pData, int nPriority);

	int moveAxisToPlusLimitPos(SOriginGroupData* pData);
	int getPreMoveDistance(SOriginGroupData* pData);

	int moveOriginReturnReadyPos(SOriginGroupData* pData, int iRefAxisNo, int iMoveAxisNo);
	int moveOriginReturnSafePos(SOriginGroupData* pData, int iRefAxisNo, int iMoveAxisNo);

	void changeDisplayForceOriginButton(BOOL bState);

	int DlgID2AxisID(int iDialogID);

	// Doolin kwangilshin 2017. 08. 27.
	//
	BOOL	Check_IsOPPanelSwitch(int nOrgGrpNo);	
	// End.

// Construction
public:
	CAutoMotorOriginDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL ReturnOrigin(void);
	BOOL IsOriginComplete(int nPriority);

// Dialog Data
	//{{AFX_DATA(CAutoMotorOriginDlg)
	enum { IDD = IDD_AUTO_MOTOR_ORIGIN };
	//}}AFX_DATA

	CBtnEnh	m_btnForceOrigin;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoMotorOriginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBrush m_brBckColor;

	CBtnEnh m_btnAxisOrigin[DEF_MOTOR_ORIGIN_DLG_MAX_BUTTON];

	CBtnEnh	m_btnAllSelect;
	CBtnEnh	m_btnAllUnselect;
	CBtnEnh	m_btnServoOn;
	CBtnEnh	m_btnServoOff;
	CBtnEnh	m_btnReturnOrigin;
	CBtnEnh	m_btnExit;

	BOOL	m_rgbForceOriginReturn[DEF_AXIS_MAX_AXIS];

	BOOL	m_bAxisSelectFlag[DEF_AXIS_MAX_AXIS];
	BOOL	m_bAxisOriginFlag[DEF_AXIS_MAX_AXIS];
	CString	m_strOnOriginIcon;
	CString	m_strOffOriginIcon;

	double m_rgdTabMounterPrepareMoveDistance[DEF_MAX_GROUP];
	double m_rgdInspectionPrepareMoveDistance[DEF_MAX_GROUP];
	double m_rgdTabCarrierPrepareMoveDistance[DEF_MAX_GROUP];

	UINT	m_uiTimerID;
	ISercosSetup* m_plnkSercosSetup;
	IAxis*	m_plnkAxis[DEF_AXIS_MAX_AXIS];
	MManageOpPanel* m_plnkManageOpPanel;
	MSystemData* m_plnkSystemData;

	// hongju_SESL
	MCANSetup*	m_plnkCANSetup;

	MPanelAligner* m_plnkPanelAligner;

	// Generated message map functions
	//{{AFX_MSG(CAutoMotorOriginDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnInitRun();
	afx_msg void OnServoOn();
	afx_msg void OnServoOff();
	afx_msg void OnAllSelect();
	afx_msg void OnAllCancel();
	afx_msg void OnClickAxisOrigin0();
	afx_msg void OnClickAxisOrigin1();
	afx_msg void OnClickAxisOrigin2();
	afx_msg void OnClickAxisOrigin3();
	afx_msg void OnClickAxisOrigin4();
	afx_msg void OnClickAxisOrigin5();
	afx_msg void OnClickAxisOrigin6();
	afx_msg void OnClickAxisOrigin7();
	afx_msg void OnClickAxisOrigin8();
	afx_msg void OnClickAxisOrigin9();
	afx_msg void OnClickAxisOrigin10();
	afx_msg void OnClickAxisOrigin11();
	afx_msg void OnClickAxisOrigin12();
	afx_msg void OnClickAxisOrigin13();
	afx_msg void OnClickAxisOrigin14();
	afx_msg void OnClickAxisOrigin15();
	afx_msg void OnClickAxisOrigin16();
	afx_msg void OnClickAxisOrigin17();
	afx_msg void OnClickAxisOrigin18();
	afx_msg void OnClickAxisOrigin19();
	afx_msg void OnClickAxisOrigin20();
	afx_msg void OnClickAxisOrigin21();
	afx_msg void OnClickAxisOrigin22();
	afx_msg void OnClickAxisOrigin23();
	afx_msg void OnClickAxisOrigin24();
	afx_msg void OnClickAxisOrigin25();
	afx_msg void OnClickAxisOrigin26();
	afx_msg void OnClickAxisOrigin27();
	afx_msg void OnClickAxisOrigin28();
	afx_msg void OnClickAxisOrigin29();
	afx_msg void OnClickAxisOrigin30();
	afx_msg void OnClickAxisOrigin31();
	afx_msg void OnClickAxisOrigin32();
	afx_msg void OnClickAxisOrigin33();
	afx_msg void OnClickAxisOrigin34();
	afx_msg void OnClickAxisOrigin35();
	afx_msg void OnClickAxisOrigin36();
	afx_msg void OnClickAxisOrigin37();
	afx_msg void OnClickAxisOrigin38();
	afx_msg void OnClickAxisOrigin39();
	afx_msg void OnClickAxisOrigin40();
	afx_msg void OnClickAxisOrigin41();
	afx_msg void OnClickAxisOrigin42();
	afx_msg void OnClickAxisOrigin43();
	afx_msg void OnClickAxisOrigin44();
	afx_msg void OnClickAxisOrigin45();
	afx_msg void OnClickAxisOrigin46();
	afx_msg void OnClickAxisOrigin47();
	afx_msg void OnClickAxisOrigin48();
	afx_msg void OnClickAxisOrigin49();
	afx_msg void OnClickAxisOrigin50();
	afx_msg void OnClickAxisOrigin51();
	afx_msg void OnClickAxisOrigin52();
	afx_msg void OnClickAxisOrigin53();
	afx_msg void OnClickAxisOrigin54();
	afx_msg void OnClickAxisOrigin55();
	afx_msg void OnClickAxisOrigin56();
	afx_msg void OnClickAxisOrigin57();
	afx_msg void OnClickAxisOrigin58();
	afx_msg void OnClickAxisOrigin59();
	afx_msg void OnClickAxisOrigin60();
	afx_msg void OnClickAxisOrigin61();
	afx_msg void OnClickAxisOrigin62();
	afx_msg void OnClickAxisOrigin63();
	afx_msg void OnClickAxisOrigin64();
	afx_msg void OnClickAxisOrigin65();
	afx_msg void OnClickAxisOrigin66();
	afx_msg void OnClickAxisOrigin67();
	afx_msg void OnClickAxisOrigin68();
	afx_msg void OnClickAxisOrigin69();
	afx_msg void OnClickAxisOrigin70();
	afx_msg void OnClickAxisOrigin71();
	afx_msg void OnClickAxisOrigin72();
	afx_msg void OnClickAxisOrigin73();
	afx_msg void OnClickAxisOrigin74();
	afx_msg void OnClickAxisOrigin75();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnForceOriginReturn();
	afx_msg void OnSERCOSnetInit();
	afx_msg void OnClickAxisOrigin76();
	afx_msg void OnClickAxisOrigin77();
	afx_msg void OnClickAxisOrigin78();
	afx_msg void OnClickAxisOrigin79();
	afx_msg void OnClickAxisOrigin80();
	afx_msg void OnClickAxisOrigin81();
	afx_msg void OnClickAxisOrigin82();
	afx_msg void OnClickAxisOrigin83();
	afx_msg void OnClickAxisOrigin84();
	afx_msg void OnClickAxisOrigin85();
	afx_msg void OnClickAxisOrigin86();
	afx_msg void OnClickAxisOrigin87();
	afx_msg void OnClickAxisOrigin88();
	afx_msg void OnClickAxisOrigin89();
	afx_msg void OnClickAxisOrigin90();
	afx_msg void OnClickAxisOrigin91();
	afx_msg void OnClickAxisOrigin92();
	afx_msg void OnClickAxisOrigin93();
	afx_msg void OnClickAxisOrigin94();
	afx_msg void OnClickAxisOrigin95();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	static int sm_rgiGroupResult[DEF_MAX_ORIGIN_GROUP];
	static int sm_rgiGroupComplete[DEF_MAX_ORIGIN_GROUP];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTORORIGINDLG_H__FE0BE927_C365_47E6_9442_29DCDC34275A__INCLUDED_)
