// FeederTeachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "TeachPressDlg.h"
#include "MPlatformOlbSystem.h"
#include "MSystemData.h"
#include "MTrsAutoManager.h"

#include <math.h>

#include "MyMsgWnd.h"
#include "MRFID.h"

#include "atlconv.h"

#include "DataMoldInfoDlg.h"  //For Test

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachPressDlg dialog
CTeachPressDlg::CTeachPressDlg(int iFixModel, int iGroupNo, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachPressDlg::IDD, pParent)
{ 
	//{{AFX_DATA_INIT(CTeachPressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iSelUnit = iGroupNo;
	m_iJogSelect = 0;
	m_iSelectedPos = DEF_PRESS_UP_POS;

	// Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	m_dDisplayOffsetZ = 0.0;

	// 전체 Position
	for (int i = 0; i < DEF_PRESS_MAX_POSITION; i++)
	{
		m_rgdFixedPosData[i] = 0.0;
	}

	m_plnkTabFeeder = MOlbSystemPre.GetTabFeederComponent(m_iSelUnit);
	
	m_plnkTabFeeder->GetPressPosParameter(m_rgdFixedPosData, m_rgdOffsetPosData);

	m_plnkC_MngOpPanel = MOlbSystemPre.GetManageOpPanelComponent();

	//130424.kms______
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	//________________

	m_pRFID = MOlbSystemPre.GetRFIDComponent(m_iSelUnit);
}


void CTeachPressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachPressDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDS_GROUP_TITLE,	m_lblGroup);
	DDX_Control(pDX, IDB_FEEDER_POS0,	m_BtnPos[0]);
	DDX_Control(pDX, IDB_FEEDER_POS1,	m_BtnPos[1]);
	DDX_Control(pDX, IDB_FEEDER_POS2,	m_BtnPos[2]);

	DDX_Control(pDX, IDC_TGT_POS_X,		m_sCoord[0]);
	DDX_Control(pDX, IDC_FIX_POS_X,		m_sCoord[1]);
	DDX_Control(pDX, IDC_OFFSET_POS_X,	m_sCoord[2]);
	DDX_Control(pDX, IDC_CUR_POS_X,		m_sCoord[3]);
	DDX_Control(pDX, IDC_DIF_POS_X,		m_sCoord[4]);

