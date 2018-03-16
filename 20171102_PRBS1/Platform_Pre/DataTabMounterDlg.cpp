// DataTabMounterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "DataTabMounterDlg.h"
#include "MSystemData.h"
#include "MProcessData.h"
#include "MTabMounter.h"
#include "MCameraCarrier.h"
#include "MTrsAutoManager.h"
#include "MTrsInspectionCamera.h"
#include "MTabCarrier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataTabMounterDlg dialog

CDataTabMounterDlg::CDataTabMounterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataTabMounterDlg::IDD, pParent), m_iSelectedItemNo(0)
{
	//{{AFX_DATA_INIT(CDataTabMounterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CDataTabMounterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataTabMounterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	//TabMounter Button___________________________________
	DDX_Control(pDX, IDB_SYSTEM0, m_btnItem_TabMounter[0]);
	DDX_Control(pDX, IDB_SYSTEM1, m_btnItem_TabMounter[1]);
	DDX_Control(pDX, IDB_SYSTEM2, m_btnItem_TabMounter[2]);
	DDX_Control(pDX, IDB_SYSTEM3, m_btnItem_TabMounter[3]);
	DDX_Control(pDX, IDB_SYSTEM4, m_btnItem_TabMounter[4]);
	DDX_Control(pDX, IDB_SYSTEM5, m_btnItem_TabMounter[5]);
	DDX_Control(pDX, IDB_SYSTEM6, m_btnItem_TabMounter[6]);
	DDX_Control(pDX, IDB_SYSTEM7, m_btnItem_TabMounter[7]);
	DDX_Control(pDX, IDB_SYSTEM8, m_btnItem_TabMounter[8]);
	DDX_Control(pDX, IDB_SYSTEM9, m_btnItem_TabMounter[9]);

	//TabMounter Label____________________________________
	DDX_Control(pDX, IDS_SYSTEM0, m_lblItem_TabMounter[0]);
	DDX_Control(pDX, IDS_SYSTEM1, m_lblItem_TabMounter[1]);
	DDX_Control(pDX, IDS_SYSTEM2, m_lblItem_TabMounter[2]);
	DDX_Control(pDX, IDS_SYSTEM3, m_lblItem_TabMounter[3]);
	DDX_Control(pDX, IDS_SYSTEM4, m_lblItem_TabMounter[4]);
	DDX_Control(pDX, IDS_SYSTEM5, m_lblItem_TabMounter[5]);
	DDX_Control(pDX, IDS_SYSTEM6, m_lblItem_TabMounter[6]);
	DDX_Control(pDX, IDS_SYSTEM7, m_lblItem_TabMounter[7]);
	DDX_Control(pDX, IDS_SYSTEM8, m_lblItem_TabMounter[8]);
	DDX_Control(pDX, IDS_SYSTEM9, m_lblItem_TabMounter[9]);

	//Camera Column Button________________________________
	DDX_Control(pDX, IDB_COLUMN0, m_btnItem_CameraColumn[0]);
	DDX_Control(pDX, IDB_COLUMN1, m_btnItem_CameraColumn[1]);
	DDX_Control(pDX, IDB_COLUMN2, m_btnItem_CameraColumn[2]);

	//Camera Button_______________________________________
	DDX_Control(pDX, IDB_SYSTEM10, m_btnItem_Camera[0]);
	DDX_Control(pDX, IDB_SYSTEM11, m_btnItem_Camera[1]);
	DDX_Control(pDX, IDB_SYSTEM12, m_btnItem_Camera[2]);
	DDX_Control(pDX, IDB_SYSTEM13, m_btnItem_Camera[3]);
	DDX_Control(pDX, IDB_SYSTEM14, m_btnItem_Camera[4]);
	DDX_Control(pDX, IDB_SYSTEM15, m_btnItem_Camera[5]);
	DDX_Control(pDX, IDB_SYSTEM16, m_btnItem_Camera[6]);
	DDX_Control(pDX, IDB_SYSTEM17, m_btnItem_Camera[7]);
	DDX_Control(pDX, IDB_SYSTEM18, m_btnItem_Camera[8]);
	DDX_Control(pDX, IDB_SYSTEM19, m_btnItem_Camera[9]);
	DDX_Control(pDX, IDB_SYSTEM20, m_btnItem_Camera[10]);
	DDX_Control(pDX, IDB_SYSTEM21, m_btnItem_Camera[11]);

	//Camera Label________________________________________
	DDX_Control(pDX, IDS_SYSTEM10, m_lblItem_Camera[0][0]);
	DDX_Control(pDX, IDS_SYSTEM11, m_lblItem_Camera[1][0]);
	DDX_Control(pDX, IDS_SYSTEM12, m_lblItem_Camera[2][0]);
	DDX_Control(pDX, IDS_SYSTEM13, m_lblItem_Camera[3][0]);
	DDX_Control(pDX, IDS_SYSTEM14, m_lblItem_Camera[4][0]);
	DDX_Control(pDX, IDS_SYSTEM15, m_lblItem_Camera[5][0]);
	DDX_Control(pDX, IDS_SYSTEM16, m_lblItem_Camera[6][0]);
	DDX_Control(pDX, IDS_SYSTEM17, m_lblItem_Camera[7][0]);
	DDX_Control(pDX, IDS_SYSTEM18, m_lblItem_Camera[8][0]);
	DDX_Control(pDX, IDS_SYSTEM19, m_lblItem_Camera[9][0]);
	DDX_Control(pDX, IDS_SYSTEM20, m_lblItem_Camera[10][0]);
	DDX_Control(pDX, IDS_SYSTEM21, m_lblItem_Camera[11][0]);

	DDX_Control(pDX, IDS_SYSTEM10_1, m_lblItem_Camera[0][1]);
	DDX_Control(pDX, IDS_SYSTEM11_1, m_lblItem_Camera[1][1]);
	DDX_Control(pDX, IDS_SYSTEM12_1, m_lblItem_Camera[2][1]);
	DDX_Control(pDX, IDS_SYSTEM13_1, m_lblItem_Camera[3][1]);
	DDX_Control(pDX, IDS_SYSTEM14_1, m_lblItem_Camera[4][1]);
	DDX_Control(pDX, IDS_SYSTEM15_1, m_lblItem_Camera[5][1]);
	DDX_Control(pDX, IDS_SYSTEM16_1, m_lblItem_Camera[6][1]);
	DDX_Control(pDX, IDS_SYSTEM17_1, m_lblItem_Camera[7][1]);
	DDX_Control(pDX, IDS_SYSTEM18_1, m_lblItem_Camera[8][1]);
	DDX_Control(pDX, IDS_SYSTEM19_1, m_lblItem_Camera[9][1]);
	DDX_Control(pDX, IDS_SYSTEM20_1, m_lblItem_Camera[10][1]);
	DDX_Control(pDX, IDS_SYSTEM21_1, m_lblItem_Camera[11][1]);

	DDX_Control(pDX, IDS_SYSTEM10_2, m_lblItem_Camera[0][2]);
	DDX_Control(pDX, IDS_SYSTEM11_2, m_lblItem_Camera[1][2]);
	DDX_Control(pDX, IDS_SYSTEM12_2, m_lblItem_Camera[2][2]);
	DDX_Control(pDX, IDS_SYSTEM13_2, m_lblItem_Camera[3][2]);
	DDX_Control(pDX, IDS_SYSTEM14_2, m_lblItem_Camera[4][2]);
	DDX_Control(pDX, IDS_SYSTEM15_2, m_lblItem_Camera[5][2]);
	DDX_Control(pDX, IDS_SYSTEM16_2, m_lblItem_Camera[6][2]);
	DDX_Control(pDX, IDS_SYSTEM17_2, m_lblItem_Camera[7][2]);
	DDX_Control(pDX, IDS_SYSTEM18_2, m_lblItem_Camera[8][2]);
	DDX_Control(pDX, IDS_SYSTEM19_2, m_lblItem_Camera[9][2]);
	DDX_Control(pDX, IDS_SYSTEM20_2, m_lblItem_Camera[10][2]);
	DDX_Control(pDX, IDS_SYSTEM21_2, m_lblItem_Camera[11][2]);
}


