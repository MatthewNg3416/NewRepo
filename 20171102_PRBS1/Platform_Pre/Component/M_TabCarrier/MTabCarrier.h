/* 
 * TabCarrier Component
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
 * TabCarrier.h : Headerfile of stage component.
 */


#ifndef MTABCARRIER_H
#define MTABCARRIER_H

#include "DefTabCarrier.h"
#include "ICommon.h"
#include "IStateTabCarrier.h"
#include <afxmt.h>
#include "MProcessData.h"

class MTabCarrier : public virtual ICommon, public IStateTabCarrier
{
private:   
/************************************************************************/
/*                             Vacuum                                   */
/************************************************************************/
	/** TabCarrier Vacuum Object */
//	IVacuum*	m_plnkVac[DEF_MAX_WORKER_PER_GROUP];
	IVacuum*	m_plnkVac;
	
/************************************************************************/
/*                             Cylinder                                 */
/************************************************************************/
	/** Up/Down Cylinder Object */
    ICylinder*	m_plnkUDCyl;

	ICylinder*	m_plnkUDCyl2; //SJ_YYK 150817 Add...

	ICylinder*	m_plnkFBCyl;

	ICylinder*	m_plnkCamESCyl;

	ICylinder*	m_plnkDumpBasketFBCyl;

/************************************************************************/
/*                             Motion                                   */
/************************************************************************/
	/** TabCarrier X Motion Object */
	IAxis*		m_plnkX;

	/** TabCarrier Y Motion Object */
	IAxis*		m_plnkY;

	/** TabCarrier T Motion Object */
	IAxis*		m_plnkT;

	IAxis*		m_plnkExpandAxis;

	IAxis*		m_plnkZAxis;

	ISercosMultiAxis*	m_plnkXZAxis; //SJ_YYK 150212 Add..

/************************************************************************/
/*                             State Interface                          */
/************************************************************************/
	/** TabFeeder State Object */
	IStateTabFeeder*	m_plnkStateTabFeeder;

	/** TabMounter State Object */
	IStateTabMounter*	m_plnkStateTabMounter[DEF_MAX_WORKER_PER_GROUP];

	IStateTabCarrier*	m_plnkStateTabCarrier[DEF_MAX_TABCARRIER];

/************************************************************************/
/*                             Data                                     */
/************************************************************************/
	/** System Data Object */
	MSystemData*	m_plnkSystemData;
	MPreBonderData*	m_plnkPreBonderData;
	MTabOffsetData*	m_plnkTabOffsetData;


	/** 소유하고 있는 Camera 번호 */
	int				m_iCamNo1;
	int				m_iCamNo2;

	// Work Group
	int				m_iWorkInstance;
	int				m_iWorkGroup;

	/** **************************************************************************
	 *	이전/다음 TabCarrier 번호 설정
	 *  ⇒ 이전/다음 구분 기준
	 *		- 자신보다 먼저 작업 위치로 이동하는 TabMounter 가 이전 TabMounter 임.
	 */

	/** 이전 TabMounter 번호 : 1번 TabMounter 의 [이전 TabMounter 번호]는 -1 이다.
	 *	: TabMounter 가 Mount 위치로 이동하기 전에, Previous TabMounter 의 위치를 확인해야 한다.
	 */
	int				m_iPreTabCarrierNo;

	/** 다음 TabMounter 번호
	 *	: 각 Group 의 마지막 TabMounter 의 [다음 TabMounter 번호]는 DEF_LAST_PER_GROUP 이다.
	 *	: TabMounter 가 Load 위치로 이동하기 전에, Next TabMounter 의 위치를 확인해야 한다.
	 */
	int				m_iNextTabCarrierNo;

/************************************************************************/
/*                             IO Address                               */
/************************************************************************/


/************************************************************************/
/*                             Position                                 */
/************************************************************************/
	CString m_strPosFileName;
	CString m_strOffsetPosFileName;
	CString m_strOffsetPosFilePath;

	/** TabCarrier X,Y 축 Position 정보 */
    STeachingInfo m_sXYTTeachingInfo;

	STeachingInfo m_sExpandTeachingInfo;

	STeachingInfo m_sZTeachingInfo; //SJ_YYK 150109 Add...

	/** TabCarrier X,Y축 고정좌표계 */
	MPos_XYT	m_rgsXYTFixedPos[DEF_TABCARRIER_MAX_POSITION];

