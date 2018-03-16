// MPlatformOlbSystem.h: interface for the MPlatformOlbSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPlatformOlbSYSTEM_H__4AB8E2E4_F33D_4945_B124_7AA2B0045CF4__INCLUDED_)
#define AFX_MPlatformOlbSYSTEM_H__4AB8E2E4_F33D_4945_B124_7AA2B0045CF4__INCLUDED_

#include "DefSystem.h"
#include "DefActuatorPreBonder.h"
#include "DefAxisPreBonder.h"

#include "MSystemData.h"
#include "Common.h"
#include "MLCNet.h"

#define	DEF_SYSTEM_DATA_FILE_NAME						"System.dat"
#define	DEF_SYSTEM_ACTUATOR_PARAMETER_FILE_NAME			"Actuator.dat"
#define	DEF_SYSTEM_MOTION_PARAMETER_FILE_NAME			"Motion.dat"
#define	DEF_SYSTEM_POSITION_FILE_NAME					"Position.dat"
#define	DEF_SYSTEM_OFFSET_FILE_NAME						"PositionOffset.dat"
#define	DEF_SYSTEM_PANEL_DATA_FILE_NAME					"Panel.dat"
#define	DEF_SYSTEM_PRE_DATA_FILE_NAME					"PreBonder.dat"
#define	DEF_SYSTEM_TAB_DATA_FILE_NAME					"TabData.dat"
#define DEF_SYSTEM_TAB_OFFSET_FILE_NAME					"TabOffset.dat"
#define DEF_SYSTEM_TAB_OFFSET_FILE_NAME_REAR			"TabOffset_Rear.dat"
#define	DEF_SYSTEM_DISP_MESSAGE_FILE					"DisplayMsgKor.dat"
#define	DEF_SYSTEM_PRODUCT_INFO_FILE					"Product.dat"
#define	DEF_SYSTEM_VISION_CALIBRATION_DATA_FILE_NAME	"Calibration.dat"
#define	DEF_SYSTEM_ERROR_MESSAGE_PATH					".\\Info"
#define	DEF_SYSTEM_TOWERLAMP_DATA_FILE_NAME				"TowerLamp.dat"

#define DEF_SYSTEM_LOG_TABIC_LOSS_PATH_FRONT			"..\\Log\\TabICLossF"
#define DEF_SYSTEM_LOG_TABIC_LOSS_PATH_REAR				"..\\Log\\TabICLossR"
#define DEF_SYSTEM_DB_TABIC_ORG_PATH					".\\DataBase\\TabICLoss\\TabLossOrg.mdb"

#	ifdef DEF_SOURCE_SYSTEM
	#define	DEF_SYSTEM_TEACHING_INFO_FILE_NAME		"PosInfoPreBonder_Source.inf"
	#define	DEF_SYSTEM_IO_MAP_FILE_NAME				"IO_Source_3rd.dat"
#	else
	#define	DEF_SYSTEM_TEACHING_INFO_FILE_NAME		"PosInfoPreBonder_Gate.inf"
	#if (DEF_LINE_NO == DEF_LINE_NO_GROUP_F_1ST)
		#define	DEF_SYSTEM_IO_MAP_FILE_NAME				"IO_Gate_1st.dat"
	#else if (DEF_LINE_NO == DEF_LINE_NO_GROUP_F_2ND || DEF_LINE_NO == DEF_LINE_NO_GROUP_F_3RD || DEF_LINE_NO == DEF_LINE_NO_GROUP_F_4TH )
		#define	DEF_SYSTEM_IO_MAP_FILE_NAME				"IO_Gate_2nd.dat"
	#endif
#	endif

#define DEF_AUTOALIGN_DATA_FILE_NAME					"AlignData.dat" //SJ_YYK 150318 Add..	

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int ERR_DATA_READ_FAILURE		= -1;
const int ERR_SKIP_ERROR_DISPLAY	= -2;

class CBtnEnh;
class MPanelData;
class MPreBonderData;
class MTabData;
class MProductInfoData;
class MSystemInfo;
class MTeachingInfo;
class MTeachingPosData;
class MTabOffsetData;
//20110223 SJ_HJG
class MTowerLampData;
// jdy sesl 081231	class MErrorMsg;
class MProcessData;
class MWorkSchedule;
class IAxisData;
class ICommon;
class MAutoAlignData; //SJ_YYK 150318 Add..


