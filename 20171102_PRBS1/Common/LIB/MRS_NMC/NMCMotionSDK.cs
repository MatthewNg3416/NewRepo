using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Runtime.InteropServices;
using System.Security.Permissions;

static class NMCDEF
{
    public const uint MAX_EEPROM_SIZE = 0xFFFF; //16bit max (64k)
    public const uint ECAT_MAX_NAME = 0xFF; //255
    public const uint MAX_BOARD_CNT = 4;
    public const uint MAX_AXIS_CNT = 64;
    public const uint MAX_AXIS_ID = 65535; //Ver_0c010012_6
    public const uint MAX_NODE_CNT = 256;
    public const uint MAX_PHYSICAL_ADDR = 65535; //Ver_0c010012_3
    public const uint MAX_MASTER_ID = 10;
    public const uint MAX_ERR_LEN = 128;
    public const uint MAX_PROFILE_ITEM_COUNT = 6;
    public const uint MAX_AXES_IN_GROUP = 3;
    public const uint MAX_AXES_GROUP_CNT = 32;
    public const uint MAX_AXES_STATUS_SIZE = 8;
    public const uint MAX_ALL_STATUS_SIZE = 1536;	//Ver_0c010019_1	: 계산식 = (MAX_AXIS_CNT + MAX_AXES_GROUP_CNT)

    public const ushort INVALID_BOARD = 0xFFFF;
    public const ushort INVALID_AXIS = 0xFFFF;
    public const ushort INVALID_MASTERID = 0xFFFF;
    public const uint INVALID_FLASH = 0xFFFFFFFF;

    // Axis Parameter Definition
    public const uint COMMANDED_POSITION = 1;
    public const uint SW_LIMIT_POS = 2;
    public const uint SW_LIMIT_NEG = 3;
    public const uint ENABLE_LIMIT_POS = 4;
    public const uint ENABLE_LIMIT_NEG = 5;
    public const uint ENABLE_POS_LAG_MONITORING = 6;
    public const uint MAX_POSITION_LAG = 7;
    public const uint MAX_VELOCITY_SYSTEM = 8;
    public const uint MAX_VELOCITY_APPL = 9;
    public const uint ACTUAL_VELOCITY = 10;
    public const uint COMMANDED_VELOCITY = 11;
    public const uint MAX_ACCELERATION_SYSTEM = 12;
    public const uint MAX_ACCELERATION_APPL = 13;
    public const uint MAX_DECELERATION_SYSTEM = 14;
    public const uint MAX_DECELERATION_APPL = 15;
    public const uint MAX_JERK = 16;
    public const uint ACTUAL_POSITION = 1001;
    public const uint PROFILE_DACCEL = 1100;
    // public const uint IOMAP_HEADER_SIZE                  = 16;
    public const uint IOMAP_HEADER_SIZE = 28;//DSP 0.13 이상에서 변경됨 , 2014.08.26 (32 - 4)
    public const uint IOPAGE_SIZE = 4096;
}




namespace NMCMotionSDK
{

