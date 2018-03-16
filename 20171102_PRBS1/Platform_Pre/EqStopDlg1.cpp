// EqStopDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "EqStopDlg1.h"
#include "MLCNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern MPlatformOlbSystem	MOlbSystemPre;
/////////////////////////////////////////////////////////////////////////////
// CEqStopDlg dialog

CEqStopDlg::CEqStopDlg(CWnd* pParent)
	: CDialog(CEqStopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEqStopDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_plnkLCNet = MOlbSystemPre.GetLCNetComponent();
}


void CEqStopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_BUTTON1,	m_btnButton[0]);
	DDX_Control(pDX, IDB_BUTTON2,	m_btnButton[1]);
	DDX_Control(pDX, IDB_BUTTON3,	m_btnButton[2]);
	DDX_Control(pDX, IDB_BUTTON4,	m_btnButton[3]);
	DDX_Control(pDX, IDB_BUTTON5,	m_btnButton[4]);
	DDX_Control(pDX, IDB_BUTTON6,	m_btnButton[5]);
	DDX_Control(pDX, IDB_BUTTON7,	m_btnButton[6]);
	DDX_Control(pDX, IDB_BUTTON8,	m_btnButton[7]);
	DDX_Control(pDX, IDB_BUTTON9,	m_btnButton[8]);
	DDX_Control(pDX, IDB_BUTTON10,	m_btnButton[9]);
	DDX_Control(pDX, IDB_BUTTON11,	m_btnButton[10]);
	DDX_Control(pDX, IDB_BUTTON12,	m_btnButton[11]);
	DDX_Control(pDX, IDB_BUTTON13,	m_btnButton[12]);
	DDX_Control(pDX, IDB_BUTTON14,	m_btnButton[13]);
	DDX_Control(pDX, IDB_BUTTON15,	m_btnButton[14]);
	DDX_Control(pDX, IDB_BUTTON16,	m_btnButton[15]);
	DDX_Control(pDX, IDB_BUTTON17,	m_btnButton[16]);
	DDX_Control(pDX, IDB_BUTTON18,	m_btnButton[17]);
	DDX_Control(pDX, IDB_BUTTON19,	m_btnButton[18]);
	DDX_Control(pDX, IDB_BUTTON20,	m_btnButton[19]);
	DDX_Control(pDX, IDB_BUTTON21,	m_btnButton[20]);
	DDX_Control(pDX, IDB_BUTTON22,	m_btnButton[21]);
	DDX_Control(pDX, IDB_BUTTON23,	m_btnButton[22]);
	DDX_Control(pDX, IDB_BUTTON24,	m_btnButton[23]);
	DDX_Control(pDX, IDB_BUTTON25,	m_btnButton[24]);
	DDX_Control(pDX, IDB_BUTTON26,	m_btnButton[25]);
	DDX_Control(pDX, IDB_BUTTON27,	m_btnButton[26]);
	DDX_Control(pDX, IDB_BUTTON28,	m_btnButton[27]);
	
	DDX_Control(pDX, IDC_LABEL1,	m_lblText[0]);
	DDX_Control(pDX, IDC_LABEL2,	m_lblText[1]);
	DDX_Control(pDX, IDC_LABEL3,	m_lblText[2]);
	DDX_Control(pDX, IDC_LABEL4,	m_lblText[3]);
	DDX_Control(pDX, IDC_LABEL5,	m_lblText[4]);
	DDX_Control(pDX, IDC_LABEL6,	m_lblText[5]);
	DDX_Control(pDX, IDC_LABEL7,	m_lblText[6]);
	DDX_Control(pDX, IDC_LABEL8,	m_lblText[7]);
	DDX_Control(pDX, IDC_LABEL9,	m_lblText[8]);
	DDX_Control(pDX, IDC_LABEL10,	m_lblText[9]);
	DDX_Control(pDX, IDC_LABEL11,	m_lblText[10]);
	DDX_Control(pDX, IDC_LABEL12,	m_lblText[11]);
	DDX_Control(pDX, IDC_LABEL13,	m_lblText[12]);
	DDX_Control(pDX, IDC_LABEL14,	m_lblText[13]);
	DDX_Control(pDX, IDC_LABEL15,	m_lblText[14]);
	DDX_Control(pDX, IDC_LABEL16,	m_lblText[15]);
	DDX_Control(pDX, IDC_LABEL17,	m_lblText[16]);
	DDX_Control(pDX, IDC_LABEL18,	m_lblText[17]);
	DDX_Control(pDX, IDC_LABEL19,	m_lblText[18]);
	DDX_Control(pDX, IDC_LABEL20,	m_lblText[19]);
	DDX_Control(pDX, IDC_LABEL21,	m_lblText[20]);
	DDX_Control(pDX, IDC_LABEL22,	m_lblText[21]);
	DDX_Control(pDX, IDC_LABEL23,	m_lblText[22]);
	DDX_Control(pDX, IDC_LABEL24,	m_lblText[23]);
	DDX_Control(pDX, IDC_LABEL25,	m_lblText[24]);
	DDX_Control(pDX, IDC_LABEL26,	m_lblText[25]);
	DDX_Control(pDX, IDC_LABEL27,	m_lblText[26]);
	DDX_Control(pDX, IDC_LABEL28,	m_lblText[27]);
	

	//{{AFX_DATA_MAP(CEqStopDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEqStopDlg, CDialog)
	//{{AFX_MSG_MAP(CEqStopDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CEqStopDlg, CDialog)