// Hardware Layer
class IIO;
class IMotionLib;
class ISercosSetup;
class MRS_NMCSetup;			//161024 Add...
class IMotionLibNMC;		//161024 Add...
class MCANSetup;
class IVision;
class ICylinder;
class IVacuum;
class IAxis;
class ISercosMultiAxis;
class IRSNMCMultiAxis;		//161024 Add...
class MNetH;
class IComThread;
class ILighting;
class IInverter;
class MLCNet;

// Mechanical Layer
class MPanelAligner;
class MPanelTransfer;
//class MRollerConveyor;
class IOpPanel;
class MOpPanel;
class MTabCarrier;
class MTabFeeder;
class MCameraCarrier;
class MTabMounter;
class MTabAligner;
class MModelChange;
class MRFID;
class MHandler; //SJ_YYK 161104 Add..

// Control Layer
class MManageOpPanel;
class MVisionCalibration;
class MCtrlPanelAligner;
class MCtrlTabFeeder;
class MCtrlTabCarrier;
class MCtrlTabMounter;
class MCtrlInspectionCamera;
class MCtrlHandler; //SJ_YYK 161104 Add..

// Process Layer
class MTrsAutoManager;
class MTrsJog;
class MTrsPanelAligner;
class MTrsPanelTransfer;
class MTrsPanelTransferCenter;
class MTrsPanelTransferOut;
class MTrsLoadConveyor;
class MTrsUnloadConveyor;
class MTrsTabCarrier;
class MTrsTabFeeder;
class MTrsTabMounter;
class MTrsInspectionCamera;
class CDBTabLoss;	//@130214
class MTrsTHandler; //SJ_YYK 161104 Add..


class MPos_XYT;

class MPlatformOlbSystem  
{
public:
	MPlatformOlbSystem();
	virtual ~MPlatformOlbSystem();

private:
	// System Component Information
	MPanelData*				m_pObjPanelData;
	MPreBonderData*			m_pObjPreBonderData;
	MTabData*				m_pObjTabData;
	MTabOffsetData*			m_pObjTabOffsetData;
	MTabOffsetData*			m_pObjTabOffsetData_Rear;
	MProductInfoData*		m_pObjProductInfoData;
	MSystemData*			m_pObjSystemData;
	MSystemInfo*			m_pObjSystemInfo;
	MTeachingInfo*			m_pObjTeachingInfo;
	//20110223 SJ_HJG
	MTowerLampData*			m_pObjTowerLampData;
// jdy sesl 081231	MErrorMsg*				m_pObjErrorMsg;
	MProcessData*			m_pObjProcessData;
	MWorkSchedule*			m_pObjWorkSchedule[DEF_MAX_WORKER_GROUP];
	MWorkSchedule*			m_pObjWorkScheduleSecond[DEF_MAX_WORKER_GROUP];	//Source에서만
	//101004.KKY.자동운전중 신규모델 등록_____
	MPanelData*				m_pObjTmpPanelData;
	MPreBonderData*			m_pObjTmpPreBonderData;
	MTabData*				m_pObjTmpTabData;
	//________________________________________

	MAutoAlignData*			m_pObjAutoAlignData; //SJ_YYK 150318 Add..

	CDBTabLoss*				m_pObjDBTabLoss[DEF_MAX_WORKER_GROUP];	//@130214

	// Hardware Layer
	IIO*					m_pIO;
	IMotionLib*				m_pMMCMotionLib;
	ISercosSetup*			m_pSercosSetup;
	MRS_NMCSetup*			m_pRS_NMCSetup;			//161024 Add...
	MCANSetup*				m_pCANSetup;
	IVision*				m_pVision;
	ICommon*				m_pCamera[DEF_MAX_CAMERA_NO];
	IComThread*				m_pSerial[DEF_MAX_SERIAL];
	ICylinder*				m_pCylinder[DEF_CYL_MAX_CYLINDER];
	IVacuum*				m_pVacuum[DEF_VAC_MAX_VACUUM];

