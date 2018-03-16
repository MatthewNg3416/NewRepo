// ModulePortIDDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "ModulePortIDDlg.h"

#include "MPlatformOlbSystem.h"	
#include "MSystemData.h"
#include "CommonGetValueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModulePortIDDlg dialog

extern MPlatformOlbSystem MOlbSystemPre;

CModulePortIDDlg::CModulePortIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModulePortIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModulePortIDDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CModulePortIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModulePortIDDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_MODULE_ID,	m_bEQPID);

	DDX_Control(pDX, IDB_REPORT_LEVEL, m_bReportLevel);
	DDX_Control(pDX, IDB_MODULEID, m_bModuleID);
	DDX_Control(pDX, IDB_MODULEID2, m_bModuleID2);
	DDX_Control(pDX, IDB_MODULEID3, m_bModuleID3_1);
	DDX_Control(pDX, IDB_MODULEID4, m_bModuleID3_2);
	DDX_Control(pDX, IDB_MODULEID5, m_bModuleID3_3);
	DDX_Control(pDX, IDB_MODULEID6, m_bModuleID3_4);

	DDX_Control(pDX, IDC_MODULE_ID,	m_sEQPID);
	DDX_Control(pDX, IDC_MODULEID, m_sModuleID);
	DDX_Control(pDX, IDC_MODULEID2, m_sModuleID2);
	DDX_Control(pDX, IDC_MODULEID3, m_sModuleID3_1);
	DDX_Control(pDX, IDC_MODULEID4, m_sModuleID3_2);
	DDX_Control(pDX, IDC_MODULEID5, m_sModuleID3_3);
	DDX_Control(pDX, IDC_MODULEID6, m_sModuleID3_4);	
	DDX_Control(pDX, IDC_REPORT_LEVEL, m_sReportLevel);


	DDX_Control(pDX, IDB_PORT_ID,	m_bPortid[0]);
	DDX_Control(pDX, IDB_PORT_ID2,	m_bPortid[1]);
	DDX_Control(pDX, IDB_PORT_ID3,	m_bPortid[2]);
	DDX_Control(pDX, IDB_PORT_ID4,	m_bPortid[3]);
	DDX_Control(pDX, IDB_PORT_ID5,	m_bPortid[4]);
	DDX_Control(pDX, IDB_PORT_ID6,	m_bPortid[5]);
	DDX_Control(pDX, IDB_PORT_ID7,	m_bPortid[6]);
	DDX_Control(pDX, IDB_PORT_ID8,	m_bPortid[7]);
	DDX_Control(pDX, IDB_PORT_ID9,	m_bPortid[8]);
	DDX_Control(pDX, IDB_PORT_ID10,	m_bPortid[9]);

	DDX_Control(pDX, IDC_PORT_ID,	m_lblPortid[0]);
	DDX_Control(pDX, IDC_PORT_ID2,	m_lblPortid[1]);
	DDX_Control(pDX, IDC_PORT_ID3,	m_lblPortid[2]);
	DDX_Control(pDX, IDC_PORT_ID4,	m_lblPortid[3]);
	DDX_Control(pDX, IDC_PORT_ID5,	m_lblPortid[4]);
	DDX_Control(pDX, IDC_PORT_ID6,	m_lblPortid[5]);
	DDX_Control(pDX, IDC_PORT_ID7,	m_lblPortid[6]);
	DDX_Control(pDX, IDC_PORT_ID8,	m_lblPortid[7]);
	DDX_Control(pDX, IDC_PORT_ID9,	m_lblPortid[8]);
	DDX_Control(pDX, IDC_PORT_ID10,	m_lblPortid[9]);
	DDX_Control(pDX, IDC_PORT_ID11,	m_lblPortid[10]);
	DDX_Control(pDX, IDC_PORT_ID12,	m_lblPortid[11]);
}


