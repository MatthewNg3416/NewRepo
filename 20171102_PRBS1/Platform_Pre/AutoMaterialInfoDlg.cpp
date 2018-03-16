// AutoMaterialInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "AutoMaterialInfoDlg.h"
#include "AutoTabICInfoDlg.h"
#include "AutoZigInfoDlg.h"
#include "MPlatformOlbSystem.h"
#include "MCtrlTabFeeder.h"
#include "MLCNet.h"

#include "MoldZigInfo.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CAutoMaterialInfoDlg dialog

CAutoMaterialInfoDlg::CAutoMaterialInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoMaterialInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoMaterialInfoDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkLCNet			 = MOlbSystemPre.GetLCNetComponent();
	m_plnkSystemData	 = MOlbSystemPre.GetSystemData();
	m_plnkCtrlTabFeeder1 = MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1);
	m_plnkCtrlTabFeeder2 = NULL;
//#ifdef DEF_GATE_SYSTEM
	m_plnkCtrlTabFeeder2 = MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2);
//#endif
}


void CAutoMaterialInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoMaterialInfoDlg)
	DDX_Control(pDX, IDB_TAB_INSERT_1, m_bTabInsert1);
	DDX_Control(pDX, IDB_TAB_REMOVE_1, m_bTabRemove1);
	DDX_Control(pDX, IDB_TAB_REMOVE_COMPLETE_1, m_bTabRemoveComplete1);
	DDX_Control(pDX, IDB_TAB_INSERT_2, m_bTabInsert2);
	DDX_Control(pDX, IDB_TAB_REMOVE_2, m_bTabRemove2);
	DDX_Control(pDX, IDB_TAB_REMOVE_COMPLETE_2, m_bTabRemoveComplete2);
	DDX_Control(pDX, IDB_TAB_INSERT_3, m_bTabInsert3);
	DDX_Control(pDX, IDB_TAB_REMOVE_3, m_bTabRemove3);
	DDX_Control(pDX, IDB_TAB_REMOVE_COMPLETE_3, m_bTabRemoveComplete3);
	DDX_Control(pDX, IDB_TAB_INSERT_4, m_bTabInsert4);
	DDX_Control(pDX, IDB_TAB_REMOVE_4, m_bTabRemove4);
	DDX_Control(pDX, IDB_TAB_REMOVE_COMPLETE_4, m_bTabRemoveComplete4);
	DDX_Control(pDX, IDB_ZIG_INSERT_1, m_bZigInsert1);
	DDX_Control(pDX, IDB_ZIG_REMOVE_1, m_bZigRemove1);
	DDX_Control(pDX, IDB_ZIG_REMOVE_COMPLETE_1, m_bZigRemoveComplete1);
	DDX_Control(pDX, IDB_ZIG_INSERT_2, m_bZigInsert2);
	DDX_Control(pDX, IDB_ZIG_REMOVE_2, m_bZigRemove2);
	DDX_Control(pDX, IDB_ZIG_REMOVE_COMPLETE_2, m_bZigRemoveComplete2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoMaterialInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoMaterialInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoMaterialInfoDlg message handlers

BEGIN_EVENTSINK_MAP(CAutoMaterialInfoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoMaterialInfoDlg)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_INSERT_1, -600 /* Click */, OnTabInsert1, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_REMOVE_1, -600 /* Click */, OnTabRemove1, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_REMOVE_COMPLETE_1, -600 /* Click */, OnClickTabRemoveComplete1, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_INSERT_2, -600 /* Click */, OnTabInsert2, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_REMOVE_2, -600 /* Click */, OnTabRemove2, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_REMOVE_COMPLETE_2, -600 /* Click */, OnClickTabRemoveComplete2, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_INSERT_3, -600 /* Click */, OnTabInsert3, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_REMOVE_3, -600 /* Click */, OnTabRemove3, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_REMOVE_COMPLETE_3, -600 /* Click */, OnClickTabRemoveComplete3, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_INSERT_4, -600 /* Click */, OnTabInsert4, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_REMOVE_4, -600 /* Click */, OnTabRemove4, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_TAB_REMOVE_COMPLETE_4, -600 /* Click */, OnClickTabRemoveComplete4, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_ZIG_INSERT_1, -600 /* Click */, OnZigInsert1, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_ZIG_REMOVE_1, -600 /* Click */, OnZigRemove1, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_ZIG_REMOVE_COMPLETE_1, -600 /* Click */, OnClickZigRemoveComplete1, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_ZIG_INSERT_2, -600 /* Click */, OnZigInsert2, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_ZIG_REMOVE_2, -600 /* Click */, OnZigRemove2, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_ZIG_REMOVE_COMPLETE_2, -600 /* Click */, OnClickZigRemoveComplete2, VTS_NONE)
	ON_EVENT(CAutoMaterialInfoDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAutoMaterialInfoDlg::OnTabInsert1() 
{
	//20131003 SJ_HJG �߰�
	int iResult = 0;
	MTrsAutoManager* plnkAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
	iResult = plnkAutoManager->MaterialCodeInfoRequest(DEF_CUR_PRODUCT_PLAN, eMatStep_S);
	if (iResult == ERR_NO_DEVICEID_MCODE_REQUEST)
	{			
		MPlanRequestData *pPR;
		
		pPR = new MPlanRequestData;
		
		pPR->m_nM_KIND = 0 ;
		
		//110705.kms___________
		pPR->m_sEQPID.Format(m_plnkSystemData->m_strEQPID);
		//_____________________
		
		pPR->m_sLINE_NO.Format(_T("LINE_NO"));
		
		pPR->m_sCarrierID.Format(_T(" "));
		pPR->m_sDeviceID.Format(_T(" "));	
		pPR->m_sM_STEP.Format(_T(" "));
		
		m_plnkLCNet->ProductPlanRequest(eProductPlanRequest, pPR);				
		delete pPR;
	}
	//_____________________________

	CAutoTabICInfoDlg Dlg(DEF_TABFEEDER1, DEF_USING_REEL, eMatDockIn);
	Dlg.DoModal();
}

void CAutoMaterialInfoDlg::OnTabRemove1() 
{
	m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE = eMatStateDisable;
	//110808 SJ_KJS
//	m_plnkLCNet->MaterialReport(eMatConsumeEnd, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL]);
	m_plnkLCNet->MaterialReport(eMatStockOut, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL]);
	m_plnkLCNet->MaterialReport(eMatDockOut, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL]);
	m_bTabRemoveComplete1.EnableWindow(TRUE);	
}

