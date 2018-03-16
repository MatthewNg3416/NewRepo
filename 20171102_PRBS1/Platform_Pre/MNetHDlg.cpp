// MNetHDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MNetHDlg.h"
#include "MNetHData.h"
#include "MNetH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MNetHDlg dialog


MNetHDlg::MNetHDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MNetHDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(MNetHDlg)
	//}}AFX_DATA_INIT

	m_pMNetH = NULL;
	m_bEditSignalMode = true;
}


void MNetHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MNetHDlg)
	DDX_Control(pDX, IDC_PREV_LOCAL, m_lblPrevLocal);
	DDX_Control(pDX, IDC_OWN_LOCAL, m_lblOwnLocal);
	DDX_Control(pDX, IDC_NEXT_LOCAL, m_lblNextLocal);
	DDX_Control(pDX, IDC_LBL_WORD_1, m_lblWord_1);
	DDX_Control(pDX, IDC_LBL_WORD_2, m_lblWord_2);
	DDX_Control(pDX, IDC_LBL_WORD_3, m_lblWord_3);
	DDX_Control(pDX, IDC_LBL_WORD_4, m_lblWord_4);
	DDX_Control(pDX, IDC_LBL_WORD_5, m_lblWord_5);
	DDX_Control(pDX, IDC_LBL_WORD_6, m_lblWord_6);
	DDX_Control(pDX, IDC_LBL_WORD_7, m_lblWord_7);
	DDX_Control(pDX, IDC_LBL_WORD_8, m_lblWord_8);
	DDX_Control(pDX, IDC_LBL_WORD_9, m_lblWord_9);
	DDX_Control(pDX, IDC_LBL_WORD_10, m_lblWord_10);
	DDX_Control(pDX, IDC_LBL_WORD_11, m_lblWord_11);
	DDX_Control(pDX, IDC_LBL_WORD_12, m_lblWord_12);
	DDX_Control(pDX, IDC_LBL_WORD_13, m_lblWord_13);
	DDX_Control(pDX, IDC_LBL_WORD_14, m_lblWord_14);
	DDX_Control(pDX, IDC_LBL_WORD_15, m_lblWord_15);
	DDX_Control(pDX, IDC_LBL_WORD_16, m_lblWord_16);
	DDX_Control(pDX, IDC_LBL_WORD_17, m_lblWord_17);
	DDX_Control(pDX, IDC_LBL_WORD_18, m_lblWord_18);
	DDX_Control(pDX, IDC_LBL_WORD_19, m_lblWord_19);
	DDX_Control(pDX, IDC_LBL_WORD_20, m_lblWord_20);
	DDX_Control(pDX, IDC_LBL_WORD_21, m_lblWord_21);
	DDX_Control(pDX, IDC_LBL_WORD_22, m_lblWord_22);
	DDX_Control(pDX, IDC_LBL_WORD_23, m_lblWord_23);
	DDX_Control(pDX, IDC_LBL_WORD_24, m_lblWord_24);
	DDX_Control(pDX, IDC_LBL_WORD_25, m_lblWord_25);
	DDX_Control(pDX, IDC_LBL_WORD_26, m_lblWord_26);
	DDX_Control(pDX, IDC_LBL_WORD_27, m_lblWord_27);
	DDX_Control(pDX, IDC_LBL_WORD_28, m_lblWord_28);
	DDX_Control(pDX, IDC_LBL_WORD_29, m_lblWord_29);
	DDX_Control(pDX, IDC_LBL_WORD_30, m_lblWord_30);
	DDX_Control(pDX, IDC_LBL_WORD_31, m_lblWord_31);
	DDX_Control(pDX, IDC_LBL_WORD_32, m_lblWord_32);
	DDX_Control(pDX, IDC_EDT_WORD_1, m_edtWord_1);
	DDX_Control(pDX, IDC_EDT_WORD_2, m_edtWord_2);
	DDX_Control(pDX, IDC_EDT_WORD_3, m_edtWord_3);
	DDX_Control(pDX, IDC_EDT_WORD_4, m_edtWord_4);
	DDX_Control(pDX, IDC_EDT_WORD_5, m_edtWord_5);
	DDX_Control(pDX, IDC_EDT_WORD_6, m_edtWord_6);
	DDX_Control(pDX, IDC_EDT_WORD_7, m_edtWord_7);
	DDX_Control(pDX, IDC_EDT_WORD_8, m_edtWord_8);
	DDX_Control(pDX, IDC_EDT_WORD_9, m_edtWord_9);
	DDX_Control(pDX, IDC_EDT_WORD_10, m_edtWord_10);
	DDX_Control(pDX, IDC_EDT_WORD_11, m_edtWord_11);
	DDX_Control(pDX, IDC_EDT_WORD_12, m_edtWord_12);
	DDX_Control(pDX, IDC_EDT_WORD_13, m_edtWord_13);
	DDX_Control(pDX, IDC_EDT_WORD_14, m_edtWord_14);
	DDX_Control(pDX, IDC_EDT_WORD_15, m_edtWord_15);
	DDX_Control(pDX, IDC_EDT_WORD_16, m_edtWord_16);
	DDX_Control(pDX, IDC_EDT_WORD_17, m_edtWord_17);
	DDX_Control(pDX, IDC_EDT_WORD_18, m_edtWord_18);
	DDX_Control(pDX, IDC_EDT_WORD_19, m_edtWord_19);
	DDX_Control(pDX, IDC_EDT_WORD_20, m_edtWord_20);
	DDX_Control(pDX, IDC_EDT_WORD_21, m_edtWord_21);
	DDX_Control(pDX, IDC_EDT_WORD_22, m_edtWord_22);
	DDX_Control(pDX, IDC_EDT_WORD_23, m_edtWord_23);
	DDX_Control(pDX, IDC_EDT_WORD_24, m_edtWord_24);
	DDX_Control(pDX, IDC_EDT_WORD_25, m_edtWord_25);
	DDX_Control(pDX, IDC_EDT_WORD_26, m_edtWord_26);
	DDX_Control(pDX, IDC_EDT_WORD_27, m_edtWord_27);
	DDX_Control(pDX, IDC_EDT_WORD_28, m_edtWord_28);
	DDX_Control(pDX, IDC_EDT_WORD_29, m_edtWord_29);
	DDX_Control(pDX, IDC_EDT_WORD_30, m_edtWord_30);
	DDX_Control(pDX, IDC_EDT_WORD_31, m_edtWord_31);
	DDX_Control(pDX, IDC_EDT_WORD_32, m_edtWord_32);
	DDX_Control(pDX, IDC_TAB_AREA, m_tabArea);
	DDX_Control(pDX, IDC_TAB_UNIT, m_tabUnit);
	DDX_Control(pDX, IDC_TAB_LOCAL, m_tabLocal);
	DDX_Control(pDX, IDC_TAB_DATA, m_tabData);
	DDX_Control(pDX, IDC_BTN_BIT_1, m_btnBit_1);
	DDX_Control(pDX, IDC_BTN_BIT_2, m_btnBit_2);
	DDX_Control(pDX, IDC_BTN_BIT_3, m_btnBit_3);
	DDX_Control(pDX, IDC_BTN_BIT_4, m_btnBit_4);
	DDX_Control(pDX, IDC_BTN_BIT_5, m_btnBit_5);
	DDX_Control(pDX, IDC_BTN_BIT_6, m_btnBit_6);
	DDX_Control(pDX, IDC_BTN_BIT_7, m_btnBit_7);
	DDX_Control(pDX, IDC_BTN_BIT_8, m_btnBit_8);
	DDX_Control(pDX, IDC_BTN_BIT_9, m_btnBit_9);
	DDX_Control(pDX, IDC_BTN_BIT_10, m_btnBit_10);
	DDX_Control(pDX, IDC_BTN_BIT_11, m_btnBit_11);
	DDX_Control(pDX, IDC_BTN_BIT_12, m_btnBit_12);
	DDX_Control(pDX, IDC_BTN_BIT_13, m_btnBit_13);
	DDX_Control(pDX, IDC_BTN_BIT_14, m_btnBit_14);
	DDX_Control(pDX, IDC_BTN_BIT_15, m_btnBit_15);
	DDX_Control(pDX, IDC_BTN_BIT_16, m_btnBit_16);
	DDX_Control(pDX, IDC_BTN_BIT_17, m_btnBit_17);
	DDX_Control(pDX, IDC_BTN_BIT_18, m_btnBit_18);
	DDX_Control(pDX, IDC_BTN_BIT_19, m_btnBit_19);
	DDX_Control(pDX, IDC_BTN_BIT_20, m_btnBit_20);
	DDX_Control(pDX, IDC_BTN_BIT_21, m_btnBit_21);
	DDX_Control(pDX, IDC_BTN_BIT_22, m_btnBit_22);
	DDX_Control(pDX, IDC_BTN_BIT_23, m_btnBit_23);
	DDX_Control(pDX, IDC_BTN_BIT_24, m_btnBit_24);
	DDX_Control(pDX, IDC_BTN_BIT_25, m_btnBit_25);
	DDX_Control(pDX, IDC_BTN_BIT_26, m_btnBit_26);
	DDX_Control(pDX, IDC_BTN_BIT_27, m_btnBit_27);
	DDX_Control(pDX, IDC_BTN_BIT_28, m_btnBit_28);
	DDX_Control(pDX, IDC_BTN_BIT_29, m_btnBit_29);
	DDX_Control(pDX, IDC_BTN_BIT_30, m_btnBit_30);
	DDX_Control(pDX, IDC_BTN_BIT_31, m_btnBit_31);
	DDX_Control(pDX, IDC_BTN_BIT_32, m_btnBit_32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MNetHDlg, CDialog)
	//{{AFX_MSG_MAP(MNetHDlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTabPLC)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_UNIT, OnSelchangeTabUnit)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DATA, OnSelchangeTabData)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOCAL, OnSelchangeTabLocal)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_AREA, OnSelchangeTabArea)
	ON_BN_CLICKED(IDC_BTN_BIT_1, OnBtnBit1)
	ON_BN_CLICKED(IDC_BTN_BIT_2, OnBtnBit2)
	ON_BN_CLICKED(IDC_BTN_BIT_3, OnBtnBit3)
	ON_BN_CLICKED(IDC_BTN_BIT_4, OnBtnBit4)
	ON_BN_CLICKED(IDC_BTN_BIT_5, OnBtnBit5)
	ON_BN_CLICKED(IDC_BTN_BIT_6, OnBtnBit6)
	ON_BN_CLICKED(IDC_BTN_BIT_7, OnBtnBit7)
	ON_BN_CLICKED(IDC_BTN_BIT_8, OnBtnBit8)
	ON_BN_CLICKED(IDC_BTN_BIT_9, OnBtnBit9)
	ON_BN_CLICKED(IDC_BTN_BIT_10, OnBtnBit10)
	ON_BN_CLICKED(IDC_BTN_BIT_11, OnBtnBit11)
	ON_BN_CLICKED(IDC_BTN_BIT_12, OnBtnBit12)
	ON_BN_CLICKED(IDC_BTN_BIT_13, OnBtnBit13)
	ON_BN_CLICKED(IDC_BTN_BIT_14, OnBtnBit14)
	ON_BN_CLICKED(IDC_BTN_BIT_15, OnBtnBit15)
	ON_BN_CLICKED(IDC_BTN_BIT_16, OnBtnBit16)
	ON_BN_CLICKED(IDC_BTN_BIT_17, OnBtnBit17)
	ON_BN_CLICKED(IDC_BTN_BIT_18, OnBtnBit18)
	ON_BN_CLICKED(IDC_BTN_BIT_19, OnBtnBit19)
	ON_BN_CLICKED(IDC_BTN_BIT_20, OnBtnBit20)
	ON_BN_CLICKED(IDC_BTN_BIT_21, OnBtnBit21)
	ON_BN_CLICKED(IDC_BTN_BIT_22, OnBtnBit22)
	ON_BN_CLICKED(IDC_BTN_BIT_23, OnBtnBit23)
	ON_BN_CLICKED(IDC_BTN_BIT_24, OnBtnBit24)
	ON_BN_CLICKED(IDC_BTN_BIT_25, OnBtnBit25)
	ON_BN_CLICKED(IDC_BTN_BIT_26, OnBtnBit26)
	ON_BN_CLICKED(IDC_BTN_BIT_27, OnBtnBit27)
	ON_BN_CLICKED(IDC_BTN_BIT_28, OnBtnBit28)
	ON_BN_CLICKED(IDC_BTN_BIT_29, OnBtnBit29)
	ON_BN_CLICKED(IDC_BTN_BIT_30, OnBtnBit30)
	ON_BN_CLICKED(IDC_BTN_BIT_31, OnBtnBit31)
	ON_BN_CLICKED(IDC_BTN_BIT_32, OnBtnBit32)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MNetHDlg message handlers


