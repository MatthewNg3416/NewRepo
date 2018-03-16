// MotorSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataActuatorSettingDlg.h"
#include "IAxis.h"
#include "ICylinder.h"
#include "IVacuum.h"
#include "MPlatformOlbSystem.h"
//100807.ECID_____________
#include "MTrsAutoManager.h"
#include "MLCNetData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataActuatorSettingDlg dialog

CDataActuatorSettingDlg::CDataActuatorSettingDlg(int ButtonSel, CWnd* pParent /*=NULL*/)
	: CDialog(CDataActuatorSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataActuatorSettingDlg)
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
	
	m_iButtonSel = ButtonSel;
	m_iPageNum = 0;
	m_iCurrentPage = 0;
	m_iSelectedPage = -1;
	m_iSel = 0;
	m_iOldSel = -1;
	m_bAllInputSelect = FALSE;

	for (int i = 0; i < DEF_DLG_SETTING_MAX_BUFFER; i++)
		for (int j = 0; j < DEF_DLG_MOTOR_SET_MAX_TITLE_NO; j++)
			m_dTempBuf[i][j] = 0.0;

	for (int i = 0; i < DEF_DLG_MAX_PAGE_NO; i++)
		for (int j = 0; j < DEF_DLG_AXIS_BTN; j++)
 			m_bAxisBtnStatus[i][j] = false;

	for (int i = 0; i < DEF_DLG_MAX_PAGE_NO; i++)
		for (int j = 0; j < DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE; j++)
			for (int k = 0; k < DEF_DLG_MOTOR_SET_MAX_TITLE_NO; k++)
				m_bLblValueStatus[i][j][k] = false;
}


void CDataActuatorSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataActuatorSettingDlg)
//	DDX_Control(pDX, IDC_VEL_MODE, m_lblVelMode);
	DDX_Control(pDX, IDB_PREVIOUS, m_btnPrev);
	DDX_Control(pDX, IDB_NEXT, m_btnNext);
	DDX_Control(pDX, IDB_ALL_INPUT, m_btnAllIn);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_TITLE_0, m_btnTitle[0]);
	DDX_Control(pDX, IDB_TITLE_1, m_btnTitle[1]);
	DDX_Control(pDX, IDB_TITLE_2, m_btnTitle[2]);
	DDX_Control(pDX, IDB_TITLE_3, m_btnTitle[3]);
	
	DDX_Control(pDX, IDB_AXIS_0, m_btnAxis[0]);
	DDX_Control(pDX, IDB_AXIS_1, m_btnAxis[1]);
	DDX_Control(pDX, IDB_AXIS_2, m_btnAxis[2]);
	DDX_Control(pDX, IDB_AXIS_3, m_btnAxis[3]);
	DDX_Control(pDX, IDB_AXIS_4, m_btnAxis[4]);
	DDX_Control(pDX, IDB_AXIS_5, m_btnAxis[5]);
	DDX_Control(pDX, IDB_AXIS_6, m_btnAxis[6]);
	DDX_Control(pDX, IDB_AXIS_7, m_btnAxis[7]);
	DDX_Control(pDX, IDB_AXIS_8, m_btnAxis[8]);
	DDX_Control(pDX, IDB_AXIS_9, m_btnAxis[9]);
	DDX_Control(pDX, IDB_AXIS_10, m_btnAxis[10]);
	DDX_Control(pDX, IDB_AXIS_11, m_btnAxis[11]);
	DDX_Control(pDX, IDB_AXIS_12, m_btnAxis[12]);
	DDX_Control(pDX, IDB_AXIS_13, m_btnAxis[13]);
	DDX_Control(pDX, IDB_AXIS_14, m_btnAxis[14]);
	DDX_Control(pDX, IDB_AXIS_15, m_btnAxis[15]);
	
	DDX_Control(pDX, IDC_ITEM_0_0, m_lblValue[0][0]);
	DDX_Control(pDX, IDC_ITEM_0_1, m_lblValue[0][1]);
	DDX_Control(pDX, IDC_ITEM_0_2, m_lblValue[0][2]);
	DDX_Control(pDX, IDC_ITEM_0_3, m_lblValue[0][3]);
	DDX_Control(pDX, IDC_ITEM_1_0, m_lblValue[1][0]);
	DDX_Control(pDX, IDC_ITEM_1_1, m_lblValue[1][1]);
	DDX_Control(pDX, IDC_ITEM_1_2, m_lblValue[1][2]);
	DDX_Control(pDX, IDC_ITEM_1_3, m_lblValue[1][3]);
	DDX_Control(pDX, IDC_ITEM_2_0, m_lblValue[2][0]);
	DDX_Control(pDX, IDC_ITEM_2_1, m_lblValue[2][1]);
	DDX_Control(pDX, IDC_ITEM_2_2, m_lblValue[2][2]);
	DDX_Control(pDX, IDC_ITEM_2_3, m_lblValue[2][3]);
	DDX_Control(pDX, IDC_ITEM_3_0, m_lblValue[3][0]);
	DDX_Control(pDX, IDC_ITEM_3_1, m_lblValue[3][1]);
	DDX_Control(pDX, IDC_ITEM_3_2, m_lblValue[3][2]);
	DDX_Control(pDX, IDC_ITEM_3_3, m_lblValue[3][3]);
	DDX_Control(pDX, IDC_ITEM_4_0, m_lblValue[4][0]);
	DDX_Control(pDX, IDC_ITEM_4_1, m_lblValue[4][1]);
	DDX_Control(pDX, IDC_ITEM_4_2, m_lblValue[4][2]);
	DDX_Control(pDX, IDC_ITEM_4_3, m_lblValue[4][3]);
	DDX_Control(pDX, IDC_ITEM_5_0, m_lblValue[5][0]);
	DDX_Control(pDX, IDC_ITEM_5_1, m_lblValue[5][1]);
	DDX_Control(pDX, IDC_ITEM_5_2, m_lblValue[5][2]);
	DDX_Control(pDX, IDC_ITEM_5_3, m_lblValue[5][3]);
	DDX_Control(pDX, IDC_ITEM_6_0, m_lblValue[6][0]);
	DDX_Control(pDX, IDC_ITEM_6_1, m_lblValue[6][1]);
	DDX_Control(pDX, IDC_ITEM_6_2, m_lblValue[6][2]);
	DDX_Control(pDX, IDC_ITEM_6_3, m_lblValue[6][3]);
	DDX_Control(pDX, IDC_ITEM_7_0, m_lblValue[7][0]);
	DDX_Control(pDX, IDC_ITEM_7_1, m_lblValue[7][1]);
	DDX_Control(pDX, IDC_ITEM_7_2, m_lblValue[7][2]);
	DDX_Control(pDX, IDC_ITEM_7_3, m_lblValue[7][3]);
	DDX_Control(pDX, IDC_ITEM_8_0, m_lblValue[8][0]);
	DDX_Control(pDX, IDC_ITEM_8_1, m_lblValue[8][1]);
	DDX_Control(pDX, IDC_ITEM_8_2, m_lblValue[8][2]);
	DDX_Control(pDX, IDC_ITEM_8_3, m_lblValue[8][3]);
	DDX_Control(pDX, IDC_ITEM_9_0, m_lblValue[9][0]);
	DDX_Control(pDX, IDC_ITEM_9_1, m_lblValue[9][1]);
	DDX_Control(pDX, IDC_ITEM_9_2, m_lblValue[9][2]);
	DDX_Control(pDX, IDC_ITEM_9_3, m_lblValue[9][3]);
	DDX_Control(pDX, IDC_ITEM_10_0, m_lblValue[10][0]);
	DDX_Control(pDX, IDC_ITEM_10_1, m_lblValue[10][1]);
	DDX_Control(pDX, IDC_ITEM_10_2, m_lblValue[10][2]);
	DDX_Control(pDX, IDC_ITEM_10_3, m_lblValue[10][3]);
	DDX_Control(pDX, IDC_ITEM_11_0, m_lblValue[11][0]);
	DDX_Control(pDX, IDC_ITEM_11_1, m_lblValue[11][1]);
	DDX_Control(pDX, IDC_ITEM_11_2, m_lblValue[11][2]);
	DDX_Control(pDX, IDC_ITEM_11_3, m_lblValue[11][3]);
	DDX_Control(pDX, IDC_ITEM_12_0, m_lblValue[12][0]);
	DDX_Control(pDX, IDC_ITEM_12_1, m_lblValue[12][1]);
	DDX_Control(pDX, IDC_ITEM_12_2, m_lblValue[12][2]);
	DDX_Control(pDX, IDC_ITEM_12_3, m_lblValue[12][3]);
	DDX_Control(pDX, IDC_ITEM_13_0, m_lblValue[13][0]);
	DDX_Control(pDX, IDC_ITEM_13_1, m_lblValue[13][1]);
	DDX_Control(pDX, IDC_ITEM_13_2, m_lblValue[13][2]);
	DDX_Control(pDX, IDC_ITEM_13_3, m_lblValue[13][3]);
	DDX_Control(pDX, IDC_ITEM_14_0, m_lblValue[14][0]);
	DDX_Control(pDX, IDC_ITEM_14_1, m_lblValue[14][1]);
	DDX_Control(pDX, IDC_ITEM_14_2, m_lblValue[14][2]);
	DDX_Control(pDX, IDC_ITEM_14_3, m_lblValue[14][3]);
	DDX_Control(pDX, IDC_ITEM_15_0, m_lblValue[15][0]);
	DDX_Control(pDX, IDC_ITEM_15_1, m_lblValue[15][1]);
	DDX_Control(pDX, IDC_ITEM_15_2, m_lblValue[15][2]);
	DDX_Control(pDX, IDC_ITEM_15_3, m_lblValue[15][3]);
}


