// DataIndexOffset.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataToolOffsetDlg.h"
#include "MPlatformOlbSystem.h"
#include "DlgTabOffsetTable.h"//20111205 SJ_HJG

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataToolOffsetDlg dialog

CDataToolOffsetDlg::CDataToolOffsetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataToolOffsetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataToolOffsetDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CDataToolOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataToolOffsetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	// Label
	DDX_Control(pDX, IDS_USE_1X, m_lblOffsetMounter[0]);
	DDX_Control(pDX, IDS_USE_2X, m_lblOffsetMounter[1]);
	DDX_Control(pDX, IDS_USE_3X, m_lblOffsetMounter[2]);
	DDX_Control(pDX, IDS_USE_4X, m_lblOffsetMounter[3]);
	DDX_Control(pDX, IDS_USE_5X, m_lblOffsetMounter[4]);
	DDX_Control(pDX, IDS_USE_6X, m_lblOffsetMounter[5]);
	DDX_Control(pDX, IDS_USE_7X, m_lblOffsetMounter[6]);
	DDX_Control(pDX, IDS_USE_8X, m_lblOffsetMounter[7]);
	DDX_Control(pDX, IDS_USE_1X2, m_lblOffsetMounter[8]);
	DDX_Control(pDX, IDS_USE_2X2, m_lblOffsetMounter[9]);
	DDX_Control(pDX, IDS_USE_3X2, m_lblOffsetMounter[10]);
	DDX_Control(pDX, IDS_USE_4X2, m_lblOffsetMounter[11]);
	DDX_Control(pDX, IDS_USE_5X2, m_lblOffsetMounter[12]);
	DDX_Control(pDX, IDS_USE_6X2, m_lblOffsetMounter[13]);

	DDX_Control(pDX, IDS_USE_1Y, m_lblOffsetTabAligner[0]);
	DDX_Control(pDX, IDS_USE_2Y, m_lblOffsetTabAligner[1]);
	DDX_Control(pDX, IDS_USE_3Y, m_lblOffsetTabAligner[2]);
	DDX_Control(pDX, IDS_USE_4Y, m_lblOffsetTabAligner[3]);
	DDX_Control(pDX, IDS_USE_5Y, m_lblOffsetTabAligner[4]);
	DDX_Control(pDX, IDS_USE_6Y, m_lblOffsetTabAligner[5]);
	DDX_Control(pDX, IDS_USE_7Y, m_lblOffsetTabAligner[6]);
	DDX_Control(pDX, IDS_USE_8Y, m_lblOffsetTabAligner[7]);
	DDX_Control(pDX, IDS_USE_1Y2, m_lblOffsetTabAligner[8]);
	DDX_Control(pDX, IDS_USE_2Y2, m_lblOffsetTabAligner[9]);
	DDX_Control(pDX, IDS_USE_3Y2, m_lblOffsetTabAligner[10]);
	DDX_Control(pDX, IDS_USE_4Y2, m_lblOffsetTabAligner[11]);
	DDX_Control(pDX, IDS_USE_5Y2, m_lblOffsetTabAligner[12]);
	DDX_Control(pDX, IDS_USE_6Y2, m_lblOffsetTabAligner[13]);

	// Button
	DDX_Control(pDX, IDB_USE_1X, m_btnOffsetMounter[0]);
	DDX_Control(pDX, IDB_USE_2X, m_btnOffsetMounter[1]);
	DDX_Control(pDX, IDB_USE_3X, m_btnOffsetMounter[2]);
	DDX_Control(pDX, IDB_USE_4X, m_btnOffsetMounter[3]);
	DDX_Control(pDX, IDB_USE_5X, m_btnOffsetMounter[4]);
	DDX_Control(pDX, IDB_USE_6X, m_btnOffsetMounter[5]);
	DDX_Control(pDX, IDB_USE_7X, m_btnOffsetMounter[6]);
	DDX_Control(pDX, IDB_USE_8X, m_btnOffsetMounter[7]);
	DDX_Control(pDX, IDB_USE_1X2, m_btnOffsetMounter[8]);
	DDX_Control(pDX, IDB_USE_2X2, m_btnOffsetMounter[9]);
	DDX_Control(pDX, IDB_USE_3X2, m_btnOffsetMounter[10]);
	DDX_Control(pDX, IDB_USE_4X2, m_btnOffsetMounter[11]);
	DDX_Control(pDX, IDB_USE_5X2, m_btnOffsetMounter[12]);
	DDX_Control(pDX, IDB_USE_6X2, m_btnOffsetMounter[13]);

	DDX_Control(pDX, IDB_USE_1Y, m_btnOffsetTabAligner[0]);
	DDX_Control(pDX, IDB_USE_2Y, m_btnOffsetTabAligner[1]);
	DDX_Control(pDX, IDB_USE_3Y, m_btnOffsetTabAligner[2]);
	DDX_Control(pDX, IDB_USE_4Y, m_btnOffsetTabAligner[3]);
	DDX_Control(pDX, IDB_USE_5Y, m_btnOffsetTabAligner[4]);
	DDX_Control(pDX, IDB_USE_6Y, m_btnOffsetTabAligner[5]);
	DDX_Control(pDX, IDB_USE_7Y, m_btnOffsetTabAligner[6]);
	DDX_Control(pDX, IDB_USE_8Y, m_btnOffsetTabAligner[7]);
	DDX_Control(pDX, IDB_USE_1Y2, m_btnOffsetTabAligner[8]);
	DDX_Control(pDX, IDB_USE_2Y2, m_btnOffsetTabAligner[9]);
	DDX_Control(pDX, IDB_USE_3Y2, m_btnOffsetTabAligner[10]);
	DDX_Control(pDX, IDB_USE_4Y2, m_btnOffsetTabAligner[11]);
	DDX_Control(pDX, IDB_USE_5Y2, m_btnOffsetTabAligner[12]);
	DDX_Control(pDX, IDB_USE_6Y2, m_btnOffsetTabAligner[13]);
}