    /// <summary>
    /// Library Class
    /// </summary>
    /// 
    public partial class NMCSDKLib
    {
        // DLL name should be specified what you have.
        public const string NMC_DLL_NAME = "NMC_Motion.dll";
        // axis_source status
        public enum MC_STATUS : uint
        {
            MC_OK = 0x00000000,
            MC_ERROR_HW_NOT_INSTALLED = 0x000000DC,
            MC_ERROR_DD_SEND_ERROR = 0x000000DD,
            MC_ERROR_DD_READ_ERROR = 0x000000DE,
            MC_DD_ERROR_SEND = 0x000000DF,
            MC_DD_ERROR_RECV = 0x000000E0,
            MC_DD_OPEN_FAIL = 0x000000E6,
            MC_DD_NOT_OPENED = 0x000000E7,
            MC_DD_CONN_FAIL = 0x000000E8,
            MC_DD_CLIENT_START_FAIL = 0x000000E9,
            MC_DD_OK = 0x00000000,
            MC_CN_NOT_CONNECTED = 0x000000F0,
            MC_CN_CONNECTED = 0x000000F1,
            MC_CN_CONNERROR = 0x000000F2,
            MC_INVALID_SYSTEM_STATE = 0x00010000,// PLCOpen Motion Command Response
            MC_UNSUPPORT_CMD = 0x00020000,
            MC_INVALID_PARAM = 0x00030000,
            MC_INVALID_PARAM_1 = 0x00030001,
            MC_INVALID_PARAM_2 = 0x00030002,
            MC_INVALID_PARAM_3 = 0x00030003,
            MC_INVALID_PARAM_4 = 0x00030004,
            MC_INVALID_PARAM_5 = 0x00030005,
            MC_INVALID_PARAM_6 = 0x00030006,
            MC_INVALID_PARAM_7 = 0x00030007,
            MC_INVALID_PARAM_8 = 0x00030008,
            MC_INVALID_PARAM_9 = 0x00030009,
            MC_INVALID_PARAM_10 = 0x0003000A,
            MC_INVALID_SIZE = 0x00040000,
            MC_INVALID_AXIS_NUM = 0x00050000,
            MC_NOT_ENOUGH_RESOURCE = 0x00060000,
            MC_LIMIT_ERROR_PARAM = 0x00070000,
            MC_LIMIT_ERROR_PARAM_1 = 0x00070001,
            MC_LIMIT_ERROR_PARAM_2 = 0x00070002,
            MC_LIMIT_ERROR_PARAM_3 = 0x00070003,
            MC_LIMIT_ERROR_PARAM_4 = 0x00070004,
            MC_LIMIT_ERROR_PARAM_5 = 0x00070005,
            MC_LIMIT_ERROR_PARAM_6 = 0x00070006,
            MC_LIMIT_ERROR_PARAM_7 = 0x00070007,
            MC_LIMIT_ERROR_PARAM_8 = 0x00070008,
            MC_LIMIT_ERROR_PARAM_9 = 0x00070009,
            MC_LIMIT_ERROR_PARAM_10 = 0x0007000A,
            MC_INVALID_DEVICE_STATE = 0x00080000,
            MC_INVALID_DEVICE_STATE_ERROR = 0x00080001,
            MC_INVALID_AXIS_STATE_DISABLED = 0x00090000,
            MC_INVALID_AXIS_STATE_SYNC_MOTION = 0x00090004,
            MC_INVALID_AXIS_STATE_STOPPING = 0x00090006,
            MC_INVALID_AXIS_STATE_ERRORSTOP = 0x00090007,
            MC_INVALID_AXIS_CONFIG = 0x000A0000,
            MC_INVALID_AXIS_CONFIG_POS_LIMIT_SWITCH = 0x000A000B,
            MC_INVALID_AXIS_CONFIG_NEG_LIMIT_SWITCH = 0x000A000E,
            MC_INVALID_AXIS_CONFIG_HOME_SENSOR = 0x000A0010,
            MC_INVALID_AXIS_CONFIG_MARK_PULSE = 0x000A0012,
            MC_INVALID_AXIS_CONFIG_HOMING_MODE = 0x000A0064,
            MC_GEARING_RULE_VIOLATION = 0x000B0000,
            MC_LIMIT_POSITION_OVER = 0x000C0000,
            MC_INVALID_AXES_GROUP_NUM = 0x000D0000,
            MC_AXIS_ALREADY_ASIGNED = 0x000E0000,
            MC_AXES_GROUP_INVALID_STATE = 0x000F0000,
            MC_AXIS_IN_SINGLE_MOTION_STATE = 0x00100000,
            MC_GROUP_MEMBER_EMPTY = 0x00110000,
            MC_GROUP_MEMBER_LIMIT_OVER = 0x00120000,
            MC_GROUP_CMD_SIZE_ERROR = 0x00130000,
            MC_GROUP_MEMBER_NOT_ALLOCED = 0x00140000,
            MC_AXIS_IN_GROUP_MOTION = 0x00150000,
            MC_FAIL = 0xE00C0001, // Libary 에서 발생하는 에러들
            MC_ERROR = 0xE00C0002,
            MC_IOMAPING_ERR = 0xE00C0003,
            MC_COMMINIT_ERR = 0xE00C0004,
            MC_COMM_EVENT_INIT_ERR = 0xE00C0005,
            MC_READ_ENI_NODE_ERR = 0xE00C0006,
            MC_INVALID_AXIS_ERR = 0xE00C0007,
            MC_INVALID_BOARD_ERR = 0xE00C0008,
            MC_XML_PARSING_ERR = 0xE00C0009,
            MC_XML_ITEM_COUNT_MISMATCH = 0xE00C000A,
            MC_NO_BOARD_INSTALLED = 0xE00C000B,
            MC_INVALID_DOWNLOAD_FILE_TYPE = 0xE00C000C,
            MC_OPEN_ENI_ERR = 0xE00C000D,
            MC_FILE_OPEN_FAIL = 0xE00C000E,
            MC_NO_MATCHING_DOWNLOADINFORMATION = 0xE00C000F,
            MC_NONE_OP = 0xE00C0010,
            MC_FAIL_GEN_DOWNLOAD_FILE = 0xE00C0011,
            MC_REG_KEY_READ_FAIL = 0xE00C0012,
            MC_NOT_ALLOWED_IN_THIS_MASTER_MODE = 0xE00C0014,
            MC_MASTERID_OUT_OF_RANGE = 0xE00C0015,
            MC_BOARDNO_OUT_OF_RANGE = 0xE00C0016,
            MC_AXISNO_OUT_OF_RANGE = 0xE00C0017,
            MC_BOARDCNT_OUT_OF_RANGE = 0xE00C0018,
            MC_RETURN_SIZE_NOT_EQUAL = 0xE00C001A,
            MC_MASTERID_DUPLICATION_ERR = 0xE00C001B,
            MC_PARAM_ERROR_FILE_IS_NULL = 0xE00C001C,
            MC_NO_MATCHING_BOARDID_FOUND = 0xE00C001D,
            MC_NOT_READY_NETWORK_CONFIGURATION = 0xE00C001E,
            MC_INVALID_MASTERID_ERR = 0xE00C001F,
            MC_MASTER_MODE_CHANGE_NOT_ALLOWED = 0xE00C0020,
            MC_MASTER_REQUEST_PARAM_ERROR = 0xE00C0021,
            MC_MASTER_INVALID_STATE = 0xE00C0022,
            MC_NOT_MOTION_LIBRAY_INITIALIZED = 0xE00C0023, //2014.08.22 Ver_0c01000D_2
            MC_IOMANAGER_NOT_RUNNING = 0xE00C0024, //2014.08.22 Ver_0c01000D_2
            MC_ANOTHER_PROGRAM_IS_USING_NMC_LIBRARY = 0xE00C0025, //2014.10.02 Ver_0c010010_2
            MC_PCICIP_GEN_10 = 0xCC100000,
            COMM_CONNECTION_ESTABLISHED = 0xED000001,
            COMM_CONN_CONFIG_FAILED_INVALID_NETWORK_PATH = 0xED000002,
            COMM_CONN_CONFIG_FAILED_NO_RESPONSE = 0xED000003,
            COMM_CONN_CONFIG_FAILED_ERROR_RESPONSE = 0xED000004,
            COMM_CONNECTION_TIMED_OUT = 0xED000005,
            COMM_CONNECTION_CLOSED = 0xED000006,
            COMM_INCOMING_CONNECTION_RUN_IDLE_FLAG_CHANGED = 0xED000007,
            COMM_ASSEMBLY_NEW_INSTANCE_DATA = 0xED000008,
            COMM_ASSEMBLY_NEW_MEMBER_DATA = 0xED000009,
            COMM_CONNECTION_NEW_INPUT_SCANNER_DATA = 0xED00000A,
            COMM_CONNECTION_VERIFICATION = 0xED00000B,
            COMM_CONNECTION_RECONFIGURED = 0xED00000C,
            COMM_REQUEST_RESPONSE_RECEIVED = 0xED000064,
            COMM_REQUEST_FAILED_INVALID_NETWORK_PATH = 0xED000065,
            COMM_REQUEST_TIMED_OUT = 0xED000066,
            COMM_CLIENT_OBJECT_REQUEST_RECEIVED = 0xED000067,
            COMM_NEW_CLASS3_RESPONSE = 0xED000068,
            COMM_CLIENT_PCCC_REQUEST_RECEIVED = 0xED000069,
            COMM_NEW_LIST_IDENTITY_RESPONSE = 0xED00006A,
            COMM_ID_RESET = 0xED00006B,
            COMM_BACKPLANE_REQUEST_RECEIVED = 0xED00006C,
            COMM_OUT_OF_MEMORY = 0xED0000C8,
            COMM_UNABLE_INTIALIZE_WINSOCK = 0xED0000C9,
            COMM_UNABLE_START_THREAD = 0xED0000CA,
            COMM_ERROR_USING_WINSOCK = 0xED0000CB,
            COMM_ERROR_SETTING_SOCKET_TO_NONBLOCKING = 0xED0000CC,
            COMM_ERROR_SETTING_TIMER = 0xED0000CD,
            COMM_SESSION_COUNT_LIMIT_REACHED = 0xED0000CE,
            COMM_CONNECTION_COUNT_LIMIT_REACHED = 0xED0000CF,
            COMM_PENDING_REQUESTS_LIMIT_REACHED = 0xED0000D0,
            COMM_PENDING_REQUEST_GROUPS_LIMIT_REACHED = 0xED0000D1,
            COMM_ERROR_UNABLE_START_MODBUS = 0xED0000D2,
            COMM_ERROR_HW_NOT_INSTALLED = 0xED0000DC,
            COMM_ERROR_DD_SEND_ERROR = 0xED0000DD,
            COMM_ERROR_DD_READ_ERROR = 0xED0000DE,
            COMM_DD_ERROR_SEND = 0xED0000DF,
            COMM_DD_ERROR_RECV = 0xED0000E0,
            COMM_DD_OPEN_FAIL = 0xED0000E6,
            COMM_DD_NOT_OPENED = 0xED0000E7,
            COMM_DD_CONN_FAIL = 0xED0000E8,
            COMM_DD_CLIENT_START_FAIL = 0xED0000E9,
            COMM_DD_OK = 0xED000000,
            COMM_CN_NOT_CONNECTED = 0xED0000F0,
            COMM_CN_CONNECTED = 0xED0000F1,
            COMM_CN_CONNERROR = 0xED0000F2,
            COMM_ERROR_SUCCESS = 0xEE000000,
            COMM_ERROR_FAILURE = 0xEE010000,
            COMM_EXT_ERR_DUPLICATE_FWD_OPEN = 0xEE010100,
            COMM_EXT_ERR_CLASS_TRIGGER_INVALID = 0xEE010103,
            COMM_EXT_ERR_OWNERSHIP_CONFLICT = 0xEE010106,
            COMM_EXT_ERR_CONNECTION_NOT_FOUND = 0xEE010107,
            COMM_EXT_ERR_INVALID_CONN_TYPE = 0xEE010108,
            COMM_EXT_ERR_INVALID_CONN_SIZE = 0xEE010109,
            COMM_EXT_ERR_DEVICE_NOT_CONFIGURED = 0xEE010110,
            COMM_EXT_ERR_RPI_NOT_SUPPORTED = 0xEE010111,
            COMM_EXT_ERR_CONNECTION_LIMIT_REACHED = 0xEE010113,
            COMM_EXT_ERR_VENDOR_PRODUCT_CODE_MISMATCH = 0xEE010114,
            COMM_EXT_ERR_PRODUCT_TYPE_MISMATCH = 0xEE010115,
            COMM_EXT_ERR_REVISION_MISMATCH = 0xEE010116,
            COMM_EXT_ERR_INVALID_CONN_POINT = 0xEE010117,
            COMM_EXT_ERR_INVALID_CONFIG_FORMAT = 0xEE010118,
            COMM_EXT_ERR_NO_CONTROLLING_CONNECTION = 0xEE010119,
            COMM_EXT_ERR_TARGET_CONN_LIMIT_REACHED = 0xEE01011A,
            COMM_EXT_ERR_RPI_SMALLER_THAN_INHIBIT = 0xEE01011B,
            COMM_EXT_ERR_CONNECTION_TIMED_OUT = 0xEE010203,
            COMM_EXT_ERR_UNCONNECTED_SEND_TIMED_OUT = 0xEE010204,
            COMM_EXT_ERR_PARAMETER_ERROR = 0xEE010205,
            COMM_EXT_ERR_MESSAGE_TOO_LARGE = 0xEE010206,
            COMM_EXT_ERR_UNCONN_ACK_WITHOUT_REPLY = 0xEE010207,
            COMM_EXT_ERR_NO_BUFFER_MEMORY_AVAILABLE = 0xEE010301,
            COMM_EXT_ERR_BANDWIDTH_NOT_AVAILABLE = 0xEE010302,
            COMM_EXT_ERR_TAG_FILTERS_NOT_AVAILABLE = 0xEE010303,
            COMM_EXT_ERR_REAL_TIME_DATA_NOT_CONFIG = 0xEE010304,
            COMM_EXT_ERR_PORT_NOT_AVAILABLE = 0xEE010311,
            COMM_EXT_ERR_LINK_ADDR_NOT_AVAILABLE = 0xEE010312,
            COMM_EXT_ERR_INVALID_SEGMENT_TYPE_VALUE = 0xEE010315,
            COMM_EXT_ERR_PATH_CONNECTION_MISMATCH = 0xEE010316,
            COMM_EXT_ERR_INVALID_NETWORK_SEGMENT = 0xEE010317,
            COMM_EXT_ERR_INVALID_LINK_ADDRESS = 0xEE010318,
            COMM_EXT_ERR_SECOND_RESOURCES_NOT_AVAILABLE = 0xEE010319,
            COMM_EXT_ERR_CONNECTION_ALREADY_ESTABLISHED = 0xEE01031A,
            COMM_EXT_ERR_DIRECT_CONN_ALREADY_ESTABLISHED = 0xEE01031B,
            COMM_EXT_ERR_MISC = 0xEE01031C,
            COMM_EXT_ERR_REDUNDANT_CONNECTION_MISMATCH = 0xEE01031D,
            COMM_EXT_ERR_NO_MORE_CONSUMER_RESOURCES = 0xEE01031E,
            COMM_EXT_ERR_NO_TARGET_PATH_RESOURCES = 0xEE01031F,
            COMM_EXT_ERR_VENDOR_SPECIFIC = 0xEE010320,
            COMM_ERROR_NO_RESOURCE = 0xEE020000,
            COMM_ERROR_INVALID_PARAMETER_VALUE = 0xEE030000,
            COMM_ERROR_INVALID_SEG_TYPE = 0xEE040000,
            COMM_ERROR_INVALID_DESTINATION = 0xEE050000,
            COMM_ERROR_PARTIAL_DATA = 0xEE060000,
            COMM_ERROR_CONN_LOST = 0xEE070000,
            COMM_ERROR_BAD_SERVICE = 0xEE080000,
            COMM_ERROR_BAD_ATTR_DATA = 0xEE090000,
            COMM_ERROR_ATTR_LIST_ERROR = 0xEE0A0000,
            COMM_ERROR_ALREADY_IN_REQUESTED_MODE = 0xEE0B0000,
            COMM_ERROR_OBJECT_STATE_CONFLICT = 0xEE0C0000,
            COMM_ERROR_OBJ_ALREADY_EXISTS = 0xEE0D0000,
            COMM_ERROR_ATTR_NOT_SETTABLE = 0xEE0E0000,
            COMM_ERROR_PERMISSION_DENIED = 0xEE0F0000,
            COMM_ERROR_DEV_IN_WRONG_STATE = 0xEE100000,
            COMM_ERROR_REPLY_DATA_TOO_LARGE = 0xEE110000,
            COMM_ERROR_FRAGMENT_PRIMITIVE = 0xEE120000,
            COMM_ERROR_NOT_ENOUGH_DATA = 0xEE130000,
            COMM_ERROR_ATTR_NOT_SUPPORTED = 0xEE140000,
            COMM_ERROR_TOO_MUCH_DATA = 0xEE150000,
            COMM_ERROR_OBJ_DOES_NOT_EXIST = 0xEE160000,
            COMM_ERROR_NO_FRAGMENTATION = 0xEE170000,
            COMM_ERROR_DATA_NOT_SAVED = 0xEE180000,
            COMM_ERROR_DATA_WRITE_FAILURE = 0xEE190000,
            COMM_ERROR_REQUEST_TOO_LARGE = 0xEE1A0000,
            COMM_ERROR_RESPONSE_TOO_LARGE = 0xEE1B0000,
            COMM_ERROR_MISSING_LIST_DATA = 0xEE1C0000,
            COMM_ERROR_INVALID_LIST_STATUS = 0xEE1D0000,
            COMM_ERROR_SERVICE_ERROR = 0xEE1E0000,
            COMM_ERROR_VENDOR_SPECIFIC = 0xEE1F0000,
            COMM_ERROR_INVALID_PARAMETER = 0xEE200000,
            COMM_ERROR_WRITE_ONCE_FAILURE = 0xEE210000,
            COMM_ERROR_INVALID_REPLY = 0xEE220000,
            COMM_ERROR_BAD_KEY_IN_PATH = 0xEE250000,
            COMM_ERROR_BAD_PATH_SIZE = 0xEE260000,
            COMM_ERROR_UNEXPECTED_ATTR = 0xEE270000,
            COMM_ERROR_INVALID_MEMBER = 0xEE280000,
            COMM_ERROR_MEMBER_NOT_SETTABLE = 0xEE290000,
            COMM_ERROR_UNKNOWN_MODBUS_ERROR = 0xEE2B0000,
            COMM_ERROR_HW_NOT_INSTALLED1 = 0xEE2C0000,
            COMM_ERROR_ENCAP_PROTOCOL = 0xEE6A0000,
            COMM_ERROR_STILL_PROCESSING = 0xEEFF0000,
            MC_DOWNLOAD_FAIL_DUE_TO_ANOTHER_PROGRAM_IS_RUNNING = 0xEE800000,//Ver_0c010012_1
        }
        public enum EcState : uint
        {
            eST_UNKNOWN = 0,
            eST_INIT    = 0x01,		// Ver_0c010016_2
            eST_PREOP   = 0x02,		// Ver_0c010016_2
            eST_SAFEOP  = 0x04,		// Ver_0c010016_2
            eST_OP      = 0x08,		// Ver_0c010016_2
            eST_ACKERR  = 0x10		// Ver_0c010016_2
        }
        public enum EcMstMode : uint
        {
            eMM_IDLE = 0,
            eMM_SCAN,
            eMM_RUN,
            eMM_INTRANSITION,
            eMM_ERR,
            eMM_LINKBROKEN,
        }
        public enum EcScanMode : uint
        {
            SCAN_ALL = 0,
            SCAN_SINGLE,
        }
        public enum EcScanSts : uint
        {
            SCAN_NONE = 0,
            SCAN_BUSY,
            SCAN_DONE
        }
        public enum IOBufMode : ushort
        {
            BUF_OUT = 0,
            BUF_IN = 1,
        }
        public enum MC_AXISSTATUS : uint
        {
            mcErrorStop = 0x00000001,
            mcDisabled = 0x00000002,
            mcStopping = 0x00000004,
            mcStandStill = 0x00000008,
            mcDiscreteMotion = 0x00000010,
            mcContinuousMotion = 0x00000020,
            mcSynchroMotion = 0x00000040,
            mcHoming = 0x00000080,
            mcReserved_as_8 = 0x00000100,
            mcReserved_as_9 = 0x00000200,
            mcConstantVelocity = 0x00000400,
            mcAccelerating = 0x00000800,
            mcDecelerating = 0x00001000,
            mcDirectionPositive = 0x00002000,
            mcDirectionNegative = 0x00004000,
            mcLimitSwitchNeg = 0x00008000,
            mcLimitSwitchPos = 0x00010000,
            mcHomeAbsSwitch = 0x00020000,
            mcLimitSwitchPosEvent = 0x00040000,
            mcLimitSwitchNegEvent = 0x00080000,
            mcDriveFault = 0x00100000,
            mcSensorStop = 0x00200000,
            mcReadyForPowerOn = 0x00400000,
            mcPowerOn = 0x00800000,
            mcIsHomed = 0x01000000,
            mcAxisWarning = 0x02000000,
            mcMotionComplete = 0x04000000,
            mcGearing = 0x08000000,
            mcGroupMotion = 0x10000000,
            mcBufferFull = 0x20000000,
            mcReserved_as_30 = 0x40000000,
            mcReserved_as_31 = 0x80000000,
        }

