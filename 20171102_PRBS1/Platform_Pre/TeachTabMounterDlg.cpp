// TeachTabMounterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "TeachTabMounterDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTabMounter.h"
#include "MCameraCarrier.h"
#include "MTabAligner.h"
#include "MManageOpPanel.h"
#include "VisionMonitorDlg.h"
#include "VisionCalibDlg.h"
#include "IAxis.h"
#include "MAxisData.h"
#include "MProcessData.h"
// jdy2
#include "MCameraCarrier.h"
#include "DlgViewWorkSchedule.h"
#include "MTrsAutoManager.h"


#include "TeachMainForm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachTabMounterDlg dialog

CTeachTabMounterDlg::CTeachTabMounterDlg(int iFixModel, int iTabMounterGroupNo, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachTabMounterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeachTabMounterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iGroupNo = iTabMounterGroupNo;
	m_iFixModel = iFixModel;
	m_iSelectedPos = 0;
	m_iTabMounterOldID = -1;
	m_iSelectedAxis = 0;
	m_iSelectedTabNo = -1;

	m_bGantryMoveSts = FALSE;

	for (int i = 0; i < DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE; i++)
		m_rgdDisplayOffset[i] = 0.0;

	m_plnkC_MngOpPanel	= MOlbSystemPre.GetManageOpPanelComponent();
	m_plnkSystemData	= MOlbSystemPre.GetSystemData();
	m_plnkProcessData	= MOlbSystemPre.GetProcessData();
//@#ifdef DEF_SOURCE_SYSTEM
//@	m_plnkWorkSchedule	= MOlbSystemPre.GetWorkSchedule(DEF_FRONT_GROUP);
//@#else
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST]		= MOlbSystemPre.GetWorkSchedule(m_iGroupNo);
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND]	= MOlbSystemPre.GetWorkScheduleSecond(m_iGroupNo);
#endif
//@#endif

//	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
//@#ifdef DEF_SOURCE_SYSTEM
//@	{
//@		// 확인 필요 : m_iGroupNo != 0 인 경우 에러 처리.
//@		m_iTabMounterID = DEF_TABMOUNTER8;
//@		m_plnkCameraCarrier = MOlbSystemPre.GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
//@	}
//@//	else
//@#else
	{
//@		m_iTabMounterID = DEF_TABMOUNTER4;
		m_iTabMounterID = DEF_MAX_TABMOUNTER-1;
		if (DEF_FRONT_GROUP == m_iGroupNo)
			m_plnkCameraCarrier = MOlbSystemPre.GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
		else
			m_plnkCameraCarrier = MOlbSystemPre.GetRCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
	}
//	m_iGantryLoadPosID = 0;
//@#endif
	m_iWorkScheduleNo = DEF_WORK_SCHEDULE_FIRST;
}

void CTeachTabMounterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachTabMounterDlg)
	DDX_Control(pDX, IDB_SELECT_NEXT, m_btnNextTabMounter);
	DDX_Control(pDX, IDB_SELECT_PREVIOS, m_btnPrevTabMounter);

	
	//DDX_Control(pDX, IDC_GRD_AXIS_MOUNT_SNS_STS,	m_grd_AxisSnsState[eAxisTabMount]);
	//DDX_Control(pDX, IDC_GRD_AXIS_MOUNT_Z_SNS_STS,	m_grd_AxisSnsState[eAxisTabMountZ]);
	//DDX_Control(pDX, IDC_GRD_AXIS_GANTRY_SNS_STS,	m_grd_AxisSnsState[eAxisGantry]);

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
	DDX_Control(pDX, IDC_PRE_OFFSET_POS1, m_lblCoord[0][4]); //SJ_YYK 150109 Add..
	DDX_Control(pDX, IDC_CUR_POS1, m_lblCoord[0][5]);
	DDX_Control(pDX, IDC_DIF_POS1, m_lblCoord[0][6]);

	DDX_Control(pDX, IDC_TGT_POS2, m_lblCoord[1][0]);
	DDX_Control(pDX, IDC_FIX_POS2, m_lblCoord[1][1]);
	DDX_Control(pDX, IDC_MODEL_POS2, m_lblCoord[1][2]);
	DDX_Control(pDX, IDC_OFFSET_POS2, m_lblCoord[1][3]);
	DDX_Control(pDX, IDC_PRE_OFFSET_POS2, m_lblCoord[1][4]); //SJ_YYK 150109 Add..
	DDX_Control(pDX, IDC_CUR_POS2, m_lblCoord[1][5]);
	DDX_Control(pDX, IDC_DIF_POS2, m_lblCoord[1][6]);

	DDX_Control(pDX, IDC_TGT_POS3, m_lblCoord[2][0]);
	DDX_Control(pDX, IDC_FIX_POS3, m_lblCoord[2][1]);
	DDX_Control(pDX, IDC_MODEL_POS3, m_lblCoord[2][2]);
	DDX_Control(pDX, IDC_OFFSET_POS3, m_lblCoord[2][3]);
	DDX_Control(pDX, IDC_PRE_OFFSET_POS3, m_lblCoord[2][4]); //SJ_YYK 150109 Add..
	DDX_Control(pDX, IDC_CUR_POS3, m_lblCoord[2][5]);
	DDX_Control(pDX, IDC_DIF_POS3, m_lblCoord[2][6]);

	DDX_Control(pDX, IDC_WORKER_NO, m_lblWorkerNo);
	DDX_Control(pDX, IDB_AXIS1, m_ctrlBtnAxis[eAxisTabMount]);
	DDX_Control(pDX, IDB_AXIS2, m_ctrlBtnAxis[eAxisTabMountZ]);
	DDX_Control(pDX, IDB_AXIS3, m_ctrlBtnAxis[eAxisGantry]);

	DDX_Control(pDX, IDB_GANTRY_ENABLE, m_btnGantryEnable);

	DDX_Control(pDX, IDC_SEL_TAB_NO, m_lblSelTabNo);
	DDX_Control(pDX, IDS_SCHEDULE_NO, m_lblScheduleNo);	
}


