// VisionOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Common.h"
#include "VisionOptionDlg.h"
#include "VisionIlluminationDlg.h"
#include "MPreBonderData.h"
#include "MPlatformOlbSystem.h"
#include "IVision.h"
#include "ILighting.h"
#include "atlconv.h"
#include "VisionBlobThresholdDlg.h" //20171027 KDW ADD

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CVisionOptionDlg dialog

CVisionOptionDlg::CVisionOptionDlg(IVision* plnkVision, EMarkType eMarkType, int iCamNo, int iModelNo, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionOptionDlg::IDD, pParent), m_plnkVision(plnkVision), m_eMarkType(eMarkType), m_iCamNo(iCamNo), m_iModelNo(iModelNo), m_bIsValidCamNo(TRUE)
{
	//{{AFX_DATA_INIT(CVisionOptionDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
}

void CVisionOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionOptionDlg)
	DDX_Control(pDX, IDC_CAMERA_NUMBER, m_ctrlLblCamNo);
	DDX_Control(pDX, IDC_MARK_NUMBER, m_ctrlLblMarkName);
	DDX_Control(pDX, IDC_ILLUMINATION1, m_lblIllumination1);
	DDX_Control(pDX, IDC_ILLUMINATION2, m_lblIllumination2);
	DDX_Control(pDX, IDB_ILLUMINATION1_ONOFF, m_btnillumination1);
	DDX_Control(pDX, IDB_ILLUMINATION2_ONOFF, m_btnillumination2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_DIRECTION, m_rgCtrlBtn[0]);
	DDX_Control(pDX, IDB_COLOR,		m_rgCtrlBtn[1]);
	DDX_Control(pDX, IDB_THRESHOLD, m_rgCtrlBtn[2]);
	DDX_Control(pDX, IDB_MAX_NUM,	m_rgCtrlBtn[3]);

	DDX_Control(pDX, IDC_DIRECTION, m_rgCtrlLbl[0]);
	DDX_Control(pDX, IDC_COLOR,		m_rgCtrlLbl[1]);
	DDX_Control(pDX, IDC_THRESHOLD, m_rgCtrlLbl[2]);
	DDX_Control(pDX, IDC_MAX_NUM,	m_rgCtrlLbl[3]);
}

