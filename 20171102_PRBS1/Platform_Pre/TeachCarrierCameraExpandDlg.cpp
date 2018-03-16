// TeachTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "TeachCarrierCameraExpandDlg.h"
#include "MPlatformOlbSystem.h"
#include <math.h>
#include "MManageOpPanel.h"
#include "MSystemData.h"
#include "MTrsAutoManager.h"

#include "TeachMainForm.h"
#include "IAxis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeachCarrierCameraExpandDlg dialog

extern	MPlatformOlbSystem	MOlbSystemPre;

CTeachCarrierCameraExpandDlg::CTeachCarrierCameraExpandDlg(int iFixModel, int iGroupID, CWnd* pParent /*=NULL*/)
: CDialog(CTeachCarrierCameraExpandDlg::IDD, pParent), m_iFixModel(iFixModel),m_iGroupID(iGroupID)
{
	//{{AFX_DATA_INIT(CTeachCarrierCameraExpandDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	switch (iGroupID) {
	case DEF_FRONT_GROUP:
		m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(0);
		break;
	case DEF_REAR_GROUP:
		m_plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(0);
		break;
	default:
		ASSERT(FALSE);
	}

	m_plnkTabCarrier->GetExpandPosParameter(m_rgdExpandFixedPosData, m_rgdExpandOffsetPosData, m_rgdExpandModelPosData);

	m_plnkC_MngOpPanel	= MOlbSystemPre.GetManageOpPanelComponent();

	m_plnkSystemData = MOlbSystemPre.GetSystemData();

	m_dDisplayExpandOffset = 0.0;
	m_iSelPos = 0;
	m_iJogSelectIndex = 0;
	m_uiTimerID = 0;
	m_iExpandOldID = -1;

	m_iExpandID = 0;

}


void CTeachCarrierCameraExpandDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachCarrierCameraExpandDlg)
	DDX_Control(pDX, IDB_SAVE, m_btnSave);
	DDX_Control(pDX, IDB_JOG, m_btnJogSel);
	
	DDX_Control(pDX, IDB_AFU_X, m_ctrlBtnAxis[eAxisX]);
//	DDX_Control(pDX, IDC_GRD_AXISX_SNS_STS, m_grd_AxisSnsState[eAxisX]);
	

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

	DDX_Control(pDX, IDB_SELECT_NEXT,		m_btnNextExpand); //SJ_YYK 150817 Add...
	DDX_Control(pDX, IDB_SELECT_PREVIOS,	m_btnPrevExpand); //SJ_YYK 150817 Add...
	DDX_Control(pDX, IDC_WORKER_NO,			m_lblWorkerNo);  //SJ_YYK 150817 Add...
}


