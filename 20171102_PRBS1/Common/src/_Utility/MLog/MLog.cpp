#include "stdafx.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <time.h>

#include "MFileUtil.h"
#include "MLog.h"

#include "atlconv.h"

//#define BOM_UNICODE 0xFEFF

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** @stereotype constructor */
MLog::MLog()
:m_iObjectID(0), m_pLogFile(NULL), m_strLogFilePath("T"), m_strLogFileName("T"), 
m_ucLogLevel(DEF_MLOG_NONE_LOG_LEVEL), m_iLogKeepingDays(DEF_MLOG_DEFAULT_KEEPING_DAYS)
{
	m_pLogView = new SLogViewItem;

	m_pLogView->m_iNumLog = 0;
	m_pLogView->m_iCurrentIndex = 0;
	
	// critical section

	InitializeCriticalSection(&m_csWriteControl);
}

/**
  * Log file�� ���õ� attribute�� �ѹ��� �����Ѵ�.
  *
  * @param	iObjectID		: ���� ������ Component�� ������ Object ID
  * @param	strFullFileName	: file path �� file name�� ������ string
  * @param	ucLevel			: log level ���� bitwise ����
  * @param	iDays			: (OPTION=30) ���� set�Ǿ� �ִ� log file ������
  * @return  Error Code		: 0 = Success, �׿�= Error
  */

MLog::MLog(int iObjectID, CString strFullFileName, BYTE ucLevel, int iDays)
:m_iObjectID(iObjectID), m_pLogFile(NULL), m_ucLogLevel(ucLevel), m_iLogKeepingDays(iDays)
{
	int iFound, iLength;
	CString tempString;
	MFileUtil FileUtil;

	iLength = strFullFileName.GetLength();
	iFound = strFullFileName.ReverseFind('\\');
	if (iFound == -1)
		iFound = strFullFileName.ReverseFind('/');

	m_strLogFilePath = strFullFileName.Left(iFound+1);

	if (iLength-iFound <= 1)
		m_strLogFileName = "";
	else
		m_strLogFileName = strFullFileName.Right(iLength-iFound-1);

	FileUtil.MakeDir(m_strLogFilePath);

	m_pLogView = new SLogViewItem;

	m_pLogView->m_iNumLog = 0;
	m_pLogView->m_iCurrentIndex = 0;
	
	// critical section
	InitializeCriticalSection(&m_csWriteControl);
}

/** @stereotype destructor */
MLog::~MLog()
{
	if (m_pLogView != NULL)
		delete m_pLogView;
}

/**
  * Object ID�� set�Ѵ�.
  *
  * @param	iObjectID		: ���� ������ Component�� ������ Object ID
  * @return  Error Code		: 0 = Success, �׿�= Error
  */
int MLog::SetObjectID(int iObjectID)
{
	m_iObjectID = iObjectID;
	return DEF_MLOG_SUCCESS;
}

/**
  * Object ID�� ��ȯ�Ѵ�.
  *
  * @return  Object ID		: SetObjectID()�� ���Ͽ� set�� Object ID�� ��ȯ
  */
int MLog::GetObjectID(void)
{
	return m_iObjectID;
}


/**
  * Log File Path�� set�Ѵ�.
  * File Path�� �ݵ�� '\\'(backslash)�� �����ؾ� �Ѵ�.
  *
  * @param	strPath			: CString type�� File Path
  * @return  Error Code		: 0 = Success, �׿�= Error
  */
int MLog::SetLogFilePath(CString strPath)
{
	MFileUtil	FileUtil;

	if (strPath.ReverseFind('\\') == strPath.GetLength() - 1)
		m_strLogFilePath = strPath;
	else
		m_strLogFilePath.Format(_T("%s\\"), strPath);

	FileUtil.MakeDir(m_strLogFilePath, FALSE);

	return DEF_MLOG_SUCCESS;
}

/**
  * Log File path�� ��ȯ�Ѵ�.
  *
  * @return  File Path 		: File path�� ��ȯ�Ѵ�.
  */
CString MLog::GetLogFilePath(void)
{
	return m_strLogFilePath;
}

/**
  * Log File �̸��� set�Ѵ�.
  * �ش��̸��� ���� �����̸��� �ٸ���. 
  * �����̸��� ��¥ ������ �����Ѵ�.
  *
  * @param	strName			: CString type�� File �̸�
  * @return  Error Code		: 0 = Success, �׿�= Error
  */
