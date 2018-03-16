// AutoZigInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "AutoZigInfoDlg.h"
#include "MPlatformOlbSystem.h"
#include "CommonGetValueDlg.h"
#include "MRFID.h"
#include "MLCNet.h"
#include "MTabFeeder.h"
#include "MCtrlTabFeeder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoZigInfoDlg dialog
extern	MPlatformOlbSystem	MOlbSystemPre;


CAutoZigInfoDlg::CAutoZigInfoDlg(int iFeederSel, CWnd* pParent /*=NULL*/)
	: CDialog(CAutoZigInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoZigInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_iFeederSel = iFeederSel;
	m_pRFID = MOlbSystemPre.GetRFIDComponent(m_iFeederSel);
	m_plnkCtrlTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(m_iFeederSel);
	m_plnkTabFeeder = MOlbSystemPre.GetTabFeederComponent(m_iFeederSel);
	m_plnkLCNet			= MOlbSystemPre.GetLCNetComponent();
	m_plnkSystemData	= MOlbSystemPre.GetSystemData();
}


void CAutoZigInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoZigInfoDlg)
	//DDX_Control(pDX, IDC_MSFLEXGRID_TAB_IC_INFORM, m_FlexGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoZigInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoZigInfoDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoZigInfoDlg message handlers

BOOL CAutoZigInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strTemp;
//	if (m_iFeederSel == DEF_TABFEEDER1)
//		m_FlexGrid.SetTextMatrix(0, 0, _T("Feeder [1]"));
//	else
//		m_FlexGrid.SetTextMatrix(0, 0, _T("Feeder [2]"));
//
//	m_FlexGrid.SetRow(0);
	CString szTextTemp;
//	
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_1"), &szTextTemp);
////구분
//	m_FlexGrid.SetCol(0); m_FlexGrid.SetText(szTextTemp);	m_FlexGrid.SetCellFontBold(TRUE);
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_2"), &szTextTemp);
////값
//	m_FlexGrid.SetCol(1); m_FlexGrid.SetText(szTextTemp);	m_FlexGrid.SetCellFontBold(TRUE);
//
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_3"), &szTextTemp);	
////금형 ID
//	m_FlexGrid.SetTextMatrix(1, 0, szTextTemp);
//
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_4"), &szTextTemp);	
////Regrinding 횟수
//	m_FlexGrid.SetTextMatrix(2, 0, szTextTemp);
//
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_5"), &szTextTemp);	
////Regrinding 날짜
//	m_FlexGrid.SetTextMatrix(3, 0, szTextTemp);
//	
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_6"), &szTextTemp);	
////MATCODE
//	m_FlexGrid.SetTextMatrix(4, 0, szTextTemp);
//	
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_7"), &szTextTemp);	
////총 제한 횟수
//	m_FlexGrid.SetTextMatrix(5, 0, szTextTemp);
//	
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_8"), &szTextTemp);	
////총 사용 횟수
//	m_FlexGrid.SetTextMatrix(6, 0, szTextTemp);
//	
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_9"), &szTextTemp);	
////Regrinding 후 사용 제한 횟수
//	m_FlexGrid.SetTextMatrix(7, 0, szTextTemp);
//	
//	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_10"), &szTextTemp);	
////Regrinding 후 사용 횟수
//	m_FlexGrid.SetTextMatrix(8, 0, szTextTemp);
//		
//	m_FlexGrid.SetTextMatrix(9, 0, _T("STATE"));
//	
//	m_FlexGrid.SetTextMatrix(10, 0, _T("STEP"));
//	
//
//	m_FlexGrid.SetTextMatrix(1, 1, m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_BATCHID); // ZIG ID
//
//	m_FlexGrid.SetTextMatrix(2, 1, _T(" "));
//	
//	m_FlexGrid.SetTextMatrix(3, 1, _T(" "));
//
//	m_FlexGrid.SetTextMatrix(4, 1, m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_CODE);
//
//	m_FlexGrid.SetTextMatrix(5, 1, m_plnkCtrlTabFeeder->m_matZigInfo.m_sPORT_STATE);
//
//	m_FlexGrid.SetTextMatrix(6, 1, m_plnkCtrlTabFeeder->m_matZigInfo.m_sPORT_TYPE);
//
//	strTemp.Format(_T("%d"), m_plnkCtrlTabFeeder->m_matZigInfo.m_nT_QTY);
//	m_FlexGrid.SetTextMatrix(7, 1, strTemp);
//
//	strTemp.Format(_T("%d"), m_plnkCtrlTabFeeder->m_matZigInfo.m_nU_QTY);
//	m_FlexGrid.SetTextMatrix(8, 1, strTemp);
//
//	strTemp.Format(_T("%s"), m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_STEP);
//	m_FlexGrid.SetTextMatrix(10, 1, strTemp);  //Step 정보 Display
//
	m_uiTimerID = SetTimer(1, 500, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
//나가기
	ReadDisplayMessage(MAIN_AUTO, _T("AutoZigInfoDlg_11"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);											

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CAutoZigInfoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoZigInfoDlg)
	ON_EVENT(CAutoZigInfoDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CAutoZigInfoDlg, IDB_CHECK_DOCK_IN, -600 /* Click */, OnClickCheckDockIn, VTS_NONE)
	ON_EVENT(CAutoZigInfoDlg, IDB_DATA_READ, -600 /* Click */, OnClickDataRead, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAutoZigInfoDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID) KillTimer(m_uiTimerID);

	CDialog::OnCancel();
}

