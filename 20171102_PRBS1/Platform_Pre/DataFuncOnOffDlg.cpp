// DataFuncOnOffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataFuncOnOffDlg.h"
#include "MPlatformOlbSystem.h"
#include "SystemThreeBtnDlg.h"
#include "MPanelAligner.h"
//100807.ECID_____________
#include "MTrsAutoManager.h"
#include "MLCNetData.h"
#include "MPrebonderData.h"

//20111205 SJ_HJG
#include "MTabData.h"
#include "MTabOffsetData.h"
#include "MTabMounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataFuncOnOffDlg dialog

CDataFuncOnOffDlg::CDataFuncOnOffDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataFuncOnOffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataFuncOnOffDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
}

void CDataFuncOnOffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataFuncOnOffDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ON_OFF_VALUE_00,	m_sFuncOnOff[0]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_01,	m_sFuncOnOff[1]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_02,	m_sFuncOnOff[2]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_03,	m_sFuncOnOff[3]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_04,	m_sFuncOnOff[4]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_05,	m_sFuncOnOff[5]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_06,	m_sFuncOnOff[6]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_07,	m_sFuncOnOff[7]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_08,	m_sFuncOnOff[8]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_09,	m_sFuncOnOff[9]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_10,	m_sFuncOnOff[10]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_11,	m_sFuncOnOff[11]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_12,	m_sFuncOnOff[12]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_13,	m_sFuncOnOff[13]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_14,	m_sFuncOnOff[14]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_15,	m_sFuncOnOff[15]);
	//20081110 by kss
	DDX_Control(pDX, IDC_ON_OFF_VALUE_16,	m_sFuncOnOff[16]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_17,	m_sFuncOnOff[17]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_18,	m_sFuncOnOff[18]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_19,	m_sFuncOnOff[19]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_20,	m_sFuncOnOff[20]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_21,	m_sFuncOnOff[21]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_22,	m_sFuncOnOff[22]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_23,	m_sFuncOnOff[23]);

	DDX_Control(pDX, IDC_ON_OFF_VALUE_24,	m_sFuncOnOff[24]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_25,	m_sFuncOnOff[25]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_26,	m_sFuncOnOff[26]);

	DDX_Control(pDX, IDC_ON_OFF_VALUE_27,	m_sFuncOnOff[27]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_28,	m_sFuncOnOff[28]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_29,	m_sFuncOnOff[29]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_30,	m_sFuncOnOff[30]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_31,	m_sFuncOnOff[31]);
	DDX_Control(pDX, IDC_ON_OFF_VALUE_32,	m_sFuncOnOff[32]);
}


BEGIN_MESSAGE_MAP(CDataFuncOnOffDlg, CDialog)
	//{{AFX_MSG_MAP(CDataFuncOnOffDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataFuncOnOffDlg message handlers

