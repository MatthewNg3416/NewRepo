// ManualIOMonitoringDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "ManualIOMonitoringDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

extern MPlatformOlbSystem	MOlbSystemPre;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManualIOMonitoringDlg dialog


CManualIOMonitoringDlg::CManualIOMonitoringDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualIOMonitoringDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualIOMonitoringDlg)
	//}}AFX_DATA_INIT
	m_iCurrentPage_In = m_iCurrentPage_Out = 0;	
	m_iAddrBase_In = m_iAddrBase_Out = 0;
	m_iMaxPage_In = DEF_IOMONITOR_MAX_INPUT_PAGE;
	m_iMaxPage_Out = DEF_IOMONITOR_MAX_OUTPUT_PAGE;

	m_brBckColor.CreateSolidBrush(BACKGROUND_COLOR);
}


void CManualIOMonitoringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualIOMonitoringDlg)
	DDX_Control(pDX, IDB_NEXT, m_bNext);
	DDX_Control(pDX, IDB_PREVIOUS, m_bPrev);
	DDX_Control(pDX, IDB_NEXT2, m_bNext2);
	DDX_Control(pDX, IDB_PREVIOUS2, m_bPrev2);
	//}}AFX_DATA_MAP
	
	DDX_Control(pDX, IDC_IO_TITLE0, m_lblIOTitle_In[0]);
	DDX_Control(pDX, IDC_IO_TITLE1, m_lblIOTitle_In[1]);
	DDX_Control(pDX, IDC_IO_TITLE2, m_lblIOTitle_Out[0]);
	DDX_Control(pDX, IDC_IO_TITLE3, m_lblIOTitle_Out[1]);

	DDX_Control(pDX, IDB_IO_0, m_btnIO_In[0]);
	DDX_Control(pDX, IDB_IO_1, m_btnIO_In[1]);
	DDX_Control(pDX, IDB_IO_2, m_btnIO_In[2]);
	DDX_Control(pDX, IDB_IO_3, m_btnIO_In[3]);
	DDX_Control(pDX, IDB_IO_4, m_btnIO_In[4]);
	DDX_Control(pDX, IDB_IO_5, m_btnIO_In[5]);
	DDX_Control(pDX, IDB_IO_6, m_btnIO_In[6]);
	DDX_Control(pDX, IDB_IO_7, m_btnIO_In[7]);

	DDX_Control(pDX, IDB_IO_8, m_btnIO_In[8]);
	DDX_Control(pDX, IDB_IO_9, m_btnIO_In[9]);
	DDX_Control(pDX, IDB_IO_A, m_btnIO_In[10]);
	DDX_Control(pDX, IDB_IO_B, m_btnIO_In[11]);
	DDX_Control(pDX, IDB_IO_C, m_btnIO_In[12]);
	DDX_Control(pDX, IDB_IO_D, m_btnIO_In[13]);
	DDX_Control(pDX, IDB_IO_E, m_btnIO_In[14]);
	DDX_Control(pDX, IDB_IO_F, m_btnIO_In[15]);

	DDX_Control(pDX, IDB_IO_10, m_btnIO_Out[0]);
	DDX_Control(pDX, IDB_IO_11, m_btnIO_Out[1]);
	DDX_Control(pDX, IDB_IO_12, m_btnIO_Out[2]);
	DDX_Control(pDX, IDB_IO_13, m_btnIO_Out[3]);
	DDX_Control(pDX, IDB_IO_14, m_btnIO_Out[4]);
	DDX_Control(pDX, IDB_IO_15, m_btnIO_Out[5]);
	DDX_Control(pDX, IDB_IO_16, m_btnIO_Out[6]);
	DDX_Control(pDX, IDB_IO_17, m_btnIO_Out[7]);

	DDX_Control(pDX, IDB_IO_18, m_btnIO_Out[8]);
	DDX_Control(pDX, IDB_IO_19, m_btnIO_Out[9]);
	DDX_Control(pDX, IDB_IO_1A, m_btnIO_Out[10]);
	DDX_Control(pDX, IDB_IO_1B, m_btnIO_Out[11]);
	DDX_Control(pDX, IDB_IO_1C, m_btnIO_Out[12]);
	DDX_Control(pDX, IDB_IO_1D, m_btnIO_Out[13]);
	DDX_Control(pDX, IDB_IO_1E, m_btnIO_Out[14]);
	DDX_Control(pDX, IDB_IO_1F, m_btnIO_Out[15]);
}


