// TeachTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "TeachTransferDlg.h"
#include "MPlatformOlbSystem.h"
#include <math.h>
#include "MManageOpPanel.h"
#include "MTrsAutoManager.h"
#include "MPanelAligner.h" //170316 SJ_YSH

#include "TeachMainForm.h"
#include "IAxis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeachTransferDlg dialog

extern	MPlatformOlbSystem	MOlbSystemPre;

CTeachTransferDlg::CTeachTransferDlg(int iFixModel, int iTransferID, CWnd* pParent /*=NULL*/)
: CDialog(CTeachTransferDlg::IDD, pParent), m_iFixModel(iFixModel), m_iTransferID(iTransferID)
{
	//{{AFX_DATA_INIT(CTeachTransferDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	switch (m_iTransferID) {
#ifdef DEF_USE_TRANSFER_CENTER
	case DEF_PANEL_TRANSFER_CENTER:
		m_plnkPanelTransfer = MOlbSystemPre.GetPanelTransferCenterComponent();
		break;
#endif
#ifdef DEF_SOURCE_SYSTEM
	case DEF_PANEL_TRANSFER_IN:
		m_plnkPanelTransfer = MOlbSystemPre.GetPanelTransferComponent();
		break;
#endif
	case DEF_PANEL_TRANSFER_OUT:
		m_plnkPanelTransfer = MOlbSystemPre.GetPanelTransferOutComponent();
		break;
	default:
		ASSERT(FALSE);
	}

	m_plnkPanelTransfer->GetXPosParameter(m_rgdTransferFixedPosData, m_rgdTransferOffsetPosData, m_rgdTransferModelPosData);

	m_plnkC_MngOpPanel	= MOlbSystemPre.GetManageOpPanelComponent();

	m_bPanelTransferVacSts = m_plnkPanelTransfer->IsPanelAbsorbed();
	m_bPanelTransferVacOnCheck = !m_bPanelTransferVacSts;

	m_bPanelTransferUpSts = m_plnkPanelTransfer->IsUpPickUpUDCyl();
	m_bPanelTransferUpCheck = !m_bPanelTransferUpSts;

//@	m_bAirCVBlowOnSts = m_plnkPanelTransfer->IsAirCVBlowOn();
//@	m_bAirCVBlowOnCheck = !m_bAirCVBlowOnSts;

//@	m_bAirCVVacSts = m_plnkPanelTransfer->IsAirCVVacAllOn();
//@	m_bAirCVVacOnCheck = !m_bAirCVVacSts;

	m_dDisplayTransferOffset = 0.0;
	m_iSelPos = 0;
	m_iJogSelectIndex = 0;
	m_uiTimerID = 0;
}


void CTeachTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachTransferDlg)
	DDX_Control(pDX, IDB_SAVE, m_btnSave);
	DDX_Control(pDX, IDB_JOG, m_btnJogSel);
	DDX_Control(pDX, IDB_AFU_X, m_ctrlBtnAxis[eAxisXFirst]);
	//DDX_Control(pDX, IDC_GRD_AXISX_SNS_STS, m_grd_AxisSnsState[eAxisXFirst]);
	

	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDB_CM_MOVE,					m_BtnCmd[0]);
	DDX_Control(pDX, IDB_CM_RESERVED0,				m_BtnCmd[1]);
	DDX_Control(pDX, IDB_POS_SET,					m_BtnCmd[2]);
	DDX_Control(pDX, IDB_CM_RESERVED1,				m_BtnCmd[3]);
	DDX_Control(pDX, IDB_CM_TRANSFER_UP_DOWN,		m_BtnCmd[4]);
	DDX_Control(pDX, IDB_CM_TRANSFER_VACUUM,		m_BtnCmd[5]);
	DDX_Control(pDX, IDB_CM_TRANSFER_UPVAC,			m_BtnCmd[6]);
	DDX_Control(pDX, IDB_CM_TRANSFER_RELEASEDOWN,	m_BtnCmd[7]);
	DDX_Control(pDX, IDB_CM_AIR_CV_ONOFF,			m_BtnCmd[8]);
	DDX_Control(pDX, IDB_CM_AIR_CV_VAC_ONOFF,		m_BtnCmd[9]);

	DDX_Control(pDX, IDB_AFU_POS0, m_BtnPos[0]);
	DDX_Control(pDX, IDB_AFU_POS1, m_BtnPos[1]);
	DDX_Control(pDX, IDB_AFU_POS2, m_BtnPos[2]);

	DDX_Control(pDX, IDC_TGT_POS,		m_sCoordTitle[0]);
	DDX_Control(pDX, IDC_TGT_POS_X,		m_sCoord[0][0]);
	DDX_Control(pDX, IDC_FIX_POS,		m_sCoordTitle[1]);
	DDX_Control(pDX, IDC_FIX_POS_X,		m_sCoord[1][0]);
	DDX_Control(pDX, IDC_MODEL_POS,		m_sCoordTitle[2]);
	DDX_Control(pDX, IDC_MODEL_POS_X,	m_sCoord[2][0]);
	DDX_Control(pDX, IDC_OFFSET_POS,	m_sCoordTitle[3]);
	DDX_Control(pDX, IDC_OFFSET_POS_X,	m_sCoord[3][0]);
	DDX_Control(pDX, IDC_CUR_POS,		m_sCoordTitle[4]);
	DDX_Control(pDX, IDC_CUR_POS_X,		m_sCoord[4][0]);
	DDX_Control(pDX, IDC_DIF_POS,		m_sCoordTitle[5]);
	DDX_Control(pDX, IDC_DIF_POS_X,		m_sCoord[5][0]);
}


