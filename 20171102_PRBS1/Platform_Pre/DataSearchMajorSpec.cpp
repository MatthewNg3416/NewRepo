// DataSearchMajorSpec.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "DataSearchMajorSpec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSearchMajorSpec dialog


CDataSearchMajorSpec::CDataSearchMajorSpec(CString strCurrent, CString strNext, CWnd* pParent /*=NULL*/)
	: CDialog(CDataSearchMajorSpec::IDD, pParent), m_strCurrent(strCurrent), m_strNext(strNext)
{
	//{{AFX_DATA_INIT(CDataSearchMajorSpec)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDataSearchMajorSpec::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataSearchMajorSpec)
	DDX_Control(pDX, IDC_EXPLORER1, m_MyBrowser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataSearchMajorSpec, CDialog)
	//{{AFX_MSG_MAP(CDataSearchMajorSpec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataSearchMajorSpec message handlers

BOOL CDataSearchMajorSpec::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString strAddress;
	strAddress.Format(_T("http://100.100.100.100/SearchResult.asp?t1=%s&t2=%s&t3="), m_strCurrent, m_strNext);
	m_MyBrowser.Navigate(strAddress, NULL, NULL, NULL, NULL);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