BEGIN_EVENTSINK_MAP(CDataFuncOnOffDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataFuncOnOffDlg)
	ON_EVENT(CDataFuncOnOffDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_0, -600 /* Click */, OnClickOnOffButton0, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_1, -600 /* Click */, OnClickOnOffButton1, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_2, -600 /* Click */, OnClickOnOffButton2, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_3, -600 /* Click */, OnClickOnOffButton3, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_4, -600 /* Click */, OnClickOnOffButton4, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_5, -600 /* Click */, OnClickOnOffButton5, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_6, -600 /* Click */, OnClickOnOffButton6, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_7, -600 /* Click */, OnClickOnOffButton7, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_8, -600 /* Click */, OnClickOnOffButton8, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_9, -600 /* Click */, OnClickOnOffButton9, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_10, -600 /* Click */, OnClickOnOffButton10, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_11, -600 /* Click */, OnClickOnOffButton11, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_12, -600 /* Click */, OnClickOnOffButton12, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_13, -600 /* Click */, OnClickOnOffButton13, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_14, -600 /* Click */, OnClickOnOffButton14, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_15, -600 /* Click */, OnClickOnOffButton15, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_16, -600 /* Click */, OnClickOnOffButton16, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_17, -600 /* Click */, OnClickOnOffButton17, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_18, -600 /* Click */, OnClickOnOffButton18, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_19, -600 /* Click */, OnClickOnOffButton19, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_20, -600 /* Click */, OnClickOnOffButton20, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_21, -600 /* Click */, OnClickOnOffButton21, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_22, -600 /* Click */, OnClickOnOffButton22, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_23, -600 /* Click */, OnClickOnOffButton23, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_24, -600 /* Click */, OnClickOnOffButton24, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_25, -600 /* Click */, OnClickOnOffButton25, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_26, -600 /* Click */, OnClickOnOffButton26, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_27, -600 /* Click */, OnClickOnOffButton27, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_28, -600 /* Click */, OnClickOnOffButton28, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_29, -600 /* Click */, OnClickOnOffButton29, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_30, -600 /* Click */, OnClickOnOffButton30, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_31, -600 /* Click */, OnClickOnOffButton31, VTS_NONE)
	ON_EVENT(CDataFuncOnOffDlg, IDB_ON_OFF_BUTTON_32, -600 /* Click */, OnClickOnOffButton32, VTS_NONE) 
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CDataFuncOnOffDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_ItemMax = 0;
	for (int i = m_ItemMax; i < DEF_MAX_FUNC_NUMBER; i++)
		m_sFuncOnOff[i].EnableWindow(false);

	for (int i = 0; i < m_ItemMax; i++)
		UpdateDisplay(i);

	return true;

	memset(m_iFuncOnOff, 0, sizeof(m_iFuncOnOff));
	memset(m_iFuncOnOffOld, 0, sizeof(m_iFuncOnOffOld));

	m_iFuncOnOff[0] = m_plnkSystemData->m_bUseAutoSearchPanel;
//	m_iFuncOnOff[1] = m_plnkSystemData->m_bUsePlateVacuum;
	m_iFuncOnOff[1] = m_plnkSystemData->m_bUseInspectionTabBolb; //170612 JSH
	m_iFuncOnOff[2] = m_plnkSystemData->m_bUseTabReelAutoChange;	// 임시로 PanelMarkManualSearch 기능으로 사용.
	m_iFuncOnOff[3] = m_plnkSystemData->m_bUseMaterialPrealarm;
	m_iFuncOnOff[4] = m_plnkSystemData->m_bUseBuzzer;
//@	m_iFuncOnOff[5] = m_plnkSystemData->m_bUseTabPanelAlign;
	m_iFuncOnOff[5] = m_plnkPreBonderData->m_bUseTabPanelAlign;
	//m_iFuncOnOff[6] = m_plnkSystemData->m_bSafetySensor;
	m_iFuncOnOff[6] = m_plnkSystemData->m_bUse1stPanelAlign; //170718 JSH
	m_iFuncOnOff[7] = m_plnkSystemData->m_bUseHeaterAlarm;
//	m_iFuncOnOff[8] = m_plnkSystemData->m_bUseAutoSearchAreaInAfterInspection;
	m_iFuncOnOff[8] = m_plnkSystemData->m_bUseBondingAfterImgLog; //170927 JSH
	m_iFuncOnOff[9] = m_plnkSystemData->m_bUseInspectionAfterMountInAutoRun;
//	m_iFuncOnOff[10] = m_plnkSystemData->m_bUseTestStop;
	m_iFuncOnOff[10] = m_plnkSystemData->m_bUsePanelPusher;
	m_iFuncOnOff[11] = m_plnkSystemData->m_bUsePanelValidation;	

	m_iFuncOnOff[12] = m_plnkSystemData->m_bUsePanelAbsorbOffsetReflection;
//	m_iFuncOnOff[12] = 1;

