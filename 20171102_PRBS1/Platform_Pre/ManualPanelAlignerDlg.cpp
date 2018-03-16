// ManualPanelAlignerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "ManualPanelAlignerDlg.h"
#include "MPlatformOlbSystem.h"
#include "MPanelAligner.h"
#include "MPanelTransfer.h"
#include "IIO.h"
#include "DefIOAddrPreBonder.h"
#include "MModelChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualPanelAlignerDlg dialog
extern MPlatformOlbSystem	MOlbSystemPre;

CManualPanelAlignerDlg::CManualPanelAlignerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualPanelAlignerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualPanelAlignerDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
#ifdef DEF_SOURCE_SYSTEM
	m_plnkPanelTransfer = MOlbSystemPre.GetPanelTransferComponent();
#endif
	m_plnkPanelTransferOut = MOlbSystemPre.GetPanelTransferOutComponent();

	m_plnkModelChange	= MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP);

	m_plnkIIO			= MOlbSystemPre.GetIOComponent();
}


void CManualPanelAlignerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualPanelAlignerDlg)
	DDX_Control(pDX, IDB_PANEL_ALIGNER_PICKUP_UP, m_btnPanelAlignerPickupUp);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_PICKUP_DOWN, m_btnPanelAlignerPickupDown);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_VACUUM_ON, m_btnPanelAlignerVacuumOn);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_VACUUM_OFF, m_btnPanelAlignerVacuumOff);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_LEVITATION_ON, m_btnPanelAlignerAirCVBlowOn);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_LEVITATION_OFF, m_btnPanelAlignerAirCVBlowOff);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_ALL_ON, m_btnPanelAlignerAirVacuumAllOn);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_ALL_OFF, m_btnPanelAlignerAirVacuumAllOff);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE1_ON, m_btnPanelAlignerAirVacuumPlate1On);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE1_OFF, m_btnPanelAlignerAirVacuumPlate1Off);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE2_ON, m_btnPanelAlignerAirVacuumPlate2On);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE2_OFF, m_btnPanelAlignerAirVacuumPlate2Off);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE3_ON, m_btnPanelAlignerAirVacuumPlate3On);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE3_OFF, m_btnPanelAlignerAirVacuumPlate3Off);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE4_ON, m_btnPanelAlignerAirVacuumPlate4On);
	DDX_Control(pDX, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE4_OFF, m_btnPanelAlignerAirVacuumPlate4Off);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_UP, m_btnPanelTransferUp);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_DOWN, m_btnPanelTransferDown);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_VACUUM_ON, m_btnPanelTransferVacuumOn);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_VACUUM_OFF, m_btnPanelTransferVacuumOff);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_OUT_UP, m_btnPanelTransferOutUp);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_OUT_DOWN, m_btnPanelTransferOutDown);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_VACUUM_OUT_ON, m_btnPanelTransferOutVacuumOn);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_VACUUM_OUT_OFF, m_btnPanelTransferOutVacuumOff);
	DDX_Control(pDX, IDS_PANEL_TRANSFER_IN_PANEL_DETECT, m_LblPanelDetectTIn);
	DDX_Control(pDX, IDS_PANEL_TRANSFER_OUT_PANEL_DETECT, m_LblPanelDetectTOut);

	DDX_Control(pDX, IDB_MODELCHANGE_AIRRAIL_FOR1, m_btnModelChangeAirRailFor1);
	DDX_Control(pDX, IDB_MODELCHANGE_AIRRAIL_BACK1, m_btnModelChangeAirRailBack1);
	DDX_Control(pDX, IDB_MODELCHANGE_AIRRAIL_FOR2, m_btnModelChangeAirRailFor2);
	DDX_Control(pDX, IDB_MODELCHANGE_AIRRAIL_BACK2, m_btnModelChangeAirRailBack2);
	//}}AFX_DATA_MAP
}


