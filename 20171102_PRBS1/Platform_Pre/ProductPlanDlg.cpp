// ProductPlanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "ProductPlanDlg.h"
#include "MPlatformOlbSystem.h"

#include "MLCNet.h"
#include "MLCNetData.h"
#include "MTrsAutoManager.h"
#include "MProductInfoData.h"
#include "PanelValidationCheckDlg.h"
#include "MSystemData.h"

#include "ProductCreateDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProductPlanDlg dialog
extern MPlatformOlbSystem MOlbSystemPre;

CProductPlanDlg::CProductPlanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductPlanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProductPlanDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_plnkLCNet = MOlbSystemPre.GetLCNetComponent();
	m_lnkProductInfoData = MOlbSystemPre.GetProductInfoData();
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CProductPlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProductPlanDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	//___________________________________________________________
	DDX_Control(pDX, IDC_NEXT_1,m_lblNext[0]);
	DDX_Control(pDX, IDC_NEXT_2,m_lblNext[1]);
	DDX_Control(pDX, IDC_NEXT_3,m_lblNext[2]);
	DDX_Control(pDX, IDC_NEXT_4,m_lblNext[3]);
	DDX_Control(pDX, IDC_NEXT_5,m_lblNext[4]);

	DDX_Control(pDX, IDS_WORK_ORDER_1, m_lblWorkOrderCur);
	DDX_Control(pDX, IDS_DEVIDE_ID_1, m_lblDeviceIDCur);
	DDX_Control(pDX, IDS_FIRST_PRODUCT_ORDER_1, m_lblInitProductPlanCntCur);
	DDX_Control(pDX, IDS_FINAL_PRODUCT_ORDER_1, m_lblLastProductPlanCntCur);
	DDX_Control(pDX, IDS_BATCH_STATE_1, m_lblBatchStateCntCur);
	DDX_Control(pDX, IDS_PPID_1, m_lblPPIDCur);

	//___________________________________________________________
	DDX_Control(pDX, IDS_WORK_ORDER_2, m_lblWorkOrderNext[0]);
	DDX_Control(pDX, IDS_DEVIDE_ID_2, m_lblDeviceIDNext[0]);
	DDX_Control(pDX, IDS_FIRST_PRODUCT_ORDER_2, m_lblInitProductPlanCntNext[0]);
	DDX_Control(pDX, IDS_PPID_2, m_lblPPIDNext[0]);
	DDX_Control(pDX, IDS_FINAL_PRODUCT_ORDER_2, m_lblLastProductPlanCntNext[0]);
	DDX_Control(pDX, IDS_BATCH_STATE_2, m_lblBatchStateCntNext[0]);

	DDX_Control(pDX, IDS_WORK_ORDER_3, m_lblWorkOrderNext[1]);
	DDX_Control(pDX, IDS_DEVIDE_ID_3, m_lblDeviceIDNext[1]);
	DDX_Control(pDX, IDS_FIRST_PRODUCT_ORDER_3, m_lblInitProductPlanCntNext[1]);
	DDX_Control(pDX, IDS_PPID_3, m_lblPPIDNext[1]);
	DDX_Control(pDX, IDS_FINAL_PRODUCT_ORDER_3, m_lblLastProductPlanCntNext[1]);
	DDX_Control(pDX, IDS_BATCH_STATE_3, m_lblBatchStateCntNext[1]);
	

	DDX_Control(pDX, IDS_WORK_ORDER_4, m_lblWorkOrderNext[2]);
	DDX_Control(pDX, IDS_DEVIDE_ID_4, m_lblDeviceIDNext[2]);
	DDX_Control(pDX, IDS_FIRST_PRODUCT_ORDER_4, m_lblInitProductPlanCntNext[2]);
	DDX_Control(pDX, IDS_PPID_4, m_lblPPIDNext[2]);
	DDX_Control(pDX, IDS_FINAL_PRODUCT_ORDER_4, m_lblLastProductPlanCntNext[2]);
	DDX_Control(pDX, IDS_BATCH_STATE_4, m_lblBatchStateCntNext[2]);

	DDX_Control(pDX, IDS_WORK_ORDER_5, m_lblWorkOrderNext[3]);
	DDX_Control(pDX, IDS_DEVIDE_ID_5, m_lblDeviceIDNext[3]);
	DDX_Control(pDX, IDS_FIRST_PRODUCT_ORDER_5, m_lblInitProductPlanCntNext[3]);
	DDX_Control(pDX, IDS_PPID_5, m_lblPPIDNext[3]);
	DDX_Control(pDX, IDS_FINAL_PRODUCT_ORDER_5, m_lblLastProductPlanCntNext[3]);
	DDX_Control(pDX, IDS_BATCH_STATE_5, m_lblBatchStateCntNext[3]);

	DDX_Control(pDX, IDS_WORK_ORDER_6, m_lblWorkOrderNext[4]);
	DDX_Control(pDX, IDS_DEVIDE_ID_6, m_lblDeviceIDNext[4]);
	DDX_Control(pDX, IDS_FIRST_PRODUCT_ORDER_6, m_lblInitProductPlanCntNext[4]);
	DDX_Control(pDX, IDS_PPID_6, m_lblPPIDNext[4]);
	DDX_Control(pDX, IDS_FINAL_PRODUCT_ORDER_6, m_lblLastProductPlanCntNext[4]);
	DDX_Control(pDX, IDS_BATCH_STATE_6, m_lblBatchStateCntNext[4]);

	//___________________________________________________________
	DDX_Control(pDX, IDS_COMPLETE_WORK_ORDER_1, m_lblWorkOrderEnd[0]);
	DDX_Control(pDX, IDS_COMPLETE_DEVICE_ID_1, m_lblDeviceIDEnd[0]);
	DDX_Control(pDX, IDS_COMPLETE_FINAL_PRODUCT_ORDER_1, m_lblLastProductPlanCntEnd[0]);
	DDX_Control(pDX, IDS_COMPLETE_PPID_1, m_lblPPIDEnd[0]);

	DDX_Control(pDX, IDS_COMPLETE_WORK_ORDER_2, m_lblWorkOrderEnd[1]);
	DDX_Control(pDX, IDS_COMPLETE_DEVICE_ID_2, m_lblDeviceIDEnd[1]);
	DDX_Control(pDX, IDS_COMPLETE_FINAL_PRODUCT_ORDER_2, m_lblLastProductPlanCntEnd[1]);
	DDX_Control(pDX, IDS_COMPLETE_PPID_2, m_lblPPIDEnd[1]);

	DDX_Control(pDX, IDS_COMPLETE_WORK_ORDER_3, m_lblWorkOrderEnd[2]);
	DDX_Control(pDX, IDS_COMPLETE_DEVICE_ID_3, m_lblDeviceIDEnd[2]);
	DDX_Control(pDX, IDS_COMPLETE_FINAL_PRODUCT_ORDER_3, m_lblLastProductPlanCntEnd[2]);
	DDX_Control(pDX, IDS_COMPLETE_PPID_3, m_lblPPIDEnd[2]);

	DDX_Control(pDX, IDS_COMPLETE_WORK_ORDER_4, m_lblWorkOrderEnd[3]);
	DDX_Control(pDX, IDS_COMPLETE_DEVICE_ID_4, m_lblDeviceIDEnd[3]);
	DDX_Control(pDX, IDS_COMPLETE_FINAL_PRODUCT_ORDER_4, m_lblLastProductPlanCntEnd[3]);
	DDX_Control(pDX, IDS_COMPLETE_PPID_4, m_lblPPIDEnd[3]);

	DDX_Control(pDX, IDS_COMPLETE_WORK_ORDER_5, m_lblWorkOrderEnd[4]);
	DDX_Control(pDX, IDS_COMPLETE_DEVICE_ID_5, m_lblDeviceIDEnd[4]);
	DDX_Control(pDX, IDS_COMPLETE_FINAL_PRODUCT_ORDER_5, m_lblLastProductPlanCntEnd[4]);
	DDX_Control(pDX, IDS_COMPLETE_PPID_5, m_lblPPIDEnd[4]);
}


