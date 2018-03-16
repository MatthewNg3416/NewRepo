/* 
 * OpPanel Component
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
 * MOpPanel.h : This file defines the class of op-panel component.
 *			  이 파일은 op-panel Class에 대한 class를 정의한다.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef MOPPANEL_H
#define MOPPANEL_H

#include "IOpPanel.h"

/** This class is defined OpPanel component implements class as a abstract class.
 * 이 Class는 OpPanel Class에 대한 implements Class로 정의한다.
 * 
 * @author Yong-seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */
class MOpPanel : public IOpPanel
{
private:    
    /**
	 * Panel에 있는 각종 Switch 및 LED, Lamp, Buzzer등의 IO Address Table
	 *
     * @link aggregationByValue
     * @supplierCardinality 1 
     */
    SOpPanelIOAddr	m_IOAddrTable;

    /**
	 * Jog Key로 움직일 Motion 대상 관련 정보 Table
     */
    SJogTable		m_JogTable;

    /**
	 * IO Object
	 *
     * @link aggregation
     * @supplierCardinality 1 
     */
    IIO*			m_plnkIO;

public:    

    /**
	 * 생성자
	 *
     * @stereotype constructor 
     */
    MOpPanel();

    /**
	 * 생성자 with 초기화
	 *
	 * @param	iObjectID : Object ID
	 * @param	*pIO : IO Object
	 * @param	iErrorBase : Error Base
	 * @param	strLogFullFileName : file path 및 file name을 가지는 string
	 * @param	ucLogLevel : log level 지정 bitwise 정보
	 * @param	iLogDays : 현재 set되어 있는 log file 저장일
	 * @param	sPanelIOAddr : 각종 IO Address
	 * @param	sJogTable : Jog로 움직일 대상에 대한 정보
	 *
     * @stereotype constructor 
     */
    MOpPanel(SCommonAttribute commonData, IIO *pIO, SOpPanelIOAddr sPanelIOAddr, SJogTable sJogTable);

    /**
	 * 소멸자
	 *
     * @stereotype destructor 
     */
    virtual ~MOpPanel();


    virtual BOOL GetStartButtonStatus();

    virtual BOOL GetStopButtonStatus();

	//20100212.kms____________________
	virtual BOOL GetStopButtonStatus(BOOL* bIsFrontStopBtn);

    virtual BOOL GetEStopButtonStatus();

    virtual BOOL GetTPStatus();

    virtual BOOL GetTPStopButtonStatus();

    virtual BOOL GetJogYPlusButtonStatus();

    virtual BOOL GetJogYMinusButtonStatus();

    virtual BOOL GetJogXPlusButtonStatus();

    virtual BOOL GetJogXMinusButtonStatus();

    virtual BOOL GetJogZPlusButtonStatus();

    virtual BOOL GetJogZMinusButtonStatus();

    virtual BOOL GetJogTPlusButtonStatus();

    virtual BOOL GetJogTMinusButtonStatus();

	virtual void SetDoorCheckFlag(BOOL bFlag, int iGroup = -1, int iIndex = -1);

	virtual void SetMaterialChangeFlag(BOOL bFlag, int iGroup = -1, int iIndex = -1);

    virtual int SetStartLamp(BOOL bStatus);

    virtual BOOL GetResetButtonStatus();

    virtual BOOL GetSafeDoorStatus(int iGroup = -1, int iIndex = -1);

    virtual BOOL GetAirErrorStatus(int iIndex = -1);

    virtual BOOL GetVacuumErrorStatus(int iIndex = -1);
	

    virtual int SetStopLamp(BOOL bStatus);

    virtual int SetResetLamp(BOOL bStatus);

    virtual int SetTowerRedLamp(BOOL bStatus);

    virtual int SetTowerYellowLamp(BOOL bStatus);

    virtual int SetTowerGreenLamp(BOOL bStatus);

    virtual int SetBuzzerStatus(int iMode, BOOL bStatus);

    virtual int GetEnabledOpPanelID();

    virtual int ChangeOpPanel(int iOpPanelID);

    virtual int GetOpPanelSelectSW();


    virtual void SetIOAddress(SOpPanelIOAddr opIOAddress);

    virtual void GetIOAddress(SOpPanelIOAddr *popIOAddress);

    virtual void SetJogTable(SJogTable sJogTable);

    virtual void GetJogTable(SJogTable *psJogTable);

	virtual int MoveJogSlow(int iUnitIndex, int iKey, BOOL bDir);

	virtual int MoveJogFast(int iUnitIndex, int iKey, BOOL bDir);

	virtual int StopJog(int iUnitIndex, int iKey);

	virtual int StopJog(int iUnitIndex);

	virtual int CheckAxisStateForJog(int iUnitIndex, int iKey, BOOL bDir);

	virtual int GetMotionType(int iUnitIndex, int iKey);

    virtual void AssignComponents(IIO* pIO);

/** Internal Operation */
private:

//@	BOOL getPanelSwitchStatus(CString strLogName, unsigned short usiFrontSWAddr, unsigned short usiBackSWAddr);
	BOOL getPanelSwitchStatus(CString strLogName, unsigned short usiFrontSWAddr, unsigned short usiBackSWAddr,
			unsigned short usiFrontPressSWAddr, unsigned short usiBackPressSWAddr);

	int setPanelLedStatus(CString strLogName, unsigned short usiFrontLedAddr, unsigned short usiBackLedAddr, BOOL bStatus);

	int setTowerLampStatus(CString strLogName, unsigned short usiTowerAddr, BOOL bStatus);

	void intializeData();
};

#endif //MOPPANEL_H