BEGIN_MESSAGE_MAP(CDataTabMounterDlg, CDialog)
	//{{AFX_MSG_MAP(CDataTabMounterDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTabMounterDlg message handlers

BEGIN_EVENTSINK_MAP(CDataTabMounterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataTabMounterDlg)
	ON_EVENT(CDataTabMounterDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_COLUMN0, -600 /* Click */, OnColumn0, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_COLUMN1, -600 /* Click */, OnColumn1, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_COLUMN2, -600 /* Click */, OnColumn2, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM0, -600 /* Click */, OnSystem0, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM1, -600 /* Click */, OnSystem1, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM2, -600 /* Click */, OnSystem2, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM3, -600 /* Click */, OnSystem3, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM4, -600 /* Click */, OnSystem4, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM5, -600 /* Click */, OnSystem5, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM6, -600 /* Click */, OnSystem6, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM7, -600 /* Click */, OnSystem7, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM8, -600 /* Click */, OnSystem8, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM9, -600 /* Click */, OnSystem9, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM10, -600 /* Click */, OnSystem10, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM11, -600 /* Click */, OnSystem11, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM12, -600 /* Click */, OnSystem12, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM13, -600 /* Click */, OnSystem13, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM14, -600 /* Click */, OnSystem14, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM15, -600 /* Click */, OnSystem15, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM16, -600 /* Click */, OnSystem16, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM17, -600 /* Click */, OnSystem17, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM18, -600 /* Click */, OnSystem18, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM19, -600 /* Click */, OnSystem19, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM20, -600 /* Click */, OnClickSystem20, VTS_NONE)
	ON_EVENT(CDataTabMounterDlg, IDB_SYSTEM21, -600 /* Click */, OnClickSystem21, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataTabMounterDlg::OnSave() 
{
//	변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 41100, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkSystemData->m_iMountRetryLimit			= static_cast<int>(m_dData_TabMounter[0]);
	m_plnkSystemData->m_dBondTime					= m_dData_TabMounter[1];
	m_plnkSystemData->m_dStartMoveDelay				= m_dData_TabMounter[2];
	m_plnkSystemData->m_d2ndGroupMountStartDelay	= m_dData_TabMounter[3];
	m_plnkSystemData->m_dIFDelayLimit				= m_dData_TabMounter[4];
	//												= m_dData_TabMounter[5];
	m_plnkSystemData->m_dInspectCamFixedPosWidth	= m_dData_TabMounter[5];
	m_plnkSystemData->m_dTabMounterStabilizationDelay = m_dData_TabMounter[6];
	m_plnkSystemData->m_dMountPosTolerance			= m_dData_TabMounter[7];
	m_plnkSystemData->m_dMountMoveSpeed				= m_dData_TabMounter[8];
	//Reserved										= m_dData_TabMounter[9];

	//Expand Distance_______________________________
	m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][0]					= m_dData_Camera[0][0];
	m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][1]					= m_dData_Camera[1][0];
