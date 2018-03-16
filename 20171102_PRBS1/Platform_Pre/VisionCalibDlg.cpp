// VisionCalibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "VisionCalibDlg.h"
#include "VisionSelectMarkDlg.h"
#include "VisionOptionDlg.h"
#include "VisionJogKeyDlg.h"
#include "IVision.h"
#include "IAxis.h"
#include "MVisionCalibration.h"
#include "MPanelAligner.h"
#include "MModelChange.h"
#include "MTabMounter.h"
//KKY.Del #include "MTabAligner.h"
#include "MCameraCarrier.h"
#include "MTabCarrier.h"
#include "MManageOpPanel.h"
#include "MCtrlPanelAligner.h"
#include <math.h>
#include "MPanelData.h"
#include "MPreBonderData.h"
#include "MPlatformOlbSystem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int g_iDisplayReferenceLine;
extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CVisionCalibDlg dialog

CVisionCalibDlg::CVisionCalibDlg(int iTargetCamNo, int iGroupID, int iInstanceID, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionCalibDlg::IDD, pParent), m_bIsOnLive(FALSE)
{
	//{{AFX_DATA_INIT(CVisionCalibDlg)
	//}}AFX_DATA_INIT
	int i = 0;
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkPanelData = MOlbSystemPre.GetPanelData();
	m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
	m_plnkVision = MOlbSystemPre.GetVisionComponent();
	m_plnkCalibration = MOlbSystemPre.GetVisionCalibrationComponent();
	m_dAngle = 3.0;

	m_plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
	m_plnkModelChange = MOlbSystemPre.GetModelChangeComponent(0);
	m_plnkModelChange2 = MOlbSystemPre.GetModelChangeComponent(1);
	m_plnkTabMounter = NULL;
//	m_plnkTabAligner = NULL;
	m_plnkCamCarrier = NULL;
	m_plnkTabCarrier = NULL;
	m_plnkCtrlPanelAligner = MOlbSystemPre.GetCtrlPanelAlignerComponent();

	m_iTargetCamNo = iTargetCamNo;
	
	m_iGroupID = iGroupID;
	m_iInstanceID = iInstanceID;
	m_iCalibUnit = 0;
	
	//Inspection 카메라 : PanelAligner가 0, TabMounter가 1
	//TabAlign 카메라	: 각 TabCarrier Instance
	switch (iGroupID)
	{
	case DEF_CALIB_GROUP_PANELALIGNER:
		m_iCalibUnit = 0;
		break;
	case DEF_CALIB_GROUP_MOUNTER:
		m_iCalibUnit = 1;
		break;
	case DEF_CALIB_GROUP_TABCARRIER:
		//m_iCalibUnit = iInstanceID;
		m_iCalibUnit = 0;
		break;
	}

	for (int i = 0; i < DEF_VISION_CALIBRATION_UNIT_PER_CAM; i++)
	{
		for (int j = 0 ; j < DEF_FACTOR_NUMBER ; j++)
		{
			m_rgdPortingFactor[i][j] = 0.0;
		}
	}

	// Camera 별 작업 Type 설정.
	for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
	{
		m_rgsType[i].m_bUsePrism = FALSE;
		///switch (m_iTargetCamNo)
		switch(i)
		{
		case DEF_F_TABALIGNER_CAMERA_1:
		case DEF_F_TABALIGNER_CAMERA_2:
		case DEF_R_TABALIGNER_CAMERA_1:
		case DEF_R_TABALIGNER_CAMERA_2:
		case DEF_F_TABALIGNER_CAMERA_3: //SJ_YYK 150817 Add...
		case DEF_F_TABALIGNER_CAMERA_4: //SJ_YYK 150817 Add...
		case DEF_R_TABALIGNER_CAMERA_3: //SJ_YYK 150817 Add...
		case DEF_R_TABALIGNER_CAMERA_4: //SJ_YYK 150817 Add...
			//TabCarrier로 이동하며 TabCarrier Camera Calibration
			m_rgsType[i].m_iType = DEF_CALIBRATION_TABCARRIER_2D;
			break;
		case DEF_INSPECTION_CAMERA_1:
		case DEF_INSPECTION_CAMERA_2:
		case DEF_INSPECTION_CAMERA_3:
		case DEF_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CAMERA_5:
		case DEF_INSPECTION_CAMERA_6:
		case DEF_INSPECTION_CAMERA_7:
		case DEF_INSPECTION_CAMERA_8:
#endif
		case DEF_R_INSPECTION_CAMERA_1:
		case DEF_R_INSPECTION_CAMERA_2:
		case DEF_R_INSPECTION_CAMERA_3:
		case DEF_R_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
		case DEF_R_INSPECTION_CAMERA_5:
		case DEF_R_INSPECTION_CAMERA_6:
		case DEF_R_INSPECTION_CAMERA_7:
		case DEF_R_INSPECTION_CAMERA_8:
#endif
			if(m_iCalibUnit == 0)
				//[Panel Aligner로 이동하며 InspectionCarrier Camera Calibration]
				m_rgsType[i].m_iType = DEF_CALIBRATION_2D;
			else
				//[TabMounter/Gantry로 이동하며 InspectionCarrier Camera Calibration]
				m_rgsType[i].m_iType = DEF_CALIBRATION_GANTRY_2D;
			break;
		default:
			break;
		}
	}

	MOlbSystemPre.LightOn(DEF_MAX_CAMERA_NO);
	setWorkingUnit(iTargetCamNo);
}

void CVisionCalibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionCalibDlg)
	DDX_Control(pDX, IDC_LIST1, m_listResult);
	DDX_Control(pDX, IDB_X_MOVE_POINT, m_ctrlBtnMovePointX);
	DDX_Control(pDX, IDB_X_MOVE_WIDTH, m_ctrlBtnMoveWidthX);
	DDX_Control(pDX, IDB_Y_MOVE_POINT, m_ctrlMovePointY);
	DDX_Control(pDX, IDB_Y_MOVE_WIDTH, m_ctrlBtnMoveWidthY);
	DDX_Control(pDX, IDC_X_MOVE_POINT, m_ctrlLblMovePointX);
	DDX_Control(pDX, IDC_X_MOVE_WIDTH, m_ctrlLblMoveWidthX);
	DDX_Control(pDX, IDC_Y_MOVE_POINT, m_ctrlLblMovePointY);
	DDX_Control(pDX, IDC_Y_MOVE_WIDTH, m_ctrlLblMoveWidthY);
	DDX_Control(pDX, IDC_CAMERA_NUM, m_ctrlLblCameraNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisionCalibDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionCalibDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionCalibDlg message handlers

BOOL CVisionCalibDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_MONITOR);
	RECT rect;
	pWnd->GetWindowRect(&rect);
	CRect rectView(rect.left, rect.top, rect.right, rect.bottom);
	ScreenToClient(&rectView);
	m_iViewID = m_plnkVision->CreateLocalView(&rectView, this);

	g_iDisplayReferenceLine = TRUE;

	OnLive();
	m_KeyTimerID = SetTimer(9, 100, NULL);

	m_bOnStillContinue = TRUE;

	displayLabel();

//@#ifdef DEF_SOURCE_SYSTEM
	GetDlgItem(IDB_CAMERA_DIST4)->EnableWindow(FALSE);
//@#endif

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;

	ReadDisplayMessage(SUB_DLG, _T("VisionCalibDlg_1"), &szTextTemp);
	//고정좌표 얻기\r\n(Panel Aligner 위치 Data)
	GetDlgItem(IDB_FIX_COORD)->SetWindowText(szTextTemp);

//	ReadDisplayMessage(SUB_DLG, _T("VisionCalibDlg_2"), &szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionCalibDlg_9"), &szTextTemp);
	//TabAlign Camera\r\n간 거리 얻기 ---> PreAlign Camera 가변 길이 측정
	GetDlgItem(IDB_CAMERA_DIST2)->SetWindowText(szTextTemp);


	ReadDisplayMessage(SUB_DLG, _T("VisionCalibDlg_4"), &szTextTemp);
	//Inspection Camera\r\nVision 중심 간 거리 얻기
	GetDlgItem(IDB_CAMERA_DIST3)->SetWindowText(szTextTemp);

	ReadDisplayMessage(SUB_DLG, _T("VisionCalibDlg_6"), &szTextTemp);
	//Live\r\n영상
	GetDlgItem(IDB_LIVE)->SetWindowText(szTextTemp);

	ReadDisplayMessage(SUB_DLG, _T("VisionCalibDlg_7"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);

	ReadDisplayMessage(SUB_DLG, _T("VisionCalibDlg_8"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	ReadDisplayMessage(SUB_DLG, _T("VisionCalibDlg_12"), &szTextTemp);
	//고정좌표 얻기(Tab PreAlign Camera)
	GetDlgItem(IDB_FIX_COORD_TABCARRIER)->SetWindowText(szTextTemp);
	
//	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVisionCalibDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionCalibDlg)
	ON_EVENT(CVisionCalibDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_CAMERA_NUM, -600 /* Click */, OnCameraNum, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_X_MOVE_POINT, -600 /* Click */, OnXMovePoint, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_X_MOVE_WIDTH, -600 /* Click */, OnXMoveWidth, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_Y_MOVE_POINT, -600 /* Click */, OnYMovePoint, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_Y_MOVE_WIDTH, -600 /* Click */, OnYMoveWidth, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_EXECUTE_CALIB, -600 /* Click */, OnExecuteCalib, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_SHOW_MARK, -600 /* Click */, OnShowMark, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_DELETE_MARK, -600 /* Click */, OnDeleteMark, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_MAKE_MARK, -600 /* Click */, OnMakeMark, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_OPTIONS, -600 /* Click */, OnOptions, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_TEST, -600 /* Click */, OnTest, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_LIVE, -600 /* Click */, OnLive, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_FIX_COORD, -600 /* Click */, OnFixCoord, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_CAMERA_DIST2, -600 /* Click */, OnCameraDist2, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_CAMERA_DIST3, -600 /* Click */, OnCameraDist3, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_CAMERA_DIST4, -600 /* Click */, OnClickCameraDist4, VTS_NONE)
	ON_EVENT(CVisionCalibDlg, IDB_FIX_COORD_TABCARRIER, -600 /* Click */, OnClickFixCoordTabcarrier, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVisionCalibDlg::OnExit() 
{
	m_bOnStillContinue = FALSE;
	haltImage(m_iTargetCamNo);

	g_iDisplayReferenceLine = FALSE;
	
	m_plnkVision->DestroyView(m_iViewID);
	
	MOlbSystemPre.LightDefult(DEF_MAX_CAMERA_NO); //171004 JSH
	CDialog::OnCancel();
}

void CVisionCalibDlg::OnSave() 
{
	// 저장하시겠습니까?
	//if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Question", M_ICONQUESTION|M_YESNO) == IDNO)
	if (MyMessageBox(MY_MESSAGE_BOX,63500, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	// Calibration Porting Factor 저장하기
	for (int i = 0; i < DEF_VISION_CALIBRATION_UNIT_PER_CAM; i++)
	{
//		m_plnkCalibration->SetPortingFactor(m_iTargetCamNo, m_rgdPortingFactor[i], m_iCalibUnit);
		m_plnkCalibration->SetPortingFactor(m_iTargetCamNo, m_rgdPortingFactor[i], i);
	}

	// Calibration 파라미터 저장하기
	m_plnkCalibration->SetCalibrationData(m_iTargetCamNo, &m_sCalibData);
}

void CVisionCalibDlg::displayLabel()
{
	CString strTemp;
	// Camera Num
	strTemp.Format(_T("Camera %d"), m_iTargetCamNo+1);
	m_ctrlLblCameraNum.SetCaption(strTemp);

	// X Move Point
	m_sCalibData.m_iMatrixRowCount = 3;
	m_ctrlLblMovePointX.SetCaption(_T("3"));	// 이동 위치는 3 으로 Fix!

	// X Move Width
	strTemp.Format(_T("%.1f"), m_sCalibData.m_dMoveWidth_X);
	m_ctrlLblMoveWidthX.SetCaption(strTemp);

	// Y Move Point
	m_sCalibData.m_iMatrixColCount = 3;
	m_ctrlLblMovePointY.SetCaption(_T("3"));	// 이동 위치는 3 으로 Fix!

	// Y Move Width
	strTemp.Format(_T("%.1f"), m_sCalibData.m_dMoveWidth_Y);
	m_ctrlLblMoveWidthY.SetCaption(strTemp);
}

void CVisionCalibDlg::setWorkingUnit(int iCamNo)
{
	m_iTargetCamNo = iCamNo;
	m_iSelectedMarkNo = DEF_CALIBRATION_MARK1;
	m_plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
	setWorkGroup();

//	[Panel Aligner로 이동하며 InspectionCarrier Camera Calibration]	[TabMounter/Gantry로 이동하며 InspectionCarrier Camera Calibration]
	if (DEF_CALIBRATION_2D == m_rgsType[m_iTargetCamNo].m_iType	|| DEF_CALIBRATION_GANTRY_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		switch (m_iTargetCamNo)
		{
		case DEF_INSPECTION_CAMERA_1:
		case DEF_INSPECTION_CAMERA_2:
			m_plnkCamCarrier = MOlbSystemPre.GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
			break;
		case DEF_INSPECTION_CAMERA_3:
		case DEF_INSPECTION_CAMERA_4:
			m_plnkCamCarrier = MOlbSystemPre.GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_2);
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CAMERA_5:
		case DEF_INSPECTION_CAMERA_6:
			m_plnkCamCarrier = MOlbSystemPre.GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_3);
			break;
		case DEF_INSPECTION_CAMERA_7:
		case DEF_INSPECTION_CAMERA_8:
			m_plnkCamCarrier = MOlbSystemPre.GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_4);
			break;
#endif

		case DEF_R_INSPECTION_CAMERA_1:
		case DEF_R_INSPECTION_CAMERA_2:
			m_plnkCamCarrier = MOlbSystemPre.GetRCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
			break;
		case DEF_R_INSPECTION_CAMERA_3:
		case DEF_R_INSPECTION_CAMERA_4:
			m_plnkCamCarrier = MOlbSystemPre.GetRCameraCarrierComponent(DEF_INSPECTION_CARRIER_2);
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_R_INSPECTION_CAMERA_5:
		case DEF_R_INSPECTION_CAMERA_6:
			m_plnkCamCarrier = MOlbSystemPre.GetRCameraCarrierComponent(DEF_INSPECTION_CARRIER_3);
			break;
		case DEF_R_INSPECTION_CAMERA_7:
		case DEF_R_INSPECTION_CAMERA_8:
			m_plnkCamCarrier = MOlbSystemPre.GetRCameraCarrierComponent(DEF_INSPECTION_CARRIER_4);
			break;
#endif
		default:
			break;
		}

		if (DEF_CALIBRATION_GANTRY_2D == m_rgsType[m_iTargetCamNo].m_iType)
		{
			switch (m_iTargetCamNo)
			{
			case DEF_INSPECTION_CAMERA_1:
			case DEF_INSPECTION_CAMERA_2:
			case DEF_INSPECTION_CAMERA_3:
			case DEF_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
			case DEF_INSPECTION_CAMERA_5:
			case DEF_INSPECTION_CAMERA_6:
			case DEF_INSPECTION_CAMERA_7:
			case DEF_INSPECTION_CAMERA_8:
#endif

#ifdef DEF_GATE_SYSTEM
				m_plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER2);
#else
				m_plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER4);
#endif
				break;
			case DEF_R_INSPECTION_CAMERA_1:
			case DEF_R_INSPECTION_CAMERA_2:
			case DEF_R_INSPECTION_CAMERA_3:
			case DEF_R_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
			case DEF_R_INSPECTION_CAMERA_5:
			case DEF_R_INSPECTION_CAMERA_6:
			case DEF_R_INSPECTION_CAMERA_7:
			case DEF_R_INSPECTION_CAMERA_8:
#endif

#ifdef DEF_GATE_SYSTEM
				m_plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(DEF_TABMOUNTER2);
#else
				m_plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(DEF_TABMOUNTER4);
#endif
				break;
			}
		}
//@		GetDlgItem(IDB_FIX_COORD_TABCARRIER)->EnableWindow(FALSE);
	}
	//TabCarrier로 이동하며 TabCarrier Camera Calibration
	else if (DEF_CALIBRATION_TABCARRIER_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		switch (m_iTargetCamNo)
		{
		case DEF_F_TABALIGNER_CAMERA_1:
		case DEF_F_TABALIGNER_CAMERA_2:
		case DEF_F_TABALIGNER_CAMERA_3: //SJ_YYK 150817 Add..
		case DEF_F_TABALIGNER_CAMERA_4: //SJ_YYK 150817 Add..
			m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(m_iInstanceID);
			break;
		case DEF_R_TABALIGNER_CAMERA_1:
		case DEF_R_TABALIGNER_CAMERA_2:
		case DEF_R_TABALIGNER_CAMERA_3: //SJ_YYK 150817 Add..
		case DEF_R_TABALIGNER_CAMERA_4:	//SJ_YYK 150817 Add..
			m_plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(m_iInstanceID);
			break;
		default:
			break;
		}
//@		GetDlgItem(IDB_FIX_COORD_TABCARRIER)->EnableWindow(TRUE);
	}
	else
	{
		ASSERT(0);
	}

	m_sCalibData = m_plnkCalibration->GetCalibrationData(m_iTargetCamNo);
	m_sCalibData.m_iMatrixRowCount = 3;
	m_sCalibData.m_iMatrixColCount = 3;
	
	for (int j = 0; j < DEF_VISION_CALIBRATION_UNIT_PER_CAM; j++)
	{
//KKY.Del		if (1 == j && FALSE == m_rgsType[m_iTargetCamNo].m_bUsePrism)
//KKY.Del			continue;

		for (int i = 0 ; i < DEF_FACTOR_NUMBER ; i++)
		{
			m_rgdPortingFactor[j][i] = m_plnkCalibration->GetPortingFactor(m_iTargetCamNo, j)[i];
		}
	}
}

void CVisionCalibDlg::OnCameraNum() 
{
#ifdef SIMULATION	
	// for test
	double VX, VY;
	double RX,RY;
	VX = 200.0;
	VY = 150.0;
	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, 0, VX, VY, RX, RY);

	TRACE2("Rx, Ry = %.3lf, %.3lf\n", RX, RY);

	m_plnkCalibration->ConvertR2V(DEF_PANEL_ALIGN_CAMERA_1, 0, RX, RY, VX, VY);

	TRACE2("Vx, Vy = %.3lf, %.3lf\n", VX, VY);

	return;

#endif

	CString strTemp;
	CString strPrev;

	strPrev.Format(_T("%d"), m_iTargetCamNo + 1);

	if (!GetNumberBox(strTemp.GetBuffer(10), 10, strPrev.GetBuffer(10))) 
		return;	

	int iCamNo = _wtoi(strTemp) - 1;

	if (iCamNo < 0 || iCamNo >= DEF_CAMERA_19)
	{
		// Camera 번호 선택 오류
		MyMessageBox(MY_MESSAGE_BOX,63501, _T("Error"), M_ICONERROR);
		return;
	}

	if (m_iTargetCamNo == iCamNo)
		return;

	m_bOnStillContinue = FALSE;
	Sleep(100);
	haltImage(m_iTargetCamNo);

	setWorkingUnit(iCamNo);

	OnLive();
	m_bOnStillContinue = TRUE;
	displayLabel();
}

