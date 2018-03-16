/* 
 * PanelTransfer Component
 *
 * Copyright 2005 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MPanelTransfer.h : Headerfile of stage component.
 */

#ifndef M_TRANSFER_H
#define M_TRANSFER_H

#include "ICommon.h"
#include "DefPanelTransfer.h"
#include "IStatePanelTransfer.h"
#include "IStateCameraCarrier.h"

/** PanelTransfer X 축을 가지는 PanelTransfer Unit을 모델링한 Class이다. */
class MPanelTransfer : public IStatePanelTransfer, public virtual ICommon
{
protected:

/************************************************************************/
/*                             IO                                       */
/************************************************************************/
	IIO*		m_plnkIO;

/************************************************************************/
/*                             Vacuum                                   */
/************************************************************************/
	/** PanelTransfer Vacuum */
	IVacuum*	m_plnkVac;
	IVacuum*	m_plnkVac2;

/************************************************************************/
/*                             Cylinder                                 */
/************************************************************************/
	/** PanelTransfer Up/Down Cylinder */
	ICylinder*	m_plnkUDCyl;

	/** Stopper Up/Down Cylinder */
	ICylinder*	m_plnkStopperUDCyl;

	ICylinder*	m_plnkAirRailFB1;
	ICylinder*	m_plnkAirRailFB2;

/************************************************************************/
/*                             Motion                                   */
/************************************************************************/
	/** PanelTransfer X Motion Object */
	IAxis*	m_plnkX1;
	IAxis*	m_plnkX2;

	/** PanelTransfer X MultiMotion Object */
	//ISercosMultiAxis*	m_plnkMultiX;
	IRSNMCMultiAxis*	m_plnkMultiX;

/************************************************************************/
/*                             Data                                     */
/************************************************************************/
	/** System Data Object */
	MSystemData*	m_plnkSystemData;

	/** Panel Data Object */
	MPanelData*		m_plnkPanelData;

/************************************************************************/
/*                             IO Address                               */
/************************************************************************/
	
	/** Air CVBlow Air Blow Output IO Address	*/
//@	unsigned short		m_usOAddrAirBlow1;
//@	unsigned short		m_usOAddrAirBlow2;

	/** Air CVBlow Air Blow Input IO Address	*/
//@	unsigned short		m_usIAddrAirBlow1;
//@	unsigned short		m_usIAddrAirBlow2;

	/** Air CVBlow Air Vacuum Output IO Address	*/
//@	unsigned short		m_usOAddrAirVacuum1;
//@	unsigned short		m_usOAddrAirVacuum2;

	/** Air CVBlow Air Vacuum Input IO Address	*/
//@	unsigned short		m_usIAddrAirVacuum1;
//@	unsigned short		m_usIAddrAirVacuum2;

	unsigned short		m_usIPanelDetect;

	//120224.kms_______
	unsigned short      m_usIPanelDetectLoading;
	unsigned short      m_usIPanelDetectUnLoading;
	//_________________

	// Transfer Type
	UINT				m_uiTransferType;

/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	CString		m_strPosFileName;
	CString		m_strOffsetPosFileName;
	CString		m_strOffsetPosFilePath;

	/** PanelTransfer X축 Teaching 정보 */
	STeachingInfo	m_sXTeachingInfo;

	/** PanelTransfer X축 고정 좌표 */
	double		m_rgdXFixedPos[DEF_PANEL_TRANSFER_MAX_POSITION];
	/** PanelTransfer X축 Offset 좌표 */
	double		m_rgdXOffsetPos[DEF_PANEL_TRANSFER_MAX_POSITION];
	/** PanelTransfer X축 모델 좌표 */
	double		m_rgdXModelPos[DEF_PANEL_TRANSFER_MAX_POSITION];

/************************************************************************/
/*        기타
/************************************************************************/
// jdy sesl	char		m_szLogMsg[250];

public:

/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

	/**
	 * PanelTransfer에 Component List를 설정한다.
	 *
	 * @param	SPanelTransferRefCompList : 설정할 PanelTransfer Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignComponents(SPanelTransferRefCompList PanelTransferCompList);

	/**
	 * PanelTransfer의 Motion Component를 돌려 받는다.
	 *
	 * @param nAxisID : 축 ID (0: X축, 1:Y축 , 2:T축, 3:PanelX축)
	 * @return	IAixs : 해당하는 축 ID에 맞는 Axis 
	 */
	IAxis* GetMotionComponent(int nAxisID);

	/**
	 * PanelTransfer의 MultiMotion Component를 돌려 받는다.
	 *
	 * @return	ISercosMultiAxis : 해당하는 Multi축 ID에 맞는 Axis 
	 */
	//ISercosMultiAxis* GetMultiMotionComponent();
	IRSNMCMultiAxis* GetMultiMotionComponent();

/************************************************************************/
/*        Teaching 관련 제공 Interface                                  */
/************************************************************************/