BEGIN_EVENTSINK_MAP(CManualPanelAlignerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualPanelAlignerDlg)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_PICKUP_UP, -600 /* Click */, OnClickPanelAlignerPickupUp, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_PICKUP_DOWN, -600 /* Click */, OnClickPanelAlignerPickupDown, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_VACUUM_ON, -600 /* Click */, OnClickPanelAlignerVacuumOn, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_VACUUM_OFF, -600 /* Click */, OnClickPanelAlignerVacuumOff, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_LEVITATION_ON, -600 /* Click */, OnClickPanelAlignerAirCVBlowOn, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_LEVITATION_OFF, -600 /* Click */, OnClickPanelAlignerAirCVBlowOff, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_ALL_ON, -600 /* Click */, OnClickPanelAlignerAirVacuumAllOn, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_ALL_OFF, -600 /* Click */, OnClickPanelAlignerAirVacuumAllOff, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE1_ON, -600 /* Click */, OnClickPanelAlignerAirVacuumPlate1On, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE1_OFF, -600 /* Click */, OnClickPanelAlignerAirVacuumPlate1Off, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE2_ON, -600 /* Click */, OnClickPanelAlignerAirVacuumPlate2On, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE2_OFF, -600 /* Click */, OnClickPanelAlignerAirVacuumPlate2Off, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_TRANSFER_UP, -600 /* Click */, OnClickPanelTransferUp, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_TRANSFER_DOWN, -600 /* Click */, OnClickPanelTransferDown, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_TRANSFER_VACUUM_ON, -600 /* Click */, OnClickPanelTransferVacuumOn, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_TRANSFER_VACUUM_OFF, -600 /* Click */, OnClickPanelTransferVacuumOff, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE3_ON, -600 /* Click */, OnClickPanelAlignerAirVacuumPlate3On, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE3_OFF, -600 /* Click */, OnClickPanelAlignerAirVacuumPlate3Off, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE4_ON, -600 /* Click */, OnClickPanelAlignerAirVacuumPlate4On, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_ALIGNER_AIR_VACUUM_PLATE4_OFF, -600 /* Click */, OnClickPanelAlignerAirVacuumPlate4Off, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_TRANSFER_OUT_UP, -600 /* Click */, OnClickPanelTransferOutUp, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_TRANSFER_OUT_DOWN, -600 /* Click */, OnClickPanelTransferOutDown, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_TRANSFER_VACUUM_OUT_ON, -600 /* Click */, OnClickPanelTransferVacuumOutOn, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_PANEL_TRANSFER_VACUUM_OUT_OFF, -600 /* Click */, OnClickPanelTransferVacuumOutOff, VTS_NONE)

	ON_EVENT(CManualPanelAlignerDlg, IDB_MODELCHANGE_AIRRAIL_FOR1, -600 /* Click */, OnClickModelchangeAirrailFor1, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_MODELCHANGE_AIRRAIL_BACK1, -600 /* Click */, OnClickModelchangeAirrailBack1, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_MODELCHANGE_AIRRAIL_FOR2, -600 /* Click */, OnClickModelchangeAirrailFor2, VTS_NONE)
	ON_EVENT(CManualPanelAlignerDlg, IDB_MODELCHANGE_AIRRAIL_BACK2, -600 /* Click */, OnClickModelchangeAirrailBack2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BEGIN_MESSAGE_MAP(CManualPanelAlignerDlg, CDialog)
	//{{AFX_MSG_MAP(CManualPanelAlignerDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualPanelAlignerDlg message handlers

BOOL CManualPanelAlignerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_uiTimerID = SetTimer(1, 200, NULL);
	
	CString szTextTemp;
	//나가기
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualPanelAlignerDlg_1"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

#ifdef DEF_GATE_SYSTEM
	GetDlgItem(IDB_PANEL_TRANSFER_UP)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_TRANSFER_DOWN)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_TRANSFER_VACUUM_ON)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_TRANSFER_VACUUM_OFF)->EnableWindow(FALSE);
	GetDlgItem(IDS_PANEL_TRANSFER_IN_PANEL_DETECT_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDS_PANEL_TRANSFER_IN_PANEL_DETECT)->EnableWindow(FALSE);
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManualPanelAlignerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL	bStatus = FALSE;
	BOOL	bOldStatus = FALSE;

	if (m_uiTimerID == nIDEvent)
	{
		// Panel Aligner PickUp Unit Up/Down Cylinder
		// Panel Aligner PickUp Unit Up Status Check
		bStatus = m_plnkPanelAligner->IsUpPickUpUnit();
		bOldStatus = m_btnPanelAlignerPickupUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerPickupUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerPickupUp.SetValue(FALSE);

		// Panel Aligner PickUp Unit Down Status Check
		bStatus = m_plnkPanelAligner->IsDownPickUpUnit();
		bOldStatus = m_btnPanelAlignerPickupDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerPickupDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerPickupDown.SetValue(FALSE);
		
		// Panel Aligner PickUp Unit Vacuum
		// Panel Aligner PickUp Unit Absorb Status Check
		bStatus = m_plnkPanelAligner->IsPanelAbsorbed();
		bOldStatus = m_btnPanelAlignerVacuumOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerVacuumOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerVacuumOn.SetValue(FALSE);

		// Panel Aligner PickUp Unit Release Status Check
		bStatus = m_plnkPanelAligner->IsPanelReleased();
		bOldStatus = m_btnPanelAlignerVacuumOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerVacuumOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerVacuumOff.SetValue(FALSE);

		// Panel Aligner Air CVBlow
		// Panel Aligner Air CVBlow On Status Check
		bStatus = m_plnkPanelAligner->IsAirCVBlowOn();
		bOldStatus = m_btnPanelAlignerAirCVBlowOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirCVBlowOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirCVBlowOn.SetValue(FALSE);

		// Panel Aligner Air CVBlow Off Status Check
		bStatus = m_plnkPanelAligner->IsAirCVBlowOff();
		bOldStatus = m_btnPanelAlignerAirCVBlowOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirCVBlowOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirCVBlowOff.SetValue(FALSE);

		// Panel Aligner Air Vacuum
		// Panel Aligner Air Vacuum All On Status Check
		int iWorkPanelNo = DEF_MAX_WORK_SCHEDULE;
#ifdef DEF_GATE_SYSTEM
		iWorkPanelNo = 0;
#endif
		bStatus = m_plnkPanelAligner->IsAirCVVacAllOn(iWorkPanelNo);
		bOldStatus = m_btnPanelAlignerAirVacuumAllOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumAllOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumAllOn.SetValue(FALSE);

		// Panel Aligner Air Vacuum All Off Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacAllOff();
		bOldStatus = m_btnPanelAlignerAirVacuumAllOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumAllOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumAllOff.SetValue(FALSE);

		// Panel Aligner Air Vacuum Point On Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacPlate1On();
		bOldStatus = m_btnPanelAlignerAirVacuumPlate1On.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumPlate1On.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumPlate1On.SetValue(FALSE);

		// Panel Aligner Air Vacuum Point Off Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacPlate1Off();
		bOldStatus = m_btnPanelAlignerAirVacuumPlate1Off.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumPlate1Off.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumPlate1Off.SetValue(FALSE);

		// Panel Aligner Air Vacuum Surface On Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacPlate2On();
		bOldStatus = m_btnPanelAlignerAirVacuumPlate2On.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumPlate2On.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumPlate2On.SetValue(FALSE);

		// Panel Aligner Air Vacuum Surface Off Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacPlate2Off();
		bOldStatus = m_btnPanelAlignerAirVacuumPlate2Off.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumPlate2Off.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumPlate2Off.SetValue(FALSE);

		// Panel Aligner Air Vacuum Surface On Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacPlate3On();
		bOldStatus = m_btnPanelAlignerAirVacuumPlate3On.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumPlate3On.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumPlate3On.SetValue(FALSE);

		// Panel Aligner Air Vacuum Surface Off Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacPlate3Off();
		bOldStatus = m_btnPanelAlignerAirVacuumPlate3Off.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumPlate3Off.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumPlate3Off.SetValue(FALSE);
		
		// Panel Aligner Air Vacuum Surface On Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacPlate4On();
		bOldStatus = m_btnPanelAlignerAirVacuumPlate4On.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumPlate4On.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumPlate4On.SetValue(FALSE);
		
		// Panel Aligner Air Vacuum Surface Off Status Check
		bStatus = m_plnkPanelAligner->IsAirCVVacPlate4Off();
		bOldStatus = m_btnPanelAlignerAirVacuumPlate4Off.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelAlignerAirVacuumPlate4Off.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelAlignerAirVacuumPlate4Off.SetValue(FALSE);
		
		
#ifdef DEF_SOURCE_SYSTEM
		bStatus = m_plnkPanelTransfer->IsUpPickUpUDCyl();
		bOldStatus = m_btnPanelTransferUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferUp.SetValue(FALSE);

		// Panel Transfer Down Status Check
		bStatus = m_plnkPanelTransfer->IsDownPickUpUDCyl();
		bOldStatus = m_btnPanelTransferDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferDown.SetValue(FALSE);

		// Panel Transfer Vacuum
		// Panel Transfer Absorb Status Check
		bStatus = m_plnkPanelTransfer->IsPanelAbsorbed();
		bOldStatus = m_btnPanelTransferVacuumOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferVacuumOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferVacuumOn.SetValue(FALSE);

		bStatus = m_plnkPanelTransfer->IsPanelDetected();
		bOldStatus = m_LblPanelDetectTIn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_LblPanelDetectTIn.SetCaption(_T("On"));
			m_LblPanelDetectTIn.SetBackColor(DEF_ON_COLOR);
			m_LblPanelDetectTIn.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_LblPanelDetectTIn.SetCaption(_T("Off"));
			m_LblPanelDetectTIn.SetBackColor(DEF_OFF_COLOR);
			m_LblPanelDetectTIn.SetValue(bStatus);
		}
