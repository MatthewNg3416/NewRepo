/* 
 * TabMounter Component
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
 * TabMounter.h : Headerfile of stage component.
 */

#ifndef MTABMOUNTER_H
#define MTABMOUNTER_H

#include "ICommon.h"
#include "DefTabMounter.h"
#include "IStateTabMounter.h"
#include <afxmt.h>
#include "MProcessData.h"

class MTabMounter : public virtual ICommon, public IStateTabMounter
{
private:   
	/** Axis Object */
    IAxis*		m_plnkAxis;				// Mounter Y Axis Object
	IAxis*		m_plnkAxisZ;			// Mounter Z Axis Object

	IAxis*		m_plnkGantryAxis;	// Gantry Axis Object

	/** Vacuum Object */
	IVacuum*		m_plnkVac;

	/** Cylinder Object */
    ICylinder*		m_plnk1stUDCyl;

	ICylinder*		m_plnkPusherUDCyl;

#if FALSE	//==============================================
	ICylinder*		m_plnk2ndUDCyl;
	ICylinder*		m_plnkBackupUDCyl;
	ICylinder*		m_plnkBackupFBCyl;
#endif		//#if FALSE	//==================================

	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData* m_plnkPreBonderData;
	MProcessData*	m_plnkProcessData;
//@	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];

	MTabOffsetData*	m_plnkTabOffsetData;
	//120208.kms________
	MTabOffsetData* m_plnkRearTabOffsetData;

	/** PositionInfo.inf File 정보 구조체 : TabMounterY = 0, TabMounterZ = 1 */
	STeachingInfo	m_sTeachingInfo[DEF_TABMOUNTER_AXIS_TYPE];
	

	/** Offset Data가 저장될 File Name */
	CString			m_strOffsetPosFileName;

	/** 파라미터 데이타가 저장될 File Name */
	CString			m_strPosFileName;
	CString			m_strOffsetPosFilePath;

/**********
 *  Mechanical Layer State Check Components
 */
	IStateCameraCarrier*	m_plnkCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateTabMounter*		m_plnkTabMounter[DEF_MAX_WORKER_PER_GROUP];
	IStatePanelAligner*		m_plnkPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	IStatePanelTransfer*	m_plnkPanelTransfer;
//@	IStateTabCarrier*		m_plnkTabCarrier;
//@	IStateTabCarrier*		m_plnkTabCarrierOther;
#endif
	IStateTabCarrier*		m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	IStatePanelTransfer*	m_plnkPanelTransferOut;

/**********
 * 좌표
 */
	/** TabMounter - 고정좌표 */
//    double			m_rgdFixedPos[DEF_TABMOUNTER_AXIS_TYPE][DEF_TABMOUNTER_MAX_POS];	
	/** TabMounter - 모델좌표 */
//    double			m_rgdModelPos[DEF_TABMOUNTER_AXIS_TYPE][DEF_TABMOUNTER_MAX_POS];	
	/** TabMounter - Offset 좌표 */
//    double			m_rgdOffsetPos[DEF_TABMOUNTER_AXIS_TYPE][DEF_TABMOUNTER_MAX_POS];

	/** TabMounter, Gantry - 고정좌표 -> TabMounterY, TabMounterZ 고정좌표 */
	double*			m_rgdFixedPos[DEF_TABMOUNTER_AXIS_TYPE];
	/** TabMounter, Gantry - 모델좌표 -> TabMounterY, TabMounterZ 모델좌표 */
	double*			m_rgdModelPos[DEF_TABMOUNTER_AXIS_TYPE];
	/** TabMounter, Gantry - Offset 좌표 -> TabMounterY, TabMounterZ Offset좌표 */
	double*			m_rgdOffsetPos[DEF_TABMOUNTER_AXIS_TYPE];

