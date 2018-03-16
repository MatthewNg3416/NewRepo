// CommonErrMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MManageOpPanel.h"
#include "CommonErrMsgDlg.h"
#include "MPlatformOlbSystem.h"
#include "MDebug.h"

//LKH_20090114 : CommonErrMsgDlg 통합 관리용 define
#if defined DEF_PLATFORM_ARP_SYSTEM
	extern MPlatformOlbSystem	MOlbSystemACF;
#elif defined DEF_PLATFORM_PREBONDER_SYSTEM
	extern MPlatformOlbSystem	MOlbSystemPre;
#elif defined DEF_PLATFORM_FINALBONDER_SYSTEM
	extern MPlatformOlbSystem	MOlbSystemFinal;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonErrMsgDlg dialog

CCommonErrMsgDlg::CCommonErrMsgDlg(CWnd* pParent /*=NULL*/)	: CDialog(CCommonErrMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonErrMsgDlg)
	m_sErrorLevel = _T("");
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(RGB(255,255,255));
	m_bIsRecoveryExist = FALSE;

//LKH_20090114 : CommonErrMsgDlg 통합 관리용 define
#if defined DEF_PLATFORM_ARP_SYSTEM
	m_plnkManageOpPanel = MOlbSystemACF.GetManageOpPanelComponent();
	m_bBuzzerMode = MOlbSystemACF.GetSystemData()->m_bUseBuzzer;
#elif defined DEF_PLATFORM_PREBONDER_SYSTEM
	m_plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
	m_bBuzzerMode = MOlbSystemPre.GetSystemData()->m_bUseBuzzer;
#elif defined DEF_PLATFORM_FINALBONDER_SYSTEM
	m_plnkManageOpPanel = MOlbSystemFinal.GetManageOpPanelComponent();
	m_bBuzzerMode = MOlbSystemFinal.GetSystemData()->m_bUseBuzzer;
#endif
	
	m_plnkManageOpPanel->SetBuzzerMode(m_bBuzzerMode);
}


void CCommonErrMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonErrMsgDlg)
	DDX_Control(pDX, IDB_INFO, m_bInfo);
	DDX_Control(pDX, IDB_OK, m_bOK);
	DDX_Control(pDX, IDS_MESSAGE, m_sMessage);
	DDX_Control(pDX, IDS_MESSAGE_TITLE, m_sMessageTitle);
	DDX_Control(pDX, IDS_PROCESS_OBJ_ID, m_sProcess);
	DDX_Control(pDX, IDS_PROCESS_OBJ_TITLE, m_sProcessTitle);
	DDX_Control(pDX, IDS_UNIT_OBJ_ID, m_sUnit);
	DDX_Control(pDX, IDS_UNIT_OBJ_TITLE, m_sUnitTitle);
	DDX_Control(pDX, IDS_ERROR_INFO_TITLE, m_sErrorInfoTitle);
	//DDX_Control(pDX, IDS_ERROR_INFO, m_sErrorInfo);
	DDX_Control(pDX, IDS_ERROR_RECOVERY_TITLE, m_sErrorRecoveryTitle);
	DDX_Control(pDX, IDS_ERROR_RECOVERY, m_sErrorRecovery);
	DDX_Control(pDX, IDB_BUZZER_OFF, m_btnBuzzerOff);
	DDX_Text(pDX, IDC_EDIT1, m_sErrorLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonErrMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonErrMsgDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonErrMsgDlg message handlers

BEGIN_EVENTSINK_MAP(CCommonErrMsgDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommonErrMsgDlg)
	ON_EVENT(CCommonErrMsgDlg, IDB_OK, -600 /* Click */, OnOk, VTS_NONE)
	ON_EVENT(CCommonErrMsgDlg, IDB_INFO, -600 /* Click */, OnClickInfo, VTS_NONE)
	ON_EVENT(CCommonErrMsgDlg, IDB_BUZZER_OFF, -600 /* Click */, OnClickBuzzerOff, VTS_NONE)
	ON_EVENT(CCommonErrMsgDlg, IDB_CREATE_REPORT, -600 /* Click */, OnClickCreateReport, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCommonErrMsgDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void CCommonErrMsgDlg::CloseWnd()
{
	CDialog::OnCancel();
}

BOOL CCommonErrMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strSection;
	
	//LKH_20081118 : 에러 조치 방법 숨김, 에러발생정보를 보이기 위한 윈도우 리사이즈 막음
	// 에러 발생 정보 내용을 3D Button에서 Edit Box로 교체(글자수 제한 때문)

	// TODO: Add extra initialization here
	//getErrorRecovery();	// Error Recovery 정보를 읽어온다.
	//resizeWindoow();	// 정보에 맞게 Window 크기를 조절한다.
	m_sErrorRecoveryTitle.ShowWindow(SW_HIDE);
	m_sErrorRecovery.ShowWindow(SW_HIDE);
	m_bInfo.ShowWindow(SW_HIDE);
	displayMessage();	// 메세지를 Display 한다.

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;

	//공정
	ReadDisplayMessage(SUB_DLG, _T("CommonErrMsgDlg_1"), &szTextTemp);
	GetDlgItem(IDS_PROCESS_OBJ_TITLE)->SetWindowText(szTextTemp);
	//유닛
	ReadDisplayMessage(SUB_DLG, _T("CommonErrMsgDlg_2"), &szTextTemp);
	GetDlgItem(IDS_UNIT_OBJ_TITLE)->SetWindowText(szTextTemp);
	//에러 메세지
	ReadDisplayMessage(SUB_DLG, _T("CommonErrMsgDlg_3"), &szTextTemp);
	GetDlgItem(IDS_MESSAGE_TITLE)->SetWindowText(szTextTemp);
	//조치 방법
	ReadDisplayMessage(SUB_DLG, _T("CommonErrMsgDlg_4"), &szTextTemp);
	GetDlgItem(IDS_ERROR_RECOVERY_TITLE)->SetWindowText(szTextTemp);
	//에러 발생 정보
	ReadDisplayMessage(SUB_DLG, _T("CommonErrMsgDlg_5"), &szTextTemp);
	GetDlgItem(IDS_ERROR_INFO_TITLE)->SetWindowText(szTextTemp);
	//확인
	ReadDisplayMessage(SUB_DLG, _T("CommonErrMsgDlg_6"), &szTextTemp);
	GetDlgItem(IDB_OK)->SetWindowText(szTextTemp);
	//에러 발생 정보
	ReadDisplayMessage(SUB_DLG, _T("CommonErrMsgDlg_7"), &szTextTemp);
	GetDlgItem(IDB_INFO)->SetWindowText(szTextTemp);
	//경고음 끄기
	ReadDisplayMessage(SUB_DLG, _T("CommonErrMsgDlg_8"), &szTextTemp);
	GetDlgItem(IDB_BUZZER_OFF)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonErrMsgDlg::displayMessage()
{
	WCHAR m_MsgTitle[200];

	m_sMessage.SetCaption(m_Msg);
	//m_sErrorInfo.SetCaption(m_MsgInfo);


	if(m_bIsRecoveryExist) m_sErrorRecovery.SetCaption(m_MsgRecovery);

	m_sProcess.SetCaption(m_MsgProcess);
	m_sUnit.SetCaption(m_MsgUnit);

	swprintf(m_MsgTitle, _T("Error Display"));

	SetWindowText(m_MsgTitle);
}

// 사용 안 함
void CCommonErrMsgDlg::getErrorRecovery()
{
	CString	strItem;
	WCHAR	chrData[256];

    memset(chrData, NULL, sizeof(chrData));
	strItem.Format(_T("%d"),m_iUnitErrorCode/1000);
	CString szTemp=DEF_SYSTEM_ERROR_RECOVERY_FILE;

	GetPrivateProfileString(_T("Error Recovery"), strItem, _T("NO Item"), chrData, 256, szTemp.GetBuffer(szTemp.GetLength()));

	if(_tcscmp(chrData, _T("NO Item")) == 0) 
	{
		GetDlgItem(IDS_ERROR_RECOVERY)->SetWindowText(_T("No Info"));
		m_bIsRecoveryExist = FALSE;
	}
	else
	{
		swprintf(&m_MsgRecovery[0],_T("%s"),chrData);
		m_bIsRecoveryExist = TRUE;
	}
}

// 사용 안 함
void CCommonErrMsgDlg::resizeWindoow()
{
	// 에러 발생 정보 창은 숨긴다.
	m_sErrorInfoTitle.ShowWindow(SW_HIDE);
	//m_sErrorInfo.ShowWindow(SW_HIDE);

	//  Error Recovery 정보가 없으면
	
	if(!m_bIsRecoveryExist)
	{
		// Recovery 정보 Window 숨기기
		m_sErrorRecoveryTitle.ShowWindow(SW_HIDE);
		m_sErrorRecovery.ShowWindow(SW_HIDE);

		// Window 축소
		CRect	DialogRect;
		CPoint	LeftTop;
		GetWindowRect(&DialogRect);

		LeftTop	= DialogRect.TopLeft();
		MoveWindow(LeftTop.x, LeftTop.y, DialogRect.Width(), DialogRect.Height() - 70);

		// OK 버튼 위로 보내기 
		CRect  ButtonRect;
		CPoint ButtonLeftTop;

		m_bOK.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		m_bOK.MoveWindow(ButtonLeftTop.x - LeftTop.x, ButtonLeftTop.y - LeftTop.y - 90,
							ButtonRect.Width(), ButtonRect.Height());
		
		// Buzzeroff 버튼 위로 보내기 
		m_btnBuzzerOff.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		m_btnBuzzerOff.MoveWindow(ButtonLeftTop.x - LeftTop.x, ButtonLeftTop.y - LeftTop.y - 90,
							ButtonRect.Width(), ButtonRect.Height());

		// Info 버튼 위로 보내기
		m_bInfo.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		m_bInfo.MoveWindow(ButtonLeftTop.x - LeftTop.x, ButtonLeftTop.y - LeftTop.y - 90,
							ButtonRect.Width(), ButtonRect.Height());
	}
}


void CCommonErrMsgDlg::SetTextMsg(WCHAR* strProcess, WCHAR* strUnit, WCHAR* strMsg, WCHAR *strInfo, int iErrorCode)
{
	_tcscpy(m_MsgProcess, strProcess);
	_tcscpy(m_MsgUnit, strUnit);
	_tcscpy(m_Msg, strMsg);
	//_tcscpy(m_MsgInfo,strInfo);
	m_sErrorLevel=strInfo;

	m_iUnitErrorCode = iErrorCode;
}

BOOL CCommonErrMsgDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
        if (pMsg->wParam == 13) 
		{
			pMsg->message = 0;
			pMsg->wParam = 0;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CCommonErrMsgDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	iButton = MLOK;
	EndDialog(iButton);	
}

void CCommonErrMsgDlg::OnClickInfo()
{
	extendWindow();

	// 에러 발생 정보 창은 보이게 한다.
	m_sErrorInfoTitle.ShowWindow(SW_SHOW);
	//m_sErrorInfo.ShowWindow(SW_SHOW);

	// 에러 발생 정보 버튼을 안보이게 한다.
	m_bInfo.ShowWindow(SW_HIDE);
}

// 에러 발생 정보를 표시하기 위해 Window를 확장 시킨다.
void CCommonErrMsgDlg::extendWindow()
{
	CRect	DialogRect;
	CPoint	LeftTop;
	CRect  ButtonRect;
	CPoint ButtonLeftTop;

	if (m_bIsRecoveryExist) //  Error Recovery 정보가 있을 경우
	{
		// Window 확장
		GetWindowRect(&DialogRect);
		LeftTop = DialogRect.TopLeft();
		MoveWindow(LeftTop.x, LeftTop.y, DialogRect.Width(), DialogRect.Height() + 70);

		// OK 버튼 아래로 보내기 
		m_bOK.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		m_bOK.MoveWindow(ButtonLeftTop.x - LeftTop.x, ButtonLeftTop.y - LeftTop.y + 50,
							ButtonRect.Width(), ButtonRect.Height());

		// Buzzer Off 버튼 아래로 보내기 
		m_btnBuzzerOff.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		m_btnBuzzerOff.MoveWindow(ButtonLeftTop.x - LeftTop.x, ButtonLeftTop.y - LeftTop.y + 50,
							ButtonRect.Width(), ButtonRect.Height());
	}
	else //  Error Recovery 정보가 없을 경우
	{
		// Window 확장
		GetWindowRect(&DialogRect);
		LeftTop = DialogRect.TopLeft();
		MoveWindow(LeftTop.x, LeftTop.y, DialogRect.Width(), DialogRect.Height() + 70);

		// OK 버튼 아래로 보내기 
		m_bOK.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		m_bOK.MoveWindow(ButtonLeftTop.x - LeftTop.x, ButtonLeftTop.y - LeftTop.y + 50,
							ButtonRect.Width(), ButtonRect.Height());

		// Buzzer Off 버튼 아래로 보내기 
		m_btnBuzzerOff.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		m_btnBuzzerOff.MoveWindow(ButtonLeftTop.x - LeftTop.x, ButtonLeftTop.y - LeftTop.y + 50,
							ButtonRect.Width(), ButtonRect.Height());

		// 발생정보 Title 위로 보내기
		m_sErrorInfoTitle.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		m_sErrorInfoTitle.MoveWindow(ButtonLeftTop.x - LeftTop.x - 3, ButtonLeftTop.y - LeftTop.y - 102,
							ButtonRect.Width(),ButtonRect.Height());

		// 발생정보 메세지 위로 보내기
		//m_sErrorInfo.GetWindowRect(&ButtonRect);
		ButtonLeftTop = ButtonRect.TopLeft();
		//m_sErrorInfo.MoveWindow(ButtonLeftTop.x-LeftTop.x-2,ButtonLeftTop.y-LeftTop.y-110,
		//					ButtonRect.Width(),ButtonRect.Height());

	}
}

void CCommonErrMsgDlg::OnClickBuzzerOff() 
{
	// TODO: Add your control notification handler code here
	m_plnkManageOpPanel->SetBuzzerMode(FALSE);
}

void CCommonErrMsgDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//m_plnkManageOpPanel->SetBuzzerMode(m_bBuzzerMode);
}

void CCommonErrMsgDlg::OnClickCreateReport() 
{
	// TODO: Add your control notification handler code here
	MDebug::CreateUserDump();
}
