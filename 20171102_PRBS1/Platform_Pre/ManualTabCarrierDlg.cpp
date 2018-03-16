// ManualTabCarrierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "ManualTabCarrierDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTabCarrier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualTabCarrierDlg dialog
extern	MPlatformOlbSystem	MOlbSystemPre;

CManualTabCarrierDlg::CManualTabCarrierDlg(int iGroupNo, CWnd* pParent /*=NULL*/)
	: CDialog(CManualTabCarrierDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualTabCarrierDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iGroupNo = iGroupNo;

	if (m_iGroupNo == DEF_FRONT_GROUP)
	{
		for (int i = 0; i<DEF_MAX_TABCARRIER; i++)
			m_plnkTabCarrier[i] = MOlbSystemPre.GetTabCarrierComponent(i);
	}
	else
	{
		for (int i = 0; i<DEF_MAX_TABCARRIER; i++)
			m_plnkTabCarrier[i] = MOlbSystemPre.GetRTabCarrierComponent(i);
	}
}


void CManualTabCarrierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualTabCarrierDlg)
	DDX_Control(pDX, IDB_VAC1_ON,	m_bVac1On);
	DDX_Control(pDX, IDB_VAC1_OFF,	m_bVac1Off);
	DDX_Control(pDX, IDB_VAC2_ON,	m_bVac2On);
	DDX_Control(pDX, IDB_VAC2_OFF,	m_bVac2Off);
	DDX_Control(pDX, IDB_VAC3_OFF,	m_bVac3Off);
	DDX_Control(pDX, IDB_VAC3_ON,	m_bVac3On);
	DDX_Control(pDX, IDB_VAC4_OFF,	m_bVac4Off);
	DDX_Control(pDX, IDB_VAC4_ON,	m_bVac4On);

	DDX_Control(pDX, IDB_UP,		m_bHandUp);
	DDX_Control(pDX, IDB_DOWN,		m_bHandDown);
	DDX_Control(pDX, IDB_FORWARD,	m_bHand2Up);
	DDX_Control(pDX, IDB_BACKWARD,	m_bHand2Down);
	DDX_Control(pDX, IDB_UP2,		m_bHandUp2);
	DDX_Control(pDX, IDB_DOWN2,		m_bHandDown2);

	DDX_Control(pDX, IDB_FORWARD_DUMP_BASKET,	m_bForDumpBasket);
	DDX_Control(pDX, IDB_BACKWARD_DUMP_BASKET,	m_bBackDumpBasket);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManualTabCarrierDlg, CDialog)
	//{{AFX_MSG_MAP(CManualTabCarrierDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ManualTabCarrier message handlers

