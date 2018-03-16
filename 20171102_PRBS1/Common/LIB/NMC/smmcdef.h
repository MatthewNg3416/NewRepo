///////////////////////////////////////////////////////////////////////////////////////////////////
// smmcdef.h : SERCOSLIB.DLL header file
//
//                         Copyright (c) 2006
//          Rockwell Automation Korea-- SEOUL, REPUBLIC OF KOREA
//
// This program is the property of the Rockwell Automation, Inc. and it shall not be reproduced, 
// distributed or used without permission of an authorized Company official. 
// This is an unpublished work subject to Trade Secret and Copyright protection.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" 
{
#endif

#ifndef __SMMC_LIB_H__
#define __SMMC_LIB_H__

#ifdef SERCOSLIB_EXPORTS
    #define SMMC_DEFINED __declspec(dllexport)
#else
    #define SMMC_DEFINED __declspec(dllimport)
#endif

// Type Redefine
typedef unsigned char    INT8U;
typedef signed   char    INT8S;
typedef unsigned short   INT16U;
typedef signed   short   INT16S;
typedef unsigned int     INT32U;
typedef signed   int     INT32S;
typedef float            FP32;
typedef double           FP64;

// Drive Type
#define CSDM_AM_IAM                         1           // csdm-iam or csdm-am module
#define CSDM_IMA                            2           // csdm-i/o module
#define CSDM_PMA                            3           // csdm-pulse module

// Homing Option and Config
#define HOMING_USE_MARKER                   0x0001      // homing by marker signal
#define HOMING_USE_SWITCH                   0x0002      // homing by home switch signal

#define HOMING_DIR_SWITCH_NEG               0x0000      // negative direction for switch homing
#define HOMING_DIR_SWITCH_POS               0x0001      // positive direction for switch homing
#define HOMING_DIR_MARKER_NEG               0x0000      // negative direction for marker homing
#define HOMING_DIR_MARKER_POS               0x0002      // positive direction for marker homing
#define HOMING_DIR_EDGE_NEG                 0x0000      // negative direction for home switch edge
#define HOMING_DIR_EDGE_POS                 0x0004      // positive direction for home switch edge

// Position Capture
#define PC_SW_TYPE_REG1                     0x0000      // reg1 switch
#define PC_SW_TYPE_REG2                     0x0000      // reg2 switch
#define PC_SW_TYPE_POS_LIMIT                0x0001      // Hardware Limit+ switch
#define PC_SW_TYPE_NEG_LIMIT                0x0002      // Hardware Limit- switch
#define PC_SW_TYPE_HOME                     0x0003      // Home switch

#define PC_EDGE_NONE                        0x0000      // not used
#define PC_EDGE_RISE                        0x0001      // rising edge use
#define PC_EDGE_FALL                        0x0002      // falling edge use
#define PC_EDGE_BOTH                        0x0003      // rising + falling edge use

#define PC_SW_RISEFALL_NONE_NONE            0x0000      // switch latched status : rise(0), fall(0)
#define PC_SW_RISEFALL_LATCHED_NONE         0x0001      // switch latched status : rise(1), fall(0)
#define PC_SW_RISEFALL_NONE_LATCHED         0x0002      // switch latched status : rise(0), fall(1)
#define PC_SW_RISEFALL_LATCHED_LATCHED      0x0003      // switch latched status : rise(1), fall(1)

// CSDM Switch Level
#define SWITCH_LOW_ACTIVE                   0x0000
#define SWITCH_HIGH_ACTIVE                  0x0001

// CSDM Switch Status
#define NEGATIVE_SWITCH                     0x0001      // negative limit switch
#define POSITIVE_SWITCH                     0x0002      // positive limit switch
#define HOME_SWITCH                         0x0004      // home switch
#define INPOS_STATE                         0x0010      // in position state
#define SW_NEGATIVE_SWITCH                  0x0040      // software negative limit switch
#define SW_POSITIVE_SWITCH                  0x0080      // software positive limit switch

// Limit Event Use Control
#define HW_POSITIVE_EVENT_USE               0x0001      // use Hardware+ Limit
#define HW_NEGATIVE_EVENT_USE               0x0002      // use Hardware- Limit
#define SW_POSITIVE_EVENT_USE               0x0004      // use Software+ Limit
#define SW_NEGATIVE_EVENT_USE               0x0008      // use Software- Limit
#define HW_POSITIVE_EVENT_NOT_USE           0x0010      // not use Hardware+ Limit
#define HW_NEGATIVE_EVENT_NOT_USE           0x0020      // not use Hardware- Limit
#define SW_POSITIVE_EVENT_NOT_USE           0x0040      // not use Software+ Limit
#define SW_NEGATIVE_EVENT_NOT_USE           0x0080      // not use Software- Limit

// Limit Switch Setting
#define NO_EVENT                            0           // event not use
#define RESERVED_EVENT                      1           // reserved
#define E_STOP_EVENT                        2           // NC stop event
#define ABORT_EVENT                         3           // NC off event
#define DRV_EVENT                           4           // Drive off event

// Limit Switch Event Source
#define SW_POSITIVE_LIMIT_EVENT             0x0001      // software positive limit event
#define SW_NEGATIVE_LIMIT_EVENT             0x0002      // software negative limit event
#define POSITIVE_LIMIT_EVENT                0x0004      // hardware positive limit event
#define NEGATIVE_LIMIT_EVENT                0x0008      // hardware negative limit event

// PTP Command Mode 
#define TRIGGER_INPOSION                    0x0000      // Trigger mode : update after inposition
#define TRIGGER_IMMEDIATE                   0x0010      // Trigger mode : update Immediate
#define CHECK_DEFAULT                       0x0000      // Inposition condition : position 
#define CHECK_POSITION                      0x0100      // Inposition condition : position, time 
#define CHECK_VELOCITY                      0x0200      // Inposition condition : position, velocity 

// System status 
#define PHASE0                              0xE001      // CP0 
#define PHASE0_1                            0xE011      // CP0->CP1
#define PHASE1                              0xE002      // CP1
#define PHASE1_2                            0xE012      // CP1->CP2
#define PHASE2                              0xE003      // CP2
#define PHASE2_3                            0xE013      // CP2->CP3
#define PHASE3                              0xE004      // CP3
#define PHASE3_4                            0xE014      // CP3->CP4
#define PHASE4                              0xE005      // CP4
#define DISCONNECTED_CABLE                  0xE008      // Disconnect fiber optic cable
#define RESET                               0x0000      // reset

// Node status 
#define AXIS_DONE                           0x0001      // axis done state
#define MOTION_DONE                         0x0002      // motion done state
#define IN_POSITION                         0x0004      // in position state
#define IN_MOTION                           0x0008      // in motion state
#define IN_SEQUENCE                         0x0010      // in Sequence state

// Drive status
#define DRIVE_STATUS_FAULT                  0x0001      // drive fault status
#define DRIVE_STATUS_SERVO_ON               0x0002      // drive servo on status
#define DRIVE_STATUS_MAIN_POWER_ON          0x0004      // drive main power on status

// Non-Volitile Parameter Procedure
#define PROC_NV_LOAD_DEFAULT                0x0106      // drive parameter load default
#define PROC_NV_RECALL_SAVED                0x0107      // drive parameter recall saved
#define PROC_NV_SAVE                        0x0108      // drive parameter save

// Position Redefine status
#define POS_REDEFINE_NONE                   0x0000      // position not redefined(default)
#define POS_REDEFINE_EXTERNAL_APP           0x0100      // position re-defined by external application
#define POS_REDEFINE_INTERNAL_APP           0x0200      // position re-defined by internal application

// Motor Type
#define MOTOR_INC                           0x0000      // incremental motor
#define MOTOR_ABS                           0x0001      // absolute motor

// PMA Position Compare Mode
#define PMA_POS_CMP_MODE_LOW				0x0000		// low level
#define PMA_POS_CMP_MODE_HIGH				0x0001		// high level

// Error define
#define WRONG_RING_NUMBER                   0x0001      // wrong ring number
#define WRONG_NODE_NUMBER                   0x0002      // wrong node number
#define WRONG_STAGE_NUMBER                  0x0003      // wrong stage nmuber
#define WRONG_AXIS_NUMBER                   0x0004      // wrong axis number
#define WRONG_RING_CONFIG                   0x0005      // wrong ring configuration
#define WRONG_STAGE_CONFIG                  0x0006      // wrong stage configuration
#define TIME_OUT_ERROR                      0x0007      // time out error
#define CANNOT_READ_NODE_LIST               0x0008      // can not read a node number list
#define CANNOT_READ_STAGE_COUNT             0x0009      // can not read a stage count
#define CANNOT_CHANGE_PHASE0                0x000A      // can not change phase 0
#define CANNOT_CHANGE_PHASE1                0x000B      // can not change phase 1
#define CANNOT_CHANGE_PHASE2                0x000C      // can not change phase 2
#define CANNOT_CHANGE_PHASE3                0x000D      // can not change phase 3
#define CANNOT_CHANGE_PHASE4                0x000E      // can not change phase 4
#define THERE_IS_NO_NMC_BOARD               0x000F      // There is no nmc board
#define DRIVER_IS_NOT_READY                 0x0010      // Driver is not ready to operate
#define THE_FIRST_PARAM_IS_WRONG            0x0011      // The first parameter is wrong
#define THE_SECOND_PARAM_IS_WRONG           0x0012      // The second parameter is wrong
#define THE_THIRD_PARAM_IS_WRONG            0x0013      // The third parameter is wrong
#define THE_FOURTH_PARAM_IS_WRONG           0x0014      // The fourth parameter is wrong
#define THE_FIFTH_PARAM_IS_WRONG            0x0015      // The fifth parameter is wrong
#define THE_SIXTH_PARAM_IS_WRONG            0x0016      // The sixth parameter is wrong
#define THE_SEVENTH_PARAM_IS_WRONG          0x0017      // The seventh parameter is wrong
#define THE_EIGHTH_PARAM_IS_WRONG           0x0018      // The eighth parameter is wrong
#define THE_NINTH_PARAM_IS_WRONG            0x0019      // The ninth parameter is wrong
#define THE_TENTH_PARAM_IS_WRONG            0x001A      // The tenth parameter is wrong
#define IDN_DATA_TYPE_IS_WRONG              0x001B      // The idn data type is not a two bytes long or four bytes long
#define CANNOT_SERVO_ON                     0x001C      // Drive Cannot servo on
#define CANNOT_SERVO_OFF                    0x001D      // Drive Cannot servo off
#define EVENT_WAIT_ABANDONED                0x001E      // In position event object wait abandoned
#define EVENT_WAIT_TIMEOUT                  0x001F      // In position event object wait timeout
#define EVENT_WAIT_FAILED                   0x0020      // In position event object wait failed
#define HOMING_NOT_STOPPED                  0x0021      // Previous homing operation is running
#define API_PARAMETER_RANGE_ERROR           0x0022      // API Parameter range error
#define NOT_AVAILABLE_AMP_STATE             0x0023      // API cannot be done in this amp state  
#define UNDEFINED_SWITCH_STATUS             0x0024      // Undefined switch status 
#define LIBRARY_INIT_ERROR                  0x0025      // Library init fail 
#define NOT_ABSOLUTE_MOTOR                  0x0026      // Motor is not ablosute type

#define NO_IDN_NUMBER                       0x1001      // no ident number
#define WRONG_ACCESS_TO_ELEMENT1            0X1009      // wrong access to element 1
#define NO_NAME                             0x2001      // no name
#define NAME_TRASMISSION_TOO_SHORT          0x2002      // name transmission too short
#define NAME_TRASMISSION_TOO_LONG           0x2003      // name transmission too long
#define NAME_CANNOT_BE_CHANGED              0x2004      // name cannot be changed
#define NAME_WRINTE_PROTECTED               0x2005      // name write protected at this time
#define ATTRIBUTE_TRANSMISSION_TOO_SHORT    0x3002      // attribute transmission too short
#define ATTRIBUTE_TRANSMISSION_TOO_LONG     0x3003      // attribute transmission too long
#define ATTRIBUTE_CANNOT_BE_MODIFIED        0x3004      // attribute cannot be modified
#define ATTRIBUTE_WRITE_PROTECTED           0x3005      // attribute is write protected at this time
#define NO_UNIT                             0x4001      // no units
#define UNIT_TRANSMISSION_TOO_SHORT         0x4002      // unit transmission too short
#define UNIT_TRANSMISSION_TOO_LONG          0x4003      // unit transmission too long
#define UNIT_CANNOT_BE_CHANGED              0x4004      // unit cannot be changed
#define UNIT_WRITE_PROTECTED                0x4005      // unit is write protected at this time
#define NO_MINIMUM_VALUE                    0x5001      // no minimum value
#define MINVALUE_TRANSMISSION_TOO_SHORT     0x5002      // minimum value transmission too short
#define MINVALUE_TRANSMISSION_TOO_LONG      0X5003      // minimum value transmission too long
#define MINVALUE_CANNOT_BE_CHANGED          0x5004      // minimum value cannot be changed
#define MINVALUE_WRITE_PROTECTED            0x5005      // minimum value is write protected at this time
#define NO_MAXMUM_VALUE                     0x6001      // no maximum value
#define MAXVALUE_TRANSMISSION_TOO_SHORT     0x6002      // maximum value transmission too short
#define MAXVALUE_TRANSMISSION_TOO_LONG      0x6003      // maximum value transmission too long
#define MAXVALUE_CANNOT_BE_CHANGED          0x6004      // maximum value cannot be changed
#define MAXVALUE_WRITE_PROTECTED            0x6005      // maximum value is write protected at this time
#define DATA_TRANSMISSION_TOO_SHORT         0x7002      // data transmission too short
#define DATA_TRANSMISSION_TOO_LONG          0x7003      // data transmission too long
#define DATA_CANNOT_BE_CHANGED              0x7004      // data cannot be changed
#define DATA_WRITE_PROTECTED                0x7005      // data write protected at this time
#define DATA_SMALLER_THAN_MINVALUE          0x7006      // data smaller than the min. value
#define DATA_GREATER_THAN_MAXVALUE          0x7007      // data greater than the max. value
#define DATA_NOT_CORRECT                    0x7008      // data not correct
#define DATA_WRITE_PROTECTED_BY_PASSWORD    0x7009      // data write protected by password
#define DATA_CYCLIC_CONFIGURED              0x700a      // data cyclic configured at this time
#define DATA_OUTSIDE_THE_VALID_NUMBER_AREA  0x700c      // data outside the valid number area
#define DATA_CANNOT_BE_CHANGED_IN_HOMING    0x7010      // data can not be changed during homing
#define MOTION_NOT_COMPLETED                0x7011      // user motion does not completed

//Diagnostic Error Define
#define SERVICE_CHANNEL_PRESENTLY_BUSY      0x8001      // service channel presently busy
#define PROBLEM_IN_SERVICE_CHANNEL          0x8002      // problem in service channel
#define DRIVE_ADDRESS_NOT_CORRECT           0x8005      // drive address not corrrect
#define HS_TIMEOUT                          0x8006      // HS timeout
#define AT_FAILURE                          0x8007      // double AT failure
#define FIBER_OPTIC_RING_INTERRUPT          0x8009      // fiber optics cable disconnected
#define TRANSMISSION_ABORTED                0x800b      // transmission aborted(higher priority)
#define ILLEGAL_ACCESS                      0x800c      // illegal access(service channel still active)
#define DIAG_STATUS_CMD_CH_NOT_ACTIVE       0xC003      // command channel presently not active
#define DIAG_STATUS_CLASS_1                 0xD001      // drive error(class 1 diagnostics, S-0-0011)
#define DIAG_STATUS_CP23                    0xD002      // changeover phase 2->3 not possible
#define DIAG_STATUS_CP34                    0xD003      // changeover phase 3->4 not possible
#define DIAG_STATUS_CMD_CANNOT_EXECUTED     0xD004      // command in drive cannot be executed
#define DIAG_STATUS_CONFIG                  0xF001      // configuration error(command/actual value channel)
#define DIAG_STATUS_TIME_SLOT               0xF002      // error in time slot calculation
#define DIAG_STATUS_INCORRECT_PHASE         0xF003      // incorrect phase from NC
#define DIAG_STATUS_INTERNAL                0xF004      // internal error
#define DIAG_STATUS_LIFE_COUNTER            0xF005      // lifecounter error
#define DIAG_STATUS_TIMES_TOO_LONG          0xF006      // copy times too long
#define DIAG_STATUS_CHECKSUM_ERR            0xF007      // checksum error(Y parameter)
#define DIAG_STATUS_DOUBLE_MST_OR_SYNCIN    0xF008      // double MST failure or input signal SYNCIN failed
#define DIAG_STATUS_MMI_BUSY                0xF015      // mmi busy
#define DIAG_STATUS_NC_COMM_ERROR           0xF016      // non cycle communication error
#define DIAG_STATUS_ILLEGAL_ACCESS_CP0      0xF040      // illegal access in CP0
#define DIAG_STATUS_ILLEGAL_ACCESS_CP1      0xF041      // illegal access in CP1
#define DIAG_STATUS_ILLEGAL_ACCESS_CP23     0xF052      // illegal access in CP2->CP3
#define DIAG_STATUS_ILLEGAL_ACCESS_CP34     0xF053      // illegal access in CP3->CP4

// Board to Device Driver Error
#define ENMC_CMD_TIMEOUT_SERCOS_COMM        0xFF96      // (-106): procedure command timeout
#define ENMC_INV_DATA_SERCOS_COMM           0xFF97      // (-105): invalid data
#define ENMC_NC_TIMEOUT_SERCOS_COMM         0xFF98      // (-104): NC timeout 
#define ENMC_MMI_TIMEOUT_SERCOS_COMM        0xFF99      // (-103): MMI timeout 
#define ENMC_NOINIT_SERCOS_COMM             0xFF9A      // (-102): board not initizlied 
#define ENMC_INIT_SERCOS_COMM               0xFF9B      // (-101): init error 
#define ENMC_TIMEOUT_SERCOS_COMM            0xFF9C      // (-100): timeout

// Homing Error Define
#define HOMING_NO_ERROR                     0x0000      // No Homing Error
#define HOMING_ERROR_AMP_FAULT              0xFFF1      // Amp Fault while homing
#define HOMING_ERROR_EVENT_STATUS           0xFFF2      // EVENT Status is not cleared
#define HOMING_ERROR_EVENT_SETTING          0xFFF3      // EVENT does not set to NO_EVENT
#define HOMING_ERROR_USER_CANCEL            0xFFF4      // Homing User Cancel
#define HOMING_ERROR_SHORT_DISTANCE         0xFFF5      // Short Homing Distance
#define HOMING_ERROR_CMD_BUF                0xFFF6      // Motion Command Buffer not Empty.
#define HOMING_ERROR_DIRECTION              0xFFF7      // Limit Switch disagree with Moving Direction.
#define HOMING_NOT_FOUND_MARKER             0xFFF8      // Not found Marker in 1 turn.
#define HOMING_ERROR_SERVO_OFF              0xFFF9      // Servo off in homing procedure
#define HOMING_ERROR_MODULO_SETTING         0xFFFA      // Modulo Setting is Enable
#define HOMING_ABNORMAL_STATE               0xFFFB      // Homing Procedure State is abnormal.
#define HOMING_ERROR_SWITCH                 0xFFFC      // Not found Switch or abnormal Switch status.
#define HOMING_NOT_FOUND_HOME               0xFFFD      // Not found Home Switch.
#define HOMING_ERROR_SERVO_ON               0xFFFE      // Not Servo on.
#define HOMING_WDT_TIME_OUT                 0xFFFF      // Homing Watch dog time out.

// StartUp
SMMC_DEFINED INT32S smmc_init();
SMMC_DEFINED INT32S smmc_get_ring_list(INT32S *nRingNoList, INT32S *nRingCnt);
SMMC_DEFINED INT32S smmc_get_node_list(INT32S nRingNo, INT32S *nNodeNoList, INT32S *nNodeCnt);
SMMC_DEFINED INT32S smmc_hardware_reset(INT32S nRingNo);

// Drive enable/disable/fault clear
SMMC_DEFINED INT32S smmc_set_amp_enable(INT32S nRingNo, INT32S nNodeNo, INT32S nState);
SMMC_DEFINED INT32S smmc_get_amp_enable(INT32S nRingNo, INT32S nNodeNo, INT32S *nState);
SMMC_DEFINED INT32S smmc_amp_fault_clear(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_emergency_stop(INT32S nRingNo);
SMMC_DEFINED INT32S smmc_get_node_status(INT32S nRingNo, INT32S nNodeNo, INT32U *nStatus);
SMMC_DEFINED INT32S smmc_set_in_position(INT32S nRingNo, INT32S nNodeNo, INT16U InPositionValue, INT16U InTimeValue);
SMMC_DEFINED INT32S smmc_get_in_position(INT32S nRingNo, INT32S nNodeNo, INT16U *InPositionValue, INT16U *InTimeValue);
SMMC_DEFINED INT32S smmc_frames_left(INT32S nRingNo, INT32S nNodeNo, INT32S *nCount);
SMMC_DEFINED INT32S smmc_frames_clear(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_set_vel_override(INT32S nRingNo, INT32S nNodeNo, INT16U nOverride);
SMMC_DEFINED INT32S smmc_get_vel_override(INT32S nRingNo, INT32S nNodeNo, INT16U *nOverride);
SMMC_DEFINED INT32S smmc_set_current_position(INT32S nRingNo, INT32S nNodeNo );

// Stop, pause, resume
SMMC_DEFINED INT32S smmc_set_stop_rate(INT32S nRingNo, INT32S nNodeNo, INT32U nDeceleration);
SMMC_DEFINED INT32S smmc_get_stop_rate(INT32S nRingNo, INT32S nNodeNo, INT32U *nDeceleration);
SMMC_DEFINED INT32S smmc_set_stop(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_set_pause(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_set_resume(INT32S nRingNo, INT32S nNodeNo);

// Homing
SMMC_DEFINED INT32S smmc_homing_start(INT32S nRingNo, INT32S nNodeNo, INT32S *nErrCode);
SMMC_DEFINED INT32S smmc_homing_process(INT32S nRingNo, INT32S nNodeNo, INT32U *nState);
SMMC_DEFINED INT32S smmc_homing(INT32S nRingNo, INT32S nNodeNo, INT32S *nErrCode);
SMMC_DEFINED INT32S smmc_homingn(INT32S nLen, INT32S nRingNo, INT32S *nNodeNo, INT32S *nErrCode);
SMMC_DEFINED INT32S smmc_homing_stop(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_homingn_stop(INT32S nLen, INT32S nRingNo, INT32S *nNodeNo);
SMMC_DEFINED INT32S smmc_is_homing(INT32S nRingNo, INT32S nNodeNo, INT32S *nState);
SMMC_DEFINED INT32S smmc_is_homingn(INT32S nLen, INT32S nRingNo, INT32S *nNodeNo, INT32S *nState);

SMMC_DEFINED INT32S smmc_set_profile_homing(INT32S nRingNo,  INT32S nNodeNo, INT32U nEstopDec,
                                            INT32U nHomeVel, INT32U nHomeAcc, INT32U nHomeDec, 
                                            INT32U nRetVel,  INT32U nRetAcc,  INT32U nRetDec);

SMMC_DEFINED INT32S smmc_get_profile_homing(INT32S nRingNo,  INT32S nNodeNo, INT32U *nEstopDec,
                                            INT32U *nHomeVel,INT32U *nHomeAcc,INT32U *nHomeDec, 
                                            INT32U *nRetVel, INT32U *nRetAcc, INT32U *nRetDec );

SMMC_DEFINED INT32S smmc_set_option_homing(INT32S nRingNo, INT32S nNodeNo, INT32U nDistance, INT32U nTimeout, INT32U nConfig, INT32U nDir);
SMMC_DEFINED INT32S smmc_get_option_homing(INT32S nRingNo, INT32S nNodeNo, INT32U *nDistance, INT32U *nTimeout, INT32U *nConfig, INT32U *nDir);

SMMC_DEFINED INT32S smmc_set_option_homing2(INT32S nRingNo, INT32S nNodeNo, INT32U nDistance, INT32U nTimeout, INT32U nConfig, INT32U nDir);
SMMC_DEFINED INT32S smmc_get_option_homing2(INT32S nRingNo, INT32S nNodeNo, INT32U *nDistance, INT32U *nTimeout, INT32U *nConfig, INT32U *nDir);

SMMC_DEFINED INT32S smmc_get_marker_position(INT32S nRingNo, INT32S nNodeNo, INT32S *nMarkerPos);

SMMC_DEFINED INT32S smmc_get_homing_switch_source(INT32S nRingNo, INT32S nNodeNo, INT32S *nSourceNode);
SMMC_DEFINED INT32S smmc_set_homing_switch_source(INT32S nRingNo, INT32S nNodeNo, INT32S  nSourceNode);

// Motion profile
SMMC_DEFINED INT32S smmc_set_profile(INT32S nRingNo, INT32S nNodeNo, INT32U nVelocity, INT32U nAccelretion, INT32U nDeceleration, INT32U nJerk);
SMMC_DEFINED INT32S smmc_get_profile(INT32S nRingNo, INT32S nNodeNo, INT32U *nVelocity, INT32U *nAcceleration, INT32U *nDeceleration, INT32U *nJerk);

// Motion for the single axis
// PTP move
SMMC_DEFINED INT32S smmc_move(INT32S nRingNo, INT32S nNodeNo, INT32S nPos);
SMMC_DEFINED INT32S smmc_move_2(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nPos1, INT32S nPos2);
SMMC_DEFINED INT32S smmc_move_3(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nNodeNo3, INT32S nPos1, INT32S nPos2, INT32S nPos3);
SMMC_DEFINED INT32S smmc_move_n(INT32S nLen, INT32S nRingNo, INT32S *nNodeNo, INT32S *nPos);
// Relative move
SMMC_DEFINED INT32S smmc_r_move(INT32S nRingNo, INT32S nNodeNo, INT32S nPos);
SMMC_DEFINED INT32S smmc_r_move_2(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nPos1, INT32S nPos2);
SMMC_DEFINED INT32S smmc_r_move_3(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nNodeNo3, INT32S nPos1, INT32S nPos2, INT32S nPos3); 
SMMC_DEFINED INT32S smmc_r_move_n( INT32S nLen, INT32S nRingNo, INT32S *nNodeNo, INT32S *nPos);
// Arc move
SMMC_DEFINED INT32S smmc_arc_2(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nCenterX, INT32S nCenterY, FP64 dAngle);
SMMC_DEFINED INT32S smmc_arc_3(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nNodeNo3, INT32S nCenterX, INT32S nCenterY, INT32S nPos3, FP64 dAngle);
SMMC_DEFINED INT32S smmc_arc_n(INT32S nLen, INT32S nRingNo, INT32S *nNodeNo, INT32S nCenterX, INT32S nCenterY, INT32S *nPos3, FP64 dAngle);
// Spline move
SMMC_DEFINED INT32S smmc_spl_line_move1(INT32S nRingNo, INT32S nNodeNo, INT32S nPos);
SMMC_DEFINED INT32S smmc_spl_line_move2(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nPos1, INT32S nPos2);
SMMC_DEFINED INT32S smmc_spl_line_move3(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nNodeNo3, INT32S nPos1, INT32S nPos2, INT32S nPos3);
SMMC_DEFINED INT32S smmc_spl_line_moven(INT32S nLen, INT32S nRingNo, INT32S *nNodeNo, INT32S *nPos);
// Spline arc move
SMMC_DEFINED INT32S smmc_spl_arc_move2(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nCenterX, INT32S nCenterY, FP64 dAngle);
SMMC_DEFINED INT32S smmc_spl_arc_move3(INT32S nRingNo, INT32S nNodeNo1, INT32S nNodeNo2, INT32S nNodeNo3, INT32S nCenterX, INT32S nCenterY, INT32S nPos3, FP64 dAngle);
SMMC_DEFINED INT32S smmc_spl_arc_moven(INT32S nLen, INT32S nRingNo, INT32S *nNodeNo, INT32S nCenterX, INT32S nCenterY, INT32S *nPos3, FP64 dAngle);
// Velocity move
SMMC_DEFINED INT32S smmc_v_move(INT32S nRingNo, INT32S nNodeNo, INT32S Vel, INT32U Acc, INT32U Dec, INT32U Jerk);
SMMC_DEFINED INT32S smmc_v_move_stop(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_const_move( INT32S nRingNo, INT32S nNodeNo1, INT32S Vel, INT32U Acc);

// Position data
SMMC_DEFINED INT32S smmc_get_position(INT32S nRingNo, INT32S nNodeNo, INT32S *nPosition);
SMMC_DEFINED INT32S smmc_get_command(INT32S nRingNo, INT32S nNodeNo, INT32S *nPosition);

// Limits and Home switch
SMMC_DEFINED INT32S smmc_get_positive_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nEvent);
SMMC_DEFINED INT32S smmc_set_positive_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nEvent);
SMMC_DEFINED INT32S smmc_get_negative_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nEvent);
SMMC_DEFINED INT32S smmc_set_negative_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nEvent);

SMMC_DEFINED INT32S smmc_set_positive_sw_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nEvent);
SMMC_DEFINED INT32S smmc_get_positive_sw_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nEvent);
SMMC_DEFINED INT32S smmc_set_negative_sw_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nEvent);
SMMC_DEFINED INT32S smmc_get_negative_sw_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nEvent);

