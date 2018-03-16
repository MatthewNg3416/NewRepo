// SerialPort.cpp: implementation of the MSerialPort class.
//
//////////////////////////////////////////////////////////////////////
                      
#include "stdafx.h"
#include "MSerialPort.h"
#include "Common.h"

/* jdy sesl 090102
// ML_hongju
#include "atlconv.h"
*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


MSerialPort::MSerialPort(void)
{
	m_uPortNo = 0;
	m_uBaud = 0;
	m_uDatabits = 0;
	m_uParity = 0;
	m_uStopbits = 0;
	m_uWritebuffersize = 0;
	
	m_hComm = NULL;
	pPortOwnerWindow = NULL;
	// initialize overlapped structure members to zero
	m_hShutdownEvent = NULL;

	m_bThreadAlive = FALSE;
	m_dwCommEvents = NULL;
	m_iObjectID = 0;
	m_szEnd = CR;
	m_cwSendThread = NULL;
	m_cwReceiveThread = NULL;

	// initialize overlapped structure members to zero
	memset((void*)&m_osSend, 0x00, sizeof(m_osSend));
	memset((void*)&m_osReceive, 0x00, sizeof(m_osReceive));
	
	InitializeCriticalSection(&m_Sync);

//	m_plogMng	= new MLog();

}

MSerialPort::MSerialPort(SCommonAttribute commonData, SSerialPortData datComponent)
: ICommon(commonData)
{
//	::ICommon(commonData);
	m_sPortName.Format(_T("COM%d"),datComponent.m_uPortNo);
//	m_sPortName.Format("COM%d",datComponent.m_uPortNo);
	m_uPortNo = datComponent.m_uPortNo;
	m_uBaud = datComponent.m_uBaud;
	m_uDatabits = datComponent.m_uDatabits;
	m_uParity = datComponent.m_uParity;
	m_uStopbits = datComponent.m_uStopbits;
	m_uWritebuffersize = datComponent.m_uWritebuffersize;		
	m_szEnd = datComponent.m_szEnd;

	m_hComm = NULL;
	pPortOwnerWindow = NULL;
	// initialize overlapped structure members to zero
	m_hShutdownEvent = NULL;

	m_bThreadAlive = FALSE;
	m_dwCommEvents = NULL;
	m_iObjectID = 0;
	
	m_cwSendThread = NULL;
	m_cwReceiveThread = NULL;

	// initialize overlapped structure members to zero
	memset((void*)&m_osSend, 0x00, sizeof(m_osSend));
	memset((void*)&m_osReceive, 0x00, sizeof(m_osReceive));


	InitializeCriticalSection(&m_Sync);

	OpenPort(NULL, m_uPortNo, EV_BREAK | EV_ERR | EV_RXCHAR | EV_TXEMPTY, m_uBaud,
			m_uParity, m_uDatabits, m_uStopbits, m_uWritebuffersize);
}

MSerialPort::~MSerialPort()
{
	char	*RecvBuf;
	ClosePort();

	if (m_cwSendThread  != NULL)
		m_cwSendThread->SuspendThread(); 
	
	if (m_cwReceiveThread  != NULL)
		m_cwReceiveThread->SuspendThread(); 

	Sleep(100);
	
	/** Send/Receive QUEUE에 있는 쓰레기를 치우자.....
	 */
	while (TRUE)
	{
		if (!m_SendQueue.IsEmpty())
		{
			if (m_SendQueue.Get(&RecvBuf))
				free(RecvBuf);
		}
		else
			break;
		Sleep(10);
	}

	while (TRUE)
	{
		if (!m_ReceiveQueue.IsEmpty())
		{
			if (m_ReceiveQueue.Get(&RecvBuf))
				free(RecvBuf);
		}
		else
			break;

		Sleep(10);
	}

	if (m_cwReceiveThread  != NULL)
		m_cwReceiveThread->ExitInstance();
	
	if (m_cwSendThread  != NULL)
		m_cwSendThread->ExitInstance();

	if (m_cwReceiveThread  != NULL)
		delete m_cwReceiveThread;
	if (m_cwSendThread  != NULL)
		delete m_cwSendThread;

	TRACE(_T("SerialPort Thread ended\n"));

}