//SESL_LKH_090711 : m_bSafetySensor와 통합 후 삭제
//	m_iFuncOnOff[13] = m_plnkSystemData->m_bDisplayDoorOpenError;	//reserved

	m_iFuncOnOff[13] = m_plnkSystemData->m_nUseFeeder;

	m_iFuncOnOff[14] = m_plnkSystemData->m_bUseVIPMode;
	m_iFuncOnOff[15] = m_plnkSystemData->m_bUseMoldBlow;
	//20081110 by kss
	m_iFuncOnOff[16] = m_plnkSystemData->m_bUsePanelPreAlign;
	m_iFuncOnOff[17] = m_plnkSystemData->m_bUseAutoModelChange;
	m_iFuncOnOff[18] = m_plnkSystemData->m_bUseMaterialDeviceID;
	m_iFuncOnOff[19] = m_plnkSystemData->m_bUseMoldValidation;
	// 100807. ECID _______________
	m_iFuncOnOff[20] = m_plnkSystemData->m_bUseECDataReport;
	m_bOldUseECDataReport = m_iFuncOnOff[20];
	//@130206.Add____________
	m_iFuncOnOff[21] = m_plnkSystemData->m_bUseTabRemoveAuto;
	//@______________________

	m_iFuncOnOff[22] = m_plnkSystemData->m_bUse1FeederAllMount;
	//SJ_YYK 110414 Add.. Aling Pass Mode...
	m_iFuncOnOff[23] = m_plnkSystemData->m_bUseAlignPass;

	m_iFuncOnOff[24] = m_plnkSystemData->m_bUseSyncMode;
	m_iFuncOnOff[25] = m_plnkSystemData->m_bUseProductPlanValidation;
	m_iFuncOnOff[26] = m_plnkSystemData->m_bUseTXDataCheckMode;

	m_iFuncOnOff[27] = m_plnkSystemData->m_nPressActionMode;//0->Pin상승후 Punching, 1->동시동작
	m_iFuncOnOff[28] = m_plnkSystemData->m_nSprocket1ClutchMode;//0->Feeding후 ClutchOff, 1->Punching후 ClutchOff

	//20111205 SJ_HJG
	m_iFuncOnOff[29] = m_plnkSystemData->m_bUseAutoAlignSystem;
	//120130.kms_______4Mount All Mode
	m_iFuncOnOff[30] = m_plnkSystemData->m_bUse4MountAll;
	m_iFuncOnOff[31] = m_plnkSystemData->m_bUseMoldAutoSearch;
	m_iFuncOnOff[32] = m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading; //170201 SJ_YSH


	for (int i = 0; i < DEF_MAX_FUNC_NUMBER; i++)
		UpdateDisplay(i);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_15"), &szTextTemp);
//	기능 On/Off Dialog
	SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_1"), &szTextTemp);
//	Fiducial Mark\r\nAuto Search 사용
	GetDlgItem(IDB_ON_OFF_BUTTON_0)->SetWindowText(szTextTemp);

//	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_2"), &szTextTemp);
//	Plate Vacuum 사용
//	GetDlgItem(IDB_ON_OFF_BUTTON_1)->SetWindowText(szTextTemp);

//	Plate Vacuum 사용
//	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_2"), &szTextTemp);
	szTextTemp.Format(_T("UseTabBolb ForDouble Attachment"));
	GetDlgItem(IDB_ON_OFF_BUTTON_1)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_3"), &szTextTemp);