//	DDX_Control(pDX, IDC_CAM_POS_CAL,   m_sCamPosCal);
	DDX_Control(pDX, IDC_MOLD_REG_CNT,  m_sMoldRegCnt);

	//130503.kms___________
	DDX_Control(pDX, IDC_CAM_POS_0,   m_sCamPosData[0]);
	DDX_Control(pDX, IDC_CAM_POS_1,   m_sCamPosData[1]);
	DDX_Control(pDX, IDC_CAM_POS_2,   m_sCamPosData[2]);
	DDX_Control(pDX, IDC_CAM_POS_3,   m_sCamPosData[3]);
	DDX_Control(pDX, IDC_CAM_POS_4,   m_sCamPosData[4]);
	DDX_Control(pDX, IDC_CAM_POS_5,   m_sCamPosData[5]);
	DDX_Control(pDX, IDC_CAM_POS_6,   m_sCamPosData[6]);
	DDX_Control(pDX, IDC_CAM_POS_7,   m_sCamPosData[7]);
	DDX_Control(pDX, IDC_CAM_POS_8,   m_sCamPosData[8]);
	DDX_Control(pDX, IDC_CAM_POS_9,   m_sCamPosData[9]);
	DDX_Control(pDX, IDC_CAM_POS_10,   m_sCamPosData[10]);
	DDX_Control(pDX, IDC_CAM_POS_11,   m_sCamPosData[11]);
	DDX_Control(pDX, IDC_CAM_POS_12,   m_sCamPosData[12]);
	DDX_Control(pDX, IDC_CAM_POS_13,   m_sCamPosData[13]);
	DDX_Control(pDX, IDC_CAM_POS_14,   m_sCamPosData[14]);
	DDX_Control(pDX, IDC_CAM_POS_15,   m_sCamPosData[15]);
	DDX_Control(pDX, IDC_CAM_POS_16,   m_sCamPosData[16]);
	DDX_Control(pDX, IDC_CAM_POS_17,   m_sCamPosData[17]);
	DDX_Control(pDX, IDC_CAM_POS_18,   m_sCamPosData[18]);
	DDX_Control(pDX, IDC_CAM_POS_19,   m_sCamPosData[19]);
	DDX_Control(pDX, IDC_CAM_POS_20,   m_sCamPosData[20]);
	DDX_Control(pDX, IDC_CAM_POS_21,   m_sCamPosData[21]);
	DDX_Control(pDX, IDC_CAM_POS_22,   m_sCamPosData[22]);
	DDX_Control(pDX, IDC_CAM_POS_23,   m_sCamPosData[23]);
	DDX_Control(pDX, IDC_CAM_POS_24,   m_sCamPosData[24]);
	DDX_Control(pDX, IDC_CAM_POS_25,   m_sCamPosData[25]);
	DDX_Control(pDX, IDC_CAM_POS_26,   m_sCamPosData[26]);
	DDX_Control(pDX, IDC_CAM_POS_27,   m_sCamPosData[27]);
	DDX_Control(pDX, IDC_CAM_POS_28,   m_sCamPosData[28]);
	DDX_Control(pDX, IDC_CAM_POS_29,   m_sCamPosData[29]);
	DDX_Control(pDX, IDC_CAM_POS_30,   m_sCamPosData[30]);


	DDX_Control(pDX, IDC_CAM_POS_CAL0,   m_sCamPosCal[0]);
	DDX_Control(pDX, IDC_CAM_POS_CAL1,   m_sCamPosCal[1]);
	DDX_Control(pDX, IDC_CAM_POS_CAL2,   m_sCamPosCal[2]);
	DDX_Control(pDX, IDC_CAM_POS_CAL3,   m_sCamPosCal[3]);
	DDX_Control(pDX, IDC_CAM_POS_CAL4,   m_sCamPosCal[4]);
	DDX_Control(pDX, IDC_CAM_POS_CAL5,   m_sCamPosCal[5]);
	DDX_Control(pDX, IDC_CAM_POS_CAL6,   m_sCamPosCal[6]);
	DDX_Control(pDX, IDC_CAM_POS_CAL7,   m_sCamPosCal[7]);
	DDX_Control(pDX, IDC_CAM_POS_CAL8,   m_sCamPosCal[8]);
	DDX_Control(pDX, IDC_CAM_POS_CAL9,   m_sCamPosCal[9]);
	DDX_Control(pDX, IDC_CAM_POS_CAL10,   m_sCamPosCal[10]);
	DDX_Control(pDX, IDC_CAM_POS_CAL11,   m_sCamPosCal[11]);
	DDX_Control(pDX, IDC_CAM_POS_CAL12,   m_sCamPosCal[12]);
	DDX_Control(pDX, IDC_CAM_POS_CAL13,   m_sCamPosCal[13]);
	DDX_Control(pDX, IDC_CAM_POS_CAL14,   m_sCamPosCal[14]);
	DDX_Control(pDX, IDC_CAM_POS_CAL15,   m_sCamPosCal[15]);
	DDX_Control(pDX, IDC_CAM_POS_CAL16,   m_sCamPosCal[16]);
	DDX_Control(pDX, IDC_CAM_POS_CAL17,   m_sCamPosCal[17]);
	DDX_Control(pDX, IDC_CAM_POS_CAL18,   m_sCamPosCal[18]);
	DDX_Control(pDX, IDC_CAM_POS_CAL19,   m_sCamPosCal[19]);
	DDX_Control(pDX, IDC_CAM_POS_CAL20,   m_sCamPosCal[20]);
	DDX_Control(pDX, IDC_CAM_POS_CAL21,   m_sCamPosCal[21]);
	DDX_Control(pDX, IDC_CAM_POS_CAL22,   m_sCamPosCal[22]);
	DDX_Control(pDX, IDC_CAM_POS_CAL23,   m_sCamPosCal[23]);
	DDX_Control(pDX, IDC_CAM_POS_CAL24,   m_sCamPosCal[24]);
	DDX_Control(pDX, IDC_CAM_POS_CAL25,   m_sCamPosCal[25]);
	DDX_Control(pDX, IDC_CAM_POS_CAL26,   m_sCamPosCal[26]);
	DDX_Control(pDX, IDC_CAM_POS_CAL27,   m_sCamPosCal[27]);
	DDX_Control(pDX, IDC_CAM_POS_CAL28,   m_sCamPosCal[28]);
	DDX_Control(pDX, IDC_CAM_POS_CAL29,   m_sCamPosCal[29]);
	DDX_Control(pDX, IDC_CAM_POS_CAL30,   m_sCamPosCal[30]);

	DDX_Control(pDX, IDC_MOLD_REG_READ,   m_sMoldRead);
	//_______________________

}