void CAutoMaterialInfoDlg::OnClickTabRemoveComplete1() 
{
	m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE = eMatStateDisable;

	m_plnkLCNet->MaterialReport(eMatBatchCancel, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL]);			

	m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL].ResetData();
//Feeder1 ��� Reel �ǹ��� �����Ƿ� Data ���� �Ͽ����ϴ�.
	MyMessageBox(MY_MESSAGE_BOX, 20100, _T("Feeder#1 Using Reel Data Removal"), M_ICONINFORMATION);
	m_bTabInsert1.EnableWindow(TRUE);
}

void CAutoMaterialInfoDlg::OnTabInsert2() 
{
//#ifdef DEF_GATE_SYSTEM
	//20131003 SJ_HJG �߰�
	int iResult = 0;
	MTrsAutoManager* plnkAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
	iResult = plnkAutoManager->MaterialCodeInfoRequest(DEF_CUR_PRODUCT_PLAN, eMatStep_S);
	if (iResult == ERR_NO_DEVICEID_MCODE_REQUEST)
	{			
		MPlanRequestData *pPR;
		
		pPR = new MPlanRequestData;
		
		pPR->m_nM_KIND = 0 ;
		
		//110705.kms___________
		pPR->m_sEQPID.Format(m_plnkSystemData->m_strEQPID);
		//_____________________
		
		pPR->m_sLINE_NO.Format(_T("LINE_NO"));
		
		pPR->m_sCarrierID.Format(_T(" "));
		pPR->m_sDeviceID.Format(_T(" "));	
		pPR->m_sM_STEP.Format(_T(" "));
		
		m_plnkLCNet->ProductPlanRequest(eProductPlanRequest, pPR);				
		delete pPR;
	}
	//_____________________________

	CAutoTabICInfoDlg Dlg(DEF_TABFEEDER2, DEF_USING_REEL, eMatDockIn);
	Dlg.DoModal();	
//#endif
}

