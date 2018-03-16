/* Generated by Together */
/* samsung */
/* 
 * Heater Controller Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MHeaterController.h : This file implements the interface class of IHeaterController.
 *
 * @author  Arena99 
 * @version $Revision$
 */

#ifndef MHEATER_CONTROLLER_H
#define MHEATER_CONTROLLER_H

#include "IHeaterController.h"

const	int	DEF_MAX_COMMAND_CHAR_LENGTH = 256;



class MHeaterController : public IHeaterController 
{
/** Attribute */
private:
	/** Number of Heaters */
	int				m_iNumHeaters;

	/** Number Of OverHeat Alarms */
	int				m_iNumOfOverHeaterAlarm;

	/** Number if Heater DisconnectAlarms*/
	int				m_iNumOfDisconnectHeaterAlarm;

	/** Max Temperature */
	double			m_dMaxTemperature;

	/** Min Temperature */
	double			m_dMinTemperature;

	/** IO Address Heater Alarm*/
	unsigned short	m_usAddrHeaterAlarm[DEF_MAX_NUM_HEATER];

	/** Over Heater Alarm */
	// Added on 2004.11.29
	unsigned short	m_usAddrOverHeatAlarm[DEF_MAX_NUM_OVERHEAT_ALARM];
	
	/** Heater Disconnect Check*/
	unsigned short	m_usAddrDisconnectHeatAlarm[DEF_MAX_NUM_CONNECT_ALARM];

	/** Serial communication timeout Setting */
	double			m_dTimeoutSerialComm;	// unit - sec

	/** command buffer for setting temperature */
	char			m_strSendCommand[DEF_MAX_COMMAND_CHAR_LENGTH];	
	
	/** Critical Section */
	CRITICAL_SECTION	m_csHeaterControl;

	/** IO component */
	IIO					*m_plnkIO;

	/** Serial Component communicating with Temperature/Pressure controller */
//    ISerialPort			*m_plnkSerial;
	IComThread			*m_plnkSerial;


public:
	/** @stereotype constructor */
	MHeaterController();

	/** Constructor, Destructor, 초기화 관련 */
    /** @stereotype constructor
     * @param	iObjectID : int RegisterObject()에서 반환한 ObjectID
     * @param   listRefComponent :  SAcfToolRefComList, 참조할 Component List 구조체
     * @param   iErrorBase : int, Component의 에러 시작 
     * @param   strFullFileName : CString, 로그 파일 Path와 이름 
     * @param   ucLevel : BYTE, 로그 수준(MLog Class 참조)
     * @param   iDalys : int, 로깅보관 기간, Default 30
     * @param   datComponent; SAcfToolData, 콤포넌트의 데이타 구조
     */
	MHeaterController(	SCommonAttribute commonData, 
						SHeaterControllerRefCompList listRefComponent, 
						SHeaterControllerData datComponent);

    /** @stereotype destructor */
	virtual ~MHeaterController();
 

    /**
     * Initialize Heaer Controller Component. 
	 *
     * Local variable initialization
     * @return  Error Code : 0 = Success
     *                        그외= Error
     */
    virtual int Initialize();

/*---------- Controlling Temperature Operations --------------------*/
	/**
	* Setting Temperature of the given indexed Heater
	*
	* @param	iHeaterIndex : heater index - 1,2,3 (1-Based)
	* @param	dTemperature : temperature to be set( Min/Max definded in DefAcfTool.h )
	* @return	Error Code : 0 = SUCCESS, etc = Error
	*/
	virtual int	SetHeaterTemperature( EHeaterControllerID iHeaterIndex, double dTemperature );

	/**
	* Getting Temperature of the given indexed Heater
	*
	* @param	iHeaterIndex : heater index - 1,2,3 (1-Based)
	* @param	dTemperature : temperature to get( Min/Max definded in DefAcfTool.h )
	* @return	Error Code : 0 = SUCCESS, etc = Error
	*/
	virtual int	GetHeaterTemperature( EHeaterControllerID iHeaterIndex, double *dTemperature );


	/**
	* Getting Setting Temperature of the given indexed Heater
	*
	* @param	iHeaterIndex : heater index - 1,2,3 (1-Based)
	* @param	dTemperature : temperature to get( Min/Max definded in DefAcfTool.h )
	* @return	Error Code : 0 = SUCCESS, etc = Error
	*/
	virtual int	GetSettingHeaterTemperature( EHeaterControllerID iHeaterIndex, double *dTemperature );

	/**
	* Setting Temperature of all Heaters
	*
	* @param	iNumHeaters : array size
	* @param	eHeaterIndex : Heater indexes
	* @param	rgdTemperature : temperature array to be set( Min/Max definded in DefHeaterController.h )
	* @return	Error Code : 0 = SUCCESS, etc = Error
	*/
	virtual int	SetHeaterTemperature(	int iNumHeaters, 
										EHeaterControllerID *eHeaterIndex, 
										double *rgdTemperature );
	/**
	* Getting Temperature of all Heaters
	*
	* @param	dTemperature : temperature array to get( Min/Max definded in DefAcfTool.h )
	* @return	Error Code : 0 = SUCCESS, etc = Error
	*/
	virtual int	GetHeaterTemperature( double *dTemperature, int *iNumHeaters );

	/**
	* Getting Setting Temperature of all Heaters
	*
	* @param	dTemperature : temperature array to get( Min/Max definded in DefAcfTool.h )
	* @return	Error Code : 0 = SUCCESS, etc = Error
	*/
	virtual int	GetSettingHeaterTemperature( double *dTemperature, int *iNumHeaters );

	/**
	* Check Heater Alarms
	*
	* @param	alarm io index : return value of the alarm i/o 
	* @return	boolean value : TRUE - Alarm State, FALSE - no alarm state 
	*/
	virtual BOOL IsHeaterInAlarmState(int *iAlarmIndex = NULL );

	/**
	* Check Heater Alarms of the given indexed Heater
	*
	* @param	iHeaterIndex : heater index - 1,2,3 ...(1-Based) - defined in a definition file
	* @param	HeaterAlarm : FALSE = No Alram, TURE = Alarm
	* @return	Error : 0 - Success, etc. = error
	*/
	virtual int	CheckHeaterAlarms( EHeaterControllerID iHeaterIndex, BOOL *pbStates );

	/**
	* Check Heater Alarms of all Heaters
	*
	* @param	HeaterAlarm : FALSE = No Alram, TURE = Alarm
	* @return	Error : 0 - Success, etc. = error
	*/
	virtual int	CheckHeaterAlarms( BOOL *rgbStaes, int *iNumHeaters );

	/**
	* Assign component linkage
	* 
	* @param	listRefComponent	: SAcfToolRefCompList information
	*/
	virtual int AssignComponents(SHeaterControllerRefCompList listRefComponent);

	/** 
	* Component Data 처리 
	*
	* @param	pData : SAcfToolData구조체에 대한 포인터
	*/
	virtual int SetData(SHeaterControllerData datComponent);

	/** 
	* Component Data 처리 
	*
	* @param	pData : SDrawerData구조체에 대한 포인터
	*/
	virtual int GetData(SHeaterControllerData *pData);

 private:

	/** Make Sending command of Serial Communication 
	*
	* @param	cmdTemperature : enumeration of control command defined in DefHeaterController.h
	* @param	eHeaterIndex : heater index
	*/
	void makeSerialTemperatureCommand(	ETemperatureControlCommand cmdTemperature, 
										EHeaterControllerID eHeaterIndex, 
										void *pData = NULL );
};
#endif //MHEATER_CONTROLLER_H
