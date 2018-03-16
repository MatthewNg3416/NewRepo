// TabDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataTabICDlg.h"
#include "MPlatformOlbSystem.h"
#include "DataTabICImageDlg.h"

#include <math.h>
#include "MTabAligner.h"
#include "MCameraCarrier.h"
#include "MCtrlTabFeeder.h"
#include "MTabCarrier.h" //2009.08.14 CYJ
#include "MPanelAligner.h"
#include "MTabData.h"
#include "MPanelData.h"
#include "MPreBonderData.h"
#include "MProcessData.h"
#include "MTrsAutoManager.h"

#include "MTrsTabCarrier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataTabICDlg dialog

//101004.KKY_____
//CDataTabICDlg::CDataTabICDlg(int iSel, CWnd* pParent /*=NULL*/)
CDataTabICDlg::CDataTabICDlg(BOOL bCurrentModel, BOOL bEditable, int iSel, CWnd* pParent /*=NULL*/)   // standard constructor
//_______________
	: CDialog(CDataTabICDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataTabICDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	//101004.KKY_____
//	m_plnkPanelData		= MOlbSystemPre.GetPanelData();
//	m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
//	m_plnkTabData		= MOlbSystemPre.GetTabData();
	if (bCurrentModel)
	{
		m_plnkPanelData		= MOlbSystemPre.GetPanelData();
		m_plnkPreBonderData = MOlbSystemPre.GetPreBonderData();
		m_plnkTabData		= MOlbSystemPre.GetTabData();
	}
	else
	{
		m_plnkPanelData		= MOlbSystemPre.GetTmpPanelData();
		m_plnkPreBonderData = MOlbSystemPre.GetTmpPreBonderData();
		m_plnkTabData		= MOlbSystemPre.GetTmpTabData();
	}
	m_bEditable = bEditable;
	//_______________
	m_plnkSystemData	= MOlbSystemPre.GetSystemData();
	m_iTabPage = 0;
}


void CDataTabICDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataTabICDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDB_BATCH_INPUT, m_bBatch);
	DDX_Control(pDX, IDB_TAB_NEXT, m_bNext);
	DDX_Control(pDX, IDB_TAB_PREVIOUS, m_bPrevious);
	DDX_Control(pDX, IDB_TAB_DATA_CANCEL, m_bExit);
	DDX_Control(pDX, IDB_TAB_DATA_SAVE, m_bSave);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, PDB_ON_1, m_btnTabOn[0]);
	DDX_Control(pDX, PDB_ON_2, m_btnTabOn[1]);
	DDX_Control(pDX, PDB_ON_3, m_btnTabOn[2]);
	DDX_Control(pDX, PDB_ON_4, m_btnTabOn[3]);
	DDX_Control(pDX, PDB_ON_5, m_btnTabOn[4]);
	DDX_Control(pDX, PDB_ON_6, m_btnTabOn[5]);
	DDX_Control(pDX, PDB_ON_7, m_btnTabOn[6]);
	DDX_Control(pDX, PDB_ON_8, m_btnTabOn[7]);
	DDX_Control(pDX, PDB_ON_9, m_btnTabOn[8]);
	DDX_Control(pDX, PDB_ON_10, m_btnTabOn[9]);
	DDX_Control(pDX, PDB_ON_11, m_btnTabOn[10]);
	DDX_Control(pDX, PDB_ON_12, m_btnTabOn[11]);
	DDX_Control(pDX, PDB_ON_13, m_btnTabOn[12]);
	DDX_Control(pDX, PDB_ON_14, m_btnTabOn[13]);
	DDX_Control(pDX, PDB_ON_15, m_btnTabOn[14]);
	DDX_Control(pDX, PDB_ON_16, m_btnTabOn[15]);
	DDX_Control(pDX, PDB_ON_17, m_btnTabOn[16]);
	DDX_Control(pDX, PDB_ON_18, m_btnTabOn[17]);
	DDX_Control(pDX, PDB_ON_19, m_btnTabOn[18]);
	DDX_Control(pDX, PDB_ON_20, m_btnTabOn[19]);
	DDX_Control(pDX, PDB_ON_21, m_btnTabOn[20]);
	DDX_Control(pDX, PDB_ON_22, m_btnTabOn[21]);
	DDX_Control(pDX, PDB_ON_23, m_btnTabOn[22]);
	DDX_Control(pDX, PDB_ON_24, m_btnTabOn[23]);
	DDX_Control(pDX, PDB_ON_25, m_btnTabOn[24]);
	DDX_Control(pDX, PDB_ON_26, m_btnTabOn[25]);
	DDX_Control(pDX, PDB_ON_27, m_btnTabOn[26]);
	DDX_Control(pDX, PDB_ON_28, m_btnTabOn[27]);
	DDX_Control(pDX, PDB_ON_29, m_btnTabOn[28]);
	DDX_Control(pDX, PDB_ON_30, m_btnTabOn[29]);
	DDX_Control(pDX, PDB_ON_31, m_btnTabOn[30]);
	DDX_Control(pDX, PDB_ON_32, m_btnTabOn[31]);
	DDX_Control(pDX, PDB_ON_33, m_btnTabOn[32]);
	DDX_Control(pDX, PDB_ON_34, m_btnTabOn[33]);
	DDX_Control(pDX, PDB_ON_35, m_btnTabOn[34]);
	DDX_Control(pDX, PDB_ON_36, m_btnTabOn[35]);

	DDX_Control(pDX, IDB_TITLE1, m_btnWorkTabNum);

	DDX_Control(pDX, IDS_TITLE1, m_sData[0]);
	DDX_Control(pDX, IDS_TITLE2, m_sData[1]);
	DDX_Control(pDX, IDS_TITLE3, m_sData[2]);
	DDX_Control(pDX, IDS_TITLE4, m_sData[3]);
	DDX_Control(pDX, IDS_TITLE5, m_sData[4]);
	DDX_Control(pDX, IDS_TITLE6, m_sData[5]);
	DDX_Control(pDX, IDS_TITLE7, m_sData[6]);
	DDX_Control(pDX, IDS_TITLE8, m_sData[7]);
	DDX_Control(pDX, IDS_TITLE9, m_sData[8]);
	DDX_Control(pDX, IDS_TITLE10, m_sData[9]);
	DDX_Control(pDX, IDS_TITLE11, m_sData[10]);
	DDX_Control(pDX, IDS_TITLE12, m_sData[11]);	//35
	DDX_Control(pDX, IDS_TITLE13, m_sData[12]);	//36
	DDX_Control(pDX, IDS_TITLE14, m_sData[13]);	//37
	DDX_Control(pDX, IDS_TITLE15, m_sData[14]);	//38
	DDX_Control(pDX, IDS_TITLE16, m_sData[15]);	//39
	DDX_Control(pDX, IDS_TITLE17, m_sData[16]);	//40
	DDX_Control(pDX, IDS_TITLE18, m_sData[17]);	//41
	DDX_Control(pDX, IDS_TITLE19, m_sData[18]); //42
	DDX_Control(pDX, IDS_TITLE20, m_sData[19]); //42

	DDX_Control(pDX, PDS_USE_1, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+0]);
	DDX_Control(pDX, PDS_USE_2, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+1]);
	DDX_Control(pDX, PDS_USE_3, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+2]);
	DDX_Control(pDX, PDS_USE_4, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+3]);
	DDX_Control(pDX, PDS_USE_5, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+4]);
	DDX_Control(pDX, PDS_USE_6, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+5]);
	DDX_Control(pDX, PDS_USE_7, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+6]);
	DDX_Control(pDX, PDS_USE_8, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+7]);
	DDX_Control(pDX, PDS_USE_9, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+8]);
	DDX_Control(pDX, PDS_USE_10, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+9]);
	DDX_Control(pDX, PDS_USE_11, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+10]);
	DDX_Control(pDX, PDS_USE_12, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+11]);
	DDX_Control(pDX, PDS_USE_13, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+12]);
	DDX_Control(pDX, PDS_USE_14, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+13]);
	DDX_Control(pDX, PDS_USE_15, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+14]);
	DDX_Control(pDX, PDS_USE_16, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+15]);
	DDX_Control(pDX, PDS_USE_17, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+16]);
	DDX_Control(pDX, PDS_USE_18, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+17]);
	DDX_Control(pDX, PDS_USE_19, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+18]);
	DDX_Control(pDX, PDS_USE_20, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+19]);
	DDX_Control(pDX, PDS_USE_21, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+20]);
	DDX_Control(pDX, PDS_USE_22, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+21]);
	DDX_Control(pDX, PDS_USE_23, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+22]);
	DDX_Control(pDX, PDS_USE_24, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+23]);
	DDX_Control(pDX, PDS_USE_25, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+24]);
	DDX_Control(pDX, PDS_USE_26, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+25]);
	DDX_Control(pDX, PDS_USE_27, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+26]);
	DDX_Control(pDX, PDS_USE_28, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+27]);
	DDX_Control(pDX, PDS_USE_29, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+28]);
	DDX_Control(pDX, PDS_USE_30, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+29]);
	DDX_Control(pDX, PDS_USE_31, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+30]);
	DDX_Control(pDX, PDS_USE_32, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+31]);
	DDX_Control(pDX, PDS_USE_33, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+32]);
	DDX_Control(pDX, PDS_USE_34, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+33]);
	DDX_Control(pDX, PDS_USE_35, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+34]);
	DDX_Control(pDX, PDS_USE_36, m_sData[DEF_TAB_DATA_INDEX_START_OFFSET+35]);
}


BEGIN_MESSAGE_MAP(CDataTabICDlg, CDialog)
	//{{AFX_MSG_MAP(CDataTabICDlg)
	ON_BN_CLICKED(IDC_TAB_PITCH, OnTabPitch)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTabICDlg message handlers