/*========================================================================================
	FUNCTION : MNetH::OnDestroy()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/12, KJH
	LAST	 : 2004/08/14, KJH
========================================================================================*/
void MNetHDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
		
}

/*========================================================================================
	FUNCTION : MNetH::OnTimer()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/12, KJH
	LAST	 : 2004/08/14, KJH
========================================================================================*/
void MNetHDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_pMNetH->MonitoringPLCArea();
	
	CDialog::OnTimer(nIDEvent);
}

/*========================================================================================
	FUNCTION : MNetH::OnInitDialog()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/12, KJH
	LAST	 : 2004/08/14, KJH
========================================================================================*/
BOOL MNetHDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitProcess();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*========================================================================================
	FUNCTION : MNetH::InitProcess()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/12, KJH
	LAST	 : 2004/08/14, KJH
========================================================================================*/
void MNetHDlg::InitProcess()
{
	InitCtl();
	InitTab();
	InitDataTab();

	ViewBitCtlVisible(true);
	ViewWordCtlVisible(false);

	m_tabLocal.SetCurSel(m_pMNetH->m_nCurLocal-1);
	m_tabUnit.SetCurSel(m_pMNetH->m_nCurUnit-1);

	if(1==m_pMNetH->m_nCurLocal){
		m_lblPrevLocal.SetWindowText(_T(""));
		m_lblOwnLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(0));
		m_lblNextLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(1));
	}
	else if(1<m_pMNetH->m_nCurLocal){
		m_lblPrevLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(m_pMNetH->m_nCurLocal-2));
		m_lblOwnLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(m_pMNetH->m_nCurLocal-1));
		m_lblNextLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(m_pMNetH->m_nCurLocal));
	}
	else{
		m_lblPrevLocal.SetWindowText(_T(""));
		m_lblOwnLocal.SetWindowText(_T(""));
		m_lblNextLocal.SetWindowText(_T(""));
	}
}

/*========================================================================================
	FUNCTION : MNetH::InitDataTab()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/01, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::InitDataTab(bool bBitArea) 
{
	TC_ITEM		item;

	item.mask=TCIF_TEXT;
	m_tabData.DeleteAllItems();
	if(bBitArea){
		item.pszText = _T("to Upper Equipment");				m_tabData.InsertItem(0, &item);
		item.pszText = _T("to Lower Equipment");				m_tabData.InsertItem(1, &item);
		item.pszText = _T("Contact Point State(to Upper)");		m_tabData.InsertItem(2, &item);
		item.pszText = _T("Contact Point State(to Lower)");		m_tabData.InsertItem(3, &item);
		item.pszText = _T("NSMC Info");							m_tabData.InsertItem(4, &item);

		InitBitAreaTitle();
	}
	else{
		item.pszText = _T("Equipment State(Own)");				m_tabData.InsertItem(0, &item);
		item.pszText = _T("Equipment State(Related)");			m_tabData.InsertItem(1, &item);
		item.pszText = _T("Job Summary");						m_tabData.InsertItem(2, &item);
		item.pszText = _T("to Upper Equipment");				m_tabData.InsertItem(3, &item);
		item.pszText = _T("to Lower Equipment");				m_tabData.InsertItem(4, &item);
		item.pszText = _T("NSMC Info");							m_tabData.InsertItem(5, &item);
		item.pszText = _T("Material Info");						m_tabData.InsertItem(6, &item); //SJ_YYK 150821 Add..
		item.pszText = _T("FDC Info");							m_tabData.InsertItem(7, &item); //SJ_YYK 151024 Add..
		InitWordAreaTitle();
	}
}

/*========================================================================================
	FUNCTION : MNetH::InitTab()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/01, KJH
	LAST	 : 2004/09/03, KJH
========================================================================================*/
void MNetHDlg::InitTab() 
{
	WCHAR		szTitle[255];	
	TC_ITEM		item;


	item.mask=TCIF_TEXT;

	// Area.
	item.pszText = _T("Bit Area");	m_tabArea.InsertItem(0, &item);
	item.pszText = _T("Word Area"); m_tabArea.InsertItem(1, &item);

	// Unit.

	int nUnitQty = m_pMNetH->m_asUnitName.GetSize();
	for(int i=0; i<nUnitQty; i++)
	{	
		_tcscpy(szTitle, m_pMNetH->m_asUnitName.GetAt(i));	
		item.pszText = szTitle; m_tabUnit.InsertItem(i, &item); memset(szTitle, NULL, sizeof(szTitle));
		//_tcscpy(szTitle, m_pMNetH->m_asUnitName.GetAt(1));	
		//item.pszText = szTitle; m_tabUnit.InsertItem(1, &item); memset(szTitle, NULL, sizeof(szTitle));
		//_tcscpy(szTitle, m_pMNetH->m_asUnitName.GetAt(2));	
		//item.pszText = szTitle;	m_tabUnit.InsertItem(2, &item); memset(szTitle, NULL, sizeof(szTitle));
	}

	// Local.
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(0));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(0, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(1));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(1, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(2));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(2, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(3));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(3, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(4));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(4, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(5));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(5, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(6));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(6, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(7));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(7, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(8));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(8, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(9));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(9, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(10));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(10, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(11));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(11, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(12));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(12, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(13));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(13, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(14));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(14, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(15));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(15, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(16));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(16, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(17));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(17, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(18));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(18, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(19));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(19, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(20));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(20, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(21));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(21, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(22));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(22, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(23));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(23, &item);	memset(szTitle, NULL, sizeof(szTitle));
	_tcscpy(szTitle, m_pMNetH->m_asLocalName.GetAt(24));	
	item.pszText = szTitle;	m_tabLocal.InsertItem(24, &item);	memset(szTitle, NULL, sizeof(szTitle));
}