BEGIN_MESSAGE_MAP(CManualIOMonitoringDlg, CDialog)
	//{{AFX_MSG_MAP(CManualIOMonitoringDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualIOMonitoringDlg message handlers

void CManualIOMonitoringDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(),&m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CManualIOMonitoringDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_plnkIO = MOlbSystemPre.GetIOComponent();
	m_plnkIO->GetIOTable(m_IODataInput, m_IODataOutput);

	UpdateDisplay_In();
	UpdateDisplay_Out();

	m_uiTimerID = SetTimer(1, 300, NULL);
	
	CString szTextTemp;

	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_10"), &szTextTemp);
//	입력/출력 접점
	SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_1"), &szTextTemp);
//	이전
	GetDlgItem(IDB_PREVIOUS)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_2"), &szTextTemp);
//	다음
	GetDlgItem(IDB_NEXT)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_3"), &szTextTemp);
//	맨 앞
	GetDlgItem(IDB_TOP)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_4"), &szTextTemp);
//	맨 뒤
	GetDlgItem(IDB_BOTTOM)->SetWindowText(szTextTemp);

	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_5"), &szTextTemp);
//	이전
	GetDlgItem(IDB_PREVIOUS2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_6"), &szTextTemp);
//	다음
	GetDlgItem(IDB_NEXT2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_7"), &szTextTemp);
//	맨 앞
	GetDlgItem(IDB_TOP2)->SetWindowText(szTextTemp);
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_8"), &szTextTemp);
//	맨 뒤
	GetDlgItem(IDB_BOTTOM2)->SetWindowText(szTextTemp);


//	나가기
	ReadDisplayMessage(MAIN_MANUAL, _T("ManualIOMonitoringDlg_9"), &szTextTemp);
	GetDlgItem(IDB_EXIT1)->SetWindowText(szTextTemp);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CManualIOMonitoringDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualIOMonitoringDlg)
	ON_EVENT(CManualIOMonitoringDlg, IDB_PREVIOUS, -600 /* Click */, OnPrevious, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_NEXT, -600 /* Click */, OnNext, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_TOP, -600 /* Click */, OnTop, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_BOTTOM, -600 /* Click */, OnBottom, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_10, -600 /* Click */, OnIo10, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_11, -600 /* Click */, OnIo11, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_12, -600 /* Click */, OnIo12, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_13, -600 /* Click */, OnIo13, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_14, -600 /* Click */, OnIo14, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_15, -600 /* Click */, OnIo15, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_16, -600 /* Click */, OnIo16, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_17, -600 /* Click */, OnIo17, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_18, -600 /* Click */, OnIo18, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_19, -600 /* Click */, OnIo19, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_1A, -600 /* Click */, OnIo1A, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_1B, -600 /* Click */, OnIo1B, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_1C, -600 /* Click */, OnIo1C, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_1D, -600 /* Click */, OnIo1D, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_1E, -600 /* Click */, OnIo1E, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_IO_1F, -600 /* Click */, OnIo1F, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_EXIT1, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_PREVIOUS2, -600 /* Click */, OnClickPrevious2, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_NEXT2, -600 /* Click */, OnClickNext2, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_TOP2, -600 /* Click */, OnClickTop2, VTS_NONE)
	ON_EVENT(CManualIOMonitoringDlg, IDB_BOTTOM2, -600 /* Click */, OnClickBottom2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CManualIOMonitoringDlg::OnPrevious() 
{
	// TODO: Add your control notification handler code here
	if (m_iCurrentPage_In > 0)	
		m_iCurrentPage_In--;

	UpdateDisplay_In();	
}

void CManualIOMonitoringDlg::OnNext() 
{
	// TODO: Add your control notification handler code here
	if (m_iCurrentPage_In < m_iMaxPage_In - 1)
		m_iCurrentPage_In++;
	
	UpdateDisplay_In();	
}

void CManualIOMonitoringDlg::OnTop() 
{
	// TODO: Add your control notification handler code here
	m_iCurrentPage_In = 0;
	
	UpdateDisplay_In();	
}

void CManualIOMonitoringDlg::OnBottom() 
{
	// TODO: Add your control notification handler code here
	m_iCurrentPage_In = m_iMaxPage_In - 1;

	UpdateDisplay_In();	
}

void CManualIOMonitoringDlg::OnClickPrevious2() 
{
	// TODO: Add your control notification handler code here
	if (m_iCurrentPage_Out > 0)	
		m_iCurrentPage_Out--;

	UpdateDisplay_Out();		
}

void CManualIOMonitoringDlg::OnClickNext2() 
{
	// TODO: Add your control notification handler code here
	if (m_iCurrentPage_Out < m_iMaxPage_Out - 1)
		m_iCurrentPage_Out++;
	
	UpdateDisplay_Out();		
}

void CManualIOMonitoringDlg::OnClickTop2() 
{
	// TODO: Add your control notification handler code here
	m_iCurrentPage_Out = 0;
	
	UpdateDisplay_Out();		
}

void CManualIOMonitoringDlg::OnClickBottom2() 
{
	// TODO: Add your control notification handler code here
	m_iCurrentPage_Out = m_iMaxPage_Out - 1;

	UpdateDisplay_Out();	
	
}

void CManualIOMonitoringDlg::OnIo10() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 0);
}

