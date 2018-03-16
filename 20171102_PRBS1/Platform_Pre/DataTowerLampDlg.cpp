// DataTowerLampDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataTowerLampDlg.h"
#include "MPlatformOlbSystem.h"
#include "CommonMsgDlg.h"
#include "SystemThreeBtnDlg.h"
#include "SystemBtnDlg.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern MPlatformOlbSystem	MOlbSystemPre;
/////////////////////////////////////////////////////////////////////////////
// CDataTowerLampDlg dialog

CDataTowerLampDlg::CDataTowerLampDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataTowerLampDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataTowerLampDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_plnkTowerLampData	= MOlbSystemPre.GetTowerLampData();
	memset(m_rgOldValue, NULL, sizeof(m_rgOldValue));
	memset(m_rgNewValue, NULL, sizeof(m_rgNewValue));
}

void CDataTowerLampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataTowerLampDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LAMP_STATE_RUN_RED, m_lblRowData[0][0]);
	DDX_Control(pDX, IDC_LAMP_STATE_RUN_YELLOW, m_lblRowData[0][1]);
	DDX_Control(pDX, IDC_LAMP_STATE_RUN_GREEN, m_lblRowData[0][2]);
	DDX_Control(pDX, IDC_LAMP_STATE_RUN_BUZZER, m_lblRowData[0][3]);
	DDX_Control(pDX, IDC_LAMP_STATE_RUN_START, m_lblRowData[0][4]);
	DDX_Control(pDX, IDC_LAMP_STATE_RUN_STOP, m_lblRowData[0][5]);

	DDX_Control(pDX, IDC_LAMP_STATE_STEP_STOP_RED, m_lblRowData[1][0]);
	DDX_Control(pDX, IDC_LAMP_STATE_STEP_STOP_YELLOW, m_lblRowData[1][1]);
	DDX_Control(pDX, IDC_LAMP_STATE_STEP_STOP_GREEN, m_lblRowData[1][2]);
	DDX_Control(pDX, IDC_LAMP_STATE_STEP_STOP_BUZZER, m_lblRowData[1][3]);
	DDX_Control(pDX, IDC_LAMP_STATE_STEP_STOP_START, m_lblRowData[1][4]);
	DDX_Control(pDX, IDC_LAMP_STATE_STEP_STOP_STOP, m_lblRowData[1][5]);

	DDX_Control(pDX, IDC_LAMP_STATE_ERROR_STOP_RED, m_lblRowData[2][0]);
	DDX_Control(pDX, IDC_LAMP_STATE_ERROR_STOP_YELLOW, m_lblRowData[2][1]);
	DDX_Control(pDX, IDC_LAMP_STATE_ERROR_STOP_GREEN, m_lblRowData[2][2]);
	DDX_Control(pDX, IDC_LAMP_STATE_ERROR_STOP_BUZZER, m_lblRowData[2][3]);
	DDX_Control(pDX, IDC_LAMP_STATE_ERROR_STOP_START, m_lblRowData[2][4]);
	DDX_Control(pDX, IDC_LAMP_STATE_ERROR_STOP_STOP, m_lblRowData[2][5]);

	DDX_Control(pDX, IDC_LAMP_STATE_PANEL_EMPTY_RED, m_lblRowData[3][0]);
	DDX_Control(pDX, IDC_LAMP_STATE_PANEL_EMPTY_YELLOW, m_lblRowData[3][1]);
	DDX_Control(pDX, IDC_LAMP_STATE_PANEL_EMPTY_GREEN, m_lblRowData[3][2]);
	DDX_Control(pDX, IDC_LAMP_STATE_PANEL_EMPTY_BUZZER, m_lblRowData[3][3]);
	DDX_Control(pDX, IDC_LAMP_STATE_PANEL_EMPTY_START, m_lblRowData[3][4]);
	DDX_Control(pDX, IDC_LAMP_STATE_PANEL_EMPTY_STOP, m_lblRowData[3][5]);

	
	DDX_Control(pDX, IDC_LAMP_STATE_MATERIAL_CHANGE_RED, m_lblRowData[4][0]);
	DDX_Control(pDX, IDC_LAMP_STATE_MATERIAL_CHANGE_YELLOW, m_lblRowData[4][1]);
	DDX_Control(pDX, IDC_LAMP_STATE_MATERIAL_CHANGE_GREEN, m_lblRowData[4][2]);
	DDX_Control(pDX, IDC_LAMP_STATE_MATERIAL_CHANGE_BUZZER, m_lblRowData[4][3]);
	DDX_Control(pDX, IDC_LAMP_STATE_MATERIAL_CHANGE_START, m_lblRowData[4][4]);
	DDX_Control(pDX, IDC_LAMP_STATE_MATERIAL_CHANGE_STOP, m_lblRowData[4][5]);


	DDX_Control(pDX, IDC_LAMP_STATE_OP_CALL_RED, m_lblRowData[5][0]);
	DDX_Control(pDX, IDC_LAMP_STATE_OP_CALL_YELLOW, m_lblRowData[5][1]);
	DDX_Control(pDX, IDC_LAMP_STATE_OP_CALL_GREEN, m_lblRowData[5][2]);
	DDX_Control(pDX, IDC_LAMP_STATE_OP_CALL_BUZZER, m_lblRowData[5][3]);
	DDX_Control(pDX, IDC_LAMP_STATE_OP_CALL_START, m_lblRowData[5][4]);
	DDX_Control(pDX, IDC_LAMP_STATE_OP_CALL_STOP, m_lblRowData[5][5]);

	DDX_Control(pDX, IDC_DELAY_TIME,		m_lblDelayTime);
}