BEGIN_MESSAGE_MAP(CTeachCarrierCameraExpandDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachCarrierCameraExpandDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachCarrierCameraExpandDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachCarrierCameraExpandDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachCarrierCameraExpandDlg)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_JOG, -600 /* Click */, OnClickJog, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_AFU_POS0, -600 /* Click */, OnClickAfuPos0, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_AFU_POS1, -600 /* Click */, OnClickAfuPos1, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_AFU_X, -600 /* Click */, OnClickAfuX, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_CM_MOVE, -600 /* Click */, OnClickCmMove, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_POS_SET, -600 /* Click */, OnClickPosSet, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_CM_TRANSFER_UP_DOWN, -600 /* Click */, OnClickCmTransferUpDown, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_CM_TRANSFER_VACUUM, -600 /* Click */, OnClickCmTransferVacuum, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_CM_TRANSFER_UPVAC, -600 /* Click */, OnClickCmTransferUpvac, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_CM_TRANSFER_RELEASEDOWN, -600 /* Click */, OnClickCmTransferReleasedown, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_CM_AIR_CV_ONOFF, -600 /* Click */, OnClickCmAirCvOnoff, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_CM_AIR_CV_VAC_ONOFF, -600 /* Click */, OnClickCmAirCvVacOnoff, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_SELECT_PREVIOS, -600 /* Click */, OnClickSelectPrevios, VTS_NONE)
	ON_EVENT(CTeachCarrierCameraExpandDlg, IDB_SELECT_NEXT, -600 /* Click */, OnClickSelectNext, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachCarrierCameraExpandDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CTeachCarrierCameraExpandDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	SetDlgCaption();
	UpdateCoordinate();
	SelectPosition(m_iSelPos);
	DisplayLanguage();
	SetJogObject();

	m_uiTimerID = SetTimer(1, 300, NULL);

	// Doolin kwangilshin 2017. 08. 24.
	//
	Init_GridAxisStste();
	// End


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachCarrierCameraExpandDlg::SetDlgCaption()
{
	TCHAR* strDlgName[DEF_MAX_WORKER_GROUP][2] = {
		{ _T("TabCarrier Camera Expand F Fixed Pos Teaching"), _T("TabCarrier Camera Expand F Model Pos Teaching") },
		{ _T("TabCarrier Camera Expand R Fixed Pos Teaching"),	_T("TabCarrier Camera Expand R Model Pos Teaching") },
	};

	SetWindowText(strDlgName[m_iGroupID][m_iFixModel]);

	// Position Name
	STeachingInfo sTeachingInfo = m_plnkTabCarrier->GetExpandTeachingInfo();
	for (int i=0; i<DEF_TABCARRIER_EXPAND_MAX_POS; i++) {
		m_BtnPos[i].SetWindowText((LPCTSTR)(sTeachingInfo.m_sPosition[i].m_strNewLineName[0]));
	}

	// Button Disable
	/*if (m_iExpandID != DEF_PANEL_TRANSFER_CENTER) {
		m_BtnPos[2].ShowWindow(SW_HIDE);
	}*/
}

void CTeachCarrierCameraExpandDlg::UpdateDisplay()
{
	WCHAR szTemp[250];
	double dCurrentPos = 0.0;	
	double dTargetPos = 0.0;

	memset(szTemp, NULL, sizeof(szTemp));

	dTargetPos = m_rgdExpandFixedPosData[m_iSelPos] + m_rgdExpandModelPosData[m_iSelPos] + m_rgdExpandOffsetPosData[m_iSelPos];

	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0][0].SetCaption(szTemp);

#ifndef SIMULATION
	dCurrentPos = m_plnkTabCarrier->GetCurrentExpandPos();
	if (m_plnkC_MngOpPanel->GetResetSWStatus()) {
		m_dDisplayExpandOffset = dCurrentPos;
	}
#else
	dCurrentPos = 0.0;
#endif

	dCurrentPos = dCurrentPos - m_dDisplayExpandOffset;	
	
	swprintf(szTemp, _T("%.3f"), dCurrentPos);
	m_sCoord[4][0].SetCaption(szTemp);
	
	// 편차 계산하기
	swprintf(szTemp, _T("%.3f"), dTargetPos - dCurrentPos);
	m_sCoord[5][0].SetCaption(szTemp);
}

/**
 * 고정좌표, Offset좌표 Display (Carrier축)
 */
void CTeachCarrierCameraExpandDlg::UpdateCoordinate()
{
	WCHAR szTemp[250];

	memset(szTemp, NULL, sizeof(szTemp));
	
	/** 고정좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdExpandFixedPosData[m_iSelPos]);
	m_sCoord[1][0].SetCaption(szTemp);
	/** 모델좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdExpandModelPosData[m_iSelPos]);
	m_sCoord[2][0].SetCaption(szTemp);
	/** 옵셋좌표 화면 Display */
	swprintf(szTemp, _T("%.3f"), m_rgdExpandOffsetPosData[m_iSelPos]);
	m_sCoord[3][0].SetCaption(szTemp);
}

void CTeachCarrierCameraExpandDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelPos].SetValue(FALSE);
	m_iSelPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}