int MLog::SetLogFileName(CString strName)
{
	m_strLogFileName = strName;

	return DEF_MLOG_SUCCESS;
}


/**
  * Log File �̸��� ��ȯ�Ѵ�.
  *
  * @return  File Name 		: File �̸��� ��ȯ�Ѵ�.
  */
CString MLog::GetLogFileName(void)
{
	return m_strLogFileName;
}

/**
  * Log Level�� set�Ѵ�.
  * Log Level�� bitwise�� mask�ȴ�. 
  *
  * @param	ucLevel			: 1 Byte bitwise log mask ������ �����Ѵ�.
  * @return  Error Code		: 0 = Success, �׿�= Error
  */
int MLog::SetLogLevel(BYTE ucLevel)
{
	m_ucLogLevel = ucLevel;
	return DEF_MLOG_SUCCESS;
}

/**
  * Log Level�� get�Ѵ�.
  * Log Level�� bitwise�� �Ǿ��ִ�. 
  *
  * @return  ucLevel		: 1 Byte bitwise log mask ������ �����Ѵ�.
  */
BYTE MLog::GetLogLevel(void)
{
	return m_ucLogLevel;
}

/**
  * Log file�� �����ϰ� �����ϴ� ���ڸ� set�Ѵ�.
  *
  * @param	iDays			: (OPTION=30) log file ���� ��
  * @return  Error Code		: 0 = Success, �׿�= Error
  */
int	MLog::SetLogKeepingDays(int days)
{
	//if (days < 10)
	if (days < 15)
		return ERR_MLOG_TOO_SHORT_KEEPING_DAYS;
	m_iLogKeepingDays = days;

	return DEF_MLOG_SUCCESS;
}

/**
  * Log file�� �����ϰ� �����ϴ� ���ڸ� get�Ѵ�.
  *
  * @return  iDays			: ���� set�Ǿ� �ִ� log file ������
  */
int MLog::GetLogKeepingDays(void)
{
	return m_iLogKeepingDays;
}

/**
  * Log file�� ���õ� attribute�� �ѹ��� �����Ѵ�.
  *
  * @param	iObjectID		: ���� ������ Component�� ������ Object ID
  * @param	strFullFileName	: file path �� file name�� ������ string
  * @param	ucLevel			: log level ���� bitwise ����
  * @param	iDays			: (OPTION=30) ���� set�Ǿ� �ִ� log file ������
  * @return  Error Code		: 0 = Success, �׿�= Error
  */

int MLog::SetLogAttribute(int iObjectID, CString strFullFileName, BYTE ucLevel, int iDays)
{
	int iFound, iLength;
	CString tempString;
	MFileUtil	FileUtil;

	m_iObjectID = iObjectID;

	iLength = strFullFileName.GetLength();
	iFound = strFullFileName.ReverseFind('\\');
	if (iFound == -1)
		iFound = strFullFileName.ReverseFind('/');

	m_strLogFilePath = strFullFileName.Left(iFound+1);

	if (iLength-iFound <= 1)
		m_strLogFileName = "";
	else
		m_strLogFileName = strFullFileName.Right(iLength-iFound-1);

	m_ucLogLevel = ucLevel;

	m_iLogKeepingDays = iDays;

	FileUtil.MakeDir(m_strLogFilePath);

	return DEF_MLOG_SUCCESS;
}


/**
  * Log file�� ���õ� attribute�� �ѹ��� ���´�.
  *
  * @param	*piObjectID			: ���� ������ Component�� ������ Object ID
  * @param	*pstrFullFileName	: file path �� file name�� ������ string
  * @param	*pucLevel			: log level ���� bitwise ����
  * @param	*piDays				: ���� set�Ǿ� �ִ� log file ������
  */
void MLog::GetLogAttribute(int *piObjectID, CString& pstrFullFileName, BYTE *pucLevel, int *piDays)
{
	*piObjectID = m_iObjectID;
	*pucLevel = m_ucLogLevel;
	*piDays = m_iLogKeepingDays;
	pstrFullFileName.Format(_T("%s%s"), m_strLogFilePath, m_strLogFileName);
}

/**
  * Log file�� log�� ����Ѵ�.
  *
  * @param	ucLevel			: �����ϰ��� �ϴ� log level
  * @param	strLog			: ����ϰ��� �ϴ� log
  * @param	*pcErrFileName	: log�� ����ϰ��� ȣ���ϴ� �����̸�
  * @param	iErrLine		: log�� ����ϰ��� ȣ���ϴ� ���� line
  * @return  Error Code		: 0 = Success, �׿�= Error
  */
