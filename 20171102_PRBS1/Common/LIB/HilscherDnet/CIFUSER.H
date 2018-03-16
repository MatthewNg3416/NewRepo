/* <St> *******************************************************************

  FILENAME      : CIFUSER.H

  -------------------------------------------------------------------------
  CREATETED     : R. Mayer,  Hilscher GmbH
  DATE          : 28.11.95
  PROJEKT       : CIF device driver
  =========================================================================

  DISCRIPTION

    User interface definition.

  =========================================================================

  CHANGES

  version name        date        Discription
  -------------------------------------------------------------------------
  V1.021  MY          21.10.99    - Definition against multiple inclusion
                                    added

  V1.020  MY          22.03.99    - New Function DevDownload included
                                  - Definitions for download modes included

  V1.011  MY          20.02.98    - SPC_CONTROL_SET/CLEAR included

  V1.010  MY          28.10.97    - Modes in DevExchangeIOErr changed from
                                    0,1,2 to 2,3,4

  V1.009  MY          16.09.97    - MS-C++ support include (#ifdef _cplusplus)
                                  - DevPutMessage(), pvData renamed into
                                    ptMessage
                                  - Function: DevReadWriteRAW()
                                              DevExchangeIOEx()
                                              DevExchangeIOErr() included.
                                  - MSG.data in MSG_STRUCT reduced to
                                    255 Byte, which is the max. data length.
                                  - New reset definition BOOTSTART included,
                                    to save parameters on CIF40
                                  - PcWatchDog into HostWatchDog renamed
                                  - Error numbers -6, -26, -27 included
                                  - Reset mode BOOTSTART included
                                  - New handshake and definitions for
                                    state field transfer included

  V1.008  MY          05.06.97    - GETMESSAGECMD, size of message included,
                                    to prevent overwriting of user buffer

  V1.007  MY          05.06.97    - static definition of the
                                    IOSEND and IORECEIVE data area removed
                                  - BOARD_INFOEX structure and function
                                    DevGetBoardInfoEx included

  V1.006  MY          25.04.97    - function DevGetExtendedInfo included

  V1.005  MY          30.11.96    - function DevExtendedData, DevGetMBXData
                                    included
                                  - error -25 included

  V1.004  MY          18.06.96    - IO function IOCTLREADSEND
                                    included

  V1.003  MY          25.04.96    - IO function IOCTLEXIO
                                                IOGETPARAM
                                                IOSETHOST
                                    included

  V1.002  MY          28.02.96    - function DevGetMBXState included
                                  - Variable name MSG_STRUC.daten
                                    changed into data

  V1.001  MY          31.01.96    - user interface changed

  V1.000  MY


  ******************************************************************** <En> */

#ifndef _DNET_IO
#define _DNET_IO

//#include "rcs_user.h"       /* Include file for RCS definition */
//#include "ASC_USER.h"
//#include "CIFUSER.H"

#define	MIO_OFF		0
#define	MIO_ON		1
#define	MIO_ERROR	256

#define CONFIG_SIZE	63
#define MAX_DEVICE	64

#define ON		1
#define OFF		0

const BYTE bitOnMask[8] =
	{
		0x01,  //0000 0001
		0x02,  //0000 0010
		0x04,  //0000 0100
		0x08,  //0000 1000
		0x10,  //0001 0000
		0x20,  //0010 0000
		0x40,  //0100 0000
		0x80   //1000 0000
	};

const BYTE bitOffMask[8] =
	{
		0xFE,  //1111 1110
		0xFD,  //1111 1101
		0xFB,  //1111 1011
		0xF7,  //1111 0111
		0xEF,  //1110 1111
		0xDF,  //1101 1111
		0xBF,  //1011 1111
		0x7F   //0111 1111
	};

//--------------------------
// Device config data
//--------------------------
typedef struct
{
	WORD MacId;
	WORD Output1Size;
	WORD Output1Offset;
	WORD Input1Size;
	WORD Input1Offset;
} DNS_DEVICE_CFG;

////////////////////////////////////////////////////////////////
//rcs_user.h
////////////////////////////////////////////////////////////////
/* <St> *******************************************************************

  FILENAME    : RCS_USER.H

  -------------------------------------------------------------------------
  CREATED BY  : R.Mayer,  Hilscher GmbH
  CREATED AT  : 10.05.96
  PROJECT     : global
  =========================================================================

  FUNCTION :
    General RCS definitions

  =========================================================================

  CHANGES OF REVISIONS :

  Version Name      Date      Change
  -------------------------------------------------------------------------

  V1.003  Mayer     10.03.99  fault in #ifdef __cplusplus fixed

  V1.002  Mayer     14.05.98  RCS_TELEGRAMHEADERDATA_10 structure included

  V1.001  Mayer     29.05.96  Task errors included

  V1.000  Mayer     10.05.96  created from the file rc090203.h

  ******************************************************************** <En> */

#ifdef __cplusplus
//{
#endif

#if defined( _MSC_VER)          /* Microsoft C */
    #pragma pack(1)             /* Byte Alignment   */
#endif

/* ======================================================================== */
/* General task errors                                                      */
/* ======================================================================== */

#define TASK_F_OK                              0
#define TASK_F_NO_COMMUNICATION                1
#define TASK_F_IDLE                            2

#define TASK_F_INIT_BASE                       50

#define TASK_F_PARITY                          100
#define TASK_F_FRAMING                         101
#define TASK_F_OVERRUN                         102
#define TASK_F_DATACOUNT                       103
#define TASK_F_CHECKSUM                        104
#define TASK_F_TIMEOUT                         105
#define TASK_F_PROTOCOL                        106
#define TASK_F_DATA                            107
#define TASK_F_NACK                            108

#define TASK_F_PROTOCOL_BASE                   110

