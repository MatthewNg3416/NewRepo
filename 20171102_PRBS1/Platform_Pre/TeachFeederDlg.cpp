// FeederTeachDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "TeachFeederDlg.h"
#include "MPlatformOlbSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CTeachFeederDlg dialog

CTeachFeederDlg::CTeachFeederDlg(int iSel, int iFeederID,CWnd* pParent /*=NULL*/)
	: CDialog(CTeachFeederDlg::IDD, pParent)
{ 
	//{{AFX_DATA_INIT(CTeachFeederDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iSelUnit = iFeederID;
	m_iJogSelect = 0;
	m_iSelectedPos = DEF_PRESS_UP_POS;

	// Dispaly Offset �� (Reset S/W ������ ��� ��ġ �� Dispaly ����)
	m_dDisplayOffsetZ = 0.0;

	// ��ü Position
	for(int i=0; i<DEF_PRESS_MAX_POSITION; i++)
	{
		m_rgdFixedPosData[i] = 0.0;
	}

	m_plnkIO = MOlbSystemPre.GetIOComponent();
	m_plnkTabFeeder = MOlbSystemPre.GetTabFeederComponent();
	m_plnkCtrlTabFeeder = MOlbSystemPre.GetCtrlTabFeederComponent();
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkPanelData = MOlbSystemPre.GetPanelData();
	m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
	m_plnkC_MngOpPanel = MOlbSystemPre.GetManageOpPanelComponent();

	m_plnkTabFeeder->GetPressZPosParameter(m_rgdFixedPosData, m_rgdOffsetPosData);
}


void CTeachFeederDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeachFeederDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_FEEDER_POS0,			m_BtnPos[0]);
	DDX_Control(pDX, IDB_FEEDER_POS1,			m_BtnPos[1]);
	DDX_Control(pDX, IDB_FEEDER_POS2,			m_BtnPos[2]);

	DDX_Control(pDX, IDC_TGT_POS_X,			m_sCoord[0]);
	DDX_Control(pDX, IDC_FIX_POS_X,			m_sCoord[1]);
	DDX_Control(pDX, IDC_OFFSET_POS_X,		m_sCoord[2]);
	DDX_Control(pDX, IDC_CUR_POS_X,			m_sCoord[3]);
	DDX_Control(pDX, IDC_DIF_POS_X,			m_sCoord[4]);
}


BEGIN_MESSAGE_MAP(CTeachFeederDlg, CDialog)
	//{{AFX_MSG_MAP(CTeachFeederDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeachFeederDlg message handlers