BEGIN_MESSAGE_MAP(CVisionOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionOptionDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionOptionDlg message handlers

void CVisionOptionDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CVisionOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	CRect mainrect;
	CWnd* pParent = GetParent();
	pParent->GetWindowRect(&mainrect);
    GetWindowRect(&rect);
	MoveWindow(mainrect.right-rect.Width(), mainrect.top, rect.Width(), rect.Height());

	if (m_plnkVision->IsValidCam(m_iCamNo) == FALSE)
	{
		m_bIsValidCamNo = FALSE;

		//Camera 번호 선택이 잘못 되었습니다. VisionOptionDialog 를 닫아 주세요.
		MyMessageBox(MY_MESSAGE_BOX, 64500, _T("Camera No. Error"), M_ICONERROR);
		for (int j = 0 ; j < 4 ; j++)
		{
			m_rgCtrlBtn[j].EnableWindow(FALSE);
			m_rgCtrlLbl[j].SetCaption(_T("-.-"));

			m_btnillumination1.EnableWindow(FALSE);
			m_btnillumination2.EnableWindow(FALSE);
		}

		return FALSE;
	}

	// Camera Name Display
	CString szTemp=m_plnkVision->GetCameraName(m_iCamNo).c_str();
	m_ctrlLblCamNo.SetCaption(szTemp.GetBuffer(szTemp.GetLength()));

	// Mark Name Display
/* 확인 필요 : 어떤 이름을 디스플레이할까?
	char szMarkName[256];
	int iResult = m_plnkVision->GetMarkName(m_iCamNo, m_eMarkType, m_iModelNo, szMarkName);
	CString strMarkName(szMarkName);
	if (strMarkName.IsEmpty())
		strMarkName.Format("-");

	m_ctrlLblMarkName.SetCaption(strMarkName);
*/
	// Option Display
	int i = 0;
	CString strTmpValue;
	switch (m_eMarkType)
	{
	case DEF_PM_MARK:
		SetWindowText(_T("Pattern Matching Option"));

		// 1
		m_rgCtrlBtn[0].SetCaption(_T("Acceptance\nThreshold"));
		m_rgiValue[0] = static_cast<int>(m_plnkVision->GetSearchAcceptanceThreshold(m_iCamNo, m_iModelNo));
		strTmpValue.Format(_T("%d"), m_rgiValue[0]);
		m_rgCtrlLbl[0].SetCaption(strTmpValue);
		
		// 2, 3, 4
		for (int i=1 ; i<4 ; i++)
		{
			m_rgCtrlBtn[i].SetCaption(_T("Reserved"));
			m_rgCtrlBtn[i].EnableWindow(FALSE);
			m_rgCtrlLbl[i].SetCaption(_T("Not Used"));
			m_rgCtrlLbl[i].EnableWindow(FALSE);
		}
		break;
	case DEF_BLOB_MARK:
		SetWindowText(_T("Blob Analysis Option"));

		// 1
		m_rgCtrlBtn[0].SetCaption(_T("Threshold\n[1 ~ 255]"));
		m_rgiValue[0] = static_cast<int>(m_plnkVision->GetBlobThreshold(m_iCamNo, m_iModelNo));
		strTmpValue.Format(_T("%d"), m_rgiValue[0]);
		m_rgCtrlLbl[0].SetCaption(strTmpValue);
		
		// 2
		m_rgCtrlBtn[1].SetCaption(_T("Polarity\n[B:0 / W:1]"));
		m_rgiValue[1] = static_cast<int>(m_plnkVision->GetBlobPolarity(m_iCamNo, m_iModelNo));
		if (m_rgiValue[1] == 0)	// Black
		{
			strTmpValue.Format(_T("Black"));
		}
		else if (m_rgiValue[1] == 1)	// White
		{
			strTmpValue.Format(_T("White"));
		}
		m_rgCtrlLbl[1].SetCaption(strTmpValue);
		
	//170616 JSH.s
		// 3
		m_rgCtrlBtn[2].SetCaption(_T("Min Pixels"));
		m_rgiValue[2] = m_plnkVision->GetBlobJudgeMinPixelPercent(m_iCamNo, m_iModelNo);
		strTmpValue.Format(_T("%d"), m_rgiValue[2]);
		m_rgCtrlLbl[2].SetCaption(strTmpValue);

		// 4
		m_rgCtrlBtn[3].SetCaption(_T("Max Pixels"));
		m_rgiValue[3] = m_plnkVision->GetBlobJudgeMaxPixelPercent(m_iCamNo, m_iModelNo);
		strTmpValue.Format(_T("%d"), m_rgiValue[3]);
		m_rgCtrlLbl[3].SetCaption(strTmpValue);
	//170616 JSH.e

		break;

	case DEF_CALIPER_MARK:
		SetWindowText(_T("Edge Finder Option"));

		// 1
		m_rgCtrlBtn[0].SetCaption(_T("Search\nDirection"));
		m_rgiValue[0] = static_cast<int>(m_plnkVision->GetEdgeFinderDirection(m_iCamNo, m_iModelNo));
		if (m_rgiValue[0] == 0)	// Horizontal ?
		{
			strTmpValue.Format(_T("Horizontal"));
		}
		else if (m_rgiValue[0] == 1)	// Vertical
		{
			strTmpValue.Format(_T("Vertical"));
		}
		m_rgCtrlLbl[0].SetCaption(strTmpValue);

		// 2
		m_rgCtrlBtn[1].SetCaption(_T("Polarity\n[B:0 / W:1]"));
		m_rgiValue[1] = static_cast<int>(m_plnkVision->GetEdgeFinderPolarity(m_iCamNo, m_iModelNo));
		if (m_rgiValue[1] == 0)	// Black
		{
			strTmpValue.Format(_T("Black"));
		}
		else if (m_rgiValue[1] == 1)	// White
		{
			strTmpValue.Format(_T("White"));
		}
		m_rgCtrlLbl[1].SetCaption(strTmpValue);

		// 3
		m_rgCtrlBtn[2].SetCaption(_T("Threshold\n[1 ~ 255]"));
		m_rgiValue[2] = static_cast<int>(m_plnkVision->GetEdgeFinderThreshold(m_iCamNo, m_iModelNo));
		strTmpValue.Format(_T("%d"), m_rgiValue[2]);
		m_rgCtrlLbl[2].SetCaption(strTmpValue);

		// 4
		m_rgCtrlBtn[3].SetCaption(_T("Max Number"));
/*		m_rgiValue[3] = static_cast<int>(m_plnkVision->GetEdgeFinderNumOfResults(m_iCamNo, m_iModelNo));
		strTmpValue.Format(_T("%d", m_rgiValue[3]);
		m_rgCtrlLbl[3].SetCaption(strTmpValue);
*/		break;
	default:
		break;
	}

	// 조명 값 Display
	displayLightingValue();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	
	CString szTextTemp;
	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_1"), &szTextTemp);	
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_2"), &szTextTemp);	
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);

