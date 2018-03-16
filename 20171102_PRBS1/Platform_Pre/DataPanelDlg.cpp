// DataPanelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MProcessData.h"
#include "DataPanelDlg.h"
#include "MPlatformOlbSystem.h"
#include "MPanelAligner.h"
#include "MPanelTransfer.h"
#include "MTabCarrier.h"
#include "MTabFeeder.h"
#include "MCameraCarrier.h"
#include "MCtrlTabFeeder.h"
#include "MModelChange.h"
//MERGE_LKH_090704 : SESL_조재근S 버전으로 적용
#include "MTabMounter.h"
#include "MTrsAutoManager.h"
//110114.kms__________
#include "CommonGetValueDlg.h"

#include "MSystemData.h"

#include "MTrsAutoManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataPanelDlg dialog

//101004.KKY_____
//CDataPanelDlg::CDataPanelDlg(CWnd* pParent /*=NULL*/)
CDataPanelDlg::CDataPanelDlg(BOOL bCurrentModel, BOOL bEditable, CWnd* pParent /*=NULL*/)   // standard constructor
//_______________
	: CDialog(CDataPanelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataPanelDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	memset(m_szOldValue, NULL, sizeof(m_szOldValue));
	memset(m_szNewValue, NULL, sizeof(m_szNewValue));

	//101004.KKY_____
//	m_plnkPanelData = MOlbSystemPre.GetPanelData();
	if (bCurrentModel)
		m_plnkPanelData = MOlbSystemPre.GetPanelData();
	else
		m_plnkPanelData = MOlbSystemPre.GetTmpPanelData();
	m_bEditable = bEditable;
	//_______________

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}

void CDataPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataPanelDlg)
	DDX_Control(pDX, IDC_PANEL_SIZE_X, m_lblPanelSize_X);
	DDX_Control(pDX, IDC_PANEL_SIZE_Y, m_lblPanelSize_Y);
	DDX_Control(pDX, IDC_PANEL_THICKNESS, m_lblPanelThickness);
	DDX_Control(pDX, IDC_LOW_POL_THICKNESS, m_lblPanelLowSidePolThickness);
	DDX_Control(pDX, IDC_LOAD_DIR, m_lblPanel_InDirection);
	DDX_Control(pDX, IDC_UNLOAD_DIR, m_lblPanel_OutDirection);
	DDX_Control(pDX, IDC_RUN_MODE, m_lblRunMode);
	//110114.kms__________
	DDX_Control(pDX, IDC_REFERENCE_DEVICE, m_lblReferenceDevice);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_USE_XU, m_lblRowData[0][0]);
	DDX_Control(pDX, IDC_USE_XD, m_lblRowData[0][1]);
	DDX_Control(pDX, IDC_USE_YL, m_lblRowData[0][2]);
	DDX_Control(pDX, IDC_USE_YR, m_lblRowData[0][3]);

	DDX_Control(pDX, IDC_PANEL_MARK_X_0, m_lblRowData[1][0]);
	DDX_Control(pDX, IDC_PANEL_MARK_X_1, m_lblRowData[1][1]);
	DDX_Control(pDX, IDC_PANEL_MARK_X_2, m_lblRowData[1][2]);
	DDX_Control(pDX, IDC_PANEL_MARK_X_3, m_lblRowData[1][3]);

	DDX_Control(pDX, IDC_PANEL_MARK_Y_0, m_lblRowData[2][0]);
	DDX_Control(pDX, IDC_PANEL_MARK_Y_1, m_lblRowData[2][1]);
	DDX_Control(pDX, IDC_PANEL_MARK_Y_2, m_lblRowData[2][2]);
	DDX_Control(pDX, IDC_PANEL_MARK_Y_3, m_lblRowData[2][3]);

	DDX_Control(pDX, IDC_PANEL_MARK_DIST_0, m_lblRowData[3][0]);
	DDX_Control(pDX, IDC_PANEL_MARK_DIST_1, m_lblRowData[3][1]);
	DDX_Control(pDX, IDC_PANEL_MARK_DIST_2, m_lblRowData[3][2]);
	DDX_Control(pDX, IDC_PANEL_MARK_DIST_3, m_lblRowData[3][3]);

	DDX_Control(pDX, IDC_SUB_MARK_USE_0, m_lblRowData[4][0]);
	DDX_Control(pDX, IDC_SUB_MARK_USE_1, m_lblRowData[4][1]);
	DDX_Control(pDX, IDC_SUB_MARK_USE_2, m_lblRowData[4][2]);
	DDX_Control(pDX, IDC_SUB_MARK_USE_3, m_lblRowData[4][3]);

	DDX_Control(pDX, IDC_SUB_MARK_X_0, m_lblRowData[5][0]);
	DDX_Control(pDX, IDC_SUB_MARK_X_1, m_lblRowData[5][1]);
	DDX_Control(pDX, IDC_SUB_MARK_X_2, m_lblRowData[5][2]);
	DDX_Control(pDX, IDC_SUB_MARK_X_3, m_lblRowData[5][3]);

	DDX_Control(pDX, IDC_SUB_MARK_Y_0, m_lblRowData[6][0]);
	DDX_Control(pDX, IDC_SUB_MARK_Y_1, m_lblRowData[6][1]);
	DDX_Control(pDX, IDC_SUB_MARK_Y_2, m_lblRowData[6][2]);
	DDX_Control(pDX, IDC_SUB_MARK_Y_3, m_lblRowData[6][3]);

	DDX_Control(pDX, IDC_SUB_MARK_DIST_0, m_lblRowData[7][0]);
	DDX_Control(pDX, IDC_SUB_MARK_DIST_1, m_lblRowData[7][1]);
	DDX_Control(pDX, IDC_SUB_MARK_DIST_2, m_lblRowData[7][2]);
	DDX_Control(pDX, IDC_SUB_MARK_DIST_3, m_lblRowData[7][3]);

	DDX_Control(pDX, IDC_MARK_DIST_PANELTOTAB_0, m_lblRowData[8][0]);
	DDX_Control(pDX, IDC_MARK_DIST_PANELTOTAB_1, m_lblRowData[8][1]);
	DDX_Control(pDX, IDC_MARK_DIST_PANELTOTAB_2, m_lblRowData[8][2]);
	DDX_Control(pDX, IDC_MARK_DIST_PANELTOTAB_3, m_lblRowData[8][3]);

}

