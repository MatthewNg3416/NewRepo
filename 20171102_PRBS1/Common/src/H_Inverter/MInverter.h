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

	/** ������ Inverter�� ���� Bit �ּ� */
	unsigned short m_usIAddrStartBit;
	unsigned short m_usOAddrStartBit;

	/** �ӵ� ���� �� ���� Bit �ּ� = ���� BIT �ּ� +16 */
	unsigned short m_usIAddrSpeedSetStartBit;
	unsigned short m_usOAddrSpeedSetStartBit;

	/** Speed �� ���� */
	double m_dNormalSpeed;
	double m_dReducedSpeed;
	double m_dTransferSpeed;

	/** ���� Speed �� ���� */
	double m_dRunningSpeed;

public:
	/** Default ������ */
	MInverter();

	/** 
	 * Constructor
	 * 
	 * @param	commonData : ���������� ����� Data
	 * @param	pIIO : ������ I/O Component ������
	 * @param	datComponent : ��ü������ ����� Data
	 */
	MInverter( SCommonAttribute commonData, IIO* pIIO, SInverterVal sInverter);

	/** �Ҹ��� */
	virtual ~MInverter();

    /** �ӵ� �� ����  */
    virtual int SetSpeed(unsigned short val, int iSpeedType);

	/** �ӵ� ���� �� return */
	virtual double GetSpeed(int iSpeedType);

    /** ���� �ӵ� ���� (Normal, Reduced, Transfer) Ȯ��  */
	virtual BOOL IsSpeed(int iSpeedType);

	/** ���� */
	virtual int Stop();

	/** ���� */
	virtual int Start();

	/** Fault Clear */
	virtual int ClearFaults();

	/** ������ ����*/
	virtual int ForwardDirection();

	/** ������ ����*/
	virtual int BackwardDirection();

	/** ��� ���� Return */
	virtual BOOL IsReady();

	/** ���� ���� Return */
	virtual BOOL IsActive();

	/** ������ Command */
	virtual BOOL IsForwardDirectionCommand();

	/** ������ Command */
	virtual BOOL IsBackwardDirectionCommand();

	/** ������ Actual */
	virtual BOOL IsForwardDirectionActual();

	/** ������ Actual */
	virtual BOOL IsBackwardDirectionActual();

	/** ������ */
	virtual BOOL IsAccel();

	/** ������ */
	virtual BOOL IsDecel();

	/** Alarm �������� Return */
	virtual BOOL IsAlarm();

	/** Fault �������� Return */
	virtual BOOL IsFault();

	/** Reference �ӵ��� ���� ������ Return */
	virtual BOOL IsReferenceSpeed();

	/** ������� ���������� Return */
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
	 * @param	pIO	: ������ IO Component Pointer
	 */
	int AssignComponents(IIO* pIO);  	

	int SetData(SInverterVal sInverter);
};
#endif //MINVERTER_H