	/** TabCarrier X,Y축 Offset 좌표계 */
	MPos_XYT	m_rgsXYTOffsetPos[DEF_TABCARRIER_MAX_POSITION];

	/** TabCarrier X,Y축 모델좌표계 */
	MPos_XYT	m_rgsXYTModelPos[DEF_TABCARRIER_MAX_POSITION];

	/** ExpandY축 고정좌표계 */
	double	m_rgsExpandFixedPos[DEF_TABCARRIER_EXPAND_MAX_POS];

	/** ExpandY축 Offset 좌표계 */
	double	m_rgsExpandOffsetPos[DEF_TABCARRIER_EXPAND_MAX_POS];

	/** ExpandY축 모델좌표계 */
	double	m_rgsExpandModelPos[DEF_TABCARRIER_EXPAND_MAX_POS];
	
	//SJ_YYK 150109 Add...
	double m_rgsZFixedPos[DEF_TABCARRIER_Z_MAX_POS];

	double m_rgsZModelPos[DEF_TABCARRIER_Z_MAX_POS];

	double m_rgsZOffsetPos[DEF_TABCARRIER_Z_MAX_POS];


/************************************************************************/
/*                             Align                                    */
/************************************************************************/
	/** PreAlign Mark Offset*/
	MPos_XYT	m_rgAlignMarkOffset;

	/** Align 수행 여부 */
	BOOL		m_bMarkAligned;

	/** Align하기 전의 T 축 값 */
	double		m_dValueThetaAlign;
	

/************************************************************************/
/*                             기타 메시지                              */
/************************************************************************/
	/** Log Message	*/
// jdy sesl	char	m_szLogMsg[250];

/************************************************************************/
/*                             Flag									    */
/************************************************************************/
	BOOL	m_bJogStats;
	
	/**(KKY)
	 * 이동시 인근 Carrier와 충돌여부 확인하는데 사용
	 **/
	MPos_XYT					m_posTargetMove;
	BOOL						m_bMoving;
	static CCriticalSection		m_csMoveLock;

	MTabInfo					m_TabInfo;
/************************************************************************/
/*                             ETC									    */
/************************************************************************/

/************************************************************************/
/*      TabCarrier Component 내부에서만 사용할 Operation                */
/************************************************************************/

	//KKY
//@	void					SetMovingY(BOOL bMove, MPos_XYT posTarget);

	/**
	 * TabCarrier 이동전에 Interlock을 확인한다.
	 *
	 * @param	iPosID : 목표 위치 ID
	 * @return	int (0 = Safety, Error Code = 그 외)
	 */
//	int checkInterlockForXY(MPos_XY posTarget, int iPosID);
	int checkInterlockForXYT(MPos_XYT posTarget, int iPosID, BOOL bAuto = FALSE);

	// TabCarrier Up 동작 Interlock Check.
	int checkInterlockForUp();

	int checkInterlockForDumpBasketFor();
	/**
	 * TabCarrier X,Y축 Model 좌표를 계산한다..
	 *
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
	int calculateXYTModelPosParameter();

	int calculateExpandPosParameter();

	int checkInterlockTabMounter4MoveUnload(int iPosID);

public:
	MTabCarrier();

	/** 
	 * 생성자
	 *
	 * @param commonData : ObjectI나 Log와 같은 Component Common Data
	 * @param listRefComponent : 참조할 Component 포인터 리스트
	 * @param datComponent : TabCarrier Data
	 */
	MTabCarrier( SCommonAttribute commonData, STabCarrierRefCompList listRefComponents, STabCarrierData datComponent );

    ~MTabCarrier();    

/************************************************************************/
/*        Component 공통 제공 Interface                                 */
/************************************************************************/

