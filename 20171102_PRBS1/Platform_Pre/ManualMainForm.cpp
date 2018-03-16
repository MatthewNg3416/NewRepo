// ManualMainForm.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "ManualMainForm.h"
#include "ManualIOMonitoringDlg.h"
#include "ManualLimitSensorDlg.h"
#include "AutoMotorOriginDlg.h"
#include "SystemThreeBtnDlg.h"
#include "ManualPanelAlignerDlg.h"
//@#include "ManualTiltDlg.h"

#include "ManualTabMounterDlg.h"
#include "ManualTabFeederDlg.h"
#include "VisionMonitorDlg.h"
#include "ManualTabCarrierDlg.h"
#include "ManualHandlerDlg.h"
#include "MPanelAligner.h"
#include "MPanelTransfer.h"
#include "MTabCarrier.h"
#include "MTabFeeder.h"
#include "MTabMounter.h"
#include "MTabAligner.h"
#include "MCameraCarrier.h"
#include "IOpPanel.h"
#include "MCtrlPanelAligner.h"
#include "MCtrlTabMounter.h"
#include "MCtrlInspectionCamera.h"
#include "MCtrlTabFeeder.h"
#include "MCtrlTabCarrier.h"
#include "MTrsAutoManager.h"
//#include "MRollerConveyor.h"
#include "MProcessData.h"
#include "DefIOAddrPreBonder.h"
#include "ManualCameraExpShrDlg.h"
#include "ManualPanelTransferDlg.h"
#include "ISercosSetup.h"
#include "IVision.h"
#include "MVisionCalibration.h"
#include "MTrsAutoManager.h"

//EQ_STOP Report Flag 추가 100121 SJ_YYK
//#include "EqStopDlg1.h"
#include "EqStopCodeDlg.h"   //100825.KMS
#include "TeachBadTabDetectorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualMainForm
extern	MPlatformOlbSystem	MOlbSystemPre;

IMPLEMENT_DYNCREATE(CManualMainForm, CFormView)

CManualMainForm::CManualMainForm() : CFormView(CManualMainForm::IDD)
{
	//{{AFX_DATA_INIT(CManualMainForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkProcessData = MOlbSystemPre.GetProcessData();

	m_plnkVision = MOlbSystemPre.GetVisionComponent();

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_plnkTabCarrier[i] = MOlbSystemPre.GetTabCarrierComponent(i);
		m_plnkCtrlTabCarrier[i]	= MOlbSystemPre.GetCtrlTabCarrierComponent(i);

		m_plnkRTabCarrier[i] = MOlbSystemPre.GetRTabCarrierComponent(i);
		m_plnkCtrlRTabCarrier[i] = MOlbSystemPre.GetCtrlRTabCarrierComponent(i);
	}

	m_plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
	m_plnkCtrlPanelAligner = MOlbSystemPre.GetCtrlPanelAlignerComponent();
	
	//101001.KKY_____
	m_bInitialized = FALSE;
	//_______________

}

CManualMainForm::~CManualMainForm()
{
}


void CManualMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualMainForm)
	DDX_Control(pDX, IDC_IO_JOB, m_lblIOJob);
	DDX_Control(pDX, IDC_INIT, m_lblInitJob);
	DDX_Control(pDX, IDC_BATCH_JOB, m_lblBatchJob);
	DDX_Control(pDX, IDC_UNIT_JOB, m_lblUnitJob);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManualMainForm, CFormView)
	//{{AFX_MSG_MAP(CManualMainForm)
	ON_WM_PAINT()
	ON_MESSAGE(WM_DISP_MULTI_LANGUAGE_DLG_REQ, OnDisplayLanguage)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualMainForm diagnostics

#ifdef _DEBUG
void CManualMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CManualMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CManualMainForm message handlers

void CManualMainForm::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CFormView::OnPaint() for painting messages
}

BEGIN_EVENTSINK_MAP(CManualMainForm, CFormView)
    //{{AFX_EVENTSINK_MAP(CManualMainForm)
	ON_EVENT(CManualMainForm, IDB_IO_CHECK, -600 /* Click */, OnIOCheck, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_LIMIT_CHECK, -600 /* Click */, OnLimitCheck, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_RETURN_ORIGIN, -600 /* Click */, OnReturnOrigin, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_PANEL_ALIGN, -600 /* Click */, OnPanelAlign, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_ROLLER_CONVEYOR, -600 /* Click */, OnClickRollerConveyor, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABMOUNTER, -600 /* Click */, OnClickTabmounter, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABCARRIER, -600 /* Click */, OnClickTabCarrier, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABFEEDER, -600 /* Click */, OnClickTabFeeder, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_PANEL_ALIGNER, -600 /* Click */, OnPanelAlignerDlg, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABCARRIER_R, -600 /* Click */, OnClickTabcarrierR, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABFEEDER_R, -600 /* Click */, OnClickTabfeederR, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_MOUNT_TABIC, -600 /* Click */, OnMountTabIC, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_AUTO_CHANGE_TABIC_REEL, -600 /* Click */, OnClickAutoChangeTabicReel, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_AUTO_CHANGE_TABIC_REEL2, -600 /* Click */, OnClickAutoChangeTabicReel2, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_HEATER_POWER_RESET, -600 /* Click */, OnHeaterPowerReset, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_SERCOS_INITIALIZE, -600 /* Click */, OnSercosInitialize, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_CAMERA_ES, -600 /* Click */, OnClickCameraExpandShrink, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_CHECK_PUNCH_REPEATABILITY, -600 /* Click */, OnCheckPunchRepeatability, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_SETUP_TEST, -600 /* Click */, OnClickSetupTest, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_ENG_STOP, -600 /* Click */, OnClickEngStop, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_PANEL_TRANSFER_CENTER, -600 /* Click */, OnClickPanelTransferCenter, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_PANEL_ALIGNER3, -600 /* Click */, OnClickPanelAligner3, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABIC_LOAD_CARRIER, -600 /* Click */, OnClickTabicLoadCarrier, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TILTING_JOB, -600 /* Click */, OnClickTiltingJob, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_FRONT_TABMOUNTER__MOVE_TO_LOAD_POS, -600 /* Click */, OnClickFrontTabmounterMoveToLoadPos, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_REAR_TABMOUNTER__MOVE_TO_LOAD_POS, -600 /* Click */, OnClickRearTabmounterMoveToLoadPos, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_FRONT_TABCARRIER__MOVE_TO_READY_POS, -600 /* Click */, OnClickFrontTabcarrierMoveToReadyPos, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_REAR_TABCARRIER__MOVE_TO_READY_POS, -600 /* Click */, OnClickRearTabcarrierMoveToReadyPos, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_FRONT_TABCARRIER__MOVE_TO_UNLOAD_POS, -600 /* Click */, OnClickFrontTabcarrierMoveToUnloadPos, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_REAR_TABCARRIER__MOVE_TO_UNLOAD_POS, -600 /* Click */, OnClickRearTabcarrierMoveToUnloadPos, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABIC_LOAD_ALL_TABMOUNTER, -600 /* Click */, OnClickTabicLoadAllTabmounter, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABIC_LOAD_ALL_TABMOUNTER_REAR, -600 /* Click */, OnClickTabicLoadAllTabmounterRear, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABIC_REMOVE_FRONT, -600 /* Click */, OnClickTabicRemoveFront, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_TABIC_REMOVE_REAR, -600 /* Click */, OnClickTabicRemoveRear, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_MANUAL_TURN_HANDLER, -600 /* Click */, OnClickManualTurnHandler, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_PANEL_TRANSFER_IN, -600 /* Click */, OnClickPanelTransferIn, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_FRONT_GANTRY_MOVE_TO_CLEAN_POS, -600 /* Click */, OnClickFrontGantryMoveToCleanPos, VTS_NONE)
	ON_EVENT(CManualMainForm, IDB_REAR_GANTRY_MOVE_TO_CLEAN_POS, -600 /* Click */, OnClickRearGantryMoveToCleanPos, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManualMainForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class	
#ifndef DEF_SETUP
	// hongju 추가...20080829
	GetDlgItem(IDB_SETUP_TEST)->EnableWindow(FALSE);
	GetDlgItem(IDB_SETUP_TEST)->ShowWindow(FALSE);
#endif

	GetDlgItem(IDB_PANEL_ALIGN)->EnableWindow(FALSE);
	
	GetDlgItem(IDB_TILTING_JOB)->EnableWindow(FALSE);
#ifdef DEF_GATE_SYSTEM
	GetDlgItem(IDB_PANEL_TRANSFER_CENTER)->EnableWindow(FALSE);	//@ ---> Transfer In
#endif


	GetDlgItem(IDB_REAR_TABMOUNTER__MOVE_TO_LOAD_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_TABCARRIER__MOVE_TO_READY_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_TABCARRIER__MOVE_TO_UNLOAD_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABIC_REMOVE_REAR)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABCARRIER_R)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_GANTRY_MOVE_TO_CLEAN_POS)->EnableWindow(FALSE);

	
#ifndef DEF_USE_TURN_HANDLER
	GetDlgItem(IDB_MANUAL_TURN_HANDLER)->EnableWindow(FALSE);
#endif
#ifndef DEF_USE_TRANSFER_CENTER
	GetDlgItem(IDB_PANEL_TRANSFER_CENTER)->EnableWindow(FALSE);
#endif

	// ML_hongju
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetManualViewHandle(m_hWnd);
	this->SendMessage(WM_DISP_MULTI_LANGUAGE_DLG_REQ);
	
	//101001.KKY_____
	m_bInitialized = TRUE;
	//_______________
}

void CManualMainForm::OnIOCheck() 
{
	MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetPReelTensionManualOperation(TRUE);
	MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetPReelTensionManualOperation(TRUE);

	gIOTestMode = TRUE;
	CManualIOMonitoringDlg dlg;
	dlg.DoModal();
	gIOTestMode = FALSE;

	MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetPReelTensionManualOperation(FALSE);
	MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetPReelTensionManualOperation(FALSE);
}

void CManualMainForm::OnLimitCheck() 
{
	CManualLimitSensorDlg dlg;
	dlg.DoModal();
}

void CManualMainForm::OnReturnOrigin() 
{
	CAutoMotorOriginDlg dlg;
	dlg.DoModal();
}

void CManualMainForm::OnClickTabFeeder() 
{
	CManualTabFeederDlg dlg(DEF_FRONT_GROUP);
	dlg.DoModal();		
}