BEGIN_EVENTSINK_MAP(CDataTabICDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataTabICDlg)
	ON_EVENT(CDataTabICDlg, IDB_TITLE1, -600 /* Click */, OnTitle1, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE2, -600 /* Click */, OnTitle2, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE3, -600 /* Click */, OnTitle3, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE4, -600 /* Click */, OnTitle4, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE5, -600 /* Click */, OnTitle5, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE6, -600 /* Click */, OnTitle6, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE7, -600 /* Click */, OnTitle7, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE8, -600 /* Click */, OnTitle8, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE9, -600 /* Click */, OnTitle9, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE10, -600 /* Click */, OnTitle10, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE11, -600 /* Click */, OnTitle11, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_1, -600 /* Click */, OnOn1, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_2, -600 /* Click */, OnOn2, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_3, -600 /* Click */, OnOn3, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_4, -600 /* Click */, OnOn4, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_5, -600 /* Click */, OnOn5, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_6, -600 /* Click */, OnOn6, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_7, -600 /* Click */, OnOn7, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_8, -600 /* Click */, OnOn8, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_9, -600 /* Click */, OnOn9, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_10, -600 /* Click */, OnOn10, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_11, -600 /* Click */, OnOn11, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_12, -600 /* Click */, OnOn12, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_13, -600 /* Click */, OnOn13, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_14, -600 /* Click */, OnOn14, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_15, -600 /* Click */, OnOn15, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_16, -600 /* Click */, OnOn16, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_17, -600 /* Click */, OnOn17, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_18, -600 /* Click */, OnOn18, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_19, -600 /* Click */, OnOn19, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_20, -600 /* Click */, OnOn20, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_21, -600 /* Click */, OnClickOn21, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_22, -600 /* Click */, OnClickOn22, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_23, -600 /* Click */, OnClickOn23, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_24, -600 /* Click */, OnClickOn24, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_25, -600 /* Click */, OnClickOn25, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_26, -600 /* Click */, OnClickOn26, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_27, -600 /* Click */, OnClickOn27, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_28, -600 /* Click */, OnClickOn28, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_29, -600 /* Click */, OnClickOn29, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_30, -600 /* Click */, OnClickOn30, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_31, -600 /* Click */, OnClickOn31, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_32, -600 /* Click */, OnClickOn32, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_33, -600 /* Click */, OnClickOn33, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_34, -600 /* Click */, OnClickOn34, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_35, -600 /* Click */, OnClickOn35, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_ON_36, -600 /* Click */, OnClickOn36, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_1, -600 /* Click */, OnUse1, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_2, -600 /* Click */, OnUse2, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_3, -600 /* Click */, OnUse3, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_4, -600 /* Click */, OnUse4, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_5, -600 /* Click */, OnUse5, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_6, -600 /* Click */, OnUse6, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_7, -600 /* Click */, OnUse7, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_8, -600 /* Click */, OnUse8, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_9, -600 /* Click */, OnUse9, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_10, -600 /* Click */, OnUse10, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_11, -600 /* Click */, OnUse11, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_12, -600 /* Click */, OnUse12, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_13, -600 /* Click */, OnUse13, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_14, -600 /* Click */, OnUse14, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_15, -600 /* Click */, OnUse15, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_16, -600 /* Click */, OnUse16, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_17, -600 /* Click */, OnUse17, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_18, -600 /* Click */, OnUse18, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_19, -600 /* Click */, OnUse19, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_20, -600 /* Click */, OnUse20, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_21, -600 /* Click */, OnClickUse21, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_22, -600 /* Click */, OnClickUse22, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_23, -600 /* Click */, OnClickUse23, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_24, -600 /* Click */, OnClickUse24, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_25, -600 /* Click */, OnClickUse25, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_26, -600 /* Click */, OnClickUse26, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_27, -600 /* Click */, OnClickUse27, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_28, -600 /* Click */, OnClickUse28, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_29, -600 /* Click */, OnClickUse29, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_30, -600 /* Click */, OnClickUse30, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_31, -600 /* Click */, OnClickUse31, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_32, -600 /* Click */, OnClickUse32, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_33, -600 /* Click */, OnClickUse33, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_34, -600 /* Click */, OnClickUse34, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_35, -600 /* Click */, OnClickUse35, VTS_NONE)
	ON_EVENT(CDataTabICDlg, PDB_USE_36, -600 /* Click */, OnClickUse36, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TAB_DATA_SAVE, -600 /* Click */, OnTabDataSave, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_BATCH_INPUT, -600 /* Click */, OnBatchInput, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TAB_PREVIOUS, -600 /* Click */, OnTabPrevious, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TAB_NEXT, -600 /* Click */, OnTabNext, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TAB_DATA_CANCEL, -600 /* Click */, OnTabDataCancel, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_IMAGE_VIEW, -600 /* Click */, OnClickImageView, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE12, -600 /* Click */, OnClickTitle12, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE13, -600 /* Click */, OnClickTitle13, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE14, -600 /* Click */, OnClickTitle14, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE15, -600 /* Click */, OnClickTitle15, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE16, -600 /* Click */, OnClickTitle16, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE17, -600 /* Click */, OnClickTitle17, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE19, -600 /* Click */, OnClickTitle19, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE18, -600 /* Click */, OnClickTitle18, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TAB_PREVIOUS3, -600 /* Click */, OnClickTabPrevious3, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TAB_NEXT3, -600 /* Click */, OnClickTabNext3, VTS_NONE)
	ON_EVENT(CDataTabICDlg, IDB_TITLE20, -600 /* Click */, OnClickTitle20, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



BOOL CDataTabICDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_GATE && m_plnkPanelData->m_bUseFlag_Xu) 
		UpdateDisplay(WORKING_SIDE_X_UP);
	else if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_GATE && m_plnkPanelData->m_bUseFlag_Xd) 
		UpdateDisplay(WORKING_SIDE_X_DOWN);
	else if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_SOURCE && m_plnkPanelData->m_bUseFlag_Yl) 
		UpdateDisplay(WORKING_SIDE_Y_LEFT);
	// jdy sesl 081229	else
	else if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_SOURCE && m_plnkPanelData->m_bUseFlag_Yr)
		UpdateDisplay(WORKING_SIDE_Y_RIGHT);
	// jdy sesl 081229
	else
		UpdateDisplay(WORKING_SIDE_UNKNOWN);
	
	m_iFuncOnOff = m_plnkPreBonderData->m_bUseTCP;
	if (m_iFuncOnOff)
		m_sData[9].SetCaption(_T("TCP"));
	else
		m_sData[9].SetCaption(_T("COf"));

	m_iBadTabLimitCount = m_plnkSystemData->m_uiBadTabLimitCount;
	swprintf(m_cBuf,_T("%d"), m_iBadTabLimitCount);
	m_sData[10].SetCaption(m_cBuf);

	/*/
	//MERGE_LKH_090704
	// 20090619 sj_ysb
	m_uiPermissionXLeftValue13 = m_plnkPreBonderData->m_uiPermissionXLeftValue13;
	m_dData[31] = m_uiPermissionXLeftValue13;
	swprintf(m_cBuf,_T("%.4f"), m_uiPermissionXLeftValue13);
	m_sData[31].SetCaption(m_cBuf);

	m_uiPermissionXRightValue13 = m_plnkPreBonderData->m_uiPermissionXRightValue13;
	m_dData[32] = m_uiPermissionXRightValue13;
	swprintf(m_cBuf,_T("%.4f"), m_uiPermissionXRightValue13);
	m_sData[32].SetCaption(m_cBuf);

	m_uiPermissionXLeftValue24 = m_plnkPreBonderData->m_uiPermissionXLeftValue24;
	m_dData[33] = m_uiPermissionXLeftValue24;
	swprintf(m_cBuf,_T("%.4f"), m_uiPermissionXLeftValue24);
	m_sData[33].SetCaption(m_cBuf);

	m_uiPermissionXRightValue24 = m_plnkPreBonderData->m_uiPermissionXRightValue24;
	m_dData[34] = m_uiPermissionXRightValue24;
	swprintf(m_cBuf,_T("%.4f"), m_uiPermissionXRightValue24);
	m_sData[34].SetCaption(m_cBuf);
	/*/

	m_dData[11] = m_plnkPreBonderData->m_dToleranceSRLine;
	swprintf(m_cBuf,_T("%.3f"), m_dData[11]);
	m_sData[11].SetCaption(m_cBuf);

	m_dData[12] = m_plnkPreBonderData->m_dToleranceTabEnd;
	swprintf(m_cBuf,_T("%.3f"), m_dData[12]);
	m_sData[12].SetCaption(m_cBuf);

	m_dData[13] = m_plnkPreBonderData->m_dNoCareValueTabEnd;
	swprintf(m_cBuf,_T("%.3f"), m_dData[13]);
	m_sData[13].SetCaption(m_cBuf);
	
	//20120828 sj_ysb
	m_dSRLineMarkDistance = m_plnkPreBonderData->m_dSRLineMarkDistance;
	m_dData[15] = m_dSRLineMarkDistance;
	swprintf(m_cBuf,_T("%.4f"), m_dSRLineMarkDistance);
	m_sData[15].SetCaption(m_cBuf);

	m_dTabEndMarkDistance = m_plnkPreBonderData->m_dTabEndMarkDistance;
	m_dData[16] = m_dTabEndMarkDistance;
	swprintf(m_cBuf,_T("%.4f"), m_dTabEndMarkDistance);
	m_sData[16].SetCaption(m_cBuf);

	m_bMarkDistanceFunc = m_plnkPreBonderData->m_bMarkDistanceFunc;
	if (m_plnkPreBonderData->m_bMarkDistanceFunc)
	{
		m_sData[17].SetCaption(_T("ON"));
		m_dData[17] = 1.0;
	}
	else
	{
		m_sData[17].SetCaption(_T("OFF"));
		m_dData[17] = 0.0;
	}

	m_bSRLineAlignFunc = m_plnkPreBonderData->m_bSRLineAlignFunc;
	if (m_plnkPreBonderData->m_bSRLineAlignFunc)
	{
		m_sData[18].SetCaption(_T("ON"));
		m_dData[18] = 1.0;
	}
	else
	{
		m_sData[18].SetCaption(_T("OFF"));
		m_dData[18] = 0.0;
	}


	//2009.08.14 CYJ
	m_dLength_between_ChipCenter_and_TabEnd = m_plnkPreBonderData->m_dLength_between_ChipCenter_and_TabEnd;
	m_dData[1] = m_dLength_between_ChipCenter_and_TabEnd;
	swprintf(m_cBuf,_T("%.3f"), m_dLength_between_ChipCenter_and_TabEnd);
	m_sData[1].SetCaption(m_cBuf);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛


	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_1"), &szTextTemp);