int MSerialPort::OpenPort(CWnd* pPortOwner,
					      UINT PortNo, 
					      DWORD dwCommEvents, 
				          UINT baud, 
					      UINT parity, 
					      UINT databits, 
					      UINT stopsbits, 
					      UINT nBufferSize)
{
	CString LogString;
	char szBuff[250];
	
	EnterCriticalSection(&m_Sync);

	BOOL bResult = FALSE;
	char szPort[50];
	char szBaud[50];

	m_osSend.Offset = 0 ;
	m_osSend.OffsetHigh = 0 ;
	
	m_osReceive.Offset = 0 ;
	m_osReceive.OffsetHigh = 0 ;

	memset(szPort, 0x00, sizeof(szPort));
	memset(szBaud, 0x00, sizeof(szBaud));
	
	// Port 번호의 유효성을 체크한다...
	ASSERT(m_uPortNo > 0 && m_uPortNo < MAX_COM_PORT_NO);
	pPortOwnerWindow = NULL;

	// if the thread is alive: Kill
	if (m_bThreadAlive)
	{
		PauseThread();
		printf("Thread ended \n");
	}

	// Read Event 생성
	if (m_osReceive.hEvent != NULL)
		ResetEvent(m_osReceive.hEvent);
	m_osReceive.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	// Write Event 생성
	if (m_osSend.hEvent != NULL)
		ResetEvent(m_osSend.hEvent);
	m_osSend.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Shutdown Event 생성
	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	m_dwCommEvents = EV_BREAK | EV_ERR | EV_RXCHAR | EV_TXEMPTY;

	FillMemory(&m_dcb, sizeof(m_dcb), 0);
	m_dcb.DCBlength = sizeof(m_dcb);


	// Port가 전에 열렸으면 닫는다.
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings
	sprintf(szPort, "COM%d", m_uPortNo);
	sprintf(szBaud, "baud=%d parity=%d data=%d stop=%d", m_uBaud, m_uParity, m_uDatabits, m_uStopbits);
	//sprintf(szBaud, "%d,%c,%d,%d", m_uBaud, m_szParity, m_uDatabits, m_uStopbits);

	sprintf(szBuff, "%s, %s", szPort, szBaud);

	/* jdy sesl 090102
	USES_CONVERSION;
	WCHAR* pszAnsi1 = NULL;
	pszAnsi1 = A2W(szPort);

	WCHAR* pszBaud = NULL;
	pszBaud = A2W(szBaud);
	*/
	CString strPort(szPort);
		
	// COMM device를 파일 형식으로 연결하여, ComPort의 Handle을 반환한다... 
//	m_hComm = CreateFile(szPort,								// 파일명, 포트명 (COMxx)
	// jdy sesl 090102	m_hComm = CreateFile(pszAnsi1,								// 파일명, 포트명 (COMxx)
	m_hComm = CreateFile(strPort.GetBuffer(strPort.GetLength()),// 파일명, 포트명 (COMxx)
					     GENERIC_READ | GENERIC_WRITE,			// 접근모드 읽기쓰기인가 읽기 전용인가
					     FILE_SHARE_READ | FILE_SHARE_WRITE ,   // 다른 프로그램과 공유를 할것인가 아닌가.
						 NULL,									// 보안에 관한 속성(현재 개방된 파일이 다른 사람들에게 오픈되지 못하게 잠그는 속성)
					     OPEN_ALWAYS,							// 어떻게 열것인가(새로 만들것인가, 기존에 있는 파일을 열것인가[OPEN_EXISTING])
					     FILE_FLAG_OVERLAPPED,					// 파일 속성(보통 파일이면서 overlapped 되게 한다.)
						 0);									// 템플릿 파일 핸들러

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		CString LogString;
		LogString.Format(_T("CreateFile Error : %s, %s"),szPort, szBaud);
		//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
		

		LeaveCriticalSection(&m_Sync);	
		if (MyMessageBox(szBuff, "RS232C Port Open Fail" , M_ICONERROR|M_YESNO) == IDNO) 
		{
			SetErrorLevel(_T("SERIALPORT open Port"),3, __FILE__, __LINE__);
			return generateErrorCode(108004);	// ERR_SERIALPORT_OPEN_FAIL
		}
	}


	// Communication Device를 위한 Timeout Parameter 설정
	m_CommTimeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	m_CommTimeouts.ReadTotalTimeoutConstant = 0;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / m_uBaud;
	m_CommTimeouts.WriteTotalTimeoutConstant = 0;
	SetupComm(m_hComm, m_uWritebuffersize, m_uWritebuffersize);
	

	// Communication Device가 read/write 를 수행하는데 필요한 Timeout Parameter Set
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{				

		/*
		** 
		** 컴포트에서 데이터를 교환하는 방법을 char 단위를 기본으로 설정 하자. 
		** MASK 설정 : a set of events to be monitored for a communications device.
		** EV_RXCHAR	: 문자가 수신되었음
		** EV_BREAK		: 입력시에 갑자기 중단이 되었음
		** EV_CTS		: CTS 신호가 변경되었음
		** EV_DSR		: DSR 신호가 변경되었음
		** EV_ERR		: 에러가 감지 되었음
		** EV_RING		: 전화벨이 울렸음
		** EV_RXFLAG	: DCB구조체의 EvtChar 의 문자가 수신되어 입력버퍼에 저장되었음
		** EV_TXEMPTY	: 출력 버퍼로 마지막 문자가 전송되었음
		*/
	
		if (SetCommMask(m_hComm, m_dwCommEvents))
		{

			// Communication Device의 Device Control Block의 현재 설정치를 가져온다...
			if (GetCommState(m_hComm, &m_dcb))
			{
				m_dcb.fBinary = 1;
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// RTS flow control bit high Setting!
				
				// jdy sesl 090102
				CString strBaud(szBaud);

//				if (BuildCommDCB(szBaud, &m_dcb))
				// jdy sesl 090102	if (BuildCommDCB(pszBaud, &m_dcb))
				if (BuildCommDCB(strBaud.GetBuffer(strBaud.GetLength()), &m_dcb))
				{
					// 변경된 Device Control Block 설정
					if (SetCommState(m_hComm, &m_dcb))
						; // normal operation... continue
					else
					{
						CString LogString;
						LogString.Format(_T("SetCommState Error[%d]"), GetLastError());
						//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);	
					}
				}
				else
				{
				//	CString LogString;
				//	LogString.Format("BuildCommDCB Error");
				//	m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
					m_dcb.fBinary = 1;
					m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// RTS flow control bit high Setting!
					m_dcb.BaudRate = m_uBaud;
					m_dcb.ByteSize = m_uDatabits;
					m_dcb.Parity = m_uParity;
					m_dcb.StopBits = m_uStopbits;
					m_dcb.fOutxCtsFlow = CTS_DONE;
					m_dcb.fOutxDsrFlow = DSR_DONE;
					m_dcb.fInX = m_dcb.fOutX = 1;		// Xon, Xoff 사용.
					m_dcb.XonChar = ASCII_XON;
					m_dcb.XoffChar = ASCII_XOFF;
					m_dcb.XonLim = 100;
					m_dcb.XoffLim = 100;
					if (! SetCommState(m_hComm, &m_dcb))	
					{
						CString LogString;
						LogString.Format(_T("SetCommState Error[%d]"), GetLastError());
						//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);	

					}

				}		
			}
			else
			{
					CString LogString;
					LogString.Format(_T("GetCommState Error [%d]"), GetLastError());
					//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
			}
		}
		else
		{
			CString LogString;
			LogString.Format(_T("SetCommTimeouts Error"));
			//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
		}
	}
	else
	{
			CString LogString;
			LogString.Format(_T("SetCommMask Error"));
			//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
	}

	//SetupComm(m_hComm, 4096, 4096);

	//디바이스에 쓰레기가 있을지 모르니까 깨끗이 청소를 하자. 
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	printf("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", m_uPortNo);

	LeaveCriticalSection(&m_Sync);
	
	Sleep(500);

	StartThread();


	LogString.Format(_T("COM Port Open Success & ComThread Start Success"));
	//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
	return ERR_SERIALPORT_SUCCESS;

}

