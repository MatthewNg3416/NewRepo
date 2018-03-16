// TeachCarrierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "TeachCarrierDlg.h"
#include "MPlatformOlbSystem.h"
#include "ManualTabCarrierDlg.h"
#include "VisionMonitorDlg.h"
#include "VisionCalibDlg.h"
#include "IAxis.h"
#include "MAxisData.h"
#include "Math.h"
#include "MTrsAutoManager.h"

#include "TeachMainForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachCarrierDlg dialog

CTeachCarrierDlg::CTeachCarrierDlg(int iFixModel, int iGroupNo, CWnd* pParent/*=NULL*/)
	: CDialog(CTeachCarrierDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeachCarrierDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iFixModel		= iFixModel;
	m_iSelectedPos	= DEF_TABCARRIER_READY_POS;
//	m_iJogSelect	= iGroupNo;
	m_iGroupNo		= iGroupNo;

	// Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	m_dDisplayOffset.Init(0.0, 0.0, 0.0);

	m_dDisplayOffsetZ = 0.0; //SJ_YYK 150109 Add..

	// 전체 Position
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		m_rgdFixedPosData[i].Init(0.0,0.0,0.0);
		m_rgdOffsetPosData[i].Init(0.0,0.0,0.0);
		m_rgdModelPosData[i].Init(0.0,0.0,0.0);
	}
	m_XYTMarkOffset.Init(0.0,0.0,0.0);

	m_iCarrierOldID = -1;
	m_iCarrierID = DEF_MAX_TABCARRIER-1;//@DEF_TABCARRIER4;

	if (iGroupNo == DEF_FRONT_GROUP)
		m_plnkTabCarrier	= MOlbSystemPre.GetTabCarrierComponent(m_iCarrierID);
	else
		m_plnkTabCarrier	= MOlbSystemPre.GetRTabCarrierComponent(m_iCarrierID);

	m_plnkC_MngOpPanel	= MOlbSystemPre.GetManageOpPanelComponent();
	m_plnkSystemData	= MOlbSystemPre.GetSystemData();
	
	m_plnkTabCarrier->GetXYTPosParameter(m_rgdFixedPosData, m_rgdOffsetPosData, m_rgdModelPosData);

	m_plnkTabCarrier->GetZPosParameter(m_rgdFixedZPosData, m_rgdOffsetZPosData, m_rgdModelZPosData); //SJ_YYK 150109 Add..
	m_bZSelected = FALSE;
	m_bYAxisSel = FALSE; //SJ_YYK 151021 Add..
}


void CTeachCarrierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachCarrierDlg)
	DDX_Control(pDX, IDB_X, m_ctrlBtnAxis[eAxisX]);
	DDX_Control(pDX, IDB_Y, m_ctrlBtnAxis[eAxisY]);
	DDX_Control(pDX, IDB_T, m_ctrlBtnAxis[eAxisT]);
	DDX_Control(pDX, IDB_Z, m_ctrlBtnAxis[eAxisZ]);

	//DDX_Control(pDX, IDC_GRD_AXISX_SNS_STS, m_grd_AxisSnsState[eAxisX]);
	//DDX_Control(pDX, IDC_GRD_AXISY_SNS_STS, m_grd_AxisSnsState[eAxisY]);
	//DDX_Control(pDX, IDC_GRD_AXIST_SNS_STS, m_grd_AxisSnsState[eAxisT]);
	//DDX_Control(pDX, IDC_GRD_AXISZ_SNS_STS, m_grd_AxisSnsState[eAxisZ]);

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDS_GROUP_TITLE,		m_lblGroup);
	DDX_Control(pDX, IDC_WORKER_NO,			m_lblWorkerNo);

	DDX_Control(pDX, IDB_SELECT_NEXT,		m_btnNextCarrier);
	DDX_Control(pDX, IDB_SELECT_PREVIOS,	m_btnPrevCarrier);

	DDX_Control(pDX, IDC_TGT_POS_X,			m_sCoord[0][0]);
	DDX_Control(pDX, IDC_FIX_POS_X,			m_sCoord[1][0]);
	DDX_Control(pDX, IDC_MODEL_POS_X,		m_sCoord[2][0]);
	DDX_Control(pDX, IDC_OFFSET_POS_X,		m_sCoord[3][0]);
	DDX_Control(pDX, IDC_CUR_POS_X,			m_sCoord[4][0]);
	DDX_Control(pDX, IDC_DIF_POS_X,			m_sCoord[5][0]);
	DDX_Control(pDX, IDC_PRE_OFFSET_POS_X,	m_sCoord[6][0]);

	DDX_Control(pDX, IDC_TGT_POS_Y,			m_sCoord[0][1]);
	DDX_Control(pDX, IDC_FIX_POS_Y,			m_sCoord[1][1]);
	DDX_Control(pDX, IDC_MODEL_POS_Y,		m_sCoord[2][1]);
	DDX_Control(pDX, IDC_OFFSET_POS_Y,		m_sCoord[3][1]);
	DDX_Control(pDX, IDC_CUR_POS_Y,			m_sCoord[4][1]);
	DDX_Control(pDX, IDC_DIF_POS_Y,			m_sCoord[5][1]);
	DDX_Control(pDX, IDC_PRE_OFFSET_POS_Y,	m_sCoord[6][1]);

	DDX_Control(pDX, IDC_TGT_POS_T,			m_sCoord[0][2]);
	DDX_Control(pDX, IDC_FIX_POS_T,			m_sCoord[1][2]);
	DDX_Control(pDX, IDC_MODEL_POS_T,		m_sCoord[2][2]);
	DDX_Control(pDX, IDC_OFFSET_POS_T,		m_sCoord[3][2]);
	DDX_Control(pDX, IDC_CUR_POS_T,			m_sCoord[4][2]);
	DDX_Control(pDX, IDC_DIF_POS_T,			m_sCoord[5][2]);
	DDX_Control(pDX, IDC_PRE_OFFSET_POS_T,	m_sCoord[6][2]);

	DDX_Control(pDX, IDC_TGT_POS_Z,			m_sCoord[0][3]);
	DDX_Control(pDX, IDC_FIX_POS_Z,			m_sCoord[1][3]);
	DDX_Control(pDX, IDC_MODEL_POS_Z,		m_sCoord[2][3]);
	DDX_Control(pDX, IDC_OFFSET_POS_Z,		m_sCoord[3][3]);
	DDX_Control(pDX, IDC_CUR_POS_Z,			m_sCoord[4][3]);
	DDX_Control(pDX, IDC_DIF_POS_Z,			m_sCoord[5][3]);
	DDX_Control(pDX, IDC_PRE_OFFSET_POS_Z,	m_sCoord[6][3]);

	DDX_Control(pDX, IDB_CARRIER_POS0,		m_BtnPos[0]);
	DDX_Control(pDX, IDB_CARRIER_POS1,		m_BtnPos[1]);
	DDX_Control(pDX, IDB_CARRIER_POS2,		m_BtnPos[2]);
	DDX_Control(pDX, IDB_CARRIER_POS3,		m_BtnPos[3]);
	DDX_Control(pDX, IDB_CARRIER_POS4,		m_BtnPos[4]);
	DDX_Control(pDX, IDB_CARRIER_POS5,		m_BtnPos[5]);
	DDX_Control(pDX, IDB_CARRIER_POS6,		m_BtnPos[6]);
	DDX_Control(pDX, IDB_CARRIER_POS7,		m_BtnPos[7]);
	DDX_Control(pDX, IDB_CARRIER_POS8,		m_BtnPos[8]);
	DDX_Control(pDX, IDB_CARRIER_POS9,		m_BtnPos[9]);
}


