// MotorOriginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "DefIOAddrPreBonder.h"
#include "AutoMotorOriginDlg.h"
#include "MSercosAxis.h"
#include "MSercosSetup.h"
#include "MManageOpPanel.h"
#include "MPanelAligner.h"
#include "MPanelTransfer.h"
#include "MTabFeeder.h"
#include "MTabCarrier.h"
#include "MTabMounter.h"
#include "MTabAligner.h"
#include "MCameraCarrier.h"
#include "MCtrlTabFeeder.h"
#include "MTrsAutoManager.h"
#include "IIO.h"
#include <math.h>

//120208.kms________
#include "MModelChange.h"

// hongju_SESL
#include "MCANSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAutoMotorOriginDlg dialog

extern	MPlatformOlbSystem	MOlbSystemPre;

// sesl_jdy
int CAutoMotorOriginDlg::sm_rgiGroupResult[DEF_MAX_ORIGIN_GROUP];
int CAutoMotorOriginDlg::sm_rgiGroupComplete[DEF_MAX_ORIGIN_GROUP];

CAutoMotorOriginDlg::CAutoMotorOriginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoMotorOriginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoMotorOriginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	WCHAR pBuf[MAX_PATH] = {0};
	::GetCurrentDirectory(sizeof(pBuf), pBuf);

	m_strOnOriginIcon.Format(_T("%s\\res\\전부선택.ico"), pBuf);
	m_strOffOriginIcon.Format(_T(""), pBuf);

	// selsk_jdy
	int i = 0;
	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		m_rgdTabMounterPrepareMoveDistance[i] = 0.0;
		m_rgdInspectionPrepareMoveDistance[i] = 0.0;
		m_rgdTabCarrierPrepareMoveDistance[i] = 0.0;
	}
	
	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		m_plnkAxis[i] = MOlbSystemPre.GetMotionComponent(i);

	m_plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkSercosSetup = MOlbSystemPre.GetSercosSetupComponent();
	m_plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
	m_plnkCANSetup = MOlbSystemPre.GetCANSetupComponent();

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		m_rgbForceOriginReturn[i] = FALSE;
}


void CAutoMotorOriginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoMotorOriginDlg)
	DDX_Control(pDX, IDB_SET_FORCE_ORIGIN_RETURN, m_btnForceOrigin);
	//}}AFX_DATA_MAP
	
	// DefAxisMPI~.h에 등록된 ID 순으로 배열을 맞춤
	DDX_Control(pDX, IDB_AXIS_ORIGIN_0,		m_btnAxisOrigin[DlgID2AxisID(0)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_1,		m_btnAxisOrigin[DlgID2AxisID(1)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_2,		m_btnAxisOrigin[DlgID2AxisID(2)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_3,		m_btnAxisOrigin[DlgID2AxisID(3)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_4,		m_btnAxisOrigin[DlgID2AxisID(4)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_5,		m_btnAxisOrigin[DlgID2AxisID(5)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_6,		m_btnAxisOrigin[DlgID2AxisID(6)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_7,		m_btnAxisOrigin[DlgID2AxisID(7)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_8,		m_btnAxisOrigin[DlgID2AxisID(8)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_9,		m_btnAxisOrigin[DlgID2AxisID(9)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_10,	m_btnAxisOrigin[DlgID2AxisID(10)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_11,	m_btnAxisOrigin[DlgID2AxisID(11)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_12,	m_btnAxisOrigin[DlgID2AxisID(12)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_13,	m_btnAxisOrigin[DlgID2AxisID(13)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_14,	m_btnAxisOrigin[DlgID2AxisID(14)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_15,	m_btnAxisOrigin[DlgID2AxisID(15)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_16,	m_btnAxisOrigin[DlgID2AxisID(16)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_17,	m_btnAxisOrigin[DlgID2AxisID(17)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_18,	m_btnAxisOrigin[DlgID2AxisID(18)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_19,	m_btnAxisOrigin[DlgID2AxisID(19)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_20,	m_btnAxisOrigin[DlgID2AxisID(20)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_21,	m_btnAxisOrigin[DlgID2AxisID(21)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_22,	m_btnAxisOrigin[DlgID2AxisID(22)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_23,	m_btnAxisOrigin[DlgID2AxisID(23)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_24,	m_btnAxisOrigin[DlgID2AxisID(24)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_25,	m_btnAxisOrigin[DlgID2AxisID(25)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_26,	m_btnAxisOrigin[DlgID2AxisID(26)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_27,	m_btnAxisOrigin[DlgID2AxisID(27)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_28,	m_btnAxisOrigin[DlgID2AxisID(28)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_29,	m_btnAxisOrigin[DlgID2AxisID(29)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_30,	m_btnAxisOrigin[DlgID2AxisID(30)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_31,	m_btnAxisOrigin[DlgID2AxisID(31)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_32,	m_btnAxisOrigin[DlgID2AxisID(32)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_33,	m_btnAxisOrigin[DlgID2AxisID(33)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_34,	m_btnAxisOrigin[DlgID2AxisID(34)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_35,	m_btnAxisOrigin[DlgID2AxisID(35)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_36,	m_btnAxisOrigin[DlgID2AxisID(36)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_37,	m_btnAxisOrigin[DlgID2AxisID(37)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_38,	m_btnAxisOrigin[DlgID2AxisID(38)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_39,	m_btnAxisOrigin[DlgID2AxisID(39)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_40,	m_btnAxisOrigin[DlgID2AxisID(40)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_41,	m_btnAxisOrigin[DlgID2AxisID(41)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_42,	m_btnAxisOrigin[DlgID2AxisID(42)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_43,	m_btnAxisOrigin[DlgID2AxisID(43)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_44,	m_btnAxisOrigin[DlgID2AxisID(44)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_45,	m_btnAxisOrigin[DlgID2AxisID(45)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_46,	m_btnAxisOrigin[DlgID2AxisID(46)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_47,	m_btnAxisOrigin[DlgID2AxisID(47)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_48,	m_btnAxisOrigin[DlgID2AxisID(48)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_49,	m_btnAxisOrigin[DlgID2AxisID(49)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_50,	m_btnAxisOrigin[DlgID2AxisID(50)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_51,	m_btnAxisOrigin[DlgID2AxisID(51)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_52,	m_btnAxisOrigin[DlgID2AxisID(52)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_53,	m_btnAxisOrigin[DlgID2AxisID(53)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_54,	m_btnAxisOrigin[DlgID2AxisID(54)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_55,	m_btnAxisOrigin[DlgID2AxisID(55)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_56,	m_btnAxisOrigin[DlgID2AxisID(56)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_57,	m_btnAxisOrigin[DlgID2AxisID(57)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_58,	m_btnAxisOrigin[DlgID2AxisID(58)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_59,	m_btnAxisOrigin[DlgID2AxisID(59)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_60,	m_btnAxisOrigin[DlgID2AxisID(60)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_61,	m_btnAxisOrigin[DlgID2AxisID(61)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_62,	m_btnAxisOrigin[DlgID2AxisID(62)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_63,	m_btnAxisOrigin[DlgID2AxisID(63)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_64,	m_btnAxisOrigin[DlgID2AxisID(64)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_65,	m_btnAxisOrigin[DlgID2AxisID(65)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_66,	m_btnAxisOrigin[DlgID2AxisID(66)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_67,	m_btnAxisOrigin[DlgID2AxisID(67)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_68,	m_btnAxisOrigin[DlgID2AxisID(68)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_69,	m_btnAxisOrigin[DlgID2AxisID(69)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_70,	m_btnAxisOrigin[DlgID2AxisID(70)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_69,	m_btnAxisOrigin[DlgID2AxisID(69)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_70,	m_btnAxisOrigin[DlgID2AxisID(70)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_71,	m_btnAxisOrigin[DlgID2AxisID(71)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_72,	m_btnAxisOrigin[DlgID2AxisID(72)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_73,	m_btnAxisOrigin[DlgID2AxisID(73)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_74,	m_btnAxisOrigin[DlgID2AxisID(74)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_75,	m_btnAxisOrigin[DlgID2AxisID(75)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_76,	m_btnAxisOrigin[DlgID2AxisID(76)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_77,	m_btnAxisOrigin[DlgID2AxisID(77)]);

	DDX_Control(pDX, IDB_AXIS_ORIGIN_78,	m_btnAxisOrigin[DlgID2AxisID(78)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_79,	m_btnAxisOrigin[DlgID2AxisID(79)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_80,	m_btnAxisOrigin[DlgID2AxisID(80)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_81,	m_btnAxisOrigin[DlgID2AxisID(81)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_82,	m_btnAxisOrigin[DlgID2AxisID(82)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_83,	m_btnAxisOrigin[DlgID2AxisID(83)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_84,	m_btnAxisOrigin[DlgID2AxisID(84)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_85,	m_btnAxisOrigin[DlgID2AxisID(85)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_86,	m_btnAxisOrigin[DlgID2AxisID(86)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_87,	m_btnAxisOrigin[DlgID2AxisID(87)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_88,	m_btnAxisOrigin[DlgID2AxisID(88)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_89,	m_btnAxisOrigin[DlgID2AxisID(89)]);

	DDX_Control(pDX, IDB_AXIS_ORIGIN_90,	m_btnAxisOrigin[DlgID2AxisID(90)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_91,	m_btnAxisOrigin[DlgID2AxisID(91)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_92,	m_btnAxisOrigin[DlgID2AxisID(92)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_93,	m_btnAxisOrigin[DlgID2AxisID(93)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_94,	m_btnAxisOrigin[DlgID2AxisID(94)]);
	DDX_Control(pDX, IDB_AXIS_ORIGIN_95,	m_btnAxisOrigin[DlgID2AxisID(95)]);

	DDX_Control(pDX, IDB_ALL_SELECT, m_btnAllSelect);
	DDX_Control(pDX, IDB_ALL_CANCEL, m_btnAllUnselect);
	DDX_Control(pDX, IDB_SERVO_ON, m_btnServoOn);
	DDX_Control(pDX, IDB_SERVO_OFF, m_btnServoOff);
	DDX_Control(pDX, IDB_INIT_RUN, m_btnReturnOrigin);
	DDX_Control(pDX, IDB_EXIT, m_btnExit);
}


BEGIN_MESSAGE_MAP(CAutoMotorOriginDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoMotorOriginDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoMotorOriginDlg message handlers

void CAutoMotorOriginDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CAutoMotorOriginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// E-Stop 후 Safety Relay Manual Reset 반드시 해야 함.
	// 090609 Suwon Reset S/W로 대체
	/*
	MOlbSystemPre.GetIOComponent()->OutputOn(OUT_POWER_RESET);
	Sleep(200);
	MOlbSystemPre.GetIOComponent()->OutputOff(OUT_POWER_RESET);
	*/
	// selsk_jdy
	int i = 0;
	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		m_bAxisSelectFlag[i] = FALSE;
	}

	int iSpace = 0;
	CString strAxisName;
	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		strAxisName = m_plnkAxis[i]->GetAxisName();
		iSpace = strAxisName.Find(' ', 0);
		if (iSpace > 2 && strAxisName.GetLength() > 10)
		{
			strAxisName.Insert(iSpace, '\r');
			strAxisName.SetAt(iSpace+1, '\n');
		}
		m_btnAxisOrigin[i].SetCaption(strAxisName);	//m_plnkAxis[i]->GetAxisName());
	}

	for ( ; i < DEF_MOTOR_ORIGIN_DLG_MAX_BUTTON; i++)
	{
		m_btnAxisOrigin[i].EnableWindow(FALSE);
		m_btnAxisOrigin[i].ShowWindow(SW_HIDE);
	}

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
		{
			m_btnAxisOrigin[i].EnableWindow(FALSE);
			m_bAxisOriginFlag[i] = FALSE;
			continue;
		}
		
		if (TRUE == m_plnkAxis[i]->IsAmpFault() || FALSE == m_plnkAxis[i]->IsAmpEnable())
			m_plnkAxis[i]->ResetOrigin();

		m_bAxisOriginFlag[i] = m_plnkAxis[i]->IsOriginReturn();
		m_btnAxisOrigin[i].SetValue(m_bAxisOriginFlag[i]);
	}

	m_uiTimerID = SetTimer(1, 300, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;

	ReadDisplayMessage(MAIN_AUTO, _T("AutoMotorOriginDlg_1"), &szTextTemp);
//축 원점 복귀
	SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMotorOriginDlg_2"), &szTextTemp);
//전부 선택
	GetDlgItem(IDB_ALL_SELECT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMotorOriginDlg_3"), &szTextTemp);
//전부 취소
	GetDlgItem(IDB_ALL_CANCEL)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMotorOriginDlg_4"), &szTextTemp);
//강제 원점복귀
	GetDlgItem(IDB_SET_FORCE_ORIGIN_RETURN)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMotorOriginDlg_5"), &szTextTemp);
//서보 ON
	GetDlgItem(IDB_SERVO_ON)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMotorOriginDlg_6"), &szTextTemp);
//서보 OFF
	GetDlgItem(IDB_SERVO_OFF)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMotorOriginDlg_7"), &szTextTemp);
//원점복귀 실행
	GetDlgItem(IDB_INIT_RUN)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_AUTO, _T("AutoMotorOriginDlg_8"), &szTextTemp);
//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
	//170713 JSH
	szTextTemp.Format(_T("NMC\nInitalize"));
	GetDlgItem(IDB_SERCOS_NET_INIT)->SetWindowText(szTextTemp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CAutoMotorOriginDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAutoMotorOriginDlg)
	ON_EVENT(CAutoMotorOriginDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_INIT_RUN, -600 /* Click */, OnInitRun, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_SERVO_ON, -600 /* Click */, OnServoOn, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_SERVO_OFF, -600 /* Click */, OnServoOff, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_ALL_SELECT, -600 /* Click */, OnAllSelect, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_ALL_CANCEL, -600 /* Click */, OnAllCancel, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_0, -600 /* Click */, OnClickAxisOrigin0, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_1, -600 /* Click */, OnClickAxisOrigin1, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_2, -600 /* Click */, OnClickAxisOrigin2, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_3, -600 /* Click */, OnClickAxisOrigin3, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_4, -600 /* Click */, OnClickAxisOrigin4, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_5, -600 /* Click */, OnClickAxisOrigin5, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_6, -600 /* Click */, OnClickAxisOrigin6, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_7, -600 /* Click */, OnClickAxisOrigin7, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_8, -600 /* Click */, OnClickAxisOrigin8, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_9, -600 /* Click */, OnClickAxisOrigin9, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_10, -600 /* Click */, OnClickAxisOrigin10, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_11, -600 /* Click */, OnClickAxisOrigin11, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_12, -600 /* Click */, OnClickAxisOrigin12, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_13, -600 /* Click */, OnClickAxisOrigin13, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_14, -600 /* Click */, OnClickAxisOrigin14, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_15, -600 /* Click */, OnClickAxisOrigin15, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_16, -600 /* Click */, OnClickAxisOrigin16, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_17, -600 /* Click */, OnClickAxisOrigin17, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_18, -600 /* Click */, OnClickAxisOrigin18, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_19, -600 /* Click */, OnClickAxisOrigin19, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_20, -600 /* Click */, OnClickAxisOrigin20, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_21, -600 /* Click */, OnClickAxisOrigin21, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_22, -600 /* Click */, OnClickAxisOrigin22, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_23, -600 /* Click */, OnClickAxisOrigin23, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_24, -600 /* Click */, OnClickAxisOrigin24, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_25, -600 /* Click */, OnClickAxisOrigin25, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_26, -600 /* Click */, OnClickAxisOrigin26, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_27, -600 /* Click */, OnClickAxisOrigin27, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_28, -600 /* Click */, OnClickAxisOrigin28, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_29, -600 /* Click */, OnClickAxisOrigin29, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_30, -600 /* Click */, OnClickAxisOrigin30, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_31, -600 /* Click */, OnClickAxisOrigin31, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_32, -600 /* Click */, OnClickAxisOrigin32, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_33, -600 /* Click */, OnClickAxisOrigin33, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_34, -600 /* Click */, OnClickAxisOrigin34, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_35, -600 /* Click */, OnClickAxisOrigin35, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_36, -600 /* Click */, OnClickAxisOrigin36, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_37, -600 /* Click */, OnClickAxisOrigin37, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_38, -600 /* Click */, OnClickAxisOrigin38, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_39, -600 /* Click */, OnClickAxisOrigin39, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_40, -600 /* Click */, OnClickAxisOrigin40, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_41, -600 /* Click */, OnClickAxisOrigin41, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_42, -600 /* Click */, OnClickAxisOrigin42, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_43, -600 /* Click */, OnClickAxisOrigin43, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_44, -600 /* Click */, OnClickAxisOrigin44, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_45, -600 /* Click */, OnClickAxisOrigin45, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_46, -600 /* Click */, OnClickAxisOrigin46, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_47, -600 /* Click */, OnClickAxisOrigin47, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_48, -600 /* Click */, OnClickAxisOrigin48, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_49, -600 /* Click */, OnClickAxisOrigin49, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_50, -600 /* Click */, OnClickAxisOrigin50, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_51, -600 /* Click */, OnClickAxisOrigin51, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_52, -600 /* Click */, OnClickAxisOrigin52, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_53, -600 /* Click */, OnClickAxisOrigin53, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_54, -600 /* Click */, OnClickAxisOrigin54, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_55, -600 /* Click */, OnClickAxisOrigin55, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_56, -600 /* Click */, OnClickAxisOrigin56, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_57, -600 /* Click */, OnClickAxisOrigin57, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_58, -600 /* Click */, OnClickAxisOrigin58, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_59, -600 /* Click */, OnClickAxisOrigin59, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_60, -600 /* Click */, OnClickAxisOrigin60, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_61, -600 /* Click */, OnClickAxisOrigin61, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_62, -600 /* Click */, OnClickAxisOrigin62, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_63, -600 /* Click */, OnClickAxisOrigin63, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_64, -600 /* Click */, OnClickAxisOrigin64, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_65, -600 /* Click */, OnClickAxisOrigin65, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_66, -600 /* Click */, OnClickAxisOrigin66, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_67, -600 /* Click */, OnClickAxisOrigin67, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_68, -600 /* Click */, OnClickAxisOrigin68, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_69, -600 /* Click */, OnClickAxisOrigin69, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_70, -600 /* Click */, OnClickAxisOrigin70, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_71, -600 /* Click */, OnClickAxisOrigin71, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_72, -600 /* Click */, OnClickAxisOrigin72, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_73, -600 /* Click */, OnClickAxisOrigin73, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_74, -600 /* Click */, OnClickAxisOrigin74, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_75, -600 /* Click */, OnClickAxisOrigin75, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_SET_FORCE_ORIGIN_RETURN, -600 /* Click */, OnForceOriginReturn, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_SERCOS_NET_INIT, -600 /* Click */, OnSERCOSnetInit, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_76, -600 /* Click */, OnClickAxisOrigin76, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_77, -600 /* Click */, OnClickAxisOrigin77, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_78, -600 /* Click */, OnClickAxisOrigin78, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_79, -600 /* Click */, OnClickAxisOrigin79, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_80, -600 /* Click */, OnClickAxisOrigin80, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_81, -600 /* Click */, OnClickAxisOrigin81, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_82, -600 /* Click */, OnClickAxisOrigin82, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_83, -600 /* Click */, OnClickAxisOrigin83, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_84, -600 /* Click */, OnClickAxisOrigin84, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_85, -600 /* Click */, OnClickAxisOrigin85, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_86, -600 /* Click */, OnClickAxisOrigin86, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_87, -600 /* Click */, OnClickAxisOrigin87, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_88, -600 /* Click */, OnClickAxisOrigin88, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_89, -600 /* Click */, OnClickAxisOrigin89, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_90, -600 /* Click */, OnClickAxisOrigin90, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_91, -600 /* Click */, OnClickAxisOrigin91, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_92, -600 /* Click */, OnClickAxisOrigin92, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_93, -600 /* Click */, OnClickAxisOrigin93, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_94, -600 /* Click */, OnClickAxisOrigin94, VTS_NONE)
	ON_EVENT(CAutoMotorOriginDlg, IDB_AXIS_ORIGIN_95, -600 /* Click */, OnClickAxisOrigin95, VTS_NONE)
	//}}AFX_EVENTSINK_MAP

END_EVENTSINK_MAP()

void CAutoMotorOriginDlg::OnExit() 
{
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

// sesl_jdy	m_plnkSercosSetup->SetOriginReturn(FALSE);

	CDialog::OnCancel();
}

void CAutoMotorOriginDlg::OnInitRun()
{
	// Doolin kwangilshin 2017. 09. 06.
	//
#ifndef DEF_SETUP 
	if (m_plnkSystemData->m_bSafetySensor)
	{
		if (!MOlbSystemPre.IsAllDoorLock(DEF_FRONT_GROUP) || !MOlbSystemPre.IsAllDoorLock(DEF_REAR_GROUP))
		{
			MyMessageBox(MY_MESSAGE_BOX, 20218, _T("Door Open"), M_ICONERROR);
			return;
		}
	}

	//20171004 KDW Start
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen_AOC())
	{
		int iResult = 300013000;
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("AOC Door Open Error"), M_ICONINFORMATION);
		return;
	}
	//20171004 KDW End

#endif
	// End

	if(MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen() == TRUE)
	{
		MyMessageBox(MY_MESSAGE_BOX, 20218, _T("Door Open"), M_ICONERROR);
		return;
	}

	// E-Stop 후 Safety Relay Manual Reset 반드시 해야 함.
	MOlbSystemPre.GetIOComponent()->OutputOn(OUT_POWER_RESET);
	Sleep(200);
	MOlbSystemPre.GetIOComponent()->OutputOff(OUT_POWER_RESET);

	CString strMsg;
	BOOL bSelectFlag = FALSE;

	// selsk_jdy
	int i = 0;
	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		bSelectFlag |= m_bAxisSelectFlag[i];
		//20131220 SJ_HJG
		if(i == DEF_AXIS_SERCOS_TABIC_CARRIER1_T ||
			i == DEF_AXIS_SERCOS_TABIC_CARRIER2_T ||
			//170723_KDH Set Currnet 함수를 이용하는 축은 무조건 강제원점
			i == DEF_AXIS_SERCOS_INSPECT_EXPAND1 ||
			i == DEF_AXIS_SERCOS_INSPECT_EXPAND2 ||
			i == DEF_AXIS_SERCOS_TABCAMERA_EXPAND ||
			i == DEF_AXIS_SERCOS_TABCAMERA_EXPAND2 ||
#ifdef DEF_GATE_SYSTEM
			i == DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T ||
			i == DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T ||
			i == DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T ||
			i == DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T ||
			i == DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND ||
			i == DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2 ||
			i == DEF_AXIS_SERCOS_R_INSPECT_EXPAND1 ||
			i == DEF_AXIS_SERCOS_R_INSPECT_EXPAND2 ||
#else
			i == DEF_AXIS_SERCOS_TABIC_CARRIER3_T ||
			i == DEF_AXIS_SERCOS_TABIC_CARRIER4_T ||
			i == DEF_AXIS_SERCOS_INSPECT_EXPAND3 ||
			i == DEF_AXIS_SERCOS_INSPECT_EXPAND4 
#endif
			//_________________
			)
		{
			if(m_bAxisSelectFlag[i] == TRUE)
			{
				m_rgbForceOriginReturn[i] = TRUE;
				m_btnAxisOrigin[i].SetBackColor(RED);
			}
		}
		//_______________
	}

	if (bSelectFlag == FALSE)
	{
//	선택한 축이 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20200, _T("Origin Return Error"), M_ICONERROR);
		return;
	}

//	선택한 축을 원점복귀 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 20212, _T("Origin Return"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	



	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		m_btnAxisOrigin[i].EnableWindow(FALSE);		
	}
	m_btnAllSelect.EnableWindow(FALSE);
	m_btnAllUnselect.EnableWindow(FALSE);
	m_btnServoOn.EnableWindow(FALSE);
	m_btnServoOff.EnableWindow(FALSE);
	m_btnReturnOrigin.EnableWindow(FALSE);
	m_btnExit.EnableWindow(FALSE);
	GetDlgItem(IDB_SERCOS_NET_INIT)->EnableWindow(FALSE);
	m_btnForceOrigin.EnableWindow(FALSE);
	
	int iResult = ReturnOrigin();	// 원점복귀 동작

	m_btnAllSelect.EnableWindow(TRUE);
	m_btnAllUnselect.EnableWindow(TRUE);
	m_btnServoOn.EnableWindow(TRUE);
	m_btnServoOff.EnableWindow(TRUE);
	m_btnReturnOrigin.EnableWindow(TRUE);
	m_btnExit.EnableWindow(TRUE);
	GetDlgItem(IDB_SERCOS_NET_INIT)->EnableWindow(TRUE);
	m_btnForceOrigin.EnableWindow(TRUE);
	
	switch (iResult)
	{
	case ERR_DLG_ORIGIN_SUCCESS:
		{
			CMyProgressWnd ProgWnd(NULL, _T("Processing After Origin ..."));
			ProgWnd.SetRange(0, DEF_AXIS_MAX_AXIS);
			ProgWnd.SetText(_T("Processing After Origin Return...\n")
				_T("Wait a moment..."));
			
			for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
			{
				ProgWnd.StepIt();
				ProgWnd.PeekAndPump();
				
				if (FALSE == m_bAxisSelectFlag[i])
					continue;
				
		#ifndef SIMULATION
				iResult = checkAfterOrigin(i);
				//	원점복귀가 실패했습니다.
				//170720_KDH 에러코드 추가
				if(iResult)
				{
					ReadDisplayMessage(MY_MESSAGE_BOX,_T("Message_20209"), &strMsg);
					strMsg += _T("\n\n") + MOlbSystemPre.GetErrorMessage(iResult);
					MyMessageBox(strMsg, _T("Origin Return Fail"), M_ICONERROR);
				}
				//______________
				iResult = 0;
		#endif
				/* sesl_jdy
				#ifndef SIMULATION
				if (m_bAxisSelectFlag[i] == TRUE)
				{
				iResult = checkAfterOrigin(i);
				}
				#endif
				*/
			}
			ProgWnd.DestroyWindow();
		}
//	원점복귀가 완료되었습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20201, _T("Origin Success"), M_ICONINFORMATION);
		break;
		
	case ERR_DLG_ORIGIN_DETECT_E_STOP:
//	원점 복귀 중 비상정지에 의해 정지 되었습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20202, _T("Origin Return Fail"), M_ICONERROR);
		break;
	case ERR_DLG_ORIGIN_DETECT_USER_STOP:
//	원점 복귀 중 사용자에 의해 정지 되었습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20203, _T("Origin Return Fail"), M_ICONERROR);
		break;
		
	case ERR_DLG_ORIGIN_DETECT_AMP_FAULT:
//	원점 복귀 중 AmpFault 상태가 되었습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20204, _T("Origin Return Fail"), M_ICONERROR);
		break;
		
	case ERR_DLG_ORIGIN_DETECT_EQ_IF_SIGNAL:
//	원점 복귀 중 설비 I/F 신호에 의해 정지 되었습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20205, _T("Origin Return Fail"), M_ICONERROR);
		break;

	case ERR_DLG_ORIGIN_LIMIT_TIME_OVER:
//	원점 복귀 중 제한 시간 초과 발생.
		MyMessageBox(MY_MESSAGE_BOX, 20206, _T("Origin Return Fail"), M_ICONERROR);
		break;

	case ERR_DLG_ORIGIN_SERCOS_PHASE_ERROR:
//	Sercos Phase 4 아님.
		MyMessageBox(MY_MESSAGE_BOX, 20207, _T("Origin Return Fail"), M_ICONERROR);
		break;

	case ERR_DLG_ORIGIN_INTERLOCK:
		// Error Message 는 앞에서 표시 완료.
		break;

	// sesl_jdy
	case ERR_DLG_ORIGIN_PC_ERROR:
//	원점 복귀 Thread 이상 발생.
		MyMessageBox(MY_MESSAGE_BOX, 20208, _T("Origin Return Fail"), M_ICONERROR);
		break;
		
	default:
//	원점복귀가 실패했습니다.
	    ReadDisplayMessage(MY_MESSAGE_BOX,_T("Message_20209"), &strMsg);
		strMsg += _T("\n\n") + MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strMsg, _T("Origin Return Fail"), M_ICONERROR);
		break;
	}
	
	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
			continue;

		m_bAxisSelectFlag[i] = FALSE;
		m_btnAxisOrigin[i].EnableWindow(TRUE);
		m_btnAxisOrigin[i].SetPicture(m_strOffOriginIcon);

		// sesl_jdy
		m_rgbForceOriginReturn[i] = FALSE;
		m_btnAxisOrigin[i].SetBackColor(DKGRAY);
	}

	// sesl_jdy
	changeDisplayForceOriginButton(FALSE);
}

void CAutoMotorOriginDlg::OnServoOn()
{
	int i = 0;

	// E-Stop 후 Safety Relay Manual Reset 반드시 해야 함.
	MOlbSystemPre.GetIOComponent()->OutputOn(OUT_POWER_RESET);
	Sleep(200);
	MOlbSystemPre.GetIOComponent()->OutputOff(OUT_POWER_RESET);

	clearAxisForIAMNode();
	
	BOOL bSelectFlag = FALSE;
	CString strMsg;

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		bSelectFlag |= m_bAxisSelectFlag[i];

	if (bSelectFlag == FALSE)
	{
//	선택한 축이 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20210, _T("Origin Return Fail"), M_ICONERROR);
		return;
	}

//	선택한 축의 Servo를 On 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 20211, _T("Servo On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Servo On..."));
	ProgWnd.SetRange(0, DEF_AXIS_MAX_AXIS);
	ProgWnd.SetText(_T("Servo On...\nWait a moment.."));

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();

		// Doolin kwangilshin 2017. 08. 23.
		//
		if (MOlbSystemPre.IsSkipAxis(i) || !m_bAxisSelectFlag[i])
		{
			continue;
		}
		
		int iResult = m_plnkAxis[i]->ServoOn(NULL, TRUE);
		
		if (iResult != ERR_DLG_ORIGIN_SUCCESS)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr, _T("Servo On Error"), M_ICONERROR,_T("Confirm"),_T("Cancel"));
		}
		// End.
	}
	ProgWnd.DestroyWindow();
}

void CAutoMotorOriginDlg::OnServoOff() 
{
	BOOL bSelectFlag = FALSE;
	CString strMsg;

	clearAxisForIAMNode();

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		bSelectFlag |= m_bAxisSelectFlag[i];

	if (bSelectFlag == FALSE)
	{
//	선택한 축이 없습니다.
		MyMessageBox(MY_MESSAGE_BOX, 20213, _T("Origin Return Error"), M_ICONERROR);
		return;
	}

//	선택한 축의 Servo를 Off 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 20214, _T("Servo Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Servo Off..."));
	ProgWnd.SetRange(0, DEF_AXIS_MAX_AXIS);
	ProgWnd.SetText(_T("Servo Off...\n")
					    _T("Wait a moment..."));

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		ProgWnd.StepIt();
		ProgWnd.PeekAndPump();

		if (m_bAxisSelectFlag[i] == TRUE)
		{
			if (TRUE == MOlbSystemPre.IsSkipAxis(i))
				continue;

			if (DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T == i)
				MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetHomeFlagSprocket1(FALSE);
			if (DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T == i)
				MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetHomeFlagSprocket2(FALSE);
			if (DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T == i)
				MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetHomeFlagSprocket1(FALSE);
			if (DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T == i)
				MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetHomeFlagSprocket2(FALSE);
			
			int iResult = m_plnkAxis[i]->ServoOff(NULL, TRUE);

			if (iResult != ERR_DLG_ORIGIN_SUCCESS)
			{
				CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
				MyMessageBox(strErr, _T("Servo Off Error"), M_ICONERROR,_T("Confirm"),_T("Cancel"));
				return;
			}

			m_btnAxisOrigin[i].SetValue(FALSE);
		}
	}
	ProgWnd.DestroyWindow();
}

void CAutoMotorOriginDlg::OnAllSelect()
{
	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
			continue;
		
		m_bAxisSelectFlag[i] = TRUE;
		m_btnAxisOrigin[i].SetPicture(m_strOnOriginIcon);
	}
}

void CAutoMotorOriginDlg::OnAllCancel() 
{
	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		m_bAxisSelectFlag[i] = FALSE;
		m_btnAxisOrigin[i].SetPicture(m_strOffOriginIcon);
	}
}

void CAutoMotorOriginDlg::setAxisSelectFlag(int iSel, BOOL bIsSelected)
{
	BOOL bOriginState = m_plnkAxis[iSel]->IsOriginReturn();

	m_bAxisSelectFlag[iSel] = bIsSelected;
	m_btnAxisOrigin[iSel].SetValue(bOriginState);

	if (bIsSelected)
	{
		m_btnAxisOrigin[iSel].SetPicture(m_strOnOriginIcon);	
	}
	else
	{
		m_btnAxisOrigin[iSel].SetPicture(m_strOffOriginIcon);
	}
}

void CAutoMotorOriginDlg::axisSelect(int iSel)
{
	BOOL bIsSelFlg = !m_bAxisSelectFlag[iSel];

	switch (iSel)
	{
		case DEF_AXIS_SERCOS_TABIC_CARRIER1_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER1_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER1_Y, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER1_T, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_TABIC_CARRIER2_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER2_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER2_Y, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER2_T, bIsSelFlg);
			break;

			// Doolin kwangilshin 2017. 08. 23.
			//
		case DEF_AXIS_SERCOS_TABIC_CARRIER1_T:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER1_T, bIsSelFlg);
			if (!m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER1_X])
			{
				setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER1_Y, bIsSelFlg);
			}
			break;
		case DEF_AXIS_SERCOS_TABIC_CARRIER2_T:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER2_T, bIsSelFlg);
			if (!m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER2_X])
			{
				setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER2_Y, bIsSelFlg);
			}
			break;
			// End.
#ifdef DEF_SOURCE_SYSTEM
		case DEF_AXIS_SERCOS_TABIC_CARRIER3_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER3_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER3_Y, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER3_T, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_TABIC_CARRIER4_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER4_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER4_Y, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER4_T, bIsSelFlg);
			break;
			
			// Doolin kwangilshin 2017. 08. 23.
			//
		case DEF_AXIS_SERCOS_TABIC_CARRIER3_T:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER3_T, bIsSelFlg);
			if (!m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER3_X])
			{
				setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER3_Y, bIsSelFlg);
			}
			break;
		case DEF_AXIS_SERCOS_TABIC_CARRIER4_T:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER4_T, bIsSelFlg);
			if (!m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER4_X])
			{
				setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_CARRIER4_Y, bIsSelFlg);
			}
			break;
			// End.
#endif

		case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T, bIsSelFlg);
			break;
			
			// Doolin kwangilshin 2017. 08. 23.
			//
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T, bIsSelFlg);
			if (!m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X])
			{
				setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y, bIsSelFlg);
			}
			break;
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T, bIsSelFlg);
			if (!m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X])
			{
				setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y, bIsSelFlg);
			}
			break;
			// End
#ifdef DEF_SOURCE_SYSTEM
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T,bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T, bIsSelFlg);
			break;
			
			// Doolin kwangilshin 2017. 08. 23.
			//
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T, bIsSelFlg);
			if (!m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X])
			{
				setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y, bIsSelFlg);
			}
			break;
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T, bIsSelFlg);
			if (!m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X])
			{
				setAxisSelectFlag(DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y, bIsSelFlg);
			}
			break;
			// End
#endif

		//Tab Mounter Y <-> Z		
		case DEF_AXIS_SERCOS_MOUNTER1_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_MOUNTER1_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MOUNTER1_Z, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_MOUNTER2_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_MOUNTER2_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MOUNTER2_Z, bIsSelFlg);
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_AXIS_SERCOS_MOUNTER3_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_MOUNTER3_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MOUNTER3_Z, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_MOUNTER4_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_MOUNTER4_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MOUNTER4_Z, bIsSelFlg);
			break;
#endif
		case DEF_AXIS_SERCOS_R_MOUNTER1_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_MOUNTER1_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_MOUNTER1_Z, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_R_MOUNTER2_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_MOUNTER2_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_MOUNTER2_Z, bIsSelFlg);
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_AXIS_SERCOS_R_MOUNTER3_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_MOUNTER3_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_MOUNTER3_Z, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_R_MOUNTER4_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_MOUNTER4_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_MOUNTER4_Z, bIsSelFlg);
			break;
#endif
		case DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_INSPECT_EXPAND1, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_BACKUP1_Z, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MODEL_CHANGE_1, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_INSPECT_EXPAND2, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_BACKUP2_Z, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MODEL_CHANGE_1, bIsSelFlg);
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_AXIS_SERCOS_INSPECTION_CAMERA3_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_INSPECTION_CAMERA3_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_INSPECT_EXPAND3, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_BACKUP3_Z, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MODEL_CHANGE_1, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_INSPECT_EXPAND4, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_BACKUP4_Z, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MODEL_CHANGE_1, bIsSelFlg);
			break;
#endif
		case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MODEL_CHANGE_2, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MODEL_CHANGE_2, bIsSelFlg);
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA3_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_INSPECTION_CAMERA3_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MODEL_CHANGE_2, bIsSelFlg);
			break;
		case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X:
			setAxisSelectFlag(DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_MODEL_CHANGE_2, bIsSelFlg);
			break;
			//170929 KDW Start
#ifdef DEF_1ST_EQ
		case DEF_AXIS_SERCOS_TRANSFER_CENTER_1:
		case DEF_AXIS_SERCOS_TRANSFER_CENTER_2:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TRANSFER_CENTER_1, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TRANSFER_CENTER_2, bIsSelFlg);
			break;

		case DEF_AXIS_SERCOS_TRANSFER_OUT:
		case DEF_AXIS_SERCOS_TRANSFER_OUT_2:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TRANSFER_OUT, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TRANSFER_OUT_2, bIsSelFlg);
			break;
#endif
		case DEF_AXIS_SERCOS_TRANSFER_IN:
		case DEF_AXIS_SERCOS_TRANSFER_IN_2:
			setAxisSelectFlag(DEF_AXIS_SERCOS_TRANSFER_IN, bIsSelFlg);
			setAxisSelectFlag(DEF_AXIS_SERCOS_TRANSFER_IN_2, bIsSelFlg);
			break;
#endif
			//170929 KDW End
		default:
			setAxisSelectFlag(iSel, bIsSelFlg);
			break;
	}
}

void CAutoMotorOriginDlg::OnClickAxisOrigin0()
{
	axisSelect(DlgID2AxisID(0));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin1() 
{
	axisSelect(DlgID2AxisID(1));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin2() 
{
	axisSelect(DlgID2AxisID(2));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin3() 
{
	axisSelect(DlgID2AxisID(3));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin4() 
{
	axisSelect(DlgID2AxisID(4));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin5() 
{
	axisSelect(DlgID2AxisID(5));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin6() 
{
	axisSelect(DlgID2AxisID(6));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin7() 
{
	axisSelect(DlgID2AxisID(7));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin8() 
{
	axisSelect(DlgID2AxisID(8));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin9() 
{
	axisSelect(DlgID2AxisID(9));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin10() 
{
	axisSelect(DlgID2AxisID(10));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin11() 
{
	axisSelect(DlgID2AxisID(11));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin12() 
{
	axisSelect(DlgID2AxisID(12));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin13() 
{
	axisSelect(DlgID2AxisID(13));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin14() 
{
	axisSelect(DlgID2AxisID(14));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin15() 
{
	axisSelect(DlgID2AxisID(15));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin16() 
{
	axisSelect(DlgID2AxisID(16));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin17() 
{
	axisSelect(DlgID2AxisID(17));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin18() 
{
	axisSelect(DlgID2AxisID(18));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin19() 
{
	axisSelect(DlgID2AxisID(19));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin20() 
{
	axisSelect(DlgID2AxisID(20));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin21() 
{
	axisSelect(DlgID2AxisID(21));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin22() 
{
	axisSelect(DlgID2AxisID(22));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin23() 
{
	axisSelect(DlgID2AxisID(23));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin24() 
{
	axisSelect(DlgID2AxisID(24));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin25() 
{
	axisSelect(DlgID2AxisID(25));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin26() 
{
	axisSelect(DlgID2AxisID(26));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin27() 
{
	axisSelect(DlgID2AxisID(27));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin28() 
{
	axisSelect(DlgID2AxisID(28));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin29() 
{
	axisSelect(DlgID2AxisID(29));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin30() 
{
	axisSelect(DlgID2AxisID(30));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin31() 
{
	axisSelect(DlgID2AxisID(31));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin32() 
{
	axisSelect(DlgID2AxisID(32));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin33() 
{
	axisSelect(DlgID2AxisID(33));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin34() 
{
	axisSelect(DlgID2AxisID(34));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin35() 
{
	axisSelect(DlgID2AxisID(35));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin36() 
{
	axisSelect(DlgID2AxisID(36));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin37() 
{
	axisSelect(DlgID2AxisID(37));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin38() 
{
	axisSelect(DlgID2AxisID(38));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin39() 
{
	axisSelect(DlgID2AxisID(39));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin40() 
{
	axisSelect(DlgID2AxisID(40));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin41() 
{
	axisSelect(DlgID2AxisID(41));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin42() 
{
	axisSelect(DlgID2AxisID(42));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin43() 
{
	axisSelect(DlgID2AxisID(43));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin44() 
{
	axisSelect(DlgID2AxisID(44));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin45() 
{
	axisSelect(DlgID2AxisID(45));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin46() 
{
	axisSelect(DlgID2AxisID(46));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin47() 
{
	axisSelect(DlgID2AxisID(47));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin48() 
{
	axisSelect(DlgID2AxisID(48));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin49() 
{
	axisSelect(DlgID2AxisID(49));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin50() 
{
	axisSelect(DlgID2AxisID(50));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin51() 
{
	axisSelect(DlgID2AxisID(51));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin52() 
{
	axisSelect(DlgID2AxisID(52));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin53() 
{
	axisSelect(DlgID2AxisID(53));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin54() 
{
	axisSelect(DlgID2AxisID(54));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin55() 
{
	axisSelect(DlgID2AxisID(55));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin56() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(56));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin57() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(57));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin58() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(58));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin59() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(59));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin60() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(60));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin61() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(61));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin62() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(62));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin63() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(63));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin64() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(64));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin65() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(65));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin66() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(66));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin67() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(67));	
}

void CAutoMotorOriginDlg::OnClickAxisOrigin68() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(68));		
}

void CAutoMotorOriginDlg::OnClickAxisOrigin69() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(69));		
}

void CAutoMotorOriginDlg::OnClickAxisOrigin70() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(70));		
}

void CAutoMotorOriginDlg::OnClickAxisOrigin71() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(71));		
}

void CAutoMotorOriginDlg::OnClickAxisOrigin72() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(72));		
}

void CAutoMotorOriginDlg::OnClickAxisOrigin73() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(73));		
}

void CAutoMotorOriginDlg::OnClickAxisOrigin74() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(74));		
}

void CAutoMotorOriginDlg::OnClickAxisOrigin75() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(75));	
}

void CAutoMotorOriginDlg::OnClickAxisOrigin76() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(76));	
}

void CAutoMotorOriginDlg::OnClickAxisOrigin77() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(77));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin78() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(78));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin79() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(79));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin80() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(80));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin81() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(81));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin82() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(82));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin83() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(83));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin84() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(84));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin85() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(85));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin86() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(86));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin87() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(87));
}

void CAutoMotorOriginDlg::OnTimer(UINT nIDEvent) 
{
	//090609 Suwon 화면 상태 표시
	BOOL bOriginFlag	= FALSE;
	BOOL bAmpFaultFlag	= FALSE;
	BOOL bAmpEnableFlag	= FALSE;

	if (m_uiTimerID == nIDEvent)
	{
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			if (TRUE == MOlbSystemPre.IsSkipAxis(i))
				continue;
#ifndef SIMULATION
			bAmpFaultFlag	= m_plnkAxis[i]->IsAmpFault() || m_plnkAxis[i]->IsEStopEvent();
			// Doolin kwangilshin 2017. 08. 23.
			//
			bAmpEnableFlag	= m_plnkAxis[i]->IsAmpEnable();
			//
			// End.
#endif
			if (bAmpFaultFlag)
			{
				m_bAxisOriginFlag[i] = FALSE;
				m_btnAxisOrigin[i].SetValue(FALSE);
				m_btnAxisOrigin[i].SetBackColorInterior(RGB(192, 0, 0));
			}
			else
			{
				bOriginFlag	= m_plnkAxis[i]->IsOriginReturn();
				m_btnAxisOrigin[i].SetBackColorInterior(BASIC);
				
				if (m_bAxisOriginFlag[i] != bOriginFlag)
				{
					if (bOriginFlag == TRUE)
					{
						m_btnAxisOrigin[i].SetValue(TRUE);
					}
					else
					{
						m_btnAxisOrigin[i].SetValue(FALSE);
					}

					m_bAxisOriginFlag[i] = bOriginFlag;
				}
			}

			// Doolin kwangilshin 2017. 08. 23.
			//
			if (!m_rgbForceOriginReturn[i])
			{
				if (bAmpEnableFlag && m_btnAxisOrigin[i].GetBackColor() != RGB(0, 255, 0))
				{
					m_btnAxisOrigin[i].SetBackColor(RGB(0, 255, 0));
				}
				else if (!bAmpEnableFlag && m_btnAxisOrigin[i].GetBackColor() != DKGRAY)
				{
					m_btnAxisOrigin[i].SetBackColor(DKGRAY);
				}
			}
			//
			// End.
		}
	}
	CDialog::OnTimer(nIDEvent);
}

int CAutoMotorOriginDlg::ReturnOrigin(void)
{
	int iResult = ERR_DLG_ORIGIN_SUCCESS;
	int i = 0;
	int rgiOriginMoveType[DEF_AXIS_MAX_AXIS];
	memset(rgiOriginMoveType, 0, sizeof(rgiOriginMoveType));
	
	MOlbSystemPre.GetTrsAutoManagerComponent()->ResetUnitInitialFlag();

	checkAxisSelectionForGroup(rgiOriginMoveType);

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
			continue;
		
		if (m_bAxisSelectFlag[i])
		{
			if (i < DEF_AXIS_SERCOS_MAX_AXIS)
			{
				if (TRUE == m_rgbForceOriginReturn[i])
				{
					m_plnkAxis[i]->ResetOriginComplete();
					changeDisplayForceOriginButton(TRUE);
				}
				else if (FALSE == m_plnkAxis[i]->IsOriginComplete())
				{
					m_rgbForceOriginReturn[i] = TRUE;
					m_btnAxisOrigin[i].SetBackColor(RED);
					changeDisplayForceOriginButton(TRUE);
				}
			}
			
			m_plnkAxis[i]->ServoOn(NULL, TRUE);

#ifndef SIMULATION
			m_plnkAxis[i]->ClearAxis();
			m_plnkAxis[i]->ResetOrigin();

			if (i == DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T
				|| i == DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T
				|| i == DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T
				|| i == DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T
				|| i == DEF_AXIS_SERCOS_FEEDER_REEL_T
				|| i == DEF_AXIS_SERCOS_R_FEEDER_REEL_T
				)
			{
				rgiOriginMoveType[i] = DEF_ORIGIN_RETURN_NONE_MOVE;
			}
			else
			{
				if (DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE != rgiOriginMoveType[i])
				{
					rgiOriginMoveType[i] = DEF_ORIGIN_RETURN_ALL_MOVE;
				}
			}

			// 원점 복귀 진행 전 확인해야 할 부분 처리
			iResult = checkBeforeOrigin(i);
			if (iResult)
				return iResult;
#endif /* SIMULATION */
		}
	}

	CMyProgressWnd ProgWnd(this, _T("Motor Origin Executing..."));
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText(_T("...is executing the Motor Origin...\nWait a moment..."));

	CWinThread* rgpThread[DEF_MAX_ORIGIN_GROUP];

	// Thread 관련 변수 초기화
	for (int i = 0; i < DEF_MAX_ORIGIN_GROUP; i++)
	{
		sm_rgiGroupResult[i] = SUCCESS;
		sm_rgiGroupComplete[i] = FALSE;
		rgpThread[i] = NULL;
	}

	SOriginGroupData sData[DEF_MAX_ORIGIN_GROUP] =
	{
#ifdef DEF_SOURCE_SYSTEM
		//	pThisDlg,	prgiOriginMoveType,	iOriginGroupNo,						bGetPreMoveDist,	iStartAxisNo,					iEndAxisNo
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_TAB_MOUNTER_Z,		FALSE,		DEF_AXIS_SERCOS_MOUNTER1_Z,				DEF_AXIS_SERCOS_MOUNTER4_Z				},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_TAB_MOUNTER_X,		FALSE,		DEF_AXIS_SERCOS_MOUNTER1_X,				DEF_AXIS_SERCOS_MOUNTER4_X				},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TAB_MOUNTER_Z,	FALSE,		DEF_AXIS_SERCOS_R_MOUNTER1_Z,			DEF_AXIS_SERCOS_R_MOUNTER4_Z			},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X,	FALSE,		DEF_AXIS_SERCOS_R_MOUNTER1_X,			DEF_AXIS_SERCOS_R_MOUNTER4_X			},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_BACKUP_Z,			FALSE,		DEF_AXIS_SERCOS_BACKUP1_Z,				DEF_AXIS_SERCOS_BACKUP4_Z				},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_INSPECTION_X,		FALSE,		DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X,	DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X	},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_BACKUP_Z,		FALSE,		DEF_AXIS_SERCOS_R_BACKUP1_Z,			DEF_AXIS_SERCOS_R_BACKUP4_Z				},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_INSPECTION_X,	FALSE,		DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X,	DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X	},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_TABCARRIER_X,	FALSE,		DEF_AXIS_SERCOS_TABIC_CARRIER1_X,		DEF_AXIS_SERCOS_TABIC_CARRIER4_X		}, //SJ_YYK 150109 Modify..
//		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_TABCARRIER_X,	FALSE,		DEF_AXIS_SERCOS_TABIC_CARRIER_Z,		DEF_AXIS_SERCOS_TABIC_CARRIER1_X		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_TABCARRIER_T,	FALSE,		DEF_AXIS_SERCOS_TABIC_CARRIER1_T,		DEF_AXIS_SERCOS_TABIC_CARRIER4_T		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_TABCARRIER_Y,	FALSE,		DEF_AXIS_SERCOS_TABIC_CARRIER1_Y,		DEF_AXIS_SERCOS_TABIC_CARRIER4_Y		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TABCARRIER_X,	FALSE,		DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X,		DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X		},
//		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TABCARRIER_X,	FALSE,		DEF_AXIS_SERCOS_TABIC_R_CARRIER_Z,		DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X		}, //SJ_YYK 150109 Modify...
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TABCARRIER_T,	FALSE,		DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T,		DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TABCARRIER_Y,	FALSE,		DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y,		DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y		},
#else
		//	pThisDlg,	prgiOriginMoveType,	iOriginGroupNo,						bGetPreMoveDist,	iStartAxisNo,					iEndAxisNo
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_TAB_MOUNTER_Z,		FALSE,		DEF_AXIS_SERCOS_MOUNTER1_Z,				DEF_AXIS_SERCOS_MOUNTER2_Z				},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_TAB_MOUNTER_X,		FALSE,		DEF_AXIS_SERCOS_MOUNTER1_X,				DEF_AXIS_SERCOS_MOUNTER2_X				},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TAB_MOUNTER_Z,	FALSE,		DEF_AXIS_SERCOS_R_MOUNTER1_Z,			DEF_AXIS_SERCOS_R_MOUNTER2_Z			},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X,	FALSE,		DEF_AXIS_SERCOS_R_MOUNTER1_X,			DEF_AXIS_SERCOS_R_MOUNTER2_X			},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_BACKUP_Z,			FALSE,		DEF_AXIS_SERCOS_BACKUP1_Z,				DEF_AXIS_SERCOS_BACKUP2_Z				},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_INSPECTION_X,		FALSE,		DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X,	DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X	},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_BACKUP_Z,		FALSE,		DEF_AXIS_SERCOS_R_BACKUP1_Z,			DEF_AXIS_SERCOS_R_BACKUP2_Z				},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_INSPECTION_X,	FALSE,		DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X,	DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X	},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_TABCARRIER_X,	FALSE,		DEF_AXIS_SERCOS_TABIC_CARRIER1_X,		DEF_AXIS_SERCOS_TABIC_CARRIER2_X		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_TABCARRIER_T,	FALSE,		DEF_AXIS_SERCOS_TABIC_CARRIER1_T,		DEF_AXIS_SERCOS_TABIC_CARRIER2_T		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_TABCARRIER_Y,	FALSE,		DEF_AXIS_SERCOS_TABIC_CARRIER1_Y,		DEF_AXIS_SERCOS_TABIC_CARRIER2_Y		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TABCARRIER_X,	FALSE,		DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X,		DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TABCARRIER_T,	FALSE,		DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T,		DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TABCARRIER_Y,	FALSE,		DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y,		DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y		},
#endif
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_TAB_PRESS,		FALSE,		DEF_AXIS_SERCOS_BADTAB_DETECTOR_X,		DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_TAB_PRESS,		FALSE,		DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X,	DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_PANEL_TRANSFER,	FALSE,		DEF_AXIS_SERCOS_STAGE_Y,				DEF_AXIS_SERCOS_MODEL_CHANGE_2			},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_GANTRY,			FALSE,		DEF_AXIS_ACS_GANTRY_Y,					DEF_AXIS_ACS_R_GANTRY_Y					},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_F_CAMERA_EXPAND,	FALSE,		DEF_AXIS_SERCOS_INSPECT_EXPAND1,		DEF_AXIS_SERCOS_TABCAMERA_EXPAND		},
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_R_CAMERA_EXPAND,	FALSE,		DEF_AXIS_SERCOS_R_INSPECT_EXPAND1,		DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2		},
#ifdef DEF_USE_TRANSFER_CENTER
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_HANDLER,			FALSE,		DEF_AXIS_SERCOS_TRANSFER_CENTER_1,		DEF_AXIS_SERCOS_TRANSFER_OUT_2			},
#else
		{	this,		rgiOriginMoveType,	DEF_ORIGIN_GROUP_HANDLER,			FALSE,		DEF_AXIS_SERCOS_TRANSFER_IN_2,		DEF_AXIS_SERCOS_TRANSFER_IN_2			},
#endif
	};

	for (int i=0; i<DEF_MAX_ORIGIN_GROUP; i++)
	{
		sData[i].bGetPreMoveDist = FALSE;
		switch (sData[i].iOriginGroupNo)
		{	
		case DEF_ORIGIN_GROUP_TAB_MOUNTER_X:
		case DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X:
		case DEF_ORIGIN_GROUP_INSPECTION_X:
		case DEF_ORIGIN_GROUP_F_TABCARRIER_X:
		case DEF_ORIGIN_GROUP_R_TABCARRIER_X:
			if (TRUE == m_rgbForceOriginReturn[sData[i].iStartAxisNo])
			{
				sData[i].bGetPreMoveDist = TRUE;// TabMounter (+) Limit 위치로 몰아 두기 동작 포함.		
			}
			break;
		default:
			break;
		}

		// Doolin kwangilshin 2017. 08. 24.
		//
		for (int nGtpCnt = sData[i].iStartAxisNo; nGtpCnt <= sData[i].iEndAxisNo; nGtpCnt++)
		{
			if (m_bAxisSelectFlag[nGtpCnt])
			{
				if (TRUE == sData[i].bGetPreMoveDist)
				{
					rgpThread[sData[i].iOriginGroupNo] = AfxBeginThread(returnForcedOriginGroupAxis, (LPVOID)&sData[i]);
				}
				else
				{
					rgpThread[sData[i].iOriginGroupNo] = AfxBeginThread(returnOriginNormalAxis, (LPVOID)&sData[i]);
				}

				break;
			}
		}
	}

	ProgWnd.StepIt();

	MTickTimer timerLimit;
	timerLimit.StartTimer();

	BOOL bAllComplete	= TRUE;

	while (TRUE)
	{
		ProgWnd.PeekAndPump();

		bAllComplete = TRUE;
		for (int i = 0; i < DEF_MAX_ORIGIN_GROUP; i++)
		{
			if (NULL != rgpThread[i] && FALSE == sm_rgiGroupComplete[i])
			{
				bAllComplete = FALSE;
				break;
			}
		}

		if (TRUE == bAllComplete)
			break;
		
		Sleep(10);
		
		if (TRUE == timerLimit.MoreThan(15 * 60.0))
		{
			ProgWnd.DestroyWindow();

			// 발생할 수 없는 Case : 발생했다면 OS 문제로 봐야 함. [Thread 정상 종료 안됨.]
			// Thread 강제 종료는 하지 않음.
			return ERR_DLG_ORIGIN_PC_ERROR;
		}

		// 확인 필요.
		// error 발생 시, 나머지 축 모두 원점 복귀 될 때가지 기다릴까?
	}

	ProgWnd.DestroyWindow();

	// error 확인.
	for (int i = 0; i < DEF_MAX_ORIGIN_GROUP; i++)
	{
		if (sm_rgiGroupResult[i])
			return sm_rgiGroupResult[i];
	}

	return ERR_DLG_ORIGIN_SUCCESS;
}

void CAutoMotorOriginDlg::checkAxisSelectionForGroup(int* rgiOriginMoveType)
{
	for (int i = 0; i < DEF_AXIS_SERCOS_MAX_AXIS; i++)
	{
		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
			continue;

		if (FALSE == m_bAxisSelectFlag[i])
			continue;

		int iStartAxisID = 0;
		int iLastAxisID = 0;
#ifdef DEF_SOURCE_SYSTEM
		iLastAxisID = DEF_AXIS_SERCOS_MOUNTER4_X;
#else
		iLastAxisID = DEF_AXIS_SERCOS_MOUNTER2_X;
#endif
		//Front Tab Mounter X
		if (i >= DEF_AXIS_SERCOS_MOUNTER1_X && i <= iLastAxisID)
		{
			for (int k = DEF_AXIS_SERCOS_MOUNTER1_X; k <= iLastAxisID; k++)
			{
				if (FALSE == m_rgbForceOriginReturn[k] && TRUE == m_plnkAxis[k]->IsOriginComplete())
					continue;

				for (int l = DEF_AXIS_SERCOS_MOUNTER1_X; l <= iLastAxisID; l++)
				{
					m_bAxisSelectFlag[l] = TRUE;
					rgiOriginMoveType[l] = DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE;
					m_plnkAxis[l]->ResetOrigin();
					m_plnkAxis[l]->ResetOriginComplete();
					if (DEF_AXIS_SERCOS_MOUNTER1_X != l)
						m_plnkAxis[l]->SetHomingSwitchSource(m_plnkAxis[DEF_AXIS_SERCOS_MOUNTER1_X]->GetNodeID());
				}
				i = iLastAxisID;
				break;
			}
		}

#ifdef DEF_SOURCE_SYSTEM
		iLastAxisID = DEF_AXIS_SERCOS_R_MOUNTER4_X;
#else
		iLastAxisID = DEF_AXIS_SERCOS_R_MOUNTER2_X;
#endif
		//Rear Tab Mounter X
		if (i >= DEF_AXIS_SERCOS_R_MOUNTER1_X && i <= iLastAxisID)
		{
			for (int k = DEF_AXIS_SERCOS_R_MOUNTER1_X; k <= iLastAxisID; k++)
			{
				if (FALSE == m_rgbForceOriginReturn[k] && TRUE == m_plnkAxis[k]->IsOriginComplete())
					continue;

				for (int l = DEF_AXIS_SERCOS_R_MOUNTER1_X; l <= iLastAxisID; l++)
				{
					m_bAxisSelectFlag[l] = TRUE;
					rgiOriginMoveType[l] = DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE;
					m_plnkAxis[l]->ResetOrigin();
					m_plnkAxis[l]->ResetOriginComplete();
					if (DEF_AXIS_SERCOS_R_MOUNTER1_X != l)
						m_plnkAxis[l]->SetHomingSwitchSource(m_plnkAxis[DEF_AXIS_SERCOS_R_MOUNTER1_X]->GetNodeID());
				}
				i = iLastAxisID;
				break;
			}
		}

#ifdef DEF_SOURCE_SYSTEM
		iLastAxisID = DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X;
#else
		iLastAxisID = DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X;
#endif
		//Front Inspect Camera X
		if (i >= DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X && i <= iLastAxisID)
		{
			for (int k = DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X; k <= iLastAxisID; k++)
			{
				if (FALSE == m_rgbForceOriginReturn[k] && TRUE == m_plnkAxis[k]->IsOriginComplete())
					continue;
				
				for (int l = DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X; l <= iLastAxisID; l++)
				{
					m_bAxisSelectFlag[l] = TRUE;
					rgiOriginMoveType[l] = DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE;
					m_plnkAxis[l]->ResetOrigin();
					m_plnkAxis[l]->ResetOriginComplete();
					if (DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X != l)
						m_plnkAxis[l]->SetHomingSwitchSource(m_plnkAxis[DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X]->GetNodeID());
				}
				i = iLastAxisID;
				break;
			}
		}

#ifdef DEF_SOURCE_SYSTEM
		iLastAxisID = DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X;
#else
		iLastAxisID = DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X;
#endif

		//SJ_YYK 150109 Del..
		iStartAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER1_X;
#	ifdef DEF_SOURCE_SYSTEM
		iLastAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER4_X;
#	else
		iLastAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER2_X;
#	endif

		//Front Tab Carrier Y
		if (i >= iStartAxisID && i <= iLastAxisID)
		{
			for (int k = iStartAxisID; k <= iLastAxisID; k++)
			{
				if (FALSE == m_rgbForceOriginReturn[k] && TRUE == m_plnkAxis[k]->IsOriginComplete())
					continue;
				
				for (int l = iStartAxisID; l <= iLastAxisID; l++)
				{
					m_bAxisSelectFlag[l] = TRUE;
					rgiOriginMoveType[l] = DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE;
					m_plnkAxis[l]->ResetOrigin();
					m_plnkAxis[l]->ResetOriginComplete();
					if (iStartAxisID != l)
						m_plnkAxis[l]->SetHomingSwitchSource(m_plnkAxis[iStartAxisID]->GetNodeID());
				}
				i = iLastAxisID;
				break;
			}
		}

		iStartAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X;
#	ifdef DEF_SOURCE_SYSTEM
		iLastAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X;
#	else
		iLastAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X;
#	endif
		//Rear Tab Carrier Y
		if (i >= iStartAxisID && i <= iLastAxisID)
		{
			for (int k = iStartAxisID; k <= iLastAxisID; k++)
			{
				if (FALSE == m_rgbForceOriginReturn[k] && TRUE == m_plnkAxis[k]->IsOriginComplete())
					continue;
				
				for (int l = iStartAxisID; l <= iLastAxisID; l++)
				{
					m_bAxisSelectFlag[l] = TRUE;
					rgiOriginMoveType[l] = DEF_ORIGIN_RETURN_EXCEPT_ORIGIN_OFFSET_MOVE;
					m_plnkAxis[l]->ResetOrigin();
					m_plnkAxis[l]->ResetOriginComplete();
					if (iStartAxisID != l)
						m_plnkAxis[l]->SetHomingSwitchSource(m_plnkAxis[iStartAxisID]->GetNodeID());
				}
				i = iLastAxisID;
				break;
			}
		}//*/
	}
	
}

/**
 * 같은 우선순위를 갖는 모든 축의 원점 복귀 완료 상태 여부를 판별한다.
 *
 *  @param nPriority : 우선순위
 *
 *  @return     TRUE : 원점복귀 완료
 *              FALSE: 원점복귀 미완료
 */
BOOL CAutoMotorOriginDlg::IsOriginComplete(int nPriority)
{	
	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
			continue;

		if (m_plnkAxis[i] == NULL)
			continue;

		if (m_bAxisSelectFlag[i]									// 원점복귀 선택여부
			&& (m_plnkAxis[i]->GetOriginPriority() == nPriority))	// 원점우선순위
		{
			if (m_plnkAxis[i]->GetOriginStep() != DEF_ORIGIN_FINISH_STEP)	
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CAutoMotorOriginDlg::isAxisGroupOriginComplete(SOriginGroupData* pData, int nPriority)
{
	for (int i = pData->iStartAxisNo; i <= pData->iEndAxisNo; i++)
	{
		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
			continue;

		if (m_plnkAxis[i] == NULL)
			continue;

		if (m_bAxisSelectFlag[i]									// 원점복귀 선택여부
			&& (m_plnkAxis[i]->GetOriginPriority() == nPriority))	// 원점우선순위
		{
			if (FALSE == m_plnkAxis[i]->IsOriginReturn())
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CAutoMotorOriginDlg::isAxisGroupOriginComplete(SOriginGroupData* pData)
{	
	for (int i = pData->iStartAxisNo; i <= pData->iEndAxisNo; i++)
	{
		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
			continue;

		if (m_plnkAxis[i] == NULL)
			continue;

		if (m_bAxisSelectFlag[i] && FALSE == m_plnkAxis[i]->IsOriginReturn())
			return FALSE;
	}
	return TRUE;
}

int CAutoMotorOriginDlg::checkBeforeOrigin(int iAxisID)
{
	int iResult = ERR_DLG_ORIGIN_SUCCESS;
	
	double dCarrierXSafePos = 0.0;
	switch (iAxisID)
	{
	case DEF_AXIS_SERCOS_STAGE_Y:			
	case DEF_AXIS_SERCOS_STAGE_X:			
	case DEF_AXIS_SERCOS_STAGE_T:
		//강제 원점 복귀 시만, Panel 흡착 상태 확인.
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			MPanelAligner* plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();			
			if (plnkPanelAligner->IsPanelAbsorbed())
			{
				//171013_KDH 진공흡착중이면 강제원점 못하도록 변경
				return 450000000;
				/*
				iResult = plnkPanelAligner->AirCVBlowOff();
				if (iResult)
					return iResult;
				
				iResult = plnkPanelAligner->AirCVVacAllOn(DEF_NONE_WORK_SCHEDULE);
				if (iResult)
					return iResult;
				
				iResult = plnkPanelAligner->ReleasePanel();
				if (iResult)
					return iResult;
				*/
				//__________________
			}
		}
		break;

#ifdef DEF_SOURCE_SYSTEM		
	case DEF_AXIS_SERCOS_TRANSFER_IN:	
		// selsk_jdy 강제 원점 복귀 시만, Panel 흡착 상태 확인.
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			MPanelTransfer* plnkPanelTransfer = MOlbSystemPre.GetPanelTransferComponent();
			if (TRUE == plnkPanelTransfer->IsPanelAbsorbed())
			{
				//171013_KDH 진공흡착중이면 강제원점 못하도록 변경
				return 450001000;
				/*
				MPanelAligner* plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
				iResult = plnkPanelAligner->AirCVBlowOff();
				if (iResult)
					return iResult;				
				iResult = plnkPanelTransfer->ReleasePanel();
				if (iResult)
					return iResult;
					*/
			}
			
			iResult = plnkPanelTransfer->DownPickUpUDCyl();
			if (iResult)
				return iResult;
		}

		//170928 JSH.s Gear
		iResult = MOlbSystemPre.SetGearOut(DEF_AXIS_SERCOS_TRANSFER_IN);
		if (iResult)
			return iResult;
		//170928 JSH.e

		break;


	//170929 KDW Start //Gear
#ifdef DEF_1ST_EQ
	case DEF_AXIS_SERCOS_TRANSFER_CENTER_1:		
		iResult = MOlbSystemPre.SetGearOut(DEF_AXIS_SERCOS_TRANSFER_CENTER_1);
		if (iResult)
			return iResult;
		
		break;
#endif
	//170929 KDW End
#endif

	case DEF_AXIS_SERCOS_TRANSFER_OUT:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			MPanelTransfer* plnkPanelTransfer = MOlbSystemPre.GetPanelTransferOutComponent();
			if (TRUE == plnkPanelTransfer->IsPanelAbsorbed())
			{
				//171013_KDH 진공흡착중이면 강제원점 못하도록 변경
				return 450002000;
				/*
				MPanelAligner* plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
				iResult = plnkPanelAligner->AirCVBlowOff();
				if (iResult)
					return iResult;
				
				iResult = plnkPanelTransfer->ReleasePanel();
				if (iResult)
					return iResult;
				*/
			}
			
			iResult = plnkPanelTransfer->DownPickUpUDCyl();
			if (iResult)
				return iResult;
		}
#ifdef DEF_1ST_EQ
		//170928 JSH.s Gear
		iResult = MOlbSystemPre.SetGearOut(DEF_AXIS_SERCOS_TRANSFER_OUT);
		if (iResult)
			return iResult;
		//170928 JSH.e
#endif

		break;

	case DEF_AXIS_SERCOS_TABIC_CARRIER1_Y:
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_Y:
#	ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_Y:
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_Y:
#	endif
		{
			int iTabCarrierNo = iAxisID - DEF_AXIS_SERCOS_TABIC_CARRIER1_Y;

			MTabCarrier* plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iTabCarrierNo);			

			iResult = plnkTabCarrier->Down();
			if (iResult)
				return iResult;
			iResult = plnkTabCarrier->Down2();
			if (iResult)
				return iResult;
//@			iResult = plnkTabCarrier->BackwardDumpBasket();
//@			if (iResult)
//@				return iResult;
		}
		break;

	case DEF_AXIS_SERCOS_TABIC_CARRIER1_X:
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_X: //SJ_YYK 150109 Del..
#	ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_X:
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_X:
#	endif
		{
			int iTabCarrierNo = iAxisID - DEF_AXIS_SERCOS_TABIC_CARRIER1_X;
			MTabCarrier* plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iTabCarrierNo);			

			iResult = plnkTabCarrier->Down();
			if (iResult)
				return iResult;
			iResult = plnkTabCarrier->Down2();
			if (iResult)
				return iResult;
			iResult = plnkTabCarrier->Backward();
			if (iResult)
				return iResult;
//@			iResult = plnkTabCarrier->BackwardDumpBasket();
//@			if (iResult)
//@				return iResult;
			//CarrierY강제원점일 경우
			if (TRUE == m_rgbForceOriginReturn[iAxisID])
			{
				//CarrierX원점선택안되어 있을 경우 원점복귀 되어 있으면 이동하고, 아니면 에러 발생...
				if (FALSE == m_bAxisSelectFlag[iAxisID-DEF_MAX_TABCARRIER])
				{


					dCarrierXSafePos = plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
					iResult = plnkTabCarrier->MoveYPos(dCarrierXSafePos);
					if (iResult)
						return iResult;
				}
			}
		}
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER1_T:
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_T:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_T:
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_T:
#endif
		{
			int iTabCarrierNo = iAxisID - DEF_AXIS_SERCOS_TABIC_CARRIER1_T;
			MTabCarrier* plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iTabCarrierNo);	
			iResult = plnkTabCarrier->Down();
			if (iResult)
				return iResult;

			// Doolin kwangilshin 2017. 08. 23.
			//
			if (plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->IsOriginReturn())
			{
				if (TRUE == m_rgbForceOriginReturn[iAxisID])
				{
					//CarrierX원점선택안되어 있을 경우 원점복귀 되어 있으면 이동하고, 아니면 에러 발생...
					if (FALSE == m_bAxisSelectFlag[iAxisID-DEF_MAX_TABCARRIER*2])
					{
						dCarrierXSafePos = plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
						iResult = plnkTabCarrier->MoveYPos(dCarrierXSafePos);
						if (iResult)
							return iResult;
					}
				}
			}
			// End.
		}
		break;


	case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y:
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y:
#	ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y:
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y:
#	endif
		{
			int iTabCarrierNo = DEF_TABCARRIER1 + iAxisID - DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y;
			MTabCarrier* plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(iTabCarrierNo);	
			iResult = plnkTabCarrier->Down();
			if (iResult)
				return iResult;

			iResult = plnkTabCarrier->Down2();
			if (iResult)
				return iResult;
//@			iResult = plnkTabCarrier->BackwardDumpBasket();
//@			if (iResult)
//@				return iResult;
		}
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X:
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X:	//SJ_YYK 150109 Del..
#	ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X:
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X:
#	endif
		{
			int iTabCarrierNo = DEF_TABCARRIER1 + iAxisID - DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X;
			MTabCarrier* plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(iTabCarrierNo);		
			iResult = plnkTabCarrier->Down();
			if (iResult)
				return iResult;

			iResult = plnkTabCarrier->Down2();
			if (iResult)
				return iResult;
			iResult = plnkTabCarrier->Backward();
			if (iResult)
				return iResult;
			//CarrierY강제원점일 경우
//@			iResult = plnkTabCarrier->BackwardDumpBasket();
//@			if (iResult)
//@				return iResult;
			if (TRUE == m_rgbForceOriginReturn[iAxisID])
			{
				//CarrierX원점선택안되어 있을 경우
				if (FALSE == m_bAxisSelectFlag[iAxisID-DEF_MAX_TABCARRIER])
				{

					dCarrierXSafePos = plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
					iResult = plnkTabCarrier->MoveYPos(dCarrierXSafePos);
					if (iResult)
						return iResult;
				}
			}
		}
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T:
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T:
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T:
#endif
		{
			int iTabCarrierNo = iAxisID - DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T;
			MTabCarrier* plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(iTabCarrierNo);		
			iResult = plnkTabCarrier->Down();
			if (iResult)
				return iResult;
			iResult = plnkTabCarrier->Down2();
			if (iResult)
				return iResult;
			iResult = plnkTabCarrier->BackwardDumpBasket();
			if (iResult)
				return iResult;

			// Doolin kwangilshin 2017. 08. 23.
			//
			if (plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->IsOriginReturn())
			{
				if (TRUE == m_rgbForceOriginReturn[iAxisID])
				{
					//CarrierX원점선택안되어 있을 경우 원점복귀 되어 있으면 이동하고, 아니면 에러 발생...
					if (FALSE == m_bAxisSelectFlag[iAxisID-DEF_MAX_TABCARRIER*2])
					{
						dCarrierXSafePos = plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
						iResult = plnkTabCarrier->MoveYPos(dCarrierXSafePos);
						if (iResult)
							return iResult;
					}
				}
			}
		}
		break;

		//Model Change 원점 복귀시 
	case DEF_AXIS_SERCOS_MODEL_CHANGE_1:
	case DEF_AXIS_SERCOS_MODEL_CHANGE_2:
		/*/절대모터이므로 움지이지 않음
		//CarrierY강제원점일 경우
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			MPanelAligner* plnkPanelAligner = MOlbSystemPre.GetPanelAlignerComponent();
			if (plnkPanelAligner->IsPanelAbsorbed())
			{
				return 210082000;
			}
		}
		/*/
		break;

	case DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X:
	case DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_INSPECTION_CAMERA3_X:
	case DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X:
#endif
		{
			if (TRUE == m_rgbForceOriginReturn[iAxisID])
			{
				//BackupZ원점선택안되어 있을 경우
				if (FALSE == m_bAxisSelectFlag[iAxisID-DEF_MAX_INSPECTION_CAM_CARRIER])
				{					
					iResult = MOlbSystemPre.GetCameraCarrierComponent(iAxisID-DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
					if (iResult)
						return iResult;
				}
			}
 
			// Doolin kwangilshin 2017.07.22.
			// Tab Mount 충돌 방지.
			//
			for (int nAxisCnt = 0; nAxisCnt < DEF_MAX_TABMOUNTER; nAxisCnt++)
			{
				MOlbSystemPre.GetTabMounterComponent(nAxisCnt)->UpCyl(TRUE);
	//			MOlbSystemPre.GetTabMounterComponent(nAxisCnt)->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS, FALSE, FALSE);
			}
		}
		break;
	case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X:
	case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA3_X:
	case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X:
#endif
		{
			
			if (TRUE == m_rgbForceOriginReturn[iAxisID])
			{
				//BackupZ원점선택안되어 있을 경우
				if (FALSE == m_bAxisSelectFlag[iAxisID-DEF_MAX_INSPECTION_CAM_CARRIER])
				{					
					iResult = MOlbSystemPre.GetRCameraCarrierComponent(iAxisID-DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
					if (iResult)
						return iResult;
				}
			}
		}
		break;
	case DEF_AXIS_SERCOS_MOUNTER1_X:			
	case DEF_AXIS_SERCOS_MOUNTER2_X:			
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_MOUNTER3_X:			
	case DEF_AXIS_SERCOS_MOUNTER4_X:			
#endif
		{
			int iTabMounterNo = iAxisID - DEF_AXIS_SERCOS_MOUNTER1_X;
			MTabMounter* plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(iTabMounterNo);			
			iResult = plnkTabMounter->UpCyl();
			if (iResult)
				return iResult;
		}
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER1_X:			
	case DEF_AXIS_SERCOS_R_MOUNTER2_X:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_R_MOUNTER3_X:			
	case DEF_AXIS_SERCOS_R_MOUNTER4_X:
#endif
		{
			int iTabMounterNo = iAxisID - DEF_AXIS_SERCOS_R_MOUNTER1_X;
			MTabMounter* plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(iTabMounterNo);			
			iResult = plnkTabMounter->UpCyl();
			if (iResult)
				return iResult;
		}
		break;
	case DEF_AXIS_ACS_GANTRY_Y:
		{
			MTabMounter* plnkTabMounter = NULL;
			for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
			{
				plnkTabMounter = MOlbSystemPre.GetTabMounterComponent(i);
				iResult = plnkTabMounter->UpCyl();
				if (iResult)
					return iResult;
			}
		}
		break;
	case DEF_AXIS_ACS_R_GANTRY_Y:
		{
			MTabMounter* plnkTabMounter = NULL;
			for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
			{
				plnkTabMounter = MOlbSystemPre.GetRTabMounterComponent(i);
				iResult = plnkTabMounter->UpCyl();
				if (iResult)
					return iResult;
			}
		}
		break;

	case DEF_AXIS_SERCOS_BACKUP1_Z:
	case DEF_AXIS_SERCOS_BACKUP2_Z:
	case DEF_AXIS_SERCOS_BACKUP3_Z:
	case DEF_AXIS_SERCOS_BACKUP4_Z:
		{
// 			if (TRUE == m_rgbForceOriginReturn[iAxisID])
// 			{
// 				MyMessageBox(_T("Stoper Not wearing. Make sure that BackupZ axis and front and rear Inspection do not interfere and then return to origin.\r\nStoper미입고 상태 입니다. BackupZ축과 전후 Inspection이 간섭 없는지 확인 후 원점 복귀해 주세요."), _T("Confirm"));
// 			}
		}
		break;
	default:
		break;
	}
	
	return ERR_DLG_ORIGIN_SUCCESS;
}

int CAutoMotorOriginDlg::checkOnOrigin(int iAxisID)
{
	int iError = ERR_DLG_ORIGIN_SUCCESS;
 
	//20171004 KDW Start
#ifndef DEF_SETUP
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->IsDoorOpen_AOC())
	{
// 		int iResult = 300013000;
// 		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
// 		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("AOC Door Open Error"), M_ICONINFORMATION);
		return 300013000;
	}
#endif
	//20171004 KDW End
	

	if (TRUE == MOlbSystemPre.IsSkipAxis(iAxisID))
		return ERR_DLG_ORIGIN_SUCCESS;

	// 원점 복귀 도중 비상정지 입력
	if (m_plnkManageOpPanel->GetEStopSWStatus())
		return ERR_DLG_ORIGIN_DETECT_E_STOP;
	
	// 원점 복귀 도중 Step Stop switch 입력
	if (m_plnkManageOpPanel->GetStopSWStatus())
		return ERR_DLG_ORIGIN_DETECT_USER_STOP;

	// 원점복귀 Step 이 ErrorStep 인 경우
	if (DEF_ORIGIN_ERROR_STEP == m_plnkAxis[iAxisID]->GetOriginStep())
	{
		//170721 KMH TEST
		//int iError = ERR_DLG_ORIGIN_SUCCESS;
		m_plnkAxis[iAxisID]->GetOriginError(&iError);
		return iError;
	}
//
	//확인 필요.
	// Amp Fault Check 170721_TEST
	if ((iAxisID != DEF_AXIS_ACS_GANTRY_Y && iAxisID != DEF_AXIS_ACS_R_GANTRY_Y)
		&& (TRUE == m_plnkAxis[iAxisID]->IsAmpFault()
		|| TRUE == m_plnkAxis[iAxisID]->IsEStopEvent()
		|| FALSE == m_plnkAxis[iAxisID]->IsAmpEnable()))
	{	
			return ERR_DLG_ORIGIN_DETECT_AMP_FAULT;
	}
	

	// 전/후 설비 Interface 동작 금지 신호 Check : 가압착은 확인 불필요.
	
	// 원점 복귀 중 Check할 사항

	return ERR_DLG_ORIGIN_SUCCESS;
}
//170922 JSH.s
int CAutoMotorOriginDlg::checkAfterOrigin(int iAxisID)
{
	// 원점복귀 완료된 후 해야할 동작을 실행할 수 있도록 해야 한다.
	int iResult = ERR_DLG_ORIGIN_SUCCESS;
	double dHomeOffset = 0.0;
	double dCarrierXSafePos = 0.0;
	

	switch (iAxisID)
	{
		
		//SJ_YYK 150109 Add..
	case DEF_AXIS_SERCOS_TABIC_CARRIER1_Y:
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_Y:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_Y:
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_Y:
#endif
		if(TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			int iTabCarrierNo = DEF_TABCARRIER1 + iAxisID - DEF_AXIS_SERCOS_TABIC_CARRIER1_Y;
			
			MTabCarrier* plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iTabCarrierNo);		
			dCarrierXSafePos = plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
			
			iResult = plnkTabCarrier->MoveYPos(dCarrierXSafePos);
			if(iResult)
				return iResult;
		}
		break;
		
		
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y:
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y:
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y:
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y:
#endif
		if(TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			int iTabCarrierNo = DEF_TABCARRIER1 + iAxisID - DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y;
			
			MTabCarrier* plnkTabCarrier = MOlbSystemPre.GetRTabCarrierComponent(iTabCarrierNo);	
			dCarrierXSafePos = plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
			iResult = plnkTabCarrier->MoveYPos(dCarrierXSafePos);
			if(iResult)
				return iResult;
		}
		break;

	//170929 KDW Start //Gear
#ifdef DEF_1ST_EQ
	case DEF_AXIS_SERCOS_TRANSFER_CENTER_1:
		iResult = MOlbSystemPre.SetGearIn(DEF_AXIS_SERCOS_TRANSFER_CENTER_1);
		if (iResult)
			return iResult;
		break;
#endif

	case DEF_AXIS_SERCOS_TRANSFER_IN:
		iResult = MOlbSystemPre.SetGearIn(DEF_AXIS_SERCOS_TRANSFER_IN);
		if (iResult)
			return iResult;
		break;
	//170929 KDW End

	//170928 JSH.s Gear
	case DEF_AXIS_SERCOS_TRANSFER_OUT:
#ifdef DEF_1ST_EQ
		iResult = MOlbSystemPre.SetGearIn(DEF_AXIS_SERCOS_TRANSFER_OUT);
		if (iResult)
			return iResult;
		break;
	//170928 JSH.e
#endif

	default:
		break;
	}
	return ERR_DLG_ORIGIN_SUCCESS;
}

int CAutoMotorOriginDlg::checkCompleteOrigin(int iAxisID)
{
	// 원점복귀 완료된 후 해야할 동작을 실행할 수 있도록 해야 한다.

	int iResult = ERR_DLG_ORIGIN_SUCCESS;
	double dHomeOffset = 0.0;
	double dCarrierXSafePos = 0.0;

	switch (iAxisID)
	{
	case DEF_AXIS_SERCOS_INSPECT_EXPAND1:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][0]);				
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
			if(iResult)
				return iResult;
			//_____________________
		}
		break;
	case DEF_AXIS_SERCOS_INSPECT_EXPAND2:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][1]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}		
			if(iResult) //170720_KDH 임시 에러처리,
				return iResult;
		}
		break;
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_INSPECT_EXPAND3:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][2]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
			if(iResult)
				return iResult;
		}
		break;
	case DEF_AXIS_SERCOS_INSPECT_EXPAND4:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][3]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
			if(iResult)
				return iResult;
		
		}
		break;
#endif
	case DEF_AXIS_SERCOS_TABCAMERA_EXPAND:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][4]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
			if(iResult)
				return iResult;
		}
		break;

	case DEF_AXIS_SERCOS_TABCAMERA_EXPAND2:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_FRONT_GROUP][5]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
			if(iResult) //170720_KDH 임시 에러처리,
				return iResult;
		}
			break;

	case DEF_AXIS_SERCOS_R_INSPECT_EXPAND1:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
{
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][0]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
			if(iResult)
				return iResult;
		}
		break;
	case DEF_AXIS_SERCOS_R_INSPECT_EXPAND2:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][1]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
		
			if(iResult) //170720_KDH 임시 에러처리,
				return iResult;
		}
		break;
#ifdef DEF_SOURCE_SYSTEM
	case DEF_AXIS_SERCOS_R_INSPECT_EXPAND3:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
			m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][2]);
		break;
	case DEF_AXIS_SERCOS_R_INSPECT_EXPAND4:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
			m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][3]);
		break;
#endif
	case DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][4]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
			
		
			if(iResult) //170720_KDH 임시 에러처리,
				return iResult;	
		}
		break;

	case DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2:
		if (TRUE == m_rgbForceOriginReturn[iAxisID])
		{
			//170721_KDH Current Pos 실패 시 에러처리 및 다시시도
			for(int i = 0; i < 10; i++)
			{
				iResult = m_plnkAxis[iAxisID]->SetCurrentPosition(m_plnkSystemData->m_dData_CameraExpand[DEF_REAR_GROUP][5]);
				if(!iResult) //170720_KDH 임시 에러처리,
					break;
				Sleep(100);
			}
			if(iResult) //170720_KDH 임시 에러처리,
				return iResult;
		}
		break;
	}
	return ERR_DLG_ORIGIN_SUCCESS;
}
//170922 JSH.e

void CAutoMotorOriginDlg::OnForceOriginReturn() 
{
	// sesl_jdy
	BOOL bAlreadyAll = TRUE;	// 선택된 축들이 모두 원점 강제 원점복귀 하도록 설정되어 있는지 여부

	for (int i = 0; i < DEF_AXIS_SERCOS_MAX_AXIS; i++)
	{
		if (TRUE == m_bAxisSelectFlag[i])
			bAlreadyAll &= m_rgbForceOriginReturn[i];
	}

	if (TRUE == bAlreadyAll)
	{
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			m_rgbForceOriginReturn[i] = FALSE;

			if (FALSE == MOlbSystemPre.IsSkipAxis(i))
				m_btnAxisOrigin[i].SetBackColor(DKGRAY);
		}
		changeDisplayForceOriginButton(FALSE);
		//MyMessageBox(_T("강제원점복귀가 해제되었습니다."), _T("Confirm"));
		MyMessageBox(_T("Force Origin Return Unlock.\n강제원점복귀가 해제되었습니다."), _T("Confirm"));
	}
	else
	{
		//	선택한 축을 강제 원점복귀 하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX, 20217, _T("Origin Force"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		for (int i = 0; i < DEF_AXIS_SERCOS_MAX_AXIS; i++)
		{
			if (TRUE == m_bAxisSelectFlag[i])
			{
				m_rgbForceOriginReturn[i] = TRUE;
//@KKY.130913.원점복귀 동작시 설정하도록 변경				m_plnkAxis[i]->ResetOriginComplete();
				m_btnAxisOrigin[i].SetBackColor(RED);
			}
		}
		changeDisplayForceOriginButton(TRUE);
	}
}

void CAutoMotorOriginDlg::OnSERCOSnetInit() 
{
#ifndef SIMULATION
	CMyProgressWnd ProgWnd(this, _T("NMC Network Initializing..."));
	ProgWnd.SetRange(0, DEF_AXIS_MAX_PRIORITY);
	ProgWnd.SetText(_T("...is executing NMC Network Initialization...\n")
					    _T("Wait a moment..."));
	ProgWnd.PeekAndPump();

//	MMC2 초기화(Phase4)를 하시겠습니까?
	if ( MyMessageBox(MY_MESSAGE_BOX, 20219, _T("NMC Initialize"), M_ICONQUESTION|M_YESNO) == IDNO )
		return;

	m_btnAllSelect.EnableWindow(FALSE);
	m_btnAllUnselect.EnableWindow(FALSE);
	m_btnServoOn.EnableWindow(FALSE);
	m_btnServoOff.EnableWindow(FALSE);
	m_btnReturnOrigin.EnableWindow(FALSE);
	m_btnExit.EnableWindow(FALSE);
	GetDlgItem(IDB_SERCOS_NET_INIT)->EnableWindow(FALSE);
	m_btnForceOrigin.EnableWindow(FALSE);

	int iResult = MOlbSystemPre.InitRS_NMCBoard();  //170717 JSH

	// hongju_SESL
//	iResult = m_plnkCANSetup->BoardInit();

	m_btnAllSelect.EnableWindow(TRUE);
	m_btnAllUnselect.EnableWindow(TRUE);
	m_btnServoOn.EnableWindow(TRUE);
	m_btnServoOff.EnableWindow(TRUE);
	m_btnReturnOrigin.EnableWindow(TRUE);
	m_btnExit.EnableWindow(TRUE);
	GetDlgItem(IDB_SERCOS_NET_INIT)->EnableWindow(TRUE);
	m_btnForceOrigin.EnableWindow(TRUE);

	if (iResult != ERR_DLG_ORIGIN_SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr, _T("Initialize Error"), M_ICONERROR,_T("Confirm"),_T("Cancel"));
		return;
	}

//	초기화가 완료되었습니다.
	MyMessageBox(MY_MESSAGE_BOX, 20220, _T("Initialize Success"), M_ICONINFORMATION);
#endif
}

void CAutoMotorOriginDlg::clearAxisForIAMNode(BOOL bServoOnOff)
{
//	#pragma message(__LOC__ "여기 IAMNode확인필요")
	int rgiIAMNode[] = {
		DEF_AXIS_SERCOS_STAGE_Y,
#ifdef DEF_SOURCE_SYSTEM
		DEF_AXIS_SERCOS_BACKUP1_Z,
		DEF_AXIS_SERCOS_BADTAB_DETECTOR_X,
		DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X,
#else	// DEF_GATE_SYSTEM
		DEF_AXIS_SERCOS_BADTAB_DETECTOR_X,
#endif
		-1
	};

	for (int i = 0; rgiIAMNode[i] != -1; i++)
		m_plnkAxis[rgiIAMNode[i]]->ClearAxis();

	if (TRUE == bServoOnOff)
	{
		for (int i = 0; rgiIAMNode[i] != -1; i++)
			m_plnkAxis[rgiIAMNode[i]]->ServoOff();

		for (int i = 0; rgiIAMNode[i] != -1; i++)
		{
			Sleep(100);
			m_plnkAxis[rgiIAMNode[i]]->ServoOn();
		}
	}
}

//161108 JSH.s Origin
void CAutoMotorOriginDlg::getOriginGroupAxisOrder(int iGroupId, int* iAxisId)
{
	int i = 0;
	memset(iAxisId, -1, sizeof(int)*DEF_MAX_ORIGIN_GROUP);	
	switch (iGroupId)
	{
		case DEF_ORIGIN_GROUP_TAB_MOUNTER_Z:
			iAxisId[0] = DEF_AXIS_SERCOS_MOUNTER1_Z;
			iAxisId[1] = DEF_AXIS_SERCOS_MOUNTER2_Z;
			break;
		case DEF_ORIGIN_GROUP_TAB_MOUNTER_X:
			iAxisId[0] = DEF_AXIS_SERCOS_MOUNTER1_X;
			iAxisId[1] = DEF_AXIS_SERCOS_MOUNTER2_X;
			break;
		case DEF_ORIGIN_GROUP_R_TAB_MOUNTER_Z:
			iAxisId[0] = DEF_AXIS_SERCOS_R_MOUNTER1_Z;
			iAxisId[1] = DEF_AXIS_SERCOS_R_MOUNTER2_Z;
			break;
		case DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X:
			iAxisId[0] = DEF_AXIS_SERCOS_R_MOUNTER1_X;
			iAxisId[1] = DEF_AXIS_SERCOS_R_MOUNTER2_X;
			break;
		case DEF_ORIGIN_GROUP_BACKUP_Z:
			iAxisId[0] = DEF_AXIS_SERCOS_BACKUP1_Z;
			iAxisId[1] = DEF_AXIS_SERCOS_BACKUP2_Z;
			break;
		case DEF_ORIGIN_GROUP_INSPECTION_X:
			iAxisId[0] = DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X;
			iAxisId[1] = DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X;
			break;
		case DEF_ORIGIN_GROUP_R_BACKUP_Z:
			iAxisId[0] = DEF_AXIS_SERCOS_R_BACKUP1_Z;
			iAxisId[1] = DEF_AXIS_SERCOS_R_BACKUP2_Z;
			break;
		case DEF_ORIGIN_GROUP_R_INSPECTION_X:
			iAxisId[0] = DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X;
			iAxisId[1] = DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X;
			break;
		case DEF_ORIGIN_GROUP_F_TABCARRIER_X:
			iAxisId[0] = DEF_AXIS_SERCOS_TABIC_CARRIER1_X;
			iAxisId[1] = DEF_AXIS_SERCOS_TABIC_CARRIER2_X;
			break;
		case DEF_ORIGIN_GROUP_F_TABCARRIER_T:
			iAxisId[0] = DEF_AXIS_SERCOS_TABIC_CARRIER1_T;
			iAxisId[1] = DEF_AXIS_SERCOS_TABIC_CARRIER2_T;
			break;
		case DEF_ORIGIN_GROUP_F_TABCARRIER_Y:
			iAxisId[0] = DEF_AXIS_SERCOS_TABIC_CARRIER1_Y;
			iAxisId[1] = DEF_AXIS_SERCOS_TABIC_CARRIER2_Y;
			break;
		case DEF_ORIGIN_GROUP_R_TABCARRIER_X:
			iAxisId[0] = DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X;
			iAxisId[1] = DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X;
			break;
		case DEF_ORIGIN_GROUP_R_TABCARRIER_T:
			iAxisId[0] = DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T;
			iAxisId[1] = DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T;
			break;
		case DEF_ORIGIN_GROUP_R_TABCARRIER_Y:
			iAxisId[0] = DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y;
			iAxisId[1] = DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y;
			break;
		case DEF_ORIGIN_GROUP_F_TAB_PRESS:
			iAxisId[0] = DEF_AXIS_SERCOS_BADTAB_DETECTOR_X;
			iAxisId[1] = DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T;
			iAxisId[2] = DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T;
			iAxisId[3] = DEF_AXIS_SERCOS_TAB_PRESS_Z;
			iAxisId[4] = DEF_AXIS_SERCOS_FEEDER_REEL_T;
			iAxisId[5] = DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z;
			break;
		case DEF_ORIGIN_GROUP_R_TAB_PRESS:
			iAxisId[0] = DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X;
			iAxisId[1] = DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T;
			iAxisId[2] = DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T;
			iAxisId[3] = DEF_AXIS_SERCOS_R_TAB_PRESS_Z;
			iAxisId[4] = DEF_AXIS_SERCOS_R_FEEDER_REEL_T;
			iAxisId[5] = DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z;
			break;
		case DEF_ORIGIN_GROUP_PANEL_TRANSFER:
			iAxisId[0] = DEF_AXIS_SERCOS_STAGE_Y;
			iAxisId[1] = DEF_AXIS_SERCOS_STAGE_T;
			iAxisId[2] = DEF_AXIS_SERCOS_STAGE_X;
			iAxisId[3] = DEF_AXIS_SERCOS_STAGE_Z;
			iAxisId[4] = DEF_AXIS_SERCOS_MODEL_CHANGE_1;
			iAxisId[5] = DEF_AXIS_SERCOS_MODEL_CHANGE_2;
			break;
		case DEF_ORIGIN_GROUP_GANTRY:
			iAxisId[0] = DEF_AXIS_SERCOS_TRANSFER_OUT;
			iAxisId[2] = DEF_AXIS_SERCOS_TABCAMERA_EXPAND2;
			iAxisId[3] = DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2;
			iAxisId[4] = DEF_AXIS_ACS_GANTRY_Y;
			iAxisId[5] = DEF_AXIS_ACS_R_GANTRY_Y;
			break;
		case DEF_ORIGIN_GROUP_F_CAMERA_EXPAND:
			iAxisId[0] = DEF_AXIS_SERCOS_INSPECT_EXPAND1;
			iAxisId[1] = DEF_AXIS_SERCOS_INSPECT_EXPAND2;
			iAxisId[2] = DEF_AXIS_SERCOS_TABCAMERA_EXPAND;
			break;
		case DEF_ORIGIN_GROUP_R_CAMERA_EXPAND:
			iAxisId[0] = DEF_AXIS_SERCOS_R_INSPECT_EXPAND1;
			iAxisId[1] = DEF_AXIS_SERCOS_R_INSPECT_EXPAND2;
			iAxisId[2] = DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND;
			break;
#ifdef DEF_USE_PRE_ALIGN_CAMERA
		//170112 JSH.s
		case DEF_ORIGIN_GROUP_PRE_ALIGN_CAMERA:
			iAxisId[0] = DEF_AXIS_SERCOS_PRE_ALIGN_CAMERA1;
			iAxisId[1] = DEF_AXIS_SERCOS_PRE_ALIGN_CAMERA2;
			break;
		//170112 JSH.e
#endif
	}
}
//161108 JSH.e Origin

UINT CAutoMotorOriginDlg::returnForcedOriginGroupAxis(LPVOID pParam)
{
	SOriginGroupData sData;
	CAutoMotorOriginDlg* pDlg = (CAutoMotorOriginDlg*)((SOriginGroupData*)pParam)->pThisDlg;
	pDlg->setData((SOriginGroupData*)pParam, &sData);

	
	double dCarrierXSafePos = 0.0;
	if (DEF_ORIGIN_GROUP_F_TABCARRIER_X == sData.iOriginGroupNo)
	{
		DWORD sTime = GetTickCount();

		while (1)
		{
			Sleep(10);
			BOOL allXOrigin = TRUE;
			
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				allXOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_TABIC_CARRIER1_Y+i]->IsOriginReturn();
			}
		
			if (TRUE == allXOrigin)
				break;
			
			if (GetTickCount() - sTime > 120*1000)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			else if (pDlg->Check_IsOPPanelSwitch(sData.iOriginGroupNo))
			{
				return 0;
			}
		}

		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{
			dCarrierXSafePos = MOlbSystemPre.GetTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
			int iResult = MOlbSystemPre.GetTabCarrierComponent(i)->MoveYPos(dCarrierXSafePos);
			if (iResult)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
		}
	}
	else if (DEF_ORIGIN_GROUP_R_TABCARRIER_X == sData.iOriginGroupNo)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			BOOL allXOrigin = TRUE;
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				allXOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y+i]->IsOriginReturn();
			}
			if (TRUE == allXOrigin)
				break;
			if (GetTickCount() - sTime > 120*1000)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			else if (pDlg->Check_IsOPPanelSwitch(sData.iOriginGroupNo))
			{
				return 0;
			}
		}
		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{
			dCarrierXSafePos = MOlbSystemPre.GetRTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
			int iResult = MOlbSystemPre.GetRTabCarrierComponent(i)->MoveYPos(dCarrierXSafePos);
			if (iResult)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
		}
	}
	else if (DEF_ORIGIN_GROUP_INSPECTION_X == sData.iOriginGroupNo)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			BOOL allXOrigin = TRUE;
			BOOL allExpandOrigin = TRUE; //SJ_YYK 151119 Add...
			for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			{
				allXOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_BACKUP1_Z+i]->IsOriginReturn();
			}

			//SJ_YYK 151119 Add...
			for (int i =0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			{
				allExpandOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_INSPECT_EXPAND1+i]->IsOriginReturn();
				//170723_KDH 
				if (pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_INSPECT_EXPAND1+i]
					|| pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_INSPECT_EXPAND1+i])
					allExpandOrigin &= !(pDlg->m_plnkAxis[DEF_AXIS_SERCOS_INSPECT_EXPAND1+i]->IsInMotion());	
				//______________
			}
		
			if (TRUE == allXOrigin && TRUE == allExpandOrigin) //SJ_YYK 151119 Modify..
				break;

			if (GetTickCount() - sTime > 120*1000)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			else if (pDlg->Check_IsOPPanelSwitch(sData.iOriginGroupNo))
			{
				return 0;
			}
		}
		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			int iResult = MOlbSystemPre.GetCameraCarrierComponent(i)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
		}
	}	
	else if (DEF_ORIGIN_GROUP_R_INSPECTION_X == sData.iOriginGroupNo)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			BOOL allXOrigin = TRUE;

			BOOL allExpandOrigin = TRUE; //SJ_YYK 151119 Add...

			for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			{
				if (TRUE == MOlbSystemPre.IsSkipAxis(i) || !pDlg->m_bAxisSelectFlag[i])
				{
					continue;
				}

				allXOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_R_BACKUP1_Z+i]->IsOriginReturn();
			}

			//SJ_YYK 151119 Add...
			for (int i =0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			{
				if (TRUE == MOlbSystemPre.IsSkipAxis(i) || !pDlg->m_bAxisSelectFlag[i])
				{
					continue;
				}

				allExpandOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_R_INSPECT_EXPAND1+i]->IsOriginReturn();
			}

			if (TRUE == allXOrigin && TRUE == allExpandOrigin) //SJ_YYK 151119 Modify..
				break;
			if (GetTickCount() - sTime > 120*1000)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			else if (pDlg->Check_IsOPPanelSwitch(sData.iOriginGroupNo))
			{
				return 0;
			}
		}
		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			if (TRUE == MOlbSystemPre.IsSkipAxis(i) || !pDlg->m_bAxisSelectFlag[i])
			{
				continue;
			}

			int iResult = MOlbSystemPre.GetRCameraCarrierComponent(i)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
		}
	}
	else if (DEF_ORIGIN_GROUP_F_TABCARRIER_X == sData.iOriginGroupNo)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			BOOL allXOrigin = TRUE;
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				//161122 JSH.s Modify...
				/*/
				if ((pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER_Z] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER_Z])
					||( pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER1_X] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER1_X]))		
				/*/
				if ((pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER1_X] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER1_X])
					||( pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER2_X] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER2_X]))	
				//161122 JSH.e	
				allXOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_TABIC_CARRIER1_Y+i]->IsOriginReturn();
			}
			if (TRUE == allXOrigin)
				break;
			if (GetTickCount() - sTime > 120*1000)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			else if (pDlg->Check_IsOPPanelSwitch(sData.iOriginGroupNo))
			{
				return 0;
			}
		}
		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{
			//161122 JSH.s Modify...
			/*/
			if ((pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER_Z] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER_Z])
					||( pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER1_X] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER1_X]))		
			/*/
			if ((pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER1_X] 
				&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER1_X])
				||( pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER2_X] 
				&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER2_X]))	
			//161122 JSH.e	
			{
				dCarrierXSafePos = MOlbSystemPre.GetTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
				int iResult = MOlbSystemPre.GetTabCarrierComponent(i)->MoveYPos(dCarrierXSafePos);
				if (iResult)
				{
					sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
					sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
					return 0;
				}
			}
		}
	}
	else if (DEF_ORIGIN_GROUP_R_TABCARRIER_X == sData.iOriginGroupNo)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			BOOL allXOrigin = TRUE;
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				//161122 JSH.s Modify...
				/*/
				if ((pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER_Z] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER_Z])
					||(pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X]))			
				/*/
				if ((pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X])
					||( pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X]))	
				//161122 JSH.e				
				allXOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y+i]->IsOriginReturn();
			}
			if (TRUE == allXOrigin)
				break;
			if (GetTickCount() - sTime > 120*1000)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			else if (pDlg->Check_IsOPPanelSwitch(sData.iOriginGroupNo))
			{
				return 0;
			}
		}
		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{
			//161122 JSH.s Modify...
			/*/
			if ((pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER_Z] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER_Z])
					||(pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X]))				
			/*/
			if ((pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X] 
				&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X])
				||( pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X] 
				&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X]))	
			//161122 JSH.e			
			{
				dCarrierXSafePos = MOlbSystemPre.GetRTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
				int iResult = MOlbSystemPre.GetRTabCarrierComponent(i)->MoveYPos(dCarrierXSafePos);
				if (iResult)
				{
					sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
					sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
					return 0;
				}
			}
		}
	}

	// (+) Limit 방향으로 모으는 작업을 먼저 한다.
	int iResult = pDlg->moveAxisToPlusLimitPos(&sData);
	if (iResult)
	{		
		for (int nAxisIdx = sData.iStartAxisNo; nAxisIdx <= sData.iEndAxisNo; nAxisIdx++)
		{
			pDlg->m_plnkAxis[nAxisIdx]->SetNegativeSensorEvent(DEF_E_STOP_EVENT);
		}

		sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
		sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
		return 0;
	}

	//120208.kms___________
	//무조건 Init 위치로 이동하여 
