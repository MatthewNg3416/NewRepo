// MaterialChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MaterialChangeDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTabFeeder.h"
#include "MTabCarrier.h"
#include "MCtrlTabFeeder.h"
#include "IAxis.h"
#include "IOpPanel.h"
#include "MPreBonderData.h"
#include "DefIOAddrPreBonder.h"
#include "MTrsAutoManager.h"
#include "IIO.h"
#include "MManageOpPanel.h"

#include "MSystemData.h"
#include "MCtrlTabFeeder.h"
#include "MTabCarrier.h"
#include "MCtrlTabCarrier.h"
#include <math.h>
#include "VisionMonitorDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaterialChangeDlg dialog
extern	MPlatformOlbSystem	MOlbSystemPre; 

CMaterialChangeDlg::CMaterialChangeDlg(int iGroupNo, CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaterialChangeDlg)
		// NOTE: the ClassWizard will add member initialization here
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

	m_plnkSystemData = MOlbSystemPre.GetSystemData();

	m_bOnWithdrawingReel = FALSE;

    iCutTestRealCount	= 0;
    iCutTestSetCount	= 0;
}


void CMaterialChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaterialChangeDlg)
	DDX_Control(pDX, IDB_FEEDER_SPROCKET_UP,			m_btnFeederSprocketUp);
	DDX_Control(pDX, IDB_FEEDER_SPROCKET_DOWN,			m_btnFeederSprocketDown);
	DDX_Control(pDX, IDB_TENSION_SPROCKET_DOWN,			m_btnTensionSprocketDown);
	DDX_Control(pDX, IDB_TENSION_SPROCKET_UP,			m_btnTensionSprocketUp);
	DDX_Control(pDX, IDB_RECOVERY_SPROCKET_DOWN,		m_btnRecoverySprocketDown);
	DDX_Control(pDX, IDB_RECOVERY_SPROCKET_UP,			m_btnRecoverySprocketUp);
	DDX_Control(pDX, IDB_SPROCKET_ALL_DOWN,				m_btnSprocketAllDown);
	DDX_Control(pDX, IDB_SPROCKET_ALL_UP,				m_btnSprocketAllUp);

	DDX_Control(pDX, IDB_SUPPLIER_CHUCK_LOCK,			m_btnSupplierChuckLock);
	DDX_Control(pDX, IDB_SUPPLIER_CHUCK_UNLOCK,			m_btnSupplierChuckUnlock);

	DDX_Control(pDX, IDB_SUPPLIER_TENSION_SMALL,		m_btnSupplierTensionSmall);	
	DDX_Control(pDX, IDB_SUPPLIER_TENSION_MOTOR_OFF,	m_btnSupplierTensionMotorOff);
	DDX_Control(pDX, IDB_RECOVERY_MOTOR_ON,				m_btnRecoveryMotorOn);
	DDX_Control(pDX, IDB_RECOVERY_MOTOR_OFF,			m_btnRecoveryMotorOff);
	DDX_Control(pDX, IDB_P_REEL_MOTOR_ON,				m_btnPReelMotorOn);
	DDX_Control(pDX, IDB_P_REEL_MOTOR_OFF,				m_btnPReelMotorOff);
	DDX_Control(pDX, IDB_P_REEL_REVERSE_MOTOR_ON,		m_btnPReelReverseMotorOn);
	DDX_Control(pDX, IDB_BRUSH_MOTOR_ON,				m_btnBrushMotorOn);
	DDX_Control(pDX, IDB_BRUSH_MOTOR_OFF,				m_btnBrushMotorOff);

	DDX_Control(pDX, IDB_REEL_DOWN,						m_btnReelDown);
	DDX_Control(pDX, IDB_REEL_UP,						m_btnReelUp);
	DDX_Control(pDX, IDB_REEL_CLAMP_DOWN,				m_btnReelClampDown);
	DDX_Control(pDX, IDB_REEL_CLAMP_UP,					m_btnReelClampUp);
	
	DDX_Control(pDX, IDB_SPROCKET1_CLUTCH_ON,			m_btnSprocketClutchOn);
	DDX_Control(pDX, IDB_SPROCKET1_CLUTCH_OFF,			m_btnSprocketClutchOff);

	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_HOME_SPROCKET1,				m_BtnCmd[0]);
	DDX_Control(pDX, IDB_HOME_SPROCKET2,				m_BtnCmd[1]);
	DDX_Control(pDX, IDB_FEEDING_REVERSE,				m_BtnCmd[2]);

	DDX_Control(pDX, IDC_TENSION_OFFSET_CHANGE, m_sTensionOffset);
}