/*========================================================================================
	FUNCTION : MNetH::InitCtl()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/01, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::InitCtl() 
{
	// Signal Bit.
	m_btnBit_1.SetIcon(IDI_LED_OFF);	m_btnBit_1.DrawBorder(false);		m_btnBit[0]=&m_btnBit_1;
	m_btnBit_2.SetIcon(IDI_LED_OFF);	m_btnBit_2.DrawBorder(false);		m_btnBit[1]=&m_btnBit_2;
	m_btnBit_3.SetIcon(IDI_LED_OFF);	m_btnBit_3.DrawBorder(false);		m_btnBit[2]=&m_btnBit_3;
	m_btnBit_4.SetIcon(IDI_LED_OFF);	m_btnBit_4.DrawBorder(false);		m_btnBit[3]=&m_btnBit_4;
	m_btnBit_5.SetIcon(IDI_LED_OFF);	m_btnBit_5.DrawBorder(false);		m_btnBit[4]=&m_btnBit_5;
	m_btnBit_6.SetIcon(IDI_LED_OFF);	m_btnBit_6.DrawBorder(false);		m_btnBit[5]=&m_btnBit_6;
	m_btnBit_7.SetIcon(IDI_LED_OFF);	m_btnBit_7.DrawBorder(false);		m_btnBit[6]=&m_btnBit_7;
	m_btnBit_8.SetIcon(IDI_LED_OFF);	m_btnBit_8.DrawBorder(false);		m_btnBit[7]=&m_btnBit_8;
	m_btnBit_9.SetIcon(IDI_LED_OFF);	m_btnBit_9.DrawBorder(false);		m_btnBit[8]=&m_btnBit_9;
	m_btnBit_10.SetIcon(IDI_LED_OFF);	m_btnBit_10.DrawBorder(false);		m_btnBit[9]=&m_btnBit_10;
	m_btnBit_11.SetIcon(IDI_LED_OFF);	m_btnBit_11.DrawBorder(false);		m_btnBit[10]=&m_btnBit_11;
	m_btnBit_12.SetIcon(IDI_LED_OFF);	m_btnBit_12.DrawBorder(false);		m_btnBit[11]=&m_btnBit_12;
	m_btnBit_13.SetIcon(IDI_LED_OFF);	m_btnBit_13.DrawBorder(false);		m_btnBit[12]=&m_btnBit_13;
	m_btnBit_14.SetIcon(IDI_LED_OFF);	m_btnBit_14.DrawBorder(false);		m_btnBit[13]=&m_btnBit_14;
	m_btnBit_15.SetIcon(IDI_LED_OFF);	m_btnBit_15.DrawBorder(false);		m_btnBit[14]=&m_btnBit_15;
	m_btnBit_16.SetIcon(IDI_LED_OFF);	m_btnBit_16.DrawBorder(false);		m_btnBit[15]=&m_btnBit_16;
	m_btnBit_17.SetIcon(IDI_LED_OFF);	m_btnBit_17.DrawBorder(false);		m_btnBit[16]=&m_btnBit_17;
	m_btnBit_18.SetIcon(IDI_LED_OFF);	m_btnBit_18.DrawBorder(false);		m_btnBit[17]=&m_btnBit_18;
	m_btnBit_19.SetIcon(IDI_LED_OFF);	m_btnBit_19.DrawBorder(false);		m_btnBit[18]=&m_btnBit_19;
	m_btnBit_20.SetIcon(IDI_LED_OFF);	m_btnBit_20.DrawBorder(false);		m_btnBit[19]=&m_btnBit_20;
	m_btnBit_21.SetIcon(IDI_LED_OFF);	m_btnBit_21.DrawBorder(false);		m_btnBit[20]=&m_btnBit_21;
	m_btnBit_22.SetIcon(IDI_LED_OFF);	m_btnBit_22.DrawBorder(false);		m_btnBit[21]=&m_btnBit_22;
	m_btnBit_23.SetIcon(IDI_LED_OFF);	m_btnBit_23.DrawBorder(false);		m_btnBit[22]=&m_btnBit_23;
	m_btnBit_24.SetIcon(IDI_LED_OFF);	m_btnBit_24.DrawBorder(false);		m_btnBit[23]=&m_btnBit_24;
	m_btnBit_25.SetIcon(IDI_LED_OFF);	m_btnBit_25.DrawBorder(false);		m_btnBit[24]=&m_btnBit_25;
	m_btnBit_26.SetIcon(IDI_LED_OFF);	m_btnBit_26.DrawBorder(false);		m_btnBit[25]=&m_btnBit_26;
	m_btnBit_27.SetIcon(IDI_LED_OFF);	m_btnBit_27.DrawBorder(false);		m_btnBit[26]=&m_btnBit_27;
	m_btnBit_28.SetIcon(IDI_LED_OFF);	m_btnBit_28.DrawBorder(false);		m_btnBit[27]=&m_btnBit_28;
	m_btnBit_29.SetIcon(IDI_LED_OFF);	m_btnBit_29.DrawBorder(false);		m_btnBit[28]=&m_btnBit_29;
	m_btnBit_30.SetIcon(IDI_LED_OFF);	m_btnBit_30.DrawBorder(false);		m_btnBit[29]=&m_btnBit_30;
	m_btnBit_31.SetIcon(IDI_LED_OFF);	m_btnBit_31.DrawBorder(false);		m_btnBit[30]=&m_btnBit_31;
	m_btnBit_32.SetIcon(IDI_LED_OFF);	m_btnBit_32.DrawBorder(false);		m_btnBit[31]=&m_btnBit_32;

	// Word Label.
	m_lblWord[0]=&m_lblWord_1;
	m_lblWord[1]=&m_lblWord_2;
	m_lblWord[2]=&m_lblWord_3;
	m_lblWord[3]=&m_lblWord_4;
	m_lblWord[4]=&m_lblWord_5;
	m_lblWord[5]=&m_lblWord_6;
	m_lblWord[6]=&m_lblWord_7;
	m_lblWord[7]=&m_lblWord_8;
	m_lblWord[8]=&m_lblWord_9;
	m_lblWord[9]=&m_lblWord_10;
	m_lblWord[10]=&m_lblWord_11;
	m_lblWord[11]=&m_lblWord_12;
	m_lblWord[12]=&m_lblWord_13;
	m_lblWord[13]=&m_lblWord_14;
	m_lblWord[14]=&m_lblWord_15;
	m_lblWord[15]=&m_lblWord_16;
	m_lblWord[16]=&m_lblWord_17;
	m_lblWord[17]=&m_lblWord_18;
	m_lblWord[18]=&m_lblWord_19;
	m_lblWord[19]=&m_lblWord_20;
	m_lblWord[20]=&m_lblWord_21;
	m_lblWord[21]=&m_lblWord_22;
	m_lblWord[22]=&m_lblWord_23;
	m_lblWord[23]=&m_lblWord_24;
	m_lblWord[24]=&m_lblWord_25;
	m_lblWord[25]=&m_lblWord_26;
	m_lblWord[26]=&m_lblWord_27;
	m_lblWord[27]=&m_lblWord_28;
	m_lblWord[28]=&m_lblWord_29;
	m_lblWord[29]=&m_lblWord_30;
	m_lblWord[30]=&m_lblWord_31;
	m_lblWord[31]=&m_lblWord_32;

	// Word Edit.
	m_edtWord[0]=&m_edtWord_1;
	m_edtWord[1]=&m_edtWord_2;
	m_edtWord[2]=&m_edtWord_3;
	m_edtWord[3]=&m_edtWord_4;
	m_edtWord[4]=&m_edtWord_5;
	m_edtWord[5]=&m_edtWord_6;
	m_edtWord[6]=&m_edtWord_7;
	m_edtWord[7]=&m_edtWord_8;
	m_edtWord[8]=&m_edtWord_9;
	m_edtWord[9]=&m_edtWord_10;
	m_edtWord[10]=&m_edtWord_11;
	m_edtWord[11]=&m_edtWord_12;
	m_edtWord[12]=&m_edtWord_13;
	m_edtWord[13]=&m_edtWord_14;
	m_edtWord[14]=&m_edtWord_15;
	m_edtWord[15]=&m_edtWord_16;
	m_edtWord[16]=&m_edtWord_17;
	m_edtWord[17]=&m_edtWord_18;
	m_edtWord[18]=&m_edtWord_19;
	m_edtWord[19]=&m_edtWord_20;
	m_edtWord[20]=&m_edtWord_21;
	m_edtWord[21]=&m_edtWord_22;
	m_edtWord[22]=&m_edtWord_23;
	m_edtWord[23]=&m_edtWord_24;
	m_edtWord[24]=&m_edtWord_25;
	m_edtWord[25]=&m_edtWord_26;
	m_edtWord[26]=&m_edtWord_27;
	m_edtWord[27]=&m_edtWord_28;
	m_edtWord[28]=&m_edtWord_29;
	m_edtWord[29]=&m_edtWord_30;
	m_edtWord[30]=&m_edtWord_31;
	m_edtWord[31]=&m_edtWord_32;
}

/*========================================================================================
	FUNCTION : MNetH::InitBitAreaTitle()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/01, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::InitBitAreaTitle(int nTitleIndex) 
{
	if(0==nTitleIndex){
		m_btnBit_1.SetWindowText(_T("Reserved0"));		
		m_btnBit_2.SetWindowText(_T("Machine Pause"));		
		m_btnBit_3.SetWindowText(_T("Machine Down"));	
		m_btnBit_4.SetWindowText(_T("Machine Alarm"));	
		m_btnBit_5.SetWindowText(_T("Receive Able"));	
		m_btnBit_6.SetWindowText(_T("Receive Start"));	
		m_btnBit_7.SetWindowText(_T("Receive Complete"));	
		m_btnBit_8.SetWindowText(_T("Exchange Flag"));	
		m_btnBit_9.SetWindowText(_T("Return Send Start"));	
		m_btnBit_10.SetWindowText(_T("Return Send Complete"));	
		m_btnBit_11.SetWindowText(_T("Action1 Complete"));	
		m_btnBit_12.SetWindowText(_T("Action2 Complete"));	
		m_btnBit_13.SetWindowText(_T("Reserved1"));	
		m_btnBit_14.SetWindowText(_T("Reserved2"));	
		m_btnBit_15.SetWindowText(_T("Receive Refuse"));	
		m_btnBit_16.SetWindowText(_T("Glass Read Complete"));	
		m_btnBit_17.SetWindowText(_T("Loading Stop"));	
		m_btnBit_18.SetWindowText(_T("Transfer Stop"));	
		m_btnBit_19.SetWindowText(_T("Immediatedly Pause Request"));	
		m_btnBit_20.SetWindowText(_T("Immediatedly Stop Request"));	
		m_btnBit_21.SetWindowText(_T("ReceiveAbleRemainedStep(2^0)"));	
		m_btnBit_22.SetWindowText(_T("ReceiveAbleRemainedStep(2^1)"));	
		m_btnBit_23.SetWindowText(_T("ReceiveAbleRemainedStep(2^2)"));	
		m_btnBit_24.SetWindowText(_T("ReceiveAbleRemainedStep(2^3)"));	
		m_btnBit_25.SetWindowText(_T("HandShakeCancelRequest"));	
		m_btnBit_26.SetWindowText(_T("HandShakeAbortRequest"));	
		m_btnBit_27.SetWindowText(_T("HandShakeResumeRequest"));	
		m_btnBit_28.SetWindowText(_T("HandShakeRecoveryAckRequest"));	
		m_btnBit_29.SetWindowText(_T("HandShakeRecoveryNakRequest"));	
		m_btnBit_30.SetWindowText(_T("Reserved3"));	
		m_btnBit_31.SetWindowText(_T("Reserved4"));	
		m_btnBit_32.SetWindowText(_T("Reserved5"));	
	}
	else if(1==nTitleIndex){
		m_btnBit_1.SetWindowText(_T("Reserved0"));		
		m_btnBit_2.SetWindowText(_T("Machine Pause"));		
		m_btnBit_3.SetWindowText(_T("Machine Down"));	
		m_btnBit_4.SetWindowText(_T("Machine Alarm"));	
		m_btnBit_5.SetWindowText(_T("Send Able"));	
		m_btnBit_6.SetWindowText(_T("Send Start"));	
		m_btnBit_7.SetWindowText(_T("Send Complete"));	
		m_btnBit_8.SetWindowText(_T("Exchange Flag"));	
		m_btnBit_9.SetWindowText(_T("Return Receive Start"));	
		m_btnBit_10.SetWindowText(_T("Return Receive Complete"));	
		m_btnBit_11.SetWindowText(_T("Action1 Request"));	
		m_btnBit_12.SetWindowText(_T("Action2 Request"));	
		m_btnBit_13.SetWindowText(_T("Reserved1"));	
		m_btnBit_14.SetWindowText(_T("Reserved2"));	
		m_btnBit_15.SetWindowText(_T("Work Start"));	
		m_btnBit_16.SetWindowText(_T("Work Cancel"));	
		m_btnBit_17.SetWindowText(_T("Reserved3"));	
		m_btnBit_18.SetWindowText(_T("Reserved4"));	
		m_btnBit_19.SetWindowText(_T("Immediatedly Pause Request"));	
		m_btnBit_20.SetWindowText(_T("Immediatedly Stop Request"));	
		m_btnBit_21.SetWindowText(_T("SendAbleRemainedStep(2^0)"));	
		m_btnBit_22.SetWindowText(_T("SendAbleRemainedStep(2^1)"));	
		m_btnBit_23.SetWindowText(_T("SendAbleRemainedStep(2^2)"));	
		m_btnBit_24.SetWindowText(_T("SendAbleRemainedStep(2^3)"));	
		m_btnBit_25.SetWindowText(_T("HandShakeCancelRequest"));	
		m_btnBit_26.SetWindowText(_T("HandShakeAbortRequest"));	
		m_btnBit_27.SetWindowText(_T("HandShakeResumeRequest"));	
		m_btnBit_28.SetWindowText(_T("HandShakeRecoveryAckRequest"));	
		m_btnBit_29.SetWindowText(_T("HandShakeRecoveryNakRequest"));	
		m_btnBit_30.SetWindowText(_T("Reserved5"));	
		m_btnBit_31.SetWindowText(_T("Reserved6"));	
		m_btnBit_32.SetWindowText(_T("Reserved7"));	
	}
	else if(2==nTitleIndex){
		m_btnBit_1.SetWindowText(_T("Robot Abnormal"));		
		m_btnBit_2.SetWindowText(_T("Type of Arm"));		
		m_btnBit_3.SetWindowText(_T("Type of Stage/Conveyor"));	
		m_btnBit_4.SetWindowText(_T("Arm1 Violate"));	
		m_btnBit_5.SetWindowText(_T("Arm2 Violate"));	
		m_btnBit_6.SetWindowText(_T("Arm1 Fold Complete"));	
		m_btnBit_7.SetWindowText(_T("Arm2 Fold Complete"));	
		m_btnBit_8.SetWindowText(_T("Arm1 GlassCheckSensorOn"));	
		m_btnBit_9.SetWindowText(_T("Arm2 GlassCheckSensorOn"));	
		m_btnBit_10.SetWindowText(_T("Reserved0"));	
		m_btnBit_11.SetWindowText(_T("Reserved1"));	
		m_btnBit_12.SetWindowText(_T("Reserved2"));	
		m_btnBit_13.SetWindowText(_T("Manual Operation"));	
		m_btnBit_14.SetWindowText(_T("Robot Direction"));	
		m_btnBit_15.SetWindowText(_T("Reserved3"));	
		m_btnBit_16.SetWindowText(_T("Reserved4"));	

		m_btnBit_17.SetWindowText(_T("Stage/Conveyor Abnormal"));	
		m_btnBit_18.SetWindowText(_T("Type of Arm"));	
		m_btnBit_19.SetWindowText(_T("Type of Stage/Conveyor"));	
		m_btnBit_20.SetWindowText(_T("Empty"));	
		m_btnBit_21.SetWindowText(_T("Idle"));	
		m_btnBit_22.SetWindowText(_T("Busy"));	
		m_btnBit_23.SetWindowText(_T("Pin(Lift) Up"));	
		m_btnBit_24.SetWindowText(_T("Pin(Lift) Down"));	
		m_btnBit_25.SetWindowText(_T("Stopper Up"));	
		m_btnBit_26.SetWindowText(_T("Stopper Down"));	
		m_btnBit_27.SetWindowText(_T("Door Open"));	
		m_btnBit_28.SetWindowText(_T("Door Close"));	
		m_btnBit_29.SetWindowText(_T("Manual Operation"));	
		m_btnBit_30.SetWindowText(_T("ModelChange Moving"));	
		m_btnBit_31.SetWindowText(_T("Glass Check Sensor On"));	
		m_btnBit_32.SetWindowText(_T("Reserved0"));	
	}
	else if (3==nTitleIndex){
		m_btnBit_1.SetWindowText(_T("eModelChangeReserve"));		
		m_btnBit_2.SetWindowText(_T("eModelChangeStart"));		
		m_btnBit_3.SetWindowText(_T("eModelChangeComplete"));	
		m_btnBit_4.SetWindowText(_T("eModelChangeDisable"));	
		m_btnBit_5.SetWindowText(_T("Model Reserve Complete"));	// Add. 110525 기종변경 예약 Bit 추가
		m_btnBit_6.SetWindowText(_T("Model Reserve Disable"));	
		m_btnBit_7.SetWindowText(_T("Reserved"));	
		m_btnBit_8.SetWindowText(_T("Reserved"));	
		m_btnBit_9.SetWindowText(_T("Reserved"));	
		m_btnBit_10.SetWindowText(_T("Reserved"));	
		m_btnBit_11.SetWindowText(_T("Reserved"));	
		m_btnBit_12.SetWindowText(_T("Reserved"));	
		m_btnBit_13.SetWindowText(_T("Reserved"));	
		m_btnBit_14.SetWindowText(_T("Reserved"));	
		m_btnBit_15.SetWindowText(_T("Reserved"));	
		m_btnBit_16.SetWindowText(_T("Reserved"));	
		m_btnBit_17.SetWindowText(_T("Reserved"));	
		m_btnBit_18.SetWindowText(_T("Reserved"));	
		m_btnBit_19.SetWindowText(_T("Reserved"));	
		m_btnBit_20.SetWindowText(_T("Reserved"));	
		m_btnBit_21.SetWindowText(_T("Reserved"));	
		m_btnBit_22.SetWindowText(_T("Reserved"));	
		m_btnBit_23.SetWindowText(_T("Reserved"));	
		m_btnBit_24.SetWindowText(_T("Reserved"));	
		m_btnBit_25.SetWindowText(_T("Reserved"));	
		m_btnBit_26.SetWindowText(_T("Reserved"));	
		m_btnBit_27.SetWindowText(_T("Reserved"));	
		m_btnBit_28.SetWindowText(_T("Reserved"));	
		m_btnBit_29.SetWindowText(_T("Reserved"));	
		m_btnBit_30.SetWindowText(_T("Reserved"));	
		m_btnBit_31.SetWindowText(_T("Reserved"));	
		m_btnBit_32.SetWindowText(_T("Reserved"));	
	}
}

/*========================================================================================
	FUNCTION : MNetH::InitWordAreaTitle()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/01, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::InitWordAreaTitle(int nTitleIndex) 
{
	//////////////////////////////////////////////////////////////////////////////////////
	// Equipment State(Own/Related)
	int nSelWorkGroup = 0;
	if(0==nTitleIndex){
		m_lblWord_1.SetWindowText(_T(" EqState"));		
		m_lblWord_2.SetWindowText(_T(" ProcState"));		
		m_lblWord_3.SetWindowText(_T(" PPID"));	
		m_lblWord_4.SetWindowText(_T(" GlassSize"));	
		m_lblWord_5.SetWindowText(_T(" GlassThickness"));	
		m_lblWord_6.SetWindowText(_T(" TACT(Set)"));	
		m_lblWord_7.SetWindowText(_T(" TACT(Current)"));	
		m_lblWord_8.SetWindowText(_T(" RCDG"));	
		m_lblWord_9.SetWindowText(_T(" EO-Mode"));	
		m_lblWord_10.SetWindowText(_T(" ERC-Mode"));	
		m_lblWord_11.SetWindowText(_T(" GMC-Mode"));	
		m_lblWord_12.SetWindowText(_T(" Operate-Mode"));	
		m_lblWord_13.SetWindowText(_T(" Reserved"));	
		m_lblWord_14.SetWindowText(_T(""));	
		m_lblWord_15.SetWindowText(_T(""));	
		m_lblWord_16.SetWindowText(_T(""));	
		m_lblWord_17.SetWindowText(_T(""));	
		m_lblWord_18.SetWindowText(_T(""));	
		m_lblWord_19.SetWindowText(_T(""));	
		m_lblWord_20.SetWindowText(_T(""));	
		m_lblWord_21.SetWindowText(_T(""));	
		m_lblWord_22.SetWindowText(_T(""));	
		m_lblWord_23.SetWindowText(_T(""));	
		m_lblWord_24.SetWindowText(_T(""));	
		m_lblWord_25.SetWindowText(_T(""));	
		m_lblWord_26.SetWindowText(_T(""));	
		m_lblWord_27.SetWindowText(_T(""));	
		m_lblWord_28.SetWindowText(_T(""));	
		m_lblWord_29.SetWindowText(_T(""));	
		m_lblWord_30.SetWindowText(_T(""));	
		m_lblWord_31.SetWindowText(_T(""));	
		m_lblWord_32.SetWindowText(_T(""));	
	}
	//////////////////////////////////////////////////////////////////////////////////////
	// Job Summary
	else if(1==nTitleIndex)
	{			
		//20130205 SJ_HJG 
		nSelWorkGroup = m_tabUnit.GetCurSel();	
		if(nSelWorkGroup == 0)
		{			
			m_lblWord_1.SetWindowText(_T(" TabOffset-F00"));		
			m_lblWord_2.SetWindowText(_T(" TabOffset-F01"));		
			m_lblWord_3.SetWindowText(_T(" TabOffset-F02"));
			m_lblWord_4.SetWindowText(_T(" TabOffset-F03"));
			m_lblWord_5.SetWindowText(_T(" TabOffset-F04"));
			m_lblWord_6.SetWindowText(_T(" TabOffset-F05"));
			m_lblWord_7.SetWindowText(_T(" TabOffset-F06"));
			m_lblWord_8.SetWindowText(_T(" TabOffset-F07"));
			m_lblWord_9.SetWindowText(_T(" TabOffset-F08"));
			m_lblWord_10.SetWindowText(_T(" TabOffset-F09"));
			m_lblWord_11.SetWindowText(_T(" TabOffset-F10"));
			m_lblWord_12.SetWindowText(_T(" TabOffset-F11"));
			m_lblWord_13.SetWindowText(_T(" TabOffset-F12"));
			m_lblWord_14.SetWindowText(_T(" TabOffset-F13"));
			m_lblWord_15.SetWindowText(_T(" TabOffset-F14"));
			m_lblWord_16.SetWindowText(_T(" TabOffset-F15"));
			m_lblWord_17.SetWindowText(_T(" TabOffset-F16"));
			m_lblWord_18.SetWindowText(_T(" TabOffset-F17"));
			m_lblWord_19.SetWindowText(_T(" TabOffset-F18"));
			m_lblWord_20.SetWindowText(_T(" TabOffset-F19"));
			m_lblWord_21.SetWindowText(_T(" TabOffset-F20"));
			m_lblWord_22.SetWindowText(_T(" TabOffset-F21"));
			m_lblWord_23.SetWindowText(_T(" TabOffset-F22"));
			m_lblWord_24.SetWindowText(_T(" TabOffset-F23"));
			m_lblWord_25.SetWindowText(_T(""));
			m_lblWord_26.SetWindowText(_T(""));
			m_lblWord_27.SetWindowText(_T(""));
			m_lblWord_28.SetWindowText(_T(""));
			m_lblWord_29.SetWindowText(_T(""));
			m_lblWord_30.SetWindowText(_T(""));
			m_lblWord_31.SetWindowText(_T(""));
			m_lblWord_32.SetWindowText(_T(""));
		}
		else
		{
			m_lblWord_1.SetWindowText(_T(" TabOffset-R00"));		
			m_lblWord_2.SetWindowText(_T(" TabOffset-R01"));		
			m_lblWord_3.SetWindowText(_T(" TabOffset-R02"));
			m_lblWord_4.SetWindowText(_T(" TabOffset-R03"));
			m_lblWord_5.SetWindowText(_T(" TabOffset-R04"));
			m_lblWord_6.SetWindowText(_T(" TabOffset-R05"));
			m_lblWord_7.SetWindowText(_T(" TabOffset-R06"));
			m_lblWord_8.SetWindowText(_T(" TabOffset-R07"));
			m_lblWord_9.SetWindowText(_T(" TabOffset-R08"));
			m_lblWord_10.SetWindowText(_T(" TabOffset-R09"));
			m_lblWord_11.SetWindowText(_T(" TabOffset-R10"));
			m_lblWord_12.SetWindowText(_T(" TabOffset-R11"));
			m_lblWord_13.SetWindowText(_T(" TabOffset-R12"));
			m_lblWord_14.SetWindowText(_T(" TabOffset-R13"));
			m_lblWord_15.SetWindowText(_T(" TabOffset-R14"));
			m_lblWord_16.SetWindowText(_T(" TabOffset-R15"));
			m_lblWord_17.SetWindowText(_T(" TabOffset-R16"));
			m_lblWord_18.SetWindowText(_T(" TabOffset-R17"));
			m_lblWord_19.SetWindowText(_T(" TabOffset-R18"));
			m_lblWord_20.SetWindowText(_T(" TabOffset-R19"));
			m_lblWord_21.SetWindowText(_T(" TabOffset-R20"));
			m_lblWord_22.SetWindowText(_T(" TabOffset-R21"));
			m_lblWord_23.SetWindowText(_T(" TabOffset-R22"));
			m_lblWord_24.SetWindowText(_T(" TabOffset-R23"));
			m_lblWord_25.SetWindowText(_T(""));
			m_lblWord_26.SetWindowText(_T(""));
			m_lblWord_27.SetWindowText(_T(""));
			m_lblWord_28.SetWindowText(_T(""));
			m_lblWord_29.SetWindowText(_T(""));
			m_lblWord_30.SetWindowText(_T(""));
			m_lblWord_31.SetWindowText(_T(""));
			m_lblWord_32.SetWindowText(_T(""));

		}

			//20111205 SJ_HJG
		/*
		m_lblWord_1.SetWindowText(_T(" TGlass-Count"));		
		m_lblWord_2.SetWindowText(_T(" TFT-Count"));		
		m_lblWord_3.SetWindowText(_T(" CF-Count"));	
		m_lblWord_4.SetWindowText(_T(" Dummy-Count"));	
		m_lblWord_5.SetWindowText(_T(" BPG-Count"));	
		m_lblWord_6.SetWindowText(_T(" Process-id #1"));	
		m_lblWord_7.SetWindowText(_T(" Step-id #1"));	
		m_lblWord_8.SetWindowText(_T(" Batch-id #1"));	
		m_lblWord_9.SetWindowText(_T(" Glass-count #1"));	
		m_lblWord_10.SetWindowText(_T(" Process-id #2"));	
		m_lblWord_11.SetWindowText(_T(" Step-id #2"));	
		m_lblWord_12.SetWindowText(_T(" Batch-id #2"));	
		m_lblWord_13.SetWindowText(_T(" Glass-count #2"));	
		m_lblWord_14.SetWindowText(_T(" Process-id #3"));	
		m_lblWord_15.SetWindowText(_T(" Step-id #3"));	
		m_lblWord_16.SetWindowText(_T(" Batch-id #3"));	
		m_lblWord_17.SetWindowText(_T(" Glass-count #3"));	
		m_lblWord_18.SetWindowText(_T(" Reserved"));	
		m_lblWord_19.SetWindowText(_T(""));	
		m_lblWord_20.SetWindowText(_T(""));	
		m_lblWord_21.SetWindowText(_T(""));	
		m_lblWord_22.SetWindowText(_T(""));	
		m_lblWord_23.SetWindowText(_T(""));	
		m_lblWord_24.SetWindowText(_T(""));	
		m_lblWord_25.SetWindowText(_T(""));	
		m_lblWord_26.SetWindowText(_T(""));	
		m_lblWord_27.SetWindowText(_T(""));	
		m_lblWord_28.SetWindowText(_T(""));	
		m_lblWord_29.SetWindowText(_T(""));	
		m_lblWord_30.SetWindowText(_T(""));	
		m_lblWord_31.SetWindowText(_T(""));	
		m_lblWord_32.SetWindowText(_T(""));	
		*/
	}
	//////////////////////////////////////////////////////////////////////////////////////
	// To Upper Equipment
	else if(2==nTitleIndex){
		m_lblWord_1.SetWindowText(_T(" Reading Glass-id"));		
		m_lblWord_2.SetWindowText(_T(" Reading Glass-id Type"));		
		m_lblWord_3.SetWindowText(_T(" Glass-Type"));	
		m_lblWord_4.SetWindowText(_T(" Receive Refuse Code"));	
		m_lblWord_5.SetWindowText(_T(" CSIF"));	
		m_lblWord_6.SetWindowText(_T(" AS"));	
		m_lblWord_7.SetWindowText(_T(" APS"));	
		m_lblWord_8.SetWindowText(_T(""));	
		m_lblWord_9.SetWindowText(_T(""));	
		m_lblWord_10.SetWindowText(_T(""));	
		m_lblWord_11.SetWindowText(_T(""));	
		m_lblWord_12.SetWindowText(_T(""));	
		m_lblWord_13.SetWindowText(_T(""));	
		m_lblWord_14.SetWindowText(_T(""));	
		m_lblWord_15.SetWindowText(_T(""));	
		m_lblWord_16.SetWindowText(_T(""));	
		m_lblWord_17.SetWindowText(_T(""));	
		m_lblWord_18.SetWindowText(_T(""));	
		m_lblWord_19.SetWindowText(_T(""));	
		m_lblWord_20.SetWindowText(_T(""));	
		m_lblWord_21.SetWindowText(_T(""));	
		m_lblWord_22.SetWindowText(_T(""));	
		m_lblWord_23.SetWindowText(_T(""));	
		m_lblWord_24.SetWindowText(_T(""));	
		m_lblWord_25.SetWindowText(_T(""));	
		m_lblWord_26.SetWindowText(_T(""));	
		m_lblWord_27.SetWindowText(_T(""));	
		m_lblWord_28.SetWindowText(_T(""));	
		m_lblWord_29.SetWindowText(_T(""));	
		m_lblWord_30.SetWindowText(_T(""));	
		m_lblWord_31.SetWindowText(_T(""));	
		m_lblWord_32.SetWindowText(_T(""));	
	}
	//////////////////////////////////////////////////////////////////////////////////////
	// To Lower Equipment
	else if(3==nTitleIndex){
		m_lblWord_1.SetWindowText(_T(" H-Panel-id"));		
		m_lblWord_2.SetWindowText(_T(" E-Panel-id"));		
		m_lblWord_3.SetWindowText(_T(" BatchID"));	
		m_lblWord_4.SetWindowText(_T(" ProdType"));	
		m_lblWord_5.SetWindowText(_T(" ProdKind"));	
		m_lblWord_6.SetWindowText(_T(" DeviceID"));	
		m_lblWord_7.SetWindowText(_T(" StepID"));	
		m_lblWord_8.SetWindowText(_T(" PPID"));	
		m_lblWord_9.SetWindowText(_T(" Thickness"));	
		m_lblWord_10.SetWindowText(_T(" InsFlag"));	
		m_lblWord_11.SetWindowText(_T(" PanelSize"));	
		m_lblWord_12.SetWindowText(_T(" PanelPosition"));	
		m_lblWord_13.SetWindowText(_T(" Count1"));	
		m_lblWord_14.SetWindowText(_T(" Count2"));	
		m_lblWord_15.SetWindowText(_T(" Grade"));	
		m_lblWord_16.SetWindowText(_T(" Comment"));	
		m_lblWord_17.SetWindowText(_T(" CompSize"));	
		m_lblWord_18.SetWindowText(_T(" ReadingFlag"));	
		m_lblWord_19.SetWindowText(_T(" PanelState"));	
		m_lblWord_20.SetWindowText(_T(" Judgement"));	
		m_lblWord_21.SetWindowText(_T(" Code"));	
		m_lblWord_22.SetWindowText(_T(" RunLine"));	
		m_lblWord_23.SetWindowText(_T(" UniqueID"));	
		m_lblWord_24.SetWindowText(_T(" PairHPanelID"));	
		m_lblWord_25.SetWindowText(_T(" PairEPanelID"));	
		m_lblWord_26.SetWindowText(_T(" PairGrade"));	
		m_lblWord_27.SetWindowText(_T(" PairUniqueID"));	
		m_lblWord_28.SetWindowText(_T(" FlowRecipe"));	
		m_lblWord_29.SetWindowText(_T(" Reserved"));	
		m_lblWord_30.SetWindowText(_T(" BitsSignal"));	
		m_lblWord_31.SetWindowText(_T(" Refer Data"));	
		m_lblWord_32.SetWindowText(_T(""));	
	}
	else if (4==nTitleIndex){
		m_lblWord_1.SetWindowText(_T(" m_strDeviceID"));		
		m_lblWord_2.SetWindowText(_T(" m_strEQState"));		
		m_lblWord_3.SetWindowText(_T(" m_strReplyCode"));	
		m_lblWord_4.SetWindowText(_T(" m_strPanelWIP"));	
		m_lblWord_5.SetWindowText(_T(""));	
		m_lblWord_6.SetWindowText(_T(""));		
		m_lblWord_7.SetWindowText(_T(""));		
		m_lblWord_8.SetWindowText(_T(""));		
		m_lblWord_9.SetWindowText(_T(""));		
		m_lblWord_10.SetWindowText(_T(""));	
		m_lblWord_11.SetWindowText(_T(""));	
		m_lblWord_12.SetWindowText(_T(""));	
		m_lblWord_13.SetWindowText(_T(""));	
		m_lblWord_14.SetWindowText(_T(""));	
		m_lblWord_15.SetWindowText(_T(""));	
		m_lblWord_16.SetWindowText(_T(""));	
		m_lblWord_17.SetWindowText(_T(""));		
		m_lblWord_18.SetWindowText(_T(""));		
		m_lblWord_19.SetWindowText(_T(""));		
		m_lblWord_20.SetWindowText(_T(""));		
		m_lblWord_21.SetWindowText(_T(""));	
		m_lblWord_22.SetWindowText(_T(""));	
		m_lblWord_23.SetWindowText(_T(""));	
		m_lblWord_24.SetWindowText(_T(""));	
		m_lblWord_25.SetWindowText(_T(""));	
		m_lblWord_26.SetWindowText(_T(""));	
		m_lblWord_27.SetWindowText(_T(""));	
		m_lblWord_28.SetWindowText(_T(""));	
		m_lblWord_29.SetWindowText(_T(""));	
		m_lblWord_30.SetWindowText(_T(""));	
		m_lblWord_31.SetWindowText(_T(""));	
		m_lblWord_32.SetWindowText(_T(""));	
	}
	//SJ_YYK 150821 Add..
	else if (5==nTitleIndex){
		m_lblWord_1.SetWindowText(_T(" sPortID"));		
		m_lblWord_2.SetWindowText(_T(" sEQPState"));		
		m_lblWord_3.SetWindowText(_T(" sPORT_STATE"));	
		m_lblWord_4.SetWindowText(_T(" sPORT_TYPE"));	
		m_lblWord_5.SetWindowText(_T(" sCSTID"));	
		m_lblWord_6.SetWindowText(_T(" nMAP_STIF"));	
		m_lblWord_7.SetWindowText(_T(" nCUR_STIF"));	
		m_lblWord_8.SetWindowText(_T(" sM_BATCHID"));	
		m_lblWord_9.SetWindowText(_T(" sM_MAKER"));	
		m_lblWord_10.SetWindowText(_T(" nM_KIND"));	
		m_lblWord_11.SetWindowText(_T(" nM_TYPE"));	
		m_lblWord_12.SetWindowText(_T(" nM_STATE"));	
		m_lblWord_13.SetWindowText(_T(" nT_QTY"));	
		m_lblWord_14.SetWindowText(_T(" nU_QTY"));	
		m_lblWord_15.SetWindowText(_T(" nR_QTY"));	
		m_lblWord_16.SetWindowText(_T(" nN_QTY"));	
		m_lblWord_17.SetWindowText(_T(" nA_QTY"));	
		m_lblWord_18.SetWindowText(_T(" nE_FLAG"));	
		m_lblWord_19.SetWindowText(_T(" sC_CODE"));	
		m_lblWord_20.SetWindowText(_T(" sM_STEP"));	
		m_lblWord_21.SetWindowText(_T(" nS_QTY"));	
		m_lblWord_22.SetWindowText(_T(" sM_CODE"));	
		m_lblWord_23.SetWindowText(_T(""));	
		m_lblWord_24.SetWindowText(_T(""));	
		m_lblWord_25.SetWindowText(_T(""));	
		m_lblWord_26.SetWindowText(_T(""));	
		m_lblWord_27.SetWindowText(_T(""));	
		m_lblWord_28.SetWindowText(_T(""));	
		m_lblWord_29.SetWindowText(_T(""));	
		m_lblWord_30.SetWindowText(_T(""));	
		m_lblWord_31.SetWindowText(_T(""));	
		m_lblWord_32.SetWindowText(_T(""));	
	}
	else if (6==nTitleIndex){
		m_lblWord_1.SetWindowText(_T(" ToolTempM01"));		
		m_lblWord_2.SetWindowText(_T(" ToolTempS01"));		
		m_lblWord_3.SetWindowText(_T(" ToolTempM02"));	
		m_lblWord_4.SetWindowText(_T(" ToolTempS02"));	
		m_lblWord_5.SetWindowText(_T(" ToolTempM03"));	
		m_lblWord_6.SetWindowText(_T(" ToolTempS03"));	
		m_lblWord_7.SetWindowText(_T(" ToolTempM04"));	
		m_lblWord_8.SetWindowText(_T(" ToolTempS04"));	
		m_lblWord_9.SetWindowText(_T(" IndexTempM01"));	
		m_lblWord_10.SetWindowText(_T(" IndexTempS01"));	
		m_lblWord_11.SetWindowText(_T(" IndexTempM02"));	
		m_lblWord_12.SetWindowText(_T(" IndexTempS02"));	
		m_lblWord_13.SetWindowText(_T(" IndexTempM03"));	
		m_lblWord_14.SetWindowText(_T(" IndexTempS03"));	
		m_lblWord_15.SetWindowText(_T(" IndexTempM04"));	
		m_lblWord_16.SetWindowText(_T(" IndexTempS04"));	
		m_lblWord_17.SetWindowText(_T(" IndexTempM05"));	
		m_lblWord_18.SetWindowText(_T(" IndexTempS05"));	
		m_lblWord_19.SetWindowText(_T(" IndexTempM06"));	
		m_lblWord_20.SetWindowText(_T(" IndexTempS06"));	
		m_lblWord_21.SetWindowText(_T(""));	
		m_lblWord_22.SetWindowText(_T(""));	
		m_lblWord_23.SetWindowText(_T(""));	
		m_lblWord_24.SetWindowText(_T(""));	
		m_lblWord_25.SetWindowText(_T(""));	
		m_lblWord_26.SetWindowText(_T(""));	
		m_lblWord_27.SetWindowText(_T(""));	
		m_lblWord_28.SetWindowText(_T(""));	
		m_lblWord_29.SetWindowText(_T(""));	
		m_lblWord_30.SetWindowText(_T(""));	
		m_lblWord_31.SetWindowText(_T(""));	
		m_lblWord_32.SetWindowText(_T(""));	
	}
}