	/**
	 * TabCarrier에 Data Parameter를 설정한다. 
	 *
	 * @param	datcomponent : 설정할 TabCarrier Parameter
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int SetData(STabCarrierData datcomponent);

	/**
	 * TabCarrier에 Component List를 설정한다.
	 *
	 * @param	TabCarrierCompList : 설정할 TabCarrier Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignComponents(STabCarrierRefCompList TabCarrierCompList);

	/**
	 * TabCarrier에 State 확인할 Component List를 설정한다.
	 *
	 * @param	sCompList : 설정할 State 확인을 위한 Component List
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int AssignStateCheckComponents(SStateCheckCompListForTabCarrier sCompList);

	/**
	 * TabCarrier의 Motion Component를 돌려 받는다.
	 *
	 * @param iAxisID : 축 ID (0: X축, 1:Y축)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	IAxis* GetMotionComponent(int iAxisID);

	IAxis* GetExpandMotionComponent();

	IAxis* GetZMotionComponent(); //SJ_YYK 150109 Add...

	/**
	 * Model을 변경함을 TabCarrier에 알린다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int ChangeModel(void);

/************************************************************************/
/*        Teaching 관련 제공 Interface                                  */
/************************************************************************/
	/**
	 * TabCarrier의 X,Y 축 관련 고정좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	//int LoadXYFixedPosParameter(void);
	int LoadXYTFixedPosParameter(void);

	int LoadExpandFixedPosParameter(void);

	int LoadZFixedPosParameter(void); //SJ_YYK 150109 Add...

	/**
	 * TabCarrier의 X,Y 축 관련 Offset좌표 Data를 File에서 읽어온다.
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	int LoadXYOffsetPosParameter(void);
	int LoadXYTOffsetPosParameter(void);

	int LoadExpandOffsetPosParameter(void);

	int LoadZOffsetPosParameter(void); //SJ_YYK 150109 Add...

	/**
	 * TabCarrier의 X,Y 축 관련 고정좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsFixedPos : 저장할 고정좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	int SaveXYFixedPosParameter(MPos_XY* rgsFixedPos);
	int SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos);

	int SaveExpandFixedPosParameter(double* rgsFixedPos);

	int SaveZFixedPosParamter(double* rgsFixedPos); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier의 X,Y 축 관련 Offset좌표 Data를 File에 저장한다.
	 *
	 * @param	rgsOffsetPos : 저장할 Offset좌표 배열
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	int SaveXYOffsetPosParameter(MPos_XY* rgsOffsetPos);
	int SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos);

	int SaveExpandOffsetPosParameter(double* rgsOffsetPos);

	int SaveZOffsetPosParameter(double* rgsOffsetPos); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier의 X,Y 축 좌표 관련 모든 Data를 반환 한다.
	 *
	 * @param	rgsFixedPos : 고정좌표
	 * @param	rgsOffsetPos : Offset좌표
	 * @param	rgsModelPos : Model좌표
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	int GetXYPosParameter(MPos_XY* rgsFixedPos, MPos_XY* rgsOffsetPos, MPos_XY* rgsModelPos);
	int GetXYTPosParameter(MPos_XYT* rgsFixedPos, MPos_XYT* rgsOffsetPos, MPos_XYT* rgsModelPos);

	int GetExpandPosParameter(double* rgsFixedPos, double* rgsOffsetPos, double* rgsModelPos);

	int GetZPosParameter(double* rgsFixedPos, double* rgsOffsetPos, double* rgsModelPos); //SJ_YYK 150109 Add..

	/**
     * TabCarrier X,Y Teaching 정보를 읽는다.
	 *
	 * @return	STeachingInfo : Teaching 정보
     */
//	STeachingInfo GetXYTeachingInfo();
	STeachingInfo GetXYTTeachingInfo();

	STeachingInfo GetExpandTeachingInfo();

	STeachingInfo GetZTeachingInfo(); //SJ_YYK 150109 Add...

/************************************************************************/
/*        Vacuum Operation Interface                                    */
/************************************************************************/
	/**
	 * TabCarrier의 Vacuum이 정상인지 확인한다. 정상이면 Success Return.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	int CheckVacuum(int iVacID);
	int CheckVacuum();

	/**
	 * TabCarrier의 Vacuum을 압축한다.
	 *
	 * @param	iVacID : Vacuum ID
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	int AbsorbTabIC(int iVacID, BOOL bSkipSensor = FALSE);
	int AbsorbTabIC(BOOL bSkipSensor = FALSE);
	
	/**
	 * TabCarrier의 Vacuum을 해제한다.
	 *
	 * @param	iVacID : Vacuum ID
	 * @param	bSkipSensor : (OPTION = FALSE) Vacuum 동작 중 센서 체크 여부 (FALSE=확인, TRUE=무시)
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
//	int ReleaseTabIC(int iVacID, BOOL bSkipSensor = FALSE);
	int ReleaseTabIC(BOOL bSkipSensor = FALSE);

	/**
	 * TabCarrier의 Vacuum 압축상태를 확인한다.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	BOOL : (TRUE = 압축 상태, FALSE = 해제 상태)
	 */
//	BOOL IsAbsorbTabIC(int iVacID);
	BOOL IsAbsorbTabIC();

