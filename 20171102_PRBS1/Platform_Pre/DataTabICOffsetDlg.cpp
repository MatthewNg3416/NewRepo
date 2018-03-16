// TabOffsetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "DataTabICOffsetDlg.h"
#include "MPlatformOlbSystem.h"
#include "MPanelData.h"
#include "MTabData.h"
#include "MTabOffsetData.h"
#include "DataTabOffsetImageDlg.h"
#include "MTrsAutoManager.h"
#include "DlgTabOffsetTable.h"
#include <MATH.H>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDataTabICOffsetDlg dialog

CDataTabICOffsetDlg::CDataTabICOffsetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataTabICOffsetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataTabICOffsetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_plnkPanelData = MOlbSystemPre.GetPanelData();
	m_plnkSystemData = MOlbSystemPre.GetSystemData();
	m_plnkTabData = MOlbSystemPre.GetTabData();
	m_plnkTabOffset = MOlbSystemPre.GetTabOffsetData();
	m_plnkTabOffset_Rear = MOlbSystemPre.GetTabOffsetData_Rear();

	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)
	{
		if (TRUE == m_plnkPanelData->m_bUseFlag_Xu)
			m_iWorkSide = WORKING_SIDE_X_UP;
		else // if (TRUE == m_plnkPanelData->m_bUseFlag_Xd)
			m_iWorkSide = WORKING_SIDE_X_DOWN;
	}
	else // if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
	{
		if (TRUE == m_plnkPanelData->m_bUseFlag_Yl)
			m_iWorkSide = WORKING_SIDE_Y_LEFT;
		else // if (TRUE == m_plnkPanelData->m_bUseFlag_Yr)
			m_iWorkSide = WORKING_SIDE_Y_RIGHT;
	}

	m_iWorkGroup = DEF_FRONT_GROUP;
}

void CDataTabICOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataTabICOffsetDlg)
	DDX_Control(pDX, IDB_BATCHY_INPUT, m_bBatchY);
	DDX_Control(pDX, IDB_BATCHX_INPUT, m_bBatchX);
	DDX_Control(pDX, IDB_TAB_NEXT, m_bNext);
	DDX_Control(pDX, IDB_TAB_PREVIOUS, m_bPrevious);
	DDX_Control(pDX, IDB_TAB_DATA_SAVE, m_bSave);
	DDX_Control(pDX, IDB_TAB_DATA_CANCEL, m_bExit);
	DDX_Control(pDX, IDB_TAB_OFFSET_TABLE, m_bTabOffsetTable);
	
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDB_USE_1, m_bBtn[0]);
	DDX_Control(pDX, IDB_USE_2, m_bBtn[1]);
	DDX_Control(pDX, IDB_USE_3, m_bBtn[2]);
	DDX_Control(pDX, IDB_USE_4, m_bBtn[3]);
	DDX_Control(pDX, IDB_USE_5, m_bBtn[4]);
	DDX_Control(pDX, IDB_USE_6, m_bBtn[5]);
	DDX_Control(pDX, IDB_USE_7, m_bBtn[6]);
	DDX_Control(pDX, IDB_USE_8, m_bBtn[7]);
	DDX_Control(pDX, IDB_USE_9, m_bBtn[8]);
	DDX_Control(pDX, IDB_USE_10,m_bBtn[9]);
	DDX_Control(pDX, IDB_USE_11, m_bBtn[10]);
	DDX_Control(pDX, IDB_USE_12, m_bBtn[11]);
	DDX_Control(pDX, IDB_USE_13, m_bBtn[12]);
	DDX_Control(pDX, IDB_USE_14, m_bBtn[13]);
	DDX_Control(pDX, IDB_USE_15, m_bBtn[14]);
	DDX_Control(pDX, IDB_USE_16, m_bBtn[15]);
	DDX_Control(pDX, IDB_USE_17, m_bBtn[16]);
	DDX_Control(pDX, IDB_USE_18, m_bBtn[17]);
	DDX_Control(pDX, IDB_USE_19, m_bBtn[18]);
	DDX_Control(pDX, IDB_USE_20, m_bBtn[19]);
	DDX_Control(pDX, IDB_USE_21, m_bBtn[20]);
	DDX_Control(pDX, IDB_USE_22, m_bBtn[21]);
	DDX_Control(pDX, IDB_USE_23, m_bBtn[22]);
	DDX_Control(pDX, IDB_USE_24, m_bBtn[23]);
	DDX_Control(pDX, IDB_USE_25, m_bBtn[24]);
	DDX_Control(pDX, IDB_USE_26, m_bBtn[25]);
	DDX_Control(pDX, IDB_USE_27, m_bBtn[26]);
	DDX_Control(pDX, IDB_USE_28, m_bBtn[27]);
	DDX_Control(pDX, IDB_USE_29, m_bBtn[28]);
	DDX_Control(pDX, IDB_USE_30, m_bBtn[29]);
	DDX_Control(pDX, IDB_USE_31, m_bBtn[30]);
	DDX_Control(pDX, IDB_USE_32, m_bBtn[31]);
	DDX_Control(pDX, IDB_USE_33, m_bBtn[32]);
	DDX_Control(pDX, IDB_USE_34, m_bBtn[33]);
	DDX_Control(pDX, IDB_USE_35, m_bBtn[34]);
	DDX_Control(pDX, IDB_USE_36, m_bBtn[35]);
	DDX_Control(pDX, IDB_USE_37, m_bBtn[36]);
	DDX_Control(pDX, IDB_USE_38, m_bBtn[37]);
	DDX_Control(pDX, IDB_USE_39, m_bBtn[38]);
	DDX_Control(pDX, IDB_USE_40, m_bBtn[39]);
	DDX_Control(pDX, IDB_USE_41, m_bBtn[40]);
	DDX_Control(pDX, IDB_USE_42, m_bBtn[41]);
	DDX_Control(pDX, IDB_USE_43, m_bBtn[42]);
	DDX_Control(pDX, IDB_USE_44, m_bBtn[43]);
	DDX_Control(pDX, IDB_USE_45, m_bBtn[44]);
	DDX_Control(pDX, IDB_USE_46, m_bBtn[45]);
	DDX_Control(pDX, IDB_USE_47, m_bBtn[46]);
	DDX_Control(pDX, IDB_USE_48, m_bBtn[47]);
	DDX_Control(pDX, IDB_USE_49, m_bBtn[48]);
	DDX_Control(pDX, IDB_USE_50, m_bBtn[49]);
	DDX_Control(pDX, IDB_USE_51, m_bBtn[50]);
	DDX_Control(pDX, IDB_USE_52, m_bBtn[51]);
	DDX_Control(pDX, IDB_USE_53, m_bBtn[52]);
	DDX_Control(pDX, IDB_USE_54, m_bBtn[53]);
	DDX_Control(pDX, IDB_USE_55, m_bBtn[54]);
	DDX_Control(pDX, IDB_USE_56, m_bBtn[55]);
	DDX_Control(pDX, IDB_USE_57, m_bBtn[56]);
	DDX_Control(pDX, IDB_USE_58, m_bBtn[57]);
	DDX_Control(pDX, IDB_USE_59, m_bBtn[58]);
	DDX_Control(pDX, IDB_USE_60, m_bBtn[59]);
	DDX_Control(pDX, IDB_USE_61, m_bBtn[60]);
	DDX_Control(pDX, IDB_USE_62, m_bBtn[61]);
	DDX_Control(pDX, IDB_USE_63, m_bBtn[62]);
	DDX_Control(pDX, IDB_USE_64, m_bBtn[63]);
	DDX_Control(pDX, IDB_USE_65, m_bBtn[64]);
	DDX_Control(pDX, IDB_USE_66, m_bBtn[65]);
	DDX_Control(pDX, IDB_USE_67, m_bBtn[66]);
	DDX_Control(pDX, IDB_USE_68, m_bBtn[67]);
	DDX_Control(pDX, IDB_USE_69, m_bBtn[68]);
	DDX_Control(pDX, IDB_USE_70, m_bBtn[69]);
	DDX_Control(pDX, IDB_USE_71, m_bBtn[70]);
	DDX_Control(pDX, IDB_USE_72, m_bBtn[71]);
	DDX_Control(pDX, IDB_USE_73, m_bBtn[72]);
	DDX_Control(pDX, IDB_USE_74, m_bBtn[73]);
	DDX_Control(pDX, IDB_USE_75, m_bBtn[74]);
	DDX_Control(pDX, IDB_USE_76, m_bBtn[75]);
	DDX_Control(pDX, IDB_USE_77, m_bBtn[76]);
	DDX_Control(pDX, IDB_USE_78, m_bBtn[77]);
	DDX_Control(pDX, IDB_USE_79, m_bBtn[78]);
	DDX_Control(pDX, IDB_USE_80, m_bBtn[79]);
	DDX_Control(pDX, IDB_USE_81, m_bBtn[80]);
	DDX_Control(pDX, IDB_USE_82, m_bBtn[81]);
	DDX_Control(pDX, IDB_USE_83, m_bBtn[82]);
	DDX_Control(pDX, IDB_USE_84, m_bBtn[83]);
	DDX_Control(pDX, IDB_USE_85, m_bBtn[84]);
	DDX_Control(pDX, IDB_USE_86, m_bBtn[85]);
	DDX_Control(pDX, IDB_USE_87, m_bBtn[86]);
	DDX_Control(pDX, IDB_USE_88, m_bBtn[87]);
	DDX_Control(pDX, IDB_USE_89, m_bBtn[88]);
	DDX_Control(pDX, IDB_USE_90, m_bBtn[89]);
	DDX_Control(pDX, IDB_USE_91, m_bBtn[90]);
	DDX_Control(pDX, IDB_USE_92, m_bBtn[91]);
	DDX_Control(pDX, IDB_USE_93, m_bBtn[92]);
	DDX_Control(pDX, IDB_USE_94, m_bBtn[93]);
	DDX_Control(pDX, IDB_USE_95, m_bBtn[94]);
	DDX_Control(pDX, IDB_USE_96, m_bBtn[95]);
	DDX_Control(pDX, IDB_USE_97, m_bBtn[96]);
	DDX_Control(pDX, IDB_USE_98, m_bBtn[97]);
	DDX_Control(pDX, IDB_USE_99, m_bBtn[98]);
	DDX_Control(pDX, IDB_USE_100, m_bBtn[99]);
	DDX_Control(pDX, IDB_USE_101, m_bBtn[100]);
	DDX_Control(pDX, IDB_USE_102, m_bBtn[101]);
	DDX_Control(pDX, IDB_USE_103, m_bBtn[102]);
	DDX_Control(pDX, IDB_USE_104, m_bBtn[103]);
	DDX_Control(pDX, IDB_USE_105, m_bBtn[104]);
	DDX_Control(pDX, IDB_USE_106, m_bBtn[105]);
	DDX_Control(pDX, IDB_USE_107, m_bBtn[106]);
	DDX_Control(pDX, IDB_USE_108, m_bBtn[107]);

	//라벨용
	DDX_Control(pDX, IDS_USE_1, m_sData[0]);
	DDX_Control(pDX, IDS_USE_2, m_sData[1]);
	DDX_Control(pDX, IDS_USE_3, m_sData[2]);
	DDX_Control(pDX, IDS_USE_4, m_sData[3]);
	DDX_Control(pDX, IDS_USE_5, m_sData[4]);
	DDX_Control(pDX, IDS_USE_6, m_sData[5]);
	DDX_Control(pDX, IDS_USE_7, m_sData[6]);
	DDX_Control(pDX, IDS_USE_8, m_sData[7]);
	DDX_Control(pDX, IDS_USE_9, m_sData[8]);
	DDX_Control(pDX, IDS_USE_10,m_sData[9]);
	DDX_Control(pDX, IDS_USE_11, m_sData[10]);
	DDX_Control(pDX, IDS_USE_12, m_sData[11]);
	DDX_Control(pDX, IDS_USE_13, m_sData[12]);
	DDX_Control(pDX, IDS_USE_14, m_sData[13]);
	DDX_Control(pDX, IDS_USE_15, m_sData[14]);
	DDX_Control(pDX, IDS_USE_16, m_sData[15]);
	DDX_Control(pDX, IDS_USE_17, m_sData[16]);
	DDX_Control(pDX, IDS_USE_18, m_sData[17]);
	DDX_Control(pDX, IDS_USE_19, m_sData[18]);
	DDX_Control(pDX, IDS_USE_20, m_sData[19]);
	DDX_Control(pDX, IDS_USE_21, m_sData[20]);
	DDX_Control(pDX, IDS_USE_22, m_sData[21]);
	DDX_Control(pDX, IDS_USE_23, m_sData[22]);
	DDX_Control(pDX, IDS_USE_24, m_sData[23]);
	DDX_Control(pDX, IDS_USE_25, m_sData[24]);
	DDX_Control(pDX, IDS_USE_26, m_sData[25]);
	DDX_Control(pDX, IDS_USE_27, m_sData[26]);
	DDX_Control(pDX, IDS_USE_28, m_sData[27]);
	DDX_Control(pDX, IDS_USE_29, m_sData[28]);
	DDX_Control(pDX, IDS_USE_30, m_sData[29]);
	DDX_Control(pDX, IDS_USE_31, m_sData[30]);
	DDX_Control(pDX, IDS_USE_32, m_sData[31]);
	DDX_Control(pDX, IDS_USE_33, m_sData[32]);
	DDX_Control(pDX, IDS_USE_34, m_sData[33]);
	DDX_Control(pDX, IDS_USE_35, m_sData[34]);
	DDX_Control(pDX, IDS_USE_36, m_sData[35]);
	DDX_Control(pDX, IDS_USE_37, m_sData[36]);
	DDX_Control(pDX, IDS_USE_38, m_sData[37]);
	DDX_Control(pDX, IDS_USE_39, m_sData[38]);
	DDX_Control(pDX, IDS_USE_40, m_sData[39]);
	DDX_Control(pDX, IDS_USE_41, m_sData[40]);
	DDX_Control(pDX, IDS_USE_42, m_sData[41]);
	DDX_Control(pDX, IDS_USE_43, m_sData[42]);
	DDX_Control(pDX, IDS_USE_44, m_sData[43]);
	DDX_Control(pDX, IDS_USE_45, m_sData[44]);
	DDX_Control(pDX, IDS_USE_46, m_sData[45]);
	DDX_Control(pDX, IDS_USE_47, m_sData[46]);
	DDX_Control(pDX, IDS_USE_48, m_sData[47]);
	DDX_Control(pDX, IDS_USE_49, m_sData[48]);
	DDX_Control(pDX, IDS_USE_50, m_sData[49]);
	DDX_Control(pDX, IDS_USE_51, m_sData[50]);
	DDX_Control(pDX, IDS_USE_52, m_sData[51]);
	DDX_Control(pDX, IDS_USE_53, m_sData[52]);
	DDX_Control(pDX, IDS_USE_54, m_sData[53]);
	DDX_Control(pDX, IDS_USE_55, m_sData[54]);
	DDX_Control(pDX, IDS_USE_56, m_sData[55]);
	DDX_Control(pDX, IDS_USE_57, m_sData[56]);
	DDX_Control(pDX, IDS_USE_58, m_sData[57]);
	DDX_Control(pDX, IDS_USE_59, m_sData[58]);
	DDX_Control(pDX, IDS_USE_60, m_sData[59]);
	DDX_Control(pDX, IDS_USE_61, m_sData[60]);
	DDX_Control(pDX, IDS_USE_62, m_sData[61]);
	DDX_Control(pDX, IDS_USE_63, m_sData[62]);
	DDX_Control(pDX, IDS_USE_64, m_sData[63]);
	DDX_Control(pDX, IDS_USE_65, m_sData[64]);
	DDX_Control(pDX, IDS_USE_66, m_sData[65]);
	DDX_Control(pDX, IDS_USE_67, m_sData[66]);
	DDX_Control(pDX, IDS_USE_68, m_sData[67]);
	DDX_Control(pDX, IDS_USE_69, m_sData[68]);
	DDX_Control(pDX, IDS_USE_70, m_sData[69]);
	DDX_Control(pDX, IDS_USE_71, m_sData[70]);
	DDX_Control(pDX, IDS_USE_72, m_sData[71]);
	DDX_Control(pDX, IDS_USE_73, m_sData[72]);
	DDX_Control(pDX, IDS_USE_74, m_sData[73]);
	DDX_Control(pDX, IDS_USE_75, m_sData[74]);
	DDX_Control(pDX, IDS_USE_76, m_sData[75]);
	DDX_Control(pDX, IDS_USE_77, m_sData[76]);
	DDX_Control(pDX, IDS_USE_78, m_sData[77]);
	DDX_Control(pDX, IDS_USE_79, m_sData[78]);
	DDX_Control(pDX, IDS_USE_80, m_sData[79]);
	DDX_Control(pDX, IDS_USE_81, m_sData[80]);
	DDX_Control(pDX, IDS_USE_82, m_sData[81]);
	DDX_Control(pDX, IDS_USE_83, m_sData[82]);
	DDX_Control(pDX, IDS_USE_84, m_sData[83]);
	DDX_Control(pDX, IDS_USE_85, m_sData[84]);
	DDX_Control(pDX, IDS_USE_86, m_sData[85]);
	DDX_Control(pDX, IDS_USE_87, m_sData[86]);
	DDX_Control(pDX, IDS_USE_88, m_sData[87]);
	DDX_Control(pDX, IDS_USE_89, m_sData[88]);
	DDX_Control(pDX, IDS_USE_90, m_sData[89]);
	DDX_Control(pDX, IDS_USE_91, m_sData[90]);
	DDX_Control(pDX, IDS_USE_92, m_sData[91]);
	DDX_Control(pDX, IDS_USE_93, m_sData[92]);
	DDX_Control(pDX, IDS_USE_94, m_sData[93]);
	DDX_Control(pDX, IDS_USE_95, m_sData[94]);
	DDX_Control(pDX, IDS_USE_96, m_sData[95]);
	DDX_Control(pDX, IDS_USE_97, m_sData[96]);
	DDX_Control(pDX, IDS_USE_98, m_sData[97]);
	DDX_Control(pDX, IDS_USE_99, m_sData[98]);
	DDX_Control(pDX, IDS_USE_100, m_sData[99]);
	DDX_Control(pDX, IDS_USE_101, m_sData[100]);
	DDX_Control(pDX, IDS_USE_102, m_sData[101]);
	DDX_Control(pDX, IDS_USE_103, m_sData[102]);
	DDX_Control(pDX, IDS_USE_104, m_sData[103]);
	DDX_Control(pDX, IDS_USE_105, m_sData[104]);
	DDX_Control(pDX, IDS_USE_106, m_sData[105]);
	DDX_Control(pDX, IDS_USE_107, m_sData[106]);
	DDX_Control(pDX, IDS_USE_108, m_sData[107]);
}


BEGIN_MESSAGE_MAP(CDataTabICOffsetDlg, CDialog)
	//{{AFX_MSG_MAP(CDataTabICOffsetDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTabICOffsetDlg message handlers