BEGIN_MESSAGE_MAP(CDataActuatorSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CDataActuatorSettingDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataActuatorSettingDlg message handlers

void CDataActuatorSettingDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDataActuatorSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int i = 0;

	/** Motor Parameter 전체 읽어오기 */
	if (m_iButtonSel < DEF_DLG_CYL_DLY)
	{
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			m_plnkMotion[i] = MOlbSystemPre.GetMotionComponent(i);
			m_plnkMotion[i]->GetParameter(&m_rgsMotorParameter[i]);
			m_strAxisName[i] = m_plnkMotion[i]->GetAxisName();
		}
	}

	switch (m_iButtonSel)
	{
	case DEF_DLG_VEL_ACC :
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			m_dTempBuf[i][0] = m_rgsMotorParameter[i].dRunVelocity;
			m_dTempBuf[i][1] = (double)m_rgsMotorParameter[i].iRunAccelerate;
			m_dTempBuf[i][2] = m_rgsMotorParameter[i].dSlowRunVelocity;
		}
		break;
	case DEF_DLG_ORG_VEL_ACC :
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			m_dTempBuf[i][0] = m_rgsMotorParameter[i].dHomeFastVelocity;
			m_dTempBuf[i][1] = m_rgsMotorParameter[i].dHomeSlowVelocity;
			m_dTempBuf[i][2] = (double)m_rgsMotorParameter[i].iHomeAccelerate;
			m_dTempBuf[i][3] = m_rgsMotorParameter[i].dHomeOffset;
		}
		break;
	case DEF_DLG_LIMIT :
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			m_dTempBuf[i][0] = m_rgsMotorParameter[i].dCWSWLimit;
			m_dTempBuf[i][1] = m_rgsMotorParameter[i].dCCWSWLimit;
		}
		break;
	case DEF_DLG_JOG_PITCH :
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			m_dTempBuf[i][0] = m_rgsMotorParameter[i].dJogSlow;
			m_dTempBuf[i][1] = m_rgsMotorParameter[i].dJogFast;
		}
		break;
	case DEF_DLG_TOLERANCE :
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
			m_dTempBuf[i][0] = m_rgsMotorParameter[i].dTolerance;
		break;
	case DEF_DLG_MTR_DLY :
		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			m_dTempBuf[i][0] = m_rgsMotorParameter[i].dLimitTime;
			m_dTempBuf[i][1] = m_rgsMotorParameter[i].dStabilityTime;
			m_dTempBuf[i][2] = m_rgsMotorParameter[i].dOriginLimitTime;
		}
		break;
	case DEF_DLG_CYL_DLY :
		for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
		{
			m_plnkCylinder[i] = MOlbSystemPre.GetCylinderComponent(i);
			m_plnkCylinder[i]->GetParameter(&m_rgsCylParameter[i]);
			m_strCylinderName[i] = m_plnkCylinder[i]->GetCylinderName();
			
			m_dTempBuf[i][0] = m_rgsCylParameter[i].m_dMovingTime;
			m_dTempBuf[i][1] = m_rgsCylParameter[i].m_dSettlingTime1;
			m_dTempBuf[i][2] = m_rgsCylParameter[i].m_dSettlingTime2;
			m_dTempBuf[i][3] = m_rgsCylParameter[i].m_dNoSenMovingTime;
		}
		break;
	case DEF_DLG_VAC_DLY :
		for (int i = 0; i < DEF_VAC_MAX_VACUUM; i++)
		{
			m_plnkVacuum[i] = MOlbSystemPre.GetVacuumComponent(i);
			m_plnkVacuum[i]->GetParameter(&m_rgsVacParameter[i]);
			m_strVacuumName[i] = m_plnkVacuum[i]->GetVacuumName();
			
			m_dTempBuf[i][0] = m_rgsVacParameter[i].m_dTurningTime;
			m_dTempBuf[i][1] = m_rgsVacParameter[i].m_dOnSettlingTime;
			m_dTempBuf[i][2] = m_rgsVacParameter[i].m_dOffSettlingTime;
		}
		break;
	default:
		break;
	}
	InitialButton();

	// 100807. ECID Test _______
	//InitialDatabase();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_1"), &szTextTemp);
