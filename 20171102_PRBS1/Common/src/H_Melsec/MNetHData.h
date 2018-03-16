//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for T7-2
// MNetHData.h: interface for the MNetHData class.
//
//
//		Version		Updated		 Author		 Note
//      -------     -------      ------      ----
//		   1.0      2005/12/20   Grouchy	 Create	
//		   1.1      2006/02/27   cha		 Modify	(BitSignals)
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNETHDATA_H__BE15CBBD_BFA0_48BF_9074_C57D719A5C5B__INCLUDED_)
#define AFX_MNETHDATA_H__BE15CBBD_BFA0_48BF_9074_C57D719A5C5B__INCLUDED_

#include "DefSystem.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Error Code List
#define	ERR_NO					0

#define	ERR_T1_TIMEOUT			-10001
#define	ERR_T2_TIMEOUT			-10002
#define	ERR_T3_TIMEOUT			-10003
#define	ERR_DEFAULT_TIMEOUT		-10004

#define	ERR_NOT_READY_TRANSFER	-10101
#define	ERR_NOT_SET_GLASS_DATA	-10102


#define	B_L_SIZE				48
#define	B_L01_U1				0x0000
#define	B_L01_L1				0x0090

#define	B_L02_U1				0x0180
#define	B_L02_L1				0x0210

#define	B_L03_U1				0x0300
#define	B_L03_L1				0x0390

#define	B_L04_U1				0x0480
#define	B_L04_L1				0x0510

#define	B_L05_U1				0x0600
#define	B_L05_L1				0x0690

#define	B_L06_U1				0x0780
#define	B_L06_L1				0x0810

#define	B_L07_U1				0x0900
#define	B_L07_L1				0x0990

#define	B_L08_U1				0x0A80
#define	B_L08_L1				0x0B10

#define	B_L09_U1				0x0C00
#define	B_L09_L1				0x0C90

#define	B_L10_U1				0x0D80
#define	B_L10_L1				0x0E10

#define	B_L11_U1				0x0F00
#define	B_L11_L1				0x0F90

#define	B_L12_U1				0x1080
#define	B_L12_L1				0x1110

#define	B_L13_U1				0x1200
#define	B_L13_L1				0x1290

#define	B_L14_U1				0x1380
#define	B_L14_L1				0x1410

#define	B_L15_U1				0x1500
#define	B_L15_L1				0x1590

#define	B_L16_U1				0x1680
#define	B_L16_L1				0x1710

#define	B_L17_U1				0x1800
#define	B_L17_L1				0x1890

#define	B_L18_U1				0x1980
#define	B_L18_L1				0x1A10

#define	B_L19_U1				0x1B00
#define	B_L19_L1				0x1B90

#define	B_L20_U1				0x1C80
#define	B_L20_L1				0x1D10

#define	B_L21_U1				0x1E00
#define	B_L21_L1				0x1E90

#define	B_L22_U1				0x1F80
#define	B_L22_L1				0x2010

#define	B_L23_U1				0x2100
#define	B_L23_L1				0x2190

#define	B_L24_U1				0x2280
#define	B_L24_L1				0x2310

#define	B_L25_U1				0x2400
#define	B_L25_L1				0x2490

//SJ_YYK 160225 Add..
#define	B_M01_U1				0x0000
#define	B_M01_L1				0x0180

#define	B_M02_U1				0x0300
#define	B_M02_L1				0x0480

#define	B_M03_U1				0x0600
#define	B_M03_L1				0x0780

#define	B_M04_U1				0x0900
#define	B_M04_L1				0x0A80

#define	B_M05_U1				0x0C00
#define	B_M05_L1				0x0D80

#ifndef DEF_MELSEC_VER_2014

#define	W_EO_SIZE				48
#define	W_ER_SIZE				48
#define	W_GS_SIZE				80
#define	W_U_SIZE				16
#define	W_L_SIZE				112

#define	W_L01_U1				0x0000
#define	W_L01_L1				0x0030
#define	W_L01_EO				0x0180
#define	W_L01_ER				0x01B0
#define	W_L01_GS				0x01E0

#define	W_L02_U1				0x0240
#define	W_L02_L1				0x0270
#define	W_L02_EO				0x03C0
#define	W_L02_ER				0x03F0
#define	W_L02_GS				0x0420

#define	W_L03_U1				0x0480
#define	W_L03_L1				0x04B0
#define	W_L03_EO				0x0600
#define	W_L03_ER				0x0630
#define	W_L03_GS				0x06B0

#define	W_L04_U1				0x06C0
#define	W_L04_L1				0x06F0
#define	W_L04_EO				0x0840
#define	W_L04_ER				0x0870
#define	W_L04_GS				0x08A0

#define	W_L05_U1				0x0900
#define	W_L05_L1				0x0930
#define	W_L05_EO				0x0A80
#define	W_L05_ER				0x0AB0
#define	W_L05_GS				0x0AE0

#define	W_L06_U1				0x0B40
#define	W_L06_L1				0x0B70
#define	W_L06_EO				0x0CC0
#define	W_L06_ER				0x0CF0
#define	W_L06_GS				0x0D20

#define	W_L07_U1				0x0D80
#define	W_L07_L1				0x0DB0
#define	W_L07_EO				0x0F00
#define	W_L07_ER				0x0F30
#define	W_L07_GS				0x0F60

#define	W_L08_U1				0x0FC0
#define	W_L08_L1				0x0FF0
#define	W_L08_EO				0x1140
#define	W_L08_ER				0x1170
#define	W_L08_GS				0x11A0

#define	W_L09_U1				0x1200
#define	W_L09_L1				0x1230
#define	W_L09_EO				0x1380
#define	W_L09_ER				0x13B0
#define	W_L09_GS				0x13E0

#define	W_L10_U1				0x1440
#define	W_L10_L1				0x1470
#define	W_L10_EO				0x15C0
#define	W_L10_ER				0x15F0
#define	W_L10_GS				0x1620