BEGIN_MESSAGE_MAP(CDataPanelDlg, CDialog)
	//{{AFX_MSG_MAP(CDataPanelDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataPanelDlg message handlers

void CDataPanelDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDataPanelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iSelectedRow = DEF_MAX_ROW;

	m_dPanelSize_X = m_plnkPanelData->m_dPanelSize_X;	
	m_dPanelSize_Y = m_plnkPanelData->m_dPanelSize_Y;	
	m_ePanel_InDirection = m_plnkPanelData->m_ePanel_InDirection;		
	m_ePanel_OutDirection = m_plnkPanelData->m_ePanel_OutDirection;	
	m_dPanelThickness = m_plnkPanelData->m_dPanelThickness;			
	m_dPanelLowSidePolThickness = m_plnkPanelData->m_dPanelLowSidePolThickness;

	m_rgbUseSideFlag[0] = m_plnkPanelData->m_bUseFlag_Xu;
	m_rgbUseSideFlag[1] = m_plnkPanelData->m_bUseFlag_Xd;
	m_rgbUseSideFlag[2] = m_plnkPanelData->m_bUseFlag_Yl;
	m_rgbUseSideFlag[3] = m_plnkPanelData->m_bUseFlag_Yr;

	m_rgbUse2ndMarkFlag[0] = m_plnkPanelData->m_bUseFlag_SubMark_Xu;
	m_rgbUse2ndMarkFlag[1] = m_plnkPanelData->m_bUseFlag_SubMark_Xd;
	m_rgbUse2ndMarkFlag[2] = m_plnkPanelData->m_bUseFlag_SubMark_Yl;
	m_rgbUse2ndMarkFlag[3] = m_plnkPanelData->m_bUseFlag_SubMark_Yr;

	m_rgdPanelData[1][0] = m_plnkPanelData->m_dX_FiduMark_Xu;		
	m_rgdPanelData[2][0] = m_plnkPanelData->m_dY_FiduMark_Xu;
	m_rgdPanelData[3][0] = m_plnkPanelData->m_dDistance_FiduMark_Xu;
	m_rgdPanelData[5][0] = m_plnkPanelData->m_dX_FiduSubMark_Xu;	
	m_rgdPanelData[6][0] = m_plnkPanelData->m_dY_FiduSubMark_Xu;	
	m_rgdPanelData[7][0] = m_plnkPanelData->m_dDistance_FiduSubMark_Xu;
	m_rgdPanelData[8][0] = m_plnkPanelData->m_dMarkDist_PanelToTab_Xu;

	m_rgdPanelData[1][1] = m_plnkPanelData->m_dX_FiduMark_Xd;
	m_rgdPanelData[2][1] = m_plnkPanelData->m_dY_FiduMark_Xd;
	m_rgdPanelData[3][1] = m_plnkPanelData->m_dDistance_FiduMark_Xd;
	m_rgdPanelData[5][1] = m_plnkPanelData->m_dX_FiduSubMark_Xd;
	m_rgdPanelData[6][1] = m_plnkPanelData->m_dY_FiduSubMark_Xd;
	m_rgdPanelData[7][1] = m_plnkPanelData->m_dDistance_FiduSubMark_Xd;
	m_rgdPanelData[8][1] = m_plnkPanelData->m_dMarkDist_PanelToTab_Xd;

	m_rgdPanelData[1][2] = m_plnkPanelData->m_dX_FiduMark_Yl;
	m_rgdPanelData[2][2] = m_plnkPanelData->m_dY_FiduMark_Yl;
	m_rgdPanelData[3][2] = m_plnkPanelData->m_dDistance_FiduMark_Yl;
	m_rgdPanelData[5][2] = m_plnkPanelData->m_dX_FiduSubMark_Yl;
	m_rgdPanelData[6][2] = m_plnkPanelData->m_dY_FiduSubMark_Yl;
	m_rgdPanelData[7][2] = m_plnkPanelData->m_dDistance_FiduSubMark_Yl;
	m_rgdPanelData[8][2] = m_plnkPanelData->m_dMarkDist_PanelToTab_Yl;

	m_rgdPanelData[1][3] = m_plnkPanelData->m_dX_FiduMark_Yr;
	m_rgdPanelData[2][3] = m_plnkPanelData->m_dY_FiduMark_Yr;
	m_rgdPanelData[3][3] = m_plnkPanelData->m_dDistance_FiduMark_Yr;
	m_rgdPanelData[5][3] = m_plnkPanelData->m_dX_FiduSubMark_Yr;
	m_rgdPanelData[6][3] = m_plnkPanelData->m_dY_FiduSubMark_Yr;
	m_rgdPanelData[7][3] = m_plnkPanelData->m_dDistance_FiduSubMark_Yr;
	m_rgdPanelData[8][3] = m_plnkPanelData->m_dMarkDist_PanelToTab_Yr;

	//110114.kms
	m_strReferenceDevice = m_plnkSystemData->m_strCurrentDeviceID; //170915 JSh

	m_eRunMode = m_plnkPanelData->m_eRunMode;

	UpdateDisplay();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelDlg_1"), &szTextTemp);
	//저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataPanelDlg_2"), &szTextTemp);
	//나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	//101004.KKY_____
	if (FALSE == m_bEditable)
	{
		GetDlgItem(IDB_X_UPPER)->EnableWindow(FALSE);
		GetDlgItem(IDB_X_LOWER)->EnableWindow(FALSE);
		GetDlgItem(IDB_Y_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDB_Y_RIGHT)->EnableWindow(FALSE);

		GetDlgItem(IDB_RUN_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDB_PANEL_SIZE_X)->EnableWindow(FALSE);
		GetDlgItem(IDB_PANEL_SIZE_Y)->EnableWindow(FALSE);
		GetDlgItem(IDB_LOAD_DIR)->EnableWindow(FALSE);
		GetDlgItem(IDB_UNLOAD_DIR)->EnableWindow(FALSE);
		GetDlgItem(IDB_PANEL_THICKNESS)->EnableWindow(FALSE);
		GetDlgItem(IDB_LOW_POL_THICKNESS)->EnableWindow(FALSE);

		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	}
	//_______________

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDataPanelDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataPanelDlg)
	ON_EVENT(CDataPanelDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_PANEL_MARK_X, -600 /* Click */, OnPanelMarkX, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_PANEL_MARK_Y, -600 /* Click */, OnPanelMarkY, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_PANEL_MARK_DIST, -600 /* Click */, OnPanelMarkDist, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_SUB_MARK_USE, -600 /* Click */, OnSubMarkUse, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_SUB_MARK_X, -600 /* Click */, OnSubMarkX, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_SUB_MARK_Y, -600 /* Click */, OnSubMarkY, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_SUB_MARK_DIST, -600 /* Click */, OnSubMarkDist, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_X_UPPER, -600 /* Click */, OnXUpper, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_X_LOWER, -600 /* Click */, OnXLower, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_Y_LEFT, -600 /* Click */, OnYLeft, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_Y_RIGHT, -600 /* Click */, OnYRight, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_PANEL_SIZE_X, -600 /* Click */, OnPanelSizeX, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_PANEL_SIZE_Y, -600 /* Click */, OnPanelSizeY, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_LOAD_DIR, -600 /* Click */, OnLoadDir, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_UNLOAD_DIR, -600 /* Click */, OnUnloadDir, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_PANEL_THICKNESS, -600 /* Click */, OnPanelThickness, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_LOW_POL_THICKNESS, -600 /* Click */, OnLowPolThickness, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_USE_SIDE, -600 /* Click */, OnClickUseSide, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_RUN_MODE, -600 /* Click */, OnClickRunMode, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_MARK_DIST_PANELTOTAB, -600 /* Click */, OnClickMarkDistPaneltotab, VTS_NONE)
	ON_EVENT(CDataPanelDlg, IDB_REFERENCE_INFO, -600 /* Click */, OnClickReferenceInfo, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataPanelDlg::OnSave() 
{
//  Data를 저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40900, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkPanelData->m_dPanelSize_X = m_dPanelSize_X;	
	m_plnkPanelData->m_dPanelSize_Y = m_dPanelSize_Y;	
	m_plnkPanelData->m_ePanel_InDirection = m_ePanel_InDirection;		
	m_plnkPanelData->m_ePanel_OutDirection = m_ePanel_OutDirection;	
	m_plnkPanelData->m_dPanelThickness = m_dPanelThickness;			
	m_plnkPanelData->m_dPanelLowSidePolThickness = m_dPanelLowSidePolThickness;

	m_plnkPanelData->m_bUseFlag_Xu = m_rgbUseSideFlag[0];	
	m_plnkPanelData->m_bUseFlag_Xd = m_rgbUseSideFlag[1];	
	m_plnkPanelData->m_bUseFlag_Yl = m_rgbUseSideFlag[2];	
	m_plnkPanelData->m_bUseFlag_Yr = m_rgbUseSideFlag[3];	

	m_plnkPanelData->m_bUseFlag_SubMark_Xu = m_rgbUse2ndMarkFlag[0];	
	m_plnkPanelData->m_bUseFlag_SubMark_Xd = m_rgbUse2ndMarkFlag[1];	
	m_plnkPanelData->m_bUseFlag_SubMark_Yl = m_rgbUse2ndMarkFlag[2];	
	m_plnkPanelData->m_bUseFlag_SubMark_Yr = m_rgbUse2ndMarkFlag[3];	

	m_plnkPanelData->m_dX_FiduMark_Xu			= m_rgdPanelData[1][0];		
	m_plnkPanelData->m_dY_FiduMark_Xu			= m_rgdPanelData[2][0];
	m_plnkPanelData->m_dDistance_FiduMark_Xu	= m_rgdPanelData[3][0];
	m_plnkPanelData->m_dX_FiduSubMark_Xu		= m_rgdPanelData[5][0];
	m_plnkPanelData->m_dY_FiduSubMark_Xu		= m_rgdPanelData[6][0];
	m_plnkPanelData->m_dDistance_FiduSubMark_Xu = m_rgdPanelData[7][0];
	m_plnkPanelData->m_dMarkDist_PanelToTab_Xu	= m_rgdPanelData[8][0];

	m_plnkPanelData->m_dX_FiduMark_Xd			= m_rgdPanelData[1][1];
	m_plnkPanelData->m_dY_FiduMark_Xd			= m_rgdPanelData[2][1];
	m_plnkPanelData->m_dDistance_FiduMark_Xd	= m_rgdPanelData[3][1];
	m_plnkPanelData->m_dX_FiduSubMark_Xd		= m_rgdPanelData[5][1];
	m_plnkPanelData->m_dY_FiduSubMark_Xd		= m_rgdPanelData[6][1];
	m_plnkPanelData->m_dDistance_FiduSubMark_Xd = m_rgdPanelData[7][1];
	m_plnkPanelData->m_dMarkDist_PanelToTab_Xd	= m_rgdPanelData[8][1];

	m_plnkPanelData->m_dX_FiduMark_Yl			= m_rgdPanelData[1][2];
	m_plnkPanelData->m_dY_FiduMark_Yl			= m_rgdPanelData[2][2];
	m_plnkPanelData->m_dDistance_FiduMark_Yl	= m_rgdPanelData[3][2];
	m_plnkPanelData->m_dX_FiduSubMark_Yl		= m_rgdPanelData[5][2];
	m_plnkPanelData->m_dY_FiduSubMark_Yl		= m_rgdPanelData[6][2];
	m_plnkPanelData->m_dDistance_FiduSubMark_Yl = m_rgdPanelData[7][2];
	m_plnkPanelData->m_dMarkDist_PanelToTab_Yl	= m_rgdPanelData[8][2];

	m_plnkPanelData->m_dX_FiduMark_Yr			= m_rgdPanelData[1][3];
	m_plnkPanelData->m_dY_FiduMark_Yr			= m_rgdPanelData[2][3];
	m_plnkPanelData->m_dDistance_FiduMark_Yr	= m_rgdPanelData[3][3];
	m_plnkPanelData->m_dX_FiduSubMark_Yr		= m_rgdPanelData[5][3];
	m_plnkPanelData->m_dY_FiduSubMark_Yr		= m_rgdPanelData[6][3];
	m_plnkPanelData->m_dDistance_FiduSubMark_Yr = m_rgdPanelData[7][3];
	m_plnkPanelData->m_dMarkDist_PanelToTab_Yr	= m_rgdPanelData[8][3];
	
	m_plnkPanelData->m_eRunMode					= m_eRunMode;

	//110114.kms
//	m_plnkPanelData->m_strReferenceDevice		= m_strReferenceDevice; //170919 JSH Del.

	m_plnkPanelData->WriteData();

	//111201.kms_________
	m_plnkSystemData->m_eRunMode = m_plnkPanelData->m_eRunMode;
	m_plnkSystemData->WriteData();
	::SendMessage(MOlbSystemPre.GetTrsAutoManagerComponent()->GetAutoViewHandle(), WM_DISP_RUN_MODE, NULL, NULL);
	//___________________

	//101004.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
		return;
	//_______________

	int i = 0;

	MOlbSystemPre.GetProcessData()->ChangeProcessData();

	MOlbSystemPre.AssignWorkScheduleData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		MOlbSystemPre.GetWorkSchedule(i)->ChangeModel();
#ifdef DEF_SOURCE_SYSTEM
		MOlbSystemPre.GetWorkScheduleSecond(i)->ChangeModel();
#endif
	}

	MOlbSystemPre.GetPanelAlignerComponent()->ChangeModel();

#ifdef DEF_SOURCE_SYSTEM
//@	MOlbSystemPre.GetPanelTransferCenterComponent()->ChangeModel();
	MOlbSystemPre.GetPanelTransferComponent()->ChangeModel();
#endif
//@#else
	MOlbSystemPre.GetModelChangeComponent(DEF_FRONT_GROUP)->ChangeModel();
	MOlbSystemPre.GetModelChangeComponent(DEF_REAR_GROUP)->ChangeModel();
//@#endif
	MOlbSystemPre.GetPanelTransferOutComponent()->ChangeModel();

	// Model생성시 gentry Model Change 안됨
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		MOlbSystemPre.GetTabMounterComponent(i)->ChangeModel();
//@#ifdef DEF_GATE_SYSTEM
		MOlbSystemPre.GetRTabMounterComponent(i)->ChangeModel();
//@#endif
	}
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		MOlbSystemPre.GetTabCarrierComponent(i)->ChangeModel();
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		MOlbSystemPre.GetRTabCarrierComponent(i)->ChangeModel();

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		MOlbSystemPre.GetCtrlTabFeederComponent(i)->ChangeModel();
		MOlbSystemPre.GetTabFeederComponent(i)->ChangeModel();
	}
	
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		MOlbSystemPre.GetCameraCarrierComponent(i)->ChangeModel();
//@#ifdef DEF_GATE_SYSTEM
		MOlbSystemPre.GetRCameraCarrierComponent(i)->ChangeModel();
