// DataConveyorSpeedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataConveyorSpeedDlg.h"
#include "MPlatformOlbSystem.h"
#include "IInverter.h"
//#include "MRollerConveyor.h"
#include "MPreBonderData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataConveyorSpeedDlg dialog

CDataConveyorSpeedDlg::CDataConveyorSpeedDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataConveyorSpeedDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataConveyorSpeedDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkLoadConveyorInverter	= MOlbSystemPre.GetInverterComponent(DEF_LOAD_CONVEYOR);
	m_plnkUnloadConveyorInverter= MOlbSystemPre.GetInverterComponent(DEF_UNLOAD_CONVEYOR);
	m_plnkPreBonderData			= MOlbSystemPre.GetPreBonderData();
	m_plnkLoadConveyor			= MOlbSystemPre.GetRollerConveyorComponent(DEF_LOAD_CONVEYOR);
	m_plnkUnloadConveyor		= MOlbSystemPre.GetRollerConveyorComponent(DEF_UNLOAD_CONVEYOR);
}


void CDataConveyorSpeedDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataConveyorSpeedDlg)
	DDX_Control(pDX, IDS_LOAD_CONVEYOR_NORMAL_SPEED, m_lblLoadConveyorNormalSpeed);
	DDX_Control(pDX, IDS_LOAD_CONVEYOR_REDUCED_SPEED, m_lblLoadConveyorReducedSpeed);
	DDX_Control(pDX, IDS_LOAD_CONVEYOR_MOTOR_NORMAL_FREQ, m_lblLoadConveyorMotorNormalFreq);
	DDX_Control(pDX, IDS_LOAD_CONVEYOR_MOTOR_REDUCED_FREQ, m_lblLoadConveyorMotorReducedFreq);
	DDX_Control(pDX, IDS_UNLOAD_CONVEYOR_NORMAL_SPEED, m_lblUnloadConveyorNormalSpeed);
	DDX_Control(pDX, IDS_UNLOAD_CONVEYOR_REDUCED_SPEED, m_lblUnloadConveyorReducedSpeed);
	DDX_Control(pDX, IDS_UNLOAD_CONVEYOR_MOTOR_NORMAL_FREQ, m_lblUnloadConveyorMotorNormalFreq);
	DDX_Control(pDX, IDS_UNLOAD_CONVEYOR_MOTOR_REDUCED_FREQ, m_lblUnloadConveyorMotorReducedFreq);
	DDX_Control(pDX, IDS_LOAD_CONVEYOR_TRANSFER_SPEED, m_lblLoadConveyorTransferSpeed);
	DDX_Control(pDX, IDS_LOAD_CONVEYOR_MOTOR_TRANSFER_FREQ, m_lblLoadConveyorMotorTransferFreq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataConveyorSpeedDlg, CDialog)
	//{{AFX_MSG_MAP(CDataConveyorSpeedDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataConveyorSpeedDlg message handlers