void CVisionCalibDlg::OnXMovePoint()
{
/* 삭제 예정.
	CString strTemp;
	CString strPrev;

	strPrev.Format("%d", m_sCalibData.m_iMatrixRowCount);

	if (!GetNumberBox(strTemp.GetBuffer(10), 10, strPrev.GetBuffer(10))) 
		return;	

	m_sCalibData.m_iMatrixRowCount = atoi(strTemp);
	displayLabel();
*/
}

void CVisionCalibDlg::OnXMoveWidth() 
{
	CString strTemp;
	CString strPrev;

	strPrev.Format(_T("%.1f"), m_sCalibData.m_dMoveWidth_X);

	if (!GetNumberBox(strTemp.GetBuffer(10), 10, strPrev.GetBuffer(10))) 
		return;	

	m_sCalibData.m_dMoveWidth_X = wcstod(strTemp.GetBuffer(strTemp.GetLength()), NULL);
	displayLabel();
}

void CVisionCalibDlg::OnYMovePoint() 
{
	/* 삭제 예정.
	CString strTemp;
	CString strPrev;

	strPrev.Format("%d", m_sCalibData.m_iMatrixColCount);

	if (!GetNumberBox(strTemp.GetBuffer(10), 10, strPrev.GetBuffer(10))) 
		return;	

	m_sCalibData.m_iMatrixColCount = atoi(strTemp);
	displayLabel();
	*/
}

void CVisionCalibDlg::OnYMoveWidth() 
{
	CString strTemp;
	CString strPrev;

	strPrev.Format(_T("%.1f"), m_sCalibData.m_dMoveWidth_Y);

	if (!GetNumberBox(strTemp.GetBuffer(10), 10, strPrev.GetBuffer(10))) 
		return;	

	m_sCalibData.m_dMoveWidth_Y = wcstod(strTemp.GetBuffer(strTemp.GetLength()), NULL);
	displayLabel();
}

