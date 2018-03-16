// ManualPanelTransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "ManualPanelTransferDlg.h"
#include "MPlatformOlbSystem.h"
#include "MPanelTransfer.h"
#include "DefIOAddrPreBonder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualPanelTransferDlg dialog

extern	MPlatformOlbSystem	MOlbSystemPre; 

CManualPanelTransferDlg::CManualPanelTransferDlg(int iTransferID, CWnd* pParent /*=NULL*/)
	: CDialog(CManualPanelTransferDlg::IDD, pParent), m_iTransferID(iTransferID)
{
	//{{AFX_DATA_INIT(CManualPanelTransferDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	switch (m_iTransferID) {
#ifdef DEF_USE_TRANSFER_CENTER
	case DEF_PANEL_TRANSFER_CENTER:
		m_plnkPanelTransfer = MOlbSystemPre.GetPanelTransferCenterComponent();
		break;
#endif
#ifdef DEF_SOURCE_SYSTEM
	case DEF_PANEL_TRANSFER_IN:
		m_plnkPanelTransfer = MOlbSystemPre.GetPanelTransferComponent();
		break;
#endif
	case DEF_PANEL_TRANSFER_OUT:
		m_plnkPanelTransfer = MOlbSystemPre.GetPanelTransferOutComponent();
		break;
	default:
		ASSERT(FALSE);
	}

	m_plnkIIO = MOlbSystemPre.GetIOComponent();

	memset(m_szTimeValue, NULL, sizeof(m_szTimeValue));
}


void CManualPanelTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualPanelTransferDlg)
	DDX_Control(pDX, IDB_PANEL_TRANSFER_UP, m_btnPanelTransferUp);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_DOWN, m_btnPanelTransferDown);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_VACUUM_ON, m_btnPanelTransferVacuumOn);
	DDX_Control(pDX, IDB_PANEL_TRANSFER_VACUUM_OFF, m_btnPanelTransferVacuumOff);
	DDX_Control(pDX, IDS_PANEL_TRANSFER_IN_PANEL_DETECT, m_lblPanelDetect);
	DDX_Control(pDX, IDC_MANUAL_PANEL_TRANSFER_NAME, m_btnTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManualPanelTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CManualPanelTransferDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualPanelTransferDlg message handlers

BOOL CManualPanelTransferDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgCaption();

	m_uiTimerID = SetTimer(1, 200, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManualPanelTransferDlg::SetDlgCaption()
{
	TCHAR* strDlgName[DEF_MAX_PANEL_TRANSFER] = {
		_T("ACF Transfer-Center Manual Dialog"), 
#ifdef DEF_SOURCE_SYSTEM
		_T("ACF Transfer-In Manual Dialog"), 
#endif
		_T("ACF Transfer-Out Manual Dialog")
	};

	TCHAR* strTitleName[DEF_MAX_PANEL_TRANSFER] = {
		_T("Transfer-Center"),
#ifdef DEF_SOURCE_SYSTEM
		_T("Transfer-In"),
#endif
		_T("Transfer-Out")
	};

	m_btnTitle.SetCaption(strTitleName[m_iTransferID]);
	SetWindowText(strDlgName[m_iTransferID]);

}

BEGIN_EVENTSINK_MAP(CManualPanelTransferDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualPanelTransferDlg)
	ON_EVENT(CManualPanelTransferDlg, IDB_PANEL_TRANSFER_UP, -600 /* Click */, OnClickPanelTransferUp, VTS_NONE)
	ON_EVENT(CManualPanelTransferDlg, IDB_PANEL_TRANSFER_DOWN, -600 /* Click */, OnClickPanelTransferDown, VTS_NONE)
	ON_EVENT(CManualPanelTransferDlg, IDB_PANEL_TRANSFER_VACUUM_ON, -600 /* Click */, OnClickPanelTransferVacuumOn, VTS_NONE)
	ON_EVENT(CManualPanelTransferDlg, IDB_PANEL_TRANSFER_VACUUM_OFF, -600 /* Click */, OnClickPanelTransferVacuumOff, VTS_NONE)
	ON_EVENT(CManualPanelTransferDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManualPanelTransferDlg::OnClickPanelTransferUp() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Up/Down Cylinder Up..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Up/Down Cylinder Up.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->UpPickUpUDCyl();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelTransferDlg::OnClickPanelTransferDown() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Up/Down Cylinder Down..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Up/Down Cylinder Down.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	// hongju Ãß°¡ Check Interlock
	if (TRUE == m_plnkPanelTransfer->IsPanelAbsorbed())
	{
		MyMessageBox(_T("Panel Transfer Panel Absorbed"), _T("Error"), M_ICONERROR);
		return;
	}

	iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelTransferDlg::OnClickPanelTransferVacuumOn() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Absorb..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Absorb.\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->AbsorbPanel();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelTransferDlg::OnClickPanelTransferVacuumOff() 
{
	// TODO: Add your control notification handler code here
	int iResult = SUCCESS;

	CMyProgressWnd ProgWnd(NULL, _T("Panel Transfer Release..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Panel Transfer Vacuum off .\n")
					    _T("Wait for one moment..."));
	ProgWnd.PeekAndPump();

	iResult = m_plnkPanelTransfer->ReleasePanel();
	if(iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualPanelTransferDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL	bStatus = FALSE;
	BOOL	bOldStatus = FALSE;

	if (m_uiTimerID == nIDEvent)
	{
		// Panel Transfer Up/Down Cylinder
		// Panel Transfer Up Status Check
		bStatus = m_plnkPanelTransfer->IsUpPickUpUDCyl();
		bOldStatus = m_btnPanelTransferUp.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferUp.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferUp.SetValue(FALSE);

		// Panel Transfer Down Status Check
		bStatus = m_plnkPanelTransfer->IsDownPickUpUDCyl();
		bOldStatus = m_btnPanelTransferDown.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferDown.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferDown.SetValue(FALSE);

		// Panel Transfer Vacuum
		// Panel Transfer Absorb Status Check
		bStatus = m_plnkPanelTransfer->IsPanelAbsorbed();
		bOldStatus = m_btnPanelTransferVacuumOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferVacuumOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferVacuumOn.SetValue(FALSE);

		// Panel Transfer Release Status Check
		bStatus = m_plnkPanelTransfer->IsPanelReleased();
		bOldStatus = m_btnPanelTransferVacuumOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_btnPanelTransferVacuumOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_btnPanelTransferVacuumOff.SetValue(FALSE);

		// Panel Detect Check
		bStatus = m_plnkPanelTransfer->IsPanelDetected();
		bOldStatus = m_lblPanelDetect.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
		{
			m_lblPanelDetect.SetCaption(_T("On"));
			m_lblPanelDetect.SetBackColor(DEF_ON_COLOR);
			m_lblPanelDetect.SetValue(bStatus);
		}
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
		{
			m_lblPanelDetect.SetCaption(_T("Off"));
			m_lblPanelDetect.SetBackColor(DEF_OFF_COLOR);
			m_lblPanelDetect.SetValue(bStatus);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CManualPanelTransferDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CDialog::OnCancel();	
}
