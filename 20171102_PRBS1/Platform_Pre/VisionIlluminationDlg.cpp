// VisionIlluminationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "VisionIlluminationDlg.h"
#include "MPreBonderData.h"
#include "IVision.h"
#include "ILighting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CVisionIlluminationDlg dialog

CVisionIlluminationDlg::CVisionIlluminationDlg(int iCamNo, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionIlluminationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisionIlluminationDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
	m_plnkVision = MOlbSystemPre.GetVisionComponent();
	
	m_iCamNo = iCamNo;

	for (int i = 0; i < DEF_MAX_LIGHTING; i++)
	{
		m_plnkLighting[i] = MOlbSystemPre.GetLightingComponent(i);
	}
}

void CVisionIlluminationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionIlluminationDlg)
	DDX_Control(pDX, IDB_LIGHT1, m_bLight1);
	DDX_Control(pDX, IDB_LIGHT2, m_bLight2);
	DDX_Control(pDX, IDC_LIGHTENING_VALUE1, m_lblLightValue1);
	DDX_Control(pDX, IDC_LIGHTENING_VALUE2, m_lblLightValue2);
	DDX_Control(pDX, IDB_DOWN1, m_bDown1);
	DDX_Control(pDX, IDB_DOWN2, m_bDown2);
	DDX_Control(pDX, IDB_UP1, m_bUp1);
	DDX_Control(pDX, IDB_UP2, m_bUp2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisionIlluminationDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionIlluminationDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionIlluminationDlg message handlers

void CVisionIlluminationDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CVisionIlluminationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	CRect mainrect;
	CWnd* pParent = GetParent();
	pParent->GetWindowRect(&mainrect);
    GetWindowRect(&rect);
	MoveWindow(mainrect.right-rect.Width(), mainrect.bottom-rect.Height(), rect.Width(), rect.Height());

	if (m_plnkVision->IsValidCam(m_iCamNo) == FALSE)
		m_iCamNo = DEF_CAMERA_1;

	for (int i = 0; i < DEF_MAX_LIGHTING; i++)
		m_rgiLightening[i] = m_plnkPreBonderData->m_rgiLightingValue[i];
	
	displayUpdate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVisionIlluminationDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionIlluminationDlg)
	ON_EVENT(CVisionIlluminationDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CVisionIlluminationDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CVisionIlluminationDlg, IDB_LIGHT1, -600 /* Click */, OnClickLight1, VTS_NONE)
	ON_EVENT(CVisionIlluminationDlg, IDB_LIGHT2, -600 /* Click */, OnClickLight2, VTS_NONE)
	ON_EVENT(CVisionIlluminationDlg, IDB_UP1, -600 /* Click */, OnClickUp1, VTS_NONE)
	ON_EVENT(CVisionIlluminationDlg, IDB_DOWN1, -600 /* Click */, OnClickDown1, VTS_NONE)
	ON_EVENT(CVisionIlluminationDlg, IDB_UP2, -600 /* Click */, OnClickUp2, VTS_NONE)
	ON_EVENT(CVisionIlluminationDlg, IDB_DOWN2, -600 /* Click */, OnClickDown2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVisionIlluminationDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	setLightingValue();

	CDialog::OnCancel();
}

int CVisionIlluminationDlg::setLightingValue()
{
	if (s_rgLightIdForCam[m_iCamNo].x != -1)
	{
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].x]->SetValue((unsigned short)m_plnkPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[m_iCamNo].x]);
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].x]->LightOn();
	}

	if (s_rgLightIdForCam[m_iCamNo].y != -1)
	{
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].y]->SetValue((unsigned short)m_plnkPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[m_iCamNo].y]);
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].y]->LightOn();
	}
	return 0;
}

void CVisionIlluminationDlg::OnSave() 
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < DEF_MAX_LIGHTING; i++)
		m_plnkPreBonderData->m_rgiLightingValue[i] = m_rgiLightening[i];

	m_plnkPreBonderData->WriteData();
	setLightingValue();
}

void CVisionIlluminationDlg::displayUpdate()
{
	CString strValue1, strValue2;
	CString strName1, strName2 ;
	CString strTemp;
	CString szTemp;
	
	if (s_rgLightIdForCam[m_iCamNo].x != -1)
	{
		strName1.Format(_T("Cam%d "), m_iCamNo + 1);
		//Ãø±¤
		ReadDisplayMessage(SUB_DLG, _T("VisionIlluminationDlg_1"), &szTemp);
		strName1 += szTemp;
		strValue1.Format(_T("%d"), m_rgiLightening[s_rgLightIdForCam[m_iCamNo].x]);
		
		m_bLight1.SetCaption(strName1);
		m_lblLightValue1.SetCaption(strValue1);
	}
	else
	{
		m_bLight1.SetCaption(_T("-"));
		m_bLight1.EnableWindow(FALSE);
		
		m_lblLightValue1.SetCaption(_T("-"));
		m_lblLightValue1.EnableWindow(FALSE);

		m_bUp1.EnableWindow(FALSE);
		m_bDown1.EnableWindow(FALSE);
	}
	
	if (s_rgLightIdForCam[m_iCamNo].y != -1)
	{
		strName2.Format(_T("Cam%d "), m_iCamNo + 1);
		//µ¿Ãà
		ReadDisplayMessage(SUB_DLG, _T("VisionIlluminationDlg_2"), &szTemp);
		strName2 += szTemp;
		strValue2.Format(_T("%d"), m_rgiLightening[s_rgLightIdForCam[m_iCamNo].y]);
		
		m_bLight2.SetCaption(strName2);
		m_lblLightValue2.SetCaption(strValue2);
	}
	else
	{
		m_bLight2.SetCaption(_T("-"));
		m_bLight2.EnableWindow(FALSE);
		
		m_lblLightValue2.SetCaption(_T("-"));
		m_lblLightValue2.EnableWindow(FALSE);

		m_bUp2.EnableWindow(FALSE);
		m_bDown2.EnableWindow(FALSE);
	}
}

