// DataModelChangeDistDlg.cpp : implementation file

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "DataModelChangeDistDlg.h"
#include "MPanelAligner.h"
#include "MModelChange.h"
#include "MTabMounter.h"
#include "MCameraCarrier.h"
#include "MPanelData.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataModelChangeDistDlg dialog

CDataModelChangeDistDlg::CDataModelChangeDistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataModelChangeDistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataModelChangeDistDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkPanelData = MOlbSystemPre.GetPanelData();
}


void CDataModelChangeDistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataModelChangeDistDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDS_SYSTEM0, m_lblDistParameter[0]);
	DDX_Control(pDX, IDS_SYSTEM1, m_lblDistParameter[1]);
	DDX_Control(pDX, IDS_SYSTEM2, m_lblDistParameter[2]);
	DDX_Control(pDX, IDS_SYSTEM3, m_lblDistParameter[3]);
	DDX_Control(pDX, IDS_SYSTEM4, m_lblDistParameter[4]);
	DDX_Control(pDX, IDS_SYSTEM5, m_lblDistParameter[5]);
	DDX_Control(pDX, IDS_SYSTEM6, m_lblDistParameter[6]);
	DDX_Control(pDX, IDS_SYSTEM7, m_lblDistParameter[7]);
	DDX_Control(pDX, IDS_SYSTEM8, m_lblDistParameter[8]);
	DDX_Control(pDX, IDS_SYSTEM9, m_lblDistParameter[9]);
	DDX_Control(pDX, IDS_SYSTEM10, m_lblDistParameter[10]);
	DDX_Control(pDX, IDS_SYSTEM11, m_lblDistParameter[11]);
	DDX_Control(pDX, IDS_SYSTEM12, m_lblDistParameter[12]);
	DDX_Control(pDX, IDS_SYSTEM13, m_lblDistParameter[13]);
	DDX_Control(pDX, IDS_SYSTEM14, m_lblDistParameter[14]);
}


BEGIN_MESSAGE_MAP(CDataModelChangeDistDlg, CDialog)
	//{{AFX_MSG_MAP(CDataModelChangeDistDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataModelChangeDistDlg message handlers

BEGIN_EVENTSINK_MAP(CDataModelChangeDistDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataModelChangeDistDlg)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM0, -600 /* Click */, OnModelChangeDist0, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM1, -600 /* Click */, OnModelChangeDist1, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM2, -600 /* Click */, OnModelChangeDist2, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM4, -600 /* Click */, OnClickSystem4, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM5, -600 /* Click */, OnClickSystem5, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM6, -600 /* Click */, OnClickSystem6, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM7, -600 /* Click */, OnClickSystem7, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM8, -600 /* Click */, OnClickSystem8, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM9, -600 /* Click */, OnClickSystem9, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM10, -600 /* Click */, OnClickSystem10, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM11, -600 /* Click */, OnClickSystem11, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM12, -600 /* Click */, OnClickSystem12, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM13, -600 /* Click */, OnClickSystem13, VTS_NONE)
	ON_EVENT(CDataModelChangeDistDlg, IDB_SYSTEM14, -600 /* Click */, OnClickSystem14, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataModelChangeDistDlg::OnSave() 
{
//  변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40500, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
/*
	m_plnkSystemData->m_dModelChangeOriginDist		= m_dData[0];
	m_plnkSystemData->m_dOriginDistBetweenAligners	= m_dData[1];
	m_plnkSystemData->m_dDistBetweenRGantryHomeToModelChangeHome = m_dData[2];
*/
	m_plnkSystemData->m_dAlignCam1_FixedPos_X = m_dData[0];
	m_plnkSystemData->m_dAlignCam1_FixedPos_Y = m_dData[1];
	m_plnkSystemData->m_dDistBetweenAligners = m_dData[3];
	m_plnkSystemData->m_dDistPosInspectCamera[0] = m_dData[4];
	m_plnkSystemData->m_dDistNegInspectCamera[0] = m_dData[5];

//@#ifdef DEF_GATE_SYSTEM
	m_plnkSystemData->m_dAlignCam2_FixedPos_X = m_dData[2];
	m_plnkSystemData->m_dAlignCam2_FixedPos_Y = m_dData[6];
	m_plnkSystemData->m_dGantryOriginDist[0] = m_dData[7];
	m_plnkSystemData->m_dGantryOriginDist[1] = m_dData[8];
	m_plnkSystemData->m_dDistPosInspectCamera[1] = m_dData[9];
	m_plnkSystemData->m_dDistNegInspectCamera[1] = m_dData[10];
//@#endif

	m_plnkSystemData->m_dDistBadTabOriginDist[0] = m_dData[11];
	m_plnkSystemData->m_dDistBadTabOriginDist[1] = m_dData[12];

	m_plnkSystemData->m_dCamCarrier1_FixedPosX = m_dData[13];
	m_plnkSystemData->m_dCamCarrier2_FixedPosX = m_dData[14];

	m_plnkSystemData->WriteData();

	// Model Chagne 수행 함수 호출
	MOlbSystemPre.GetPanelAlignerComponent()->ChangeModel();
//@#ifdef DEF_GATE_SYSTEM
	MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->ChangeModel();
	MOlbSystemPre.GetModelChangeComponent(DEF_REAR_GROUP)->ChangeModel();
//@#endif

	int i = 0;
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		MOlbSystemPre.GetTabMounterComponent(i)->ChangeModel();
//@#ifdef DEF_GATE_SYSTEM
		MOlbSystemPre.GetRTabMounterComponent(i)->ChangeModel();
//@#endif
	}

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		//MOlbSystemPre.GetCameraCarrierComponent(i)->SetFixedOffsetBySystemData();
		MOlbSystemPre.GetCameraCarrierComponent(i)->ChangeModel();
//@#ifdef DEF_GATE_SYSTEM
		MOlbSystemPre.GetRCameraCarrierComponent(i)->ChangeModel();
//@#endif
	}

	MOlbSystemPre.AssignWorkScheduleData();

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		MOlbSystemPre.GetWorkSchedule(i)->ChangeModel();
#ifdef DEF_SOURCE_SYSTEM
		MOlbSystemPre.GetWorkScheduleSecond(i)->ChangeModel();