//@#endif
	}
}

void CDataPanelDlg::OnExit() 
{
	CDialog::OnCancel();
}

void CDataPanelDlg::UpdateDisplay() 
{
	swprintf(m_szNewValue, _T("%.3f"), m_dPanelSize_X);
	m_lblPanelSize_X.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%.3f"), m_dPanelSize_Y);
	m_lblPanelSize_Y.SetCaption(m_szNewValue);

	if (m_ePanel_InDirection == INPUT_DIRECTION_NORMAL)
		m_lblPanel_InDirection.SetCaption(_T("Normal"));
	else
		m_lblPanel_InDirection.SetCaption(_T("Reverse"));

	if (m_ePanel_OutDirection == OUTPUT_DIRECTION_NORMAL)
		m_lblPanel_OutDirection.SetCaption(_T("Normal"));
	else
		m_lblPanel_OutDirection.SetCaption(_T("Reverse"));
	
	swprintf(m_szNewValue, _T("%.3f"), m_dPanelThickness);
	m_lblPanelThickness.SetCaption(m_szNewValue);

	swprintf(m_szNewValue, _T("%.3f"), m_dPanelLowSidePolThickness);
	m_lblPanelLowSidePolThickness.SetCaption(m_szNewValue);

	// Use Working Side
	if (m_rgbUseSideFlag[0])
		m_lblRowData[0][0].SetCaption(_T("TRUE"));
	else
		m_lblRowData[0][0].SetCaption(_T("FALSE"));

	if (m_rgbUseSideFlag[1])
		m_lblRowData[0][1].SetCaption(_T("TRUE"));
	else
		m_lblRowData[0][1].SetCaption(_T("FALSE"));
	
	if (m_rgbUseSideFlag[2])
		m_lblRowData[0][2].SetCaption(_T("TRUE"));
	else
		m_lblRowData[0][2].SetCaption(_T("FALSE"));

	if (m_rgbUseSideFlag[3])
		m_lblRowData[0][3].SetCaption(_T("TRUE"));
	else
		m_lblRowData[0][3].SetCaption(_T("FALSE"));
	
	// Use 2nd Mark
	if (m_rgbUse2ndMarkFlag[0])
		m_lblRowData[4][0].SetCaption(_T("TRUE"));
	else
		m_lblRowData[4][0].SetCaption(_T("FALSE"));

	if (m_rgbUse2ndMarkFlag[1])
		m_lblRowData[4][1].SetCaption(_T("TRUE"));
	else
		m_lblRowData[4][1].SetCaption(_T("FALSE"));
	
	if (m_rgbUse2ndMarkFlag[2])
		m_lblRowData[4][2].SetCaption(_T("TRUE"));
	else
		m_lblRowData[4][2].SetCaption(_T("FALSE"));

	if (m_rgbUse2ndMarkFlag[3])
		m_lblRowData[4][3].SetCaption(_T("TRUE"));
	else
		m_lblRowData[4][3].SetCaption(_T("FALSE"));

	for(int i=0; i<DEF_MAX_ROW; i++)
	{
		if (i != 0 && i != 4)
		{
			for(int j=0; j<DEF_MAX_COLUMN;j++)
			{
				swprintf(m_szNewValue, _T("%.3f"), m_rgdPanelData[i][j]);
				m_lblRowData[i][j].SetCaption(m_szNewValue);
			}
		}
	}

	//110114.kms
	swprintf(m_szNewValue, _T("%s"), m_strReferenceDevice.GetBuffer());
	m_lblReferenceDevice.SetCaption(m_szNewValue);

	//NSMC KJS
	if (m_eRunMode == NORMAL_RUN_MODE)
		m_lblRunMode.SetCaption(_T("Normal"));
	else if (m_eRunMode == PASS_RUN_MODE)
		m_lblRunMode.SetCaption(_T("Pass"));
	else if (m_eRunMode == DRY_RUN_MODE)
		m_lblRunMode.SetCaption(_T("Dry"));
	else if (m_eRunMode == REPAIR_RUN_MODE)
		m_lblRunMode.SetCaption(_T("Repare"));
	else
		m_lblRunMode.SetCaption(_T("Normal"));
}