void CManualIOMonitoringDlg::OnIo11() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 1);
}

void CManualIOMonitoringDlg::OnIo12() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 2);
}

void CManualIOMonitoringDlg::OnIo13() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 3);
}

void CManualIOMonitoringDlg::OnIo14() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 4);
}

void CManualIOMonitoringDlg::OnIo15() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 5);
}

void CManualIOMonitoringDlg::OnIo16() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 6);
}

void CManualIOMonitoringDlg::OnIo17() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 7);
}

void CManualIOMonitoringDlg::OnIo18() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 8);
}

void CManualIOMonitoringDlg::OnIo19() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 9);
}

void CManualIOMonitoringDlg::OnIo1A() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 10);
}

void CManualIOMonitoringDlg::OnIo1B() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 11);
}

void CManualIOMonitoringDlg::OnIo1C() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 12);
}

void CManualIOMonitoringDlg::OnIo1D() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 13);
}

void CManualIOMonitoringDlg::OnIo1E() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 14);
}

void CManualIOMonitoringDlg::OnIo1F() 
{
	// TODO: Add your control notification handler code here
	ToggleOutput(DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out + 15);
}

void CManualIOMonitoringDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	if (m_uiTimerID)
		KillTimer(m_uiTimerID);

	CDialog::OnCancel();
}