BEGIN_MESSAGE_MAP(CTeachTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachTransferDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachTransferDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachTransferDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachTransferDlg)
	ON_EVENT(CTeachTransferDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_JOG, -600 /* Click */, OnClickJog, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_AFU_POS0, -600 /* Click */, OnClickAfuPos0, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_AFU_POS1, -600 /* Click */, OnClickAfuPos1, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_AFU_X, -600 /* Click */, OnClickAfuX, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_CM_MOVE, -600 /* Click */, OnClickCmMove, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_POS_SET, -600 /* Click */, OnClickPosSet, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_CM_TRANSFER_UP_DOWN, -600 /* Click */, OnClickCmTransferUpDown, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_CM_TRANSFER_VACUUM, -600 /* Click */, OnClickCmTransferVacuum, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_CM_TRANSFER_UPVAC, -600 /* Click */, OnClickCmTransferUpvac, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_CM_TRANSFER_RELEASEDOWN, -600 /* Click */, OnClickCmTransferReleasedown, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_CM_AIR_CV_ONOFF, -600 /* Click */, OnClickCmAirCvOnoff, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_CM_AIR_CV_VAC_ONOFF, -600 /* Click */, OnClickCmAirCvVacOnoff, VTS_NONE)
	ON_EVENT(CTeachTransferDlg, IDB_AFU_POS2, -600 /* Click */, OnClickAfuPos2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachTransferDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CTeachTransferDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	SetDlgCaption();
	UpdateCoordinate();
	SelectPosition(m_iSelPos);
	DisplayLanguage();
	SetJogObject();

	m_BtnCmd[8].EnableWindow(FALSE);
	m_BtnCmd[9].EnableWindow(FALSE);

	m_uiTimerID = SetTimer(1, 300, NULL);


	// Doolin kwangilshin 2017. 08. 24.
	//
	Init_GridAxisStste();
	// End

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachTransferDlg::SetDlgCaption()
{
	TCHAR* strDlgName[DEF_MAX_PANEL_TRANSFER][2] = {
#ifdef DEF_USE_TRANSFER_CENTER  //170919 JSh
		{ _T("ACF Transfer-Center Fixed Pos Teaching"), _T("ACF Transfer-Center Model Pos Teaching") },
#endif
		{ _T("ACF Transfer-In Fixed Pos Teaching"),		_T("ACF Transfer-In Model Pos Teaching") },
		{ _T("ACF Transfer-Out Fixed Pos Teaching"),	_T("ACF Transfer-Out Model Pos Teaching") }
	};

	SetWindowText(strDlgName[m_iTransferID][m_iFixModel]);

	// Position Name
	STeachingInfo sTeachingInfo = m_plnkPanelTransfer->GetXTeachingInfo();
	for (int i=0; i<DEF_PANEL_TRANSFER_MAX_POSITION; i++) 
	{
		m_BtnPos[i].SetWindowText((LPCTSTR)(sTeachingInfo.m_sPosition[i].m_strNewLineName[0]));
	}

	// Button Disable
//@	if (m_iTransferID != DEF_PANEL_TRANSFER_CENTER) {
		//m_BtnPos[2].ShowWindow(SW_HIDE);
//@	}

	//170201 SJ_YSH
	if(m_iTransferID != DEF_PANEL_TRANSFER_IN) 
	{
		m_BtnPos[2].ShowWindow(SW_HIDE);
	}
	//_____________
}

void CTeachTransferDlg::UpdateDisplay()
{
	WCHAR szTemp[250];
	double dCurrentPos = 0.0;	
	double dTargetPos = 0.0;

	memset(szTemp, NULL, sizeof(szTemp));

	dTargetPos = m_rgdTransferFixedPosData[m_iSelPos] + m_rgdTransferModelPosData[m_iSelPos] + m_rgdTransferOffsetPosData[m_iSelPos];

	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0][0].SetCaption(szTemp);

#ifndef SIMULATION
	dCurrentPos = m_plnkPanelTransfer->GetXCurrentPos();
	if (m_plnkC_MngOpPanel->GetResetSWStatus()) {
		m_dDisplayTransferOffset = dCurrentPos;
	}
#else
	dCurrentPos = 0.0;
#endif

	dCurrentPos = dCurrentPos - m_dDisplayTransferOffset;	
	
	swprintf(szTemp, _T("%.3f"), dCurrentPos);
	m_sCoord[4][0].SetCaption(szTemp);
	
	// 편차 계산하기
	swprintf(szTemp, _T("%.3f"), dTargetPos - dCurrentPos);
	m_sCoord[5][0].SetCaption(szTemp);
}

