/* 
 * Motion Component
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


#ifndef DEFSERIALVALUE_H
#define DEFSERIALVALUE_H

#define		MAX_COM_PORT_NO			20
#define		STX 		0x02
#define		DLE			0x10
#define		ETX 		0x03
#define		ACK 		0x06
#define		NAK 		0x15
#define		CR	 		0x0D
#define		LF	 		0x0A
#define		NOTUSE		0x00

#define WM_COMM_BREAK_DETECTED		WM_USER+1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_USER+2	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_USER+5	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent.  


const int DEF_SERIALPORT_SUCCESS					= 0;


#define    ASCII_LF			0x0a
#define    ASCII_CR			0x0d
#define    ASCII_XON		0x11
#define    ASCII_XOFF		0x13

#define BIT_5				5			/* Word length define	*/
#define BIT_6				6
#define BIT_7				7
#define BIT_8				8

#define CTS_DONE			0
#define CTS_USE				1
#define DSR_DONE			0
#define DSR_USE				1
#define RI_DONE				0
#define RI_USE				1
#define CD_DONE				0
#define CD_USE				1

#define DEF_DATABIT5        ((WORD)0x0005)
#define DEF_DATABIT6        ((WORD)0x0006)
#define DEF_DATABIT7        ((WORD)0x0007)
#define DEF_DATABIT8        ((WORD)0x0008)

#define    WM_COMM_READ	(WM_USER+1)		

typedef struct tagSSerialPortData
{
	UINT m_uPortNo;
	UINT m_uBaud;
	UINT m_uParity;
	UINT m_uDatabits;
	UINT m_uStopbits;
	UINT m_uWritebuffersize;	
	char m_szEnd;
} SSerialPortData;
#endif //DEFSERIALVALUE_H

