// DataMainForm1.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "DataMainForm.h"
#include "DataModelDlg.h"
#include "DataModelDlgNew.h"
#include "DataPanelDlg.h"
//20110223 SJ_HJG
#include "DataTowerLampDlg.h"

#include "DataActuatorSettingDlg.h"
#include "DataTabICDlg.h"
#include "DataCommonDlg.h"
#include "DataTabICOffsetDlg.h"
#include "DataFuncOnOffDlg.h"
#include "DataTabMounterDlg.h"
#include "DataToolOffsetDlg.h"
#include "DataPanelAlignerDlg.h"
// #include "DataConveyorSpeedDlg.h"
#include "DataMoldInfoDlg.h"
#include "DataModelChangeDistDlg.h"
// 100807. ECID ___________
#include "DataECDBDlg.h"

#include "IVision.h"

#include "MTrsAutoManager.h"

#include "TeachBadTabDetectorDlg.h"
#include "PlateVacModelDataDlg.h"

//130320.KMS________
#include "ErrorNotSendDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataMainForm

IMPLEMENT_DYNCREATE(CDataMainForm, CFormView)

CDataMainForm::CDataMainForm()
	: CFormView(CDataMainForm::IDD)
{
	//{{AFX_DATA_INIT(CDataMainForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	//101001.KKY_____
	m_bInitialized = FALSE;
	//_______________
}

CDataMainForm::~CDataMainForm()
{
}

void CDataMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataMainForm)
	DDX_Control(pDX, IDC_MODEL_SET, m_lblModel);
	DDX_Control(pDX, IDC_DELAYTIME_SET, m_lblDelay);
	DDX_Control(pDX, IDC_MOTION_SET, m_lblMotion);
	DDX_Control(pDX, IDC_PROCESS_SET, m_lblProcess);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataMainForm, CFormView)
	//{{AFX_MSG_MAP(CDataMainForm)
	ON_WM_PAINT()
	ON_MESSAGE(WM_DISP_MULTI_LANGUAGE_DLG_REQ, OnDisplayLanguage)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataMainForm diagnostics

#ifdef _DEBUG
void CDataMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CDataMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDataMainForm message handlers

void CDataMainForm::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CFormView::OnPaint() for painting messages
}

BEGIN_EVENTSINK_MAP(CDataMainForm, CFormView)
    //{{AFX_EVENTSINK_MAP(CDataMainForm)
	ON_EVENT(CDataMainForm, IDB_MODEL_DATA, -600 /* Click */, OnModelData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_PANEL_DATA, -600 /* Click */, OnPanelData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_RUN_VELOCITY, -600 /* Click */, OnRunVelocity, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_SW_LIMIT, -600 /* Click */, OnSwLimit, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_TOLERANCE, -600 /* Click */, OnTolerance, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_ORIGIN_RETURN, -600 /* Click */, OnOriginReturn, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_JOG_PITCH, -600 /* Click */, OnJogPitch, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_MOTOR_DELAY, -600 /* Click */, OnMotorDelay, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_CYLINDER_DELAY, -600 /* Click */, OnCylinderDelay, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_VACUUM_DELAY, -600 /* Click */, OnVacuumDelay, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_TAB_OFFSET, -600 /* Click */, OnTabOffset, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_FUND_ONOFF, -600 /* Click */, OnClickFundOnoff, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_TAB_DATA1, -600 /* Click */, OnClickTabData1, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_TAB_MOUNTER_ALIGNER_DATA, -600 /* Click */, OnTabMounterData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_PANEL_ALIGNER_DATA, -600 /* Click */, OnPanelAlignerData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_DATA_BACKUP, -600 /* Click */, OnClickDataBackup, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_COMMON_DATA, -600 /* Click */, OnCommonData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_TOOL_OFFSET_DATA, -600 /* Click */, OnClickToolOffsetData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_CONVEYOR_SPEED_DATA, -600 /* Click */, OnConveyorSpeedData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_MATERIAL_DATA, -600 /* Click */, OnClickMaterialData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_MATERIAL_DATA2, -600 /* Click */, OnClickMaterialData2, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_MODEL_CHANGE, -600 /* Click */, OnModelChangeDistData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_DATA_DB_EDIT, -600 /* Click */, OnClickDataDbEdit, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_HOME_TEACHING, -600 /* Click */, OnClickHomeTeaching, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_TOWER_LAMP_DATA, -600 /* Click */, OnClickTowerLampData, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_ACF_PLATE_VACUUM_MODELDATA, -600 /* Click */, OnClickAcfPlateVacuumModeldata, VTS_NONE)
	ON_EVENT(CDataMainForm, IDB_ERROR_DATA, -600 /* Click */, OnClickErrorData, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataMainForm::OnModelData() 
{
	// TODO: Add your control notification handler code here
//	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
//	{
//		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
//		MyMessageBox(MY_MESSAGE_BOX, 40400,_T("Login Authority Information"), M_ICONINFORMATION);
//		return;
//	}

// 2011.12.14 KWC
	CDataModelDlgNew dlg;
	dlg.DoModal();
}

void CDataMainForm::OnPanelData() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40401,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	//101004.KKY_____
//	CDataPanelDlg dlg;
	BOOL bCurrentModel = FALSE;
	BOOL bEditable = FALSE;
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
	{
		bCurrentModel = TRUE;
		bEditable = TRUE;
	}
	else
	{
		bCurrentModel = TRUE;
		bEditable = FALSE;
	}
	CDataPanelDlg dlg(bCurrentModel, bEditable);
	//_______________
	dlg.DoModal();
}

