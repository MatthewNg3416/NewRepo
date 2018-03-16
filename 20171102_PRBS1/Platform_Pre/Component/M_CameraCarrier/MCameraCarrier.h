/* 
 * CameraCarrier Component
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
 * CameraCarrier.h : Headerfile of stage component.
 */
#ifndef MCAMERACARRIER_H
#define MCAMERACARRIER_H

#include "ICommon.h"
#include "DefCameraCarrier.h"
#include "IStateCameraCarrier.h"
#include <afxmt.h>

class MCameraCarrier : public virtual ICommon, public IStateCameraCarrier
{
private:   
    IAxis*			m_plnkAxis;
	IAxis*			m_plnkAxisBackupZ;
	IAxis*			m_plnkAxisExpand;
	ICylinder*		m_plnkESCyl;

	MSystemData*	m_plnkSystemData;
	MPanelData*		m_plnkPanelData;
	MPreBonderData*	m_plnkPreBonderData;
	MProcessData*	m_plnkProcessData;
	MTabData*		m_plnkTabData; //SJ_YYK 150109 Add..
//@	MWorkSchedule*	m_plnkWorkSchedule;
	MWorkSchedule*	m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];

	/** PositionInfo.inf File 정보 구조체 */
	STeachingInfo	m_sTeachingInfo;
	STeachingInfo	m_sTeachingInfoBackupZ;
	STeachingInfo	m_sTeachingInfoExpand;

	/** Offset Data가 저장될 File Name */
	CString			m_strOffsetPosFileName;

	/** 파라미터 데이타가 저장될 File Name */
	CString			m_strPosFileName;
	CString			m_strOffsetPosFilePath;

	/** 동작 전 상태 확인해야 하는 인접 Camera Carrier 에 대한 link
	 *	- 할당 기준 : 해당 그룹에 속하는 TabMounter 설정 상태와 일치시킴.
	 */
	IStateCameraCarrier*	m_plnkPreCamCarrier;
	IStateCameraCarrier*	m_plnkNextCamCarrier;
//	IStateTabMounter*		m_plnkTabMounter;
	IStateTabMounter*		m_plnkTabMounter[DEF_MAX_TABMOUNTER];
	IStatePanelAligner*		m_plnkPanelAligner;

/**********
 * 좌표
 */
	/** CameraCarrier - 고정좌표 */
    double			m_rgdFixedPos[DEF_CAMERACARRIER_MAX_POS];
	/** CameraCarrier - 모델좌표 */
//@    double			m_rgdModelPos[DEF_CAMERACARRIER_MAX_POS];
    double			m_rgdModelPos[DEF_MAX_WORK_SCHEDULE][DEF_CAMERACARRIER_MAX_POS];
	/** CameraCarrier - Offset 좌표 */
    double			m_rgdOffsetPos[DEF_CAMERACARRIER_MAX_POS];

	/** CameraCarrier - 고정좌표 */
    double			m_rgdFixedPosBackupZ[DEF_CAMERACARRIER_BACKUP_MAX_POS];
	/** CameraCarrier - 모델좌표 */
    double			m_rgdModelPosBackupZ[DEF_CAMERACARRIER_BACKUP_MAX_POS];
	/** CameraCarrier - Offset 좌표 */
    double			m_rgdOffsetPosBackupZ[DEF_CAMERACARRIER_BACKUP_MAX_POS];

	/** CameraCarrier ExpandY- 고정좌표 */
    double			m_rgdFixedPosExpand[DEF_CAMERACARRIER_EXPAND_MAX_POS];
	/** CameraCarrier ExpandY- 모델좌표 */
    double			m_rgdModelPosExpand[DEF_CAMERACARRIER_EXPAND_MAX_POS];
	/** CameraCarrier ExpandY- Offset 좌표 */
    double			m_rgdOffsetPosExpand[DEF_CAMERACARRIER_EXPAND_MAX_POS];

/**********
 * 기타
 */
	/** CameraCarrier 가 속한 Group 번호 */
	int				m_iGroupNo;
	int				m_iWorkInstance;

	/** 작업변 정보 */
	EWorkingSide	m_eWorkSide;

	/** 소유하고 있는 Camera 번호 */
//	int				m_iCamNo;
	int				m_iCamNo1;
	int				m_iCamNo2;

	BOOL			m_isPanelAlignUnit;

	/**(KKY)
	 * 이동시 인근 Carrier와 충돌여부 확인하는데 사용
	 **/
	double						m_posTargetMove;
	BOOL						m_bMoving;
	static CCriticalSection		m_csMoveLock;