BEGIN_MESSAGE_MAP(CTeachTabMounterDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachTabMounterDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachTabMounterDlg message handlers

void CTeachTabMounterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CTeachTabMounterDlg::updatePosButton()
{
	// Axis Name Display
	int i;
	for (i = 0; i < DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE; i++)
	{
		if (m_iSelectedAxis == i)
			m_ctrlBtnAxis[i].EnableWindow(TRUE);
		else
			m_ctrlBtnAxis[i].EnableWindow(FALSE);
	}

	STeachingInfo sTeachingInfo = m_plnkTabMounter->GetTeachingInfo(m_iSelectedAxis);
	int iMaxPos = m_plnkTabMounter->GetMaxPosNum(m_iSelectedAxis);

	for (i = 0; i < DEF_TABMOUNTER_TEACH_MAX_POS_BTN; i++)
		m_btnPos[i].EnableWindow(TRUE);

	int iSelected = -1;
	for (i = 0; i < iMaxPos; i++)
	{
		if ((sTeachingInfo.m_sPosition[i]).m_bUse == TRUE)
		{
			m_btnPos[i].SetWindowText((LPCTSTR)((sTeachingInfo.m_sPosition[i]).m_strNewLineName[0]));
			
			if (sTeachingInfo.m_sPosition[i].m_bEnable == FALSE)
				m_btnPos[i].EnableWindow(FALSE);
			else
			{
				m_btnPos[i].ShowWindow(SW_SHOW);
				m_btnPos[i].EnableWindow(TRUE);
			}

			// 설정에 따라 Button Disable
			if (iSelected == -1 && m_iSelectedPos == i && sTeachingInfo.m_sPosition[i].m_bEnable == TRUE)
				iSelected = i;
		}
		else
			m_btnPos[i].EnableWindow(FALSE);
	}
	
	// 사용하지 않는 Position Button 감추기
	for (; i < DEF_TABMOUNTER_TEACH_MAX_POS_BTN; i++)
		m_btnPos[i].ShowWindow(SW_HIDE);

	// 기본 selectPosition 설정 하기
	if (iSelected != -1)
		selectPosition(iSelected, TRUE);
	else
		selectPosition(0, TRUE);
}

void CTeachTabMounterDlg::setWorkTabMounter(const int iTabMounterID)
{
	if (m_iTabMounterOldID == iTabMounterID)
		return;

	if (-1 != m_iTabMounterOldID)
//		MOlbSystemPre.GetTabMounterComponent(m_iTabMounterOldID)->SetGantryMotionEnable(FALSE, 0);
		MOlbSystemPre.GetTabMounterComponent(m_iTabMounterOldID)->SetGantryMotionEnable(FALSE);

//@#ifdef DEF_SOURCE_SYSTEM
//@	m_plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(iTabMounterID);
//@#else
	if (m_iGroupNo == DEF_FRONT_GROUP)
		m_plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(iTabMounterID);
	else
		m_plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(iTabMounterID);
//@#endif

	m_bVacSts = !m_plnkTabMounter->IsAbsorbTabIC();
	m_bCylSts = !m_plnkTabMounter->IsUpCyl();

	if (DEF_GANTRY == m_iSelectedAxis)
	{
		m_iSelectedAxis = DEF_MOUNT_Y;
		m_iSelectedPos = 0;
	}

	if (TRUE == m_bGantryMoveSts)
	{
		m_bGantryMoveSts = FALSE;
		m_btnGantryEnable.SetValue(FALSE);
	}

	//KKY_____
//	EWorkingSide eWorkSide = m_plnkProcessData->GetCurrentWorkingSide(m_iGroupNo);
//	m_iSelectedTabNo = m_plnkProcessData->GetMounterDefaultTabNo(eWorkSide, m_plnkTabMounter->GetInstanceNo());
//	if (-1 == m_iSelectedTabNo)
//		m_iSelectedTabNo = 0;
	m_iSelectedTabNo = 0;
	//________

	/** TabMounter 에서 좌표 얻어오기 */
	m_plnkTabMounter->GetPosParameter(DEF_MOUNT_Y, m_rgdFixedPosData[0], m_rgdOffsetPosData[0], m_rgdModelPosData[0]);
	m_plnkTabMounter->GetPosParameter(DEF_MOUNT_Z, m_rgdFixedPosData[1], m_rgdOffsetPosData[1], m_rgdModelPosData[1]);
//@#ifdef DEF_GATE_SYSTEM
	m_plnkTabMounter->GetPosParameter(DEF_GANTRY, m_rgdFixedPosData[2], m_rgdOffsetPosData[2], m_rgdModelPosData[2]);
//@#endif
	/*KKY.Del
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		m_iJogSelect = DEF_JOG_F_TABMOUNTER1_Y + iTabMounterID;
	}
	else
	{
		if (DEF_FRONT_GROUP == m_iGroupNo)
			m_iJogSelect = DEF_JOG_F_TABMOUNTER1_X + iTabMounterID;
		else	// if (m_iGroupNo == DEF_REAR_GROUP)
			m_iJogSelect = DEF_JOG_R_TABMOUNTER1_X + iTabMounterID - DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[DEF_REAR_GROUP];
	}
	*/
	if (DEF_MOUNT_Z == m_iSelectedAxis)
	{
//		m_iJogSelect = DEF_JOG_F_TABMOUNTER1_Z + iTabMounterID;
		//120110.kms__________
		if (m_iGroupNo == DEF_FRONT_GROUP)
			m_iJogSelect = DEF_JOG_F_TABMOUNTER1_Z + iTabMounterID;
		else
			m_iJogSelect = DEF_JOG_R_TABMOUNTER1_Z + iTabMounterID;
		//____________________
	}
	else
	{
		if (m_iGroupNo == DEF_FRONT_GROUP)
			m_iJogSelect = DEF_JOG_F_TABMOUNTER1_X + iTabMounterID;
		else
			m_iJogSelect = DEF_JOG_R_TABMOUNTER1_X + iTabMounterID;
	}
	m_plnkC_MngOpPanel->SetJogUnit(m_iJogSelect);

	updatePosButton();	// Teaching Position Name Update

	updateCoordinate();	// Selected Position 에 대한 Coordinate Update
}

BOOL CTeachTabMounterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE:
		{
//			 TabMounter 모델좌표 Teaching
			CString szTextTemp;
			ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_1"), &szTextTemp);
			SetWindowText(szTextTemp);
			GetDlgItem(IDB_GET_ORIGIN_OFFSET)->ShowWindow(SW_HIDE);
			break;
		}
	case DEF_FIX_MODE:
		{
	//		 TabMounter 고정좌표 Teaching
			CString szTextTemp;
			ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_2"), &szTextTemp);
			SetWindowText(szTextTemp);
			break;
		}
	}

	if (m_iGroupNo == DEF_FRONT_GROUP)
		SetWindowText(_T("Tab Mounter Teaching [ Front Group]"));
	else
		SetWindowText(_T("Tab Mounter Teaching [ Rear Group]"));

	WCHAR szTemp[250];
	swprintf(szTemp, _T("%d"), m_iTabMounterID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
	
//@#ifdef DEF_SOURCE_SYSTEM
//@	swprintf(szTemp, _T("TabMounter Y \n%d"), m_iTabMounterID + 1);
//@#else
	swprintf(szTemp, _T("TabMounter X \n%d"), m_iTabMounterID + 1);
//@#endif
	m_ctrlBtnAxis[DEF_MOUNT_Y].SetWindowText(szTemp);

	swprintf(szTemp, _T("TabMounter Z \n%d"), m_iTabMounterID + 1);
	m_ctrlBtnAxis[DEF_MOUNT_Z].SetWindowText(szTemp);
	

//	if (DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[m_iGroupNo] == m_iTabMounterID)
	if (DEF_MAX_WORKER_PER_GROUP-1 == m_iTabMounterID)
		m_btnNextTabMounter.EnableWindow(FALSE);

//	if (DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[m_iGroupNo] == m_iTabMounterID)
	if (0 == m_iTabMounterID)
		m_btnPrevTabMounter.EnableWindow(FALSE);
	
	setWorkTabMounter(m_iTabMounterID);
	
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDB_GET_ORIGIN_OFFSET)->ShowWindow(SW_HIDE);
	}

