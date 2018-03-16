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

	// Dispaly Offset �� (Reset S/W ������ ��� ��ġ �� Dispaly ����)
	m_dDisplayOffsetZ = 0.0;

	// ��ü Position
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
//		 Tab Press1 Z ������ǥ Teaching
		CString szTextTemp;
		ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_1"), &szTextTemp);
		SetWindowText(szTextTemp);

		m_lblGroup.SetCaption(_T("Front Group"));
	}
	else	// if (m_iSelUnit == DEF_REAR_GROUP)
	{
//		 Tab Press2 Z ������ǥ Teaching
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
	//���� �� Display
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


	//���� �ʿ�
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

	// �⺻ SelectPosition ���� �ϱ�)
	if (m_iSelectedPos != -1)
		SelectPosition(m_iSelectedPos);
	m_uiTimerID = SetTimer(1, 200, NULL);

	//������������������������������������������������������������������������������������������������������
	// Multi Language Button Title Display                                                                ��
	//������������������������������������������������������������������������������������������������������
	CString szTextTemp;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_3"), &szTextTemp);
	//��
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_4"), &szTextTemp);
	//Z ��
	GetDlgItem(IDB_X)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_5"), &szTextTemp);
	//��ǥ ��ġ
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_6"), &szTextTemp);
	//���� ��ǥ
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_7"), &szTextTemp);
	//Offset ��ǥ
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_8"), &szTextTemp);
	//���� ��ġ
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_9"), &szTextTemp);
	//���̰�
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_10"), &szTextTemp);
	//��ǥ ��ġ��\r\n�̵�
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_11"), &szTextTemp);
	//��ǥ ��ġ��\r\n���簪����
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_12"), &szTextTemp);
	//����
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPressDlg_13"), &szTextTemp);
	//������
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
	
	/** ��ǥ��ġ �Է� �ޱ� */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	dOldTargetPos = m_rgdFixedPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];

	/** ��ǥ��ġ �����ϱ� */
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

//	 ��ǥ��ġ�� �̵��Ͻðڽ��ϱ�?
	if (MyMessageBox(MY_MESSAGE_BOX,50700,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Move Target Position..Wait a moment..."));
	ProgWnd.PeekAndPump();

	// �̵� �� Jog�� ����ϰ� �Ǹ� Motion Error�� ���Ƿ� Jog ��� ����
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

	// Jog ��� �簳
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
	

//	 ��ǥ ��ġ�� ���� ��ġ�� �����Ͻðڽ��ϱ�?
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
//	 Data�� �����Ͻðڽ��ϱ�?
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
 * ��ǥ��ġ, Mark Offset, ��Ÿ����, ������ġ, ���� Display (XYT��)
 */
void CTeachPressDlg::UpdateDisplay()
{
	//130424.kms_____
//	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos];
	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];
	
	/** ���� ��ġ �а�, Display �ϱ� */
	double dCurrentPos = m_plnkTabFeeder->GetPressCurrentPos();
	
	// Reset Switch ������ ���� ��ġ�� 0���� ǥ�õǰ� �Ͽ� 
	// Teaching�� ���Ÿ� ������ ���� �Ѵ�.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
	{
		m_dDisplayOffsetZ = dCurrentPos;
	}
	
	dCurrentPos = dCurrentPos - m_dDisplayOffsetZ;	
	
	WCHAR szTemp[250];
	swprintf(szTemp, _T("%.3f"), dCurrentPos);
	m_sCoord[3].SetCaption(szTemp);
	
	/** ���� ����ϱ� */
	swprintf(szTemp, _T("%.3f"), dCurrentPos - dTargetPos);
	m_sCoord[4].SetCaption(szTemp);
}

/**
 * ������ǥ, Offset��ǥ Display
 */
void CTeachPressDlg::UpdateCoordinate()
{
	/** ��ǥ ��ġ */
	//130424.kms_____
//	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos];
	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];

	WCHAR szTemp[250];
	swprintf(szTemp, _T("%.3f"), dTargetPos);	
	m_sCoord[0].SetCaption(szTemp);

	/** ������ǥ ȭ�� Display */
	swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);

	//130422.kms__________
	swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos]);
	m_sCoord[2].SetCaption(szTemp);
	//___________________
}

/**
 * ���� Position ���� 
 */
void CTeachPressDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}