BOOL CManualTabCarrierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CBtnEnh* rgbtnVacOn[] = 
	{
		&m_bVac1On, &m_bVac2On, &m_bVac3On, &m_bVac4On
	};

	CBtnEnh* rgbtnVacOff[] = 
	{
		&m_bVac1Off, &m_bVac2Off, &m_bVac3Off, &m_bVac4Off
	};

	CBtnEnh* rgbtnCylUp[] =
	{
		&m_bHandUp, &m_bHand2Up, &m_bForDumpBasket
	};

	CBtnEnh* rgbtnCylDown[] =
	{
		&m_bHandDown, &m_bHand2Down, &m_bBackDumpBasket
	};

	BOOL bStatus;

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		bStatus = m_plnkTabCarrier[i]->IsAbsorbTabIC();
		rgbtnVacOn[i]->SetValue(bStatus);
		rgbtnVacOff[i]->SetValue(!bStatus);
	//	bStatus = m_plnkTabCarrier[i]->IsUp();
	//	rgbtnCylUp[i]->SetValue(bStatus);
	//	rgbtnCylDown[i]->SetValue(!bStatus);
	}

	for (int i=DEF_MAX_TABCARRIER; i<DEF_MANUAL_TABCARRIER_MAX_BUTTON; i++)
	{
		rgbtnVacOn[i]->EnableWindow(FALSE);
		rgbtnVacOff[i]->EnableWindow(FALSE);
	}

	bStatus = m_plnkTabCarrier[0]->IsUp();
	rgbtnCylUp[0]->SetValue(bStatus);
	rgbtnCylDown[0]->SetValue(!bStatus);

	bStatus = m_plnkTabCarrier[0]->IsForward();
	rgbtnCylUp[1]->SetValue(bStatus);
	rgbtnCylDown[1]->SetValue(!bStatus);


	m_bForDumpBasket.EnableWindow(FALSE);
	m_bBackDumpBasket.EnableWindow(FALSE);

	if (m_iGroupNo == DEF_FRONT_GROUP)
		SetWindowText(_T("Front Tab Carrier Manual Dialog"));
		
	else	// if (m_iGroupNo == DEF_REAR_GROUP)
		SetWindowText(_T("Rear Tab Carrier Manual Dialog"));

	m_uiTimerID = SetTimer(1, 200, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualTabCarrierDlg_1"), &szTextTemp);
//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CManualTabCarrierDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualTabCarrierDlg)
	ON_EVENT(CManualTabCarrierDlg, IDB_VAC1_ON, -600 /* Click */,	OnClickVac1On, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_VAC1_OFF, -600 /* Click */,	OnClickVac1Off, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_VAC2_ON, -600 /* Click */,	OnClickVac2On, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_VAC2_OFF, -600 /* Click */,	OnClickVac2Off, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_VAC3_ON, -600 /* Click */,	OnClickVac3On, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_VAC3_OFF, -600 /* Click */,	OnClickVac3Off, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_VAC4_ON, -600 /* Click */,	OnClickVac4On, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_VAC4_OFF, -600 /* Click */,	OnClickVac4Off, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_UP, -600 /* Click */,		OnClickHandUp, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_DOWN, -600 /* Click */,		OnClickHandDown, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_EXIT, -600 /* Click */,		OnClickExit, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_FORWARD, -600 /* Click */, OnClickForward, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_BACKWARD, -600 /* Click */, OnClickBackward, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_FORWARD_DUMP_BASKET, -600 /* Click */, OnClickForwardDumpBasket, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_BACKWARD_DUMP_BASKET, -600 /* Click */, OnClickBackwardDumpBasket, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_UP2, -600 /* Click */, OnClickUp2, VTS_NONE)
	ON_EVENT(CManualTabCarrierDlg, IDB_DOWN2, -600 /* Click */, OnClickDown2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManualTabCarrierDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL	bStatus = FALSE;
	BOOL	bOldStatus = FALSE;

	CBtnEnh* rgbtnVacOn[] = 
	{
		&m_bVac1On, &m_bVac2On, &m_bVac3On, &m_bVac4On
	};

	CBtnEnh* rgbtnVacOff[] = 
	{
		&m_bVac1Off, &m_bVac2Off, &m_bVac3Off, &m_bVac4Off
	};

	CBtnEnh* rgbtnCylUp[] =
	{
		&m_bHandUp, &m_bHand2Up, &m_bForDumpBasket, &m_bHandUp2
	};

	CBtnEnh* rgbtnCylDown[] =
	{
		&m_bHandDown, &m_bHand2Down, &m_bBackDumpBasket, &m_bHandDown2
	};

	if (m_uiTimerID == nIDEvent)
	{
		// Carrier Vacuum
		for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		{
			bStatus = m_plnkTabCarrier[i]->IsAbsorbTabIC();
			bOldStatus = rgbtnVacOn[i]->GetValue();
			if (bStatus == TRUE && bOldStatus == FALSE)
				rgbtnVacOn[i]->SetValue(TRUE);
			else if (bStatus == FALSE && bOldStatus != FALSE)
				rgbtnVacOn[i]->SetValue(FALSE);

			bStatus = m_plnkTabCarrier[i]->IsReleaseTabIC();
			bOldStatus = rgbtnVacOff[i]->GetValue();
			if (bStatus == TRUE && bOldStatus == FALSE)
				rgbtnVacOff[i]->SetValue(TRUE);
			else if (bStatus == FALSE && bOldStatus != FALSE)
				rgbtnVacOff[i]->SetValue(FALSE);

		}
		bStatus = m_plnkTabCarrier[0]->IsUp();
		bOldStatus = rgbtnCylUp[0]->GetValue();
		if (bStatus == TRUE && bOldStatus == FALSE)
			rgbtnCylUp[0]->SetValue(TRUE);
		else if (bStatus == FALSE && bOldStatus != FALSE)
			rgbtnCylUp[0]->SetValue(FALSE);
		
		bStatus = m_plnkTabCarrier[0]->IsDown();
		bOldStatus = rgbtnCylDown[0]->GetValue();
		if (bStatus == TRUE && bOldStatus == FALSE)
			rgbtnCylDown[0]->SetValue(TRUE);
		else if (bStatus == FALSE && bOldStatus != FALSE)
			rgbtnCylDown[0]->SetValue(FALSE);

		bStatus = m_plnkTabCarrier[0]->IsForward();
		bOldStatus = rgbtnCylUp[1]->GetValue();
		if (bStatus == TRUE && bOldStatus == FALSE)
			rgbtnCylUp[1]->SetValue(TRUE);
		else if (bStatus == FALSE && bOldStatus != FALSE)
			rgbtnCylUp[1]->SetValue(FALSE);
		
		bStatus = m_plnkTabCarrier[0]->IsBackward();
		bOldStatus = rgbtnCylDown[1]->GetValue();
		if (bStatus == TRUE && bOldStatus == FALSE)
			rgbtnCylDown[1]->SetValue(TRUE);
		else if (bStatus == FALSE && bOldStatus != FALSE)
			rgbtnCylDown[1]->SetValue(FALSE);

		bStatus = m_plnkTabCarrier[0]->IsUp2();
		bOldStatus = rgbtnCylUp[3]->GetValue();
		if (bStatus == TRUE && bOldStatus == FALSE)
			rgbtnCylUp[3]->SetValue(TRUE);
		else if (bStatus == FALSE && bOldStatus != FALSE)
			rgbtnCylUp[3]->SetValue(FALSE);
		
		bStatus = m_plnkTabCarrier[0]->IsDown2();
		bOldStatus = rgbtnCylDown[3]->GetValue();
		if (bStatus == TRUE && bOldStatus == FALSE)
			rgbtnCylDown[3]->SetValue(TRUE);
		else if (bStatus == FALSE && bOldStatus != FALSE)
			rgbtnCylDown[3]->SetValue(FALSE);

	

	}

	CDialog::OnTimer(nIDEvent);
}

void CManualTabCarrierDlg::OnClickVac1On() 
{
	int iResult;

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER1]->AbsorbTabIC()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Vaccum1 Absorb"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabCarrierDlg::OnClickVac1Off() 
{
	int iResult;

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER1]->ReleaseTabIC()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Carrier Vaccum1 Release"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabCarrierDlg::OnClickVac2On() 
{
	int iResult;

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER2]->AbsorbTabIC()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Vaccum2 Absorb"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabCarrierDlg::OnClickVac2Off() 
{
	int iResult;

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER2]->ReleaseTabIC()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Vaccum2 Release"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
}

