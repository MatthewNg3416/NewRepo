// VisionSearchAreaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Common.h"
#include "MPlatformOlb.h"
#include "VisionSearchAreaDlg.h"
#include "IVision.h"
#include "MyMsgWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisionSearchAreaDlg dialog

CVisionSearchAreaDlg::CVisionSearchAreaDlg(IVision* plnkVision, int iCamNo, int iMarkNo, char* szMarkName, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionSearchAreaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisionSearchAreaDlg)
	//}}AFX_DATA_INIT

	m_iCamNo = iCamNo;
	m_iMarkNo = iMarkNo;
	m_pszMarkName = szMarkName;
	m_plnkVision = plnkVision;
	m_iCenterX = 0;
	m_iCenterY = 0;
	m_iWidth = 0;
	m_iHeight = 0;
}


void CVisionSearchAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionSearchAreaDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_SELECT_MARK1, m_btnCtrl);
	DDX_Control(pDX, IDC_CENTER_X1, m_lblCenterX);
	DDX_Control(pDX, IDC_CENTER_Y1, m_lblCenterY);
	DDX_Control(pDX, IDC_WIDTH1, m_lblWidth);
	DDX_Control(pDX, IDC_HEIGHT1, m_lblHeight);
}


BEGIN_MESSAGE_MAP(CVisionSearchAreaDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionSearchAreaDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionSearchAreaDlg message handlers

BEGIN_EVENTSINK_MAP(CVisionSearchAreaDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionSearchAreaDlg)
	ON_EVENT(CVisionSearchAreaDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CVisionSearchAreaDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CVisionSearchAreaDlg, IDB_CENTER_X, -600 /* Click */, OnClickCenterX, VTS_NONE)
	ON_EVENT(CVisionSearchAreaDlg, IDB_CENTER_Y, -600 /* Click */, OnClickCenterY, VTS_NONE)
	ON_EVENT(CVisionSearchAreaDlg, IDB_WIDTH, -600 /* Click */, OnClickWidth, VTS_NONE)
	ON_EVENT(CVisionSearchAreaDlg, IDB_HEIGHT, -600 /* Click */, OnClickHeight, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CVisionSearchAreaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString szTextTemp;
	ReadDisplayMessage(SUB_DLG, _T("VisionSearchAreaDlg_1"), &szTextTemp);	
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionSearchAreaDlg_2"), &szTextTemp);	
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionSearchAreaDlg_3"), &szTextTemp);	
	//X 중심
	GetDlgItem(IDB_CENTER_X)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionSearchAreaDlg_4"), &szTextTemp);	
	//Y 중심
	GetDlgItem(IDB_CENTER_Y)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionSearchAreaDlg_5"), &szTextTemp);	
	//너비
	GetDlgItem(IDB_WIDTH)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionSearchAreaDlg_6"), &szTextTemp);	
	//높이
	GetDlgItem(IDB_HEIGHT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(SUB_DLG, _T("VisionSearchAreaDlg_7"), &szTextTemp);	
	//Mark 이름
	GetDlgItem(IDB_SELECT_MARK1)->SetWindowText(szTextTemp);


	CString strName(m_pszMarkName);	// Mark Name String 가공
	strName.Replace(_T("DEF_"), _T(""));
	strName.Replace(_T("CALIBRATION"), _T("Calibration"));
	strName.Replace(_T("FIDU"), _T("Fiducial"));
	strName.Replace(_T("SUB"), _T("Sub"));
	strName.Replace(_T("PANEL"), _T("Panel"));
	strName.Replace(_T("TAB"), _T("Tab"));
	strName.Replace(_T("LEAD"), _T("Lead"));
	strName.Replace(_T("BLOB"), _T("Blob"));
	strName.Replace(_T("INSPECTION"), _T("Inspection"));
	strName.Replace(_T("EDGE"), _T("Edge"));
	strName.Replace(_T("AFTER"), _T("after"));
	strName.Replace(_T("MOUNT"), _T("Mount"));
	strName.Replace(_T("_MARK"), _T("\nMark"));
	strName.Replace(_T("XU"), _T("Xu"));
	strName.Replace(_T("XD"), _T("Xd"));
	strName.Replace(_T("YL"), _T("Yl"));
	strName.Replace(_T("YR"), _T("Yr"));
	strName.Replace(_T("RESERVED"), _T("Reserved"));
	strName.Replace(_T("_"), _T(" "));
	m_btnCtrl.SetCaption(strName);

	CRect rectSearch = m_plnkVision->GetSearchAreaRect(m_iCamNo, m_iMarkNo);

	CString strTemp;
	strTemp.Format(_T("%d"), rectSearch.CenterPoint().x);
	m_lblCenterX.SetCaption(strTemp);
	m_iCenterX = rectSearch.CenterPoint().x;
	
	strTemp.Format(_T("%d"), rectSearch.CenterPoint().y);
	m_lblCenterY.SetCaption(strTemp);
	m_iCenterY = rectSearch.CenterPoint().y;
	
	strTemp.Format(_T("%d"), rectSearch.Width());
	m_lblWidth.SetCaption(strTemp);
	m_iWidth = rectSearch.Width();
	
	strTemp.Format(_T("%d"), rectSearch.Height());
	m_lblHeight.SetCaption(strTemp);
	m_iHeight = rectSearch.Height();

		return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVisionSearchAreaDlg::OnClickCenterX() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];

	m_lblCenterX.GetWindowText(cBeforeVal, 30);

	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	m_iCenterX = _wtoi(cAfterVal);
	m_lblCenterX.SetCaption(cAfterVal);
}

void CVisionSearchAreaDlg::OnClickCenterY() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];

	m_lblCenterY.GetWindowText(cBeforeVal, 30);

	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	m_iCenterY = _wtoi(cAfterVal);
	m_lblCenterY.SetCaption(cAfterVal);
}