BEGIN_MESSAGE_MAP(CProductPlanDlg, CDialog)
	//{{AFX_MSG_MAP(CProductPlanDlg)
//	ON_MESSAGE(WM_DISP_PRODUCT_PLAN_MSG, OnDisplayProductPlan)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProductPlanDlg message handlers

BEGIN_EVENTSINK_MAP(CProductPlanDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CProductPlanDlg)
	ON_EVENT(CProductPlanDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDB_JOB_PLAN_REQUEST, -600 /* Click */, OnClickJobPlanRequest, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDB_JOB_PLAN_REGIST, -600 /* Click */, OnClickJobPlanRegist, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDB_JOB_PLAN_REMOVE, -600 /* Click */, OnClickJobPlanRemove, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDB_JOB_PLAN_MODIFY, -600 /* Click */, OnClickJobPlanModify, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDB_VALIDATION_CHECK_ITEM, -600 /* Click */, OnClickValidationCheckItem, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDC_NEXT_1, -600 /* Click */, OnClickNext1, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDC_NEXT_2, -600 /* Click */, OnClickNext2, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDC_NEXT_3, -600 /* Click */, OnClickNext3, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDC_NEXT_4, -600 /* Click */, OnClickNext4, VTS_NONE)
	ON_EVENT(CProductPlanDlg, IDC_NEXT_5, -600 /* Click */, OnClickNext5, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
	// Standard printing commands
END_EVENTSINK_MAP()



BOOL CProductPlanDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	clearDisplay();
	updateDisplay();
	m_uiTimerID = SetTimer(1, 200, NULL);

	m_lNextButtonRGB = m_lblNext[0].GetBackColor();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CProductPlanDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CProductPlanDlg::OnClickJobPlanRequest() 
{
	// TODO: Add your control notification handler code here
	MTrsAutoManager* plnkAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();

	MPlanRequestData *pPR;

	pPR = new MPlanRequestData;

	pPR->m_nM_KIND = 0 ;

	//110705.kms___________
	pPR->m_sEQPID.Format(m_plnkSystemData->m_strEQPID);
	//_____________________

	pPR->m_sLINE_NO.Format(_T("LINE_NO"));

	pPR->m_sCarrierID.Format(_T(" "));
	pPR->m_sDeviceID.Format(_T(" "));	
	pPR->m_sM_STEP.Format(_T(" "));

	m_plnkLCNet->ProductPlanRequest(eProductPlanRequest, pPR);

	updateDisplay();

	delete pPR;
}

void CProductPlanDlg::OnClickJobPlanRegist() 
{
	// TODO: Add your control notification handler code here
	UpdateCreatePlan(0);	
}

void CProductPlanDlg::OnClickJobPlanRemove() 
{
	// TODO: Add your control notification handler code here
	int SelectedPlanNum = -1;
	for(int i=0; i<5; i++)
	{
		if(m_bSelNextPlan[i] == TRUE)
		{
			SelectedPlanNum = i;
			break;
		}
	}

	if(SelectedPlanNum == -1)
	{
		MyMessageBox("No items selected.\r\n선택된 항목이 없습니다.", "Product Create Error",  M_ICONINFORMATION|M_OK);
		return;
	}

	if(m_lblWorkOrderNext[SelectedPlanNum].GetCaption() == _T("")
		&& m_lblDeviceIDNext[SelectedPlanNum].GetCaption() == _T("")
		&& m_lblInitProductPlanCntNext[SelectedPlanNum].GetCaption() == _T("")
		&& m_lblLastProductPlanCntNext[SelectedPlanNum].GetCaption() == _T("")
		&& m_lblBatchStateCntNext[SelectedPlanNum].GetCaption() == _T("")
		&& m_lblPPIDNext[SelectedPlanNum].GetCaption() == _T(""))
	{
		MyMessageBox("The selected item has no data to delete.\r\n선택한 항목은 삭제할 Data가 없습니다.", "Product Delete Error",  M_ICONINFORMATION|M_OK);
		return;
	}

	if( SelectedPlanNum != -1)
	{
		m_lblWorkOrderNext[SelectedPlanNum].SetCaption(_T(""));
		m_lblDeviceIDNext[SelectedPlanNum].SetCaption(_T(""));
		m_lblInitProductPlanCntNext[SelectedPlanNum].SetCaption(_T(""));	
		m_lblLastProductPlanCntNext[SelectedPlanNum].SetCaption(_T(""));
		m_lblBatchStateCntNext[SelectedPlanNum].SetCaption(_T(""));
		m_lblPPIDNext[SelectedPlanNum].SetCaption(_T(""));
		m_plnkLCNet->m_caNextProductPlan.RemoveAt(SelectedPlanNum);
	}
	clearDisplay();
	updateDisplay();
}

void CProductPlanDlg::OnClickJobPlanModify() 
{
	// TODO: Add your control notification handler code here
	UpdateCreatePlan(1)	;
}

void CProductPlanDlg::OnClickValidationCheckItem() 
{
	// TODO: Add your control notification handler code here
	CPanelValidationCheckDlg Dlg;
	Dlg.DoModal();
}

void CProductPlanDlg::OnDisplayProductPlan(UINT iPlanCode)
{
	clearDisplay();
	updateDisplay();
}


void CProductPlanDlg::clearDisplay()
{
	int i = 0;
	m_lblWorkOrderCur.SetCaption(_T(""));
	m_lblDeviceIDCur.SetCaption(_T(""));
	m_lblInitProductPlanCntCur.SetCaption(_T(""));
	m_lblLastProductPlanCntCur.SetCaption(_T(""));
	m_lblBatchStateCntCur.SetCaption(_T(""));
	m_lblPPIDCur.SetCaption(_T(""));

	for (i=0; i<DEF_MAX_PRODUCT_PLAN_CNT; i++)
	{
		m_lblWorkOrderNext[i].SetCaption(_T(""));
		m_lblDeviceIDNext[i].SetCaption(_T(""));
		m_lblInitProductPlanCntNext[i].SetCaption(_T(""));
		m_lblInitProductPlanCntNext[i].SetCaption(_T(""));
		m_lblPPIDNext[i].SetCaption(_T(""));
		m_lblLastProductPlanCntNext[i].SetCaption(_T(""));
		m_lblBatchStateCntNext[i].SetCaption(_T(""));
	}

	for (i=0; i<DEF_MAX_PRODUCT_PLAN_CNT; i++)
	{
		m_lblWorkOrderEnd[i].SetCaption(_T(""));
		m_lblDeviceIDEnd[i].SetCaption(_T(""));
		m_lblLastProductPlanCntEnd[i].SetCaption(_T(""));
		m_lblPPIDEnd[i].SetCaption(_T(""));
	}
}

void CProductPlanDlg::updateDisplay()
{
	int i = 0;
	CString strCaption;
	CString strBatch[3] = { _T("Create"), _T("Busy"), _T("End") };

	OnDisplayLanguage();
	
	m_lblWorkOrderCur.SetCaption(m_plnkLCNet->m_CurProductPlan.m_sBatchID);
	m_lblDeviceIDCur.SetCaption(m_plnkLCNet->m_CurProductPlan.m_sDeviceID);

	strCaption.Format(_T("%d"), m_plnkLCNet->m_CurProductPlan.m_nBatchSize);
	m_lblInitProductPlanCntCur.SetCaption(strCaption);

	strCaption.Format(_T("%d"), m_plnkLCNet->m_CurProductPlan.m_nBatchSizeLast);
	m_lblLastProductPlanCntCur.SetCaption(strCaption);
	
	int iState = m_plnkLCNet->m_CurProductPlan.m_nBatchState;
	if (iState < 3)
		m_lblBatchStateCntCur.SetCaption(strBatch[iState]);

	m_lblPPIDCur.SetCaption(m_plnkLCNet->m_CurProductPlan.m_sPPID);	

	int iCnt = m_plnkLCNet->m_caNextProductPlan.GetSize();
	for (i=0; i<iCnt; i++)
	{
		MProductPlanBatchData* p = (MProductPlanBatchData*)m_plnkLCNet->m_caNextProductPlan.GetAt(i);

		m_lblWorkOrderNext[i].SetCaption(p->m_sBatchID);
		m_lblDeviceIDNext[i].SetCaption(p->m_sDeviceID);
		strCaption.Format(_T("%d"), p->m_nBatchSize);
		m_lblInitProductPlanCntNext[i].SetCaption(strCaption);
		m_lblPPIDNext[i].SetCaption(p->m_sPPID);
		
		strCaption.Format(_T("%d"), p->m_nBatchSizeLast);
		m_lblLastProductPlanCntNext[i].SetCaption(strCaption);
		int iNextState = p->m_nBatchState;
		if (iNextState < 3)
			m_lblBatchStateCntNext[i].SetCaption(strBatch[iNextState]);
	}

	iCnt = m_plnkLCNet->m_caEndProductPlan.GetSize();
	for (i=0; i<iCnt; i++)
	{
		MProductPlanBatchData* p = (MProductPlanBatchData*)m_plnkLCNet->m_caEndProductPlan.GetAt(i);
		m_lblWorkOrderEnd[i].SetCaption(p->m_sBatchID);
		m_lblDeviceIDEnd[i].SetCaption(p->m_sDeviceID);
		strCaption.Format(_T("%d"), p->m_nBatchSizeLast);
		m_lblLastProductPlanCntEnd[i].SetCaption(strCaption);
		m_lblPPIDEnd[i].SetCaption(p->m_sPPID);
	}
}

void CProductPlanDlg::OnClickNext1() 
{
	// TODO: Add your control notification handler code here	
	UpdateSelectPlan(0);
}

void CProductPlanDlg::OnClickNext2() 
{
	// TODO: Add your control notification handler code here
	UpdateSelectPlan(1);
}

void CProductPlanDlg::OnClickNext3() 
{
	// TODO: Add your control notification handler code here
	UpdateSelectPlan(2);
}

void CProductPlanDlg::OnClickNext4() 
{
	// TODO: Add your control notification handler code here
	UpdateSelectPlan(3);
}

void CProductPlanDlg::OnClickNext5() 
{
	// TODO: Add your control notification handler code here
	UpdateSelectPlan(4);
}
void CProductPlanDlg::UpdateSelectPlan(int sel)
{
	for(int i=0; i<5; i++)
	{
		if(i == sel)
			m_bSelNextPlan[i] = TRUE;
		else
			m_bSelNextPlan[i] = FALSE;
	}
	UpdateSelectButton(sel);
}
void CProductPlanDlg::UpdateSelectButton(int sel)
{
	for(int i=0; i<5 ; i++)
	{
		if(i == sel)
			m_lblNext[i].SetBackColor(YELLOW);
		else
			m_lblNext[i].SetBackColor(m_lNextButtonRGB);
	}
}

void CProductPlanDlg::UpdateCreatePlan(int sel)
{
	int i;
	for( i=0; i<5; i++)
	{
		if(m_bSelNextPlan[i] == TRUE)
			break;
	}
	
	if( i == 5)
	{
		MyMessageBox("No items selected.\r\n선택된 항목이 없습니다.", "Product Create Error",  M_ICONINFORMATION|M_OK);
		return;
	}

	m_strWorkOrderNext = m_lblWorkOrderNext[i].GetCaption();
	m_strDeviceIDNext = m_lblDeviceIDNext[i].GetCaption();
	m_strInitProductPlanCntNext = m_lblInitProductPlanCntNext[i].GetCaption();
	m_strLastProductPlanCntNext = m_lblLastProductPlanCntNext[i].GetCaption();
	m_strBatchStateCntNext = m_lblBatchStateCntNext[i].GetCaption();
	m_strPPIDNext = m_lblPPIDNext[i].GetCaption();

	if(sel == 0)//생성할 경우
	{
		if(m_lblWorkOrderNext[i].GetCaption() != _T("")
			|| m_lblDeviceIDNext[i].GetCaption() != _T("")
			|| m_lblInitProductPlanCntNext[i].GetCaption() != _T("")
			|| m_lblLastProductPlanCntNext[i].GetCaption() != _T("")
			|| m_lblBatchStateCntNext[i].GetCaption() != _T("")
			|| m_lblPPIDNext[i].GetCaption() != _T(""))
		{
			MyMessageBox("The selected item already has data and can not be newly created.\r\n선택한 항목은 이미 Data를 가지고 있어 새로 생성 할수 없습니다.", "Product Create Error",  M_ICONINFORMATION|M_OK);
			return;
		}
	}
	else
	{
		if(m_lblWorkOrderNext[i].GetCaption() == _T("")
			&& m_lblDeviceIDNext[i].GetCaption() == _T("")
			&& m_lblInitProductPlanCntNext[i].GetCaption() == _T("")
			&& m_lblLastProductPlanCntNext[i].GetCaption() == _T("")
			&& m_lblBatchStateCntNext[i].GetCaption() == _T("")
			&& m_lblPPIDNext[i].GetCaption() == _T(""))
		{
			MyMessageBox("The selected item has no data to delete.\r\n선택한 항목에 수정할 Data가 없습니다.", "Product Create Error",  M_ICONINFORMATION|M_OK);
			return;
		}
	}
	
	BOOL flgOnClickExit;
	CProductCreateDlg dlgProductCreate(sel,this,i);
	flgOnClickExit = dlgProductCreate.DoModal();

	//CProductCreateDlg 에서 Exit를 선택한 경우 아무 작업도 하지 않는다.
	if(flgOnClickExit)
		return;

	//Update
	m_lblWorkOrderNext[i].SetCaption(m_strWorkOrderNext);
	m_lblDeviceIDNext[i].SetCaption(m_strDeviceIDNext);
	m_lblInitProductPlanCntNext[i].SetCaption(m_strInitProductPlanCntNext);
	m_lblLastProductPlanCntNext[i].SetCaption(m_strLastProductPlanCntNext);
	m_lblBatchStateCntNext[i].SetCaption(m_strBatchStateCntNext);
	m_lblPPIDNext[i].SetCaption(m_strPPIDNext);

	if(sel ==0 )//생성일 경우
	{
		if( i == 0)
		{
			MProductPlanBatchData* pNEW;
			if(m_plnkLCNet->m_caNextProductPlan.GetSize())
			{
				pNEW = (MProductPlanBatchData*)m_plnkLCNet->m_caNextProductPlan.GetAt(i);
			}
			else 
			{
				pNEW = new MProductPlanBatchData;
			}
			
			pNEW->m_sBatchID = m_strWorkOrderNext;
			pNEW->m_sDeviceID = m_strDeviceIDNext;
			pNEW->m_nBatchSize = (int)wcstod(m_strInitProductPlanCntNext ,NULL);
			pNEW->m_nBatchSizeLast = (int)wcstod(m_strLastProductPlanCntNext ,NULL);
			pNEW->m_nBatchState = m_nBatchStateCntNext;
			pNEW->m_sPPID = m_strPPIDNext;
			
			m_plnkLCNet->m_caNextProductPlan.Add(pNEW);
			m_plnkLCNet->ManageNextProductPlanData();
			m_plnkLCNet->m_NextProductPlan.SetData((MProductPlanBatchData*)m_plnkLCNet->m_caNextProductPlan.GetAt(0));		
		}	
		if( i != 0)
		{
			MProductPlanBatchData* pNEW;
			pNEW = new MProductPlanBatchData;
			pNEW->m_sBatchID = m_strWorkOrderNext;
			pNEW->m_sDeviceID = m_strDeviceIDNext;
			pNEW->m_nBatchSize = (int)wcstod(m_strInitProductPlanCntNext ,NULL);
			pNEW->m_nBatchSizeLast = (int)wcstod(m_strLastProductPlanCntNext ,NULL);
			pNEW->m_nBatchState = m_nBatchStateCntNext;
			pNEW->m_sPPID = m_strPPIDNext;
			m_plnkLCNet->m_caNextProductPlan.Add(pNEW);
		}	
	}
	else//수정일 경우
	{
		MProductPlanBatchData* pOld = (MProductPlanBatchData*)m_plnkLCNet->m_caNextProductPlan.GetAt(i);
		
		pOld->m_sBatchID = m_strWorkOrderNext;
		pOld->m_sDeviceID = m_strDeviceIDNext;
		pOld->m_nBatchSize = (int)wcstod(m_strInitProductPlanCntNext ,NULL);
		pOld->m_nBatchSizeLast = (int)wcstod(m_strLastProductPlanCntNext ,NULL);
		pOld->m_nBatchState = m_nBatchStateCntNext;
		pOld->m_sPPID = m_strPPIDNext;
	
	}
	clearDisplay();
	updateDisplay();
}

void CProductPlanDlg::OnDisplayLanguage()
{
	CString strTemp;

	ReadDisplayMessage(MAIN_AUTO, _T("ProductPlanDlg_1"), &strTemp);
	GetDlgItem(IDC_FIRST_PRODUCT_ORDER)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductPlanDlg_2"), &strTemp);
	GetDlgItem(IDC_FINAL_PRODUCT_ORDER)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductPlanDlg_3"), &strTemp);
	GetDlgItem(IDB_JOB_PLAN_REQUEST)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductPlanDlg_4"), &strTemp);
	GetDlgItem(IDB_JOB_PLAN_REGIST)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductPlanDlg_5"), &strTemp);
	GetDlgItem(IDB_JOB_PLAN_REMOVE)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductPlanDlg_6"), &strTemp);
	GetDlgItem(IDB_JOB_PLAN_MODIFY)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductPlanDlg_2"), &strTemp);
	GetDlgItem(IDC_COMPLETE_FINAL_PRODUCT_ORDER)->SetWindowText(strTemp);

	ReadDisplayMessage(MAIN_AUTO, _T("ProductPlanDlg_7"), &strTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(strTemp);

}

void CProductPlanDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_uiTimerID)
	{
		updateDisplay();
	}

	CDialog::OnTimer(nIDEvent);
}