//	Panel Mark\r\nManual Search
	GetDlgItem(IDB_ON_OFF_BUTTON_2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_4"), &szTextTemp);
//	자재 사전 Alarm
	GetDlgItem(IDB_ON_OFF_BUTTON_3)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_5"), &szTextTemp);
//	Buzzer 사용
	GetDlgItem(IDB_ON_OFF_BUTTON_4)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_6"), &szTextTemp);
//	Heater Alarm\r\n사용	
	GetDlgItem(IDB_ON_OFF_BUTTON_7)->SetWindowText(szTextTemp);

	/*
	GetDlgItem(IDB_ON_OFF_BUTTON_13)->SetWindowText(_T("Reserved"));
	GetDlgItem(IDB_ON_OFF_BUTTON_13)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_13)->EnableWindow(FALSE);
	*/
	//Use Feeder 선택
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_16"), &szTextTemp);
	GetDlgItem(IDB_ON_OFF_BUTTON_13)->SetWindowText(szTextTemp);
	
	
	//GetDlgItem(IDB_ON_OFF_BUTTON_8)->SetWindowText(_T("Auto Search Area\r\nIn After Insepction"));
	GetDlgItem(IDB_ON_OFF_BUTTON_8)->SetWindowText(_T("Use Bonding \r\n After Img Log"));

	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_8"), &szTextTemp);
//	TabIC 압착 후\r\n압착 상태 검사
	GetDlgItem(IDB_ON_OFF_BUTTON_9)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_9"), &szTextTemp);
//	Panel Validation\r\nCheck
	GetDlgItem(IDB_ON_OFF_BUTTON_11)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_10"), &szTextTemp);
//	Panel 흡착에 따른\r\nPanel 위치 변화 반영
	GetDlgItem(IDB_ON_OFF_BUTTON_12)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_11"), &szTextTemp);
//	R/Y Lamp Off\r\nBuzzer Off
	GetDlgItem(IDB_ON_OFF_BUTTON_14)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_12"), &szTextTemp);
//	금형 Blow 사용
	GetDlgItem(IDB_ON_OFF_BUTTON_15)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_13"), &szTextTemp);
//	저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_14"), &szTextTemp);

//	나가기	
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	//@KKY.130911_______
	GetDlgItem(IDB_ON_OFF_BUTTON_30)->SetWindowText(_T("TabIC\r\nbefore Press"));
	//@_________________

	//JHB_101109 m_bUseMaterialDeviceID관련
//	GetDlgItem(IDB_ON_OFF_BUTTON_18)->EnableWindow(FALSE);
//	GetDlgItem(IDC_ON_OFF_VALUE_18)->EnableWindow(FALSE);
//20110811 SJ_HJG 임시
	GetDlgItem(IDB_ON_OFF_BUTTON_18)->EnableWindow(TRUE);
	GetDlgItem(IDC_ON_OFF_VALUE_18)->EnableWindow(TRUE);

	//GetDlgItem(IDB_ON_OFF_BUTTON_1)->EnableWindow(FALSE); //170612 JSH
	GetDlgItem(IDB_ON_OFF_BUTTON_5)->EnableWindow(FALSE);
	//GetDlgItem(IDC_ON_OFF_VALUE_01)->EnableWindow(FALSE); //170612 JSH
	GetDlgItem(IDC_ON_OFF_VALUE_05)->EnableWindow(FALSE);

	//환경안전 요청사항_________
	//20171006 KDW Start
	//GetDlgItem(IDB_ON_OFF_BUTTON_4)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_ON_OFF_VALUE_04)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_ON_OFF_BUTTON_4)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_04)->EnableWindow(FALSE);
	//20171006 KDW End
	/*/
#ifndef DEF_SETUP
	GetDlgItem(IDB_ON_OFF_BUTTON_6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ON_OFF_VALUE_06)->ShowWindow(SW_HIDE);
#endif
	/*/
//	GetDlgItem(IDB_ON_OFF_BUTTON_7)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_ON_OFF_VALUE_07)->ShowWindow(SW_HIDE);

	//장광온 책임 요청 사항_____
	GetDlgItem(IDB_ON_OFF_BUTTON_20)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_20)->EnableWindow(FALSE);

	GetDlgItem(IDB_ON_OFF_BUTTON_5)->EnableWindow(TRUE);
	GetDlgItem(IDC_ON_OFF_VALUE_05)->EnableWindow(TRUE);


	GetDlgItem(IDC_ON_OFF_VALUE_12)->EnableWindow(TRUE);
	GetDlgItem(IDB_ON_OFF_BUTTON_12)->EnableWindow(TRUE);



	//110110.Add.TabMount Switching Mode_____
	ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_22"), &szTextTemp);
	GetDlgItem(IDB_ON_OFF_BUTTON_22)->SetWindowText(szTextTemp);
	//_______________________________________
	GetDlgItem(IDB_ON_OFF_BUTTON_23)->SetWindowText(_T("Align Pass"));
	
	//101001.KKY._____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	//_______________

//#ifdef DEF_GATE_SYSTEM
	GetDlgItem(IDB_ON_OFF_BUTTON_22)->EnableWindow(FALSE);
	m_sFuncOnOff[22].EnableWindow(FALSE);
