// TeachHandlerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DefSystem.h"
#include "MPlatformOlbSystem.h"
#include "TeachHandlerDlg.h"
#include "VisionCalibDlg.h"
#include "VisionMonitorDlg.h"
#include "MManageOpPanel.h"
#include "MPanelTransfer.h"
#include "MTrsAutoManager.h"

#include "TeachMainForm.h"

//#include "MWorkBench.h"

extern MPlatformOlbSystem	MOlbSystemPre;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CTeachHandlerDlg dialog

CTeachHandlerDlg::CTeachHandlerDlg(int iHandlerType, int iFixModel, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachHandlerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeachHandlerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_bZPosSelected = FALSE;
 
	m_iHandlerID = iHandlerType;
	m_iFixModel = iFixModel;
	m_iSelectedPos = 0;
	m_iSelectedZPos = 0;
	m_iSelRealPos = 0;
	m_iJogSelect = -1;
	m_bOnCheck = FALSE;

	m_dDisplayOffsetZ = 0.0;

	m_iHandlerID = DEF_HANDLER_UNLOADING;
//@	if (m_iHandlerID == DEF_HANDLER_LOADING)
//@		m_plnkHandler = MOlbSystemPre.GetLoadHandlerComponent();
//@	else
#ifdef DEF_USE_TURN_HANDLER
		m_plnkHandler = MOlbSystemPre.GetTurnHandlerComponent();
#endif

//@	m_plnkStage			= MOlbSystemPre.GetStageComponent();
	//m_plnkWorkBench		= MOlbSystemPre.GetWorkBenchComponent();
	m_plnkC_MngOpPanel	= MOlbSystemPre.GetManageOpPanelComponent();
	m_plnkSystemData	= MOlbSystemPre.GetSystemData();
	m_plnkPanelData		= MOlbSystemPre.GetPanelData();
//@	m_plnkAcfData		= MOlbSystemPre.GetAcfData();
	m_plnkIO			= MOlbSystemPre.GetIOComponent();
	
	/** Handler에서 좌표 얻어오기 */
	m_plnkHandler->GetXYTPosParameter(m_rgXYTFixedPosData, m_rgXYTOffsetPosData, m_rgXYTModelPosData);
	m_plnkHandler->GetZPosParameter(m_rgdZFixedPosData, m_rgdZOffsetPosData, NULL);
	// 060824 kjw 데이타를 왜 로딩하나?
	m_plnkHandler->calculateHandlerXYTModelPosParameter();	
//	m_plnkHandler->LoadXYTFixedPosParameter();
	SHandlerData sHandlerData;	
	m_plnkHandler->GetData(&sHandlerData);
}

void CTeachHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachHandlerDlg)
	DDX_Control(pDX, IDS_VISION, m_ctrlLblVision);
	DDX_Control(pDX, IDB_VISION_CALIB, m_ctrlBtnVisionCalib);
	DDX_Control(pDX, IDB_REGISTER_MARK, m_ctrlBtnRegisterMark);
	DDX_Control(pDX, IDB_CAMERA1, m_ctrlBtnCamera1);
	DDX_Control(pDX, IDB_CAMERA2, m_ctrlBtnCamera2);
	DDX_Control(pDX, IDB_X, m_ctrlBtnAxis[eAxisX]);
	DDX_Control(pDX, IDB_Y, m_ctrlBtnAxis[eAxisY]);
	DDX_Control(pDX, IDB_T, m_ctrlBtnAxis[eAxisT]);
	DDX_Control(pDX, IDB_Z, m_ctrlBtnAxis[eAxisZ]);
	
	//DDX_Control(pDX, IDC_GRD_AXISX_SNS_STS, m_grd_AxisSnsState[eAxisX]);
	//DDX_Control(pDX, IDC_GRD_AXISY_SNS_STS, m_grd_AxisSnsState[eAxisY]);
	//DDX_Control(pDX, IDC_GRD_AXIST_SNS_STS, m_grd_AxisSnsState[eAxisT]);
	//DDX_Control(pDX, IDC_GRD_AXISZ_SNS_STS, m_grd_AxisSnsState[eAxisZ]);	
	
	DDX_Control(pDX, IDB_Z_SELECT, m_ctrlBtnZSelect);
	DDX_Control(pDX, IDB_JOG, m_ctrlBtnJog);
	DDX_Control(pDX, IDB_SAVE, m_ctrlBtnSave);
	DDX_Control(pDX, IDB_EXIT, m_ctrlBtnCancel);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDB_CM_MOVE,		m_BtnCmd[0]);
	DDX_Control(pDX, IDB_CM_RESERVED0,	m_BtnCmd[1]);
	DDX_Control(pDX, IDB_POS_SET,		m_BtnCmd[2]);
	DDX_Control(pDX, IDB_CM_RESERVED1,	m_BtnCmd[3]);
	
	DDX_Control(pDX, IDB_HANDLER_POS0, m_BtnPos[0]);
	DDX_Control(pDX, IDB_HANDLER_POS1, m_BtnPos[1]);
	DDX_Control(pDX, IDB_HANDLER_POS2, m_BtnPos[2]);
	DDX_Control(pDX, IDB_HANDLER_POS3, m_BtnPos[3]);
	DDX_Control(pDX, IDB_HANDLER_POS4, m_BtnPos[4]);
	DDX_Control(pDX, IDB_HANDLER_POS5, m_BtnPos[5]);
	DDX_Control(pDX, IDB_HANDLER_POS6, m_BtnPos[6]);
	DDX_Control(pDX, IDB_HANDLER_POS7, m_BtnPos[7]);
	DDX_Control(pDX, IDB_HANDLER_POS8, m_BtnPos[8]);
	DDX_Control(pDX, IDB_HANDLER_POS9, m_BtnPos[9]);

	DDX_Control(pDX, IDC_TGT_POS_X,		m_sCoord[0][0]);
	DDX_Control(pDX, IDC_TGT_POS_Y,		m_sCoord[0][1]);
	DDX_Control(pDX, IDC_TGT_POS_T,		m_sCoord[0][2]);
	DDX_Control(pDX, IDC_TGT_POS_Z,		m_sCoord[0][3]);

	DDX_Control(pDX, IDC_FIX_POS_X,		m_sCoord[1][0]);
	DDX_Control(pDX, IDC_FIX_POS_Y,		m_sCoord[1][1]);
	DDX_Control(pDX, IDC_FIX_POS_T,		m_sCoord[1][2]);
	DDX_Control(pDX, IDC_FIX_POS_Z,		m_sCoord[1][3]);
	
	DDX_Control(pDX, IDC_MODEL_POS_X,	m_sCoord[2][0]);
	DDX_Control(pDX, IDC_MODEL_POS_Y,	m_sCoord[2][1]);
	DDX_Control(pDX, IDC_MODEL_POS_T,	m_sCoord[2][2]);
	DDX_Control(pDX, IDC_MODEL_POS_Z,	m_sCoord[2][3]);

	DDX_Control(pDX, IDC_OFFSET_POS_X,	m_sCoord[3][0]);
	DDX_Control(pDX, IDC_OFFSET_POS_Y,	m_sCoord[3][1]);
	DDX_Control(pDX, IDC_OFFSET_POS_T,	m_sCoord[3][2]);
	DDX_Control(pDX, IDC_OFFSET_POS_Z,	m_sCoord[3][3]);

	DDX_Control(pDX, IDC_CUR_POS_X,		m_sCoord[4][0]);
	DDX_Control(pDX, IDC_CUR_POS_Y,		m_sCoord[4][1]);
	DDX_Control(pDX, IDC_CUR_POS_T,		m_sCoord[4][2]);
	DDX_Control(pDX, IDC_CUR_POS_Z,		m_sCoord[4][3]);

	DDX_Control(pDX, IDC_DIF_POS_X,		m_sCoord[5][0]);
	DDX_Control(pDX, IDC_DIF_POS_Y,		m_sCoord[5][1]);
	DDX_Control(pDX, IDC_DIF_POS_T,		m_sCoord[5][2]);
	DDX_Control(pDX, IDC_DIF_POS_Z,		m_sCoord[5][3]);

	DDX_Control(pDX, IDB_Z_POS0, m_BtnZPos[0]);
	DDX_Control(pDX, IDB_Z_POS1, m_BtnZPos[1]);
	DDX_Control(pDX, IDB_Z_POS2, m_BtnZPos[2]);
	DDX_Control(pDX, IDB_Z_POS3, m_BtnZPos[3]);

	DDX_Control(pDX, IDC_TGT_POS,		m_sCoordTitle[0]);
	DDX_Control(pDX, IDC_FIX_POS,		m_sCoordTitle[1]);
	DDX_Control(pDX, IDC_MODEL_POS,		m_sCoordTitle[2]);
	DDX_Control(pDX, IDC_OFFSET_POS,	m_sCoordTitle[3]);
	DDX_Control(pDX, IDC_CUR_POS,		m_sCoordTitle[4]);
	DDX_Control(pDX, IDC_DIF_POS,		m_sCoordTitle[5]);
}


