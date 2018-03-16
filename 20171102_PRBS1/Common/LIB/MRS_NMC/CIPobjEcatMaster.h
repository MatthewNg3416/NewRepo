#ifndef CIPOBJ_ECAT_MASTER_H
#define CIPOBJ_ECAT_MASTER_H

#include "NMC_Motion.h"

//============================================================================
//                              > DEFINES <
//----------------------------------------------------------------------------
// CIP Definition. Do not change value
#define CIP_CLASS_ECAT_MASTER														0x350

#define ECAT_MASTER_OBJ_CLASS_ATTR__REVISION				1
#define ECAT_MASTER_OBJ_CLASS_ATTR__MAX_INST				1
#define ECAT_MASTER_OBJ_CLASS_ATTR__INST_COUNT				1

#define ECAT_MASTER_OBJ_INST_ATTR__VENDOR_ID				1
#define ECAT_MASTER_OBJ_INST_ATTR__PRODUCT_CODE				2
#define ECAT_MASTER_OBJ_INST_ATTR__SERIAL_NUM				3
#define ECAT_MASTER_OBJ_INST_ATTR__REDUNDANCY_SUPPORT		4
#define ECAT_MASTER_OBJ_INST_ATTR__REDUNDANCY_ENABLE		5
#define ECAT_MASTER_OBJ_INST_ATTR__MAIN_DEV_MAC_ADDR		6
#define ECAT_MASTER_OBJ_INST_ATTR__BACKUP_DEV_MAC_ADDR		7
#define ECAT_MASTER_OBJ_INST_ATTR__ACT_ECAT_STATE			8
#define ECAT_MASTER_OBJ_INST_ATTR__REQ_ECAT_STATE			9
#define ECAT_MASTER_OBJ_INST_ATTR__SLAVE_NUM				10
#define ECAT_MASTER_OBJ_INST_ATTR__PROTOCOL_SUPPORT			11
#define ECAT_MASTER_OBJ_INST_ATTR__DC_CLK_SUPPORT			12
#define ECAT_MASTER_OBJ_INST_ATTR__DC_CLK_VALUE				13
#define ECAT_MASTER_OBJ_INST_ATTR__NETWORK_STATUS			14
#define ECAT_MASTER_OBJ_INST_ATTR__ERROR_CODE				15
#define ECAT_MASTER_OBJ_INST_ATTR__SCAN_STATUS				16
#define ECAT_MASTER_OBJ_INST_ATTR__SCAN_MODE				17
#define ECAT_MASTER_OBJ_INST_ATTR__SCAN_TARGET_DEVICE		18
#define ECAT_MASTER_OBJ_INST_ATTR__BOOT_REVISION			19
#define ECAT_MASTER_OBJ_INST_ATTR__OS_REVISION				20
#define ECAT_MASTER_OBJ_INST_ATTR__MC_REVISION				21
#define ECAT_MASTER_OBJ_INST_ATTR__EXIST_CONFIG_FILE		22
#define ECAT_MASTER_OBJ_INST_ATTR__MASTERID					23
#define ECAT_MASTER_OBJ_MAX_INST_ATTR										(ECAT_MASTER_OBJ_INST_ATTR__MASTERID)

#define ECAT_MASTER_OBJ_SERVICE_DOWN_LOAD_CFG_FILE			0x33
#define ECAT_MASTER_OBJ_SERVICE_UP_LOAD_CFG_FILE			0x34

#define ECAT_MASTER_PROTOCOL_AOE							0x0001
#define ECAT_MASTER_PROTOCOL_EOE							0x0002
#define ECAT_MASTER_PROTOCOL_COE							0x0004
#define ECAT_MASTER_PROTOCOL_FOE							0x0008
#define ECAT_MASTER_PROTOCOL_SOE							0x0010
#define ECAT_MASTER_PROTOCOL_VOE							0x0020

#define ECAT_MASTER_OBJ_SVC_MODE_CHG						0x35
#define ECAT_MASTER_MODE_CHG_REQ_SIZE						4

#define ECAT_MASTER_OBJ_SCAN_NONE							0
#define ECAT_MASTER_OBJ_SCAN_BUSY							1
#define ECAT_MASTER_OBJ_SCAN_DONE							2