// Doolin kwangilshin 2017. 08. 24.
//
void CTeachCarrierCameraExpandDlg::Init_GridAxisStste()
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


BOOL CTeachCarrierCameraExpandDlg::Check_AxisStste(IAxis *pclsAxis)
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


void CTeachCarrierCameraExpandDlg::SetAxisHomeState(CMSFlexGrid *pclsGrd, BOOL bIsHome)
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


void CTeachCarrierCameraExpandDlg::SetAxisPLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachCarrierCameraExpandDlg::SetAxisNLimitSensor(CMSFlexGrid *pclsGrd, BOOL bIsOn)
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


void CTeachCarrierCameraExpandDlg::SetAxisAmpEnable(CMSFlexGrid *pclsGrd, BOOL bIsEnable)
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


void CTeachCarrierCameraExpandDlg::updateAxisSnsSts()
{
	IAxis *pclsAxis[eAxisEnd];
	
	pclsAxis[eAxisX]	= m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_EXPAND_AXIS);
	
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


void CTeachCarrierCameraExpandDlg::OnClickJog() 
{
	// TODO: Add your control notification handler code here
	// 우선 동시동작만 구현
	switch (m_iGroupID) {
	case DEF_FRONT_GROUP:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABCARRIER_CAMERA_EXPAND);
		break;
	case DEF_REAR_GROUP:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABCARRIER_CAMERA_EXPAND);
		break;
	default:
		ASSERT(FALSE);
	}

}

void CTeachCarrierCameraExpandDlg::OnClickAfuPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);
}

void CTeachCarrierCameraExpandDlg::OnClickAfuPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}

void CTeachCarrierCameraExpandDlg::OnTimer(UINT nIDEvent) 
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

void CTeachCarrierCameraExpandDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	//Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 50500, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

	/*/
	if (m_iFixModel == DEF_FIX_MODE) {
#	ifdef DEF_UNB_PROJECT
		if (m_iExpandID == DEF_CARRIER_CAMERA_EXPAND_FRONT)
		{
			m_rgdExpandFixedPosData[0] = m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][4];
			m_rgdExpandFixedPosData[1] = m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][4];
		}
		else
		{
			m_rgdExpandFixedPosData[0] = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][4];
			m_rgdExpandFixedPosData[1] = m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][4];
		}
#	endif
		m_plnkTabCarrier->SaveExpandFixedPosParameter(m_rgdExpandFixedPosData);
	}
	/*/
//@	else if (m_iFixModel == DEF_MODEL_MODE)
	{
		m_plnkTabCarrier->SaveExpandOffsetPosParameter(m_rgdExpandOffsetPosData);
	}
	UpdateCoordinate();
}

void CTeachCarrierCameraExpandDlg::OnClickAfuX() 
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
	if (!Check_AxisStste(m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_EXPAND_AXIS)))
	{
		return;
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
		m_rgdExpandFixedPosData[m_iSelPos] = dNewTargetPos
												- m_rgdExpandModelPosData[m_iSelPos]
												- m_rgdExpandOffsetPosData[m_iSelPos];
														
		swprintf(szTemp, _T("%.3f"), m_rgdExpandFixedPosData[m_iSelPos]);
		m_sCoord[1][0].SetCaption(szTemp);
	}
	else if (m_iFixModel == DEF_MODEL_MODE)
	{
		m_rgdExpandOffsetPosData[m_iSelPos] = dNewTargetPos
												- m_rgdExpandModelPosData[m_iSelPos]
												- m_rgdExpandFixedPosData[m_iSelPos];
														
		swprintf(szTemp, _T("%.3f"), m_rgdExpandOffsetPosData[m_iSelPos]);
		m_sCoord[3][0].SetCaption(szTemp);
	}
}

