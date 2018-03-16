// ManualMarkRecogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "ManualMarkRecogDlg.h"

#include "IVision.h"
#include "MManageOpPanel.h"
#include "MTrsAutoManager.h"
#include "MPlatformOlbSystem.h"
#include "MVision.h"
#include "VisionJogKeyDlg.h"
#include "IIO.h" 
#include "DefIOAddrPreBonder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CManualMarkRecogDlg dialog


CManualMarkRecogDlg::CManualMarkRecogDlg(int iCamNo, BOOL bSelectCam, CWnd* pParent /*=NULL*/)
	: CDialog(CManualMarkRecogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualMarkRecogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_bSelectCamera = bSelectCam;

	m_iModelNo = DEF_FIDU_MARK;
	m_bIsOnLive = FALSE;

	m_iCamIndex = 0;
	m_iCamNo[0] = DEF_PANEL_ALIGN_CAMERA_1;
	m_iCamNo[1] = DEF_PANEL_ALIGN_CAMERA_2;
	for (int i=0; i<2; i++)
	{
		if (m_iCamNo[i] == iCamNo)
		{
			m_iCamIndex = i;
			break;
		}
	}

	m_plnkTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
	m_plnkVision = MOlbSystemPre.GetVisionComponent();
	m_plnkIO = MOlbSystemPre.GetIOComponent();

	m_MarkPoint[0].x = (long)(m_plnkVision->GetDistribution(m_iCamNo[m_iCamIndex]).x/2.0);
	m_MarkPoint[0].y = (long)(m_plnkVision->GetDistribution(m_iCamNo[m_iCamIndex]).y/2.0);
	m_MarkPoint[1].x = m_MarkPoint[0].x;
	m_MarkPoint[1].y = m_MarkPoint[0].y;

	m_rectLimit.left = 3;
	m_rectLimit.right = m_plnkVision->GetDistribution(m_iCamNo[m_iCamIndex]).x-3;
	m_rectLimit.top = 3;
	m_rectLimit.bottom = m_plnkVision->GetDistribution(m_iCamNo[m_iCamIndex]).y-3;

	m_plnkManageOpPanel = MOlbSystemPre.GetManageOpPanelComponent();
}


void CManualMarkRecogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualMarkRecogDlg)
	DDX_Control(pDX, IDB_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_DESCRIPTION, m_lblDescription);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_CAM_1, m_btnCamNo[0]);
	DDX_Control(pDX, IDB_CAM_2, m_btnCamNo[1]);
}


BEGIN_MESSAGE_MAP(CManualMarkRecogDlg, CDialog)
	//{{AFX_MSG_MAP(CManualMarkRecogDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualMarkRecogDlg message handlers

BOOL CManualMarkRecogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_lblDescription.SetCaption(_T("Cross Mark를 Mark의 중심에 JOG로 이동해 주세요"));

	CWnd* pWnd = GetDlgItem(IDC_MONITOR);

	RECT rect;
	pWnd->GetWindowRect(&rect);
	CRect rectView(rect.left, rect.top, rect.right, rect.bottom);
	ScreenToClient(&rectView);

	m_iViewID = m_plnkVision->CreateLocalView(&rectView, this);

	int i = 0;
	if (TRUE == m_bSelectCamera)
	{
		for (i=0; i<2; i++)
		{
			m_plnkVision->ClearOverlay(m_iCamNo[i]);
			m_plnkVision->Grab(m_iCamNo[i]);
			int iResult = m_plnkVision->RecognitionPatternMark(m_iCamNo[i], m_iModelNo);
			if (iResult == SUCCESS)
			{
				m_MarkPoint[i].x = (long)m_plnkVision->GetSearchResultX(m_iCamNo[i], m_iModelNo);
				m_MarkPoint[i].y = (long)m_plnkVision->GetSearchResultY(m_iCamNo[i], m_iModelNo);
			}
			else
				m_iCamIndex = i;
		}
	}
	else
	{
		m_plnkVision->ClearOverlay(m_iCamNo[m_iCamIndex]);
		m_plnkVision->Grab(m_iCamNo[m_iCamIndex]);
		int iResult = m_plnkVision->RecognitionPatternMark(m_iCamNo[m_iCamIndex], m_iModelNo);
		if (iResult == SUCCESS)
		{
			m_MarkPoint[m_iCamIndex].x = (long)m_plnkVision->GetSearchResultX(m_iCamNo[m_iCamIndex], m_iModelNo);
			m_MarkPoint[m_iCamIndex].y = (long)m_plnkVision->GetSearchResultY(m_iCamNo[m_iCamIndex], m_iModelNo);
		}
	}

	if (TRUE == m_bSelectCamera)
	{
		m_btnCamNo[m_iCamIndex].SetValue(1);
	}
	else
	{
		m_btnCamNo[0].EnableWindow(FALSE);
		m_btnCamNo[1].EnableWindow(FALSE);
	}