/*
	if (DEF_ORIGIN_GROUP_INSPECTION_X == sData.iOriginGroupNo)
	{
		iResult = MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->SafeMoveYPos(DEF_MODEL_CHANGE_INIT_POS);
		if (iResult)
		{
//@			return iResult;
			sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
			sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
			return 0;
		}
	}
	else if (DEF_ORIGIN_GROUP_R_INSPECTION_X == sData.iOriginGroupNo)
	{
		iResult = MOlbSystemPre.GetModelChangeComponent(DEF_REAR_GROUP)->SafeMoveYPos(DEF_MODEL_CHANGE_INIT_POS);
		if (iResult)
		{
//@			return iResult;
			sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
			sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
			return 0;
		}
	}
	*/	
	//_____________________
	
	// 첫번째 축에 대해서 이동 거리 측정 작업을 한다.
	if (TRUE == sData.bGetPreMoveDist)
	{
		// Home Sensor 와 기준 TabMounter 간 거리 측정.
		iResult = pDlg->getPreMoveDistance(&sData);
		if (iResult)
		{
			sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
			sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
			return 0;
		}
	}

	// 원점 복귀를 한다.
	// 원점 복귀를 하는 중에 다음 축을 이동시킨다.
	// 원점 복귀 끝나고 간섭 없는 위치로 이동 시킨다.
	
	int iStartAxisNo = sData.iStartAxisNo;
	int iEndAxisNo = sData.iEndAxisNo;

