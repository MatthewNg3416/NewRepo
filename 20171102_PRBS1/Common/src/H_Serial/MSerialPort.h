// MSerialPort.h: interface for the MSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSERIALPORT_H__13A6E9E0_79FC_4496_9CCB_48519897708B__INCLUDED_)
#define AFX_MSERIALPORT_H__13A6E9E0_79FC_4496_9CCB_48519897708B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ISerialPort.h"
#include "DefSerialValue.h"
#include "MGQue.h"

const int ERR_SERIALPORT_SUCCESS = 0;
class MSerialPort : public ISerialPort
{
public:
    /** 포트 이름 (COM1 ..)		*/
    CString m_sPortName;

	/**	PORT NUM  ex) COM1 = 1	*/
	UINT			m_uPortNo;
	
	/**	BaudRate				*/
	UINT			m_uBaud;				// baudrate
	
	/** Parity				    */
	UINT			m_uParity;				// parity 
	
	/**	Databits				*/
	UINT			m_uDatabits;			// databits 

	/** Stopbits				*/
	UINT			m_uStopbits;			// stopbits 

	/** size to WriteBuffer		*/
	UINT			m_uWritebuffersize;		// size to the writebuffer
	
	/** COMM Event				*/
	DWORD			m_dwCommEvents;			// EV_RXCHAR, EV_CTS etc
	
	/** Data를 Receive 하기 위한 Windowthread */
	CWinThread*			m_cwReceiveThread;

	/** Data를 Send 하기 위한 Windowthread */
	CWinThread*			m_cwSendThread;

	/**	Thread의 생사 판단 	 */
	BOOL				m_bThreadAlive;

	/**	Thread의 생사 판단 	 */
	BOOL				m_bThreadTerminate;

	// handles
	/** Shutdown Event handles	*/
	HANDLE				m_hShutdownEvent;
	/** Comm File Handle	*/
	HANDLE				m_hComm;

	// structures
	/** 포트 파일 I/O를 위한 Receive Overlapped structures	 */
	OVERLAPPED			m_osReceive;

	/** 포트 파일 I/O를 위한 Send Overlapped structures	 */
	OVERLAPPED			m_osSend;

	/** COMM Timeout	 */
	COMMTIMEOUTS		m_CommTimeouts;

	/**  Device Control Block   */
	DCB					m_dcb;

	/** CWnd Class */
	CWnd				*pPortOwnerWindow;

	/** ReceiveQueue 	 */
	MGQue<char *>		m_ReceiveQueue;

	/** SendQueue 	 */
	MGQue<char *>		m_SendQueue;

	CRITICAL_SECTION	m_Sync;
	
private:
	SSerialPortData		m_Data;
	char m_szEnd;

protected:

	/** SendThread를 실행한다.
	 *
	 *	@param	pParam 
	 *	@return	(Success : TRUE)		*/
	static UINT SendEntryPoint(LPVOID pParam);

	/** ReceiveThread를 실행한다.
	 *
	 *	@param	pParam 
	 *	@return	(Success : TRUE)		*/
	static UINT ReceiveEntryPoint(LPVOID pParam);

	/** Data Send 위한 Thread
	 *
	 *	@param	pParam 
	 *	@return	: (Success : DEF_SERIALPORT_SUCCESS) */
	UINT CommSendThread(LPVOID pParam);

	/** Data Receive 위한 Thread
	 *
	 *	@param	pParam 
	 *	@return	:(Success : DEF_SERIALPORT_SUCCESS)	*/
	UINT CommReceiveThread(LPVOID pParam);

public:
	/** Open한 Port를 닫는다. */
	void ClosePort();

	/** RS232C Port DCB Set */
	void SetDCB(DCB	pdcb);
	
	/** RS232C Port DCB Get */
	void GetDCB(DCB	*pdcb);

	/** 선택한 Port를 연다.
	 *	@return	: (Success : DEF_SERIALPORT_SUCCESS)				
	 */
	int OpenPort(	CWnd* pPortOwner,
					UINT PortNo, 
					DWORD dwCommEvents, 
					UINT baud, 
					UINT parity, 
					UINT databits, 
					UINT stopsbits, 
					UINT nBufferSize);
	int OpenPort();
	
	/** Send & Receive Thread를 시작한다. 
	 *	@return	:(Success : DEF_SERIALPORT_SUCCESS)	   
	 */	
	int StartThread();

	/** Send & Receive Thread를 Suspend한다. (Pause 개념) 
	 *
	 *	@return	: (Success : DEF_SERIALPORT_SUCCESS)	   */
	int PauseThread();

	/** Thread를 Resume한다. (Restart 개념)	 
	 *
	 *	@return : (Success : DEF_SERIALPORT_SUCCESS)
	 */
	int RestartThread();

	/** Receive Queue에서 Data를 얻는다.. 
	 *
	 *	@return Receive Queue 크기 > 0  Data 내용, 아니면 NULL
	 */
	int ReceiveData(char *pRecvData);

	int ReceiveData(CString *pRecvData); 


	/** Send Queue에 Data를 넣는다.
	 *
	 *	@param	pszMsg : Send Queue에 들어갈 Data
	 *  @return	Success : TRUE, Fail : FALSE
	 */
	int SendData(char *pszMsg);

	/** Receive Queue를 Clear한다. */
	void ClearRecvQueue();

	/** 전송된 Data의 오류를 체크한다.
	 *
	 *	@param	Command : 전송된 Data내용
	 */
	char * CheckSum(char *Command);

	char* E5CNCheckSum(char *Command);

	int GetErrorBase();


	/** Constructor				*/	
	MSerialPort();
	
	/**
	 * Constructor : Set data, set inner object
	 *				 전달받은 인수로 Data를 설정하고, 내부 개체를 설정한다.
	 *
	 * @param	iObjectID		: Object ID
	 * @param	iErrorBase		: Error Code Start Number
	 * @param	strFullFileName	: Log File Path & File Name
	 * @param	ucLevel			: Log Level (None, Normal, Warning, Error)
	 * @param	iDays			: Log 유지 일 수
	 * @param   datComponent	: SerialPort Component Data
	 * @stereotype constructor
	 */
	MSerialPort(SCommonAttribute commonData, SSerialPortData datComponent);

	/** Destructor				*/
	virtual ~MSerialPort();

};

#endif // !defined(AFX_MSERIALPORT_H__13A6E9E0_79FC_4496_9CCB_48519897708B__INCLUDED_)
