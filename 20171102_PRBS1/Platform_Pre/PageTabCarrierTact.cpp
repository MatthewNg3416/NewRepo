// PageTabCarrierTact.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "PageTabCarrierTact.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CPageTabCarrierTact dialog
IMPLEMENT_DYNCREATE(CPageTabCarrierTact, CPropertyPage)

CPageTabCarrierTact::CPageTabCarrierTact() : CPropertyPage(CPageTabCarrierTact::IDD)
{
	//{{AFX_DATA_INIT(CPageTabCarrierTact)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hAutoMainWnd = NULL;
	m_plnkAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
}

void CPageTabCarrierTact::SetAutoMainWnd(HWND hMainView)
{
	m_hAutoMainWnd = hMainView;
}

CPageTabCarrierTact::~CPageTabCarrierTact()
{
}


void CPageTabCarrierTact::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageTabCarrierTact)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageTabCarrierTact, CPropertyPage)
	//{{AFX_MSG_MAP(CPageTabCarrierTact)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageTabCarrierTact message handlers

BOOL CPageTabCarrierTact::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	RenewTabCarrierTactTime();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageTabCarrierTact::OnKillActive() 
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

BOOL CPageTabCarrierTact::OnSetActive() 
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

void CPageTabCarrierTact::RenewTabCarrierTactTime()
{	
	ClearDisplay();
	UpdateDisplay();
}


void CPageTabCarrierTact::ClearDisplay()
{

}

void CPageTabCarrierTact::UpdateDisplay()
{
	
}