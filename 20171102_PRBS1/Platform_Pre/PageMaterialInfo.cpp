// PageMaterialInfo.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "PageMaterialInfo.h"
#include "MPlatformOlbSystem.h"
#include "MLCNet.h"
#include "MTrsAutoManager.h"
#include "MCtrlTabFeeder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CPageMaterialInfo property page

IMPLEMENT_DYNCREATE(CPageMaterialInfo, CPropertyPage)

CPageMaterialInfo::CPageMaterialInfo() : CPropertyPage(CPageMaterialInfo::IDD)
{
	//{{AFX_DATA_INIT(CPageMaterialInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hAutoMainWnd = NULL;
	m_plnkLCNet	 = MOlbSystemPre.GetLCNetComponent();
	m_plnkAutoManager	= MOlbSystemPre.GetTrsAutoManagerComponent();
	for(int i = 0; i <DEF_MAX_TABFEEDER; i++)
		m_plnkCtrlFeeder[i]	= MOlbSystemPre.GetCtrlTabFeederComponent(i);

}

void CPageMaterialInfo::SetAutoMainWnd(HWND hMainView)
{
	m_hAutoMainWnd = hMainView;
}

CPageMaterialInfo::~CPageMaterialInfo()
{
}

void CPageMaterialInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMaterialInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MID_S_1, m_lblMID[0]);
	DDX_Control(pDX, IDC_MID_S_2, m_lblMID[1]);
	DDX_Control(pDX, IDC_MID_S_3, m_lblMID[2]);
	DDX_Control(pDX, IDC_MID_S_4, m_lblMID[3]);
	DDX_Control(pDX, IDC_MID_S_5, m_lblMID[4]);
	DDX_Control(pDX, IDC_MID_S_6, m_lblMID[5]);
	DDX_Control(pDX, IDC_MID_S_7, m_lblMID[6]);
	DDX_Control(pDX, IDC_MID_S_8, m_lblMID[7]);
	DDX_Control(pDX, IDC_MID_S_9, m_lblMID[8]);
	DDX_Control(pDX, IDC_MID_S_10, m_lblMID[9]);
	
	DDX_Control(pDX, IDC_MCODE_S_1, m_lblMCode[0]);
	DDX_Control(pDX, IDC_MCODE_S_2, m_lblMCode[1]);
	DDX_Control(pDX, IDC_MCODE_S_3, m_lblMCode[2]);
	DDX_Control(pDX, IDC_MCODE_S_4, m_lblMCode[3]);
	DDX_Control(pDX, IDC_MCODE_S_5, m_lblMCode[4]);
	DDX_Control(pDX, IDC_MCODE_S_6, m_lblMCode[5]);
	DDX_Control(pDX, IDC_MCODE_S_7, m_lblMCode[6]);
	DDX_Control(pDX, IDC_MCODE_S_8, m_lblMCode[7]);
	DDX_Control(pDX, IDC_MCODE_S_9, m_lblMCode[8]);
	DDX_Control(pDX, IDC_MCODE_S_10, m_lblMCode[9]);

	DDX_Control(pDX, IDC_TQTY_S_1, m_lblTQty[0]);
	DDX_Control(pDX, IDC_TQTY_S_2, m_lblTQty[1]);
	DDX_Control(pDX, IDC_TQTY_S_3, m_lblTQty[2]);
	DDX_Control(pDX, IDC_TQTY_S_4, m_lblTQty[3]);
	DDX_Control(pDX, IDC_TQTY_S_5, m_lblTQty[4]);
	DDX_Control(pDX, IDC_TQTY_S_6, m_lblTQty[5]);
	DDX_Control(pDX, IDC_TQTY_S_7, m_lblTQty[6]);
	DDX_Control(pDX, IDC_TQTY_S_8, m_lblTQty[7]);
	DDX_Control(pDX, IDC_TQTY_S_9, m_lblTQty[8]);
	DDX_Control(pDX, IDC_TQTY_S_10, m_lblTQty[9]);

	DDX_Control(pDX, IDC_UQTY_S_1, m_lblUQty[0]);
	DDX_Control(pDX, IDC_UQTY_S_2, m_lblUQty[1]);
	DDX_Control(pDX, IDC_UQTY_S_3, m_lblUQty[2]);
	DDX_Control(pDX, IDC_UQTY_S_4, m_lblUQty[3]);
	DDX_Control(pDX, IDC_UQTY_S_5, m_lblUQty[4]);
	DDX_Control(pDX, IDC_UQTY_S_6, m_lblUQty[5]);
	DDX_Control(pDX, IDC_UQTY_S_7, m_lblUQty[6]);
	DDX_Control(pDX, IDC_UQTY_S_8, m_lblUQty[7]);
	DDX_Control(pDX, IDC_UQTY_S_9, m_lblUQty[8]);
	DDX_Control(pDX, IDC_UQTY_S_10, m_lblUQty[9]);

	DDX_Control(pDX, IDC_RQTY_S_1, m_lblRQty[0]);
	DDX_Control(pDX, IDC_RQTY_S_2, m_lblRQty[1]);
	DDX_Control(pDX, IDC_RQTY_S_3, m_lblRQty[2]);
	DDX_Control(pDX, IDC_RQTY_S_4, m_lblRQty[3]);
	DDX_Control(pDX, IDC_RQTY_S_5, m_lblRQty[4]);
	DDX_Control(pDX, IDC_RQTY_S_6, m_lblRQty[5]);
	DDX_Control(pDX, IDC_RQTY_S_7, m_lblRQty[6]);
	DDX_Control(pDX, IDC_RQTY_S_8, m_lblRQty[7]);
	DDX_Control(pDX, IDC_RQTY_S_9, m_lblRQty[8]);
	DDX_Control(pDX, IDC_RQTY_S_10, m_lblRQty[9]);

	DDX_Control(pDX, IDC_NQTY_S_1, m_lblNQty[0]);
	DDX_Control(pDX, IDC_NQTY_S_2, m_lblNQty[1]);
	DDX_Control(pDX, IDC_NQTY_S_3, m_lblNQty[2]);
	DDX_Control(pDX, IDC_NQTY_S_4, m_lblNQty[3]);
	DDX_Control(pDX, IDC_NQTY_S_5, m_lblNQty[4]);
	DDX_Control(pDX, IDC_NQTY_S_6, m_lblNQty[5]);
	DDX_Control(pDX, IDC_NQTY_S_7, m_lblNQty[6]);
	DDX_Control(pDX, IDC_NQTY_S_8, m_lblNQty[7]);
	DDX_Control(pDX, IDC_NQTY_S_9, m_lblNQty[8]);
	DDX_Control(pDX, IDC_NQTY_S_10, m_lblNQty[9]);

	DDX_Control(pDX, IDC_AQTY_S_1, m_lblAQty[0]);
	DDX_Control(pDX, IDC_AQTY_S_2, m_lblAQty[1]);
	DDX_Control(pDX, IDC_AQTY_S_3, m_lblAQty[2]);
	DDX_Control(pDX, IDC_AQTY_S_4, m_lblAQty[3]);
	DDX_Control(pDX, IDC_AQTY_S_5, m_lblAQty[4]);
	DDX_Control(pDX, IDC_AQTY_S_6, m_lblAQty[5]);
	DDX_Control(pDX, IDC_AQTY_S_7, m_lblAQty[6]);
	DDX_Control(pDX, IDC_AQTY_S_8, m_lblAQty[7]);
	DDX_Control(pDX, IDC_AQTY_S_9, m_lblAQty[8]);
	DDX_Control(pDX, IDC_AQTY_S_10, m_lblAQty[9]);

	DDX_Control(pDX, IDC_MSTEP1, m_lblMStep[0]);
	DDX_Control(pDX, IDC_MSTEP2, m_lblMStep[1]);
	DDX_Control(pDX, IDC_MSTEP3, m_lblMStep[2]);
	DDX_Control(pDX, IDC_MSTEP4, m_lblMStep[3]);
	DDX_Control(pDX, IDC_MSTEP5, m_lblMStep[4]);
	DDX_Control(pDX, IDC_MSTEP6, m_lblMStep[5]);
	DDX_Control(pDX, IDC_MSTEP7, m_lblMStep[6]);
	DDX_Control(pDX, IDC_MSTEP8, m_lblMStep[7]);
	DDX_Control(pDX, IDC_MSTEP9, m_lblMStep[8]);
	DDX_Control(pDX, IDC_MSTEP10, m_lblMStep[9]);

	DDX_Control(pDX, IDC_STATE_S_1, m_lblMState[0]);
	DDX_Control(pDX, IDC_STATE_S_2, m_lblMState[1]);
	DDX_Control(pDX, IDC_STATE_S_3, m_lblMState[2]);
	DDX_Control(pDX, IDC_STATE_S_4, m_lblMState[3]);
	DDX_Control(pDX, IDC_STATE_S_5, m_lblMState[4]);
	DDX_Control(pDX, IDC_STATE_S_6, m_lblMState[5]);
	DDX_Control(pDX, IDC_STATE_S_7, m_lblMState[6]);
	DDX_Control(pDX, IDC_STATE_S_8, m_lblMState[7]);
	DDX_Control(pDX, IDC_STATE_S_9, m_lblMState[8]);
	DDX_Control(pDX, IDC_STATE_S_10, m_lblMState[9]);

}