BEGIN_MESSAGE_MAP(CTeachCarrierDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachCarrierDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachCarrierDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachCarrierDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachCarrierDlg)
	ON_EVENT(CTeachCarrierDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_JOG, -600 /* Click */, OnClickJog, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_X, -600 /* Click */, OnClickX, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_Y, -600 /* Click */, OnClickY, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CM_MOVE, -600 /* Click */, OnClickCmMove, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_POS_SET, -600 /* Click */, OnClickPosSet, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS0, -600 /* Click */, OnClickCarrierPos0, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS1, -600 /* Click */, OnClickCarrierPos1, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS2, -600 /* Click */, OnClickCarrierPos2, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS3, -600 /* Click */, OnClickCarrierPos3, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS4, -600 /* Click */, OnClickCarrierPos4, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS5, -600 /* Click */, OnClickCarrierPos5, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS6, -600 /* Click */, OnClickCarrierPos6, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS7, -600 /* Click */, OnClickCarrierPos7, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS8, -600 /* Click */, OnClickCarrierPos8, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CARRIER_POS9, -600 /* Click */, OnClickCarrierPos9, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CM_CYL1, -600 /* Click */, OnClickCmCyl1, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_CM_VAC1, -600 /* Click */, OnClickCmVac1, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_VISION_CALIB, -600 /* Click */, OnClickVisionCalib, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_REGISTER_MARK, -600 /* Click */, OnClickRegisterMark, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_GET_ORIGIN_OFFSET, -600 /* Click */, OnClickGetOriginOffset, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_SELECT_PREVIOS, -600 /* Click */, OnClickSelectPrevios, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_SELECT_NEXT, -600 /* Click */, OnClickSelectNext, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_T, -600 /* Click */, OnClickT, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_TAB_ALIGN, -600 /* Click */, OnClickTabAlign, VTS_NONE)
	ON_EVENT(CTeachCarrierDlg, IDB_Z, -600 /* Click */, OnClickZ, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachCarrierDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CTeachCarrierDlg::updatePosButton()
{
	int i = 0;

	if(m_bZSelected == TRUE)
	{
		m_sTeachingZInfo = m_plnkTabCarrier->GetZTeachingInfo();
		int iMaxPos = DEF_TABCARRIER_Z_MAX_POS;
		for (int i = 0; i< DEF_TABCARRIER_TEACH_MAX_POS_BTN; i++)
			m_BtnPos[i].EnableWindow(TRUE);

		int iTemp = 0;
		int iSelected = m_iSelectedPos;
		for (int i = 0; i < DEF_TABCARRIER_Z_MAX_POS; i++)
		{
			if ((m_sTeachingZInfo.m_sPosition[i]).m_bUse == TRUE)
			{
				m_BtnPos[i].SetWindowText((LPCTSTR)((m_sTeachingZInfo.m_sPosition[i]).m_strNewLineName[0]));
							
				if (m_sTeachingZInfo.m_sPosition[i].m_bEnable == FALSE)
					m_BtnPos[i].EnableWindow(FALSE);
				else
				{
					m_BtnPos[i].ShowWindow(SW_SHOW);
					m_BtnPos[i].EnableWindow(TRUE);
				}
				iTemp++;			
				if (iSelected == -1 && m_sTeachingZInfo.m_sPosition[i].m_bEnable == TRUE)
					iSelected = i;
			}
		}
		
		// 사용하지 않는 Position Button 감추기
		for (int i = iTemp; i < DEF_TABCARRIER_TEACH_MAX_POS_BTN; i++)
		{
			m_BtnPos[i].ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_sTeachingInfo = m_plnkTabCarrier->GetXYTTeachingInfo();
		int iMaxPos = DEF_TABCARRIER_MAX_POSITION;
		for (int i = 0; i< DEF_TABCARRIER_TEACH_MAX_POS_BTN; i++)
			m_BtnPos[i].EnableWindow(TRUE);

		int iTemp = 0;
		int iSelected = m_iSelectedPos;
		for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
		{
			if ((m_sTeachingInfo.m_sPosition[i]).m_bUse == TRUE)
			{
				m_BtnPos[i].SetWindowText((LPCTSTR)((m_sTeachingInfo.m_sPosition[i]).m_strNewLineName[0]));
							
				if (m_sTeachingInfo.m_sPosition[i].m_bEnable == FALSE)
					m_BtnPos[i].EnableWindow(FALSE);
				else
				{
					m_BtnPos[i].ShowWindow(SW_SHOW);
					m_BtnPos[i].EnableWindow(TRUE);
				}
				iTemp++;			
				if (iSelected == -1 && m_sTeachingInfo.m_sPosition[i].m_bEnable == TRUE)
					iSelected = i;
			}
		}
		
		// 사용하지 않는 Position Button 감추기
		for (int i = iTemp; i < DEF_TABCARRIER_TEACH_MAX_POS_BTN; i++)
		{
			m_BtnPos[i].ShowWindow(SW_HIDE);
		}
	}

	m_BtnPos[DEF_TABCARRIER_ESCAPE_POS].EnableWindow(FALSE);
	GetDlgItem(IDB_CARRIER_POS5)->EnableWindow(FALSE);

	// 기본 selectPosition 설정 하기
	if (m_iSelectedPos != -1)
		SelectPosition(m_iSelectedPos);
	else
		SelectPosition(0);
}

void CTeachCarrierDlg::SetWorkCarrier(const int iCarrierID)
{
	if (m_iCarrierOldID == iCarrierID)
		return;

	if (m_iGroupNo == DEF_FRONT_GROUP)
	{
		m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iCarrierID);
		m_plnkCtrlTabCarrier = MOlbSystemPre.GetCtrlTabCarrierComponent(iCarrierID);
		m_iJogSelect = DEF_JOG_F_TABCARRIER1_XYT + iCarrierID;
	}
	else
	{
		m_plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(iCarrierID);
		m_plnkCtrlTabCarrier = MOlbSystemPre.GetCtrlRTabCarrierComponent(iCarrierID);
		m_iJogSelect = DEF_JOG_R_TABCARRIER1_XYT + iCarrierID;
	}

	m_plnkTabCarrier->GetXYTPosParameter(m_rgdFixedPosData, m_rgdOffsetPosData, m_rgdModelPosData);
	m_plnkC_MngOpPanel->SetJogUnit(m_iJogSelect);
	updatePosButton();	// Teaching Position Name Update
	updateCoordinate();	// Selected Position 에 대한 Coordinate Update
}

BOOL CTeachCarrierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE:
		{
	//		TabIC Carrier 모델좌표 Teaching
			CString szTextTemp;
			ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_1"), &szTextTemp);
			SetWindowText(szTextTemp);
			break;
		}
	case DEF_FIX_MODE:
		{
	//		TabIC Carrier 고정좌표 Teaching
			CString szTextTemp;
			///ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_2"), &szTextTemp);
			szTextTemp = _T("Fix Coord");
			SetWindowText(szTextTemp);
			break;
		}
	}

	if (m_iGroupNo == DEF_FRONT_GROUP)
		m_lblGroup.SetCaption(_T("Front Group"));
	else
		m_lblGroup.SetCaption(_T("Rear Group"));

	WCHAR szTemp[250];
	swprintf(szTemp, _T("%d"), m_iCarrierID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);

	if (m_iCarrierID == 0)
		m_btnPrevCarrier.EnableWindow(FALSE);
	if (m_iCarrierID == DEF_MAX_TABCARRIER - 1)
		m_btnNextCarrier.EnableWindow(FALSE);

	m_iSelectedPos = -1;

	SetWorkCarrier(m_iCarrierID);
	