void CAutoZigInfoDlg::DisplayRFIDInformation()
{
	CString strTemp;

	//strTemp = m_pRFID->ReadData(ZIGID);
	//m_FlexGrid.SetTextMatrix(1, 1, strTemp); // ZIG ID

	//strTemp = m_pRFID->ReadData(REG_QTY);
	//m_FlexGrid.SetTextMatrix(2, 1, strTemp);
	//
	//strTemp = m_pRFID->ReadData(REG_DATE);
	//m_FlexGrid.SetTextMatrix(3, 1, strTemp);

	//strTemp = m_pRFID->ReadData(MATCODE);
	//m_FlexGrid.SetTextMatrix(4, 1, strTemp);

	//strTemp = m_pRFID->ReadData(TOT_QTY_ZIG);
	//m_FlexGrid.SetTextMatrix(5, 1, strTemp);

	//strTemp = m_pRFID->ReadData(ZIG_USED_QTY);
	//m_FlexGrid.SetTextMatrix(6, 1, strTemp);

	//strTemp = m_pRFID->ReadData(TOT_QTY_AFTER_REG);
	//m_FlexGrid.SetTextMatrix(7, 1, strTemp);

	//strTemp = m_pRFID->ReadData(REG_USED_QTY);
	//m_FlexGrid.SetTextMatrix(8, 1, strTemp);

	//strTemp.Format(_T("%s"), m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_STEP);
	//m_FlexGrid.SetTextMatrix(10, 1, strTemp);  //Step 정보 Display
}

void CAutoZigInfoDlg::OnClickCheckDockIn() 
{
	if (m_plnkLCNet->m_bConnected)
	{
		if (m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE == eMatStateEnable
			|| m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE == eMatStateRun)
		{
//		금형 자재 상태가 Run 혹은 Enable입니다. 먼저 기존 자재를 제거해 주세요.
			MyMessageBox(MY_MESSAGE_BOX, 20600, _T("Confirm"), M_ICONINFORMATION);

			return;
		}
	}

//	ZIG 정보를 적용하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 20601, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	MMaterial Material;
	
	m_plnkCtrlTabFeeder->SetMoldCount(0);

	if (!m_plnkTabFeeder->IsInPressPos(DEF_PRESS_UP_POS) && !m_plnkTabFeeder->IsInPressPos(DEF_PRESS_READY_POS))
	{
//	금형 Press가 Down 위치이므로 읽을 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20602, _T("Error"), M_ICONERROR);
		return;
	}

	int iResult = m_plnkCtrlTabFeeder->ReadZigInfo();
	if (iResult)
	{
//	금형 RFID 정보 읽을 수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20603, _T("Error"), M_ICONERROR);

//	금형정보를 직접 입력하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX, 20604, _T("Question"), M_ICONQUESTION|M_YESNO) == IDYES)
		{
			CCommonGetValueDlg	valueDialog(20, _T(""), _T("Mold ID"));
			CString str;

//		금형 ID를 입력하세요.
			MyMessageBox(MY_MESSAGE_BOX, 20605, _T("Information"), M_ICONINFORMATION);
			valueDialog.DoModal();
			str = valueDialog.GetStringValue();
			m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_BATCHID = str;

			m_plnkCtrlTabFeeder->m_matZigInfo.m_nT_QTY = 500000;
		}
		else
			return;
	}

	//110705.kms Modify____________
	if (m_iFeederSel == DEF_TABFEEDER1)
		m_plnkCtrlTabFeeder->m_matZigInfo.m_sPORTID = m_plnkSystemData->m_strModulePortID[4];
	else
		m_plnkCtrlTabFeeder->m_matZigInfo.m_sPORTID = m_plnkSystemData->m_strModulePortID[5];
	/*if (m_iFeederSel == DEF_TABFEEDER1)
		m_plnkCtrlTabFeeder->m_matZigInfo.m_sPORTID = _T("FEEDER1");
	else
		m_plnkCtrlTabFeeder->m_matZigInfo.m_sPORTID = _T("FEEDER2");
		*/
	//______________________________
	m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE = eMatStateIdle;
	m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_KIND = 12;
	if (m_plnkSystemData->m_eSystemType == SYSTEM_TYPE_SOURCE)
		m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_STEP = "SOURCE";
	else // if (m_plnkSystemData->m_eSystemType == SYSTEM_TYPE_GATE)
		m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_STEP = "GATE";
	/* 삭제 예정.
	else
	{
		if (m_iFeederSel == DEF_TABFEEDER1)
			m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_STEP = "SOURCE";
		else
			m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_STEP = "GATE";
	}
	*/
	Material = m_plnkCtrlTabFeeder->m_matZigInfo;

	m_plnkLCNet->MaterialReport(eMatDockIn, &Material);
	m_plnkCtrlTabFeeder->SetZigConsumStart(FALSE);
	m_plnkCtrlTabFeeder->SetZigWarningState(FALSE);
}

void CAutoZigInfoDlg::OnClickDataRead() 
{
	// TODO: Add your control notification handler code here
	DisplayRFIDInformation();
}

void CAutoZigInfoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString strTemp;
	// Material State  -----------------------------------------------------
	switch (m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE)
	{
	case eMatStateIdle:		strTemp.Format(_T("IDLE"));		break;
	case eMatStateEnable:	strTemp.Format(_T("ENABLE"));	break;
	case eMatStateRun:		strTemp.Format(_T("RUN"));		break;
	case eMatStateHold:		strTemp.Format(_T("HOLD"));		break;
	case eMatStateDisable:	strTemp.Format(_T("DISABLE"));	break;
	case eMatStateRemove :	strTemp.Format(_T("REMOVE"));	break;
	default:				strTemp.Format(_T("No Data"));	break;
	}
	//m_FlexGrid.SetTextMatrix(9, 1, strTemp);

	CDialog::OnTimer(nIDEvent);
}

void CAutoZigInfoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}