        //MC_ReadStatus
        public enum MC_Status : uint
        {
            mcASErrorStop = 0x00000001,
            mcASDisabled = 0x00000002,
            mcASStopping = 0x00000004,
            mcASStandStill = 0x00000008,
            mcASDiscreteMotion = 0x00000010,
            mcASContinuousMotion = 0x00000020,
            mcASSynchroMotion = 0x00000040,
            mcASHoming = 0x00000080,
        }

        //MC_ReadMotionState
        public enum MC_MOTIONSTATE : uint
        {
            mcMSConstantVelocity = 0x00000001,
            mcMSAccelerating = 0x00000002,
            mcMSDecelerating = 0x00000004,
            mcMSDirectionPositive = 0x00000008,
            mcMSDirectionNegative = 0x00000010,
        }
        //MC_ReadAxisInfo
        public enum MC_AXISINFO : uint
        {
            mcAIHomeAbsSwitch = 0x00000001,
            mcAILimitSwitchPos = 0x00000002,
            mcAILimitSwitchNeg = 0x00000004,
            mcAIReserved3 = 0x00000008,
            mcAIReserved4 = 0x00000010,
            mcAIReadyForPowerOn = 0x00000020,
            mcAIPowerOn = 0x00000040,
            mcAIIsHomed = 0x00000080,
            mcAIAxisWarining = 0x00000100,
            mcAIMotionComplete = 0x00000200,
            mcAIGearing = 0x00000400,
            mcAIGroupMotion = 0x00000800,
            mcAIBufferFull = 0x00001000,
            mcAIReseved13 = 0x00002000,
        }
        public enum MC_AXISERROR : uint
        {
            mcAxis_NO_ERROR = 0x0000,
            mcAxis_DEVICE_ERROR = 0x0001,
            mcAxis_INVALID_AXIS_STATE = 0x0002,
            mcAxis_PARAMETER_INVALID = 0x0003,
            mcAxis_UNSUPPORT_CMD_REQUEST = 0x0004,
            mcAxis_CMD_REQUEST_FORMAT_WRONG = 0x0005,
            mcAxis_RESOURCE_ERROR = 0x0006,
            mcAxis_CONFIG_INVALID = 0x0007,
            mcAxis_POSITION_FOLLOWING_ERROR = 0x0008,
            mcAxis_VELOCITY_FOLLOWING_ERROR = 0x0009,
            mcAxis_SYSTEM_MAX_VELOCITY_OVER_ERROR = 0x000A,
            mcAxis_SYSTEM_MAX_ACCEL_OVER_ERROR = 0x000B,
            mcAxis_SYSTEM_MAX_DECEL_OVER_ERROR = 0x000C,
            mcAxis_SYSYEM_MAX_JERK_OVER_ERROR = 0x000D,
            mcAxis_MALFUNCTION_ERROR = 0x000E,
            mcAxis_GEARING_RULE_VIOLATION = 0x000F,
            mcAxis_HW_LIMIT_REACHED_WARNING = 0x8001,
            mcAxis_SW_LIMIT_REACHED_WARNING = 0x8002,
        }
        public struct MC_AxisErrorInfo
        {
            ushort ErrorId;
            ushort ErrorInfo0;
            ushort ErrorInfo1;
        }
        public struct MC_AxesGroupRawDataStatus
        {
            byte Mode;				    // Raw Data Mode
            byte Enabled;				// Enabled Flag
            uint EmptyBufferCount;	    // Empty Buffer Count
            ushort InBufferIndex;		// In Buffer Index
            ushort OutBufferIndex;		// Out Buffer Index
            uint CurrentRawDataID;	    // Current Out RawDataSet ID
            byte CoordSystem;			// Reserved
        }
        public enum MC_SOURCE : uint
        {
            mcSetValue = 0, //Synchronization on master set value
            mcActualValue,  //Synchronization on master actual value
        }
        public enum MC_EXECUTION_MODE : uint
        {
            mcImmediately = 0,
            mcQueued,
        }
        public enum MC_BUFFER_MODE : uint
        {
            //0:Aborting, 1:Buffered, 2:BlendingLow, 3:BlendingPrevious, 4:BlendingNext, 5:BlendingHigh
            mcAborting = 0,
            mcBuffered,
            mcBlendingLow,
            mcBlendingPrevious,
            mcBlendingNext,
            mcBlendingHigh,
            mcBufferedBlendingLow,
            mcBufferedBlendingPrevious,
            mcBufferedBlendingNext,
            mcBufferedBlendingHigh,
        }
        public enum MC_DIRECTION : uint
        {
            // 0:Positive Direction, 1:Shortest_way, 2:Negative_Direction, 3:Current_Direction
            mcPositiveDirection = 0,
            mcShortestWay,
            mcNegativeDirection,
            mcCurrentDirection,
        }
        public enum MC_GearStatus : ushort
        {
            mcGearActive = 0x0001,
            mcGearIn = 0x0002,
            mcGearReserved2 = 0x0004,
            mcGearReserved3 = 0x0008,
            mcGearReserved4 = 0x0010,
            mcGearReserved5 = 0x0020,
            mcGearReserved6 = 0x0040,
            mcGearReserved7 = 0x0080,
            mcGearReserved8 = 0x0100,
            mcGearReserved9 = 0x0200,
            mcGearReserved10 = 0x0400,
            mcGearReserved11 = 0x0800,
            mcGearReserved12 = 0x1000,
            mcGearReserved13 = 0x2000,
            mcGearReserved14 = 0x4000,
            mcGearReserved15 = 0x8000,
        }
        public enum AxisStopType : uint
        {
            IMMEDIATE = 0,
            DECEL
        }
        public enum MC_AXIS_DIRECTION : uint
        {
            CW = 0,
            CCW
        }
        public enum MC_AXIS_CONTROL : uint
        {
            OL_PULSE_DIRECTION = 0,
            OL_TWO_PULSE,
            OL_QUDARATURE_PULSE,
            CL_VELOCITY_LEVEL_ANALOG,
            CL_TORQUE_LEVEL_ANALOG,
        }
        public enum MC_AXIS_I_MODE : uint
        {
            IN_STANDING = 0,
            ALWAYS,
        }
        public enum BinFileType : uint
        {
            BIN_BOOT = 1,
            BIN_A8OS = 2,
            BIN_TM = 3,
            BIN_DSPOS = 5,
            BIN_ENI = 6
        }
        public enum SWVerType : ushort
        {
            SW_VER_MOTION = 0,
            SW_VER_PCICIP,
            SW_VER_DDSDK,
            SW_VER_DD,
        }
        public enum MC_CoordSystem : byte
        {
            mcACS = 1,
            mcMCS,
            mcPCS,
        }
        public enum MC_TRANSITION_MODE : byte
        {
            mcTMNone = 0,			//Insert no transition curve (default mode)
            mcTMStartVelocity,		//Transition with given start velocity
            mcTMConstantVelocity,	//Transition with given constant velocity
            mcTMCornerDistance,		//Transition with given corner distance
            mcTMMaxCornerDeviation, //Transition with given maximum corner deviation
            //5 - 9 Reserved by PLCopen
            //10 -… Supplier specific modes
        }
        public enum MC_CIRC_MODE : byte
        {
            mcBORDER = 0,
            mcCENTER,
            mcRADIUS,
            mcCENTER_ANGLE = 10,
            mcBORDER_ANGLE = 11,
        }
        public enum MC_CIRC_PATHCHOICE : byte
        {
            mcClockWise = 0,
            mcCounterClockWise,
        }
        public enum MC_RAW_DATA_MODE : byte
        {
            mcPositionCmdMode = 0,
            mcVelocityCmdMode,
            mcTorqueCmdMode,
        }
        public enum MC_SAVE_MODE : byte
        {
            mcSMAuto = 0,
            mcSMIndex,
        }
        public enum MC_GroupStatus : uint
        {
            GroupMoving = 0x00000001,
            GroupHoming = 0x00000002,
            GroupErrorStop = 0x00000004,
            GroupStandby = 0x00000008,
            GroupStopping = 0x00000010,
            GroupDisabled = 0x00000020,
            ConstantVelocity = 0x00000040,
            Accelerating = 0x00000080,
            Decelerating = 0x00000100,
            InPosition = 0x00000200,
        }		
		public enum MC_ParamID : uint
		{
			mcpCommandedPosition          		= 1,
			mcpSWLimitPos_  					= 2,
			mcpSWLimitNeg_  					= 3,
			mcpEnableLimitPos_  				= 4,
			mcpEnableLimitNeg_  				= 5,
			mcpEnablePosLagMonitoring  			= 6,
			mcpMaxPositionLag_  				= 7,
			mcpMaxVelocitySystem_  				= 8,
			mcpMaxVelocityAppl_  				= 9,
			mcpActualVelocity  					= 10,
			mcpCommandedVelocity  				= 11,
			mcpMaxAccelerationSystem_  			= 12,
			mcpMaxAccelerationAppl  			= 13,
			mcpMaxDecelerationSystem_  			= 14,
			mcpMaxDecelerationAppl  			= 15,
			mcpMaxJerkSystem  					= 16,
			mcpMaxJerkAppl_  					= 17,
			mcpActualPosition					= 1000,
			mcpCommandedAccel					= 1001,
			mcpActualAccel						= 1002,
			mcpCommandedJerk					= 1003,
			mcpActualJerk						= 1004,
			mcpTotalBufferCount					= 1010,
			mcpAvailableBufferCount				= 1011,
			mcpAxisType							= 2002,
			mcpModuloAxis						= 2003,
			mcpModuloValue						= 2004,
			mcpEnableHWLimitPos					= 2010,
			mcpHWLimitPosInputNum				= 2011,
			mcpHWLimitPosActLevel				= 2012,
			mcpEnableHWLimitNeg					= 2013,
			mcpHWLimitNegInputNum				= 2014,
			mcpHWLimitNegActLevel				= 2015,
			mcpHomeInputNum						= 2016,
			mcpHomeActLevel						= 2017,
			mcpMarkerInputNum					= 2018,
			mcpMarkerActLevel					= 2019,
			mcpInputActLevel					= 2020,
			mcpEnableLimitPos					= 2030,
			mcpSWLimitPos						= 2031,
			mcpEnableLimitNeg					= 2032,
			mcpSWLimitNeg						= 2033,
			mcpMaxVelocityAppl					= 2034,
			mcpMaxAccelAppl						= 2035,
			mcpMaxDecelAppl						= 2036,
			mcpMaxJerkAppl						= 2037,
			mcpMaxVelocitySystem				= 2038,
			mcpMaxAccelerationSystem			= 2039,
			mcpMaxDecelerationSystem			= 2040,
			mcpmcpMaxJerkSystem					= 2041,
			mcpmcpEStopType						= 2060,
			mcpEStopDecel						= 2061,
			mcpEStopJerk						= 2062,
			mcpInvertCmdDir						= 2070,
			mcpCmdScaleFactor					= 2071,
			mcpFeedbackMode						= 2072,
			mcpInvertFeedbackDir				= 2073,
			mcpFeedbackScaleFactor				= 2074,
			mcpPositionFeedbackFilter			= 2075,
			mcpVelocityFeedbackFilter			= 2076,
			mcpAccelerationFeedbackFilter	    = 2077,
			mcpStartVelocityOffset				= 2078,
			mcpStopVelocityOffset				= 2079,
			mcpInPositionCheckType				= 2080,
			mcpInPositionWindowSize				= 2081,
			mcpInVelocityWindowSize				= 2082,
			mcpEnablePositionLagMonit			= 2083,
			mcpMaxPositionLag					= 2084,
			mcpPositionLagCalMethod				= 2085,
			mcpEnableVelocityLagMonit			= 2086,
			mcpMaxVelocityLag					= 2087,
			mcpVelocityLagCalMethod				= 2088,
			mcpHomingType						= 2100,
			mcpHomingDir						= 2101,
			mcpHomingVelocity					= 2102,
			mcpHomingAcceleration				= 2103,
			mcpHomingDeceleration				= 2104,
			mcpHomingJerk						= 2105,
			mcpHomingCreepVelocity				= 2106,
			mcpHomePositionOffset				= 2107,
			mcpHomeCompleteFlagHandle			= 2108,
			//v12.1.0.33 syna
			mcpSensor0StopEnable 				= 2109,
			mcpSensor0StopMode	 				= 2110,
			mcpSensor0StopIOOffset 				= 2111,
			mcpSensor0StopIOSize 				= 2112,
			mcpSensor0StopIOBit 				= 2113,
			mcpSensor0StopPosOffset 			= 2114,
			mcpSensor1StopEnable 				= 2115,
			mcpSensor1StopMode	 				= 2116,
			mcpSensor1StopIOOffset 				= 2117,
			mcpSensor1StopIOSize 				= 2118,
			mcpSensor1StopIOBit 				= 2119,
			mcpSensor1StopPosOffset 			= 2120,
			mcpSensor2StopEnable 				= 2121,
			mcpSensor2StopMode	 				= 2122,
			mcpSensor2StopIOOffset 				= 2123,
			mcpSensor2StopIOSize 				= 2124,
			mcpSensor2StopIOBit 				= 2125,
			mcpSensor2StopPosOffset 			= 2126,
			mcpSensor3StopEnable 				= 2127,
			mcpSensor3StopMode	 				= 2128,
			mcpSensor3StopIOOffset 				= 2129,
			mcpSensor3StopIOSize 				= 2130,
			mcpSensor3StopIOBit 				= 2131,
			mcpSensor3StopPosOffset 			= 2132,
			mcpSensor4StopEnable 				= 2133,
			mcpSensor4StopMode	 				= 2134,
			mcpSensor4StopIOOffset 				= 2135,
			mcpSensor4StopIOSize 				= 2136,
			mcpSensor4StopIOBit 				= 2137,
			mcpSensor4StopPosOffset 			= 2138,
		}	
    }
}