BEGIN_MESSAGE_MAP(CDataToolOffsetDlg, CDialog)
	//{{AFX_MSG_MAP(CDataToolOffsetDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataToolOffsetDlg message handlers

BEGIN_EVENTSINK_MAP(CDataToolOffsetDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataToolOffsetDlg)
	ON_EVENT(CDataToolOffsetDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_BATCHX_INPUT, -600 /* Click */, OnBatchxInput, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_BATCHY_INPUT, -600 /* Click */, OnBatchyInput, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_1X, -600 /* Click */, OnClickUse1x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_2X, -600 /* Click */, OnClickUse2x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_3X, -600 /* Click */, OnClickUse3x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_4X, -600 /* Click */, OnClickUse4x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_5X, -600 /* Click */, OnClickUse5x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_6X, -600 /* Click */, OnClickUse6x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_7X, -600 /* Click */, OnClickUse7x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_8X, -600 /* Click */, OnClickUse8x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_1X2, -600 /* Click */, OnClickUse9x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_2X2, -600 /* Click */, OnClickUse10x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_3X2, -600 /* Click */, OnClickUse11x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_4X2, -600 /* Click */, OnClickUse12x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_5X2, -600 /* Click */, OnClickUse13x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_6X2, -600 /* Click */, OnClickUse14x, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_1Y, -600 /* Click */, OnClickUse1y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_2Y, -600 /* Click */, OnClickUse2y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_3Y, -600 /* Click */, OnClickUse3y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_4Y, -600 /* Click */, OnClickUse4y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_5Y, -600 /* Click */, OnClickUse5y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_6Y, -600 /* Click */, OnClickUse6y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_7Y, -600 /* Click */, OnClickUse7y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_8Y, -600 /* Click */, OnClickUse8y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_1Y2, -600 /* Click */, OnClickUse9y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_2Y2, -600 /* Click */, OnClickUse10y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_3Y2, -600 /* Click */, OnClickUse11y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_4Y2, -600 /* Click */, OnClickUse12y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_5Y2, -600 /* Click */, OnClickUse13y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_USE_6Y2, -600 /* Click */, OnClickUse14y, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_CANCEL, -600 /* Click */, OnClickCancel, VTS_NONE)
	ON_EVENT(CDataToolOffsetDlg, IDB_TABOFFSET_TABLE, -600 /* Click */, OnClickTaboffsetTable, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CDataToolOffsetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dOffsetMounter[0]			= m_plnkSystemData->m_rgdToolOffsetMounter[0];
	m_dOffsetMounter[1]			= m_plnkSystemData->m_rgdToolOffsetMounter[1];
	m_dOffsetMounter[2]			= m_plnkSystemData->m_rgdToolOffsetMounter[2];
	m_dOffsetMounter[3]			= m_plnkSystemData->m_rgdToolOffsetMounter[3];
	m_dOffsetMounter[4]			= m_plnkSystemData->m_rgdToolOffsetMounter[4];
	m_dOffsetMounter[5]			= m_plnkSystemData->m_rgdToolOffsetMounter[5];
	
	m_dOffsetTabAligner[0]		= m_plnkSystemData->m_rgdToolOffsetTabAligner[0];
	m_dOffsetTabAligner[1]		= m_plnkSystemData->m_rgdToolOffsetTabAligner[1];
	m_dOffsetTabAligner[2]		= m_plnkSystemData->m_rgdToolOffsetTabAligner[2];
	m_dOffsetTabAligner[3]		= m_plnkSystemData->m_rgdToolOffsetTabAligner[3];
	m_dOffsetTabAligner[4]		= m_plnkSystemData->m_rgdToolOffsetTabAligner[4];
	m_dOffsetTabAligner[5]		= m_plnkSystemData->m_rgdToolOffsetTabAligner[5];
	
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		m_dOffsetMounter[6]		= m_plnkSystemData->m_rgdToolOffsetMounter[6];
		m_dOffsetMounter[7]		= m_plnkSystemData->m_rgdToolOffsetMounter[7];
		
		m_dOffsetTabAligner[6]	= m_plnkSystemData->m_rgdToolOffsetTabAligner[6];
		m_dOffsetTabAligner[7]	= m_plnkSystemData->m_rgdToolOffsetTabAligner[7];
		
		GetDlgItem(IDC_TITLE1)->ShowWindow(SW_HIDE);
		for (int i = 8; i < DEF_MAX_DISPLAY_ITEM; i++)
		{
			m_btnOffsetMounter[i].EnableWindow(FALSE);
			m_btnOffsetMounter[i].ShowWindow(SW_HIDE);

			m_lblOffsetMounter[i].EnableWindow(FALSE);
			m_lblOffsetMounter[i].ShowWindow(SW_HIDE);

			m_btnOffsetTabAligner[i].EnableWindow(FALSE);
			m_btnOffsetTabAligner[i].ShowWindow(SW_HIDE);

			m_lblOffsetTabAligner[i].EnableWindow(FALSE);
			m_lblOffsetTabAligner[i].ShowWindow(SW_HIDE);
		}
	}
	else // if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		m_dOffsetMounter[6]		= 0.0;	
		m_dOffsetMounter[7]		= 0.0;	
		m_dOffsetMounter[8]		= m_plnkSystemData->m_rgdToolOffsetMounter[6];	
		m_dOffsetMounter[9]		= m_plnkSystemData->m_rgdToolOffsetMounter[7];	
		m_dOffsetMounter[10]	= m_plnkSystemData->m_rgdToolOffsetMounter[8];
		m_dOffsetMounter[11]	= m_plnkSystemData->m_rgdToolOffsetMounter[9];
		m_dOffsetMounter[12]	= m_plnkSystemData->m_rgdToolOffsetMounter[10];
		m_dOffsetMounter[13]	= m_plnkSystemData->m_rgdToolOffsetMounter[11];
		
		m_dOffsetTabAligner[6]	= 0.0;	
		m_dOffsetTabAligner[7]	= 0.0;	
		m_dOffsetTabAligner[8]	= m_plnkSystemData->m_rgdToolOffsetTabAligner[6];	
		m_dOffsetTabAligner[9]	= m_plnkSystemData->m_rgdToolOffsetTabAligner[7];	
		m_dOffsetTabAligner[10]	= m_plnkSystemData->m_rgdToolOffsetTabAligner[8];	
		m_dOffsetTabAligner[11]	= m_plnkSystemData->m_rgdToolOffsetTabAligner[9];	
		m_dOffsetTabAligner[12]	= m_plnkSystemData->m_rgdToolOffsetTabAligner[10];	
		m_dOffsetTabAligner[13]	= m_plnkSystemData->m_rgdToolOffsetTabAligner[11];
		
		for (int i = 6; i < 8; i++)
		{
			m_btnOffsetMounter[i].EnableWindow(FALSE);
			m_btnOffsetMounter[i].ShowWindow(SW_HIDE);

			m_lblOffsetMounter[i].EnableWindow(FALSE);
			m_lblOffsetMounter[i].ShowWindow(SW_HIDE);

			m_btnOffsetTabAligner[i].EnableWindow(FALSE);
			m_btnOffsetTabAligner[i].ShowWindow(SW_HIDE);

			m_lblOffsetTabAligner[i].EnableWindow(FALSE);
			m_lblOffsetTabAligner[i].ShowWindow(SW_HIDE);
		}
	}

	UpdateDisplay();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataToolOffsetDlg_1"), &szTextTemp);