int MSerialPort::OpenPort()
{
	CString LogString;
	char szBuff[250];
	
	EnterCriticalSection(&m_Sync);

	BOOL bResult = FALSE;
	char szPort[50];
	char szBaud[50];

	m_osSend.Offset = 0 ;
	m_osSend.OffsetHigh = 0 ;
	
	m_osReceive.Offset = 0 ;
	m_osReceive.OffsetHigh = 0 ;

	memset(szPort, 0x00, sizeof(szPort));
	memset(szBaud, 0x00, sizeof(szBaud));
	
	// Port 번호의 유효성을 체크한다...
	ASSERT(m_uPortNo > 0 && m_uPortNo < MAX_COM_PORT_NO);
	pPortOwnerWindow = NULL;

	// if the thread is alive: Kill
	if (m_bThreadAlive)
	{
		PauseThread();
		printf("Thread ended \n");
	}

	// Read Event 생성
	if (m_osReceive.hEvent != NULL)
		ResetEvent(m_osReceive.hEvent);
	m_osReceive.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	// Write Event 생성
	if (m_osSend.hEvent != NULL)
		ResetEvent(m_osSend.hEvent);
	m_osSend.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Shutdown Event 생성
	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	m_dwCommEvents = EV_BREAK | EV_ERR | EV_RXCHAR | EV_TXEMPTY;

	FillMemory(&m_dcb, sizeof(m_dcb), 0);
	m_dcb.DCBlength = sizeof(m_dcb);


	// Port가 전에 열렸으면 닫는다.
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings
	sprintf(szPort, "COM%d", m_uPortNo);
	sprintf(szBaud, "baud=%d parity=%d data=%d stop=%d", m_uBaud, m_uParity, m_uDatabits, m_uStopbits);
	//sprintf(szBaud, "%d,%c,%d,%d", m_uBaud, m_szParity, m_uDatabits, m_uStopbits);

	sprintf(szBuff, "%s, %s", szPort, szBaud);

	/* jdy sesl 090102
	USES_CONVERSION;
	WCHAR* pszAnsi1 = NULL;
	pszAnsi1 = A2W(szPort);

	WCHAR* pszBaud = NULL;
	pszBaud = A2W(szBaud);
	*/
	CString strPort(szPort);

	// COMM device를 파일 형식으로 연결하여, ComPort의 Handle을 반환한다... 
//	m_hComm = CreateFile(szPort,								// 파일명, 포트명 (COMxx)
	// jdy sesl 090102	m_hComm = CreateFile(pszAnsi1,								// 파일명, 포트명 (COMxx)
	m_hComm = CreateFile(strPort.GetBuffer(strPort.GetLength()),// 파일명, 포트명 (COMxx)
					     GENERIC_READ | GENERIC_WRITE,			// 접근모드 읽기쓰기인가 읽기 전용인가
					     FILE_SHARE_READ | FILE_SHARE_WRITE ,   // 다른 프로그램과 공유를 할것인가 아닌가.
						 NULL,									// 보안에 관한 속성(현재 개방된 파일이 다른 사람들에게 오픈되지 못하게 잠그는 속성)
					     OPEN_ALWAYS,							// 어떻게 열것인가(새로 만들것인가, 기존에 있는 파일을 열것인가[OPEN_EXISTING])
					     FILE_FLAG_OVERLAPPED,					// 파일 속성(보통 파일이면서 overlapped 되게 한다.)
						 0);									// 템플릿 파일 핸들러

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		CString LogString;
		LogString.Format(_T("CreateFile Error : %s, %s"),szPort, szBaud);
		//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
		

		LeaveCriticalSection(&m_Sync);	
		if (MyMessageBox(szBuff, "RS232C Port Open Fail" , M_ICONERROR|M_YESNO) == IDNO) 
		{
			SetErrorLevel(_T("SERIALPORT open Port"),3, __FILE__, __LINE__);
			return generateErrorCode(108004);	// ERR_SERIALPORT_OPEN_FAIL
		}
	}


	// Communication Device를 위한 Timeout Parameter 설정
	m_CommTimeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	m_CommTimeouts.ReadTotalTimeoutConstant = 0;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / m_uBaud;
	m_CommTimeouts.WriteTotalTimeoutConstant = 0;
	SetupComm(m_hComm, m_uWritebuffersize, m_uWritebuffersize);
	

	// Communication Device가 read/write 를 수행하는데 필요한 Timeout Parameter Set
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{				

		/*
		** 
		** 컴포트에서 데이터를 교환하는 방법을 char 단위를 기본으로 설정 하자. 
		** MASK 설정 : a set of events to be monitored for a communications device.
		** EV_RXCHAR	: 문자가 수신되었음
		** EV_BREAK		: 입력시에 갑자기 중단이 되었음
		** EV_CTS		: CTS 신호가 변경되었음
		** EV_DSR		: DSR 신호가 변경되었음
		** EV_ERR		: 에러가 감지 되었음
		** EV_RING		: 전화벨이 울렸음
		** EV_RXFLAG	: DCB구조체의 EvtChar 의 문자가 수신되어 입력버퍼에 저장되었음
		** EV_TXEMPTY	: 출력 버퍼로 마지막 문자가 전송되었음
		*/

		if (SetCommMask(m_hComm, m_dwCommEvents))
		{

			// Communication Device의 Device Control Block의 현재 설정치를 가져온다...
			if (GetCommState(m_hComm, &m_dcb))
			{
				m_dcb.fBinary = 1;
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// RTS flow control bit high Setting!
				
				// jdy sesl 090102
				CString strBaud(szBaud);

//				if (BuildCommDCB(szBaud, &m_dcb))
				// jdy sesl 090102	if (BuildCommDCB(pszBaud, &m_dcb))
				if (BuildCommDCB(strBaud.GetBuffer(strBaud.GetLength()), &m_dcb))
				{
					// 변경된 Device Control Block 설정
					if (SetCommState(m_hComm, &m_dcb))
						; // normal operation... continue
					else
					{
						CString LogString;
						LogString.Format(_T("SetCommState Error[%d]"), GetLastError());
						//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);	
					}
				}
				else
				{
				//	CString LogString;
				//	LogString.Format("BuildCommDCB Error");
				//	m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
					m_dcb.fBinary = 1;
					m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// RTS flow control bit high Setting!
					m_dcb.BaudRate = m_uBaud;
					m_dcb.ByteSize = m_uDatabits;
					m_dcb.Parity = m_uParity;
					m_dcb.StopBits = m_uStopbits;
					m_dcb.fOutxCtsFlow = CTS_DONE;
					m_dcb.fOutxDsrFlow = DSR_DONE;
					m_dcb.fInX = m_dcb.fOutX = 1;		// Xon, Xoff 사용.
					m_dcb.XonChar = ASCII_XON;
					m_dcb.XoffChar = ASCII_XOFF;
					m_dcb.XonLim = 100;
					m_dcb.XoffLim = 100;
					if (! SetCommState(m_hComm, &m_dcb))	
					{
						CString LogString;
						LogString.Format(_T("SetCommState Error[%d]"), GetLastError());
						//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);	

					}

				}		
			}
			else
			{
					CString LogString;
					LogString.Format(_T("GetCommState Error [%d]"), GetLastError());
					//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
			}
		}
		else
		{
			CString LogString;
			LogString.Format(_T("SetCommTimeouts Error"));
			//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
		}
	}
	else
	{
			CString LogString;
			LogString.Format(_T("SetCommMask Error"));
			//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
	}

	//SetupComm(m_hComm, 4096, 4096);

	//디바이스에 쓰레기가 있을지 모르니까 깨끗이 청소를 하자. 
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	printf("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", m_uPortNo);

	LeaveCriticalSection(&m_Sync);
	
	Sleep(500);

	StartThread();


	LogString.Format(_T("COM Port Open Success & ComThread Start Success"));
	//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
	return ERR_SERIALPORT_SUCCESS;

}