void CVisionCalibDlg::OnExecuteCalib() 
{
	CString strMsg;
	// Vision Calibration을 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 63502, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = SUCCESS;

	// Panel Aligner 로 Calibration 을 수행할 경우, backlash 보정 동작을 미리 수행하고 Calibration Mark 등록을 해야 한다.
	if (DEF_CALIBRATION_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		// 현재 위치 기준으로 Backlash 보정 동작 수행
		// 설비 프로그램에서 backlash 보정 동작 방향 통일 필요...
		// 보정량 - system data

		// 확인 필요 : Prism 일 경우, 기준선 위치 결정해야 함.

		// Calibration Mark 재등록 요청.

		//Backlash 보정 동작 후 Calibration Mark 를 기준 위치에 맞게 등록했는지 확인하세요.\r\nCalibration 을 계속 진행하겠습니까?
		iResult = MyMessageBox(MY_MESSAGE_BOX, 63503, _T("Question"), M_ICONQUESTION|M_YESNO);
		if (IDNO == iResult)
			return;
	}
	else if (DEF_CALIBRATION_GANTRY_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		//InspectionCamera 기준위치에 TabMount4번을 위치했는지 확인하세요. Calibration을 계속 진행하겠습니까?
		iResult = MyMessageBox(MY_MESSAGE_BOX, 63541, _T("Question"), M_ICONQUESTION|M_YESNO);
		if (IDNO == iResult)
			return;
	}
	else if (DEF_CALIBRATION_TABCARRIER_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		//TabCarrierCamera 기준위치에 TabCarrier를 위치했는지 확인하세요. Calibration을 계속 진행하겠습니까?
		iResult = MyMessageBox(MY_MESSAGE_BOX, 63542, _T("Question"), M_ICONQUESTION|M_YESNO);
		if (IDNO == iResult)
			return;
	}

	iResult = setupMovePositions();
	if(ERR_VCALIB_SUCCESS != iResult)
	{
		//Calibration 동작에 이용할 좌표 생성에 실패 했습니다.
		MyMessageBox(MY_MESSAGE_BOX,63506, _T("Calibration Error"), M_ICONERROR);
		return;
	}

	int iPrismSideNo = DEF_PRISM_SIDE_ALL;//필요없는 인자. 나중 삭제요...
	iResult = executeCalibrationMoves(iPrismSideNo);
	if(ERR_VCALIB_SUCCESS != iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Calibration Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	if (DEF_CALIBRATION_GANTRY_REVERSE_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		double dY = 0.0;

		dY = m_rgxyOffsetPos[0].dY;
		m_rgxyOffsetPos[0].dY = m_rgxyOffsetPos[6].dY;
		m_rgxyOffsetPos[6].dY = dY;

		dY = m_rgxyOffsetPos[1].dY;
		m_rgxyOffsetPos[1].dY = m_rgxyOffsetPos[7].dY;
		m_rgxyOffsetPos[7].dY = dY;

		dY = m_rgxyOffsetPos[2].dY;
		m_rgxyOffsetPos[2].dY = m_rgxyOffsetPos[8].dY;
		m_rgxyOffsetPos[8].dY = dY;
	}

	iResult = m_plnkCalibration->Calculate2DCalibration(m_iTargetCamNo, m_iCalibUnit, m_rgxyPixelPos[m_iCalibUnit], m_rgxyOffsetPos, m_rgdPortingFactor[m_iCalibUnit]);
	if (iResult != SUCCESS)
	{
		liveImage(m_iTargetCamNo);
		
		CString strTemp;
		strTemp.Format(_T(" %s"), MOlbSystemPre.GetErrorMessage(iResult));
		//Vision Calibration 을 실패하였습니다.
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63507"), &strMsg);
		strMsg += strTemp;
		MyMessageBox(strMsg, _T("Calibration Error"), M_ICONERROR);
		return;
	}

	m_plnkCalibration->SetCalibrationData(m_iTargetCamNo, &m_sCalibData);

	CString strTitle;
	strTitle.Format(_T("Camera %d : Calibratioin Result 1"), m_iTargetCamNo + 1);
	m_listResult.DeleteString(0);
	m_listResult.InsertString(0, strTitle);

	CString strResult;

	strResult.Format(_T("[F0:%2.5f][F1:%2.5f][F2:%2.5f]"), m_rgdPortingFactor[m_iCalibUnit][0], m_rgdPortingFactor[m_iCalibUnit][1], m_rgdPortingFactor[m_iCalibUnit][2]);
	m_listResult.DeleteString(1);
	m_listResult.InsertString(1, strResult);
	
	strResult.Format(_T("[F3:%2.5f][F4:%2.5f][F5:%2.5f]"),m_rgdPortingFactor[m_iCalibUnit][3], m_rgdPortingFactor[m_iCalibUnit][4], m_rgdPortingFactor[m_iCalibUnit][5]);
	m_listResult.DeleteString(2);
	m_listResult.InsertString(2, strResult);
	
	strResult.Format(_T("[F6:%2.5f][F7:%2.5f][F8:%2.5f]"),m_rgdPortingFactor[m_iCalibUnit][6], m_rgdPortingFactor[m_iCalibUnit][7], m_rgdPortingFactor[m_iCalibUnit][8]);
	m_listResult.DeleteString(3);
	m_listResult.InsertString(3, strResult);

	if (MyMessageBox(_T("Data Save? 적용 하시겠습니까?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_sCalibData.m_Complete_Flag = TRUE;

	m_plnkCalibration->SetPortingFactor(m_iTargetCamNo, m_rgdPortingFactor[m_iCalibUnit], m_iCalibUnit);

	OnLive();
	m_bOnStillContinue = TRUE;
	//Vision Calibration이 완료되었습니다.
	MyMessageBox(MY_MESSAGE_BOX,63508, _T("Manage Model"), M_ICONERROR);
}

void CVisionCalibDlg::OnShowMark()
{
	SMarkInfo sMarkInfo = { m_iTargetCamNo, DEF_PM_MARK, &m_iSelectedMarkNo, };

	CVisionSelectMarkDlg dlg(m_plnkVision, DEF_SHOW_MARK, &sMarkInfo);
	if (dlg.DoModal() == IDCANCEL)
		return ;

	stillImage(m_iTargetCamNo);

	m_plnkVision->DisplayPatternImage(m_iTargetCamNo, DEF_PM_MARK, m_iSelectedMarkNo);
}

void CVisionCalibDlg::OnDeleteMark() 
{
	SMarkInfo sMarkInfo = { m_iTargetCamNo, DEF_PM_MARK, &m_iSelectedMarkNo, };

	CVisionSelectMarkDlg dlg(m_plnkVision, DEF_DELETE_MARK, &sMarkInfo);
	if (dlg.DoModal() == IDCANCEL)
		return ;

	// 선택한 Mark 를 삭제하겠습니까?
	UINT uiResult = MyMessageBox(MY_MESSAGE_BOX,36512, _T("Mark Delete"), M_ICONQUESTION|M_YESNO);
	if (uiResult == IDYES)
		m_plnkVision->DeleteMark(m_iTargetCamNo, DEF_PM_MARK, m_iSelectedMarkNo);
}

void CVisionCalibDlg::OnMakeMark()
{
	stillImage(m_iTargetCamNo);

	SMarkInfo sMarkInfo = { m_iTargetCamNo, DEF_PM_MARK, &m_iSelectedMarkNo, };

	CVisionSelectMarkDlg dlg(m_plnkVision, DEF_MAKE_MARK, &sMarkInfo);
	if (dlg.DoModal() == IDCANCEL)
		return ;

#ifdef M_COLOR_SIMULATION
	return ;
#endif	/* M_COLOR_SIMULATION */

	CPoint cpImageSize = m_plnkVision->GetDistribution(m_iTargetCamNo);

	// Model 등록 작업
	CRect rectLimit;
	rectLimit.SetRect(3, 3, cpImageSize.x - 3, cpImageSize.y - 3);
	
	CRect rectSearch;
	rectSearch.SetRect(3, 3, cpImageSize.x - 3, cpImageSize.y - 3);
	rectSearch.OffsetRect(3, 3);
	
	CRect rectModel;
	rectModel.SetRect(0, 0, cpImageSize.x / 4 , cpImageSize.y / 4);
	rectModel.OffsetRect(cpImageSize.x * 3 / 8, cpImageSize.y * 3 / 8);
	
	CString strMsg;
	
	// Search Area 설정

	//Search Area 를 설정하세요.
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63509"), &strMsg);
	CVisionJogKeyDlg jogSearchAreaDlg(m_plnkVision, m_iTargetCamNo, strMsg, rectLimit, &rectSearch, this);
	if (jogSearchAreaDlg.DoModal() == IDCANCEL)
	{
		m_plnkVision->ClearOverlay(m_iTargetCamNo);
		return ;
	}
	
	// Model Area 설정

	//Model Area 를 설정하세요.
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63510"), &strMsg);
	CVisionJogKeyDlg jogModelAreaDlg(m_plnkVision, m_iTargetCamNo, strMsg, rectSearch, &rectModel, this);
	if (jogModelAreaDlg.DoModal() == IDCANCEL)
	{
		m_plnkVision->ClearOverlay(m_iTargetCamNo);
		return ;
	}
	
	// Reference Point 설정

	//Reference Point 를 설정하세요.
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63511"), &strMsg);
	CPoint pointReference = rectModel.CenterPoint();
//	CVisionJogKeyDlg jogRefPointDlg(m_plnkVision, m_iTargetCamNo, strMsg, rectSearch, &pointReference, this);
	CVisionJogKeyDlg jogRefPointDlg(m_plnkVision, m_iTargetCamNo, strMsg, rectSearch, &pointReference, DEF_POINT, this);
	if (jogRefPointDlg.DoModal() == IDCANCEL)
	{
		m_plnkVision->ClearOverlay(m_iTargetCamNo);
		return ;
	}
	
	int iResult = m_plnkVision->RegisterPatternMark(m_iTargetCamNo, m_iSelectedMarkNo, rectSearch, rectModel, pointReference.x, pointReference.y);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	CString strName(sMarkInfo.m_szName); // 2009.02.11 CYJ
	m_plnkVision->SetModelName(m_iTargetCamNo, DEF_PM_MARK, m_iSelectedMarkNo, strName.GetBuffer(strName.GetLength()));

	//현재 모델을 저장하겠습니까?
	iResult = MyMessageBox(MY_MESSAGE_BOX,63513, _T("Vision Model Save"), M_ICONQUESTION |M_YESNO);
	if (iResult == IDYES)
		m_plnkVision->WriteModelData(m_iTargetCamNo, DEF_PM_MARK, m_iSelectedMarkNo);	
}

void CVisionCalibDlg::OnOptions()
{
	CVisionOptionDlg dlg(m_plnkVision, DEF_PM_MARK, m_iTargetCamNo, m_iSelectedMarkNo, this);
	dlg.DoModal();
}

void CVisionCalibDlg::OnTest() 
{
	stillImage(m_iTargetCamNo);

	int iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo, m_iSelectedMarkNo);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Vision Monitor Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	double dVisionX = m_plnkVision->GetSearchResultX(m_iTargetCamNo, m_iSelectedMarkNo);
	double dVisionY = m_plnkVision->GetSearchResultY(m_iTargetCamNo, m_iSelectedMarkNo);
	
	int iCalibUnitNo = MOlbSystemPre.GetCalibrationUnitNo(m_iTargetCamNo, m_iInstanceID);
	if (1 == m_plnkCalibration->GetPortingFactor(m_iTargetCamNo, iCalibUnitNo)[8])
	{
		double dRobotX = 0.0;
		double dRobotY = 0.0;
		
		m_plnkCalibration->ConvertV2R(m_iTargetCamNo, iCalibUnitNo, dVisionX, dVisionY, dRobotX, dRobotY);
		
		char szResult[256];
		memset(szResult, 0, sizeof(szResult));
		sprintf(szResult, "(x,y)=(%.3lf,%.3lf)", dRobotX, dRobotY);
		m_plnkVision->DrawOverlayText(m_iTargetCamNo, szResult);
	}
}

void CVisionCalibDlg::OnLive() 
{
	liveImage(m_iTargetCamNo);
	m_bOnStillContinue = TRUE;
}

void CVisionCalibDlg::OnFixCoord()
{
	int iResult = 0;
	if (NULL == m_plnkPanelAligner)
		m_plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();

	/* [사전 작업]
	- Inspection Camera Carrier F/R 1번을 -방향으로 최대한 이동시킨다.
	- Panel중심과 Stage중심을 일치하게 Panel을 올려 놓고 흡착한다.
	- ModelChange축을 이동시켜 Panel Mark가 Camera중심에 오도록 위치시킨다.
	*/

	/*
	if (TRUE == m_plnkPanelAligner->IsPanelAbsorbed())
	{
		// PanelAligner 가 Panel 을 흡착하고 있습니다.
		MyMessageBox(MY_MESSAGE_BOX,63514, _T("Pre-condition Check"), M_ICONERROR);
		return;
	}

	if (FALSE == m_plnkPanelAligner->IsInZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS))
	{
		// PanelAligner 가 Down 상태가 아닙니다.
		MyMessageBox(MY_MESSAGE_BOX,63515, _T("Pre-condition Check"), M_ICONERROR);
		return;
	}

	if (FALSE == m_plnkPanelAligner->IsAirCVVacAllOn())
	{
		// Panel이 부상유닛에 흡착되어 있지 않습니다.
		//MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Pre-condition Check", M_ICONERROR);
		MyMessageBox(MY_MESSAGE_BOX,63516, _T("Pre-condition Check"), M_ICONERROR);
		return;
	}
	*/

	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (FALSE == m_plnkPanelAligner->IsAirCVVacAllOn())
	{
		iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_WORK_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = m_plnkPanelAligner->AirCVVacAllOn();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
#else
	//부상 가운데 있는 Rail2개만 흡착하고 나머지는 Blow시킴
	if (FALSE == m_plnkPanelAligner->IsAirCVVacPlate2On()
		|| FALSE == m_plnkPanelAligner->IsAirCVVacPlate3On())
	{
		iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_WORK_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = m_plnkPanelAligner->AirCVVacPlate2On();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = m_plnkPanelAligner->AirCVVacPlate3On();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
	m_plnkPanelAligner->AirCVBlowOn1();
	m_plnkPanelAligner->AirCVBlowOn4();
	Sleep(500);
#endif
	/*/
	iResult = m_plnkCamCarrier->CheckOriginExpand();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	if (FALSE == m_plnkCamCarrier->IsInPosExpand(DEF_CAMERACARRIER_EXPAND_WORKING_POS))
	{
		MyMessageBox(_T("CamCarrier is not in working position. Go to the working location and retry.\r\nCamCarrier가 Working위치에 있지 않습니다. Working위치로 이동후 재시도 하세요"), _T("Confirm"));
		return;
	}
	double dRefPos = m_plnkCamCarrier->GetTargetPosExpand(DEF_CAMERACARRIER_EXPAND_WORKING_POS);
	m_plnkSystemData->m_dInspectCamFixedPosWidth = dRefPos;
	
	iResult = m_plnkPanelAligner->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = m_plnkModelChange->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = m_plnkModelChange2->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	int iPanelWorkNo = m_plnkPanelAligner->GetPanelWorkNoFromCurPos();
//@	if (FALSE == m_plnkPanelAligner->IsAirCVVacAllOn())
	if (FALSE == m_plnkPanelAligner->IsAirCVVacAllOn(DEF_NONE_WORK_SCHEDULE))
	{
		iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_WORK_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = m_plnkPanelAligner->AirCVVacAllOn(DEF_NONE_WORK_SCHEDULE);//@iPanelWorkNo);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}

	iResult = m_plnkPanelAligner->ReleasePanel();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	Sleep(200);
	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//___________________________________________

	/*/
	MPos_XYT posOrigin(0.0, 0.0, 0.0);
	iResult = m_plnkPanelAligner->SyncMoveXYTPos(posOrigin);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	/*/

	MCameraCarrier *pCamCarrier1 = NULL, *pCamCarrier2 = NULL;

	pCamCarrier1 = MOlbSystemPre.GetCameraCarrierComponent(DEF_PANEL_ALIGN_CAMERA_CARRIER1);
	pCamCarrier2 = MOlbSystemPre.GetRCameraCarrierComponent(DEF_PANEL_ALIGN_CAMERA_CARRIER2);
	
	iResult = pCamCarrier1->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = pCamCarrier2->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = pCamCarrier1->Expand();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = pCamCarrier2->Expand();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	// 확인 버튼을 누르면 작업을 진행합니다.
	MyMessageBox(MY_MESSAGE_BOX,63520, _T("Confirm"), M_ICONINFORMATION);

	int i = 0;
	//Camera간 거리를 Panel Mark간 거리와  일치시킨다______
	// Panel Align Camera 1
	for (int i=0; i<10; i++)
	{
		//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
		stillImage(DEF_PANEL_ALIGN_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
#else
		stillImage(DEF_INSPECTION_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif		
		//____________
		if (iResult == SUCCESS)
			break;
		Sleep(100);
	}
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	double dPreVisionX1 = m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dPreVisionY1 = m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
#else
	double dPreVisionX1 = m_plnkVision->GetSearchResultX(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dPreVisionY1 = m_plnkVision->GetSearchResultY(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif
	//______________
	double dPreRobotX1 = 0.0;
	double dPreRobotY1 = 0.0;
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, 0, dPreVisionX1, dPreVisionY1, dPreRobotX1, dPreRobotY1);
#else
	m_plnkCalibration->ConvertV2R(DEF_INSPECTION_CAMERA_1, 0, dPreVisionX1, dPreVisionY1, dPreRobotX1, dPreRobotY1);
#endif
	//_____________

	// Panel Align Camera 2
	for (int i=0; i<10; i++)
	{	
		//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
		stillImage(DEF_PANEL_ALIGN_CAMERA_2);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
#else
		stillImage(DEF_R_INSPECTION_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif
		//_______________
		if (iResult == SUCCESS)
			break;
		Sleep(100);
	}
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	double dPreVisionX2 = m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
	double dPreVisionY2 = m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
#else
	double dPreVisionX2 = m_plnkVision->GetSearchResultX(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dPreVisionY2 = m_plnkVision->GetSearchResultY(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif
	//_____________
	double dPreRobotX2 = 0.0;
	double dPreRobotY2 = 0.0;
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_2, 0, dPreVisionX2, dPreVisionY2, dPreRobotX2, dPreRobotY2);
#else
	m_plnkCalibration->ConvertV2R(DEF_R_INSPECTION_CAMERA_1, 0, dPreVisionX2, dPreVisionY2, dPreRobotX2, dPreRobotY2);
#endif
	//____________

#ifdef DEF_GATE_SYSTEM
	double dMarkDist = m_plnkPanelData->m_dDistance_FiduMark_Xu;
#else
	double dMarkDist = m_plnkPanelData->m_dDistance_FiduMark_Yl;
#endif
	double RX = dPreRobotX2 - dPreRobotX1;
	double RY = dPreRobotY2 - dPreRobotY1;
	double dY = sqrt(dMarkDist*dMarkDist - RY*RY) - RY - dMarkDist;

	iResult = m_plnkPanelAligner->UpPickUpUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelAligner->AbsorbPanel();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("PanelAligner Teaching Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelAligner->AirCVBlowOn();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("PanelAligner Teaching Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	Sleep(1000);

	//ModelChange를 이동시켜 Panel Align Mark간 거리와 일치시킨다.
	double dTarget = MOlbSystemPre.GetModelChangeComponent(DEF_REAR_GROUP)->GetYCurrentPos() + dY;
	iResult = MOlbSystemPre.GetModelChangeComponent(DEF_REAR_GROUP)->MoveYPos(dTarget, DEF_MODEL_CHANGE_MODEL_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	//현재 Panel Aligner의 좌표를 적용하여 준다
	MPos_XYT curPos = m_plnkPanelAligner->GetXYTCurrentPos();

	Sleep(100);
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	stillImage(DEF_PANEL_ALIGN_CAMERA_2);
#else
	stillImage(DEF_R_INSPECTION_CAMERA_1);
#endif
	//______________
	Sleep(1000);
	
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];

	swprintf(cBeforeVal, _T("0"));
	
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal, _T("Y2 축 이동량 설정 [0.1 ~ 1.5 mm]")))
		return;	
	
	double dY2Move = wcstod(cAfterVal, NULL);

	if (dY2Move < 0.1 || dY2Move > 1.5)
	{
		MyMessageBox(_T("The input value is removed from the limit (0.1 to 1.5).\r\n입력값이 Limit(0.1 ~ 1.5)를 벗어낫습니다."), _T("Input Error"));
		return;
	}

	// 이동 전 Vision 위치 인식.
	for (int i=0; i<10; i++)
	{
		//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
		stillImage(DEF_PANEL_ALIGN_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
#else
		stillImage(DEF_INSPECTION_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
		//_____________
#endif
		if (iResult == SUCCESS)
			break;
		Sleep(100);
	}
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	double dVisionXl1 = m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dVisionYl1 = m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
#else
	double dVisionXl1 = m_plnkVision->GetSearchResultX(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dVisionYl1 = m_plnkVision->GetSearchResultY(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif
	//______________
	double dRobotXl1 = 0.0;
	double dRobotYl1 = 0.0;
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, 0, dVisionXl1, dVisionYl1, dRobotXl1, dRobotYl1);
#else
	m_plnkCalibration->ConvertV2R(DEF_INSPECTION_CAMERA_1, 0, dVisionXl1, dVisionYl1, dRobotXl1, dRobotYl1);
#endif
	//____________

	// Panel Align Camera 2
	for (int i=0; i<10; i++)
	{
		//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
		stillImage(DEF_PANEL_ALIGN_CAMERA_2);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
#else
		stillImage(DEF_R_INSPECTION_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif
		//________________
		if (iResult == SUCCESS)
			break;
		Sleep(100);
	}
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	double dVisionXr1 = m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
	double dVisionYr1 = m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
#else
	double dVisionXr1 = m_plnkVision->GetSearchResultX(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dVisionYr1 = m_plnkVision->GetSearchResultY(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
	//______________
#endif
	double dRobotXr1 = 0.0;
	double dRobotYr1 = 0.0;
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_2, 0, dVisionXr1, dVisionYr1, dRobotXr1, dRobotYr1);
#else
	m_plnkCalibration->ConvertV2R(DEF_R_INSPECTION_CAMERA_1, 0, dVisionXr1, dVisionYr1, dRobotXr1, dRobotYr1);
#endif
	//_____________

	// 이동 후 Vision 위치 인식
	// Y2 축 이동. (이동량은 각도가 아닌 수평 이동량임.)
	iResult = m_plnkPanelAligner->GetMotionComponent(DEF_T)->RMove(dY2Move);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	Sleep(100);

	for (int i=0; i<10; i++)
	{	
		//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
		stillImage(DEF_PANEL_ALIGN_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
#else
		stillImage(DEF_INSPECTION_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif
		//______________
		if (iResult == SUCCESS)
			break;
		Sleep(100);
	}
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, "PanelAligner Teaching Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	double dVisionXl2 = m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dVisionYl2 = m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIBRATION_MARK1);
#else
	double dVisionXl2 = m_plnkVision->GetSearchResultX(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dVisionYl2 = m_plnkVision->GetSearchResultY(DEF_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif
	//______________
	double dRobotXl2 = 0.0;
	double dRobotYl2 = 0.0;
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, 0, dVisionXl2, dVisionYl2, dRobotXl2, dRobotYl2);
#else
	m_plnkCalibration->ConvertV2R(DEF_INSPECTION_CAMERA_1, 0, dVisionXl2, dVisionYl2, dRobotXl2, dRobotYl2);
#endif
	//_____________

	for (int i=0; i<10; i++)
	{
		//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
		stillImage(DEF_PANEL_ALIGN_CAMERA_2);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
#else
		stillImage(DEF_R_INSPECTION_CAMERA_1);
		iResult = m_plnkVision->RecognitionPatternMark(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
#endif
		//________________
		if (iResult == SUCCESS)
			break;
		Sleep(100);
	}
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	double dVisionXr2 = m_plnkVision->GetSearchResultX(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
	double dVisionYr2 = m_plnkVision->GetSearchResultY(DEF_PANEL_ALIGN_CAMERA_2, DEF_CALIBRATION_MARK1);
#else
	double dVisionXr2 = m_plnkVision->GetSearchResultX(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
	double dVisionYr2 = m_plnkVision->GetSearchResultY(DEF_R_INSPECTION_CAMERA_1, DEF_CALIBRATION_MARK1);
	//_____________________
#endif
	double dRobotXr2 = 0.0;
	double dRobotYr2 = 0.0;
	//150323 SJ_YSH
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Modify..
	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_2, 0, dVisionXr2, dVisionYr2, dRobotXr2, dRobotYr2);
#else
	m_plnkCalibration->ConvertV2R(DEF_R_INSPECTION_CAMERA_1, 0, dVisionXr2, dVisionYr2, dRobotXr2, dRobotYr2);
#endif
	//______________

	//Panel Aligner 회전 중심에서 Camera2번 중심까지 거리를 구함
	double dCamCarrier1OrgDistX;
	double dCamCarrier2OrgDistX;
	double dOrgDistX_To_Cam2;
	double dOrgDistY_To_Cam2;
	double dt_rad = asin((dRobotXr2-dRobotXl2)/dMarkDist) - asin((dRobotXr1-dRobotXl1)/dMarkDist);
	dOrgDistY_To_Cam2 = 1/(2*(1-cos(dt_rad))) * ( (cos(dt_rad)-1)*(dRobotYr1+dRobotYr2) + sin(dt_rad)*(dRobotXr2-dRobotXr1) );
	dOrgDistX_To_Cam2 = 1/(2*(1-cos(dt_rad))) * ( (cos(dt_rad)-1)*(dRobotXr1-dRobotXr2) + sin(dt_rad)*(dRobotYr1-dRobotYr2) );
	
	//현재 Panel Aligner의 좌표를 적용하여 준다
	dCamCarrier1OrgDistX = dOrgDistX_To_Cam2;
	dCamCarrier2OrgDistX = dOrgDistX_To_Cam2;

	dOrgDistY_To_Cam2 += curPos.dY;
	dOrgDistX_To_Cam2 += curPos.dX;

	double dOrgDistY_To_Cam1 = dOrgDistY_To_Cam2 - dMarkDist;
	double dOrgDistX_To_Cam1 = dOrgDistX_To_Cam2;

	double dCam1CurPos = pCamCarrier1->GetCurrentPos();
	double dCam2CurPos = pCamCarrier2->GetCurrentPos();
	double dModelChange1CurPos = MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->GetYCurrentPos();
	double dModelChange2CurPos = MOlbSystemPre.GetModelChangeComponent(DEF_REAR_GROUP)->GetYCurrentPos();

	dOrgDistY_To_Cam1 -= dModelChange1CurPos;
	dOrgDistY_To_Cam2 -= dModelChange2CurPos;

	//Panel Aligner 중심간 거리를 구한다.
	double dAlignerDist = dY2Move / sin(dt_rad);		//---> DEF_PANELALIGNER_DIST_CENTERTOCENTER 와 비교

	m_plnkSystemData->m_dAlignCam2_FixedPos_X = dOrgDistX_To_Cam2;
	m_plnkSystemData->m_dAlignCam2_FixedPos_Y = dOrgDistY_To_Cam2;
	m_plnkSystemData->m_dAlignCam1_FixedPos_X = dOrgDistX_To_Cam1;
	m_plnkSystemData->m_dAlignCam1_FixedPos_Y = dOrgDistY_To_Cam1;
//@	m_plnkSystemData->m_dCamCarrier1_FixedPosX = (dCamCarrier1OrgDistX - dCam1CurPos);
//@	m_plnkSystemData->m_dCamCarrier2_FixedPosX = (dCamCarrier2OrgDistX - dCam2CurPos);
	m_plnkSystemData->m_dCamCarrier1_FixedPosX = dCam1CurPos;
	m_plnkSystemData->m_dCamCarrier2_FixedPosX = dCam2CurPos;
	m_plnkSystemData->m_dDistBetweenAligners  = -dAlignerDist;

	m_plnkSystemData->WriteData();

	// Y2 축 원래 위치로 이동. (이동량은 각도가 아닌 수평 이동량임.)
	iResult = m_plnkPanelAligner->GetMotionComponent(DEF_T)->RMove(-dY2Move);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	Sleep(100);
	m_plnkPanelAligner->ChangeModel();
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		MOlbSystemPre.GetCameraCarrierComponent(i)->ChangeModel();
		MOlbSystemPre.GetRCameraCarrierComponent(i)->ChangeModel();
	}
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		MOlbSystemPre.GetTabMounterComponent(i)->ChangeModel();
		MOlbSystemPre.GetRTabMounterComponent(i)->ChangeModel();
	}

	MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->ChangeModel();
	MOlbSystemPre.GetModelChangeComponent(DEF_REAR_GROUP)->ChangeModel();

	//PanelAligner 거리 Data 얻기 작업을 완료했습니다.
	MyMessageBox(MY_MESSAGE_BOX, 63521, _T("Work Completion"), M_ICONINFORMATION);
}

void CVisionCalibDlg::stillImage(int iCamNo)
{
	m_bOnStillContinue = FALSE;
	haltImage(iCamNo);

	m_plnkVision->ClearOverlay(iCamNo);
	int iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
		// jdy sesl		MyMessageBox(iResult, "Vision Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkVision->GrabWait(iCamNo);
	if (iResult)
	{
		// jdy sesl		MyMessageBox(iResult, "Vision Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
}

void CVisionCalibDlg::haltImage(int iCamNo)
{
	if (m_bIsOnLive == FALSE)
		return;
	m_bOnStillContinue = FALSE;
	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->HaltVideo(iCamNo);
	m_bIsOnLive = FALSE;
}

void CVisionCalibDlg::liveImage(int iCamNo)
{
	if (m_bIsOnLive == TRUE)
	{
		if (m_iTargetCamNo == iCamNo)
			return;
		else
			haltImage(m_iTargetCamNo);
	}

	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iCamNo);
	m_bIsOnLive = TRUE;
	m_bOnStillContinue = TRUE;
}

/** 이동할 좌표를 계산한다. 
 *
 * @param	iUnitID : 작업을 수행할 Stage 번호
 * @param	iCamera : Unit의 Camera Index (Vision의 Camera ID가 아님)
 * @return	Error : 0=SUCCESS, 그외=Error Code
 */
int CVisionCalibDlg::setupMovePositions()
{
	double	SignedDeltaX = 0.0;
	double	SignedDeltaY = 0.0;
	double  SignedDeltaT = 0.0;

	MPos_XYT xytPosition;	// 모션의 좌표를 저장한다.

	//[Panel Aligner로 이동하며 InspectionCarrier Camera Calibration]
	if (DEF_CALIBRATION_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		ASSERT(m_plnkPanelAligner != NULL);
		xytPosition = m_plnkPanelAligner->GetXYTCurrentPos();
		
		for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
		{
			if (i % DEF_CALIBRATION_MOVE_POINT == 0)
				SignedDeltaX = -m_sCalibData.m_dMoveWidth_X;
			else if (i % DEF_CALIBRATION_MOVE_POINT == 1)
				SignedDeltaX = 0.0;
			else
				SignedDeltaX = m_sCalibData.m_dMoveWidth_X;
			
			if (i < DEF_CALIBRATION_MOVE_POINT)
				SignedDeltaY = -m_sCalibData.m_dMoveWidth_Y;
			else if (i >= DEF_CALIBRATION_MOVE_POINT && i < DEF_CALIBRATION_MOVE_POINT * 2)
				SignedDeltaY = 0;
			else
				SignedDeltaY = m_sCalibData.m_dMoveWidth_Y;

			m_rgxyRobotPos[i].dX = xytPosition.dX + SignedDeltaX;
			m_rgxyRobotPos[i].dY = xytPosition.dY + SignedDeltaY;

			m_rgxyOffsetPos[i].dX = SignedDeltaX;
			m_rgxyOffsetPos[i].dY = SignedDeltaY;
		}
	}
	//TabCarrier로 이동하며 TabCarrier Camera Calibration
	else if (DEF_CALIBRATION_TABCARRIER_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		ASSERT(m_plnkTabCarrier != NULL);
		xytPosition = m_plnkTabCarrier->GetCurrentXYTPos();
		
		for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
		{
			if (i % DEF_CALIBRATION_MOVE_POINT == 0)
				SignedDeltaX = -m_sCalibData.m_dMoveWidth_X;
			else if (i % DEF_CALIBRATION_MOVE_POINT == 1)
				SignedDeltaX = 0.0;
			else
				SignedDeltaX = m_sCalibData.m_dMoveWidth_X;
			
			if (i < DEF_CALIBRATION_MOVE_POINT)
				SignedDeltaY = -m_sCalibData.m_dMoveWidth_Y;
			else if (i >= DEF_CALIBRATION_MOVE_POINT && i < DEF_CALIBRATION_MOVE_POINT * 2)
				SignedDeltaY = 0;
			else
				SignedDeltaY = m_sCalibData.m_dMoveWidth_Y;

			m_rgxyRobotPos[i].dX = xytPosition.dX + SignedDeltaX;
			m_rgxyRobotPos[i].dY = xytPosition.dY + SignedDeltaY;

			m_rgxyOffsetPos[i].dX = SignedDeltaX;
			m_rgxyOffsetPos[i].dY = SignedDeltaY;
		}
	}
	//[TabMounter/Gantry로 이동하며 InspectionCarrier Camera Calibration]
	else if (DEF_CALIBRATION_GANTRY_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		ASSERT(m_plnkTabMounter != NULL);

		xytPosition.dX = m_plnkTabMounter->GetCurrentPos();
		xytPosition.dY = m_plnkTabMounter->GetCurrentGantryPos();
		xytPosition.dT = 0.0;
		
		for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
		{
			if (i % DEF_CALIBRATION_MOVE_POINT == 0)
				SignedDeltaX = -m_sCalibData.m_dMoveWidth_X;
			else if (i % DEF_CALIBRATION_MOVE_POINT == 1)
				SignedDeltaX = 0.0;
			else
				SignedDeltaX = m_sCalibData.m_dMoveWidth_X;
			
			if (i < DEF_CALIBRATION_MOVE_POINT)
				SignedDeltaY = -m_sCalibData.m_dMoveWidth_Y;
			else if (i >= DEF_CALIBRATION_MOVE_POINT && i < DEF_CALIBRATION_MOVE_POINT * 2)
				SignedDeltaY = 0;
			else
				SignedDeltaY = m_sCalibData.m_dMoveWidth_Y;

			m_rgxyRobotPos[i].dX = xytPosition.dX + SignedDeltaX;
			m_rgxyRobotPos[i].dY = xytPosition.dY + SignedDeltaY;

			m_rgxyOffsetPos[i].dX = SignedDeltaX;
			m_rgxyOffsetPos[i].dY = SignedDeltaY;
		}
	}
	m_dMovePosition.dT = xytPosition.dT;	/** T 는 현재 위치 저장 */
	return ERR_VCALIB_SUCCESS;
} 

int CVisionCalibDlg::executeCalibrationMoves(int iPrismSideNo)
{
	int i = 0;
	int iResult = ERR_VCALIB_SUCCESS;

	double dXDistance = 0.0;
	double dYDistance = 0.0;
	double dBacklashDistance = m_plnkSystemData->m_dBacklash;
	BOOL bMoveBacklashX = FALSE;
	BOOL bMoveBacklashY = FALSE;

	stillImage(m_iTargetCamNo);

	// 계산된 Robot 좌표마다 움직여 Pixel  찾아내기.
	if (DEF_CALIBRATION_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		MPos_XYT sPos = m_plnkPanelAligner->GetXYTCurrentPos();
		for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
		{
			MPos_XYT sCurrentPos = m_plnkPanelAligner->GetXYTCurrentPos();

			dXDistance = m_rgxyRobotPos[i].dX - sCurrentPos.dX;
			dYDistance = m_rgxyRobotPos[i].dY - sCurrentPos.dY;

			if (dXDistance < 0)
			{
				dXDistance -= dBacklashDistance;
				bMoveBacklashX = TRUE;
			}

			if (dYDistance < 0)
			{
				dYDistance -= dBacklashDistance;
				bMoveBacklashY = TRUE;
			}
			MPos_XYT sTgtPos;

			sTgtPos.dX = sTgtPos.dX + dXDistance;
			sTgtPos.dY = sTgtPos.dY + dYDistance;

			iResult = m_plnkPanelAligner->RMoveXPosOnly(dXDistance, DEF_PANEL_ALIGNER_NONE_POS, TRUE);
			if (iResult)
				return iResult;

			Sleep(500);

			iResult = m_plnkPanelAligner->RMoveYPosOnly(dYDistance, DEF_PANEL_ALIGNER_NONE_POS, TRUE);
			if (iResult)
				return iResult;

			Sleep(1000);

			if (TRUE == bMoveBacklashX)
			{
				bMoveBacklashX = FALSE;

				iResult = m_plnkPanelAligner->RMoveXPosOnly(dBacklashDistance, DEF_PANEL_ALIGNER_NONE_POS, TRUE);
				if (iResult)
					return iResult;

				Sleep(500);
			}

			if (TRUE == bMoveBacklashY)
			{
				bMoveBacklashY = FALSE;

				iResult = m_plnkPanelAligner->RMoveYPosOnly(dBacklashDistance, DEF_PANEL_ALIGNER_NONE_POS, TRUE);
				if (iResult)
					return iResult;

				Sleep(1000);
			}

			stillImage(m_iTargetCamNo);

			iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo, DEF_CALIBRATION_MARK1);
			if (iResult)
			{
				Sleep(300);
				stillImage(m_iTargetCamNo);
				iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo, DEF_CALIBRATION_MARK1);
				if(iResult)
				{
					return iResult;
				}
			}				
			m_rgxyPixelPos[m_iCalibUnit][i].dX = m_plnkVision->GetSearchResultX(m_iTargetCamNo, DEF_CALIBRATION_MARK1);
			m_rgxyPixelPos[m_iCalibUnit][i].dY = m_plnkVision->GetSearchResultY(m_iTargetCamNo, DEF_CALIBRATION_MARK1);
		}

		iResult = m_plnkPanelAligner->SyncMoveXYTPos(sPos, DEF_VCALIB_NO_POSID);
	}
	else if (DEF_CALIBRATION_TABCARRIER_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		MPos_XYT sPos = m_plnkTabCarrier->GetCurrentXYTPos();
		MPos_XYT PosTarget = sPos;
		for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
		{
			MPos_XYT sCurrentPos = m_plnkTabCarrier->GetCurrentXYTPos();
			PosTarget.dX = m_rgxyRobotPos[i].dX;
			PosTarget.dY = m_rgxyRobotPos[i].dY;

			///필요하면 살려서 테스트 
			///dXDistance = m_rgxyRobotPos[i].dX - sCurrentPos.dX;
			///dYDistance = m_rgxyRobotPos[i].dY - sCurrentPos.dY;

			if (dXDistance < 0)
			{
				PosTarget.dX -= dBacklashDistance;
				bMoveBacklashX = TRUE;
			}

			if (dYDistance < 0)
			{
				PosTarget.dY -= dBacklashDistance;
				bMoveBacklashY = TRUE;
			}

			iResult = m_plnkTabCarrier->MoveXYTPos(PosTarget, -1, TRUE, TRUE);
			if (iResult)
				return iResult;

			Sleep(1000);

			if (TRUE == bMoveBacklashX || TRUE == bMoveBacklashY)
			{
				bMoveBacklashX = FALSE;
				bMoveBacklashY = FALSE;
				PosTarget.dX = m_rgxyRobotPos[i].dX;
				PosTarget.dY = m_rgxyRobotPos[i].dY;
				iResult = m_plnkTabCarrier->MoveXYTPos(PosTarget, -1, TRUE, FALSE);
				if (iResult)
					return iResult;

				Sleep(1000);
			}

			stillImage(m_iTargetCamNo);

			iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo, DEF_CALIBRATION_MARK1);
			if (iResult)
			{
				Sleep(300);
				stillImage(m_iTargetCamNo);
				iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo, DEF_CALIBRATION_MARK1);
				if(iResult)
				{
					return iResult;
				}
			}			
			m_rgxyPixelPos[m_iCalibUnit][i].dX = m_plnkVision->GetSearchResultX(m_iTargetCamNo, DEF_CALIBRATION_MARK1);
			m_rgxyPixelPos[m_iCalibUnit][i].dY = m_plnkVision->GetSearchResultY(m_iTargetCamNo, DEF_CALIBRATION_MARK1);
		}
		iResult = m_plnkTabCarrier->MoveXYTPos(sPos, -1, TRUE, TRUE);
		if (iResult)
			return iResult;
//		iResult = m_plnkPanelAligner->SyncMoveXYTPos(sPos, DEF_VCALIB_NO_POSID);
	}
	else if(DEF_CALIBRATION_GANTRY_2D == m_rgsType[m_iTargetCamNo].m_iType)
	{
		MPos_XYT sPos;
		sPos.Init(0.0, 0.0, 0.0);
		MPos_XYT PosTarget = sPos;
		sPos.dX = m_plnkTabMounter->GetCurrentPos();
		sPos.dY = m_plnkTabMounter->GetCurrentGantryPos();

		for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
		{
			MPos_XYT sCurrentPos; 
			sCurrentPos.Init(0.0, 0.0, 0.0);
			sCurrentPos.dX = m_plnkTabMounter->GetCurrentPos();
			sCurrentPos.dY = m_plnkTabMounter->GetCurrentGantryPos();		
			
			PosTarget.dX = m_rgxyRobotPos[i].dX;
			PosTarget.dY = m_rgxyRobotPos[i].dY;

			///필요하면 살려서 테스트 
			///dXDistance = m_rgxyRobotPos[i].dX - sCurrentPos.dX;
			///dYDistance = m_rgxyRobotPos[i].dY - sCurrentPos.dY;

			if (dXDistance < 0)
			{
				PosTarget.dX -= dBacklashDistance;
				bMoveBacklashX = TRUE;
			}

			if (dYDistance < 0)
			{
				PosTarget.dY -= dBacklashDistance;
				bMoveBacklashY = TRUE;
			}

			iResult = m_plnkTabMounter->MovePos(PosTarget.dX, DEF_TABMOUNTER_NONE_POS );
			if (iResult)
				return iResult;

			iResult = m_plnkTabMounter->MoveGantryPos(PosTarget.dY, DEF_TABMOUNTER_NONE_POS);
			if (iResult)
				return iResult;

			Sleep(1000);

			if (TRUE == bMoveBacklashX || TRUE == bMoveBacklashY)
			{
				bMoveBacklashX = FALSE;
				bMoveBacklashY = FALSE;
				PosTarget.dX = m_rgxyRobotPos[i].dX;
				PosTarget.dY = m_rgxyRobotPos[i].dY;
				iResult = m_plnkTabMounter->MovePos(PosTarget.dX, DEF_TABMOUNTER_NONE_POS );
				if (iResult)
					return iResult;
				iResult = m_plnkTabMounter->MoveGantryPos(PosTarget.dY, DEF_TABMOUNTER_NONE_POS);
				if (iResult)
					return iResult;

				Sleep(1000);
			}

			stillImage(m_iTargetCamNo);

			iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo, DEF_CALIBRATION_MARK2);
			if (iResult)
			{
				Sleep(300);
				stillImage(m_iTargetCamNo);
				iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo, DEF_CALIBRATION_MARK2);
				if(iResult)
				{
					return iResult;
				}
			}			
			m_rgxyPixelPos[m_iCalibUnit][i].dX = m_plnkVision->GetSearchResultX(m_iTargetCamNo, DEF_CALIBRATION_MARK2);
			m_rgxyPixelPos[m_iCalibUnit][i].dY = m_plnkVision->GetSearchResultY(m_iTargetCamNo, DEF_CALIBRATION_MARK2);
		}
		iResult = m_plnkTabMounter->MovePos(sPos.dX, DEF_TABMOUNTER_NONE_POS );
		if (iResult)
			return iResult;
		iResult = m_plnkTabMounter->MoveGantryPos(sPos.dY, DEF_TABMOUNTER_NONE_POS);
		if (iResult)
			return iResult;
	}
	else
	{
		ASSERT(FALSE);	// Calibration Type 결정 안됨.
	}

	return ERR_VCALIB_SUCCESS;
}

