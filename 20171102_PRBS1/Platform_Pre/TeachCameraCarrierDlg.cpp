// CameraTeachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "TeachCameraCarrierDlg.h"
#include "MPlatformOlbSystem.h"
#include "VisionMonitorDlg.h"
#include "VisionCalibDlg.h"
#include "VisionViewDlg.h"
#include "IVision.h"
#include "MTabMounter.h"
#include "IAxis.h"
#include "MAxisData.h"
#include "MPreBonderData.h"
#include "MProcessData.h"
#include "MPanelAligner.h"
#include <math.h> // 2009.04.19 SESL_CYJ
#include "MTrsAutoManager.h"

#include "TeachMainForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachCameraCarrierDlg dialog

CTeachCameraCarrierDlg::CTeachCameraCarrierDlg(int iFixModel, int iCameraCarrierGroupNo, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachCameraCarrierDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeachCameraCarrierDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iGroupNo = iCameraCarrierGroupNo;
	m_iFixModel = iFixModel;
	m_iCameraCarrierOldID = -1;
	m_iSelectedAxis = 0;
	m_iSelectedTabNo = -1;

	m_bTemporaryView = FALSE;
	m_pdlgView = NULL;

	for (int i = 0; i < DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE; i++)
		m_rgdDisplayOffset[i] = 0.0;

	m_plnkC_MngOpPanel	= MOlbSystemPre.GetManageOpPanelComponent();
	m_plnkSystemData	= MOlbSystemPre.GetSystemData();
	m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
	m_plnkProcessData	= MOlbSystemPre.GetProcessData();
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST]	= MOlbSystemPre.GetWorkSchedule(m_iGroupNo);
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND]	= MOlbSystemPre.GetWorkScheduleSecond(m_iGroupNo);
#endif

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (m_iGroupNo == DEF_FRONT_GROUP)
		{
			m_rgiJog_Carrier[i] = DEF_JOG_F_INSPECTION_CAMERA1_X + i;
			m_rgiJog_BackupZ[i] = DEF_JOG_F_BACKUP1_Z + i;
			m_rgiJog_Expand[i] = DEF_JOG_F_INSPECTION_CAMERA1_EXPAND + i;
		}
		else
		{
			m_rgiJog_Carrier[i] = DEF_JOG_R_INSPECTION_CAMERA1_X + i;
			m_rgiJog_BackupZ[i] = DEF_JOG_R_BACKUP1_Z + i;
			m_rgiJog_Expand[i] = DEF_JOG_R_INSPECTION_CAMERA1_EXPAND + i;
		}
	}

	m_rgiJog_BackupX = NULL;

	m_iCameraCarrierID = DEF_INSPECTION_CARRIER_1;
	m_iSelectedPos = DEF_CAMERACARRIER_PANEL_MARK_POS;

	m_iWorkScheduleNo = DEF_WORK_SCHEDULE_FIRST;
}

void CTeachCameraCarrierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachCameraCarrierDlg)
	DDX_Control(pDX, IDB_SELECT_NEXT, m_btnNextCameraCarrier);
	DDX_Control(pDX, IDB_SELECT_PREVIOS, m_btnPrevCameraCarrier);

	DDX_Control(pDX, IDB_AXIS1, m_ctrlBtnAxis[eAxisX]);
	DDX_Control(pDX, IDB_AXIS2, m_ctrlBtnAxis[eAxisZ]);
	DDX_Control(pDX, IDB_AXIS3, m_ctrlBtnAxis[eAxisExp]);
	
//	DDX_Control(pDX, IDC_GRD_AXIS_CAMERA_CARRIER_SNS_STS,	m_grd_AxisSnsState[eAxisX]);
//	DDX_Control(pDX, IDC_GRD_AXIS_BACUP_Z_SNS_STS,			m_grd_AxisSnsState[eAxisZ]);
//	DDX_Control(pDX, IDC_GRD_AXIS_CAMERA_EXPAND_SNS_STS,	m_grd_AxisSnsState[eAxisExp]);



	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_CM_UPDOWN,	m_btnCmd[0]);
	DDX_Control(pDX, IDB_CM_ONOFF,	m_btnCmd[1]);

	DDX_Control(pDX, IDB_POS0, m_btnPos[0]);
	DDX_Control(pDX, IDB_POS1, m_btnPos[1]);
	DDX_Control(pDX, IDB_POS2, m_btnPos[2]);
	DDX_Control(pDX, IDB_POS3, m_btnPos[3]);
	DDX_Control(pDX, IDB_POS4, m_btnPos[4]);
	DDX_Control(pDX, IDB_POS5, m_btnPos[5]);
	DDX_Control(pDX, IDB_POS6, m_btnPos[6]);
	DDX_Control(pDX, IDB_POS7, m_btnPos[7]);
	DDX_Control(pDX, IDB_POS8, m_btnPos[8]);

	DDX_Control(pDX, IDC_TGT_POS1, m_lblCoord[0][0]);
	DDX_Control(pDX, IDC_FIX_POS1, m_lblCoord[0][1]);
	DDX_Control(pDX, IDC_MODEL_POS1, m_lblCoord[0][2]);
	DDX_Control(pDX, IDC_OFFSET_POS1, m_lblCoord[0][3]);
	DDX_Control(pDX, IDC_CUR_POS1, m_lblCoord[0][4]);
	DDX_Control(pDX, IDC_DIF_POS1, m_lblCoord[0][5]);

	DDX_Control(pDX, IDC_TGT_POS2, m_lblCoord[1][0]);
	DDX_Control(pDX, IDC_FIX_POS2, m_lblCoord[1][1]);
	DDX_Control(pDX, IDC_MODEL_POS2, m_lblCoord[1][2]);
	DDX_Control(pDX, IDC_OFFSET_POS2, m_lblCoord[1][3]);
	DDX_Control(pDX, IDC_CUR_POS2, m_lblCoord[1][4]);
	DDX_Control(pDX, IDC_DIF_POS2, m_lblCoord[1][5]);

	DDX_Control(pDX, IDC_TGT_POS3, m_lblCoord[2][0]);
	DDX_Control(pDX, IDC_FIX_POS3, m_lblCoord[2][1]);
	DDX_Control(pDX, IDC_MODEL_POS3, m_lblCoord[2][2]);
	DDX_Control(pDX, IDC_OFFSET_POS3, m_lblCoord[2][3]);
	DDX_Control(pDX, IDC_CUR_POS3, m_lblCoord[2][4]);
	DDX_Control(pDX, IDC_DIF_POS3, m_lblCoord[2][5]);

	DDX_Control(pDX, IDC_WORKER_NO, m_lblWorkerNo);

	DDX_Control(pDX, IDC_SEL_TAB_NO, m_lblSelTabNo);
	DDX_Control(pDX, IDS_SCHEDULE_NO, m_lblScheduleNo);	
}