#	ifdef DEF_SOURCE_SYSTEM
	m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][2]					= m_dData_Camera[2][0];
	m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][3]					= m_dData_Camera[3][0];
#	endif
	m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][0]					= m_dData_Camera[8][0];
	m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][1]					= m_dData_Camera[9][0];
#	ifdef DEF_SOURCE_SYSTEM
	m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][2]					= m_dData_Camera[10][0];
	m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][3]					= m_dData_Camera[11][0];
#	endif
	m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][4]					= m_dData_Camera[4][0];
	m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][5]					= m_dData_Camera[6][0];

	m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][4]					= m_dData_Camera[5][0];
	m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][5]					= m_dData_Camera[7][0];
	//_______________________________________________

	//Shrink Distance________________________________
	m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_FRONT_GROUP][0] = m_dData_Camera[0][1];
	m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_FRONT_GROUP][1] = m_dData_Camera[1][1];
#	ifdef DEF_SOURCE_SYSTEM
	m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_FRONT_GROUP][2] = m_dData_Camera[2][1];
	m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_FRONT_GROUP][3] = m_dData_Camera[3][1];
#	endif
	m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_REAR_GROUP][0] = m_dData_Camera[6][1];
	m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_REAR_GROUP][1] = m_dData_Camera[7][1];