	IAxis*					m_pMotion[DEF_AXIS_MAX_AXIS];
	ISercosMultiAxis*		m_pMultiSercosMotion[DEF_MULTI_AXIS_MAX_AXIS];
	IMotionLibNMC*			m_pMotionLib;									//161024 Add...
	IRSNMCMultiAxis*		m_pMultiNMCMotion[DEF_MULTI_AXIS_MAX_AXIS];		//161024 Add...

	MNetH*					m_pMelsecIF;
	MNetH*					m_pMelsecIF2;
	ILighting*				m_pLighting[DEF_MAX_LIGHTING];
	IInverter*				m_pInverter[DEF_MAX_CONVEYOR];
	
	// Mechanical Layer
	MOpPanel*				m_pOpPanel;
	MPanelAligner*			m_pPanelAligner;
	MPanelTransfer*			m_pPanelTransferCenter;
	MPanelTransfer*			m_pPanelTransfer;
	MPanelTransfer*			m_pPanelTransferOut;
	MTabCarrier*			m_pTabCarrier[DEF_MAX_TABCARRIER];
	MTabCarrier*			m_pRTabCarrier[DEF_MAX_TABCARRIER];
	MTabFeeder*				m_pTabFeeder[DEF_MAX_TABFEEDER];
	MCameraCarrier*			m_pCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	MCameraCarrier*			m_pRCameraCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTabMounter*			m_pTabMounter[DEF_MAX_TABMOUNTER];
	MTabMounter*			m_pRTabMounter[DEF_MAX_TABMOUNTER];
	MModelChange*			m_pModelChange[DEF_MAX_WORKER_GROUP];
	MLCNet*					m_pLCNet;
	MRFID*					m_pRFID[DEF_MAX_RFID];
	MHandler*				m_pTurnHander; //SJ_YYK 161104 Add...
//===================================================================

//===================================================================
	// Control layer
	MManageOpPanel*			m_pManageOpPanel;
	MVisionCalibration*		m_pVisionCalibration;
	MCtrlPanelAligner*		m_pCtrlPanelAligner;
	MCtrlTabCarrier*		m_pCtrlTabCarrier[DEF_MAX_TABCARRIER];
	MCtrlTabCarrier*		m_pCtrlRTabCarrier[DEF_MAX_TABCARRIER];
	MCtrlTabFeeder*			m_pCtrlTabFeeder[DEF_MAX_TABFEEDER];
	MCtrlTabMounter*		m_pCtrlTabMounter[DEF_MAX_TABMOUNTER];
	MCtrlTabMounter*		m_pCtrlRTabMounter[DEF_MAX_TABMOUNTER];
	MCtrlInspectionCamera*	m_pCtrlInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MCtrlInspectionCamera*	m_pCtrlRInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MCtrlHandler*			m_pCtrlTHandler; //SJ_YYK 161104 Add..

	// Process Layer
	MTrsAutoManager*				m_pTrsAutoManager;
	MTrsJog*						m_pTrsJog;
	MTrsPanelAligner*				m_pTrsPanelAligner;
	MTrsPanelTransferCenter*		m_pTrsPanelTransferCenter;
	MTrsPanelTransfer*				m_pTrsPanelTransfer;
	MTrsPanelTransferOut*			m_pTrsPanelTransferOut;
	MTrsTabCarrier*					m_pTrsTabCarrier[DEF_MAX_TABCARRIER];
	MTrsTabCarrier*					m_pTrsRTabCarrier[DEF_MAX_TABCARRIER];
	MTrsTabFeeder*					m_pTrsTabFeeder[DEF_MAX_TABFEEDER];
	MTrsTabMounter*					m_pTrsTabMounter[DEF_MAX_TABMOUNTER];
	MTrsTabMounter*					m_pTrsRTabMounter[DEF_MAX_TABMOUNTER];
	MTrsInspectionCamera*			m_pTrsInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTrsInspectionCamera*			m_pTrsRInspectionCamera[DEF_MAX_INSPECTION_CAM_CARRIER];
	MTrsTHandler*					m_pTrsTHandler; //SJ_YYK 161104 Add..
//===================================================================
//===================================================================

	EUserGroup				m_eCurrentUserGroup;

