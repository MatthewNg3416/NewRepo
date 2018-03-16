// LogMainForm.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "LogMainForm.h"
#include "LogConfigDlg.h"
#include "LogFileSelectDlg.h"
#include "MPlatformOlbSystem.h"
#include "MProductInfoData.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CLogMainForm

IMPLEMENT_DYNCREATE(CLogMainForm, CFormView)

CLogMainForm::CLogMainForm()
	: CFormView(CLogMainForm::IDD)
{
	//{{AFX_DATA_INIT(CLogMainForm)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	//m_ListControl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT, CRect(0, 0, 300, 100), this, IDC_LIST_VIEW);

	m_iCurrentLogType = -1;
}

CLogMainForm::~CLogMainForm()
{
}

void CLogMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogMainForm)
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_MFCSHELLLIST, m_ListControl);
	DDX_Control(pDX, IDC_LIST_VIEW, m_ListControl);
}


BEGIN_MESSAGE_MAP(CLogMainForm, CFormView)
	//{{AFX_MSG_MAP(CLogMainForm)
	ON_MESSAGE(WM_REFRESH_LOG_MAIN_WINDOW, OnRefreshLogMainWindow)
	// ML_hongju
	ON_MESSAGE(WM_DISP_MULTI_LANGUAGE_DLG_REQ, OnDisplayLanguage)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_VIEW, &CLogMainForm::OnLvnItemchangedListView)
//ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_VIEW, &CLogMainForm::OnLvnBegindragListView)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogMainForm diagnostics

#ifdef _DEBUG
void CLogMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CLogMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogMainForm message handlers

void CLogMainForm::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);

	// Do not call CFormView::OnPaint() for painting messages
}

BEGIN_EVENTSINK_MAP(CLogMainForm, CFormView)
	//{{AFX_EVENTSINK_MAP(CLogMainForm)
	ON_EVENT(CLogMainForm, IDB_LOG_SETTING, -600 /* Click */, OnLogSetting, VTS_NONE)
	ON_EVENT(CLogMainForm, IDB_TACTTIME_LOG, -600 /* Click */, OnTacttimeLog, VTS_NONE)
	ON_EVENT(CLogMainForm, IDB_ERROR_LOG, -600 /* Click */, OnErrorLog, VTS_NONE)
	ON_EVENT(CLogMainForm, IDB_NORMAL_LOG, -600 /* Click */, OnNormalLog, VTS_NONE)
	ON_EVENT(CLogMainForm, IDB_LINE_TACT_LOG, -600 /* Click */, OnClickLineTactLog, VTS_NONE)
	ON_EVENT(CLogMainForm, IDB_PAGE_UP, -600 /* Click */, OnClickPageUp, VTS_NONE)
	ON_EVENT(CLogMainForm, IDB_PAGE_DOWN, -600 /* Click */, OnClickPageDown, VTS_NONE)
	ON_EVENT(CLogMainForm, IDB_MACHINE_TACT_LOG, -600 /* Click */, OnClickMachineTactLog, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CLogMainForm::OnLogSetting()
{
	// TODO: Add your control notification handler code here
	InitializeButton();

	CLogConfigDlg dlg(this);
	dlg.DoModal();

}

void CLogMainForm::OnTacttimeLog()
{
	return;
}

void CLogMainForm::OnErrorLog()
{
	InitializeFlexGrid(DEF_ERROR_LOG);

	int iErrorCode[DEF_MAX_ERROR_LIST_COUNT] = { 0, };
	CString strErrorTime[DEF_MAX_ERROR_LIST_COUNT] = { _T(""), };
	CString strErrorMsg[DEF_MAX_ERROR_LIST_COUNT] = { _T(""), };
	int iCount = MLanguageManager::ReadErrorListMessage(&iErrorCode, &strErrorTime, &strErrorMsg);
	for (int i = 0; i < iCount; i++)
	{ 
		CString strNo, strErrorCode;
		strNo.Format(_T("%d"), i + 1);
		strErrorCode.Format(_T("%d"), iErrorCode[i]);
		m_ListControl.InsertItem(i, strNo);

		CString strMessage = MLanguageManager::ReadErrorMessage(iErrorCode[i], eLANGUAGE_SELECTED, TRUE);
		m_ListControl.SetItemText(i, 1, strErrorTime[i]);
		m_ListControl.SetItemText(i, 2, strErrorCode);
		m_ListControl.SetItemText(i, 3, strMessage);
	}

	return;
}

// syr..Log: Display Table한다
void CLogMainForm::displayTable()
{

}
//syr..Log
void CLogMainForm::redrawRowColor()
{
}


//syr..Log
void CLogMainForm::OnClickPageUp()
{
	//	int iNowPosition = ;

	CRect rect;
	m_ListControl.GetItemRect(0, &rect, LVIR_BOUNDS);
	CSize size = CSize(m_ListControl.GetScrollPos(SB_HORZ), m_ListControl.GetScrollPos(SB_VERT) - (rect.Height() * 10.0f));
	m_ListControl.Scroll(size);

}
void CLogMainForm::OnClickPageDown()
{
	CRect rect;
	m_ListControl.GetItemRect(0, &rect, LVIR_BOUNDS);
	CSize size = CSize(m_ListControl.GetScrollPos(SB_HORZ), m_ListControl.GetScrollPos(SB_VERT) + (rect.Height() * 10.0f));
	m_ListControl.Scroll(size);
}


void CLogMainForm::OnNormalLog()
{
	CLogFileSelectDlg dlg(this);
	m_iObjectID = dlg.DoModal();
}

void CLogMainForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetLogViewHandle(m_hWnd);

	InitializeFlexGrid(DEF_ERROR_LOG);
	InitializeButton();

	m_bFormInitialized = TRUE;
	m_iLogViewItems = 50;

	this->SendMessage(WM_DISP_MULTI_LANGUAGE_DLG_REQ);
}