/************************************************************************/
/*
/* Private Operation
/*
/************************************************************************/ 
private :

	//KKY
	void					SetMovingY(BOOL bMove, double posTarget);

	/**
	 * 축 이동전에 Interlock을 확인한다.
	 *
	 * @param	dTarget : 목표 위치
	 * @return	int (0 = Safety, Error Code = 그 외)
	 */
	int checkInterlock(double dTarget);

	int checkInterlockForBackup(int iPosID, double dTarget, BOOL bAuto = FALSE);

	/**
	 * Model 좌표를 계산한다.
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculateModelPosParameter(void);

	int calculateExpandModelPosParameter(void);

public:
	/** 
	 * 생성자
	 *
	 * @param commonData : ObjectI나 Log와 같은 Component Common Data
	 * @param listRefComponent : 참조할 Component 포인터 리스트
	 * @param datComponent : CameraCarrier Data
	 */
	MCameraCarrier(SCommonAttribute commonData, SCameraCarrierRefCompList listRefComponents, SCameraCarrierData datComponent);

    ~MCameraCarrier(void);    
	
/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

	/**
	 * CameraCarrier에 Data Parameter를 설정한다. 
	 *
	 * @param	datcomponent : 설정할 CameraCarrier Parameter
	 */
	void SetData(SCameraCarrierData datcomponent);

	/**
	 * CameraCarrier에 Component List를 설정한다.
	 *
	 * @param	CameraCarrierCompList : 설정할 CameraCarrier Component List
	 */
	void AssignComponents(SCameraCarrierRefCompList CameraCarrierCompList);

	/**
	 * CameraCarrier에 State 확인할 Component List를 설정한다.
	 *
	 * @param	sCompList : 설정할 State 확인을 위한 Component List
	 */
	void AssignStateCheckComponents(SStateCheckCompListForCameraCarrier sCompList);

	/**
	 * CameraCarrier의 Motion Component를 돌려 받는다.
	 */
	IAxis* GetMotionComponent(void);

	IAxis* GetMotionComponentBackup(void);

	IAxis* GetMotionComponentExpand();

	/**
	 * Model을 변경함을 CameraCarrier에 알린다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching 관련 제공 Interface                                  */
/************************************************************************/

	int LoadFixedPosParameter(void);

	int LoadFixedPosParameterBackup(void);

	int LoadFixedPosParameterExpand(void);

	/**
	 * CameraCarrier의 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadOffsetPosParameter(void);
	
	int LoadOffsetPosParameterBackup(void);

	int LoadOffsetPosParameterExpand(void);

	int SaveFixPosParameter(double* pdFixPos);

	int SaveFixedPosParameterBackup(double* dFixedPos);

	int SaveFixedPosParameterExpand(double* dFixedPos);

	/**
	 * CameraCarrier의 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgdOffsetPos : 저장할 Offset좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveOffsetPosParameter(double* pdOffsetPos);

	int SaveOffsetPosParameterBackup(double* pdOffsetPos);

	int SaveOffsetPosParameterExpand(double* pdOffsetPos);

	/**
	 * CameraCarrier의 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	rgsFixedPos : 고정좌표 배열
	 * @param	rgsOffsetPos : Offset좌표 배열
	 * @param	rgsModelPos : Model좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int GetPosParameter(double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);
	int GetPosParameter(int iScheduleNo, double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);

	int GetPosParameterBackup(double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);

	int GetPosParameterExpand(double* pdFixedPos, double* pdOffsetPos, double* pdModelPos);

	/**
     * CameraCarrier Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
     */
	STeachingInfo GetTeachingInfo(void);

	STeachingInfo GetTeachingInfoBackup(void);

	STeachingInfo GetTeachingInfoExpand(void);

	/**
	 *	기구 고정 거리(SystemData) 로 결정되는 Fixed Offset 을 설정한다.
	 */
	void SetFixedOffsetBySystemData();

/************************************************************************/
/*        CameraCarrier Move Operation Interface                        */
/************************************************************************/
	/**
	 * CameraCarrier 를 목표 위치로 이동시킨다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bWaitOption : 동작 완료를 기다릴 것인지 설정.
	 * @param	iTabNumber : 작업 Tab No.
	 * @param	bIgnoreInterlock : Check Interlock 여부.
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//@	int SafeMovePos(int iPosID, BOOL bWaitOption = TRUE, int iTabNumber = -1, BOOL bIgnoreInterlock = FALSE);
	int SafeMovePos(int iScheduleNo, int iPosID, BOOL bWaitOption = TRUE, int iTabNumber = -1, BOOL bIgnoreInterlock = FALSE);

	int SafeMoveBackupPos(int iBackupPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

	int SafeMoveExpand(int iExpandPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

	/**
	 * CameraCarrier 이동 동작 완료 대기
	 */
	int Wait4Done(BOOL bSkipMode = FALSE);

	int Wait4DoneBackup(BOOL bSkipMode = FALSE);

	int Wait4DoneExpand(BOOL bSkipMode = FALSE);

	/**
	 * CameraCarrier 를 목표 위치로 이동시킨다.
	 *
	 * @param	dTarget : 목표 위치
	 * @param	iPosID : 이동할 Position Index
	 * @param	bWaitOption : 동작 완료를 기다릴 것인지 설정.
	 * @param	bIgnoreInterlock : Check Interlock 여부.
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int MovePos(double dTarget, int iPosID, BOOL bWaitOption = TRUE, BOOL bIgnoreInterlock = FALSE);

	int MoveBackupPos(double dBackupTargetPos, int iBackupPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

	int MoveExpandPos(double dExpandTargetPos, int iExpandPosID, BOOL bWaitOption = TRUE, BOOL bAuto = TRUE);

/************************************************************************/
/*        CameraCarrier Position 확인 Interface                         */
/************************************************************************/
	
	/**
	 * 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckOrigin(void);

	int CheckOriginBackup(void);

	int CheckOriginExpand(void);

	/**
	 * CameraCarrier 의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : CameraCarrier의 현재 위치 
	 */
	double GetCurrentPos(BOOL bType = FALSE);

	double GetCurrentPosBackup(BOOL bType = FALSE);

	double GetCurrentPosExpand(BOOL bType = FALSE);

	/**
	 * CameraCarrier 의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : CameraCarrier의 목표 위치 
	 */