/*========================================================================================
	FUNCTION : MNetH::ViewBitCtlVisible()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/01, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::ViewBitCtlVisible(bool bView)
{
	for(int i=0; i<32; i++){
		m_btnBit[i]->ShowWindow((BOOL)bView);
	}
}

/*========================================================================================
	FUNCTION : MNetH::ViewWordCtlVisible()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/09/01, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::ViewWordCtlVisible(bool bView)
{
	for(int i=0; i<32; i++){
		m_lblWord[i]->ShowWindow((BOOL)bView);
		m_edtWord[i]->ShowWindow((BOOL)bView);
	}
}

/*========================================================================================
	FUNCTION : MNetH::OnSelchangeTabPLC()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/14, KJH
	LAST	 : 2004/08/14, KJH
========================================================================================*/
void MNetHDlg::OnSelchangeTabPLC(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

/*========================================================================================
	FUNCTION : MNetH::UpdateBitData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/14, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::UpdateBitData(MRobotArmStateBit *p, bool bEtcFullZero) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if(0==nSelArea && (2==nSelData || 3==nSelData)){  
		UpdateSignalBit(nIndex++,  p->m_nRobotAbnormal);			
		UpdateSignalBit(nIndex++,  p->m_nTypeOfArm);				
		UpdateSignalBit(nIndex++,  p->m_nTypeOfStageConveyor);		
		UpdateSignalBit(nIndex++,  p->m_nArm1Violate);		
		UpdateSignalBit(nIndex++,  p->m_nArm2Violate);		
		UpdateSignalBit(nIndex++,  p->m_nArm1FoldComplete);		
		UpdateSignalBit(nIndex++,  p->m_nArm2FoldComplete);	
		UpdateSignalBit(nIndex++,  p->m_nArm1GlassCheckSensor);			
		UpdateSignalBit(nIndex++,  p->m_nArm2GlassCheckSensor);		
		UpdateSignalBit(nIndex++,  p->m_nReserved0);				
		UpdateSignalBit(nIndex++,  p->m_nReserved1);			
		UpdateSignalBit(nIndex++,  p->m_nReserved2);			
		UpdateSignalBit(nIndex++,  p->m_nManualOperation);				
		UpdateSignalBit(nIndex++,  p->m_nRobotDirection);				
		UpdateSignalBit(nIndex++,  p->m_nReserved3);				
		UpdateSignalBit(nIndex++,  p->m_nReserved4);	
		if(bEtcFullZero){
			UpdateSignalBit(nIndex++,  0);					
			UpdateSignalBit(nIndex++,  0);					
			UpdateSignalBit(nIndex++,  0);						
			UpdateSignalBit(nIndex++,  0);						
			UpdateSignalBit(nIndex++,  0);						
			UpdateSignalBit(nIndex++,  0);						
			UpdateSignalBit(nIndex++,  0);					
			UpdateSignalBit(nIndex++,  0);					
			UpdateSignalBit(nIndex++,  0);		
			UpdateSignalBit(nIndex++,  0);		
			UpdateSignalBit(nIndex++,  0);		
			UpdateSignalBit(nIndex++,  0);	
			UpdateSignalBit(nIndex++,  0);	
			UpdateSignalBit(nIndex++,  0);					
			UpdateSignalBit(nIndex++,  0);					
			UpdateSignalBit(nIndex++,  0);	
		}
	}
}


/*========================================================================================
	FUNCTION : MNetH::UpdateBitData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/14, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::UpdateBitData(MStageStateBit *p, bool bEtcFullZero) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if(0==nSelArea && (2==nSelData || 3==nSelData)){  
		if(bEtcFullZero){
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
			UpdateSignalBit(nIndex++,  0);
		}
		else{ nIndex=17; }
		UpdateSignalBit(nIndex++,  p->m_nStageCVAbnormal);							
		UpdateSignalBit(nIndex++,  p->m_nTypeOfArm);								
		UpdateSignalBit(nIndex++,  p->m_nTypeOfStageConveyor);							
		UpdateSignalBit(nIndex++,  p->m_nEmpty);										
		UpdateSignalBit(nIndex++,  p->m_nIdle);											
		UpdateSignalBit(nIndex++,  p->m_nBusy);											
		UpdateSignalBit(nIndex++,  p->m_nPinUp);									
		UpdateSignalBit(nIndex++,  p->m_nPinDown);									
		UpdateSignalBit(nIndex++,  p->m_nStopperUp);						
		UpdateSignalBit(nIndex++,  p->m_nStopperDown);					
		UpdateSignalBit(nIndex++,  p->m_nDoorOpen);					
		UpdateSignalBit(nIndex++,  p->m_nDoorClose);		
		UpdateSignalBit(nIndex++,  p->m_nManualOperation);			
		UpdateSignalBit(nIndex++,  p->m_nBodyMoving);								
		UpdateSignalBit(nIndex++,  p->m_nGlassCheckSensorOn);								
		UpdateSignalBit(nIndex++,  p->m_nReserved0);	
	}

}


/*========================================================================================
	FUNCTION : MNetH::UpdateBitData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/14, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::UpdateBitData(MToUpperEqBit *pUS) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if(0==nSelArea && 0==nSelData){  
		UpdateSignalBit(nIndex++, pUS->m_nReserved0);					
		UpdateSignalBit(nIndex++, pUS->m_nMachinePause);					
		UpdateSignalBit(nIndex++, pUS->m_nMachineDown);					
		UpdateSignalBit(nIndex++, pUS->m_nMachineAlarm);					
		UpdateSignalBit(nIndex++, pUS->m_nReceiveAble);					
		UpdateSignalBit(nIndex++, pUS->m_nReceiveStart);					
		UpdateSignalBit(nIndex++, pUS->m_nReceiveComplete);				
		UpdateSignalBit(nIndex++, pUS->m_nExchangeFlag);					
		UpdateSignalBit(nIndex++, pUS->m_nReturnSendStart);				
		UpdateSignalBit(nIndex++, pUS->m_nReturnSendComplete);	
		UpdateSignalBit(nIndex++, pUS->m_nAction1Complete);					
		UpdateSignalBit(nIndex++, pUS->m_nAction2Complete);					
		UpdateSignalBit(nIndex++, pUS->m_nReserved1);					
		UpdateSignalBit(nIndex++, pUS->m_nReserved2);					
		UpdateSignalBit(nIndex++, pUS->m_nReceiveRefuse);			
		UpdateSignalBit(nIndex++, pUS->m_nGlassIdReadComplete);			
		UpdateSignalBit(nIndex++, pUS->m_nLoadingStop);					
		UpdateSignalBit(nIndex++, pUS->m_nTransferStop);				
		UpdateSignalBit(nIndex++, pUS->m_nImmediatelyPauseRequest);	
		UpdateSignalBit(nIndex++, pUS->m_nImmediatelyStopRequest);		
		UpdateSignalBit(nIndex++, pUS->m_nReceiveAbleRemainedStep0);	
		UpdateSignalBit(nIndex++, pUS->m_nReceiveAbleRemainedStep1);	
		UpdateSignalBit(nIndex++, pUS->m_nReceiveAbleRemainedStep2);	
		UpdateSignalBit(nIndex++, pUS->m_nReceiveAbleRemainedStep3);
		UpdateSignalBit(nIndex++, pUS->m_nHandShakeCancelRequest);		
		UpdateSignalBit(nIndex++, pUS->m_nHandShakeAbortRequest);		
		UpdateSignalBit(nIndex++, pUS->m_nHandShakeResumeRequest);		
		UpdateSignalBit(nIndex++, pUS->m_nHandShakeRecoveryAckReply);	
		UpdateSignalBit(nIndex++, pUS->m_nHandShakeRecoveryNckReply);	
		UpdateSignalBit(nIndex++, pUS->m_nReserved3);					
		UpdateSignalBit(nIndex++, pUS->m_nReserved4);					
		UpdateSignalBit(nIndex++, pUS->m_nReserved5);					
	}

}


/*========================================================================================
	FUNCTION : MNetH::UpdateBitData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/14, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::UpdateBitData(MToLowerEqBit *pLS) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if(0==nSelArea && 1==nSelData){  
		UpdateSignalBit(nIndex++,  pLS->m_nReserved0);					
		UpdateSignalBit(nIndex++,  pLS->m_nMachinePause);					
		UpdateSignalBit(nIndex++,  pLS->m_nMachineDown);					
		UpdateSignalBit(nIndex++,  pLS->m_nMachineAlarm);					
		UpdateSignalBit(nIndex++,  pLS->m_nSendAble);						
		UpdateSignalBit(nIndex++,  pLS->m_nSendStart);					
		UpdateSignalBit(nIndex++,  pLS->m_nSendComplete);					
		UpdateSignalBit(nIndex++,  pLS->m_nExchangeFlag);					
		UpdateSignalBit(nIndex++,  pLS->m_nReturnReceiveStart);			
		UpdateSignalBit(nIndex++,  pLS->m_nReturnReceiveComplete);	
		UpdateSignalBit(nIndex++,  pLS->m_nAction1Request);			
		UpdateSignalBit(nIndex++,  pLS->m_nAction2Request);	
		UpdateSignalBit(nIndex++,  pLS->m_nReserved1);					
		UpdateSignalBit(nIndex++,  pLS->m_nReserved2);					
		UpdateSignalBit(nIndex++,  pLS->m_nWorkStart);					
		UpdateSignalBit(nIndex++,  pLS->m_nWorkCancel);					
		UpdateSignalBit(nIndex++,  pLS->m_nReserved3);					
		UpdateSignalBit(nIndex++,  pLS->m_nReserved4);					
		UpdateSignalBit(nIndex++,  pLS->m_nImmediatelyPauseRequest);		
		UpdateSignalBit(nIndex++,  pLS->m_nImmediatelyStopRequest);
		UpdateSignalBit(nIndex++,  pLS->m_nSendAbleRemainedStep0);		
		UpdateSignalBit(nIndex++,  pLS->m_nSendAbleRemainedStep1);		
		UpdateSignalBit(nIndex++,  pLS->m_nSendAbleRemainedStep2);		
		UpdateSignalBit(nIndex++,  pLS->m_nSendAbleRemainedStep3);
		UpdateSignalBit(nIndex++,  pLS->m_nHandShakeCancelRequest);		
		UpdateSignalBit(nIndex++,  pLS->m_nHandShakeAbortRequest);		
		UpdateSignalBit(nIndex++,  pLS->m_nHandShakeResumeRequest);		
		UpdateSignalBit(nIndex++,  pLS->m_nHandShakeRecoveryAckReply);	
		UpdateSignalBit(nIndex++,  pLS->m_nHandShakeRecoveryNckReply);	
		UpdateSignalBit(nIndex++,  pLS->m_nReserved5);					
		UpdateSignalBit(nIndex++,  pLS->m_nReserved6);					
		UpdateSignalBit(nIndex++,  pLS->m_nReserved7);					
	}
}

/*========================================================================================
	FUNCTION : MNetH::UpdateBitData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2009/05/29, KJS
	LAST	 : 2009/05/29, KJS
========================================================================================*/
void MNetHDlg::UpdateBitData(MModelChangeBitDatas *pBD) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem="";
	
	
	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if (0==nSelArea && 4==nSelData){  
		UpdateSignalBit(nIndex++,  pBD->m_iModelChangeReserve);					
		UpdateSignalBit(nIndex++,  pBD->m_iModelChangeStart);					
		UpdateSignalBit(nIndex++,  pBD->m_iModelChangeComplete);					
		UpdateSignalBit(nIndex++,  pBD->m_iModelChangeDisable);					
		// Add. 110525 기종변경 예약 Bit 추가
		UpdateSignalBit(nIndex++,  pBD->m_iModelReserveComplete);
		UpdateSignalBit(nIndex++,  pBD->m_iModelReserveDisable);
	}
}