//TabAlign Camera중심간 거리 얻기
void CVisionCalibDlg::OnCameraDist2()
{
	// 카메라 간 거리 얻기를 실행하시겠습니까?\r\n다른 Mounter 들의 안전 위치를 확보하세요.",
	if (MyMessageBox(MY_MESSAGE_BOX,63524, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = expandTabCarrierCamera();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	// TargetCamNo 에 따라서 전/후면 구분.
	int iGroupNo = DEF_FRONT_GROUP;
	MTabCarrier* plnkTabCarrier = NULL;
	MManageOpPanel* plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
	int iOldJogID = plnkManageOpPanel->GetJogUnit();
	switch (m_iTargetCamNo)
	{
	case DEF_F_TABALIGNER_CAMERA_1:
	case DEF_F_TABALIGNER_CAMERA_2:
		//SJ_YYK 150817 Add..
	case DEF_F_TABALIGNER_CAMERA_3:
	case DEF_F_TABALIGNER_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
		plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(DEF_TABCARRIER4);
		plnkManageOpPanel->SetJogUnit(DEF_JOG_F_TABCARRIER4_XYT);
#else
		plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(DEF_TABCARRIER2);
		plnkManageOpPanel->SetJogUnit(DEF_JOG_F_TABCARRIER2_XYT);
#endif
		iGroupNo = DEF_FRONT_GROUP;
		break;
	case DEF_R_TABALIGNER_CAMERA_1:
	case DEF_R_TABALIGNER_CAMERA_2:
		//SJ_YYK 150817 Add..
	case DEF_R_TABALIGNER_CAMERA_3:
	case DEF_R_TABALIGNER_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
		plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(DEF_TABCARRIER4);
		plnkManageOpPanel->SetJogUnit(DEF_JOG_R_TABCARRIER4_XYT);
#else
		plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(DEF_TABCARRIER2);
		plnkManageOpPanel->SetJogUnit(DEF_JOG_R_TABCARRIER2_XYT);
#endif
		iGroupNo = DEF_REAR_GROUP;
		break;

	default:
		//Camera 번호 선택 오류
		MyMessageBox(MY_MESSAGE_BOX,63522, _T("Error"), M_ICONERROR);
		return;
	}

	// 확인 필요 
	// 작업 TabMounter 에 TabIC 흡착 후, PanelAlign Camera 위치로 이동. 
	// TabIC 흡착 작업은 미리 해 둬야 함.

	// PanelAlign Camera 에 측광 조명 장착했는지 확인.

	const int DEF_MOVING_POINT = 2;
	int rgiCameraID[DEF_MOVING_POINT] = { -1, -1};
	double rgdCameraPos[DEF_MOVING_POINT] = { 0.0, 0.0 };
	double rgdCameraPosShrink[DEF_MOVING_POINT] = { 0.0, 0.0 };

	if (m_iTargetCamNo == DEF_F_TABALIGNER_CAMERA_1
		|| m_iTargetCamNo == DEF_F_TABALIGNER_CAMERA_2)
	{
		rgiCameraID[0] = DEF_F_TABALIGNER_CAMERA_1;
		rgiCameraID[1] = DEF_F_TABALIGNER_CAMERA_2;
	}
	else if(m_iTargetCamNo == DEF_F_TABALIGNER_CAMERA_3
		|| m_iTargetCamNo == DEF_F_TABALIGNER_CAMERA_4)
	{
		rgiCameraID[0] = DEF_F_TABALIGNER_CAMERA_3;
		rgiCameraID[1] = DEF_F_TABALIGNER_CAMERA_4;
	}
	else if(m_iTargetCamNo == DEF_R_TABALIGNER_CAMERA_1
		|| m_iTargetCamNo == DEF_R_TABALIGNER_CAMERA_2)
	{
		rgiCameraID[0] = DEF_R_TABALIGNER_CAMERA_1;
		rgiCameraID[1] = DEF_R_TABALIGNER_CAMERA_2;
	}
	else
	{			
		rgiCameraID[0] = DEF_R_TABALIGNER_CAMERA_3;
		rgiCameraID[1] = DEF_R_TABALIGNER_CAMERA_4;
	}

	// Jog 로 위치 확인 요청.
	for (int i = 0; i < DEF_MOVING_POINT; i++)
	{
		liveImage(rgiCameraID[i]);
		
		// Jog로 Mark를 이동하여 Vision에 들어오게 한다.

		//Jog를 이용하여 Mark 가 Vision 화면에 들어오게 하고 확인 버튼을 눌러 주세요.
		if (MyMessageBox(MY_MESSAGE_BOX,63523, _T("Question"), M_ICONQUESTION |M_YESNO, _T("Confirm"), _T("CanCel")) == IDNO)
		{
			m_bOnStillContinue = FALSE;
			haltImage(rgiCameraID[i]);
			plnkTabCarrier->Down();

			//사용자 요청에 의해 작업을 중단함.
			MyMessageBox(MY_MESSAGE_BOX,63525, _T("Information"), M_ICONINFORMATION);

			plnkManageOpPanel->SetJogUnit(iOldJogID);
			return;
		}
		Sleep(1000);

		m_bOnStillContinue = FALSE;
		haltImage(rgiCameraID[i]);		
		rgdCameraPos[i] = plnkTabCarrier->GetCurrentXYTPos().dY;
	}


//	m_plnkSystemData->m_dTabCameraDist1[iGroupNo] = fabs(rgdCameraPos[0] - rgdCameraPos[1]);
	double dCamDistExpand = fabs(rgdCameraPos[0] - rgdCameraPos[1]);
	double dCamDistShrink = fabs(rgdCameraPosShrink[0] - rgdCameraPosShrink[1]);
	double dStartExpand = rgdCameraPos[0] < rgdCameraPos[1] ? rgdCameraPos[0] : rgdCameraPos[1];
	double dStartShrink = rgdCameraPosShrink[0] < rgdCameraPosShrink[1] ? rgdCameraPosShrink[0] : rgdCameraPosShrink[1];


	if(m_iTargetCamNo == DEF_F_TABALIGNER_CAMERA_1 || m_iTargetCamNo == DEF_F_TABALIGNER_CAMERA_2
		|| m_iTargetCamNo == DEF_R_TABALIGNER_CAMERA_1 || m_iTargetCamNo == DEF_R_TABALIGNER_CAMERA_2)
		m_plnkSystemData->m_dData_CameraExpand[iGroupNo][DEF_CAM_TABCARRIER] = dCamDistExpand;
	else
		m_plnkSystemData->m_dData_CameraExpand[iGroupNo][DEF_CAM_TABCARRIER2] = dCamDistExpand;
	plnkTabCarrier->GetExpandMotionComponent()->ResetOrigin();
	plnkTabCarrier->ChangeModel();


	m_plnkSystemData->WriteData();

	OnLive();
	m_bOnStillContinue = TRUE;

	CString strTemp;
	strTemp.Format(_T("Distance between Cameras [Panel Align Cam ... TabAlign Cam2] : %.3f"),
		m_plnkSystemData->m_dTabCameraDist1[iGroupNo]);
// jdy sesl	MyMessageBox(iResult, _T("Result Information"), M_ICONINFORMATION|M_OK);
	MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()), _T("Result Information"), M_ICONINFORMATION|M_OK);

	plnkManageOpPanel->SetJogUnit(iOldJogID);
	return;

}

