// SelectMarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisionSelectMarkDlg.h"
#include "IVision.h"
// jdy2
#include "DefSystem.h"
// ML_hongju
#include "atlconv.h"//꼭 해줘야 함

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char* g_rgstrPMMark[] = {
#undef VISION_MARK
#define VISION_MARK(no, pm_mark, blob_mark) #pm_mark,
#include "info\VisionMark.inf"
#undef VISION_MARK
};


char* g_rgstrBlobMark[] = {
#undef VISION_MARK
#define VISION_MARK(no, pm_mark, blob_mark) #blob_mark,
#include "info\VisionMark.inf"
#undef VISION_MARK
};

/////////////////////////////////////////////////////////////////////////////
// CVisionSelectMarkDlg dialog

CVisionSelectMarkDlg::CVisionSelectMarkDlg(IVision* plnkVision, int iOpType, SMarkInfo* pMarkInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CVisionSelectMarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisionSelectMarkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);

	m_iOpType = iOpType;
	m_psMarkInfo = pMarkInfo;
	m_plnkVision = plnkVision;

	if (m_psMarkInfo->m_eType == DEF_PM_MARK)
		m_pMarkName = g_rgstrPMMark;
	else if (m_psMarkInfo->m_eType == DEF_BLOB_MARK)
		m_pMarkName = g_rgstrBlobMark;
	else if (m_psMarkInfo->m_eType == DEF_CALIPER_MARK)
		m_pMarkName = NULL;
}

void CVisionSelectMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisionSelectMarkDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_SELECT_0, m_rgCtrlBtn[0]);
	DDX_Control(pDX, IDB_SELECT_1, m_rgCtrlBtn[1]);
	DDX_Control(pDX, IDB_SELECT_2, m_rgCtrlBtn[2]);
	DDX_Control(pDX, IDB_SELECT_3, m_rgCtrlBtn[3]);
	DDX_Control(pDX, IDB_SELECT_4, m_rgCtrlBtn[4]);
	DDX_Control(pDX, IDB_SELECT_5, m_rgCtrlBtn[5]);
	DDX_Control(pDX, IDB_SELECT_6, m_rgCtrlBtn[6]);
	DDX_Control(pDX, IDB_SELECT_7, m_rgCtrlBtn[7]);
	DDX_Control(pDX, IDB_SELECT_8, m_rgCtrlBtn[8]);
	DDX_Control(pDX, IDB_SELECT_9, m_rgCtrlBtn[9]);
	DDX_Control(pDX, IDB_SELECT_10, m_rgCtrlBtn[10]);
	DDX_Control(pDX, IDB_SELECT_11, m_rgCtrlBtn[11]);
	DDX_Control(pDX, IDB_SELECT_12, m_rgCtrlBtn[12]);
	DDX_Control(pDX, IDB_SELECT_13, m_rgCtrlBtn[13]);
	DDX_Control(pDX, IDB_SELECT_14, m_rgCtrlBtn[14]);
	DDX_Control(pDX, IDB_SELECT_15, m_rgCtrlBtn[15]);
	DDX_Control(pDX, IDB_SELECT_16, m_rgCtrlBtn[16]);
	DDX_Control(pDX, IDB_SELECT_17, m_rgCtrlBtn[17]);
	DDX_Control(pDX, IDB_SELECT_18, m_rgCtrlBtn[18]);
	DDX_Control(pDX, IDB_SELECT_19, m_rgCtrlBtn[19]);
	DDX_Control(pDX, IDB_SELECT_20, m_rgCtrlBtn[20]);
	DDX_Control(pDX, IDB_SELECT_21, m_rgCtrlBtn[21]);
	DDX_Control(pDX, IDB_SELECT_22, m_rgCtrlBtn[22]);
	DDX_Control(pDX, IDB_SELECT_23, m_rgCtrlBtn[23]);
	DDX_Control(pDX, IDB_SELECT_24, m_rgCtrlBtn[24]);
	DDX_Control(pDX, IDB_SELECT_25, m_rgCtrlBtn[25]);
	DDX_Control(pDX, IDB_SELECT_26, m_rgCtrlBtn[26]);
	DDX_Control(pDX, IDB_SELECT_27, m_rgCtrlBtn[27]);
}