int CManualMainForm::getWorkOption(EWorkingSide* peWorkSide, int* piGroupNo, int* piTab)	//@--->미사용 함수
{
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;
	*piGroupNo = DEF_FRONT_GROUP;

	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
//		작업변을 선택하세요.
		iResult = MyMessageBox(MY_MESSAGE_BOX, 30300, _T("Using work side selection"), M_ICONQUESTION|M_YESNO, _T("Yl"), _T("Yr"));
		EWorkingSide eWorkSide = (IDYES == iResult) ? WORKING_SIDE_Y_LEFT : WORKING_SIDE_Y_RIGHT;

#ifndef DEF_MIRROR_
		if (WORKING_SIDE_Y_RIGHT == eWorkSide)
#else
		if (WORKING_SIDE_Y_LEFT == eWorkSide)
#endif
		{
			*piGroupNo = DEF_REAR_GROUP;
		}
	}

	*peWorkSide = m_plnkProcessData->GetCurrentWorkingSide(*piGroupNo);
	if (WORKING_SIDE_UNKNOWN == *peWorkSide)
	{
//		작업 수행 불가합니다. [해당 작업변 사용하지 않음.]
		MyMessageBox(MY_MESSAGE_BOX, 30301, _T("Error"), M_ICONERROR);
		return NOT_USE_WORKSIDE;
	}

	CString strMsg;
	//	TabIC Loading 방식을 결정하세요.
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_9"), &strMsg);
	CString strBtn1;
	//	전체 Loading
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_10"), &strBtn1);
	CString strBtn2;
	//	Tab 번호 선택
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_11"), &strBtn2);
	
	iResult = MyMessageBox(strMsg, _T("TabIC Loading form select"), M_ICONQUESTION|M_YESNO, strBtn1.GetBuffer(strBtn1.GetLength()), strBtn2.GetBuffer(strBtn2.GetLength())); // 수정할 필요 없는 메시지박스_영락


	if (IDNO == iResult)
	{
		WCHAR cAfterVal[10];
		memset(cAfterVal, 0, sizeof(cAfterVal));

		if (FALSE == GetNumberBox(cAfterVal, 10, _T("1")))
			return ERR_MANUAL_MAINFORM_FAILURE;
		
		int iTabNo = _wtoi(cAfterVal) - 1;

		if (iTabNo < 0 || iTabNo >= m_plnkProcessData->GetMaxTabQty(*peWorkSide))
		{
//			Tab 번호 입력 오류
			MyMessageBox(MY_MESSAGE_BOX, 30302, _T("Input Error"), M_ICONERROR);
			return ERR_MANUAL_MAINFORM_FAILURE;
		}

		*piTab = iTabNo;
	}
	else
		*piTab = -1;

	return ERR_MANUAL_MAINFORM_SUCCESS;
}

void CManualMainForm::OnClickRollerConveyor() 
{
#if FALSE
	CManualConveyorDlg dlg;
	dlg.DoModal();
#endif
}

void CManualMainForm::OnClickTabmounter() 
{
	CManualTabMounterDlg dlg;
	dlg.DoModal();
}

void CManualMainForm::OnClickTabCarrier() 
{
	CManualTabCarrierDlg dlg(DEF_FRONT_GROUP);
	dlg.DoModal();	
}

void CManualMainForm::OnPanelAlignerDlg() 
{
	CManualPanelAlignerDlg dlg;
	dlg.DoModal();	
}

void CManualMainForm::OnPanelAlign() 
{
#if FALSE	//KKY.임시
	int i,iResult;
	MTickTimer tm;
//	MRollerConveyor*	m_plnkLoadConveyor;
//	MRollerConveyor*	m_plnkUnloadConveyor;
	MPanelTransfer*		m_plnkPanelTransfer;
	MTabMounter*		m_plnkTabMounter[DEF_MAX_TABMOUNTER];
	CVisionMonitorDlg	dlg(m_plnkVision, DEF_PM_MARK, DEF_PANEL_ALIGN_CAMERA_1);

	m_plnkLoadConveyor		= MOlbSystemPre.GetRollerConveyorComponent(DEF_LOAD_CONVEYOR);
	m_plnkUnloadConveyor	= MOlbSystemPre.GetRollerConveyorComponent(DEF_UNLOAD_CONVEYOR);
	m_plnkPanelTransfer		= MOlbSystemPre.GetPanelTransferComponent();
	for (int i = 0 ; i < DEF_MAX_TABMOUNTER ; i++)
	{
		m_plnkTabMounter[i] = MOlbSystemPre.GetTabMounterComponent(i);
	}


	CString strMsg;
//	잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_15"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();


	// Interlock Check - All Mounter Up
	for (int i = 0 ; i < DEF_MAX_TABMOUNTER ; i++)
	{
		if (FALSE == m_plnkTabMounter[i]->IsUpUDCyl())
		{
//			Tab Mounter가 Up 상태가 아닙니다.
			MyMessageBox(MY_MESSAGE_BOX, 30323, _T("Tab Mounter Not Up"), M_ICONERROR);
			return;
		}
	}

	// Interlock Check - Panel Not Exist at Panel Transfer
	if (TRUE == m_plnkPanelTransfer->IsPanelDetected()
		|| TRUE == m_plnkPanelTransfer->IsPanelAbsorbed())
	{
//		Panel Transfer에 Panel이 존재합니다.
		MyMessageBox(MY_MESSAGE_BOX, 30324, _T("Transfer Panel Exist"), M_ICONERROR);
		return;  
	}

	// Interlock Check - Panel Not Exist at Panel Aligner
	if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed())
	{
//		Panel Aligner에 Panel이 존재합니다.
		MyMessageBox(MY_MESSAGE_BOX, 30325, _T("Aligner Panel Exist"), M_ICONERROR);
		return;
	}

	// Interlock Check - Aligner Down
	if (FALSE == m_plnkPanelAligner->IsDownPickUpUnit())
	{
//		Panel Aligner가 Down 상태가 아닙니다.
		MyMessageBox(MY_MESSAGE_BOX, 30326, _T("Aligner Not Down"), M_ICONERROR);
		return;
	}

	// Interlock Check - Transfer Down
	if (FALSE == m_plnkPanelTransfer->IsDownPickUpUDCyl())
	{
//		Panel Transfer가 Down 상태가 아닙니다.
		MyMessageBox(MY_MESSAGE_BOX, 30327, _T("Transfer Not Down"), M_ICONERROR);
		return;
	}

	// Interlock Check - Transfer Stopper Down
	/*if (FALSE == m_plnkPanelTransfer->IsDownStopperUDCyl())
	{
//		Panel Transfer Stopper가 Down 상태가 아닙니다.
		MyMessageBox(MY_MESSAGE_BOX, 30328, _T("Transfer Stopper Not Down"), M_ICONERROR);
		return;
	}*/

	// Interlock Check - Load Conveyor Panel Not Exist at Stopper Pos
	if (m_plnkLoadConveyor->IsPanelStopDetected())
	{
//		Panel이 Load Conveyor Stopper와 너무 가깝습니다. 정지센서에 감지되지 않도록 뒤로 이동해 주세요
		MyMessageBox(MY_MESSAGE_BOX, 30329, _T("Panel Exist Near at Load Conveyor Stopper"), M_ICONERROR);
		return;
	}