BEGIN_EVENTSINK_MAP(CDataTabICOffsetDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataTabICOffsetDlg)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_1, -600 /* Click */, OnUse1, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_2, -600 /* Click */, OnUse2, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_3, -600 /* Click */, OnUse3, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_4, -600 /* Click */, OnUse4, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_5, -600 /* Click */, OnUse5, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_6, -600 /* Click */, OnUse6, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_7, -600 /* Click */, OnUse7, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_8, -600 /* Click */, OnUse8, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_9, -600 /* Click */, OnUse9, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_10, -600 /* Click */, OnUse10, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_11, -600 /* Click */, OnUse11, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_12, -600 /* Click */, OnUse12, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_13, -600 /* Click */, OnUse13, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_14, -600 /* Click */, OnUse14, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_15, -600 /* Click */, OnUse15, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_16, -600 /* Click */, OnUse16, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_17, -600 /* Click */, OnUse17, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_18, -600 /* Click */, OnUse18, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_19, -600 /* Click */, OnUse19, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_20, -600 /* Click */, OnUse20, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_21, -600 /* Click */, OnUse21, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_22, -600 /* Click */, OnUse22, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_23, -600 /* Click */, OnUse23, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_24, -600 /* Click */, OnUse24, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_25, -600 /* Click */, OnUse25, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_26, -600 /* Click */, OnUse26, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_27, -600 /* Click */, OnUse27, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_28, -600 /* Click */, OnUse28, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_29, -600 /* Click */, OnUse29, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_30, -600 /* Click */, OnUse30, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_31, -600 /* Click */, OnUse31, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_32, -600 /* Click */, OnUse32, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_33, -600 /* Click */, OnUse33, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_34, -600 /* Click */, OnUse34, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_35, -600 /* Click */, OnUse35, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_36, -600 /* Click */, OnUse36, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_37, -600 /* Click */, OnUse37, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_38, -600 /* Click */, OnUse38, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_39, -600 /* Click */, OnUse39, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_40, -600 /* Click */, OnUse40, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_41, -600 /* Click */, OnUse41, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_42, -600 /* Click */, OnUse42, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_43, -600 /* Click */, OnUse43, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_44, -600 /* Click */, OnUse44, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_45, -600 /* Click */, OnUse45, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_46, -600 /* Click */, OnUse46, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_47, -600 /* Click */, OnUse47, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_48, -600 /* Click */, OnUse48, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_49, -600 /* Click */, OnUse49, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_50, -600 /* Click */, OnUse50, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_51, -600 /* Click */, OnUse51, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_52, -600 /* Click */, OnUse52, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_53, -600 /* Click */, OnUse53, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_54, -600 /* Click */, OnUse54, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_55, -600 /* Click */, OnUse55, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_56, -600 /* Click */, OnUse56, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_57, -600 /* Click */, OnUse57, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_58, -600 /* Click */, OnUse58, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_59, -600 /* Click */, OnUse59, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_60, -600 /* Click */, OnUse60, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_61, -600 /* Click */, OnUse61, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_62, -600 /* Click */, OnUse62, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_63, -600 /* Click */, OnUse63, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_64, -600 /* Click */, OnUse64, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_65, -600 /* Click */, OnUse65, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_66, -600 /* Click */, OnUse66, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_67, -600 /* Click */, OnUse67, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_68, -600 /* Click */, OnUse68, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_69, -600 /* Click */, OnUse69, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_70, -600 /* Click */, OnUse70, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_71, -600 /* Click */, OnUse71, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_72, -600 /* Click */, OnUse72, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_73, -600 /* Click */, OnUse73, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_74, -600 /* Click */, OnUse74, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_75, -600 /* Click */, OnUse75, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_76, -600 /* Click */, OnUse76, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_77, -600 /* Click */, OnUse77, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_78, -600 /* Click */, OnUse78, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_79, -600 /* Click */, OnUse79, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_80, -600 /* Click */, OnUse80, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_81, -600 /* Click */, OnUse81, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_82, -600 /* Click */, OnUse82, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_83, -600 /* Click */, OnUse83, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_84, -600 /* Click */, OnUse84, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_85, -600 /* Click */, OnUse85, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_86, -600 /* Click */, OnUse86, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_87, -600 /* Click */, OnUse87, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_88, -600 /* Click */, OnUse88, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_89, -600 /* Click */, OnUse89, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_90, -600 /* Click */, OnUse90, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_91, -600 /* Click */, OnUse91, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_92, -600 /* Click */, OnUse92, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_93, -600 /* Click */, OnUse93, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_94, -600 /* Click */, OnUse94, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_95, -600 /* Click */, OnUse95, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_96, -600 /* Click */, OnUse96, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_97, -600 /* Click */, OnUse97, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_98, -600 /* Click */, OnUse98, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_99, -600 /* Click */, OnUse99, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_100, -600 /* Click */, OnUse100, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_101, -600 /* Click */, OnUse101, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_102, -600 /* Click */, OnUse102, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_103, -600 /* Click */, OnUse103, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_104, -600 /* Click */, OnUse104, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_105, -600 /* Click */, OnUse105, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_106, -600 /* Click */, OnUse106, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_107, -600 /* Click */, OnUse107, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_USE_108, -600 /* Click */, OnUse108, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_TAB_DATA_SAVE, -600 /* Click */, OnTabDataSave, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_BATCHX_INPUT, -600 /* Click */, OnBatchxInput, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_BATCHY_INPUT, -600 /* Click */, OnBatchyInput, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_TAB_PREVIOUS, -600 /* Click */, OnTabPrevious, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_TAB_NEXT, -600 /* Click */, OnTabNext, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_TAB_DATA_CANCEL, -600 /* Click */, OnTabDataCancel, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_BATCHT_INPUT, -600 /* Click */, OnClickBatchtInput, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_VIEW_OFFSET_DIR, -600 /* Click */, OnClickViewOffsetDir, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_WORK_GROUP1, -600 /* Click */, OnClickWorkGroup1, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_WORK_GROUP2, -600 /* Click */, OnClickWorkGroup2, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_TAB_OFFSET_TABLE, -600 /* Click */, OnClickTabOffsetTable, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_TAB_PREVIOUS2, -600 /* Click */, OnClickTabPrevious2, VTS_NONE)
	ON_EVENT(CDataTabICOffsetDlg, IDB_TAB_NEXT2, -600 /* Click */, OnClickTabNext2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataTabICOffsetDlg::OnTabDataCancel() 
{
	CDialog::OnCancel();
}

BOOL CDataTabICOffsetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_iTabPage = 0;
	// TODO: Add extra initialization here
	display(m_iWorkSide);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
	// Multi Language Button Title Display                                                                ┃
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
	CString szTextTemp;
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_1"), &szTextTemp);
	//	X\r\n일괄입력
	GetDlgItem(IDB_BATCHX_INPUT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_2"), &szTextTemp);
	//	Y\r\n일괄입력
	GetDlgItem(IDB_BATCHY_INPUT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_3"), &szTextTemp);
	//	이전
	GetDlgItem(IDB_TAB_PREVIOUS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_4"), &szTextTemp);
	//	다음
	GetDlgItem(IDB_TAB_NEXT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_5"), &szTextTemp);
	//	저장
	GetDlgItem(IDB_TAB_DATA_SAVE)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_6"), &szTextTemp);
	//	나가기
	GetDlgItem(IDB_TAB_DATA_CANCEL)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_7"), &szTextTemp);
	//	Y\r\n일괄입력
	GetDlgItem(IDB_BATCHT_INPUT)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_8"), &szTextTemp);
	//	이전 Tab Page
	GetDlgItem(IDB_TAB_PREVIOUS2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_DATA, _T("DataTabICOffsetDlg_9"), &szTextTemp);
	//	다음 Tab Page
	GetDlgItem(IDB_TAB_NEXT2)->SetWindowText(szTextTemp);
	

	// hongju 추가..
	changeButtonState();


	/*/@________________________________________________
#ifdef DEF_GATE_SYSTEM
	GetDlgItem(IDB_WORK_GROUP1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_WORK_GROUP2)->ShowWindow(SW_HIDE);
#else
	((CBtnEnh*)(GetDlgItem(IDB_WORK_GROUP1)))->SetValue(1);
#endif
	/*/

	GetDlgItem(IDB_WORK_GROUP1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDB_WORK_GROUP2)->ShowWindow(SW_HIDE);
	//@_________________________________________________
	
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
	{
		GetDlgItem(IDB_TAB_DATA_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDB_BATCHX_INPUT)->EnableWindow(FALSE);
		GetDlgItem(IDB_BATCHY_INPUT)->EnableWindow(FALSE);
		GetDlgItem(IDB_BATCHT_INPUT)->EnableWindow(FALSE);
	}
	//_______________


	if(m_plnkSystemData->m_bUseAutoAlignSystem == TRUE)
	{
		if(m_iTabPage == 0)
		{
			for (int i = 0; i < m_plnkTabData->m_iXuTabQty; i++)
			{
				if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
					continue;

				m_bBtn[(i*3)].EnableWindow(FALSE);
				m_sData[(i*3)].EnableWindow(FALSE);
			}
		}
		else
		{
			for (int i = 0; i < m_plnkTabData->m_iXuTabQty - DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
			{				
				m_bBtn[(i*3)].EnableWindow(FALSE);
				m_sData[(i*3)].EnableWindow(FALSE);
			}
		}
	}
	else
	{
		m_bTabOffsetTable.EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataTabICOffsetDlg::OnUse1() 
{
	ChangeData(0);
}

void CDataTabICOffsetDlg::OnUse2() 
{
	ChangeData(1);
}

void CDataTabICOffsetDlg::OnUse3() 
{
	ChangeData(2);
}

void CDataTabICOffsetDlg::OnUse4() 
{
	ChangeData(3);
}

void CDataTabICOffsetDlg::OnUse5() 
{
	ChangeData(4);
}

void CDataTabICOffsetDlg::OnUse6() 
{
	ChangeData(5);
}

void CDataTabICOffsetDlg::OnUse7() 
{
	ChangeData(6);
}

void CDataTabICOffsetDlg::OnUse8() 
{
	ChangeData(7);
}

void CDataTabICOffsetDlg::OnUse9() 
{
	ChangeData(8);
}

void CDataTabICOffsetDlg::OnUse10() 
{
	ChangeData(9);
}

void CDataTabICOffsetDlg::OnUse11() 
{
	ChangeData(10);
}

void CDataTabICOffsetDlg::OnUse12() 
{
	ChangeData(11);
}

void CDataTabICOffsetDlg::OnUse13() 
{
	ChangeData(12);
}

void CDataTabICOffsetDlg::OnUse14() 
{
	ChangeData(13);
}

void CDataTabICOffsetDlg::OnUse15() 
{
	ChangeData(14);
}

void CDataTabICOffsetDlg::OnUse16() 
{
	ChangeData(15);
}

void CDataTabICOffsetDlg::OnUse17() 
{
	ChangeData(16);
}

void CDataTabICOffsetDlg::OnUse18() 
{
	ChangeData(17);
}

void CDataTabICOffsetDlg::OnUse19() 
{
	ChangeData(18);
}

void CDataTabICOffsetDlg::OnUse20() 
{
	ChangeData(19);
}

void CDataTabICOffsetDlg::OnUse21() 
{
	ChangeData(20);
}

void CDataTabICOffsetDlg::OnUse22() 
{
	ChangeData(21);
}

void CDataTabICOffsetDlg::OnUse23() 
{
	ChangeData(22);
}

void CDataTabICOffsetDlg::OnUse24() 
{
	ChangeData(23);
}

void CDataTabICOffsetDlg::OnUse25() 
{
	ChangeData(24);
}

void CDataTabICOffsetDlg::OnUse26() 
{
	ChangeData(25);
}

void CDataTabICOffsetDlg::OnUse27() 
{
	ChangeData(26);
}

void CDataTabICOffsetDlg::OnUse28() 
{
	ChangeData(27);
}

void CDataTabICOffsetDlg::OnUse29() 
{
	ChangeData(28);
}

void CDataTabICOffsetDlg::OnUse30() 
{
	ChangeData(29);
}

void CDataTabICOffsetDlg::OnUse31() 
{
	ChangeData(30);
}

void CDataTabICOffsetDlg::OnUse32() 
{
	ChangeData(31);
}

void CDataTabICOffsetDlg::OnUse33() 
{
	ChangeData(32);
}

void CDataTabICOffsetDlg::OnUse34() 
{
	ChangeData(33);
}

void CDataTabICOffsetDlg::OnUse35() 
{
	ChangeData(34);
}

void CDataTabICOffsetDlg::OnUse36() 
{
	ChangeData(35);
}

void CDataTabICOffsetDlg::OnUse37() 
{
	ChangeData(36);
}

void CDataTabICOffsetDlg::OnUse38() 
{
	ChangeData(37);
}

void CDataTabICOffsetDlg::OnUse39() 
{
	ChangeData(38);
}

void CDataTabICOffsetDlg::OnUse40() 
{
	ChangeData(39);
}

void CDataTabICOffsetDlg::OnUse41() 
{
	// TODO: Add your control notification handler code here
	ChangeData(40);
}

void CDataTabICOffsetDlg::OnUse42() 
{
	// TODO: Add your control notification handler code here
	ChangeData(41);
}

void CDataTabICOffsetDlg::OnUse43() 
{
	// TODO: Add your control notification handler code here
	ChangeData(42);	
}

void CDataTabICOffsetDlg::OnUse44() 
{
	// TODO: Add your control notification handler code here
	ChangeData(43);
}

void CDataTabICOffsetDlg::OnUse45() 
{
	// TODO: Add your control notification handler code here
	ChangeData(44);
}

void CDataTabICOffsetDlg::OnUse46() 
{
	// TODO: Add your control notification handler code here
	ChangeData(45);
}

void CDataTabICOffsetDlg::OnUse47() 
{
	// TODO: Add your control notification handler code here
	ChangeData(46);
}

void CDataTabICOffsetDlg::OnUse48() 
{
	// TODO: Add your control notification handler code here
	ChangeData(47);
}

void CDataTabICOffsetDlg::OnUse49() 
{
	// TODO: Add your control notification handler code here
	ChangeData(48);
}

void CDataTabICOffsetDlg::OnUse50() 
{
	// TODO: Add your control notification handler code here
	ChangeData(49);
}

void CDataTabICOffsetDlg::OnUse51() 
{
	// TODO: Add your control notification handler code here
	ChangeData(50);
}

void CDataTabICOffsetDlg::OnUse52() 
{
	// TODO: Add your control notification handler code here
	ChangeData(51);
}

void CDataTabICOffsetDlg::OnUse53() 
{
	// TODO: Add your control notification handler code here
	ChangeData(52);
}

void CDataTabICOffsetDlg::OnUse54() 
{
	// TODO: Add your control notification handler code here
	ChangeData(53);
}

void CDataTabICOffsetDlg::OnUse55() 
{
	// TODO: Add your control notification handler code here
	ChangeData(54);
}

void CDataTabICOffsetDlg::OnUse56() 
{
	// TODO: Add your control notification handler code here
	ChangeData(55);
}

void CDataTabICOffsetDlg::OnUse57() 
{
	// TODO: Add your control notification handler code here
	ChangeData(56);
}

void CDataTabICOffsetDlg::OnUse58() 
{
	// TODO: Add your control notification handler code here
	ChangeData(57);
}

void CDataTabICOffsetDlg::OnUse59() 
{
	// TODO: Add your control notification handler code here
	ChangeData(58);
}

void CDataTabICOffsetDlg::OnUse60() 
{
	// TODO: Add your control notification handler code here
	ChangeData(59);
}

void CDataTabICOffsetDlg::OnUse61() 
{
	// TODO: Add your control notification handler code here
	ChangeData(60);
}

void CDataTabICOffsetDlg::OnUse62() 
{
	// TODO: Add your control notification handler code here
	ChangeData(61);
}

void CDataTabICOffsetDlg::OnUse63() 
{
	// TODO: Add your control notification handler code here
	ChangeData(62);
}

void CDataTabICOffsetDlg::OnUse64() 
{
	// TODO: Add your control notification handler code here
	ChangeData(63);
}

void CDataTabICOffsetDlg::OnUse65() 
{
	// TODO: Add your control notification handler code here
	ChangeData(64);
}

void CDataTabICOffsetDlg::OnUse66() 
{
	// TODO: Add your control notification handler code here
	ChangeData(65);
}

void CDataTabICOffsetDlg::OnUse67() 
{
	// TODO: Add your control notification handler code here
	ChangeData(66);
}

void CDataTabICOffsetDlg::OnUse68() 
{
	// TODO: Add your control notification handler code here
	ChangeData(67);
}

void CDataTabICOffsetDlg::OnUse69()
{
	// TODO: Add your control notification handler code here
	ChangeData(68);
}

void CDataTabICOffsetDlg::OnUse70() 
{
	// TODO: Add your control notification handler code here
	ChangeData(69);
}

void CDataTabICOffsetDlg::OnUse71() 
{
	// TODO: Add your control notification handler code here
	ChangeData(70);
}

void CDataTabICOffsetDlg::OnUse72() 
{
	// TODO: Add your control notification handler code here
	ChangeData(71);
}

void CDataTabICOffsetDlg::OnUse73()
{
	// TODO: Add your control notification handler code here
	ChangeData(72);
}

void CDataTabICOffsetDlg::OnUse74()
{
	// TODO: Add your control notification handler code here
	ChangeData(73);
}

void CDataTabICOffsetDlg::OnUse75()
{
	// TODO: Add your control notification handler code here
	ChangeData(74);
}

void CDataTabICOffsetDlg::OnUse76()
{
	// TODO: Add your control notification handler code here
	ChangeData(75);
}

void CDataTabICOffsetDlg::OnUse77()
{
	// TODO: Add your control notification handler code here
	ChangeData(76);
}

void CDataTabICOffsetDlg::OnUse78()
{
	// TODO: Add your control notification handler code here
	ChangeData(77);
}

void CDataTabICOffsetDlg::OnUse79()
{
	// TODO: Add your control notification handler code here
	ChangeData(78);
}

void CDataTabICOffsetDlg::OnUse80()
{
	// TODO: Add your control notification handler code here
	ChangeData(79);
}

void CDataTabICOffsetDlg::OnUse81()
{
	// TODO: Add your control notification handler code here
	ChangeData(80);
}

void CDataTabICOffsetDlg::OnUse82()
{
	// TODO: Add your control notification handler code here
	ChangeData(81);
}

void CDataTabICOffsetDlg::OnUse83()
{
	// TODO: Add your control notification handler code here
	ChangeData(82);
}

void CDataTabICOffsetDlg::OnUse84()
{
	// TODO: Add your control notification handler code here
	ChangeData(83);
}

void CDataTabICOffsetDlg::OnUse85()
{
	// TODO: Add your control notification handler code here
	ChangeData(84);
}

void CDataTabICOffsetDlg::OnUse86()
{
	// TODO: Add your control notification handler code here
	ChangeData(85);
}

void CDataTabICOffsetDlg::OnUse87()
{
	// TODO: Add your control notification handler code here
	ChangeData(86);
}

void CDataTabICOffsetDlg::OnUse88()
{
	// TODO: Add your control notification handler code here
	ChangeData(87);
}

void CDataTabICOffsetDlg::OnUse89()
{
	// TODO: Add your control notification handler code here
	ChangeData(88);
}

void CDataTabICOffsetDlg::OnUse90()
{
	// TODO: Add your control notification handler code here
	ChangeData(89);
}

void CDataTabICOffsetDlg::OnUse91()
{
	// TODO: Add your control notification handler code here
	ChangeData(90);
}

void CDataTabICOffsetDlg::OnUse92()
{
	// TODO: Add your control notification handler code here
	ChangeData(91);
}

void CDataTabICOffsetDlg::OnUse93()
{
	// TODO: Add your control notification handler code here
	ChangeData(92);
}

void CDataTabICOffsetDlg::OnUse94()
{
	// TODO: Add your control notification handler code here
	ChangeData(93);
}

void CDataTabICOffsetDlg::OnUse95()
{
	// TODO: Add your control notification handler code here
	ChangeData(94);
}

void CDataTabICOffsetDlg::OnUse96()
{
	// TODO: Add your control notification handler code here
	ChangeData(95);
}

void CDataTabICOffsetDlg::OnUse97()
{
	// TODO: Add your control notification handler code here
	ChangeData(96);
}

void CDataTabICOffsetDlg::OnUse98()
{
	// TODO: Add your control notification handler code here
	ChangeData(97);
}

void CDataTabICOffsetDlg::OnUse99()
{
	// TODO: Add your control notification handler code here
	ChangeData(98);
}

void CDataTabICOffsetDlg::OnUse100()
{
	// TODO: Add your control notification handler code here
	ChangeData(99);
}

void CDataTabICOffsetDlg::OnUse101()
{
	// TODO: Add your control notification handler code here
	ChangeData(100);
}

void CDataTabICOffsetDlg::OnUse102()
{
	// TODO: Add your control notification handler code here
	ChangeData(101);
}

void CDataTabICOffsetDlg::OnUse103()
{
	// TODO: Add your control notification handler code here
	ChangeData(102);
}

void CDataTabICOffsetDlg::OnUse104()
{
	// TODO: Add your control notification handler code here
	ChangeData(103);
}

void CDataTabICOffsetDlg::OnUse105()
{
	// TODO: Add your control notification handler code here
	ChangeData(104);
}

void CDataTabICOffsetDlg::OnUse106()
{
	// TODO: Add your control notification handler code here
	ChangeData(105);
}

void CDataTabICOffsetDlg::OnUse107()
{
	// TODO: Add your control notification handler code here
	ChangeData(106);
}

void CDataTabICOffsetDlg::OnUse108()
{
	// TODO: Add your control notification handler code here
	ChangeData(107);
}


void CDataTabICOffsetDlg::ChangeData(int sel)
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
	{
		return;
	}
	//_______________


	memset(m_cBufGet, 0, sizeof(m_cBufGet));

	int iRemain = sel % 3;
	sel = int(sel / 3);

	if(m_iTabPage == 0)
	{
		if (iRemain == 0)
			swprintf(m_cBuf,_T("%.3f"), m_xytTabOffset[sel].dX);
		else if (iRemain == 1)
			swprintf(m_cBuf,_T("%.3f"), m_xytTabOffset[sel].dY);
		else
			swprintf(m_cBuf,_T("%.3f"), m_xytTabOffset[sel].dT);
	}
	else
	{
		if (iRemain == 0)
			swprintf(m_cBuf,_T("%.3f"), m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dX);
		else if (iRemain == 1)
			swprintf(m_cBuf,_T("%.3f"), m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dY);
		else
			swprintf(m_cBuf,_T("%.3f"), m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dT);
	}

	if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
		return;	
	
	if (iRemain == 0)
	{
		if (fabs(m_xytTabOffset[sel].dX - wcstod(m_cBufGet,NULL)) > 0.01)
		{
//			Offset 값은 10㎛ 보다 작아야 합니다.
			MyMessageBox(MY_MESSAGE_BOX, 41000, _T("Error"), M_ICONERROR);
			return;
		}

		if(m_iTabPage == 0)
		{			
			m_xytTabOffset[sel].dX = wcstod(m_cBufGet,NULL);
			swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[sel].dX);
		}
		else
		{
			m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dX = wcstod(m_cBufGet,NULL);
			swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dX);
		}
	}
	else if (iRemain == 1)
	{
		/*/@______________________________________
		//120208.kms________
#ifdef DEF_SOURCE_SYSTEM
		if (wcstod(m_cBufGet,NULL) > 1.0
			|| wcstod(m_cBufGet,NULL) < -1.0 )
		{
//			Offset 값은 1mm 보다 작아야 합니다.
			MyMessageBox(MY_MESSAGE_BOX, 41003, _T("Error"), M_ICONERROR);
			return;
		}
#else
		if (wcstod(m_cBufGet,NULL) > 0.2
			|| wcstod(m_cBufGet,NULL) < -0.2)
		{
//			Offset 값은 0.2mm 보다 작아야 합니다.
			MyMessageBox(MY_MESSAGE_BOX, 41005, _T("Error"), M_ICONERROR);
			return;
		}
#endif
		//______________
		/*/
		if (wcstod(m_cBufGet,NULL) > 0.5
			|| wcstod(m_cBufGet,NULL) < -0.5)
		{
//			Offset 값은 0.5mm 보다 작아야 합니다.
			MyMessageBox(MY_MESSAGE_BOX, 41005, _T("Error"), M_ICONERROR);
			return;
		}
		//@________________________________________

		if(m_iTabPage == 0)
		{			
			m_xytTabOffset[sel].dY = wcstod(m_cBufGet,NULL);
			swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[sel].dY);
		}
		else
		{			
			m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dY = wcstod(m_cBufGet,NULL);
			swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dY);
		}
	}
	else
	{
		if (wcstod(m_cBufGet,NULL) > 0.5
			|| wcstod(m_cBufGet,NULL) < -0.5)
		{
//			Offset 값은 0.5(degree) 보다 작아야 합니다.
			MyMessageBox(MY_MESSAGE_BOX, 41004, _T("Error"), M_ICONERROR);
			return;
		}
		if(m_iTabPage == 0)
		{			
			m_xytTabOffset[sel].dT = wcstod(m_cBufGet,NULL);
			swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[sel].dT);
		}
		else
		{			
			m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dT = wcstod(m_cBufGet,NULL);
			swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[sel + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dT);
		}
	}

	m_sData[sel*3+iRemain].SetCaption(m_cBuf);
}

void CDataTabICOffsetDlg::changeWorkSide()
{
	m_iTabPage = 0;
	switch (m_iWorkSide)
	{
	case WORKING_SIDE_X_UP:	
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xd)
			return;
		
		m_iWorkSide = WORKING_SIDE_X_DOWN;
		break;
		
	case WORKING_SIDE_X_DOWN:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Xu)
			return;
		
		m_iWorkSide = WORKING_SIDE_X_UP;
		break;

	case WORKING_SIDE_Y_LEFT:	
		if (FALSE == m_plnkPanelData->m_bUseFlag_Yr)
			return;
		
		m_iWorkSide = WORKING_SIDE_Y_RIGHT;
		break;
		
	case WORKING_SIDE_Y_RIGHT:
		if (FALSE == m_plnkPanelData->m_bUseFlag_Yl)
			return;
		
		m_iWorkSide = WORKING_SIDE_Y_LEFT;
		break;
	}

	display(m_iWorkSide);
}