int MLog::WriteLog(BYTE ucLevel, CString strLog, char *pcErrFileName, int iErrLine)
{
	struct tm *tmNewTime;
	time_t timetClock;
	struct _timeb timebCurrentTime;
	unsigned short usMiliSec;
	char szTime[128];
	WCHAR szFullName[128], szLogFile[128], szLevel[10];
	int iIndex;
	CString strFileNameOnly;
	int	iFound;
	CString	strFileNameResult;

	if ((ucLevel & m_ucLogLevel) == 0x00)
		return DEF_MLOG_SUCCESS;

	// File Name �� ���� ���� Log ���� �ʰ� �Ѿ��.
	if ((m_strLogFileName.GetLength() == 0))
		return DEF_MLOG_SUCCESS;

	if ((ucLevel & m_ucLogLevel) == DEF_MLOG_ERROR_LOG_LEVEL)
	{
		swprintf(szLevel, _T("[1-ERR]"));
	}
	else if ((ucLevel & m_ucLogLevel) == DEF_MLOG_MONITORING_LOG_LEVEL)
	{
		swprintf(szLevel, _T("[2-MNT]"));
	}
	else if ((ucLevel & m_ucLogLevel) == DEF_MLOG_NORMAL_LOG_LEVEL)
	{
		swprintf(szLevel, _T("[3-NOR]"));
	}
	else if ((ucLevel & m_ucLogLevel) == DEF_MLOG_TACT_TIME_LOG_LEVEL)
	{
		swprintf(szLevel, _T("[1-TACT]"));
	}
//170427 JSH.s
	else if ((ucLevel & m_ucLogLevel) == DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL)
	{
		swprintf(szLevel, _T("[5-EQTACT]"));
	}
//170427 JSH.e
	else
		swprintf(szLevel, _T("[UNKNOWN]"));

	// store log item to memory
	// Enter critical section
	EnterCriticalSection((RTL_CRITICAL_SECTION*)&m_csWriteControl);

	// Get time in seconds 
	time(&timetClock);                 
	// Convert time to struct  tm form 
	tmNewTime = localtime(&timetClock); 

	swprintf(szLogFile, _T("%s%04d%02d%02d-%s"), m_strLogFilePath.GetBuffer(), tmNewTime->tm_year+1900, 
		tmNewTime->tm_mon+1, tmNewTime->tm_mday, m_strLogFileName.GetBuffer());
	
	_strtime(szTime);
    _ftime(&timebCurrentTime);	

	usMiliSec = (timebCurrentTime.millitm) / 10;

	USES_CONVERSION;
	WCHAR* pszTime = NULL;
	pszTime = A2W(szTime);

    swprintf(szFullName, _T("[%04d-%02d-%02d %s.%02d] [%04d]"), tmNewTime->tm_year+1900, 
		tmNewTime->tm_mon+1, tmNewTime->tm_mday, pszTime, usMiliSec, m_iObjectID);

	if (m_pLogView->m_iNumLog >= DEF_MLOG_NUM_VIEW_DISPLAY_LOG)
	{
		strFileNameOnly = pcErrFileName;
		iFound = strFileNameOnly.ReverseFind('/');
		if (iFound == -1)
			iFound = strFileNameOnly.ReverseFind('\\');

		strFileNameResult = strFileNameOnly.Mid(iFound+1);

		m_pLogView->m_iCurrentIndex = m_pLogView->m_iCurrentIndex % DEF_MLOG_NUM_VIEW_DISPLAY_LOG;
		for (int i = 0; i < DEF_MLOG_NUM_VIEW_DISPLAY_LOG; i++)
		{
			if (i != m_pLogView->m_iCurrentIndex)
				m_pLogView->m_logItemView[i].m_iMyOrder -= 1;
		}

		iIndex = m_pLogView->m_iCurrentIndex;
		m_pLogView->m_logItemView[iIndex].m_iObjectID	= m_iObjectID;

		m_pLogView->m_logItemView[iIndex].m_strDate.Format(_T("%s.%02d"), pszTime, usMiliSec);

		m_pLogView->m_logItemView[iIndex].m_strFileName	= strFileNameResult; 
		m_pLogView->m_logItemView[iIndex].m_iLineNumber	= iErrLine;
		m_pLogView->m_logItemView[iIndex].m_strLogMsg	= strLog;
		m_pLogView->m_logItemView[iIndex].m_ucLevel		= ucLevel;
		m_pLogView->m_logItemView[iIndex].m_iMyOrder	= DEF_MLOG_NUM_VIEW_DISPLAY_LOG - 1;

		m_pLogView->m_iCurrentIndex++;
	}
	else
	{
		strFileNameOnly = pcErrFileName;
		iFound = strFileNameOnly.ReverseFind('/');
		if (iFound == -1)
			iFound = strFileNameOnly.ReverseFind('\\');

		strFileNameResult = strFileNameOnly.Mid(iFound+1);

		m_pLogView->m_logItemView[m_pLogView->m_iNumLog].m_iMyOrder = m_pLogView->m_iCurrentIndex;

		iIndex = m_pLogView->m_iCurrentIndex;
		m_pLogView->m_logItemView[iIndex].m_iObjectID	= m_iObjectID;

		m_pLogView->m_logItemView[iIndex].m_strDate.Format(_T("%s.%02d"), pszTime, usMiliSec);

		m_pLogView->m_logItemView[iIndex].m_strFileName	= strFileNameResult; 
		m_pLogView->m_logItemView[iIndex].m_iLineNumber	= iErrLine;
		m_pLogView->m_logItemView[iIndex].m_strLogMsg	= strLog;
		m_pLogView->m_logItemView[iIndex].m_ucLevel		= ucLevel;

		m_pLogView->m_iNumLog++;
		m_pLogView->m_iCurrentIndex++;
		if (m_pLogView->m_iCurrentIndex >= DEF_MLOG_NUM_VIEW_DISPLAY_LOG)
			m_pLogView->m_iCurrentIndex = 0;
	}

	//LKH_20090129 : Log Unicode Writing
	CFileFind fn;
	//WCHAR buf[500];
	//size_t strSize;
	int count = 0;
	

	if ((m_pLogFile = _wfopen(szLogFile, _T("ab+,ccs=UNICODE"))) == NULL)
	{
		// Leave critical section
		LeaveCriticalSection((RTL_CRITICAL_SECTION*)&m_csWriteControl);
		return ERR_MLOG_FILE_OPEN_ERROR;
	}

	//���� �߻� �α� ���
//	fwprintf(m_pLogFile, _T("%s %s %s   (%s, %d)\n"), szFullName, szLevel, (LPCTSTR)strLog, (LPCTSTR)strFileNameResult, iErrLine);
	CString csTemp;
	csTemp.Format(_T("%s %s %s   (%s, %d)"), szFullName, szLevel, (LPCTSTR)strLog, (LPCTSTR)strFileNameResult, iErrLine);

	//���� �߻� �α� ���
	fwprintf(m_pLogFile, _T("%s\r\n"), csTemp.GetBuffer());
	/*
	wcscpy(buf,csTemp);
	strSize = wcslen(buf);
	fwrite(buf, sizeof(WCHAR), strSize, m_pLogFile);
	*/

	fclose(m_pLogFile);

	// Leave critical section
	LeaveCriticalSection((RTL_CRITICAL_SECTION*)&m_csWriteControl);
	return DEF_MLOG_SUCCESS;
}