#define ECAT_MASTER_OBJ_SVC_GET_SCAN_INFO					0x36
#define ECAT_MASTER_OBJ_SVC_GET_SCAN_INFO_RET_SIZE			6

#define ECAT_MASTER_OBJ_SVC_GET_SII_SIZE					0x37
#define ECAT_MASTER_OBJ_SVC_GET_SII_SIZE_RET_SIZE			6

#define ECAT_MASTER_OBJ_SVC_GET_SII_DATA					0x38
#define ECAT_MASTER_OBJ_SVC_GET_SII_DATA_RET_SIZE			10

#define ECAT_MASTER_OBJ_SVC_GET_SDO_NUM						0x39
#define ECAT_MASTER_OBJ_SVC_GET_SDO_NUM_RET_SIZE			4

#define ECAT_MASTER_OBJ_SVC_GET_SDO_LIST					0x3A
#define ECAT_MASTER_OBJ_SVC_GET_SDO_LIST_RET_SIZE			6

#define ECAT_MASTER_OBJ_SVC_GET_SDO							0x3B
#define ECAT_MASTER_OBJ_SVC_GET_SDO_RET_SIZE				6

#define ECAT_MASTER_OBJ_SVC_GET_SDO_ENTRY					0x3C
#define ECAT_MASTER_OBJ_SVC_GET_SDO_ENTRY_RET_SIZE			12

#define ECAT_MASTER_OBJ_SVC_GET_CUR_MST_MODE				0x3D
#define ECAT_MASTER_OBJ_SVC_GET_CUR_MST_MODE_SIZE			0 //has no request data

#define ECAT_MASTER_OBJ_SVC_GET_REQ_MST_MODE				0x3E
#define ECAT_MASTER_OBJ_SVC_GET_REQ_MST_MODE_SIZE			0 // has no request data

#define ECAT_MASTER_OBJ_SVC_UPLOAD_SDODATA					0x40
//#define ECAT_MASTER_OBJ_SVC_UPLOAD_SDODATA_RET_SIZE		12

#define ECAT_MASTER_OBJ_SVC_DNLOAD_SDODATA					0x41
//#define ECAT_MASTER_OBJ_SVC_GET_SDO_ENTRY_RET_SIZE		12

#define ECAT_MASTER_OBJ_SVC_ECAT_DIRECT_ACCESS				0x42

//============================================================================
//                              > MACROS <
//----------------------------------------------------------------------------
#define SET_SINGLE_ATTRIBUTE 0x10
#define GET_SINGLE_ATTRIBUTE 0x0E
#define GET_ALL_ATTRIBUTE		0x01
//============================================================================
//                              > ENUMERATED TYPES <
//----------------------------------------------------------------------------

//============================================================================
//                              > STRUCTURES and TYPEDEFS <
//----------------------------------------------------------------------------
#pragma pack(push, 1)

typedef struct ecatMasterClassAttrAll
{
	UINT16 Revision;
	UINT16 MaxInstance;
	UINT16 MaxIDNoClassAttr;
	UINT16 MaxIDNoInstAttr;
}EcatMasterClassAttrAll;


typedef struct ecatMasterInstAttrAll
{
	UINT16 vendorID;
	UINT16 productCode;
	UINT32 serialNum;
	UINT32 mainDevMacAddr;
	UINT32 backupDevMacAddr;
	UINT16 totalSlaveNum;
	UINT16 protocolSupport;
	UINT8  dcClkSupport;
	UINT8  redundancySupport;
}EcatMasterInstAttrAll;

// 0x35 request
typedef struct ecatMasterChangeMasterModeReq
{
	UINT8 MasterMode; //0- idle, 1-scan, 2-run
	UINT8  ScanMode; //0-scan all, 1-scan single target slave device
	UINT16 SlaveNum; //- scan all = 0000, scan target device - target device index
}EcatMasterModeReq;

