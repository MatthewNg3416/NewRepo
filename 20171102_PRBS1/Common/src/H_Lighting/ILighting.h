/* samsung */
#ifndef ILIGHTING_H
#define ILIGHTING_H

#include "ICommon.h"

class IIO;

const int ERR_LIGHTING_SUCCESS			= 0;		// Success

const int DEF_LIGHTING_CHANEL			= 8;

const int DEF_LIGHTING_NONE_CHANNEL		= 255;	//  unsigned interger ������ ���ԵǾ�� �ϹǷ�, ������ ����� �� ����.
const int DEF_LIGHTING_CHANNEL1			= 0;
const int DEF_LIGHTING_CHANNEL2			= 1;
const int DEF_LIGHTING_CHANNEL3			= 2;
const int DEF_LIGHTING_CHANNEL4			= 3;
const int DEF_LIGHTING_CHANNEL5			= 4;
const int DEF_LIGHTING_CHANNEL6			= 5;
const int DEF_LIGHTING_CHANNEL7			= 6;
const int DEF_LIGHTING_CHANNEL8			= 7;

const int DEF_LIGHTING_USE_8_BIT		= 8;
const int DEF_LIGHTING_USE_10_BIT		= 10;

const int DEF_LIGHTING_CHANNEL_BIT		= 2;

const int DEF_LIGHTING_MAX_VALUE		= 1024;	// Ȯ�� �ʿ� : 10 ��Ʈ ����� ���� 8��Ʈ ����� �� ���� �ʿ�.

typedef struct
{
	unsigned short m_usSelectChanel;
	
	/** ������ Lighting�� ���� Bit �ּ� */
	unsigned short m_usOAddrStartBit;

	/** ���� �� ������ ���� ���Ǵ� Bit �� */
	unsigned short m_usUseBit;

	/** ���� Channel Change ���� Bit �ּ� */
	unsigned short m_usOAddrChChangeStartBit;

	/** Channel On/Off �� ���� ����ϴ� IO ��� �ּ� ���� */
	unsigned short m_usOAddrOnOff;

	unsigned short m_usTrigger;

	/**KKY. ���� �� ������ �ݴ�(Off/On �ݴ�) */
	unsigned short m_usInverse;

} SLightingVal;


/** @interface */
class ILighting : public virtual ICommon 
{
public :
	/** �ʱ�ȭ */
	virtual int Initialize() = 0;

	/** Lighting Value return */
	virtual int GetValue() = 0;
	
    /** Lighting Value ������ ���۽�Ŵ  */
    virtual int SetValue(unsigned short val) = 0;
	
/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: ������ IO Component Pointer
	 */
	virtual int AssignComponents(IIO *pIO) = 0;

	virtual int SetData(SLightingVal sLighting) = 0;
	
	virtual int LightOn() = 0;

	virtual int LightOff() = 0;
	
	virtual BOOL IsOnLighting() = 0;
};
#endif //ILIGHTING_H