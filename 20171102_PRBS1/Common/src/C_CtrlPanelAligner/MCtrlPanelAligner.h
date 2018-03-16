/* 
 * Control PanelAligner Component
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

#ifndef MCTRL_PANEL_ALIGNER_H
#define MCTRL_PANEL_ALIGNER_H

#include "ICommon.h"
#include "MPos_XYT.h"
#include "DefSystem.h"

class MSystemData;
class MPanelData;
class MAutoAlignData; //SJ_YYK 150318 Add..
class MTabOffsetData; //SJ_YYK 150318 Add..
class MTabData; //SJ_YYK 150318 Add..

class IVision;
class MVisionCalibration;
class MPanelAligner;
class ILighting;
class IStateCameraCarrier;
class MCameraCarrier;
class MWorkSchedule;
class MModelChange;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
class IStateTabMounter;
class MPreBonderData;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
class MACFData;
class MCameraCarrier;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
class MFinalData;
class MCameraCarrier;
#endif

/**
 * 이 구조체는 Control PanelAligner의 내부 연결될 Component List를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct 
{
	MVisionCalibration*	m_plnkCtrlVisionCalibration;
	IVision*			m_plnkVision;
	MPanelAligner*		m_plnkPanelAligner;
#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
	IStateTabMounter*		m_plnkStateTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateTabMounter*		m_plnkStateRTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateRCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	MModelChange*			m_plnkModelChange1;
	MModelChange*			m_plnkModelChange2;
#endif

	MCameraCarrier* m_plnkCameraCarrier1;
	MCameraCarrier* m_plnkCameraCarrier2;

	ILighting*			m_plnkLighting[DEF_MAX_LIGHTING];
} SCtrlPanelAlignerRefCompList;

/**
 * This structure is defined data of Control PanelAligner.
 * 이 구조체는 Control PanelAligner 특성 정보를 관리하는 구조체이다.
 *
 * @stereotype struct 
 */
typedef struct
{
	MSystemData*		m_plnkSystemData;
	MPanelData*			m_plnkPanelData;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
	MPreBonderData*		m_plnkPreBonderData;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
	MACFData*			m_plnkACFData;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	MFinalData*			m_plnkFinalData;
#endif

	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORKER_GROUP];
	MWorkSchedule*		m_plnkWorkScheduleSecond[DEF_MAX_WORKER_GROUP];

	MAutoAlignData*		m_plnkAutoAlignData; //SJ_YYK 150318 Add..
	MTabOffsetData*		m_plnkTabOffset; //SJ_YYK 150318 Add..
	MTabOffsetData*		m_plnkTabOffset_Rear; //SJ_YYK 150318 Add..
	MTabData*			m_plnkTabData; //SJ_YYK 150318 Add..

} SCtrlPanelAlignerData;

/** Align Panel Value Define */
const	int	DEF_ALIGNPANEL_USER_WAIT	= 0;
const	int	DEF_ALIGNPANEL_USER_RECOG	= 1;
const	int	DEF_ALIGNPANEL_USER_ABORT	= 2;
	
const	int	DEF_ALIGNPANEL_JOG_MODE		= 0;
const	int	DEF_ALIGNPANEL_USER_MODE	= 1;

const	double DEF_ALIGN_T_TOLERANCE	= 5.0;
const	double DEF_ALIGN_XY_TOLERANCE	= 10.0;

const int DEF_SEARCH_FIDU_MARK_IN_SEARCH				= 0;
const int DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_1ST_STEP	= 1;
const int DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_2ND_STEP	= 2;
const int DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_3RD_STEP	= 3;
const int DEF_SEARCH_FIDU_MARK_IN_AUTO_SEARCH_4TH_STEP	= 4;

/**
 * Error Define
 */
const	int ERR_CTRL_PANEL_ALIGNER_SUCCESS		= 0;

/**
 * Control PanelAligner Class Define
 */
class MCtrlPanelAligner : virtual public ICommon
{
private:
	IVision*			m_plnkVision;
	MVisionCalibration*	m_plnkVisionCalibration;
	MPanelAligner*		m_plnkPanelAligner;
	ILighting*			m_plnkLighting[DEF_MAX_LIGHTING];

#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
	IStateTabMounter*		m_plnkStateTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	IStateTabMounter*		m_plnkStateRTabMounter[DEF_MAX_TABMOUNTER];
	IStateCameraCarrier*	m_plnkStateRCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	MModelChange*			m_plnkModelChange1;
	MModelChange*			m_plnkModelChange2;
#endif

//#ifndef DEF_PLATFORM_PREBONDER_SYSTEM
	MCameraCarrier* m_plnkCameraCarrier1;
	MCameraCarrier* m_plnkCameraCarrier2;
//#endif

	/** System Data Object*/
	MSystemData*		m_plnkSystemData;

	/** Panel Data Object*/
	MPanelData*			m_plnkPanelData;

#if defined(DEF_PLATFORM_PREBONDER_SYSTEM)
	/** PreBonder Data Object*/
	MPreBonderData*		m_plnkPreBonderData;
#elif defined(DEF_PLATFORM_ARP_SYSTEM)
	/** ACF Data Object*/
	MACFData*			m_plnkACFData;
#elif defined(DEF_PLATFORM_FINALBONDER_SYSTEM)
	/** Final Data Object*/
	MFinalData*			m_plnkFinalData;
#endif

