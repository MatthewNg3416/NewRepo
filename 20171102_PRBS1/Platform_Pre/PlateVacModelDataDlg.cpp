// PlateVacModelDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "PlateVacModelDataDlg.h"

#include "MPlatformOlbSystem.h"
#include "MPanelData.h"
#include "MTrsAutoManager.h"


extern MPlatformOlbSystem	MOlbSystemPre;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlateVacModelDataDlg dialog


CPlateVacModelDataDlg::CPlateVacModelDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlateVacModelDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlateVacModelDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkPanelData	= MOlbSystemPre.GetPanelData();
}


void CPlateVacModelDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlateVacModelDataDlg)
	DDX_Control(pDX, IDB_SAVE, m_btnSave);
	DDX_Control(pDX, IDB_EXIT, m_btnExit);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_PLATE_VAC_0, m_btnPlateVacBtn[0][0]);
	DDX_Control(pDX, IDB_PLATE_VAC_1, m_btnPlateVacBtn[0][1]);
	DDX_Control(pDX, IDB_PLATE_VAC_2, m_btnPlateVacBtn[0][2]);
	DDX_Control(pDX, IDB_PLATE_VAC_3, m_btnPlateVacBtn[0][3]);

	DDX_Control(pDX, IDC_VALUE_0, m_lblPlateVac[0][0]);
	DDX_Control(pDX, IDC_VALUE_1, m_lblPlateVac[0][1]);
	DDX_Control(pDX, IDC_VALUE_2, m_lblPlateVac[0][2]);
	DDX_Control(pDX, IDC_VALUE_3, m_lblPlateVac[0][3]);


	DDX_Control(pDX, IDB_PLATE_VAC_4, m_btnPlateVacBtn[1][0]);
	DDX_Control(pDX, IDB_PLATE_VAC_5, m_btnPlateVacBtn[1][1]);
	DDX_Control(pDX, IDB_PLATE_VAC_6, m_btnPlateVacBtn[1][2]);
	DDX_Control(pDX, IDB_PLATE_VAC_7, m_btnPlateVacBtn[1][3]);

	DDX_Control(pDX, IDC_VALUE_4, m_lblPlateVac[1][0]);
	DDX_Control(pDX, IDC_VALUE_5, m_lblPlateVac[1][1]);
	DDX_Control(pDX, IDC_VALUE_6, m_lblPlateVac[1][2]);
	DDX_Control(pDX, IDC_VALUE_7, m_lblPlateVac[1][3]);

	DDX_Control(pDX, IDB_PLATE_VAC_8, m_btnTHandlerVac[0]);
	DDX_Control(pDX, IDB_PLATE_VAC_9, m_btnTHandlerVac[1]);
	DDX_Control(pDX, IDB_PLATE_VAC_10, m_btnTHandlerVac[2]);
	DDX_Control(pDX, IDC_VALUE_8, m_lblTHandlerVac[0]);
	DDX_Control(pDX, IDC_VALUE_9, m_lblTHandlerVac[1]);
	DDX_Control(pDX, IDC_VALUE_10, m_lblTHandlerVac[2]);
}


BEGIN_MESSAGE_MAP(CPlateVacModelDataDlg, CDialog)
	//{{AFX_MSG_MAP(CPlateVacModelDataDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlateVacModelDataDlg message handlers