void CDataMainForm::OnRunVelocity() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40402,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataActuatorSettingDlg dlg(0);
	dlg.DoModal();
}

void CDataMainForm::OnSwLimit() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40403,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataActuatorSettingDlg dlg(2);
	dlg.DoModal();
}

void CDataMainForm::OnTolerance() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40404,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataActuatorSettingDlg dlg(4);
	dlg.DoModal();
}

void CDataMainForm::OnOriginReturn() 
{
	// jdy2
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40405,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataActuatorSettingDlg dlg(1);
	dlg.DoModal();
}

void CDataMainForm::OnJogPitch() 
{
	// jdy2
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40406,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataActuatorSettingDlg dlg(3);
	dlg.DoModal();
}

void CDataMainForm::OnMotorDelay() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40407,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataActuatorSettingDlg dlg(5);
	dlg.DoModal();
}

void CDataMainForm::OnCylinderDelay() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40408,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataActuatorSettingDlg dlg(6);
	dlg.DoModal();
}

void CDataMainForm::OnVacuumDelay() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40409,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataActuatorSettingDlg dlg(7);
	dlg.DoModal();
}

void CDataMainForm::OnTabMounterData() 
{
	// jdy2
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40410,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataTabMounterDlg dlg;
	dlg.DoModal();
}

void CDataMainForm::OnPanelAlignerData() 
{
	// jdy2
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40411,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataPanelAlignerDlg dlg;
	dlg.DoModal();
}

void CDataMainForm::OnTabOffset() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40412,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataTabICOffsetDlg dlg;
	dlg.DoModal();
}

void CDataMainForm::OnClickFundOnoff() 
{
	// jdy2
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40413,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CDataFuncOnOffDlg dlg;
	dlg.DoModal();	
}

void CDataMainForm::OnClickTabData1() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40413,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	//101004.KKY_____
//	CDataTabICDlg dlg(0);
	BOOL bCurrentModel = FALSE;
	BOOL bEditable = FALSE;
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
	{
		bCurrentModel = TRUE;
		bEditable = TRUE;
	}
	else
	{
		bCurrentModel = TRUE;
		bEditable = FALSE;
	}
	CDataTabICDlg dlg(bCurrentModel, bEditable, 0);	
	//_______________
	dlg.DoModal();	
}

