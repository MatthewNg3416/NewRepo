// BadTabTeachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "TeachBadTabDetectorDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTabFeeder.h"
#include "MCtrlTabFeeder.h"
#include "MManageOpPanel.h"
#include "MPreBonderData.h"
#include "MSystemData.h"
#include "MProcessData.h"
#include "MTabMounter.h"
#include "IVision.h"
#include "VisionMonitorDlg.h"
#include <math.h>
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachBadTabDetectorDlg dialog

CTeachBadTabDetectorDlg::CTeachBadTabDetectorDlg(int iFixModel, int iGroupNo, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachBadTabDetectorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTeachBadTabDetectorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iGroupNo		= iGroupNo; 
	m_iSelectedPos	= DEF_BADTABDETECTOR_WORK_POS;
	
	// Dispaly Offset �� (Reset S/W ������ ��� ��ġ �� Dispaly ����)
	m_dDisplayOffsetZ = 0.0;

	// ��ü Position
	for (int i = 0; i < DEF_BADTABDETECTOR_MAX_POSITION; i++)
	{
		m_rgdModelPosData[i]	= 0.0;
		m_rgdOffsetPosData[i]	= 0.0;
	}

	if (DEF_FRONT_GROUP == iGroupNo)
	{
		m_plnkTabFeeder		= MOlbSystemPre.GetTabFeederComponent(DEF_TABFEEDER1);
		m_plnkCtrlTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1);
	}
	else
	{
		m_plnkTabFeeder		= MOlbSystemPre.GetTabFeederComponent(DEF_TABFEEDER2);
		m_plnkCtrlTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2);
	}

	m_plnkSystemData		= MOlbSystemPre.GetSystemData();
	m_plnkC_MngOpPanel		= MOlbSystemPre.GetManageOpPanelComponent();
	m_plnkPreBonderData		= MOlbSystemPre.GetPreBonderData();

	m_plnkTabFeeder->GetBadTabPosParameter(m_rgdOffsetPosData, m_rgdModelPosData);
}

void CTeachBadTabDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachBadTabDetectorDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDS_GROUP_TITLE,	m_lblGroup);
	DDX_Control(pDX, IDB_X,				m_BtnAxis[0]);

	DDX_Control(pDX, IDB_BADTAB_POS0,	m_BtnPos[0]);
	DDX_Control(pDX, IDB_BADTAB_POS1,	m_BtnPos[1]);
	
	DDX_Control(pDX, IDC_TGT_POS_Z,		m_sCoord[0]);
	DDX_Control(pDX, IDC_MODEL_POS_Z,	m_sCoord[1]);
	DDX_Control(pDX, IDC_OFFSET_POS_Z,	m_sCoord[2]);
	DDX_Control(pDX, IDC_CUR_POS_Z,		m_sCoord[3]);
	DDX_Control(pDX, IDC_DIF_POS_Z,		m_sCoord[4]);

	DDX_Control(pDX, IDC_TENSION_OFFSET, m_sTensionOffset);
	DDX_Control(pDX, IDB_REEL_DOWN,		m_btnReelDown);
	DDX_Control(pDX, IDB_REEL_UP,		m_btnReelUp);
}


BEGIN_MESSAGE_MAP(CTeachBadTabDetectorDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachBadTabDetectorDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachBadTabDetectorDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachBadTabDetectorDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachBadTabDetectorDlg)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_JOG, -600 /* Click */,				OnJog, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_X, -600 /* Click */,					OnZ1, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_BADTAB_POS0, -600 /* Click */,		OnBadtabPos0, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_BADTAB_POS1, -600 /* Click */,		OnBadtabPos1, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_CM_MOVE, -600 /* Click */,			OnCmMove, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_POS_SET, -600 /* Click */,			OnPosSet, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_SAVE, -600 /* Click */,				OnSave, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_EXIT, -600 /* Click */,				OnExit, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_CM_SPROCKET1_HOMING, -600 /* Click */,OnClickCmSprocket1Homing, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_CM_SPROCKET2_HOMING, -600 /* Click */,OnClickCmSprocket2Homing, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_CM_CHECK_CUTTING_LENGTH, -600 /* Click */,OnCheckCuttingLength, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_TAB_CUTTING_OFFSET, -600 /* Click */,	OnClickTabCuttingOffset, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_REEL_UP, -600 /* Click */,			OnClickReelUp, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_REEL_DOWN, -600 /* Click */,			OnClickReelDown, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_REEL_CLAMP_UP, -600 /* Click */,		OnClickReelClampUp, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_REEL_CLAMP_DOWN, -600 /* Click */,	OnClickReelClampDown, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_CM_SPROCKET1_JOG, -600 /* Click */, OnClickCmSprocket1Jog, VTS_NONE)
	ON_EVENT(CTeachBadTabDetectorDlg, IDB_CM_SPROCKET2_JOG, -600 /* Click */, OnClickCmSprocket2Jog, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachBadTabDetectorDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTeachBadTabDetectorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i = 0, iTemp = 0; 
	int iSelected = -1;
	
	/** Teaching Position Name Reading (Z ��) */
	m_sTeachingInfo = m_plnkTabFeeder->GetBadTabTeachingInfo();

	for (int i = 0; i < DEF_BADTABDETECTOR_MAX_POSITION; i++)
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

	//hongju �ӽ� ����...Ready ��ư ��� ���ϰ�..
//	SelectPosition(iSelected);
	SelectPosition(1);
	m_BtnPos[0].ShowWindow(SW_HIDE);

	// ������� �ʴ� Position Button ���߱�
	for (int i = iTemp; i < DEF_BADTABDETECTOR_MAX_POSITION; i++)
	{
		m_BtnPos[i].ShowWindow(SW_HIDE);
	}

	if (DEF_FRONT_GROUP == m_iGroupNo)
	{
//		BadTab Detector1 X ����ǥ Teaching
		CString szTextTemp;
		//���ڰ� ���� �ö� ���� ���̴� ���� ������ �ϵ��ڵ���
//		ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_1"), &szTextTemp);
//		SetWindowText(szTextTemp);
		SetWindowText(_T("BadTab Detector1 X ����ǥ Teaching"));

		m_lblGroup.SetCaption(_T("Front Group"));
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_BADTAB_DETECTOR_X);
	}
	else
	{
//		BadTab Detector2 X ����ǥ Teaching
		CString szTextTemp;
		//���ڰ� ���� �ö� ���� ���̴� ���� ������ �ϵ��ڵ���
//		ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_2"), &szTextTemp);
//		SetWindowText(szTextTemp);
		SetWindowText(_T("BadTab Detector2 X ����ǥ Teaching"));

		m_lblGroup.SetCaption(_T("Rear Group"));
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_BADTAB_DETECTOR_X);
	}

	getUserOffsetX_withDisplay();

	UpdateCoordinate();

	m_uiTimerID = SetTimer(1, 200, NULL);

	//������������������������������������������������������������������������������������������������������
	// Multi Language Button Title Display                                                                ��
	//������������������������������������������������������������������������������������������������������
	CString szTextTemp;

	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_3"), &szTextTemp);
	//��
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_4"), &szTextTemp);
	//X ��
	GetDlgItem(IDB_X)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_5"), &szTextTemp);
	//��ǥ ��ġ
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_6"), &szTextTemp);
	//Model ��ǥ
	GetDlgItem(IDC_MODEL_POS)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_7"), &szTextTemp);