BEGIN_MESSAGE_MAP(CMaterialChangeDlg, CDialog)
	//{{AFX_MSG_MAP(CMaterialChangeDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaterialChangeDlg message handlers

BEGIN_EVENTSINK_MAP(CMaterialChangeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMaterialChangeDlg)
	ON_EVENT(CMaterialChangeDlg, IDB_TENSION_SPROCKET_UP, -600 /* Click */, OnClickTensionSprocketUp, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_TENSION_SPROCKET_DOWN, -600 /* Click */, OnClickTensionSprocketDown, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_FEEDER_SPROCKET_UP, -600 /* Click */, OnClickFeederSprocketUp, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_FEEDER_SPROCKET_DOWN, -600 /* Click */, OnClickFeederSprocketDown, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_RECOVERY_SPROCKET_UP, -600 /* Click */, OnClickRecoverySprocketUp, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_RECOVERY_SPROCKET_DOWN, -600 /* Click */, OnClickRecoverySprocketDown, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_SPROCKET_ALL_UP, -600 /* Click */, OnClickSprocketAllUp, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_SPROCKET_ALL_DOWN, -600 /* Click */, OnClickSprocketAllDown, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_SUPPLIER_CHUCK_LOCK, -600 /* Click */, OnClickSupplierChuckLock, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_SUPPLIER_CHUCK_UNLOCK, -600 /* Click */, OnClickSupplierChuckUnlock, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_SUPPLIER_TENSION_SMALL, -600 /* Click */, OnClickSupplierTensionSmall, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_SUPPLIER_TENSION_MOTOR_OFF, -600 /* Click */, OnClickSupplierTensionMotorOff, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_RECOVERY_MOTOR_ON, -600 /* Click */, OnClickRecoveryMotorOn, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_RECOVERY_MOTOR_OFF, -600 /* Click */, OnClickRecoveryMotorOff, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_P_REEL_MOTOR_ON, -600 /* Click */, OnClickPReelMotorOn, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_P_REEL_MOTOR_OFF, -600 /* Click */, OnClickPReelMotorOff, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_P_REEL_REVERSE_MOTOR_ON, -600 /* Click */, OnClickPReelReverseMotorOn, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_BRUSH_MOTOR_ON, -600 /* Click */, OnClickBrushMotorOn, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_BRUSH_MOTOR_OFF, -600 /* Click */, OnClickBrushMotorOff, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_HOME_SPROCKET1, -600 /* Click */, OnClickHomeSprocket1, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_HOME_SPROCKET2, -600 /* Click */, OnClickHomeSprocket2, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_FORCED_HOMING, -600 /* Click */, OnClickForcedHoming, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_ONETAB, -600 /* Click */, OnClickOnetab, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_ONE_PITCH, -600 /* Click */, OnClickOnePitch, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_TENTAB, -600 /* Click */, OnClickTentab, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_TEN_PITCH, -600 /* Click */, OnClickTenPitch, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_FEEDING_REVERSE, -600 /* Click */, OnClickFeedingReverse, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_REEL_UP, -600 /* Click */, OnClickReelUp, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_REEL_DOWN, -600 /* Click */, OnClickReelDown, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_REEL_CLAMP_UP, -600 /* Click */, OnClickReelClampUp, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_REEL_CLAMP_DOWN, -600 /* Click */, OnClickReelClampDown, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_SPROCKET1_CLUTCH_ON, -600 /* Click */, OnClickSprocket1ClutchOn, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_SPROCKET1_CLUTCH_OFF, -600 /* Click */, OnClickSprocket1ClutchOff, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_TABIC_LOAD_CARRIER_CHANGE, -600 /* Click */, OnClickTabicLoadCarrierChange, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_TAB_CUTTING_OFFSET_CHANGE, -600 /* Click */, OnClickTabCuttingOffsetChange, VTS_NONE)
	ON_EVENT(CMaterialChangeDlg, IDB_REGISTER_MARK_CHANGE, -600 /* Click */, OnClickRegisterMarkChange, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMaterialChangeDlg::OnClickTensionSprocketUp() 
{
	// TODO: Add your control notification handler code here
	
}

void CMaterialChangeDlg::OnClickTensionSprocketDown() 
{
	// TODO: Add your control notification handler code here
	
}

void CMaterialChangeDlg::OnClickFeederSprocketUp() 
{
	/*
	// TODO: Add your control notification handler code here
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

void CMaterialChangeDlg::OnClickFeederSprocketDown() 
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

void CMaterialChangeDlg::OnClickRecoverySprocketUp() 
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

void CMaterialChangeDlg::OnClickRecoverySprocketDown() 
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

void CMaterialChangeDlg::OnClickSprocketAllUp() 
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

void CMaterialChangeDlg::OnClickSprocketAllDown() 
{
	// TODO: Add your control notification handler code here
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

void CMaterialChangeDlg::OnClickSupplierChuckLock() 
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

void CMaterialChangeDlg::OnClickSupplierChuckUnlock() 
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

	if(MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
	{
		MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eMaterialChangeStart, ePre_Reel);
	}
	else	MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eMaterialChangeStart, ePre_Reel_Auto);
}

void CMaterialChangeDlg::OnClickSupplierTensionSmall() 
{
	// TODO: Add your control notification handler code here

	if (MyMessageBox(MY_MESSAGE_BOX,30651, _T("Supplier Servo On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->ServoOnOffFeederReel(TRUE);
	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CMaterialChangeDlg::OnClickSupplierTensionMotorOff() 
{
	// TODO: Add your control notification handler code here
	//	Supplier Tension Motor를 정지 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,30652, _T("Supplier Servo Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->ServoOnOffFeederReel(FALSE);
	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CMaterialChangeDlg::OnClickRecoveryMotorOn() 
{
	// TODO: Add your control notification handler code here
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

void CMaterialChangeDlg::OnClickRecoveryMotorOff() 
{
	// TODO: Add your control notification handler code here
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

void CMaterialChangeDlg::OnClickPReelMotorOn() 
{
	// TODO: Add your control notification handler code here
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

void CMaterialChangeDlg::OnClickPReelMotorOff() 
{
	// TODO: Add your control notification handler code here
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

void CMaterialChangeDlg::OnClickPReelReverseMotorOn() 
{
	// TODO: Add your control notification handler code here
	
}

void CMaterialChangeDlg::OnClickBrushMotorOn() 
{
	// TODO: Add your control notification handler code here
	
}

void CMaterialChangeDlg::OnClickBrushMotorOff() 
{
	// TODO: Add your control notification handler code here
	
}

void CMaterialChangeDlg::OnClickHomeSprocket1() 
{
	// TODO: Add your control notification handler code here
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

void CMaterialChangeDlg::OnClickHomeSprocket2() 
{
	// TODO: Add your control notification handler code here
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

	if(MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
		MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eMaterialChangeEnd, ePre_Reel);
	else
		MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eMaterialChangeEnd, ePre_Reel_Auto);

	MyMessageBox(_T("You have completed the Sprocket Homing operation.\r\nSprocket Homing동작을 완료하였습니다."), _T("Confirm"));
}

void CMaterialChangeDlg::OnClickForcedHoming() 
{
	// TODO: Add your control notification handler code here
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
	MyMessageBox(_T("You have completed the Sprocket Homing operation.\r\nSprocket Homing동작을 완료하였습니다."), _T("Confirm"));
}

void CMaterialChangeDlg::OnClickOnetab() 
{
	// TODO: Add your control notification handler code here
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
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	m_plnkCtrlTabFeeder->SetRunTension(FALSE);

	m_plnkCtrlTabFeeder->SetFeederReelMoveType(E_FEEDER_REEL_MOVE_NONE);
}

void CMaterialChangeDlg::OnClickOnePitch() 
{
	// TODO: Add your control notification handler code here
	// hongju 한 pitch씩 이동하면 나중에 TabIC의 위치는 새로 찾아야 한다. Sprocket2만..
	if (MyMessageBox("Do you want to rotate Sproket 1Pitch !?\r\nSproket을 1Pitch 회전 하시겠습니까!?", "Sproket Dir", M_ICONQUESTION|M_YESNO) == IDNO) 
		return;
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
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	m_plnkCtrlTabFeeder->SetRunTension(FALSE);
}

void CMaterialChangeDlg::OnClickTentab() 
{
	// TODO: Add your control notification handler code here
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

void CMaterialChangeDlg::OnClickTenPitch() 
{
	// TODO: Add your control notification handler code here
	// hongju 한 pitch씩 이동하면 나중에 TabIC의 위치는 새로 찾아야 한다. Sprocket2만..
	if (MyMessageBox("Do you want to spin Sproket 10Pitch !?\r\nSproket을 10Pitch 회전 하시겠습니까!?", "Sproket Dir", M_ICONQUESTION|M_YESNO) == IDNO) 
		return;
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

void CMaterialChangeDlg::OnClickFeedingReverse() 
{
	// TODO: Add your control notification handler code here
	if (m_rgbCmdState[DEF_CMD_REVERSE])	
		m_rgbCmdState[DEF_CMD_REVERSE] = FALSE;
	else
		m_rgbCmdState[DEF_CMD_REVERSE] = TRUE;
	
	m_BtnCmd[DEF_CMD_REVERSE].SetValue(m_rgbCmdState[DEF_CMD_REVERSE]);	
}

void CMaterialChangeDlg::OnClickExit()
{
	// TODO: Add your control notification handler code here
	m_plnkCtrlTabFeeder->SetPReelTensionManualOperation(FALSE);//110209.Add

	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	/*/
	//SJ_YYK 101124 Auto Run 중 Door Lock Control...
	if(MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
	{
		if(m_iGroupNo == DEF_FRONT_GROUP)
		{
			MOlbSystemPre.GetIOComponent()->OutputOff(OUT_FRONT_DOOR_LOCK_OPEN); //101124
		}
		else
		{
			MOlbSystemPre.GetIOComponent()->OutputOff(OUT_REAR_DOOR_LOCK_OPEN); //101124
		}
	}
	/*/

	Sleep(500);

	MOlbSystemPre.GetTrsAutoManagerComponent()->SetMaterialDoorCheck(FALSE, m_iGroupNo); // Doork Check 무효화
	MOlbSystemPre.GetTrsAutoManagerComponent()->m_bFeederChange = FALSE;
	//_________________________________________*/

	//120116.kms_________
	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		m_plnkCtrlTabFeeder->SetFeederReady4DryRun(TRUE);
		MOlbSystemPre.GetTrsAutoManagerComponent()->SetMountAutoChange(TRUE);
	}

	CMaterialChangeDlg::OnCancel();
}

BOOL CMaterialChangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_iGroupNo == DEF_FRONT_GROUP)
		SetWindowText(_T("Front Tab Feeder Manual Dialog"));
	else	// if (m_iGroupNo == DEF_REAR_GROUP)
		SetWindowText(_T("Rear Tab Feeder Manual Dialog"));

	m_btnReelClampUp.EnableWindow(FALSE);
	m_btnReelClampDown.EnableWindow(FALSE);