//	if (DEF_ORIGIN_GROUP_TAB_MOUNTER_F == sData.iOriginGroupNo)
	///	iStartAxisNo += 1;	// DEF_AXIS_SERCOS_MOUNTER1_Y

	for (int i = iStartAxisNo; i <= iEndAxisNo; i++)
	{
		pDlg->m_plnkAxis[i]->SetNegativeSensorEvent(DEF_E_STOP_EVENT);

		if (!pDlg->m_plnkAxis[i]->IsAmpEnable() || pDlg->m_plnkAxis[i]->IsAmpFault() || pDlg->m_plnkAxis[i]->IsEStopEvent())
		{
			if ((iResult = pDlg->m_plnkAxis[i]->ServoOn(NULL, TRUE)) != ERR_MOTION_SUCCESS)
			{
				return iResult;
			}

			Sleep(500);
		}
		
		if (pDlg->m_plnkAxis[i]->IsInMotion())
		{
			if ((iResult = pDlg->m_plnkAxis[i]->Stop()) != ERR_MOTION_SUCCESS)
			{
				return iResult;
			}
		}
	}

	int iPriority;
	BOOL bNextAxisMoveStart;
	for (int i = iStartAxisNo; i <= iEndAxisNo; i++)
	{
		iPriority = pDlg->m_plnkAxis[i]->GetOriginPriority();
		bNextAxisMoveStart = FALSE;

		iResult = pDlg->m_plnkAxis[i]->Wait4Done();
		if (iResult)
		{
			/*
			sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
			sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
			*/
			//SJ_YYK 160315 Add.
			pDlg->m_plnkAxis[i]->ServoOn();				
			Sleep(50);
			iResult = pDlg->m_plnkAxis[i]->Wait4Done();
			if(iResult)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}			
		}

		double	dCurrentPos			= pDlg->m_plnkAxis[i]->GetCurrentPosition();
		double	dVelocity			= pDlg->m_plnkAxis[i]->GetCoarseVelocity() * 2.0;
		int		iAccel				= pDlg->m_plnkAxis[i]->GetCoarseAccelerate();
		BOOL	bOriginDir			= pDlg->m_plnkAxis[i]->GetOriginDir();
		double	dOriginLimitTime	= pDlg->m_plnkAxis[i]->GetOriginWaitTime();

		iResult = pDlg->m_plnkAxis[i]->VMove(dVelocity, iAccel, bOriginDir);
		if (iResult)
			return iResult;
		
		MTickTimer timerLimit;
		timerLimit.StartTimer();
		while (TRUE)
		{
			if (TRUE == pDlg->m_plnkAxis[iStartAxisNo]->IsOnHomeSensor())
			{
				iResult =pDlg-> m_plnkAxis[i]->VStop();
				if (iResult)
					return iResult;
				
				break;
			}
			
			Sleep(10);
			
			if (TRUE == timerLimit.MoreThan(dOriginLimitTime))
			{
				pDlg->m_plnkAxis[i]->VStop();
				return ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
			}
			else if (pDlg->Check_IsOPPanelSwitch(sData.iOriginGroupNo))
			{
				return ERR_DLG_ORIGIN_FAIL;
			}
			
			iResult = pDlg->checkOnOrigin(i);
			if (iResult)
			{
				pDlg->m_plnkAxis[i]->VStop();
				return iResult;
			}
		}
		//________________________________________________*/

		BOOL bMoveToReady = FALSE;
		DWORD sTime = 0;

		while (TRUE)
		{
			Sleep(5);
			
			if (pDlg->Check_IsOPPanelSwitch(sData.iOriginGroupNo))
			{
				return 0;
			}

			iResult = pDlg->doOriginReturnSingleAxis(&sData, i, iPriority);
			if (iResult)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;

				for (int k = sData.iStartAxisNo; k <= sData.iEndAxisNo; k++)
					pDlg->m_plnkAxis[k]->Stop();

				pDlg->m_plnkAxis[i]->HomingStop();
 				return 0;
			}
			
			int itemp = pDlg->m_plnkAxis[i]->GetOriginStep();
