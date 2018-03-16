#ifndef MSYSTEMDATA_H
#define MSYSTEMDATA_H

#include "MFileManager.h"
#include "DefSystem.h"
#include "MDataDouble.h"
#include "StdAfx.h"

const int DEF_STOP_MAX_ITEM	= 28;

//110624_____
const int DEF_VALIDATION_MAX_ITEM = 32;
//___________
const int DEF_MODULE_PORT_MAX_ID = 12;
const int DEF_MODULE_PORT_MAX_ID_2= 12;

typedef enum
{
	SYSTEM_TYPE_NONE			= -1,
    SYSTEM_TYPE_SOURCE			= 0,
    SYSTEM_TYPE_GATE			= 1,
} ESystemType;

typedef enum
{
	ENCODER_PULSE				= 0,
	COMMAND_PULSE				= 1,
} EPositionRef;

#define COMMON_SECTION			_T("Common")
#define FUNCTION_SECTION		_T("Function")
#define MATERIAL_SECTION		_T("Material")
#define TABMOUNTER_SECTION		_T("TabMounter")
#define TABFEEDER_SECTION		_T("TabFeeder")
#define TABCARRIER_SECTION		_T("TabCarrier")
#define	PANELALIGNER_SECTION	_T("PanelAligner")
#define MODEL_CHANGE_DIST_SECTION _T("ModelChangeDistance")
#define	TABALIGNER_SECTION		_T("TabAligner")
#define CAMERA_PRISM_SECTION	_T("CameraPrism")
#define R_CAMERA_PRISM_SECTION	_T("RCameraPrism")
#define LOG_SECTION				_T("Log")
#define EQSTOP_SECTION			_T("EqStopItem")
#define PRODUCT_CHECK_SECTION	_T("Product Check")	//110624
#define MODULEPORT_SECTION		_T("ModulePort")
#define AUTOALIGN_SECTION		_T("AutoAlign")
#define ERRORNOTSEND_SECTION	_T("ErrorNotSend")

/* jdy sesl 081231
const UINT LANGUAGE_SELECTED	= -1;
const UINT LANGUAGE_KOREAN		= 0;
const UINT LANGUAGE_ENGLISH		= 1;
const UINT LANGUAGE_KOR_ENG		= 2;
*/

/**
 * System Data를 파일에서 읽어서 Member Variable 이 그 값을 갖고 있다.
 */

#include <vector>
using namespace std;

class MSystemData : public MFileManager
{
private:
	CString			m_strSection;
	vector<IDataType*> m_iDataType;

public:
#undef SYSTEM_DATA
#undef SYSTEM_DATA_ARR
#define SYSTEM_DATA(type, name) type name;
#define SYSTEM_DATA_ARR(type, name, arr) type name[arr];
#include "info\test.inf"
#undef SYSTEM_DATA
#undef SYSTEM_DATA_ARR

	/************************************************************************/
	/*        Common Section											    */
	/************************************************************************/
	/** Model File Name */
	CString			m_strModelFileName;
	/** Model File이 저장된 Directory */
	CString			m_strModelFileDir;
	/** Operator Password */
	CString			m_strOperatorPassWord;
	/** Engineer Password */
	CString			m_strEngineerPassWord;
	/** Maker Password */
	CString			m_strMakerPassWord;
	/** 설비 사용 언어 설정 */
	UINT			m_uiLanguageSelect;
	/** Run Mode Status */
	ERunMode		m_eRunMode;
	/** Velocity Mode */
	int				m_iVelocityMode;
	/** Line Controller IP */
	CString			m_strLineControllerIP;
	/** Line Controller Port */
	UINT			m_uiLineControllerPort;
	/** System Type */
	ESystemType		m_eSystemType;
	/** Unit Number */
	int				m_iUnitNumber;

	/** Stop Sensor 감지 후 Stopper 까지 도달하기 위한 Delay */
	double			m_dStopperArrivingDelay;

	/** Panel PreAlign 전진 후 Stopper 까지 도달하기 위한 Delay */
	//	double			m_dPanelPreAlignDelay;

	//PanelMark간 거리 Tab Mark간 거리 비교 Data
	double			m_dPanelTabDist;

	//20110914 SJ_HJG
	double			m_dEmptyElapseTime;

