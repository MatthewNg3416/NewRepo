// BottomForm.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "BottomForm.h"
#include "MPlatformOlbSystem.h"
#include "IAxis.h"

// 20090216 CYJ
#include "DefIOAddrPreBonder.h"
#include "IIO.h"

// jdy sesl
#include "CommonSelectUserGroupDlg.h"
//110929 SJ_KJS
#include "MTrsAutoManager.h"
#include "MainFrm.h"
#include "VersionDisplayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CBottomForm

IMPLEMENT_DYNCREATE(CBottomForm, CFormView)

CBottomForm::CBottomForm()
	: CFormView(CBottomForm::IDD)
{
	//{{AFX_DATA_INIT(CBottomForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(RGB(60,90,151));

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}

CBottomForm::~CBottomForm()
{
}

void CBottomForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBottomForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX,  IDC_AUTO,		m_btnBottom[DEF_AUTO_BTN]);
	DDX_Control(pDX,  IDC_MANUAL,	m_btnBottom[DEF_MANUAL_BTN]);
	DDX_Control(pDX,  IDC_DATA,		m_btnBottom[DEF_DATA_BTN]);
	DDX_Control(pDX,  IDC_TEACH,	m_btnBottom[DEF_TEACH_BTN]);
	DDX_Control(pDX,  IDC_LOG,		m_btnBottom[DEF_LOG_BTN]);
	DDX_Control(pDX,  IDHELP,		m_btnBottom[DEF_HELP_BTN]);
	DDX_Control(pDX,  IDB_DOOR_LOCK, m_lblDoorLock);
	DDX_Control(pDX,  IDB_DOOR_LOCK2, m_lblDoorLock2);
	DDX_Control(pDX,  IDB_DOOR_LOCK3, m_lblDoorLock3);
	DDX_Control(pDX,  IDB_DOOR_LOCK4, m_lblDoorLock4);
}


BEGIN_MESSAGE_MAP(CBottomForm, CFormView)
	//{{AFX_MSG_MAP(CBottomForm)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBottomForm diagnostics

#ifdef _DEBUG
void CBottomForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CBottomForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBottomForm message handlers

void CBottomForm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);	
	
	// Do not call CFormView::OnPaint() for painting messages
}

