// DataModelIDList.cpp : implementation file
//

#include "stdafx.h"
//#include "mwintolb.h"
#include "DataModelIDList.h"
#include "MFileUtil.h"
#include "common.h"
#include "DefSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataModelIDList dialog


CDataModelIDList::CDataModelIDList(CString strModelFileDir, CString* pstrModelID, CWnd* pParent /*=NULL*/)
	: CDialog(CDataModelIDList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataModelIDList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strModelFileDir = strModelFileDir;
	m_pstrModelID = pstrModelID;
}


void CDataModelIDList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataModelIDList)
	DDX_Control(pDX, IDC_MODEL_LIST, m_lboxModelList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataModelIDList, CDialog)
	//{{AFX_MSG_MAP(CDataModelIDList)
	ON_LBN_SELCHANGE(IDC_MODEL_LIST, OnSelchangeModelList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataModelIDList message handlers

BEGIN_EVENTSINK_MAP(CDataModelIDList, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataModelIDList)
	ON_EVENT(CDataModelIDList, IDB_EXIT, -600 /* Click */, OnExit, VTS_NONE)
	ON_EVENT(CDataModelIDList, IDB_PAGE_UP, -600 /* Click */, OnPageUp, VTS_NONE)
	ON_EVENT(CDataModelIDList, IDB_UP, -600 /* Click */, OnUp, VTS_NONE)
	ON_EVENT(CDataModelIDList, IDB_DOWN, -600 /* Click */, OnDown, VTS_NONE)
	ON_EVENT(CDataModelIDList, IDB_PAGE_DOWN, -600 /* Click */, OnPageDown, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataModelIDList::OnExit() 
{
	if (m_iCurrentSel >= 0)
		m_lboxModelList.GetText(m_iCurrentSel, *m_pstrModelID);

	CDialog::OnCancel();
}

void CDataModelIDList::OnPageUp() 
{
	if (m_iModelCount <= 0) return;

	if (m_iCurrentSel - 10 <= 0) m_iCurrentSel = 0;
	else m_iCurrentSel -= 10;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
}

void CDataModelIDList::OnUp() 
{
	if (m_iModelCount <= 0) return;

	if (m_iCurrentSel <= 0) return;
	else m_iCurrentSel--;

	m_lboxModelList.SetCurSel(m_iCurrentSel);
}

void CDataModelIDList::OnDown() 
{
	if (m_iModelCount <= 0) return;

	if (m_iCurrentSel >= (m_iModelCount-1)) return;
	else m_iCurrentSel++;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
}

void CDataModelIDList::OnPageDown() 
{
	if (m_iModelCount <= 0) return;

	if (m_iCurrentSel + 10 >= (m_iModelCount-1)) m_iCurrentSel = m_iModelCount-1;
	else m_iCurrentSel += 10;
	
	m_lboxModelList.SetCurSel(m_iCurrentSel);
}

BOOL CDataModelIDList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iCurrentSel = -1;

	MFileUtil util;

	m_iModelCount = util.GetDirCountInDir(m_strModelFileDir);
	if (m_iModelCount > DEF_MAX_MODEL)
	{
		// hongju 임시 막음.
//		CString strMsg;
//		strMsg.Format("모델 갯수(%s)가 시스템 허용치(%s)를 초과 합니다.", m_iModelCount, DEF_MAX_MODEL);
//		MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "Manage Model", M_ICONERROR);
		return TRUE;
	}
	else if (m_iModelCount <= 0)
	{
		MyMessageBox("There is no registered model.\r\n등록된 모델이 하나도 없습니다.", "Manage Model", M_ICONERROR);
		return TRUE;
	}

	CString* pstrModelFolderList = new CString[m_iModelCount];

	util.GetDirListInDir(m_strModelFileDir, pstrModelFolderList, m_iModelCount);

	m_lboxModelList.ResetContent();

	for (int i = 0 ; i < m_iModelCount ;i++)
	{
		pstrModelFolderList[i].MakeUpper();
		m_lboxModelList.InsertString(-1, (LPCTSTR)pstrModelFolderList[i]);	
	}

	delete[] pstrModelFolderList;

	m_lboxModelList.SetCurSel(m_iCurrentSel);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataModelIDList::OnSelchangeModelList() 
{
	m_iCurrentSel = m_lboxModelList.GetCurSel();	
}
