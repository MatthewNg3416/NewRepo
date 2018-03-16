/* <St> *******************************************************************

  FILENAME    : DNM_USER.H

  -------------------------------------------------------------------------
  CREATED BY  : A.Beck,  Hilscher GmbH
  CREATED AT  : 02.03.98
  PROJECT     : DNM
  =========================================================================

  FUNCTION :
    User interface to DeviceNet master DEVICE

  =========================================================================

  CHANGES OF REVISIONS :

  Version Name      Date      Change
  -------------------------------------------------------------------------
  V1.014  Beck      17.11.00  - changing usFragTimeout to usRecFragTimer
                              - new command DNM_Auto_Config
                              - defintions of DeviceNet Class and Attributes
  V1.013  Beck      07.09.00  - new Command DNM_Upload
  V1.012  Beck      10.05.99  - bPerDup inserted in the structure
                                DNM_DIAGNOSTICS
  V1.011  Beck      07.01.99  - isertion of the parameter usWatchDogTime
                                in the structure DNM_INIT_PARAMETERS
  V1.010  Beck      27.07.98  - DNM_BIT_OFFSET_ADR_MSK definition
  V1.000  Beck      02.03.98  - creation

  ******************************************************************** <En> */

#if defined( _MSC_VER)          /* Microsoft C */
    #pragma pack(1)             /* Byte Alignment   */
#endif

/* ------------------------------------------------------------------------------------ */
/*   process image transfer mode definitions                                            */
/* ------------------------------------------------------------------------------------ */

#define DNM_GET_MODE_MASK                       0xf0

#define DNM_GET_MODE_NOT_SUPPORTED              0x00
#define DNM_GET_MODE_BUFFERED_DEVICE_CONTROLLED 0x20
#define DNM_GET_MODE_UNCONTROLLED               0x30
#define DNM_GET_MODE_BUFFERED_HOST_CONTROLLED   0x40

/* ======================================================================== */
/* Protocol parameter structure                                             */
/* ======================================================================== */

/* --------------------------- */
/* device warm start parameter */
/* --------------------------- */

typedef struct DNM_INIT_PARAMETERStag {
  unsigned char  bMode;               /* handshake of process data */
  #define DNM_SET_MODE_BUFFERED_DEVICE_CONTROLLED 1
  #define DNM_SET_MODE_UNCONTROLLED               2
  #define DNM_SET_MODE_BUFFERED_HOST_CONTROLLED   3
  unsigned char  bReserved;
  unsigned char  bFormat;             /* storage format of word oriented data */
  #define        DNM_FORMAT_INTEL     0
  #define        DNM_FORMAT_MOTOROLA  1
  unsigned short usWatchDogTime;
  unsigned char  abReservedA[3];
  unsigned char  abReservedB[8];
} DNM_INIT_PARAMETERS;

/* -------------------------------------------------------- */
/* structures to set up device data set by message transfer */
/* -------------------------------------------------------- */

#define DNM_MAX_NUM_DEVICES 64

/* header definition of the device data set */

typedef struct DNM_DEV_PRM_HEADERtag {
  unsigned short  usDevParaLen;    /* length of whole parameter data set */
  struct
  {
    unsigned char bUcmmSup     : 1; /* UCMM manager supported by device */
    unsigned char bReserved1   : 1; /* reserved */
    unsigned char bVendorID    : 1; /* key check Vendor ID on/off    */
    unsigned char bDeviceType  : 1; /* key check Device Type on/off  */
    unsigned char bProductCode : 1; /* key check Product Code on/off */
    unsigned char bRevision    : 1; /* key check for Revision on/off */
    unsigned char bReserved2   : 1; /* reserved area */
    unsigned char bActive      : 1; /* set device access active */
  }bDvFlag;                         /* device related flags */

  #define DNM_UCMM_SUPP  0x01
  #define DNM_KEY_VENDOR 0x04
  #define DNM_KEY_DTYPE  0x08
  #define DNM_KEY_PCODE  0x10
  #define DNM_KEY_REV    0x20
  #define DNM_ACTIVE     0x80

  unsigned char   bUcmmGroup;       /* dynamic UCMM connection group */
  #define DNM_GROUP_1 0
  #define DNM_GROUP_2 1
  #define DNM_GROUP_3 3
  unsigned short  usRecFragTimer;   /* timeout value for reconnection
                                       and for fragmented transfer */
  unsigned short  usVendorID;       /* Vendor ID */
  unsigned short  usDeviceType;     /* Device Type */
  unsigned short  usProductCode;    /* Product code */
  unsigned char   bMajorRevision;   /* Revision major digit */
  unsigned char   bMinorRevision;   /* Revision minor digit */

  unsigned char   bOctetString[2];  /* reserved field */
} DNM_DEV_PRM_HEADER;

