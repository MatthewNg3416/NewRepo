// MLighting_ONI.h: interface for the MLighting_ONI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MLighting_ONI_H__79E90D52_2B30_43EA_80AB_50B5B711981F__INCLUDED_)
#define AFX_MLighting_ONI_H__79E90D52_2B30_43EA_80AB_50B5B711981F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ILighting.h"

class IComThread;

class MLighting_ONI : public ILighting  
{
private:
	IIO*			m_plnkIO;
	IComThread*		m_pSerialPort;

	char			m_cSendCommand[100];
	char			m_cBuffers[100];
	char			m_cSendCmd;

//	unsigned short	m_rgusValue[DEF_LIGHTING_CHANEL];
	int				m_nPacketLen;
//	BOOL			m_bLightOnOff[DEF_LIGHTING_CHANEL];
	unsigned short	m_usValue;
	BOOL			m_bLightOnOff;

	unsigned short	m_usSelectChanel;
	/** 조명 값 조정을 위해 사용되는 Bit 수 */
	unsigned short m_usUseBit;				//ATL Light 사용

public:
	MLighting_ONI( IComThread *m_plnkSerial, SLightingVal sLight);
	virtual ~MLighting_ONI();
	
	int				Open();
	int				Close();
public:
	int	Initialize();

	/** Lighting Value return */
//	int GetValue(unsigned short usCh);
	int	GetValue();
	
    /** Lighting Value 값으로 동작시킴  */
//    int SetValue(unsigned short val, unsigned short usCh);
	int SetValue(unsigned short val);
	
/*------------------------ Common Interface -----------------------*/
    /**
	 * Assign component linkage
	 * 
	 * @param	pIO	: 참조할 IO Component Pointer
	 */
	int	LightOn();

	int	LightOff();

	BOOL IsOnLighting();

	int ClearQueue();

//	int SetValue(unsigned short val[DEF_LIGHTING_CHANEL]);
//	int AllChannelOn();
//	int AllChannelOff();
//	int ChannelOn(int iChannel);
//	int ChannelOff(int iChannel);
//	BOOL IsOnChannel(int iChannel);


private:

	int AssignComponents(IIO *pIO);

	int SetData(SLightingVal sLighting);

	char getCS();
	void makePacket(unsigned short Value[DEF_LIGHTING_CHANEL]);
};

#endif // !defined(AFX_MLighting_ONI_H__79E90D52_2B30_43EA_80AB_50B5B711981F__INCLUDED_)
