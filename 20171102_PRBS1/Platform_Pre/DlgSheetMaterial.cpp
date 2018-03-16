// DlgSheetMaterial.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "DlgSheetMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSheetMaterial dialog


CDlgSheetMaterial::CDlgSheetMaterial(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSheetMaterial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSheetMaterial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pPageMaterialInfo = NULL;
	m_pPageMCodeInfo	= NULL;
	m_pPageMCodeInfo2ND	= NULL;
	if (pParent != NULL)
		m_hAutoMainWnd = pParent->m_hWnd;
	else
		m_hAutoMainWnd = NULL;
}

CDlgSheetMaterial::~CDlgSheetMaterial()
{
	if (m_pPageMaterialInfo != NULL)
		delete m_pPageMaterialInfo;
	if (m_pPageMCodeInfo != NULL)
		delete m_pPageMCodeInfo;
	if (m_pPageMCodeInfo2ND != NULL)
		delete m_pPageMCodeInfo2ND;
}

void CDlgSheetMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSheetMaterial)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSheetMaterial, CDialog)
	//{{AFX_MSG_MAP(CDlgSheetMaterial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSheetMaterial message handlers

BOOL CDlgSheetMaterial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	createSheet();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSheetMaterial::createSheet()
{
	if (m_pPageMaterialInfo == NULL)
		m_pPageMaterialInfo = new CPageMaterialInfo();
	if (m_pPageMCodeInfo == NULL)
		m_pPageMCodeInfo = new CPageMCodeInfo();
	if (m_pPageMCodeInfo2ND == NULL)
		m_pPageMCodeInfo2ND = new CPageMCodeInfo2ND();
	
	m_pPageMaterialInfo->SetAutoMainWnd(m_hAutoMainWnd);
	m_pPageMCodeInfo->SetAutoMainWnd(m_hAutoMainWnd);
	m_pPageMCodeInfo2ND->SetAutoMainWnd(m_hAutoMainWnd);

	m_sheet.AddPage(m_pPageMCodeInfo);
	m_sheet.AddPage(m_pPageMCodeInfo2ND);
	m_sheet.AddPage(m_pPageMaterialInfo);

	m_sheet.EnableStackedTabs( FALSE );
	m_sheet.Create(this, WS_CHILD | WS_VISIBLE , 0);
	m_sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT|WS_EX_DLGMODALFRAME);
//	m_sheet.ModifyStyleEx (0, WS_EX_CONTROLPARENT);
	m_sheet.ModifyStyle( 0, WS_TABSTOP );

	m_sheet.GetTabControl()->SetItemSize(CSize(100, 30));
	m_sheet.GetTabControl()->SetMinTabWidth(100);
	m_sheet.SetActivePage(m_pPageMCodeInfo);

	m_sheet.GetTabControl()->SetItemSize(CSize(100, 30));
	m_sheet.GetTabControl()->SetMinTabWidth(100);
	m_sheet.SetActivePage(m_pPageMCodeInfo2ND);

	m_sheet.GetTabControl()->SetItemSize(CSize(100, 30));
	m_sheet.GetTabControl()->SetMinTabWidth(100);
	m_sheet.SetActivePage(m_pPageMaterialInfo);

	m_sheet.GetTabControl()->SetItemSize(CSize(100, 30));
	m_sheet.GetTabControl()->SetMinTabWidth(100);
	m_sheet.SetActivePage(m_pPageMCodeInfo);

	CRect rc;
	GetDlgItem(IDC_SHEET_FRAME)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_sheet.MoveWindow(&rc);
	GetDlgItem(IDC_SHEET_FRAME)->ShowWindow(SW_HIDE);
}

BEGIN_EVENTSINK_MAP(CDlgSheetMaterial, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgSheetMaterial)
	ON_EVENT(CDlgSheetMaterial, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDlgSheetMaterial::OnClickExit() 
{
	// TODO: Add your control notification handler code here
//	CDialog::OnOK();
	ShowWindow(SW_HIDE);
}

void CDlgSheetMaterial::RenewMaterialCode()
{
	if (m_pPageMaterialInfo != NULL)
		m_pPageMaterialInfo->RenewMaterialCode();
	if (m_pPageMCodeInfo != NULL)
		m_pPageMCodeInfo->RenewMaterialCode();
	if (m_pPageMCodeInfo2ND != NULL)
		m_pPageMCodeInfo2ND->RenewMaterialCode();
}