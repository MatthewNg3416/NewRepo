/*
* T7 S/W Standardization
*
* Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
*All rights reserved.
*This software is the confidential and proprietary information
*of Samsung Electronics, Inc("Confidential Information"). You
*shall not disclose such Confidential Information and shall use
*it only in accordance with the terms of the license agreement
*you entered into with Samsung.
*/

#ifndef IIO_H
#define IIO_H

#include "ICommon.h"
#include "CIFUSER.h"

const int	DEF_IO_SUCCESS		= 0;

const int	ERR_IO_SUCCESS		= 0;
const int	ERR_IO_MODULE_FAIL	= 2;

#define	ERR_DATA_FORMAT			-1
#define	ERR_NULL_POINTER		-2
#define	ERR_DEVICE_DRIVER		-3
#define	ERR_RECEIVED_DATA		-4
#define ERR_COMMUNICATION		-5
#define ERR_TX_TIMEOUT			-6
#define ERR_RX_TIMEOUT			-7

//const char	STS_NONE			= 0;		//171228 JSh
const char	STS_NORMAL			= 1;
const char	STS_FAIL			= 2;
const char	STS_STOP			= 3;	

typedef
/**
* DeviceNet 의 상태를 올려 주기 위한 데이타 구조이다. 
*/
struct DN_STATUS
{
	/** Master 모듈의 상태 표시 */
	char dnm_status;
	/** 4바이트 Align */
	char reserved[3];
	/** 64개의 Slave에대한 각각의 상태 표시 */
	char dns_status[MAX_DEVICE];
} DN_STATUS;

#pragma pack(1)

typedef struct tagMSG_ID_STRUC
{
	unsigned char	service_code;		// 함수 선택
	unsigned char	clss;				
	unsigned char	mac;
	unsigned short	instance;
	unsigned char	attr;
	unsigned char	srvLen;
	unsigned char	*p_srvBuffer;
} MSG_ID_STRUC;

#pragma pack()

typedef struct tagIODATA
{
	unsigned IO_0 : 1;
	unsigned IO_1 : 1;
	unsigned IO_2 : 1;
	unsigned IO_3 : 1;
	unsigned IO_4 : 1;
	unsigned IO_5 : 1;
	unsigned IO_6 : 1;
	unsigned IO_7 : 1;
}IOData;

typedef struct tagIOTABLE
{
	WCHAR	Name[80];
	int		Address;
	int		Index;
	int		Type;
}SIOTABLE;

const int	INPUT_ADDRESS					= 0;
const int	OUTPUT_ADDRESS					= 1;

const int	INPUT_MAX_NUM_ADDRESS			= 512;
const int	OUTPUT_MAX_NUM_ADDRESS			= 512;

const int	INPUT_BASE_ADDRESS				= 1000;
const int	OUTPUT_BASE_ADDRESS				= 2000;

const int	DEF_DEVICE_NET_BOARD_NUMBER_0	=	0;

/**
* This is an interface class for the IO component.
* 각 Hischer Board당 하나의 I/O Component가 대응한다 (하나의 CHilscherDnet 객체 생성)
* @author Jeongseung Moon
* version 1.0
* @interface
*/
class IIO : public virtual ICommon
{
public:  
	
	virtual int	GetIOTable(SIOTABLE* Input, SIOTABLE* Output)=0;
	
	virtual int LoadIOMap(CString strFileName = "IO.Dat")=0;
	
	
    virtual int Initialize() = 0;
	
    virtual int GetBit(unsigned short usIOAddr, BOOL* pbval)  = 0;
	
    virtual int Terminate()  = 0;
	
    virtual BOOL IsOn(unsigned short usIOAddr) = 0;
	
    virtual BOOL IsOff(unsigned short usIOAddr) = 0;
	
    virtual int OutputOn(unsigned short usIOAddr) = 0;
	
    virtual int OutputOff(unsigned short usIOAddr) = 0;
	
    virtual int OutputToggle(unsigned short usIOAddr) = 0;
	
    virtual int GetByte(unsigned short usIOAddr, BYTE & pcValue) = 0;
	
    virtual int PutByte(unsigned short usIOAddr, BYTE pcValue) = 0;
	
    virtual int GetWord(unsigned short usIOAddr, WORD & pwValue) = 0;
	
    virtual int PutWord(unsigned short usIOAddr, WORD pwValue) = 0;
	
    virtual int GetBit(CString strIOAddr, BOOL* pbVal) = 0;
	
    virtual int IsOn(CString strIOAddr, BOOL* pbVal) = 0;
	
    virtual int IsOff(CString strIOAddr, BOOL* pbVal) = 0;
	
    virtual int OutputOn(CString strIOAddr) = 0;
	
    virtual int OutputOff(CString strIOAddr) = 0;
	
    virtual int OutputToggle(CString strIOAddr) = 0;
	
    virtual int GetByte(CString strIOAddr, BYTE & pcValue) = 0;
	
    virtual int PutByte(CString strIOAddr, BYTE pcValue) = 0;
	
    virtual int GetWord(CString strIOAddr, WORD & pwValue) = 0;
	
    virtual int PutWord(CString strIOAddr, WORD pwValue) = 0;
	
    virtual void RunIOThread() = 0;
	
	virtual int DnStatusGet(DN_STATUS DnStatus) = 0;
	
	virtual int SendExplicitMsg(MSG_ID_STRUC* psMsgID, MSG_STRUC* psReceiveMsg) = 0;

	//110613.KKY_____
	virtual int UpdateIO() = 0;
	//_______________
	
};
#endif //IIO_H
