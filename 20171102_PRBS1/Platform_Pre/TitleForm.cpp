// TitleForm.cpp : implementation file
//
#include "stdafx.h"
#include "MPlatformOlb.h"
#include "TitleForm.h"
#include "MPlatformOlbSystem.h"
#include "CommonSelectUserGroupDlg.h"
// selsk_jdy #include "CommonMaterialChangeNotify.h"
#include "MCtrlTabFeeder.h"
#include "MTrsAutoManager.h"
// selsk_jdy
#include "MManageOpPanel.h"
// sesl_jdy
#include "AutoPanelMarkManualSearchDlg.h"
#include "AutoInspectJudgeDlg.h"
#include "ManualMarkRecogDlg.h"
#include "SystemFourBtnDlg.h"
#include "CommonMatCodeCheckDlg.h"
#include "MTrsPanelAligner.h"

// 2009.11.01 Park, YK
#include "MSystemData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTitleForm

IMPLEMENT_DYNCREATE(CTitleForm, CFormView)

CTitleForm::CTitleForm()
	: CFormView(CTitleForm::IDD)
{
	//{{AFX_DATA_INIT(CTitleForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bStepStopState = FALSE; //syr..090508
	m_pLogMng = new MLog;

	CString szTextTemp=DEF_VERSION_STRING;
	m_strVer.Format(szTextTemp.GetBuffer(szTextTemp.GetLength()));
}

CTitleForm::~CTitleForm()
{
	delete m_pLogMng;
}

void CTitleForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitleForm)
	DDX_Control(pDX, IDC_TIMER_LABEL, m_LblTimer);
	DDX_Control(pDX, IDC_TITLE_MSG, m_LblTitleMsg);
	DDX_Control(pDX, IDC_LOGIN, m_btnLogin);
	DDX_Control(pDX, IDB_BUZZER_OFF, m_btnBuzzerOff);
	DDX_Control(pDX, IDC_TITLE_LOGO, m_btnTitleLogo);
	DDX_Control(pDX, IDB_KEYBOARD, m_btnKeyBoard);
	
	//}}AFX_DATA_MAP
#ifdef SIMULATION

#endif	/* SIMULATION */
}


BEGIN_MESSAGE_MAP(CTitleForm, CFormView)
	//{{AFX_MSG_MAP(CTitleForm)
	ON_WM_TIMER()
	ON_MESSAGE(WM_ALARM_MSG, OnAlarmMsg)
	ON_MESSAGE(WM_DISP_E_STOP_STATUS, OnDisplayEStopStatus)
	ON_MESSAGE(WM_DISP_TEACH_PENDANT_CONNECTING_STATUS, OnDisplayTPConnectingStatus)
// selsk_jdy	ON_MESSAGE(WM_DISP_FEEDER_AUTO_HOMING_STATUS, OnDisplayFeederAutoHomingStatus)
	ON_MESSAGE(WM_TABFEEDER_RAIL_UNFOLD_REQ, OnDisplayRailUnfoldRequest)