BEGIN_MESSAGE_MAP(CModulePortIDDlg, CDialog)
	//{{AFX_MSG_MAP(CModulePortIDDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModulePortIDDlg message handlers

BOOL CModulePortIDDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString szTextTemp;
	//저장
	ReadDisplayMessage(MAIN_DATA, _T("ModulePortIDDlg_1"), &szTextTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	//나가기
	ReadDisplayMessage(MAIN_DATA, _T("ModulePortIDDlg_2"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);


	m_strEQPID = m_plnkSystemData->m_strEQPID;
	//20110811 SJ_HJG
	m_strModuleID = m_plnkSystemData->m_strModuleID;
	m_strModuleID2 = m_plnkSystemData->m_strModuleID2;
	m_strModuleID3_1 = m_plnkSystemData->m_strModuleID3_1;
	m_strModuleID3_2 = m_plnkSystemData->m_strModuleID3_2;
	m_strModuleID3_3 = m_plnkSystemData->m_strModuleID3_3;
	m_strModuleID3_4 = m_plnkSystemData->m_strModuleID3_4;
	m_iReportLevel = m_plnkSystemData->m_iReportModuleLevel;
	//___________________
	m_strPortID[0] = m_plnkSystemData->m_strModulePortID[0];
	m_strPortID[1] = m_plnkSystemData->m_strModulePortID[1];
	m_strPortID[2] = m_plnkSystemData->m_strModulePortID[2];
	m_strPortID[3] = m_plnkSystemData->m_strModulePortID[3];
	m_strPortID[4] = m_plnkSystemData->m_strModulePortID[4];
	m_strPortID[5] = m_plnkSystemData->m_strModulePortID[5];
	m_strPortID[6] = m_plnkSystemData->m_strModulePortID[6];
	m_strPortID[7] = m_plnkSystemData->m_strModulePortID[7];
	m_strPortID[8] = m_plnkSystemData->m_strModulePortID[8];
	m_strPortID[9] = m_plnkSystemData->m_strModulePortID[9];
	m_strPortID[10] = m_plnkSystemData->m_strModulePortID[10];
	m_strPortID[11] = m_plnkSystemData->m_strModulePortID[11];


	swprintf(m_szNewValue, _T("%s"), m_strEQPID.GetBuffer());
	m_sEQPID.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%s"), m_strModuleID.GetBuffer());
	m_sModuleID.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%s"), m_strModuleID2.GetBuffer());
	m_sModuleID2.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%s"), m_strModuleID3_1.GetBuffer());
	m_sModuleID3_1.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%s"), m_strModuleID3_2.GetBuffer());
	m_sModuleID3_2.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%s"), m_strModuleID3_3.GetBuffer());
	m_sModuleID3_3.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%s"), m_strModuleID3_4.GetBuffer());
	m_sModuleID3_4.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%d"), m_iReportLevel);
	m_sReportLevel.SetCaption(m_szNewValue);


	for(int i = 0; i<DEF_MAX_MODULE_NUMBER; i++)
	{
		swprintf(m_szNewValue, _T("%s"), m_strPortID[i].GetBuffer());
		m_lblPortid[i].SetCaption(m_szNewValue);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CModulePortIDDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CModulePortIDDlg)
	ON_EVENT(CModulePortIDDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_MODULE_ID, -600 /* Click */, OnClickModuleId, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID, -600 /* Click */, OnClickPortId, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID2, -600 /* Click */, OnClickPortId2, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID3, -600 /* Click */, OnClickPortId3, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID4, -600 /* Click */, OnClickPortId4, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID5, -600 /* Click */, OnClickPortId5, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID6, -600 /* Click */, OnClickPortId6, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID7, -600 /* Click */, OnClickPortId7, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID8, -600 /* Click */, OnClickPortId8, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID9, -600 /* Click */, OnClickPortId9, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID10, -600 /* Click */, OnClickPortId10, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_MODULEID, -600 /* Click */, OnClickModuleid, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_MODULEID2, -600 /* Click */, OnClickModuleid2, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_MODULEID3, -600 /* Click */, OnClickModuleid3, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_MODULEID4, -600 /* Click */, OnClickModuleid4, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_MODULEID5, -600 /* Click */, OnClickModuleid5, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_MODULEID6, -600 /* Click */, OnClickModuleid6, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_REPORT_LEVEL, -600 /* Click */, OnClickReportLevel, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID11, -600 /* Click */, OnClickPortId11, VTS_NONE)
	ON_EVENT(CModulePortIDDlg, IDB_PORT_ID12, -600 /* Click */, OnClickPortId12, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CModulePortIDDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CModulePortIDDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	//Save Data?\r\nData를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40101, _T("Confirmation"), M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	m_plnkSystemData->m_strEQPID = m_strEQPID;
	m_plnkSystemData->m_strModulePortID[0] = m_strPortID[0];
	m_plnkSystemData->m_strModulePortID[1] = m_strPortID[1];
	m_plnkSystemData->m_strModulePortID[2] = m_strPortID[2];
	m_plnkSystemData->m_strModulePortID[3] = m_strPortID[3];
	m_plnkSystemData->m_strModulePortID[4] = m_strPortID[4];
	m_plnkSystemData->m_strModulePortID[5] = m_strPortID[5];
	m_plnkSystemData->m_strModulePortID[6] = m_strPortID[6];
	m_plnkSystemData->m_strModulePortID[7] = m_strPortID[7];
	m_plnkSystemData->m_strModulePortID[8] = m_strPortID[8];
	m_plnkSystemData->m_strModulePortID[9] = m_strPortID[9];
	m_plnkSystemData->m_strModulePortID[10] = m_strPortID[10];
	m_plnkSystemData->m_strModulePortID[11] = m_strPortID[11];

	//20110811 SJ_HJG
	m_plnkSystemData->m_strEQPID = m_strEQPID;
	m_plnkSystemData->m_strModuleID = m_strModuleID;
	m_plnkSystemData->m_strModuleID2 = m_strModuleID2;
	m_plnkSystemData->m_strModuleID3_1 = m_strModuleID3_1;
	m_plnkSystemData->m_strModuleID3_2 = m_strModuleID3_2;
	m_plnkSystemData->m_strModuleID3_3 = m_strModuleID3_3;
	m_plnkSystemData->m_strModuleID3_4 = m_strModuleID3_4;

	m_plnkSystemData->m_iReportModuleLevel = m_iReportLevel;

	switch(m_iReportLevel)
	{
	case 0:
		//m_plnkSystemData->m_strReportModuelID = m_strEQPID;
		//break;
	case 1:
		//m_plnkSystemData->m_strReportModuelID = m_strModuleID;
		//break;
	case 2:
		m_plnkSystemData->m_strReportModuelID = m_strModuleID2;
		break;
	case 3:
		m_plnkSystemData->m_strReportModuelID = m_strModuleID2;
		break;
	}

	MOlbSystemPre.GetLCNetComponent()->SetUnitReportID(m_plnkSystemData->m_strReportModuelID);
	//__________________________________________
	m_plnkSystemData->WriteData();
}


void CModulePortIDDlg::OnClickModuleId() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Module ID")))
		return;

	m_strEQPID.Format(_T("%s"), szTemp);
	m_sEQPID.SetCaption(m_strEQPID);
}

void CModulePortIDDlg::OnClickPortId() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[0].Format(_T("%s"), szTemp);
	m_lblPortid[0].SetCaption(m_strPortID[0]);
}

