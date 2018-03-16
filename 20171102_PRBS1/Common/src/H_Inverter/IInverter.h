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
	/** ������ Inverter�� ���� Bit �ּ� */
	unsigned short m_usIAddrStartBit;
	unsigned short m_usOAddrStartBit;

	/** �ӵ� ���� �� ���� Bit �ּ� = ���� BIT �ּ� +16 */
	unsigned short m_usIAddrSpeedSetStartBit;
	unsigned short m_usOAddrSpeedSetStartBit;

} SInverterVal;


/** @interface */
class IInverter : public virtual ICommon 
{
public :  

	/** �ӵ� ���� �� return */
	virtual double GetSpeed(int iSpeedType) = 0;

    /** �ӵ� �� ����  */
    virtual int SetSpeed(unsigned short val, int iSpeedType) = 0;

    /** ���� �ӵ� ���� (Normal, Reduced, Transfer) Ȯ��  */
	virtual BOOL IsSpeed(int iSpeedType) = 0;

/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: ������ IO Component Pointer
	 */
	virtual int AssignComponents(IIO *pIO) = 0;

	virtual int SetData(SInverterVal sInverter) = 0;

	/** ���� */
	virtual int Stop() = 0;

	/** ���� */
	virtual int Start() = 0;

	/** Fault Clear */
	virtual int ClearFaults() = 0;

	/** ������ ����*/
	virtual int ForwardDirection() = 0;

	/** ������ ����*/
	virtual int BackwardDirection() = 0;

	/** ��� ���� Return */
	virtual BOOL IsReady() = 0;

	/** ���� ���� Return */
	virtual BOOL IsActive() = 0;

	/** ������ Command */
	virtual BOOL IsForwardDirectionCommand() = 0;

	/** ������ Command */
	virtual BOOL IsBackwardDirectionCommand() = 0;

	/** ������ Actual */
	virtual BOOL IsForwardDirectionActual() = 0;

	/** ������ Actual */
	virtual BOOL IsBackwardDirectionActual() = 0;

	/** ������ */
	virtual BOOL IsAccel() = 0;

	/** ������ */
	virtual BOOL IsDecel() = 0;

	/** Alarm �������� Return */
	virtual BOOL IsAlarm() = 0;

	/** Fault �������� Return */
	virtual BOOL IsFault() = 0;

	/** Reference �ӵ��� ���� ������ Return */
	virtual BOOL IsReferenceSpeed() = 0;

	/** ������� ���������� Return */
	virtual BOOL IsControlledbyComm() = 0;

	/** Speed(mm/s) -> Freq.(Hz) */
	virtual double	CalculateSpeed2Freq(double dSpeed) = 0;

	/** Freq.(Hz) -> Speed(mm/s) */
	virtual double	CalculateFreq2Speed(double dFreq) = 0;

};
#endif //IINVERTER_H