BEGIN_EVENTSINK_MAP(CBottomForm, CFormView)
    //{{AFX_EVENTSINK_MAP(CBottomForm)
	ON_EVENT(CBottomForm, IDC_AUTO, -600 /* Click */, OnClickAuto, VTS_NONE)
	ON_EVENT(CBottomForm, IDC_MANUAL, -600 /* Click */, OnClickManual, VTS_NONE)
	ON_EVENT(CBottomForm, IDC_DATA, -600 /* Click */, OnClickData, VTS_NONE)
	ON_EVENT(CBottomForm, IDC_TEACH, -600 /* Click */, OnClickTeach, VTS_NONE)
	ON_EVENT(CBottomForm, IDC_LOG, -600 /* Click */, OnClickLog, VTS_NONE)
	ON_EVENT(CBottomForm, IDHELP, -600 /* Click */, OnClickHelp, VTS_NONE)
	ON_EVENT(CBottomForm, IDB_DOOR_LOCK, -600 /* Click */, OnClickDoorLock, VTS_NONE)
	ON_EVENT(CBottomForm, IDB_DOOR_LOCK2, -600 /* Click */, OnClickDoorLock2, VTS_NONE)
	ON_EVENT(CBottomForm, IDB_DOOR_LOCK3, -600 /* Click */, OnClickDoorLock3, VTS_NONE)
	ON_EVENT(CBottomForm, IDB_DOOR_LOCK4, -600 /* Click */, OnClickDoorLock4, VTS_NONE)
	ON_EVENT(CBottomForm, IDB_VER_DISP, -600 /* Click */, OnClickVerDisp, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CBottomForm::OnClickAuto()
{
	if (m_flgAutoSelect != TRUE) 
	//syr..SESL 090613 
	//Auto Mode에서 Auto 다시 눌렀을 때 패스워드 묻지 않도록 수정. 설비 Run중 Auto, Log 모드 전환시 패스워드 묻지 않도록 수정
	//Manual, Data, Teach도 마찬가지로 Flag 설정
	{
		if (!checkPassword())
			return;
		
		/*/
//SESL_LKH_090711 : 변수명 통일
		if (m_plnkSystemData->m_bSafetySensor)
		{
			MOlbSystemPre.GetIOComponent()->OutputOff(OUT_FRONT_DOOR_LOCK_OPEN);
			MOlbSystemPre.GetIOComponent()->OutputOff(OUT_REAR_DOOR_LOCK_OPEN);
		}
//#endif
		/*/

		if (AfxGetApp()->GetMainWnd() != NULL)
		{
			CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
			pFrm->SwitchMainViews(pFrm->m_nView[DEF_AUTO_BTN]);
			pFrm->SendMessage(WM_SWITCH_VIEW);
			
			//syr..SESL 090613
			m_flgAutoSelect =  TRUE; 
			m_flgManualSelect = FALSE;
			m_flgDataSelect = FALSE;
			m_flgTeachSelect = FALSE;
		}
	}
	/*101001.KKY_____
	else
	{
		if (AfxGetApp()->GetMainWnd() != NULL)
		{
			CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
			pFrm->SwitchMainViews(pFrm->m_nView[DEF_AUTO_BTN]);
			pFrm->SendMessage(WM_SWITCH_VIEW);	
		}
	}
	*/
}

void CBottomForm::OnClickManual()
{
	if (m_flgManualSelect != TRUE) 
	{
		if (!checkPassword())
			return;
		
		/*
//SESL_LKH_090711 : 변수명 통일
		//110929 SJ_KJS
		if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
		{
			if (m_plnkSystemData->m_bSafetySensor)
				MOlbSystemPre.GetIOComponent()->OutputOff(OUT_DOOR_LOCK_OPENCLOSE);
		}
//#endif
*/
		
		if (AfxGetApp()->GetMainWnd() != NULL)
		{
			CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
			pFrm->SwitchMainViews(pFrm->m_nView[DEF_MANUAL_BTN]);
			pFrm->SendMessage(WM_SWITCH_VIEW);
			
			m_flgAutoSelect =  FALSE;
			m_flgManualSelect = TRUE;
			m_flgDataSelect = FALSE;
			m_flgTeachSelect = FALSE;
		}
	}
	/*101001.KKY_____
	else
	{
		if (AfxGetApp()->GetMainWnd() != NULL)
		{
			CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
			pFrm->SwitchMainViews(pFrm->m_nView[DEF_MANUAL_BTN]);
			pFrm->SendMessage(WM_SWITCH_VIEW);		
		}
	}
	*/	
}

void CBottomForm::OnClickData() 
{
	if (m_flgDataSelect != TRUE) 
	{
		if (!checkPassword())
			return;
		
		/*
//SESL_LKH_090711 : 변수명 통일
		//110929 SJ_KJS
		if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
		{
			if (m_plnkSystemData->m_bSafetySensor)
				MOlbSystemPre.GetIOComponent()->OutputOff(OUT_DOOR_LOCK_OPENCLOSE);
		}
//#endif
		*/
		
		if (AfxGetApp()->GetMainWnd() != NULL)
		{
			CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
			pFrm->SwitchMainViews(pFrm->m_nView[DEF_DATA_BTN]);
			pFrm->SendMessage(WM_SWITCH_VIEW);
			
			m_flgAutoSelect =  FALSE;
			m_flgManualSelect = FALSE;
			m_flgDataSelect = TRUE;
			m_flgTeachSelect = FALSE;
		}
	}
	/*101001.KKY_____
	else
	{
		if (AfxGetApp()->GetMainWnd() != NULL)
		{
			CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
			pFrm->SwitchMainViews(pFrm->m_nView[DEF_DATA_BTN]);
			pFrm->SendMessage(WM_SWITCH_VIEW);
		}
	}
	*/
}


void CBottomForm::OnClickTeach() 
{
	if (m_flgTeachSelect != TRUE) 
	{
		if (!checkPassword())
			return;

		/*
//SESL_LKH_090711 : 변수명 통일
		//110929 SJ_KJS
		if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
		{
			if (m_plnkSystemData->m_bSafetySensor)
				MOlbSystemPre.GetIOComponent()->OutputOff(OUT_DOOR_LOCK_OPENCLOSE);
		}
//#endif
*/
		
		if (AfxGetApp()->GetMainWnd() != NULL)
		{
			CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
			pFrm->SwitchMainViews(pFrm->m_nView[DEF_TEACH_BTN]);
			pFrm->SendMessage(WM_SWITCH_VIEW);
			
			m_flgAutoSelect =  FALSE;
			m_flgManualSelect = FALSE;
			m_flgDataSelect = FALSE;
			m_flgTeachSelect = TRUE;
		}
	}
	/*101001.KKY_____
	else
	{
		if (AfxGetApp()->GetMainWnd() != NULL)
		{
			CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
			pFrm->SwitchMainViews(pFrm->m_nView[DEF_TEACH_BTN]);
			pFrm->SendMessage(WM_SWITCH_VIEW);
		}
	}
	*/
}
void CBottomForm::OnClickLog() 
{
	if (AfxGetApp()->GetMainWnd() != NULL)
	{
		CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		pFrm->SwitchMainViews(pFrm->m_nView[DEF_LOG_BTN]);
		pFrm->SendMessage(WM_SWITCH_VIEW);

		m_flgAutoSelect =  FALSE;
		m_flgManualSelect = FALSE;
		m_flgDataSelect = FALSE;
		m_flgTeachSelect = FALSE;
	}
}

void CBottomForm::OnClickHelp() 
{
//	HtmlHelp(GetDesktopWindow()->m_hWnd, HELP_FILE_PATH, HH_DISPLAY_TOPIC, NULL);
	if (IDNO == MyMessageBox(_T("Would you like to write debug information to isolate the problem?\r\n원활한 문제 파악을 위해 디버그 정보를 작성하시겠습니까?"), _T("Confirm"), M_ICONQUESTION|M_YESNO))
		return;
	MDebug::CreateUserDump();
}

void CBottomForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	m_uiTimerID = SetTimer( 1, 300, NULL);

	m_pTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();

	// TODO: Add your specialized code here and/or call the base class
	OnClickAuto();
	((CBtnEnh *)GetDlgItem(IDC_AUTO))->SetValue(TRUE);
	if (MOlbSystemPre.IsMaterialDoorLock(DEF_FRONT_GROUP) == FALSE)
	{
		m_lblDoorLock.SetBackColorInterior(RGB(255,0,0));
		m_lblDoorLock.SetCaption(_T("Door Lock \n Off"));
	}
	else 
	{
		m_lblDoorLock.SetBackColorInterior(RGB(170,255,85));
		m_lblDoorLock.SetCaption(_T("Door Lock \n On"));
	}
	if (MOlbSystemPre.IsMaterialDoorLock(DEF_REAR_GROUP) == FALSE)
	{
		m_lblDoorLock2.SetBackColorInterior(RGB(255,0,0));
		m_lblDoorLock2.SetCaption(_T("Door Lock \n Off"));
	}
	else 
	{
		m_lblDoorLock2.SetBackColorInterior(RGB(170,255,85));
		m_lblDoorLock2.SetCaption(_T("Door Lock \n On"));
	}

	if (MOlbSystemPre.IsAllDoorLock(DEF_FRONT_GROUP) == FALSE)
	{
		m_lblDoorLock3.SetBackColorInterior(RGB(255,0,0));
		m_lblDoorLock3.SetCaption(_T("Front Door Lock \n Off"));
	}
	else 
	{
		m_lblDoorLock3.SetBackColorInterior(RGB(170,255,85));
		m_lblDoorLock3.SetCaption(_T("Front Door Lock \n On"));
	}
	

	if (MOlbSystemPre.IsAllDoorLock(DEF_REAR_GROUP) == FALSE)
	{
		m_lblDoorLock4.SetBackColorInterior(RGB(255,0,0));
		m_lblDoorLock4.SetCaption(_T("Rear Door Lock \n Off"));
	}
	else 
	{
		m_lblDoorLock4.SetBackColorInterior(RGB(170,255,85));
		m_lblDoorLock4.SetCaption(_T("Rear Door Lock \n On"));
	}
}