/**
  * ������ Log file�� �����Ѵ�.
  *
  * @return  Error Code		: 0 = Success, �׿�= Error
  */
int MLog::DeleteOldLogFiles(void)
{
	int pDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; 
	struct tm *tmNewTime;
	time_t timetClock;
	int iLeapYear;
	int iDDay, iDMonth, iDYear, iDElapsedDay, iDiffYear, iToday;
	WCHAR szLogFile[256], szDeleteFile[256], szWildCardFileName[256], szToday[20];

	HANDLE			handleFirstFile;
	WIN32_FIND_DATA	finddataFile;

	CString	currentFile;
	
	// File Name�� ���� ���� Log ���� �ʰ� �Ѿ��.
	if ((m_strLogFileName.GetLength() == 0))
		return DEF_MLOG_SUCCESS;

	// Get time in seconds 
	time(&timetClock);                 
	// Convert time to struct  tm form 
	tmNewTime = localtime(&timetClock); 

	iLeapYear = tmNewTime->tm_year + 1900;
	if ((!(iLeapYear % 4) && (iLeapYear % 100)) || !(iLeapYear % 400))
		pDays[1] = 29;

	if (m_strLogFilePath.ReverseFind('\\') == m_strLogFilePath.GetLength() - 1)
	{
		swprintf(szWildCardFileName, _T("%s*%s"), (LPCTSTR)m_strLogFilePath, (LPCTSTR)m_strLogFileName);
		swprintf(szLogFile,_T("%s"), (LPCTSTR)m_strLogFilePath);
	}
	else if (m_strLogFilePath.ReverseFind('/') == m_strLogFilePath.GetLength() - 1)
	{
		swprintf(szWildCardFileName, _T("%s*%s"), (LPCTSTR)m_strLogFilePath, (LPCTSTR)m_strLogFileName);
		swprintf(szLogFile,_T("%s"), (LPCTSTR)m_strLogFilePath);
	}
	else
	{
		swprintf(szWildCardFileName, _T("%s\\*%s"), (LPCTSTR)m_strLogFilePath, (LPCTSTR)m_strLogFileName);
		swprintf(szLogFile,_T("%s\\"), (LPCTSTR)m_strLogFilePath);
	}

	swprintf(szToday, _T("%04d%02d%02d"), tmNewTime->tm_year+1900, tmNewTime->tm_mon+1, tmNewTime->tm_mday);

	//iToday = atoi(szToday);
	iToday = _wtoi(szToday);

	handleFirstFile = FindFirstFile(szWildCardFileName, &finddataFile);
	if (handleFirstFile == INVALID_HANDLE_VALUE)
		return DEF_MLOG_SUCCESS;
	else
	{
		currentFile.Format(finddataFile.cFileName);
		//iDYear = atoi(_T(currentFile.Left(4)));
		//iDMonth = atoi(_T(currentFile.Mid(4,2)));
		//iDDay = atoi(_T(currentFile.Mid(6,2)));
		iDYear = _wtoi((LPCTSTR)currentFile.Left(4));
		iDMonth = _wtoi((LPCTSTR)currentFile.Mid(4,2));
		iDDay = _wtoi((LPCTSTR)currentFile.Mid(6,2));

		iDiffYear = tmNewTime ->tm_year + 1900 - iDYear;

		if (iDiffYear == 0)
		{
			iDElapsedDay = 0;
			for (int i = iDMonth; i < tmNewTime->tm_mon+2 ; i++)
			{
				if (iDMonth == tmNewTime->tm_mon + 1)
				{
					iDElapsedDay = tmNewTime->tm_mday - iDDay;
					break;
				}

				if (i == iDMonth)
					iDElapsedDay += (pDays[iDMonth-1] - iDDay);
				else if (i == tmNewTime->tm_mon+ 1)
					iDElapsedDay += tmNewTime->tm_mday;
				else
					iDElapsedDay += pDays[i-1];
			}

			if (iDElapsedDay > m_iLogKeepingDays)
			{
				swprintf(szDeleteFile, _T("%s%s"), szLogFile, finddataFile.cFileName);
				DeleteFile(szDeleteFile);
			}
		}
		else
		{
			iDElapsedDay = 0;
			for (int i = iDMonth; i < 13 ; i++)
			{
				if (i == iDMonth) 
					iDElapsedDay += (pDays[iDMonth-1] - iDDay);
				else
					iDElapsedDay += pDays[i-1];
			}
			
			for (int i = 1; i < tmNewTime->tm_mon+2 ; i++)
			{
				if (i == tmNewTime->tm_mon + 1) 
					iDElapsedDay += tmNewTime->tm_mday;
				else
					iDElapsedDay += pDays[i];
			}

			iDElapsedDay += (iDiffYear - 1) * 365;
			if (iDElapsedDay > m_iLogKeepingDays)
			{
				swprintf(szDeleteFile, _T("%s%s"), szLogFile, finddataFile.cFileName);
				DeleteFile(szDeleteFile);
			}
		}
	}
	while (FindNextFile(handleFirstFile, &finddataFile))
	{
		currentFile.Format(finddataFile.cFileName);
		//iDYear = atoi(_T(currentFile.Left(4)));
		//iDMonth = atoi(_T(currentFile.Mid(4,2)));
		//iDDay = atoi(_T(currentFile.Mid(6,2)));
		iDYear = _wtoi((LPCTSTR)currentFile.Left(4));
		iDMonth = _wtoi((LPCTSTR)currentFile.Mid(4,2));
		iDDay = _wtoi((LPCTSTR)currentFile.Mid(6,2));

		iDiffYear = tmNewTime ->tm_year + 1900 - iDYear;

		if (iDiffYear == 0)
		{
			iDElapsedDay = 0;
			for (int i = iDMonth; i < tmNewTime->tm_mon+2 ; i++)
			{
				if (iDMonth == tmNewTime->tm_mon + 1)
				{
					iDElapsedDay = tmNewTime->tm_mday - iDDay;
					break;
				}
				
				if (i == iDMonth)
					iDElapsedDay += (pDays[iDMonth-1] - iDDay);
				else if (i == tmNewTime->tm_mon+ 1)
					iDElapsedDay += tmNewTime->tm_mday;
				else
					iDElapsedDay += pDays[i-1];
			}

			if (iDElapsedDay > m_iLogKeepingDays)
			{
				swprintf(szDeleteFile, _T("%s%s"), szLogFile, finddataFile.cFileName);
				DeleteFile(szDeleteFile);
			}
		}
		else
		{
			iDElapsedDay = 0;
			for (int i = iDMonth; i < 13 ; i++)
			{
				if (i == iDMonth) 
					iDElapsedDay += (pDays[iDMonth-1] - iDDay);
				else
					iDElapsedDay += pDays[i-1];
			}
			
			for (int i = 1; i < tmNewTime->tm_mon+2 ; i++)
			{
				if (i == tmNewTime->tm_mon + 1) 
					iDElapsedDay += tmNewTime->tm_mday;
				else
					iDElapsedDay += pDays[i];
			}

			iDElapsedDay += (iDiffYear - 1) * 365;
			if (iDElapsedDay > m_iLogKeepingDays)
			{
				swprintf(szDeleteFile, _T("%s%s"), szLogFile, finddataFile.cFileName);
				DeleteFile(szDeleteFile);
			}
		}
	}
	return DEF_MLOG_SUCCESS;
}