UINT MSerialPort::ReceiveEntryPoint(LPVOID pParam)
{
	MSerialPort *pSelf = (MSerialPort*)pParam;
	
	return pSelf->CommReceiveThread(pParam);
}

UINT MSerialPort::SendEntryPoint(LPVOID pParam)
{
	MSerialPort *pSelf = (MSerialPort*)pParam;
	
	return pSelf->CommSendThread(pParam);
}


UINT MSerialPort::CommSendThread(LPVOID pParam)
{

	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	BOOL  bResult = TRUE;

	BOOL	bRead = TRUE; 
	BOOL bWrite = TRUE;
	DWORD BytesSent = 0;
	unsigned int  i= 0;

	char	SendBuffer[255];
	char	*data;

	char szSendMessage[255];


	memset(SendBuffer, 0x00, sizeof(SendBuffer));

	for (;;)
	{
		//디바이스에 쓰레기가 있을지 모르니까 깨끗이 청소를 하자. 
		// Clears the output buffer (if the device driver has one).
		PurgeComm(m_hComm, PURGE_TXCLEAR | PURGE_TXABORT);

		for (;;)
		{
	
			if (m_SendQueue.Get(&data))
			{
				memcpy(SendBuffer,data, sizeof(SendBuffer));
				free(data);
				break;
			}
			Sleep(10);
		}
	
		switch (m_szEnd)
		{
		case CR:
			sprintf(szSendMessage, "%s%c", SendBuffer, CR);
			break;
		case LF:
			sprintf(szSendMessage, "%s%c%c", SendBuffer, CR, LF);
			break;
//		case ETX:
//			sprintf(szSendMessage, "%s%c", SendBuffer,ETX);
//			break;
		default:
			sprintf(szSendMessage, "%s", SendBuffer);
			break;
		}
		char strLog[100];
		sprintf(strLog, "%s%s", "Send Msg = ",szSendMessage);
		//WriteNormalLog(CString(strLog));

		bResult = WriteFile(m_hComm,							// Handle to COMM Port
							(LPCVOID)szSendMessage,				// Pointer to message buffer in calling finction
							(DWORD)strlen(szSendMessage),		// Length of message to send
							&BytesSent,							// Where to store the number of bytes sent
							&m_osSend);							// Overlapped structure

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
			case ERROR_IO_PENDING:
				{
					while (!GetOverlappedResult(m_hComm, &m_osSend, &BytesSent, TRUE))
					{
						dwError = GetLastError();
						if (dwError != ERROR_IO_INCOMPLETE)
							break;
					}
					break;
				}
			case ERROR_INVALID_PARAMETER :
				{
					CString LogString;
					LogString.Format(_T("Invalid Parameter Error"));
					//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);				
				}

				break;

			default:
				{
					CString LogString;
					LogString.Format(_T("WriteFile Error"));
					//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);	
				}
			}


		} // end if


		// Verify that the data size send equals what we tried to send
		if (BytesSent != (strlen(SendBuffer)+1))
		{
			CString LogString;
			LogString.Format(_T("Send >> %s \n"), SendBuffer);
//			m_plogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, LogString, NULL, NULL);
		}
		else
		{
			CString LogString;
//			LogString.Format("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d \n", BytesSent, strlen((char*)SendBuffer));
//			m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
		}

		Sleep(10);
	}

	AfxEndThread(0);

	if (::WaitForSingleObject(m_cwSendThread->m_hThread, 1000))		//INFINITE))
	{
		AfxMessageBox(_T("Thread 소멸 실패"));
	}

	return ERR_SERIALPORT_SUCCESS;
}

