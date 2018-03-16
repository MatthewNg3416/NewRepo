/* 
 * ModelChange Component
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
 * MModelChange.h : Headerfile of stage component.
 */

#ifndef M_MODEL_CHANGE_H
#define M_MODEL_CHANGE_H

#include "DefModelChange.h"
#include "ICommon.h"
#include "IStateModelChange.h"


const double COMPARE_PANEL_SIZE_TOLERANCE = 5.0;
const double COMPARE_AIR_PLATE_SIZE = 150.0;

/** ModelChange Y 축을 가지는 ModelChange Unit을 모델링한 Class이다. */
class MModelChange : public IStateModelChange, public virtual ICommon
{
protected:

/************************************************************************/
/*								I/O										*/
/************************************************************************/
	/** ModelChange Locl/Unlock I/O */
	IIO*		m_plnkIO;

/************************************************************************/
/*								Cylinder								*/
/************************************************************************/
	/** ModelChange Expand/Shrink Cylinder */
	ICylinder*	m_plnkESCyl;

	/** ModelChange UP/DOWN Cylinder */
	ICylinder*	m_plnkUDCyl;

	ICylinder*	m_plnkAirRailFB1;
	ICylinder*	m_plnkAirRailFB2;

/************************************************************************/
/*								Motion									*/
/************************************************************************/
	/** ModelChange Y Motion Object */
	IAxis*	m_plnkY;

/************************************************************************/
/*								Data									*/
/************************************************************************/
	/** System Data Object */
	MSystemData*	m_plnkSystemData;

	/** Panel Data Object */
	MPanelData*		m_plnkPanelData;
	
	int				m_iWorkGroup;
	EWorkingSide	m_eWorkSide;
/************************************************************************/
/*								I/O Output								*/
/************************************************************************/
	/** Lock을 위한 IO Address */
	unsigned short	m_usOLockModelChangeRail;

	/** Unlock을 위한 IO Address */
	unsigned short	m_usOUnlockModelChangeRail;

/************************************************************************/
/*								Position								*/
/************************************************************************/
	CString		m_strPosFileName;
	CString		m_strOffsetPosFileName;
	CString		m_strOffsetPosFilePath;

	/** PanelAlignerXYT축 Teaching 정보				*/
    STeachingInfo		m_sPanelAlignerXYTTeachingInfo;	
	/** PanelTransfer X축 Teaching 정보				*/
	STeachingInfo		m_sPanelTransferXTeachingInfo;
	/** ModelChange Y축 Teaching 정보 */
	STeachingInfo	m_sYTeachingInfo;

	/** ModelChange X축 고정 좌표 */
	double		m_rgdYFixedPos[DEF_MODEL_CHANGE_MAX_POSITION];
	/** ModelChange Y축 Offset 좌표 */
	double		m_rgdYOffsetPos[DEF_MODEL_CHANGE_MAX_POSITION];
	/** ModelChange Y축 모델 좌표 */
	double		m_rgdYModelPos[DEF_MODEL_CHANGE_MAX_POSITION];

/************************************************************************/
/*		기타															*/
/************************************************************************/
// jdy sesl	char		m_szLogMsg[250];

public:

/************************************************************************/
/*		Component 공통 제공 Interface									*/
/************************************************************************/
	/**
	 * ModelChange에 Component List를 설정한다.
	 *
	 * @param	SModelChangeRefCompList : 설정할 ModelChange Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignComponents(SModelChangeRefCompList ModelChangeCompList);

	/**
	 * ModelChange의 Motion Component를 돌려 받는다.
	 *
	 * @return	IAixs : 해당하는 축 ID에 맞는 Axis 
	 */
	IAxis* GetMotionComponent();

/************************************************************************/
/*		Teaching 관련 제공 Interface									*/
/************************************************************************/
	/**
	 * ModelChange Y 축 관련 고정좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadYFixedPosParameter(void);

	/**
	 * ModelChange Y 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadYOffsetPosParameter(void);

	/**
	 * ModelChange Y 축 관련 고정좌표 Data를 File에 저장한다.
	 *
	 * @param	rgdFixedPos : 저장할 고정좌표 배열 (Y)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveYFixedPosParameter(double* rgdFixedPos);

	/**
	 * ModelChange Y 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgdOffsetPos : 저장할 Offset좌표 배열 (Y)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveYOffsetPosParameter(double* rgdOffsetPos);

	/**
	 * ModelChange Y 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	rgdFixedPos : 고정좌표 배열 (Y)
	 * @param	rgdOffsetPos : Offset좌표 배열 (Y)
	 * @param	rgdModelPos : Model좌표 배열 (Y)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetYPosParameter(double* rgdFixedPos, double* rgdOffsetPos, double* rgdModelPos);

	/**
	 * ModelChange Y Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
	 */
	STeachingInfo GetYTeachingInfo();

/************************************************************************/
/*		Cylinder Operation Interface									*/
/************************************************************************/

	/**
	 * ModelChange ES Cyl를 Expand 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int Expand(BOOL bSkipSensor = FALSE);

	/**
	 * ModelChange ES Cyl Shrink 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int Shrink(BOOL bSkipSensor = FALSE);

	/**
	 * ModelChange UD Cyl를 UP 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int Up(BOOL bSkipSensor = FALSE);

	/**
	 * ModelChange UD Cyl DOWN 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int Down(BOOL bSkipSensor = FALSE);

	/**
	 * ModelChange Expand/Shrink Cylinder 의 Expand 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * ModelChange Expand/Shrink Cylinder 의 Shrink 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

	/**
	 * ModelChange Up/Down Cylinder 의 Up 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Up , FALSE = Down)
	 */
	BOOL IsUp(void);

	/**
	 * ModelChange Up/Down Cylinder 의 Down 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Down , FALSE = Up)
	 */
	BOOL IsDown(void);