void CAutoMaterialInfoDlg::OnTabRemove2() 
{
//#ifdef DEF_GATE_SYSTEM
	m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE = eMatStateDisable;
	//110808 SJ_KJS
//	m_plnkLCNet->MaterialReport(eMatConsumeEnd, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL]);
	m_plnkLCNet->MaterialReport(eMatStockOut, &m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_USING_REEL]);
	m_plnkLCNet->MaterialReport(eMatDockOut, &m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_USING_REEL]);
	m_bTabRemoveComplete2.EnableWindow(TRUE);	
//#endif
}

void CAutoMaterialInfoDlg::OnClickTabRemoveComplete2() 
{
//#ifdef DEF_GATE_SYSTEM
	m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE = eMatStateDisable;

	m_plnkLCNet->MaterialReport(eMatBatchCancel, &m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_USING_REEL]);			

	m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_USING_REEL].ResetData();
//Feeder2 ��� Reel �ǹ��� �����Ƿ� Data ���� �Ͽ����ϴ�.
	MyMessageBox(MY_MESSAGE_BOX, 20101, _T("Feeder#2 Using Reel Data Removal"), M_ICONINFORMATION);
	m_bTabInsert2.EnableWindow(TRUE);
//#endif
}

void CAutoMaterialInfoDlg::OnTabInsert3() 
{
	CAutoTabICInfoDlg Dlg(DEF_TABFEEDER1, DEF_SPARE_REEL, eMatDockIn);
	Dlg.DoModal();
}

void CAutoMaterialInfoDlg::OnTabRemove3() 
{
	m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_SPARE_REEL].m_nM_STATE = eMatStateDisable;
	//110808 SJ_KJS
//	m_plnkLCNet->MaterialReport(eMatConsumeEnd, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL]);
	m_plnkLCNet->MaterialReport(eMatStockOut, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_SPARE_REEL]);
	m_plnkLCNet->MaterialReport(eMatDockOut, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_SPARE_REEL]);
	m_bTabRemoveComplete3.EnableWindow(TRUE);	
}

void CAutoMaterialInfoDlg::OnClickTabRemoveComplete3() 
{
	m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_SPARE_REEL].m_nM_STATE = eMatStateDisable;


	m_plnkLCNet->MaterialReport(eMatBatchCancel, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_SPARE_REEL]);			

	m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_SPARE_REEL].ResetData();
	m_plnkCtrlTabFeeder1->m_bConsumeStart[DEF_SPARE_REEL] = TRUE;
//Feeder1 ��� Reel �ǹ��� �����Ƿ� Data ���� �Ͽ����ϴ�.
	MyMessageBox(MY_MESSAGE_BOX, 20102, _T("Feeder#1 Sprae Reel Data Removal"), M_OK);
	m_bTabInsert3.EnableWindow(TRUE);
}

void CAutoMaterialInfoDlg::OnTabInsert4() 
{
//#ifdef DEF_GATE_SYSTEM
	CAutoTabICInfoDlg Dlg(DEF_TABFEEDER2, DEF_SPARE_REEL, eMatDockIn);
	Dlg.DoModal();	
//#endif
}

void CAutoMaterialInfoDlg::OnTabRemove4() 
{
//#ifdef DEF_GATE_SYSTEM
	m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_SPARE_REEL].m_nM_STATE = eMatStateDisable;
//110808 SJ_KJS
//	m_plnkLCNet->MaterialReport(eMatConsumeEnd, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL]);
	m_plnkLCNet->MaterialReport(eMatStockOut, &m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_SPARE_REEL]);
	m_plnkLCNet->MaterialReport(eMatDockOut, &m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_SPARE_REEL]);
	m_bTabRemoveComplete4.EnableWindow(TRUE);	
//#endif
}

void CAutoMaterialInfoDlg::OnClickTabRemoveComplete4() 
{
//#ifdef DEF_GATE_SYSTEM
	m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_SPARE_REEL].m_nM_STATE = eMatStateDisable;


	m_plnkLCNet->MaterialReport(eMatBatchCancel, &m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_SPARE_REEL]);

	m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_SPARE_REEL].ResetData();
	m_plnkCtrlTabFeeder2->m_bConsumeStart[DEF_SPARE_REEL] = TRUE;
//Feeder2 ��� Reel �ǹ��� �����Ƿ� Data ���� �Ͽ����ϴ�.
	MyMessageBox(MY_MESSAGE_BOX, 20103, _T("Feeder#1 Sprae Reel Data Removal"), M_ICONINFORMATION);
	m_bTabInsert4.EnableWindow(TRUE);
//#endif
}