	/** TabMounter - 고정좌표 */
    double			m_rgdFixedPosMounter[DEF_TABMOUNTER_MAX_POS];
	// 기구 구조로 인해서 결정되는 TabMounter 축 고정좌표 (SystemData 로부터 계산됨)
	double			m_rgdCalculatedFixedPosMounter[DEF_TABMOUNTER_MAX_POS];
	/** TabMounter - 모델좌표 */
    double			m_rgdModelPosMounter[DEF_TABMOUNTER_MAX_POS];	
	/** TabMounter - Offset 좌표 */
    double			m_rgdOffsetPosMounter[DEF_TABMOUNTER_MAX_POS];

	/** TabMounter Z - 고정좌표 */
    double			m_rgdFixedPosMounterZ[DEF_TABMOUNTER_Z_MAX_POS];
	/** TabMounter - 모델좌표 */
    double			m_rgdModelPosMounterZ[DEF_TABMOUNTER_Z_MAX_POS];	
	/** TabMounter - Offset 좌표 */
    double			m_rgdOffsetPosMounterZ[DEF_TABMOUNTER_Z_MAX_POS];


	// Front
	/** Gantry - 고정좌표 */
    static double	sm_rgdFixedGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];	
	/** Gantry - 모델좌표 */
    static double	sm_rgdModelGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];	
	/** Gantry - Offset 좌표 */
    static double	sm_rgdOffsetGantryPos1[DEF_TABMOUNTER_GANTRY_MAX_POS];

	// Rear
	/** Gantry - 고정좌표 */
    static double	sm_rgdFixedGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];	
	/** Gantry - 모델좌표 */
    static double	sm_rgdModelGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];	
	/** Gantry - Offset 좌표 */
    static double	sm_rgdOffsetGantryPos2[DEF_TABMOUNTER_GANTRY_MAX_POS];

/**********
 * 기타
 */
	/** TabMounter 가 속한 Group 번호 */
	int				m_iGroupNo;

	/** 작업변 정보 */
	EWorkingSide	m_eWorkSide;

	/** Gantry Axis Motion Enable Flag */
	BOOL			m_bGantryMotionEnable;

	/** 각 축 별 최대 Position 개수 저장 */
	int				m_iMaxPos[DEF_TABMOUNTER_AXIS_TYPE];

	/** **************************************************************************
	 *	이전/다음 TabMounter 번호 설정
	 *  ⇒ 이전/다음 구분 기준
	 *		- 자신보다 먼저 Prealign 위치로 이동하는 TabMounter 가 이전 TabMounter 임.
	 *		- Gate 의 경우 TabCarrier Unload 위치에 따라 Front/Rear Group 의 TabMounter 출발 위치가 달라짐.
	 */

	/** 이전 TabMounter 번호 : 1번 TabMounter 의 [이전 TabMounter 번호]는 -1 이다.
	 *	: TabMounter 가 Mount 위치로 이동하기 전에, Previous TabMounter 의 위치를 확인해야 한다.
	 */
	int				m_iPreTabMounterNo;

	/** 다음 TabMounter 번호
	 *	: 각 Group 의 마지막 TabMounter 의 [다음 TabMounter 번호]는 DEF_LAST_PER_GROUP 이다.
	 *	: TabMounter 가 Load 위치로 이동하기 전에, Next TabMounter 의 위치를 확인해야 한다.
	 */
	int				m_iNextTabMounterNo;


	/**(KKY)
	 * 이동시 인근 Carrier와 충돌여부 확인하는데 사용
	 **/
	double						m_posTargetMove;
	BOOL						m_bMoving;
	static CCriticalSection		m_csMoveLock;

	MTabInfo					m_TabInfo;

	int	m_iWorkGroup;
	int m_iWorkInstance;