void CDataTabICOffsetDlg::OnTabNext() 
{
	changeWorkSide();
}

void CDataTabICOffsetDlg::OnTabPrevious() 
{
	changeWorkSide();
}

void CDataTabICOffsetDlg::OnTabDataSave()
{
//	변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,41001,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	// Data 배열값을 Tab Data에 할당
	switch (m_iWorkSide) 
	{
	case WORKING_SIDE_X_UP:
		for (int i = 0; i < m_plnkTabData->m_iXuTabQty; i++)
		{
//@			/*
			//120116.kms______
			// m_xytTabOffset X 는 Tab 기준 X => TabMounter 축과 일치
			m_plnkTabOffset->m_rgdXuTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
			m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
//@			*/

			/*/
			// m_xytTabOffset Y 는 Tab 기준 Y => TabAligner 축과 일치
			if (m_iWorkGroup == DEF_FRONT_GROUP)
			{
				//120116.kms_____
//@				m_plnkTabOffset->m_rgdXuTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
				//_______________
				m_plnkTabOffset->m_rgdXuTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
				m_plnkTabOffset->m_rgdXuTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
			}
			else
			{
				//120116.kms_____
//@				m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
				//_______________
				m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
				m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
			}
			/*/
			m_plnkTabOffset->m_rgdXuTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
			m_plnkTabOffset->m_rgdXuTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
		}
		break;

	case WORKING_SIDE_X_DOWN:
		for (int i = 0; i < m_plnkTabData->m_iXdTabQty; i++)
		{
			m_plnkTabOffset->m_rgdXdTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
			m_plnkTabOffset_Rear->m_rgdXdTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;

			/*/
			if (m_iWorkGroup == DEF_FRONT_GROUP)
			{
				m_plnkTabOffset->m_rgdXdTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
				m_plnkTabOffset->m_rgdXdTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
			}
			else
			{
				m_plnkTabOffset_Rear->m_rgdXdTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
				m_plnkTabOffset_Rear->m_rgdXdTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
			}
			/*/
			m_plnkTabOffset->m_rgdXdTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
			m_plnkTabOffset->m_rgdXdTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
		}
		break;

	case WORKING_SIDE_Y_LEFT:
		for (int i = 0; i < m_plnkTabData->m_iYlTabQty; i++)
		{
			m_plnkTabOffset->m_rgdYlTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
			m_plnkTabOffset_Rear->m_rgdYlTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
			/*/
			if (m_iWorkGroup == DEF_FRONT_GROUP)
			{
				m_plnkTabOffset->m_rgdYlTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
				m_plnkTabOffset->m_rgdYlTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
			}
			else
			{
				m_plnkTabOffset_Rear->m_rgdYlTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
				m_plnkTabOffset_Rear->m_rgdYlTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
			}
			/*/
			m_plnkTabOffset->m_rgdYlTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
			m_plnkTabOffset->m_rgdYlTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
		}
		break;

	case WORKING_SIDE_Y_RIGHT:
		for (int i = 0; i < m_plnkTabData->m_iYrTabQty; i++)
		{
			m_plnkTabOffset->m_rgdYrTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
			m_plnkTabOffset_Rear->m_rgdYrTabOffsetTabMounter[i] = m_xytTabOffset[i].dX;
			/*/
			if (m_iWorkGroup == DEF_FRONT_GROUP)
			{
				m_plnkTabOffset->m_rgdYrTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
				m_plnkTabOffset->m_rgdYrTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
			}
			else
			{
				m_plnkTabOffset_Rear->m_rgdYrTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
				m_plnkTabOffset_Rear->m_rgdYrTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
			}
			/*/
			m_plnkTabOffset->m_rgdYrTabOffsetTabAligner[i] = m_xytTabOffset[i].dY;
			m_plnkTabOffset->m_rgdYrTabOffsetTabAlignerT[i] = m_xytTabOffset[i].dT;
		}
		break;
	}

	// Tab Data를 File에 저장
	m_plnkTabOffset->WriteData();
	m_plnkTabOffset_Rear->WriteData();

	//SJ_YYK 110421 Add.. Ellas
	// 2011.11.05 Park, YK - //ELLAS_111103
	//MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eAlignCheck);
	MOlbSystemPre.GetLCNetComponent()->EqSpecialStateReport(eBondingCheck);
}