	int ForAirRailFB1(BOOL bSkipSensor = FALSE);
	int ForAirRailFB2(BOOL bSkipSensor = FALSE);

	int BackAirRailFB1(BOOL bSkipSensor = FALSE);
	int BackAirRailFB2(BOOL bSkipSensor = FALSE);

	BOOL IsForAirRailFB1();
	BOOL IsForAirRailFB2();

	BOOL IsBackAirRailFB1();
	BOOL IsBackAirRailFB2();

/************************************************************************/
/*		I/O Operation Interface											*/
/************************************************************************/

	/**
	 * ModelChange Rail을 Lock 시킨다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LockRail();

	/**
	 * ModelChange Rail을 Unlock 시킨다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int UnlockRail();

	/**
	 * ModelChange Rail을 Lock 시키는 I/O 출력 On을 확인한다.
	 *
	 * @return	BOOL : (TRUE = I/O On, FALSE = I/O Off)
	 */
	BOOL IsLockRail(void);

	/**
	 * ModelChange Rail을 Lock 시키는 I/O 출력 Off를 확인한다.
	 *
	 * @return	BOOL : (TRUE = I/O Off, FALSE = I/O On)
	 */
	BOOL IsUnlockRail(void);

/************************************************************************/
/*		ModelChange Move Operation Interface							*/
/************************************************************************/
	/**
	 * ModelChange Y 축을 Index 위치로 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bUseAuto : Manual Mode로 사용 할 것인지 않할 것인지 확인 (FLASE : Manual Mode, FALSE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeMoveYPos(int iPosID, BOOL bUseAuto = TRUE, BOOL bWaitOption = TRUE);

/************************************************************************/
/*		ModelChange Position 확인 Interface								*/
/************************************************************************/
	/**
	 * ModelChange Y 축 원점복귀 여부를 확인한다. 원점복귀가 되어 있으면 Success Return, 
	 * 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckOrigin(void);

	/**
	 *	ModelChange Y Motion 중인지 확인한다.
	 */
	BOOL IsInYMotion();

	/**
	 * Y 축의 현재 위치를 읽는다.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double	: ModelChange의 현재 위치 (Z축)
	 */
	double GetYCurrentPos(BOOL bType = FALSE);

	/**
	 * Y 축의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : ModelChange의 목표 위치 (Y축)
	 */
	double GetYTargetPos(int iPosID);

	/**
	 * Y 축의 목표 너비를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : ModelChange의 목표 너비 (Y축)
	 */
	double GetTargetWidth(int iPosID);

	/**
	 * Y 축의 현재 너비를 읽는다.
	 *
	 * @param	bType	: (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double	: ModelChange의 현재 너비 (Y축)
	 */
	double GetCurrentWidth(BOOL bType = FALSE);
	
	/**
	 * ModelChange Y 축의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInYPos(int iPosID, double* pdTolerance = NULL);

	/**
	 * ModelChange Y 축의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInYPos(double dTargetPos, double* pdTolerance = NULL);

private:

/**********
 *  Mechanical Layer State Check Components
 */
	// 설비별 Unit 설정.

	IStatePanelAligner*		m_plnkStatePanelAligner;
//@	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkStatePanelTransfer;
#endif
	IStatePanelTransfer*	m_plnkStatePanelTransferOut;
	
	IStateCameraCarrier*	m_plnkStateCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private:
	/**
	 * ModelChange(Y축) 이동전에 Interlock을 확인한다.
	 *
	 * @param	iPosID : 목표 위치 ID (iPosID == DEF_PANEL_TRANSFER_NONE_POS 이면 Interlock check 안함.)
	 * @param	bUseAuto : Auto mode 사용 여부를 설정 (default = TRUE)
	 * @return	int (0 = Safety, Error Code = 그 외)
	 */
	int checkInterlockForPanelY(int iPosID, BOOL bUseAuto);

	int checkInterlockForRailMove();

	/** ModelChange Y축 관련 좌표를 계산한다.	*/
	int calculateModelChangePanelYModelPosParameter();

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
	MModelChange(SCommonAttribute commonData, SModelChangeRefCompList listRefComponents, SModelChangeData datComponent);

	/** 소멸자 함수 */
	~MModelChange();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
	/**
	 * ModelChange에 Data Parameter를 설정한다. 
	 *
	 * @param	SModelChangeParam	: 설정할 ModelChange Parameter
	 * @return	Error Code	: 0 = SUCCESS, 그 외 = Error
	 */
	int	SetData(SModelChangeData datcomponent);

	/**
	 * ModelChange가 상태 체크를 위해 필요한  Component List를 Assign 한다. 
	 *
	 * @param	SStateCheckCompList : Component List
	 */
	int	AssignStateCheckComponents(SStateCheckCompListForModelChange sCompList);

	/**
	 * Model 변경시 작업을 수행한다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int	ChangeModel();

	/**
	 * ModelChange 운전 초기 상태로 이동한다.
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
	 * ModelChange(Y축)를 Index위치로 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	dTarget	: 좌표
	 * @param	iPosID	: Position Index ID
	 * @param	bUseAuto : Auto Mode로 사용 할 것인지 여부 설정 (default = TRUE)
	 *						(FLASE : Manual Mode, TRUE : Auto Mode)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int MoveYPos(double dTarget, int iPosID, BOOL bUseAuto = TRUE, BOOL bWaitOption = TRUE);
	int Wait4DoneY(BOOL bSkipMode = FALSE);
/**********
 * Etc.
 */
	/**
	 *	ModelChange(Y축) Model Position 에 적용된 Offset 값을 얻는다.
	 */
	double GetModelPosOffset();

};
#endif //M_MODEL_CHANGE_H