namespace NMCMotionSDK
{
    // MMC Library APIs
    public partial class NMCSDKLib
    {

        //============================================================================
        //                              > APIs <
        //----------------------------------------------------------------------------
        // Initialization
        // General & Utiltiy Functions
        //===============================================================================
        #region Public Function
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_Init();
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_MasterInit(ushort MasterID);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_MasterRUN(ushort MasterID);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_MasterSTOP(ushort MasterID);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GetSWVersion(ushort Type, ref ushort Major, ref ushort Minor);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GetErrorMessage(uint ErrorCode, uint Size, StringBuilder ErrorMessage);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GetMasterMap(ushort[] MasterMap /*MAX_BOARD_CNT*/, ref ushort MasterCount);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GetMasterCount(ref ushort MasterCount);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GetBoardScanNo(ushort BoardID, ref ushort BoardScanNo);

        //===========================================================================
        // Master Utility APIs
        //---------------------------------------------------------------------------
        //============================================================================
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterGetCurMode(ushort BoardID, ref byte MasterMode);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterGetBoardID(ushort MasterScanNo, ref ushort MasterID);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterGetAxesCount(ushort BoardID, ref uint TotalAxisCount);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterGetOSRevision(ushort BoardID, ref byte Major, ref byte Minor);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterGetMCRevision(ushort BoardID, ref byte Major, ref byte Minor);
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterGetSDOData(
                                                        ushort BoardID,
                                                        ushort SlaveNo,
                                                        ushort SDOIndex,
                                                        byte SubIndex,
                                                        uint DataSize,
                                                        ref uint respDataSize,
                                                        byte[] bDataArray
                                                        );
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterGetSDODataEcatAddr(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ushort SDOIndex,
                                    byte SubIndex,
                                    uint DataSize,
                                    ref uint respDataSize,
                                    byte[] bDataArray
                                    );

        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterSetSDOData(
                                                        ushort BoardID,
                                                        ushort SlaveNo,
                                                        ushort SDOIndex,
                                                        byte SubIndex,
                                                        uint DataSize,
                                                        ref uint respDataSize,
                                                        byte[] bDataArray
                                                        );
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterSetSDODataEcatAddr(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ushort SDOIndex,
                                    byte SubIndex,
                                    uint DataSize,
                                    ref uint respDataSize,
                                    byte[] bDataArray
                                    );

        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterGetLastError(
                                    ushort BoardID,
                                    ref uint pSequenceNo,
                                    ref uint pErrorCode,
                                    byte[] ExtErrorInfo
                                    );

        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterClearError(
                                    ushort BoardID
                                    );

        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MasterECatDirectAccess(
                                    ushort BoardID,
                                    byte EcatCmd,
                                    ushort Adp,
                                    ushort Ado,
                                    ushort ReqDataSize,
                                    byte[] bReqDataArray,
                                    ref ushort RespDataSize,
                                    byte[] bRespDataArray,
                                    ref ushort WC
                                    );
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS SlaveGetAliasNo(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ref ushort AliasID
                                    );
        //Ver_0c010012_8
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS SlaveSetAliasNo(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ushort AliasID
                                    );
        //21 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS SlaveGetCurState(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ref byte data
                                    );