//	int m_iGantryLoadCarrierID;

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private :

	//KKY
	void					SetMovingY(BOOL bMove, double posTarget);

	/**
	 * 축 이동전에 Interlock을 확인한다.
	 *
	 * @param	iPosID : 목표 위치 ID
	 * @return	int (0 = Safety, Error Code = 그 외)
	 */
	int checkInterlock(int iPosID, double dTarget, BOOL bAuto = FALSE);
	int checkInterlockZ(int iPosID, double dTarget, BOOL bAuto = FALSE);

	// TabMounter UD cylinder Down Interlock
	int checkInterlockForDown(int iDownType);

	/**
	 * 기구 Data 로 결정되는 Fixed 좌표를 계산한다.
	 */
	void calculateFixedPosParameter(int iAxisType);

	/**
	 * Model 좌표를 계산한다.
	 */
	void calculateModelPosParameter(int iAxisType);


	int checkInterlockForGantry(int iPosID, double dTarget, BOOL bAuto = FALSE);

	/**
	 * TabMounter 축 Position ID 로부터 Gantry 축 Position ID 를 얻는다.
	 */
	int getGantryPosID(int iTabMounterPosID);
	/**
	 *	Interlock Check 를 배제한 Gantry Move 동작을 수행한다.
	 */
	int moveGantryPos(double dGantryTargetPos, int iGantryPosID, BOOL bWaitOption, BOOL bAuto);

public:
	/** 
	 * 생성자
	 *
	 * @param commonData : ObjectI나 Log와 같은 Component Common Data
	 * @param listRefComponent : 참조할 Component 포인터 리스트
	 * @param datComponent : TabMounter Data
	 */
	MTabMounter(SCommonAttribute commonData, STabMounterRefCompList listRefComponents, STabMounterData datComponent);

    ~MTabMounter(void);    
	
/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

	/**
	 * TabMounter에 Data Parameter를 설정한다. 
	 *
	 * @param	datcomponent : 설정할 TabMounter Parameter
	 */
	void SetData(STabMounterData datcomponent);

	/**
	 * TabMounter에 Component List를 설정한다.
	 *
	 * @param	TabMounterCompList : 설정할 TabMounter Component List
	 */
	void AssignComponents(STabMounterRefCompList TabMounterCompList);

	/**
	 * TabMounter에 State 확인할 Component List를 설정한다.
	 *
	 * @param	sCompList : 설정할 State 확인을 위한 Component List
	 */
	void AssignStateCheckComponents(SStateCheckCompListForTabMounter sCompList);

	/**
	 * TabMounter의 Motion Component를 돌려 받는다.
	 */
	IAxis* GetMotionComponent(void);

	IAxis* GetMotionComponentZ();

	// Doolin kwangilshin 2017. 09. 05.
	//
	IAxis* GetMotionComponentGantry();
	// End.
	/**
	 * Model을 변경함을 TabMounter에 알린다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching 관련 제공 Interface                                  */
/************************************************************************/

	/**
	 * TabMounter, Gantry, Backup(mech ver3) 의 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadOffsetPosParameter(int iAxisType);

	/**
	 * TabMounter, Gantry, Backup(mech ver3) 의 축 관련 고정좌표 Data를 File에서 읽어온다.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadFixedPosParameter(int iAxisType);

	/**
	 * TabMounter, Gantry, Backup(mech ver3) 의 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @param	rgdOffsetPos : 저장할 Offset좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveOffsetPosParameter(int iAxisType, const double* const pdOffsetPos);

	/**
	 * TabMounter, Gantry, Backup(mech ver3) 의 축 관련 고정좌표 Data를 File에 저장한다.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @param	dFixedPos : 저장할 고정좌표 배열 (X)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveFixedPosParameter(int iAxisType, const double* const dFixedPos);

	/**
	 * TabMounter, Gantry, Backup(mech ver3) 의 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @param	rgsFixedPos : 고정좌표 배열
	 * @param	rgsOffsetPos : Offset좌표 배열
	 * @param	rgsModelPos : Model좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetPosParameter(int iAxisType, double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);

	/**
     * TabMounter, Gantry, Backup(mech ver3) Teaching 정보를 읽는다.
	 *
	 * @param	int : (0 = TabMounter Axis, 1 = Gantry Axis, 2 = Backup Axis)
	 * @return	STeachingInfo : Teaching 정보
     */
	STeachingInfo GetTeachingInfo(int iAxisType);

	/**
	 *	기구 고정 거리(SystemData) 로 결정되는 Fixed Offset 을 설정한다.
	 */
	void SetFixedOffsetBySystemData();

