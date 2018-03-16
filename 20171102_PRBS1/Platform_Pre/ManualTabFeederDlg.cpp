// ManualTabFeederDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "ManualTabFeederDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTabFeeder.h"
#include "MTabCarrier.h"
#include "MCtrlTabFeeder.h"
#include "IAxis.h"
#include "IOpPanel.h"
#include "MPreBonderData.h"
#include "DefIOAddrPreBonder.h"
#include "MLCNet.h"
#include "IIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualTabFeederDlg dialog
extern	MPlatformOlbSystem	MOlbSystemPre; 

CManualTabFeederDlg::CManualTabFeederDlg(int iGroupNo, CWnd* pParent /*=NULL*/)
	: CDialog(CManualTabFeederDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualTabFeederDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iGroupNo = iGroupNo;
	
	if (DEF_FRONT_GROUP == iGroupNo)
	{
		m_plnkTabFeeder		= MOlbSystemPre.GetTabFeederComponent(DEF_TABFEEDER1);
		m_plnkCtrlTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1);
	}
	else
	{
		m_plnkTabFeeder		= MOlbSystemPre.GetTabFeederComponent(DEF_TABFEEDER2);
		m_plnkCtrlTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2);
	}

	m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
	m_plnkOpPanel		= MOlbSystemPre.GetOpPanelComponent();
	m_plnkIO			= MOlbSystemPre.GetIOComponent();

	m_bOnWithdrawingReel = FALSE;

    iCutTestRealCount	= 0;
    iCutTestSetCount	= 0;
}


void CManualTabFeederDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualTabFeederDlg)
	DDX_Control(pDX, IDB_PRESS1_VAC_ON,					m_btnPress1VacOn);
	DDX_Control(pDX, IDB_PRESS1_VAC_OFF,				m_btnPress1VacOff);
	DDX_Control(pDX, IDB_NEW_REEL_VAC_ON,				m_btnNewReelVacOn);
	DDX_Control(pDX, IDB_NEW_REEL_VAC_OFF,				m_btnNewReelVacOff);
	DDX_Control(pDX, IDB_REEL_DOWN,						m_btnReelDown);
	DDX_Control(pDX, IDB_REEL_UP,						m_btnReelUp);
	DDX_Control(pDX, IDB_REEL_CLAMP_DOWN,				m_btnReelClampDown);
	DDX_Control(pDX, IDB_REEL_CLAMP_UP,					m_btnReelClampUp);
	DDX_Control(pDX, IDB_SUPPLIER_CHUCK_LOCK,			m_btnSupplierChuckLock);
	DDX_Control(pDX, IDB_SUPPLIER_CHUCK_UNLOCK,			m_btnSupplierChuckUnlock);
	DDX_Control(pDX, IDB_CUTTING_SUPPORT_LEFT_DOWN,		m_btnCuttingSupportLeftDown);
	DDX_Control(pDX, IDB_CUTTING_SUPPORT_LEFT_UP,		m_btnCuttingSupportLeftUp);
	DDX_Control(pDX, IDB_CUTTING_SUPPORT_RIGHT_DOWN,	m_btnCuttingSupportRightDown);
	DDX_Control(pDX, IDB_CUTTING_SUPPORT_RIGHT_UP,		m_btnCuttingSupportRightUp);
	DDX_Control(pDX, IDB_FEEDER_SPROCKET_DOWN,			m_btnFeederSprocketDown);
	DDX_Control(pDX, IDB_FEEDER_SPROCKET_UP,			m_btnFeederSprocketUp);
	DDX_Control(pDX, IDB_TENSION_SPROCKET_DOWN,			m_btnTensionSprocketDown);
	DDX_Control(pDX, IDB_TENSION_SPROCKET_UP,			m_btnTensionSprocketUp);
	DDX_Control(pDX, IDB_RECOVERY_SPROCKET_DOWN,		m_btnRecoverySprocketDown);
	DDX_Control(pDX, IDB_RECOVERY_SPROCKET_UP,			m_btnRecoverySprocketUp);
	DDX_Control(pDX, IDB_SPROCKET_ALL_DOWN,				m_btnSprocketAllDown);
	DDX_Control(pDX, IDB_SPROCKET_ALL_UP,				m_btnSprocketAllUp);
	DDX_Control(pDX, IDB_ATTACH_PRESS_DOWN,				m_btnAttachPressDown);
	DDX_Control(pDX, IDB_ATTACH_PRESS_UP,				m_btnAttachPressUp);
	DDX_Control(pDX, IDB_ATTACH_REEL_CUTTER_UP,			m_btnAttachReelCutterUp);
	DDX_Control(pDX, IDB_ATTACH_REEL_CUTTER_DOWN,		m_btnAttachReelCutterDown);
	DDX_Control(pDX, IDB_ATTACH_REEL_FORWARD,			m_btnAttachReelForward);
	DDX_Control(pDX, IDB_ATTACH_REEL_BACKWARD,			m_btnAttachReelBackward);
	DDX_Control(pDX, IDB_RECOVERY_MOTOR_ON,				m_btnRecoveryMotorOn);
	DDX_Control(pDX, IDB_RECOVERY_MOTOR_OFF,			m_btnRecoveryMotorOff);
	DDX_Control(pDX, IDB_P_REEL_MOTOR_ON,				m_btnPReelMotorOn);
	DDX_Control(pDX, IDB_P_REEL_REVERSE_MOTOR_ON,		m_btnPReelReverseMotorOn);
	DDX_Control(pDX, IDB_P_REEL_MOTOR_OFF,				m_btnPReelMotorOff);
	DDX_Control(pDX, IDB_SUPPLIER_TENSION_MOTOR_OFF,	m_btnSupplierTensionMotorOff);
	DDX_Control(pDX, IDB_SUPPLIER_TENSION_SMALL,		m_btnSupplierTensionSmall);
	DDX_Control(pDX, IDB_BRUSH_MOTOR_ON,				m_btnBrushMotorOn);
	DDX_Control(pDX, IDB_BRUSH_MOTOR_OFF,				m_btnBrushMotorOff);
	DDX_Control(pDX, IDS_TAB_PRESS_BADTAB_DETECT_1,		m_lBadTabDetect1);
	DDX_Control(pDX, IDS_TAB_PRESS_BADTAB_DETECT_2,		m_lBadTabDetect2);
	DDX_Control(pDX, IDS_TENSION_UPPER_LIMIT,			m_lTensionUpperLimit);
	DDX_Control(pDX, IDS_TENSION_UPPER,					m_lTensionUpper);
	DDX_Control(pDX, IDS_TENSION_LOWER,					m_lTensionLower);
	DDX_Control(pDX, IDS_TENSION_LOWER_LIMIT,			m_lTensionLowerLimit);
	DDX_Control(pDX, IDS_UNFOLD_DETECT_VALUE,			m_LblUnfoldDetect);
	DDX_Control(pDX, IDS_CURRENT_REEL_DETECT_VALUE,		m_LblCurrentReelDetect);
	DDX_Control(pDX, IDS_NEW_REEL_DETECT_VALUE,			m_LblNewReelDetect);
	DDX_Control(pDX, IDS_FEEDER_MOLD_CHECK,				m_bTabPressMoldCheck);
	DDX_Control(pDX, IDB_SPROCKET1_CLUTCH_ON,			m_btnSprocketClutchOn);
	DDX_Control(pDX, IDB_SPROCKET1_CLUTCH_OFF,			m_btnSprocketClutchOff);
	DDX_Control(pDX, IDB_WITHDRAW_REEL, m_btnWithdrawReel);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_HOME_SPROCKET1,	m_BtnCmd[0]);
	DDX_Control(pDX, IDB_HOME_SPROCKET2,	m_BtnCmd[1]);
	DDX_Control(pDX, IDB_FEEDING_REVERSE,	m_BtnCmd[2]);
}