//@	m_btnRecoveryMotorOn.EnableWindow(FALSE);
//@	m_btnRecoveryMotorOff.EnableWindow(FALSE);

//	m_btnPReelMotorOn.EnableWindow(FALSE);
//	m_btnPReelMotorOff.EnableWindow(FALSE);

	CBtnEnh* pTitle = (CBtnEnh*)(GetDlgItem(IDC_MANUAL_TABCARRIER_UD));
	pTitle->SetCaption(_T("Mold Pin UpDown"));
	pTitle = (CBtnEnh*)(GetDlgItem(IDC_MANUAL_TABCARRIER_UD3));
	pTitle->SetCaption(_T("Supply Reel"));

	m_btnReelUp.SetCaption(_T("Mold Pin\r\nUp"));
	m_btnReelDown.SetCaption(_T("Mold Pin\r\nDown"));

	m_btnSupplierTensionSmall.SetCaption(_T("Supplier Reel\r\nServo On"));
	m_btnSupplierTensionMotorOff.SetCaption(_T("Supplier Reel\r\nServo Off"));

	m_BtnCmd[0].EnableWindow(FALSE);

	//@_________________________
	m_btnSupplierChuckLock.EnableWindow(FALSE);
	m_btnSupplierChuckUnlock.EnableWindow(FALSE);
	m_btnPReelMotorOn.SetCaption(_T("P Reel Motor\r\nStop"));
	m_btnPReelMotorOff.EnableWindow(FALSE);
	//@_________________________


	getUserOffsetX_withDisplay();

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
	