/*========================================================================================
	FUNCTION : MNetH::MonitoringStart()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/12, KJH
	LAST	 : 2004/08/14, KJH
========================================================================================*/
void MNetHDlg::MonitoringStart()
{
	SetTimer(0, 500, NULL);
}


/*========================================================================================
	FUNCTION : MNetH::MonitoringStop()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/12, KJH
	LAST	 : 2004/08/14, KJH
========================================================================================*/
void MNetHDlg::MonitoringStop()
{
	KillTimer(0);
}


/*========================================================================================
	FUNCTION : MNetH::UpdateEqState_Own()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/14, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::UpdateEqState_Own(MEqStates *pES_Own) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if(1==nSelArea && 0==nSelData){  
		UpdateWordData(nIndex++, pES_Own->m_strEqState);
		UpdateWordData(nIndex++, pES_Own->m_strProcState);
		UpdateWordData(nIndex++, pES_Own->m_strPPID);
		UpdateWordData(nIndex++, pES_Own->m_strGlassSize);
		UpdateWordData(nIndex++, pES_Own->m_strGlassThickness);
		UpdateWordData(nIndex++, pES_Own->m_strTACTSet);
		UpdateWordData(nIndex++, pES_Own->m_strTACTCurrent);
		UpdateWordData(nIndex++, pES_Own->m_strRCDG);
		UpdateWordData(nIndex++, pES_Own->m_strEOMode);
		UpdateWordData(nIndex++, pES_Own->m_strERCMode);
		UpdateWordData(nIndex++, pES_Own->m_strGMCMode);
		UpdateWordData(nIndex++, pES_Own->m_strOperateMode);
		UpdateWordData(nIndex++, pES_Own->m_strReserved);
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
	}
}


/*========================================================================================
	FUNCTION : MNetH::UpdateEqState_Related()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/14, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::UpdateEqState_Related(MEqStates *pES_Related) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if(1==nSelArea && 1==nSelData){  
		UpdateWordData(nIndex++, pES_Related->m_strEqState);
		UpdateWordData(nIndex++, pES_Related->m_strProcState);
		UpdateWordData(nIndex++, pES_Related->m_strPPID);
		UpdateWordData(nIndex++, pES_Related->m_strGlassSize);
		UpdateWordData(nIndex++, pES_Related->m_strGlassThickness);
		UpdateWordData(nIndex++, pES_Related->m_strTACTSet);
		UpdateWordData(nIndex++, pES_Related->m_strTACTCurrent);
		UpdateWordData(nIndex++, pES_Related->m_strRCDG);
		UpdateWordData(nIndex++, pES_Related->m_strEOMode);
		UpdateWordData(nIndex++, pES_Related->m_strERCMode);
		UpdateWordData(nIndex++, pES_Related->m_strGMCMode);
		UpdateWordData(nIndex++, pES_Related->m_strOperateMode);
		UpdateWordData(nIndex++, pES_Related->m_strReserved);
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
	}
}


/*========================================================================================
	FUNCTION : MNetH::UpdateJobSummary()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/14, KJH
	LAST	 : 2004/09/01, KJH
========================================================================================*/
void MNetHDlg::UpdateJobSummary(MGlassSummarys *pGS) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	int			nSelWorkGroup = 0;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	//20130205 SJ_HJG 
	nSelWorkGroup = m_tabUnit.GetCurSel();
	// Check curent Tab.
	if(1==nSelArea && 2==nSelData)
	{  
		InitWordAreaTitle(1);
		if(nSelWorkGroup == 0 ) //Front Group
		{
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[0]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[1]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[2]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[3]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[4]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[5]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[6]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[7]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[8]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[9]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[10]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[11]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[12]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[13]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[14]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[15]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[16]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[17]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[18]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[19]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[20]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[21]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[22]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataA[23]);
		}
		else //Rear Group
		{
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[0]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[1]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[2]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[3]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[4]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[5]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[6]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[7]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[8]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[9]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[10]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[11]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[12]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[13]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[14]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[15]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[16]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[17]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[18]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[19]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[20]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[21]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[22]);
			UpdateWordData(nIndex++, pGS->m_strReceiveTabOffsetDataB[23]);
			

		}

	//20111205 SJ_HJG
		/*
		UpdateWordData(nIndex++, pGS->m_strTGlassCount);
		UpdateWordData(nIndex++, pGS->m_strTFTCount);
		UpdateWordData(nIndex++, pGS->m_strCFCount);
		UpdateWordData(nIndex++, pGS->m_strDummyCount);
		UpdateWordData(nIndex++, pGS->m_strBPGCount);
		UpdateWordData(nIndex++, pGS->m_strProcessID1);
		UpdateWordData(nIndex++, pGS->m_strStepID1);
		UpdateWordData(nIndex++, pGS->m_strBatchID1);
		UpdateWordData(nIndex++, pGS->m_strGlassCount1);
		UpdateWordData(nIndex++, pGS->m_strProcessID2);
		UpdateWordData(nIndex++, pGS->m_strStepID2);
		UpdateWordData(nIndex++, pGS->m_strBatchID2);
		UpdateWordData(nIndex++, pGS->m_strGlassCount2);
		UpdateWordData(nIndex++, pGS->m_strProcessID3);
		UpdateWordData(nIndex++, pGS->m_strStepID3);
		UpdateWordData(nIndex++, pGS->m_strBatchID3);
		UpdateWordData(nIndex++, pGS->m_strGlassCount3);
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		*/
	}
}

