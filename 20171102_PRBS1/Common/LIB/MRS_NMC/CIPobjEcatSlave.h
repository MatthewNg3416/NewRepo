#ifndef CIPOBJ_ECAT_SLAVE_H
#define CIPOBJ_ECAT_SLAVE_H

#include "CIPobjEcatMaster.h"

//============================================================================
//                              > DEFINES <
//----------------------------------------------------------------------------
#define CIP_CLASS_ECAT_SLAVE 0x360
#define CIP_SLAVE_CLASS_TIMEOUT 10000

#define CIP_SLAVE_INST_ATTR_NUMBER_OF_ATTR				1
#define CIP_SLAVE_INST_ATTR_ATTRLIST					2
#define CIP_SLAVE_INST_ATTR_ALIASNUM					1
#define CIP_SLAVE_INST_ATTR_VENDORID					2
#define CIP_SLAVE_INST_ATTR_PRODUCTCODE					3
#define CIP_SLAVE_INST_ATTR_SERIALNUM					4
#define CIP_SLAVE_INST_ATTR_REVISIONNUM					5
#define CIP_SLAVE_INST_ATTR_BOOTRXMAILBOXOFFSET			6
#define CIP_SLAVE_INST_ATTR_BOOTRXMAILBOXSIZE			7
#define CIP_SLAVE_INST_ATTR_BOOTTXMAILBOXOFFSET			8
#define CIP_SLAVE_INST_ATTR_BOOTTXMAILBOXSIZE			9
#define CIP_SLAVE_INST_ATTR_STDRXMAILBOXOFFSET			10
#define CIP_SLAVE_INST_ATTR_STDRXMAILBOXSIZE			11
#define CIP_SLAVE_INST_ATTR_STDTXMAILBOXOFFSET			12
#define CIP_SLAVE_INST_ATTR_STDTXMAILBOXSIZE			13
#define CIP_SLAVE_INST_ATTR_MAILBOXPROTOCOL				14
#define CIP_SLAVE_INST_ATTR_HASGENCATEGORY				15
#define CIP_SLAVE_INST_ATTR_MASTERINSTANCENUM			16
#define CIP_SLAVE_INST_ATTR_RINGPOSITION				17
#define CIP_SLAVE_INST_ATTR_STATIONADDR					18
#define CIP_SLAVE_INST_ATTR_ERRORFLAG					19
#define CIP_SLAVE_INST_ATTR_REQUESTEDSTATE				20
#define CIP_SLAVE_INST_ATTR_CURRENTSTATE				21
#define CIP_SLAVE_INST_ATTR_CONFIGUREDFLAG				22
#define CIP_SLAVE_INST_ATTR_CNFRXMAILBOXOFFSET			23
#define CIP_SLAVE_INST_ATTR_CNFRXMAILBOXSIZE			24
#define CIP_SLAVE_INST_ATTR_CNFTXMAILBOXOFFSET			25
#define CIP_SLAVE_INST_ATTR_CNFTXMAILBOXSIZE			26
#define CIP_SLAVE_INST_ATTR_FMMUCOUNT					27
#define CIP_SLAVE_INST_ATTR_SYNCCOUNT					28
#define CIP_SLAVE_INST_ATTR_FMMUBITOPERATION			29
#define CIP_SLAVE_INST_ATTR_DCSUPPORTED					30
#define CIP_SLAVE_INST_ATTR_DCRANGE						31
#define CIP_SLAVE_INST_ATTR_HASDCSYSTEMTIME				32
#define CIP_SLAVE_INST_ATTR_TRANSMISSIONDELAY			33
#define CIP_SLAVE_INST_ATTR_SIIWORDNUM					34
#define CIP_SLAVE_INST_ATTR_SDODICFETCHED				35


//Specific Services
#define CIP_SLAVE_SVC_GET_SII_DATA						0x35
#define CIP_SLAVE_SVC_GET_DC_INFO						0x36
#define CIP_SLAVE_SVC_GET_PORT_INFO						0x37

//============================================================================
//                              > MACROS <
//----------------------------------------------------------------------------

//============================================================================
//                              > ENUMERATED TYPES <
//----------------------------------------------------------------------------

//============================================================================
//                              > STRUCTURES and TYPEDEFS <
//----------------------------------------------------------------------------

#pragma pack(push, 1)