//Sensor\r\n사용 개수
	GetDlgItem(IDB_TITLE6)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_2"), &szTextTemp);
//TabIC\r\n종류
	GetDlgItem(IDB_TITLE10)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_3"), &szTextTemp);
//TabIC Data 설정
	SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_4"), &szTextTemp);
//일괄\r\n입력
	GetDlgItem(IDB_BATCH_INPUT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_5"), &szTextTemp);
//이전
	GetDlgItem(IDB_TAB_PREVIOUS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_6"), &szTextTemp);
//다음
	GetDlgItem(IDB_TAB_NEXT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_7"), &szTextTemp);
//저장
	GetDlgItem(IDB_TAB_DATA_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_8"), &szTextTemp);
//나가기
	GetDlgItem(IDB_TAB_DATA_CANCEL)->SetWindowText(szTextTemp);

//이전 Work Side
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_9"), &szTextTemp);
	GetDlgItem(IDB_TAB_PREVIOUS)->SetWindowText(szTextTemp);
//다음 Work Side
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_10"), &szTextTemp);
	GetDlgItem(IDB_TAB_NEXT)->SetWindowText(szTextTemp);
//이전 Tab Page
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_11"), &szTextTemp);
	GetDlgItem(IDB_TAB_PREVIOUS3)->SetWindowText(szTextTemp);
//다음 Tab Page
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_12"), &szTextTemp);
	GetDlgItem(IDB_TAB_NEXT3)->SetWindowText(szTextTemp);
//SR Line 허용오차값
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_13"), &szTextTemp);
	GetDlgItem(IDB_TITLE12)->SetWindowText(szTextTemp);
//Tab EndLine 허용오차값
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_14"), &szTextTemp);
	GetDlgItem(IDB_TITLE13)->SetWindowText(szTextTemp);
//Tab EndLine 미보정 오차값
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICDlg_15"), &szTextTemp);
	GetDlgItem(IDB_TITLE14)->SetWindowText(szTextTemp);
	
	

	//101004.KKY_____
	if (FALSE == m_bEditable)
	{
		for (int i=0; i<DEF_TABIC_DLG_MAX_TAB_NUM; i++)
			m_btnTabOn[i].EnableWindow(FALSE);
		GetDlgItem(IDB_TAB_DATA_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDB_BATCH_INPUT)->EnableWindow(FALSE);
	}
	//_______________

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataTabICDlg::OnTitle1() 
{
	ChangeData(0);
	OffsetDisplay();
}

void CDataTabICDlg::OnTitle2() 
{
	ChangeData(1);
}

void CDataTabICDlg::OnTitle3() 
{
	ChangeData(2);
}

void CDataTabICDlg::OnTitle4() 
{
	ChangeData(3);
}

void CDataTabICDlg::OnTitle5() 
{
	ChangeData(4);
}

void CDataTabICDlg::OnTitle6() 
{
	ChangeData(5);
}

void CDataTabICDlg::OnTitle7() 
{
	ChangeData(6);
}

void CDataTabICDlg::OnTitle8() 
{
	ChangeData(7);
}

void CDataTabICDlg::OnTitle9() 
{
	ChangeData(8);
}

void CDataTabICDlg::OnTitle10() 
{
	//101004.KKY_____
	if (FALSE == m_bEditable)
		return;
	//_______________
	if (m_iFuncOnOff)
	{
		m_iFuncOnOff = FALSE;
		m_sData[9].SetCaption(_T("COf"));
	}
	else
	{
		m_iFuncOnOff = TRUE;
		m_sData[9].SetCaption(_T("TCP"));
	}

	UpdateDisplay(m_iWorkSide);
}

void CDataTabICDlg::OnTitle11() 
{
	//101004.KKY_____
	if (FALSE == m_bEditable)
		return;
	//_______________

	memset(m_cTmpBuf, 0, sizeof(m_cTmpBuf));
	
	swprintf(m_cBuf,_T("%d"), m_iBadTabLimitCount);
	if (!GetNumberBox(m_cTmpBuf, 10, m_cBuf))
		return ;
	
	m_iBadTabLimitCount = (int)wcstod(m_cTmpBuf,NULL);
	
	m_sData[10].SetCaption(m_cBuf);

	//BadTab Limit Count를 저장 하시겠습니까?
	int iResult = MyMessageBox(MY_MESSAGE_BOX,40950,_T("BadTab Limit Count Save"), M_ICONQUESTION|M_YESNO);
	if (iResult == IDNO)
		return;

	m_plnkSystemData->m_uiBadTabLimitCount = m_iBadTabLimitCount;
	m_plnkSystemData->WriteData();

	swprintf(m_cBuf,_T("%d"), m_iBadTabLimitCount);
	m_sData[10].SetCaption(m_cBuf);
}

void CDataTabICDlg::OnUse1() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 0);
}

void CDataTabICDlg::OnUse2() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 1);
}

void CDataTabICDlg::OnUse3()
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 2);
}

void CDataTabICDlg::OnUse4() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 3);
}

void CDataTabICDlg::OnUse5() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 4);
}

void CDataTabICDlg::OnUse6() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 5);
}

void CDataTabICDlg::OnUse7() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 6);
}

void CDataTabICDlg::OnUse8() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 7);
}

void CDataTabICDlg::OnUse9() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 8);
}

void CDataTabICDlg::OnUse10() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 9);
}

void CDataTabICDlg::OnUse11() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 10);
}

void CDataTabICDlg::OnUse12() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 11);
}

void CDataTabICDlg::OnUse13() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 12);
}

void CDataTabICDlg::OnUse14() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 13);
}

void CDataTabICDlg::OnUse15() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 14);
}

void CDataTabICDlg::OnUse16() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 15);
}

void CDataTabICDlg::OnUse17() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 16);
}

void CDataTabICDlg::OnUse18() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 17);
}

void CDataTabICDlg::OnUse19() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 18);
}

void CDataTabICDlg::OnUse20() 
{
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 19);
}

void CDataTabICDlg::OnClickUse21() 
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 20);
}

void CDataTabICDlg::OnClickUse22() 
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 21);
}

void CDataTabICDlg::OnClickUse23() 
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 22);
}

void CDataTabICDlg::OnClickUse24() 
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 23);
}

void CDataTabICDlg::OnClickUse25()  
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 24);
}

void CDataTabICDlg::OnClickUse26()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 25);
}

void CDataTabICDlg::OnClickUse27()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 26);
}

void CDataTabICDlg::OnClickUse28()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 27);
}

void CDataTabICDlg::OnClickUse29()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 28);
}

void CDataTabICDlg::OnClickUse30()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 29);
}

void CDataTabICDlg::OnClickUse31()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 30);
}

void CDataTabICDlg::OnClickUse32()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 31);
}

void CDataTabICDlg::OnClickUse33()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 32);
}

void CDataTabICDlg::OnClickUse34()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 33);
}

void CDataTabICDlg::OnClickUse35()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 34);
}

void CDataTabICDlg::OnClickUse36()
{
	// TODO: Add your control notification handler code here
	ChangeData(DEF_TAB_DATA_INDEX_START_OFFSET + 35);
}


void CDataTabICDlg::OnOn1() 
{
	ChangeUse(0);
}

void CDataTabICDlg::OnOn2() 
{
	ChangeUse(1);	
}

void CDataTabICDlg::OnOn3() 
{
	ChangeUse(2);	
}

void CDataTabICDlg::OnOn4() 
{
	ChangeUse(3);	
}

void CDataTabICDlg::OnOn5() 
{
	ChangeUse(4);	
}

void CDataTabICDlg::OnOn6() 
{
	ChangeUse(5);	
}

void CDataTabICDlg::OnOn7() 
{
	ChangeUse(6);	
}

void CDataTabICDlg::OnOn8() 
{
	ChangeUse(7);	
}

void CDataTabICDlg::OnOn9() 
{
	ChangeUse(8);	
}

void CDataTabICDlg::OnOn10() 
{
	ChangeUse(9);	
}

void CDataTabICDlg::OnOn11() 
{
	ChangeUse(10);	
}

void CDataTabICDlg::OnOn12() 
{
	ChangeUse(11);	
}

void CDataTabICDlg::OnOn13() 
{
	ChangeUse(12);	
}

void CDataTabICDlg::OnOn14() 
{
	ChangeUse(13);	
}

void CDataTabICDlg::OnOn15() 
{
	ChangeUse(14);	
}

void CDataTabICDlg::OnOn16() 
{
	ChangeUse(15);	
}

void CDataTabICDlg::OnOn17() 
{
	ChangeUse(16);	
}

void CDataTabICDlg::OnOn18() 
{
	ChangeUse(17);
}

void CDataTabICDlg::OnOn19() 
{
	ChangeUse(18);
}

void CDataTabICDlg::OnOn20() 
{
	ChangeUse(19);
}

void CDataTabICDlg::OnClickOn21() 
{
	// TODO: Add your control notification handler code here
	ChangeUse(20);
}

void CDataTabICDlg::OnClickOn22() 
{
	// TODO: Add your control notification handler code here
	ChangeUse(21);
}

void CDataTabICDlg::OnClickOn23() 
{
	// TODO: Add your control notification handler code here
	ChangeUse(22);
}

void CDataTabICDlg::OnClickOn24() 
{
	// TODO: Add your control notification handler code here
	ChangeUse(23);
}

void CDataTabICDlg::OnClickOn25() 
{
	// TODO: Add your control notification handler code here
	ChangeUse(24);	
}

void CDataTabICDlg::OnClickOn26()
{
	// TODO: Add your control notification handler code here
	ChangeUse(25);
}

void CDataTabICDlg::OnClickOn27()
{
	// TODO: Add your control notification handler code here
	ChangeUse(26);
}

void CDataTabICDlg::OnClickOn28()
{
	// TODO: Add your control notification handler code here
	ChangeUse(27);
}

