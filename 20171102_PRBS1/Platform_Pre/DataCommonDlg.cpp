// SystemDataDlg.cpp : implementation file
//
#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataCommonDlg.h"
#include "MPlatformOlbSystem.h"
#include "SystemThreeBtnDlg.h"
#include "MSystemData.h"
#include "MProcessData.h"

// ML_hongju
#include "MTrsAutoManager.h"

#include "ModulePortIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataCommonDlg dialog

CDataCommonDlg::CDataCommonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataCommonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataCommonDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	memset(m_dData, 0, sizeof(m_dData));

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CDataCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataCommonDlg)
	DDX_Control(pDX, IDB_EXIT, m_bExit);
	DDX_Control(pDX, IDB_SAVE, m_bSave);
	//}}AFX_DATA_MAP

	// Button
	DDX_Control(pDX, IDB_SYSTEM0, m_btnItem[0]);
	DDX_Control(pDX, IDB_SYSTEM1, m_btnItem[1]);
	DDX_Control(pDX, IDB_SYSTEM2, m_btnItem[2]);
	DDX_Control(pDX, IDB_SYSTEM3, m_btnItem[3]);
	DDX_Control(pDX, IDB_SYSTEM4, m_btnItem[4]);
	DDX_Control(pDX, IDB_SYSTEM5, m_btnItem[5]);
	DDX_Control(pDX, IDB_SYSTEM6, m_btnItem[6]);
	DDX_Control(pDX, IDB_SYSTEM7, m_btnItem[7]);
	DDX_Control(pDX, IDB_SYSTEM8, m_btnItem[8]);
	DDX_Control(pDX, IDB_SYSTEM9, m_btnItem[9]);
	//syr_090311
	DDX_Control(pDX, IDB_SYSTEM10, m_btnItem[10]);
	DDX_Control(pDX, IDB_SYSTEM22, m_btnItem[11]);

	// Label
	DDX_Control(pDX, IDS_SYSTEM0, m_lblItem[0]);
	DDX_Control(pDX, IDS_SYSTEM1, m_lblItem[1]);
	DDX_Control(pDX, IDS_SYSTEM2, m_lblItem[2]);
	DDX_Control(pDX, IDS_SYSTEM3, m_lblItem[3]);
	DDX_Control(pDX, IDS_SYSTEM4, m_lblItem[4]);
	DDX_Control(pDX, IDS_SYSTEM5, m_lblItem[5]);
	DDX_Control(pDX, IDS_SYSTEM6, m_lblItem[6]);
	DDX_Control(pDX, IDS_SYSTEM7, m_lblItem[7]);
	DDX_Control(pDX, IDS_SYSTEM8, m_lblItem[8]);
	DDX_Control(pDX, IDS_SYSTEM9, m_lblItem[9]);
	//syr_090311
	DDX_Control(pDX, IDS_SYSTEM22, m_lblItem[10]);
	DDX_Control(pDX, IDS_SYSTEM11, m_lblItem[11]);
	DDX_Control(pDX, IDS_SYSTEM24, m_lblItem[12]);
	DDX_Control(pDX, IDS_SYSTEM25, m_lblItem[13]);
	DDX_Control(pDX, IDS_SYSTEM26, m_lblItem[14]);
}


BEGIN_MESSAGE_MAP(CDataCommonDlg, CDialog)
	//{{AFX_MSG_MAP(CDataCommonDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataCommonDlg message handlers