/* definition of the connections which shall be established over
   predefined master slave connection set */

typedef struct DNM_IO_MODULEtag {
  unsigned char bDataType;      /* data type of I/O module */
  #define DNM_OUTPUT       0x80 /* selection input / output */
  #define DNM_WORD_MODULE  0x40 /* indication if word swap shall be performed */
  unsigned char bDataSize;      /* data size of module in multiples of bytes */
} DNM_IO_MODULE;

typedef struct DNM_PRED_MSTSL_IO_OBJ_HEADERtag {
  unsigned char  bConnectionType;  /* type of connection which shall be established */
  #define DNM_TYPE_CYCLIC       0x08   /* cyclic connection */
  #define DNM_TYPE_CHG_OF_STATE 0x04   /* change of state connection */
  #define DNM_TYPE_BIT_STROBED  0x02   /* bit strobe connection */
  #define DNM_TYPE_POLLED       0x01   /* polled connection */
  unsigned char  bWatchdogTimeoutAction; /* behavior when watchdog times out */
  #define DNM_TRANSMISSION_TO_TIMEOUT 0x00 /* external reset must be performed */
  #define DNM_AUTO_DELETE             0x01 /* delete connection automatically */
  #define DNM_AUTO_RESET              0x02 /* restart watchdog timer again */
  unsigned short usProdInhibitTime;    /* production inhibit time im msec */
  unsigned short usExpPacketRate;   /* established connection supervision timer */
  unsigned char  bNumOfIOModules;   /* number of following I/O modules */
} DNM_PRED_MSTSL_IO_OBJ_HEADER;

typedef struct DNM_PRED_MSTSL_CONNOBJtag {
  DNM_PRED_MSTSL_IO_OBJ_HEADER tPredMstSlObjHeader; /* header for each connection */
  DNM_IO_MODULE                atIOModule[1]; /* I/O modules for connection */
} DNM_PRED_MSTSL_CONNOBJ;

typedef struct DNM_PRED_MSTSL_CFG_DATAtag {
  unsigned short         usPredMstslCfgDataLen; /* length of the predef.MstSl. configuration data field*/
  DNM_PRED_MSTSL_CONNOBJ atConnObjInst[1];  /* connection structure */
} DNM_PRED_MSTSL_CFG_DATA;

typedef struct DNM_PRED_MSTSL_ADD_TABtag {
  unsigned short  usAddTabLen;  /* whole length of addtional table field */
  unsigned char   bInputCount;  /* number of inputs in the offset table */
  unsigned char   bOutputCount; /* number of output in the offset table */
  unsigned short  ausIOOffsets[1]; /* process data offset table */
} DNM_PRED_MSTSL_ADD_TAB;

/* attributes which shall be changed via explicit messaging by
   'Set_Attribute' command before establishing the I/O connections */

typedef struct DNM_SET_ATTR_DATAtag {
  unsigned short  usClassID;    /* class ID to be addressed */
  unsigned short  usInstanceID; /* instance in this class be addressed */
  unsigned char   bAttributeID; /* attribute which shall be changed */
  unsigned char   bDataCnt;     /* size of the following data field */
  unsigned char   abData[1];    /* attribute value */
} DNM_SET_ATTR_DATA;

typedef struct DNM_EXPL_SET_ATTR_DATAtag {
  unsigned short    usAttrDataLen;    /* length of the parameter data field */
  DNM_SET_ATTR_DATA atSetAttrData[1]; /* n parameter data fields */
} DNM_EXPL_SET_ATTR_DATA;