void CDataPanelDlg::OnPanelSizeX() 
{
	m_lblPanelSize_X.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dPanelSize_X = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dPanelSize_X);
	m_dPanelSize_X = wcstod(m_szNewValue, NULL);

	
	m_lblPanelSize_X.SetCaption(m_szNewValue);

	calculateFiducialMarkDist();//110225.Add_____
}

void CDataPanelDlg::OnPanelSizeY()
{
	m_lblPanelSize_Y.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dPanelSize_Y = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dPanelSize_Y);
	m_dPanelSize_Y = wcstod(m_szNewValue, NULL);

	
	m_lblPanelSize_Y.SetCaption(m_szNewValue);

	calculateFiducialMarkDist();//110225.Add_____
}

void CDataPanelDlg::OnLoadDir() 
{
	if (m_ePanel_InDirection == INPUT_DIRECTION_NORMAL)	
	{
		m_ePanel_InDirection = INPUT_DIRECTION_REVERSE;
		m_lblPanel_InDirection.SetCaption(_T("Reverse"));
	}
	else 
	{
		m_ePanel_InDirection = INPUT_DIRECTION_NORMAL;
		m_lblPanel_InDirection.SetCaption(_T("Normal"));
	}	
}

void CDataPanelDlg::OnUnloadDir() 
{
	if (m_ePanel_OutDirection == OUTPUT_DIRECTION_NORMAL)	
	{
		m_ePanel_OutDirection = OUTPUT_DIRECTION_REVERSE;
		m_lblPanel_OutDirection.SetCaption(_T("Reverse"));
	}
	else 
	{
		m_ePanel_OutDirection = OUTPUT_DIRECTION_NORMAL;
		m_lblPanel_OutDirection.SetCaption(_T("Normal"));
	}
}

