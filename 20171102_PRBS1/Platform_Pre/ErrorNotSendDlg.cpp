// ErrorNotSendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "ErrorNotSendDlg.h"

#include "MPlatformOlbSystem.h"
#include "MSystemData.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CErrorNotSendDlg dialog



CErrorNotSendDlg::CErrorNotSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorNotSendDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CErrorNotSendDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
//	memset(m_dData, 0, sizeof(m_dData));

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CErrorNotSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorNotSendDlg)
	DDX_Control(pDX, IDB_EXIT, m_bExit);
	DDX_Control(pDX, IDB_SAVE, m_bSave);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDB_ERROR_NO0, m_btnErrorNo[0]);
	DDX_Control(pDX, IDB_ERROR_NO1, m_btnErrorNo[1]);
	DDX_Control(pDX, IDB_ERROR_NO2, m_btnErrorNo[2]);
	DDX_Control(pDX, IDB_ERROR_NO3, m_btnErrorNo[3]);
	DDX_Control(pDX, IDB_ERROR_NO4, m_btnErrorNo[4]);
	DDX_Control(pDX, IDB_ERROR_NO5, m_btnErrorNo[5]);
	DDX_Control(pDX, IDB_ERROR_NO6, m_btnErrorNo[6]);
	DDX_Control(pDX, IDB_ERROR_NO7, m_btnErrorNo[7]);
	DDX_Control(pDX, IDB_ERROR_NO8, m_btnErrorNo[8]);
	DDX_Control(pDX, IDB_ERROR_NO9, m_btnErrorNo[9]);
	DDX_Control(pDX, IDB_ERROR_NO10, m_btnErrorNo[10]);
	DDX_Control(pDX, IDB_ERROR_NO11, m_btnErrorNo[11]);
	DDX_Control(pDX, IDB_ERROR_NO12, m_btnErrorNo[12]);
	DDX_Control(pDX, IDB_ERROR_NO13, m_btnErrorNo[13]);
	DDX_Control(pDX, IDB_ERROR_NO14, m_btnErrorNo[14]);
	DDX_Control(pDX, IDB_ERROR_NO15, m_btnErrorNo[15]);
	DDX_Control(pDX, IDB_ERROR_NO16, m_btnErrorNo[16]);
	DDX_Control(pDX, IDB_ERROR_NO17, m_btnErrorNo[17]);
	DDX_Control(pDX, IDB_ERROR_NO18, m_btnErrorNo[18]);
	DDX_Control(pDX, IDB_ERROR_NO19, m_btnErrorNo[19]);
	DDX_Control(pDX, IDB_ERROR_NO20, m_btnErrorNo[20]);
	DDX_Control(pDX, IDB_ERROR_NO21, m_btnErrorNo[21]);
	DDX_Control(pDX, IDB_ERROR_NO22, m_btnErrorNo[22]);
	DDX_Control(pDX, IDB_ERROR_NO23, m_btnErrorNo[23]);
	DDX_Control(pDX, IDB_ERROR_NO24, m_btnErrorNo[24]);
	DDX_Control(pDX, IDB_ERROR_NO25, m_btnErrorNo[25]);
	DDX_Control(pDX, IDB_ERROR_NO26, m_btnErrorNo[26]);
	DDX_Control(pDX, IDB_ERROR_NO27, m_btnErrorNo[27]);
	DDX_Control(pDX, IDB_ERROR_NO28, m_btnErrorNo[28]);
	DDX_Control(pDX, IDB_ERROR_NO29, m_btnErrorNo[29]);

	DDX_Control(pDX, IDS_ERROR_NO0, m_lblErrorNo[0]);
	DDX_Control(pDX, IDS_ERROR_NO1, m_lblErrorNo[1]);
	DDX_Control(pDX, IDS_ERROR_NO2, m_lblErrorNo[2]);
	DDX_Control(pDX, IDS_ERROR_NO3, m_lblErrorNo[3]);
	DDX_Control(pDX, IDS_ERROR_NO4, m_lblErrorNo[4]);
	DDX_Control(pDX, IDS_ERROR_NO5, m_lblErrorNo[5]);
	DDX_Control(pDX, IDS_ERROR_NO6, m_lblErrorNo[6]);
	DDX_Control(pDX, IDS_ERROR_NO7, m_lblErrorNo[7]);
	DDX_Control(pDX, IDS_ERROR_NO8, m_lblErrorNo[8]);
	DDX_Control(pDX, IDS_ERROR_NO9, m_lblErrorNo[9]);
	DDX_Control(pDX, IDS_ERROR_NO10, m_lblErrorNo[10]);
	DDX_Control(pDX, IDS_ERROR_NO11, m_lblErrorNo[11]);
	DDX_Control(pDX, IDS_ERROR_NO12, m_lblErrorNo[12]);
	DDX_Control(pDX, IDS_ERROR_NO13, m_lblErrorNo[13]);
	DDX_Control(pDX, IDS_ERROR_NO14, m_lblErrorNo[14]);
	DDX_Control(pDX, IDS_ERROR_NO15, m_lblErrorNo[15]);
	DDX_Control(pDX, IDS_ERROR_NO16, m_lblErrorNo[16]);
	DDX_Control(pDX, IDS_ERROR_NO17, m_lblErrorNo[17]);
	DDX_Control(pDX, IDS_ERROR_NO18, m_lblErrorNo[18]);
	DDX_Control(pDX, IDS_ERROR_NO19, m_lblErrorNo[19]);
	DDX_Control(pDX, IDS_ERROR_NO20, m_lblErrorNo[20]);
	DDX_Control(pDX, IDS_ERROR_NO21, m_lblErrorNo[21]);
	DDX_Control(pDX, IDS_ERROR_NO22, m_lblErrorNo[22]);
	DDX_Control(pDX, IDS_ERROR_NO23, m_lblErrorNo[23]);
	DDX_Control(pDX, IDS_ERROR_NO24, m_lblErrorNo[24]);
	DDX_Control(pDX, IDS_ERROR_NO25, m_lblErrorNo[25]);
	DDX_Control(pDX, IDS_ERROR_NO26, m_lblErrorNo[26]);
	DDX_Control(pDX, IDS_ERROR_NO27, m_lblErrorNo[27]);
	DDX_Control(pDX, IDS_ERROR_NO28, m_lblErrorNo[28]);
	DDX_Control(pDX, IDS_ERROR_NO29, m_lblErrorNo[29]);

}