/************************************************************************/
/*        Cylinder Operation Interface                                  */
/************************************************************************/

	int UpCyl(BOOL bSkipSensor = FALSE);
	int DownCyl(BOOL bSkipSensor = FALSE);

	BOOL IsUpCyl(void);
	BOOL IsDownCyl(void);

	int UpPusherCyl(BOOL bSkipSensor = FALSE);
	int DownPusherCyl(BOOL bSkipSensor = FALSE);

	BOOL IsUpPusherCyl(void);
	BOOL IsDownPusherCyl(void);

/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/
	/**
	 * TabMounter의 Vacuum이 정상인지 확인한다. 정상이면 Success Return.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckVacuum(void);

	/**
	 * TabMounter의 Vacuum을 압축한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AbsorbTabIC(BOOL bSkipSensor = FALSE);

	/**
	 * TabMounter의 Vacuum을 해제한다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ReleaseTabIC(BOOL bSkipSensor = FALSE);

	/**
	 * TabMounter의 Vacuum 압축상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
	BOOL IsAbsorbTabIC(void);

	/**
	 * TabMounter의 Vacuum 해제상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
	BOOL IsReleaseTabIC(void);

/************************************************************************/
/*        TabMounter Move Operation Interface                           */
/************************************************************************/
	/**
	 * TabMounter를 목표위치로 이동시킨다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bWaitOption : 동작 완료를 기다릴 것인지 설정.
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeMovePos(int iScheduleNo, int iPosID, BOOL bWaitOption = TRUE, int iTabNumber = -1, BOOL bAuto = TRUE);

	int SafeMoveZPos(int iPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);
	/**
	 * TabMounter를 목표위치로 이동시킨다.
	 *
	 * @param	dTarget : 목표 위치
	 * @param	iPosID : 이동할 Position Index
	 * @param	bWaitOption : 동작 완료를 기다릴 것인지 설정.
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int MovePos(double dTarget, int iPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

	int MoveZPos(double dTarget, int iPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);


	/**
	 * TabMounter, Gantry 의 이동 동작 완료를 기다린다.
	 */
	int Wait4Done(BOOL bSkipMode = FALSE);

	int Wait4DoneZ(BOOL bSkipMode = FALSE);


	/**
	 * Gantry 를 목표위치로 이동시킨다.
	 *
	 * @param	iGantryPosID : Position Index ID
	 * @param	bWaitOption : 동작 완료를 기다릴 것인지 설정.
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	int SafeMoveGantryPos(int iGantryPosID, int iLoadCarrierID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);
	int SafeMoveGantryPos(int iGantryPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);


	/**
	 * Gantry 를 목표위치로 이동시킨다.
	 *
	 * @param	dGantryTargetPos : 목표 위치
	 * @param	iGantryPosID : 이동할 Position Index
	 * @param	bWaitOption : 동작 완료를 기다릴 것인지 설정.
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int MoveGantryPos(double dGantryTargetPos, int iGantryPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

/************************************************************************/
/*        TabMounter Position 확인 Interface                            */
/************************************************************************/
	
	/**
	 * 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckOrigin(void);

	int CheckOriginZ(void);

	/**
	 * TabMounter 의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabMounter의 현재 위치
	 */
	double GetCurrentPos(BOOL bType = FALSE);

	double GetCurrentPosZ(BOOL bType = FALSE);

	/**
	 * TabMounter 의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabMounter의 목표 위치
	 */
	double GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber = -1);

	double GetTargetPosZ(int iPosID);

	double CalculateMountPos(double dModelPos);
	/**
	 * Gantry 의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : Gantry 의 현재 위치
	 */
	double GetCurrentGantryPos(BOOL bType = FALSE);

	/**
	 * Gantry 의 목표 위치를 읽는다.
	 *
	 * @param	iGantryPosID : Gantry Position Index ID
	 * @return	double : Gantry 의 목표 위치
	 */