#define	W_L11_U1				0x1680
#define	W_L11_L1				0x16B0
#define	W_L11_EO				0x1800
#define	W_L11_ER				0x1830
#define	W_L11_GS				0x1860

#define	W_L12_U1				0x18C0
#define	W_L12_L1				0x18F0
#define	W_L12_EO				0x1A40
#define	W_L12_ER				0x1A70
#define	W_L12_GS				0x1AA0

#define	W_L13_U1				0x1B00
#define	W_L13_L1				0x1B30
#define	W_L13_EO				0x1C80
#define	W_L13_ER				0x1CB0
#define	W_L13_GS				0x1CE0

#define	W_L14_U1				0x1D40
#define	W_L14_L1				0x1D70
#define	W_L14_EO				0x1EC0
#define	W_L14_ER				0x1EF0
#define	W_L14_GS				0x1F20

#define	W_L15_U1				0x1F80
#define	W_L15_L1				0x1FB0
#define	W_L15_EO				0x2100
#define	W_L15_ER				0x2130
#define	W_L15_GS				0x2160

#define	W_L16_U1				0x21C0
#define	W_L16_L1				0x21F0
#define	W_L16_EO				0x2340
#define	W_L16_ER				0x2370
#define	W_L16_GS				0x23A0

#define	W_L17_U1				0x2400
#define	W_L17_L1				0x2430
#define	W_L17_EO				0x2580
#define	W_L17_ER				0x25B0
#define	W_L17_GS				0x25E0

#define	W_L18_U1				0x2640
#define	W_L18_L1				0x2670
#define	W_L18_EO				0x27C0
#define	W_L18_ER				0x27F0
#define	W_L18_GS				0x2820

#define	W_L19_U1				0x2880
#define	W_L19_L1				0x28B0
#define	W_L19_EO				0x2A00
#define	W_L19_ER				0x2A30
#define	W_L19_GS				0x2A60

#define	W_L20_U1				0x2AC0
#define	W_L20_L1				0x2AF0
#define	W_L20_EO				0x2C40
#define	W_L20_ER				0x2C70
#define	W_L20_GS				0x2AC0

#define	W_L21_U1				0x2D00
#define	W_L21_L1				0x2D30
#define	W_L21_EO				0x2E80
#define	W_L21_ER				0x2EB0
#define	W_L21_GS				0x2EE0

#define	W_L22_U1				0x2F40
#define	W_L22_L1				0x2F70
#define	W_L22_EO				0x30C0
#define	W_L22_ER				0x30F0
#define	W_L22_GS				0x3120

#define	W_L23_U1				0x3180
#define	W_L23_L1				0x31B0
#define	W_L23_EO				0x3300
#define	W_L23_ER				0x3330
#define	W_L23_GS				0x3360

#define	W_L24_U1				0x33C0
#define	W_L24_L1				0x33F0
#define	W_L24_EO				0x3540
#define	W_L24_ER				0x3570
#define	W_L24_GS				0x35A0

#define	W_L25_U1				0x3600
#define	W_L25_L1				0x3630
#define	W_L25_EO				0x3780
#define	W_L25_ER				0x37B0
#define	W_L25_GS				0x37E0

#else //DEF_MELSEC_VER_2014 AI¶§

#define	W_EO_SIZE				32
#define	W_ER_SIZE				32
#define	W_GS_SIZE				80
#define	W_U_SIZE				16
#define	W_L_SIZE				116

#define	W_L01_U1				0x0000
#define	W_L01_L1				0x0030
#define	W_L01_EO				0x018C
#define	W_L01_ER				0x01AC
#define	W_L01_GS				0x01CC

#define	W_L02_U1				0x0296
#define	W_L02_L1				0x02C6
#define	W_L02_EO				0x0422
#define	W_L02_ER				0x0442
#define	W_L02_GS				0x0462

#define	W_L03_U1				0x052C
#define	W_L03_L1				0x055C
#define	W_L03_EO				0x06B8
#define	W_L03_ER				0x06D8
#define	W_L03_GS				0x06F8

#define	W_L04_U1				0x07C2
#define	W_L04_L1				0x07F2
#define	W_L04_EO				0x094E
#define	W_L04_ER				0x096E
#define	W_L04_GS				0x098E

#define	W_L05_U1				0x0A58
#define	W_L05_L1				0x0A88
#define	W_L05_EO				0x0BE4
#define	W_L05_ER				0x0C04
#define	W_L05_GS				0x0C24

#define	W_L06_U1				0x0CEE
#define	W_L06_L1				0x0D1E
#define	W_L06_EO				0x0E7A
#define	W_L06_ER				0x0E9A
#define	W_L06_GS				0x0EBA

#define	W_L07_U1				0x0F84
#define	W_L07_L1				0x0FB4
#define	W_L07_EO				0x1110
#define	W_L07_ER				0x1130
#define	W_L07_GS				0x1150

#define	W_L08_U1				0x121A
#define	W_L08_L1				0x124A
#define	W_L08_EO				0x13A6
#define	W_L08_ER				0x13C6
#define	W_L08_GS				0x13E6

#define	W_L09_U1				0x14B0
#define	W_L09_L1				0x14E0
#define	W_L09_EO				0x163C
#define	W_L09_ER				0x165C
#define	W_L09_GS				0x167C

#define	W_L10_U1				0x1746
#define	W_L10_L1				0x1776
#define	W_L10_EO				0x18D2
#define	W_L10_ER				0x18F2
#define	W_L10_GS				0x1912

#define	W_L11_U1				0x19DC
#define	W_L11_L1				0x1A0C
#define	W_L11_EO				0x1B68
#define	W_L11_ER				0x1B88
#define	W_L11_GS				0x1BA8

#define	W_L12_U1				0x1C72
#define	W_L12_L1				0x1CA2
#define	W_L12_EO				0x1DFE
#define	W_L12_ER				0x1E1E
#define	W_L12_GS				0x1E3E