//	Load Conveyor에서 Panel을 투입하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30330, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
/*
	iResult = m_plnkLoadConveyor->UpStopperUDCyl();
	if (iResult)
	{
		CString strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()), "Error", M_ICONERROR);
		return;
	}
*/
	if (FALSE == m_plnkLoadConveyor->IsPanelDecelDetected())
	{
		iResult = m_plnkLoadConveyor->MoveForward();
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}

	tm.StartTimer();
	while (TRUE)
	{
		if (m_plnkLoadConveyor->IsPanelDecelDetected())
		{
			if (m_plnkLoadConveyor->IsInMovingForward())
			{
				iResult = m_plnkLoadConveyor->ReduceSpeed();
				if (iResult)
				{
					m_plnkLoadConveyor->StopMoving();
// jdy sesl					MyMessageBox(iResult, _T("Error"), M_ICONERROR);
					CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
					MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
					return;
				}
			}
			else
			{
				iResult = m_plnkLoadConveyor->MoveForwardReducedSpeed();
				if (iResult)
				{
					m_plnkLoadConveyor->StopMoving();
// jdy sesl					MyMessageBox(iResult, _T("Error"), M_ICONERROR);
					CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
					MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
					return;
				}
			}
			break;
		}

		if (tm.MoreThan(10))
		{
			m_plnkLoadConveyor->StopMoving();
//			10초 동안 Load Conveyor 감속 센서에 Panel 감지가 되지 않았습니다.
			MyMessageBox(MY_MESSAGE_BOX, 30331, _T("Decel Sensor Panel Not Detected"), M_ICONERROR);
			return;
		}
		Sleep(1);
	}

	tm.StartTimer();
	while (TRUE)
	{
		if (m_plnkLoadConveyor->IsPanelStopDetected())
		{
			Sleep((ULONG)(m_plnkSystemData->m_dStopperArrivingDelay)*1000);

			iResult = m_plnkLoadConveyor->StopMoving();
			if (iResult)
			{
				m_plnkLoadConveyor->StopMoving();
// jdy sesl				MyMessageBox(iResult, _T("Error"), M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
			break;
		}

		if (tm.MoreThan(10 + m_plnkSystemData->m_dStopperArrivingDelay))
		{
			m_plnkLoadConveyor->StopMoving();
//			10초 동안 Load Conveyor 감속 센서에 Panel 감지가 되지 않았습니다.
			MyMessageBox(MY_MESSAGE_BOX, 30332, _T("Stop Sensor Panel Not Detected"), M_ICONERROR);
			return;
		}
		Sleep(1);
	}

	// Panel Transfer Panel 인수 위치로 이동
	m_plnkPanelTransfer->DownPickUpUDCyl(TRUE);
	///m_plnkPanelTransfer->UpStopperUDCyl(TRUE);

	iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
	if (iResult)
	{
// jdy sesl    MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	/*
	iResult = m_plnkPanelTransfer->UpStopperUDCyl();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	*/
	//170928 JSH Gear
	//iResult = m_plnkPanelTransfer->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS, FALSE);
	iResult = m_plnkPanelTransfer->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	// Panel Transfer 로 Panel 진입
	iResult = m_plnkPanelAligner->AirCVBlowOn();						
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
	/*
	iResult = m_plnkLoadConveyor->DownStopperUDCyl();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}*/

	iResult = m_plnkLoadConveyor->MoveForward();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	tm.StartTimer();
	while (TRUE)
	{
		if (TRUE == m_plnkPanelTransfer->IsPanelDetected())
		{
			iResult = m_plnkLoadConveyor->ReduceSpeed();
			if (iResult)
			{
// jdy sesl				MyMessageBox(iResult, _T("Error"), M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
			break;
		}

		if (tm.MoreThan(10))
		{
			m_plnkLoadConveyor->StopMoving();
//			10초 동안 Panel Transfer 센서에 Panel 감지가 되지 않았습니다.
			MyMessageBox(MY_MESSAGE_BOX, 30333, _T("Panel Transfer Sensor Panel Not Detected"), M_ICONERROR);
			return;
		}
		Sleep(1);
	}

	Sleep((ULONG)(m_plnkSystemData->m_dStopperArrivingDelay * 1000 * 1.5));

	iResult = m_plnkLoadConveyor->StopMoving();
	if (iResult)
	{
		m_plnkLoadConveyor->StopMoving();
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	if (TRUE == m_plnkPanelTransfer->IsPanelAbsorbed())
	{
//		Panel Transfer에 Panel이 이미 흡착되어 있습니다.
		MyMessageBox(MY_MESSAGE_BOX, 30334, _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelTransfer->UpPickUpUDCyl();
	if (iResult != 0)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelTransfer->AbsorbPanel();
	if (iResult != 0)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	/*
	iResult = m_plnkPanelTransfer->DownStopperUDCyl();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}*/

	// Panel Aligner 로 Panel 진입
	iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(DEF_PANEL_ALIGNER_LOAD_POS, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//170928 JSH Gear
	//iResult = m_plnkPanelTransfer->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS, FALSE);
	iResult = m_plnkPanelTransfer->SafeMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkCtrlPanelAligner->PickUpPanel();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelTransfer->ReleasePanel();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	dlg.DoModal();

//	Panel Align 동작을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30335, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	MOlbSystemPre.LightOn(DEF_PANEL_ALIGN_CAMERA_1);
	MOlbSystemPre.LightOn(DEF_PANEL_ALIGN_CAMERA_2);
	Sleep(100);

	iResult = m_plnkCtrlPanelAligner->AlignPanel();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	// selsk_jdy iResult = m_plnkCtrlPanelAligner->MovetoAlignPos();
	iResult = m_plnkCtrlPanelAligner->MovetoAlignPos(FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	dlg.DoModal();

//	Unload Conveyor로 Panel을 배출하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30336, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(DEF_PANEL_ALIGNER_UNLOAD_POS, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelAligner->ReleasePanel();			
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelAligner->DownPickUpUnit();			
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkUnloadConveyor->MoveForwardReducedSpeed();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	tm.StartTimer();
	while (TRUE)
	{
		if (m_plnkUnloadConveyor->IsPanelDetected())
			break;

		if (tm.MoreThan(20))
		{
			m_plnkUnloadConveyor->StopMoving();
//			20초 동안 Unload Conveyor 센서에 Panel 감지가 되지 않았습니다.
			MyMessageBox(MY_MESSAGE_BOX, 30337, _T("Sensor Panel Not Detected"), M_ICONERROR);
			return;
		}
		Sleep(1);
	}
	
	iResult = m_plnkUnloadConveyor->StopMoving();
	if (iResult)
	{
		m_plnkUnloadConveyor->StopMoving();
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
//	Panel 투입, Align, 배출 동작을 완료하였습니다.
	MyMessageBox(MY_MESSAGE_BOX, 30338, _T("Panel Load, Align, Unload Finished"), M_ICONINFORMATION);

#endif

}

int CManualMainForm::alignPanel()
{
#if FALSE
	if (m_plnkPanelAligner->IsPanelAbsorbed() == FALSE)
	{
		MPanelTransfer* plnkPanelTransfer = MOlbSystemPre.GetPanelTransferComponent();
		
		int iResult = m_plnkPanelAligner->AirCVBlowOff();
		if (iResult)
			return iResult;
		
		m_plnkPanelAligner->DownPickUpUnit();
		
		iResult = plnkPanelTransfer->DownPickUpUDCyl();
		if (iResult)
			return iResult;
		
		//170928 JSH Gear
		//iResult = plnkPanelTransfer->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS, FALSE);		
		iResult = plnkPanelTransfer->SafeMoveXPos(DEF_PANEL_TRANSFER_LOAD_POS, FALSE);		
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->AirCVVacAllOn();								
		if (iResult)
			return iResult;
		
		iResult = plnkPanelTransfer->AbsorbPanel(TRUE);							
		if (iResult)
			return iResult;
		
		iResult = plnkPanelTransfer->UpPickUpUDCyl();										
		if (iResult)
			return iResult;
		
		iResult = plnkPanelTransfer->AbsorbPanel();								
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->AirCVBlowOn();							
		if (iResult)
			return iResult;

		//170928 JSH Gear
		//iResult = plnkPanelTransfer->SafeSyncMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS, FALSE);		
		iResult = plnkPanelTransfer->SafeMoveXPos(DEF_PANEL_TRANSFER_UNLOAD_POS, FALSE);		
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->AirCVBlowOff();							
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->AirCVVacAllOn();								
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->DownPickUpUnit();							
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->SafeMoveXYTPos(DEF_PANEL_ALIGNER_LOAD_POS);
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->UpPickUpUnit();							
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->AbsorbPanel();								
		if (iResult)
			return iResult;
		
		iResult = plnkPanelTransfer->ReleasePanel();								
		if (iResult)
			return iResult;
		
		iResult = m_plnkPanelAligner->AirCVBlowOn();							
		if (iResult)
			return iResult;
		
		iResult = plnkPanelTransfer->DownPickUpUDCyl();										
		if (iResult)
			return iResult;
		
		// 코드 확인 필요 : 나중에 이동 Position 개별 적용 필요
		iResult = m_plnkPanelAligner->SafeMoveXYTPos(DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS);	
		if (iResult)
			return iResult;
	}

	if (m_plnkPanelAligner->IsAligned() == TRUE)
		return ERR_MANUAL_MAINFORM_SUCCESS;
	
	return m_plnkCtrlPanelAligner->AlignPanel();
#endif
	return 0;//임시
}

void CManualMainForm::OnClickTabcarrierR() 
{
	CManualTabCarrierDlg dlg(DEF_REAR_GROUP);
	dlg.DoModal();	
}

void CManualMainForm::OnClickTabfeederR() 
{
	CManualTabFeederDlg dlg(DEF_REAR_GROUP);
	dlg.DoModal();	
}

int CManualMainForm::operateTabICInspection(EWorkingSide eWorkSide, int iGroupNo, int iWorkTabNo, int iTargetStatus, MPos_XY* prgAlignOffset)
{
	return ERR_MANUAL_MAINFORM_SUCCESS;
}

void CManualMainForm::OnMountTabIC() 
{
#if FALSE	//임시삭제

	CString strMsg;
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
//		Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
		MyMessageBox(MY_MESSAGE_BOX, 30352, _T("Error"), M_ICONERROR);
		return;
	}

//	TabIC Mount 작업을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30353, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	EWorkingSide eWorkSide = WORKING_SIDE_UNKNOWN;
	int iWorkGroupNo = DEF_FRONT_GROUP;
	int iTabNo = -1;

	int iResult = getWorkOption(&eWorkSide, &iWorkGroupNo, &iTabNo);
	if (iResult)
		return;

	int iWorkTabMounterNo = -1;
	if (-1 != iTabNo)
	{
		iWorkTabMounterNo = m_plnkProcessData->GetWorkTabMounterNo(eWorkSide, iTabNo);
		if (DEF_NONE_TABMOUNTER == iWorkTabMounterNo)
		{
//			선택된 Tab 번호는 작업하지 않는 Tab 번호임.
			MyMessageBox(MY_MESSAGE_BOX, 30354, _T("Tab No select error"), M_ICONERROR);
			return;
		}
	}

	// Panel 준비 상태 확인.
	MPanelAligner* plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
	if (FALSE == plnkPanelAligner->IsPanelAbsorbed())
	{
//		Panel Aligner가 Panel을 흡착하고 있지 않습니다.
		MyMessageBox(MY_MESSAGE_BOX, 30355, _T("Error"), M_ICONERROR);
		return;
	}

	if (FALSE == plnkPanelAligner->IsAligned())
	{
//		Panel Align 이 수행되지 않았습니다.
		MyMessageBox(MY_MESSAGE_BOX, 30356, _T("Error"), M_ICONERROR);
		return;
	}

	// 확인 필요 : 항상 Xu Pos 로 계속 사용할 것인가?
	if (FALSE == plnkPanelAligner->IsInXYTPos(DEF_PANEL_ALIGNER_PRESS_XU_POS))
	{
		iResult = plnkPanelAligner->SafeMoveXYTPos(DEF_PANEL_ALIGNER_PRESS_XU_POS);
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}

	int iWorkCount = 2 + (-1 == iTabNo) ? DEF_MAX_WORKER_PER_GROUP : 1;

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_20"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Mount TabIC..."));
	ProgWnd.SetRange(0, iWorkCount);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();

	iResult = operateTabICInspection(eWorkSide, iWorkGroupNo, iTabNo, DEF_BEFORE_MOUNT);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		if (ERR_MANUAL_MAINFORM_SKIP_ERROR_DISPLAY == iResult)
			return;
		
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	ProgWnd.StepIt();

	MTabMounter* plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iWorkGroupNo]);

	iResult = plnkTabMounter->UpBackupUDCyl(TRUE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = plnkTabMounter->ForwardBackupFBCyl();
	if (iResult)
	{
		ProgWnd.DestroyWindow();
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = plnkTabMounter->UpBackupUDCyl();
	if (iResult)
	{
		ProgWnd.DestroyWindow();
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	ProgWnd.StepIt();

	int iStartMounterNo = -1;
	int iEndMounterNo = -1;
	int iGap = 0;
	
	if (DEF_FRONT_GROUP == iWorkGroupNo)
	{
		iStartMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[iWorkGroupNo];
		iEndMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iWorkGroupNo] - 1;
		iGap = -1;
	}
	else // if (DEF_REAR_GROUP == iGroupNo)
	{
		iStartMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iWorkGroupNo];
		iEndMounterNo = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[iWorkGroupNo] + 1;
		iGap = 1;
	}

	int iTempTabNo = -1;
	MCtrlTabMounter* plnkCtrlTabMounter = NULL;
	for (int i = iStartMounterNo; i != iEndMounterNo; i += iGap)
	{
		if (-1 != iWorkTabMounterNo && i != iWorkTabMounterNo)
		{
			ProgWnd.StepIt();
			continue;
		}

		plnkCtrlTabMounter = MOlbSystemPre.GetCtrlTabMounterComponent(i);
		iTempTabNo = m_plnkProcessData->GetMounterDefaultTabNo(eWorkSide, i);
		if (-1 == iTempTabNo)
		{
			// Mounter 개수 보다 Use Tab 개수가 작은 경우
			// 즉, 사용하지 않는 TabMounter 는 아무 작업없이 그냥 둔다.
			
			ProgWnd.StepIt();
			continue;
		}

		if (i == iWorkTabMounterNo)
		{
			ASSERT(iTempTabNo == iTempTabNo);
		}

		iResult = plnkCtrlTabMounter->MountTabIC(eWorkSide, iTempTabNo);
		if (iResult)
		{
			ProgWnd.DestroyWindow();
// jdy sesl			MyMessageBox(iResult, _T("Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		ProgWnd.StepIt();
	}

	// TabMounter Up 대기
	Sleep(500);	// 확인 필요 : 나중에 확실하게 sensor 확인하도록 수정할 것.

	iResult = plnkTabMounter->DownBackupUDCyl(TRUE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = plnkTabMounter->BackwardBackupFBCyl();
	if (iResult)
	{
		ProgWnd.DestroyWindow();
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = plnkTabMounter->DownBackupUDCyl();
	if (iResult)
	{
		ProgWnd.DestroyWindow();
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	ProgWnd.DestroyWindow();

#endif

//	작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30357, _T("confirm"), M_ICONINFORMATION);	
}

void CManualMainForm::OnClickAutoChangeTabicReel()
{
#if FALSE
//	Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_25"), &strMsg);	
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Reel Auto Change(Front) 작업을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30368, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = autoChangeTabICReel(DEF_TABFEEDER1);
	if (iResult)
	{
		MOlbSystemPre.GetTabFeederComponent(DEF_TABFEEDER1)->WriteAutoChangeLog(_T("AutoChange Error"), iResult);
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Auto Change 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30369, _T("confirm"), M_ICONINFORMATION);
#endif
}

void CManualMainForm::OnClickAutoChangeTabicReel2()
{
#if FALSE
//	Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_27"), &strMsg);
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Reel Auto Change(Rear) 작업을 진행하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30370, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = autoChangeTabICReel(DEF_TABFEEDER2);
	if (iResult)
	{
		MOlbSystemPre.GetTabFeederComponent(DEF_TABFEEDER2)->WriteAutoChangeLog(_T("AutoChange Error"), iResult);
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Auto Change 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30371, _T("confirm"), M_ICONINFORMATION);
#endif
}

int CManualMainForm::autoChangeTabICReel(int iFeederNo)			//@--->미사용 함수
{
//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_27"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("Reel Auto Change..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	int iResult;
	MOlbSystemPre.GetTabFeederComponent(iFeederNo)->ReleasePress();
	iResult = MOlbSystemPre.GetTabFeederComponent(iFeederNo)->SafeMovePressPos(DEF_PRESS_READY_POS);

	MOlbSystemPre.GetTabFeederComponent(iFeederNo)->WriteAutoChangeLog(_T("AutoChange Start"), 0);

	// 사용 완료된 Reel 정보는 제거하고, 대기 중이던 Reel 정보를 사용중 Reel 정보로 이동시킨다.
	// AutoChange 도중 Error 발생한 경우 TabIC Reel 정보가 넘어가지 않는 문제 때문에,
	// 먼저 Data 를 넘기고 AutoChange 를 시작한다.
	// 적어도 현재 사용 중이던 Reel 은 버릴 수 있을 것이므로, Data 를 먼저 넘긴다.
	MOlbSystemPre.GetCtrlTabFeederComponent(iFeederNo)->ChangeSpareReelInfotoUsingReel();

	iResult = MOlbSystemPre.GetCtrlTabFeederComponent(iFeederNo)->ExecuteReelAutoChange();
	if (iResult)
		return iResult;

#ifndef DEF_SETUP
	iResult = MOlbSystemPre.GetCtrlTabFeederComponent(iFeederNo)->ExecuteAutoHoming(TRUE);
	if (iResult)
		return iResult;
#else
	for (int i = 0; i < 2; i++)
	{
		iResult = MOlbSystemPre.GetCtrlTabFeederComponent(iFeederNo)->FeedTabIC(2);
		if (iResult)
			return iResult;
	}

//	Setup 기간 중에는 AutoHoming 동작을 생략합니다.
	MyMessageBox(MY_MESSAGE_BOX, 30372, _T("Information"), M_ICONINFORMATION);
#endif

	MOlbSystemPre.GetTabFeederComponent(iFeederNo)->WriteAutoChangeLog(_T("AutoChange End"), 0);
	return ERR_MANUAL_MAINFORM_SUCCESS;
}

void CManualMainForm::OnHeaterPowerReset()
{
	IIO* plnkIO = MOlbSystemPre.GetIOComponent();

	int iResult = plnkIO->OutputOn(OUT_HEATER_POWER_RESET);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

		plnkIO->OutputOff(OUT_HEATER_POWER_RESET);
		return;
	}

//	Heater Power Reset 출력 점멸 중 입니다.\n잠시만 기다려 주세요...
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_28"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("Heater Power Reset..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	Sleep(1000);

	plnkIO->OutputOff(OUT_HEATER_POWER_RESET);
	ProgWnd.DestroyWindow();

//	Heater Power Reset 출력 점멸 완료.
	MyMessageBox(MY_MESSAGE_BOX, 30373, _T("Information"), M_ICONINFORMATION);
}

void CManualMainForm::OnSercosInitialize()
{
#ifndef SIMULATION
	CMyProgressWnd ProgWnd(this, _T("Sercos Network Initializing..."));
	ProgWnd.SetRange(0, DEF_AXIS_MAX_PRIORITY);
	ProgWnd.SetText(_T("...is executing Sercos Network Initialization... Wait a moment..."));
	ProgWnd.PeekAndPump();

//	MMC2 초기화(Phase4)를 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30374, _T("MMC2 Initialize"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = ERR_DLG_ORIGIN_SUCCESS;

	iResult = MOlbSystemPre.GetSercosSetupComponent()->SercosSetup();

	ProgWnd.DestroyWindow();
	if(iResult != ERR_DLG_ORIGIN_SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Initialize Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	초기화가 완료되었습니다.
	MyMessageBox(MY_MESSAGE_BOX, 30375, _T("Initialize Success"), M_ICONINFORMATION);
#endif
}

void CManualMainForm::OnClickCameraExpandShrink() 
{
	CManualCameraExpShrDlg dlg;
	dlg.DoModal();	
}

#if FALSE
int CManualMainForm::moveGateInspectionCameraToSafePosition(EWorkingSide eWorkSide, int iGroupNo)
{
#ifdef DEF_GATE_SYSTEM
	// 주의 : 이 함수는 Gate 변 작업 Tab 이 한 개인 경우는 대응 못함.
	
	int i = 0;
	int iResult = SUCCESS;
	MCameraCarrier* rgCamCarrier[DEF_MAX_INSPECTION_CAM_PER_GROUP] = { NULL, NULL, NULL };
	int rgiWorkTabNo[DEF_MAX_INSPECTION_CAM_PER_GROUP] = { -1, -1, -1 };
	double rgdTargetPos[DEF_MAX_INSPECTION_CAM_PER_GROUP] = { -9999.0, -9999.0, -9999.0 };

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
	{
		rgCamCarrier[i] = MOlbSystemPre.GetCameraCarrierComponent(DEF_CAMERACARRIER3 + i + iGroupNo * DEF_MAX_INSPECTION_CAM_PER_GROUP);

		rgiWorkTabNo[i] = m_plnkProcessData->GetInspectorDefaultTabNo(eWorkSide, i + iGroupNo * DEF_MAX_INSPECTION_CAM_PER_GROUP);

		if (-1 != rgdTargetPos[i])
			rgdTargetPos[i] = rgCamCarrier[i]->GetTargetPos(DEF_CAMERACARRIER_TAB_MARK_POS, rgiWorkTabNo[i]);
		else
		{
			ASSERT((i + 1 + iGroupNo * DEF_MAX_INSPECTION_CAM_PER_GROUP) % DEF_MAX_INSPECTION_CAM_PER_GROUP == 0);
			rgdTargetPos[i] = rgCamCarrier[i]->GetMotionComponent()->GetPositiveSWLimit() - 5.0;
		}
	}

	// 이동 순서 주의
	int rgiWorkOrder[DEF_MAX_INSPECTION_CAM_PER_GROUP] = { 0, 2, 1 };

	int j = 0;
	for (j = 0; j < DEF_MAX_INSPECTION_CAM_PER_GROUP; j++)
	{
		iResult = rgCamCarrier[rgiWorkOrder[j]]->MovePos(rgdTargetPos[rgiWorkOrder[j]], -1);
		if (iResult)
			return iResult;
	}
#endif
	return SUCCESS;
}
#endif

void CManualMainForm::OnCheckPunchRepeatability()
{	
	CString strMsg;
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
//		Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
		MyMessageBox(MY_MESSAGE_BOX, 30376, _T("Error"), M_ICONERROR);
		return;
	}
	
	int iResult = MyMessageBox(MY_MESSAGE_BOX, 30390, _T("confirm"), M_ICONQUESTION|M_YESNO);
	if (IDYES != iResult)
		return;

	int iWorkGroup;
	iResult = MyMessageBox(_T("What feeder do you want to eject the Tab IC?\r\nTab IC를 어떤 Feeder 에서 타발 하시겠습니까?"), _T("confirm"), M_ICONQUESTION|M_YESNO, _T("Front"), _T("Rear"));
	if (IDYES == iResult)
	{
		iWorkGroup = DEF_FRONT_GROUP;
	}
	else
	{
		iWorkGroup = DEF_REAR_GROUP;
	}

	int nRetryCnt = 0;
	WCHAR m_cbuf[100];
	WCHAR m_cBufGet[100];
	memset(m_cBufGet, 0, sizeof(m_cBufGet));
	if (GetNumberBox(m_cBufGet, 10, m_cbuf, _T("타발횟수를 입력하세요."), FALSE))
		nRetryCnt = _wtoi(m_cBufGet);
	else
		return;

	if(nRetryCnt < 1 || nRetryCnt > 100 )
	{
		MyMessageBox(_T("WRONG NUMBER"),_T("1 ~ 100"),M_ICONINFORMATION);
		return;
	}

//		일련 동작 중 입니다.\n잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_29"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabFeeder punching tabic..."));
	ProgWnd.SetRange(0, nRetryCnt+1);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	for (int i = 0; i < nRetryCnt; i++)
	{
		if (MOlbSystemPre.GetIOComponent()->IsOn(IN_FRONT_STEP_STOP_SW)
			|| MOlbSystemPre.GetIOComponent()->IsOn(IN_REAR_STEP_STOP_SW))
		{
			MyMessageBox(MY_MESSAGE_BOX, 30395, _T("confirm"), M_ICONINFORMATION);
			return;
		}
		iResult = MOlbSystemPre.GetCtrlTabFeederComponent(iWorkGroup)->FeedTabIC();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = MOlbSystemPre.GetCtrlTabFeederComponent(iWorkGroup)->PressTapeStep1();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = MOlbSystemPre.GetCtrlTabFeederComponent(iWorkGroup)->PressTapeStep2();
		if (iResult)
		{
			ProgWnd.DestroyWindow();
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		ProgWnd.StepIt();
	}

	ProgWnd.DestroyWindow();

	//	작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30380, _T("confirm"), M_ICONINFORMATION);
}

int CManualMainForm::checkTabPunchRepeatability(EWorkingSide eWorkSide, MCameraCarrier* plnkCamCarrier, int iTabNo)
{
#if FALSE	//임시삭제

	ASSERT(DRY_RUN_MODE != m_plnkSystemData->m_eRunMode);

	const int iCamNo = plnkCamCarrier->GetOwnedCameraNo();
	int iPairCamNo = -1;
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		switch (iCamNo)
		{
		case DEF_F_INSPECTION_CAMERA_1:
			iPairCamNo = DEF_F_INSPECTION_CAMERA_2;
			break;
		case DEF_F_INSPECTION_CAMERA_3:
			iPairCamNo = DEF_F_INSPECTION_CAMERA_4;
			break;
		default:
			break;
		}
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// parameter 유효성 검증, 사용 Data 준비(Mark 번호 etc.)
	int iMaxTabNo = m_plnkProcessData->GetMaxTabQty(eWorkSide);
	if (iTabNo < 0 || iTabNo >= iMaxTabNo)
		return FAILURE;

	int rgiModel[] = { -1, -1, -1, -1 };
	rgiModel[0] = DEF_PANEL_LEAD_XD_MARK1;
	rgiModel[1] = DEF_PANEL_LEAD_XD_MARK2;
	rgiModel[2] = DEF_TAB_LEAD_XU_MARK1 + int(eWorkSide) * 2;
	rgiModel[3] = DEF_TAB_LEAD_XU_MARK2 + int(eWorkSide) * 2;

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Unit 상태 확인은 무시!

	// TabMounter 상태
	// 1. TabIC 흡착
	// 2. Mount 위치 (Mounter 축)
	// 3. Mounter 높이 (DownReady)
	// 4. Backup 위치 (후진, 하강)

	// CameraCarrier 상태
	// 1. CameraCarrier 위치

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Vision 인식
	m_plnkVision->LockCamera(iCamNo);
	if (-1 != iPairCamNo)
		m_plnkVision->LockCamera(iPairCamNo);

	m_plnkVision->ClearOverlay(iCamNo);
	if (-1 != iPairCamNo)
		m_plnkVision->ClearOverlay(iPairCamNo);
	
	int iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iCamNo);
		if (-1 != iPairCamNo)
			m_plnkVision->UnlockCamera(iPairCamNo);

		return iResult;
	}

	if (-1 != iPairCamNo)
	{
		iResult = m_plnkVision->Grab(iPairCamNo);
		if (iResult)
		{
			m_plnkVision->UnlockCamera(iCamNo);
			if (-1 != iPairCamNo)
				m_plnkVision->UnlockCamera(iPairCamNo);

			return iResult;
		}
	}

	// Panel Tab Lead Mark 인식
	int rgiCamNo[2] = { iCamNo, iCamNo };
	if (-1 != iPairCamNo)
		rgiCamNo[1] = iPairCamNo;
	
	int i;
	for (int i = 0; i < 2; i++)
	{
		// Prism 사용하는 경우, TabIC Mark 와 Edge 가 한 화면에 보이지 않을 수 있으므로 별도 처리 필요.
		if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
		{
			double dMoveOffset = 0.5;
			double dCurrentPos = plnkCamCarrier->GetCurrentPos();
			double dTarget = (0 == i) ? dCurrentPos - dMoveOffset : dCurrentPos + dMoveOffset;

			iResult = plnkCamCarrier->MovePos(dTarget, DEF_CAMERACARRIER_TAB_MARK_POS);
			if (iResult)
				return iResult;

			Sleep(100);

			iResult = m_plnkVision->Grab(rgiCamNo[i]);
			if (iResult)
			{
				m_plnkVision->UnlockCamera(iCamNo);
				if (-1 != iPairCamNo)
					m_plnkVision->UnlockCamera(iPairCamNo);
				
				return iResult;
			}
		}
		
		// TabIC Edge Mark 인식.
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[i]);
		if (iResult)
		{
			m_plnkVision->UnlockCamera(iCamNo);
			if (-1 != iPairCamNo)
				m_plnkVision->UnlockCamera(iPairCamNo);
			
			return iResult;
		}

		// TabIC Lead Mark 인식
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[2 + i]);
		if (iResult)
		{
			m_plnkVision->UnlockCamera(iCamNo);
			if (-1 != iPairCamNo)
				m_plnkVision->UnlockCamera(iPairCamNo);

			return iResult;
		}
	}

	MPos_XY rgRxy[4];
	MPos_XY rgVxy[4];	// Vision 인식 좌표 임시 저장 변수
	int iPrizmSide = 0;	// OLB 설비 좌표계 축 기준으로 작은 쪽이 0, 큰 쪽이 1 이다.

	for (int i = 0; i < 4; i++)
	{
		// 전 Unit 에서 동일하게 OLB 설비 좌표계 기준으로 Vision 인식 좌표를 얻는다.		
		rgVxy[i].dX = m_plnkVision->GetSearchResultX(rgiCamNo[i % 2], rgiModel[i]);
		rgVxy[i].dY = m_plnkVision->GetSearchResultY(rgiCamNo[i % 2], rgiModel[i]);
		if (-1 == iPairCamNo)
			iPrizmSide = i % 2;
		
		MVisionCalibration* plnkCalib = MOlbSystemPre.GetVisionCalibrationComponent();
		plnkCalib->ConvertV2R(rgiCamNo[i % 2], iPrizmSide, rgVxy[i].dX, rgVxy[i].dY, rgRxy[i].dX, rgRxy[i].dY);
	}

	m_plnkVision->UnlockCamera(iCamNo);
	if (-1 != iPairCamNo)
		m_plnkVision->UnlockCamera(iPairCamNo);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Vision 인식 결과 계산 (양측 X,Y 편차 계산)
	
	double dDiff1X = rgRxy[2].dX - rgRxy[0].dX;	// Mark.X - Edge.X
	double dDiff1Y = rgRxy[2].dY - rgRxy[0].dY; // Mark.Y - Edge.Y

	double dDiff2X = rgRxy[1].dX - rgRxy[3].dX;	// Edge.X - Mark.X
	double dDiff2Y = rgRxy[3].dY - rgRxy[1].dY; // Mark.Y - Edge.Y

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
	{
		CString strLog;
		strLog.Format(_T("ws(%d),tab(%02d),t_e1(,%.4f,%.4f,),t_e2(,%.4f,%.4f,),t_m1(,%.4f,%.4f,),t_m2(,%.4f,%.4f,),diff1(,%.4f,%.4f,),diff2(,%.4f,%.4f,)"),
			eWorkSide, iTabNo,
			rgRxy[0].dX, rgRxy[0].dY, rgRxy[1].dX, rgRxy[1].dY, rgRxy[2].dX, rgRxy[2].dY, rgRxy[3].dX, rgRxy[3].dY,
			dDiff1X, dDiff1Y, dDiff2X, dDiff2Y);
		
		m_plnkVision->WriteLog(_T("..\\Log\\TabPunchRepeatability_log.csv"), (LPCTSTR)strLog);
	}

#endif

	return SUCCESS;
}

void CManualMainForm::OnClickSetupTest() 
{
	// hongju...20080829
	// 1. Mount를 측정하기 원하는 Camera 위치로 이동 함.
	// 2. Mount의 번호를 입력 받음
	// 3. Camera 번호를 입력 받음.
	// 4. Down 후 Vision 촬상 후 값 저장
	// 5. Up 후 Vision 촬상 후 값 저장.
	CString strMsg;
	int iResult = SUCCESS;
	int iMountNum, iCameraNum;
	WCHAR cAfterVal[10];
	int iMarkModel = DEF_TAB_LEAD_XU_MARK1;
	MPos_XY rgRxy, rgRxy2;
	MPos_XY rgVxy, rgVxy2;	// Vision 인식 좌표 임시 저장 변수

//	Mount를 측정 가능한 위치로 이동시키세요.
	if (MyMessageBox(MY_MESSAGE_BOX, 30381, _T("comfirm"), M_ICONQUESTION|M_YESNO, _T("Ok"), _T("Cancel")) == IDNO)
		return;

//	#pragma message(__LOC__ "Front/Rear Mount선택, Camera선택 수정하기")

//	Mount의 번호를 입력하세요.
	if (MyMessageBox(MY_MESSAGE_BOX, 30382, _T("comfirm"), M_ICONQUESTION|M_YESNO, _T("Ok"), _T("Cancel")) == IDNO)
		return;
	else
	{
		memset(cAfterVal, 0, sizeof(cAfterVal));

		if (FALSE == GetNumberBox(cAfterVal, 10, _T("1")))
			return ;
		
		iMountNum = _wtoi(cAfterVal);
		iMountNum -= 1;
		if (iMountNum < 0 || iMountNum >= DEF_MAX_TABMOUNTER)
		{
			MyMessageBox(MY_MESSAGE_BOX, 30391, _T("comfirm"));
			return;
		}
	}

//	Camera 번호를 입력하세요.
	if (MyMessageBox(MY_MESSAGE_BOX, 30383, _T("comfirm"), M_ICONQUESTION|M_YESNO, _T("Ok"), _T("Cancel")) == IDNO)
		return;
	else
	{
		memset(cAfterVal, 0, sizeof(cAfterVal));

		if (FALSE == GetNumberBox(cAfterVal, 10, _T("5")))
			return ;
		
		iCameraNum = _wtoi(cAfterVal);
		iCameraNum -= 1;
		if (iCameraNum < 4 || iCameraNum >= 12)
		{
			MyMessageBox(MY_MESSAGE_BOX, 30391, _T("comfirm"));
			return;
		}
		if (iCameraNum % 2 == 1)
			iMarkModel = DEF_TAB_LEAD_XU_MARK2;
	}

//	MOlbSystemPre.GetTabMounterComponent(iMountNum)->DownReady(TRUE,TRUE);
	iResult = MOlbSystemPre.GetTabMounterComponent(iMountNum)->DownCyl();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);				
		return;
	}
	MOlbSystemPre.GetTabMounterComponent(iMountNum)->SafeMoveZPos(DEF_TABMOUNTER_Z_INSPECTION_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);				
		return;
	}
	Sleep(500);

	m_plnkVision->LockCamera(iCameraNum);
	m_plnkVision->ClearOverlay(iCameraNum);
	m_plnkVision->Grab(iCameraNum);

	// Mark 인식.
	iResult = m_plnkVision->RecognitionPatternMark(iCameraNum, iMarkModel);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iCameraNum);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);				
		return;
	}

	rgVxy.dX = m_plnkVision->GetSearchResultX(iCameraNum, iMarkModel);
	rgVxy.dY = m_plnkVision->GetSearchResultY(iCameraNum, iMarkModel);
	
	MVisionCalibration* plnkCalib = MOlbSystemPre.GetVisionCalibrationComponent();
	plnkCalib->ConvertV2R(iCameraNum, 0, rgVxy.dX, rgVxy.dY, rgRxy.dX, rgRxy.dY);

	m_plnkVision->UnlockCamera(iCameraNum);

	Sleep(500);

