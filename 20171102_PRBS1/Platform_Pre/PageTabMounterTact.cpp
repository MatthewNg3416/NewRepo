// PageTabMounterTact.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "PageTabMounterTact.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CPageTabMounterTact dialog
IMPLEMENT_DYNCREATE(CPageTabMounterTact, CPropertyPage)

CPageTabMounterTact::CPageTabMounterTact() : CPropertyPage(CPageTabMounterTact::IDD)
{
	//{{AFX_DATA_INIT(CPageTabMounterTact)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hAutoMainWnd = NULL;
	m_plnkAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
}

void CPageTabMounterTact::SetAutoMainWnd(HWND hMainView)
{
	m_hAutoMainWnd = hMainView;
}

CPageTabMounterTact::~CPageTabMounterTact()
{
}



void CPageTabMounterTact::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageTabMounterTact)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageTabMounterTact, CPropertyPage)
	//{{AFX_MSG_MAP(CPageTabMounterTact)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageTabMounterTact message handlers

BOOL CPageTabMounterTact::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	RenewTabMounterTactTime();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageTabMounterTact::OnKillActive() 
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

BOOL CPageTabMounterTact::OnSetActive() 
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

void CPageTabMounterTact::RenewTabMounterTactTime()
{	
	ClearDisplay();
	UpdateDisplay();
}


void CPageTabMounterTact::ClearDisplay()
{

}

void CPageTabMounterTact::UpdateDisplay()
{
	
}