//	Offset ��ǥ
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_8"), &szTextTemp);
//	���� ��ġ
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_9"), &szTextTemp);
//	���̰�
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_10"), &szTextTemp);
//	��ǥ ��ġ��\r\n�̵�
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_11"), &szTextTemp);
//	��ǥ ��ġ��\r\n���簪����
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_12"), &szTextTemp);
//	����
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_13"), &szTextTemp);
//	������
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_14"), &szTextTemp);
//	Ÿ�� ���� �̼�����(����:mm)	
	GetDlgItem(IDS_STATE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachBadTabDetectorDlg_15"), &szTextTemp);
//	Ÿ�� ���� Ȯ��
	GetDlgItem(IDB_CM_CHECK_CUTTING_LENGTH)->SetWindowText(szTextTemp);
	
	m_iSp1JogSel = 0;
	m_iSp2JogSel = 0;

	GetDlgItem(IDB_CM_SPROCKET1_JOG)->SetWindowText(_T("Selected\nBadTab_X"));

	m_btnReelUp.SetCaption(_T("Mold Pin\r\nUp"));
	m_btnReelDown.SetCaption(_T("Mold Pin\r\nDown"));

	GetDlgItem(IDB_CM_SPROCKET1_HOMING)->EnableWindow(FALSE);
	GetDlgItem(IDB_REEL_CLAMP_UP)->SetWindowText(_T("Sprocket1\nClutch On"));
	GetDlgItem(IDB_REEL_CLAMP_DOWN)->SetWindowText(_T("Sprocket1\nClutch Off"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachBadTabDetectorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		UpdateDisplay();
		UpdateButton();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTeachBadTabDetectorDlg::OnJog() 
{
	// TODO: Add your control notification handler code here
}

void CTeachBadTabDetectorDlg::OnZ1() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0].GetWindowText(cBeforeVal, 30);
	
	/** ��ǥ��ġ �Է� �ޱ� */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** ��ǥ��ġ �����ϱ� */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), dNewTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	m_rgdOffsetPosData[m_iSelectedPos] = dNewTargetPos - m_rgdModelPosData[m_iSelectedPos];

	swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos]);
	m_sCoord[2].SetCaption(szTemp);
}

void CTeachBadTabDetectorDlg::OnCmMove() 
{	
#ifndef DEF_SETUP
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen())
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(300003000);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
		return;	
	}
#endif


//	��ǥ��ġ�� �̵��Ͻðڽ��ϱ�?
	if (MyMessageBox(MY_MESSAGE_BOX,50000,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("TabFeeder is Moved to Target Position...Wait a moment..."));
	ProgWnd.PeekAndPump();

	// �̵� �� Jog�� ����ϰ� �Ǹ� Motion Error�� ���Ƿ� Jog ��� ����
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

	int iResult = m_plnkTabFeeder->SafeMoveBadTabPos(m_iSelectedPos);		
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("TabFeeder Teaching Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	

	ProgWnd.StepIt();

	// Jog ��� �簳
	if (DEF_FRONT_GROUP == m_iGroupNo)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_BADTAB_DETECTOR_X);
	else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_BADTAB_DETECTOR_X);
}