#endif


		bStatus = m_plnkPanelTransferOut->IsUpPickUpUDCyl();
		bOldStatus = m_btnPanelTransferOutUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferOutUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferOutUp.SetValue(FALSE);

		// Panel Transfer Down Status Check
		bStatus = m_plnkPanelTransferOut->IsDownPickUpUDCyl();
		bOldStatus = m_btnPanelTransferOutDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferOutDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferOutDown.SetValue(FALSE);

		// Panel Transfer Vacuum
		// Panel Transfer Absorb Status Check
		bStatus = m_plnkPanelTransferOut->IsPanelAbsorbed();
		bOldStatus = m_btnPanelTransferOutVacuumOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferOutVacuumOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferOutVacuumOn.SetValue(FALSE);

		// Panel Transfer Panel Stop Detect
		bStatus = m_plnkPanelTransferOut->IsPanelDetected();
		bOldStatus = m_LblPanelDetectTOut.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_LblPanelDetectTOut.SetCaption(_T("On"));
			m_LblPanelDetectTOut.SetBackColor(DEF_ON_COLOR);
			m_LblPanelDetectTOut.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_LblPanelDetectTOut.SetCaption(_T("Off"));
			m_LblPanelDetectTOut.SetBackColor(DEF_OFF_COLOR);
			m_LblPanelDetectTOut.SetValue(bStatus);
		}
	
		bStatus = m_plnkPanelTransfer->IsForAirRailFB1();
		bOldStatus = m_btnModelChangeAirRailFor1.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnModelChangeAirRailFor1.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnModelChangeAirRailFor1.SetValue(FALSE);

		bStatus = m_plnkPanelTransfer->IsBackAirRailFB1();
		bOldStatus = m_btnModelChangeAirRailBack1.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnModelChangeAirRailBack1.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnModelChangeAirRailBack1.SetValue(FALSE);


		bStatus = m_plnkPanelTransfer->IsForAirRailFB2();
		bOldStatus = m_btnModelChangeAirRailFor2.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnModelChangeAirRailFor2.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnModelChangeAirRailFor2.SetValue(FALSE);

		bStatus = m_plnkPanelTransfer->IsBackAirRailFB2();
		bOldStatus = m_btnModelChangeAirRailBack2.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnModelChangeAirRailBack2.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnModelChangeAirRailBack2.SetValue(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CManualPanelAlignerDlg::OnClickPanelAlignerPickupUp() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner PickUp Up/Down Cylinder Up..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Will do Panel Aligner PickUp Up/Down Cylinder Up.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->UpPickUpUnit();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner PickUp Up/Down Cylinder Up"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerPickupDown() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner PickUp Up/Down Cylinder Down..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner PickUp Up/Down Cylinder Down.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->DownPickUpUnit();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner PickUp Up/Down Cylinder Down"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerVacuumOn() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner PickUp Unit Absorb..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner PickUp Unit Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AbsorbPanel();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner PickUp Unit Absorb"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerVacuumOff() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner PickUp Unit Release..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner PickUp Unit release.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->ReleasePanel();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner PickUp Unit Release"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirCVBlowOn() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air CVBlow On..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Blow.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVBlowOn();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air CVBlow On"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirCVBlowOff() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air CVBlow Off..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Float Off.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVBlowOff();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air CVBlow Off"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumAllOn() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum All On..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuun All Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacAllOn(DEF_MAX_WORK_SCHEDULE);
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum All On"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumAllOff()
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum All Off..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuum All Off.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacAllOff();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum All Off"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumPlate1On() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum Plate 1 On..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuun Plate 1 Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacPlate1On();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum Plate 1,2 On"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumPlate1Off() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum Plate 1 Off..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuum Plate 1 Off.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacPlate1Off();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum Plate 1,2 Off"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumPlate2On() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum Plate 2 On..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuun Plate 2 Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacPlate2On();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum Plate 3,4 On"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumPlate2Off() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum Plate 2 Off..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuum Plate 2 Off.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacPlate2Off();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum Plate 3,4 Off"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumPlate3On()
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum Plate 3 On..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuun Plate 3 Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacPlate3On();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum Plate 3,4 On"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumPlate3Off()
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum Plate 3 Off..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuum Plate 3 Off.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacPlate3Off();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum Plate 3,4 Off"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumPlate4On()
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum Plate 4 On..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuun Plate 4 Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacPlate4On();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum Plate 3,4 On"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();		
}