BEGIN_MESSAGE_MAP(CDataTowerLampDlg, CDialog)
	//{{AFX_MSG_MAP(CDataTowerLampDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTowerLampDlg message handlers

BOOL CDataTowerLampDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_iSelectedRow = DEF_TOWER_MAX_ROW;

	m_rgdTowerLampData[0][0] = m_plnkTowerLampData->m_bUseRedLamp_NomalRun;
	m_rgdTowerLampData[1][0] = m_plnkTowerLampData->m_bUseRedLamp_StepStop;
	m_rgdTowerLampData[2][0] = m_plnkTowerLampData->m_bUseRedLamp_ErroeStop;
	m_rgdTowerLampData[3][0] = m_plnkTowerLampData->m_bUseRedLamp_PanelEmpty;
	m_rgdTowerLampData[4][0] = m_plnkTowerLampData->m_bUseRedLamp_MaterialChange;
	m_rgdTowerLampData[5][0] = m_plnkTowerLampData->m_bUseRedLamp_OPCall;

	m_rgdTowerLampData[0][1] = m_plnkTowerLampData->m_bUseYellowLamp_NomalRun;
	m_rgdTowerLampData[1][1] = m_plnkTowerLampData->m_bUseYellowLamp_StepStop;
	m_rgdTowerLampData[2][1] = m_plnkTowerLampData->m_bUseYellowLamp_ErroeStop;
	m_rgdTowerLampData[3][1] = m_plnkTowerLampData->m_bUseYellowLamp_PanelEmpty;
	m_rgdTowerLampData[4][1] = m_plnkTowerLampData->m_bUseYellowLamp_MaterialChange;
	m_rgdTowerLampData[5][1] = m_plnkTowerLampData->m_bUseYellowLamp_OPCall;

	m_rgdTowerLampData[0][2] = m_plnkTowerLampData->m_bUseGreenLamp_NomalRun;
	m_rgdTowerLampData[1][2] = m_plnkTowerLampData->m_bUseGreenLamp_StepStop;
	m_rgdTowerLampData[2][2] = m_plnkTowerLampData->m_bUseGreenLamp_ErroeStop;
	m_rgdTowerLampData[3][2] = m_plnkTowerLampData->m_bUseGreenLamp_PanelEmpty;
	m_rgdTowerLampData[4][2] = m_plnkTowerLampData->m_bUseGreenLamp_MaterialChange;
	m_rgdTowerLampData[5][2] = m_plnkTowerLampData->m_bUseGreenLamp_OPCall;

	m_rgdTowerLampData[0][3] = m_plnkTowerLampData->m_bUseBuzzer_NomalRun;
	m_rgdTowerLampData[1][3] = m_plnkTowerLampData->m_bUseBuzzer_StepStop;
	m_rgdTowerLampData[2][3] = m_plnkTowerLampData->m_bUseBuzzer_ErroeStop;
	m_rgdTowerLampData[3][3] = m_plnkTowerLampData->m_bUseBuzzer_PanelEmpty;
	m_rgdTowerLampData[4][3] = m_plnkTowerLampData->m_bUseBuzzer_MaterialChange;
	m_rgdTowerLampData[5][3] = m_plnkTowerLampData->m_bUseBuzzer_OPCall;

	m_rgdTowerLampData[0][4] = m_plnkTowerLampData->m_bUseStartLamp_NomalRun;
	m_rgdTowerLampData[1][4] = m_plnkTowerLampData->m_bUseStartLamp_StepStop;
	m_rgdTowerLampData[2][4] = m_plnkTowerLampData->m_bUseStartLamp_ErroeStop;
	m_rgdTowerLampData[3][4] = m_plnkTowerLampData->m_bUseStartLamp_PanelEmpty;
	m_rgdTowerLampData[4][4] = m_plnkTowerLampData->m_bUseStartLamp_MaterialChange;
	m_rgdTowerLampData[5][4] = m_plnkTowerLampData->m_bUseStartLamp_OPCall;

	m_rgdTowerLampData[0][5] = m_plnkTowerLampData->m_bUseStopLamp_NomalRun;
	m_rgdTowerLampData[1][5] = m_plnkTowerLampData->m_bUseStopLamp_StepStop;
	m_rgdTowerLampData[2][5] = m_plnkTowerLampData->m_bUseStopLamp_ErroeStop;
	m_rgdTowerLampData[3][5] = m_plnkTowerLampData->m_bUseStopLamp_PanelEmpty;
	m_rgdTowerLampData[4][5] = m_plnkTowerLampData->m_bUseStopLamp_MaterialChange;
	m_rgdTowerLampData[5][5] = m_plnkTowerLampData->m_bUseStopLamp_OPCall;

	m_iDelayTime			 = m_plnkTowerLampData->m_iDelayTime;

	UpdateDisplay();

	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
	{
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDB_SAVE)->EnableWindow(TRUE);
	}

	CString szTextTemp;
	
	//저장
	ReadDisplayMessage(MAIN_DATA, _T("DataTowerLampDlg_1"), &szTextTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	//나가기
	ReadDisplayMessage(MAIN_DATA, _T("DataTowerLampDlg_2"), &szTextTemp);
	GetDlgItem(IDCANCEL)->SetWindowText(szTextTemp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDataTowerLampDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataTowerLampDlg)
	ON_EVENT(CDataTowerLampDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDCANCEL, -600 /* Click */, OnClickCancel, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_RED_LAMP_STATE, -600 /* Click */, OnClickRedLampState, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_YELLOW_LAMP_STATE, -600 /* Click */, OnClickYellowLampState, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_GREEN_LAMP_STATE, -600 /* Click */, OnClickGreenLampState, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_LAMP_STATE_RUN, -600 /* Click */, OnClickLampStateRun, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_LAMP_STATE_STEP_STOP, -600 /* Click */, OnClickLampStateStepStop, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_LAMP_STATE_ERROR_STOP, -600 /* Click */, OnClickLampStateErrorStop, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_LAMP_STATE_PANEL_EMPTY, -600 /* Click */, OnClickLampStatePanelEmpty, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_LAMP_STATE_MATERIAL_CHANGE, -600 /* Click */, OnClickLampStateMaterialChange, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_BUZZER_SELECT, -600 /* Click */, OnClickBuzzerSelect, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_LAMP_STATE_OP_CALL, -600 /* Click */, OnClickLampStateOpCall, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_START_LAMP_STATE, -600 /* Click */, OnClickStartLampState, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_STOP_LAMP_STATE, -600 /* Click */, OnClickStopLampState, VTS_NONE)
	ON_EVENT(CDataTowerLampDlg, IDB_DELAY_TIME, -600 /* Click */, OnClickDelayTime, VTS_NONE)

	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataTowerLampDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	
	if (MyMessageBox(_T("Do you want to save the changed data?\r\n변경된 Data를 저장하시겠습니까?"), _T("Save Confirm"), M_ICONQUESTION|M_YESNO, _T("예"), _T("아니오")) == IDNO)
		return;

	m_plnkTowerLampData->m_bUseRedLamp_NomalRun          =	m_rgdTowerLampData[0][0];
	m_plnkTowerLampData->m_bUseRedLamp_StepStop          =	m_rgdTowerLampData[1][0];
	m_plnkTowerLampData->m_bUseRedLamp_ErroeStop         =	m_rgdTowerLampData[2][0];
	m_plnkTowerLampData->m_bUseRedLamp_PanelEmpty        =	m_rgdTowerLampData[3][0];
	m_plnkTowerLampData->m_bUseRedLamp_MaterialChange    =	m_rgdTowerLampData[4][0];
	m_plnkTowerLampData->m_bUseRedLamp_OPCall            =	m_rgdTowerLampData[5][0];
	
	m_plnkTowerLampData->m_bUseYellowLamp_NomalRun       =	m_rgdTowerLampData[0][1];     
	m_plnkTowerLampData->m_bUseYellowLamp_StepStop       =	m_rgdTowerLampData[1][1];     
	m_plnkTowerLampData->m_bUseYellowLamp_ErroeStop      =	m_rgdTowerLampData[2][1];      
	m_plnkTowerLampData->m_bUseYellowLamp_PanelEmpty     =	m_rgdTowerLampData[3][1];       
	m_plnkTowerLampData->m_bUseYellowLamp_MaterialChange =	m_rgdTowerLampData[4][1];           
	m_plnkTowerLampData->m_bUseYellowLamp_OPCall         =	m_rgdTowerLampData[5][1];   
	
	m_plnkTowerLampData->m_bUseGreenLamp_NomalRun        =	m_rgdTowerLampData[0][2];    
	m_plnkTowerLampData->m_bUseGreenLamp_StepStop        =	m_rgdTowerLampData[1][2];    
	m_plnkTowerLampData->m_bUseGreenLamp_ErroeStop       =	m_rgdTowerLampData[2][2];     
	m_plnkTowerLampData->m_bUseGreenLamp_PanelEmpty      =	m_rgdTowerLampData[3][2];      
	m_plnkTowerLampData->m_bUseGreenLamp_MaterialChange  =	m_rgdTowerLampData[4][2];          
	m_plnkTowerLampData->m_bUseGreenLamp_OPCall          =	m_rgdTowerLampData[5][2];  
	
	m_plnkTowerLampData->m_bUseBuzzer_NomalRun           =	m_rgdTowerLampData[0][3]; 
	m_plnkTowerLampData->m_bUseBuzzer_StepStop           =	m_rgdTowerLampData[1][3]; 
	m_plnkTowerLampData->m_bUseBuzzer_ErroeStop          =	m_rgdTowerLampData[2][3];  
	m_plnkTowerLampData->m_bUseBuzzer_PanelEmpty         =	m_rgdTowerLampData[3][3];   
	m_plnkTowerLampData->m_bUseBuzzer_MaterialChange     =	m_rgdTowerLampData[4][3];       
	m_plnkTowerLampData->m_bUseBuzzer_OPCall             =  m_rgdTowerLampData[5][3];
	
	m_plnkTowerLampData->m_bUseStartLamp_NomalRun        =	m_rgdTowerLampData[0][4];    
	m_plnkTowerLampData->m_bUseStartLamp_StepStop        =	m_rgdTowerLampData[1][4];    
	m_plnkTowerLampData->m_bUseStartLamp_ErroeStop       =	m_rgdTowerLampData[2][4];     
	m_plnkTowerLampData->m_bUseStartLamp_PanelEmpty      =	m_rgdTowerLampData[3][4];      
	m_plnkTowerLampData->m_bUseStartLamp_MaterialChange  =	m_rgdTowerLampData[4][4];          
	m_plnkTowerLampData->m_bUseStartLamp_OPCall          =	m_rgdTowerLampData[5][4];  
	
	m_plnkTowerLampData->m_bUseStopLamp_NomalRun         =	m_rgdTowerLampData[0][5];   
	m_plnkTowerLampData->m_bUseStopLamp_StepStop         =	m_rgdTowerLampData[1][5];   
	m_plnkTowerLampData->m_bUseStopLamp_ErroeStop        =	m_rgdTowerLampData[2][5];    
	m_plnkTowerLampData->m_bUseStopLamp_PanelEmpty       =	m_rgdTowerLampData[3][5];     
	m_plnkTowerLampData->m_bUseStopLamp_MaterialChange   =	m_rgdTowerLampData[4][5];         
	m_plnkTowerLampData->m_bUseStopLamp_OPCall           =	m_rgdTowerLampData[5][5]; 

	m_plnkTowerLampData->m_iDelayTime					=	m_iDelayTime;

	m_plnkTowerLampData->WriteData();
}