void CDataTabICDlg::OnClickOn29()
{
	// TODO: Add your control notification handler code here
	ChangeUse(28);
}

void CDataTabICDlg::OnClickOn30()
{
	// TODO: Add your control notification handler code here
	ChangeUse(29);
}

void CDataTabICDlg::OnClickOn31()
{
	// TODO: Add your control notification handler code here
	ChangeUse(30);
}

void CDataTabICDlg::OnClickOn32()
{
	// TODO: Add your control notification handler code here
	ChangeUse(31);
}

void CDataTabICDlg::OnClickOn33()
{
	// TODO: Add your control notification handler code here
	ChangeUse(32);
}

void CDataTabICDlg::OnClickOn34()
{
	// TODO: Add your control notification handler code here
	ChangeUse(33);
}

void CDataTabICDlg::OnClickOn35()
{
	// TODO: Add your control notification handler code here
	ChangeUse(34);
}

void CDataTabICDlg::OnClickOn36()
{
	// TODO: Add your control notification handler code here
	ChangeUse(35);
}

void CDataTabICDlg::OnTabDataSave()
{
	//변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,40951,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	int i = 0;
	double dLeadWidthX					= m_plnkPreBonderData->m_dLead_Width_Xtab;
	unsigned int uiTabPitchCountX		= m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;
	unsigned int uiSprocketPitchCountX	= m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
	unsigned int uiBadTabCountX			= m_plnkPreBonderData->m_uiBad_Tab_Count_Xtab;

	double dLeadWidthY					= m_plnkPreBonderData->m_dLead_Width_Ytab;
	unsigned int uiTabPitchCountY		= m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;
	unsigned int uiSprocketPitchCountY	= m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
	unsigned int uiBadTabCountY			= m_plnkPreBonderData->m_uiBad_Tab_Count_Ytab;

	// Data 배열값을 Tab Data에 할당
	m_plnkTabData->m_iXuTabQty = (int)m_dData[DEF_XU_DATA_INDEX];
	m_plnkTabData->m_iXdTabQty = (int)m_dData[DEF_XD_DATA_INDEX];

	if (m_iWorkSide == WORKING_SIDE_X_UP)
	{
		m_plnkPreBonderData->m_dLead_Width_Xtab					= m_dData[DEF_XU_DATA_INDEX + 3];
		m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab			= (UINT)m_dData[DEF_XU_DATA_INDEX + 4];
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0]	= (UINT)m_dData[DEF_XU_DATA_INDEX + 5];
		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab		= (UINT)m_dData[DEF_XU_DATA_INDEX + 6];
		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab_Second		= (UINT)m_dData[DEF_XU_DATA_INDEX + 19]; //SJ_YYK 150213 Add..
		m_plnkPreBonderData->m_uiAutoChange_Move_Tab_Count_Xtab = (UINT)m_dData[DEF_XU_DATA_INDEX + 7];
		m_plnkPreBonderData->m_uiBad_Tab_Count_Xtab				= (UINT)m_dData[DEF_XU_DATA_INDEX + 8];

		m_plnkPreBonderData->m_dDistTabEgdeToMark_Xtab			= m_dData[DEF_XU_DATA_INDEX + 2];
	}
	else if (m_iWorkSide == WORKING_SIDE_X_DOWN)
	{
		m_plnkPreBonderData->m_dLead_Width_Xtab					= m_dData[DEF_XD_DATA_INDEX + 3];
		m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab			= (UINT)m_dData[DEF_XD_DATA_INDEX + 4];
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0]	= (UINT)m_dData[DEF_XD_DATA_INDEX + 5];
		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab		= (UINT)m_dData[DEF_XD_DATA_INDEX + 6];
		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab_Second		= (UINT)m_dData[DEF_XU_DATA_INDEX + 19]; //SJ_YYK 150213 Add..
		m_plnkPreBonderData->m_uiAutoChange_Move_Tab_Count_Xtab = (UINT)m_dData[DEF_XD_DATA_INDEX + 7];
		m_plnkPreBonderData->m_uiBad_Tab_Count_Xtab				= (UINT)m_dData[DEF_XD_DATA_INDEX + 8];

		m_plnkPreBonderData->m_dDistTabEgdeToMark_Xtab			= m_dData[DEF_XD_DATA_INDEX + 2];
	}

	m_plnkTabData->m_iYlTabQty = static_cast<int>(m_dData[DEF_YL_DATA_INDEX]);
	m_plnkTabData->m_iYrTabQty = static_cast<int>(m_dData[DEF_YR_DATA_INDEX]);
	
	if (m_iWorkSide == WORKING_SIDE_Y_LEFT)
	{
//		m_plnkPreBonderData->m_dLead_Width_Xtab				= m_dData[DEF_YL_DATA_INDEX + 3];
//		m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab		= (UINT)m_dData[DEF_YL_DATA_INDEX + 4];
		m_plnkPreBonderData->m_dLead_Width_Ytab				= m_dData[DEF_YL_DATA_INDEX + 3];
		m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab		= (UINT)m_dData[DEF_YL_DATA_INDEX + 4];
#ifndef DEF_MIRROR_
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0] = (UINT)m_dData[DEF_YL_DATA_INDEX + 5];
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[1] = (UINT)m_dData[DEF_YL_DATA_INDEX + 5];	//@Add
#else
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0] = (UINT)m_dData[DEF_YL_DATA_INDEX + 5];	//@Add
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[1] = (UINT)m_dData[DEF_YL_DATA_INDEX + 5];
#endif
//		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab	= (UINT)m_dData[DEF_YL_DATA_INDEX + 6];
		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab	= (UINT)m_dData[DEF_YL_DATA_INDEX + 6];
		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab_Second	= (UINT)m_dData[DEF_YL_DATA_INDEX + 19];
		m_plnkPreBonderData->m_uiAutoChange_Move_Tab_Count_Xtab = (UINT)m_dData[DEF_YL_DATA_INDEX + 7];
//		m_plnkPreBonderData->m_uiBad_Tab_Count_Xtab			= (UINT)m_dData[DEF_YL_DATA_INDEX + 8];
		m_plnkPreBonderData->m_uiBad_Tab_Count_Ytab			= (UINT)m_dData[DEF_YL_DATA_INDEX + 8];

		m_plnkPreBonderData->m_dDistTabEgdeToMark_Ytab			= m_dData[DEF_YL_DATA_INDEX + 2];
	}
	else if (m_iWorkSide == WORKING_SIDE_Y_RIGHT)
	{
		m_plnkPreBonderData->m_dLead_Width_Ytab				= m_dData[DEF_YR_DATA_INDEX + 3];
		m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab		= (UINT)m_dData[DEF_YR_DATA_INDEX + 4];
#ifndef DEF_MIRROR_
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0] = (UINT)m_dData[DEF_YR_DATA_INDEX + 5];	//@Add
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[1] = (UINT)m_dData[DEF_YR_DATA_INDEX + 5];
#else
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0] = (UINT)m_dData[DEF_YR_DATA_INDEX + 5];
		m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[1] = (UINT)m_dData[DEF_YR_DATA_INDEX + 5];	//@Add