#define TASK_F_MESSAGEHEADER                   150
#define TASK_F_MESSAGESIZE                     151
#define TASK_F_MESSAGECOMMAND                  152
#define TASK_F_MESSAGESTRUCTURE                153
#define TASK_F_MESSAGEERROR                    154
#define TASK_F_MESSAGETIMEOUT                  155

#define TASK_F_TELEGRAMHEADER                  160
#define TASK_F_DEVICE_ADR                      161
#define TASK_F_DATA_AREA                       162
#define TASK_F_DATA_ADR                        163
#define TASK_F_DATA_IDX                        164
#define TASK_F_DATA_CNT                        165
#define TASK_F_DATA_TYPE                       166
#define TASK_F_FUNCTION                        167

#define TASK_F_MESSAGE_BASE                    170

#define TASK_F_NOT_INITIALIZED                 200
#define TASK_F_BUSY                            201
#define TASK_F_SEGMENT                         202
#define TASK_F_USER                            203

#define TASK_F_DATABASE                        210
#define TASK_F_DATABASE_WRITE                  211
#define TASK_F_DATABASE_READ                   212
#define TASK_F_STRUCTURE                       213
#define TASK_F_PARAMETER                       214
#define TASK_F_CONFIGURATION                   215
#define TASK_F_FUNCTIONLIST                    216
#define TASK_F_SYSTEM                          217

#define TASK_F_SYSTEM_BASE                     220


/* ======================================================================== */
/* Task commands                                                            */
/* ======================================================================== */

#define TASK_B_10                           16     /* RCS_TELEGRAMHEADER_10 */
#define TASK_B_11                           17     /* Task specific         */


/* ======================================================================== */
/* Message definitions                                                      */
/* ======================================================================== */

/* ----------------- Definition of the standard telegram header ----------- */
/* Keyword: MESSAGE    ---------------------------------------------------- */
/* TelegramFunCtion */

#define TASK_TFC_UNUSED                        0
#define TASK_TFC_READ                          1
#define TASK_TFC_WRITE                         2
#define TASK_TFC_QUERRY                        3

/* TelegramDataArea */
/* 'data_area'      */

#define TASK_TDA_UNUSED                        0
#define TASK_TDA_BIT                           1
#define TASK_TDA_BYTE                          2
#define TASK_TDA_WORD                          3
#define TASK_TDA_DWORD                         4
#define TASK_TDA_FLOAT                         5

/* TelegramDataType */
/* 'data_type' for MOTOROLA data types !                                    */
/* For INTEL data types, the 'RCS_TDT_IDF_MASK' flag must be set            */

#define TASK_TDT_UNUSED                        0
#define TASK_TDT_BOOLEAN                       1
#define TASK_TDT_INT8                          2
#define TASK_TDT_INT16                         3
#define TASK_TDT_INT32                         4
#define TASK_TDT_UINT8                         5
#define TASK_TDT_UINT16                        6
#define TASK_TDT_UINT32                        7
#define TASK_TDT_FLOAT                         8
#define TASK_TDT_ASCII                         9
#define TASK_TDT_STRING                        10
#define TASK_TDT_BIT                           14

#define TASK_TDT_IDF_MSK                     0X80

/* ======================================================================== */
/* Message structure definitions                                            */
/* ======================================================================== */

/* -------------------- General  RCS definitions -------------------------- */
/* Keyword: MESSAGE  ------------------------------------------------------ */

#define RCS_SEGMENT_LEN                        288
#define RCS_MESSAGEHEADER_LEN                  8
#define RCS_TELEGRAMHEADER_LEN                 8

/* ------------------------ RCS message definition ------------------------ */

typedef struct RCS_MESSAGEHEADERtag {
   unsigned char        rx;             /* receiver                         */
   unsigned char        tx;             /* transmitter                      */
   unsigned char        ln;             /* lenght                           */
   unsigned char        nr;             /* number                           */
   unsigned char        a;              /* answer                           */
   unsigned char        f;              /* fault                            */
   unsigned char        b;              /* command                          */
   unsigned char        e;              /* extension                        */
} RCS_MESSAGEHEADER;

typedef struct RCS_MESSAGEtag {
   unsigned char        rx;             /* receiver                         */
   unsigned char        tx;             /* transmitter                      */
   unsigned char        ln;             /* lenght                           */
   unsigned char        nr;             /* number                           */
   unsigned char        a;              /* answer                           */
   unsigned char        f;              /* fault                            */
   unsigned char        b;              /* command                          */
   unsigned char        e;              /* extension                        */
   unsigned char        d[ RCS_SEGMENT_LEN-RCS_MESSAGEHEADER_LEN ]; /* data */
} RCS_MESSAGE;


/* ----------------- Standard telegram header ----------------------------- */
/* Keyword: MESSAGE, TASK_B_10  --------------------------------------------*/

typedef struct RCS_TELEGRAMHEADER_10tag {
   unsigned char        device_adr;     /* device address                   */
   unsigned char        data_area;      /* data area                        */
   unsigned short       data_adr;       /* data address                     */
   unsigned char        data_idx;       /* data index                       */
   unsigned char        data_cnt;       /* data count                       */
   unsigned char        data_type;      /* data type                        */
   unsigned char        function;       /* function                         */
} RCS_TELEGRAMHEADER_10;

typedef struct RCS_MESSAGETELEGRAMHEADER_10_tag {
   unsigned char        rx;             /* receiver                         */
   unsigned char        tx;             /* transmitter                      */
   unsigned char        ln;             /* lenght                           */
   unsigned char        nr;             /* number                           */
   unsigned char        a;              /* answer                           */
   unsigned char        f;              /* fault                            */
   unsigned char        b;              /* command                          */
   unsigned char        e;              /* extension                        */
   unsigned char        device_adr;     /* device address                   */
   unsigned char        data_area;      /* data area                        */
   unsigned short       data_adr;       /* data address                     */
   unsigned char        data_idx;       /* data index                       */
   unsigned char        data_cnt;       /* data count                       */
   unsigned char        data_type;      /* data type                        */
   unsigned char        function;       /* function                         */
} RCS_MESSAGETELEGRAMHEADER_10;