void CManualIOMonitoringDlg::UpdateDisplay_In()
{
	int tempNum;
	m_iAddrBase_In = DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_In;
	
	CString m_sInputTitleText[DEF_IOMONITOR_MAX_TITLE_PER_PAGE * DEF_IOMONITOR_MAX_INPUT_PAGE] = {	
		                                "X000 ~ X007", "X008 ~ X00F", 
										"X010 ~ X017", "X018 ~ X01F",
										"X020 ~ X027", "X028 ~ X02F", 
										"X030 ~ X037", "X038 ~ X03F",
										"X040 ~ X047", "X048 ~ X04F", 
										"X050 ~ X057", "X058 ~ X05F",
										"X060 ~ X067", "X068 ~ X06F", 
										"X070 ~ X077", "X078 ~ X07F",
										"X080 ~ X087", "X088 ~ X08F", 
										"X090 ~ X097", "X098 ~ X09F",
										"X100 ~ X107", "X108 ~ X10F", 
										"X110 ~ X117", "X118 ~ X11F",
										"X120 ~ X127", "X128 ~ X12F", 
										"X130 ~ X137", "X138 ~ X13F",
										"X140 ~ X147", "X148 ~ X14F", 
										"X150 ~ X157", "X158 ~ X15F",
										"X160 ~ X167", "X168 ~ X16F",
										/*/
										"X170 ~ X177", "X178 ~ X17F",
										"X180 ~ X187", "X188 ~ X18F",
										"X190 ~ X197", "X198 ~ X19F",
										"X200 ~ X207", "X208 ~ X20F", 
										"X210 ~ X217", "X218 ~ X21F",
										"X220 ~ X227", "X228 ~ X22F", 
										"X230 ~ X237", "X238 ~ X23F",
										"X240 ~ X247", "X248 ~ X24F", 
										"X250 ~ X257", "X258 ~ X25F",
#ifdef DEF_GATE_SYSTEM
										"X260 ~ X267", "X268 ~ X26F", 
										"X270 ~ X277", "X278 ~ X27F",
										"X280 ~ X287", "X288 ~ X28F", 
										"X290 ~ X297", "X298 ~ X29F",
#endif
										/*/
										};

	if (m_iCurrentPage_In == 0)
		m_bPrev.EnableWindow(FALSE);
	else
		m_bPrev.EnableWindow(TRUE);

	CString* rgstrTitle = m_sInputTitleText;
	
	for (int i = 0; i < DEF_IOMONITOR_MAX_TITLE_PER_PAGE; i++)
	{
		tempNum = DEF_IOMONITOR_MAX_TITLE_PER_PAGE * m_iCurrentPage_In + i;
		m_lblIOTitle_In[i].SetCaption(rgstrTitle[tempNum]);
	}
	
	if (m_iCurrentPage_In == m_iMaxPage_In - 1)
		m_bNext.EnableWindow(FALSE);
	else
		m_bNext.EnableWindow(TRUE);

	CString strIOName;
	CString strTemp;
	SIOTABLE* rgIO = m_IODataInput;
	
	for (int i=0 ; i<DEF_IOMONITOR_MAX_IO_PER_PAGE ; i++)
	{
		strIOName.Format(_T("%s"), rgIO[m_iAddrBase_In+i].Name);
		strTemp = strIOName.Left(5);
		if (!strTemp.Compare(_T("Spare")))
		{
			m_btnIO_In[i].SetCaption(_T("Spare"));
			m_btnIO_In[i].SetForeColor(LTGRAY);
		}
		else
		{
			m_btnIO_In[i].SetWindowText(strIOName);
			m_btnIO_In[i].SetForeColor(BLACK);
		}
	}
}