BEGIN_EVENTSINK_MAP(CDataCommonDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataCommonDlg)
	ON_EVENT(CDataCommonDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM0, -600 /* Click */, OnSystem0, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM1, -600 /* Click */, OnSystem1, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM2, -600 /* Click */, OnSystem2, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM3, -600 /* Click */, OnSystem3, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM4, -600 /* Click */, OnSystem4, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM5, -600 /* Click */, OnSystem5, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM6, -600 /* Click */, OnSystem6, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM7, -600 /* Click */, OnSystem7, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM8, -600 /* Click */, OnSystem8, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM9, -600 /* Click */, OnSystem9, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM22, -600 /* Click */, OnClickSystem22, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM23, -600 /* Click */, OnClickSystem23, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM24, -600 /* Click */, OnClickSystem24, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM25, -600 /* Click */, OnClickSystem25, VTS_NONE)
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM26, -600 /* Click */, OnClickSystem26, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CDataCommonDlg, IDB_SYSTEM10, DISPID_CLICK, CDataCommonDlg::ClickSystem10, VTS_NONE)
END_EVENTSINK_MAP()

void CDataCommonDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDataCommonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	displayItemLabel(TRUE);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	SetWindowText(_T("Setting System Common Data"));
	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataCommonDlg_1"), &szTextTemp);
//	사용 언어
	GetDlgItem(IDB_SYSTEM2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataCommonDlg_2"), &szTextTemp);
//	금형 RFID\r\nWriting 주기
	GetDlgItem(IDB_SYSTEM7)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataCommonDlg_3"), &szTextTemp);
//	금형 Blow\r\n주기
	GetDlgItem(IDB_SYSTEM8)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataCommonDlg_4"), &szTextTemp);
//	금형 Blow\r\n시간
	GetDlgItem(IDB_SYSTEM9)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataCommonDlg_5"), &szTextTemp);
//	저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataCommonDlg_6"), &szTextTemp);
//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	//_______________

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataCommonDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}

void CDataCommonDlg::getUserInput(int iItemNo)
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________

	ASSERT(iItemNo >= 0 && iItemNo < DEF_DATA_COMMON_DLG_ALL_ITEM);

	int iResult = SUCCESS;
	memset(m_cBufGet, 0, sizeof(m_cBufGet));
	CString strMsg;
	int iLanguageData = MLanguageManager::GetNowLanguage();
	switch (iItemNo)
	{
	case 2:
		{
			iLanguageData++;
			if (iLanguageData > 2)	
				iLanguageData = 0;

			m_dData[iItemNo]=iLanguageData;
		}
		break;

	case 3:
//		속도 모드를 선택해 주세요.
		iResult = MyMessageBox(MY_MESSAGE_BOX, 40100, _T("Velocity Mode"), M_ICONQUESTION|M_YESNO, _T("Normal"), _T("Slow"));
		if (IDYES == iResult)
			m_dData[iItemNo] = 1.0;
		else
			m_dData[iItemNo] = 0.0;

		break;

	case 4:
		if (!GetValueBox(m_cBufGet, 20, _T(""), _T("LineController IP"), FALSE, FALSE))
			return;
		m_strLineControllerIP.Format(_T("%s"), m_cBufGet);
		break;

	default:
		//syr_090312 수정
		/*/
		swprintf(m_cBuf, _T("%.0f"), m_dData[iItemNo]);
		if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
				return;	

			m_dData[iItemNo] = wcstod(m_cBufGet, NULL);
		break;

		*/

		//syr_090312: PanelPreAlignDelay Data 값을 수정시 0~5 사이의 값만 입력하게 하기 위한 조치
		swprintf(m_cBuf, _T("%.0f"), m_dData[iItemNo]);
		/*
		if(iItemNo == 10) // Panel PreAlign Data 값 입력시
		{
			while(1)
			{				
			if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
				return;				
				m_dData[iItemNo] = wcstod(m_cBufGet, NULL);

			if(m_dData[iItemNo] < 5 && m_dData[iItemNo] > 0)
				break;
			else
				MyMessageBox(_T("Input Value Range: 0 ~ 5"), _T("Input Error"));
			}
		}
		else
		*/
		if (iItemNo == 11)
		{
			while(1)
			{
			if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
				return;	
			m_dData[iItemNo] = wcstod(m_cBufGet, NULL);

			if(m_dData[iItemNo] < 801.0)
				break;
			else
				MyMessageBox(_T("Input Value More Less 800"), _T("Input Error"));
			}

		}
		else if (iItemNo == 12)
		{
			while(1)
			{
			if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
				return;	
			m_dData[iItemNo] = wcstod(m_cBufGet, NULL);

			if(m_dData[iItemNo] >= 0.0)
				break;
			else
				MyMessageBox(_T("Input Value Error"), _T("Input Error"));
			}

		}
		else // Panel PreAlign Data 이외의 값 입력시
		{
			if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
				return;	

			m_dData[iItemNo] = wcstod(m_cBufGet, NULL);
		}
		break;
	}
	displayItemLabel();
}