/* structure for UCMM connections to the device */
/* not used at the moment, but must be reserved in the download procedure */

typedef struct DNM_UCMM_CONN_OBJ_CFG_DATAtag {
  unsigned short usCfgDataLen; /* second connection table */
} DNM_UCMM_CONN_OBJ_CFG_DATA;       /* not used at the moment */

typedef struct DNM_UCMM_CONN_OBJ_ADD_TABtag {
  unsigned short  usAddTabLen;   /* whole length of addtional table field */
  unsigned char   bInputCount;   /* number of inputs in the offset table */
  unsigned char   bOutputCount;  /* number of output in the offset table */
  unsigned short  IOOffsets[1];  /* process data offset table */
  #define DNM_BIT_OFFSET_ADR_MSK 0x1FFF
} DNM_UCMM_CONN_OBJ_ADD_TAB;


/* -------------------------------------------------------- */
/* structure to set up master parameter by message transfer */
/* -------------------------------------------------------- */

typedef struct BUS_DNMtag {
  unsigned char  bOwnMacId;        /* MAC-ID of the DEVICE */
  unsigned short usVendorId;       /* = 283 for Hilscher Device */
  #define DNM_HILSCHER_VENDOR 283
  unsigned char  bBaudrate;        /* fixes the baudrate */
  #define DNM_BAUD_125   3
  #define DNM_BAUD_250   2
  #define DNM_BAUD_500   1
  unsigned char  bReserved;        /* reserved value */
  unsigned short usReserved;       /* reserved value */
  unsigned char  bAutoClear;       /* autoclear if one device shuts down */
  #define DNM_ACLR_INACTIVE 0
  #define DNM_ACLR_ACTIVE   1
  unsigned char  bSrvConsConnSize;  /* consumed I/O connection size as server */
  unsigned short usConsOffset;      /* offset adress in input area for server input data */
  unsigned char  bSrvProdConnSize;  /* produced I/O connection size as server */
  unsigned short usProdOffset;      /* offset adress in output area for server output data */
} BUS_DNM;


/* ======================================================================== */
/* Protocol slave specific single diagnostic structure                      */
/* ======================================================================== */
#define MAX_ERR_TAB_LENGTH 50

