// DataPanelAlignerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "DataPanelAlignerDlg.h"
#include "MPanelAligner.h"
#include "MCameraCarrier.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataPanelAlignerDlg dialog

CDataPanelAlignerDlg::CDataPanelAlignerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataPanelAlignerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataPanelAlignerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}

void CDataPanelAlignerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataPanelAlignerDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDS_SYSTEM0, m_lblPanelAlignParameter[0]);
	DDX_Control(pDX, IDS_SYSTEM1, m_lblPanelAlignParameter[1]);
	DDX_Control(pDX, IDS_SYSTEM2, m_lblPanelAlignParameter[2]);
	DDX_Control(pDX, IDS_SYSTEM3, m_lblPanelAlignParameter[3]);
	DDX_Control(pDX, IDS_SYSTEM4, m_lblPanelAlignParameter[4]);
	DDX_Control(pDX, IDS_SYSTEM5, m_lblPanelAlignParameter[5]);
	DDX_Control(pDX, IDS_SYSTEM6, m_lblPanelAlignParameter[6]);
	DDX_Control(pDX, IDS_SYSTEM7, m_lblPanelAlignParameter[7]);
	DDX_Control(pDX, IDS_SYSTEM8, m_lblPanelAlignParameter[8]);
	DDX_Control(pDX, IDS_SYSTEM9, m_lblPanelAlignParameter[9]);
}


BEGIN_MESSAGE_MAP(CDataPanelAlignerDlg, CDialog)
	//{{AFX_MSG_MAP(CDataPanelAlignerDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataPanelAlignerDlg message handlers

BOOL CDataPanelAlignerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dData[0] = m_plnkSystemData->m_iDistance4DifVel;
	m_dData[1] = m_plnkSystemData->m_dActiveAlignerPosX;
	m_dData[2] = m_plnkSystemData->m_dActiveAlignerPosY;
	m_dData[3] = m_plnkSystemData->m_dAutoSearchPanelMarkDistance;
	m_dData[4] = m_plnkSystemData->m_dPanelAlignTolerance;
	m_dData[5] = m_plnkSystemData->m_iPanelAlignRetryLimit;
	m_dData[6] = m_plnkSystemData->m_dBacklash;
	m_dData[7] = m_plnkSystemData->m_dAirPlateBlowOnDelay;
	m_dData[8] = m_plnkSystemData->m_dDist_AlignerToRotator_X;
	m_dData[9] = m_plnkSystemData->m_dDist_AlignerToRotator_Y;
	updateDisplay();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_1"), &szTextTemp);
//	저속 이동 기준 거리
	GetDlgItem(IDB_SYSTEM0)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_2"), &szTextTemp);
//	기준 Aligner X
	GetDlgItem(IDB_SYSTEM1)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_3"), &szTextTemp);