BEGIN_EVENTSINK_MAP(CTeachFeederDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTeachFeederDlg)
	ON_EVENT(CTeachFeederDlg, IDB_JOG, -600 /* Click */, OnJog, VTS_NONE)
	ON_EVENT(CTeachFeederDlg, IDB_X, -600 /* Click */, OnX, VTS_NONE)
	ON_EVENT(CTeachFeederDlg, IDB_FEEDER_POS0, -600 /* Click */, OnFeederPos0, VTS_NONE)
	ON_EVENT(CTeachFeederDlg, IDB_FEEDER_POS1, -600 /* Click */, OnFeederPos1, VTS_NONE)
	ON_EVENT(CTeachFeederDlg, IDB_FEEDER_POS2, -600 /* Click */, OnFeederPos2, VTS_NONE)
	ON_EVENT(CTeachFeederDlg, IDB_BM_MOVE, -600 /* Click */, OnBmMove, VTS_NONE)
	ON_EVENT(CTeachFeederDlg, IDB_POS_SET, -600 /* Click */, OnPosSet, VTS_NONE)
	ON_EVENT(CTeachFeederDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CTeachFeederDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTeachFeederDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTeachFeederDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i=0, iTemp = 0; 
	int iSelected = -1;

	/** Teaching Position Name Reading (Z ��) */
	m_sTeachingInfo = m_plnkTabFeeder->GetPressZTeachingInfo();
	for(i=0; i<DEF_PRESS_MAX_POSITION; i++)
	{
		if ((m_sTeachingInfo.m_sPosition[i]).m_bUse == TRUE)
		{
			m_BtnPos[iTemp].SetWindowText(_T((m_sTeachingInfo.m_sPosition[i]).m_strNewLineName[0]));
						
			if (m_sTeachingInfo.m_sPosition[i].m_bEnable == FALSE )
				m_BtnPos[iTemp].EnableWindow(FALSE);
			else
			{
				m_BtnPos[iTemp].ShowWindow(SW_SHOW);
				m_BtnPos[iTemp].EnableWindow(TRUE);
			}
			
			iTemp++;
			if (iSelected == -1 && m_sTeachingInfo.m_sPosition[i].m_bEnable == TRUE )
				iSelected = i;
		}
	}
	
	for(i=iTemp; i<DEF_PRESS_MAX_POSITION; i++)
	{
		m_BtnPos[i].ShowWindow(SW_HIDE);
	}
	// �۾��� ���� ������� �ʴ� ��ư Disable ��Ű��.

	if (m_iSelUnit == DEF_TABFEEDER1 )
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_TABPRESSING1);
	else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_TABPRESSING2);

	UpdateCoordinate();

	// �⺻ SelectPosition ���� �ϱ�)
	if (m_iSelectedPos != -1) SelectPosition(m_iSelectedPos);

	m_uiTimerID = SetTimer(1, 200, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTeachFeederDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_uiTimerID == nIDEvent)
	{
		UpdateDisplay();
		UpdateButton();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CTeachFeederDlg::OnJog() 
{
	// TODO: Add your control notification handler code here
	
}

void CTeachFeederDlg::OnX() 
{
	// TODO: Add your control notification handler code here
	char cAfterVal[100];
	char cBeforeVal[100];
	double dNewTargetPos;

	char szTemp[250];

	m_sCoord[0].GetWindowText(cBeforeVal, 30);
	
	/** ��ǥ��ġ �Է� �ޱ� */
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;

	/** ��ǥ��ġ �����ϱ� */
	dNewTargetPos = atof(cAfterVal);
	sprintf(szTemp, "%.3f", dNewTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	m_rgdFixedPosData[m_iSelectedPos] = dNewTargetPos;
				
	sprintf(szTemp, "%.3f", m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
}

void CTeachFeederDlg::OnBmMove() 
{
	CString strMsg = ML_MSG("��ǥ��ġ�� �̵��Ͻðڽ��ϱ�?", "Do you move press z to target position?");
	if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Question", M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, "Move Target Position...");
	ProgWnd.SetRange(0, 2);
	ProgWnd.SetText("It is moving TabFeeder to target position.\n"
						"Please wait a moment...");
	ProgWnd.PeekAndPump();

	// �̵� �� Jog�� ����ϰ� �Ǹ� Motion Error�� ���Ƿ� Jog ��� ����
	m_plnkC_MngOpPanel->SetJogUnit();

	ProgWnd.StepIt();

	int iResult = m_plnkTabFeeder->SafeMovePressZPos(m_iSelectedPos);		

	if (iResult != ERR_TABFEEDER_SUCCESS)
	{
// aaaa		MyMessageBox(iResult, "Backup Teaching Error", M_ICONERROR);
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}	

	ProgWnd.StepIt();

	// Jog ��� �簳
	if (m_iSelUnit == DEF_TABFEEDER1 )
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_TABPRESSING1);
	else
		m_plnkC_MngOpPanel->SetJogUnit(DEF_JOG_TABPRESSING2);
}

void CTeachFeederDlg::OnPosSet() 
{
	CString strMsg = ML_MSG("��ǥ ��ġ�� ���� ��ġ�� �����Ͻðڽ��ϱ�?", "Do you change target position to current position?");
	if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Question", M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	double dCurrent = m_plnkTabFeeder->GetPressZCurrentPos();
	double dTargetPos = dCurrent;
	char szTemp[250];	
	sprintf(szTemp, "%.3f", dTargetPos);
	m_sCoord[0].SetCaption(szTemp);

	m_rgdFixedPosData[m_iSelectedPos] =	dTargetPos;

	sprintf(szTemp, "%.3f", m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
}

void CTeachFeederDlg::OnSave() 
{
	CString strMsg = ML_MSG("Data�� �����Ͻðڽ��ϱ�?", "Do you save data?");
	if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Question", M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkTabFeeder->SavePressZFixedPosParameter(m_rgdFixedPosData);
}

void CTeachFeederDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	m_plnkC_MngOpPanel->SetJogUnit();

	CDialog::OnCancel();
}

/**
 * ��ǥ��ġ, Mark Offset, ��Ÿ����, ������ġ, ���� Display (XYT��)
 */
void CTeachFeederDlg::UpdateDisplay()
{
	char szTemp[250];
	double dCurrentPos;	
	double dTargetPos;
	
	dTargetPos = m_rgdFixedPosData[m_iSelectedPos];
	
	/** ���� ��ġ �а�, Display �ϱ� */
	dCurrentPos = m_plnkTabFeeder->GetPressZCurrentPos();
	
	// Reset Switch ������ ���� ��ġ�� 0���� ǥ�õǰ� �Ͽ� 
	// Teaching�� ���Ÿ� ������ ���� �Ѵ�.
	if (m_plnkIO->IsOn(IN_RESET_SW))
	{
		m_dDisplayOffsetZ = dCurrentPos;
	}
	
	dCurrentPos = dCurrentPos - m_dDisplayOffsetZ;	
	
	sprintf(szTemp, "%.3f", dCurrentPos);
	m_sCoord[3].SetCaption(szTemp);
	
	/** ���� ����ϱ� */
	sprintf(szTemp, "%.3f", dCurrentPos - dTargetPos);
	m_sCoord[4].SetCaption(szTemp);

}
/**
 * ������ǥ, Offset��ǥ Display
 */
void CTeachFeederDlg::UpdateCoordinate()
{
	char szTemp[250];
	double dTargetPos;

	/** ��ǥ ��ġ */
	dTargetPos = m_rgdFixedPosData[m_iSelectedPos];
	sprintf(szTemp, "%.3f", dTargetPos);	
	m_sCoord[0].SetCaption(szTemp);

	/** ������ǥ ȭ�� Display */
	sprintf(szTemp, "%.3f", m_rgdFixedPosData[m_iSelectedPos]);
	m_sCoord[1].SetCaption(szTemp);
}
/**
 * ���� Position ���� 
 */
void CTeachFeederDlg::SelectPosition(int iPosIndex)
{
	m_BtnPos[m_iSelectedPos].SetValue(FALSE);
	m_iSelectedPos = iPosIndex;
	m_BtnPos[iPosIndex].SetValue(TRUE);

	UpdateCoordinate();
}
/**
 * Command Button ���� Update
 */
void CTeachFeederDlg::UpdateButton()
{
}

void CTeachFeederDlg::OnFeederPos0() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(0);
}

void CTeachFeederDlg::OnFeederPos1() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(1);
}

void CTeachFeederDlg::OnFeederPos2() 
{
	// TODO: Add your control notification handler code here
	SelectPosition(2);
}