#endif
		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab	= (UINT)m_dData[DEF_YR_DATA_INDEX + 6];
		m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab_Second	= (UINT)m_dData[DEF_YR_DATA_INDEX + 19];
		m_plnkPreBonderData->m_uiAutoChange_Move_Tab_Count_Xtab = (UINT)m_dData[DEF_YR_DATA_INDEX + 7];
		m_plnkPreBonderData->m_uiBad_Tab_Count_Ytab			= (UINT)m_dData[DEF_YR_DATA_INDEX + 8];

		m_plnkPreBonderData->m_dDistTabEgdeToMark_Ytab			= m_dData[DEF_YR_DATA_INDEX + 2];
	}

	for (int i = 0; i < DEF_MAX_TAB; i++)
	{
		m_plnkTabData->m_dXuTabPitch[i] = m_dData[DEF_XU_DATA_INDEX + DEF_TAB_DATA_INDEX_START_OFFSET + i];
		m_plnkTabData->m_bUseXuTab[i]	= m_bUse[DEF_XU_DATA_INDEX + i];

		m_plnkTabData->m_dXdTabPitch[i] = m_dData[DEF_XD_DATA_INDEX + DEF_TAB_DATA_INDEX_START_OFFSET + i];
		m_plnkTabData->m_bUseXdTab[i]	= m_bUse[DEF_XD_DATA_INDEX + i];

		m_plnkTabData->m_dYlTabPitch[i] = m_dData[DEF_YL_DATA_INDEX + DEF_TAB_DATA_INDEX_START_OFFSET + i];
		m_plnkTabData->m_bUseYlTab[i]	= m_bUse[DEF_YL_DATA_INDEX + i];

		m_plnkTabData->m_dYrTabPitch[i] = m_dData[DEF_YR_DATA_INDEX + DEF_TAB_DATA_INDEX_START_OFFSET + i];
		m_plnkTabData->m_bUseYrTab[i]	= m_bUse[DEF_YR_DATA_INDEX + i];
	}

	// Tab Pitch Data 유효성 검증 후 저장 수행.
	// 유효하지 않은 Data 의 경우, Panel Data 추가 수정 요망 메세지 출력
	double dTabPitchSum = 0;
	if (m_plnkPanelData->m_bUseFlag_Xu == TRUE)
	{
		for (int i = 0; i < m_plnkTabData->m_iXuTabQty; i++)
			dTabPitchSum += m_plnkTabData->m_dXuTabPitch[i];
		
		if (m_plnkPanelData->m_dDistance_FiduMark_Xu < dTabPitchSum)
		{
			//Xu Tab Pitch 합산값이 Xu 변 Fidu. Mark 간 거리 값을 초과합니다.
			//현재 Tab Data 설정값을 저장할 수 없습니다.
			//Panel Data 를 수정 후, 다시 Tab Data 를 입력해 주세요.
			MyMessageBox(MY_MESSAGE_BOX,40954,_T("Can not save Tab Data"),M_ICONERROR);
			return;
		}
	}
	
	if (m_plnkPanelData->m_bUseFlag_Xd == TRUE)
	{
		dTabPitchSum = 0;
		for (int i = 0; i < m_plnkTabData->m_iXdTabQty; i++)
			dTabPitchSum += m_plnkTabData->m_dXdTabPitch[i];
		
		if (m_plnkPanelData->m_dDistance_FiduMark_Xd < dTabPitchSum)
		{
			//Xd Tab Pitch 합산값이 Xd 변 Fidu. Mark 간 거리 값을 초과합니다.
			//현재 Tab Data 설정값을 저장할 수 없습니다.
			//Panel Data 를 수정 후, 다시 Tab Data 를 입력해 주세요.
			MyMessageBox(MY_MESSAGE_BOX,40955,_T("Can not save Tab Data"),M_ICONERROR);
			return;
		}
	}
	
	if (m_plnkPanelData->m_bUseFlag_Yl == TRUE)
	{
		dTabPitchSum = 0;
		for (int i = 0; i < m_plnkTabData->m_iYlTabQty; i++)
			dTabPitchSum += m_plnkTabData->m_dYlTabPitch[i];
		
		if (m_plnkPanelData->m_dDistance_FiduMark_Yl < dTabPitchSum)
		{
			//Yl Tab Pitch 합산값이 Yl 변 Fidu. Mark 간 거리 값을 초과합니다.
			//현재 Tab Data 설정값을 저장할 수 없습니다.
			//Panel Data 를 수정 후, 다시 Tab Data 를 입력해 주세요.
			MyMessageBox(MY_MESSAGE_BOX,40956,_T("Can not save Tab Data"),M_ICONERROR);
			return;
		}
	}
	
	if (m_plnkPanelData->m_bUseFlag_Yr == TRUE)
	{
		dTabPitchSum = 0;
		for (int i = 0; i < m_plnkTabData->m_iYrTabQty; i++)
			dTabPitchSum += m_plnkTabData->m_dYrTabPitch[i];
		
		if (m_plnkPanelData->m_dDistance_FiduMark_Yr < dTabPitchSum)
		{
			//Yr Tab Pitch 합산값이 Yr 변 Fidu. Mark 간 거리 값을 초과합니다.
			//현재 Tab Data 설정값을 저장할 수 없습니다.
			//Panel Data 를 수정 후, 다시 Tab Data 를 입력해 주세요.
			MyMessageBox(MY_MESSAGE_BOX,40957,_T("Can not save Tab Data"),M_ICONERROR);
			return;
		}
	}

	/*/
	//MERGE_LKH_090704
	//20090620 sj_ysb
	m_plnkPreBonderData->m_uiPermissionXLeftValue13 = m_dData[31];//m_uiPermissionXLeftValue13;
	m_plnkPreBonderData->m_uiPermissionXRightValue13 = m_dData[32];
	m_plnkPreBonderData->m_uiPermissionXLeftValue24 = m_dData[33];
	m_plnkPreBonderData->m_uiPermissionXRightValue24 = m_dData[34];
	/*/
	m_plnkPreBonderData->m_dToleranceSRLine		= m_dData[11];
	m_plnkPreBonderData->m_dToleranceTabEnd		= m_dData[12];
	m_plnkPreBonderData->m_dNoCareValueTabEnd	= m_dData[13];

	//20120828 sj_ysb
	m_plnkPreBonderData->m_dSRLineMarkDistance = m_dData[15];//m_uiPermissionXLeftValue13;
	m_plnkPreBonderData->m_dTabEndMarkDistance = m_dData[16];
	m_plnkPreBonderData->m_bMarkDistanceFunc = m_bMarkDistanceFunc;
	m_plnkPreBonderData->m_bSRLineAlignFunc = m_bSRLineAlignFunc;
	//2009.08.14 CYJ
	m_plnkPreBonderData->m_dLength_between_ChipCenter_and_TabEnd = m_dData[1];

	m_plnkPreBonderData->m_bUseTCP = m_iFuncOnOff;

	m_plnkPreBonderData->WriteData();
	m_plnkTabData->WriteData();

	//101004.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
		return;
	//_______________

	MOlbSystemPre.GetProcessData()->ChangeProcessData();
	MOlbSystemPre.AssignWorkScheduleData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		MOlbSystemPre.GetWorkSchedule(i)->ChangeModel();
#ifdef DEF_SOURCE_SYSTEM
		MOlbSystemPre.GetWorkScheduleSecond(i)->ChangeModel();
#endif
	}
	//170922 JSh.s
	//회피 위치 만큼 작업 거리에서 재외하기 위해 필요한 중복 코드
	MOlbSystemPre.AssignWorkScheduleData();
 	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
 	{
 		MOlbSystemPre.GetWorkSchedule(i)->ChangeModel();
#ifdef DEF_SOURCE_SYSTEM
 		MOlbSystemPre.GetWorkScheduleSecond(i)->ChangeModel();
#endif
	}
	//170922 JSh.e

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		MOlbSystemPre.GetTabCarrierComponent(i)->ChangeModel();
		MOlbSystemPre.GetRTabCarrierComponent(i)->ChangeModel();
	}

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		MOlbSystemPre.GetCtrlTabFeederComponent(i)->ChangeModel();
	}
	
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		MOlbSystemPre.GetCameraCarrierComponent(i)->ChangeModel();
//@#ifdef DEF_GATE_SYSTEM
		MOlbSystemPre.GetRCameraCarrierComponent(i)->ChangeModel();
//@#endif
	}
	
	MOlbSystemPre.GetPanelAlignerComponent()->ChangeModel();

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Tab 관련 Data 변경에 따른 Sprocket Homing 상태 설정
	if (dLeadWidthX != m_plnkPreBonderData->m_dLead_Width_Xtab
		|| uiTabPitchCountX != m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab
		|| uiSprocketPitchCountX != m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab
		|| uiBadTabCountX != m_plnkPreBonderData->m_uiBad_Tab_Count_Xtab)
	{
		MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetHomeFlagSprocket2(FALSE);
		MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetHomeFlagSprocket2(FALSE);	//@ Add
	}

	if (dLeadWidthY != m_plnkPreBonderData->m_dLead_Width_Ytab
		|| uiTabPitchCountY != m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab
		|| uiSprocketPitchCountY != m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab
		|| uiBadTabCountY != m_plnkPreBonderData->m_uiBad_Tab_Count_Ytab)
	{
		MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER2)->SetHomeFlagSprocket2(FALSE);
		MOlbSystemPre.GetCtrlTabFeederComponent(DEF_TABFEEDER1)->SetHomeFlagSprocket2(FALSE);	//@ Add
	}
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	//130307.kms_______
	// 초기화 Flg를 Off 시키는 코드 필요.
	MOlbSystemPre.GetTrsTabCarrierComponent(0)->ResetInitialized();
	MOlbSystemPre.GetTrsRTabCarrierComponent(0)->ResetInitialized();
	//_________________

	// Report Model Data
	MOlbSystemPre.ReportModel();
}

void CDataTabICDlg::OnBatchInput() 
{
	memset(m_cTmpBuf, 0, sizeof(m_cTmpBuf));

	swprintf(m_cBuf,_T("%.4f"), 0.0);
	if (!GetNumberBox(m_cTmpBuf, 10, m_cBuf))	
		return ;	

	for (int i = 0; i < int(m_dData[m_iWorkSide*DEF_MAX_TAB_DATA]); i++)
	{
		m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + i + DEF_TAB_DATA_INDEX_START_OFFSET] = wcstod(m_cTmpBuf,NULL);
		m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + i + DEF_TAB_DATA_INDEX_START_OFFSET] = fabs(m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + i + DEF_TAB_DATA_INDEX_START_OFFSET]);
		if(i>=DEF_TABIC_DLG_MAX_TAB_NUM)
			continue;
		swprintf(m_cBuf, _T("%.4f"), m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + i + DEF_TAB_DATA_INDEX_START_OFFSET]);
		m_sData[i + DEF_TAB_DATA_INDEX_START_OFFSET].SetCaption(m_cBuf);
	}
}

void CDataTabICDlg::OnTabPrevious() 
{
	m_iTabPage = 0;

	int iNewWorkSide = m_iWorkSide - 1;
	if (iNewWorkSide < 0)
		// jdy sesl 081229	iNewWorkSide = 3;
		return;

	for (int i = 0; i < DEF_MAX_WORK_SIDE; i++)
	{
		if (iNewWorkSide == WORKING_SIDE_X_UP)
		{
			if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_GATE
				&& m_plnkPanelData->m_bUseFlag_Xu)
				break;
		}
		else if (iNewWorkSide == WORKING_SIDE_X_DOWN)
		{
			if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_GATE
				&& m_plnkPanelData->m_bUseFlag_Xd)
				break;
		}
		else if (iNewWorkSide == WORKING_SIDE_Y_LEFT)
		{
			if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_SOURCE
				&& m_plnkPanelData->m_bUseFlag_Yl)
				break;
		}
		else if (iNewWorkSide == WORKING_SIDE_Y_RIGHT)
		{
			if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_SOURCE
				&& m_plnkPanelData->m_bUseFlag_Yr)
				break;
		}
		
		iNewWorkSide--;
		if (iNewWorkSide < 0)
			// jdy sesl 081229	iNewWorkSide = 3;
			return;
	}

	if (m_iWorkSide != iNewWorkSide)
	{
		m_iWorkSide = iNewWorkSide;
		UpdateDisplay(m_iWorkSide);
	}
}