BEGIN_MESSAGE_MAP(CTeachHandlerDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachHandlerDlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachHandlerDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachHandlerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachHandlerDlg)
	ON_EVENT(CTeachHandlerDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS0, -600 /* Click */, OnClickPtuPos0, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS1, -600 /* Click */, OnClickPtuPos1, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS2, -600 /* Click */, OnClickPtuPos2, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS3, -600 /* Click */, OnClickPtuPos3, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS4, -600 /* Click */, OnClickHandlerPos4, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS5, -600 /* Click */, OnClickHandlerPos5, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS6, -600 /* Click */, OnClickHandlerPos6, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS7, -600 /* Click */, OnClickHandlerPos7, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS8, -600 /* Click */, OnClickHandlerPos8, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_HANDLER_POS9, -600 /* Click */, OnClickHandlerPos9, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_CM_MOVE, -600 /* Click */, OnClickCmMove, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_POS_SET, -600 /* Click */, OnClickPosSet, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_CM_RESERVED0, -600 /* Click */, OnClickCmReserved0, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_CM_RESERVED1, -600 /* Click */, OnClickCmReserved1, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_CAMERA1, -600 /* Click */, OnClickCamera1, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_CAMERA2, -600 /* Click */, OnClickCamera2, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_VISION_CALIB, -600 /* Click */, OnClickVisionCalib, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_REGISTER_MARK, -600 /* Click */, OnClickRegisterMark, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_X, -600 /* Click */, OnClickX, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_Y, -600 /* Click */, OnClickY, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_T, -600 /* Click */, OnClickT, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_Z, -600 /* Click */, OnClickZ, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_Z_SELECT, -600 /* Click */, OnClickZSelect, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_Z_POS0, -600 /* Click */, OnClickZPos0, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_Z_POS1, -600 /* Click */, OnClickZPos1, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_Z_POS2, -600 /* Click */, OnClickZPos2, VTS_NONE)
	ON_EVENT(CTeachHandlerDlg, IDB_Z_POS3, -600 /* Click */, OnClickZPos3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachHandlerDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(),&m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTeachHandlerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_bOnCheck = FALSE;
	// TODO: Add extra initialization here
	// SJ_Chi 언어별 Display
	if (m_plnkSystemData->m_uiLanguageSelect == LANGUAGE_KOREAN)
	{
		if ( m_iFixModel == DEF_MODEL_MODE )
			SetWindowText(_T("Handler 모델좌표 Teaching"));
		else // m_iFixModel == DEF_FIX_MODE
			SetWindowText(_T("Handler 고정좌표 Teaching"));
	}
	else
	{
		if ( m_iFixModel == DEF_MODEL_MODE )
			SetWindowText(_T("Handler Model Coord Teaching"));		
		else // m_iFixModel == DEF_FIX_MODE
			SetWindowText(_T("Handler Fixed Coord Teaching"));

		m_ctrlBtnJog.SetCaption(_T("Jog Axis"));
		m_ctrlBtnAxis[eAxisX].SetCaption(_T("X Axis"));
		m_ctrlBtnAxis[eAxisY].SetCaption(_T("Y Axis"));	
		m_ctrlBtnAxis[eAxisT].SetCaption(_T("T Axis"));	
		m_ctrlBtnAxis[eAxisZ].SetCaption(_T("Z Axis"));
		m_sCoordTitle[0].SetCaption(_T("Target Pos"));
		m_sCoordTitle[1].SetCaption(_T("Fixed Pos"));
		m_sCoordTitle[2].SetCaption(_T("Model Pos"));
		m_sCoordTitle[3].SetCaption(_T("Offset Pos"));
		m_sCoordTitle[4].SetCaption(_T("Current Pos"));
		m_sCoordTitle[5].SetCaption(_T("Diff. Pos"));

		m_BtnCmd[0].SetCaption(_T("Move to\nTgt. Pos"));
		m_BtnCmd[2].SetCaption(_T("Change\nCurrent Pos\nto Target Pos"));
		m_ctrlBtnZSelect.SetCaption(_T("Z Axis\nSelect"));

		m_ctrlBtnSave.SetCaption(_T("Save"));
		m_ctrlBtnCancel.SetCaption(_T("Cancel"));
	}
	
	/** Teaching Position Name Reading (XYT 축) */
	int iTemp = 0;
	int iSelected = -1;
	STeachingInfo sTeachingInfo = m_plnkHandler->GetXYTTeachingInfo();
	for (int i = 0; i < DEF_HANDLER_TEACH_MAX_POS; i++)
	{
		if ((sTeachingInfo.m_sPosition[i]).m_bUse == TRUE)
		{
			m_BtnPos[iTemp].SetWindowText((LPCTSTR)(sTeachingInfo.m_sPosition[i].m_strNewLineName[0]));
			m_rgiIndexMapping[iTemp] = i;
			
			if (sTeachingInfo.m_sPosition[i].m_bEnable == FALSE)
				m_BtnPos[iTemp].EnableWindow(FALSE);
			
			// 설정에 따라 Button Disable
			disabledPositionIndex(i, iTemp);
			iTemp++;
			if (iSelected == -1 && sTeachingInfo.m_sPosition[i].m_bEnable == TRUE)
				iSelected = i;
		}
	}

	// 사용하지 않는 Position Button 감추기
	for (int i = iTemp; i < DEF_HANDLER_TEACH_MAX_POS_BTN; i++)
		m_BtnPos[i].ShowWindow(SW_HIDE);

	// 기본 Select Position 설정 하기
	if (iSelected != -1)
		selectXYTPosition(iSelected);
	
	/** Teaching Position Name Reading (Z 축) */
	sTeachingInfo = m_plnkHandler->GetZTeachingInfo();
	for (int i = 0; i < DEF_HANDLER_MAX_Z_POS; i++)
	{
		m_BtnZPos[i].SetWindowText((LPCTSTR)(sTeachingInfo.m_sPosition[i].m_strNewLineName[0]));
		m_BtnZPos[i].EnableWindow(FALSE);
	}

	// 사용하지 않는 Position Button 감추기
	for (int i = DEF_HANDLER_MAX_Z_POS; i < DEF_HANDLER_TEACH_MAX_Z_POS_BTN; i++)
		m_BtnZPos[i].ShowWindow(SW_HIDE);
	

    //HAHA
//@	if (m_iHandlerID == DEF_LOAD_HANDLER_TEACH)
//@		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_LOAD_HANDLER);
//@	else if (m_iHandlerID == DEF_UNLOAD_HANDLER_TEACH)
#ifdef DEF_USE_TURN_HANDLER
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_HANDLER_XT);
#endif
	m_ctrlBtnAxis[eAxisX].EnableWindow(FALSE);
	m_sCoord[0][0].EnableWindow(FALSE);
	m_sCoord[1][0].EnableWindow(FALSE);
	m_sCoord[2][0].EnableWindow(FALSE);
	m_sCoord[3][0].EnableWindow(FALSE);
	m_sCoord[4][0].EnableWindow(FALSE);
	m_sCoord[5][0].EnableWindow(FALSE);

	m_ctrlBtnAxis[eAxisY].EnableWindow(FALSE);
	m_sCoord[0][1].EnableWindow(FALSE);
	m_sCoord[1][1].EnableWindow(FALSE);
	m_sCoord[2][1].EnableWindow(FALSE);
	m_sCoord[3][1].EnableWindow(FALSE);
	m_sCoord[4][1].EnableWindow(FALSE);
	m_sCoord[5][1].EnableWindow(FALSE);

	m_ctrlBtnAxis[eAxisZ].EnableWindow(FALSE);	

	updateCoordinate();
	updateZCoordinate();

	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);

	m_uiTimerID = SetTimer(1, 300, NULL);
	
	// Doolin kwangilshin 2017. 08. 24.
	//
	Init_GridAxisStste();
	// End.


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachHandlerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
#ifndef SIMULATION
	m_plnkC_MngOpPanel->SetJogUnit(-1);