//#endif

	//110322.Add_____
	if (TRUE == m_iFuncOnOff[22])
	{
		GetDlgItem(IDB_ON_OFF_BUTTON_13)->EnableWindow(FALSE);
		m_sFuncOnOff[13].EnableWindow(FALSE);
	}
	for (int i = 0; i < DEF_MAX_FUNC_NUMBER; i++)
		m_iFuncOnOffOld[i] = m_iFuncOnOff[i];
	//_______________

	//170718 JSH.s
	/*/
	//@130206________
	if (m_iFuncOnOff[6] == 1)
	{
		GetDlgItem(IDB_ON_OFF_BUTTON_6)->EnableWindow(FALSE);
		GetDlgItem(IDC_ON_OFF_VALUE_06)->EnableWindow(FALSE);
	}
	/*/
	GetDlgItem(IDB_ON_OFF_BUTTON_6)->SetWindowText(_T("Use 1st Panel Align"));
	//170718 JSH.e

	GetDlgItem(IDB_ON_OFF_BUTTON_5)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_05)->EnableWindow(FALSE);
	GetDlgItem(IDB_ON_OFF_BUTTON_9)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_09)->EnableWindow(FALSE);	
	//GetDlgItem(IDB_ON_OFF_BUTTON_8)->EnableWindow(FALSE);
	//GetDlgItem(IDC_ON_OFF_VALUE_08)->EnableWindow(FALSE);
	GetDlgItem(IDB_ON_OFF_BUTTON_13)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_13)->EnableWindow(FALSE);
	GetDlgItem(IDB_ON_OFF_BUTTON_24)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_24)->EnableWindow(FALSE);
	GetDlgItem(IDB_ON_OFF_BUTTON_16)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_16)->EnableWindow(FALSE);
	GetDlgItem(IDB_ON_OFF_BUTTON_17)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_17)->EnableWindow(FALSE);

	//@TabIC 선타발 기능으로 변경함_____
//@	GetDlgItem(IDB_ON_OFF_BUTTON_30)->EnableWindow(FALSE);
//@	GetDlgItem(IDC_ON_OFF_VALUE_30)->EnableWindow(FALSE);
	//__________________________________
	
	//170412 SJ_YSH
	GetDlgItem(IDB_ON_OFF_BUTTON_32)->EnableWindow(FALSE);
	GetDlgItem(IDC_ON_OFF_VALUE_32)->EnableWindow(FALSE);
	//_____________
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataFuncOnOffDlg::OnExit() 
{
	m_plnkSystemData->CancelChangeData();

	if (m_iFuncOnOffOld[13] != m_iFuncOnOff[13] || m_iFuncOnOffOld[22] != m_iFuncOnOff[22])
	{
		MOlbSystemPre.GetTrsAutoManagerComponent()->ResetUnitInitialFlag();
	}

	OnOK();
}