#	ifdef DEF_SOURCE_SYSTEM
	m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_REAR_GROUP][2] = m_dData_Camera[8][1];
	m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_REAR_GROUP][3] = m_dData_Camera[9][1];
#	endif
	m_plnkSystemData->m_rgdPreAlignCamDistViewCenter_Shrink[0] = m_dData_Camera[4][1];
	m_plnkSystemData->m_rgdPreAlignCamDistViewCenter_Shrink[1] = m_dData_Camera[5][1];
	//_______________________________________________

	//Center to Center Distance______________________
	m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_FRONT_GROUP][0] = m_dData_Camera[0][2];
	m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_FRONT_GROUP][1] = m_dData_Camera[1][2];
#	ifdef DEF_SOURCE_SYSTEM
	m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_FRONT_GROUP][2] = m_dData_Camera[2][2];
	m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_FRONT_GROUP][3] = m_dData_Camera[3][2];
#	endif
	m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_REAR_GROUP][0] = m_dData_Camera[6][2];
	m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_REAR_GROUP][1] = m_dData_Camera[7][2];
#	ifdef DEF_SOURCE_SYSTEM
	m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_REAR_GROUP][2] = m_dData_Camera[8][2];
	m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_REAR_GROUP][3] = m_dData_Camera[9][2];
#	endif
	m_plnkSystemData->m_rgdPreAlignCamDist_CenterToCenter[0] = m_dData_Camera[4][2];
	m_plnkSystemData->m_rgdPreAlignCamDist_CenterToCenter[1] = m_dData_Camera[5][2];
	//_______________________________________________

	m_plnkSystemData->WriteData();

	int i = 0;

	MOlbSystemPre.GetProcessData()->ChangeProcessData();
	MOlbSystemPre.AssignWorkScheduleData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		MOlbSystemPre.GetWorkSchedule(i)->ChangeModel();
#ifdef DEF_SOURCE_SYSTEM
		MOlbSystemPre.GetWorkScheduleSecond(i)->ChangeModel();
#endif
	}

	// TabMounter, Inspection Camera, Panel Align Cam2(Source) 고정 좌표에 측정 결과 반영.
	for (int i=0; i<DEF_MAX_WORKER_PER_GROUP; i++)
	{
		MOlbSystemPre.GetTabMounterComponent(i)->ChangeModel();
//@#ifdef DEF_GATE_SYSTEM
		MOlbSystemPre.GetRTabMounterComponent(i)->ChangeModel();
//@#endif
	}

	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		MOlbSystemPre.GetCameraCarrierComponent(i)->ChangeModel();
//@#ifdef DEF_GATE_SYSTEM
		MOlbSystemPre.GetRCameraCarrierComponent(i)->ChangeModel();
//@#endif
		//SJ_YYK 110519 Add..
		MOlbSystemPre.GetTrsInspectionCameraComponent(i)->ResetInitialized();
		MOlbSystemPre.GetTrsRInspectionCameraComponent(i)->ResetInitialized();
	}

	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		MOlbSystemPre.GetTabCarrierComponent(i)->ChangeModel();
		MOlbSystemPre.GetRTabCarrierComponent(i)->ChangeModel();
	}
}

void CDataTabMounterDlg::OnExit() 
{
	CDialog::OnCancel();	
}