//	// 작업에 따라 사용하지 않는 버튼 Disable 시키기.
//#ifdef DEF_SOURCE_SYSTEM
//	GetDlgItem(IDB_CM_CYL2)->EnableWindow(FALSE);
//	GetDlgItem(IDB_CM_CYL2)->ShowWindow(SW_HIDE);
//#endif

	//SJ_YYK 150109 Add..
	for(int i=0; i<7; i++)
	{
		m_sCoord[i][0].EnableWindow(TRUE);
		m_sCoord[i][1].EnableWindow(TRUE);
		m_sCoord[i][2].EnableWindow(TRUE);
		m_sCoord[i][3].EnableWindow(FALSE);
	}

	m_ctrlBtnAxis[eAxisZ].EnableWindow(FALSE);

	GetDlgItem(IDB_JOG)->EnableWindow(FALSE);


	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDB_GET_ORIGIN_OFFSET)->ShowWindow(SW_HIDE);
	}

	m_uiTimerID = SetTimer(1, 200, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_3"), &szTextTemp);
	//Jog 축 변경
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_4"), &szTextTemp);
	//X 축
	GetDlgItem(IDB_X)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_5"), &szTextTemp);
	//Y 축
	GetDlgItem(IDB_Y)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_6"), &szTextTemp);
	//목표 위치
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_7"), &szTextTemp);
	//고정 좌표
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_8"), &szTextTemp);
	//모델 좌표
	GetDlgItem(IDC_MODEL_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_9"), &szTextTemp);
	//Offset 좌표
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_10"), &szTextTemp);
	//현재 위치
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_11"), &szTextTemp);
	//차이값
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_12"), &szTextTemp);
	//목표 위치로\r\n이동
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_13"), &szTextTemp);
	//목표 위치를\r\n현재값으로
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_14"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_15"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_16"), &szTextTemp);
	//보정값
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);

	//원점복귀 Offset 얻기
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_17"), &szTextTemp);
	GetDlgItem(IDB_GET_ORIGIN_OFFSET)->SetWindowText(szTextTemp);

	//TabCarrier 번호 선택
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_18"), &szTextTemp);
	GetDlgItem(IDS_SELECT)->SetWindowText(szTextTemp);

	//T축
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_19"), &szTextTemp);
	GetDlgItem(IDB_T)->SetWindowText(szTextTemp);

	//Z축
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierDlg_20"), &szTextTemp);
	GetDlgItem(IDB_Z)->SetWindowText(szTextTemp);

	// Doolin kwangilshin 2017. 08. 24.
	//
	Init_GridAxisStste();
	// End

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachCarrierDlg::OnClickSave()
{
//	Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 50202,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	if (m_iFixModel == DEF_FIX_MODE)
	{
		/* 삭제. 위치 이동.
		// 1번 Carrier Vac Load 위치가 기준임. 나머지 Vac Load 위치는 Model 좌표 반영.
		for (int i = DEF_TABCARRIER_LOAD_POS1; i <= DEF_TABCARRIER_LOAD_POS8; i++)
		{
			m_rgdFixedPosData[i].dX = m_rgdFixedPosData[DEF_TABCARRIER_LOAD_POS1].dX;
			m_rgdFixedPosData[i].dY = m_rgdFixedPosData[DEF_TABCARRIER_LOAD_POS1].dY;
		}
		*/
		m_plnkTabCarrier->SaveXYTFixedPosParameter(m_rgdFixedPosData);

		m_plnkTabCarrier->SaveZFixedPosParamter(m_rgdFixedZPosData); //SJ_YYK 150109 Add..

		// Ready, Load 위치 강제 반영
		int iInstanceNo = m_plnkTabCarrier->GetInstanceNo();
		int iWorkInstance = m_plnkTabCarrier->GetWorkInstance();
		int iStart = DEF_TABCARRIER1;
		int iEnd = DEF_MAX_TABCARRIER;
		MPos_XYT posValue;
		int i = 0;
		
		if ( DEF_TABCARRIER1 == iWorkInstance)	//TabCarrier 고정좌표는 1번으로 Teaching한다
		{
			//Load위치는 1,3번 Carrier에 적용
			posValue = m_rgdFixedPosData[DEF_TABCARRIER_LOAD_POS];
			setCarrierPosOffset(DEF_TABCARRIER1, m_iFixModel, DEF_TABCARRIER_READY_POS, posValue);
			setCarrierPosOffset(DEF_TABCARRIER1, m_iFixModel, DEF_TABCARRIER_LOAD_POS, posValue);
			setCarrierPosOffset(DEF_TABCARRIER3, m_iFixModel, DEF_TABCARRIER_LOAD_POS, posValue);

			//PreAlign위치는 3번 Carrier에 적용
			posValue = m_rgdFixedPosData[DEF_TABCARRIER_PREALIGN_POS];
			setCarrierPosOffset(DEF_TABCARRIER1, m_iFixModel, DEF_TABCARRIER_PREALIGN_POS, posValue);
			setCarrierPosOffset(DEF_TABCARRIER3, m_iFixModel, DEF_TABCARRIER_PREALIGN_POS, posValue);

			
			for (int i = iStart; i < iEnd; i++)
			{
				posValue = m_rgdFixedPosData[DEF_TABCARRIER_UNLOAD_POS];
				posValue.dX = m_rgdFixedPosData[DEF_TABCARRIER_UNLOAD_POS].dX + DEF_TABCARRIER_PICKUP_UNIT_INTERVAL * i;				
				setCarrierPosOffset(i, m_iFixModel, DEF_TABCARRIER_UNLOAD_POS, posValue);
			}
		}

		else if ( DEF_TABCARRIER2 == iWorkInstance)
		{
			//Load위치는 2,4번 Carrier에 적용
			posValue = m_rgdFixedPosData[DEF_TABCARRIER_LOAD_POS];
			setCarrierPosOffset(DEF_TABCARRIER2, m_iFixModel, DEF_TABCARRIER_READY_POS, posValue);
			setCarrierPosOffset(DEF_TABCARRIER2, m_iFixModel, DEF_TABCARRIER_LOAD_POS, posValue);
			setCarrierPosOffset(DEF_TABCARRIER4, m_iFixModel, DEF_TABCARRIER_LOAD_POS, posValue);
			
			//Load위치는 4번 Ready위치
			for (int i = DEF_TABCARRIER3; i < iEnd; i++)
			{
				posValue = m_rgdFixedPosData[DEF_TABCARRIER_LOAD_POS];
				posValue.dX = m_rgdFixedPosData[DEF_TABCARRIER_LOAD_POS].dX + ( DEF_TABCARRIER_STANDARD_GAP * (i-1));
				setCarrierPosOffset(i, m_iFixModel, DEF_TABCARRIER_READY_POS, posValue);
			}

			//PreAlign위치는 4번 Carrier에 적용
			posValue = m_rgdFixedPosData[DEF_TABCARRIER_PREALIGN_POS];
			setCarrierPosOffset(DEF_TABCARRIER2, m_iFixModel, DEF_TABCARRIER_PREALIGN_POS, posValue);
			setCarrierPosOffset(DEF_TABCARRIER4, m_iFixModel, DEF_TABCARRIER_PREALIGN_POS, posValue);
		}
	}
	else
	{
		//SJ_PJC 20170412 최웅일과장 요청
		if(DEF_TABCARRIER1 == m_iCarrierID
		|| DEF_TABCARRIER2 == m_iCarrierID
		|| DEF_TABCARRIER3 == m_iCarrierID
		|| DEF_TABCARRIER4 == m_iCarrierID)
		{
			m_rgdOffsetPosData[DEF_TABCARRIER_READY_POS] = m_rgdOffsetPosData[DEF_TABCARRIER_LOAD_POS];
		}
		//______________________________
		m_plnkTabCarrier->SaveXYTOffsetPosParameter(m_rgdOffsetPosData);
		m_plnkTabCarrier->SaveZOffsetPosParameter(m_rgdOffsetZPosData);
	}
}

void CTeachCarrierDlg::setCarrierPosOffset(const int iTargetCarrierID, const int iFixModel, const int iPosID, const MPos_XYT posValue)
{
	if (iTargetCarrierID == m_iCarrierID && 
		(iPosID != DEF_TABCARRIER_READY_POS && iPosID != DEF_TABCARRIER_UNLOAD_POS))
		return;
	MPos_XYT rgsFixedPosData[DEF_TABCARRIER_MAX_POSITION];
	MPos_XYT rgsOffsetPosData[DEF_TABCARRIER_MAX_POSITION];
	MPos_XYT rgsModelPosData[DEF_TABCARRIER_MAX_POSITION];
	MTabCarrier* plnkTabCarrier;
	if (m_iGroupNo == DEF_FRONT_GROUP)
		plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iTargetCarrierID);
	else
		plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(iTargetCarrierID);
	
	plnkTabCarrier->GetXYTPosParameter(rgsFixedPosData, rgsOffsetPosData, rgsModelPosData);
	
	if (DEF_FIX_MODE == iFixModel)
	{
		rgsFixedPosData[iPosID]  = posValue;
		plnkTabCarrier->SaveXYTFixedPosParameter(rgsFixedPosData);
	}
	else if (DEF_MODEL_MODE == iFixModel)
	{
		rgsOffsetPosData[iPosID] = posValue;
		plnkTabCarrier->SaveXYTOffsetPosParameter(rgsOffsetPosData);
	}	
}