UINT MSerialPort::CommReceiveThread(LPVOID pParam)
{
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;

	BOOL	bRead = TRUE; 
	DWORD	BytesRead = 0;
	char	RXBuff[255];
	unsigned int  i= 0;
	
	char ReceiveBuffer[250];
	char *SendBuffer;
	SendBuffer = NULL;
	char szTemp[250];
	int itemp = 0;
	


	memset(szTemp, 0x00, sizeof(szTemp));

	// Set the status variable in the dialog class to TRUE to indicate the thread is running.
	m_bThreadAlive = TRUE;	
		
	// 통신을 시작하기 위해 통신 Port를 청소한다...
	if (m_hComm)		// check if the port is opened
	{
		// Clears the input buffer (if the device driver has one). 
		PurgeComm(m_hComm, PURGE_RXCLEAR  | PURGE_RXABORT);
	}

	Sleep(100);
	// begin forever loop.  This loop will run as long as the thread is alive.
	for (;;) 
	{ 
		// 문자가 들어오기를 기다립니다. /**/
		// Clears the input buffer (if the device driver has one). 
		//PurgeComm(m_hComm, PURGE_RXCLEAR  | PURGE_RXABORT);

		bResult = WaitCommEvent(m_hComm, &Event, NULL);
		
		GetCommMask(m_hComm, &CommEvent);

		if (pPortOwnerWindow != NULL)
		{
			if (CommEvent & EV_CTS)
			{
				CString LogString;
				LogString.Format(_T("COMM CTS Detected"));
				//m_plogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, LogString, NULL, NULL);		

				if (pPortOwnerWindow->m_hWnd != NULL)
					::SendMessage(pPortOwnerWindow->m_hWnd, WM_COMM_CTS_DETECTED, (WPARAM) 0, (LPARAM)m_uPortNo);
			}
			if (CommEvent & EV_RXFLAG)
			{
				CString LogString;
				LogString.Format(_T("COMM RXFLAG Detected"));
				//m_plogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, LogString, NULL, NULL);	
				
				if (pPortOwnerWindow->m_hWnd != NULL)		
					::SendMessage(pPortOwnerWindow->m_hWnd, WM_COMM_RXFLAG_DETECTED, (WPARAM) 0, (LPARAM)m_uPortNo);
			}
			if (CommEvent & EV_BREAK)
			{
				CString LogString;
				LogString.Format(_T("COMM BREAK Detected"));
				//m_plogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, LogString, NULL, NULL);
				if (pPortOwnerWindow->m_hWnd != NULL)
					::SendMessage(pPortOwnerWindow->m_hWnd, WM_COMM_BREAK_DETECTED, (WPARAM) 0, (LPARAM)m_uPortNo);
			}
			if (CommEvent & EV_ERR)
			{
				CString LogString;
				LogString.Format(_T("COMM ERR Detected"));
				//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);
				if (pPortOwnerWindow->m_hWnd != NULL)
					::SendMessage(pPortOwnerWindow->m_hWnd, WM_COMM_ERR_DETECTED, (WPARAM) 0, (LPARAM)m_uPortNo);		
			}
			if (CommEvent & EV_RING)
			{
				CString LogString;
				LogString.Format(_T("COMM RING Detected"));
				//m_plogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, LogString, NULL, NULL);
		
				if (pPortOwnerWindow->m_hWnd != NULL)
					::SendMessage(pPortOwnerWindow->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM)m_uPortNo);
			}
		
		}

		if (CommEvent & EV_RXCHAR)
		{
			int j = 0;
			memset(RXBuff, 0x00, sizeof(RXBuff));
			memset(ReceiveBuffer, 0x00, sizeof(ReceiveBuffer));
				
			for (;;)
			{
				// ClearCommError() will update the COMSTAT structure and clear any other errors.
				bResult = ClearCommError(m_hComm, &dwError, &comstat);
				if (comstat.cbInQue == 0)
				{
					// break out when all bytes have been read
					break;
				}		
				bResult = ReadFile(m_hComm,			// Handle to COMM port 
									&RXBuff,				// RX Buffer Pointer
									1,					// Read one byte
									&BytesRead,			// Stores number of bytes read
									&m_osReceive);			// pointer to the m_ov structure
				// deal with the error code 
				if (!bResult)  
				{ 
					switch (dwError = GetLastError()) 
					{ 
					case ERROR_IO_PENDING: 	
						while (!GetOverlappedResult(m_hComm, &m_osReceive, &BytesRead, TRUE))
						{
							dwError = GetLastError();
							if (dwError != ERROR_IO_INCOMPLETE)
								break;
						}
					default:
						{
							//이곳에 에러를 넣는다. 
							//즉 ReadFile했을 때 데이터가 제대로 안 나오면 bResult 여러 
							//에러 코드를 리턴한다. 이 때 복구할 수 있으면 좋지만 실질적인 
							//복구가 불가능하다. 따라서, 재송출을 해달라는 메시지를 해주는 것이 좋다. 
							// Another error has occured.  Process this error.
							CString LogString;
							LogString.Format(_T("ReadFile Error"));
							//m_plogMng->WriteLog(DEF_MLOG_ERROR_LOG_LEVEL, LogString, NULL, NULL);		
							
							break;
						} 
					}
				}
				else
				{
					j = strlen(RXBuff);
					memcpy(ReceiveBuffer + i, RXBuff, j);
					i = i + j;
				}
				// ML_hongju 20081107 확인 필요.
				Sleep(10);
			} // end forever loop

			

			///char strLog[100];
			///sprintf(strLog, "%s%s", "Receive Msg = ",ReceiveBuffer);
			///WriteNormalLog(CString(strLog));

			if (strcmp(ReceiveBuffer,""))
			{
				// jdy sesl 090102	CString strTemp;
				char	szJoinMessage[250];
				char	szSplitMessage[250];
				char	szBuff[250];
				int ifind=0, iLen = 0;
				i = 0;

				SendBuffer = (char *)malloc(sizeof(char) * 250);

				memset(szJoinMessage, 0x00, sizeof(szJoinMessage));
				memset(szSplitMessage, 0x00, sizeof(szSplitMessage));
				memset(szBuff, 0x00, sizeof(szBuff));
				memset(SendBuffer, 0x00, sizeof(SendBuffer));
				memcpy(SendBuffer, ReceiveBuffer, sizeof(ReceiveBuffer));
				
				/* jdy sesl 090102
				USES_CONVERSION;
				WCHAR* pszAnsi1 = NULL;
				pszAnsi1 = A2W(ReceiveBuffer);
	
				strTemp = pszAnsi1;
//				strTemp = ReceiveBuffer;
				*/

				CString strTemp(ReceiveBuffer);

				iLen = strlen(ReceiveBuffer);

				//////////////////////////////////////////////////////////////////////////
				ifind = strTemp.Find(m_szEnd);
				
				if (ifind!=-1)
				{		
					if (!strcmp(szTemp, "")) // NULL
					{

						if ((ifind + 1) == iLen) // CR의 위치가 End인 경우
						{
							sprintf(SendBuffer, "%s", ReceiveBuffer);
//							sprintf(SendBuffer, "%s", strTemp);
							memset(szTemp, 0x00, sizeof(szTemp));
						}
						else if (ifind ==0) // First
						{
							sprintf(szBuff, "%s", strTemp);
							memset(szTemp, 0x00, sizeof(szTemp));
							memcpy(szTemp, szBuff + 1 , iLen -1);
							memset(szBuff, 0x00, sizeof(szBuff));
							memset(SendBuffer, 0x00, sizeof(SendBuffer));
						}
						else // Mid
						{
							sprintf(szBuff, "%s", strTemp);
							strncpy(szSplitMessage, szBuff, ifind);
							sprintf(SendBuffer, "%s", szSplitMessage);
							memset(szSplitMessage, 0x00, sizeof(szSplitMessage));
							memcpy(szSplitMessage, szBuff + ifind + 1, iLen - ifind -1);
							memset(szTemp, 0x00, sizeof(szTemp));
							strcpy(szTemp, szSplitMessage);
						}						
					}
					else
					{
						if ((ifind + 1) == iLen) // CR의 위치가 End인 경우
						{
							sprintf(szBuff, "%s", strTemp);
							strcpy(szSplitMessage, szBuff);
							sprintf(szJoinMessage, "%s%s", szTemp, szSplitMessage);
							sprintf(SendBuffer, "%s", szJoinMessage);
							memset(szTemp, 0x00, sizeof(szTemp));
						}
						else if (ifind ==0) // First
						{
							sprintf(szBuff, "%s", strTemp);
							strncpy(szSplitMessage, szBuff, 1);
							sprintf(szJoinMessage, "%s%s", szTemp, szSplitMessage);
							sprintf(SendBuffer, "%s", szJoinMessage);
							memset(szTemp, 0x00, sizeof(szTemp));
							memcpy(szBuff, szBuff + 1, iLen - 1);
							strcpy(szTemp, szBuff);
							memset(szBuff, 0x00, sizeof(szBuff));
							
						}
						else // Mid
						{
							sprintf(szBuff, "%s", strTemp);
							strncpy(szSplitMessage, szBuff, ifind);
							sprintf(szJoinMessage, "%s%s", szTemp, szSplitMessage);
							sprintf(SendBuffer, "%s", szJoinMessage);
							memset(szTemp, 0x00, sizeof(szTemp));
							memcpy(szBuff, szBuff + ifind + 1, iLen - ifind);
							strcpy(szTemp, szBuff);
							memset(szBuff, 0x00, sizeof(szBuff));
							
						}
						
					}
				}
				else
				{
					if (!strcmp(szTemp, ""))
					{
						sprintf(szTemp, "%s", ReceiveBuffer);
						memset(SendBuffer, 0x00, sizeof(SendBuffer));
					}
					else
					{
						sprintf(szJoinMessage, "%s%s", szTemp, strTemp);
						strcpy(szTemp, szJoinMessage);
						memset(SendBuffer, 0x00, sizeof(SendBuffer));
					}
				}

				if (strcmp(SendBuffer, ""))
				{
					CString LogString;
					LogString.Format(_T("Receive << %s \n"), SendBuffer);
					//WriteNormalLog(LogString);
					//m_plogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, LogString, NULL, NULL);
					m_ReceiveQueue.Put(&SendBuffer);
					//if (pPortOwnerWindow->m_hWnd != NULL)
					// **1202
					if (pPortOwnerWindow != NULL)
					{
						if (pPortOwnerWindow->m_hWnd != NULL)
							::SendMessage(pPortOwnerWindow->m_hWnd, WM_COMM_RXCHAR, (WPARAM) 0, (LPARAM)m_uPortNo);
					}
					
				}
			}

		}
		
		Sleep(10);
	} // close forever loop
	
	AfxEndThread(0);

	if (::WaitForSingleObject(m_cwReceiveThread->m_hThread, 1000))		//INFINITE))
	{
		AfxMessageBox(_T("Thread 소멸 실패"));
	}

	
	free(SendBuffer);


	return ERR_SERIALPORT_SUCCESS;
}