//	Tool Offset Data 설정
	SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataToolOffsetDlg_2"), &szTextTemp);
//	X\r\n일괄입력
	GetDlgItem(IDB_BATCHX_INPUT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataToolOffsetDlg_3"), &szTextTemp);
//	Y\r\n일괄입력
	GetDlgItem(IDB_BATCHY_INPUT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataToolOffsetDlg_4"), &szTextTemp);
//	저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataToolOffsetDlg_5"), &szTextTemp);
//	나가기
	GetDlgItem(IDB_CANCEL)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataToolOffsetDlg::ChangeVal(int iType, int iSel)
{
	WCHAR szOldValue[50];
	WCHAR szNewValue[50];
	
	CBtnEnh* psLabel;
	double* pdValue;
	if (iType == DEF_TYPE_TABMOUNTER)
	{
		pdValue = m_dOffsetMounter;
		psLabel = m_lblOffsetMounter;
	}
	else
	{
		pdValue = m_dOffsetTabAligner;
		psLabel = m_lblOffsetTabAligner;
	}

	psLabel[iSel].GetWindowText(szOldValue, 10);
	
	if (!GetNumberBox(szNewValue, 10, szOldValue))
		return;

	pdValue[iSel] = wcstod(szNewValue, NULL);
	
	swprintf(szNewValue, _T("%.3f"), pdValue[iSel]);
	psLabel[iSel].SetCaption(szNewValue);
}

void CDataToolOffsetDlg::UpdateDisplay()
{
	WCHAR szNewValue[50];
	
	for (int i = 0; i < DEF_MAX_DISPLAY_ITEM; i++)
	{
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType && i >= DEF_MAX_TABMOUNTER)
			break;
		else if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType && (i == 6 || i == 7))
			continue;

		// TabMounter Tool Offset (TabMounter 축 방향)
		if (fabs(m_dOffsetMounter[i]) > 5.0)
			m_dOffsetMounter[i] = 0.0;
		
		swprintf(szNewValue, _T("%.3f"), m_dOffsetMounter[i]);
		m_lblOffsetMounter[i].SetCaption(szNewValue);
		
		// TabMounter Tool Offset (TabAligner 축 방향)
		if (fabs(m_dOffsetTabAligner[i]) > 5.0)
			m_dOffsetTabAligner[i] = 0.0;
		
		swprintf(szNewValue, _T("%.3f"), m_dOffsetTabAligner[i]);
		m_lblOffsetTabAligner[i].SetCaption(szNewValue);
	}
}