#endif /* SIMULATION */
}

void CTeachHandlerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		updateDisplay();
		updateZDisplay();
		updateButton();

		// Doolin kwangilshin 2017. 08. 25.
		//
		updateAxisSnsSts();
		// End.
	}

	CDialog::OnTimer(nIDEvent);
}

void CTeachHandlerDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	m_plnkC_MngOpPanel->SetJogUnit();

	OnOK();
}

/**
 * 목표위치, Mark Offset, 기타보정, 현재위치, 편차 Display
 */
void CTeachHandlerDlg::updateDisplay()
{
//@	char szTemp[250];
	TCHAR szTemp[250];

	MPos_XYT sTargetPos;
	MPos_XYT sPosCurr;	
	
	double rgdFixedPosData[3];
	double rgdModelPosData[3];
	double rgdOffsetPosData[3];
	
	double rgdTargetPos[3];
	double rgdCurrentPos[3];

	m_rgXYTFixedPosData[m_iSelRealPos].TransToArray(rgdFixedPosData);
	m_rgXYTModelPosData[m_iSelRealPos].TransToArray(rgdModelPosData);
	m_rgXYTOffsetPosData[m_iSelRealPos].TransToArray(rgdOffsetPosData);
	
	/** 목표위치 계산하기 (목표 위치 = 고정좌표 + 모델좌표 + Offset좌표) */
	sTargetPos.dX =	rgdFixedPosData[0] + rgdModelPosData[0] + rgdOffsetPosData[0];
	sTargetPos.dY =	rgdFixedPosData[1] + rgdModelPosData[1] + rgdOffsetPosData[1];
	sTargetPos.dT =	rgdFixedPosData[2] + rgdModelPosData[2] + rgdOffsetPosData[2];

	sTargetPos.TransToArray(rgdTargetPos);

//@	swprintf(szTemp, "%.3f", rgdTargetPos[0]);
	swprintf(szTemp, _T("%.3f"), rgdTargetPos[0]);
	m_sCoord[0][0].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdTargetPos[1]);
	m_sCoord[0][1].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdTargetPos[2]);
	m_sCoord[0][2].SetCaption(szTemp);