void CVisionSearchAreaDlg::OnClickWidth() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];

	m_lblWidth.GetWindowText(cBeforeVal, 30);

	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	m_iWidth = _wtoi(cAfterVal);
	m_lblWidth.SetCaption(cAfterVal);
}

void CVisionSearchAreaDlg::OnClickHeight() 
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];

	m_lblHeight.GetWindowText(cBeforeVal, 30);

	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	m_iHeight = _wtoi(cAfterVal);
	m_lblHeight.SetCaption(cAfterVal);	
}

void CVisionSearchAreaDlg::OnSave() 
{
	//111006 SJ_KJS Del..
	/*
	CMyMsgWnd mwnd(this);
	mwnd.SetMessage(_T("Saving.\nWait a Moment..."));
	mwnd.Show();
	mwnd.PeekAndPump();
	*/
	
	if (m_plnkVision->CheckModel(m_iCamNo, DEF_PM_MARK, m_iMarkNo) != ERR_VISION_SUCCESS)
		return;
	
	RECT rectSearch = {
		m_iCenterX - int(m_iWidth / 2),		// left
		m_iCenterY - int(m_iHeight / 2),	// top
		m_iCenterX + int(m_iWidth / 2),		// right
		m_iCenterY + int(m_iHeight / 2)		// bottom
	};
	
	if (rectSearch.left < 0)
		rectSearch.left = 0;

	if (rectSearch.top < 0)
		rectSearch.top = 0;

	if (rectSearch.right < 0)
		rectSearch.right = 0;

	if (rectSearch.bottom < 0)
		rectSearch.bottom = 0;

	POINT xySize = m_plnkVision->GetDistribution(m_iCamNo);

	if (rectSearch.left > xySize.x)
		rectSearch.left = xySize.x;

	if (rectSearch.top > xySize.y)
		rectSearch.top = xySize.y;

	if (rectSearch.right > xySize.x)
		rectSearch.right = xySize.x;

	if (rectSearch.bottom > xySize.y)
		rectSearch.bottom = xySize.y;

	m_plnkVision->SetSearchArea(m_iCamNo, m_iMarkNo, rectSearch);
	m_plnkVision->WriteModelData(m_iCamNo, DEF_PM_MARK, m_iMarkNo);
	
	//저장 완료
	MyMessageBox(MY_MESSAGE_BOX, 65000, _T("Information"), M_ICONINFORMATION);

}

void CVisionSearchAreaDlg::OnExit() 
{
	CDialog::OnCancel();	
}