// 0x36 response
typedef struct ecatMasterGetScanInfoResp
{
	UINT8 ScanStatus; //0-no scan, 1-scan busy, 2-scan done
	UINT8  ScanMode; // 0- scan all, 1-scan single
	UINT16 SlaveNum; //- scan all = 0000, scan target device - target device index
	UINT16 TotalSlaveCount; //- Total slave count what it scaned
}EcatMasterScanInfoResp;

// 0x37
typedef struct ecatMasterSiiSizeReq
{
	UINT16 slaveNum;
}EcatMasterSiiSizeReq;

typedef struct ecatMasterSiiSizeRsp
{
	UINT16 slaveNum;
	UINT32 EEPROMSize;
}EcatMasterSiiSizeRsp;


//0x38
typedef struct ecatMasterSiiDataReq
{
	UINT16 slaveNum;
	UINT32  eepromOffset;
	UINT32 reqSize;
}EcatMasterSiiDataReq;

typedef struct ecatMasterSiiDataRsp
{
	UINT16 slaveNum; //2
	UINT32  eepromOffset; //4
	UINT32 rspSize; //4
	UINT8	 data[MAX_EEPROM_SIZE];
}EcatMasterSiiDataRsp;


//0x39
typedef struct ecatMasterSdoCntReq
{
	UINT16 slaveNum;
}EcatMasterSdoCntReq;

typedef struct ecatMasterSdoCntRsp
{
	UINT16 slaveNum;
	UINT16 SdoListCount;
}EcatMasterSdoCntRsp;


//0x3a
typedef struct ecatMasterSdoListReq
{
	UINT16 slaveNum;
	UINT16 listCountOffset;
	UINT16 reqListCount;
}EcatMasterSdoListReq;

typedef struct ecatMasterSdoListRsp
{
	UINT16 slaveNum;
	UINT16 listCountOffset;
	UINT16 rspListCount;
	UINT16 * SDOList; 
}EcatMasterSdoListRsp;

//0x3b get SDO
typedef struct ecatMasterSdoReq
{
	UINT16 slaveNum;
	UINT16 sdoIndexNum;
}EcatMasterSdoReq;

typedef struct ecatMasterSdoRsp
{
	UINT16 slaveNum;
	UINT16 sdoIndexNum;
	UINT8	 objectCode;
	UINT8	 maxSubIndexNum;
	UINT8 name[ECAT_MAX_NAME];
}EcatMasterSdoRsp;

//0x3c get SDO entry
typedef struct ecatMasterSdoEntryReq
{
	UINT16 slaveNum;
	UINT16 sdoIndexNum;
	UINT8  sdoSbuIndexNum;
	UINT8	 reserved;
}EcatMasterSdoEntryReq;

typedef struct ecatMasterSdoEntryRsp
{
	UINT16 slaveNum;
	UINT16 sdoIndexNum;
	UINT8	 sdoSbuIndexNum;
	UINT8	 reserved;
	UINT16 dataType;
	UINT16 bitLength;
	UINT16 accessRule;
	UINT8	 name[ECAT_MAX_NAME];
}EcatMasterSdoEntryRsp;

//0x3d get current master mode
typedef struct ecatMasterGetCurModeRsp_Tag
{
	UINT8 masterMode;
	UINT8 masterScanMode;
	UINT16 masterScanTargetDevice;
}EcatMasterGetCurModeResp;

//0x3e get requested mater mode
typedef struct ecatMasterGetReqModeRsp_Tag
{
	UINT8 masterMode;
	UINT8 masterScanMode;
	UINT16 masterScanTargetDevice;
}EcatMasterGetReqModeResp;

//0x40 Upload SDO Data(CoE)
typedef struct ecatMasterUploadSDODataReq
{
	UINT16 slaveNum;
	UINT16 sdoIndexNum;
	UINT8  sdoSbuIndexNum;
	UINT8	 reserved;
	UINT32 DataSize;
}EcatMasterUploadSDODataReq;

typedef struct ecatMasterUploadSDODataRsp
{
	UINT16  slaveNum;
	UINT16  sdoIndexNum;
	UINT8	 sdoSbuIndexNum;
	UINT8	 reserved;
	UINT32  DataSize;
	UINT8	 Data[1]; // Data should be momory allocated by user.
}EcatMasterUploadSDODataRsp;