void CModulePortIDDlg::OnClickPortId2() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[1].Format(_T("%s"), szTemp);
	m_lblPortid[1].SetCaption(m_strPortID[1]);
}

void CModulePortIDDlg::OnClickPortId3() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[2].Format(_T("%s"), szTemp);
	m_lblPortid[2].SetCaption(m_strPortID[2]);
}

void CModulePortIDDlg::OnClickPortId4() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[3].Format(_T("%s"), szTemp);
	m_lblPortid[3].SetCaption(m_strPortID[3]);
}

void CModulePortIDDlg::OnClickPortId5() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[4].Format(_T("%s"), szTemp);
	m_lblPortid[4].SetCaption(m_strPortID[4]);
}

void CModulePortIDDlg::OnClickPortId6() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[5].Format(_T("%s"), szTemp);
	m_lblPortid[5].SetCaption(m_strPortID[5]);
}

void CModulePortIDDlg::OnClickPortId7() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[6].Format(_T("%s"), szTemp);
	m_lblPortid[6].SetCaption(m_strPortID[6]);
}

void CModulePortIDDlg::OnClickPortId8() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[7].Format(_T("%s"), szTemp);
	m_lblPortid[7].SetCaption(m_strPortID[7]);
}

void CModulePortIDDlg::OnClickPortId9() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[8].Format(_T("%s"), szTemp);
	m_lblPortid[8].SetCaption(m_strPortID[8]);
}