//	이전
	GetDlgItem(IDB_PREVIOUS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_2"), &szTextTemp);
//	다음
	GetDlgItem(IDB_NEXT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_3"), &szTextTemp);
//	일괄입력
	GetDlgItem(IDB_ALL_INPUT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_4"), &szTextTemp);
//	저장
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_5"), &szTextTemp);
//	나가기
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

//	EC Data 생성
	ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_36"), &szTextTemp);
	GetDlgItem(IDB_CREATE_ECDATA)->SetWindowText(szTextTemp);
	
	
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
	{
		GetDlgItem(IDB_TITLE_0)->EnableWindow(FALSE);
		GetDlgItem(IDB_TITLE_1)->EnableWindow(FALSE);
		GetDlgItem(IDB_TITLE_2)->EnableWindow(FALSE);
		GetDlgItem(IDB_TITLE_3)->EnableWindow(FALSE);
		GetDlgItem(IDB_CREATE_ECDATA)->EnableWindow(FALSE);
		GetDlgItem(IDB_ALL_INPUT)->EnableWindow(FALSE);
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	}
	//_______________

	//_________________________
	// 101108 Add. EC Data 개수가 1000개 이상이되어 아래 Data 삭제함
	switch(m_iButtonSel)
	{
	case DEF_DLG_ORG_VEL_ACC:
	case DEF_DLG_LIMIT:
	case DEF_DLG_JOG_PITCH:
		GetDlgItem(IDB_CREATE_ECDATA)->EnableWindow(FALSE);
		break;
	}
	if (MOlbSystemPre.GetCurrentUserGroup() != MAKER_GROUP)
		GetDlgItem(IDB_CREATE_ECDATA)->EnableWindow(FALSE);
	//_________________________
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDataActuatorSettingDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataActuatorSettingDlg)
	ON_EVENT(CDataActuatorSettingDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_ALL_INPUT, -600 /* Click */, OnAllInput, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_PREVIOUS, -600 /* Click */, OnPrevious, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_NEXT, -600 /* Click */, OnNext, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_TITLE_0, -600 /* Click */, OnTitle0, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_TITLE_1, -600 /* Click */, OnTitle1, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_TITLE_2, -600 /* Click */, OnTitle2, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_TITLE_3, -600 /* Click */, OnTitle3, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_0, -600 /* Click */, OnAxis0, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_1, -600 /* Click */, OnAxis1, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_2, -600 /* Click */, OnAxis2, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_3, -600 /* Click */, OnAxis3, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_4, -600 /* Click */, OnAxis4, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_5, -600 /* Click */, OnAxis5, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_6, -600 /* Click */, OnAxis6, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_7, -600 /* Click */, OnAxis7, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_8, -600 /* Click */, OnAxis8, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_9, -600 /* Click */, OnAxis9, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_10, -600 /* Click */, OnAxis10, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_11, -600 /* Click */, OnAxis11, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_12, -600 /* Click */, OnAxis12, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_13, -600 /* Click */, OnAxis13, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_14, -600 /* Click */, OnAxis14, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_AXIS_15, -600 /* Click */, OnAxis15, VTS_NONE)
	ON_EVENT(CDataActuatorSettingDlg, IDB_CREATE_ECDATA, -600 /* Click */, OnClickCreateEcdata, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CDataActuatorSettingDlg::TitleSelect(int iSel)
{
	WCHAR cAfterVal[100];
	WCHAR cBeforeVal[100];
	CString sBuf;
	BOOL bGroupInput = FALSE;

	memset(cAfterVal, 0, sizeof(cAfterVal));
	int iAxisID = (DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE * m_iCurrentPage) + m_iSel;
	
	double dPrevVal = m_dTempBuf[iAxisID][iSel];
	
	if ((m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 1) || (m_iButtonSel == DEF_DLG_ORG_VEL_ACC && iSel == 2))
		swprintf(cBeforeVal, _T("%d"), (int)dPrevVal);
	else
		swprintf(cBeforeVal, _T("%.3f"), dPrevVal);
	
	if (!GetNumberBox(cAfterVal, 10, cBeforeVal))
		return;
	
	if (m_bAllInputSelect)
	{
		for (int i = 0; i < DEF_DLG_SETTING_MAX_BUFFER; i++)
		{
			// 자리수 설정
			if ((m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 1) || (m_iButtonSel == DEF_DLG_ORG_VEL_ACC && iSel == 2))
			{
				m_dTempBuf[i][iSel] = _wtoi(cAfterVal);
				sBuf.Format(_T("%d"), _wtoi(cAfterVal));
			}
			else
			{
				m_dTempBuf[i][iSel] = wcstod(cAfterVal, NULL);
				sBuf.Format(_T("%.3f"), wcstod(cAfterVal, NULL));
			}

			// 속도 제한 설정
			if (m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 2)
			{
				// 확인 필요 : 50 이하로만 사용해야 하는 축들도 있음. 이에 대한 처리 추가 필요.

				// 250mm/s 초과 설정 금지
				if (m_dTempBuf[i][iSel] > 250
					&& i != DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T
					&& i != DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T
					&& i != DEF_AXIS_SERCOS_TAB_PRESS_Z
					&& i != DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T
					&& i != DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T
					&& i != DEF_AXIS_SERCOS_R_TAB_PRESS_Z					
					) 
				{
					m_dTempBuf[i][iSel] = 250;
					sBuf.Format(_T("%.3f"), m_dTempBuf[i][iSel]);
				}
			}

			// Gantry축 가감속 시간 300 ms 미만 설정 금지
			if ((m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 1) || (m_iButtonSel == DEF_DLG_ORG_VEL_ACC && iSel == 2))
			{
				if ((i == DEF_AXIS_ACS_GANTRY_Y || i == DEF_AXIS_ACS_R_GANTRY_Y)
					&& m_dTempBuf[i][iSel] < 200) 
				{
					m_dTempBuf[i][iSel] = 200;
					sBuf.Format(_T("%.3f"), m_dTempBuf[i][iSel]);
				}
			}

			if (i < DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE)
			{
				m_lblValue[i % DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE][iSel].SetCaption(sBuf);
				m_lblValue[i % DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE][iSel].SetForeColor(DATA_ITEM_CHANGE_TEXT_COLOR);
				m_lblValue[i % DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE][iSel].SetBackColor(DATA_ITEM_CHANGE_BACKGROUND_COLOR);
				m_bLblValueStatus[m_iCurrentPage][i % DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE][iSel] = true;
			}
		}
	}
	else
	{
		// 자리수 설정
		if ((m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 1) || (m_iButtonSel == DEF_DLG_ORG_VEL_ACC && iSel == 2))
		{
			m_dTempBuf[iAxisID][iSel] = _wtoi(cAfterVal);
			sBuf.Format(_T("%d"), _wtoi(cAfterVal));
		}
		else
		{
			m_dTempBuf[iAxisID][iSel] = wcstod(cAfterVal, NULL);
			sBuf.Format(_T("%.3f"), wcstod(cAfterVal, NULL));
		}

		// 속도 제한 설정
		if (m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 2)
		{
			// 250mm/s 초과 설정 금지
			if (m_dTempBuf[iAxisID][iSel] > 2500.
				&& iAxisID != DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T
				&& iAxisID != DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T
				&& iAxisID != DEF_AXIS_SERCOS_TAB_PRESS_Z
				&& iAxisID != DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T
				&& iAxisID != DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T
				&& iAxisID != DEF_AXIS_SERCOS_R_TAB_PRESS_Z				
				) 
			{
				m_dTempBuf[iAxisID][iSel] = 250.;
				sBuf.Format(_T("%.3f"), m_dTempBuf[iAxisID][iSel]);
			}
		}

		// 속도 변경이 같이 이루어져야 하는 Axis Group 처리.
		//161221 JSH.s Modify..
		//if (m_iButtonSel == DEF_DLG_VEL_ACC || (m_iButtonSel == DEF_DLG_ORG_VEL_ACC && iSel == 2))
		if (TRUE)
		//161221 JSH.e
		{
			BOOL rgbChangeSts[DEF_DLG_SETTING_MAX_BUFFER];
			memset(rgbChangeSts, FALSE, sizeof(rgbChangeSts));

			if (m_iButtonSel == DEF_DLG_VEL_ACC || m_iButtonSel == DEF_DLG_JOG_PITCH || (m_iButtonSel == DEF_DLG_ORG_VEL_ACC && iSel == 2))
			{
				
				// Panel Aligner
				if (iAxisID == DEF_AXIS_SERCOS_STAGE_Y || iAxisID == DEF_AXIS_SERCOS_STAGE_T)
				{
					m_dTempBuf[DEF_AXIS_SERCOS_STAGE_Y][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_STAGE_Y] = TRUE;
					m_dTempBuf[DEF_AXIS_SERCOS_STAGE_T][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_STAGE_T] = TRUE;
					bGroupInput = TRUE;
				}
				
				//120522.KMS_______________
#ifdef DEF_SOURCE_SYSTEM
				// 속도 제한 설정
				if (m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 0)
				{
					if (iAxisID == DEF_AXIS_SERCOS_TRANSFER_IN || iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT) 
					{
						// 400mm/s 초과 설정 금지
						if (wcstod(cAfterVal,NULL) > 800.00) 
						{
							MyMessageBox(_T("Transfer Unit can not input more than 600.\r\nTransfer Unit는 600이상 입력 불가 합니다."), _T("Vel Force Setting"), M_ICONERROR, _T("확인"));
							return;
						}
					}
				}			
#endif
				
#ifdef DEF_GATE_SYSTEM
				// 속도 제한 설정
				if (m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 0)
				{
					if (iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT)
					{
						// 400mm/s 초과 설정 금지
						if (wcstod(cAfterVal,NULL) > 800.00) 
						{
							MyMessageBox(_T("Transfer Unit can not input more than 400.\r\nTransfer Unit는 400이상 입력 불가 합니다."), _T("Vel Force Setting"), M_ICONERROR, _T("확인"));
							return;
						}
					}
				}
#endif
				//_________________________
				
				// Panel Transfer
				//#ifdef DEF_SOURCE_SYSTEM
				//			if (iAxisID == DEF_AXIS_SERCOS_TRANSFER_IN)
				//			{
				//				m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_IN][iSel] = m_dTempBuf[iAxisID][iSel];
				//				rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_IN] = TRUE;
				//				m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_OUT][iSel] = m_dTempBuf[iAxisID][iSel];
				//				rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_OUT] = TRUE;
				//				bGroupInput = TRUE;
				//			}
				//
				//			if (iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT)
				//			{
				//				m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_OUT][iSel] = m_dTempBuf[iAxisID][iSel];
				//				rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_OUT] = TRUE;
				//				m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_IN][iSel] = m_dTempBuf[iAxisID][iSel];
				//				rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_IN] = TRUE;
				//				bGroupInput = TRUE;
				//			}
				//#endif
				
				// Sprocket1,2
				if (iAxisID == DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T || iAxisID == DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T)
				{
					m_dTempBuf[DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T] = TRUE;
					m_dTempBuf[DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T] = TRUE;
					bGroupInput = TRUE;
				}
				
				if (iAxisID == DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T	|| iAxisID == DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T)
				{
					m_dTempBuf[DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T] = TRUE;
					m_dTempBuf[DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T] = TRUE;
					bGroupInput = TRUE;
				}
				
#ifdef DEF_SOURCE_SYSTEM
				if (iAxisID >= DEF_AXIS_SERCOS_MOUNTER1_X && iAxisID <= DEF_AXIS_SERCOS_MOUNTER4_X)
#else
					if (iAxisID >= DEF_AXIS_SERCOS_MOUNTER1_X && iAxisID <= DEF_AXIS_SERCOS_MOUNTER2_X)
#endif
					{
						for (int j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
						{
							m_dTempBuf[DEF_AXIS_SERCOS_MOUNTER1_X + j][iSel] = m_dTempBuf[iAxisID][iSel];
							rgbChangeSts[DEF_AXIS_SERCOS_MOUNTER1_X + j] = TRUE;
						}
						bGroupInput = TRUE;
					}
					
#ifdef DEF_SOURCE_SYSTEM
					if (iAxisID >= DEF_AXIS_SERCOS_R_MOUNTER1_X && iAxisID <= DEF_AXIS_SERCOS_R_MOUNTER4_X) 
#else
						if (iAxisID >= DEF_AXIS_SERCOS_R_MOUNTER1_X && iAxisID <= DEF_AXIS_SERCOS_R_MOUNTER2_X) 
#endif
						{
							for (int j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
							{
								m_dTempBuf[DEF_AXIS_SERCOS_R_MOUNTER1_X + j][iSel] = m_dTempBuf[iAxisID][iSel];
								rgbChangeSts[DEF_AXIS_SERCOS_R_MOUNTER1_X + j] = TRUE;
							}
							bGroupInput = TRUE;
						}
						
#ifdef DEF_SOURCE_SYSTEM
						if (iAxisID >= DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X && iAxisID <= DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X)
#else
							if (iAxisID >= DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X && iAxisID <= DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X)
#endif
							{
								for (int j = 0; j < DEF_MAX_INSPECTION_CAM_PER_GROUP; j++)
								{
									m_dTempBuf[DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X + j][iSel] = m_dTempBuf[iAxisID][iSel];
									rgbChangeSts[DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X + j] = TRUE;
								}
								bGroupInput = TRUE;
							}
							
#ifdef DEF_SOURCE_SYSTEM
							if (iAxisID >= DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X && iAxisID <= DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X)
#else
								if (iAxisID >= DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X && iAxisID <= DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X)
#endif
								{
									for (int j = 0; j < DEF_MAX_INSPECTION_CAM_PER_GROUP; j++)
									{
										m_dTempBuf[DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X + j][iSel] = m_dTempBuf[iAxisID][iSel];
										rgbChangeSts[DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X + j] = TRUE;
									}
									bGroupInput = TRUE;
								}
								
#ifdef DEF_SOURCE_SYSTEM
								if (iAxisID >= DEF_AXIS_SERCOS_TABIC_CARRIER1_Y && iAxisID <= DEF_AXIS_SERCOS_TABIC_CARRIER4_Y)
#else
									if (iAxisID >= DEF_AXIS_SERCOS_TABIC_CARRIER1_Y && iAxisID <= DEF_AXIS_SERCOS_TABIC_CARRIER2_Y)
#endif
									{
										for (int j = 0; j < DEF_MAX_TABCARRIER; j++)
										{
											m_dTempBuf[DEF_AXIS_SERCOS_TABIC_CARRIER1_Y + j][iSel] = m_dTempBuf[iAxisID][iSel];
											rgbChangeSts[DEF_AXIS_SERCOS_TABIC_CARRIER1_Y + j] = TRUE;
										}
										bGroupInput = TRUE;
									}
									
#ifdef DEF_SOURCE_SYSTEM
									if (iAxisID >= DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y && iAxisID <= DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y)
#else
										if (iAxisID >= DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y && iAxisID <= DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y)
#endif
										{
											for (int j = 0; j < DEF_MAX_TABCARRIER; j++)
											{
												m_dTempBuf[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y + j][iSel] = m_dTempBuf[iAxisID][iSel];
												rgbChangeSts[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y + j] = TRUE;
											}
											bGroupInput = TRUE;
										}
			}

			//170928 JSh.s Gear
			if ( m_iButtonSel == DEF_DLG_VEL_ACC
				|| m_iButtonSel == DEF_DLG_JOG_PITCH
				|| m_iButtonSel == DEF_DLG_LIMIT
				|| m_iButtonSel == DEF_DLG_TOLERANCE
				|| m_iButtonSel == DEF_DLG_MTR_DLY
				)
			{	
				//170929 KDW Start Gear
				if ( iAxisID == DEF_AXIS_SERCOS_TRANSFER_IN || iAxisID == DEF_AXIS_SERCOS_TRANSFER_IN_2)
				{
					m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_IN][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_IN] = TRUE;
					m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_IN_2][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_IN_2] = TRUE;
					bGroupInput = TRUE;
				}
				//170929 KDW End

#ifdef DEF_1ST_EQ
				// Panel Transfer
				if ( iAxisID == DEF_AXIS_SERCOS_TRANSFER_CENTER_1 || iAxisID == DEF_AXIS_SERCOS_TRANSFER_CENTER_2)
				{
					m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_CENTER_1][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_CENTER_1] = TRUE;
					m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_CENTER_2][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_CENTER_2] = TRUE;
					bGroupInput = TRUE;
				}
				if ( iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT || iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT_2)
				{
					m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_OUT][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_OUT] = TRUE;
					m_dTempBuf[DEF_AXIS_SERCOS_TRANSFER_OUT_2][iSel] = m_dTempBuf[iAxisID][iSel];
					rgbChangeSts[DEF_AXIS_SERCOS_TRANSFER_OUT_2] = TRUE;
					bGroupInput = TRUE;
				}
#endif
			}
			//170928 JSh.e


			int iAxisNoPerPage = -1;
			for (int k = 0; k < DEF_DLG_SETTING_MAX_BUFFER; k++)
			{
				if (FALSE == rgbChangeSts[k])
					continue;
				
				if (m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 1 || m_iButtonSel == DEF_DLG_ORG_VEL_ACC && iSel == 2)
					sBuf.Format(_T("%d"), static_cast<int>(m_dTempBuf[k][iSel]));
				else
					sBuf.Format(_T("%.3f"), m_dTempBuf[k][iSel]);
				
				iAxisNoPerPage = k - (DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE * m_iCurrentPage);

				m_bLblValueStatus[m_iCurrentPage][iAxisNoPerPage][iSel] = true;

				if (iAxisNoPerPage >= DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE)
					continue;
				
				m_lblValue[iAxisNoPerPage][iSel].SetCaption(sBuf);
				m_lblValue[iAxisNoPerPage][iSel].SetForeColor(DATA_ITEM_CHANGE_TEXT_COLOR);
				m_lblValue[iAxisNoPerPage][iSel].SetBackColor(DATA_ITEM_CHANGE_BACKGROUND_COLOR);
			}
		}

		// Gantry 축 속도 관련 제한 : 가속도 300 msec 미만 설정 금지
		if ((m_iButtonSel == DEF_DLG_VEL_ACC && iSel == 1) || (m_iButtonSel == DEF_DLG_ORG_VEL_ACC && iSel == 2))
		{
			if ((iAxisID == DEF_AXIS_ACS_GANTRY_Y || iAxisID == DEF_AXIS_ACS_R_GANTRY_Y)
				&& m_dTempBuf[iAxisID][iSel] < 200) 
			{
				m_dTempBuf[iAxisID][iSel] = 200;
				sBuf.Format(_T("%.3f"), m_dTempBuf[iAxisID][iSel]);
			}
		}

		m_lblValue[m_iSel][iSel].SetCaption(sBuf);
		m_lblValue[m_iSel][iSel].SetForeColor(DATA_ITEM_CHANGE_TEXT_COLOR);
		m_lblValue[m_iSel][iSel].SetBackColor(DATA_ITEM_CHANGE_BACKGROUND_COLOR);
		m_bLblValueStatus[m_iCurrentPage][m_iSel][iSel] = true;
	}

	switch (m_iButtonSel)
	{
	case DEF_DLG_VEL_ACC :
		if (TRUE == m_bAllInputSelect || TRUE == bGroupInput)
		{
			for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
			{
				if (iSel == 0)		m_rgsMotorParameter[i].dRunVelocity		= m_dTempBuf[i][iSel];
				else if (iSel == 1)	m_rgsMotorParameter[i].iRunAccelerate	= (int)m_dTempBuf[i][iSel];
				else				m_rgsMotorParameter[i].dSlowRunVelocity	= m_dTempBuf[i][iSel];
			}
		}
		else
		{
			if (iSel == 0)		
			{
				m_rgsMotorParameter[iAxisID].dRunVelocity = m_dTempBuf[iAxisID][iSel];

				if (iAxisID == 0)
				{
					m_rgsMotorParameter[1].dRunVelocity = m_dTempBuf[1][iSel];
					m_rgsMotorParameter[2].dRunVelocity = m_dTempBuf[2][iSel];
				}
				else if (iAxisID == 1)
				{
					m_rgsMotorParameter[0].dRunVelocity = m_dTempBuf[0][iSel];
					m_rgsMotorParameter[2].dRunVelocity = m_dTempBuf[2][iSel];
				}
				else if (iAxisID == 2)
				{
					m_rgsMotorParameter[0].dRunVelocity = m_dTempBuf[0][iSel];
					m_rgsMotorParameter[1].dRunVelocity = m_dTempBuf[1][iSel];
				}
			}
			else if (iSel == 1)
			{
				m_rgsMotorParameter[iAxisID].iRunAccelerate	= (int)m_dTempBuf[iAxisID][iSel];
				
				if (iAxisID == 0)
				{
					m_rgsMotorParameter[1].iRunAccelerate = (int)m_dTempBuf[1][iSel];
					m_rgsMotorParameter[2].iRunAccelerate = (int)m_dTempBuf[2][iSel];
				}
				else if (iAxisID == 1)
				{
					m_rgsMotorParameter[0].iRunAccelerate = (int)m_dTempBuf[0][iSel];
					m_rgsMotorParameter[2].iRunAccelerate = (int)m_dTempBuf[2][iSel];
				}
				else if (iAxisID == 2)
				{
					m_rgsMotorParameter[0].iRunAccelerate = (int)m_dTempBuf[0][iSel];
					m_rgsMotorParameter[1].iRunAccelerate = (int)m_dTempBuf[1][iSel];
				}
			}
			else
			{
				m_rgsMotorParameter[iAxisID].dSlowRunVelocity	= m_dTempBuf[iAxisID][iSel];
				
				if (iAxisID == 0)
				{
					m_rgsMotorParameter[1].dSlowRunVelocity = m_dTempBuf[1][iSel];
					m_rgsMotorParameter[2].dSlowRunVelocity = m_dTempBuf[2][iSel];
				}
				else if (iAxisID == 1)
				{
					m_rgsMotorParameter[0].dSlowRunVelocity = m_dTempBuf[0][iSel];
					m_rgsMotorParameter[2].dSlowRunVelocity = m_dTempBuf[2][iSel];
				}
				else if (iAxisID == 2)
				{
					m_rgsMotorParameter[0].dSlowRunVelocity = m_dTempBuf[0][iSel];
					m_rgsMotorParameter[1].dSlowRunVelocity = m_dTempBuf[1][iSel];
				}
			}
		}
		break;

	case DEF_DLG_ORG_VEL_ACC :
		if (TRUE == m_bAllInputSelect || TRUE == bGroupInput)
		{
			for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
			{
				if (iSel == 0)		m_rgsMotorParameter[i].dHomeFastVelocity	= m_dTempBuf[i][iSel];
				else if (iSel == 1)	m_rgsMotorParameter[i].dHomeSlowVelocity	= m_dTempBuf[i][iSel];
				else if (iSel == 2)	m_rgsMotorParameter[i].iHomeAccelerate		= (int)m_dTempBuf[i][iSel];
				else				m_rgsMotorParameter[i].dHomeOffset			= m_dTempBuf[i][iSel];
			}
		}
		else
		{
			if (iSel == 0)		m_rgsMotorParameter[iAxisID].dHomeFastVelocity	= m_dTempBuf[iAxisID][iSel];
			else if (iSel == 1)	m_rgsMotorParameter[iAxisID].dHomeSlowVelocity	= m_dTempBuf[iAxisID][iSel];
			else if (iSel == 2)	m_rgsMotorParameter[iAxisID].iHomeAccelerate	= (int)m_dTempBuf[iAxisID][iSel];
			else				m_rgsMotorParameter[iAxisID].dHomeOffset		= m_dTempBuf[iAxisID][iSel];
		}
		break;

	case DEF_DLG_LIMIT :
		if (TRUE == m_bAllInputSelect || TRUE == bGroupInput)
		{
			for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
			{
				if (iSel == 0)		m_rgsMotorParameter[i].dCWSWLimit	= m_dTempBuf[i][iSel];
				else				m_rgsMotorParameter[i].dCCWSWLimit	= m_dTempBuf[i][iSel];
			}
		}
		else
		{
			if (iSel == 0)		m_rgsMotorParameter[iAxisID].dCWSWLimit		= m_dTempBuf[iAxisID][iSel];
			else				m_rgsMotorParameter[iAxisID].dCCWSWLimit	= m_dTempBuf[iAxisID][iSel];
		}
		break;

	case DEF_DLG_JOG_PITCH :
		if (TRUE == m_bAllInputSelect || TRUE == bGroupInput)
		{
			for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
			{
				if(iSel == 0)		m_rgsMotorParameter[i].dJogSlow	= m_dTempBuf[i][iSel];
				else				m_rgsMotorParameter[i].dJogFast	= m_dTempBuf[i][iSel];
			}
		}
		else
		{
			if(iSel == 0)		m_rgsMotorParameter[iAxisID].dJogSlow	= m_dTempBuf[iAxisID][iSel];
			else				m_rgsMotorParameter[iAxisID].dJogFast	= m_dTempBuf[iAxisID][iSel];
		}
		break;

	case DEF_DLG_TOLERANCE :
		if (TRUE == m_bAllInputSelect || TRUE == bGroupInput)
		{
			for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
			{
				m_rgsMotorParameter[i].dTolerance	= m_dTempBuf[i][iSel];
			}
		}
		else
		{
			m_rgsMotorParameter[iAxisID].dTolerance	= m_dTempBuf[iAxisID][iSel];
		}
		break;

	case DEF_DLG_MTR_DLY :
		if (TRUE == m_bAllInputSelect || TRUE == bGroupInput)
		{
			for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
			{
				if (iSel == 0)		m_rgsMotorParameter[i].dLimitTime			= m_dTempBuf[i][iSel];
				else if (iSel == 1)	m_rgsMotorParameter[i].dStabilityTime		= m_dTempBuf[i][iSel];
				else				m_rgsMotorParameter[i].dOriginLimitTime		= m_dTempBuf[i][iSel];
			}
		}
		else
		{
			if (iSel == 0)		m_rgsMotorParameter[iAxisID].dLimitTime			= m_dTempBuf[iAxisID][iSel];
			else if (iSel == 1)	m_rgsMotorParameter[iAxisID].dStabilityTime		= m_dTempBuf[iAxisID][iSel];
			else				m_rgsMotorParameter[iAxisID].dOriginLimitTime	= m_dTempBuf[iAxisID][iSel];
		}
		break;
	case DEF_DLG_CYL_DLY :
		if (TRUE == m_bAllInputSelect || TRUE == bGroupInput)
		{
			for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
			{
				if (iSel == 0)		m_rgsCylParameter[i].m_dMovingTime			= m_dTempBuf[i][iSel];
				else if (iSel == 1)	m_rgsCylParameter[i].m_dSettlingTime1		= m_dTempBuf[i][iSel];
				else if (iSel == 2)	m_rgsCylParameter[i].m_dSettlingTime2		= m_dTempBuf[i][iSel];
				else				m_rgsCylParameter[i].m_dNoSenMovingTime		= m_dTempBuf[i][iSel];
			}
		}
		else
		{
			if (iSel == 0)		m_rgsCylParameter[iAxisID].m_dMovingTime		= m_dTempBuf[iAxisID][iSel];
			else if (iSel == 1)	m_rgsCylParameter[iAxisID].m_dSettlingTime1		= m_dTempBuf[iAxisID][iSel];
			else if (iSel == 2)	m_rgsCylParameter[iAxisID].m_dSettlingTime2		= m_dTempBuf[iAxisID][iSel];
			else				m_rgsCylParameter[iAxisID].m_dNoSenMovingTime	= m_dTempBuf[iAxisID][iSel];
		}
		break;
	case DEF_DLG_VAC_DLY :
		if (TRUE == m_bAllInputSelect || TRUE == bGroupInput)
		{
			for (int i = 0; i < DEF_VAC_MAX_VACUUM; i++)
			{
				if (iSel == 0)		m_rgsVacParameter[i].m_dTurningTime			= m_dTempBuf[i][iSel];
				else if (iSel == 1)	m_rgsVacParameter[i].m_dOnSettlingTime		= m_dTempBuf[i][iSel];
				else				m_rgsVacParameter[i].m_dOffSettlingTime		= m_dTempBuf[i][iSel];
			}
		}
		else
		{
			if (iSel == 0)		m_rgsVacParameter[iAxisID].m_dTurningTime		= m_dTempBuf[iAxisID][iSel];
			else if (iSel == 1)	m_rgsVacParameter[iAxisID].m_dOnSettlingTime	= m_dTempBuf[iAxisID][iSel];
			else				m_rgsVacParameter[iAxisID].m_dOffSettlingTime	= m_dTempBuf[iAxisID][iSel];
		}
		break;
	default:
		break;
	}

	//100807.ECID_____________
	m_iaChangedItem.Add((DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE * m_iCurrentPage) + m_iSel);
}