	/************************************************************************/
	/*        Function Section											    */
	/************************************************************************/
	/** Fidu Mark Auto Search 기능 사용여부 */
	BOOL			m_bUseAutoSearchPanel;
	/** 보조마크 사용 여부 */
	BOOL			m_bUsePlateVacuum;
	/** Tab Reel 자동전환 사용 여부 */
	BOOL			m_bUseTabReelAutoChange;	// sesl_jdy : 임시로 PanelMarkManualSearch 기능으로 사용.
												// 자재 사전 Alarm 사용 여부
	BOOL			m_bUseMaterialPrealarm;
	/** Buzzer Mode */
	BOOL			m_bUseBuzzer;
	/** Safety Module 사용 여부 */
	BOOL			m_bSafetySensor;
	/** TabMounter : After Inspection 시 Tab Mark Search Area Auto Teaching 사용 여부 */
	BOOL			m_bUseAutoSearchAreaInAfterInspection;
	/** 자동운전 시, 압착 후 Inspection 을 수행할 것인지 여부 */
	BOOL			m_bUseInspectionAfterMountInAutoRun;

	/** Test 용도 : Inspectoin Offset 반영 이동 후 자동운전 진행 안함. */
	//	BOOL			m_bUseTestStop;

	/**  Panel Pusher 사용 여부 반영. */	//@131127.KKY.기능 수정 : 0->미사용, 1->Align인식에러시 재시도, 2->무조건 사용 
	BOOL			m_bUsePanelPusher; // 2009.01.23 CYJ
									   /** Panel Validation 기능 사용 여부 */
	BOOL			m_bUsePanelValidation;
	/** Panel 흡착으로 인한 Panel 위치 변화 반영 여부 */
	BOOL			m_bUsePanelAbsorbOffsetReflection;
	/** Safety Module 을 사용하지 않을 경우, Door Open Error 를 Display 할 것인지 여부를 저장. */
	//SESL_LKH_090711 : m_bSafetySensor와 통합
	//	BOOL			m_bDisplayDoorOpenError;
	/** VIP Mode : R,Y Lamp Off, Buzzer Off, G Lamp 항시 On */
	BOOL			m_bUseVIPMode;
	/** Mold Blow 사용 여부 */
	BOOL			m_bUseMoldBlow;
	//20081110 by kss
	/** Use Panel Pre Align */
	BOOL			m_bUsePanelPreAlign;
	/** Use Heater Alarm */
	BOOL			m_bUseHeaterAlarm;

	UINT			m_nUseFeeder;//0->Front, 1->Rear, 2->All

	BOOL			m_bUseDryrunFeed;

	// NSMC
	/** Auto Model Change 사용 여부 */
	BOOL			m_bUseAutoModelChange;
	CString			m_strCurrentDeviceID;
	BOOL			m_bUseMaterialDeviceID;

	//100825.kms___________________
	BOOL			m_bUseMoldValidation;

	// 100807. ECID ___________
	BOOL		m_bUseECDataReport;

	//110110.Add_____
	BOOL		m_bUse1FeederAllMount;//사용중이던 TabIC가 소진되면 반대편 TabFeeder사용
									  //SJ_YYK 110414 Add.. Align Pass Mode
	BOOL		m_bUseAlignPass;

	//Online 관련 추가
	BOOL			m_bUseSyncMode;
	BOOL			m_bUseProductPlanValidation;
	BOOL			m_bUseTXDataCheckMode;
	//120130.kms______
	BOOL		m_bUse4MountAll;
	BOOL		m_bUseIonizerCheck;

	BOOL		m_bUseTabRemoveAuto;	//@TabCarrier Mark에러시 자동버림

	BOOL		m_bUseMoldAutoSearch;

	BOOL		m_bUseCheckVacuumBeforeTabLoading; //161109 JSH cam add

												   //170612 JSH 이중부착 방지 Inspection Tab Blob 여부
	BOOL		m_bUseInspectionTabBolb;

	//170718 JSH 임시 기능 잘되면 항상 사용 
	BOOL		m_bUse1stPanelAlign;

	//170927 JSH
	BOOL		m_bUseBondingAfterImgLog;

	/************************************************************************/
	/*        Material Section											    */
	/************************************************************************/
	/** Mold[1] 금형  전체 사용 횟수 */
	UINT			m_uiMold1_TotalCount;
	/** Mold[2] 금형 전체 사용 횟수 */
	UINT			m_uiMold2_TotalCount;
	/** Mold[1] 금형 세정 후 사용 횟수 */
	UINT			m_uiMold1_CurCount;
	/** Mold[2] 금형 세정 후 사용 횟수 */
	UINT			m_uiMold2_CurCount;

	/************************************************************************/
	/*        TabMounter Section											    */
	/************************************************************************/
	/** Tool Offset : TabMounter 방향 */
	double			m_rgdToolOffsetMounter[DEF_MAX_TABMOUNTER];
	/** Tool Offset : TabAligner 방향 */
	double			m_rgdToolOffsetTabAligner[DEF_MAX_TABMOUNTER];

