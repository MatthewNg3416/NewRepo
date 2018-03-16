// PanelValidationCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "PanelValidationCheckDlg.h"
#include "MPlatformOlbSystem.h"
#include "MSystemData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPanelValidationCheckDlg dialog
extern MPlatformOlbSystem MOlbSystemPre;


CPanelValidationCheckDlg::CPanelValidationCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelValidationCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPanelValidationCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_plnkSystemData = MOlbSystemPre.GetSystemData();

	for(int i=0; i<DEF_MAX_CHECK_NUMBER; i++)
	{
		m_CheckSelect[i] = FALSE;
	}
}


void CPanelValidationCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelValidationCheckDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_CHECK_SELECT_1,	m_bCheckSelect[0]);
	DDX_Control(pDX, IDB_CHECK_SELECT_2,	m_bCheckSelect[1]);
	DDX_Control(pDX, IDB_CHECK_SELECT_3,	m_bCheckSelect[2]);
	DDX_Control(pDX, IDB_CHECK_SELECT_4,	m_bCheckSelect[3]);
	DDX_Control(pDX, IDB_CHECK_SELECT_5,	m_bCheckSelect[4]);
	DDX_Control(pDX, IDB_CHECK_SELECT_6,	m_bCheckSelect[5]);
	DDX_Control(pDX, IDB_CHECK_SELECT_7,	m_bCheckSelect[6]);
	DDX_Control(pDX, IDB_CHECK_SELECT_8,	m_bCheckSelect[7]);
	DDX_Control(pDX, IDB_CHECK_SELECT_9,	m_bCheckSelect[8]);
	DDX_Control(pDX, IDB_CHECK_SELECT_10,	m_bCheckSelect[9]);
	DDX_Control(pDX, IDB_CHECK_SELECT_11,	m_bCheckSelect[10]);
	DDX_Control(pDX, IDB_CHECK_SELECT_12,	m_bCheckSelect[11]);
	DDX_Control(pDX, IDB_CHECK_SELECT_13,	m_bCheckSelect[12]);
	DDX_Control(pDX, IDB_CHECK_SELECT_14,	m_bCheckSelect[13]);
	DDX_Control(pDX, IDB_CHECK_SELECT_15,	m_bCheckSelect[14]);
	DDX_Control(pDX, IDB_CHECK_SELECT_16,	m_bCheckSelect[15]);
	DDX_Control(pDX, IDB_CHECK_SELECT_17,	m_bCheckSelect[16]);
	DDX_Control(pDX, IDB_CHECK_SELECT_18,	m_bCheckSelect[17]);
	DDX_Control(pDX, IDB_CHECK_SELECT_19,	m_bCheckSelect[18]);
	DDX_Control(pDX, IDB_CHECK_SELECT_20,	m_bCheckSelect[19]);
	DDX_Control(pDX, IDB_CHECK_SELECT_21,	m_bCheckSelect[20]);
	DDX_Control(pDX, IDB_CHECK_SELECT_22,	m_bCheckSelect[21]);
	DDX_Control(pDX, IDB_CHECK_SELECT_23,	m_bCheckSelect[22]);
	DDX_Control(pDX, IDB_CHECK_SELECT_24,	m_bCheckSelect[23]);
	DDX_Control(pDX, IDB_CHECK_SELECT_25,	m_bCheckSelect[24]);
	DDX_Control(pDX, IDB_CHECK_SELECT_26,	m_bCheckSelect[25]);
	DDX_Control(pDX, IDB_CHECK_SELECT_27,	m_bCheckSelect[26]);
	DDX_Control(pDX, IDB_CHECK_SELECT_28,	m_bCheckSelect[27]);
	DDX_Control(pDX, IDB_CHECK_SELECT_29,	m_bCheckSelect[28]);
	DDX_Control(pDX, IDB_CHECK_SELECT_30,	m_bCheckSelect[29]);
	DDX_Control(pDX, IDB_CHECK_SELECT_31,	m_bCheckSelect[30]);
	DDX_Control(pDX, IDB_CHECK_SELECT_32,	m_bCheckSelect[31]);

	DDX_Control(pDX, IDC_CHECK_ITEM_1,	m_lblCheckItem[0]);
	DDX_Control(pDX, IDC_CHECK_ITEM_2,	m_lblCheckItem[1]);
	DDX_Control(pDX, IDC_CHECK_ITEM_3,	m_lblCheckItem[2]);
	DDX_Control(pDX, IDC_CHECK_ITEM_4,	m_lblCheckItem[3]);
	DDX_Control(pDX, IDC_CHECK_ITEM_5,	m_lblCheckItem[4]);
	DDX_Control(pDX, IDC_CHECK_ITEM_6,	m_lblCheckItem[5]);
	DDX_Control(pDX, IDC_CHECK_ITEM_7,	m_lblCheckItem[6]);
	DDX_Control(pDX, IDC_CHECK_ITEM_8,	m_lblCheckItem[7]);
	DDX_Control(pDX, IDC_CHECK_ITEM_9,	m_lblCheckItem[8]);
	DDX_Control(pDX, IDC_CHECK_ITEM_10,	m_lblCheckItem[9]);
	DDX_Control(pDX, IDC_CHECK_ITEM_11,	m_lblCheckItem[10]);
	DDX_Control(pDX, IDC_CHECK_ITEM_12,	m_lblCheckItem[11]);
	DDX_Control(pDX, IDC_CHECK_ITEM_13,	m_lblCheckItem[12]);
	DDX_Control(pDX, IDC_CHECK_ITEM_14,	m_lblCheckItem[13]);
	DDX_Control(pDX, IDC_CHECK_ITEM_15,	m_lblCheckItem[14]);
	DDX_Control(pDX, IDC_CHECK_ITEM_16,	m_lblCheckItem[15]);
	DDX_Control(pDX, IDC_CHECK_ITEM_17,	m_lblCheckItem[16]);
	DDX_Control(pDX, IDC_CHECK_ITEM_18,	m_lblCheckItem[17]);
	DDX_Control(pDX, IDC_CHECK_ITEM_19,	m_lblCheckItem[18]);
	DDX_Control(pDX, IDC_CHECK_ITEM_20,	m_lblCheckItem[19]);
	DDX_Control(pDX, IDC_CHECK_ITEM_21,	m_lblCheckItem[20]);
	DDX_Control(pDX, IDC_CHECK_ITEM_22,	m_lblCheckItem[21]);
	DDX_Control(pDX, IDC_CHECK_ITEM_23,	m_lblCheckItem[22]);
	DDX_Control(pDX, IDC_CHECK_ITEM_24,	m_lblCheckItem[23]);	
	DDX_Control(pDX, IDC_CHECK_ITEM_25,	m_lblCheckItem[24]);	
	DDX_Control(pDX, IDC_CHECK_ITEM_26,	m_lblCheckItem[25]);	
	DDX_Control(pDX, IDC_CHECK_ITEM_27,	m_lblCheckItem[26]);	
	DDX_Control(pDX, IDC_CHECK_ITEM_28,	m_lblCheckItem[27]);	
	DDX_Control(pDX, IDC_CHECK_ITEM_29,	m_lblCheckItem[28]);	
	DDX_Control(pDX, IDC_CHECK_ITEM_30,	m_lblCheckItem[29]);	
	DDX_Control(pDX, IDC_CHECK_ITEM_31,	m_lblCheckItem[30]);	
	DDX_Control(pDX, IDC_CHECK_ITEM_32,	m_lblCheckItem[31]);	
}