//	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)

	/*/
#ifdef DEF_SOURCE_SYSTEM
	{
		m_ctrlBtnAxis[DEF_GANTRY].EnableWindow(FALSE);		
		m_btnGantryEnable.EnableWindow(FALSE);
		m_btnGantryEnable.ShowWindow(SW_HIDE);
	}
#endif
	/*/

	m_uiTimerID = SetTimer(1, 200, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_3"), &szTextTemp);
	//축 변경
	GetDlgItem(IDB_CHANGE_AXIS)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_4"), &szTextTemp);
	//목표 위치
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_5"), &szTextTemp);
	//고정 좌표
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_6"), &szTextTemp);
	//모델 좌표
	GetDlgItem(IDC_MODEL_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_7"), &szTextTemp);
	//Offset 좌표
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_8"), &szTextTemp);
	//현재 위치
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_9"), &szTextTemp);
	//차이값
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_10"), &szTextTemp);
	//TabMounter 번호 선택
	GetDlgItem(IDS_SELECT)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_11"), &szTextTemp);
	//목표 위치로\r\n이동
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_12"), &szTextTemp);
	//목표 위치를\r\n현재값으로
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_13"), &szTextTemp);
	//원점복귀\r\nOffset 얻기
	GetDlgItem(IDB_GET_ORIGIN_OFFSET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_14"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachTabMounterDlg_15"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
//	if (FALSE == m_plnkWorkSchedule[0]->GetUseWorkSide())
	{
		GetDlgItem(IDB_CM_VIEW_SCHEDULE)->EnableWindow(FALSE);		
	}

	// Doolin kwangilshin 2017. 08. 24.
	//
	Init_GridAxisStste();
	// End.

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CTeachTabMounterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachTabMounterDlg)
	ON_EVENT(CTeachTabMounterDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_CHANGE_AXIS, -600 /* Click */, OnClickChangeAxis, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_AXIS1, -600 /* Click */, OnAxis1, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_AXIS2, -600 /* Click */, OnAxis2, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_AXIS3, -600 /* Click */, OnAxis3, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS0, -600 /* Click */, OnClickPos0, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS1, -600 /* Click */, OnClickPos1, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS2, -600 /* Click */, OnClickPos2, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS3, -600 /* Click */, OnClickPos3, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS4, -600 /* Click */, OnClickPos4, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_VISION_CALIB, -600 /* Click */, OnClickVisionCalib, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_REGISTER_MARK, -600 /* Click */, OnClickRegisterMark, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_CM_MOVE, -600 /* Click */, OnClickCmMove, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS_SET, -600 /* Click */, OnClickPosSet, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_CM_UPDOWN, -600 /* Click */, OnClickCmUpdown, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_CM_ONOFF, -600 /* Click */, OnClickCmOnoff, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_SELECT_PREVIOS, -600 /* Click */, OnClickSelectPrevios, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_SELECT_NEXT, -600 /* Click */, OnClickSelectNext, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_GET_ORIGIN_OFFSET, -600 /* Click */, OnGetOriginOffset, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS5, -600 /* Click */, OnClickPos5, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS6, -600 /* Click */, OnClickPos6, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS7, -600 /* Click */, OnClickPos7, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_POS8, -600 /* Click */, OnClickPos8, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_GANTRY_ENABLE, -600 /* Click */, OnGantryEnable, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_CM_VIEW_SCHEDULE, -600 /* Click */, OnClickCmViewSchedule, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_CM_MOVE_ALL, -600 /* Click */, OnClickCmMoveAll, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_SELECT_SCHEDULE_PREV, -600 /* Click */, OnClickSelectSchedulePrev, VTS_NONE)
	ON_EVENT(CTeachTabMounterDlg, IDB_SELECT_SCHEDULE_NEXT, -600 /* Click */, OnClickSelectScheduleNext, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachTabMounterDlg::OnClickSave()
{
//	 Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,51000,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	if (DEF_FIX_MODE == m_iFixModel)
	{		
		m_plnkTabMounter->SaveFixedPosParameter(m_iSelectedAxis, m_rgdFixedPosData[m_iSelectedAxis]);
		
		if (DEF_MOUNT_Y == m_iSelectedAxis)
		{
			// Ready, Load 위치 강제 반영
//@#ifdef DEF_SOURCE_SYSTEM
//@			int iInstanceNo = m_plnkTabMounter->GetInstanceNo();
//@#else
			int iInstanceNo = m_plnkTabMounter->GetWorkInstance();
//@#endif
			int iStart = 0;	
			int iEnd = DEF_MAX_WORKER_PER_GROUP;
			double dValue = 0.0;
			int i = 0;
			/* ///
			//1번 Tool로 위치지정
			if (DEF_TABMOUNTER1 == iInstanceNo)
			{
				//Front Loading 위치
				for (int i = iStart; i < DEF_TABMOUNTER5; i++)
				{
#ifdef DEF_SOURCE_SYSTEM
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * i);
#else
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS];
#endif
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_LOAD_POS, dValue);
				}

#ifdef DEF_SOURCE_SYSTEM
				//1~4번 Tool Ready 위치는 Loading위치와 동일
				for (int i = iStart; i < DEF_TABMOUNTER5; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * i);
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_READY_POS, dValue);
				}
				//5~8번 RLoad위치 지정
				for (int i = DEF_TABMOUNTER5; i<iEnd; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * i);
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_R_LOAD_POS, dValue);
				}
#endif

				//1~4번 Tool TabRemove 위치
				for (int i = iStart; i < DEF_TABMOUNTER5; i++)
				{
					//dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_REMOVE_POS] - (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (DEF_TABMOUNTER4-i));
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * i);
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_REMOVE_POS, dValue);
				}

				//Gate에서는 Loading위치로 Escape위치 결정
#ifdef DEF_GATE_SYSTEM
				for (int i = iStart; i < DEF_TABMOUNTER5; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] - (DEF_TABMOUNTER_STANDARD_GAP * (DEF_TABMOUNTER4-i));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_ESCAPE_POS, dValue);
				}
#endif
			}

			//Gate에서는 4번으로 Ready위치 지정
#ifdef DEF_GATE_SYSTEM
			else if (DEF_TABMOUNTER4 == iInstanceNo)
			{
				//1~4번 Tool Ready 위치
				for (int i = iStart; i < DEF_MAX_TABMOUNTER; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_READY_POS] - (DEF_TABMOUNTER_STANDARD_GAP * (DEF_TABMOUNTER4-i));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_READY_POS, dValue);
				}
			}
#endif
			else if (DEF_TABMOUNTER8 == iInstanceNo)
			{
				//Rear Loading 위치
				//for (int i = iStart; i < iEnd; i++)
				for (int i = DEF_TABMOUNTER5; i < iEnd; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] - (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*(DEF_TABMOUNTER8-i));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_LOAD_POS, dValue);
				}
				
				//5~8번 Tool Ready 위치는 Loading위치와 동일
				for (int i = DEF_TABMOUNTER5; i < iEnd; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] - (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*(DEF_TABMOUNTER8-i));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_READY_POS, dValue);
				}

				//1~4번 RLoad위치 지정
				for (int i = iStart; i < DEF_TABMOUNTER5; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] - (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*(DEF_TABMOUNTER8-i));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_R_LOAD_POS, dValue);
				}

				//5~8번 Tool TabRemove 위치
				for (int i = DEF_TABMOUNTER5; i < iEnd; i++)
				{
					//dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_REMOVE_POS] + DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*(i-DEF_TABMOUNTER5);
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_REMOVE_POS] - (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*(DEF_TABMOUNTER8-i));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_REMOVE_POS, dValue);
				}
			}
			*/	///

			if (DEF_TABMOUNTER1 == iInstanceNo)
			{
//@#ifdef DEF_GATE_SYSTEM
				//Front Loading 위치
				for (int i = iStart; i <= DEF_MAX_TABMOUNTER-1; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS]+(DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * i);
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_LOAD_POS, dValue);
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_READY_POS, dValue);
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_ESCAPE_POS, dValue);
				}
				//1~4번 Tool TabRemove 위치
				for (int i = iStart; i <= DEF_MAX_TABMOUNTER-1; i++)
				{
					//dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * i);
					dValue = m_rgdFixedPosData[0][DEF_TABMOUNTER_REMOVE_POS];
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_REMOVE_POS, dValue);
				}
//@#else
				;
//@#endif
			}
//@			else if (DEF_TABMOUNTER4 == iInstanceNo)
			else if ((DEF_MAX_TABMOUNTER-1) == iInstanceNo)
			{
				/*/
#ifdef DEF_SOURCE_SYSTEM
				//Front Loading 위치
				for (int i = iStart; i < DEF_TABMOUNTER5; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] - (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (DEF_TABMOUNTER4-i));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_LOAD_POS, dValue);
				}
				//Rear R Loading 위치				
				for (int i = iStart; i < DEF_TABMOUNTER5; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] - (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (DEF_TABMOUNTER4-i));
					setTabMounterPosOffset(i+DEF_TABMOUNTER5, m_iFixModel, DEF_TABMOUNTER_R_LOAD_POS, dValue);
				}

				//Front Tool Ready 위치
				for (int i = iStart; i < DEF_MAX_TABMOUNTER; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] - (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (DEF_MOUNTER_WORK_PER_GROUP-1)) - (DEF_TABMOUNTER_STANDARD_GAP * (DEF_MOUNTER_WORK_PER_GROUP-i));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_READY_POS, dValue);
				}

				//Front Remove 위치
				for (int i = iStart; i < DEF_TABMOUNTER5; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_REMOVE_POS];
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_REMOVE_POS, dValue);
				}
#else
				/*/
				//1~4번 Tool Ready 위치
//@				for (int i = iStart; i < DEF_MAX_TABMOUNTER; i++)

//@#endif
			}
			else if (DEF_TABMOUNTER5 == iInstanceNo)		//@ ---> 해당사항 없음
			{
				//Rear Loading 위치
				for (int i = DEF_TABMOUNTER5; i < iEnd; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (i-DEF_TABMOUNTER5));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_LOAD_POS, dValue);
				}
				//Front R Loading 위치				
				for (int i = DEF_TABMOUNTER5; i < iEnd; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (i-DEF_TABMOUNTER5));
					setTabMounterPosOffset(i-DEF_TABMOUNTER5, m_iFixModel, DEF_TABMOUNTER_R_LOAD_POS, dValue);
				}

				//Rear Tool Ready 위치
				for (int i = DEF_TABMOUNTER5; i < iEnd; i++)
				{					
//@					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (DEF_MOUNTER_WORK_PER_GROUP-1)) + (DEF_TABMOUNTER_STANDARD_GAP * (i-DEF_MOUNTER_WORK_PER_GROUP+1));
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_LOAD_POS] + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (DEF_MOUNTER_WORK_PER_GROUP-1)) + (DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * (i-DEF_MOUNTER_WORK_PER_GROUP+1));
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_READY_POS, dValue);
				}

				//Rear Remove 위치
				for (int i = DEF_TABMOUNTER5; i < iEnd; i++)
				{
					dValue = m_rgdFixedPosData[DEF_MOUNT_Y][DEF_TABMOUNTER_REMOVE_POS];
					setTabMounterPosOffset(i, m_iFixModel, DEF_TABMOUNTER_REMOVE_POS, dValue);
				}				

			}

			m_plnkTabMounter->GetPosParameter(DEF_MOUNT_Y, m_rgdFixedPosData[0], m_rgdOffsetPosData[0], m_rgdModelPosData[0]);
		}
		else if (DEF_MOUNT_Z == m_iSelectedAxis)
		{
//@#ifdef DEF_SOURCE_SYSTEM
//@			int iInstanceNo = m_plnkTabMounter->GetInstanceNo();
//@#else
			int iInstanceNo = m_plnkTabMounter->GetWorkInstance();
//@#endif
			int iStart = 0;
			int iEnd = DEF_MAX_WORKER_PER_GROUP;
			double dValue = 0.0;
			int i = 0;
			double rgdFixedPosData[DEF_TABMOUNTER_TEACH_MAX_POS];
			double rgdOffsetPosData[DEF_TABMOUNTER_TEACH_MAX_POS];
			double rgdModelPosData[DEF_TABMOUNTER_TEACH_MAX_POS];
			MTabMounter* plnkTabMounter = NULL;
			if (DEF_TABMOUNTER1 == iInstanceNo)
			{
				for (int i = iStart; i <= DEF_MAX_TABMOUNTER-1; i++)
				{
					if (i == m_iTabMounterID) continue;
					if (m_iGroupNo == DEF_FRONT_GROUP)
						plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(i);
					else
						plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(i);
					plnkTabMounter->GetPosParameter(DEF_MOUNT_Z, rgdFixedPosData, rgdOffsetPosData, rgdModelPosData);
					for (int j=0; j<DEF_TABMOUNTER_Z_MAX_POS; j++)
					{
						dValue = m_rgdFixedPosData[DEF_MOUNT_Z][j];
						rgdFixedPosData[j] = dValue;
					}
					plnkTabMounter->SaveFixedPosParameter(DEF_MOUNT_Z, rgdFixedPosData);
				}
			}
			else if (DEF_TABMOUNTER8 == iInstanceNo)	//@ ---> 해당 사항 없음
			{
				for (int i = DEF_TABMOUNTER5; i < iEnd; i++)
				{
					if (i == m_iTabMounterID) continue;
					plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(i);
					plnkTabMounter->GetPosParameter(DEF_MOUNT_Z, rgdFixedPosData, rgdOffsetPosData, rgdModelPosData);
					for (int j=0; j<DEF_TABMOUNTER_Z_MAX_POS; j++)
					{
						dValue = m_rgdFixedPosData[DEF_MOUNT_Z][j];
						rgdFixedPosData[j] = dValue;
					}
					plnkTabMounter->SaveFixedPosParameter(DEF_MOUNT_Z, rgdFixedPosData);
				}
			}
			m_plnkTabMounter->GetPosParameter(DEF_MOUNT_Z, m_rgdFixedPosData[1], m_rgdOffsetPosData[1], m_rgdModelPosData[1]);
		}
		else	//else if (DEF_GANTRY == m_iSelectedAxis)
		{
			//TabCarrier 1번 Unloading위치에 Gantry를 위치시킴(Gantry Loading1 위치)
			/*
			if (m_iGroupNo == DEF_FRONT_GROUP)
			{
				m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD2_POS] = m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD1_POS] + DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*1.0;
				m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD3_POS] = m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD1_POS] + DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*2.0;
				m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD4_POS] = m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD1_POS] + DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*3.0;
			}
			else
			{
				m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD2_POS] = m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD1_POS] - DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*1.0;
				m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD3_POS] = m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD1_POS] - DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*2.0;
				m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD4_POS] = m_rgdFixedPosData[m_iSelectedAxis][DEF_TABMOUNTER_GANTRY_LOAD1_POS] - DEF_TABCARRIER_PICKUP_UNIT_INTERVAL*3.0;
			}
			*/
			;
		}
	}
	else
	{
		//SJ_PJC 20170412 최웅일과장 요청
		if (DEF_MOUNT_Y == m_iSelectedAxis)
		{
			m_rgdOffsetPosData[m_iSelectedAxis][DEF_TABMOUNTER_READY_POS] = m_rgdOffsetPosData[m_iSelectedAxis][DEF_TABMOUNTER_LOAD_POS];
		}
		//______________________________
		m_plnkTabMounter->SaveOffsetPosParameter(m_iSelectedAxis, m_rgdOffsetPosData[m_iSelectedAxis]);
	}

	/*
	if (m_iSelectedAxis == DEF_MOUNT_Z)
		return;

	if (DEF_TABMOUNTER_READY_POS == m_iSelectedPos || DEF_TABMOUNTER_LOAD_POS == m_iSelectedPos
		|| DEF_TABMOUNTER_R_LOAD_POS == m_iSelectedPos || DEF_TABMOUNTER_REMOVE_POS == m_iSelectedPos)
		return;

//	 같은 Group 내의 TabMounter 위치 Data 에 같은 Teaching 값을 반영하시겠습니까? (Ready, Load 제외)
	if (MyMessageBox(MY_MESSAGE_BOX,51001,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iStart = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[m_iGroupNo];
	int iEnd = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[m_iGroupNo] + 1;
	double dValue = 0.0;
	/// 임시
	for (int i = iStart; i < iEnd; i++)
	{
		//for (int j = DEF_TABMOUNTER_ALIGN1_POS; j < DEF_TABMOUNTER_MAX_POS; j++)
		for (int j = DEF_TABMOUNTER_INSPECTION_POS; j <= DEF_TABMOUNTER_MOUNT_POS; j++) 
		{
			dValue = (DEF_FIX_MODE == m_iFixModel) ? m_rgdFixedPosData[DEF_MOUNT_Y][j] : m_rgdOffsetPosData[DEF_MOUNT_Y][j];
			setTabMounterPosOffset(i, m_iFixModel, j, dValue);
		}
	}
	*/
}