void CTeachBadTabDetectorDlg::OnPosSet() 
{
//	��ǥ ��ġ�� ���� ��ġ�� �����Ͻðڽ��ϱ�?
	if (MyMessageBox(MY_MESSAGE_BOX,50001,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	double dCurrent = m_plnkTabFeeder->GetBadTabCurrentPos();
	double dTargetPos = dCurrent;
	WCHAR szTemp[25];	
	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	m_rgdOffsetPosData[m_iSelectedPos] = dTargetPos - m_rgdModelPosData[m_iSelectedPos];

	swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos]);
	m_sCoord[2].SetCaption(szTemp);

	// Home Flag Reset
	if (DEF_FRONT_GROUP == m_iGroupNo)
		MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetHomeFlagSprocket2(FALSE);
	else
		MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetHomeFlagSprocket2(FALSE);
}

void CTeachBadTabDetectorDlg::OnSave() 
{
//	Data�� �����Ͻðڽ��ϱ�?
	if (MyMessageBox(MY_MESSAGE_BOX,50002,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkTabFeeder->SaveBadTabOffsetPosParameter(m_rgdOffsetPosData);
}

void CTeachBadTabDetectorDlg::OnExit() 
{
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	m_plnkC_MngOpPanel->SetJogUnit();

	CDialog::OnCancel();
}

/**
 * ��ǥ��ġ, Mark Offset, ��Ÿ����, ������ġ, ���� Display (XYT��)
 */
void CTeachBadTabDetectorDlg::UpdateDisplay()
{
	WCHAR szTemp[250];
	double dCurrentPos;	
	double dTargetPos;
	int i=0;
	
	dTargetPos = m_rgdModelPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];
	
	/** ���� ��ġ �а�, Display �ϱ� */
	dCurrentPos = m_plnkTabFeeder->GetBadTabCurrentPos();
	
	// Reset Switch ������ ���� ��ġ�� 0���� ǥ�õǰ� �Ͽ� 
	// Teaching�� ���Ÿ� ������ ���� �Ѵ�.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
	{
		m_dDisplayOffsetZ = dCurrentPos;
	}
	
	dCurrentPos = dCurrentPos - m_dDisplayOffsetZ;	
	
	swprintf(szTemp, _T("%.3f"), dCurrentPos);
	m_sCoord[3].SetCaption(szTemp);
	
	/** ���� ����ϱ� */
	swprintf(szTemp, _T("%.3f"), dCurrentPos - dTargetPos);
	m_sCoord[4].SetCaption(szTemp);
}
/**
 * ������ǥ, Offset��ǥ Display
 */
void CTeachBadTabDetectorDlg::UpdateCoordinate()
{
	WCHAR szTemp[250];
	double dTargetPos;

	/** ��ǥ ��ġ */
	dTargetPos = m_rgdModelPosData[m_iSelectedPos] + m_rgdOffsetPosData[m_iSelectedPos];
	swprintf(szTemp, _T("%.3f"), dTargetPos);	
	m_sCoord[0].SetCaption(szTemp);

	/** ����ǥ ȭ�� Display */
	swprintf(szTemp, _T("%.3f"), m_rgdModelPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
	/** Offset��ǥ ȭ�� Display */
	swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos]);
	m_sCoord[2].SetCaption(szTemp);
}
/**
 * ���� Position ���� 
 */
void CTeachBadTabDetectorDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}
/**
 * Command Button ���� Update
 */
void CTeachBadTabDetectorDlg::UpdateButton()
{
	BOOL bStatus = FALSE;
	BOOL bOldStatus = FALSE;

	CBtnEnh* pbtn = NULL;

	pbtn = (CBtnEnh*)(GetDlgItem(IDB_REEL_CLAMP_UP));
	bStatus = m_plnkTabFeeder->IsClutchOnSprocket1Axis();
	bOldStatus = pbtn->GetValue();
	if ((bStatus == TRUE) && (bOldStatus == FALSE))
		pbtn->SetValue(TRUE);
	else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		pbtn->SetValue(FALSE);

	pbtn = (CBtnEnh*)(GetDlgItem(IDB_REEL_CLAMP_DOWN));
	bStatus = !m_plnkTabFeeder->IsClutchOnSprocket1Axis();
	bOldStatus = pbtn->GetValue();
	if ((bStatus == TRUE) && (bOldStatus == FALSE))
		pbtn->SetValue(TRUE);
	else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		pbtn->SetValue(FALSE);

}

void CTeachBadTabDetectorDlg::OnBadtabPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);
}
void CTeachBadTabDetectorDlg::OnBadtabPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}

