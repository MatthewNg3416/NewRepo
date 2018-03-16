// MoldZigInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MoldZigInfo.h"
#include "MTickTimer.h"
#include "MPlatformOlbSystem.h"
#include "SystemThreeBtnDlg.h"
#include "CommonGetValueDlg.h"
#include "IComThread.h"
#include "MLCNet.h"
#include "MCtrlTabFeeder.h"
#include "MTrsAutoManager.h"
#include "MRFID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMoldZigInfo dialog
extern	MPlatformOlbSystem	MOlbSystemPre;

CMoldZigInfo::CMoldZigInfo(int iFeederSelect, EMatEvent eMode, CWnd* pParent /*=NULL*/)
	: CDialog(CMoldZigInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMoldZigInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_iFeederSelect = iFeederSelect;
	m_eMode = eMode; 
	m_bCancelInput = FALSE;

	m_plnkSerialBarCode = MOlbSystemPre.GetSerialComponent((DEF_SERIAL_FRONT_BCR_READER + m_iFeederSelect*2));
	m_plnkCtrlTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent(m_iFeederSelect);
	m_plnkLCNet			= MOlbSystemPre.GetLCNetComponent();
	m_plnkSystemData	= MOlbSystemPre.GetSystemData();

	m_pRFID = MOlbSystemPre.GetRFIDComponent(iFeederSelect);
}


void CMoldZigInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoldZigInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDB_EXIT, m_bExit);
	//DDX_Control(pDX, IDC_MSFLEXGRID_TAB_IC_INFORM, m_FlexGrid);
	DDX_Control(pDX, IDB_START_END, m_bStartEnd);
	DDX_Control(pDX, IDB_FEEDER_NUM, m_bFeederNum);
	DDX_Control(pDX, IDB_TABIC_TYPE, m_bTabICType);
	DDX_Control(pDX, IDB_MAKER, m_bMaker);
	DDX_Control(pDX, IDB_TABIC_ID, m_bTabIcID);
	DDX_Control(pDX, IDB_DOCK_IN, m_bDockIn);
	DDX_Control(pDX, IDB_VALIDATION_CHECK, m_bValidationCheck);
	DDX_Control(pDX, IDS_VALIDATION_CHECK, m_sValidationCheck);
	DDX_Control(pDX, IDS_START_END, m_sStartEnd);
	DDX_Control(pDX, IDS_FEEDER_NUM, m_sFeederNum);
	DDX_Control(pDX, IDS_TABIC_TYPE, m_sTabICType);
	DDX_Control(pDX, IDS_MAKER, m_sMaker);
	DDX_Control(pDX, IDS_TABIC_ID, m_sTabIcID);
	DDX_Control(pDX, IDB_CANCEL_DOCK_IN, m_bCancelDockIn);
	DDX_Control(pDX, IDB_CHECK_DOCK_IN, m_bCheckDockIn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoldZigInfo, CDialog)
	//{{AFX_MSG_MAP(CMoldZigInfo)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoldZigInfo message handlers

void CMoldZigInfo::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_uiTimerID) KillTimer(m_uiTimerID);
}

void CMoldZigInfo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	// Do not call CDialog::OnPaint() for painting messages
}