void CDataMainForm::OnClickDataBackup() 
{
	// DataBackup folder 없는 경우, folder 생성
	CFileFind finder;
	if (!finder.FindFile(_T("C:\\DataBackup")))
		CreateDirectory(_T("C:\\DataBackup"), NULL);

	// selsk_jdy
	if (!finder.FindFile(_T("D:\\DataBackup")))
		CreateDirectory(_T("D:\\DataBackup"), NULL);

	MFileManager::AutoDeleteFile(_T("C:\\DataBackup\\*.*"), 50, TRUE);
	// selsk_jdy
	MFileManager::AutoDeleteFile(_T("D:\\DataBackup\\*.*"), 50, TRUE);

	CTime t = CTime::GetCurrentTime();
	WCHAR dest[256];
	swprintf(dest, _T("C:\\DataBackup\\%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute());
	MFileManager::CreateBackupFolder(dest, _T(".\\Data"), TRUE);
	// selsk_jdy
	swprintf(dest, _T("D:\\DataBackup\\%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute());
	MFileManager::CreateBackupFolder(dest, _T(".\\Data"), TRUE);

	MOlbSystemPre.GetVisionComponent()->DeleteOldImageFiles(_T("..\\LogImage"));

	//Data Backup 을 완료했습니다.
	MyMessageBox(MY_MESSAGE_BOX, 40414,_T("Data Backup"), M_ICONINFORMATION);
}

void CDataMainForm::OnCommonData() 
{
	// jdy2
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40415,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataCommonDlg dlg;
	dlg.DoModal();
}

void CDataMainForm::OnClickToolOffsetData() 
{
	// jdy2
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40416,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataToolOffsetDlg dlg;
	dlg.DoModal();
}

void CDataMainForm::OnConveyorSpeedData()
{
#if FALSE
	// jdy2
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40417,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataConveyorSpeedDlg dlg;
	dlg.DoModal();
#endif
}

void CDataMainForm::OnClickMaterialData() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40418,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CDataMoldInfoDlg dlg(DEF_TABFEEDER1);
	dlg.DoModal();		
}	


void CDataMainForm::OnClickMaterialData2() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40419,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CDataMoldInfoDlg dlg(DEF_TABFEEDER2);
	dlg.DoModal();	
}

void CDataMainForm::OnModelChangeDistData() 
{
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
	{
		//이 작업을 위해서는 Maker 권한이 필요합니다.
		MyMessageBox(MY_MESSAGE_BOX, 40420,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	CDataModelChangeDistDlg dlg;
	dlg.DoModal();	
}

void CDataMainForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if (SYSTEM_TYPE_SOURCE == MOlbSystemPre.GetSystemData()->m_eSystemType)
	{
		GetDlgItem(IDB_MATERIAL_DATA2)->EnableWindow(FALSE);
		GetDlgItem(IDB_MATERIAL_DATA2)->ShowWindow(SW_HIDE);
	}
	// ML_hongju
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetDataViewHandle(m_hWnd);
	this->SendMessage(WM_DISP_MULTI_LANGUAGE_DLG_REQ);
	//101001.KKY_____
	m_bInitialized = TRUE;
	//_______________
}

LRESULT CDataMainForm::OnDisplayLanguage(UINT wParam, LPARAM lParam)
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;

//	모델 설정
	SetTitleLT(&m_lblModel, MAIN_DATA, _T("DataMainForm_1"));
//	지연시간 설정
	SetTitleLT(&m_lblDelay, MAIN_DATA, _T("DataMainForm_2"));
//	모션 설정
	SetTitleLT(&m_lblMotion, MAIN_DATA, _T("DataMainForm_3"));
//	공정 설정
	SetTitleLT(&m_lblProcess, MAIN_DATA, _T("DataMainForm_4"));

	ReadDisplayMessage(MAIN_DATA, _T("DataMainForm_5"), &szTextTemp);
//	속도/가감속 시간
	GetDlgItem(IDB_RUN_VELOCITY)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataMainForm_6"), &szTextTemp);
//	원점복귀\r\n속도/가감속 시간
	GetDlgItem(IDB_ORIGIN_RETURN)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataMainForm_7"), &szTextTemp);
//	Jog 속도
	GetDlgItem(IDB_JOG_PITCH)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataMainForm_8"), &szTextTemp);
//	기능 On/Off
	GetDlgItem(IDB_FUND_ONOFF)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataMainForm_9"), &szTextTemp);
//	금형 1 관리
	GetDlgItem(IDB_MATERIAL_DATA)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataMainForm_10"), &szTextTemp);
//	금형 2 관리
	GetDlgItem(IDB_MATERIAL_DATA2)->SetWindowText(szTextTemp);
	return 0;
}

void CDataMainForm::OnClickDataDbEdit() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40412,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}
	
	// 100807. ECID ___________
	CDataECDBDlg	dlg;
	dlg.DoModal();		
}

//101001.KKY_____
void CDataMainForm::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if (TRUE == m_bInitialized)
	{
		if (FALSE == bShow)
		{
			KillTimer(10);
		}
		else
		{
			m_eOldOpMode = (EOpMode)(-1);
			SetTimer(10, 500, NULL);
		}
	}	
}

void CDataMainForm::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 10)
	{
		if (m_eOldOpMode != MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode())
		{
			if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
			{
				showButtonManual();
			}
			else
			{
				showButtonAuto();
			}
		}
		m_eOldOpMode = MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode();
	}

	CFormView::OnTimer(nIDEvent);
}