void CDataPanelDlg::OnPanelThickness() 
{
	m_lblPanelThickness.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dPanelThickness = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dPanelThickness);
	m_dPanelThickness = wcstod(m_szNewValue, NULL);
	
	m_lblPanelThickness.SetCaption(m_szNewValue);
}

void CDataPanelDlg::OnLowPolThickness() 
{
	m_lblPanelLowSidePolThickness.GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))	return;

	m_dPanelLowSidePolThickness = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_dPanelLowSidePolThickness);
	m_dPanelLowSidePolThickness = wcstod(m_szNewValue, NULL);
	
	m_lblPanelLowSidePolThickness.SetCaption(m_szNewValue);
}

void CDataPanelDlg::OnXUpper() 
{
	if ((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_MAX_ROW))
	{
//	행 선택이 되지 않았습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40901, _T("Error"), M_ICONERROR);
		return;
	}
	
	updateDisplay(0);
}

void CDataPanelDlg::OnXLower() 
{
	if ((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_MAX_ROW))
	{
//		행 선택이 되지 않았습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40902, _T("Error"), M_ICONERROR);
		return;
	}

	updateDisplay(1);
}

void CDataPanelDlg::OnYLeft() 
{
	if ((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_MAX_ROW))
	{
//	행 선택이 되지 않았습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40903, _T("Error"), M_ICONERROR);
		return;
	}

	updateDisplay(2);
}

