// TeachPanelAlignerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "MPanelData.h"
#include "MPreBonderData.h"
#include "TeachPanelAlignerDlg.h"
#include "VisionCalibDlg.h"
#include "VisionMonitorDlg.h"
#include "ManualPanelAlignerDlg.h"
#include "IAxis.h"
#include "IVision.h"
#include <math.h>
#include "CommonGetValueDlg.h"
#include "MTrsAutoManager.h"
//#include "MRollerConveyor.h"
#include "MCameraCarrier.h"
#include "MPanelAligner.h"
#include "MPanelTransfer.h"
#include "MCtrlPanelAligner.h"
#include "MManageOpPanel.h"

// hongju 추가
#include "MPanelTransfer.h"

#include "TeachMainForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeachPanelAlignerDlg dialog
extern MPlatformOlbSystem MOlbSystemPre;

CTeachPanelAlignerDlg::CTeachPanelAlignerDlg(int iFixModel, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachPanelAlignerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeachPanelAlignerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_bZSelected = FALSE;
	m_iFixModel = iFixModel;
	m_iSelectedPos = 0;
	m_iSelectedZPos = 0;
	m_iSelRealPos = 0;
	m_iJogSelect = -1;

	m_dDisplayOffsetStageZ = 0.0;

	m_plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();

	// PanelAligner 에서 좌표 얻어오기
	m_plnkPanelAligner->GetXYTPosParameter(m_rgXYTFixedPosData, m_rgXYTOffsetPosData, m_rgXYTModelPosData);
	m_plnkPanelAligner->GetZPosParameter(m_rgZFixedPosData, m_rgZOffsetPosData, m_rgZModelPosData);

	m_plnkCtrlPanelAligner	= MOlbSystemPre.GetCtrlPanelAlignerComponent();
	
	m_plnkC_MngOpPanel	= MOlbSystemPre.GetManageOpPanelComponent();
	m_plnkSystemData	= MOlbSystemPre.GetSystemData();
	m_plnkPanelData		= MOlbSystemPre.GetPanelData();
	m_plnkPreBonderData		= MOlbSystemPre.GetPreBonderData();
	
	m_bPanelAlignerVacSts = m_plnkPanelAligner->IsPanelAbsorbed();
	m_bPanelAlignerVacOnCheck = !m_bPanelAlignerVacSts;

	m_bAirCVBlowOnSts = m_plnkPanelAligner->IsAirCVBlowOn();
	m_bAirCVBlowOnCheck = !m_bAirCVBlowOnSts;

	int iPanelWorkNo = m_plnkPanelAligner->GetPanelWorkNoFromCurPos();
	m_bAirCVVacSts = m_plnkPanelAligner->IsAirCVVacAllOn(iPanelWorkNo);
	m_bAirCVVacOnCheck = !m_bAirCVVacSts;

	m_bMultiXJog = TRUE;
	m_dDisplayOffsetX2 = 0.0;
}


void CTeachPanelAlignerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachPanelAlignerDlg)
	DDX_Control(pDX, IDB_X, m_ctrlBtnAxis[eAxisX]);
	DDX_Control(pDX, IDB_Y, m_ctrlBtnAxis[eAxisY]);
	DDX_Control(pDX, IDB_T, m_ctrlBtnAxis[eAxisT]);
	DDX_Control(pDX, IDB_Z, m_ctrlBtnAxis[eAxisZ]);
	
	//DDX_Control(pDX, IDC_GRD_AXISX_SNS_STS, m_grd_AxisSnsState[eAxisX]);
	//DDX_Control(pDX, IDC_GRD_AXISY_SNS_STS, m_grd_AxisSnsState[eAxisY]);
	//DDX_Control(pDX, IDC_GRD_AXIST_SNS_STS, m_grd_AxisSnsState[eAxisT]);
	//DDX_Control(pDX, IDC_GRD_AXISZ_SNS_STS, m_grd_AxisSnsState[eAxisZ]);


	DDX_Control(pDX, IDB_Z_SELECT, m_ctrlBtnZSelect);
	DDX_Control(pDX, IDB_JOG, m_ctrlBtnSelectJog);
	//}}AFX_DATA_MAP
	
	DDX_Control(pDX, IDB_VISION_CALIB,			m_BtnVision[0]);
	DDX_Control(pDX, IDB_REGISTER_MARK,			m_BtnVision[1]);

	DDX_Control(pDX, IDB_CM_MOVE,				m_BtnCmd[0]);
	DDX_Control(pDX, IDB_CM_MARK_ALIGN,			m_BtnCmd[1]);
	DDX_Control(pDX, IDB_POS_SET,				m_BtnCmd[2]);
//	DDX_Control(pDX, IDB_CM_VACUUM,				m_BtnCmd[4]);
//	DDX_Control(pDX, IDB_CM_AIR_CV_ONOFF,		m_BtnCmd[7]);
//	DDX_Control(pDX, IDB_CM_AIR_CV_VAC_ONOFF,	m_BtnCmd[8]);
	
	DDX_Control(pDX, IDB_Z_POS0, m_BtnZPos[0]);
	DDX_Control(pDX, IDB_Z_POS1, m_BtnZPos[1]);
	DDX_Control(pDX, IDB_Z_POS2, m_BtnZPos[2]);
	DDX_Control(pDX, IDB_Z_POS3, m_BtnZPos[3]);

	DDX_Control(pDX, IDB_STAGE_POS0, m_BtnPos[0]);
	DDX_Control(pDX, IDB_STAGE_POS1, m_BtnPos[1]);
	DDX_Control(pDX, IDB_STAGE_POS2, m_BtnPos[2]);
	DDX_Control(pDX, IDB_STAGE_POS3, m_BtnPos[3]);
	DDX_Control(pDX, IDB_STAGE_POS4, m_BtnPos[4]);
	DDX_Control(pDX, IDB_STAGE_POS5, m_BtnPos[5]);
	DDX_Control(pDX, IDB_STAGE_POS6, m_BtnPos[6]);
	DDX_Control(pDX, IDB_STAGE_POS7, m_BtnPos[7]);
	DDX_Control(pDX, IDB_STAGE_POS8, m_BtnPos[8]);
	DDX_Control(pDX, IDB_STAGE_POS9, m_BtnPos[9]);	
	DDX_Control(pDX, IDB_STAGE_POS10, m_BtnPos[10]);	
	DDX_Control(pDX, IDB_STAGE_POS11, m_BtnPos[11]);	
	DDX_Control(pDX, IDB_STAGE_POS12, m_BtnPos[12]);	
	DDX_Control(pDX, IDB_STAGE_POS13, m_BtnPos[13]);	
	DDX_Control(pDX, IDB_STAGE_POS14, m_BtnPos[14]);	
	DDX_Control(pDX, IDB_STAGE_POS15, m_BtnPos[15]);	
	
	DDX_Control(pDX, IDC_TGT_POS, m_sCoordTitle[0]);
	DDX_Control(pDX, IDC_TGT_POS_X, m_sCoord[0][0]);
	DDX_Control(pDX, IDC_TGT_POS_Y, m_sCoord[0][1]);
	DDX_Control(pDX, IDC_TGT_POS_T, m_sCoord[0][2]);
	DDX_Control(pDX, IDC_TGT_POS_Z, m_sCoord[0][3]);

	DDX_Control(pDX, IDC_FIX_POS, m_sCoordTitle[1]);
	DDX_Control(pDX, IDC_FIX_POS_X, m_sCoord[1][0]);
	DDX_Control(pDX, IDC_FIX_POS_Y, m_sCoord[1][1]);
	DDX_Control(pDX, IDC_FIX_POS_T, m_sCoord[1][2]);
	DDX_Control(pDX, IDC_FIX_POS_Z, m_sCoord[1][3]);

	DDX_Control(pDX, IDC_MODEL_POS, m_sCoordTitle[2]);
	DDX_Control(pDX, IDC_MODEL_POS_X, m_sCoord[2][0]);
	DDX_Control(pDX, IDC_MODEL_POS_Y, m_sCoord[2][1]);
	DDX_Control(pDX, IDC_MODEL_POS_T, m_sCoord[2][2]);
	DDX_Control(pDX, IDC_MODEL_POS_Z, m_sCoord[2][3]);

	DDX_Control(pDX, IDC_OFFSET_POS, m_sCoordTitle[3]);
	DDX_Control(pDX, IDC_OFFSET_POS_X, m_sCoord[3][0]);
	DDX_Control(pDX, IDC_OFFSET_POS_Y, m_sCoord[3][1]);
	DDX_Control(pDX, IDC_OFFSET_POS_T, m_sCoord[3][2]);
	DDX_Control(pDX, IDC_OFFSET_POS_Z, m_sCoord[3][3]);

	DDX_Control(pDX, IDC_PANEL_MARK_OFFSET, m_sCoordTitle[4]);
	DDX_Control(pDX, IDC_PANEL_MARK_OFFSET_X, m_sCoord[4][0]);
	DDX_Control(pDX, IDC_PANEL_MARK_OFFSET_Y, m_sCoord[4][1]);
	DDX_Control(pDX, IDC_PANEL_MARK_OFFSET_T, m_sCoord[4][2]);
	DDX_Control(pDX, IDC_PANEL_MARK_OFFSET_Z, m_sCoord[4][3]);

	DDX_Control(pDX, IDC_ETC_OFFSET, m_sCoordTitle[5]);
	DDX_Control(pDX, IDC_ETC_OFFSET_X, m_sCoord[5][0]);
	DDX_Control(pDX, IDC_ETC_OFFSET_Y, m_sCoord[5][1]);
	DDX_Control(pDX, IDC_ETC_OFFSET_T, m_sCoord[5][2]);
	DDX_Control(pDX, IDC_ETC_OFFSET_Z, m_sCoord[5][3]);

	DDX_Control(pDX, IDC_CUR_POS, m_sCoordTitle[6]);
	DDX_Control(pDX, IDC_CUR_POS_X, m_sCoord[6][0]);
	DDX_Control(pDX, IDC_CUR_POS_Y, m_sCoord[6][1]);
	DDX_Control(pDX, IDC_CUR_POS_T, m_sCoord[6][2]);
	DDX_Control(pDX, IDC_CUR_POS_Z, m_sCoord[6][3]);

	DDX_Control(pDX, IDC_DIF_POS, m_sCoordTitle[7]);
	DDX_Control(pDX, IDC_DIF_POS_X, m_sCoord[7][0]);
	DDX_Control(pDX, IDC_DIF_POS_Y, m_sCoord[7][1]);
	DDX_Control(pDX, IDC_DIF_POS_T, m_sCoord[7][2]);
	DDX_Control(pDX, IDC_DIF_POS_Z, m_sCoord[7][3]);
}