void CTeachCarrierCameraExpandDlg::OnClickCmMove() 
{
	// TODO: Add your control notification handler code here
	/*/// 2010.07.27 KBC -----------------------
	if (!m_iJogSelectIndex) {
		//Transfer X2축이 Jog 설정되어 있습니다. Jog Mode를 바꿔주세요!
		MyMessageBox(MY_MESSAGE_BOX, 54003, _T("Error!"), M_ICONERROR);

		return;
	}
	/*/// 2010.07.27 KBC -----------------------

	
	//20171004 KDW Start
#ifndef DEF_SETUP
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen_AOC())
	{
		int iResult = 300013000;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("AOC Door Open Error"), M_ICONINFORMATION);
		return;
	}
#endif
	//20171004 KDW End

	int iPrevJogUnit = 0;
	int iResult = 0;
/*	if (m_plnkPanelTransfer->IsPanelAbsorbed() && (m_iExpandID != DEF_PANEL_TRANSFER_CENTER)) {
		double currentX = 0;
		double currentX2 = 0;
		currentX = m_plnkPanelTransfer->GetXCurrentPos();
		currentX2 = m_plnkPanelTransfer->GetX2CurrentPos();

		if (fabs(currentX - currentX2) > 1) {
			//Panel Transfer에 Panel이 흡착되어 있는데 X1축과 X2축의 현재 위치가 다릅니다. Panel을 흡착해제 하세요.
			MyMessageBox(MY_MESSAGE_BOX, 54004, _T("Error!"), M_ICONERROR);

			return;
		}
	}*/
	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Move the PanelTransfer X to the target position.\n")
						_T("Wait a moment..."));
	ProgWnd.PeekAndPump();

	// 이동 중 Jog를 사용하게 되면 Motion Error가 나므로 Jog 사용 중지
	iPrevJogUnit = m_plnkC_MngOpPanel->GetJogUnit();
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

	iResult = m_plnkTabCarrier->SafeMoveExpandPos(m_iSelPos, TRUE, FALSE);
	if (iResult != ERR_CAMDRA_EXPAND_TEACH_DLG_SUCCESS) {
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	ProgWnd.StepIt();

	// Jog 사용 재개
	m_plnkC_MngOpPanel->SetJogUnit(iPrevJogUnit);
}