void CManualIOMonitoringDlg::UpdateDisplay_Out()
{
	int tempNum;
	m_iAddrBase_Out = DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out;
	
	CString m_sOutputTitleText[DEF_IOMONITOR_MAX_TITLE_PER_PAGE * DEF_IOMONITOR_MAX_OUTPUT_PAGE] = {	
		                                "Y000 ~ Y007", "Y008 ~ Y00F", 
										"Y010 ~ X017", "Y018 ~ Y01F",
										"Y020 ~ Y027", "Y028 ~ Y02F", 
										"Y030 ~ Y037", "Y038 ~ Y03F",
										"Y040 ~ Y047", "Y048 ~ Y04F", 
										"Y050 ~ Y057", "Y058 ~ Y05F",
										"Y060 ~ Y067", "Y068 ~ Y06F", 
										"Y070 ~ Y077", "Y078 ~ Y07F",
										"Y080 ~ Y087", "Y088 ~ Y08F", 
										"Y090 ~ Y097", "Y098 ~ Y09F",
										"Y100 ~ Y107", "Y108 ~ Y10F", 
										"Y110 ~ Y117", "Y118 ~ Y11F",
										"Y120 ~ Y127", "Y128 ~ Y12F", 
										"Y130 ~ Y137", "Y138 ~ Y13F",
										"Y140 ~ Y147", "Y148 ~ Y14F", 
										"Y150 ~ Y157", "Y158 ~ Y15F",
										"Y160 ~ Y167", "Y168 ~ Y16F", 
										"Y170 ~ Y177", "Y178 ~ Y17F",
										"Y180 ~ Y187", "Y188 ~ Y18F", 
#ifdef DEF_SOURCE_SYSTEM
										"Y190 ~ Y197", "Y198 ~ Y19F",
										"Y200 ~ Y207", "Y208 ~ Y20F",
										"Y210 ~ Y217", "Y218 ~ Y21F", 	
										"Y220 ~ Y227", "Y228 ~ Y22F", 
										"Y230 ~ Y237", "Y238 ~ Y23F", 
										"Y240 ~ Y247", "Y248 ~ Y24F", 
#endif
										/*/
										"Y250 ~ Y257", "Y258 ~ Y25F", 	
										"Y260 ~ Y267", "Y268 ~ Y26F", 
										"Y270 ~ Y277", "Y278 ~ Y27F",
										"Y280 ~ Y287", "Y288 ~ Y28F", 
										"Y290 ~ Y297", "Y298 ~ Y29F",
										"Y300 ~ Y307", "Y308 ~ Y30F",
										/*/
										};

	if (m_iCurrentPage_Out == 0)
		m_bPrev2.EnableWindow(FALSE);
	else
		m_bPrev2.EnableWindow(TRUE);

	CString* rgstrTitle = m_sOutputTitleText;
	
	for (int i = 0; i < DEF_IOMONITOR_MAX_TITLE_PER_PAGE; i++)
	{
		tempNum = DEF_IOMONITOR_MAX_TITLE_PER_PAGE * m_iCurrentPage_Out + i;
		m_lblIOTitle_Out[i].SetCaption(rgstrTitle[tempNum]);
	}
	
	if (m_iCurrentPage_Out == m_iMaxPage_Out - 1)
		m_bNext2.EnableWindow(FALSE);
	else
		m_bNext2.EnableWindow(TRUE);

	CString strIOName;
	CString strTemp;
	SIOTABLE* rgIO = m_IODataOutput;
	
	for (int i=0 ; i<DEF_IOMONITOR_MAX_IO_PER_PAGE ; i++)
	{
		strIOName.Format(_T("%s"), rgIO[m_iAddrBase_Out+i].Name);
		strTemp = strIOName.Left(5);
		if (!strTemp.Compare(_T("Spare")))
		{
			m_btnIO_Out[i].SetCaption(_T("Spare"));
			m_btnIO_Out[i].SetForeColor(LTGRAY);
		}
		else
		{
			m_btnIO_Out[i].SetWindowText(strIOName);
			m_btnIO_Out[i].SetForeColor(BLACK);
		}
	}
}

int CManualIOMonitoringDlg::ToggleOutput(int iAddr)
{
	//101001.KKY_____
	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
		return 0;
	//_______________

	int iResult = SUCCESS;

	if (m_IODataOutput[iAddr].Address != 0)
	{
		iResult = m_plnkIO->OutputToggle(m_IODataOutput[iAddr].Address);
		if (iResult != SUCCESS)
		{
			//IO 출력 동작이 실패했습니다.
			MyMessageBox(MY_MESSAGE_BOX,30100, _T("Confirm"), MB_ICONQUESTION|MB_YESNO);
		}
	}
	return iResult;
}

void CManualIOMonitoringDlg::UpdateButton()
{
	m_iAddrBase_In = DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_In;
	m_iAddrBase_Out = DEF_IOMONITOR_MAX_IO_PER_PAGE * m_iCurrentPage_Out;

	for (int i=0 ; i<DEF_IOMONITOR_MAX_IO_PER_PAGE ; i++)	// Output Display인 경우
	{
		if (m_IODataInput[m_iAddrBase_In+i].Address != 0)
		{
			if (m_plnkIO->IsOn(m_IODataInput[m_iAddrBase_In+i].Address))
			{
				m_btnIO_In[i].SetValue(TRUE);
			}
			else
			{
				m_btnIO_In[i].SetValue(FALSE);
			}
		}
		if (m_IODataOutput[m_iAddrBase_Out+i].Address != 0)
		{
			if (m_plnkIO->IsOn(m_IODataOutput[m_iAddrBase_Out+i].Address))
			{
				m_btnIO_Out[i].SetValue(TRUE);
			}
			else
			{
				m_btnIO_Out[i].SetValue(FALSE);
			}
		}
	}
}

void CManualIOMonitoringDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == m_uiTimerID)
	{
		UpdateButton();
	}	

	CDialog::OnTimer(nIDEvent);
}