void CDataToolOffsetDlg::OnSave() 
{
//	변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 41001, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkSystemData->m_rgdToolOffsetMounter[0]			= m_dOffsetMounter[0];
	m_plnkSystemData->m_rgdToolOffsetMounter[1]			= m_dOffsetMounter[1];
	m_plnkSystemData->m_rgdToolOffsetMounter[2]			= m_dOffsetMounter[2];
	m_plnkSystemData->m_rgdToolOffsetMounter[3]			= m_dOffsetMounter[3];
	m_plnkSystemData->m_rgdToolOffsetMounter[4]			= m_dOffsetMounter[4];
	m_plnkSystemData->m_rgdToolOffsetMounter[5]			= m_dOffsetMounter[5];
	
	m_plnkSystemData->m_rgdToolOffsetTabAligner[0]		= m_dOffsetTabAligner[0];
	m_plnkSystemData->m_rgdToolOffsetTabAligner[1]		= m_dOffsetTabAligner[1];
	m_plnkSystemData->m_rgdToolOffsetTabAligner[2]		= m_dOffsetTabAligner[2];
	m_plnkSystemData->m_rgdToolOffsetTabAligner[3]		= m_dOffsetTabAligner[3];
	m_plnkSystemData->m_rgdToolOffsetTabAligner[4]		= m_dOffsetTabAligner[4];
	m_plnkSystemData->m_rgdToolOffsetTabAligner[5]		= m_dOffsetTabAligner[5];
	
	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		m_plnkSystemData->m_rgdToolOffsetMounter[6]		= m_dOffsetMounter[6];
		m_plnkSystemData->m_rgdToolOffsetMounter[7]		= m_dOffsetMounter[7];
		
		m_plnkSystemData->m_rgdToolOffsetTabAligner[6]	= m_dOffsetTabAligner[6];
		m_plnkSystemData->m_rgdToolOffsetTabAligner[7]	= m_dOffsetTabAligner[7];
	}
	else if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		m_plnkSystemData->m_rgdToolOffsetMounter[6]		= m_dOffsetMounter[8];
		m_plnkSystemData->m_rgdToolOffsetMounter[7]		= m_dOffsetMounter[9];
		m_plnkSystemData->m_rgdToolOffsetMounter[8]		= m_dOffsetMounter[10];
		m_plnkSystemData->m_rgdToolOffsetMounter[9]		= m_dOffsetMounter[11];
		m_plnkSystemData->m_rgdToolOffsetMounter[10]	= m_dOffsetMounter[12];
		m_plnkSystemData->m_rgdToolOffsetMounter[11]	= m_dOffsetMounter[13];
		
		m_plnkSystemData->m_rgdToolOffsetTabAligner[6]	= m_dOffsetTabAligner[8];
		m_plnkSystemData->m_rgdToolOffsetTabAligner[7]	= m_dOffsetTabAligner[9];
		m_plnkSystemData->m_rgdToolOffsetTabAligner[8]	= m_dOffsetTabAligner[10];
		m_plnkSystemData->m_rgdToolOffsetTabAligner[9]	= m_dOffsetTabAligner[11];
		m_plnkSystemData->m_rgdToolOffsetTabAligner[10] = m_dOffsetTabAligner[12];
		m_plnkSystemData->m_rgdToolOffsetTabAligner[11] = m_dOffsetTabAligner[13];
	}

	m_plnkSystemData->WriteData();
}

