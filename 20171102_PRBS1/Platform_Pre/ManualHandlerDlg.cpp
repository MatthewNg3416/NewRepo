// ManualHandlerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include "ManualHandlerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CManualHandlerDlg dialog

CManualHandlerDlg::CManualHandlerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualHandlerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualHandlerDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

}

void CManualHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualHandlerDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_UNLOAD_HANDLER_VAC_ON, m_ctrlBtnUnloadHandlerVacuumOn1);
	DDX_Control(pDX, IDB_UNLOAD_HANDLER_VAC_OFF, m_ctrlBtnUnloadHandlerVacuumOff1);
	DDX_Control(pDX, IDB_UNLOAD_HANDLER_VAC_ON2, m_ctrlBtnUnloadHandlerVacuumOn2);
	DDX_Control(pDX, IDB_UNLOAD_HANDLER_VAC_OFF2, m_ctrlBtnUnloadHandlerVacuumOff2);
	DDX_Control(pDX, IDB_UNLOAD_HANDLER_VAC_ON3, m_ctrlBtnUnloadHandlerVacuumOn3);
	DDX_Control(pDX, IDB_UNLOAD_HANDLER_VAC_OFF3, m_ctrlBtnUnloadHandlerVacuumOff3);
	DDX_Control(pDX, IDB_UNLOAD_HANDLER_VAC_ON_ALL, m_ctrlBtnUnloadHandlerVacOn);
	DDX_Control(pDX, IDB_UNLOAD_HANDLER_VAC_OFF_ALL, m_ctrlBtnUnloadHandlerVacOff);
	DDX_Control(pDX, IDB_EXIT, m_bCancel);
}

BEGIN_MESSAGE_MAP(CManualHandlerDlg, CDialog)
	//{{AFX_MSG_MAP(CManualHandlerDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualHandlerDlg message handlers

BEGIN_EVENTSINK_MAP(CManualHandlerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualHandlerDlg)
	ON_EVENT(CManualHandlerDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CManualHandlerDlg, IDB_UNLOAD_HANDLER_VAC_ON, -600 /* Click */, OnClickUnloadHandlerVacOn1, VTS_NONE)
	ON_EVENT(CManualHandlerDlg, IDB_UNLOAD_HANDLER_VAC_OFF, -600 /* Click */, OnClickUnloadHandlerVacOff1, VTS_NONE)
	ON_EVENT(CManualHandlerDlg, IDB_UNLOAD_HANDLER_VAC_ON2, -600 /* Click */, OnClickUnloadHandlerVacOn2, VTS_NONE)
	ON_EVENT(CManualHandlerDlg, IDB_UNLOAD_HANDLER_VAC_OFF2, -600 /* Click */, OnClickUnloadHandlerVacOff2, VTS_NONE)
	ON_EVENT(CManualHandlerDlg, IDB_UNLOAD_HANDLER_VAC_ON3, -600 /* Click */, OnClickUnloadHandlerVacOn3, VTS_NONE)
	ON_EVENT(CManualHandlerDlg, IDB_UNLOAD_HANDLER_VAC_OFF3, -600 /* Click */, OnClickUnloadHandlerVacOff3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CManualHandlerDlg, IDB_UNLOAD_HANDLER_VAC_ON_ALL, -600 /* Click */, OnClickUnloadHandlerVacOnAll, VTS_NONE)
	ON_EVENT(CManualHandlerDlg, IDB_UNLOAD_HANDLER_VAC_OFF_ALL, -600 /* Click */, OnClickUnloadHandlerVacOffAll, VTS_NONE)

END_EVENTSINK_MAP()

void CManualHandlerDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CManualHandlerDlg::OnCancel();
}

void CManualHandlerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	BOOL	bStatus = FALSE;
	BOOL	bOldStatus = FALSE;

	if (m_uiTimerID == nIDEvent)
	{
		// Unload Handler Vacuum Absorb Status Check
		bStatus = m_plnkUnloadHandler->IsAbsorbPanel(0);
		bOldStatus = m_ctrlBtnUnloadHandlerVacuumOn1.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_ctrlBtnUnloadHandlerVacuumOn1.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_ctrlBtnUnloadHandlerVacuumOn1.SetValue(FALSE);

		bStatus = m_plnkUnloadHandler->IsReleasePanel(0);
		bOldStatus = m_ctrlBtnUnloadHandlerVacuumOff1.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_ctrlBtnUnloadHandlerVacuumOff1.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_ctrlBtnUnloadHandlerVacuumOff1.SetValue(FALSE);

		bStatus = m_plnkUnloadHandler->IsAbsorbPanel(1);
		bOldStatus = m_ctrlBtnUnloadHandlerVacuumOn2.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_ctrlBtnUnloadHandlerVacuumOn2.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_ctrlBtnUnloadHandlerVacuumOn2.SetValue(FALSE);

		bStatus = m_plnkUnloadHandler->IsReleasePanel(1);
		bOldStatus = m_ctrlBtnUnloadHandlerVacuumOff2.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_ctrlBtnUnloadHandlerVacuumOff2.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_ctrlBtnUnloadHandlerVacuumOff2.SetValue(FALSE);

		bStatus = m_plnkUnloadHandler->IsAbsorbPanel(2);
		bOldStatus = m_ctrlBtnUnloadHandlerVacuumOn3.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_ctrlBtnUnloadHandlerVacuumOn3.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_ctrlBtnUnloadHandlerVacuumOn3.SetValue(FALSE);

		bStatus = m_plnkUnloadHandler->IsReleasePanel(2);
		bOldStatus = m_ctrlBtnUnloadHandlerVacuumOff3.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_ctrlBtnUnloadHandlerVacuumOff3.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_ctrlBtnUnloadHandlerVacuumOff3.SetValue(FALSE);

		bStatus = m_plnkUnloadHandler->IsAbsorbPanel();
		bOldStatus = m_ctrlBtnUnloadHandlerVacOn.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_ctrlBtnUnloadHandlerVacOn.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_ctrlBtnUnloadHandlerVacOn.SetValue(FALSE);

		bStatus = m_plnkUnloadHandler->IsReleasePanel();
		bOldStatus = m_ctrlBtnUnloadHandlerVacOff.GetValue();
		if ((bStatus == TRUE) && (bOldStatus == FALSE))
			m_ctrlBtnUnloadHandlerVacOff.SetValue(TRUE);
		else if ((bStatus == FALSE) && (bOldStatus != FALSE))
			m_ctrlBtnUnloadHandlerVacOff.SetValue(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CManualHandlerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
#ifdef DEF_USE_TURN_HANDLER
	m_plnkUnloadHandler = MOlbSystemPre.GetTurnHandlerComponent();
#endif
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkPanelData = MOlbSystemPre.GetPanelData();

	// SJ_Chi 언어별 Display
	if (m_plnkSystemData->m_uiLanguageSelect != LANGUAGE_KOREAN)
	{
		m_bCancel.SetCaption(_T("Cancel"));
	}

	m_uiTimerID = SetTimer(1, 500, NULL);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CManualHandlerDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(),&m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CManualHandlerDlg::OnClickUnloadHandlerVacOn1() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CString strTemp;

	if (MyMessageBox(_T("Absorb the 1'st vacuum of Unload Handler?\r\n Unload Handler 의 Vacuum을 흡착하시겠습니까?"), 
			_T("Unload Handler Vacuum ON"), M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Absorbing the Unload Handler 1'st Vacumm.\n")
					_T("Wait a moment...\n")
					_T("Unload Handler Vacuum 를 흡착 중 입니다.\n")
				    _T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkUnloadHandler->AbsorbPanel(0, FALSE)) != SUCCESS)
	{
		strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()),"Unload Handler Vacuum ON",M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CManualHandlerDlg::OnClickUnloadHandlerVacOff1() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CString strTemp;

	if (MyMessageBox(_T("Release the 1'st vacuum of Unload Handler?\r\n Unload Handler 의 Vacuum을 탈착하시겠습니까?"), 
			_T("Unload Handler Vacuum OFF"), M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Releasing the Unload Handler 1'st Vacumm.\n")
					_T("Wait a moment...\n")
					_T("Unload Handler Vacuum 를 탈착 중 입니다.\n")
				    _T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	/*/
	if (!(m_plnkUnloadHandler->IsInXYTPos(DEF_HANDLER_LOAD_POS)
		&& m_plnkUnloadHandler->IsInZPos(DEF_HANDLER_Z_LOAD_POS))
		&& !(m_plnkUnloadHandler->IsInXYTPos(DEF_HANDLER_UNLOAD_POS)
		&& m_plnkUnloadHandler->IsInZPos(DEF_HANDLER_Z_UNLOAD_POS)))
	{
		MyMessageBox("Cannot release the Handler Vacuum because Handler is not in Loading or Unlaoding position.\r\n"
			"Handler가 Loading위치도 Unloading 위치도 아니라서 Vacuum Off를 할 수 없습니다.", "Vacuum Interlock Error");
		return;
	}
	/*/

	if ((iResult = m_plnkUnloadHandler->ReleasePanel(0, FALSE)) != SUCCESS)
	{
		strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()),"Unload Handler Vacuum OFF",M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CManualHandlerDlg::OnClickUnloadHandlerVacOn2() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CString strTemp;

	if (MyMessageBox(_T("Absorb the 2nd vacuum of Unload Handler?\r\n")
			_T("Unload Handler 의 Vacuum2을 흡착하시겠습니까?"), 
			_T("Unload Handler Vacuum ON"), M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum2..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Absorbing the Unload Handler Vacuum2.\n")
					_T("Wait a moment...\n")
					_T("Unload Handler Vacuum2 를 흡착 중 입니다.\n")
				    _T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkUnloadHandler->AbsorbPanel(1, FALSE)) != SUCCESS)
	{
		strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()),"Unload Handler Vacuum ON",M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CManualHandlerDlg::OnClickUnloadHandlerVacOff2() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CString strTemp;

	if (MyMessageBox(_T("Release the 2nd vacuum of Load Handler?\r\n")
			_T("Unload Handler 의 Vacuum2을 탈착하시겠습니까?"), _T("Unload Handler Vacuum OFF"), M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum2..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Releasing the Unload Handler Vacuum2.\n")
					_T("Wait a moment...\n")
					_T("Unload Handler Vacuum2 를 탈착 중 입니다.\n")
				    _T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkUnloadHandler->ReleasePanel(1, FALSE)) != SUCCESS)
	{
		strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()),"Unload Handler Vacuum OFF",M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CManualHandlerDlg::OnClickUnloadHandlerVacOn3() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CString strTemp;

	if (MyMessageBox(_T("Absorb the 3th vacuum of Unload Handler?\r\n")
			_T("Unload Handler 의 Vacuum3을 흡착하시겠습니까?"), _T("Unload Handler Vacuum ON"),
				M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum3..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Absorbing the Unload Handler Vacuum3.\n")
					_T("Wait a moment...\n")
					_T("Unload Handler Vacuum 를 흡착 중 입니다.\n")
				    _T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkUnloadHandler->AbsorbPanel(2, FALSE)) != SUCCESS)
	{
		strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()),"Unload Handler Vacuum ON",M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CManualHandlerDlg::OnClickUnloadHandlerVacOff3() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CString strTemp;

	if (MyMessageBox(_T("Rlease the 3th vacuum of Load Handler?\r\n")
			_T("Unload Handler 의 Vacuum3을 탈착하시겠습니까?"), 
			_T("Unload Handler Vacuum OFF"), M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum3..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Rleasing the Unload Handler Vacuum3.\n")
					_T("Wait a moment...\n")
					_T("Unload Handler Vacuum3 를 탈착 중 입니다.\n")
				    _T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkUnloadHandler->ReleasePanel(2, FALSE)) != SUCCESS)
	{
		strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()),"Unload Handler Vacuum OFF",M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CManualHandlerDlg::OnClickUnloadHandlerVacOnAll()
{
	int iResult;
	CString strTemp;

	if (MyMessageBox(_T("Absorb the vacuum of Unload Handler?\r\n Unload Handler 의 Vacuum을 흡착하시겠습니까?"),
			_T("Unload Handler Vacuum ON"), M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Absorbing the Unload Handler Vacumm.\n")
					_T("Wait a moment...\n")
					_T("Unload Handler Vacuum 를 흡착 중 입니다.\n")
				    _T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkUnloadHandler->AbsorbPanel(FALSE)) != SUCCESS)
	{
		strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()),"Unload Handler Vacuum ON",M_ICONERROR);
	}

	ProgWnd.StepIt();
}

void CManualHandlerDlg::OnClickUnloadHandlerVacOffAll()
{
	int iResult;
	CString strTemp;

	if (MyMessageBox(_T("Release the vacuum of Unload Handler?\r\n Unload Handler 의 Vacuum을 탈착하시겠습니까?"), 
			_T("Unload Handler Vacuum OFF"), M_ICONQUESTION|M_YESNO, _T("Yes"), _T("No")) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Vacuum..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Releasing the Unload Handler Vacumm.\n")
					_T("Wait a moment...\n")
					_T("Unload Handler Vacuum 를 탈착 중 입니다.\n")
				    _T("잠시만 기다려 주세요..."));
	ProgWnd.PeekAndPump();

	/*/
	if (!(m_plnkUnloadHandler->IsInXYTPos(DEF_HANDLER_LOAD_POS)
		&& m_plnkUnloadHandler->IsInZPos(DEF_HANDLER_Z_LOAD_POS))
		&& !(m_plnkUnloadHandler->IsInXYTPos(DEF_HANDLER_UNLOAD_POS)
		&& m_plnkUnloadHandler->IsInZPos(DEF_HANDLER_Z_UNLOAD_POS)))
	{
		MyMessageBox("Cannot release the Handler Vacuum because Handler is not in Loading or Unlaoding position.\r\n"
			"Handler가 Loading위치도 Unloading 위치도 아니라서 Vacuum Off를 할 수 없습니다.", "Vacuum Interlock Error");
		return;
	}
	/*/

	if ((iResult = m_plnkUnloadHandler->ReleasePanel(FALSE)) != SUCCESS)
	{
		strTemp = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strTemp.GetBuffer(strTemp.GetLength()),"Unload Handler Vacuum OFF",M_ICONERROR);
	}

	ProgWnd.StepIt();
}