void CManualTabCarrierDlg::OnClickVac3Off() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER3]->ReleaseTabIC()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Carrier Vaccum3 Release"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	
}

void CManualTabCarrierDlg::OnClickVac3On() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER3]->AbsorbTabIC()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Carrier Vaccum3 Absorb"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	
}

void CManualTabCarrierDlg::OnClickVac4Off() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER4]->ReleaseTabIC()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult,_T("Carrier Vaccum4 Release"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	
}

void CManualTabCarrierDlg::OnClickVac4On() 
{
	// TODO: Add your control notification handler code here
	int iResult;

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER4]->AbsorbTabIC()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Vaccum4 Absorb"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}		
}

void CManualTabCarrierDlg::OnClickHandUp() 
{
	int iResult;
//	Carrier PickUp Unit Up/Down 실린더를 Up하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30500, _T("Carrier Up/Down Cylinder Up"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Carrier Up/Down Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Carrier PickUp Unit Up/Down Cylinder Up.. Wait a moment..."));
	ProgWnd.PeekAndPump();

	int nTabCarrierNo = DEF_TABCARRIER1;
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		if (m_plnkTabCarrier[i]->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
		{
			nTabCarrierNo = i;
			break;
		}
	}
	if ((iResult = m_plnkTabCarrier[nTabCarrierNo]->Up()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Hand Up") ,M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabCarrierDlg::OnClickHandDown() 
{
	int iResult;
//	Carrier PickUp Unit Up/Down 실린더를 Down하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30501, _T("Carrier Up/Down Cylinder Down"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Carrier Up/Down Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Carrier Hand Up/Down Cylinder Down.. Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER1]->Down()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Hand Down"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabCarrierDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CManualTabCarrierDlg::OnCancel();
}

void CManualTabCarrierDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CManualTabCarrierDlg::OnClickForward() 
{
	// TODO: Add your control notification handler code here
		int iResult;
//	Carrier PickUp Unit Up/Down 실린더를 Forward하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30506, _T("Carrier For/Back Cylinder Up"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Carrier For/Back Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Carrier PickUp Unit For/Back Cylinder Up.. Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER1]->Forward()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Hand Up") ,M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
	
}

void CManualTabCarrierDlg::OnClickBackward() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	//	Carrier PickUp Unit Up/Down 실린더를 Backward하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30507, _T("Carrier For/Back Cylinder Up"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Carrier For/Back Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Carrier PickUp Unit For/Back Cylinder Up.. Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER1]->Backward()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Hand Up") ,M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualTabCarrierDlg::OnClickForwardDumpBasket() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CMyProgressWnd ProgWnd(NULL, _T("Carrier DumpBasket Cylinder For..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Carrier DumpBasket Cylinder For.. Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER4]->ForwardDumpBasket ()) != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabCarrierDlg::OnClickBackwardDumpBasket() 
{
	// TODO: Add your control notification handler code here
	int iResult;
	CMyProgressWnd ProgWnd(NULL, _T("Carrier DumpBasket Cylinder Back..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Carrier DumpBasket Cylinder Back.. Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER4]->BackwardDumpBasket ()) != SUCCESS)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();	
}

void CManualTabCarrierDlg::OnClickUp2() 
{
	// TODO: Add your control notification handler code here
	int iResult;
//	Carrier PickUp Unit Up/Down 실린더를 Up하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30500, _T("Carrier Up/Down Cylinder Up"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Carrier Up/Down Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Carrier PickUp Unit Up/Down Cylinder Up.. Wait a moment..."));
	ProgWnd.PeekAndPump();

	int nTabCarrierNo = DEF_TABCARRIER1;
	for (int i=DEF_TABCARRIER2; i<DEF_MAX_TABCARRIER; i+=2)
	{
		if (m_plnkTabCarrier[i]->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
		{
			nTabCarrierNo = i;
			break;
		}
	}
	if ((iResult = m_plnkTabCarrier[nTabCarrierNo]->Up2()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Hand Up") ,M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}

void CManualTabCarrierDlg::OnClickDown2() 
{
	// TODO: Add your control notification handler code here
	int iResult;
//	Carrier PickUp Unit Up/Down 실린더를 Down하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 30501, _T("Carrier Up/Down Cylinder Down"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Carrier Up/Down Cylinder..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Carrier Hand Up/Down Cylinder Down.. Wait a moment..."));
	ProgWnd.PeekAndPump();

	if ((iResult = m_plnkTabCarrier[DEF_TABCARRIER2]->Down2()) != SUCCESS)
	{
// jdy sesl		MyMessageBox(iResult, _T("Carrier Hand Down"),M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	ProgWnd.StepIt();
}