// selsk_jdy 	ON_MESSAGE(WM_DISP_MANUAL_MODE_CHANGE_REQ, OnDisplayManualModeChanging)
	// selsk_jdy
	ON_MESSAGE(WM_DISP_MATERIAL_EXHAUST_PRE_ALARM_REQ, OnDisplayMaterialExhaustPreAlarm)
	ON_MESSAGE(WM_DISP_MY_MESSAGE, OnDisplayMyMessage)
	// sesl_jdy
	ON_MESSAGE(WM_DISP_MANUAL_PANEL_MARK_SEARCH_DLG_REQ, OnDispPanelMarkManualSearchDlg)
	ON_MESSAGE(WM_DISP_INSPECTION_JUDGE_DLG_REQ, OnDispInspectionJudgeDlg)

	ON_WM_DESTROY()
	ON_MESSAGE(WM_DISP_PROGRESS_DLG_FOR_AUTO_MODEL_CHANGE, OnDisplayProgressDlgForAutoMC)
	ON_MESSAGE(WM_DISP_MAT_CHECK, OnDispMatCheckDlg)
	ON_MESSAGE(WM_DISP_EQSTOP_MSG, OnDisplayEqStopMessage)
	ON_MESSAGE(WM_DISP_OPERATER_MSG, OnDisplayOperaterChangeMessage)
	ON_MESSAGE(WM_DISP_MANUAL_SEARCH_REQUEST, OnDisplayManualSearchRequest)
	ON_MESSAGE(WM_DISP_MANUAL_SEARCH_POINT, OnDisplayManualSearchPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleForm diagnostics

#ifdef _DEBUG
void CTitleForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CTitleForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTitleForm message handlers

void CTitleForm::OnTimer(UINT nIDEvent) 
{
	//110817 SJ_KJS
	if (MOlbSystemPre.GetLCNetComponent()->m_bConnected == TRUE)
		m_strLCConnected.Format(_T("[ONLINE]"));
	else
		m_strLCConnected.Format(_T("[OFFLINE]"));
	//_________________________________________________
	// 현재 Version Setting
	m_tCurrentTime = CTime::GetCurrentTime();
	m_strCurrentTime = m_tCurrentTime.Format("%Y-%b-%d [%a] <%H:%M:%S>  ");
	m_strCurrentTime = m_strCurrentTime + m_strVer + m_strLCConnected;

	m_LblTimer.SetCaption(m_strCurrentTime);
//	m_LblTimer.SetBackColor(RGB(60,90,151));
//	m_LblTimer.SetForeColor(GREEN);

	//syr..SESL : hongju 수정... 이부분 확인 필요...스위치로 했을때 안 나올 수 있음.
//	if (m_plnkTrsAutoManager->IsStepStopSWPressed())
	if (STEP_STOP == m_plnkTrsAutoManager->GetOPStatus() && AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
	{
		if (!m_bStepStopState)
		{
			CString szTextTemp1;
			ReadDisplayMessage(MY_MESSAGE_BOX, _T("NotifyStop"), &szTextTemp1);
			m_StepStopNotifyDlg.SetMessage(szTextTemp1);
			m_StepStopNotifyDlg.ShowWindow(SW_SHOW);
			m_bStepStopState = TRUE;
		}
	}
	else
	{
		m_StepStopNotifyDlg.ShowWindow(SW_HIDE);
		if (m_bStepStopState)
		{
			m_bStepStopState = FALSE;
		}
	}

	// selsk_jdy
	if (FALSE == m_plnkManageOpPanel->IsBuzzerMode())
	{
		m_btnBuzzerOff.ShowWindow(SW_HIDE);
		m_btnBuzzerOff.EnableWindow(FALSE);
	}
	
	// hongju 수정...SESL의 경우 Autorun 상태가 되면 무조건 Operator 모드로 변경
#ifdef DEF_SESL_SITE
	if (AUTORUN_MODE == MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode())
	{
		m_btnLogin.EnableWindow(FALSE);
		if (MOlbSystemPre.GetCurrentUserGroup() != OPERATOR_GROUP)
		{
			MOlbSystemPre.SetCurrentUserGroup(OPERATOR_GROUP);
			dispLogInInfo();

			CString strMsg;
			CTime	tCurrentTime = CTime::GetCurrentTime();
			CString strCurrentTime = tCurrentTime.Format(_T("[%Y-%b-%d : %H:%M:%S] "));

			strMsg.Format(_T("자동 Logoff 되었습니다.\nIt is logoff auto"));
			m_LblTitleMsg.SetCaption(strCurrentTime + _T(" ") + strMsg);

			m_pLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL , strMsg, __FILE__, __LINE__);
		}
	}
	else
	{
		m_btnLogin.EnableWindow(TRUE);
	}
#endif //DEF_SESL_SITE

	/*
	//120507.kms___________
	if (AUTORUN_MODE == MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode())
	{
		if (MOlbSystemPre.GetCurrentUserGroup() != OPERATOR_GROUP)
		{
			MOlbSystemPre.SetCurrentUserGroup(OPERATOR_GROUP);
			dispLogInInfo();
		}
	}
	//_____________________*/

	// NSMC
	if (m_plnkTrsAutoManager->GetOPStatus() == ERROR_STOP
		|| m_plnkTrsAutoManager->GetOPStatus() == STEP_STOP)
	{
		m_ProgressDlgForAutoMC.ShowWindow(SW_HIDE);
	}
	
	CFormView::OnTimer(nIDEvent);
}

void CTitleForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	// selsk_jdy
	m_plnkTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
	m_plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();

	// 2009.11.01 Park, YK
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_uiLanguageSelect = m_plnkSystemData->m_uiLanguageSelect;

	CString strMsg;
	m_EStopNotifyDlg.Create(IDD_COMMON_ESTOP_NOTIFY, this);
	// \"Emergency Stop\"으로 인한 설비 전원 차단 상태임.\nE-Stop 해제 후, Reset Switch 를 누르세요.\n조치 후, 축 원점복귀를 수행하세요.
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63000"), &strMsg);
	m_EStopNotifyDlg.SetMessage(strMsg);
	m_EStopNotifyDlg.CenterWindow();

	m_StepStopNotifyDlg.Create(IDD_COMMON_STEPSTOP_NOTIFY,this); //syr..090508
	m_StepStopNotifyDlg.CenterWindow();

	m_PendantNotifyDlg.Create(IDD_COMMON_PENDANT_NOTIFY, this);
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63001"), &strMsg);
	// Teaching Pendant 가 연결되어 있음.\nTeaching Pendant 상의 Jog Key 동작만 가능함.
	m_PendantNotifyDlg.SetMessage(strMsg);
	m_PendantNotifyDlg.CenterWindow();

	// NSMC
	m_ProgressDlgForAutoMC.Create(IDD_COMMON_NOTIFY, this);
	m_ProgressDlgForAutoMC.CenterWindow();

	//100825.KMS
	m_dlgEqStopCode.Create(IDD_EQ_STOP, this);
	m_dlgEqStopCode.CenterWindow();
	
	
	/* selsk_jdy
	// selsk_jdy m_MaterialChangeNotifyDlg.Create(IDD_COMMON_MATERIAL_CHANGE_NOTIFY, this);
	// selsk_jdy
	m_MaterialChangeNotifyDlg.Create(IDD_COMMON_NOTIFY_DLG, this);
	// selsk_jdy
	m_MaterialChangeNotifyDlg.SetWindowText(ML_BTN("자재 교환 알림", "Material Exchange Alarm"));
	m_MaterialChangeNotifyDlg.CenterWindow();
	*/

	/* selsk_jdy
	// selsk_jdy m_ModeChangeNotifyDlg.Create(IDD_COMMON_MODE_CHANGE_NOTIFY, this);
	// selsk_jdy
	m_ModeChangeNotifyDlg.Create(IDD_COMMON_NOTIFY_DLG, this);
	// selsk_jdy
	m_ModeChangeNotifyDlg.SetWindowText("Operation Mode Change");
	strMsg = ML_MSG("Operation Mode 전환 중입니다.", "Operation Mode is altering.");
	m_ModeChangeNotifyDlg.SetMessage(strMsg);
	m_ModeChangeNotifyDlg.CenterWindow();
	*/

	// selsk_jdy
	m_btnBuzzerOff.ShowWindow(SW_HIDE);
	m_btnBuzzerOff.EnableWindow(FALSE);

	m_plnkTrsAutoManager->SetTitleWindowHandle(m_hWnd);

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
		MOlbSystemPre.GetCtrlTabFeederComponent(i)->SetTitleWindowHandle(m_hWnd);

	dispLogInInfo();
	
	// TODO: Add your specialized code here and/or call the base class
#ifndef SIMULATION

#endif /* SIMULATION */
	m_pLogMng->SetLogAttribute(-1, "..\\Log\\Login\\Login.log", DEF_MLOG_NORMAL_LOG_LEVEL);
	m_TimerID = SetTimer(1, 500, NULL);
}