BEGIN_MESSAGE_MAP(CTeachPanelAlignerDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachPanelAlignerDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachPanelAlignerDlg message handlers

void CTeachPanelAlignerDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(),&m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTeachPanelAlignerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	switch (m_iFixModel)
	{
		case DEF_MODEL_MODE:
		SetWindowText(_T("PanelAligner Model Coordinates Teaching"));		
		break;
	case DEF_FIX_MODE:
		SetWindowText(_T("PanelAligner Fixed Coordinates Teaching"));
		break;		
	}
	
	/** Teaching Position Name Reading (XYT 축) */
	int iTemp = 0;
	int iSelected = -1;
	STeachingInfo sTeachingInfo = m_plnkPanelAligner->GetXYTTeachingInfo();
	for (int i = 0; i < DEF_PANEL_ALIGNER_TEACH_MAX_POS; i++)
	{
		if (FALSE == disabledPositionIndex(i, iTemp))
			continue;
			
		if ((sTeachingInfo.m_sPosition[i]).m_bUse == TRUE)
		{
			m_BtnPos[iTemp].SetWindowText((LPCTSTR)((sTeachingInfo.m_sPosition[i]).m_strNewLineName[0]));
			m_rgiIndexMapping[iTemp] = i;
			
			if (sTeachingInfo.m_sPosition[i].m_bEnable == FALSE)
				m_BtnPos[iTemp].EnableWindow(FALSE);
			
			iTemp++;
			
			if (iSelected == -1 && sTeachingInfo.m_sPosition[i].m_bEnable == TRUE)
				iSelected = i;
		}
	}
	
	// 사용하지 않는 Position Button 감추기
	for (int i = iTemp; i < DEF_PANEL_ALIGNER_TEACH_MAX_POS_BTN; i++)
		m_BtnPos[i].ShowWindow(SW_HIDE);
	
	// Teaching Position Name Reading
	sTeachingInfo = m_plnkPanelAligner->GetZTeachingInfo();
	for (int i = 0; i < DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS; i++)
	{
		m_BtnZPos[i].SetWindowText((LPCTSTR)(sTeachingInfo.m_sPosition[i].m_strNewLineName[0]));
		m_BtnZPos[i].EnableWindow(FALSE);
	}

	// 사용하지 않는 Position Button 감추기
	for (int i = DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS; i < DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS_BTN; i++)
		m_BtnZPos[i].ShowWindow(SW_HIDE);

	m_bMultiXJog = TRUE;
	m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_STAGE_XYT);

	// 사용하지 않는 Axis Button 감추기
	m_ctrlBtnAxis[eAxisZ].EnableWindow(FALSE);	

	UpdateCoordinate();
	UpdateZCoordinate();

	// 기본 SelectPosition 설정 하기
	if (iSelected != -1)
		SelectPosition(iSelected);
	
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);

	m_uiTimerID = SetTimer(1, 200, NULL);
	m_uiTimerID3 = SetTimer(3, 500, NULL);

#ifndef DEF_GATE_SYSTEM
	GetDlgItem(IDB_CM_PANEL_TURN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_CM_PANEL_RETURN)->ShowWindow(SW_HIDE);
