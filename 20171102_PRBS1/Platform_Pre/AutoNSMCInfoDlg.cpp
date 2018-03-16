// CAutoNSMCInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
//NSMC NSMCdlg 추가
#include "AutoNSMCInfoDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTabFeeder.h"
#include "MTrsAutoManager.h"

#include "DataModelIDList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoNSMCInfoDlg dialog
extern	MPlatformOlbSystem	MOlbSystemPre;


CAutoNSMCInfoDlg::CAutoNSMCInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoNSMCInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoNSMCInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
//	m_plnkTabFeederOperationData1 = MOlbSystemPre.GetTabFeederComponent(0)->GetTabFeederOperationData();
//	m_plnkTabFeederOperationData2 = MOlbSystemPre.GetTabFeederComponent(1)->GetTabFeederOperationData();

	for (int i=0; i<DEF_MAX_NSMC_INFO; i++)
		m_rgstrNSMCInfo[i].Empty();

	m_uiTimerID = 0;
	m_pNSMCData = m_plnkTrsAutoManager->GetCurrentNSMCInfo();

	// hongju_MC
	m_strNewModelID = _T("NONE");

}


void CAutoNSMCInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoNSMCInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_NSMC_INFO0,	m_sNSMCInfo[0]);
	DDX_Control(pDX, IDC_NSMC_INFO1,	m_sNSMCInfo[1]);
	DDX_Control(pDX, IDC_NSMC_INFO2,	m_sNSMCInfo[2]);
	DDX_Control(pDX, IDC_NSMC_INFO3,	m_sNSMCInfo[3]);
	DDX_Control(pDX, IDC_NSMC_INFO4,	m_sNSMCInfo[4]);
	DDX_Control(pDX, IDC_NSMC_INFO5,	m_sNSMCInfo[5]);
	DDX_Control(pDX, IDC_NSMC_INFO6,	m_sNSMCInfo[6]);
	DDX_Control(pDX, IDC_NSMC_INFO7,	m_sNSMCInfo[7]);
	DDX_Control(pDX, IDC_NSMC_INFO8,	m_sNSMCInfo[8]);
	DDX_Control(pDX, IDC_NSMC_INFO9,	m_sNSMCInfo[9]);
	DDX_Control(pDX, IDC_NSMC_INFO10,	m_sNSMCInfo[10]);
	DDX_Control(pDX, IDC_NSMC_INFO11,	m_sNSMCInfo[11]);
	DDX_Control(pDX, IDC_NSMC_INFO12,	m_sNSMCInfo[12]);
	DDX_Control(pDX, IDC_NSMC_INFO13,	m_sNSMCInfo[13]);
	DDX_Control(pDX, IDC_NSMC_INFO14,	m_sNSMCInfo[14]);
	DDX_Control(pDX, IDC_NSMC_INFO15,	m_sNSMCInfo[15]);
}


BEGIN_MESSAGE_MAP(CAutoNSMCInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoNSMCInfoDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoNSMCInfoDlg message handlers

void CAutoNSMCInfoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_rgstrNSMCInfo[0].Format(_T("%d"),m_plnkSystemData->m_bUseAutoModelChange);
	m_rgstrNSMCInfo[1].Format(_T("%d"),m_plnkSystemData->m_bUseMaterialDeviceID);
	m_rgstrNSMCInfo[2].Format(_T("%d"),m_plnkTrsAutoManager->IsLastGlassInput());
	//hongju_MC : 수동 Model Change시만 필요없음..
	m_rgstrNSMCInfo[3] = m_plnkTrsAutoManager->GetNewModelName();
	//111130.kms___________
	if(m_plnkSystemData->m_strCurrentDeviceID == m_plnkTrsAutoManager->m_strNewDeviceID)
		m_rgstrNSMCInfo[4] = "";
	else
		m_rgstrNSMCInfo[4] = m_plnkTrsAutoManager->m_strNewDeviceID;
//	m_rgstrNSMCInfo[4] = m_pNSMCData->m_strDeviceID;
	//_____________________
	m_rgstrNSMCInfo[5] = m_plnkSystemData->m_strModelFileName;
	m_rgstrNSMCInfo[6] = m_plnkSystemData->m_strCurrentDeviceID;
	m_rgstrNSMCInfo[7].Format(_T("%d"),m_plnkTrsAutoManager->GetModelChangeStart());
	m_rgstrNSMCInfo[8] = m_pNSMCData->m_strEQState;
	m_rgstrNSMCInfo[9] = m_pNSMCData->m_strReplyCode;
//	m_rgstrNSMCInfo[10] = m_pNSMCData->m_strPanelWIP;

	for (int i=0; i<DEF_MAX_NSMC_INFO; i++)
		m_sNSMCInfo[i].SetCaption(m_rgstrNSMCInfo[i]);
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CAutoNSMCInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTemp;

	ReadDisplayMessage(MAIN_AUTO, _T("AutoNSMCInfoDlg_1"), &strTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(strTemp);
	
	// TODO: Add extra initialization here
	if (m_uiTimerID == 0)
		m_uiTimerID = SetTimer(10, 500, NULL);

	//hongju_MC : 수동 Model Change시만 필요없음..
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CAutoNSMCInfoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoNSMCInfoDlg)
	ON_EVENT(CAutoNSMCInfoDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CAutoNSMCInfoDlg, IDB_SELECT_NEW_MODEL, -600 /* Click */, OnClickSelectNewModel, VTS_NONE)
	ON_EVENT(CAutoNSMCInfoDlg, IDB_DELETE_NEW_MODEL, -600 /* Click */, OnClickDeleteNewModel, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAutoNSMCInfoDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
	{
		KillTimer(m_uiTimerID);
		m_uiTimerID = 0;
		Sleep(100);
	}

	this->ShowWindow(SW_HIDE);	
}

void CAutoNSMCInfoDlg::StartDlgTimer()
{
	if (m_uiTimerID == 0)
		m_uiTimerID = SetTimer(10, 500, NULL);
}

void CAutoNSMCInfoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);	
}

void CAutoNSMCInfoDlg::OnClickSelectNewModel() 
{
	// TODO: Add your control notification handler code here

	// hongju_MC
	CString strFullPath = m_plnkSystemData->m_strModelFileDir;// + "\\" + m_plnkSystemData->m_strModelFileName;

	CDataModelIDList dlg(strFullPath, &m_strNewModelID);
	dlg.DoModal();

	if (_T("NONE") == m_strNewModelID)
	{
		MyMessageBox("Model not selected.\r\n Model 선택 안됨.", "Error", M_ICONERROR);
		return;
	}

	m_plnkTrsAutoManager->SetNewModelName(m_strNewModelID);
}

void CAutoNSMCInfoDlg::OnClickDeleteNewModel() 
{
	// TODO: Add your control notification handler code here
	m_strNewModelID = _T("NONE");
	m_plnkTrsAutoManager->SetNewModelName(m_strNewModelID);
}