void CDataToolOffsetDlg::OnBatchxInput()
{
	getBatchInput(DEF_TYPE_TABMOUNTER);	
}

void CDataToolOffsetDlg::OnBatchyInput()
{
	getBatchInput(DEF_TYPE_TABALIGNER);
}

void CDataToolOffsetDlg::getBatchInput(int iAxis)
{
	WCHAR szOldValue[50];
	WCHAR szNewValue[50];

	swprintf(szOldValue, _T("%.3f") ,0.0);

	if (!GetNumberBox(szNewValue, 10, szOldValue))
		return;

	// 자리수 조정
	double dNewValue = wcstod(szNewValue, NULL);
	swprintf(szNewValue, _T("%.3f"), dNewValue);

	for (int i = 0; i < DEF_MAX_DISPLAY_ITEM; i++)
	{
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType && i >= DEF_MAX_TABMOUNTER)
			break;
		else if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType && (i == 7 || i == 8))
			continue;
		
		// TabMounter Tool Offset (TabMounter 축 방향)
		if (iAxis == DEF_TYPE_TABMOUNTER)
		{
			m_dOffsetMounter[i] = dNewValue;
			m_lblOffsetMounter[i].SetCaption(szNewValue);
		}
		
		// TabMounter Tool Offset (TabAligner 축 방향)
		if (iAxis == DEF_TYPE_TABALIGNER)
		{
			m_dOffsetTabAligner[i] = dNewValue;
			m_lblOffsetTabAligner[i].SetCaption(szNewValue);
		}
	}
}

