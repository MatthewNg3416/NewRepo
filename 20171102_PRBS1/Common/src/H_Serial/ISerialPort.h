// ISerialPort.h: interface for the ISerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISERIALPORT_H__968BCAFF_08E5_47F1_A1F8_2B22A83181D7__INCLUDED_)
#define AFX_ISERIALPORT_H__968BCAFF_08E5_47F1_A1F8_2B22A83181D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ICommon.h"

/**
 * Class ISerial Port	
 */
class ISerialPort  : public virtual ICommon
{
public:
	
	/** RS232C Port DCB Set */
	virtual void SetDCB(DCB	pdcb) = 0;

	/** RS232C Port DCB Get */
	virtual void GetDCB(DCB	*pdcb) = 0;


	/** ���۵� Data�� ������ üũ�Ѵ�.
	 *
	 *	@param	Command : ���۵� Data���� 	 */
	virtual char* CheckSum(char *Command) =0;

	virtual char* E5CNCheckSum(char *Command) =0;

	/** Send Queue�� Data�� �ִ´�.
	 *
	 *	@param	pszMsg : Send Queue�� �� Data
	 *  @return	:(Success : TRUE, Fail : FALSE)	 */
	virtual int SendData(char *pszMsg) =0;

	/** Receive Queue���� Data�� ��´�.. 
	 *
	 *	@return : (Receive Queue ũ�� > 0 : Data ����, �ƴϸ� NULL)	 */
	virtual int ReceiveData(char *pRecvData) =0;

	virtual int ReceiveData(CString *pRecvData)  = 0;


	/** Receive Queue�� Clear�Ѵ�. */
	virtual void ClearRecvQueue() = 0;

	/** Thread�� Resume�Ѵ�. (Restart ����)	 
	 *
	 *	@return : (Success : DEF_SERIALPORT_SUCCESS)		*/
	virtual int RestartThread() =0;

	/** Send & Receive Thread�� �����Ѵ�. 
	 *
	 *	@return	: (Success : DEF_SERIALPORT_SUCCESS)	   */	
	virtual int StartThread() = 0;

	/** Send & Receive Thread�� Suspend�Ѵ�. (Pause ����) 
	 *
	 *	@return	: (Success : DEF_SERIALPORT_SUCCESS)	   */
	virtual int PauseThread() = 0;


	/** ������ Port�� ����.
	 *
	 *	@param	pPortOwner	: CWnd Class
	 *  @param	dwCommEvents	: COMM Event  ex) EV_RXCHAR, EV_RXFLAG, EV_TXEMPTY, EV_CTS, EV_DSR, EV_RLSD...
	 *	@param	baud	: Baudrate
	 *	@param	parity	: parity
	 *	@param	databits	: Databits
	 *	@param	stopbits	: Stopbits
	 *	@param	nBufferSize	: WriteBufferSize
	 *	@return	: (Success : DEF_SERIALPORT_SUCCESS)				*/
	virtual int OpenPort(	CWnd* pPortOwner, 
							UINT PortNo,
							DWORD dwCommEvents, 
							UINT baud, 
							UINT parity, 
							UINT databits, 
							UINT stopsbits, 
							UINT nBufferSize ) = 0;
	virtual int OpenPort() = 0;

 //  virtual int OpenPort();
	/** Open�� Port�� �ݴ´�.	 */
	virtual void ClosePort() = 0;

};

#endif // !defined(AFX_ISERIALPORT_H__968BCAFF_08E5_47F1_A1F8_2B22A83181D7__INCLUDED_)