void CAutoMaterialInfoDlg::OnZigInsert1() 
{
	//100825.KMS
//	CAutoZigInfoDlg Dlg(0);
	CMoldZigInfo Dlg(DEF_TABFEEDER1, eMatDockIn);
	Dlg.DoModal();
}

void CAutoMaterialInfoDlg::OnZigRemove1() 
{
	m_plnkCtrlTabFeeder1->m_matZigInfo.m_nM_STATE = eMatStateDisable;
	//110808 SJ_KJS
//	m_plnkLCNet->MaterialReport(eMatConsumeEnd, &m_plnkCtrlTabFeeder1->m_matZigInfo);
	m_plnkLCNet->MaterialReport(eMatDockOut, &m_plnkCtrlTabFeeder1->m_matZigInfo);
	m_plnkLCNet->MaterialReport(eMatStockOut, &m_plnkCtrlTabFeeder1->m_matZigInfo);
	m_bZigRemoveComplete1.EnableWindow(TRUE);	
}

void CAutoMaterialInfoDlg::OnClickZigRemoveComplete1() 
{
	m_plnkCtrlTabFeeder1->m_matZigInfo.m_nM_STATE = eMatStateDisable;
	//110808 SJ_KJS
//	m_plnkLCNet->MaterialReport(eMatDockOut, &m_plnkCtrlTabFeeder1->m_matZigInfo);			
	m_plnkLCNet->MaterialReport(eMatBatchCancel, &m_plnkCtrlTabFeeder1->m_matZigInfo);
	m_plnkCtrlTabFeeder1->m_matZigInfo.ResetData();
//Feeder1 Zig �ǹ��� �����Ƿ� Data ���� �Ͽ����ϴ�.
	MyMessageBox(MY_MESSAGE_BOX, 20104, _T("Feeder#1 Zig Data Removal"), M_ICONINFORMATION);
	m_bZigInsert1.EnableWindow(TRUE);
}

void CAutoMaterialInfoDlg::OnZigInsert2() 
{
//#ifdef DEF_GATE_SYSTEM
//	CAutoZigInfoDlg Dlg(1);
	CMoldZigInfo Dlg(DEF_TABFEEDER2, eMatDockIn);
	Dlg.DoModal();
//#endif
}

void CAutoMaterialInfoDlg::OnZigRemove2() 
{
//#ifdef DEF_GATE_SYSTEM
	m_plnkCtrlTabFeeder2->m_matZigInfo.m_nM_STATE = eMatStateDisable;
	//110808 SJ_KJS
//	m_plnkLCNet->MaterialReport(eMatConsumeEnd, &m_plnkCtrlTabFeeder1->m_matZigInfo);
	m_plnkLCNet->MaterialReport(eMatDockOut, &m_plnkCtrlTabFeeder1->m_matZigInfo);
	m_plnkLCNet->MaterialReport(eMatStockOut, &m_plnkCtrlTabFeeder1->m_matZigInfo);
	m_bZigRemoveComplete2.EnableWindow(TRUE);	
//#endif
}

void CAutoMaterialInfoDlg::OnClickZigRemoveComplete2() 
{
//#ifdef DEF_GATE_SYSTEM
	m_plnkCtrlTabFeeder2->m_matZigInfo.m_nM_STATE = eMatStateDisable;
	//110808 SJ_KJS
//	m_plnkLCNet->MaterialReport(eMatDockOut, &m_plnkCtrlTabFeeder2->m_matZigInfo);	
	m_plnkLCNet->MaterialReport(eMatBatchCancel, &m_plnkCtrlTabFeeder2->m_matZigInfo);			
	m_plnkCtrlTabFeeder2->m_matZigInfo.ResetData();
//Feeder2 Zig �ǹ��� �����Ƿ� Data ���� �Ͽ����ϴ�.
	MyMessageBox(MY_MESSAGE_BOX, 20105, _T("Feeder#1 Zig Data Removal"), M_ICONINFORMATION);
	m_bZigInsert2.EnableWindow(TRUE);
//#endif
}

void CAutoMaterialInfoDlg::OnExit() 
{
	CDialog::OnCancel();	
}