//	MOlbSystemPre.GetTabMounterComponent(iMountNum)->Up(TRUE);
//	Sleep(1000);
	iResult = MOlbSystemPre.GetTabMounterComponent(iMountNum)->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iCameraNum);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);				
		return;
	}

//	MyMessageBox(MY_MESSAGE_BOX, 30384, _T("Confirm"), M_ICONINFORMATION);
	Sleep(500);

	m_plnkVision->LockCamera(iCameraNum);
	m_plnkVision->ClearOverlay(iCameraNum);
	m_plnkVision->Grab(iCameraNum);

	// Mark 인식.
	iResult = m_plnkVision->RecognitionPatternMark(iCameraNum, iMarkModel);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iCameraNum);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);				
		return;
	}

	rgVxy2.dX = m_plnkVision->GetSearchResultX(iCameraNum, iMarkModel);
	rgVxy2.dY = m_plnkVision->GetSearchResultY(iCameraNum, iMarkModel);
	
	plnkCalib = MOlbSystemPre.GetVisionCalibrationComponent();
	plnkCalib->ConvertV2R(iCameraNum, 0, rgVxy2.dX, rgVxy2.dY, rgRxy2.dX, rgRxy2.dY);

	m_plnkVision->UnlockCamera(iCameraNum);

	CString strLog;
	strLog.Format(_T("Mount(%d),Camera(%d),Before_V(,%.4f,%.4f,),Before_R(,%.4f,%.4f,),After_V(,%.4f,%.4f,),After_R(,%.4f,%.4f,)"),
		iMountNum, iCameraNum,
		rgVxy.dX, rgVxy.dY, rgRxy.dX, rgRxy.dY, rgVxy2.dX, rgVxy2.dY, rgRxy2.dX, rgRxy2.dY);
	
	m_plnkVision->WriteLog(_T("..\\Log\\Mount_Camera_log.csv"), (LPCTSTR)strLog);