void CTeachTabMounterDlg::setTabMounterPosOffset(const int iTargetTabMounterID, const int iFixModel, const int iPosID, const double dValue)
{
	if (iTargetTabMounterID == m_iTabMounterID && iPosID == DEF_TABMOUNTER_LOAD_POS)
		return;
	
	double rgdFixedPosData[DEF_TABMOUNTER_TEACH_MAX_POS];
	double rgdOffsetPosData[DEF_TABMOUNTER_TEACH_MAX_POS];
	double rgdModelPosData[DEF_TABMOUNTER_TEACH_MAX_POS];

	MTabMounter* plnkTabMounter;
	if (m_iGroupNo == DEF_FRONT_GROUP)
		plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(iTargetTabMounterID);
	else
		plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(iTargetTabMounterID);
	
	plnkTabMounter->GetPosParameter(DEF_MOUNT_Y, rgdFixedPosData, rgdOffsetPosData, rgdModelPosData);
	
	if (DEF_FIX_MODE == iFixModel)
	{
		rgdFixedPosData[iPosID]  = dValue;
		plnkTabMounter->SaveFixedPosParameter(DEF_MOUNT_Y, rgdFixedPosData);
	}
	else if (DEF_MODEL_MODE == iFixModel)
	{
		rgdOffsetPosData[iPosID] = dValue;
		plnkTabMounter->SaveOffsetPosParameter(DEF_MOUNT_Y, rgdOffsetPosData);
	}
}

void CTeachTabMounterDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

#ifndef SIMULATION
	m_plnkC_MngOpPanel->SetJogUnit(-1);
#endif /* SIMULATION */

//	m_plnkTabMounter->SetGantryMotionEnable(FALSE, 0);
	m_plnkTabMounter->SetGantryMotionEnable(FALSE);

	CDialog::OnCancel();
}

void CTeachTabMounterDlg::OnClickChangeAxis() 
{
//	m_iSelectedAxis = (++m_iSelectedAxis) % DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE;
	m_iSelectedAxis = (++m_iSelectedAxis) % DEF_TABMOUNTER_AXIS_TYPE;

	if (m_iSelectedPos != 0)
	{
		m_btnPos[m_iSelectedPos].SetValue(FALSE);
	}
	m_iSelectedPos = 0;

	updatePosButton();

	if (DEF_MOUNT_Z == m_iSelectedAxis)
	{
		//120110.kms__________
//		m_iJogSelect = DEF_JOG_F_TABMOUNTER1_Z + m_iTabMounterID;
//@#ifdef DEF_SOURCE_SYSTEM
//@		m_iJogSelect = DEF_JOG_F_TABMOUNTER1_Z + m_iTabMounterID;
//@#else
		if (m_iGroupNo == DEF_FRONT_GROUP)
			m_iJogSelect = DEF_JOG_F_TABMOUNTER1_Z + m_iTabMounterID;
		else
			m_iJogSelect = DEF_JOG_R_TABMOUNTER1_Z + m_iTabMounterID;
//@#endif
		//____________________

//		m_plnkTabMounter->SetGantryMotionEnable(FALSE, 0);
		m_plnkTabMounter->SetGantryMotionEnable(FALSE);
		if (TRUE == m_bGantryMoveSts)
		{
			m_bGantryMoveSts = FALSE;
			m_btnGantryEnable.SetValue(FALSE);
		}
//		m_iGantryLoadPosID = 0;
	}
	else if (DEF_MOUNT_Y == m_iSelectedAxis)
	{
		if (m_iGroupNo == DEF_FRONT_GROUP)
			m_iJogSelect = DEF_JOG_F_TABMOUNTER1_X + m_iTabMounterID;
		else
			m_iJogSelect = DEF_JOG_R_TABMOUNTER1_X + m_iTabMounterID;
//		m_plnkTabMounter->SetGantryMotionEnable(FALSE, 0);
		m_plnkTabMounter->SetGantryMotionEnable(FALSE);
		if (TRUE == m_bGantryMoveSts)
		{
			m_bGantryMoveSts = FALSE;
			m_btnGantryEnable.SetValue(FALSE);
		}
//		m_iGantryLoadPosID = 0;
	}
	else	//(DEF_GANTRY == m_iSelectedAxis)
	{
		if (DEF_FRONT_GROUP == m_iGroupNo)
		{
			m_iJogSelect = DEF_JOG_GANTRY_Y;
		}
		else
		{
			m_iJogSelect = DEF_JOG_R_GANTRY_Y;
		}
//		m_iGantryLoadPosID = 0;
//		m_plnkTabMounter->SetGantryMotionEnable(TRUE, m_iGantryLoadPosID);
		m_plnkTabMounter->SetGantryMotionEnable(TRUE);
	}

	m_plnkC_MngOpPanel->SetJogUnit(m_iJogSelect);
}