BEGIN_MESSAGE_MAP(CPanelValidationCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CPanelValidationCheckDlg)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelValidationCheckDlg message handlers

BOOL CPanelValidationCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strTemp;
//저장
	ReadDisplayMessage(MAIN_AUTO, _T("PanelValidationCheckDlg_1"), &strTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(strTemp);
//나가기
	ReadDisplayMessage(MAIN_AUTO, _T("PanelValidationCheckDlg_2"), &strTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(strTemp);

	
	// TODO: Add extra initialization here
	m_lblCheckItem[0].SetWindowText(_T("HPanel ID"));
	m_lblCheckItem[1].SetWindowText(_T("EPanel ID"));
	m_lblCheckItem[2].SetWindowText(_T("Batch ID"));
	m_lblCheckItem[3].SetWindowText(_T("Prod Type"));
	m_lblCheckItem[4].SetWindowText(_T("Prod Kind"));
	m_lblCheckItem[5].SetWindowText(_T("Device ID"));
	m_lblCheckItem[6].SetWindowText(_T("StepID"));
	m_lblCheckItem[7].SetWindowText(_T("PPID"));
	m_lblCheckItem[8].SetWindowText(_T("Thickness"));
	m_lblCheckItem[9].SetWindowText(_T("InsFlag"));
	m_lblCheckItem[10].SetWindowText(_T("PanelSize"));
	m_lblCheckItem[11].SetWindowText(_T("PanelPosition"));
	m_lblCheckItem[12].SetWindowText(_T("Count1"));
	m_lblCheckItem[13].SetWindowText(_T("Count2"));
	m_lblCheckItem[14].SetWindowText(_T("Grade"));
	m_lblCheckItem[15].SetWindowText(_T("Comment"));
	m_lblCheckItem[16].SetWindowText(_T("CompSize"));
	m_lblCheckItem[17].SetWindowText(_T("ReadingFlag"));
	m_lblCheckItem[18].SetWindowText(_T("PanelState"));
	m_lblCheckItem[19].SetWindowText(_T("Judgement"));
	m_lblCheckItem[20].SetWindowText(_T("Code"));
	m_lblCheckItem[21].SetWindowText(_T("RunLine"));
	m_lblCheckItem[22].SetWindowText(_T("UniqueId"));
	m_lblCheckItem[23].SetWindowText(_T("PairHPanelID"));
	m_lblCheckItem[24].SetWindowText(_T("PairEPanelID"));
	m_lblCheckItem[25].SetWindowText(_T("PairGrade"));
	m_lblCheckItem[26].SetWindowText(_T("PairUniqueId"));
	m_lblCheckItem[27].SetWindowText(_T("FlowRecipe"));
	m_lblCheckItem[28].SetWindowText(_T("Reserved0"));
	m_lblCheckItem[29].SetWindowText(_T("BitsSignals"));
	m_lblCheckItem[30].SetWindowText(_T("ReferData"));
	m_lblCheckItem[31].SetWindowText(_T("NONE"));

	for(int i=0; i<DEF_MAX_CHECK_NUMBER; i++)
	{
		m_CheckSelect[i] = m_plnkSystemData->m_bValidationCheckSelect[i];
//		m_sCheckItem[i] = m_plnkSystemData->m_strValidationCheckItem[i];
	}

	//NONE_____________________
	m_bCheckSelect[31].EnableWindow(FALSE);
	m_lblCheckItem[31].EnableWindow(FALSE);
	//_________________________

	m_uiTimerID = SetTimer(1, 200, NULL);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPanelValidationCheckDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		for(int i=0; i<DEF_MAX_CHECK_NUMBER; i++)
		{
			if(m_CheckSelect[i] == TRUE)
				m_bCheckSelect[i].SetBackColorInterior(RED);
			else
				m_bCheckSelect[i].SetBackColorInterior(DKGRAY);

//			m_lblCheckItem[i].SetCaption(m_sCheckItem[i]);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CPanelValidationCheckDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPanelValidationCheckDlg)
	ON_EVENT(CPanelValidationCheckDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_1, -600 /* Click */, OnClickCheckSelect1, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_2, -600 /* Click */, OnClickCheckSelect2, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_3, -600 /* Click */, OnClickCheckSelect3, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_4, -600 /* Click */, OnClickCheckSelect4, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_5, -600 /* Click */, OnClickCheckSelect5, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_6, -600 /* Click */, OnClickCheckSelect6, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_7, -600 /* Click */, OnClickCheckSelect7, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_8, -600 /* Click */, OnClickCheckSelect8, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_9, -600 /* Click */, OnClickCheckSelect9, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_10, -600 /* Click */, OnClickCheckSelect10, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_11, -600 /* Click */, OnClickCheckSelect11, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_12, -600 /* Click */, OnClickCheckSelect12, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_13, -600 /* Click */, OnClickCheckSelect13, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_14, -600 /* Click */, OnClickCheckSelect14, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_15, -600 /* Click */, OnClickCheckSelect15, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_16, -600 /* Click */, OnClickCheckSelect16, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_17, -600 /* Click */, OnClickCheckSelect17, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_18, -600 /* Click */, OnClickCheckSelect18, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_19, -600 /* Click */, OnClickCheckSelect19, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_20, -600 /* Click */, OnClickCheckSelect20, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_21, -600 /* Click */, OnClickCheckSelect21, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_22, -600 /* Click */, OnClickCheckSelect22, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_23, -600 /* Click */, OnClickCheckSelect23, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_24, -600 /* Click */, OnClickCheckSelect24, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_25, -600 /* Click */, OnClickCheckSelect25, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_26, -600 /* Click */, OnClickCheckSelect26, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_27, -600 /* Click */, OnClickCheckSelect27, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_28, -600 /* Click */, OnClickCheckSelect28, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_29, -600 /* Click */, OnClickCheckSelect29, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_30, -600 /* Click */, OnClickCheckSelect30, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_31, -600 /* Click */, OnClickCheckSelect31, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDB_CHECK_SELECT_32, -600 /* Click */, OnClickCheckSelect32, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_1, -600 /* Click */, OnClickCheckItem1, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_2, -600 /* Click */, OnClickCheckItem2, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_3, -600 /* Click */, OnClickCheckItem3, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_4, -600 /* Click */, OnClickCheckItem4, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_5, -600 /* Click */, OnClickCheckItem5, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_6, -600 /* Click */, OnClickCheckItem6, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_7, -600 /* Click */, OnClickCheckItem7, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_8, -600 /* Click */, OnClickCheckItem8, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_9, -600 /* Click */, OnClickCheckItem9, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_10, -600 /* Click */, OnClickCheckItem10, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_11, -600 /* Click */, OnClickCheckItem11, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_12, -600 /* Click */, OnClickCheckItem12, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_13, -600 /* Click */, OnClickCheckItem13, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_14, -600 /* Click */, OnClickCheckItem14, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_15, -600 /* Click */, OnClickCheckItem15, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_16, -600 /* Click */, OnClickCheckItem16, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_17, -600 /* Click */, OnClickCheckItem17, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_18, -600 /* Click */, OnClickCheckItem18, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_19, -600 /* Click */, OnClickCheckItem19, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_20, -600 /* Click */, OnClickCheckItem20, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_21, -600 /* Click */, OnClickCheckItem21, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_22, -600 /* Click */, OnClickCheckItem22, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_23, -600 /* Click */, OnClickCheckItem23, VTS_NONE)
	ON_EVENT(CPanelValidationCheckDlg, IDC_CHECK_ITEM_24, -600 /* Click */, OnClickCheckItem24, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPanelValidationCheckDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	//Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40000, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

	for(int i=0; i<DEF_MAX_CHECK_NUMBER; i++)
	{
		m_plnkSystemData->m_bValidationCheckSelect[i] = m_CheckSelect[i];
//		m_plnkSystemData->m_strValidationCheckItem[i] = m_sCheckItem[i];
	}

	m_plnkSystemData->WriteData();
}

void CPanelValidationCheckDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CDialog::OnCancel();
}

void CPanelValidationCheckDlg::OnClickCheckSelect1() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[0] == FALSE)
		m_CheckSelect[0] = TRUE;
	else
		m_CheckSelect[0] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect2() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[1] == FALSE)
		m_CheckSelect[1] = TRUE;
	else
		m_CheckSelect[1] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect3() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[2] == FALSE)
		m_CheckSelect[2] = TRUE;
	else
		m_CheckSelect[2] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect4() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[3] == FALSE)
		m_CheckSelect[3] = TRUE;
	else
		m_CheckSelect[3] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect5() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[4] == FALSE)
		m_CheckSelect[4] = TRUE;
	else
		m_CheckSelect[4] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect6() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[5] == FALSE)
		m_CheckSelect[5] = TRUE;
	else
		m_CheckSelect[5] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect7() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[6] == FALSE)
		m_CheckSelect[6] = TRUE;
	else
		m_CheckSelect[6] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect8() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[7] == FALSE)
		m_CheckSelect[7] = TRUE;
	else
		m_CheckSelect[7] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect9() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[8] == FALSE)
		m_CheckSelect[8] = TRUE;
	else
		m_CheckSelect[8] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect10() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[9] == FALSE)
		m_CheckSelect[9] = TRUE;
	else
		m_CheckSelect[9] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect11() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[10] == FALSE)
		m_CheckSelect[10] = TRUE;
	else
		m_CheckSelect[10] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect12() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[11] == FALSE)
		m_CheckSelect[11] = TRUE;
	else
		m_CheckSelect[11] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect13() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[12] == FALSE)
		m_CheckSelect[12] = TRUE;
	else
		m_CheckSelect[12] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect14() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[13] == FALSE)
		m_CheckSelect[13] = TRUE;
	else
		m_CheckSelect[13] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect15() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[14] == FALSE)
		m_CheckSelect[14] = TRUE;
	else
		m_CheckSelect[14] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect16() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[15] == FALSE)
		m_CheckSelect[15] = TRUE;
	else
		m_CheckSelect[15] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect17() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[16] == FALSE)
		m_CheckSelect[16] = TRUE;
	else
		m_CheckSelect[16] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect18() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[17] == FALSE)
		m_CheckSelect[17] = TRUE;
	else
		m_CheckSelect[17] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect19() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[18] == FALSE)
		m_CheckSelect[18] = TRUE;
	else
		m_CheckSelect[18] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect20() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[19] == FALSE)
		m_CheckSelect[19] = TRUE;
	else
		m_CheckSelect[19] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect21() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[20] == FALSE)
		m_CheckSelect[20] = TRUE;
	else
		m_CheckSelect[20] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect22() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[21] == FALSE)
		m_CheckSelect[21] = TRUE;
	else
		m_CheckSelect[21] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect23() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[22] == FALSE)
		m_CheckSelect[22] = TRUE;
	else
		m_CheckSelect[22] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect24() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[23] == FALSE)
		m_CheckSelect[23] = TRUE;
	else
		m_CheckSelect[23] = FALSE;
}