	//20121004 SJ_HJG 횡전개
	double					m_dTabEndMarkDistance[2];

	//@____________________
	MTabData*				m_pTabDataSourceFirst;
	MTabData*				m_pTabDataSourceSecond;
	//@____________________

	double					m_dInsCamFrontFiduPos; 	//SJ_YYK 150109 Add..

private:
	void registerComponentInstance(int iObjectID, ICommon* plnkComp);

	// Hardware Layer
	int createBasicHWComponents();
	int	createCylinderComponents();
	int	createVacuumComponents();
	int	createMotionComponents();
	int createLightingComponents();
	int	createInverterComponents();

	// Mechanical Layer
	int createOpPanelComponents();
	int	createPanelAlignerComponents();
#ifdef DEF_USE_TRANSFER_CENTER
	int createPanelTransferCenterComponents();
#endif 
	int	createPanelTransferComponents();
	int createPanelTransferOutComponents();
	int createTabCarrierComponents();
	int createRTabCarrierComponents();
	int createTabFeederComponents();
	int createTabMounterComponents();
	int createCameraCarrierComponents();
	int createModelChangeComponents();
	int createRFIDComponents();
	int createLCNetComponent();
#ifdef DEF_USE_TURN_HANDLER
	int createTurnHandlerComponents(); //SJ_YYK 161104 Add..
#endif


	// Control Layer
	int	createVisionCalibrationComponents();
	int	createManageOpPanelComponents();
	int createCtrlPanelAlignerComponents();
	int createCtrlTabCarrierComponents();
	int createCtrlRTabCarrierComponents();
	int createCtrlTabFeederComponents();
	int createCtrlTabMounterComponents();
	int createCtrlInspectionCameraComponents();
	
#ifdef DEF_USE_TURN_HANDLER
	int createCtrlTHandlerComponents(); //SJ_YYK 161104 Add.
#endif

	// Process Layer
	int createTrsAutoManageComponents();
	int createTrsJogComponents();
	int createTrsPanelAlignerComponents();
#ifdef DEF_USE_TRANSFER_CENTER
	int createTrsPanelTransferCenterComponents();
#endif 
	int createTrsPanelTransferComponents();
	int createTrsPanelTransferOutComponents();
	int createTrsTabCarrierComponents();
	int createTrsRTabCarrierComponents();
	int createTrsTabFeederComponents();
	int createTrsTabMounterComponents();
	int createTrsInspectionCameraComponents();
	
#ifdef DEF_USE_TURN_HANDLER
	int createTrsTHandlerComponents(); //SJ_YYK 161104 Add..
#endif

	void startThreads();
	void initSystem();
	int initSercos();	

	CString	getCylIOAddr(int iErrorUnit);
	CString	getVacIOAddr(int iErrorUnit);
	CString	Convert2IOString(int iIOConstance);

	short changeWorkingSetSize();

	int removeTabMounterTabIC(int iWorkGroupNo);
	//101004.KKY.자동운전중 신규모델 등록_____
	int						CreateTmpPanelData(CString strModelName);
	int						CreateTmpPreBonderData(CString strModelName);
	int						CreateTmpTabData(CString strModelName);
	//________________________________________

public:
	MSystemInfo*			GetSystemInfo();
	MSystemData*			GetSystemData();
	MPanelData*				GetPanelData();		
	MPreBonderData*			GetPreBonderData();
	MTabData*				GetTabData();
	MProductInfoData*		GetProductInfoData();
	MTabOffsetData*			GetTabOffsetData();
	MTabOffsetData*			GetTabOffsetData_Rear();
	MProcessData*			GetProcessData();
	MWorkSchedule*			GetWorkSchedule(int iWorkGroup);
	MWorkSchedule*			GetWorkScheduleSecond(int iWorkGroup);
	//20110223 SJ_HJG
	MTowerLampData*			GetTowerLampData();

	CDBTabLoss*				GetDBTabLoss(int iWorkGroup);	//@130214

	MAutoAlignData*			GetAutoAlignData(); //SJ_YYK 150318 Add..