BEGIN_EVENTSINK_MAP(CTitleForm, CFormView)
    //{{AFX_EVENTSINK_MAP(CTitleForm)
	ON_EVENT(CTitleForm, IDC_LOGIN, -600 /* Click */, OnClickLogin, VTS_NONE)
	ON_EVENT(CTitleForm, IDB_BUZZER_OFF, -600 /* Click */, OnBuzzerOff, VTS_NONE)
	ON_EVENT(CTitleForm, IDC_LANGUAGE, -600 /* Click */, OnLanguageChange, VTS_NONE)
	ON_EVENT(CTitleForm, IDC_TITLE_LOGO, -600 /* Click */, OnClickTitleLogo, VTS_NONE)
	ON_EVENT(CTitleForm, IDB_KEYBOARD, -600 /* Click */, OnClickKeyBoard, VTS_NONE)
	ON_EVENT(CTitleForm, IDB_INFO, -600 /* Click */, OnClickInfo, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
#ifdef SIMULATION

#endif	/* SIMULATION */
END_EVENTSINK_MAP()

LRESULT CTitleForm::OnAlarmMsg(UINT wParam, LPARAM lParam)
{
	CTime tCurrentTime = CTime::GetCurrentTime();
	CString strCurrentTime = tCurrentTime.Format("[%Y-%b-%d : %H:%M:%S] ");

	CString strAlarmMsg;

// jdy sesl	strAlarmMsg.Format(_T("%s %s"), strCurrentTime, (char *)lParam);
	strAlarmMsg.Format(_T("%s%s"), strCurrentTime, (LPCTSTR)lParam);
	m_LblTitleMsg.SetCaption(strAlarmMsg);
	// selsk_jdy
	// sesl_jdy m_LblTitleMsg.SetForeColor(BLACK);
	if (FALSE != wParam)
		m_LblTitleMsg.SetForeColor(RED);
	else
		m_LblTitleMsg.SetForeColor(BLACK);
	return 0;
}

LRESULT CTitleForm::OnDisplayEStopStatus(UINT wParam, LPARAM lParam)
{
	if (TRUE == wParam)
		m_EStopNotifyDlg.ShowWindow(SW_SHOW);
	else
		m_EStopNotifyDlg.ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CTitleForm::OnDisplayTPConnectingStatus(UINT wParam, LPARAM lParam)
{
	if (TRUE == wParam)
		m_PendantNotifyDlg.ShowWindow(SW_SHOW);
	else
		m_PendantNotifyDlg.ShowWindow(SW_HIDE);
	return 0;
}

/* selsk_jdy
void CTitleForm::OnDisplayFeederAutoHomingStatus(UINT wParam, LPARAM lParam)
{
	if (TRUE == wParam)
	{
		CString strMsg;
		strMsg.Format("Feeder%d ", lParam + 1);
		CString strMsg2 = ML_STR("Sprocket2 AutoHoming 진행 중입니다.", "Sprocket2 AutoHoming is going on.");
		strMsg += strMsg2;
		m_MaterialChangeNotifyDlg.SetMessage(strMsg);
		m_MaterialChangeNotifyDlg.ShowWindow(SW_SHOW);
	}
	else
		m_MaterialChangeNotifyDlg.ShowWindow(SW_HIDE);
}
*/

LRESULT CTitleForm::OnDisplayRailUnfoldRequest(UINT wParam, LPARAM lParam)
{
	int iFeederNo = (int)wParam;
	CString strMsg;
	strMsg.Format(_T("TabFeeder%d "), iFeederNo + 1);
	//Reel 회수부가 접혀 있습니다.
	CString strMsg2;
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63002"), &strMsg2);
	strMsg += strMsg2;
	UINT uiDlgType = M_ICONINFORMATION;
	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
	{
		// sesl_jdy m_plnkTrsAutoManager->SetOperatorCall(TRUE, strMsg);
		m_plnkTrsAutoManager->SetOperatorCall(TRUE, strMsg, DEF_DEFAULT, FALSE);
		uiDlgType = M_ICONERROR;
	}
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63003"), &strMsg2);
	//회수부를 편 후, 메세지 창을 닫아 주세요.
	strMsg = strMsg + _T("\r\n") + strMsg2;
	MyMessageBox(strMsg, _T("Request"), uiDlgType);

	if (AUTORUN_MODE == m_plnkTrsAutoManager->GetOPMode())
	{
		//회수부 Rail Unfold : 사용자 확인 완료
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63005"), &strMsg2);
		m_plnkTrsAutoManager->SetOperatorCall(FALSE, strMsg2);
	}
	return 0;
}

/* selsk_jdy
void CTitleForm::OnDisplayManualModeChanging(UINT wParam, LPARAM lParam)
{
	if (TRUE == wParam)
		m_ModeChangeNotifyDlg.ShowWindow(SW_SHOW);
	else
		m_ModeChangeNotifyDlg.ShowWindow(SW_HIDE);
}
*/

// selsk_jdy
LRESULT CTitleForm::OnDisplayMaterialExhaustPreAlarm(UINT wParam, LPARAM lParam)
{
	if (TRUE == m_plnkTrsAutoManager->IsMaterialExchange())
		return 0;

	m_plnkTrsAutoManager->SetMaterialExchange(TRUE);
	m_plnkManageOpPanel->SetBuzzerMode(MOlbSystemPre.GetSystemData()->m_bUseBuzzer);
	Sleep(0);
	peekNpump();

	CString strMsg;
	CString szTemp;
	if (DEF_TABFEEDER1 == (int)wParam)
		strMsg.Format(_T("Front TabFeeder")); 
	else // if (DEF_TABFEEDER2 == (int)wParam)
		strMsg.Format(_T("Rear TabFeeder"));
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_63004"), &szTemp);
	//: 대기 Reel 을 장착해 주세요.
	strMsg = strMsg + _T("\r\n") + szTemp;

	CTime	tCurrentTime = CTime::GetCurrentTime();
	CString strCurrentTime = tCurrentTime.Format(_T("[%b-%d: %H:%M:%S] "));

	// jdy sesl		m_LblTitleMsg.SetCaption(strCurrentTime + " " +strMsg);
	m_LblTitleMsg.SetCaption(strCurrentTime + strMsg);
	m_LblTitleMsg.SetForeColor(RED);

	m_btnBuzzerOff.EnableWindow(TRUE);
	m_btnBuzzerOff.ShowWindow(SW_SHOW);
	return 0;
}

// selsk_jdy
void CTitleForm::peekNpump()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			return;
		}
	}
}

