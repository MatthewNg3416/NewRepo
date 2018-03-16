#ifndef CIPOBJ_EEPROM_H
#define CIPOBJ_EEPROM_H

#include "NMC_Motion.h"

//============================================================================
//                              > DEFINES <
//----------------------------------------------------------------------------
#define CIP_CLASS_EEPROM 0x380
#define CIP_CLASS_EEPROM_TIMEOUT 10000

#define CIP_EEPROM_CLASS_ATTR_REVISION					1
#define CIP_EEPROM_CLASS_ATTR_MAXINST					2
#define CIP_EEPROM_CLASS_ATTR_INSTANCENUM				3
#define CIP_EEPROM_CLASS_ATTR_LIST						4
#define CIP_EEPROM_CLASS_ATTR_OPTLISTNUM				5
#define CIP_EEPROM_CLASS_ATTR_OPTLIST					6
#define CIP_EEPROM_CLASS_ATTR_OPSVCLIST					7

/*
1	Required	R	File Size		Uint16		System EEPROM의 File Size를 나타낸다.	
2	Required	R	File Revision	Uint16		System EEPROM File의 Revision.	HByte : Major Rev. LByte : Minor Rev.
3	Required	R	Catalog Num		Uint8[16]	제품의 Catalog Number	
4	Required	R	H/W Revision	Uint16		Hardware Revision	HByte : Major Rev. LByte : Minor Rev.
5	Required	R	Product Code	Uint32		제품의 Ethercat Product Code를 나타낸다.	
6	Required	R	Max Axes		Uint16		제품이 지원하는 최대 Motion Axes Number	
7	Required	R	Max IO			Uint16		제품이 지원하는 최대 IO Size를 Byte 단위로 표시	
8	Required	R	Mac. Address	Uint8[6]	제품에 할당된 Ethernet Mac. Address를 나타낸다.	
*/
#define CIP_EEPROM_INST_ATTR_SIZE						1
#define CIP_EEPROM_INST_ATTR_REVISION					2
#define CIP_EEPROM_INST_ATTR_CATNUM						3
#define CIP_EEPROM_INST_ATTR_HWREVISION					4
#define CIP_EEPROM_INST_ATTR_PRODUCTCODE				5
#define CIP_EEPROM_INST_ATTR_MAXAXES					6
#define CIP_EEPROM_INST_ATTR_MAXIO						7
#define CIP_EEPROM_INST_ATTR_MACADDR					8


//Specific Services
#define CIP_EEPROM_SVC_UPDATE							0xAA

//============================================================================
//                              > MACROS <
//----------------------------------------------------------------------------

//============================================================================
//                              > ENUMERATED TYPES <
//----------------------------------------------------------------------------
typedef enum Catalog_Tag
{
	MMC_BDPO08PCA = 0x01000001,
	MMC_BDP016PCA = 0x01000002,
	MMC_BDP032PCA = 0x01000003,
	MMC_BDP064PCA = 0x01000004,
}Catalog;
//============================================================================
//                              > STRUCTURES and TYPEDEFS <
//----------------------------------------------------------------------------

#pragma pack(push, 1)
// Class Attribute ALL
typedef struct ecatEepromClassAttrAll_tag
{
	UINT16 Revison;
	UINT16 MaxInstance;
	UINT16 MaxIDClassAttr;
	UINT16 MaxIDInstAttr;
}EcatEepromClassAttrAll, *PEcatEepromClassAttrAll;

// Instance Attribute All
typedef struct ecatEepromInstAttrAll_tag
{
	UINT16 FileSize;         
	UINT16 FileRevision;
	char   CatalogNum[16];
	UINT16 HWRevision;
	UINT32 ProductCode;
	UINT16 MaxAxes;
	UINT16 MaxIO;
	UINT8  MacAddr[6];
}EcatEepromInstAttrAll, *PEcatEepromInstAttrAll;

// Specific Services
typedef struct ecatEepromUpdateData_tag
{
	UINT32 ProductCode;
	char MacAddr[6];
}EcatEepromUpdateData;

#pragma pack(pop)
//============================================================================
//                              > FUNCTION PROTOTYPES <
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Attribute
//----------------------------------------------------------------------------
// All
extern "C" NMCMOTIONAPI MC_STATUS  EepromGetInstAttributeAll(
									UINT16 BoardID,  
									UINT16 Instance, 
									EcatEepromInstAttrAll * data
									);
// Single
//----------------------------------------------------------------------------
// Services
//----------------------------------------------------------------------------
//0x40
extern "C" NMCMOTIONAPI MC_STATUS  EepromUpdate(
									UINT16 BoardID, 
									UINT32 ProductCode, 
									UINT8  MacAddr[6]
									);
//============================================================================
//                              > INLINE FUNCTION DEFINITIONS <
//----------------------------------------------------------------------------

#endif