	/**
	 * TabCarrier의 Vacuum 해제상태를 확인한다.
	 *
	 * @param	iVacID : Vacuum ID
	 * @return	BOOL : (TRUE = 해제 상태, FALSE = 압축 상태)
	 */
//	BOOL IsReleaseTabIC(int iVacID);
	BOOL IsReleaseTabIC();

/************************************************************************/
/*        Cylinder Operation Interface                                    */
/************************************************************************/
    /**
     * TabCarrier를 상승시킨다.
	 *
     * @param	BOOL bSkipSensor (동작 후 Sensor 확인을 skip 할 것인지를 결정, default = FALSE)
     * @return	int (0 = Success, Error Code = 그 외) 
     */
	int Up(BOOL bSkipSensor = FALSE);

    /**
     * TabCarrier를 하강시킨다.
	 *
     * @param	BOOL bSkipSensor (동작 후 Sensor 확인을 skip 할 것인지를 결정, default = FALSE)
     * @return	int (0 = Success, Error Code = 그 외) 
     */
	int Down(BOOL bSkipSensor = FALSE);

	/**
     * TabCarrier를 상승시킨다.
	 *
     * @param	BOOL bSkipSensor (동작 후 Sensor 확인을 skip 할 것인지를 결정, default = FALSE)
     * @return	int (0 = Success, Error Code = 그 외) 
     */
	int Up2(BOOL bSkipSensor = FALSE);

    /**
     * TabCarrier를 하강시킨다.
	 *
     * @param	BOOL bSkipSensor (동작 후 Sensor 확인을 skip 할 것인지를 결정, default = FALSE)
     * @return	int (0 = Success, Error Code = 그 외) 
     */
	int Down2(BOOL bSkipSensor = FALSE);

	/**
     * TabCarrier를 전진시킨다.
	 *
     * @param	BOOL bSkipSensor (동작 후 Sensor 확인을 skip 할 것인지를 결정, default = FALSE)
     * @return	int (0 = Success, Error Code = 그 외) 
     */
	int Forward(BOOL bSkipSensor = FALSE);

	/**
     * TabCarrier를 후진시킨다.
	 *
     * @param	BOOL bSkipSensor (동작 후 Sensor 확인을 skip 할 것인지를 결정, default = FALSE)
     * @return	int (0 = Success, Error Code = 그 외) 
     */
	int Backward(BOOL bSkipSensor = FALSE);

	/**
	 * TabCarrier가 상승된 상태인지 확인한다.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Up, FALSE = 그 외)
	 */
	BOOL IsUp();

	/**
	 * TabCarrier가 하강된 상태인지 확인한다.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Down, FALSE = 그 외)
	 */
	BOOL IsDown();

		/**
	 * TabCarrier가 상승된 상태인지 확인한다.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Up, FALSE = 그 외)
	 */
	BOOL IsUp2();

	/**
	 * TabCarrier가 하강된 상태인지 확인한다.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Down, FALSE = 그 외)
	 */
	BOOL IsDown2();

	/**
	 * TabCarrier가 전진된 상태인지 확인한다.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Up, FALSE = 그 외)
	 */
	BOOL IsForward();

	/**
	 * TabCarrier가 후진된 상태인지 확인한다.
	 *
	 * @param	void
	 * @return	BOOL (TRUE = Down, FALSE = 그 외)
	 */
	BOOL IsBackward();


	int ExpandShrinkForPreAlign(BOOL bSkipSensor = FALSE);
	int ShrinkCamera(BOOL bSkipSensor = FALSE);
	int ExpandCamera(BOOL bSkipSensor = FALSE);
	BOOL IsShrinkCamera();
	BOOL IsExpandCamera();

	int ForwardDumpBasket(BOOL bSkipSensor = FALSE);
	int BackwardDumpBasket(BOOL bSkipSensor = FALSE);
	BOOL IsForwardDumpBasket();
	BOOL IsBackwardDumpBasket();
/************************************************************************/
/*        IO Operation Interface                                    */
/************************************************************************/


/************************************************************************/
/*        TabCarrier Move Operation Interface                                    */
/************************************************************************/
	/**
	 * TabCarrier를 Index위치로 이동시킨다. Interlock을 확인한다.
	 *
	 * @param	iPosID		: Position Index ID
	 * @return	int			: (0 = Success, Error Code = 그 외)
	 */
//	int SafeMoveXYPos(int iPosID);
	int SafeMoveXYTPos(int iPosID, BOOL bAuto = TRUE);	