void CDataFuncOnOffDlg::OnSave() 
{
//	Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40300, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkSystemData->WriteData();

	for (int i = 0; i < m_ItemMax; i++)
		UpdateDisplay(i);

	return;

	m_plnkSystemData->m_bUseAutoSearchPanel				= m_iFuncOnOff[0];
//	m_plnkSystemData->m_bUsePlateVacuum					= m_iFuncOnOff[1];
	m_plnkSystemData->m_bUseInspectionTabBolb			= m_iFuncOnOff[1]; //170612 JSH
	m_plnkSystemData->m_bUseTabReelAutoChange			= m_iFuncOnOff[2];	// 임시로 PanelMarkManualSearch 기능으로 사용.
	m_plnkSystemData->m_bUseMaterialPrealarm			= m_iFuncOnOff[3];
	m_plnkSystemData->m_bUseBuzzer						= m_iFuncOnOff[4];
//@	m_plnkSystemData->m_bUseTabPanelAlign				= m_iFuncOnOff[5];
	m_plnkPreBonderData->m_bUseTabPanelAlign			= m_iFuncOnOff[5];
	//m_plnkSystemData->m_bSafetySensor					= m_iFuncOnOff[6];
	m_plnkSystemData->m_bUse1stPanelAlign				= m_iFuncOnOff[6];
	m_plnkSystemData->m_bUseHeaterAlarm					= m_iFuncOnOff[7];
	//m_plnkSystemData->m_bUseAutoSearchAreaInAfterInspection	= m_iFuncOnOff[8];
	m_plnkSystemData->m_bUseBondingAfterImgLog = m_iFuncOnOff[8];
	m_plnkSystemData->m_bUseInspectionAfterMountInAutoRun = m_iFuncOnOff[9];
//	m_plnkSystemData->m_bUseTestStop					= m_iFuncOnOff[10];
	m_plnkSystemData->m_bUsePanelPusher					= m_iFuncOnOff[10];
	m_plnkSystemData->m_bUsePanelValidation				= m_iFuncOnOff[11];
	m_plnkSystemData->m_bUsePanelAbsorbOffsetReflection = m_iFuncOnOff[12];

	m_plnkSystemData->m_nUseFeeder						= m_iFuncOnOff[13];

	m_plnkSystemData->m_bUseVIPMode						= m_iFuncOnOff[14];
	m_plnkSystemData->m_bUseMoldBlow					= m_iFuncOnOff[15];
	//20081110 by kss
	m_plnkSystemData->m_bUsePanelPreAlign				= m_iFuncOnOff[16];
	m_plnkSystemData->m_bUseAutoModelChange				= m_iFuncOnOff[17];
	m_plnkSystemData->m_bUseMaterialDeviceID			= m_iFuncOnOff[18];

	//100825.kms
	m_plnkSystemData->m_bUseMoldValidation				= m_iFuncOnOff[19];

	// 100807. ECID ________________
	m_plnkSystemData->m_bUseECDataReport				= m_iFuncOnOff[20];
	//@130206.add___________________
	m_plnkSystemData->m_bUseTabRemoveAuto				= m_iFuncOnOff[21];
	//@_____________________________
	//120116.KMS_______
//	m_plnkSystemData->m_bUse1FeederAllMount			= m_iFuncOnOff[22];
	m_plnkSystemData->m_bUse1FeederAllMount			= FALSE;
	//_________________

	//SJ_YYK 110414 Add...Align Pass Mode..
	m_plnkSystemData->m_bUseAlignPass					= m_iFuncOnOff[23];

	m_plnkSystemData->m_bUseSyncMode					= m_iFuncOnOff[24];
	m_plnkSystemData->m_bUseProductPlanValidation					= m_iFuncOnOff[25];
	m_plnkSystemData->m_bUseTXDataCheckMode					= m_iFuncOnOff[26];

	m_plnkSystemData->m_nPressActionMode				= m_iFuncOnOff[27];
	m_plnkSystemData->m_nSprocket1ClutchMode			= m_iFuncOnOff[28];
	
	m_plnkPreBonderData->m_bUseTabPanelAlign			= FALSE;

	//20111205 SJ_HJG

	if(m_plnkSystemData->m_bUseAutoAlignSystem	!= m_iFuncOnOff[29])
	{
		//20111205 SJ_HJG Auto Align 기능 Off 시 현재 Offet 값 저장! 
		if(MOlbSystemPre.GetSystemData()->m_bUseAutoAlignSystem == TRUE)
		{
			//20120126 수정.... OffsetData 바로 사용!!
			/*
			CString strAutoAlignLog,strTabdataLog = "",strdataLog = "";
			
			for(int i = 0; i < MOlbSystemPre.GetTabData()->m_iXuTabQty; i++)
			{
				strTabdataLog.Format(_T("[Tab No:%d] %0.3f -> %0.3f, "),i + 1, MOlbSystemPre.GetTabOffsetData()->m_rgdXuTabOffsetTabMounter[i], MOlbSystemPre.GetTabMounterComponent(0)->sm_rgdFinalTabOffset[i]);
				MOlbSystemPre.GetTabOffsetData()->m_rgdXuTabOffsetTabMounter[i] = MOlbSystemPre.GetTabMounterComponent(0)->sm_rgdFinalTabOffset[i];
				strdataLog += strTabdataLog;
			}
			
			
			strAutoAlignLog.Format(_T("Auto Align System [Off], %s"),strdataLog);
			*/
			CString strAutoAlignLog;
			strAutoAlignLog.Format(_T("Auto Align System [Off]"));
			MOlbSystemPre.GetTabOffsetData()->WriteData();
			//___________________________________________________
			MOlbSystemPre.GetTrsAutoManagerComponent()->WriteAutoAlignLog(strAutoAlignLog);
		}
		else
		{
			//20120126 수정.... OffsetData 바로 사용!!
			/*
			CString strAutoAlignLog,strTabdataLog = "",strdataLog = "";
			
			for(int i = 0; i < MOlbSystemPre.GetTabData()->m_iXuTabQty; i++)
			{
				strTabdataLog.Format(_T("[Tab No:%d] %0.3f -> %0.3f, "),i + 1,  MOlbSystemPre.GetTabMounterComponent(0)->sm_rgdFinalTabOffset[i], MOlbSystemPre.GetTabOffsetData()->m_rgdXuTabOffsetTabMounter[i]);
				MOlbSystemPre.GetTabMounterComponent(0)->sm_rgdFinalTabOffset[i] = MOlbSystemPre.GetTabOffsetData()->m_rgdXuTabOffsetTabMounter[i];
				strdataLog += strTabdataLog;
			}
			
			strAutoAlignLog.Format(_T("Auto Align System [On], %s"),strdataLog);
			*/
			CString strAutoAlignLog;
			strAutoAlignLog.Format(_T("Auto Align System [On]"));
			//___________________________________________________
			MOlbSystemPre.GetTrsAutoManagerComponent()->WriteAutoAlignLog(strAutoAlignLog);
		}
		//________________________

	}
	m_plnkSystemData->m_bUseAutoAlignSystem				= m_iFuncOnOff[29];

	//120130.kms_________
	m_plnkSystemData->m_bUse4MountAll                      = m_iFuncOnOff[30];
	m_plnkSystemData->m_bUseMoldAutoSearch                = m_iFuncOnOff[31];
	m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading			= m_iFuncOnOff[32]; //170201 SJ_YSH
	m_plnkSystemData->m_bUseCheckVacuumBeforeTabLoading			= TRUE; //170920 JSh

	if(m_iFuncOnOff[20] && (m_bOldUseECDataReport != m_iFuncOnOff[20]))
	{
		MOlbSystemPre.GetTrsAutoManagerComponent()->ReplyEqConstList();
	}
	m_bOldUseECDataReport = m_iFuncOnOff[20];

	m_plnkSystemData->WriteData();
	m_plnkPreBonderData->WriteData();

	MOlbSystemPre.GetPanelAlignerComponent()->ChangeModel();

	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_RUN_MODE, NULL, NULL);
}