/*========================================================================================
	FUNCTION : MNetH::UpdateLowerEqData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/15, KJH
	LAST	 : 2004/08/15, KJH
========================================================================================*/
void MNetHDlg::UpdateLowerEqData(MToLowerEqs *pLE) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if(1==nSelArea && 4==nSelData){  
		UpdateWordData(nIndex++, pLE->m_strHPanelID);
		UpdateWordData(nIndex++, pLE->m_strEPanelID);
		UpdateWordData(nIndex++, pLE->m_strBatchID);
		UpdateWordData(nIndex++, pLE->m_strProdType);
		UpdateWordData(nIndex++, pLE->m_strProdKind);
		UpdateWordData(nIndex++, pLE->m_strDeviceID);
		UpdateWordData(nIndex++, pLE->m_strStepID);
		UpdateWordData(nIndex++, pLE->m_strPPID);
		UpdateWordData(nIndex++, pLE->m_strThickness);
		UpdateWordData(nIndex++, pLE->m_strInsFlag);
		UpdateWordData(nIndex++, pLE->m_strPanelSize);
		UpdateWordData(nIndex++, pLE->m_strPanelPosition);
		UpdateWordData(nIndex++, pLE->m_strCount1);
		UpdateWordData(nIndex++, pLE->m_strCount2);
		UpdateWordData(nIndex++, pLE->m_strGrade);
		UpdateWordData(nIndex++, pLE->m_strComment);
		UpdateWordData(nIndex++, pLE->m_strCompSize);
		UpdateWordData(nIndex++, pLE->m_strReadingFlag);
		UpdateWordData(nIndex++, pLE->m_strPanelState);
		UpdateWordData(nIndex++, pLE->m_strJudgement);
		UpdateWordData(nIndex++, pLE->m_strCode);
		UpdateWordData(nIndex++, pLE->m_strRunLine);
		UpdateWordData(nIndex++, pLE->m_strUniqueId);
		UpdateWordData(nIndex++, pLE->m_strPairHPanelID);
		UpdateWordData(nIndex++, pLE->m_strPairEPanelID);
		UpdateWordData(nIndex++, pLE->m_strPairGrade);
		UpdateWordData(nIndex++, pLE->m_strPairUniqueId);
		UpdateWordData(nIndex++, pLE->m_strFlowRecipe);
		UpdateWordData(nIndex++, pLE->m_strReserved0);
		UpdateWordData(nIndex++, pLE->m_strBitsSignals);
		UpdateWordData(nIndex++, pLE->m_strReferData);
	}
}