SMMC_DEFINED INT32S smmc_get_positive_level(INT32S nRingNo, INT32S nNodeNo, INT32S *nLevel);
SMMC_DEFINED INT32S smmc_set_positive_level(INT32S nRingNo, INT32S nNodeNo, INT32S  nLevel);
SMMC_DEFINED INT32S smmc_get_negative_level(INT32S nRingNo, INT32S nNodeNo, INT32S *nLevel);
SMMC_DEFINED INT32S smmc_set_negative_level(INT32S nRingNo, INT32S nNodeNo, INT32S  nLevel);

SMMC_DEFINED INT32S smmc_get_home_level(INT32S nRingNo, INT32S nNodeNo, INT32S *nLevel);
SMMC_DEFINED INT32S smmc_set_home_level(INT32S nRingNo, INT32S nNodeNo, INT32S  nLevel);

SMMC_DEFINED INT32S smmc_set_positive_sw_value(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitPosition);
SMMC_DEFINED INT32S smmc_get_positive_sw_value(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitPosition);
SMMC_DEFINED INT32S smmc_set_negative_sw_value(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitPosition);
SMMC_DEFINED INT32S smmc_get_negative_sw_value(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitPosition);

SMMC_DEFINED INT32S smmc_set_bipolar_torque_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitTorque);
SMMC_DEFINED INT32S smmc_get_bipolar_torque_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitTorque);
SMMC_DEFINED INT32S smmc_set_positive_torque_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitTorque);
SMMC_DEFINED INT32S smmc_get_positive_torque_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitTorque);
SMMC_DEFINED INT32S smmc_set_negative_torque_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitTorque);
SMMC_DEFINED INT32S smmc_get_negative_torque_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitTorque);