	MWorkSchedule*		m_plnkWorkSchedule[DEF_MAX_WORKER_GROUP];
	MWorkSchedule*		m_plnkWorkScheduleSecond[DEF_MAX_WORKER_GROUP];

	MAutoAlignData*		m_plnkAutoAlignData; //SJ_YYK 150318 Add..
	MTabOffsetData*		m_plnkTabOffset; //SJ_YYK 150318 Add..
	MTabOffsetData*		m_plnkTabOffset_Rear; //SJ_YYK 150318 Add..
	MTabData*			m_plnkTabData; //SJ_YYK 150318 Add..

	double				m_dLastAutoSearchOffset;

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/
private:
	void calculateFiduMarkPosition(MPos_XYT &MarkPosition1, MPos_XYT &MarkPosition2);

//	int alignPanel(int iMarkNo, int iVisionMarkPos, BOOL bUseAutoSearch);
	int alignPanel(int iMarkNo[2], int iVisionMarkPos, BOOL bUseAutoSearch, BOOL bUseManualSearch);
	
//	void setMarkNoCameraPos(int &iMarkNo, int &iVisionMarkPos);
	void setMarkNoCameraPos(int iMarkNo[2], int &iVisionMarkPos);

//	int getFiduMarkPosition(int* rgiCamNo, int iMarkNo, int iVisionMarkPos, MPos_XYT** pxytMark, BOOL bUseAutoSearch);
	int getFiduMarkPosition(int* rgiCamNo, int* iMarkNo, int iVisionMarkPos, MPos_XYT** pxytMark, BOOL bUseAutoSearch, BOOL bUseManualSearch);

//	int autoSearchFiduMark(int* rgiCamNo, int iMarkNo, int iVisionMarkPos, MPos_XYT** pxytMark, BOOL* rgbResult);
	int autoSearchFiduMark(int* rgiCamNo, int* iMarkNo, int iVisionMarkPos, MPos_XYT** pxytMark, BOOL* rgbResult);

//	int searchFiduMark(int* rgiCamNo, int iMarkNo, BOOL* rgbUse, BOOL* rgbResult, int iOpType = DEF_SEARCH_FIDU_MARK_IN_SEARCH);
	int searchFiduMark(int* rgiCamNo, int* iMarkNo, BOOL* rgbUse, BOOL* rgbResult, int iOpType = DEF_SEARCH_FIDU_MARK_IN_SEARCH);

//	int checkPanelAlignerGetPosition(int iMarkNo, int iVisionMarkPos);
	int checkPanelAlignerGetPosition(int* iMarkNo, int iVisionMarkPos, BOOL bUseManualSearch);

	int checkTargetPosBySWLimit(MPos_XYT xytTarget);

public:  

	int CaptureFiduMark(CString strMsg);
	int CaptureFiduMarkTest(CString strMsg);

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/************************************************************************************************/
	MCtrlPanelAligner(SCommonAttribute			commonData,
				SCtrlPanelAlignerRefCompList	listRefComponent,
				SCtrlPanelAlignerData			datComponent);

	~MCtrlPanelAligner();

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 
	int Initialize(void);

	int	SetData(SCtrlPanelAlignerData datcomponent);
	
	int	AssignComponents(SCtrlPanelAlignerRefCompList listRefComponents);

/***********************************************************************************************
/*
/* Operations
/*
/************************************************************************************************/
	int PickUpPanel();
	int SendPanel();

	int MovetoLoadPos();

	int MovetoAlignPos(BOOL bUseAuto = TRUE);

	int MovetoInspectPos(int iInspectPosId, BOOL bUseAuto = TRUE);	//Pre, ARP

	int MovetoWorkPos(BOOL bUseAuto = TRUE);
	int MoveZtoWorkPos(BOOL bUseAuto = TRUE);
	int MovetoUnloadPos(BOOL bUseAuto = TRUE);

//@	int MoveCameraToPos(int PosID, BOOL wait);

	int AlignPanel(BOOL bUseAutoSearch = TRUE, BOOL bUseManualSearch = FALSE);
	int AlignPanelSetPoint(CPoint ptMark1, CPoint ptMark2);	//@

	int InspectPanel(int iPosID);		

	const BOOL CheckFiduMarkPos();

	void CheckPlateBlowOn();

	//1.PanelAligner가 Panel흡착한 상태로 Align위치로 이동하고
	//2.부상이 흡착, Aligner흡착 해제, 하강하고
	//3.Rotator실린더가 Panel중심위치로 이동하고
	//4.Rotator실린더가 상승/흡착
	int TransferPanel_AlignerToRotator();
	//1.Rotator가 Panel흡착한 상태
	//2.부상이 흡착, Rotator흡착 해제, 하강하고
	//3.Aligner가 PanelAligne위치로 이동하고
	//4.Aligner가 상승/흡착
	int TransferPanel_RotatorToAligner();

	//20111020 by kss
	/////////////////////////////////////////////////////////
	BOOL WriteAPCPreLog(CString strMsg);
	/////////////////////////////////////////////////////////

//	int MovetoTurnPos();

#ifdef DEF_PLATFORM_ARP_SYSTEM
	int LightingAlign(BOOL bWaitOpt = TRUE);

	int LightingInspection(BOOL bWaitOpt = TRUE);
#endif

	int UpdateAlignData(); //SJ_YYK 150318 Add..

};
#endif //MCTRL_PANEL_ALIGNER_H