//{{AFX_EVENTSINK_MAP(CEqStopDlg)
ON_EVENT(CEqStopDlg, IDB_BUTTON1, -600 /* Click */, OnButton1, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON2, -600 /* Click */, OnButton2, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON3, -600 /* Click */, OnButton3, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON4, -600 /* Click */, OnButton4, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON5, -600 /* Click */, OnButton5, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON6, -600 /* Click */, OnButton6, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON7, -600 /* Click */, OnButton7, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON8, -600 /* Click */, OnButton8, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON9, -600 /* Click */, OnButton9, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON10, -600 /* Click */, OnButton10, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON11, -600 /* Click */, OnButton11, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON12, -600 /* Click */, OnButton12, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON13, -600 /* Click */, OnButton13, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON14, -600 /* Click */, OnButton14, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON15, -600 /* Click */, OnButton15, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON16, -600 /* Click */, OnButton16, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON17, -600 /* Click */, OnButton17, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON18, -600 /* Click */, OnButton18, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON19, -600 /* Click */, OnButton19, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON20, -600 /* Click */, OnButton20, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON21, -600 /* Click */, OnButton21, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON22, -600 /* Click */, OnButton22, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON23, -600 /* Click */, OnButton23, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON24, -600 /* Click */, OnButton24, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON25, -600 /* Click */, OnButton25, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON26, -600 /* Click */, OnButton26, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON27, -600 /* Click */, OnButton27, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_BUTTON28, -600 /* Click */, OnButton28, VTS_NONE)

ON_EVENT(CEqStopDlg, IDB_INPUT, -600 /* Click */, OnInput, VTS_NONE)
ON_EVENT(CEqStopDlg, IDB_SAVE, -600 /* Click */, OnSave, VTS_NONE)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEqStopDlg message handlers

BOOL CEqStopDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	MOlbSystemPre.GetSystemData()->m_strStopItem[0] = _T("S/W Update");
	MOlbSystemPre.GetSystemData()->m_strStopItem[1] = _T("Recipe Setting");
	MOlbSystemPre.GetSystemData()->m_strStopItem[2] = _T("Unexpected EQP. PM");
	MOlbSystemPre.GetSystemData()->m_strStopItem[3] = _T("EQ Cleaning");
	MOlbSystemPre.GetSystemData()->m_strStopItem[4] = _T("Etc.");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[5] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[6] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[7] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[8] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[9] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[10] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[11] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[12] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[13] = _T("");
	
	MOlbSystemPre.GetSystemData()->m_strStopItem[14] = _T("OLB Align Check");
	MOlbSystemPre.GetSystemData()->m_strStopItem[15] = _T("OLB Bonding Check");
	MOlbSystemPre.GetSystemData()->m_strStopItem[16] = _T("PCB Align Check");
	MOlbSystemPre.GetSystemData()->m_strStopItem[17] = _T("PCB Bonding Check");
	MOlbSystemPre.GetSystemData()->m_strStopItem[18] = _T("OLB ACF Bonding Fail Check");
	MOlbSystemPre.GetSystemData()->m_strStopItem[19] = _T("PCB ACF Bonding Fail Check");
	MOlbSystemPre.GetSystemData()->m_strStopItem[20] = _T("SI Application Check");
	MOlbSystemPre.GetSystemData()->m_strStopItem[21] = _T("SI Hardening Fail Check");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[22] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[23] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[24] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[25] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[26] = _T("");
//	MOlbSystemPre.GetSystemData()->m_strStopItem[27] = _T("");

	MOlbSystemPre.GetSystemData()->WriteData();
	
	UpDateDisplay();

	SelectButton(0);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEqStopDlg::UpDateDisplay()
{
	for(int i = 0; i < DEF_MAX_BUTTON; i++)
	{
//		m_btnButton[i].SetCaption(MOlbSystemPre.GetSystemData()->m_strStopItem[i]);   
		m_lblText[i].SetCaption(MOlbSystemPre.GetSystemData()->m_strStopItem[i]);   

//		m_btnButton[DEF_MAX_BUTTON/2 + i].SetCaption(MOlbSystemPre.GetSystemData()->m_strStopItem[i]);   
//		m_lblText[DEF_MAX_BUTTON + i].SetCaption(MOlbSystemPre.GetSystemData()->m_strStopItem[i]); 
	}
}