BEGIN_MESSAGE_MAP(CManualTabFeederDlg, CDialog)
	//{{AFX_MSG_MAP(CManualTabFeederDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualTabFeederDlg message handlers

BEGIN_EVENTSINK_MAP(CManualTabFeederDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualTabFeederDlg)
	ON_EVENT(CManualTabFeederDlg, IDB_PRESS1_VAC_ON, -600 /* Click */,					OnClickPress1VacOn, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_PRESS1_VAC_OFF, -600 /* Click */,					OnClickPress1VacOff, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_NEW_REEL_VAC_ON, -600 /* Click */,				OnClickNewReelVacOn, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_NEW_REEL_VAC_OFF, -600 /* Click */,				OnClickNewReelVacOff, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_REEL_UP, -600 /* Click */,						OnClickReelUp, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_REEL_DOWN, -600 /* Click */,						OnClickReelDown, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_REEL_CLAMP_UP, -600 /* Click */,					OnClickReelClampUp, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_REEL_CLAMP_DOWN, -600 /* Click */,				OnClickReelClampDown, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_SUPPLIER_CHUCK_LOCK, -600 /* Click */,			OnClickSupplierChuckLock, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_SUPPLIER_CHUCK_UNLOCK, -600 /* Click */,			OnClickSupplierChuckUnlock, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_CUTTING_SUPPORT_LEFT_UP, -600 /* Click */,		OnClickCuttingSupportLeftUp, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_CUTTING_SUPPORT_LEFT_DOWN, -600 /* Click */,		OnClickCuttingSupportLeftDown, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_CUTTING_SUPPORT_RIGHT_UP, -600 /* Click */,		OnClickCuttingSupportRightUp, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_CUTTING_SUPPORT_RIGHT_DOWN, -600 /* Click */,		OnClickCuttingSupportRightDown, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_FEEDER_SPROCKET_UP, -600 /* Click */,				OnClickFeederSprocketOpen, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_FEEDER_SPROCKET_DOWN, -600 /* Click */,			OnClickFeederSprocketClose, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_TENSION_SPROCKET_UP, -600 /* Click */,			OnClickTensionSprocketOpen, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_TENSION_SPROCKET_DOWN, -600 /* Click */,			OnClickTensionSprocketClose, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_RECOVERY_SPROCKET_UP, -600 /* Click */,			OnClickRecoverySprocketOpen, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_RECOVERY_SPROCKET_DOWN, -600 /* Click */,			OnClickRecoverySprocketClose, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_SPROCKET_ALL_UP, -600 /* Click */,				OnClickSprocketAllOpen, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_SPROCKET_ALL_DOWN, -600 /* Click */,				OnClickSprocketAllClose, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_ATTACH_PRESS_UP, -600 /* Click */,				OnClickAttachPressUp, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_ATTACH_PRESS_DOWN, -600 /* Click */,				OnClickAttachPressDown, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_ATTACH_REEL_CUTTER_UP, -600 /* Click */,			OnClickAttachReelCutterOpen, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_ATTACH_REEL_CUTTER_DOWN, -600 /* Click */,		OnClickAttachReelCutterCutting, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_ATTACH_REEL_FORWARD, -600 /* Click */,			OnClickAttachReelForward, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_ATTACH_REEL_BACKWARD, -600 /* Click */,			OnClickAttachReelBackward, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_EXIT, -600 /* Click */,							OnClickExit, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_HOME_SPROCKET1, -600 /* Click */,					OnClickSprocket1Home, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_HOME_SPROCKET2, -600 /* Click */,					OnClickSprocket2Home, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_ONETAB, -600 /* Click */,							OnClickOnetab, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_ONE_PITCH, -600 /* Click */,						OnClickOnePitch, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_TENTAB, -600 /* Click */,							OnClickTentab, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_TEN_PITCH, -600 /* Click */,						OnClickTenPitch, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_FEEDING_REVERSE, -600 /* Click */,				OnClickFeedingReverse, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_RECOVERY_MOTOR_ON, -600 /* Click */,				OnClickRecoveryMotorOn, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_RECOVERY_MOTOR_OFF, -600 /* Click */,				OnClickRecoveryMotorOff, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_P_REEL_MOTOR_ON, -600 /* Click */,				OnClickPReelMotorOn, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_P_REEL_MOTOR_OFF, -600 /* Click */,				OnClickPReelMotorOff, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_SUPPLIER_TENSION_MOTOR_OFF, -600 /* Click */,		OnClickSupplierTensionMotorOff, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_SUPPLIER_TENSION_SMALL, -600 /* Click */,			OnClickSupplierTensionSmall, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_BRUSH_MOTOR_ON, -600 /* Click */,					OnClickBrushMotorOn, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_BRUSH_MOTOR_OFF, -600 /* Click */,				OnClickBrushMotorOff, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_PRESS_TABIC, -600 /* Click */,					OnClickPressTabic, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_FORCED_HOMING, -600 /* Click */,					OnClickForcedHoming, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_REMOVE_MOLD, -600 /* Click */,					OnRemoveMold, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_WITHDRAW_REEL, -600 /* Click */,					OnWithdrawReel, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_SPROCKET1_CLUTCH_ON, -600 /* Click */, OnClickSprocket1ClutchOn, VTS_NONE)
	ON_EVENT(CManualTabFeederDlg, IDB_SPROCKET1_CLUTCH_OFF, -600 /* Click */, OnClickSprocket1ClutchOff, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManualTabFeederDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	BOOL	bStatus = FALSE;
	BOOL	bOldStatus = FALSE;

	if (m_uiTimerID == nIDEvent)
	{
		/*/
		if (DEF_FRONT_GROUP == m_iGroupNo)
		{
			// TabFeeder Press1 Attach Chuck
#ifdef DEF_SOURCE_SYSTEM
			bStatus = MOlbSystemPre.GetIOComponent()->IsOn(OUT_TABPRESS1_SUPPLIER_CHUCK_SHRINK);
#else
			bStatus = MOlbSystemPre.GetIOComponent()->IsOff(OUT_TABPRESS1_SUPPLIER_CHUCK_EXPANDSHRINK);
#endif
			bOldStatus = m_btnSupplierChuckLock.GetValue();
			if ((bStatus == TRUE) && (bOldStatus == FALSE))
				m_btnSupplierChuckLock.SetValue(TRUE);
			else if ((bStatus == FALSE) && (bOldStatus != FALSE))
				m_btnSupplierChuckLock.SetValue(FALSE);
			
#ifdef DEF_SOURCE_SYSTEM
			bStatus = MOlbSystemPre.GetIOComponent()->IsOn(OUT_TABPRESS1_SUPPLIER_CHUCK_EXPAND);
#else
			bStatus = MOlbSystemPre.GetIOComponent()->IsOn(OUT_TABPRESS1_SUPPLIER_CHUCK_EXPANDSHRINK);
#endif
			bOldStatus = m_btnSupplierChuckUnlock.GetValue();
			if ((bStatus == TRUE) && (bOldStatus == FALSE))
				m_btnSupplierChuckUnlock.SetValue(TRUE);
			else if ((bStatus == FALSE) && (bOldStatus != FALSE))
				m_btnSupplierChuckUnlock.SetValue(FALSE);
		}
		else
		{
			// TabFeeder Press2 Attach Chuck
#ifdef DEF_SOURCE_SYSTEM
			bStatus = MOlbSystemPre.GetIOComponent()->IsOn(OUT_TABPRESS2_SUPPLIER_CHUCK_SHRINK);
#else
			bStatus = MOlbSystemPre.GetIOComponent()->IsOff(OUT_TABPRESS2_SUPPLIER_CHUCK_EXPANDSHRINK);
#endif
			bOldStatus = m_btnSupplierChuckLock.GetValue();
			if ((bStatus == TRUE) && (bOldStatus == FALSE))
				m_btnSupplierChuckLock.SetValue(TRUE);
			else if ((bStatus == FALSE) && (bOldStatus != FALSE))
				m_btnSupplierChuckLock.SetValue(FALSE);
			
#ifdef DEF_SOURCE_SYSTEM
			bStatus = MOlbSystemPre.GetIOComponent()->IsOn(OUT_TABPRESS2_SUPPLIER_CHUCK_EXPAND);
#else
			bStatus = MOlbSystemPre.GetIOComponent()->IsOn(OUT_TABPRESS2_SUPPLIER_CHUCK_EXPANDSHRINK);
#endif
			bOldStatus = m_btnSupplierChuckUnlock.GetValue();
			if ((bStatus == TRUE) && (bOldStatus == FALSE))
				m_btnSupplierChuckUnlock.SetValue(TRUE);
			else if ((bStatus == FALSE) && (bOldStatus != FALSE))
				m_btnSupplierChuckUnlock.SetValue(FALSE);
		}
		/*/

		/*/
		// Protect Reel Motor On Status Check
		bStatus = m_plnkTabFeeder->IsPReelMotorOn();
		bOldStatus = m_btnPReelMotorOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPReelMotorOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPReelMotorOn.SetValue(FALSE);
		/*/

		// Protect Reel Motor On Status Check
//		bStatus = m_plnkTabFeeder->IsPReelReverseMotorOn();
//		bOldStatus = m_btnPReelReverseMotorOn.GetValue();
//		if ((bStatus == TRUE) && (bOldStatus == FALSE))
//			m_btnPReelReverseMotorOn.SetValue(TRUE);
//		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
//			m_btnPReelReverseMotorOn.SetValue(FALSE);

		// Recovery Motor Off Status Check
//		bStatus = m_plnkTabFeeder->IsPReelMotorOff() && m_plnkTabFeeder->IsPReelReverseMotorOff();
		/*/
		bStatus = m_plnkTabFeeder->IsPReelMotorOff();
		bOldStatus = m_btnPReelMotorOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPReelMotorOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPReelMotorOff.SetValue(FALSE);
		/*/
		bStatus = m_plnkTabFeeder->IsClutchOnSprocket1Axis();
		bOldStatus = m_btnSprocketClutchOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnSprocketClutchOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnSprocketClutchOn.SetValue(FALSE);

		bStatus = !m_plnkTabFeeder->IsClutchOnSprocket1Axis();
		bOldStatus = m_btnSprocketClutchOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnSprocketClutchOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnSprocketClutchOff.SetValue(FALSE);

		// TabFeeder Press1 Vacuum
		bStatus = m_plnkTabFeeder->IsAbsorbPress();
		bOldStatus = m_btnPress1VacOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPress1VacOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPress1VacOn.SetValue(FALSE);

		bStatus = m_plnkTabFeeder->IsReleasePress();
		bOldStatus = m_btnPress1VacOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPress1VacOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPress1VacOff.SetValue(FALSE);


		// Reel Up/Down Cylinder
		// Reel Up Status Check
		bStatus = m_plnkTabFeeder->IsInMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
		bOldStatus = m_btnReelUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnReelUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnReelUp.SetValue(FALSE);

		// Reel Down Status Check
		bStatus = m_plnkTabFeeder->IsInMoldPinPos(DEF_MOLDPIN_FIX_POS);
		bOldStatus = m_btnReelDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnReelDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnReelDown.SetValue(FALSE);

		// Tape Reel Clamp Up/Down Cylinder
		// Tape Reel Clamp Up Status Check


		// Feeder Sprocket Up Status Check
/*
		bStatus = m_plnkTabFeeder->IsOpenFeederSprocketCyl();
		bOldStatus = m_btnFeederSprocketUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnFeederSprocketUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnFeederSprocketUp.SetValue(FALSE);

		// Feeder Sprocket Down Status Check
		bStatus = m_plnkTabFeeder->IsCloseFeederSprocketCyl();
		bOldStatus = m_btnFeederSprocketDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnFeederSprocketDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnFeederSprocketDown.SetValue(FALSE);
*/

/*
		// Tension Sprocket Up Status Check
		bStatus = m_plnkTabFeeder->IsOpenTensionSprocketCyl();
		bOldStatus = m_btnTensionSprocketUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnTensionSprocketUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnTensionSprocketUp.SetValue(FALSE);

		// Tension Sprocket Down Status Check
		bStatus = m_plnkTabFeeder->IsCloseTensionSprocketCyl();
		bOldStatus = m_btnTensionSprocketDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnTensionSprocketDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnTensionSprocketDown.SetValue(FALSE);
*/

		// Recovery Sprocket Up Status Check
		bStatus = m_plnkTabFeeder->IsOpenRecoverySprocketCyl();
		bOldStatus = m_btnRecoverySprocketUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnRecoverySprocketUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnRecoverySprocketUp.SetValue(FALSE);

		// Recovery Sprocket Down Status Check
		bStatus = m_plnkTabFeeder->IsCloseRecoverySprocketCyl();
		bOldStatus = m_btnRecoverySprocketDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnRecoverySprocketDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnRecoverySprocketDown.SetValue(FALSE);

/*		// Sprocket All Up Status Check
		bStatus = m_plnkTabFeeder->IsOpenRecoverySprocketCyl()
					&& m_plnkTabFeeder->IsOpenFeederSprocketCyl()
					&& m_plnkTabFeeder->IsOpenTensionSprocketCyl();
		bOldStatus = m_btnSprocketAllUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnSprocketAllUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnSprocketAllUp.SetValue(FALSE);

		// Sprocket All Down Status Check
		bStatus = m_plnkTabFeeder->IsCloseRecoverySprocketCyl()
					&& m_plnkTabFeeder->IsCloseFeederSprocketCyl()
					&& m_plnkTabFeeder->IsCloseTensionSprocketCyl();
		bOldStatus = m_btnSprocketAllDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnSprocketAllDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnSprocketAllDown.SetValue(FALSE);
*/

		// Recovery Motor On Status Check
		bStatus = m_plnkTabFeeder->IsRecoveryIndMotorOn();
		bOldStatus = m_btnRecoveryMotorOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnRecoveryMotorOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnRecoveryMotorOn.SetValue(FALSE);

		// Recovery Motor Off Status Check
		bStatus = m_plnkTabFeeder->IsRecoveryIndMotorOff();
		bOldStatus = m_btnRecoveryMotorOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnRecoveryMotorOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnRecoveryMotorOff.SetValue(FALSE);


		// Supplier Motor On Status Check
		bStatus = m_plnkTabFeeder->IsServoOffFeederReel();
		bOldStatus = m_btnSupplierTensionMotorOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnSupplierTensionMotorOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnSupplierTensionMotorOff.SetValue(FALSE);

		bStatus = m_plnkTabFeeder->IsServoOnFeederReel();
		bOldStatus = m_btnSupplierTensionSmall.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnSupplierTensionSmall.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnSupplierTensionSmall.SetValue(FALSE);

		// Brush Motor On/Off
		// Brush Motor On Status Check
/*		bStatus = m_plnkTabFeeder->IsBrushMotorOn();
		bOldStatus = m_btnBrushMotorOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnBrushMotorOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnBrushMotorOn.SetValue(FALSE);

		// Brush Motor On Status Check
		bStatus = m_plnkTabFeeder->IsBrushMotorOff();
		bOldStatus = m_btnBrushMotorOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnBrushMotorOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnBrushMotorOff.SetValue(FALSE);
*/
		// TabPress Mold Check
		bStatus = m_plnkTabFeeder->IsPressMoldDetect();
		bOldStatus = m_bTabPressMoldCheck.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_bTabPressMoldCheck.SetCaption(_T("On"));
			m_bTabPressMoldCheck.SetBackColor(DEF_ON_COLOR);
			m_bTabPressMoldCheck.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_bTabPressMoldCheck.SetCaption(_T("Off"));
			m_bTabPressMoldCheck.SetBackColor(DEF_OFF_COLOR);
			m_bTabPressMoldCheck.SetValue(bStatus);
		}

		// Feeder Sensor Status Check
		// Bad Tab Check1
		if (TRUE == m_plnkPreBonderData->m_bUseTCP)
//@			bStatus = m_plnkTabFeeder->IsBadTabDetected(DEF_BADTABDETECTOR_LEFT_SENSOR_TCP);
			bStatus = m_plnkTabFeeder->IsBadTabDetected(DEF_BADTABDETECTOR_SENSOR_TCP);
		else
//@			bStatus = m_plnkTabFeeder->IsBadTabDetected(DEF_BADTABDETECTOR_LEFT_SENSOR_COF);
			bStatus = m_plnkTabFeeder->IsBadTabDetected(DEF_BADTABDETECTOR_SENSOR_COF);
		
		bOldStatus = m_lBadTabDetect1.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_lBadTabDetect1.SetCaption(_T("On"));
			m_lBadTabDetect1.SetBackColor(DEF_ON_COLOR);
			m_lBadTabDetect1.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_lBadTabDetect1.SetCaption(_T("Off"));
			m_lBadTabDetect1.SetBackColor(DEF_OFF_COLOR);
			m_lBadTabDetect1.SetValue(bStatus);
		}
		
		// Bad Tab Check2
		if (TRUE == m_plnkPreBonderData->m_bUseTCP)
