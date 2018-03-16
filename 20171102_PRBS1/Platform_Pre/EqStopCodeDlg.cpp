// EqStopCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"
#include "EqStopCodeDlg.h"
#include "MSystemData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CEqStopCodeDlg dialog

CEqStopCodeDlg::CEqStopCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEqStopCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEqStopCodeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CEqStopCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEqStopCodeDlg)
	DDX_Control(pDX, IDB_SAVE, m_bSave);
	DDX_Control(pDX, IDB_REPORT, m_bInput);
	DDX_Control(pDX, IDC_EQSTOP_CODE, m_CodeItem);
	DDX_Control(pDX, IDC_EQSTOP_ITEM, m_EditItem);	
	//}}AFX_DATA_MAP
	// Button
	DDX_Control(pDX, IDB_EQSTOP_BTN_0,	m_btnButton[0]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_1,	m_btnButton[1]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_2,	m_btnButton[2]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_3,	m_btnButton[3]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_4,	m_btnButton[4]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_5,	m_btnButton[5]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_6,	m_btnButton[6]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_7,	m_btnButton[7]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_8,	m_btnButton[8]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_9,	m_btnButton[9]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_10,	m_btnButton[10]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_11,	m_btnButton[11]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_12,	m_btnButton[12]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_13,	m_btnButton[13]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_14,	m_btnButton[14]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_15,	m_btnButton[15]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_16,	m_btnButton[16]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_17,	m_btnButton[17]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_18,	m_btnButton[18]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_19,	m_btnButton[19]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_20,	m_btnButton[20]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_21,	m_btnButton[21]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_22,	m_btnButton[22]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_23,	m_btnButton[23]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_24,	m_btnButton[24]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_25,	m_btnButton[25]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_26,	m_btnButton[26]);
	DDX_Control(pDX, IDB_EQSTOP_BTN_27,	m_btnButton[27]);
	// Label
	DDX_Control(pDX, IDC_EQSTOP_LBL_0,	m_lblLabel[0]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_1,	m_lblLabel[1]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_2,	m_lblLabel[2]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_3,	m_lblLabel[3]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_4,	m_lblLabel[4]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_5,	m_lblLabel[5]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_6,	m_lblLabel[6]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_7,	m_lblLabel[7]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_8,	m_lblLabel[8]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_9,	m_lblLabel[9]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_10,	m_lblLabel[10]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_11,	m_lblLabel[11]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_12,	m_lblLabel[12]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_13,	m_lblLabel[13]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_14,	m_lblLabel[14]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_15,	m_lblLabel[15]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_16,	m_lblLabel[16]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_17,	m_lblLabel[17]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_18,	m_lblLabel[18]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_19,	m_lblLabel[19]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_20,	m_lblLabel[20]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_21,	m_lblLabel[21]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_22,	m_lblLabel[22]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_23,	m_lblLabel[23]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_24,	m_lblLabel[24]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_25,	m_lblLabel[25]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_26,	m_lblLabel[26]);
	DDX_Control(pDX, IDC_EQSTOP_LBL_27,	m_lblLabel[27]);

}


