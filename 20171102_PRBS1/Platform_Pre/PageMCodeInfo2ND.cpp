// PageMCodeInfo2ND.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "PageMCodeInfo2ND.h"
#include "MPlatformOlbSystem.h"
#include "MLCNet.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CPageMCodeInfo2ND property page

IMPLEMENT_DYNCREATE(CPageMCodeInfo2ND, CPropertyPage)

CPageMCodeInfo2ND::CPageMCodeInfo2ND() : CPropertyPage(CPageMCodeInfo2ND::IDD)
{
	//{{AFX_DATA_INIT(CPageMCodeInfo2ND)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hAutoMainWnd		= NULL;
	m_plnkLCNet			= MOlbSystemPre.GetLCNetComponent();
	m_plnkAutoManager	= MOlbSystemPre.GetTrsAutoManagerComponent();
}

CPageMCodeInfo2ND::~CPageMCodeInfo2ND()
{
}

void CPageMCodeInfo2ND::SetAutoMainWnd(HWND hMainView)
{
	m_hAutoMainWnd = hMainView;
}

void CPageMCodeInfo2ND::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageMCodeInfo2ND)
	DDX_Control(pDX, IDB_REQUEST_S_CURRENT, m_btnRequestCurrentS);
	DDX_Control(pDX, IDB_REQUEST_G_CURRENT, m_btnRequestCurrentG);
	DDX_Control(pDX, IDB_REQUEST_S_NEXT, m_btnRequestNextS);
	DDX_Control(pDX, IDB_REQUEST_G_NEXT, m_btnRequestNextG);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_WORKORDER_CURRENT, m_lblWorkOrder[0]);
	DDX_Control(pDX, IDC_WORKORDER_NEXT, m_lblWorkOrder[1]);
	DDX_Control(pDX, IDC_WORKORDER_COMPLETE, m_lblWorkOrder[2]);

	DDX_Control(pDX, IDC_DEVICEID_CURRENT, m_lblDeviceID[0]);
	DDX_Control(pDX, IDC_DEVICEID_NEXT, m_lblDeviceID[1]);
	DDX_Control(pDX, IDC_DEVICEID_COMPLETE, m_lblDeviceID[2]);

	//[DEF_MAX_PRODUCT_PLAN][DEF_MAX_TABFEEDER]
	DDX_Control(pDX, IDC_MSTEP_S_CURRENT, m_lblStep[0][0]);
	DDX_Control(pDX, IDC_MSTEP_G_CURRENT, m_lblStep[0][1]);
	DDX_Control(pDX, IDC_MSTEP_S_NEXT, m_lblStep[1][0]);
	DDX_Control(pDX, IDC_MSTEP_G_NEXT, m_lblStep[1][1]);
	DDX_Control(pDX, IDC_MSTEP_S_COMPLETE, m_lblStep[2][0]);
	DDX_Control(pDX, IDC_MSTEP_G_COMPLETE, m_lblStep[2][1]);
			
	//[DEF_MAX_PRODUCT_PLAN][DEF_MAX_TABFEEDER]
	DDX_Control(pDX, IDC_KIND_S_CURRENT, m_lblKind[0][0]);
	DDX_Control(pDX, IDC_KIND_G_CURRENT, m_lblKind[0][1]);
	DDX_Control(pDX, IDC_KIND_S_NEXT, m_lblKind[1][0]);
	DDX_Control(pDX, IDC_KIND_G_NEXT, m_lblKind[1][1]);
	DDX_Control(pDX, IDC_KIND_S_COMPLETE, m_lblKind[2][0]);
	DDX_Control(pDX, IDC_KIND_G_COMPLETE, m_lblKind[2][1]);

	//[DEF_MAX_PRODUCT_PLAN][DEF_MAX_TABFEEDER][DEF_MAX_MCODE];
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT1, m_lblMCode[0][0][0]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT2, m_lblMCode[0][0][1]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT3, m_lblMCode[0][0][2]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT4, m_lblMCode[0][0][3]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT5, m_lblMCode[0][0][4]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT6, m_lblMCode[0][0][5]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT7, m_lblMCode[0][0][6]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT8, m_lblMCode[0][0][7]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT9, m_lblMCode[0][0][8]);
	DDX_Control(pDX, IDC_MATERIAL_S_CURRENT10, m_lblMCode[0][0][9]);

	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT1, m_lblMCode[0][1][0]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT2, m_lblMCode[0][1][1]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT3, m_lblMCode[0][1][2]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT4, m_lblMCode[0][1][3]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT5, m_lblMCode[0][1][4]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT6, m_lblMCode[0][1][5]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT7, m_lblMCode[0][1][6]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT8, m_lblMCode[0][1][7]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT9, m_lblMCode[0][1][8]);
	DDX_Control(pDX, IDC_MATERIAL_G_CURRENT10, m_lblMCode[0][1][9]);
	
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT1, m_lblMCode[1][0][0]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT2, m_lblMCode[1][0][1]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT3, m_lblMCode[1][0][2]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT4, m_lblMCode[1][0][3]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT5, m_lblMCode[1][0][4]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT6, m_lblMCode[1][0][5]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT7, m_lblMCode[1][0][6]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT8, m_lblMCode[1][0][7]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT9, m_lblMCode[1][0][8]);
	DDX_Control(pDX, IDC_MATERIAL_S_NEXT10, m_lblMCode[1][0][9]);

	DDX_Control(pDX, IDC_MATERIAL_G_NEXT1, m_lblMCode[1][1][0]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT2, m_lblMCode[1][1][1]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT3, m_lblMCode[1][1][2]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT4, m_lblMCode[1][1][3]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT5, m_lblMCode[1][1][4]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT6, m_lblMCode[1][1][5]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT7, m_lblMCode[1][1][6]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT8, m_lblMCode[1][1][7]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT9, m_lblMCode[1][1][8]);
	DDX_Control(pDX, IDC_MATERIAL_G_NEXT10, m_lblMCode[1][1][9]);

	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE1, m_lblMCode[2][0][0]);
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE2, m_lblMCode[2][0][1]);
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE3, m_lblMCode[2][0][2]);
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE4, m_lblMCode[2][0][3]);
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE5, m_lblMCode[2][0][4]);
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE6, m_lblMCode[2][0][5]);
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE7, m_lblMCode[2][0][6]);  
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE8, m_lblMCode[2][0][7]);
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE9, m_lblMCode[2][0][8]);
	DDX_Control(pDX, IDC_MATERIAL_S_COMPLETE10, m_lblMCode[2][0][9]);

	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE1, m_lblMCode[2][1][0]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE2, m_lblMCode[2][1][1]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE3, m_lblMCode[2][1][2]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE4, m_lblMCode[2][1][3]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE5, m_lblMCode[2][1][4]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE6, m_lblMCode[2][1][5]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE7, m_lblMCode[2][1][6]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE8, m_lblMCode[2][1][7]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE9, m_lblMCode[2][1][8]);
	DDX_Control(pDX, IDC_MATERIAL_G_COMPLETE10, m_lblMCode[2][1][9]);
}