void CTeachCarrierDlg::OnClickExit()
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	m_plnkC_MngOpPanel->SetJogUnit();

	CDialog::OnCancel();
}

BOOL CTeachCarrierDlg::isEnabledToModifyPosValue()
{
	CString strMsg = "";
	if (DEF_MODEL_MODE == m_iFixModel)
	{
		//SJ_PJC 20170117 Del..
		/*/
		//20160603 HJG Offset 사용 하지 안도록....
		if(m_iSelectedPos == DEF_TABCARRIER_LOAD_POS
			|| m_iSelectedPos == DEF_TABCARRIER_PREALIGN_POS)
		{
			//Message_50218=TabCarrier Loading Pos, Pre Align Pos은 Offset Data를 사용 할 수 없습니다.
			MyMessageBox(MY_MESSAGE_BOX, 50218, _T("Data Input Error"), M_ICONINFORMATION);
			return FALSE;
		}
		else
			//_________________H
		/*/
		//_____________________
			return TRUE;
	}

	int iInstanceNo = m_plnkTabCarrier->GetInstanceNo();
	int iWorkInstanceNo = m_plnkTabCarrier->GetWorkInstance();

	switch (m_iSelectedPos)
	{
	case DEF_TABCARRIER_LOAD_POS:
	case DEF_TABCARRIER_PREALIGN_POS:
		if (DEF_TABCARRIER1 == iWorkInstanceNo || DEF_TABCARRIER2 == iWorkInstanceNo)
			return TRUE;
		MyMessageBox(MY_MESSAGE_BOX, 50209, _T("Chane Only 1th Carrier"), M_ICONINFORMATION);
		break;

	case DEF_TABCARRIER_UNLOAD_POS:
		if (DEF_TABCARRIER1 == iWorkInstanceNo)
			return TRUE;
		MyMessageBox(MY_MESSAGE_BOX, 50217, _T("Chane Only 1th Carrier"), M_ICONINFORMATION);
		break;

	case DEF_TABCARRIER_READY_POS:
		if (DEF_TABCARRIER1 == iWorkInstanceNo)
			return TRUE;
		else
			MyMessageBox(MY_MESSAGE_BOX, 50210, _T("Chane Only 1th Carrier"), M_ICONINFORMATION);
		break;
	default:
		ASSERT(0);
		break;
	}

	return FALSE;
}

void CTeachCarrierDlg::OnClickX() 
{
	m_bYAxisSel = FALSE; //SJ_YYK 151021 Add..
	if (FALSE == isEnabledToModifyPosValue())
		return;
//	int iWorkInstanceNo = m_plnkTabCarrier->GetWorkInstance();
//	if (DEF_TABCARRIER1 != iWorkInstanceNo)
//	{
//		MyMessageBox(MY_MESSAGE_BOX, 50213, _T("Chane Only 1th Carrier"), M_ICONINFORMATION);
//		return;
//	}


	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0][0].GetWindowText(cBeforeVal, 30);
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)))
	{
		return;
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	double dOldTargetPos
		= m_rgdFixedPosData[m_iSelectedPos].dX
		+ m_rgdModelPosData[m_iSelectedPos].dX
		+ m_rgdOffsetPosData[m_iSelectedPos].dX
		+ m_XYTMarkOffset.dX;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), dNewTargetPos);
	m_sCoord[0][0].SetCaption(szTemp);

	double dDiff = dNewTargetPos - dOldTargetPos;

	if (m_iFixModel == DEF_FIX_MODE)
	{
		m_rgdFixedPosData[m_iSelectedPos].dX += dDiff;
		/*
		m_rgdFixedPosData[m_iSelectedPos].dX = dNewTargetPos
												- m_rgdOffsetPosData[m_iSelectedPos].dX
												- m_rgdModelPosData[m_iSelectedPos].dX;
		*/
		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dX);
		m_sCoord[1][0].SetCaption(szTemp);

		/*
		if (MyMessageBox(MY_MESSAGE_BOX, 50211,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		MTabCarrier* pTabCarrier;
		MPos_XYT rgdFixedPosData[DEF_TABCARRIER_MAX_POSITION];
		MPos_XYT rgdOffsetPosData[DEF_TABCARRIER_MAX_POSITION];
		MPos_XYT rgdModelPosData[DEF_TABCARRIER_MAX_POSITION];
		for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		{
			if (m_iGroupNo == DEF_FRONT_GROUP)
				pTabCarrier = MOlbSystemPre.GetTabCarrierComponent(i);
			else
				pTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(i);
			pTabCarrier->GetXYTPosParameter(rgdFixedPosData, rgdOffsetPosData, rgdModelPosData);
			rgdFixedPosData[m_iSelectedPos].dX = m_rgdFixedPosData[m_iSelectedPos].dX;
		}
		*/
	}
	else
	{
		m_rgdOffsetPosData[m_iSelectedPos].dX += dDiff;
		/*
		m_rgdOffsetPosData[m_iSelectedPos].dX = dNewTargetPos
												- m_rgdFixedPosData[m_iSelectedPos].dX
												- m_rgdModelPosData[m_iSelectedPos].dX;
		*/
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dX);
		m_sCoord[3][0].SetCaption(szTemp);
	}
}

