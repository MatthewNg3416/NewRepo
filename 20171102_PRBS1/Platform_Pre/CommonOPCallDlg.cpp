// CommonOPCallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "CommonOPCallDlg.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommonOPCallDlg dialog
extern	MPlatformOlbSystem	MOlbSystemPre;

CCommonOPCallDlg::CCommonOPCallDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonOPCallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommonOPCallDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iSelect = -1;
}


void CCommonOPCallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommonOPCallDlg)
	DDX_Control(pDX, IDC_EDITLABEL, m_editLabl);
	DDX_Control(pDX, IDC_PICTURE, m_MyIcon);
	DDX_Control(pDX, ID_SELECT1, m_bSelect1);
	DDX_Control(pDX, ID_SELECT2, m_bSelect2);
	DDX_Control(pDX, ID_MYOK, m_bOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommonOPCallDlg, CDialog)
	//{{AFX_MSG_MAP(CCommonOPCallDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommonOPCallDlg message handlers

void CCommonOPCallDlg::SetMessage(CString str)
{
	CString tmpMessage;
	CString tmpSubMessage;

	m_strArray.Add(str);
	if (m_strArray.GetSize() > 2)
		m_strArray.RemoveAt(0);
	for (int i=0; i<m_strArray.GetSize(); i++)
	{
		tmpSubMessage = i==0 ? m_strArray.GetAt(i) : "\n" +m_strArray.GetAt(i);
		tmpMessage += tmpSubMessage;
	}
	m_editLabl.SetCaption(tmpMessage);
}

BEGIN_EVENTSINK_MAP(CCommonOPCallDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCommonOPCallDlg)
	ON_EVENT(CCommonOPCallDlg, ID_MYOK, -600 /* Click */, OnClickMyok, VTS_NONE)
	ON_EVENT(CCommonOPCallDlg, ID_SELECT1, -600 /* Click */, OnClickSelect1, VTS_NONE)
	ON_EVENT(CCommonOPCallDlg, ID_SELECT2, -600 /* Click */, OnClickSelect2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCommonOPCallDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strTemp;
	WCHAR pBuf[MAX_PATH] = {0};
	::GetCurrentDirectory(sizeof(pBuf), pBuf);
	strTemp.Format(_T("%s\\res\\infoicon.bmp"),pBuf);
	m_MyIcon.SetPicture(strTemp); 
	m_MyIcon.SetPicturePosition(10);
	m_MyIcon.SetPictureKeepRatio(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommonOPCallDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectArea;
	GetClientRect(&rectArea);
	dc.FillRect(rectArea.operator LPRECT(), &m_brBckColor);
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CCommonOPCallDlg::OnClickSelect1() 
{
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetSelectState(m_iSelect, 0);
	
	m_strArray.RemoveAll();
	m_editLabl.SetCaption(_T(""));
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetOperatorCall(FALSE, "Select 1");

	ShowWindow(SW_HIDE);
}

void CCommonOPCallDlg::OnClickSelect2() 
{
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetSelectState(m_iSelect, 1);
	
	m_strArray.RemoveAll();
	m_editLabl.SetCaption(_T(""));
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetOperatorCall(FALSE, "Select 2");

	ShowWindow(SW_HIDE);
}

void CCommonOPCallDlg::OnClickMyok() 
{
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetSelectState(m_iSelect, 2);

	m_strArray.RemoveAll();
	m_editLabl.SetCaption(_T(""));
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetOperatorCall(FALSE, "Select 3");

	ShowWindow(SW_HIDE);
}

void CCommonOPCallDlg::SetSelect(int iMode)
{
	m_iSelect = iMode;

	if (m_iSelect == NULL)
	{
		m_bSelect1.SetCaption(_T("Yes"));
		m_bSelect2.SetCaption(_T("No"));
		m_bOK.SetCaption(_T("OK"));

		m_bSelect1.ShowWindow(FALSE);
		m_bSelect2.ShowWindow(FALSE);
		m_bOK.ShowWindow(TRUE);
	}
	else if (m_iSelect == DEF_FEEDER1_TABIC_EMPTY)
	{
		m_bSelect1.SetCaption(_T("Yes"));
		m_bSelect2.SetCaption(_T("No"));
		m_bOK.SetCaption(_T("OK"));

		m_bSelect1.ShowWindow(TRUE);
		m_bSelect2.ShowWindow(TRUE);
		m_bOK.ShowWindow(FALSE);
	}
	else if (m_iSelect ==DEF_FEEDER2_TABIC_EMPTY)
	{
		m_bSelect1.SetCaption(_T("Yes"));
		m_bSelect2.SetCaption(_T("No"));
		m_bOK.SetCaption(_T("OK"));

		m_bSelect1.ShowWindow(TRUE);
		m_bSelect2.ShowWindow(TRUE);
		m_bOK.ShowWindow(FALSE);
	}
}