void CModulePortIDDlg::OnClickPortId10() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[9].Format(_T("%s"), szTemp);
	m_lblPortid[9].SetCaption(m_strPortID[9]);
}

void CModulePortIDDlg::OnClickModuleid() 
{
	// TODO: Add your control notification handler code here

	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Module ID...(Ex)OL_AC01)")))
		return;

	m_strModuleID.Format(_T("%s"), szTemp);
	m_sModuleID.SetCaption(m_strModuleID);	
}

void CModulePortIDDlg::OnClickModuleid2() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Module ID...(Ex)OL_AC01)")))
		return;

	m_strModuleID2.Format(_T("%s"), szTemp);
	m_sModuleID2.SetCaption(m_strModuleID2);	
}

void CModulePortIDDlg::OnClickModuleid3() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Module ID...(Ex)OL_AC01)")))
		return;

	m_strModuleID3_1.Format(_T("%s"), szTemp);
	m_sModuleID3_1.SetCaption(m_strModuleID3_1);	
}

void CModulePortIDDlg::OnClickModuleid4() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Module ID...(Ex)OL_AC01)")))
		return;

	m_strModuleID3_2.Format(_T("%s"), szTemp);
	m_sModuleID3_2.SetCaption(m_strModuleID3_2);	
}

void CModulePortIDDlg::OnClickModuleid5() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Module ID...(Ex)OL_AC01)")))
		return;

	m_strModuleID3_3.Format(_T("%s"), szTemp);
	m_sModuleID3_3.SetCaption(m_strModuleID3_3);	
}

void CModulePortIDDlg::OnClickModuleid6() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Module ID...(Ex)OL_AC01)")))
		return;

	m_strModuleID3_4.Format(_T("%s"), szTemp);
	m_sModuleID3_4.SetCaption(m_strModuleID3_4);
	
}

void CModulePortIDDlg::OnClickReportLevel() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];
	CString	strReportLevel;

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T("%d"), m_iReportLevel);
	MyMessageBox("Set Report Level)	\r\n"
			"<< Report Level 을 선택하세요..(0L ~ 3L) >>","확인", M_ICONINFORMATION);

	if(!GetNumberBox(szTemp, 10, szBuf))	
		return;

	m_iReportLevel = _wtoi(szTemp);	

	strReportLevel.Format(_T("%s"), szTemp);
	m_sReportLevel.SetCaption(strReportLevel);
}

void CModulePortIDDlg::OnClickPortId11() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[10].Format(_T("%s"), szTemp);
	m_lblPortid[10].SetCaption(m_strPortID[10]);
}

void CModulePortIDDlg::OnClickPortId12() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Port ID")))
		return;

	m_strPortID[11].Format(_T("%s"), szTemp);
	m_lblPortid[11].SetCaption(m_strPortID[11]);
}