// selsk_jdy
LRESULT CTitleForm::OnDisplayMyMessage(UINT wParam, LPARAM lParam)
{
	if (FALSE == (BOOL)wParam)
	{
		m_MyMessageDlg.ShowWindow(SW_HIDE);
		return 0;
	} 

	if (NULL == lParam)
		return 0;

	m_MyMessageDlg.SetMessage((LPCTSTR)lParam);
	m_MyMessageDlg.Show();
	return 0;
}

void CTitleForm::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_TimerID)
		KillTimer(m_TimerID);	
}

void CTitleForm::OnClickLogin() 
{
	// TODO: Add your control notification handler code here
	// selsk_jdy CCommonSelectUserGroupDlg dlg(m_pLogMng);
	// selsk_jdy
	CCommonSelectUserGroupDlg dlg(m_pLogMng, this);
	dlg.DoModal();
	dispLogInInfo();
}

void CTitleForm::OnClickTitleLogo()
{
	GetParentFrame()->ShowWindow(SW_MINIMIZE);
}

void CTitleForm::OnClickKeyBoard()
{
	MUtilManager::DisplayWindowsKeyboard();

}

void CTitleForm::dispLogInInfo()
{
	// selsk_jdy
	switch (MOlbSystemPre.GetCurrentUserGroup())
	{
	case OPERATOR_GROUP:
		m_btnLogin.SetCaption(_T("Operator"));
		break;
	case ENGINEER_GROUP:
		m_btnLogin.SetCaption(_T("Engineer"));
		break;
	case MAKER_GROUP:
		m_btnLogin.SetCaption(_T("Maker"));
		break;
	}

/* selsk_jdy
	CString strUserGroup;

	switch (MOlbSystemPre.GetCurrentUserGroup())
	{
	case OPERATOR_GROUP:
		strUserGroup = "Operator";
		break;
	case ENGINEER_GROUP:
		strUserGroup = "Engineer";
		break;
	case MAKER_GROUP:
		strUserGroup = "Maker";
		break;
	}

	m_btnLogin.SetCaption(strUserGroup);

	CTime	tCurrentTime = CTime::GetCurrentTime();
	CString strCurrentTime = tCurrentTime.Format("[%Y-%b-%d : %H:%M:%S] ");
	CString strMsg = strUserGroup + ML_STR(" 로 Log in 되었습니다.", " is logged in.");
	m_LblTitleMsg.SetCaption(strCurrentTime + " " + strMsg);
	// selsk_jdy
	m_LblTitleMsg.SetForeColor(BLACK);

	m_pLogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL , strMsg, __FILE__, __LINE__);
	*/
}