void CEqStopDlg::SelectButton(int iBtnNo)
{
	m_nSelectCode = iBtnNo;
	
	for(int i = 0; i < DEF_MAX_BUTTON; i++)
	{
		if(i == iBtnNo)
		{
			m_btnButton[i].SetValue(TRUE);
		}
		else
		{
			m_btnButton[i].SetValue(FALSE);
		}
	}
}

void CEqStopDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	SelectButton(0);	
}

void CEqStopDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	SelectButton(1);
}

void CEqStopDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	SelectButton(2);
}

void CEqStopDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	SelectButton(3);
}

void CEqStopDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	SelectButton(4);
}

void CEqStopDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	SelectButton(5);
}

void CEqStopDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	SelectButton(6);
}

void CEqStopDlg::OnButton8() 
{
	// TODO: Add your control notification handler code here
	SelectButton(7);
}

void CEqStopDlg::OnButton9() 
{
	// TODO: Add your control notification handler code here
	SelectButton(8);
}

void CEqStopDlg::OnButton10() 
{
	// TODO: Add your control notification handler code here
	SelectButton(9);
}

void CEqStopDlg::OnButton11()
{
	SelectButton(10);

}
void CEqStopDlg::OnButton12() 
{
	// TODO: Add your control notification handler code here
	SelectButton(11);
}

void CEqStopDlg::OnButton13() 
{
	// TODO: Add your control notification handler code here
	SelectButton(12);
}

void CEqStopDlg::OnButton14() 
{
	// TODO: Add your control notification handler code here
	SelectButton(13);
}


void CEqStopDlg::OnButton15() 
{
	// TODO: Add your control notification handler code here
	SelectButton(14);
}

void CEqStopDlg::OnButton16() 
{
	// TODO: Add your control notification handler code here
	SelectButton(15);
}

void CEqStopDlg::OnButton17() 
{
	// TODO: Add your control notification handler code here
	SelectButton(16);
}

void CEqStopDlg::OnButton18() 
{
	// TODO: Add your control notification handler code here
	SelectButton(17);
}

void CEqStopDlg::OnButton19() 
{
	// TODO: Add your control notification handler code here
	SelectButton(18);
}

void CEqStopDlg::OnButton20() 
{
	// TODO: Add your control notification handler code here
	SelectButton(19);
}

void CEqStopDlg::OnButton21()
{
	SelectButton(20);
	
}
void CEqStopDlg::OnButton22() 
{
	// TODO: Add your control notification handler code here
	SelectButton(21);
}

void CEqStopDlg::OnButton23() 
{
	// TODO: Add your control notification handler code here
	SelectButton(22);
}

void CEqStopDlg::OnButton24() 
{
	// TODO: Add your control notification handler code here
	SelectButton(23);
}

void CEqStopDlg::OnButton25() 
{
	// TODO: Add your control notification handler code here
	SelectButton(24);
}

void CEqStopDlg::OnButton26() 
{
	// TODO: Add your control notification handler code here
	SelectButton(25);
}

void CEqStopDlg::OnButton27() 
{
	// TODO: Add your control notification handler code here
	SelectButton(26);
}

void CEqStopDlg::OnButton28() 
{
	// TODO: Add your control notification handler code here
	SelectButton(27);
}


void CEqStopDlg::OnInput() 
{
	if(m_nSelectCode < 14)
	{
		if(MOlbSystemPre.GetSystemData()->m_strStopItem[m_nSelectCode] != _T(""))
		{
			m_plnkLCNet->EqProcessStopReport(ePause, _ttoi(MOlbSystemPre.GetSystemData()->m_strStopItem[m_nSelectCode]),
				MOlbSystemPre.GetSystemData()->m_strStopItem[m_nSelectCode]);
		}
	}
	else
	{
		if(MOlbSystemPre.GetSystemData()->m_strStopItem[m_nSelectCode] != _T(""))
		{
			m_plnkLCNet->EqProcessStopReport(ePause, _ttoi(MOlbSystemPre.GetSystemData()->m_strStopItem[m_nSelectCode]),
			MOlbSystemPre.GetSystemData()->m_strStopItem[m_nSelectCode]);
		}
	}
	
	OnOK();
}

void CEqStopDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CEqStopDlg::OnSave() 
{
	CString strTemp;

	GetDlgItemText(IDC_STR, strTemp);

	if(m_nSelectCode < 14)
	{
		MOlbSystemPre.GetSystemData()->m_strStopItem[m_nSelectCode] = strTemp;
	}
	else
	{
		MOlbSystemPre.GetSystemData()->m_strStopItem[m_nSelectCode] = strTemp;
	}

	MOlbSystemPre.GetSystemData()->WriteData();

	UpDateDisplay();

	return;
	
}