void CDataPanelDlg::OnYRight() 
{
	if ((m_iSelectedRow < 0) || (m_iSelectedRow >= DEF_MAX_ROW))
	{
//	행 선택이 되지 않았습니다.
		MyMessageBox(MY_MESSAGE_BOX, 40904, _T("Error"), M_ICONERROR);
		return;
	}
	
	updateDisplay(3);
}

void CDataPanelDlg::OnClickUseSide() 
{
	m_iSelectedRow = 0;	
}

void CDataPanelDlg::OnPanelMarkX() 
{
	m_iSelectedRow = 1;
}

void CDataPanelDlg::OnPanelMarkY() 
{
	m_iSelectedRow = 2;
}

void CDataPanelDlg::OnPanelMarkDist() 
{
	m_iSelectedRow = 3;
}

void CDataPanelDlg::OnSubMarkUse() 
{
	m_iSelectedRow = 4;
}

void CDataPanelDlg::OnSubMarkX() 
{
	m_iSelectedRow = 5;
}

void CDataPanelDlg::OnSubMarkY() 
{
	m_iSelectedRow = 6;
}

void CDataPanelDlg::OnSubMarkDist() 
{
	m_iSelectedRow = 7;
}

void CDataPanelDlg::OnClickMarkDistPaneltotab() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedRow = 8;	
}