#endif
	}

//	if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
//		MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[DEF_REAR_GROUP])->ChangeModel();
}

void CDataModelChangeDistDlg::OnExit()
{
	OnOK();
}

BOOL CDataModelChangeDistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/*
	m_dData[0] = m_plnkSystemData->m_dModelChangeOriginDist;
	m_dData[1] = m_plnkSystemData->m_dOriginDistBetweenAligners;
	m_dData[2] = m_plnkSystemData->m_dDistBetweenRGantryHomeToModelChangeHome;
//KKY.Del	m_dData[3] = m_dData[1] + m_plnkPanelData->m_dDistance_FiduMark_Xu
//KKY.Del		- m_dData[0] + MOlbSystemPre.GetModelChangeComponent()->GetModelPosOffset();
	m_dData[3] = 0.0;
	*/
	m_dData[0] = m_plnkSystemData->m_dAlignCam1_FixedPos_X;
	m_dData[1] = m_plnkSystemData->m_dAlignCam1_FixedPos_Y;
	m_dData[3] = m_plnkSystemData->m_dDistBetweenAligners;
	m_dData[4] = m_plnkSystemData->m_dDistPosInspectCamera[0];
	m_dData[5] = m_plnkSystemData->m_dDistNegInspectCamera[0];

//@#ifdef DEF_GATE_SYSTEM
	m_dData[2] = m_plnkSystemData->m_dAlignCam2_FixedPos_X;
	m_dData[6] = m_plnkSystemData->m_dAlignCam2_FixedPos_Y;
	m_dData[7] = m_plnkSystemData->m_dGantryOriginDist[0];
	m_dData[8] = m_plnkSystemData->m_dGantryOriginDist[1];
	m_dData[9] = m_plnkSystemData->m_dDistPosInspectCamera[1];
	m_dData[10] = m_plnkSystemData->m_dDistNegInspectCamera[1];