#ifndef SIMULATION
	/** 현재 위치 읽어오기 */
	sPosCurr = m_plnkHandler->GetXYTCurrentPos();
#endif /* SIMULATION */
	sPosCurr.TransToArray(rgdCurrentPos);

	// Reset Switch 감지시 현재 위치를 0으로 표시되게 하여 
	// Teaching시 상대거리 측정을 쉽게 한다.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
	{
		m_sDisplayOffsetXYT = sPosCurr;
	}

	sPosCurr = sPosCurr - m_sDisplayOffsetXYT;
	sPosCurr.TransToArray(rgdCurrentPos);

	/** 현재 위치 Display */
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[0]);
	m_sCoord[4][0].SetCaption(szTemp);
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[1]);
	m_sCoord[4][1].SetCaption(szTemp);
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[2]);
	m_sCoord[4][2].SetCaption(szTemp);
	
	/** 편차 계산하고, Display */
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[0] - rgdTargetPos[0]);
	m_sCoord[5][0].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[1] - rgdTargetPos[1]);
	m_sCoord[5][1].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[2] - rgdTargetPos[2]);
	m_sCoord[5][2].SetCaption(szTemp);
}

/**
 * 목표위치, Mark Offset, 기타보정, 현재위치, 편차 Display (Z축)
 */
void CTeachHandlerDlg::updateZDisplay()
{
//@	char szTemp[250];
	TCHAR szTemp[250];
	double dCurrentZPos = 0.0;
	
	double dTargetZPos = m_rgdZFixedPosData[m_iSelectedZPos] + m_rgdZOffsetPosData[m_iSelectedZPos];
	
	/** 목표 위치 */
	swprintf(szTemp, _T("%.3f"), dTargetZPos);	
	m_sCoord[0][3].SetCaption(szTemp);	

#ifndef SIMULATION
	/** 현재 위치 읽고, Display 하기 */
	dCurrentZPos = m_plnkHandler->GetZCurrentPos();
#endif /* SIMULATION */

	// Reset Switch 감지시 현재 위치를 0으로 표시되게 하여 
	// Teaching시 상대거리 측정을 쉽게 한다.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
	{
		m_dDisplayOffsetZ = dCurrentZPos;
	}
	
	dCurrentZPos = dCurrentZPos - m_dDisplayOffsetZ;	

	swprintf(szTemp, _T("%.3f"), dCurrentZPos);
	m_sCoord[4][3].SetCaption(szTemp);
	
	/** 편차 계산하기 */
	swprintf(szTemp, _T("%.3f"),  dCurrentZPos - dTargetZPos);
	m_sCoord[5][3].SetCaption(szTemp);
}

/**
 * 고정좌표, Offset좌표 Display
 */
void CTeachHandlerDlg::updateCoordinate()
{
	TCHAR szBuff[250];
	memset(szBuff, 0x00, sizeof(szBuff));

	double rgdFixedPosData[DEF_HANDLER_MAX_POS];
	double rgdModelPosData[DEF_HANDLER_MAX_POS];
	double rgdOffsetPosData[DEF_HANDLER_MAX_POS];

	m_rgXYTFixedPosData[m_iSelRealPos].TransToArray(rgdFixedPosData);
	m_rgXYTModelPosData[m_iSelRealPos].TransToArray(rgdModelPosData);
	m_rgXYTOffsetPosData[m_iSelRealPos].TransToArray(rgdOffsetPosData);

	for (int i = 0; i < DEF_HANDLER_TEACH_XYT_AXIS_TITLE; i++)
	{
		swprintf(szBuff, _T("%.3f"), rgdFixedPosData[i]);
		/** 고정좌표 화면 Display */
		m_sCoord[1][i].SetWindowText(szBuff);

		swprintf(szBuff, _T("%.3f"), rgdModelPosData[i]);
		/** 모델좌표 화면 Display */
		m_sCoord[2][i].SetWindowText(szBuff);

		/** 옵셋좌표 화면 Display */
		swprintf(szBuff, _T("%.3f"), rgdOffsetPosData[i]);
		m_sCoord[3][i].SetWindowText(szBuff);
	}

	updateDisplay();
}

/**
 * 고정좌표, 모델좌표, Offset좌표 Display (Z축)
 */
void CTeachHandlerDlg::updateZCoordinate()
{
	TCHAR szTemp[250];
	/** 고정좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdZFixedPosData[m_iSelectedZPos]);
	m_sCoord[1][3].SetCaption(szTemp);
	/** 모델좌표 화면 Display */
	m_sCoord[2][3].SetCaption(_T("0.000"));
	/** 옵셋좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdZOffsetPosData[m_iSelectedZPos]);
	m_sCoord[3][3].SetCaption(szTemp);

	for (int i = 0; i < DEF_HANDLER_TEACH_MAX_COORDINATE_TYPE; i++)
	{
		if (!m_bZPosSelected)
			m_sCoord[i][3].SetCaption(_T("-"));
		
		m_sCoord[i][3].EnableWindow(m_bZPosSelected);
	}
	updateZDisplay();
}

/**
 * 선택 Position 변경 (XYT축)
 */
void CTeachHandlerDlg::selectXYTPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelRealPos = m_rgiIndexMapping[iPosIndex];
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	updateCoordinate();
}

/**
 * Command Button 상태 update
 */
void CTeachHandlerDlg::updateButton()
{
	if (!m_bOnCheck && m_plnkHandler->IsAbsorbPanel())
	{
		m_BtnCmd[3].SetBackColorInterior(YELLOW);//SetBackColor(DEF_ON_COLOR);	//SetBackColorInterior(YELLOW);
		m_bOnCheck = TRUE;
	}
	if (m_bOnCheck && !m_plnkHandler->IsAbsorbPanel())
	{
		m_BtnCmd[3].SetBackColorInterior(BASIC); //SetBackColor(DEF_OFF_COLOR);	//SetBackColorInterior(BASIC);
		m_bOnCheck = FALSE;
	}
}


void CTeachHandlerDlg::disabledPositionIndex(int iPosIndex, int iTemp)
{
	BOOL bEnable = TRUE;
	switch (iPosIndex)
	{
	case DEF_HANDLER_LOAD_POS:
	case DEF_HANDLER_UNLOAD_POS:
	case DEF_HANDLER_TURN_POS:
	case DEF_HANDLER_MAX_POS:
		bEnable = TRUE; 
		break;
	}

	m_BtnPos[iTemp].EnableWindow(bEnable);
}

