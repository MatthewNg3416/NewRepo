// FeederTeachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "TeachModelChangeDlg.h"
#include "MPlatformOlbSystem.h"
#include "VisionMonitorDlg.h"
#include "MTrsAutoManager.h"

#include "TeachMainForm.h"
#include "IAxis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachModelChangeDlg dialog

CTeachModelChangeDlg::CTeachModelChangeDlg(int iWorkGroup, int iFixModel, CWnd* pParent /*=NULL*/)
	: CDialog(CTeachModelChangeDlg::IDD, pParent)
{ 
	//{{AFX_DATA_INIT(CTeachModelChangeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iWorkGroup = iWorkGroup;
	m_iJogSelect	= 0;
	m_iSelectedPos	= DEF_MODEL_CHANGE_INIT_POS;
	m_iFixModel		= iFixModel;

	// Dispaly Offset 값 (Reset S/W 누른후 상대 위치 값 Dispaly 위해)
	m_dDisplayOffsetZ = 0.0;

	// 전체 Position
	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
	{
		m_rgdFixedPosData[i] = 0.0;
	}

	m_plnkModelChange = MOlbSystemPre.GetModelChangeComponent(m_iWorkGroup);
	
	m_plnkModelChange->GetYPosParameter(m_rgdFixedPosData, m_rgdOffsetPosData, m_rgdModelPosData);

	m_plnkC_MngOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
}


void CTeachModelChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachModelChangeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here

	DDX_Control(pDX, IDB_AXISY, m_ctrlBtnAxis[eAxisY]);
	//DDX_Control(pDX, IDC_GRD_AXISY_SNS_STS,	m_grd_AxisSnsState[eAxisY]);
	
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_FEEDER_POS0,		m_BtnPos[0]);
	DDX_Control(pDX, IDB_FEEDER_POS1,		m_BtnPos[1]);
	DDX_Control(pDX, IDB_FEEDER_POS2,		m_BtnPos[2]);

	DDX_Control(pDX, IDC_TGT_POS_X,			m_sCoord[0]);
	DDX_Control(pDX, IDC_FIX_POS_X,			m_sCoord[1]);
	DDX_Control(pDX, IDC_MODEL_POS_X,		m_sCoord[2]);
	DDX_Control(pDX, IDC_OFFSET_POS_X,		m_sCoord[3]);
	DDX_Control(pDX, IDC_CUR_POS_X,			m_sCoord[4]);
	DDX_Control(pDX, IDC_DIF_POS_X,			m_sCoord[5]);

	DDX_Control(pDX, IDB_CM_MODEL_CHANGE_EXPAND, m_BtnCmd[0]);
	DDX_Control(pDX, IDB_CM_MODEL_CHANGE_SHRINK, m_BtnCmd[1]);
}


BEGIN_MESSAGE_MAP(CTeachModelChangeDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachModelChangeDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachModelChangeDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachModelChangeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachModelChangeDlg)
	ON_EVENT(CTeachModelChangeDlg, IDB_JOG, -600 /* Click */,			OnJog, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_AXISY, -600 /* Click */,			OnAxisY, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_FEEDER_POS0, -600 /* Click */,	OnFeederPos0, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_FEEDER_POS1, -600 /* Click */,	OnFeederPos1, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_FEEDER_POS2, -600 /* Click */,	OnFeederPos2, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_CM_MOVE, -600 /* Click */,		OnBmMove, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_POS_SET, -600 /* Click */,		OnPosSet, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_SAVE, -600 /* Click */,			OnSave, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_EXIT, -600 /* Click */,			OnExit, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_CM_MODEL_CHANGE_EXPAND, -600 /* Click */, OnClickCmModelChangeExpand, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_CM_MODEL_CHANGE_SHRINK, -600 /* Click */, OnClickCmModelChangeShrink, VTS_NONE)
	ON_EVENT(CTeachModelChangeDlg, IDB_REGISTER_MARK, -600 /* Click */, OnClickRegisterMark, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachModelChangeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTeachModelChangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i = 0, iTemp = 0; 
	int iSelected = -1;

	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE:
		{
	//		Model Change 모델좌표 Teaching
			CString szTextTemp;
			ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_13"), &szTextTemp);
			SetWindowText(szTextTemp);
			break;
		}
	
	case DEF_FIX_MODE:
		{
	//		Model Change 고정좌표 Teaching
			CString szTextTemp;
			ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_14"), &szTextTemp);
			SetWindowText(szTextTemp);
			break;
		}
	}

	m_sTeachingInfo = m_plnkModelChange->GetYTeachingInfo();

	for (int i = 0; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
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
	
	for (int i = iTemp; i < DEF_MODEL_CHANGE_MAX_POSITION; i++)
	{
		m_BtnPos[i].ShowWindow(SW_HIDE);
	}

	m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_MODEL_CHANGE_Y1 + m_iWorkGroup);

	UpdateCoordinate();

	// 기본 SelectPosition 설정 하기)
	if (m_iSelectedPos != -1)
		SelectPosition(m_iSelectedPos);

	m_uiTimerID = SetTimer(1, 200, NULL);

	GetDlgItem(IDS_COMMAND2)->ShowWindow(SW_HIDE);
	m_BtnCmd[0].EnableWindow(FALSE);
	m_BtnCmd[0].ShowWindow(SW_HIDE);
	m_BtnCmd[1].EnableWindow(FALSE);
	m_BtnCmd[1].ShowWindow(SW_HIDE);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_1"), &szTextTemp);
	//축
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_2"), &szTextTemp);
	//Y 축
	GetDlgItem(IDB_AXISY)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_3"), &szTextTemp);
