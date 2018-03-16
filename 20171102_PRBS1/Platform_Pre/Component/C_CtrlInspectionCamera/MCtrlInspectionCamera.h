/* 
 * Control InspectionCamera Component
 *
 * Copyright 2007 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef MCTRLINSPECTIONCAMERA_H
#define MCTRLINSPECTIONCAMERA_H

#include "ICommon.h"

class MSystemData;
class MProcessData;
class MWorkSchedule;

class IVision;
class MVisionCalibration;
class MCameraCarrier;
class MTabMounter;

//20120216 SJ_HJG
class ILighting;
class MPreBonderData;
class MTabData;

/**
 * 이 구조체는 Control InspectionCamera의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	IVision*			m_plnkVision;
	MCameraCarrier*		m_plnkCamCarrier;
	MVisionCalibration*	m_plnkCtrlVisionCalibration;
	MTabMounter*		m_plnkTabMounter;
	ILighting*			m_plnkLighting[DEF_MAX_LIGHTING]; //20120216 SJ_HJG

} SCtrlInspectionCameraRefCompList;

/**
 * This structure is defined data of Control InspectionCamera.
 * 이 구조체는 Control InspectionCamera 특성 정보를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct
{
	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MWorkSchedule*		m_plnkWorkSchedule;
	MWorkSchedule*		m_plnkWorkScheduleSecond;
	MPreBonderData*		m_plnkPreBonderData; //20120216 SJ_HJG
	MTabData*			m_plnkTabData;

} SCtrlInspectionCameraData;

/**
 * Error Define
 */
const int ERR_CTRL_INSPECTIONCAMERA_SUCCESS = 0;

/**
 * Control InspectionCamera Class Define
 */
class MCtrlInspectionCamera : virtual public ICommon
{
private:
	IVision*			m_plnkVision;
	MCameraCarrier*		m_plnkCamCarrier;
	MVisionCalibration*	m_plnkCtrlVisionCalibration;
	MTabMounter*		m_plnkTabMounter;

	MSystemData*		m_plnkSystemData;
	MProcessData*		m_plnkProcessData;
	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORK_SCHEDULE];

	//20120216 SJ_HJG
	ILighting*			m_plnkLighting[DEF_MAX_LIGHTING];
	MPreBonderData*		m_plnkPreBonderData;
	MTabData*			m_plnkTabData;

	int					m_iWorkInstance;
	int					m_iWorkGroup;

private:
	int getTabAlignOffset(int iScheduleNo, int iTabNo, int iTargetStatus = DEF_BEFORE_MOUNT, CString strGlassData = "");

	int getErrorCode(int iCamNo, int iVisionErrorCode, int iTargetMarkType);
public:  
/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/
	MCtrlInspectionCamera(SCommonAttribute commonData, SCtrlInspectionCameraRefCompList listRefComponent, SCtrlInspectionCameraData datComponent);

	~MCtrlInspectionCamera();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
	int Initialize(void);

	void SetData(SCtrlInspectionCameraData datcomponent);
	
	void AssignComponents(SCtrlInspectionCameraRefCompList listRefComponents);

/***********************************************************************************************
/*
/* Operations
/*
/************************************************************************************************/
//20111020 by kss
/////////////////////////////////////////////////////////
	int InspectBeforeMount(int iScheduleNo, int iTabNo, CString strGlassData ="");

	int InspectAfterMount(int iScheduleNo, int iTabNo, CString strGlassData ="");
/////////////////////////////////////////////////////////
	
	//131029. SJ_YTH
	BOOL WriteTabCheckLog(CString strMsg);
	//int WriteTabImageLog(int iCamNo); //170324 SJ_YSH
	int WriteTabImageLog(int ScheduleNo, int TabNo); //170324 SJ_YSH
	int InspectTabAttached(int iTabNo); //170922 JSh
};
#endif //MCTRLINSPECTIONCAMERA_H