void CDataPanelDlg::updateDisplay(int iColNo)
{
	switch(m_iSelectedRow)
	{
	case 0:
		if (m_rgbUseSideFlag[iColNo])	
		{
			m_rgbUseSideFlag[iColNo] = FALSE;
			m_lblRowData[m_iSelectedRow][iColNo].SetCaption(_T("FALSE"));
		}
		else 
		{
			m_rgbUseSideFlag[iColNo] = TRUE;
			m_lblRowData[m_iSelectedRow][iColNo].SetCaption(_T("TRUE"));
		}
		break;
	case 4:
		if (m_rgbUse2ndMarkFlag[iColNo])	
		{
			m_rgbUse2ndMarkFlag[iColNo] = FALSE;
			m_lblRowData[m_iSelectedRow][iColNo].SetCaption(_T("FALSE"));
		}
		else 
		{
			m_rgbUse2ndMarkFlag[iColNo] = TRUE;
			m_lblRowData[m_iSelectedRow][iColNo].SetCaption(_T("TRUE"));
		}
		break;
	default :
		m_lblRowData[m_iSelectedRow][iColNo].GetWindowText(m_szOldValue, 30);
		
		if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))
			return;
		
		m_rgdPanelData[m_iSelectedRow][iColNo] = wcstod(m_szNewValue, NULL);
		swprintf(m_szNewValue, _T("%.3f"), m_rgdPanelData[m_iSelectedRow][iColNo]);
		m_rgdPanelData[m_iSelectedRow][iColNo] = wcstod(m_szNewValue, NULL);
		
		m_lblRowData[m_iSelectedRow][iColNo].SetCaption(m_szNewValue);

		//110225.Add_____
		if ( 1 == m_iSelectedRow || 2 == m_iSelectedRow) //170915 JSh
		{
			calculateFiducialMarkDist();
		}
		//_______________
	}
}

