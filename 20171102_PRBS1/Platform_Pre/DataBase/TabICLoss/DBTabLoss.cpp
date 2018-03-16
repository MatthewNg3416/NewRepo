// ProductManager.cpp : implementation file
//

#include "stdafx.h"
#include "DBTabLoss.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBTabLoss
CDBTabLoss::CDBTabLoss(CString strDBFilePath, CString strOrgDBName, CString strTableName, int iKeepingDays)
{
	int i = 0;
	for (i=0; i<DEF_MAX_COLUMN_TABERR; i++)
	{
		m_lCol_Field[i] = i;
		m_iFieldCount[i] = 0;
	};

	m_strFieldName[0] = _T("Time");
	m_strFieldName[1] = _T("TP");
	m_strFieldName[2] = _T("CA1");
	m_strFieldName[3] = _T("CA2");
	m_strFieldName[4] = _T("CA3");
	m_strFieldName[5] = _T("CA4");
	m_strFieldName[6] = _T("CA1D");
	m_strFieldName[7] = _T("CA2D");
	m_strFieldName[8] = _T("CA3D");
	m_strFieldName[9] = _T("CA4D");
	m_strFieldName[10] = _T("TM1");
	m_strFieldName[11] = _T("TM2");
	m_strFieldName[12] = _T("TM3");
	m_strFieldName[13] = _T("TM4");

	m_strDBFilePath = strDBFilePath;
	m_strOrgDBName	= strOrgDBName;
	m_strTableName	= strTableName;

	m_iLogKeepingDays = iKeepingDays;

	m_iCurDay = CTime::GetCurrentTime().GetDay();
	m_iCurTime = CTime::GetCurrentTime().GetHour();
}

CDBTabLoss::~CDBTabLoss()
{
	if (IsConnected())
		Disconnect();
}

BOOL CDBTabLoss::Initialize()
{
	/*/
	CString strCommand;
	strCommand.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False", m_strDBFileName);

	if (!Connect(strCommand)){
		AfxMessageBox("TrayRFIDInfoDB Connect Error");
		return FALSE;
	}
	/*/
//@	m_iCurDay = -1;//CTime::GetCurrentTime().GetDay();
//@	m_bChangedDay = TRUE;

	m_iCurDay = CTime::GetCurrentTime().GetDay();
	m_iCurTime = CTime::GetCurrentTime().GetHour();
	return TRUE;
}

BOOL CDBTabLoss::IsFileExist(CString strFileName)
{
	CFileFind aFile;
	BOOL IsExist = aFile.FindFile(strFileName);
	aFile.Close();
	return IsExist;
}

CString CDBTabLoss::GetPath()
{
	CString strPath = m_strDBFilePath;
	return strPath;
}

CString CDBTabLoss::GetTableName()
{
	return m_strTableName;
}

CString	CDBTabLoss::GetFieldName(int iIndex)
{
	ASSERT(iIndex >= 0 && iIndex < DEF_MAX_COLUMN_TABERR);
	return m_strFieldName[iIndex];
}

BOOL CDBTabLoss::LoadData()
{
	m_csDBAccess.Lock();
	CTime CurTime = CTime::GetCurrentTime();
	CString strCurDate = GetFormatDate(CurTime);
	CString strCurTime;
	strCurTime.Format(_T("%02d-%02d-%02d"), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour());
	CString strFileName = m_strDBFilePath + "\\" + strCurDate + _T(".mdb");
	CString strCommand;
	CString strResult;
	int i = 0;
	if (FALSE == IsFileExist(strFileName))
	{
		for (i=0; i<DEF_MAX_COLUMN_TABERR; i++)
		{
			m_iFieldCount[i] = 0;
		}
//@		CString strSource = m_strDBFilePath + _T("\\") + m_strOrgDBName;
		CString strSource = m_strOrgDBName;
		if (IsConnected())	Disconnect();
		if (FALSE == IsFileExist(strSource))
		{
			m_csDBAccess.Unlock();
			return FALSE;
		}
		if (FALSE == IsFileExist(m_strDBFilePath))
		{
//@			_mkdir((LPCSTR)(LPCTSTR)m_strDBFilePath);
			CreateDirectory(m_strDBFilePath, NULL);
		}

		if (!::CopyFile(strSource, strFileName, FALSE))
		{
			m_csDBAccess.Unlock();
			return FALSE;
		}
		strCommand.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False"), strFileName);
		if (FALSE == Connect(strCommand))
		{
			m_csDBAccess.Unlock();
			return FALSE;
		}
		//@												   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20
		strCommand.Format(_T("insert into %s values ('%s','0','0','0','0','0','0','0','0','0','0','0','0','0')"),
											m_strTableName, strCurTime);
		if (FALSE == InsUpDel(strCommand))
		{
			m_csDBAccess.Unlock();
			return FALSE;
		}
	}
	else
	{
		if (FALSE == IsConnected())
		{
			strCommand.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Persist Security Info=False"), strFileName);
			if (FALSE == Connect(strCommand))
			{
				m_csDBAccess.Unlock();
				return FALSE;
			}
		}
//		strCommand.Format(_T("Select * From %s Where %s = '%s' Order by %s DESC"),
//						GetTableName(), GetFieldName(0), strCurTime, GetFieldName(1));
		strCommand.Format(_T("Select * From %s Where %s = '%s'"),
						GetTableName(), GetFieldName(0), strCurTime);
		if (FALSE == ExcuteQry(strCommand))
		{
			m_csDBAccess.Unlock();
			return FALSE;
		}
		if (GetRowSize() == 0)
		{
			for (i=0; i<DEF_MAX_COLUMN_TABERR; i++)
			{
				m_iFieldCount[i] = 0;
			}
			//@												   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20
			strCommand.Format(_T("insert into %s values ('%s','0','0','0','0','0','0','0','0','0','0','0','0','0')"),
												m_strTableName, strCurTime);
			if (FALSE == InsUpDel(strCommand))
			{
				m_csDBAccess.Unlock();
				return FALSE;
			}
		}
		else
		{
			for (i=1; i<DEF_MAX_COLUMN_TABERR; i++)
			{
				GetData(i, 0, m_iFieldCount[i]);
			}
		}
	}
	m_csDBAccess.Unlock();
	return TRUE;
}