#endif

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;

  //Jog 축 변경
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_1"), &szTextTemp);
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	//X축
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_2"), &szTextTemp);
	GetDlgItem(IDB_X)->SetWindowText(szTextTemp);
	//Y축
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_3"), &szTextTemp);
	GetDlgItem(IDB_Y)->SetWindowText(szTextTemp);
	//T축
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_4"), &szTextTemp);
	GetDlgItem(IDB_T)->SetWindowText(szTextTemp);
	//Transfer\r\nX축
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_5"), &szTextTemp);
	GetDlgItem(IDB_Z)->SetWindowText(szTextTemp);

  //목표 위치
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_6"), &szTextTemp);
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	//고정 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_7"), &szTextTemp);
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	//모델 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_8"), &szTextTemp);
	GetDlgItem(IDC_MODEL_POS)->SetWindowText(szTextTemp);
	//Offset 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_9"), &szTextTemp);
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	//Panel Mark 보정
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_10"), &szTextTemp);
	GetDlgItem(IDC_PANEL_MARK_OFFSET)->SetWindowText(szTextTemp);
	//기타 보정값
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_11"), &szTextTemp);
	GetDlgItem(IDC_ETC_OFFSET)->SetWindowText(szTextTemp);
	//현재 위치
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_12"), &szTextTemp);
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	//차이값
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_13"), &szTextTemp);
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);

  //목표 위치로\r\n이동
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_14"), &szTextTemp);
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	//목표 위치를\r\n현재값으로
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_15"), &szTextTemp);
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	//Plate 부상\r\nOn/Off
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_16"), &szTextTemp);
	GetDlgItem(IDB_CM_AIR_CV_ONOFF)->SetWindowText(szTextTemp);
	//Plate Vacuum\r\nOn/Off"
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_17"), &szTextTemp);
	GetDlgItem(IDB_CM_AIR_CV_VAC_ONOFF)->SetWindowText(szTextTemp);
	//Teaching\r\n유닛 변경
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_18"), &szTextTemp);
	GetDlgItem(IDB_Z_SELECT)->SetWindowText(szTextTemp);

  //저장
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_19"), &szTextTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	//나가기
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_20"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	// Doolin YoonDaeRoh 2017. 09. 05.
	//
	Init_GridAxisStste();
	// End

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CTeachPanelAlignerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachPanelAlignerDlg)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_JOG, -600 /* Click */, OnJog, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_X, -600 /* Click */, OnX, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_Y, -600 /* Click */, OnY, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_T, -600 /* Click */, OnT, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_Z, -600 /* Click */, OnZ, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS0, -600 /* Click */, OnPanelAlignerPos0, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS1, -600 /* Click */, OnPanelAlignerPos1, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS2, -600 /* Click */, OnPanelAlignerPos2, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS3, -600 /* Click */, OnPanelAlignerPos3, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS4, -600 /* Click */, OnPanelAlignerPos4, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS5, -600 /* Click */, OnPanelAlignerPos5, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS6, -600 /* Click */, OnPanelAlignerPos6, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS7, -600 /* Click */, OnPanelAlignerPos7, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS8, -600 /* Click */, OnPanelAlignerPos8, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS9, -600 /* Click */, OnPanelAlignerPos9, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS10, -600 /* Click */, OnPanelAlignerPos10, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS11, -600 /* Click */, OnPanelAlignerPos11, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS12, -600 /* Click */, OnPanelAlignerPos12, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS13, -600 /* Click */, OnPanelAlignerPos13, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS14, -600 /* Click */, OnPanelAlignerPos14, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_STAGE_POS15, -600 /* Click */, OnPanelAlignerPos15, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_VISION_CALIB, -600 /* Click */, OnVisionCalib, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_REGISTER_MARK, -600 /* Click */, OnRegisterMark, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_MOVE, -600 /* Click */, OnCmMove, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_MARK_ALIGN, -600 /* Click */, OnCmMarkAlign, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_POS_SET, -600 /* Click */, OnPosSet, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_VACUUM, -600 /* Click */, OnCmVacuum, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_Z_SELECT, -600 /* Click */, OnZSelect, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_Z_POS0, -600 /* Click */, OnClickZPos0, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_Z_POS1, -600 /* Click */, OnClickZPos1, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_Z_POS2, -600 /* Click */, OnClickZPos2, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_Z_POS3, -600 /* Click */, OnClickZPos3, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_AIR_CV_ONOFF, -600 /* Click */, OnClickCmAirCvOnoff, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_AIR_CV_VAC_ONOFF, -600 /* Click */, OnClickCmAirCvVacOnoff, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_STAGE_UPVAC, -600 /* Click */, OnClickCmStageUpvac, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_STAGE_RELEASEDOWN, -600 /* Click */, OnClickCmStageReleasedown, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_RESET_ALIGN_OFFSET, -600 /* Click */, OnClickCmResetAlignOffset, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_PANEL_SET_STAGE, -600 /* Click */, OnClickCmPanelSetStage, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_MANUAL_VIEW, -600 /* Click */, OnClickCmManualView, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_PANEL_TURN, -600 /* Click */, OnClickCmPanelTurn, VTS_NONE)
	ON_EVENT(CTeachPanelAlignerDlg, IDB_CM_PANEL_RETURN, -600 /* Click */, OnClickCmPanelReturn, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachPanelAlignerDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID) KillTimer(m_uiTimerID);
	if (m_uiTimerID3) KillTimer(m_uiTimerID3);

#ifndef SIMULATION
	m_plnkC_MngOpPanel->SetJogUnit(-1);
#endif /* SIMULATION */

	CDialog::OnCancel();
}

void CTeachPanelAlignerDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	//Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 50500, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

	if (m_iFixModel == DEF_FIX_MODE)
	{
//@#ifdef DEF_SOURCE_SYSTEM
//@		m_plnkSystemData->m_dAlignCam1_FixedPos_X = m_rgXYTFixedPosData[DEF_PANEL_ALIGNER_FIDU_MARK_XU_POS].dX;
//@		m_plnkSystemData->m_dAlignCam2_FixedPos_X = m_plnkSystemData->m_dAlignCam1_FixedPos_X;
//@		m_plnkSystemData->WriteData();
//@#endif
		m_plnkPanelAligner->SaveXYTFixedPosParameter(m_rgXYTFixedPosData);
		m_plnkPanelAligner->SaveZFixedPosParameter(m_rgZFixedPosData);
	}
	else
	{
		m_plnkPanelAligner->SaveXYTOffsetPosParameter(m_rgXYTOffsetPosData);
		m_plnkPanelAligner->SaveZOffsetPosParameter(m_rgZOffsetPosData);
	}

	m_plnkPanelAligner->ChangeModel();
}

void CTeachPanelAlignerDlg::OnJog() 
{
	// TODO: Add your control notification handler code here
	if (m_bZSelected)
	{
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_STAGE_Z);
		m_ctrlBtnAxis[eAxisZ].SetValue(FALSE);
		m_ctrlBtnAxis[eAxisZ].SetWindowText(_T("Z Axis"));
	}
	else
	{
		if (m_bMultiXJog)
		{
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_STAGE_YT);
			m_ctrlBtnAxis[eAxisX].SetValue(FALSE);
//			X축\r\n개별 Jog
			m_ctrlBtnAxis[eAxisY].SetWindowText(_T("free\r\nY Jog"));
			m_ctrlBtnAxis[eAxisY].SetValue(FALSE);
//			T축\r\n개별 Jog
			m_ctrlBtnAxis[eAxisT].SetWindowText(_T("free\r\nT Jog"));
			m_ctrlBtnAxis[eAxisT].SetValue(FALSE);
			m_bMultiXJog = FALSE;
		}
		else
		{
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_STAGE_XYT);
			m_ctrlBtnAxis[eAxisX].SetValue(TRUE);
			m_ctrlBtnAxis[eAxisX].SetWindowText(_T("X Axis"));
			m_ctrlBtnAxis[eAxisY].SetValue(TRUE);
			m_ctrlBtnAxis[eAxisY].SetWindowText(_T("Y Axis"));
			m_ctrlBtnAxis[eAxisT].SetValue(TRUE);
			m_ctrlBtnAxis[eAxisT].SetWindowText(_T("T Axis"));
			m_bMultiXJog = TRUE;
		}
	}
}

void CTeachPanelAlignerDlg::OnX() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0][0].GetWindowText(cBeforeVal, 30);
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkPanelAligner->GetMotionComponent(DEF_PANEL_ALIGNER_X_AXIS)))
	{
		return;
	}
	// End.

	// 목표위치 입력 받기
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	// 목표위치 설정하기
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(cAfterVal, _T("%.3f"), dNewTargetPos);
	m_sCoord[0][0].SetCaption(cAfterVal);
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		m_rgXYTOffsetPosData[m_iSelRealPos].dX = dNewTargetPos
												- m_rgXYTFixedPosData[m_iSelRealPos].dX
												- m_rgXYTModelPosData[m_iSelRealPos].dX
												- m_XYTMarkOffset.dX
												- m_XYTEtcOffset.dX;
	
		swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[m_iSelRealPos].dX);
		m_sCoord[3][0].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		m_rgXYTFixedPosData[m_iSelRealPos].dX = dNewTargetPos
												- m_rgXYTModelPosData[m_iSelRealPos].dX
												- m_rgXYTOffsetPosData[m_iSelRealPos].dX
												- m_XYTMarkOffset.dX
												- m_XYTEtcOffset.dX;

		swprintf(szTemp, _T("%.3f"), m_rgXYTFixedPosData[m_iSelRealPos].dX);
		m_sCoord[1][0].SetCaption(szTemp);
		break;
	}	
}

void CTeachPanelAlignerDlg::OnY() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0][1].GetWindowText(cBeforeVal, 30);
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkPanelAligner->GetMotionComponent(DEF_PANEL_ALIGNER_Y_AXIS)))
	{
		return;
	}
	// End.

	// 목표위치 입력 받기
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	// 목표위치 설정하기
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(cAfterVal, _T("%.3f"), dNewTargetPos);
	m_sCoord[0][1].SetCaption(cAfterVal);
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		m_rgXYTOffsetPosData[m_iSelRealPos].dY = dNewTargetPos
												- m_rgXYTModelPosData[m_iSelRealPos].dY
												- m_rgXYTFixedPosData[m_iSelRealPos].dY
												- m_XYTMarkOffset.dY
												- m_XYTEtcOffset.dY;
	
		swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[m_iSelRealPos].dY);
		m_sCoord[3][1].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		m_rgXYTFixedPosData[m_iSelRealPos].dY = dNewTargetPos
												- m_rgXYTModelPosData[m_iSelRealPos].dY
												- m_rgXYTOffsetPosData[m_iSelRealPos].dY
												- m_XYTMarkOffset.dY
												- m_XYTEtcOffset.dY;

		swprintf(szTemp, _T("%.3f"), m_rgXYTFixedPosData[m_iSelRealPos].dY);
		m_sCoord[1][1].SetCaption(szTemp);
		break;		
	}
}

