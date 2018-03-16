
// ProductCreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "ProductCreateDlg.h"
#include "ProductPlanDlg.h"
#include "common.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProductCreateDlg dialog


CProductCreateDlg::CProductCreateDlg(int nProductID, CProductPlanDlg *pProductPlan, int nSel, CWnd* pParent /*=NULL*/)
	: CDialog(CProductCreateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProductCreateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_plnkProductPlan = pProductPlan;
	m_nCurrentSel = nSel;
	m_nProductID = nProductID;
}


void CProductCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProductCreateDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_WORK_ORDER, m_lblProductData[0]);
	DDX_Control(pDX, IDC_DEVIDE_ID, m_lblProductData[1]);
	DDX_Control(pDX, IDC_FIRST_PRODUCT_ORDER, m_lblProductData[2]);
	DDX_Control(pDX, IDC_FINAL_PRODUCT_ORDER, m_lblProductData[3]);
	DDX_Control(pDX, IDC_BATCH_STATE, m_lblProductData[4]);
	DDX_Control(pDX, IDC_PPID, m_lblProductData[5]);

	DDX_Control(pDX, IDS_WORK_ORDER, m_lblWorkOrder);
	DDX_Control(pDX, IDS_DEVIDE_ID, m_lblDeviceID);
	DDX_Control(pDX, IDS_FIRST_PRODUCT_ORDER, m_lblInitProductPlanCnt);
	DDX_Control(pDX, IDS_FINAL_PRODUCT_ORDER, m_lblLastProductPlanCnt);
	DDX_Control(pDX, IDS_BATCH_STATE, m_lblBatchStateCnt);
	DDX_Control(pDX, IDS_PPID, m_lblPPID);
}


BEGIN_MESSAGE_MAP(CProductCreateDlg, CDialog)
	//{{AFX_MSG_MAP(CProductCreateDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProductCreateDlg message handlers

BEGIN_EVENTSINK_MAP(CProductCreateDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CProductCreateDlg)
	ON_EVENT(CProductCreateDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CProductCreateDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CProductCreateDlg, IDC_WORK_ORDER, -600 /* Click */, OnClickWorkOrder, VTS_NONE)
	ON_EVENT(CProductCreateDlg, IDC_FIRST_PRODUCT_ORDER, -600 /* Click */, OnClickFirstProductOrder, VTS_NONE)
	ON_EVENT(CProductCreateDlg, IDC_FINAL_PRODUCT_ORDER, -600 /* Click */, OnClickFinalProductOrder, VTS_NONE)
	ON_EVENT(CProductCreateDlg, IDC_BATCH_STATE, -600 /* Click */, OnClickBatchState, VTS_NONE)
	ON_EVENT(CProductCreateDlg, IDC_PPID, -600 /* Click */, OnClickPpid, VTS_NONE)
	ON_EVENT(CProductCreateDlg, IDC_DEVIDE_ID, -600 /* Click */, OnClickDevideId, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CProductCreateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strWorkOrder = m_plnkProductPlan->m_strWorkOrderNext;
	m_strDeviceID = m_plnkProductPlan->m_strDeviceIDNext;
	m_strInitProductPlanCnt = m_plnkProductPlan->m_strInitProductPlanCntNext;
	m_strLastProductPlanCnt = m_plnkProductPlan->m_strLastProductPlanCntNext;
	m_strBatchStateCnt = m_plnkProductPlan->m_strBatchStateCntNext;
	m_strPPID = m_plnkProductPlan->m_strPPIDNext;

	m_nBatchStateCnt = 0; //초기값..
	CString strBatch[3] = { _T("Create"), _T("Busy"), _T("End") };
	if(m_strBatchStateCnt == "")
		m_strBatchStateCnt.Format(_T("%s"), strBatch[m_nBatchStateCnt]);
		

	updateDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CProductCreateDlg::OnClickSave() 
{
	if (MyMessageBox("Do you want to save the data?\r\nData를 저장 하시겠습니까?", "Data Save", M_ICONQUESTION|M_YESNO, _T("YES"), _T("NO")) == IDNO)
		return;

	// TODO: Add your control notification handler code here
	m_plnkProductPlan->m_strWorkOrderNext = m_strWorkOrder;
	m_plnkProductPlan->m_strDeviceIDNext = m_strDeviceID;
	m_plnkProductPlan->m_strInitProductPlanCntNext = m_strInitProductPlanCnt;
	m_plnkProductPlan->m_strLastProductPlanCntNext = m_strLastProductPlanCnt;
	m_plnkProductPlan->m_strBatchStateCntNext = m_strBatchStateCnt;
	m_plnkProductPlan->m_nBatchStateCntNext = m_nBatchStateCnt;
	m_plnkProductPlan->m_strPPIDNext = m_strPPID;
	EndDialog(0);
}

void CProductCreateDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	EndDialog(1);
	CDialog::OnCancel();
}