void CTeachCarrierDlg::OnClickY() 
{
	m_bYAxisSel = TRUE; //SJ_YYK 151021 Add...
	if (FALSE == isEnabledToModifyPosValue())
		return;

	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;
	WCHAR szTemp[250];

	m_sCoord[0][1].GetWindowText(cBeforeVal, 30);
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)))
	{
		return;
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	double dOldTargetPos
		= m_rgdFixedPosData[m_iSelectedPos].dY
		+ m_rgdModelPosData[m_iSelectedPos].dY
		+ m_rgdOffsetPosData[m_iSelectedPos].dY
		+ m_XYTMarkOffset.dY;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), dNewTargetPos);
	m_sCoord[0][1].SetCaption(szTemp);

	double dDiff = dNewTargetPos - dOldTargetPos;

	if (m_iFixModel == DEF_FIX_MODE)
	{
		m_rgdFixedPosData[m_iSelectedPos].dY += dDiff;
		/*
		m_rgdFixedPosData[m_iSelectedPos].dY = dNewTargetPos
												- m_rgdOffsetPosData[m_iSelectedPos].dY
												- m_rgdModelPosData[m_iSelectedPos].dY;
		*/
		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dY);
		m_sCoord[1][1].SetCaption(szTemp);
	}
	else
	{
		m_rgdOffsetPosData[m_iSelectedPos].dY += dDiff;
		/*
		m_rgdOffsetPosData[m_iSelectedPos].dY = dNewTargetPos
												- m_rgdFixedPosData[m_iSelectedPos].dY
												- m_rgdModelPosData[m_iSelectedPos].dY;
		*/
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dY);
		m_sCoord[3][1].SetCaption(szTemp);
	}
}

void CTeachCarrierDlg::OnClickT()
{
	// TODO: Add your control notification handler code here
	m_bYAxisSel = FALSE; //SJ_YYK 151021 Add..
	if (FALSE == isEnabledToModifyPosValue())
		return;
//	int iWorkInstanceNo = m_plnkTabCarrier->GetWorkInstance();
//	if (DEF_TABCARRIER1 != iWorkInstanceNo)
//	{
//		MyMessageBox(MY_MESSAGE_BOX, 50213, _T("Chane Only 1th Carrier"), M_ICONINFORMATION);
//		return;
//	}

	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;
	WCHAR szTemp[250];

	m_sCoord[0][2].GetWindowText(cBeforeVal, 30);

	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_T_AXIS)))
	{
		return;
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	double dOldTargetPos
		= m_rgdFixedPosData[m_iSelectedPos].dT
		+ m_rgdModelPosData[m_iSelectedPos].dT
		+ m_rgdOffsetPosData[m_iSelectedPos].dT
		+ m_XYTMarkOffset.dT;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), dNewTargetPos);
	m_sCoord[0][2].SetCaption(szTemp);

	double dDiff = dNewTargetPos - dOldTargetPos;

	if (m_iFixModel == DEF_FIX_MODE)
	{
		m_rgdFixedPosData[m_iSelectedPos].dT += dDiff;
		/*
		m_rgdFixedPosData[m_iSelectedPos].dY = dNewTargetPos
												- m_rgdOffsetPosData[m_iSelectedPos].dY
												- m_rgdModelPosData[m_iSelectedPos].dY;
		*/
		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dT);
		m_sCoord[1][2].SetCaption(szTemp);
	}
	else
	{
		m_rgdOffsetPosData[m_iSelectedPos].dT += dDiff;
		/*
		m_rgdOffsetPosData[m_iSelectedPos].dY = dNewTargetPos
												- m_rgdFixedPosData[m_iSelectedPos].dY
												- m_rgdModelPosData[m_iSelectedPos].dY;
		*/
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dT);
		m_sCoord[3][2].SetCaption(szTemp);
	}	
}

void CTeachCarrierDlg::OnClickCarrierPos0() 
{
	SelectPosition(0);
}

void CTeachCarrierDlg::OnClickCarrierPos1() 
{
	SelectPosition(1);
}

void CTeachCarrierDlg::OnClickCarrierPos2() 
{
	SelectPosition(2);	
}

void CTeachCarrierDlg::OnClickCarrierPos3() 
{
	SelectPosition(3);
}

void CTeachCarrierDlg::OnClickCarrierPos4() 
{
	SelectPosition(4);
}

void CTeachCarrierDlg::OnClickCarrierPos5() 
{
	SelectPosition(5);	
}

void CTeachCarrierDlg::OnClickCarrierPos6() 
{
	SelectPosition(6);
}

void CTeachCarrierDlg::OnClickCarrierPos7() 
{
	SelectPosition(7);
}

void CTeachCarrierDlg::OnClickCarrierPos8() 
{
	SelectPosition(8);
}

void CTeachCarrierDlg::OnClickCarrierPos9() 
{
	SelectPosition(9);
}

int CTeachCarrierDlg::getWorkCameraNo()
{
	int iCamNo = 0;
	if (m_iGroupNo == DEF_FRONT_GROUP)
		iCamNo = DEF_F_TABALIGNER_CAMERA_1;
	else
		iCamNo = DEF_R_TABALIGNER_CAMERA_1;
	return iCamNo;
}

void CTeachCarrierDlg::OnClickVisionCalib()
{
	// TODO: Add your control notification handler code here
	int iGroupID = DEF_CALIB_GROUP_TABCARRIER;

//	if (m_iGroupNo == DEF_FRONT_GROUP)
//		iGroupID = DEF_CALIB_GROUP_F_TABCARRIER;
//	else
//		iGroupID = DEF_CALIB_GROUP_R_TABCARRIER;

	int iCamNo = getWorkCameraNo();
	CVisionCalibDlg dlg(iCamNo, iGroupID, m_iCarrierID);
	dlg.DoModal();

	m_plnkTabCarrier->GetXYTPosParameter(m_rgdFixedPosData, m_rgdOffsetPosData, m_rgdModelPosData);
	updateDisplay();
	SelectPosition(m_iSelectedPos);	
}

void CTeachCarrierDlg::OnClickRegisterMark()
{
	// TODO: Add your control notification handler code here
	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
	int iCamNo = getWorkCameraNo();
	
	// Operation Type = DEF_PM_MARK / DEF_BLOB_MARK / DEF_CALIPER_MARK
	CVisionMonitorDlg dlg(plnkVision, DEF_PM_MARK, iCamNo);
	dlg.DoModal();
}

void CTeachCarrierDlg::OnClickCmMove() 
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
	if (MyMessageBox(MY_MESSAGE_BOX,50200,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	
	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("TabCarrier is Moving to Target Position...Wait a moment..."));
	ProgWnd.PeekAndPump();

	
	int iResult;

	// 이동 중 Jog 를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
	int iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

	//SJ_YYK 150109 Modify..
	if(m_bZSelected == TRUE)
	{
		iResult = m_plnkTabCarrier->SafeMoveZPos(m_iSelectedPos);		
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
	// aaaa		MyMessageBox(iResult, _T("TabCarrier Teaching Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
	}
	else
	{

		iResult = m_plnkTabCarrier->SafeMoveXYTPos(m_iSelectedPos,FALSE);		
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			// aaaa		MyMessageBox(iResult, _T("TabCarrier Teaching Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
	
	}

	ProgWnd.StepIt();

	// Jog 사용 재개
	m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
}