void CManualPanelAlignerDlg::OnClickPanelAlignerAirVacuumPlate4Off()
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Aligner Air Vacuum Plate 4 Off..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Aligner Air Vacuum Plate 4 Off.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->AirCVVacPlate4Off();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Aligner Air Vacuum Plate 3,4 Off"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualPanelAlignerDlg::OnClickPanelTransferUp() 
{
	// TODO: Add your control notification handler code here
#ifdef DEF_SOURCE_SYSTEM
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Up/Down Cylinder Up..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Up/Down Cylinder Up.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->UpPickUpUDCyl();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Transfer Up/Down Cylinder Up"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
#endif
}

void CManualPanelAlignerDlg::OnClickPanelTransferDown() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Up/Down Cylinder Down..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Up/Down Cylinder Down.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

#ifdef DEF_SOURCE_SYSTEM
	// hongju 추가 Check Interlock
	iResult = MOlbSystemPre.GetPanelTransferComponent()->IsPanelReleased();
	if (!iResult)
	{
//		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
//		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		MyMessageBox(_T("Panel Transfer Panel Absorbed"), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Transfer Up/Down Cylinder Down"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
#endif
}

void CManualPanelAlignerDlg::OnClickPanelTransferVacuumOn() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Absorb..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->AbsorbPanel();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Transfer Absorb"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelTransferVacuumOff() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Release..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Vacuum off .\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->ReleasePanel();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Transfer Release"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickPanelTransferOutUp() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Up/Down Cylinder Up..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Up/Down Cylinder Up.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransferOut->UpPickUpUDCyl();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Transfer Up/Down Cylinder Up"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualPanelAlignerDlg::OnClickPanelTransferOutDown() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Up/Down Cylinder Down..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Up/Down Cylinder Down.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	// hongju 추가 Check Interlock
	iResult = MOlbSystemPre.GetPanelTransferOutComponent()->IsPanelReleased();
	if (!iResult)
	{
//		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
//		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		MyMessageBox(_T("Panel Transfer Panel Absorbed"), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelTransferOut->DownPickUpUDCyl();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Transfer Up/Down Cylinder Down"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualPanelAlignerDlg::OnClickPanelTransferVacuumOutOn() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Absorb..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransferOut->AbsorbPanel();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Transfer Absorb"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualPanelAlignerDlg::OnClickPanelTransferVacuumOutOff() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Release..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Vacuum off .\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransferOut->ReleasePanel();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult,_T("Panel Transfer Release"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualPanelAlignerDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CDialog::OnCancel();	
}