	// Hardware Layer
	IIO*					GetIOComponent();
	IVision*				GetVisionComponent();
	IComThread*				GetSerialComponent(int iSerialIndex);
	MNetH*					GetMelsecComponent();
	MNetH*					GetMelsecComponent2();
	ICylinder*				GetCylinderComponent(int iIndex);
	IVacuum*				GetVacuumComponent(int iIndex);
	//Sercos 추가 
	IAxis*					GetMotionComponent(int iIndex);
	ISercosMultiAxis*		GetSercosMultiMotionComponent(int iIndex);
	IRSNMCMultiAxis*		GetNMCMultiMotionComponent(int iIndex)		{ return m_pMultiNMCMotion[iIndex];	}	//161024 Add...
	ISercosSetup*			GetSercosSetupComponent();
	MRS_NMCSetup*			GetRSNMCSetupComponent()	{ return m_pRS_NMCSetup; }		//161024 Add...
	MCANSetup*				GetCANSetupComponent();
	ILighting*				GetLightingComponent(int iIndex);
	IInverter*				GetInverterComponent(int iIndex);

	// Mechanical Layer
	IOpPanel*				GetOpPanelComponent();
	MPanelAligner*			GetPanelAlignerComponent();
#ifdef DEF_USE_TRANSFER_CENTER
	MPanelTransfer*			GetPanelTransferCenterComponent();
#endif 
	MPanelTransfer*			GetPanelTransferComponent();
	MPanelTransfer*			GetPanelTransferOutComponent();
	MTabCarrier*			GetTabCarrierComponent(int iIndex);
	MTabCarrier*			GetRTabCarrierComponent(int iIndex);
	MTabFeeder*				GetTabFeederComponent(int iIndex);
	MTabMounter*			GetTabMounterComponent(int iIndex);
	MTabMounter*			GetRTabMounterComponent(int iIndex);
	MCameraCarrier*			GetCameraCarrierComponent(int iIndex);
	MCameraCarrier*			GetRCameraCarrierComponent(int iIndex);
	MRFID*					GetRFIDComponent(int iIndex);
	MLCNet*					GetLCNetComponent();
	MModelChange*			GetModelChangeComponent(int iGroupID);
#ifdef DEF_USE_TURN_HANDLER
	MHandler*				GetTurnHandlerComponent(); //SJ_YYK 161104 Add..
#endif

//===================================================================
//===================================================================

	// Control Layer
	MManageOpPanel*			GetManageOpPanelComponent();
	MVisionCalibration*		GetVisionCalibrationComponent();
	MCtrlPanelAligner*		GetCtrlPanelAlignerComponent();
	MCtrlTabCarrier*		GetCtrlTabCarrierComponent(int iIndex);
	MCtrlTabCarrier*		GetCtrlRTabCarrierComponent(int iIndex);
	MCtrlTabFeeder*			GetCtrlTabFeederComponent(int iIndex);
	MCtrlTabMounter*		GetCtrlTabMounterComponent(int iIndex);
	MCtrlTabMounter*		GetCtrlRTabMounterComponent(int iIndex);
	MCtrlInspectionCamera*	GetCtrlInspectionCameraComponent(int iIndex);
	MCtrlInspectionCamera*	GetCtrlRInspectionCameraComponent(int iIndex);
#ifdef DEF_USE_TURN_HANDLER
	MCtrlHandler*			GetCtrlTHandlerComponent(); //SJ_YYK 161104 Add..
#endif

	// Process Layer
	MTrsAutoManager*			GetTrsAutoManagerComponent();
	MTrsPanelAligner*			GetTrsPanelAlignerComponent();
#ifdef DEF_USE_TRANSFER_CENTER
	MTrsPanelTransferCenter*	GetTrsPanelTransferCenterComponent();
#endif
	MTrsPanelTransfer*			GetTrsPanelTransferComponent();
	MTrsPanelTransferOut*		GetTrsPanelTransferOutComponent();
	MTrsTabCarrier*				GetTrsTabCarrierComponent(int iIndex);
	MTrsTabCarrier*				GetTrsRTabCarrierComponent(int iIndex);
	MTrsTabFeeder*				GetTrsTabFeederComponent(int iIndex);
	MTrsTabMounter*				GetTrsTabMounterComponent(int iIndex);
	MTrsTabMounter*				GetTrsRTabMounterComponent(int iIndex);
	MTrsInspectionCamera*		GetTrsInspectionCameraComponent(int iIndex);
	MTrsInspectionCamera*		GetTrsRInspectionCameraComponent(int iIndex);
#ifdef DEF_USE_TURN_HANDLER
	MTrsTHandler*				GetTrsTHandlerComponent(); //SJ_YYK 161104 Add..
#endif