void CTeachCarrierDlg::OnClickPosSet()
{

	if(m_bZSelected == TRUE)
	{
		//	목표 위치를 현재 위치로 변경하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX,50201,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		WCHAR szTemp[250];

		double dCurPos = m_plnkTabCarrier->GetCurrentZPos();
		double dTargetPos = dCurPos;

		double dOldTgtPos = m_rgdFixedZPosData[m_iSelectedPos] + m_rgdModelZPosData[m_iSelectedPos] + m_rgdOffsetZPosData[m_iSelectedPos];
		
		swprintf(szTemp, _T("%.3f"), dTargetPos);
		m_sCoord[0][3].SetCaption(szTemp);

		double dDiff = dTargetPos - dOldTgtPos;

		switch (m_iFixModel)
		{
		case DEF_MODEL_MODE:
			m_rgdOffsetZPosData[m_iSelectedPos] = m_rgdOffsetZPosData[m_iSelectedPos] + dDiff;

			swprintf(szTemp, _T("%.3f"), m_rgdOffsetZPosData[m_iSelectedPos]);
			m_sCoord[3][3].SetCaption(szTemp);
			break;
			
		case DEF_FIX_MODE:
			m_rgdFixedZPosData[m_iSelectedPos] = m_rgdFixedZPosData[m_iSelectedPos] + dDiff;

			swprintf(szTemp, _T("%.3f"), m_rgdFixedZPosData[m_iSelectedPos]);
			m_sCoord[1][3].SetCaption(szTemp);
			break;

		default:
			break;
		}

	}
	else
	{
		if (FALSE == isEnabledToModifyPosValue())
			return;

	  //	목표 위치를 현재 위치로 변경하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX,50201,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		WCHAR szTemp[250];
		MPos_XYT xyCurrent = m_plnkTabCarrier->GetCurrentXYTPos();
		MPos_XYT xyTarget = xyCurrent;

		MPos_XYT xyOld = m_rgdFixedPosData[m_iSelectedPos] + m_rgdModelPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];

		swprintf(szTemp, _T("%.3f"), xyTarget.dX);
		m_sCoord[0][0].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), xyTarget.dY);
		m_sCoord[0][1].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), xyTarget.dT);
		m_sCoord[0][2].SetCaption(szTemp);

		MPos_XYT xyDiff = xyTarget - xyOld;

		switch (m_iFixModel)
		{
		case DEF_MODEL_MODE:
			m_rgdOffsetPosData[m_iSelectedPos] = m_rgdOffsetPosData[m_iSelectedPos] + xyDiff;
			/*
			m_rgdOffsetPosData[m_iSelectedPos] = xyTarget
												- m_rgdFixedPosData[m_iSelectedPos]
												- m_rgdModelPosData[m_iSelectedPos];
			*/
			swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dX);
			m_sCoord[3][0].SetCaption(szTemp);
			swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dY);
			m_sCoord[3][1].SetCaption(szTemp);
			swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dT);
			m_sCoord[3][2].SetCaption(szTemp);
			break;
			
		case DEF_FIX_MODE:
			m_rgdFixedPosData[m_iSelectedPos] = m_rgdFixedPosData[m_iSelectedPos] + xyDiff;
			/*
			m_rgdFixedPosData[m_iSelectedPos]	= xyTarget
												- m_rgdOffsetPosData[m_iSelectedPos]
												- m_rgdModelPosData[m_iSelectedPos];
			*/
			swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dX);
			m_sCoord[1][0].SetCaption(szTemp);
			swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dY);
			m_sCoord[1][1].SetCaption(szTemp);
			swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dT);
			m_sCoord[1][2].SetCaption(szTemp);
			break;

		default:
			break;
		}
	}
}

void CTeachCarrierDlg::OnClickCmCyl1() 
{
	CMyProgressWnd ProgWnd(NULL, _T("TabCarrier Up/Down..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("TabCarrier Up/Down Cylinder is operating..Wait a moment..."));
	ProgWnd.PeekAndPump();

	int iResult;

	if(m_iCarrierID  %2 == 0)
	{
		if (FALSE == m_plnkTabCarrier->IsUp())
			iResult = m_plnkTabCarrier->Up();
		else
			iResult = m_plnkTabCarrier->Down();
	}
	else
	{
		if (FALSE == m_plnkTabCarrier->IsUp2())
			iResult = m_plnkTabCarrier->Up2();
		else
			iResult = m_plnkTabCarrier->Down2();
	}

	if (iResult != ERR_TABCARRIER_SUCCESS)
	{
// aaaa		MyMessageBox(iResult, _T("TabCarrier Teaching Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}	
}

void CTeachCarrierDlg::OnClickCmVac1() 
{
	CManualTabCarrierDlg dlg(m_iGroupNo);
	dlg.DoModal();
}

void CTeachCarrierDlg::updateDisplay()
{
	WCHAR szTemp[250];
	MPos_XYT dCurrentPos;	
	MPos_XYT dTarget;
	MPos_XYT sPosMarkOffsetXYT;

	if(m_bZSelected == TRUE)
	{
		double dCurPos;
		double dTgtPos;

		dTgtPos = m_rgdFixedZPosData[m_iSelectedPos]
			+ m_rgdOffsetZPosData[m_iSelectedPos]
			+ m_rgdModelZPosData[m_iSelectedPos];

		swprintf(szTemp, _T("%.3f"), dTgtPos);	
		m_sCoord[0][3].SetCaption(szTemp);

		/** 현재 위치 읽고, Display 하기 */
		dCurPos = m_plnkTabCarrier->GetCurrentZPos();

		// Reset Switch 감지시 현재 위치를 0으로 표시되게 하여 
		// Teaching시 상대거리 측정을 쉽게 한다.
		if (m_plnkC_MngOpPanel->GetResetSWStatus())
			m_dDisplayOffsetZ = dCurPos;
		
		dCurPos = dCurPos - m_dDisplayOffsetZ;	

		/** 현재 위치 */
		swprintf(szTemp, _T("%.3f"), dCurPos);
		m_sCoord[4][3].SetCaption(szTemp);
		/** 편차 계산하기 */
		swprintf(szTemp, _T("%.3f"), dCurPos - dTgtPos);
		m_sCoord[5][3].SetCaption(szTemp);
		/** Align Offset */
		swprintf(szTemp, _T("%.3f"), 0.0);
		m_sCoord[6][3].SetCaption(szTemp);


	}
	else
	{

		dTarget = m_rgdFixedPosData[m_iSelectedPos] 
			+ m_rgdOffsetPosData[m_iSelectedPos] 
			+ m_rgdModelPosData[m_iSelectedPos] ;
		
		if (m_iSelectedPos == DEF_TABCARRIER_PREALIGN_POS || m_iSelectedPos == DEF_TABCARRIER_UNLOAD_POS)
		{
			sPosMarkOffsetXYT = m_plnkTabCarrier->CalculateAlignMarkOffset(dTarget);
			m_XYTMarkOffset = sPosMarkOffsetXYT;
		}
		else
		{
			m_XYTMarkOffset.Init(0.0, 0.0, 0.0);
		}
		dTarget = dTarget + m_XYTMarkOffset;

		swprintf(szTemp, _T("%.3f"), dTarget.dX);	
		m_sCoord[0][0].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), dTarget.dY);	
		m_sCoord[0][1].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), dTarget.dT);	
		m_sCoord[0][2].SetCaption(szTemp);


		/** 현재 위치 읽고, Display 하기 */
		dCurrentPos = m_plnkTabCarrier->GetCurrentXYTPos();
		
		// Reset Switch 감지시 현재 위치를 0으로 표시되게 하여 
		// Teaching시 상대거리 측정을 쉽게 한다.
		if (m_plnkC_MngOpPanel->GetResetSWStatus())
			m_dDisplayOffset = dCurrentPos;
		
		dCurrentPos = dCurrentPos - m_dDisplayOffset;	

		/** 현재 위치 */
		swprintf(szTemp, _T("%.3f"), dCurrentPos.dX);
		m_sCoord[4][0].SetCaption(szTemp);
		/** 편차 계산하기 */
		swprintf(szTemp, _T("%.3f"), dCurrentPos.dX - dTarget.dX);
		m_sCoord[5][0].SetCaption(szTemp);
		/** Align Offset */
		swprintf(szTemp, _T("%.3f"), m_XYTMarkOffset.dX);
		m_sCoord[6][0].SetCaption(szTemp);

		/** 현재 위치 */
		swprintf(szTemp, _T("%.3f"), dCurrentPos.dY);
		m_sCoord[4][1].SetCaption(szTemp);
		/** 편차 계산하기 */
		swprintf(szTemp, _T("%.3f"), dCurrentPos.dY - dTarget.dY);
		m_sCoord[5][1].SetCaption(szTemp);
		/** Align Offset */
		swprintf(szTemp, _T("%.3f"), m_XYTMarkOffset.dY);
		m_sCoord[6][1].SetCaption(szTemp);

		/** 현재 위치 */
		swprintf(szTemp, _T("%.3f"), dCurrentPos.dT);
		m_sCoord[4][2].SetCaption(szTemp);
		/** 편차 계산하기 */
		swprintf(szTemp, _T("%.3f"), dCurrentPos.dT - dTarget.dT);
		m_sCoord[5][2].SetCaption(szTemp);
		/** Align Offset */
		swprintf(szTemp, _T("%.3f"), m_XYTMarkOffset.dT);
		m_sCoord[6][2].SetCaption(szTemp); 
	}
}