typedef struct RCS_TELEGRAMHEADERDATA_10tag {
   unsigned char        device_adr;
   unsigned char        data_area;
   unsigned short       data_adr;
   unsigned char        data_idx;
   unsigned char        data_cnt;
   unsigned char        data_type;
   unsigned char        function;
   unsigned char        d[ RCS_SEGMENT_LEN-RCS_MESSAGEHEADER_LEN-RCS_TELEGRAMHEADER_LEN];
} RCS_TELEGRAMHEADERDATA_10;

typedef struct RCS_MESSAGETELEGRAM_10tag {
   unsigned char        rx;             /* receiver                         */
   unsigned char        tx;             /* transmitter                      */
   unsigned char        ln;             /* lenght                           */
   unsigned char        nr;             /* number                           */
   unsigned char        a;              /* answer                           */
   unsigned char        f;              /* fault                            */
   unsigned char        b;              /* command                          */
   unsigned char        e;              /* extension                        */
   unsigned char        device_adr;     /* device address                   */
   unsigned char        data_area;      /* data area                        */
   unsigned short       data_adr;       /* data address                     */
   unsigned char        data_idx;       /* data index                       */
   unsigned char        data_cnt;       /* data count                       */
   unsigned char        data_type;      /* data type                        */
   unsigned char        function;       /* function                         */
   unsigned char        d[ RCS_SEGMENT_LEN-RCS_MESSAGEHEADER_LEN-RCS_TELEGRAMHEADER_LEN];
} RCS_MESSAGETELEGRAM_10;


#if defined( _MSC_VER)          /* Microsoft C */
    #pragma pack()              /* Byte Alignment   */
#endif

#ifdef __cplusplus
//}
#endif

/* === eof 'RCS_USER.H' === */

////////////////////////////////////////////////////////////////
//ASC_USER.h
////////////////////////////////////////////////////////////////
/* <St> *******************************************************************

  FILENAME    : ASC_USER.H

  -------------------------------------------------------------------------
  CREATED BY  : R. Mayer,  Hilscher GmbH
  CREATED AT  : 29.05.96
  PROJECT     : ASC
  =========================================================================

  FUNCTION :
    User interface ASCII protocol

  =========================================================================

  CHANGES OF REVISIONS :

  Version Name      Date      Change
  -------------------------------------------------------------------------

  V1.000  Mayer     29.05.96  Created

  ******************************************************************** <En> */

#if defined( _MSC_VER)          /* Microsoft C */
    #pragma pack(1)             /* Byte Alignment   */
#endif

/* ======================================================================== */
/* Protocol definition                                                      */
/* ======================================================================== */

/* Mode */

#define ASC_MODE_SLAVE          0
#define ASC_MODE_MASTER         1

/* Stop mode */

#define ASC_ENDMODE_TIMEOUT     0       /* Timeout                          */
#define ASC_ENDMODE_ID          1       /* Identifier                       */
#define ASC_ENDMODE_QUITT       2       /* Acknowledg telegram              */
#define ASC_ENDMODE_ID_QUITT    3       /* Identifier / Acknowledg telegram */
#define ASC_ENDMODE_NUMBER      4       /* Character number                 */
#define ASC_ENDMODE_DEFNUMBER   5       /* Predefined character number      */

/* Checksum */

#define ASC_CHKSUM_NONE         0
#define ASC_CHKSUM_BIN7         1
#define ASC_CHKSUM_BIN8         2
#define ASC_CHKSUM_BCC          3
#define ASC_CHKSUM_BCC_ASCII    4

/* Checksum area */

#define ASC_CHK_AREA_DATA_ONLY    0     /* Data only                        */
#define ASC_CHK_AREA_DATA_STARTID 1     /* Data with start identifier       */
#define ASC_CHK_AREA_DATA_ENDID   2     /* Data with stop identifier        */
#define ASC_CHK_AREA_FULL         3     /* Full telegram                    */

/* Filter */

#define ASC_FILTER_NONE         0
#define ASC_FILTER_DOUBLE_CHAR  1

/* Timeouts */

#define ASC_TIMEOUT_NONE        0

/* Telegram definition */

#define ASC_TELEGRAM_LENGTH_ZERO  0

/* Telegram sequenz */

#define ASC_TELEGRAM_SEQUENZ_NONE 0


/* ======================================================================== */
/* Protocol parameter structure                                             */
/* ======================================================================== */

typedef struct ASC_PARAMETRtag {
  unsigned char         bScl;           /* Communication line number        */
  unsigned char         bRtsControl;    /* RTS control                      */
  unsigned char         bBaudrate;      /* Baudrate                         */
  unsigned char         bDataBits;      /* Number of data bits              */
  unsigned char         bStopBits;      /* Number of stop bits              */
  unsigned char         bParityBit;     /* Parity                           */
  unsigned char         bMode;          /* Mode                             */
  unsigned char         bEndMode;       /* End mode                         */
  unsigned char         bCheckMode;     /* Check mode                       */
  unsigned char         bCheckArea;     /* Check area                       */
  unsigned char         bFilterMode;    /* Filter mode                      */
  unsigned short        usFilterCharacter; /* Filter characters             */
  unsigned short        usTelTimeout;   /* Telegram timeout                 */
  unsigned short        usStartTimeout; /* Telegram start timeout           */
  unsigned short        usCharTimeout;  /* Charater timeout                 */
  unsigned char         bRetries;       /* Telegram retries                 */
  unsigned char         bErrorLed;      /* Mode of the error LED            */
  unsigned char         bTelStartLen;   /* Telegram start length            */
  unsigned char         bTelStart[8];   /* Start telegram                   */
  unsigned char         bTelEndLen;     /* End telegram length              */
  unsigned char         bTelEnd[8];     /* End telegram                     */
  unsigned char         bTelAckLen;     /* ACK telegram length              */
  unsigned char         bTelAck[8];     /* ACK telegram                     */
  unsigned char         bTelNackLen;    /* NACK telegram length             */
  unsigned char         bTelNack[8];    /* NACK telegram                    */
  unsigned char         bTelDeviceLen;  /* Device telegram length           */
  unsigned short        usTelFollowTime;/* Telegram following time          */
} ASC_PARAMETER;