void CDataMainForm::showButtonManual()
{
	GetDlgItem(IDB_MODEL_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_PANEL_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_TAB_DATA1)->EnableWindow(TRUE);
	GetDlgItem(IDB_TAB_OFFSET)->EnableWindow(TRUE);
	GetDlgItem(IDB_ACF_PLATE_VACUUM_MODELDATA)->EnableWindow(TRUE);

	GetDlgItem(IDB_MOTOR_DELAY)->EnableWindow(TRUE);
	GetDlgItem(IDB_CYLINDER_DELAY)->EnableWindow(TRUE);
	GetDlgItem(IDB_VACUUM_DELAY)->EnableWindow(TRUE);

	GetDlgItem(IDB_RUN_VELOCITY)->EnableWindow(TRUE);
	GetDlgItem(IDB_ORIGIN_RETURN)->EnableWindow(TRUE);
	GetDlgItem(IDB_SW_LIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDB_JOG_PITCH)->EnableWindow(TRUE);
	GetDlgItem(IDB_TOLERANCE)->EnableWindow(TRUE);

	GetDlgItem(IDB_MATERIAL_DATA)->EnableWindow(TRUE);
//	GetDlgItem(IDB_MATERIAL_DATA2)->EnableWindow(TRUE);

	GetDlgItem(IDB_FUND_ONOFF)->EnableWindow(TRUE);
	GetDlgItem(IDB_DATA_DB_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDB_DATA_BACKUP)->EnableWindow(TRUE);

	GetDlgItem(IDB_COMMON_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_PANEL_ALIGNER_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_TAB_MOUNTER_ALIGNER_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_MODEL_CHANGE)->EnableWindow(TRUE);
}

void CDataMainForm::showButtonAuto()
{
	GetDlgItem(IDB_MODEL_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_PANEL_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_TAB_DATA1)->EnableWindow(TRUE);
	GetDlgItem(IDB_TAB_OFFSET)->EnableWindow(TRUE);
	GetDlgItem(IDB_ACF_PLATE_VACUUM_MODELDATA)->EnableWindow(TRUE);

	GetDlgItem(IDB_MOTOR_DELAY)->EnableWindow(TRUE);
	GetDlgItem(IDB_CYLINDER_DELAY)->EnableWindow(TRUE);
	GetDlgItem(IDB_VACUUM_DELAY)->EnableWindow(TRUE);

	GetDlgItem(IDB_RUN_VELOCITY)->EnableWindow(TRUE);
	GetDlgItem(IDB_ORIGIN_RETURN)->EnableWindow(TRUE);
	GetDlgItem(IDB_SW_LIMIT)->EnableWindow(TRUE);
	GetDlgItem(IDB_JOG_PITCH)->EnableWindow(TRUE);
	GetDlgItem(IDB_TOLERANCE)->EnableWindow(TRUE);

	GetDlgItem(IDB_MATERIAL_DATA)->EnableWindow(FALSE);
//	GetDlgItem(IDB_MATERIAL_DATA2)->EnableWindow(TRUE);

	GetDlgItem(IDB_FUND_ONOFF)->EnableWindow(TRUE);
	GetDlgItem(IDB_DATA_DB_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDB_DATA_BACKUP)->EnableWindow(FALSE);

	GetDlgItem(IDB_COMMON_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_PANEL_ALIGNER_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_TAB_MOUNTER_ALIGNER_DATA)->EnableWindow(TRUE);
	GetDlgItem(IDB_MODEL_CHANGE)->EnableWindow(TRUE);
}
//_______________

void CDataMainForm::OnClickHomeTeaching() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40400,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	if(MyMessageBox(MY_MESSAGE_BOX, 90003, _T("Home Teaching!!"), M_ICONQUESTION|M_YESNO, _T("Front"), _T("Rear")) == IDYES)
	{
		//Front
		CTeachBadTabDetectorDlg dlg(DEF_MODEL_MODE, DEF_FRONT_GROUP);
		dlg.DoModal();
	}
	else
	{
		//Rear
		CTeachBadTabDetectorDlg dlg(DEF_MODEL_MODE, DEF_REAR_GROUP);
		dlg.DoModal();
	}
}

void CDataMainForm::OnClickTowerLampData() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40401,_T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CDataTowerLampDlg dlg;
	//_______________
	dlg.DoModal();
	
}

void CDataMainForm::OnClickAcfPlateVacuumModeldata() 
{
	// TODO: Add your control notification handler code here
	if (MOlbSystemPre.GetCurrentUserGroup() == OPERATOR_GROUP)
	{
		//현재 사용자의 권한으로는 이 작업을 수행할 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40401, _T("Login Authority Information"), M_ICONINFORMATION);
		return;
	}

	CPlateVacModelDataDlg dlg;
	dlg.DoModal();	
}

//130320.KMS________
void CDataMainForm::OnClickErrorData() 
{
	// TODO: Add your control notification handler code here
	CErrorNotSendDlg dlg;
	dlg.DoModal();
}