	int SafeMoveExpandPos(int iPosID, BOOL bWaitOpt=TRUE, BOOL bAuto = TRUE);

	int SafeMoveZPos(int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier를 Index위치로 이동시킨다.
	 *
	 * @param	dTarget : 목표 위치
	 * @param	iPosID : 이동할 Position Index
	 * @return	int (0 = Success, Error Code = 그 외)
	 */
//	int MoveXYPos(MPos_XY posTarget, int iPosID, BOOL bWaitOpt = TRUE);
	int MoveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE);

	int MoveYTPos(int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE); //SJ_YYK 150223 Add...

	int MoveXPos(double posTarget);
	int MoveYPos(double posTarget);
	int MoveTPos(double posTarget);

	int MoveExpandPos(double posTarget, int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE);

	int MoveZPos(double posTarget, int iPosID, BOOL bWaitOpt = TRUE, BOOL bAuto = TRUE); //SJ_YYK 150109 Add..

/************************************************************************/
/*        TabCarrier Position확인 Interface                                    */
/************************************************************************/
	/**
	 * 원점복귀 여부를 확인한다. 모두 원점복귀가 되어 있으면 Success Return, 아니면 안되어 있는 축 Error Code Return
	 *
	 * @return	int : (0 = Success, Error Code = 그 외)
	 */
	int CheckOrigin(void);
	int CheckOriginX(void);
	int CheckOriginY(void);
	int CheckOriginExpand();
	int CheckOriginZ(); //SJ_YYK 150109 Add...

	/**
	 * TabCarrier의 현재 위치를 읽는다.
	 *
	 * @param	bType : (OPTION=FALSE) Motion의 위치 종류 (FALSE=Encoder값, TRUE=Command값)
	 * @return	MPos_XY : TabCarrier의 현재 위치
	 */
//	MPos_XY GetCurrentXYPos(BOOL bType = FALSE);
	MPos_XYT GetCurrentXYTPos(BOOL bType = FALSE);

	double	GetCurrentExpandPos(BOOL bType = FALSE);

	double GetCurrentZPos(BOOL bType = FALSE); //SJ_YYK 150105 Add.

	/**
	 * TabCarrier의 목표 위치를 읽는다.
	 *
	 * @param	iPosID : Position Index ID
	 * @return	MPos_XY : TabCarrier의 목표 위치
	 */
//	MPos_XY GetTargetXYPos(int iPosID);
	MPos_XYT GetTargetXYTPos(int iPosID);

	double	GetTargetExpandPos(int iPosID);

	double GetTargetZPos(int iPosID); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier X,Y의 현재위치와 Index위치를 비교한다.
	 *
	 * @param	iPosID : Position Index ID
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
//	BOOL IsInXYPos(int iPosID, MPos_XY* pTolerance = NULL);
	BOOL IsInXYTPos(int iPosID, MPos_XYT* pTolerance = NULL);
	BOOL IsInXPos(int iPosID, double* pTolerance = NULL);
	BOOL IsInYPos(int iPosID, double* pTolerance = NULL);
	BOOL IsInTPos(int iPosID, double* pTolerance = NULL);

	BOOL IsInExpandPos(int iPosID, double* pTolerance = NULL);

	BOOL IsInZPos(int iPosID, double* pTolerance = NULL); //SJ_YYK 150109 Add..