SMMC_DEFINED INT32S smmc_set_bipolar_velocity_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitVelocity);
SMMC_DEFINED INT32S smmc_get_bipolar_velocity_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitVelocity);
SMMC_DEFINED INT32S smmc_set_positive_velocity_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitVelocity);
SMMC_DEFINED INT32S smmc_get_positive_velocity_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitVelocity);
SMMC_DEFINED INT32S smmc_set_negative_velocity_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitVelocity);
SMMC_DEFINED INT32S smmc_get_negative_velocity_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitVelocity);

SMMC_DEFINED INT32S smmc_set_following_error_limit(INT32S nRingNo, INT32S nNodeNo, INT32S  nLimitFollowing);
SMMC_DEFINED INT32S smmc_get_following_error_limit(INT32S nRingNo, INT32S nNodeNo, INT32S *nLimitFollowing);

SMMC_DEFINED INT32S smmc_get_axis_event_state(INT32S nRingNo, INT32S nNodeNo, INT32S *nEvent);
SMMC_DEFINED INT32S smmc_get_axis_event_source(INT32S nRingNo, INT32S nNodeNo, INT32S *nEventSource);
SMMC_DEFINED INT32S smmc_clear_event_status(INT32S nRingNo, INT32S nNodeNo);

SMMC_DEFINED INT32S smmc_get_home_switch(INT32S nRingNo, INT32S nNodeNo, INT32S *nState);
SMMC_DEFINED INT32S smmc_get_positive_switch(INT32S nRingNo, INT32S nNodeNo, INT32S *nState);
SMMC_DEFINED INT32S smmc_get_negative_switch(INT32S nRingNo, INT32S nNodeNo, INT32S *nState);