//			TRACE(_T("%d \n"),itemp);
			if (FALSE == bNextAxisMoveStart && TRUE == sData.bGetPreMoveDist)
			{
				/*
				if (710 == pDlg->m_plnkAxis[i]->GetOriginStep()
					&& i != sData.iEndAxisNo
					)
				{
					int iMoveAxisNo = i + 1;
					iResult = pDlg->moveOriginReturnReadyPos(&sData, iStartAxisNo, iMoveAxisNo);
					if (iResult)
					{
						sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
						sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;

						for (int k = sData.iStartAxisNo; k <= sData.iEndAxisNo; k++)
							pDlg->m_plnkAxis[k]->Stop();
						
						pDlg->m_plnkAxis[i]->HomingStop();
						return 0;
					}
					bNextAxisMoveStart = TRUE;
				}
				*/
				if (700 <= pDlg->m_plnkAxis[i]->GetOriginStep() && i != sData.iEndAxisNo)
				{
					if (FALSE == bMoveToReady)
					{
						bMoveToReady = TRUE;
						sTime = GetTickCount();
					}
				}

				// Doolin kwangilshin 2017. 08. 23.
				// if (TRUE == bMoveToReady && (GetTickCount() - sTime > 1000))
				if (TRUE == bMoveToReady)
				{
					bMoveToReady = FALSE;
					int iMoveAxisNo = i+1;


					iResult = pDlg->moveOriginReturnReadyPos(&sData, iStartAxisNo, iMoveAxisNo);
					if (iResult)
					{
						sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
						sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
						sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;

						for (int k = sData.iStartAxisNo; k <= sData.iEndAxisNo; k++)
							pDlg->m_plnkAxis[k]->Stop();
						
						pDlg->m_plnkAxis[i]->HomingStop();
						return 0;
					}
					bNextAxisMoveStart = TRUE;
				}
			}
			
			if (pDlg->m_plnkAxis[i]->IsOriginReturn())
			{
				iResult = pDlg->moveOriginReturnSafePos(&sData, iEndAxisNo, i);
				if (iResult)
				{
					sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
					sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;

					for (int k = sData.iStartAxisNo; k <= sData.iEndAxisNo; k++)
						pDlg->m_plnkAxis[k]->Stop();
					
					pDlg->m_plnkAxis[i]->HomingStop();
					return 0;
				}

				break;
			}
		} //while (TRUE)	
	} //for (int i = pData->iStartAxisNo; i <= pData->iEndAxisNo; i++)

	int iStartAxisSet = 0;
	if(sData.iOriginGroupNo == DEF_ORIGIN_GROUP_TAB_MOUNTER_X)
		iStartAxisSet = DEF_AXIS_SERCOS_MOUNTER1_X; 
	else if(sData.iOriginGroupNo == DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X)
		iStartAxisSet = DEF_AXIS_SERCOS_R_MOUNTER1_X; 
	else if (sData.iOriginGroupNo == DEF_ORIGIN_GROUP_F_TABCARRIER_X)
		iStartAxisSet = DEF_AXIS_SERCOS_TABIC_CARRIER1_X;
	else if (sData.iOriginGroupNo == DEF_ORIGIN_GROUP_R_TABCARRIER_X)
		iStartAxisSet = DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X; 
	else if (sData.iOriginGroupNo == DEF_ORIGIN_GROUP_INSPECTION_X)
		iStartAxisSet = DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X; 
	else if (sData.iOriginGroupNo == DEF_ORIGIN_GROUP_R_INSPECTION_X)
		iStartAxisSet = DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X; 
	else
		iStartAxisSet = DEF_AXIS_SERCOS_MOUNTER1_X; 

	iResult = pDlg->m_plnkAxis[iStartAxisSet]->Wait4Done();
	if (iResult)
	{
		sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
		sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
		return 0;
	}
	
	iPriority = pDlg->m_plnkAxis[iStartAxisSet]->GetOriginPriority();
	while (TRUE)
	{
		iResult = pDlg->doOriginReturnSingleAxis(&sData, iStartAxisSet, iPriority);
		if (iResult)
		{
			sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
			sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
			return 0;
		}
		
		if (DEF_ORIGIN_FINISH_STEP == pDlg->m_plnkAxis[iStartAxisSet]->GetOriginStep())
			break;

		Sleep(5);
	} //while (TRUE)

	sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;

	return 0;
}