//	완료
//	MyMessageBox(MY_MESSAGE_BOX, 30385, _T("Confirm"), M_ICONINFORMATION);
	MyMessageBox(strLog, _T("Down/Up Recog"));
}

LRESULT CManualMainForm::OnDisplayLanguage(UINT wParam, LPARAM lParam)
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
//	I/O 동작
	SetTitleLT(&m_lblIOJob, MAIN_MANUAL, _T("ManualMainForm_1"));
//	수동 초기화
	SetTitleLT(&m_lblInitJob, MAIN_MANUAL, _T("ManualMainForm_2"));
//	일련 동작
	SetTitleLT(&m_lblBatchJob, MAIN_MANUAL, _T("ManualMainForm_3"));
//	단위 동작
	SetTitleLT(&m_lblUnitJob, MAIN_MANUAL, _T("ManualMainForm_4"));
	
	CString szTextTemp;
//	입/출력 확인
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_5"), &szTextTemp);
	GetDlgItem(IDB_IO_CHECK)->SetWindowText(szTextTemp);
//	Limit 센서\r\n입력 확인
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_6"), &szTextTemp);
	GetDlgItem(IDB_LIMIT_CHECK)->SetWindowText(szTextTemp);
//	축 원점복귀
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_7"), &szTextTemp);
	GetDlgItem(IDB_RETURN_ORIGIN)->SetWindowText(szTextTemp);
//	Panel\r\n투입, 정렬, 배출
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_8"), &szTextTemp);
	GetDlgItem(IDB_PANEL_ALIGN)->SetWindowText(szTextTemp);
//	Motion Network\r\n초기화
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_30"), &szTextTemp);
	GetDlgItem(IDB_SERCOS_INITIALIZE)->SetWindowText(szTextTemp);

	return 0;
}

void CManualMainForm::OnClickEngStop() 
{
	// TODO: Add your control notification handler code here
	if (MAKER_GROUP != MOlbSystemPre.GetCurrentUserGroup())
	{
	//Maker 권한으로만 수정 할 수 있습니다.
		MyMessageBox(MY_MESSAGE_BOX, 50328, _T("Login Authority Information"), M_ICONINFORMATION);

		return;
	}

//	CEqStopDlg dlg;
//	dlg.DoModal();

	CEqStopCodeDlg dlg;
	dlg.DoModal();
}

void CManualMainForm::OnClickPanelTransferCenter() 
{
#ifdef DEF_USE_TRANSFER_CENTER
	// TODO: Add your control notification handler code here
	CManualPanelTransferDlg dlg(DEF_PANEL_TRANSFER_CENTER);
	dlg.DoModal();	
#endif
}

void CManualMainForm::OnClickPanelAligner3() 
{
	// TODO: Add your control notification handler code here
	CManualPanelTransferDlg dlg(DEF_PANEL_TRANSFER_OUT);
	dlg.DoModal();
}