SMMC_DEFINED INT32S smmc_get_limit_event_use(INT32S nRingNo, INT32S nNodeNo, INT32U *nValue);
SMMC_DEFINED INT32S smmc_set_limit_event_use(INT32S nRingNo, INT32S nNodeNo, INT32U nValue);

// Error 
SMMC_DEFINED char  *smmc_get_error_string();
SMMC_DEFINED INT32S smmc_get_comm_status(INT32S nRingNo, INT32S *nStatus);
SMMC_DEFINED INT32S smmc_get_system_error(INT32S nRingNo, INT32S *nErrorCode);
SMMC_DEFINED INT32S smmc_clear_system_error(INT32S nRingNo);
SMMC_DEFINED char  *smmc_get_system_string(INT32S nRingNo);
SMMC_DEFINED INT32S smmc_get_node_error(INT32S nRingNo,  INT32S nNodeNo, INT16U *nErrorCode);
SMMC_DEFINED INT32S smmc_get_node_error_string(INT32S nRingNo, INT16U nErrorCode, char* szError);
SMMC_DEFINED INT32S smmc_get_class_diagonostics(INT32S nRingNo, INT32S nNodeNo, INT32S nClass, INT32S *nDiagnostics);
SMMC_DEFINED INT32S smmc_get_node_string(INT32S nRingNo, INT32S nNodeNo, char *szError);
SMMC_DEFINED INT32S smmc_clear_node_error(INT32S nRingNo, INT32S nNodeNo);