void CBottomForm::OnAutoRun() 
{
	//101001.KKY_____
	/*
	m_btnBottom[DEF_MANUAL_BTN].EnableWindow(FALSE);
	m_btnBottom[DEF_DATA_BTN].EnableWindow(FALSE);
	m_btnBottom[DEF_TEACH_BTN].EnableWindow(FALSE);
	m_btnBottom[DEF_HELP_BTN].EnableWindow(FALSE);
	*/
	m_btnBottom[DEF_TEACH_BTN].EnableWindow(FALSE);
	m_btnBottom[DEF_HELP_BTN].EnableWindow(FALSE);
	//_______________

	m_lblDoorLock.EnableWindow(FALSE);
	m_lblDoorLock2.EnableWindow(FALSE);
	m_lblDoorLock3.EnableWindow(FALSE);
	m_lblDoorLock4.EnableWindow(FALSE);
}

void CBottomForm::OnManual()
{
	m_btnBottom[DEF_MANUAL_BTN].EnableWindow(TRUE);
	m_btnBottom[DEF_DATA_BTN].EnableWindow(TRUE);
	m_btnBottom[DEF_TEACH_BTN].EnableWindow(TRUE);
	m_btnBottom[DEF_HELP_BTN].EnableWindow(TRUE);

	m_lblDoorLock.EnableWindow(TRUE);
	m_lblDoorLock2.EnableWindow(TRUE);
	m_lblDoorLock3.EnableWindow(TRUE);
	m_lblDoorLock4.EnableWindow(TRUE);

}