void CDataFuncOnOffDlg::ChangeParameterValue(int iFuncID)
{
	if (iFuncID >= m_ItemMax)
		return;
	m_Item[iFuncID]->InputTool();
	return;
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________
	if (iFuncID == 13)
	{
		CString szTextTemp;
		ReadDisplayMessage(MAIN_DATA, _T("DataFuncOnOffDlg_17"), &szTextTemp);
		CSystemThreeBtnDlg dlg(szTextTemp, "Front", "Rear", "All");
		m_iFuncOnOff[iFuncID] = dlg.DoModal();
	}
	else if (iFuncID == 28)
	{
		if (m_iFuncOnOff[iFuncID] == 0)
			m_iFuncOnOff[iFuncID] = 1;
		else if (m_iFuncOnOff[iFuncID] == 1)
			m_iFuncOnOff[iFuncID] = 2;
		else
			m_iFuncOnOff[iFuncID] = 0;

	}
	//@131127.KKY.기능 수정__________
	else if (iFuncID == 10)
	{
		if (m_iFuncOnOff[iFuncID] == 0)
			m_iFuncOnOff[iFuncID] = 1;
		else if (m_iFuncOnOff[iFuncID] == 1)
			m_iFuncOnOff[iFuncID] = 2;
		else
			m_iFuncOnOff[iFuncID] = 0;
	}
	//@______________________________
	else
	{
		if (m_iFuncOnOff[iFuncID])
			m_iFuncOnOff[iFuncID] = FALSE;
		else
			m_iFuncOnOff[iFuncID] = TRUE;
	}
}