BEGIN_MESSAGE_MAP(CTeachPressDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachPressDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachPressDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachPressDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachPressDlg)
	ON_EVENT(CTeachPressDlg, IDB_JOG, -600 /* Click */, OnJog, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_X, -600 /* Click */, OnX, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_FEEDER_POS0, -600 /* Click */, OnFeederPos0, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_FEEDER_POS1, -600 /* Click */, OnFeederPos1, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_FEEDER_POS2, -600 /* Click */, OnFeederPos2, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_CM_MOVE, -600 /* Click */, OnBmMove, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_POS_SET, -600 /* Click */, OnPosSet, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_CAM_POS_CAL, -600 /* Click */, OnClickCamPosCal, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_MOLD_REG_CNT, -600 /* Click */, OnClickMoldRegCnt, VTS_NONE)
	ON_EVENT(CTeachPressDlg, IDB_MOLD_REG_READ, -600 /* Click */, OnClickMoldRegRead, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachPressDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTeachPressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i = 0, iTemp = 0; 
	int iSelected = -1;

	if (m_iSelUnit == DEF_FRONT_GROUP)
	{
//		 Tab Press1 Z 고정좌표 Teaching
		CString szTextTemp;
		ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_1"), &szTextTemp);
		SetWindowText(szTextTemp);

		m_lblGroup.SetCaption(_T("Front Group"));
	}
	else	// if (m_iSelUnit == DEF_REAR_GROUP)
	{
//		 Tab Press2 Z 고정좌표 Teaching
		CString szTextTemp;
		ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_2"), &szTextTemp);
		SetWindowText(szTextTemp);

		m_lblGroup.SetCaption(_T("Rear Group"));
	}
	
	/** Teaching Position Name Reading */
	m_sTeachingInfo = m_plnkTabFeeder->GetPressTeachingInfo();

	for (int i = 0; i < DEF_PRESS_MAX_POSITION; i++)
	{
		if ((m_sTeachingInfo.m_sPosition[i]).m_bUse == TRUE)
		{
			m_BtnPos[iTemp].SetWindowText((LPCTSTR)((m_sTeachingInfo.m_sPosition[i]).m_strNewLineName[0]));
						
			if (m_sTeachingInfo.m_sPosition[i].m_bEnable == FALSE)
				m_BtnPos[iTemp].EnableWindow(FALSE);
			else
			{
				m_BtnPos[iTemp].ShowWindow(SW_SHOW);
				m_BtnPos[iTemp].EnableWindow(TRUE);
			}
			
			iTemp++;
			if (iSelected == -1 && m_sTeachingInfo.m_sPosition[i].m_bEnable == TRUE)
				iSelected = i;
		}
	}
	
	for (int i = iTemp; i < DEF_PRESS_MAX_POSITION; i++)
	{
		m_BtnPos[i].ShowWindow(SW_HIDE);
	}

	if (DEF_TABFEEDER1 == m_iSelUnit)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TAB_PRESS_Z);
	else // if (m_iSelUnit == DEF_TABFEEDER2)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TAB_PRESS_Z);

	UpdateCoordinate();

	//130503.kms____________
	//고정 값 Display
	WCHAR szPosData[250];

	for (int i = 0; i < 31; i++)
	{
//		m_dCamPosData = i / 10.0 ;
//		swprintf(szPosData, _T("%.1f"), m_dCamPosData);
		m_iCamPosData = i * 10 ;
		swprintf(szPosData, _T("%d"), m_iCamPosData);
		m_sCamPosData[i].SetCaption(szPosData);
	}
	//______________________


	//수정 필요
	//130424.kms_________
	//Display.....
	WCHAR szTemp[250];
//	WCHAR szTemp1[250];
	for (int i = 0; i < 31; i++)
	{
		if(m_iSelUnit == 0)
		{
			m_dCalResult[i] = m_plnkSystemData->m_dCalResult_F[i];
//			m_dMoldRegCnt = m_plnkSystemData->m_dMoldRegCnt_F[i];
		}
		else
		{
			m_dCalResult[i] = m_plnkSystemData->m_dCalResult_R[i];
//			m_dMoldRegCnt = m_plnkSystemData->m_dMoldRegCnt_R[i];
		}

		swprintf(szTemp, _T("%.3f"), m_dCalResult[i]);
		m_sCamPosCal[i].SetCaption(szTemp);
	}