	/** Mounter Tool 사용 유무 */	// 확인 필요 : 사용 안함.
								// 삭제 예정		BOOL			m_rgbUseMounter[DEF_MAX_TABMOUNTER];

								/** TabIC 부착 Retry 제한 횟수
								*	: 동일 Tab 에 대해서 이 값 이상 부착 Retry 시도하면 Error 처리.
								*/
	int				m_iMountRetryLimit;

	/** 가압착 시간 */
	double			m_dBondTime;

	/** TabMounter 이동 시작 후 대기 시간 : 다른 TabMounter 동작 시작 Delay */
	double			m_dStartMoveDelay;

	/** TabMounter Group 별 Mount 동작 시차 설정
	*	: Group1 압착 후, 이 시간 동안 기다렸다가 Group2 압착 시작.
	*/
	double			m_d2ndGroupMountStartDelay;

	/** Interface 신호 대기 제한 시간 설정 (단위: 초) */
	double			m_dIFDelayLimit;

	/** TabMounter 작업 관련 동작 안정화 시간 (단위: 초) */
	double			m_dTabMounterStabilizationDelay;

	/** TabMounter Mount 동작 관련 위치에 대한 위치 값 Tolerance */
	double			m_dMountPosTolerance;

	/** TabMounter Mount 동작 관련 위치에 대한 이동 속도 값*/
	double			m_dMountMoveSpeed;

	/***********************************************************************/
	/*       TabFeeder Section											   */
	/***********************************************************************/
	/** 금형 Data 기록 주기 : 설정된 회수만큼의 Panel 배출 후 Data 기록. */
	UINT			m_uiMoldMaxCount;
	/** 금형 Blow 주기*/
	UINT			m_uiMoldBlowPeriod;
	/** 금형 Blow 시간*/
	UINT			m_uiMoldBlowTime;
	/** 세정 작업 후 금형 사용 제한 횟수*/
	UINT			m_uiMoldLimitCountAfterCleaning; //presscleancount
													 /** 연속 Bad-Tab IC 제한 횟수*/
	UINT			m_uiBadTabLimitCount;			//m_dTabICDefectCount

	double	m_dSprocket1_F_AutoHoming_Offset;
	double	m_dSprocket1_R_AutoHoming_Offset;
	double	m_dSprocket2_F_AutoHoming_Offset;
	double	m_dSprocket2_R_AutoHoming_Offset;

	//130424.kms____________
	//	double m_dCalResult_F;
	//	double m_dCalResult_R;
	double m_dCalResult_F[31];
	double m_dCalResult_R[31];
	double m_dMoldRegCnt_F;
	double m_dMoldRegCnt_R;
	//______________________

	/**********************************************************************/
	/*       TabCarrier Section										      */
	/**********************************************************************/
	/** Carrier Tab Loading Retry 횟수 */
	// 삭제 예정.	int				m_iCarrierRetryCount;

	double			m_rgdCarrierHeadCenterX[DEF_MAX_TABCARRIER];
	double			m_rgdCarrierHeadCenterY[DEF_MAX_TABCARRIER];
	double			m_rgdRCarrierHeadCenterX[DEF_MAX_TABCARRIER];
	double			m_rgdRCarrierHeadCenterY[DEF_MAX_TABCARRIER];
	/**********************************************************************/
	/*       PanelAlinger Section								          */
	/**********************************************************************/
	/** 이 값보다 PanelAligner, TabMounter 이동 거리가 작으면 저속 이동(설정 속도의 1/5) 한다. */
	int				m_iDistance4DifVel;
	/** Panel Align Reference Camera 기준 위치 ~ Active Panel Aligner Vacuum Pad 간 거리 : X */
	double			m_dActiveAlignerPosX;
	/** Panel Align Reference Camera 기준 위치 ~ Active Panel Aligner Vacuum Pad 간 거리 : Y */
	double			m_dActiveAlignerPosY;
	/** Model Change 축 원점위치에서 Panel Aligner Vacuum Pad Active Center ~ Passive Center 간 거리 : Y */
	double			m_dOriginDistBetweenAligners;
	/** Fidu Mark 자동 검색 범위 */
	double			m_dAutoSearchPanelMarkDistance;
	/** Panel Aligner Align Tolerance */
	double			m_dPanelAlignTolerance;
	/** Panel Aligner Retry Limit */
	int				m_iPanelAlignRetryLimit;
	/** Panel Aligner Backlash */
	double			m_dBacklash;
	/** Panel Aligner Air Plate Blow On Delay Time (sec) */
	double			m_dAirPlateBlowOnDelay;
	/** Panel Aligner Work Position Velocity */
	double          m_dPanelAlignerWorkVelocity;

