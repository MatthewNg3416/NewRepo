// AutoMaterialCmdGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "AutoMaterialCmdGroupDlg.h"
//110225.Add_____
#include "AutoMaterialInfoDlg.h"
#include "MaterialChangeDlg.h"
#include "SystemThreeBtnDlg.h"
#include "MTrsAutoManager.h"
#include "IIO.h"
#include "DefIOAddrPreBonder.h"
//_______________

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CAutoMaterialCmdGroupDlg dialog

// selsk_jdy CAutoMaterialCmdGroupDlg::CAutoMaterialCmdGroupDlg(HWND hWnd, CWnd* pParent /*=NULL*/)
CAutoMaterialCmdGroupDlg::CAutoMaterialCmdGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoMaterialCmdGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoMaterialCmdGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// selsk_jdy m_hAutoMainWnd = hWnd;
	// selsk_jdy
	m_hAutoMainWnd = pParent->m_hWnd;
}


void CAutoMaterialCmdGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoMaterialCmdGroupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoMaterialCmdGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoMaterialCmdGroupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoMaterialCmdGroupDlg message handlers

BEGIN_EVENTSINK_MAP(CAutoMaterialCmdGroupDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoMaterialCmdGroupDlg)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_SCRAP, -600 /* Click */, OnPanelScrap, VTS_NONE)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_UNSCRAP, -600 /* Click */, OnPanelUnscrap, VTS_NONE)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_MATERIALINFO, -600 /* Click */, OnMaterialinfo, VTS_NONE)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_MATERIALCHANGE, -600 /* Click */, OnClickMaterialchange, VTS_NONE)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_MATERIALINFO_VIEW, -600 /* Click */, OnClickMaterialinfoView, VTS_NONE)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_PRODUCT_PLAN_VIEW, -600 /* Click */, OnClickProductPlanView, VTS_NONE)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_AUTO_HOMING, -600 /* Click */, OnClickAutoHoming, VTS_NONE)
	ON_EVENT(CAutoMaterialCmdGroupDlg, IDB_AUTO_HOMING_R, -600 /* Click */, OnClickAutoHomingR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAutoMaterialCmdGroupDlg::OnPanelScrap() 
{
	::PostMessage(m_hAutoMainWnd, WM_DISP_PANEL_SCRAP_DLG, NULL, NULL);
	EndDialog(0);
}

void CAutoMaterialCmdGroupDlg::OnPanelUnscrap() 
{
	::PostMessage(m_hAutoMainWnd, WM_DISP_PANEL_UNSCRAP_DLG, NULL, NULL);
	EndDialog(0);
}

void CAutoMaterialCmdGroupDlg::OnMaterialinfo() 
{
	//110225.Modify(이원준D 요청)_____
//	::PostMessage(m_hAutoMainWnd, WM_DISP_MATERIAL_INFO_DLG, NULL, NULL);
//	EndDialog(0);
	CAutoMaterialInfoDlg dlg;
	dlg.DoModal();
	//________________________________
}

void CAutoMaterialCmdGroupDlg::OnExit() 
{
	EndDialog(0);
}

BOOL CAutoMaterialCmdGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString szTextTemp;
//자재 교체
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialCmdGroupDlg_1"), &szTextTemp);
	GetDlgItem(IDB_MATERIALINFO)->SetWindowText(szTextTemp);
	
//나가기
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialCmdGroupDlg_2"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

//생산정보
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialCmdGroupDlg_3"), &szTextTemp);
	GetDlgItem(IDB_PRODUCT_PLAN_VIEW)->SetWindowText(szTextTemp);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoMaterialCmdGroupDlg::OnClickMaterialchange() 
{
	// TODO: Add your control notification handler code here
	//110225.Modify(이원준D 요청)_____	
//	::PostMessage(m_hAutoMainWnd, WM_DISP_MATERIAL_CHANGE_DLG, NULL, NULL);
//	EndDialog(0);

	int iSelWorkGroup;
	CSystemThreeBtnDlg dlg(_T("Tab IC 를 교체 하시겠습니까?"), "Front\nFeeder", "Rear\nFeeder", "Cancel");
	iSelWorkGroup = dlg.DoModal();

	if(iSelWorkGroup > 1) //Cancel....
		return;

	MOlbSystemPre.GetTrsAutoManagerComponent()->SetMaterialDoorCheck(TRUE, iSelWorkGroup); //Door Check 무효화....
	MOlbSystemPre.GetTrsAutoManagerComponent()->m_bFeederChange = TRUE;

	/*/
	if(MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE) //Auto Run 아니면 굳이 일일이 Door Lock 풀어줄 필요 없음...
	{
		if(iSelWorkGroup == DEF_FRONT_GROUP)
			MOlbSystemPre.GetIOComponent()->OutputOn(OUT_FRONT_DOOR_LOCK_OPEN); //101124
		else if (iSelWorkGroup == DEF_REAR_GROUP)
			MOlbSystemPre.GetIOComponent()->OutputOn(OUT_REAR_DOOR_LOCK_OPEN); //101124
	}
	/*/
	CMaterialChangeDlg dlg2(iSelWorkGroup);
	dlg2.DoModal();

	/*/
	//________________________________
	if(MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE) //Auto Run 아니면 굳이 일일이 Door Lock 풀어줄 필요 없음...
	{
		if(iSelWorkGroup == DEF_FRONT_GROUP)
			MOlbSystemPre.GetIOComponent()->OutputOff(OUT_FRONT_DOOR_LOCK_OPEN); //101124
		else if (iSelWorkGroup == DEF_REAR_GROUP)
			MOlbSystemPre.GetIOComponent()->OutputOff(OUT_REAR_DOOR_LOCK_OPEN); //101124
	}
	//________________________________
	/*/
}

void CAutoMaterialCmdGroupDlg::OnClickMaterialinfoView() 
{
	// TODO: Add your control notification handler code here
	::PostMessage(m_hAutoMainWnd, WM_SHOW_MATERIAL_CODEINFO_DLG, NULL, NULL);
	EndDialog(0);
}

//110624___________
void CAutoMaterialCmdGroupDlg::OnClickProductPlanView() 
{
	// TODO: Add your control notification handler code here
	::PostMessage(m_hAutoMainWnd, WM_SHOW_PRODUCT_PLAN_DLG, NULL, NULL);
	EndDialog(0);
}
//_________________

void CAutoMaterialCmdGroupDlg::OnClickAutoHoming() 
{
	// TODO: Add your control notification handler code here
	::PostMessage(m_hAutoMainWnd, WM_START_TAB_FEEDER_AUTO_HOMING_REQ, DEF_TABFEEDER1, NULL);
	EndDialog(0);	
}

void CAutoMaterialCmdGroupDlg::OnClickAutoHomingR() 
{
	// TODO: Add your control notification handler code here
	::PostMessage(m_hAutoMainWnd, WM_START_TAB_FEEDER_AUTO_HOMING_REQ, DEF_TABFEEDER2, NULL);
	EndDialog(0);	
}