BOOL CDataTabMounterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_iSelectedItemNo = -1;
	m_iSelectedColumnNo = 0;

	// TODO: Add extra initialization here
	displayItemTitle();
	displayItemLabel(TRUE);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataTabMounterDlg_1"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataTabMounterDlg_2"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	/*/
#ifdef DEF_SOURCE_SYSTEM
	for (int i = 0; i < 4; i++)
	{
		m_btnItem_Camera[6+i].EnableWindow(FALSE);
		for (int j = 0; j < DEF_DATA_TABMOUNTER_DLG_CAMERA_COLUMN_ITEM; j++)
		{
			m_lblItem_Camera[6+i][j].EnableWindow(FALSE);
		}
	}
#endif
	/*/

	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	//_______________

	m_btnItem_CameraColumn[1].EnableWindow(FALSE);
	m_btnItem_CameraColumn[2].EnableWindow(FALSE);

	for (int i=0; i<DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM; i++)
	{
		m_lblItem_Camera[i][1].EnableWindow(FALSE);
		m_lblItem_Camera[i][2].EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataTabMounterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDataTabMounterDlg::displayItemTitle()
{
	WCHAR* szItemTabMounter[DEF_DATA_TABMOUNTER_DLG_TABMOUNTER_ITEM]
		= {	
			_T("Mount Retry\r\nLimit"),
			_T("Bond Time"),
			_T("Start Move\r\nDelay"),
			_T("2ndGroupMountStartDelay"),
			_T("IF Delay\r\nLimit"),
//@			_T("Reserved"),
			_T("Camera width at fixed pos"),
			_T("Stabilization\r\nDelay"),
			_T("Mount Pos\r\nTolerance"),
			_T("Mountr Mount\r\nMove Speed"),
			_T("Reserved")
			};
	for (int i = 0; i < DEF_DATA_TABMOUNTER_DLG_TABMOUNTER_ITEM; i++)
	{
		m_btnItem_TabMounter[i].SetWindowText(szItemTabMounter[i]);
		if (0 == lstrcmp(szItemTabMounter[i], _T("Reserved")))
			m_btnItem_TabMounter[i].EnableWindow(FALSE);
	}

	WCHAR* szItemCamera[DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM]
		= {	
			_T("Inspect Camera\r\nDist1"),
			_T("Inspect Camera\r\nDist2"),
			_T("Inspect Camera\r\nDist3"),
			_T("Inspect Camera\r\nDist4"),
			_T("Carrier Camera\r\nDist"),
			_T("RCarrier Camera\r\nDist"),
			_T("Carrier Camera2\r\nDist"),
			_T("RCarrier Camera2\r\nDist"),
			_T("RInspect Camera\r\nDist1"),
			_T("RInspect Camera\r\nDist2"),
			_T("RInspect Camera\r\nDist3"),
			_T("RInspect Camera\r\nDist4"),
			};

	for (int i = 0; i < DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM; i++)
	{
		m_btnItem_Camera[i].SetWindowText(szItemCamera[i]);
		if (0 == lstrcmp(szItemCamera[i], _T("Reserved")))
			m_btnItem_Camera[i].EnableWindow(FALSE);
	}


	m_btnItem_CameraColumn[0].SetWindowText(_T("Origin\r\nDistance"));
	m_btnItem_CameraColumn[1].SetWindowText(_T("Shrink\r\nDistance"));
	m_btnItem_CameraColumn[2].SetWindowText(_T("Center2Center\r\nDistance"));
}

void CDataTabMounterDlg::displayItemLabel(BOOL bUpdate)
{
	int i = 0;
	if (TRUE == bUpdate)
	{
		m_dData_TabMounter[0] = m_plnkSystemData->m_iMountRetryLimit;
		m_dData_TabMounter[1] = m_plnkSystemData->m_dBondTime;
		m_dData_TabMounter[2] = m_plnkSystemData->m_dStartMoveDelay;
		m_dData_TabMounter[3] = m_plnkSystemData->m_d2ndGroupMountStartDelay;
		m_dData_TabMounter[4] = m_plnkSystemData->m_dIFDelayLimit;
//@		m_dData_TabMounter[5] = 0.0;
		m_dData_TabMounter[5] = m_plnkSystemData->m_dInspectCamFixedPosWidth;
		m_dData_TabMounter[6] = m_plnkSystemData->m_dTabMounterStabilizationDelay;
		m_dData_TabMounter[7] = m_plnkSystemData->m_dMountPosTolerance;
		m_dData_TabMounter[8] = m_plnkSystemData->m_dMountMoveSpeed;
		m_dData_TabMounter[9] = 0.0;
		

		m_dData_Camera[0][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][0];
		m_dData_Camera[1][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][1];
#	ifdef DEF_SOURCE_SYSTEM
		m_dData_Camera[2][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][2];
		m_dData_Camera[3][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][3];
#	endif
		m_dData_Camera[8][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][0];
		m_dData_Camera[9][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][1];
#	ifdef DEF_SOURCE_SYSTEM
		m_dData_Camera[10][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][2];
		m_dData_Camera[11][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][3];
#	endif
		m_dData_Camera[4][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][4];
		m_dData_Camera[6][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][5];

		m_dData_Camera[5][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][4];
		m_dData_Camera[7][0] = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][5];


		m_dData_Camera[0][1] = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_FRONT_GROUP][0];
		m_dData_Camera[1][1] = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_FRONT_GROUP][1];