typedef struct DNM_DEVICE_SINGLE_DIAGNOSTICStag {

  struct
  {
    unsigned char bDvNoResponse : 1; /* device do not respond, timeout */
    unsigned char bReserved1    : 1;
    unsigned char bPrmFault     : 1; /* parameterization error at device */
    unsigned char bCfgFault     : 1; /* configuration error at device */
    unsigned char bUcmmSupp     : 1; /* device supports UCMM services */
    unsigned char bReserved2    : 2;
    unsigned char bDeactivated  : 1; /* device is deactivated in actual configuration */
  } bDevStatus1;

  unsigned char  bDevMainState;
  #define DNM_DV_ENTER                   0
  #define DNM_DV_INACTIVE                1
  #define DNM_DV_MANAGE_DUP_MAC          2
  #define DNM_DV_INIT_PRED_MSTSL         3
  #define DNM_DV_ALLOC_PRED_MSTSL_CONN   4
  #define DNM_DV_WAIT_FOR_ALLOC          5
  #define DNM_DV_RELEASE_MSTSL_CONN      6
  #define DNM_DV_WAIT_FOR_RELEASE        7
  #define DNM_DV_INIT_IO_CONF            8
  #define DNM_DV_ALLOC_IO_CONN           9
  #define DNM_DV_WAIT_FOR_IO_ALLOC      10
  #define DNM_DV_RELEASE_IO_CONN        11
  #define DNM_DV_WAIT_FOR_IO_RELEASE    12
  #define DNM_DV_READ_CONSUMED_SIZE     13
  #define DNM_DV_WAIT_READ_CONS         14
  #define DNM_DV_CHECK_CONS_SIZE        15
  #define DNM_DV_READ_PRODUCED_SIZE     16
  #define DNM_DV_WAIT_READ_PROD         17
  #define DNM_DV_CHECK_PROD_SIZE        18
  #define DNM_DV_CONFIGURE_CONNECTION   19
  #define DNM_DV_SET_EXPCT_RATE         20
  #define DNM_DV_WAIT_EXPCT_RATE        21
  #define DNM_DV_END_IO_POLL            22
  #define DNM_DV_WAIT_IO_POLL           23
  #define DNM_DV_END_IO_POLL_2          24
  #define DNM_DV_WAIT_IO_POLL_2         25
  #define DNM_DV_END_IO_POLL_3          26
  #define DNM_DV_WAIT_IO_POLL_3         27
  #define DNM_DV_HRTB_TIME_OUT          28
  #define DNM_DV_OPEN_EXPLICIT_CONN     30
  #define DNM_DV_OPEN_EXPLICIT_RES      31
  #define DNM_DV_OPEN_EXPLICIT_CONN_2   32
  #define DNM_DV_OPEN_EXPLICIT_RES_2    33
  #define DNM_DV_CHECK_CLOSE_CON        34
  #define DNM_DV_WAIT_CLOSE_RES         35
  #define DNM_DV_RELEASE_CONNECTION     36
  #define DNM_DV_WAIT_RELEASE_ALL       37
  #define DNM_DV_USR_EXPL_CONN          38
  #define DNM_DV_USR_EXPLICIT_RES       39
  #define DNM_DV_USR_PRED_MSL_CONN      40
  #define DNM_DV_WAIT_FOR_USER_ALLOC    41
  #define DNM_DV_CHECK_USR_CLOSE_CON    42
  #define DNM_DV_WAIT_USR_CLOSE_RES     43
  #define DNM_DV_GET_SET_ATTRIBUTE      44
  #define DNM_DV_WAIT_GET_SET_ATTR      45
  #define DNM_DV_CHECK_GET_SET_RESPONSE 46

  unsigned char  bOnlineError;
  #define DNM_DEVICE_ERROR_Timeout                      30
  #define DNM_SERV_ERROR_Unknown                        32
  #define DNM_SERV_ERROR_Response                       35
  #define DNM_SERV_ERROR_Wrong_Prod_Size                36
  #define DNM_SERV_ERROR_Wrong_Cons_Size                37
  #define DNM_SERV_ERROR_SRV_Unknown                    38
  #define DNM_SERV_ERROR_Already_Request                39
  #define DNM_SERV_ERROR_CAN_Msg_Res_Length             40
  #define DNM_SERV_ERROR_Already_Connect                41
  #define DNM_DEVICE_ERROR_Res_Poll_Len                 42
  #define DNM_DEVICE_ERROR_Res_Poll_Seq                 43
  #define DNM_DEVICE_ERROR_Res_Poll_Frag                44
  #define DNM_DEVICE_ERROR_Res_Poll_First_Seg           45
  #define DNM_DEVICE_ERROR_Res_Bit_Strobe_Len           46
  #define DNM_DEVICE_ERROR_Res_COS_Cyclic_Len           47
  #define DNM_DEVICE_ERROR_Res_COS_Cyclic_Seq           48
  #define DNM_DEVICE_ERROR_Res_COS_Cyclic_Frag          49
  #define DNM_DEVICE_ERROR_Res_COS_Cyclic_First_Seg     50
  #define DNM_DEVICE_ERROR_Res_Group_Select_Unavailable 51

  unsigned char  bGeneralErrorCode;
  unsigned char  bAdditionalCode;
  unsigned short usHrtBeatTimeout;


} DNM_DEVICE_SINGLE_DIAGNOSTICS;


/* ======================================================================== */
/* Protocol master state structure                                          */
/* ======================================================================== */