void CDataCommonDlg::OnSave() 
{
//	변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40101, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

// 사용자 설정 사항 아님.	m_plnkSystemData->m_eSystemType				= (ESystemType)(int)m_dData[0];
	m_plnkSystemData->m_iUnitNumber				= (int)m_dData[1];
	m_plnkSystemData->m_uiLanguageSelect		= (UINT)m_dData[2];
	m_plnkSystemData->m_iVelocityMode			= (int)m_dData[3];
	m_plnkSystemData->m_strLineControllerIP		= m_strLineControllerIP;
	m_plnkSystemData->m_uiLineControllerPort	= (UINT)m_dData[5];
	m_plnkSystemData->m_dStopperArrivingDelay	= m_dData[6];
	m_plnkSystemData->m_uiMoldMaxCount			= (UINT)m_dData[7];
	m_plnkSystemData->m_uiMoldBlowPeriod		= (UINT)m_dData[8];
	m_plnkSystemData->m_uiMoldBlowTime			= (UINT)m_dData[9];
	//syr_090311
	m_plnkSystemData->m_dPanelTabDist	= m_dData[10];
	//m_plnkSystemData->m_dPanelPreAlignDelay = 0; //-> 초기값 설정 위해 임시로 넣었던 라인
	m_plnkSystemData->m_dPanelAlignerWorkVelocity	= m_dData[11];

	m_plnkSystemData->m_dEmptyElapseTime		= m_dData[12];
	m_plnkSystemData->m_dTabXCheck				= m_dData[13];
	m_plnkSystemData->m_dTabYCheck				= m_dData[14];


	m_plnkSystemData->WriteData();

	MOlbSystemPre.GetProcessData()->ChangeProcessData();

	MLanguageManager::SetNowLanguage(m_plnkSystemData->m_uiLanguageSelect);

	
// ML_hongu MainView에 화면을 Display
	
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetManualViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetDataViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetTeachViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetLogViewHandle(), WM_DISP_MULTI_LANGUAGE_DLG_REQ, NULL, NULL);
	
	

}