//@			bStatus = m_plnkTabFeeder->IsBadTabDetected(DEF_BADTABDETECTOR_RIGHT_SENSOR_TCP);
			bStatus = m_plnkTabFeeder->IsBadTabDetected(DEF_BADTABDETECTOR_SENSOR_TCP);
		else
//@			bStatus = m_plnkTabFeeder->IsBadTabDetected(DEF_BADTABDETECTOR_RIGHT_SENSOR_COF);
			bStatus = m_plnkTabFeeder->IsBadTabDetected(DEF_BADTABDETECTOR_SENSOR_COF);
		
		bOldStatus = m_lBadTabDetect2.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_lBadTabDetect2.SetCaption(_T("On"));
			m_lBadTabDetect2.SetBackColor(DEF_ON_COLOR);
			m_lBadTabDetect2.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_lBadTabDetect2.SetCaption(_T("Off"));
			m_lBadTabDetect2.SetBackColor(DEF_OFF_COLOR);
			m_lBadTabDetect2.SetValue(bStatus);
		}
		
		// Tension Upper Limit Detect
		bStatus = m_plnkTabFeeder->IsTensionUpperLimitDetected();
		bOldStatus = m_lTensionUpperLimit.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_lTensionUpperLimit.SetCaption(_T("On"));
			m_lTensionUpperLimit.SetBackColor(DEF_ON_COLOR);
			m_lTensionUpperLimit.SetValue(bStatus);
			
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_lTensionUpperLimit.SetCaption(_T("Off"));
			m_lTensionUpperLimit.SetBackColor(DEF_OFF_COLOR);
			m_lTensionUpperLimit.SetValue(bStatus);
		}
		
		// Tension Upper Detect
		bStatus = m_plnkTabFeeder->IsTensionUpperDetected();
		bOldStatus = m_lTensionUpper.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_lTensionUpper.SetCaption(_T("On"));
			m_lTensionUpper.SetBackColor(DEF_ON_COLOR);
			m_lTensionUpper.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_lTensionUpper.SetCaption(_T("Off"));
			m_lTensionUpper.SetBackColor(DEF_OFF_COLOR);
			m_lTensionUpper.SetValue(bStatus);
		}
		
		// Tension Lower Detect
		bStatus = m_plnkTabFeeder->IsTensionLowerDetected();
		bOldStatus = m_lTensionLower.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_lTensionLower.SetCaption(_T("On"));
			m_lTensionLower.SetBackColor(DEF_ON_COLOR);
			m_lTensionLower.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_lTensionLower.SetCaption(_T("Off"));
			m_lTensionLower.SetBackColor(DEF_OFF_COLOR);
			m_lTensionLower.SetValue(bStatus);
		}
		
		// Tension Lower Limit Detect
		bStatus = m_plnkTabFeeder->IsTensionLowerLimitDetected();
		bOldStatus = m_lTensionLowerLimit.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_lTensionLowerLimit.SetCaption(_T("On"));
			m_lTensionLowerLimit.SetBackColor(DEF_ON_COLOR);
			m_lTensionLowerLimit.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_lTensionLowerLimit.SetCaption(_T("Off"));
			m_lTensionLowerLimit.SetBackColor(DEF_OFF_COLOR);
			m_lTensionLowerLimit.SetValue(bStatus);
		}

		// Recovery Tape Detect