/**
 * 고정좌표, Offset좌표 Display (Carrier축)
 */
void CTeachTransferDlg::UpdateCoordinate()
{
	WCHAR szTemp[250];

	memset(szTemp, NULL, sizeof(szTemp));
	
	/** 고정좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdTransferFixedPosData[m_iSelPos]);
	m_sCoord[1][0].SetCaption(szTemp);
	/** 모델좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdTransferModelPosData[m_iSelPos]);
	m_sCoord[2][0].SetCaption(szTemp);
	/** 옵셋좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdTransferOffsetPosData[m_iSelPos]);
	m_sCoord[3][0].SetCaption(szTemp);
}

void CTeachTransferDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelPos].SetValue(FALSE);
	m_iSelPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}

// Doolin kwangilshin 2017. 08. 24.
//
void CTeachTransferDlg::Init_GridAxisStste()
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


BOOL CTeachTransferDlg::Check_AxisStste(IAxis *pclsAxis)
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


void CTeachTransferDlg::SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome)
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


void CTeachTransferDlg::SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachTransferDlg::SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachTransferDlg::SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable)
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


void CTeachTransferDlg::updateAxisSnsSts()
{
	IAxis *pclsAxis[eAxisEnd + 1];
	
	pclsAxis[eAxisXFirst]	= m_plnkPanelTransfer->GetMotionComponent(DEF_PANEL_TRASFER_X1_AXIS);
	pclsAxis[eAxisXSecond]	= m_plnkPanelTransfer->GetMotionComponent(DEF_PANEL_TRASFER_X2_AXIS);

	//if (pclsAxis[eAxisXFirst] && pclsAxis[eAxisXSecond])
	//{
	//	if (pclsAxis[eAxisXFirst]->IsAmpEnable() && pclsAxis[eAxisXSecond]->IsAmpEnable())
	//	{	
	//		SetAxisAmpEnable(&m_grd_AxisSnsState[eAxisXFirst], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisAmpEnable(&m_grd_AxisSnsState[eAxisXFirst], FALSE);
	//	}
	//	
	//	if (pclsAxis[eAxisXFirst]->IsOriginReturn() && pclsAxis[eAxisXSecond]->IsOriginReturn())
	//	{	
	//		SetAxisHomeState(&m_grd_AxisSnsState[eAxisXFirst], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisHomeState(&m_grd_AxisSnsState[eAxisXFirst], FALSE);
	//	}
	//	
	//	if (pclsAxis[eAxisXFirst]->IsOnNegativeSensor() || pclsAxis[eAxisXSecond]->IsOnNegativeSensor())
	//	{	
	//		SetAxisNLimitSensor(&m_grd_AxisSnsState[eAxisXFirst], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisNLimitSensor(&m_grd_AxisSnsState[eAxisXFirst], FALSE);
	//	}
	//	
	//	if (pclsAxis[eAxisXFirst]->IsOnPositiveSensor() || pclsAxis[eAxisXSecond]->IsOnPositiveSensor())
	//	{	
	//		SetAxisPLimitSensor(&m_grd_AxisSnsState[eAxisXFirst], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisPLimitSensor(&m_grd_AxisSnsState[eAxisXFirst], FALSE);
	//	}
	//	
	//	if (pclsAxis[eAxisXFirst]->IsAmpFault() || pclsAxis[eAxisXFirst]->IsEStopEvent() || 
	//		pclsAxis[eAxisXSecond]->IsAmpFault() || pclsAxis[eAxisXSecond]->IsEStopEvent())
	//	{	
	//		m_ctrlBtnAxis[eAxisXFirst].SetBackColorInterior(RGB(192, 0, 0));			
	//	}
	//	else
	//	{
	//		m_ctrlBtnAxis[eAxisXFirst].SetBackColorInterior(BASIC);
	//	}
	//}
	//else
	//{
	//	if (pclsAxis[eAxisXFirst]->IsAmpEnable())
	//	{	
	//		SetAxisAmpEnable(&m_grd_AxisSnsState[eAxisXFirst], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisAmpEnable(&m_grd_AxisSnsState[eAxisXFirst], FALSE);
	//	}
	//	
	//	if (pclsAxis[eAxisXFirst]->IsOriginReturn())
	//	{	
	//		SetAxisHomeState(&m_grd_AxisSnsState[eAxisXFirst], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisHomeState(&m_grd_AxisSnsState[eAxisXFirst], FALSE);
	//	}
	//	
	//	if (pclsAxis[eAxisXFirst]->IsOnNegativeSensor())
	//	{	
	//		SetAxisNLimitSensor(&m_grd_AxisSnsState[eAxisXFirst], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisNLimitSensor(&m_grd_AxisSnsState[eAxisXFirst], FALSE);
	//	}
	//	
	//	if (pclsAxis[eAxisXFirst]->IsOnPositiveSensor())
	//	{	
	//		SetAxisPLimitSensor(&m_grd_AxisSnsState[eAxisXFirst], TRUE);
	//	}
	//	else
	//	{
	//		SetAxisPLimitSensor(&m_grd_AxisSnsState[eAxisXFirst], FALSE);
	//	}
	//	
	//	if (pclsAxis[eAxisXFirst]->IsAmpFault() || pclsAxis[eAxisXFirst]->IsEStopEvent())
	//	{	
	//		m_ctrlBtnAxis[eAxisXFirst].SetBackColorInterior(RGB(192, 0, 0));			
	//	}
	//	else
	//	{
	//		m_ctrlBtnAxis[eAxisXFirst].SetBackColorInterior(BASIC);
	//	}
	//}
}
// End.



void CTeachTransferDlg::OnClickJog() 
{
	// TODO: Add your control notification handler code here
	// 우선 동시동작만 구현
	switch (m_iTransferID) {
		//161229 SJ_YSH 살려요
	case DEF_PANEL_TRANSFER_CENTER:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_CENTER_X);
		break;
#ifdef DEF_SOURCE_SYSTEM
	case DEF_PANEL_TRANSFER_IN:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_IN_X);
		break;
#endif
	case DEF_PANEL_TRANSFER_OUT:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_OUT_X);
		break;
	default:
		ASSERT(FALSE);
	}
/*
	m_iJogSelectIndex++;

	CString szTextTemp;
	if (m_iJogSelectIndex == 0)
	{
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_IN);
		m_ctrlBtnXAxis.SetBackColorInterior(BASIC);
		//X축 이동
		ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_23"), &szTextTemp);
		m_ctrlBtnXAxis.SetCaption(szTextTemp);
	}
	else if (m_iJogSelectIndex == 1)
	{
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_X2);
		m_ctrlBtnZAxis.SetBackColorInterior(DEF_BTN_ON_COLOR);
		//X2 Jog 이동
		ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelAlignerDlg_22"), &szTextTemp);
		m_ctrlBtnZAxis.SetCaption(szTextTemp);
		
		m_bTransferXJog = FALSE;
	}
	*/
}

