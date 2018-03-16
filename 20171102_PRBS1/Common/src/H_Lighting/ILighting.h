/* samsung */
#ifndef ILIGHTING_H
#define ILIGHTING_H

#include "ICommon.h"

class IIO;

const int ERR_LIGHTING_SUCCESS			= 0;		// Success

const int DEF_LIGHTING_CHANEL			= 8;

const int DEF_LIGHTING_NONE_CHANNEL		= 255;	//  unsigned interger 변수에 대입되어야 하므로, 음수를 사용할 수 없다.
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

const int DEF_LIGHTING_MAX_VALUE		= 1024;	// 확인 필요 : 10 비트 사용할 때와 8비트 사용할 때 구분 필요.

typedef struct
{
	unsigned short m_usSelectChanel;
	
	/** 생성된 Lighting의 시작 Bit 주소 */
	unsigned short m_usOAddrStartBit;

	/** 조명 값 조정을 위해 사용되는 Bit 수 */
	unsigned short m_usUseBit;

	/** 조명 Channel Change 시작 Bit 주소 */
	unsigned short m_usOAddrChChangeStartBit;

	/** Channel On/Off 를 위해 사용하는 IO 출력 주소 저장 */
	unsigned short m_usOAddrOnOff;

	unsigned short m_usTrigger;

	/**KKY. 조명 값 조절이 반대(Off/On 반대) */
	unsigned short m_usInverse;

} SLightingVal;


/** @interface */
class ILighting : public virtual ICommon 
{
public :
	/** 초기화 */
	virtual int Initialize() = 0;

	/** Lighting Value return */
	virtual int GetValue() = 0;
	
    /** Lighting Value 값으로 동작시킴  */
    virtual int SetValue(unsigned short val) = 0;
	
/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: 참조할 IO Component Pointer
	 */
	virtual int AssignComponents(IIO *pIO) = 0;

	virtual int SetData(SLightingVal sLighting) = 0;
	
	virtual int LightOn() = 0;

	virtual int LightOff() = 0;
	
	virtual BOOL IsOnLighting() = 0;
};
#endif //ILIGHTING_H