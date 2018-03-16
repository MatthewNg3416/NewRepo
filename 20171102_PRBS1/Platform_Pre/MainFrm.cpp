// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MPlatformOlb.h"

#include "MainFrm.h"
#include "AutoMainForm.h"
#include "ManualMainForm.h"
#include "DataMainForm.h"
#include "TeachMainForm.h"
#include "LogMainForm.h"
#include "MNetHDlg.h"
#include "MNetH.h"
#include "TitleForm.h"
#include "BottomForm.h"
#include "MProductInfoData.h"
#include "MTrsAutoManager.h"
#include "MPlatformOlbSystem.h"
#include "MCtrlTabFeeder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int VIEW_AUTO		= 0;
const int VIEW_MANUAL	= 1;
const int VIEW_DATA		= 2;
const int VIEW_TEACHING = 3;

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator (Spare - 1)
	ID_SEPARATOR,           // status line indicator (Spare - 2)
	ID_SEPARATOR,           // status line indicator (Spare - 3)
	ID_SEPARATOR,           // status line indicator (Spare - 4)
	ID_SEPARATOR,           // status line indicator (Spare - 5)
	ID_SEPARATOR,           // status line indicator (Version 기록 - 6)
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
*/
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
//	ID_INDICATOR_CAPS,
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
	MNetHDlg** pNetHDlg = MOlbSystemPre.GetMelsecComponent()->GetMNetHDlg();
	if (*pNetHDlg != NULL)
	{
		(*pNetHDlg)->DestroyWindow();
		delete *pNetHDlg;
		*pNetHDlg = NULL;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	CString st = "      ";

	SetStatusText(0,st);	// 

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	// Row : 3, Column : 1
	if (!m_wndSplitter.CreateStatic(this, 3, 1))
	{
        TRACE0("Failed to Create splitter bar\n");
        return FALSE;
    }

    // 각각의 분할뷰에 대한 View Class를 지정하고, 사이즈를 정해준다.(각 View Class에 대한 헤더파일 필요^^)

	m_pTitleForm		= RUNTIME_CLASS(CTitleForm);
	m_pAutoMainForm		= RUNTIME_CLASS(CAutoMainForm);
	m_pManualMainForm	= RUNTIME_CLASS(CManualMainForm);
	m_pDataMainForm		= RUNTIME_CLASS(CDataMainForm);
	m_pTeachMainForm	= RUNTIME_CLASS(CTeachMainForm);
	m_pLogMainForm		= RUNTIME_CLASS(CLogMainForm);
	m_pBottomForm		= RUNTIME_CLASS(CBottomForm);

	
	m_wndSplitter.CreateView(0, 0, m_pTitleForm, CSize(100,100), pContext);

	
	m_nView[0] = m_wndSplitter.AddView(1, 0,	m_pAutoMainForm,	pContext);
	m_nView[1] = m_wndSplitter.AddView(1, 0,	m_pManualMainForm,	pContext);
	m_nView[2] = m_wndSplitter.AddView(1, 0,	m_pDataMainForm ,	pContext);
	m_nView[3] = m_wndSplitter.AddView(1, 0,	m_pTeachMainForm,	pContext);
	m_nView[4] = m_wndSplitter.AddView(1, 0,	m_pLogMainForm,		pContext);


	m_wndSplitter.CreateView(2, 0, m_pBottomForm, CSize(100,100), pContext);

	m_wndSplitter.SetRowInfo(0, 65, 0);
	m_wndSplitter.SetRowInfo(1, 580, 0);
	m_wndSplitter.SetRowInfo(2, 70, 0);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_VISIBLE | WS_POPUP | WS_BORDER;
	cs.x = 0;
	cs.y = 0;
	cs.cx= 1024;
	cs.cy= 768;

	/*
	*********************************
	**	Menu Bar를 지운다.
	*********************************
	*/
	CMenu* pMenu = new CMenu;
	pMenu->Attach(cs.hMenu);
	pMenu->DestroyMenu();
	cs.hMenu = NULL;
	delete pMenu;
	/**********************************/
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::SwitchMainViews(int viewNumber)
{
	m_wndSplitter.ShowView(viewNumber);
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
///	if (message == WM_MSGBOX_MSG)
///	{
		/** MyMessageBox 띄우고 그 결과를 wParam으로 전달한다. */
///		*(int *)(wParam) = ((CMyMessageBoxDlg *)(lParam))->DoModal();
///	}

	if (message == WM_SWITCH_VIEW)
	{
		m_wndSplitter.GetView(m_nView[0])->SendMessage(message);
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnAppExit() 
{
	// TODO: Add your command handler code here
	if (m_wndSplitter.GetCurrentViewID() == VIEW_AUTO + 1)
	{
		MTrsAutoManager* plnkAutoManager = MOlbSystemPre.GetTrsAutoManagerComponent();
		EOpMode eOpMode = plnkAutoManager->GetOPMode();
		if (AUTORUN_MODE == eOpMode)
			return;
		
		//운용 프로그램을 종료하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX, 61000, _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		//재확인 합니다. 프로그램을 종료하시겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX, 61001, _T("Confirm"), M_ICONQUESTION|M_YESNO) == IDNO)
			return;

		MSystemData* plnkSystemData = MOlbSystemPre.GetSystemData();
		if (REPAIR_RUN_MODE == plnkSystemData->m_eRunMode)
		{
			plnkSystemData->m_eRunMode = NORMAL_RUN_MODE;
			plnkSystemData->WriteData();
		}

		//SJ_YYK 110421 Add.. Ellas...
		MLCNet* plnkLCNet = MOlbSystemPre.GetLCNetComponent();
		plnkLCNet->EqSpecialStateReport(eProgramEnd);
		
		MProductInfoData* plnkProductData = MOlbSystemPre.GetProductInfoData();
		plnkProductData->WriteData();
		plnkProductData->WriteMaterialData();
		
		for (int i = 0; i < DEF_MAX_PANEL_UNIT; i++)
			plnkProductData->m_Glass[i] = *(MOlbSystemPre.GetTrsAutoManagerComponent()->m_pGlassData[i]);
		
		plnkProductData->WriteGlassData();

		//eMatNG 12.08.14 yh
		//Feeder1
		MCtrlTabFeeder* m_plnkCtrlTabFeeder1;
		m_plnkCtrlTabFeeder1 = MOlbSystemPre.GetCtrlTabFeederComponent(0);

		int iTemp1= m_plnkCtrlTabFeeder1->m_iNG_CurTab - m_plnkCtrlTabFeeder1->m_iNG_OldTab;
		m_plnkCtrlTabFeeder1->m_iNG_Count = iTemp1;
		m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL].m_nN_QTY = iTemp1;
		MOlbSystemPre.GetLCNetComponent()->MaterialReport(eMatNG, &m_plnkCtrlTabFeeder1->m_matTabICInfo[DEF_USING_REEL]);
		
		//Feeder2
		MCtrlTabFeeder* m_plnkCtrlTabFeeder2;
		m_plnkCtrlTabFeeder2 = MOlbSystemPre.GetCtrlTabFeederComponent(1);

		int iTemp2= m_plnkCtrlTabFeeder2->m_iNG_CurTab - m_plnkCtrlTabFeeder2->m_iNG_OldTab;
		m_plnkCtrlTabFeeder2->m_iNG_Count = iTemp2;
		m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_USING_REEL].m_nN_QTY = iTemp2;
		MOlbSystemPre.GetLCNetComponent()->MaterialReport(eMatNG, &m_plnkCtrlTabFeeder2->m_matTabICInfo[DEF_USING_REEL]);
		
		
		this->SendMessage(WM_CLOSE);
	}
}
void CMainFrame::SetStatusText(int iOrder, CString st)
{
	if (iOrder== 0)
	{
		m_wndStatusBar.SetWindowText((LPCTSTR)st);
	}
	else
	{
		TEXTMETRIC tm;
		CClientDC dc (this);
		
		CFont* pFont = m_wndStatusBar.GetFont();
		CFont* pOldFont = dc.SelectObject(pFont);
		dc.GetTextMetrics(&tm);
		dc.SelectObject(pOldFont);
		
		int cxWidth;
		UINT nID, nStyle;

		m_wndStatusBar.GetPaneInfo(iOrder, nID, nStyle, cxWidth);
		cxWidth = tm.tmAveCharWidth * st.GetLength();
		m_wndStatusBar.SetPaneInfo(iOrder, nID, nStyle, cxWidth);

		m_wndStatusBar.SetPaneText(iOrder, (LPCTSTR)st, TRUE);
	}
}
