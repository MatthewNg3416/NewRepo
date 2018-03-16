// ModelNDeviceIDListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "ModelNDeviceIDListDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"
#include "MPanelData.h"
#include "MSystemData.h"

extern MPlatformOlbSystem	MOlbSystemPre;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelNDeviceIDListDlg dialog


CModelNDeviceIDListDlg::CModelNDeviceIDListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModelNDeviceIDListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelNDeviceIDListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CModelNDeviceIDListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelNDeviceIDListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_MODEL_NAME_DISPLAY, m_btnModelNameDisplay);
	DDX_Control(pDX, IDC_DEVICE_ID_DISPLAY, m_btnDeviceIDDisplay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModelNDeviceIDListDlg, CDialog)
	//{{AFX_MSG_MAP(CModelNDeviceIDListDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelNDeviceIDListDlg message handlers

BEGIN_EVENTSINK_MAP(CModelNDeviceIDListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CModelNDeviceIDListDlg)
	ON_EVENT(CModelNDeviceIDListDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CModelNDeviceIDListDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}

BOOL CModelNDeviceIDListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strModelName;
	CString	strDeviceID;

	strModelName = MOlbSystemPre.GetSystemData()->m_strModelFileName;
	strDeviceID = MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName;

	m_btnModelNameDisplay.SetWindowText(strModelName);
	m_btnDeviceIDDisplay.SetWindowText(strDeviceID);

	SetWindowPos(NULL, 30, 105, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