#	ifdef DEF_SOURCE_SYSTEM
		m_dData_Camera[2][1] = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_FRONT_GROUP][2];
		m_dData_Camera[3][1] = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_FRONT_GROUP][3];
#	endif
		m_dData_Camera[4][1] = m_plnkSystemData->m_rgdPreAlignCamDistViewCenter_Shrink[0];
		m_dData_Camera[5][1] = m_plnkSystemData->m_rgdPreAlignCamDistViewCenter_Shrink[1];
		m_dData_Camera[6][1] = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_REAR_GROUP][0];
		m_dData_Camera[7][1] = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_REAR_GROUP][1];
#	ifdef DEF_SOURCE_SYSTEM
		m_dData_Camera[8][1] = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_REAR_GROUP][2];
		m_dData_Camera[9][1] = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Shrink[DEF_REAR_GROUP][3];
#	endif
		m_dData_Camera[0][2] = m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_FRONT_GROUP][0];
		m_dData_Camera[1][2] = m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_FRONT_GROUP][1];
#	ifdef DEF_SOURCE_SYSTEM
		m_dData_Camera[2][2] = m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_FRONT_GROUP][2];
		m_dData_Camera[3][2] = m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_FRONT_GROUP][3];
#	endif
		m_dData_Camera[4][2] = m_plnkSystemData->m_rgdPreAlignCamDist_CenterToCenter[0];
		m_dData_Camera[5][2] = m_plnkSystemData->m_rgdPreAlignCamDist_CenterToCenter[1];
		m_dData_Camera[6][2] = m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_REAR_GROUP][0];
		m_dData_Camera[7][2] = m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_REAR_GROUP][1];
#	ifdef DEF_SOURCE_SYSTEM
		m_dData_Camera[8][2] = m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_REAR_GROUP][2];
		m_dData_Camera[9][2] = m_plnkSystemData->m_rgdInspectionCamDist_CenterToCenter[DEF_REAR_GROUP][3];

		m_dData_Camera[10][1] = 0.0;
		m_dData_Camera[11][1] = 0.0;
		m_dData_Camera[10][2] = 0.0;
		m_dData_Camera[11][2] = 0.0;
#	endif
	}

	CString strValue;
	for (int i=0; i<DEF_DATA_TABMOUNTER_DLG_TABMOUNTER_ITEM; i++)
	{
		if (i == 0)
			strValue.Format(_T("%d"), static_cast<int>(m_dData_TabMounter[i]));
		else
			strValue.Format(_T("%.3lf"), m_dData_TabMounter[i]);
		m_lblItem_TabMounter[i].SetCaption(strValue);
	}

	for (int i=0; i<DEF_DATA_TABMOUNTER_DLG_CAMERA_COLUMN_ITEM; i++)
	{
		for (int j=0; j<DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM; j++)
		{
			strValue.Format(_T("%.3lf"), m_dData_Camera[j][i]);
			m_lblItem_Camera[j][i].SetCaption(strValue);
		}
	}
}

