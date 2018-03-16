/* samsung */
/* 
 * Common Interface Class Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * ICommon.cpp
 *
 *
 * @author  Arena99 
 * @version $Revision$
 */

#include "StdAfx.h"
#include "ICommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString ICommon::m_strErrorLogFile		= "../Log/Error/Error.log";
CString	ICommon::m_strTactTimeLogFile	= "../Log/TactTime/TactTime.log";
//170427 JSH.s
CString	ICommon::m_strEqTactTimeLogFile	= "../Log/TactTime/EqTactTime.log";
//170427 JSH.e

MLog *ICommon::m_pErrorLogMng = new MLog(0, ICommon::m_strErrorLogFile, DEF_MLOG_ERROR_LOG_LEVEL, 
										DEF_MLOG_DEFAULT_KEEPING_DAYS);

MLog *ICommon::m_pTactTimeLogMng = new MLog(0, ICommon::m_strTactTimeLogFile, DEF_MLOG_TACT_TIME_LOG_LEVEL, 
										DEF_MLOG_DEFAULT_KEEPING_DAYS);

//170427 JSH.s
MLog *ICommon::m_pEqTactTimeLogMng = new MLog(0, ICommon::m_strEqTactTimeLogFile, DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL, 
										DEF_MLOG_DEFAULT_KEEPING_DAYS);
//170427 JSH.e


/** default constructor */
ICommon::ICommon()
:m_iObjectID(0)
{
	m_plogMng = new MLog;
}

/** @stereotype constructor
 * @param	iObjectID : int RegisterObject()에서 반환한 ObjectID
 * @param   listRefComponent :  SAcfToolRefComList, 참조할 Component List 구조체
 * @param   iErrorBase : int, Component의 에러 시작 
 * @param   strFullFileName : CString, 로그 파일 Path와 이름 
 * @param   ucLevel : BYTE, 로그 수준(MLog Class 참조)
 * @param   iDalys : int, 로깅보관 기간, Default 30
 * @param   datComponent; SHeaterControllerData, 콤포넌트의 데이타 구조
 */
ICommon::ICommon(SCommonAttribute commonData)
{
	// Object ID
	m_iObjectID = commonData.m_iObjectID;

	// InstanceNo
	m_iInstanceNo = commonData.m_iInstanceNo;
	
	// Object Name
	m_strObjectName = commonData.m_strObjectName;

	// Log Attribute
	m_strLogFileName = commonData.m_strLogFileName;
	m_ucLevel = commonData.m_ucLevel;
	m_iLogKeepingDays = commonData.m_iLogKeepingDays;

	// Log Manager
	m_plogMng = new MLog;
	m_plogMng->SetLogAttribute(m_iObjectID, commonData.m_strLogFileName, commonData.m_ucLevel,
		commonData.m_iLogKeepingDays);
}

/** @stereotype destructor */
ICommon::~ICommon()
{
	if (m_plogMng != NULL) delete m_plogMng;

	if (m_pTactTimeLogMng!=NULL)
	{
		delete m_pTactTimeLogMng;
		m_pTactTimeLogMng= NULL;
	}
//170427 JSH.s
	if (m_pEqTactTimeLogMng!=NULL)
	{
		delete m_pEqTactTimeLogMng;
		m_pEqTactTimeLogMng= NULL;
	}
//170427 JSH.e

	if (m_pErrorLogMng!=NULL)
	{
		delete m_pErrorLogMng;
		m_pErrorLogMng= NULL;
	}

}

/** Set Common Attribute */
void ICommon::SetCommonAttribute(SCommonAttribute commonData)
{
	// Object ID;
	m_iObjectID = commonData.m_iObjectID;

	// InstanceNo
	m_iInstanceNo = commonData.m_iInstanceNo;

	// Object Name
	m_strObjectName = commonData.m_strObjectName;

	// Log Attribute
	m_strLogFileName = commonData.m_strLogFileName;
	m_ucLevel = commonData.m_ucLevel;
	m_iLogKeepingDays = commonData.m_iLogKeepingDays;

	if (m_plogMng == NULL)	m_plogMng = new MLog;

	m_plogMng->SetLogAttribute(m_iObjectID,
								commonData.m_strLogFileName,
								commonData.m_ucLevel,
								commonData.m_iLogKeepingDays);	
}

/** Get Common Attribute */
void ICommon::GetCommonAttribute(SCommonAttribute *pCommonData)
{
	if (pCommonData == NULL)	return;

	// Object ID
	pCommonData->m_iObjectID = m_iObjectID;

	// InstanceNo
	pCommonData->m_iInstanceNo = m_iInstanceNo;

	// Object Name
	pCommonData->m_strObjectName = m_strObjectName;

	// Log Attribute
	pCommonData->m_strLogFileName = m_strLogFileName;
	pCommonData->m_ucLevel = m_ucLevel;
	pCommonData->m_iLogKeepingDays = m_iLogKeepingDays;
}

/**
  * Object ID를 설정한다. 
  *
  * @param	iObjectID : 설정할 Object ID 값
  */
void ICommon::SetObjectID(int iObjectID)
{
	m_iObjectID = iObjectID;
}

/**
  * Object ID를 읽는다. 
  *
  * @return	int : Object ID 값
  */
int ICommon::GetObjectID(void)
{
	return m_iObjectID;
}

/**
 * Instance 번호를 설정한다. 
 *
 * @param	iInstanceNo : 설정할 Instacne 번호
 */
void ICommon::SetInstanceNo(int iInstanceNo)
{
	m_iInstanceNo = iInstanceNo;
}