BEGIN_EVENTSINK_MAP(CDataConveyorSpeedDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataConveyorSpeedDlg)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_NORMAL_SPEED, -600 /* Click */, OnClickLoadConveyorNormalSpeed, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_REDUCED_SPEED, -600 /* Click */, OnClickLoadConveyorReducedSpeed, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_UNLOAD_CONVEYOR_NORMAL_SPEED, -600 /* Click */, OnClickUnloadConveyorNormalSpeed, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_UNLOAD_CONVEYOR_REDUCED_SPEED, -600 /* Click */, OnClickUnloadConveyorReducedSpeed, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_NORMAL_SPEED_MOVE_FORWARD, -600 /* Click */, OnClickLoadConveyorNormalSpeedMoveForward, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_NORMAL_SPEED_MOVE_BACKWARD, -600 /* Click */, OnClickLoadConveyorNormalSpeedMoveBackward, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_REDUCED_SPEED_MOVE_FORWARD, -600 /* Click */, OnClickLoadConveyorReducedSpeedMoveForward, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_REDUCED_SPEED_MOVE_BACKWARD, -600 /* Click */, OnClickLoadConveyorReducedSpeedMoveBackward, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_STOP, -600 /* Click */, OnClickLoadConveyorStop, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_UNLOAD_CONVEYOR_NORMAL_SPEED_MOVE_FORWARD, -600 /* Click */, OnClickUnloadConveyorNormalSpeedMoveForward, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_UNLOAD_CONVEYOR_NORMAL_SPEED_MOVE_BACKWARD, -600 /* Click */, OnClickUnloadConveyorNormalSpeedMoveBackward, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_UNLOAD_CONVEYOR_REDUCED_SPEED_MOVE_FORWARD, -600 /* Click */, OnClickUnloadConveyorReducedSpeedMoveForward, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_UNLOAD_CONVEYOR_REDUCED_SPEED_MOVE_BACKWARD, -600 /* Click */, OnClickUnloadConveyorReducedSpeedMoveBackward, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_UNLOAD_CONVEYOR_STOP, -600 /* Click */, OnClickUnloadConveyorStop, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_TRANSFER_SPEED, -600 /* Click */, OnClickLoadConveyorTransferSpeed, VTS_NONE)
	ON_EVENT(CDataConveyorSpeedDlg, IDB_LOAD_CONVEYOR_TRANSFER_SPEED_MOVE_FORWARD, -600 /* Click */, OnClickLoadConveyorTransferSpeedMoveForward, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CDataConveyorSpeedDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dLoadConveyorNormalSpeed = m_plnkPreBonderData->m_rgdConveyorNormalSpeed[DEF_LOAD_CONVEYOR];
	m_dLoadConveyorReducedSpeed = m_plnkPreBonderData->m_rgdConveyorReducedSpeed[DEF_LOAD_CONVEYOR];
	m_dUnloadConveyorNormalSpeed = m_plnkPreBonderData->m_rgdConveyorNormalSpeed[DEF_UNLOAD_CONVEYOR];
	m_dUnloadConveyorReducedSpeed = m_plnkPreBonderData->m_rgdConveyorReducedSpeed[DEF_UNLOAD_CONVEYOR];

	m_dLoadConveyorTransferSpeed = m_plnkPreBonderData->m_rgdConveyorTransferSpeed[DEF_LOAD_CONVEYOR];

	UpdateDisplay();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataConveyorSpeedDlg_1"), &szTextTemp);
	// 저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataConveyorSpeedDlg_2"), &szTextTemp);
	// 나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataConveyorSpeedDlg::UpdateDisplay()
{
	// Load Conveyor Normal
	// 속도값 -> 주파수
	m_dLoadConveyorMotorNormalFreq = m_plnkLoadConveyorInverter->CalculateSpeed2Freq(m_dLoadConveyorNormalSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dLoadConveyorMotorNormalFreq);
	m_dLoadConveyorMotorNormalFreq = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorMotorNormalFreq.SetCaption(m_szNewValue);

	// 주파수 -> 적용속도값
	m_dLoadConveyorNormalSpeed = m_plnkLoadConveyorInverter->CalculateFreq2Speed(m_dLoadConveyorMotorNormalFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorNormalSpeed);
	m_dLoadConveyorNormalSpeed = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorNormalSpeed.SetCaption(m_szNewValue);


	// Load Conveyor Reduced
	// 속도값 -> 주파수
	m_dLoadConveyorMotorReducedFreq = m_plnkLoadConveyorInverter->CalculateSpeed2Freq(m_dLoadConveyorReducedSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dLoadConveyorMotorReducedFreq);
	m_dLoadConveyorMotorReducedFreq = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorMotorReducedFreq.SetCaption(m_szNewValue);

	// 주파수 -> 적용속도값
	m_dLoadConveyorReducedSpeed = m_plnkLoadConveyorInverter->CalculateFreq2Speed(m_dLoadConveyorMotorReducedFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorReducedSpeed);
	m_dLoadConveyorReducedSpeed = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorReducedSpeed.SetCaption(m_szNewValue);


	// Unload Conveyor Normal
	// 속도값 -> 주파수
	m_dUnloadConveyorMotorNormalFreq = m_plnkUnloadConveyorInverter->CalculateSpeed2Freq(m_dUnloadConveyorNormalSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dUnloadConveyorMotorNormalFreq);
	m_dUnloadConveyorMotorNormalFreq = wcstod(m_szNewValue, NULL);

	m_lblUnloadConveyorMotorNormalFreq.SetCaption(m_szNewValue);

	// 주파수 -> 적용속도값
	m_dUnloadConveyorNormalSpeed = m_plnkUnloadConveyorInverter->CalculateFreq2Speed(m_dUnloadConveyorMotorNormalFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dUnloadConveyorNormalSpeed);
	m_dUnloadConveyorNormalSpeed = wcstod(m_szNewValue, NULL);

	m_lblUnloadConveyorNormalSpeed.SetCaption(m_szNewValue);

	
	// Unload Conveyor Reduced
	// 속도값 -> 주파수
	m_dUnloadConveyorMotorReducedFreq = m_plnkUnloadConveyorInverter->CalculateSpeed2Freq(m_dUnloadConveyorReducedSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dUnloadConveyorMotorReducedFreq);
	m_dUnloadConveyorMotorReducedFreq = wcstod(m_szNewValue, NULL);

	m_lblUnloadConveyorMotorReducedFreq.SetCaption(m_szNewValue);

	// 주파수 -> 적용속도값
	m_dUnloadConveyorReducedSpeed = m_plnkUnloadConveyorInverter->CalculateFreq2Speed(m_dUnloadConveyorMotorReducedFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dUnloadConveyorReducedSpeed);
	m_dUnloadConveyorReducedSpeed = wcstod(m_szNewValue, NULL);

	m_lblUnloadConveyorReducedSpeed.SetCaption(m_szNewValue);


	// Load Conveyor Transfer
	// 속도값 -> 주파수
	m_dLoadConveyorMotorTransferFreq = m_plnkLoadConveyorInverter->CalculateSpeed2Freq(m_dLoadConveyorTransferSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dLoadConveyorMotorTransferFreq);
	m_dLoadConveyorMotorTransferFreq = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorMotorTransferFreq.SetCaption(m_szNewValue);

	// 주파수 -> 적용속도값
	m_dLoadConveyorTransferSpeed = m_plnkLoadConveyorInverter->CalculateFreq2Speed(m_dLoadConveyorMotorTransferFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorTransferSpeed);
	m_dLoadConveyorTransferSpeed = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorTransferSpeed.SetCaption(m_szNewValue);
}

void CDataConveyorSpeedDlg::OnClickSave()
{
//	변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40200, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkPreBonderData->m_rgdConveyorNormalSpeed[DEF_LOAD_CONVEYOR]	= m_dLoadConveyorNormalSpeed;
	m_plnkPreBonderData->m_rgdConveyorReducedSpeed[DEF_LOAD_CONVEYOR]	= m_dLoadConveyorReducedSpeed;
	m_plnkPreBonderData->m_rgdConveyorNormalSpeed[DEF_UNLOAD_CONVEYOR]	= m_dUnloadConveyorNormalSpeed;
	m_plnkPreBonderData->m_rgdConveyorReducedSpeed[DEF_UNLOAD_CONVEYOR]	= m_dUnloadConveyorReducedSpeed;

	m_plnkPreBonderData->m_rgdConveyorTransferSpeed[DEF_LOAD_CONVEYOR]	= m_dLoadConveyorTransferSpeed;

	m_plnkPreBonderData->WriteData();
}

void CDataConveyorSpeedDlg::OnClickExit()
{
	// TODO: Add your control notification handler code here

	CDialog::OnCancel();
}


void CDataConveyorSpeedDlg::OnClickLoadConveyorNormalSpeed()
{
	// TODO: Add your control notification handler code here
	m_lblLoadConveyorNormalSpeed.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dLoadConveyorNormalSpeed = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorNormalSpeed);
	m_dLoadConveyorNormalSpeed = wcstod(m_szNewValue, NULL);

	// 속도값 -> 주파수
	m_dLoadConveyorMotorNormalFreq = m_plnkLoadConveyorInverter->CalculateSpeed2Freq(m_dLoadConveyorNormalSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dLoadConveyorMotorNormalFreq);
	m_dLoadConveyorMotorNormalFreq = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorMotorNormalFreq.SetCaption(m_szNewValue);
	
	// 주파수 -> 적용속도값
	m_dLoadConveyorNormalSpeed = m_plnkLoadConveyorInverter->CalculateFreq2Speed(m_dLoadConveyorMotorNormalFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorNormalSpeed);
	m_dLoadConveyorNormalSpeed = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorNormalSpeed.SetCaption(m_szNewValue);
}

