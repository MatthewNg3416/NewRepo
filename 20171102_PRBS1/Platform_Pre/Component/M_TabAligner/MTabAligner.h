/* 
 * TabAligner Component
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
 * TabAligner.h : Headerfile of stage component.
 */


#ifndef MALIGNER_H
#define MALIGNER_H

#include "ICommon.h"
#include "DefTabAligner.h"
#include "IStateTabAligner.h"

class MTabAligner : public virtual ICommon, public IStateTabAligner
{
private:   
	/** Axis1 Object */
    IAxis*			m_plnkAxis1;

	/** Axis2 Object */
	IAxis*			m_plnkAxis2;

	/** Expand/Shrink Cycliner Object */
	ICylinder*		m_plnkESCyl;	// Only Use for Mech ver2.0

	/** System Data 관리 Class */
	MSystemData*	m_plnkSystemData;

	/** PositionInfo.inf File 정보 구조체 */
	STeachingInfo	m_sTeachingInfo;

	/** Offset Data가 저장될 File Name */
	CString			m_strOffsetPosFileName;

	/** 파라미터 데이타가 저장될 File Name */
	CString			m_strPosFileName;
	CString			m_strOffsetPosFilePath;

/**********
 * 좌표
 */
	/** TabAligner - 고정좌표  X */
    MPos_XYT		m_rgsFixedPos[DEF_TABALIGNER_MAX_POS];	
	/** TabAligner - 모델좌표 X */
    MPos_XYT		m_rgsModelPos[DEF_TABALIGNER_MAX_POS];	
	/** TabAligner - Offset 좌표 X */
    MPos_XYT		m_rgsOffsetPos[DEF_TABALIGNER_MAX_POS];	

/************************************************************************/
/*        기타
/************************************************************************/
// jdy sesl	char			m_szLogMsg[250];

	/** 이 TabAligner 로 TabIC Prealign 작업을 할 때 사용하는 Camera 번호 */
	int				m_iPrealignCameraNo;

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
private :
	/**
	 * 이동전에 Interlock을 확인한다.
	 *
	 * @param	iPosID : 목표 위치 ID
	 * @return	int (0 = Safety, Error Code = 그 외)
	 */
	int checkInterlock(int iPosID);

	/**
	 * Model 좌표를 계산한다..
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculateModelPosParameter(void);

public:
	/** 
	 * 생성자
	 *
	 * @param commonData : ObjectI나 Log와 같은 Component Common Data
	 * @param listRefComponent : 참조할 Component 포인터 리스트
	 * @param datComponent : TabAligner Data
	 */
	MTabAligner(SCommonAttribute commonData, STabAlignerRefCompList listRefComponents, STabAlignerData datComponent);

    ~MTabAligner(void);
	
/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

	/**
	 * TabAligner 에 Data Parameter 를 설정한다. 
	 *
	 * @param	datcomponent : 설정할 TabAligner Parameter
	 */
	void SetData(STabAlignerData datcomponent);

	/**
	 * TabAligner에 Component List를 설정한다.
	 *
	 * @param	TabAlignerCompList : 설정할 TabAligner Component List
	 */
	void AssignComponents(STabAlignerRefCompList TabAlignerCompList);

	/**
	 * TabAligner에 State 확인할 Component List를 설정한다.
	 *
	 * @param	sCompList : 설정할 State 확인을 위한 Component List
	 */
	void AssignStateCheckComponents(SStateCheckCompListForTabAligner sCompList);

	/**
	 * TabAligner 의 Motion Component를 돌려 받는다.
	 *
	 * @param nAxisID : 축 ID (0: , 1:Y축 , 2:T축, 3:)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	IAxis* GetMotionComponent(int nAxisID);

	/**
	 * Model을 변경함을 TabAligner에 알린다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching 관련 제공 Interface                                  */
/************************************************************************/

	/**
	 * TabAligner 의 X 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadOffsetPosParameter(void);

	/**
	 * TabAligner 의 X 축 관련 Fixed좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int LoadFixedPosParameter(void);

	/**
	 * TabAligner 의 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgdOffsetPos : 저장할 Offset좌표 배열 (Z)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveOffsetPosParameter(MPos_XYT* pOffsetPos);

	/**
	 * TabAligner 의 관련 Fixed좌표 Data를 File에 저장한다.
	 *
	 * @param	rgdOffsetPos : 저장할 Fixed좌표 배열 (Z)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SaveFixedPosParameter(MPos_XYT* pFixedPos);

	/**
	 * TabAligner 의 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	rgsFixedPos : 고정좌표 배열 (Z)
	 * @param	rgsOffsetPos : Offset좌표 배열 (Z)
	 * @param	rgsModelPos : Model좌표 배열 (Z)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int GetPosParameter(MPos_XYT* pFixedPos, MPos_XYT* pOffsetPos, MPos_XYT* pModelPos);

	/**
     * TabAligner Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
     */
	STeachingInfo GetTeachingInfo(void);


/************************************************************************/
/*        TabAligner Move Operation Interface                           */
/************************************************************************/
	/**
	 * TabAligner 를 Index 위치로 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	bWaitOption : 동작 완료를 기다릴 것인지 설정.
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SafeMovePos(int iPosID, BOOL bWaitOption = TRUE);

	/**
	 * TabAligner 를 Index 위치로 이동시킨다.
	 *
	 * @param	dTarget : 목표 위치
	 * @param	iPosID : 이동할 Position Index
	 * @param	bWaitOption : 동작 완료를 기다릴 것인지 설정.
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int MovePos(MPos_XYT posTarget, int iPosID, BOOL bWaitOption = TRUE);

	/**
	 * TabAligner Motion 동작 완료를 기다린다.
	 */
	int Wait4Done();

/************************************************************************/
/*        TabAligner Position 확인 Interface                            */
/************************************************************************/
	
	/**
	 * 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckOrigin(void);

	/**
	 * TabAligner 의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	double : TabAligner 의 현재 위치 
	 */
	MPos_XYT GetCurrentPos(BOOL bType = FALSE);

	/**
	 * TabAligner 의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	double : TabAligner 의 목표 위치 
	 */
	MPos_XYT GetTargetPos(int iPosID);

	/**
	 * TabAligner 의 현재위치와 Index 위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInPos(int iPosID, MPos_XYT* pTolerance = NULL);

	/**
	 * TabAligner 의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pdTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
	BOOL IsInPos(MPos_XYT dTargetPos, MPos_XYT* pTolerance = NULL);

/************************************************************************/
/*        Cylinder														*/
/************************************************************************/

	/**
	 * TabAligner ES Cyl를 Expand 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int Expand(BOOL bSkipSensor = FALSE);

	/**
	 * TabAligner ES Cyl Shrink 시킨다.
	 *
	 * @param	bSkipSensor : (OPTION = FALSE) 실린더 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int Shrink(BOOL bSkipSensor = FALSE);

	/**
	 * TabAligner Expand/Shrink Cylinder 의 Expand 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Expand , FALSE = Shrink)
	 */
	BOOL IsExpand(void);

	/**
	 * TabAligner Expand/Shrink Cylinder 의 Shrink 상태를 확인한다.
	 *
	 * @return	BOOL : (TRUE = Shrink , FALSE = Expand)
	 */
	BOOL IsShrink(void);

/************************************************************************/
/*        기타															*/
/************************************************************************/

	/** 이 TabAligner 로 TabIC Prealign 작업을 할 때 사용하는 Camera 번호를 반환한다.
	 */
	int GetPrealignCamNo(void);
};

#endif // MTabAligner_H