void CDataToolOffsetDlg::OnClickUse1x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 0);
}

void CDataToolOffsetDlg::OnClickUse2x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 1);
}

void CDataToolOffsetDlg::OnClickUse3x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 2);	
}

void CDataToolOffsetDlg::OnClickUse4x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 3);	
}

void CDataToolOffsetDlg::OnClickUse5x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 4);
}

void CDataToolOffsetDlg::OnClickUse6x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 5);
}

void CDataToolOffsetDlg::OnClickUse7x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 6);	
}

void CDataToolOffsetDlg::OnClickUse8x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 7);	
}

void CDataToolOffsetDlg::OnClickUse9x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 8);
}

void CDataToolOffsetDlg::OnClickUse10x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 9);
}

void CDataToolOffsetDlg::OnClickUse11x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 10);	
}

void CDataToolOffsetDlg::OnClickUse12x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 11);	
}

void CDataToolOffsetDlg::OnClickUse13x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 12);	
}

void CDataToolOffsetDlg::OnClickUse14x() 
{
	ChangeVal(DEF_TYPE_TABMOUNTER, 13);	
}

void CDataToolOffsetDlg::OnClickUse1y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 0);
}

void CDataToolOffsetDlg::OnClickUse2y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 1);
}

void CDataToolOffsetDlg::OnClickUse3y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 2);	
}

void CDataToolOffsetDlg::OnClickUse4y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 3);	
}

void CDataToolOffsetDlg::OnClickUse5y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 4);
}

void CDataToolOffsetDlg::OnClickUse6y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 5);
}

void CDataToolOffsetDlg::OnClickUse7y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 6);	
}

void CDataToolOffsetDlg::OnClickUse8y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 7);	
}

void CDataToolOffsetDlg::OnClickUse9y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 8);
}

void CDataToolOffsetDlg::OnClickUse10y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 9);
}

void CDataToolOffsetDlg::OnClickUse11y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 10);	
}

void CDataToolOffsetDlg::OnClickUse12y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 11);	
}

void CDataToolOffsetDlg::OnClickUse13y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 12);	
}

void CDataToolOffsetDlg::OnClickUse14y() 
{
	ChangeVal(DEF_TYPE_TABALIGNER, 13);	
}

void CDataToolOffsetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDataToolOffsetDlg::OnClickCancel() 
{
	CDialog::OnCancel();
}

void CDataToolOffsetDlg::OnClickTaboffsetTable() 
{
	// TODO: Add your control notification handler code here

	CDlgTabOffsetTable dlg;
	dlg.DoModal();
	
}