//	swprintf(szTemp, _T("%.3f"), m_dCalResult);
//	m_sCamPosCal.SetCaption(szTemp);
//	swprintf(szTemp1, _T("%.3f"), m_dMoldRegCnt);
//	m_sMoldRegCnt.SetCaption(szTemp1);
	//___________________


	//130617.kms__________
	if(m_plnkSystemData->m_bUseMoldAutoSearch == TRUE)
	{
		GetDlgItem(IDB_X)->EnableWindow(FALSE);
		GetDlgItem(IDB_CAM_POS_CAL)->EnableWindow(FALSE);
		GetDlgItem(IDB_POS_SET)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDB_X)->EnableWindow(TRUE);
		GetDlgItem(IDB_CAM_POS_CAL)->EnableWindow(TRUE);
		GetDlgItem(IDB_POS_SET)->EnableWindow(TRUE);
	}
	//____________________

	// 기본 SelectPosition 설정 하기)
	if (m_iSelectedPos != -1)
		SelectPosition(m_iSelectedPos);
	m_uiTimerID = SetTimer(1, 200, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_3"), &szTextTemp);
	//축
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_4"), &szTextTemp);
	//Z 축
	GetDlgItem(IDB_X)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_5"), &szTextTemp);
	//목표 위치
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_6"), &szTextTemp);
	//고정 좌표
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_7"), &szTextTemp);
	//Offset 좌표
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_8"), &szTextTemp);
	//현재 위치
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_9"), &szTextTemp);
	//차이값
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_10"), &szTextTemp);
	//목표 위치로\r\n이동
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_11"), &szTextTemp);
	//목표 위치를\r\n현재값으로
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_12"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_13"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachPressDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		UpdateDisplay();
		UpdateButton();

		//@130710.KKY___________________
		if (TRUE == m_plnkC_MngOpPanel->GetStopSWStatus() && TRUE == m_plnkC_MngOpPanel->GetResetSWStatus())
		{
			m_lblGroup.ShowWindow(SW_SHOW);
			GetDlgItem(IDB_CAM_POS_CAL)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT)->ShowWindow(SW_SHOW);	
			GetDlgItem(IDS_GROUP_TITLE4)->ShowWindow(SW_SHOW);	
			GetDlgItem(IDS_GROUP_TITLE2)->ShowWindow(SW_SHOW);	
			GetDlgItem(IDB_MOLD_REG_READ)->ShowWindow(SW_SHOW);	
			m_sMoldRead.ShowWindow(SW_SHOW);	
			GetDlgItem(IDB_MOLD_REG_CNT)->ShowWindow(SW_SHOW);

			GetDlgItem(IDS_COUNT_0)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_7)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_8)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_10)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_11)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_12)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_13)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_14)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_15)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_16)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_17)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_18)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_19)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_20)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_21)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_22)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_23)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_24)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_25)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_26)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_27)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_28)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_29)->ShowWindow(SW_SHOW);
			GetDlgItem(IDS_COUNT_30)->ShowWindow(SW_SHOW);

			for (int i=0; i<31; i++)
			{
				m_sCamPosData[i].ShowWindow(SW_SHOW);
				m_sCamPosCal[i].ShowWindow(SW_SHOW);
			}
		}
		//@_____________________________

	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTeachPressDlg::OnJog() 
{
	// TODO: Add your control notification handler code here
	
}

void CTeachPressDlg::OnX() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;
	double dOldTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0].GetWindowText(cBeforeVal, 30);
	
	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	dOldTargetPos = m_rgdFixedPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), dNewTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	double dDiff = dNewTargetPos - dOldTargetPos;

	m_rgdFixedPosData[m_iSelectedPos] += dDiff;
	swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);

	UpdateCoordinate();
}

void CTeachPressDlg::OnBmMove() 
{
#ifndef DEF_SETUP
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;	
	}
#endif

//	 목표위치로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50700,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Move Target Position..Wait a moment..."));
	ProgWnd.PeekAndPump();

	// 이동 중 Jog를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

	int iResult = m_plnkTabFeeder->SafeMovePressPos(m_iSelectedPos);
	if (iResult != DEF_TEACH_PRESS_DLG_SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Press Motion Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	

	ProgWnd.StepIt();

	// Jog 사용 재개
	if (DEF_TABFEEDER1 == m_iSelUnit)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TAB_PRESS_Z);
	else // if (m_iSelUnit == DEF_TABFEEDER2)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TAB_PRESS_Z);
}

void CTeachPressDlg::OnPosSet() 
{
//	For Test
	/*
	for(int i = 0; i < 31; i++)
	{
		m_plnkSystemData->m_dCalResult_F[i] = 0.0;
		m_plnkSystemData->m_dCalResult_R[i] = 0.0;
	}
	m_plnkSystemData->WriteData();
	return;
	*/


	/*
//	For Test
	CDataMoldInfoDlg dlg(m_iSelUnit);
	dlg.DoModal();
	return;
	*/
	

//	 목표 위치를 현재 위치로 변경하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50701,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	double dCurrent = m_plnkTabFeeder->GetPressCurrentPos();
	double dTargetPos = dCurrent;
	
	WCHAR szTemp[250];	
	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	m_rgdFixedPosData[m_iSelectedPos] =	dTargetPos;

	swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
}