void CDataTabICDlg::OnTabNext()
{
	m_iTabPage = 0;
	int iNewWorkSide = m_iWorkSide + 1;
	if (iNewWorkSide > 3)
		// jdy sesl 081229	iNewWorkSide = 0;
		return;

	for (int i = 0; i < DEF_MAX_WORK_SIDE; i++)
	{
		if (iNewWorkSide == WORKING_SIDE_X_UP)
		{
			if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_GATE
				&& m_plnkPanelData->m_bUseFlag_Xu)
				break;
		}
		else if (iNewWorkSide == WORKING_SIDE_X_DOWN)
		{
			if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_GATE
				&& m_plnkPanelData->m_bUseFlag_Xd)
				break;
		}
		else if (iNewWorkSide == WORKING_SIDE_Y_LEFT)
		{
			if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_SOURCE
				&& m_plnkPanelData->m_bUseFlag_Yl)
				break;
		}
		else if (iNewWorkSide == WORKING_SIDE_Y_RIGHT)
		{
			if (m_plnkSystemData->m_eSystemType != SYSTEM_TYPE_SOURCE
				&& m_plnkPanelData->m_bUseFlag_Yr)
				break;
		}
		
		iNewWorkSide++;
		if (iNewWorkSide > 3)
			// jdy sesl 081229	iNewWorkSide = 0;
			return;
	}

	if (m_iWorkSide != iNewWorkSide)
	{
		m_iWorkSide = iNewWorkSide;
		UpdateDisplay(m_iWorkSide);
	}
}

void CDataTabICDlg::OnTabDataCancel() 
{
	CDialog::OnCancel();	
}

void CDataTabICDlg::UpdateDisplay(int iWorkSideType)
{
	// jdy sesl 081229
	if (WORKING_SIDE_UNKNOWN == iWorkSideType)
	{
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
			m_iWorkSide = WORKING_SIDE_X_UP;
		else
			m_iWorkSide = WORKING_SIDE_Y_LEFT;
	}
	else
		m_iWorkSide = iWorkSideType;

	memset(m_dData, 0, sizeof(m_dData));
	memset(m_bUse, 0, sizeof(m_bUse));

	m_dData[DEF_XU_DATA_INDEX] = m_plnkTabData->m_iXuTabQty;
	m_dData[DEF_XD_DATA_INDEX] = m_plnkTabData->m_iXdTabQty;

	m_dData[DEF_XU_DATA_INDEX + 1] = m_dData[DEF_XD_DATA_INDEX + 1] = 0.0;
	m_dData[DEF_XU_DATA_INDEX + 2] = m_dData[DEF_XD_DATA_INDEX + 2] = m_plnkPreBonderData->m_dDistTabEgdeToMark_Xtab;
	m_dData[DEF_XU_DATA_INDEX + 3] = m_dData[DEF_XD_DATA_INDEX + 3] = m_plnkPreBonderData->m_dLead_Width_Xtab;
	m_dData[DEF_XU_DATA_INDEX + 4] = m_dData[DEF_XD_DATA_INDEX + 4] = m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab;
	m_dData[DEF_XU_DATA_INDEX + 5] = m_dData[DEF_XD_DATA_INDEX + 5] = m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0];
	m_dData[DEF_XU_DATA_INDEX + 6] = m_dData[DEF_XD_DATA_INDEX + 6] = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab;
	m_dData[DEF_XU_DATA_INDEX + 19] = m_dData[DEF_XD_DATA_INDEX + 19] = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Xtab_Second; //SJ_YYK 150213 Add.
	m_dData[DEF_XU_DATA_INDEX + 7] = m_dData[DEF_XD_DATA_INDEX + 7] = m_plnkPreBonderData->m_uiAutoChange_Move_Tab_Count_Xtab;
	m_dData[DEF_XU_DATA_INDEX + 8] = m_dData[DEF_XD_DATA_INDEX + 8] = m_plnkPreBonderData->m_uiBad_Tab_Count_Xtab;

	m_dData[DEF_YL_DATA_INDEX] = m_plnkTabData->m_iYlTabQty;
	m_dData[DEF_YR_DATA_INDEX] = m_plnkTabData->m_iYrTabQty;

	m_dData[DEF_YL_DATA_INDEX + 1] = m_dData[DEF_YR_DATA_INDEX + 1] = 0.0;
	m_dData[DEF_YL_DATA_INDEX + 2] = m_dData[DEF_YR_DATA_INDEX + 2] = m_plnkPreBonderData->m_dDistTabEgdeToMark_Ytab;
	m_dData[DEF_YL_DATA_INDEX + 3] = m_dData[DEF_YR_DATA_INDEX + 3] = m_plnkPreBonderData->m_dLead_Width_Ytab;
	m_dData[DEF_YL_DATA_INDEX + 4] = m_dData[DEF_YR_DATA_INDEX + 4] = m_plnkPreBonderData->m_uiTab_Pitch_Count_Ytab;
#ifndef DEF_MIRROR_
	m_dData[DEF_YL_DATA_INDEX + 5] = m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0];
	m_dData[DEF_YR_DATA_INDEX + 5] = m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0];//@m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[1];
#else
	m_dData[DEF_YL_DATA_INDEX + 5] = m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0];//@m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[1];
	m_dData[DEF_YR_DATA_INDEX + 5] = m_plnkPreBonderData->m_rgiNumberOfUsingSensorForWorker[0];
#endif
	m_dData[DEF_YL_DATA_INDEX + 6] = m_dData[DEF_YR_DATA_INDEX + 6] = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab;
	m_dData[DEF_YL_DATA_INDEX + 19] = m_dData[DEF_YR_DATA_INDEX + 19] = m_plnkPreBonderData->m_uiSprocket_Pitch_Count_Ytab_Second; //SJ_YYK 150213 Add..
	m_dData[DEF_YL_DATA_INDEX + 7] = m_dData[DEF_YR_DATA_INDEX + 7] = m_plnkPreBonderData->m_uiAutoChange_Move_Tab_Count_Xtab;
	m_dData[DEF_YL_DATA_INDEX + 8] = m_dData[DEF_YR_DATA_INDEX + 8] = m_plnkPreBonderData->m_uiBad_Tab_Count_Ytab;

	int i;
	for (i = 0; i < DEF_MAX_TAB; i++) 
	{
		m_dData[DEF_XU_DATA_INDEX + i + DEF_TAB_DATA_INDEX_START_OFFSET] = m_plnkTabData->m_dXuTabPitch[i];
		m_dData[DEF_XD_DATA_INDEX + i + DEF_TAB_DATA_INDEX_START_OFFSET] = m_plnkTabData->m_dXdTabPitch[i];
		m_dData[DEF_YL_DATA_INDEX + i + DEF_TAB_DATA_INDEX_START_OFFSET] = m_plnkTabData->m_dYlTabPitch[i];
		m_dData[DEF_YR_DATA_INDEX + i + DEF_TAB_DATA_INDEX_START_OFFSET] = m_plnkTabData->m_dYrTabPitch[i];
	}

	for (i = 0; i < DEF_MAX_TAB; i++) 
	{
		m_bUse[DEF_XU_DATA_INDEX + i] = m_plnkTabData->m_bUseXuTab[i];
		if (m_bUse[DEF_XU_DATA_INDEX + i] == -99)
			m_bUse[DEF_XU_DATA_INDEX + i] = 0;
		
		m_bUse[DEF_XD_DATA_INDEX + i] = m_plnkTabData->m_bUseXdTab[i];
		if (m_bUse[DEF_XD_DATA_INDEX + i] == -99)
			m_bUse[DEF_XD_DATA_INDEX + i] = 0;
		
		m_bUse[DEF_YL_DATA_INDEX + i] = m_plnkTabData->m_bUseYlTab[i];
		if (m_bUse[DEF_YL_DATA_INDEX + i] == -99)
			m_bUse[DEF_YL_DATA_INDEX + i] = 0;
		
		m_bUse[DEF_YR_DATA_INDEX + i] = m_plnkTabData->m_bUseYrTab[i];
		if (m_bUse[DEF_YR_DATA_INDEX + i] == -99)
			m_bUse[DEF_YR_DATA_INDEX + i] = 0;
	}

	for ( ; i < DEF_TABIC_DLG_MAX_TAB_NUM; i++)
	{
		m_bUse[DEF_XU_DATA_INDEX + i] = FALSE;
		m_bUse[DEF_XD_DATA_INDEX + i] = FALSE;
		m_bUse[DEF_YL_DATA_INDEX + i] = FALSE;
		m_bUse[DEF_YR_DATA_INDEX + i] = FALSE;
	}

	WCHAR cbuf[DEF_MAX_SET_DATA_NUM][DEF_TABIC_DLG_MAX_TAB_NUM];

	for (int j = 0; j < DEF_MAX_SET_DATA_NUM; j++) 
	{
		if (j == 0		// Tab 수
			|| j == 4	// Tab Pitch
			|| j == 5	// Bad Tab Sensor 사용 개수
			|| j == 6	// Sprocket Pitch
			|| j == 7	// Auto Chagne Move Pitch Count
			|| j == 8	// Bad Tab Count
			|| j == 9	// Bad Tab Limit Count
			|| j == 19)	// Sprocket2 Pitch //SJ_YYK 150213
		{
			swprintf(cbuf[j], _T("%d"), int(m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + j]));
		}
		/*/
		else if (j == 31)
		{
			if (m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + j] == 1.0)	// Yes
			{
				swprintf(cbuf[j], _T("Yes"));
			}
			else
			{
				swprintf(cbuf[j], _T("No"));
			}
		}
		/*/
		else if (j == 17 || j == 18)
		{
			if (m_dData[j] == 1.0)	// Yes
			{
				swprintf(cbuf[j], _T("ON"));
			}
			else
			{
				swprintf(cbuf[j], _T("OFF"));
			}
		}
		else 
		{
			if(j < DEF_TAB_DATA_INDEX_START_OFFSET)
			{
				swprintf(cbuf[j], _T("%.4f"), m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + j]);
			}
			else
			{
				if(m_iTabPage == 0)
				{
					swprintf(cbuf[j], _T("%.4f"), m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + j]);
				}
				else
				{
					swprintf(cbuf[j], _T("%.4f"), m_dData[m_iWorkSide*DEF_MAX_TAB_DATA + j + DEF_TABIC_DLG_MAX_TAB_NUM]);
				}
			}
		}
	}

	// Min_Button 배열 초기화
	for (int k = 0; k < DEF_MAX_SET_DATA_NUM; k++)
	{
		if (k != 9 && k != 10)
			m_sData[k].SetCaption(cbuf[k]);
	}
	
	m_bPrevious.ShowWindow(SW_SHOW);
	m_bNext.ShowWindow(SW_SHOW);
	
	int iIndexBase = -1;
	CString strWorkSideName;
	switch (m_iWorkSide)
	{
	case WORKING_SIDE_X_UP:
		GetDlgItem(IDS_TITLE9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_TITLE9)->ShowWindow(SW_SHOW);
		strWorkSideName = "Xu";
		iIndexBase = DEF_XU_DATA_INDEX;
		break;

	case WORKING_SIDE_X_DOWN:
//@		GetDlgItem(IDS_TITLE9)->ShowWindow(SW_HIDE);
//@		GetDlgItem(IDB_TITLE9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDS_TITLE9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_TITLE9)->ShowWindow(SW_SHOW);
		strWorkSideName = "Xd";
		iIndexBase = DEF_XD_DATA_INDEX;
		break;

	case WORKING_SIDE_Y_LEFT:
		GetDlgItem(IDS_TITLE9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_TITLE9)->ShowWindow(SW_SHOW);
		strWorkSideName = "Yl";
		iIndexBase = DEF_YL_DATA_INDEX;
		break;

	case WORKING_SIDE_Y_RIGHT:
		GetDlgItem(IDS_TITLE9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDB_TITLE9)->ShowWindow(SW_SHOW);
		strWorkSideName = "Yr";
		iIndexBase = DEF_YR_DATA_INDEX;
		break;
	}
	m_btnWorkTabNum.SetCaption(strWorkSideName + _T("Tab Number"));
	GetDlgItem(IDC_TAB_DATA_WORKSIDE_TITLE)->SetWindowText(strWorkSideName + _T("Pitch Data"));

	for (int i = 0; i < DEF_TABIC_DLG_MAX_TAB_NUM; i++) 
	{
		if(m_iTabPage ==0)
			m_btnTabOn[i].SetValue(m_bUse[iIndexBase + i]);
		else
			m_btnTabOn[i].SetValue(m_bUse[iIndexBase + i + DEF_TABIC_DLG_MAX_TAB_NUM]);
	}
	
	if (m_iFuncOnOff)
		m_sData[9].SetCaption(_T("TCP"));
	else
		m_sData[9].SetCaption(_T("COF"));

	m_dData[11] = m_plnkPreBonderData->m_dToleranceSRLine;
	swprintf(m_cBuf,_T("%.3f"), m_dData[11]);
	m_sData[11].SetCaption(m_cBuf);

	m_dData[12] = m_plnkPreBonderData->m_dToleranceTabEnd;
	swprintf(m_cBuf,_T("%.3f"), m_dData[12]);
	m_sData[12].SetCaption(m_cBuf);

	m_dData[13] = m_plnkPreBonderData->m_dNoCareValueTabEnd;
	swprintf(m_cBuf,_T("%.3f"), m_dData[13]);
	m_sData[13].SetCaption(m_cBuf);

	//2009.08.14 CYJ
	m_dLength_between_ChipCenter_and_TabEnd = m_plnkPreBonderData->m_dLength_between_ChipCenter_and_TabEnd;
	m_dData[1] = m_dLength_between_ChipCenter_and_TabEnd;
	swprintf(m_cBuf,_T("%.3f"), m_dLength_between_ChipCenter_and_TabEnd);
	m_sData[1].SetCaption(m_cBuf);
	m_dData[15] = m_plnkPreBonderData->m_dSRLineMarkDistance;
	swprintf(m_cBuf,_T("%.4f"), m_dData[15]);
	m_sData[15].SetCaption(m_cBuf);

	m_dData[16] = m_plnkPreBonderData->m_dTabEndMarkDistance;
	swprintf(m_cBuf,_T("%.4f"), m_dData[16]);
	m_sData[16].SetCaption(m_cBuf);

	if (m_bMarkDistanceFunc)
	{
		m_sData[17].SetCaption(_T("ON"));
		m_dData[17] = 1.0;
	}
	else
	{
		m_sData[17].SetCaption(_T("OFF"));
		m_dData[17] = 0.0;
	}

	if (m_bSRLineAlignFunc)
	{
		m_sData[18].SetCaption(_T("ON"));
		m_dData[18] = 1.0;
	}
	else
	{
		m_sData[18].SetCaption(_T("OFF"));
		m_dData[18] = 0.0;
	}

	GetDlgItem(IDS_TITLE15)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_TITLE15)->ShowWindow(SW_HIDE);
	
	OffsetDisplay();
}