void CPanelValidationCheckDlg::OnClickCheckSelect25() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[24] == FALSE)
		m_CheckSelect[24] = TRUE;
	else
		m_CheckSelect[24] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect26() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[25] == FALSE)
		m_CheckSelect[25] = TRUE;
	else
		m_CheckSelect[25] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect27() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[26] == FALSE)
		m_CheckSelect[26] = TRUE;
	else
		m_CheckSelect[26] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect28() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[27] == FALSE)
		m_CheckSelect[27] = TRUE;
	else
		m_CheckSelect[27] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect29() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[28] == FALSE)
		m_CheckSelect[28] = TRUE;
	else
		m_CheckSelect[28] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect30() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[29] == FALSE)
		m_CheckSelect[29] = TRUE;
	else
		m_CheckSelect[29] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect31() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[30] == FALSE)
		m_CheckSelect[30] = TRUE;
	else
		m_CheckSelect[30] = FALSE;
}

void CPanelValidationCheckDlg::OnClickCheckSelect32() 
{
	// TODO: Add your control notification handler code here
	if(m_CheckSelect[31] == FALSE)
		m_CheckSelect[31] = TRUE;
	else
		m_CheckSelect[31] = FALSE;
}




void CPanelValidationCheckDlg::OnClickCheckItem1() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(0);
}