// 	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_3"), &szTextTemp);	
// 	//검색 방향
// 	GetDlgItem(IDB_DIRECTION)->SetWindowText(szTextTemp);
// 	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_4"), &szTextTemp);	
// 	//검색 색상
// 	GetDlgItem(IDB_COLOR)->SetWindowText(szTextTemp);
// 	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_5"), &szTextTemp);	
// 	//Threshold 값
// 	GetDlgItem(IDB_THRESHOLD)->SetWindowText(szTextTemp);
// 	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_6"), &szTextTemp);	
// 	//ACF 회수부 장착
// 	GetDlgItem(IDB_MAX_NUM)->SetWindowText(szTextTemp);
 	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_7"), &szTextTemp);	
 	//조명
 	GetDlgItem(IDB_ILLUMINATION1)->SetWindowText(szTextTemp);
 	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_8"), &szTextTemp);	
 	//측광
 	GetDlgItem(IDB_ILLUMINATION1_ONOFF)->SetWindowText(szTextTemp);
 	ReadDisplayMessage(SUB_DLG, _T("VisionOptionDlg_9"), &szTextTemp);	
 	//동축
 	GetDlgItem(IDB_ILLUMINATION2_ONOFF)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVisionOptionDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionOptionDlg)
	ON_EVENT(CVisionOptionDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_DIRECTION, -600 /* Click */, OnDirection, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_COLOR, -600 /* Click */, OnColor, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_THRESHOLD, -600 /* Click */, OnThreshold, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_MAX_NUM, -600 /* Click */, OnMaxNum, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_CAM_CHANGE_TIME, -600 /* Click */, OnCamChangeTime, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_VISION_WAIT, -600 /* Click */, OnVisionWait, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_ILLUMINATION1, -600 /* Click */, OnClickIllumination1, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_ILLUMINATION1_ONOFF, -600 /* Click */, OnIllumination1OnOff, VTS_NONE)
	ON_EVENT(CVisionOptionDlg, IDB_ILLUMINATION2_ONOFF, -600 /* Click */, OnIllumination2OnOff, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVisionOptionDlg::OnExit() 
{
	CDialog::OnCancel();
}

void CVisionOptionDlg::OnSave() 
{
	if (!m_bIsValidCamNo)
		return;

	int iResult;
	switch (m_eMarkType)
	{
	case DEF_PM_MARK:
		iResult = m_plnkVision->SetSearchAcceptanceThreshold(m_iCamNo, m_iModelNo, m_rgiValue[0]);
		break;
	case DEF_BLOB_MARK:
		iResult = m_plnkVision->SetBlobThreshold(m_iCamNo, m_iModelNo, m_rgiValue[0]);
		if (iResult)
		{
			MyMessageBox("Threshold Error", "Vision Option Error", M_ICONERROR);
			return;
		}
		
		iResult = m_plnkVision->SetBlobPolarity(m_iCamNo, m_iModelNo, m_rgiValue[1]);
		if (iResult)
		{
			MyMessageBox("Polarity Error", "Vision Option Error", M_ICONERROR);
			return;
		}

		//170616 JSH.s
		iResult = m_plnkVision->SetBlobJudgeMinPixelPercent(m_iCamNo, m_iModelNo, m_rgiValue[2]);
		if (iResult)
		{
			MyMessageBox("Polarity Error", "Vision Option Error", M_ICONERROR);
			return;
		}
		
		iResult = m_plnkVision->SetBlobJudgeMaxPixelPercent(m_iCamNo, m_iModelNo, m_rgiValue[3]);
		if (iResult)
		{
			MyMessageBox("Polarity Error", "Vision Option Error", M_ICONERROR);
			return;
		}
		//170616 JSH.e
		break;
	case DEF_CALIPER_MARK:
		// 1
		iResult = m_plnkVision->SetEdgeFinderDirection(m_iCamNo, m_iModelNo, (double)m_rgiValue[0]);
		if (iResult)
		{
			MyMessageBox("EdgeFinderDirection Error", "Vision Option Error", M_ICONERROR);
			return;
		}
		// 2
		iResult = m_plnkVision->SetEdgeFinderPolarity(m_iCamNo, m_iModelNo, (double)m_rgiValue[1]);
		if (iResult)
		{
			MyMessageBox("EdgeFinderPolarity Error", "Vision Option Error", M_ICONERROR);
			return;
		}
		// 3
		iResult = m_plnkVision->SetEdgeFinderThreshold(m_iCamNo, m_iModelNo, (double)m_rgiValue[2]);
		if (iResult)
		{
			MyMessageBox("EdgeFinderPolarity Error", "Vision Option Error", M_ICONERROR);
			return;
		}
		// 4
/*		iResult = m_plnkVision->SetEdgeFinderNumOfResults(m_iCamNo, m_iModelNo, (double)m_rgiValue[3]);
		if (iResult)
		{
			MyMessageBox("EdgeFinderPolarity Error", "Vision Option Error", M_ICONERROR);
			return;
		}
*/
		break;
	default:
		break;
	}
/*
	if (iResult)
	{
		CString strErrorMsg = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox((char *)((LPCTSTR)strErrorMsg), "Vision Option Error", M_ICONERROR);
		return;
	}
*/
	iResult = m_plnkVision->WriteModelData(m_iCamNo, m_eMarkType, m_iModelNo);
	if (iResult)
	{
		//Model 쓰기 에러
		MyMessageBox(MY_MESSAGE_BOX,64501, _T("Vision Option Error"), M_ICONERROR);
		return;
	}
}

void CVisionOptionDlg::OnDirection() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	CString strTmpValue;

	memset(cAfterVal, 0, sizeof(cAfterVal));
	swprintf(cBeforeVal,_T("%d"), m_rgiValue[0]);
	
	
	switch (m_eMarkType)
	{
	case DEF_PM_MARK:
		if(!GetNumberBox(cAfterVal, 10, cBeforeVal))
			return;

		m_rgiValue[0] = _wtoi(cAfterVal);

		if ( m_rgiValue[0] >= 100 )
			m_rgiValue[0] = 100;
		if ( m_rgiValue[0] <= 50 )
			m_rgiValue[0] = 50;
		
		strTmpValue.Format(_T("%d"), m_rgiValue[0]);
		break;

	case DEF_BLOB_MARK:
		//20171027 KDW Start
		{
 			int iReturnThreshold = m_rgiValue[0];
			
			CVisionBlobThresholdDlg dlg(m_plnkVision, m_iCamNo, m_iModelNo, &iReturnThreshold, this);
			dlg.DoModal();
			
			m_rgiValue[0] = iReturnThreshold;
		}
		//20171027 KDW End
		strTmpValue.Format(_T("%d"), m_rgiValue[0]);
		break;

	case DEF_CALIPER_MARK:
		if (m_rgiValue[0] == 0)	// Horizontal ?
		{
			strTmpValue.Format(_T("Horizontal"));
		}
		else if (m_rgiValue[0] == 1)	// Vertical
		{
			strTmpValue.Format(_T("Vertical"));
		}
		break;

	default:
		return;
	}
	m_rgCtrlLbl[0].SetCaption(strTmpValue);
}