/**
 * 고정좌표, Offset좌표 Display
 */
void CTeachCarrierDlg::updateCoordinate()
{
	WCHAR szTemp[250];
	if(m_bZSelected == TRUE)
	{
		/** 고정좌표 화면 Display */
		swprintf(szTemp, _T("%.3f"), m_rgdFixedZPosData[m_iSelectedPos]);
		m_sCoord[1][3].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), m_rgdModelZPosData[m_iSelectedPos]);
		m_sCoord[2][3].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetZPosData[m_iSelectedPos]);
		m_sCoord[3][3].SetCaption(szTemp);
	}
	else
	{
		MPos_XYT dTarget;
		
		/** 목표 위치 */
		//	dTarget = m_rgdFixedPosData[m_iSelectedPos] 
		//		+ m_rgdOffsetPosData[m_iSelectedPos] 
		//		+ m_rgdModelPosData[m_iSelectedPos];
		//
		//	swprintf(szTemp, _T("%.3f"), dTarget.dX);	
		//	m_sCoord[0][0].SetCaption(szTemp);
		//	swprintf(szTemp, _T("%.3f"), dTarget.dY);	
		//	m_sCoord[0][1].SetCaption(szTemp);
		//	swprintf(szTemp, _T("%.3f"), dTarget.dT);	
		//	m_sCoord[0][2].SetCaption(szTemp);
		
		/** 고정좌표 화면 Display */
		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dX);
		m_sCoord[1][0].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dY);
		m_sCoord[1][1].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos].dT);
		m_sCoord[1][2].SetCaption(szTemp);
		
		/** Model 화면 Display */
		swprintf(szTemp, _T("%.3f"), m_rgdModelPosData[m_iSelectedPos].dX);
		m_sCoord[2][0].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), m_rgdModelPosData[m_iSelectedPos].dY);
		m_sCoord[2][1].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), m_rgdModelPosData[m_iSelectedPos].dT);
		m_sCoord[2][2].SetCaption(szTemp);
		
		/** Offset 화면 Display */
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dX);
		m_sCoord[3][0].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dY);
		m_sCoord[3][1].SetCaption(szTemp);
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos].dT);
		m_sCoord[3][2].SetCaption(szTemp);
	}

	updateDisplay();
}

/**
 * 선택 Position 변경 
 */
void CTeachCarrierDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	updateCoordinate();
}

// Doolin kwangilshin 2017. 08. 24.
//
void CTeachCarrierDlg::Init_GridAxisStste()
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


BOOL CTeachCarrierDlg::Check_AxisStste(IAxis *pclsAxis)
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


void CTeachCarrierDlg::SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome)
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


void CTeachCarrierDlg::SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachCarrierDlg::SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachCarrierDlg::SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable)
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


void CTeachCarrierDlg::updateAxisSnsSts()
{
	IAxis *pclsAxis[eAxisEnd];
	
	pclsAxis[eAxisX] = m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS);
	pclsAxis[eAxisY] = m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS);
	pclsAxis[eAxisT] = m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_T_AXIS);
	pclsAxis[eAxisZ] = m_plnkTabCarrier->GetZMotionComponent();

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


void CTeachCarrierDlg::OnTimer(UINT nIDEvent) 
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

void CTeachCarrierDlg::updateButton()
{
}

