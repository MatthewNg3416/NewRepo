// DlgTabICLoss.cpp : implementation file
//

#include "stdafx.h"
#include "mplatformolb.h"
#include "MPlatformOlbSystem.h"
#include "DlgTabICLoss.h"
#include "DBTabLoss.h"
#include "common.h"
#include "MSystemData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTabICLoss dialog
extern MPlatformOlbSystem	MOlbSystemPre;

CDlgTabICLoss::CDlgTabICLoss(int iWorkGroup, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTabICLoss::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTabICLoss)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	if (iWorkGroup == 0)
		m_pDBTabLoss = new CDBTabLoss(DEF_SYSTEM_LOG_TABIC_LOSS_PATH_FRONT, DEF_SYSTEM_DB_TABIC_ORG_PATH, _T("TabLoss"), 14);
	else
		m_pDBTabLoss = new CDBTabLoss(DEF_SYSTEM_LOG_TABIC_LOSS_PATH_REAR, DEF_SYSTEM_DB_TABIC_ORG_PATH, _T("TabLoss"), 14);
	m_pDBTabLoss->Initialize();

	m_plnkSystemData = MOlbSystemPre.GetSystemData();

	m_iColumnCnt = 0;
}


void CDlgTabICLoss::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTabICLoss)
	DDX_Control(pDX, IDC_LIST_TRAY, m_listResult);
	DDX_Control(pDX, IDC_DATEPICKER_END, m_dtEndDay);
	DDX_Control(pDX, IDC_DATEPICKER_START, m_dtStartDay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTabICLoss, CDialog)
	//{{AFX_MSG_MAP(CDlgTabICLoss)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTabICLoss message handlers

BOOL CDlgTabICLoss::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFont font;
	font.CreateFont(-15,0,0,0,FW_BOLD,0,0,0,0,0,0,ANTIALIASED_QUALITY, 0, _T("Arial"));
	m_listResult.SetFont(&font);
	m_strResult.Empty();
	m_strTitle = _T("TabICLoss");
//@	SetWindowText(m_strTitle + _T("[LogPath:") + m_plnkSystemData->m_strTrayLogPath + _T("]"));
	SetWindowText(m_strTitle);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTabICLoss::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_pDBTabLoss;
}