#define	W_L13_U1				0x1F08
#define	W_L13_L1				0x1F38
#define	W_L13_EO				0x2094
#define	W_L13_ER				0x20B4
#define	W_L13_GS				0x20D4

#define	W_L14_U1				0x219E
#define	W_L14_L1				0x21CE
#define	W_L14_EO				0x2324
#define	W_L14_ER				0x2344
#define	W_L14_GS				0x2364

#define	W_L15_U1				0x2434
#define	W_L15_L1				0x2464
#define	W_L15_EO				0x25C0
#define	W_L15_ER				0x25E0
#define	W_L15_GS				0x2600

#define	W_L16_U1				0x26CA
#define	W_L16_L1				0x26FA
#define	W_L16_EO				0x2856
#define	W_L16_ER				0x2876
#define	W_L16_GS				0x2896

#define	W_L17_U1				0x2960
#define	W_L17_L1				0x2990
#define	W_L17_EO				0x2AEC
#define	W_L17_ER				0x2B0C
#define	W_L17_GS				0x2B2C

#define	W_L18_U1				0x2B8C
#define	W_L18_L1				0x2BBC
#define	W_L18_EO				0x2D18
#define	W_L18_ER				0x2D38
#define	W_L18_GS				0x2D58

#define	W_L19_U1				0x2E22
#define	W_L19_L1				0x2E52
#define	W_L19_EO				0x2FAE
#define	W_L19_ER				0x2FCE
#define	W_L19_GS				0x2FEE

#define	W_L20_U1				0x304E
#define	W_L20_L1				0x307E
#define	W_L20_EO				0x31DA
#define	W_L20_ER				0x31FA
#define	W_L20_GS				0x321A

#define	W_L21_U1				0x32E4
#define	W_L21_L1				0x3314
#define	W_L21_EO				0x3470
#define	W_L21_ER				0x3490
#define	W_L21_GS				0x34B0

#define	W_L22_U1				0x357A
#define	W_L22_L1				0x35AA
#define	W_L22_EO				0x3706
#define	W_L22_ER				0x3726
#define	W_L22_GS				0x3746

#define	W_L23_U1				0x3810
#define	W_L23_L1				0x3840
#define	W_L23_EO				0x399C
#define	W_L23_ER				0x39BC
#define	W_L23_GS				0x39DC

#define	W_L24_U1				0x3AA6
#define	W_L24_L1				0x3AD6
#define	W_L24_EO				0x3C32
#define	W_L24_ER				0x3C52
#define	W_L24_GS				0x3C72

#define	W_L25_U1				0x3D3C
#define	W_L25_L1				0x3D6C
#define	W_L25_EO				0x3EC8
#define	W_L25_ER				0x3EE8
#define	W_L25_GS				0x3F08
#endif

#define W_MAT_SIZE				277
#define W_FDC_SIZE				88

//171011 Jas_SYS
#define W_L01_MT				0x0000
#define W_L02_MT				0x0D80
#define W_L03_MT				0x1B00
#define W_L04_MT				0x2880

//NSMC KJS for SESL
#define NSMC_CONTROL_EQUIP		1//Loader

#define	B_NSMC_SIZE				0x0010
#define	B_NSMC_01				0x2580

#define	W_NSMC_SIZE				0x0030
#define	W_NSMC_01				0x3A80


//	User defined enum variable
enum ELocs
{
	eUpper = 0,	
	eLower = 1,
	eOwn,
	eUpperOwn,
	eLowerOwn
};

enum EEqLocs
{
	eUpperEq = 0,
	eLowerEq = 1,
	eOwnEq
};

enum EUnitTypes
{
	eUnitNothing = 0,
	eRobot = 1,
	eStage
};

enum EEqStateTypes
{
	eNothing_EqStateType = 0,
	eEqStateOwn_EqStateType = 1,
	eEqStateRelated_EqStateType
};

enum EAreaTypes
{
	eAreaNothing = 0,
	eRobotArmStateBit_Area = 1,
	eStageStateBit_Area,
	eToUpperEqpBit_Area,
	eToLowerEqpBit_Area,
	eEqpState_Area,
	eGlassSummary_Area,
	eToUpperEqp_Area,
	eToLowerEqp_Area,
	ePortInfomation_Area,
};

enum ERobotArmStateBits
{
	eNothing_Robot = -1,
	eRobotAbnormal_Robot = 32,	
	eTypeOfArm_Robot,
	eTypeOfStageConveyor_Robot,
	eArm1Violate_Robot,
	eArm2Violate_Robot,
	eArm1FoldComplete_Robot,
	eArm2FoldComplete_Robot,
	eArm1GlassCheckSensor_Robot,
	eArm2GlassCheckSensor_Robot,
	eReserved0_Robot,
	eReserved1_Robot,
	eReserved2_Robot,
	eManualOperation_Robot,
	eRobotDirection_Robot,
	eReserved3_Robot,
	eReserved4_Robot
};

enum EStageStateBits
{
	eNothing_Stage = -1,
	eStageAbnormal_Stage = 32,
	eTypeOfArm_Stage,
	eTypeOfStageConveyor_Stage,
	eEmpty_Stage,
	eIdle_Stage,
	eBusy_Stage,
	ePinUp_Stage,
	ePinDown_Stage,
	eStopperUp_Stage,
	eStopperDown_Stage,
	eDoorOpen_Stage,
	eDoorClose_Stage,
	eManualOperation_Stage,	
	eBodyMoving_Stage,	
	eGlassCheckSensorOn_Stage,
	eReserved0_Stage
};