void CTeachTransferDlg::OnClickAfuPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);
}

void CTeachTransferDlg::OnClickAfuPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}

void CTeachTransferDlg::OnClickAfuPos2() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(2);
}

void CTeachTransferDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent) {
		UpdateDisplay();
		UpdateButton();

		// Doolin kwangilshin 2017. 08. 25.
		//
		updateAxisSnsSts();
		// End.
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTeachTransferDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	//Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 50500, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

	if (m_iFixModel == DEF_FIX_MODE) {
		m_plnkPanelTransfer->SaveXFixedPosParameter(m_rgdTransferFixedPosData);
	}
	else if (m_iFixModel == DEF_MODEL_MODE) {
		m_plnkPanelTransfer->SaveXOffsetPosParameter(m_rgdTransferOffsetPosData);
	}
}

void CTeachTransferDlg::OnClickAfuX() 
{
	// TODO: Add your control notification handler code here
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	double dNewTargetPos;

	WCHAR szTemp[250];
	memset(szTemp, NULL, sizeof(szTemp));

	m_sCoord[0][0].GetWindowText(cBeforeVal, 30);

	// Doolin kwangilshin 2017. 08. 25.
	//
	
	IAxis *pclsAxis[eAxisEnd + 1];
	
	pclsAxis[eAxisXFirst]	= m_plnkPanelTransfer->GetMotionComponent(DEF_PANEL_TRASFER_X1_AXIS);
	pclsAxis[eAxisXSecond]	= m_plnkPanelTransfer->GetMotionComponent(DEF_PANEL_TRASFER_X2_AXIS);

	if (pclsAxis[eAxisXFirst] && pclsAxis[eAxisXSecond])
	{
		if (!Check_AxisStste(pclsAxis[eAxisXFirst]) || !Check_AxisStste(pclsAxis[eAxisXSecond]))
		{
			return;
		}
	}
	else
	{
		if (!Check_AxisStste(pclsAxis[eAxisXFirst]))
		{
			return;
		}
	}
	// End.

	/** 목표위치 입력 받기 */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** 목표위치 설정하기 */
	dNewTargetPos = wcstod(cAfterVal,NULL);
	m_sCoord[0][0].SetCaption(cAfterVal);
	if (m_iFixModel == DEF_FIX_MODE)
	{
		m_rgdTransferFixedPosData[m_iSelPos] = dNewTargetPos
												- m_rgdTransferModelPosData[m_iSelPos]
												- m_rgdTransferOffsetPosData[m_iSelPos];
														
		swprintf(szTemp, _T("%.3f"), m_rgdTransferFixedPosData[m_iSelPos]);
		m_sCoord[1][0].SetCaption(szTemp);
	}
	else if (m_iFixModel == DEF_MODEL_MODE)
	{
		m_rgdTransferOffsetPosData[m_iSelPos] = dNewTargetPos
												- m_rgdTransferModelPosData[m_iSelPos]
												- m_rgdTransferFixedPosData[m_iSelPos];
														
		swprintf(szTemp, _T("%.3f"), m_rgdTransferOffsetPosData[m_iSelPos]);
		m_sCoord[3][0].SetCaption(szTemp);
	}
}

