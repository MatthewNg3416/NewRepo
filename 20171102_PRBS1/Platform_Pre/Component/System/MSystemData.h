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
 * System Data�� ���Ͽ��� �о Member Variable �� �� ���� ���� �ִ�.
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
	/** Model File�� ����� Directory */
	CString			m_strModelFileDir;
	/** Operator Password */
	CString			m_strOperatorPassWord;
	/** Engineer Password */
	CString			m_strEngineerPassWord;
	/** Maker Password */
	CString			m_strMakerPassWord;
	/** ���� ��� ��� ���� */
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

	/** Stop Sensor ���� �� Stopper ���� �����ϱ� ���� Delay */
	double			m_dStopperArrivingDelay;

	/** Panel PreAlign ���� �� Stopper ���� �����ϱ� ���� Delay */
	//	double			m_dPanelPreAlignDelay;

	//PanelMark�� �Ÿ� Tab Mark�� �Ÿ� �� Data
	double			m_dPanelTabDist;

	//20110914 SJ_HJG
	double			m_dEmptyElapseTime;

	/************************************************************************/
	/*        Function Section											    */
	/************************************************************************/
	/** Fidu Mark Auto Search ��� ��뿩�� */
	BOOL			m_bUseAutoSearchPanel;
	/** ������ũ ��� ���� */
	BOOL			m_bUsePlateVacuum;
	/** Tab Reel �ڵ���ȯ ��� ���� */
	BOOL			m_bUseTabReelAutoChange;	// sesl_jdy : �ӽ÷� PanelMarkManualSearch ������� ���.
												// ���� ���� Alarm ��� ����
	BOOL			m_bUseMaterialPrealarm;
	/** Buzzer Mode */
	BOOL			m_bUseBuzzer;
	/** Safety Module ��� ���� */
	BOOL			m_bSafetySensor;
	/** TabMounter : After Inspection �� Tab Mark Search Area Auto Teaching ��� ���� */
	BOOL			m_bUseAutoSearchAreaInAfterInspection;
	/** �ڵ����� ��, ���� �� Inspection �� ������ ������ ���� */
	BOOL			m_bUseInspectionAfterMountInAutoRun;

	/** Test �뵵 : Inspectoin Offset �ݿ� �̵� �� �ڵ����� ���� ����. */
	//	BOOL			m_bUseTestStop;

	/**  Panel Pusher ��� ���� �ݿ�. */	//@131127.KKY.��� ���� : 0->�̻��, 1->Align�νĿ����� ��õ�, 2->������ ��� 
	BOOL			m_bUsePanelPusher; // 2009.01.23 CYJ
									   /** Panel Validation ��� ��� ���� */
	BOOL			m_bUsePanelValidation;
	/** Panel �������� ���� Panel ��ġ ��ȭ �ݿ� ���� */
	BOOL			m_bUsePanelAbsorbOffsetReflection;
	/** Safety Module �� ������� ���� ���, Door Open Error �� Display �� ������ ���θ� ����. */
	//SESL_LKH_090711 : m_bSafetySensor�� ����
	//	BOOL			m_bDisplayDoorOpenError;
	/** VIP Mode : R,Y Lamp Off, Buzzer Off, G Lamp �׽� On */
	BOOL			m_bUseVIPMode;
	/** Mold Blow ��� ���� */
	BOOL			m_bUseMoldBlow;
	//20081110 by kss
	/** Use Panel Pre Align */
	BOOL			m_bUsePanelPreAlign;
	/** Use Heater Alarm */
	BOOL			m_bUseHeaterAlarm;

	UINT			m_nUseFeeder;//0->Front, 1->Rear, 2->All

	BOOL			m_bUseDryrunFeed;

	// NSMC
	/** Auto Model Change ��� ���� */
	BOOL			m_bUseAutoModelChange;
	CString			m_strCurrentDeviceID;
	BOOL			m_bUseMaterialDeviceID;

	//100825.kms___________________
	BOOL			m_bUseMoldValidation;

	// 100807. ECID ___________
	BOOL		m_bUseECDataReport;

	//110110.Add_____
	BOOL		m_bUse1FeederAllMount;//������̴� TabIC�� �����Ǹ� �ݴ��� TabFeeder���
									  //SJ_YYK 110414 Add.. Align Pass Mode
	BOOL		m_bUseAlignPass;

	//Online ���� �߰�
	BOOL			m_bUseSyncMode;
	BOOL			m_bUseProductPlanValidation;
	BOOL			m_bUseTXDataCheckMode;
	//120130.kms______
	BOOL		m_bUse4MountAll;
	BOOL		m_bUseIonizerCheck;

	BOOL		m_bUseTabRemoveAuto;	//@TabCarrier Mark������ �ڵ�����

	BOOL		m_bUseMoldAutoSearch;

	BOOL		m_bUseCheckVacuumBeforeTabLoading; //161109 JSH cam add

												   //170612 JSH ���ߺ��� ���� Inspection Tab Blob ����
	BOOL		m_bUseInspectionTabBolb;

	//170718 JSH �ӽ� ��� �ߵǸ� �׻� ��� 
	BOOL		m_bUse1stPanelAlign;

	//170927 JSH
	BOOL		m_bUseBondingAfterImgLog;

	/************************************************************************/
	/*        Material Section											    */
	/************************************************************************/
	/** Mold[1] ����  ��ü ��� Ƚ�� */
	UINT			m_uiMold1_TotalCount;
	/** Mold[2] ���� ��ü ��� Ƚ�� */
	UINT			m_uiMold2_TotalCount;
	/** Mold[1] ���� ���� �� ��� Ƚ�� */
	UINT			m_uiMold1_CurCount;
	/** Mold[2] ���� ���� �� ��� Ƚ�� */
	UINT			m_uiMold2_CurCount;

	/************************************************************************/
	/*        TabMounter Section											    */
	/************************************************************************/
	/** Tool Offset : TabMounter ���� */
	double			m_rgdToolOffsetMounter[DEF_MAX_TABMOUNTER];
	/** Tool Offset : TabAligner ���� */
	double			m_rgdToolOffsetTabAligner[DEF_MAX_TABMOUNTER];

	/** Mounter Tool ��� ���� */	// Ȯ�� �ʿ� : ��� ����.
								// ���� ����		BOOL			m_rgbUseMounter[DEF_MAX_TABMOUNTER];

								/** TabIC ���� Retry ���� Ƚ��
								*	: ���� Tab �� ���ؼ� �� �� �̻� ���� Retry �õ��ϸ� Error ó��.
								*/
	int				m_iMountRetryLimit;

	/** ������ �ð� */
	double			m_dBondTime;

	/** TabMounter �̵� ���� �� ��� �ð� : �ٸ� TabMounter ���� ���� Delay */
	double			m_dStartMoveDelay;

	/** TabMounter Group �� Mount ���� ���� ����
	*	: Group1 ���� ��, �� �ð� ���� ��ٷȴٰ� Group2 ���� ����.
	*/
	double			m_d2ndGroupMountStartDelay;

	/** Interface ��ȣ ��� ���� �ð� ���� (����: ��) */
	double			m_dIFDelayLimit;

	/** TabMounter �۾� ���� ���� ����ȭ �ð� (����: ��) */
	double			m_dTabMounterStabilizationDelay;

	/** TabMounter Mount ���� ���� ��ġ�� ���� ��ġ �� Tolerance */
	double			m_dMountPosTolerance;

	/** TabMounter Mount ���� ���� ��ġ�� ���� �̵� �ӵ� ��*/
	double			m_dMountMoveSpeed;

	/***********************************************************************/
	/*       TabFeeder Section											   */
	/***********************************************************************/
	/** ���� Data ��� �ֱ� : ������ ȸ����ŭ�� Panel ���� �� Data ���. */
	UINT			m_uiMoldMaxCount;
	/** ���� Blow �ֱ�*/
	UINT			m_uiMoldBlowPeriod;
	/** ���� Blow �ð�*/
	UINT			m_uiMoldBlowTime;
	/** ���� �۾� �� ���� ��� ���� Ƚ��*/
	UINT			m_uiMoldLimitCountAfterCleaning; //presscleancount
													 /** ���� Bad-Tab IC ���� Ƚ��*/
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
	/** Carrier Tab Loading Retry Ƚ�� */
	// ���� ����.	int				m_iCarrierRetryCount;

	double			m_rgdCarrierHeadCenterX[DEF_MAX_TABCARRIER];
	double			m_rgdCarrierHeadCenterY[DEF_MAX_TABCARRIER];
	double			m_rgdRCarrierHeadCenterX[DEF_MAX_TABCARRIER];
	double			m_rgdRCarrierHeadCenterY[DEF_MAX_TABCARRIER];
	/**********************************************************************/
	/*       PanelAlinger Section								          */
	/**********************************************************************/
	/** �� ������ PanelAligner, TabMounter �̵� �Ÿ��� ������ ���� �̵�(���� �ӵ��� 1/5) �Ѵ�. */
	int				m_iDistance4DifVel;
	/** Panel Align Reference Camera ���� ��ġ ~ Active Panel Aligner Vacuum Pad �� �Ÿ� : X */
	double			m_dActiveAlignerPosX;
	/** Panel Align Reference Camera ���� ��ġ ~ Active Panel Aligner Vacuum Pad �� �Ÿ� : Y */
	double			m_dActiveAlignerPosY;
	/** Model Change �� ������ġ���� Panel Aligner Vacuum Pad Active Center ~ Passive Center �� �Ÿ� : Y */
	double			m_dOriginDistBetweenAligners;
	/** Fidu Mark �ڵ� �˻� ���� */
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

	/** Panel Aligner ȸ�� �߽ɿ��� Aligner Camera(Inspection Cam1, Inspection Cam8)�߽ɱ����� �Ÿ�(KKY)*/
	double			m_dAlignCam1_FixedPos_X;	//PanelAlignerȸ�� �߽ɿ��� Align�� Inspection
	double			m_dAlignCam1_FixedPos_Y;
	double			m_dAlignCam2_FixedPos_X;
	double			m_dAlignCam2_FixedPos_Y;
	double			m_dCamCarrier1_FixedPosX;	//Align�� Inspection Camera�� �߽��� PanelMark�� ��ġ�ϴ���ġ
	double			m_dCamCarrier2_FixedPosX;
	/** Panel Aligner �� �߽� �Ÿ� **/
	double			m_dDistBetweenAligners;

	/** Panel Aligner �߽ɰ� ȸ�� �Ǹ��� �߽ɰ� �Ÿ�(Gate������) **/
	double			m_dDist_AlignerToRotator_X;
	double			m_dDist_AlignerToRotator_Y;

	/** Inspect Camera�� �̵��Ҽ� �ִ� +/- Limit �Ÿ� **/
	double			m_dDistPosInspectCamera[DEF_MAX_GROUP];
	double			m_dDistNegInspectCamera[DEF_MAX_GROUP];

	//@KKY.130902.������ǥ �������� CameraCarrier�� �ʺ�____________
	double			m_dInspectCamFixedPosWidth;
	//@_____________________________________________________________

	/**********************************************************************/
	/*       Modle Change Distance Section						          */
	/**********************************************************************/
	/** Model Change �� ���� ��ġ ~ ���� ���� �۾� ���� ��ġ �� �Ÿ�
	*	 : Model �� ���� Model Change �� �̵� �Ÿ� ��� Data �� ���.
	*/

	//Panel Aligner �߽ɿ��� ���� ModelChange(Inspection Camera�߽�)���� �Ÿ�
	double			m_dModelChangeOriginDist[DEF_MAX_GROUP];

	/** Rear Gantry ���� ��ġ�κ��� Model Change �� ���������� �Ÿ�.
	*/
	//	double			m_dDistBetweenRGantryHomeToModelChangeHome;

	//Panel Aligner �߽ɿ��� ���� Gantry���� �Ÿ�(ToolMounter)
	//	double			m_dDistBetweenGantryHomeToModelChangeHome[DEF_MAX_GROUP];
	double			m_dGantryOriginDist[DEF_MAX_GROUP];

	double          m_dDistBadTabOriginDist[DEF_MAX_GROUP];


	/**********************************************************************/
	/*        TabAligner Section								          */
	/**********************************************************************/
	/** Panel Align Reference Camera ���� ��ġ ~ TabIC Pre-Align Camera 1 �� �Ÿ�
	*	 - Source : Y �� �Ÿ�, Gate_Front/Rear : X �� �Ÿ�
	*/
	double			m_dTabCameraDist1[DEF_MAX_WORKER_GROUP];
	/** TabIC Pre-Align Camera 1 ~ TabIC Pre-Align Camera 2 �� �Ÿ�
	*	 - Source : Y �� �Ÿ�, Gate_Front/Rear : X �� �Ÿ�
	*/
	double			m_dTabCameraDist2[DEF_MAX_WORKER_GROUP];
	/** TabIC Pre-Align Camera 2 ~ TabIC Pre-Align Camera 3 �� �Ÿ�
	*	 - Source : Y �� �Ÿ�, Gate_Front/Rear : X �� �Ÿ�
	*/
	double			m_dTabCameraDist3[DEF_MAX_WORKER_GROUP];
	/** TabIC Pre-Align Camera 3 ~ TabIC Pre-Align Camera 4 �� �Ÿ�
	*	 - Source : Y �� �Ÿ�, Gate_Front/Rear : X �� �Ÿ�
	*/
	double			m_dTabCameraDist4[DEF_MAX_WORKER_GROUP];

	/** Panel Lead �� TabIC Lead ���� �� ���� �� */
	double			m_dTabICLeadCheck;

	/** TabAligner �� 1 Tab Prealign �� ���� �����̴� ���� Ƚ�� */
	int				m_iTabAlignerOperationLimit;

	/** TabAligner �۾� �� Tab Mark �ν� ��ġ ��� ���� (Aligner �� ����) */
	double			m_dTabAlignerTolerance;

	/** TabAligner ��� ���� ����. */
	BOOL			m_rgbUseTabAligner[DEF_MAX_TABALIGNER];

	/** TabIC Aligner �� Base Offset 1,2 : [Source - X1,X2] [Gate - Y1,Y2] */
	double			m_rgdTabAlignerBase1[DEF_MAX_TABALIGNER];
	double			m_rgdTabAlignerBase2[DEF_MAX_TABALIGNER];

	/** TabIC Aligner �� Etc. Offset 1,2 : [Source - X1,X2] [Gate - Y1,Y2] */
	double			m_rgdTabAlignerEtcOffset1[DEF_MAX_TABALIGNER];
	double			m_rgdTabAlignerEtcOffset2[DEF_MAX_TABALIGNER];

	/************************************************************************/
	/*    Camera Prism Section										        */
	/************************************************************************/
	/** Dual View Camera Prism ��� ��, Vision ȭ���� ����ϴ� View ���� �߽� �Ÿ� */
	///double			m_rgdDistBetweenPrismViewCenter[DEF_MAX_CAMERA_NO]; 
	//	double			m_rgdInspectionCamDistBetweenPrismViewCenter[DEF_MAX_INSPECTION_CAMERA]; ///4
	//	double			m_rgdPreAlignCamDistBetweenPrismViewCenter[DEF_MAX_GROUP]; ///2	

	double			m_rgdInspectionCamDistViewCenter_Expand[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER]; //Ȯ��� ī�޶� �߽ɰ� �Ÿ�
	double			m_rgdInspectionCamDistViewCenter_Shrink[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER]; //��ҽ� ī�޶� �߽ɰ� �Ÿ�
	double			m_rgdInspectionCamDist_CenterToCenter[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];//Ȯ�� ī�޶� �߽� - ��� ī�޶� �߽�
																										 //	double			m_rgdInspectionCamVariableDist[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER];

	double			m_dData_CameraExpand[DEF_MAX_GROUP][DEF_MAX_CAM_EXPAND];//Ȯ�� ī�޶� �߽� - ��� ī�޶� �߽�

	double			m_rgdPreAlignCamDistViewCenter_Expand[DEF_MAX_GROUP]; //Ȯ��� ī�޶� �߽ɰ� �Ÿ�
	double			m_rgdPreAlignCamDistViewCenter_Shrink[DEF_MAX_GROUP]; //��ҽ� ī�޶� �߽ɰ� �Ÿ�
	double			m_rgdPreAlignCamDist_CenterToCenter[DEF_MAX_GROUP];//Ȯ�� ī�޶� �߽� - ��� ī�޶� �߽�
	double			m_rgdPrealignCamVariableDist[DEF_MAX_GROUP];

	/************************************************************************/
	/*        Log Section											        */
	/************************************************************************/
	/** ���� �޼��� �α� ���� */
	BOOL			m_bLogLevelError;
	/** Monitoring �޼��� �α� ���� */
	BOOL			m_bLogLevelMonitoring;
	/** Normal �޼��� �α� ���� */
	BOOL			m_bLogLevelNormal;
	/** Tact Time �α� ���� */
	BOOL			m_bLogLevelTactTime;
	//170427 JSH.s
	/** Eq Tact Time �α� ���� */
	BOOL			m_bLogLevelEqTactTime;
	//170427 JSH.e
	/** �α� ���� ���� �Ⱓ */
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
	BOOL m_nPressActionMode;//0->Pin����� Punching, 1->���õ���
	BOOL m_nSprocket1ClutchMode;//0->Feeding�� ClutchOff, 1->Punching�� ClutchOff, 2->Clutch �̻��
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