        //===========================================================================
        // Motion APIs
        //---------------------------------------------------------------------------
        //5.1.1.1
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_Power(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    bool Enable// Axis enable : 0:Disable, 1:Enable
                                    );
        //5.1.1.2
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_MoveAbsolute(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    double Position, //Target Position
                                    double Velocity, // Max Velocity
                                    double Accel, // Max Acceleration
                                    double Decel, // Max Deceleration
                                    double Jerk, //Max Jerk
                                    MC_DIRECTION Dir, // 0:Positive Direction, 1:Shortest_way, 2:Negative_Direction, 3:Current_Direction
                                    MC_BUFFER_MODE BufferMode //Buffer Mode : 0:Aborting, 1:Buffered, 2:BlendingLow, 3:BlendingPrevious, 4:BlendingNext, 5:BlendingHigh
                                    );
        //5.1.1.3
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_MoveRelative(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    double Distance, //Distance
                                    double Velocity, // Max Velocity
                                    double Accel, // Max Acceleration
                                    double Decel, // Max Deceleration
                                    double Jerk, //Max Jerk
                                    MC_BUFFER_MODE BufferMode //Buffer Mode : 0:Aborting, 1:Buffered, 2:BlendingLow, 3:BlendingPrevious, 4:BlendingNext, 5:BlendingHigh
                                    );
        //5.1.1.6
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_MoveVelocity(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    double Velocity, // Max Velocity
                                    double Accel, // Max Acceleration
                                    double Decel, // Max Deceleration
                                    double Jerk, //Max Jerk
                                    MC_DIRECTION Dir, // 0:Positive Direction, 1:Shortest_way, 2:Negative_Direction, 3:Current_Direction
                                    MC_BUFFER_MODE BufferMode //Buffer Mode : 0:Aborting, 1:Buffered, 2:BlendingLow, 3:BlendingPrevious, 4:BlendingNext, 5:BlendingHigh
                                    );
        //5.1.1.7
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_Home(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    double Position, // 
                                    MC_BUFFER_MODE BufferMode
                                    );
        //5.1.1.8
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_Stop(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    bool Execute,
                                    double Decel, // Max Deceleration
                                    double Jerk //Max Jerk
                                    );
        //5.1.1.9 MCReadStatus
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadStatus(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    ref uint pStatus
                                    );
        //5.1.1.10
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadAxisError(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    ref ushort pErrorID,
                                    ref ushort pErrorInfo,
                                    ref ushort pErrorInfoExt
                                    );
        //5.1.1.11
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_Reset(
                                    ushort BoardID, // BoardID
                                    ushort AxisID //Axis number
                                    );