void CTeachTransferDlg::OnClickCmMove() 
{
	// TODO: Add your control notification handler code here
	/*/// 2010.07.27 KBC -----------------------
	if (!m_iJogSelectIndex) {
		//Transfer X2축이 Jog 설정되어 있습니다. Jog Mode를 바꿔주세요!
		MyMessageBox(MY_MESSAGE_BOX, 54003, _T("Error!"), M_ICONERROR);

		return;
	}
	/*/// 2010.07.27 KBC -----------------------
#ifndef DEF_SETUP
	if(m_iTransferID == DEF_PANEL_TRANSFER_OUT)
	{
		if(FALSE == MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorClose_NextMCLoad())
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(400082000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Door Open Error"), M_ICONINFORMATION);
			return;	
		}
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
	if(m_iTransferID == DEF_PANEL_TRANSFER_CENTER)
	{
		if (m_plnkPanelTransfer->IsPanelAbsorbed() == TRUE && 
			m_iSelPos == DEF_PANEL_TRANSFER_UNLOAD_POS &&
			MOlbSystemPre.GetPanelTransferComponent()->IsPanelAbsorbed() == TRUE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(440000000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Target Pos Move Error"), M_ICONINFORMATION);
			return;	
		}
	}

	if(m_iTransferID == DEF_PANEL_TRANSFER_IN)
	{
		//170916 JSH.s
#ifdef DEF_USE_TRANSFER_CENTER 
		if (m_plnkPanelTransfer->IsPanelAbsorbed() == TRUE
			&& m_iSelPos == DEF_PANEL_TRANSFER_LOAD_POS
			&& MOlbSystemPre.GetPanelTransferCenterComponent()->IsPanelAbsorbed() == TRUE //kdh Test
			)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(440001000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Target Pos Move Error"), M_ICONINFORMATION);
			return;	
		}
#endif
		//170916 JSH.e

		if (m_plnkPanelTransfer->IsPanelAbsorbed() == TRUE && 
			m_iSelPos == DEF_PANEL_TRANSFER_UNLOAD_POS &&
			MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed() == TRUE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(440002000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Target Pos Move Error"), M_ICONINFORMATION);
			return;	
		}
	}
	if(m_iTransferID == DEF_PANEL_TRANSFER_OUT)
	{
		if (m_plnkPanelTransfer->IsPanelAbsorbed() == TRUE && 
			m_iSelPos == DEF_PANEL_TRANSFER_LOAD_POS &&
			MOlbSystemPre.GetPanelAlignerComponent()->IsPanelAbsorbed() == TRUE)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(440003000);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Target Pos Move Error"), M_ICONINFORMATION);
			return;	
		}
	}
	//_____________
#endif
	int iPrevJogUnit = 0;
	int iResult = 0;
//@	if (m_plnkPanelTransfer->IsPanelAbsorbed() && (m_iTransferID != DEF_PANEL_TRANSFER_CENTER)) {
	if (m_plnkPanelTransfer->IsPanelAbsorbed())
	{
		double currentX = 0;
		double currentX2 = 0;
		currentX = m_plnkPanelTransfer->GetXCurrentPos();
		currentX2 = m_plnkPanelTransfer->GetX2CurrentPos();

		if (fabs(currentX - currentX2) > 1) {
			//Panel Transfer에 Panel이 흡착되어 있는데 X1축과 X2축의 현재 위치가 다릅니다. Panel을 흡착해제 하세요.
			MyMessageBox(MY_MESSAGE_BOX, 54004, _T("Error!"), M_ICONERROR);

			return;
		}
	}
	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Move the PanelTransfer X to the target position.\n")
						_T("Wait a moment..."));
	ProgWnd.PeekAndPump();

	// 이동 중 Jog를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
	iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

//170930 KDW Start //주석
//#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS //170719 JSH 
//170930 KDW End
	//170928 JSH Gear
	//	iResult = m_plnkPanelTransfer->SafeSyncMoveXPos(m_iSelPos, FALSE);
	iResult = m_plnkPanelTransfer->SafeMoveXPos(m_iSelPos, FALSE);
//170930 KDW Start //주석
//#else
//	if (m_iTransferID == DEF_PANEL_TRANSFER_OUT) {
//		iResult = m_plnkPanelTransfer->SafeMoveXPos(m_iSelPos, FALSE);
//	}
//	else {
//		iResult = m_plnkPanelTransfer->SafeSyncMoveXPos(m_iSelPos, FALSE);
//	}
//#endif
//170930 KDW End

	if (iResult != ERR_TRANSFER_TEACH_DLG_SUCCESS) {
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	ProgWnd.StepIt();

	// Jog 사용 재개
	m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
}

void CTeachTransferDlg::OnClickPosSet() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[250];	
	memset(szTemp, NULL, sizeof(szTemp));
	
	//목표 위치를 현재 위치로 변경하시겠습니까?
	if ( MyMessageBox(MY_MESSAGE_BOX, 50507, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;
	
	double dCurrent;
	double dTargetPos;
	
	dCurrent = m_plnkPanelTransfer->GetXCurrentPos();
	
	dTargetPos = dCurrent;
	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0][0].SetCaption(szTemp);
	
	if (m_iFixModel == DEF_FIX_MODE) {
		m_rgdTransferFixedPosData[m_iSelPos] = dTargetPos
												- m_rgdTransferModelPosData[m_iSelPos]
												- m_rgdTransferOffsetPosData[m_iSelPos];
		swprintf(szTemp, _T("%.3f"), m_rgdTransferFixedPosData[m_iSelPos]);
		m_sCoord[1][0].SetCaption(szTemp);
	}
	else if (m_iFixModel == DEF_MODEL_MODE) {
		m_rgdTransferOffsetPosData[m_iSelPos] = dTargetPos
												- m_rgdTransferModelPosData[m_iSelPos]
												- m_rgdTransferFixedPosData[m_iSelPos];
		swprintf(szTemp, _T("%.3f"), m_rgdTransferOffsetPosData[m_iSelPos]);
		m_sCoord[3][0].SetCaption(szTemp);
	}
}

void CTeachTransferDlg::DisplayLanguage()
{
	CString szTextTemp;
	//Jog 축 변경
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_1"), &szTextTemp);
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	// X축 
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_2"), &szTextTemp);
	GetDlgItem(IDB_AFU_X)->SetWindowText(szTextTemp);
	//목표 위치
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_3"), &szTextTemp);
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	//고정 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_4"), &szTextTemp);
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	//모델 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_5"), &szTextTemp);
	GetDlgItem(IDC_MODEL_POS)->SetWindowText(szTextTemp);
	//Offset 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_6"), &szTextTemp);
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	//현재 위치
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_7"), &szTextTemp);
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	//차이값
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_8"), &szTextTemp);
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);

	//목표 위치로\r\n이동
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_9"), &szTextTemp);
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	//목표 위치를\r\n현재값으로
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_10"), &szTextTemp);
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	//저장
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_11"), &szTextTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	//나가기
	ReadDisplayMessage(MAIN_TEACH, _T("TeachPanelTransferDlg_12"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
}

void CTeachTransferDlg::OnClickCmTransferUpDown() 
{
	// TODO: Add your control notification handler code here
	CMyProgressWnd ProgWnd(NULL, _T("PanelTransfer Up/Down..."));
	ProgWnd.SetRange(0, 1);

	int iResult = ERR_TRANSFER_TEACH_DLG_SUCCESS;
	if (m_bPanelTransferUpSts == TRUE)
	{
		ProgWnd.SetText(_T("PanelTransfer Down.\nWait a moment..."));
		ProgWnd.PeekAndPump();

		// hongju 추가 Check Interlock
		iResult = m_plnkPanelTransfer->IsPanelReleased();
		if (!iResult)
		{
			MyMessageBox(_T("Panel Transfer Panel Absorbed"), _T("Error"), M_ICONERROR);
			return;
		}

		iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
	}
	else
	{
		ProgWnd.SetText(_T("PanelTransfer Up.\nWait for one moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkPanelTransfer->UpPickUpUDCyl();
	}

	if (iResult != ERR_TRANSFER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	m_bPanelTransferUpSts = !m_bPanelTransferUpSts;

	ProgWnd.StepIt();	
}

void CTeachTransferDlg::OnClickCmTransferVacuum() 
{
	// TODO: Add your control notification handler code here
	CMyProgressWnd ProgWnd(NULL, _T("PanelTransfer Vacuum..."));
	ProgWnd.SetRange(0, 1);

	int iResult = ERR_TRANSFER_TEACH_DLG_SUCCESS;
	if (m_bPanelTransferVacSts == TRUE)
	{
		ProgWnd.SetText(_T("Releasing the PanelTransfer Vacuum.\nWait a moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkPanelTransfer->ReleasePanel();
	}
	else
	{
		ProgWnd.SetText(_T("PanelTransfer Vacuum .\nWait for one moment..."));
		ProgWnd.PeekAndPump();

		iResult = m_plnkPanelTransfer->AbsorbPanel();
	}

	if (iResult != ERR_TRANSFER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	m_bPanelTransferVacSts = !m_bPanelTransferVacSts;

	ProgWnd.StepIt();	
}

void CTeachTransferDlg::OnClickCmTransferUpvac() 
{
	// TODO: Add your control notification handler code here
	CMyProgressWnd ProgWnd(NULL, _T("Stopper Down/Transfer Up/Vacuum..."));
	ProgWnd.SetRange(0, 3);
	Sleep(500);
	ProgWnd.StepIt();
	ProgWnd.SetText(_T("PanelTransfer Up.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	m_plnkPanelTransfer->UpPickUpUDCyl();
	Sleep(500);
	ProgWnd.StepIt();
	ProgWnd.SetText(_T("PanelTransfer Vacuum.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	m_plnkPanelTransfer->AbsorbPanel();
	Sleep(500);
	ProgWnd.StepIt();
}

void CTeachTransferDlg::OnClickCmTransferReleasedown() 
{
	// TODO: Add your control notification handler code here
	CMyProgressWnd ProgWnd(NULL, _T("Stopper Down/Transfer Up/Vacuum..."));
	ProgWnd.SetRange(0, 3);
	Sleep(500);
	ProgWnd.StepIt();
	ProgWnd.SetText(_T("PanelTransfer Release.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	m_plnkPanelTransfer->ReleasePanel();
	Sleep(500);
	ProgWnd.StepIt();
	ProgWnd.SetText(_T("PanelTransfer Down.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	m_plnkPanelTransfer->DownPickUpUDCyl();
	Sleep(500);
	ProgWnd.StepIt();
}

void CTeachTransferDlg::OnClickCmAirCvOnoff() 
{
	// TODO: Add your control notification handler code here
	/*/
	int iResult = ERR_TRANSFER_TEACH_DLG_SUCCESS;
	if (m_bAirCVBlowOnSts == TRUE)
	{
		iResult = m_plnkPanelTransfer->AirCVBlowOff();
	}
	else
	{
		iResult = m_plnkPanelTransfer->AirCVBlowOn();
		m_bAirCVVacSts = FALSE;
	}

	if (iResult != ERR_TRANSFER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	m_bAirCVBlowOnSts = !m_bAirCVBlowOnSts;
	/*/
}

void CTeachTransferDlg::OnClickCmAirCvVacOnoff() 
{
	// TODO: Add your control notification handler code here
	/*/
	int iResult = ERR_TRANSFER_TEACH_DLG_SUCCESS;
	if (m_bAirCVVacSts == TRUE)
	{
		iResult = m_plnkPanelTransfer->AirCVVacAllOff();
	}
	else
	{
		iResult = m_plnkPanelTransfer->AirCVVacAllOn();
		m_bAirCVBlowOnSts = FALSE;
	}

	if (iResult != ERR_TRANSFER_TEACH_DLG_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	m_bAirCVVacSts = !m_bAirCVVacSts;
	/*/
}

void CTeachTransferDlg::UpdateButton()
{
	// PanelTransfer
	if (!m_bPanelTransferUpCheck && m_plnkPanelTransfer->IsUpPickUpUDCyl())
	{
		m_BtnCmd[4].SetBackColorInterior(DEF_BTN_ON_COLOR);
		m_bPanelTransferUpCheck = TRUE;
	}
	if (m_bPanelTransferUpCheck && !m_plnkPanelTransfer->IsUpPickUpUDCyl())
	{
		m_BtnCmd[4].SetBackColorInterior(BASIC);
		m_bPanelTransferUpCheck = FALSE;
	}
	
	if (!m_bPanelTransferVacOnCheck && m_plnkPanelTransfer->IsPanelAbsorbed())
	{
		m_BtnCmd[5].SetBackColorInterior(DEF_BTN_ON_COLOR);
		m_bPanelTransferVacOnCheck = TRUE;
	}
	if (m_bPanelTransferVacOnCheck && !m_plnkPanelTransfer->IsPanelAbsorbed())
	{
		m_BtnCmd[5].SetBackColorInterior(BASIC);
		m_bPanelTransferVacOnCheck = FALSE;
	}

	/*/
	if (!m_bAirCVBlowOnCheck && m_plnkPanelTransfer->IsAirCVBlowOn())
	{
		m_BtnCmd[8].SetBackColorInterior(DEF_BTN_ON_COLOR);
		m_bAirCVBlowOnCheck = TRUE;
	}
	if (m_bAirCVBlowOnCheck && !m_plnkPanelTransfer->IsAirCVBlowOn())
	{
		m_BtnCmd[8].SetBackColorInterior(BASIC);
		m_bAirCVBlowOnCheck = FALSE;
	}

	if (!m_bAirCVVacOnCheck && m_plnkPanelTransfer->IsAirCVVacAllOn())
	{
		m_BtnCmd[9].SetBackColorInterior(DEF_BTN_ON_COLOR);
		m_bAirCVVacOnCheck = TRUE;
	}
	if (m_bAirCVVacOnCheck && !m_plnkPanelTransfer->IsAirCVVacAllOn())
	{
		m_BtnCmd[9].SetBackColorInterior(BASIC);
		m_bAirCVVacOnCheck = FALSE;
	}
	/*/

}

void CTeachTransferDlg::SetJogObject()
{
	switch (m_iTransferID) {
		//161229 SJ_YSH 살려요
	case DEF_PANEL_TRANSFER_CENTER:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_CENTER_X);
		break;
#ifdef DEF_SOURCE_SYSTEM
	case DEF_PANEL_TRANSFER_IN:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_IN_X);
		break;
#endif
	case DEF_PANEL_TRANSFER_OUT:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_PANEL_TRANSFER_OUT_X);
		break;
	default:
		ASSERT(FALSE);
	}

}