//	Sprocket2\r\n강제 Homing
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabFeederDlg_2"), &szTextTemp);
	GetDlgItem(IDB_FORCED_HOMING)->SetWindowText(szTextTemp);
//	나가기
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabFeederDlg_4"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMaterialChangeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	// Do not call CDialog::OnPaint() for painting messages
}

void CMaterialChangeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL	bStatus = FALSE;
	BOOL	bOldStatus = FALSE;

	if (m_uiTimerID == nIDEvent)
	{
		/*/
		if (DEF_FRONT_GROUP == m_iGroupNo)
		{
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

		/*/
		// Recovery Motor Off Status Check
//		bStatus = m_plnkTabFeeder->IsPReelMotorOff() && m_plnkTabFeeder->IsPReelReverseMotorOff();
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
		//____________________________________________*/
	}	
	CDialog::OnTimer(nIDEvent);
}


void CMaterialChangeDlg::OnClickReelUp() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	if (MyMessageBox(MY_MESSAGE_BOX, 30649, _T("MoldPin Up/Down Axis Up"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("MoldPin Up/Down Axis..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("MoldPin Axis Up/Down Axis Up...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_FIX_POS)) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Press Reel Up/Down Cylinder Up"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CMaterialChangeDlg::OnClickReelDown() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	if (MyMessageBox(MY_MESSAGE_BOX, 30650, _T("MoldPin Up/Down Axis Down"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("MoldPin Up/Down Axis..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("MoldPin Axis Up/Down Axis Down...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS)) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Press Reel Up/Down Cylinder Up"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CMaterialChangeDlg::OnClickReelClampUp() 
{
	// TODO: Add your control notification handler code here

}

void CMaterialChangeDlg::OnClickReelClampDown() 
{
	// TODO: Add your control notification handler code here
	
}

void CMaterialChangeDlg::OnClickSprocket1ClutchOn() 
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

void CMaterialChangeDlg::OnClickSprocket1ClutchOff() 
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

void CMaterialChangeDlg::OnClickTabicLoadCarrierChange() 
{
	// TODO: Add your control notification handler code here
	//20100821 sj_ysb
	CString strMsg;
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
//		Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
		MyMessageBox(MY_MESSAGE_BOX, 30376, _T("Error"), M_ICONERROR);
		return;
	}

//	타발 정도 확인 작업을 진행하시겠습니까?
	
	EWorkingSide eWorkSide = WORKING_SIDE_UNKNOWN;
	int iWorkGroupNo = DEF_FRONT_GROUP;
	int iTabNo = -1;
	int iCarrierNo = -1;
	int i = 0;
	int j = 0;
	WCHAR m_cbuf[100];
	WCHAR m_cBufGet[100];
	memset(m_cBufGet, 0, sizeof(m_cBufGet));
	swprintf(m_cbuf,_T("%d"), 4);

	MTabCarrier* m_plnkWorkTabCarrier[4];
	MCtrlTabFeeder* m_plnkCtrlWorkTabFeeder;
	MCtrlTabCarrier* m_plnkCtrlWorkTabCarrier[4];

	int iResult = MyMessageBox(MY_MESSAGE_BOX, 30389, _T("confirm"), M_ICONQUESTION|M_YESNO);
	if (iResult != IDYES)
		return;

//	iResult = MyMessageBox(_T("Tab IC를 어떤 Feeder 에서 타발 하시겠습니까?"), _T("confirm"), M_ICONQUESTION|M_YESNO, _T("Front"), _T("Rear"));
//	if (IDYES == iResult)
	if (DEF_FRONT_GROUP == m_iGroupNo)
	{
		for(i = 0 ; i< 4; i++)
		{
			m_plnkWorkTabCarrier[i] = MOlbSystemPre.GetTabCarrierComponent(i);
			m_plnkCtrlWorkTabCarrier[i] = MOlbSystemPre.GetCtrlTabCarrierComponent(i);
		}

		m_plnkCtrlWorkTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(0);
	}
	else    //(DEF_REAR_GROUP == m_iGroupNo)
	{
		for(i = 0 ; i< 4; i++)
		{
			m_plnkWorkTabCarrier[i] = MOlbSystemPre.GetRTabCarrierComponent(i);
			m_plnkCtrlWorkTabCarrier[i] = MOlbSystemPre.GetCtrlRTabCarrierComponent(i);
		}

		m_plnkCtrlWorkTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(1);
	}

	MyMessageBox(_T("INPUT CARRIERNUMBER"),_T("INPUT"),M_ICONINFORMATION);
	if (GetNumberBox(m_cBufGet, 10, m_cbuf))
	{
		iCarrierNo = _wtoi(m_cBufGet);
	}
	iCarrierNo--;
	if(iCarrierNo < 0 || iCarrierNo > 3 )
	{
		MyMessageBox(_T("WRONG NUMBER"),_T("INPUT"),M_ICONINFORMATION);
		return;
	}
	
//		일련 동작 중 입니다.\n잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_29"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabCarrier Prealign TabIC..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();
	for (i = 0; i<iCarrierNo; i++)
	{			
		iResult = m_plnkWorkTabCarrier[i]->SafeMoveXYTPos(DEF_TABCARRIER_READY_POS);			
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);				
			return;
		}
	}

	for (i = 3; i > iCarrierNo; i--)
	{
#ifdef DEF_SOURCE_SYSTEM
		iResult = m_plnkWorkTabCarrier[i]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
#else
		iResult = m_plnkWorkTabCarrier[i]->SafeMoveXYTPos(DEF_TABCARRIER_ESCAPE_POS);
#endif
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);				
			return;
		}
	}

	if (m_plnkWorkTabCarrier[iCarrierNo]->IsShortPitchTab())
	{
		iResult = m_plnkWorkTabCarrier[iCarrierNo]->Forward();
	}
	else
	{
		iResult = m_plnkWorkTabCarrier[iCarrierNo]->Backward();
	}
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		
		return;
	}

	iResult = m_plnkWorkTabCarrier[iCarrierNo]->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);			
		return;
	}