int CAutoMotorOriginDlg::moveOriginReturnReadyPos(SOriginGroupData* pData, int iRefAxisNo, int iMoveAxisNo)
{	
	ASSERT(DEF_ORIGIN_GROUP_TAB_MOUNTER_X == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_INSPECTION_X == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_R_INSPECTION_X == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_F_TABCARRIER_X == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_R_TABCARRIER_X == pData->iOriginGroupNo);

	int		iResult				= ERR_DLG_ORIGIN_SUCCESS;
	int		iOffsetMultiplier	= 0;
	double	dMoveDist			= 0.0;

	double	dVelocity			= 0.0;
	int		iAccel				= 0;
	int		iDecel				= 0;


	if (DEF_ORIGIN_GROUP_TAB_MOUNTER_X == pData->iOriginGroupNo)
	{
		iOffsetMultiplier = iMoveAxisNo - iRefAxisNo - 1;
		for (int i = 0; i < iOffsetMultiplier; i++)
			dMoveDist += DEF_TABMOUNTER_SAFE_GAP*0.8;
		dMoveDist = m_rgdTabMounterPrepareMoveDistance[DEF_FRONT_GROUP] - dMoveDist;
	}
	else if (DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X == pData->iOriginGroupNo)
	{
		iOffsetMultiplier = iMoveAxisNo - iRefAxisNo - 1;
		for (int i = 0; i < iOffsetMultiplier; i++)
			dMoveDist += DEF_TABMOUNTER_SAFE_GAP*0.8;
		dMoveDist = m_rgdTabMounterPrepareMoveDistance[DEF_REAR_GROUP] - dMoveDist;
	}
	else if (DEF_ORIGIN_GROUP_F_TABCARRIER_X == pData->iOriginGroupNo)
	{
		iOffsetMultiplier = iMoveAxisNo - iRefAxisNo - 1;
		for (int i = 0; i < iOffsetMultiplier; i++)
			dMoveDist += DEF_TABCARRIER_SAFE_GAP*0.8;
		dMoveDist = m_rgdTabCarrierPrepareMoveDistance[DEF_FRONT_GROUP] - dMoveDist;
	}
	else if (DEF_ORIGIN_GROUP_R_TABCARRIER_X == pData->iOriginGroupNo)
	{
		iOffsetMultiplier = iMoveAxisNo - iRefAxisNo - 1;
		for (int i = 0; i < iOffsetMultiplier; i++)
			dMoveDist += DEF_TABCARRIER_SAFE_GAP*0.8;
		dMoveDist = m_rgdTabCarrierPrepareMoveDistance[DEF_REAR_GROUP] - dMoveDist;
	}
	else if (DEF_ORIGIN_GROUP_INSPECTION_X == pData->iOriginGroupNo)
	{
		iOffsetMultiplier = iMoveAxisNo - iRefAxisNo - 2;
		for (int i = 0; i < iOffsetMultiplier; i++)
			dMoveDist += DEF_INSPECTION_SAFE_GAP*0.8;
		dMoveDist = m_rgdInspectionPrepareMoveDistance[DEF_FRONT_GROUP] - dMoveDist;
	}
	else if (DEF_ORIGIN_GROUP_R_INSPECTION_X == pData->iOriginGroupNo)
	{
		iOffsetMultiplier = iMoveAxisNo - iRefAxisNo - 2;
		for (int i = 0; i < iOffsetMultiplier; i++)
			dMoveDist += DEF_INSPECTION_SAFE_GAP*0.8;
		dMoveDist = m_rgdInspectionPrepareMoveDistance[DEF_REAR_GROUP] - dMoveDist;
	}
	else
	{
		return ERR_DLG_ORIGIN_SUCCESS;
	}

	if (!m_plnkAxis[iMoveAxisNo]->IsAmpEnable() || m_plnkAxis[iMoveAxisNo]->IsAmpFault() || m_plnkAxis[iMoveAxisNo]->IsEStopEvent())
	{
		if ((iResult = m_plnkAxis[iMoveAxisNo]->ServoOn(NULL, TRUE)) != ERR_MOTION_SUCCESS)
		{
			return iResult;
		}

		Sleep(500);
	}
	
	if (m_plnkAxis[iMoveAxisNo]->IsInMotion())
	{
		if ((iResult = m_plnkAxis[iMoveAxisNo]->Stop()) != ERR_MOTION_SUCCESS)
		{
			return iResult;
		}
	}

	dVelocity		= m_plnkAxis[iMoveAxisNo]->GetSlowMovingVelocity();
	iAccel = iDecel = m_plnkAxis[iMoveAxisNo]->GetMovingAccelerate();

	if ((iResult = m_plnkAxis[iMoveAxisNo]->StartRMove(dMoveDist, dVelocity, iAccel, iDecel)) != ERR_DLG_ORIGIN_SUCCESS)
	{
		return iResult;
	}

	m_plnkAxis[iMoveAxisNo]->SetPositiveSensorEvent(DEF_E_STOP_EVENT);

	return ERR_DLG_ORIGIN_SUCCESS;
}