//0x41 Upload SDO Data(CoE)
typedef struct ecatMasterDnloadSDODataReq
{
	UINT16 slaveNum;
	UINT16 sdoIndexNum;
	UINT8  sdoSbuIndexNum;
	UINT8	 reserved;
	UINT32 DataSize;
	UINT8 Data[1]; // Data Should be memory allocated by user..
}EcatMasterDnloadSDODataReq;

typedef struct ecatMasterDnloadSDODataRsp
{
	UINT16  slaveNum;
	UINT16  sdoIndexNum;
	UINT8	 sdoSbuIndexNum;
	UINT8	 reserved;
	UINT32  DataSize;
}EcatMasterDnloadSDODataRsp;


//0x42 Ecat Direct Access
typedef struct ecatMasterEcatDirectAccessReq
{
	UINT8 ECatCmd;
	UINT8 reserved;
	UINT16 Adp;
	UINT16	 Ado;
	UINT16 DataSize;
	UINT8 Data[1];
}EcatMasterEcatDirectAccessReq;

typedef struct ecatMasterEcatDirectAccessRsp
{
	UINT8 ECatCmd;
	UINT8 reserved;
	UINT16 Adp;
	UINT16	 Ado;
	UINT16 DataSize;
	UINT16 WC;
	UINT8 Data[1];
}EcatMasterEcatDirectAccessRsp;


#pragma pack(pop)

//============================================================================
//                              > FUNCTION PROTOTYPES <
//----------------------------------------------------------------------------

//*****************************************************************************************************************
//Class Attribute
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetClassAttributeAll(UINT16 BoardID,  ecatMasterClassAttrAll * data);
// single Attribute
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetClassAttribute(UINT16 BoardID, UINT16 AttibuteID, UINT16 * data);