void CDataActuatorSettingDlg::axisSelect(int iSel)
{
	ASSERT(iSel >= 0 && iSel < DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE);

	m_iSel = iSel;
	m_iSelectedPage = m_iCurrentPage;

 	for (int i = 0; i < DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE; i++)
	{
		if (i == iSel)
		{
			m_bAxisBtnStatus[m_iCurrentPage][i] = true;
			if(m_btnAxis[i].IsWindowEnabled() && m_btnAxis[i].IsWindowVisible()) //170911 JSH
				m_btnAxis[i].SetValue(TRUE);
		}
		else
		{
			m_bAxisBtnStatus[m_iCurrentPage][i] = false;
			m_btnAxis[i].SetValue(FALSE);
		}
	}
}

void CDataActuatorSettingDlg::UpdateDisplay()
{
	// Prev, Next Button 처리
	if ((m_iPageNum - 1) == m_iCurrentPage)
		m_btnNext.ShowWindow(SW_HIDE);
	else
		m_btnNext.ShowWindow(SW_SHOW);

	if (m_iCurrentPage == 0)
		m_btnPrev.ShowWindow(SW_HIDE);
	else
		m_btnPrev.ShowWindow(SW_SHOW);
	
	// Axis, Cylinder, Vacuum Name 처리
	switch (m_iButtonSel)
	{
	case DEF_DLG_VEL_ACC:
		displayTable(DEF_AXIS_MAX_AXIS, m_strAxisName, 1);
		break;

	case DEF_DLG_ORG_VEL_ACC:
		displayTable(DEF_AXIS_MAX_AXIS, m_strAxisName, 2);
		break;

	case DEF_DLG_LIMIT:
		/** Fall Through */
	case DEF_DLG_JOG_PITCH:
		/** Fall Through */
	case DEF_DLG_TOLERANCE:
		/** Fall Through */
	case DEF_DLG_MTR_DLY:
		displayTable(DEF_AXIS_MAX_AXIS, m_strAxisName);
		break;

	case DEF_DLG_CYL_DLY:
		displayTable(DEF_CYL_MAX_CYLINDER, m_strCylinderName);
		break;

	case DEF_DLG_VAC_DLY:
		displayTable(DEF_VAC_MAX_VACUUM, m_strVacuumName);
		break;
	}

	// Label 처리
	for (int i = 0; i < DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE; i++)	
	{
		for (int j = 0; j < DEF_DLG_MOTOR_SET_MAX_TITLE_NO; j++)
		{
			if (DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE * m_iCurrentPage + i < m_iRowNum && j < m_iColumnNum)	
				m_lblValue[i][j].ShowWindow(SW_SHOW);
			else	
				m_lblValue[i][j].ShowWindow(SW_HIDE);
		}
	}
}