//	목표 위치
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_4"), &szTextTemp);
//	고정 좌표
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_5"), &szTextTemp);
//	모델 좌표
	GetDlgItem(IDC_MODEL_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_6"), &szTextTemp);
//	Offset 좌표
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_7"), &szTextTemp);
//	현재 위치
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_8"), &szTextTemp);
//	차이값
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_9"), &szTextTemp);
	//목표 위치로\r\n이동
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_10"), &szTextTemp);
	//목표 위치를\r\n현재값으로
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_11"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_TEACH, _T("TeachModelChangeDlg_12"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	// Doolin kwangilshin 2017. 08. 24.
	//
	Init_GridAxisStste();
	// End

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachModelChangeDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL	bStatus = FALSE;
	BOOL	bOldStatus = FALSE;

	if (m_uiTimerID == nIDEvent)
	{
		UpdateDisplay();
		UpdateButton();

		// Doolin kwangilshin 2017. 08. 25.
		//
		updateAxisSnsSts();
		// End.
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTeachModelChangeDlg::OnJog() 
{
	// TODO: Add your control notification handler code here
	
}

void CTeachModelChangeDlg::OnAxisY() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];

	m_sCoord[0].GetWindowText(cBeforeVal, 30);
	
	
	// Doolin kwangilshin 2017. 08. 25.
	//
	if (!Check_AxisStste(m_plnkModelChange->GetMotionComponent()))
	{
		return;
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	swprintf(szTemp, _T("%.3f"), dNewTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		m_rgdOffsetPosData[m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[m_iSelectedPos]  - m_rgdFixedPosData[m_iSelectedPos];
	
		swprintf(szTemp, _T("%.3f"), m_rgdOffsetPosData[m_iSelectedPos]);
		m_sCoord[3].SetCaption(szTemp);
		break;

	case DEF_FIX_MODE :
		m_rgdFixedPosData[m_iSelectedPos]
			= dNewTargetPos - m_rgdModelPosData[m_iSelectedPos] - m_rgdOffsetPosData[m_iSelectedPos];

		swprintf(szTemp, _T("%.3f"), m_rgdFixedPosData[m_iSelectedPos]);
		m_sCoord[1].SetCaption(szTemp);
		break;		
	}
}

void CTeachModelChangeDlg::OnBmMove() 
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
	if (MyMessageBox(MY_MESSAGE_BOX,50400,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL,_T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Move Target Position..Wait a moment..."));
	ProgWnd.PeekAndPump();

	// 이동 중 Jog를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

	int iResult = m_plnkModelChange->SafeMoveYPos(m_iSelectedPos);
	if (iResult != DEF_TEACH_MODEL_CHANGE_DLG_SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Press Motion Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	

	ProgWnd.StepIt();

	m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_MODEL_CHANGE_Y1 + m_iWorkGroup);
}

void CTeachModelChangeDlg::OnPosSet() 
{
//	목표 위치를 현재 위치로 변경하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50401,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL,_T("Set Target Position..."));
	ProgWnd.SetRange(0, 4);
	ProgWnd.SetText(_T("Model Change Target Position is setting to Current Position..Wait a moment..."));
	ProgWnd.PeekAndPump();

	double dCurrent = m_plnkModelChange->GetYCurrentPos();

	ProgWnd.StepIt();

	double dTargetPos = dCurrent;
	double dFixedPosData = m_rgdFixedPosData[m_iSelectedPos];
	double dModelPosData = m_rgdModelPosData[m_iSelectedPos];
	double dOffsetPosData = m_rgdOffsetPosData[m_iSelectedPos];

	WCHAR szTemp[250];	
	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	ProgWnd.StepIt();

	switch (m_iFixModel)
	{
	case DEF_MODEL_MODE :
		dOffsetPosData = dTargetPos - dModelPosData - dFixedPosData;
		swprintf(szTemp, _T("%.3f"), dOffsetPosData);
		// selsk_jdy m_sCoord[2].SetCaption(szTemp);
		m_sCoord[3].SetCaption(szTemp);
		m_rgdOffsetPosData[m_iSelectedPos] = dOffsetPosData;
		break;
	
	case DEF_FIX_MODE :
		dFixedPosData = dTargetPos - dModelPosData - dOffsetPosData;
		swprintf(szTemp, _T("%.3f"), dFixedPosData);
		m_sCoord[1].SetCaption(szTemp);
		m_rgdFixedPosData[m_iSelectedPos] = dFixedPosData;
		break;
	}

	ProgWnd.StepIt();
}

void CTeachModelChangeDlg::OnSave() 
{
//	Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50402,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	if (m_iFixModel == DEF_FIX_MODE)
	{
		m_plnkModelChange->SaveYFixedPosParameter(m_rgdFixedPosData);
	}
	else
	{
		m_plnkModelChange->SaveYOffsetPosParameter(m_rgdOffsetPosData);
	}

}

void CTeachModelChangeDlg::OnExit() 
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
void CTeachModelChangeDlg::UpdateDisplay()
{
	WCHAR szTemp[250];
	double dPosCurr = 0.0;	
	double dFixedPosData = m_rgdFixedPosData[m_iSelectedPos];
	double dModelPosData = m_rgdModelPosData[m_iSelectedPos];
	double dOffsetPosData = m_rgdOffsetPosData[m_iSelectedPos];

	double dTargetPos = dFixedPosData + dModelPosData + dOffsetPosData;

	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0].SetCaption(szTemp);	

	/** 현재 위치 읽고, Display 하기 */
	dPosCurr = m_plnkModelChange->GetYCurrentPos();
	
	// Reset Switch 감지 시 현재 위치를 0으로 표시되게 하여 
	// Teaching시 상대거리 측정을 쉽게 한다.
	if (m_plnkC_MngOpPanel->GetResetSWStatus())
		m_dDisplayOffsetZ = dPosCurr;

	dPosCurr -= m_dDisplayOffsetZ;

	/** 현재 위치 Display */
	swprintf(szTemp, _T("%.3f"), dPosCurr);
	m_sCoord[4].SetCaption(szTemp);
	
	/** 편차 계산하고, Display */
	swprintf(szTemp, _T("%.3f"), dPosCurr - dTargetPos);
	m_sCoord[5].SetCaption(szTemp);	
}

/**
 * 고정좌표, Offset좌표 Display
 */
void CTeachModelChangeDlg::UpdateCoordinate()
{
	WCHAR szBuff[250];

	double dFixedPosData = m_rgdFixedPosData[m_iSelectedPos];
	double dModelPosData = m_rgdModelPosData[m_iSelectedPos];
	double dOffsetPosData = m_rgdOffsetPosData[m_iSelectedPos];

	memset(szBuff, 0x00, sizeof(szBuff));
	
	swprintf(szBuff, _T("%.3f"), dFixedPosData);
	m_sCoord[1].SetWindowText(szBuff);

	swprintf(szBuff, _T("%.3f"), dModelPosData);
	m_sCoord[2].SetWindowText(szBuff);

	swprintf(szBuff, _T("%.3f"), dOffsetPosData);
	m_sCoord[3].SetWindowText(szBuff);

	UpdateDisplay();
}

/**
 * 선택 Position 변경 
 */
void CTeachModelChangeDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}

/**
 * Command Button 상태 Update
 */
void CTeachModelChangeDlg::UpdateButton()
{
}

void CTeachModelChangeDlg::OnFeederPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);
}