#ifndef SIMULATION
	OnLiveImage();
#endif
	
//@	m_plnkVision->DrawOverlayCrossMark(m_iCamNo[m_iCamIndex], m_MarkPoint[m_iCamIndex].x, m_MarkPoint[m_iCamIndex].y, DEF_FLOATING_OBJECT);
	m_plnkVision->DrawOverlayCross_Measure(m_iCamNo[m_iCamIndex], m_MarkPoint[m_iCamIndex].x, m_MarkPoint[m_iCamIndex].y, DEF_FLOATING_OBJECT);

	SetTimer(10, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManualMarkRecogDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 10)
	{
		Trace();

		switch (getJogKey())
		{
		case JOGKEY_LEFT :
			m_vp_key = VP_LEFT;
			break;
		case JOGKEY_RIGHT :
			m_vp_key = VP_RIGHT;
			break;
		case JOGKEY_UP :
			m_vp_key = VP_UP;
			break;
		case JOGKEY_DOWN :
			m_vp_key = VP_DOWN;
			break;
		case JOGKEY_FAST_LEFT :
			m_vp_key = VP_FAST_LEFT;
			break;
		case JOGKEY_FAST_RIGHT :
			m_vp_key = VP_FAST_RIGHT;
			break;
		case JOGKEY_FAST_DOWN :
			m_vp_key = VP_FAST_DOWN;
			break;
		case JOGKEY_FAST_UP :
			m_vp_key = VP_FAST_UP;
			break;
		default :
			m_vp_key = 0;
			break;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CManualMarkRecogDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualMarkRecogDlg)
	ON_EVENT(CManualMarkRecogDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CManualMarkRecogDlg, IDB_START, -600 /* Click */, OnClickStart, VTS_NONE)
	ON_EVENT(CManualMarkRecogDlg, IDB_CAM_1, -600 /* Click */, OnClickCam1, VTS_NONE)
	ON_EVENT(CManualMarkRecogDlg, IDB_CAM_2, -600 /* Click */, OnClickCam2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManualMarkRecogDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	OnStillImage();
	m_plnkVision->DestroyView(m_iViewID);
	CDialog::OnCancel();
}

void CManualMarkRecogDlg::OnStillImage() 
{
	if (m_bIsOnLive == TRUE)
	{
		m_plnkVision->HaltVideo(m_iCamNo[m_iCamIndex]);

		// 확인 필요 : Live 종료를 확인하는 방법 필요.
		TRACE0("Live Stop for Still Image...");
	}

	m_plnkVision->ClearOverlay(m_iCamNo[m_iCamIndex]);
	m_plnkVision->Grab(m_iCamNo[m_iCamIndex]);
	m_bIsOnLive = FALSE;
}

void CManualMarkRecogDlg::OnLiveImage() 
{
	if (m_bIsOnLive == TRUE)
		return;

	m_plnkVision->ClearOverlay(m_iCamNo[m_iCamIndex]);
	m_plnkVision->LiveVideo(m_iCamNo[m_iCamIndex]);
	m_bIsOnLive = TRUE;
//	m_bIsOnStill = FALSE;
}


void CManualMarkRecogDlg::OnClickStart() 
{
	// TODO: Add your control notification handler code here
	/*/
	Sleep(500);
#ifndef SIMULATION
//	OnLiveImage();
	m_plnkVision->DrawSourceImage(m_iCurrentCamNo);
#endif

	Sleep(1000);

	CPoint Point (500, 300);
	CRect SArea;//(256, 70, 256+512, 70+240);		
	double dX, dY;

	dX = m_plnkVision->GetSearchResultX(m_iCurrentCamNo, m_iModelNo);
	dY = m_plnkVision->GetSearchResultY(m_iCurrentCamNo, m_iModelNo);

	SArea = m_plnkVision->GetSearchAreaRect(m_iCurrentCamNo, m_iModelNo);
	m_plnkVision->DrawOverlayCrossMark(m_iCurrentCamNo, Point.x, Point.y);

	m_plnkVision->SetSearchResultX(m_iCurrentCamNo, m_iModelNo, dX);
	m_plnkVision->SetSearchResultY(m_iCurrentCamNo, m_iModelNo, dY);

	if(m_iCurrentCamNo == DEF_PANEL_ALIGN_CAMERA_1)
	{
		//Camera의 한 값의 XY를 받는다.
	}
	else if(m_iCurrentCamNo == DEF_PANEL_ALIGN_CAMERA_2)
	{
	    //Camera 2의 위치 값의 XY를 받는다.
	}
	/*/
	OnStillImage();
	m_plnkVision->DestroyView(m_iViewID);
	CDialog::OnOK();
}

void CManualMarkRecogDlg::OnClickCam1() 
{
	// TODO: Add your control notification handler code here
	selectCamera(0);
}

void CManualMarkRecogDlg::OnClickCam2() 
{
	// TODO: Add your control notification handler code here
	selectCamera(1);
}

void CManualMarkRecogDlg::selectCamera(int iCamNo)
{
	if (m_iCamIndex != iCamNo && m_bIsOnLive == TRUE)
	{
		m_btnCamNo[m_iCamIndex].SetValue(0);
		m_iCamIndex = iCamNo;		
		m_plnkVision->HaltVideo(m_iCamNo[m_iCamIndex]);
		m_plnkVision->ClearOverlay(m_iCamNo[m_iCamIndex]);
		m_bIsOnLive = FALSE;
		m_btnCamNo[m_iCamIndex].SetValue(1);
	}

	m_iCamIndex = iCamNo;

//	checkLightForCurrentCamera();

	OnLiveImage();

//@	m_plnkVision->DrawOverlayCrossMark(m_iCamNo[m_iCamIndex], m_MarkPoint[m_iCamIndex].x, m_MarkPoint[m_iCamIndex].y, DEF_FLOATING_OBJECT);
	m_plnkVision->DrawOverlayCross_Measure(m_iCamNo[m_iCamIndex], m_MarkPoint[m_iCamIndex].x, m_MarkPoint[m_iCamIndex].y, DEF_FLOATING_OBJECT);
}


void CManualMarkRecogDlg::Trace()
{
	int iResult;
	
	iResult = tracePoint();
	if (iResult == 0)	// Draw
//@		m_plnkVision->DrawOverlayCrossMark(m_iCamNo[m_iCamIndex], m_MarkPoint[m_iCamIndex].x, m_MarkPoint[m_iCamIndex].y, DEF_FLOATING_OBJECT);
		m_plnkVision->DrawOverlayCross_Measure(m_iCamNo[m_iCamIndex], m_MarkPoint[m_iCamIndex].x, m_MarkPoint[m_iCamIndex].y, DEF_FLOATING_OBJECT);	
}

int CManualMarkRecogDlg::tracePoint()
{
	CPoint pointTmp = m_MarkPoint[m_iCamIndex];
	
	if (m_vp_key == VP_LBUTTON)			// Ok.
		return 2;
	else if (m_vp_key == VP_RBUTTON)	// Canceal.
		return 3;
	
	switch (m_vp_key)
	{
	case VP_LEFT:
	case VP_FAST_LEFT:
		if (m_vp_key == VP_LEFT)	pointTmp.x -= 1;
		else						pointTmp.x -= 5;
		if (pointTmp.x <= m_rectLimit.left)
		{
			pointTmp.x = m_rectLimit.left;
			break;
		}
		break;

	case VP_RIGHT:
	case VP_FAST_RIGHT:
		if (m_vp_key == VP_RIGHT)	pointTmp.x += 1;
		else						pointTmp.x += 5;
		if (pointTmp.x >= m_rectLimit.right)
		{
			pointTmp.x = m_rectLimit.right;
			break;
		}
		break;	
		
	case VP_UP:
	case VP_FAST_UP:
		if (m_vp_key == VP_UP)		pointTmp.y -= 1;
		else						pointTmp.y -= 5;
		if (pointTmp.y <= m_rectLimit.top)
		{
			pointTmp.y = m_rectLimit.top;
			break;
		}
		break;
		
	case VP_DOWN:
	case VP_FAST_DOWN:
		if (m_vp_key == VP_DOWN)	pointTmp.y += 1;
		else						pointTmp.y += 5;
		if (pointTmp.y >= m_rectLimit.bottom)
		{
			pointTmp.y = m_rectLimit.bottom;
			break;
		}
		break;
		
	default :
		break;
	}
	
	if (m_MarkPoint[m_iCamIndex] != pointTmp)
	{
		m_MarkPoint[m_iCamIndex] = pointTmp;
		return 0;
	}
	return 5; // 1;
}

unsigned CManualMarkRecogDlg::getJogKey()
{
	switch (checkKey())
	{
		case 1:
			m_nJogKeyValue=JOGKEY_UP;
			break;
		case 2:
			m_nJogKeyValue=JOGKEY_DOWN;
			break;
		case 4:
			m_nJogKeyValue=JOGKEY_LEFT;
			break;
		case 8:
			m_nJogKeyValue=JOGKEY_RIGHT;
			break;
		case 3:
			if (m_nOldKeyValue==JOGKEY_DOWN ||m_nOldKeyValue==JOGKEY_FAST_DOWN)
				m_nJogKeyValue=JOGKEY_FAST_DOWN;
			else
				m_nJogKeyValue=JOGKEY_FAST_UP;
			break;
		case 12:
			if (m_nOldKeyValue==JOGKEY_RIGHT || m_nOldKeyValue==JOGKEY_FAST_RIGHT)
				m_nJogKeyValue=JOGKEY_FAST_RIGHT;
			else
				m_nJogKeyValue=JOGKEY_FAST_LEFT;
			break;
		default:
			m_nJogKeyValue=0;
	}

	m_nOldKeyValue=m_nJogKeyValue;
	return m_nJogKeyValue;
}

unsigned CManualMarkRecogDlg::checkKey()
{
	unsigned retkey = 0;

	if (m_plnkIO->IsOn(IN_FRONT_JOG_F_SW) || m_plnkIO->IsOn(IN_REAR_JOG_F_SW))
		retkey = 1;
	
	if (m_plnkIO->IsOn(IN_FRONT_JOG_B_SW) || m_plnkIO->IsOn(IN_REAR_JOG_B_SW))
		retkey += 2;
	
	if (m_plnkIO->IsOn(IN_FRONT_JOG_L_SW) || m_plnkIO->IsOn(IN_REAR_JOG_L_SW))
		retkey += 4;

	if (m_plnkIO->IsOn(IN_FRONT_JOG_R_SW) || m_plnkIO->IsOn(IN_REAR_JOG_R_SW))
		retkey += 8;

	return retkey;
}