	int						Initialize();
	int						GetModelNameList(CString *strModelList, int *iModelCount);
	int						DeleteModel(LPCTSTR pStrModel);
	CString					GetCurrentModelName();
	//111201.kms________
	int						ChangeModel(CString strModelName, BOOL m_NSMCAutoChange = FALSE);
	//__________________
	int						CreateModel(LPCTSTR dest, LPCTSTR src);
	void					ReportModel();
	void					SetLogAttribute(BYTE ucLevel, int iLogKeepingDays);
	CString					GetErrorMessage(int iErrorCode, int iLanguageType = LANGUAGE_SELECTED, BOOL bErrorReport = FALSE);//170930 JSH
	CString					GetObjectName(int iErrorCode);
	
	const EUserGroup		GetCurrentUserGroup() const;
	void					SetCurrentUserGroup(EUserGroup eUserGroup);

	BOOL					IsSkipAxis(int iAxisID);
	
	int						LightOn(int iCamNo);
	int						LightDefult(int iCamNo);
	int						LightOff(int iCamNo);
	int						SetLightValue(int iCamNo);

	int						GetCalibrationUnitNo(int iCamNo, int iInatanceID);

	CString					GetOpenDoorPosition(void);

	// 2011.12.14 KWC -----
	//Chang_
	int			GetDeviceIDList(CString strModelDataDir, CString *strDeviceIDList, int *iDeviceIDCount);
	//Chang_
	int			DeleteDeviceID(LPCTSTR pStrDeviceIDPath);
	// --------------------

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Manual Operation Functions

	// Remove TabIC : TabMounter, TabCarrier 에 있는 모든 TabIC 를 쓰레기통에 버림.
	int						RemoveTabIC(int iGroupNo, BOOL bRemoveTabCarrierTabIC = FALSE);

	// TabCarrier TabIC Loading
	int						LoadTabCarrierTabIC(int iGroupNo, int iCarrierVacNo);

	// TabMounter TabIC Loading
	int						LoadTabMounterTabIC(int iGroupNo);

	// TabIC Prealign
	int						PrealignTabIC(int iGroupNo, int iWorkTabNo, BOOL bSkipAfterMove = FALSE, BOOL bUseBaseLineOnly = FALSE);

	// Get X Offset Prealign TabIC // 2009.07.14 CYJ
	int						GetXOffsetPrealignTabIC(int iGroupNo, int iWorkTabNo, BOOL bSkipAfterMove = FALSE);

	// jdy sesl
	const BOOL IsNoWorkInAutoRunMode() const;

	BOOL					m_bSystemInitialized;

	int						Get_CarrierToTabMounter(int iWorkGroup, int iCarrierNo);
	int						Get_CarrierToTabMounterOtherGroup(int iWorkGroup, int iCarrierNo);
	int						Get_ToolToCarrier(int iToolNo);
	int						Get_ToolToCarrierOtherGroup(int iToolNo);
	int						Get_ToolInstanceNo(int iWorkGroup, int iWorkToolNo);

	//KKY____________________
	int						MoveAllFrontTabCarrier(MPos_XYT tgPos[DEF_MAX_TABCARRIER], int iPosID);

	int						MoveAllRearTabCarrier(MPos_XYT tgPos[DEF_MAX_TABCARRIER], int iPosID);

	int						MoveAllTabMounter(double tgPos[DEF_MOUNTER_WORK_PER_GROUP], double tgPosR[DEF_MOUNTER_WORK_PER_GROUP], int iPosID);

	int						MoveAllFrontTabMounter(double tgPos[DEF_MOUNTER_WORK_PER_GROUP], int iPosID);

	int						MoveAllRearTabMounter(double tgPos[DEF_MOUNTER_WORK_PER_GROUP], int iPosID);