/*		bStatus = m_plnkTabFeeder->IsRecoveryTapeDetected();
		bOldStatus = m_LblRecoveryTapeDetect.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_LblRecoveryTapeDetect.SetCaption("On");
			m_LblRecoveryTapeDetect.SetBackColor(DEF_ON_COLOR);
			m_LblRecoveryTapeDetect.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_LblRecoveryTapeDetect.SetCaption("Off");
			m_LblRecoveryTapeDetect.SetBackColor(DEF_OFF_COLOR);
			m_LblRecoveryTapeDetect.SetValue(bStatus);
		}
*/
		// Rail Discard Detect
		bStatus = m_plnkTabFeeder->IsRailUnfoldDetected();
		bOldStatus = m_LblUnfoldDetect.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_LblUnfoldDetect.SetCaption(_T("On"));
			m_LblUnfoldDetect.SetBackColor(DEF_ON_COLOR);
			m_LblUnfoldDetect.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_LblUnfoldDetect.SetCaption(_T("Off"));
			m_LblUnfoldDetect.SetBackColor(DEF_OFF_COLOR);
			m_LblUnfoldDetect.SetValue(bStatus);
		}

		// Current Reel Detect
		bStatus = m_plnkTabFeeder->IsCurrentReelDtetcted();
		bOldStatus = m_LblCurrentReelDetect.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_LblCurrentReelDetect.SetCaption(_T("On"));
			m_LblCurrentReelDetect.SetBackColor(DEF_ON_COLOR);
			m_LblCurrentReelDetect.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_LblCurrentReelDetect.SetCaption(_T("Off"));
			m_LblCurrentReelDetect.SetBackColor(DEF_OFF_COLOR);
			m_LblCurrentReelDetect.SetValue(bStatus);
		}

		// New Reel Detect
		bStatus = m_plnkTabFeeder->IsNewReelDtetcted();
		bOldStatus = m_LblNewReelDetect.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_LblNewReelDetect.SetCaption(_T("On"));
			m_LblNewReelDetect.SetBackColor(DEF_ON_COLOR);
			m_LblNewReelDetect.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_LblNewReelDetect.SetCaption(_T("Off"));
			m_LblNewReelDetect.SetBackColor(DEF_OFF_COLOR);
			m_LblNewReelDetect.SetValue(bStatus);
		}

		// Cmd Sprocket1 Home State Display
		bStatus = m_plnkCtrlTabFeeder->GetHomeFlagSprocket1();
		bOldStatus = m_rgbCmdState[DEF_CMD_HOME_SPROCKET1];
		if (TRUE == bStatus && FALSE == bOldStatus)
			m_rgbCmdState[DEF_CMD_HOME_SPROCKET1] = TRUE;
		else if (FALSE == bStatus && TRUE == bOldStatus)
			m_rgbCmdState[DEF_CMD_HOME_SPROCKET1] = FALSE;

		m_BtnCmd[DEF_CMD_HOME_SPROCKET1].SetValue(m_rgbCmdState[DEF_CMD_HOME_SPROCKET1]);

		// Cmd Sprocket2 Home State Display
		bStatus = m_plnkCtrlTabFeeder->GetHomeFlagSprocket2();
		bOldStatus = m_rgbCmdState[DEF_CMD_HOME_SPROCKET2];
		if (TRUE == m_plnkCtrlTabFeeder->GetFindTabICFlag() && TRUE == bStatus && FALSE == bOldStatus)
			m_rgbCmdState[DEF_CMD_HOME_SPROCKET2] = TRUE;
		else if (FALSE == bStatus && TRUE == bOldStatus)
			m_rgbCmdState[DEF_CMD_HOME_SPROCKET2] = FALSE;

		m_BtnCmd[DEF_CMD_HOME_SPROCKET2].SetValue(m_rgbCmdState[DEF_CMD_HOME_SPROCKET2]);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CManualTabFeederDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_iGroupNo == DEF_FRONT_GROUP)
		SetWindowText(_T("Front Tab Feeder Manual Dialog"));
	else	// if (m_iGroupNo == DEF_REAR_GROUP)
		SetWindowText(_T("Rear Tab Feeder Manual Dialog"));

	m_btnNewReelVacOn.EnableWindow(FALSE);
	m_btnNewReelVacOff.EnableWindow(FALSE);

	m_btnReelClampUp.EnableWindow(FALSE);
	m_btnReelClampDown.EnableWindow(FALSE);

	m_btnCuttingSupportRightUp.EnableWindow(FALSE);
	m_btnCuttingSupportRightDown.EnableWindow(FALSE);
	m_btnCuttingSupportLeftUp.EnableWindow(FALSE);
	m_btnCuttingSupportLeftDown.EnableWindow(FALSE);
	m_btnAttachPressUp.EnableWindow(FALSE);
	m_btnAttachPressDown.EnableWindow(FALSE);
	m_btnAttachReelForward.EnableWindow(FALSE);
	m_btnAttachReelBackward.EnableWindow(FALSE);
	m_btnAttachReelCutterDown.EnableWindow(FALSE);
	m_btnAttachReelCutterUp.EnableWindow(FALSE);