typedef struct DNM_DIAGNOSTICStag {
  /* bit field to show network and DEVICE main errors */
  struct
  {
    unsigned char bCtrl     : 1; /* wrong parameterization                  */
    unsigned char bAClr     : 1; /* auto_clear activated                    */
    unsigned char bNonExch  : 1; /* no data exchange to at least on station */
    unsigned char bFatal    : 1; /* fatal error occured                     */
    unsigned char bEvent    : 1; /* bus error events occured                */
    unsigned char bNRdy     : 1; /* host program not ready                  */
    unsigned char bDupMAC   : 1; /* duplicate MAC id detected check failed  */
    unsigned char bPerDup   : 1; /* duplicate MAC Id check active           */
  } bGlobalBits;

  /* global state for the different DEVICE main states */
  unsigned char   bDNM_state;
  #define OFFLINE  0x00
  #define STOP     0x40
  #define CLEAR    0x80
  #define OPERATE  0xC0

  /* location of error and error code */
  struct
  {
    unsigned char bErr_Dev_Adr; /* 0-63, 255 */
    #define MST_ERR  0xFF

    unsigned char bErr_Event;   /* see #defines */

    /* DEVICE internal errors */

    #define TASK_F_UNKNOWN_MODE                     52 /* unknown handshake mode configured */
    #define TASK_F_BAUDRATE_OUT_RANGE               53 /* configured batudrate not supported */
    #define TASK_F_OWN_MAC_ID_OUT_RANGE             54 /* DEVICE MAC-ID out of range */
    #define TASK_F_DUPLICATE_MAC_ID                 57 /* a duplicate MAC-ID detected */
    #define TASK_F_NO_DEV_TAB                       58 /* data base in the DEVICE has no entries included */
    #define TASK_F_ADR_DOUBLE                       59 /* double MAC-ID configured internally */
    #define TASK_F_DATA_SET_FIELD_LEN               60 /* size of one device data set invalid */
    #define TASK_F_PRED_MST_SL_ADD_LEN              61 /* offset table for predef.mst slave conn. invalid */
    #define TASK_F_PRED_MSTSL_CFG_FIELD_LEN         62 /* configur. table length for predef.mst.slave conn. invalid */
    #define TASK_F_PRED_MST_SL_ADD_TAB_INCONS       63 /* offset table do not correspond to I/O configuration table */
    #define TASK_F_EXPL_PRM_FIELD_LEN               64 /* size indicator of parameter data table corrupt */
    #define TASK_F_PRED_MSTSL_CFG_ADD_INPUT_INCONS  65 /* num of inputs in add tab not equal I/O configuration */
    #define TASK_F_PRED_MSTSL_CFG_ADD_OUTPUT_INCONS 66 /* num of outputs in add tab not equal I/O configuration */
    #define TASK_F_UNKNOWN_DATA_TYPE                67 /* unknown data type in I/O configuration */
    #define TASK_F_MODULE_DATA_SIZE                 68 /* data type does not correspond to its configured length */
    #define TASK_F_OUTPUT_OFF_RANGE                 69 /* configured output offset address out of range */
    #define TASK_F_INPUT_OFF_RANGE                  70 /* configured input offset address out of
    #define TASK_F_WRONG_TYPE_OF_CONNECTION         71 /* one predefined connection type is unknown */
    #define TASK_F_TYPE_CONNECTION_REDEFINITION     72 /* multiple connections defined in parallel */
    #define TASK_F_EXP_PACKET_LESS_PROD_INHIBIT     73 /* configured EXP_PCKT_RATE less then PROD_INHIBIT_TIME */
    #define TASK_F_PRM_FIELD_LEN_INCONSISTENT       74 /* parameter field DNM_SET_ATTR_DATA in data set inconsitent */
    #define TASK_F_NO_CAN                           75 /* no device responding on CAN network at cfg.baudrate */

  } tError;

  /* counter for the bus error events */
  unsigned short  usBus_Error_Cnt;

  /* counter of bus off reports of the CAN chip */
  unsigned short  usBus_Off_Cnt;

  /* reserved area */
  struct
  {
    unsigned char bSrvExpl   : 1; /* server explicit connection established */
    unsigned char bSrvIO     : 1; /* server I/O poll connection established */
    unsigned char bReserved  : 6; /* reserved bits */
  } bSrvStatus;
  #define SRV_EXPL 0x01
  #define SRV_IO   0x02

  unsigned char   abReserved[7];

  /* Bit-Ready, Cfg-Ready and diagnostic display of the devices */
  unsigned char   abDv_cfg  [16];  /* device configuration area */
  unsigned char   abDv_state[16];  /* device state information area */
  unsigned char   abDv_diag [16];  /* device diagnostic area */

} DNM_DIAGNOSTICS;

/* ======================================================================== */
/* Protocol definition                                                      */
/* ======================================================================== */