BEGIN_MESSAGE_MAP(CErrorNotSendDlg, CDialog)
	//{{AFX_MSG_MAP(CErrorNotSendDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorNotSendDlg message handlers

BOOL CErrorNotSendDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString szTextTemp;

	// TODO: Add extra initialization here
	displayItemLabel(TRUE);

	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		GetDlgItem(IDB_SAVE)->EnableWindow(FALSE);
	
	//저장
	ReadDisplayMessage(MAIN_DATA, _T("ErrorNotSendDlg_1"), &szTextTemp);
	GetDlgItem(IDB_SAVE)->SetWindowText(szTextTemp);
	//나가기
	ReadDisplayMessage(MAIN_DATA, _T("ErrorNotSendDlg_2"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);
	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CErrorNotSendDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CErrorNotSendDlg)
	ON_EVENT(CErrorNotSendDlg, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO0, -600 /* Click */, OnClickErrorNo0, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO1, -600 /* Click */, OnClickErrorNo1, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO2, -600 /* Click */, OnClickErrorNo2, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO3, -600 /* Click */, OnClickErrorNo3, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO4, -600 /* Click */, OnClickErrorNo4, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO5, -600 /* Click */, OnClickErrorNo5, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO6, -600 /* Click */, OnClickErrorNo6, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO7, -600 /* Click */, OnClickErrorNo7, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO8, -600 /* Click */, OnClickErrorNo8, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO9, -600 /* Click */, OnClickErrorNo9, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO10, -600 /* Click */, OnClickErrorNo10, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO11, -600 /* Click */, OnClickErrorNo11, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO12, -600 /* Click */, OnClickErrorNo12, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO13, -600 /* Click */, OnClickErrorNo13, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO14, -600 /* Click */, OnClickErrorNo14, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO15, -600 /* Click */, OnClickErrorNo15, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO16, -600 /* Click */, OnClickErrorNo16, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO17, -600 /* Click */, OnClickErrorNo17, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO18, -600 /* Click */, OnClickErrorNo18, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO19, -600 /* Click */, OnClickErrorNo19, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO20, -600 /* Click */, OnClickErrorNo20, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO21, -600 /* Click */, OnClickErrorNo21, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO22, -600 /* Click */, OnClickErrorNo22, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO23, -600 /* Click */, OnClickErrorNo23, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO24, -600 /* Click */, OnClickErrorNo24, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO25, -600 /* Click */, OnClickErrorNo25, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO26, -600 /* Click */, OnClickErrorNo26, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO27, -600 /* Click */, OnClickErrorNo27, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO28, -600 /* Click */, OnClickErrorNo28, VTS_NONE)
	ON_EVENT(CErrorNotSendDlg, IDB_ERROR_NO29, -600 /* Click */, OnClickErrorNo29, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CErrorNotSendDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CErrorNotSendDlg::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	//	변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX, 40101, _T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;
	for(int i=0; i<DEF_DATA_ERROR_DLG_ALL_ITEM; i++)
		m_plnkSystemData->m_strErrorNo[i]	= m_sData[i];

	m_plnkSystemData->WriteData();
}

void CErrorNotSendDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}

void CErrorNotSendDlg::displayItemLabel(BOOL bUpdate)
{
	CString label_buf[DEF_DATA_ERROR_DLG_ALL_ITEM];
	int i;

	if (bUpdate)
	{
		for(i=0; i<DEF_DATA_ERROR_DLG_ALL_ITEM; i++)
		{
			m_strErrorNo[i] = m_plnkSystemData->m_strErrorNo[i];
			m_sData[i] = m_plnkSystemData->m_strErrorNo[i];
		}
	}

	for (i = 0; i < DEF_DATA_ERROR_DLG_ALL_ITEM; i++)
	{
		label_buf[i] = m_strErrorNo[i];
		m_lblErrorNo[i].SetCaption(label_buf[i]);
	}
}