BOOL CTeachTabMounterDlg::isEnabledToModifyPosValue()
{
	CString strMsg = "";
	if (DEF_MODEL_MODE == m_iFixModel)
	{
		return TRUE;
	}

	if (DEF_MOUNT_Z == m_iSelectedAxis)
		return TRUE;

	int iInstanceNo = m_plnkTabMounter->GetInstanceNo();
	int iWorkInstance = m_plnkTabMounter->GetWorkInstance();

	switch (m_iSelectedPos)
	{
	case DEF_TABMOUNTER_READY_POS: 
//@#ifdef DEF_SOURCE_SYSTEM
//@		//Ready위치는 1번 Tool Loading위치로 결정됨
//@		MyMessageBox(MY_MESSAGE_BOX,51012,_T("Ready Pos change prohibition"), M_ICONINFORMATION);
//@#else
		MyMessageBox(_T("Ready위치는 Load위치와 동일합니다."), _T("Information"));
//@#endif
		break;

	case DEF_TABMOUNTER_LOAD_POS:
//@#ifdef DEF_SOURCE_SYSTEM
//@		if (DEF_TABMOUNTER4 == iWorkInstance)
//@			return TRUE;
//@//		 현재 TabMounter 에서는 Load 위치 변경 작업을 할 수 없습니다.
//@		MyMessageBox(MY_MESSAGE_BOX,51003,_T("Load Pos change prohibition"), M_ICONINFORMATION);
//@#else
		if (DEF_TABMOUNTER1 == iWorkInstance)
			return TRUE;
//		 현재 TabMounter 에서는 Load 위치 변경 작업을 할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,51004,_T("Load Pos change prohibition"), M_ICONINFORMATION);
//@#endif
		break;

//@#ifdef DEF_SOURCE_SYSTEM
//@	case DEF_TABMOUNTER_R_LOAD_POS:
//@		MyMessageBox(MY_MESSAGE_BOX, 51017,_T("RLoad Pos change prohibition"), M_ICONINFORMATION);
//@		break;		
//@#else
	case DEF_TABMOUNTER_ESCAPE_POS:
		MyMessageBox(_T("Escape Position is equals Load Position."), _T("Information"));
		break;
//@#endif

	case DEF_TABMOUNTER_REMOVE_POS:
		if (DEF_TABMOUNTER1 == iWorkInstance)
			return TRUE;
		MyMessageBox(MY_MESSAGE_BOX,51013,_T("Ready Pos change prohibition"), M_ICONINFORMATION);
		break;

//	case DEF_TABMOUNTER_REMOVE2_POS:
//		return TRUE;
	default:
		// 고정좌표 Teaching 불가 위치인 경우
		// Source : Align1, Align2, Inspection, Mount, Remove1
		// Gate : Align1, Align2, Inspection, Mount

//		 고정좌표 변경 불가.
		MyMessageBox(MY_MESSAGE_BOX,51005,_T("Fixed coordinate change prohibition"), M_ICONINFORMATION);
		break;
	}

	return FALSE;
}

void CTeachTabMounterDlg::OnAxis1() 
{
	if (FALSE == isEnabledToModifyPosValue())
		return;

	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_lblCoord[0][0].GetWindowText(cBeforeVal, 30);

	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkTabMounter->GetMotionComponent()))
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

	//SJ_YYK 150109 Add...
	double dPrealignOffset = 0.0;

	if(m_iSelectedAxis == DEF_MOUNT_Y)
	{
		if(m_iSelectedPos == DEF_TABMOUNTER_LOAD_POS)  
			dPrealignOffset = MOlbSystemPre.GetTrsAutoManagerComponent()->ReadTabCarrierAlignResult(m_iGroupNo,m_iTabMounterID);
	}

	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		m_rgdOffsetPosData[0][m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[0][m_iSelectedPos]  - m_rgdFixedPosData[0][m_iSelectedPos] - dPrealignOffset;
	
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[0][m_iSelectedPos]);
		m_lblCoord[0][3].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		m_rgdFixedPosData[0][m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[0][m_iSelectedPos] - m_rgdOffsetPosData[0][m_iSelectedPos] - dPrealignOffset;

		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[0][m_iSelectedPos]);
		m_lblCoord[0][1].SetCaption(szTemp);
		break;		
	}
}

void CTeachTabMounterDlg::OnAxis2() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_lblCoord[1][0].GetWindowText(cBeforeVal, 30);

	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkTabMounter->GetMotionComponentZ()))
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

void CTeachTabMounterDlg::OnAxis3()
{
	// TODO: Add your control notification handler code here
	/*
	if ((m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD2_POS
		|| m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD3_POS
		|| m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD4_POS)
		&& m_iFixModel == DEF_FIX_MODE)
	{
		MyMessageBox(MY_MESSAGE_BOX, 51014, _T("Fixed coordinate change prohibition"), M_ICONINFORMATION);
		return;
	}
	*/

	/*/
	if (m_iFixModel == DEF_FIX_MODE)
	{
		if (m_iSelectedPos == DEF_TABMOUNTER_GANTRY_INSPECTION_POS
			|| m_iSelectedPos == DEF_TABMOUNTER_GANTRY_MOUNT_POS)
		{
			MyMessageBox(_T("선택한 Gantry 고정좌표는 입력할수 없습니다."), _T("Confirm"));
			return;
		}
	}
	/*/

	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_lblCoord[2][0].GetWindowText(cBeforeVal, 30);

	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkTabMounter->GetMotionComponentGantry()))
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

void CTeachTabMounterDlg::OnClickPos0() 
{
	// TODO: Add your control notification handler code here
	selectPosition(0);	
}

void CTeachTabMounterDlg::OnClickPos1() 
{
	// TODO: Add your control notification handler code here
	selectPosition(1);	
}

void CTeachTabMounterDlg::OnClickPos2() 
{
	// TODO: Add your control notification handler code here
	selectPosition(2);	
}

void CTeachTabMounterDlg::OnClickPos3() 
{
	// TODO: Add your control notification handler code here
	selectPosition(3);	
}

void CTeachTabMounterDlg::OnClickPos4() 
{
	// TODO: Add your control notification handler code here
	selectPosition(4);	
}

void CTeachTabMounterDlg::OnClickPos5() 
{
	// TODO: Add your control notification handler code here
	selectPosition(5);	
}

void CTeachTabMounterDlg::OnClickPos6() 
{
	// TODO: Add your control notification handler code here
	selectPosition(6);	
}

void CTeachTabMounterDlg::OnClickPos7() 
{
	// TODO: Add your control notification handler code here
	selectPosition(7);
}

void CTeachTabMounterDlg::OnClickPos8() 
{
	// TODO: Add your control notification handler code here
	selectPosition(8);	
}

int CTeachTabMounterDlg::getWorkCameraNo()
{
	/*
	if (TRUE == m_plnkTabMounter->IsInPos(DEF_TABMOUNTER_ALIGN1_POS))
	{
		return (DEF_FRONT_GROUP == m_iGroupNo) ? DEF_F_TABALIGNER_CAMERA_1 : DEF_R_TABALIGNER_CAMERA_1;
	}
	else if (TRUE == m_plnkTabMounter->IsInPos(DEF_TABMOUNTER_ALIGN2_POS))
	{
		return (DEF_FRONT_GROUP == m_iGroupNo) ? DEF_F_TABALIGNER_CAMERA_2 : DEF_R_TABALIGNER_CAMERA_2;
	}
	*/
	// jdy2
	double dTolerance = 1.5;
	double dTabMounterCurrentPos = m_plnkTabMounter->GetCurrentPos();
	MCameraCarrier* plnkCamCarrier = NULL;
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (m_iGroupNo == DEF_FRONT_GROUP)
			plnkCamCarrier = MOlbSystemPre.GetCameraCarrierComponent(i);
		else
			plnkCamCarrier = MOlbSystemPre.GetRCameraCarrierComponent(i);
		
		if (DEF_NONE_INSPECTION_CAMERA == plnkCamCarrier->GetInspectCamCarrierNo())
			continue;

		if (m_iGroupNo != plnkCamCarrier->GetCameraCarrierGroupNo())
			continue;
		
		if (TRUE == plnkCamCarrier->IsInPos(dTabMounterCurrentPos, &dTolerance))
			//return plnkCamCarrier->GetOwnedCameraNo();
			return plnkCamCarrier->GetOwnedCameraNo1();
	}

//	return (DEF_FRONT_GROUP == m_iGroupNo) ? DEF_F_INSPECTION_CAMERA_1 : DEF_R_INSPECTION_CAMERA_1;
//@#ifdef DEF_SOURCE_SYSTEM
//@	return DEF_INSPECTION_CAMERA_1;
//@#else
	return (DEF_FRONT_GROUP == m_iGroupNo) ? DEF_INSPECTION_CAMERA_1 : DEF_R_INSPECTION_CAMERA_1;
