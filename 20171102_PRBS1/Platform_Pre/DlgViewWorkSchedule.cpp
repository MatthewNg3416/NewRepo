// DlgViewWorkSchedule.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "DlgViewWorkSchedule.h"
#include "MCameraCarrier.h"
#include "MTabMounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewWorkSchedule dialog
extern	MPlatformOlbSystem	MOlbSystemPre;

CDlgViewWorkSchedule::CDlgViewWorkSchedule(MWorkSchedule *pSchedule, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewWorkSchedule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewWorkSchedule)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_plnkWorkSchedule = pSchedule;
	m_plnkWorkSchedule->GetOrgWorkList(&m_listWorkTab);
	m_iInspectSelecPos = DEF_CAMERACARRIER_TAB_MARK_POS; //170826 JSH
}


void CDlgViewWorkSchedule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewWorkSchedule)
	DDX_Control(pDX, IDC_SCHEDULE_NO, m_ctrlLblScheduleNo);
	DDX_Control(pDX, IDC_WORKGROUP, m_ctrlLblWorkGroup);
	DDX_Control(pDX, IDC_WORKTAB_NO, m_ctrlLblWorkTabNo);
	DDX_Control(pDX, IDB_MOVE_INSPECT_POS_SELECT, m_ctrlBtnInspectPosSelect);

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TAB_NO_1, m_ctrlLblTabNo[0]);
	DDX_Control(pDX, IDC_TAB_NO_2, m_ctrlLblTabNo[1]);
	DDX_Control(pDX, IDC_TAB_NO_3, m_ctrlLblTabNo[2]);
	DDX_Control(pDX, IDC_TAB_NO_4, m_ctrlLblTabNo[3]);

	DDX_Control(pDX, IDC_INSPECTION_NO_1, m_ctrlLblInspectNo[0]);
	DDX_Control(pDX, IDC_INSPECTION_NO_2, m_ctrlLblInspectNo[1]);
	DDX_Control(pDX, IDC_INSPECTION_NO_3, m_ctrlLblInspectNo[2]);
	DDX_Control(pDX, IDC_INSPECTION_NO_4, m_ctrlLblInspectNo[3]);

	DDX_Control(pDX, IDC_INSPECTION_POS_1, m_ctrlLblInspectPos[0]);
	DDX_Control(pDX, IDC_INSPECTION_POS_2, m_ctrlLblInspectPos[1]);
	DDX_Control(pDX, IDC_INSPECTION_POS_3, m_ctrlLblInspectPos[2]);
	DDX_Control(pDX, IDC_INSPECTION_POS_4, m_ctrlLblInspectPos[3]);

	DDX_Control(pDX, IDC_MOUNTER_NO_1, m_ctrlLblMounterNo[0]);
	DDX_Control(pDX, IDC_MOUNTER_NO_2, m_ctrlLblMounterNo[1]);
	DDX_Control(pDX, IDC_MOUNTER_NO_3, m_ctrlLblMounterNo[2]);
	DDX_Control(pDX, IDC_MOUNTER_NO_4, m_ctrlLblMounterNo[3]);

	DDX_Control(pDX, IDC_MOUNTER_POS_1, m_ctrlLblMounterPos[0]);
	DDX_Control(pDX, IDC_MOUNTER_POS_2, m_ctrlLblMounterPos[1]);
	DDX_Control(pDX, IDC_MOUNTER_POS_3, m_ctrlLblMounterPos[2]);
	DDX_Control(pDX, IDC_MOUNTER_POS_4, m_ctrlLblMounterPos[3]);
}


BEGIN_MESSAGE_MAP(CDlgViewWorkSchedule, CDialog)
	//{{AFX_MSG_MAP(CDlgViewWorkSchedule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewWorkSchedule message handlers