void CTeachPanelAlignerDlg::OnT() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0][2].GetWindowText(cBeforeVal, 30);
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkPanelAligner->GetMotionComponent(DEF_PANEL_ALIGNER_T_AXIS)))
	{
		return;
	}
	// End.

	// 목표위치 입력 받기
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	// 목표위치 설정하기
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(cAfterVal, _T("%.3f"), dNewTargetPos);
	m_sCoord[0][2].SetCaption(cAfterVal);
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		m_rgXYTOffsetPosData[m_iSelRealPos].dT = dNewTargetPos
												- m_rgXYTModelPosData[m_iSelRealPos].dT
												- m_rgXYTFixedPosData[m_iSelRealPos].dT
												- m_XYTMarkOffset.dT
												- m_XYTEtcOffset.dT;
	
		swprintf(szTemp, _T("%.3f"), m_rgXYTOffsetPosData[m_iSelRealPos].dT);
		m_sCoord[3][2].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		m_rgXYTFixedPosData[m_iSelRealPos].dT = dNewTargetPos
												- m_rgXYTModelPosData[m_iSelRealPos].dT
												- m_rgXYTOffsetPosData[m_iSelRealPos].dT
												- m_XYTMarkOffset.dT
												- m_XYTEtcOffset.dT;

		swprintf(szTemp, _T("%.3f"), m_rgXYTFixedPosData[m_iSelRealPos].dT);
		m_sCoord[1][2].SetCaption(szTemp);
		break;		
	}
}

void CTeachPanelAlignerDlg::OnZ() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;
	WCHAR szTemp[250];

	if (m_bZSelected == TRUE)
	{
		m_sCoord[0][3].GetWindowText(cBeforeVal, 30);
		
		// Doolin kwangilshin 2017. 08. 25.
		//
		if (!Check_AxisStste(m_plnkPanelAligner->GetZMotionComponent()))
		{
			return;
		}
		// End.

		// 목표위치 입력 받기
		if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
			return;
		
		// 목표위치 설정하기
		dNewTargetPos = wcstod(cAfterVal,NULL);
		m_sCoord[0][3].SetCaption(cAfterVal);
		switch (m_iFixModel)
		{
		case DEF_MODEL_MODE :
			m_rgZOffsetPosData[m_iSelectedZPos] = dNewTargetPos
												- m_rgZModelPosData[m_iSelectedZPos]
												- m_rgZFixedPosData[m_iSelectedZPos];
			
			swprintf(szTemp, _T("%.3f"), m_rgZOffsetPosData[m_iSelectedZPos]);
			m_sCoord[3][3].SetCaption(szTemp);
			break;

		case DEF_FIX_MODE :
			m_rgZFixedPosData[m_iSelectedZPos] = dNewTargetPos
												- m_rgZModelPosData[m_iSelectedZPos]
												- m_rgZOffsetPosData[m_iSelectedZPos];

			swprintf(szTemp, _T("%.3f"), m_rgZFixedPosData[m_iSelectedZPos]);
			m_sCoord[1][3].SetCaption(szTemp);
			break;		
		}
	}
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);	
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos2() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(2);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos3() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(3);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos4() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(4);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos5() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(5);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos6() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(6);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos7() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(7);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos8() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(8);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos9() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(9);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos10() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(10);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos11() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(11);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos12() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(12);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos13() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(13);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos14() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(14);
}

void CTeachPanelAlignerDlg::OnPanelAlignerPos15() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(15);
}

void CTeachPanelAlignerDlg::OnVisionCalib() 
{
	if (m_bZSelected)
		OnZSelect();

	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		//Dry-Run일 경우는 Vision Calibration을 수행할 수 없습니다
		MyMessageBox(MY_MESSAGE_BOX, 50501, _T("Confirm"), M_ICONINFORMATION|M_OK);

		return;
	}

	/*/// 2010.07.22 KBC -------------------------------------
	m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_ALIGNER_XYT);
	/*/// 2010.07.22 KBC -------------------------------------
	m_ctrlBtnAxis[eAxisX].SetBackColorInterior(BASIC);
	m_ctrlBtnAxis[eAxisX].SetCaption(_T("X Axis"));
	m_ctrlBtnAxis[eAxisT].SetBackColorInterior(BASIC);
	m_ctrlBtnAxis[eAxisT].SetCaption(_T("T Axis"));
	
	m_bMultiXJog = TRUE;

	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
//	CVisionCalibDlg dlg(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIB_GROUP_PANELALIGNER, -1);
	CVisionCalibDlg dlg(DEF_PANEL_ALIGN_CAMERA_1, DEF_CALIB_GROUP_PANELALIGNER, 0);
	dlg.DoModal();

	// PanelAligner에서 좌표 얻어오기
	m_plnkPanelAligner->GetXYTPosParameter(m_rgXYTFixedPosData, m_rgXYTOffsetPosData, m_rgXYTModelPosData);

	UpdateDisplay();

	SelectPosition(m_iSelectedPos);
}

void CTeachPanelAlignerDlg::OnRegisterMark() 
{
	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
	m_iCamNo = DEF_PANEL_ALIGN_CAMERA_1;
	// Operation Type = DEF_PM_MARK / DEF_BLOB_MARK / DEF_CALIPER_MARK
	CVisionMonitorDlg dlg(plnkVision, DEF_PM_MARK, m_iCamNo);
	dlg.DoModal();
}

void CTeachPanelAlignerDlg::OnCmMove() 
{
	//목표위치로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 50502, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

//090908_이정용수정_문학일 요청사항
// Setup 기간 중, Setup 편의를 위해 필요한 기능 구현 관련 Define.
#ifndef DEF_SETUP
	if (FALSE == MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorClose_PrevMCUnload())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(210141000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;
	}

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

	//170316 SJ_YSH
	//목표위치 이동간 Panel 충돌 방지 Interlock 추가 이기환D 요청
	if(m_plnkPanelAligner->IsPanelAbsorbed() == TRUE)
	{
		if((m_iSelRealPos == DEF_PANEL_ALIGNER_LOAD_POS || m_iSelRealPos == DEF_PANEL_ALIGNER_FIRST_MARK_POS) && 
			MOlbSystemPre.GetPanelTransferComponent()->IsPanelAbsorbed() == TRUE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(440004000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Target Pos Move Error"), M_ICONINFORMATION);
			return;	
		}

		if((m_iSelRealPos == DEF_PANEL_ALIGNER_UNLOAD_POS || m_iSelRealPos == DEF_PANEL_ALIGNER_INSPECT_2_POS) && 
			MOlbSystemPre.GetPanelTransferOutComponent()->IsPanelAbsorbed() == TRUE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(440005000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Target Pos Move Error"), M_ICONINFORMATION);
			return;	
		}
	}
	//_____________
#endif
	//______________
	int iResult;
	int iPrevJogUnit;
	
	if (m_bZSelected)
	{
		// Z축 이동시 Interlock 확인
		CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
		ProgWnd.SetRange(0, 2);
		ProgWnd.SetText(_T("Move the PanelTransfer X to the target position.\n")
							_T("Wait a moment..."));
		ProgWnd.PeekAndPump();

		// 이동 중 Jog를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
		iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
		m_plnkC_MngOpPanel->SetJogUnit();

		ProgWnd.StepIt();

		iResult = m_plnkPanelAligner->SafeMoveZPos(m_iSelectedZPos);
		if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}

		ProgWnd.StepIt();

		m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
	}
	else
	{
		CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
		ProgWnd.SetRange(0, 3);
		ProgWnd.SetText(_T("PanelAligner XYT Move Target Pos.\n")
							_T("Wait for one moment..."));
		ProgWnd.PeekAndPump();

		// 이동 중 Jog를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
		iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
		m_plnkC_MngOpPanel->SetJogUnit();

		ProgWnd.StepIt();

		if (m_iSelRealPos == DEF_PANEL_ALIGNER_FIRST_MARK_POS
			|| m_iSelRealPos == DEF_PANEL_ALIGNER_SECOND_MARK_POS)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 50550, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDYES)
			{
				iResult = MOlbSystemPre.MoveInspectCameraPanelAlignPos();
				if (iResult)
				{
					CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
					MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				}
			}
		}
		iResult = m_plnkPanelAligner->SafeSyncMoveXYTPos(m_iSelRealPos, FALSE);
		if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl				MyMessageBox(iResult, _T("PanelAligner Teaching Error"), M_ICONERROR);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
		ProgWnd.StepIt();

		m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
	}	
}