void CTitleForm::OnBuzzerOff() 
{
	m_plnkManageOpPanel->SetBuzzerMode(FALSE);

	m_btnBuzzerOff.ShowWindow(SW_HIDE);
	m_btnBuzzerOff.EnableWindow(FALSE);		
}

// sesl_jdy
LRESULT CTitleForm::OnDispPanelMarkManualSearchDlg(UINT wParam, LPARAM lParam)
{
	m_plnkTrsAutoManager->SetOperatorCall(TRUE, _T(""), DEF_DEFAULT, FALSE);
	CAutoPanelMarkManualSearchDlg dlg;
	int* iRet = (int*)lParam;
	*iRet = dlg.DoModal();
	m_plnkTrsAutoManager->SetOperatorCall(FALSE, _T(""));
	return 0;
}

LRESULT CTitleForm::OnDispInspectionJudgeDlg(UINT wParam, LPARAM lParam)
{
	int iCamNo = (int)wParam;
	CAutoInspectJudgeDlg dlg(iCamNo);
	int* iRet = (int*)lParam;
	*iRet = dlg.DoModal();
	return 0;
}

// NSMC
LRESULT CTitleForm::OnDisplayProgressDlgForAutoMC(WPARAM wParam, LPARAM lParam)
{
	if (TRUE == (BOOL)wParam)
	{
		m_ProgressDlgForAutoMC.SetTitle(_T("Model Change"));
		m_ProgressDlgForAutoMC.SetMessage(_T("Model Changing..."));
		m_ProgressDlgForAutoMC.PeekAndPump();
		m_ProgressDlgForAutoMC.ShowWindow(SW_SHOW);
		m_ProgressDlgForAutoMC.PeekAndPump();
		return 0;
	}
	else
		m_ProgressDlgForAutoMC.ShowWindow(SW_HIDE);
	
	return 0;
}

