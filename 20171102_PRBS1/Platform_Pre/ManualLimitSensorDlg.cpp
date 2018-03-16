// LimitSensorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "ManualLimitSensorDlg.h"
#include "MPlatformOlbSystem.h"
#include "IAxis.h"
#include "MCtrlTabFeeder.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualLimitSensorDlg dialog

extern	MPlatformOlbSystem	MOlbSystemPre;

CManualLimitSensorDlg::CManualLimitSensorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualLimitSensorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualLimitSensorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_iCurrentPage = 0;
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	for (int i = 0; i < DEF_LIMIT_SENSOR_MAX_MOTION_NUM; i++)
	{
		m_plnkAxis[i] = MOlbSystemPre.GetMotionComponent(i);
	}
}


void CManualLimitSensorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualLimitSensorDlg)
	DDX_Control(pDX, IDB_NEXT, m_bNext);
	DDX_Control(pDX, IDB_PREVIOUS, m_bPrev);
	//}}AFX_DATA_MAP
		
	DDX_Control(pDX, IDB_SERVO_ON_0, m_bServoOn[0]);
	DDX_Control(pDX, IDB_SERVO_ON_1, m_bServoOn[1]);
	DDX_Control(pDX, IDB_SERVO_ON_2, m_bServoOn[2]);
	DDX_Control(pDX, IDB_SERVO_ON_3, m_bServoOn[3]);
	DDX_Control(pDX, IDB_SERVO_ON_4, m_bServoOn[4]);
	DDX_Control(pDX, IDB_SERVO_ON_5, m_bServoOn[5]);
	DDX_Control(pDX, IDB_SERVO_ON_6, m_bServoOn[6]);
	DDX_Control(pDX, IDB_SERVO_ON_7, m_bServoOn[7]);
	DDX_Control(pDX, IDB_SERVO_ON_8, m_bServoOn[8]);
	DDX_Control(pDX, IDB_SERVO_ON_9, m_bServoOn[9]);
	DDX_Control(pDX, IDB_SERVO_ON_10, m_bServoOn[10]);
	DDX_Control(pDX, IDB_SERVO_ON_11, m_bServoOn[11]);
	DDX_Control(pDX, IDB_SERVO_ON_12, m_bServoOn[12]);
	DDX_Control(pDX, IDB_SERVO_ON_13, m_bServoOn[13]);
	DDX_Control(pDX, IDB_SERVO_ON_14, m_bServoOn[14]);
	DDX_Control(pDX, IDB_SERVO_ON_15, m_bServoOn[15]);

	DDX_Control(pDX, IDB_SERVO_OFF_0, m_bServoOff[0]);
	DDX_Control(pDX, IDB_SERVO_OFF_1, m_bServoOff[1]);
	DDX_Control(pDX, IDB_SERVO_OFF_2, m_bServoOff[2]);
	DDX_Control(pDX, IDB_SERVO_OFF_3, m_bServoOff[3]);
	DDX_Control(pDX, IDB_SERVO_OFF_4, m_bServoOff[4]);
	DDX_Control(pDX, IDB_SERVO_OFF_5, m_bServoOff[5]);
	DDX_Control(pDX, IDB_SERVO_OFF_6, m_bServoOff[6]);
	DDX_Control(pDX, IDB_SERVO_OFF_7, m_bServoOff[7]);
	DDX_Control(pDX, IDB_SERVO_OFF_8, m_bServoOff[8]);
	DDX_Control(pDX, IDB_SERVO_OFF_9, m_bServoOff[9]);
	DDX_Control(pDX, IDB_SERVO_OFF_10, m_bServoOff[10]);
	DDX_Control(pDX, IDB_SERVO_OFF_11, m_bServoOff[11]);
	DDX_Control(pDX, IDB_SERVO_OFF_12, m_bServoOff[12]);
	DDX_Control(pDX, IDB_SERVO_OFF_13, m_bServoOff[13]);
	DDX_Control(pDX, IDB_SERVO_OFF_14, m_bServoOff[14]);
	DDX_Control(pDX, IDB_SERVO_OFF_15, m_bServoOff[15]);

	DDX_Control(pDX, IDS_MTR_NAME_0, m_lAxisName[0]);
	DDX_Control(pDX, IDS_MTR_NAME_1, m_lAxisName[1]);
	DDX_Control(pDX, IDS_MTR_NAME_2, m_lAxisName[2]);
	DDX_Control(pDX, IDS_MTR_NAME_3, m_lAxisName[3]);
	DDX_Control(pDX, IDS_MTR_NAME_4, m_lAxisName[4]);
	DDX_Control(pDX, IDS_MTR_NAME_5, m_lAxisName[5]);
	DDX_Control(pDX, IDS_MTR_NAME_6, m_lAxisName[6]);
	DDX_Control(pDX, IDS_MTR_NAME_7, m_lAxisName[7]);
	DDX_Control(pDX, IDS_MTR_NAME_8, m_lAxisName[8]);
	DDX_Control(pDX, IDS_MTR_NAME_9, m_lAxisName[9]);
	DDX_Control(pDX, IDS_MTR_NAME_10, m_lAxisName[10]);
	DDX_Control(pDX, IDS_MTR_NAME_11, m_lAxisName[11]);
	DDX_Control(pDX, IDS_MTR_NAME_12, m_lAxisName[12]);
	DDX_Control(pDX, IDS_MTR_NAME_13, m_lAxisName[13]);
	DDX_Control(pDX, IDS_MTR_NAME_14, m_lAxisName[14]);
	DDX_Control(pDX, IDS_MTR_NAME_15, m_lAxisName[15]);

	DDX_Control(pDX, IDS_NEG_STATUS_0, m_lNegSensor[0]);
	DDX_Control(pDX, IDS_NEG_STATUS_1, m_lNegSensor[1]);
	DDX_Control(pDX, IDS_NEG_STATUS_2, m_lNegSensor[2]);
	DDX_Control(pDX, IDS_NEG_STATUS_3, m_lNegSensor[3]);
	DDX_Control(pDX, IDS_NEG_STATUS_4, m_lNegSensor[4]);
	DDX_Control(pDX, IDS_NEG_STATUS_5, m_lNegSensor[5]);
	DDX_Control(pDX, IDS_NEG_STATUS_6, m_lNegSensor[6]);
	DDX_Control(pDX, IDS_NEG_STATUS_7, m_lNegSensor[7]);
	DDX_Control(pDX, IDS_NEG_STATUS_8, m_lNegSensor[8]);
	DDX_Control(pDX, IDS_NEG_STATUS_9, m_lNegSensor[9]);
	DDX_Control(pDX, IDS_NEG_STATUS_10, m_lNegSensor[10]);
	DDX_Control(pDX, IDS_NEG_STATUS_11, m_lNegSensor[11]);
	DDX_Control(pDX, IDS_NEG_STATUS_12, m_lNegSensor[12]);
	DDX_Control(pDX, IDS_NEG_STATUS_13, m_lNegSensor[13]);
	DDX_Control(pDX, IDS_NEG_STATUS_14, m_lNegSensor[14]);
	DDX_Control(pDX, IDS_NEG_STATUS_15, m_lNegSensor[15]);

	DDX_Control(pDX, IDS_HOME_STATUS_0, m_lHomeSensor[0]);
	DDX_Control(pDX, IDS_HOME_STATUS_1, m_lHomeSensor[1]);
	DDX_Control(pDX, IDS_HOME_STATUS_2, m_lHomeSensor[2]);
	DDX_Control(pDX, IDS_HOME_STATUS_3, m_lHomeSensor[3]);
	DDX_Control(pDX, IDS_HOME_STATUS_4, m_lHomeSensor[4]);
	DDX_Control(pDX, IDS_HOME_STATUS_5, m_lHomeSensor[5]);
	DDX_Control(pDX, IDS_HOME_STATUS_6, m_lHomeSensor[6]);
	DDX_Control(pDX, IDS_HOME_STATUS_7, m_lHomeSensor[7]);
	DDX_Control(pDX, IDS_HOME_STATUS_8, m_lHomeSensor[8]);
	DDX_Control(pDX, IDS_HOME_STATUS_9, m_lHomeSensor[9]);
	DDX_Control(pDX, IDS_HOME_STATUS_10, m_lHomeSensor[10]);
	DDX_Control(pDX, IDS_HOME_STATUS_11, m_lHomeSensor[11]);
	DDX_Control(pDX, IDS_HOME_STATUS_12, m_lHomeSensor[12]);
	DDX_Control(pDX, IDS_HOME_STATUS_13, m_lHomeSensor[13]);
	DDX_Control(pDX, IDS_HOME_STATUS_14, m_lHomeSensor[14]);
	DDX_Control(pDX, IDS_HOME_STATUS_15, m_lHomeSensor[15]);

	DDX_Control(pDX, IDS_POS_STATUS_0,  m_lPosSensor[0]);
	DDX_Control(pDX, IDS_POS_STATUS_1,  m_lPosSensor[1]);
	DDX_Control(pDX, IDS_POS_STATUS_2,  m_lPosSensor[2]);
	DDX_Control(pDX, IDS_POS_STATUS_3,  m_lPosSensor[3]);
	DDX_Control(pDX, IDS_POS_STATUS_4,  m_lPosSensor[4]);
	DDX_Control(pDX, IDS_POS_STATUS_5,  m_lPosSensor[5]);
	DDX_Control(pDX, IDS_POS_STATUS_6,  m_lPosSensor[6]);
	DDX_Control(pDX, IDS_POS_STATUS_7,  m_lPosSensor[7]);
	DDX_Control(pDX, IDS_POS_STATUS_8,  m_lPosSensor[8]);
	DDX_Control(pDX, IDS_POS_STATUS_9,  m_lPosSensor[9]);
	DDX_Control(pDX, IDS_POS_STATUS_10, m_lPosSensor[10]);
	DDX_Control(pDX, IDS_POS_STATUS_11, m_lPosSensor[11]);
	DDX_Control(pDX, IDS_POS_STATUS_12, m_lPosSensor[12]);
	DDX_Control(pDX, IDS_POS_STATUS_13, m_lPosSensor[13]);
	DDX_Control(pDX, IDS_POS_STATUS_14, m_lPosSensor[14]);
	DDX_Control(pDX, IDS_POS_STATUS_15, m_lPosSensor[15]);

	DDX_Control(pDX, IDS_AXIS_STATUS_0,  m_lAxisStatus[0]);
	DDX_Control(pDX, IDS_AXIS_STATUS_1,  m_lAxisStatus[1]);
	DDX_Control(pDX, IDS_AXIS_STATUS_2,  m_lAxisStatus[2]);
	DDX_Control(pDX, IDS_AXIS_STATUS_3,  m_lAxisStatus[3]);
	DDX_Control(pDX, IDS_AXIS_STATUS_4,  m_lAxisStatus[4]);
	DDX_Control(pDX, IDS_AXIS_STATUS_5,  m_lAxisStatus[5]);
	DDX_Control(pDX, IDS_AXIS_STATUS_6,  m_lAxisStatus[6]);
	DDX_Control(pDX, IDS_AXIS_STATUS_7,  m_lAxisStatus[7]);
	DDX_Control(pDX, IDS_AXIS_STATUS_8,  m_lAxisStatus[8]);
	DDX_Control(pDX, IDS_AXIS_STATUS_9,  m_lAxisStatus[9]);
	DDX_Control(pDX, IDS_AXIS_STATUS_10, m_lAxisStatus[10]);
	DDX_Control(pDX, IDS_AXIS_STATUS_11, m_lAxisStatus[11]);
	DDX_Control(pDX, IDS_AXIS_STATUS_12, m_lAxisStatus[12]);
	DDX_Control(pDX, IDS_AXIS_STATUS_13, m_lAxisStatus[13]);
	DDX_Control(pDX, IDS_AXIS_STATUS_14, m_lAxisStatus[14]);
	DDX_Control(pDX, IDS_AXIS_STATUS_15, m_lAxisStatus[15]);


	DDX_Control(pDX, IDS_MTR_NUM_0, m_lAxisNum[0]);
	DDX_Control(pDX, IDS_MTR_NUM_1, m_lAxisNum[1]);
	DDX_Control(pDX, IDS_MTR_NUM_2, m_lAxisNum[2]);
	DDX_Control(pDX, IDS_MTR_NUM_3, m_lAxisNum[3]);
	DDX_Control(pDX, IDS_MTR_NUM_4, m_lAxisNum[4]);
	DDX_Control(pDX, IDS_MTR_NUM_5, m_lAxisNum[5]);
	DDX_Control(pDX, IDS_MTR_NUM_6, m_lAxisNum[6]);
	DDX_Control(pDX, IDS_MTR_NUM_7, m_lAxisNum[7]);
	DDX_Control(pDX, IDS_MTR_NUM_8, m_lAxisNum[8]);
	DDX_Control(pDX, IDS_MTR_NUM_9, m_lAxisNum[9]);
	DDX_Control(pDX, IDS_MTR_NUM_10, m_lAxisNum[10]);
	DDX_Control(pDX, IDS_MTR_NUM_11, m_lAxisNum[11]);
	DDX_Control(pDX, IDS_MTR_NUM_12, m_lAxisNum[12]);
	DDX_Control(pDX, IDS_MTR_NUM_13, m_lAxisNum[13]);
	DDX_Control(pDX, IDS_MTR_NUM_14, m_lAxisNum[14]);
	DDX_Control(pDX, IDS_MTR_NUM_15, m_lAxisNum[15]);
}


