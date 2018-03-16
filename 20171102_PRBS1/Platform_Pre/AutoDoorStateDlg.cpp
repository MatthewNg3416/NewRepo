// AutoDoorStateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "AutoDoorStateDlg.h"
#include "IOpPanel.h"
#include "MPlatformOlbSystem.h"
#include "textdescriptor.h"
#include "DefIOAddrPreBonder.h"

extern MPlatformOlbSystem	MOlbSystemPre;

// CAutoDoorStateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoDoorStateDlg, CDialog)

CAutoDoorStateDlg::CAutoDoorStateDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAutoDoorStateDlg::IDD, pParent)
{  
	//초기화
	int i=0;
	for (int i = 0; i < DEF_MAX_DOOR_BTN ; i++)
	{
		m_bDoorStateFlg[i] = FALSE;
		m_strDoorName[i] = _T("");
	}

	m_strDoorName[0]			= _T("FRONT DOOR1");
	m_bDoorStateFlg[0]			= TRUE;
	m_uiDoorAddr[0]				= IN_FRONT_DOOR1;

	m_strDoorName[1]			= _T("FRONT DOOR2");
	m_bDoorStateFlg[1]			= TRUE;
	m_uiDoorAddr[1]				= IN_FRONT_DOOR2;

	m_strDoorName[2]			= _T("FRONT DOOR3");
	m_bDoorStateFlg[2]			= FALSE;
	m_uiDoorAddr[2]				= IN_FRONT_DOOR3;

	m_strDoorName[3]			= _T("FRONT DOOR4");
	m_bDoorStateFlg[3]			= FALSE;
	m_uiDoorAddr[3]				= IN_FRONT_DOOR4;

	m_strDoorName[4]			= _T("FRONT DOOR5");
	m_bDoorStateFlg[4]			= FALSE;
	m_uiDoorAddr[4]				= IN_FRONT_DOOR5;

	m_strDoorName[5]			= _T("FRONT DOOR6");
	m_bDoorStateFlg[5]			= FALSE;
	m_uiDoorAddr[5]				= IN_FRONT_DOOR6;

	m_strDoorName[8]			= _T("REAR DOOR1");
	m_bDoorStateFlg[8]			= TRUE;
	m_uiDoorAddr[8]				= IN_REAR_DOOR1;

	m_strDoorName[9]			= _T("REAR DOOR2");
	m_bDoorStateFlg[9]			= TRUE;
	m_uiDoorAddr[9]				= IN_REAR_DOOR2;

	m_strDoorName[10]			= _T("REAR DOOR3");
	m_bDoorStateFlg[10]			= FALSE;
	m_uiDoorAddr[10]				= IN_REAR_DOOR3;

	m_strDoorName[11]			= _T("REAR DOOR4");
	m_bDoorStateFlg[11]			= FALSE;
	m_uiDoorAddr[11]				= IN_REAR_DOOR4;

	m_strDoorName[12]			= _T("REAR DOOR5");
	m_bDoorStateFlg[12]			= FALSE;
	m_uiDoorAddr[12]				= IN_REAR_DOOR5;

	m_strDoorName[13]			= _T("REAR DOOR6");
	m_bDoorStateFlg[13]			= FALSE;
	m_uiDoorAddr[13]				= IN_REAR_DOOR6;



	//Key Display
	for (int i = 0; i < DEF_MAX_DOOR_KEY_BTN ; i++)
	{
		m_bKeyStateFlg[i] = FALSE;
		m_strKeyName[i] = _T("");
	}

	m_strKeyName[0]				= _T("Front Key");
	m_bKeyStateFlg[0]			= TRUE;
	m_uiKeyAddr[0]				= IN_SAFETY_KEY1_AUTO;

	m_strKeyName[1]				= _T("REAR Key");
	m_bKeyStateFlg[1]			= TRUE;
	m_uiKeyAddr[1]				= IN_SAFETY_KEY2_AUTO;

	m_strKeyName[2]				= _T("F Material Key");
	m_bKeyStateFlg[2]			= FALSE;
	m_uiKeyAddr[2]				= IN_SAFETY_KEY_F_MATERIAL;

	m_strKeyName[3]				= _T("R Material Key");
	m_bKeyStateFlg[3]			= FALSE;
	m_uiKeyAddr[3]				= IN_SAFETY_KEY_R_MATERIAL;
	//____________
}

CAutoDoorStateDlg::~CAutoDoorStateDlg()
{
}

void CAutoDoorStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOOR_STATE0, m_lblDoorState[0]);
	DDX_Control(pDX, IDC_DOOR_STATE1, m_lblDoorState[1]);
	DDX_Control(pDX, IDC_DOOR_STATE2, m_lblDoorState[2]);
	DDX_Control(pDX, IDC_DOOR_STATE3, m_lblDoorState[3]);
	DDX_Control(pDX, IDC_DOOR_STATE4, m_lblDoorState[4]);
	DDX_Control(pDX, IDC_DOOR_STATE5, m_lblDoorState[5]);
	DDX_Control(pDX, IDC_DOOR_STATE6, m_lblDoorState[6]);
	DDX_Control(pDX, IDC_DOOR_STATE7, m_lblDoorState[7]);
	DDX_Control(pDX, IDC_DOOR_STATE8, m_lblDoorState[8]);
	DDX_Control(pDX, IDC_DOOR_STATE9, m_lblDoorState[9]);
	DDX_Control(pDX, IDC_DOOR_STATE10, m_lblDoorState[10]);
	DDX_Control(pDX, IDC_DOOR_STATE11, m_lblDoorState[11]);
	DDX_Control(pDX, IDC_DOOR_STATE12, m_lblDoorState[12]);
	DDX_Control(pDX, IDC_DOOR_STATE13, m_lblDoorState[13]);
	DDX_Control(pDX, IDC_DOOR_STATE14, m_lblDoorState[14]);
	DDX_Control(pDX, IDC_DOOR_STATE15, m_lblDoorState[15]);

	DDX_Control(pDX, IDC_KEY_STATE0, m_lblKeyState[0]);
	DDX_Control(pDX, IDC_KEY_STATE1, m_lblKeyState[1]);
	DDX_Control(pDX, IDC_KEY_STATE2, m_lblKeyState[2]);
	DDX_Control(pDX, IDC_KEY_STATE3, m_lblKeyState[3]);
}


BEGIN_MESSAGE_MAP(CAutoDoorStateDlg, CDialog)
ON_WM_TIMER()
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAutoDoorStateDlg 메시지 처리기입니다.

void CAutoDoorStateDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_uiTimerID == nIDEvent)
	{
		int i = 0;
		for (int i=0; i < DEF_MAX_DOOR_BTN ; i++)
		{
			if (m_bDoorStateFlg[i])
			{

				if(MOlbSystemPre.GetIOComponent()->IsOn(m_uiDoorAddr[i]))
					m_lblDoorState[i].SetBackColorInterior(RGB(255,0,0));
				else
					m_lblDoorState[i].SetBackColorInterior(RGB(0,255,0));
			}
		}		

		for (int i=0; i < DEF_MAX_DOOR_KEY_BTN ; i++)
		{
			if (m_bKeyStateFlg[i])
			{

				if(MOlbSystemPre.GetIOComponent()->IsOn(m_uiKeyAddr[i]))
					m_lblKeyState[i].SetBackColorInterior(RGB(0,255,0));
				else
					m_lblKeyState[i].SetBackColorInterior(RGB(255,0,0));
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CAutoDoorStateDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialog::OnClose();
}

BEGIN_EVENTSINK_MAP(CAutoDoorStateDlg, CDialog)
	ON_EVENT(CAutoDoorStateDlg, IDB_EXIT, DISPID_CLICK, CAutoDoorStateDlg::ClickExit, VTS_NONE)
END_EVENTSINK_MAP()

void CAutoDoorStateDlg::ClickExit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(m_uiTimerID);
	OnOK();
}

BOOL CAutoDoorStateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_uiTimerID = SetTimer(1, 200, NULL);

	for (int i = 0; i < DEF_MAX_DOOR_BTN ; i++)
	{
		if (m_bDoorStateFlg[i])
		{
			m_lblDoorState[i].ShowWindow(SW_SHOW);
			m_lblDoorState[i].SetCaption(m_strDoorName[i]);
		}
		else
		{
			m_lblDoorState[i].ShowWindow(SW_HIDE);
		}
	}

	for (int i = 0; i < DEF_MAX_DOOR_KEY_BTN ; i++)
	{
		if (m_bKeyStateFlg[i])
		{
			m_lblKeyState[i].ShowWindow(SW_SHOW);
			m_lblKeyState[i].SetCaption(m_strDoorName[i]);
		}
		else
		{
			m_lblKeyState[i].ShowWindow(SW_HIDE);
		}
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