void CDataPanelDlg::OnClickRunMode() 
{
	// TODO: Add your control notification handler code here
	//NSMC KJS Normal, Pass로만 변경함.
	if (m_eRunMode == NORMAL_RUN_MODE)
		m_eRunMode = PASS_RUN_MODE;
	else
		m_eRunMode = NORMAL_RUN_MODE;
	
	UpdateDisplay();
}

//110114.kms__________
void CDataPanelDlg::OnClickReferenceInfo() 
{
	// TODO: Add your control notification handler code here
	WCHAR szTemp[100];
	WCHAR szBuf[100];

	memset(szTemp, 0, sizeof(szTemp));
	swprintf(szBuf, _T(""));

	if(!GetValueBox(szTemp, 20, szBuf, _T("Input Device ID")))
		return;

	m_strReferenceDevice.Format(_T("%s"), szTemp);
	m_lblReferenceDevice.SetCaption(m_strReferenceDevice);
}
//____________________

//110225.Add_____
void CDataPanelDlg::calculateFiducialMarkDist()
{
	m_rgdPanelData[3][0] = m_dPanelSize_X - m_rgdPanelData[1][0] - m_rgdPanelData[2][0];  //170915 JSh
	swprintf(m_szNewValue, _T("%.3f"), m_rgdPanelData[3][0]);
	m_rgdPanelData[3][0] = wcstod(m_szNewValue, NULL);	
	m_lblRowData[3][0].SetCaption(m_szNewValue);

	m_rgdPanelData[3][1] = m_dPanelSize_X - m_rgdPanelData[1][1] - m_rgdPanelData[2][1];  //170915 JSh
	swprintf(m_szNewValue, _T("%.3f"), m_rgdPanelData[3][1]);
	m_rgdPanelData[3][1] = wcstod(m_szNewValue, NULL);	
	m_lblRowData[3][1].SetCaption(m_szNewValue);

	m_rgdPanelData[3][2] = m_dPanelSize_Y - m_rgdPanelData[1][2] - m_rgdPanelData[2][2];  //170915 JSh
	swprintf(m_szNewValue, _T("%.3f"), m_rgdPanelData[3][2]);
	m_rgdPanelData[3][2] = wcstod(m_szNewValue, NULL);	
	m_lblRowData[3][2].SetCaption(m_szNewValue);

	m_rgdPanelData[3][3] = m_dPanelSize_Y - m_rgdPanelData[1][3] - m_rgdPanelData[2][3]; //170915 JSh
	swprintf(m_szNewValue, _T("%.3f"), m_rgdPanelData[3][3]);
	m_rgdPanelData[3][3] = wcstod(m_szNewValue, NULL);
	m_lblRowData[3][3].SetCaption(m_szNewValue);
}
//_______________