BOOL CPlateVacModelDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString szTextTemp;
	//나가기
	ReadDisplayMessage(MAIN_DATA, _T("PlateVacModelDataDlg_1"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
		
	//저장
	ReadDisplayMessage(MAIN_DATA, _T("PlateVacModelDataDlg_2"), &szTextTemp);	
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);


	int i, j;

	for (i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
	{
		for (j=0; j<DEF_MAX_AIRVACUUM; j++)
		{
			m_bACFOnOff[i][j] = m_plnkPanelData->m_bUseAirVacuum[i][j];
			UpdateDisplay(i, j);
		}
	}
	for (i=DEF_MAX_WORK_SCHEDULE; i<DEF_MAX_MANUAL_WORKCNT; i++)
	{
		for (j=0; j<DEF_MAX_AIRVACUUM; j++)
		{
			m_btnPlateVacBtn[i][j].EnableWindow(FALSE);
			m_lblPlateVac[i][j].EnableWindow(FALSE);
		}
	}

    //SJ_YYK 161104 Add..
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		m_bUseTHandlerVac[i] = m_plnkPanelData->m_bUseTHandlerVac[i];
		updateDisplayTHandlerVac(i);
	}

	
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
	{
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDB_SAVE)->EnableWindow(TRUE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CPlateVacModelDataDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CPlateVacModelDataDlg)
	ON_EVENT(CPlateVacModelDataDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_0, -600 /* Click */, OnClickPlateVac0, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_1, -600 /* Click */, OnClickPlateVac1, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_2, -600 /* Click */, OnClickPlateVac2, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_3, -600 /* Click */, OnClickPlateVac3, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_4, -600 /* Click */, OnClickPlateVac4, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_5, -600 /* Click */, OnClickPlateVac5, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_6, -600 /* Click */, OnClickPlateVac6, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_7, -600 /* Click */, OnClickPlateVac7, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_8, -600 /* Click */, OnClickPlateVac8, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_9, -600 /* Click */, OnClickPlateVac9, VTS_NONE)
	ON_EVENT(CPlateVacModelDataDlg, IDB_PLATE_VAC_10, -600 /* Click */, OnClickPlateVac10, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPlateVacModelDataDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	int i, j;
	//Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40000, _T("Question"), M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

	for (i=0; i<DEF_MAX_WORK_SCHEDULE; i++)
	{
		for (j=0; j<DEF_MAX_AIRVACUUM; j++)
		{
			m_plnkPanelData->m_bUseAirVacuum[i][j] = m_bACFOnOff[i][j];
		}
	}

	//SJ_YYK 161104 Modify..
	for (i=0; i<DEF_MAX_THANDLER_VAC; i++)
	{
		m_plnkPanelData->m_bUseTHandlerVac[i] = m_bUseTHandlerVac[i];
	}

	// ACF Length Data Save
	m_plnkPanelData->WriteData();
}

void CPlateVacModelDataDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CPlateVacModelDataDlg::UpdateDisplay(int iWCnt, int iAirVac) 
{
	CString strValue;

	if (m_bACFOnOff[iWCnt][iAirVac])
		m_lblPlateVac[iWCnt][iAirVac].SetCaption(_T("Use"));
	else
		m_lblPlateVac[iWCnt][iAirVac].SetCaption(_T("Not Use"));
}

void CPlateVacModelDataDlg::ChangeParameterValue(int iWCnt, int iAirVac)
{
	if (m_bACFOnOff[iWCnt][iAirVac])
		m_bACFOnOff[iWCnt][iAirVac] = FALSE;
	else
		m_bACFOnOff[iWCnt][iAirVac] = TRUE;
}

void CPlateVacModelDataDlg::changeParamTHandlerVac(int iVacID)
{
	if (TRUE == m_bUseTHandlerVac[iVacID])
		m_bUseTHandlerVac[iVacID] = FALSE;
	else
		m_bUseTHandlerVac[iVacID] = TRUE;
}

void CPlateVacModelDataDlg::updateDisplayTHandlerVac(int iVacID)
{
	if (TRUE == m_bUseTHandlerVac[iVacID])
		m_lblTHandlerVac[iVacID].SetCaption(_T("Use"));
	else
		m_lblTHandlerVac[iVacID].SetCaption(_T("NotUse"));
}

void CPlateVacModelDataDlg::OnClickPlateVac0() 
{
	// TODO: Add your control notification handler code here
	m_iWCnt = 0;
	m_iAirVac = 0;
	ChangeParameterValue(m_iWCnt, m_iAirVac);
	UpdateDisplay(m_iWCnt, m_iAirVac);
}

void CPlateVacModelDataDlg::OnClickPlateVac1() 
{
	// TODO: Add your control notification handler code here
	m_iWCnt = 0;
	m_iAirVac = 1;
	ChangeParameterValue(m_iWCnt, m_iAirVac);
	UpdateDisplay(m_iWCnt, m_iAirVac);
}

void CPlateVacModelDataDlg::OnClickPlateVac2() 
{
	// TODO: Add your control notification handler code here
	m_iWCnt = 0;
	m_iAirVac = 2;
	ChangeParameterValue(m_iWCnt, m_iAirVac);
	UpdateDisplay(m_iWCnt, m_iAirVac);
}

void CPlateVacModelDataDlg::OnClickPlateVac3() 
{
	// TODO: Add your control notification handler code here
	m_iWCnt = 0;
	m_iAirVac = 3;
	ChangeParameterValue(m_iWCnt, m_iAirVac);
	UpdateDisplay(m_iWCnt, m_iAirVac);
}

void CPlateVacModelDataDlg::OnClickPlateVac4() 
{
	// TODO: Add your control notification handler code here
	m_iWCnt = 1;
	m_iAirVac = 0;
	ChangeParameterValue(m_iWCnt, m_iAirVac);
	UpdateDisplay(m_iWCnt, m_iAirVac);
}

void CPlateVacModelDataDlg::OnClickPlateVac5() 
{
	// TODO: Add your control notification handler code here
	m_iWCnt = 1;
	m_iAirVac = 1;
	ChangeParameterValue(m_iWCnt, m_iAirVac);
	UpdateDisplay(m_iWCnt, m_iAirVac);
}

void CPlateVacModelDataDlg::OnClickPlateVac6() 
{
	// TODO: Add your control notification handler code here
	m_iWCnt = 1;
	m_iAirVac = 2;
	ChangeParameterValue(m_iWCnt, m_iAirVac);
	UpdateDisplay(m_iWCnt, m_iAirVac);
}

void CPlateVacModelDataDlg::OnClickPlateVac7() 
{
	// TODO: Add your control notification handler code here
	m_iWCnt = 1;
	m_iAirVac = 3;
	ChangeParameterValue(m_iWCnt, m_iAirVac);
	UpdateDisplay(m_iWCnt, m_iAirVac);
}

void CPlateVacModelDataDlg::OnClickPlateVac8() 
{
	// TODO: Add your control notification handler code here
	changeParamTHandlerVac(0);
	updateDisplayTHandlerVac(0);
}

void CPlateVacModelDataDlg::OnClickPlateVac9() 
{
	// TODO: Add your control notification handler code here
	changeParamTHandlerVac(1);
	updateDisplayTHandlerVac(1);
}

void CPlateVacModelDataDlg::OnClickPlateVac10() 
{
	// TODO: Add your control notification handler code here
	changeParamTHandlerVac(2);
	updateDisplayTHandlerVac(2);
}