BEGIN_MESSAGE_MAP(CEqStopCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CEqStopCodeDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEqStopCodeDlg message handlers

BOOL CEqStopCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CodeData();
	ItemData();	

	SetWindowText(_T("Equipment LossReport Code Input/Modify"));

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	m_nSelectCode = 0;

	// Item
	UpDateDisplay(m_nSelectCode);
	// Code
	UpDateCodeDisplay(m_nSelectCode);

	SetWindowPos(&(CWnd::wndTopMost), 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEqStopCodeDlg::CodeData() 
{
	for(int i = 0; i<DEF_MAX_BUTTON; i++)
	{
		// EqStop Code
		m_strCodeItem[i] = m_plnkSystemData->m_strStopCode[i];
		m_btnButton[i].SetWindowText(m_strCodeItem[i]);
		m_nCode[i] = m_strCodeItem[i] ;
	}

	return;
}

void CEqStopCodeDlg::ItemData() 
{
	for (int i= 0; i<DEF_MAX_BUTTON; i++)
	{
		// EqStop Item
		m_strItem[i] = m_plnkSystemData->m_strStopItem[i];
		m_lblLabel[i].SetWindowText(m_strItem[i]);
	}

	return;
}

void CEqStopCodeDlg::UpDateDisplay(int m_nButton) 
{
	int m_nSelect;
	m_nSelect = m_nButton;

	OnDisplayLanguage();
	for(int i=0; i<DEF_MAX_BUTTON; i++)
	{
		m_bState[i] = FALSE;
	}	
	m_bState[m_nSelect]=TRUE;

	for(int i= 0; i<DEF_MAX_BUTTON; i++)
	{
		if(m_bState[i] == TRUE)
		{
			m_nSelectitem = i;
		}
	}
	
	m_EditItem.SetWindowText(m_strItem[m_nSelect]);

	// 내용이 없는 항목은 전송이 안되게 한다.
	m_strItem[m_nSelectCode].TrimLeft();

	if (m_strItem[m_nSelectCode] != "")
		m_bInput.EnableWindow(TRUE);
	else
		m_bInput.EnableWindow(FALSE);

	return;
}


void CEqStopCodeDlg::UpDateCodeDisplay(int m_nButton) 
{
	int m_nSelect;
	m_nSelect = m_nButton;
	
	for(int i=0; i<DEF_MAX_BUTTON; i++)
	{
		m_bState[i] = FALSE;
		//m_btnButton[i].SetValue(FALSE);
	}	
	m_bState[m_nSelect]=TRUE;
	//m_btnButton[m_nSelect].SetValue(TRUE);

	m_CodeItem.SetWindowText(m_strCodeItem[m_nSelect]);

	// 내용이 없는 항목은 전송이 안되게 한다.
	//m_strItem[m_nSelectCode].TrimLeft();
	//if (m_strCodeItem[m_nSelectCode] != "")
	//	m_bInput.EnableWindow(TRUE);
	//else
	//	m_bInput.EnableWindow(FALSE);

	return;
}

BEGIN_EVENTSINK_MAP(CEqStopCodeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CEqStopCodeDlg)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_0, -600 /* Click */, OnClickEqstopBtn0, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_1, -600 /* Click */, OnClickEqstopBtn1, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_2, -600 /* Click */, OnClickEqstopBtn2, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_3, -600 /* Click */, OnClickEqstopBtn3, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_4, -600 /* Click */, OnClickEqstopBtn4, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_5, -600 /* Click */, OnClickEqstopBtn5, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_6, -600 /* Click */, OnClickEqstopBtn6, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_7, -600 /* Click */, OnClickEqstopBtn7, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_8, -600 /* Click */, OnClickEqstopBtn8, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_9, -600 /* Click */, OnClickEqstopBtn9, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_10, -600 /* Click */, OnClickEqstopBtn10, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_11, -600 /* Click */, OnClickEqstopBtn11, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_12, -600 /* Click */, OnClickEqstopBtn12, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_13, -600 /* Click */, OnClickEqstopBtn13, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_14, -600 /* Click */, OnClickEqstopBtn14, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_15, -600 /* Click */, OnClickEqstopBtn15, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_16, -600 /* Click */, OnClickEqstopBtn16, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_17, -600 /* Click */, OnClickEqstopBtn17, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_18, -600 /* Click */, OnClickEqstopBtn18, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_19, -600 /* Click */, OnClickEqstopBtn19, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_20, -600 /* Click */, OnClickEqstopBtn20, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_21, -600 /* Click */, OnClickEqstopBtn21, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_22, -600 /* Click */, OnClickEqstopBtn22, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_23, -600 /* Click */, OnClickEqstopBtn23, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_24, -600 /* Click */, OnClickEqstopBtn24, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_25, -600 /* Click */, OnClickEqstopBtn25, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_26, -600 /* Click */, OnClickEqstopBtn26, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_EQSTOP_BTN_27, -600 /* Click */, OnClickEqstopBtn27, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CEqStopCodeDlg, IDB_REPORT, -600 /* Click */, OnClickReport, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CEqStopCodeDlg::OnClickEqstopBtn0() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 0;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn1() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 1;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn2() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 2;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn3() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 3;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn4() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 4;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn5() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 5;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn6() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 6;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn7() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 7;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn8() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 8;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn9() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 9;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn10() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 10;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn11() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 11;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn12() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 12;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn13() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 13;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn14() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 14;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn15() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 15;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn16() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 16;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn17() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 17;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn18() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 18;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn19() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 19;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn20() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 20;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn21() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 21;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn22() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 22;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn23() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 23;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn24() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 24;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn25() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 25;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn26() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 26;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickEqstopBtn27() 
{
	// TODO: Add your control notification handler code here
	m_nSelectCode = 27;	
	UpDateDisplay(m_nSelectCode);	
	UpDateCodeDisplay(m_nSelectCode);	
}

void CEqStopCodeDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	// EqStop Code
	m_CodeItem.GetWindowText(m_strCodeItem[m_nSelectCode]);
	m_btnButton[m_nSelectCode].SetWindowText(m_strCodeItem[m_nSelectCode]);
	// EqStop Item
	m_EditItem.GetWindowText(m_strItem[m_nSelectCode]);
	m_lblLabel[m_nSelectCode].SetWindowText(m_strItem[m_nSelectCode]);

	for(int i =0; i<DEF_MAX_BUTTON; i++)
	{
		m_plnkSystemData->m_strStopCode[i] = m_strCodeItem[i];
		m_plnkSystemData->m_strStopItem[i] = m_strItem[i];
	}

	MOlbSystemPre.GetSystemData()->WriteData();

	return;	
}

void CEqStopCodeDlg::OnClickReport()
{
	// TODO: Add your control notification handler code here
	CString m_strSelectitem;
//	int m_nSelectcode;
	CString m_nStringcode;

//	m_nSelectcode = m_nCode[m_nSelectCode];
	m_nStringcode = m_nCode[m_nSelectCode];

	m_EditItem.GetWindowText(m_strSelectitem);
	m_CodeItem.GetWindowText(m_nStringcode);
	///////Code 상위 보고 하는 부분 적용!!!!!!!!!!

	if(!m_strSelectitem.IsEmpty())
	{
		if (MOlbSystemPre.GetLCNetComponent()->m_eEqState == eNormal)
		{
			MOlbSystemPre.GetLCNetComponent()->EqProcessStopReport(ePause, _ttoi(m_nStringcode), m_strSelectitem);
		}
		::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_EQ_STATE,
			(WPARAM)(ePM), NULL);
	}

	else
	{
		// 100522 유실 보고 화면에서 "Pause" 보고
		if (MOlbSystemPre.GetLCNetComponent()->m_eEqProcState != ePause)
		{
			MOlbSystemPre.GetLCNetComponent()->EqProcessStateReport(ePause);
		}
	}
		
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_EQP_STATE,
				(WPARAM)MOlbSystemPre.GetLCNetComponent()->m_eEqProcState, NULL);

	CDialog::OnOK();	
}

void CEqStopCodeDlg::OnDisplayLanguage()
{
	CString strTemp;


	ReadDisplayMessage(MAIN_MANUAL, _T("CEqStopCodeDlg_1"), &strTemp);
	GetDlgItem(IDB_REPORT)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_MANUAL, _T("CEqStopCodeDlg_2"), &strTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_MANUAL, _T("CEqStopCodeDlg_3"), &strTemp);
	GetDlgItem(IDB_EQSTOP_CODE)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_MANUAL, _T("CEqStopCodeDlg_4"), &strTemp);
	GetDlgItem(IDB_EQSTOP_ITEM)->SetWindowText(strTemp);

	//ReadDisplayMessage(MAIN_MANUAL, _T("CEqStopCodeDlg_5"), &strTemp);
	//GetDlgItem(IDC_MISSING_ITEM)->SetWindowText(strTemp);

}