BEGIN_MESSAGE_MAP(CTeachCameraCarrierDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachCameraCarrierDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachCameraCarrierDlg message handlers

void CTeachCameraCarrierDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CTeachCameraCarrierDlg::updatePosButton()
{
	int i = 0;
	for (int i = 0; i < DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE; i++)
	{
		if (m_iSelectedAxis == i)
			m_ctrlBtnAxis[i].EnableWindow(TRUE);
		else
			m_ctrlBtnAxis[i].EnableWindow(FALSE);
	}

	STeachingInfo sTeachingInfo;
	int	iMaxPos;
	if (m_iSelectedAxis == 0)
	{
		sTeachingInfo = m_plnkCamCarrier->GetTeachingInfo();
		iMaxPos = DEF_CAMERACARRIER_MAX_POS;
	}
	else if (m_iSelectedAxis == 1)
	{
		sTeachingInfo = m_plnkCamCarrier->GetTeachingInfoBackup();
		iMaxPos = DEF_CAMERACARRIER_BACKUP_MAX_POS;
	}
	else
	{
		sTeachingInfo = m_plnkCamCarrier->GetTeachingInfoExpand();
		iMaxPos = DEF_CAMERACARRIER_EXPAND_MAX_POS;
	}

	for (int i = 0; i < DEF_CAMERACARRIER_TEACH_MAX_POS_BTN; i++)
	{
		m_btnPos[i].ShowWindow(SW_SHOW);
		m_btnPos[i].EnableWindow(TRUE);
	}

	int iSelected = -1;
	//for (int i = 0; i < DEF_CAMERACARRIER_MAX_POS; i++)
	for (int i = 0; i < iMaxPos; i++)
	{
		if ((sTeachingInfo.m_sPosition[i]).m_bUse == TRUE)
		{
			m_btnPos[i].SetWindowText((LPCTSTR)(sTeachingInfo.m_sPosition[i].m_strNewLineName[0])); // LPCTSTR붙임_영락
			
			if (sTeachingInfo.m_sPosition[i].m_bEnable == FALSE)
				m_btnPos[i].EnableWindow(FALSE);
			else
			{
				m_btnPos[i].ShowWindow(SW_SHOW);
				m_btnPos[i].EnableWindow(TRUE);
			}

			if (iSelected == -1 && m_iSelectedPos == i && sTeachingInfo.m_sPosition[i].m_bEnable == TRUE)
				iSelected = i;
		}
	}
	
	// 사용하지 않는 Position Button 감추기
	for (; i < DEF_CAMERACARRIER_TEACH_MAX_POS_BTN; i++)
		m_btnPos[i].ShowWindow(SW_HIDE);

	// 기본 selectPosition 설정 하기
	if (iSelected != -1)
		selectPosition(iSelected, TRUE);
	else
		selectPosition(0, TRUE);
}

void CTeachCameraCarrierDlg::setWorkCameraCarrier(const int iCameraCarrierID)
{
	if (m_iCameraCarrierOldID == iCameraCarrierID)
		return;

	if (m_iGroupNo == DEF_FRONT_GROUP)
		m_plnkCamCarrier = MOlbSystemPre.GetCameraCarrierComponent(iCameraCarrierID);
	else
		m_plnkCamCarrier = MOlbSystemPre.GetRCameraCarrierComponent(iCameraCarrierID);
	m_plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();

//	m_iSelectedAxis = 0;

//	EWorkingSide eWorkSide = m_plnkProcessData->GetCurrentWorkingSide(m_iGroupNo);
	int iInspectorNo = m_plnkCamCarrier->GetInspectCamCarrierNo();
	if (DEF_NONE_INSPECTION_CAMERA != iInspectorNo)
	{
		//KKY_____
//		m_iSelectedTabNo = m_plnkProcessData->GetInspectorDefaultTabNo(eWorkSide, iInspectorNo);
//		if (-1 == m_iSelectedTabNo)
//			m_iSelectedTabNo = 0;
		m_iSelectedTabNo = 0;
		//________
	}

	/** CameraCarrier 에서 좌표 얻어오기 */
	m_plnkCamCarrier->GetPosParameter(m_iWorkScheduleNo, m_rgdFixedPosData[0], m_rgdOffsetPosData[0], m_rgdModelPosData[0]);
	m_plnkCamCarrier->GetPosParameterBackup(m_rgdFixedPosData[1], m_rgdOffsetPosData[1], m_rgdModelPosData[1]);
	m_plnkCamCarrier->GetPosParameterExpand(m_rgdFixedPosData[2], m_rgdOffsetPosData[2], m_rgdModelPosData[2]);
//@#ifdef DEF_SOURCE_SYSTEM
//@	m_plnkPanelAligner->GetBackupMoveXPosParameter(m_rgdFixedPosData[2], m_rgdOffsetPosData[2], m_rgdModelPosData[2]);
//@#endif

//	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
//		ASSERT(DEF_MAX_CAMERACARRIER == 3);
		ASSERT(iCameraCarrierID >= 0 && iCameraCarrierID < DEF_MAX_INSPECTION_CAM_CARRIER);
		if (m_iSelectedAxis == 0)
		{
			m_iJogSelect = m_rgiJog_Carrier[iCameraCarrierID];
		}
		else if (m_iSelectedAxis == 1)
		{
			m_iJogSelect = m_rgiJog_BackupZ[iCameraCarrierID];
		}
		else
		{
			m_iJogSelect = m_rgiJog_Expand[iCameraCarrierID];
		}
//@#ifdef DEF_SOURCE_SYSTEM
//@		else
//@		{
//@			m_iJogSelect = m_rgiJog_BackupX;
//@		}
//@#endif
	}
	m_plnkC_MngOpPanel->SetJogUnit(m_iJogSelect);

	updatePosButton();	// Teaching Position Name Update

	updateCoordinate();	// Selected Position 에 대한 Coordinate Update
}