BOOL CAutoMaterialInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bTabRemoveComplete1.EnableWindow(FALSE);	
	m_bTabRemoveComplete2.EnableWindow(FALSE);	
	m_bTabRemoveComplete3.EnableWindow(FALSE);	
	m_bTabRemoveComplete4.EnableWindow(FALSE);	
	m_bZigRemoveComplete1.EnableWindow(FALSE);
	m_bZigRemoveComplete2.EnableWindow(FALSE);
	
	/*
	if(SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		m_bTabInsert2.EnableWindow(FALSE);
		m_bTabRemove2.EnableWindow(FALSE);
		m_bTabInsert4.EnableWindow(FALSE);
		m_bTabRemove4.EnableWindow(FALSE);
		m_bZigInsert2.EnableWindow(FALSE);
		m_bZigRemove2.EnableWindow(FALSE);
	}
	*/

/*	switch(MOlbSystemPre.GetSystemData()->m_nUseFeeder)
	{
	case USE_FEEDER1:
		m_bTabInsert2.EnableWindow(FALSE);
		m_bTabRemove2.EnableWindow(FALSE);
		m_bTabRemoveComplete2.EnableWindow(FALSE);
		m_bZigInsert2.EnableWindow(FALSE);
		m_bZigRemove2.EnableWindow(FALSE);
		m_bZigRemoveComplete2.EnableWindow(FALSE);
		break;
	case USE_FEEDER2:
		m_bTabInsert1.EnableWindow(FALSE);
		m_bTabRemove1.EnableWindow(FALSE);
		m_bTabRemoveComplete1.EnableWindow(FALSE);
		m_bZigInsert1.EnableWindow(FALSE);
		m_bZigRemove1.EnableWindow(FALSE);
		m_bZigRemoveComplete1.EnableWindow(FALSE);
		break;
	}
*/

	//������������������������������������������������������������������������������������������������������
	// Multi Language Button Title Display                                                                ��
	//������������������������������������������������������������������������������������������������������
	
	CString szTextTemp; //�����ڵ� ��ȯ ���� �߰�

	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_1"), &szTextTemp);
//	Tab IC ����\r\n(��ȸ)
	GetDlgItem(IDB_TAB_INSERT_1)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_2"), &szTextTemp);
//	Tab IC ����
	GetDlgItem(IDB_TAB_REMOVE_1)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_3"), &szTextTemp);
//	���� �Ϸ�
	GetDlgItem(IDB_TAB_REMOVE_COMPLETE_1)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_4"), &szTextTemp);
//	Tab IC ����\r\n(��ȸ)
	GetDlgItem(IDB_TAB_INSERT_2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_5"), &szTextTemp);
//	Tab IC ����
	GetDlgItem(IDB_TAB_REMOVE_2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_6"), &szTextTemp);
//	���� �Ϸ�
	GetDlgItem(IDB_TAB_REMOVE_COMPLETE_2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_7"), &szTextTemp);
//	Tab IC ����\r\n(��ȸ)
	GetDlgItem(IDB_TAB_INSERT_3)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_8"), &szTextTemp);
//	Tab IC ����
	GetDlgItem(IDB_TAB_REMOVE_3)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_9"), &szTextTemp);
//	���� �Ϸ� 
	GetDlgItem(IDB_TAB_REMOVE_COMPLETE_3)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_10"), &szTextTemp);
//	Tab IC ����\r\n(��ȸ)
	GetDlgItem(IDB_TAB_INSERT_4)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_11"), &szTextTemp);
//	Tab IC ����
	GetDlgItem(IDB_TAB_REMOVE_4)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_12"), &szTextTemp);
//	���� �Ϸ�
	GetDlgItem(IDB_TAB_REMOVE_COMPLETE_4)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_13"), &szTextTemp);
//	Zig ����\r\n(��ȸ)
	GetDlgItem(IDB_ZIG_INSERT_1)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_14"), &szTextTemp);
//	Zig ����
	GetDlgItem(IDB_ZIG_REMOVE_1)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_15"), &szTextTemp);
//	���� �Ϸ�
	GetDlgItem(IDB_ZIG_REMOVE_COMPLETE_1)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_16"), &szTextTemp);
//	Zig ����\r\n(��ȸ)
	GetDlgItem(IDB_ZIG_INSERT_2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_17"), &szTextTemp);
//	Zig ����
	GetDlgItem(IDB_ZIG_REMOVE_2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_18"), &szTextTemp);
//	���� �Ϸ� 
	GetDlgItem(IDB_ZIG_REMOVE_COMPLETE_2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMaterialInfoDlg_19"), &szTextTemp);
//	������
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
