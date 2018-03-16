// VisionJogKeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisionJogKeyDlg.h"
#include "IVision.h"
#include "MPlatformOlbSystem.h"
// jdy2 #include "IOpPanel.h"
#include "IIO.h"
#include "DefIOAddrPreBonder.h"

#include "MManageOpPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CVisionJogKeyDlg dialog

CVisionJogKeyDlg::CVisionJogKeyDlg(IVision* plnkVision, int iCamNo, CString strMsg, CRect rectLimit, CRect* pRect, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionJogKeyDlg::IDD, pParent)
{
	m_plnkIVision = plnkVision;
	// jdy2 m_plnkOpPanel = MOlbSystemPre.GetOpPanelComponent();
	m_plnkIO = MOlbSystemPre.GetIOComponent();
	m_pC_MngOpPanel = MOlbSystemPre.GetManageOpPanelComponent();

	m_iObjectType = DEF_RECTANGLE;
	m_plnkRect = pRect;
	m_rectLimit = rectLimit;
	m_plnkPoint = NULL;

	m_iCurrentCam = iCamNo;

	m_bTimerInitFlag = FALSE;
	m_bOpMode = FALSE;

	m_strMsg = strMsg;

	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iOldJogID = m_pC_MngOpPanel->GetJogUnit();
	m_pC_MngOpPanel->SetJogUnit(-1);
}

CVisionJogKeyDlg::CVisionJogKeyDlg(IVision* plnkVision, int iCamNo, CString strMsg, CRect rectLimit, CPoint* pPoint, int iObjectType, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionJogKeyDlg::IDD, pParent)
{
	m_plnkIVision = plnkVision;
	// jdy2 m_plnkOpPanel = MOlbSystemPre.GetOpPanelComponent();
	m_plnkIO = MOlbSystemPre.GetIOComponent();
	m_pC_MngOpPanel = MOlbSystemPre.GetManageOpPanelComponent();

	ASSERT(iObjectType == DEF_POINT || iObjectType == DEF_POINT_MEASURE);

//	m_iObjectType = DEF_POINT;
	m_iObjectType = iObjectType;

	m_plnkRect = NULL;
	m_rectLimit = rectLimit;
	m_plnkPoint = pPoint;

	m_iCurrentCam = iCamNo;

	m_bTimerInitFlag = FALSE;
	m_bOpMode = FALSE;

	m_strMsg = strMsg;

	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iOldJogID = m_pC_MngOpPanel->GetJogUnit();
	m_pC_MngOpPanel->SetJogUnit(-1);
}

CVisionJogKeyDlg::CVisionJogKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVisionJogKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisionJogKeyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
}

CVisionJogKeyDlg::~CVisionJogKeyDlg()
{
	if (m_bTimerInitFlag)
	{
		KillTimer(m_KeyTimerID);
	}

	m_pC_MngOpPanel->SetJogUnit(m_iOldJogID);
}

void CVisionJogKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionJogKeyDlg)
	DDX_Control(pDX, IDC_JOG_MSG, m_ctrlJogMsg);
	DDX_Control(pDX, IDB_JOG_LEFT_BTN, m_ctrlBtnLeft);
	DDX_Control(pDX, IDB_JOG_RIGHT_BTN, m_ctrlBtnRight);
	DDX_Control(pDX, IDB_JOG_CENTER_BTN, m_ctrlBtnCenter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisionJogKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionJogKeyDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionJogKeyDlg message handlers

BOOL CVisionJogKeyDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
    {
		switch (pMsg->wParam)
		{
		case 13 :
		case VK_ESCAPE :
			pMsg->message = 0;
			pMsg->wParam = 0;
			break;
	
		case VK_LEFT : m_vp_key = VP_LEFT; break;
		case VK_RIGHT : m_vp_key = VP_RIGHT; break;
		case VK_UP : m_vp_key = VP_UP; break;
		case VK_DOWN : m_vp_key = VP_DOWN; break;
		case VK_F5 : m_vp_key = VP_LBUTTON; break;
		case VK_F6 : m_vp_key = VP_CBUTTON; break;
		case VK_F7 : m_vp_key = VP_RBUTTON; break;
		default : break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CVisionJogKeyDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CVisionJogKeyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	CRect mainrect;
	CWnd* pParent = GetParent();
	pParent->GetWindowRect(&mainrect);
    GetWindowRect(&rect);
	MoveWindow(mainrect.right-rect.Width(), mainrect.top, rect.Width(), rect.Height());

	m_ctrlJogMsg.SetCaption(m_strMsg);

	m_ctrlBtnLeft.SetCaption(_T("OK"));
	m_ctrlBtnRight.SetCaption(_T("Cancel"));

	if (m_iObjectType == DEF_RECTANGLE)
	{
		if (m_bOpMode)		// 현재 상태 표시 : TRUE 이면 Resize Box
			m_ctrlBtnCenter.SetCaption(_T("Resize Box"));
		else
			m_ctrlBtnCenter.SetCaption(_T("Move Box"));
		
		m_plnkIVision->DrawOverlayAreaRect(m_iCurrentCam, *m_plnkRect, DEF_FLOATING_OBJECT);
	}
	else
	{
		m_ctrlBtnCenter.ShowWindow(SW_HIDE);
		
		m_plnkIVision->DrawOverlayCrossMark(m_iCurrentCam, m_plnkPoint->x, m_plnkPoint->y, DEF_FLOATING_OBJECT);
	}

	m_KeyTimerID = SetTimer(9, 100, NULL);

	if (!m_KeyTimerID)
	{
		CString szTemp;
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_66500"), &szTemp);
				//Timer가 생성 되지 않았습니다.
		::MessageBox(NULL, szTemp,_T("Windows Error"), MB_ICONERROR);
		m_bTimerInitFlag = FALSE;
		return FALSE;
	}
	m_bTimerInitFlag = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVisionJogKeyDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionJogKeyDlg)
	ON_EVENT(CVisionJogKeyDlg, IDB_JOG_UP, -600 /* Click */, OnClickJogUp, VTS_NONE)
	ON_EVENT(CVisionJogKeyDlg, IDB_JOG_CENTER_BTN, -600 /* Click */, OnClickJogCenterBtn, VTS_NONE)
	ON_EVENT(CVisionJogKeyDlg, IDB_JOG_LEFT, -600 /* Click */, OnClickJogLeft, VTS_NONE)
	ON_EVENT(CVisionJogKeyDlg, IDB_JOG_RIGHT, -600 /* Click */, OnClickJogRight, VTS_NONE)
	ON_EVENT(CVisionJogKeyDlg, IDB_JOG_DOWN, -600 /* Click */, OnClickJogDown, VTS_NONE)
	ON_EVENT(CVisionJogKeyDlg, IDB_JOG_LEFT_BTN, -600 /* Click */, OnClickJogLeftBtn, VTS_NONE)
	ON_EVENT(CVisionJogKeyDlg, IDB_JOG_RIGHT_BTN, -600 /* Click */, OnClickJogRightBtn, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVisionJogKeyDlg::OnClickJogLeft() 
{
	// TODO: Add your control notification handler code here
	m_vp_key = VP_LEFT;	
}

void CVisionJogKeyDlg::OnClickJogRight() 
{
	// TODO: Add your control notification handler code here
	m_vp_key = VP_RIGHT;
}

void CVisionJogKeyDlg::OnClickJogUp() 
{
	// TODO: Add your control notification handler code here
	m_vp_key = VP_UP;
}

void CVisionJogKeyDlg::OnClickJogDown() 
{
	// TODO: Add your control notification handler code here
	m_vp_key = VP_DOWN;
}

void CVisionJogKeyDlg::OnClickJogCenterBtn() 
{
	// TODO: Add your control notification handler code here
	m_vp_key = VP_CBUTTON;
	m_bOpMode = (m_bOpMode) ? FALSE : TRUE;
	if (m_iObjectType == DEF_RECTANGLE)
	{
		if (m_bOpMode)
			m_ctrlBtnCenter.SetCaption(_T("Resize Box"));
		else
			m_ctrlBtnCenter.SetCaption(_T("Move Box"));
	}
}

void CVisionJogKeyDlg::OnClickJogLeftBtn() 
{
	// TODO: Add your control notification handler code here
	m_vp_key = VP_LBUTTON;
}

void CVisionJogKeyDlg::OnClickJogRightBtn() 
{
	// TODO: Add your control notification handler code here
	m_vp_key = VP_RBUTTON;
}

void CVisionJogKeyDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_KeyTimerID)
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

unsigned CVisionJogKeyDlg::getJogKey()
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

unsigned CVisionJogKeyDlg::checkKey()
{
	unsigned retkey = 0;
	/* jdy2
	if (m_plnkOpPanel->GetJogYPlusButtonStatus())
		retkey=1;
	if(m_plnkOpPanel->GetJogYMinusButtonStatus())
		retkey+=2;
	if(m_plnkOpPanel->GetJogXMinusButtonStatus())
		retkey+=4;
	if(m_plnkOpPanel->GetJogXPlusButtonStatus())
		retkey+=8;
	*/
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

void CVisionJogKeyDlg::Trace()
{
	int iResult;
	
	if (m_iObjectType == DEF_RECTANGLE)
	{
		iResult = traceBox();
		if (iResult == 0)	// Draw
			m_plnkIVision->DrawOverlayAreaRect(m_iCurrentCam, *m_plnkRect, DEF_FLOATING_OBJECT);
	}
	else if (m_iObjectType == DEF_POINT)
	{
		iResult = tracePoint();
		if (iResult == 0)	// Draw
			m_plnkIVision->DrawOverlayCrossMark(m_iCurrentCam, m_plnkPoint->x, m_plnkPoint->y, DEF_FLOATING_OBJECT);
	}
	//110214.Add_____
	else if (m_iObjectType == DEF_POINT_MEASURE)
	{
		iResult = tracePoint();
		if (iResult == 0)	// Draw
			m_plnkIVision->DrawOverlayCross_Measure(m_iCurrentCam, m_plnkPoint->x, m_plnkPoint->y, DEF_FLOATING_OBJECT);
	}
	else if (m_iObjectType == DEF_LINE_VER)
	{
	}
	else if (m_iObjectType == DEF_LINE_HOR)
	{
	}
	//_______________

	if (iResult == 2 || iResult == 3)		// iResult == 1 : Not Draw
	{
		if (m_bTimerInitFlag)
		{
			if (m_KeyTimerID)
				KillTimer(m_KeyTimerID);
			m_bTimerInitFlag = FALSE;
		}
		
		if (iResult == 2)
			CDialog::OnOK();
		else
			CDialog::OnCancel();
	}
}

int CVisionJogKeyDlg::traceBox()
{
	CRect rectTmp = *m_plnkRect;

	if (m_vp_key == VP_LBUTTON)			// Ok.
		return 2;
	else if (m_vp_key == VP_RBUTTON)	// Canceal.
		return 3;
	
	switch (m_vp_key)
	{
	case VP_LEFT:
		if (!m_bOpMode)
		{
			if (rectTmp.left <= m_rectLimit.left)
				break;					
			rectTmp.left -= 1;
			rectTmp.right -= 1;
		}
		else
		{
			if (rectTmp.right <= m_rectLimit.left)
				break;
			rectTmp.right -= 1;
		}
		break;
		
	case VP_RIGHT:
		if (!m_bOpMode)
		{
			if (rectTmp.right >= m_rectLimit.right)
				break;
			rectTmp.left += 1;
			rectTmp.right += 1;
		}
		else
		{
			if (rectTmp.right >= m_rectLimit.right)
				break;
			rectTmp.right += 1;
		}
		break;
		
	case VP_UP:
		if (!m_bOpMode)
		{
			if (rectTmp.top <= m_rectLimit.top)
				break;
			rectTmp.top -= 1;
			rectTmp.bottom -= 1;
		}
		else
		{
			if (rectTmp.bottom <= m_rectLimit.top)
				break;
			rectTmp.bottom -= 1;
		}
		break;
		
	case VP_DOWN:
		if (!m_bOpMode)
		{
			if (rectTmp.bottom >= m_rectLimit.bottom)
				break;
			rectTmp.top += 1;
			rectTmp.bottom += 1;
		}
		else
		{
			if (rectTmp.bottom >= m_rectLimit.bottom)
				break;
			rectTmp.bottom += 1;
		}
		break;				
		
	case VP_FAST_LEFT:
		if (!m_bOpMode)
		{
			if (rectTmp.left <= m_rectLimit.left)
				break;					
			rectTmp.left -= 10;
			rectTmp.right -= 10;
		}
		else
		{
			if (rectTmp.right <= m_rectLimit.left)
				break;
			rectTmp.right -= 10;
		}
		break;
		
	case VP_FAST_RIGHT:
		if (!m_bOpMode)
		{
			if (rectTmp.right >= m_rectLimit.right)
				break;
			rectTmp.left += 10;
			rectTmp.right += 10;
		}
		else
		{
			if (rectTmp.right >= m_rectLimit.right)
				break;
			rectTmp.right += 10;
		}
		break;
		
	case VP_FAST_UP:
		if (!m_bOpMode)
		{
			if (rectTmp.top <= m_rectLimit.top)
				break;
			rectTmp.top -= 10;
			rectTmp.bottom -= 10;
		}
		else
		{
			if (rectTmp.bottom <= m_rectLimit.top)
				break;
			rectTmp.bottom -= 10;
		}
		break;
		
	case VP_FAST_DOWN:
		if (!m_bOpMode)
		{
			if (rectTmp.bottom >= m_rectLimit.bottom)
				break;
			rectTmp.top += 10;
			rectTmp.bottom += 10;
		}
		else
		{
			if (rectTmp.bottom >= m_rectLimit.bottom)
				break;
			rectTmp.bottom += 10;
		}
		break;		

	default :
		break;
	}

	if (*m_plnkRect == rectTmp)
		return 1;

	*m_plnkRect = rectTmp;
	return 0;
}

int CVisionJogKeyDlg::tracePoint()
{
	CPoint pointTmp = *m_plnkPoint;
	
	if (m_vp_key == VP_LBUTTON)			// Ok.
		return 2;
	else if (m_vp_key == VP_RBUTTON)	// Canceal.
		return 3;
	
	switch (m_vp_key)
	{
	case VP_LEFT:
		if (pointTmp.x <= m_rectLimit.left)
			break; 
		pointTmp.x -= 1;
		break;
		
	case VP_RIGHT:
		if (pointTmp.x >= m_rectLimit.right)
			break;
		pointTmp.x += 1;
		break;
		
	case VP_UP:
		if (pointTmp.y <= m_rectLimit.top)
			break;
		pointTmp.y -= 1;
		break;
		
	case VP_DOWN:
		if (pointTmp.y >= m_rectLimit.bottom)
			break;
		pointTmp.y += 1;
		break;

	case VP_FAST_LEFT:
		if (pointTmp.x <= m_rectLimit.left)
			break; 
		pointTmp.x -= 10;
		break;
		
	case VP_FAST_RIGHT:
		if (pointTmp.x >= m_rectLimit.right)
			break;
		pointTmp.x += 10;
		break;
		
	case VP_FAST_UP:
		if (pointTmp.y <= m_rectLimit.top)
			break;
		pointTmp.y -= 10;
		break;
		
	case VP_FAST_DOWN:
		if (pointTmp.y >= m_rectLimit.bottom)
			break;
		pointTmp.y += 10;
		break;
		
	default :
		break;
	}
	
	if (*m_plnkPoint == pointTmp)
		return 1;

	*m_plnkPoint = pointTmp;
	return 0;
}