BOOL CDBTabLoss::AppendFieldCount(E_TABVAC_ERR_FIELD eField)
{	
	if (FALSE == LoadData())
		return FALSE;

	m_csDBAccess.Lock();
	CTime CurTime = CTime::GetCurrentTime();
	int iCurDay = CurTime.GetDay();
	int iCurTime = CurTime.GetHour();
	CString strCurDate = GetFormatDate(CurTime);
	CString strCurTime;
	strCurTime.Format(_T("%02d-%02d-%02d"), CurTime.GetMonth(), CurTime.GetDay(), CurTime.GetHour());
	CString strFileName = m_strDBFilePath + "\\" + strCurDate + _T(".mdb");
	CString strCommand;
	CString strResult;
	
	if (m_iCurDay != iCurDay)
	{
		deleteOldFilesInFolder(m_iLogKeepingDays);
	}

	int iType = (int)eField;
	m_iFieldCount[iType]++;
	strCommand.Format(_T("update %s set %s='%d' where %s = '%s'"), 
				m_strTableName, m_strFieldName[iType], m_iFieldCount[iType], m_strFieldName[0], strCurTime);
	if (FALSE == InsUpDel(strCommand))
	{
		m_csDBAccess.Unlock();
		return FALSE;
	}
	m_csDBAccess.Unlock();
	return TRUE;
}

void CDBTabLoss::deleteOldFilesInFolder(int iKeepingDays)
{
	CFileFind	ff;
	int			nt;			// 남은 파일의 갯수
	CString		strFind;
	CTime		curTime = CTime::GetCurrentTime();
	CTime		createTime;

	strFind.Format(_T("%s\\*.*"),m_strDBFilePath);

	if( !ff.FindFile(strFind,0) ) return;
	nt = ff.FindNextFile();
	while(TRUE)
	{
		if(!ff.IsDirectory())
		{
			strFind = ff.GetFilePath();
			ff.GetLastWriteTime(createTime);
			if (curTime - createTime > CTimeSpan(iKeepingDays,0,0,0))
			{
				DeleteFile(strFind);
			}
		}
		if( !nt ) break;
		nt = ff.FindNextFile();
	}
}

CString	CDBTabLoss::GetFormatDateTime(CTime& referTime)
{
	CString sTime;
	sTime.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), 
			referTime.GetYear(), referTime.GetMonth(), referTime.GetDay(), referTime.GetHour(), referTime.GetMinute(), referTime.GetSecond());
	return sTime;
}

CString CDBTabLoss::GetFormatDate(CTime& referTime)
{
	CString sTime;
	sTime.Format(_T("%d-%02d-%02d"), referTime.GetYear(), referTime.GetMonth(), referTime.GetDay());
	return sTime;
}

CString CDBTabLoss::GetFormatDate(COleDateTime& referTime)
{
	CString sTime;
	sTime.Format(_T("%d-%02d-%02d"), referTime.GetYear(), referTime.GetMonth(), referTime.GetDay());
	return sTime;
}

CString CDBTabLoss::GetFormatTime(CTime& referTime)
{
	CString sTime;
	sTime.Format(_T("%02d:%02d:%02d"), referTime.GetHour(), referTime.GetMinute(), referTime.GetSecond());
	return sTime;
}

/*/
void CDBTabLoss::checkDayChanged()
{
	int iCurDay = CTime::GetCurrentTime().GetDay();
	if (m_iCurDay != iCurDay)
	{
		m_bChangedDay = TRUE;
		m_iCurDay = iCurDay;
	}
}
/*/