void CDataFuncOnOffDlg::UpdateDisplay(int iFuncID)
{
	m_sFuncOnOff[iFuncID].SetCaption(m_Item[iFuncID]->GetCStringData());
	return;
	if (iFuncID == 13)
	{
		if (m_iFuncOnOff[iFuncID] == 0)
			m_sFuncOnOff[iFuncID].SetCaption(_T("Front"));
		else if (m_iFuncOnOff[iFuncID] == 1)
			m_sFuncOnOff[iFuncID].SetCaption(_T("Rear"));
		else
			m_sFuncOnOff[iFuncID].SetCaption(_T("All"));
	}
	else if (iFuncID == 27)
	{
		if (m_iFuncOnOff[iFuncID] == 0)
			m_sFuncOnOff[iFuncID].SetCaption(_T("Punching\nafter Pin Up"));
		else
			m_sFuncOnOff[iFuncID].SetCaption(_T("Multie action"));
	}
	else if (iFuncID == 28)
	{
		if (m_iFuncOnOff[iFuncID] == 0)
			m_sFuncOnOff[iFuncID].SetCaption(_T("Clutch Off\nafter Feeding"));
		else if (m_iFuncOnOff[iFuncID] == 1)
			m_sFuncOnOff[iFuncID].SetCaption(_T("Clutch Off\nafter Punching"));
		else
			m_sFuncOnOff[iFuncID].SetCaption(_T("Clutch\r\nNot Use"));
	}
	//@131127.KKY.기능 수정__________
	else if (iFuncID == 10)
	{
		if (m_iFuncOnOff[iFuncID] == 0)
			m_sFuncOnOff[iFuncID].SetCaption(_T("Not Use"));
		else if (m_iFuncOnOff[iFuncID] == 1)
			m_sFuncOnOff[iFuncID].SetCaption(_T("Use when\nRecognition Fail"));
		else
			m_sFuncOnOff[iFuncID].SetCaption(_T("Continue use"));
	}
	//@______________________________
	else
	{
		if (m_iFuncOnOff[iFuncID])
			m_sFuncOnOff[iFuncID].SetCaption(_T("ON"));
		else
			m_sFuncOnOff[iFuncID].SetCaption(_T("OFF"));
	}
}

void CDataFuncOnOffDlg::OnClickOnOffButton0() 
{
	m_iFuncID = 0;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton1() 
{
	m_iFuncID = 1;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton2() 
{
	m_iFuncID = 2;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton3() 
{
	m_iFuncID = 3;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton4()
{
	m_iFuncID = 4;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton5()
{
	m_iFuncID = 5;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton6() 
{
	m_iFuncID = 6;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton7() 
{
	m_iFuncID = 7;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton8() 
{
	m_iFuncID = 8;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton9() 
{
	m_iFuncID = 9;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton10() 
{
	m_iFuncID = 10;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton11() 
{
	m_iFuncID = 11;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton12() 
{
	m_iFuncID = 12;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton13() 
{
	m_iFuncID = 13;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton14() 
{
	m_iFuncID = 14;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

void CDataFuncOnOffDlg::OnClickOnOffButton15() 
{
	m_iFuncID = 15;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
}

//20081110 by kss
void CDataFuncOnOffDlg::OnClickOnOffButton16() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 16;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton17() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 17;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton18() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 18;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton19() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 19;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton20() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 20;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton21() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 21;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
	
}

void CDataFuncOnOffDlg::OnClickOnOffButton22() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 22;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);
	//110322.Add_____
	if (TRUE == m_iFuncOnOff[22])
	{
		GetDlgItem(IDB_ON_OFF_BUTTON_13)->EnableWindow(FALSE);
		m_sFuncOnOff[13].EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDB_ON_OFF_BUTTON_13)->EnableWindow(TRUE);
		m_sFuncOnOff[13].EnableWindow(TRUE);
	}
	//_______________
}


void CDataFuncOnOffDlg::OnClickOnOffButton23() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 23;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}



void CDataFuncOnOffDlg::OnClickOnOffButton24() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 24;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton25() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 25;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton26() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 26;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton27() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 27;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);		
}

void CDataFuncOnOffDlg::OnClickOnOffButton28() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 28;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDataFuncOnOffDlg::OnClickOnOffButton29() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 29;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}
	

void CDataFuncOnOffDlg::OnClickOnOffButton30() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 30;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

void CDataFuncOnOffDlg::OnClickOnOffButton31() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 31;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}

//161109 JSH.s Cam Add...
void CDataFuncOnOffDlg::OnClickOnOffButton32() 
{
	// TODO: Add your control notification handler code here
	m_iFuncID = 32;
	ChangeParameterValue(m_iFuncID);
	UpdateDisplay(m_iFuncID);	
}
//161109 JSH.e