//	for (j = 3; j<0; j--)
//	{
		iResult = m_plnkCtrlWorkTabFeeder->FeedTabIC();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}
		
		iResult = m_plnkCtrlWorkTabFeeder->PressTapeStep1();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}

		/*
		iResult = m_plnkWorkTabCarrier[iCarrierNo]->Up();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}
		
		iResult = m_plnkWorkTabCarrier[iCarrierNo]->AbsorbTabIC();
		if (iResult)
		{
			m_plnkWorkTabCarrier[iCarrierNo]->Down();
			m_plnkCtrlWorkTabFeeder->PressTapeStep2();
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}
		*/
		iResult = m_plnkCtrlWorkTabCarrier[iCarrierNo]->LoadTabIC();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = m_plnkCtrlWorkTabFeeder->PressTapeStep2();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}						
		iResult = m_plnkWorkTabCarrier[iCarrierNo]->Down();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}

		/*
		iResult = m_plnkWorkTabCarrier[iCarrierNo]->Backward();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}
		*/

		iResult = m_plnkWorkTabCarrier[iCarrierNo]->SafeMoveXYTPos(DEF_TABCARRIER_PREALIGN_POS);
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}
		/*
		MPos_XYT	m_posAlignOffset;
		m_posAlignOffset.Init(0.0, 0.0, 0.0);
		
		iResult = MOlbSystemPre.GetCtrlTabCarrierComponent(i)->AlignTabIC(&m_posAlignOffset);
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}
		*/
		/*iResult = m_plnkTabCarrier[i]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			
			return;
		}*/