//@	double GetTargetPos(int iPosID, int iTabNumber = -1);
	double GetTargetPos(int iScheduleNo, int iPosID, int iTabNumber = -1);

	double GetTargetPosBackup(int iPosID, int iTabNumber = -1);

	double GetTargetPosExpand(int iPosID, int iTabNumber = -1);

//@	double CalculateInspectPos(double dInspectModelPos);
	double CalculateInspectPos(int iScheduleNo, double dInspectModelPos);
	double CalculateMountPos(int iScheduleNo, double dInspectModelPos);  //170826 JSh.s

	/**
	 * CameraCarrier 의 현재위치와 Index 위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
//@	BOOL IsInPos(int iPosID, double* pdTolerance = NULL);
	BOOL IsInPos(int iScheduleNo, int iPosID, double* pdTolerance = NULL);

	BOOL IsInPosBackup(int iPosID, double* pdTolerance = NULL);

	BOOL IsInPosExpand(int iPosID, double* pdTolerance = NULL);

	/**
	 * CameraCarrier 의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInPos(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInPosBackup(double dTargetPos, double* pdTolerance = NULL);

	BOOL IsInPosExpand(double dTargetPos, double* pdTolerance = NULL);

/************************************************************************/
/*        Cylinder														*/
/************************************************************************/

	int ExpandShrinkForTabAlign(BOOL bSkipSensor = FALSE);
	int ExpandShrinkForPanelAlign(BOOL bSkipSensor = FALSE);

	/**
	 * CameraCarrier ES Cyl를 Expand 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int Expand(BOOL bSkipSensor = FALSE);

	/**
	 * CameraCarrier ES Cyl Shrink 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int Shrink(BOOL bSkipSensor = FALSE);

	/**
	 * CameraCarrier Expand/Shrink Cylinder 의 Expand 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * CameraCarrier Expand/Shrink Cylinder 의 Shrink 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

/************************************************************************/
/*        ETC. Interface					                            */
/************************************************************************/

	/**
	 * CameraCarrier 가 속한 Group 번호를 반환한다.
	 */
	const int GetCameraCarrierGroupNo(void) const
	{
		return m_iGroupNo;
	}

	/**
	 * CameraCarrier 가 작업하는 작업변을 반환한다.
	 */
	const EWorkingSide GetCameraCarrierWorkSide(void) const
	{
		return m_eWorkSide;
	}

	/**
	 * CameraCarrier 가 소유하고 있는 Camera 번호를 반환한다.
	 */
//	const int GetOwnedCameraNo(void) const
//	{
//		return m_iCamNo;
//	}
	const int GetOwnedCameraNo1(void) const
	{
		return m_iCamNo1;
	}
	const int GetOwnedCameraNo2(void) const
	{
		return m_iCamNo2;
	}

	/**
	 * Pre Camera Carrier 의 Inspect Camera 번호를 반환한다.
	 */
	const int GetPreCamCarrierNo(void);

	/**
	 * Next Camera Carrier 의 Inspect Camera 번호를 반환한다.
	 */
	const int GetNextCamCarrierNo(void);

	/**
	 *	현재 Camera Carrier 가 Inspection Camera Carrier 일 경우,
	 *	다른 Camera Carrier 가 참조 가능하도록
	 *	현재 Camera Carrier 에 해당하는 Inspection Camera Carrier 번호를 반환한다.
	 */
	const int GetInspectCamCarrierNo(void) const ;

	/**(KKY)
	 * 이동시 인근 Carrier와 충돌여부 확인하는 함수
	 **/
	int	CheckCollisionOther(int iScheduleNo, int iPosID, int iTabNumber);
	int CheckCollisionOtherD(double posTarget);
	double GetMovingTgPos();
	BOOL IsMovingY();//Y축이 이동중인지...

	int GetCameraNo(int iCameraId);

	BOOL IsPanelAlignUnit() { return m_isPanelAlignUnit; }
};

#endif // MCAMERACARRIER_H
