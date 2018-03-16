/* 
 * RollerConveyor Component
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

#ifndef M_ROLLER_CONVEYOR_H
#define M_ROLLER_CONVEYOR_H

#include "DefSystem.h"
#include "ICommon.h"
#include "DefRollerConveyor.h"
#include "IStateRollerConveyor.h"
//#include "MTeachingInfo.h"

/**
 * MRollerConveyor.h : This file defines the class of RollerConveyor component.
 *
 */
class MRollerConveyor : public IStateRollerConveyor, public virtual ICommon
{
private:
	IIO*					m_plnkIO;

	ICylinder*				m_plnkStopperUDCyl;

	IInverter*				m_plnkInverter;

	// 20081110 by kss
	ICylinder*				m_plnkPanelPreAlignFBCyl;

	IStatePanelAligner*		m_plnkStatePanelAligner;

	IStatePanelTransfer*	m_plnkStatePanelTransfer;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
	/** PreBonder Data		*/
	MPreBonderData*			m_plnkPreBonderData;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
	/** ACF Data			*/
	MACFData*				m_plnkACFData;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	/** Final Data			*/
	MFinalData*				m_plnkFinalData;
#endif

	/** Panel 감속 감지를 위한 IO Address */
	unsigned short	m_usIAddrPanelDecelDetect;

	/** Panel 정지 감지를 위한 IO Address */
	unsigned short	m_usIAddrPanelStopDetect;

	/** Roller Conveyor Index */
	int				m_iRollerConveyorIndex;

	/** Roller Conveyor 고속 속도 */
	double			m_dNormalSpeed;

	/** Roller Conveyor 저속 속도 */
	double			m_dReducedSpeed;

	/** Roller Conveyor Transfer 이송 속도 */
	double			m_dTransferSpeed;

	/** Roller Conveyor 현재 속도 */
	double			m_dRunningSpeed;

// jdy sesl	char			m_szLogMsg[250];

public:    
	MRollerConveyor();

	MRollerConveyor(SCommonAttribute commonData, SRollerConveyorRefCompList listRefComponents, SRollerConveyorData datComponent);

    ~MRollerConveyor();    
	
/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

	/**
	 * RollerConveyor에 Data Parameter를 설정한다. 
	 *
	 * @param	datcomponent : 설정할 RollerConveyor Parameter
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SetData(SRollerConveyorData datcomponent);

	/**
	 * RollerConveyor에 Component List를 설정한다.
	 *
	 * @param	RollerConveyorCompList : 설정할 RollerConveyor Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignComponents(SRollerConveyorRefCompList RollerConveyorCompList);

	/**
	 * RollerConveyor에 State 확인할 Component List를 설정한다.
	 *
	 * @param	sCompList : 설정할 State 확인을 위한 Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignStateCheckComponents(SStateCheckCompListForRollerConveyor sCompList);


	int UpStopperUDCyl(BOOL bSkipSensor = FALSE, BOOL bUseAuto = TRUE);

	int DownStopperUDCyl(BOOL bSkipSensor = FALSE, BOOL bUseAuto = TRUE);

	//20081110 by kss
	int ForwardPanelPreAlignFBCyl(BOOL bSkipSensor = FALSE);

	int BackwardPanelPreAlignFBCyl(BOOL bSkipSensor = FALSE);

	BOOL IsUpStopperUDCyl(void);
	
	BOOL IsDownStopperUDCyl(void);

	//20081110 by kss
	BOOL IsForwardPanelPreAlignFBCyl(void);

	BOOL IsBackwardPanelPreAlignFBCyl(void);

	BOOL IsInMovingForward(void);

	BOOL IsInMovingBackward(void);

	BOOL IsNormalSpeed(void);

	BOOL IsReducedSpeed(void);

	BOOL IsTransferSpeed(void);

	BOOL IsStopMoving(void);

	int MoveForward(BOOL bUseAuto = TRUE);

	int MoveBackward(BOOL bUseAuto = TRUE);

	int MoveForwardReducedSpeed(BOOL bUseAuto = TRUE);

	int MoveBackwardReducedSpeed(BOOL bUseAuto = TRUE);

	int MoveForwardTransferSpeed(BOOL bUseAuto = TRUE);

	int ReduceSpeed();

	int StopMoving();

	BOOL IsInverterReady();

	BOOL IsInverterFault();

	int ResetInverterFault();

	BOOL IsPanelDecelDetected();

	BOOL IsPanelStopDetected();

	BOOL IsPanelDetected();

	double GetSpeed(int iSpeedType);

	int checkInterlockForMove(int Direction, BOOL bUseAuto = TRUE);

};
#endif //M_ROLLER_CONVEYOR_H