enum EToUpperEqBits
{
	eNothing_UpperEqBit = -1,
	eReserved0_UpperEqBit = 0,
	eMachinePause_UpperEqBit = 1,
	eMachineDown_UpperEqBit,
	eMachineAlarm_UpperEqBit,	
	eReceiveAble_UpperEqBit,
	eReceiveStart_UpperEqBit,
	eReceiveComplete_UpperEqBit,
	eExchangeFlag_UpperEqBit,
	eReturnSendStart_UpperEqBit,
	eReturnSendComplete_UpperEqBit,
	eAction1Complete_UpperEqBit,
	eAction2Complete_UpperEqBit,
	eReserved1_UpperEqBit,
	eReserved2_UpperEqBit,
	eReceiveRefuse_UpperEqBit,
	eGlassIdReadComplete_UpperEqBit,
	eLoadingStop_UpperEqBit,
	eTransferStop_UpperEqBit,
	eImmediatelyPauseRequest_UpperEqBit,
	eImmediatelyStopRequest_UpperEqBit,
	eReceiveAbleRemainedStep0_UpperEqBit,
	eReceiveAbleRemainedStep1_UpperEqBit,
	eReceiveAbleRemainedStep2_UpperEqBit,
	eReceiveAbleRemainedStep3_UpperEqBit,
	eHandShakeCancelRequest_UpperEqBit,
	eHandShakeAbortRequest_UpperEqBit,
	eHandShakeResumeRequest_UpperEqBit,
	eHandShakeRecoveryAckReply_UpperEqBit,
	eHandShakeRecoveryNckReply_UpperEqBit,
	eReserved3_UpperEqBit,	
	eReserved4_UpperEqBit,	
	eReserved5_UpperEqBit
};

enum EToLowerEqBits
{
	eNothing_LowerEqBit = -1,
	eReserved0_LowerEqBit = 0,
	eMachinePause_LowerEqBit = 1,
	eMachineDown_LowerEqBit,
	eMachineAlarm_LowerEqBit,	
	eSendAble_LowerEqBit,
	eSendStart_LowerEqBit,
	eSendComplete_LowerEqBit,
	eExchangeFlag_LowerEqBit,
	eReturnReceiveStart_LowerEqBit,
	eReturnReceiveComplete_LowerEqBit,
	eAction1Request_LowerEqBit,
	eAction2Request_LowerEqBit,	
	eReserved1_LowerEqBit,
	eReserved2_LowerEqBit,
	eWorkStart_LowerEqBit,
	eWorkCancel_LowerEqBit,
	eReserved3_LowerEqBit,
	eReserved4_LowerEqBit,
	eImmediatelyPauseRequest_LowerEqBit,
	eImmediatelyStopRequest_LowerEqBit,
	eSendAbleRemainedStep0_LowerEqBit,
	eSendAbleRemainedStep1_LowerEqBit,
	eSendAbleRemainedStep2_LowerEqBit,
	eSendAbleRemainedStep3_LowerEqBit,
	eHandShakeCancelRequest_LowerEqBit,
	eHandShakeAbortRequest_LowerEqBit,
	eHandShakeResumeRequest_LowerEqBit,
	eHandShakeRecoveryAckReply_LowerEqBit,
	eHandShakeRecoveryNckReply_LowerEqBit,	
	eReserved5_LowerEqBit,
	eReserved6_LowerEqBit,
	eReserved7_LowerEqBit
};

enum EIfEqBits
{
	eSendAble = 4,
	eReceiveAble = 4,
	eReceiveStart = 5,
	eSendStart = 5,
	eReceiveComplete = 6,
	eSendComplete = 6,
	ePanelExistenceInfo1 = 12,  //AOC Vac Info Add..
	ePanelExistenceInfo2 = 13,
	eWorkStart_LowerEq = 14,
	eImmediatelyPauseRequest = 18,
	eTurnSafetyAOC1 = 20, // AOC Turn Interlock
	eTurnSafetyAOC2 = 21,
	eUseOnlyFrontFeeder1 = 22, //AOC Material Check Bit
	eUseOnlyFrontFeeder2 = 23, //AOC Material Check Bit
	eUseAllFrontFeeder = 24, //AOC Material Check Bit
	eUseOnlyRearFeeder1 = 25, //AOC Material Check Bit
	eUseOnlyRearFeeder2 = 26, //AOC Material Check Bit
	eUseAllRearFeeder = 27, //AOC Material Check Bit
	eArm1FoldComplete = 37,
	ePinUp = 38,
	eGlassCheckSensorOn = 46
};


//NSMC KJS
enum ENSMCLocalBits
{
	eModelChangeReserve = 1, 
	eModelChangeStart = 2,
	eModelChangeComplete = 3,
	eModelChangeDisable = 4, 
	// Add. 110525 기종변경 예약 Bit 추가
	eModelReserveComplete = 5,
	eModelReserveDisable = 6,
};

enum ENSMCControlBits
{
	eLocalCommand1 = 0,
	eLocalCommand2,
	eLocalCommand3,
	eLocalCommand4,
	eLocalCommand5,
	eLocalCommand6,
	eLocalCommand7,
	eLocalCommand8,
	eLocalCommand9,
	eLocalCommand10,
	eLocalCommand11,
	eLocalCommand12,
	eLocalCommand13,
	eLocalCommand14,
	eLocalCommand15,
	eLocalCommand16,
};


enum EToUpperEqs
{
	eNothing_UpperEq = -1,					
	eReadingGlassID_UpperEq = 6,					
	eReadingGlassIDType_UpperEq = 7,				
	eGlassType_UpperEq = 8,
	eReceiveRefuseCode_UpperEq = 9,
	eReserved_UpperEq = 10,			
	eCSIF_UpperEq = 12,					
	eAS_UpperEq = 14,				
	eAPS_UpperEq = 16					
};