//@#endif

	m_dData[11] = m_plnkSystemData->m_dDistBadTabOriginDist[0];
	m_dData[12] = m_plnkSystemData->m_dDistBadTabOriginDist[1];

	m_dData[13] = m_plnkSystemData->m_dCamCarrier1_FixedPosX;
	m_dData[14] = m_plnkSystemData->m_dCamCarrier2_FixedPosX;

	updateDisplay();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_1"), &szTextTemp);
	//Stage중심에서 Align1 Cam중심거리 X
	GetDlgItem(IDB_SYSTEM0)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_2"), &szTextTemp);
	//Stage중심에서 Align1 Cam중심거리 Y
	GetDlgItem(IDB_SYSTEM1)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_3"), &szTextTemp);
	//Stage중심에서 Align2Cam중심 거리 X
	GetDlgItem(IDB_SYSTEM2)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_4"), &szTextTemp);
	//Stage중심에서 Align2Cam중심 거리 Y
	GetDlgItem(IDB_SYSTEM6)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_5"), &szTextTemp);
	//Stage중심 Pad간 거리
	GetDlgItem(IDB_SYSTEM3)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_6"), &szTextTemp);
	//BadTab Origin 거리(Front)
	GetDlgItem(IDB_SYSTEM11)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_7"), &szTextTemp);
	//BadTab Origin 거리(Rear)
	GetDlgItem(IDB_SYSTEM12)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_8"), &szTextTemp);
	//F Inspect Camera 고정좌표 X
	GetDlgItem(IDB_SYSTEM13)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_9"), &szTextTemp);
	//Gantry원점에서 PanelAligner원점간 거리(Front)
	GetDlgItem(IDB_SYSTEM7)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_10"), &szTextTemp);
	//Gantry원점에서 PanelAligner원점간 거리(Rear)
	GetDlgItem(IDB_SYSTEM8)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_11"), &szTextTemp);
	//Positive Inspect Camera 거리(Front)
	GetDlgItem(IDB_SYSTEM4)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_12"), &szTextTemp);
	//Negative Inspect Camera 거리(Front)
	GetDlgItem(IDB_SYSTEM5)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_13"), &szTextTemp);
	//Positive Inspect Camera 거리(Rear)
	GetDlgItem(IDB_SYSTEM9)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_14"), &szTextTemp);
	//Negative Inspect Camera거리(Rear)
	GetDlgItem(IDB_SYSTEM10)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_15"), &szTextTemp);
	//R Inspect Camera 고정좌표 X
	GetDlgItem(IDB_SYSTEM14)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_16"), &szTextTemp);
	//Stage중심에서 Align2Cam중심 거리 Y
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataModelChangeDistDlg_17"), &szTextTemp);
	//Stage중심에서 Align2Cam중심 거리 Y
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	//_______________

	/*/
#ifdef DEF_SOURCE_SYSTEM
	GetDlgItem(IDB_SYSTEM2)->EnableWindow(FALSE);
	GetDlgItem(IDS_SYSTEM2)->EnableWindow(FALSE);
	GetDlgItem(IDB_SYSTEM6)->EnableWindow(FALSE);
	GetDlgItem(IDS_SYSTEM6)->EnableWindow(FALSE);
	GetDlgItem(IDB_SYSTEM7)->EnableWindow(FALSE);
	GetDlgItem(IDS_SYSTEM7)->EnableWindow(FALSE);
	GetDlgItem(IDB_SYSTEM8)->EnableWindow(FALSE);
	GetDlgItem(IDS_SYSTEM8)->EnableWindow(FALSE);
	GetDlgItem(IDB_SYSTEM9)->EnableWindow(FALSE);
	GetDlgItem(IDS_SYSTEM9)->EnableWindow(FALSE);
	GetDlgItem(IDB_SYSTEM10)->EnableWindow(FALSE);
	GetDlgItem(IDS_SYSTEM10)->EnableWindow(FALSE);
#endif
	/*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataModelChangeDistDlg::updateDisplay()
{
	WCHAR szValue[DEF_MODEL_CHANGE_DIST_MAX_LABEL][100];

	for (int i = 0; i < DEF_MODEL_CHANGE_DIST_MAX_LABEL; i++)
	{
		swprintf(szValue[i], _T("%.4f"), m_dData[i]);
		m_lblDistParameter[i].SetCaption(szValue[i]);
	}
}

void CDataModelChangeDistDlg::getNewValue(int iIndex)
{
	m_lblDistParameter[iIndex].GetWindowText(m_szOldValue, 10);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))
		return;

	// 자리수 조정
	m_dData[iIndex] = wcstod(m_szNewValue, NULL);

//	if (m_dData[iIndex] < 0.0)
//		m_dData[iIndex] = 0.0;
	
	swprintf(m_szNewValue, _T("%.4f"), m_dData[iIndex]);
	m_dData[iIndex] = wcstod(m_szNewValue, NULL);
	
	m_lblDistParameter[iIndex].SetCaption(m_szNewValue);	
}

void CDataModelChangeDistDlg::OnModelChangeDist0() 
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(0);
}

void CDataModelChangeDistDlg::OnModelChangeDist1() 
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(1);
}

void CDataModelChangeDistDlg::OnModelChangeDist2() 
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(2);
}

void CDataModelChangeDistDlg::OnClickSystem4() 
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(4);

}

void CDataModelChangeDistDlg::OnClickSystem5() 
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(5);

}

void CDataModelChangeDistDlg::OnClickSystem6() 
{
	// TODO: Add your control notification handler code here
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(6);	
}

void CDataModelChangeDistDlg::OnClickSystem7() 
{
	// TODO: Add your control notification handler code here
//	Model Change 축 원점 거리는 구해져 있습니까?
	
#ifdef SIMULATION
	m_lblDistParameter[7].GetWindowText(m_szOldValue, 10);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))
		return;

	// 자리수 조정
	m_dData[7] = wcstod(m_szNewValue, NULL);

//	if (m_dData[iIndex] < 0.0)
//		m_dData[iIndex] = 0.0;
	
	swprintf(m_szNewValue, _T("%.4f"), m_dData[7]);
	m_dData[7] = wcstod(m_szNewValue, NULL);
	
	m_lblDistParameter[7].SetCaption(m_szNewValue);
	return;
#endif

	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________

	int iResult = MyMessageBox(MY_MESSAGE_BOX, 40505, _T("Question"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
		return;
	MTabMounter* plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER1);
	MModelChange* plnkModelChange = MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP);

	iResult = plnkTabMounter->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = plnkModelChange->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	/*
//	Rear Gantry 축의 현재 위치가 현재 작업 Model 에 대한 Mount 위치가 맞습니까?"
	iResult = MyMessageBox(MY_MESSAGE_BOX, 40506, _T("Question"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
	{
//		Rear Gantry 축 원점과 Model Change 축 원점 간의 거리를 설정할 수 없습니다."
		MyMessageBox(MY_MESSAGE_BOX, 40507, _T("Error"), M_ICONERROR);
		return;
	}

	if (FALSE == plnkTabMounter->IsInGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS))
	{
//		Rear Gantry 축의 현재 위치가 Mount 위치가 아니라서, Rear Gantry 축 원점과 Model Change 축 원점 간의 거리를 설정할 수 없습니다.",
		MyMessageBox(MY_MESSAGE_BOX, 40508, _T("Error"), M_ICONERROR);
		return;
	}
	*/

	double dGantryCurrent = plnkTabMounter->GetCurrentGantryPos();
	double dModelCurrent = plnkModelChange->GetYCurrentPos();
	double dRefPos = m_plnkSystemData->m_dAlignCam1_FixedPos_Y;

	double dPos = dRefPos + dModelCurrent - dGantryCurrent;
	m_dData[7] = dPos;
	updateDisplay();
}