// Doolin kwangilshin 2017. 08. 24.
//
void CTeachHandlerDlg::Init_GridAxisStste()
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


BOOL CTeachHandlerDlg::Check_AxisStste(IAxis *pclsAxis)
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


void CTeachHandlerDlg::SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome)
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


void CTeachHandlerDlg::SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachHandlerDlg::SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachHandlerDlg::SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable)
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


void CTeachHandlerDlg::updateAxisSnsSts()
{
	IAxis *pclsAxis[eAxisEnd];
	
	//for (int nAxisCnt = 0; nAxisCnt < eAxisEnd; nAxisCnt++)
	//{
	//	pclsAxis[nAxisCnt] = m_plnkHandler->GetMotionComponent(nAxisCnt);

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


void CTeachHandlerDlg::OnClickPtuPos0() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(0);
}

void CTeachHandlerDlg::OnClickPtuPos1() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(1);
}

void CTeachHandlerDlg::OnClickPtuPos2() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(2);
}

void CTeachHandlerDlg::OnClickPtuPos3() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(3);
}

void CTeachHandlerDlg::OnClickHandlerPos4() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(4);
}

void CTeachHandlerDlg::OnClickHandlerPos5() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(5);
}

void CTeachHandlerDlg::OnClickHandlerPos6() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(6);
}

void CTeachHandlerDlg::OnClickHandlerPos7() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(7);	
}

void CTeachHandlerDlg::OnClickHandlerPos8() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(8);
}

void CTeachHandlerDlg::OnClickHandlerPos9() 
{
	// TODO: Add your control notification handler code here
	selectXYTPosition(9);
}

void CTeachHandlerDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
//@	if (MyMessageBox("Save to the positioni data?\r\n"
//@		"Data를 저장하시겠습니까?", "Saving Data", M_ICONQUESTION|M_YESNO, "Yes", "No") == IDNO)
//@		return;
	if (MyMessageBox(MY_MESSAGE_BOX,50101,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	if (m_iFixModel == DEF_FIX_MODE)
	{
		m_plnkHandler->SaveXYTFixedPosParameter(m_rgXYTFixedPosData);
		m_plnkHandler->SaveZFixedPosParameter(m_rgdZFixedPosData);
	}
	else
	{
		m_plnkHandler->SaveXYTOffsetPosParameter(m_rgXYTOffsetPosData);
		m_plnkHandler->SaveZOffsetPosParameter(m_rgdZOffsetPosData);
	}
}

void CTeachHandlerDlg::OnClickCmMove() 
{
	// TODO: Add your control notification handler code here
#ifndef DEF_SETUP
	if (m_plnkC_MngOpPanel->GetDoorSWStatus())
	{
		CString strError;
		strError = MOlbSystemPre.GetErrorMessage(300003000);
		MyMessageBox(strError.GetBuffer(strError.GetLength()), "Door Open Error", M_ICONERROR);
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

//@	if (MyMessageBox("Move the Handler to the selected position?\r\n"
//@		"선택된 위치로 Handler 를 이동 하시겠습니까?", "Handler Moving", M_ICONQUESTION|M_YESNO, "Yes", "No") == IDNO)
//@		return;

	//	목표위치로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50103,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	
	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Move the Handler to target position.\n")
					_T("Wait a moment...\n")
					_T("Handler 를 목표 위치로 이동 중 입니다.\n")
					_T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();
	
	int iPreviousJogUnit = m_plnkC_MngOpPanel->GetJogUnit();	
	m_plnkC_MngOpPanel->SetJogUnit();
	ProgWnd.StepIt();
	
	int iResult;	
	if (m_bZPosSelected)
	{
		/*
		if (m_plnkWorkBench->IsInArmPos(DEF_WORKBENCH_ARM_UNLOAD_POS)
			&& m_plnkWorkBench->IsInZPos(DEF_WORKBENCH_Z_UP_POS)
			&& m_plnkWorkBench->IsAbsorbPanel()
			&& m_plnkHandler->IsAbsorbPanel()
			&& m_plnkHandler->IsInXYTPos(DEF_HANDLER_LOAD_POS)
			&& m_plnkHandler->IsInZPos(DEF_HANDLER_Z_LOAD_DOWN_POS))
		{
			//@MultiLanguage
			MyMessageBox(_T("Cannot move the Handler because Both of Stage and Load Handler absorb a Panel.\r\n")
				_T("Stage,LHandler모두 Vacuum흡착 상태라 이동 할 수 없습니다."),"Stage Teaching Error", M_ICONERROR);
			return;
		}
		*/

		/*/
		MStage* plnkStage = MOlbSystemAcf.GetStageComponent();
		if (plnkStage->IsInXYTPos(DEF_STAGE_LOAD_POS)
		   && plnkStage->IsInZPos(DEF_STAGE_Z_MOVE_POS)
		   && plnkStage->IsAbsorbPanel()
		   && m_plnkHandler->IsAbsorbPanel()
		   && m_plnkHandler->IsInXYTPos(DEF_HANDLER_UNLOAD_POS)
		   && m_plnkHandler->IsInZPos(DEF_HANDLER_Z_UNLOAD_POS))
		{
			MyMessageBox("Cannot move the Handler because Both of Stage and Load Handler absorb a Panel.\r\n"
				"Stage,LHandler모두 Vacuum흡착 상태라 이동 할 수 없습니다.","Stage Teaching Error", M_ICONERROR);
			return;
		}

		if(m_iHandlerID == DEF_HANDLER_LOADING)
		{
			if(m_plnkHandler->IsInXYTPos(DEF_HANDLER_LOAD_POS)
				&& m_plnkHandler->IsInZPos(DEF_HANDLER_Z_LOAD_POS)
				&& m_plnkHandler->IsAbsorbPanel())
			{
				if(MyMessageBox("전공정 WorkBench와 Load Handler가 Loading 위치에서 동시에 Panel을 흡착하고 있을 수 있습니다. 계속 진행하시겠습니까?.","질문", M_ICONQUESTION|M_YESNO, "예", "아니오") == IDNO)				
					return;
			}
		}
		else
		{
			if(m_plnkHandler->IsInXYTPos(DEF_HANDLER_UNLOAD_POS)
				&& m_plnkHandler->IsInZPos(DEF_HANDLER_Z_UNLOAD_POS)
				&& m_plnkHandler->IsAbsorbPanel())
			{
				if(MyMessageBox("Unload Handler is absorbing a Panel on the unloading position. Move up the Handler?\r\n"
					//"UHandler가 UnLoading 위치에서 Panel을 흡착하고있습니다. 계속 진행하시겠습니까?."
					"다음 설비 Stage가 Panel을 흡착하고 있을 수 있습니다. 계속 진행하시겠습니까?.","Handler Moving", M_ICONQUESTION|M_YESNO, "Yes", "No") == IDNO)
					return;
			}
		}

		if (plnkStage->IsInXYTPos(DEF_STAGE_LOAD_POS)
		   && plnkStage->IsInZPos(DEF_STAGE_Z_MOVE_POS)
		   && plnkStage->IsAbsorbPanel()
		   && m_plnkHandler->IsAbsorbPanel()
		   && m_plnkHandler->IsInXYTPos(DEF_HANDLER_UNLOAD_POS)
		   && m_plnkHandler->IsInZPos(DEF_HANDLER_Z_UNLOAD_POS))
		{
			MyMessageBox("Cannot move the Handler because Both of Stage and Load Handler absorb a Panel.\r\n"
				"Stage,LHandler모두 Vacuum흡착 상태라 이동 할 수 없습니다.", "Stage Teaching Error", M_ICONERROR);
			return;
		}
		MWorkBench* plnkWorkBench = MOlbSystemPre.GetWorkBenchComponent();
		if (plnkWorkBench->IsInArmPos(DEF_WORKBENCH_ARM_UNLOAD_POS)
		   && plnkWorkBench->IsInBodyPos(DEF_WORKBENCH_BODY_UNLOAD_POS)
		   && plnkWorkBench->IsInZPos(DEF_WORKBENCH_Z_UP_POS)
		   && plnkWorkBench->IsAbsorbPanel()
		   && m_plnkHandler->IsAbsorbPanel()
		   && m_plnkHandler->IsInXYTPos(DEF_HANDLER_LOAD_POS)
		   && m_plnkHandler->IsInZPos(DEF_HANDLER_Z_LOAD_POS))
		{
			/ *
			MyMessageBox("WorkBench,LHandler모두 Vacuum흡착 상태라 이동 할 수 없습니다.",
				"Stage Teaching Error", M_ICONERROR);
			return;
			* /
		}
		/*/

		iResult = m_plnkHandler->SafeMoveZPos(m_iSelectedZPos);
	}
	else
		iResult = m_plnkHandler->SafeMoveXYTPos(m_iSelRealPos);
	
	if (iResult != ERR_HANDLER_TEACH_DLG_SUCCESS)
	{
		CString strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()), "Handler Teaching Error", M_ICONERROR);
	}
	ProgWnd.StepIt();

	// Jog 사용 재개
	m_plnkC_MngOpPanel->SetJogUnit(iPreviousJogUnit);
}