#define MAX_LEN_DATA_UNIT           240

/* ------------------------------- */
/* message: shared_memory          */
/* ------------------------------- */

/* message command msg.b msg.a */
#define DNM_Shared_Memory        0x11

/* ------------------------------- */
/* message: slave_diag             */
/* ------------------------------- */

/* message command msg.b msg.a */
#define DNM_Device_Diag          0x42  /* Read stored slave diagnostic data*/

typedef struct DNM_DEVICE_DIAG_CONFIRMtag {
  DNM_DEVICE_SINGLE_DIAGNOSTICS tDiagData;
} DNM_DEVICE_DIAG_CONFIRM;


/* ------------------------------- */
/* message: start_seq              */
/* ------------------------------- */

/* message command msg.b msg.a */
#define DNM_Start_Seq             0x43

typedef struct DNM_START_SEQ_REQUESTtag {
   unsigned char        bReq_Add;   /* 0 */
   unsigned char        bArea_Code; /* 0 - 63 */
   unsigned short       usTimeout;  /* 0 - 65535 */
} DNM_START_SEQ_REQUEST;

typedef struct DNM_START_SEQ_CONFIRMtag {
   unsigned char        bMax_Len_Data_Unit;   /* 240 */
} DNM_START_SEQ_CONFIRM;


/* ------------------------------- */
/* message: download               */
/* ------------------------------- */

/* message command msg.b msg.a */
#define DNM_Download              0x44

#define DNM_DEVICE_PRM            127 
/* message data structure */
typedef struct DNM_DOWNLOAD_REQUESTtag {
   unsigned char        bReq_Add;    /* 0 */
   unsigned char        bArea_Code;  /* 0 - 63, 127 */
   unsigned short       usAdd_Offset; /* 0 - 760 */
   unsigned char        abData[MAX_LEN_DATA_UNIT];
} DNM_DOWNLOAD_REQUEST;


/* ------------------------------- */
/* message: upload                 */
/* ------------------------------- */

/* message command msg.b msg.a */
#define DNM_Upload        0x50

/* message data structure */
typedef struct DNM_UPLOAD_REQUESTtag {
   unsigned char        bReq_Add;     /* 0 */
   unsigned char        bArea_Code;   /* 0 - 63 127 */
   unsigned short       usAdd_Offset; /* 0 - 760 */
   unsigned char        bData_Len;    /* 1-240 */
} DNM_UPLOAD_REQUEST;

/* ------------------------------- */
/* message: end_sequence           */
/* ------------------------------- */

/* message command msg.b msg.a */
#define DNM_End_Seq               0x45

/* message data structure */
typedef struct DNM_END_SEQ_REQUESTtag {
   unsigned char        bReq_Add;   /* 0 */
} DNM_END_SEQ_REQUEST;


/* ------------------------------- */
/* message: Get_Set_Attribute      */
/* ------------------------------- */

/* message command msg.b msg.a */
#define DNM_Get_Set_Attribute     0x4A

/* DeviceNet Class defintions */
#define DNM_IDENTITY_CLASS        1
#define DNM_MSGROUTER_CLASS       2
#define DNM_DNET_CLASS            3
#define DNM_ASSY_CLASS            4
#define DNM_CNXN_CLASS            5

/* Identity Class Attributes */
#define DNM_IDOBJ_VENDOR_ID        0x01
#define DNM_IDOBJ_DEV_TYPE         0x02
#define DNM_IDOBJ_PROD_CODE        0x03
#define DNM_IDOBJ_REV              0x04
#define DNM_IDOBJ_STATUS           0x05
#define DNM_IDOBJ_SERIAL_NUM       0x06
#define DNM_IDOBJ_NAME             0x07

/* DeviceNet Class Attributes */
#define DNM_DNETOBJ_MACID          0x01
#define DNM_DNETOBJ_BAUD           0x02
#define DNM_DNETOBJ_BOI            0x03
#define DNM_DNETOBJ_BUS_OFF_CNTR   0x04
#define DNM_DNETOBJ_ALLOC_INFO     0x05