/*========================================================================================
	FUNCTION : MNetH::UpdateUpperEqData()
	DESCRIPT : 
	RETURN	 : 
	ARGUMENT :
	FIRST	 : 2004/08/15, KJH
	LAST	 : 2004/08/15, KJH
========================================================================================*/
void MNetHDlg::UpdateUpperEqData(MToUpperEqs *pUE) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem=_T("");


	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	// Check curent Tab.
	if(1==nSelArea && 3==nSelData){  
		UpdateWordData(nIndex++, pUE->m_strReadingGlassID);
		UpdateWordData(nIndex++, pUE->m_strReadingGlassIDType);
		UpdateWordData(nIndex++, pUE->m_strGlassType);
		UpdateWordData(nIndex++, pUE->m_strReceiveRefuseCode);
		UpdateWordData(nIndex++, pUE->m_strCSIF);
		UpdateWordData(nIndex++, pUE->m_strAS);
		UpdateWordData(nIndex++, pUE->m_strAPS);
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
		UpdateWordData(nIndex++, _T(""));
	}
}

/*========================================================================================
FUNCTION : MNetH::UpdateModelChangeWord()
DESCRIPT : 
RETURN	 : 
ARGUMENT :
FIRST	 : 2009/05/29 KJS
LAST	 : 2009/05/29 KJS
========================================================================================*/
void MNetHDlg::UpdateModelChangeWord(MModelChangeWordDatas *pWD) 
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem="";
	
	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	
	// Check curent Tab.
	if (1==nSelArea && 5==nSelData)
	{  
		UpdateWordData(nIndex++, pWD->m_strDeviceID);
		UpdateWordData(nIndex++, pWD->m_strEQState);
		UpdateWordData(nIndex++, pWD->m_strReplyCode);
		UpdateWordData(nIndex++, pWD->m_strPanelWIP);
	}
}
//SJ_YYK 150821 Add...
void MNetHDlg::UpdateMaterialInfo(MMaterial* pMat)
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem="";
	
	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	
	// Check curent Tab.
	CString strTmp;
	if (1==nSelArea && 6==nSelData)
	{  
		UpdateWordData(nIndex++, pMat->m_sPORTID);
		UpdateWordData(nIndex++, pMat->m_sEQP_STATE);
		UpdateWordData(nIndex++, pMat->m_sPORT_STATE);
		UpdateWordData(nIndex++, pMat->m_sPORT_TYPE);
		UpdateWordData(nIndex++, pMat->m_sCSTID);
		strTmp.Format(_T("%d"),	pMat->m_nMAP_STIF);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nCUR_STIF);
		UpdateWordData(nIndex++, strTmp);
		UpdateWordData(nIndex++, pMat->m_sM_BATCHID);
		UpdateWordData(nIndex++, pMat->m_sM_MAKER);
		strTmp.Format(_T("%d"),	pMat->m_nM_KIND);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nM_TYPE);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nM_STATE);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nT_QTY);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nU_QTY);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nR_QTY);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nN_QTY);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nA_QTY);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%d"),	pMat->m_nE_FLAG);
		UpdateWordData(nIndex++, strTmp);
		UpdateWordData(nIndex++, pMat->m_sC_CODE);
		UpdateWordData(nIndex++, pMat->m_sM_STEP);
		strTmp.Format(_T("%d"),	pMat->m_nS_QTY);
		UpdateWordData(nIndex++, strTmp);
		UpdateWordData(nIndex++, pMat->m_sM_CODE);
	}
}

void MNetHDlg::UpdateFDCInfo(MProcessParamInfo* pParam)
{
	int			nSelArea=-1;
	int			nSelData=-1;
	int			nIndex=1;
	CString		sItem="";
	
	nSelArea = m_tabArea.GetCurSel();
	nSelData = m_tabData.GetCurSel();
	
	// Check curent Tab.
	CString strTmp;
	if (1==nSelArea && 7==nSelData)
	{  
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[0].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[0].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[1].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[1].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[2].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[2].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[3].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[3].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[4].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[4].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[5].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[5].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[6].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[6].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[7].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[7].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[8].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[8].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);

		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[9].m_dMeasureValue);
		UpdateWordData(nIndex++, strTmp);
		strTmp.Format(_T("%.3f"), pParam->m_rgsProcessParam[9].m_dSettingValue);
		UpdateWordData(nIndex++, strTmp);
	}
}
//__________________________________________*/
void MNetHDlg::OnSelchangeTabUnit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int			nSelUnit=-1;

	nSelUnit = m_tabUnit.GetCurSel();
	/////////////////////////////////////////////////////////////////
	// Unit 1
	if (0==nSelUnit){ 
//		UpdateSignalBit(1, 1);
	}
	/////////////////////////////////////////////////////////////////
	// Unit 2
	else if(1==nSelUnit){ 
//		UpdateSignalBit(1, 0);
	}
	
	*pResult = 0;
}

void MNetHDlg::OnSelchangeTabData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int			nSelArea=-1;
	int			nSelRow=-1;

	nSelArea = m_tabArea.GetCurSel();
	nSelRow = m_tabData.GetCurSel();
	/////////////////////////////////////////////////////////////////
	// Bit Area
	if (0==nSelArea){ 
		//===========================================================
		// To Upper Eqp
		if(0==nSelRow){
			InitBitAreaTitle(0);
		}
		//===========================================================
		// To Lower Eqp
		else if(1==nSelRow){
			InitBitAreaTitle(1);
		}
		//===========================================================
		// Contact Point State(to Upper)
		else if(2==nSelRow){
			InitBitAreaTitle(2);
		}
		//===========================================================
		// Contact Point State(to Lower)
		else if(3==nSelRow){
			InitBitAreaTitle(2);
		}
		// NSMC Bit
		else if (4==nSelRow){
			InitBitAreaTitle(3);
		}
	}
	/////////////////////////////////////////////////////////////////
	// Word Area
	else if(1==nSelArea){ 
		//===========================================================
		// Equipment State(Own)
		if(0==nSelRow){
			InitWordAreaTitle(0);
		}
		//===========================================================
		// Equipment State(Related)
		else if(1==nSelRow){
			InitWordAreaTitle(0);
		}
		//===========================================================
		// Job Summary
		else if(2==nSelRow){
			InitWordAreaTitle(1);
		}
		//===========================================================
		// To Upper Equipment
		else if(3==nSelRow){
			InitWordAreaTitle(2);
		}
		//===========================================================
		// To Lower Equipment
		else if(4==nSelRow){
			InitWordAreaTitle(3);
		}
		//===========================================================
		// NSMC Word
		else if (5==nSelRow){
			InitWordAreaTitle(4);
		}
		//Material SJ_YYK 150821 Add...
		else if (6==nSelRow){
			InitWordAreaTitle(5);
		}
		else if (7==nSelRow){
			InitWordAreaTitle(6);
		}
	}

	
	*pResult = 0;
}

void MNetHDlg::OnSelchangeTabLocal(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
/*
	if(0==GetCurSelLocalTab()){
		m_lblPrevLocal.SetWindowText(_T(""));
		m_lblOwnLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(0));
		m_lblNextLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(1));
	}
	else if(14==GetCurSelLocalTab()){
		m_lblPrevLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(13));
		m_lblOwnLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(14));
		m_lblNextLocal.SetWindowText(_T(""));
	}
	else if(0<GetCurSelLocalTab() && 14>GetCurSelLocalTab()){
		m_lblPrevLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(GetCurSelLocalTab()-1));
		m_lblOwnLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(GetCurSelLocalTab()));
		m_lblNextLocal.SetWindowText(m_pMNetH->m_asLocalName.GetAt(GetCurSelLocalTab()+1));
	}
	else{
		m_lblPrevLocal.SetWindowText(_T(""));
		m_lblOwnLocal.SetWindowText(_T(""));
		m_lblNextLocal.SetWindowText(_T(""));
	}
*/	
	*pResult = 0;
}

void MNetHDlg::OnSelchangeTabArea(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int			nSelRow=-1;

	nSelRow = m_tabArea.GetCurSel();
	/////////////////////////////////////////////////////////////////
	// Bit Area
	if ( 0==nSelRow ){ 
		InitDataTab(); 
		ViewBitCtlVisible(true);
		ViewWordCtlVisible(false);
	}
	/////////////////////////////////////////////////////////////////
	// Word Area
	else if(1==nSelRow){ 
		InitDataTab(false); 
		ViewBitCtlVisible(false);
		ViewWordCtlVisible(true);
	}

	

	*pResult = 0;
}

void MNetHDlg::UpdateSignalBit(int nBitIndex, int nValue) 
{
	if(0==nValue){
		if(1==m_btnBit[nBitIndex-1]->GetCheck()){
			m_btnBit[nBitIndex-1]->SetCheck(0);	
			m_btnBit[nBitIndex-1]->SetIcon(IDI_LED_OFF);
		}
	}
	else if(1==nValue){
		if(0==m_btnBit[nBitIndex-1]->GetCheck()){
			m_btnBit[nBitIndex-1]->SetCheck(1);	
			m_btnBit[nBitIndex-1]->SetIcon(IDI_LED_ON);
		}
	}
}

void MNetHDlg::UpdateWordData(int nItemIndex, CString sValue) 
{
	CString sStr=_T("");

	m_edtWord[nItemIndex-1]->GetWindowText(sStr);
	if(sStr!=sValue){
		m_edtWord[nItemIndex-1]->SetWindowText(sValue);
	}
}

int MNetHDlg::GetCurSelAreaTab() 
{
	return m_tabArea.GetCurSel();
}

int MNetHDlg::GetCurSelLocalTab() 
{
	return m_tabLocal.GetCurSel();
}

int MNetHDlg::GetCurSelUnitTab() 
{
	return m_tabUnit.GetCurSel();
}

int MNetHDlg::GetCurSelDataTab() 
{
	return m_tabData.GetCurSel();
}