//	m_btnRecoveryMotorOn.EnableWindow(FALSE);
//	m_btnRecoveryMotorOff.EnableWindow(FALSE);
	
	//110630_____
//@	m_btnRecoveryMotorOn.SetCaption(_T("PReel\r\nBreakOn"));
//@	m_btnRecoveryMotorOff.SetCaption(_T("PReel\r\nBreakOff"));
	//___________


	//@________________________________
	m_btnSupplierChuckLock.EnableWindow(FALSE);
	m_btnSupplierChuckUnlock.EnableWindow(FALSE);

	m_btnPReelMotorOn.SetCaption(_T("P Reel Motor\r\nStop"));
	m_btnPReelMotorOff.EnableWindow(FALSE);
	//@________________________________


	CBtnEnh* pTitle = (CBtnEnh*)(GetDlgItem(IDC_MANUAL_TABCARRIER_UD));
	pTitle->SetCaption(_T("Mold Pin UpDown"));
	pTitle = (CBtnEnh*)(GetDlgItem(IDC_MANUAL_TABCARRIER_UD3));
	pTitle->SetCaption(_T("Supply Reel"));

	m_btnReelUp.SetCaption(_T("Mold Pin\r\nUp"));
	m_btnReelDown.SetCaption(_T("Mold Pin\r\nDown"));

	m_btnSupplierTensionSmall.SetCaption(_T("Supplier Reel\r\nServo On"));
	m_btnSupplierTensionMotorOff.SetCaption(_T("Supplier Reel\r\nServo Off"));

	m_BtnCmd[0].EnableWindow(FALSE);



	m_rgbCmdState[DEF_CMD_HOME_SPROCKET1]	= m_plnkCtrlTabFeeder->GetHomeFlagSprocket1();
	m_rgbCmdState[DEF_CMD_HOME_SPROCKET2]	= m_plnkCtrlTabFeeder->GetHomeFlagSprocket2();

	m_BtnCmd[DEF_CMD_HOME_SPROCKET1].SetValue(m_rgbCmdState[DEF_CMD_HOME_SPROCKET1]);
	m_BtnCmd[DEF_CMD_HOME_SPROCKET2].SetValue(m_rgbCmdState[DEF_CMD_HOME_SPROCKET2]);

	m_rgbCmdState[DEF_CMD_REVERSE] = FALSE;

	m_uiTimerID = SetTimer(1, 200, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabFeederDlg_1"), &szTextTemp);
//	금형 제거
	GetDlgItem(IDB_REMOVE_MOLD)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabFeederDlg_2"), &szTextTemp);
//	Sprocket2\r\n강제 Homing
	GetDlgItem(IDB_FORCED_HOMING)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabFeederDlg_3"), &szTextTemp);
//	Reel 제거
	GetDlgItem(IDB_WITHDRAW_REEL)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabFeederDlg_4"), &szTextTemp);
//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManualTabFeederDlg::OnClickPress1VacOn() 
{
	// TODO: Add your control notification handler code here
	CMyProgressWnd ProgWnd(NULL, _T("Press Vac On"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabFeeder is Press Vac On...Wait a moment..."));
	ProgWnd.PeekAndPump();

	int iResult = m_plnkTabFeeder->AbsorbPress();
	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();		
}

void CManualTabFeederDlg::OnClickPress1VacOff() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	CMyProgressWnd ProgWnd(NULL, _T("Press Vac Off"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabFeeder is Press Vac Off...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->ReleasePress()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();		
}

void CManualTabFeederDlg::OnClickNewReelVacOn() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	CMyProgressWnd ProgWnd(NULL, _T("New Reel Vac On"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabFeeder is New Reel Vac On...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->AbsorbNewReel()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();		
}

void CManualTabFeederDlg::OnClickNewReelVacOff() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	CMyProgressWnd ProgWnd(NULL, _T("New Reel Vac Off"));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabFeeder is New Reel Vac Off...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->ReleaseNewReel()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();		
}

void CManualTabFeederDlg::OnClickReelUp() 
{
	int iResult;
	if (MyMessageBox(MY_MESSAGE_BOX, 30649, _T("MoldPin Up/Down Axis Up"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("MoldPin Up/Down Axis..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("MoldPin Axis Up/Down Axis Up...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS)) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Press Reel Up/Down Cylinder Up"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabFeederDlg::OnClickReelDown() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	if (MyMessageBox(MY_MESSAGE_BOX, 30650, _T("MoldPin Up/Down Axis Down"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("MoldPin Up/Down Axis..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("MoldPin Axis Up/Down Axis Down...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_FIX_POS)) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Press Reel Up/Down Cylinder Up"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabFeederDlg::OnClickReelClampUp() 
{
	// TODO: Add your control notification handler code here
}

void CManualTabFeederDlg::OnClickReelClampDown() 
{
	// TODO: Add your control notification handler code here
}

void CManualTabFeederDlg::OnClickSupplierChuckLock() 
{
	// TODO: Add your control notification handler code here
//	Supplier Chuck를 Lock 하시겠습니까?
	int iResult = MyMessageBox(MY_MESSAGE_BOX, 30604, _T("Supplier Chuck Lock/Unlock Cylinder Lock"), M_ICONQUESTION|M_YESNO);
	if (iResult == IDNO)
		return;
	
	CMyProgressWnd ProgWnd(NULL, _T("Supplier Chuck Lock/Unlock Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Supplier Chuck Lock/Unlock Cylinder Lock..Wait a moment..."));
	ProgWnd.PeekAndPump();
	

	iResult = m_plnkTabFeeder->LockSupplierChuckCyl(TRUE);
	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Supplier Chuck Lock/Unlock Cylinder Lock"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabFeederDlg::OnClickSupplierChuckUnlock() 
{
	// TODO: Add your control notification handler code here
//	Supplier Chuck를 Unlock 하시겠습니까?
	int iResult = MyMessageBox(MY_MESSAGE_BOX, 30605, _T("Supplier Chuck Lock/Unlock Cylinder Unlock"), M_ICONQUESTION|M_YESNO);
	if (iResult == IDNO)
		return;
	
	CMyProgressWnd ProgWnd(NULL, _T("Supplier Chuck Lock/Unlock Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Supplier Chuck Lock/Unlock Cylinder Unlock..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	iResult = m_plnkTabFeeder->UnlockSupplierChuckCyl(TRUE);
	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Supplier Chuck Lock/Unlock Cylinder Unlock"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();

	//SJ_YYK 110421 Add... Ellas
	MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eMaterialChangeStart, ePre_Reel);
}

void CManualTabFeederDlg::OnClickCuttingSupportLeftUp() 
{
}

void CManualTabFeederDlg::OnClickCuttingSupportLeftDown() 
{
}

void CManualTabFeederDlg::OnClickCuttingSupportRightUp() 
{
}

void CManualTabFeederDlg::OnClickCuttingSupportRightDown() 
{
}

void CManualTabFeederDlg::OnClickFeederSprocketOpen() 
{
	// TODO: Add your control notification handler code here
	/*
	int iResult;
//	Feeder Sprocket을 Open하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30614, _T("Feeder Sprocket Open/Close Cylinder Open"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Feeder Sprocket Open/Close Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Feeder Sprocket Open/Close Cylinder Open..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->OpenFeederSprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Feeder Sprocket Open/Close Cylinder Open"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
	*/
}

void CManualTabFeederDlg::OnClickFeederSprocketClose() 
{
	// TODO: Add your control notification handler code here
	/*
	int iResult;
//	Feeder Sprocket을 Close하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30615, _T("Feeder Sprocket Open/Close Cylinder Close"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Feeder Sprocket Open/Close Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Feeder Sprocket Open/Close Cylinder Close..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->CloseFeederSprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Feeder Sprocket Open/Close Cylinder Close"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
	*/
}

void CManualTabFeederDlg::OnClickTensionSprocketOpen() 
{
	// TODO: Add your control notification handler code here
	/*
	int iResult;
//	Tension Sprocket을 Open하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30616, _T("Tension Sprocket Open/Close Cylinder Open"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Tension Sprocket Open/Close Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Tension Sprocket Open/Close Cylinder Open ..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->OpenTensionSprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tension Sprocket Open/Close Cylinder Open"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
	*/
}

void CManualTabFeederDlg::OnClickTensionSprocketClose() 
{
	// TODO: Add your control notification handler code here
	/*
	int iResult;
//	Tension Sprocket을 Close하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30617, _T("Tension Sprocket Open/Close Cylinder Close"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Tension Sprocket Open/Close Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Tension Sprocket Open/Close Cylinder Close ..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->CloseTensionSprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tension Sprocket Open/Close Cylinder Close"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
	*/
}

void CManualTabFeederDlg::OnClickRecoverySprocketOpen() 
{
	// TODO: Add your control notification handler code here
	int iResult;
//	Recovery Sprocket을 Open하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30618, _T("Recovery Sprocket Open/Close Cylinder  Open"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Recovery Sprocket Open/Close Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Recovery Sprocket Open/Close Cylinder Open..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->OpenRecoverySprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Recovery Sprocket Open/Close Cylinder  Open"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabFeederDlg::OnClickRecoverySprocketClose() 
{
	// TODO: Add your control notification handler code here
	int iResult;
//	Recovery Sprocket을 Close하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30619, _T("Recovery Sprocket Open/Close Cylinder Close"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Recovery Sprocket Open/Close Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Recovery Sprocket Open/Close Cylinder Close..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->CloseRecoverySprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Recovery Sprocket Open/Close Cylinder Close"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabFeederDlg::OnClickSprocketAllOpen() 
{
	// TODO: Add your control notification handler code here
	/*
	int iResult;
//	All Sprocket을 Open하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30620, _T("All Sprocket Open/Close Cylinder  Open"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("All Sprocket Open/Close Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("All Sprocket Open/Close Cylinder Open..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->OpenRecoverySprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("All Sprocket Open/Close Cylinder  Open"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	if ((iResult = m_plnkTabFeeder->OpenTensionSprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("All Sprocket Open/Close Cylinder  Open"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	
	if ((iResult = m_plnkTabFeeder->OpenFeederSprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("All Sprocket Open/Close Cylinder  Open"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	*/
}

void CManualTabFeederDlg::OnClickSprocketAllClose() 
{
	// TODO: Add your control notification handler code here
	/*
	int iResult;
//	All Sprocket을 Close하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30621, _T("All Sprocket Open/Close Cylinder Close"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("All Sprocket Open/Close Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("All Sprocket Open/Close Cylinder Close..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->CloseRecoverySprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("All Sprocket Open/Close Cylinder Close"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	if ((iResult = m_plnkTabFeeder->CloseTensionSprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("All Sprocket Open/Close Cylinder Close"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	if ((iResult = m_plnkTabFeeder->CloseFeederSprocketCyl()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("All Sprocket Open/Close Cylinder Close"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	*/
}

void CManualTabFeederDlg::OnClickAttachPressUp() 
{
	// TODO: Add your control notification handler code here
}

void CManualTabFeederDlg::OnClickAttachPressDown() 
{
	// TODO: Add your control notification handler code here
}

void CManualTabFeederDlg::OnClickAttachReelCutterOpen() 
{

}

void CManualTabFeederDlg::OnClickAttachReelCutterCutting() 
{
	// TODO: Add your control notification handler code here
}

void CManualTabFeederDlg::OnClickAttachReelForward() 
{
	// TODO: Add your control notification handler code here
}

void CManualTabFeederDlg::OnClickAttachReelBackward() 
{
	// TODO: Add your control notification handler code here
}

void CManualTabFeederDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(FALSE);//110209.Add
	//110630_______
	m_plnkTabFeeder->PReelBreakOnOff(TRUE);
	//_____________
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CManualTabFeederDlg::OnCancel();	
}

void CManualTabFeederDlg::OnClickSprocket1Home() 
{
	return;

	if (TRUE == m_plnkCtrlTabFeeder->GetHomeFlagSprocket1())
		return;

//	Sprocket1 Home잡는 동작을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30628, _T("Recovery Motor On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	if (m_plnkTabFeeder->CheckBadTabOrigin()
		|| m_plnkTabFeeder->CheckPressOrigin()
		|| m_plnkTabFeeder->CheckSprocket2Origin())
	{
//		Press, Badtab Detector, Sprocket이 모두 원점복귀되어야 합니다.
		MyMessageBox(MY_MESSAGE_BOX,30629, _T("confirm"), M_OK);
		return;
	}

	CMyProgressWnd ProgWnd(NULL, _T("Sprocket1 Homing..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Sprocket1 Homing.. Wait a moment..."));
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	int iResult = m_plnkCtrlTabFeeder->ExecuteSprocket1Homing();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
}

void CManualTabFeederDlg::OnClickSprocket2Home() 
{
//	Sprocket2 Home 잡는 동작을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30630, _T("Recovery Motor On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	if (TRUE == m_plnkCtrlTabFeeder->GetFindTabICFlag())
	{
		// 강제 TabIC 찾기.
//		이미 Home 동작이 완료 되었습니다. 다시, 강제 Sprocket2 Home 잡는 동작을 진행하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX,30631, _T("Sprocket2 Homing"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;
	}

	m_plnkCtrlTabFeeder->SetFindTabICFlag(FALSE);

	int iResult = m_plnkCtrlTabFeeder->ExecuteAutoHoming();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	//SJ_YYK 110421 Add... Ellas
	MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eMaterialChangeEnd, ePre_Reel);
}

void CManualTabFeederDlg::OnClickOnetab() 
{
	if (MyMessageBox("Do you want to rotate Sproket 1Tab !?\r\nSproket을 1Tab 회전 하시겠습니까!?", "Sproket Dir", M_ICONQUESTION|M_YESNO) == IDNO) 
		return;
	int iCmd = m_rgbCmdState[DEF_CMD_REVERSE] ? -2 : 2;

	if (iCmd > 0)
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_FIND_DOWN);
	else
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_ESCAPE_DOWN);
	m_plnkCtrlTabFeeder->SetRunTension(TRUE);

	int iResult = m_plnkCtrlTabFeeder->FeedTabIC(iCmd);
	if (iResult)
	{
		m_plnkCtrlTabFeeder->SetRunTension(FALSE);
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	m_plnkCtrlTabFeeder->SetRunTension(FALSE);
	m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
}

void CManualTabFeederDlg::OnClickOnePitch() 
{
	if (MyMessageBox("Do you want to rotate Sproket 1Pitch !?\r\nSproket을 1Pitch 회전 하시겠습니까!?", "Sproket Dir", M_ICONQUESTION|M_YESNO) == IDNO) 
		return;
	// hongju 한 pitch씩 이동하면 나중에 TabIC의 위치는 새로 찾아야 한다. Sprocket2만..
	m_plnkCtrlTabFeeder->SetFindTabICFlag(FALSE);

	int iCmd = m_rgbCmdState[DEF_CMD_REVERSE] ? -1 : 1;
	int iResult = 0;

	/*
	if (iCmd > 0)
		iResult = m_plnkCtrlTabFeeder->FindWeightDownSensor();
	else
		iResult = m_plnkCtrlTabFeeder->EscapeWeightDownSensor();

	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	*/
	if (iCmd > 0)
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_FIND_DOWN);
	else
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_ESCAPE_DOWN);

	m_plnkCtrlTabFeeder->SetRunTension(TRUE);

	iResult = m_plnkCtrlTabFeeder->FeedTabIC(iCmd);
	if (iResult)
	{
		m_plnkCtrlTabFeeder->SetRunTension(FALSE);
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"), M_ICONERROR);
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	m_plnkCtrlTabFeeder->SetRunTension(FALSE);
	m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
}

void CManualTabFeederDlg::OnClickTentab() 
{
	if (MyMessageBox("Do you want to spin Sproket 10Tab !?\r\nSproket을 10Tab 회전 하시겠습니까!?", "Sproket Dir", M_ICONQUESTION|M_YESNO) == IDNO) 
		return;
	int iCmd = m_rgbCmdState[DEF_CMD_REVERSE] ? -2 : 2;

	if (iCmd > 0)
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_FIND_DOWN);
	else
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_ESCAPE_DOWN);

	m_plnkCtrlTabFeeder->SetRunTension(TRUE);
	for (int i = 0; i < 10; i++)
	{
		int iResult = m_plnkCtrlTabFeeder->FeedTabIC(iCmd);
		if (iResult)
		{
			m_plnkCtrlTabFeeder->SetRunTension(FALSE);
			m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		if (TRUE == m_plnkOpPanel->GetStopButtonStatus())
			break;
	}
	m_plnkCtrlTabFeeder->SetRunTension(FALSE);
	m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);

}

void CManualTabFeederDlg::OnClickTenPitch() 
{
	if (MyMessageBox("Do you want to spin Sproket 10Pitch !?\r\nSproket을 10Pitch 회전 하시겠습니까!?", "Sproket Dir", M_ICONQUESTION|M_YESNO) == IDNO) 
		return;
	// hongju 한 pitch씩 이동하면 나중에 TabIC의 위치는 새로 찾아야 한다. Sprocket2만..
	m_plnkCtrlTabFeeder->SetFindTabICFlag(FALSE);

	int iCmd = m_rgbCmdState[DEF_CMD_REVERSE] ? -1 : 1;

	if (iCmd > 0)
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_FIND_DOWN);
	else
		m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_ESCAPE_DOWN);

	m_plnkCtrlTabFeeder->SetRunTension(TRUE);
	for (int i = 0; i < 10; i++)
	{
		int iResult = m_plnkCtrlTabFeeder->FeedTabIC(iCmd);
		if (iResult)
		{
			m_plnkCtrlTabFeeder->SetRunTension(FALSE);
			m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		if (TRUE == m_plnkOpPanel->GetStopButtonStatus())
			break;
	}
	m_plnkCtrlTabFeeder->SetRunTension(FALSE);

	m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);

}

void CManualTabFeederDlg::OnClickFeedingReverse() 
{
	if (m_rgbCmdState[DEF_CMD_REVERSE])	
		m_rgbCmdState[DEF_CMD_REVERSE] = FALSE;
	else
		m_rgbCmdState[DEF_CMD_REVERSE] = TRUE;
	
	m_BtnCmd[DEF_CMD_REVERSE].SetValue(m_rgbCmdState[DEF_CMD_REVERSE]);
}

void CManualTabFeederDlg::OnClickRecoveryMotorOn() 
{
//	회수부 Motor를 동작 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30632, _T("Recovery Motor On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->RecoveryIndMotorOn();
	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Recovery Motor On"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

}

void CManualTabFeederDlg::OnClickRecoveryMotorOff() 
{
//	Recovery Motor를 정지 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30633, _T("Recovery Motor Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->RecoveryIndMotorOff();
	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Recovery Motor Off"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

//	Recovery Motor를 정지했습니다.
	MyMessageBox(MY_MESSAGE_BOX,30634, _T("Recovery Motor off confirm"), M_ICONINFORMATION);	

}

void CManualTabFeederDlg::OnClickPReelMotorOn() 
{
	/*/
//	Protect Reel Motor를 동작 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30635, _T("Recovery Motor On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	// JDY
	m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(TRUE);
	int iResult = SUCCESS;
//	int iResult = m_plnkTabFeeder->PReelReverseMotorOff();
//	if (iResult != SUCCESS)
//	{
//		// JDY
//		m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(FALSE);
//		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
//		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
//	}

	iResult = m_plnkTabFeeder->PReelMotorOn();
	if (iResult != SUCCESS)
	{
		// JDY
		m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(FALSE);

// jdy sesl		MyMessageBox(iResult,_T("Recovery Motor On"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	/*/
	m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(FALSE);

	int iResult = m_plnkTabFeeder->PReelMotorStop();

}

/*
void CManualTabFeederDlg::OnClickPReelReverseMotorOn() 
{
//	Protect Reel Reverse Motor를 동작 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30636, _T("Recovery Motor On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	// JDY
	m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(TRUE);

	int iResult = m_plnkTabFeeder->PReelMotorOn();
	if (iResult != SUCCESS)
	{
		// JDY
		m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(FALSE);

// jdy sesl		MyMessageBox(iResult,_T("Recovery Motor Off"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	iResult = m_plnkTabFeeder->PReelReverseMotorOn();
	if (iResult != SUCCESS)
	{
		// JDY
		m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(FALSE);

// jdy sesl		MyMessageBox(iResult,_T("Recovery Motor On"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}
*/

void CManualTabFeederDlg::OnClickPReelMotorOff() 
{
//	Protect Reel Motor를 정지 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30637, _T("Recovery Motor Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	// JDY
	m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(FALSE);

	/* 불필요.
	int iResult = m_plnkTabFeeder->PReelReverseMotorOff();
	if (iResult != SUCCESS)
	{

		MyMessageBox(iResult,_T("Recovery Motor Off"), M_ICONERROR);
	}

	iResult = m_plnkTabFeeder->PReelMotorOff();
	if (iResult != SUCCESS)
	{

		MyMessageBox(iResult,_T("Recovery Motor Off"), M_ICONERROR);
	}
	*/
}

void CManualTabFeederDlg::OnClickSupplierTensionMotorOff() 
{
	if (MyMessageBox(MY_MESSAGE_BOX,30652, _T("Supplier Servo Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->ServoOnOffFeederReel(FALSE);
	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabFeederDlg::OnClickSupplierTensionSmall() 
{
	if (MyMessageBox(MY_MESSAGE_BOX,30651, _T("Supplier Servo On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->ServoOnOffFeederReel(TRUE);
	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabFeederDlg::OnClickBrushMotorOn() 
{
//	Brush Motor를 동작 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30640, _T("Brush Motor On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->BrushMotorOn();
	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Brush Motor On"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabFeederDlg::OnClickBrushMotorOff() 
{
//	Brush Motor를 정지 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30641, _T("Brush Motor Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->BrushMotorOff();
	if (iResult != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Brush Motor Off"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabFeederDlg::OnClickPressTabic() 
{
	CString strMsg;
	if (TRUE == m_plnkTabFeeder->IsAbsorbPress())
	{
//		TabIC 를 흡착하고 있습니다.
		MyMessageBox(MY_MESSAGE_BOX,30642, _T("Error"), M_ICONQUESTION|M_YESNO);
		return;
	}

//	금형 Press 작업을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30643, _T("Press TabIC"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	int iResult = m_plnkCtrlTabFeeder->FeedTabIC();	// TabIC Feeding
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
	iResult = m_plnkCtrlTabFeeder->PressTapeStep1();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkTabFeeder->ReleasePress();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
	iResult = m_plnkTabFeeder->SafeMovePressPos(DEF_PRESS_READY_POS);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
}

void CManualTabFeederDlg::OnClickForcedHoming()
{
	// 강제 TabIC 찾기.
//	강제 Sprocket2 Home 잡는 동작을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30644, _T("Sprocket2 Homing"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkCtrlTabFeeder->SetFindTabICFlag(FALSE);

	int iResult = m_plnkCtrlTabFeeder->ExecuteAutoHoming();
	if (iResult)
	{
		/* jdy sesl
		CString strMsg;

		MyMessageBox(iResult,_T("Tab Feeder"), M_ICONERROR);
		*/
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
}

int CManualTabFeederDlg::freeWorkState()
{
	int iResult = m_plnkTabFeeder->CheckPressOrigin();
	if (iResult)
		return iResult;
	m_plnkTabFeeder->ServoOnOffFeederReel(FALSE);
	m_plnkTabFeeder->RecoveryIndMotorOff();

	iResult = m_plnkTabFeeder->OpenRecoverySprocketCyl();
	if (iResult)
		return iResult;

	iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
	if (iResult)
		return iResult;

	iResult = m_plnkTabFeeder->SafeMovePressPos(DEF_PRESS_UP_POS);
	if (iResult)
		return iResult;

	return SUCCESS;
}

void CManualTabFeederDlg::OnRemoveMold() 
{
//	금형 제거 작업을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30645, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = freeWorkState();
	if (iResult)
	{
		/* jdy sels
		CString strMsg;

		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		*/
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	금형 제거 준비 완료.
	MyMessageBox(MY_MESSAGE_BOX,30646, _T("Information"), M_ICONINFORMATION);
}

void CManualTabFeederDlg::OnWithdrawReel()
{
	CString strMsg;

	if (TRUE == m_bOnWithdrawingReel)
	{
		m_plnkTabFeeder->VStopSprocket1();
		m_plnkTabFeeder->VStopSprocket2();
		m_bOnWithdrawingReel = FALSE;

		int iResult = m_plnkTabFeeder->VStopSprocket2();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult,_T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = m_plnkTabFeeder->VStopSprocket1();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult,_T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

//		Reel 제거를 위한 일련 동작을 수행하시겠습니까?
		iResult = MyMessageBox(MY_MESSAGE_BOX,30647, _T("Question"), M_ICONQUESTION|M_YESNO);
		if (IDNO == iResult)
			return;

		iResult = freeWorkState();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = m_plnkTabFeeder->UnlockSupplierChuckCyl();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
	else
	{
//		Reel 제거 작업을 진행하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX,30648, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		{
			m_btnWithdrawReel.SetValue(FALSE);
			return;
		}

		// Tension, Cylinder 위치, Press 금형 위치 확인을 위한 -1 Pitch 이동 동작.
		int iResult = m_plnkCtrlTabFeeder->FeedTabIC(-1);	// -1 Pitch 이동
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = m_plnkTabFeeder->VMoveSprocket1(0.0, FALSE);
		if (iResult)
		{
			m_plnkTabFeeder->VStopSprocket1();

// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = m_plnkTabFeeder->VMoveSprocket2(0.0, FALSE);
		if (iResult)
		{
			m_plnkTabFeeder->VStopSprocket1();
			m_plnkTabFeeder->VStopSprocket2();

// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		m_bOnWithdrawingReel = TRUE;
	}	
}

void CManualTabFeederDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CManualTabFeederDlg::OnClickSprocket1ClutchOn() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox(MY_MESSAGE_BOX,30653, _T("Sprocket Clutch On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->ClutchOnSprocket1Axis();
	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	
}

void CManualTabFeederDlg::OnClickSprocket1ClutchOff() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox(MY_MESSAGE_BOX,30654, _T("Sprocket Clutch Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->ClutchOffSprocket1Axis();
	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	
}
