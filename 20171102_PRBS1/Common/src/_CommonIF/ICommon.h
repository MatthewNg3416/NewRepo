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

//WriteErrorLog�� SetErrorLevel�� ��ü
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
     * Object ID�� �����Ѵ�. 
	 *
	 * @param	iObjectID : ������ Object ID
     */

    virtual void SetObjectID(int iObjectID);

    /**
     * Object ID�� �д´�. 
	 *
	 * @return	int : Object ID
     */
    virtual int GetObjectID(void);
	
   /**
     * Instance ��ȣ�� �����Ѵ�. 
	 *
	 * @param	iInstanceNo : ������ Instacne ��ȣ
     */
    virtual void SetInstanceNo(int iInstanceNo);

    /**
     * Instance ��ȣ�� �д´�. 
	 *
	 * @return	int : Instacne ��ȣ
     */
    virtual int GetInstanceNo(void);
	
    /**
     * Log file�� log�� ����Ѵ�.
	 *
	 * @param	ucLevel			: �����ϰ��� �ϴ� log level
	 * @param	strLog			: ����ϰ��� �ϴ� log
	 * @param	*pcErrFileName	: log�� ����ϰ��� ȣ���ϴ� �����̸�
	 * @param	iErrLine		: log�� ����ϰ��� ȣ���ϴ� ���� line
	 * @return  Error Code		: 0 = Success, �׿�= Error
     */
	virtual int WriteLog(BYTE ucLevel, CString strLog, char *pcErrFileName, int iErrLine);
	
   /**
     * Log File�� ���õ� attribute�� �����Ѵ�.
     *
	 * @param	iObjectID : ObjectID
     * @param	strFileName : file path �� file name�� ������ string
     * @param	ucLevel : log level ���� bitwise ����
     * @param	iDays : (OPTION=30) ���� set�Ǿ� �ִ� log file ������
     * @return	Error Code : 0 = Success, �� �� = Error
     */
    virtual int SetLogAttribute (int iObjectID, CString strFullFileName, BYTE ucLevel, int iDays = 30);
    /**
     * ������ Log file�� �����Ѵ�.
     *
     * @return	Error Code : 0 = Success, �� �� = Error
     */
    virtual int DeleteOldLogFiles (void);

	/** log manager�� ��ȯ�Ѵ�. */
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
	  * Error Code �����ϱ�
	  *
	  * @param	iErrCode : �߻��� Error Code
	  * @return	Error Code : Object ID (2bytes)�� Error Code + Error Base (2bytes)�� 4bytes�� ������ �ڵ�
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

	//����� ȣȯ���� ���ؼ�...
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


//180104_KDH Test��
//MDebug::SetNowStackTrace();
//MStackTrace stackTrace = MDebug::GetStackTrace();
//for (unsigned int i = 0; i < stackTrace.GetCount(); i++)
//	TRACE(_T("\n String Value : %s \n"), stackTrace.GetString(i));




#endif //ICOMMON_H