BEGIN_EVENTSINK_MAP(CDlgTabICLoss, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgTabICLoss)
	ON_EVENT(CDlgTabICLoss, IDB_SEARCH_PCBLOG, -600 /* Click */, OnClickSearchPcblog, VTS_NONE)
	ON_EVENT(CDlgTabICLoss, IDB_LOG_PATH, -600 /* Click */, OnClickLogPath, VTS_NONE)
	ON_EVENT(CDlgTabICLoss, IDB_SAVE, -600 /* Click */, OnClickSave, VTS_NONE)
	ON_EVENT(CDlgTabICLoss, IDB_EXIT, -600 /* Click */, OnClickExit, VTS_NONE)
	ON_EVENT(CDlgTabICLoss, IDB_SEARCH_PCBLOG_SHIFT, -600 /* Click */, OnClickSearchPcblogShift, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDlgTabICLoss::OnClickSearchPcblog()
{
	CString strDate;
	CString strFName;
	CString strQuery;

	int nSYear, nSMonth, nSDay;
	int nEYear, nEMonth, nEDay;
	m_dtStartDay.GetWindowText(strDate);
	_stscanf(strDate, _T("%d-%d-%d"), &nSYear, &nSMonth, &nSDay);
	m_dtEndDay.GetWindowText(strDate);
	_stscanf(strDate, _T("%d-%d-%d"), &nEYear, &nEMonth, &nEDay);

	COleDateTime SDay(nSYear, nSMonth, nSDay, 0, 0, 0);
	COleDateTime EDay(nEYear, nEMonth, nEDay, 0, 0, 0);
	COleDateTimeSpan ElapseDay(1, 0, 0, 0);
	COleDateTime SDayTime, EDayTime;

	m_listResult.LockWindowUpdate();

	int i=0, j=0;
	for (i=0; i<m_iColumnCnt; i++)
		m_listResult.DeleteColumn(0);
	m_listResult.DeleteAllItems();

	m_iColumnCnt = DEF_MAX_COLUMN_TABERR;

	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_listResult.SetExtendedStyle(dwExStyle);

	LV_COLUMN lvcolumn;
//@	lvcolumn.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt=LVCFMT_LEFT;
	lvcolumn.iSubItem = 0;
	lvcolumn.cchTextMax = 100;

	m_strResult.Empty();
	m_strResult = _T("");

	for (i=0; i<m_iColumnCnt; i++)
	{
//		swprintf(lvcolumn.pszText, _T("%s"), m_pDBTabLoss->GetFieldName(i));
		lvcolumn.iSubItem = i;
		lvcolumn.pszText = (LPTSTR)((LPCTSTR)m_pDBTabLoss->GetFieldName(i));
		if (i == 0)
			lvcolumn.cx = 100;
		else
			lvcolumn.cx = 65;
		m_listResult.InsertColumn(i, &lvcolumn);
		m_strResult += (m_pDBTabLoss->GetFieldName(i) + _T(","));
	}
	m_strResult += _T("\n");
	
	CString strData;
	int iRowSize = 0, iColSize = 0;	
	int iTotSize = 0;
	int iTotCnt[DEF_MAX_COLUMN_TABERR] = {0, };
	for (EDay; EDay>=SDay; EDay -= ElapseDay)
	{
		if (TRUE == m_pDBTabLoss->IsConnected()) m_pDBTabLoss->Disconnect();		

		strDate = m_pDBTabLoss->GetFormatDate(EDay);
		strFName.Format(_T("%s\\%s.mdb"), m_pDBTabLoss->GetPath(), strDate);
		if (FALSE == m_pDBTabLoss->IsFileExist(strFName))
			continue;

		strQuery.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False"), strFName);
		if (FALSE == m_pDBTabLoss->Connect(strQuery))
			continue;
		strQuery.Format(_T("Select * From %s Order by %s ASC"), m_pDBTabLoss->GetTableName(), m_pDBTabLoss->GetFieldName(0));
//		if (FALSE == m_pDBTabLoss->ExcuteQryEx(strQuery))
		if (FALSE == m_pDBTabLoss->ExcuteQry(strQuery))
			continue;

		iRowSize = m_pDBTabLoss->GetRowSize();
//@		iColSize = m_pDBTabLoss->GetColSize();
		iColSize = m_iColumnCnt;
		if (iRowSize > 0)
		{
			for (i=iTotSize; i<iTotSize+iRowSize; i++)
			{
				for (j=0; j<iColSize; j++)
				{
					m_pDBTabLoss->GetData(j, i-iTotSize, strData);
					if (j==0)
					{
						itemDisplay(j, i-iTotSize, strData);
						m_strResult += (strData + _T(","));
					}
					else
					{
						iTotCnt[j] += _ttoi(strData);
						itemDisplay(j, i-iTotSize, strData);
						m_strResult += (strData + _T(","));
					}
				}
				m_strResult += _T("\n");
			}
			iTotSize += iRowSize;
		}
	}

	for (i=1; i<DEF_MAX_COLUMN_TABERR; i++)
		iTotCnt[0] += iTotCnt[i];

	for (i=0; i<DEF_MAX_COLUMN_TABERR; i++)
	{
		strData.Format(_T("Total:%d"), iTotCnt[i]);
		itemDisplay(i, iTotSize, strData, RGB(128, 128, 0));
	}

	m_listResult.UnlockWindowUpdate();
}

void CDlgTabICLoss::itemDisplay(int iCol, int iRow, CString strData, COLORREF crBackground)
{
	/*/
	LV_ITEM lvitem;
	lvitem.mask=LVIF_TEXT;
	lvitem.iItem=iRow;
	lvitem.iSubItem=iCol;
	lvitem.pszText=(LPTSTR)((LPCTSTR)strData);

	if (iCol == 0)
	{
		m_listResult.InsertItem(&lvitem);
	}
	else
	{
		m_listResult.SetItem(&lvitem);
	}
	/*/
	if (iCol == 0)
	{
		m_listResult.InsertItem(iRow, strData);//@, RGB(0,0,0), crBackground);
	}
	else
	{
		if (crBackground == NULL)
			m_listResult.SetItemText(iRow, iCol, strData);
		else
			m_listResult.SetItemText(iRow, iCol, strData, RGB(0,0,0), crBackground);
	}
}

void CDlgTabICLoss::OnClickLogPath() 
{
	// TODO: Add your control notification handler code here
	/*/
	ITEMIDLIST *pildBrowse;
    TCHAR pszPathname[MAX_PATH];
    BROWSEINFO bInfo;
    memset( &bInfo, 0, sizeof(bInfo));
 
	_stprintf(pszPathname, _T("%s"), m_plnkSystemData->m_strTrayLogPath);

    bInfo.hwndOwner = GetSafeHwnd();
    bInfo.pidlRoot = NULL;
    bInfo.pszDisplayName = pszPathname;
    bInfo.lpszTitle = _T("Select the directory");
    bInfo.ulFlags = BIF_RETURNONLYFSDIRS;

    pildBrowse = ::SHBrowseForFolder(&bInfo); 

    if( pildBrowse != NULL )
    {
        SHGetPathFromIDList(pildBrowse, pszPathname);
//      (LPCTSTR)pszPathname);
        m_plnkSystemData->m_strTrayLogPath.Format(_T("%s"), pszPathname);
		SetWindowText(m_strTitle + _T("[LogPath:") + m_plnkSystemData->m_strTrayLogPath + _T("]"));
		m_plnkSystemData->WriteData();
    }
	/*/
}

void CDlgTabICLoss::OnClickSave() 
{
	// TODO: Add your control notification handler code here
	/*/
	if (m_strResult.IsEmpty())
	{
		MyMessageBox(_T("Log Data가 없습니다."), _T("Confirm"));
		return;
	}
	if (MyMessageBox(_T("Log Data를 저장하겠습니까?"), _T("Question"), M_YESNO|M_ICONQUESTION) == IDNO)
		return;

	CString strPath = m_plnkSystemData->m_strTrayLogPath + _T("\\");
	CString strFName;
	CTime cTime = CTime::GetCurrentTime();
	strFName.Format(_T("%d-%d-%d@%02d%02d%02d.csv"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(),
												cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	strFName = strPath + strFName;
	FILE* fp = _tfopen(strFName, _T("w+"));
	if (fp == NULL) 
		return ;
	_ftprintf(fp, m_strResult);
	fclose(fp);
	/*/
}

void CDlgTabICLoss::OnClickExit() 
{
	// TODO: Add your control notification handler code here
	CDlgTabICLoss::OnOK();
}

void CDlgTabICLoss::OnClickSearchPcblogShift()
{
	// TODO: Add your control notification handler code here
	CString strDate;
	CString strFName;
	CString strQuery;

	int nSYear, nSMonth, nSDay;
	int nEYear, nEMonth, nEDay;
	m_dtStartDay.GetWindowText(strDate);
	_stscanf(strDate, _T("%d-%d-%d"), &nSYear, &nSMonth, &nSDay);
	m_dtEndDay.GetWindowText(strDate);
	_stscanf(strDate, _T("%d-%d-%d"), &nEYear, &nEMonth, &nEDay);

	COleDateTime SDay(nSYear, nSMonth, nSDay, 0, 0, 0);
	COleDateTime EDay(nEYear, nEMonth, nEDay, 0, 0, 0);
	COleDateTimeSpan ElapseDay(1, 0, 0, 0);
	COleDateTime SDayTime, EDayTime;

	int i=0, j=0;
	for (i=0; i<m_iColumnCnt; i++)
		m_listResult.DeleteColumn(0);
	m_listResult.DeleteAllItems();

	m_iColumnCnt = DEF_MAX_COLUMN_TABERR;

	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_listResult.SetExtendedStyle(dwExStyle);

	LV_COLUMN lvcolumn;
	lvcolumn.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt=LVCFMT_LEFT;
	lvcolumn.iSubItem = 0;
	lvcolumn.cchTextMax = 100;

	m_strResult.Empty();
	m_strResult = _T("");

	for (i=0; i<m_iColumnCnt; i++)
	{
//		swprintf(lvcolumn.pszText, _T("%s"), m_pDBTabLoss->GetFieldName(i));
		lvcolumn.pszText = (LPTSTR)((LPCTSTR)m_pDBTabLoss->GetFieldName(i));
//@		if (i==0)
//@			lvcolumn.cx = 90;
//@		else
//@			lvcolumn.cx = 130;
		if (i==0)
			lvcolumn.cx = 100;
		else
			lvcolumn.cx = 65;
		m_listResult.InsertColumn(i, &lvcolumn);
		m_strResult += (m_pDBTabLoss->GetFieldName(i) + _T(","));
	}
	m_strResult += _T("\n");
	int iRowNo = 0;
	
	CString strData;
	int iRowSize = 0, iColSize = 0;	
	int iTotCnt[DEF_MAX_COLUMN_TABERR] = {0, };
	int iDailyTot[DEF_MAX_COLUMN_TABERR] = {0, };
	CString ShiftStart, ShiftEnd;
	COleDateTime ShiftTime;
	for (EDay; EDay>=SDay; EDay -= ElapseDay)
	{
		memset(iDailyTot, 0, sizeof(iDailyTot));

		//GY(22:00 ~ 24:00)
		memset(iTotCnt, 0, sizeof(iTotCnt));
		if (TRUE == m_pDBTabLoss->IsConnected()) m_pDBTabLoss->Disconnect();		
		ShiftTime = EDay - ElapseDay;
		strDate = m_pDBTabLoss->GetFormatDate(ShiftTime);
		strFName.Format(_T("%s\\%s.mdb"), m_pDBTabLoss->GetPath(), strDate);
		strQuery.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False"), strFName);
		if (TRUE == m_pDBTabLoss->IsFileExist(strFName) && TRUE == m_pDBTabLoss->Connect(strQuery))
		{
			ShiftStart.Format(_T("%02d-%02d-22"), ShiftTime.GetMonth(), ShiftTime.GetDay());
			ShiftEnd.Format(_T("%02d-%02d-24"), ShiftTime.GetMonth(), ShiftTime.GetDay());

			strQuery.Format(_T("Select * From %s where %s >= '%s' and %s < '%s'"), 
				m_pDBTabLoss->GetTableName(), m_pDBTabLoss->GetFieldName(0), ShiftStart, m_pDBTabLoss->GetFieldName(0), ShiftEnd);
			m_pDBTabLoss->ExcuteQry(strQuery);
			iRowSize = m_pDBTabLoss->GetRowSize();
	//@		iColSize = m_pDBTabLoss->GetColSize();
			iColSize = m_iColumnCnt;
			if (iRowSize > 0)
			{
				for (i=0; i<iRowSize; i++)
				{
					for (j=1; j<iColSize; j++)
					{
						m_pDBTabLoss->GetData(j, i, strData);
						iTotCnt[j] += _ttoi(strData);
					}
				}
			}
		}

		//GY(00:00 ~ 06:00)
		if (TRUE == m_pDBTabLoss->IsConnected()) m_pDBTabLoss->Disconnect();		
		ShiftTime = EDay;
		strDate = m_pDBTabLoss->GetFormatDate(ShiftTime);
		strFName.Format(_T("%s\\%s.mdb"), m_pDBTabLoss->GetPath(), strDate);
		if (FALSE == m_pDBTabLoss->IsFileExist(strFName))
			continue;
		strQuery.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False"), strFName);
		if (FALSE == m_pDBTabLoss->Connect(strQuery))
			continue;

		ShiftStart.Format(_T("%02d-%02d-00"), ShiftTime.GetMonth(), ShiftTime.GetDay());
		ShiftEnd.Format(_T("%02d-%02d-06"), ShiftTime.GetMonth(), ShiftTime.GetDay());

		strQuery.Format(_T("Select * From %s where %s >= '%s' and %s < '%s'"), 
			m_pDBTabLoss->GetTableName(), m_pDBTabLoss->GetFieldName(0), ShiftStart, m_pDBTabLoss->GetFieldName(0), ShiftEnd);
		m_pDBTabLoss->ExcuteQry(strQuery);
		iRowSize = m_pDBTabLoss->GetRowSize();
//@		iColSize = m_pDBTabLoss->GetColSize();
		iColSize = m_iColumnCnt;
		if (iRowSize > 0)
		{
			for (i=0; i<iRowSize; i++)
			{
				for (j=1; j<iColSize; j++)
				{
					m_pDBTabLoss->GetData(j, i, strData);
					iTotCnt[j] += _ttoi(strData);
				}
			}
		}
		strData.Format(_T("%02d-%02d GY"), ShiftTime.GetMonth(), ShiftTime.GetDay());
		itemDisplay(0, iRowNo, strData);
		for (i=1; i<iColSize; i++)
		{
			strData.Format(_T("%d"), iTotCnt[i]);
			itemDisplay(i, iRowNo, strData);
		}
		iRowNo++;		
	
		for (i=1; i<iColSize; i++)
		{
			iDailyTot[i] += iTotCnt[i];
			iDailyTot[0] += iTotCnt[i];
		}

		//Day(06:00 ~ 14:00)
		memset(iTotCnt, 0, sizeof(iTotCnt));
		if (TRUE == m_pDBTabLoss->IsConnected()) m_pDBTabLoss->Disconnect();		
		ShiftTime = EDay;
		strDate = m_pDBTabLoss->GetFormatDate(ShiftTime);
		strFName.Format(_T("%s\\%s.mdb"), m_pDBTabLoss->GetPath(), strDate);
		if (FALSE == m_pDBTabLoss->IsFileExist(strFName))
			continue;
		strQuery.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False"), strFName);
		if (FALSE == m_pDBTabLoss->Connect(strQuery))
			continue;

		ShiftStart.Format(_T("%02d-%02d-06"), ShiftTime.GetMonth(), ShiftTime.GetDay());
		ShiftEnd.Format(_T("%02d-%02d-14"), ShiftTime.GetMonth(), ShiftTime.GetDay());

		strQuery.Format(_T("Select * From %s where %s >= '%s' and %s < '%s'"), 
			m_pDBTabLoss->GetTableName(), m_pDBTabLoss->GetFieldName(0), ShiftStart, m_pDBTabLoss->GetFieldName(0), ShiftEnd);
		m_pDBTabLoss->ExcuteQry(strQuery);
		iRowSize = m_pDBTabLoss->GetRowSize();
//@		iColSize = m_pDBTabLoss->GetColSize();
		iColSize = m_iColumnCnt;
		if (iRowSize > 0)
		{
			for (i=0; i<iRowSize; i++)
			{
				for (j=1; j<iColSize; j++)
				{
					m_pDBTabLoss->GetData(j, i, strData);
					iTotCnt[j] += _ttoi(strData);
				}
			}
		}
		strData.Format(_T("%02d-%02d Day"), ShiftTime.GetMonth(), ShiftTime.GetDay());
		itemDisplay(0, iRowNo, strData);
		for (i=1; i<iColSize; i++)
		{
			strData.Format(_T("%d"), iTotCnt[i]);
			itemDisplay(i, iRowNo, strData);
		}
		iRowNo++;

		for (i=1; i<iColSize; i++)
		{
			iDailyTot[i] += iTotCnt[i];
			iDailyTot[0] += iTotCnt[i];
		}

		//Swing(14:00 ~ 22:00)
		memset(iTotCnt, 0, sizeof(iTotCnt));
		if (TRUE == m_pDBTabLoss->IsConnected()) m_pDBTabLoss->Disconnect();		
		ShiftTime = EDay;
		strDate = m_pDBTabLoss->GetFormatDate(ShiftTime);
		strFName.Format(_T("%s\\%s.mdb"), m_pDBTabLoss->GetPath(), strDate);
		if (FALSE == m_pDBTabLoss->IsFileExist(strFName))
			continue;
		strQuery.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False"), strFName);
		if (FALSE == m_pDBTabLoss->Connect(strQuery))
			continue;

		ShiftStart.Format(_T("%02d-%02d-14"), ShiftTime.GetMonth(), ShiftTime.GetDay());
		ShiftEnd.Format(_T("%02d-%02d-22"), ShiftTime.GetMonth(), ShiftTime.GetDay());

		strQuery.Format(_T("Select * From %s where %s >= '%s' and %s < '%s'"), 
			m_pDBTabLoss->GetTableName(), m_pDBTabLoss->GetFieldName(0), ShiftStart, m_pDBTabLoss->GetFieldName(0), ShiftEnd);
		m_pDBTabLoss->ExcuteQry(strQuery);
		iRowSize = m_pDBTabLoss->GetRowSize();
//@		iColSize = m_pDBTabLoss->GetColSize();
		iColSize = m_iColumnCnt;
		if (iRowSize > 0)
		{
			for (i=0; i<iRowSize; i++)
			{
				for (j=1; j<iColSize; j++)
				{
					m_pDBTabLoss->GetData(j, i, strData);
					iTotCnt[j] += _ttoi(strData);
				}
			}
		}
		strData.Format(_T("%02d-%02d Swing"), ShiftTime.GetMonth(), ShiftTime.GetDay());
		itemDisplay(0, iRowNo, strData);
		for (i=1; i<iColSize; i++)
		{
			strData.Format(_T("%d"), iTotCnt[i]);
			itemDisplay(i, iRowNo, strData);
		}
		iRowNo++;

		for (i=1; i<iColSize; i++)
		{
			iDailyTot[i] += iTotCnt[i];
			iDailyTot[0] += iTotCnt[i];
		}

		for (i=0; i<iColSize; i++)
		{
			strData.Format(_T("%d"), iDailyTot[i]);
			itemDisplay(i, iRowNo, strData, RGB(128, 128, 0));
		}
		iRowNo++;
		for (i=0; i<iColSize; i++)
		{
			if (i==0)
				strData = _T(" ");
			else
				strData.Format(_T("%.1lf%%"), (iDailyTot[i]*100.0/iDailyTot[0]));
			itemDisplay(i, iRowNo, strData, RGB(128, 128, 0));
		}
		iRowNo++;
	}
}