BOOL CTeachCameraCarrierDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE:
		{
	//		CameraCarrier 모델좌표 Teaching
			CString szTextTemp;
			ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_1"), &szTextTemp);
			GetDlgItem(IDB_GET_ORIGIN_OFFSET)->SetWindowText(szTextTemp);
			break;
		}

	case DEF_FIX_MODE:
		{
	//		CameraCarrier 고정좌표 Teaching
			CString szTextTemp;
			ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_2"), &szTextTemp);
			SetWindowText(szTextTemp);
			break;
		}
	}

	if (m_iGroupNo == DEF_FRONT_GROUP)
		SetWindowText(_T("Camera Carrier Teaching [ Front Group]"));
	else
		SetWindowText(_T("Camera Carrier Teaching [ Rear Group]"));

	WCHAR szTemp[250];

	swprintf(szTemp, _T("Schedule\r\nNO.%d"), m_iWorkScheduleNo+1);
	m_lblScheduleNo.SetWindowText(szTemp);

	swprintf(szTemp, _T("%d"), m_iCameraCarrierID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
//@#ifdef DEF_SOURCE_SYSTEM
//@	swprintf(szTemp, _T("Camera\nCarrier%d Y"), m_iCameraCarrierID + 1);
//@#else
	swprintf(szTemp, _T("Camera\nCarrier%d X"), m_iCameraCarrierID + 1);
//@#endif
	m_ctrlBtnAxis[DEF_CAMERACARRIER_TEACH_CAMERA_Y_AXIS_TITLE].SetWindowText(szTemp);

	swprintf(szTemp, _T("Backup\n%d Z"), m_iCameraCarrierID + 1);
	m_ctrlBtnAxis[DEF_CAMERACARRIER_TEACH_BACKUP_Z_AXIS_TITLE].SetWindowText(szTemp);

	swprintf(szTemp, _T("Camera\nExpand %d"), m_iCameraCarrierID + 1);
	m_ctrlBtnAxis[DEF_CAMERACARRIER_TEACH_EXPAND_AXIS_TITLE].SetWindowText(szTemp);

	/*
	if (DEF_CAMERACARRIER3 == m_iCameraCarrierID || DEF_CAMERACARRIER6 == m_iCameraCarrierID)
		m_btnPrevCameraCarrier.EnableWindow(FALSE);
	
	if (DEF_CAMERACARRIER5 == m_iCameraCarrierID || DEF_CAMERACARRIER8 == m_iCameraCarrierID)
		m_btnNextCameraCarrier.EnableWindow(FALSE);
	*/
	if (DEF_INSPECTION_CARRIER_1 == m_iCameraCarrierID)
		m_btnPrevCameraCarrier.EnableWindow(FALSE);
	if (DEF_MAX_INSPECTION_CAM_CARRIER-1 == m_iCameraCarrierID)
		m_btnNextCameraCarrier.EnableWindow(FALSE);

	/*
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		if (DEF_CAMERACARRIER2 == m_iCameraCarrierID)
		{
			m_btnPrevCameraCarrier.EnableWindow(FALSE);
			m_btnNextCameraCarrier.EnableWindow(FALSE);

			m_btnCmd[0].EnableWindow(FALSE);
			m_btnCmd[0].ShowWindow(SW_HIDE);
			m_btnCmd[1].EnableWindow(FALSE);
			m_btnCmd[1].ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if (DEF_INSPECTION_CAMERA_NONE_GROUP == m_iGroupNo)
		{
			GetDlgItem(IDB_GET_ORIGIN_OFFSET)->ShowWindow(SW_HIDE);
			
			m_btnCmd[0].EnableWindow(FALSE);
			m_btnCmd[0].ShowWindow(SW_HIDE);
			m_btnCmd[1].EnableWindow(FALSE);
			m_btnCmd[1].ShowWindow(SW_HIDE);
		}
	}
	*/

	setWorkCameraCarrier(m_iCameraCarrierID);

	// 확인 필요 : Maker 권한 줄까 말까?
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDB_GET_ORIGIN_OFFSET)->ShowWindow(SW_HIDE);
	}

	m_uiTimerID = SetTimer(1, 200, NULL);

	// 사용하지 않는 축 좌표 표시 Label 비활성화.
	GetDlgItem(IDB_AXIS2)->EnableWindow(FALSE);
	for (int i = 0; i < DEF_CAMERACARRIER_TEACH_MAX_COORDINATE_TYPE; i++)
	{
		m_lblCoord[1][i].EnableWindow(FALSE);
		m_lblCoord[2][i].EnableWindow(FALSE);
	}

	if(m_iGroupNo == DEF_REAR_GROUP)
	{
		GetDlgItem(IDB_SELECT_PREVIOS)->EnableWindow(FALSE);
		GetDlgItem(IDB_SELECT_NEXT)->EnableWindow(FALSE);
	}
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_3"), &szTextTemp);
//	목표 위치
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_4"), &szTextTemp);
//	고정 좌표
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_5"), &szTextTemp);
//	모델 좌표
	GetDlgItem(IDC_MODEL_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_6"), &szTextTemp);
//	Offset 좌표
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_7"), &szTextTemp);
//	현재 위치
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_8"), &szTextTemp);
//	차이값
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_9"), &szTextTemp);
//	Camera Carrier 번호 선택
	GetDlgItem(IDS_SELECT)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_10"), &szTextTemp);
//	목표 위치로\r\n이동
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_11"), &szTextTemp);
//	목표 위치를\r\n현재값으로
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_12"), &szTextTemp);
//	(+) 방향\r\n1 Tab 이동
	GetDlgItem(IDB_CM_UPDOWN)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_13"), &szTextTemp);
//	(-) 방향\r\n1 Tab 이동
	GetDlgItem(IDB_CM_ONOFF)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_14"), &szTextTemp);
//	원점복귀\r\nOffset 얻기
	GetDlgItem(IDB_GET_ORIGIN_OFFSET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_15"), &szTextTemp);
//	저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_16"), &szTextTemp);
//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachCameraCarrierDlg_18"), &szTextTemp);
	GetDlgItem(IDB_CHANGE_AXIS)->SetWindowText(szTextTemp);
	
	// Doolin kwangilshin 2017. 08. 24.
	//
	Init_GridAxisStste();
	// End

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CTeachCameraCarrierDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachCameraCarrierDlg)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_CHANGE_AXIS, -600 /* Click */, OnClickChangeAxis, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_AXIS1, -600 /* Click */, OnAxis1, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_AXIS2, -600 /* Click */, OnAxis2, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_VISION_CALIB, -600 /* Click */, OnClickVisionCalib, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_REGISTER_MARK, -600 /* Click */, OnClickRegisterMark, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_CM_MOVE, -600 /* Click */, OnClickCmMove, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS_SET, -600 /* Click */, OnClickPosSet, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_CM_UPDOWN, -600 /* Click */, OnClickCmOneTabPlus, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_CM_ONOFF, -600 /* Click */, OnClickCmOneTabMinus, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_SELECT_PREVIOS, -600 /* Click */, OnClickSelectPrevios, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_SELECT_NEXT, -600 /* Click */, OnClickSelectNext, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_GET_ORIGIN_OFFSET, -600 /* Click */, OnGetOriginOffset, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS0, -600 /* Click */, OnClickPos0, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS1, -600 /* Click */, OnClickPos1, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS2, -600 /* Click */, OnClickPos2, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS3, -600 /* Click */, OnClickPos3, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS4, -600 /* Click */, OnClickPos4, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS5, -600 /* Click */, OnClickPos5, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS6, -600 /* Click */, OnClickPos6, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS7, -600 /* Click */, OnClickPos7, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_POS8, -600 /* Click */, OnClickPos8, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_TEMPORARY_VIEW, -600 /* Click */, OnTemporaryView, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_AXIS3, -600 /* Click */, OnClickAxis3, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_SELECT_SCHEDULE_PREV, -600 /* Click */, OnClickSelectSchedulePrev, VTS_NONE)
	ON_EVENT(CTeachCameraCarrierDlg, IDB_SELECT_SCHEDULE_NEXT, -600 /* Click */, OnClickSelectScheduleNext, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachCameraCarrierDlg::OnClickSave() 
{
	CString strMsg = _T("");
/*임시
	if (m_iFixModel == DEF_FIX_MODE && m_iSelectedAxis == 0)
	{
//		Camera Carrier 고정좌표는 Teaching 할 수 없음.
		MyMessageBox(MY_MESSAGE_BOX,50100,_T("Information"), M_ICONINFORMATION);
		return;
	}
*/
//	Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50101,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	if (m_iFixModel == DEF_FIX_MODE)
	{
		if (m_iSelectedAxis == 0)
		{
			m_plnkCamCarrier->SaveFixPosParameter(m_rgdFixedPosData[m_iSelectedAxis]);
		}
		else if (m_iSelectedAxis == 1)
		{
			m_plnkCamCarrier->SaveFixedPosParameterBackup(m_rgdFixedPosData[m_iSelectedAxis]);
		}
//@		else
//@		{
//@			m_plnkPanelAligner->SaveBackupMoveXFixedPosParameter(m_rgdFixedPosData[m_iSelectedAxis]);
//@		}
	}
	else
	{
		if (m_iSelectedAxis == 0)
		{
			m_plnkCamCarrier->SaveOffsetPosParameter(m_rgdOffsetPosData[m_iSelectedAxis]);
		}
		else if (m_iSelectedAxis == 1)
		{
			m_plnkCamCarrier->SaveOffsetPosParameterBackup(m_rgdOffsetPosData[m_iSelectedAxis]);
		}
		else
		{
			m_plnkCamCarrier->SaveOffsetPosParameterExpand(m_rgdOffsetPosData[m_iSelectedAxis]);
		}
//@		else
//@		{
//@			m_plnkPanelAligner->SaveBackupMoveXOffsetPosParameter(m_rgdOffsetPosData[m_iSelectedAxis]);
//@		}
	}
}

void CTeachCameraCarrierDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	if (NULL != m_pdlgView)
	{
		m_pdlgView->DestroyWindow();
		delete m_pdlgView;
	}

#ifndef SIMULATION
	m_plnkC_MngOpPanel->SetJogUnit(-1);
#endif /* SIMULATION */

	CDialog::OnCancel();
}

void CTeachCameraCarrierDlg::OnClickChangeAxis() 
{
	m_iSelectedAxis = (++m_iSelectedAxis) % DEF_CAMERACARRIER_MAX_AXIS_TYPE;//DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE;
	m_iSelectedPos = 0;
	updatePosButton();

	if (m_iSelectedAxis == 1)
	{
		m_iJogSelect = m_rgiJog_BackupZ[m_iCameraCarrierID];
	}
	else if (m_iSelectedAxis == 2)
	{
		m_iJogSelect = m_rgiJog_Expand[m_iCameraCarrierID];
	}
//@	else if (m_iSelectedAxis == 2)
//@	{
//@		m_iJogSelect = m_rgiJog_BackupX;
//@	}
	else
	{
		m_iJogSelect = m_rgiJog_Carrier[m_iCameraCarrierID];
	}

	m_plnkC_MngOpPanel->SetJogUnit(m_iJogSelect);
}

void CTeachCameraCarrierDlg::OnAxis1() 
{
	if (m_iFixModel == DEF_FIX_MODE)
	{
//		Camera Carrier 고정좌표는 Teaching 할 수 없음.
		MyMessageBox(MY_MESSAGE_BOX,50102,_T("Information"), M_ICONINFORMATION);
		return;
	}

	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_lblCoord[0][0].GetWindowText(cBeforeVal, 30);
	
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkCamCarrier->GetMotionComponent()))
	{
		return;
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	m_lblCoord[0][0].SetCaption(cAfterVal);
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		m_rgdOffsetPosData[0][m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[0][m_iSelectedPos]  - m_rgdFixedPosData[0][m_iSelectedPos];
	
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[0][m_iSelectedPos]);
		m_lblCoord[0][3].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		m_rgdFixedPosData[0][m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[0][m_iSelectedPos] - m_rgdOffsetPosData[0][m_iSelectedPos];

		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[0][m_iSelectedPos]);
		m_lblCoord[0][1].SetCaption(szTemp);
		break;		
	}
}