	int						MoveAllTabMounterZ(int iWorkGroup, int iPosID);

	int						MoveAllGroupCameraCarrier(int iWorkGroup, double tgPos[DEF_MAX_INSPECTION_CAM_CARRIER]);
	int						MoveAllCameraCarrier(double tgFPos[DEF_MAX_INSPECTION_CAM_CARRIER], double tgRPos[DEF_MAX_INSPECTION_CAM_CARRIER]);//Gate에서만
	int						MoveInspectCameraPanelAlignPos();
	//_______________________

	//101004.KKY.자동운전중 신규모델 등록_____
	int						CreateTmpModelData(CString strModelName);
	MPanelData*				GetTmpPanelData();
	MPreBonderData*			GetTmpPreBonderData();
	MTabData*				GetTmpTabData();
	//________________________________________

	//PanelAligner회전중심에서 Panel끝단까지 거리(+)
	double					GetDistance_StageCenterToPanelEdge_X();
	//PanelAligner회전중심에서 Panel Mark까지 거리(+)
	double					GetDistance_StageCenterToPanelMark_X();
	//PanelAligner회전중심에서 Panel중심까지 거리(+) ---> 편심량
	double					GetEccentricDist_StageToPanel_X();
	//PanelAligner Plate중심에서 Panel Mark까지 거리(+)
	double					GetDistance_StagePlateCenterToPanelMark_X();

	//TabAligner회전중심에서 TabICMark까지 거리
	double					GetDistance_TabAlignerCenterToTabMarkX();
	double					GetDistance_TabAlignerCenterToTabMarkY();

	//FiduMark에서 지정 Tab의 중심까지의 거리 구하기
	double					GetXDistance_FiduMarkToTabCenter(EWorkingSide eWorkSide, int iTabNo);
	//FiduMark에서 지정 Tab의 LeftMark까지의 거리 구하기
	double					GetXDistance_FiduMarkToTabMarkL(EWorkingSide eWorkSide, int iTabNo);
	//FiduMark에서 지정 Tab의 RightMark까지의 거리 구하기
	double					GetXDistance_FiduMarkToTabMarkR(EWorkingSide eWorkSide, int iTabNo);
	//FirstTab 중심에서 SecondTab 중심 까지의 거리 구하기
	double					GetXDistance_TabCenterToCenter(EWorkingSide eWorkSide, int iStartTabNo, int iLastTabNo);
	//부상길이와 작업변 길이의 차이를 구한다.
	double					GetDifferLen_PlateToPanelLen();

	//SJ_YYK 150109 Add...
	double					GetInsCamFrontWorkFiduPos(){return m_dInsCamFrontFiduPos;}
	void					SetInsCamFrontWorkFiduPos(double dVal) { m_dInsCamFrontFiduPos = dVal;}
	//________________________*/

	BOOL					GetUseFlagPanel(int iWorkGroup);
	int						GetPanelAlignGroup();

	BOOL					IsExistRemoveTab_TabMounter(int iWorkGroup);
	BOOL					IsExistRemoveTab_TabCarrier(int iWorkGroup);

	BOOL					IsAllDoorLock(int iWorkGroup);	//20170830 KDW ADD
	BOOL					IsMaterialDoorLock(int iWorkGroup);	//20170830 KDW ADD	
	//20121004 SJ_HJG 횡전개
	void					SetTabEndMarkDistance(int iWorkGroupNo, double dTabEndMarkDistance);
	double					GetTabEndMarkDistance(int iWorkGroupNo);

	void					AssignWorkScheduleData();

	//161024 Add...
	/************************************************************************/
	/*	축 종류 반환                                                        */
	/************************************************************************/
	BOOL		IsFunctionModuleSlaveAxis(int iAxis);
	BOOL		IsFunctionModuleAxis(int iAxis);
	int			InitRS_NMCBoard();			//RS 이더캣 모션 초기화

	int			SetGearIn(int iAxisID);
	int			SetGearOut(int iAxisID);
	int			IsGearIn(int iAxisID);
	int			IsGearOut(int iAxisID);

};
#endif // !defined(AFX_MPlatformOlbSYSTEM_H__4AB8E2E4_F33D_4945_B124_7AA2B0045CF4__INCLUDED_)