BEGIN_MESSAGE_MAP(CPageMCodeInfo2ND, CPropertyPage)
	//{{AFX_MSG_MAP(CPageMCodeInfo2ND)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageMCodeInfo2ND message handlers

BOOL CPageMCodeInfo2ND::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
#ifdef DEF_SOURCE_SYSTEM
	m_btnRequestCurrentG.EnableWindow(FALSE);
	m_btnRequestNextG.EnableWindow(FALSE);
#else
	m_btnRequestCurrentS.EnableWindow(FALSE);
	m_btnRequestNextS.EnableWindow(FALSE);
#endif
	
	RenewMaterialCode();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPageMCodeInfo2ND::OnKillActive() 
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

BOOL CPageMCodeInfo2ND::OnSetActive() 
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

BEGIN_EVENTSINK_MAP(CPageMCodeInfo2ND, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(CPageMCodeInfo2ND)
	ON_EVENT(CPageMCodeInfo2ND, IDB_REQUEST_S_CURRENT, -600 /* Click */, OnClickRequestSCurrent, VTS_NONE)
	ON_EVENT(CPageMCodeInfo2ND, IDB_REQUEST_G_CURRENT, -600 /* Click */, OnClickRequestGCurrent, VTS_NONE)
	ON_EVENT(CPageMCodeInfo2ND, IDB_REQUEST_S_NEXT, -600 /* Click */, OnClickRequestSNext, VTS_NONE)
	ON_EVENT(CPageMCodeInfo2ND, IDB_REQUEST_G_NEXT, -600 /* Click */, OnClickRequestGNext, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPageMCodeInfo2ND::clearDisplay()
{
	int i = 0, j = 0, k = 0;
	for (i=0; i<DEF_MAX_PRODUCT_PLAN; i++)
	{
		m_lblWorkOrder[i].SetCaption(_T(""));
		m_lblDeviceID[i].SetCaption(_T(""));
		for (j=0; j<DEF_MAX_TABFEEDER; j++)
		{
			m_lblStep[i][j].SetCaption(_T(""));
			m_lblKind[i][j].SetCaption(_T(""));
			for (k=0; k<DEF_MAX_MCODE; k++)
			{
				m_lblMCode[i][j][k].SetCaption(_T(""));
			}
		}
	}
}

void CPageMCodeInfo2ND::updateDisplay()
{	
	MMaterialCodeInfo* pMatCodeInfo[DEF_MAX_PRODUCT_PLAN][DEF_MAX_TABFEEDER] =
	{
		{	&m_plnkLCNet->m_CurMaterialCodeInfo_ZS,	&m_plnkLCNet->m_CurMaterialCodeInfo_ZG	},
		{	&m_plnkLCNet->m_NextMaterialCodeInfo_ZS,	&m_plnkLCNet->m_NextMaterialCodeInfo_ZG	},
		{	&m_plnkLCNet->m_EndMaterialCodeInfo_ZS,	&m_plnkLCNet->m_EndMaterialCodeInfo_ZG	}
	};

	CString strCaption;
	for (int i=0; i<DEF_MAX_PRODUCT_PLAN; i++)
	{
		strCaption.Format(_T("%d"), pMatCodeInfo[i][DEF_TABFEEDER1]->m_nWorkOrder);
		m_lblWorkOrder[i].SetCaption(strCaption);
		strCaption.Format(_T("%s"), pMatCodeInfo[i][DEF_TABFEEDER1]->m_sDeviceID);
		m_lblDeviceID[i].SetCaption(strCaption);

		for (int j=0; j<DEF_MAX_TABFEEDER; j++)
		{
			m_lblStep[i][j].SetCaption(pMatCodeInfo[i][j]->m_sM_STEP);
			strCaption.Format(_T("%d"), pMatCodeInfo[i][j]->m_nM_KIND); 
			m_lblKind[i][j].SetCaption(strCaption);

			int nCnt = pMatCodeInfo[i][j]->m_asM_CODE.GetSize();
			for (int k=0; k<nCnt; k++)
			{
				strCaption.Format(_T("%s"), pMatCodeInfo[i][j]->m_asM_CODE.GetAt(k));
				m_lblMCode[i][j][k].SetCaption(strCaption);
			}
		}
	}
}

void CPageMCodeInfo2ND::RenewMaterialCode()
{
	clearDisplay();
	updateDisplay();
}

void CPageMCodeInfo2ND::OnClickRequestSCurrent() 
{
	// TODO: Add your control notification handler code here
	materialCodeInfoRequest(DEF_CUR_PRODUCT_PLAN, eMatStep_S);
}

void CPageMCodeInfo2ND::OnClickRequestGCurrent() 
{
	// TODO: Add your control notification handler code here
	materialCodeInfoRequest(DEF_CUR_PRODUCT_PLAN, eMatStep_G);
}

void CPageMCodeInfo2ND::OnClickRequestSNext()
{
	// TODO: Add your control notification handler code here
	materialCodeInfoRequest(DEF_NEXT_PRODUCT_PLAN, eMatStep_S);
}

void CPageMCodeInfo2ND::OnClickRequestGNext() 
{
	// TODO: Add your control notification handler code here
	materialCodeInfoRequest(DEF_NEXT_PRODUCT_PLAN, eMatStep_G);
}

void CPageMCodeInfo2ND::materialCodeInfoRequest(int iPlanType, EMaterialStep eStep)
{
	int iResult = m_plnkAutoManager->MaterialCodeInfoRequest(iPlanType, eStep);
	if (iResult == ERR_NO_DEVICEID_MCODE_REQUEST)
	{
		MyMessageBox(_T("DeviceID of production information is empty. Can not request material information.\r\n생산 정보의 DeviceID가 비어 있습니다. 자재정보를 요청할수 없습니다."), _T("Confirm"));
	}
}