//	double GetTargetGantryPos(int iGantryPosID, int iCarrierID);
	double GetTargetGantryPos(int iGantryPosID);

	/**
	 * TabMounter 축의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInPos(int iScheduleNo, int iPosID, double* pdTolerance = NULL);

	BOOL IsInPosZ(int iPosID, double* pdTolerance = NULL);
	/**
	 * TabMounter 축의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInPos(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInPosZ(double dTargetPos, double* pdTolerance = NULL);

	/**
	 * Gantry 축의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iGantryPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
//	BOOL IsInGantryPos(int iGantryPosID, int iLoadCarrierID, double* pdTolerance = NULL);
	BOOL IsInGantryPos(int iGantryPosID, double* pdTolerance = NULL);

	/**
	 * Gantry 축의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dGantryTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInGantryPos(double dGantryTargetPos, double* pdTolerance = NULL);

/************************************************************************/
/*        ETC. Interface					                            */
/************************************************************************/

	/**
	 * TabMounter 가 속한 Group 번호를 반환한다.
	 */
	const int GetTabMounterGroupNo(void) const
	{
		return m_iGroupNo;
	}

	/**
	 * TabMounter 가 작업하는 작업변을 반환한다.
	 */
	const EWorkingSide GetTabMounterWorkSide(void) const
	{
		return m_eWorkSide;
	}

	/**
	 *	Gantry Axis Motion Enable Flag 를 설정한다.
	 */
//	void SetGantryMotionEnable(BOOL bState, int iLoadCarrierID);
	void SetGantryMotionEnable(BOOL bState);

	/**
	 *	Gantry Axis Motion Enable Flag 를 반환한다.
	 */
	BOOL IsGantryMotionEnable(void)
	{
		return m_bGantryMotionEnable;
	}

	/**
	 *	인자로 전달된 축에 대한 Position 개수를 반환한다.
	 */
	const int GetMaxPosNum(int iAxisType) const
	{
		return m_iMaxPos[iAxisType];
	}

	/**
	 *	다른 Mounter 가 현재 Mounter 의 Instance No 를 알 수 있도록 InstanceNo 를 반환한다.
	 */
	const int GetTabMounterNo(void) const
	{
		return m_iInstanceNo;
	}

	/** **************************************************************************
	 *	이전/다음 TabMounter 번호 설정
	 *  ⇒ 이전/다음 구분 기준
	 *		- 자신보다 먼저 Prealign 위치로 이동하는 TabMounter 가 이전 TabMounter 임.
	 *		- Gate 의 경우 TabCarrier Unload 위치에 따라 Front/Rear Group 의 TabMounter 출발 위치가 달라짐.
	 */

	/**
	 *	Pre Tab Mounter 의 TabMounter 번호를 반환한다.
	 */
	int GetPreTabMounterNo(void)
	{
		return m_iPreTabMounterNo;
	}

	/**
	 *	Next Tab Mounter 의 TabMounter 번호를 반환한다.
	 */
	int GetNextTabMounterNo(void)
	{
		return m_iNextTabMounterNo;
	}

	/** **************************************************************************
	 */

	const BOOL IsUpAllTabMounterInGroup();

	int CheckGantryState(int iGantryPosID);

	const BOOL IsInSafePosTabMounterFromAligner(int nToolID);

	const BOOL IsIn_FrontTabLoadPos_TabMounter(int iScheduleNo, int nToolID);
	
	const BOOL IsIn_RearTabLoadPos_TabMounter(int iScheduleNo, int nToolID);	

	BOOL IsInDangerPosWithTabCarrier();

	/**(KKY)
	 * 이동시 인근 Mount와 충돌여부 확인하는 함수
	 **/
	int	CheckCollisionOther(int iScheduleNo, int iPosID, int iTabNumber);
	int CheckCollisionOtherD(double posTarget);
	double GetMovingTgPos();
	BOOL IsMovingY();//Y축이 이동중인지...

	void		SetTabInfo(MTabInfo tabinfo);
	MTabInfo	GetTabInfo();

	int	GetWorkGroup() { return m_iWorkGroup; }
	int	GetWorkInstance() { return m_iWorkInstance; }

	double GetLoadReadyPos();

	//20111205 SJ_HJG
	double GetReceiveAkdata(EWorkingSide eWorkSide, int iTabNo);
//	static double sm_rgdFinalTabOffset[20];
	static double sm_rgdOldAkReceiveData[20];
};

#endif // MTABMOUNTER_H
