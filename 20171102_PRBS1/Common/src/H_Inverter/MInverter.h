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

#ifndef MINVERTER_H
#define MINVERTER_H

#include "IInverter.h"

class MInverter : public IInverter
{
private:
	IIO* m_plnkIO;

	/** 생성된 Inverter의 시작 Bit 주소 */
	unsigned short m_usIAddrStartBit;
	unsigned short m_usOAddrStartBit;

	/** 속도 설정 값 시작 Bit 주소 = 시작 BIT 주소 +16 */
	unsigned short m_usIAddrSpeedSetStartBit;
	unsigned short m_usOAddrSpeedSetStartBit;

	/** Speed 값 저장 */
	double m_dNormalSpeed;
	double m_dReducedSpeed;
	double m_dTransferSpeed;

	/** 동작 Speed 값 저장 */
	double m_dRunningSpeed;

public:
	/** Default 생성자 */
	MInverter();

	/** 
	 * Constructor
	 * 
	 * @param	commonData : 공통적으로 사용할 Data
	 * @param	pIIO : 참조할 I/O Component 포인터
	 * @param	datComponent : 자체적으로 사용할 Data
	 */
	MInverter( SCommonAttribute commonData, IIO* pIIO, SInverterVal sInverter);

	/** 소멸자 */
	virtual ~MInverter();

    /** 속도 값 설정  */
    virtual int SetSpeed(unsigned short val, int iSpeedType);

	/** 속도 설정 값 return */
	virtual double GetSpeed(int iSpeedType);

    /** 현재 속도 종류 (Normal, Reduced, Transfer) 확인  */
	virtual BOOL IsSpeed(int iSpeedType);

	/** 정지 */
	virtual int Stop();

	/** 동작 */
	virtual int Start();

	/** Fault Clear */
	virtual int ClearFaults();

	/** 정방향 설정*/
	virtual int ForwardDirection();

	/** 역방향 설정*/
	virtual int BackwardDirection();

	/** 대기 상태 Return */
	virtual BOOL IsReady();

	/** 동작 상태 Return */
	virtual BOOL IsActive();

	/** 정방향 Command */
	virtual BOOL IsForwardDirectionCommand();

	/** 역방향 Command */
	virtual BOOL IsBackwardDirectionCommand();

	/** 정방향 Actual */
	virtual BOOL IsForwardDirectionActual();

	/** 역방향 Actual */
	virtual BOOL IsBackwardDirectionActual();

	/** 가속중 */
	virtual BOOL IsAccel();

	/** 감속중 */
	virtual BOOL IsDecel();

	/** Alarm 상태인지 Return */
	virtual BOOL IsAlarm();

	/** Fault 상태인지 Return */
	virtual BOOL IsFault();

	/** Reference 속도로 동작 중인지 Return */
	virtual BOOL IsReferenceSpeed();

	/** 통신으로 제어중인지 Return */
	virtual BOOL IsControlledbyComm();

	/** Speed(mm/s) -> Freq.(Hz) */
	virtual double	CalculateSpeed2Freq(double dSpeed);

	/** Freq.(Hz) -> Speed(mm/s) */
	virtual double	CalculateFreq2Speed(double dFreq);

private:
	
/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: 참조할 IO Component Pointer
	 */
	int AssignComponents(IIO* pIO);  	

	int SetData(SInverterVal sInverter);
};
#endif //MINVERTER_H