void CTeachCarrierCameraExpandDlg::OnClickPosSet() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[250];	
	memset(szTemp, NULL, sizeof(szTemp));
	
	//목표 위치를 현재 위치로 변경하시겠습니까?
	if ( MyMessageBox(MY_MESSAGE_BOX, 50507, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;
	
	double dCurrent;
	double dTargetPos;
	
	dCurrent = m_plnkTabCarrier->GetCurrentExpandPos();
	
	dTargetPos = dCurrent;
	swprintf(szTemp, _T("%.3f"), dTargetPos);
	m_sCoord[0][0].SetCaption(szTemp);
	
	/*/
	if (m_iFixModel == DEF_FIX_MODE) {
		m_rgdExpandFixedPosData[m_iSelPos] = dTargetPos
												- m_rgdExpandModelPosData[m_iSelPos]
												- m_rgdExpandOffsetPosData[m_iSelPos];
		swprintf(szTemp, _T("%.3f"), m_rgdExpandFixedPosData[m_iSelPos]);
		m_sCoord[1][0].SetCaption(szTemp);
	}
	/*/
//@	else if (m_iFixModel == DEF_MODEL_MODE)
	{
		m_rgdExpandOffsetPosData[m_iSelPos] = dTargetPos
												- m_rgdExpandModelPosData[m_iSelPos]
												- m_rgdExpandFixedPosData[m_iSelPos];
		swprintf(szTemp, _T("%.3f"), m_rgdExpandOffsetPosData[m_iSelPos]);
		m_sCoord[3][0].SetCaption(szTemp);
	}
}

void CTeachCarrierCameraExpandDlg::DisplayLanguage()
{
	CString szTextTemp;
	//Jog 축 변경
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_1"), &szTextTemp);
	GetDlgItem(IDB_JOG)->SetWindowText(szTextTemp);
	// X축 
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_2"), &szTextTemp);
	GetDlgItem(IDB_AFU_X)->SetWindowText(szTextTemp);
	//목표 위치
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_3"), &szTextTemp);
	GetDlgItem(IDC_TGT_POS)->SetWindowText(szTextTemp);
	//고정 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_4"), &szTextTemp);
	GetDlgItem(IDC_FIX_POS)->SetWindowText(szTextTemp);
	//모델 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_5"), &szTextTemp);
	GetDlgItem(IDC_MODEL_POS)->SetWindowText(szTextTemp);
	//Offset 좌표
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_6"), &szTextTemp);
	GetDlgItem(IDC_OFFSET_POS)->SetWindowText(szTextTemp);
	//현재 위치
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_7"), &szTextTemp);
	GetDlgItem(IDC_CUR_POS)->SetWindowText(szTextTemp);
	//차이값
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_8"), &szTextTemp);
	GetDlgItem(IDC_DIF_POS)->SetWindowText(szTextTemp);

	//목표 위치로\r\n이동
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_9"), &szTextTemp);
	GetDlgItem(IDB_CM_MOVE)->SetWindowText(szTextTemp);
	//목표 위치를\r\n현재값으로
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_10"), &szTextTemp);
	GetDlgItem(IDB_POS_SET)->SetWindowText(szTextTemp);
	//저장
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_11"), &szTextTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	//나가기
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_12"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	//Expand 번호 선택
	ReadDisplayMessage(MAIN_TEACH, _T("TeachCarrierCameraExpandDlg_13"), &szTextTemp);
	GetDlgItem(IDS_SELECT2)->SetWindowText(szTextTemp);
	
}

void CTeachCarrierCameraExpandDlg::OnClickCmTransferUpDown() 
{
	// TODO: Add your control notification handler code here
/*	CMyProgressWnd ProgWnd(NULL, _T("PanelTransfer Up/Down..."));
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

	ProgWnd.StepIt();	*/
}

void CTeachCarrierCameraExpandDlg::OnClickCmTransferVacuum() 
{
	// TODO: Add your control notification handler code here
/*	CMyProgressWnd ProgWnd(NULL, _T("PanelTransfer Vacuum..."));
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

	ProgWnd.StepIt();	*/
}

void CTeachCarrierCameraExpandDlg::OnClickCmTransferUpvac() 
{
	// TODO: Add your control notification handler code here
	/*/// 2010.07.27 KBC ------------------------
	CMyProgressWnd ProgWnd(NULL, _T("Stopper Down/Transfer Up/Vacuum..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("PanelTransfer Stopper Down.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	m_plnkPanelTransfer->DownStopperUDCyl();
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
	/*/// 2010.07.27 KBC ------------------------
}

void CTeachCarrierCameraExpandDlg::OnClickCmTransferReleasedown() 
{
	// TODO: Add your control notification handler code here
	/*/// 2010.07.27 KBC ------------------------
	CMyProgressWnd ProgWnd(NULL, _T("Stopper Down/Transfer Up/Vacuum..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("PanelTransfer Stopper Down.\nWait a moment..."));
	ProgWnd.PeekAndPump();
	m_plnkPanelTransfer->DownStopperUDCyl();
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
	/*/// 2010.07.27 KBC ------------------------
}

void CTeachCarrierCameraExpandDlg::OnClickCmAirCvOnoff() 
{
	// TODO: Add your control notification handler code here
/*	int iResult = ERR_TRANSFER_TEACH_DLG_SUCCESS;
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
	m_bAirCVBlowOnSts = !m_bAirCVBlowOnSts;*/
}

void CTeachCarrierCameraExpandDlg::OnClickCmAirCvVacOnoff() 
{
	// TODO: Add your control notification handler code here
/*	int iResult = ERR_TRANSFER_TEACH_DLG_SUCCESS;
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
	m_bAirCVVacSts = !m_bAirCVVacSts;*/
}

void CTeachCarrierCameraExpandDlg::UpdateButton()
{
	// PanelTransfer
/*	if (!m_bPanelTransferUpCheck && m_plnkPanelTransfer->IsUpPickUpUDCyl())
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
*/
}

void CTeachCarrierCameraExpandDlg::SetJogObject()
{
	switch (m_iGroupID) {
	case DEF_FRONT_GROUP:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_F_TABCARRIER_CAMERA_EXPAND);
		break;
	case DEF_REAR_GROUP:
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_R_TABCARRIER_CAMERA_EXPAND);
		break;

	default:
		ASSERT(FALSE);
	}

}