enum EToLowerEqs
{
	eNothing_LowerEq = -1,					
	eHPanelID_LowerEq = 6,					
	eEPanelID_LowerEq = 12,
	eBatchID_LowerEq = 18,					
	eProdType_LowerEq = 19,										
	eProdKind_LowerEq = 20,				
	eDeviceID_LowerEq = 29,			
	eStepID_LowerEq = 34,					
	ePPID_LowerEq = 42,
	eThickness_LowerEq = 43,	
	eInsFlag_LowerEq = 44,				
	ePanelSize_LowerEq = 46,			
	ePanelPosition_LowerEq = 47,					
	eCount1_LowerEq = 48,
	eCount2_LowerEq = 49,		
	eGrade_LowerEq = 52,					
	eComment_LowerEq = 60,	
	eCompSize_LowerEq = 61,
	eReadingFlag_LowerEq = 62,
	ePanelState_LowerEq = 63,
	eJudgement_LowerEq = 65,	
	eCode_LowerEq = 67,							
	eRunLine_LowerEq = 77,
	eUniqueId_LowerEq = 79,
	ePairHPanelID_LowerEq = 85,				
	ePairEPanelID_LowerEq = 91,					
	ePairGrade_LowerEq = 94,				
	ePairUniqueId_LowerEq = 96,	
	eFlowRecipe_LowerEq = 97,		
	eReserved0_LowerEq = 103,	
	eBitsSignals_LowerEq = 104,
	eReferData_LowerEq = 106,
	eCSIF_LowerEq = 108,					
	eAS_LowerEq = 110,				
	eAPS_LowerEq = 112		
};

enum EEqStates
{
	eNothing_EqState = -1,					
	eEqState_EqState = 1,					
	eProcState_EqState = 2,				
	ePPID_EqState = 10,					
	eGlassSize_EqState = 12,				
	eGlassThickness_EqState = 13,					
	eTACTSet_EqState = 14,				
	eTACTCurrent_EqState = 15,					
	eRCDG_EqState = 16,			
	eEOMode_EqState = 17,					
	eERCMode_EqState = 18,				
	eGMCMode_EqState = 19,				
	eOperateMode_EqState = 20,				
	eReserved_EqState = 47
};

enum EGlassSummarys
{
	eNothing_GlassSum = -1,					
	eTGlassCount_GlassSum = 1,					
	eTFTCount_GlassSum = 2,				
	eCFCount_GlassSum = 3,					
	eDummyCount_GlassSum = 4,				
	eBPGCount_GlassSum = 5,					
	eProcessID1_GlassSum = 14,				
	eStepID1_GlassSum = 19,					
	eBatchID1_GlassSum = 25,				
	eGlassCount1_GlassSum = 26,					
	eProcessID2_GlassSum = 35,				
	eStepID2_GlassSum = 40,					
	eBatchID2_GlassSum = 46,				
	eGlassCount2_GlassSum = 47,					
	eProcessID3_GlassSum = 56,				
	eStepID3_GlassSum = 61,					
	eBatchID3_GlassSum = 67,				
	eGlassCount3_GlassSum = 68, 	
	eReserved_GlassSum = 80			
};

enum EBitsSignals
{
	eNothing_BitsSignal = -1,					
	eWorkSkip_BitsSignal = 0,					
	eJobStart_BitsSignal = 1,				
	eJobEnd_BitsSignal = 2,					
	eHotFlowFlag_BitsSignal = 3,				
	ePhsyicalFlag_BitsSignal = 4,					
	eMStartFlag_BitsSignal = 5,				
	eFileLoc_BitsSignal = 6,					
	eBatchStart_BitsSignal = 7,				
	eBatchEnd_BitsSignal = 8,					
	eAlignFlag_BitsSignal = 9,				
	eReserved0_BitsSignal = 10,
	eReserved1_BitsSignal = 11,
	eReserved2_BitsSignal = 12,
	eReserved3_BitsSignal = 13,
	eReserved4_BitsSignal = 14,
	eReserved5_BitsSignal = 15				
};

enum EReqGlassTypes
{
	eNothing_GlassType = -1,					
	eReadingGlassID_GlassType = 6,					
	eReadingGlassIDType_GlassType = 7,				
	eGlassType_GlassType = 8,	
	eReceiveRefuseCode_GlassType = 9,
	eReserved0_Glass = 10,			
	eCSIF_GlassType = 12,					
	eAS_GlassType = 14,				
	eAPS_GlassType = 16					
};

enum EGlassDatas
{
	eNothing_GlassData = -1,					
	eHPanelID_GlassData = 6,					
	eEPanelID_GlassData = 12,
	eBatchID_GlassData = 18,					
	eProdType_GlassData = 19,										
	eProdKind_GlassData = 20,				
	eDeviceID_GlassData = 29,			
	eStepID_GlassData = 34,					
	ePPID_GlassData = 42,
	eThickness_GlassData = 43,	
	eInsFlag_GlassData = 44,				
	ePanelSize_GlassData = 46,			
	ePanelPosition_GlassData = 47,					
	eCount1_GlassData = 48,
	eCount2_GlassData = 49,		
	eGrade_GlassData = 52,					
	eComment_GlassData = 60,	
	eCompSize_GlassData = 61,
	eReadingFlag_GlassData = 62,
	ePanelState_GlassData = 63,
	eJudgement_GlassData = 65,	
	eCode_GlassData = 66,							
	eRunLine_GlassData = 77,
	eUniqueId_GlassData = 79,
	ePairHPanelID_GlassData = 85,				
	ePairEPanelID_GlassData = 91,					
	ePairGrade_GlassData = 94,				
	ePairUniqueId_GlassData = 96,	
	eFlowRecipe_GlassData = 97,		
	eReserved0_GlassData = 103,	
	eBitsSignals_GlassData = 104,
	eReferData_GlassData = 106			
};

enum EPortInformations
{
	eCstID_PortInfo = 5,					
	erMSIF_PortInfo = 10,				
	eSSIF_PortInfo = 15,					
	eCSIF_PortInfo = 20,			
	ePortStatus_PortInfo = 21,					
	ePortMode_PortInfo = 22,				
	ePortType_PortInfo = 23,					
	eSortType_PortInfo = 24,				
	eProdType_PortInfo = 25					
};