	/**
	 * TabCarrier X,Y의 현재위치와 전달위치를 비교한다.
	 *
	 * @param	dTargetPos : 현재위치와 비교할 전달위치
	 * @param	pTolerance : (OPTION=NULL) 비교 시 사용할 허용오차 (NULL이면 Motion의 Tolerance 사용)
	 * @return	BOOL : (TRUE = 일치, FALSE = 불일치)
	 */
//	BOOL IsInXYPos(MPos_XY sTargetPos, MPos_XY* pTolerance = NULL);
	BOOL IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance = NULL);
	BOOL IsInXPos(double dTargetPos, double* pTolerance = NULL);
	BOOL IsInYPos(double dTargetPos, double* pTolerance = NULL);
	BOOL IsInTPos(double dTargetPos, double* pTolerance = NULL);

	BOOL IsInExpandPos(double dTargetPos, double* pTolerance = NULL);

	BOOL IsInZPos(double dTargetPos, double* pTolerance = NULL); //SJ_YYK 150109 Add..

	//KKY
	//선택한 축의 현재 위치와 ComparePosID의 위치를 비교한다.
	BOOL CompareCurrentPos(int CarrierAxisID, int ComparePosID, ECompareType CompareType);
	//선택한 축의 현재 위치와 dComparePos의 위치를 비교한다.
	BOOL CompareCurrentPos(int CarrierAxisID, double dComparePos, ECompareType CompareType);
	//선택한 축의 목표 위치와 ComparePosID의 위치를 비교한다.
	BOOL CompareTargetPos(int CarrierAxisID, int TargetPosID, int ComparePosID, ECompareType CompareType);


	int VMoveVelocity(UINT nSel, BOOL dir);

	int VMovePitch(UINT nSel, BOOL dir);
	
	int VMoveEnd();

/************************************************************************/
/*        Align Operation Interface										*/
/************************************************************************/

	/**
	 * Align 결과인 Align Mark Offset을 초기화한다.
	 * Align되었다는 결과 Flag인 m_bMarkAligned를 FALSE로 설정하고
	 * Align Mark Offset을 "0.0"으로 설정한다.
	 * Thread에서 PanelAligner를 진행할 때 Loading Position으로 이동하면서 사용한다. 
	 */
	void InitAlignMarkOffset(void);

	/**
	 * 기록된 Align 결과인 Align Mark Offset을 읽는다.
	 * Align 결과가 설정되어 있으면 SUCCESS를 return하고 Align Mark Offset을 전달한다.
	 * Align 결과가 설정되어 있지 않으면 Error를 return하고 *psPos에는 0.0을 설정한다. 
	 *
	 * @param	*posxytTarget: 목표 위치 값
	 * @return	MPos_XYT	 : 설정되어 있는 Align Offset 좌표 값
	 */
	MPos_XYT CalculateAlignMarkOffset(MPos_XYT posxytTarget);
	
	/**
	 * Align Mark Offset을 설정한다.
	 * m_bMarkAligned을 TRUE로 설정한다.
	 * Align Panel에서 Align 수행 후 기록해주어야 하는 함수 
	 *
	 * @param	sPos		: 설정할 Alaign Mark Offset
	 * @param	dValueThetaAlign	: Align시의  T축의 설정값
	 * @return	int	: 0 = SUCCESS, 그 외 = Error
	 */
	void SetAlignMarkOffset(MPos_XYT sPos, double dValueThetaAlign);
	
	/**
	 * 기록된 Align 결과인 Align Mark Offset을 읽는다.
	 * Align 결과가 설정되어 있으면 SUCCESS를 return하고 Align Mark Offset을 전달한다.
	 * Align 결과가 설정되어 있지 않으면 Error를 return하고 *psPos에는 0.0을 설정한다. 
	 *
	 * @return	Error Code	: 0 = SUCCESS, 그 외 = Error
	 */
	MPos_XYT GetAlignMarkOffset();
	
	/**
	 * Align 수행여부를 알린다.
	 *
	 * @return BOOL		: TRUE(Aligned), FALSE(Not Aligned)
	 */
	BOOL IsAligned();

	/**(KKY)
	 * 이동시 인근 Carrier와 충돌여부 확인하는 함수
	 **/
	int	CheckCollisionOther(int iPosID);
	int CheckCollisionOther(MPos_XYT posTarget);
	MPos_XYT GetMovingTgPos();
	BOOL IsMovingY();//Y축이 이동중인지...

	void		SetTabInfo(MTabInfo tabinfo);
	MTabInfo	GetTabInfo();

	int	GetWorkGroup() { return m_iWorkGroup; }
	int	GetWorkInstance() { return m_iWorkInstance; }

	BOOL IsShortPitchTab();	//5Pitch이하 Short, 5Pitch초과 Long

	MPos_XYT GetPrealignReadyPos();		//110128.Add
	MPos_XYT GetUnloadingReadyPos();	//110128.Add

	int		Wait4DoneXYT();
	void					SetMovingY(BOOL bMove, MPos_XYT posTarget);

	int StartSplineMove(int iMaxPoint, double *pdPosition, double *pdPosition2, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt = TRUE); //SJ_YYK 150212 Add..
};
#endif // MTABCARRIER_H