BEGIN_MESSAGE_MAP(CVisionSelectMarkDlg, CDialog)
	//{{AFX_MSG_MAP(CVisionSelectMarkDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisionSelectMarkDlg message handlers

void CVisionSelectMarkDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CVisionSelectMarkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	CRect mainrect;
	CWnd* pParent = GetParent();
	pParent->GetWindowRect(&mainrect);
    GetWindowRect(&rect);
	MoveWindow(mainrect.right-rect.Width(), mainrect.top, rect.Width(), rect.Height());

	BOOL bBtnStateReverse = (m_iOpType == DEF_MAKE_MARK) ? TRUE : FALSE;

	CString strName;
	for (int i = 0 ; i < DEF_DLG_SELECT_MARK_MAX_BUTTON ; i++)
	{
		if (m_psMarkInfo->m_eType == DEF_PM_MARK)
			strName = g_rgstrPMMark[i];
		else if (m_psMarkInfo->m_eType == DEF_BLOB_MARK)
			strName = g_rgstrBlobMark[i];
		else if (m_psMarkInfo->m_eType == DEF_CALIPER_MARK)
		{
		}

		// Mark Name String 가공
		strName.Replace(_T("DEF_"), _T(""));
		strName.Replace(_T("CALIBRATION"), _T("Calibration"));
		strName.Replace(_T("FIDU"), _T("Fiducial"));
		strName.Replace(_T("SUB"), _T("Sub"));
		strName.Replace(_T("PANEL"), _T("Panel"));
		strName.Replace(_T("TAB"), _T("Tab"));
		strName.Replace(_T("LEAD"), _T("Lead"));
		strName.Replace(_T("BLOB"), _T("Blob"));
		strName.Replace(_T("INSPECTION"), _T("Inspection"));
		strName.Replace(_T("EDGE"), _T("Edge"));
		strName.Replace(_T("AFTER"), _T("after"));
		strName.Replace(_T("MOUNT"), _T("Mount"));
		strName.Replace(_T("_MARK"), _T("\nMark"));
		strName.Replace(_T("XU"), _T("Xu"));
		strName.Replace(_T("XD"), _T("Xd"));
		strName.Replace(_T("YL"), _T("Yl"));
		strName.Replace(_T("YR"), _T("Yr"));
		strName.Replace(_T("RESERVED"), _T("Reserved"));
		strName.Replace(_T("_"), _T(" "));

		m_rgCtrlBtn[i].SetCaption(strName.GetBuffer(strName.GetLength()));
		m_rgCtrlBtn[i].EnableWindow(bBtnStateReverse);

		if (m_plnkVision->CheckModel(m_psMarkInfo->m_iCamNo, m_psMarkInfo->m_eType, i) == 0)
			m_rgCtrlBtn[i].EnableWindow(!bBtnStateReverse);
		else if (TRUE == bBtnStateReverse)
		{
			if (FALSE == isUsedMark(m_psMarkInfo->m_iCamNo, m_psMarkInfo->m_eType, i))
				m_rgCtrlBtn[i].SetForeColor(LTGRAY);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVisionSelectMarkDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisionSelectMarkDlg)
	ON_EVENT(CVisionSelectMarkDlg, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_0, -600 /* Click */, OnSelect0, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_1, -600 /* Click */, OnSelect1, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_2, -600 /* Click */, OnSelect2, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_3, -600 /* Click */, OnSelect3, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_4, -600 /* Click */, OnSelect4, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_5, -600 /* Click */, OnSelect5, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_10, -600 /* Click */, OnClickSelect10, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_6, -600 /* Click */, OnClickSelect6, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_7, -600 /* Click */, OnClickSelect7, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_8, -600 /* Click */, OnClickSelect8, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_9, -600 /* Click */, OnClickSelect9, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_11, -600 /* Click */, OnClickSelect11, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_12, -600 /* Click */, OnClickSelect12, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_13, -600 /* Click */, OnClickSelect13, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_14, -600 /* Click */, OnClickSelect14, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_15, -600 /* Click */, OnClickSelect15, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_16, -600 /* Click */, OnClickSelect16, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_17, -600 /* Click */, OnClickSelect17, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_18, -600 /* Click */, OnClickSelect18, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_19, -600 /* Click */, OnClickSelect19, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_20, -600 /* Click */, OnClickSelect20, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_21, -600 /* Click */, OnClickSelect21, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_22, -600 /* Click */, OnClickSelect22, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_23, -600 /* Click */, OnClickSelect23, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_24, -600 /* Click */, OnClickSelect24, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_25, -600 /* Click */, OnClickSelect25, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_26, -600 /* Click */, OnClickSelect26, VTS_NONE)
	ON_EVENT(CVisionSelectMarkDlg, IDB_SELECT_27, -600 /* Click */, OnClickSelect27, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVisionSelectMarkDlg::OnExit() 
{
	CDialog::OnCancel();
}

void CVisionSelectMarkDlg::OnSelect0() 
{
	*(m_psMarkInfo->m_piNo) = 0;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();	
}

void CVisionSelectMarkDlg::OnSelect1() 
{
	*(m_psMarkInfo->m_piNo) = 1;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnSelect2() 
{
	*(m_psMarkInfo->m_piNo) = 2;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();	
}

void CVisionSelectMarkDlg::OnSelect3() 
{
	*(m_psMarkInfo->m_piNo) = 3;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();	
}

void CVisionSelectMarkDlg::OnSelect4() 
{
	*(m_psMarkInfo->m_piNo) = 4;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();	
}

void CVisionSelectMarkDlg::OnSelect5() 
{
	*(m_psMarkInfo->m_piNo) = 5;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect6() 
{
	*(m_psMarkInfo->m_piNo) = 6;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect7() 
{
	*(m_psMarkInfo->m_piNo) = 7;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect8() 
{
	*(m_psMarkInfo->m_piNo) = 8;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect9() 
{
	*(m_psMarkInfo->m_piNo) = 9;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect10() 
{
	*(m_psMarkInfo->m_piNo) = 10;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect11() 
{
	*(m_psMarkInfo->m_piNo) = 11;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect12() 
{
	*(m_psMarkInfo->m_piNo) = 12;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect13() 
{
	*(m_psMarkInfo->m_piNo) = 13;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect14() 
{
	*(m_psMarkInfo->m_piNo) = 14;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect15() 
{
	*(m_psMarkInfo->m_piNo) = 15;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect16() 
{
	*(m_psMarkInfo->m_piNo) = 16;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect17() 
{
	*(m_psMarkInfo->m_piNo) = 17;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect18() 
{
	*(m_psMarkInfo->m_piNo) = 18;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect19() 
{
	*(m_psMarkInfo->m_piNo) = 19;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect20() 
{
	*(m_psMarkInfo->m_piNo) = 20;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect21() 
{
	*(m_psMarkInfo->m_piNo) = 21;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect22() 
{
	*(m_psMarkInfo->m_piNo) = 22;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect23() 
{
	*(m_psMarkInfo->m_piNo) = 23;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect24() 
{
	*(m_psMarkInfo->m_piNo) = 24;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();
}

void CVisionSelectMarkDlg::OnClickSelect25() 
{
	*(m_psMarkInfo->m_piNo) = 25;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();	
}

void CVisionSelectMarkDlg::OnClickSelect26() 
{
	*(m_psMarkInfo->m_piNo) = 26;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();	
}

void CVisionSelectMarkDlg::OnClickSelect27() 
{
	*(m_psMarkInfo->m_piNo) = 27;
	memset(m_psMarkInfo->m_szName, 0, sizeof(m_psMarkInfo->m_szName));
	strcpy(m_psMarkInfo->m_szName, m_pMarkName[*(m_psMarkInfo->m_piNo)]);
	CDialog::OnOK();	
}

// jdy2
BOOL CVisionSelectMarkDlg::isUsedMark(int iCamNo, int iType, int iMarkNo)
{
	if (m_psMarkInfo->m_eType == DEF_PM_MARK)
	{
		switch (iCamNo)
		{
		case DEF_F_TABALIGNER_CAMERA_1:
			switch (iMarkNo)
			{
			case DEF_TAB_LEAD_XU_MARK1:
			case DEF_CALIBRATION_MARK1:
				return TRUE;
			}
			break;
		case DEF_F_TABALIGNER_CAMERA_2:
			switch (iMarkNo)
			{
			case DEF_TAB_LEAD_XU_MARK2:
			case DEF_CALIBRATION_MARK1:
				return TRUE;
			}
			break;
		case DEF_R_TABALIGNER_CAMERA_1:
			switch (iMarkNo)
			{
			case DEF_TAB_LEAD_XU_MARK1:
			case DEF_CALIBRATION_MARK1:
				return TRUE;
			}
			break;
		case DEF_R_TABALIGNER_CAMERA_2:
			switch (iMarkNo)
			{
			case DEF_TAB_LEAD_XU_MARK2:
			case DEF_CALIBRATION_MARK1:
				return TRUE;
			}
			break;
		case DEF_INSPECTION_CAMERA_1:
		case DEF_INSPECTION_CAMERA_3:
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CAMERA_5:
		case DEF_INSPECTION_CAMERA_7:
		case DEF_R_INSPECTION_CAMERA_1:
#endif
			switch (iMarkNo)
			{
			case DEF_FIDU_MARK:
			case DEF_FIDU_SUB_MARK:
			case DEF_PANEL_LEAD_XU_MARK1:
			case DEF_TAB_LEAD_XU_MARK1:
			case DEF_TAB_LEAD_XU_AFTER_MOUNT_MARK1:
			case DEF_CALIBRATION_MARK1:
				return TRUE;
			}
			break;
		case DEF_INSPECTION_CAMERA_2:
		case DEF_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CAMERA_6:
		case DEF_INSPECTION_CAMERA_8:
		case DEF_R_INSPECTION_CAMERA_2:
#endif
			switch (iMarkNo)
			{
			case DEF_FIDU_MARK:
			case DEF_FIDU_SUB_MARK:
			case DEF_PANEL_LEAD_XU_MARK2:
			case DEF_TAB_LEAD_XU_MARK2:
			case DEF_TAB_LEAD_XU_AFTER_MOUNT_MARK2:
			case DEF_CALIBRATION_MARK1:
				return TRUE;
			}
			break;
		}
	}
	//170616 JSH.s
	else if (m_psMarkInfo->m_eType == DEF_BLOB_MARK)
	{
		switch (iCamNo)
		{
		case DEF_INSPECTION_CAMERA_1:
		case DEF_INSPECTION_CAMERA_3:
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CAMERA_5:
		case DEF_INSPECTION_CAMERA_7:
#endif
		case DEF_R_INSPECTION_CAMERA_1:
		case DEF_R_INSPECTION_CAMERA_3:
			switch (iMarkNo)
			{
			case DEF_BLOB_TAB_ATTACHED1:
				return TRUE;
			}
			break;
		case DEF_INSPECTION_CAMERA_2:
		case DEF_INSPECTION_CAMERA_4:
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CAMERA_6:
		case DEF_INSPECTION_CAMERA_8:
#endif
		case DEF_R_INSPECTION_CAMERA_2:
		case DEF_R_INSPECTION_CAMERA_4:
			switch (iMarkNo)
			{
			case DEF_BLOB_TAB_ATTACHED2:
				return TRUE;
			}
			break;
	
		default:
			return TRUE;
			break;
		}
	}
	//170616 JSH.e
	else if (m_psMarkInfo->m_eType == DEF_CALIPER_MARK)
	{
		return TRUE;
	}

	return FALSE;
}