void CTeachPressDlg::OnSave() 
{
//	 Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50702,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	m_plnkTabFeeder->SavePressFixedPosParameter(m_rgdFixedPosData);
}

void CTeachPressDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	m_plnkC_MngOpPanel->SetJogUnit();
	
	CDialog::OnCancel();
}

/**
 * 목표위치, Mark Offset, 기타보정, 현재위치, 편차 Display (XYT축)
 */
void CTeachPressDlg::UpdateDisplay()
{
	//130424.kms_____
//	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos];
	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];
	
	/** 현재 위치 읽고, Display 하기 */
	double dCurrentPos = m_plnkTabFeeder->GetPressCurrentPos();
	
	// Reset Switch 감지시 현재 위치를 0으로 표시되게 하여 
	// Teaching시 상대거리 측정을 쉽게 한다.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
	{
		m_dDisplayOffsetZ = dCurrentPos;
	}
	
	dCurrentPos = dCurrentPos - m_dDisplayOffsetZ;	
	
	WCHAR szTemp[250];
	swprintf(szTemp, _T("%.3f"), dCurrentPos);
	m_sCoord[3].SetCaption(szTemp);
	
	/** 편차 계산하기 */
	swprintf(szTemp, _T("%.3f"), dCurrentPos - dTargetPos);
	m_sCoord[4].SetCaption(szTemp);
}

/**
 * 고정좌표, Offset좌표 Display
 */
void CTeachPressDlg::UpdateCoordinate()
{
	/** 목표 위치 */
	//130424.kms_____
//	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos];
	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];

	WCHAR szTemp[250];
	swprintf(szTemp, _T("%.3f"), dTargetPos);	
	m_sCoord[0].SetCaption(szTemp);

	/** 고정좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);

	//130422.kms__________
	swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos]);
	m_sCoord[2].SetCaption(szTemp);
	//___________________
}

/**
 * 선택 Position 변경 
 */
void CTeachPressDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}

/**
 * Command Button 상태 Update
 */
void CTeachPressDlg::UpdateButton()
{
}

void CTeachPressDlg::OnFeederPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);
}

void CTeachPressDlg::OnFeederPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}

void CTeachPressDlg::OnFeederPos2() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(2);
}