int CAutoMotorOriginDlg::moveOriginReturnSafePos(SOriginGroupData* pData, int iRefAxisNo, int iMoveAxisNo)
{
#ifndef SIMULATION
	int		i			= 0;
	double	dTarget		= 0.0;
	double	dGapMargin	= 10.0;	// 이 값은 일단 fix!
	double	dHomeOffset = m_plnkAxis[iMoveAxisNo]->GetHomeOffset();
	
	m_plnkAxis[iMoveAxisNo]->SetCurrentPosition(-dHomeOffset);

	int iOffsetMultiplier = iRefAxisNo - iMoveAxisNo;
	if(pData->iOriginGroupNo == DEF_ORIGIN_GROUP_F_TABCARRIER_X)
	{
		for (int i = 0; i<iOffsetMultiplier; i++)
		{
			dTarget -= (DEF_TABCARRIER_STANDARD_GAP + dGapMargin);
		}
	}
	else if(pData->iOriginGroupNo == DEF_ORIGIN_GROUP_R_TABCARRIER_X)
	{
		for (int i = 0; i<iOffsetMultiplier; i++)
		{
			dTarget -= (DEF_TABCARRIER_STANDARD_GAP + dGapMargin);
		}
	}
	else if(pData->iOriginGroupNo == DEF_ORIGIN_GROUP_INSPECTION_X)
	{
		dGapMargin = 3.0;
		for (int i = 0; i<iOffsetMultiplier; i++)
		{
			dTarget -= (DEF_INSPECTION_STANDARD_GAP + dGapMargin);
		}
//		dTarget -= 10.0;
	}		
	else if(pData->iOriginGroupNo == DEF_ORIGIN_GROUP_R_INSPECTION_X)
	{
		dGapMargin = 3.0;
		for (int i = 0; i<iOffsetMultiplier; i++)
		{
			dTarget -= (DEF_INSPECTION_STANDARD_GAP + dGapMargin);
		}
//		dTarget -= 10.0;
	}		
	else if(pData->iOriginGroupNo == DEF_ORIGIN_GROUP_TAB_MOUNTER_X)
	{
		for (int i = 0; i<iOffsetMultiplier; i++)
		{
			dTarget -= (DEF_TABMOUNTER_STANDARD_GAP + dGapMargin);
		}
	}
	else if(pData->iOriginGroupNo == DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X)
	{
		for (int i = 0; i<iOffsetMultiplier; i++)
		{
			dTarget -= (DEF_TABMOUNTER_STANDARD_GAP + dGapMargin);
		}
	}
	else
	{
//		TRACE(_T("TEST move Origin Return Safe Pos"));
		;
	}

	int iResult = m_plnkAxis[iMoveAxisNo]->StartRMove(dTarget);
	if (iResult)
	{
		m_plnkAxis[iMoveAxisNo]->Stop();
		return iResult;
	}

#endif
	return ERR_DLG_ORIGIN_SUCCESS;
}

int CAutoMotorOriginDlg::getPreMoveDistance(SOriginGroupData* pData)
{
	ASSERT(DEF_ORIGIN_GROUP_TAB_MOUNTER_X  == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X  == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_INSPECTION_X   == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_R_INSPECTION_X == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_F_TABCARRIER_X == pData->iOriginGroupNo
		|| DEF_ORIGIN_GROUP_R_TABCARRIER_X == pData->iOriginGroupNo);

	int iResult;
	int iWorkAxisID				= pData->iStartAxisNo;
	int iRefHomeSensorAxisID	= iWorkAxisID;
	
	if (!m_plnkAxis[iWorkAxisID]->IsAmpEnable() || m_plnkAxis[iWorkAxisID]->IsAmpFault() || m_plnkAxis[iWorkAxisID]->IsEStopEvent())
	{
		if ((iResult = m_plnkAxis[iWorkAxisID]->ServoOn(NULL, TRUE)) != ERR_MOTION_SUCCESS)
		{
			return iResult;
		}

		Sleep(500);
	}
	
	if (m_plnkAxis[iWorkAxisID]->IsInMotion())
	{
		if ((iResult = m_plnkAxis[iWorkAxisID]->Stop()) != ERR_MOTION_SUCCESS)
		{
			return iResult;
		}
	}

	double	dCurrentPos			= m_plnkAxis[iWorkAxisID]->GetCurrentPosition();
	double	dVelocity			= m_plnkAxis[iWorkAxisID]->GetCoarseVelocity() * 2;
	int		iAccel				= m_plnkAxis[iWorkAxisID]->GetCoarseAccelerate();
	BOOL	bOriginDir			= m_plnkAxis[iWorkAxisID]->GetOriginDir();
	double	dOriginLimitTime	= m_plnkAxis[iWorkAxisID]->GetOriginWaitTime();

	iResult = m_plnkAxis[iWorkAxisID]->VMove(dVelocity, iAccel, bOriginDir);
	if (iResult)
		return iResult;

	MTickTimer timerLimit;
	timerLimit.StartTimer();
	while (TRUE)
	{
		if (TRUE == m_plnkAxis[iRefHomeSensorAxisID]->IsOnHomeSensor())
		{
			iResult = m_plnkAxis[iWorkAxisID]->VStop();
			if (iResult)
				return iResult;

			break;
		}

		Sleep(10);

		if (TRUE == timerLimit.MoreThan(dOriginLimitTime))
		{
			m_plnkAxis[iWorkAxisID]->VStop();
			return ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
		}
		else if (Check_IsOPPanelSwitch(pData->iOriginGroupNo))
		{
			return ERR_DLG_ORIGIN_FAIL;
		}

		iResult = checkOnOrigin(iWorkAxisID);
		if (iResult)
		{
			m_plnkAxis[iWorkAxisID]->VStop();
			return iResult;
		}
	}

	double dAfterMovePos = m_plnkAxis[iWorkAxisID]->GetCurrentPosition();

	/*
	const double DEF_SAFETY_DISTANCE_FOR_LINEAR_MARKERS = 80.0;
	double dMoveDist = dAfterMovePos - dCurrentPos;
	if (TRUE == bOriginDir)	//  양의 방향으로 이동한 경우
		dMoveDist -= DEF_SAFETY_DISTANCE_FOR_LINEAR_MARKERS;
	else
		dMoveDist += DEF_SAFETY_DISTANCE_FOR_LINEAR_MARKERS;

	if (DEF_ORIGIN_GROUP_TAB_MOUNTER_F == pData->iOriginGroupNo)
		m_rgdTabMounterPrepareMoveDistance[DEF_FRONT_GROUP] = dMoveDist;
	else // if (DEF_ORIGIN_GROUP_TAB_MOUNTER_R == pData->iOriginGroupNo)
		m_rgdTabMounterPrepareMoveDistance[DEF_REAR_GROUP] = dMoveDist;
	*/
	double dSafetyDistForLinearMarkers = 80.0;
	double dMoveDist = dAfterMovePos - dCurrentPos;
	if (TRUE == bOriginDir)	//  양의 방향으로 이동한 경우
		dMoveDist -= dSafetyDistForLinearMarkers;
	else
		dMoveDist += dSafetyDistForLinearMarkers;

	if (DEF_ORIGIN_GROUP_TAB_MOUNTER_X == pData->iOriginGroupNo)
		m_rgdTabMounterPrepareMoveDistance[DEF_FRONT_GROUP] = dMoveDist;
	else if (DEF_ORIGIN_GROUP_R_TAB_MOUNTER_X == pData->iOriginGroupNo)
		m_rgdTabMounterPrepareMoveDistance[DEF_REAR_GROUP] = dMoveDist;
	else if (DEF_ORIGIN_GROUP_INSPECTION_X == pData->iOriginGroupNo)
		m_rgdInspectionPrepareMoveDistance[DEF_FRONT_GROUP] = dMoveDist;
	else if (DEF_ORIGIN_GROUP_R_INSPECTION_X == pData->iOriginGroupNo)
		m_rgdInspectionPrepareMoveDistance[DEF_REAR_GROUP] = dMoveDist;
	else if (DEF_ORIGIN_GROUP_F_TABCARRIER_X == pData->iOriginGroupNo)
		m_rgdTabCarrierPrepareMoveDistance[DEF_FRONT_GROUP] = dMoveDist;
	else if (DEF_ORIGIN_GROUP_R_TABCARRIER_X == pData->iOriginGroupNo)
		m_rgdTabCarrierPrepareMoveDistance[DEF_REAR_GROUP] = dMoveDist;

	return ERR_DLG_ORIGIN_SUCCESS;
}

