/* samsung */

#ifndef DEFRFID_H
#define DEFRFID_H

#include "IComThread.h"

#define OneToOne
#define V600			


#ifdef OneToOne
const	int DEF_MAX_FRAME_MESSAGE_SIZE		= 256;
#endif


const	int DEF_MAX_COMMAND_MESSAGE_SIZE	= 256;


const	int DEF_INVALID_UNIT_NO				= -1;
const	int DEF_INVALID_COMMAND				= -1;
const	int DEF_INVALID_START_ADDRESS		= -1;
const	int DEF_INVALID_HEAD_NO				= -1;
const	int DEF_INVALID_READBYTE_NO			= -1;

const int DEF_RFID_SUCCESS						=	0;


/** Battery 부족 상태 */
const int DEF_BATTERY_STATE_SHORTAGE	= 0;
/** Battery 충분 상태 */
const int DEF_BATTERY_STATE_GOOD		= 1;




typedef enum tagEDataCodeSpec
{
	DEF_INVALID_CODE_SPEC,
	DEF_ASCII, // ASCII Code Type
	DEF_HEX,   // Hexa Code Type
	DEF_PROCESSING_RESULT_INQUIRY,
	DEF_CANCEL,
	DEF_COLLATION,
	DEF_CALCULATION,
}EDataCodeSpec;


typedef enum tagECommandType
{
	DEF_COMMAND_READ,
	DEF_COMMAND_WRITE,
	DEF_COMMAND_AUTO_READ,
	DEF_COMMAND_AUTO_WRITE,
	DEF_COMMAND_POLLING_AUTO_READ,
	DEF_COMMAND_POLLING_AUTO_WRITE,
	DEF_COMMAND_CANCEL,
	DEF_COMMAND_DATA,
	DEF_COMMAND_DATA_LIMIT,
	DEF_COMMAND_DATA_TEST,
	DEF_COMMAND_RESET,
	DEF_COMMAND_NOT_DEFINED,
}ECommandType;





#ifdef V600

/**
 * V600 Data Code Specification
 */	
const char DEF_ASCII_CODE[]									= "A";
const char DEF_HEX_CODE[]									= "H";
const char DEF_PROCESSING_RESULT_INQUIRY_CODE[]				= "C";
const char DEF_CANCEL_CODE[]								= "E";
const char DEF_COLLATION_CODE[]								= "C";
const char DEF_CALCULATION_CODE[]							= "K";

/**
 * V600 RFID Command Format
 */

const char	DEF_STX[]							= "@";
const char	DEF_ETX[]							= "*";
const int	DEF_CR								= 0xD;

/**
 * V600 RFID Command Format
 */
const char DEF_HEADER_READ[]					= "RD";
const char DEF_HEADER_WRITE[]					= "WT";
const char DEF_HEADER_POLLING_AUTO_READ[]		= "PR";
const char DEF_HEADER_POLLING_AUTO_WRITE[]		= "PW";
const char DEF_HEADER_AUTO_READ[]				= "AR";
const char DEF_HEADER_AUTO_WRITE[]				= "AW";
const char DEF_HEADER_COMMAND_CANCEL[]			= "AA";
const char DEF_HEADER_DATA_COMMAND[]			= "MD";
const char DEF_HEADER_MANAGEMENT_DATA_LIMIT[]	= "MDL";
const char DEF_HEADER_DATA_TEST_COMMAND[]		= "TS";
const char DEF_HEADER_RESET_COMMAND[]			= "XZ";
const char DEF_HEADER_COMMAND_NOT_DEFINED[]		= "IC";	



/**
 * Normal Completion
 */
const char DEF_NORMAL_COMPLETION[]								= "00";
/**
 * Host Communications Error
 */
const char DEF_PARITY_ERROR[]									= "10";
const char DEF_FRAMING_ERROR[]									= "11";
const char DEF_OVERRUN_ERROR[]									= "12";
const char DEF_FCS_ERROR[]										= "13";
const char DEF_COMMAND_INPUT_ERROR[]							= "14";
const char DEF_FRAME_LENGTH_MAXIMUM_ERROR[]						= "18";