/**
 * Instance 번호를 읽는다. 
 *
 * @return	int : Instacne 번호
 */
int ICommon::GetInstanceNo(void)
{
	return m_iInstanceNo;
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
int ICommon::WriteLog(BYTE ucLevel, CString strLog, char *pcErrFileName, int iErrLine)
{
	if (ucLevel == DEF_MLOG_ERROR_LOG_LEVEL)
	{
		if (m_pErrorLogMng != NULL)
		{
			m_pErrorLogMng->SetObjectID(m_iObjectID);
			return m_pErrorLogMng->WriteLog(ucLevel, strLog, pcErrFileName, iErrLine);
		}
		else
			return -1;
	}
	else if (ucLevel == DEF_MLOG_TACT_TIME_LOG_LEVEL)
	{
		if (m_pTactTimeLogMng != NULL)
		{
			m_pTactTimeLogMng->SetObjectID(m_iObjectID);
			return m_pTactTimeLogMng->WriteLog(ucLevel, strLog, pcErrFileName, iErrLine);
		}
		else
			return -1;
	}
//170427 JSH.s
	else if (ucLevel == DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL)
	{
		if (m_pEqTactTimeLogMng != NULL)
		{
			m_pEqTactTimeLogMng->SetObjectID(m_iObjectID);
			return m_pEqTactTimeLogMng->WriteLog(ucLevel, strLog, pcErrFileName, iErrLine);
		}
		else
			return -1;
	}
//170427 JSH.e
	else
	{
		if (m_plogMng != NULL)
			return m_plogMng->WriteLog(ucLevel, strLog, pcErrFileName, iErrLine);
		else
			return -1;
	}
}


/**
  * Component의 Log File 정보를 set한다.
  *
  * @param	iObjectID		: Object ID
  * @param	strFilePath		: log file의 Full Path를 지정한다.
  * @param	ucLogLevel		: bitwise형태의 log level을 설정한다.
  * @param	iDays			: log file을 저장하는 기간
  * @return	Error Code		: 0 = Success
  *							  그외 = Error
  */
int ICommon::SetLogAttribute(int iObjectID, CString strFullFileName, BYTE ucLevel, int iDays)
{
	ASSERT(m_plogMng != NULL);

	m_plogMng->SetLogAttribute(m_iObjectID, strFullFileName, ucLevel, iDays);
	return ERR_ICOMMON_SUCCESS;
}

/**
 * 오래된 Log file을 삭제한다.
 *
 * @return	Error Code : 0 = Success, 그 외 = Error
 */
int ICommon::DeleteOldLogFiles(void)
{
	ASSERT(m_plogMng != NULL);

	m_plogMng->DeleteOldLogFiles();

	if (m_pErrorLogMng != NULL)
		m_pErrorLogMng->DeleteOldLogFiles();

	if (m_pTactTimeLogMng != NULL)
		m_pTactTimeLogMng->DeleteOldLogFiles();
//170427 JSH.s
	if (m_pEqTactTimeLogMng != NULL)
		m_pEqTactTimeLogMng->DeleteOldLogFiles();
//170427 JSH.e
	
	return ERR_ICOMMON_SUCCESS;
}

/** log manager를 반환한다. */
MLog *ICommon::GetLogManager()
{
	return m_plogMng;
}

/**
 * Get latest log items stored in memory	
 */
SLogViewItem *ICommon::GetNormalLogViewItem(void) const
{
	return m_plogMng->GetLogViewItem();
}

/**
 * Get Last Log item	
 */
SLogItem ICommon::GetNormalLastLogItem(void) const
{
	return m_plogMng->GetLastLogItem();
}

/**
 * Get latest log items stored in memory	
 */
SLogViewItem *ICommon::GetErrorLogViewItem(void)
{
	return m_pErrorLogMng->GetLogViewItem();
}

/**
 * Get File Name of the error log	
 */
CString ICommon::GetErrorLogFileName(void)
{
	return m_strErrorLogFile;
}

/**
 * Get Last Log item	
 */
SLogItem ICommon::GetErrorLastLogItem(void)
{
	return m_pErrorLogMng->GetLastLogItem();
}

/**
 * Get latest log items stored in memory	
 */
SLogViewItem *ICommon::GetTactTimeLogViewItem(void)
{
	return m_pTactTimeLogMng->GetLogViewItem();
}

/**
 * Get File Name of the error log	
 */
CString ICommon::GetTactTimeLogFileName(void)
{
	return m_strTactTimeLogFile;
}

/**
 * Get Last Log item	
 */
SLogItem ICommon::GetTactTimeLastLogItem(void)
{
	return m_pTactTimeLogMng->GetLastLogItem();
}

//170427 JSH.s
/**
 * Get File Name of the error log	
 */
CString ICommon::GetEqTactTimeLogFileName(void)
{
	return m_strEqTactTimeLogFile;
}

/**
 * Get Last Log item	
 */
SLogItem ICommon::GetEqTactTimeLastLogItem(void)
{
	return m_pEqTactTimeLogMng->GetLastLogItem();
}

/**
 * Get latest log items stored in memory	
 */
SLogViewItem *ICommon::GetEqTactTimeLogViewItem(void)
{
	return m_pEqTactTimeLogMng->GetLogViewItem();
}

//170427 JSH.e
/**
  * Error Code 생성하기
  *
  * @param	iErrCode : 발생한 Error Code
  * @return	Error Code : Object ID (2bytes)와 Error Code + Error Base (2bytes)를 4bytes로 조합한 코드
  */
ErrorInfo ICommon::generateErrorCode(int iErrCode)
{
	return ErrorInfo(iErrCode, this);
}