void CTeachCarrierDlg::OnClickSelectPrevios() 
{
	// TODO: Add your control notification handler code here
	if (m_iCarrierID - 1 < DEF_TABCARRIER1)
		return;

	// 확인 필요 : Timer 와의 충돌 피하기 위한 처리 필요한가?
	m_iCarrierOldID = m_iCarrierID;

	m_iCarrierID--;

	m_btnNextCarrier.EnableWindow(TRUE);

	if (m_iCarrierID - 1 < DEF_TABCARRIER1)
		m_btnPrevCarrier.EnableWindow(FALSE);

	WCHAR szTemp[250];
	CMyProgressWnd ProgWnd(NULL, _T("Change Worker..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Change TabCarrier..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	ProgWnd.StepIt();
	
	swprintf(szTemp, _T("%d"), m_iCarrierID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
	
	SetWorkCarrier(m_iCarrierID);
	
	ProgWnd.StepIt();	
}

void CTeachCarrierDlg::OnClickSelectNext() 
{
	// TODO: Add your control notification handler code here
	if (m_iCarrierID + 1 >= DEF_MAX_TABCARRIER)
		return;

	// 확인 필요 : Timer 와의 충돌 피하기 위한 처리 필요한가?
	m_iCarrierOldID = m_iCarrierID;

	m_iCarrierID++;

	m_btnPrevCarrier.EnableWindow(TRUE);

	if (m_iCarrierID + 1 >= DEF_MAX_TABCARRIER)
		m_btnNextCarrier.EnableWindow(FALSE);

	CMyProgressWnd ProgWnd(NULL, _T("Change Worker..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Change TabCarrier..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	ProgWnd.StepIt();
	
	WCHAR szTemp[50];
	swprintf(szTemp, _T("%d"), m_iCarrierID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
	
	SetWorkCarrier(m_iCarrierID);
	
	ProgWnd.StepIt();	
}

void CTeachCarrierDlg::OnClickGetOriginOffset() 
{
	// TODO: Add your control notification handler code here
	CString strMsg = "";
	// 원점 복귀 완료 확인.
	if (FALSE == m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->IsOriginReturn()
		|| FALSE == m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->IsOriginReturn()
		|| FALSE == m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->IsOriginReturn())
	{
		//현재 TabMounter 원점 복귀 안됨.
		MyMessageBox(MY_MESSAGE_BOX,51110,_T("Error"), M_ICONERROR);
		return;
	}

	// 기준 위치로 이동 확인.
	//	 TabMounter가 기준 위치에 있습니까?
	int iResult = MyMessageBox(MY_MESSAGE_BOX,51111,_T("Check Pre-Condition"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
		return;
/*
	iResult = m_plnkTabMounter->UpCyl();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, "Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
*/
	// 현재 위치 저장
	double dCurrentPos = m_plnkTabCarrier->GetCurrentXYTPos().dY;
	double dCamCarrierCurPos = 0.0;
	
	// 원점 복귀 Offset 에 현재 위치 더하기.
	MAxisData sAxisData;
	m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->GetParameter(&sAxisData);
	double dHomeOffset = sAxisData.dHomeOffset;
	double dMechOffset = 0.0;

	double dPrismDist = 0.0;
	
//	#pragma message(__LOC__ "Gate Type에서도 추가 해야 하는지, 아니면 코드를 삭제해야 하는지 확인하기")
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		/*
		dPrismDist = m_plnkSystemData->m_rgdInspectionCamDistBetweenPrismViewCenter[DEF_F_TABALIGNER_CAMERA_2];
		dMechOffset = (dPrismDist - DEF_TABMOUNTER_TOOL_SIZE) / 2 + DEF_TABMOUNTER_TOOL_SIZE;
		// (+) -> (-) 방향으로 Offset 측정
		dHomeOffset += dCurrentPos - dMechOffset + dCamCarrierCurPos;
		*/
		dPrismDist = m_plnkSystemData->m_rgdInspectionCamDistViewCenter_Expand[m_iGroupNo][DEF_TABALIGNER_CARRIER_1];
		dMechOffset = (dPrismDist - DEF_TABCARRIER_TOOL_SIZE) / 2.0;
		dHomeOffset += dCamCarrierCurPos - (dCurrentPos - dMechOffset);
	}

	// 원점 복귀 Offset 저장
	sAxisData.dHomeOffset = dHomeOffset;
	m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->SaveParameter(&sAxisData);
	
	// 원점 복귀 Flag Clear
	m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->ResetOrigin();
	m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->ResetOriginComplete();
	
	// 원점 복귀 재요구.
//	 TabMounter 원점 복귀 Offset 계산 완료.\r\n원점 복귀 완료 Flag를 해제함.
	MyMessageBox(MY_MESSAGE_BOX,51112,_T("Operation Complete"), M_ICONINFORMATION);
	
}

void CTeachCarrierDlg::OnClickJog() 
{
	// TODO: Add your control notification handler code here
	if (m_bZSelected == FALSE)
	{
		m_bZSelected = TRUE;
		if (m_iGroupNo == DEF_FRONT_GROUP)
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABCARRIER_Z);
		else
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABCARRIER_Z);

		for (int i=0; i<7; i++)
		{
			m_sCoord[i][0].EnableWindow(FALSE);
			m_sCoord[i][1].EnableWindow(FALSE);
			m_sCoord[i][2].EnableWindow(FALSE);
			m_sCoord[i][3].EnableWindow(TRUE);
		}
//		m_ctrlBtnSelectJog.EnableWindow(FALSE);
		
		m_ctrlBtnAxis[eAxisX].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisY].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisT].EnableWindow(FALSE);
		m_ctrlBtnAxis[eAxisZ].EnableWindow(TRUE);

		SelectPosition(0);
		updatePosButton();	// Teaching Position Name Update
		updateCoordinate();	// Selected Position 에 대한 Coordinate Update
	}
	else
	{
		m_bZSelected = FALSE;
		SetWorkCarrier(m_iCarrierID);
	
		for (int i=0; i<7; i++)
		{
			m_sCoord[i][0].EnableWindow(TRUE);
			m_sCoord[i][1].EnableWindow(TRUE);
			m_sCoord[i][2].EnableWindow(TRUE);
			m_sCoord[i][3].EnableWindow(FALSE);
		}
	
		m_ctrlBtnAxis[eAxisX].EnableWindow(TRUE);
		m_ctrlBtnAxis[eAxisY].EnableWindow(TRUE);
		m_ctrlBtnAxis[eAxisT].EnableWindow(TRUE);
		m_ctrlBtnAxis[eAxisZ].EnableWindow(FALSE);
	}
}

void CTeachCarrierDlg::OnClickTabAlign() 
{
	// TODO: Add your control notification handler code here
	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		//OP Mode Dry Run 상태입니다. OP Mode를 바꿔주세요!
		MyMessageBox(MY_MESSAGE_BOX, 54005, _T("Error!"), M_ICONERROR);

		return;
	}

	//Panel Align을 하시겠습니까?
	///if (MyMessageBox(MY_MESSAGE_BOX, 54006, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
	if (MyMessageBox(_T("TAB ALIGN?"), _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

	//______________
	CMyProgressWnd ProgWnd(NULL, _T("TabCarrier Tab Align..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabCarrier Tab Align.\nWait for one moment..."));
	ProgWnd.PeekAndPump();

	MPos_XYT	m_posAlignOffset;
	int m_uiRetryCount = 0;
RETRAY:
	m_posAlignOffset.Init(0.0, 0.0, 0.0);	
	int iResult = m_plnkCtrlTabCarrier->AlignTabIC(&m_posAlignOffset);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		//MyMessageBox(strError.GetBuffer(strError.GetLength()),"Mark Align Error",M_ICONERROR);
// jdy sesl		MyMessageBox(iResult,_T("Mark Align Error"),M_ICONERROR);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	TRACE(_T("retry num : %d  ,초기 Offset : %.5lf\n"), m_uiRetryCount, m_posAlignOffset.dT);
	///if(fabs(m_posAlignOffset.dT) > 0.3)
	if(FALSE)
	{	
		Sleep(700);
		m_uiRetryCount++;
		if (m_uiRetryCount >= 5)
		{
			m_uiRetryCount = 0;
			MyMessageBox(_T("Check the angle result is over 0.3 degrees.\r\n") , _T("Error"), M_ICONERROR);
			return;
		}
		goto RETRAY;
	}
	
	char  str[100];
	sprintf(str,"Tab Align Succes.\nX:%.3f Y:%.3f T: %.3f", m_posAlignOffset.dX, m_posAlignOffset.dY, m_posAlignOffset.dT);
	MyMessageBox(str, _T("Panel Align Success"), M_ICONERROR);	
	
}

void CTeachCarrierDlg::OnClickZ() 
{
	// TODO: Add your control notification handler code here

	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;
	WCHAR szTemp[250];

	m_sCoord[0][3].GetWindowText(cBeforeVal, 30);

	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkTabCarrier->GetZMotionComponent()))
	{
		return;
	}
	// End.
	
	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	double dOldTargetPos
		= m_rgdFixedZPosData[m_iSelectedPos]
		+ m_rgdModelZPosData[m_iSelectedPos]
		+ m_rgdOffsetZPosData[m_iSelectedPos];

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), dNewTargetPos);
	m_sCoord[0][3].SetCaption(szTemp);

	double dDiff = dNewTargetPos - dOldTargetPos;

	if (m_iFixModel == DEF_FIX_MODE)
	{
		m_rgdFixedZPosData[m_iSelectedPos] += dDiff;
		
		swprintf(szTemp, _T("%.3f"), m_rgdFixedZPosData[m_iSelectedPos]);
		m_sCoord[1][3].SetCaption(szTemp);
	}
	else
	{
		m_rgdOffsetZPosData[m_iSelectedPos] += dDiff;
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetZPosData[m_iSelectedPos]);
		m_sCoord[3][3].SetCaption(szTemp);
	}	
}
	