//Inspection Camera중심간 거리 얻기
void CVisionCalibDlg::OnCameraDist3() 
{
	m_listResult.DeleteString(0);
	m_listResult.InsertString(0, _T("Distance Between View Centers"));

	// TargetCamNo 에 따라서 전/후면 구분.
	int iGroupNo = DEF_FRONT_GROUP;
	MCameraCarrier *pCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER] = {NULL, };
	MTabMounter* plnkTabMounter = NULL;
	MManageOpPanel* plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();

	int iOldJogID = plnkManageOpPanel->GetJogUnit();

	switch (m_iTargetCamNo)
	{
	case DEF_INSPECTION_CAMERA_1:
	case DEF_INSPECTION_CAMERA_2:	
	case DEF_INSPECTION_CAMERA_3:	
	case DEF_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_INSPECTION_CAMERA_5:	
	case DEF_INSPECTION_CAMERA_6:	
	case DEF_INSPECTION_CAMERA_7:	
	case DEF_INSPECTION_CAMERA_8:
#endif
		iGroupNo = DEF_FRONT_GROUP;
		for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			pCamCarrier[i] = MOlbSystemPre.GetCameraCarrierComponent(i);
#ifdef DEF_SOURCE_SYSTEM
		plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER4);
		plnkManageOpPanel->SetJogUnit(DEF_JOG_F_TABMOUNTER4_X);
#else
		plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER2);
		plnkManageOpPanel->SetJogUnit(DEF_JOG_F_TABMOUNTER2_X);
#endif
		break;
	case DEF_R_INSPECTION_CAMERA_1:
	case DEF_R_INSPECTION_CAMERA_2:	
	case DEF_R_INSPECTION_CAMERA_3:	
	case DEF_R_INSPECTION_CAMERA_4:	
#ifdef DEF_SOURCE_SYSTEM
	case DEF_R_INSPECTION_CAMERA_5:
	case DEF_R_INSPECTION_CAMERA_6:
	case DEF_R_INSPECTION_CAMERA_7:
	case DEF_R_INSPECTION_CAMERA_8:
#endif
		iGroupNo = DEF_REAR_GROUP;
		for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			pCamCarrier[i] = MOlbSystemPre.GetRCameraCarrierComponent(i);
#ifdef DEF_SOURCE_SYSTEM
		plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(DEF_TABMOUNTER4);
		plnkManageOpPanel->SetJogUnit(DEF_JOG_R_TABMOUNTER4_X);
#else
		plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(DEF_TABMOUNTER2);
		plnkManageOpPanel->SetJogUnit(DEF_JOG_R_TABMOUNTER2_X);