void CDataConveyorSpeedDlg::OnClickLoadConveyorReducedSpeed()
{
	// TODO: Add your control notification handler code here
	m_lblLoadConveyorReducedSpeed.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dLoadConveyorReducedSpeed = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorReducedSpeed);
	m_dLoadConveyorReducedSpeed = wcstod(m_szNewValue, NULL);

	// 속도값 -> 주파수
	m_dLoadConveyorMotorReducedFreq = m_plnkLoadConveyorInverter->CalculateSpeed2Freq(m_dLoadConveyorReducedSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dLoadConveyorMotorReducedFreq);
	m_dLoadConveyorMotorReducedFreq = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorMotorReducedFreq.SetCaption(m_szNewValue);
	
	// 주파수 -> 적용속도값
	m_dLoadConveyorReducedSpeed = m_plnkLoadConveyorInverter->CalculateFreq2Speed(m_dLoadConveyorMotorReducedFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorReducedSpeed);
	m_dLoadConveyorReducedSpeed = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorReducedSpeed.SetCaption(m_szNewValue);
}

void CDataConveyorSpeedDlg::OnClickUnloadConveyorNormalSpeed()
{
	// TODO: Add your control notification handler code here
	m_lblUnloadConveyorNormalSpeed.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dUnloadConveyorNormalSpeed = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dUnloadConveyorNormalSpeed);
	m_dUnloadConveyorNormalSpeed = wcstod(m_szNewValue, NULL);

	// 속도값 -> 주파수
	m_dUnloadConveyorMotorNormalFreq = m_plnkUnloadConveyorInverter->CalculateSpeed2Freq(m_dUnloadConveyorNormalSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dUnloadConveyorMotorNormalFreq);
	m_dUnloadConveyorMotorNormalFreq = wcstod(m_szNewValue, NULL);

	m_lblUnloadConveyorMotorNormalFreq.SetCaption(m_szNewValue);
	
	// 주파수 -> 적용속도값
	m_dUnloadConveyorNormalSpeed = m_plnkUnloadConveyorInverter->CalculateFreq2Speed(m_dUnloadConveyorMotorNormalFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dUnloadConveyorNormalSpeed);
	m_dUnloadConveyorNormalSpeed = wcstod(m_szNewValue, NULL);

	m_lblUnloadConveyorNormalSpeed.SetCaption(m_szNewValue);
}

