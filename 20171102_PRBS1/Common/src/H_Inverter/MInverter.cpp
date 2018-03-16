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

#include "stdafx.h"
#include "IIO.h"
#include "MInverter.h"
#include "common.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
 * Assign component linkage
 * 
 * @param	pIO	: 참조할 IO Component Pointer
 */
int MInverter::AssignComponents(IIO *pIO)
{
	ASSERT(pIO != NULL);
	m_plnkIO = pIO;
	return ERR_INVERTER_SUCCESS;
}

int MInverter::SetData(SInverterVal sInverter)
{
	m_usIAddrStartBit = sInverter.m_usIAddrStartBit;
	m_usOAddrStartBit = sInverter.m_usOAddrStartBit;
	m_usIAddrSpeedSetStartBit = sInverter.m_usIAddrSpeedSetStartBit;
	m_usOAddrSpeedSetStartBit = sInverter.m_usOAddrSpeedSetStartBit;

	return ERR_INVERTER_SUCCESS;
}

/** @stereotype constructor */
MInverter::MInverter()
{
	m_plnkIO = NULL;
	m_usIAddrStartBit = 0;
	m_usOAddrStartBit = 0;
	m_usIAddrSpeedSetStartBit = 0;
	m_usOAddrSpeedSetStartBit = 0;
	m_dNormalSpeed = 0.0;
	m_dReducedSpeed = 0.0;
	m_dTransferSpeed = 0.0;
	m_dRunningSpeed = 0.0;
}

/** @stereotype constructor
*/
MInverter::MInverter(SCommonAttribute commonData, IIO *pIIO, SInverterVal sInverter)
: ICommon(commonData), m_dNormalSpeed(0.0), m_dReducedSpeed(0.0), m_dTransferSpeed(0.0)
{
	AssignComponents(pIIO);	
	SetData(sInverter);

	m_dNormalSpeed = 0.0;
	m_dReducedSpeed = 0.0;
	m_dTransferSpeed = 0.0;
	m_dRunningSpeed = 0.0;

	int iError = Stop();
	if (iError)
	{
		//WriteErrorLog("MInverter : Stop Error!");
		//SetErrorLevel(_T("Inverter creation"),3, __FILE__, __LINE__);
	}

	if (IsFault())
	{
		iError = ClearFaults();
		if (iError)
		{
			//WriteErrorLog("MInverter : Clear Faults Error!");
			//SetErrorLevel(_T("Inverter creation"),3, __FILE__, __LINE__);
		}
	}

	if (FALSE == IsReady())
	{
		iError = ClearFaults();
		if (iError)
		{
			//WriteErrorLog("MInverter : Clear Faults Error!");
			//SetErrorLevel(_T("Inverter creation"),3, __FILE__, __LINE__);
		}
	}
}

MInverter::~MInverter()
{
	TRACE(_T("MInverter 소멸\n"));
}

int MInverter::SetSpeed(unsigned short val, int iSpeedType)
{
	unsigned short rgusRemainder[DEF_INVERTER_SPEED_BIT];
	unsigned short usDivideValue = val;

	memset(rgusRemainder, 0, sizeof(rgusRemainder));

	if (usDivideValue < 0 || usDivideValue > pow(2.0, DEF_INVERTER_SPEED_BIT))
		return generateErrorCode(116006);	

	/** 속도값 설정 하기 */
	for (int i = 0; i < DEF_INVERTER_SPEED_BIT && usDivideValue > 0; i++)
	{
		rgusRemainder[i] = usDivideValue % 2;
		usDivideValue = unsigned short(usDivideValue / 2);
	}

	for (int i = 0; i < DEF_INVERTER_SPEED_BIT; i++)
	{
		rgusRemainder[i] ? m_plnkIO->OutputOn(m_usOAddrSpeedSetStartBit + i) : m_plnkIO->OutputOff(m_usOAddrSpeedSetStartBit + i);
	}

	if (iSpeedType == DEF_INVERTER_NORMAL_SPEED)
	{
		m_dNormalSpeed = CalculateFreq2Speed(((double)val)/10.0);
		m_dRunningSpeed = m_dNormalSpeed;
	}
	else if (iSpeedType == DEF_INVERTER_REDUCED_SPEED)
	{
		m_dReducedSpeed = CalculateFreq2Speed(((double)val)/10.0);
		m_dRunningSpeed = m_dReducedSpeed;
	}
	else // DEF_INVERTER_TRANSFER_SPEED
	{
		m_dTransferSpeed = CalculateFreq2Speed(((double)val)/10.0);
		m_dRunningSpeed = m_dTransferSpeed;
	}

	return ERR_INVERTER_SUCCESS;
}