#endif
		break;

	default:
		//Camera 번호 선택 오류이거나, Prism 을 사용하지 않는 Camera 임.",
		MyMessageBox(MY_MESSAGE_BOX,63527, _T("Error"), M_ICONERROR);
		return;
	}


	CString strResult;
	m_listResult.ResetContent();
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{

		strResult.Format(_T("Cam%d : %.3f"), i + 1, m_plnkSystemData->m_dData_CameraExpand[iGroupNo][i]);
//		m_listResult.DeleteString(i + 1);
		m_listResult.InsertString(i + 1, strResult);
	}

	// Camera View Center 간 거리 얻기 작업을 수행하시겠습니까?
	int iResult = MyMessageBox(MY_MESSAGE_BOX, 63526, _T("Question"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
		return;

/*	iResult = expandTabAlignerCamera();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, "Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);		
		return;
	}
/*
	iResult = expandInspectCamera();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, "Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
*/	


	iResult = m_plnkCamCarrier->SafeMoveExpand(DEF_CAMERACARRIER_EXPAND_INIT_POS, TRUE, FALSE);

	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = plnkTabMounter->DownCyl();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_INSPECTION_POS);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	m_bOnStillContinue = FALSE;
	haltImage(m_iTargetCamNo);
	int rgiTargetCamNo[2] = { m_iTargetCamNo, m_iTargetCamNo };
//	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		switch (m_iTargetCamNo)
		{
		case DEF_INSPECTION_CAMERA_1:
			rgiTargetCamNo[1] = DEF_INSPECTION_CAMERA_2;
			break;
		case DEF_INSPECTION_CAMERA_2:
			rgiTargetCamNo[1] = DEF_INSPECTION_CAMERA_1;
			break;
		case DEF_INSPECTION_CAMERA_3:
			rgiTargetCamNo[1] = DEF_INSPECTION_CAMERA_4;
			break;
		case DEF_INSPECTION_CAMERA_4:
			rgiTargetCamNo[1] = DEF_INSPECTION_CAMERA_3;
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CAMERA_5:
			rgiTargetCamNo[1] = DEF_INSPECTION_CAMERA_6;
			break;
		case DEF_INSPECTION_CAMERA_6:
			rgiTargetCamNo[1] = DEF_INSPECTION_CAMERA_5;
			break;
		case DEF_INSPECTION_CAMERA_7:
			rgiTargetCamNo[1] = DEF_INSPECTION_CAMERA_8;
			break;
		case DEF_INSPECTION_CAMERA_8:
			rgiTargetCamNo[1] = DEF_INSPECTION_CAMERA_7;
			break;
#endif

		case DEF_R_INSPECTION_CAMERA_1:
			rgiTargetCamNo[1] = DEF_R_INSPECTION_CAMERA_2;
			break;
		case DEF_R_INSPECTION_CAMERA_2:
			rgiTargetCamNo[1] = DEF_R_INSPECTION_CAMERA_1;
			break;
		case DEF_R_INSPECTION_CAMERA_3:
			rgiTargetCamNo[1] = DEF_R_INSPECTION_CAMERA_4;
			break;
		case DEF_R_INSPECTION_CAMERA_4:
			rgiTargetCamNo[1] = DEF_R_INSPECTION_CAMERA_3;
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_R_INSPECTION_CAMERA_5:
			rgiTargetCamNo[1] = DEF_R_INSPECTION_CAMERA_6;
			break;
		case DEF_R_INSPECTION_CAMERA_6:
			rgiTargetCamNo[1] = DEF_R_INSPECTION_CAMERA_5;
			break;
		case DEF_R_INSPECTION_CAMERA_7:
			rgiTargetCamNo[1] = DEF_R_INSPECTION_CAMERA_8;
			break;
		case DEF_R_INSPECTION_CAMERA_8:
			rgiTargetCamNo[1] = DEF_R_INSPECTION_CAMERA_7;
			break;
#endif
		default:
			break;
		}
	}

	double rgdTabMounterPos[2] = { 0.0, 0.0 };
	double rgdTabMounterPosShrink[2] = {0.0, 0.0};
	for (int i = 0; i < 2; i++)
	{
		liveImage(rgiTargetCamNo[i]);
		
		// Jog로 Mark를 이동하여 Vision에 들어오게 한다.

		//Jog를 이용하여 Mark 가 Vision 화면 기준 위치에 오도록 한 후, 확인 버튼을 눌러 주세요.
		if (MyMessageBox(MY_MESSAGE_BOX,63528, _T("Question"), M_ICONQUESTION |M_YESNO, _T("Confirm"), _T("Cancel")) == IDNO)
		{
			m_bOnStillContinue = FALSE;
			haltImage(rgiTargetCamNo[i]);
			plnkTabMounter->UpCyl();

			//사용자 요청에 의해 작업을 중단함.
			MyMessageBox(MY_MESSAGE_BOX, 63529, _T("Information"), M_ICONINFORMATION);

			plnkManageOpPanel->SetJogUnit(iOldJogID);
			return;
		}

		Sleep(1000);

		m_bOnStillContinue = FALSE;
		haltImage(rgiTargetCamNo[i]);
		
		rgdTabMounterPos[i] = plnkTabMounter->GetCurrentPos();
	}

	iResult = plnkTabMounter->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}


	int iTargetCamNo = m_iTargetCamNo;
	
	switch (iTargetCamNo)
	{
	case DEF_INSPECTION_CAMERA_1:
	case DEF_INSPECTION_CAMERA_2:
	case DEF_R_INSPECTION_CAMERA_1:
	case DEF_R_INSPECTION_CAMERA_2:
		iTargetCamNo = DEF_INSPECTION_CARRIER_1;
		break;
	case DEF_INSPECTION_CAMERA_3:
	case DEF_INSPECTION_CAMERA_4:
	case DEF_R_INSPECTION_CAMERA_3:
	case DEF_R_INSPECTION_CAMERA_4:
		iTargetCamNo = DEF_INSPECTION_CARRIER_2;
		break;

#ifdef DEF_SOURCE_SYSTEM
	case DEF_INSPECTION_CAMERA_5:
	case DEF_INSPECTION_CAMERA_6:
	case DEF_R_INSPECTION_CAMERA_5:
	case DEF_R_INSPECTION_CAMERA_6:
		iTargetCamNo = DEF_INSPECTION_CARRIER_3;
		break;
	case DEF_INSPECTION_CAMERA_7:
	case DEF_INSPECTION_CAMERA_8:
	case DEF_R_INSPECTION_CAMERA_7:
	case DEF_R_INSPECTION_CAMERA_8:
		iTargetCamNo = DEF_INSPECTION_CARRIER_4;
		break;
#endif
		
	default:
		break;
	}

	double Dist_Expand = fabs(rgdTabMounterPos[0] - rgdTabMounterPos[1]);
	double Dist_Shrink = fabs(rgdTabMounterPosShrink[0] - rgdTabMounterPosShrink[1]);
	double dStartExpand = rgdTabMounterPos[0] < rgdTabMounterPos[1] ? rgdTabMounterPos[0] : rgdTabMounterPos[1];
	double dStartShrink = rgdTabMounterPosShrink[0] < rgdTabMounterPosShrink[1] ? rgdTabMounterPosShrink[0] : rgdTabMounterPosShrink[1];

	m_plnkSystemData->m_dData_CameraExpand[iGroupNo][iTargetCamNo] = Dist_Expand;
	m_plnkCamCarrier->GetMotionComponentExpand()->ResetOrigin();

	m_plnkSystemData->WriteData();
	
	m_plnkPanelAligner->ChangeModel();
	m_plnkCamCarrier->ChangeModel();

	OnLive();
	m_bOnStillContinue = TRUE;

	CString strTemp;
	strTemp.Format(_T("Distance between Camera%d View Centers : %.3f"), m_iTargetCamNo + 1, m_plnkSystemData->m_dData_CameraExpand[iGroupNo][iTargetCamNo]);

	MyMessageBox(strTemp, _T("Result"), M_ICONERROR|M_OK);

	plnkManageOpPanel->SetJogUnit(iOldJogID);
}

void CVisionCalibDlg::OnClickCameraDist4()
{
	/*/

#ifdef DEF_GATE_SYSTEM

	// TODO: Add your control notification handler code here
	if (MyMessageBox(MY_MESSAGE_BOX,63543, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	//=====Process=====
	//PanelAligner에 Panel 흡착되어 있는지 확인
	//Air부상 흡착해제
	//PanelAligner Move위치로 이동
	//PanelRotator하강
	//PanelRotator정위치 회전	
	//PanelAligner PanelAlign동작
	//Air부상 Panel흡착
	//PanelAligner흡착해제, 하강
	//PanelRotator흡착위치로 이동
	//First 마크인식
	//PanelRotator Panel흡착, 상승
	//PanelRotator Panel회전
	//Air부상 Panel흡착
	//First 마크 인식
	//중심간 거리 계산
	//=================
	int iResult = ERR_VCALIB_SUCCESS;

#ifndef SIMULATION
	//설계 치수.....
	const double dHWDistX = 500.0;
	const double dHWDistY = 0.0;
	//..............

	double dDistX = m_plnkSystemData->m_dDist_AlignerToRotator_X;
	double dDistY = m_plnkSystemData->m_dDist_AlignerToRotator_Y;
	if (fabs(dDistX - dHWDistX) > 10.0)
		m_plnkSystemData->m_dDist_AlignerToRotator_X = dHWDistX;
	if (fabs(dDistY - dHWDistY) > 10.0)
		m_plnkSystemData->m_dDist_AlignerToRotator_Y = dHWDistY;

	//PanelAligner Panel흡착 확인
	if (FALSE == m_plnkPanelAligner->IsPanelAbsorbed())
	{
		// PanelAligner 가 Panel 을 흡착하고 있습니다.
		MyMessageBox(MY_MESSAGE_BOX,63514, _T("Pre-condition Check"), M_ICONERROR);
		return;
	}
	//Air부상 Panel Blow확인
	iResult = m_plnkPanelAligner->AirCVBlowOn();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//PanelAligner Z축 Move위치로 이동
	iResult = m_plnkPanelAligner->UpPickUpUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//Rotator Panel 흡착 해제
	iResult = m_plnkPanelAligner->ReleasePanelRotator();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//Rotator 하강
	iResult = m_plnkPanelAligner->DownRotatorUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	//Rotator Return확인
	iResult = m_plnkPanelAligner->ReturnRotatorUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	//PanelAligne Mode를 Fiducial Mark로 지정
	BOOL bUseTabAlign = m_plnkSystemData->m_bUseTabPanelAlign;
	m_plnkSystemData->m_bUseTabPanelAlign = FALSE;

	//PanelAlign 동작
	iResult = m_plnkCtrlPanelAligner->AlignPanel(FALSE);
	if (iResult)
	{
		m_plnkSystemData->m_bUseTabPanelAlign = bUseTabAlign;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	int i = 0;
	for (int i = 1; ; i++)
	{
		iResult = m_plnkCtrlPanelAligner->MovetoAlignPos();
		if (iResult != ERR_VCALIB_SUCCESS)
		{
			m_plnkSystemData->m_bUseTabPanelAlign = bUseTabAlign;
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		if (TRUE == m_plnkCtrlPanelAligner->CheckFiduMarkPos())
		{
			break;
		}
		iResult = m_plnkCtrlPanelAligner->AlignPanel(FALSE); 
		if (iResult != ERR_VCALIB_SUCCESS)
		{
			Sleep(1000);
			iResult = m_plnkCtrlPanelAligner->AlignPanel(FALSE);
			if (iResult)
			{
				m_plnkSystemData->m_bUseTabPanelAlign = bUseTabAlign;
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}
		if (i > m_plnkSystemData->m_iPanelAlignRetryLimit)
		{
			m_plnkSystemData->m_bUseTabPanelAlign = bUseTabAlign;
			CString strErr = MOlbSystemPre.GetErrorMessage(410005*1000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}

	//PanelAlign Mode를 원래대로 지정
	m_plnkSystemData->m_bUseTabPanelAlign = bUseTabAlign;

	iResult = m_plnkCtrlPanelAligner->TransferPanel_AlignerToRotator();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelAligner->TurnRotatorUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	MPos_XYT AlignerPos1 = m_plnkPanelAligner->GetXYTCurrentPos();

	MManageOpPanel* plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
	int iOldJogID = plnkManageOpPanel->GetJogUnit();
	plnkManageOpPanel->SetJogUnit(DEF_JOG_STAGE_XY);

	haltImage(DEF_INSPECTION_CAMERA_1);
	liveImage(DEF_INSPECTION_CAMERA_1);
		
	// Jog로 Mark를 이동하여 Vision에 들어오게 한다.

	//Jog를 이용하여 Mark 가 Vision 화면 기준 위치에 오도록 한 후, 확인 버튼을 눌러 주세요.
	if (MyMessageBox(MY_MESSAGE_BOX,63528, _T("Question"), M_ICONQUESTION |M_YESNO, _T("Confirm"), _T("Cancel")) == IDNO)
	{
		haltImage(DEF_INSPECTION_CAMERA_1);
		//사용자 요청에 의해 작업을 중단함.
		MyMessageBox(MY_MESSAGE_BOX, 63529, _T("Information"), M_ICONINFORMATION);
		plnkManageOpPanel->SetJogUnit(iOldJogID);
		return;
	}

	stillImage(DEF_INSPECTION_CAMERA_1);
	iResult = m_plnkVision->RecognitionPatternMark(DEF_INSPECTION_CAMERA_1, DEF_PANEL_LEAD_XU_MARK1);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
#endif

#ifndef SIMULATION
	double dVisionCenterX = m_plnkVision->GetDistribution(DEF_INSPECTION_CAMERA_1).x / 2 - 1;
	double dVisionCenterY = m_plnkVision->GetDistribution(DEF_INSPECTION_CAMERA_1).y / 2 - 1;
	double dVisionX = m_plnkVision->GetSearchResultX(DEF_INSPECTION_CAMERA_1, DEF_PANEL_LEAD_XU_MARK1);
	double dVisionY = m_plnkVision->GetSearchResultY(DEF_INSPECTION_CAMERA_1, DEF_PANEL_LEAD_XU_MARK1);
	double dRobotCenterX = 0.0;
	double dRobotCenterY = 0.0;
	double dRobotX = 0.0;
	double dRobotY = 0.0;
	m_plnkCalibration->ConvertV2R(DEF_INSPECTION_CAMERA_1, 0, dVisionCenterX, dVisionCenterY, dRobotCenterX, dRobotCenterY);
	m_plnkCalibration->ConvertV2R(DEF_INSPECTION_CAMERA_1, 0, dVisionX, dVisionY, dRobotX, dRobotY);
	MPos_XYT AlignerPos2 = m_plnkPanelAligner->GetXYTCurrentPos();
#else
	double dRobotCenterX = 0.0;
	double dRobotCenterY = 0.0;
	double dRobotX = -23.21;
	double dRobotY = -33.48;
	MPos_XYT AlignerPos1(460.08, 609.39, 0.0);
	MPos_XYT AlignerPos2(292.15, 294.59, 0.0);
#endif

//	AlignerPos2.dX += (dRobotCenterX - dRobotX);
//	AlignerPos2.dY += (dRobotCenterY - dRobotY);
	AlignerPos2.dX -= dRobotX;
	AlignerPos2.dY -= dRobotY;

	double dCenterOffsetX = (AlignerPos1.dX - AlignerPos2.dX)/2.0;
	double dCenterOffsetY = (AlignerPos1.dY - AlignerPos2.dY)/2.0;
	
	dCenterOffsetX = m_plnkSystemData->m_dDist_AlignerToRotator_X + dCenterOffsetX;
	dCenterOffsetY = m_plnkSystemData->m_dDist_AlignerToRotator_Y + dCenterOffsetY;

#ifndef SIMULATION
	CString strResult;
	strResult.Format(_T("값을 적용하겠습니까? [ CenterX:%.3lf, CenterY:%.3lf]"), dCenterOffsetX, dCenterOffsetY);
	if (MyMessageBox(strResult, _T("Question"), M_ICONQUESTION|M_YESNO) == IDYES)
	{
		m_plnkSystemData->m_dDist_AlignerToRotator_X = dCenterOffsetX;
		m_plnkSystemData->m_dDist_AlignerToRotator_Y = dCenterOffsetY;
		m_plnkSystemData->WriteData();
	}
#endif

#endif
	/*/

}

/** Camera 기준좌표로 고정좌표 찾기
 * 기 등록된 Mark를 인식하여 위치를 알아낸다. 인식할 Mark는 Xu변 또는 Xd변의 좌상단 Mark임.
 * @preconditions ExecuteCalibration()을 수행하여 CamFactor가 계산되어 있어야 한다.
 * @preconditions SetCellData()를 이용해서 Cell정보가 입력되어 있어야 한다.
 *
 * @param iCamNo : 카메라 번호
 * @param iMark   : 마크 번호
 * @param pX : X 좌표
 * @param pY : Y 좌표  
 * @return	Error : 0 = Success, 그외=기타
 */
int CVisionCalibDlg::GetTabCarrierCameraFixedPosition(int iCarrierID, 
								int iCamNo,
								int iMarkNo,
								double dTAngle,
								double* pX,
								double* pY,
								double* pCamDistX)
{
	double		dDistanceXY;
	double		dDistanceX;
	double		dDistanceY;
	double		dRadius;
	
	CString		tmpDisplay;			// Display를 위한 Temp String

	MPos_XYT	posFirstMark;		// 좌표를 저장한다.
	MPos_XYT	posSecondMark;		// 좌표를 저장한다.
	MPos_XYT	posFirst;			// 초기 좌표를 저장한다.
	MPos_XYT	posSecond;			// TAngle도 회전한후의 좌표를 저장한다.
	MPos_XYT	posMove;			// 이동할 좌표 저장
	MPos_XYT	posCenter;			// Center
	MPos_XYT	posTarget;			 

	int			iResult;
	double		dPixelX;
	double		dPixelY;

	MTabCarrier*  m_plnkTabCarrier;

	m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iCarrierID);

	posFirst = m_plnkTabCarrier->GetCurrentXYTPos();
	
	/**  1번째 Mark를 인식한다.-------------------------------*/
	m_plnkVision->Grab(iCamNo);

	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 1번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkCalibration->ConvertV2R(iCamNo, 
											m_plnkTabCarrier->GetInstanceNo(),
											dPixelX, dPixelY,
											posFirstMark.dX, posFirstMark.dY);
	}
	else
	{
		return iResult;
	}


	
	/** Stage 축을 dTurnAgle만큼 회전하고, XY를 움직여 Mark를 움직여 화면내에 들어오게 한다. */
	///dDistanceX = m_plnkPreBonderData->m_dLead_Width_Xtab / 2 ;