const char DEF_DC_COMMUNICATION_ERROR[]							= "70";
const char DEF_DIS_AGREEMENT_ERROR[]							= "71";
const char DEF_DC_NOT_PRESENT_ERROR[]							= "72";
const char DEF_COMMAND_RECEIVED[]								= "74";
const char DEF_COMMAND_AUTO_PROCESSING_CANCELED_BEFOR_COMM[]	= "75";
const char DEF_COMMAND_AUTO_PROCESSING_CANCELED_AFTER_COMM[]	= "76";
const char DEF_ADDRESS_TOO_HIGH[]								= "7A";
const char DEF_LOWER_BATTERY[]									= "7B";
const char DEF_READ_WRITE_HEAD_NOT_CONNECTED[]					= "7C";
const char DEF_WRITE_PROTECT_ERROR[]							= "7D";

typedef enum tagECommandResult
{
	DEF_RESULT_INVALID_ERROR	= -1,
	DEF_RESULT_NORMAL_COMPLETION,
	DEF_RESULT_PARITY_ERROR,
	DEF_RESULT_FRAMING_ERROR,
	DEF_RESULT_OVERRUN_ERROR,
	DEF_RESULT_FCS_ERROR,
	DEF_RESULT_FORMAT_ERROR,
	DEF_RESULT_FRAMING_LENGTH_ERROR,
	DEF_RESULT_DC_COMM_ERROR,
	DEF_RESULT_DIS_AGREEMENT_ERROR,
	DEF_RESULT_DC_NOT_PRESENT_ERROR,
	DEF_RESULT_COMMAND_RECEIVED,
	DEF_RESULT_COMMAND_AUTO_PROCESSING_CANCELED_BEFOR_COMM,
	DEF_RESULT_COMMAND_AUTO_PROCESSING_CANCELED_AFTER_COMM,
	DEF_RESULT_ADDRESS_TOO_HIGH,
	DEF_RESULT_LOWER_BATTERY,
	DEF_RESULT_READ_WRITE_HEAD_NOT_CONNECTED,
	DEF_RESULT_WRITE_PROTECT_ERROR,
}ECommandResult;

#endif


#ifdef V700

/**
 * V700 Data Code Specification
 */
const char DEF_ASCII_CODE[]						= "A";
const char DEF_HEX_CODE[]						= "H";

/**
 * V700 RFID Command Format
 */

const char	DEF_STX[]							= "@";
const char	DEF_ETX[]							= "*";
const int	DEF_CR								= 0xD;

/**
 * V700 RFID Communications Command Header
 */
const char DEF_HEADER_READ[]					= "RD";
const char DEF_HEADER_WRITE[]					= "WT";
const char DEF_HEADER_POLLING_AUTO_READ[]		= "PR";
const char DEF_HEADER_POLLING_AUTO_WRITE[]		= "PW";
const char DEF_HEADER_POLLING_CHECK[]			= "PC";
const char DEF_HEADER_POLLING_CANCEL[]			= "PE";
const char DEF_HEADER_MEMORY_CHECK[]			= "MC";
const char DEF_HEADER_MEMORY_CALCULATION[]		= "MK";

/**
 * V700 RFID System Command Header
 */
const char DEF_HEADER_SYSTEM_READ[]				= "SR";
const char DEF_HEADER_SYSTEM_WRITE[]			= "SW";
const char DEF_HEADER_SYSTEM_LOCK[]				= "SL";
const char DEF_HEADER_EAS_SETTING[]				= "ES";
const char DEF_HEADER_QUIET_BIT_SETTING[]		= "QB";
const char DEF_HEADER_LOCK[]					= "LK";
const char DEF_HEADER_EAS_CHECK[]				= "EA";

/**
 * V700 RFID Read/Write Control Command
 */