BEGIN_MESSAGE_MAP(CManualLimitSensorDlg, CDialog)
	//{{AFX_MSG_MAP(CManualLimitSensorDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualLimitSensorDlg message handlers

void CManualLimitSensorDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CManualLimitSensorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for (int i = 0; i < DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE; i++)
	{
		m_bOldOnOffStatus[i] = FALSE;
		m_bOldNegSensor[i] = FALSE;
		m_bOldHomeSensor[i] = FALSE;
		m_bOldPosSensor[i] = FALSE;
	}

	UpdateDisplay();
	
	m_uiTimerID = SetTimer(1, 500, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualLimitSensorDlg_1"), &szTextTemp);
	//이전
	GetDlgItem(IDB_PREVIOUS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualLimitSensorDlg_2"), &szTextTemp);
	//다음
	GetDlgItem(IDB_NEXT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualLimitSensorDlg_3"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
	{
		GetDlgItem(IDB_ALL_SERVO_ON)->EnableWindow(FALSE);
		GetDlgItem(IDB_ALL_SERVO_OFF)->EnableWindow(FALSE);
	}
	//_______________

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CManualLimitSensorDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualLimitSensorDlg)
	ON_EVENT(CManualLimitSensorDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_PREVIOUS, -600 /* Click */, OnPrevious, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_NEXT, -600 /* Click */, OnNext, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_0, -600 /* Click */, OnServoOn0, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_1, -600 /* Click */, OnServoOn1, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_2, -600 /* Click */, OnServoOn2, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_3, -600 /* Click */, OnServoOn3, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_4, -600 /* Click */, OnServoOn4, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_5, -600 /* Click */, OnServoOn5, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_6, -600 /* Click */, OnServoOn6, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_7, -600 /* Click */, OnServoOn7, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_8, -600 /* Click */, OnServoOn8, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_9, -600 /* Click */, OnServoOn9, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_10, -600 /* Click */, OnServoOn10, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_11, -600 /* Click */, OnServoOn11, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_12, -600 /* Click */, OnServoOn12, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_13, -600 /* Click */, OnServoOn13, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_14, -600 /* Click */, OnServoOn14, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_ON_15, -600 /* Click */, OnServoOn15, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_0, -600 /* Click */, OnServoOff0, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_1, -600 /* Click */, OnServoOff1, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_2, -600 /* Click */, OnServoOff2, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_3, -600 /* Click */, OnServoOff3, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_4, -600 /* Click */, OnServoOff4, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_5, -600 /* Click */, OnServoOff5, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_6, -600 /* Click */, OnServoOff6, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_7, -600 /* Click */, OnServoOff7, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_8, -600 /* Click */, OnServoOff8, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_9, -600 /* Click */, OnServoOff9, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_10, -600 /* Click */, OnServoOff10, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_11, -600 /* Click */, OnServoOff11, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_12, -600 /* Click */, OnServoOff12, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_13, -600 /* Click */, OnServoOff13, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_14, -600 /* Click */, OnServoOff14, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_SERVO_OFF_15, -600 /* Click */, OnServoOff15, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_ALL_SERVO_ON, -600 /* Click */, OnAllServoOn, VTS_NONE)
	ON_EVENT(CManualLimitSensorDlg, IDB_ALL_SERVO_OFF, -600 /* Click */, OnAllServoOff, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManualLimitSensorDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CDialog::OnCancel();
}

void CManualLimitSensorDlg::OnPrevious() 
{
	if (m_iCurrentPage > 0)
		m_iCurrentPage--;

	UpdateDisplay();
}

void CManualLimitSensorDlg::OnNext() 
{
	if (m_iCurrentPage < DEF_LIMIT_SENSOR_MAX_PAGE - 1)
		m_iCurrentPage++;

	UpdateDisplay();
}

void CManualLimitSensorDlg::UpdateDisplay()
{
	int iAxisId;

	for (int i = 0; i < DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE; i++)
	{
		iAxisId = i + (m_iCurrentPage * DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE);

		/** 축 개수 외의 항목은 disable */
		if (iAxisId >= DEF_LIMIT_SENSOR_MAX_MOTION_NUM || TRUE == MOlbSystemPre.IsSkipAxis(iAxisId)) //170713 JSH
		{
			m_lAxisName[i].ShowWindow(SW_HIDE);
			m_lNegSensor[i].ShowWindow(SW_HIDE);
			m_lHomeSensor[i].ShowWindow(SW_HIDE);
			m_lPosSensor[i].ShowWindow(SW_HIDE);
			m_lAxisStatus[i].ShowWindow(SW_HIDE);
			m_bServoOn[i].ShowWindow(SW_HIDE);
			m_bServoOff[i].ShowWindow(SW_HIDE);

			m_lAxisNum[i].ShowWindow(SW_HIDE);
		}
		else
		{
			// 축 개수 외의 항목은 disable 했던것들 다시 보여 주는 동작
			if (iAxisId < DEF_LIMIT_SENSOR_MAX_MOTION_NUM)
				m_lAxisNum[i].ShowWindow(SW_SHOW);

			m_lAxisNum[i].SetCaption(m_plnkAxis[iAxisId]->GetAxisName());

			int m_strNodeNum;
			CString strTemp;
			m_strNodeNum = m_plnkAxis[iAxisId]->GetNodeID();
			strTemp.Format(_T("%d"), m_strNodeNum);
			m_lAxisNum[i].SetCaption(strTemp);


			CString strAxisName;
//			strAxisName.Format(_T("[%d] %s"), m_plnkAxis[iAxisId]->GetNodeID(), m_plnkAxis[iAxisId]->GetAxisName());
			strAxisName.Format(_T("%s"), m_plnkAxis[iAxisId]->GetAxisName());
			m_lAxisName[i].SetCaption(strAxisName);
			
			m_bOldOnOffStatus[i] = FALSE;
			m_bOldNegSensor[i] = FALSE;
			m_bOldHomeSensor[i] = FALSE;
			m_bOldPosSensor[i] = FALSE;
			
			m_lNegSensor[i].SetBackColor(RGB(0, 255, 0));
			m_lNegSensor[i].SetForeColor(RGB(0, 0, 0));
			m_lNegSensor[i].SetCaption(_T("OFF"));
			
			m_lPosSensor[i].SetBackColor(RGB(0, 255, 0));
			m_lPosSensor[i].SetForeColor(RGB(0, 0, 0));
			m_lPosSensor[i].SetCaption(_T("OFF"));
			
			m_lHomeSensor[i].SetBackColor(RGB(0, 255, 0));
			m_lHomeSensor[i].SetForeColor(RGB(0, 0, 0));
			m_lHomeSensor[i].SetCaption(_T("OFF"));
			
			m_bServoOn[i].SetValue(FALSE);
			m_bServoOff[i].SetValue(TRUE);
			m_bServoOn[i].EnableWindow(TRUE);
			m_bServoOff[i].EnableWindow(TRUE);
			
			m_lAxisName[i].ShowWindow(SW_SHOW);
			m_lNegSensor[i].ShowWindow(SW_SHOW);
			m_lHomeSensor[i].ShowWindow(SW_SHOW);
			m_lPosSensor[i].ShowWindow(SW_SHOW);
			m_lAxisStatus[i].ShowWindow(SW_SHOW);
			m_bServoOn[i].ShowWindow(SW_SHOW);
			m_bServoOff[i].ShowWindow(SW_SHOW);
		}
	}

	/** 맨 첫 page일 경우 "이전" 버튼 숨김 */
	if (m_iCurrentPage == 0)
		m_bPrev.ShowWindow(SW_HIDE);
	else
		m_bPrev.ShowWindow(SW_SHOW);

	/** 실제로 사용하는 맨 마지막 page일 경우 "다음" 버튼 숨김 */
	int iLimitPageNo = DEF_LIMIT_SENSOR_MAX_MOTION_NUM / DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE;
//	ASSERT(iLimitPageNo < DEF_LIMIT_SENSOR_MAX_PAGE);

	if (m_iCurrentPage == iLimitPageNo)
		m_bNext.ShowWindow(SW_HIDE);
	else
		m_bNext.ShowWindow(SW_SHOW);

	UpdateButton();
}

void CManualLimitSensorDlg::UpdateButton()
{
	/** Sensor 상태 읽어서.. */
	/** On이면 노란 바탕에 빨간 글씨로 'ON' */
	/** Off이면 초록 바탕에 까만 글씨 'OFF'*/
	
	/** Motion Status 읽어서.. */
	/** 정상이면 초록 바탕에 까만 글씨로 '0x%04x' 형태 */
	/** 비정상이면 노란 바탕에 빨간 글씨로 '0x%04x' 형태 */
	int iAxisId;
	BOOL bSts;
	int iStatus;
	CString strTemp;
	int iResult;
	
	for (int i = 0; i < DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE; i++)
	{
		iAxisId = i + (m_iCurrentPage * DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE);

		// Spare Axis 처리
		if (TRUE == MOlbSystemPre.IsSkipAxis(iAxisId))
			continue;		
		
		// 축 개수 외의 항목은 처리안함
		if (iAxisId < DEF_LIMIT_SENSOR_MAX_MOTION_NUM)
		{
			// Sensor 상태 읽어서..
			// On이면 노란 바탕에 빨간 글씨로 'ON'
			// Off이면 초록 바탕에 까만 글씨 'OFF'
			// (-) Sensor Status

			bSts = m_plnkAxis[iAxisId]->IsOnNegativeSensor();
			if (bSts != m_bOldNegSensor[i])
			{
				if (bSts == TRUE)
				{
					m_lNegSensor[i].SetBackColor(RGB(255, 255, 0));
					m_lNegSensor[i].SetForeColor(RGB(255, 0, 0));
					m_lNegSensor[i].SetCaption(_T("ON"));
				}
				else
				{
					m_lNegSensor[i].SetBackColor(RGB(0, 255, 0));
					m_lNegSensor[i].SetForeColor(RGB(0, 0, 0));
					m_lNegSensor[i].SetCaption(_T("OFF"));
				}
				
				m_bOldNegSensor[i] = bSts;
			}
			
			// (+) Sensor Status
			bSts = m_plnkAxis[iAxisId]->IsOnPositiveSensor();
			if (bSts != m_bOldPosSensor[i])
			{
				if (bSts == TRUE)
				{
					m_lPosSensor[i].SetBackColor(RGB(255, 255, 0));
					m_lPosSensor[i].SetForeColor(RGB(255, 0, 0));
					m_lPosSensor[i].SetCaption(_T("ON"));
				}
				else
				{
					m_lPosSensor[i].SetBackColor(RGB(0, 255, 0));
					m_lPosSensor[i].SetForeColor(RGB(0, 0, 0));
					m_lPosSensor[i].SetCaption(_T("OFF"));
				}
				
				m_bOldPosSensor[i] = bSts;
			}
			
			// (H) Sensor Status
			bSts = m_plnkAxis[iAxisId]->IsOnHomeSensor();
			if (bSts != m_bOldHomeSensor[i])
			{
				if (bSts == TRUE)
				{
					m_lHomeSensor[i].SetBackColor(RGB(255, 255, 0));
					m_lHomeSensor[i].SetForeColor(RGB(255, 0, 0));
					m_lHomeSensor[i].SetCaption(_T("ON"));
				}
				else
				{
					m_lHomeSensor[i].SetBackColor(RGB(0, 255, 0));
					m_lHomeSensor[i].SetForeColor(RGB(0, 0, 0));
					m_lHomeSensor[i].SetCaption(_T("OFF"));
				}
				
				m_bOldHomeSensor[i] = bSts;
			}
			
			// Motion Status 읽어서..
			// 정상이면 초록 바탕에 까만 글씨로 '0x%04x' 형태
			// 비정상이면 노란 바탕에 빨간 글씨로 '0x%04x' 형태
			//iResult = m_plnkAxis[iAxisId]->GetAxisState(&iStatus);
			iResult = m_plnkAxis[iAxisId]->GetAxisSource(&iStatus); //170713 JSH
			if (iResult == ERR_MOTION_SUCCESS)
			{			 
				strTemp.Format(_T("0x%04x"), iStatus);
				if (iStatus == 0)
				{
					m_lAxisStatus[i].SetBackColor(RGB(0, 255, 0));
					m_lAxisStatus[i].SetForeColor(RGB(0, 0, 0));
					m_lAxisStatus[i].SetCaption(strTemp);
				}
				else
				{
					m_lAxisStatus[i].SetBackColor(RGB(255, 255, 0));
					m_lAxisStatus[i].SetForeColor(RGB(255, 0, 0));
					m_lAxisStatus[i].SetCaption(strTemp);
				}
			}
			
			// Servo On 상태 읽어서 
			// On이면 ON Color
			// Off이면 바탕색
			bSts = m_plnkAxis[iAxisId]->IsAmpEnable();
			if (bSts != m_bOldOnOffStatus[i])
			{
				if (bSts == TRUE)
				{
					m_bServoOn[i].SetValue(TRUE);
					m_bServoOff[i].SetValue(FALSE);
				}
				else
				{
					m_bServoOn[i].SetValue(FALSE);
					m_bServoOff[i].SetValue(TRUE);
				}
				
				m_bOldOnOffStatus[i] = bSts;
			}
		}
	}
}

void CManualLimitSensorDlg::OnServoOn0() 
{
	ServoOnOff(0, TRUE);
}

void CManualLimitSensorDlg::OnServoOn1() 
{
	ServoOnOff(1, TRUE);
}

void CManualLimitSensorDlg::OnServoOn2() 
{
	ServoOnOff(2, TRUE);
}

void CManualLimitSensorDlg::OnServoOn3() 
{
	ServoOnOff(3, TRUE);
}

void CManualLimitSensorDlg::OnServoOn4() 
{
	ServoOnOff(4, TRUE);
}

void CManualLimitSensorDlg::OnServoOn5() 
{
	ServoOnOff(5, TRUE);
}

void CManualLimitSensorDlg::OnServoOn6() 
{
	ServoOnOff(6, TRUE);
}

void CManualLimitSensorDlg::OnServoOn7() 
{
	ServoOnOff(7, TRUE);
}

void CManualLimitSensorDlg::OnServoOn8() 
{
	ServoOnOff(8, TRUE);
}

void CManualLimitSensorDlg::OnServoOn9() 
{
	ServoOnOff(9, TRUE);
}

void CManualLimitSensorDlg::OnServoOn10() 
{
	ServoOnOff(10, TRUE);
}

void CManualLimitSensorDlg::OnServoOn11() 
{
	ServoOnOff(11, TRUE);
}

void CManualLimitSensorDlg::OnServoOn12() 
{
	ServoOnOff(12, TRUE);
}

void CManualLimitSensorDlg::OnServoOn13() 
{
	ServoOnOff(13, TRUE);
}

void CManualLimitSensorDlg::OnServoOn14() 
{
	ServoOnOff(14, TRUE);
}

void CManualLimitSensorDlg::OnServoOn15() 
{
	ServoOnOff(15, TRUE);
}

void CManualLimitSensorDlg::OnServoOff0() 
{
	ServoOnOff(0, FALSE);
}

void CManualLimitSensorDlg::OnServoOff1() 
{
	ServoOnOff(1, FALSE);
}

void CManualLimitSensorDlg::OnServoOff2() 
{
	ServoOnOff(2, FALSE);
}

void CManualLimitSensorDlg::OnServoOff3() 
{
	ServoOnOff(3, FALSE);
}

void CManualLimitSensorDlg::OnServoOff4() 
{
	ServoOnOff(4, FALSE);
}

void CManualLimitSensorDlg::OnServoOff5() 
{
	ServoOnOff(5, FALSE);
}

void CManualLimitSensorDlg::OnServoOff6() 
{
	ServoOnOff(6, FALSE);
}

void CManualLimitSensorDlg::OnServoOff7() 
{
	ServoOnOff(7, FALSE);
}

void CManualLimitSensorDlg::OnServoOff8() 
{
	ServoOnOff(8, FALSE);
}

void CManualLimitSensorDlg::OnServoOff9() 
{
	ServoOnOff(9, FALSE);
}

void CManualLimitSensorDlg::OnServoOff10() 
{
	ServoOnOff(10, FALSE);
}

void CManualLimitSensorDlg::OnServoOff11() 
{
	ServoOnOff(11, FALSE);
}

void CManualLimitSensorDlg::OnServoOff12() 
{
	ServoOnOff(12, FALSE);
}

void CManualLimitSensorDlg::OnServoOff13() 
{
	ServoOnOff(13, FALSE);
}

void CManualLimitSensorDlg::OnServoOff14() 
{
	ServoOnOff(14, FALSE);
}

void CManualLimitSensorDlg::OnServoOff15() 
{
	ServoOnOff(15, FALSE);
}

void CManualLimitSensorDlg::ServoOnOff(int iIndex, BOOL bOnOff)
{
	/** On인지 Off인지에 따라 해당 명령 처리 */
	int iResult;
	int iAxisId = iIndex + (m_iCurrentPage * DEF_LIMIT_SENSOR_MAX_AXIS_PER_PAGE);
	CString strMsg = "";

	// Servo On
	if (bOnOff == TRUE)
	{
//		선택한 축의 Servo를 On 하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX, 30200, _T("Servo On"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		CMyProgressWnd ProgWnd(NULL, _T("Servo On..."));
		ProgWnd.SetRange(0, 2);
		ProgWnd.SetText(_T("Servo On..Wait a minute..."));
		ProgWnd.PeekAndPump();
		ProgWnd.StepIt();

		iResult = m_plnkAxis[iAxisId]->ServoOn();
		if (iResult != DEF_SUCCESS)
		{
			m_bServoOn[iIndex].SetValue(FALSE);
// jdy sesl			MyMessageBox(iResult, _T("Servo On Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
	// Servo Off
	else
	{
//		선택한 축의 Servo를 Off 하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX, 30201, _T("Servo Off"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		CMyProgressWnd ProgWnd(NULL, _T("Servo Off..."));
		ProgWnd.SetRange(0, 2);
		ProgWnd.SetText(_T("Servo Off.. Wait a minute..."));
		ProgWnd.PeekAndPump();
		ProgWnd.StepIt();
		
		if (DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T == iAxisId)
			MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetHomeFlagSprocket1(FALSE);
		if (DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T == iAxisId)
			MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetHomeFlagSprocket2(FALSE);

		if (DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T == iAxisId)
			MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetHomeFlagSprocket1(FALSE);
		if (DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T == iAxisId)
			MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetHomeFlagSprocket2(FALSE);
		
		iResult = m_plnkAxis[iAxisId]->ServoOff();
		if (iResult != DEF_SUCCESS)
		{
			m_bServoOff[iIndex].SetValue(FALSE);
// jdy sesl			MyMessageBox(iResult, _T("Servo Off Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}
}

void CManualLimitSensorDlg::OnAllServoOn() 
{
	int iResult;
	CString strErr;

//	모든 축의 Servo를 On 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30202, _T("All Servo On"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("All Servo On..."));
	ProgWnd.SetRange(0, DEF_LIMIT_SENSOR_MAX_MOTION_NUM);
	ProgWnd.SetText(_T("Servo On.. Wait a minute..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < DEF_LIMIT_SENSOR_MAX_MOTION_NUM; i++)
	{
		ProgWnd.StepIt();

		if (TRUE == MOlbSystemPre.IsSkipAxis(i))
			continue;

		/** AMP Fault Reset */
		if ((iResult = m_plnkAxis[i]->ServoOn()) != DEF_SUCCESS)
		{
			ProgWnd.DestroyWindow();
// jdy sesl			MyMessageBox(iResult, _T("All Servo On Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}	
}

void CManualLimitSensorDlg::OnAllServoOff() 
{
	int iResult;
	CString strErr;

//	모든 축의 Servo를 Off 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30203, _T("All Servo Off"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("All Servo Off..."));
	ProgWnd.SetRange(0, DEF_LIMIT_SENSOR_MAX_MOTION_NUM);
	ProgWnd.SetText(_T("Servo On.. Wait a minute..."));
	ProgWnd.PeekAndPump();

	for (int i = 0; i < DEF_LIMIT_SENSOR_MAX_MOTION_NUM; i++)
	{
		ProgWnd.StepIt();
		m_plnkAxis[i]->ResetOrigin();

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

		/** AMP Disable */
		if ((iResult = m_plnkAxis[i]->ServoOff()) != DEF_SUCCESS)
		{
			ProgWnd.DestroyWindow();
// jdy sesl			MyMessageBox(iResult, _T("All Servo Off Error"), M_ICONERROR);
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
			return;
		}
	}	
}

void CManualLimitSensorDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		UpdateButton();
	}
	
	CDialog::OnTimer(nIDEvent);
}