//	기준 Aligner Y
	GetDlgItem(IDB_SYSTEM2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_4"), &szTextTemp);
//	Panel Auto Search\r\n시 이동 거리
	GetDlgItem(IDB_SYSTEM3)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_5"), &szTextTemp);
//	부상 Plate\r\nBlow On Delay (sec)
	GetDlgItem(IDB_SYSTEM7)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_6"), &szTextTemp);
//	저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_7"), &szTextTemp);
//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_8"), &szTextTemp);
//	Align중심과 회전 실린더중심간 거리X
	GetDlgItem(IDB_SYSTEM8)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelAlignerDlg_9"), &szTextTemp);
//	Align중심과 회전 실린더중심간 거리Y
	GetDlgItem(IDB_SYSTEM9)->SetWindowText(szTextTemp);
	
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	//_______________

	GetDlgItem(IDB_SYSTEM1)->EnableWindow(FALSE);
	m_lblPanelAlignParameter[1].EnableWindow(FALSE);
	GetDlgItem(IDB_SYSTEM2)->EnableWindow(FALSE);
	m_lblPanelAlignParameter[2].EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataPanelAlignerDlg::updateDisplay()
{
	WCHAR szValue[DEF_PANEL_ALIGNER_MAX_LABEL][100];

	for (int i = 0; i < DEF_PANEL_ALIGNER_MAX_LABEL; i++)
	{
		if (0 == i || 5 == i)
			swprintf(szValue[i], _T("%d"), static_cast<int>(m_dData[i]));
		else if (7 == i)
			swprintf(szValue[i], _T("%.1f"), m_dData[i]);
		else
			swprintf(szValue[i], _T("%.4f"), m_dData[i]);

		m_lblPanelAlignParameter[i].SetCaption(szValue[i]);
	}
}

BEGIN_EVENTSINK_MAP(CDataPanelAlignerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataPanelAlignerDlg)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM0, -600 /* Click */, OnPanelAlignData0, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM1, -600 /* Click */, OnPanelAlignData1, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM2, -600 /* Click */, OnPanelAlignData2, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM3, -600 /* Click */, OnPanelAlignData3, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM4, -600 /* Click */, OnPanelAlignData4, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM5, -600 /* Click */, OnPanelAlignData5, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM6, -600 /* Click */, OnPanelAlignData6, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM7, -600 /* Click */, OnPanelAlignData7, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM8, -600 /* Click */, OnClickSystem8, VTS_NONE)
	ON_EVENT(CDataPanelAlignerDlg, IDB_SYSTEM9, -600 /* Click */, OnClickSystem9, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataPanelAlignerDlg::OnClickSave() 
{
//	변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40800, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkSystemData->m_iDistance4DifVel				= static_cast<int>(m_dData[0]);
	m_plnkSystemData->m_dActiveAlignerPosX				= m_dData[1];
	m_plnkSystemData->m_dActiveAlignerPosY				= m_dData[2];
	m_plnkSystemData->m_dAutoSearchPanelMarkDistance	= m_dData[3];
	m_plnkSystemData->m_dPanelAlignTolerance			= m_dData[4];
	m_plnkSystemData->m_iPanelAlignRetryLimit			= static_cast<int>(m_dData[5]);
	m_plnkSystemData->m_dBacklash						= m_dData[6];
	m_plnkSystemData->m_dAirPlateBlowOnDelay			= m_dData[7];
	m_plnkSystemData->m_dDist_AlignerToRotator_X		= m_dData[8];
	m_plnkSystemData->m_dDist_AlignerToRotator_Y		= m_dData[9];

	m_plnkSystemData->WriteData();

	// Model Change
	MOlbSystemPre.GetPanelAlignerComponent()->ChangeModel();

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		MOlbSystemPre.GetCameraCarrierComponent(i)->ChangeModel();
#ifdef DEF_GATE_SYSTEM
		MOlbSystemPre.GetRCameraCarrierComponent(i)->ChangeModel();
#endif
	}
}

void CDataPanelAlignerDlg::OnClickExit() 
{
	OnOK();
}

void CDataPanelAlignerDlg::getNewValue(int iIndex)
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________

	m_lblPanelAlignParameter[iIndex].GetWindowText(m_szOldValue, 10);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))
		return;

	// 자리수 조정
	m_dData[iIndex] = wcstod(m_szNewValue, NULL);

	if (m_dData[iIndex] < 0.0)
		m_dData[iIndex] = 0.0;
	
	if (0 == iIndex || 5 == iIndex)
		swprintf(m_szNewValue, _T("%d"), static_cast<int>(m_dData[iIndex]));
	else if (7 == iIndex)
		swprintf(m_szNewValue, _T("%.1f"), m_dData[iIndex]);
	else
		swprintf(m_szNewValue, _T("%.4f"), m_dData[iIndex]);

	m_dData[iIndex] = wcstod(m_szNewValue, NULL);
	
	m_lblPanelAlignParameter[iIndex].SetCaption(m_szNewValue);	
}

void CDataPanelAlignerDlg::OnPanelAlignData0() 
{
	getNewValue(0);
}

void CDataPanelAlignerDlg::OnPanelAlignData1() 
{
	getNewValue(1);
}

void CDataPanelAlignerDlg::OnPanelAlignData2() 
{
	getNewValue(2);
}

void CDataPanelAlignerDlg::OnPanelAlignData3() 
{
	getNewValue(3);
}

void CDataPanelAlignerDlg::OnPanelAlignData4()
{
	getNewValue(4);
}

void CDataPanelAlignerDlg::OnPanelAlignData5()
{
	getNewValue(5);
}

void CDataPanelAlignerDlg::OnPanelAlignData6()
{
	getNewValue(6);
}

void CDataPanelAlignerDlg::OnPanelAlignData7()
{
	getNewValue(7);
}

void CDataPanelAlignerDlg::OnClickSystem8() 
{
	// TODO: Add your control notification handler code here
	getNewValue(8);
}

void CDataPanelAlignerDlg::OnClickSystem9() 
{
	// TODO: Add your control notification handler code here
	getNewValue(9);	
}

void CDataPanelAlignerDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}