//	}

		ProgWnd.DestroyWindow();

	//	작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30380, _T("confirm"), M_ICONINFORMATION);	
}

void CMaterialChangeDlg::OnClickTabCuttingOffsetChange() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR szTemp[250];

//	길게 타발하시려면 (+), 짧게 타발하시려면 (-)값을 넣어주세요.
	MyMessageBox(MY_MESSAGE_BOX,50018,_T("confirm"), M_OK);

	/** 목표위치 입력 받기 */
	double dOldXOffset = getUserOffsetX_withDisplay();

	swprintf(szTemp, _T("%.3f"), -dOldXOffset);
	if (!GetNumberBox(cAfterVal, 10, szTemp))
		return;

	/** 목표위치 설정하기 */
	double dNewXOffset = wcstod(cAfterVal,NULL);

	if (fabs(dNewXOffset) > 1.0)
	{
//		+1 보다 크거나 -1 보다 작습니다.
		MyMessageBox(MY_MESSAGE_BOX,50019,_T("confirm"), M_OK);
		return;
	}

//	Offset값을 지금 적용하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50020,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("The cutting offset application..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Wait for a moment..."));
	ProgWnd.PeekAndPump();

//@	int iSprocketPitchCount = (DEF_FRONT_GROUP == m_iGroupNo)
//@		? (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab
//@		: (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#ifdef DEF_SOURCE_SYSTEM
	int iSprocketPitchCount = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	int iSprocketPitchCount = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif

	double dNewTOffset = 360.0 / DEF_SPROCKET_ONE_PITCH_DIST / iSprocketPitchCount * dNewXOffset;

	double dOldTOffset;
	if (DEF_FRONT_GROUP == m_iGroupNo)
	{
		dOldTOffset = m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset;
		m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset = -dNewTOffset;
	}
	else
	{
		dOldTOffset = m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset;
		m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset = -dNewTOffset;
	}
	
	m_plnkPreBonderData->WriteData();

	getUserOffsetX_withDisplay();

	double dT_Diff = -dNewTOffset - dOldTOffset;	// 단위 : degree
	double dX_Diff = -dNewXOffset - dOldXOffset;

	m_rgdOffsetPosData[DEF_BADTABDETECTOR_WORK_POS] += dX_Diff;
	m_plnkTabFeeder->SaveBadTabOffsetPosParameter(m_rgdOffsetPosData);

	int iResult = m_plnkTabFeeder->RMoveSprocket2Pos(dT_Diff);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkTabFeeder->SafeMoveBadTabPos(DEF_BADTABDETECTOR_WORK_POS);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
	iResult = m_plnkCtrlTabFeeder->FeedTabIC(-2);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
	Sleep(500);
	
	iResult = m_plnkCtrlTabFeeder->FeedTabIC(2);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Offset 적용 완료.
	MyMessageBox(MY_MESSAGE_BOX,50021,_T("Information"), M_ICONINFORMATION);
}