int MSerialPort::StartThread()
{
	TRACE(_T("Thread started\n"));

	if (m_cwReceiveThread==NULL)
	{
		if (!(m_cwReceiveThread = AfxBeginThread(ReceiveEntryPoint, this))) 
		{
			//WriteErrorLog("StartThread() : Serial Port Receive Thread Begin Fail");
			SetErrorLevel(_T("SERIALPORT begin Receive Thread"),3, __FILE__, __LINE__);
			return generateErrorCode(108001);	// ERR_SERIALPORT_RECEIVETHREAD_BEGIN_FAIL
		}
	}
	else
		m_cwReceiveThread->ResumeThread();
	if (m_cwSendThread==NULL)
	{
		if (!(m_cwSendThread = AfxBeginThread(SendEntryPoint, this))) 
		{
			//WriteErrorLog("StartThread() : Serial Port Send Thread Begin Fail");
			SetErrorLevel(_T("SERIALPORT begin Send Thread"),3, __FILE__, __LINE__);
			return generateErrorCode(108002);	// ERR_SERIALPORT_SENDTHREAD_BEGIN_FAIL
		}
	}
	else
		m_cwSendThread->ResumeThread();


	m_bThreadAlive = TRUE;
	return ERR_SERIALPORT_SUCCESS;
}





