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
    /** ��Ʈ �̸� (COM1 ..)		*/
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
	
	/** Data�� Receive �ϱ� ���� Windowthread */
	CWinThread*			m_cwReceiveThread;

	/** Data�� Send �ϱ� ���� Windowthread */
	CWinThread*			m_cwSendThread;

	/**	Thread�� ���� �Ǵ� 	 */
	BOOL				m_bThreadAlive;

	/**	Thread�� ���� �Ǵ� 	 */
	BOOL				m_bThreadTerminate;

	// handles
	/** Shutdown Event handles	*/
	HANDLE				m_hShutdownEvent;
	/** Comm File Handle	*/
	HANDLE				m_hComm;

	// structures
	/** ��Ʈ ���� I/O�� ���� Receive Overlapped structures	 */
	OVERLAPPED			m_osReceive;

	/** ��Ʈ ���� I/O�� ���� Send Overlapped structures	 */
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

	/** SendThread�� �����Ѵ�.
	 *
	 *	@param	pParam 
	 *	@return	(Success : TRUE)		*/
	static UINT SendEntryPoint(LPVOID pParam);

	/** ReceiveThread�� �����Ѵ�.
	 *
	 *	@param	pParam 
	 *	@return	(Success : TRUE)		*/
	static UINT ReceiveEntryPoint(LPVOID pParam);

	/** Data Send ���� Thread
	 *
	 *	@param	pParam 
	 *	@return	: (Success : DEF_SERIALPORT_SUCCESS) */
	UINT CommSendThread(LPVOID pParam);

	/** Data Receive ���� Thread
	 *
	 *	@param	pParam 
	 *	@return	:(Success : DEF_SERIALPORT_SUCCESS)	*/
	UINT CommReceiveThread(LPVOID pParam);

public:
	/** Open�� Port�� �ݴ´�. */
	void ClosePort();

	/** RS232C Port DCB Set */
	void SetDCB(DCB	pdcb);
	
	/** RS232C Port DCB Get */
	void GetDCB(DCB	*pdcb);

	/** ������ Port�� ����.
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
	
	/** Send & Receive Thread�� �����Ѵ�. 
	 *	@return	:(Success : DEF_SERIALPORT_SUCCESS)	   
	 */	
	int StartThread();

	/** Send & Receive Thread�� Suspend�Ѵ�. (Pause ����) 
	 *
	 *	@return	: (Success : DEF_SERIALPORT_SUCCESS)	   */
	int PauseThread();

	/** Thread�� Resume�Ѵ�. (Restart ����)	 
	 *
	 *	@return : (Success : DEF_SERIALPORT_SUCCESS)
	 */
	int RestartThread();

	/** Receive Queue���� Data�� ��´�.. 
	 *
	 *	@return Receive Queue ũ�� > 0  Data ����, �ƴϸ� NULL
	 */
	int ReceiveData(char *pRecvData);

	int ReceiveData(CString *pRecvData); 


	/** Send Queue�� Data�� �ִ´�.
	 *
	 *	@param	pszMsg : Send Queue�� �� Data
	 *  @return	Success : TRUE, Fail : FALSE
	 */
	int SendData(char *pszMsg);

	/** Receive Queue�� Clear�Ѵ�. */
	void ClearRecvQueue();

	/** ���۵� Data�� ������ üũ�Ѵ�.
	 *
	 *	@param	Command : ���۵� Data����
	 */
	char * CheckSum(char *Command);

	char* E5CNCheckSum(char *Command);

	int GetErrorBase();


	/** Constructor				*/	
	MSerialPort();
	
	/**
	 * Constructor : Set data, set inner object
	 *				 ���޹��� �μ��� Data�� �����ϰ�, ���� ��ü�� �����Ѵ�.
	 *
	 * @param	iObjectID		: Object ID
	 * @param	iErrorBase		: Error Code Start Number
	 * @param	strFullFileName	: Log File Path & File Name
	 * @param	ucLevel			: Log Level (None, Normal, Warning, Error)
	 * @param	iDays			: Log ���� �� ��
	 * @param   datComponent	: SerialPort Component Data
	 * @stereotype constructor
	 */
	MSerialPort(SCommonAttribute commonData, SSerialPortData datComponent);

	/** Destructor				*/
	virtual ~MSerialPort();

};

#endif // !defined(AFX_MSERIALPORT_H__13A6E9E0_79FC_4496_9CCB_48519897708B__INCLUDED_)