double CMaterialChangeDlg::getUserOffsetX_withDisplay()
{
	double dUserOffset;
	int iSprocketPitchCount;

	if (DEF_FRONT_GROUP == m_iGroupNo)
	{
		dUserOffset = m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset;
//		iSprocketPitchCount = (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
	}
	else
	{
		dUserOffset = m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset;
//		iSprocketPitchCount = (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
	}

#ifdef DEF_SOURCE_SYSTEM
	iSprocketPitchCount = (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	iSprocketPitchCount = (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif

	double dX_Diff = DEF_SPROCKET_ONE_PITCH_DIST * iSprocketPitchCount / 360.0 * dUserOffset;

	WCHAR str[100];
	swprintf(str,_T("%.3f"), -dX_Diff);
	m_sTensionOffset.SetCaption(str);

	return dX_Diff;
}

void CMaterialChangeDlg::OnClickRegisterMarkChange() 
{
	// TODO: Add your control notification handler code here
	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();

	int iCamNo = 0;
	if (m_iGroupNo == DEF_FRONT_GROUP)
		iCamNo = DEF_F_TABALIGNER_CAMERA_1;
	else
		iCamNo = DEF_R_TABALIGNER_CAMERA_1;

	
	// Operation Type = DEF_PM_MARK / DEF_BLOB_MARK / DEF_CALIPER_MARK
	CVisionMonitorDlg dlg(plnkVision, DEF_PM_MARK, iCamNo);
	dlg.DoModal();
}
