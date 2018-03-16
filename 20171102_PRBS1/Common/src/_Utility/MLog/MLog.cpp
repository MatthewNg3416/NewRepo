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
  * Log file과 관련된 attribute를 한번에 지정한다.
  *
  * @param	iObjectID		: 정수 값으로 Component가 가지는 Object ID
  * @param	strFullFileName	: file path 및 file name을 가지는 string
  * @param	ucLevel			: log level 지정 bitwise 정보
  * @param	iDays			: (OPTION=30) 현재 set되어 있는 log file 저장일
  * @return  Error Code		: 0 = Success, 그외= Error
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
  * Object ID를 set한다.
  *
  * @param	iObjectID		: 정수 값으로 Component가 가지는 Object ID
  * @return  Error Code		: 0 = Success, 그외= Error
  */
int MLog::SetObjectID(int iObjectID)
{
	m_iObjectID = iObjectID;
	return DEF_MLOG_SUCCESS;
}

/**
  * Object ID를 반환한다.
  *
  * @return  Object ID		: SetObjectID()를 통하여 set된 Object ID를 반환
  */
int MLog::GetObjectID(void)
{
	return m_iObjectID;
}


/**
  * Log File Path를 set한다.
  * File Path는 반드시 '\\'(backslash)를 포함해야 한다.
  *
  * @param	strPath			: CString type의 File Path
  * @return  Error Code		: 0 = Success, 그외= Error
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
  * Log File path를 반환한다.
  *
  * @return  File Path 		: File path를 반환한다.
  */
CString MLog::GetLogFilePath(void)
{
	return m_strLogFilePath;
}

/**
  * Log File 이름를 set한다.
  * 해당이름은 실제 파일이름과 다르다. 
  * 실제이름은 날짜 정보를 포함한다.
  *
  * @param	strName			: CString type의 File 이름
  * @return  Error Code		: 0 = Success, 그외= Error
  */
int MLog::SetLogFileName(CString strName)
{
	m_strLogFileName = strName;

	return DEF_MLOG_SUCCESS;
}


/**
  * Log File 이름을 반환한다.
  *
  * @return  File Name 		: File 이름을 반환한다.
  */
CString MLog::GetLogFileName(void)
{
	return m_strLogFileName;
}

/**
  * Log Level을 set한다.
  * Log Level은 bitwise로 mask된다. 
  *
  * @param	ucLevel			: 1 Byte bitwise log mask 정보를 포함한다.
  * @return  Error Code		: 0 = Success, 그외= Error
  */
int MLog::SetLogLevel(BYTE ucLevel)
{
	m_ucLogLevel = ucLevel;
	return DEF_MLOG_SUCCESS;
}

/**
  * Log Level을 get한다.
  * Log Level은 bitwise로 되어있다. 
  *
  * @return  ucLevel		: 1 Byte bitwise log mask 정보를 포함한다.
  */
BYTE MLog::GetLogLevel(void)
{
	return m_ucLogLevel;
}

/**
  * Log file을 저장하고 보관하는 일자를 set한다.
  *
  * @param	iDays			: (OPTION=30) log file 저장 일
  * @return  Error Code		: 0 = Success, 그외= Error
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
  * Log file을 저장하고 보관하는 일자를 get한다.
  *
  * @return  iDays			: 현재 set되어 있는 log file 저장일
  */
int MLog::GetLogKeepingDays(void)
{
	return m_iLogKeepingDays;
}

/**
  * Log file과 관련된 attribute를 한번에 지정한다.
  *
  * @param	iObjectID		: 정수 값으로 Component가 가지는 Object ID
  * @param	strFullFileName	: file path 및 file name을 가지는 string
  * @param	ucLevel			: log level 지정 bitwise 정보
  * @param	iDays			: (OPTION=30) 현재 set되어 있는 log file 저장일
  * @return  Error Code		: 0 = Success, 그외= Error
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
  * Log file과 관련된 attribute를 한번에 얻어온다.
  *
  * @param	*piObjectID			: 정수 값으로 Component가 가지는 Object ID
  * @param	*pstrFullFileName	: file path 및 file name을 가지는 string
  * @param	*pucLevel			: log level 지정 bitwise 정보
  * @param	*piDays				: 현재 set되어 있는 log file 저장일
  */
void MLog::GetLogAttribute(int *piObjectID, CString& pstrFullFileName, BYTE *pucLevel, int *piDays)
{
	*piObjectID = m_iObjectID;
	*pucLevel = m_ucLogLevel;
	*piDays = m_iLogKeepingDays;
	pstrFullFileName.Format(_T("%s%s"), m_strLogFilePath, m_strLogFileName);
}

/**
  * Log file에 log를 기록한다.
  *
  * @param	ucLevel			: 저정하고자 하는 log level
  * @param	strLog			: 기록하고자 하는 log
  * @param	*pcErrFileName	: log를 기록하고자 호출하는 파일이름
  * @param	iErrLine		: log를 기록하고자 호출하는 파일 line
  * @return  Error Code		: 0 = Success, 그외= Error
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

	// File Name 이 없는 경우는 Log 하지 않고 넘어간다.
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

	//현재 발생 로그 기록
//	fwprintf(m_pLogFile, _T("%s %s %s   (%s, %d)\n"), szFullName, szLevel, (LPCTSTR)strLog, (LPCTSTR)strFileNameResult, iErrLine);
	CString csTemp;
	csTemp.Format(_T("%s %s %s   (%s, %d)"), szFullName, szLevel, (LPCTSTR)strLog, (LPCTSTR)strFileNameResult, iErrLine);

	//현재 발생 로그 기록
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
  * 오래된 Log file을 삭제한다.
  *
  * @return  Error Code		: 0 = Success, 그외= Error
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
	
	// File Name이 없는 경우는 Log 하지 않고 넘어간다.
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
  * 현재의 날짜를 스트링으로 리턴한다.
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

	t = t.GetCurrentTime();					// 현재 시간을 얻는다.

	strYear.Format(_T("%d"), t.GetYear());		// 년도를 스트링으로 포맷한다.
	strMonth.Format(_T("%d"), t.GetMonth());	// 월을 스트링으로 포맷한다.
	strDay.Format(_T("%d"), t.GetDay());		// 일을 스트링으로 포맷한다.

	if (strYear.GetLength() == 1) strYear = "0" + strYear;
	if (strMonth.GetLength() == 1) strMonth = "0" + strMonth;
	if (strDay.GetLength() == 1) strDay = "0" + strDay;

	strResult = strYear+ "/" + strMonth + "/" + strDay;

	return strResult;
}

/**
  * 현재의 시간을 스트링으로 리턴한다.
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
  * 현재의 날짜와 시간을 스트링으로 리턴한다.
  *
  * @return		
  */
CString MLog::GetCurDataAndTime()
{
	return(GetCurDate()+GetCurTime());
}