	/** Panel Aligner 회전 중심에서 Aligner Camera(Inspection Cam1, Inspection Cam8)중심까지의 거리(KKY)*/
	double			m_dAlignCam1_FixedPos_X;	//PanelAligner회전 중심에서 Align시 Inspection
	double			m_dAlignCam1_FixedPos_Y;
	double			m_dAlignCam2_FixedPos_X;
	double			m_dAlignCam2_FixedPos_Y;
	double			m_dCamCarrier1_FixedPosX;	//Align시 Inspection Camera간 중심이 PanelMark와 일치하는위치
	double			m_dCamCarrier2_FixedPosX;
	/** Panel Aligner 간 중심 거리 **/
	double			m_dDistBetweenAligners;

	/** Panel Aligner 중심과 회전 실린더 중심간 거리(Gate에서만) **/
	double			m_dDist_AlignerToRotator_X;
	double			m_dDist_AlignerToRotator_Y;

	/** Inspect Camera가 이동할수 있는 +/- Limit 거리 **/
	double			m_dDistPosInspectCamera[DEF_MAX_GROUP];
	double			m_dDistNegInspectCamera[DEF_MAX_GROUP];

	//@KKY.130902.고정좌표 잡을때의 CameraCarrier의 너비____________
	double			m_dInspectCamFixedPosWidth;
	//@_____________________________________________________________

	/**********************************************************************/
	/*       Modle Change Distance Section						          */
	/**********************************************************************/
	/** Model Change 축 원점 위치 ~ 물류 방향 작업 기준 위치 간 거리
	*	 : Model 에 따른 Model Change 축 이동 거리 계산 Data 로 사용.
	*/

	//Panel Aligner 중심에서 부터 ModelChange(Inspection Camera중심)까지 거리
	double			m_dModelChangeOriginDist[DEF_MAX_GROUP];

	/** Rear Gantry 원점 위치로부터 Model Change 축 원점까지의 거리.
	*/
	//	double			m_dDistBetweenRGantryHomeToModelChangeHome;

	//Panel Aligner 중심에서 부터 Gantry까지 거리(ToolMounter)
	//	double			m_dDistBetweenGantryHomeToModelChangeHome[DEF_MAX_GROUP];
	double			m_dGantryOriginDist[DEF_MAX_GROUP];

	double          m_dDistBadTabOriginDist[DEF_MAX_GROUP];


	/**********************************************************************/
	/*        TabAligner Section								          */
	/**********************************************************************/
	/** Panel Align Reference Camera 기준 위치 ~ TabIC Pre-Align Camera 1 간 거리
	*	 - Source : Y 축 거리, Gate_Front/Rear : X 축 거리
	*/
	double			m_dTabCameraDist1[DEF_MAX_WORKER_GROUP];
	/** TabIC Pre-Align Camera 1 ~ TabIC Pre-Align Camera 2 간 거리
	*	 - Source : Y 축 거리, Gate_Front/Rear : X 축 거리
	*/
	double			m_dTabCameraDist2[DEF_MAX_WORKER_GROUP];
	/** TabIC Pre-Align Camera 2 ~ TabIC Pre-Align Camera 3 간 거리
	*	 - Source : Y 축 거리, Gate_Front/Rear : X 축 거리
	*/
	double			m_dTabCameraDist3[DEF_MAX_WORKER_GROUP];
	/** TabIC Pre-Align Camera 3 ~ TabIC Pre-Align Camera 4 간 거리
	*	 - Source : Y 축 거리, Gate_Front/Rear : X 축 거리
	*/
	double			m_dTabCameraDist4[DEF_MAX_WORKER_GROUP];

	/** Panel Lead 와 TabIC Lead 길이 차 제한 값 */
	double			m_dTabICLeadCheck;

	/** TabAligner 가 1 Tab Prealign 을 위해 움직이는 제한 횟수 */
	int				m_iTabAlignerOperationLimit;

	/** TabAligner 작업 시 Tab Mark 인식 위치 허용 오차 (Aligner 축 방향) */
	double			m_dTabAlignerTolerance;

	/** TabAligner 사용 여부 설정. */
	BOOL			m_rgbUseTabAligner[DEF_MAX_TABALIGNER];

	/** TabIC Aligner 별 Base Offset 1,2 : [Source - X1,X2] [Gate - Y1,Y2] */
	double			m_rgdTabAlignerBase1[DEF_MAX_TABALIGNER];
	double			m_rgdTabAlignerBase2[DEF_MAX_TABALIGNER];