void CPanelValidationCheckDlg::OnClickCheckItem2() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(1);
}

void CPanelValidationCheckDlg::OnClickCheckItem3() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(2);
}

void CPanelValidationCheckDlg::OnClickCheckItem4() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(3);
}

void CPanelValidationCheckDlg::OnClickCheckItem5() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(4);
}

void CPanelValidationCheckDlg::OnClickCheckItem6() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(5);
}

void CPanelValidationCheckDlg::OnClickCheckItem7() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(6);
}

void CPanelValidationCheckDlg::OnClickCheckItem8() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(7);
}

void CPanelValidationCheckDlg::OnClickCheckItem9() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(8);
}

void CPanelValidationCheckDlg::OnClickCheckItem10() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(9);
}

void CPanelValidationCheckDlg::OnClickCheckItem11() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(10);
}

void CPanelValidationCheckDlg::OnClickCheckItem12() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(11);
}

void CPanelValidationCheckDlg::OnClickCheckItem13() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(12);
}

void CPanelValidationCheckDlg::OnClickCheckItem14() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(13);
}

void CPanelValidationCheckDlg::OnClickCheckItem15() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(14);
}

void CPanelValidationCheckDlg::OnClickCheckItem16() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(15);
}

void CPanelValidationCheckDlg::OnClickCheckItem17() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(16);
}