double MInverter::GetSpeed(int iSpeedType)
{
	if (iSpeedType == DEF_INVERTER_NORMAL_SPEED)
		return m_dNormalSpeed;
	else if (iSpeedType == DEF_INVERTER_REDUCED_SPEED)
		return m_dReducedSpeed;
	else // DEF_INVERTER_TRANSFER_SPEED
		return m_dTransferSpeed;
}

BOOL MInverter::IsSpeed(int iSpeedType)
{
	if (iSpeedType == DEF_INVERTER_NORMAL_SPEED)
	{
		if (m_dRunningSpeed == m_dNormalSpeed)
			return TRUE;
		else
			return FALSE;
	}
	else if (iSpeedType == DEF_INVERTER_REDUCED_SPEED)
	{
		if (m_dRunningSpeed == m_dReducedSpeed)
			return TRUE;
		else
			return FALSE;
	}
	else // DEF_INVERTER_TRANSFER_SPEED
	{
		if (m_dRunningSpeed == m_dTransferSpeed)
			return TRUE;
		else
			return FALSE;
	}
}

int MInverter::Stop()
{
	int iError = m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_START_BIT);
	if (iError)
	{
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_START_BIT);
		Sleep(100);
		m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_STOP_BIT);
		return generateErrorCode(116003);
	}

	Sleep(100);

	iError = m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_STOP_BIT);
	if (iError)
	{
		m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_STOP_BIT);
		return generateErrorCode(116000);
	}

	return ERR_INVERTER_SUCCESS;
}

int MInverter::Start()
{
	if (IsFault() || FALSE == IsReady())
	{
		int iResult = ClearFaults();
		if (iResult)
			return iResult;

		Sleep(100);

		if (IsFault())
			return generateErrorCode(116011);

		if (FALSE == IsReady())
			return generateErrorCode(116012);
	}

	int iError = m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_STOP_BIT);
	if (iError)
	{
		m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_STOP_BIT);
		Sleep(100);
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_START_BIT);
		return generateErrorCode(116001);
	}

	Sleep(100);

	iError = m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_START_BIT);
	if (iError)
	{
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_START_BIT);
		Sleep(100);
		m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_STOP_BIT);
		return generateErrorCode(116002);
	}

	return ERR_INVERTER_SUCCESS;
}

int MInverter::ClearFaults()
{
	int iError = m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_CLEAR_FAULTS_BIT);
	if (iError)
	{
		m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_CLEAR_FAULTS_BIT);
		Sleep(100);
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_CLEAR_FAULTS_BIT);
		return generateErrorCode(116004);
	}

	Sleep(100);

	iError = m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_CLEAR_FAULTS_BIT);
	if (iError)
	{
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_CLEAR_FAULTS_BIT);
		return generateErrorCode(116005);
	}

	return ERR_INVERTER_SUCCESS;
}

int MInverter::ForwardDirection()
{
	int iError = m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_REVERSE_BIT);
	if (iError)
	{
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_REVERSE_BIT);
		return generateErrorCode(116010);
	}

	Sleep(100);

	iError = m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_FORWARD_BIT);
	if (iError)
	{
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_FORWARD_BIT);
		return generateErrorCode(116007);
	}

	return ERR_INVERTER_SUCCESS;
}

int MInverter::BackwardDirection()
{
	int iError = m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_FORWARD_BIT);
	if (iError)
	{
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_FORWARD_BIT);
		return generateErrorCode(116008);
	}

	Sleep(100);

	iError = m_plnkIO->OutputOn(m_usOAddrStartBit + DEF_INVERTER_OUT_REVERSE_BIT);
	if (iError)
	{
		m_plnkIO->OutputOff(m_usOAddrStartBit + DEF_INVERTER_OUT_REVERSE_BIT);
		return generateErrorCode(116009);
	}

	return ERR_INVERTER_SUCCESS;
}