void CManualMainForm::OnClickTabicLoadCarrier()
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

	//Front/Rear 구분 용 Flg
	BOOL m_bFlgUseFront = FALSE;
	BOOL m_bFlgUseRear = FALSE;

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

	iResult = MyMessageBox(_T("What feeder do you want to eject the Tab IC?\r\nTab IC를 어떤 Feeder 에서 타발 하시겠습니까?"), _T("confirm"), M_ICONQUESTION|M_YESNO, _T("Front"), _T("Rear"));
	if (IDYES == iResult)
	{
		for(i = 0 ; i< DEF_MAX_TABCARRIER; i++)
		{
			m_plnkWorkTabCarrier[i] = MOlbSystemPre.GetTabCarrierComponent(i);
			m_plnkCtrlWorkTabCarrier[i] = MOlbSystemPre.GetCtrlTabCarrierComponent(i);
		}

		iWorkGroupNo = DEF_FRONT_GROUP;
		m_plnkCtrlWorkTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(0);

		m_bFlgUseFront = TRUE;
		m_bFlgUseRear = FALSE;
	}
	else
	{
		for(i = 0 ; i< DEF_MAX_TABCARRIER; i++)
		{
			m_plnkWorkTabCarrier[i] = MOlbSystemPre.GetRTabCarrierComponent(i);
			m_plnkCtrlWorkTabCarrier[i] = MOlbSystemPre.GetCtrlRTabCarrierComponent(i);
		}

		iWorkGroupNo = DEF_REAR_GROUP;
		m_plnkCtrlWorkTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(1);

		m_bFlgUseFront = FALSE;
		m_bFlgUseRear = TRUE;
	}

	MyMessageBox(_T("INPUT CARRIERNUMBER"),_T("INPUT"),M_ICONINFORMATION);
	if (GetNumberBox(m_cBufGet, 10, m_cbuf))
	{
		iCarrierNo = _wtoi(m_cBufGet);
	}
	iCarrierNo--;
	if(iCarrierNo < 0 || iCarrierNo >= DEF_MAX_TABCARRIER )
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

	
	//20131018 SJ_HJG
	//선택한 IC가 TabIC를 가지고 있는 경우
	if(m_plnkWorkTabCarrier[iCarrierNo]->IsAbsorbTabIC() == TRUE)
	{
		iResult = MyMessageBox(_T("The selected Tab Carrier is adsorbing the Tab IC. \n Do you want to remove the TabIC being adsorbed and proceed?\r\n선택한 Tab Carrier가 Tab IC를 흡착 하고 있습니다.\n흡착중인 TabIC를 제거 하고 진행 하시겠습니까?"), _T("confirm"), M_ICONQUESTION|M_YESNO, _T("TabIC 제거"), _T("동작 중지"));
		if (IDYES == iResult)
		{
			iResult = MOlbSystemPre.RemoveTabIC(iWorkGroupNo, TRUE);
			if (iResult != SUCCESS && iResult != ERR_SKIP_ERROR_DISPLAY)
			{
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}
		else
			return;

	}
	//_______________

	for (int i = DEF_MAX_TABCARRIER-1; i>iCarrierNo; i--)
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

	for (int i = 0; i < iCarrierNo; i++)
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
	if (FALSE == m_plnkWorkTabCarrier[iCarrierNo]->IsAbsorbTabIC())
	{
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
	}
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



	//130306.kms________________
	//Vision 창 Display.......
	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
	//전후면 구분 하여  Camera 선택
	//전면 
	if(m_bFlgUseFront == TRUE && m_bFlgUseRear == FALSE)
		m_iCamNo = DEF_F_TABALIGNER_CAMERA_1;
	//후면
	else if(m_bFlgUseFront == FALSE && m_bFlgUseRear == TRUE)
		m_iCamNo = DEF_R_TABALIGNER_CAMERA_1;
	else
		m_iCamNo = DEF_F_TABALIGNER_CAMERA_1;

	// Operation Type = DEF_PM_MARK / DEF_BLOB_MARK / DEF_CALIPER_MARK
	CVisionMonitorDlg dlg(plnkVision, DEF_PM_MARK, m_iCamNo);
	dlg.DoModal();
	//__________________________

	Sleep(500);

	if (MyMessageBox(_T("타발 길이를 조정 하시겠습니까?"), _T("confirm"), M_ICONQUESTION|M_YESNO) == IDYES)
	{
		if(m_iCamNo == DEF_F_TABALIGNER_CAMERA_1)
		{
			CTeachBadTabDetectorDlg dlg(DEF_MODEL_MODE, DEF_FRONT_GROUP);
			dlg.DoModal();
		}
		else
		{
			CTeachBadTabDetectorDlg dlg(DEF_MODEL_MODE, DEF_REAR_GROUP);
			dlg.DoModal();
		}
	}
}

void CManualMainForm::OnClickTiltingJob() 
{
	// TODO: Add your control notification handler code here
	/*/
	CManualTiltDlg dlg;
	dlg.DoModal();
	/*/
}

void CManualMainForm::OnClickFrontTabmounterMoveToLoadPos()
{
	// TODO: Add your control notification handler code here
//	작업변의 TabMounter를 Load 위치로 이동 시키겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30358, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;

	CString strMsg;
	double tgPos[DEF_MOUNTER_WORK_PER_GROUP];
	/*/
#ifdef DEF_SOURCE_SYSTEM
	int iPosID = DEF_TABMOUNTER_LOAD_POS;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_32"), &strMsg);
	CSystemThreeBtnDlg dlg(strMsg, "ReadyPos", "LoadPos", "RLoadPos");
	iResult = dlg.DoModal();
	if (iResult == 0)
		iPosID = DEF_TABMOUNTER_READY_POS;
	else if (iResult == 1)
		iPosID = DEF_TABMOUNTER_LOAD_POS;
	else
		iPosID = DEF_TABMOUNTER_R_LOAD_POS;

	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		tgPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(iPosID);
	}
#else
	/*/
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		tgPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
	}
//@#endif

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_21"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Move To Load Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.MoveAllFrontTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Load 위치로 이동 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30360, _T("confirm"), M_ICONINFORMATION);	
}

void CManualMainForm::OnClickRearTabmounterMoveToLoadPos() 
{
	// TODO: Add your control notification handler code here
//	작업변의 TabMounter를 Load 위치로 이동 시키겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30358, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;

	CString strMsg;
	double tgPos[DEF_MOUNTER_WORK_PER_GROUP];
	/*/
#ifdef DEF_SOURCE_SYSTEM
	int iPosID = DEF_TABMOUNTER_LOAD_POS;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_32"), &strMsg);
	CSystemThreeBtnDlg dlg(strMsg, "ReadyPos", "LoadPos", "RLoadPos");
	iResult = dlg.DoModal();
	if (iResult == 0)
		iPosID = DEF_TABMOUNTER_READY_POS;
	else if (iResult == 1)
		iPosID = DEF_TABMOUNTER_LOAD_POS;
	else
		iPosID = DEF_TABMOUNTER_R_LOAD_POS;

	for (int i=DEF_MAX_TABMOUNTER-1; i>=DEF_MOUNTER_WORK_PER_GROUP; i--)
	{	//i			: 7		6		5		4
		//workindex : 0		1		2		3
		int workindex = (DEF_MAX_TABMOUNTER -1) - i;
		tgPos[workindex] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(iPosID);
	}
#else
	/*/
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		tgPos[i] = MOlbSystemPre.GetRTabMounterComponent(i)->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
	}
//@#endif

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_21"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Move To Load Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.MoveAllRearTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Load 위치로 이동 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30360, _T("confirm"), M_ICONINFORMATION);	
}

void CManualMainForm::OnClickFrontTabcarrierMoveToReadyPos() 
{
	// TODO: Add your control notification handler code here
//	작업변의 TabMounter를 Load 위치로 이동 시키겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30392, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_21"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabCarrier Move To Ready Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	MPos_XYT tgPos[DEF_MAX_TABCARRIER];
	for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
	{			
		tgPos[i] = MOlbSystemPre.GetTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_READY_POS);
	}
	iResult = MOlbSystemPre.MoveAllFrontTabCarrier(tgPos, DEF_TABCARRIER_READY_POS);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Load 위치로 이동 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30394, _T("confirm"), M_ICONINFORMATION);
}

void CManualMainForm::OnClickRearTabcarrierMoveToReadyPos() 
{
	// TODO: Add your control notification handler code here
//	작업변의 TabMounter를 Load 위치로 이동 시키겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30392, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_21"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabCarrier Move To Ready Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	MPos_XYT tgPos[DEF_MAX_TABCARRIER];
	for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
	{			
		tgPos[i] = MOlbSystemPre.GetRTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_READY_POS);
	}
	iResult = MOlbSystemPre.MoveAllRearTabCarrier(tgPos, DEF_TABCARRIER_READY_POS);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Load 위치로 이동 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30394, _T("confirm"), M_ICONINFORMATION);	
}

void CManualMainForm::OnClickFrontTabcarrierMoveToUnloadPos() 
{
	// TODO: Add your control notification handler code here
//	작업변의 TabMounter를 Load 위치로 이동 시키겠습니까?
//@#ifdef DEF_SOURCE_SYSTEM
//@	if (MyMessageBox(MY_MESSAGE_BOX, 30393, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
//@#else
	if (MyMessageBox(MY_MESSAGE_BOX, 30397, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
//@#endif
		return;
	
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_21"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabCarrier Move To Ready Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	MPos_XYT tgPos[DEF_MAX_TABCARRIER];
	int iPosID;
//@#ifdef DEF_SOURCE_SYSTEM
//@	iPosID = DEF_TABCARRIER_UNLOAD_POS;
//@#else
	iPosID = DEF_TABCARRIER_UNLOAD_POS;
//@#endif
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{			
		tgPos[i] = MOlbSystemPre.GetTabCarrierComponent(i)->GetTargetXYTPos(iPosID);
	}
	iResult = MOlbSystemPre.MoveAllFrontTabCarrier(tgPos, iPosID);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Load 위치로 이동 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30394, _T("confirm"), M_ICONINFORMATION);		
}

void CManualMainForm::OnClickRearTabcarrierMoveToUnloadPos() 
{
	// TODO: Add your control notification handler code here
//	작업변의 TabMounter를 Load 위치로 이동 시키겠습니까?
//@#ifdef DEF_SOURCE_SYSTEM
//@	if (MyMessageBox(MY_MESSAGE_BOX, 30393, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
//@#else
	if (MyMessageBox(MY_MESSAGE_BOX, 30397, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
//@#endif
		return;
	
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_21"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabCarrier Move To Ready Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	MPos_XYT tgPos[DEF_MAX_TABCARRIER];
	int iPosID;
//@#ifdef DEF_SOURCE_SYSTEM
//@	iPosID = DEF_TABCARRIER_UNLOAD_POS;
//@#else
	iPosID = DEF_TABCARRIER_UNLOAD_POS;
//@#endif
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{			
		tgPos[i] = MOlbSystemPre.GetRTabCarrierComponent(i)->GetTargetXYTPos(iPosID);
	}
	iResult = MOlbSystemPre.MoveAllRearTabCarrier(tgPos, iPosID);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Load 위치로 이동 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30394, _T("confirm"), M_ICONINFORMATION);	
}

void CManualMainForm::OnClickTabicLoadAllTabmounter() 
{
	// TODO: Add your control notification handler code here
	CString strMsg;
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
//		Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
		MyMessageBox(MY_MESSAGE_BOX, 30376, _T("Error"), M_ICONERROR);
		return;
	}

	if (MyMessageBox(MY_MESSAGE_BOX, 30396, _T("confirm"), M_ICONQUESTION|M_YESNO) != IDYES)
		return;

	int iResult;
// #ifdef DEF_SOURCE_SYSTEM
// 	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_33"), &strMsg);
// 	iResult = MyMessageBox(strMsg, _T("Select Carrier"), M_ICONQUESTION|M_YESNO, _T("Front"), _T("Rear"));
// 	int iCarrierGroup = (iResult == IDYES) ? DEF_FRONT_GROUP : DEF_REAR_GROUP;	
// #else
// 	int iCarrierGroup = DEF_FRONT_GROUP;
// #endif

	int iCarrierGroup = DEF_FRONT_GROUP;
	iResult = tabICLoadAllTabMounter(DEF_FRONT_GROUP, iCarrierGroup);
	if (iResult)
	{
		if (iResult != ERR_SKIP_ERROR_DISPLAY)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
		return;
	}
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_31"), &strMsg);
	MyMessageBox(strMsg, _T("Complete"));
}

void CManualMainForm::OnClickTabicLoadAllTabmounterRear() 
{
	// TODO: Add your control notification handler code here
	CString strMsg;
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
//		Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
		MyMessageBox(MY_MESSAGE_BOX, 30376, _T("Error"), M_ICONERROR);
		return;
	}

	if (MyMessageBox(MY_MESSAGE_BOX, 30396, _T("confirm"), M_ICONQUESTION|M_YESNO) != IDYES)
		return;

	int iResult;
#ifdef DEF_SOURCE_SYSTEM
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_33"), &strMsg);
	iResult = MyMessageBox(strMsg, _T("Select Carrier"), M_ICONQUESTION|M_YESNO, _T("Front"), _T("Rear"));
	int iCarrierGroup = (iResult == IDYES) ? DEF_FRONT_GROUP : DEF_REAR_GROUP;
#else
	int iCarrierGroup = DEF_REAR_GROUP;
#endif
	iResult = tabICLoadAllTabMounter(DEF_REAR_GROUP, iCarrierGroup);
	if (iResult)
	{
		if (iResult != ERR_SKIP_ERROR_DISPLAY)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
		return;
	}
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_31"), &strMsg);
	MyMessageBox(strMsg, _T("Complete"));
}

