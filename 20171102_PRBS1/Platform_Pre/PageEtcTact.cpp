// PageEtcTact.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "PageEtcTact.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CPageEtcTact dialog
IMPLEMENT_DYNCREATE(CPageEtcTact, CPropertyPage)

CPageEtcTact::CPageEtcTact() : CPropertyPage(CPageEtcTact::IDD)
{
	//{{AFX_DATA_INIT(CPageEtcTact)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hAutoMainWnd = NULL;
	m_plnkAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
}

void CPageEtcTact::SetAutoMainWnd(HWND hMainView)
{
	m_hAutoMainWnd = hMainView;
}

CPageEtcTact::~CPageEtcTact()
{
}

void CPageEtcTact::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageEtcTact)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CPageEtcTact, CPropertyPage)
	//{{AFX_MSG_MAP(CPageEtcTact)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageEtcTact message handlers


BOOL CPageEtcTact::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	RenewEtcTactTime();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageEtcTact::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* pParent = (CPropertySheet*)this->GetParent();
	if (pParent)
	{
		if (pParent->GetTabControl())
		{
	        short nActiveItem = pParent->GetPageIndex(this);
			pParent->GetTabControl()->HighlightItem(nActiveItem, FALSE);
		}
	}		
	
	return CPropertyPage::OnKillActive();
}

BOOL CPageEtcTact::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertySheet* pParent = (CPropertySheet*)this->GetParent();
    if (pParent)
	{
		if (pParent->GetTabControl())
		{
	        short nActiveItem = pParent->GetPageIndex(this);
            pParent->GetTabControl()->HighlightItem(nActiveItem);
		}
	}
	
	return CPropertyPage::OnSetActive();
}

void CPageEtcTact::RenewEtcTactTime()
{	
	ClearDisplay();
	UpdateDisplay();
}


void CPageEtcTact::ClearDisplay()
{

}

void CPageEtcTact::UpdateDisplay()
{
	
}