//@#endif
}

void CTeachTabMounterDlg::OnClickVisionCalib() 
{
	int iCamNo = getWorkCameraNo();
	CVisionCalibDlg dlg(iCamNo, DEF_CALIB_GROUP_MOUNTER, m_iTabMounterID);
	dlg.DoModal();

	m_plnkTabMounter->GetPosParameter(DEF_MOUNT_Y, m_rgdFixedPosData[0], m_rgdOffsetPosData[0], m_rgdModelPosData[0]);
	updateDisplay();
	selectPosition(m_iSelectedPos);
}

void CTeachTabMounterDlg::OnClickRegisterMark() 
{
	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
	int iCamNo = getWorkCameraNo();
	
	// Operation Type = DEF_PM_MARK / DEF_BLOB_MARK / DEF_CALIPER_MARK
	CVisionMonitorDlg dlg(plnkVision, DEF_PM_MARK, iCamNo);
	dlg.DoModal();
}

void CTeachTabMounterDlg::OnClickCmMove() 
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
//	 목표위치로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,51006,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("TabMounter is Moved to Target Position..Wait a moment..."));
	ProgWnd.PeekAndPump();

	// 이동 중 Jog를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
	int iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

	int iResult = ERR_TABMOUNTER_TEACH_DLG_SUCCESS;

	BOOL bOldGantryEnableStatus = m_plnkTabMounter->IsGantryMotionEnable();
	
	if (DEF_MOUNT_Y == m_iSelectedAxis)
		iResult = m_plnkTabMounter->SafeMovePos(m_iWorkScheduleNo, m_iSelectedPos, TRUE, m_iSelectedTabNo, FALSE);
	else if (DEF_MOUNT_Z == m_iSelectedAxis)
		iResult = m_plnkTabMounter->SafeMoveZPos(m_iSelectedPos, TRUE, FALSE);
	else		
	{
		/*
		if (m_iSelectedPos >= DEF_TABMOUNTER_GANTRY_INSPECTION_POS)
			iResult = m_plnkTabMounter->SafeMoveGantryPos(m_iSelectedPos, m_iGantryLoadPosID, TRUE, FALSE);
		else
			iResult = m_plnkTabMounter->SafeMoveGantryPos(DEF_TABMOUNTER_GANTRY_LOAD1_POS, m_iGantryLoadPosID, TRUE, FALSE);
		*/
		iResult = m_plnkTabMounter->SafeMoveGantryPos(m_iSelectedPos, TRUE, TRUE);
	}

	if (TRUE == bOldGantryEnableStatus)
//		m_plnkTabMounter->SetGantryMotionEnable(TRUE, m_iGantryLoadPosID);
		m_plnkTabMounter->SetGantryMotionEnable(TRUE);

	if (iResult != ERR_TABMOUNTER_TEACH_DLG_SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, "TabMounter Teaching Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	
	m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);	// Jog 사용 재개
	ProgWnd.StepIt();
}