void CDataTabICDlg::OffsetDisplay()
{
	int iValue;
	int iUseBtnValue[DEF_TABIC_DLG_MAX_TAB_NUM] =
	{
		PDB_USE_1, PDB_USE_2, PDB_USE_3, PDB_USE_4, PDB_USE_5,
		PDB_USE_6, PDB_USE_7, PDB_USE_8, PDB_USE_9, PDB_USE_10,
		PDB_USE_11, PDB_USE_12, PDB_USE_13, PDB_USE_14, PDB_USE_15,
		PDB_USE_16, PDB_USE_17, PDB_USE_18, PDB_USE_19, PDB_USE_20,
		PDB_USE_21, PDB_USE_22, PDB_USE_23, PDB_USE_24, PDB_USE_25,
		PDB_USE_26, PDB_USE_27, PDB_USE_28, PDB_USE_29, PDB_USE_30,
		PDB_USE_31, PDB_USE_32, PDB_USE_33, PDB_USE_34, PDB_USE_35,
		PDB_USE_36,
	};

	int iOnBtnValue[DEF_TABIC_DLG_MAX_TAB_NUM] =
	{
		PDB_ON_1, PDB_ON_2, PDB_ON_3, PDB_ON_4, PDB_ON_5,
		PDB_ON_6, PDB_ON_7, PDB_ON_8, PDB_ON_9, PDB_ON_10,
		PDB_ON_11, PDB_ON_12, PDB_ON_13, PDB_ON_14, PDB_ON_15,
		PDB_ON_16, PDB_ON_17, PDB_ON_18, PDB_ON_19, PDB_ON_20,
		PDB_ON_21, PDB_ON_22, PDB_ON_23, PDB_ON_24, PDB_ON_25,
		PDB_ON_26, PDB_ON_27, PDB_ON_28, PDB_ON_29, PDB_ON_30,
		PDB_ON_31, PDB_ON_32, PDB_ON_33, PDB_ON_34, PDB_ON_35,
		PDB_ON_36,
	};

	int iUseStr[DEF_TABIC_DLG_MAX_TAB_NUM] =
	{
		PDS_USE_1, PDS_USE_2, PDS_USE_3, PDS_USE_4, PDS_USE_5,
		PDS_USE_6, PDS_USE_7, PDS_USE_8, PDS_USE_9, PDS_USE_10,
		PDS_USE_11, PDS_USE_12, PDS_USE_13, PDS_USE_14, PDS_USE_15,
		PDS_USE_16, PDS_USE_17, PDS_USE_18, PDS_USE_19, PDS_USE_20,
		PDS_USE_21, PDS_USE_22, PDS_USE_23, PDS_USE_24, PDS_USE_25,
		PDS_USE_26, PDS_USE_27, PDS_USE_28, PDS_USE_29, PDS_USE_30,
		PDS_USE_31, PDS_USE_32, PDS_USE_33, PDS_USE_34, PDS_USE_35,
		PDS_USE_36,
	};

	iValue = int(m_dData[DEF_MAX_TAB_DATA*m_iWorkSide]);

	switch (m_iWorkSide)
	{
	case WORKING_SIDE_X_UP:
		if (!m_plnkTabData->m_bUseXuTab)
			iValue = 0;
		break;
	case WORKING_SIDE_X_DOWN:
		if (!m_plnkTabData->m_bUseXdTab)
			iValue = 0;
		break;
	case WORKING_SIDE_Y_LEFT:
		if (!m_plnkTabData->m_bUseYlTab)
			iValue = 0;
		break;
	case WORKING_SIDE_Y_RIGHT:
		if (!m_plnkTabData->m_bUseYrTab)
			iValue = 0;
		break;
	}

	if(m_iTabPage == 0)
	{
		for (int i = 0; i < iValue; i++)
		{
			if(i >= DEF_TABIC_DLG_MAX_TAB_NUM)
				continue;
			GetDlgItem(iUseBtnValue[i])->ShowWindow(SW_SHOW);
			GetDlgItem(iOnBtnValue[i])->ShowWindow(SW_SHOW);
			GetDlgItem(iUseStr[i])->ShowWindow(SW_SHOW);
		}
		
		for (int i = iValue; i < DEF_TABIC_DLG_MAX_TAB_NUM; i++)
		{
			GetDlgItem(iUseBtnValue[i])->ShowWindow(SW_HIDE);
			GetDlgItem(iOnBtnValue[i])->ShowWindow(SW_HIDE);
			GetDlgItem(iUseStr[i])->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		for (int i = 0; i < iValue - DEF_TABIC_DLG_MAX_TAB_NUM; i++)
		{
			GetDlgItem(iUseBtnValue[i])->ShowWindow(SW_SHOW);
			GetDlgItem(iOnBtnValue[i])->ShowWindow(SW_SHOW);
			GetDlgItem(iUseStr[i])->ShowWindow(SW_SHOW);
		}
		
		for (int i = iValue - DEF_TABIC_DLG_MAX_TAB_NUM; i < DEF_TABIC_DLG_MAX_TAB_NUM; i++)
		{
			GetDlgItem(iUseBtnValue[i])->ShowWindow(SW_HIDE);
			GetDlgItem(iOnBtnValue[i])->ShowWindow(SW_HIDE);
			GetDlgItem(iUseStr[i])->ShowWindow(SW_HIDE);
		}
	}

	//21031024 SJ_HJG
	CString szTextTemp;
	for (int i = 0; i < DEF_TABIC_DLG_MAX_TAB_NUM; i++)
	{
		if(m_iTabPage == 0)
			szTextTemp.Format(_T("P%d"),i+1);			
		else
			szTextTemp.Format(_T("P%d"),i+1 + DEF_TABIC_DLG_MAX_TAB_NUM);			
		GetDlgItem(iUseBtnValue[i])->SetWindowText(szTextTemp);
	}

	//_______________
}

void CDataTabICDlg::ChangeData(int iSel)
{
	//101004.KKY_____
	if (FALSE == m_bEditable)
		return;
	//_______________

	memset(m_cTmpBuf, 0, sizeof(m_cTmpBuf));
	
	swprintf(m_cBuf,_T("%.4f"), m_dData[DEF_MAX_TAB_DATA * m_iWorkSide + iSel]);
	if (!GetNumberBox(m_cTmpBuf, 10, m_cBuf))	
		return ;

	if (iSel == 1
		|| iSel == 15
		|| iSel == 16
		|| iSel == 17
		|| iSel == 18
		|| iSel == 11
		|| iSel == 12
		|| iSel == 13
		|| iSel == 14
	)
	{
		m_dData[iSel] = wcstod(m_cTmpBuf,NULL);
	}
	else
	{
		if(iSel < DEF_TAB_DATA_INDEX_START_OFFSET)
		{
			m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = wcstod(m_cTmpBuf,NULL);
		}
		else
		{
			if(m_iTabPage == 0)
			{
				m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = wcstod(m_cTmpBuf,NULL);
			}
			else
			{
				m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel + DEF_TABIC_DLG_MAX_TAB_NUM] = wcstod(m_cTmpBuf,NULL);
			}
		}
	}
	
	if (iSel == 0)
	{
		/*/
		if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType 
//@			&& m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] > DEF_MAX_WORKER_PER_GROUP * 2)
			&& m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] > DEF_MAX_WORKER_PER_GROUP * 5)
		{
//@			m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = DEF_MAX_WORKER_PER_GROUP * 2;
			m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = DEF_MAX_WORKER_PER_GROUP * 5;
			//입력값이 작업 가능 Tab 개수를 초과함. [최대값으로 강제 설정]
			MyMessageBox(MY_MESSAGE_BOX,40952,_T("Error"), M_ICONERROR);
		}
		else if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType
//@			&& m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] > DEF_MAX_WORKER_PER_GROUP * 2)
			&& m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] > DEF_MAX_WORKER_PER_GROUP * 5)
		{
//@			m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = DEF_MAX_WORKER_PER_GROUP * 2;
			m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = DEF_MAX_WORKER_PER_GROUP * 5;
			//입력값이 작업 가능 Tab 개수를 초과함. [최대값으로 강제 설정]
			MyMessageBox(MY_MESSAGE_BOX,40953,_T("Error"), M_ICONERROR);
		}
		/*/
		if (m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] > DEF_MAX_TAB)
		{
			m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = DEF_MAX_TAB;
			//입력값이 작업 가능 Tab 개수를 초과함. [최대값으로 강제 설정]
			MyMessageBox(MY_MESSAGE_BOX,40953,_T("Error"), M_ICONERROR);
		}
	}
	else if (iSel == 8)
	{
		if (m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] > DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS)
			m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = DEF_MAX_TABIC_NUM_BETWEEN_MOLD_AND_PRESS;
	}
	else if (iSel == 5)
	{
		if (1 != m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel]
			&& 2 != m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel])
		{
			m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = 1.0;
		}
	}

	//MERGE_LKH_090704
	if (iSel != 11
		&& iSel != 12
		&& iSel != 13
		&& iSel != 14
		&& iSel != 1  // 2009.08.14 CYJ
		&& iSel != 15
		&& iSel != 16
		&& iSel != 17
		&& iSel != 18
		)
	{
		m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = fabs(m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel]);
	}
	/*
	else
	{
		m_dData[iSel] = m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel];
	}
	*/

	/*/
	if (iSel == 15
		|| iSel == 16
		|| iSel == 17
		|| iSel == 18
	)
	{
		m_dData[15 + iSel] = m_dData[iSel];
	}
	/*/

	switch (iSel)
	{
	case 0:	// Tab 수
	case 4:	// Tab Pitch
	case 5:	// Bad Tab Sensor 사용 개수
	case 6:	// Sprocket Pitch
	case 7:	// Auto Chagne Move Pitch Count
	case 8:	// Bad Tab Count
	case 9:	// Bad Tab Limit Count
	case 19: //Sprocket 2 Pitch //SJ_YYK 150213
		swprintf(m_cBuf, _T("%.0f"), m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel]);
		break;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 1:
		swprintf(m_cBuf, _T("%.4f"), m_dData[iSel]);
		break;

	default:
		if(iSel < DEF_TAB_DATA_INDEX_START_OFFSET)
		{
			swprintf(m_cBuf, _T("%.4f"), m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel]);
		}
		else
		{
			if(m_iTabPage == 0)
			{
				swprintf(m_cBuf, _T("%.4f"), m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel]);
			}
			else
			{
				swprintf(m_cBuf, _T("%.4f"), m_dData[m_iWorkSide * DEF_MAX_TAB_DATA + iSel + DEF_TABIC_DLG_MAX_TAB_NUM]);
			}
		}
		break;
	}	
	
	m_sData[iSel].SetCaption(m_cBuf);
}