/* ======================================================================== */
/* Protocol task state structure                                            */
/* ======================================================================== */

typedef struct ASC_STATEtag {
  unsigned char         bTaskState;     /* Task state                       */
  unsigned long         ulTxCount;      /* Transmitt telegram count         */
  unsigned long         ulRxCount;      /* Receive telegram count           */
  unsigned short        usTxErrorCount; /* Transmitt error count            */
  unsigned short        usRxErrorCount; /* Receive error count              */
  unsigned short        usErrorBits;    /* Error bits                       */
  unsigned char         bError;         /* Last error                       */
} ASC_STATE;


#if defined( _MSC_VER)          /* Microsoft C */
    #pragma pack()              /* Byte Alignment   */
#endif

#endif // ifndef CIFUSER_H_INCLUDED
/* === eof 'USER.H' === */
////////////////////////////////////////////////////////////////
//CIFUSER.H
////////////////////////////////////////////////////////////////
/* <St> *******************************************************************

  FILENAME      : CIFUSER.H

  -------------------------------------------------------------------------
  CREATETED     : R. Mayer,  Hilscher GmbH
  DATE          : 28.11.95
  PROJEKT       : CIF device driver
  =========================================================================

  DISCRIPTION

    User interface definition.

  =========================================================================

  CHANGES

  version name        date        Discription
  -------------------------------------------------------------------------
  V1.021  MY          21.10.99    - Definition against multiple inclusion
                                    added

  V1.020  MY          22.03.99    - New Function DevDownload included
                                  - Definitions for download modes included

  V1.011  MY          20.02.98    - SPC_CONTROL_SET/CLEAR included

  V1.010  MY          28.10.97    - Modes in DevExchangeIOErr changed from
                                    0,1,2 to 2,3,4

  V1.009  MY          16.09.97    - MS-C++ support include (#ifdef _cplusplus)
                                  - DevPutMessage(), pvData renamed into
                                    ptMessage
                                  - Function: DevReadWriteRAW()
                                              DevExchangeIOEx()
                                              DevExchangeIOErr() included.
                                  - MSG.data in MSG_STRUCT reduced to
                                    255 Byte, which is the max. data length.
                                  - New reset definition BOOTSTART included,
                                    to save parameters on CIF40
                                  - PcWatchDog into HostWatchDog renamed
                                  - Error numbers -6, -26, -27 included
                                  - Reset mode BOOTSTART included
                                  - New handshake and definitions for
                                    state field transfer included

  V1.008  MY          05.06.97    - GETMESSAGECMD, size of message included,
                                    to prevent overwriting of user buffer

  V1.007  MY          05.06.97    - static definition of the
                                    IOSEND and IORECEIVE data area removed
                                  - BOARD_INFOEX structure and function
                                    DevGetBoardInfoEx included

  V1.006  MY          25.04.97    - function DevGetExtendedInfo included

  V1.005  MY          30.11.96    - function DevExtendedData, DevGetMBXData
                                    included
                                  - error -25 included

  V1.004  MY          18.06.96    - IO function IOCTLREADSEND
                                    included

  V1.003  MY          25.04.96    - IO function IOCTLEXIO
                                                IOGETPARAM
                                                IOSETHOST
                                    included

  V1.002  MY          28.02.96    - function DevGetMBXState included
                                  - Variable name MSG_STRUC.daten
                                    changed into data

  V1.001  MY          31.01.96    - user interface changed

  V1.000  MY


  ******************************************************************** <En> */

/* prevent multiple inclusion */
#ifndef CIFUSER_H_INCLUDED
  #define CIFUSER_H_INCLUDED