BOOL CBottomForm::checkPassword()
{
// 2009.05.08 CYJ ver3에서 Locking Type Door Switch 사용의 경우에는 무조건 Check,기존 설비는 Operator 아니면 통과
	if (OPERATOR_GROUP != MOlbSystemPre.GetCurrentUserGroup())
		return TRUE;

#ifdef DEF_SETUP
	return TRUE;
#endif

	WCHAR getbuf[128];
	if (!GetValueBox(getbuf, 10, _T(""), _T("Input Password. (Max 10)"), TRUE))
		return FALSE;
	
	// jdy sesl	CString szTemp=decodePassword(m_plnkSystemData->m_strOperatorPassWord);
	CString szTemp = CCommonSelectUserGroupDlg::DecodePassword(m_plnkSystemData->m_strOperatorPassWord);
	
	if (wcscmp(szTemp.GetBuffer(szTemp.GetLength()), getbuf))
	{
		//Operator Password 가 일치 하지 않습니다.
		MyMessageBox(MY_MESSAGE_BOX,60000, _T("Password Error"), M_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

/* jdy sesl
CString	CBottomForm::encodePassword(CString strCode)
{
	int iTemp;
	WCHAR szCode[128];
	CString strResult = "";
	CString strTemp;

	int iLength = strCode.GetLength();
	if (iLength <= 0 || iLength >= 128)
		return "";

	wcscpy(szCode, strCode.GetBuffer(strCode.GetLength()));

	for (int i=0; i<iLength; i++)
	{
		iTemp = szCode[i];
		strTemp.Format(_T("%02X"), iTemp);
		strResult += strTemp;
	}

	return strResult;
}
	
CString	CBottomForm::decodePassword(CString strCode)
{
	int iTemp;
	char szCode[128];
	CString strResult = "";
	CString strTemp;

	int iLength = strCode.GetLength();
	if (iLength <= 0 || iLength >= 128)
		return "";

	if (iLength % 2 != 0)
		return "";

	for (int i = 0, j = 0; i < iLength; i+=2, j++)
	{
		swscanf(strCode.Mid(i,2), _T("%X"), &iTemp);
		szCode[j] = (char)iTemp;
	}

	szCode[j] = NULL;
	strResult.Format(_T("%s"), szCode);
	return strResult;
}
*/

void CBottomForm::OnClickDoorLock() 
{
	// TODO: Add your control notification handler code here
	int iResult = 0;
	if (MOlbSystemPre.IsMaterialDoorLock(DEF_FRONT_GROUP) == FALSE)
	{
		if (TRUE == m_pTrsAutoManager->IsMeterialDoorOpenFront())
		{
			MyMessageBox(_T("Front Meterial Door is open. Please close the door and try again.\r\nFront Meterial Door가 열려 있습니다. Door를 닫고 재시도 하여 주세요."), _T("Confirm"));
			return;
		}

		iResult = m_pTrsAutoManager->LockMaterialFrontDoor();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
		Sleep(100);
		if (MOlbSystemPre.IsMaterialDoorLock(DEF_FRONT_GROUP) == FALSE)
		{
			MyMessageBox(_T("Front Meterial DoorLock is not. Check it please.\r\n전면부 Meterial DoorLock이 되지 않습니다. 확인하여 주세요."), _T("Confirm"));
			iResult = m_pTrsAutoManager->UnlockMaterialFrontDoor();
			return;
		}
	}
	else
	{
		iResult = m_pTrsAutoManager->UnlockMaterialFrontDoor();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

			//@KKY.130908______________
			if (iResult / 1000 == 400131)
			{
				BOOL bMotorStop[DEF_AXIS_MAX_AXIS] = {FALSE, };
				m_pTrsAutoManager->IsAllMotorStop(bMotorStop);
				CString strMotorName;
				for (int i=0; i<DEF_AXIS_MAX_AXIS; i++)
				{
					if (bMotorStop[i] == FALSE)
					{
						strMotorName = MOlbSystemPre.GetMotionComponent(i)->GetAxisName();
						break;
					}
				}
				CString strText;
				strText.Format(_T("%s Motor가 정지상태가 아닙니다.\n%s Motor is not StopState."), strMotorName, strMotorName);
				MyMessageBox(strText, _T("Confirm"));
			}
			//@________________________
		}
		m_pTrsAutoManager->WriteMonitoringLog("Manual 상태 에서 Door Lock 버튼을 Off 하였습니다.");
	}	
}

void CBottomForm::OnClickDoorLock2() 
{
	// TODO: Add your control notification handler code here
	int iResult = 0;
	if (MOlbSystemPre.IsMaterialDoorLock(DEF_REAR_GROUP)  == FALSE)
	{
		if (TRUE == m_pTrsAutoManager->IsMeterialDoorOpenRear())
		{
			MyMessageBox(_T("The rear door is open. Please close the door and try again.\r\n후면부 자재 Door가 열려 있습니다. Door를 닫고 재시도 하여 주세요."), _T("Confirm"));
			return;
		}
		iResult = m_pTrsAutoManager->LockMaterialRearDoor();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
		Sleep(100);
		if (MOlbSystemPre.IsMaterialDoorLock(DEF_REAR_GROUP)  == FALSE)
		{
			MyMessageBox(_T("Rear Door Material DoorLock does not work. Check it please.\r\n후면부 자재 DoorLock이 되지 않습니다. 확인하여 주세요."), _T("Confirm"));
			iResult = m_pTrsAutoManager->UnlockMeterialRearDoor();
			return;
		}
//@		m_lblDoorLock.SetBackColorInterior(RGB(170,255,85));
//@		m_lblDoorLock.SetCaption(_T("Door Lock \n On"));
	}
	else
	{
		iResult = m_pTrsAutoManager->UnlockMeterialRearDoor();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			//@KKY.130908______________
			if (iResult / 1000 == 400131)
			{
				BOOL bMotorStop[DEF_AXIS_MAX_AXIS] = {FALSE, };
				m_pTrsAutoManager->IsAllMotorStop(bMotorStop);
				CString strMotorName;
				for (int i=0; i<DEF_AXIS_MAX_AXIS; i++)
				{
					if (bMotorStop[i] == FALSE)
					{
						strMotorName = MOlbSystemPre.GetMotionComponent(i)->GetAxisName();
						break;
					}
				}
				CString strText;
				strText.Format(_T("%s Motor가 정지상태가 아닙니다.\n%s Motor is not StopState."), strMotorName, strMotorName);
				MyMessageBox(strText, _T("Confirm"));
			}
			//@________________________
		}

//@		m_lblDoorLock.SetBackColorInterior(RGB(255,0,0));
//@		m_lblDoorLock.SetCaption(_T("Door Lock \n Off"));

		m_pTrsAutoManager->WriteMonitoringLog("Manual 상태 에서 Door Lock 버튼을 Off 하였습니다.");
	}	
}

void CBottomForm::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_uiTimerID)
	{
		//131111. SJ_YTH
		if (MOlbSystemPre.IsAllDoorLock(DEF_FRONT_GROUP)  == FALSE)
		{
			m_lblDoorLock3.SetBackColorInterior(RGB(255,0,0));
			m_lblDoorLock3.SetCaption(_T("Front Door\nLock Off"));
		}
		else 
		{
			m_lblDoorLock3.SetBackColorInterior(RGB(170,255,85));
			m_lblDoorLock3.SetCaption(_T("Front Door\nLock On"));
		}

		if (MOlbSystemPre.IsAllDoorLock(DEF_REAR_GROUP)  == FALSE)
		{
			m_lblDoorLock4.SetBackColorInterior(RGB(255,0,0));
			m_lblDoorLock4.SetCaption(_T("Rear Door\nLock Off"));
		}
		else 
		{
			m_lblDoorLock4.SetBackColorInterior(RGB(170,255,85));
			m_lblDoorLock4.SetCaption(_T("Rear Door\nLock On"));
		}
		
		/*/
		//1503026 SJ_YSH
		if (MOlbSystemPre.GetIOComponent()->IsOn(IN_FRONT_DOOR_LOCK))
		{
			MOlbSystemPre.GetIOComponent()->OutputOn(OUT_FRONT_SAFETY_KEY_UNLOCK);
		}
		else
		{
			MOlbSystemPre.GetIOComponent()->OutputOff(OUT_FRONT_SAFETY_KEY_UNLOCK);
		}

		if (MOlbSystemPre.GetIOComponent()->IsOn(IN_REAR_DOOR_LOCK))
		{
			MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_SAFETY_KEY_UNLOCK);
		}
		else
		{
			MOlbSystemPre.GetIOComponent()->OutputOff(OUT_REAR_SAFETY_KEY_UNLOCK);
		}
		/*/

	}
	
	CFormView::OnTimer(nIDEvent);
}

