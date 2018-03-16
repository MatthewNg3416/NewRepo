// MPlatformOlb.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MainFrm.h"
#include "MPlatformOlbDoc.h"
#include "MPlatformOlbView.h"
#include "MPlatformOlbSystem.h"

extern MPlatformOlbSystem	MOlbSystemPre;
BOOL	gIOTestMode = FALSE;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Data Segment 생성 중 [Shared] Section 을 만든다.
#pragma data_seg("Shared")
// [Shared] Data Segment Section 에 들어가는 변수
// 주의 : 초기화를 하지 않으면, [Shared] Section 이 아닌 bbs 섹션으로 들어간다.
long g_lAppUsageCount = 0;

#pragma data_seg()

// Linker 에게 [Shared] Section 은 쓰기, 읽기, 공유로 Link 함을 알림.
//#pragma comment(linker, DEF_MSG)
#pragma comment(linker, "/section:Shared,rws")

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbApp

BEGIN_MESSAGE_MAP(CMPlatformOlbApp, CWinApp)
	//{{AFX_MSG_MAP(CMPlatformOlbApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbApp construction

CMPlatformOlbApp::CMPlatformOlbApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
CMPlatformOlbApp::~CMPlatformOlbApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	if(MDebug::GetCreateDumpStatus() == FALSE)
		MDebug::RemoveDumpPath();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMPlatformOlbApp object

CMPlatformOlbApp theApp;

//100807.ECID__________________
simpleAdo MyInputDB;
bool IsConnect;

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbApp initialization

BOOL CMPlatformOlbApp::InitInstance()
{
	g_lAppUsageCount++; 
	
//	if (g_lAppUsageCount > 1)
//		return FALSE;

	//110708.kms.add_______________________________________________________
	HANDLE hMutex;
	hMutex = CreateMutex(NULL,FALSE, _T("MPlatformOlb.exe"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AfxMessageBox(_T("MPlatformOlb program is already the execution.^^!!"));
		CloseHandle(hMutex);
		return FALSE;
	}
	//____________________________________________________________________

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	int iResult = MOlbSystemPre.Initialize();
	if (iResult)
	{
// jdy sesl		MyMessageBox(iResult, _T("Error"), M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		return FALSE;
	}

	MDebug::RegisterUserDump();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMPlatformOlbDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMPlatformOlbView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//100807.ECID___________________
	if (!MyInputDB.Connect(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")
		_T(".\\Info\\ECID_PRB.mdb;Persist Security Info=False")))
		return false;
	IsConnect = true;

	// Test
	//CString strMySql;
	//strMySql = _T("Select * From EQ_ECID order by ECID ASC");
	//if (!MyInputDB.ExcuteQry((LPCTSTR)strMySql)) {
	//	MyInputDB.Disconnect();
	//	return false;
	//}
	//_______________________________

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMPlatformOlbApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMPlatformOlbApp message handlers


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}