	/**
	 * PanelTransfer X 축 관련 고정좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadXFixedPosParameter(void);

	/**
	 * PanelTransfer X 축 관련 고정좌표 Data를 File에 저장한다.
	 *
	 * @param	rgdFixedPos : 저장할 고정좌표 배열 (X)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveXFixedPosParameter(double* rgdFixedPos);

	/**
	 * PanelTransfer X 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadXOffsetPosParameter(void);

	/**
	 * PanelTransfer X 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgdOffsetPos : 저장할 Offset좌표 배열 (X)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveXOffsetPosParameter(double* rgdOffsetPos);

	/**
	 * PanelTransfer X 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	rgdFixedPos : 고정좌표 배열 (X)
	 * @param	rgdOffsetPos : Offset좌표 배열 (X)
	 * @param	rgdModelPos : Model좌표 배열 (X)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetXPosParameter(double* rgdFixedPos, double* rgdOffsetPos, double* rgdModelPos);

	/**
	 * PanelTransfer X Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
	 */
	STeachingInfo GetXTeachingInfo();

/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/

	/**
	 * PanelTransfer 의 Vacuum이 정상인지 확인한다. 정상이면 Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckVacuum(void);

	/**
	 * PanelTransfer 의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AbsorbPanel(BOOL bSkipSensor = FALSE);

	/**
	 * PanelTransfer 의 Vacuum을 해제한다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ReleasePanel(BOOL bSkipSensor = FALSE);

	/**
	 * PanelTransfer 의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	BOOL IsPanelAbsorbed(void);

	/**
	 * PanelTransfer 의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	BOOL IsPanelReleased(void);

	/**
	 * PanelAligner의 Plate Unit에서 Air Blow 부상한다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int AirCVBlowOn();

	/**
	 * PanelAligner의 Plate Unit에서 Air Blow 부상 해제한다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int AirCVBlowOff();

	/**
	 * PanelAligner의 Plate Unit에서 Air Blow 부상 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 부상 상태, FALSE = 해제 상태)
	 */
//@	BOOL IsAirCVBlowOn();

	/**
	 * PanelAligner의 Plate Unit에서 Air Blow 부상해제 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 부상 상태)
	 */
//@	BOOL IsAirCVBlowOff();

	/**
	 * PanelAligner의 Plate Unit 전체의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int AirCVVacAllOn();

	/**
	 * PanelAligner의 Plate Unit 전체의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int AirCVVacAllOff();

	/**
	 * PanelAligner의 Plate Unit 전체의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
//@	BOOL IsAirCVVacAllOn();

	/**
	 * PanelAligner의 Plate Unit 전체의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
//@	BOOL IsAirCVVacAllOff();

	/**
	 * PanelAligner의 Plate Unit 1,2의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int AirCVVacPlate1On();

	/**
	 * PanelAligner의 Plate Unit 1,2의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int AirCVVacPlate1Off();

	/**
	 * PanelAligner의 Plate Unit 1,2의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
//@	BOOL IsAirCVVacPlate1On();

	/**
	 * PanelAligner의 Plate Unit 1,2의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
//@	BOOL IsAirCVVacPlate1Off();

	/**
	 * PanelAligner의 Plate Unit 3,4의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int AirCVVacPlate2On();

	/**
	 * PanelAligner의 Plate Unit 3,4의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int AirCVVacPlate2Off();

	/**
	 * PanelAligner의 Plate Unit 3,4의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
//@	BOOL IsAirCVVacPlate2On();

	/**
	 * PanelAligner의 Plate Unit 3,4의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
//@	BOOL IsAirCVVacPlate2Off();

	// Air Conveyor 부상
//@	int AirCVFloatingOn();

	// Air Conveyor 흡착
//@	int AirCVFloatingOff();

/************************************************************************/
/*        Cylinder Operation Interface                                  */
/************************************************************************/

	/**
	 * PanelTransfer UD Cyl를 Up 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int UpPickUpUDCyl(BOOL bSkipSensor = FALSE);

	/**
	 * PanelTransfer UD Cyl Down 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int DownPickUpUDCyl(BOOL bSkipSensor = FALSE);

	/**
	 * PanelTransfer Up/Down Cylinder 의 U/D 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsUpPickUpUDCyl(void);

	/**
	 * PanelTransfer Up/Down Cylinder 의 U/D 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsDownPickUpUDCyl(void);


	int ForAirRailFB1(BOOL bSkipSensor = FALSE);
	int ForAirRailFB2(BOOL bSkipSensor = FALSE);

	int BackAirRailFB1(BOOL bSkipSensor = FALSE);
	int BackAirRailFB2(BOOL bSkipSensor = FALSE);

	BOOL IsForAirRailFB1();
	BOOL IsForAirRailFB2();

	BOOL IsBackAirRailFB1();
	BOOL IsBackAirRailFB2();

/************************************************************************/
/*        PanelTransfer Move Operation Interface                                */
/************************************************************************/

	/**
	 * PanelTransfer X 축을 Index 위치로 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bUseAuto : Manual Mode로 사용 할 것인지 않할 것인지 확인 (FLASE : Manual Mode, FALSE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeSyncMoveXPos(int iPosID, BOOL bUseAuto = TRUE);
	int SafeMoveXPos(int iPosID, BOOL bUseAuto = TRUE);

/************************************************************************/
/*        PanelTransfer Position 확인 Interface                                 */
/************************************************************************/