void CBottomForm::OnClickDoorLock3() 
{
	// TODO: Add your control notification handler code here
	int iResult = 0;
	if (MOlbSystemPre.IsAllDoorLock(DEF_FRONT_GROUP) == FALSE)
	{
		if (TRUE == m_pTrsAutoManager->IsFrontDoorOpen())
		{
			MyMessageBox(_T("Front Door is open. Please close the door and try again.\r\nFront Door가 열려 있습니다. Door를 닫고 재시도 하여 주세요."), _T("Confirm"));
			return;
		}

		iResult = m_pTrsAutoManager->LockFrontDoor();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
		Sleep(200);
		if (MOlbSystemPre.IsAllDoorLock(DEF_FRONT_GROUP) == FALSE)
		{
			MyMessageBox(_T("Full DoorLock is not. Check it please.\r\n전체 DoorLock이 되지 않습니다. 확인하여 주세요."), _T("Confirm"));
			iResult = m_pTrsAutoManager->UnlockMaterialFrontDoor();
			return;
		}
	}
	else
	{
		iResult = m_pTrsAutoManager->UnlockFrontDoor();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

			//@KKY.130908______________
			if (iResult / 1000 == 400131)
			{
				BOOL bMotorStop[DEF_AXIS_MAX_AXIS] = {FALSE, };
				m_pTrsAutoManager->IsAllMotorStop(bMotorStop);
				CString strMotorName;
				for (int i=0; i<DEF_AXIS_MAX_AXIS; i++)
				{
					if (bMotorStop[i] == FALSE)
					{
						strMotorName = MOlbSystemPre.GetMotionComponent(i)->GetAxisName();
						break;

					}
				}
				CString strText;
				strText.Format(_T("%s Motor가 정지상태가 아닙니다.\n%s Motor is not StopState."), strMotorName, strMotorName);
				MyMessageBox(strText, _T("Confirm"));
			}
			//@________________________
		}
		m_pTrsAutoManager->WriteMonitoringLog("Manual 상태 에서 Door Lock 버튼을 Off 하였습니다.");
	}	
}