void CDataActuatorSettingDlg::displayTable(int iUnitLimit, CString* pstrUnitName, int iDecimalColumn)
{
	int iUnitID;
	CString sBuf;
	for (int i = 0; i < DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE; i++)	
	{
		if ((iUnitID = ((DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE * m_iCurrentPage) + i)) < iUnitLimit)
		{
			m_btnAxis[i].SetWindowText(pstrUnitName[iUnitID]);
			m_btnAxis[i].ShowWindow(SW_SHOW);
			m_btnAxis[i].EnableWindow(TRUE);
			
			for (int j = 0; j < m_iColumnNum; j++)
			{
				if (j == iDecimalColumn)
					sBuf.Format(_T("%d"), (int)m_dTempBuf[(DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE * m_iCurrentPage) + i][j]);
				else
					sBuf.Format(_T("%.3f"), m_dTempBuf[(DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE * m_iCurrentPage) + i][j]);
				
				if (m_bLblValueStatus[m_iCurrentPage][i][j])
				{
					m_lblValue[i][j].SetCaption(sBuf);
					m_lblValue[i][j].SetForeColor(DATA_ITEM_CHANGE_TEXT_COLOR);
					m_lblValue[i][j].SetBackColor(DATA_ITEM_CHANGE_BACKGROUND_COLOR);
				}
				else
				{
					m_lblValue[i][j].SetCaption(sBuf);
					m_lblValue[i][j].SetForeColor(DATA_ITEM_TEXT_COLOR);
					m_lblValue[i][j].SetBackColor(BASIC);
				}
			}
		}
		else	
			m_btnAxis[i].ShowWindow(SW_HIDE);
	}
}

void CDataActuatorSettingDlg::InitialButton()
{
	CString szTextTemp;
	// 9개의 Dialog를 공동으로 사용함.
	switch (m_iButtonSel)
	{
	case DEF_DLG_VEL_ACC :
		m_iColumnNum = 3;
		m_iRowNum = DEF_AXIS_MAX_AXIS;
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_6"), &szTextTemp);
//	Motor 속도 / 가속도 Setting Dialog
		SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_7"), &szTextTemp);
//	Motor\r\n속도
		m_btnTitle[0].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_8"), &szTextTemp);
//	Motor\r\n가감속 시간
		m_btnTitle[1].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_9"), &szTextTemp);
//	Motor\r\nSlow 속도
		m_btnTitle[2].SetWindowText(szTextTemp);
		break;

	case DEF_DLG_ORG_VEL_ACC :
		m_iColumnNum = 4;
		m_iRowNum = DEF_AXIS_MAX_AXIS;
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_10"), &szTextTemp);
//	Motor 원점복귀 속도 / 가속도 Setting Dialog
		SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_11"), &szTextTemp);
//	Origin\r\nFast
		m_btnTitle[0].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_12"), &szTextTemp);
//	Origin\r\nSlow
		m_btnTitle[1].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_13"), &szTextTemp);
//	원점복귀\r\n가감속 시간
		m_btnTitle[2].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_14"), &szTextTemp);
//	원점복귀\r\nOffset
		m_btnTitle[3].SetWindowText(szTextTemp);
		break;
		
	case DEF_DLG_JOG_PITCH :
		m_iColumnNum = 2;
		m_iRowNum = DEF_AXIS_MAX_AXIS;
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_15"), &szTextTemp);
//	Jog 속도 Setting Dialog
		SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_16"), &szTextTemp);
//	저속 이동\r\n속도
		m_btnTitle[0].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_17"), &szTextTemp);
//	고속 이동\r\n속도
		m_btnTitle[1].SetWindowText(szTextTemp);			
		break;
		
	case DEF_DLG_LIMIT :
		m_iColumnNum = 2;
		m_iRowNum = DEF_AXIS_MAX_AXIS;
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_18"), &szTextTemp);
//	Motor S/W Limit Setting Dialog
		SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_19"), &szTextTemp);
//	+ Limit
		m_btnTitle[0].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_20"), &szTextTemp);
//	- Limit
		m_btnTitle[1].SetWindowText(szTextTemp);			
		break;
		
	case DEF_DLG_TOLERANCE :
		m_iColumnNum = 1;
		m_iRowNum = DEF_AXIS_MAX_AXIS;
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_21"), &szTextTemp);
//	Tolerance Setting Dialog
		SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_22"), &szTextTemp);
//	Tolerace
		m_btnTitle[0].SetWindowText(szTextTemp);
		break;
		
	case DEF_DLG_MTR_DLY :
		m_iColumnNum = 3;
		m_iRowNum = DEF_AXIS_MAX_AXIS;
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_23"), &szTextTemp);
//	Motor 지연 시간 설정 Dialog
		SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_24"), &szTextTemp);
//	동작\r\n제한 시간
		m_btnTitle[0].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_25"), &szTextTemp);
//	동작 후\r\n대기 시간
		m_btnTitle[1].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_26"), &szTextTemp);
//	원점복귀\r\n제한 시간
		m_btnTitle[2].SetWindowText(szTextTemp);
		break;
		
	case DEF_DLG_CYL_DLY :
		m_iColumnNum = 4;
		m_iRowNum = DEF_CYL_MAX_CYLINDER;
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_27"), &szTextTemp);
//	Cylinder 지연 시간 설정 Dialog
		SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_28"), &szTextTemp);
//	동작\r\n제한 시간
		m_btnTitle[0].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_29"), &szTextTemp);
//	동작 후\r\n대기 시간1
		m_btnTitle[1].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_30"), &szTextTemp);
//	동작 후\r\n대기 시간2
		m_btnTitle[2].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_31"), &szTextTemp);
//	No Sensor\r\n대기 시간		
		m_btnTitle[3].SetWindowText(szTextTemp);			
		break;
		
	case DEF_DLG_VAC_DLY :
		m_iColumnNum = 3;
		m_iRowNum = DEF_VAC_MAX_VACUUM;
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_32"), &szTextTemp);
//	Vacuum 지연 시간 설정 Dialog
		SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_33"), &szTextTemp);
//	동작\r\n제한 시간
		m_btnTitle[0].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_34"), &szTextTemp);
//	Vacuum On 후\r\n대기 시간
		m_btnTitle[1].SetWindowText(szTextTemp);
		ReadDisplayMessage(MAIN_DATA, _T("DataActuatorSettingDlg_35"), &szTextTemp);
//	Vacuum Off 후\r\n대기 시간
		m_btnTitle[2].SetWindowText(szTextTemp);	
		break;
		
	}
	
	// 필요한 만큼의 Title만 보이기
	for (int i = 0 ; i < DEF_DLG_MOTOR_SET_MAX_TITLE_NO ; i++)	
	{
		if (i < m_iColumnNum)	
			m_btnTitle[i].ShowWindow(SW_SHOW);
		else	
			m_btnTitle[i].ShowWindow(SW_HIDE);
	}
	/* jdy sesl 081229
	m_iPageNum = (m_iRowNum / DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE)
				+ (m_iRowNum % DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE == 0 ? (-1) : 0);
	*/
//@	m_iPageNum = m_iRowNum / (DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE + 1);
	m_iPageNum = ((m_iRowNum-1) / DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE) + 1;

	UpdateDisplay();
}

void CDataActuatorSettingDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CDataActuatorSettingDlg::OnSave() 
{
	//	저장하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40000, _T("Data Save"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	//171003 JSh.s
	if(m_iButtonSel == DEF_DLG_TOLERANCE)
	{
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER1_X].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_X)
			MyMessageBox(MY_MESSAGE_BOX, 40001, _T("Tolerance"), M_ICONQUESTION);
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER1_Y].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_Y)
			MyMessageBox(MY_MESSAGE_BOX, 40002, _T("Tolerance"), M_ICONQUESTION);
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER1_T].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_T)
			MyMessageBox(MY_MESSAGE_BOX, 40003, _T("Tolerance"), M_ICONQUESTION);
		
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER2_X].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_X)
			MyMessageBox(MY_MESSAGE_BOX, 40004, _T("Tolerance"), M_ICONQUESTION);
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER2_Y].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_Y)
			MyMessageBox(MY_MESSAGE_BOX, 40005, _T("Tolerance"), M_ICONQUESTION);
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER2_T].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_T)
			MyMessageBox(MY_MESSAGE_BOX, 40006, _T("Tolerance"), M_ICONQUESTION);
		
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER3_X].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_X)
			MyMessageBox(MY_MESSAGE_BOX, 40007, _T("Tolerance"), M_ICONQUESTION);
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER3_Y].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_Y)
			MyMessageBox(MY_MESSAGE_BOX, 40008, _T("Tolerance"), M_ICONQUESTION);
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER3_T].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_T)
			MyMessageBox(MY_MESSAGE_BOX, 40009, _T("Tolerance"), M_ICONQUESTION);
		
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER4_X].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_X)
			MyMessageBox(MY_MESSAGE_BOX, 40010, _T("Tolerance"), M_ICONQUESTION);
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER4_Y].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_Y)
			MyMessageBox(MY_MESSAGE_BOX, 40011, _T("Tolerance"), M_ICONQUESTION);
		if(m_rgsMotorParameter[DEF_AXIS_SERCOS_TABIC_CARRIER4_T].dTolerance > DEF_ALIGN_TOLERANCE_TAB_CARRIER_T)
			MyMessageBox(MY_MESSAGE_BOX, 40012, _T("Tolerance"), M_ICONQUESTION);
	}
	//171003 JSh.e
	
	CMyProgressWnd ProgWnd(NULL, _T("Actuator Data File Writing..."));
	
	// Axis, Cylinder, Vacuum Name 처리
	switch (m_iButtonSel)
	{
	case DEF_DLG_VEL_ACC :
	case DEF_DLG_ORG_VEL_ACC :
	case DEF_DLG_JOG_PITCH :
	case DEF_DLG_LIMIT :
	case DEF_DLG_TOLERANCE :
	case DEF_DLG_MTR_DLY :
		ProgWnd.SetRange(0, DEF_AXIS_MAX_AXIS);
		ProgWnd.SetText(_T("Motion Data is Writing..Wait a moment..."));
		ProgWnd.PeekAndPump();

		for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		{
			ProgWnd.StepIt();

			//2009.04.30 CYJ Load할 때 Data 없음 에러 발생.
//			if (TRUE == MOlbSystemPre.IsSkipAxis(i))
//				continue;

			m_plnkMotion[i]->SaveParameter(&m_rgsMotorParameter[i]);

			if (DEF_DLG_VEL_ACC == m_iButtonSel)
				m_plnkMotion[i]->SetEStopRate();
		}
		break;
	case DEF_DLG_CYL_DLY :
		ProgWnd.SetRange(0, DEF_CYL_MAX_CYLINDER);
		ProgWnd.SetText(_T("Motion Data is Writing..Wait a moment..."));

		ProgWnd.PeekAndPump();

		for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
		{
			m_plnkCylinder[i]->SaveParameter(&m_rgsCylParameter[i]);

			ProgWnd.StepIt();
		}
		break;
	case DEF_DLG_VAC_DLY :
		ProgWnd.SetRange(0, DEF_VAC_MAX_VACUUM);
		ProgWnd.SetText(_T("Motion Data is Writing..Wait a moment..."));
		ProgWnd.PeekAndPump();

		for (int i = 0; i < DEF_VAC_MAX_VACUUM; i++)
		{
			m_plnkVacuum[i]->SaveParameter(&m_rgsVacParameter[i]);

			ProgWnd.StepIt();
		}
		break;
	default:
		break;
	}
	ProgWnd.DestroyWindow();

	// 100807. ECID ___
	UpdateDatabase();
}

void CDataActuatorSettingDlg::OnAllInput() 
{
	// TODO: Add your control notification handler code here
	m_bAllInputSelect = !m_bAllInputSelect;
	if (m_bAllInputSelect == FALSE)
	{
		m_btnAllIn.SetValue(FALSE);
	}
	else
	{
		if(m_btnAllIn.IsWindowEnabled() && m_btnAllIn.IsWindowVisible()) //170911 JSH
			m_btnAllIn.SetValue(TRUE);
	}	
}

void CDataActuatorSettingDlg::OnPrevious() 
{
	m_iCurrentPage--;
	for (int i = 0; i < DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE; i++)
	{
		if (m_bAxisBtnStatus[m_iCurrentPage][i])
		{
			if(m_btnAxis[i].IsWindowEnabled() && m_btnAxis[i].IsWindowVisible()) //170911 JSH
				m_btnAxis[i].SetValue(TRUE);
		}
		else
		{
			m_btnAxis[i].SetValue(FALSE);
		}
	}
	UpdateDisplay();
}

void CDataActuatorSettingDlg::OnNext() 
{
	m_iCurrentPage++;
	for (int i = 0; i < DEF_DLG_MOTOR_SET_MAX_ITEM_PER_PAGE; i++)
	{
		if (m_bAxisBtnStatus[m_iCurrentPage][i])
		{
			if(m_btnAxis[i].IsWindowEnabled() && m_btnAxis[i].IsWindowVisible()) //170911 JSH
				m_btnAxis[i].SetValue(TRUE);
		}
		else
		{
			m_btnAxis[i].SetValue(FALSE);
		}
	}
	UpdateDisplay();
}

void CDataActuatorSettingDlg::OnTitle0() 
{
	// TODO: Add your control notification handler code here
	TitleSelect(0);
}

void CDataActuatorSettingDlg::OnTitle1() 
{
	// TODO: Add your control notification handler code here
	TitleSelect(1);
}

void CDataActuatorSettingDlg::OnTitle2() 
{
	// TODO: Add your control notification handler code here
	TitleSelect(2);
}

void CDataActuatorSettingDlg::OnTitle3() 
{
	// TODO: Add your control notification handler code here
	TitleSelect(3);
}

void CDataActuatorSettingDlg::OnAxis0() 
{
	// TODO: Add your control notification handler code here
	axisSelect(0);
}

void CDataActuatorSettingDlg::OnAxis1() 
{
	// TODO: Add your control notification handler code here
	axisSelect(1);
}

void CDataActuatorSettingDlg::OnAxis2() 
{
	// TODO: Add your control notification handler code here
	axisSelect(2);
}

void CDataActuatorSettingDlg::OnAxis3() 
{
	// TODO: Add your control notification handler code here
	axisSelect(3);
}

void CDataActuatorSettingDlg::OnAxis4() 
{
	// TODO: Add your control notification handler code here
	axisSelect(4);
}

void CDataActuatorSettingDlg::OnAxis5() 
{
	// TODO: Add your control notification handler code here
	axisSelect(5);
}

void CDataActuatorSettingDlg::OnAxis6() 
{
	// TODO: Add your control notification handler code here
	axisSelect(6);
}

void CDataActuatorSettingDlg::OnAxis7() 
{
	// TODO: Add your control notification handler code here
	axisSelect(7);
}

void CDataActuatorSettingDlg::OnAxis8() 
{
	// TODO: Add your control notification handler code here
	axisSelect(8);
}

void CDataActuatorSettingDlg::OnAxis9() 
{
	// TODO: Add your control notification handler code here
	axisSelect(9);
}

void CDataActuatorSettingDlg::OnAxis10() 
{
	// TODO: Add your control notification handler code here
	axisSelect(10);
}

void CDataActuatorSettingDlg::OnAxis11() 
{
	// TODO: Add your control notification handler code here
	axisSelect(11);
}

void CDataActuatorSettingDlg::OnAxis12() 
{
	// TODO: Add your control notification handler code here
	axisSelect(12);
}

void CDataActuatorSettingDlg::OnAxis13() 
{
	// TODO: Add your control notification handler code here
	axisSelect(13);
}

void CDataActuatorSettingDlg::OnAxis14() 
{
	// TODO: Add your control notification handler code here
	axisSelect(14);
}

void CDataActuatorSettingDlg::OnAxis15() 
{
	// TODO: Add your control notification handler code here
	axisSelect(15);
}

//100807.ECID_____________
void CDataActuatorSettingDlg::InitialDatabase()
{
	if (FALSE == MOlbSystemPre.GetSystemData()->m_bUseECDataReport)
		return;

	// Insert
	switch(m_iButtonSel)
	{
	case DEF_DLG_VEL_ACC:
	case DEF_DLG_ORG_VEL_ACC:
	case DEF_DLG_LIMIT:
	case DEF_DLG_JOG_PITCH:
	case DEF_DLG_TOLERANCE:
	case DEF_DLG_MTR_DLY:
		InsertMotionData();
		break;
	case DEF_DLG_CYL_DLY:
	case DEF_DLG_VAC_DLY:
		InsertActuatorData();
		break;
	}

	return;
}