/* Connection Class Attributes */
#define DNM_CNXN_STATE_ATTR_ID     0x01
#define DNM_CNXN_TYPE_ATTR_ID      0x02
#define DNM_CNXN_XPT_ATTR_ID       0x03
#define DNM_CNXN_PRODID_ATTR_ID    0x04
#define DNM_CNXN_CONSID_ATTR_ID    0x05
#define DNM_CNXN_INITC_ATTR_ID     0x06
#define DNM_CNXN_PSIZE_ATTR_ID     0x07
#define DNM_CNXN_CSIZE_ATTR_ID     0x08
#define DNM_CNXN_EPR_ATTR_ID       0x09
#define DNM_CNXN_TIMEOUT_ACTION    0x0C
#define DNM_CNXN_PROD_PATH_LEN     0x0D
#define DNM_CNXN_PROD_PATH_ATTR_ID 0x0E
#define DNM_CNXN_CONS_PATH_LEN     0x0F
#define DNM_CNXN_CONS_PATH_ATTR_ID 0x10

/* Predefined master/slave connection set instance numbers */
#define DNM_EXPLICIT_CNXN_INSTANCE        1
#define DNM_POLLED_CNXN_INSTANCE          2
#define DNM_BIT_STROBED_CNXN_INSTANCE     3
#define DNM_COS_CYCLIC_CNXN_INSTANCE      4


/* ------------------------------- */
/* message: Auto_Config            */
/* ------------------------------- */

/* message command msg.b msg.a */
#define DNM_Auto_Config           0x4E

/* message data structure */
typedef struct DNM_AUTO_CONFIG_REQUESTtag {
   unsigned char        bBaudrate;   
   unsigned char        bOwnMacId;
} DNM_AUTO_CONFIG_REQUEST;

typedef struct DNM_AUTO_CONFIG_CONFIRMtag {
   unsigned char        abSuppFnc[DNM_MAX_NUM_DEVICES];   
   unsigned char        abUcmmGroup[DNM_MAX_NUM_DEVICES];
} DNM_AUTO_CONFIG_CONFIRM;

#define DNM_SUPP_FNC_EXPLICIT       0x01
#define DNM_SUPP_FNC_POLLED         0x02
#define DNM_SUPP_FNC_BITSTROBED     0x04
#define DNM_SUPP_FNC_CHANGEOFSTATE  0x10
#define DNM_SUPP_FNC_CYCLIC         0x20
#define DNM_SUPP_FNC_UCMM           0x40



/* ======================================================================== */
/* message response error values                                            */
/* ======================================================================== */

#define  DNM_CON_OK   0x00  /* everything is OK                       */
#define  DNM_CON_RES  0x02  /* device reports resources unavailable   */
#define  DNM_CON_SRV  0x08  /* service not available in device        */
#define  DNM_CON_ATT  0x09  /* invalid attribute value                */
#define  DNM_CON_ALR  0x0B  /* already in request mode                */
#define  DNM_CON_STA  0x0C  /* object state conflict                  */
#define  DNM_CON_NST  0x0E  /* attribute not settable                 */
#define  DNM_CON_PRM  0x0F  /* a permission check failed              */
#define  DNM_CON_CLF  0x10  /* state conflict, device state prohibits the execution */
#define  DNM_CON_NA   0x11  /* no response from the device            */
#define  DNM_CON_NDA  0x13  /* not enough data received               */
#define  DNM_CON_NAT  0x14  /* attribute not supported                */
#define  DNM_CON_MDA  0x15  /* to much data received                  */
#define  DNM_CON_ONE  0x16  /* object does not exists                 */
#define  DNM_CON_REP  0x17  /* reply data too large, internal buffer to small */

#define  DNM_CON_RE   0x32  /* format error in response telegram      */
#define  DNM_CON_AD   0x36  /* another service still active           */
#define  DNM_CON_IP   0x37  /* MAC-ID address out of range            */
#define  DNM_CON_SE   0x39  /* sequence error in response sequence    */
#define  DNM_CON_NC   0xC8  /* DEVICE not configured                  */

#if defined( _MSC_VER)          /* Microsoft C */
    #pragma pack()              /* Byte Alignment   */
#endif

/* === eof 'DNM_USER.H' === */
