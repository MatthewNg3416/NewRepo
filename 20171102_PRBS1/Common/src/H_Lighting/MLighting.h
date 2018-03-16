/* Generated by Together */

#ifndef MLIGHTING_H
#define MLIGHTING_H

#include "ILighting.h"

const int DEF_MAX_LIGHTING_VALUE_BIT = 10;

class MLighting : public ILighting
{
private:
	IIO* m_plnkIO;

	/** 조명 On/Off 를 위해 사용하는 IO 출력 주소 저장 */
	unsigned short m_usOAddrOnOff;

	/** 조명 Channel Change 시작 Bit 주소 */
	unsigned short m_usOAddrChChangeStartBit;

	unsigned short m_usTrigger;	

	/** 조명 값 저장 */
	unsigned short m_usValue;

	/** Channel 조정 Bit 에 쓸 값을 저장할 배열 */
	unsigned short m_rgusChannel[DEF_LIGHTING_CHANNEL_BIT];

	/** 생성된 Lighting의 시작 Bit 주소 */
	unsigned short m_usOAddrStartBit;

	/** 조명 값 조정을 위해 사용되는 Bit 수 */
	unsigned short m_usUseBit;

	/**KKY. 조명 값 조절이 반대(Off/On 반대) */
	unsigned short m_usInverse;

public:
	/** Default 생성자 */
	MLighting();

	/** 
	 * Constructor
	 * 
	 * @param	commonData : 공통적으로 사용할 Data
	 * @param	pIIO : 참조할 I/O Component 포인터
	 * @param	datComponent : 자체적으로 사용할 Data
	 */
	MLighting( SCommonAttribute commonData, IIO* pIIO, SLightingVal sLight);

	/** 소멸자 */
	virtual ~MLighting();

	/** 초기화 */
	int Initialize();

	/** Lighting Value return */
	int GetValue();

	/** Lighting Value 값으로 동작시킴  */
    int SetValue(unsigned short val);

	/** 조명 켬 */
	int LightOn();

	/** 조명 끔 */
	int LightOff();
	
	BOOL IsOnLighting();

private:
	
/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: 참조할 IO Component Pointer
	 */
	int AssignComponents(IIO* pIO);  	

	int SetData(SLightingVal sLighting);

	/** IO 출력 처리 함수 
	 *	- IO 신호가 뒤바뀌는 경우에 대해 유지보수를 쉽게 하기 위해 사용.
	 */
	int outputOn(unsigned short usIOAddr);
	int outputOff(unsigned short usIOAddr);
	BOOL isOn(unsigned short usIOAddr);
};
#endif //MLIGHTING_H