void CDataTabMounterDlg::getUserInputTabMounter(int iItemNo)
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________

	swprintf(m_cBuf, _T("%.0f"), m_dData_TabMounter[iItemNo]);
	if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
		return;	

	m_dData_TabMounter[iItemNo] = wcstod(m_cBufGet, NULL);
	displayItemLabel();
}

void CDataTabMounterDlg::getUserInput(int iColumnNo)
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________

	memset(m_cBufGet, 0, sizeof(m_cBufGet));
	swprintf(m_cBuf, _T("%.4f"), m_dData_Camera[m_iSelectedItemNo][iColumnNo]);
	if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
		return;	

	m_dData_Camera[m_iSelectedItemNo][iColumnNo] = wcstod(m_cBufGet, NULL);
	displayItemLabel();
}

void CDataTabMounterDlg::OnColumn0() 
{
	if (m_iSelectedItemNo < 0 || m_iSelectedItemNo >= DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM )
		return;
	m_iSelectedColumnNo = 0;
	getUserInput(0);
}

void CDataTabMounterDlg::OnColumn1() 
{
	if (m_iSelectedItemNo < 0 || m_iSelectedItemNo >= DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM )
		return;
	m_iSelectedColumnNo = 1;
	getUserInput(1);
}

void CDataTabMounterDlg::OnColumn2() 
{
	if (m_iSelectedItemNo < 0 || m_iSelectedItemNo >= DEF_DATA_TABMOUNTER_DLG_CAMERA_ITEM )
		return;
	m_iSelectedColumnNo = 2;
	getUserInput(2);
}

void CDataTabMounterDlg::OnSystem0() 
{
	getUserInputTabMounter(0);
}

void CDataTabMounterDlg::OnSystem1() 
{
	getUserInputTabMounter(1);
}

void CDataTabMounterDlg::OnSystem2() 
{
	getUserInputTabMounter(2);
}

void CDataTabMounterDlg::OnSystem3() 
{
	getUserInputTabMounter(3);
}

void CDataTabMounterDlg::OnSystem4() 
{
	getUserInputTabMounter(4);
}

void CDataTabMounterDlg::OnSystem5() 
{
	getUserInputTabMounter(5);
}

void CDataTabMounterDlg::OnSystem6() 
{
	getUserInputTabMounter(6);
}

void CDataTabMounterDlg::OnSystem7() 
{
	getUserInputTabMounter(7);
}

void CDataTabMounterDlg::OnSystem8() 
{
	getUserInputTabMounter(8);
}

void CDataTabMounterDlg::OnSystem9() 
{
	getUserInputTabMounter(9);
}

void CDataTabMounterDlg::OnSystem10() 
{
	m_iSelectedItemNo = 0;
}

void CDataTabMounterDlg::OnSystem11() 
{
	m_iSelectedItemNo = 1;
}

void CDataTabMounterDlg::OnSystem12() 
{
	m_iSelectedItemNo = 2;
}

void CDataTabMounterDlg::OnSystem13()
{
	m_iSelectedItemNo = 3;
}

void CDataTabMounterDlg::OnSystem14() 
{
	m_iSelectedItemNo = 4;
}

void CDataTabMounterDlg::OnSystem15() 
{
	m_iSelectedItemNo = 5;
}

void CDataTabMounterDlg::OnSystem16() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedItemNo = 6;	
}

void CDataTabMounterDlg::OnSystem17() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedItemNo = 7;	
}

void CDataTabMounterDlg::OnSystem18() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedItemNo = 8;	
}

void CDataTabMounterDlg::OnSystem19() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedItemNo = 9;	
}

void CDataTabMounterDlg::OnClickSystem20() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedItemNo = 10;	
}

void CDataTabMounterDlg::OnClickSystem21() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedItemNo = 11;	
}