void CProductCreateDlg::OnClickWorkOrder() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));

	swprintf(szBuf, _T(""));
	GetValueBox(szTemp, 20, szBuf, _T("Input Work Order"));

	m_strWorkOrder.Format(_T("%s"), szTemp);
	m_lblWorkOrder.SetCaption(szTemp);
	
}

void CProductCreateDlg::OnClickFirstProductOrder() 
{
	// TODO: Add your control notification handler code here
	WCHAR m_szNewValue[100];
	WCHAR m_szOldValue[100];
	int nTempValue;
	
	m_lblInitProductPlanCnt.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;
	nTempValue = (int)wcstod(m_szNewValue,NULL);
	if(nTempValue < 0 )
	{
		MyMessageBox("You can not enter a value less than 0.\r\n0 보다 작은 값은 입력 할수 없습니다.", "In Put Data Error", M_ICONINFORMATION|M_OK);		
		return;
	}
	swprintf(m_szNewValue, _T("%d"), nTempValue);
	m_strInitProductPlanCnt = m_szNewValue;
	m_lblInitProductPlanCnt.SetCaption(m_szNewValue);
}

void CProductCreateDlg::OnClickFinalProductOrder() 
{
	// TODO: Add your control notification handler code here
	WCHAR m_szNewValue[100];
	WCHAR m_szOldValue[100];
	int nTempValue;
	
	m_lblLastProductPlanCnt.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;
	nTempValue = (int)wcstod(m_szNewValue,NULL);

	if(nTempValue < 0 )
	{
		MyMessageBox("You can not enter a value less than 0.\r\n0 보다 작은 값은 입력 할수 없습니다.", "In Put Data Error", M_ICONINFORMATION|M_OK);		
		return;
	}
	swprintf(m_szNewValue, _T("%d"), nTempValue);
	m_strLastProductPlanCnt = m_szNewValue;
	m_lblLastProductPlanCnt.SetCaption(m_szNewValue);
	
}

void CProductCreateDlg::OnClickBatchState() 
{
	// TODO: Add your control notification handler code here
	WCHAR m_szNewValue[100];
	WCHAR m_szOldValue[100];
	int nTempValue;

	CString strBatch[3] = { _T("Create"), _T("Busy"), _T("End") };
	MyMessageBox("Please select Batch State\r\nBatch State를 선택해 주십시오\r\n[0]: Create, [1]: Busy, [2]: End", "Select Batch State", M_ICONINFORMATION|M_OK);
	
	m_lblBatchStateCnt.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;
	nTempValue = (int)wcstod(m_szNewValue,NULL);

	if(nTempValue < 0 || nTempValue > 2 )
	{
		MyMessageBox("Please enter a value between 0 and 2\r\n0 ~ 2 사이의 값을 입력해 주세요", "In Put Data Error",  M_ICONINFORMATION|M_OK);		
		return;
	}
	m_nBatchStateCnt = nTempValue;
	m_strBatchStateCnt = strBatch[nTempValue];
	m_lblBatchStateCnt.SetCaption(m_strBatchStateCnt);
	
}

void CProductCreateDlg::OnClickPpid() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));

	swprintf(szBuf, _T(""));
	GetValueBox(szTemp, 20, szBuf, _T("Input PPID"));

	m_strPPID.Format(_T("%s"), szTemp);
	m_lblPPID.SetCaption(szTemp);
	
}

void CProductCreateDlg::updateDisplay()
{
	m_lblWorkOrder.SetCaption(m_strWorkOrder);
	m_lblDeviceID.SetCaption(m_strDeviceID);
	m_lblInitProductPlanCnt.SetCaption(m_strInitProductPlanCnt);
	m_lblLastProductPlanCnt.SetCaption(m_strLastProductPlanCnt);
	m_lblBatchStateCnt.SetCaption(m_strBatchStateCnt);
	m_lblPPID.SetCaption(m_strPPID);

	CString strTemp;
	ReadDisplayMessage(MAIN_AUTO, _T("ProductCreateDlg_1"), &strTemp);
	GetDlgItem(IDC_FIRST_PRODUCT_ORDER)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductCreateDlg_2"), &strTemp);
	GetDlgItem(IDC_FINAL_PRODUCT_ORDER)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductCreateDlg_3"), &strTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductCreateDlg_4"), &strTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(strTemp);
	
	
	
}

void CProductCreateDlg::OnClickDevideId() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));

	swprintf(szBuf, _T(""));
	GetValueBox(szTemp, 20, szBuf, _T("Input Device ID"));

	m_strDeviceID.Format(_T("%s"), szTemp);
	m_lblDeviceID.SetCaption(szTemp);	
}