void CDataTabICDlg::ChangeUse(int iSel)
{
	BOOL bNew;
	if(m_iTabPage == 0)
	{
		bNew = !!!(m_bUse[m_iWorkSide * DEF_MAX_TAB_DATA + iSel]);
		m_bUse[m_iWorkSide * DEF_MAX_TAB_DATA + iSel] = bNew;
	}
	else
	{
		bNew = !!!(m_bUse[m_iWorkSide * DEF_MAX_TAB_DATA + iSel + DEF_TABIC_DLG_MAX_TAB_NUM]);
		m_bUse[m_iWorkSide * DEF_MAX_TAB_DATA + iSel + DEF_TABIC_DLG_MAX_TAB_NUM] = bNew;
	}

	m_btnTabOn[iSel].SetValue(bNew);
}

void CDataTabICDlg::OnTabPitch() 
{
	
}

void CDataTabICDlg::OnClickImageView() 
{
	CDataTabICImageDlg dlg(m_iWorkSide);
	dlg.DoModal();
}

void CDataTabICDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}
	//MERGE_LKH_090704
void CDataTabICDlg::OnClickTitle12() 
{
	// TODO: Add your control notification handler code here
	ChangeData(11);	
}

void CDataTabICDlg::OnClickTitle13() 
{
	// TODO: Add your control notification handler code here
	ChangeData(12);
	
}

void CDataTabICDlg::OnClickTitle14() 
{
	// TODO: Add your control notification handler code here
	ChangeData(13);
	
}

void CDataTabICDlg::OnClickTitle15() 
{
	// TODO: Add your control notification handler code here
	ChangeData(14);
	
}

void CDataTabICDlg::OnClickTitle16() 
{
	// TODO: Add your control notification handler code here
	ChangeData(15);
	
}

void CDataTabICDlg::OnClickTitle17() 
{
	// TODO: Add your control notification handler code here
	ChangeData(16);
}

void CDataTabICDlg::OnClickTitle19() 
{
	// TODO: Add your control notification handler code here
	
	//ChangeData(37);
	if (!m_bSRLineAlignFunc)
	{
		m_sData[18].SetCaption(_T("ON"));
		m_dData[18] = 1.0;
		m_bSRLineAlignFunc = TRUE;
	}
	else
	{
		m_sData[18].SetCaption(_T("OFF"));
		m_dData[18] = 0.0;
		m_bSRLineAlignFunc = FALSE;
	}
}

void CDataTabICDlg::OnClickTitle18() 
{
	// TODO: Add your control notification handler code here
	if (!m_bMarkDistanceFunc)
	{
		m_sData[17].SetCaption(_T("ON"));
		m_dData[17] = 1.0;
		m_bMarkDistanceFunc = TRUE;
	}
	else
	{
		m_sData[17].SetCaption(_T("OFF"));
		m_dData[17] = 0.0;
		m_bMarkDistanceFunc = FALSE;
	}	
}

void CDataTabICDlg::OnClickTabPrevious3() 
{
	// TODO: Add your control notification handler code here
	if(m_iTabPage == 0	)
		return;

	switch (m_iWorkSide)
	{
	case WORKING_SIDE_X_UP:	
		if(m_plnkTabData->m_iXuTabQty <= DEF_TABIC_DLG_MAX_TAB_NUM)
			return;

		break;
		
	case WORKING_SIDE_X_DOWN:
		if(m_plnkTabData->m_iXdTabQty <= DEF_TABIC_DLG_MAX_TAB_NUM)
			return;
		break;

	case WORKING_SIDE_Y_LEFT:	
		if(m_plnkTabData->m_iYlTabQty <= DEF_TABIC_DLG_MAX_TAB_NUM)
			return;
		break;
		
	case WORKING_SIDE_Y_RIGHT:
		if(m_plnkTabData->m_iYrTabQty <= DEF_TABIC_DLG_MAX_TAB_NUM)
			return;
		
		break;
	}

	m_iTabPage--;
	UpdateDisplay(m_iWorkSide);
	
}

void CDataTabICDlg::OnClickTabNext3() 
{
	// TODO: Add your control notification handler code here
	if(m_iTabPage == 1	)
		return;

	switch (m_iWorkSide)
	{
	case WORKING_SIDE_X_UP:	
		if(m_plnkTabData->m_iXuTabQty <= DEF_TABIC_DLG_MAX_TAB_NUM)
			return;

		break;
		
	case WORKING_SIDE_X_DOWN:
		if(m_plnkTabData->m_iXdTabQty <= DEF_TABIC_DLG_MAX_TAB_NUM)
			return;
		break;

	case WORKING_SIDE_Y_LEFT:	
		if(m_plnkTabData->m_iYlTabQty <= DEF_TABIC_DLG_MAX_TAB_NUM)
			return;
		break;
		
	case WORKING_SIDE_Y_RIGHT:
		if(m_plnkTabData->m_iYrTabQty <= DEF_TABIC_DLG_MAX_TAB_NUM)
			return;
		
		break;
	}

	m_iTabPage++;
	UpdateDisplay(m_iWorkSide);
}

void CDataTabICDlg::OnClickTitle20() 
{
	// TODO: Add your control notification handler code here
	ChangeData(19);
}