void CDataTowerLampDlg::OnClickCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CDataTowerLampDlg::OnClickRedLampState() 
{
	// TODO: Add your control notification handler code here
	if((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_TOWER_MAX_ROW))
	{
		//Row가 선택 안되었습니다. 수정하려는 Row를 선택해 주세요.
		MyMessageBox(MY_MESSAGE_BOX, 40701, _T("Row Selection"), M_ICONINFORMATION|M_OK);
		return;
	}
	SelectLamp(0);
}

void CDataTowerLampDlg::OnClickYellowLampState() 
{
	// TODO: Add your control notification handler code here
	if((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_TOWER_MAX_ROW))
	{
		//Row가 선택 안되었습니다. 수정하려는 Row를 선택해 주세요.
		MyMessageBox(MY_MESSAGE_BOX, 40701, _T("Row Selection"), M_ICONINFORMATION|M_OK);
		return;
	}
	SelectLamp(1);
}

void CDataTowerLampDlg::OnClickGreenLampState() 
{
	// TODO: Add your control notification handler code here
	if((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_TOWER_MAX_ROW))
	{
		//Row가 선택 안되었습니다. 수정하려는 Row를 선택해 주세요.
		MyMessageBox(MY_MESSAGE_BOX, 40701, _T("Row Selection"), M_ICONINFORMATION|M_OK);
		return;
	}
	SelectLamp(2);
}