// Instance Attribute All
typedef struct ecatSlaveInstAttrAll_tag
{
	UINT32 VenderID;         
	UINT32 ProductCode;
	UINT32 SerialNum;
	UINT32 RevisionNum;
	UINT32 MasterInstNum;
	UINT16 AlialsNum;
	UINT16 RingPosition;
	UINT16 StationAddr;
	UINT8 ErrorFlag;
	UINT8 ConfiguredFlag;
	UINT32 SIIWordNum;
	UINT8 HasGenCategory;
	UINT8 SDODicFetehed;
}EcatSlaveInstAttrAll, *PEcatSlaveInstAttrAll;

// Specific Services
typedef struct ecatSlaveSIIData_tag
{
	UINT32 sizeinWord;
	BYTE * Data;
}EcatSlaveSIIData;

typedef struct ecatSlaveDCInfo_tag
{
	UINT8 DCSupported;
	UINT8 DCRange;
	UINT8 hasDCSystemTime;
	UINT32 TransmitionDelay;
}EcatSlaveDCInfo;

typedef struct ecatSlavePortInfo_tag
{
	BYTE Desc;
	BYTE LinkStatus;
	WORD RingPositionNum; //그 포트에 연결된 Device 의 RingPosition Number 이다.
	DWORD DelaytoNextDC;
}EcatSlavePortInfo, *PEcatSlavePortInfo;

typedef struct EcatSlavePortInfoResponse_tag
{
	BYTE Count;
	BYTE reserved[3];
	EcatSlavePortInfo Port[4];
}EcatSlavePortInfoResp, *PEcatSlavePortInfoResp;

#pragma pack(pop)
//============================================================================
//                              > FUNCTION PROTOTYPES <
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Attribute
//----------------------------------------------------------------------------
// All
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetInstAttributeAll(UINT16 BoardID,  UINT16 Instance, EcatSlaveInstAttrAll * data);
// Single
//1 : MMC_Motion.h 로 이동함. //Ver_0c01000E_4
//extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetAliasNo(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//2
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetVendorID(UINT16 BoardID, UINT16 Instance, UINT32 * data);
//3
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetProductCode(UINT16 BoardID, UINT16 Instance, UINT32 * data);
//4
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetSerialNo(UINT16 BoardID, UINT16 Instance, UINT32 * data);
//5
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetRevisionNo(UINT16 BoardID, UINT16 Instance, UINT32 * data);
//6
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetBootRxMBOffset(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//7
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetBootRxMBSize(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//8
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetBootTxMBOffset(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//9
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetBootTxMBSize(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//10
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetStdRxMBOffset(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//11
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetStdRxMBSize(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//12
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetStdTxMBOffset(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//13
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetStdTxMBSize(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//14
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetMBProtocol(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//15
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetHasGenCat(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//16
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetMasterInstanceNo(UINT16 BoardID, UINT16 Instance, UINT32 * data);
//17
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetRingPositionNo(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//18
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetStationAddr(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//19
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetErrorFlag(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//20
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetReqState(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//21
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetCurState(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//22
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetCfgFlag(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//23
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetCfgRxMBOffset(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//24
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetCfgRxMBSize(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//25
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetCfgTxMBOffset(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//26
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetCfgTxMBSize(UINT16 BoardID, UINT16 Instance, UINT16 * data);
//27
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetFMMUCount(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//28
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetSyncCount(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//29
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetFMMUBitOp(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//30
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetDCSupported(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//31
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetDCRange(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//32
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetHasDCSystemTime(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//33
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetTRDelay(UINT16 BoardID, UINT16 Instance, INT16 * data);
//34
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetSIIWORDNo(UINT16 BoardID, UINT16 Instance, UINT32 * data);
//35
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetSDODicFetched(UINT16 BoardID, UINT16 Instance, UINT8 * data);
//----------------------------------------------------------------------------
// Services
//----------------------------------------------------------------------------
//0x35
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetSIIData(UINT16 BoardID, 
										UINT16 Instance, //1
										UINT32 * RspSize, 
										UINT8 * Data);
// 36h - CIP_SLAVE_SVC_GET_DC_INFO
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetDCInfo(UINT16 BoardID, 
										UINT16 Instance, //1
										UINT8 * DC_Supported, 
										UINT8 * DC_Range,
										UINT8 * DC_HasSystemTime,
										UINT32 * TransmissionDelay
										);
// 37h - CIP_SLAVE_SVC_GET_PORT_INFO
extern "C" NMCMOTIONAPI MC_STATUS  SlaveGetPortInfo(UINT16 BoardID, 
										UINT16 Instance, //1
										PEcatSlavePortInfoResp PortInfo
										);
//============================================================================
//                              > INLINE FUNCTION DEFINITIONS <
//----------------------------------------------------------------------------

#endif