void CVisionOptionDlg::OnColor() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	CString strTmpValue;

	memset(cAfterVal, 0, sizeof(cAfterVal));
	swprintf(cBeforeVal,_T("%d"), m_rgiValue[1]);
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;
	
	m_rgiValue[1] = _wtoi(cAfterVal);
	
	switch (m_eMarkType)
	{
	case DEF_PM_MARK:
		return;
	case DEF_BLOB_MARK:
	case DEF_CALIPER_MARK:
		if (m_rgiValue[1] == 0)			// Black
		{
			strTmpValue.Format(_T("Black"));
		}
		else if (m_rgiValue[1] == 1)	// White
		{
			strTmpValue.Format(_T("White"));
		}
		break;
	default:
		return;
	}
	m_rgCtrlLbl[1].SetCaption(strTmpValue);
}

void CVisionOptionDlg::OnThreshold() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	CString strTmpValue;

	memset(cAfterVal, 0, sizeof(cAfterVal));
	swprintf(cBeforeVal,_T("%d"), m_rgiValue[2]);
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;
	
	m_rgiValue[2] = _wtoi(cAfterVal);

	switch (m_eMarkType)
	{
	case DEF_PM_MARK:
		break;
	case DEF_BLOB_MARK:
		// 잘못된 입력의 경우 나중에 확인할 것.

		if ( m_rgiValue[2] <= 0 )
			m_rgiValue[2] = 0;

		strTmpValue.Format(_T("%d"), m_rgiValue[2]);
		break;
	case DEF_CALIPER_MARK:
		if ( m_rgiValue[2] >= 255 )
			m_rgiValue[2] = 255;
		if ( m_rgiValue[2] <= 0 )
			m_rgiValue[2] = 0;

		strTmpValue.Format(_T("%d"), m_rgiValue[2]);
		break;
	default:
		return;
	}
	m_rgCtrlLbl[2].SetCaption(strTmpValue);
}

