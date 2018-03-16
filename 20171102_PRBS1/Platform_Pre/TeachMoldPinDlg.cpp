// TeachMoldPinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "TeachMoldPinDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachMoldPinDlg dialog
CTeachMoldPinDlg::CTeachMoldPinDlg(int iFixModel, int iGroupNo, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachMoldPinDlg::IDD, pParent)
{ 
	//{{AFX_DATA_INIT(CTeachMoldPinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iSelUnit = iGroupNo;
	m_iJogSelect = 0;
	m_iSelectedPos = DEF_MOLDPIN_RELEASE_POS;

	// Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	m_dDisplayOffsetZ = 0.0;

	// 전체 Position
	for (int i = 0; i < DEF_MOLDPIN_MAX_POSITION; i++)
	{
		m_rgdFixedPosData[i] = 0.0;
	}

	m_plnkTabFeeder = MOlbSystemPre.GetTabFeederComponent(m_iSelUnit);
	
	m_plnkTabFeeder->GetMoldPinPosParameter(m_rgdFixedPosData, m_rgdOffsetPosData);

	m_plnkC_MngOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
}


void CTeachMoldPinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachMoldPinDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDS_GROUP_TITLE,	m_lblGroup);
	DDX_Control(pDX, IDB_FEEDER_POS0,	m_BtnPos[0]);
	DDX_Control(pDX, IDB_FEEDER_POS1,	m_BtnPos[1]);

	DDX_Control(pDX, IDC_TGT_POS_X,		m_sCoord[0]);
	DDX_Control(pDX, IDC_FIX_POS_X,		m_sCoord[1]);
	DDX_Control(pDX, IDC_OFFSET_POS_X,	m_sCoord[2]);
	DDX_Control(pDX, IDC_CUR_POS_X,		m_sCoord[3]);
	DDX_Control(pDX, IDC_DIF_POS_X,		m_sCoord[4]);
}


BEGIN_MESSAGE_MAP(CTeachMoldPinDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachMoldPinDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachMoldPinDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachMoldPinDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachMoldPinDlg)
	ON_EVENT(CTeachMoldPinDlg, IDB_JOG, -600 /* Click */, OnJog, VTS_NONE)
	ON_EVENT(CTeachMoldPinDlg, IDB_X, -600 /* Click */, OnX, VTS_NONE)
	ON_EVENT(CTeachMoldPinDlg, IDB_FEEDER_POS0, -600 /* Click */, OnFeederPos0, VTS_NONE)
	ON_EVENT(CTeachMoldPinDlg, IDB_FEEDER_POS1, -600 /* Click */, OnFeederPos1, VTS_NONE)
	ON_EVENT(CTeachMoldPinDlg, IDB_CM_MOVE, -600 /* Click */, OnBmMove, VTS_NONE)
	ON_EVENT(CTeachMoldPinDlg, IDB_POS_SET, -600 /* Click */, OnPosSet, VTS_NONE)
	ON_EVENT(CTeachMoldPinDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CTeachMoldPinDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachMoldPinDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTeachMoldPinDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i = 0, iTemp = 0; 
	int iSelected = -1;

	if (m_iSelUnit == DEF_FRONT_GROUP)
	{
		CString szTextTemp;
		ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_1"), &szTextTemp);
		SetWindowText(szTextTemp);

		m_lblGroup.SetCaption(_T("Front Group"));
	}
	else	// if (m_iSelUnit == DEF_REAR_GROUP)
	{
//		 Tab Press2 Z 고정좌표 Teaching
		CString szTextTemp;
		ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_2"), &szTextTemp);
		SetWindowText(szTextTemp);

		m_lblGroup.SetCaption(_T("Rear Group"));
	}
	
	/** Teaching Position Name Reading */
	m_sTeachingInfo = m_plnkTabFeeder->GetMoldPinTeachingInfo();

	for (i = 0; i < DEF_MOLDPIN_MAX_POSITION; i++)
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
	
	for (i = iTemp; i < DEF_MOLDPIN_MAX_POSITION; i++)
	{
		m_BtnPos[i].ShowWindow(SW_HIDE);
	}

	if (DEF_TABFEEDER1 == m_iSelUnit)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_MOLDING_UPDOWN_Z);
	else // if (m_iSelUnit == DEF_TABFEEDER2)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_MOLDING_UPDOWN_Z);

	UpdateCoordinate();

	// 기본 SelectPosition 설정 하기)
	if (m_iSelectedPos != -1)
		SelectPosition(m_iSelectedPos);

	m_uiTimerID = SetTimer(1, 200, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_3"), &szTextTemp);
	//축
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_4"), &szTextTemp);
	//Z 축
	GetDlgItem(IDB_X)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_5"), &szTextTemp);
	//목표 위치
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_6"), &szTextTemp);
	//고정 좌표
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_7"), &szTextTemp);
	//Offset 좌표
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_8"), &szTextTemp);
	//현재 위치
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_9"), &szTextTemp);
	//차이값
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_10"), &szTextTemp);
	//목표 위치로\r\n이동
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_11"), &szTextTemp);
	//목표 위치를\r\n현재값으로
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_12"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachMoldPinDlg_13"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachMoldPinDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		UpdateDisplay();
		UpdateButton();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTeachMoldPinDlg::OnJog() 
{
	// TODO: Add your control notification handler code here
	
}

void CTeachMoldPinDlg::OnX() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0].GetWindowText(cBeforeVal, 30);
	
	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), dNewTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	m_rgdFixedPosData[m_iSelectedPos] = dNewTargetPos;
				
	swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
}

void CTeachMoldPinDlg::OnBmMove() 
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

	int iResult = m_plnkTabFeeder->SafeMoveMoldPinPos(m_iSelectedPos);
	if (iResult != DEF_TEACH_MOLDPIN_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	

	ProgWnd.StepIt();

	// Jog 사용 재개
	if (DEF_TABFEEDER1 == m_iSelUnit)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_MOLDING_UPDOWN_Z);
	else // if (m_iSelUnit == DEF_TABFEEDER2)
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_MOLDING_UPDOWN_Z);
}

void CTeachMoldPinDlg::OnPosSet() 
{
//	 목표 위치를 현재 위치로 변경하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50701,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	double dCurrent = m_plnkTabFeeder->GetMoldPinCurrentPos();
	double dTargetPos = dCurrent;
	
	WCHAR szTemp[250];	
	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	m_rgdFixedPosData[m_iSelectedPos] =	dTargetPos;

	swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
}

void CTeachMoldPinDlg::OnSave() 
{
//	 Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50702,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	m_plnkTabFeeder->SaveMoldPinFixedPosParameter(m_rgdFixedPosData);
}

void CTeachMoldPinDlg::OnExit() 
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
void CTeachMoldPinDlg::UpdateDisplay()
{
	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos];
	
	/** 현재 위치 읽고, Display 하기 */
	double dCurrentPos = m_plnkTabFeeder->GetMoldPinCurrentPos();
	
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
void CTeachMoldPinDlg::UpdateCoordinate()
{
	/** 목표 위치 */
	double dTargetPos = m_rgdFixedPosData[m_iSelectedPos];

	WCHAR szTemp[250];
	swprintf(szTemp, _T("%.3f"), dTargetPos);	
	m_sCoord[0].SetCaption(szTemp);

	/** 고정좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
}

/**
 * 선택 Position 변경 
 */
void CTeachMoldPinDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}

/**
 * Command Button 상태 Update
 */
void CTeachMoldPinDlg::UpdateButton()
{
}

void CTeachMoldPinDlg::OnFeederPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);
}

void CTeachMoldPinDlg::OnFeederPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}