void CPanelValidationCheckDlg::OnClickCheckItem18() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(17);
}

void CPanelValidationCheckDlg::OnClickCheckItem19() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(18);
}

void CPanelValidationCheckDlg::OnClickCheckItem20() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(19);
}

void CPanelValidationCheckDlg::OnClickCheckItem21() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(20);
}

void CPanelValidationCheckDlg::OnClickCheckItem22() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(21);
}

void CPanelValidationCheckDlg::OnClickCheckItem23() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(22);
}

void CPanelValidationCheckDlg::OnClickCheckItem24() 
{
	// TODO: Add your control notification handler code here
	selectPanelItem(23);
}

void CPanelValidationCheckDlg::selectPanelItem(int iIndex)
{
	/*
	ASSERT(iIndex >= 0 && iIndex < DEF_MAX_CHECK_NUMBER);

	WCHAR buf[100], szTemp[100];
	memset(buf, 0, sizeof(buf));	
	memset(szTemp, 0, sizeof(szTemp));
	swprintf(buf, _T("%s"), m_sCheckItem[iIndex]);
	//_______________

	if(!GetValueBox(szTemp, 20, buf, _T("Input the Item name.")))
		return;

	m_sCheckItem[iIndex].Format(_T("%s"), szTemp);
	m_lblCheckItem[iIndex].SetCaption(m_sCheckItem[iIndex]);
	*/
}