void CBottomForm::OnClickDoorLock4() 
{
	// TODO: Add your control notification handler code here
	int iResult = 0;
	if (MOlbSystemPre.IsAllDoorLock(DEF_REAR_GROUP) == FALSE)
	{
		if (TRUE == m_pTrsAutoManager->IsRearDoorOpen())
		{
			MyMessageBox(_T("Front Door is open. Please close the door and try again.\r\nFront Door가 열려 있습니다. Door를 닫고 재시도 하여 주세요."), _T("Confirm"));
			return;
		}

		iResult = m_pTrsAutoManager->LockRearDoor();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
		Sleep(200);
		if (MOlbSystemPre.IsAllDoorLock(DEF_REAR_GROUP) == FALSE)
		{
			MyMessageBox(_T("Rear DoorLock does not work. Check it please.\r\nRear DoorLock이 되지 않습니다. 확인하여 주세요."), _T("Confirm"));
			iResult = m_pTrsAutoManager->UnlockMaterialFrontDoor();
			return;
		}
	}
	else
	{
		iResult = m_pTrsAutoManager->UnlockRearDoor();
		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);

			//@KKY.130908______________
			if (iResult / 1000 == 400131)
			{
				BOOL bMotorStop[DEF_AXIS_MAX_AXIS] = {FALSE, };
				m_pTrsAutoManager->IsAllMotorStop(bMotorStop);
				CString strMotorName;
				for (int i=0; i<DEF_AXIS_MAX_AXIS; i++)
				{
					if (bMotorStop[i] == FALSE)
					{
						strMotorName = MOlbSystemPre.GetMotionComponent(i)->GetAxisName();
						break;

					}
				}
				CString strText;
				strText.Format(_T("%s Motor가 정지상태가 아닙니다.\n%s Motor is not StopState."), strMotorName, strMotorName);
				MyMessageBox(strText, _T("Confirm"));
			}
			//@________________________
		}
		m_pTrsAutoManager->WriteMonitoringLog("Manual 상태 에서 Door Lock 버튼을 Off 하였습니다.");
	}	
}

void CBottomForm::OnClickVerDisp() 
{
	// TODO: Add your control notification handler code here
	CVersionDisplayDlg dlg;
	dlg.DoModal();		
}