//SJ_YYK 150821 Add...
enum EMaterialInfo
{
	e_sPORTID		= 4,		// Material Port ID					4
	e_sEQP_STATE	= 5,		// Eq State							1
	e_sPORT_STATE	= 6,		// Port State						1
	e_sPORT_TYPE	= 7,		// Port Type(Input/Output/Both)		1
	e_sCSTID		= 23,		// 자재 반송 매체 ID				16
	e_nMAP_STIF		= 103,		// Mapping 정보						80
	e_nCUR_STIF		= 183,		// UnLoading 시 Mapping 정보		80
	e_sM_BATCHID	= 203,		// Material Batchid					20
	e_sM_MAKER		= 221,		// Material Maker					18
	e_nM_KIND		= 222,		// Material Kind					1
	e_nM_TYPE		= 223,		// Material Type					1
	e_nM_STATE		= 224,		// Material State					1
	e_nT_QTY		= 226,		// 자재 전체 수량					2
	e_nU_QTY		= 228,		// 사용된 수량						2
	e_nR_QTY		= 230,		// 남은 수량						2
	e_nN_QTY		= 232,		// NG 누적 수량						2
	e_nA_QTY		= 234,		// Assemble  자재 수량				2
	e_nE_FLAG		= 235,		// 자재 완료되었을 때, 자투리 인지 유무	1
	e_sC_CODE		= 239,		// Cancel Code						4
	e_sM_STEP		= 249,		// 자재 종류 구분					10
	e_nS_QTY		= 251,		// 자재 요청 수량					2
	e_sM_CODE		= 267,		// Material Defect Code				16
};
//____________________________*/

enum eFDCinfo
{
	e_ToolTempM01 = 2,
	e_ToolTempM02 = 4,	
	e_ToolTempM03 = 6,
	e_ToolTempM04 = 8,
	e_IndexTempM01 = 10,
	e_IndexTempM02 = 12,
	e_IndexTempM03 = 14,
	e_IndexTempM04 = 16,
	e_IndexTempM05 = 18,
	e_IndexTempM06 = 20,
	e_IndexTempM07 = 22,
	e_IndexTempM08 = 24,
	e_IndexTempM09 = 26,
	e_IndexTempM10 = 28,
	e_IndexTempM11 = 30,
	e_IndexTempM12 = 32,
	e_IndexTempM13 = 34,
	e_IndexTempM14 = 36,
	e_IndexTempM15 = 38,
	e_IndexTempM16 = 40,

	e_ToolTempS01 = 42,
	e_ToolTempS02 = 44,	
	e_ToolTempS03 = 46,
	e_ToolTempS04 = 48,
	e_IndexTempS01 = 50,
	e_IndexTempS02 = 52,
	e_IndexTempS03 = 54,
	e_IndexTempS04 = 56,
	e_IndexTempS05 = 58,
	e_IndexTempS06 = 60,
	e_IndexTempS07 = 62,
	e_IndexTempS08 = 64,
	e_IndexTempS09 = 66,
	e_IndexTempS10 = 68,
	e_IndexTempS11 = 70,
	e_IndexTempS12 = 72,
	e_IndexTempS13 = 74,
	e_IndexTempS14 = 76,
	e_IndexTempS15 = 78,
	e_IndexTempS16 = 80
};

class MRobotArmStateBit 
{
public:
	MRobotArmStateBit();
	virtual ~MRobotArmStateBit();
public:
	int			m_nRobotAbnormal;
	int			m_nTypeOfArm;
	int			m_nTypeOfStageConveyor;
	int			m_nArm1Violate;
	int			m_nArm2Violate;
	int			m_nArm1FoldComplete;
	int			m_nArm2FoldComplete;
	int			m_nArm1GlassCheckSensor;
	int			m_nArm2GlassCheckSensor;
	int			m_nReserved0;
	int			m_nReserved1;
	int			m_nReserved2;
	int			m_nManualOperation;
	int			m_nRobotDirection;
	int			m_nReserved3;
	int			m_nReserved4;
	
public:
	void		SetData(MRobotArmStateBit *p);
};

class MStageStateBit 
{
public:
	MStageStateBit();
	virtual ~MStageStateBit();
public:
	int		 	m_nStageCVAbnormal;
	int		 	m_nTypeOfArm;
	int		 	m_nTypeOfStageConveyor;
	int		 	m_nEmpty;
	int		 	m_nIdle;
	int		 	m_nBusy;
	int		 	m_nPinUp;
	int		 	m_nPinDown;
	int		 	m_nStopperUp;
	int		 	m_nStopperDown;
	int		 	m_nDoorOpen;
	int		 	m_nDoorClose;
	int		 	m_nManualOperation;
	int		 	m_nBodyMoving;	
	int		 	m_nGlassCheckSensorOn;
	int		 	m_nReserved0;
public:
	void		SetData(MStageStateBit *p);
};

class MContactStateBit 
{
public:
	MContactStateBit();
	virtual ~MContactStateBit();
public:
	int		 	m_nContactState[W_U_SIZE];	
public:
	void		SetData(MContactStateBit *p);
};

class MToUpperEqs 
{
public:
	MToUpperEqs();
	virtual ~MToUpperEqs();
public:
	CString			m_strReadingGlassID;					
	CString			m_strReadingGlassIDType;				
	CString			m_strGlassType;	
	CString			m_strReceiveRefuseCode;
	CString			m_strReserved;			
	CString			m_strCSIF;					
	CString			m_strAS;				
	CString			m_strAPS;					
};