BEGIN_EVENTSINK_MAP(CDlgViewWorkSchedule, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgViewWorkSchedule)
	ON_EVENT(CDlgViewWorkSchedule, IDB_SELECT_PREVIOS, -600 /* Click */, OnClickSelectPrevios, VTS_NONE)
	ON_EVENT(CDlgViewWorkSchedule, IDB_SELECT_NEXT, -600 /* Click */, OnClickSelectNext, VTS_NONE)
	ON_EVENT(CDlgViewWorkSchedule, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CDlgViewWorkSchedule, IDB_MOVE_INSPECT_CARRIER, -600 /* Click */, OnClickMoveInspectCarrier, VTS_NONE)
	ON_EVENT(CDlgViewWorkSchedule, IDB_MOVE_INSPECT_POS_SELECT, -600 /* Click */, OnClickInspectPosSelect, VTS_NONE)
	ON_EVENT(CDlgViewWorkSchedule, IDB_MOVE_TAB_MOUNTER, -600 /* Click */, OnClickMoveTabMounter, VTS_NONE)
	ON_EVENT(CDlgViewWorkSchedule, IDB_MOVE_TAB_MOUNT_Z_CHECK, -600 /* Click */, OnClickMoveTabMountZCheck, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CDlgViewWorkSchedule::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iScheduleListNo = 0;
	CString szTextTemp;
	
	//나가기
	ReadDisplayMessage(MAIN_TEACH, _T("DlgViewWorkSchedule_1"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
	displayScheduleInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgViewWorkSchedule::OnClickSelectPrevios() 
{
	// TODO: Add your control notification handler code here
	if (m_iScheduleListNo <= 0)
		return;
	m_iScheduleListNo--;
	displayScheduleInfo();
}

void CDlgViewWorkSchedule::OnClickSelectNext() 
{
	// TODO: Add your control notification handler code here
	if (m_iScheduleListNo >= m_listWorkTab.GetCount()-1)
		return;
	m_iScheduleListNo++;
	displayScheduleInfo();
}

void CDlgViewWorkSchedule::displayScheduleInfo()
{
	int iScheduleNo = m_plnkWorkSchedule->GetWorkScheculeNo();
	CString strDisplay;
	strDisplay.Format(_T("ScheduleNo:%d\r\n(Tab. %d~%d)"), iScheduleNo+1, m_plnkWorkSchedule->GetFirstTabNo()+1, m_plnkWorkSchedule->GetLastTabNo()+1);
	m_ctrlLblWorkTabNo.SetCaption(strDisplay);

	m_pWorkList = m_listWorkTab.GetSubList(m_iScheduleListNo);
	
	int iWorkGroup = m_pWorkList->GetWorkGroup();
	
	assignComponent();

	int i = 0;
	for (i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		m_dInspectCamTgPos[i] = 0.0;
	for (i=0; i<DEF_MAX_TABMOUNTER; i++)
#ifdef DEF_SOURCE_SYSTEM
//		m_dTabMounterTgPos[i] = m_plnkTabMounter[i]->GetTargetPos(DEF_TABMOUNTER_LOAD_POS);
		m_dTabMounterTgPos[i] = m_plnkTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
#else
		m_dTabMounterTgPos[i] = m_plnkTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
#endif

	m_plnkWorkSchedule->FittingTabNoToMountNo(m_pWorkList, &m_listMounter);
	m_plnkWorkSchedule->FittingTabNoToInspectionNo(m_pWorkList, &m_listInspect);
	MInspectCarrierInfo iInfo;
	MTabInfo			tInfo;
	double dTgPos = 0.0;
	
	strDisplay.Format(_T("%d"), m_iScheduleListNo+1);
	m_ctrlLblScheduleNo.SetCaption(strDisplay);

	if (m_pWorkList->GetWorkGroup() == DEF_FRONT_GROUP)
		strDisplay.Format(_T("Front"));
	else
		strDisplay.Format(_T("Rear"));
	m_ctrlLblWorkGroup.SetCaption(strDisplay);
	
	if(m_iInspectSelecPos == DEF_CAMERACARRIER_TAB_MARK_POS)
	{
		CString strTemp;
		strTemp.Format(_T("Inspection Unit InpsecPos"));
		m_ctrlBtnInspectPosSelect.SetCaption(strTemp);
		
	}
	else if(m_iInspectSelecPos == DEF_CAMERACARRIER_TAB_MOUNT_POS)
	{
		CString strTemp;
		strTemp.Format(_T("Inspection Unit MountPos"));
		m_ctrlBtnInspectPosSelect.SetCaption(strTemp);
	} 

	int cnt = m_listInspect.GetCount();
	int notWorkTabMount = 0;
	for (i = 0; i < cnt; i++)
	{
		iInfo = m_listInspect.GetInfo(i);

		//Tab No Display_____
		if (iInfo.m_nWorkTabNo == -1)
			strDisplay.Format(_T("XXX"));
		else
//@			strDisplay.Format(_T("%d"), iInfo.m_nWorkTabNo+1);
			strDisplay.Format(_T("%d"), iInfo.m_nRealTabNo+1);
		m_ctrlLblTabNo[i].SetCaption(strDisplay);

		//Inspect No Display_____
		strDisplay.Format(_T("%d"), i+1);
		m_ctrlLblInspectNo[i].SetCaption(strDisplay);

		//Inspect Position Display_____
		if(m_iInspectSelecPos == DEF_CAMERACARRIER_TAB_MARK_POS)
		{
			dTgPos = m_plnkCameraCarrier[i]->CalculateInspectPos(iScheduleNo, iInfo.m_dInspectPos);
		}
		else if(m_iInspectSelecPos == DEF_CAMERACARRIER_TAB_MOUNT_POS)
		{
			dTgPos = m_plnkCameraCarrier[i]->CalculateMountPos(iScheduleNo, iInfo.m_dInspectPos);
		} 
		m_dInspectCamTgPos[i] = dTgPos;
		strDisplay.Format(_T("%.3lf"), dTgPos);
		m_ctrlLblInspectPos[i].SetCaption(strDisplay);

		//Mounter No/Position Display
		int tcnt = m_pWorkList->GetCount();
		int j;
		for ( j=0; j<tcnt; j++)
		{
			tInfo = m_pWorkList->GetInfo(j);
			if (tInfo.m_nTabNo == iInfo.m_nWorkTabNo)
			{
				int iToolNo = MOlbSystemPre.Get_ToolInstanceNo(iWorkGroup, tInfo.m_jobInfo.m_nWorkedToolID);
				strDisplay.Format(_T("%d"), m_plnkTabMounter[iToolNo]->GetWorkInstance()+1);
				m_ctrlLblMounterNo[i].SetCaption(strDisplay);

				dTgPos = m_plnkTabMounter[iToolNo]->CalculateMountPos(tInfo.m_dModelOffset);
				m_dTabMounterTgPos[iToolNo] = dTgPos;
				strDisplay.Format(_T("%.3lf"), dTgPos);
				m_ctrlLblMounterPos[i].SetCaption(strDisplay);
				break;
			}
		}
		if (j==tcnt)
		{
			//170922 JSh.s
			//m_ctrlLblMounterNo[i].SetCaption(_T("XXX"));
			//m_ctrlLblMounterPos[i].SetCaption(_T("0.0"));
			int iToolNo = notWorkTabMount; //수정필요
			notWorkTabMount++;
			
			strDisplay.Format(_T("%d"), m_plnkTabMounter[iToolNo]->GetWorkInstance()+1);
			m_ctrlLblMounterNo[i].SetCaption(strDisplay);

			MInspectCarrierInfo	m_InspectInfo = m_plnkWorkSchedule->GetInspectCarrierInfo_FromCurrentWorkList(iToolNo);
			dTgPos = m_plnkTabMounter[iToolNo]->CalculateMountPos(m_InspectInfo.m_dInspectPos);
			m_dTabMounterTgPos[iToolNo] = dTgPos;
			strDisplay.Format(_T("%.3lf"), dTgPos);
			m_ctrlLblMounterPos[i].SetCaption(strDisplay);
			//170922 JSh.e
		}
	}
}

void CDlgViewWorkSchedule::assignComponent()
{
	int iWorkGroup;
	iWorkGroup = m_pWorkList->GetWorkGroup();

	m_plnkCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	m_plnkTabMounter[DEF_MOUNTER_WORK_PER_GROUP];

	int i = 0;
	for (i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		/*/
#ifdef DEF_SOURCE_SYSTEM
		m_plnkCameraCarrier[i] = MOlbSystemPre.GetCameraCarrierComponent(i);
#else
		/*/
		if (iWorkGroup == DEF_FRONT_GROUP)
			m_plnkCameraCarrier[i] = MOlbSystemPre.GetCameraCarrierComponent(i);
		else
			m_plnkCameraCarrier[i] = MOlbSystemPre.GetRCameraCarrierComponent(i);
//@#endif
	}

	for (i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
		/*/
#ifdef DEF_SOURCE_SYSTEM
		m_plnkTabMounter[i] = MOlbSystemPre.GetTabMounterComponent(i);
#else
		/*/
		if (iWorkGroup == DEF_FRONT_GROUP)
			m_plnkTabMounter[i] = MOlbSystemPre.GetTabMounterComponent(i);
		else
			m_plnkTabMounter[i] = MOlbSystemPre.GetRTabMounterComponent(i);
//@#endif
	}
}

void CDlgViewWorkSchedule::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CDlgViewWorkSchedule::OnClickMoveInspectCarrier() 
{
	// TODO: Add your control notification handler code here
//	 목표위치로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,51006,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("IncpectCamera is Moved to Target Position..Wait a moment..."));
	ProgWnd.PeekAndPump();
	int iWorkGroup = m_pWorkList->GetWorkGroup();
	int iResult = MOlbSystemPre.MoveAllGroupCameraCarrier(iWorkGroup, m_dInspectCamTgPos);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	ProgWnd.StepIt();	
}
void CDlgViewWorkSchedule::OnClickMoveTabMounter() 
{
	// TODO: Add your control notification handler code here
	if (MyMessageBox(MY_MESSAGE_BOX,51006,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("TabMounter is Moved to Target Position..Wait a moment..."));
	ProgWnd.PeekAndPump();
	int iWorkGroup = m_pWorkList->GetWorkGroup();
	int iResult = 0;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	double dFTgPos[DEF_MOUNTER_WORK_PER_GROUP];
	double dRTgPos[DEF_MOUNTER_WORK_PER_GROUP];
	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
		if (i<DEF_MOUNTER_WORK_PER_GROUP)
			dFTgPos[i] = m_dTabMounterTgPos[i];
		else
			dRTgPos[DEF_MAX_TABMOUNTER-1-i] = m_dTabMounterTgPos[i];
	}
	iResult = MOlbSystemPre.MoveAllTabMounter(dFTgPos, dRTgPos, DEF_TABMOUNTER_INSPECTION_POS);
#else
	/*/
	if (iWorkGroup == DEF_FRONT_GROUP)
		iResult = MOlbSystemPre.MoveAllFrontTabMounter(m_dTabMounterTgPos, DEF_TABMOUNTER_INSPECTION_POS);
	else
		iResult = MOlbSystemPre.MoveAllRearTabMounter(m_dTabMounterTgPos, DEF_TABMOUNTER_INSPECTION_POS);
//@#endif
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	ProgWnd.StepIt();	
	
}

//150416 SJ_YSH
void CDlgViewWorkSchedule::OnClickMoveTabMountZCheck() 
{
	// TODO: Add your control notification handler code here
	//	 목표위치로 이동하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,51018,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	CMyProgressWnd ProgWnd(NULL, _T("Move Target Position..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("Mount Check is Moved to Target Position..Wait a moment..."));
	ProgWnd.PeekAndPump();
	int i=0;
	int iResult = 0;
	int iWorkGroup = m_pWorkList->GetWorkGroup();


	for (i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (iWorkGroup == DEF_FRONT_GROUP)
			iResult = MOlbSystemPre.GetCameraCarrierComponent(i)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_VISION_POS);
		else
			iResult = MOlbSystemPre.GetRCameraCarrierComponent(i)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_VISION_POS);

		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
		
	}



	iResult = MOlbSystemPre.MoveAllTabMounterZ(iWorkGroup, DEF_TABMOUNTER_Z_INSPECTION_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}
	
	Sleep(1000);

	iResult = MOlbSystemPre.MoveAllTabMounterZ(iWorkGroup, DEF_TABMOUNTER_Z_MOUNT_CHECK_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}

	Sleep((DWORD)MOlbSystemPre.GetSystemData()->m_dBondTime * 2);  

	iResult = MOlbSystemPre.MoveAllTabMounterZ(iWorkGroup, DEF_TABMOUNTER_Z_MOVE_POS);
	if (iResult)
	{
		CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
		MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
	}



	for (i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (iWorkGroup == DEF_FRONT_GROUP)
			iResult = MOlbSystemPre.GetCameraCarrierComponent(i)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
		else
			iResult = MOlbSystemPre.GetRCameraCarrierComponent(i)->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);

		if (iResult)
		{
			CString strErr = MOlbSystemPre.GetErrorMessage(iResult);
			MyMessageBox(strErr.GetBuffer(strErr.GetLength()), _T("Error"), M_ICONERROR);
		}
	}


	ProgWnd.StepIt();
//	ProgWnd.DestroyWindow();

	MyMessageBox(MY_MESSAGE_BOX, 51019, _T("confirm"), M_ICONINFORMATION);
}
//_____________

//170826 JSH.s
void CDlgViewWorkSchedule::OnClickInspectPosSelect() 
{
	if(m_iInspectSelecPos == DEF_CAMERACARRIER_TAB_MARK_POS)
		m_iInspectSelecPos = DEF_CAMERACARRIER_TAB_MOUNT_POS;
	else
		m_iInspectSelecPos = DEF_CAMERACARRIER_TAB_MARK_POS;

	displayScheduleInfo();
}
//170826 JSH.e