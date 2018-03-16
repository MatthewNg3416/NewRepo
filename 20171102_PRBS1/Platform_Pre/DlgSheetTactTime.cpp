// DlgSheetTactTime.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "DlgSheetTactTime.h"
#include "PageTabMounterTact.h"
#include "PageTabCarrierTact.h"
#include "PageEtcTact.h"
#include "MPlatformOlbSystem.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSheetTactTime dialog


CDlgSheetTactTime::CDlgSheetTactTime(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSheetTactTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSheetTactTime)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPageTabMounterTact = NULL;
	m_pPageTabCarrierTact	= NULL;
	m_pPageEtcTact		= NULL;
	if (pParent != NULL)
		m_hAutoMainWnd = pParent->m_hWnd;
	else
		m_hAutoMainWnd = NULL;
}

CDlgSheetTactTime::~CDlgSheetTactTime()
{
	if (m_pPageTabMounterTact != NULL)
		delete m_pPageTabMounterTact;
	if (m_pPageTabCarrierTact != NULL)
		delete m_pPageTabCarrierTact;
	if (m_pPageEtcTact != NULL)
		delete m_pPageEtcTact;
}

void CDlgSheetTactTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSheetTactTime)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSheetTactTime, CDialog)
	//{{AFX_MSG_MAP(CDlgSheetTactTime)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSheetTactTime message handlers

BOOL CDlgSheetTactTime::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	CString strTemp;

	//¨Ï©ª¡§¡Ì¢®¨¡¡Ë¢ç¢®¨úa
	ReadDisplayMessage(MAIN_MANUAL, _T("DlgSheetTactTime_1"), &strTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(strTemp);

	
	// TODO: Add extra initialization here
	createSheet();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSheetTactTime::createSheet()
{
	if (m_pPageTabMounterTact == NULL)
		m_pPageTabMounterTact = new CPageTabMounterTact();
	if (m_pPageTabCarrierTact == NULL)
		m_pPageTabCarrierTact = new CPageTabCarrierTact();
	if (m_pPageEtcTact == NULL)
		m_pPageEtcTact = new CPageEtcTact();
	
	m_pPageEtcTact->SetAutoMainWnd(m_hAutoMainWnd);
	m_pPageTabMounterTact->SetAutoMainWnd(m_hAutoMainWnd);
	m_pPageTabCarrierTact->SetAutoMainWnd(m_hAutoMainWnd);

	m_sheet.AddPage(m_pPageTabMounterTact);
	m_sheet.AddPage(m_pPageTabCarrierTact);
	m_sheet.AddPage(m_pPageEtcTact);

	m_sheet.EnableStackedTabs( FALSE );
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE , 0);
	m_sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT|WS_EX_DLGMODALFRAME);
//	m_sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_sheet.ModifyStyle( 0, WS_TABSTOP );

	m_sheet.GetTabControl()->SetItemSize(CSize(100, 30));
	m_sheet.GetTabControl()->SetMinTabWidth(100);
	m_sheet.SetActivePage(m_pPageTabMounterTact);

	m_sheet.GetTabControl()->SetItemSize(CSize(100, 30));
	m_sheet.GetTabControl()->SetMinTabWidth(100);
	m_sheet.SetActivePage(m_pPageTabCarrierTact);

	m_sheet.GetTabControl()->SetItemSize(CSize(100, 30));
	m_sheet.GetTabControl()->SetMinTabWidth(100);
	m_sheet.SetActivePage(m_pPageEtcTact);

	m_sheet.GetTabControl()->SetItemSize(CSize(100, 30));
	m_sheet.GetTabControl()->SetMinTabWidth(100);
	m_sheet.SetActivePage(m_pPageTabMounterTact);

}

BEGIN_EVENTSINK_MAP(CDlgSheetTactTime, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgSheetTactTime)
	ON_EVENT(CDlgSheetTactTime, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDlgSheetTactTime::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
//	ShowWindow(SW_HIDE);
}

void CDlgSheetTactTime::RenewTactTimeCode()
{
	/*
	if (m_pPageEtcTact != NULL)
		m_pPageEtcTact->RenewTactTimeCode();
	if (m_pPageTabMounterTact != NULL)
		m_pPageTabMounterTact->RenewTactTimeCode();
	if (m_pPageTabCarrierTact != NULL)
		m_pPageTabCarrierTact->RenewTactTimeCode();
		*/
}