BOOL MInverter::IsReady()
{
#ifdef SIMULATION
	return TRUE;
#endif
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_READY_BIT);
	return bStatus;
}

BOOL MInverter::IsActive()
{
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_ACTIVE_BIT);
	return bStatus;
}

BOOL MInverter::IsForwardDirectionCommand()
{
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_FORWARD_COMMAND_BIT);
	return bStatus;
}

BOOL MInverter::IsBackwardDirectionCommand()
{
	BOOL bStatus = m_plnkIO->IsOff(m_usIAddrStartBit + DEF_INVERTER_IN_FORWARD_COMMAND_BIT);
	return bStatus;
}

BOOL MInverter::IsForwardDirectionActual()
{
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_FORWARD_ACTUAL_BIT);
	return bStatus;
}

BOOL MInverter::IsBackwardDirectionActual()
{
	BOOL bStatus = m_plnkIO->IsOff(m_usIAddrStartBit + DEF_INVERTER_IN_FORWARD_ACTUAL_BIT);
	return bStatus;
}

BOOL MInverter::IsAccel()
{
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_ACCEL_BIT);
	return bStatus;
}

BOOL MInverter::IsDecel()
{
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_DECEL_BIT);
	return bStatus;
}

BOOL MInverter::IsAlarm()
{
#ifdef SIMULATION
	return FALSE;
#endif
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_ALARM_BIT);
	return bStatus;
}

BOOL MInverter::IsFault()
{
#ifdef SIMULATION
	return FALSE;
#endif
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_FAULT_BIT);
	return bStatus;
}

BOOL MInverter::IsReferenceSpeed()
{
	BOOL bStatus = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_REFERENCE_SPEED_BIT);
	return bStatus;
}

BOOL MInverter::IsControlledbyComm()
{
	BOOL bStatus1 = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_MAIN_FREQ_BIT);
	BOOL bStatus2 = m_plnkIO->IsOn(m_usIAddrStartBit + DEF_INVERTER_IN_OPERATION_COMMAND_BIT);
	return bStatus1 && bStatus2;
}

double MInverter::CalculateSpeed2Freq(double dSpeed)
{
	// Freq. -> RPM : N = 120f/P
	// - N : 회전수(RPM), f : 주파수(Hz), P : 극수 4극
	// RPM -> Speed : V = (N/60)/Gear * (2*Pi*R) => V = 2f/P/Gear*2R*Pi
	// - V : Roller 선속도(mm/s), Gear : 감속비 7.5:1, Pi=3.14, 2R : 지름 60mm
	// Speed -> Freq. : f = V*P*Gear/2R/Pi/2 => f = V*4*7.5/60/3.14/2 => f = V*0.25/3.14
	double dFreq = 0.0;
	int iPole = 4;
	double dGearRatio = 7.5, dDiameter = 60.0, dPi = 3.1415926535;

	dFreq = dSpeed * iPole * dGearRatio / (dDiameter * dPi * 2);

	if (dFreq > 60.0)
		dFreq = 60.0;

	return dFreq;
}

double MInverter::CalculateFreq2Speed(double dFreq)
{
	// Freq. -> RPM : N = 120f/P
	// - N : 회전수(RPM), f : 주파수(Hz), P : 극수 4극
	// RPM -> Speed : V = (N/60)/Gear * (2*Pi*R) => V = 2f/P/Gear*2R*Pi
	// - V : Roller 선속도(mm/s), Gear : 감속비 7.5:1, Pi=3.14, 2R : 지름 60mm
	//					=> V = 2*f/4/7.5*60*3.14 => V = f*4*3.14
	double dSpeed = 0.0;
	int iPole = 4;
	double dGearRatio = 7.5, dDiameter = 60.0, dPi = 3.1415926535;

	dSpeed = 2 * dFreq * dDiameter * dPi / (iPole * dGearRatio);

	return dSpeed;
}