void CDataConveyorSpeedDlg::OnClickUnloadConveyorReducedSpeed()
{
	// TODO: Add your control notification handler code here
	m_lblUnloadConveyorReducedSpeed.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dUnloadConveyorReducedSpeed = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dUnloadConveyorReducedSpeed);
	m_dUnloadConveyorReducedSpeed = wcstod(m_szNewValue, NULL);

	// 속도값 -> 주파수
	m_dUnloadConveyorMotorReducedFreq = m_plnkUnloadConveyorInverter->CalculateSpeed2Freq(m_dUnloadConveyorReducedSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dUnloadConveyorMotorReducedFreq);
	m_dUnloadConveyorMotorReducedFreq = wcstod(m_szNewValue, NULL);

	m_lblUnloadConveyorMotorReducedFreq.SetCaption(m_szNewValue);
	
	// 주파수 -> 적용속도값
	m_dUnloadConveyorReducedSpeed = m_plnkUnloadConveyorInverter->CalculateFreq2Speed(m_dUnloadConveyorMotorReducedFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dUnloadConveyorReducedSpeed);
	m_dUnloadConveyorReducedSpeed = wcstod(m_szNewValue, NULL);

	m_lblUnloadConveyorReducedSpeed.SetCaption(m_szNewValue);
}

