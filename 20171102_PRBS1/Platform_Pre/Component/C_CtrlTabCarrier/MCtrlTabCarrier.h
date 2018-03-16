/* 
 * Control TabCarrier Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef MCTRLTABCARRIER_H
#define MCTRLTABCARRIER_H

#include "ICommon.h"
#include "MTickTimer.h" //101124 sj_ysb 

class MSystemData;
class MPreBonderData;
class MTabOffsetData;
class IVision;
class MTabCarrier;
//class IStateTabFeeder;
class MTabFeeder;
class MVisionCalibration;
class MPos_XYT;
class CDBTabLoss;
class MNetH; //SJ_YYK 150908 Add..

/**
 * 이 구조체는 Control TabCarrier의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	IVision*			m_plnkVision;
	MTabCarrier*		m_plnkTabCarrier;
//	IStateTabFeeder*	m_plnkTabFeeder;
	MTabFeeder*			m_plnkTabFeeder;
	MVisionCalibration*	m_plnkCtrlVisionCalibration;
	MNetH*				m_plnkMelsecAOCIF;  //SJ_YYK 150908 Add...
} SCtrlTabCarrierRefCompList;

/**
 * This structure is defined data of Control TabCarrier.
 * 이 구조체는 Control TabCarrier 특성 정보를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct
{
	/** System Data Object*/
	MSystemData*	m_plnkSystemData;
	MPreBonderData*		m_plnkPreBonderData;
	MTabOffsetData*		m_plnkTabOffsetData;
	MTabOffsetData*		m_plnkTabOffsetData_Rear;
	CDBTabLoss*			m_plnkDBTabLoss;
	int				m_iTabAlignCam1 ;
	int				m_iTabAlignCam2 ;
} SCtrlTabCarrierData;

/**
 * Error Define
 */
const int ERR_CTRL_TABCARRIER_SUCCESS	= 0;
const int DEF_COLLISION_WAIT_TIME		= 10000;

/**
 * Control TabCarrier Class Define
 */
class MCtrlTabCarrier : virtual public ICommon
{
private:	
	IVision*			m_plnkVision;
	MTabCarrier*		m_plnkTabCarrier;
//	IStateTabFeeder*	m_plnkTabFeeder;
	MTabFeeder*			m_plnkTabFeeder;
	MNetH*				m_plnkMelsecAOCIF;  //SJ_YYK 150908 Add...

	/** System Data Object*/
	MSystemData*		m_plnkSystemData;
	MPreBonderData*		m_plnkPreBonderData;
	MTabOffsetData*		m_plnkTabOffsetData;
	MTabOffsetData*		m_plnkTabOffsetData_Rear;
	MVisionCalibration* m_plnkCtrlVisionCalibration;
	CDBTabLoss*			m_plnkDBTabLoss;

	int m_iTabAlignCam1;
	int m_iTabAlignCam2;

	int	m_iWorkGroup;
	int m_iWorkInstance;
	int m_iAOCUnitNo; //170906 JSH

	MTickTimer			m_timerPressBlow; //101124 sj_ysb

public:  
	/**
	 * @stereotype constructor
	 *
	 * @param	commonData			: 공통 Component Data
	 * @param	listRefComponent	: CtrlTabCarrier Component에서 참조할 Component Pointer
	 * @param	datComponent		: CtrlTabCarrier Component Data
	 */
	MCtrlTabCarrier(SCommonAttribute commonData, SCtrlTabCarrierRefCompList listRefComponent, SCtrlTabCarrierData datComponent);

	/**
	 * Destructor
	 */
	~MCtrlTabCarrier();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
	int Initialize(void);

	int	SetData(SCtrlTabCarrierData datcomponent);
	
	int	AssignComponents(SCtrlTabCarrierRefCompList listRefComponents);

/***********************************************************************************************
/*
/* Operations
/*
/************************************************************************************************/
//	int LoadTabIC(int iVacNo);

//	int WaitTabIC(int iVacNo);
	
//	int MoveLoadingPos(int iLoadPosNum);

	int LoadTabIC();
//	int LoadTabIC(BOOL bFirst = TRUE, BOOL bSecond = FALSE);

	int WaitTabIC();
	
	int MoveReadyPos();

	int MoveLoadingPos();

	int MovePreAlignPos();

	int MoveUnloadingPos(int nTabNo = -1);	
	int MoveUnloading2Pos(int nTabNo = -1);	

	int AlignTabIC(MPos_XYT* pRetAlignOffset);
	void getFiduTabMarkPosition(MPos_XYT &TabMarkPosition1, 
									 MPos_XYT &TabMarkPosition2, 
									 int& piFiduTabMark1Pos, 
									 int& piFiduTabMark2Pos,
									 int& piTabCamPos);
	int	getTabMarkRecogPosition(
		int			iCameraNo,
		int			iMarkNo,
		int			iPosIndex,
		MPos_XYT*	pResult);

	//20121004 SJ_HJG 횡전개
	//20120826 sj_ysb
	int	getTabMarkRecogBlobPosition(
		int			iCameraNo,
		int			iMarkNo,
		int			iPosIndex,
		double		dRefVX,
		double		dRefVY,
		MPos_XYT*	pResult);

	//SJ_YYK 150213 Add..
	int MoveSplineLoadPos();
	int MoveSplinePrealignPos();

};
#endif //MCTRLTABCARRIER_H