BEGIN_MESSAGE_MAP(CPageMaterialInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CPageMaterialInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMaterialInfo message handlers

BOOL CPageMaterialInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	RenewMaterialCode();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageMaterialInfo::OnKillActive() 
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

BOOL CPageMaterialInfo::OnSetActive() 
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

void CPageMaterialInfo::RenewMaterialCode()
{	
	ClearDisplay();
	UpdateDisplay();
}


void CPageMaterialInfo::ClearDisplay()
{
	for(int i=0; i<DEF_MAX_MATERIAL_ITEM; i++)
	{
		m_lblMID[i].SetCaption(_T(""));
		m_lblMCode[i].SetCaption(_T(""));
		m_lblTQty[i].SetCaption(_T(""));
		m_lblUQty[i].SetCaption(_T(""));
		m_lblRQty[i].SetCaption(_T(""));
		m_lblNQty[i].SetCaption(_T(""));		
		m_lblAQty[i].SetCaption(_T(""));
		m_lblMStep[i].SetCaption(_T(""));
		m_lblMState[i].SetCaption(_T(""));
	}
}

void CPageMaterialInfo::UpdateDisplay()
{
	CString	strTQty, strUQty, strNQty, strRQty, strAQty, strMState;
	int m_nState;
	int j = DEF_MAX_TABFEEDER;
	//Feeder
	for(int i=0; i<DEF_MAX_TABFEEDER; i++)
	{
		m_lblMID[i].SetCaption(m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_sM_BATCHID);
		m_lblMCode[i].SetCaption(m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_sM_CODE);
		strTQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_nT_QTY);
		strUQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_nU_QTY);
		strRQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_nR_QTY);
		strNQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_nN_QTY);
		strAQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_nA_QTY);
		m_lblTQty[i].SetCaption(strTQty);
		m_lblUQty[i].SetCaption(strUQty);
		m_lblRQty[i].SetCaption(strRQty);
		m_lblNQty[i].SetCaption(strNQty);
		m_lblAQty[i].SetCaption(strAQty);
		m_lblMStep[i].SetCaption(m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_sM_STEP);
		m_nState = m_plnkCtrlFeeder[i]->m_matTabICInfo[DEF_USING_REEL].m_nM_STATE;
		switch(m_nState)
		{
		case eMatStateIdle:
			strMState = "Idle";
			break;
		case eMatStateEnable:
			strMState = "Enable";
			break;
		case eMatStateRun:
			strMState = "Run";
			break;
		case eMatStateHold:
			strMState = "Hold";
			break;
		case eMatStateDisable:
			strMState = "Disable";
			break;
		case eMatStateRemove :
			strMState = "Remove";
			break;
		}
		m_lblMState[i].SetCaption(strMState);
		
		//Zig!
		m_lblMID[j].SetCaption(m_plnkCtrlFeeder[i]->m_matZigInfo.m_sM_BATCHID);
		m_lblMCode[j].SetCaption(m_plnkCtrlFeeder[i]->m_matZigInfo.m_sM_CODE);
		strTQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matZigInfo.m_nT_QTY);
		strUQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matZigInfo.m_nU_QTY);
		strRQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matZigInfo.m_nR_QTY);
		strNQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matZigInfo.m_nN_QTY);
		strAQty.Format(_T("%d"), m_plnkCtrlFeeder[i]->m_matZigInfo.m_nA_QTY);
		m_lblTQty[j].SetCaption(strTQty);
		m_lblUQty[j].SetCaption(strUQty);
		m_lblRQty[j].SetCaption(strRQty);
		m_lblNQty[j].SetCaption(strNQty);
		m_lblAQty[j].SetCaption(strAQty);
		m_lblMStep[j].SetCaption(m_plnkCtrlFeeder[i]->m_matZigInfo.m_sM_STEP);
		m_nState = m_plnkCtrlFeeder[i]->m_matZigInfo.m_nM_STATE;
		switch(m_nState)
		{
		case eMatStateIdle:
			strMState = "Idle";
			break;
		case eMatStateEnable:
			strMState = "Enable";
			break;
		case eMatStateRun:
			strMState = "Run";
			break;
		case eMatStateHold:
			strMState = "Hold";
			break;
		case eMatStateDisable:
			strMState = "Disable";
			break;
		case eMatStateRemove :
			strMState = "Remove";
			break;
		}
		m_lblMState[j].SetCaption(strMState);
		j++;
		//________________________
		
	}

	
}