	/** TabIC Aligner 별 Etc. Offset 1,2 : [Source - X1,X2] [Gate - Y1,Y2] */
	double			m_rgdTabAlignerEtcOffset1[DEF_MAX_TABALIGNER];
	double			m_rgdTabAlignerEtcOffset2[DEF_MAX_TABALIGNER];

	/************************************************************************/
	/*    Camera Prism Section										        */
	/************************************************************************/
	/** Dual View Camera Prism 사용 시, Vision 화면을 양분하는 View 간의 중심 거리 */
	///double			m_rgdDistBetweenPrismViewCenter[DEF_MAX_CAMERA_NO]; 
	//	double			m_rgdInspectionCamDistBetweenPrismViewCenter[DEF_MAX_INSPECTION_CAMERA]; ///4
	//	double			m_rgdPreAlignCamDistBetweenPrismViewCenter[DEF_MAX_GROUP]; ///2	

	double			m_rgdInspectionCamDistViewCenter_Expand[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER]; //확장시 카메라 중심간 거리
	double			m_rgdInspectionCamDistViewCenter_Shrink[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER]; //축소시 카메라 중심간 거리
	double			m_rgdInspectionCamDist_CenterToCenter[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];//확장 카메라 중심 - 축소 카메라 중심
																										 //	double			m_rgdInspectionCamVariableDist[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];

	double			m_dData_CameraExpand[DEF_MAX_GROUP][DEF_MAX_CAM_EXPAND];//확장 카메라 중심 - 축소 카메라 중심

	double			m_rgdPreAlignCamDistViewCenter_Expand[DEF_MAX_GROUP]; //확장시 카메라 중심간 거리
	double			m_rgdPreAlignCamDistViewCenter_Shrink[DEF_MAX_GROUP]; //축소시 카메라 중심간 거리
	double			m_rgdPreAlignCamDist_CenterToCenter[DEF_MAX_GROUP];//확장 카메라 중심 - 축소 카메라 중심
	double			m_rgdPrealignCamVariableDist[DEF_MAX_GROUP];

	/************************************************************************/
	/*        Log Section											        */
	/************************************************************************/
	/** 에러 메세지 로깅 여부 */
	BOOL			m_bLogLevelError;
	/** Monitoring 메세지 로깅 여부 */
	BOOL			m_bLogLevelMonitoring;
	/** Normal 메세지 로깅 여부 */
	BOOL			m_bLogLevelNormal;
	/** Tact Time 로깅 여부 */
	BOOL			m_bLogLevelTactTime;
	//170427 JSH.s
	/** Eq Tact Time 로깅 여부 */
	BOOL			m_bLogLevelEqTactTime;
	//170427 JSH.e
	/** 로그 파일 보관 기간 */
	int				m_iLogKeepingDays;

	//** Eq Stop String Data **/
	//100121 SJ_YYK
	CString	m_strStopItem[DEF_STOP_MAX_ITEM];
	CString	m_strStopCode[DEF_STOP_MAX_ITEM];

	//110624_____
	BOOL m_bValidationCheckSelect[DEF_VALIDATION_MAX_ITEM];
	//___________

	//110705.KMS_________
	CString m_strEQPID;
	CString m_strModulePortID[DEF_MODULE_PORT_MAX_ID];
	//___________________

	//110707_____
	BOOL m_nPressActionMode;//0->Pin상승후 Punching, 1->동시동작
	BOOL m_nSprocket1ClutchMode;//0->Feeding후 ClutchOff, 1->Punching후 ClutchOff, 2->Clutch 미사용
								//___________

								//110812 SJ_KJS
	CString m_strModuleID;
	CString m_strModuleID2;
	CString m_strModuleID3_1;
	CString m_strModuleID3_2;
	CString m_strModuleID3_3;
	CString m_strModuleID3_4;
	UINT m_iReportModuleLevel;
	CString m_strReportModuelID;
	//___________________

	//130320.kms________
	CString	m_strErrorNo[DEF_MAX_ERROR_NOT_SEND];

	//20111205 SJ_HJG
	double m_dTabOffsetdata;
	double m_dTabOffsetAverage;
	BOOL   m_bUseAutoAlignSystem;
	//_______________

	//131029. SJ_YTH
	double m_dTabXCheck;
	double m_dTabYCheck;
	//___________________//
	

private:
	void initialize();

public:
    MSystemData(CString strFileName, CString strPath);
	MSystemData(CString strFileName);
	~MSystemData();

	int ReadData();    
    int WriteData();
	int CancelChangeData();

	// jdy2
	BOOL WriteRunModeData();
};
#endif // MSYSTEMDATA_H