int CManualMainForm::tabICLoadAllTabMounter(int iWorkGroup, int iCarrierWorkGroup)
{	
	int i = 0;
	int iResult = 0;
	CString strMsg;
	MTabCarrier* pTabCarrier[DEF_MAX_TABCARRIER];
	MTabMounter* pTabMounter[DEF_MAX_TABMOUNTER];
	MTabMounter* pTabMounterR[DEF_MAX_TABMOUNTER];
	MCtrlTabCarrier* pCtrlTabCarrier[DEF_MAX_TABCARRIER];
	MCtrlTabMounter* pCtrlTabMounter[DEF_MAX_TABMOUNTER];
	MCtrlTabFeeder* pCtrlWorkTabFeeder;

	if (iWorkGroup == DEF_FRONT_GROUP)
	{
		for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
		{
			pTabMounter[i] = MOlbSystemPre.GetTabMounterComponent(i);
			pCtrlTabMounter[i] = MOlbSystemPre.GetCtrlTabMounterComponent(i);
#ifdef DEF_SOURCE_SYSTEM
			pTabMounterR[i] = MOlbSystemPre.GetTabMounterComponent((DEF_MAX_TABMOUNTER-1)-i);
#endif
		}

		if (iCarrierWorkGroup == DEF_FRONT_GROUP)
		{
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				pTabCarrier[i] = MOlbSystemPre.GetTabCarrierComponent(i);
				pCtrlTabCarrier[i] = MOlbSystemPre.GetCtrlTabCarrierComponent(i);
			}
			pCtrlWorkTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(0);
		}
		else
		{
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				pTabCarrier[i] = MOlbSystemPre.GetRTabCarrierComponent(i);
				pCtrlTabCarrier[i] = MOlbSystemPre.GetCtrlRTabCarrierComponent(i);
			}
			pCtrlWorkTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(1);
		}
	}
	else
	{
		for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
		{
#ifdef DEF_SOURCE_SYSTEM
			pTabMounter[i] = MOlbSystemPre.GetTabMounterComponent((DEF_MAX_TABMOUNTER-1)-i);		//7, 6, 5, 4
			pCtrlTabMounter[i] = MOlbSystemPre.GetCtrlTabMounterComponent((DEF_MAX_TABMOUNTER-1)-i);
			pTabMounterR[i] = MOlbSystemPre.GetTabMounterComponent(i);
#else
			pTabMounter[i] = MOlbSystemPre.GetRTabMounterComponent(i);
			pCtrlTabMounter[i] = MOlbSystemPre.GetCtrlRTabMounterComponent(i);
#endif
		}

		if (iCarrierWorkGroup == DEF_FRONT_GROUP)
		{
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				pTabCarrier[i] = MOlbSystemPre.GetTabCarrierComponent(i);
				pCtrlTabCarrier[i] = MOlbSystemPre.GetCtrlTabCarrierComponent(i);
			}
			pCtrlWorkTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(0);
		}
		else
		{
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				pTabCarrier[i] = MOlbSystemPre.GetRTabCarrierComponent(i);
				pCtrlTabCarrier[i] = MOlbSystemPre.GetCtrlRTabCarrierComponent(i);
			}
			pCtrlWorkTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(1);
		}
	}

//		일련 동작 중 입니다.\n잠시만 기다려 주세요...
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_29"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Loading TabIC..."));
	ProgWnd.SetRange(0, 5);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	double tgPos[DEF_MOUNTER_WORK_PER_GROUP];
	
	//Mounter Group과 Carrier Group이 틀릴경우 반대편 TabMounter Ready위치로 보내기
#ifdef DEF_SOURCE_SYSTEM
	if (iWorkGroup != iCarrierWorkGroup)
	{
		for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
		{
			tgPos[i] = pTabMounterR[i]->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
		}
		if (iWorkGroup == DEF_FRONT_GROUP)
			iResult = MOlbSystemPre.MoveAllRearTabMounter(tgPos, DEF_TABMOUNTER_READY_POS);
		else
			iResult = MOlbSystemPre.MoveAllFrontTabMounter(tgPos, DEF_TABMOUNTER_READY_POS);
	}
#endif

	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
#ifdef DEF_SOURCE_SYSTEM
		if (iWorkGroup == iCarrierWorkGroup)
			tgPos[i] = pTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_LOAD_POS);
		else
			tgPos[i] = pTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_R_LOAD_POS);
#else
		tgPos[i] = pTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
#endif
	}
	
#ifdef DEF_SOURCE_SYSTEM
	if (iWorkGroup == DEF_FRONT_GROUP)
		iResult = MOlbSystemPre.MoveAllFrontTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
	else
		iResult = MOlbSystemPre.MoveAllRearTabMounter(tgPos, DEF_TABMOUNTER_R_LOAD_POS);
#else
	if (iWorkGroup == DEF_FRONT_GROUP)
		iResult = MOlbSystemPre.MoveAllFrontTabMounter(tgPos, DEF_TABMOUNTER_READY_POS);
	else
		iResult = MOlbSystemPre.MoveAllRearTabMounter(tgPos, DEF_TABMOUNTER_READY_POS);
#endif

	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return iResult;
		return iResult;
	}

	MPos_XYT MtgPos[DEF_MAX_TABCARRIER];
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{			
		MtgPos[i] = pTabCarrier[i]->GetTargetXYTPos(DEF_TABCARRIER_READY_POS);
	}

	if (iCarrierWorkGroup == DEF_FRONT_GROUP)
		iResult = MOlbSystemPre.MoveAllFrontTabCarrier(MtgPos, DEF_TABCARRIER_READY_POS);
	else
		iResult = MOlbSystemPre.MoveAllRearTabCarrier(MtgPos, DEF_TABCARRIER_READY_POS);

	if (iResult)
	{
		ProgWnd.DestroyWindow();
		if (ERR_SKIP_ERROR_DISPLAY == iResult)
			return ERR_SKIP_ERROR_DISPLAY;
		return iResult;
	}

	int iMounterNo = 0;
	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
#ifdef DEF_SOURCE_SYSTEM
		if (iWorkGroup == iCarrierWorkGroup)
			iMounterNo = i;
		else
			iMounterNo = DEF_MOUNTER_WORK_PER_GROUP-i-1;	//0, 1, 2, 3
#else
		iMounterNo = DEF_MOUNTER_WORK_PER_GROUP-i-1;
#endif
		if (MOlbSystemPre.GetIOComponent()->IsOn(IN_FRONT_STEP_STOP_SW)
			|| MOlbSystemPre.GetIOComponent()->IsOn(IN_REAR_STEP_STOP_SW))
		{
			MyMessageBox(MY_MESSAGE_BOX, 30395, _T("confirm"), M_ICONINFORMATION);
			return ERR_SKIP_ERROR_DISPLAY;
		}

		iResult = pTabCarrier[i]->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
		{
			return iResult;
		}
		if (FALSE == pTabCarrier[i]->IsAbsorbTabIC())
		{
			MyMessageBox(MY_MESSAGE_BOX, 30340, _T("confirm"), M_ICONINFORMATION);
			return ERR_SKIP_ERROR_DISPLAY;
		}
		/*
		if (FALSE == pTabCarrier[i]->IsAbsorbTabIC())
		{
			iResult = pCtrlWorkTabFeeder->FeedTabIC();
			if (iResult)
			{
				return iResult;
			}
			
			iResult = pCtrlWorkTabFeeder->PressTapeStep1();
			if (iResult)
			{
				return iResult;
			}

			iResult = pCtrlTabCarrier[i]->LoadTabIC();
			if (iResult)
			{
				return iResult;
			}
			iResult = pCtrlWorkTabFeeder->PressTapeStep2();
			if (iResult)
			{
				return iResult;
			}
		}
		*/
		
		iResult = pTabCarrier[i]->SafeMoveXYTPos(DEF_TABCARRIER_PREALIGN_POS);
		if (iResult)
		{
			return iResult;
		}

		MPos_XYT	posAlignOffset;
		posAlignOffset.Init(0.0, 0.0, 0.0);
		
		iResult = pCtrlTabCarrier[i]->AlignTabIC(&posAlignOffset);
		if (iResult)
		{
			return iResult;
		}

		iResult = pTabCarrier[i]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
		{
			return iResult;
		}

#ifdef DEF_GATE_SYSTEM
//		pTabMounter[iMounterNo]->SetGantryMotionEnable(TRUE, i);
		pTabMounter[iMounterNo]->SetGantryMotionEnable(TRUE);
		iResult = pTabMounter[iMounterNo]->SafeMovePos(DEF_TABMOUNTER_LOAD_POS);
		if (iResult)
		{
			return iResult;
		}
#endif
		iResult = pCtrlTabMounter[iMounterNo]->LoadTabIC(iCarrierWorkGroup, i);
		if (iResult)
		{
			return iResult;
		}
		ProgWnd.StepIt();
#ifdef DEF_GATE_SYSTEM
		iResult = pTabMounter[iMounterNo]->SafeMovePos(DEF_TABMOUNTER_ESCAPE_POS, FALSE);
		if (iResult)
			return iResult;
		iResult = pTabCarrier[i]->SafeMoveXYTPos(DEF_TABCARRIER_ESCAPE_POS);
		if (iResult)
			return iResult;
		iResult = pTabMounter[iMounterNo]->Wait4Done();
		if (iResult)
			return iResult;
#endif
	}
	return ERR_MANUAL_MAINFORM_SUCCESS;
}

//101001.KKY_____
void CManualMainForm::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (TRUE == m_bInitialized)
	{
		if (FALSE == bShow)
		{
			KillTimer(10);
		}
		else
		{
			m_eOldOpMode = (EOpMode)(-1);
			SetTimer(10, 500, NULL);
		}
	}	
}