class MToLowerEqs 
{
public:
	MToLowerEqs();
	virtual ~MToLowerEqs();
public:
	CString			m_strHPanelID;		// Ascil  12byte					
	CString			m_strEPanelID;		// Ascil  12byte
	CString			m_strBatchID;		// Ascil  12byte				
	CString			m_strProdType;		// Ascil  2byte
	CString			m_strProdKind;		// Ascil  2byte
	CString			m_strDeviceID;		// Ascil  18byte
	CString			m_strStepID;		// Ascil  10byte
	CString			m_strPPID;		// Ascil  16byte
	CString			m_strThickness;		// Binary 2byte
	CString			m_strInsFlag;		// Ascil  2byte
	CString			m_strPanelSize;		// Binary 4byte
	CString			m_strPanelPosition;	// Ascil  2byte
	CString			m_strCount1;		// Ascil  2byte
	CString			m_strCount2;		// Ascil  2byte
	CString			m_strGrade;		// Bits	  6byte
	CString			m_strComment;		// Ascil  16byte
	CString			m_strCompSize;		// Binary 2byte
	CString			m_strReadingFlag;	// Ascil  2byte
	CString			m_strPanelState;	// Binary 2byte
	CString			m_strJudgement;		// Ascil  4byte
	CString			m_strCode;		// Ascil  4byte
	CString			m_strRunLine;		// Binary 20byte
	CString			m_strUniqueId;		// Binary 4byte
	CString			m_strPairHPanelID;	// Ascil  12byte
	CString			m_strPairEPanelID;	// Ascil  12byte
	CString			m_strPairGrade;		// Bits   6byte
	CString			m_strPairUniqueId;	// Binary 4byte
	CString			m_strFlowRecipe;	// Ascil  2byte
	CString			m_strReserved0;		// Any    12byte
	CString			m_strBitsSignals;	// Bits   2byte
	CString			m_strReferData;		// Any    4byte
	CString			m_strCSIF;
	CString			m_strAS;
	CString			m_strAPS;

public:
  void			CopyData(MToLowerEqs Data);
	void			ResetData(); 
	void	SetJobOrder();
	void	RstJobOrder();
};

class MStageState 
{
public:
	MStageState();
	virtual ~MStageState();
public:
	CString		m_strStageCVAbnormal;	
	CString		m_strTypeOfArm;
	CString		m_strTypeOfStageConveyor;	
	CString		m_strEmpty;
	CString		m_strIdle;
	CString		m_strBusy;
	CString		m_strComplete;
	CString		m_strLiftUp;
	CString		m_strLiftDown;
	CString		m_strStopperUp;
	CString		m_strStopperDown;
	CString		m_strGlassCheckSensorOn;
	CString		m_strManualOperation;
	CString		m_strEmergency;
	CString		m_strBodyMoving;
};

class MToLowerEqBit 
{
public:
	MToLowerEqBit();
	virtual ~MToLowerEqBit();
public:
	int		 	m_nReserved0;
	int		 	m_nMachinePause;
	int		 	m_nMachineDown;
	int		 	m_nMachineAlarm;	
	int		 	m_nSendAble;
	int		 	m_nSendStart;
	int		 	m_nSendComplete;
	int		 	m_nExchangeFlag;
	int		 	m_nReturnReceiveStart;
	int		 	m_nReturnReceiveComplete;
	int		 	m_nAction1Request;
	int		 	m_nAction2Request;	
	int		 	m_nReserved1;
	int		 	m_nReserved2;
	int		 	m_nWorkStart;
	int		 	m_nWorkCancel;
	int		 	m_nReserved3;
	int		 	m_nReserved4;
	int		 	m_nImmediatelyPauseRequest;
	int		 	m_nImmediatelyStopRequest;
	int		 	m_nSendAbleRemainedStep0;
	int		 	m_nSendAbleRemainedStep1;
	int		 	m_nSendAbleRemainedStep2;
	int		 	m_nSendAbleRemainedStep3;
	int		 	m_nHandShakeCancelRequest;
	int		 	m_nHandShakeAbortRequest;
	int		 	m_nHandShakeResumeRequest;
	int		 	m_nHandShakeRecoveryAckReply;
	int		 	m_nHandShakeRecoveryNckReply;	
	int		 	m_nReserved5;
	int		 	m_nReserved6;
	int		 	m_nReserved7;
};

class MEqStates 
{
public:
	MEqStates();
	virtual ~MEqStates();
public:
	CString			m_strEqState;		// Binary 2byte					
	CString			m_strProcState;		// Binary 2byte				
	CString			m_strPPID;		// Ascil  16byte					
	CString			m_strGlassSize;		// Binary 4byte			
	CString			m_strGlassThickness;	// Binary 2byte				
	CString			m_strTACTSet;		// Binary 2byte			
	CString			m_strTACTCurrent;	// Binary 2byte					
	CString			m_strRCDG;		// Binary 2byte				
	CString			m_strEOMode;		// Ascil  2byte					
	CString			m_strERCMode;		// Ascil  2byte				
	CString			m_strGMCMode;		// Ascil  2byte					
	CString			m_strOperateMode;	// Ascil  2byte				
	CString			m_strReserved;		// Ascil  2byte
};

class MToUpperEqBit 
{
public:
	MToUpperEqBit();
	virtual ~MToUpperEqBit();
public:
	int		 	m_nReserved0;
	int		 	m_nMachinePause;
	int		 	m_nMachineDown;
	int		 	m_nMachineAlarm;	
	int		 	m_nReceiveAble;
	int		 	m_nReceiveStart;
	int		 	m_nReceiveComplete;
	int		 	m_nExchangeFlag;
	int		 	m_nReturnSendStart;
	int		 	m_nReturnSendComplete;
	int		 	m_nAction1Complete;
	int		 	m_nAction2Complete;
	int		 	m_nReserved1;
	int		 	m_nReserved2;
	int		 	m_nReceiveRefuse;
	int		 	m_nGlassIdReadComplete;
	int		 	m_nLoadingStop;
	int		 	m_nTransferStop;
	int		 	m_nImmediatelyPauseRequest;
	int		 	m_nImmediatelyStopRequest;
	int		 	m_nReceiveAbleRemainedStep0;
	int		 	m_nReceiveAbleRemainedStep1;
	int		 	m_nReceiveAbleRemainedStep2;
	int		 	m_nReceiveAbleRemainedStep3;
	int		 	m_nHandShakeCancelRequest;
	int		 	m_nHandShakeAbortRequest;
	int		 	m_nHandShakeResumeRequest;
	int		 	m_nHandShakeRecoveryAckReply;
	int		 	m_nHandShakeRecoveryNckReply;
	int		 	m_nReserved3;	
	int		 	m_nReserved4;	
	int		 	m_nReserved5;
};