void CDataConveyorSpeedDlg::OnClickLoadConveyorNormalSpeedMoveForward() 
{
	int iResult = m_plnkLoadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_FORWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Load Conveyor를 정상 속도로 정회전 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40201, _T("Load Conveyor Moves to Forward in the Normal Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Load Conveyor Moves to Forward in the Normal Speed..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Load Conveyor Moves to Forward in the Normal Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkLoadConveyorInverter->SetSpeed((unsigned short)(m_dLoadConveyorMotorNormalFreq*10), DEF_INVERTER_NORMAL_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkLoadConveyorInverter->ForwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkLoadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkLoadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnClickLoadConveyorNormalSpeedMoveBackward() 
{
	int iResult = m_plnkLoadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_BACKWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Load Conveyor를 정상 속도로 역회전 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40202, _T("Load Conveyor Moves to Forward in the Normal Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Load Conveyor Moves to Backward in the Normal Speed..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("Load Conveyor Moves to Backward in the Normal Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkLoadConveyorInverter->SetSpeed((unsigned short)(m_dLoadConveyorMotorNormalFreq*10), DEF_INVERTER_NORMAL_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkLoadConveyorInverter->BackwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkLoadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkLoadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnClickLoadConveyorReducedSpeedMoveForward() 
{
	int iResult = m_plnkLoadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_FORWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Load Conveyor를 느린 속도로 정방향으로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40203, _T("Load Conveyor Moves to Forward in the Reduced Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Load Conveyor Moves to Forward in the Reduced Speed..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Load Conveyor Moves to Forward in the Reduced Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkLoadConveyorInverter->SetSpeed((unsigned short)(m_dLoadConveyorMotorReducedFreq*10), DEF_INVERTER_REDUCED_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkLoadConveyorInverter->ForwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkLoadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkLoadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnClickLoadConveyorReducedSpeedMoveBackward() 
{
	int iResult = m_plnkLoadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_BACKWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Backward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Load Conveyor를 느린 속도로 정회전 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40204, _T("Load Conveyor Moves to Backward in the Reduced Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Load Conveyor Moves to Backward in the Reduced Speed..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Load Conveyor Moves to Backward in the Reduced Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkLoadConveyorInverter->SetSpeed((unsigned short)(m_dLoadConveyorMotorReducedFreq*10), DEF_INVERTER_REDUCED_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Backward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkLoadConveyorInverter->BackwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Backward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkLoadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkLoadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Backward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnClickLoadConveyorStop() 
{
	int iResult = m_plnkLoadConveyorInverter->Stop();
	if (iResult)
	{
		m_plnkLoadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Stop"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Load Conveyor가 정지했습니다.
	MyMessageBox(MY_MESSAGE_BOX, 40210, _T("Load Conveyor Stop"), M_ICONINFORMATION);
}

void CDataConveyorSpeedDlg::OnClickUnloadConveyorNormalSpeedMoveForward() 
{
	int iResult = m_plnkUnloadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_FORWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Unload Conveyor를 정상 속도로 정회전 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40205, _T("Unload Conveyor Moves to Forward in the Normal Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Unload Conveyor Moves to Forward in the Normal Speed..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Unload Conveyor Moves to Forward in the Normal Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkUnloadConveyorInverter->SetSpeed((unsigned short)(m_dUnloadConveyorMotorNormalFreq*10), DEF_INVERTER_NORMAL_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkUnloadConveyorInverter->ForwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkUnloadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkUnloadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnClickUnloadConveyorNormalSpeedMoveBackward() 
{
	int iResult = m_plnkUnloadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_BACKWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Unload Conveyor를 정상 속도로 역회전 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40206, _T("Unload Conveyor Moves to Backward in the Normal Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Unload Conveyor Moves to Backward in the Normal Speed..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Unload Conveyor Moves to Backward in the Normal Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkUnloadConveyorInverter->SetSpeed((unsigned short)(m_dUnloadConveyorMotorNormalFreq*10), DEF_INVERTER_NORMAL_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkUnloadConveyorInverter->BackwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkUnloadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkUnloadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnClickUnloadConveyorReducedSpeedMoveForward() 
{
	int iResult = m_plnkUnloadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_FORWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Unload Conveyor를 느린 속도로 정회전 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40207, _T("Unload Conveyor Moves to Forward in the Reduced Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Unload Conveyor Moves to Forward in the Reduced Speed..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Unload Conveyor Moves to Forward in the Reduced Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkUnloadConveyorInverter->SetSpeed((unsigned short)(m_dUnloadConveyorMotorReducedFreq*10), DEF_INVERTER_REDUCED_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Forward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkUnloadConveyorInverter->ForwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Forward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkUnloadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkUnloadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Forward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnClickUnloadConveyorReducedSpeedMoveBackward() 
{
	int iResult = m_plnkUnloadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_BACKWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Unload Conveyor를 느린 속도로 역회전 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40208, _T("Unload Conveyor Moves to Backward in the Reduced Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Unload Conveyor Moves to Backward in the Reduced Speed..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Unload Conveyor Moves to Backward in the Reduced Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkUnloadConveyorInverter->SetSpeed((unsigned short)(m_dUnloadConveyorMotorReducedFreq*10), DEF_INVERTER_REDUCED_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Backward in the Normal Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkUnloadConveyorInverter->BackwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Backward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkUnloadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkUnloadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Moves to Backward in the Reduced Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnClickUnloadConveyorStop() 
{
	int iResult = m_plnkUnloadConveyorInverter->Stop();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Unload Conveyor Stop"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Unload Conveyor가 정지했습니다.
	MyMessageBox(MY_MESSAGE_BOX, 40211, _T("Unload Conveyor Stop"), M_ICONINFORMATION);
}

void CDataConveyorSpeedDlg::OnClickLoadConveyorTransferSpeed() 
{
	// TODO: Add your control notification handler code here
	m_lblLoadConveyorTransferSpeed.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dLoadConveyorTransferSpeed = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorTransferSpeed);
	m_dLoadConveyorTransferSpeed = wcstod(m_szNewValue, NULL);

	// 속도값 -> 주파수
	m_dLoadConveyorMotorTransferFreq = m_plnkLoadConveyorInverter->CalculateSpeed2Freq(m_dLoadConveyorTransferSpeed);
	swprintf(m_szNewValue, _T("%.1f"), m_dLoadConveyorMotorTransferFreq);
	m_dLoadConveyorMotorTransferFreq = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorMotorTransferFreq.SetCaption(m_szNewValue);
	
	// 주파수 -> 적용속도값
	m_dLoadConveyorTransferSpeed = m_plnkLoadConveyorInverter->CalculateFreq2Speed(m_dLoadConveyorMotorTransferFreq);
	swprintf(m_szNewValue, _T("%.3f"), m_dLoadConveyorTransferSpeed);
	m_dLoadConveyorTransferSpeed = wcstod(m_szNewValue, NULL);

	m_lblLoadConveyorTransferSpeed.SetCaption(m_szNewValue);
}

void CDataConveyorSpeedDlg::OnClickLoadConveyorTransferSpeedMoveForward() 
{
	int iResult = m_plnkLoadConveyor->checkInterlockForMove(DEF_CONVEYOR_MOVE_FORWARD, FALSE);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward int the Transfer Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

//	Load Conveyor를 Panel 전달 속도로 정회전 시키시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40209, _T("Load Conveyor Moves to Forward int the Transfer Speed"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Load Conveyor Moves to Forward int the Transfer Speed..."));
	ProgWnd.SetRange(0, 3);
	ProgWnd.SetText(_T("Load Conveyor Moves to Forward int the Transfer Speed...Wait a moment..."));
	ProgWnd.PeekAndPump();

	//속도 설정
	iResult = m_plnkLoadConveyorInverter->SetSpeed((unsigned short)(m_dLoadConveyorMotorTransferFreq*10), DEF_INVERTER_TRANSFER_SPEED);
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward int the Transfer Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	//방향 설정
	iResult = m_plnkLoadConveyorInverter->ForwardDirection();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward int the Transfer Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	ProgWnd.StepIt();

	iResult = m_plnkLoadConveyorInverter->Start();
	if (iResult)
	{
		m_plnkLoadConveyorInverter->Stop();
// jdy sesl		MyMessageBox(iResult, _T("Load Conveyor Moves to Forward int the Transfer Speed"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CDataConveyorSpeedDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}