void CDataTowerLampDlg::OnClickBuzzerSelect() 
{
	// TODO: Add your control notification handler code here
	if((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_TOWER_MAX_ROW))
	{
		//Row가 선택 안되었습니다. 수정하려는 Row를 선택해 주세요.
		MyMessageBox(MY_MESSAGE_BOX, 40701, _T("Row Selection"), M_ICONINFORMATION|M_OK);
		return;
	}
	SelectLamp(3);
}

void CDataTowerLampDlg::OnClickStartLampState() 
{
	// TODO: Add your control notification handler code here
	if((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_TOWER_MAX_ROW))
	{
		//Row가 선택 안되었습니다. 수정하려는 Row를 선택해 주세요.
		MyMessageBox(MY_MESSAGE_BOX, 40701, _T("Row Selection"), M_ICONINFORMATION|M_OK);
		return;
	}
	SelectLamp(4);
	
}

void CDataTowerLampDlg::OnClickStopLampState() 
{
	// TODO: Add your control notification handler code here
	if((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_TOWER_MAX_ROW))
	{
		//Row가 선택 안되었습니다. 수정하려는 Row를 선택해 주세요.
		MyMessageBox(MY_MESSAGE_BOX, 40701, _T("Row Selection"), M_ICONINFORMATION|M_OK);
		return;
	}
	SelectLamp(5);
	
}