void CErrorNotSendDlg::getUserInput(int iItemNo)
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return;
	//_______________

	ASSERT(iItemNo >= 0 && iItemNo < DEF_DATA_ERROR_DLG_ALL_ITEM);

	int iResult = SUCCESS;
	memset(m_cBufGet, 0, sizeof(m_cBufGet));
	CString strMsg;

	if (!GetValueBox(m_cBufGet, 20, _T(""), _T("Error No"), FALSE, FALSE))
			return;

	m_strErrorNo[iItemNo].Format(_T("%s"), m_cBufGet);
	m_sData[iItemNo].Format(_T("%s"), m_cBufGet);

	displayItemLabel();
}

void CErrorNotSendDlg::OnClickErrorNo0() 
{
	// TODO: Add your control notification handler code here
	getUserInput(0);
}

void CErrorNotSendDlg::OnClickErrorNo1() 
{
	// TODO: Add your control notification handler code here
	getUserInput(1);
}

void CErrorNotSendDlg::OnClickErrorNo2() 
{
	// TODO: Add your control notification handler code here
	getUserInput(2);
}

void CErrorNotSendDlg::OnClickErrorNo3() 
{
	// TODO: Add your control notification handler code here
	getUserInput(3);
}

void CErrorNotSendDlg::OnClickErrorNo4() 
{
	// TODO: Add your control notification handler code here
	getUserInput(4);
}

void CErrorNotSendDlg::OnClickErrorNo5() 
{
	// TODO: Add your control notification handler code here
	getUserInput(5);
}

void CErrorNotSendDlg::OnClickErrorNo6() 
{
	// TODO: Add your control notification handler code here
	getUserInput(6);
}

void CErrorNotSendDlg::OnClickErrorNo7() 
{
	// TODO: Add your control notification handler code here
	getUserInput(7);
}

void CErrorNotSendDlg::OnClickErrorNo8() 
{
	// TODO: Add your control notification handler code here
	getUserInput(8);
}

void CErrorNotSendDlg::OnClickErrorNo9() 
{
	// TODO: Add your control notification handler code here
	getUserInput(9);
}

void CErrorNotSendDlg::OnClickErrorNo10() 
{
	// TODO: Add your control notification handler code here
	getUserInput(10);
}

void CErrorNotSendDlg::OnClickErrorNo11() 
{
	// TODO: Add your control notification handler code here
	getUserInput(11);
}

void CErrorNotSendDlg::OnClickErrorNo12() 
{
	// TODO: Add your control notification handler code here
	getUserInput(12);
}

void CErrorNotSendDlg::OnClickErrorNo13() 
{
	// TODO: Add your control notification handler code here
	getUserInput(13);
}

void CErrorNotSendDlg::OnClickErrorNo14() 
{
	// TODO: Add your control notification handler code here
	getUserInput(14);
}

void CErrorNotSendDlg::OnClickErrorNo15() 
{
	// TODO: Add your control notification handler code here
	getUserInput(15);
}

void CErrorNotSendDlg::OnClickErrorNo16() 
{
	// TODO: Add your control notification handler code here
	getUserInput(16);
}

void CErrorNotSendDlg::OnClickErrorNo17() 
{
	// TODO: Add your control notification handler code here
	getUserInput(17);
}

void CErrorNotSendDlg::OnClickErrorNo18() 
{
	// TODO: Add your control notification handler code here
	getUserInput(18);
}

void CErrorNotSendDlg::OnClickErrorNo19() 
{
	// TODO: Add your control notification handler code here
	getUserInput(19);	
}

void CErrorNotSendDlg::OnClickErrorNo20() 
{
	// TODO: Add your control notification handler code here
	getUserInput(20);
}

void CErrorNotSendDlg::OnClickErrorNo21() 
{
	// TODO: Add your control notification handler code here
	getUserInput(21);
}

void CErrorNotSendDlg::OnClickErrorNo22() 
{
	// TODO: Add your control notification handler code here
	getUserInput(22);
}

void CErrorNotSendDlg::OnClickErrorNo23() 
{
	// TODO: Add your control notification handler code here
	getUserInput(23);
}

void CErrorNotSendDlg::OnClickErrorNo24() 
{
	// TODO: Add your control notification handler code here
	getUserInput(24);
}

void CErrorNotSendDlg::OnClickErrorNo25() 
{
	// TODO: Add your control notification handler code here
	getUserInput(25);
}

void CErrorNotSendDlg::OnClickErrorNo26() 
{
	// TODO: Add your control notification handler code here
	getUserInput(26);
}

void CErrorNotSendDlg::OnClickErrorNo27() 
{
	// TODO: Add your control notification handler code here
	getUserInput(27);
}

void CErrorNotSendDlg::OnClickErrorNo28() 
{
	// TODO: Add your control notification handler code here
	getUserInput(28);
}

void CErrorNotSendDlg::OnClickErrorNo29() 
{
	// TODO: Add your control notification handler code here
	getUserInput(29);
}
