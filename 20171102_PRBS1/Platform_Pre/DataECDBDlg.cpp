// DataECDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "DataECDBDlg.h"
#include "MPlatformOlbSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataECDBDlg dialog


CDataECDBDlg::CDataECDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataECDBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataECDBDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDataECDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataECDBDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
		DDX_Control(pDX, IDB_EDIT_MODE, m_btnEditMode);
		//DDX_Control(pDX, IDC_MSFLEXGRID1, m_grdData);
		DDX_Control(pDX, IDC_ECID, m_editECID);
		DDX_Control(pDX, IDC_ECNAME, m_editECNAME);
		DDX_Control(pDX, IDC_ECDEF, m_editECDEF);
		DDX_Control(pDX, IDC_ECSLL, m_editECSLL);
		DDX_Control(pDX, IDC_ECSUL, m_editECSUL);
		DDX_Control(pDX, IDC_ECWLL, m_editECWLL);
		DDX_Control(pDX, IDC_ECWUL, m_editECWUL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataECDBDlg, CDialog)
	//{{AFX_MSG_MAP(CDataECDBDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataECDBDlg message handlers

BOOL CDataECDBDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString szTextTemp;
	
	// TODO: Add extra initialization here
	m_flgEditMode = FALSE;

	m_RowSel = 1;

	InitGrid();

	UpdateECData();

	OnClickGrid();

	//Data 추가
	ReadDisplayMessage(MAIN_DATA, _T("DataECDBDlg_1"), &szTextTemp);
	GetDlgItem(IDB_INSERT_DATA)->SetWindowText(szTextTemp);
	//Data 수정
	ReadDisplayMessage(MAIN_DATA, _T("DataECDBDlg_2"), &szTextTemp);
	GetDlgItem(IDB_MODIFY_DATA)->SetWindowText(szTextTemp);
	//Data 삭제
	ReadDisplayMessage(MAIN_DATA, _T("DataECDBDlg_3"), &szTextTemp);
	GetDlgItem(IDB_DELETE_DATA)->SetWindowText(szTextTemp);
	//수정모드	
	ReadDisplayMessage(MAIN_DATA, _T("DataECDBDlg_4"), &szTextTemp);
	GetDlgItem(IDB_EDIT_MODE)->SetWindowText(szTextTemp);
	//나가기
	ReadDisplayMessage(MAIN_DATA, _T("DataECDBDlg_5"), &szTextTemp);
	GetDlgItem(IDB_EXIT)->SetWindowText(szTextTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CDataECDBDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CDataECDBDlg)
	ON_EVENT(CDataECDBDlg, IDB_INSERT_DATA, -600 /* Click */, OnClickInsertData, VTS_NONE)
	ON_EVENT(CDataECDBDlg, IDB_MODIFY_DATA, -600 /* Click */, OnClickModifyData, VTS_NONE)
	ON_EVENT(CDataECDBDlg, IDB_DELETE_DATA, -600 /* Click */, OnClickDeleteData, VTS_NONE)
	ON_EVENT(CDataECDBDlg, IDB_EDIT_MODE, -600 /* Click */, OnClickEditMode, VTS_NONE)
	ON_EVENT(CDataECDBDlg, IDC_MSFLEXGRID1, -600 /* Click */, OnClickGrid, VTS_NONE)
	ON_EVENT(CDataECDBDlg, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDataECDBDlg::OnClickInsertData() 
{
	// TODO: Add your control notification handler code here

	if (m_flgEditMode != TRUE)
		return;

	m_editECID.GetWindowText(m_strECID);
	m_editECNAME.GetWindowText(m_strECNAME);
	m_editECSLL.GetWindowText(m_strECSLL);
	m_editECSUL.GetWindowText(m_strECSUL);
	m_editECWLL.GetWindowText(m_strECWLL);
	m_editECWUL.GetWindowText(m_strECWUL);
	m_editECDEF.GetWindowText(m_strECDEF);

	CString strSqlMsg;
	strSqlMsg.Format(_T("insert into EQ_EC_DATA (ECID, ECNAME, ECSLL, ECSUL, ECWLL, ECWUL, ECDEF) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s')"),
					m_strECID, m_strECNAME, m_strECSLL, m_strECSUL, m_strECWLL, m_strECWUL, m_strECDEF);

	MyInputDB.InsUpDel(strSqlMsg);

	UpdateECData();
	
	//m_pLCNet->ParamECChangeReport(eChangeRequest, pECData);	
}

void CDataECDBDlg::OnClickModifyData() 
{
	// TODO: Add your control notification handler code here

	if (m_flgEditMode != TRUE)
		return;

	m_editECID.GetWindowText(m_strECID);
	m_editECNAME.GetWindowText(m_strECNAME);
	m_editECSLL.GetWindowText(m_strECSLL);
	m_editECSUL.GetWindowText(m_strECSUL);
	m_editECWLL.GetWindowText(m_strECWLL);
	m_editECWUL.GetWindowText(m_strECWUL);
	m_editECDEF.GetWindowText(m_strECDEF);

	CString strMySql;

	strMySql.Format(_T("Update EQ_EC_DATA set ECNAME='%s', ECSLL='%s', ECSUL='%s', ECWLL='%s', ECWUL='%s', ECDEF='%s' where ECID='%s'"),
					m_strECNAME, m_strECSLL, m_strECSUL, m_strECWLL, m_strECWUL, m_strECDEF, m_strECID);

	if (!MyInputDB.InsUpDel(strMySql)) {
	//	MyInputDB.Disconnect();
		return;
	}

	UpdateECData();	
}

void CDataECDBDlg::OnClickDeleteData() 
{
	// TODO: Add your control notification handler code here
	
	if (m_flgEditMode != TRUE)
		return;
	
	CString strMySql;
	strMySql.Format(_T("delete * from EQ_EC_DATA where ECID='%s'"), m_strECID);

	if (!MyInputDB.InsUpDel(strMySql)) {
	//	MyInputDB.Disconnect();
		return;
	}	

	UpdateECData();

	OnClickGrid();
}

void CDataECDBDlg::OnClickEditMode() 
{
	// TODO: Add your control notification handler code here

	if (m_flgEditMode == TRUE)
	{
		m_editECID.EnableWindow(FALSE);
		m_editECNAME.EnableWindow(FALSE);
		m_editECDEF.EnableWindow(FALSE);
		m_editECSLL.EnableWindow(FALSE);
		m_editECSUL.EnableWindow(FALSE);
		m_editECWLL.EnableWindow(FALSE);
		m_editECWUL.EnableWindow(FALSE);

		m_btnEditMode.SetValue(FALSE);

		m_flgEditMode = FALSE;
	}
	else
	{
		m_editECID.EnableWindow(TRUE);
		m_editECNAME.EnableWindow(TRUE);
		m_editECDEF.EnableWindow(TRUE);
		m_editECSLL.EnableWindow(TRUE);
		m_editECSUL.EnableWindow(TRUE);
		m_editECWLL.EnableWindow(TRUE);
		m_editECWUL.EnableWindow(TRUE);

		m_btnEditMode.SetValue(TRUE);

		m_flgEditMode = TRUE;
	}	
}

void CDataECDBDlg::InitGrid()
{
	int nColWidth[] = { 500, 800, 4000, 1000, 1000, 1000, 1000, 1000, 1800, 1000, 1000};
	WCHAR* data[] = {_T("NO"), _T("ECID"), _T("ECNAME"), _T("ECSLL"), _T("ECSUL"), _T("ECWLL"), _T("ECWUL"), _T("ECDEF"), _T("HOST Change")};
	//m_grdData.SetColAlignment(0, 4);
	//m_grdData.SetColWidth(0, 500);
	//for (int i=0; i<9; i++) {
	//	m_grdData.SetColAlignment(i, 4);
	//	m_grdData.SetTextMatrix(0, i, data[i]);
	//	m_grdData.SetColWidth(i, nColWidth[i]);
	//}
}

void CDataECDBDlg::UpdateECData()
{
	CString strMySql, strTempValue;
	long col, row;

	strMySql = _T("Select * From EQ_EC_DATA order by ECID ASC");
	if (!MyInputDB.ExcuteQry(strMySql)) {
	//	MyInputDB.Disconnect();
		return;
	}

	col = MyInputDB.GetColSize();
	row = MyInputDB.GetRowSize();

	//// Read Data
	//if (row > 0) 
	//{
	//	m_grdData.SetRows(row + 1);
	//	CString strTemp;
	//	for (int i=0; i<row; i++) 
	//	{
	//		// 번호 ....b
	//		strTemp.Format(_T("%d"), i+1);
	//		m_grdData.SetTextMatrix(i+1, 0, strTemp);
	//		// ECID
	//		MyInputDB.GetData(0, i, strTempValue);
	//		m_grdData.SetTextMatrix(i+1, 1, (LPCTSTR)strTempValue);
	//		// ECNAME
	//		MyInputDB.GetData(1, i, strTempValue);
	//		m_grdData.SetTextMatrix(i+1, 2, (LPCTSTR)strTempValue);
	//		// ECSLL
	//		MyInputDB.GetData(7, i, strTempValue);
	//		m_grdData.SetTextMatrix(i+1, 3, (LPCTSTR)strTempValue);
	//		// ECSUL
	//		MyInputDB.GetData(8, i, strTempValue);
	//		m_grdData.SetTextMatrix(i+1, 4, (LPCTSTR)strTempValue);
	//		// ECWLL
	//		MyInputDB.GetData(9, i, strTempValue);
	//		m_grdData.SetTextMatrix(i+1, 5, (LPCTSTR)strTempValue);
	//		// ECWUL
	//		MyInputDB.GetData(10, i, strTempValue);
	//		m_grdData.SetTextMatrix(i+1, 6, (LPCTSTR)strTempValue);
	//		// ECDEF
	//		MyInputDB.GetData(11, i, strTempValue);
	//		m_grdData.SetTextMatrix(i+1, 7, (LPCTSTR)strTempValue);
	//		// HOST_CHANGE
	//		MyInputDB.GetData(12, i, strTempValue);
	//		m_grdData.SetTextMatrix(i+1, 8, (LPCTSTR)strTempValue);
	//	}
	//}
}

void CDataECDBDlg::OnClickGrid() 
{
	// TODO: Add your control notification handler code here
	CString strTemp, strECID, strSqlMsg;

	//// 선택된 행 받아오기	
	//m_RowSel = m_grdData.GetRow();

	//m_strECID = m_grdData.GetTextMatrix(m_RowSel, 1);
	//m_strECNAME = m_grdData.GetTextMatrix(m_RowSel, 2);
	//m_strECSLL = m_grdData.GetTextMatrix(m_RowSel, 3);
	//m_strECSUL = m_grdData.GetTextMatrix(m_RowSel, 4);
	//m_strECWLL = m_grdData.GetTextMatrix(m_RowSel, 5);
	//m_strECWUL = m_grdData.GetTextMatrix(m_RowSel, 6);
	//m_strECDEF = m_grdData.GetTextMatrix(m_RowSel, 7);

	m_editECID.SetWindowText((LPCTSTR)m_strECID);
	m_editECNAME.SetWindowText((LPCTSTR)m_strECNAME);
	m_editECDEF.SetWindowText((LPCTSTR)m_strECDEF);
	m_editECSLL.SetWindowText((LPCTSTR)m_strECSLL);
	m_editECSUL.SetWindowText((LPCTSTR)m_strECSUL);
	m_editECWLL.SetWindowText((LPCTSTR)m_strECWLL);
	m_editECWUL.SetWindowText((LPCTSTR)m_strECWUL);
	
	UpdateData(FALSE);	
}

void CDataECDBDlg::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	
	CDialog::OnCancel();
}