void CTeachCameraCarrierDlg::OnAxis2() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_lblCoord[1][0].GetWindowText(cBeforeVal, 30);
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkCamCarrier->GetMotionComponentBackup()))
	{
		return;
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	m_lblCoord[1][0].SetCaption(cAfterVal);
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		m_rgdOffsetPosData[1][m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[1][m_iSelectedPos]  - m_rgdFixedPosData[1][m_iSelectedPos];
	
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[1][m_iSelectedPos]);
		m_lblCoord[1][3].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		m_rgdFixedPosData[1][m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[1][m_iSelectedPos] - m_rgdOffsetPosData[1][m_iSelectedPos];

		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[1][m_iSelectedPos]);
		m_lblCoord[1][1].SetCaption(szTemp);
		break;		
	}	
}

void CTeachCameraCarrierDlg::OnClickAxis3() 
{
	if (m_iFixModel == DEF_FIX_MODE)
	{
//		Camera Carrier 고정좌표는 Teaching 할 수 없음.
		MyMessageBox(MY_MESSAGE_BOX,50129,_T("Information"), M_ICONINFORMATION);
		return;
	}

	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_lblCoord[2][0].GetWindowText(cBeforeVal, 30);

	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkCamCarrier->GetMotionComponentExpand()))
	{
		return;
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	m_lblCoord[2][0].SetCaption(cAfterVal);
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		m_rgdOffsetPosData[2][m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[2][m_iSelectedPos]  - m_rgdFixedPosData[2][m_iSelectedPos];
	
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[2][m_iSelectedPos]);
		m_lblCoord[2][3].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		m_rgdFixedPosData[2][m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[2][m_iSelectedPos] - m_rgdOffsetPosData[2][m_iSelectedPos];

		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[2][m_iSelectedPos]);
		m_lblCoord[2][1].SetCaption(szTemp);
		break;		
	}	
}

void CTeachCameraCarrierDlg::OnClickPos0() 
{
	// TODO: Add your control notification handler code here
	selectPosition(0);	
}

void CTeachCameraCarrierDlg::OnClickPos1() 
{
	// TODO: Add your control notification handler code here
	selectPosition(1);	
}

void CTeachCameraCarrierDlg::OnClickPos2() 
{
	// TODO: Add your control notification handler code here
	selectPosition(2);	
}

void CTeachCameraCarrierDlg::OnClickPos3() 
{
	// TODO: Add your control notification handler code here
	selectPosition(3);	
}

void CTeachCameraCarrierDlg::OnClickPos4() 
{
	// TODO: Add your control notification handler code here
	selectPosition(4);	
}

void CTeachCameraCarrierDlg::OnClickPos5() 
{
	// TODO: Add your control notification handler code here
	selectPosition(5);
}

void CTeachCameraCarrierDlg::OnClickPos6() 
{
	// TODO: Add your control notification handler code here
	selectPosition(6);
}

void CTeachCameraCarrierDlg::OnClickPos7() 
{
	// TODO: Add your control notification handler code here
	selectPosition(7);
}

void CTeachCameraCarrierDlg::OnClickPos8() 
{
	// TODO: Add your control notification handler code here
	selectPosition(8);
}

void CTeachCameraCarrierDlg::OnClickVisionCalib()
{	
//	int iCamNo = m_plnkCamCarrier->GetOwnedCameraNo();
	int iCamNo = m_plnkCamCarrier->GetOwnedCameraNo1();
	CVisionCalibDlg dlg(iCamNo, DEF_CALIB_GROUP_MOUNTER, DEF_TABMOUNTER1);
	dlg.DoModal();

	m_plnkCamCarrier->GetPosParameter(m_iWorkScheduleNo, m_rgdFixedPosData[m_iSelectedAxis], m_rgdOffsetPosData[m_iSelectedAxis], m_rgdModelPosData[m_iSelectedAxis]);
	updateDisplay();
	selectPosition(m_iSelectedPos);
}

void CTeachCameraCarrierDlg::OnClickRegisterMark()
{
	if (TRUE == m_bTemporaryView)
	{
		if (NULL != m_pdlgView)
		{
			m_pdlgView->DestroyWindow();
			delete m_pdlgView;
			m_pdlgView = NULL;
		}
		m_bTemporaryView = FALSE;
	}

	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
//	int iCamNo = m_plnkCamCarrier->GetOwnedCameraNo();
	int iCamNo = m_plnkCamCarrier->GetOwnedCameraNo1();
	CVisionMonitorDlg dlg(plnkVision, DEF_PM_MARK, iCamNo);	// Operation Type = DEF_PM_MARK / DEF_BLOB_MARK / DEF_CALIPER_MARK
	dlg.DoModal();
}

void CTeachCameraCarrierDlg::OnClickCmMove() 
{
#ifndef DEF_SETUP
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;	
	}
	//20171004 KDW Start
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen_AOC())
	{
		int iResult = 300013000;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("AOC Door Open Error"), M_ICONINFORMATION);
		return;
	}
	//20171004 KDW End
#endif
//	목표위치로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50103,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
	m_plnkC_MngOpPanel->SetJogUnit();
	
	int iResult;
	if (m_iSelectedAxis == 0)
	{
		iResult = moveCameraCarrier(m_iSelectedPos, m_iSelectedTabNo);
	}
	else if (m_iSelectedAxis == 2)
	{
		iResult = m_plnkCamCarrier->SafeMoveExpand(m_iSelectedPos, TRUE, FALSE);
	}
//@	else if (m_iSelectedAxis == 2)
//@	{
//@		iResult = m_plnkPanelAligner->SafeMoveBackupMoveXPos(m_iSelectedPos);
//@	}
	else
	{
		iResult = m_plnkCamCarrier->SafeMoveBackupPos(m_iSelectedPos, TRUE, FALSE);
	}
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("CameraCarrier Teaching Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
}