void CDataTabICOffsetDlg::display(int iWorkSideType)
{
	// TODO: Add extra initialization here
	int iTabQty;
	m_iWorkSide = iWorkSideType;
	CString strWorkSideTitle;

	switch (m_iWorkSide) 
	{
	case WORKING_SIDE_X_UP:
		strWorkSideTitle = "Xu";
		iTabQty = m_plnkTabData->m_iXuTabQty;
		pitchDisplay(iTabQty);

		for (int i = 0; i < m_plnkTabData->m_iXuTabQty; i++)
		{
			//120116.kms_________
//			m_xytTabOffset[i].dX = m_plnkTabOffset->m_rgdXuTabOffsetTabMounter[i];
			//___________________
			m_xytTabOffset[i].dX = m_plnkTabOffset->m_rgdXuTabOffsetTabMounter[i];	//@원복
			/*/
			if (m_iWorkGroup == DEF_FRONT_GROUP)
			{
				//120116.kms________
//@				m_xytTabOffset[i].dX = m_plnkTabOffset->m_rgdXuTabOffsetTabMounter[i];
				//__________________
				m_xytTabOffset[i].dY = m_plnkTabOffset->m_rgdXuTabOffsetTabAligner[i];
				m_xytTabOffset[i].dT = m_plnkTabOffset->m_rgdXuTabOffsetTabAlignerT[i];
			}
			else
			{
				//120116.kms________
//@				m_xytTabOffset[i].dX = m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabMounter[i];
				//__________________
				m_xytTabOffset[i].dY = m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabAligner[i];
				m_xytTabOffset[i].dT = m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabAlignerT[i];
			}
			/*/
			m_xytTabOffset[i].dY = m_plnkTabOffset->m_rgdXuTabOffsetTabAligner[i];
			m_xytTabOffset[i].dT = m_plnkTabOffset->m_rgdXuTabOffsetTabAlignerT[i];
		}
		break;

	case WORKING_SIDE_X_DOWN:
		strWorkSideTitle = "Xd";
		iTabQty = m_plnkTabData->m_iXdTabQty;
		pitchDisplay(iTabQty);

		for (int i = 0; i < m_plnkTabData->m_iXdTabQty; i++)
		{
			m_xytTabOffset[i].dX = m_plnkTabOffset->m_rgdXdTabOffsetTabMounter[i];
			/*/
			if (m_iWorkGroup == DEF_FRONT_GROUP)
			{
				m_xytTabOffset[i].dY = m_plnkTabOffset->m_rgdXuTabOffsetTabAligner[i];
				m_xytTabOffset[i].dT = m_plnkTabOffset->m_rgdXuTabOffsetTabAlignerT[i];
			}
			else
			{
				m_xytTabOffset[i].dY = m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabAligner[i];
				m_xytTabOffset[i].dT = m_plnkTabOffset_Rear->m_rgdXuTabOffsetTabAlignerT[i];
			}
			/*/
			m_xytTabOffset[i].dY = m_plnkTabOffset->m_rgdXdTabOffsetTabAligner[i];
			m_xytTabOffset[i].dT = m_plnkTabOffset->m_rgdXdTabOffsetTabAlignerT[i];
		}
		break;

	case WORKING_SIDE_Y_LEFT:
		strWorkSideTitle = "Yl";
		iTabQty = m_plnkTabData->m_iYlTabQty;
		pitchDisplay(iTabQty);
		
		for (int i = 0; i < m_plnkTabData->m_iYlTabQty; i++)
		{
			m_xytTabOffset[i].dX = m_plnkTabOffset->m_rgdYlTabOffsetTabMounter[i];
			/*/
			if (m_iWorkGroup == DEF_FRONT_GROUP)
			{
				m_xytTabOffset[i].dY = m_plnkTabOffset->m_rgdYlTabOffsetTabAligner[i];
				m_xytTabOffset[i].dT = m_plnkTabOffset->m_rgdYlTabOffsetTabAlignerT[i];
			}
			else
			{
				m_xytTabOffset[i].dY = m_plnkTabOffset_Rear->m_rgdYlTabOffsetTabAligner[i];
				m_xytTabOffset[i].dT = m_plnkTabOffset_Rear->m_rgdYlTabOffsetTabAlignerT[i];
			}
			/*/
			m_xytTabOffset[i].dY = m_plnkTabOffset->m_rgdYlTabOffsetTabAligner[i];
			m_xytTabOffset[i].dT = m_plnkTabOffset->m_rgdYlTabOffsetTabAlignerT[i];
		}
		break;

	case WORKING_SIDE_Y_RIGHT:
		strWorkSideTitle = "Yr";
		iTabQty = m_plnkTabData->m_iYrTabQty;
		pitchDisplay(iTabQty);
		
		for (int i = 0; i < m_plnkTabData->m_iYrTabQty; i++)
		{
			m_xytTabOffset[i].dX = m_plnkTabOffset->m_rgdYrTabOffsetTabMounter[i];
			/*/
			if (m_iWorkGroup == DEF_FRONT_GROUP)
			{
				m_xytTabOffset[i].dY = m_plnkTabOffset->m_rgdYrTabOffsetTabAligner[i];
				m_xytTabOffset[i].dT = m_plnkTabOffset->m_rgdYrTabOffsetTabAlignerT[i];
			}
			else
			{
				m_xytTabOffset[i].dY = m_plnkTabOffset_Rear->m_rgdYrTabOffsetTabAligner[i];
				m_xytTabOffset[i].dT = m_plnkTabOffset_Rear->m_rgdYrTabOffsetTabAlignerT[i];
			}
			/*/
			m_xytTabOffset[i].dY = m_plnkTabOffset->m_rgdYrTabOffsetTabAligner[i];
			m_xytTabOffset[i].dT = m_plnkTabOffset->m_rgdYrTabOffsetTabAlignerT[i];
		}
		break;
	}
	GetDlgItem(IDC_TAB_DATA_WORKSIDE_TITLE)->SetWindowText(strWorkSideTitle + " Offset Data");

	if(m_iTabPage == 0)
	{
		for (int i = 0; i < iTabQty; i++)
		{
			if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
				continue;

			swprintf(m_cBuf,_T("%.3f"),m_xytTabOffset[i].dX);
			m_sData[i*3+0].SetCaption(m_cBuf);
			swprintf(m_cBuf,_T("%.3f"),m_xytTabOffset[i].dY);
			m_sData[i*3+1].SetCaption(m_cBuf);
			swprintf(m_cBuf,_T("%.3f"),m_xytTabOffset[i].dT);
			m_sData[i*3+2].SetCaption(m_cBuf);
		}
	}
	else
	{
		for (int i = 0; i < iTabQty - DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
		{
			swprintf(m_cBuf,_T("%.3f"),m_xytTabOffset[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dX);
			m_sData[i*3+0].SetCaption(m_cBuf);
			swprintf(m_cBuf,_T("%.3f"),m_xytTabOffset[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dY);
			m_sData[i*3+1].SetCaption(m_cBuf);
			swprintf(m_cBuf,_T("%.3f"),m_xytTabOffset[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM].dT);
			m_sData[i*3+2].SetCaption(m_cBuf);
		}
	}

	// hongju 추가..
	changeButtonState();
}

void CDataTabICOffsetDlg::OnBatchxInput() 
{
	// TODO: Add your control notification handler code here
	memset(m_cBufGet, 0, sizeof(m_cBufGet));

	int iTabQty = 0;
	if (m_iWorkSide == WORKING_SIDE_X_UP)
		iTabQty = m_plnkTabData->m_iXuTabQty;
	if (m_iWorkSide == WORKING_SIDE_X_DOWN)
		iTabQty = m_plnkTabData->m_iXdTabQty;
	if (m_iWorkSide == WORKING_SIDE_Y_LEFT)
		iTabQty = m_plnkTabData->m_iYlTabQty;
	if (m_iWorkSide == WORKING_SIDE_Y_RIGHT)
		iTabQty = m_plnkTabData->m_iYrTabQty;

	swprintf(m_cBuf,_T("%.3f"),0.0);
	if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
		return;	

	if (wcstod(m_cBufGet,NULL) > 0.05)
	{
//		Offset 값은 50㎛ 보다 작아야 합니다.
		MyMessageBox(MY_MESSAGE_BOX,41002,_T("Error"), M_ICONERROR);
		return;
	}

	for (int i = 0; i < iTabQty; i++) 
	{
		m_xytTabOffset[i].dX = wcstod(m_cBufGet,NULL);
		if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			continue;

		swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[i].dX);	
		m_sData[i*3+0].SetCaption(m_cBuf);
	}
	return;	
}

void CDataTabICOffsetDlg::OnBatchyInput() 
{
	// TODO: Add your control notification handler code here
	memset(m_cBufGet, 0, sizeof(m_cBufGet));

	int iTabQty = 0;
	if (m_iWorkSide == WORKING_SIDE_X_UP)
		iTabQty = m_plnkTabData->m_iXuTabQty;
	if (m_iWorkSide == WORKING_SIDE_X_DOWN)
		iTabQty = m_plnkTabData->m_iXdTabQty;
	if (m_iWorkSide == WORKING_SIDE_Y_LEFT)
		iTabQty = m_plnkTabData->m_iYlTabQty;
	if (m_iWorkSide == WORKING_SIDE_Y_RIGHT)
		iTabQty = m_plnkTabData->m_iYrTabQty;

	swprintf(m_cBuf,_T("%.3f"),0.0);
	if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
		return;	

	for (int i = 0; i < iTabQty; i++) 
	{
		m_xytTabOffset[i].dY = wcstod(m_cBufGet,NULL);
		if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			continue;

		swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[i].dY);
		m_sData[i*3+1].SetCaption(m_cBuf);
	}
	return;
}

void CDataTabICOffsetDlg::OnClickBatchtInput() 
{
	// TODO: Add your control notification handler code here
	memset(m_cBufGet, 0, sizeof(m_cBufGet));

	int iTabQty = 0;
	if (m_iWorkSide == WORKING_SIDE_X_UP)
		iTabQty = m_plnkTabData->m_iXuTabQty;
	if (m_iWorkSide == WORKING_SIDE_X_DOWN)
		iTabQty = m_plnkTabData->m_iXdTabQty;
	if (m_iWorkSide == WORKING_SIDE_Y_LEFT)
		iTabQty = m_plnkTabData->m_iYlTabQty;
	if (m_iWorkSide == WORKING_SIDE_Y_RIGHT)
		iTabQty = m_plnkTabData->m_iYrTabQty;

	swprintf(m_cBuf,_T("%.3f"),0.0);
	if (!GetNumberBox(m_cBufGet, 10, m_cBuf))	
		return;	

	for (int i = 0; i < iTabQty; i++)
	{
		m_xytTabOffset[i].dT = wcstod(m_cBufGet,NULL);
		if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			continue;

		swprintf(m_cBuf, _T("%.3f"), m_xytTabOffset[i].dT);
		m_sData[i*3+2].SetCaption(m_cBuf);
	}
	return;	
}

void CDataTabICOffsetDlg::pitchDisplay(int value)
{
	int idb_value[DEF_TABIC_OFFSET_DLG_MAX_ITEM * 3] =
	{	
		IDB_USE_1,	IDB_USE_4,	IDB_USE_7,	IDB_USE_10,	IDB_USE_13,
		IDB_USE_16, IDB_USE_19, IDB_USE_22, IDB_USE_25, IDB_USE_28,
		IDB_USE_31, IDB_USE_34, IDB_USE_37, IDB_USE_40, IDB_USE_43,
		IDB_USE_46, IDB_USE_49, IDB_USE_52, IDB_USE_55, IDB_USE_58,
		IDB_USE_61, IDB_USE_64, IDB_USE_67, IDB_USE_70,	IDB_USE_73,
		IDB_USE_76, IDB_USE_79, IDB_USE_82, IDB_USE_85,	IDB_USE_88,
		IDB_USE_91, IDB_USE_94,	IDB_USE_97,	IDB_USE_100, IDB_USE_103,
		IDB_USE_106,

		IDB_USE_2,	IDB_USE_5,	IDB_USE_8,	IDB_USE_11,	IDB_USE_14,
		IDB_USE_17, IDB_USE_20, IDB_USE_23, IDB_USE_26, IDB_USE_29,
		IDB_USE_32, IDB_USE_35, IDB_USE_38, IDB_USE_41, IDB_USE_44,
		IDB_USE_47, IDB_USE_50, IDB_USE_53, IDB_USE_56, IDB_USE_59,
		IDB_USE_62, IDB_USE_65, IDB_USE_68, IDB_USE_71, IDB_USE_74,
		IDB_USE_77, IDB_USE_80, IDB_USE_83, IDB_USE_86, IDB_USE_89,
		IDB_USE_92, IDB_USE_95,	IDB_USE_98,	IDB_USE_101, IDB_USE_104,
		IDB_USE_107,

		IDB_USE_3,	IDB_USE_6,	IDB_USE_9,	IDB_USE_12,	IDB_USE_15,
		IDB_USE_18, IDB_USE_21, IDB_USE_24, IDB_USE_27, IDB_USE_30,
		IDB_USE_33, IDB_USE_36, IDB_USE_39, IDB_USE_42, IDB_USE_45,
		IDB_USE_48, IDB_USE_51, IDB_USE_54, IDB_USE_57, IDB_USE_60,
		IDB_USE_63, IDB_USE_66, IDB_USE_69, IDB_USE_72, IDB_USE_75,
		IDB_USE_78, IDB_USE_81, IDB_USE_84, IDB_USE_87, IDB_USE_90,
		IDB_USE_93, IDB_USE_96,	IDB_USE_99,	IDB_USE_102, IDB_USE_105,
		IDB_USE_108,
	};

	int ids_value[DEF_TABIC_OFFSET_DLG_MAX_ITEM * 3] =
	{	
		IDS_USE_1,	IDS_USE_4,	IDS_USE_7,	IDS_USE_10,	IDS_USE_13,
		IDS_USE_16, IDS_USE_19, IDS_USE_22, IDS_USE_25, IDS_USE_28,
		IDS_USE_31, IDS_USE_34, IDS_USE_37, IDS_USE_40, IDS_USE_43,
		IDS_USE_46, IDS_USE_49, IDS_USE_52, IDS_USE_55, IDS_USE_58,
		IDS_USE_61, IDS_USE_64, IDS_USE_67, IDS_USE_70, IDS_USE_73,
		IDS_USE_76, IDS_USE_79, IDS_USE_82, IDS_USE_85, IDS_USE_88,
		IDS_USE_91, IDS_USE_94,	IDS_USE_97,	IDS_USE_100, IDS_USE_103,
		IDS_USE_106,

		IDS_USE_2,	IDS_USE_5,	IDS_USE_8,	IDS_USE_11,	IDS_USE_14,
		IDS_USE_17, IDS_USE_20, IDS_USE_23, IDS_USE_26, IDS_USE_29,
		IDS_USE_32, IDS_USE_35, IDS_USE_38, IDS_USE_41, IDS_USE_44,
		IDS_USE_47, IDS_USE_50, IDS_USE_53, IDS_USE_56, IDS_USE_59,
		IDS_USE_62, IDS_USE_65, IDS_USE_68, IDS_USE_71, IDS_USE_74,
		IDS_USE_77, IDS_USE_80, IDS_USE_83, IDS_USE_86, IDS_USE_89,
		IDS_USE_92, IDS_USE_95,	IDS_USE_98,	IDS_USE_101, IDS_USE_104,
		IDS_USE_107, 

		IDS_USE_3,	IDS_USE_6,	IDS_USE_9,	IDS_USE_12,	IDS_USE_15,
		IDS_USE_18, IDS_USE_21, IDS_USE_24, IDS_USE_27, IDS_USE_30,
		IDS_USE_33, IDS_USE_36, IDS_USE_39, IDS_USE_42, IDS_USE_45,
		IDS_USE_48, IDS_USE_51, IDS_USE_54, IDS_USE_57, IDS_USE_60,
		IDS_USE_63, IDS_USE_66, IDS_USE_69, IDS_USE_72, IDS_USE_75,
		IDS_USE_78, IDS_USE_81, IDS_USE_84, IDS_USE_87, IDS_USE_90,
		IDS_USE_93, IDS_USE_96,	IDS_USE_99,	IDS_USE_102, IDS_USE_105,
		IDS_USE_108,
	};

	int value_copy = value;

	switch (m_iWorkSide)
	{
	case WORKING_SIDE_X_UP: 
		if (!m_plnkPanelData->m_bUseFlag_Xu) value_copy = 0;
		break;
	case WORKING_SIDE_X_DOWN: 
		if (!m_plnkPanelData->m_bUseFlag_Xd) value_copy = 0;
		break;
	case WORKING_SIDE_Y_LEFT: 
		if (!m_plnkPanelData->m_bUseFlag_Yl) value_copy = 0;
		break;
	case WORKING_SIDE_Y_RIGHT: 
		if (!m_plnkPanelData->m_bUseFlag_Yr) value_copy = 0;
		break;
	}

	if(m_iTabPage == 0)
	{
		for (int i = 0; i < value_copy; i++)
		{
			if(i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
				continue;
			GetDlgItem(idb_value[i])->ShowWindow(SW_SHOW);
			GetDlgItem(ids_value[i])->ShowWindow(SW_SHOW);
			GetDlgItem(idb_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*1])->ShowWindow(SW_SHOW);
			GetDlgItem(ids_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*1])->ShowWindow(SW_SHOW);
			GetDlgItem(idb_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*2])->ShowWindow(SW_SHOW);
			GetDlgItem(ids_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*2])->ShowWindow(SW_SHOW);
		}
		
		for (int i = value_copy; i < DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
		{
			GetDlgItem(idb_value[i])->ShowWindow(SW_HIDE);
			GetDlgItem(ids_value[i])->ShowWindow(SW_HIDE);
			GetDlgItem(idb_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*1])->ShowWindow(SW_HIDE);
			GetDlgItem(ids_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*1])->ShowWindow(SW_HIDE);
			GetDlgItem(idb_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*2])->ShowWindow(SW_HIDE);
			GetDlgItem(ids_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*2])->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		for (int i = 0; i < value_copy - DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
		{
			GetDlgItem(idb_value[i])->ShowWindow(SW_SHOW);
			GetDlgItem(ids_value[i])->ShowWindow(SW_SHOW);
			GetDlgItem(idb_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*1])->ShowWindow(SW_SHOW);
			GetDlgItem(ids_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*1])->ShowWindow(SW_SHOW);
			GetDlgItem(idb_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*2])->ShowWindow(SW_SHOW);
			GetDlgItem(ids_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*2])->ShowWindow(SW_SHOW);
		}
		
		for (int i = value_copy - DEF_TABIC_OFFSET_DLG_MAX_ITEM; i < DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
		{
			GetDlgItem(idb_value[i])->ShowWindow(SW_HIDE);
			GetDlgItem(ids_value[i])->ShowWindow(SW_HIDE);
			GetDlgItem(idb_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*1])->ShowWindow(SW_HIDE);
			GetDlgItem(ids_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*1])->ShowWindow(SW_HIDE);
			GetDlgItem(idb_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*2])->ShowWindow(SW_HIDE);
			GetDlgItem(ids_value[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM*2])->ShowWindow(SW_HIDE);
		}
	}

}

void CDataTabICOffsetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDataTabICOffsetDlg::changeButtonState()
{
	
	// hongju 추가...기본적으로 TRUE로 하고 지운다..
	//for (int i = 0; i < (DEF_MAX_TAB * 3) ; i++)
	for (int i = 0; i < (DEF_TABIC_OFFSET_DLG_MAX_ITEM * 3) ; i++)
	{
		m_bBtn[i].EnableWindow(TRUE);
		m_sData[i].EnableWindow(TRUE);
	}

	switch (m_iWorkSide) 
	{
	case WORKING_SIDE_X_UP:
		if(m_iTabPage == 0)
		{
			for (int i = 0; i < m_plnkTabData->m_iXuTabQty; i++)
			{				
				if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
					continue;
				if (FALSE == m_plnkTabData->m_bUseXuTab[i])
				{					
					m_bBtn[(i*3)].EnableWindow(FALSE);
					m_bBtn[(i*3)+1].EnableWindow(FALSE);
					m_bBtn[(i*3)+2].EnableWindow(FALSE);
					m_sData[(i*3)].EnableWindow(FALSE);
					m_sData[(i*3)+1].EnableWindow(FALSE);
					m_sData[(i*3)+2].EnableWindow(FALSE);
				}
			}			
		}
		else
		{
			for (int i = 0; i < m_plnkTabData->m_iXuTabQty - DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
			{				
				if (FALSE == m_plnkTabData->m_bUseXuTab[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM])
				{
					m_bBtn[(i*3)].EnableWindow(FALSE);
					m_bBtn[(i*3)+1].EnableWindow(FALSE);
					m_bBtn[(i*3)+2].EnableWindow(FALSE);
					m_sData[(i*3)].EnableWindow(FALSE);
					m_sData[(i*3)+1].EnableWindow(FALSE);
					m_sData[(i*3)+2].EnableWindow(FALSE);
				}
			}
		}
		break;

	case WORKING_SIDE_X_DOWN:
		if(m_iTabPage == 0)
		{
			for (int i = 0; i < m_plnkTabData->m_iXdTabQty; i++)
			{
				if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
					continue;
				if (FALSE == m_plnkTabData->m_bUseXdTab[i])
				{
					m_bBtn[(i*3)].EnableWindow(FALSE);
					m_bBtn[(i*3)+1].EnableWindow(FALSE);
					m_bBtn[(i*3)+2].EnableWindow(FALSE);
					m_sData[(i*3)].EnableWindow(FALSE);
					m_sData[(i*3)+1].EnableWindow(FALSE);
					m_sData[(i*3)+2].EnableWindow(FALSE);
				}
			}
		}
		else
		{
			for (int i = 0; i < m_plnkTabData->m_iXdTabQty - DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
			{
				if (FALSE == m_plnkTabData->m_bUseXdTab[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM])
				{
					m_bBtn[(i*3)].EnableWindow(FALSE);
					m_bBtn[(i*3)+1].EnableWindow(FALSE);
					m_bBtn[(i*3)+2].EnableWindow(FALSE);
					m_sData[(i*3)].EnableWindow(FALSE);
					m_sData[(i*3)+1].EnableWindow(FALSE);
					m_sData[(i*3)+2].EnableWindow(FALSE);
				}
			}
		}
		break;

	case WORKING_SIDE_Y_LEFT:
		if(m_iTabPage == 0)
		{
			for (int i = 0; i < m_plnkTabData->m_iYlTabQty; i++)
			{
				if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
					continue;
				if (FALSE == m_plnkTabData->m_bUseYlTab[i])
				{
					m_bBtn[(i*3)].EnableWindow(FALSE);
					m_bBtn[(i*3)+1].EnableWindow(FALSE);
					m_bBtn[(i*3)+2].EnableWindow(FALSE);
					m_sData[(i*3)].EnableWindow(FALSE);
					m_sData[(i*3)+1].EnableWindow(FALSE);
					m_sData[(i*3)+2].EnableWindow(FALSE);
				}
			}
		}
		else
		{
			for (int i = 0; i < m_plnkTabData->m_iYlTabQty - DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
			{
				if (FALSE == m_plnkTabData->m_bUseYlTab[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM])
				{
					m_bBtn[(i*3)].EnableWindow(FALSE);
					m_bBtn[(i*3)+1].EnableWindow(FALSE);
					m_bBtn[(i*3)+2].EnableWindow(FALSE);
					m_sData[(i*3)].EnableWindow(FALSE);
					m_sData[(i*3)+1].EnableWindow(FALSE);
					m_sData[(i*3)+2].EnableWindow(FALSE);
				}
			}
		}
		break;

	case WORKING_SIDE_Y_RIGHT:
		if(m_iTabPage == 0)
		{
			for (int i = 0; i < m_plnkTabData->m_iYrTabQty; i++)
			{
				if( i >= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
					continue;
				if (FALSE == m_plnkTabData->m_bUseYrTab[i])
				{
					m_bBtn[(i*3)].EnableWindow(FALSE);
					m_bBtn[(i*3)+1].EnableWindow(FALSE);
					m_bBtn[(i*3)+2].EnableWindow(FALSE);
					m_sData[(i*3)].EnableWindow(FALSE);
					m_sData[(i*3)+1].EnableWindow(FALSE);
					m_sData[(i*3)+2].EnableWindow(FALSE);
				}
			}
		}
		else
		{
			for (int i = 0; i < m_plnkTabData->m_iYrTabQty - DEF_TABIC_OFFSET_DLG_MAX_ITEM; i++)
			{
				if (FALSE == m_plnkTabData->m_bUseYrTab[i + DEF_TABIC_OFFSET_DLG_MAX_ITEM])
				{
					m_bBtn[(i*3)].EnableWindow(FALSE);
					m_bBtn[(i*3)+1].EnableWindow(FALSE);
					m_bBtn[(i*3)+2].EnableWindow(FALSE);
					m_sData[(i*3)].EnableWindow(FALSE);
					m_sData[(i*3)+1].EnableWindow(FALSE);
					m_sData[(i*3)+2].EnableWindow(FALSE);
				}
			}
		}
		break;
	}

	//20131024 SJ_HJG

	int iRemain;
	WCHAR cBuf[60];
	
	for (int i = 0; i < DEF_TABIC_OFFSET_DLG_MAX_ITEM * 3; i++)
	{
		iRemain = i % 3;

		if(m_iTabPage == 0)
		{
			if (iRemain == 0)
				swprintf(cBuf,_T("X%d"), (i / 3) + 1);
			else if (iRemain == 1)
				swprintf(cBuf,_T("Y%d"), (i / 3) + 1);
			else
				swprintf(cBuf,_T("T%d"), (i / 3) + 1);
		}
		else
		{
			if (iRemain == 0)
				swprintf(cBuf,_T("X%d"), (i / 3) + 1 + DEF_TABIC_OFFSET_DLG_MAX_ITEM);
			else if (iRemain == 1)
				swprintf(cBuf,_T("Y%d"), (i / 3) + 1 + DEF_TABIC_OFFSET_DLG_MAX_ITEM);
			else
				swprintf(cBuf,_T("T%d"), (i / 3) + 1 + DEF_TABIC_OFFSET_DLG_MAX_ITEM);
		}

		m_bBtn[i].SetCaption(cBuf);
	}				
	
	
}

void CDataTabICOffsetDlg::OnClickViewOffsetDir() 
{
	// TODO: Add your control notification handler code here
	CDataTabOffsetImageDlg dlg;
	dlg.DoModal();
}

void CDataTabICOffsetDlg::OnClickWorkGroup1() 
{
	// TODO: Add your control notification handler code here
	m_iWorkGroup = DEF_FRONT_GROUP;
	display(m_iWorkSide);
}

void CDataTabICOffsetDlg::OnClickWorkGroup2() 
{
	// TODO: Add your control notification handler code here
	m_iWorkGroup = DEF_REAR_GROUP;
	display(m_iWorkSide);
}

void CDataTabICOffsetDlg::OnClickTabOffsetTable() 
{
	// TODO: Add your control notification handler code here
	CDlgTabOffsetTable dlg;
	dlg.DoModal();
}

void CDataTabICOffsetDlg::OnClickTabPrevious2() 
{
	// TODO: Add your control notification handler code here
	if(m_iTabPage == 0	)
		return;

	switch (m_iWorkSide)
	{
	case WORKING_SIDE_X_UP:	
		if(m_plnkTabData->m_iXuTabQty <= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			return;

		break;
		
	case WORKING_SIDE_X_DOWN:
		if(m_plnkTabData->m_iXdTabQty <= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			return;
		break;

	case WORKING_SIDE_Y_LEFT:	
		if(m_plnkTabData->m_iYlTabQty <= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			return;
		break;
		
	case WORKING_SIDE_Y_RIGHT:
		if(m_plnkTabData->m_iYrTabQty <= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			return;
		
		break;
	}

	m_iTabPage--;
	display(m_iWorkSide);
}

void CDataTabICOffsetDlg::OnClickTabNext2() 
{
	// TODO: Add your control notification handler code here
	if(m_iTabPage == 1	)
		return;
	
	switch (m_iWorkSide)
	{
	case WORKING_SIDE_X_UP:	
		if(m_plnkTabData->m_iXuTabQty <= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			return;

		break;
		
	case WORKING_SIDE_X_DOWN:
		if(m_plnkTabData->m_iXdTabQty <= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			return;
		break;

	case WORKING_SIDE_Y_LEFT:	
		if(m_plnkTabData->m_iYlTabQty <= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			return;
		break;
		
	case WORKING_SIDE_Y_RIGHT:
		if(m_plnkTabData->m_iYrTabQty <= DEF_TABIC_OFFSET_DLG_MAX_ITEM)
			return;
		
		break;
	}


	m_iTabPage++;
	display(m_iWorkSide);
	
}