void CTeachPanelAlignerDlg::OnCmMarkAlign()
{
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		//OP Mode Dry Run 상태입니다. OP Mode를 바꿔주세요!
		MyMessageBox(MY_MESSAGE_BOX, 50505, _T("Error!"), M_ICONERROR);

		return;
	}

	//Panel Align을 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 50506, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)

		return;

//090908_이정용수정_문학일 요청사항
// Setup 기간 중, Setup 편의를 위해 필요한 기능 구현 관련 Define.
#ifndef DEF_SETUP
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;
	}	
#endif
	//______________
	CMyProgressWnd ProgWnd(NULL, _T("PanelAligner Panel Align..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("PanelAligner Panel Align.\nWait for one moment..."));
	ProgWnd.PeekAndPump();
	
	int iResult = 0;
#ifdef DEF_SOURCE_SYSTEM
	iResult = MOlbSystemPre.GetPanelTransferComponent()->IsPanelReleased();
	if (!iResult)
	{
//		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
//		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		MyMessageBox(_T("Panel Transfer Panel Absorbed"), _T("Error"), M_ICONERROR);
		return;
	}
#endif

	iResult = m_plnkCtrlPanelAligner->AlignPanel();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		//MyMessageBox(strError.GetBuffer(strError.GetLength()),"Mark Align Error",M_ICONERROR);
// jdy sesl		MyMessageBox(iResult,_T("Mark Align Error"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	MPos_XYT tmpXYTPos = m_plnkPanelAligner->GetAlignMarkOffset();
	char  str[100];
	sprintf(str,"Panel Mark Align Succes.\nX:%.3f Y:%.3f T: %.3f", tmpXYTPos.dX, tmpXYTPos.dY, tmpXYTPos.dT);
	MyMessageBox(str, _T("Panel Align Success"), M_ICONERROR);	
}

void CTeachPanelAlignerDlg::OnPosSet() 
{
	WCHAR szTemp[250];	
	
	//목표 위치를 현재 위치로 변경하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 50507, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)

		return;

	if (m_bZSelected == FALSE)
	{
		MPos_XYT sCurrentXYT;
		MPos_XYT sMarkOffset;

		double rgdTargetPos[3];
		double rgdFixedPosData[3];
		double rgdModelPosData[3];
		double rgdOffsetPosData[3];
		double rgdMarkOffset[3];

		CMyProgressWnd ProgWnd(NULL, _T("Set Target Position..."));
		ProgWnd.SetRange(0, 4);
		ProgWnd.SetText(_T("Change the target position of PanelAligner XYT to current postion.\n")
							_T("Wait a moment..."));
		ProgWnd.PeekAndPump();

		sCurrentXYT = m_plnkPanelAligner->GetXYTCurrentPos();

		ProgWnd.StepIt();

		// Align Mark Offset은 T축에 따라 달리 계산 되므로 CalculateAlignMarkOffset 함수로 읽어 온다
		sMarkOffset = m_plnkPanelAligner->CalculateAlignMarkOffset(sCurrentXYT);

		ProgWnd.StepIt();
		
		sCurrentXYT.TransToArray(rgdTargetPos);
		sMarkOffset.TransToArray(rgdMarkOffset);

		m_rgXYTFixedPosData[m_iSelRealPos].TransToArray(rgdFixedPosData);
		m_rgXYTModelPosData[m_iSelRealPos].TransToArray(rgdModelPosData);
		m_rgXYTOffsetPosData[m_iSelRealPos].TransToArray(rgdOffsetPosData);
		
		for (int i = 0; i < 3; i++)
		{
			swprintf(szTemp, _T("%.3f"), rgdTargetPos[i]);
			m_sCoord[0][i].SetCaption(szTemp);
		}

		ProgWnd.StepIt();

		switch (m_iFixModel)
		{
		case DEF_MODEL_MODE :
			for (int i = 0; i < 3; i++)
			{
				rgdOffsetPosData[i] = rgdTargetPos[i] - rgdModelPosData[i] - rgdFixedPosData[i] - rgdMarkOffset[i];
			
				swprintf(szTemp, _T("%.3f"), rgdOffsetPosData[i]);
				m_sCoord[3][i].SetCaption(szTemp);
			}			

			m_rgXYTOffsetPosData[m_iSelRealPos].Init(rgdOffsetPosData[0], rgdOffsetPosData[1], rgdOffsetPosData[2]);
			break;
		
		case DEF_FIX_MODE :
			for (int i = 0; i < 3; i++)
			{
				rgdFixedPosData[i] = rgdTargetPos[i] - rgdModelPosData[i] - rgdOffsetPosData[i] - rgdMarkOffset[i];
				swprintf(szTemp, _T("%.3f"), rgdFixedPosData[i]);
				m_sCoord[1][i].SetCaption(szTemp);
			}
			m_rgXYTFixedPosData[m_iSelRealPos].Init(rgdFixedPosData[0], rgdFixedPosData[1], rgdFixedPosData[2]);
			break;
		}

		ProgWnd.StepIt();
	}
	else // Z 축 선택 선택
	{
		double dCurrentZ;
		double dTargetPos;

		CMyProgressWnd ProgWnd(NULL, _T("Set Target Position..."));
		ProgWnd.SetRange(0, 2);
		ProgWnd.SetText(_T("Change the target postion of the Stage Z to the current postion\n")
							_T("Wait a moment..."));
		ProgWnd.PeekAndPump();

		dCurrentZ = m_plnkPanelAligner->GetZCurrentPos();

		ProgWnd.StepIt();

		dTargetPos = dCurrentZ;
		swprintf(szTemp, _T("%.3f"), dTargetPos);
		m_sCoord[0][3].SetCaption(szTemp);
	
		switch (m_iFixModel)
		{
		case DEF_MODEL_MODE :	
			m_rgZModelPosData[m_iSelectedZPos]
							= dTargetPos
							- m_rgZOffsetPosData[m_iSelectedZPos]
							- m_rgZFixedPosData[m_iSelectedZPos];
			swprintf(szTemp, _T("%.3f"), m_rgZModelPosData[m_iSelectedZPos]);
			m_sCoord[3][3].SetCaption(szTemp);
			break;
			
		case DEF_FIX_MODE :
			m_rgZFixedPosData[m_iSelectedZPos]
							= dTargetPos
							- m_rgZOffsetPosData[m_iSelectedZPos]
							- m_rgZModelPosData[m_iSelectedZPos];
			swprintf(szTemp, _T("%.3f"), m_rgZFixedPosData[m_iSelectedZPos]);
			m_sCoord[1][3].SetCaption(szTemp);
			break;
		}

		ProgWnd.StepIt();
	}	
}

void CTeachPanelAlignerDlg::OnCmVacuum() 
{
	CMyProgressWnd ProgWnd(NULL, _T("PanelAligner Vacuum..."));
	ProgWnd.SetRange(0, 1);

	int iResult = ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS;
	if (m_bPanelAlignerVacSts == TRUE) {
		ProgWnd.SetText(_T("PanelAligner Vacuum Release.\nWait for one moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkPanelAligner->ReleasePanel();
	}
	else {
		ProgWnd.SetText(_T("PanelAligner Vacuum Absorb.\nWait for one moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkPanelAligner->AbsorbPanel();
	}

	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// jdy sesl		MyMessageBox(iResult, _T("PanelAligner Teaching Error"), M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	m_bPanelAlignerVacSts = !m_bPanelAlignerVacSts;

	ProgWnd.StepIt();
}

void CTeachPanelAlignerDlg::OnZSelect() 
{
	// TODO: Add your control notification handler code here
	if (m_bZSelected == FALSE)
	{
		m_bZSelected = TRUE;
//		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_X);

		OnJog();
		
		m_ctrlBtnZSelect.SetValue(TRUE);

		for (int i=0; i<DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS_BTN; i++)
			m_BtnZPos[i].EnableWindow(TRUE);

		for (int i=0; i<DEF_PANEL_ALIGNER_TEACH_MAX_POS_BTN; i++)
			m_BtnPos[i].EnableWindow(FALSE);

//		m_ctrlBtnSelectJog.EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisX].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisY].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisT].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisZ].EnableWindow(TRUE);

		SelectZPosition(m_iSelectedZPos);
	}
	else
	{
		m_bZSelected = FALSE;
//		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_ALIGNER_XYT);
		m_bMultiXJog = FALSE;
		OnJog();
		
		m_ctrlBtnZSelect.SetValue(FALSE);

		for (int i = 0; i < DEF_PANEL_ALIGNER_TEACH_MAX_Z_POS_BTN; i++)
			m_BtnZPos[i].EnableWindow(FALSE);

		/** Teaching Position Name Reading (XYT 축) */
		int iTemp = 0;
//		for (int i = 0; i < DEF_PANEL_ALIGNER_TEACH_MAX_POS; i++, iTemp++)
//		{
//			// 설정에 따라 Button Disable
//			if (FALSE == disabledPositionIndex(i, iTemp))
//				continue;
//			
//			iTemp++;
//		}
		
		for (int i=0; i<DEF_PANEL_ALIGNER_TEACH_MAX_POS_BTN; i++)
			m_BtnPos[i].EnableWindow(TRUE);

//		m_ctrlBtnSelectJog.EnableWindow(TRUE);
		m_ctrlBtnAxis[eAxisX].EnableWindow(TRUE);
		m_ctrlBtnAxis[eAxisY].EnableWindow(TRUE);
		m_ctrlBtnAxis[eAxisT].EnableWindow(TRUE);
		m_ctrlBtnAxis[eAxisZ].EnableWindow(FALSE);
	}
}

BOOL CTeachPanelAlignerDlg::disabledPositionIndex(int iPosIndex, int iTemp)
{
	// 사용하는 Position 만 활성화한다.
	switch (iPosIndex)
	{
	case DEF_PANEL_ALIGNER_LOAD_POS:			
	case DEF_PANEL_ALIGNER_FIRST_MARK_POS:
	case DEF_PANEL_ALIGNER_SECOND_MARK_POS:
	case DEF_PANEL_ALIGNER_PRESS_1_POS:
	case DEF_PANEL_ALIGNER_PRESS_2_POS:
	case DEF_PANEL_ALIGNER_INSPECT_1_POS:
	case DEF_PANEL_ALIGNER_INSPECT_2_POS:
	case DEF_PANEL_ALIGNER_INSPECT_3_POS:
	case DEF_PANEL_ALIGNER_UNLOAD_POS:
		break;

	default:
		return FALSE;
	}

	m_BtnPos[iTemp].EnableWindow(TRUE);
	return TRUE;
}

/**
 * 목표위치, Mark Offset, 기타보정, 현재위치, 편차 Display (XYT축)
 */
void CTeachPanelAlignerDlg::UpdateDisplay()
{
	WCHAR szTemp[250];

	MPos_XYT sTargetPos;
	MPos_XYT sPosMarkOffsetXYT;
	MPos_XYT sPosCurr;	

	double rgdFixedPosData[4];
	double rgdModelPosData[4];
	double rgdOffsetPosData[4];
	double rgdMarkOffset[4];
	double rgdEtcOffset[4];
	
	double rgdTargetPos[4];
	double rgdCurrentPos[4];

	m_rgXYTFixedPosData[m_iSelRealPos].TransToArray(rgdFixedPosData);
	m_rgXYTModelPosData[m_iSelRealPos].TransToArray(rgdModelPosData);
	m_rgXYTOffsetPosData[m_iSelRealPos].TransToArray(rgdOffsetPosData);
	
	// 기타 좌표 읽어 오기.
	m_XYTEtcOffset.Init(0.0, 0.0, 0.0);
	m_XYTEtcOffset.TransToArray(rgdEtcOffset);

	// 목표위치 계산하기 (목표 위치 = 고정좌표 + 모델좌표 + Offset좌표 + 기타좌표)
	sTargetPos.dX =	rgdFixedPosData[0] + rgdModelPosData[0] + rgdOffsetPosData[0] + rgdEtcOffset[0];
	sTargetPos.dY =	rgdFixedPosData[1] + rgdModelPosData[1] + rgdOffsetPosData[1] + rgdEtcOffset[1];
	sTargetPos.dT =	rgdFixedPosData[2] + rgdModelPosData[2] + rgdOffsetPosData[2] + rgdEtcOffset[2];

	sTargetPos.TransToArray(rgdTargetPos);

	// Mark Offset 가져 와서 Display 하기
	// Align Mark Offset은 T축에 따라 달리 계산 되므로 CalculateAlignMarkOffset 함수로 읽어 온다

#ifdef DEF_GATE_SYSTEM
//@	if (m_iSelRealPos != DEF_PANEL_ALIGNER_TURN_POS)
		sPosMarkOffsetXYT = m_plnkPanelAligner->CalculateAlignMarkOffset(sTargetPos);
#else
	sPosMarkOffsetXYT = m_plnkPanelAligner->CalculateAlignMarkOffset(sTargetPos);
#endif

	m_XYTMarkOffset = sPosMarkOffsetXYT;
	sPosMarkOffsetXYT.TransToArray(rgdMarkOffset);
	
	swprintf(szTemp, _T("%.3f"), rgdMarkOffset[0]);
	m_sCoord[4][0].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdMarkOffset[1]);
	m_sCoord[4][1].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdMarkOffset[2]);
	m_sCoord[4][2].SetCaption(szTemp);

	// 목표위치에 MarkOffset 반영하기 (목표위치 = 목표 위치 + MarkOffset)
	rgdTargetPos[0] +=	rgdMarkOffset[0];
	rgdTargetPos[1] +=	rgdMarkOffset[1];
	rgdTargetPos[2] +=	rgdMarkOffset[2];
	
	swprintf(szTemp, _T("%.3f"), rgdTargetPos[0]);
	m_sCoord[0][0].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdTargetPos[1]);
	m_sCoord[0][1].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdTargetPos[2]);
	m_sCoord[0][2].SetCaption(szTemp);

	// 기타 Offset 표시하기
	swprintf(szTemp, _T("%.3f"), rgdEtcOffset[0]);
	m_sCoord[5][0].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdEtcOffset[1]);
	m_sCoord[5][1].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdEtcOffset[2]);
	m_sCoord[5][2].SetCaption(szTemp);