void CTeachModelChangeDlg::OnFeederPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}

void CTeachModelChangeDlg::OnFeederPos2() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(2);
}

void CTeachModelChangeDlg::OnClickCmModelChangeExpand() 
{
	int iResult = SUCCESS;
//	Model Change를 Expand하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50403,_T("ModelChange Cylinder Operation"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	CMyProgressWnd ProgWnd(NULL,_T("ModelChange Cylinder Operation..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("ModelChange Expand/Shrink Cylinder Expand..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	iResult = m_plnkModelChange->Expand();
	if(iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("ModelChange Expand/Shrink Cylinder Expand"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

}

void CTeachModelChangeDlg::OnClickCmModelChangeShrink() 
{
	int iResult = SUCCESS;
//	Model Change를 Shrink하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,50405,_T("ModelChange Cylinder Operation"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	
	CMyProgressWnd ProgWnd(NULL,_T("ModelChange Cylinder Operation..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("ModelChange Expand/Shrink Cylinder Shrink..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	iResult = m_plnkModelChange->Shrink();
	if(iResult)
	{
// jdy sesl		MyMessageBox(iResult,_T("ModelChange Cylinder Operation"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

}

void CTeachModelChangeDlg::OnClickRegisterMark() 
{
	// TODO: Add your control notification handler code here
	IVision* plnkVision = MOlbSystemPre.GetVisionComponent();
	int iCamNo = DEF_PANEL_ALIGN_CAMERA_1;
	// Operation Type = DEF_PM_MARK / DEF_BLOB_MARK / DEF_CALIPER_MARK
	CVisionMonitorDlg dlg(plnkVision, DEF_PM_MARK, iCamNo);
	dlg.DoModal();	
}

// Doolin kwangilshin 2017. 08. 24.
//
void CTeachModelChangeDlg::Init_GridAxisStste()
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


BOOL CTeachModelChangeDlg::Check_AxisStste(IAxis *pclsAxis)
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


void CTeachModelChangeDlg::SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome)
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


void CTeachModelChangeDlg::SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachModelChangeDlg::SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachModelChangeDlg::SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable)
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


void CTeachModelChangeDlg::updateAxisSnsSts()
{
	IAxis *pclsAxis[eAxisEnd];
	
	pclsAxis[eAxisY] = m_plnkModelChange->GetMotionComponent();

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