int MSerialPort::PauseThread()
{
	TRACE(_T("Thread suspended\n"));

	m_cwSendThread->SuspendThread(); 
	m_cwReceiveThread->SuspendThread(); 
	
	return ERR_SERIALPORT_SUCCESS;
}

int MSerialPort::RestartThread()
{
	TRACE(_T("Thread resumed\n"));
	m_cwSendThread->ResumeThread();
	m_cwReceiveThread->ResumeThread();
	return ERR_SERIALPORT_SUCCESS;
}
int MSerialPort::SendData(char *pszMsg)
{
	double SendMessageSize;
	char	*szSendMessage;
	szSendMessage = (char*) malloc(sizeof(char) * 250);
	memset(szSendMessage, 0x00, sizeof(char));

	SendMessageSize = strlen(pszMsg);

	sprintf(szSendMessage, "%s", pszMsg);
	if (SendMessageSize >= 250)
	{
		//WriteErrorLog("SendData() : Send Message Size is lager than 250 Byte");
		SetErrorLevel(_T("SERIALPORT send Data"),3, __FILE__, __LINE__);
		return generateErrorCode(108005);	// ERR_SERIALPORT_SEND_DATA_WRONG_SIZE
	}
	else
	{
		if (m_SendQueue.Put(&szSendMessage))
			return ERR_SERIALPORT_SUCCESS;
	}

	//WriteErrorLog("SendData() : Putting the SendData to the SendQue is Fail");
	SetErrorLevel(_T("SERIALPORT send Data"),3, __FILE__, __LINE__);
	return generateErrorCode(108006);	// ERR_SERIALPORT_SEND_DATA_PUT_FAIL
}