void CDataModelChangeDistDlg::OnClickSystem8() 
{
	// TODO: Add your control notification handler code here
//	Model Change 축 원점 거리는 구해져 있습니까?
	
#ifdef SIMULATION
	m_lblDistParameter[8].GetWindowText(m_szOldValue, 10);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))
		return;

	// 자리수 조정
	m_dData[8] = wcstod(m_szNewValue, NULL);

//	if (m_dData[iIndex] < 0.0)
//		m_dData[iIndex] = 0.0;
	
	swprintf(m_szNewValue, _T("%.4f"), m_dData[8]);
	m_dData[8] = wcstod(m_szNewValue, NULL);
	
	m_lblDistParameter[8].SetCaption(m_szNewValue);
	return;
#endif

	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________

	int iResult = MyMessageBox(MY_MESSAGE_BOX, 40505, _T("Question"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
		return;
//@	MTabMounter* plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER5);
	MTabMounter* plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(DEF_TABMOUNTER1);
	MModelChange* plnkModelChange = MOlbSystemPre.GetModelChangeComponent(DEF_REAR_GROUP);

	iResult = plnkTabMounter->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}
	iResult = plnkModelChange->CheckOrigin();
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return;
	}

	/*
//	Rear Gantry 축의 현재 위치가 현재 작업 Model 에 대한 Mount 위치가 맞습니까?"
	iResult = MyMessageBox(MY_MESSAGE_BOX, 40506, _T("Question"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
	{
//		Rear Gantry 축 원점과 Model Change 축 원점 간의 거리를 설정할 수 없습니다."
		MyMessageBox(MY_MESSAGE_BOX, 40507, _T("Error"), M_ICONERROR);
		return;
	}

	if (FALSE == plnkTabMounter->IsInGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS))
	{
//		Rear Gantry 축의 현재 위치가 Mount 위치가 아니라서, Rear Gantry 축 원점과 Model Change 축 원점 간의 거리를 설정할 수 없습니다.",
		MyMessageBox(MY_MESSAGE_BOX, 40508, _T("Error"), M_ICONERROR);
		return;
	}
	*/

	double dGantryCurrent = plnkTabMounter->GetCurrentGantryPos();
	double dModelCurrent = plnkModelChange->GetYCurrentPos();
	double dRefPos = m_plnkSystemData->m_dAlignCam2_FixedPos_Y;

	double dPos = dRefPos + dModelCurrent - dGantryCurrent;
	m_dData[8] = dPos;
	updateDisplay();
}

void CDataModelChangeDistDlg::OnClickSystem9() 
{
	// TODO: Add your control notification handler code here
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(9);	
}

void CDataModelChangeDistDlg::OnClickSystem10() 
{
	// TODO: Add your control notification handler code here
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(10);	
}

void CDataModelChangeDistDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDataModelChangeDistDlg::OnClickSystem11() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(11);	
}

void CDataModelChangeDistDlg::OnClickSystem12() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	getNewValue(12);	
}

void CDataModelChangeDistDlg::OnClickSystem13() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	getNewValue(13);
}

void CDataModelChangeDistDlg::OnClickSystem14() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	getNewValue(14);	
}