void CTeachBadTabDetectorDlg::OnClickCmSprocket1Homing() 
{
	// TODO: Add your control notification handler code here

	// Sprocket1�� Offset�� ����Ѵ�. �� ���� Sprocket1�� �������� Offset�� ����..
	int iResult;
	CString strMsg = "";

	if (m_plnkTabFeeder->CheckBadTabOrigin()
		|| m_plnkTabFeeder->CheckPressOrigin()
		|| m_plnkTabFeeder->CheckSprocket1Origin()
		|| m_plnkTabFeeder->CheckSprocket2Origin())
	{
//		Press, Badtab Detector, Sprocket�� ��� �������͵Ǿ�� �մϴ�
		MyMessageBox(MY_MESSAGE_BOX,50003,_T("confirm"), M_OK);
		return;
	}

//	Sprocket1�� Homing ������ �Ͻðڽ��ϱ�?
	if (MyMessageBox(MY_MESSAGE_BOX,50004,_T("Home"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Sprocket1 Homing..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Sprocket1 Homing..Wait a moment..."));
	ProgWnd.PeekAndPump();

	if (!m_plnkCtrlTabFeeder->GetHomeFlagSprocket1())
	{
		iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		m_plnkCtrlTabFeeder->SetRunTension(TRUE);
		iResult = m_plnkTabFeeder->HomingSprocket(DEF_SPROCKET1_UNIT);
		if (iResult)
		{
			m_plnkCtrlTabFeeder->SetRunTension(FALSE);
// jdy sesl			MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		//m_plnkCtrlTabFeeder->SetRunTension(FALSE);
/*		if (DEF_FRONT_GROUP == m_iGroupNo)
		{
			iResult = m_plnkTabFeeder->RMoveSprocket1Pos(m_plnkPreBonderData->m_dSprocket1_F_AutoHoming_Offset);
			if (iResult)
			{
				
				MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()), "AutoHoming Error", M_ICONERROR);
				return;
			}
		}
		else
		{
			iResult = m_plnkTabFeeder->RMoveSprocket1Pos(m_plnkPreBonderData->m_dSprocket1_R_AutoHoming_Offset);
			if (iResult)
			{
				
				MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()), "AutoHoming Error", M_ICONERROR);
				return;
			}
		}
*/
		if (DEF_FRONT_GROUP == m_iGroupNo)
		{
			//120329.kms______
//			iResult = m_plnkTabFeeder->RMoveSprocket1Pos(m_plnkSystemData->m_dSprocket1_F_AutoHoming_Offset);
			iResult = m_plnkTabFeeder->RMoveSprocket1Pos(m_plnkPreBonderData->m_dSprocket1_F_AutoHoming_Offset);
			//________________
			if (iResult)
			{
// jdy sesl				MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}
		else
		{
			//120329.kms________
//			iResult = m_plnkTabFeeder->RMoveSprocket1Pos(m_plnkSystemData->m_dSprocket1_R_AutoHoming_Offset);
			iResult = m_plnkTabFeeder->RMoveSprocket1Pos(m_plnkPreBonderData->m_dSprocket1_R_AutoHoming_Offset);
			//__________________
			if (iResult)
			{
// jdy sesl				MyMessageBox(iResult, _T("AutoHoming Error"), M_ICONERROR);
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}
	}
	ProgWnd.StepIt();

	m_plnkCtrlTabFeeder->SetRunTension(FALSE);

	if (DEF_FRONT_GROUP == m_iGroupNo)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABFEEDER_SPROCKET1_T);
	else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABFEEDER_SPROCKET1_T);

	m_plnkCtrlTabFeeder->SetHomeFlagSprocket1(FALSE);

	double dOldTPos = m_plnkTabFeeder->GetSprocket1CurrentPos();

//	Sprocket1(T) ����Ű�� �̿��ؼ� Hole�� ���� �� Ȯ���� �����ּ���.
	MyMessageBox(MY_MESSAGE_BOX,50005,_T("confirm"), M_OK);

	double dNewTPos = m_plnkTabFeeder->GetSprocket1CurrentPos();

	double dT_Diff = dNewTPos - dOldTPos;	// ���� : degree

	// hongju ����..20080825
//	if (DEF_FRONT_GROUP == m_iGroupNo)
//		m_plnkPreBonderData->m_dSprocket1_F_AutoHoming_Offset += dT_Diff;
//	else
//		m_plnkPreBonderData->m_dSprocket1_R_AutoHoming_Offset += dT_Diff;
	//120329.kms______________
	/*
	if (DEF_FRONT_GROUP == m_iGroupNo)
		m_plnkSystemData->m_dSprocket1_F_AutoHoming_Offset += dT_Diff;
	else
		m_plnkSystemData->m_dSprocket1_R_AutoHoming_Offset += dT_Diff;

	m_plnkSystemData->WriteData();
	*/

	if (DEF_FRONT_GROUP == m_iGroupNo)
		m_plnkPreBonderData->m_dSprocket1_F_AutoHoming_Offset += dT_Diff;
	else
		m_plnkPreBonderData->m_dSprocket1_R_AutoHoming_Offset += dT_Diff;

	m_plnkPreBonderData->WriteData();

	//________________________
	
	
	// sesl_jdy m_plnkPreBonderData->WriteData();

	m_plnkCtrlTabFeeder->SetHomeFlagSprocket1(TRUE);

	// Jog ��� �簳
	if (DEF_FRONT_GROUP == m_iGroupNo)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_BADTAB_DETECTOR_X);
	else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_BADTAB_DETECTOR_X);

//	Sprocket1 Homing�� �Ϸ��Ͽ����ϴ�.
	MyMessageBox(MY_MESSAGE_BOX,50006,_T("confirm"), M_OK);
}

void CTeachBadTabDetectorDlg::OnClickCmSprocket2Homing() 
{
	int iResult;
	CString strMsg = "";

	if (m_plnkTabFeeder->CheckBadTabOrigin()
		|| m_plnkTabFeeder->CheckPressOrigin()
		|| m_plnkTabFeeder->CheckSprocket1Origin()
		|| m_plnkTabFeeder->CheckSprocket2Origin()
		|| m_plnkTabFeeder->CheckMoldPinOrigin()
		)
	{
//		Press, Badtab Detector, Sprocket�� ��� �������͵Ǿ�� �մϴ�.
		MyMessageBox(MY_MESSAGE_BOX,50028,_T("confirm"), M_OK);
		return;
	}

//	Sprocket2�� Homing ������ �Ͻðڽ��ϱ�?
	if (MyMessageBox(MY_MESSAGE_BOX,50008,_T("Home"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Sprocket2 Homing..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Sprocket1 Homing..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	if (!m_plnkCtrlTabFeeder->GetHomeFlagSprocket2())
	{
		m_plnkCtrlTabFeeder->SetFindTabICFlag(FALSE);

		//120328.kms____________
		/*
		if (DEF_FRONT_GROUP == m_iGroupNo)
		{
			m_plnkSystemData->m_dSprocket1_F_AutoHoming_Offset = 0.0;
			m_plnkSystemData->m_dSprocket2_F_AutoHoming_Offset = 0.0;
		}
		else
		{
			m_plnkSystemData->m_dSprocket1_R_AutoHoming_Offset = 0.0;
			m_plnkSystemData->m_dSprocket2_R_AutoHoming_Offset = 0.0;
		}
		*/
		if (DEF_FRONT_GROUP == m_iGroupNo)
		{
			m_plnkPreBonderData->m_dSprocket1_F_AutoHoming_Offset = 0.0;
			m_plnkPreBonderData->m_dSprocket2_F_AutoHoming_Offset = 0.0;
		}
		else
		{
			m_plnkPreBonderData->m_dSprocket1_R_AutoHoming_Offset = 0.0;
			m_plnkPreBonderData->m_dSprocket2_R_AutoHoming_Offset = 0.0;
		}
		//______________________

		iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}

		m_plnkCtrlTabFeeder->SetRunTension(TRUE);
		iResult = m_plnkTabFeeder->HomingSprocket(DEF_SPROCKET2_UNIT);
		if (iResult)
		{
			m_plnkCtrlTabFeeder->SetRunTension(FALSE);
// jdy sesl			MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
		m_plnkCtrlTabFeeder->SetRunTension(FALSE);
		//hongju ����...20080825
//		double dHomeOffset = (DEF_FRONT_GROUP == m_iGroupNo)
//			? m_plnkPreBonderData->m_dSprocket2_F_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset
//			: m_plnkPreBonderData->m_dSprocket2_R_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset;
		//120329.kms__________
		/*
		double dHomeOffset = (DEF_FRONT_GROUP == m_iGroupNo)
			? m_plnkSystemData->m_dSprocket2_F_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset
			: m_plnkSystemData->m_dSprocket2_R_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset;
		*/
		double dHomeOffset = (DEF_FRONT_GROUP == m_iGroupNo)
			? m_plnkPreBonderData->m_dSprocket2_F_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset
			: m_plnkPreBonderData->m_dSprocket2_R_AutoHoming_Offset + m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset;
		
		iResult = m_plnkTabFeeder->RMoveSprocket2Pos(dHomeOffset);
		if (iResult)
		{
// jdy sesl			MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
	ProgWnd.StepIt();

	if (m_iGroupNo == DEF_TABFEEDER1)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABFEEDER_SPROCKET2_T);
	else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABFEEDER_SPROCKET2_T);

	iResult = m_plnkCtrlTabFeeder->FindWeightDownSensor();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_FIX_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	double dOldTPos	= m_plnkTabFeeder->GetSprocket2CurrentPos();

//	Sprocket2(T) ����Ű�� �̿��ؼ� Hole�� ���� �� Ȯ���� �����ּ���.
	MyMessageBox(MY_MESSAGE_BOX,50009,_T("confirm"), M_OK);
	
	double dNewTPos = m_plnkTabFeeder->GetSprocket2CurrentPos();

	double dT_Diff = dNewTPos - dOldTPos;	// ���� : degree

//@	int iSprocketPitchCount = (DEF_FRONT_GROUP == m_iGroupNo)
//@		? (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab
//@		: (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#ifdef DEF_SOURCE_SYSTEM
	int iSprocketPitchCount = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	int iSprocketPitchCount = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif

	double dX_Diff = DEF_SPROCKET_ONE_PITCH_DIST * iSprocketPitchCount / 360.0 * dT_Diff;

	if (DEF_FRONT_GROUP == m_iGroupNo)
	{
		// hongju ����..
		// ���� : Homing ���� Offset �� ������ degree �̴�.
//		m_plnkPreBonderData->m_dSprocket2_F_AutoHoming_Offset
//			+= m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset + dT_Diff;
		//120329.kms_________
		/*
		m_plnkSystemData->m_dSprocket2_F_AutoHoming_Offset
			+= m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset + dT_Diff;
		*/
		m_plnkPreBonderData->m_dSprocket2_F_AutoHoming_Offset
			+= m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset + dT_Diff;
		//___________________

		m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset = 0.0;
	}
	else
	{
//		m_plnkPreBonderData->m_dSprocket2_R_AutoHoming_Offset
//			+= m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset + dT_Diff;
		//120329.kms_________
		/*
		m_plnkSystemData->m_dSprocket2_R_AutoHoming_Offset
			+= m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset + dT_Diff;
		*/
		m_plnkPreBonderData->m_dSprocket2_R_AutoHoming_Offset
			+= m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset + dT_Diff;
		//___________________

		m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset = 0.0;
	}
	
//	m_plnkSystemData->WriteData();
	m_plnkPreBonderData->WriteData();

	m_plnkCtrlTabFeeder->SetHomeFlagSprocket2(TRUE);

	m_plnkC_MngOpPanel->SetJogUnit();

	// TabIC�� Press ��ġ�� ����� ������ Pitch �̵� ��Ŵ.
	while (TRUE)
	{
//		TabIC�� Press ��ġ�� �°� �ֽ��ϱ�?
		if (MyMessageBox(MY_MESSAGE_BOX,50010,_T("Home"), M_ICONQUESTION|M_YESNO) == IDNO)
		{
			iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
			if (iResult)
			{
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
			m_plnkCtrlTabFeeder->FeedTabIC(1);

			iResult = m_plnkCtrlTabFeeder->FindWeightDownSensor();
			if (iResult)
			{
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}

			iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_FIX_POS);
			if (iResult)
			{
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
				return;
			}
		}
		else
			break;
	}

	iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	// selsk_jdy : model ��ǥ�� �� ���� ���� �����Ƿ� ���߿� ���� Offset ��ǥ�� �ݿ�.
	// Sprocket2 �̵��� ���� ��ġ ��ȭ �ݿ�.
	double dTabDetectorWorkPos = m_plnkTabFeeder->GetBadTabTargetPos(DEF_BADTABDETECTOR_WORK_POS);
	double dTabDetectorTargetPos = dTabDetectorWorkPos + dX_Diff;
	iResult = m_plnkTabFeeder->MoveBadTabPos(dTabDetectorTargetPos, DEF_BADTABDETECTOR_WORK_POS);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	// Jog ��� �簳
	if (DEF_FRONT_GROUP == m_iGroupNo)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_BADTAB_DETECTOR_X);
	else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_BADTAB_DETECTOR_X);

	// BadTabIC�� Jog�� �̵��Ͽ� TabIC�� ã��.
//	BadTabDetector(X) ����Ű�� �̿��ؼ� TabIC�� ���� �� Ȯ���� �����ּ���.
	MyMessageBox(MY_MESSAGE_BOX,50011,_T("confirm"), M_OK);

	double dTargetPos = m_plnkTabFeeder->GetBadTabCurrentPos();
	m_rgdOffsetPosData[DEF_BADTABDETECTOR_WORK_POS] = dTargetPos - m_rgdModelPosData[DEF_BADTABDETECTOR_WORK_POS];
	m_plnkTabFeeder->SaveBadTabOffsetPosParameter(m_rgdOffsetPosData);

	UpdateCoordinate();
	getUserOffsetX_withDisplay();

	// ���� TabIC ã�� ���� ����.
	m_plnkCtrlTabFeeder->SetFindTabICFlag(FALSE);

	iResult = m_plnkCtrlTabFeeder->ExecuteAutoHoming();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("Tab Feeder"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Sprocket2 Homing�� �Ϸ��Ͽ����ϴ�.
	MyMessageBox(MY_MESSAGE_BOX,50012,_T("confirm"), M_OK);
}

void CTeachBadTabDetectorDlg::OnCheckCuttingLength() 
{
	/*/
	int iCarrierVacNo;
	int iTabMounterNo;
	int iCameraNo;
	EWorkingSide eWorkSide = MOlbSystemPre.GetProcessData()->GetCurrentWorkingSide(m_iGroupNo);
	CString strMsg = "";

	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		iCarrierVacNo = DEF_TABMOUNTER8;
		iTabMounterNo = DEF_TABMOUNTER8;
		iCameraNo = DEF_F_TABALIGNER_CAMERA_2;
	}
	else // if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		if (DEF_FRONT_GROUP == m_iGroupNo)
		{
			iCarrierVacNo = DEF_TABMOUNTER1;
			iTabMounterNo = DEF_TABMOUNTER1;
			iCameraNo = DEF_F_TABALIGNER_CAMERA_1;
		}
		else // if (DEF_REAR_GROUP == m_iGroupNo)
		{
			iCarrierVacNo = DEF_TABMOUNTER7 - m_iGroupNo * DEF_MAX_WORKER_PER_GROUP;
			iTabMounterNo = DEF_TABMOUNTER7;
			iCameraNo = DEF_R_TABALIGNER_CAMERA_1;
		}
	}
	int iTabNo = MOlbSystemPre.GetProcessData()->GetMounterDefaultTabNo(eWorkSide, iTabMounterNo);

	CMyProgressWnd ProgWnd(NULL, _T("Check TabIC Cutting Length..."));
	ProgWnd.SetRange(0, 6);
	ProgWnd.SetText(_T("Check TabIC Cutting Length..Wait a moment..."));
	ProgWnd.PeekAndPump();

	int iResult = MOlbSystemPre.RemoveTabIC(m_iGroupNo, TRUE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		if (USER_STOP == iResult)
		{
//			����� ��û�� ���� �۾��� �ߴ���.
			MyMessageBox(MY_MESSAGE_BOX,50013,_T("Information"), M_ICONINFORMATION);
			return;
		}

// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.LoadTabCarrierTabIC(m_iGroupNo, iCarrierVacNo);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		if (USER_STOP == iResult)
		{
//			����� ��û�� ���� �۾��� �ߴ���.
			MyMessageBox(MY_MESSAGE_BOX,50014,_T("Information"), M_ICONINFORMATION);
			return;
		}

// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.LoadTabMounterTabIC(m_iGroupNo, iTabMounterNo);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		if (USER_STOP == iResult)
		{
//			����� ��û�� ���� �۾��� �ߴ���.
			MyMessageBox(MY_MESSAGE_BOX,50015,_T("Information"), M_ICONINFORMATION);
			return;
		}

// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = MOlbSystemPre.PrealignTabIC(m_iGroupNo, iTabNo, TRUE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		
// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

//	iResult = MOlbSystemPre.GetTabMounterComponent(iTabMounterNo)->DownReady();
	iResult = MOlbSystemPre.GetTabMounterComponent(iTabMounterNo)->SafeMoveZPos(DEF_TABMOUNTER_Z_INSPECTION_POS, TRUE, FALSE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		
// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
	CVisionMonitorDlg dlg(plnkVision, DEF_PM_MARK, iCameraNo);
	dlg.DoModal();

	iResult = MOlbSystemPre.RemoveTabIC(m_iGroupNo, TRUE);
	if (iResult)
	{
		ProgWnd.DestroyWindow();

		if (USER_STOP == iResult)
		{
//			����� ��û�� ���� �۾��� �ߴ���.
			MyMessageBox(MY_MESSAGE_BOX,50016,_T("Information"), M_ICONINFORMATION);
			return;
		}

// jdy sesl		MyMessageBox(iResult,_T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//2009.09.18 CYJ
	iResult = m_plnkCtrlTabFeeder->FeedTabIC();
	if (iResult)
	{
		ProgWnd.DestroyWindow();
		
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	ProgWnd.DestroyWindow();

//	Ȯ�� �۾� �Ϸ�
	MyMessageBox(MY_MESSAGE_BOX,50017,_T("Information"), M_ICONINFORMATION);
	/*/
	CString strMsg = "";
	int iResult = SUCCESS;
	if (MyMessageBox(_T("Do you want to check the length of the punch?\r\nŸ�� ���� Ȯ�� �۾��� �ϰڽ��ϱ�?"), _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	//@130202.���� �߰� �ϱ�
}

void CTeachBadTabDetectorDlg::OnClickTabCuttingOffset() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR szTemp[250];

//	��� Ÿ���Ͻ÷��� (+), ª�� Ÿ���Ͻ÷��� (-)���� �־��ּ���.
	MyMessageBox(MY_MESSAGE_BOX,50018,_T("confirm"), M_OK);

	/** ��ǥ��ġ �Է� �ޱ� */
	double dOldXOffset = getUserOffsetX_withDisplay();

	swprintf(szTemp, _T("%.3f"), -dOldXOffset);
	if (!GetNumberBox(cAfterVal, 10, szTemp))
		return;

	/** ��ǥ��ġ �����ϱ� */
	double dNewXOffset = wcstod(cAfterVal,NULL);

	if (fabs(dNewXOffset) > 1.0)
	{
//		+1 ���� ũ�ų� -1 ���� �۽��ϴ�.
		MyMessageBox(MY_MESSAGE_BOX,50019,_T("confirm"), M_OK);
		return;
	}

//	Offset���� ���� �����Ͻðڽ��ϱ�?
	if (MyMessageBox(MY_MESSAGE_BOX,50020,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("The cutting offset application..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Wait for a moment..."));
	ProgWnd.PeekAndPump();

//@	int iSprocketPitchCount = (DEF_FRONT_GROUP == m_iGroupNo)
//@		? (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab
//@		: (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#ifdef DEF_SOURCE_SYSTEM
	int iSprocketPitchCount = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	int iSprocketPitchCount = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif

	double dNewTOffset = 360.0 / DEF_SPROCKET_ONE_PITCH_DIST / iSprocketPitchCount * dNewXOffset;

	double dOldTOffset;
	if (DEF_FRONT_GROUP == m_iGroupNo)
	{
		dOldTOffset = m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset;
		m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset = -dNewTOffset;
	}
	else
	{
		dOldTOffset = m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset;
		m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset = -dNewTOffset;
	}
	
	m_plnkPreBonderData->WriteData();

	getUserOffsetX_withDisplay();

	double dT_Diff = -dNewTOffset - dOldTOffset;	// ���� : degree
	double dX_Diff = -dNewXOffset - dOldXOffset;

	m_rgdOffsetPosData[DEF_BADTABDETECTOR_WORK_POS] += dX_Diff;
	m_plnkTabFeeder->SaveBadTabOffsetPosParameter(m_rgdOffsetPosData);
	UpdateCoordinate();

	int iResult = m_plnkTabFeeder->RMoveSprocket2Pos(dT_Diff);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkTabFeeder->SafeMoveBadTabPos(DEF_BADTABDETECTOR_WORK_POS);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
	iResult = m_plnkCtrlTabFeeder->FeedTabIC(-2);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	
	Sleep(500);
	
	iResult = m_plnkCtrlTabFeeder->FeedTabIC(2);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("AutoHoming Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.DestroyWindow();

//	Offset ���� �Ϸ�.
	MyMessageBox(MY_MESSAGE_BOX,50021,_T("Information"), M_ICONINFORMATION);
}

double CTeachBadTabDetectorDlg::getUserOffsetX_withDisplay()
{
	double dUserOffset;
	int iSprocketPitchCount;

	if (DEF_FRONT_GROUP == m_iGroupNo)
	{
		dUserOffset = m_plnkPreBonderData->m_dSprocket2_F_User_AutoHoming_Offset;
//		iSprocketPitchCount = (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
	}
	else
	{
		dUserOffset = m_plnkPreBonderData->m_dSprocket2_R_User_AutoHoming_Offset;
//		iSprocketPitchCount = (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
	}

#ifdef DEF_SOURCE_SYSTEM
	iSprocketPitchCount = (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
#else
	iSprocketPitchCount = (int)m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
#endif

	double dX_Diff = DEF_SPROCKET_ONE_PITCH_DIST * iSprocketPitchCount / 360.0 * dUserOffset;

	WCHAR str[100];
	swprintf(str,_T("%.3f"), -dX_Diff);
	m_sTensionOffset.SetCaption(str);

	return dX_Diff;
}

void CTeachBadTabDetectorDlg::OnClickReelUp() 
{
	int iResult;
	if (MyMessageBox(MY_MESSAGE_BOX, 50026, _T("MoldPin Up/Down Axis Up"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("MoldPin Up/Down Axis..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("MoldPin Axis Up/Down Axis Up...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_FIX_POS)) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Press Reel Up/Down Cylinder Up"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CTeachBadTabDetectorDlg::OnClickReelDown() 
{
	// TODO: Add your control notification handler code here
	int iResult;
//	Press Reel�� Down�Ͻðڽ��ϱ�?

	if (MyMessageBox(MY_MESSAGE_BOX, 50027, _T("MoldPin Up/Down Axis Down"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("MoldPin Up/Down Axis..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("MoldPin Axis Up/Down Axis Down...Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(DEF_MOLDPIN_RELEASE_POS)) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Press Reel Up/Down Cylinder Up"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CTeachBadTabDetectorDlg::OnClickReelClampUp() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox(MY_MESSAGE_BOX,30653, _T("Sprocket Clutch On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->ClutchOnSprocket1Axis();
	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	
}

void CTeachBadTabDetectorDlg::OnClickReelClampDown() 
{
	// TODO: Add your control notification handler code here
//	Reel Clamp�� Down �Ͻðڽ��ϱ�?

	if (MyMessageBox(MY_MESSAGE_BOX,30654, _T("Sprocket Clutch Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int iResult = m_plnkTabFeeder->ClutchOffSprocket1Axis();
	if (iResult != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}


void CTeachBadTabDetectorDlg::OnClickCmSprocket1Jog() 
{
	// TODO: Add your control notification handler code here
	if (DEF_FRONT_GROUP == m_iGroupNo)
	{
		if(m_iSp1JogSel == 0)
		{
			m_plnkTabFeeder->ClutchOnSprocket1Axis();
			GetDlgItem(IDB_CM_SPROCKET1_JOG)->SetWindowText(_T("Selected\nSprocket1"));
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABFEEDER_SPROCKET1_T);
			m_iSp1JogSel = 1;
		}
		else if (m_iSp1JogSel == 1)
		{
			m_plnkTabFeeder->ClutchOffSprocket1Axis();
			GetDlgItem(IDB_CM_SPROCKET1_JOG)->SetWindowText(_T("Selected\nSprocket2"));
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABFEEDER_SPROCKET2_T);
			m_iSp1JogSel = 2;
		}
		else if (m_iSp1JogSel == 2)
		{
			m_plnkTabFeeder->ClutchOffSprocket1Axis();
			GetDlgItem(IDB_CM_SPROCKET1_JOG)->SetWindowText(_T("Selected\nBadTab_X"));
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_BADTAB_DETECTOR_X);
			m_iSp1JogSel = 0;
		}		
	}
	else
	{
		if(m_iSp1JogSel == 0)
		{
			m_plnkTabFeeder->ClutchOnSprocket1Axis();
			GetDlgItem(IDB_CM_SPROCKET1_JOG)->SetWindowText(_T("Selected\nSprocket1"));
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABFEEDER_SPROCKET1_T);
			m_iSp1JogSel = 1;
		}
		else if (m_iSp1JogSel == 1)
		{
			m_plnkTabFeeder->ClutchOffSprocket1Axis();
			GetDlgItem(IDB_CM_SPROCKET1_JOG)->SetWindowText(_T("Selected\nSprocket2"));
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABFEEDER_SPROCKET2_T);
			m_iSp1JogSel = 2;
		}
		else if (m_iSp1JogSel == 2)
		{
			m_plnkTabFeeder->ClutchOffSprocket1Axis();
			GetDlgItem(IDB_CM_SPROCKET1_JOG)->SetWindowText(_T("Selected\nBadTab_X"));
			m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_BADTAB_DETECTOR_X);
			m_iSp1JogSel = 0;
		}		
	}
}

void CTeachBadTabDetectorDlg::OnClickCmSprocket2Jog() 
{
	// TODO: Add your control notification handler code here	
}