//*****************************************************************************************************************
// Instant Attribute
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetInstAttributeAll(UINT16 BoardID, UINT16 Instance, ecatMasterInstAttrAll * data);
// Single Attribute
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetVendorID(UINT16 BoardID, UINT32 * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetProductCode(UINT16 BoardID, UINT32 * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSerialNum(UINT16 BoardID, UINT32 * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetRedundancySupport(UINT16 BoardID, UINT8	* data);
//NMCMOTIONAPI MC_STATUS  MasterSetRedundancyEnable(UINT16 BoardID, UINT8    data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetDevMacAddr(UINT16 BoardID, UINT8    data[6]);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetBackupDevMacAddr(UINT16 BoardID, UINT8    data[6]);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetActEcatState(UINT16 BoardID, UINT8  * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetReqEcatState(UINT16 BoardID, UINT8  * data);
//NMCMOTIONAPI MC_STATUS  MasterSetReqEcatState(UINT16 BoardID, UINT8    data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSalveNum(UINT16 BoardID, UINT16 * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetProtocolSupport(UINT16 BoardID, UINT16 * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetDCClkSupport(UINT16 BoardID, UINT8  * data);
//NMCMOTIONAPI MC_STATUS  MasterGetDCClkValue(UINT16 BoardID, UINT64 * data);
//NMCMOTIONAPI MC_STATUS  MasterSetDCClkValue(UINT16 BoardID, UINT64   data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetNetworkStatus(UINT16 BoardID, UINT32 *LinkStatus,UINT32 *TimeoutCount,UINT32 *SendPacketCount,UINT32 *RecvPacketCount);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetErrorCode(UINT16 BoardID, UINT32 * data);
//NMCMOTIONAPI MC_STATUS  MasterSetErrorCode(UINT16 BoardID, UINT32   data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetScanStatus(UINT16 BoardID, UINT8  * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetScanMode(UINT16 BoardID, UINT8  * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetScanTargetDevice(UINT16 BoardID, UINT16 * data);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetBootRevision(UINT16 BoardID, UINT8 * Major, UINT8 * Minor);
//NMCMOTIONAPI MC_STATUS  MasterGetOSRevision(UINT16 BoardID, UINT8 * Major, UINT8 * Minor);
//NMCMOTIONAPI MC_STATUS  MasterGetMCRevision(UINT16 BoardID, UINT8 * Major, UINT8 * Minor);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGethasConfigFile(UINT16 BoardID, UINT8 * Exist);
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetBoardID(UINT16 MasterScanNo, UINT16 * MasterID);

//*****************************************************************************************************************
// Common Service
extern "C" NMCMOTIONAPI MC_STATUS  MasterReset(UINT16 BoardID);

//*****************************************************************************************************************

// Object Specific Service
// 35h - Change Master Mode
extern "C" NMCMOTIONAPI MC_STATUS  MasterChgMasterMode(UINT16 BoardID, 
//											UINT16 Instance, 
											EcMstMode MasterMode,  
											EcScanMode ScanMode, 
											UINT16 ScanTargetSlave);
// 36h - Get Scan Info
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetScanInfo(UINT16 BoardID, 
//											UINT16 Instance, 
											EcScanSts * ScanSts,  
											EcScanMode * ScanMode, 
											UINT16 * ScanTargetSlave, 
											UINT16 * TotalSlaveNo);

// 37h - Get Slave SII Size
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSlaveSIISize(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT16 SlaveNo, 
											UINT32 * EROMSize);

// 38h - Get Slave SII Data
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSlaveSIIData(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT16 SlaveNo, 
											UINT32 Offs, 
											UINT32 ReqSize, 
											UINT32 * RspSize, 
											UINT8 * Data);
// 39h - Get SDO Dic. Num
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSDODicCount(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT16 SlaveNo, 
											UINT16 * DicCount);

// 3ah - Get SDO List
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSDOList(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT16 SlaveNo, 
											UINT16 SDOOffset,
											UINT16 ReqCount,
											UINT16* RspCount,
											UINT16* SDOList
											);
// 3bh - GetSDOInfo
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSDOInfo(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT16 SlaveNo, 
											UINT16 SDOIndex,
											UINT8 * ObjectCode,
											UINT8 * MaxSubIndexNum,
											char * sName
											);
// 3ch - GetSDOEntry
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSDOEntry(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT16 SlaveNo, 
											UINT16 SDOIndex,
											UINT8  SubIndex,
											UINT16 * DataType,
											UINT16 * BitLength,
											UINT16 * AccessRule,
											char * sName
											);
//// 3dh - GetCurMode
//extern "C" NMCMOTIONAPI MC_STATUS  MasterGetCurMode(UINT16 BoardID, 
////											UINT16 Instance, 
//											UINT8 * masterMode, 
//											UINT8 * scanMode,
//											UINT16 * ScanTargetDevice
//											);
// 3eh - GetReqModeRaw
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetReqModeRaw(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT8 * masterMode, 
											UINT8 * scanMode,
											UINT16 * ScanTargetDevice
											);
// 3eh - GetReqMode
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetReqMode(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT8 * masterMode
											);
// 40h - UploadSDOData
extern "C" NMCMOTIONAPI MC_STATUS  MasterGetSDOData(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT16 SlaveNo, 
											UINT16 SDOIndex,
											UINT8  SubIndex,
											UINT32  DataSize,
											UINT32 *respDataSize,
											UINT8 *bDataArray
											);

// 41h - DnloadSDOData
extern "C" NMCMOTIONAPI MC_STATUS  MasterSetSDOData(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT16 SlaveNo, 
											UINT16 SDOIndex,
											UINT8  SubIndex,
											UINT32  DataSize,
											UINT32 *respDataSize,
											UINT8 *bDataArray
											);

// 42h - Ethercat Direct Access
extern "C" NMCMOTIONAPI MC_STATUS  MasterECatDirectAccess(UINT16 BoardID, 
//											UINT16 Instance, 
											UINT8 EcatCmd, 
											UINT16 Adp,
											UINT16  Ado,
											UINT16  ReqDataSize,
											UINT8  *bReqDataArray,
											UINT16 *RespDataSize,
											UINT8   *bRespDataArray,
											UINT16 *WC
											);

//============================================================================
//                              > INLINE FUNCTION DEFINITIONS <
//----------------------------------------------------------------------------
//extern void CIPEcatMasterObj_ParseClassInstanceRqst( PCIP_MSG  pRequest );

#endif

