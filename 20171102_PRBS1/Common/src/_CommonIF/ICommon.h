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
 * ICommon.h 
 *
 * @version $Revision$
 */

#ifndef ICOMMON_H
#define ICOMMON_H

#include "MLog.h"
#include "MDebug.h"

class ErrorInfo;

//WriteErrorLog를 SetErrorLevel로 대체
//#define WriteErrorLog(strLogMsg)	WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, strLogMsg, __FILE__, __LINE__ )
#define WriteNormalLog(strLogMsg)	WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strLogMsg, __FILE__, __LINE__ )
#define WriteTactTimeLog(strLogMsg)	WriteLog(DEF_MLOG_TACT_TIME_LOG_LEVEL, strLogMsg, __FILE__, __LINE__ )
#define WriteEqTactTimeLog(strLogMsg)	WriteLog(DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL, strLogMsg, __FILE__, __LINE__ ) //170427 JSH

const int ERR_ICOMMON_SUCCESS			= 0;
const int ERR_ICOMMON_NULL_LOG_POINTER	= 1;

typedef	struct tagSCommonAttribute
{
	/** Object ID */
	int		m_iObjectID;
	/** Instance NO */
	int		m_iInstanceNo;
	/** Object Name */
	CString	m_strObjectName;
	/** Log File Name */
	CString	m_strLogFileName;
	/** Log Level */
	BYTE	m_ucLevel;
	/** Log Keeping Days */
	int		m_iLogKeepingDays;
} SCommonAttribute;

/** @interface */
class ICommon
{
protected:
	/** Object ID */
	int		m_iObjectID;

	/** Instance NO */
	int		m_iInstanceNo;

	/** Object Name */
	CString	m_strObjectName;
	
	/** Log File Name */
	CString	m_strLogFileName;

	/** Log Level */
	BYTE	m_ucLevel;

	/** Log Keeping Days */
	int		m_iLogKeepingDays;
	
	/** Log File Name */
	MLog	*m_plogMng;
	
	/** Error Log File name */
	static CString m_strErrorLogFile;

	/** Tact time log file */
	static CString m_strTactTimeLogFile;

//170427 JSH.s
	/** Eq Tact time log file */
	static CString m_strEqTactTimeLogFile;
//170427 JSH.e

public:
	/** Error Log Manager */
	static MLog	*m_pErrorLogMng;

	/** Tact time Log Manager */
	static MLog	*m_pTactTimeLogMng;
//170427 JSH.s
	/** Tact time Log Manager */
	static MLog	*m_pEqTactTimeLogMng;
//170427 JSH.e

public:
	/** @default constructor */
	ICommon();
	
	/** @constructor */
	ICommon(SCommonAttribute commonData);

	/** @destructor */
	virtual ~ICommon();

	/** Set Common Attribute */
	virtual void SetCommonAttribute(SCommonAttribute commonData);

	/** Get Common Attribute */
	virtual void GetCommonAttribute(SCommonAttribute *pCommonData);

	/** GetObject Name */
	virtual CString GetObjectName() { return m_strObjectName; }

    /**
     * Object ID를 설정한다. 
	 *
	 * @param	iObjectID : 설정할 Object ID
     */

    virtual void SetObjectID(int iObjectID);

    /**
     * Object ID를 읽는다. 
	 *
	 * @return	int : Object ID
     */
    virtual int GetObjectID(void);
	
   /**
     * Instance 번호를 설정한다. 
	 *
	 * @param	iInstanceNo : 설정할 Instacne 번호
     */
    virtual void SetInstanceNo(int iInstanceNo);

    /**
     * Instance 번호를 읽는다. 
	 *
	 * @return	int : Instacne 번호
     */
    virtual int GetInstanceNo(void);
	
    /**
     * Log file에 log를 기록한다.
	 *
	 * @param	ucLevel			: 저정하고자 하는 log level
	 * @param	strLog			: 기록하고자 하는 log
	 * @param	*pcErrFileName	: log를 기록하고자 호출하는 파일이름
	 * @param	iErrLine		: log를 기록하고자 호출하는 파일 line
	 * @return  Error Code		: 0 = Success, 그외= Error
     */
	virtual int WriteLog(BYTE ucLevel, CString strLog, char *pcErrFileName, int iErrLine);
	