// NSMC
LRESULT CTitleForm::OnDispMatCheckDlg(WPARAM wParam, LPARAM lParam)
{
	CCommonMatCodeCheckDlg dlg(wParam, lParam);
	dlg.DoModal();
	
	return 0;
}

LRESULT CTitleForm::OnDisplayEqStopMessage(WPARAM wParam, LPARAM lParam)
{
	m_dlgEqStopCode.ShowWindow(SW_SHOW);

	return 0;
}

LRESULT CTitleForm::OnDisplayOperaterChangeMessage(WPARAM wParam, LPARAM lParam)
{
	MOlbSystemPre.SetCurrentUserGroup(OPERATOR_GROUP);
	dispLogInInfo();

	return 0;
}

LRESULT CTitleForm::OnDisplayManualSearchRequest(WPARAM wParam, LPARAM lParam)
{
	EMarkErrorTreat* peRet = (EMarkErrorTreat*)wParam;
	*peRet = eMarkStop;
	int iReturn = 0;

	CString strMsg;
	m_plnkTrsAutoManager->SetOperatorCall(TRUE, strMsg, DEF_DEFAULT, FALSE);

	CSystemFourBtnDlg dlg(_T("Align Mathod"), "정지\n확인", "마크\n지정", "Panel\nJog이동", "작업Pass\n(OS)");
	int iSelect = dlg.DoModal();
	m_plnkTrsAutoManager->SetOperatorCall(FALSE, strMsg, DEF_DEFAULT, FALSE);

	CManualMarkRecogDlg dlg1(0, TRUE);
	CAutoPanelMarkManualSearchDlg dlg2;

	switch (iSelect)
	{
	case 0:
		*peRet = eMarkStop;
		break;
	case 1:
		iReturn = dlg1.DoModal();
		if (iReturn == IDOK)
		{
			CPoint markPt[2];
			markPt[0] = dlg1.GetMarkPoint(0);
			markPt[1] = dlg1.GetMarkPoint(1);
			MOlbSystemPre.GetTrsPanelAlignerComponent()->SetManualMarkPos(0, markPt[0]);
			MOlbSystemPre.GetTrsPanelAlignerComponent()->SetManualMarkPos(1, markPt[1]);
			*peRet = eMarkRunManual;
		}
		else
		{
			*peRet = eMarkStop;	//@정지
		}
		break;
	case 2:
		iReturn = dlg2.DoModal();
		if (iReturn == DEF_PANEL_ALIGN_RUN_STOP)	//@정지
			*peRet = eMarkStop;
		else if (iReturn == DEF_PANEL_ALIGN_PASS)	//@계속 진행
			*peRet = eMarkRunRetry;
		break;
	case 3:
		*peRet = eMarkOS;
		break;
	}
	
	return 0L;
}