void CTeachCarrierCameraExpandDlg::OnClickSelectPrevios() 
{
	// TODO: Add your control notification handler code here
	if (m_iExpandID - 1 < 0)
		return;

	// 확인 필요 : Timer 와의 충돌 피하기 위한 처리 필요한가?
	m_iExpandOldID = m_iExpandID;

	m_iExpandID--;

	m_btnNextExpand.EnableWindow(TRUE);

	if (m_iExpandID - 1 < 0)
		m_btnPrevExpand.EnableWindow(FALSE);

	WCHAR szTemp[250];
	CMyProgressWnd ProgWnd(NULL, _T("Change Worker..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Change TabCarrier..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	ProgWnd.StepIt();
	
	swprintf(szTemp, _T("%d"), m_iExpandID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
	
	SetWorkExpand(m_iExpandID);
	
	ProgWnd.StepIt();	
}

void CTeachCarrierCameraExpandDlg::OnClickSelectNext() 
{
	// TODO: Add your control notification handler code here
	if (m_iExpandID + 1 >= 2)
		return;

	// 확인 필요 : Timer 와의 충돌 피하기 위한 처리 필요한가?
	m_iExpandOldID = m_iExpandID;

	m_iExpandID++;

	m_btnPrevExpand.EnableWindow(TRUE);

	if (m_iExpandID + 1 >= 2)
		m_btnNextExpand.EnableWindow(FALSE);

	CMyProgressWnd ProgWnd(NULL, _T("Change Worker..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Change TabCarrier..Wait a moment..."));
	ProgWnd.PeekAndPump();
	
	ProgWnd.StepIt();
	
	WCHAR szTemp[50];
	swprintf(szTemp, _T("%d"), m_iExpandID + 1);
	m_lblWorkerNo.SetWindowText(szTemp);
	
	SetWorkExpand(m_iExpandID);
	
	ProgWnd.StepIt();
}

void CTeachCarrierCameraExpandDlg::SetWorkExpand(const int iExpandID)
{
	if (m_iExpandOldID == iExpandID)
		return;

	if (m_iGroupID == DEF_FRONT_GROUP)
	{
		m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iExpandID);
		if(iExpandID==0)
			m_iJogSelect = DEF_JOG_F_TABCARRIER_CAMERA_EXPAND;
		else 
			m_iJogSelect = DEF_JOG_F_TABCARRIER_CAMERA_EXPAND2;
	}
	else
	{
		m_plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(iExpandID);
		if(iExpandID == 0)
			m_iJogSelect = DEF_JOG_R_TABCARRIER_CAMERA_EXPAND;
		else
			m_iJogSelect = DEF_JOG_R_TABCARRIER_CAMERA_EXPAND2;
	}

	m_plnkTabCarrier->GetExpandPosParameter(m_rgdExpandFixedPosData, m_rgdExpandOffsetPosData, m_rgdExpandModelPosData);
	m_plnkC_MngOpPanel->SetJogUnit(m_iJogSelect);
	UpdateButton();	// Teaching Position Name Update
	UpdateCoordinate();	// Selected Position 에 대한 Coordinate Update
}