void CDataCommonDlg::displayItemLabel(BOOL bUpdate)
{
	CString label_buf[DEF_DATA_COMMON_DLG_ALL_ITEM];

	if (bUpdate)
	{
		m_dData[0] = (double)m_plnkSystemData->m_eSystemType;
		m_dData[1] = (double)m_plnkSystemData->m_iUnitNumber;
		m_dData[2] = (double)m_plnkSystemData->m_uiLanguageSelect;
		m_dData[3] = (double)m_plnkSystemData->m_iVelocityMode;
		m_strLineControllerIP = m_plnkSystemData->m_strLineControllerIP;
		m_dData[5] = (double)m_plnkSystemData->m_uiLineControllerPort;
		m_dData[6] = m_plnkSystemData->m_dStopperArrivingDelay;
		m_dData[7] = (double)m_plnkSystemData->m_uiMoldMaxCount;
		m_dData[8] = (double)m_plnkSystemData->m_uiMoldBlowPeriod;
		m_dData[9] = (double)m_plnkSystemData->m_uiMoldBlowTime;
		// syr_090312
		m_dData[10] = m_plnkSystemData->m_dPanelTabDist;
		m_dData[11] = m_plnkSystemData->m_dPanelAlignerWorkVelocity;
		m_dData[12] = m_plnkSystemData->m_dEmptyElapseTime;

		//131029. SJ_YTH
		m_dData[13] = m_plnkSystemData->m_dTabXCheck;
		m_dData[14] = m_plnkSystemData->m_dTabYCheck;
	}

	for (int i = 0; i < DEF_DATA_COMMON_DLG_ALL_ITEM; i++)
	{
		switch (i)
		{
		case 0:	// 작업변 선택
			if (m_dData[i] == 0.0)		label_buf[i] = "Source";
			else if (m_dData[i] == 1.0)	label_buf[i] = "Gate";
			else						label_buf[i] = "Source";
			break;
		//LKH_20081211 : Multilanguage 대응
		case 2:	// Language
			// 0:Korean, 1:English, 2:Chinese, 3:Slovak
			if (m_dData[i] == 0.0)		label_buf[i] = _T("Korean");
			else if (m_dData[i] == 1.0)	label_buf[i] = _T("English");
			else if (m_dData[i] == 2.0)	label_buf[i] = _T("Chinese");
			else						label_buf[i] = _T("Slovak");
			break;

		case 3:	// Velocity Mode
			if (m_dData[i] == 0.0)		label_buf[i] = "Slow";
			else						label_buf[i] = "Normal";
			break;
		case 4:	// LC IP
			label_buf[i] = m_strLineControllerIP;
			break;
		case 6:	// Stopper Delay Time
			label_buf[i].Format(_T("%.1f"), m_dData[i]);
			break;
		//syr_090311
		case 10: // Panel PreAlign Delay Time
			label_buf[i].Format(_T("%.3f"), m_dData[i]);
			break;
		case 13: 
			label_buf[i].Format(_T("%.3f"), m_dData[i]);
			break;
		case 14: 
			label_buf[i].Format(_T("%.3f"), m_dData[i]);
			break;


		default:
			label_buf[i].Format(_T("%.0f"), m_dData[i]);
			break;
		}
		m_lblItem[i].SetCaption(label_buf[i]);
	}
}

void CDataCommonDlg::OnSystem0() 
{
	// 사용자 설정 사항 아님. getUserInput(0);
}

void CDataCommonDlg::OnSystem1() 
{
	getUserInput(1);
}

void CDataCommonDlg::OnSystem2() 
{
	getUserInput(2);	
}

void CDataCommonDlg::OnSystem3() 
{
	getUserInput(3);	
}

void CDataCommonDlg::OnSystem4() 
{
	getUserInput(4);
}

void CDataCommonDlg::OnSystem5() 
{
	getUserInput(5);
}

void CDataCommonDlg::OnSystem6() 
{
	getUserInput(6);
}

void CDataCommonDlg::OnSystem7() 
{
	getUserInput(7);
}

void CDataCommonDlg::OnSystem8() 
{
	getUserInput(8);
}

void CDataCommonDlg::OnSystem9() 
{
	getUserInput(9);
}

//syr_090311
void CDataCommonDlg::ClickSystem10()
{	
	getUserInput(10);

}


void CDataCommonDlg::OnClickSystem22()
{	
	getUserInput(11);
}

void CDataCommonDlg::OnClickSystem23() 
{
	// TODO: Add your control notification handler code here
	CModulePortIDDlg dlg;
	dlg.DoModal();
}

void CDataCommonDlg::OnClickSystem24() 
{
	// TODO: Add your control notification handler code here
	getUserInput(12);
}

void CDataCommonDlg::OnClickSystem25() 
{
	// TODO: Add your control notification handler code here
	getUserInput(13);
}

void CDataCommonDlg::OnClickSystem26() 
{
	// TODO: Add your control notification handler code here
	getUserInput(14);
}
