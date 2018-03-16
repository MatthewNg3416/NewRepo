/* 
 * Control TabMounter Component
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

#ifndef MCTRLTABMOUNTER_H
#define MCTRLTABMOUNTER_H

#include "ICommon.h"
#include "DefSystem.h"

class MSystemData;
class MProcessData;
class MWorkSchedule;
class MTabOffsetData;
class MPreBonderData;
class MTabMounter;
class MTabAligner;
class MTabCarrier;
class IVision;
class MVisionCalibration;
class MPos_XYT;
class CDBTabLoss;

/**
 * 이 구조체는 Control TabMounter의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	MTabMounter*		m_plnkTabMounter;
//	IStateTabCarrier*	m_plnkTabCarrier[DEF_MAX_TABCARRIER];
//	IStateTabCarrier*	m_plnkRTabCarrier[DEF_MAX_TABCARRIER];
	MTabCarrier*		m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	MTabCarrier*		m_plnkRTabCarrier[DEF_MAX_TABCARRIER];
	IVision*			m_plnkVision;
	MVisionCalibration*	m_plnkCtrlVisionCalibration;

#if FALSE
	MTabAligner*		m_rgplnkTabAligner[DEF_MAX_TABALIGNER_PER_GROUP];
#endif

} SCtrlTabMounterRefCompList;

/**
 * This structure is defined data of Control TabMounter.
 * 이 구조체는 Control TabMounter 특성 정보를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct
{
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkScheduleSecond;
	MTabOffsetData*		m_plnkTabOffsetData;
	MPreBonderData*		m_plnkPreBonderData;
	MTabOffsetData*		m_plnkTabOffsetData_Rear; //120208.kms
	CDBTabLoss*			m_plnkDBTabLoss;

} SCtrlTabMounterData;

/**
 * Error Define
 */
const int ERR_CTRL_TABMOUNTER_SUCCESS	= 0;

/**
 * Control TabMounter Class Define
 */
class MCtrlTabMounter : virtual public ICommon
{
private:
	MTabMounter*		m_plnkTabMounter;
//KKY.Del	MTabAligner*		m_rgplnkTabAligner[DEF_MAX_TABALIGNER_PER_GROUP];
//	IStateTabCarrier*	m_plnkTabCarrier[DEF_MAX_TABCARRIER];
//	IStateTabCarrier*	m_plnkRTabCarrier[DEF_MAX_TABCARRIER];
	MTabCarrier*		m_plnkTabCarrier[DEF_MAX_TABCARRIER];
	MTabCarrier*		m_plnkRTabCarrier[DEF_MAX_TABCARRIER];
	IVision*			m_plnkVision;
	MVisionCalibration*	m_plnkVisionCalibration;

	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];
	MTabOffsetData*		m_plnkTabOffsetData;
	//120208.kms______
	MTabOffsetData*		m_plnkTabOffsetData_Rear;
	//20090620 sj_ysb
	MPreBonderData*		m_plnkPreBonderData;
	
	CDBTabLoss*			m_plnkDBTabLoss;

	int					m_iWorkInstance;
	int					m_iWorkGroup;

private:
//KKY.Del	int getMarkPos(EWorkingSide eWorkSide, int iTabNo, int iRefCamNo, int* rgiModel, MPos_XYT* rgResultPos, RECT* prgTargetArea);

	int getWorkTabAlignerIdPerGroup(EWorkingSide eWorkSide, int iTabNo, int& iReturn);

//KKY.Del	void calculateTabPrealignTargetArea(int iRefCamNo, double* prgdReference, double dTolerance, double dRetryMargin, RECT* prgResult);

public:  
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/
	MCtrlTabMounter(SCommonAttribute commonData, SCtrlTabMounterRefCompList listRefComponent, SCtrlTabMounterData datComponent);

	~MCtrlTabMounter();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
	int	Initialize(void);

	void SetData(SCtrlTabMounterData datcomponent);
	
	void AssignComponents(SCtrlTabMounterRefCompList listRefComponents);

/***********************************************************************************************
/*
/* Operations
/*
/************************************************************************************************/
//	int LoadTabIC();
//	int LoadTabIC(int iCarrierWorkGroup, int iCarrierNo);
	int LoadTabIC(int iCarrierWorkGroup, int iCarrierNo, BOOL* parpTabLoading = NULL); //170830 JSH

//KKY.Del	int MoveToPrealignPos(EWorkingSide eWorkSide, int iTabNo);

	//2009.07.13 CYJ
//KKY.Del	int PreAlignTabIC(EWorkingSide eWorkSide, int iTabNo, BOOL bBaseLineOnlyUse = FALSE);

	//2009.06.26 CYJ
//KKY.Del	int GetPreAlignXoffsetOfTabIC(EWorkingSide eWorkSide, int iTabNo);

	// TabMounter 에 따른 Remove 위치로 이동 후, 하강해서 쓰레기통에 TabIC 를 버리고 다시 상승하는 동작까지 담당.
	int Remove();

	int MountTabIC(EWorkingSide eWorkSide, int iTabNo);

	int LogVisionMarkPos(int iCameraNo, int iMarkNo, CString strMsg);

	int MoveToMountPos(int iWorkScheduleNo, EWorkingSide eWorkSide, int iRealTabNo);

	int GetPanelRefFiduMarkPosY(double* pdPanelRefFiduMarkPosY);

	int MoveForPanelAbsorbOffsetReflection(EWorkingSide eWorkSide, int iTabNo, double dPanelAbsorbOffset);
};
#endif //MCTRLTABMOUNTER_H