void CTeachPressDlg::OnClickCamPosCal() 
{
	// TODO: Add your control notification handler code here
	int iResult = 0;
	double PressCur_1 = 0.0;
	double PressCur_2 = 0.0;
//	WCHAR cAfterVal[100];
//	WCHAR cBeforeVal[100];
	WCHAR szTemp[250];
//	double dNewTargetPos;
	int i = 1;

	//1.측정 하시겠습니까?
	if (MyMessageBox(_T("Press Z Do you want to measure the length of the Cam in the Axis?\r\nPress Z Axis의 Cam의 길이를 측정 하시겠습니까?"),_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	//2.Press Z 축 원점 확인 
	iResult = m_plnkTabFeeder->CheckPressOrigin();
	if (iResult != DEF_TEACH_PRESS_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	

	//3.현재 위치 확인 현재 위치가 0의 위치 인지 확인 하도록 한다.
	//  강제 원점 복귀한 다음 이여야 한다.(질문)
	if (MyMessageBox(_T("Press Is the Z axis at the current zero position after forced reference point return?\r\nPress Z 축을 강제 원점 복귀 하여 현재 0 의 위치에 있습니까?"),_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	//4.JOG 동작 하여 금형의 단면이 아래로 내려가지 않고 하판에 일치 되는 위치 즉 Down 고정 좌표의 위치에 이동한다.
	MyMessageBox(_T("Press the Z axis to move to the position matching the lower plate with JOG. Press the OK button after completing the move.\r\nPress Z 축을 JOG로 하판과 일치 하는 위치에 이동 하여 주세요. 이동 완료후 확인 버튼을 눌러 주세요."), _T("Information"), M_ICONINFORMATION);

RETRYCNT:
	//차후 변수 명 변경......
	if(i > 30)
		return;

	//5.현재 위치_1 측정
	PressCur_1 = m_plnkTabFeeder->GetPressCurrentPos();

	//6.JOG 동작 하여 원하는 길이 가 되도록 내려 가도록 한다.
	//  측정완료가 되면 측정 완료 버튼을 누른다.
	MyMessageBox(_T("Press the Z axis to the desired position below the mold with JOG. move Press the OK button after completing.\r\nPress Z 축을 JOG로 금형 아래로 원하는 길이 만큼 이동 하여 주세요. 이동 완료후 확인 버튼을 눌러 주세요."), _T("Information"), M_ICONINFORMATION);

	//7.현재 위치_2 측정
	PressCur_2 = m_plnkTabFeeder->GetPressCurrentPos();
	
	//8.현재 위치_1 과 현재 위치_2를 비교 하여 길이 를 만든다.
	double PressCal = PressCur_1 - PressCur_2;
	PressCal = fabs(PressCal);
	
	//9.현재 금형 아래로 나온 길이가 실제로 얼마인지 입력 한다.
//	m_sCamPosCal.GetWindowText(cBeforeVal, 30);
	//  입력
//	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
//		return;

//	dNewTargetPos = wcstod(cAfterVal,NULL);

	//10.입력한 길이와 측정하여 비교된 길이를 연산 하여 0.1mm 의 경우 Motor의 Data가 
	//   얼마나 움직 여야 하는지 Calculate 한다.
//	m_dCalResult[i] = (PressCal * 0.1)/dNewTargetPos;
	m_dCalResult[i] = (PressCal * 0.1) / 0.1;

//	swprintf(szTemp, _T("%.3f"), m_dCalResult);
//	m_sCamPosCal.SetCaption(szTemp);

	//11.0.1mm당 Press Z Axis 이동 거리를 저장한다.
	//   SystemData Save
	m_dCalResult[0] = 0.0;

	if(m_iSelUnit == 0)
		m_plnkSystemData->m_dCalResult_F[i] = m_dCalResult[i];
	else
		m_plnkSystemData->m_dCalResult_R[i] = m_dCalResult[i];

	m_plnkSystemData->m_dCalResult_F[0] = m_dCalResult[0];
	m_plnkSystemData->m_dCalResult_R[0] = m_dCalResult[0];

	m_plnkSystemData->WriteData();

	swprintf(szTemp, _T("%.3f"), m_dCalResult[i]);
	m_sCamPosCal[i].SetCaption(szTemp);

	i++;

	goto RETRYCNT; 
}


void CTeachPressDlg::OnClickMoldRegRead() 
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	char cbuf[50];
	WCHAR szTemp1[250];
	BOOL m_bRfidReadingError = FALSE;

	//미리 더하기 해 놓는 변수.
	double m_dCalResult5, m_dCalResult10, m_dCalResult15, m_dCalResult20, m_dCalResult25, m_dCalResult30;

	m_dCalResult5 = m_dCalResult[0] + m_dCalResult[1] + m_dCalResult[2] + m_dCalResult[3] + m_dCalResult[4] +m_dCalResult[5];
	m_dCalResult10 = m_dCalResult5 + m_dCalResult[6] + m_dCalResult[7] + m_dCalResult[8] + m_dCalResult[9] +m_dCalResult[10];
	m_dCalResult15 = m_dCalResult10 + m_dCalResult[11] + m_dCalResult[12] + m_dCalResult[13] + m_dCalResult[14] + m_dCalResult[15];
	m_dCalResult20 = m_dCalResult15 + m_dCalResult[16] + m_dCalResult[17] + m_dCalResult[18] + m_dCalResult[19] + m_dCalResult[20];
	m_dCalResult25 = m_dCalResult20 + m_dCalResult[21] + m_dCalResult[22] + m_dCalResult[23] + m_dCalResult[24] + m_dCalResult[25];
	m_dCalResult30 = m_dCalResult25 + m_dCalResult[26] + m_dCalResult[27] + m_dCalResult[28] + m_dCalResult[29] + m_dCalResult[30];
	//_____________________________

	if (MyMessageBox(_T("Is the press Z axis in the Up position?\r\nPress Z 축이 Up 위치에 있습니까?"),_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyMsgWnd wndMyMsg(this);
	wndMyMsg.SetMessage(_T("Read mold data..Wait a moment..n"));
	wndMyMsg.Show();
	wndMyMsg.PeekAndPump();

	memset(cbuf,0,sizeof(cbuf));
	strTemp = m_pRFID->ReadData(MATCODE);
	strTemp = strTemp.Left(4);   //5자리를 써왔다... 4자리로만 계산 하는것이 맞는 거 같다.
	m_sMoldRead.SetCaption(strTemp);

	m_dMoldRead = wcstod(strTemp, NULL);

	m_dMoldRead = m_dMoldRead / 100.0;


//	m_FlexGrid.SetTextMatrix(4, 2, strTemp.GetBuffer(strTemp.GetLength()));
	wndMyMsg.PeekAndPump();

//	if (strTemp == "Data Read Error")
	if (strTemp == "Data")
	{
		MyMessageBox(MY_MESSAGE_BOX, 40700, _T("Error"), M_ICONERROR);
//		return;
		m_bRfidReadingError = TRUE;
	}

	if(m_dMoldRead > 39.10 || m_dMoldRead < 37.40)	
	{
		MyMessageBox(_T("Cutter tolerance read from mold (RFID) is out of range. Please change the mold."), _T("Error"), M_ICONERROR);
//		return;
		m_bRfidReadingError = TRUE;
	}

	//RFID 인식 안될경우를 대비 하여 수동으로 입력 하게 만들어 둠.
	if(m_bRfidReadingError == TRUE)
	{
		if (MyMessageBox(_T("Do you want to manually enter the RFID value? Please input only 4 digits when inputting.\r\nRFID값을 수동으로 입력 하시겠습니까? 입력시 4자리만 입력하여 주세요."),_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		WCHAR cAfterVal[100];
		WCHAR cBeforeVal[100];
		WCHAR szTemp[250];
		double dNewTargetPos;

		if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
			return;

//		m_dMoldRead = wcstod(cAfterVal,NULL);
		dNewTargetPos = wcstod(cAfterVal,NULL);
		swprintf(szTemp, _T("%.2f"), dNewTargetPos);

		m_sMoldRead.SetCaption(szTemp);

		m_dMoldRead = wcstod(szTemp, NULL);

		m_dMoldRead = m_dMoldRead / 100.0;
	}
	//________________________


	//RFID 읽은 String  값을 가지고 Offset 값을 만든다.
	//39.0은 고정 값...
	m_dMoldCalOffset = 39.0 - m_dMoldRead;
	m_dMoldCalOffset = fabs(m_dMoldCalOffset);
	m_dMoldCalOffset = m_dMoldCalOffset + 1.5;

	if(m_dMoldCalOffset <= 0.0)
		m_dMoldCalOffset = m_dCalResult[0];
	else if(m_dMoldCalOffset <= 0.1)
		m_dMoldCalOffset = m_dCalResult[0] + m_dCalResult[1];
	else if(m_dMoldCalOffset <= 0.2)
		m_dMoldCalOffset = m_dCalResult[0] + m_dCalResult[1] + m_dCalResult[2];
	else if(m_dMoldCalOffset <= 0.3)
		m_dMoldCalOffset = m_dCalResult[0] + m_dCalResult[1] + m_dCalResult[2] + m_dCalResult[3];
	else if(m_dMoldCalOffset <= 0.4)
		m_dMoldCalOffset = m_dCalResult[0] + m_dCalResult[1] + m_dCalResult[2] + m_dCalResult[3] + m_dCalResult[4];
	else if(m_dMoldCalOffset <= 0.5)
		m_dMoldCalOffset = m_dCalResult5;
	else if(m_dMoldCalOffset <= 0.6)
		m_dMoldCalOffset = m_dCalResult5 + m_dCalResult[6];
	else if(m_dMoldCalOffset <= 0.7)
		m_dMoldCalOffset = m_dCalResult5 + m_dCalResult[6] + m_dCalResult[7];
	else if(m_dMoldCalOffset <= 0.8)
		m_dMoldCalOffset = m_dCalResult5 + m_dCalResult[6] + m_dCalResult[7] + m_dCalResult[8];
	else if(m_dMoldCalOffset <= 0.9)
		m_dMoldCalOffset = m_dCalResult5 + m_dCalResult[6] + m_dCalResult[7] + m_dCalResult[8] + m_dCalResult[9];
	else if(m_dMoldCalOffset <= 1.0)
		m_dMoldCalOffset = m_dCalResult10;
	else if(m_dMoldCalOffset <= 1.1)
		m_dMoldCalOffset = m_dCalResult10 + m_dCalResult[11];
	else if(m_dMoldCalOffset <= 1.2)
		m_dMoldCalOffset = m_dCalResult10 + m_dCalResult[11] + m_dCalResult[12];
	else if(m_dMoldCalOffset <= 1.3)
		m_dMoldCalOffset = m_dCalResult10 + m_dCalResult[11] + m_dCalResult[12] + m_dCalResult[13];
	else if(m_dMoldCalOffset <= 1.4)
		m_dMoldCalOffset = m_dCalResult10 + m_dCalResult[11] + m_dCalResult[12] + m_dCalResult[13] + m_dCalResult[14];
	else if(m_dMoldCalOffset <= 1.5)
		m_dMoldCalOffset = m_dCalResult15;
	else if(m_dMoldCalOffset <= 1.6)
		m_dMoldCalOffset = m_dCalResult15 + m_dCalResult[16];
	else if(m_dMoldCalOffset <= 1.7)
		m_dMoldCalOffset = m_dCalResult15 + m_dCalResult[16] + m_dCalResult[17];
	else if(m_dMoldCalOffset <= 1.8)
		m_dMoldCalOffset = m_dCalResult15 + m_dCalResult[16] + m_dCalResult[17] + m_dCalResult[18];
	else if(m_dMoldCalOffset <= 1.9)
		m_dMoldCalOffset = m_dCalResult15 + m_dCalResult[16] + m_dCalResult[17] + m_dCalResult[18] + m_dCalResult[19];
	else if(m_dMoldCalOffset <= 2.0)
		m_dMoldCalOffset = m_dCalResult20;
	else if(m_dMoldCalOffset <= 2.1)
		m_dMoldCalOffset = m_dCalResult20 + m_dCalResult[21];
	else if(m_dMoldCalOffset <= 2.2)
		m_dMoldCalOffset = m_dCalResult20 + m_dCalResult[21] + m_dCalResult[22];
	else if(m_dMoldCalOffset <= 2.3)
		m_dMoldCalOffset = m_dCalResult20 + m_dCalResult[21] + m_dCalResult[22] + m_dCalResult[23];
	else if(m_dMoldCalOffset <= 2.4)
		m_dMoldCalOffset = m_dCalResult20 + m_dCalResult[21] + m_dCalResult[22] + m_dCalResult[23] + m_dCalResult[24];
	else if(m_dMoldCalOffset <= 2.5)
		m_dMoldCalOffset = m_dCalResult25;
	else if(m_dMoldCalOffset <= 2.6)
		m_dMoldCalOffset = m_dCalResult25 + m_dCalResult[26];
	else if(m_dMoldCalOffset <= 2.7)
		m_dMoldCalOffset = m_dCalResult25 + m_dCalResult[26] + m_dCalResult[27];
	else if(m_dMoldCalOffset <= 2.8)
		m_dMoldCalOffset = m_dCalResult25 + m_dCalResult[26] + m_dCalResult[27] + m_dCalResult[28];
	else if(m_dMoldCalOffset <= 2.9)
		m_dMoldCalOffset = m_dCalResult25 + m_dCalResult[26] + m_dCalResult[27] + m_dCalResult[28] + m_dCalResult[29];
	else if(m_dMoldCalOffset <= 3.0)
		m_dMoldCalOffset = m_dCalResult30;



	m_rgdOffsetPosData[DEF_PRESS_DOWN_POS] = - m_dMoldCalOffset;
//	m_rgdOffsetPosData[DEF_PRESS_DOWN_POS] = 0.0;

	m_plnkTabFeeder->SavePressOffsetPosParameter(m_rgdOffsetPosData);

	swprintf(szTemp1, _T("%.3f"), m_rgdOffsetPosData[DEF_PRESS_DOWN_POS]);
	m_sCoord[2].SetCaption(szTemp1);

	UpdateCoordinate();

	return;

}



void CTeachPressDlg::OnClickMoldRegCnt() 
{
	/*
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	WCHAR szTemp[250];
	WCHAR szTemp1[250];

	m_sMoldRegCnt.GetWindowText(cBeforeVal, 30);
	
	// 목표위치 입력 받기
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	// 목표위치 설정하기 
	m_dMoldRegCnt = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), m_dMoldRegCnt);
	m_sMoldRegCnt.SetCaption(szTemp);

	//System Data Save 필요
	if(m_iSelUnit == 0)
		m_plnkSystemData->m_dMoldRegCnt_F = m_dMoldRegCnt;
	else
		m_plnkSystemData->m_dMoldRegCnt_R = m_dMoldRegCnt;

	m_plnkSystemData->WriteData();

	m_rgdOffsetPosData[DEF_PRESS_DOWN_POS] = - ( (m_dCalResult * 10.0) + ( m_dCalResult * 10.0 * m_dMoldRegCnt * 0.07) );
//	m_rgdOffsetPosData[DEF_PRESS_DOWN_POS] = 0.0;

	m_plnkTabFeeder->SavePressOffsetPosParameter(m_rgdOffsetPosData);

	swprintf(szTemp1, _T("%.3f"), m_rgdOffsetPosData[DEF_PRESS_DOWN_POS]);
	m_sCoord[2].SetCaption(szTemp1);

	UpdateCoordinate();
	*/

	/*
	m_rgdFixedPosData[m_iSelectedPos] = m_dMoldRegCnt;
	swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
	*/
}

