/**
 * Rockwell Automation Inverter - Power Flex 40
 * DeviceNet Communication B'd Option
 * -> Input 32 bit
 *   - Control Command 16 bit
 *   - Speed Reference 16 bit
 * -> Output 32 bit
 *   - Status Feedback 16 bit
 *   - Speed Feedback 16 bit
 */

#ifndef IINVERTER_H
#define IINVERTER_H

#include "ICommon.h"

class IIO;

const int ERR_INVERTER_SUCCESS					= 0;	// Success

const int DEF_INVERTER_CONTROL_BIT				= 16;
const int DEF_INVERTER_SPEED_BIT				= 16;

const int DEF_INVERTER_OUT_STOP_BIT				= 0;
const int DEF_INVERTER_OUT_START_BIT			= 1;
const int DEF_INVERTER_OUT_CLEAR_FAULTS_BIT		= 3;
const int DEF_INVERTER_OUT_FORWARD_BIT			= 4;
const int DEF_INVERTER_OUT_REVERSE_BIT			= 5;

const int DEF_INVERTER_IN_READY_BIT				= 0;
const int DEF_INVERTER_IN_ACTIVE_BIT			= 1;
const int DEF_INVERTER_IN_FORWARD_COMMAND_BIT	= 2;
const int DEF_INVERTER_IN_FORWARD_ACTUAL_BIT	= 3;
const int DEF_INVERTER_IN_ACCEL_BIT				= 4;
const int DEF_INVERTER_IN_DECEL_BIT				= 5;
const int DEF_INVERTER_IN_ALARM_BIT				= 6;
const int DEF_INVERTER_IN_FAULT_BIT				= 7;
const int DEF_INVERTER_IN_REFERENCE_SPEED_BIT	= 8;
const int DEF_INVERTER_IN_MAIN_FREQ_BIT			= 9;
const int DEF_INVERTER_IN_OPERATION_COMMAND_BIT	= 10;

const int DEF_INVERTER_NORMAL_SPEED				= 0;
const int DEF_INVERTER_REDUCED_SPEED			= 1;
const int DEF_INVERTER_TRANSFER_SPEED			= 2;

typedef struct
{
	/** 생성된 Inverter의 시작 Bit 주소 */
	unsigned short m_usIAddrStartBit;
	unsigned short m_usOAddrStartBit;

	/** 속도 설정 값 시작 Bit 주소 = 시작 BIT 주소 +16 */
	unsigned short m_usIAddrSpeedSetStartBit;
	unsigned short m_usOAddrSpeedSetStartBit;

} SInverterVal;


/** @interface */
class IInverter : public virtual ICommon 
{
public :  

	/** 속도 설정 값 return */
	virtual double GetSpeed(int iSpeedType) = 0;

    /** 속도 값 설정  */
    virtual int SetSpeed(unsigned short val, int iSpeedType) = 0;

    /** 현재 속도 종류 (Normal, Reduced, Transfer) 확인  */
	virtual BOOL IsSpeed(int iSpeedType) = 0;

/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: 참조할 IO Component Pointer
	 */
	virtual int AssignComponents(IIO *pIO) = 0;

	virtual int SetData(SInverterVal sInverter) = 0;

	/** 정지 */
	virtual int Stop() = 0;

	/** 동작 */
	virtual int Start() = 0;

	/** Fault Clear */
	virtual int ClearFaults() = 0;

	/** 정방향 설정*/
	virtual int ForwardDirection() = 0;

	/** 역방향 설정*/
	virtual int BackwardDirection() = 0;

	/** 대기 상태 Return */
	virtual BOOL IsReady() = 0;

	/** 동작 상태 Return */
	virtual BOOL IsActive() = 0;

	/** 정방향 Command */
	virtual BOOL IsForwardDirectionCommand() = 0;

	/** 역방향 Command */
	virtual BOOL IsBackwardDirectionCommand() = 0;

	/** 정방향 Actual */
	virtual BOOL IsForwardDirectionActual() = 0;

	/** 역방향 Actual */
	virtual BOOL IsBackwardDirectionActual() = 0;

	/** 가속중 */
	virtual BOOL IsAccel() = 0;

	/** 감속중 */
	virtual BOOL IsDecel() = 0;

	/** Alarm 상태인지 Return */
	virtual BOOL IsAlarm() = 0;

	/** Fault 상태인지 Return */
	virtual BOOL IsFault() = 0;

	/** Reference 속도로 동작 중인지 Return */
	virtual BOOL IsReferenceSpeed() = 0;

	/** 통신으로 제어중인지 Return */
	virtual BOOL IsControlledbyComm() = 0;

	/** Speed(mm/s) -> Freq.(Hz) */
	virtual double	CalculateSpeed2Freq(double dSpeed) = 0;

	/** Freq.(Hz) -> Speed(mm/s) */
	virtual double	CalculateFreq2Speed(double dFreq) = 0;

};
#endif //IINVERTER_H