void CVisionIlluminationDlg::setLightValue(int iLightType)
{
	WCHAR szOldValue[10];
	WCHAR szNewValue[10];
	
	if (iLightType == DEF_TABIC_LIGHT)	// Ãø±¤
	{
		m_lblLightValue1.GetWindowText(szOldValue, 10);
	}
	else if (iLightType == DEF_PANEL_LIGHT)	// µ¿Ãà
	{
		m_lblLightValue2.GetWindowText(szOldValue, 10);
	}
	
	if (!GetNumberBox(szNewValue, 10, szOldValue))
		return;
	
	int iLightID = -1;
	if (iLightType == DEF_TABIC_LIGHT)	// Ãø±¤
	{
		iLightID = s_rgLightIdForCam[m_iCamNo].x;
	}
	else if (iLightType == DEF_PANEL_LIGHT)	// µ¿Ãà
	{
		iLightID = s_rgLightIdForCam[m_iCamNo].y;
	}

	int* piLightingValue = &(m_rgiLightening[iLightID]);

	*piLightingValue = _wtoi(szNewValue);
	
	if (*piLightingValue > DEF_LIGHTING_MAX_VALUE)
		*piLightingValue = DEF_LIGHTING_MAX_VALUE;
	else if (*piLightingValue < 0)
		*piLightingValue = 0;
	
	displayUpdate();

	if (iLightType == DEF_TABIC_LIGHT)	// Ãø±¤
	{
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].x]->SetValue((unsigned short)m_rgiLightening[s_rgLightIdForCam[m_iCamNo].x]);
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].x]->LightOn();
	}
	else if (iLightType == DEF_PANEL_LIGHT)	// µ¿Ãà
	{
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].y]->SetValue((unsigned short)m_rgiLightening[s_rgLightIdForCam[m_iCamNo].y]);
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].y]->LightOn();
	}
}

void CVisionIlluminationDlg::OnClickLight1() // Tab Á¶¸í : Ãø±¤
{
	setLightValue(DEF_TABIC_LIGHT);
}

void CVisionIlluminationDlg::OnClickLight2() // Panel Á¶¸í : µ¿Ãà
{
	setLightValue(DEF_PANEL_LIGHT);
}

void CVisionIlluminationDlg::changeLightValue(int iLightType, int iChangeValue)
{
	int iLightID = -1;
	if (iLightType == DEF_TABIC_LIGHT)	// Ãø±¤
	{
		iLightID = s_rgLightIdForCam[m_iCamNo].x;
	}
	else if (iLightType == DEF_PANEL_LIGHT)	// µ¿Ãà
	{
		iLightID = s_rgLightIdForCam[m_iCamNo].y;
	}
	
	int* piLightingValue = &(m_rgiLightening[iLightID]);

	*piLightingValue += iChangeValue;
	
	if (*piLightingValue > DEF_LIGHTING_MAX_VALUE)
		*piLightingValue = DEF_LIGHTING_MAX_VALUE;
	else if (*piLightingValue < 0)
		*piLightingValue = 0;
	
	displayUpdate();

	if (iLightType == DEF_TABIC_LIGHT)	// Ãø±¤
	{
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].x]->SetValue((unsigned short)m_rgiLightening[s_rgLightIdForCam[m_iCamNo].x]);
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].x]->LightOn();
	}
	else if (iLightType == DEF_PANEL_LIGHT)	// µ¿Ãà
	{
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].y]->SetValue((unsigned short)m_rgiLightening[s_rgLightIdForCam[m_iCamNo].y]);
		m_plnkLighting[s_rgLightIdForCam[m_iCamNo].y]->LightOn();
	}
}

void CVisionIlluminationDlg::OnClickUp1() 
{
	changeLightValue(DEF_TABIC_LIGHT, 5);
}

void CVisionIlluminationDlg::OnClickDown1() 
{
	changeLightValue(DEF_TABIC_LIGHT, -5);
}

void CVisionIlluminationDlg::OnClickUp2() 
{
	changeLightValue(DEF_PANEL_LIGHT, 5);
}

void CVisionIlluminationDlg::OnClickDown2() 
{
	changeLightValue(DEF_PANEL_LIGHT, -5);
}