void CTeachTabMounterDlg::OnClickPosSet() 
{
	if (DEF_MOUNT_Y == m_iSelectedAxis)
	{
		if (FALSE == isEnabledToModifyPosValue())
			return;
	}
	else if (DEF_GANTRY == m_iSelectedAxis)
	{
		/*/
		if ((m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD2_POS
			|| m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD3_POS
			|| m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD4_POS)
			&& m_iFixModel == DEF_FIX_MODE)
		{
			MyMessageBox(MY_MESSAGE_BOX, 51014, _T("Fixed coordinate change prohibition"), M_ICONINFORMATION);
			return;
		}
		/*/

		/*/
		if (m_iSelectedPos == DEF_TABMOUNTER_GANTRY_INSPECTION_POS
			|| m_iSelectedPos == DEF_TABMOUNTER_GANTRY_MOUNT_POS)
		{
			MyMessageBox(_T("선택한 Gantry 고정좌표는 입력할수 없습니다."), _T("Confirm"));
			return;
		}
		/*/
	}

	WCHAR szTemp[250];	
	
//	 목표 위치를 현재 위치로 변경하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,51007,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Set Target Position..."));
	ProgWnd.SetRange(0, 4);
	ProgWnd.SetText(_T("TabMounter Target Position is changing to Current Position..Wait a moment..."));
	ProgWnd.PeekAndPump();

	double dCurrent = 0.0;
	if (DEF_MOUNT_Y == m_iSelectedAxis)
		dCurrent = m_plnkTabMounter->GetCurrentPos();
	else if (DEF_MOUNT_Z == m_iSelectedAxis)
		dCurrent = m_plnkTabMounter->GetCurrentPosZ();
	else if (DEF_GANTRY == m_iSelectedAxis)
		dCurrent = m_plnkTabMounter->GetCurrentGantryPos();

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

void CTeachTabMounterDlg::OnClickCmUpdown() 
{
	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Vacuum..."));
	ProgWnd.SetRange(0, 1);

	int iResult = ERR_TABMOUNTER_SUCCESS;
	if (m_plnkTabMounter->IsUpCyl())
	{
		ProgWnd.SetText(_T("TabMounter Down Ready..Wait a moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkTabMounter->DownCyl();
	}
	else
	{
		ProgWnd.SetText(_T("TabMounter Up..Wait a moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkTabMounter->UpCyl();
	}

	if (iResult != ERR_TABMOUNTER_SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, "TabMounter Teaching Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CTeachTabMounterDlg::OnClickCmOnoff() 
{
	CMyProgressWnd ProgWnd(NULL, _T("TabMounter Vacuum..."));
	ProgWnd.SetRange(0, 1);

	int iResult = ERR_TABMOUNTER_SUCCESS;
	if (m_plnkTabMounter->IsAbsorbTabIC())
	{
		ProgWnd.SetText(_T("TabMounter Vacuum Release..Wait a moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkTabMounter->IsReleaseTabIC();
	}
	else
	{
		ProgWnd.SetText(_T("TabMounter Vacuum Absorb..Wait a moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkTabMounter->AbsorbTabIC();
	}

	if (iResult != ERR_TABMOUNTER_SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, "TabMounter Teaching Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CTeachTabMounterDlg::updateDisplay()
{
//	int iNotUseAxis = (m_iSelectedAxis + 1) % DEF_TABMOUNTER_AXIS_TYPE;//DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE;
	int iNotUseAxis[2];
	iNotUseAxis[0] = (m_iSelectedAxis + 1) % DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE;
	iNotUseAxis[1] = (m_iSelectedAxis + 2) % DEF_TABMOUNTER_TEACH_MAX_AXIS_TITLE;
	for (int i = 0; i < DEF_TABMOUNTER_TEACH_MAX_COORDINATE_TYPE; i++)
	{
		m_lblCoord[iNotUseAxis[0]][i].EnableWindow(FALSE);
		m_lblCoord[iNotUseAxis[1]][i].EnableWindow(FALSE);
		m_lblCoord[m_iSelectedAxis][i].EnableWindow(TRUE);
	}

	WCHAR szTemp[250];
	double dFixedPosData = m_rgdFixedPosData[m_iSelectedAxis][m_iSelectedPos];
	double dModelPosData = m_rgdModelPosData[m_iSelectedAxis][m_iSelectedPos];
	double dOffsetPosData = m_rgdOffsetPosData[m_iSelectedAxis][m_iSelectedPos];

	double dTargetPos = dFixedPosData + dModelPosData + dOffsetPosData;

	//SJ_YYK 150109 Add...
	double dPrealignOffset = 0.0;

	if(m_iSelectedAxis == DEF_MOUNT_Y)
	{

	}

	dTargetPos += dPrealignOffset;

	swprintf(szTemp, _T("%.3f"), dPrealignOffset);
	m_lblCoord[m_iSelectedAxis][4].SetCaption(szTemp);
	//___________________________*/

	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_lblCoord[m_iSelectedAxis][0].SetCaption(szTemp);	

	double dPosCurr = 0.0;
	if (DEF_MOUNT_Y == m_iSelectedAxis)
		dPosCurr = m_plnkTabMounter->GetCurrentPos();
	else if (DEF_MOUNT_Z == m_iSelectedAxis)
		dPosCurr = m_plnkTabMounter->GetCurrentPosZ();
	else
		dPosCurr = m_plnkTabMounter->GetCurrentGantryPos();

	// Reset Switch 감지 시 현재 위치를 0으로 표시되게 하여 
	// Teaching시 상대거리 측정을 쉽게 한다.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
		m_rgdDisplayOffset[m_iSelectedAxis] = dPosCurr;

	dPosCurr -= m_rgdDisplayOffset[m_iSelectedAxis];

	/** 현재 위치 Display */
	swprintf(szTemp, _T("%.3f"), dPosCurr);
	m_lblCoord[m_iSelectedAxis][5].SetCaption(szTemp);
	
	/** 편차 계산하고, Display */
	swprintf(szTemp, _T("%.3f"), dPosCurr - dTargetPos);
	m_lblCoord[m_iSelectedAxis][6].SetCaption(szTemp);	
}

void CTeachTabMounterDlg::updateCoordinate()
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

	setEnableAllMoveBtn();//110225.Add
}

void CTeachTabMounterDlg::selectPosition(int iPosIndex, BOOL bAskTabNoSkip)
{
	if (DEF_MOUNT_Y == m_iSelectedAxis)
	{
		if (iPosIndex == DEF_TABMOUNTER_INSPECTION_POS
			|| iPosIndex == DEF_TABMOUNTER_MOUNT_POS)
		{
			if (FALSE == m_plnkWorkSchedule[m_iWorkScheduleNo]->GetUseWorkSide())
			{
				MyMessageBox(_T("This work side is not use.\n사용하지 않는 작업변 입니다."), _T("Confirm"));
				m_btnPos[m_iSelectedPos].SetValue(TRUE);
				return;
			}

			if (FALSE == bAskTabNoSkip)
			{
				if (ERR_TABMOUNTER_TEACH_DLG_CANCEAL == getWorkTabNo())
				{
					m_btnPos[iPosIndex].SetValue(FALSE);
					m_btnPos[m_iSelectedPos].SetValue(TRUE);
					return;
				}
			}
			
			EWorkingSide eWorkSide = m_plnkProcessData->GetCurrentWorkingSide(m_iGroupNo);
			m_rgdModelPosData[m_iSelectedAxis][iPosIndex] = m_plnkWorkSchedule[m_iWorkScheduleNo]->GetTabPos(m_iSelectedTabNo);
		}
		else
		{
			m_rgdModelPosData[m_iSelectedAxis][iPosIndex] = 0.0;
		}
	}

	m_btnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_btnPos[iPosIndex].SetValue(TRUE);
	/*
	if (DEF_GANTRY == m_iSelectedAxis)
	{
		if (m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD1_POS)
			m_iGantryLoadPosID = 0;
		else if (m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD2_POS)
			m_iGantryLoadPosID = 1;
		else if (m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD3_POS)
			m_iGantryLoadPosID = 2;
		else if (m_iSelectedPos == DEF_TABMOUNTER_GANTRY_LOAD4_POS)
			m_iGantryLoadPosID = 3;
		else
			m_iGantryLoadPosID = 0;
	}
	*/
	updateCoordinate();
}

// Doolin kwangilshin 2017. 08. 24.
//
void CTeachTabMounterDlg::Init_GridAxisStste()
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


BOOL CTeachTabMounterDlg::Check_AxisStste(IAxis *pclsAxis)
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


void CTeachTabMounterDlg::SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome)
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


void CTeachTabMounterDlg::SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachTabMounterDlg::SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachTabMounterDlg::SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable)
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


void CTeachTabMounterDlg::updateAxisSnsSts()
{
	IAxis *pclsAxis[eAxisEnd];
	
	pclsAxis[eAxisTabMount]		= m_plnkTabMounter->GetMotionComponent();
	pclsAxis[eAxisTabMountZ]	= m_plnkTabMounter->GetMotionComponentZ();
	pclsAxis[eAxisGantry]		= m_plnkTabMounter->GetMotionComponentGantry();

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


void CTeachTabMounterDlg::OnTimer(UINT nIDEvent) 
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

void CTeachTabMounterDlg::updateButton()
{
	if (m_bCylSts != m_plnkTabMounter->IsUpCyl())
	{
		m_bCylSts = m_plnkTabMounter->IsUpCyl();
		m_btnCmd[0].SetValue(m_bCylSts);
		if (TRUE == m_plnkTabMounter->IsUpCyl())
			m_btnCmd[0].SetCaption(_T("Up"));
		else if (TRUE == m_plnkTabMounter->IsDownCyl())
			m_btnCmd[0].SetCaption(_T("Down"));
	}

	if (m_bVacSts != m_plnkTabMounter->IsAbsorbTabIC())
	{
		m_bVacSts = m_plnkTabMounter->IsAbsorbTabIC();
		m_btnCmd[1].SetValue(m_bVacSts);
		if (TRUE == m_bVacSts)
			m_btnCmd[1].SetCaption(_T("Vac On"));
		else
			m_btnCmd[1].SetCaption(_T("Vac Off"));
	}
}

void CTeachTabMounterDlg::OnClickSelectPrevios() 
{
//	if (m_iTabMounterID - 1 < DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[m_iGroupNo])
	if (m_iTabMounterID - 1 < 0)
		return;

	// 확인 필요 : Timer 와의 충돌 피하기 위한 처리 필요한가?
	m_iTabMounterOldID = m_iTabMounterID;

	m_iTabMounterID--;

	m_btnNextTabMounter.EnableWindow(TRUE);

//	if (m_iTabMounterID - 1 < DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[m_iGroupNo])
	if (m_iTabMounterID -1 < 0)
		m_btnPrevTabMounter.EnableWindow(FALSE);

	WCHAR szTemp[250];
	CMyProgressWnd ProgWnd(NULL, _T("Change Worker..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Change TabMounter..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	ProgWnd.StepIt();
	
	swprintf(szTemp, _T("%d"), m_iTabMounterID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
//@#ifdef DEF_SOURCE_SYSTEM
//@	swprintf(szTemp, _T("TabMounter Y \n%d"), m_iTabMounterID + 1);
//@#else
	swprintf(szTemp, _T("TabMounter X \n%d"), m_iTabMounterID + 1);
//@#endif
	m_ctrlBtnAxis[DEF_MOUNT_Y].SetWindowText(szTemp);

	swprintf(szTemp, _T("TabMounter Z \n%d"), m_iTabMounterID + 1);
	m_ctrlBtnAxis[DEF_MOUNT_Z].SetWindowText(szTemp);
	
	setWorkTabMounter(m_iTabMounterID);
	
	ProgWnd.StepIt();
}

void CTeachTabMounterDlg::OnClickSelectNext() 
{
//	if (m_iTabMounterID + 1 > DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[m_iGroupNo])
	if (m_iTabMounterID + 1 > DEF_MAX_TABMOUNTER - 1)
		return;

	// 확인 필요 : Timer 와의 충돌 피하기 위한 처리 필요한가?
	m_iTabMounterOldID = m_iTabMounterID;

	m_iTabMounterID++;

	m_btnPrevTabMounter.EnableWindow(TRUE);

//	if (m_iTabMounterID + 1 > DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[m_iGroupNo])
	if (m_iTabMounterID + 1 > DEF_MAX_TABMOUNTER - 1)
		m_btnNextTabMounter.EnableWindow(FALSE);

	CMyProgressWnd ProgWnd(NULL, _T("Change Worker..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Change TabMounter..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	ProgWnd.StepIt();
	
	WCHAR szTemp[50];
	swprintf(szTemp, _T("%d"), m_iTabMounterID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
//@#ifdef DEF_SOURCE_SYSTEM
//@	swprintf(szTemp, _T("TabMounter Y \n%d"), m_iTabMounterID + 1);
//@#else
	swprintf(szTemp, _T("TabMounter X \n%d"), m_iTabMounterID + 1);
//@#endif
	m_ctrlBtnAxis[DEF_MOUNT_Y].SetWindowText(szTemp);

	swprintf(szTemp, _T("TabMounter Z \n%d"), m_iTabMounterID + 1);
	m_ctrlBtnAxis[DEF_MOUNT_Z].SetWindowText(szTemp);
	
	setWorkTabMounter(m_iTabMounterID);
	
	ProgWnd.StepIt();
}

void CTeachTabMounterDlg::OnGetOriginOffset() 
{
	CString strMsg = "";
	// 원점 복귀 완료 확인.
	if (FALSE == m_plnkTabMounter->GetMotionComponent()->IsOriginReturn())
	{
		m_plnkTabMounter->UpCyl();

//		 현재 TabMounter 원점 복귀 안됨.
		MyMessageBox(MY_MESSAGE_BOX,51008,_T("Error"), M_ICONERROR);
		return;
	}
	int iResult = m_plnkCameraCarrier->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	// 기준 위치로 이동 확인.
//	 TabMounter가 기준 위치에 있습니까?
	iResult = MyMessageBox(MY_MESSAGE_BOX,51009,_T("Check Pre-Condition"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
		return;

	iResult = m_plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, "Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	// 현재 위치 저장
	double dCurrentPos = m_plnkTabMounter->GetCurrentPos();
	double dCamCarrierCurPos = m_plnkCameraCarrier->GetCurrentPos();
	
	// 원점 복귀 Offset 에 현재 위치 더하기.
	MAxisData sAxisData;
	m_plnkTabMounter->GetMotionComponent()->GetParameter(&sAxisData);
	double dHomeOffset = sAxisData.dHomeOffset;
	double dMechOffset = 0.0;

	double dPrismDist = 0.0;
//	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		// (+) -> (-) 방향으로 Offset 측정
		dPrismDist = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[m_iGroupNo][DEF_INSPECTION_CARRIER_1];
		dMechOffset = (dPrismDist + DEF_TABMOUNTER_TOOL_SIZE) / 2.0;
//		dHomeOffset += dCamCarrierCurPos - (dCurrentPos - dMechOffset);
		dHomeOffset += (dCurrentPos - dMechOffset) - dCamCarrierCurPos;
	}

	// 원점 복귀 Offset 저장
	sAxisData.dHomeOffset = dHomeOffset;
	m_plnkTabMounter->GetMotionComponent()->SaveParameter(&sAxisData);
	
	// 원점 복귀 Flag Clear
	m_plnkTabMounter->GetMotionComponent()->ResetOrigin();
	m_plnkTabMounter->GetMotionComponent()->ResetOriginComplete();
	
	// 원점 복귀 재요구.
//	 TabMounter 원점 복귀 Offset 계산 완료.\r\n원점 복귀 완료 Flag를 해제함.
	MyMessageBox(MY_MESSAGE_BOX,51010,_T("Operation Complete"), M_ICONINFORMATION);
}

int CTeachTabMounterDlg::getWorkTabNo()
{
	WCHAR szBeforeVal[100];
	WCHAR szAfterVal[100];

	memset(szBeforeVal, 0, sizeof(szBeforeVal));
	memset(szAfterVal, 0, sizeof(szAfterVal));
//	 이동 위치의 Tab 번호
	CString strMsg;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_16"), &strMsg);	

	
	swprintf(szBeforeVal, _T("%d"), m_iSelectedTabNo + 1);
	if (FALSE == GetNumberBox(szAfterVal, 10, szBeforeVal, strMsg.GetBuffer(strMsg.GetLength())))
		return ERR_TABMOUNTER_TEACH_DLG_CANCEAL;
	
	int iTabNo = _wtoi(szAfterVal) - 1;
//	EWorkingSide eWorkSide = m_plnkProcessData->GetCurrentWorkingSide(m_iGroupNo);
//	if (iTabNo < -1 || iTabNo >= m_plnkProcessData->GetMaxTabQty(eWorkSide))
//		iTabNo = 0;
	if (iTabNo < -1 || iTabNo >= m_plnkWorkSchedule[m_iWorkScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP))
		iTabNo = 0;
	
	m_iSelectedTabNo = iTabNo;
	return ERR_TABMOUNTER_TEACH_DLG_SUCCESS;
}

void CTeachTabMounterDlg::OnGantryEnable() 
{
	m_bGantryMoveSts = !!!m_bGantryMoveSts;
//	m_plnkTabMounter->SetGantryMotionEnable(m_bGantryMoveSts, m_iGantryLoadPosID);
	m_plnkTabMounter->SetGantryMotionEnable(m_bGantryMoveSts);
}

//110225.Add____________
void CTeachTabMounterDlg::setEnableAllMoveBtn()
{
	BOOL bEnableBtn = FALSE;
	switch (m_iSelectedAxis)
	{
	case DEF_MOUNT_Y:
		switch (m_iSelectedPos)
		{
		case DEF_TABMOUNTER_READY_POS:
			bEnableBtn = TRUE;
			break;
		case DEF_TABMOUNTER_LOAD_POS:
//@#ifdef DEF_SOURCE_SYSTEM
//@			bEnableBtn = TRUE;
//@#else
			bEnableBtn = FALSE;
//@#endif
			break;
//@#ifdef DEF_SOURCE_SYSTEM
//@		case DEF_TABMOUNTER_R_LOAD_POS:
//@			bEnableBtn = TRUE;
//@			break;
//@#else
		case DEF_TABMOUNTER_ESCAPE_POS:
			bEnableBtn = TRUE;
			break;
//@#endif
		case DEF_TABMOUNTER_INSPECTION_POS:
		case DEF_TABMOUNTER_MOUNT_POS:
		case DEF_TABMOUNTER_REMOVE_POS:
			bEnableBtn = FALSE;
			break;
		}
		break;
	case DEF_MOUNT_Z:
		bEnableBtn = TRUE;
		break;
	case DEF_GANTRY:
		bEnableBtn = FALSE;
		break;
	}
	GetDlgItem(IDB_CM_MOVE_ALL)->EnableWindow(bEnableBtn);
}

void CTeachTabMounterDlg::OnClickCmViewSchedule() 
{
	// TODO: Add your control notification handler code here
	CDlgViewWorkSchedule dlg(MOlbSystemPre.GetWorkSchedule(m_iGroupNo));
	dlg.DoModal();
}

void CTeachTabMounterDlg::OnClickCmMoveAll() 
{
	// TODO: Add your control notification handler code here
//	 목표위치로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,51006,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
	m_plnkC_MngOpPanel->SetJogUnit();

	int iResult = 0;
	int i = 0;
	int iWorkGroup = m_iGroupNo;
//@#ifdef DEF_SOURCE_SYSTEM
//@	if (m_iTabMounterID < DEF_MOUNTER_WORK_PER_GROUP)
//@		iWorkGroup = DEF_FRONT_GROUP;
//@	else
//@		iWorkGroup = DEF_REAR_GROUP;
//@#endif
	double dTgFPos[DEF_MOUNTER_WORK_PER_GROUP];
	double dTgRPos[DEF_MOUNTER_WORK_PER_GROUP];
	switch (m_iSelectedAxis)
	{		
	case DEF_MOUNT_Y:
		/*/
#ifdef DEF_SOURCE_SYSTEM
		switch (m_iSelectedPos)
		{
		case DEF_TABMOUNTER_READY_POS:
		case DEF_TABMOUNTER_LOAD_POS:
			for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
			{
				if (i < DEF_MOUNTER_WORK_PER_GROUP)
					dTgFPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(m_iSelectedPos);
				else
					dTgRPos[DEF_MAX_TABMOUNTER-1-i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(m_iSelectedPos);
			}
			iResult = MOlbSystemPre.MoveAllTabMounter(dTgFPos, dTgRPos, m_iSelectedPos);
			break;
		case DEF_TABMOUNTER_R_LOAD_POS:
			if (iWorkGroup == DEF_FRONT_GROUP)
			{
				for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
				{
					if (i < DEF_MOUNTER_WORK_PER_GROUP)
						dTgFPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(m_iSelectedPos);
					else
						dTgRPos[DEF_MAX_TABMOUNTER-1-i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(DEF_TABMOUNTER_READY_POS);
				}
			}
			else
			{
				for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
				{
					if (i < DEF_MOUNTER_WORK_PER_GROUP)
						dTgFPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(DEF_TABMOUNTER_READY_POS);
					else
						dTgRPos[DEF_MAX_TABMOUNTER-1-i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(m_iSelectedPos);
				}
			}
			iResult = MOlbSystemPre.MoveAllTabMounter(dTgFPos, dTgRPos, m_iSelectedPos);
			break;
		}
#else
		/*/
		switch (m_iSelectedPos)
		{
		case DEF_TABMOUNTER_READY_POS:
		case DEF_TABMOUNTER_ESCAPE_POS:
			for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
			{
				if (iWorkGroup == DEF_FRONT_GROUP)
				{
					dTgFPos[i] = MOlbSystemPre.GetTabMounterComponent(i)->GetTargetPos(m_iWorkScheduleNo, m_iSelectedPos);
					iResult = MOlbSystemPre.MoveAllFrontTabMounter(dTgFPos, m_iSelectedPos);
				}
				else
				{
					dTgRPos[i] = MOlbSystemPre.GetRTabMounterComponent(i)->GetTargetPos(m_iWorkScheduleNo, m_iSelectedPos);
					iResult = MOlbSystemPre.MoveAllRearTabMounter(dTgRPos, m_iSelectedPos);
				}
			}
			break;
		}
//@#endif
		break;
	case DEF_MOUNT_Z:
		iResult = MOlbSystemPre.MoveAllTabMounterZ(iWorkGroup, m_iSelectedPos);
		break;
	}

	if (iResult != ERR_TABMOUNTER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
}

//______________________

void CTeachTabMounterDlg::OnClickSelectSchedulePrev() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[250];
	if (m_iWorkScheduleNo <= 0) return;
	m_iWorkScheduleNo--;
	swprintf(szTemp, _T("Schedule\r\nNO.%d"), m_iWorkScheduleNo+1);
	m_lblScheduleNo.SetWindowText(szTemp);

	m_iTabMounterOldID = -1;
	setWorkTabMounter(m_iTabMounterID);
	
}

void CTeachTabMounterDlg::OnClickSelectScheduleNext() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[250];
	if (m_iWorkScheduleNo >= DEF_MAX_WORK_SCHEDULE-1) return;
	m_iWorkScheduleNo++;
	swprintf(szTemp, _T("Schedule\r\nNO.%d"), m_iWorkScheduleNo+1);
	m_lblScheduleNo.SetWindowText(szTemp);

	m_iTabMounterOldID = -1;
	setWorkTabMounter(m_iTabMounterID);	
}