void CDataActuatorSettingDlg::UpdateDatabase()
{
	if (FALSE == MOlbSystemPre.GetSystemData()->m_bUseECDataReport)
		return;

	// 일괄 저장
	if (m_bAllInputSelect) {
		InitialDatabase();
		return;
	}

	/*========================================================*/
	// Motion Data Name
	CString szMotionDataName[DEF_MOTION_DATA_SIZE] =	{
		// 101108 ECID 전체 개수가 1000개가 넘어서 줄임
		//"CWSWLimit",		"CCWSWLimit",		"HomeFastVelocity",		"HomeSlowVelocity",
		//"HomeAccelerate",	"HomeOffset",		"JogSlow",				"JogFast",
		//SJ_YYK 110407 Modify
		/*"RunAccelerate",	"RunVelocity",		"SlowRunVelocity",		"LimitTime",
		"OriginLimitTime",	"StabilityTime",	"Tolerance",			"PassOffset",*/
		"RunAccel",			"RunVel",			"SlowRunVel",			"LimitTime",
		"OriginTime",		"StabilityTime",	"Tolerance",			"PassOffset",
	};
	// Actuator Data Name
	CString szCylinderDataName[DEF_CYLINDER_DATA_SIZE] =	{
		// 101108 ECNAME 글자수 제한으로 이름 변경
		"MovingTime",	"SetTime1",	"SetTime2",	"NoSensTime"
		//"MovingTime",	"SettlingTime1",	"SettlingTime2",	"NoSensTime"
	};
	CString szVacuumDataName[DEF_VACUUM_DATA_SIZE] =	{
		"TurningTime",	"OnSettlingTime",	"OffSettlingTime"
	};
	// 
	/*==========================================================*/
	int i;
	MEqConstant *pECData;
	pECData = new MEqConstant;

	WCHAR szECID[10], szECNAME[40], szECDEF[20]; // ECNAME 40 글자로 제한

	// Update
	switch(m_iButtonSel)
	{
	case DEF_DLG_VEL_ACC:
		for(; m_iaChangedItem.GetSize() > 0; )
		{
			i = m_iaChangedItem.GetAt(0); m_iaChangedItem.RemoveAt(0);
			// RunAccelerate
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE );
			swprintf(szECDEF, _T("%.d"), m_rgsMotorParameter[i].iRunAccelerate);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i].GetBuffer(), szMotionDataName[0].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// RunVelocity
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 1);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dRunVelocity);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i].GetBuffer(), szMotionDataName[1].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// RunVelocity
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 2);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dSlowRunVelocity);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i].GetBuffer(), szMotionDataName[2].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
		}
		break;
	//_______________________________________
	// 101108 ECID 전체 개수가 1000개가 넘어서 줄임
	/*
	case DEF_DLG_ORG_VEL_ACC:
		for(; m_iaChangedItem.GetSize() > 0; )
		{
			i = m_iaChangedItem.GetAt(0); m_iaChangedItem.RemoveAt(0);
			// HomeFastVelocity
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 2);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dHomeFastVelocity);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i], szMotionDataName[2]);
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// HomeSlowVelocity
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 3);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dHomeSlowVelocity);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i], szMotionDataName[3]);
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// HomeAccelerate
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 4);
			swprintf(szECDEF, _T("%d"), m_rgsMotorParameter[i].iHomeAccelerate);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i], szMotionDataName[4]);
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// HomeOffset
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 5);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dHomeOffset);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i], szMotionDataName[5]);
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
		}
		break;
	case DEF_DLG_LIMIT:
		for(; m_iaChangedItem.GetSize() > 0; )
		{
			i = m_iaChangedItem.GetAt(0); m_iaChangedItem.RemoveAt(0);
			// CWSWLimit
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 0);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dCWSWLimit);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i], szMotionDataName[0]);
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// CCWSWLimit
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 1);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dCCWSWLimit);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i], szMotionDataName[1]);
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
		}
		break;
	case DEF_DLG_JOG_PITCH:
		for(; m_iaChangedItem.GetSize() > 0; )
		{
			i = m_iaChangedItem.GetAt(0); m_iaChangedItem.RemoveAt(0);
			// JogSlow
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 6);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dJogSlow);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i], szMotionDataName[6]);
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// JogFast
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 7);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dJogFast);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i], szMotionDataName[7]);
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
		}
		break;
	*/
	//__________________________________________
	case DEF_DLG_TOLERANCE:
		for(; m_iaChangedItem.GetSize() > 0; )
		{
			i = m_iaChangedItem.GetAt(0); m_iaChangedItem.RemoveAt(0);
			// Tolerance
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 6);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dTolerance);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i].GetBuffer(), szMotionDataName[6].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
		}
		break;
	case DEF_DLG_MTR_DLY:
		for(; m_iaChangedItem.GetSize() > 0; )
		{
			i = m_iaChangedItem.GetAt(0); m_iaChangedItem.RemoveAt(0);
			// LimitTime
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 3);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dLimitTime);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i].GetBuffer(), szMotionDataName[3].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// OriginLimitTime
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 4);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dOriginLimitTime);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i].GetBuffer(), szMotionDataName[4].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// StabilityTime
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + 5);
			swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dStabilityTime);
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i].GetBuffer(), szMotionDataName[5].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
		}
		break;
	case DEF_DLG_CYL_DLY:
		for(; m_iaChangedItem.GetSize() > 0; )
		{
			i = m_iaChangedItem.GetAt(0); m_iaChangedItem.RemoveAt(0);
			// MovingTime
			swprintf(szECID, _T("%d"), DEF_EC_ID_CYLINDER_BASE + i*DEF_CYLINDER_DATA_SIZE + 0);
			swprintf(szECDEF, _T("%.3f"), m_rgsCylParameter[i].m_dMovingTime);
			swprintf(szECNAME, _T("%s %s"), m_strCylinderName[i].GetBuffer(), szCylinderDataName[0].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// SettlingTime1
			swprintf(szECID, _T("%d"), DEF_EC_ID_CYLINDER_BASE + i*DEF_CYLINDER_DATA_SIZE + 1);
			swprintf(szECDEF, _T("%.3f"), m_rgsCylParameter[i].m_dSettlingTime1);
			swprintf(szECNAME, _T("%s %s"), m_strCylinderName[i].GetBuffer(), szCylinderDataName[1].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// SettlingTime2
			swprintf(szECID, _T("%d"), DEF_EC_ID_CYLINDER_BASE + i*DEF_CYLINDER_DATA_SIZE + 2);
			swprintf(szECDEF, _T("%.3f"), m_rgsCylParameter[i].m_dSettlingTime2);
			swprintf(szECNAME, _T("%s %s"), m_strCylinderName[i].GetBuffer(), szCylinderDataName[2].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// NoSensTime
			swprintf(szECID, _T("%d"), DEF_EC_ID_CYLINDER_BASE + i*DEF_CYLINDER_DATA_SIZE + 3);
			swprintf(szECDEF, _T("%.3f"), m_rgsCylParameter[i].m_dNoSenMovingTime);
			swprintf(szECNAME, _T("%s %s"), m_strCylinderName[i].GetBuffer(), szCylinderDataName[3].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
		}
		break;
	case DEF_DLG_VAC_DLY:
		for(; m_iaChangedItem.GetSize() > 0; )
		{
			i = m_iaChangedItem.GetAt(0); m_iaChangedItem.RemoveAt(0);
			// TurningTime
			swprintf(szECID, _T("%d"), DEF_EC_ID_VACUUM_BASE + i*DEF_VACUUM_DATA_SIZE + 0);
			swprintf(szECDEF, _T("%.3f"), m_rgsVacParameter[i].m_dTurningTime);
			swprintf(szECNAME, _T("%s %s"), m_strVacuumName[i].GetBuffer(), szVacuumDataName[0].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// OnSettlingTime
			swprintf(szECID, _T("%d"), DEF_EC_ID_VACUUM_BASE + i*DEF_VACUUM_DATA_SIZE + 1);
			swprintf(szECDEF, _T("%.3f"), m_rgsVacParameter[i].m_dOnSettlingTime);
			swprintf(szECNAME, _T("%s %s"), m_strVacuumName[i].GetBuffer(), szVacuumDataName[1].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
			// OffSettlingTime
			swprintf(szECID, _T("%d"), DEF_EC_ID_VACUUM_BASE + i*DEF_VACUUM_DATA_SIZE + 2);
			swprintf(szECDEF, _T("%.3f"), m_rgsVacParameter[i].m_dOffSettlingTime);
			swprintf(szECNAME, _T("%s %s"), m_strVacuumName[i].GetBuffer(), szVacuumDataName[2].GetBuffer());
			pECData->m_strECID = (LPCTSTR)szECID; pECData->m_strECDef = (LPCTSTR)szECDEF; pECData->m_strECName = (LPCTSTR)szECNAME;
			MOlbSystemPre.GetTrsAutoManagerComponent()->UpdateEqConstData(pECData);
		}
		break;
	}

	delete pECData;
}

void CDataActuatorSettingDlg::InsertMotionData()
{
	// Motion Data Name
	CString szMotionDataName[DEF_MOTION_DATA_SIZE] =	{
		// 101108 ECID 전체 개수가 1000개가 넘어서 줄임
		//"CWSWLimit",		"CCWSWLimit",		"HomeFastVel",			"HomeSlowVel",
		//"HomeAccel",		"HomeOffset",		"JogSlow",				"JogFast",
		"RunAccel",			"RunVel",			"SlowRunVel",			"LimitTime",
		"OriginTime",		"StabilityTime",	"Tolerance",			"PassOffset",
	};

	CString strSqlMsg;
	WCHAR szECID[10], szECNAME[40], szECDEF[20]; // ECNAME 40 글자로 제한

	memset(szECID, 0x00, sizeof(szECID));
	memset(szECNAME, 0x00, sizeof(szECNAME));
	memset(szECDEF, 0x00, sizeof(szECDEF));
	
	// Delete All Data
	for (int i=0; i<DEF_AXIS_SERCOS_MAX_AXIS; i++)
	{
		for (int j=0; j<DEF_MOTION_DATA_SIZE; j++)
		{
			// ECID 
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + j);
			strSqlMsg.Format(_T("delete * from EQ_EC_DATA where ECID='%s'"), szECID); // 11000 ~ 

			if (!MyInputDB.InsUpDel(strSqlMsg)) {
				MyMessageBox(_T("Failed to delete Motion EC Data from Database?\r\n") 
					_T("Failed to delete Motion EC Data from Database.\r\nDatabase 에서 Motion EC Data 를 삭제하는 데 실패하였습니다."), "Error", M_ICONERROR);
				//MyInputDB.Disconnect();
				return;
			}
		}
	}

	// Insert Motion Data
	for (int i=0; i<DEF_AXIS_SERCOS_MAX_AXIS; i++)
	{
		for (int j=0; j<DEF_MOTION_DATA_SIZE; j++)
		{
			// ECID 
			swprintf(szECID, _T("%d"), DEF_EC_ID_MOTION_BASE + i*DEF_MOTION_DATA_SIZE + j);
			
			//SJ_YYK 110502 Add... 40 넘네...
			if(m_strAxisName[i].Left(11) == "RInspection")
				m_strAxisName[i] = "RInspect" + m_strAxisName[i].Mid(11);
			//_____________________________________________________________*/
			// ECNAME
			swprintf(szECNAME, _T("%s Axis %s"), m_strAxisName[i].GetBuffer(), szMotionDataName[j].GetBuffer());
			// ECDEF
			switch(j)
			{
			//_______________________________________
			// 101108 ECID 전체 개수가 1000개가 넘어서 줄임
				case 0:	// RunAccelerate
					swprintf(szECDEF, _T("%d"), m_rgsMotorParameter[i].iRunAccelerate);
					break;
				case 1:	// RunVelocity
					swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dRunVelocity);
					break;
				case 2: // SlowRunVelocity
					swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dSlowRunVelocity);
					break;
				case 3: // LimitTime
					swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dLimitTime);
					break;
				case 4: // OriginLimitTime
					swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dOriginLimitTime);
					break;
				case 5: // StabilityTime
					swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dStabilityTime);
					break;
				case 6: // Tolerance
					swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dTolerance);
					break;
				case 7: // PassOffset
					swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dPassOffset);
					break;
			/*
			case 0: // CWSWLimit
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dCWSWLimit);
				break;
			case 1: // CCWSWLimit
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dCCWSWLimit);
				break;
			case 2: // HomeFastVelocity
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dHomeFastVelocity);
				break;
			case 3:	// HomeSlowVelocity
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dHomeSlowVelocity);
				break;
			case 4:	// HomeAccelerate
				swprintf(szECDEF, _T("%d"), m_rgsMotorParameter[i].iHomeAccelerate);
				break;
			case 5:	// HomeOffset
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dHomeOffset);
				break;
			case 6:	// JogSlow
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dJogSlow);
				break;
			case 7:	// JogFast
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dJogFast);
				break;
			case 8:	// RunAccelerate
				swprintf(szECDEF, _T("%d"), m_rgsMotorParameter[i].iRunAccelerate);
				break;
			case 9:	// RunVelocity
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dRunVelocity);
				break;
			case 10: // SlowRunVelocity
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dSlowRunVelocity);
				break;
			case 11: // LimitTime
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dLimitTime);
				break;
			case 12: // OriginLimitTime
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dOriginLimitTime);
				break;
			case 13: // StabilityTime
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dStabilityTime);
				break;
			case 14: // Tolerance
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dTolerance);
				break;
			case 15: // PassOffset
				swprintf(szECDEF, _T("%.3f"), m_rgsMotorParameter[i].dPassOffset);
				break;
			*/
			//_________________________________________
			}
			
			strSqlMsg.Format(_T("insert into EQ_EC_DATA (ECID, ECNAME, ECDEF) values ('%s', '%s', '%s')"), szECID, szECNAME, szECDEF);
			if (!MyInputDB.InsUpDel(strSqlMsg)) {
				MyMessageBox(_T("Failed to insert Motion EC Data from Database?\r\n")
					_T("Failed to register Motion EC Data in Database.\r\nDatabase 에서 Motion EC Data 를 등록하는 데 실패하였습니다."), "Error", M_ICONERROR);
				//MyInputDB.Disconnect();
			}
		}
	}
}