	/**
	 * PanelTransfer X 축 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 
	 * 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckOrigin(void);

	int ResetOrigin(void); //171004 JSH

	/**
	 * X 축의 현재 위치를 읽는다.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double	: PanelTransfer의 현재 위치 (Z축)
	 */
	double GetXCurrentPos(BOOL bType = FALSE);

	/**
	 * X 축의 현재 위치를 읽는다.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double	: PanelTransfer의 현재 위치 (Z축)
	 */
	double GetX2CurrentPos(BOOL bType = FALSE);

	/**
	 * X 축의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : PanelTransfer의 목표 위치 (X축)
	 */
	double GetXTargetPos(int iPosID);

	/**
	 * PanelTransfer X 축의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInXPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * PanelTransfer X 축의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInXPos(double dTargetPos, double* pdTolerance = NULL);

private:

/**********
 *  Mechanical Layer State Check Components
 */

	// 설비별 Unit 설정.
	/** Panel Aligner */
	IStatePanelAligner*		m_plnkStatePanelAligner;

#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;
	IStatePanelTransfer*	m_plnkStatePanelTransferIn;
#endif

	/** Tab Mounter */
	IStateTabMounter*		m_plnkStateTabMounter[DEF_MAX_TABMOUNTER];
	IStateTabMounter*		m_plnkStateRTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateCameraCarrier*	m_plnkStateRInspectCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private:
	/**
	 * PanelTransfer(PanelX축) 이동전에 Interlock을 확인한다.
	 *
	 * @param	iPosID : 목표 위치 ID (iPosID == DEF_PANEL_TRANSFER_NONE_POS 이면 Interlock check 안함.)
	 * @param	bUseAuto : Auto mode 사용 여부를 설정 (default = TRUE)
	 * @return	int (0 = Safety, Error Code = 그 외)
	 */
	int checkInterlockForPanelX(int iPosID, BOOL bUseAuto);

	int checkInterlockForTransferIn(int iPosID, BOOL bUseAuto);	//Source Only
	int checkInterlockForTransferOut(int iPosID, BOOL bUseAuto);

	int checkInterlockForRailMove();

	/**
	 * PanelTransfer PanelX축 관련 좌표를 계산한다.
	 *
	 * @return	int (0 = Safety, Error Code = 그 외)
	 */
	int calculatePanelTransferPanelXModelPosParameter();

public:
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/

	/** 
	 * 생성자
	 * @param commonData		: ObjectI나 Log와 같은 Component Common Data
	 * @param listRefComponent	: 참조할 Component 포인터 리스트
	 * @param SMainToolData		: MainTool Data
	 */
	MPanelTransfer(SCommonAttribute commonData, SPanelTransferRefCompList listRefComponents, SPanelTransferData datComponent);

	/**
	 * 소멸자 함수
	 */
	~MPanelTransfer();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

	/**
	 * PanelTransfer에 Data Parameter를 설정한다. 
	 *
	 * @param	SPanelTransferParam	: 설정할 PanelTransfer Parameter
	 * @return	Error Code	: 0 = SUCCESS, 그 외 = Error
	 */
	int	SetData(SPanelTransferData datcomponent);

	/**
	 * PanelTransfer가 상태 체크를 위해 필요한  Component List를 Assign 한다. 
	 *
	 * @param	SStateCheckCompList : Component List
	 */
	int	AssignStateCheckComponents(SStateCheckCompListForPanelTransfer sCompList);

	/**
	 * Model 변경시 작업을 수행한다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int	ChangeModel();

	/**
	 * PanelTransfer 운전 초기 상태로 이동한다.
	 *
	 * @return	int : 0 = SUCCESS, 그 외 = Error
	 */
	int	Initialize();

/***********************************************************************************************
/*
/* Operation Interface
/*
/************************************************************************************************/ 

/**********
 * Move
 */

	/**
	 * PanelTransfer(PanelX축)를 Index위치로 동기이동시킨다. Interlock을 확인한다.
	 *
	 * @param	dTarget	: 좌표
	 * @param	iPosID	: Position Index ID
	 * @param	bUseAuto : Auto Mode로 사용 할 것인지 여부 설정 (default = TRUE)
	 *						(FLASE : Manual Mode, TRUE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SyncMoveXPos(double dTarget, int iPosID, BOOL bUseAuto = TRUE);
	int MoveXPos(double dTarget, int iPosID, BOOL bUseAuto = TRUE);

/**********
 * Sensor
 */

	/**
	 * PanelTransfer Decel or Stop Sensor 의 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Panel Exists , FALSE = Panel not Exists)
	 */
	BOOL IsPanelDetected();

	//120224.kms__________
	BOOL IsPanelLoadingDetected();
//@	BOOL IsPanelUnLoadingDetected();
	//____________________

	//@모델대응 위치로 센서 찾는 동작
	int	FindSensorTransferInAirRail();

};
#endif //M_TRANSFER_H
