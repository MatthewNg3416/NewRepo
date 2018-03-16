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
 * IOpPanel.h : This file defines the interface class of op-panel component.
 *			  이 파일은 op-panel Class에 대한 interface class를 정의한다.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef IOPPANEL_H
#define IOPPANEL_H

#include "DefOpPanel.h"
#include "ICommon.h"

/** This class is defined OpPanel component interface class as a abstract class.
 * 이 Class는 OpPanel Class에 대한 Interface Class로 정의한다.
 * 
 * @author Yong-seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 *
 * @interface
 */
class IOpPanel : public virtual ICommon
{
public:    
    virtual BOOL GetStartButtonStatus() = 0;

    virtual BOOL GetStopButtonStatus() = 0;

	//20100212.kms____________________
	virtual BOOL GetStopButtonStatus(BOOL* bIsFrontStopBtn) = 0;

    virtual BOOL GetResetButtonStatus() = 0;

    virtual int GetEStopButtonStatus() = 0;

    virtual int GetTPStatus() = 0;

    virtual BOOL GetTPStopButtonStatus() = 0;

    virtual BOOL GetJogYPlusButtonStatus() = 0;

    virtual BOOL GetJogYMinusButtonStatus() = 0;

    virtual BOOL GetJogXPlusButtonStatus() = 0;

    virtual BOOL GetJogXMinusButtonStatus() = 0;

    virtual BOOL GetJogZPlusButtonStatus() = 0;

    virtual BOOL GetJogZMinusButtonStatus() = 0;

    virtual BOOL GetJogTPlusButtonStatus() = 0;

    virtual BOOL GetJogTMinusButtonStatus() = 0;

    virtual BOOL GetSafeDoorStatus(int iGroup = -1, int iIndex = -1) = 0;

    virtual BOOL GetAirErrorStatus(int iIndex = -1) = 0;

    virtual BOOL GetVacuumErrorStatus(int iIndex = -1) = 0;

	virtual void SetDoorCheckFlag(BOOL bFlag, int iGourp = -1, int iIndex = -1) = 0;

	virtual void SetMaterialChangeFlag(BOOL bFlag, int iGroup = -1, int iIndex = -1) = 0;

    virtual int SetStartLamp(BOOL bStatus) = 0;

    virtual int SetStopLamp(BOOL bStatus) = 0;

    virtual int SetResetLamp(BOOL bStatus) = 0;

    virtual int SetTowerRedLamp(BOOL bStatus) = 0;

    virtual int SetTowerYellowLamp(BOOL bStatus) = 0;

    virtual int SetTowerGreenLamp(BOOL bStatus) = 0;

    virtual int SetBuzzerStatus(int iMode, BOOL bStatus) = 0;

    virtual int GetEnabledOpPanelID() = 0;

    virtual int ChangeOpPanel(int iOpPanelID) = 0;

    virtual int GetOpPanelSelectSW() = 0;

    virtual void SetIOAddress(SOpPanelIOAddr opIOAddress) = 0;

    virtual void GetIOAddress(SOpPanelIOAddr *popIOAddress) = 0;

    virtual void SetJogTable(SJogTable sJogTable) = 0;

    virtual void GetJogTable(SJogTable *psJogTable) = 0;

	virtual int MoveJogSlow(int iUnitIndex, int iKey, BOOL bDir) = 0;

	virtual int MoveJogFast(int iUnitIndex, int iKey, BOOL bDir) = 0;

	virtual int StopJog(int iUnitIndex, int iKey) = 0;

	virtual int StopJog(int iUnitIndex)	= 0;

	virtual int CheckAxisStateForJog(int iUnitIndex, int iKey, BOOL bDir) = 0;

	virtual int GetMotionType(int iUnitIndex, int iKey) = 0;

    virtual void AssignComponents(IIO *pIO) = 0;

};
#endif //IOPPANEL_H