        //5.1.1.12 MCReadParameter
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadParameter(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number,
                                    uint ParameterNum, //PN
                                    ref double pValue
                                    );
        //5.1.1.12 MCReadBoolParameter
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadBoolParameter(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number,
                                    uint ParameterNum, //PN
                                    ref bool pValue
                                    );
        //5.1.1.12 MCReadIntParameter
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadIntParameter(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number,
                                    uint ParameterNum, //PN
                                    ref uint pValue
                                    );
        //5.1.1.13 MCWriteParameter
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_WriteParameter(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number,
                                    uint ParameterNum, //PN
                                    double dValue
                                    );
        //5.1.1.13 MCWriteBoolParameter
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_WriteBoolParameter(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number,
                                    uint ParameterNum, //PN
                                    bool Value
                                    );
        //5.1.1.13 MCWriteIntParameter
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_WriteIntParameter(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number,
                                    uint ParameterNum, //PN
                                    uint dwValue
                                    );
        //5.1.1.14 MCReadActualPosition
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadActualPosition(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    ref double pPosition // Position Acutal Value
                                    );
        //5.1.1.21 MC_GearIn
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GearIn(
                                    ushort BoardID,
                                    ushort MasterAxisID,
                                    ushort SlaveAxisID,
                                    uint RatioNumerator,
                                    uint RatioDenominator,
                                    MC_SOURCE MasterValueSource,
                                    double Acceleration,
                                    double Deceleration,
                                    double Jerk,
                                    MC_BUFFER_MODE BufferMode
                                    );
        //5.1.1.22 MC_GearOut
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GearOut(
                                    ushort BoardID,
                                    ushort SlaveAxisID
                                    );
        //5.1.1.24
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_TouchProbe(
                                    ushort BoardID,
                                    ushort AxisID,
                                    uint TriggerInput,
                                    bool WindowOnly,
                                    double FirstPosition,
                                    double LastPosition
                                    );
        //5.1.1.25
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_AbortTrigger(
                                    ushort BoardID,
                                    ushort AxisID,
                                    uint TriggerInput
                                    );
        //5.1.1.26
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadDigitalInput(
                                    ushort BoardID,
                                    ushort AxisID,
                                    uint InputNumber,
                                    ref bool pValue
                                    );
        //5.1.1.27
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadDigitalOutput(
                                    ushort BoardID,
                                    ushort AxisID,
                                    uint OutputNumber,
                                    ref bool pValue
                                    );
        //5.1.1.28
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_WriteDigitalOutput(
                                    ushort BoardID,
                                    ushort AxisID,
                                    uint OutputNumber,
                                    bool Value
                                    );
        //5.1.1.29 MCSetPosition
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_SetPosition(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    double Position,
                                    bool Relative,
                                    MC_EXECUTION_MODE Mode
                                    );
        //5.1.1.31 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadActualVelocity(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    ref double pVelocity // Velocity Acutal Value
                                    );

        //5.1.1.37
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_Halt(
                                    ushort BoardID,
                                    ushort AxisID,
                                    double Deceleration,
                                    double Jerk,
                                    MC_BUFFER_MODE BufferMode
                                    );
        //5.1.1.38
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_TriggerMonitor(
                                    ushort BoardID,
                                    ushort AxisID,
                                    uint TriggerInput,
                                    ref bool pDone,
                                    ref double pRecordedPosition,
                                    ref bool pProbeActive
                                    );
        //5.1.1.39 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadMotionState(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    ref uint pMotionState // MC_MOTIONSTATE
                                    );
        //5.1.1.40 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadAxisInfo(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    ref uint pAxisInfo //MC_AXISINFO
                                    );
        //5.1.1.41 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadAxisStatus(
                                    ushort BoardID, // BoardID
                                    ushort AxisID, //Axis number
                                    ref uint pAxisStatus //MC_AXISSTATUS
                                    );
        //5.1.1.42 MC_GearMonitor
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GearMonitor(
                                    ushort BoardID,
                                    ushort AxisID,
                                    ref ushort pStatus
                                    );
        //5.1.1.43 MC_ReadProfileData
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadProfileData(
                                    ushort BoardID,
                                    ushort AxisID,
                                    ref uint TickCount,
                                    ref double Position,
                                    ref double Velocity,
                                    ref double Accel,
                                    ref double Jerk,
                                    ref double ActPos,
                                    ref double ActVel
                                    );

        //===========================================================================
        //GROUP Motion
        //---------------------------------------------------------------------------
        //5.1.1.44 MC_AddAxisToGroup [32]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_AddAxisToGroup(
                                    ushort BoardID,
                                    ushort AxisID,
                                    ushort AxesGroupNo,
                                    ushort IDInGroup
                                    );
        //5.1.1.45 MC_RemoveAxisFromGroup [33]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_RemoveAxisFromGroup(
                                    ushort BoardID,
                                    ushort AxesGroupNo,
                                    ushort IDInGroup
                                    );
        //5.1.1.46 MC_UngroupAllAxes [34]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_UngroupAllAxes(
                                    ushort BoardID,
                                    ushort AxesGroupNo
                                    );
        //5.1.1.47 MC_GroupReadConfiguration [35]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupReadConfiguration(
                                    ushort BoardID,
                                    ushort AxesGroupNo,
                                    ushort IDInGroup,
                                    MC_CoordSystem CoordSystem,
                                    ref ushort AxisNo
                                    );
        //5.1.1.48 MC_GroupEnable [36]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupEnable(
                                    ushort BoardID,
                                    ushort AxesGroupNo
                                    );
        //5.1.1.49 MC_GroupDisable [37]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupDisable(
                                    ushort BoardID,
                                    ushort AxesGroupNo
                                    );
        //5.1.1.50 MC_MoveLinearAbsolute [43]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_MoveLinearAbsolute(
                                    ushort BoardID,
                                    ushort AxesGroupNo,
                                    ushort PositionCount,
                                    double[] PositionData,
                                    double Velocity,
                                    double Acceleration,
                                    double Deceleration,
                                    double Jerk,
                                    MC_CoordSystem CoordSystem,        //Coordination System 을 지정한다. //ACS, MCS, PCS
                                    MC_BUFFER_MODE BufferMode,         //Buffer Mode 를 지정한다.
                                    MC_TRANSITION_MODE TransitionMode, //Transition Mode 를 지정한다. 
                                    ushort TransitionParameterCount,   //TransitionParameter 의 Count 를 명시한다.
                                    double[] TransitionParameter       //TransitionParameterCount 에 명시된 크기 만큼의 Data 를 입력한다.
                                    );
        //5.1.1.51 MC_GroupHalt [42]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupHalt(
                                    ushort BoardID,
                                    ushort AxesGroupNo,
                                    double Deceleration,
                                    double Jerk,
                                    MC_BUFFER_MODE BufferMode         //Buffer Mode 를 지정한다.
                                    );
        //5.1.1.52 MC_GroupStop [41]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupStop(
                                    ushort BoardID,
                                    ushort AxesGroupNo,
                                    bool Execute,
                                    double Deceleration,
                                    double Jerk
                                    );
        //5.1.1.53 MC_MoveCircularAbsolute2D [44]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_MoveCircularAbsolute2D(
                                    ushort BoardID,
                                    ushort AxesGroupNo,
                                    MC_CIRC_MODE CircMode,
                                    MC_CIRC_PATHCHOICE PathChoice,
                                    double[] AuxPoint,
                                    double[] EndPoint,
                                    double Angle,
                                    double Velocity,
                                    double Acceleration,
                                    double Deceleration,
                                    double Jerk,
                                    MC_CoordSystem CordSystem,
                                    MC_BUFFER_MODE BufferMode,
                                    MC_TRANSITION_MODE TransitionMode,
                                    ushort TransitionParamCount,
                                    double[] TransitionParameter
                                    );
        //5.1.1.54 MC_GroupStop [38]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupReadStatus(
                                    ushort BoardID,
                                    ushort AxesGroupNo,
                                    ref uint pGroupStatus
                                    );

        //5.1.1.55 MC_GroupReadError [39]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupReadError(
                                    ushort BoardID,
                                    ushort AxesGroupNo,
                                    ref ushort pErrorID,
                                    ref ushort pErrorInfo0,
                                    ref ushort pErrorInfo1
                                    );
        //5.1.1.56 MC_GroupReset [40]
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupReset(
                                    ushort BoardID,
                                    ushort AxesGroupNo
                                    );

        //5.1.1.57 MC_GroupReadProfileData []
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupReadProfileData (
            ushort BoardID,
            ushort AxesGroupNo,
            ref ushort AxisCount,
            ref uint TimeTick,
            double[,] ProfileDataArray
            );

        //5.1.1.58 MC_GroupReadInfo []
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupReadInfo (
            ushort BoardID,
            ushort AxesGroupNo,
            ref ushort LastIdentNum,
            ushort[] IdentAxisNumList
            );

        //5.1.1.59 MC_ReadAllStatus []
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadAllStatus (
            ushort BoardID,
            uint[] StatusData
            );

        //5.1.1.60 MC_GroupSetRawDataMode []
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupSetRawDataMode(
                ushort BoardID,
                ushort AxesGroupNo,
                MC_RAW_DATA_MODE Mode,
                bool Enable,				// Raw data mode enable : 0:Disable, 1:Enable
                byte ReservedZero1,		// MC_COORDSYSTEM CoordSystem,
                byte ReservedZero2			// MC_BUFFER_MODE BufferMode
                );

        ////5.1.1.61 MC_GroupReadRawDataStatus [] : 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupReadRawDataStatus(
                ushort BoardID,
                ushort AxesGroupNo,
                ref MC_AxesGroupRawDataStatus AxesGroupRawDataStatus
                );

        //5.1.1.62 MC_GroupClearRawData [] : 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupClearRawData(
                ushort BoardID,
                ushort AxesGroupNo,
                ushort ReservedZero1,
                ushort ReservedZero2
                );

        //5.1.1.63 MC_GroupSetRawData [] : 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupSetRawData(
                ushort BoardID,
                ushort AxesGroupNo,
                MC_SAVE_MODE SaveMode,
                ushort IndexNum,
                ushort RawDataSize,
                ushort RawDataCount,
                ref byte RawDataArray,
                ref ushort StoredNum
                );


        //5.1.1.64 MC_ReadRemainBuffer [] : 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_ReadRemainBuffer(
                ushort BoardID,
                ushort AxesNo,
                ref uint BufferSize
                );

        //5.1.1.65 MC_GroupReadRemainBuffer [] : 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GroupReadRemainBuffer(
                ushort BoardID,
                ushort AxesGroupNo,
                ref uint BufferSize
                );

        //5.1.1.66 MC_SensorStop [] : 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_SensorStop(
                ushort BoardID,
                ushort AxesNo,
                bool Enable				// Sensor enable : 0:Disable, 1:Enable
                );
        //===========================================================================
        //---------------------------------------------------------------------------
        // Device IO APIs
        //---------------------------------------------------------------------------
        //MC_GetIOState
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_GetIOState(
                                    ushort BoardID,
                                    uint type,
                                    ref uint State
                                    );

        // MC_IO_WRITE 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_RAW_WRITE(
                                    ushort BoardID,
                                    uint Offset,
                                    uint Size,
                                    byte[] DataArray
                                    );
        // MC_IO_WRITE 
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_WRITE(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    uint Offset,
                                    uint Size,
                                    byte[] DataArray
                                    );
        // MC_IO_WRITE_BIT
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_WRITE_BIT(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    uint Offset,
                                    byte bitOffset,
                                    bool data
                                    );
        // MC_IO_WRITE_BIT
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_RAW_WRITE_BIT(
                                    ushort BoardID,
                                    uint Offset,
                                    byte bitOffset,
                                    bool data
                                    );
        // MC_IO_WRITE_BYTE
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_WRITE_BYTE(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    uint Offset,
                                    byte data
                                    );
        // MC_IO_WRITE_WORD
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_WRITE_WORD(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    uint Offset,
                                    ushort data
                                    );
        // MC_IO_WRITE_DWORD
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_WRITE_DWORD(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    uint Offset,
                                    ushort data
                                    );

        // MC_IO_READ
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_READ(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ushort BufferInOut,
                                    uint Offset,
                                    uint Size,
                                    byte[] DataArray
                                    );
        // MC_IO_READ
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_RAW_READ(
                                    ushort BoardID,
                                    ushort BufferInOut,
                                    uint Offset,
                                    uint Size,
                                    byte[] DataArray
                                    );
        // MC_IO_READ_BIT
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_READ_BIT(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ushort BufferInOut,
                                    uint Offset,
                                    byte BitOffset,
                                    ref bool data
                                    );
        // MC_IO_READ_BIT
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_RAW_READ_BIT(
                                    ushort BoardID,
                                    ushort BufferInOut,
                                    uint Offset,
                                    byte BitOffset,
                                    ref bool data
                                    );
        // MC_IO_READ_BYTE
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_READ_BYTE(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ushort BufferInOut,
                                    uint Offset,
                                    ref byte data
                                    );
        // MC_IO_READ_WORD
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_READ_WORD(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ushort BufferInOut,
                                    uint Offset,
                                    ref ushort data
                                    );
        // MC_IO_READ_DWORD
        [DllImport(NMC_DLL_NAME)]
        public static extern MC_STATUS MC_IO_READ_DWORD(
                                    ushort BoardID,
                                    ushort EcatAddr,
                                    ushort BufferInOut,
                                    uint Offset,
                                    ref ushort data
                                    );

        /*
                public static extern System.IntPtr _error_message(short code);

                // C# function for error message
                public static short error_message(short code, ref string message)
                {
                    MMCString mmcstr = new MMCString();
                    if (code < 0 || code > Convert.ToInt16(MMCDef.MAX_ERROR_CODE_NUM)) 
                    {
                        code = Convert.ToInt16(MMCDef.MAX_ERROR_CODE_NUM);
                    }
                    message = mmcstr.str_Error[code];
                    return 0;
                }
        */
        #endregion
    }
}
/*
namespace NMCMotionSDK
{
    // MMC String class
    public class NMCString
    {
        // This string table is for option selection.
        #region NMC_string
        public string[] str_Event = 
                        { 
                            "No-Event", //0
                            "Stop",     //1
                            "E-Stop",   //2
                            "Abort"     //3
                        };
        public string[] str_OnOff = 
                        { 
                            "Off",      //0
                            "On"        //1
                        };
        public string[] str_TrueFalse = 
                        { 
                            "False",    //0
                            "True"      //1
                        };
        public string[] str_HighLow = 
                        { 
                            "Low",      //0
                            "High"      //1
                        };
        public string[] str_ActiveLevel = 
                        { 
                            "Low Active",       //0
                            "High Active"       //1
                        };
        public string[] str_Direction = 
                        { 
                            "Clockwise",        //0
                            "Counterclockwise"  //1
                        };
        public string[] str_Gains = 
                        { 
                            "Proportional gain",        //0
                            "Integral gain",            //1
                            "Derivative gain",          //2
                            "Velocity feed forward",    //3 
                            "Integration summing limit" //4 
                        };
        public string[] str_Sign = 
                        {   
                            "Negative",         //0
                            "Positive"          //1
                        };
        public string[] str_MotorType = 
                        { 
                            "Servo",            //0
                            "Stepper",          //1
                            "Micro Stepper"     //2
                        };
        public string[] str_Feedback = 
                        { 
                            "Encoder",          //0
                            "Analog",           //1
                            "Bi-Analog"         //2
                        };
        public string[] str_Polar = 
                        {
                            "Unipolar",         //0
                            "Bipolar"           //1
                        };
        public string[] str_EncoderType = 
                        { 
                            "Open",             //0
                            "Closed",           //1
                            "Semi"              //2
                        };
        public string[] str_ControlType = 
                        { 
                            "Velocity",         //0
                            "Torque"            //1
                        };
        public string[] str_ControlMode = 
                        { 
                            "Standing",         //0
                            "Always"            //1
                        };
        public string[] str_StepMode = 
                        { 
                            "CW and CCW",       //0
                            "Sign and Pulse"    //1
                        };
        public string[] str_Voltage = 
                        {
                            "5",                //0
                            "10"                //1
                        };
        public string[] str_SetReset = 
                        { 
                            "Reset",            //0
                            "Set"               //1
                        };
        public string[] str_Error = 
                        { 
                          "No Error",                                       //0
                          "Boot Memory has been corrupted",                 //1
                          "DPRAM Communication Error",                      //2
                          "Non Existent Axis",                              //3
                          "Illegal Analog Input Channel",                   //4
                          "Invalid I/O Port",                               //5
                          "Illegal Parameter",                              //6
                          "Not Define Map Axis",                            //7
                          "AMP Fault Occured",                              //8
                          "Motion is not completed",                        //9
                          "MMC Board is not exist",                         //10
                          "MMC Boot File Read/Write Error",                 //11
                          "MMC Checksum File Read/Write Error",             //12
                          "MMC Windows NT Driver Open Error",               //13
                          "Event Occured",                                  //14
                          "AMP Drive Power Off Status",                     //15
                          "MMC Data File Save Directory Open Error",        //16
                          "MMC Invalid CPMOTION Group",                     //17
                          "MMC Velocity Illigal Parameter",                 //18
                          "MMC Accel Illigal Parameter",                    //19
                          "MMC Invalid Error Code"                          //20
                        };
        public string[] str_Source =
                        {
                          "ST_NONE        : No problem",                      //0
                          "ST_HOME_SWITCH : Sensing Home Position Sensor",    //1
                          "ST_POS_LIMIT   : Sensing Positive Limit Sensor",   //2
                          "ST_NEG_LIMIT   : Sensing Negative Limit Sensor",   //3
                          "ST_AMP_FALUE   : AMP Fault",                       //4
                          "ST_A_LIMIT     : Acceleration Limit Over",         //5
                          "ST_V_LIMIT     : Velocity Limit Over",             //6
                          "ST_X_NEG_LIMIT : Negative Position Limit Over",    //7
                          "ST_X_POS_LIMIT : Positive Positino Limit Over",    //8
                          "ST_ERROR_LIMIT : Error Limit Over",                //9
                          "ST_PC_COMMAND  : Event occur (set_stop, set_e_stop, set_liner_all_stop)", //10
                          "ST_OUT_OF_FRAME: Frame Buffer is full",            //11
                          "ST_AMP_POWR_OFF: AMP is disabled",                 //12
                          "ST_ABS_COMM_ERR: ABS Encoder communication error", //13
                          "ST_INPOS_ST    : In Position detected",            //14
                          "ST_RUN_STOP_CMD: RUN STOP Error",                  //15
                          "ST_COLLISION_ST: Collision prevent Error"          //16
                        };
        #endregion
    }
 */