void CManualPanelAlignerDlg::OnClickModelchangeAirrailFor1() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox(_T("AirRail1 Do you want to move forward?\r\nAirRail1 전진동작을 하겠습니까?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = SUCCESS;


	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer AirRail Cylinder1 For..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer AirRail F/B Cylinder For.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->ForAirRailFB1();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickModelchangeAirrailBack1() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox(_T("Do you want AirRail1 reverse operation?\r\nAirRail1 후진동작을 하겠습니까?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer AirRail Cylinder1 Back..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer AirRail F/B Cylinder Back.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->BackAirRailFB1();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelAlignerDlg::OnClickModelchangeAirrailFor2() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox(_T("AirRail2 Do you want to move forward?\r\n\r\nAirRail2 전진동작을 하겠습니까?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = SUCCESS;


	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer AirRail Cylinder2 For..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer AirRail F/B Cylinder For.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->ForAirRailFB2();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualPanelAlignerDlg::OnClickModelchangeAirrailBack2() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox(_T("Do you want AirRail2 reverse operation?\r\nAirRail2 후진동작을 하겠습니까?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = SUCCESS;

	if (MyMessageBox(_T("Do you want AirRail2 reverse operation?\r\nAirRail2 후진동작을 하겠습니까?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer AirRail Cylinder2 Back..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer AirRail F/B Cylinder Back.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->BackAirRailFB2();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}