class MGlassSummarys 
{
public:
	MGlassSummarys();
	virtual ~MGlassSummarys();
public:
	//20111206 SJ_HJG
	/*
	CString			m_strTGlassCount;	// Binary 2byte					
	CString			m_strTFTCount;		// Binary 2byte				
	CString			m_strCFCount;		// Binary 2byte					
	CString			m_strDummyCount;	// Binary 2byte				
	CString			m_strBPGCount;		// Binary 2byte					
	CString			m_strProcessID1;	// Ascil  18byte				
	CString			m_strStepID1;		// Ascil  10byte					
	CString			m_strBatchID1;		// Ascil  12byte				
	CString			m_strGlassCount1;	// Ascil  2byte					
	CString			m_strProcessID2;	// Ascil  18byte				
	CString			m_strStepID2;		// Ascil  10byte					
	CString			m_strBatchID2;		// Ascil  12byte				
	CString			m_strGlassCount2;	// Ascil  2byte						
	CString			m_strProcessID3;	// Ascil  18byte				
	CString			m_strStepID3;		// Ascil  10byte					
	CString			m_strBatchID3;		// Ascil  12byte				
	CString			m_strGlassCount3;	// Ascil  2byte						
	*/
	CString			m_strReceiveTabOffsetDataA[DEF_MAX_TAB];
	CString			m_strReceiveTabOffsetDataB[DEF_MAX_TAB];
};

class MReqGlassTypes 
{
public:
	MReqGlassTypes();
	virtual ~MReqGlassTypes();
public:
	CString			m_strReadingGlassID;					
	CString			m_strReadingGlassIDType;				
	CString			m_strGlassType;					
	CString			m_strReceiveRefuseCode;
	CString			m_strCSIF;					
	CString			m_strAS;				
	CString			m_strAPS;					
};

class MGlassDatas
{
public:
	MGlassDatas();
	virtual ~MGlassDatas();
public:
	CString			m_strHPanelID;		// Ascil  12byte					
	CString			m_strEPanelID;		// Ascil  12byte
	CString			m_strBatchID;		// Ascil  12byte				
	CString			m_strProdType;		// Ascil  2byte
	CString			m_strProdKind;		// Ascil  2byte
	CString			m_strDeviceID;		// Ascil  18byte
	CString			m_strStepID;		// Ascil  10byte
	CString			m_strPPID;			// Ascil  16byte
	CString			m_strThickness;		// Binary 2byte
	CString			m_strInsFlag;		// Ascil  2byte
	CString			m_strPanelSize;		// Binary 4byte
	CString			m_strPanelPosition;	// Ascil  2byte
	CString			m_strCount1;		// Ascil  2byte
	CString			m_strCount2;		// Ascil  2byte
	CString			m_strGrade;			// Bits	  6byte
	CString			m_strComment;		// Ascil  16byte
	CString			m_strCompSize;		// Binary 2byte
	CString			m_strReadingFlag;	// Ascil  2byte
	CString			m_strPanelState;	// Binary 2byte
	CString			m_strJudgement;		// Ascil  4byte
	CString			m_strCode;			// Ascil  4byte
	CString			m_strRunLine;		// Binary 20byte
	CString			m_strUniqueId;		// Binary 4byte
	CString			m_strPairHPanelID;	// Ascil  12byte
	CString			m_strPairEPanelID;	// Ascil  12byte
	CString			m_strPairGrade;		// Bits   6byte
	CString			m_strPairUniqueId;	// Binary 4byte
	CString			m_strFlowRecipe;	// Ascil  2byte
	CString			m_strReserved0;		// Any    12byte
	CString			m_strBitsSignals;	// Bits   2byte
	CString			m_strReferData;		// Any    4byte
	
//	int         m_rgnBitsSignal[12];

public: 
	void			CopyData(MGlassDatas Data);
	void			ResetData();
	void			SetJobOrder();
	void			RstJobOrder();
};

class MPortInformations 
{
public:
	MPortInformations();
	virtual ~MPortInformations();
public:
	CString			m_strCstID;		// Ascil  12byte					
	CString			m_strMSIF;		// Bit 10byte
	CString			m_strSSIF;		// Bit 10byte	
	CString			m_strCSIF;		// Bit 10byte
	CString			m_strPortStatus;	// Ascil  2byte					
	CString			m_strPortMode;		// Ascil  2byte			
	CString			m_strPortType;		// Ascil  2byte				
	CString			m_strSortType;		// Ascil  2byte			
	CString			m_strProdType;		// Ascil  2byte			
};

class MEqpBitDatas 
{
public:
	MEqpBitDatas();
	virtual ~MEqpBitDatas();
public:
	bool				m_bRobot;
	MRobotArmStateBit	m_Robot;
	MStageStateBit		m_Stage;
	 
	bool				m_bUpper;
	MToUpperEqBit		m_Upper;
	MToLowerEqBit		m_Lower;
};

class MModelChangeBitDatas
{
public:
	int	m_iModelChangeReserve;
	int	m_iModelChangeStart;
	int	m_iModelChangeComplete;
	int	m_iModelChangeDisable;
	// Add. 110525 기종변경 예약 Bit 추가
	int	m_iModelReserveComplete;
	int	m_iModelReserveDisable;
};

class MModelChangeWordDatas
{
public:
	MModelChangeWordDatas();
	virtual ~MModelChangeWordDatas();
public:
	CString			m_strDeviceID;		// Ascil  10byte					
	CString			m_strEQState;		// Ascil  1byte
	CString			m_strReplyCode;		// Ascil  2byte				
	CString			m_strPanelWIP;		// Ascil  1byte
public:
	void			CopyData(MModelChangeWordDatas Data);
	void			ResetData(); 
};

#endif // !defined(AFX_MNETHDATA_H__BE15CBBD_BFA0_48BF_9074_C57D719A5C5B__INCLUDED_)