void CDataTowerLampDlg::OnClickLampStateRun() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedRow = 0;	
	
}

void CDataTowerLampDlg::OnClickLampStateStepStop() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedRow = 1;	
	
}

void CDataTowerLampDlg::OnClickLampStateErrorStop() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedRow = 2;
}

void CDataTowerLampDlg::OnClickLampStatePanelEmpty() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedRow = 3;
	
}

void CDataTowerLampDlg::OnClickLampStateMaterialChange() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedRow = 4;
	
}
void CDataTowerLampDlg::OnClickLampStateOpCall() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedRow = 5;
}

void CDataTowerLampDlg::UpdateDisplay()
{
	for(int i=0; i<DEF_TOWER_MAX_COLUMN; i++)
	{
		for(int j=0; j<DEF_TOWER_MAX_ROW; j++)
		{
			if(i == 3)
			{
				if(m_rgdTowerLampData[j][i] == 1)
					m_lblRowData[j][i].SetCaption(_T("Buzzer1"));
				else if(m_rgdTowerLampData[j][i] == 2)
					m_lblRowData[j][i].SetCaption(_T("Buzzer2"));
				else if(m_rgdTowerLampData[j][i] == 3)
					m_lblRowData[j][i].SetCaption(_T("Buzzer3"));
				else if(m_rgdTowerLampData[j][i] == 4)
					m_lblRowData[j][i].SetCaption(_T("Buzzer4"));
				else
					m_lblRowData[j][i].SetCaption(_T("Off"));

			}
			else
			{
				if(m_rgdTowerLampData[j][i] == 0)
					m_lblRowData[j][i].SetCaption(_T("Off"));
				else if(m_rgdTowerLampData[j][i] == 1)
					m_lblRowData[j][i].SetCaption(_T("On"));
				else 
					m_lblRowData[j][i].SetCaption(_T("Blink"));
			}
		}
	}
	CString StrTemp;
	StrTemp.Format(_T("%d"), m_iDelayTime);
	m_lblDelayTime.SetCaption(StrTemp);
}
void CDataTowerLampDlg::SelectLamp(int iColNo)
{
	if(iColNo == 3)
	{
		m_iSelectState = 0;
		SystemBtnDlg dlg(_T("Buzzer State"), "Off", "Buzzer1", "Buzzer2","Buzzer3","Buzzer4");
		m_iSelectState = dlg.DoModal();
		m_rgdTowerLampData[m_iSelectedRow][iColNo] = m_iSelectState;
	}
	else{
		m_iSelectState = 0;
		CSystemThreeBtnDlg dlg(_T("Lamp State"), "Off", "On", "Blink");
		m_iSelectState = dlg.DoModal();
		m_rgdTowerLampData[m_iSelectedRow][iColNo] = m_iSelectState;
	}
	UpdateDisplay();
}
void CDataTowerLampDlg::OnClickDelayTime()
{
	CString Strtemp;
	m_lblDelayTime.GetWindowText(m_rgOldValue, 30);
	
	if(!GetNumberBox(m_rgNewValue, 10, m_rgOldValue))	return;
	
	if(_wtoi(m_rgNewValue) < 30)
	{
		MyMessageBox("The congestion time can be set to 30 seconds or less.\n정체시간은 30초 이하로 설정할 수 있습니다.", "Confirm", M_OK);
		return;
	}
	
	m_iDelayTime = _wtoi(m_rgNewValue);
	Strtemp.Format(_T("%s"), m_rgNewValue);
	m_lblDelayTime.SetCaption(Strtemp);
}