void CMoldZigInfo::OnTimer(UINT nIDEvent) 
{
	// 입력 단계 간소화 작업 
	if (nIDEvent == 100) 
	{
		KillTimer(100);
		OnClickDockIn();
	}
	//______________________

	//// TODO: Add your message handler code here and/or call default
	//m_FlexGrid.SetTextMatrix(1, 1, m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_BATCHID); //TAB IC ID
	//m_FlexGrid.SetTextMatrix(2, 1, m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_STEP); // M_STEP
	//m_FlexGrid.SetTextMatrix(3, 1, m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_MAKER); // MAKER
	//m_FlexGrid.SetTextMatrix(4, 1, m_plnkCtrlTabFeeder->m_matZigInfo.m_sM_CODE); // M_CODE
	//
	//switch (m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE)
	//{
	//case 1:
	//	m_FlexGrid.SetTextMatrix(5, 1, _T("Idle")); 
	//	break;
	//case 2:
	//	m_FlexGrid.SetTextMatrix(5, 1, _T("Enable")); 
	//	break;
	//case 3:
	//	m_FlexGrid.SetTextMatrix(5, 1, _T("Run")); 
	//	break;
	//case 4:
	//	m_FlexGrid.SetTextMatrix(5, 1, _T("Hold")); 
	//	break;
	//case 5:
	//	m_FlexGrid.SetTextMatrix(5, 1, _T("Disable")); 
	//	break;
	//case 6:
	//	m_FlexGrid.SetTextMatrix(5, 1, _T("Remove")); 
	//	break;
	//default:
	//	m_FlexGrid.SetTextMatrix(5, 1, _T("Idle")); 
	//	break;
	//}
	//
	//m_strTemp.Format(_T("%d"), m_plnkCtrlTabFeeder->m_matZigInfo.m_nT_QTY);
	//m_FlexGrid.SetTextMatrix(6, 1, m_strTemp); //T_QTY
	//
	//m_strTemp.Format(_T("%d"), m_plnkCtrlTabFeeder->m_matZigInfo.m_nU_QTY);
	//m_FlexGrid.SetTextMatrix(7, 1, m_strTemp); //펀칭된 갯수 U_QTY
	//
	//m_strTemp.Format(_T("%d"), m_plnkCtrlTabFeeder->m_matZigInfo.m_nR_QTY);
	//m_FlexGrid.SetTextMatrix(8, 1, m_strTemp); //R_QTY 
	//
	//m_strTemp.Format(_T("%d"), m_plnkCtrlTabFeeder->m_matZigInfo.m_nN_QTY);
	//m_FlexGrid.SetTextMatrix(9, 1, m_strTemp); //N_QTY

	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CMoldZigInfo, CDialog)
    //{{AFX_EVENTSINK_MAP(CMoldZigInfo)
	ON_EVENT(CMoldZigInfo, IDB_DOCK_IN, -600 /* Click */, OnClickDockIn, VTS_NONE)
	ON_EVENT(CMoldZigInfo, IDB_CANCEL_DOCK_IN, -600 /* Click */, OnClickCancelDockIn, VTS_NONE)
	ON_EVENT(CMoldZigInfo, IDB_CHECK_DOCK_IN, -600 /* Click */, OnClickCheckDockIn, VTS_NONE)
	ON_EVENT(CMoldZigInfo, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CMoldZigInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_uiTimerID = SetTimer(1, 300, NULL);

	enableBtn();
	
	//m_FlexGrid.SetTextMatrix(1, 0, _T("BatchID")); //TAB IC ID
	//m_FlexGrid.SetTextMatrix(2, 0, _T("Step")); // M_STEP
	//m_FlexGrid.SetTextMatrix(3, 0, _T("Maker")); // MAKER
	//m_FlexGrid.SetTextMatrix(4, 0, _T("Code")); // M_CODE
	//m_FlexGrid.SetTextMatrix(5, 0, _T("State")); 
	//m_FlexGrid.SetTextMatrix(6, 0, _T("Total")); //T_QTY
	//m_FlexGrid.SetTextMatrix(7, 0, _T("Used")); //펀칭된 갯수 U_QTY
	//m_FlexGrid.SetTextMatrix(8, 0, _T("Remain")); //R_QTY 
	//m_FlexGrid.SetTextMatrix(9, 0, _T("NG")); //N_QTY

	// 입력 단계 간소화 작업 
	SetTimer(100, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMoldZigInfo::enableBtn()
{
	if (m_eMode == eMatDockIn) 
	{
		m_bDockIn.EnableWindow(TRUE);
		m_bCancelDockIn.EnableWindow(FALSE);
		m_bCheckDockIn.EnableWindow(TRUE);
		m_bExit.EnableWindow(TRUE);
	}
	else if (m_eMode == eMatDockOut)
	{
		m_bDockIn.EnableWindow(FALSE);
		m_bCancelDockIn.EnableWindow(FALSE);
		m_bCheckDockIn.EnableWindow(FALSE);
		m_bExit.EnableWindow(TRUE);
	}

}

void CMoldZigInfo::setFocusBtn(int iIndex)
{
	m_bStartEnd.SetValue(FALSE);
	m_bFeederNum.SetValue(FALSE);
	m_bTabICType.SetValue(FALSE);
	m_bMaker.SetValue(FALSE);
	m_bTabIcID.SetValue(FALSE);
	m_bValidationCheck.SetValue(FALSE);

	switch (iIndex)
	{
	case 0:
		m_bStartEnd.SetValue(TRUE);
		break;
	case 1:
		m_bFeederNum.SetValue(TRUE);
		break;
	case 2:
		m_bTabICType.SetValue(TRUE);
		break;
	case 3:
		m_bMaker.SetValue(TRUE);
		break;
	case 4:
		m_bTabIcID.SetValue(TRUE);
		break;
	case 5:
		m_bValidationCheck.SetValue(TRUE);
		break;
	}
}

void CMoldZigInfo::OnClickDockIn() 
{
	// TODO: Add your control notification handler code here
	CString strMsg;
	if (m_plnkLCNet->m_bConnected)
	{
		if (m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE == eMatStateEnable
			|| m_plnkCtrlTabFeeder->m_matZigInfo.m_nM_STATE == eMatStateRun)
		{
//		자재 상태가 Enable이나 Run상태입니다. 먼저 자재를 제거해 주세요
			MyMessageBox(MY_MESSAGE_BOX, 20500, _T("Confirm"), M_ICONINFORMATION|M_OK);
			return;
		}
	}

	//ZIG 정보를 입력하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 20511, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	char buffer[100];
	CString strTemp; 
	BOOL bTimeOut = FALSE;
	int sel = -1;
	CString szTextTemp1;
	CString szTextTemp2;
	ReadDisplayMessage(MAIN_AUTO, _T("AutoTabICInfoDlg_7"), &szTextTemp1);
	SetWindowText(szTextTemp1);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoTabICInfoDlg_8"), &szTextTemp2);
	SetWindowText(szTextTemp2);

//"Maker를 선택해 주세요", "SYS-LSI", "HIMAX", "직접입력"
//	CSystemThreeBtnDlg dlg1(szTextTemp1, "SYS-LSI", "HIMAX", szTextTemp2);

	CSystemThreeBtnDlg dlg2(szTextTemp1, "BarCode In", "RFID", szTextTemp2);

	m_sStartEnd.SetWindowText(_T("-"));
	m_sFeederNum.SetWindowText(_T("-"));
	m_sTabICType.SetWindowText(_T("-"));
	m_sMaker.SetWindowText(_T("-"));
	m_sTabIcID.SetWindowText(_T("-"));
	m_sValidationCheck.SetWindowText(_T("-"));

	m_strStart.Empty();
	m_strFeeder.Empty();
	m_strTabICType.Empty();
	m_strMaker.Empty();
	m_strTabICID.Empty();
	m_strValidationCheck.Empty();

	m_bDockIn.EnableWindow(FALSE);
	m_bCancelDockIn.EnableWindow(TRUE);
	m_bCheckDockIn.EnableWindow(FALSE);
	m_bExit.EnableWindow(FALSE);

	MTickTimer timer;
	
	//_________________________________________________________________________________________
	// Start
	setFocusBtn(0);

	m_strStart.Format(_T("START"));
	m_sStartEnd.SetCaption(m_strStart);

	//_________________________________________________________________________________________
	// Feeder 번호
	setFocusBtn(1);

	if (m_iFeederSelect == DEF_TABFEEDER1)
	{
		m_strFeeder.Format(_T("FEEDER1"));
	}
	else if (m_iFeederSelect == DEF_TABFEEDER2)
	{
		m_strFeeder.Format(_T("FEEDER2"));
	}

	m_sFeederNum.SetCaption(m_strFeeder);

	//_________________________________________________________________________________________
	// TabIC Type
	setFocusBtn(2);

	CString strtmp;
//작업변을 선택해 주세요
	if (MyMessageBox(MY_MESSAGE_BOX, 20502, _T("Question"), M_YESNO|M_ICONQUESTION, _T("SOURCE"), _T("GATE")) == IDYES)
		strtmp = _T("JIGS");
	else
		strtmp = _T("JIGG");

	SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType ? m_strTabICType.Format(_T("JIGS")) : m_strTabICType.Format(_T("JIGG"));
	
	if (strtmp != m_strTabICType)
	{
		// 작업변 정보가 설정된 값과 다릅니다. 확인 후 다시 진행해 주세요.
		MyMessageBox(MY_MESSAGE_BOX, 20503, _T("Error"), M_ICONERROR);
		goto DOCKIN_ERROR;
	}
	m_sTabICType.SetCaption(m_strTabICType);

	//_________________________________________________________________________________________
	// Maker
/*	sel = dlg1.DoModal();
	if (sel == 0)
		m_strMaker = "SYS-LSI";
	else if (sel == 1)
		m_strMaker = "HIMAX";
	else
	{
		setFocusBtn(3);

		CCommonGetValueDlg	valueDialog(20, _T(""), _T("Input Maker Name"));
		valueDialog.DoModal();
		m_strMaker = valueDialog.GetStringValue();
	}

	m_sMaker.SetCaption(m_strMaker);
*/
	//_________________________________________________________________________________________
	// TabIC ID

	sel = dlg2.DoModal();
	if (sel == 0) //Barcode 
	{
		setFocusBtn(4);
		bTimeOut = FALSE;
		m_plnkSerialBarCode->ClearRecvQueue();
		memset(buffer,0,sizeof(char)*100);
		timer.StartTimer();
		while (TRUE)
		{
			if (m_bCancelInput)	goto DOCKIN_ERROR;

			if (m_plnkSerialBarCode->ReceiveData(buffer,sizeof(buffer)) == SUCCESS)
				break;

			PeekAndPump();
			if (timer.MoreThan(30))
			{
				bTimeOut = TRUE;
				break;
			}
		}

		if (!bTimeOut)
		{
			m_strTabICID = CString(buffer);

			//m_strTabICID.Format(_T("%s"),buffer);
			m_strTabICID = m_strTabICID.Mid(0,m_strTabICID.GetLength()-1); //CR빼기 
			m_strTabICID.Remove(' ');
			m_strTabICID = RemoveSpecialChar(m_strTabICID);
			m_sTabIcID.SetCaption(m_strTabICID);
		}
		else
		{
			//입력 제한시간을 초과하였습니다. 수동으로 입력하시겠습니까?
			if (MyMessageBox(MY_MESSAGE_BOX, 20505, _T("Questioin"), M_YESNO|M_ICONQUESTION) == IDYES)
			{
				CCommonGetValueDlg	valueDialog(20, _T(""), _T("ZIG ID"));
				valueDialog.DoModal();
				m_strTabICID = valueDialog.GetStringValue();
				m_sTabIcID.SetCaption(m_strTabICID);
			}
			else
			{
				goto DOCKIN_ERROR;
			}
		}
	}
	else if (sel == 1)//RFID
	{
		m_strTabICID = m_pRFID->ReadData(ZIGID);

		if(m_strTabICID == "Data Read Error")
		{
			goto DOCKIN_ERROR;
		}
	}
	else //직접입력
	{
		CCommonGetValueDlg	valueDialog(20, _T(""), _T("ZIG ID"));
		valueDialog.DoModal();
		m_strTabICID = valueDialog.GetStringValue();
		m_sTabIcID.SetCaption(m_strTabICID);
	}
	//_________________________________________________________________________________________
	// End

	setFocusBtn(0);

	m_strEnd.Format(_T("END"));
	m_sStartEnd.SetCaption(m_strEnd);

	setFocusBtn(-1);	// 초기화
	enableBtn();
	m_bCancelInput = FALSE;

	// Data 입력에 성공하였습니다.
	MyMessageBox(MY_MESSAGE_BOX, 20506, _T("Confirm"));

	// 입력 단계 간소화 작업 
	OnClickCheckDockIn();

	return;

DOCKIN_ERROR:
	setFocusBtn(-1);	// 초기화
	enableBtn();
	m_bCancelInput = FALSE;
	//Data 입력에 실패했습니다. 처음부터 다시 시작하세요.
	MyMessageBox(MY_MESSAGE_BOX, 20507, _T("Error"), M_ICONERROR);
	return;
}

void CMoldZigInfo::OnClickCancelDockIn() 
{
	// TODO: Add your control notification handler code here
	m_bCancelInput = TRUE;
}

void CMoldZigInfo::OnClickCheckDockIn() 
{
	// TODO: Add your control notification handler code here
	CString strMsg;
//	TabIC 정보를 현재 Feeder에 적용하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 20601, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	WCHAR m_cBufGet[100];
	WCHAR m_cbuf[100];
	MMaterial Material = m_plnkCtrlTabFeeder->m_matZigInfo;

	Material.m_sM_BATCHID = m_strTabICID;

	Material.m_nM_KIND = 12; // ZIG

	Material.m_sM_MAKER = m_strMaker;

	BOOL bUserKeyIn = FALSE;

	CString szTextTemp;
	strMsg.Format(_T("[ZIG Total Quantity : %d]\r\n"), Material.m_nT_QTY);
//현재 입력된 ZIG 총 수량 값을 사용하시겠습니까?
	ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_20509"), &szTextTemp);
	strMsg += szTextTemp;
	int iResult = MyMessageBox(strMsg, _T("Question"), M_ICONQUESTION|M_YESNO);
	if (IDNO == iResult)
		bUserKeyIn = TRUE;

	// 수량정보가 없는 Maker 일 경우 or 사용 했던 Reel 투입되는 경우(SELSK) 에 수량을 수동으로 입력받는다.
	if (TRUE == bUserKeyIn || m_plnkLCNet->IsNoParsingInfoMaker(m_strMaker))
	{
		memset(m_cBufGet, 0, sizeof(m_cBufGet));
		swprintf(m_cbuf,_T("%d"), 0);

//		ZIG 총 수량을 입력해주세요
		CString szTextTemp3;
		ReadDisplayMessage(MAIN_AUTO, _T("AutoTabICInfoDlg_9"), &szTextTemp3);
		SetWindowText(szTextTemp3);

		if (GetNumberBox(m_cBufGet, 10, m_cbuf, szTextTemp3.GetBuffer(szTextTemp3.GetLength())))
		{
			Material.m_nT_QTY = _wtoi(m_cBufGet);
			Material.m_nR_QTY = _wtoi(m_cBufGet);
		}
		else
		{
//			총 수량 입력에 실패하였습니다.
			MyMessageBox(MY_MESSAGE_BOX, 20510, _T("Error"), M_ICONERROR);
			return;
		}
	}

	Material.m_sM_STEP = m_strTabICType;

	Material.m_nM_STATE = eMatStateIdle;

	//110705.kms Modify____________
	if (m_iFeederSelect == DEF_TABFEEDER1 )
	{
		Material.m_sPORTID = m_plnkSystemData->m_strModulePortID[4];
	}
	else if (m_iFeederSelect == DEF_TABFEEDER2 )
	{
		Material.m_sPORTID = m_plnkSystemData->m_strModulePortID[5];
	}
	//_____________________________

	m_plnkCtrlTabFeeder->m_matZigInfo = Material;

	//m_plnkLCNet->MaterialReport(eMatDockIn, &Material); //SESL 은 CheckMaterialValidation 필요
	m_plnkLCNet->MaterialReport(eMatStockIn, &Material); //SESL 은 CheckMaterialValidation 필요

	m_plnkCtrlTabFeeder->SetZigConsumStart(FALSE);
}

void CMoldZigInfo::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CMoldZigInfo::PeekAndPump()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			return;
		}
	}
}

CString CMoldZigInfo::RemoveSpecialChar(CString sRcvStr)
{
	CString	sOneChar;
	CString sRtnStr=_T("");
	
	for (int i=0; i<sRcvStr.GetLength(); i++)
	{
		sOneChar = sRcvStr.Mid(i, 1);
		//LKH_20090115 : 테스트 필수!!
		//sRtnStr += (0x20<=sOneChar && 0x7E>=sOneChar) ? sOneChar : 0x20;
		sRtnStr += ((WCHAR)0x20<=sOneChar && (WCHAR)0x7E>=sOneChar) ? sOneChar : (WCHAR)0x20;
	}
	sRtnStr.TrimLeft();
	sRtnStr.TrimRight(); 
	
	return sRtnStr;
}