#ifdef DEF_SOURCE_SYSTEM
	dDistanceY = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Xtab / 2 ;
#else
	dDistanceY = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Ytab / 2 ;
#endif
	dDistanceX = MOlbSystemPre.GetDistance_TabAlignerCenterToTabMarkX();

	double dir = -1.0;
	/*__________________________
		(-X) <---------> (+X)
				Cam
				-----------------
				|				|
				|	  Panel		|
				|				|
				-----------------
	___________________________*/
	Rotation(dTAngle, dDistanceX*dir, dDistanceY, &posMove.dX, &posMove.dY);

	posMove.dX = dDistanceX*dir - posMove.dX;
	posMove.dY = dDistanceY - posMove.dY;

	posMove.dX = posFirst.dX + posMove.dX;
	posMove.dY = posFirst.dY + posMove.dY;
	posMove.dT = posFirst.dT + dTAngle;

	iResult = m_plnkTabCarrier->MoveXYTPos(posMove, DEF_TABCARRIER_NO_WORKING_POS, TRUE, FALSE);
	if (iResult) return iResult;
	
	/** Display Vision */
	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iCamNo);

//	if ( MyMessageBox(_T("Jog를 이용하여 Mark가 Vision 화면에 들어오게 하고 OK 버튼을 눌러 주세요."),
//		_T("확인"),M_ICONQUESTION |M_YESNO,_T("OK"),_T("취소")) == IDNO)
	if (MyMessageBox(MY_MESSAGE_BOX,63528, _T("Question"), M_ICONQUESTION |M_YESNO, _T("Confirm"), _T("Cancel")) == IDNO)
	{
		m_bOnStillContinue = FALSE;
		haltImage(iCamNo);

		//사용자 요청에 의해 작업을 중단함.
		MyMessageBox(MY_MESSAGE_BOX, 63529, _T("Information"), M_ICONINFORMATION);
		return -1;
	}

	posSecond = m_plnkTabCarrier->GetCurrentXYTPos();
	
	/** 2번째 Mark를 인식한다. */
	m_plnkVision->Grab(iCamNo);
	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 2번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkCalibration->ConvertV2R(iCamNo, 
											m_plnkTabCarrier->GetInstanceNo(),
											dPixelX, dPixelY,
											posSecondMark.dX, posSecondMark.dY);
	}
	else
	{
		return iResult;
	}

	/** 고정좌표 계산 시작  */
	posFirst	= posFirst	- posFirstMark;
	posSecond	= posSecond - posSecondMark;
    
	/** Fisrt Position에서 Second Position까지의 거리를 구한다. */
	dDistanceX	= (posFirst.dX - posSecond.dX);
	dDistanceY	= (posFirst.dY - posSecond.dY);
	dDistanceXY	= sqrt(dDistanceX * dDistanceX + dDistanceY * dDistanceY);

	/** Camera 중심에서 First (Second) Position까지의 거리를 구한다.. */
	dRadius		= dDistanceXY / (sqrt(2 * (1 - cos(DEG2RAD(dTAngle)))));

	/** First Position과 Second Position을 지나고 반지름 dRadius를 가지는 원의 중심을 구한다. */
	FindCenter(dTAngle, dRadius, posFirst, posSecond, &posCenter);
	
///#ifdef DEF_MACHINE_COAXIAL_AXIS_CAMERA
	//Camera Center를 고정좌표로 하기위해_____
	int iOtherCamNo = (iCamNo == DEF_F_TABALIGNER_CAMERA_1 || iCamNo == DEF_R_TABALIGNER_CAMERA_1) ? DEF_TAB_LEAD_XU_MARK1 : DEF_TAB_LEAD_XU_MARK2 ;

	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iOtherCamNo);
	m_plnkVision->ClearOverlay(iOtherCamNo);

//	if (MyMessageBox("Jog를 이용하여 2번째 Camera Mark가 Vision 화면에 들어오게 하고 OK 버튼을 눌러 주세요.",
//		"확인",M_ICONQUESTION |M_YESNO,"OK","취소") == IDNO)
	if (MyMessageBox(MY_MESSAGE_BOX,63528, _T("Question"), M_ICONQUESTION |M_YESNO, _T("Confirm"), _T("Cancel")) == IDNO)
	{
		m_bOnStillContinue = FALSE;
		haltImage(iCamNo);

		//사용자 요청에 의해 작업을 중단함.
		MyMessageBox(MY_MESSAGE_BOX, 63529, _T("Information"), M_ICONINFORMATION);
		return -1;
	}

	m_plnkVision->Grab(iOtherCamNo);
	MPos_XYT	posThirdMark;
	MPos_XYT	posThird;
	posThird = 	m_plnkTabCarrier->GetCurrentXYTPos();
	iResult = m_plnkVision->RecognitionPatternMark(iOtherCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		dPixelX = m_plnkVision->GetSearchResultX(iOtherCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iOtherCamNo, iMarkNo);
		m_plnkCalibration->ConvertV2R(iOtherCamNo, 
											m_plnkTabCarrier->GetInstanceNo(),
											dPixelX, dPixelY,
											posThirdMark.dX, posThirdMark.dY);
	}
	else
		return iResult;
	posThird = posThird - posThirdMark;
	posCenter.dX += ((posThird.dX-posSecond.dX)/2.0);
	posCenter.dY += ((posThird.dY-posSecond.dY)/2.0);
	//__________________________________________________

	*pCamDistX = fabs(posThird.dX - posSecond.dX);
///#endif
	*pX			= posCenter.dX;	// 카메라의 X축 좌표 리턴
	*pY			= posCenter.dY;	// 카메라의 Y축 좌표 리턴

	return ERR_VCALIB_SUCCESS;
}

void CVisionCalibDlg::CalculateCarrierCenterPos(int iGroupID)
{
	// Tab Carrier 회전 중심 측정 작업을 수행하시겠습니까?
	CString strTemp;
	CString strPrev;
	int m_iTargetCamNo1;
	int m_iTargetCamNo2;
	int m_iJobID;
	MPos_XYT dPreVision1;
	MPos_XYT dPreVision2;
	MPos_XYT dBeforePreVision1;
	MPos_XYT dBeforePreVision2;
	MPos_XYT dAfterPreVision1;
	MPos_XYT dAfterPreVision2;

	double dBeforeTheta = 0.0;
	double dDifferentRange = 0.6;

	int iResult = MyMessageBox(MY_MESSAGE_BOX, 63538,_T("Question"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
		return;

	MyMessageBox(MY_MESSAGE_BOX,63539, _T("Information"), M_ICONINFORMATION);

	strPrev.Format(_T("%.d"), 1);

	if (!GetNumberBox(strTemp.GetBuffer(10), 10, strPrev.GetBuffer(10))) 
		return;	

	m_iJobID = _wtoi(strTemp);

	m_iJobID = m_iJobID - 1;

	MTabCarrier* plnkTabCarrier = NULL;
	MManageOpPanel* plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
	MTabCarrier* pTabCarrier[DEF_MAX_TABCARRIER];
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		if (iGroupID == DEF_FRONT_GROUP)
		{
			pTabCarrier[i] = MOlbSystemPre.GetTabCarrierComponent(i);
			m_iTargetCamNo1 = DEF_F_TABALIGNER_CAMERA_1;
			m_iTargetCamNo2 = DEF_F_TABALIGNER_CAMERA_2;
		}
		else
		{
			pTabCarrier[i] = MOlbSystemPre.GetRTabCarrierComponent(i);
			m_iTargetCamNo1 = DEF_R_TABALIGNER_CAMERA_1;
			m_iTargetCamNo2 = DEF_R_TABALIGNER_CAMERA_2;
		}
	}

	switch (m_iJobID)
	{
	case DEF_TABCARRIER1:
		plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(DEF_TABCARRIER1);

		iResult = pTabCarrier[DEF_TABCARRIER4]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = pTabCarrier[DEF_TABCARRIER3]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = pTabCarrier[DEF_TABCARRIER2]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_PREALIGN_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		break;
	case DEF_TABCARRIER2:
		plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(DEF_TABCARRIER2);

		iResult = pTabCarrier[DEF_TABCARRIER4]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = pTabCarrier[DEF_TABCARRIER3]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = pTabCarrier[DEF_TABCARRIER1]->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		iResult = plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_PREALIGN_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		break;

#ifdef DEF_SOURCE_SYSTEM
	case DEF_TABCARRIER3:
		plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(DEF_TABCARRIER3);

		iResult = pTabCarrier[DEF_TABCARRIER4]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = pTabCarrier[DEF_TABCARRIER1]->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = pTabCarrier[DEF_TABCARRIER2]->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_PREALIGN_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		break;
	case DEF_TABCARRIER4:
		plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(DEF_TABCARRIER4);

		iResult = pTabCarrier[DEF_TABCARRIER1]->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = pTabCarrier[DEF_TABCARRIER2]->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = pTabCarrier[DEF_TABCARRIER3]->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_PREALIGN_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		break;
#endif
	default:
		//Camera 번호 선택 오류
		MyMessageBox(MY_MESSAGE_BOX,63522, _T("Error"), M_ICONERROR);
		return;
	}

	// 최초 인식 동작
	stillImage(m_iTargetCamNo1);

	iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo1, DEF_TAB_LEAD_XU_MARK1);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	double dPreVisionX1 = m_plnkVision->GetSearchResultX(m_iTargetCamNo1, DEF_TAB_LEAD_XU_MARK1);
	double dPreVisionY1 = m_plnkVision->GetSearchResultY(m_iTargetCamNo1, DEF_TAB_LEAD_XU_MARK1);

	double dPreRobotX1 = 0.0;
	double dPreRobotY1 = 0.0;

	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, 0, dPreVisionX1, dPreVisionY1, dPreRobotX1, dPreRobotY1);

	dBeforePreVision1.dX = dPreRobotX1;
	dBeforePreVision1.dY = dPreRobotY1;

	stillImage(m_iTargetCamNo2);

	iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo2, DEF_TAB_LEAD_XU_MARK1);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	double dPreVisionX2 = m_plnkVision->GetSearchResultX(m_iTargetCamNo2, DEF_TAB_LEAD_XU_MARK1);
	double dPreVisionY2 = m_plnkVision->GetSearchResultY(m_iTargetCamNo2, DEF_TAB_LEAD_XU_MARK1);

	double dPreRobotX2 = 0.0;
	double dPreRobotY2 = 0.0; 

	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, 0, dPreVisionX2, dPreVisionY2, dPreRobotX2, dPreRobotY2);

	dBeforePreVision2.dX = dPreRobotX2;
	dBeforePreVision2.dY = dPreRobotY2;

#ifdef DEF_SOURCE_SYSTEM
	double dMarkWidth = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Xtab;
#else
	double dMarkWidth = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Ytab;
#endif
	MPos_XYT dTgtPos;
	dTgtPos = plnkTabCarrier->GetCurrentXYTPos();
	dTgtPos.dT = dTgtPos.dT + 0.2;

	iResult = plnkTabCarrier->MoveXYTPos(dTgtPos, DEF_TABCARRIER_PREALIGN_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	// T축 이동 후 인식 동작
	stillImage(m_iTargetCamNo1);

	iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo1, DEF_TAB_LEAD_XU_MARK1);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	dPreVisionX1 = m_plnkVision->GetSearchResultX(m_iTargetCamNo1, DEF_TAB_LEAD_XU_MARK1);
	dPreVisionY1 = m_plnkVision->GetSearchResultY(m_iTargetCamNo1, DEF_TAB_LEAD_XU_MARK1);

	double dRobotX1 = 0.0;
	double dRobotY1 = 0.0;

	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, 0, dPreVisionX1, dPreVisionY1, dPreRobotX1, dPreRobotY1);

	dAfterPreVision1.dX = dPreRobotX1;
	dAfterPreVision1.dY = dPreRobotY1;

	stillImage(m_iTargetCamNo2);

	iResult = m_plnkVision->RecognitionPatternMark(m_iTargetCamNo2, DEF_TAB_LEAD_XU_MARK1);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	dPreVisionX2 = m_plnkVision->GetSearchResultX(m_iTargetCamNo2, DEF_TAB_LEAD_XU_MARK1);
	dPreVisionY2 = m_plnkVision->GetSearchResultY(m_iTargetCamNo2, DEF_TAB_LEAD_XU_MARK1);

	double dRobotX2 = 0.0;
	double dRobotY2 = 0.0; 

	m_plnkCalibration->ConvertV2R(DEF_PANEL_ALIGN_CAMERA_1, 0, dPreVisionX2, dPreVisionY2, dPreRobotX2, dPreRobotY2);

	dAfterPreVision2.dX = dPreRobotX2;
	dAfterPreVision2.dY = dPreRobotY2;

	double A,B,C,D,E,F,G,H,x0,y0,x1,x2,x3,x4,y1,y2,y3,y4;
	x1 = dBeforePreVision1.dX;
	y1 = dBeforePreVision1.dY;
	x2 = dAfterPreVision1.dX;
	y2 = dAfterPreVision1.dY;
	x3 = dBeforePreVision2.dX;
	y3 = dBeforePreVision2.dY;
	x4 = dAfterPreVision2.dX;
	y4 = dAfterPreVision2.dY;

	A = (x1*x1 - x2*x2);
	B = (2*x1 - 2*x2);
	C = (y1*y1 - y2*y2);
	D = (2*y1 - 2*y2);
	E = (x3*x3 - x4*x4);
	F = (2*x3 - 2*x4);
	G = (y3*y3 - y4*y4);
	H = (2*y3 - 2*y4);
	y0 = ( (E - F/B*A) + (G - F/B*C) ) / (H - F/B*D);
	x0 = (A + C - D*y0)/B;
	x0 -= dMarkWidth/2.0;

	if (iGroupID == DEF_FRONT_GROUP)
	{
		m_plnkSystemData->m_rgdCarrierHeadCenterX[m_iJobID] = x0;
		m_plnkSystemData->m_rgdCarrierHeadCenterY[m_iJobID] = y0;
	}
	else
	{
		m_plnkSystemData->m_rgdRCarrierHeadCenterX[m_iJobID] = x0;
		m_plnkSystemData->m_rgdRCarrierHeadCenterY[m_iJobID] = y0;
	}

	m_plnkSystemData->WriteData();

	//PanelAligner 거리 Data 얻기 작업을 완료했습니다.
	MyMessageBox(MY_MESSAGE_BOX, 63540, _T("Work Completion"), M_ICONINFORMATION);
	return;
}

int CVisionCalibDlg::expandTabAlignerCamera()
{
	if (NULL == m_plnkTabCarrier)
		m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(DEF_TABCARRIER1);
	int iResult = m_plnkTabCarrier->ExpandCamera();
	if (iResult)
		return iResult;
	return SUCCESS;
}

int CVisionCalibDlg::expandInspectCamera()
{
	if (NULL == m_plnkCamCarrier)
		m_plnkCamCarrier = MOlbSystemPre.GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
	int iResult = m_plnkCamCarrier->Expand();
	if (iResult)
		return iResult;
	return SUCCESS;
}

int CVisionCalibDlg::expandTabCarrierCamera()
{
	if (NULL == m_plnkTabCarrier)
	{
		if (m_iWorkGroup == DEF_FRONT_GROUP)
			m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(DEF_TABCARRIER1);
		else
			m_plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(DEF_TABCARRIER1);
	}


	int iResult = m_plnkTabCarrier->SafeMoveExpandPos(DEF_TABCARRIER_EXPAND_INIT_POS, TRUE, FALSE);
	if (iResult)
		return iResult;
	return SUCCESS;
}

