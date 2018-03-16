// MLighting_ONI.cpp: implementation of the MLighting_ONI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MLighting_ONI.h"
//#include "MTickTimer.h"
#include "IComThread.h"
#include "ILighting.h"
#include "DefSerialValue.h"

#include <math.h>
//#include "MinUtilExt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define		DEF_ST			0xEF	//start of text
#define		DEF_ET			0xEE	//end of text
#define		DEF_CMD_WRITE	0x00

//MLighting_ONI::MLighting_ONI(SCommonAttribute commonData, IIO* pIIO, IComThread *m_plnkSerial, SLightingVal sLight)
//: ICommon(commonData), m_usValue(0)
MLighting_ONI::MLighting_ONI(IComThread *m_plnkSerial, SLightingVal sLight)
: m_usValue(0)
{	
	//m_plnkIO		= pIIO;
	m_pSerialPort	= m_plnkSerial;
	m_bLightOnOff	= FALSE;
	SetData(sLight);
//	Open();
//	LightOn(); //171004 JSH
}

MLighting_ONI::~MLighting_ONI()
{
//	Close();
//	LightOff();
//	delete m_pSerialPort;
}

int MLighting_ONI::Initialize()
{
	return ERR_LIGHTING_SUCCESS;
}

//int MLighting_ONI::GetValue(unsigned short usCh)
int MLighting_ONI::GetValue()
{
//	ASSERT(usCh >= DEF_LIGHTING_CH1 && usCh < DEF_LIGHTING_CHANEL);
//	return m_rgusValue[usCh];
	return m_usValue;
}

//int MLighting_ONI::SetValue(unsigned short val, unsigned short usCh)
int MLighting_ONI::SetValue(unsigned short val)
{
#ifdef SIMULATION
	return 0;
#endif
	if (val < 0 || val > pow(2.0, m_usUseBit))
	{
		return 61326;	//@IDS_ERR_LIGHT_VALUE_INVALID;
	}

	m_usValue = val;
	memset(m_cSendCommand, 0, sizeof(m_cSendCommand));	
	char strCh[2];
	sprintf(strCh, "%02d", m_usSelectChanel+1);
	char strLevel[3];
	sprintf(strLevel, "%03d", val);

	sprintf(m_cSendCommand, "%c%c%c%c%c%c%c%c%c", STX, strCh[0], strCh[1], 'W', 'R', strLevel[0], strLevel[1], strLevel[2], ETX);
	m_nPacketLen = 9+1;
	if (m_pSerialPort != NULL)
		m_pSerialPort->SendData(m_cSendCommand);

	if (val <= 0)	m_bLightOnOff = FALSE;
	else			m_bLightOnOff = TRUE;
	Sleep(20);
	return ERR_LIGHTING_SUCCESS;
}

int MLighting_ONI::LightOn()
{
#ifdef SIMULATION
	return 0;
#endif
	memset(m_cSendCommand, 0, sizeof(m_cSendCommand));	
	char strCh[2];
	sprintf(strCh, "%02d", m_usSelectChanel+1);

	sprintf(m_cSendCommand, "%c%c%c%c%c%c%c", STX, strCh[0], strCh[1], 'O', 'N', '1', ETX);
	m_nPacketLen = 7+1;
	if (m_pSerialPort != NULL)
		m_pSerialPort->SendData(m_cSendCommand);

	m_bLightOnOff = TRUE;
	Sleep(20);
	return ERR_LIGHTING_SUCCESS;
}

int MLighting_ONI::LightOff()
{
#ifdef SIMULATION
	return 0;
#endif
	memset(m_cSendCommand, 0, sizeof(m_cSendCommand));	
	char strCh[2];
	sprintf(strCh, "%02d", m_usSelectChanel+1);

	sprintf(m_cSendCommand, "%c%c%c%c%c%c%c", STX, strCh[0], strCh[1], 'O', 'N', '0', ETX);
	m_nPacketLen = 7+1;
	if (m_pSerialPort != NULL)
		m_pSerialPort->SendData(m_cSendCommand);

	m_bLightOnOff = TRUE;
	Sleep(20);
	return ERR_LIGHTING_SUCCESS;
}

BOOL MLighting_ONI::IsOnLighting()
{
	if (GetValue() == 0)
		return FALSE;
	else
		return TRUE;
}
/*
char MLighting_ONI::getCS()
{
	char cs = m_cSendCmd;
	for (int i=0; i<DEF_LIGHTING_CHANEL-1; i++)
		cs ^= m_rgusValue[i];
	cs ^= (m_rgusValue[DEF_LIGHTING_CHANEL-1] + 0x01);
	return cs;
}
*/
int MLighting_ONI::ClearQueue() 
{
	int ret = 0;
	if (m_pSerialPort != NULL)
		m_pSerialPort->ClearRecvQueue();
	return ret; 
}

int MLighting_ONI::AssignComponents(IIO *pIO)
{
	return ERR_LIGHTING_SUCCESS;
}