void CVisionOptionDlg::OnMaxNum() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	CString strTmpValue;

	memset(cAfterVal, 0, sizeof(cAfterVal));
	swprintf(cBeforeVal,_T("%d"), m_rgiValue[3]);
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	m_rgiValue[3] = _wtoi(cAfterVal);

	switch (m_eMarkType)
	{
	case DEF_PM_MARK:
		break;
	case DEF_BLOB_MARK:
		// 잘못된 입력의 경우 나중에 확인할 것.
		if (m_rgiValue[3] <= m_rgiValue[2])
			return ;
		/* Fall Through */
	case DEF_CALIPER_MARK:
		if ( m_rgiValue[3] <= 0 )
			m_rgiValue[3] = 0;

		strTmpValue.Format(_T("%d"), m_rgiValue[3]);
		break;

	default:
		return;
	}
	m_rgCtrlLbl[3].SetCaption(strTmpValue);
}

void CVisionOptionDlg::OnCamChangeTime() 
{
	
}

void CVisionOptionDlg::OnVisionWait() 
{
	
}

void CVisionOptionDlg::OnClickIllumination1() 
{
	CVisionIlluminationDlg dlg(m_iCamNo);
	dlg.DoModal();

	displayLightingValue();
}

void CVisionOptionDlg::displayLightingValue()
{
	CString strValue1, strValue2;
	
	if (s_rgLightIdForCam[m_iCamNo].x != -1)
	{
		strValue1.Format(_T("%d"), m_plnkPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[m_iCamNo].x]);
		m_lblIllumination1.SetCaption(strValue1);

		ILighting* plnkLighting = MOlbSystemPre.GetLightingComponent(s_rgLightIdForCam[m_iCamNo].x);
		m_btnillumination1.SetValue(plnkLighting->IsOnLighting());
	}
	else
	{
		m_btnillumination1.EnableWindow(FALSE);
		m_lblIllumination1.SetCaption(_T("-"));
	}
	
	if (s_rgLightIdForCam[m_iCamNo].y != -1)
	{
		strValue2.Format(_T("%d"), m_plnkPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[m_iCamNo].y]);
		m_lblIllumination2.SetCaption(strValue2);

		ILighting* plnkLighting = MOlbSystemPre.GetLightingComponent(s_rgLightIdForCam[m_iCamNo].y);
		m_btnillumination2.SetValue(plnkLighting->IsOnLighting());
	}
	else
	{
		m_btnillumination2.EnableWindow(FALSE);
		m_lblIllumination2.SetCaption(_T("-"));
	}
}

void CVisionOptionDlg::OnIllumination1OnOff() 
{
	ILighting* plnkLighting = MOlbSystemPre.GetLightingComponent(s_rgLightIdForCam[m_iCamNo].x);
	if (TRUE == plnkLighting->IsOnLighting())
		plnkLighting->LightOff();
	else
		plnkLighting->LightOn();
	
	m_btnillumination1.SetValue(plnkLighting->IsOnLighting());
}

void CVisionOptionDlg::OnIllumination2OnOff() 
{
	ILighting* plnkLighting = MOlbSystemPre.GetLightingComponent(s_rgLightIdForCam[m_iCamNo].y);
	if (TRUE == plnkLighting->IsOnLighting())
		plnkLighting->LightOff();
	else
		plnkLighting->LightOn();

	m_btnillumination2.SetValue(plnkLighting->IsOnLighting());
}