void CTeachHandlerDlg::OnClickPosSet() 
{
	// TODO: Add your control notification handler code here
	TCHAR szTemp[250];	
	
//@	if (MyMessageBox("Change the Target position with current position?\r\n"
//@		"목표 위치를 현재 위치로 변경하시겠습니까?", "Change Position", M_ICONQUESTION|M_YESNO, "Yes", "No") == IDNO)
//@		return;
	if (MyMessageBox(MY_MESSAGE_BOX,50105,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Set Target Position..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Changing Target position with current position.\n")
					_T("Wait a moment...\n")
					_T("Handler 의 목표 위치를 현재 위치로 변경 중 입니다.\n")
					_T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	if (m_bZPosSelected == FALSE)
	{
		MPos_XYT sCurrentXYT;
		MPos_XYT sMarkOffset;

		double rgdTargetPos[DEF_HANDLER_MAX_POS];
		double rgdFixedPosData[DEF_HANDLER_MAX_POS];
		double rgdModelPosData[DEF_HANDLER_MAX_POS];
		double rgdOffsetPosData[DEF_HANDLER_MAX_POS];

		sCurrentXYT = m_plnkHandler->GetXYTCurrentPos();
		ProgWnd.StepIt();

		sCurrentXYT.TransToArray(rgdTargetPos);

		m_rgXYTFixedPosData[m_iSelRealPos].TransToArray(rgdFixedPosData);
		m_rgXYTModelPosData[m_iSelRealPos].TransToArray(rgdModelPosData);
		m_rgXYTOffsetPosData[m_iSelRealPos].TransToArray(rgdOffsetPosData);
		
		for (int i = 0; i < DEF_HANDLER_TEACH_XYT_AXIS_TITLE; i++)
		{
			swprintf(szTemp, _T("%.3f"), rgdTargetPos[i]);
			m_sCoord[0][i].SetCaption(szTemp);
		}
		ProgWnd.StepIt();

		switch (m_iFixModel)
		{
		case DEF_MODEL_MODE :
			for (int i = 0; i < DEF_HANDLER_TEACH_XYT_AXIS_TITLE; i++)
			{
				rgdOffsetPosData[i] = rgdTargetPos[i] - rgdModelPosData[i] - rgdFixedPosData[i];
			
				swprintf(szTemp, _T("%.3f"), rgdOffsetPosData[i]);
				m_sCoord[3][i].SetCaption(szTemp);
			}			
			m_rgXYTOffsetPosData[m_iSelRealPos].Init(rgdOffsetPosData[0], rgdOffsetPosData[1], rgdOffsetPosData[2]);
			break;
		
		case DEF_FIX_MODE :
			for (int i = 0; i < DEF_HANDLER_TEACH_XYT_AXIS_TITLE; i++)
			{
				rgdFixedPosData[i] = rgdTargetPos[i] - rgdModelPosData[i] - rgdOffsetPosData[i];
				swprintf(szTemp, _T("%.3f"), rgdFixedPosData[i]);
				m_sCoord[1][i].SetCaption(szTemp);
			}
			m_rgXYTFixedPosData[m_iSelRealPos].Init(rgdFixedPosData[0],rgdFixedPosData[1],rgdFixedPosData[2]);
			break;
		}
		ProgWnd.StepIt();
	}
	else // Z Position 선택
	{
		double dCurrentZ = m_plnkHandler->GetZCurrentPos();
		ProgWnd.StepIt();

		double dTargetPos = dCurrentZ;
		swprintf(szTemp, _T("%.3f"), dTargetPos);
		m_sCoord[0][3].SetCaption(szTemp);
		ProgWnd.StepIt();
	
		switch (m_iFixModel)
		{
		case DEF_MODEL_MODE :	
			m_rgdZOffsetPosData[m_iSelectedZPos] = dTargetPos - m_rgdZFixedPosData[m_iSelectedZPos];
			swprintf(szTemp, _T("%.3f"), m_rgdZOffsetPosData[m_iSelectedZPos]);
			m_sCoord[3][3].SetCaption(szTemp);
			break;
			
		case DEF_FIX_MODE :
			m_rgdZFixedPosData[m_iSelectedZPos] = dTargetPos - m_rgdZOffsetPosData[m_iSelectedZPos];
			swprintf(szTemp, _T("%.3f"), m_rgdZFixedPosData[m_iSelectedZPos]);
			m_sCoord[1][3].SetCaption(szTemp);
			break;
		}
		ProgWnd.StepIt();
	}
}

void CTeachHandlerDlg::OnClickCmReserved0() 
{
	// TODO: Add your control notification handler code here
}

void CTeachHandlerDlg::OnClickCmReserved1() 
{
	// TODO: Add your control notification handler code here
	CMyProgressWnd ProgWnd(NULL, _T("..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T(" acting...\nPlease wait a moment..."));
	ProgWnd.PeekAndPump();
	
	int iResult;
	//if (m_bVacSts == FALSE/*!m_plnkHandler->IsAbsorbPanel()*/)		
	if (!m_plnkHandler->IsAbsorbPanel())		
	{
		iResult = m_plnkHandler->AbsorbPanel(FALSE);
		m_bVacSts = TRUE;
	}
	else
	{
		/*/
		if (!(m_plnkHandler->IsInXYTPos(DEF_HANDLER_LOAD_POS)
		 && m_plnkHandler->IsInZPos(DEF_HANDLER_Z_LOAD_POS))
		 && !(m_plnkHandler->IsInXYTPos(DEF_HANDLER_UNLOAD_POS)
		 && m_plnkHandler->IsInZPos(DEF_HANDLER_Z_UNLOAD_POS)))
		{
			MyMessageBox("Cannot release the Handler Vacuum because it is not in loading and unloading position.\r\n"
				"Handler가 Loading위치도 Unloading 위치도 아니라서 Vacuum Off를 할 수 없습니다.", "Error");
			return;
		}
		/*/

		if (MyMessageBox(_T("Do you want to release the vacuum?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;	

		iResult = m_plnkHandler->ReleasePanel(FALSE);
		m_bVacSts = FALSE;
	}

	if (iResult != SUCCESS)
	{
		CString strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()), "Vacuum Error", M_ICONERROR);
	}
}

void CTeachHandlerDlg::OnClickCamera1() 
{
	// TODO: Add your control notification handler code here
	/*/
	m_plnkIO->OutputOn(OUT_LC2_CHANNEL1_ONOFF);
	Sleep(10);
	m_plnkIO->OutputOn(OUT_LC2_CHANNEL2_ONOFF);
	Sleep(10);
	m_plnkIO->OutputOn(OUT_LC2_CHANNEL3_ONOFF);
	Sleep(10);
	m_plnkIO->OutputOn(OUT_LC2_CHANNEL4_ONOFF);
	
	CVisionMonitorDlg dlg(DEF_BLOB_TYPE, DEF_CAMERA_INSPECTION_1);
	
	dlg.DoModal();	
	/*/
}

void CTeachHandlerDlg::OnClickCamera2() 
{
	// TODO: Add your control notification handler code here
	/*/
	m_plnkIO->OutputOn(OUT_LC2_CHANNEL1_ONOFF);
	Sleep(10);
	m_plnkIO->OutputOn(OUT_LC2_CHANNEL2_ONOFF);
	Sleep(10);
	m_plnkIO->OutputOn(OUT_LC2_CHANNEL3_ONOFF);
	Sleep(10);
	m_plnkIO->OutputOn(OUT_LC2_CHANNEL4_ONOFF);

	CVisionMonitorDlg dlg(DEF_BLOB_TYPE, DEF_CAMERA_INSPECTION_2);

	dlg.DoModal();	
	/*/
}

void CTeachHandlerDlg::OnClickVisionCalib() 
{
	// TODO: Add your control notification handler code here
	/*
	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		MyMessageBox("Dry-Run일 경우는 Vision Calibration을 수행할 수 없습니다","확인",M_ICONINFORMATION|M_OK);
		return;
	}

	CVisionCalibDlg dlg(m_plnkHandler, m_iSelectedPos);
	dlg.DoModal();

	/** Stage에서 좌표 얻어오기 */
/*	m_plnkHandler->GetXYTPosParameter(m_rgXYTFixedPosData, m_rgXYTOffsetPosData, m_rgXYTModelPosData, NULL);
	m_plnkHandler->GetZPosParameter(m_rgdZFixedPosData, m_rgdZOffsetPosData, NULL);

	UpdateDisplay();
	*/
}

void CTeachHandlerDlg::OnClickRegisterMark() 
{
	// TODO: Add your control notification handler code here
/*	m_iCamNo = DEF_CAMERA_INSPECTION_1;
	// Operation Type = DEF_PATTERN_TYPE / DEF_BLOB_TYPE / DEF_CALIPER_TYPE
	CVisionMonitorDlg dlg(DEF_PATTERN_TYPE, m_iCamNo);
	dlg.DoModal();*/
}


void CTeachHandlerDlg::setTargetXYTPosValue(int iAxisID)
{
	TCHAR cAfterVal[100];
	TCHAR cBeforeVal[100];

	m_sCoord[0][iAxisID].GetWindowText(cBeforeVal, 30);

	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkHandler->GetMotionComponent(iAxisID)))
	{
		return;
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	TCHAR szTemp[250];
//@	double dNewTargetPos = atof(cAfterVal);
	double dNewTargetPos = wcstod(cAfterVal, NULL);

	m_sCoord[0][iAxisID].SetCaption(cAfterVal);

	if (m_bZPosSelected == TRUE && iAxisID == DEF_HANDLER_Z_AXIS)
	{
		switch (m_iFixModel)
		{
		case DEF_MODEL_MODE :			
			m_rgdZOffsetPosData[m_iSelectedZPos] = dNewTargetPos - m_rgdZFixedPosData[m_iSelectedZPos];

			swprintf(szTemp, _T("%.3f"), m_rgdZOffsetPosData[m_iSelectedZPos]);
			m_sCoord[3][3].SetCaption(szTemp);
			break;
			
		case DEF_FIX_MODE :
			m_rgdZFixedPosData[m_iSelectedZPos]	= dNewTargetPos - m_rgdZOffsetPosData[m_iSelectedZPos];

			swprintf(szTemp, _T("%.3f"), m_rgdZFixedPosData[m_iSelectedZPos]);
			m_sCoord[1][3].SetCaption(szTemp);
			break;
		}
		return;
	}

	double rgdXYTFixed[DEF_HANDLER_TEACH_XYT_AXIS_TITLE];
	double rgdXYTOffset[DEF_HANDLER_TEACH_XYT_AXIS_TITLE];
	double rgdXYTModel[DEF_HANDLER_TEACH_XYT_AXIS_TITLE];

	m_rgXYTFixedPosData[m_iSelRealPos].TransToArray(rgdXYTFixed);
	m_rgXYTOffsetPosData[m_iSelRealPos].TransToArray(rgdXYTOffset);
	m_rgXYTModelPosData[m_iSelRealPos].TransToArray(rgdXYTModel);

	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		rgdXYTOffset[iAxisID] = dNewTargetPos - rgdXYTFixed[iAxisID] - rgdXYTModel[iAxisID];
	
		swprintf(szTemp, _T("%.3f"), rgdXYTOffset[iAxisID]);
		m_sCoord[3][iAxisID].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		rgdXYTFixed[iAxisID] = dNewTargetPos - rgdXYTModel[iAxisID] - rgdXYTOffset[iAxisID];

		swprintf(szTemp, _T("%.3f"), rgdXYTFixed[iAxisID]);
		m_sCoord[1][iAxisID].SetCaption(szTemp);
		break;
	}

	m_rgXYTFixedPosData[m_iSelRealPos] = MPos_XYT(rgdXYTFixed[0], rgdXYTFixed[1], rgdXYTFixed[2]);
	m_rgXYTOffsetPosData[m_iSelRealPos] = MPos_XYT(rgdXYTOffset[0], rgdXYTOffset[1], rgdXYTOffset[2]);
}


void CTeachHandlerDlg::OnClickX() 
{
	// TODO: Add your control notification handler code here
	setTargetXYTPosValue(DEF_HANDLER_X_AXIS);
}

void CTeachHandlerDlg::OnClickY() 
{
	// TODO: Add your control notification handler code here
//@	setTargetXYTPosValue(DEF_HANDLER_Y_AXIS);
}

void CTeachHandlerDlg::OnClickT() 
{
	// TODO: Add your control notification handler code here
	setTargetXYTPosValue(DEF_HANDLER_T_AXIS);
}

void CTeachHandlerDlg::OnClickZ() 
{
	// TODO: Add your control notification handler code here
	setTargetXYTPosValue(DEF_HANDLER_Z_AXIS);
}

void CTeachHandlerDlg::OnClickZSelect() 
{
	// TODO: Add your control notification handler code here
	if (m_bZPosSelected == FALSE)
	{
		m_bZPosSelected = TRUE;
//@		if (m_iHandlerID == DEF_LOAD_HANDLER_TEACH)
//@			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_LOAD_HANDLER_Z);
//@		else if (m_iHandlerID == DEF_UNLOAD_HANDLER_TEACH)
//@			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_UNLOAD_HANDLER_Z);
#ifdef DEF_USE_TURN_HANDLER
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_HANDLER_Z);
#endif
		
		m_ctrlBtnZSelect.SetValue(TRUE);
		
		for (int i=0; i<DEF_HANDLER_TEACH_MAX_Z_POS_BTN; i++)
		{
			if (i < DEF_HANDLER_MAX_Z_POS)
				m_BtnZPos[i].EnableWindow(TRUE);
			else
				m_BtnZPos[i].EnableWindow(FALSE);
		}
		
		for (int i=0; i<DEF_HANDLER_TEACH_MAX_POS_BTN; i++)
			m_BtnPos[i].EnableWindow(FALSE);
		
		m_ctrlBtnAxis[eAxisX].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisY].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisT].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisZ].EnableWindow(TRUE);
		
		selectZPosition(m_iSelectedZPos);
	}
	else
	{
		m_bZPosSelected = FALSE;
		updateZCoordinate();
//@		if (m_iHandlerID == DEF_LOAD_HANDLER_TEACH)
//@			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_LOAD_HANDLER);
//@		else if (m_iHandlerID == DEF_UNLOAD_HANDLER_TEACH)
//@			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_UNLOAD_HANDLER);
#ifdef DEF_USE_TURN_HANDLER
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_HANDLER_XT);
#endif
		m_ctrlBtnZSelect.SetValue(FALSE);

		for (int i = 0; i < DEF_HANDLER_TEACH_MAX_Z_POS_BTN; i++)
			m_BtnZPos[i].EnableWindow(FALSE);

		/** Teaching Position Name Reading (XYT 축) */
		int iTemp = 0;
		for (int i = 0; i < DEF_HANDLER_TEACH_MAX_POS; i++, iTemp++)
		{
			// 설정에 따라 Button Disable
			disabledPositionIndex(i, iTemp);
		}
	
		// Doolin kwangilshin 2017. 09. 15.
		// 