const char DEF_HEADER_STOP[]					= "ST";
const char DEF_HEADER_RESET[]					= "XZ";
const char DEF_HEADER_ACK[]						= "AK";
const char DEF_HEADER_NACK[]					= "NK";
const char DEF_HEADER_IO_CONTROL[]				= "CC";
const char DEF_HEADER_TEST[]					= "TS";
const char DEF_HEADER_VERSION[]					= "VS";

/**
 * V700 RFID Setting Command
 */
const char DEF_HEADER_AFI_EN_DISABLE[]			= "AE";
const char DEF_HEADER_AFI_READING[]				= "AF";
const char DEF_HEADER_FAMILY_CODE_CHECKING[]	= "FC";
const char DEF_HEADER_APPLICATION_ID_SETTING[]	= "AI";
const char DEF_HEADER_UID_ADDITIONAL_SETTING[]	= "SN";
const char DEF_HEADER_NODE_NUMBER[]				= "NN";
const char DEF_HEADER_COM_PORT_SETTING[]		= "CP";
const char DEF_HEADER_COM_TYPE_SETTING[]		= "CT";
const char DEF_HEADER_TAG_COMMUNICATION_MODE[]	= "CM";
const char DEF_HEADER_ANTENNA_CHANGEOVER[]		= "AC";
const char DEF_HEADER_TERMINAL_RESISTANCE[]		= "TM";
const char DEF_HEADER_OFFLINE_MODE_SETTING[]	= "FL";
const char DEF_HEADER_INITIALIZATION_SETTING[]	= "IS";
const char DEF_HEADER_EEPROM_WRITING[]			= "EW";
const char DEF_HEADER_ICODE_CHANGE_OVER[]		= "TY";


/**
 * V700 RFID Response Command List
 */


const char DEF_MULTI_PROCESSING_END[]						= "72";
const char DEF_POLLING_COMMAND_RECEIVED[]					= "74";
const char DEF_POLLING_PROCESSING_CANCELED_DATA_NORMAL[]	= "75";
const char DEF_POLLING_PROCESSING_CANCELED_DATA_ERROR[]		= "76";


/**
 * Communications Error
 */
const char DEF_COMMUNICATIONS_ERROR[]						= "70";
const char DEF_WRITE_PROCESS_ERROR[]						= "71";
const char DEF_NO_TAG_ERROR[]								= "72";
const char DEF_COMMAND_ERROR[]								= "79";

/**
 * System Error
 */
const char DEF_MEMORY_ERROR[]								= "93";
/**
 * Warning
 */
const char DEF_SPECIFIED_NO_TAGS_EXCEEDED[]					= "01";
const char DEF_SPECIFIED_NO_RETRIES_EXCEEDED[]				= "02";
const char DEF_NO_TAGS_EXCEEDED[]							= "03";
const char DEF_COMM_ERROR[]									= "04";
const char DEF_SPECIFIED_NO_TAG_EXCEEDED_COMM_ERROR[]		= "05";
const char DEF_NO_TAG_EXCEEDED_COMM_ERROR[]					= "06";
const char DEF_NO_TAG_COMM_ERROR[]							= "07";


#endif

typedef struct{
/** Serial Component pointer */
    IComThread *m_plnkSerialPort;	
} SRFIDRefCompList;

typedef struct 
{
	int	m_iHeadNo;
} SRFIDData;


///////////////////////////////////////////////////////////////////////////////////////////////
const int DEF_MAX_RFID_ITEM = 8;
typedef enum tagZigItemType
{
	ZIGID,
	TOT_QTY_AFTER_REG,
	REG_QTY,
	REG_DATE,
	MATCODE,
	TOT_QTY_ZIG,
	ZIG_USED_QTY,
	REG_USED_QTY,
}EZigItemType;

typedef struct 
{
	EZigItemType	m_eItemType;
	int				m_iStartAddress;
	int				m_iEndAddress;
	int				m_iDataLength;
	EDataCodeSpec	m_eDataCodeSpec;
} SZigRFIDMap;

#endif //DEFRFID_H