int MSerialPort::ReceiveData(char *pRecvData) 
{
	char	*RecvBuf;
	char	ReBuffer[255];

	if (!m_ReceiveQueue.IsEmpty())
	{
		if (m_ReceiveQueue.Get(&RecvBuf))
		{
			memcpy(ReBuffer, RecvBuf, sizeof(ReBuffer));
			free(RecvBuf);
			memset(pRecvData, 0x00, sizeof(char));
			sprintf(pRecvData, "%s", ReBuffer);
			return ERR_SERIALPORT_SUCCESS;		
		}
	}
	//pRecvData->Format("");
	//WriteErrorLog("ReceiveData() : Receive Data Fail");
	SetErrorLevel(_T("SERIALPORT receive Data"),3, __FILE__, __LINE__);
	return generateErrorCode(108003);	// ERR_SERIALPORT_RECEIVE_DATA_FAIL
}

int MSerialPort::ReceiveData(CString *pRecvData) 
{
	char	*RecvBuf;
// jdy sesl 090102	char	ReBuffer[512];

	if (!m_ReceiveQueue.IsEmpty())
	{
		if (m_ReceiveQueue.Get(&RecvBuf))
		{
			/* jdy sesl 090102
			memcpy(ReBuffer, RecvBuf, sizeof(ReBuffer));
			free(RecvBuf);
			
			USES_CONVERSION;
			WCHAR* pszAnsi1 = NULL;
			pszAnsi1 = A2W(ReBuffer);
			pRecvData->Format(_T("%s"), pszAnsi1);

//			pRecvData->Format(_T("%s"), ReBuffer);
//			pRecvData->Format("%s", ReBuffer);
			*/
			pRecvData->Format(_T("%s"), CString(RecvBuf));
			free(RecvBuf);

			return ERR_SERIALPORT_SUCCESS;		
		}
	}
	//pRecvData->Format("");
	//WriteErrorLog("ReceiveData() : Receive Data Fail");
	SetErrorLevel(_T("SERIALPORT receive Data"),3, __FILE__, __LINE__);
	return generateErrorCode(108003);	// ERR_SERIALPORT_RECEIVE_DATA_FAIL
}
/** Receive Queue를 Clear한다. */
void MSerialPort::ClearRecvQueue()
{
	m_ReceiveQueue.Clear();
}



char * MSerialPort::CheckSum(char *Command)
{
	int		Len = 0;
	int		SumVal = 0;
	char	CheckSumVal[255];

	int i=0;

	memset(CheckSumVal, 0x00, sizeof(CheckSumVal));

	Len = strlen(Command);

	for (i=0; i< Len ;i++)
		SumVal = SumVal + (unsigned char)Command[i];
	
	sprintf(CheckSumVal,"%x", SumVal);
	
	Len = strlen(CheckSumVal);

	return _strupr(&CheckSumVal[Len-2]);
}
char* MSerialPort::E5CNCheckSum(char *Command)
{
	int iLength, iSum, i;
	char	CheckSumVal[5];
	memset(CheckSumVal, 0x00, sizeof(CheckSumVal));

	iLength = strlen(Command);
	iSum=0;
	for (i = 0; i < iLength ; i++) 
		iSum^=Command[i];	//exclusive OR
	
	sprintf(CheckSumVal,"%x", iSum);
	iLength = strlen(CheckSumVal);

	return _strupr(&CheckSumVal[iLength-2]);

}
void MSerialPort::ClosePort()
{
	SetCommMask(m_hComm, 0);
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	CloseHandle(m_hComm);
}

void MSerialPort::SetDCB(DCB	pDcb)
{
	m_dcb = pDcb;
}
void MSerialPort::GetDCB(DCB	*pDcb)
{
	*pDcb = m_dcb;
}