void CTeachCameraCarrierDlg::OnClickPosSet() 
{
	CString	strMsg = "";
	if (m_iFixModel == DEF_FIX_MODE)
	{
		if (m_iSelectedAxis == 0)
		{
	//		Camera Carrier 고정좌표는 Teaching 할 수 없음.
			MyMessageBox(MY_MESSAGE_BOX,50104,_T("Information"), M_ICONINFORMATION);
			return;
		}
		else if (m_iSelectedAxis == 2)
		{
			MyMessageBox(MY_MESSAGE_BOX,50129,_T("Information"), M_ICONINFORMATION);
			return;
		}
	}

	WCHAR szTemp[250];	
	
//	목표 위치를 현재 위치로 변경하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50105,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL,_T("Set Target Position..."));
	ProgWnd.SetRange(0, 4);
	ProgWnd.SetText(_T("CameraCarrier's Target Position is setting to Current Position..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	double dCurrent = 0.0;
	if (m_iSelectedAxis == 0)
		dCurrent = m_plnkCamCarrier->GetCurrentPos();
	else if (m_iSelectedAxis == 1)
		dCurrent = m_plnkCamCarrier->GetCurrentPosBackup();
	else
		dCurrent = m_plnkCamCarrier->GetCurrentPosExpand();

	ProgWnd.StepIt();

	double dTargetPos = dCurrent;
	double dFixedPosData = m_rgdFixedPosData[m_iSelectedAxis][m_iSelectedPos];
	double dModelPosData = m_rgdModelPosData[m_iSelectedAxis][m_iSelectedPos];
	double dOffsetPosData = m_rgdOffsetPosData[m_iSelectedAxis][m_iSelectedPos];
	
	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_lblCoord[m_iSelectedAxis][0].SetCaption(szTemp);

	ProgWnd.StepIt();

	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		dOffsetPosData = dTargetPos - dModelPosData - dFixedPosData;
		swprintf(szTemp, _T("%.3f"), dOffsetPosData);
		// selsk_jdy
		//m_lblCoord[m_iSelectedAxis][2].SetCaption(szTemp);
		m_lblCoord[m_iSelectedAxis][3].SetCaption(szTemp);
		m_rgdOffsetPosData[m_iSelectedAxis][m_iSelectedPos] = dOffsetPosData;
		break;
	
	case DEF_FIX_MODE :
		dFixedPosData = dTargetPos - dModelPosData - dOffsetPosData;
		swprintf(szTemp, _T("%.3f"), dFixedPosData);
		m_lblCoord[m_iSelectedAxis][1].SetCaption(szTemp);
		m_rgdFixedPosData[m_iSelectedAxis][m_iSelectedPos] = dFixedPosData;
		break;
	}

	ProgWnd.StepIt();
}

void CTeachCameraCarrierDlg::OnClickCmOneTabPlus() 
{
	if (m_iSelectedAxis == 1)
		return;

	if (DEF_CAMERACARRIER_TAB_MARK_POS != m_iSelectedPos)
	{
//		Tab Mark Pos 선택 안됨.
		MyMessageBox(MY_MESSAGE_BOX,50106,_T("Error"), M_ICONERROR);
		return;
	}

	int iTargetTabNo = -1;
	iTargetTabNo = m_iSelectedTabNo + 1;

	if (iTargetTabNo < 0)
		iTargetTabNo = 0;
	else if (iTargetTabNo >= m_plnkWorkSchedule[m_iWorkScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP))
		iTargetTabNo = m_plnkWorkSchedule[m_iWorkScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP) - 1;

	// 삭제 예정. 확인 필요.
	// 이 버튼 반복적으로 누를 때 순차적으로 수행되는 것 같지 않음. 확인 할 것.
	TRACE2("%d, %d\n", m_iSelectedTabNo, iTargetTabNo);

	int iResult = moveCameraCarrier(m_iSelectedPos, iTargetTabNo);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("CameraCarrier Teaching Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	m_iSelectedTabNo = iTargetTabNo;

	if (TRUE == m_bTemporaryView)
	{
		IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
		int iCamNo = m_plnkCamCarrier->GetOwnedCameraNo1();
		iResult = plnkVision->Grab(iCamNo);
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult,_T("Vision Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
	}
}

void CTeachCameraCarrierDlg::OnClickCmOneTabMinus() 
{
	if (m_iSelectedAxis == 1)
		return;

	if (DEF_CAMERACARRIER_TAB_MARK_POS != m_iSelectedPos)
	{
//		Tab Mark Pos 선택 안됨.
		MyMessageBox(MY_MESSAGE_BOX,50107,_T("Error"), M_ICONERROR);
		return;
	}

	int iTargetTabNo = -1;
	iTargetTabNo = m_iSelectedTabNo - 1;
	
	if (iTargetTabNo < 0)
		iTargetTabNo = 0;
	else if (iTargetTabNo >= m_plnkWorkSchedule[m_iWorkScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP))
		iTargetTabNo = m_plnkWorkSchedule[m_iWorkScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP) - 1;

	int iResult = moveCameraCarrier(m_iSelectedPos, iTargetTabNo);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("CameraCarrier Teaching Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	m_iSelectedTabNo = iTargetTabNo;

	if (TRUE == m_bTemporaryView)
	{
		IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
		int iCamNo = m_plnkCamCarrier->GetOwnedCameraNo1();
		iResult = plnkVision->Grab(iCamNo);
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult,_T("Vision Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
	}
}

void CTeachCameraCarrierDlg::updateDisplay()
{
	int iNotUseAxis = (m_iSelectedAxis + 1) % DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE;
	int iNotUseAxis1 = (m_iSelectedAxis + 2) % DEF_CAMERACARRIER_TEACH_MAX_AXIS_TITLE;
	for (int i = 0; i < DEF_CAMERACARRIER_TEACH_MAX_COORDINATE_TYPE; i++)
	{
		m_lblCoord[iNotUseAxis][i].EnableWindow(FALSE);
		m_lblCoord[iNotUseAxis1][i].EnableWindow(FALSE);
		m_lblCoord[m_iSelectedAxis][i].EnableWindow(TRUE);
	}
	if (m_iSelectedAxis == 1 || m_iSelectedAxis == 2)
	{
		m_btnCmd[0].EnableWindow(FALSE);
		m_btnCmd[1].EnableWindow(FALSE);
		GetDlgItem(IDB_GET_ORIGIN_OFFSET)->EnableWindow(FALSE);
	}
	else
	{
		m_btnCmd[0].EnableWindow(TRUE);
		m_btnCmd[1].EnableWindow(TRUE);
		GetDlgItem(IDB_GET_ORIGIN_OFFSET)->EnableWindow(TRUE);
	}

	WCHAR szTemp[250];
	double dPosCurr = 0.0;	
	double dFixedPosData = m_rgdFixedPosData[m_iSelectedAxis][m_iSelectedPos];
	double dModelPosData = m_rgdModelPosData[m_iSelectedAxis][m_iSelectedPos];
	double dOffsetPosData = m_rgdOffsetPosData[m_iSelectedAxis][m_iSelectedPos];

	double dTargetPos = dFixedPosData + dModelPosData + dOffsetPosData;

	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_lblCoord[m_iSelectedAxis][0].SetCaption(szTemp);	

#ifndef SIMULATION
	if (m_iSelectedAxis == 0)
	{
		dPosCurr = m_plnkCamCarrier->GetCurrentPos();
	}
	else if (m_iSelectedAxis == 1)
	{
		dPosCurr = m_plnkCamCarrier->GetCurrentPosBackup();
	}
	else
	{
		dPosCurr = m_plnkCamCarrier->GetCurrentPosExpand();
	}
//@	else
//@	{
//@		dPosCurr = m_plnkPanelAligner->GetBackupMoveXCurrentPos();
//@	}
#endif /* SIMULATION */

	// Reset Switch 감지 시 현재 위치를 0으로 표시되게 하여 
	// Teaching시 상대거리 측정을 쉽게 한다.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
		m_rgdDisplayOffset[m_iSelectedAxis] = dPosCurr;

	dPosCurr -= m_rgdDisplayOffset[m_iSelectedAxis];

	/** 현재 위치 Display */
	swprintf(szTemp, _T("%.3f"), dPosCurr);
	m_lblCoord[m_iSelectedAxis][4].SetCaption(szTemp);
	
	/** 편차 계산하고, Display */
	swprintf(szTemp, _T("%.3f"), dPosCurr - dTargetPos);
	m_lblCoord[m_iSelectedAxis][5].SetCaption(szTemp);	

	//@__________________
	swprintf(szTemp, _T("TabNo : %d"), m_iSelectedTabNo+1);
	m_lblSelTabNo.SetCaption(szTemp);
	//@__________________
}

void CTeachCameraCarrierDlg::updateCoordinate()
{
	WCHAR szBuff[250];

	double dFixedPosData = m_rgdFixedPosData[m_iSelectedAxis][m_iSelectedPos];
	double dModelPosData = m_rgdModelPosData[m_iSelectedAxis][m_iSelectedPos];
	double dOffsetPosData = m_rgdOffsetPosData[m_iSelectedAxis][m_iSelectedPos];

	memset(szBuff, 0x00, sizeof(szBuff));
	
	swprintf(szBuff, _T("%.3f"), dFixedPosData);
	m_lblCoord[m_iSelectedAxis][1].SetWindowText(szBuff);

	swprintf(szBuff, _T("%.3f"), dModelPosData);
	m_lblCoord[m_iSelectedAxis][2].SetWindowText(szBuff);

	swprintf(szBuff, _T("%.3f"), dOffsetPosData);
	m_lblCoord[m_iSelectedAxis][3].SetWindowText(szBuff);

	updateDisplay();
}

void CTeachCameraCarrierDlg::selectPosition(int iPosIndex, BOOL bAskTabNoSkip)
{
	if (m_iSelectedAxis == 0)
	{
		int iInspectorNo = m_plnkCamCarrier->GetInspectCamCarrierNo();
		if (DEF_NONE_INSPECTION_CAMERA != iInspectorNo)
		{
			if (iPosIndex == DEF_CAMERACARRIER_TAB_MARK_POS
				|| iPosIndex == DEF_CAMERACARRIER_TAB_MOUNT_POS) //170826 JSH
			{
				if (FALSE == m_plnkWorkSchedule[m_iWorkScheduleNo]->GetUseWorkSide())
				{
					MyMessageBox(MY_MESSAGE_BOX, 50128, _T("Confirm"), M_ICONINFORMATION);
					m_btnPos[iPosIndex].SetValue(FALSE);
					m_btnPos[m_iSelectedPos].SetValue(TRUE);
					return;
				}
				if (FALSE == bAskTabNoSkip)
				{
					if (ERR_CAMERACARRIER_TEACH_DLG_CANCEAL == getWorkTabNo())
					{
						m_btnPos[iPosIndex].SetValue(FALSE);
						m_btnPos[m_iSelectedPos].SetValue(TRUE);
						return;
					}
				}
				
				ASSERT(m_iSelectedTabNo != -1);
//				EWorkingSide eWorkSide = m_plnkProcessData->GetCurrentWorkingSide(m_iGroupNo);
				//110124.Modify_____
				/*
				m_rgdModelPosData[m_iSelectedAxis][iPosIndex] = m_plnkWorkSchedule->GetTabPos(m_iSelectedTabNo);
				if (m_plnkPreBonderData->m_dLead_Width_Xtab < DEF_LARGE_TYPE_TAB_WIDTH_STANDARD)
				{
					int iInstance = m_plnkCamCarrier->GetInspectCamCarrierNo();//m_plnkCamCarrier->GetInstanceNo();
					m_rgdModelPosData[m_iSelectedAxis][iPosIndex] += m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[m_iGroupNo][iInstance];
				}
				*/
				m_plnkCamCarrier->GetPosParameter(m_iWorkScheduleNo, NULL, NULL, m_rgdModelPosData[m_iSelectedAxis]);
				m_rgdModelPosData[m_iSelectedAxis][iPosIndex] += m_plnkWorkSchedule[m_iWorkScheduleNo]->GetTabPos(m_iSelectedTabNo);
			}
			else
			{
				///임시
				///m_rgdModelPosData[m_iSelectedAxis][iPosIndex] = 0.0;
			}
		}
	}

	m_btnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_btnPos[iPosIndex].SetValue(TRUE);

	updateCoordinate();
}

// Doolin kwangilshin 2017. 08. 24.
//
void CTeachCameraCarrierDlg::Init_GridAxisStste()
{	
	int		nGrdColWidth = 0;
	CRect	clsRect;
	
	//for (int nCnt = 0; nCnt < eAxisEnd; nCnt++)
	//{
	//	m_grd_AxisSnsState[nCnt].SetCols(AxisSnsStsEx::AxisStsEnd);
	//	m_grd_AxisSnsState[nCnt].SetGridColor(BLUE);
	//	m_grd_AxisSnsState[nCnt].SetForeColor(YELLOW);
	//	m_grd_AxisSnsState[nCnt].GetClientRect(&clsRect);

	//	nGrdColWidth = ((clsRect.Width() * DEF_AXIS_STS_GRD_WIDTH) / (AxisSnsStsEx::AxisStsEnd));

	//	for (int nGrdCnt = 0; nGrdCnt < AxisSnsStsEx::AxisStsEnd; nGrdCnt++)
	//	{
	//		m_grd_AxisSnsState[nCnt].SetColWidth(nGrdCnt, nGrdColWidth);	
	//		m_grd_AxisSnsState[nCnt].SetColAlignment(nGrdCnt, 3);
	//		m_grd_AxisSnsState[nCnt].SetTextMatrix(0, nGrdCnt, AxisSnsStsEx::strCvtName[nGrdCnt]);

	//		m_grd_AxisSnsState[nCnt].SetRow(0);
	//		m_grd_AxisSnsState[nCnt].SetCol(nGrdCnt);
	//		m_grd_AxisSnsState[nCnt].SetCellBackColor(BLACK);
	//	}
	//}
}


BOOL CTeachCameraCarrierDlg::Check_AxisStste(IAxis *pclsAxis)
{	
	ASSERT(pclsAxis);
	if (pclsAxis)
	{
		if (pclsAxis->IsAmpFault() || pclsAxis->IsEStopEvent())
		{
			pclsAxis->ResetAmpFault(TRUE);
			Sleep(50);
		}
		
		if (!pclsAxis->IsAmpEnable())
		{
			pclsAxis->ServoOn(NULL, TRUE);

			return FALSE;
		}
	}

	return TRUE;
}


void CTeachCameraCarrierDlg::SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome)
{
	ASSERT(pclsGrd);

	if (pclsGrd)
	{
		COLORREF clrBack = BLACK;
		COLORREF clrFore = YELLOW;
		
		if (bIsHome)	
		{	
			clrBack = GREEN;	
			clrFore = BLUE;	
		}
		
		pclsGrd->SetRow(0);
		pclsGrd->SetCol(AxisSnsStsEx::H);

		if (pclsGrd->GetCellBackColor() != clrBack)
		{
			pclsGrd->SetCellForeColor(clrFore);
			pclsGrd->SetCellBackColor(clrBack);
		}
	}
}


void CTeachCameraCarrierDlg::SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
{
	ASSERT(pclsGrd);
	
	if (pclsGrd)
	{
		COLORREF clrBack = BLACK;
		COLORREF clrFore = YELLOW;

		if (bIsOn)	
		{	
			clrBack = RED;	
			clrFore = YELLOW;	
		}
		
		pclsGrd->SetRow(0);
		pclsGrd->SetCol(AxisSnsStsEx::P);
		
		if (pclsGrd->GetCellBackColor() != clrBack)
		{
			pclsGrd->SetCellForeColor(clrFore);
			pclsGrd->SetCellBackColor(clrBack);
		}
	}
}


void CTeachCameraCarrierDlg::SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
{
	ASSERT(pclsGrd);
	
	if (pclsGrd)
	{
		COLORREF clrBack = BLACK;	
		COLORREF clrFore = YELLOW;

		if (bIsOn)	
		{	
			clrBack = RED;	
			clrFore = YELLOW;	
		}
		
		pclsGrd->SetRow(0);
		pclsGrd->SetCol(AxisSnsStsEx::N);
		
		if (pclsGrd->GetCellBackColor() != clrBack)
		{
			pclsGrd->SetCellForeColor(clrFore);
			pclsGrd->SetCellBackColor(clrBack);
		}
	}
}


void CTeachCameraCarrierDlg::SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable)
{
	ASSERT(pclsGrd);
	
	if (pclsGrd)
	{
		COLORREF clrBack = BLACK;
		COLORREF clrFore = YELLOW;
		
		if (bIsEnable)	
		{	
			clrBack = GREEN;	
			clrFore = BLUE;	
		}
		
		pclsGrd->SetRow(0);
		pclsGrd->SetCol(AxisSnsStsEx::S);
		
		if (pclsGrd->GetCellBackColor() != clrBack)
		{
			pclsGrd->SetCellForeColor(clrFore);
			pclsGrd->SetCellBackColor(clrBack);
		}
	}
}


void CTeachCameraCarrierDlg::updateAxisSnsSts()
{
	IAxis *pclsAxis[eAxisEnd];
	
	pclsAxis[eAxisX]	= m_plnkCamCarrier->GetMotionComponent();
	pclsAxis[eAxisZ]	= m_plnkCamCarrier->GetMotionComponentBackup();
	pclsAxis[eAxisExp]	= m_plnkCamCarrier->GetMotionComponentExpand();

	//for (int nAxisCnt = 0; nAxisCnt < eAxisEnd; nAxisCnt++)
	//{
	//	if (!pclsAxis[nAxisCnt])
	//	{
	//		continue;
	//	}

	//	if (pclsAxis[nAxisCnt]->IsAmpEnable())
	//	{	
	//		SetAxisAmpEnable(&m_grd_AxisSnsState[nAxisCnt], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisAmpEnable(&m_grd_AxisSnsState[nAxisCnt], FALSE);
	//	}

	//	if (pclsAxis[nAxisCnt]->IsOriginReturn())
	//	{	
	//		SetAxisHomeState(&m_grd_AxisSnsState[nAxisCnt], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisHomeState(&m_grd_AxisSnsState[nAxisCnt], FALSE);
	//	}

	//	if (pclsAxis[nAxisCnt]->IsOnNegativeSensor())
	//	{	
	//		SetAxisNLimitSensor(&m_grd_AxisSnsState[nAxisCnt], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisNLimitSensor(&m_grd_AxisSnsState[nAxisCnt], FALSE);
	//	}

	//	if (pclsAxis[nAxisCnt]->IsOnPositiveSensor())
	//	{	
	//		SetAxisPLimitSensor(&m_grd_AxisSnsState[nAxisCnt], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisPLimitSensor(&m_grd_AxisSnsState[nAxisCnt], FALSE);
	//	}

	//	if (pclsAxis[nAxisCnt]->IsAmpFault() || pclsAxis[nAxisCnt]->IsEStopEvent())
	//	{	
	//		m_ctrlBtnAxis[nAxisCnt].SetBackColorInterior(RGB(192, 0, 0));			
	//	}
	//	else
	//	{
	//		m_ctrlBtnAxis[nAxisCnt].SetBackColorInterior(BASIC);
	//	}
	//}
}
// End.


void CTeachCameraCarrierDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		updateDisplay();
		updateButton();

		// Doolin kwangilshin 2017. 08. 25.
		//
		updateAxisSnsSts();
		// End.
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTeachCameraCarrierDlg::updateButton()
{
}

void CTeachCameraCarrierDlg::OnClickSelectPrevios() 
{
	int iPrevLimitNo = -1;
	int iMinusGap = 0;
	/*
	if (m_iGroupNo == DEF_INSPECTION_CAMERA_NONE_GROUP)
	{
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
			return;

		iPrevLimitNo = DEF_CAMERACARRIER1;
		iMinusGap = 1;
	}
	else if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		// 확인 필요 : m_iGroupNo != 0 인 경우 에러 처리.

		iPrevLimitNo = DEF_CAMERACARRIER1;
		iMinusGap = 2;
	}
	else
	{
		if (m_iGroupNo == DEF_FRONT_GROUP)
		{
			iPrevLimitNo = DEF_CAMERACARRIER3;
			iMinusGap = 1;
		}
		else	// if (m_iGroupNo == DEF_REAR_GROUP)
		{
			iPrevLimitNo = DEF_CAMERACARRIER6;
			iMinusGap = 1;
		}
	}
	*/
	iMinusGap = 1;
	iPrevLimitNo = DEF_INSPECTION_CARRIER_1;

	if (m_iCameraCarrierID - iMinusGap < iPrevLimitNo)
		return;

	// 확인 필요 : Timer 와의 충돌 피하기 위한 처리 필요한가?
	m_iCameraCarrierOldID = m_iCameraCarrierID;

	m_iCameraCarrierID -= iMinusGap;

	m_btnNextCameraCarrier.EnableWindow(TRUE);

	if (m_iCameraCarrierID - iMinusGap < iPrevLimitNo)
		m_btnPrevCameraCarrier.EnableWindow(FALSE);

	WCHAR szTemp[250];
	CMyProgressWnd ProgWnd(NULL,_T("Change Worker..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("CameraCarrier is Changing...Wait a moment.."));
	ProgWnd.PeekAndPump();
	
	ProgWnd.StepIt();
	
	swprintf(szTemp,_T("%d"), m_iCameraCarrierID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
	//swprintf(szTemp,_T("Camera\nCarrier%d"), m_iCameraCarrierID + 1);
	//m_ctrlBtnAxis[m_iSelectedAxis].SetWindowText(szTemp);

//@#ifdef DEF_SOURCE_SYSTEM
//@	swprintf(szTemp, _T("Camera\nCarrier%d Y"), m_iCameraCarrierID + 1);
//@#else
	swprintf(szTemp, _T("Camera\nCarrier%d X"), m_iCameraCarrierID + 1);
//@#endif
	
	m_ctrlBtnAxis[DEF_CAMERACARRIER_TEACH_CAMERA_Y_AXIS_TITLE].SetWindowText(szTemp);

	swprintf(szTemp, _T("Backup\n%d Z"), m_iCameraCarrierID + 1);
	m_ctrlBtnAxis[DEF_CAMERACARRIER_TEACH_BACKUP_Z_AXIS_TITLE].SetWindowText(szTemp);
	
	setWorkCameraCarrier(m_iCameraCarrierID);
	
	ProgWnd.StepIt();
}

void CTeachCameraCarrierDlg::OnClickSelectNext() 
{
	int iNextLimitNo = -1;
	int iPlusGap = 0;
	/*
	if (m_iGroupNo == DEF_INSPECTION_CAMERA_NONE_GROUP)
	{
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
			return;

		iNextLimitNo = DEF_CAMERACARRIER2;
		iPlusGap = 1;
	}
	else if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		// 확인 필요 : m_iGroupNo != 0 인 경우 에러 처리.

		iNextLimitNo = DEF_CAMERACARRIER3;
		iPlusGap = 2;
	}
	else
	{
		if (m_iGroupNo == DEF_FRONT_GROUP)
		{
			iNextLimitNo = DEF_CAMERACARRIER5;
			iPlusGap = 1;
		}
		else	// if (m_iGroupNo == DEF_REAR_GROUP)
		{
			iNextLimitNo = DEF_CAMERACARRIER8;
			iPlusGap = 1;
		}
	}
	*/
	iPlusGap = 1;
	iNextLimitNo = DEF_MAX_INSPECTION_CAM_CARRIER-1;

	if (m_iCameraCarrierID + iPlusGap > iNextLimitNo)
		return;

	// 확인 필요 : Timer 와의 충돌 피하기 위한 처리 필요한가?
	m_iCameraCarrierOldID = m_iCameraCarrierID;

	m_iCameraCarrierID += iPlusGap;

	m_btnPrevCameraCarrier.EnableWindow(TRUE);

	if (m_iCameraCarrierID + iPlusGap > iNextLimitNo)
		m_btnNextCameraCarrier.EnableWindow(FALSE);

	// 확인 필요 : 코드 정리.
	WCHAR szTemp[250];
	CMyProgressWnd ProgWnd(NULL,_T("Change Worker..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("CameraCarrier is Changing..Wait a moment.."));
	ProgWnd.PeekAndPump();
	
	ProgWnd.StepIt();
	
	swprintf(szTemp,_T("%d"), m_iCameraCarrierID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
	//swprintf(szTemp,_T("Camera\nCarrier%d"), m_iCameraCarrierID + 1);
	//m_ctrlBtnAxis[m_iSelectedAxis].SetWindowText(szTemp);
//@#ifdef DEF_SOURCE_SYSTEM
//@	swprintf(szTemp, _T("Camera\nCarrier%d Y"), m_iCameraCarrierID + 1);
//@#else
	swprintf(szTemp, _T("Camera\nCarrier%d X"), m_iCameraCarrierID + 1);
//@#endif

	m_ctrlBtnAxis[DEF_CAMERACARRIER_TEACH_CAMERA_Y_AXIS_TITLE].SetWindowText(szTemp);

	swprintf(szTemp, _T("Backup\n%d Z"), m_iCameraCarrierID + 1);
	m_ctrlBtnAxis[DEF_CAMERACARRIER_TEACH_BACKUP_Z_AXIS_TITLE].SetWindowText(szTemp);
	
	setWorkCameraCarrier(m_iCameraCarrierID);
	
	ProgWnd.StepIt();
}

void CTeachCameraCarrierDlg::OnGetOriginOffset() 
{
	CString strMsg = "";
		
	//CameraCarrier 원점복귀 Offset 얻기에 TabMounter 를 이용합니다.\r\n원점복귀 Offset얻기 동작을 하겠습니까?
	int iResult = MyMessageBox(MY_MESSAGE_BOX,50114,_T("Check Pre-Condition"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
	{
		return;
	}

	MCameraCarrier* pCamCarrier;
	MTabMounter*	pTabMounter;

	if (m_iGroupNo == DEF_FRONT_GROUP)
	{
		pCamCarrier = MOlbSystemPre.GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
		pTabMounter = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER4);
	}
	else
	{
		pCamCarrier = MOlbSystemPre.GetRCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
		pTabMounter = MOlbSystemPre.GetRTabMounterComponent(DEF_TABMOUNTER4);
	}

	if (m_iCameraCarrierID == DEF_INSPECTION_CARRIER_1)
	{
		//Inspection Carrier1번은 원점Offset얻기 불필요
		MyMessageBox(MY_MESSAGE_BOX, 50125, _T("Confirm"), M_ICONINFORMATION);
		return;
	}

	if (FALSE == m_plnkCamCarrier->GetMotionComponent()->IsOriginReturn()
		|| FALSE == pCamCarrier->GetMotionComponent()->IsOriginReturn())
	{
		//CameraCarrier 원점 복귀 안됨.
		MyMessageBox(MY_MESSAGE_BOX,50118,_T("Error"), M_ICONERROR);
		return;
	}


	iResult = pCamCarrier->SafeMoveExpand(DEF_CAMERACARRIER_EXPAND_INIT_POS, TRUE, FALSE);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}


	iResult = m_plnkCamCarrier->SafeMoveExpand(DEF_CAMERACARRIER_EXPAND_INIT_POS, TRUE, FALSE);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = pTabMounter->DownCyl();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = pTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_INSPECTION_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	if (NULL != m_pdlgView)
	{
		m_pdlgView->DestroyWindow();
		delete m_pdlgView;
		m_pdlgView = NULL;
	}
	
	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
	m_pdlgView = new CVisionViewDlg(plnkVision);
	m_pdlgView->Create(IDD_VISION_VIEW);
	m_pdlgView->ShowWindow(SW_SHOW);

	plnkVision->HaltVideo(pCamCarrier->GetOwnedCameraNo1());
	plnkVision->ClearOverlay(pCamCarrier->GetOwnedCameraNo1());
	plnkVision->LiveVideo(pCamCarrier->GetOwnedCameraNo1());

	int iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
//@	m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABMOUNTER4_Y);
	if (m_iGroupNo == DEF_FRONT_GROUP)
#ifdef DEF_SOURCE_SYSTEM
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABMOUNTER4_X);
#else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABMOUNTER2_X);
#endif
	else
#ifdef DEF_SOURCE_SYSTEM
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABMOUNTER4_X);
#else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABMOUNTER2_X);
#endif

	//Message_50126=InspectionCamera1번 중앙에 Mounter4번 Mark가 보이면 확인을 눌러 주세요.
	if (MyMessageBox(MY_MESSAGE_BOX, 50126, _T("Check Mark"), M_ICONQUESTION|M_YESNO) == IDNO)
	{
		plnkVision->HaltVideo(pCamCarrier->GetOwnedCameraNo1());
		if (NULL != m_pdlgView)
		{
			m_pdlgView->DestroyWindow();
			delete m_pdlgView;
			m_pdlgView = NULL;
		}
		m_bTemporaryView = FALSE;
		m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
		return;
	}

	plnkVision->HaltVideo(pCamCarrier->GetOwnedCameraNo1());
	double dMounterPos1 = pTabMounter->GetCurrentPos();
	double dCamCarrierPos1 = pCamCarrier->GetCurrentPos();


	plnkVision->HaltVideo(m_plnkCamCarrier->GetOwnedCameraNo1());
	plnkVision->ClearOverlay(m_plnkCamCarrier->GetOwnedCameraNo1());
	plnkVision->LiveVideo(m_plnkCamCarrier->GetOwnedCameraNo1());

	//Message_50126=InspectionCamera1번 중앙에 Mounter4번 Mark가 보이면 확인을 눌러 주세요.
	if (MyMessageBox(MY_MESSAGE_BOX, 50127, _T("Check Mark"), M_ICONQUESTION|M_YESNO) == IDNO)
	{
		plnkVision->HaltVideo(m_plnkCamCarrier->GetOwnedCameraNo1());
		if (NULL != m_pdlgView)
		{
			m_pdlgView->DestroyWindow();
			delete m_pdlgView;
			m_pdlgView = NULL;
		}
		m_bTemporaryView = FALSE;
		m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
		return;
	}

	plnkVision->HaltVideo(m_plnkCamCarrier->GetOwnedCameraNo1());
	if (NULL != m_pdlgView)
	{
		m_pdlgView->DestroyWindow();
		delete m_pdlgView;
		m_pdlgView = NULL;
	}
	m_bTemporaryView = FALSE;
	m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
	pTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS, TRUE, FALSE);

	double dMounterPos2 = pTabMounter->GetCurrentPos();
	double dCamCarrierPos2 = m_plnkCamCarrier->GetCurrentPos();

	double dCenterToCenterDist = (dMounterPos2 - dMounterPos1);

	double dDiff = (dCamCarrierPos2 - dCamCarrierPos1) - dCenterToCenterDist;

	MAxisData sAxisData;
	m_plnkCamCarrier->GetMotionComponent()->GetParameter(&sAxisData);
	double dHomeOffset = sAxisData.dHomeOffset;
	
	dHomeOffset += dDiff;
	sAxisData.dHomeOffset = dHomeOffset;
	m_plnkCamCarrier->GetMotionComponent()->SaveParameter(&sAxisData);
	
	// 원점 복귀 여부를 Clear 한다.
	m_plnkCamCarrier->GetMotionComponent()->ResetOrigin();
	m_plnkCamCarrier->GetMotionComponent()->ResetOriginComplete();
		

	// 원점 복귀 재수행을 사용자에게 요구한다.	
	//		Inspection Camera Carrier 원점 복귀 Offset 계산 완료.\r\n원점 복귀 완료 Flag 를 해제함.
	MyMessageBox(MY_MESSAGE_BOX,50121,_T("Operation Complete"), M_ICONINFORMATION);
}

int CTeachCameraCarrierDlg::getWorkTabNo()
{
	WCHAR szBeforeVal[100];
	WCHAR szAfterVal[100];

	memset(szBeforeVal, 0, sizeof(szBeforeVal));
	memset(szAfterVal, 0, sizeof(szAfterVal));
	
//	이동 위치의 Tab 번호
	CString strMsg;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_17"), &strMsg);
	
	swprintf(szBeforeVal, _T("%d"), m_iSelectedTabNo + 1);
	if (FALSE == GetNumberBox(szAfterVal, 10, szBeforeVal, strMsg.GetBuffer(strMsg.GetLength())))
		return ERR_CAMERACARRIER_TEACH_DLG_CANCEAL;
	
	int iTabNo = (int)wcstod(szAfterVal,NULL) - 1;
//	EWorkingSide eWorkSide = m_plnkProcessData->GetCurrentWorkingSide(m_iGroupNo);
//	if (iTabNo < -1)
//		iTabNo = 0;
//	else if (iTabNo >= m_plnkProcessData->GetMaxTabQty(eWorkSide))
//		iTabNo = m_plnkProcessData->GetMaxTabQty(eWorkSide) - 1;

	if (iTabNo < -1)
		iTabNo = 0;
	else if (iTabNo >= m_plnkWorkSchedule[m_iWorkScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP))
		iTabNo = m_plnkWorkSchedule[m_iWorkScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP) - 1;
	
	m_iSelectedTabNo = iTabNo;
	return ERR_CAMERACARRIER_TEACH_DLG_SUCCESS;
}

int CTeachCameraCarrierDlg::moveCameraCarrier(int iPosID, int iTabNo)
{
	CMyProgressWnd ProgWnd(NULL,_T("Move Target Position..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("CameraCarrier is Moving to Target Position...Wait a moment..."));
	ProgWnd.PeekAndPump();

	// 이동 중 Jog를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
	int iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

	int iCurrentSelectedTabNo = -1;
	if (DEF_CAMERACARRIER_TAB_MARK_POS == iPosID)
		iCurrentSelectedTabNo = iTabNo;

	int iResult = m_plnkCamCarrier->SafeMovePos(m_iWorkScheduleNo, iPosID, TRUE, iCurrentSelectedTabNo);
	
	m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);	// Jog 사용 재개
	ProgWnd.StepIt();

	return iResult;
}

void CTeachCameraCarrierDlg::OnTemporaryView() 
{
	if (FALSE == m_bTemporaryView)
	{
		if (NULL != m_pdlgView)
		{
			m_pdlgView->DestroyWindow();
			delete m_pdlgView;
			m_pdlgView = NULL;
		}

		IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
		m_pdlgView = new CVisionViewDlg(plnkVision);
		m_pdlgView->Create(IDD_VISION_VIEW);
		m_pdlgView->ShowWindow(SW_SHOW);
		m_bTemporaryView = TRUE;
	}
	else
	{
		if (NULL != m_pdlgView)
		{
			m_pdlgView->DestroyWindow();
			delete m_pdlgView;
			m_pdlgView = NULL;
		}
		m_bTemporaryView = FALSE;
	}
}

void CTeachCameraCarrierDlg::OnClickSelectSchedulePrev() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[250];
	if (m_iWorkScheduleNo <= 0) return;
	m_iWorkScheduleNo--;
	swprintf(szTemp, _T("Schedule\r\nNO.%d"), m_iWorkScheduleNo+1);
	m_lblScheduleNo.SetWindowText(szTemp);

	m_iCameraCarrierOldID = -1;
	setWorkCameraCarrier(m_iCameraCarrierID);

}

void CTeachCameraCarrierDlg::OnClickSelectScheduleNext() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[250];
	if (m_iWorkScheduleNo >= DEF_MAX_WORK_SCHEDULE-1) return;
	m_iWorkScheduleNo++;
	swprintf(szTemp, _T("Schedule\r\nNO.%d"), m_iWorkScheduleNo+1);
	m_lblScheduleNo.SetWindowText(szTemp);

	m_iCameraCarrierOldID = -1;
	setWorkCameraCarrier(m_iCameraCarrierID);
}