void CManualMainForm::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 10)
	{
		if (m_eOldOpMode != MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode())
		{
			if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
			{
				showButtonManual();
			}
			else
			{
				showButtonAuto();
			}
		}
		m_eOldOpMode = MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode();
	}

	CFormView::OnTimer(nIDEvent);
}

void CManualMainForm::showButtonManual()
{
	GetDlgItem(IDB_IO_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDB_LIMIT_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDB_RETURN_ORIGIN)->EnableWindow(TRUE);
	GetDlgItem(IDB_HEATER_POWER_RESET)->EnableWindow(TRUE);
	GetDlgItem(IDB_SERCOS_INITIALIZE)->EnableWindow(TRUE);
	GetDlgItem(IDB_FRONT_TABMOUNTER__MOVE_TO_LOAD_POS)->EnableWindow(TRUE);
	GetDlgItem(IDB_REAR_TABMOUNTER__MOVE_TO_LOAD_POS)->EnableWindow(TRUE);
	GetDlgItem(IDB_FRONT_TABCARRIER__MOVE_TO_READY_POS)->EnableWindow(TRUE);
	GetDlgItem(IDB_REAR_TABCARRIER__MOVE_TO_READY_POS)->EnableWindow(TRUE);
	GetDlgItem(IDB_FRONT_TABCARRIER__MOVE_TO_UNLOAD_POS)->EnableWindow(TRUE);
	GetDlgItem(IDB_REAR_TABCARRIER__MOVE_TO_UNLOAD_POS)->EnableWindow(TRUE);
	GetDlgItem(IDB_TABIC_LOAD_CARRIER)->EnableWindow(TRUE);
	GetDlgItem(IDB_TABIC_LOAD_ALL_TABMOUNTER)->EnableWindow(TRUE);
	GetDlgItem(IDB_AUTO_CHANGE_TABIC_REEL)->EnableWindow(TRUE);
	GetDlgItem(IDB_AUTO_CHANGE_TABIC_REEL2)->EnableWindow(TRUE);
	GetDlgItem(IDB_PANEL_ALIGNER)->EnableWindow(TRUE);
	GetDlgItem(IDB_PANEL_TRANSFER_IN)->EnableWindow(TRUE);
#ifdef DEF_USE_TURN_HANDLER
	GetDlgItem(IDB_MANUAL_TURN_HANDLER)->EnableWindow(TRUE);
#endif
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	GetDlgItem(IDB_PANEL_TRANSFER_CENTER)->EnableWindow(TRUE);	//@ ---> Transfer In
#	endif
#endif
	GetDlgItem(IDB_PANEL_ALIGNER3)->EnableWindow(TRUE);
	GetDlgItem(IDB_TABMOUNTER)->EnableWindow(TRUE);
	GetDlgItem(IDB_TABCARRIER)->EnableWindow(TRUE);
	GetDlgItem(IDB_TABCARRIER_R)->EnableWindow(TRUE);
	GetDlgItem(IDB_TABFEEDER)->EnableWindow(TRUE);
	GetDlgItem(IDB_TABFEEDER_R)->EnableWindow(TRUE);
	GetDlgItem(IDB_CAMERA_ES)->EnableWindow(TRUE);

//@#ifdef DEF_SOURCE_SYSTEM
//@	GetDlgItem(IDB_TILTING_JOB)->EnableWindow(TRUE);
//@#endif

	GetDlgItem(IDB_ENG_STOP)->EnableWindow(TRUE);

	GetDlgItem(IDB_TABIC_LOAD_ALL_TABMOUNTER_REAR)->EnableWindow(TRUE);

	//SJ_YYK 110503 전종연 K 요청 사항...
	if(MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		GetDlgItem(IDB_CHECK_PUNCH_REPEATABILITY)->EnableWindow(FALSE);
		GetDlgItem(IDB_CHECK_PUNCH_REPEATABILITY)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDB_CHECK_PUNCH_REPEATABILITY)->EnableWindow(TRUE);
		GetDlgItem(IDB_CHECK_PUNCH_REPEATABILITY)->ShowWindow(SW_NORMAL);
	}

	GetDlgItem(IDB_TABIC_REMOVE_FRONT)->EnableWindow(TRUE);
	GetDlgItem(IDB_TABIC_REMOVE_REAR)->EnableWindow(TRUE);
	GetDlgItem(IDB_REAR_TABMOUNTER__MOVE_TO_LOAD_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_TABCARRIER__MOVE_TO_READY_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_TABCARRIER__MOVE_TO_UNLOAD_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABIC_REMOVE_REAR)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABCARRIER_R)->EnableWindow(FALSE);
	GetDlgItem(IDB_FRONT_GANTRY_MOVE_TO_CLEAN_POS)->EnableWindow(TRUE);
	GetDlgItem(IDB_REAR_GANTRY_MOVE_TO_CLEAN_POS)->EnableWindow(FALSE);

}

void CManualMainForm::showButtonAuto()
{
	GetDlgItem(IDB_IO_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDB_LIMIT_CHECK)->EnableWindow(TRUE);

	GetDlgItem(IDB_RETURN_ORIGIN)->EnableWindow(FALSE);
	GetDlgItem(IDB_HEATER_POWER_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDB_SERCOS_INITIALIZE)->EnableWindow(FALSE);
	GetDlgItem(IDB_FRONT_TABMOUNTER__MOVE_TO_LOAD_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_TABMOUNTER__MOVE_TO_LOAD_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_FRONT_TABCARRIER__MOVE_TO_READY_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_TABCARRIER__MOVE_TO_READY_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_FRONT_TABCARRIER__MOVE_TO_UNLOAD_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_TABCARRIER__MOVE_TO_UNLOAD_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABIC_LOAD_CARRIER)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABIC_LOAD_ALL_TABMOUNTER)->EnableWindow(FALSE);
	GetDlgItem(IDB_AUTO_CHANGE_TABIC_REEL)->EnableWindow(FALSE);
	GetDlgItem(IDB_AUTO_CHANGE_TABIC_REEL2)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_ALIGNER)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_TRANSFER_CENTER)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_ALIGNER3)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABMOUNTER)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABCARRIER)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABCARRIER_R)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABFEEDER)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABFEEDER_R)->EnableWindow(FALSE);
	GetDlgItem(IDB_CAMERA_ES)->EnableWindow(FALSE);
	GetDlgItem(IDB_TILTING_JOB)->EnableWindow(FALSE);
	GetDlgItem(IDB_ENG_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABIC_LOAD_ALL_TABMOUNTER_REAR)->EnableWindow(FALSE);
	//SJ_YYK 110503 전종연 K 요청 사항...
	GetDlgItem(IDB_CHECK_PUNCH_REPEATABILITY)->EnableWindow(FALSE);
	GetDlgItem(IDB_CHECK_PUNCH_REPEATABILITY)->ShowWindow(SW_HIDE);	

	GetDlgItem(IDB_TABIC_REMOVE_FRONT)->EnableWindow(FALSE);
	GetDlgItem(IDB_TABIC_REMOVE_REAR)->EnableWindow(FALSE);

	GetDlgItem(IDB_FRONT_GANTRY_MOVE_TO_CLEAN_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_REAR_GANTRY_MOVE_TO_CLEAN_POS)->EnableWindow(FALSE);
	GetDlgItem(IDB_PANEL_TRANSFER_IN)->EnableWindow(FALSE);
	GetDlgItem(IDB_MANUAL_TURN_HANDLER)->EnableWindow(FALSE);
}

//_______________

void CManualMainForm::OnClickTabicRemoveFront() 
{
	// TODO: Add your control notification handler code here
	CString strMsg;
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
//		Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
		MyMessageBox(MY_MESSAGE_BOX, 30376, _T("Error"), M_ICONERROR);
		return;
	}
	if (MyMessageBox(MY_MESSAGE_BOX, 30398, _T("confirm"), M_ICONQUESTION|M_YESNO) != IDYES)
		return;

	BOOL bRemoveCarrier = FALSE;	
	int iResult = MyMessageBox(_T("Please select the unit to discard the TabIC ...\r\nTabIC를 버릴 Unit을 선택하세요..."), _T("confirm"), M_ICONQUESTION|M_YESNO, _T("Mounter"), _T("Mounter&&Carrier"));
	if (IDYES == iResult)
		bRemoveCarrier = FALSE;
	else
		bRemoveCarrier = TRUE;

	iResult = MOlbSystemPre.RemoveTabIC(DEF_FRONT_GROUP, bRemoveCarrier);
	if (iResult != SUCCESS && iResult != ERR_SKIP_ERROR_DISPLAY)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_31"), &strMsg);
	MyMessageBox(strMsg, _T("Complete"));

}

void CManualMainForm::OnClickTabicRemoveRear() 
{
	// TODO: Add your control notification handler code here

	CString strMsg;
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
//		Dry Run Mode 설정 상태이므로 일련 동작 수행 불가.
		MyMessageBox(MY_MESSAGE_BOX, 30376, _T("Error"), M_ICONERROR);
		return;
	}
	if (MyMessageBox(MY_MESSAGE_BOX, 30399, _T("confirm"), M_ICONQUESTION|M_YESNO) != IDYES)
		return;
	BOOL bRemoveCarrier = FALSE;	
	int iResult = MyMessageBox(_T("Please select the unit to discard the TabIC ...\r\nTabIC를 버릴 Unit을 선택하세요..."), _T("confirm"), M_ICONQUESTION|M_YESNO, _T("Mounter"), _T("Mounter&&Carrier"));
	if (IDYES == iResult)
		bRemoveCarrier = FALSE;
	else
		bRemoveCarrier = TRUE;

	iResult = MOlbSystemPre.RemoveTabIC(DEF_REAR_GROUP, bRemoveCarrier);
	if (iResult != SUCCESS && iResult != ERR_SKIP_ERROR_DISPLAY)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_31"), &strMsg);
	MyMessageBox(strMsg, _T("Complete"));
}

void CManualMainForm::OnClickManualTurnHandler() 
{
#ifdef DEF_USE_TURN_HANDLER
	// TODO: Add your control notification handler code here
	CManualHandlerDlg dlg;
	dlg.DoModal();
#endif
}

void CManualMainForm::OnClickPanelTransferIn() 
{
	// TODO: Add your control notification handler code here
	CManualPanelTransferDlg dlg(DEF_PANEL_TRANSFER_IN);
	dlg.DoModal();	
}

void CManualMainForm::OnClickFrontGantryMoveToCleanPos() 
{
	//170316 Jas_SYS Add
#ifndef DEF_SETUP
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;	
	}
#endif
	// TODO: Add your control notification handler code here
	if (MyMessageBox(MY_MESSAGE_BOX, 30700, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_21"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("Gantry Move To Mount Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.GetTabMounterComponent(0)->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS, TRUE, TRUE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Load 위치로 이동 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30394, _T("confirm"), M_ICONINFORMATION);			
}

void CManualMainForm::OnClickRearGantryMoveToCleanPos() 
{
	//170316 Jas_SYS Add
#ifndef DEF_SETUP
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;	
	}
#endif
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if (MyMessageBox(MY_MESSAGE_BOX, 30701, _T("confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	int iResult = ERR_MANUAL_MAINFORM_SUCCESS;

//	일련 동작 중 입니다.\n잠시만 기다려 주세요...
	CString strMsg;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualMainForm_21"), &strMsg);
	CMyProgressWnd ProgWnd(NULL, _T("Gantry Move To Mount Pos..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(strMsg);
	ProgWnd.PeekAndPump();
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.GetRTabMounterComponent(0)->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS, TRUE, TRUE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Load 위치로 이동 작업 완료
	MyMessageBox(MY_MESSAGE_BOX, 30394, _T("confirm"), M_ICONINFORMATION);			
}