/**
 * Command Button ���� Update
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

	//1.���� �Ͻðڽ��ϱ�?
	if (MyMessageBox(_T("Press Z Do you want to measure the length of the Cam in the Axis?\r\nPress Z Axis�� Cam�� ���̸� ���� �Ͻðڽ��ϱ�?"),_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	//2.Press Z �� ���� Ȯ�� 
	iResult = m_plnkTabFeeder->CheckPressOrigin();
	if (iResult != DEF_TEACH_PRESS_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	

	//3.���� ��ġ Ȯ�� ���� ��ġ�� 0�� ��ġ ���� Ȯ�� �ϵ��� �Ѵ�.
	//  ���� ���� ������ ���� �̿��� �Ѵ�.(����)
	if (MyMessageBox(_T("Press Is the Z axis at the current zero position after forced reference point return?\r\nPress Z ���� ���� ���� ���� �Ͽ� ���� 0 �� ��ġ�� �ֽ��ϱ�?"),_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	//4.JOG ���� �Ͽ� ������ �ܸ��� �Ʒ��� �������� �ʰ� ���ǿ� ��ġ �Ǵ� ��ġ �� Down ���� ��ǥ�� ��ġ�� �̵��Ѵ�.
	MyMessageBox(_T("Press the Z axis to move to the position matching the lower plate with JOG. Press the OK button after completing the move.\r\nPress Z ���� JOG�� ���ǰ� ��ġ �ϴ� ��ġ�� �̵� �Ͽ� �ּ���. �̵� �Ϸ��� Ȯ�� ��ư�� ���� �ּ���."), _T("Information"), M_ICONINFORMATION);

RETRYCNT:
	//���� ���� �� ����......
	if(i > 30)
		return;

	//5.���� ��ġ_1 ����
	PressCur_1 = m_plnkTabFeeder->GetPressCurrentPos();

	//6.JOG ���� �Ͽ� ���ϴ� ���� �� �ǵ��� ���� ������ �Ѵ�.
	//  �����Ϸᰡ �Ǹ� ���� �Ϸ� ��ư�� ������.
	MyMessageBox(_T("Press the Z axis to the desired position below the mold with JOG. move Press the OK button after completing.\r\nPress Z ���� JOG�� ���� �Ʒ��� ���ϴ� ���� ��ŭ �̵� �Ͽ� �ּ���. �̵� �Ϸ��� Ȯ�� ��ư�� ���� �ּ���."), _T("Information"), M_ICONINFORMATION);

	//7.���� ��ġ_2 ����
	PressCur_2 = m_plnkTabFeeder->GetPressCurrentPos();
	
	//8.���� ��ġ_1 �� ���� ��ġ_2�� �� �Ͽ� ���� �� �����.
	double PressCal = PressCur_1 - PressCur_2;
	PressCal = fabs(PressCal);
	
	//9.���� ���� �Ʒ��� ���� ���̰� ������ ������ �Է� �Ѵ�.
//	m_sCamPosCal.GetWindowText(cBeforeVal, 30);
	//  �Է�
//	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
//		return;

//	dNewTargetPos = wcstod(cAfterVal,NULL);

	//10.�Է��� ���̿� �����Ͽ� �񱳵� ���̸� ���� �Ͽ� 0.1mm �� ��� Motor�� Data�� 
	//   �󸶳� ���� ���� �ϴ��� Calculate �Ѵ�.
//	m_dCalResult[i] = (PressCal * 0.1)/dNewTargetPos;
	m_dCalResult[i] = (PressCal * 0.1) / 0.1;

//	swprintf(szTemp, _T("%.3f"), m_dCalResult);
//	m_sCamPosCal.SetCaption(szTemp);

	//11.0.1mm�� Press Z Axis �̵� �Ÿ��� �����Ѵ�.
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

	//�̸� ���ϱ� �� ���� ����.
	double m_dCalResult5, m_dCalResult10, m_dCalResult15, m_dCalResult20, m_dCalResult25, m_dCalResult30;

	m_dCalResult5 = m_dCalResult[0] + m_dCalResult[1] + m_dCalResult[2] + m_dCalResult[3] + m_dCalResult[4] +m_dCalResult[5];
	m_dCalResult10 = m_dCalResult5 + m_dCalResult[6] + m_dCalResult[7] + m_dCalResult[8] + m_dCalResult[9] +m_dCalResult[10];
	m_dCalResult15 = m_dCalResult10 + m_dCalResult[11] + m_dCalResult[12] + m_dCalResult[13] + m_dCalResult[14] + m_dCalResult[15];
	m_dCalResult20 = m_dCalResult15 + m_dCalResult[16] + m_dCalResult[17] + m_dCalResult[18] + m_dCalResult[19] + m_dCalResult[20];
	m_dCalResult25 = m_dCalResult20 + m_dCalResult[21] + m_dCalResult[22] + m_dCalResult[23] + m_dCalResult[24] + m_dCalResult[25];
	m_dCalResult30 = m_dCalResult25 + m_dCalResult[26] + m_dCalResult[27] + m_dCalResult[28] + m_dCalResult[29] + m_dCalResult[30];
	//_____________________________

	if (MyMessageBox(_T("Is the press Z axis in the Up position?\r\nPress Z ���� Up ��ġ�� �ֽ��ϱ�?"),_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyMsgWnd wndMyMsg(this);
	wndMyMsg.SetMessage(_T("Read mold data..Wait a moment..n"));
	wndMyMsg.Show();
	wndMyMsg.PeekAndPump();

	memset(cbuf,0,sizeof(cbuf));
	strTemp = m_pRFID->ReadData(MATCODE);
	strTemp = strTemp.Left(4);   //5�ڸ��� ��Դ�... 4�ڸ��θ� ��� �ϴ°��� �´� �� ����.
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

	//RFID �ν� �ȵɰ�츦 ��� �Ͽ� �������� �Է� �ϰ� ����� ��.
	if(m_bRfidReadingError == TRUE)
	{
		if (MyMessageBox(_T("Do you want to manually enter the RFID value? Please input only 4 digits when inputting.\r\nRFID���� �������� �Է� �Ͻðڽ��ϱ�? �Է½� 4�ڸ��� �Է��Ͽ� �ּ���."),_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
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


	//RFID ���� String  ���� ������ Offset ���� �����.
	//39.0�� ���� ��...
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
	
	// ��ǥ��ġ �Է� �ޱ�
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	// ��ǥ��ġ �����ϱ� 
	m_dMoldRegCnt = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), m_dMoldRegCnt);
	m_sMoldRegCnt.SetCaption(szTemp);

	//System Data Save �ʿ�
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