LRESULT CTitleForm::OnDisplayManualSearchPoint(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;
	m_plnkTrsAutoManager->SetOperatorCall(TRUE, strMsg, DEF_DEFAULT, FALSE);

	CPoint *ptMark = (CPoint*)wParam;
	int* iResult = (int*)lParam;	//Camera번호
	if (*iResult == -1)	//Camera선택할수 있게
	{
		CManualMarkRecogDlg dlg(DEF_PANEL_ALIGN_CAMERA_1, TRUE);
		*iResult = dlg.DoModal();	//@IDOK, IDCANCEL
		if (*iResult == IDOK)
		{
			ptMark[0] = dlg.GetMarkPoint(0);
			ptMark[1] = dlg.GetMarkPoint(1);
		}
	}
	else				//Camera선택하지 못하게
	{
		CManualMarkRecogDlg dlg(*iResult, FALSE);
		*iResult = dlg.DoModal();		
		if (*iResult == IDOK)
		{
			ptMark[0] = dlg.GetMarkPoint(0);
			ptMark[1] = dlg.GetMarkPoint(1);
		}
	}
	m_plnkTrsAutoManager->SetOperatorCall(FALSE, strMsg, DEF_DEFAULT, FALSE);

	return 0L;
}

//DEL void CTitleForm::OnClickLanguage() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	
//DEL }

void CTitleForm::OnLanguageChange() 
{
	// TODO: Add your control notification handler code here
	m_uiLanguageSelect = m_plnkSystemData->m_uiLanguageSelect;
	m_uiLanguageSelect++;

	if (m_uiLanguageSelect > 2)
		m_uiLanguageSelect = 0;

	m_plnkSystemData->m_uiLanguageSelect = m_uiLanguageSelect;
	MLanguageManager::SetNowLanguage(m_plnkSystemData->m_uiLanguageSelect);

	// ML_hongu MainView에 화면을 Display
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetManualViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetDataViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetTeachViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
}

void CTitleForm::OnClickInfo() 
{
	MUtilManager::DisplayInfoPanel();
}