int MLighting_ONI::SetData(SLightingVal sLighting)
{
	m_usSelectChanel = sLighting.m_usSelectChanel;
//	m_usOAddrStartBit = sLighting.m_usOAddrStartBit;
	m_usUseBit = sLighting.m_usUseBit;
//	m_usOAddrOnOff = sLighting.m_usOAddrOnOff;
//	m_usTrigger = sLighting.m_usTrigger;
//	m_usOAddrChChangeStartBit = sLighting.m_usOAddrChChangeStartBit;
//	m_usInverse = sLighting.m_usInverse;	//KKY

	/** Chanel 설정 하기 */
	/*
	unsigned short usCh = sLighting.m_usSelectChanel;
	
	if (DEF_LIGHTING_NONE_CHANNEL != usCh)
	{
		switch (usCh)
		{
			case DEF_LIGHTING_CHANNEL1:	m_rgusChannel[0] = 1;	m_rgusChannel[1] = 1;	break;
			case DEF_LIGHTING_CHANNEL2:	m_rgusChannel[0] = 0;	m_rgusChannel[1] = 1;	break;
			case DEF_LIGHTING_CHANNEL3:	m_rgusChannel[0] = 1;	m_rgusChannel[1] = 0;	break;
			case DEF_LIGHTING_CHANNEL4:	m_rgusChannel[0] = 0;	m_rgusChannel[1] = 0;	break;
			default:
				break;
		}
	}
	else
	{
		m_rgusChannel[0] = DEF_LIGHTING_NONE_CHANNEL;
	}
	*/

	return ERR_LIGHTING_SUCCESS;
}

int MLighting_ONI::Open()
{
	BOOL bReturn = TRUE;
#ifndef SIMULATION
	/*
	bReturn = m_pSerialPort->OpenPort("COM4", CBR_9600, 3, BIT_8, NOPARITY, ONESTOPBIT);
	if (FALSE == bReturn)
		MyMessageBox("COM4 Serial Port(Light Control) open error!", "Error", M_ICONERROR);
	else
	{
		//프로그램 처음 시작시...필요한가??? 확인요_____
//			m_cSendCmd = DEF_CMD_WRITE;
//			makePacket(m_rgusValue);
//			m_pSerialPort->PutString(m_cSendCommand, m_nPacketLen);
		SetValue(m_rgusValue);
		//______________________________________________
	}
	*/
#endif

	return bReturn;
}

int	MLighting_ONI::Close()
{
	BOOL bReturn = TRUE;
#ifndef SIMULATION
	if (m_pSerialPort != NULL)
		bReturn = m_pSerialPort->ClosePort();
#endif

	return bReturn;
}

/*
void MLighting_ONI::makePacket(unsigned short Value[DEF_LIGHTING_CHANEL])
{
	memset(m_cSendCommand, 0, sizeof(m_cSendCommand));
	sprintf(m_cSendCommand, "%c%c%c"
							"%c%c%c%c"
							"%c%c%c",
							DEF_ST, DEF_ST, m_cSendCmd,
							m_rgusValue[0],m_rgusValue[1],m_rgusValue[2],m_rgusValue[3],
							getCS(), DEF_ET, DEF_ET);
	m_nPacketLen = 10+1;
}

int MLighting_ONI::SetValue(unsigned short val[DEF_LIGHTING_CHANEL])
{
	m_cSendCmd = DEF_CMD_WRITE;
	makePacket(m_rgusValue);
	m_pSerialPort->PutString(m_cSendCommand, m_nPacketLen);
	memset(m_cBuffers, 0, sizeof(m_cBuffers));
	DWORD stime = GetTickCount();
	while(!m_pSerialPort->GetString((char*)m_cBuffers, sizeof(m_cBuffers)))
	{
		Sleep(10);
		if(GetTickCount() - stime > 2000)
			break;
	}
	for (int i=0; i<DEF_LIGHTING_CHANEL; i++)
	{
		if (val[i] <= 0)	m_bLightOnOff[i] = FALSE;
		else				m_bLightOnOff[i] = TRUE;
	}
	Sleep(20);	
	return 0;
}
*/

/*
int MLighting_ONI::AllChannelOn()
{
	int ret = SetValue(m_rgusValue);
	return ret;
}

int MLighting_ONI::AllChannelOff()
{
	unsigned short usValue[DEF_LIGHTING_CHANEL];
	for (int i = 0; i < DEF_LIGHTING_CHANEL; i++)
		usValue[i] = 0;
	int ret = SetValue(usValue);
	return 0;
}

int MLighting_ONI::ChannelOn(int iChannel)
{
	ASSERT(iChannel >= DEF_LIGHTING_CH1 && iChannel < DEF_LIGHTING_CHANEL);
	SetValue(m_rgusValue[iChannel], iChannel);
	return 0;
}

int MLighting_ONI::ChannelOff(int iChannel)
{
	ASSERT(iChannel >= DEF_LIGHTING_CH1 && iChannel < DEF_LIGHTING_CHANEL);
	SetValue(0, iChannel);
	return 0;
}

BOOL MLighting_ONI::IsOnChannel(int iChannel)
{
	ASSERT(iChannel >= DEF_LIGHTING_CH1 && iChannel < DEF_LIGHTING_CHANEL);
	return m_bLightOnOff[iChannel];
}
*/
//___________