   /**
     * Log File과 관련된 attribute를 지정한다.
     *
	 * @param	iObjectID : ObjectID
     * @param	strFileName : file path 및 file name을 가지는 string
     * @param	ucLevel : log level 지정 bitwise 정보
     * @param	iDays : (OPTION=30) 현재 set되어 있는 log file 저장일
     * @return	Error Code : 0 = Success, 그 외 = Error
     */
    virtual int SetLogAttribute (int iObjectID, CString strFullFileName, BYTE ucLevel, int iDays = 30);
    /**
     * 오래된 Log file을 삭제한다.
     *
     * @return	Error Code : 0 = Success, 그 외 = Error
     */
    virtual int DeleteOldLogFiles (void);

	/** log manager를 반환한다. */
	virtual MLog *GetLogManager();
	
	/**
	 * Get latest log items stored in memory	
	 */
	SLogViewItem *GetNormalLogViewItem(void) const;

	/**
	 * Get Last Log item	
	 */
	SLogItem GetNormalLastLogItem(void) const;

	/**
	 * Get latest log items stored in memory	
	 */
	static SLogViewItem *GetErrorLogViewItem(void);
	
	/**
	 * Get File Name of the error log	
	 */
	static CString GetErrorLogFileName(void);

	/**
	 * Get Last Log item	
	 */
	static SLogItem GetErrorLastLogItem(void);

	/**
	 * Get latest log items stored in memory	
	 */
	static SLogViewItem *GetTactTimeLogViewItem(void);

	/**
	 * Get File Name of the error log	
	 */
	static CString GetTactTimeLogFileName(void);

	/**
	 * Get Last Log item	
	 */
	static SLogItem GetTactTimeLastLogItem(void);	

//170427 JSH.s
	/**
	 * Get latest log items stored in memory	
	 */
	static SLogViewItem *GetEqTactTimeLogViewItem(void);

	/**
	 * Get File Name of the error log	
	 */
	static CString GetEqTactTimeLogFileName(void);

	/**
	 * Get Last Log item	
	 */
	static SLogItem GetEqTactTimeLastLogItem(void);
//170427 JSH.e
	
//KKY. protected:

	/**
	  * Error Code 생성하기
	  *
	  * @param	iErrCode : 발생한 Error Code
	  * @return	Error Code : Object ID (2bytes)와 Error Code + Error Base (2bytes)를 4bytes로 조합한 코드
	  */
	virtual ErrorInfo generateErrorCode( int iErrorCode );

};


#define NO_ERROR_SUCESS ErrorInfo(0)

class ErrorInfo
{
private:
	ICommon *m_pCommonBased;
	MStackTrace m_StackTrace;
	int m_iErrorCode;
	BOOL m_bErrorStatus;

public:
	ErrorInfo()
	{
		m_iErrorCode = 0;
		m_pCommonBased = NULL;
		m_bErrorStatus = FALSE;
	}
	ErrorInfo(int iErrorCode, ICommon *pCommonBased = NULL)
	{
		m_iErrorCode = iErrorCode;
		if(iErrorCode != 0)
			SetErrorInfo(iErrorCode);
		m_pCommonBased = pCommonBased;
	}

	BOOL operator!= (ErrorInfo errorInfo)
	{
		return m_iErrorCode != errorInfo.GetErrorCode();
	}
	BOOL operator== (ErrorInfo errorInfo)
	{
		return m_iErrorCode == errorInfo.GetErrorCode();
	}

	//여기는 호환성을 위해서...
	BOOL operator!= (int iErrorCode)
	{
		return m_iErrorCode != iErrorCode;
	}
	BOOL operator== (int iErrorCode)
	{
		return m_iErrorCode == iErrorCode;
	}
	//____________________

	void SetErrorInfo(int iErrorCode)
	{
		m_iErrorCode = iErrorCode;
		m_StackTrace.ReadCallStack();
	}
	int GetErrorCode(void) { return m_iErrorCode; }
	MStackTrace GetStackTrace() { return m_StackTrace; }
	BOOL IsError() { return m_bErrorStatus;  }
	//IsError();
	//GetString();
};


//180104_KDH Test중
//MDebug::SetNowStackTrace();
//MStackTrace stackTrace = MDebug::GetStackTrace();
//for (unsigned int i = 0; i < stackTrace.GetCount(); i++)
//	TRACE(_T("\n String Value : %s \n"), stackTrace.GetString(i));




#endif //ICOMMON_H