/** Rotate an old point to a new point
  * @param dAngle : 각도
  * @param dOldX  : X of Point 1
  * @param dOldY  : Y of Point 1
  * @param dNewX  : X of Point 2
  * @param dNewY  : Y of Point 2
  */
void CVisionCalibDlg::Rotation(double dAngle, double dOldX, double dOldY, double* dNewX, double* dNewY)
{
	*dNewX = cos(DEG2RAD(dAngle)) * dOldX - sin(DEG2RAD(dAngle)) * dOldY;
	*dNewY = sin(DEG2RAD(dAngle)) * dOldX + cos(DEG2RAD(dAngle)) * dOldY;
}

/** Find two centers. One is in the positive range w.r.t. the line between the first position and the second position, 
    and it is an origin of a circle that passes on the first position and the sencond position with radius r.
    The other is in the negative range w.r.t. the line between the first position and the second position, 
    and it is also an origin of a circle that passes on the first position and the sencond position with radius r.
  * @param dTAngle : 각도
  * @param dRadius : 반지름
  * @param possFirst   : First Position
  * @param posSecond  : Second Position
  * @param posCenter  : the center is in the positive range w.r.t. the line passing on the First Position and the Second Position
  */
void CVisionCalibDlg::FindCenter(double dTAngle, 
						   double dRadius,
						   MPos_XYT posFirst,
						   MPos_XYT posSecond,
						   MPos_XYT* posCenter)
{
	MPos_XYT p1 = posFirst;        
	MPos_XYT p2 = posSecond;
	double t = DEG2RAD(dTAngle);

	posCenter->dX = ((-p2.dX + cos(t)*p1.dX - sin(t)*p1.dY)/(cos(t) - 1.0) + sin(t)/(cos(t) -1.0)*(-p2.dY + sin(t)*p1.dX + cos(t)*p1.dY)/(cos(t) - 1.0))/(1+sin(t)*sin(t)/(cos(t)-1.0)/(cos(t)-1.0));
	posCenter->dY = ((-p2.dY + sin(t)*p1.dX + cos(t)*p1.dY)/(cos(t) - 1.0) - sin(t)/(cos(t) -1.0)*(-p2.dX + cos(t)*p1.dX - sin(t)*p1.dY)/(cos(t) - 1.0))/(1+sin(t)*sin(t)/(cos(t)-1.0)/(cos(t)-1.0));
}

void CVisionCalibDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_KeyTimerID)
	{
		/*
		if (m_bOnStillContinue == TRUE)
		{
			//OnStillImage();
			m_plnkVision->HaltVideo(m_iTargetCamNo);
			m_bIsOnLive = FALSE;
						
			m_plnkVision->ClearOverlay(m_iTargetCamNo);
			int iResult = m_plnkVision->Grab(m_iTargetCamNo);
			if (iResult)
			{
				///if(MyMessageBox(_T("Live ON:YES    Live OFF:NO"), _T("Select"), M_ICONQUESTION|M_YESNO) == IDNO)
					m_bOnStillContinue = FALSE;
				// jdy sesl		MyMessageBox(iResult, "Vision Error", M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
			
			iResult = m_plnkVision->GrabWait(m_iTargetCamNo);
			if (iResult)
			{
				///if(MyMessageBox(_T("Live ON:YES    Live OFF:NO"), _T("Select"), M_ICONQUESTION|M_YESNO) == IDNO)
					m_bOnStillContinue = FALSE;
				// jdy sesl		MyMessageBox(iResult, "Vision Error", M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
			
		}
		*/
	}
	CDialog::OnTimer(nIDEvent);
}

void CVisionCalibDlg::setWorkGroup()
{
	switch (m_iTargetCamNo)
	{
	case DEF_F_TABALIGNER_CAMERA_1:
	case DEF_F_TABALIGNER_CAMERA_2:
	case DEF_INSPECTION_CAMERA_1:
	case DEF_INSPECTION_CAMERA_2:
	case DEF_INSPECTION_CAMERA_3:
	case DEF_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_INSPECTION_CAMERA_5:
	case DEF_INSPECTION_CAMERA_6:
	case DEF_INSPECTION_CAMERA_7:
	case DEF_INSPECTION_CAMERA_8:
#endif
		m_iWorkGroup = DEF_FRONT_GROUP;
		break;
	case DEF_R_TABALIGNER_CAMERA_1:
	case DEF_R_TABALIGNER_CAMERA_2:
	case DEF_R_INSPECTION_CAMERA_1:
	case DEF_R_INSPECTION_CAMERA_2:
	case DEF_R_INSPECTION_CAMERA_3:
	case DEF_R_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_R_INSPECTION_CAMERA_5:
	case DEF_R_INSPECTION_CAMERA_6:
	case DEF_R_INSPECTION_CAMERA_7:
	case DEF_R_INSPECTION_CAMERA_8:
#endif
		m_iWorkGroup = DEF_REAR_GROUP;
		break;
	}
}

void CVisionCalibDlg::OnClickFixCoordTabcarrier() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];

	swprintf(cBeforeVal, _T("1"));
	
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal, _T("고정좌표를 구할 TabCarrier번호를 선택하세요.")))
		return;
	int iCarrierNo = _wtoi(cAfterVal);
	iCarrierNo -= 1;
	
	if (iCarrierNo < 0) iCarrierNo = 0;
	if (iCarrierNo >= DEF_MAX_TABCARRIER) iCarrierNo = DEF_MAX_TABCARRIER-1;

	if (m_iWorkGroup == DEF_FRONT_GROUP)
		swprintf(cAfterVal, _T("Front Group %d번 Carrier의 고정좌표 구하기 동작을 시작하겠습니까?"), iCarrierNo+1);
	else
		swprintf(cAfterVal, _T("Rear Group %d번 Carrier의 고정좌표 구하기 동작을 시작하겠습니까?"), iCarrierNo+1);

	if (MyMessageBox(cAfterVal, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	MTabCarrier*		pTabCarrier = NULL;
	MCtrlTabCarrier*	pCtrlTabCarrier = NULL;
	MManageOpPanel* plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
	int iOldJogID = plnkManageOpPanel->GetJogUnit();
	int iJogID = 0;
	switch (iCarrierNo)
	{
	case DEF_TABCARRIER1:
	case DEF_TABCARRIER2:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_TABCARRIER3:
	case DEF_TABCARRIER4:
#endif
		if (m_iWorkGroup == DEF_FRONT_GROUP)
		{
			pTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iCarrierNo);
			pCtrlTabCarrier = MOlbSystemPre.GetCtrlTabCarrierComponent(iCarrierNo);
			iJogID = DEF_JOG_F_TABCARRIER1_XYT + iCarrierNo;
		}
		else
		{
			pTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(iCarrierNo);
			pCtrlTabCarrier = MOlbSystemPre.GetCtrlRTabCarrierComponent(iCarrierNo);
			iJogID = DEF_JOG_R_TABCARRIER1_XYT + iCarrierNo;
		}
		break;
	default:
		break;
	}

	if (pTabCarrier == NULL || pCtrlTabCarrier == NULL)
	{
		MyMessageBox(_T("The TabCarrier number you selected is not valid.\r\n선택하신 TabCarrier번호가 유효하지 않습니다."), _T("Error"));
		return;
	}

	if (FALSE == pTabCarrier->IsExpandCamera())
	{
		MyMessageBox(_T("Please expand the camera and obtain the fixed coordinates.\r\nCamera를 확장한 상태로 고정좌표를 구하여 주세요."), _T("Confirm"));
		return;
	}

	int iCamNo = 0, iCamNoOther = 0;
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
		iCamNo = DEF_F_TABALIGNER_CAMERA_1;
		iCamNoOther = DEF_F_TABALIGNER_CAMERA_2;
	}
	else
	{
		iCamNo = DEF_R_TABALIGNER_CAMERA_1;
		iCamNoOther = DEF_R_TABALIGNER_CAMERA_2;
	}	
	//===============================================================
	int			iResult = ERR_VISION_SUCCESS;
	double		dPixelX = 0.0;
	double		dPixelY = 0.0;
	double		dDistanceXY = 0.0;
	double		dDistanceX = 0.0;
	double		dDistanceY = 0.0;
	double		dRadius = 0.0;
	double		dDistX = 0.0;
	double		dTabLen = m_plnkPreBonderData->m_dLead_Width_Xtab;
#ifdef DEF_GATE_SYSTEM
	dTabLen = m_plnkPreBonderData->m_dLead_Width_Ytab;
#endif

	MPos_XYT	posFirstMark;		// 좌표를 저장한다.
	MPos_XYT	posSecondMark;		// 좌표를 저장한다.
	MPos_XYT	posThirdMark;		//
	MPos_XYT	posFirst;			// 초기 좌표를 저장한다.
	MPos_XYT	posSecond;			// TAngle도 회전한후의 좌표를 저장한다.
	MPos_XYT	posThird;			// Camera2번 Mark인식시 좌표
	MPos_XYT	posCenter;			// Cneter
	MPos_XYT	posMove;

	/**  1번째 Mark를 인식한다--------------------------------*/
	posFirst = pTabCarrier->GetCurrentXYTPos();

	m_plnkVision->ClearOverlay(iCamNo);
	
	iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, DEF_CALIBRATION_MARK1);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, DEF_CALIBRATION_MARK1);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, DEF_CALIBRATION_MARK1);

		/** 1번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkCalibration->ConvertV2R(m_iTargetCamNo, 
											0,
											dPixelX, dPixelY,
											posFirstMark.dX, posFirstMark.dY);
	}
	else
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	// Stage 축을 dTurnAngle 만큼 회전하고, XY를 움직여 Mark를 움직여 화면내에 들어오게 한다.
	dDistanceX = MOlbSystemPre.GetDistance_TabAlignerCenterToTabMarkX();
	dDistanceY = MOlbSystemPre.GetDistance_TabAlignerCenterToTabMarkY();

	double dTAngle = 1.5;
	Rotation(dTAngle, dDistanceX, dDistanceY, &posMove.dX, &posMove.dY);
	
	posMove.dX = dDistanceX - posMove.dX;
	posMove.dY = dDistanceY - posMove.dY;

	posMove.dX = posFirst.dX + posMove.dX;
	posMove.dY = posFirst.dY + posMove.dY;
	posMove.dT = posFirst.dT + dTAngle;

	iResult = pTabCarrier->MoveXYTPos(posMove, -1);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	plnkManageOpPanel->SetJogUnit(iJogID);

	/** Display Vision */
	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iCamNo);

	//if (MyMessageBox("Jog를 이용하여 Mark가 Vision 화면에 들어오게 하고 OK 버튼을 눌러 주세요.",
	if (MyMessageBox(MY_MESSAGE_BOX, 63523, _T("Confirm"), M_YESNO|M_ICONQUESTION) == IDNO)
	{
		plnkManageOpPanel->SetJogUnit(iOldJogID);
		MyMessageBox(MY_MESSAGE_BOX, 63525, _T("Information"));
		return;
	}


	/**  2번째 Mark를 인식한다--------------------------------*/
	posSecond = pTabCarrier->GetCurrentXYTPos();
	stillImage(iCamNo);
	m_plnkVision->ClearOverlay(iCamNo);
	iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
//		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		plnkManageOpPanel->SetJogUnit(iOldJogID);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, DEF_CALIBRATION_MARK1);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, DEF_CALIBRATION_MARK1);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, DEF_CALIBRATION_MARK1);

		/** 2번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkCalibration->ConvertV2R(iCamNo, 
											0,
											dPixelX, dPixelY,
											posSecondMark.dX, posSecondMark.dY);
	}
	else
	{
		plnkManageOpPanel->SetJogUnit(iOldJogID);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	/** 고정좌표 계산 시작  */
	posFirst	= posFirst	- posFirstMark;
	posSecond	= posSecond - posSecondMark;

	/** Fisrt Position에서 Second Position까지의 거리를 구한다. */
	dDistanceX	= (posFirst.dX - posSecond.dX);
	dDistanceY	= (posFirst.dY - posSecond.dY);
	dDistanceXY	= sqrt(dDistanceX * dDistanceX + dDistanceY * dDistanceY);

	/** Camera 중심에서 First (Second) Position까지의 거리를 구한다.. */
	dRadius		= dDistanceXY / (sqrt(2 * (1 - cos(DEG2RAD(dTAngle)))));

	/** First Position과 Second Position을 지나고 반지름 dRadius를 가지는 원의 중심을 구한다. */
	FindCenter(dTAngle, dRadius, posFirst, posSecond, &posCenter);	

	
	m_plnkVision->ClearOverlay(iCamNoOther);
	m_plnkVision->LiveVideo(iCamNoOther);
	//if (MyMessageBox("Jog를 이용하여 Mark가 우측 Camera Vision 화면에 들어오게 하고 확인 버튼을 눌러 주세요.",
	if (MyMessageBox(MY_MESSAGE_BOX, 63524, _T("Confirm"), M_YESNO|M_ICONQUESTION) == IDNO)
	{
		plnkManageOpPanel->SetJogUnit(iOldJogID);
		MyMessageBox(MY_MESSAGE_BOX, 63525, _T("Information"));
		return;

	}

	posThird = pTabCarrier->GetCurrentXYTPos();

	stillImage(iCamNoOther);
	m_plnkVision->ClearOverlay(iCamNoOther);
	iResult = m_plnkVision->Grab(iCamNoOther);
	if (iResult)
	{
		plnkManageOpPanel->SetJogUnit(iOldJogID);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkVision->RecognitionPatternMark(iCamNoOther, DEF_CALIBRATION_MARK1);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNoOther, DEF_CALIBRATION_MARK1);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNoOther, DEF_CALIBRATION_MARK1);

		/** 2번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkCalibration->ConvertV2R(iCamNoOther, 
											0,
											dPixelX, dPixelY,
											posThirdMark.dX, posThirdMark.dY);
	}
	else
	{
		plnkManageOpPanel->SetJogUnit(iOldJogID);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	posThird = posThird - posThirdMark;	
//@	dDistX = posThird.dX - posSecond.dX;
	posCenter.dX += ((posThird.dX - posSecond.dX)/2.0);
	posCenter.dY += ((posThird.dY - posSecond.dY)/2.0);

	iResult = pTabCarrier->MoveXYTPos(posFirst, -1);
	if (iResult)
	{
		plnkManageOpPanel->SetJogUnit(iOldJogID);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	plnkManageOpPanel->SetJogUnit(iOldJogID);
	
	m_plnkVision->ClearOverlay(iCamNoOther);
	m_plnkVision->LiveVideo(iCamNoOther);

	CString strResult;
	strResult.Format(_T("[Result:X=%.3lf, Y=%.3lf, CamDist=%.3lf]"), posCenter.dX, posCenter.dY, fabs(posThird.dY - posSecond.dY));
	CString strTemp;
	strTemp.Format(_T("Do you want to reflect the result% s in fixed coordinates?\r\n결과값 %s 을 고정좌표에 반영하겠습니까?"), strResult, strResult);
	if (MyMessageBox(strTemp, _T("Question"), M_YESNO|M_ICONQUESTION) == IDNO)
		return;

	MPos_XYT rgFixedPos[DEF_TABCARRIER_MAX_POSITION];
	pTabCarrier->GetXYTPosParameter(rgFixedPos, NULL, NULL);

	rgFixedPos[DEF_TABCARRIER_PREALIGN_POS].dX = posCenter.dX;
	rgFixedPos[DEF_TABCARRIER_PREALIGN_POS].dY = posCenter.dY;
	rgFixedPos[DEF_TABCARRIER_PREALIGN_POS].dT = 0.0;

	pTabCarrier->SaveXYTFixedPosParameter(rgFixedPos);
	//===============================================================
}