// 		m_ctrlBtnAxis[eAxisX].EnableWindow(TRUE);
// 		m_ctrlBtnAxis[eAxisY].EnableWindow(TRUE);
		// End.
		
		m_ctrlBtnAxis[eAxisT].EnableWindow(TRUE);
		m_ctrlBtnAxis[eAxisZ].EnableWindow(FALSE);
	}
}

/**
 * 선택 Position 변경 (Z축)
 */
void CTeachHandlerDlg::selectZPosition(int iPosIndex)
{
	m_BtnZPos[m_iSelectedZPos].SetValue(FALSE);
	m_iSelectedZPos = iPosIndex;
	m_BtnZPos[m_iSelectedZPos].SetValue(TRUE);

	updateZCoordinate();
}

void CTeachHandlerDlg::OnClickZPos0() 
{
	// TODO: Add your control notification handler code here
	selectZPosition(0);
}

void CTeachHandlerDlg::OnClickZPos1() 
{
	// TODO: Add your control notification handler code here
	selectZPosition(1);
}

void CTeachHandlerDlg::OnClickZPos2() 
{
	// TODO: Add your control notification handler code here
	selectZPosition(2);
}

void CTeachHandlerDlg::OnClickZPos3() 
{
	// TODO: Add your control notification handler code here
	selectZPosition(3);
}