// Doolin kwangilshin 2017. 08. 26.
// Tact 개선.
//
int CAutoMotorOriginDlg::moveAxisToPlusLimitPos(SOriginGroupData* pData)
{
	int			iResult = ERR_DLG_ORIGIN_SUCCESS;
	
	double		dVelocity			= m_plnkAxis[pData->iStartAxisNo]->GetCoarseVelocity() * 2;
	int			iAccel				= m_plnkAxis[pData->iStartAxisNo]->GetCoarseAccelerate();

	BOOL		bAllComplete		= TRUE;
	double		dbTgtCapDist		= 0.0;

	MTickTimer	clsTimer;

	for (int nAxisIdx = pData->iEndAxisNo; nAxisIdx >= pData->iStartAxisNo; nAxisIdx--)
	{
		m_plnkAxis[nAxisIdx]->SetNegativeSensorEvent(DEF_NO_EVENT);

		dbTgtCapDist = m_plnkAxis[nAxisIdx]->GetCurrentPosition() + DEF_MOVE_CAP;
		clsTimer.StartTimer();
		
		if (!m_plnkAxis[nAxisIdx]->IsAmpEnable() || m_plnkAxis[nAxisIdx]->IsAmpFault() || m_plnkAxis[nAxisIdx]->IsEStopEvent())
		{
			if ((iResult = m_plnkAxis[nAxisIdx]->ServoOn(NULL, TRUE)) != ERR_MOTION_SUCCESS)
			{
				return iResult;
			}

			Sleep(500);
		}
		
		if (m_plnkAxis[nAxisIdx]->IsInMotion())
		{
			if ((iResult = m_plnkAxis[nAxisIdx]->Stop()) != ERR_MOTION_SUCCESS)
			{
				return iResult;
			}
		}

		if (!m_plnkAxis[nAxisIdx]->IsOnPositiveSensor())
		{
			if ((iResult = m_plnkAxis[nAxisIdx]->VMove(dVelocity, iAccel)) != ERR_DLG_ORIGIN_SUCCESS)
			{
				return iResult;
			}
		}

		while (1)
		{
			if (m_plnkAxis[nAxisIdx]->IsAmpFault() || m_plnkAxis[nAxisIdx]->IsEStopEvent())
			{
				return ERR_DLG_ORIGIN_FAIL;
			}
			else if (!m_plnkAxis[nAxisIdx]->IsInMotion())
			{
				if (m_plnkAxis[nAxisIdx]->IsOnPositiveSensor())
				{
					break;
				}
				else if (clsTimer.MoreThan(2))
				{
					return ERR_DLG_ORIGIN_FAIL;
				}
			}
			else if (Check_IsOPPanelSwitch(pData->iOriginGroupNo))
			{
				return ERR_DLG_ORIGIN_FAIL;
			}
			else if (fabs(dbTgtCapDist - m_plnkAxis[nAxisIdx]->GetCurrentPosition()) >= DEF_MOVE_CAP)
			{
				break;
			}
			else if (clsTimer.MoreThan(5))
			{
				return ERR_DLG_ORIGIN_FAIL;
			}
		}
	}
		
	while (1)
	{
		bAllComplete = TRUE;

		for (int nAxisIdx = pData->iEndAxisNo; nAxisIdx >= pData->iStartAxisNo; nAxisIdx--)
		{				
			if (m_plnkAxis[nAxisIdx]->IsAmpFault() || m_plnkAxis[nAxisIdx]->IsEStopEvent())
			{
				if ((iResult = m_plnkAxis[nAxisIdx]->ServoOn(NULL, TRUE)) != ERR_MOTION_SUCCESS)
				{
					return iResult;
				}
			}
			
			if (m_plnkAxis[nAxisIdx]->IsAmpFault() || m_plnkAxis[nAxisIdx]->IsEStopEvent())
			{
				return ERR_DLG_ORIGIN_FAIL;
			}
			else if (!m_plnkAxis[nAxisIdx]->IsOnPositiveSensor() || !m_plnkAxis[nAxisIdx]->IsMotionDone())
			{
				bAllComplete = FALSE;
			}
			else if (Check_IsOPPanelSwitch(pData->iOriginGroupNo))
			{
				return ERR_DLG_ORIGIN_FAIL;
			}
			else if (clsTimer.MoreThan(20))
			{
				return ERR_DLG_ORIGIN_FAIL;
			}
		}

		if (bAllComplete)
		{
			break;
		}
	}

	return SUCCESS;
}

UINT CAutoMotorOriginDlg::returnOriginNormalAxis(LPVOID pParam)
{
	SOriginGroupData sData;
	CAutoMotorOriginDlg* pDlg = (CAutoMotorOriginDlg*)((SOriginGroupData*)pParam)->pThisDlg;
	pDlg->setData((SOriginGroupData*)pParam, &sData);

	double dCarrierXSafePos = 0.0;
	if (DEF_ORIGIN_GROUP_F_TABCARRIER_T == sData.iOriginGroupNo)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			BOOL allXOrigin = TRUE;
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				if (pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER1_T+i]
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER1_T+i])
					allXOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_TABIC_CARRIER1_Y+i]->IsOriginReturn();
			}
			if (TRUE == allXOrigin)
				break;
			if (GetTickCount() - sTime > 120*1000)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			// 원점 복귀 도중 비상정지 입력
			if (pDlg->m_plnkManageOpPanel->GetEStopSWStatus())
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_DETECT_E_STOP;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			// 원점 복귀 도중 Step Stop switch 입력
			if (pDlg->m_plnkManageOpPanel->GetStopSWStatus())
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_DETECT_USER_STOP;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
		}
		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{
			if (pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_CARRIER1_T+i]
				&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_CARRIER1_T+i])
			{
				dCarrierXSafePos = MOlbSystemPre.GetTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
				int iResult = MOlbSystemPre.GetTabCarrierComponent(i)->MoveYPos(dCarrierXSafePos);
				if (iResult)
				{
					sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
					sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
					return 0;
				}
			}
		}
	}


	else if (DEF_ORIGIN_GROUP_R_TABCARRIER_T == sData.iOriginGroupNo)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			BOOL allXOrigin = TRUE;
			for (int i=0; i<DEF_MAX_TABCARRIER; i++)
			{
				if (pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T+i] 
					&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T+i])
					allXOrigin &= pDlg->m_plnkAxis[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y+i]->IsOriginReturn();
			}
			if (TRUE == allXOrigin)
				break;
			if (GetTickCount() - sTime > 120*1000)
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_LIMIT_TIME_OVER;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			// 원점 복귀 도중 비상정지 입력
			if (pDlg->m_plnkManageOpPanel->GetEStopSWStatus())
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_DETECT_E_STOP;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
			// 원점 복귀 도중 Step Stop switch 입력
			if (pDlg->m_plnkManageOpPanel->GetStopSWStatus())
			{
				sm_rgiGroupResult[sData.iOriginGroupNo] = ERR_DLG_ORIGIN_DETECT_USER_STOP;
				sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
				return 0;
			}
		}
		for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		{
			if (pDlg->m_bAxisSelectFlag[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T+i] 
				&& pDlg->m_rgbForceOriginReturn[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T+i])
			{
				dCarrierXSafePos = MOlbSystemPre.GetRTabCarrierComponent(i)->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
				int iResult = MOlbSystemPre.GetRTabCarrierComponent(i)->MoveYPos(dCarrierXSafePos);
				if (iResult)
				{
					sm_rgiGroupResult[sData.iOriginGroupNo] = iResult;
					sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
					return 0;
				}
			}
		}
	}

	sm_rgiGroupResult[sData.iOriginGroupNo] = pDlg->doOriginReturnNormal(&sData);

	sm_rgiGroupComplete[sData.iOriginGroupNo] = TRUE;
	return 0;
}

void CAutoMotorOriginDlg::setData(SOriginGroupData* pSrc, SOriginGroupData* pDst)
{
	pDst->prgiOriginMoveType = pSrc->prgiOriginMoveType;
	pDst->iOriginGroupNo = pSrc->iOriginGroupNo;
	pDst->bGetPreMoveDist = pSrc->bGetPreMoveDist;
	pDst->iStartAxisNo = pSrc->iStartAxisNo;
	pDst->iEndAxisNo = pSrc->iEndAxisNo;
}

int CAutoMotorOriginDlg::doOriginReturnNormal(SOriginGroupData* pData)
{
	int iResult;

	for (int iPriority = 0; iPriority < DEF_AXIS_MAX_PRIORITY; iPriority++) 
	{
		while (TRUE)
		{
			for (int i = pData->iStartAxisNo; i <= pData->iEndAxisNo; i++)
			{
				if (TRUE == MOlbSystemPre.IsSkipAxis(i) || !m_bAxisSelectFlag[i])
				{
					continue;
				}

				iResult = doOriginReturnSingleAxis(pData, i, iPriority);
				
				if (iResult)
				{
					return iResult;	
				}
			} 
		
			if (TRUE == isAxisGroupOriginComplete(pData, iPriority))
			{
				break;
			}

			if (Check_IsOPPanelSwitch(pData->iOriginGroupNo))
			{
				return ERR_MOTION_FAIL;
			}

			Sleep(5);
		} 
	}

	return SUCCESS;
}

int CAutoMotorOriginDlg::doOriginReturnSingleAxis(SOriginGroupData* pData, int iAxisNo, int iPriority)
{
	if (m_plnkAxis[iAxisNo] == NULL)
		return SUCCESS;
	
	int iResult=0;

	if (m_bAxisSelectFlag[iAxisNo] && m_plnkAxis[iAxisNo]->GetOriginPriority() == iPriority)
	{
#ifndef SIMULATION
		// 원점 복귀 동작 중에 계속 확인 해야하는 부분 처리
		iResult = checkOnOrigin(iAxisNo);
		if (iResult)
		{
			int j;
			for (j = pData->iStartAxisNo; j <= pData->iEndAxisNo; j++)
			{
				if (m_bAxisSelectFlag[j] && FALSE == m_plnkAxis[j]->IsOriginReturn())
					m_plnkAxis[j]->Stop();
			}
			
			for (j = pData->iStartAxisNo; j <= pData->iEndAxisNo; j++)
			{
				if (m_bAxisSelectFlag[j] && FALSE == m_plnkAxis[j]->IsOriginReturn())
					m_plnkAxis[j]->HomingStop();
			}
			return iResult;
		}
#endif /* SIMULATION */
		
		m_plnkAxis[iAxisNo]->OriginReturn(pData->prgiOriginMoveType[iAxisNo]);

		//170922 JSH.s
		//완료 스텝 되었을때
		if((m_plnkAxis[iAxisNo]->GetOriginStep() == DEF_ORIGIN_FINISH_STEP))
		{
			iResult = checkCompleteOrigin(iAxisNo);
			if (iResult)
			{
				int j;
				for (j = pData->iStartAxisNo; j <= pData->iEndAxisNo; j++)
				{
					if (m_bAxisSelectFlag[j] && FALSE == m_plnkAxis[j]->IsOriginReturn())
						m_plnkAxis[j]->Stop();
				}
				
				for (j = pData->iStartAxisNo; j <= pData->iEndAxisNo; j++)
				{
					if (m_bAxisSelectFlag[j] && FALSE == m_plnkAxis[j]->IsOriginReturn())
						m_plnkAxis[j]->HomingStop();
				}
				return iResult;
			}
		}
		//170922 JSh.e
	}

	return SUCCESS;
}

void CAutoMotorOriginDlg::changeDisplayForceOriginButton(BOOL bState)
{
	if (TRUE == bState)
	{
		m_btnForceOrigin.SetForeColor(RED);
		m_btnForceOrigin.SetForeColorDisabled(RED);
		m_btnForceOrigin.SetBackColor(RED);
	}
	else
	{
		m_btnForceOrigin.SetForeColor(BLACK);
		m_btnForceOrigin.SetForeColorDisabled(BLACK);
		m_btnForceOrigin.SetBackColor(RGB(0, 150, 200));
	}
}

int CAutoMotorOriginDlg::DlgID2AxisID(int iDialogID)
{
	int iAxisID = -1;
	switch (iDialogID)
	{
		case 40:    iAxisID = DEF_AXIS_SERCOS_BADTAB_DETECTOR_X;	break;
		case 41:	iAxisID = DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T;	break;
		case 42:	iAxisID = DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T;	break; //75
		case 43:	iAxisID = DEF_AXIS_SERCOS_TAB_PRESS_Z;			break;
		case 44:	iAxisID = DEF_AXIS_SERCOS_FEEDER_REEL_T;		break;
		case 45:	iAxisID = DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z;		break;
		case 46:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER1_X;		break;
		case 47:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER2_X;		break;
		case 48:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER3_X;		break;
		case 49:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER4_X;		break;
		case 50:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER1_Y;		break;
		case 51:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER2_Y;		break;
		case 52:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER3_Y;		break;
		case 53:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER4_Y;		break;
		case 54:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER1_T;		break;
		case 55:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER2_T;		break;
		case 56:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER3_T;		break;
		case 57:	iAxisID = DEF_AXIS_SERCOS_TABIC_CARRIER4_T;		break;


		case 58:	iAxisID = DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X;	break;
		case 59:	iAxisID = DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T;	break;
		case 60:	iAxisID = DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T;	break;
		case 61:	iAxisID = DEF_AXIS_SERCOS_R_TAB_PRESS_Z;		break;
		case 62:	iAxisID = DEF_AXIS_SERCOS_R_FEEDER_REEL_T;		break;
		case 63:	iAxisID = DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z;	break; //

		case 64:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X;	break;
		case 65:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X;	break;
		case 66:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X;	break;
		case 67:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X;	break;
		case 68:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y;	break;
		case 69:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y;	break;
		case 70:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y;	break;
		case 71:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y;	break;
		case 72:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T;	break;
		case 73:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T;	break;
		case 74:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T;	break;
		case 75:	iAxisID = DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T;	break; //67
		case 76:	iAxisID = DEF_AXIS_ACS_GANTRY_Y;				break;
		case 77:	iAxisID = DEF_AXIS_ACS_R_GANTRY_Y;				break;

		case 78:	iAxisID = DEF_AXIS_SERCOS_INSPECT_EXPAND1;		break;
		case 79:	iAxisID = DEF_AXIS_SERCOS_INSPECT_EXPAND2;		break;
		case 80:	iAxisID = DEF_AXIS_SERCOS_INSPECT_EXPAND3;		break;
		case 81:	iAxisID = DEF_AXIS_SERCOS_INSPECT_EXPAND4;		break;
		case 82:	iAxisID = DEF_AXIS_SERCOS_TABCAMERA_EXPAND;		break;
		case 83:	iAxisID = DEF_AXIS_SERCOS_R_INSPECT_EXPAND1;	break;
		case 84:	iAxisID = DEF_AXIS_SERCOS_R_INSPECT_EXPAND2;	break;
		case 85:	iAxisID = DEF_AXIS_SERCOS_R_INSPECT_EXPAND3;	break;
		case 86:	iAxisID = DEF_AXIS_SERCOS_R_INSPECT_EXPAND4;	break;
		case 87:	iAxisID = DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND;	break;
		case 88:	iAxisID = DEF_AXIS_SERCOS_TABCAMERA_EXPAND2;	break;
		case 89:	iAxisID = DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2;	break;
		case 90:    iAxisID = DEF_AXIS_SERCOS_TRANSFER_IN_2;		break; //SJ_YYK 161104 Add.
#ifdef DEF_USE_TRANSFER_CENTER
		case 91:	iAxisID = DEF_AXIS_SERCOS_TRANSFER_CENTER_1;	break; //SJ_YYK 161104 Add.
		case 92:	iAxisID = DEF_AXIS_SERCOS_TRANSFER_CENTER_2;	break; //SJ_YYK 161104 Add.
#else
		case 91:	iAxisID = DEF_AXIS_SERCOS_SPARE_76;				break; //SJ_YYK 161104 Add.
		case 92:	iAxisID = DEF_AXIS_SERCOS_SPARE_77;				break; //SJ_YYK 161104 Add.
#endif
#ifdef DEF_USE_TURN_HANDLER
		case 93:	iAxisID = DEF_AXIS_SERCOS_HANDLER_T;			break; //SJ_YYK 161104 Add.
		case 94:	iAxisID = DEF_AXIS_SERCOS_HANDLER_Z;			break; //SJ_YYK 161104 Add.
#else
		case 93:	iAxisID = DEF_AXIS_SERCOS_SPARE_78;				break; 
		case 94:	iAxisID = DEF_AXIS_SERCOS_SPARE_79;				break; 
#endif

#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
		case 95:	iAxisID = DEF_AXIS_SERCOS_TRANSFER_OUT_2;		break;
#else
		case 95:	iAxisID = DEF_AXIS_SERCOS_SPARE_81;		break;
#endif	
		
		default:	iAxisID = iDialogID;							break;
	}
	return iAxisID;
}

/* End of File */

void CAutoMotorOriginDlg::OnClickAxisOrigin88() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(88));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin89() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(89));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin90() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(90));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin91() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(91));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin92() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(92));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin93() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(93));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin94() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(94));
}

void CAutoMotorOriginDlg::OnClickAxisOrigin95() 
{
	// TODO: Add your control notification handler code here
	axisSelect(DlgID2AxisID(95));
}


// Doolin kwangilshin 2017. 08. 27.
//
BOOL CAutoMotorOriginDlg::Check_IsOPPanelSwitch(int nOrgGrpNo)	
{
	BOOL bResult = FALSE;

	if (m_plnkManageOpPanel->GetEStopSWStatus())
	{
		sm_rgiGroupResult[nOrgGrpNo]	= ERR_DLG_ORIGIN_DETECT_E_STOP;
		sm_rgiGroupComplete[nOrgGrpNo]	= TRUE;

		bResult = TRUE;
	}
	if (m_plnkManageOpPanel->GetStopSWStatus())
	{
		sm_rgiGroupResult[nOrgGrpNo]	= ERR_DLG_ORIGIN_DETECT_USER_STOP;
		sm_rgiGroupComplete[nOrgGrpNo]	= TRUE;

		bResult = TRUE;
	}

	
	return bResult;
}
// End.