#ifdef __cplusplus
  extern "C" {
#endif  /* _cplusplus */

/* ------------------------------------------------------------------------------------ */
/*  global definitions                                                                  */
/* ------------------------------------------------------------------------------------ */

#define MAX_DEV_BOARDS              4       // maximum numbers of boards

/* ------------------------------------------------------------------------------------ */
/*  driver errors                                                                       */
/* ------------------------------------------------------------------------------------ */

#define DRV_NO_ERROR                 0      // no error                                            
#define DRV_BOARD_NOT_INITIALIZED   -1      // DRIVER Board not initialized                        
#define DRV_INIT_STATE_ERROR        -2      // DRIVER Error in internal init state                 
#define DRV_READ_STATE_ERROR        -3      // DRIVER Error in internal read state                 
#define DRV_CMD_ACTIVE              -4      // DRIVER Command on this channel is activ             
#define DRV_PARAMETER_UNKNOWN       -5      // DRIVER Unknown parameter in function occured        
#define DRV_WRONG_DRIVER_VERSION    -6      // DRIVER Version is incompatible with DLL             
                                               
#define DRV_PCI_SET_CONFIG_MODE     -7      // DRIVER Error during PCI set run mode                
#define DRV_PCI_READ_DPM_LENGTH     -8      // DRIVER Could not read PCI dual port memory length   
#define DRV_PCI_SET_RUN_MODE        -9      // DRIVER Error during PCI set run mode                                                                  
                                            
#define DRV_DEV_DPM_ACCESS_ERROR    -10     // DEVICE Dual port ram not accessable(board not found)
#define DRV_DEV_NOT_READY           -11     // DEVICE Not ready (ready flag failed)                
#define DRV_DEV_NOT_RUNNING         -12     // DEVICE Not running (running flag failed)            
#define DRV_DEV_WATCHDOG_FAILED     -13     // DEVICE Watchdog test failed                         
#define DRV_DEV_OS_VERSION_ERROR    -14     // DEVICE Signals wrong OS version                     
#define DRV_DEV_SYSERR              -15     // DEVICE Error in dual port flags                     
#define DRV_DEV_MAILBOX_FULL        -16     // DEVICE Send mailbox is full                         
#define DRV_DEV_PUT_TIMEOUT         -17     // DEVICE PutMessage timeout                           
#define DRV_DEV_GET_TIMEOUT         -18     // DEVICE GetMessage timeout                           
#define DRV_DEV_GET_NO_MESSAGE      -19     // DEVICE No message available                         
#define DRV_DEV_RESET_TIMEOUT       -20     // DEVICE RESET command timeout                        
#define DRV_DEV_NO_COM_FLAG         -21     // DEVICE COM-flag not set                             
#define DRV_DEV_EXCHANGE_FAILED     -22     // DEVICE IO data exchange failed                      
#define DRV_DEV_EXCHANGE_TIMEOUT    -23     // DEVICE IO data exchange timeout                     
#define DRV_DEV_COM_MODE_UNKNOWN    -24     // DEVICE IO data mode unknown                         
#define DRV_DEV_FUNCTION_FAILED     -25     // DEVICE Function call failed                         
#define DRV_DEV_DPMSIZE_MISMATCH    -26     // DEVICE DPM size differs from configuration          
#define DRV_DEV_STATE_MODE_UNKNOWN  -27     // DEVICE State mode unknown

// Error from Interface functions
#define DRV_USR_OPEN_ERROR          -30     // USER Driver not opened (device driver not loaded)   
#define DRV_USR_INIT_DRV_ERROR      -31     // USER Can't connect with device                      
#define DRV_USR_NOT_INITIALIZED     -32     // USER Board not initialized (DevInitBoard not called)
#define DRV_USR_COMM_ERR            -33     // USER IOCTRL function failed                         
#define DRV_USR_DEV_NUMBER_INVALID  -34     // USER Parameter DeviceNumber  invalid                
#define DRV_USR_INFO_AREA_INVALID   -35     // USER Parameter InfoArea unknown                     
#define DRV_USR_NUMBER_INVALID      -36     // USER Parameter Number invalid                       
#define DRV_USR_MODE_INVALID        -37     // USER Parameter Mode invalid                         
#define DRV_USR_MSG_BUF_NULL_PTR    -38     // USER NULL pointer assignment                        
#define DRV_USR_MSG_BUF_TOO_SHORT   -39     // USER Message buffer too short                       
#define DRV_USR_SIZE_INVALID        -40     // USER Parameter Size invalid                         
#define DRV_USR_SIZE_ZERO           -42     // USER Parameter Size with zero length               
#define DRV_USR_SIZE_TOO_LONG       -43     // USER Parameter Size too long                        
#define DRV_USR_DEV_PTR_NULL        -44     // USER Device address null pointer                    
#define DRV_USR_BUF_PTR_NULL        -45     // USER Pointer to buffer is a null pointer            
                                                
#define DRV_USR_SENDSIZE_TOO_LONG   -46     // USER Parameter SendSize too long                    
#define DRV_USR_RECVSIZE_TOO_LONG   -47     // USER Parameter ReceiveSize too long                 
#define DRV_USR_SENDBUF_PTR_NULL    -48     // USER Pointer to send buffer is a null pointer       
#define DRV_USR_RECVBUF_PTR_NULL    -49     // USER Pointer to receive buffer is a null pointer 

#define DRV_USR_FILE_OPEN_FAILED    -100    // USER file not opend
#define DRV_USR_FILE_SIZE_ZERO      -101    // USER file size zero
#define DRV_USR_FILE_NO_MEMORY      -102    // USER not enough memory to load file
#define DRV_USR_FILE_READ_FAILED    -103    // USER file read failed
#define DRV_USR_INVALID_FILETYPE    -104    // USER file type invalid
#define DRV_USR_FILENAME_INVALID    -105    // USER file name not valid

#define DRV_RCS_ERROR_OFFSET       1000     // RCS error number start


/* ------------------------------------------------------------------------------------ */
/*  message definition                                                                  */
/* ------------------------------------------------------------------------------------ */

#pragma pack(1)

// max. length is 288 Bytes, max message length is 255 + 8 Bytes
typedef struct tagMSG_STRUC {
  unsigned char   rx;
  unsigned char   tx;
  unsigned char   ln;
  unsigned char   nr;
  unsigned char   a;
  unsigned char   f;
  unsigned char   b;
  unsigned char   e;
  unsigned char   data[255];
  unsigned char   dummy[25];      // for compatibility with older definitions (288 Bytes)
} MSG_STRUC;

#pragma pack()

/* ------------------------------------------------------------------------------------ */
/*  INFO structure definitions                                                          */
/* ------------------------------------------------------------------------------------ */

// DEVRESET

#define COLDSTART           2
#define WARMSTART           3
#define BOOTSTART           4

// DEVMBXINFO

#define DEVICE_MBX_EMPTY    0
#define DEVICE_MBX_FULL     1
#define HOST_MBX_EMPTY      0
#define HOST_MBX_FULL       1

// TRIGGERWATCHDOG

#define WATCHDOG_STOP       0
#define WATCHDOG_START      1

// GETINFO InfoArea definitions

#define GET_DRIVER_INFO     1
#define GET_VERSION_INFO    2
#define GET_FIRMWARE_INFO   3
#define GET_TASK_INFO       4
#define GET_RCS_INFO        5
#define GET_DEV_INFO        6
#define GET_IO_INFO         7
#define GET_IO_SEND_DATA    8

// HOST mode definition

#define HOST_NOT_READY      0
#define HOST_READY          1

// DEVREADWRITERAW
// DEVREADWRITEDPMDATA
#define PARAMETER_READ      1
#define PARAMETER_WRITE     2

// STATE definition

#define STATE_ERR_NON       0
#define STATE_ERR           1

#define STATE_MODE_2        2
#define STATE_MODE_3        3
#define STATE_MODE_4        4

// DEVSPECIALCONTROL

#define SPECIAL_CONTROL_CLEAR   0
#define SPECIAL_CONTROL_SET     1

// DEVDOWNLOAD

#define FIRMWARE_DOWNLOAD         1
#define CONFIGURATION_DOWNLOAD    2

#pragma pack(1)

// Device exchange IO information
typedef struct tagIOINFO {
  unsigned char   bComBit;                /* Actual state of the COM bit                */

  unsigned long   ulIOExchangeCnt;
  unsigned char   bIOExchangeMode;        /* Actual data exchange mode (0..5)           */


/* Exchange IO counter                        */
} IOINFO;

// Device version information
typedef struct tagVERSIONINFO {           /* DEV serial number and OS versions          */
  unsigned long   ulDate;
  unsigned long   ulDeviceNo;
  unsigned long   ulSerialNo;
  unsigned long   ulReserved;
  unsigned char   abPcOsName0[4];
  unsigned char   abPcOsName1[4];
  unsigned char   abPcOsName2[4];
  unsigned char   abOemIdentifier[4];
} VERSIONINFO;

// Device firmware information
typedef struct tagFIRMWAREINFO {
  unsigned char   abFirmwareName[16];     /* Firmware name                              */
  unsigned char   abFirmwareVersion[16];  /* Firmware version                           */
} FIRMWAREINFO;

// Device task state information
typedef struct tagTASKSTATE {
  unsigned char   abTaskState[64];        /* Task state field                           */
} TASKSTATE;

// Device task paramater data
typedef struct tagTASKPARAM {
  unsigned char   abTaskParameter[64];    /* Task parameter field                       */
} TASKPARAM;

// Device raw data structure
typedef struct tagRAWDATA {
  unsigned char   abRawData[1022];        /* Definition of the last kByte               */
} RAWDATA;

// Device task information
typedef struct tagTASKINFO {
  struct  {
    unsigned char  abTaskName[8];         /* Task name                                  */
    unsigned short usTaskVersion;         /* Task version                               */
    unsigned char  bTaskCondition;        /* Actual task condition                      */
    unsigned char  abreserved[5];         /* n.c.                                       */
  } tTaskInfo [7];
} TASKINFO;

// Device operating system (RCS) information
typedef struct tagRCSINFO {
  unsigned short usRcsVersion;            /* Device operating system (RCS) version      */
  unsigned char  bRcsError;               /* Operating system errors                    */
  unsigned char  bHostWatchDog;           /* Host watchdog value                        */
  unsigned char  bDevWatchDog;            /* Device watchdog value                      */
  unsigned char  bSegmentCount;           /* RCS segment free counter                   */
  unsigned char  bDeviceAdress;           /* RCS device base address                    */
  unsigned char  bDriverType;             /* RCS driver type                            */
} RCSINFO;

// Device description
typedef struct tagDEVINFO {
  unsigned char  bDpmSize;                /* Device dpm size (2,8...)                   */
  unsigned char  bDevType;                /* Device type  (manufactor code)             */
  unsigned char  bDevModel;               /* Device model (manufactor code)             */
  unsigned char  abDevIdentifier[3];      /* Device identification characters           */
} DEVINFO;

#pragma pack()

/* ------------------------------------------------------------------------------------ */
/*  driver info structure definitions                                                   */
/* ------------------------------------------------------------------------------------ */

#pragma pack(1)

// Board information structure
typedef struct tagBOARD_INFO{
  unsigned char abDriverVersion[16];  // DRV driver information string
  struct {
    unsigned short usBoardNumber;     // DRV board number
    unsigned short usAvailable;       // DRV board is available
    unsigned long  ulPhysicalAddress; // DRV physical DPM address
    unsigned short usIrqNumber;       // DRV irq number
  } tBoard [MAX_DEV_BOARDS];
} BOARD_INFO;

// Internal driver state information structure
typedef struct tagDRIVERINFO{
  unsigned long ulOpenCnt;            // DevOpen() counter
  unsigned long ulCloseCnt;           // DevClose() counter
  unsigned long ulReadCnt;            // Number of DevGetMessage commands
  unsigned long ulWriteCnt;           // Number of DevPutMessage commands
  unsigned long ulIRQCnt;             // Number of board interrupts
  unsigned char bInitMsgFlag;         // Actual init sate
  unsigned char bReadMsgFlag;         // Actual read mailbox state
  unsigned char bWriteMsgFlag;        // Actual write mailbox state
  unsigned char bLastFunction;        // Last driver function
  unsigned char bWriteState;          // Actual write command state
  unsigned char bReadState;           // Actual read command state
  unsigned char bHostFlags;           // Actual host flags
  unsigned char bMyDevFlags;          // Actual device falgs
  unsigned char bExIOFlag;            // Actual IO flags
  unsigned long ulExIOCnt;            // DevExchangeIO() counter
} DRIVERINFO;

// Extended board information structure
typedef struct tagBOARD_INFOEX{
  unsigned char     abDriverVersion[16];    // DRV driver information string
  struct {
    unsigned short  usBoardNumber;    // DRV board number
    unsigned short  usAvailable;      // DRV board is available
    unsigned long   ulPhysicalAddress;// DRV physical DPM address
    unsigned short  usIrqNumber;      // DRV irq number
    DRIVERINFO      tDriverInfo;      // Driver information
    FIRMWAREINFO    tFirmware;
    DEVINFO         tDeviceInfo;
    RCSINFO         tRcsInfo;
    VERSIONINFO     tVersion;
  } tBoard [MAX_DEV_BOARDS];
} BOARD_INFOEX;

// Communication state field structure
typedef struct tagCOMSTATE {
  unsigned short    usMode;           // Actual STATE mode
  unsigned short    usStateFlag;      // State flag
  unsigned char     abState[64];      // State area
} COMSTATE;

#pragma pack()

// state information in bLastFunction
#define  FKT_OPEN       1;
#define  FKT_CLOSE      2;
#define  FKT_READ       3;
#define  FKT_WRITE      4;
#define  FKT_IO         5;
// state information in bWriteState and bReadState
#define  STATE_IN       0x01;
#define  STATE_WAIT     0x02;
#define  STATE_OUT      0x03;
#define  STATE_IN_IRQ   0x04;

/* ------------------------------------------------------------------------------------ */
/*  funcion prototypes                                                                  */
/* ------------------------------------------------------------------------------------ */
/**
 * Checks if the device driver is available and opens a link to it.
 * @param usDevNumber : Always 0
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevOpenDriver        ( unsigned short usDevNumber);

/*
 * Closes an open link to the device driver
 * @param usDevNumber : Always 0
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevCloseDriver       ( unsigned short usDevNumber);

/*
 * Read gloal board information
 * @param usDevNumber : Always 0
 * @param usSize : Size of the users data buffer and length of data to be read
 * @param pvData : Pointer to the users data buffer
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevGetBoardInfo      ( unsigned short usDevNumber,
                                      unsigned short usSize,
                                      void           *pvData);

/*
 * Link an application to a board
 * @param usDevNumber : Board number (0..3)
 * @param pDevAddress : Pointer to the physical board address
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevInitBoard         ( unsigned short usDevNumber,
                                      void           *pDevAddress);

/*
 * Closes a link to a board
 * @param usDevNumber : Board number (0..3)
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevExitBoard         ( unsigned short usDevNumber);

/*
 * Writes the parameters for a communication task
 * @param usDevNumber : Board number (0..3)
 * @param usNumber : Number o fthe parameter area (1..7)
 * @param usSize : Size of the parameter area and length of daya to be put
 * @param pvData : Pointer to the users task parameters
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevPutTaskParameter  ( unsigned short usDevNumber,
                                      unsigned short usNumber,
                                      unsigned short usSize,
                                      void           *pvData);
/*
 * Reset a board
 * @param usDevNumber : Board number (0..3)
 * @param usMode : 2 = COLDSTART (new initializing), 3 = WARMSTART (initializing with parameters), 4 = BOOTSTART (switches the board into bootstrap loader mode. COM modules uses this mode to store user parameters)
 * @param ulTimeout : 3 ~ 10sec for COLDSTART and 2 ~ 8sec for WARMSTART
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevReset             ( unsigned short usDevNumber,
                                      unsigned short usMode,
                                      unsigned long  ulTimeout);

/*
 * Transfer a message to the board
 * @param usDevNumber : Board number (0..3)
 * @param ptMessage : Pointer to the messge data
 * @param ulTimeout : 3 ~ 10sec for COLDSTART and 2 ~ 8sec for WARMSTART
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevPutMessage        ( unsigned short usDevNumber,
                                      MSG_STRUC      *ptMessage,
                                      unsigned long  ulTimeout);

/*
 * Read a message to the board
 * @param usDevNumber : Board number (0..3)
 * @param usSize : Size of the users data buffer (maximum length to be read)
 * @param ptMessage : Pointer to the users data area
 * @param ulTimeout : 3 ~ 10sec for COLDSTART and 2 ~ 8sec for WARMSTART
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevGetMessage        ( unsigned short usDevNumber,
                                      unsigned short usSize,
                                      MSG_STRUC      *ptMessage,
                                      unsigned long  ulTimeout);

/*
 * Read all task states from a board
 * @param usDevNumber : Board number (0..3)
 * @param usDevNumber : Number of the state area (1,2)
 * @param usSIze : Size of the users data buffer (maximum length to be read)
 * @param pvData : Pointer to the users data buffer
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevGetTaskState      ( unsigned short usDevNumber,
                                      unsigned short usNumber,
                                      unsigned short usSize,
                                      void           *pvData);

/*
 * Read the actual mailbox state
 * @param usDevNumber : Board number (0..3)
 * @param pusDevMBXState : Pointer to user buffer, to hold the device mailbox state (DEVICE_MBX_EMPTY, DEVICE_MBX_FULL)
 * @param pusHostMBXState : Pointer to user buffer, to hold the device mailbox state (HOST_MBX_EMPTY, HOST_MBX_FULL)
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevGetMBXState       ( unsigned short usDevNumber,
                                      unsigned short *pusDevMBXState,
                                      unsigned short *pusHostMBXState);

/*
 * Read the various information form a board
 * @param usDevNumber : Board number (0..3)
 * @param usFuntion : Funtion of the watchdog (WATCHDOG_STOP, WATCHDOG_START)
 * @param usDevWatchDog : Pointer to a user buffer, where the watchdog counter value can be written to
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevTriggerWatchDog   ( unsigned short usDevNumber,
                                      unsigned short usFunction,
                                      unsigned short *usDevWatchDog);

/*
 * Read the various information form a board
 * @param usDevNumber : Board number (0..3)
 * @param usinfoArea : Defines which area to be read (GET_DEIVER_INFOR, GET_VERSION_INFO, GET_FIRMWARE_INFO, GET_TASK_INFO, GET_RCS_INFO, GET_DEV_INFO, GET_IO_INFO, GET_IO_SEND_DATA)
 * @param usSIze : Size of the users data buffer and Number of byte to read
 * @param pvData : Pointer to the users data buffer
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevGetInfo           ( unsigned short usDevNumber,
                                      unsigned short usFunction,
                                      unsigned short usSize,
                                      void           *pvData);


short APIENTRY DevGetTaskParameter  ( unsigned short usDevNumber,
                                      unsigned short usNumber,
                                      unsigned short usSize,
                                      void           *pvData);

/*
 * Put/Get IO data from/to a board
 * @param usDevNumber : Board number (0..3)
 * @param usSendOffset : Byte offset in the send IO data area of the communication board
 * @param usSendSize : Length of the send IO data
 * @param pvSendData : Pointer to the user send data buffer
 * @param usReceiveOffset : Byte offset in the receive IO data area of the communication board
 * @param usReceiveSize : Length of the send IO data
 * @param pvReceiveData : Pointer to the user read data buffer
 * ulTimeout : timeout in milliseconds (0 = no timeout)
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevExchangeIO        ( unsigned short usDevNumber,
                                      unsigned short usSendOffset,
                                      unsigned short usSendSize,
                                      void           *pvSendData,
                                      unsigned short usReceiveOffset,
                                      unsigned short usReceiveSize,
                                      void           *pvReceiveData,
                                      unsigned long  ulTimeout);

/*
 * Read back IO data from the send area
 * @param usDevNumber : Board number (0..3)
 * @param usOffset : Byte offset in the send IO data area of the communication board
 * @param usSize : Length of the send IO data to be read
 * @param pvData : Pointer to the user data buffer
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevReadSendData      ( unsigned short usDevNumber,
                                      unsigned short usOffset,
                                      unsigned short usSize,
                                      void           *pvData);

/*
 * Set/Clear the information bit for host is running
 * @param usDevNumber : Board number (0..3)
 * @param usMode : Function of the watchdog (HOST_NOT_READY, HOST_READY)
 * @param ulTimeout : timeout in milliseconds (0 = notimeout)
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevSetHostState      ( unsigned short usDevNumber,
                                      unsigned short usMode,
                                      unsigned long  ulTimeout);

short APIENTRY DevExtendedData      ( unsigned short usDevNumber,
                                      unsigned short usMode,
                                      unsigned short usSize,
                                      void           *pvData);

short APIENTRY DevGetMBXData        ( unsigned short usDevNumber,
                                      unsigned short usHostSize,
                                      void           *pvHostData,
                                      unsigned short usDevSize,
                                      void           *pvDevData);

short APIENTRY DevGetBoardInfoEx    ( unsigned short usDevNumber,
                                      unsigned short usSize,
                                      void           *pvData);

/*
 * Put/Get IO data from/to a COM module
 * @param usDevNumber : Board number (0..3)
 * @param usMode : Data transfer mode (0..4)
 * @param usSendOffset : Byte offset in the send IO data area of the communication board
 * @param usSendSize : Length of the send IO data
 * @param pvSendData : Pointer to the user send data buffer
 * @param usReceiveOffset : Byte offset in the receive IO data area of the communication board
 * @param usReceiveSize : Length of the send IO data
 * @param pvReceiveData : Pointer to the user read data buffer
 * ulTimeout : timeout in milliseconds (0 = no timeout)
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevExchangeIOEx      ( unsigned short usDevNumber,
                                      unsigned short usMode,
                                      unsigned short usSendOffset,
                                      unsigned short usSendSize,
                                      void           *pvSendData,
                                      unsigned short usReceiveOffset,
                                      unsigned short usReceiveSize,
                                      void           *pvReceiveData,
                                      unsigned long  ulTimeout);

/*
 * Put/Get IO data from/to a board including state information
 * @param usDevNumber : Board number (0..3)
 * @param usSendOffset : Byte offset in the send IO data area of the communication board
 * @param usSendSize : Length of the send IO data
 * @param pvSendData : Pointer to the user send data buffer
 * @param usReceiveOffset : Byte offset in the receive IO data area of the communication board
 * @param usReceiveSize : Length of the send IO data
 * @param pvReceiveData : Pointer to the user read data buffer
 * @param ptComState : Pointer to the user COMSTATE buffer
 * ulTimeout : timeout in milliseconds (0 = no timeout)
 * @return DRV_NO_ERROR if No error
 */
short APIENTRY DevExchangeIOErr     ( unsigned short usDevNumber,
                                      unsigned short usSendOffset,
                                      unsigned short usSendSize,
                                      void           *pvSendData,
                                      unsigned short usReceiveOffset,
                                      unsigned short usReceiveSize,
                                      void           *pvReceiveData,
                                      COMSTATE       *ptState,
                                      unsigned long  ulTimeout);

short APIENTRY DevReadWriteDPMRaw   ( unsigned short usDevNumber,
                                      unsigned short usMode,
                                      unsigned short usOffset,
                                      unsigned short usSize,
                                      void           *pvData);

short APIENTRY DevSpecialControl    ( unsigned short usDevNumber,
                                      unsigned short usMode,
                                      unsigned short *pusCtrlAck);

short APIENTRY DevDownload          ( unsigned short usDevNumber,
                                      unsigned short usMode,
                                      unsigned char  *pszFileName,
                                      DWORD          *pdwBytes);

short APIENTRY DevReadWriteDPMData  ( unsigned short usDevNumber,
                                      unsigned short usMode,
                                      unsigned short usOffset,
                                      unsigned short usSize,
                                      void           *pvData);

#ifdef __cplusplus
}
#endif

#endif // ifndef CIFUSER_H_INCLUDED