void MNetHDlg::OnBtnBit1() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_1.GetCheck()){
					m_pMNetH->SetReserved0_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved0_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_1.GetCheck()){
					m_pMNetH->SetReserved0_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved0_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_1.GetCheck()){
					m_pMNetH->SetRobotAbnormal_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetRobotAbnormal_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_1.GetCheck()){
					m_pMNetH->SetRobotAbnormal_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetRobotAbnormal_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if (4==GetCurSelDataTab()){
				if (0==m_btnBit_1.GetCheck()){
					m_pMNetH->SetNSMCBit(1, 1);
				}
				else{
					m_pMNetH->SetNSMCBit(1, 0);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit2() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_2.GetCheck()){
					m_pMNetH->SetMachinePause_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetMachinePause_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_2.GetCheck()){
					m_pMNetH->SetMachinePause_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetMachinePause_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_2.GetCheck()){
					m_pMNetH->SetTypeOfArm_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTypeOfArm_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_2.GetCheck()){
					m_pMNetH->SetTypeOfArm_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTypeOfArm_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if (4==GetCurSelDataTab()){
				if (0==m_btnBit_2.GetCheck()){
					m_pMNetH->SetNSMCBit(2, 1);
				}
				else{
					m_pMNetH->SetNSMCBit(2, 0);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit3() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_3.GetCheck()){
					m_pMNetH->SetMachineDown_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetMachineDown_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_3.GetCheck()){
					m_pMNetH->SetMachineDown_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetMachineDown_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_3.GetCheck()){
					m_pMNetH->SetTypeOfStageConveyor_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTypeOfStageConveyor_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_3.GetCheck()){
					m_pMNetH->SetTypeOfStageConveyor_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTypeOfStageConveyor_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if (4==GetCurSelDataTab()){
				if (0==m_btnBit_3.GetCheck()){
					m_pMNetH->SetNSMCBit(3, 1);
				}
				else{
					m_pMNetH->SetNSMCBit(3, 0);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit4() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_4.GetCheck()){
					m_pMNetH->SetMachineAlarm_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetMachineAlarm_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_4.GetCheck()){
					m_pMNetH->SetMachineAlarm_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetMachineAlarm_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_4.GetCheck()){
					m_pMNetH->SetArm1Violate_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm1Violate_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_4.GetCheck()){
					m_pMNetH->SetArm1Violate_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm1Violate_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if (4==GetCurSelDataTab()){
				if (0==m_btnBit_4.GetCheck()){
					m_pMNetH->SetNSMCBit(4, 1);
				}
				else{
					m_pMNetH->SetNSMCBit(4, 0);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit5() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_5.GetCheck()){
					m_pMNetH->SetReceiveAble_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReceiveAble_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_5.GetCheck()){
					m_pMNetH->SetSendAble_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetSendAble_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_5.GetCheck()){
					m_pMNetH->SetArm2Violate_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm2Violate_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_5.GetCheck()){
					m_pMNetH->SetArm2Violate_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm2Violate_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit6() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_6.GetCheck()){
					m_pMNetH->SetReceiveStart_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReceiveStart_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_6.GetCheck()){
					m_pMNetH->SetSendStart_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetSendStart_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_6.GetCheck()){
					m_pMNetH->SetArm1FoldComplete_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm1FoldComplete_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_6.GetCheck()){
					m_pMNetH->SetArm1FoldComplete_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm1FoldComplete_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit7() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_7.GetCheck()){
					m_pMNetH->SetReceiveComplete_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReceiveComplete_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_7.GetCheck()){
					m_pMNetH->SetSendComplete_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetSendComplete_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_7.GetCheck()){
					m_pMNetH->SetArm2FoldComplete_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm2FoldComplete_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_7.GetCheck()){
					m_pMNetH->SetArm2FoldComplete_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm2FoldComplete_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit8() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_8.GetCheck()){
					m_pMNetH->SetExchangeFlag_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetExchangeFlag_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_8.GetCheck()){
					m_pMNetH->SetExchangeFlag_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetExchangeFlag_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_8.GetCheck()){
					m_pMNetH->SetArm1GlassCheckSensorOn_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm1GlassCheckSensorOn_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_8.GetCheck()){
					m_pMNetH->SetArm1GlassCheckSensorOn_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm1GlassCheckSensorOn_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit9() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_9.GetCheck()){
					m_pMNetH->SetReturnSendStart_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReturnSendStart_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_9.GetCheck()){
					m_pMNetH->SetReturnReceiveStart_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReturnReceiveStart_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_9.GetCheck()){
					m_pMNetH->SetArm2GlassCheckSensorOn_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm2GlassCheckSensorOn_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_9.GetCheck()){
					m_pMNetH->SetArm2GlassCheckSensorOn_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetArm2GlassCheckSensorOn_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit10() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_10.GetCheck()){
					m_pMNetH->SetReturnSendComplete_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReturnSendComplete_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_10.GetCheck()){
					m_pMNetH->SetReturnReceiveComplete_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReturnReceiveComplete_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_10.GetCheck()){
					m_pMNetH->SetReserved0_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved0_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_10.GetCheck()){
					m_pMNetH->SetReserved0_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved0_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit11() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_11.GetCheck()){
					m_pMNetH->SetAction1Complete_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetAction1Complete_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_11.GetCheck()){
					m_pMNetH->SetAction1Request_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetAction1Request_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_11.GetCheck()){
					m_pMNetH->SetReserved1_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved1_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_11.GetCheck()){
					m_pMNetH->SetReserved1_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved1_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit12() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_12.GetCheck()){
					m_pMNetH->SetAction2Complete_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetAction2Complete_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_12.GetCheck()){
					m_pMNetH->SetAction2Request_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetAction2Request_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_12.GetCheck()){
					m_pMNetH->SetReserved2_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved2_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_12.GetCheck()){
					m_pMNetH->SetReserved2_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved2_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit13() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_13.GetCheck()){
					m_pMNetH->SetReserved1_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved1_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_13.GetCheck()){
					m_pMNetH->SetReserved1_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved1_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_13.GetCheck()){
					m_pMNetH->SetManualOperation_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetManualOperation_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_13.GetCheck()){
					m_pMNetH->SetManualOperation_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetManualOperation_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit14() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_14.GetCheck()){
					m_pMNetH->SetReserved2_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved2_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_14.GetCheck()){
					m_pMNetH->SetReserved2_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved2_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_14.GetCheck()){
					m_pMNetH->SetRobotDirection_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetRobotDirection_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_14.GetCheck()){
					m_pMNetH->SetRobotDirection_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetRobotDirection_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit15() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_15.GetCheck()){
					m_pMNetH->SetReceiveRefuse_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReceiveRefuse_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_15.GetCheck()){
					m_pMNetH->SetWorkStart_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetWorkStart_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_15.GetCheck()){
					m_pMNetH->SetReserved3_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved3_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_15.GetCheck()){
					m_pMNetH->SetReserved3_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved3_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit16() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_16.GetCheck()){
					m_pMNetH->SetGlassIdReadComplete_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetGlassIdReadComplete_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_16.GetCheck()){
					m_pMNetH->SetWorkCancel_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetWorkCancel_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_16.GetCheck()){
					m_pMNetH->SetReserved4_Robot_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved4_Robot_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_16.GetCheck()){
					m_pMNetH->SetReserved4_Robot_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved4_Robot_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit17() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_17.GetCheck()){
					m_pMNetH->SetLoadingStop_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetLoadingStop_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_17.GetCheck()){
					m_pMNetH->SetReserved3_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved3_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_17.GetCheck()){
					m_pMNetH->SetStageAbnormal_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetStageAbnormal_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_17.GetCheck()){
					m_pMNetH->SetStageAbnormal_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetStageAbnormal_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit18() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_18.GetCheck()){
					m_pMNetH->SetTransferStop_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTransferStop_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_18.GetCheck()){
					m_pMNetH->SetReserved4_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved4_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_18.GetCheck()){
					m_pMNetH->SetTypeOfArm_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTypeOfArm_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_18.GetCheck()){
					m_pMNetH->SetTypeOfArm_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTypeOfArm_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit19() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_19.GetCheck()){
					m_pMNetH->SetImmediatelyPauseRequest_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetImmediatelyPauseRequest_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_19.GetCheck()){
					m_pMNetH->SetImmediatelyPauseRequest_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetImmediatelyPauseRequest_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_19.GetCheck()){
					m_pMNetH->SetTypeOfStageConveyor_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTypeOfStageConveyor_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_19.GetCheck()){
					m_pMNetH->SetTypeOfStageConveyor_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetTypeOfStageConveyor_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit20() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_20.GetCheck()){
					m_pMNetH->SetImmediatelyStopRequest_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetImmediatelyStopRequest_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_20.GetCheck()){
					m_pMNetH->SetImmediatelyStopRequest_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetImmediatelyStopRequest_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_20.GetCheck()){
					m_pMNetH->SetEmpty_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetEmpty_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_20.GetCheck()){
					m_pMNetH->SetEmpty_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetEmpty_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit21() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_21.GetCheck()){
					m_pMNetH->SetReceiveAbleRemainedStep0_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReceiveAbleRemainedStep0_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_21.GetCheck()){
					m_pMNetH->SetSendAbleRemainedStep0_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetSendAbleRemainedStep0_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_21.GetCheck()){
					m_pMNetH->SetIdle_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetIdle_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_21.GetCheck()){
					m_pMNetH->SetIdle_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetIdle_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit22() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_22.GetCheck()){
					m_pMNetH->SetReceiveAbleRemainedStep1_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReceiveAbleRemainedStep1_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_22.GetCheck()){
					m_pMNetH->SetSendAbleRemainedStep1_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetSendAbleRemainedStep1_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_22.GetCheck()){
					m_pMNetH->SetBusy_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetBusy_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_22.GetCheck()){
					m_pMNetH->SetBusy_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetBusy_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit23() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_23.GetCheck()){
					m_pMNetH->SetReceiveAbleRemainedStep2_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReceiveAbleRemainedStep2_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_23.GetCheck()){
					m_pMNetH->SetSendAbleRemainedStep2_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetSendAbleRemainedStep2_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_23.GetCheck()){
					m_pMNetH->SetPinUp_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetPinUp_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_23.GetCheck()){
					m_pMNetH->SetPinUp_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetPinUp_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit24() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_24.GetCheck()){
					m_pMNetH->SetReceiveAbleRemainedStep3_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReceiveAbleRemainedStep3_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_24.GetCheck()){
					m_pMNetH->SetSendAbleRemainedStep3_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetSendAbleRemainedStep3_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_24.GetCheck()){
					m_pMNetH->SetPinDown_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetPinDown_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_24.GetCheck()){
					m_pMNetH->SetPinDown_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetPinDown_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit25() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_25.GetCheck()){
					m_pMNetH->SetHandShakeCancelRequest_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeCancelRequest_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_25.GetCheck()){
					m_pMNetH->SetHandShakeCancelRequest_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeCancelRequest_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_25.GetCheck()){
					m_pMNetH->SetStopperUp_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetStopperUp_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_25.GetCheck()){
					m_pMNetH->SetStopperUp_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetStopperUp_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit26() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_26.GetCheck()){
					m_pMNetH->SetHandShakeAbortRequest_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeAbortRequest_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_26.GetCheck()){
					m_pMNetH->SetHandShakeAbortRequest_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeAbortRequest_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_26.GetCheck()){
					m_pMNetH->SetStopperDown_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetStopperDown_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_26.GetCheck()){
					m_pMNetH->SetStopperDown_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetStopperDown_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit27() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_27.GetCheck()){
					m_pMNetH->SetHandShakeResumeRequest_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeResumeRequest_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_27.GetCheck()){
					m_pMNetH->SetHandShakeResumeRequest_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeResumeRequest_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_27.GetCheck()){
					m_pMNetH->SetDoorOpen_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetDoorOpen_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_27.GetCheck()){
					m_pMNetH->SetDoorOpen_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetDoorOpen_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit28() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_28.GetCheck()){
					m_pMNetH->SetHandShakeRecoveryAckReply_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeRecoveryAckReply_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_28.GetCheck()){
					m_pMNetH->SetHandShakeRecoveryAckReply_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeRecoveryAckReply_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_28.GetCheck()){
					m_pMNetH->SetDoorClose_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetDoorClose_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_28.GetCheck()){
					m_pMNetH->SetDoorClose_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetDoorClose_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit29() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_29.GetCheck()){
					m_pMNetH->SetHandShakeRecoveryNckReply_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeRecoveryNckReply_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_29.GetCheck()){
					m_pMNetH->SetHandShakeRecoveryNckReply_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetHandShakeRecoveryNckReply_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_29.GetCheck()){
					m_pMNetH->SetManualOperation_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetManualOperation_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_29.GetCheck()){
					m_pMNetH->SetManualOperation_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetManualOperation_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit30() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_30.GetCheck()){
					m_pMNetH->SetReserved3_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved3_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_30.GetCheck()){
					m_pMNetH->SetReserved5_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved5_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_30.GetCheck()){
					m_pMNetH->SetBodyMoving_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetBodyMoving_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_30.GetCheck()){
					m_pMNetH->SetBodyMoving_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetBodyMoving_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit31() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_31.GetCheck()){
					m_pMNetH->SetReserved4_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved4_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_31.GetCheck()){
					m_pMNetH->SetReserved6_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved6_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_31.GetCheck()){
					m_pMNetH->SetGlassCheckSensorOn_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetGlassCheckSensorOn_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_31.GetCheck()){
					m_pMNetH->SetGlassCheckSensorOn_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetGlassCheckSensorOn_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}
void MNetHDlg::OnBtnBit32() 
{
	if(m_bEditSignalMode){
		/////////////////////////////////////////////////////////////////
		// Bit Area
		if (0==GetCurSelAreaTab()){ 
			//===========================================================
			// To Upper Eqp
			if(0==GetCurSelDataTab()){
				if(0==m_btnBit_32.GetCheck()){
					m_pMNetH->SetReserved5_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved5_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// To Lower Eqp
			else if(1==GetCurSelDataTab()){
				if(0==m_btnBit_32.GetCheck()){
					m_pMNetH->SetReserved7_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved7_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Upper)
			else if(2==GetCurSelDataTab()){
				if(0==m_btnBit_32.GetCheck()){
					m_pMNetH->SetReserved0_Stage_ToUpperEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved0_Stage_ToUpperEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
			//===========================================================
			// Contact Point State(to Lower)
			else if(3==GetCurSelDataTab()){
				if(0==m_btnBit_32.GetCheck()){
					m_pMNetH->SetReserved0_Stage_ToLowerEqBit(1, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
				else{
					m_pMNetH->SetReserved0_Stage_ToLowerEqBit(0, GetCurSelLocalTab()+1, GetCurSelUnitTab()+1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////
	}// if(m_bEditSignalMode)
}


void MNetHDlg::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}