/**
 * Get latest log items stored in memory
 */
SLogViewItem *MLog::GetLogViewItem(void) const
{
	return m_pLogView;
}

/**
 * Get Last Log item	
 */
SLogItem MLog::GetLastLogItem(void) const
{
	if (m_pLogView->m_iCurrentIndex == 0)
		return m_pLogView->m_logItemView[m_pLogView->m_iCurrentIndex];
	else
		return m_pLogView->m_logItemView[m_pLogView->m_iCurrentIndex-1]; 
}

/**
  * ������ ��¥�� ��Ʈ������ �����Ѵ�.
  *
  * @return		
  */
CString MLog::GetCurDate()
{
	CString strResult;
	CString strYear;
	CString strMonth;
	CString strDay;
	CTime t;

	t = t.GetCurrentTime();					// ���� �ð��� ��´�.

	strYear.Format(_T("%d"), t.GetYear());		// �⵵�� ��Ʈ������ �����Ѵ�.
	strMonth.Format(_T("%d"), t.GetMonth());	// ���� ��Ʈ������ �����Ѵ�.
	strDay.Format(_T("%d"), t.GetDay());		// ���� ��Ʈ������ �����Ѵ�.

	if (strYear.GetLength() == 1) strYear = "0" + strYear;
	if (strMonth.GetLength() == 1) strMonth = "0" + strMonth;
	if (strDay.GetLength() == 1) strDay = "0" + strDay;

	strResult = strYear+ "/" + strMonth + "/" + strDay;

	return strResult;
}

/**
  * ������ �ð��� ��Ʈ������ �����Ѵ�.
  *
  * @return		
  */
CString MLog::GetCurTime()
{
	CString strResult;
	CString strHour;
	CString strMinute;
	CString strSecond;
	CTime t = CTime();

	t = t.GetCurrentTime();

	strHour.Format(_T("%d"), t.GetHour());
	strMinute.Format(_T("%d"), t.GetMinute());
	strSecond.Format(_T("%d"), t.GetSecond());
	
	if (strHour.GetLength() == 1) strHour = "0" + strHour;
	if (strMinute.GetLength() == 1) strMinute = "0" + strMinute;
	if (strSecond.GetLength() == 1) strSecond = "0" + strSecond;

	strResult =":" + strHour + ":" + strMinute + ":" + strSecond;

	return strResult;
}

/**
  * ������ ��¥�� �ð��� ��Ʈ������ �����Ѵ�.
  *
  * @return		
  */
CString MLog::GetCurDataAndTime()
{
	return(GetCurDate()+GetCurTime());
}
