// DlgTabOffsetTable.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "DlgTabOffsetTable.h"
#include "MPlatformOlbSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/////////////////////////////////////////////////////////////////////////////
// CDlgTabOffsetTable dialog


CDlgTabOffsetTable::CDlgTabOffsetTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTabOffsetTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTabOffsetTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_plnkSystemData = MOlbSystemPre.GetSystemData();
}


void CDlgTabOffsetTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTabOffsetTable)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_DATA_1, m_lblRowData[0][0]);
	DDX_Control(pDX, IDC_DATA_2, m_lblRowData[0][1]);
}


BEGIN_MESSAGE_MAP(CDlgTabOffsetTable, CDialog)
	//{{AFX_MSG_MAP(CDlgTabOffsetTable)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTabOffsetTable message handlers

BEGIN_EVENTSINK_MAP(CDlgTabOffsetTable, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgTabOffsetTable)
	ON_EVENT(CDlgTabOffsetTable, IDB_BUTTON1, -600 /* Click */, OnClickButton1, VTS_NONE)
	ON_EVENT(CDlgTabOffsetTable, IDB_BUTTON2, -600 /* Click */, OnClickButton2, VTS_NONE)
	ON_EVENT(CDlgTabOffsetTable, IDB_TAB_DATA_SAVE, -600 /* Click */, OnClickTabDataSave, VTS_NONE)
	ON_EVENT(CDlgTabOffsetTable, IDB_TAB_DATA_CANCEL, -600 /* Click */, OnClickTabDataCancel, VTS_NONE)
	ON_EVENT(CDlgTabOffsetTable, IDB_USE_SIDE, -600 /* Click */, OnClickUseSide, VTS_NONE)
	ON_EVENT(CDlgTabOffsetTable, IDB_BUTTON3, -600 /* Click */, OnClickButton3, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CDlgTabOffsetTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_iSelectedRow = 0;

	m_rgdTabOffsetData[0][0] = m_plnkSystemData->m_dTabOffsetdata;
	m_rgdTabOffsetData[0][1] = m_plnkSystemData->m_dTabOffsetAverage;

	for(int i=0; i<DEF_MAX_ROW; i++)
	{
		for(int j=0; j<DEF_MAX_COLUMN;j++)
		{
			swprintf(m_szNewValue, _T("%.3f"), m_rgdTabOffsetData[i][j]);
			m_lblRowData[i][j].SetCaption(m_szNewValue);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTabOffsetTable::OnClickButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateDisplay(0);
}

void CDlgTabOffsetTable::OnClickButton2() 
{
	// TODO: Add your control notification handler code here
	UpdateDisplay(1);
}

void CDlgTabOffsetTable::UpdateDisplay(int iColNo)
{
	m_lblRowData[m_iSelectedRow][iColNo].GetWindowText(m_szOldValue, 30);
	
	if (!GetNumberBox(m_szNewValue, 10, m_szOldValue))
		return;
	
	m_rgdTabOffsetData[m_iSelectedRow][iColNo] = wcstod(m_szNewValue, NULL);
	swprintf(m_szNewValue, _T("%.3f"), m_rgdTabOffsetData[m_iSelectedRow][iColNo]);
	m_rgdTabOffsetData[m_iSelectedRow][iColNo] = wcstod(m_szNewValue, NULL);
	
	m_lblRowData[m_iSelectedRow][iColNo].SetCaption(m_szNewValue);

}

void CDlgTabOffsetTable::OnClickTabDataSave() 
{
	// TODO: Add your control notification handler code here

	//	변경된 Data를 저장 하시겠습니까?
	if (MyMessageBox(MY_MESSAGE_BOX,41001,_T("Question"), M_ICONQUESTION|M_YESNO) == IDNO)
		return;

	m_plnkSystemData->m_dTabOffsetdata = m_rgdTabOffsetData[0][0];
	m_plnkSystemData->m_dTabOffsetAverage = m_rgdTabOffsetData[0][1];

	m_plnkSystemData->WriteData();
}

void CDlgTabOffsetTable::OnClickTabDataCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}

void CDlgTabOffsetTable::OnClickUseSide() 
{
	// TODO: Add your control notification handler code here
	m_iSelectedRow = 0;	
}

void CDlgTabOffsetTable::OnClickButton3() 
{
	// TODO: Add your control notification handler code here
	
}