void CLogMainForm::InitializeFlexGrid(int iLogType)
{
	m_iCurrentLogType = iLogType;

	//여기서 모든 데이터를 한번 지운다.
	m_ListControl.DeleteAllItems();
	int iColumnCount = m_ListControl.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < iColumnCount; i++)
		m_ListControl.DeleteColumn(0);
	//__________________________________

	int iheight = 40; //세로크기
	switch (iLogType)
	{
	case DEF_ERROR_LOG:
		iheight = 40;
		m_ListControl.InsertColumn(0, _T("NO"), LVCFMT_CENTER, 40);
		m_ListControl.InsertColumn(1, _T("Time"), LVCFMT_CENTER, 120);
		m_ListControl.InsertColumn(2, _T("Code"), LVCFMT_CENTER, 80);
		m_ListControl.InsertColumn(3, _T("Text Info"), LVCFMT_LEFT, 710);
		break;

	case DEF_MACHINE_TACT_LOG:
	case DEF_LINE_TACT_LOG:
		iheight = 20;
		m_ListControl.InsertColumn(0, _T("NO"), LVCFMT_CENTER, 50);
		m_ListControl.InsertColumn(1, _T("Panel ID"), LVCFMT_CENTER, 200);
		m_ListControl.InsertColumn(2, _T("Line Tact"), LVCFMT_CENTER, 100);
		m_ListControl.InsertColumn(3, _T("Machine Tact"), LVCFMT_CENTER, 100);
		m_ListControl.InsertColumn(4, _T("Pane In Time"), LVCFMT_CENTER, 150);
		m_ListControl.InsertColumn(5, _T("Panel Out Time"), LVCFMT_CENTER, 150);
		break;

	default:
		break;
	}


	CImageList m_imgList;

	m_imgList.Create(1, iheight, ILC_COLOR4, 1, 1);
	m_ListControl.SetImageList(&m_imgList, LVSIL_SMALL);
	m_ListControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

void CLogMainForm::InitializeButton()
{
}

void CLogMainForm::InitializeLabel()
{
}

void CLogMainForm::SetCurrentObjectID(int iObjectID)
{
	m_iObjectID = iObjectID;
}

void CLogMainForm::SetLogConfig(BYTE ucLevel, int iKeepingDays, int iViewItems)
{
	m_ucLevel = ucLevel;
	m_iLogKeepingDays = iKeepingDays;
	m_iLogViewItems = iViewItems;

	MOlbSystemPre.GetSystemInfo()->SetAllObjsLogAttribute(m_ucLevel, m_iLogKeepingDays);
}

LRESULT CLogMainForm::OnRefreshLogMainWindow(UINT wParam, LPARAM lParam)
{
	if (m_iCurrentLogType != static_cast<int>(wParam))
		return 0;

	switch (m_iCurrentLogType)
	{
	case DEF_ERROR_LOG:
		OnErrorLog();
		break;
	case DEF_LINE_TACT_LOG:
		OnClickLineTactLog();
		break;
	case DEF_MACHINE_TACT_LOG:
		OnClickMachineTactLog();
		break;
	default:
		break;
	}
	return 0;
}

void CLogMainForm::OnClickLineTactLog()
{
	ChangeLogTable(DEF_LINE_TACT_LOG);
}

LRESULT CLogMainForm::OnDisplayLanguage(UINT wParam, LPARAM lParam)
{
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	return 0;
}





void CLogMainForm::OnClickMachineTactLog()
{
	// TODO: Add your control notification handler code here
	ChangeLogTable(DEF_MACHINE_TACT_LOG);
}


//170329_KDH TactTime 열람 함수
void CLogMainForm::ChangeLogTable(int iLogType)
{
	ASSERT(iLogType == DEF_LINE_TACT_LOG || iLogType == DEF_MACHINE_TACT_LOG);
	InitializeFlexGrid(iLogType);

	MTrsAutoManager* plnkTrsAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
	MProductInfoData* plnkProductInfo = MOlbSystemPre.GetProductInfoData();

	int index = plnkProductInfo->GetCurrentQueueIndex();
	for (int i = 0; i < index; i++)
	{
		CString strNo;
		strNo.Format(_T("%d"), i + 1);
		m_ListControl.InsertItem(i, strNo); 

		SPanelInfo* psPanelInfo = plnkProductInfo->GetPanelInfo(i);
		double dLineTact = psPanelInfo->m_dLineTact;
		double dMachineTact = psPanelInfo->m_dMachineTact;

		if (dLineTact != 0.0 || dMachineTact != 0.0)
		{
			CString strPanelID = psPanelInfo->m_strPanelID;
			CString strLineTact, strMachineTact;
			strLineTact.Format(_T("%.3f sec"), dLineTact);
			strMachineTact.Format(_T("%.3f sec"), dMachineTact);
			m_ListControl.SetItemText(i, 1, strPanelID);
			m_ListControl.SetItemText(i, 2, strLineTact);
			m_ListControl.SetItemText(i, 3, strMachineTact);
			m_ListControl.SetItemText(i, 4, psPanelInfo->m_timePanelIn.Format(_T("[%m/%d-%H:%M:%S]")));
			m_ListControl.SetItemText(i, 5, psPanelInfo->m_timePanelOut.Format(_T("[%m/%d-%H:%M:%S]")));
			
		}
	}
}
//___________________


//void CLogMainForm::OnLvnItemchangedListView(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//}


//void CLogMainForm::OnLvnBegindragListView(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//	m_bDrag = TRUE;
//}