#ifndef SIMULATION
	// 현재 위치 읽어오기
	sPosCurr = m_plnkPanelAligner->GetXYTCurrentPos();
#endif
	sPosCurr.TransToArray(rgdCurrentPos);

	double dCurrentPosX2 = m_plnkPanelAligner->GetMotionComponent(DEF_PANEL_ALIGNER_T_AXIS)->GetCurrentPosition();

	// Reset Switch 감지시 현재 위치를 0으로 표시되게 하여 
	// Teaching시 상대거리 측정을 쉽게 한다.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
	{
		m_sDisplayOffsetXYT = sPosCurr;
		m_dDisplayOffsetX2 = dCurrentPosX2;
	}

	sPosCurr = sPosCurr - m_sDisplayOffsetXYT;
	sPosCurr.TransToArray(rgdCurrentPos);

	if (FALSE == m_bMultiXJog)
	{
		rgdCurrentPos[2] = dCurrentPosX2 - m_dDisplayOffsetX2;
	}

	// 현재 위치 Display
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[0]);
	m_sCoord[6][0].SetCaption(szTemp);
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[1]);
	m_sCoord[6][1].SetCaption(szTemp);
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[2]);
	m_sCoord[6][2].SetCaption(szTemp);
	
	// 편차 계산하고, Display
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[0] - rgdTargetPos[0]);
	m_sCoord[7][0].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[1] - rgdTargetPos[1]);
	m_sCoord[7][1].SetCaption(szTemp);	
	swprintf(szTemp, _T("%.3f"), rgdCurrentPos[2] - rgdTargetPos[2]);
	m_sCoord[7][2].SetCaption(szTemp);
}