// S-procedure
SMMC_DEFINED INT32S smmc_start_s_proc(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN);
SMMC_DEFINED INT32S smmc_cancel_s_proc(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN);
SMMC_DEFINED INT32S smmc_done_s_proc(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32S *nDone);

// In Position Status
SMMC_DEFINED INT32S smmc_in_position(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_in_position_event(INT32S nRingNo, INT32S nNodeNo, INT32S nTimeout, INT32S *pInPosState);
SMMC_DEFINED INT32S smmc_in_position_events(INT32S nLen, INT32S nRingNo, INT32S *nNodeNo, INT32S nTimeout, INT32S *pInPosState);

// Additional
SMMC_DEFINED INT32S smmc_get_displacement(INT32S nRingNo, INT32S *nDisp);
SMMC_DEFINED INT32S smmc_set_sercos_boot_delay(INT32S nRingNo, INT32S nDelayTime);
SMMC_DEFINED INT32S smmc_get_sercos_boot_delay(INT32S nRingNo, INT32S *nDelayTime);
SMMC_DEFINED INT32S smmc_factory_set(INT32S nRingNo);
SMMC_DEFINED INT32S smmc_save_boot_frame(INT32S nRingNo);

// SERCOS Parameter Read/Write
SMMC_DEFINED INT32S smmc_set_phase(INT32S nRingNo, INT32S nPhaseNo);
SMMC_DEFINED INT32S smmc_get_phase(INT32S nRingNo, INT32S *nPhaseNo);

SMMC_DEFINED INT32S smmc_get_y_parameter_attribute(INT32S nRingNo, INT32S nIDN, INT32U *nAttribute);
SMMC_DEFINED INT32S smmc_set_y_parameter(INT32S nRingNo, INT32S nIDN, INT32U nData);
SMMC_DEFINED INT32S smmc_get_y_parameter(INT32S nRingNo, INT32S nIDN, INT32U *nData);
SMMC_DEFINED INT32S smmc_set_y_parameter_list(INT32S nRingNo, INT32S nIDN, void *data, INT32S nCount);
SMMC_DEFINED INT32S smmc_get_y_parameter_list(INT32S nRingNo, INT32S nIDN, void *data, INT32S *nCount);

SMMC_DEFINED INT32S smmc_get_g_parameter_attribute(INT32S nRingNo, INT32S nStageNo, INT32S nIDN, INT32U *nAttribute);
SMMC_DEFINED INT32S smmc_set_g_parameter(INT32S nRingNo, INT32S nStageNo, INT32S nIDN, INT32U nData);
SMMC_DEFINED INT32S smmc_get_g_parameter(INT32S nRingNo, INT32S nStageNo, INT32S nIDN, INT32U *nData);
SMMC_DEFINED INT32S smmc_set_g_parameter_list(INT32S nRingNo, INT32S nStageNo, INT32S nIDN, void *data, INT32S nCount);
SMMC_DEFINED INT32S smmc_get_g_parameter_list(INT32S nRingNo, INT32S nStageNo, INT32S nIDN, void *data, INT32S *nCount);

SMMC_DEFINED INT32S smmc_get_a_parameter_attribute(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U *nAttribute);
SMMC_DEFINED INT32S smmc_set_a_parameter(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U nData);
SMMC_DEFINED INT32S smmc_get_a_parameter(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U *nData);
SMMC_DEFINED INT32S smmc_set_a_parameter_list(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, void *data, INT32S nCount);
SMMC_DEFINED INT32S smmc_get_a_parameter_list(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, void *data, INT32S *nCount);

SMMC_DEFINED INT32S smmc_get_s_parameter_attribute(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U *nAttribute);
SMMC_DEFINED INT32S smmc_set_s_parameter(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U nData);
SMMC_DEFINED INT32S smmc_get_s_parameter(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U *nData);
SMMC_DEFINED INT32S smmc_set_s_parameter_list(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, void *data, INT32S nCount);
SMMC_DEFINED INT32S smmc_get_s_parameter_list(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, void *data, INT32S *nCount);

SMMC_DEFINED INT32S smmc_get_p_parameter_attribute(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U *nAttribute);
SMMC_DEFINED INT32S smmc_set_p_parameter(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U nData);
SMMC_DEFINED INT32S smmc_get_p_parameter(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32U *nData);
SMMC_DEFINED INT32S smmc_set_p_parameter_list(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, void *data, INT32S nCount);
SMMC_DEFINED INT32S smmc_get_p_parameter_list(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, void *data, INT32S *nCount);

SMMC_DEFINED INT32S smmc_get_a_parameter_bit(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32S nPos, INT32U *nData);
SMMC_DEFINED INT32S smmc_set_a_parameter_bit(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32S nPos, INT32U nData);
SMMC_DEFINED INT32S smmc_get_s_parameter_bit(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32S nPos, INT32U *nData);
SMMC_DEFINED INT32S smmc_set_s_parameter_bit(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32S nPos, INT32U nData);
SMMC_DEFINED INT32S smmc_get_p_parameter_bit(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32S nPos, INT32U *nData);
SMMC_DEFINED INT32S smmc_set_p_parameter_bit(INT32S nRingNo, INT32S nNodeNo, INT32S nIDN, INT32S nPos, INT32U nData);

// Position Compensation
SMMC_DEFINED INT32S smmc_get_revise_pos(INT32S nRingNo, INT32S nTableNo, INT32S *nCount, INT32S *nPositionData, INT32S* nReviseData);
SMMC_DEFINED INT32S smmc_set_revise_pos(INT32S nRingNo, INT32S nTableNo, INT32S nCount, INT32S *nPositionData, INT32S* nReviseData);
SMMC_DEFINED INT32S smmc_get_revise_pos_table_num(INT32S nRingNo, INT32S nNodeNo, INT32S *nTableNo); 
SMMC_DEFINED INT32S smmc_set_revise_pos_table_num(INT32S nRingNo, INT32S nNodeNo, INT32S nTableNo); 
SMMC_DEFINED INT32S smmc_get_revise_pos_enable(INT32S nRingNo, INT32S nNodeNo, INT32S *nState); 
SMMC_DEFINED INT32S smmc_set_revise_pos_enable(INT32S nRingNo, INT32S nNodeNo, INT32S nState); 
SMMC_DEFINED INT32S smmc_get_revise_pos_mapping_count(INT32S nRingNo, INT32S nNodeNo, INT32S *nCount);
SMMC_DEFINED INT32S smmc_set_revise_pos_mapping_count(INT32S nRingNo, INT32S nNodeNo, INT32S nCount);

// Modulo
SMMC_DEFINED INT32S smmc_get_endless_ax(INT32S nRingNo, INT32S nNodeNo, INT32S *nState);
SMMC_DEFINED INT32S smmc_set_endless_ax(INT32S nRingNo, INT32S nNodeNo, INT32S nState);
SMMC_DEFINED INT32S smmc_get_endless_range(INT32S nRingNo, INT32S nNodeNo, INT32S *nModuloVal);
SMMC_DEFINED INT32S smmc_set_endless_range(INT32S nRingNo, INT32S nNodeNo, INT32S nModuloVal);

// Position Capture
SMMC_DEFINED INT32S smmc_pc1_capture(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_pc2_capture(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_pc1_channel_config(INT32S nRingNo, INT32S nNodeNo, INT32S nSwitch, INT32S nEdge);
SMMC_DEFINED INT32S smmc_pc2_channel_config(INT32S nRingNo, INT32S nNodeNo, INT32S nSwitch, INT32S nEdge);
SMMC_DEFINED INT32S smmc_pc1_rise_position(INT32S nRingNo, INT32S nNodeNo, INT32S *nPosition);
SMMC_DEFINED INT32S smmc_pc1_fall_position(INT32S nRingNo, INT32S nNodeNo, INT32S *nPosition);
SMMC_DEFINED INT32S smmc_pc2_rise_position(INT32S nRingNo, INT32S nNodeNo, INT32S *nPosition);
SMMC_DEFINED INT32S smmc_pc2_fall_position(INT32S nRingNo, INT32S nNodeNo, INT32S *nPosition);
SMMC_DEFINED INT32S smmc_pc1_latch_status(INT32S nRingNo, INT32S nNodeNo, INT32S *nStatus);
SMMC_DEFINED INT32S smmc_pc2_latch_status(INT32S nRingNo, INT32S nNodeNo, INT32S *nStatus);

// Utility function
SMMC_DEFINED INT32S smmc_find_device_num(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_set_position(INT32S nRingNo, INT32S nNodeNo, INT32S nRedefinePos);
SMMC_DEFINED INT32S smmc_get_pos_redefine_status(INT32S nRingNo, INT32S nNodeNo, INT32U *nDoneStatus);
SMMC_DEFINED INT32S smmc_set_pos_redefine_status(INT32S nRingNo, INT32S nNodeNo, INT32U nDoneStatus);
SMMC_DEFINED INT32S smmc_set_mdt_bit_value(INT32S nRingNo, INT32S nNodeNo, INT32S nBitPos, INT32U bValue);
SMMC_DEFINED INT32S smmc_get_mdt_bit_value(INT32S nRingNo, INT32S nNodeNo, INT32S nBitPos, INT32U *bValue);
SMMC_DEFINED INT32S smmc_get_smmc_version(INT32S nRingNo, INT32U* nVersion);
SMMC_DEFINED INT32U smmc_get_lib_version();
SMMC_DEFINED INT32S smmc_get_drive_verson(INT32S nRingNo, INT32S nNodeNo, INT32U *nVersionNumber);
SMMC_DEFINED INT32S smmc_get_axis_status(INT32S nRingNo, INT32S nNodeNo, INT32U *nStatus);
SMMC_DEFINED INT32S smmc_get_dewell(INT32S nRingNo, INT32S nNodeNo, INT16U *nDelay);
SMMC_DEFINED INT32S smmc_set_dewell(INT32S nRingNo, INT32S nNodeNo, INT16U nDelay);

// In Position Signal Type
SMMC_DEFINED INT32S smmc_get_in_position_signal_type(INT32S nRingNo, INT32S nNodeNo, INT32S *nType);
SMMC_DEFINED INT32S smmc_set_in_position_signal_type(INT32S nRingNo, INT32S nNodeNo, INT32S nType);

// CSDM Linear Motor Parameter Save
SMMC_DEFINED INT32S smmc_save_linear_motor_parameter(INT32S nRingNo, INT32S nNodeNo);

// Multi Axis Phase Change
SMMC_DEFINED INT32S smmc_set_phasen(INT32S nLen, INT32S* nRingNo, INT32S* nPhaseNo);

// CSDM Drive Name
SMMC_DEFINED INT32S smmc_get_drive_name(INT32S nRingNo, INT32S nNodeNo, char* cDriveName);
SMMC_DEFINED INT32S smmc_set_drive_name(INT32S nRingNo, INT32S nNodeNo, char* cDriveName);

// CSDM Etc. Control
SMMC_DEFINED INT32S smmc_drive_parameter_control(INT32S nRingNo, INT32S nNodeNo, INT32S nProcedureType);
SMMC_DEFINED INT32S smmc_multiturn_clear(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_get_motor_type(INT32S nRingNo, INT32S nNodeNo, INT32S *nMotorType);
SMMC_DEFINED INT32S smmc_position_offset_clear(INT32S nRingNo, INT32S nNodeNo);
SMMC_DEFINED INT32S smmc_get_drive_type(INT32S nRingNo, INT32S nNodeNo, INT32S *nDriveType);

// Digital IO Read, Write and Configuration
SMMC_DEFINED INT32S smmc_io_output_get_word(INT32S nRingNo, INT32S nNodeNo, INT16U nOffset, INT16U *nData);
SMMC_DEFINED INT32S smmc_io_output_set_word(INT32S nRingNo, INT32S nNodeNo, INT16U nOffset, INT16U nData);
SMMC_DEFINED INT32S smmc_io_output_get_bit(INT32S nRingNo, INT32S nNodeNo, INT16U nOffset, INT16U nBitPos, INT16U *nData);
SMMC_DEFINED INT32S smmc_io_output_set_bit(INT32S nRingNo, INT32S nNodeNo, INT16U nOffset, INT16U nBitPos, INT16U nData);
SMMC_DEFINED INT32S smmc_io_input_get_word(INT32S nRingNo, INT32S nNodeNo, INT16U nOffset, INT16U *nData);
SMMC_DEFINED INT32S smmc_io_input_get_bit(INT32S nRingNo, INT32S nNodeNo, INT16U nOffset, INT16U nBitPos, INT16U *nData);
SMMC_DEFINED INT32S smmc_get_useroutput_mask(INT32S nRingNo, INT32S nNodeNo, INT16U nOutputPortNo, INT32U *nMaskValue);
SMMC_DEFINED INT32S smmc_set_useroutput_mask(INT32S nRingNo, INT32S nNodeNo, INT16U nOutputPortNo, INT32U  nValue);

// CSDM-PMA Control
SMMC_DEFINED INT32S smmc_get_pma_encoder_direction(INT32S nRingNo, INT32S nNodeNo, INT16U *nDirection);
SMMC_DEFINED INT32S smmc_set_pma_encoder_direction(INT32S nRingNo, INT32S nNodeNo, INT16U  nDirection);
SMMC_DEFINED INT32S smmc_get_pma_command_direction(INT32S nRingNo, INT32S nNodeNo, INT16U *nDirection);
SMMC_DEFINED INT32S smmc_set_pma_command_direction(INT32S nRingNo, INT32S nNodeNo, INT16U  nDirection);
SMMC_DEFINED INT32S smmc_get_pma_encoder_type(INT32S nRingNo, INT32S nNodeNo, INT32U *nEncType);
SMMC_DEFINED INT32S smmc_set_pma_encoder_type(INT32S nRingNo, INT32S nNodeNo, INT32U  nEncType);
SMMC_DEFINED INT32S smmc_get_pma_encoder_multiple(INT32S nRingNo, INT32S nNodeNo, INT32U *nMultipleFactor);
SMMC_DEFINED INT32S smmc_set_pma_encoder_multiple(INT32S nRingNo, INT32S nNodeNo, INT32U  nMultipleFactor);

// Drive System I/O Level
SMMC_DEFINED INT32S smmc_get_pma_amp_fault_level(INT32S nRingNo, INT32S nNodeNo, INT16U *nLevel);
SMMC_DEFINED INT32S smmc_set_pma_amp_fault_level(INT32S nRingNo, INT32S nNodeNo, INT16U  nLevel);
SMMC_DEFINED INT32S smmc_get_pma_inposition_level(INT32S nRingNo, INT32S nNodeNo, INT16U *nLevel);
SMMC_DEFINED INT32S smmc_set_pma_inposition_level(INT32S nRingNo, INT32S nNodeNo, INT16U  nLevel);
SMMC_DEFINED INT32S smmc_get_pma_fault_reset_level(INT32S nRingNo, INT32S nNodeNo, INT16U *nLevel);
SMMC_DEFINED INT32S smmc_set_pma_fault_reset_level(INT32S nRingNo, INT32S nNodeNo, INT16U  nLevel);
SMMC_DEFINED INT32S smmc_get_pma_amp_enable_level(INT32S nRingNo, INT32S nNodeNo, INT16U *nLevel);
SMMC_DEFINED INT32S smmc_set_pma_amp_enable_level(INT32S nRingNo, INT32S nNodeNo, INT16U  nLevel);

// Position Compare
SMMC_DEFINED INT32S smmc_set_pma_position_compare_config(INT32S nRingNo, INT32S nNodeNo, INT32S nStartIndex, INT32S nTotalCount);
SMMC_DEFINED INT32S smmc_set_pma_position_compare_enable(INT32S nRingNo, INT32S nNodeNo, INT32S nState, INT32S nValue);
SMMC_DEFINED INT32S smmc_set_pma_position_compare_table(INT32S nRingNo, INT32S nNodeNo, INT32S nIndex, INT32S nBitNum, INT32S nMode, INT32S nPos); 

#endif  // end of __SMMC_LIB_H__

#ifdef __cplusplus
}
#endif