void CDataActuatorSettingDlg::InsertActuatorData()
{
	// Actuator Data Name
	CString szCylinderDataName[DEF_CYLINDER_DATA_SIZE] =	{
		// 101108 ECNAME 글자수 제한으로 이름 변경
		"MovingTime",	"SetTime1",	"SetTime2",	"NoSensTime"
		//"MovingTime",	"SettlingTime1",	"SettlingTime2",	"NoSensTime"
	};
	CString szVacuumDataName[DEF_VACUUM_DATA_SIZE] =	{
		"TurningTime",	"OnSettlingTime",	"OffSettlingTime"
	};

	CString strSqlMsg;
	WCHAR szECID[10], szECNAME[40], szECDEF[20]; // ECNAME 40 글자로 제한

	memset(szECID, 0x00, sizeof(szECID));
	memset(szECNAME, 0x00, sizeof(szECNAME));
	memset(szECDEF, 0x00, sizeof(szECDEF));
	
	//_______________________________________________________
	// Insert Cylinder Data
	if (m_iButtonSel == DEF_DLG_CYL_DLY)
	{
		// Delete All Data
		for (int i=0; i<DEF_CYL_MAX_CYLINDER; i++)
		{
			for (int j=0; j<DEF_CYLINDER_DATA_SIZE; j++)
			{
				// ECID 
				swprintf(szECID, _T("%d"), DEF_EC_ID_CYLINDER_BASE + i*DEF_CYLINDER_DATA_SIZE + j);
				strSqlMsg.Format(_T("delete * from EQ_EC_DATA where ECID='%s'"), szECID); // 10000 ~ 

				if (!MyInputDB.InsUpDel(strSqlMsg)) {
					MyMessageBox(_T("Failed to delete Cylinder EC Data from Database?\r\n") 
						_T("Failed to delete Cylinder EC Data from Database.\r\nDatabase 에서 Cylinder EC Data 를 삭제하는 데 실패하였습니다."), "Error", M_ICONERROR);
					//MyInputDB.Disconnect();
					return;
				}
			}
		}

		// Insert Data
		for (int i=0; i<DEF_CYL_MAX_CYLINDER; i++)
		{
			for (int j=0; j<DEF_CYLINDER_DATA_SIZE; j++)
			{
				// ECID 
				swprintf(szECID, _T("%d"), DEF_EC_ID_CYLINDER_BASE + i*DEF_CYLINDER_DATA_SIZE + j);
				// ECNAME
				//______________________
				// 101108 Add. ECNAME 40글자 제한으로 이름 변경
				if (m_strCylinderName[i].Left(8) == "TabPress")
					m_strCylinderName[i] = "TP" + m_strCylinderName[i].Mid(8);
					//SJ_YYK 110330 Modify 40 넘는 Item 발견..
					if(m_strCylinderName[i].Left(18) == "TP1 Attach Cutting")
						m_strCylinderName[i] =  "TP1 At Cut" + m_strCylinderName[i].Mid(18);
					else if(m_strCylinderName[i].Left(18) == "TP2 Attach Cutting")
						m_strCylinderName[i] =  "TP2 At Cut" + m_strCylinderName[i].Mid(18);
				//______________________
				swprintf(szECNAME, _T("%s %s"), m_strCylinderName[i].GetBuffer(), szCylinderDataName[j].GetBuffer());
				// ECDEF
				switch(j)
				{
				case 0: // MovingTime
					swprintf(szECDEF, _T("%.3f"), m_rgsCylParameter[i].m_dMovingTime);
					break;
				case 1: // SettlingTime1
					swprintf(szECDEF, _T("%.3f"), m_rgsCylParameter[i].m_dSettlingTime1);
					break;
				case 2: // SettlingTime2
					swprintf(szECDEF, _T("%.3f"), m_rgsCylParameter[i].m_dSettlingTime2);
					break;
				case 3:	// NoSenMovingTime
					swprintf(szECDEF, _T("%.3f"), m_rgsCylParameter[i].m_dNoSenMovingTime);
					break;
				}

				strSqlMsg.Format(_T("insert into EQ_EC_DATA (ECID, ECNAME, ECDEF) values ('%s', '%s', '%s')"), szECID, szECNAME, szECDEF);
				if (!MyInputDB.InsUpDel(strSqlMsg)) {
					MyMessageBox(_T("Failed to insert Cylinder EC Data from Database?\r\n")
						_T("Failed to register Cylinder EC Data in Database.\r\nDatabase 에서 Cylinder EC Data 를 등록하는 데 실패하였습니다."), "Error", M_ICONERROR);
					//MyInputDB.Disconnect();
				}
			}
		}
	}
	//_______________________________________________________
	// Insert Vacuum Data
	else if (m_iButtonSel == DEF_DLG_VAC_DLY)
	{
		// Delete All Data
		for (int i=0; i<DEF_VAC_MAX_VACUUM; i++)
		{
			for (int j=0; j<DEF_VACUUM_DATA_SIZE; j++)
			{
				// ECID 
				swprintf(szECID, _T("%d"), DEF_EC_ID_VACUUM_BASE + i*DEF_VACUUM_DATA_SIZE + j);
				strSqlMsg.Format(_T("delete * from EQ_EC_DATA where ECID='%s'"), szECID); // 10000 ~ 

				if (!MyInputDB.InsUpDel(strSqlMsg)) {
					MyMessageBox(_T("Failed to delete Vacuum EC Data from Database?\r\n") 
						_T("Failed to delete Vacuum EC Data from Database.\r\nDatabase 에서 Vacuum EC Data 를 삭제하는 데 실패하였습니다."), "Error", M_ICONERROR);
					//MyInputDB.Disconnect();
					return;
				}
			}
		}

		// Insert Data
		for (int i=0; i<DEF_VAC_MAX_VACUUM; i++)
		{
			for (int j=0; j<DEF_VACUUM_DATA_SIZE; j++)
			{
				// ECID 
				swprintf(szECID, _T("%d"), DEF_EC_ID_VACUUM_BASE + i*DEF_VACUUM_DATA_SIZE + j);
				// ECNAME
				swprintf(szECNAME, _T("%s %s"), m_strVacuumName[i].GetBuffer(), szCylinderDataName[j].GetBuffer());
				// ECDEF
				switch(j)
				{
				case 0: // TurningTime
					swprintf(szECDEF, _T("%.3f"), m_rgsVacParameter[i].m_dTurningTime);
					break;
				case 1: // OnSettlingTime
					swprintf(szECDEF, _T("%.3f"), m_rgsVacParameter[i].m_dOnSettlingTime);
					break;
				case 2: // OffSettlingTime
					swprintf(szECDEF, _T("%.3f"), m_rgsVacParameter[i].m_dOffSettlingTime);
					break;
				}

				strSqlMsg.Format(_T("insert into EQ_EC_DATA (ECID, ECNAME, ECDEF) values ('%s', '%s', '%s')"), szECID, szECNAME, szECDEF);
				if (!MyInputDB.InsUpDel(strSqlMsg)) {
					//MyMessageBox(_T("Failed to insert Vacuum EC Data from Database?\r\n")
					//	_T("Database 에서 Vacuum EC Data 를 등록하는 데 실패하였습니다."), "Error", M_ICONERROR);
					//MyInputDB.Disconnect();
				}
			}
		}
	}
}

//_______________________________

void CDataActuatorSettingDlg::OnClickCreateEcdata() 
{
	// TODO: Add your control notification handler code here
	
	// 101108 Add.
	GetDlgItem(IDB_CREATE_ECDATA)->EnableWindow(FALSE);
	
	CMyProgressWnd ProgWnd(NULL, _T("EC Data File Creating..."));
	ProgWnd.SetRange(0, 1);
	ProgWnd.SetText(_T("EC Data is Creating..Wait a moment..."));
	ProgWnd.PeekAndPump();

	// 100807. ECID Test _______
	InitialDatabase();

	// 101108 Add.
	GetDlgItem(IDB_CREATE_ECDATA)->EnableWindow(TRUE);

	MyMessageBox("EC data generation is completed.\r\n EC Data 생성을 완료하였습니다.", _T("EC Data Creation"), M_ICONINFORMATION);
}