/**
 * 목표위치, Mark Offset, 기타보정, 현재위치, 편차 Display (Z축)
 */
void CTeachPanelAlignerDlg::UpdateZDisplay()
{
	WCHAR szTemp[250];
	double dCurrentXPos = 0.0;
	
	double dTargetZPos = m_rgZFixedPosData[m_iSelectedZPos]
						+ m_rgZOffsetPosData[m_iSelectedZPos]
						+ m_rgZModelPosData[m_iSelectedZPos];

	/** 목표 위치 */
	swprintf(szTemp, _T("%.3f"), dTargetZPos);	
	m_sCoord[0][3].SetCaption(szTemp);	

#ifndef SIMULATION
	/** 현재 위치 읽고, Display 하기 */
 	dCurrentXPos = m_plnkPanelAligner->GetZCurrentPos();
#endif /* SIMULATION */

	// Reset Switch 감지시 현재 위치를 0으로 표시되게 하여 
	// Teaching시 상대거리 측정을 쉽게 한다.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
	{
		m_dDisplayOffsetStageZ = dCurrentXPos;
	}
	
	dCurrentXPos = dCurrentXPos - m_dDisplayOffsetStageZ;	

	swprintf(szTemp, _T("%.3f"), dCurrentXPos);
	m_sCoord[6][3].SetCaption(szTemp);
	
	/** 편차 계산하기 */
	swprintf(szTemp, _T("%.3f"),  dCurrentXPos - dTargetZPos);
	m_sCoord[7][3].SetCaption(szTemp);
}

/**
 * 고정좌표, Offset좌표 Display (Z축)
 */
void CTeachPanelAlignerDlg::UpdateZCoordinate()
{
	WCHAR szTemp[250];
	// 고정좌표 화면 Display
	swprintf(szTemp, _T("%.3f"), m_rgZFixedPosData[m_iSelectedZPos]);
	m_sCoord[1][3].SetCaption(szTemp);
	// 모델좌표 화면 Display
	swprintf(szTemp, _T("%.3f"), m_rgZModelPosData[m_iSelectedZPos]);
	m_sCoord[2][3].SetCaption(szTemp);
	// 옵셋좌표 화면 Display
	swprintf(szTemp, _T("%.3f"), m_rgZOffsetPosData[m_iSelectedZPos]);
	m_sCoord[3][3].SetCaption(szTemp);
}

/**
 * 고정좌표, 모델좌표, Offset좌표 Display (XYT축)
 */
void CTeachPanelAlignerDlg::UpdateCoordinate()
{
	WCHAR szBuff[250];

	double rgdFixedPosData[3];
	double rgdModelPosData[3];
	double rgdOffsetPosData[3];

	m_rgXYTFixedPosData[m_iSelRealPos].TransToArray(rgdFixedPosData);
	m_rgXYTModelPosData[m_iSelRealPos].TransToArray(rgdModelPosData);
	m_rgXYTOffsetPosData[m_iSelRealPos].TransToArray(rgdOffsetPosData);

	for (int i = 0; i < DEF_PANEL_ALIGNER_TEACH_XYT_AXIS_TITLE; i++)
	{
		memset(szBuff, 0x00, sizeof(szBuff));
		
		swprintf(szBuff, _T("%.3f"), rgdFixedPosData[i]);
		// 고정좌표 화면 Display
		m_sCoord[1][i].SetWindowText(szBuff);

		swprintf(szBuff, _T("%.3f"), rgdModelPosData[i]);
		// 모델좌표 화면 Display
		m_sCoord[2][i].SetWindowText(szBuff);

		// 옵셋좌표 화면 Display
		swprintf(szBuff, _T("%.3f"), rgdOffsetPosData[i]);
		m_sCoord[3][i].SetWindowText(szBuff);
	}
	UpdateDisplay();
}

/**
 * 선택 Position 변경 (XYT축)
 */
void CTeachPanelAlignerDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelRealPos = m_rgiIndexMapping[iPosIndex];
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}

/**
 * 선택 Position 변경 (Z축)
 */
void CTeachPanelAlignerDlg::SelectZPosition(int iPosIndex)
{
	m_BtnZPos[m_iSelectedZPos].SetValue(FALSE);
	m_iSelectedZPos = iPosIndex;
	m_BtnZPos[m_iSelectedZPos].SetValue(TRUE);

	UpdateZCoordinate();
}

// Doolin YoonDaeRoh 2017. 09. 05.
//
void CTeachPanelAlignerDlg::Init_GridAxisStste()
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


BOOL CTeachPanelAlignerDlg::Check_AxisStste(IAxis *pclsAxis)
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


void CTeachPanelAlignerDlg::SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome)
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


void CTeachPanelAlignerDlg::SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachPanelAlignerDlg::SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachPanelAlignerDlg::SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable)
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


void CTeachPanelAlignerDlg::updateAxisSnsSts()
{
	IAxis *pclsAxis[eAxisEnd];
	
	pclsAxis[eAxisX] = m_plnkPanelAligner->GetMotionComponent(DEF_PANEL_ALIGNER_X_AXIS);
	pclsAxis[eAxisY] = m_plnkPanelAligner->GetMotionComponent(DEF_PANEL_ALIGNER_Y_AXIS);
	pclsAxis[eAxisT] = m_plnkPanelAligner->GetMotionComponent(DEF_PANEL_ALIGNER_T_AXIS);
	pclsAxis[eAxisZ] = m_plnkPanelAligner->GetZMotionComponent();

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


void CTeachPanelAlignerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		UpdateDisplay();
		UpdateZDisplay();
		UpdateButton();

		// Doolin kwangilshin 2017. 08. 25.
		//
		updateAxisSnsSts();
		// End.
	}

	CDialog::OnTimer(nIDEvent);
}

void CTeachPanelAlignerDlg::OnClickZPos0() 
{
	// TODO: Add your control notification handler code here
	SelectZPosition(0);
}

void CTeachPanelAlignerDlg::OnClickZPos1() 
{
	// TODO: Add your control notification handler code here
	SelectZPosition(1);
}

void CTeachPanelAlignerDlg::OnClickZPos2() 
{
	// TODO: Add your control notification handler code here
	SelectZPosition(2);
}

void CTeachPanelAlignerDlg::OnClickZPos3() 
{
	// TODO: Add your control notification handler code here
	SelectZPosition(3);
}

/**
 * Command Button 상태 Update
 */
void CTeachPanelAlignerDlg::UpdateButton()
{
	/*
	if (!m_bPanelAlignerVacOnCheck && m_plnkPanelAligner->IsPanelAbsorbed())
	{
		m_BtnCmd[4].SetBackColorInterior(DEF_BTN_ON_COLOR);
		m_bPanelAlignerVacOnCheck = TRUE;
	}
	if (m_bPanelAlignerVacOnCheck && !m_plnkPanelAligner->IsPanelAbsorbed())
	{
		m_BtnCmd[4].SetBackColorInterior(BASIC);
		m_bPanelAlignerVacOnCheck = FALSE;
	}
	if (!m_bAirCVBlowOnCheck && m_plnkPanelAligner->IsAirCVBlowOn())
	{
		m_BtnCmd[7].SetBackColorInterior(DEF_BTN_ON_COLOR);
		m_bAirCVBlowOnCheck = TRUE;
	}
	if (m_bAirCVBlowOnCheck && !m_plnkPanelAligner->IsAirCVBlowOn())
	{
		m_BtnCmd[7].SetBackColorInterior(BASIC);
		m_bAirCVBlowOnCheck = FALSE;
	}

	if (!m_bAirCVVacOnCheck && m_plnkPanelAligner->IsAirCVVacAllOn())
	{
		m_BtnCmd[8].SetBackColorInterior(DEF_BTN_ON_COLOR);
		m_bAirCVVacOnCheck = TRUE;
	}
	if (m_bAirCVVacOnCheck && !m_plnkPanelAligner->IsAirCVVacAllOn())
	{
		m_BtnCmd[8].SetBackColorInterior(BASIC);
		m_bAirCVVacOnCheck = FALSE;
	}
	*/
}

void CTeachPanelAlignerDlg::OnClickCmAirCvOnoff() 
{
	int iResult = ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS;
	if (m_bAirCVBlowOnSts == TRUE) {
		m_plnkPanelAligner->AirCVFloatingOff();
	}
	else {
		m_plnkPanelAligner->AirCVFloatingOn();
		m_bAirCVVacSts = FALSE;
	}

	m_bAirCVBlowOnSts = !m_bAirCVBlowOnSts;
}

void CTeachPanelAlignerDlg::OnClickCmAirCvVacOnoff() 
{
	int iResult = ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS;
	int iPanelWorkNo = m_plnkPanelAligner->GetPanelWorkNoFromCurPos();
	if (m_bAirCVVacSts == TRUE) {
		iResult = m_plnkPanelAligner->AirCVVacAllOff();
	}
	else {
		iResult = m_plnkPanelAligner->AirCVVacAllOn(iPanelWorkNo);
		m_bAirCVBlowOnSts = FALSE;
	}

	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		//MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()), "PanelAligner Teaching Error", M_ICONERROR);
// jdy sesl		MyMessageBox(iResult, _T("PanelAligner Teaching Error"), M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	m_bAirCVVacSts = !m_bAirCVVacSts;
}

void CTeachPanelAlignerDlg::OnClickCmStageUpvac() 
{
	// TODO: Add your control notification handler code here
	int iResult = ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS;
	CMyProgressWnd ProgWnd(NULL, _T("PanelAligner Z Up & Vac On..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("PanelAligner Z Up & Vac On.\nWait a moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_MOVE_UP_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	
	iResult = m_plnkPanelAligner->AbsorbPanel();
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
}

void CTeachPanelAlignerDlg::OnClickCmStageReleasedown() 
{
	// TODO: Add your control notification handler code here
	int iResult = ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS;
	CMyProgressWnd ProgWnd(NULL, _T("PanelAligner PickUp Down & Vac Off..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("PanelAligner Releasing.\nWait a moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelAligner->ReleasePanel();
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
}

//syr add..090414
void CTeachPanelAlignerDlg::OnClickCmResetAlignOffset() 
{
	//Align Offset을 Reset하시겠습니까?
	if ( MyMessageBox(MY_MESSAGE_BOX, 50511, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;
	
	m_plnkPanelAligner->InitAlignMarkOffset();	
}

void CTeachPanelAlignerDlg::OnClickCmPanelSetStage() 
{
	// TODO: Add your control notification handler code here
	// Message_54009=Panel 마크가 카메라 센터에 있습니까?
	if ( MyMessageBox(MY_MESSAGE_BOX, 50512, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

	int iResult = ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS;
	CMyProgressWnd ProgWnd(NULL, _T("Panel Set Stage ..."));
	ProgWnd.SetRange(0, 7);

	// 1. Stage Z Work Pos로 이동
	ProgWnd.SetText(_T("Stage Z Work Pos.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_WORK_POS);
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	// 2. Air CV 흡착
	int iPanelWorkNo = m_plnkPanelAligner->GetPanelWorkNoFromCurPos();
	ProgWnd.SetText(_T("Air Conveyor Panel Absorb.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	iResult = m_plnkPanelAligner->AirCVVacAllOn(iPanelWorkNo);
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	// 3. Stage 흡착 해제 및 Z Down
	ProgWnd.SetText(_T("Stage Panel Release and Z Axis Down.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	iResult = m_plnkPanelAligner->ReleasePanel();
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_DOWN_POS);
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	//110105.Add
	m_plnkPanelAligner->InitAlignMarkOffset();

	ProgWnd.StepIt();
	
	// 4. Stage Camera Pos로 이동
	ProgWnd.SetText(_T("Stage Move Camera Position.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	iResult = m_plnkPanelAligner->SafeMoveXYTPos(DEF_PANEL_ALIGNER_FIRST_MARK_POS);
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	// 5. Stage Z WorkPos로 이동 및 흡착
	ProgWnd.SetText(_T("Stage Z Work Pos.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_WORK_POS);
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = m_plnkPanelAligner->AbsorbPanel();
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	// 6. Air CV 흡착 해제 및 부상 ON
	ProgWnd.SetText(_T("Air Conveyor Panel Absorb.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	iResult = m_plnkPanelAligner->AirCVBlowOn();
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	// 7. Stage Move Up Pos로 이동
	ProgWnd.SetText(_T("Stage Move Up Work Pos.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	iResult = m_plnkPanelAligner->SafeMoveZPos(DEF_PANEL_ALIGNER_Z_MOVE_UP_POS);
	if (iResult != ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();		
}

void CTeachPanelAlignerDlg::OnClickCmManualView() 
{
	// TODO: Add your control notification handler code here
	CManualPanelAlignerDlg dlg;
	dlg.DoModal();
}

void CTeachPanelAlignerDlg::OnClickCmPanelTurn()
{
	/*/
	// TODO: Add your control notification handler code here
	if ( MyMessageBox(MY_MESSAGE_BOX, 50513, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;
	int iResult = ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Turn ..."));
	ProgWnd.SetRange(0, 5);

	ProgWnd.SetText(_T("Return Rotator...\nWait a moment..."));
	iResult = m_plnkPanelAligner->ReturnRotatorUnit();	
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	ProgWnd.SetText(_T("Transfer Aligner To Rotator...\nWait a moment..."));
	iResult = m_plnkCtrlPanelAligner->TransferPanel_AlignerToRotator();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	
	ProgWnd.SetText(_T("Turn Panel...\nWait a moment..."));
	m_plnkPanelAligner->InitAlignMarkOffset();
	iResult = m_plnkPanelAligner->TurnRotatorUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	ProgWnd.SetText(_T("Transfer Rotator To Aligner...\nWait a moment..."));
	iResult = m_plnkCtrlPanelAligner->TransferPanel_RotatorToAligner();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	
	ProgWnd.SetText(_T("Return Rotator...\nWait a moment..."));
	iResult = m_plnkPanelAligner->ReturnRotatorUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	/*/
}

void CTeachPanelAlignerDlg::OnClickCmPanelReturn()
{
	/*/
	// TODO: Add your control notification handler code here
	if ( MyMessageBox(MY_MESSAGE_BOX, 50514, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;
	int iResult = ERR_PANEL_ALIGNER_TEACH_DLG_SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Return ..."));
	ProgWnd.SetRange(0, 5);

	ProgWnd.SetText(_T("Turn Rotator...\nWait a moment..."));
	iResult = m_plnkPanelAligner->TurnRotatorUnit();	
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	ProgWnd.SetText(_T("Transfer Aligner To Rotator...\nWait a moment..."));
	iResult = m_plnkCtrlPanelAligner->TransferPanel_AlignerToRotator();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	
	ProgWnd.SetText(_T("Return Panel...\nWait a moment..."));
	m_plnkPanelAligner->InitAlignMarkOffset();
	iResult = m_plnkPanelAligner->ReturnRotatorUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	ProgWnd.SetText(_T("Transfer Rotator To Aligner...\nWait a moment..."));
	iResult = m_plnkCtrlPanelAligner->TransferPanel_RotatorToAligner();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	
	ProgWnd.SetText(_T("Return Rotator...\nWait a moment..."));
	iResult = m_plnkPanelAligner->ReturnRotatorUnit();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();
	/*/
}
