// MPlatformOlbSystem.cpp: implementation of the MPlatformOlbSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPlatformOlb.h"
#include "MPlatformOlbSystem.h"
#include <Tlhelp32.h>	// ACS Spii plus Kill Reload
#include <math.h>	// ACS Spii plus Kill Reload
#include "MFileUtil.h"

// System
#include "MPanelData.h"
#include "MProductInfoData.h"
#include "MSystemInfo.h"
#include "MTeachingPosData.h"
#include "MProcessData.h"
#include "MPreBonderData.h"
#include "MTabData.h"
#include "MTabOffsetData.h"
#include "DefIOAddrPreBonder.h"
//20110223 SJ_HJG
#include "MTowerLampData.h"
#include "DBTabLoss.h"
#include "MAutoAlignData.h" //SJ_YYK 150318 Add..

// jdy sesl 081231	#include "MErrorMsg.h"

// Hardware Layer
#include "ACSC.h"
#include "IIO.h"
#include "MMMCLib.h"
#include "MSercosSetup.h"
#include "MCANSetup.h"
#include "MVision.h"
#include "MCylinder.h"
#include "MVacuum.h"
#include "MComThread.h"
#include "MAxis.h"
#include "MCANAxis.h"
#include "MSercosAxis.h"
#include "MSercosMultiAxis.h"
#include "MACSAxis.h"
#include "MAxisData.h"
#include "MNetH.h"
#include "MHilscherDnet.h"
#include "MLighting.h"
#include "MInverter.h"
#include "MLighting_ONI.h"
#include "MRS_NMCLib.h"			//161024 Add...
#include "MRS_NMC_Setup.h"		//161024 Add...
#include "MRSNMCAxis.h"			//161024 Add...
#include "MRSNMCMultiAxis.h"	//161024 Add...
#include "IRSNMCMultiAxis.h"	//161024 Add...

// Mechanical Layer
#include "MOpPanel.h"
#include "MPanelAligner.h"
#include "MPanelTransfer.h"
//#include "MRollerConveyor.h"
#include "MTabCarrier.h"
#include "MTabFeeder.h"
#include "MTabMounter.h"
#include "MTabAligner.h"
#include "MCameraCarrier.h"
#include "MModelChange.h"
#include "MRFID.h"
#include "MLCNet.h"
#include "MHandler.h"

// Control Layer
#include "MManageOpPanel.h"
#include "MVisionCalibration.h"
#include "MCtrlPanelAligner.h"
#include "MCtrlTabCarrier.h"
#include "MCtrlTabFeeder.h"
#include "MCtrlTabMounter.h"
#include "MCtrlInspectionCamera.h"
#include "MCtrlHandler.h"

// Process Layer
#include "MTrsAutoManager.h"
#include "MTrsJog.h"
#include "MTrsPanelAligner.h"
#include "MTrsPanelTransfer.h"
#include "MTrsPanelTransferCenter.h"
#include "MTrsPanelTransferOut.h"
#include "MTrsLoadConveyor.h"
#include "MTrsUnloadConveyor.h"
#include "MTrsTabCarrier.h"
#include "MTrsTabFeeder.h"
#include "MTrsTabMounter.h"
#include "MTrsInspectionCamera.h"
#include "MTrsTHandler.h"

#include "common.h"

//SESL_LKH_090713 : 프로그램 시작 알림 Message window 추가
#include "MyMsgWnd.h"

const int ERR_SYSTEM_SUCCESS				= 0;

const int ERR_SYSTEM_MODEL_CREATE_FAIL		= 4;
const int ERR_SYSTEM_MODEL_DELETE_FAIL		= 5;
const int ERR_SYSTEM_MAX_COUNT_EXCEED_LIMIT	= 6;

// 2011.12.14 KWC ---------
//Chang_
const int	ERR_ACF_SYSTEM_DEVICEID_CREATE_FAIL				= 7;
const int	ERR_ACF_SYSTEM_DEVICEID_DELETE_FAIL				= 8;
// -----------------------

extern MPlatformOlbSystem	MOlbSystemPre;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// jdy sesl 081230
extern CRITICAL_SECTION g_csError;

extern unsigned int g_uiLanguageSelect;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MPlatformOlbSystem::MPlatformOlbSystem()
{
	int i = 0;
	m_eCurrentUserGroup = OPERATOR_GROUP;

	// System
	m_pObjSystemInfo = NULL;
	m_pObjSystemData = NULL;
	m_pObjPanelData = NULL;
	m_pObjPreBonderData = NULL;
	m_pObjTabData = NULL;
	m_pObjTabOffsetData = NULL;
	m_pObjTabOffsetData_Rear = NULL;
	m_pObjProductInfoData = NULL;
	m_pObjTowerLampData = NULL;
	m_pObjTeachingInfo = NULL;
	//자동운전중 신규모델 등록_____
	m_pObjTmpPanelData = NULL;
	m_pObjTmpPreBonderData = NULL;
	m_pObjTmpTabData = NULL;
	//_____________________________
	m_pObjAutoAlignData = NULL; //SJ_YYK 150318 Add..

	for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
		m_pObjDBTabLoss[i] = NULL;	//@130214

	m_pTabDataSourceFirst = NULL;
	m_pTabDataSourceSecond = NULL;

	// Hardware Layer
	m_pIO = NULL;
	m_pMMCMotionLib = NULL;
	m_pSercosSetup = NULL;
	m_pCANSetup = NULL;
	
	m_pVision = NULL;
	for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
		m_pCamera[i] = NULL;

	m_pMelsecIF = NULL;
	m_pMelsecIF2 = NULL;

	for (int i = 0; i < DEF_MAX_SERIAL; i++)
		m_pSerial[i] = NULL;
	
	for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
		m_pCylinder[i] = NULL;

	for (int i = 0; i < DEF_VAC_MAX_VACUUM; i++)
		m_pVacuum[i] = NULL;

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
		m_pMotion[i] = NULL;

	for (int i=0;i<DEF_MULTI_AXIS_MAX_AXIS;i++)
		//161024 Modify...
		//m_pMultiSercosMotion[i]			= NULL;
		m_pMultiNMCMotion[i]		= NULL;		

	for (int i = 0; i < DEF_MAX_CONVEYOR; i++)
		m_pInverter[i] = NULL;

	// Mechanical Layer
	m_pPanelAligner = NULL;
	m_pPanelTransferCenter = NULL;
	m_pPanelTransfer = NULL;
	m_pPanelTransferOut = NULL;


	m_pOpPanel = NULL;

	m_pLCNet = NULL;

	for (int i = 0; i < DEF_MAX_RFID; i++)
		m_pRFID[i] = NULL;
	
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		m_pTabCarrier[i] = NULL;
	
	for (int i = 0; i< DEF_MAX_TABCARRIER; i++)
		m_pRTabCarrier[i] = NULL;

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
		m_pTabFeeder[i] = NULL;

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_pTabMounter[i] = NULL;
		m_pRTabMounter[i] = NULL;
	}
	
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
		m_pModelChange[i] = NULL;

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_pCameraCarrier[i] = NULL;
		m_pRCameraCarrier[i] = NULL;
	}

	m_pTurnHander = NULL; //SJ_YYK 161104 Add..

	// Control Layer
	m_pManageOpPanel = NULL;
	m_pVisionCalibration = NULL;
	m_pCtrlPanelAligner = NULL;

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_pCtrlTabCarrier[i] = NULL;
		m_pCtrlRTabCarrier[i] = NULL;
	}

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
		m_pCtrlTabFeeder[i] = NULL;

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_pCtrlTabMounter[i] = NULL;
		m_pCtrlRTabMounter[i] = NULL;
	}

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_pCtrlInspectionCamera[i] = NULL;
		m_pCtrlRInspectionCamera[i] = NULL;
	}

	m_pCtrlTHandler			= NULL; //SJ_YYK 161104 Add..


	// Process Layer
	m_pTrsAutoManager = NULL;
	m_pTrsJog = NULL;
	m_pTrsPanelAligner = NULL;
	m_pTrsPanelTransferCenter = NULL;
	m_pTrsPanelTransfer = NULL;
	m_pTrsPanelTransferOut = NULL;

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_pTrsTabCarrier[i] = NULL;
		m_pTrsRTabCarrier[i] = NULL;
	}
	
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
		m_pTrsTabFeeder[i] = NULL;
	
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_pTrsTabMounter[i] = NULL;
		m_pTrsRTabMounter[i] = NULL;
	}

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_pTrsInspectionCamera[i] = NULL;
		m_pTrsRInspectionCamera[i] = NULL;
	}

	m_pTrsTHandler = NULL;

	//20121004 SJ_HJG 횡전개
	for (int i = 0; i < 2; i++)
	{
		m_dTabEndMarkDistance[i] = 0.0;
	}

	m_dInsCamFrontFiduPos = 0.0; //SJ_YYK 150109 Add...

}

MPlatformOlbSystem::~MPlatformOlbSystem()
{
	int i = 0;
	/** 
	 *	Thread Stop
	 */
	if (m_pTrsAutoManager != NULL)
		m_pTrsAutoManager->ThreadStop();

	if (m_pTrsJog != NULL)
		m_pTrsJog->ThreadStop();
	
	if (m_pTrsPanelAligner != NULL)
		m_pTrsPanelAligner->ThreadStop();

	if (m_pTrsPanelTransferCenter != NULL)
		m_pTrsPanelTransferCenter->ThreadStop();

#ifdef DEF_SOURCE_SYSTEM
	if (m_pTrsPanelTransfer != NULL)
		m_pTrsPanelTransfer->ThreadStop();
#endif

	if (m_pTrsPanelTransferOut != NULL)
		m_pTrsPanelTransferOut->ThreadStop();

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		if (m_pTrsTabCarrier[i] != NULL)
			m_pTrsTabCarrier[i]->ThreadStop();

		if (m_pTrsRTabCarrier[i] != NULL)
			m_pTrsRTabCarrier[i]->ThreadStop();
	}

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		if (m_pTrsTabFeeder[i] != NULL)
			m_pTrsTabFeeder[i]->ThreadStop();
	}

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		if (m_pTrsTabMounter[i] != NULL)
			m_pTrsTabMounter[i]->ThreadStop();
		if (m_pTrsRTabMounter[i] != NULL)
			m_pTrsRTabMounter[i]->ThreadStop();
	}
	
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (m_pTrsInspectionCamera[i] != NULL)
			m_pTrsInspectionCamera[i]->ThreadStop();
		if (m_pTrsRInspectionCamera[i] != NULL)
			m_pTrsRInspectionCamera[i]->ThreadStop();
	}

	//SJ_YYK 161104 Add.,
#ifdef DEF_USE_TURN_HANDLER
	if (m_pTrsTHandler != NULL)
	{
		m_pTrsTHandler->ThreadStop();
	}
#endif

	/*
	 * 1. Process Layer Component 
	 */
	if (m_pTrsAutoManager)
	{
		delete m_pTrsAutoManager;
		m_pTrsAutoManager = NULL;
	}

	if (m_pTrsJog)
	{
		delete m_pTrsJog;
		m_pTrsJog = NULL;
	}

	if (m_pTrsPanelAligner)
	{
		delete m_pTrsPanelAligner;
		m_pTrsPanelAligner = NULL;
	}

	if (m_pTrsPanelTransferCenter)
	{
		delete m_pTrsPanelTransferCenter;
		m_pTrsPanelTransferCenter = NULL;
	}

#ifdef DEF_SOURCE_SYSTEM
	if (m_pTrsPanelTransfer)
	{
		delete m_pTrsPanelTransfer;
		m_pTrsPanelTransfer = NULL;
	}
#endif

	if (m_pTrsPanelTransferOut)
	{
		delete m_pTrsPanelTransferOut;
		m_pTrsPanelTransferOut = NULL;
	}

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		if (m_pTrsTabCarrier[i])
		{
			delete m_pTrsTabCarrier[i];
			m_pTrsTabCarrier[i] = NULL;
		}
		if (m_pTrsRTabCarrier[i])
		{
			delete m_pTrsRTabCarrier[i];
			m_pTrsRTabCarrier[i] = NULL;
		}
	}
	
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		if (m_pTrsTabFeeder[i])
		{
			delete m_pTrsTabFeeder[i];
			m_pTrsTabFeeder[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		if (m_pTrsTabMounter[i])
		{
			delete m_pTrsTabMounter[i];
			m_pTrsTabMounter[i] = NULL;
		}
		if (m_pTrsRTabMounter[i])
		{
			delete m_pTrsRTabMounter[i];
			m_pTrsRTabMounter[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (m_pTrsInspectionCamera[i])
		{
			delete m_pTrsInspectionCamera[i];
			m_pTrsInspectionCamera[i] = NULL;
		}
		if (m_pTrsRInspectionCamera[i])
		{
			delete m_pTrsRInspectionCamera[i];
			m_pTrsRInspectionCamera[i] = NULL;
		}
	}

	/*
	 * 2. Control Layer Component 
	 */
	if (m_pManageOpPanel)
	{
		delete m_pManageOpPanel;
		m_pManageOpPanel = NULL;
	}

	if (m_pVisionCalibration)
	{
		delete m_pVisionCalibration;
		m_pVisionCalibration = NULL;
	}

	if (m_pCtrlPanelAligner)
	{
		delete m_pCtrlPanelAligner;
		m_pCtrlPanelAligner = NULL;
	}
	
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		if (m_pCtrlTabCarrier[i])
		{
			delete m_pCtrlTabCarrier[i];
			m_pCtrlTabCarrier[i] = NULL;
		}
		if (m_pCtrlRTabCarrier[i])
		{
			delete m_pCtrlRTabCarrier[i];
			m_pCtrlRTabCarrier[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		if (m_pCtrlTabFeeder[i])
		{
			if (m_pCtrlTabFeeder[i]->GetZigRFIDState())
				m_pCtrlTabFeeder[i]->WriteZigInfo();
			delete m_pCtrlTabFeeder[i];
			m_pCtrlTabFeeder[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		if (m_pCtrlTabMounter[i] != NULL)
		{
			delete m_pCtrlTabMounter[i];
			m_pCtrlTabMounter[i] = NULL;
		}
		if (m_pCtrlRTabMounter[i] != NULL)
		{
			delete m_pCtrlRTabMounter[i];
			m_pCtrlRTabMounter[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (m_pCtrlInspectionCamera[i])
		{
			delete m_pCtrlInspectionCamera[i];
			m_pCtrlInspectionCamera[i] = NULL;
		}
		if (m_pCtrlRInspectionCamera[i])
		{
			delete m_pCtrlRInspectionCamera[i];
			m_pCtrlRInspectionCamera[i] = NULL;
		}
	}

	//SJ_YYK 161104 Add..
	if (FALSE == m_pCtrlTHandler)
	{
		delete m_pCtrlTHandler;
		m_pCtrlTHandler = NULL;
	}

	/*
	 * 3. Mechanical Layer Component 
	 */
	if (m_pPanelAligner)
	{
		delete m_pPanelAligner;
		m_pPanelAligner = NULL;
	}
	
	if (m_pPanelTransferCenter)
	{
		delete m_pPanelTransferCenter;
		m_pPanelTransferCenter = NULL;
	}

#ifdef DEF_SOURCE_SYSTEM
	if (m_pPanelTransfer)
	{
		delete m_pPanelTransfer;
		m_pPanelTransfer = NULL;
	}
#endif

	if (m_pPanelTransferOut)
	{
		delete m_pPanelTransferOut;
		m_pPanelTransferOut = NULL;
	}
	
	if (m_pOpPanel)
	{
		delete m_pOpPanel;
		m_pOpPanel = NULL;
	}

	if (m_pLCNet)
	{
		delete m_pLCNet;
		m_pLCNet = NULL;
	}


	for (int i = 0; i < DEF_MAX_RFID; i++)
	{
		if (m_pRFID[i])
		{
			delete m_pRFID[i];
			m_pRFID[i] = NULL;
		}
	}
	
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		if (m_pTabCarrier[i] != NULL)
		{
			delete m_pTabCarrier[i];
			m_pTabCarrier[i] = NULL;
		}
		if (m_pRTabCarrier[i] != NULL)
		{
			delete m_pRTabCarrier[i];
			m_pRTabCarrier[i] = NULL;
		}
	}
	
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		if (m_pTabFeeder[i])
		{
			delete m_pTabFeeder[i];
			m_pTabFeeder[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (m_pCameraCarrier[i])
		{
			delete m_pCameraCarrier[i];
			m_pCameraCarrier[i] = NULL;
		}
		if (m_pRCameraCarrier[i])
		{
			delete m_pRCameraCarrier[i];
			m_pRCameraCarrier[i] = NULL;
		}
	}

	for (int i=0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		if (m_pModelChange[i])
		{
			delete m_pModelChange[i];
			m_pModelChange[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		if (m_pTabMounter[i] != NULL)
		{
			delete m_pTabMounter[i];
			m_pTabMounter[i] = NULL;
		}
		if (m_pRTabMounter[i] != NULL)
		{
			delete m_pRTabMounter[i];
			m_pRTabMounter[i] = NULL;
		}
	}

	if (m_pTurnHander != NULL)
	{
		delete m_pTurnHander;
		m_pTurnHander = NULL;
	}


	/*
	 * 4. H/W Layer Component 
	 */
	for (int i = 0; i < DEF_MAX_LIGHTING; i++)
	{
		if (m_pLighting[i])
		{
			delete m_pLighting[i];
			m_pLighting[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_MAX_SERIAL; i++)
	{
		if (m_pSerial[i])
		{
			delete m_pSerial[i];
			m_pSerial[i] = NULL;
		}
	}

//	if (m_pMelsecIF)
//	{
//		delete m_pMelsecIF;
//		m_pMelsecIF = NULL;
//	}

	for (int i = 0; i < DEF_AXIS_MAX_AXIS; i++)
	{
		if (m_pMotion[i])
			delete m_pMotion[i];
	}

	for (int i = 0; i < DEF_MULTI_AXIS_MAX_AXIS; i++)
	{
		//161024 Modify...
		//______________________________________
		/*
		if (m_pMultiSercosMotion[i])
		{
			delete m_pMultiSercosMotion[i];
			m_pMultiSercosMotion[i] = NULL;
		}
		*/
		//______________________________________
		if(m_pMultiNMCMotion[i])
		{
			if(NULL != m_pMultiNMCMotion[i])
				delete m_pMultiNMCMotion[i];
		}
	}

	if (m_pSercosSetup)
	{
		delete m_pSercosSetup;
		m_pSercosSetup = NULL;
	}

	if (m_pCANSetup)
	{
		delete m_pCANSetup;
		m_pCANSetup = NULL;
	}

	for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
	{
		if (NULL != m_pCamera[i])
			delete m_pCamera[i];
	}

	if (m_pVision)
	{
		delete m_pVision;
		m_pVision = NULL;
	}

	for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
	{
		if (m_pCylinder[i])
		{
			delete m_pCylinder[i];
			m_pCylinder[i] = NULL;
		}
	}

	for (int i = 0; i < DEF_VAC_MAX_VACUUM; i++)
	{
		if (m_pVacuum[i])
		{
			delete m_pVacuum[i];
			m_pVacuum[i] = NULL;
		}
	}

	if (m_pMMCMotionLib)
	{
		delete m_pMMCMotionLib;
		m_pMMCMotionLib = NULL;
	}

	if (m_pIO)
	{
		delete m_pIO;
		m_pIO = NULL;
	}

	for (int i = 0; i < DEF_MAX_CONVEYOR; i++)
	{
		if (m_pInverter[i])
		{
			delete m_pInverter[i];
			m_pInverter[i] = NULL;
		}
	}

	/*
	 * 5. System Layer Component 
	 */	
	if (m_pObjSystemInfo)
	{
		delete m_pObjSystemInfo;
		m_pObjSystemInfo = NULL;
	}

	// 1. System Data 소멸
	if (m_pObjSystemData)
	{
		delete m_pObjSystemData;
		m_pObjSystemData = NULL;
	}

	// 2. Model Panel Data 소멸
	if (m_pObjPanelData)
	{
		delete m_pObjPanelData;
		m_pObjPanelData = NULL;
	}

	//20110223 SJ_HJG
	if (m_pObjTowerLampData)
	{
		delete m_pObjTowerLampData;
		m_pObjTowerLampData = NULL;
	}

	// 3. PreBonder Data 소멸
	if (m_pObjPreBonderData)
	{
		delete m_pObjPreBonderData;
		m_pObjPreBonderData = NULL;
	}

	// 4. Tab Data 소멸
	if (m_pObjTabData)
	{
		delete m_pObjTabData;
		m_pObjTabData = NULL;
	}

	// 5. Tab Offset Data 소멸
	if (m_pObjTabOffsetData)
	{
		delete m_pObjTabOffsetData;
		m_pObjTabOffsetData = NULL;
	}
	if (m_pObjTabOffsetData_Rear)
	{
		delete m_pObjTabOffsetData_Rear;
		m_pObjTabOffsetData_Rear = NULL;
	}

	// 6. Product Information 소멸
	if (m_pObjProductInfoData)
	{
		delete m_pObjProductInfoData;
		m_pObjProductInfoData = NULL;
	}

	// 8. Teaching Info 소멸
	if (m_pObjTeachingInfo)
	{
		delete m_pObjTeachingInfo;
		m_pObjTeachingInfo = NULL;
	}

	// 9. ProcessData Object 소멸
	if (m_pObjProcessData)
	{
		delete m_pObjProcessData;
		m_pObjProcessData = NULL;
	}

	//SJ_YYK 150318 Add..
	if (m_pObjAutoAlignData)
	{
		delete m_pObjAutoAlignData;
		m_pObjAutoAlignData = NULL;
	}

	if (m_pTabDataSourceFirst)
	{
		delete m_pTabDataSourceFirst;
		m_pTabDataSourceFirst = NULL;
	}

	if (m_pTabDataSourceSecond)
	{
		delete m_pTabDataSourceSecond;
		m_pTabDataSourceSecond = NULL;
	}
	
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		if (m_pObjWorkSchedule[i])
		{
			delete m_pObjWorkSchedule[i];
			m_pObjWorkSchedule[i] = NULL;
		}
		if (m_pObjWorkScheduleSecond[i])
		{
			delete m_pObjWorkScheduleSecond[i];
			m_pObjWorkScheduleSecond[i] = NULL;
		}
	}

	//@130214____________________
	for (int i=0; i<DEF_MAX_WORKER_GROUP; i++)
	{
		if (m_pObjDBTabLoss[i])
		{
			m_pObjDBTabLoss[i]->Disconnect();
			delete m_pObjDBTabLoss[i];
			m_pObjDBTabLoss[i] = NULL;
		}
	}
	//@__________________________

	//101004.KKY.자동운전중 신규모델 등록_____
	if (m_pObjTmpPanelData)
	{
		delete m_pObjTmpPanelData;
		m_pObjTmpPanelData = NULL;
	}
	if (m_pObjTmpPreBonderData)
	{
		delete m_pObjTmpPreBonderData;
		m_pObjTmpPreBonderData = NULL;
	}
	if (m_pObjTmpTabData)
	{
		delete m_pObjTmpTabData;
		m_pObjTmpTabData = NULL;
	}
	//________________________________________

	if (m_pMelsecIF)
	{
		delete m_pMelsecIF;
		m_pMelsecIF = NULL;
	}
	if (m_pMelsecIF2)
	{
		delete m_pMelsecIF2;
		m_pMelsecIF2 = NULL;
	}

	// jdy sesl 081230
	DeleteCriticalSection(&g_csError);
}

int MPlatformOlbSystem::Initialize()
{
	// jdy sesl 081230
	InitializeCriticalSection(&g_csError);

//SESL_LKH_090713 : 프로그램 시작 알림 Message window 추가
//	CMyMsgWnd wndMyMsg;
//	wndMyMsg.SetMessage( _T("PRE Bonder EqProgram is running...")); 
//	wndMyMsg.Show();

	int i = 0, j = 0;;
	int iResult = ERR_SYSTEM_SUCCESS;

	changeWorkingSetSize();

	/** Object of System Information */
	m_pObjSystemInfo = new MSystemInfo;

	/*
	*******************************************************************************
	***	System Data Load
	*******************************************************************************
	*/

	// 1. System Data
	m_pObjSystemData = new MSystemData(DEF_SYSTEM_DATA_FILE_NAME);
	//20140306 SJ_YSH 정연수D요청
#ifndef DEF_SETUP
	m_pObjSystemData->m_bSafetySensor = TRUE;
#endif
	//___________________________
	m_pObjSystemData->ReadData();
	
	MLanguageManager::SetNowLanguage(m_pObjSystemData->m_uiLanguageSelect);

//@#ifdef DEF_GATE_SYSTEM
	m_pObjSystemData->m_bUse1FeederAllMount = FALSE;
//@#endif

	CString	strModelFileDir;
	strModelFileDir.Format(_T("%s\\%s"), m_pObjSystemData->m_strModelFileDir, m_pObjSystemData->m_strModelFileName);

	// 2. Panel Data
	m_pObjPanelData	= new MPanelData(DEF_SYSTEM_PANEL_DATA_FILE_NAME, strModelFileDir);
	m_pObjPanelData->ReadData();

	//20110223 SJ_HJG
	m_pObjTowerLampData	= new MTowerLampData(DEF_SYSTEM_TOWERLAMP_DATA_FILE_NAME);
	m_pObjTowerLampData->ReadData();

	// 3. PreBonder Data
	m_pObjPreBonderData	= new MPreBonderData(DEF_SYSTEM_PRE_DATA_FILE_NAME, strModelFileDir);
	m_pObjPreBonderData->ReadData();

	// 4. Tab Data
	m_pObjTabData = new MTabData(DEF_SYSTEM_TAB_DATA_FILE_NAME, strModelFileDir);
	m_pObjTabData->ReadData();

	m_pTabDataSourceFirst = new MTabData(DEF_SYSTEM_TAB_DATA_FILE_NAME, strModelFileDir);
	m_pTabDataSourceSecond = new MTabData(DEF_SYSTEM_TAB_DATA_FILE_NAME, strModelFileDir);


	// 5. TabOffset Data
	m_pObjTabOffsetData	= new MTabOffsetData(DEF_SYSTEM_TAB_OFFSET_FILE_NAME, strModelFileDir);
	m_pObjTabOffsetData->ReadData();

	m_pObjTabOffsetData_Rear = new MTabOffsetData(DEF_SYSTEM_TAB_OFFSET_FILE_NAME_REAR, strModelFileDir);
	m_pObjTabOffsetData_Rear->ReadData();

	// 6. Product Information
	m_pObjProductInfoData = new MProductInfoData(DEF_SYSTEM_PRODUCT_INFO_FILE);
	m_pObjProductInfoData->ReadData();
	m_pObjProductInfoData->ReadMaterialData();
	m_pObjProductInfoData->ReadGlassData();
	
	//110620.KKY.Add_____
	m_pObjProductInfoData->ReadProductPlan();
	m_pObjProductInfoData->ReadMaterialCodeInfo();
	//___________________

	// 8. Teaching 정보 Object 생성
	m_pObjTeachingInfo = new MTeachingInfo(DEF_SYSTEM_TEACHING_INFO_FILE_NAME);
	if (m_pObjTeachingInfo->LoadTeachingInfo() < 0)
	{
		//Teaching 정보 파일 로드에 실패 했습니다.
		MyMessageBox(MY_MESSAGE_BOX,61500, _T("Teaching Info Data Load Failure"), M_ICONERROR);
	}

	// 9. ProcessData Object 생성
	m_pObjProcessData = new MProcessData(m_pObjSystemData, m_pObjPanelData, m_pObjTabData);
#ifdef DEF_SOURCE_SYSTEM
		//SJ_YYK 150907 Modify....
	#ifndef DEF_MIRROR_
		m_pObjWorkSchedule[DEF_FRONT_GROUP] = new MWorkSchedule(WORKING_SIDE_X_UP, DEF_FRONT_GROUP, 0);
		m_pObjWorkSchedule[DEF_REAR_GROUP] = new MWorkSchedule(WORKING_SIDE_X_DOWN, DEF_REAR_GROUP, 0);
		
		m_pObjWorkScheduleSecond[DEF_FRONT_GROUP] = new MWorkSchedule(WORKING_SIDE_X_UP, DEF_FRONT_GROUP, 1);
		m_pObjWorkScheduleSecond[DEF_REAR_GROUP] = new MWorkSchedule(WORKING_SIDE_X_DOWN, DEF_REAR_GROUP, 1);
	#else
	m_pObjWorkSchedule[DEF_FRONT_GROUP] = new MWorkSchedule(WORKING_SIDE_X_UP, DEF_FRONT_GROUP, 0);
	m_pObjWorkSchedule[DEF_REAR_GROUP] = new MWorkSchedule(WORKING_SIDE_X_DOWN, DEF_REAR_GROUP, 0);
	
	m_pObjWorkScheduleSecond[DEF_FRONT_GROUP] = new MWorkSchedule(WORKING_SIDE_X_UP, DEF_FRONT_GROUP, 1);
	m_pObjWorkScheduleSecond[DEF_REAR_GROUP] = new MWorkSchedule(WORKING_SIDE_X_DOWN, DEF_REAR_GROUP, 1);
	#endif
#else
#	ifndef DEF_MIRROR_
	m_pObjWorkSchedule[DEF_FRONT_GROUP] = new MWorkSchedule(WORKING_SIDE_Y_LEFT, DEF_FRONT_GROUP, 0);
	m_pObjWorkSchedule[DEF_REAR_GROUP] = new MWorkSchedule(WORKING_SIDE_Y_RIGHT, DEF_REAR_GROUP, 0);
#	else
	m_pObjWorkSchedule[DEF_FRONT_GROUP] = new MWorkSchedule(WORKING_SIDE_Y_RIGHT, DEF_FRONT_GROUP, 0);
	m_pObjWorkSchedule[DEF_REAR_GROUP] = new MWorkSchedule(WORKING_SIDE_Y_LEFT, DEF_REAR_GROUP, 0);
#	endif
	m_pObjWorkScheduleSecond[DEF_FRONT_GROUP] = NULL;
	m_pObjWorkScheduleSecond[DEF_REAR_GROUP] = NULL;
#endif

	//@130214____________________
	m_pObjDBTabLoss[DEF_FRONT_GROUP] = new CDBTabLoss(DEF_SYSTEM_LOG_TABIC_LOSS_PATH_FRONT, DEF_SYSTEM_DB_TABIC_ORG_PATH, _T("TabLoss"), 14);
	m_pObjDBTabLoss[DEF_FRONT_GROUP]->Initialize();

	m_pObjDBTabLoss[DEF_REAR_GROUP] = new CDBTabLoss(DEF_SYSTEM_LOG_TABIC_LOSS_PATH_REAR, DEF_SYSTEM_DB_TABIC_ORG_PATH, _T("TabLoss"), 14);
	m_pObjDBTabLoss[DEF_REAR_GROUP]->Initialize();
	//@__________________________


	//@_______________________________________________________
	AssignWorkScheduleData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_pObjWorkSchedule[i]->ChangeModel();
	}
#ifdef DEF_SOURCE_SYSTEM
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_pObjWorkScheduleSecond[i]->ChangeModel();
	}
#endif
	//@_______________________________________________________


	// SJ_YYK 150318  Add..
	CString  strFilePath = _T("./AutoAlign");
	m_pObjAutoAlignData = new MAutoAlignData(strFilePath, DEF_AUTOALIGN_DATA_FILE_NAME);


#ifdef SIMULATION
	//SIMULATION mode로 동작 합니다.
	MyMessageBox(MY_MESSAGE_BOX,61521, _T("Information"), M_ICONINFORMATION);
#endif	/* SIMULATION */

	/*
	*******************************************************************************
	***	Hardware Component 생성 : 
	*******************************************************************************
	*/
	iResult = createBasicHWComponents();
	if (iResult)
		return iResult;
	
	createCylinderComponents();
	createVacuumComponents();
	
	// selsk_jdy : 내부에서 유효한 Error Return 하는 모든 create~ 함수에 Error 처리 필요.
	iResult = createMotionComponents();
	if (iResult)
	{
		MyMessageBox(MY_MESSAGE_BOX,61522, _T("MMC Interface Failure"), M_ICONERROR);
	}
//		return iResult;

	createLightingComponents();
	createInverterComponents();

	/*
	*******************************************************************************
	***	Mechanical Component 생성 : 
	*******************************************************************************
	*/
	createLCNetComponent();

	createOpPanelComponents();
	createPanelAlignerComponents();
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	createPanelTransferCenterComponents();
#	endif
	createPanelTransferComponents();
#endif
	createPanelTransferOutComponents();
	createTabCarrierComponents();
	createRTabCarrierComponents();
	createTabFeederComponents();
	createTabMounterComponents();
	createRFIDComponents();
	createCameraCarrierComponents();
	createModelChangeComponents();
#ifdef DEF_USE_TURN_HANDLER
	createTurnHandlerComponents(); //SJ_YYK 161104 Add..
#endif

	// Mechanical Component 간의 상호 참조 설정
	// PanelAligner_________________________________________________________________
	SStateCheckCompListForPanelAligner sCompListForPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	sCompListForPanelAligner.m_plnkStatePanelTransfer	= GetPanelTransferComponent();
#endif
	sCompListForPanelAligner.m_plnkStatePanelTransferOut = GetPanelTransferOutComponent();
	sCompListForPanelAligner.m_plnkStateModelChange1	 =	GetModelChangeComponent(DEF_FRONT_GROUP);
	sCompListForPanelAligner.m_plnkStateModelChange2	 =	GetModelChangeComponent(DEF_REAR_GROUP);

	for (int i = 0 ; i < DEF_MAX_TABMOUNTER ; i++)
	{
		sCompListForPanelAligner.m_plnkStateTabMounter[i] = GetTabMounterComponent(i);
		sCompListForPanelAligner.m_plnkStateRTabMounter[i] = GetRTabMounterComponent(i);
	}
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sCompListForPanelAligner.m_plnkStateInspectCamCarrier[i] = GetCameraCarrierComponent(i);
		sCompListForPanelAligner.m_plnkStateRInspectCamCarrier[i] = GetRCameraCarrierComponent(i);
	}

	m_pPanelAligner->AssignStateCheckComponents(sCompListForPanelAligner);
	//______________________________________________________________________________

	// PanelTransfer________________________________________________________________
	SStateCheckCompListForPanelTransfer sCompListForPanelTransfer;
	sCompListForPanelTransfer.m_plnkStatePanelAligner = GetPanelAlignerComponent();
	for (int i = 0 ; i < DEF_MAX_TABMOUNTER ; i++)
	{
		sCompListForPanelTransfer.m_plnkStateTabMounter[i] = GetTabMounterComponent(i);
		sCompListForPanelTransfer.m_plnkStateRTabMounter[i] = GetRTabMounterComponent(i);
	}
	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sCompListForPanelTransfer.m_plnkStateInspectCamCarrier[i] = GetCameraCarrierComponent(i);
		sCompListForPanelTransfer.m_plnkStateRInspectCamCarrier[i] = GetRCameraCarrierComponent(i);
	}

#ifdef DEF_SOURCE_SYSTEM
	sCompListForPanelTransfer.m_plnkStatePanelTransferOut = GetPanelTransferOutComponent();
	sCompListForPanelTransfer.m_plnkStatePanelTransferIn = GetPanelTransferComponent();
#	ifdef DEF_USE_TRANSFER_CENTER
	m_pPanelTransferCenter->AssignStateCheckComponents(sCompListForPanelTransfer);
#	endif
	m_pPanelTransfer->AssignStateCheckComponents(sCompListForPanelTransfer);
#endif
	m_pPanelTransferOut->AssignStateCheckComponents(sCompListForPanelTransfer);
	//______________________________________________________________________________


	// Tab Carrier__________________________________________________________________
	SStateCheckCompListForTabCarrier sCompListForTabCarrier;
/*
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		sCompListForTabCarrier.m_plnkStateTabFeeder = m_pTabFeeder[i];
		for (int j = 0; j < DEF_MAX_WORKER_PER_GROUP; j++)
		{
			sCompListForTabCarrier.m_plnkStateTabMounter[j] = m_pTabMounter[j + (i * DEF_MAX_WORKER_PER_GROUP)];
		}
		m_pTabCarrier[i]->AssignStateCheckComponents(sCompListForTabCarrier);
	}
*/

	//Front______
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		sCompListForTabCarrier.m_plnkStateTabMounter[i] = m_pTabMounter[i];

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		sCompListForTabCarrier.m_plnkStateTabCarrier[i] = m_pTabCarrier[i];

	sCompListForTabCarrier.m_plnkStateTabFeeder = m_pTabFeeder[DEF_TABFEEDER1];
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_pTabCarrier[i]->AssignStateCheckComponents(sCompListForTabCarrier);
	}

	//Rear_______
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		sCompListForTabCarrier.m_plnkStateTabMounter[i] = m_pRTabMounter[i];

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		sCompListForTabCarrier.m_plnkStateTabCarrier[i] = m_pRTabCarrier[i];

	sCompListForTabCarrier.m_plnkStateTabFeeder = m_pTabFeeder[DEF_TABFEEDER2];
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_pRTabCarrier[i]->AssignStateCheckComponents(sCompListForTabCarrier);
	}
	//______________________________________________________________________________
	

	// Tab Feeder___________________________________________________________________
	SStateCheckCompListForTabFeeder	sCompListForTabFeeder;
	sCompListForTabFeeder.m_plnkOpPanel = m_pOpPanel;
	/*
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		sCompListForTabFeeder.m_plnkCarrier	= m_pTabCarrier[i];
		m_pTabFeeder[i]->AssignStateCheckComponents(sCompListForTabFeeder);
	}
	*/
	for (int i = 0; i<DEF_MAX_TABCARRIER; i++)
		sCompListForTabFeeder.m_plnkCarrier[i]	= m_pTabCarrier[i];
//	m_pTabFeeder[DEF_TABFEEDER1]->AssignStateCheckComponents(sCompListForTabFeeder);
	for (int i = 0; i<DEF_MAX_TABCARRIER; i++)
		sCompListForTabFeeder.m_plnkCarrier[i]	= m_pRTabCarrier[i];
//	m_pTabFeeder[DEF_TABFEEDER2]->AssignStateCheckComponents(sCompListForTabFeeder);
	//______________________________________________________________________________

	// TabMounter___________________________________________________________________
	SStateCheckCompListForTabMounter sCompListForTabMounter;

	sCompListForTabMounter.m_plnkPanelAligner = m_pPanelAligner;
	sCompListForTabMounter.m_plnkPanelTransferOut = m_pPanelTransferOut;
#ifdef DEF_SOURCE_SYSTEM
	sCompListForTabMounter.m_plnkPanelTransfer	= m_pPanelTransfer;
#endif

	//Front_____
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		sCompListForTabMounter.m_plnkCamCarrier[i] = m_pCameraCarrier[i];
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		sCompListForTabMounter.m_plnkTabMounter[i] = m_pTabMounter[i];
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		sCompListForTabMounter.m_plnkTabCarrier[i] = m_pTabCarrier[i];

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		m_pTabMounter[i]->AssignStateCheckComponents(sCompListForTabMounter);

	//Rear______
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		sCompListForTabMounter.m_plnkCamCarrier[i] = m_pRCameraCarrier[i];
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		sCompListForTabMounter.m_plnkTabMounter[i] = m_pRTabMounter[i];
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		sCompListForTabMounter.m_plnkTabCarrier[i] = m_pRTabCarrier[i];

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		m_pRTabMounter[i]->AssignStateCheckComponents(sCompListForTabMounter);

	//______________________________________________________________________________

	// TabAligner
	// CameraCarrier________________________________________________________________
	SStateCheckCompListForCameraCarrier sCompListForCamCarrier;

	sCompListForCamCarrier.m_plnkPanelAligner = m_pPanelAligner;

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		sCompListForCamCarrier.m_plnkTabMounter[i] = m_pTabMounter[i];
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		switch (i)
		{
		case DEF_INSPECTION_CARRIER_1:
			sCompListForCamCarrier.m_plnkPreCamCarrier = m_pCameraCarrier[i+1];
			sCompListForCamCarrier.m_plnkNextCamCarrier = NULL;
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CARRIER_4:
			sCompListForCamCarrier.m_plnkPreCamCarrier = NULL;
			sCompListForCamCarrier.m_plnkNextCamCarrier = m_pCameraCarrier[i-1];
			break;
		default:
			sCompListForCamCarrier.m_plnkPreCamCarrier = m_pCameraCarrier[i+1];
			sCompListForCamCarrier.m_plnkNextCamCarrier = m_pCameraCarrier[i-1];
			break;
#else
		case DEF_INSPECTION_CARRIER_2:
			sCompListForCamCarrier.m_plnkPreCamCarrier = NULL;
			sCompListForCamCarrier.m_plnkNextCamCarrier = m_pCameraCarrier[i-1];
			break;
#endif
		}
		m_pCameraCarrier[i]->AssignStateCheckComponents(sCompListForCamCarrier);
	}

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		sCompListForCamCarrier.m_plnkTabMounter[i] = m_pRTabMounter[i];
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		switch (i)
		{
		case DEF_INSPECTION_CARRIER_1:
			sCompListForCamCarrier.m_plnkPreCamCarrier = m_pRCameraCarrier[i+1];
			sCompListForCamCarrier.m_plnkNextCamCarrier = NULL;
			break;
#ifdef DEF_SOURCE_SYSTEM
		case DEF_INSPECTION_CARRIER_4:
			sCompListForCamCarrier.m_plnkPreCamCarrier = NULL;
			sCompListForCamCarrier.m_plnkNextCamCarrier = m_pRCameraCarrier[i-1];
			break;
		default:
			sCompListForCamCarrier.m_plnkPreCamCarrier = m_pRCameraCarrier[i+1];
			sCompListForCamCarrier.m_plnkNextCamCarrier = m_pRCameraCarrier[i-1];
			break;
#else
		case DEF_INSPECTION_CARRIER_2:
			sCompListForCamCarrier.m_plnkPreCamCarrier = NULL;
			sCompListForCamCarrier.m_plnkNextCamCarrier = m_pRCameraCarrier[i-1];
			break;
#endif
		}
		m_pRCameraCarrier[i]->AssignStateCheckComponents(sCompListForCamCarrier);
	}
	//______________________________________________________________________________

	// Model Change
	SStateCheckCompListForModelChange	sCompListForModelChange;
	sCompListForModelChange.m_plnkStatePanelAligner		= GetPanelAlignerComponent();
	sCompListForModelChange.m_plnkStatePanelTransferOut	= GetPanelTransferOutComponent();
#ifdef DEF_SOURCE_SYSTEM
	sCompListForModelChange.m_plnkStatePanelTransfer	= GetPanelTransferComponent();
#endif
	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		for (int j = 0; j < DEF_MAX_INSPECTION_CAM_CARRIER; j++)
		{
			if (i == DEF_FRONT_GROUP)
				sCompListForModelChange.m_plnkStateCameraCarrier[j] = GetCameraCarrierComponent(j);
			else
				sCompListForModelChange.m_plnkStateCameraCarrier[j] = GetRCameraCarrierComponent(j);
		}
		m_pModelChange[i]->AssignStateCheckComponents(sCompListForModelChange);
	}

	//SJ_YYK 161104 Add.,.
#ifdef DEF_USE_TURN_HANDLER
	SStateCheckCompListForHandler		sCompListForHandler;
	sCompListForHandler.m_plnkOpPanel				= GetOpPanelComponent();
	sCompListForHandler.m_plnkPanelTransferIn		= GetPanelTransferComponent();
	sCompListForHandler.m_plnkPanelTransferCenter	= GetPanelTransferCenterComponent();
	m_pTurnHander->AssignStateCheckComponents(sCompListForHandler);
#endif
	//__________________________*/

	/*
	*******************************************************************************
	***	Control Component 생성 : 
	*******************************************************************************
	*/
	createVisionCalibrationComponents();
	createManageOpPanelComponents();
	createCtrlPanelAlignerComponents();
	createCtrlTabCarrierComponents();
	createCtrlRTabCarrierComponents();
	createCtrlTabFeederComponents();
	createCtrlTabMounterComponents();
	createCtrlInspectionCameraComponents();
#ifdef DEF_USE_TURN_HANDLER
	createCtrlTHandlerComponents(); //SJ_YYK 161104 Add..
#endif
	/*
	*******************************************************************************
	***	Process Component 생성 : 
	*******************************************************************************
	*/
	// 다른 Thread 에 AutoManager 할당하기 위해 TrsAutoManager 는 다른 Thread 보다 먼저 생성되어야 한다.
	createTrsAutoManageComponents();

	createTrsJogComponents();
	createTrsPanelAlignerComponents();
#ifdef DEF_USE_TRANSFER_CENTER
	createTrsPanelTransferCenterComponents();
#endif
	createTrsPanelTransferComponents();
	createTrsPanelTransferOutComponents();
	createTrsTabCarrierComponents();
	createTrsRTabCarrierComponents();
	createTrsTabFeederComponents();
	createTrsTabMounterComponents();
	createTrsInspectionCameraComponents();
#ifdef DEF_USE_TURN_HANDLER
	createTrsTHandlerComponents(); //SJ_YYK 161104 Add...
#endif

	// PanelAligner
	m_pTrsPanelAligner->AssignITI(m_pTrsPanelTransfer, m_pTrsPanelTransferOut, m_pTrsTabMounter[DEF_TABMOUNTER1], m_pTrsRTabMounter[DEF_TABMOUNTER1],
								m_pTrsInspectionCamera[DEF_INSPECTION_CARRIER_1], m_pTrsRInspectionCamera[DEF_INSPECTION_CARRIER_1], 
								m_pTrsTabFeeder[DEF_TABFEEDER1], m_pTrsTabFeeder[DEF_TABFEEDER2]);

	//PanelTransfer
#ifdef DEF_SOURCE_SYSTEM
#		ifdef DEF_USE_TRANSFER_CENTER
	m_pTrsPanelTransferCenter->AssignITI(m_pTrsTHandler,m_pTrsPanelTransfer);
#		endif
	m_pTrsPanelTransfer->AssignITI(m_pTrsTHandler, m_pTrsPanelAligner);

#endif

	m_pTrsPanelTransferOut->AssignITI(m_pTrsPanelAligner);

	//Front TabCarrier
	STabCarrierRefITIList sTabCarrierITIList;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		sTabCarrierITIList.m_plnkITITabFeeder = m_pTrsTabFeeder[DEF_TABFEEDER1];
		for (int j=0; j<DEF_MAX_TABMOUNTER; j++)
			sTabCarrierITIList.m_plnkITITabMounter[j] = m_pTrsTabMounter[j];
		m_pTrsTabCarrier[i]->AssignITI(sTabCarrierITIList);
	}

	// Rear TabCarrier
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		sTabCarrierITIList.m_plnkITITabFeeder = m_pTrsTabFeeder[DEF_TABFEEDER2];
		for (int j=0; j<DEF_MAX_TABMOUNTER; j++)
			sTabCarrierITIList.m_plnkITITabMounter[j] = m_pTrsRTabMounter[j];
		m_pTrsRTabCarrier[i]->AssignITI(sTabCarrierITIList);
	}

	// TabFeeder, Tension
	tagSTabFeederRefITIList sTabFeederITIList;
	sTabFeederITIList.m_plnkITIPanelAligner = m_pTrsPanelAligner;
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		sTabFeederITIList.m_plnkITITabCarrier[i] = m_pTrsTabCarrier[i];
	m_pTrsTabFeeder[DEF_TABFEEDER1]->AssignITI(sTabFeederITIList);

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		sTabFeederITIList.m_plnkITITabCarrier[i] = m_pTrsRTabCarrier[i];
	m_pTrsTabFeeder[DEF_TABFEEDER2]->AssignITI(sTabFeederITIList);

	// TabMounter
	STabMounterRefITIList sITIList;
	sITIList.m_plnkITI_PanelAligner = m_pTrsPanelAligner;

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		sITIList.m_rgplnkITI_InspectionCamera[i] = m_pTrsInspectionCamera[i];
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
		sITIList.m_plnkITI_TabCarrier[i] = m_pTrsTabCarrier[i];
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_pTrsTabMounter[i]->AssignITI(sITIList);
	}

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		sITIList.m_rgplnkITI_InspectionCamera[i] = m_pTrsRInspectionCamera[i];
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		sITIList.m_plnkITI_TabCarrier[i] = m_pTrsRTabCarrier[i];
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_pTrsRTabMounter[i]->AssignITI(sITIList);
	}

	// InspectionCamera
	SInspectionCameraRefITIList sInspectionCameraITIList;
	sInspectionCameraITIList.m_plnkITI_PanelAligner = m_pTrsPanelAligner;

	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		sInspectionCameraITIList.m_plnkITI_TabMounter[i] = m_pTrsTabMounter[i];
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_pTrsInspectionCamera[i]->AssignITI(sInspectionCameraITIList);
	}


	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		sInspectionCameraITIList.m_plnkITI_TabMounter[i] = m_pTrsRTabMounter[i];
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_pTrsRInspectionCamera[i]->AssignITI(sInspectionCameraITIList);
	}

#ifdef DEF_USE_TURN_HANDLER
	m_pTrsTHandler->AssignITI(m_pTrsPanelTransferCenter, m_pTrsPanelTransfer); //SJ_YYK 161104 Add..
#endif
	// AutoManager
	STrsAutoManagerRefProcess listRefTrs;
	listRefTrs.m_plnkTrsPanelAligner = m_pTrsPanelAligner;
#ifdef DEF_SOURCE_SYSTEM
	listRefTrs.m_plnkTrsPanelTransferCenter = m_pTrsPanelTransferCenter;
	listRefTrs.m_plnkTrsPanelTransfer = m_pTrsPanelTransfer;
#endif
	listRefTrs.m_plnkTrsPanelTransferOut = m_pTrsPanelTransferOut;

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		listRefTrs.m_plnkTrsTabCarrier[i] = m_pTrsTabCarrier[i];
		listRefTrs.m_plnkTrsRTabCarrier[i] = m_pTrsRTabCarrier[i];
	}

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
		listRefTrs.m_plnkTrsTabFeeder[i] = m_pTrsTabFeeder[i];
	
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		listRefTrs.m_plnkTrsInspectionCamera[i] = m_pTrsInspectionCamera[i];
		listRefTrs.m_plnkTrsRInspectionCamera[i] = m_pTrsRInspectionCamera[i];
	}

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		listRefTrs.m_plnkTrsTabMounter[i] = m_pTrsTabMounter[i];
		listRefTrs.m_plnkTrsRTabMounter[i] = m_pTrsRTabMounter[i];
	}

#ifdef DEF_USE_TURN_HANDLER
	listRefTrs.m_plnkTrsTHandler = m_pTrsTHandler; //SJ_YYK 161104 Add..
#else
	listRefTrs.m_plnkTrsTHandler = NULL;
#endif
	m_pTrsAutoManager->AssignProcess(listRefTrs);

	SPanelAlignerRefProcess TrsPanelAlignerRefProcess;
	TrsPanelAlignerRefProcess.m_plnkTrsAutoManager = m_pTrsAutoManager;
	m_pPanelAligner->AssignProcess(TrsPanelAlignerRefProcess);

	STrsTabMounterRefProcess TrsTabMounterRefProcess;
	TrsTabMounterRefProcess.m_plnkCtrlTabFeeder[DEF_TABFEEDER1] = m_pCtrlTabFeeder[DEF_TABFEEDER1];
	TrsTabMounterRefProcess.m_plnkCtrlTabFeeder[DEF_TABFEEDER2] = m_pCtrlTabFeeder[DEF_TABFEEDER2];
	TrsTabMounterRefProcess.m_plnkTrsPanelAligner = m_pTrsPanelAligner;	
	TrsTabMounterRefProcess.m_plnkTrsInspectionCarrier = m_pTrsInspectionCamera[0];
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		m_pTrsTabMounter[i]->AssignProcess(TrsTabMounterRefProcess);
	TrsTabMounterRefProcess.m_plnkTrsInspectionCarrier = m_pTrsRInspectionCamera[0];
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
		m_pTrsRTabMounter[i]->AssignProcess(TrsTabMounterRefProcess);

	STrsTabCarrierRefProcess TrsTabCarrierRefProcess;
	TrsTabCarrierRefProcess.m_plnkCtrlTabFeeder[DEF_TABFEEDER1] = m_pCtrlTabFeeder[DEF_TABFEEDER1];
	TrsTabCarrierRefProcess.m_plnkCtrlTabFeeder[DEF_TABFEEDER2] = m_pCtrlTabFeeder[DEF_TABFEEDER2];
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_pTrsTabCarrier[i]->AssignProcess(TrsTabCarrierRefProcess);
		m_pTrsRTabCarrier[i]->AssignProcess(TrsTabCarrierRefProcess);
	}

	SCtrlCompListForTabFeeder sCtrlCompListTabFeeder;
	for (int i=0; i<DEF_MAX_TABFEEDER; i++)
	{
		sCtrlCompListTabFeeder.m_plnkCtrlTabFeeder = m_pCtrlTabFeeder[i];
		//m_pTabFeeder[i]->AssignCtrlComponents(sCtrlCompListTabFeeder);
	}

	// Turn Handler============================== //SJ_YYK 161104 Add.
#ifdef DEF_USE_TURN_HANDLER
	m_pTrsTHandler->AssignAutoManager(m_pTrsAutoManager);
#endif
	// ============================================

	/*
	**********************************************************************************
	** ETC
	**********************************************************************************
	*/
	//@_______________________________________________________
	SWorkScheduleRefProcess WorkScheduletRefTrs;
	
	WorkScheduletRefTrs.m_plnkTrsPanelAligner = m_pTrsPanelAligner;
	WorkScheduletRefTrs.m_plnkTrsTabFeeder[DEF_TABFEEDER1] = m_pTrsTabFeeder[DEF_TABFEEDER1];
	WorkScheduletRefTrs.m_plnkTrsTabFeeder[DEF_TABFEEDER2] = m_pTrsTabFeeder[DEF_TABFEEDER2];

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_pObjWorkSchedule[i]->AssignProcess(WorkScheduletRefTrs);
	}
#ifdef DEF_SOURCE_SYSTEM
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_pObjWorkScheduleSecond[i]->AssignProcess(WorkScheduletRefTrs);
	}
#endif
	//@_______________________________________________________

	/*
	**********************************************************************************
	** Post Process 
	**********************************************************************************
	*/
	BYTE ucLevel = DEF_MLOG_NONE_LOG_LEVEL;
	if (m_pObjSystemData->m_bLogLevelError)
		ucLevel |= DEF_MLOG_ERROR_LOG_LEVEL;
	if (m_pObjSystemData->m_bLogLevelMonitoring)
		ucLevel |= DEF_MLOG_MONITORING_LOG_LEVEL;
	if (m_pObjSystemData->m_bLogLevelNormal)
		ucLevel |= DEF_MLOG_NORMAL_LOG_LEVEL;
	if (m_pObjSystemData->m_bLogLevelTactTime)
		ucLevel |= DEF_MLOG_TACT_TIME_LOG_LEVEL;
		//170427 JSh.s
	if (m_pObjSystemData->m_bLogLevelEqTactTime)
		ucLevel |= DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL;
		//170427 JSh.e
	m_pObjSystemInfo->SetAllObjsLogAttribute(ucLevel, m_pObjSystemData->m_iLogKeepingDays);
	m_pObjSystemInfo->DeleteOldLogFilesAllObjs();

	/*
	**********************************************************************************
	** Link Thread And Start Thread 
	**********************************************************************************
	*/
	initSystem();
	startThreads();

	// Report Model Data
	ReportModel();

	// 9자리 Error Code 에 대한 Error Message 생성.
	//m_pObjSystemInfo->MakeErrorMessageFile();

	m_bSystemInitialized = TRUE;

	CString strtemp;
	strtemp.Format(_T("Program Start."));
	m_pTrsAutoManager->WriteMonitoringLog(strtemp);

	//SJ_YYK 110421 Add... Ellas
	m_pLCNet->EqSpecialStateReport(eProgramStart);

	//170926 JSH.s
	//회피 위치 만큼 작업 거리에서 재외하기 위해 필요한 중복 코드
	AssignWorkScheduleData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_pObjWorkSchedule[i]->ChangeModel();
	}
#ifdef DEF_SOURCE_SYSTEM
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_pObjWorkScheduleSecond[i]->ChangeModel();
	}
#endif
	//170926 JSH.e

	return ERR_SYSTEM_SUCCESS;
}

CString	MPlatformOlbSystem::GetErrorMessage(int iErrorCode, int iLanguageType, BOOL bErrorReport /*= FALSE*/) //170930 JSH
{
	int iErrorBase = iErrorCode / 1000;
	CString strObjName = GetObjectName(iErrorCode);
	CString strError = ReadErrorMessage(iErrorBase, iLanguageType, bErrorReport); //170930 JSH
	
	switch (iErrorBase)
	{
	case 300003:	// 문이 열린 상태 입니다. 
	case 400002:	// 문이 열러 자동운전을 시작할 수 없습니다.
	case 400031:	// 자동운전중 Door Open이 감지 되었습니다.
		// Door Error Display
		strError += GetOpenDoorPosition();
		break;
		
	default:
		break;
	}
	
	if (iErrorBase / 1000 == 101)	// Cylinder
		strError += getCylIOAddr(iErrorCode % 1000);
	else if (iErrorBase / 1000 == 102)	// Vacuum
		strError += getVacIOAddr(iErrorCode % 1000);
	//170710 JSH.s
	CString strErrorCode;
	strErrorCode.Format(_T("%d,"), iErrorBase);

	//170710 JSH.e
	
	//	return strObjName + " " + strError;
	return strErrorCode + " " + strError ; //170710 JSH
}

int MPlatformOlbSystem::GetModelNameList(CString *strModelList, int *iModelCount)
{
	MFileUtil util;
	*iModelCount = util.GetDirCountInDir(m_pObjSystemData->m_strModelFileDir);

	if (*iModelCount > DEF_MAX_MODEL)
	{
		CString strMsg;
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_61501"), &strMsg);
		//모델 갯수가 시스템 허용치를 초과 합니다.
		CString strNum;
		strNum.Format(_T("\r\nModel number(%d), System allowable value(%d)"), *iModelCount, DEF_MAX_MODEL);

		strMsg += strNum;
		MyMessageBox(strMsg, _T("Manage Model"), M_ICONERROR);
		return ERR_SYSTEM_MAX_COUNT_EXCEED_LIMIT;
	}

	// Directory List 를 읽어와서 strModelList 에 등록한다.
	util.GetDirListInDir(m_pObjSystemData->m_strModelFileDir, strModelList, *iModelCount);

	return ERR_SYSTEM_SUCCESS;
}

void MPlatformOlbSystem::SetLogAttribute(BYTE ucLevel, int iLogKeepingDays)
{
	m_pObjSystemInfo->SetAllObjsLogAttribute(ucLevel, iLogKeepingDays);

	// log level
	if (ucLevel & DEF_MLOG_ERROR_LOG_LEVEL)
		m_pObjSystemData->m_bLogLevelError = TRUE;
	if (ucLevel & DEF_MLOG_MONITORING_LOG_LEVEL)
		m_pObjSystemData->m_bLogLevelMonitoring = TRUE;
	if (ucLevel & DEF_MLOG_NORMAL_LOG_LEVEL)
		m_pObjSystemData->m_bLogLevelNormal = TRUE;
	if (ucLevel & DEF_MLOG_TACT_TIME_LOG_LEVEL)
		m_pObjSystemData->m_bLogLevelTactTime = TRUE;
		//170427 JSh.s
	if (ucLevel & DEF_MLOG_EQ_TACT_TIME_LOG_LEVEL)
		m_pObjSystemData->m_bLogLevelEqTactTime = TRUE;
		//170427 JSh.e

	// log keeping days
	m_pObjSystemData->m_iLogKeepingDays = iLogKeepingDays;

	// save system data
	m_pObjSystemData->WriteData();	
}

int MPlatformOlbSystem::DeleteModel(LPCTSTR pstrModel)
{
	CString	strPath;
	CString szTemp;
	strPath.Format(_T("%s/%s"), m_pObjSystemData->m_strModelFileDir, pstrModel);
	szTemp = m_pObjSystemData->m_strModelFileName;
	if (!wcscmp(pstrModel,szTemp.GetBuffer(szTemp.GetLength())))
	{
		//현재 선택된 모델을 삭제할 수 없습니다
		MyMessageBox(MY_MESSAGE_BOX,61502, _T("Manage Model"), M_ICONERROR);
		return ERR_SYSTEM_MODEL_DELETE_FAIL;
	}

	MFileUtil util;
	util.DeleteFilesInDir(strPath);	// 디렉토리 내 파일들을 모두 삭제 한다.
	RemoveDirectory(strPath);		// 디렉토리를 지운다.
	return ERR_SYSTEM_SUCCESS;
}

CString MPlatformOlbSystem::GetCurrentModelName()
{
	return m_pObjSystemData->m_strModelFileName;
}

int MPlatformOlbSystem::CreateModel(LPCTSTR dest, LPCTSTR src)
{
	MFileUtil util;
	CString strDest, strSrc, strMsg;	// Full Path Directory

	if (wcscmp(dest,src) == 0) 
	{
		//생성하고자 하는 모델이 이미 존재합니다.
		MyMessageBox(MY_MESSAGE_BOX,61503 ,_T("Manage Model"), M_ICONERROR);
		return ERR_SYSTEM_MODEL_CREATE_FAIL;
	}

	int iModelCount = util.GetDirCountInDir(m_pObjSystemData->m_strModelFileDir);

	if (iModelCount > DEF_MAX_MODEL)
	{
		CString strMsg;
		//모델 갯수가 시스템 허용치를 초과 합니다.
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_61504"), &strMsg);

		CString strNum;
		strNum.Format(_T("\r\nModel number(%d), System allowable value(%d)"), iModelCount, DEF_MAX_MODEL);

		strMsg += strNum;
		MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), _T("Manage Model"), M_ICONERROR);
		return	ERR_SYSTEM_MAX_COUNT_EXCEED_LIMIT;
	}

	strDest.Format(_T("%s/%s"), m_pObjSystemData->m_strModelFileDir, dest);

	if (!CreateDirectory(strDest, NULL)) // 모델 Directory를 생성한다.
	{
		//모델 디렉토리를 생성할수 없습니다.
		MyMessageBox(MY_MESSAGE_BOX,61505, _T("Manage Model"), M_ICONERROR);
		return ERR_SYSTEM_MODEL_CREATE_FAIL;
	}

	if (src != NULL && wcslen(src))	// 복사할 디렉토리가 지정되어 있으면 복사한다.
	{
		strSrc.Format(_T("%s/%s"), m_pObjSystemData->m_strModelFileDir, src);
		int iRet = util.CopyFilesInDir(strDest, strSrc, TRUE);
	}
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::ChangeModel(CString strModelName, BOOL m_NSMCAutoChange)
{
	int i = 0;

	if (MANUAL_MODE == m_pTrsAutoManager->GetOPMode())
	{
		// 자동운전 Flag를 Clear 하여 자동 운전 초기화를 새로 하게 한다.
		m_pTrsAutoManager->ResetUnitInitialFlag();
	}

	CString	strModelFileDir;
	CString strTemp, strTemp2;
	
	m_pObjSystemData->m_strModelFileName = strModelName;	// 시스템 데이타 모델 이름 변경


	/*
	//NSMC KJS
	if (MANUAL_MODE == m_pTrsAutoManager->GetOPMode())
	{
		// NSMC
		if (FALSE == m_pObjSystemData->m_strCurrentDeviceID.IsEmpty())
		{
			CString strCheckModelName = m_pTrsAutoManager->GetNewModelDataFolderName(m_pObjSystemData->m_strCurrentDeviceID);
			if (m_pObjSystemData->m_strModelFileName != strCheckModelName)
				m_pObjSystemData->m_strCurrentDeviceID = "MANUAL_MODEL_CHANGE";
		}
		else
			m_pObjSystemData->m_strCurrentDeviceID = "MANUAL_MODEL_CHANGE";
	}
	*/
	if (MANUAL_MODE == m_pTrsAutoManager->GetOPMode())
	{
		CString strtemp;
		strtemp.Format(_T("Manual Model Change."));
		m_pTrsAutoManager->WriteMonitoringLog(strtemp);
	}
	else
	{
		CString strtemp;
		strtemp.Format(_T("Auto Model Change."));
		m_pTrsAutoManager->WriteMonitoringLog(strtemp);
	}
	//___________________

	if (!m_pObjSystemData->WriteData())						// 변경된 내역을 파일에 저장
	{
		//새 Model 명을 SystemData 에 저장하지 못했습니다.
		MyMessageBox(MY_MESSAGE_BOX,61506, _T("Manage Model"), M_ICONERROR);
		return ERR_DATA_READ_FAILURE;
	}
	
	strModelFileDir.Format(_T("%s\\%s"), m_pObjSystemData->m_strModelFileDir, m_pObjSystemData->m_strModelFileName);
	
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Data Reading
	
	// 새로운 Model Panel Data Read
	m_pObjPanelData->SetFilePath(strModelFileDir);
	if (!m_pObjPanelData->ReadData())
	{
		//Panel Data Reading 에 실패했습니다.
		MyMessageBox(MY_MESSAGE_BOX,61507, _T("Data Reading Error"), M_ICONERROR);
		
		if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
			return ERR_DATA_READ_FAILURE;
	}
	
	m_pObjPreBonderData->SetFilePath(strModelFileDir);
	if (!m_pObjPreBonderData->ReadData())
	{
		//PreBonder Data Reading 에 실패했습니다.
		MyMessageBox(MY_MESSAGE_BOX,61508, _T("Data Reading Error"), M_ICONERROR);
		if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
			return ERR_DATA_READ_FAILURE;
	}

	m_pObjTabData->SetFilePath(strModelFileDir);
	if (!m_pObjTabData->ReadData())
	{
		//Tab Data Reading 에 실패했습니다.
		MyMessageBox(MY_MESSAGE_BOX,61509, _T("Data Reading Error"), M_ICONERROR);
		if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
			return ERR_DATA_READ_FAILURE;
	}

	m_pObjTabOffsetData->SetFilePath(strModelFileDir);
	if (!m_pObjTabOffsetData->ReadData())
	{
		//TabOffset Data Reading 에 실패했습니다.
		MyMessageBox(MY_MESSAGE_BOX,61510, _T("Data Reading Error"), M_ICONERROR);
		if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
			return ERR_DATA_READ_FAILURE;
	}

	m_pObjTabOffsetData_Rear->SetFilePath(strModelFileDir);
	if (!m_pObjTabOffsetData_Rear->ReadData())
	{
		//TabOffset Data Reading 에 실패했습니다.
		MyMessageBox(MY_MESSAGE_BOX,61523, _T("Data Reading Error"), M_ICONERROR);
		if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
			return ERR_DATA_READ_FAILURE;
	}

	// jdy sesl
/*	if (TRUE == IsNoWorkInAutoRunMode())
	{
		m_pObjSystemData->m_eRunMode = PASS_RUN_MODE;
		m_pObjSystemData->WriteData();
	}
	else
	{
		m_pObjSystemData->m_eRunMode = NORMAL_RUN_MODE;
		m_pObjSystemData->WriteData();
	}
*/
	//NSMC KJS Panel Data에서 처리
	if (PASS_RUN_MODE == m_pObjPanelData->m_eRunMode)
	{
		m_pObjSystemData->m_eRunMode = PASS_RUN_MODE;
		m_pObjSystemData->WriteData();
	}
	else
	{
		m_pObjSystemData->m_eRunMode = NORMAL_RUN_MODE;
		m_pObjSystemData->WriteData();
	}

	::SendMessage(m_pTrsAutoManager->GetAutoViewHandle(), WM_DISP_RUN_MODE, NULL, NULL);

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Change Process Data
	m_pObjProcessData->ChangeProcessData();
	AssignWorkScheduleData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_pObjWorkSchedule[i]->ChangeModel();
#ifdef DEF_SOURCE_SYSTEM
		m_pObjWorkScheduleSecond[i]->ChangeModel();
#endif
	}
	
	//170926 JSH.s
	//회피 위치 만큼 작업 거리에서 재외하기 위해 필요한 중복 코드
	AssignWorkScheduleData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		m_pObjWorkSchedule[i]->ChangeModel();
#ifdef DEF_SOURCE_SYSTEM
		m_pObjWorkScheduleSecond[i]->ChangeModel();
#endif
	}
	//170926 JSH.e


	
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Unit Model Change Function Call

	for (int i=0; i<DEF_MAX_TABFEEDER; i++)
	{
		for(int j=0; j<DEF_MAX_REEL; j++)
		{
			m_pCtrlTabFeeder[i]->m_matTabICInfo[j].ResetData();
		}
	}

	//150521 SDC_KSJ 기종변경 시 금형JIG 정보 삭제
	for (int i=0; i<DEF_MAX_TABFEEDER; i++)
	{
		m_pCtrlTabFeeder[i]->m_matZigInfo.ResetData();
	}

	// Vision Component
	int iResult = m_pVision->ChangeModel(strModelFileDir.GetBuffer(strModelFileDir.GetLength()));
	if (iResult)
	{
		if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
			return iResult;
	}
	// 각 Component 는 Model Offset 을 Load 하고 모델 좌표를 계산한다.
	iResult = m_pPanelAligner->ChangeModel();
	if (iResult)
	{
		if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
			return iResult;
	}

#ifdef DEF_SOURCE_SYSTEM
//@	m_pPanelTransferCenter->ChangeModel();
	m_pPanelTransfer->ChangeModel();
#endif
	m_pPanelTransferOut->ChangeModel();

//===================================================================
	

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		iResult = m_pTabCarrier[i]->ChangeModel();
		if (iResult)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
				return iResult;
		}
		iResult = m_pRTabCarrier[i]->ChangeModel();
		if (iResult)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
				return iResult;
		}
	}

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		//iResult = m_pTabFeeder[i]->ChangeModel();
		if (iResult)
		{
//			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
//				return iResult;
		}
		
		iResult = m_pCtrlTabFeeder[i]->ChangeModel();
		if (iResult)
		{
//			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
//				return iResult;
		}
		
		// hongju_SELSK : Change 모델 시 Sprocket 원점 복귀를 새로 할 필요는 없다..그냥 Find TabIC만 새로 하자..
//		if (MANUAL_MODE == m_pTrsAutoManager->GetOPMode())
		{
//			m_pCtrlTabFeeder[i]->SetHomeFlagSprocket1(FALSE);
			m_pCtrlTabFeeder[i]->SetHomeFlagSprocket2(FALSE);
			m_pCtrlTabFeeder[i]->SetFindTabICFlag(FALSE);
		}
	}

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		iResult = m_pTabMounter[i]->ChangeModel();
		if (iResult)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
				return iResult;
		}
		iResult = m_pRTabMounter[i]->ChangeModel();
		if (iResult)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
				return iResult;
		}
	}

	
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		iResult = m_pCameraCarrier[i]->ChangeModel();
		if (iResult)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
				return iResult;
		}
		iResult = m_pRCameraCarrier[i]->ChangeModel();
		if (iResult)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
				return iResult;
		}

		//SJ_YYK 160126 Add 단변 사용 시 문제발생.
		iResult = m_pCameraCarrier[i]->ChangeModel();
		if (iResult)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
				return iResult;
		}
	}

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		iResult = m_pModelChange[i]->ChangeModel();
		if (iResult)
		{
			if (MyMessageBox(MY_MESSAGE_BOX, 61522, _T("Reload Old Model"), M_ICONQUESTION|M_YESNO) == IDNO)
				return iResult;
		}
	}

	// AutoView Model Name Display
	::SendMessage(m_pTrsAutoManager->GetAutoViewHandle(), WM_DISP_MODEL_NAME, NULL, NULL);

	// AutoView DeviceID Display
	::SendMessage(m_pTrsAutoManager->GetAutoViewHandle(), WM_DISP_MODEL_DEVICEID, NULL, NULL);

	//120113.KMS__________
	m_pTrsAutoManager->SetModelChangeAlarmReport(TRUE);

	//120228.kms_________________
	// Model Change시간을 초기화 하여 Error를 Report하지 않는다.
	m_pTrsAutoManager->StartChangeModelTimer();

//101006.KKY	::PostMessage(m_pTrsAutoManager->GetAutoViewHandle(), WM_REMOVE_TABIC_REQ, FALSE, TRUE);

	ReportModel();	// Report Model Data

/*
	MFileManager::AutoDeleteFile("C:\\DataBackup\\*.*", 50, TRUE);
	// selsk_jdy
	MFileManager::AutoDeleteFile("D:\\DataBackup\\*.*", 50, TRUE);

	CTime t = CTime::GetCurrentTime();
	WCHAR dest[256];
	swprintf(dest, _T("C:\\DataBackup\\%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	MFileManager::CreateBackupFolder(dest, _T(".\\Data"), TRUE);
	// selsk_jdy
	swprintf(dest, _T("D:\\DataBackup\\%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	MFileManager::CreateBackupFolder(dest, _T(".\\Data"), TRUE);
*/	
	
	return ERR_SYSTEM_SUCCESS;
}

CString MPlatformOlbSystem::GetObjectName(int iErrorCode)
{
	CString strErrorCode;
	strErrorCode.Format(_T("%d"), iErrorCode);
	if (strErrorCode.GetLength() != 9)
		return "";
	
	int iComponentCode = _wtoi(strErrorCode.Left(3));
	int iInstanceNo = _wtoi(strErrorCode.Right(3));

	CString strObjName;
	int iResult = m_pObjSystemInfo->GetObjectName(iComponentCode, iInstanceNo, &strObjName);
	if (iResult != ERR_SYSTEMINFO_SUCCESS)
		return "";

	return strObjName;	
}

int MPlatformOlbSystem::createBasicHWComponents()
{
	int iError, i = 0;
	// selsk_jdy
	int iResult = SUCCESS;

	/********************************************************************************
	 *	I/O Component Create
	 ********************************************************************************/
	m_pIO = new MHilscherDnet(m_pObjSystemInfo->GetCommonData(DEF_IO_OBJECT_BASE),DEF_DEVICE_NET_BOARD_NUMBER_0); // Device Net B'd Number
	registerComponentInstance(1, m_pIO);
	
	iResult = m_pIO->Initialize();
	if (iResult)
		return iResult;

	m_pIO->LoadIOMap(DEF_SYSTEM_IO_MAP_FILE_NAME);
	m_pIO->RunIOThread();
	
	/********************************************************************************
	 *	Vision Component Create
	 ********************************************************************************/
	m_pVision = new MVision();
	
	SCommonAttribute commonData;
	commonData.m_iInstanceNo = 0;
	commonData.m_iLogKeepingDays = 7;
	commonData.m_iObjectID = 104;
	commonData.m_strLogFileName.Format(_T("Vision.dat"));
	commonData.m_strObjectName.Format(_T("Vision"));
	commonData.m_ucLevel = 0;
	m_pVision->CreateCommonObject(commonData);

	registerComponentInstance(DEF_VISION_OBJECT_BASE, m_pVision->GetCommonObject());

	for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
	{
		m_pCamera[i] = new ICommon(m_pObjSystemInfo->GetCommonData(DEF_VISION_OBJECT_BASE + i + 1));
		registerComponentInstance(DEF_VISION_OBJECT_BASE + i + 1, m_pCamera[i]);
	}

	CString strVisionDir(".\\Data");
	CString strModelName = m_pObjSystemData->m_strModelFileDir + "\\" + m_pObjSystemData->m_strModelFileName;
	CString strLogDir("..\\log");
	CString strImageDir("..\\LogImage");

	iResult = m_pVision->Initialize((LPCTSTR)strVisionDir, (LPCTSTR)strModelName, (LPCTSTR)strLogDir, (LPCTSTR)strImageDir);
	if (iResult)
		return iResult;

	/********************************************************************************
	 *	Sercos Board Library Create
	 ********************************************************************************/
	TRACE0("SERCOS Creation Start.\n");

	m_pSercosSetup = new MSercosSetup();
	registerComponentInstance(DEF_SERCOS_SETUP_OBJECT_BASE, m_pSercosSetup);

	//161024 Modify..
	/*
#ifndef SIMULATION
	iResult = initSercos();
	///if (iResult)
	///	return iResult;
#endif
*/

	m_pMotionLib = new MRS_NMCLib(m_pObjSystemInfo->GetCommonData(DEF_MOTION_LIB_OBJECT_BASE));
	iError = m_pObjSystemInfo->RegisterComponent(DEF_MOTION_OBJECT_BASE, m_pMotionLib);
	if(iError) MyMessageBox(MY_MESSAGE_BOX, 60413, _T("Motion Lib Error"), M_ICONERROR);

	m_pRS_NMCSetup = new MRS_NMCSetup();

	/** Motion 보드 관련설정을 Hard Coding으로 함 */
	/** Motion Library Parameter설정 */
	SMotionBoard	motionBoardData;

	memset(&motionBoardData, -1, sizeof(motionBoardData));

	motionBoardData.iMaxBoardNo = DEF_AXIS_SERCOS_NUM_MOTION_BOARD;
	motionBoardData.rglAddress[0] = 0x8000000;

	for (int i=0; i<motionBoardData.iMaxBoardNo; i++)
	{
		motionBoardData.rgbAutoCP[i] = FALSE;
		motionBoardData.rgiPC10IndexSel[i] = DEF_PC_INDEXSEL_0;
		motionBoardData.rgiPC10Axis[i][0] = -1;
		motionBoardData.rgiPC10Axis[i][1] = -1;
	}

	iError = m_pMotionLib->SetBoardParam(motionBoardData);
	if(iError) MyMessageBox(MY_MESSAGE_BOX, 60414, _T("Motion Lib Error"), M_ICONERROR);

	Sleep(300);

	for (int i=0; i<= DEF_AXIS_SERCOS_MAX_AXIS; i++)
	{
		m_pMotionLib->ClearStatus(i);
	}

	TRACE0("SERCOS Creation complete.");

	/********************************************************************************
	 *	MNetH(Melsec) Component Create : ObjectID 50
	 ********************************************************************************/

	// selsk_jdy
	TRACE0("Melsec Creation Start.");

	CString Path;
	Path.Format(_T(".\\Data\\MNetH.ini"));
	
	m_pMelsecIF = new MNetH(Path);
	m_pMelsecIF->Start();
	
	registerComponentInstance(DEF_MELSEC_OBJECT_BASE, m_pMelsecIF);

	Path.Format(_T(".\\Data\\MNetH2.ini"));
	
	m_pMelsecIF2 = new MNetH(Path);
	m_pMelsecIF2->Start();
	
	registerComponentInstance(DEF_MELSEC_OBJECT_BASE + 1, m_pMelsecIF2);

 	TRACE0("Melsec Creation Complete.");

	/********************************************************************************
	 *	Serial Object Component Create
	 ********************************************************************************/

	TRACE0("Serial Creation Start.");

	/*	COM3 - FRONT_BCR
		COM4 - FRONT_RFID
		COM5 - REAR_BCR		(Gate Only)
		COM6 - REAR_RFID	(Gate Only)
	 */
	SSerialPortData datComponent[DEF_MAX_SERIAL] = 
	{
		//DEF_SERIAL_BCR_READER, (Front)		
		{	
			3,
			CBR_9600,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			NOTUSE	//@ETX 미사용시
		},
		//DEF_SERIAL_RFID, (Front)			
		{	
			4,
			CBR_9600,
			EVENPARITY,
			7,
			TWOSTOPBITS,
			512,
			CR
		},

		//DEF_SERIAL_REAR_BCR_READER,		
		{	
			5,
			CBR_9600,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			NOTUSE	//@ETX 미사용시
		},
		//DEF_SERIAL_REAR_RFID,			
		{	
			6,
			CBR_9600,
			EVENPARITY,
			7,
			TWOSTOPBITS,
			512,
			CR
		},

		//DEF_SERIAL_LIGHT1
		{	
			3,
			CBR_19200,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			ETX,
		},

		//DEF_SERIAL_LIGHT2
		{	
			4,
			CBR_19200,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			ETX,
		},

		//DEF_SERIAL_LIGHT3
		{	
			5,
			CBR_19200,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			ETX,
		},


		//DEF_SERIAL_LIGHT4
		{	
			6,
			CBR_19200,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			ETX,
		},

		//DEF_SERIAL_LIGHT5
			{	
				7,
			CBR_19200,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			ETX,
		},

		//DEF_SERIAL_LIGHT6
		{	
			8,
			CBR_19200,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			ETX,
		},

		//DEF_SERIAL_LIGHT7
		{	
			9,
			CBR_19200,
			NOPARITY,
			8,
			ONESTOPBIT,
			512,
			ETX,
		},

	};

	for (int i = 0; i < DEF_MAX_SERIAL; i++)
	{
		m_pSerial[i] = new MComThread(m_pObjSystemInfo->GetCommonData(DEF_SERIAL_OBJECT_BASE + i), datComponent[i]);
		registerComponentInstance(DEF_SERIAL_OBJECT_BASE + i, m_pSerial[i]);

#ifndef SIMULATION
		//SJ_YYK 150821 Add....
		if( i == DEF_SERIAL_FRONT_BCR_READER
		 || i == DEF_SERIAL_FRONT_RFID
		 ||	i == DEF_SERIAL_REAR_BCR_READER
		 ||	i == DEF_SERIAL_REAR_RFID
		 || i == DEF_SERIAL_LIGHT4
		 || i == DEF_SERIAL_LIGHT5
		 || i == DEF_SERIAL_LIGHT6
		 || i == DEF_SERIAL_LIGHT7
		 )
		 continue;
		// selsk_jdy : ComThread Error 처리 - 표준화 방식 아님. 수정 필요.
		if (SUCCESS != m_pSerial[i]->OpenPort())	
		{
			CString strPortNo;
			strPortNo.Format(_T("ComPort[%d]"), datComponent[i].m_uPortNo);

			CString strMsg;
			// 를 여는데 실패하였습니다.
			ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_61519"), &strMsg);
			strMsg = strPortNo + strMsg;
			
			MyMessageBox(strMsg, _T("Error"), M_ICONQUESTION|M_OK);
		}
#endif
	}

/*	SSerialPortData datComponent;
	datComponent.m_uWritebuffersize = 512;
	datComponent.m_uParity = EVENPARITY;
	datComponent.m_uBaud = CBR_9600;
	datComponent.m_uDatabits = 7;
	datComponent.m_uPortNo = 2;
	datComponent.m_uStopbits = TWOSTOPBITS;
	datComponent.m_szEnd = CR;

	m_pSerial[] = new MSerialPort(m_pObjSystemInfo->GetCommonData(DEF_SERIAL_OBJECT_BASE),datComponent);
	registerComponentInstance(DEF_SERIAL_OBJECT_BASE, m_pSerial);
*/
	TRACE0("Serial Creation Complete.");


	/********************************************************************************
	 *	ACS Spii plus User Manager Reload
	 ********************************************************************************/
//@#ifdef DEF_GATE_SYSTEM

	//SJ_YYK 150908 Del..
	/*
    CString sExeName = "ACSCSRV.exe";

    sExeName.MakeUpper();
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if ((int)hSnapshot != -1)
    {
        PROCESSENTRY32 pe32 ;
        pe32.dwSize=sizeof(PROCESSENTRY32);
        BOOL bContinue ;
        CString strProcessName; 

        if (Process32First(hSnapshot, &pe32))
        {
            do
            {
                strProcessName = pe32.szExeFile; //strProcessName 이 프로세스 이름;
                strProcessName.MakeUpper();

                if ((strProcessName.Find(sExeName,0) != -1))
                {
                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pe32.th32ProcessID);
                    if (hProcess)
                    {
                        DWORD dwExitCode;
                        GetExitCodeProcess(hProcess, &dwExitCode);
                        TerminateProcess(hProcess, dwExitCode); 
                        CloseHandle(hProcess);
                        CloseHandle(hSnapshot);

                        // sesl_jdy return TRUE;
						return ERR_SYSTEM_SUCCESS;	// 정상적으로 Process 를 종료시킨 경우.
                    }

					// 확인 필요.
                    // sesl_jdy 삭제... return FALSE;
                } 

                bContinue = Process32Next(hSnapshot, &pe32);

            } while (bContinue);
        } 
        CloseHandle(hSnapshot);
    }
	*/

//@#endif
////////////////////////////////////////////////////////////////////////////////

	return ERR_SYSTEM_SUCCESS;
}

int	MPlatformOlbSystem::createCylinderComponents()
{
	// Cylinder 설정 테이블
	SCylinderVal sCylVal[DEF_CYL_MAX_CYLINDER] = 
	{
#ifdef DEF_SOURCE_SYSTEM
		{	DEF_CYL_PANEL_TRANSFER_IN_UD,
			_T("Panel Transfer In UD"),									// Cylinder Name
			OUT_TRANSFER_IN_UP, OUT_TRANSFER_IN_DOWN,					// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_TRANSFER_IN_1_UP, IN_TRANSFER_IN_2_UP, NULL, NULL,		// 1동작 감지 센서
			IN_TRANSFER_IN_1_DOWN, IN_TRANSFER_IN_2_DOWN, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, DOUBLE_SOLENOID							// 실린더 타입, Solenoid 타입
		},
#endif

		{	DEF_CYL_PANEL_TRANSFER_OUT_UD,
			_T("Panel Transfer Out UD"),								// Cylinder Name
			OUT_TRANSFER_OUT_UP, OUT_TRANSFER_OUT_DOWN,					// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_TRANSFER_OUT_1_UP, IN_TRANSFER_OUT_2_UP, NULL, NULL,		// 1동작 감지 센서
			IN_TRANSFER_OUT_1_DOWN, IN_TRANSFER_OUT_2_DOWN, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, DOUBLE_SOLENOID							// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_TABMOUNTER1_PRESS_UD,						// Cylinder ID
			_T("TabMounter1 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS1_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS1_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS1_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_TABMOUNTER2_PRESS_UD,						// Cylinder ID
			_T("TabMounter2 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS2_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS2_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS2_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},
	
#ifdef DEF_SOURCE_SYSTEM
		{	DEF_CYL_TABMOUNTER3_PRESS_UD,						// Cylinder ID
			_T("TabMounter3 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS3_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS3_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS3_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},
		
		{	DEF_CYL_TABMOUNTER4_PRESS_UD,						// Cylinder ID
			_T("TabMounter4 Press UD"),							// Cylinder Name
			OUT_TABMOUNTER1_PRESS4_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS4_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS4_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},
#endif

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_CYL_R_TABMOUNTER1_PRESS_UD,						// Cylinder ID
			_T("RTabMounter1 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS5_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS5_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS5_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},
		
		{	DEF_CYL_R_TABMOUNTER2_PRESS_UD,						// Cylinder ID
			_T("RTabMounter2 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS6_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS6_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS6_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},
		
		{	DEF_CYL_R_TABMOUNTER3_PRESS_UD,						// Cylinder ID
			_T("RTabMounter3 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS7_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS7_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS7_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_R_TABMOUNTER4_PRESS_UD,						// Cylinder ID
			_T("RTabMounter4 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS8_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS8_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS8_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},
#else
		{	DEF_CYL_R_TABMOUNTER1_PRESS_UD,						// Cylinder ID
			_T("RTabMounter1 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS3_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS3_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS3_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},
		
		{	DEF_CYL_R_TABMOUNTER2_PRESS_UD,						// Cylinder ID
			_T("RTabMounter2 Press UD"),						// Cylinder Name
			OUT_TABMOUNTER1_PRESS4_1ST_DOWN, NULL,				// 동작 Solenoid
			NULL, NULL,											// 가감속 Solenoid
			IN_TABMOUNTER1_PRESS4_1ST_UP, NULL, NULL, NULL,		// 1동작 감지 센서
			IN_TABMOUNTER1_PRESS4_1ST_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,								// 미들 센서
			NULL, NULL,											// 가감속 센서
			UP_DOWN, REVERSE_SINGLE_SOLENOID					// 실린더 타입, Solenoid 타입
		},

#endif

		{	DEF_CYL_INSPECTION_CAM1_ES,								// Cylinder ID
			_T("Inspection Cam1 ES"),										// Cylinder Name
			OUT_INSPECTION_CAM1_EXPAND, OUT_INSPECTION_CAM1_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_INSPECTION_CAM1_EXPAND, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_INSPECTION_CAM1_SHRINK, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_INSPECTION_CAM2_ES,								// Cylinder ID
			_T("Inspection Cam2 ES"),										// Cylinder Name
			OUT_INSPECTION_CAM2_EXPAND, OUT_INSPECTION_CAM2_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_INSPECTION_CAM2_EXPAND, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_INSPECTION_CAM2_SHRINK, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_CYL_INSPECTION_CAM3_ES,								// Cylinder ID
			_T("Inspection Cam3 ES"),										// Cylinder Name
			OUT_INSPECTION_CAM3_EXPAND, OUT_INSPECTION_CAM3_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_INSPECTION_CAM3_EXPAND, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_INSPECTION_CAM3_SHRINK, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_INSPECTION_CAM4_ES,								// Cylinder ID
			_T("Inspection Cam4 ES"),										// Cylinder Name
			OUT_INSPECTION_CAM4_EXPAND, OUT_INSPECTION_CAM4_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_INSPECTION_CAM4_EXPAND, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_INSPECTION_CAM4_SHRINK, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},
#endif

		{	DEF_CYL_R_INSPECTION_CAM1_ES,								// Cylinder ID
			_T("R Inspection Cam1 ES"),										// Cylinder Name
			OUT_R_INSPECTION_CAM1_EXPAND, OUT_R_INSPECTION_CAM1_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_R_INSPECTION_CAM1_EXPAND, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_R_INSPECTION_CAM1_SHRINK, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_R_INSPECTION_CAM2_ES,								// Cylinder ID
			_T("R Inspection Cam2 ES"),										// Cylinder Name
			OUT_R_INSPECTION_CAM2_EXPAND, OUT_R_INSPECTION_CAM2_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_R_INSPECTION_CAM2_EXPAND, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_R_INSPECTION_CAM2_SHRINK, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_CYL_R_INSPECTION_CAM3_ES,								// Cylinder ID
			_T("R Inspection Cam3 ES"),										// Cylinder Name
			OUT_R_INSPECTION_CAM3_EXPAND, OUT_R_INSPECTION_CAM3_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_R_INSPECTION_CAM3_EXPAND, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_R_INSPECTION_CAM3_SHRINK, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_R_INSPECTION_CAM4_ES,								// Cylinder ID
			_T("R Inspection Cam4 ES"),										// Cylinder Name
			OUT_R_INSPECTION_CAM4_EXPAND, OUT_R_INSPECTION_CAM4_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_R_INSPECTION_CAM4_EXPAND, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_R_INSPECTION_CAM4_SHRINK, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},
#endif
		
		{	DEF_CYL_TABCARRIER_UD,					// Cylinder ID
			_T("TabCarrier UD"),					// Cylinder Name
			OUT_TABCARRIER_UP_SOL, NULL,				// 동작 Solenoid
			NULL, NULL,								// 가감속 Solenoid
			IN_TABCARRIER_UP, NULL, NULL, NULL,	    // 1동작 감지 센서
			IN_TABCARRIER_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,					// 미들 센서
			NULL, NULL,								// 가감속 센서
			UP_DOWN, SINGLE_SOLENOID				// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_TABCARRIER_UD2,					// Cylinder ID
			_T("TabCarrier UD2"),					// Cylinder Name
			OUT_TABCARRIER_UP_SOL2, NULL,				// 동작 Solenoid
			NULL, NULL,								// 가감속 Solenoid
			IN_TABCARRIER_UP2, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_TABCARRIER_DOWN2, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,					// 미들 센서
			NULL, NULL,								// 가감속 센서
			UP_DOWN, SINGLE_SOLENOID				// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_TABCARRIER_FB,					// Cylinder ID
			_T("TabCarrier FB"),					// Cylinder Name
			OUT_TABCARRIER_FORWARD_SOL, NULL,				// 동작 Solenoid
			NULL, NULL,								// 가감속 Solenoid
			IN_TABCARRIER_FORWARD, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_TABCARRIER_BACKWARD, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,					// 미들 센서
			NULL, NULL,								// 가감속 센서
			UP_DOWN, SINGLE_SOLENOID				// 실린더 타입, Solenoid 타입
		},



		{	DEF_CYL_PREALIGN_CAM_ES,								// Cylinder ID
			_T("PreAlign Cam ES"),										// Cylinder Name
			OUT_PREALIGN_CAM1_EXPAND, OUT_PREALIGN_CAM1_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_PREALIGN_CAM1_1_EXPAND, IN_PREALIGN_CAM1_2_EXPAND, NULL, NULL,	// 1동작 감지 센서
			IN_PREALIGN_CAM1_1_SHRINK, IN_PREALIGN_CAM1_2_SHRINK, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_R_TABCARRIER_UD,					// Cylinder ID
			_T("R TabCarrier UD"),					// Cylinder Name
			OUT_R_TABCARRIER_UP_SOL, NULL,				// 동작 Solenoid
			NULL, NULL,								// 가감속 Solenoid
			IN_R_TABCARRIER_UP, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_R_TABCARRIER_DOWN, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,					// 미들 센서
			NULL, NULL,								// 가감속 센서
			UP_DOWN, SINGLE_SOLENOID				// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_R_TABCARRIER_UD2,					// Cylinder ID
			_T("R TabCarrier UD2"),					// Cylinder Name
			OUT_R_TABCARRIER_UP_SOL2, NULL,				// 동작 Solenoid
			NULL, NULL,								// 가감속 Solenoid
			IN_R_TABCARRIER_UP2, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_R_TABCARRIER_DOWN2, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,					// 미들 센서
			NULL, NULL,								// 가감속 센서
			UP_DOWN, SINGLE_SOLENOID				// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_R_TABCARRIER_FB,					// Cylinder ID
			_T("R TabCarrier FB"),					// Cylinder Name
			OUT_R_TABCARRIER_FORWARD_SOL, NULL,		// 동작 Solenoid
			NULL, NULL,								// 가감속 Solenoid
			IN_R_TABCARRIER_FORWARD, NULL, NULL, NULL,	// 1동작 감지 센서
			IN_R_TABCARRIER_BACKWARD, NULL, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,					// 미들 센서
			NULL, NULL,								// 가감속 센서
			UP_DOWN, SINGLE_SOLENOID				// 실린더 타입, Solenoid 타입
		},


		{	DEF_CYL_R_PREALIGN_CAM_ES,								// Cylinder ID
			_T("R PreAlign Cam ES"),										// Cylinder Name
			OUT_PREALIGN_CAM2_EXPAND, OUT_PREALIGN_CAM2_SHRINK,				// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_PREALIGN_CAM2_1_EXPAND, IN_PREALIGN_CAM2_2_EXPAND, NULL, NULL,	// 1동작 감지 센서
			IN_PREALIGN_CAM2_1_SHRINK, IN_PREALIGN_CAM2_2_SHRINK, NULL, NULL,	// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			EXPAND_SHRINK, DOUBLE_SOLENOID								// 실린더 타입, Solenoid 타입
		},

		{	DEF_CYL_TABMOUNTER1_PANEL_PUSHER_UD,
			_T("TabMounter1 Panel Pusher UD"),
			OUT_TABMOUNTER1_PANEL_PUSHER,	NULL,
			NULL,							NULL,
			IN_TABMOUNTER1_PANEL_PUSHER,	NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,
			UP_DOWN,						REVERSE_SINGLE_SOLENOID
		},

		{	DEF_CYL_TABMOUNTER2_PANEL_PUSHER_UD,
			_T("TabMounter2 Panel Pusher UD"),
			OUT_TABMOUNTER2_PANEL_PUSHER,	NULL,
			NULL,							NULL,
			IN_TABMOUNTER2_PANEL_PUSHER,	NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,
			UP_DOWN,						REVERSE_SINGLE_SOLENOID
		},

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_CYL_TABMOUNTER3_PANEL_PUSHER_UD,
			_T("TabMounter3 Panel Pusher UD"),
			OUT_TABMOUNTER3_PANEL_PUSHER,	NULL,
			NULL,							NULL,
			IN_TABMOUNTER3_PANEL_PUSHER,	NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,
			UP_DOWN,						REVERSE_SINGLE_SOLENOID
		},
		
		{	DEF_CYL_TABMOUNTER4_PANEL_PUSHER_UD,
			_T("TabMounter4 Panel Pusher UD"),
			OUT_TABMOUNTER4_PANEL_PUSHER,	NULL,
			NULL,							NULL,
			IN_TABMOUNTER4_PANEL_PUSHER,	NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,
			UP_DOWN,						REVERSE_SINGLE_SOLENOID
		},
#endif
		
		{	DEF_CYL_RTABMOUNTER1_PANEL_PUSHER_UD,
			_T("RTabMounter1 Panel Pusher UD"),
			OUT_RTABMOUNTER1_PANEL_PUSHER,	NULL,
			NULL,							NULL,
			IN_RTABMOUNTER1_PANEL_PUSHER,	NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,
			UP_DOWN,						REVERSE_SINGLE_SOLENOID
		},

		{	DEF_CYL_RTABMOUNTER2_PANEL_PUSHER_UD,
			_T("RTabMounter2 Panel Pusher UD"),
			OUT_RTABMOUNTER2_PANEL_PUSHER,	NULL,
			NULL,							NULL,

			IN_RTABMOUNTER2_PANEL_PUSHER,	NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,
			UP_DOWN,						REVERSE_SINGLE_SOLENOID
		},

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_CYL_RTABMOUNTER3_PANEL_PUSHER_UD,
			_T("RTabMounter3 Panel Pusher UD"),
			OUT_RTABMOUNTER3_PANEL_PUSHER,	NULL,
			NULL,							NULL,

			IN_RTABMOUNTER3_PANEL_PUSHER,	NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,
			UP_DOWN,						REVERSE_SINGLE_SOLENOID
		},
		
		{	DEF_CYL_RTABMOUNTER4_PANEL_PUSHER_UD,
			_T("RTabMounter4 Panel Pusher UD"),
			OUT_RTABMOUNTER4_PANEL_PUSHER,	NULL,
			NULL,							NULL,

			IN_RTABMOUNTER4_PANEL_PUSHER,	NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,	NULL,	NULL,
			NULL,							NULL,
			UP_DOWN,						REVERSE_SINGLE_SOLENOID
		},
#endif


		{	DEF_CYL_TRANSFER_IN_AIRRAIL_FB1,							// Cylinder ID
			_T("Transfer In AirRail FB1"),										// Cylinder Name
			OUT_TRANSFERIN_AIRRAIL_FOR1, OUT_TRANSFERIN_AIRRAIL_BACK1,						// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_TRANSFERIN_AIRRAIL_FOR1, NULL, NULL, NULL,			// 1동작 감지 센서
			IN_TRANSFERIN_AIRRAIL_BACK1, NULL, NULL, NULL,		// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			FRONT_BACK, DOUBLE_SOLENOID									// 실린더 타입, Soloenoid 타입
		},

		{	DEF_CYL_TRANSFER_IN_AIRRAIL_FB2,							// Cylinder ID
			_T("Transfer In AirRail FB2"),										// Cylinder Name
			OUT_TRANSFERIN_AIRRAIL_FOR2, OUT_TRANSFERIN_AIRRAIL_BACK2,						// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_TRANSFERIN_AIRRAIL_FOR2, NULL, NULL, NULL,			// 1동작 감지 센서
			IN_TRANSFERIN_AIRRAIL_BACK2, NULL, NULL, NULL,		// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			FRONT_BACK, DOUBLE_SOLENOID									// 실린더 타입, Soloenoid 타입
		},


#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
		{	DEF_CYL_MODELCHANGE_AIRRAIL_FB1,							// Cylinder ID
			_T("ModelChange AirRail FB1"),										// Cylinder Name
			OUT_MODELCHANGE_AIRRAIL_FOR1, OUT_MODELCHANGE_AIRRAIL_BACK1,						// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_MODELCHANGE_AIRRAIL_FOR1, NULL, NULL, NULL,			// 1동작 감지 센서
			IN_MODELCHANGE_AIRRAIL_BACK1, NULL, NULL, NULL,		// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			FRONT_BACK, DOUBLE_SOLENOID									// 실린더 타입, Soloenoid 타입
		},

		{	DEF_CYL_MODELCHANGE_AIRRAIL_FB2,							// Cylinder ID
			_T("ModelChange AirRail FB2"),										// Cylinder Name
			OUT_MODELCHANGE_AIRRAIL_FOR2, OUT_MODELCHANGE_AIRRAIL_BACK2,						// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_MODELCHANGE_AIRRAIL_FOR2, NULL, NULL, NULL,			// 1동작 감지 센서
			IN_MODELCHANGE_AIRRAIL_BACK2, NULL, NULL, NULL,		// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			FRONT_BACK, DOUBLE_SOLENOID									// 실린더 타입, Soloenoid 타입
		},
#endif
		//150727_KDH
		
		//SJ_YYK 161104 Add..
		{	DEF_CYL_PANEL_TRANSFER_CENTER_UD,									// Cylinder ID
			_T("Transfer Center"),										// Cylinder Name
			OUT_TRANSFER_CENTER_UP, OUT_TRANSFER_CENTER_DOWN,						// 동작 Solenoid
			NULL, NULL,													// 가감속 Solenoid
			IN_TRANSFER_CENTER_1_UP, IN_TRANSFER_CENTER_2_UP, NULL, NULL,			// 1동작 감지 센서
			IN_TRANSFER_CENTER_1_DOWN, IN_TRANSFER_CENTER_2_DOWN, NULL, NULL,		// 2동작 감지 센서
			NULL, NULL, NULL, NULL,										// 미들 센서
			NULL, NULL,													// 가감속 센서
			UP_DOWN, DOUBLE_SOLENOID									// 실린더 타입, Soloenoid 타입
		},
			
		{	DEF_CYL_TABPRESS1_RECOVERY_SPROCKET_UD,										// Cylinder ID
			_T("TabPress1 Recovery Sprocket UD"),										// Cylinder Name
			OUT_TABPRESS1_RECOVERY_SPROCKET_OPEN, OUT_TABPRESS1_RECOVERY_SPROCKET_CLOSE,// 동작 Solenoid
			NULL, NULL,																	// 가감속 Solenoid
			IN_TABPRESS1_RECOVERY_SPROCKET_OPEN, NULL, NULL, NULL,						// 1동작 감지 센서
			IN_TABPRESS1_RECOVERY_SPROCKET_CLOSE, NULL, NULL, NULL,						// 2동작 감지 센서
			NULL, NULL, NULL, NULL,														// 미들 센서
			NULL, NULL,																	// 가감속 센서
			UP_DOWN, DOUBLE_SOLENOID													// 실린더 타입, Solenoid 타입
		},
		{	DEF_CYL_R_TABPRESS1_RECOVERY_SPROCKET_UD,										// Cylinder ID
			_T("TabPress2 Recovery Sprocket UD"),										// Cylinder Name
			OUT_TABPRESS2_RECOVERY_SPROCKET_OPEN, OUT_TABPRESS2_RECOVERY_SPROCKET_CLOSE,// 동작 Solenoid
			NULL, NULL,																	// 가감속 Solenoid
			IN_TABPRESS2_RECOVERY_SPROCKET_OPEN, NULL, NULL, NULL,						// 1동작 감지 센서
			IN_TABPRESS2_RECOVERY_SPROCKET_CLOSE, NULL, NULL, NULL,						// 2동작 감지 센서
			NULL, NULL, NULL, NULL,														// 미들 센서
			NULL, NULL,																	// 가감속 센서
			UP_DOWN, DOUBLE_SOLENOID													// 실린더 타입, Solenoid 타입
		},
	};

	SCylinderData datComponent;
	datComponent.m_strFileName = DEF_SYSTEM_ACTUATOR_PARAMETER_FILE_NAME;

	for (int i = 0; i < DEF_CYL_MAX_CYLINDER; i++)
	{
		datComponent.m_sCylVal.m_iCylID = sCylVal[i].m_iCylID;
		wcscpy(datComponent.m_sCylVal.m_szName, sCylVal[i].m_szName);

		for (int j = 0; j < 2; j++)
		{
			datComponent.m_sCylVal.m_rgusSolenoid[j]	= sCylVal[i].m_rgusSolenoid[j];
			datComponent.m_sCylVal.m_rgusAccSolenoid[j]	= sCylVal[i].m_rgusAccSolenoid[j];
			datComponent.m_sCylVal.m_rgusAccSensor[j]	= sCylVal[i].m_rgusAccSensor[j];
		}
		
		for (int j = 0; j < DEF_MAX_CYLINDER_SENSOR; j++)
		{
			datComponent.m_sCylVal.m_rgusUpSensor[j]		= sCylVal[i].m_rgusUpSensor[j];
			datComponent.m_sCylVal.m_rgusDownSensor[j]		= sCylVal[i].m_rgusDownSensor[j];
			datComponent.m_sCylVal.m_rgusMiddleSensor[j]	= sCylVal[i].m_rgusMiddleSensor[j];
		}

		datComponent.m_sCylVal.m_lnkECylinderType = sCylVal[i].m_lnkECylinderType;
		datComponent.m_sCylVal.m_lnkESolenoidType = sCylVal[i].m_lnkESolenoidType;

		m_pCylinder[i] = new MCylinder(m_pObjSystemInfo->GetCommonData(DEF_CYLINDER_OBJECT_BASE + i), m_pIO, datComponent);
		registerComponentInstance(DEF_CYLINDER_OBJECT_BASE + i, m_pCylinder[i]);
		
		m_pCylinder[i]->LoadParameter();
	}
	return ERR_SYSTEM_SUCCESS;
}

int	MPlatformOlbSystem::createVacuumComponents()
{
	// Vacuum 설정 테이블
	SVacuumVal sVacVal[DEF_VAC_MAX_VACUUM] = 
	{
		/* Data structure 순서 설명.
		{	Vacuum ID
			Vacuum Name
			동작 Solenoid : BLOW가 있을 경우 마지막은 BLOW SOL
			감지 센서
			타입 : SINGLE_VACUUM/SINGLE_VACUUM_W_D_BLOW/SINGLE_VACUUM_W_I_BLOW/REVERSE_SINGLE_VACUUM
			      /DOUBLE_VACUUM/DOUBLE_VACUUM_W_D_BLOW/DOUBLE_VACUUM_W_I_BLOW
			      /HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM
		},
		*/
		{	DEF_VAC_PANEL_ALIGNER1,
			_T("PanelAligner Vac1"),
			OUT_STAGE_VACUUM1, OUT_STAGE_BLOW, NULL, NULL,
			IN_STAGE_VACUUM1, NULL, NULL, NULL,
			SINGLE_VACUUM_W_I_BLOW
		},

		{	DEF_VAC_PANEL_ALIGNER2,
			_T("PanelAligner Vac2"),
			OUT_STAGE_VACUUM2, OUT_STAGE_BLOW, NULL, NULL,
			IN_STAGE_VACUUM2, NULL, NULL, NULL,
			SINGLE_VACUUM_W_I_BLOW
		},

		{	DEF_VAC_PANEL_ALIGNER3,
			_T("PanelAligner Vac3"),
			OUT_STAGE_VACUUM3, OUT_STAGE_BLOW, NULL, NULL,
			IN_STAGE_VACUUM3, NULL, NULL, NULL,
			SINGLE_VACUUM_W_I_BLOW
		},

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_VAC_PANEL_TRANSFER_IN,
			_T("PanelTransfer In Vac"),
			OUT_TRANSFER_IN_VACUUM1, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TRANSFER_IN_VACUUM1, NULL, NULL, NULL,		// 감지 센서
			SINGLE_VACUUM								
		},

		{	DEF_VAC_PANEL_TRANSFER_IN2,
			_T("PanelTransfer In2 Vac"),
			OUT_TRANSFER_IN_VACUUM2, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TRANSFER_IN_VACUUM2, NULL, NULL, NULL,		// 감지 센서
			SINGLE_VACUUM								
		},
#endif

		{	DEF_VAC_PANEL_TRANSFER_OUT,
			_T("PanelTransfer Out Vac"),
			OUT_TRANSFER_OUT_VACUUM1, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TRANSFER_OUT_VACUUM1, NULL, NULL, NULL,		// 감지 센서
			SINGLE_VACUUM								
		},

		{	DEF_VAC_PANEL_TRANSFER_OUT2,
			_T("PanelTransfer Out2 Vac"),
			OUT_TRANSFER_OUT_VACUUM2, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TRANSFER_OUT_VACUUM2, NULL, NULL, NULL,		// 감지 센서
			SINGLE_VACUUM								
		},

		{	DEF_VAC_TAB_MOUNTER1,						// Vacuum ID
			_T("TabMounter1 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC1_ON, OUT_TABMOUNTER1_BLOW1_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM1, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_TAB_MOUNTER2,						// Vacuum ID
			_T("TabMounter2 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC2_ON, OUT_TABMOUNTER1_BLOW2_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM2, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_VAC_TAB_MOUNTER3,						// Vacuum ID
			_T("TabMounter3 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC3_ON, OUT_TABMOUNTER1_BLOW3_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM3, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_TAB_MOUNTER4,						// Vacuum ID
			_T("TabMounter4 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC4_ON, OUT_TABMOUNTER1_BLOW4_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM4, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},
#endif

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_VAC_R_TAB_MOUNTER1,						// Vacuum ID
			_T("RTabMounter1 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC5_ON, OUT_TABMOUNTER1_BLOW5_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM5, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_R_TAB_MOUNTER2,						// Vacuum ID
			_T("RTabMounter2 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC6_ON, OUT_TABMOUNTER1_BLOW6_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM6, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_R_TAB_MOUNTER3,						// Vacuum ID
			_T("RTabMounter3 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC7_ON, OUT_TABMOUNTER1_BLOW7_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM7, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_R_TAB_MOUNTER4,						// Vacuum ID
			_T("RTabMounter4 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC8_ON, OUT_TABMOUNTER1_BLOW8_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM8, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},
#else
		{	DEF_VAC_R_TAB_MOUNTER1,						// Vacuum ID
			_T("RTabMounter1 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC3_ON, OUT_TABMOUNTER1_BLOW3_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM3, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_R_TAB_MOUNTER2,						// Vacuum ID
			_T("RTabMounter2 Vac"),							// Vacuum Name
			OUT_TABMOUNTER1_VAC4_ON, OUT_TABMOUNTER1_BLOW4_ON, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABMOUNTER1_VACUUM4, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM_W_I_BLOW								// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},
#endif

		{	DEF_VAC_TABCARRIER1,							// Vacuum ID
			_T("TabCarrier1 Vac"),								// Vacuum Name
			OUT_TABCARRIER1_VAC_ON_OFF, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABCARRIER1_VAC_ON_OFF, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM									// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_TABCARRIER2,							// Vacuum ID
			_T("TabCarrier2 Vac"),								// Vacuum Name
			OUT_TABCARRIER2_VAC_ON_OFF, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABCARRIER2_VAC_ON_OFF, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM									// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_VAC_TABCARRIER3,							// Vacuum ID
			_T("TabCarrier3 Vac"),								// Vacuum Name
			OUT_TABCARRIER3_VAC_ON_OFF, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABCARRIER3_VAC_ON_OFF, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM									// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_TABCARRIER4,							// Vacuum ID
			_T("TabCarrier4 Vac"),								// Vacuum Name
			OUT_TABCARRIER4_VAC_ON_OFF, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABCARRIER4_VAC_ON_OFF, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM									// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},
#endif

	

		{	DEF_VAC_R_TABCARRIER1,							// Vacuum ID
			_T("R TabCarrier1 Vac"),								// Vacuum Name
			OUT_R_TABCARRIER1_VAC_ON_OFF, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_R_TABCARRIER1_VAC_ON_OFF, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM									// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_R_TABCARRIER2,							// Vacuum ID
			_T("R TabCarrier2 Vac"),								// Vacuum Name
			OUT_R_TABCARRIER2_VAC_ON_OFF, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_R_TABCARRIER2_VAC_ON_OFF, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM									// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

#ifdef DEF_SOURCE_SYSTEM
		{	DEF_VAC_R_TABCARRIER3,							// Vacuum ID
			_T("R TabCarrier3 Vac"),								// Vacuum Name
			OUT_R_TABCARRIER3_VAC_ON_OFF, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_R_TABCARRIER3_VAC_ON_OFF, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM									// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_R_TABCARRIER4,							// Vacuum ID
			_T("R TabCarrier4 Vac"),								// Vacuum Name
			OUT_R_TABCARRIER4_VAC_ON_OFF, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_R_TABCARRIER4_VAC_ON_OFF, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM									// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},
#endif

		//SJ_YYK 161104 Add..
		{	DEF_VAC_PANEL_TRANSFER_CENTER,
			_T("PanelTransfer Center Vac"),
			OUT_TRANSFER_CENTER_VACUUM1, NULL, NULL, NULL,
			IN_TRANSFER_CENTER_VACUUM1, NULL, NULL, NULL,
			SINGLE_VACUUM	
		},

		{	DEF_VAC_PANEL_TRANSFER_CENTER2,
			_T("PanelTransfer Center Vac2"),
			OUT_TRANSFER_CENTER_VACUUM2, NULL, NULL, NULL,
			IN_TRANSFER_CENTER_VACUUM2, NULL, NULL, NULL,
			SINGLE_VACUUM	
		},

		{	DEF_VAC_TLHANDLER_1,							// Vacuum ID
			_T("THandler Vac1"),								// Vacuum Name
			OUT_TURN_HANDLER_VACUUM1, NULL, NULL, NULL,
			IN_TURN_HANDLER_VACUUM1, NULL, NULL, NULL,
			REVERSE_SINGLE_VACUUM
		},

		{	DEF_VAC_TLHANDLER_2,							// Vacuum ID
			_T("THandler Vac2"),								// Vacuum Name
			OUT_TURN_HANDLER_VACUUM2, NULL, NULL, NULL,
			IN_TURN_HANDLER_VACUUM2, NULL, NULL, NULL,
			REVERSE_SINGLE_VACUUM
		},
		
		{	DEF_VAC_TLHANDLER_3,							// Vacuum ID
			_T("THandler Vac3"),								// Vacuum Name
			OUT_TURN_HANDLER_VACUUM3, NULL, NULL, NULL,
			IN_TURN_HANDLER_VACUUM3, NULL, NULL, NULL,
			REVERSE_SINGLE_VACUUM
		},
		//________________________*/


		//150728_KDH
		{	DEF_VAC_TAB_PRESS1,															// Vacuum ID
			_T("TabPress1 Vac"),															// Vacuum Name
			OUT_TABPRESS1_PRESS_VACUUM_ON, OUT_TABPRESS1_PRESS_VACUUM_BLOW, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABPRESS1_PRESS_VACUUM_ON, NULL, NULL, NULL,								// 감지 센서
			SINGLE_VACUUM_W_D_BLOW
		},
		{	DEF_VAC_TAB_PRESS2,															// Vacuum ID
			_T("TabPress2 Vac"),															// Vacuum Name
			OUT_TABPRESS2_PRESS_VACUUM_ON, OUT_TABPRESS2_PRESS_VACUUM_BLOW, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABPRESS2_PRESS_VACUUM_ON, NULL, NULL, NULL,								// 감지 센서
			SINGLE_VACUUM_W_D_BLOW
		},
		//__________
	
		/*/
		{	DEF_VAC_TAB_PRESS1_NEW_REEL,								// Vacuum ID
			_T("TabPress1 New Reel Vac"),									// Vacuum Name
			OUT_TABPRESS1_ATTACH_NEW_REEL_VACCUM_ON, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABPRESS1_ATTACH_NEW_REEL_VACCUM_ON, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM												// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},
		
		
		{	DEF_VAC_TAB_PRESS2_NEW_REEL,								// Vacuum ID
			_T("TabPress2 New Reel Vac"),									// Vacuum Name
			OUT_TABPRESS2_ATTACH_NEW_REEL_VACCUM_ON, NULL, NULL, NULL,	// 동작 Solenoid, BLOW가 있을 경우 마지막은 BLOW SOL
			IN_TABPRESS2_ATTACH_NEW_REEL_VACCUM_ON, NULL, NULL, NULL,	// 감지 센서
			SINGLE_VACUUM												// 타입 SINGLE_VACUUM/SINGLE_VACUUM_WBLOW/DOUBLE_VACUUM/DOUBLE_VACUUM_WBLOW/HETERO_DOUBLE_VACUUM/REVERSE_DOUBLE_VACUUM/REVERSE_SINGLE_VACUUM
		},
		/*/
		//___________________
	};

	SVacuumData	datComponent;
	datComponent.m_strFileName = DEF_SYSTEM_ACTUATOR_PARAMETER_FILE_NAME;

	for (int i = 0; i < DEF_VAC_MAX_VACUUM; i++)
	{
		datComponent.m_sVacVal.m_iVacID = sVacVal[i].m_iVacID;
		wcscpy(datComponent.m_sVacVal.m_szName, sVacVal[i].m_szName);

		for (int j = 0; j < DEF_MAX_VACUUM_SOLENOID; j++)	
			datComponent.m_sVacVal.m_rgusSolenoid[j] = sVacVal[i].m_rgusSolenoid[j];
		for (int j = 0; j < DEF_MAX_VACUUM_SENSOR; j++)
			datComponent.m_sVacVal.m_rgusSensor[j] = sVacVal[i].m_rgusSensor[j];

		datComponent.m_sVacVal.m_lnkVacuumType = sVacVal[i].m_lnkVacuumType;

		m_pVacuum[i] = new MVacuum(m_pObjSystemInfo->GetCommonData(DEF_VACUUM_OBJECT_BASE + i), m_pIO, datComponent);
		registerComponentInstance(DEF_VACUUM_OBJECT_BASE + i, m_pVacuum[i]);

		m_pVacuum[i]->LoadParameter();
	}
	return ERR_SYSTEM_SUCCESS;
}

int	MPlatformOlbSystem::createMotionComponents()
{
	////////////// ACS Spii plus User Manager Reload  ////////////////////
//@#if (defined(DEF_GATE_SYSTEM) && !defined(SIMULATION))
#ifndef SIMULATION

	//SJ_YYK 150908 Modify...

	CString sExeName = "ACSCSRV.exe";

    sExeName.MakeUpper();
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	BOOL bRunningFlag = FALSE;

    if ((int)hSnapshot != -1)
    {
        PROCESSENTRY32 pe32 ;
        pe32.dwSize=sizeof(PROCESSENTRY32);
        BOOL bContinue ;
        CString strProcessName; 

        if (Process32First(hSnapshot, &pe32))
        {
            do
            {
                strProcessName = pe32.szExeFile; //strProcessName 이 프로세스 이름;
                strProcessName.MakeUpper();

                if ((strProcessName.Find(sExeName,0) != -1))
                {
					bRunningFlag = TRUE;
                } 

                bContinue = Process32Next(hSnapshot, &pe32);

            } while (bContinue);
        } 
        CloseHandle(hSnapshot);
    }


	if(bRunningFlag == FALSE)
	{
		SHELLEXECUTEINFO sei = { 0 };

		sei.cbSize = sizeof (SHELLEXECUTEINFO);
		sei.fMask  = SEE_MASK_NOCLOSEPROCESS;
		sei.lpVerb = _T("open");
 
		// 파일 경로를 입력하세요.
	//    sei.lpFile = _T("C:\\Program Files\\ACS Motion Control\\SPiiPlus 5.20\\BIN\\ACSCSRV.exe");
	//    sei.lpFile = _T("C:\\Program Files\\ACS Motion Control\\SPiiPlus 6.00\\BIN\\ACSCSRV.exe");
		//sei.lpFile = _T("C:\\Program Files\\ACS Motion Control\\SPiiPlus 6.60\\BIN\\ACSCSRV.exe");
		//sei.lpFile = _T("C:\\Program Files\\ACS Motion Control\\SPiiPlus NT Suite 2.20\\Redist\\x86\\ACSCSRV.exe");
		sei.lpFile = _T("C:\\Program Files\\ACS Motion Control\\SPiiPlusNT Suite 2.28\\Redist\\x86\\ACSCSRV.exe");
		sei.nShow  = SW_SHOWNORMAL;

		if (ShellExecuteEx (&sei))
		{
		}
	}
 
    Sleep(100);
#endif
	//////////////////////////////////////////////////////////////////////

	int i = 0;
	SSMotionData datComponent[DEF_AXIS_MAX_AXIS];
	
	/*
		Sercos Servo Driver Type ( Yaskawa :1 , RSA(CSDM): 2, PulseModule: 3, Etc:4 , default : 0)
	*/

	// Motion에 설정해야 될 데이타 테이블
	SAxis1	sAxisVal[DEF_AXIS_MAX_AXIS] =
	{
		//	Axis	NAME				  Origin	Sign	Origin	Origin	Use							Role Of					Scale						ABS		Ring 	Node	Servo
		//   ID							  Priority			Dir		FineDir	CPhase Type					Home Sensor													No		No		Type
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
		{	0,		_T("PanelStage Y1"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NONE,			131072.0 / 2.0,			FALSE,		0,		1,		1,	},
		{	1,		_T("PanelStage Y2"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NONE,			131072.0 / 2.0,			FALSE,		0,		2,		1,	},
		{	2,		_T("PanelStage X"),			1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NONE,			1000.0 / 1.0,			FALSE,		0,		3,		1,	},
		{	3,		_T("PanelStage Z"),			3,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NONE,			131072.0 / 2.0 * 3.0,	FALSE,		0,		5,		1,	},

	    {	4,		_T("Transfer In X"),		3,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			(131072.0*5.0)/(38.20*PI),	FALSE,	0,		6,		1,	}, //170208_KDH 39.79 -> 38.20 변경
		{	5,		_T("Transfer Out X"),		4,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			(131072.0*5.0)/(38.20*PI),	FALSE,	0,		7,		1,	}, //170208_KDH 39.79 -> 38.20 변경`
		{	6,		_T("Model Change1"),		0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0*15.0 / 5.0,		TRUE,	0,		8,		1,	},
		{	7,		_T("Model Change2"),		0,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0*15.0 / 5.0,		TRUE,	0,		9,		1,	},

		{	8,		_T("TabMounter1 X"),		1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,			FALSE,		0,		10,		1,	},
		{	9,		_T("TabMounter2 X"),		2,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,			FALSE,		0,		11,		1,	},
		{	10,		_T("TabMounter3 X"),		3,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,			FALSE,		0,		12,		1,	},
		{	11,		_T("TabMounter4 X"),		4,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,			FALSE,		0,		13,		1,	},

		{	12,		_T("TabMounter1 Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_N_MARKER,		HOME_N_NONE,	(131072.0)/2.0,			FALSE,		0,		14,		4,	},
		{	13,		_T("TabMounter2 Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_N_MARKER,		HOME_N_NONE,	(131072.0)/2.0,			FALSE,		0,		15,		4,	},
		{	14,		_T("TabMounter3 Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_N_MARKER,		HOME_N_NONE,	(131072.0)/2.0,			FALSE,		0,		16,		4,	},
		{	15,		_T("TabMounter4 Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_N_MARKER,		HOME_N_NONE,	(131072.0)/2.0,			FALSE,		0,		17,		4,	},

		{	16,		_T("BackUp1 Z"),			0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,		HOME_N_NEGATIVE_LIMIT,	131072.0 / 2.0,			FALSE,		0,		18,		1,	},
		{	17,		_T("BackUp2 Z"),			0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,		HOME_N_NEGATIVE_LIMIT,	131072.0 / 2.0,			FALSE,		0,		19,		1,	},
		{	18,		_T("BackUp3 Z"),			0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,		HOME_N_NEGATIVE_LIMIT,	131072.0 / 2.0,			FALSE,		0,		20,		1,	},
		{	19,		_T("BackUp4 Z"),			0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,		HOME_N_NEGATIVE_LIMIT,	131072.0 / 2.0,			FALSE,		0,		21,		1,	},
	
		{	20,		_T("Inspection Camera1 X"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			1.0 / 0.0002,			FALSE,		0,		22,		4,	},
		{	21,		_T("Inspection Camera2 X"),	2,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			1.0 / 0.0002,			FALSE,		0,		23,		4,	},
		{	22,		_T("Inspection Camera3 X"),	3,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			1.0 / 0.0002,			FALSE,		0,		24,		4,	},
		{	23,		_T("Inspection Camera4 X"),	4,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			1.0 / 0.0002,			FALSE,		0,		25,		4,	},
		
		{	24,		_T("RTabMounter1 X"),		5,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,				FALSE,	1,		26,		1,	},
		{	25,		_T("RTabMounter2 X"),		6,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,				FALSE,	1,		27,		1,	},
		{	26,		_T("RTabMounter3 X"),		7,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,				FALSE,	1,		28,		1,	},
		{	27,		_T("RTabMounter4 X"),		8,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,				FALSE,	1,		29,		1,	},

		{	28,		_T("RTabMounter1 Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_F_LIMIT,		HOME_N_NONE,			(131072.0)/2.0,				FALSE,	1,		30,		4,	},
		{	29,		_T("RTabMounter2 Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_F_LIMIT,		HOME_N_NONE,			(131072.0)/2.0,				FALSE,	1,		31,		4,	},
		{	30,		_T("RTabMounter3 Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_F_LIMIT,		HOME_N_NONE,			(131072.0)/2.0,				FALSE,	1,		32,		4,	},
		{	31,		_T("RTabMounter4 Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_F_LIMIT,		HOME_N_NONE,			(131072.0)/2.0,				FALSE,	1,		33,		4,	},
		
		{	32,		_T("RBackUp1 Z"),			0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_F_LIMIT,		HOME_N_NEGATIVE_LIMIT,	131072.0 / 2.0,				FALSE,	1,		34,		1,	},
		{	33,		_T("RBackUp2 Z"),			0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_F_LIMIT,		HOME_N_NEGATIVE_LIMIT,	131072.0 / 2.0,				FALSE,	1,		35,		1,	},
		{	34,		_T("RBackUp3 Z"),			0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_F_LIMIT,		HOME_N_NEGATIVE_LIMIT,	131072.0 / 2.0,				FALSE,	1,		36,		1,	},
		{	35,		_T("RBackUp4 Z"),			0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_F_LIMIT,		HOME_N_NEGATIVE_LIMIT,	131072.0 / 2.0,				FALSE,	1,		37,		1,	},


		{	36,		_T("RInspection Camera1 X"),	1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 10.0,				FALSE,	0,		38,		4,	},
//		{	36,		_T("RInspection Camera1 X"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 10.0,				FALSE,	0,		38,		4,	},
		{	37,		_T("RInspection Camera2 X"),	2,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			1.0 / 0.0002,				FALSE,	0,		39,		4,	},
		{	38,		_T("RInspection Camera3 X"),	3,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			1.0 / 0.0002,				FALSE,	0,		40,		4,	},
		{	39,		_T("RInspection Camera4 X"),	4,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			1.0 / 0.0002,				FALSE,	0,		41,		4,	},

		
		//170711 JSH.s
		//ABS축에서 일반축으로 변경
		/*/
		{	40,		_T("TabCarrier1 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			TRUE,		0,		42,		1,	},
		{	41,		_T("TabCarrier2 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			TRUE,		0,		43,		1,	},
		{	42,		_T("TabCarrier3 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			TRUE,		0,		44,		1,	},
		{	43,		_T("TabCarrier4 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			TRUE,		0,		45,		1,	},
		/*/
		{	40,		_T("TabCarrier1 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			FALSE,		0,		42,		1,	},
		{	41,		_T("TabCarrier2 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			FALSE,		0,		43,		1,	},
		{	42,		_T("TabCarrier3 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			FALSE,		0,		44,		1,	},
		{	43,		_T("TabCarrier4 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			FALSE,		0,		45,		1,	},
		//170711 JSH.s

		{	44,		_T("TabCarrier1 X"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0005,			FALSE,		0,		46,		1,	},
		{	45,		_T("TabCarrier2 X"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0005,			FALSE,		0,		47,		1,	},  //SJ_YYK 150109 Modify..
		{	46,		_T("TabCarrier3 X"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0005,			FALSE,		0,		48,		1,	},
		{	47,		_T("TabCarrier4 X"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0005,			FALSE,		0,		49,		1,	},
		
		//170711 JSH.s
		//Carrier Home +, H 센서 사용으로 통일
		/*/
		{	48,		_T("TabCarrier1 T"),		1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		8000.0 * 50.0 / 360.0,		FALSE,		0,		50,		1,	},
		{	49,		_T("TabCarrier2 T"),		1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		8000.0 * 50.0 / 360.0,		FALSE,		0,		51,		1,	},
		{	50,		_T("TabCarrier3 T"),		1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		8000.0 * 50.0 / 360.0,		FALSE,		0,		52,		1,	},
		{	51,		_T("TabCarrier4 T"),		1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		8000.0 * 50.0 / 360.0,		FALSE,		0,		53,		1,	},
		/*/
		{	48,		_T("TabCarrier1 T"),		1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		8000.0 * 50.0 / 360.0,		FALSE,		0,		50,		1,	},
		{	49,		_T("TabCarrier2 T"),		1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		8000.0 * 50.0 / 360.0,		FALSE,		0,		51,		1,	},
		{	50,		_T("TabCarrier3 T"),		1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		8000.0 * 50.0 / 360.0,		FALSE,		0,		52,		1,	},
		{	51,		_T("TabCarrier4 T"),		1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		8000.0 * 50.0 / 360.0,		FALSE,		0,		53,		1,	},
		//170711 JSH.e

		{	52,		_T("Inspection Expand1"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 4.0,			FALSE,		0,		54,		1,	},
		{	53,		_T("Inspection Expand2"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 4.0,			FALSE,		0,		55,		1,	},
		{	54,		_T("Inspection Expand3"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 4.0,			FALSE,		0,		56,		1,	},
		{	55,		_T("Inspection Expand4"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 4.0,			FALSE,		0,		57,		1,	},
		{	56,		_T("TabCamera Expand1"),	1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 4.0,			FALSE,		0,		58,		1,	},

		{	57,		_T("R TabCarrier1 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			TRUE,		3,		59,		1,	},
		{	58,		_T("R TabCarrier2 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			TRUE,		3,		60,		1,	},
		{	59,		_T("R TabCarrier3 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			TRUE,		3,		61,		1,	},
		{	60,		_T("R TabCarrier4 Y"),		0,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			TRUE,		3,		62,		1,	},

		//{	61,		_T("R Carrier Z"),			1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 5.0,			FALSE,		3,		63,		1,	},
		{	61,		_T("R TabCarrier1 X"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0005,			FALSE,		3,		63,		1,	},
		{	62,		_T("R TabCarrier2 X"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,			FALSE,		3,		64,		1,	},  //SJ_YYK 150109 Modify..
		{	64,		_T("R TabCarrier3 X"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,			FALSE,		3,		65,		1,	},
		{	64,		_T("R TabCarrier4 X"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			1.0 / 0.0002,			FALSE,		3,		66,		1,	},
		
		//170711 JSH.s
		//Carrier Home +, H 센서 사용으로 통일
		/*/
		{	65,		_T("R TabCarrier1 T"),		1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			8000.0 * 50.0 / 360.0,	FALSE,		3,		67,		1,	},
		{	66,		_T("R TabCarrier2 T"),		1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			8000.0 * 50.0 / 360.0,	FALSE,		3,		68,		1,	},
		{	67,		_T("R TabCarrier3 T"),		1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			8000.0 * 50.0 / 360.0,	FALSE,		3,		69,		1,	},
		{	68,		_T("R TabCarrier4 T"),		1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			8000.0 * 50.0 / 360.0,	FALSE,		3,		70,		1,	},
		/*/
		{	65,		_T("R TabCarrier1 T"),		1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			8000.0 * 50.0 / 360.0,	FALSE,		3,		67,		1,	},
		{	66,		_T("R TabCarrier2 T"),		1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			8000.0 * 50.0 / 360.0,	FALSE,		3,		68,		1,	},
		{	67,		_T("R TabCarrier3 T"),		1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			8000.0 * 50.0 / 360.0,	FALSE,		3,		69,		1,	},
		{	68,		_T("R TabCarrier4 T"),		1,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			8000.0 * 50.0 / 360.0,	FALSE,		3,		70,		1,	},
		//170711 JSH.e

		{	69,		_T("R Inspection Expand1"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 4.0,			FALSE,		0,		71,		1,	},
		{	70,		_T("R Inspection Expand2"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 4.0,			FALSE,		3,		72,		1,	},
		{	71,		_T("R Inspection Expand3"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 4.0,			FALSE,		3,		73,		1,	},
		{	72,		_T("R Inspection Expand4"),	1,		TRUE,	FALSE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 4.0,			FALSE,		3,		74,		1,	},
		{	73,		_T("R TabCamera Expand1"),	1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 4.0,			FALSE,		3,		75,		1,	},
	
		{	74,		_T("TabCamera Expand2"),	1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 4.0,			FALSE,		0,		76,		1,	},
		{	75,		_T("R TabCamera Expand2"),	1,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,	HOME_N_NONE,			131072.0 / 4.0,			FALSE,		3,		77,		1,	},
#ifdef DEF_USE_TRANSFER_CENTER
		{	76,		_T("Transfer Center X1"),	3,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0*5.0)/(38.20*PI),	FALSE,	0,		78,		1,	}, //170208_KDH 39.79 -> 38.20 변경
		{	77,		_T("Transfer Center X2"),	4,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0*5.0)/(38.20*PI),	FALSE,	0,		79,		1,	}, //170208_KDH 39.79 -> 38.20 변경
#else
		{	76,		_T("Spare 76"),				3,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0*5.0)/(38.20*PI),	FALSE,	0,		78,		1,	}, //170208_KDH 39.79 -> 38.20 변경
		{	77,		_T("Spare 77"),				4,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0*5.0)/(38.20*PI),	FALSE,	0,		79,		1,	}, //170208_KDH 39.79 -> 38.20 변경
#endif

#ifdef DEF_USE_TURN_HANDLER
		{	78,		_T("Turn Handler Z"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0*25.0)/6.0,		FALSE,	0,		81,		1,	},	
		{	79,		_T("Turn Handler T"),		2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0/(3.141592*96/10.0))*(544*3.141592/360),			FALSE,	0,		80,		1,	},
#else
		{	78,		_T("Spare 78"),				0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		131072.0/1.0,			FALSE,	0,		80,		1,	},	
		{	79,		_T("Spare 79"),				2,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0/(3.141592*96/10.0))*(544*3.141592/360),			FALSE,	0,		81,		1,	},
#endif
		{	80,		_T("Transfer In X2"),		3,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0*5.0)/(38.20*PI),	FALSE,	0,		82,		1,	}, //170208_KDH 39.79 -> 38.20 변경

#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
		{	81,		_T("Transfer Out X2"),		4,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0*5.0)/(38.20*PI),	FALSE,	0,		83,		1,	}, //170208_KDH 39.79 -> 38.20 변경`
#else
		{	81,		_T("Spare 81"),				4,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,		(131072.0*5.0)/(38.20*PI),	FALSE,	0,		83,		1,	}, //170208_KDH 39.79 -> 38.20 변경`
#endif

		{	0,		_T("Gantry1"),				0,		TRUE,	FALSE,	TRUE,	DEF_USE_MARKER_ONLY,		HOME_N_NONE,			1.0 / 1.0,				FALSE,		0,		83,		1,	},
		{	1,		_T("Gantry2"),				0,		TRUE,	TRUE,	FALSE,	DEF_USE_MARKER_ONLY,		HOME_N_NONE,			1.0 / 1.0,				FALSE,		0,		84,		1,	},

		//150727_KDH
		{	81,		_T("BadTab Detector X"),	0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NEGATIVE_LIMIT,	131072.0/1.0,			FALSE,		2,		42,		1,	},
		{	82,		_T("Sprocket1 T"),			8,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NONE,			(131072.0*45.0)/360.0,	FALSE,		2,		43,		1,	},
		{	83,		_T("Sprocket2 T"),			8,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NONE,			(131072.0*45.0)/360.0,	FALSE,		2,		44,		1,	},
		{	84,		_T("TabPress Z"),			0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			(131072.0*5.0) /360.0,	FALSE,		2,		45,		1,	},
		{	85,		_T("Feeder Reel T"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_POSITIVE_LIMIT,	131072.0*10.0 / 360.0,	FALSE,		2,		46,		1,	},		
		{	86,		_T("Molding UpDown Z"),		0,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			FALSE,		2,		47,		1,	},


		{	87,		_T("R BadTab Detector X"),	0,		TRUE,	FALSE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NEGATIVE_LIMIT,	131072.0/1.0,			FALSE,		3,		65,		1,	},
		{	88,		_T("R Sprocket1 T"),		8,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NONE,			(131072.0*45.0)/360.0,	FALSE,		3,		66,		1,	},
		{	89,		_T("R Sprocket2 T"),		8,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_N_MARKER,	HOME_N_NONE,			(131072.0*45.0)/360.0,	FALSE,		3,		67,		1,	},
		{	90,		_T("R TabPress Z"),			0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			(131072.0*5.0) / 360.0,	FALSE,		3,		68,		1,	},
		{	91,		_T("R Feeder Reel T"),		0,		TRUE,	TRUE,	FALSE,	DEF_USE_SWITCH_ONLY,		HOME_N_POSITIVE_LIMIT,	131072.0*10.0 / 360.0,	FALSE,		3,		69,		1,	},
		{	92,		_T("R Molding UpDown Z"),	0,		TRUE,	TRUE,	TRUE,	DEF_USE_SWITCH_ONLY,		HOME_N_NONE,			131072.0 / 2.0,			FALSE,		3,		70,		1,	},
	};

	// Sercos Type
	for (int i = 0; i < DEF_AXIS_SERCOS_MAX_AXIS; i++)
	{
		datComponent[i].m_sAxisVal		= sAxisVal[i];
		datComponent[i].m_strFileName	= DEF_SYSTEM_MOTION_PARAMETER_FILE_NAME;

		//161024 Modify...
		//m_pMotion[i] = new MSercosAxis(m_pObjSystemInfo->GetCommonData(DEF_MOTION_OBJECT_BASE + i), datComponent[i], m_pIO, m_pSercosSetup);
		m_pMotion[i] = new MRSNMC(m_pObjSystemInfo->GetCommonData(DEF_MOTION_OBJECT_BASE + i), datComponent[i], m_pIO, m_pSercosSetup);

		registerComponentInstance(DEF_MOTION_OBJECT_BASE + i, m_pMotion[i]);

		m_pMotion[i]->LoadParameter();
		
		if (TRUE == IsSkipAxis(i))
			continue;
	}

//@#ifdef DEF_GATE_SYSTEM
	SACSCommSet	sCommSet[DEF_AXIS_ACS_MAX_AXIS];

	sCommSet[0].strIPAddr = _T("10.0.0.100");
	sCommSet[0].Handle = NULL;

	sCommSet[1].strIPAddr = _T("10.0.0.101");
	sCommSet[1].Handle = NULL;

	// ACS Type
	//SJ_YYK 150817 Modify..
	//for (int i = DEF_AXIS_ACS_START; i < DEF_AXIS_MAX_AXIS; i++)
	for (int i = DEF_AXIS_ACS_START; i < DEF_AXIS_ACS_END; i++)
	{
		datComponent[i].m_sAxisVal		= sAxisVal[i];
		datComponent[i].m_strFileName	= DEF_SYSTEM_MOTION_PARAMETER_FILE_NAME;
		datComponent[i].m_iACSMotionID	= ACSC_AXIS_X + (i - DEF_AXIS_ACS_START);
		datComponent[i].m_iACSMotionInstance = i-DEF_AXIS_ACS_START;
//@		m_pMotion[i] = new MACSAxis(m_pObjSystemInfo->GetCommonData(DEF_MOTION_OBJECT_BASE + i), datComponent[i], GetIOComponent(), sCommSet[i - DEF_AXIS_ACS_START]);
		m_pMotion[i] = new MACSAxis(m_pObjSystemInfo->GetCommonData(DEF_MOTION_OBJECT_BASE + i), datComponent[i], GetIOComponent(), sCommSet[0]);
		registerComponentInstance(DEF_MOTION_OBJECT_BASE + i, m_pMotion[i]);

		m_pMotion[i]->LoadParameter();
	}
//@#endif

	//SJ_YYK 150817 Dummy Assign
	for (int i= DEF_AXIS_DUMMY; i< DEF_AXIS_MAX_AXIS; i++)
	{
		datComponent[i].m_sAxisVal		= sAxisVal[i];
		datComponent[i].m_strFileName	= DEF_SYSTEM_MOTION_PARAMETER_FILE_NAME;

		m_pMotion[i] = new MSercosAxis(m_pObjSystemInfo->GetCommonData(DEF_MOTION_OBJECT_BASE + i), datComponent[i], m_pIO, m_pSercosSetup);
		registerComponentInstance(DEF_MOTION_OBJECT_BASE + i, m_pMotion[i]);

		m_pMotion[i]->LoadParameter();
	}
	//____________________________*/

	
	IAxis* pMotion[2];
	pMotion[0] = m_pMotion[DEF_AXIS_SERCOS_STAGE_Y];
	pMotion[1] = m_pMotion[DEF_AXIS_SERCOS_STAGE_T];
	//161024 Modify...
	//m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT] = new MSercosMultiAxis(m_pObjSystemInfo->GetCommonData(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT), pMotion, 2);
	//registerComponentInstance(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT, m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT]);
	m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT] = new MRSNMCMultiAxis(m_pObjSystemInfo->GetCommonData(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT), pMotion, 2, DEF_AXIS_SERCOS_MULTI_STAGE_YT);
	registerComponentInstance(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT, m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT]);

#ifdef DEF_USE_TRANSFER_CENTER
	pMotion[0] = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_1];
	pMotion[1] = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_2];
	//161024 Modify...
	//m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT] = new MSercosMultiAxis(m_pObjSystemInfo->GetCommonData(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT), pMotion, 2);
	//registerComponentInstance(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT, m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT]);
	m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER] = new MRSNMCMultiAxis(m_pObjSystemInfo->GetCommonData(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER), pMotion, 2, DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER);
	registerComponentInstance(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER, m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER]);
#endif

	pMotion[0] = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN];
	pMotion[1] = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN_2];
	//161024 Modify...
	//m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT] = new MSercosMultiAxis(m_pObjSystemInfo->GetCommonData(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT), pMotion, 2);
	//registerComponentInstance(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT, m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT]);
	m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANFSER_IN] = new MRSNMCMultiAxis(m_pObjSystemInfo->GetCommonData(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_TRANFSER_IN), pMotion, 2, DEF_AXIS_SERCOS_MULTI_TRANFSER_IN);
	registerComponentInstance(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_TRANFSER_IN, m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANFSER_IN]);

#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
	pMotion[0] = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT];
	pMotion[1] = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT_2];
	//161024 Modify...
	//m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT] = new MSercosMultiAxis(m_pObjSystemInfo->GetCommonData(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT), pMotion, 2);
	//registerComponentInstance(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_STAGE_YT, m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT]);
	m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANFSER_OUT] = new MRSNMCMultiAxis(m_pObjSystemInfo->GetCommonData(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_TRANFSER_OUT), pMotion, 2, DEF_AXIS_SERCOS_MULTI_TRANFSER_OUT);
	registerComponentInstance(DEF_MULTI_MOTION_OBJECT_BASE + DEF_AXIS_SERCOS_MULTI_TRANFSER_OUT, m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANFSER_OUT]);
#endif

	return ERR_SYSTEM_SUCCESS;
}

int	MPlatformOlbSystem::createLightingComponents()
{
	SLightingVal rgsLightingVal[DEF_MAX_LIGHTING] = 
	{
		//DEF_LIGHTING_1	: PreAlign Cam1,2 외부 조명
		{
			DEF_LIGHTING_CHANNEL1,
			OUT_ILLUMINATION1_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_2	: PreAlign Cam3,4 외부 조명
		{
			DEF_LIGHTING_CHANNEL2,
			OUT_ILLUMINATION2_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_3	: Inspection Cam1 동축 조명
		{
			DEF_LIGHTING_CHANNEL3,
			OUT_ILLUMINATION3_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_4	: Inspection Cam2 동축 조명
		{
			DEF_LIGHTING_CHANNEL4,
			OUT_ILLUMINATION4_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_5	: Inspection Cam1/2 외부 조명
		{
//			DEF_LIGHTING_CHANNEL1,
			DEF_LIGHTING_CHANNEL5,
			OUT_ILLUMINATION5_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_6	: Inspection Cam3 동축 조명
		{
//			DEF_LIGHTING_CHANNEL2,
			DEF_LIGHTING_CHANNEL6,
			OUT_ILLUMINATION6_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_7	: Inspection Cam4 동축 조명
		{
//			DEF_LIGHTING_CHANNEL3,
			DEF_LIGHTING_CHANNEL7,
			OUT_ILLUMINATION7_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_8	: Inspection Cam3/4 외부 조명
		{
///			DEF_LIGHTING_CHANNEL4,
			DEF_LIGHTING_CHANNEL8,
			OUT_ILLUMINATION8_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_9	: Inspection Cam5 동축 조명
		{
			DEF_LIGHTING_CHANNEL1,
			OUT_ILLUMINATION9_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_10	: Inspection Cam6 동축 조명
		{
			DEF_LIGHTING_CHANNEL2,
			OUT_ILLUMINATION10_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_11	: Inspection Cam5/6 외부 조명
		{
			DEF_LIGHTING_CHANNEL3,
			OUT_ILLUMINATION11_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_12	: Inspection Cam7 동축 조명
		{
			DEF_LIGHTING_CHANNEL4,
			OUT_ILLUMINATION12_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_13	: Inspection Cam8 동축 조명
		{
//			DEF_LIGHTING_CHANNEL1,
			DEF_LIGHTING_CHANNEL5,
			OUT_ILLUMINATION13_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_14	: Inspection Cam7,8 외부 조명
		{
//			DEF_LIGHTING_CHANNEL2,
			DEF_LIGHTING_CHANNEL6,
			OUT_ILLUMINATION14_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},

#ifdef DEF_SOURCE_SYSTEM
		//DEF_LIGHTING_15	: R Inspection Cam1 동축 조명
		{
//			DEF_LIGHTING_CHANNEL3,
			DEF_LIGHTING_CHANNEL7,
			OUT_ILLUMINATION15_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_16	: R Inspection Cam2 동축 조명
		{
//			DEF_LIGHTING_CHANNEL4,
			DEF_LIGHTING_CHANNEL8,
			OUT_ILLUMINATION16_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_17	: R Inspection Cam1/2 외부 조명
		{
			DEF_LIGHTING_CHANNEL1,
			OUT_ILLUMINATION17_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_18	: R Inspection Cam3 동축 조명
		{
			DEF_LIGHTING_CHANNEL2,
			OUT_ILLUMINATION18_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_19	: R Inspection Cam4 동축 조명
		{
			DEF_LIGHTING_CHANNEL3,
			OUT_ILLUMINATION19_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_20	: R Inspection Cam3/4 외부 조명
		{
			DEF_LIGHTING_CHANNEL4,
			OUT_ILLUMINATION20_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_21	: R Inspection Cam5 동축 조명
		{
			DEF_LIGHTING_CHANNEL1,
			OUT_ILLUMINATION21_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_22	: R Inspection Cam6 동축 조명
		{
			DEF_LIGHTING_CHANNEL2,
			OUT_ILLUMINATION22_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_23	: R Inspection Cam5/6 외부 조명
		{
			DEF_LIGHTING_CHANNEL3,
			OUT_ILLUMINATION23_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_24	: R Inspection Cam7 동축 조명
		{
			DEF_LIGHTING_CHANNEL4,
			OUT_ILLUMINATION24_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_25	: R Inspection Cam8 동축 조명
		{
			DEF_LIGHTING_CHANNEL1,
			OUT_ILLUMINATION25_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
		//DEF_LIGHTING_26	: R Inspection Cam7,8 외부 조명
		{
			DEF_LIGHTING_CHANNEL2,
			OUT_ILLUMINATION26_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},

		//DEF_LIGHTING_27	: R Inspection Cam7,8 외부 조명
		{
			DEF_LIGHTING_CHANNEL3,
			OUT_ILLUMINATION26_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},

		//DEF_LIGHTING_28	: R Inspection Cam7,8 외부 조명
		{
			DEF_LIGHTING_CHANNEL4,
			OUT_ILLUMINATION26_DIGITAL_INPUT_1,
			DEF_LIGHTING_USE_8_BIT,
			NULL,
			NULL,
			NULL,
			TRUE,
		},
#endif
	};

	IComThread* pComThread[DEF_MAX_LIGHTING] = 
	{
		m_pSerial[DEF_SERIAL_LIGHT1],m_pSerial[DEF_SERIAL_LIGHT1],m_pSerial[DEF_SERIAL_LIGHT1],m_pSerial[DEF_SERIAL_LIGHT1],
		m_pSerial[DEF_SERIAL_LIGHT1],m_pSerial[DEF_SERIAL_LIGHT1],m_pSerial[DEF_SERIAL_LIGHT1],m_pSerial[DEF_SERIAL_LIGHT1],
		m_pSerial[DEF_SERIAL_LIGHT2],m_pSerial[DEF_SERIAL_LIGHT2],m_pSerial[DEF_SERIAL_LIGHT2],m_pSerial[DEF_SERIAL_LIGHT2],
		m_pSerial[DEF_SERIAL_LIGHT2],m_pSerial[DEF_SERIAL_LIGHT2],m_pSerial[DEF_SERIAL_LIGHT2],m_pSerial[DEF_SERIAL_LIGHT2],
		m_pSerial[DEF_SERIAL_LIGHT3],m_pSerial[DEF_SERIAL_LIGHT3],m_pSerial[DEF_SERIAL_LIGHT3],m_pSerial[DEF_SERIAL_LIGHT3],
		m_pSerial[DEF_SERIAL_LIGHT4],m_pSerial[DEF_SERIAL_LIGHT4],m_pSerial[DEF_SERIAL_LIGHT4],m_pSerial[DEF_SERIAL_LIGHT4],
		m_pSerial[DEF_SERIAL_LIGHT5],m_pSerial[DEF_SERIAL_LIGHT5],m_pSerial[DEF_SERIAL_LIGHT5],m_pSerial[DEF_SERIAL_LIGHT5],
	};


	for (int i = 0; i < DEF_MAX_LIGHTING; i++)
	{
		//SJ_YYK 150817 Modify...
		//m_pLighting[i] = new MLighting(m_pObjSystemInfo->GetCommonData(DEF_LIGHTING_OBJECT_BASE + i), GetIOComponent(), rgsLightingVal[i]);
		m_pLighting[i] = new MLighting_ONI (pComThread[i], rgsLightingVal[i]);
		registerComponentInstance(DEF_LIGHTING_OBJECT_BASE + i, m_pLighting[i]);

		m_pLighting[i]->Initialize();
	}
	return ERR_SYSTEM_SUCCESS;
}

//100714.KKY.Comment.미사용
int MPlatformOlbSystem::createInverterComponents()
{
	/*
	SInverterVal rgsInverterVal[DEF_MAX_CONVEYOR] =
	{
		// 0. DEF_LOAD_CONVEYOR : Load Conveyor Inverter
		{
			IN_LOAD_INVERTER_READY,
			OUT_LOAD_INVERTER_STOP,
			IN_LOAD_INVERTER_SPEED_00,
			OUT_LOAD_INVERTER_SPEED_00
		},

		// 1. DEF_UNLOAD_CONVEYOR : Unload Conveyor Inverter
		{
			IN_UNLOAD_INVERTER_READY,
			OUT_UNLOAD_INVERTER_STOP,
			IN_UNLOAD_INVERTER_SPEED_00,
			OUT_UNLOAD_INVERTER_SPEED_00
		}
	};

	for (int i = 0; i < DEF_MAX_CONVEYOR; i++)
	{
		m_pInverter[i] = new MInverter(m_pObjSystemInfo->GetCommonData(DEF_INVERTER_OBJECT_BASE + i), GetIOComponent(), rgsInverterVal[i]);
		registerComponentInstance(DEF_INVERTER_OBJECT_BASE + i, m_pInverter[i]);
	}
	*/
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createOpPanelComponents()
{
	//---- IO Address Assign -----------------------------------------

	// Touch Panel Selection 
	SOpPanelIOAddr addrOpPanel;
//@	addrOpPanel.usiTouchSelectAddr				= OUT_TOUCH_MONITOR_CHANGE;
	addrOpPanel.usiTouchSelectAddr[DEF_OPPANEL_FRONT_PANEL_ID-1]			= OUT_TOUCH_MONITOR_CHANGE;
	addrOpPanel.usiTouchSelectAddr[DEF_OPPANEL_BACK_PANEL_ID-1]				= OUT_TOUCH_MONITOR_CHANGE_R;
	addrOpPanel.usiTouchSelectAddr[DEF_OPPANEL_FRONT_PANEL_ID_PRESS-1]		= OUT_TOUCH_MONITOR_CHANGE_PRESS;
	addrOpPanel.usiTouchSelectAddr[DEF_OPPANEL_BACK_PANEL_ID_PRESS-1]		= OUT_TOUCH_MONITOR_CHANGE_PRESS_R;
	
	// Front Panel IO Address
	addrOpPanel.FrontPanel.usiResetInputAddr	= IN_FRONT_RESET_SW;
	addrOpPanel.FrontPanel.usiResetOutputAddr	= OUT_FRONT_RESET_SW_LAMP;
	addrOpPanel.FrontPanel.usiRunInputAddr		= IN_FRONT_START_SW;
	addrOpPanel.FrontPanel.usiRunOutputAddr		= OUT_FRONT_START_SW_LAMP;
	addrOpPanel.FrontPanel.usiStopInputAddr		= IN_FRONT_STEP_STOP_SW;
	addrOpPanel.FrontPanel.usiStopOutputAddr	= OUT_FRONT_STEP_STOP_SW_LAMP;

	//@KKY.130906____________
	addrOpPanel.FrontPressPanel.usiResetInputAddr	= IN_FRONT_RESET_SW_PRESS;
	addrOpPanel.FrontPressPanel.usiResetOutputAddr	= OUT_FRONT_RESET_SW_LAMP;
	addrOpPanel.FrontPressPanel.usiRunInputAddr		= IN_FRONT_START_SW_PRESS;
	addrOpPanel.FrontPressPanel.usiRunOutputAddr	= OUT_FRONT_START_SW_LAMP;
	addrOpPanel.FrontPressPanel.usiStopInputAddr	= IN_FRONT_STEP_STOP_SW_PRESS;
	addrOpPanel.FrontPressPanel.usiStopOutputAddr	= OUT_FRONT_STEP_STOP_SW_LAMP;
	//@______________________

	addrOpPanel.FrontPanel.usiTPStopInputAddr	= IN_FRONT_JOG_E_STOP;
	addrOpPanel.FrontPanel.usiTPDetectedInputAddr=IN_FRONT_JOG_DETECT;
#ifdef DEF_MIRROR_
	addrOpPanel.FrontPanel.usiXpInputAddr		= IN_FRONT_JOG_L_SW;
	addrOpPanel.FrontPanel.usiXnInputAddr		= IN_FRONT_JOG_R_SW;
	addrOpPanel.FrontPanel.usiYpInputAddr		= IN_FRONT_JOG_F_SW;
	addrOpPanel.FrontPanel.usiYnInputAddr		= IN_FRONT_JOG_B_SW;
	addrOpPanel.FrontPanel.usiTpInputAddr		= IN_FRONT_JOG_CW_SW;
	addrOpPanel.FrontPanel.usiTnInputAddr		= IN_FRONT_JOG_CCW_SW;
	addrOpPanel.FrontPanel.usiZpInputAddr		= IN_FRONT_JOG_U_SW;
	addrOpPanel.FrontPanel.usiZnInputAddr		= IN_FRONT_JOG_D_SW;

	//@KKY.130906____________
	addrOpPanel.FrontPressPanel.usiXpInputAddr		= IN_FRONT_JOG_L_SW;
	addrOpPanel.FrontPressPanel.usiXnInputAddr		= IN_FRONT_JOG_R_SW;
	addrOpPanel.FrontPressPanel.usiYpInputAddr		= IN_FRONT_JOG_F_SW;
	addrOpPanel.FrontPressPanel.usiYnInputAddr		= IN_FRONT_JOG_B_SW;
	addrOpPanel.FrontPressPanel.usiTpInputAddr		= IN_FRONT_JOG_CW_SW;
	addrOpPanel.FrontPressPanel.usiTnInputAddr		= IN_FRONT_JOG_CCW_SW;
	addrOpPanel.FrontPressPanel.usiZpInputAddr		= IN_FRONT_JOG_U_SW;
	addrOpPanel.FrontPressPanel.usiZnInputAddr		= IN_FRONT_JOG_D_SW;
	//@______________________

#else
	addrOpPanel.FrontPanel.usiXpInputAddr		= IN_FRONT_JOG_R_SW;
	addrOpPanel.FrontPanel.usiXnInputAddr		= IN_FRONT_JOG_L_SW;
	addrOpPanel.FrontPanel.usiYpInputAddr		= IN_FRONT_JOG_F_SW;
	addrOpPanel.FrontPanel.usiYnInputAddr		= IN_FRONT_JOG_B_SW;
	addrOpPanel.FrontPanel.usiTpInputAddr		= IN_FRONT_JOG_CCW_SW;
	addrOpPanel.FrontPanel.usiTnInputAddr		= IN_FRONT_JOG_CW_SW;
	addrOpPanel.FrontPanel.usiZpInputAddr		= IN_FRONT_JOG_U_SW;
	addrOpPanel.FrontPanel.usiZnInputAddr		= IN_FRONT_JOG_D_SW;

	//@KKY.130906____________
	addrOpPanel.FrontPressPanel.usiXpInputAddr		= IN_FRONT_JOG_R_SW;
	addrOpPanel.FrontPressPanel.usiXnInputAddr		= IN_FRONT_JOG_L_SW;
	addrOpPanel.FrontPressPanel.usiYpInputAddr		= IN_FRONT_JOG_F_SW;
	addrOpPanel.FrontPressPanel.usiYnInputAddr		= IN_FRONT_JOG_B_SW;
	addrOpPanel.FrontPressPanel.usiTpInputAddr		= IN_FRONT_JOG_CCW_SW;
	addrOpPanel.FrontPressPanel.usiTnInputAddr		= IN_FRONT_JOG_CW_SW;
	addrOpPanel.FrontPressPanel.usiZpInputAddr		= IN_FRONT_JOG_U_SW;
	addrOpPanel.FrontPressPanel.usiZnInputAddr		= IN_FRONT_JOG_D_SW;
	//@______________________
#endif

	// Back Panel IO Address
	addrOpPanel.BackPanel.usiResetInputAddr		= IN_REAR_RESET_SW;
	addrOpPanel.BackPanel.usiResetOutputAddr	= OUT_REAR_RESET_SW_LAMP;
	addrOpPanel.BackPanel.usiRunInputAddr		= IN_REAR_START_SW;
	addrOpPanel.BackPanel.usiRunOutputAddr		= OUT_REAR_START_SW_LAMP;
	addrOpPanel.BackPanel.usiStopInputAddr		= IN_REAR_STEP_STOP_SW;
	addrOpPanel.BackPanel.usiStopOutputAddr		= OUT_REAR_STEP_STOP_SW_LAMP;

	//@KKY.130906____________
	addrOpPanel.BackPressPanel.usiResetInputAddr	= IN_REAR_RESET_SW_PRESS;
	addrOpPanel.BackPressPanel.usiResetOutputAddr	= OUT_REAR_RESET_SW_LAMP;
	addrOpPanel.BackPressPanel.usiRunInputAddr		= IN_REAR_START_SW_PRESS;
	addrOpPanel.BackPressPanel.usiRunOutputAddr		= OUT_REAR_START_SW_LAMP;
	addrOpPanel.BackPressPanel.usiStopInputAddr		= IN_REAR_STEP_STOP_SW_PRESS;
	addrOpPanel.BackPressPanel.usiStopOutputAddr	= OUT_REAR_STEP_STOP_SW_LAMP;
	//@______________________

	addrOpPanel.BackPanel.usiTPStopInputAddr	= IN_REAR_JOG_E_STOP;
	addrOpPanel.BackPanel.usiTPDetectedInputAddr= IN_REAR_JOG_DETECT;
#ifdef DEF_MIRROR_
	addrOpPanel.BackPanel.usiXpInputAddr		= IN_REAR_JOG_R_SW;
	addrOpPanel.BackPanel.usiXnInputAddr		= IN_REAR_JOG_L_SW;
	addrOpPanel.BackPanel.usiYpInputAddr		= IN_REAR_JOG_B_SW;
	addrOpPanel.BackPanel.usiYnInputAddr		= IN_REAR_JOG_F_SW;
	addrOpPanel.BackPanel.usiTpInputAddr		= IN_REAR_JOG_CW_SW;
	addrOpPanel.BackPanel.usiTnInputAddr		= IN_REAR_JOG_CCW_SW;
	addrOpPanel.BackPanel.usiZpInputAddr		= IN_REAR_JOG_U_SW;
	addrOpPanel.BackPanel.usiZnInputAddr		= IN_REAR_JOG_D_SW;

	//@KKY.130906____________
	addrOpPanel.BackPressPanel.usiXpInputAddr		= IN_REAR_JOG_R_SW;
	addrOpPanel.BackPressPanel.usiXnInputAddr		= IN_REAR_JOG_L_SW;
	addrOpPanel.BackPressPanel.usiYpInputAddr		= IN_REAR_JOG_B_SW;
	addrOpPanel.BackPressPanel.usiYnInputAddr		= IN_REAR_JOG_F_SW;
	addrOpPanel.BackPressPanel.usiTpInputAddr		= IN_REAR_JOG_CW_SW;
	addrOpPanel.BackPressPanel.usiTnInputAddr		= IN_REAR_JOG_CCW_SW;
	addrOpPanel.BackPressPanel.usiZpInputAddr		= IN_REAR_JOG_U_SW;
	addrOpPanel.BackPressPanel.usiZnInputAddr		= IN_REAR_JOG_D_SW;
	//@______________________

#else
	addrOpPanel.BackPanel.usiXpInputAddr		= IN_REAR_JOG_L_SW;
	addrOpPanel.BackPanel.usiXnInputAddr		= IN_REAR_JOG_R_SW;
	addrOpPanel.BackPanel.usiYpInputAddr		= IN_REAR_JOG_B_SW;
	addrOpPanel.BackPanel.usiYnInputAddr		= IN_REAR_JOG_F_SW;
	addrOpPanel.BackPanel.usiTpInputAddr		= IN_REAR_JOG_CCW_SW;
	addrOpPanel.BackPanel.usiTnInputAddr		= IN_REAR_JOG_CW_SW;
	addrOpPanel.BackPanel.usiZpInputAddr		= IN_REAR_JOG_U_SW;
	addrOpPanel.BackPanel.usiZnInputAddr		= IN_REAR_JOG_D_SW;

	//@KKY.130906____________
	addrOpPanel.BackPressPanel.usiXpInputAddr		= IN_REAR_JOG_L_SW;
	addrOpPanel.BackPressPanel.usiXnInputAddr		= IN_REAR_JOG_R_SW;
	addrOpPanel.BackPressPanel.usiYpInputAddr		= IN_REAR_JOG_B_SW;
	addrOpPanel.BackPressPanel.usiYnInputAddr		= IN_REAR_JOG_F_SW;
	addrOpPanel.BackPressPanel.usiTpInputAddr		= IN_REAR_JOG_CCW_SW;
	addrOpPanel.BackPressPanel.usiTnInputAddr		= IN_REAR_JOG_CW_SW;
	addrOpPanel.BackPressPanel.usiZpInputAddr		= IN_REAR_JOG_U_SW;
	addrOpPanel.BackPressPanel.usiZnInputAddr		= IN_REAR_JOG_D_SW;
	//@______________________
#endif

	// Tower Lamp IO Address
	addrOpPanel.TowerLamp.usiBuzzerAddr[0]		= OUT_BUZZER_1;
	addrOpPanel.TowerLamp.usiBuzzerAddr[1]		= OUT_BUZZER_2;
	addrOpPanel.TowerLamp.usiBuzzerAddr[2]		= OUT_BUZZER_3;
	addrOpPanel.TowerLamp.usiBuzzerAddr[3]		= OUT_BUZZER_4;
	addrOpPanel.TowerLamp.usiRedLampAddr		= OUT_TOWER_LAMP_RED;
	addrOpPanel.TowerLamp.usiGreenLampAddr		= OUT_TOWER_LAMP_GREEN;
	addrOpPanel.TowerLamp.usiYellowLampAddr		= OUT_TOWER_LAMP_YELLOW;

	// E-Stop Input
	addrOpPanel.usiEStopInputAddr[0]			= IN_FRONT_JOG_E_STOP; //170701 JSH
	addrOpPanel.usiEStopInputAddr[1]			= IN_REAR_JOG_E_STOP; //170701 JSH
	addrOpPanel.usiEStopInputAddr[2]			= NULL;
	//____________________________________________________________________________________________

	// Safe Door : -> TODO (Door 확인 후 수정 필요)
	for (int i = 0; i < DEF_OPPANEL_MAX_DOOR_GROUP; i++)
	{
		for (int j = 0; j < DEF_OPPANEL_MAX_DOOR_SENSOR; j++)
		{
			addrOpPanel.usiSafeDoorAddr[i][j]	= NULL;
			addrOpPanel.rgbSafeDoorFlag[i][j]	= FALSE;
			addrOpPanel.rgbMaterialChangeFlag[i][j] = FALSE;
		}
	}

	addrOpPanel.usiSafeDoorAddr[0][0]			= IN_FRONT_DOOR1;
	addrOpPanel.rgbSafeDoorFlag[0][0]			= TRUE;
	addrOpPanel.usiSafeDoorAddr[0][1]			= IN_FRONT_DOOR2;
	addrOpPanel.rgbSafeDoorFlag[0][1]			= TRUE;
	addrOpPanel.usiSafeDoorAddr[0][2]			= IN_FRONT_DOOR3;
	addrOpPanel.rgbSafeDoorFlag[0][2]			= TRUE;
	addrOpPanel.usiSafeDoorAddr[1][0]			= IN_REAR_DOOR1;
	addrOpPanel.rgbSafeDoorFlag[1][0]			= TRUE;
	addrOpPanel.usiSafeDoorAddr[1][1]			= IN_REAR_DOOR2;	
	addrOpPanel.rgbSafeDoorFlag[1][1]			= TRUE;
	addrOpPanel.usiSafeDoorAddr[1][2]			= IN_REAR_DOOR3;
	addrOpPanel.rgbSafeDoorFlag[1][2]			= TRUE;	
	addrOpPanel.usiSafeDoorAddr[1][3]			= IN_REAR_DOOR4;
	addrOpPanel.rgbSafeDoorFlag[1][3]			= TRUE;	
	addrOpPanel.usiSafeDoorAddr[1][4]			= IN_REAR_DOOR5;
	addrOpPanel.rgbSafeDoorFlag[1][4]			= TRUE;	


	//____________________________________________________________________________________________

	// Main Air
	addrOpPanel.usiMainAirAddr[0]				= IN_MAIN_AIR1;
	addrOpPanel.usiMainAirAddr[1]				= NULL;
//	addrOpPanel.usiMainAirAddr[1]				= IN_MAIN_AIR2;
	//____________________________________________________________________________________________

	// Main Vacuum
	addrOpPanel.usiMainVacuumAddr[0]			= IN_MAIN_VACUUM1;
	addrOpPanel.usiMainVacuumAddr[1]			= NULL;
	addrOpPanel.usiMainVacuumAddr[2]			= NULL;
	addrOpPanel.usiMainVacuumAddr[3]			= NULL;
//	addrOpPanel.usiMainVacuumAddr[1]			= IN_MAIN_VACUUM2;
//	addrOpPanel.usiMainVacuumAddr[2]			= IN_MAIN_VACUUM3;
//	addrOpPanel.usiMainVacuumAddr[3]			= IN_MAIN_VACUUM4;
	//____________________________________________________________________________________________

	// Jog 설정
	SJogTable sJogTable;
	sJogTable.m_iListNo = DEF_MAX_JOG_LIST;
	for (int i = 0; i < DEF_OPPANEL_MAX_JOG_LIST; i++)
	{
		sJogTable.m_Motion[i].m_XKey.m_plnkJog		= NULL;
		sJogTable.m_Motion[i].m_XKey.m_plnkMultiJog = NULL;
		sJogTable.m_Motion[i].m_XKey.m_iAxisIndex	= -1;
		sJogTable.m_Motion[i].m_XKey.m_iMotionType	= 0;
		sJogTable.m_Motion[i].m_XKey.m_bDirMask		= TRUE;
		sJogTable.m_Motion[i].m_YKey.m_plnkJog		= NULL;
		sJogTable.m_Motion[i].m_YKey.m_plnkMultiJog = NULL;
		sJogTable.m_Motion[i].m_YKey.m_iAxisIndex	= -1;
		sJogTable.m_Motion[i].m_YKey.m_iMotionType	= 0;
		sJogTable.m_Motion[i].m_YKey.m_bDirMask		= TRUE;
		sJogTable.m_Motion[i].m_TKey.m_plnkJog		= NULL;
		sJogTable.m_Motion[i].m_TKey.m_plnkMultiJog = NULL;
		sJogTable.m_Motion[i].m_TKey.m_iAxisIndex	= -1;
		sJogTable.m_Motion[i].m_TKey.m_iMotionType	= 0;
		sJogTable.m_Motion[i].m_TKey.m_bDirMask		= TRUE;
		sJogTable.m_Motion[i].m_ZKey.m_plnkJog		= NULL;
		sJogTable.m_Motion[i].m_ZKey.m_plnkMultiJog = NULL;
		sJogTable.m_Motion[i].m_ZKey.m_iAxisIndex	= -1;
		sJogTable.m_Motion[i].m_ZKey.m_iMotionType	= 0;
		sJogTable.m_Motion[i].m_ZKey.m_bDirMask		= TRUE;
	}

	// Jog Key로 움직일 Motion 축 0: 단축, 1: Multi 축 //
	//int					m_iAxisIndex;
	// Motion 종류 0:MMC, 1:Sercos, 2:ACS //
	//int					m_iMotionType;
	// Jog 방향 TRUE: 정방향, FALSE : 역방향 //
	//BOOL				m_bDirMask;

	//Pre Aligner XT 개별 Jog 설정
	sJogTable.m_Motion[DEF_JOG_STAGE_YT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_STAGE_Y];
	sJogTable.m_Motion[DEF_JOG_STAGE_YT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_STAGE_YT].m_YKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_STAGE_YT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_STAGE_T];
	sJogTable.m_Motion[DEF_JOG_STAGE_YT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_STAGE_YT].m_TKey.m_iMotionType = 1;

	// PanelAligner XYT Jog 설정
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_XKey.m_plnkJog		= m_pMotion[DEF_AXIS_SERCOS_STAGE_X];
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_XKey.m_iAxisIndex	= 0;
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_XKey.m_iMotionType	= 1;

	//sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_YKey.m_plnkMultiJog = m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT];
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_YKey.m_plnkMultiJog	= m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT];		//161024 Modify...
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_YKey.m_iAxisIndex	= 1;
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_YKey.m_iMotionType	= 1;

	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_TKey.m_plnkJog		= m_pMotion[DEF_AXIS_SERCOS_STAGE_T];
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_TKey.m_iAxisIndex	= 0;
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_TKey.m_iMotionType	= 1;

	// PanelAligner XY Jog 설정
	sJogTable.m_Motion[DEF_JOG_STAGE_XY].m_XKey.m_plnkJog		= m_pMotion[DEF_AXIS_SERCOS_STAGE_X];
	sJogTable.m_Motion[DEF_JOG_STAGE_XY].m_XKey.m_iAxisIndex	= 0;
	sJogTable.m_Motion[DEF_JOG_STAGE_XY].m_XKey.m_iMotionType	= 1;

	//sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_YKey.m_plnkMultiJog = m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT];
	sJogTable.m_Motion[DEF_JOG_STAGE_XYT].m_YKey.m_plnkMultiJog	= m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT];		//161024 Modify...
	sJogTable.m_Motion[DEF_JOG_STAGE_XY].m_YKey.m_iAxisIndex	= 1;
	sJogTable.m_Motion[DEF_JOG_STAGE_XY].m_YKey.m_iMotionType	= 1;

	// PanelAligner Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_STAGE_Z].m_ZKey.m_plnkJog		= m_pMotion[DEF_AXIS_SERCOS_STAGE_Z];
	sJogTable.m_Motion[DEF_JOG_STAGE_Z].m_ZKey.m_iAxisIndex		= 0;
	sJogTable.m_Motion[DEF_JOG_STAGE_Z].m_ZKey.m_iMotionType	= 1;

	// PanelTransfer In Jog 설정
	//sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_IN_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN];
	//170929 KDW Start Gear
	//sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_IN_X].m_XKey.m_plnkMultiJog = m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANFSER_IN]; //SJ_YYK 161104 Modify..
	//sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_IN_X].m_XKey.m_iAxisIndex	= 1;
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_IN_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN]; //SJ_YYK 161104 Modify..
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_IN_X].m_XKey.m_iAxisIndex	= 0;
	//170929 KDW End

	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_IN_X].m_XKey.m_iMotionType	= 1;

	// PanelTransfer Out Jog 설정
#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
	//sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT];
	//170928 JSh.s Gear
	//sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_plnkMultiJog = m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANSFER_OUT];			//161014 Modify...
	//sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_iAxisIndex	= 1;
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT];			//161014 Modify...
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_iAxisIndex	= 0;
	//170928 JSh.e
	
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_iMotionType	= 1;
#else
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT];
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_iAxisIndex	= 0;
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_OUT_X].m_XKey.m_iMotionType	= 1;
#endif

	// Model Change Y1 Jog 설정
	sJogTable.m_Motion[DEF_JOG_MODEL_CHANGE_Y1].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MODEL_CHANGE_1];
	sJogTable.m_Motion[DEF_JOG_MODEL_CHANGE_Y1].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_MODEL_CHANGE_Y1].m_YKey.m_iMotionType = 1;

	// Model Change Y2 Jog 설정
	sJogTable.m_Motion[DEF_JOG_MODEL_CHANGE_Y2].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MODEL_CHANGE_2];
	sJogTable.m_Motion[DEF_JOG_MODEL_CHANGE_Y2].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_MODEL_CHANGE_Y2].m_YKey.m_iMotionType = 1;

	// BadTab Detector Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_BADTAB_DETECTOR_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_BADTAB_DETECTOR_X];
	sJogTable.m_Motion[DEF_JOG_F_BADTAB_DETECTOR_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_BADTAB_DETECTOR_X].m_XKey.m_iMotionType = 1;

	// Feeder Sprocket1 T Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TABFEEDER_SPROCKET1_T].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T];
	sJogTable.m_Motion[DEF_JOG_F_TABFEEDER_SPROCKET1_T].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABFEEDER_SPROCKET1_T].m_TKey.m_iMotionType = 1;


	sJogTable.m_Motion[DEF_JOG_F_TABFEEDER_SPROCKET2_T].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T];
	sJogTable.m_Motion[DEF_JOG_F_TABFEEDER_SPROCKET2_T].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABFEEDER_SPROCKET2_T].m_TKey.m_iMotionType = 1;

	
	// Tab Press Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TAB_PRESS_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TAB_PRESS_Z];
	sJogTable.m_Motion[DEF_JOG_F_TAB_PRESS_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TAB_PRESS_Z].m_ZKey.m_iMotionType = 1;

	// Rear BadTab Detector Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_BADTAB_DETECTOR_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X];
	sJogTable.m_Motion[DEF_JOG_R_BADTAB_DETECTOR_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_BADTAB_DETECTOR_X].m_XKey.m_iMotionType = 1;

	// Rear Feeder Sprocket1 T Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABFEEDER_SPROCKET1_T].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T];
	sJogTable.m_Motion[DEF_JOG_R_TABFEEDER_SPROCKET1_T].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABFEEDER_SPROCKET1_T].m_TKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABFEEDER_SPROCKET2_T].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T];
	sJogTable.m_Motion[DEF_JOG_R_TABFEEDER_SPROCKET2_T].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABFEEDER_SPROCKET2_T].m_TKey.m_iMotionType = 1;

	
	// Rear Tab Press Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TAB_PRESS_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_TAB_PRESS_Z];
	sJogTable.m_Motion[DEF_JOG_R_TAB_PRESS_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TAB_PRESS_Z].m_ZKey.m_iMotionType = 1;

	// F Mounter1 XZ Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER1_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOUNTER1_X];
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER1_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER1_X].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER1_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOUNTER1_Z];
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER1_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER1_Z].m_ZKey.m_iMotionType = 1;
	
	// F Mounter2 XZ Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER2_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOUNTER2_X];
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER2_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER2_X].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER2_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOUNTER2_Z];
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER2_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER2_Z].m_ZKey.m_iMotionType = 1;
	
#ifdef DEF_SOURCE_SYSTEM
	// F Mounter3 XZ Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER3_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOUNTER3_X];
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER3_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER3_X].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER3_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOUNTER3_Z];
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER3_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER3_Z].m_ZKey.m_iMotionType = 1;
	
	// F Mounter4 XZ Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER4_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOUNTER4_X];
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER4_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER4_X].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER4_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOUNTER4_Z];
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER4_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABMOUNTER4_Z].m_ZKey.m_iMotionType = 1;
#endif

	// R Mounter1 XZ Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER1_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER1_X];
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER1_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER1_X].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER1_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER1_Z];
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER1_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER1_Z].m_ZKey.m_iMotionType = 1;
	
	// R Mounter2 XZ Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER2_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER2_X];
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER2_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER2_X].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER2_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER2_Z];
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER2_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER2_Z].m_ZKey.m_iMotionType = 1;
	
#ifdef DEF_SOURCE_SYSTEM
	// R Mounter3 XZ Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER3_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER3_X];
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER3_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER3_X].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER3_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER3_Z];
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER3_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER3_Z].m_ZKey.m_iMotionType = 1;
	
	// R Mounter4 XZ Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER4_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER4_X];
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER4_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER4_X].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER4_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER4_Z];
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER4_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABMOUNTER4_Z].m_ZKey.m_iMotionType = 1;
#endif

	// Inspection Camera1 Y Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA1_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X];
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA1_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA1_X].m_XKey.m_iMotionType = 1;

	// Inspection Camera2 Y Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA2_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X];
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA2_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA2_X].m_XKey.m_iMotionType = 1;

#ifdef DEF_SOURCE_SYSTEM
	// Inspection Camera3 Y Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA3_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_INSPECTION_CAMERA3_X];
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA3_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA3_X].m_XKey.m_iMotionType = 1;

	// Inspection Camera4 Y Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA4_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X];
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA4_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA4_X].m_XKey.m_iMotionType = 1;
#endif

	// Backup1 Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_BACKUP1_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_BACKUP1_Z];
	sJogTable.m_Motion[DEF_JOG_F_BACKUP1_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_BACKUP1_Z].m_ZKey.m_iMotionType = 1;

	// Backup2 Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_BACKUP2_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_BACKUP2_Z];
	sJogTable.m_Motion[DEF_JOG_F_BACKUP2_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_BACKUP2_Z].m_ZKey.m_iMotionType = 1;

#ifdef DEF_SOURCE_SYSTEM
	// Backup3 Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_BACKUP3_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_BACKUP3_Z];
	sJogTable.m_Motion[DEF_JOG_F_BACKUP3_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_BACKUP3_Z].m_ZKey.m_iMotionType = 1;

	// Backup4 Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_BACKUP4_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_BACKUP4_Z];
	sJogTable.m_Motion[DEF_JOG_F_BACKUP4_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_BACKUP4_Z].m_ZKey.m_iMotionType = 1;
#endif

	// Inspection Camera1 Y Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA1_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X];
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA1_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA1_X].m_XKey.m_iMotionType = 1;

	// Inspection Camera2 Y Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA2_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X];
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA2_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA2_X].m_XKey.m_iMotionType = 1;

#ifdef DEF_SOURCE_SYSTEM
	// Inspection Camera3 Y Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA3_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_INSPECTION_CAMERA3_X];
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA3_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA3_X].m_XKey.m_iMotionType = 1;

	// Inspection Camera4 Y Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA4_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X];
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA4_X].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA4_X].m_XKey.m_iMotionType = 1;
#endif

	// Backup1 Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_BACKUP1_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_BACKUP1_Z];
	sJogTable.m_Motion[DEF_JOG_R_BACKUP1_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_BACKUP1_Z].m_ZKey.m_iMotionType = 1;

	// Backup2 Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_BACKUP2_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_BACKUP2_Z];
	sJogTable.m_Motion[DEF_JOG_R_BACKUP2_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_BACKUP2_Z].m_ZKey.m_iMotionType = 1;

#ifdef DEF_SOURCE_SYSTEM
	// Backup3 Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_BACKUP3_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_BACKUP3_Z];
	sJogTable.m_Motion[DEF_JOG_R_BACKUP3_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_BACKUP3_Z].m_ZKey.m_iMotionType = 1;

	// Backup4 Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_BACKUP4_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_BACKUP4_Z];
	sJogTable.m_Motion[DEF_JOG_R_BACKUP4_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_BACKUP4_Z].m_ZKey.m_iMotionType = 1;
#endif

	// TabIC Carrier1 XYT Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER1_X];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER1_Y];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_YKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER1_T];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER1_XYT].m_TKey.m_iMotionType = 1;
	
	// TabIC Carrier2 XYT Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER2_X];  //SJ_YYK 150109 Modify..
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER2_Y];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_YKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER2_T];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER2_XYT].m_TKey.m_iMotionType = 1;
	
#ifdef DEF_SOURCE_SYSTEM
	// TabIC Carrier3 XYT Jog 설정
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER3_X];  //SJ_YYK 150109 Modify..
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER3_Y];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_YKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER3_T];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER3_XYT].m_TKey.m_iMotionType = 1;
	
	// TabIC Carrier4 XYT Jog 설정..
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER4_X];  //SJ_YYK 150109 Modify..
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER4_Y];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_YKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER4_T];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER4_XYT].m_TKey.m_iMotionType = 1;
#endif
	
	// Rear TabIC Carrier1 XYT Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_YKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER1_XYT].m_TKey.m_iMotionType = 1;
	
	// Rear TabIC Carrier2 XYT Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X];  //SJ_YYK 150109 Modify..
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_YKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER2_XYT].m_TKey.m_iMotionType = 1;
#ifdef DEF_SOURCE_SYSTEM
	// Rear TabIC Carrier3 XYT Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X];  //SJ_YYK 150109 Modify..
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_YKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER3_XYT].m_TKey.m_iMotionType = 1;
	
	// Rear TabIC Carrier4 XYT Jog 설정
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X];  //SJ_YYK 150109 Modify..
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_YKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER4_XYT].m_TKey.m_iMotionType = 1;


#endif

	sJogTable.m_Motion[DEF_JOG_F_FEEDER_REEL_T].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_FEEDER_REEL_T];
	sJogTable.m_Motion[DEF_JOG_F_FEEDER_REEL_T].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_FEEDER_REEL_T].m_TKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_FEEDER_REEL_T].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_FEEDER_REEL_T];
	sJogTable.m_Motion[DEF_JOG_R_FEEDER_REEL_T].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_FEEDER_REEL_T].m_TKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_MOLDING_UPDOWN_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z];
	sJogTable.m_Motion[DEF_JOG_F_MOLDING_UPDOWN_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_MOLDING_UPDOWN_Z].m_ZKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_MOLDING_UPDOWN_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z];
	sJogTable.m_Motion[DEF_JOG_R_MOLDING_UPDOWN_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_MOLDING_UPDOWN_Z].m_ZKey.m_iMotionType = 1;

	/** Gantry Y Jog 설정 */
	sJogTable.m_Motion[DEF_JOG_GANTRY_Y].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_ACS_GANTRY_Y];
	sJogTable.m_Motion[DEF_JOG_GANTRY_Y].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_GANTRY_Y].m_YKey.m_iMotionType = 2;

	/** Gantry Y Jog 설정 */
	sJogTable.m_Motion[DEF_JOG_R_GANTRY_Y].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_ACS_R_GANTRY_Y];
	sJogTable.m_Motion[DEF_JOG_R_GANTRY_Y].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_GANTRY_Y].m_YKey.m_iMotionType = 2;

#ifdef DEF_SOURCE_SYSTEM
	
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA1_EXPAND].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_INSPECT_EXPAND1];
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA1_EXPAND].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA1_EXPAND].m_XKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA2_EXPAND].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_INSPECT_EXPAND2];
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA2_EXPAND].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA2_EXPAND].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA3_EXPAND].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_INSPECT_EXPAND3];
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA3_EXPAND].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA3_EXPAND].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA4_EXPAND].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_INSPECT_EXPAND4];
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA4_EXPAND].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_INSPECTION_CAMERA4_EXPAND].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER_CAMERA_EXPAND].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABCAMERA_EXPAND];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER_CAMERA_EXPAND].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER_CAMERA_EXPAND].m_YKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA1_EXPAND].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_INSPECT_EXPAND1];
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA1_EXPAND].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA1_EXPAND].m_XKey.m_iMotionType = 1;
	
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA2_EXPAND].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_INSPECT_EXPAND2];
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA2_EXPAND].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA2_EXPAND].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA3_EXPAND].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_INSPECT_EXPAND3];
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA3_EXPAND].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA3_EXPAND].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA4_EXPAND].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_INSPECT_EXPAND4];
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA4_EXPAND].m_XKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_INSPECTION_CAMERA4_EXPAND].m_XKey.m_iMotionType = 1;

	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER_CAMERA_EXPAND].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER_CAMERA_EXPAND].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER_CAMERA_EXPAND].m_YKey.m_iMotionType = 1;


	//SJ_YYK 150817 Add..
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER_CAMERA_EXPAND2].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TABCAMERA_EXPAND2];
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER_CAMERA_EXPAND2].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_F_TABCARRIER_CAMERA_EXPAND2].m_YKey.m_iMotionType = 1;


	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER_CAMERA_EXPAND2].m_YKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2];
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER_CAMERA_EXPAND2].m_YKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_R_TABCARRIER_CAMERA_EXPAND2].m_YKey.m_iMotionType = 1;
#ifdef DEF_USE_TRANSFER_CENTER
	//SJ_YYK 161104 Add..
	//170929 KDW Start Gear
	//sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_CENTER_X].m_XKey.m_plnkMultiJog = m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER];
	//sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_CENTER_X].m_XKey.m_iAxisIndex	= 1;
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_CENTER_X].m_XKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_1];
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_CENTER_X].m_XKey.m_iAxisIndex	= 0;
	//170929 KDW End
	sJogTable.m_Motion[DEF_JOG_PANEL_TRANSFER_CENTER_X].m_XKey.m_iMotionType	= 1;
#endif
	// Handler XT Jog 설정
#		ifdef DEF_USE_TURN_HANDLER
	sJogTable.m_Motion[DEF_JOG_HANDLER_XT].m_TKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_HANDLER_T];
	sJogTable.m_Motion[DEF_JOG_HANDLER_XT].m_TKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_HANDLER_XT].m_TKey.m_iMotionType = 1;

	// Handler Z Jog 설정
	sJogTable.m_Motion[DEF_JOG_HANDLER_Z].m_ZKey.m_plnkJog = m_pMotion[DEF_AXIS_SERCOS_HANDLER_Z];
	sJogTable.m_Motion[DEF_JOG_HANDLER_Z].m_ZKey.m_iAxisIndex = 0;
	sJogTable.m_Motion[DEF_JOG_HANDLER_Z].m_ZKey.m_iMotionType = 1;
	//_______________________________*/
#		endif
#endif

	/** Component 생성 및 등록 */
	m_pOpPanel = new MOpPanel(m_pObjSystemInfo->GetCommonData(DEF_OPPANEL_OBJECT_BASE), m_pIO, addrOpPanel, sJogTable);
	registerComponentInstance(DEF_OPPANEL_OBJECT_BASE, m_pOpPanel);

	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createPanelAlignerComponents()
{
	SPanelAlignerRefCompList	sRefCompList;
	SPanelAlignerData			sPanelAlignerData;

	//Component List-----------------------------------------------------------

	sRefCompList.m_plnkIO			= GetIOComponent();
	sRefCompList.m_plnkVac1			= m_pVacuum[DEF_VAC_PANEL_ALIGNER1];
	sRefCompList.m_plnkVac2			= m_pVacuum[DEF_VAC_PANEL_ALIGNER2];
	sRefCompList.m_plnkVac3			= m_pVacuum[DEF_VAC_PANEL_ALIGNER3];

	sRefCompList.m_plnkX			= m_pMotion[DEF_AXIS_SERCOS_STAGE_X];
	sRefCompList.m_plnkY			= m_pMotion[DEF_AXIS_SERCOS_STAGE_Y];
	sRefCompList.m_plnkT			= m_pMotion[DEF_AXIS_SERCOS_STAGE_T];
	sRefCompList.m_plnkZ			= m_pMotion[DEF_AXIS_SERCOS_STAGE_Z];
	//161024 Modify... 
	//sRefCompList.m_plnkMultiYT		= m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT];
	sRefCompList.m_plnkMultiYT		= m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_STAGE_YT];

	sPanelAlignerData.m_sXYTZTeachingInfo		= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_STAGE_XYT);
	sPanelAlignerData.m_sZTeachingInfo			= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_STAGE_Z);	

	sPanelAlignerData.m_strPosFileName			= DEF_SYSTEM_POSITION_FILE_NAME;
	sPanelAlignerData.m_strOffsetPosFileName	= DEF_SYSTEM_OFFSET_FILE_NAME;
	sPanelAlignerData.m_plnkSystemData			= GetSystemData();
	sPanelAlignerData.m_plnkPanelData			= GetPanelData();
	sPanelAlignerData.m_plnkPreBonderData		= GetPreBonderData();
	sPanelAlignerData.m_plnkTabData				= GetTabData();

	sPanelAlignerData.m_usIPanelDetect			= IN_STAGE_CELL_DETECT;

	sPanelAlignerData.m_usOAddrAirBlow1			= OUT_PLATE_AIR_LEVITATION1_ON;
	sPanelAlignerData.m_usOAddrAirBlow2			= OUT_PLATE_AIR_LEVITATION2_ON;

	sPanelAlignerData.m_usIAddrAirBlow1			= IN_PLATE_AIR_LEVITATION1_ON;
	sPanelAlignerData.m_usIAddrAirBlow2			= IN_PLATE_AIR_LEVITATION2_ON;

	sPanelAlignerData.m_usOAddrAirVacuum1		= OUT_PLATE_AIR_VACUUM1_ON;
	sPanelAlignerData.m_usOAddrAirVacuum2		= OUT_PLATE_AIR_VACUUM2_ON;
	sPanelAlignerData.m_usOAddrAirVacuum3		= OUT_PLATE_AIR_VACUUM3_ON;
	sPanelAlignerData.m_usOAddrAirVacuum4		= OUT_PLATE_AIR_VACUUM4_ON;

	sPanelAlignerData.m_usIAddrAirVacuum1		= IN_PLATE_AIR_VACUUM1_ON;
	sPanelAlignerData.m_usIAddrAirVacuum2		= IN_PLATE_AIR_VACUUM2_ON;
	sPanelAlignerData.m_usIAddrAirVacuum3		= IN_PLATE_AIR_VACUUM3_ON;
	sPanelAlignerData.m_usIAddrAirVacuum4		= IN_PLATE_AIR_VACUUM4_ON;

	#ifdef DEF_GATE_SYSTEM
	sPanelAlignerData.m_usIPanelDetectLoading	= IN_PANEL_DETECT_LOADING;
	#endif
	sPanelAlignerData.m_usIPanelDetectWorking	= IN_PANEL_DETECT_WORKING;

	sPanelAlignerData.m_usIIonizerState         = IN_IONIZER_STATE_ONOFF;
	
	//170724_KDH Spot Cleaner 추가
	sPanelAlignerData.m_usOAddrSpotCleanerOn    = OUT_SPOT_CLEANER_BLOW_ON;
	sPanelAlignerData.m_usIIonizerState         = IN_SPOT_CLEANER_BLOW_ON;
	//________________________________

	
	
	m_pPanelAligner = new MPanelAligner(m_pObjSystemInfo->GetCommonData(DEF_PANEL_ALIGNER_OBJECT_BASE), sRefCompList, sPanelAlignerData);
	registerComponentInstance(DEF_PANEL_ALIGNER_OBJECT_BASE, m_pPanelAligner);

	return ERR_SYSTEM_SUCCESS;
}
#ifdef DEF_USE_TRANSFER_CENTER
int MPlatformOlbSystem::createPanelTransferCenterComponents()
{
	
#ifdef DEF_SOURCE_SYSTEM
	SPanelTransferRefCompList	sRefCompList;
	SPanelTransferData			sPanelTransferData;

	sRefCompList.m_plnkIO					= GetIOComponent();	
	sRefCompList.m_plnkVac					= m_pVacuum[DEF_VAC_PANEL_TRANSFER_CENTER];
	sRefCompList.m_plnkVac2					= m_pVacuum[DEF_VAC_PANEL_TRANSFER_CENTER2];
	sRefCompList.m_plnkUDCyl				= m_pCylinder[DEF_CYL_PANEL_TRANSFER_CENTER_UD];

	sRefCompList.m_plnkX1					= m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_1];
	sRefCompList.m_plnkX2					= m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_2];
	sRefCompList.m_plnkMultiX				= m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANSFER_CENTER];

	sPanelTransferData.m_sXTeachingInfo			= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_PANEL_TRANSFER_CENTER_X);
	sPanelTransferData.m_strPosFileName			= DEF_SYSTEM_POSITION_FILE_NAME;
	sPanelTransferData.m_strOffsetPosFileName	= DEF_SYSTEM_OFFSET_FILE_NAME;
	sPanelTransferData.m_plnkSystemData			= GetSystemData();
	sPanelTransferData.m_plnkPanelData			= GetPanelData();

	sPanelTransferData.m_uiTransferType			= DEF_PANEL_TRANSFER_CENTER;
/*
	sPanelTransferData.m_usOAddrAirBlow1		= OUT_PLATE_AIR_LEVITATION1_ON;
	sPanelTransferData.m_usOAddrAirBlow2		= OUT_PLATE_AIR_LEVITATION2_ON;
	
	sPanelTransferData.m_usIAddrAirBlow1		= IN_PLATE_AIR_LEVITATION1_ON;
	sPanelTransferData.m_usIAddrAirBlow2		= IN_PLATE_AIR_LEVITATION2_ON;

	sPanelTransferData.m_usOAddrAirVacuum1		= OUT_PLATE_AIR_VACUUM1_ON;
	sPanelTransferData.m_usOAddrAirVacuum2		= OUT_PLATE_AIR_VACUUM2_ON;

	sPanelTransferData.m_usIAddrAirVacuum1		= IN_PLATE_AIR_VACUUM1_ON;
	sPanelTransferData.m_usIAddrAirVacuum2		= IN_PLATE_AIR_VACUUM2_ON;
*/
	sPanelTransferData.m_usIPanelDetect			= IN_TRANSFER_CENTER_CELL_DETECT;

	m_pPanelTransferCenter = new MPanelTransfer(m_pObjSystemInfo->GetCommonData(DEF_PANEL_TRANSFER_OBJECT_BASE + DEF_PANEL_TRANSFER_CENTER), sRefCompList, sPanelTransferData);
	registerComponentInstance(DEF_PANEL_TRANSFER_OBJECT_BASE + DEF_PANEL_TRANSFER_CENTER, m_pPanelTransferCenter);
#endif
	
	return ERR_SYSTEM_SUCCESS;
}
#endif

int MPlatformOlbSystem::createPanelTransferComponents()
{
#ifdef DEF_SOURCE_SYSTEM
	SPanelTransferRefCompList	sRefCompList;
	SPanelTransferData			sPanelTransferData;

	sRefCompList.m_plnkIO					= GetIOComponent();
	sRefCompList.m_plnkX1					= m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN];
	sRefCompList.m_plnkX2					= m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN_2];
	sRefCompList.m_plnkMultiX				= m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANFSER_IN];

	sRefCompList.m_plnkVac					= m_pVacuum[DEF_VAC_PANEL_TRANSFER_IN];
	sRefCompList.m_plnkVac2					= m_pVacuum[DEF_VAC_PANEL_TRANSFER_IN2];
	sRefCompList.m_plnkUDCyl				= m_pCylinder[DEF_CYL_PANEL_TRANSFER_IN_UD];

	sRefCompList.m_plnkAirRailFB1	= m_pCylinder[DEF_CYL_TRANSFER_IN_AIRRAIL_FB1];
	sRefCompList.m_plnkAirRailFB2	= m_pCylinder[DEF_CYL_TRANSFER_IN_AIRRAIL_FB2];

	sPanelTransferData.m_sXTeachingInfo			= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_PANEL_TRANSFER_IN_X);
	sPanelTransferData.m_strPosFileName			= DEF_SYSTEM_POSITION_FILE_NAME;
	sPanelTransferData.m_strOffsetPosFileName	= DEF_SYSTEM_OFFSET_FILE_NAME;
	sPanelTransferData.m_plnkSystemData			= GetSystemData();
	sPanelTransferData.m_plnkPanelData			= GetPanelData();

	sPanelTransferData.m_uiTransferType			= DEF_PANEL_TRANSFER_IN;

//@	sPanelTransferData.m_usOAddrAirBlow1		= OUT_PLATE_AIR_LEVITATION1_ON;
//@	sPanelTransferData.m_usOAddrAirBlow2		= OUT_PLATE_AIR_LEVITATION2_ON;
	
//@	sPanelTransferData.m_usIAddrAirBlow1		= IN_PLATE_AIR_LEVITATION1_ON;
//@	sPanelTransferData.m_usIAddrAirBlow2		= IN_PLATE_AIR_LEVITATION2_ON;

//@	sPanelTransferData.m_usOAddrAirVacuum1	= OUT_PLATE_AIR_VACUUM1_ON;
//@	sPanelTransferData.m_usOAddrAirVacuum2	= OUT_PLATE_AIR_VACUUM2_ON;

//@	sPanelTransferData.m_usIAddrAirVacuum1		= IN_PLATE_AIR_VACUUM1_ON;
//@	sPanelTransferData.m_usIAddrAirVacuum2		= IN_PLATE_AIR_VACUUM2_ON;

	sPanelTransferData.m_usIPanelDetect			= IN_TRANSFER_IN_CELL_DETECT;

	//120224.KMS____________	
	#ifdef DEF_GATE_SYSTEM
	sPanelTransferData.m_usIPanelDetectLoading	= NULL;
	#else
	sPanelTransferData.m_usIPanelDetectLoading  = IN_PANEL_DETECT_LOADING;
	#endif
	//______________________

	m_pPanelTransfer = new MPanelTransfer(m_pObjSystemInfo->GetCommonData(DEF_PANEL_TRANSFER_OBJECT_BASE + DEF_PANEL_TRANSFER_IN), sRefCompList, sPanelTransferData);
	registerComponentInstance(DEF_PANEL_TRANSFER_OBJECT_BASE + DEF_PANEL_TRANSFER_IN, m_pPanelTransfer);
#endif	
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createPanelTransferOutComponents()
{
	SPanelTransferRefCompList	sRefCompList;
	SPanelTransferData			sPanelTransferData;

	sRefCompList.m_plnkIO					= GetIOComponent();

#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
	sRefCompList.m_plnkX1					= m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT];
	sRefCompList.m_plnkX2					= m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT_2];
	sRefCompList.m_plnkMultiX				= m_pMultiNMCMotion[DEF_AXIS_SERCOS_MULTI_TRANFSER_OUT];
#else 
	sRefCompList.m_plnkX1					= m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT];
	sRefCompList.m_plnkX2					= NULL;
	sRefCompList.m_plnkMultiX				= NULL;
#endif	
	
	sRefCompList.m_plnkVac					= m_pVacuum[DEF_VAC_PANEL_TRANSFER_OUT];
	sRefCompList.m_plnkVac2					= m_pVacuum[DEF_VAC_PANEL_TRANSFER_OUT2];
	sRefCompList.m_plnkUDCyl				= m_pCylinder[DEF_CYL_PANEL_TRANSFER_OUT_UD];

	sPanelTransferData.m_sXTeachingInfo			= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_PANEL_TRANSFER_OUT_X);
	sPanelTransferData.m_strPosFileName			= DEF_SYSTEM_POSITION_FILE_NAME;
	sPanelTransferData.m_strOffsetPosFileName	= DEF_SYSTEM_OFFSET_FILE_NAME;
	sPanelTransferData.m_plnkSystemData			= GetSystemData();
	sPanelTransferData.m_plnkPanelData			= GetPanelData();

	sPanelTransferData.m_uiTransferType			= DEF_PANEL_TRANSFER_OUT;

	sPanelTransferData.m_usIPanelDetect			= IN_TRANSFER_OUT_CELL_DETECT;

	//120224.KMS____________
	sPanelTransferData.m_usIPanelDetectUnLoading  = NULL;//@IN_PANEL_DETECT_UNLOADING;
	//______________________

	m_pPanelTransferOut = new MPanelTransfer(m_pObjSystemInfo->GetCommonData(DEF_PANEL_TRANSFER_OBJECT_BASE + DEF_PANEL_TRANSFER_OUT), sRefCompList, sPanelTransferData);
	registerComponentInstance(DEF_PANEL_TRANSFER_OBJECT_BASE + DEF_PANEL_TRANSFER_OUT, m_pPanelTransferOut);
	
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createTabCarrierComponents()
{
	STabCarrierRefCompList	sRefCompList;
	STabCarrierData			sTabCarrierData;
	
	sTabCarrierData.m_plnkPreBonderData		= GetPreBonderData();
	sTabCarrierData.m_plnkSystemData		= GetSystemData();
	sTabCarrierData.m_plnkTabOffsetData		= GetTabOffsetData();
	sTabCarrierData.m_strOffsetPosFileName	= DEF_SYSTEM_OFFSET_FILE_NAME;
	sTabCarrierData.m_strPosFileName		= DEF_SYSTEM_POSITION_FILE_NAME;
	sTabCarrierData.m_iCamNo1				= DEF_F_TABALIGNER_CAMERA_1;
	sTabCarrierData.m_iCamNo2				= DEF_F_TABALIGNER_CAMERA_2;

	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		sRefCompList.m_plnkXAxis = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER1_X + i];
		//sRefCompList.m_plnkXAxis = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER1_X]; //SJ_YYK 150109 Modify.. //161229 SJ_YSH 변경
		sRefCompList.m_plnkYAxis = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER1_Y + i];
		sRefCompList.m_plnkTAxis = m_pMotion[DEF_AXIS_SERCOS_TABIC_CARRIER1_T + i];

		if(i%2 == 0)  //SJ_YYK 150817 modify...
			sRefCompList.m_plnkExpandAxis = m_pMotion[DEF_AXIS_SERCOS_TABCAMERA_EXPAND];
		else 
			sRefCompList.m_plnkExpandAxis = m_pMotion[DEF_AXIS_SERCOS_TABCAMERA_EXPAND2];

		sRefCompList.m_plnkUDCyl = m_pCylinder[DEF_CYL_TABCARRIER_UD];
		sRefCompList.m_plnkUDCyl2 = m_pCylinder[DEF_CYL_TABCARRIER_UD2]; //SJ_YYK 150817 Add....
		sRefCompList.m_plnkFBCyl = m_pCylinder[DEF_CYL_TABCARRIER_FB];
		sRefCompList.m_plnkCamESCyl = m_pCylinder[DEF_CYL_PREALIGN_CAM_ES];
		sRefCompList.m_plnkVac   = m_pVacuum[DEF_VAC_TABCARRIER1 + i];


		sTabCarrierData.m_sXYTTeachingInfo	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_TABIC_CARRIER1_XYT + i);
		sTabCarrierData.m_sExpandTeachingInfo = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_TABIC_CARRIER_EXPAND);


		sTabCarrierData.m_sZTeachingInfo	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_TABIC_CARRIER_Z); //SJ_YYK 150109 Add..

		if ( i == DEF_MAX_TABCARRIER-1 )
			sTabCarrierData.m_iPreTabCarrierNo = DEF_NONE_TABCARRIER;
		else
			sTabCarrierData.m_iPreTabCarrierNo = i + 1;

		if ( i == 0)
			sTabCarrierData.m_iNextTabCarrierNo = DEF_NONE_TABCARRIER;
		else
			sTabCarrierData.m_iNextTabCarrierNo = i - 1;

		m_pTabCarrier[i] = new MTabCarrier(m_pObjSystemInfo->GetCommonData(DEF_TABCARRIER_OBJECT_BASE + i),
		  								 sRefCompList, 
										 sTabCarrierData);
		registerComponentInstance(DEF_TABCARRIER_OBJECT_BASE + i, m_pTabCarrier[i]);
		
	}
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createRTabCarrierComponents()
{
	int iInstanceNo = 0;
	STabCarrierRefCompList	sRefCompList;
	STabCarrierData			sTabCarrierData;
	
	sTabCarrierData.m_plnkPreBonderData		= GetPreBonderData();
	sTabCarrierData.m_plnkSystemData		= GetSystemData();
	sTabCarrierData.m_plnkTabOffsetData		= GetTabOffsetData();
	sTabCarrierData.m_strOffsetPosFileName	= DEF_SYSTEM_OFFSET_FILE_NAME;
	sTabCarrierData.m_strPosFileName		= DEF_SYSTEM_POSITION_FILE_NAME;
	sTabCarrierData.m_iCamNo1				= DEF_R_TABALIGNER_CAMERA_1;
	sTabCarrierData.m_iCamNo2				= DEF_R_TABALIGNER_CAMERA_2;

	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		sRefCompList.m_plnkXAxis = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X + i];  //SJ_YYK 150109 Modify..
		//sRefCompList.m_plnkXAxis = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X]; //161229 SJ_YSH
		sRefCompList.m_plnkYAxis = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y + i];
		sRefCompList.m_plnkTAxis = m_pMotion[DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T + i];
		if(i%2 == 0) //SJ_YYK 150817 modify...
			sRefCompList.m_plnkExpandAxis = m_pMotion[DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND];
		else
			sRefCompList.m_plnkExpandAxis = m_pMotion[DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2];

		///임시sRefCompList.m_plnkUDCyl = m_pCylinder[DEF_CYL_R_TABCARRIER1_UD + i];
		sRefCompList.m_plnkUDCyl = m_pCylinder[DEF_CYL_R_TABCARRIER_UD];
		sRefCompList.m_plnkUDCyl2 = m_pCylinder[DEF_CYL_R_TABCARRIER_UD2]; //SJ_YYK 150817 Add....
		sRefCompList.m_plnkFBCyl = m_pCylinder[DEF_CYL_R_TABCARRIER_FB];
		sRefCompList.m_plnkCamESCyl = m_pCylinder[DEF_CYL_R_PREALIGN_CAM_ES];
		sRefCompList.m_plnkVac   = m_pVacuum[DEF_VAC_R_TABCARRIER1 + i];

		sTabCarrierData.m_sXYTTeachingInfo	  = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_TABIC_CARRIER1_XYT + i);
		if(i%2 == 0) //SJ_YYK 150817 modify...
			sTabCarrierData.m_sExpandTeachingInfo = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_TABIC_CARRIER_EXPAND);
		else 
			sTabCarrierData.m_sExpandTeachingInfo = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_TABIC_CARRIER_EXPAND2);
		sTabCarrierData.m_sZTeachingInfo	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_TABIC_CARRIER_Z); //SJ_YYK 150109 Add..

		if ( i == DEF_MAX_TABCARRIER-1 )
			sTabCarrierData.m_iPreTabCarrierNo = DEF_NONE_TABCARRIER;
		else
			sTabCarrierData.m_iPreTabCarrierNo = i + 1;

		if ( i == 0)
			sTabCarrierData.m_iNextTabCarrierNo = DEF_NONE_TABCARRIER;
		else
			sTabCarrierData.m_iNextTabCarrierNo = i - 1;


//		iInstanceNo = DEF_MAX_TABCARRIER*2 - 1 - i;
		iInstanceNo = DEF_MAX_TABCARRIER + i;
		m_pRTabCarrier[i] = new MTabCarrier(m_pObjSystemInfo->GetCommonData(DEF_TABCARRIER_OBJECT_BASE + iInstanceNo),
		  								 sRefCompList, 
										 sTabCarrierData);
		registerComponentInstance(DEF_TABCARRIER_OBJECT_BASE + iInstanceNo, m_pRTabCarrier[i]);
		
	}
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createTabFeederComponents()
{
	STabFeederRefCompList	sRefCompList;
	STabFeederData			sTabFeederData;

	/** Tab Feeder Common */
	sRefCompList.m_plnkIO								= GetIOComponent();

	sTabFeederData.m_plnkSystemData						= GetSystemData();
	sTabFeederData.m_plnkPreBonderData					= GetPreBonderData();
	sTabFeederData.m_strPressPosFileName				= DEF_SYSTEM_POSITION_FILE_NAME;
	sTabFeederData.m_strPressOffsetPosFileName			= DEF_SYSTEM_OFFSET_FILE_NAME;
	sTabFeederData.m_strBadTabPosFileName				= DEF_SYSTEM_POSITION_FILE_NAME;
	sTabFeederData.m_strBadTabOffsetPosFileName			= DEF_SYSTEM_OFFSET_FILE_NAME;
	sTabFeederData.m_strSprocket1PosFileName			= DEF_SYSTEM_POSITION_FILE_NAME;
	sTabFeederData.m_strSprocket1OffsetPosFileName		= DEF_SYSTEM_OFFSET_FILE_NAME;
	sTabFeederData.m_strSprocket2PosFileName			= DEF_SYSTEM_POSITION_FILE_NAME;
	sTabFeederData.m_strSprocket2OffsetPosFileName		= DEF_SYSTEM_OFFSET_FILE_NAME;
//@	sTabFeederData.m_strSupplierPosFileName				= DEF_SYSTEM_POSITION_FILE_NAME;
//@	sTabFeederData.m_strSupplierOffsetPosFileName		= DEF_SYSTEM_OFFSET_FILE_NAME;
	sTabFeederData.m_strMoldPinPosFileName				= DEF_SYSTEM_POSITION_FILE_NAME;
	sTabFeederData.m_strMoldPinOffsetPosFileName		= DEF_SYSTEM_OFFSET_FILE_NAME;
	/** Tab Feeder1 */
	sRefCompList.m_plnkPressAxis						= m_pMotion[DEF_AXIS_SERCOS_TAB_PRESS_Z];
	sRefCompList.m_plnkSprocket1Axis					= m_pMotion[DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T];
	sRefCompList.m_plnkSprocket2Axis					= m_pMotion[DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T];

	sRefCompList.m_plnkBadTabAxis						= m_pMotion[DEF_AXIS_SERCOS_BADTAB_DETECTOR_X];
	sRefCompList.m_plnkMoldPinAxis						= m_pMotion[DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z];
	sRefCompList.m_plnkFeederReelAxis					= m_pMotion[DEF_AXIS_SERCOS_FEEDER_REEL_T];
	sRefCompList.m_plnkSupplierAxis						= NULL;

	sRefCompList.m_plnkMultiSprocketAxis				= m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_FRONT_SPROCKET];


	/*
#ifdef DEF_USE_FEEDER_SPROCKET_UD_CYL
	sRefCompList.m_plnkFeederSprocketUDCyl				= m_pCylinder[DEF_CYL_TABPRESS1_FEEDER_SPROCKET_UD];
#endif
#ifdef DEF_USE_TENSION_SPROCKET_UD_CYL
	sRefCompList.m_plnkTensionSprocketUDCyl				= m_pCylinder[DEF_CYL_TABPRESS1_TENSION_SPROCKET_UD];
#endif
	*/

	sRefCompList.m_plnkRecoverySprocketUDCyl			= m_pCylinder[DEF_CYL_TABPRESS1_RECOVERY_SPROCKET_UD];
//@	sRefCompList.m_plnkPressReelUDCyl					= m_pCylinder[DEF_CYL_TABPRESS1_PRESS_REEL_UD];
//@	sRefCompList.m_plnkPressClampUDCyl					= m_pCylinder[DEF_CYL_TABPRESS1_PRESS_REEL_CLAMP_UD];
//@	sRefCompList.m_plnkAttachCuttingSupportLeftUDCyl	= m_pCylinder[DEF_CYL_TABPRESS1_ATTACH_CUTTING_SUPPORT_LEFT_UD];
//@	sRefCompList.m_plnkAttachCuttingSupportRightUDCyl	= m_pCylinder[DEF_CYL_TABPRESS1_ATTACH_CUTTING_SUPPORT_RIGHT_UD];
//@ sRefCompList.m_plnkAttachPressUDCyl					= m_pCylinder[DEF_CYL_TABPRESS1_ATTACH_PRESS_UD];
//@ sRefCompList.m_plnkAttachReelCutterUDCyl			= m_pCylinder[DEF_CYL_TABPRESS1_ATTACH_REEL_CUTTER_UD];
//@	sRefCompList.m_plnkAttachReelFBCyl					= m_pCylinder[DEF_CYL_TABPRESS1_ATTACH_REEL_FB];
//@ sRefCompList.m_plnkSupplierChuckOCCyl				= m_pCylinder[DEF_CYL_TABPRESS1_ATTACH_SUPPLIER_CHUCK_ES];
	sRefCompList.m_plnkPressVac							= m_pVacuum[DEF_VAC_TAB_PRESS1];
//@	sRefCompList.m_plnkNewReelVac						= m_pVacuum[DEF_VAC_TAB_PRESS1_NEW_REEL];

	sTabFeederData.m_sPressTeachingInfo					= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_TAB_PRESS_Z);
	sTabFeederData.m_sBadTabTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_BADTAB_DETECTOR_X);
	sTabFeederData.m_sSprocket1TeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_FEEDER_SPROCKET1_T);
	sTabFeederData.m_sSprocket2TeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_FEEDER_SPROCKET2_T);
//@	sTabFeederData.m_sSupplierTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_FEEDER_SUPPLIER_Y);
	sTabFeederData.m_sMoldPinTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_MOLDING_UPDOWN_Z);

	sTabFeederData.m_iGroupNo							= DEF_FRONT_GROUP;
	sTabFeederData.m_usOAddrMoldBlowOn					= OUT_TABPRESS1_PRESS_MOLD_BLOW;
	sTabFeederData.m_usOAddrRecoveryTensionMotorOn		= OUT_TABPRESS1_RECOVERY_TENSION_SPROCKET_MOTOR_ON;

//@	sTabFeederData.m_usOAddrSupplierTensionMotorOn		= OUT_TABPRESS1_SUPPLIER_TENSION_MOTOR_ON;
//@	sTabFeederData.m_usOAddrSupplierLargeTension		= OUT_TABPRESS1_SUPPLIER_LARGE_TENSION;
//@	sTabFeederData.m_usOAddrSupplierMiddleTension		= OUT_TABPRESS1_SUPPLIER_MIDDLE_TENSION;
//@	sTabFeederData.m_usOAddrSupplierSmallTension		= OUT_TABPRESS1_SUPPLIER_SMALL_TENSION;
//@	sTabFeederData.m_usOAddrCleaningMotorOn				= OUT_TABPRESS1_CLEANING_MOTOR_ON;
//@	sTabFeederData.m_usOAddrPReelTensionMotorOn			= OUT_TABPRESS1_P_REEL_MOTOR_ON;
	sTabFeederData.m_usOAddrPressRFIDReset				= OUT_TABPRESS1_PRESS_RFID_RESET;
//@	sTabFeederData.m_usIAddrBadTabDetect[0]				= IN_TABPRESS1_BADTAB_LEFT_TCP_DETECT;
//@	sTabFeederData.m_usIAddrBadTabDetect[1]				= IN_TABPRESS1_BADTAB_LEFT_COF_DETECT;
//@	sTabFeederData.m_usIAddrBadTabDetect[2]				= IN_TABPRESS1_BADTAB_RIGHT_TCP_DETECT;
//@	sTabFeederData.m_usIAddrBadTabDetect[3]				= IN_TABPRESS1_BADTAB_RIGHT_COF_DETECT;
	sTabFeederData.m_usIAddrBadTabDetect[0]				= IN_TABPRESS1_BADTAB_TCP_DETECT;
	sTabFeederData.m_usIAddrBadTabDetect[1]				= IN_TABPRESS1_BADTAB_COF_DETECT;
//@	sTabFeederData.m_usIAddrBadTabDetect[2]				= NULL;
//@	sTabFeederData.m_usIAddrBadTabDetect[3]				= NULL;
	sTabFeederData.m_usIAddrTensionUpperLimitDetect		= IN_TABPRESS1_WEIGHT_TENSION_UP;	//@IN_TABPRESS1_WEIGHT_TENSION_UP_LIMIT;
	sTabFeederData.m_usIAddrTensionUpperDetect			= IN_TABPRESS1_WEIGHT_TENSION_UP;
	sTabFeederData.m_usIAddrTensionLowerDetect			= IN_TABPRESS1_WEIGHT_TENSION_DOWN;
	sTabFeederData.m_usIAddrTensionLowerLimitDetect		= IN_TABPRESS1_WEIGHT_TENSION_DOWN;	//@IN_TABPRESS1_WEIGHT_TENSION_DOWN_LIMIT;
	sTabFeederData.m_usIAddrSupplierLargeDetect			= IN_TABPRESS1_SUPPLIER_LARGE_DETECT;
	sTabFeederData.m_usIAddrSupplierMiddleDetect		= IN_TABPRESS1_SUPPLIER_MIDDLE_DETECT;
	sTabFeederData.m_usIAddrSupplierSmallDetect			= IN_TABPRESS1_SUPPLIER_SMALL_DETECT;
//@	sTabFeederData.m_usIAddrSupplierCurrentReelDetect	= IN_TABPRESS1_SUPPLIER_CURRENT_REEL_DETECT;
//@	sTabFeederData.m_usIAddrSupplierNewReelDetect		= IN_TABPRESS1_SUPPLIER_NEW_REEL_DETECT;
	sTabFeederData.m_usIAddrRailUnfoldDetect			= IN_TABPRESS1_RAIL_UNFOLD_DETECT;
	sTabFeederData.m_usIAddrMoldDetect					= IN_TABPRESS1_PRESS_MOLD_DETECT;
//@	sTabFeederData.m_usIAddrProtectDownDetect			= IN_TABPRESS1_RECOVERY_PROTECT_ARM_DOWN;
	sTabFeederData.m_usOAddrSprocket1Clutch				= OUT_TABPRESS1_SPROCKET_CLUTCH_ONOFF;
	sTabFeederData.m_usIAddrProtectRecoveryArmUpDetect	= IN_TABPRESS1_RECOVERY_PROTECT_ARM_UP;
	sTabFeederData.m_usIAddrProtectRecoveryArmDownDetect = IN_TABPRESS1_RECOVERY_PROTECT_ARM_DOWN;

	sTabFeederData.m_usOPReelMotorPower					= OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_ON;	//@OUT_TABPRESS1_PREEL_MOTOR_POWER_ON;	//?????
	sTabFeederData.m_usOPReelMotorWind					= OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_CW;	//@OUT_TABPRESS1_PREEL_MOTOR_WIND;		//?????
	sTabFeederData.m_usOPReelMotorRelease				= OUT_TABPRESS1_RECOVERY_TENSION_MOTOR_CCW;	//@OUT_TABPRESS1_PREEL_MOTOR_RELEASE;	//?????
	sTabFeederData.m_usOPReelMotorBreak					= NULL;	//@OUT_TABPRESS1_PREEL_MOTOR_BREAK;		//?????

	//150730_KDH
	m_pTabFeeder[DEF_TABFEEDER1] = 	new MTabFeeder(m_pObjSystemInfo->GetCommonData(DEF_TABFEEDER_OBJECT_BASE), sRefCompList, sTabFeederData);
	registerComponentInstance(DEF_TABFEEDER_OBJECT_BASE, m_pTabFeeder[DEF_TABFEEDER1]);

	
	/** Tab Feeder2 */
	sRefCompList.m_plnkPressAxis						= m_pMotion[DEF_AXIS_SERCOS_R_TAB_PRESS_Z];
	sRefCompList.m_plnkSprocket1Axis					= m_pMotion[DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T];
	sRefCompList.m_plnkSprocket2Axis					= m_pMotion[DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T];

	sRefCompList.m_plnkBadTabAxis						= m_pMotion[DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X];
	sRefCompList.m_plnkMoldPinAxis						= m_pMotion[DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z];
	sRefCompList.m_plnkFeederReelAxis					= m_pMotion[DEF_AXIS_SERCOS_R_FEEDER_REEL_T];
	sRefCompList.m_plnkSupplierAxis						= NULL;

	sRefCompList.m_plnkMultiSprocketAxis				= m_pMultiSercosMotion[DEF_AXIS_SERCOS_MULTI_REAR_SPROCKET];


	/*
#ifdef DEF_USE_FEEDER_SPROCKET_UD_CYL
	sRefCompList.m_plnkFeederSprocketUDCyl				= m_pCylinder[DEF_CYL_TABPRESS2_FEEDER_SPROCKET_UD];
#endif
#ifdef DEF_USE_TENSION_SPROCKET_UD_CYL
	sRefCompList.m_plnkTensionSprocketUDCyl				= m_pCylinder[DEF_CYL_TABPRESS2_TENSION_SPROCKET_UD];
#endif
	*/

	sRefCompList.m_plnkRecoverySprocketUDCyl			= m_pCylinder[DEF_CYL_R_TABPRESS1_RECOVERY_SPROCKET_UD];
//@	sRefCompList.m_plnkPressReelUDCyl					= m_pCylinder[DEF_CYL_TABPRESS2_PRESS_REEL_UD];
//@	sRefCompList.m_plnkPressClampUDCyl					= m_pCylinder[DEF_CYL_TABPRESS2_PRESS_REEL_CLAMP_UD];
//@	sRefCompList.m_plnkAttachCuttingSupportLeftUDCyl	= m_pCylinder[DEF_CYL_TABPRESS2_ATTACH_CUTTING_SUPPORT_LEFT_UD];
//@	sRefCompList.m_plnkAttachCuttingSupportRightUDCyl	= m_pCylinder[DEF_CYL_TABPRESS2_ATTACH_CUTTING_SUPPORT_RIGHT_UD];
//@    sRefCompList.m_plnkAttachPressUDCyl					= m_pCylinder[DEF_CYL_TABPRESS2_ATTACH_PRESS_UD];
//@    sRefCompList.m_plnkAttachReelCutterUDCyl			= m_pCylinder[DEF_CYL_TABPRESS2_ATTACH_REEL_CUTTER_UD];
//@    sRefCompList.m_plnkAttachReelFBCyl					= m_pCylinder[DEF_CYL_TABPRESS2_ATTACH_REEL_FB];
//@    sRefCompList.m_plnkSupplierChuckOCCyl				= m_pCylinder[DEF_CYL_TABPRESS2_ATTACH_SUPPLIER_CHUCK_ES];
	sRefCompList.m_plnkPressVac							= m_pVacuum[DEF_VAC_TAB_PRESS2];
//@	sRefCompList.m_plnkNewReelVac						= m_pVacuum[DEF_VAC_TAB_PRESS2_NEW_REEL];

	sTabFeederData.m_sPressTeachingInfo					= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_TAB_PRESS_Z);
	sTabFeederData.m_sBadTabTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_BADTAB_DETECTOR_X);
	sTabFeederData.m_sSprocket1TeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_FEEDER_SPROCKET1_T);
	sTabFeederData.m_sSprocket2TeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_FEEDER_SPROCKET2_T);
//@	sTabFeederData.m_sSupplierTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_FEEDER_SUPPLIER_Y);
	sTabFeederData.m_sMoldPinTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_MOLDING_UPDOWN_Z);

	sTabFeederData.m_iGroupNo							= DEF_REAR_GROUP;
	sTabFeederData.m_usOAddrMoldBlowOn					= OUT_TABPRESS2_PRESS_MOLD_BLOW;
	sTabFeederData.m_usOAddrRecoveryTensionMotorOn		= OUT_TABPRESS2_RECOVERY_TENSION_SPROCKET_MOTOR_ON;
//@	sTabFeederData.m_usOAddrSupplierTensionMotorOn		= OUT_TABPRESS2_SUPPLIER_TENSION_MOTOR_ON;
//@	sTabFeederData.m_usOAddrSupplierLargeTension		= OUT_TABPRESS2_SUPPLIER_LARGE_TENSION;
//@	sTabFeederData.m_usOAddrSupplierMiddleTension		= OUT_TABPRESS2_SUPPLIER_MIDDLE_TENSION;
//@	sTabFeederData.m_usOAddrSupplierSmallTension		= OUT_TABPRESS2_SUPPLIER_SMALL_TENSION;
//@	sTabFeederData.m_usOAddrCleaningMotorOn				= OUT_TABPRESS2_CLEANING_MOTOR_ON;
//@	sTabFeederData.m_usOAddrPReelTensionMotorOn			= OUT_TABPRESS2_P_REEL_MOTOR_ON;
	sTabFeederData.m_usOAddrPressRFIDReset				= OUT_TABPRESS2_PRESS_RFID_RESET;
//@	sTabFeederData.m_usIAddrBadTabDetect[0]				= IN_TABPRESS2_BADTAB_LEFT_TCP_DETECT;
//@	sTabFeederData.m_usIAddrBadTabDetect[1]				= IN_TABPRESS2_BADTAB_LEFT_COF_DETECT;
//@	sTabFeederData.m_usIAddrBadTabDetect[2]				= IN_TABPRESS2_BADTAB_RIGHT_TCP_DETECT;
//@	sTabFeederData.m_usIAddrBadTabDetect[3]				= IN_TABPRESS2_BADTAB_RIGHT_COF_DETECT;
	sTabFeederData.m_usIAddrBadTabDetect[0]				= IN_TABPRESS2_BADTAB_TCP_DETECT;
	sTabFeederData.m_usIAddrBadTabDetect[1]				= IN_TABPRESS2_BADTAB_COF_DETECT;
//@	sTabFeederData.m_usIAddrBadTabDetect[2]				= NULL;
//@	sTabFeederData.m_usIAddrBadTabDetect[3]				= NULL;
	sTabFeederData.m_usIAddrTensionUpperLimitDetect		= IN_TABPRESS2_WEIGHT_TENSION_UP;	//IN_TABPRESS2_WEIGHT_TENSION_UP_LIMIT;
	sTabFeederData.m_usIAddrTensionUpperDetect			= IN_TABPRESS2_WEIGHT_TENSION_UP;
	sTabFeederData.m_usIAddrTensionLowerDetect			= IN_TABPRESS2_WEIGHT_TENSION_DOWN;
	sTabFeederData.m_usIAddrTensionLowerLimitDetect		= IN_TABPRESS2_WEIGHT_TENSION_DOWN;	//IN_TABPRESS2_WEIGHT_TENSION_DOWN_LIMIT;
	sTabFeederData.m_usIAddrSupplierLargeDetect			= IN_TABPRESS2_SUPPLIER_LARGE_DETECT;
	sTabFeederData.m_usIAddrSupplierMiddleDetect		= IN_TABPRESS2_SUPPLIER_MIDDLE_DETECT;
	sTabFeederData.m_usIAddrSupplierSmallDetect			= IN_TABPRESS2_SUPPLIER_SMALL_DETECT;
//@	sTabFeederData.m_usIAddrSupplierCurrentReelDetect	= IN_TABPRESS2_SUPPLIER_CURRENT_REEL_DETECT;
//@	sTabFeederData.m_usIAddrSupplierNewReelDetect		= IN_TABPRESS2_SUPPLIER_NEW_REEL_DETECT;
	sTabFeederData.m_usIAddrRailUnfoldDetect			= IN_TABPRESS2_RAIL_UNFOLD_DETECT;
	sTabFeederData.m_usIAddrMoldDetect					= IN_TABPRESS2_PRESS_MOLD_DETECT;
//@	sTabFeederData.m_usIAddrProtectDownDetect			= IN_TABPRESS2_RECOVERY_PROTECT_ARM_DOWN;
	sTabFeederData.m_usOAddrSprocket1Clutch				= OUT_TABPRESS2_SPROCKET_CLUTCH_ONOFF;
	sTabFeederData.m_usIAddrProtectRecoveryArmUpDetect	= IN_TABPRESS2_RECOVERY_PROTECT_ARM_UP;
	sTabFeederData.m_usIAddrProtectRecoveryArmDownDetect = IN_TABPRESS2_RECOVERY_PROTECT_ARM_DOWN;

	sTabFeederData.m_usOPReelMotorPower					= OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_ON;	//@OUT_TABPRESS2_PREEL_MOTOR_POWER_ON;	//?????
	sTabFeederData.m_usOPReelMotorWind					= OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_CW;	//@OUT_TABPRESS2_PREEL_MOTOR_WIND;		//?????
	sTabFeederData.m_usOPReelMotorRelease				= OUT_TABPRESS2_RECOVERY_TENSION_MOTOR_CCW;	//@OUT_TABPRESS2_PREEL_MOTOR_RELEASE;	//?????
	sTabFeederData.m_usOPReelMotorBreak					= NULL;	//@OUT_TABPRESS2_PREEL_MOTOR_BREAK;		//?????

	m_pTabFeeder[DEF_TABFEEDER2] = new MTabFeeder(m_pObjSystemInfo->GetCommonData(DEF_TABFEEDER_OBJECT_BASE + DEF_TABFEEDER2), sRefCompList, sTabFeederData);
	registerComponentInstance(DEF_TABFEEDER_OBJECT_BASE + DEF_TABFEEDER2, m_pTabFeeder[DEF_TABFEEDER2]);

	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createLCNetComponent()
{
	// 확인 필요.
	// 수정 필요함. 
	m_pLCNet = new MLCNet(GetSystemData()->m_strLineControllerIP, GetSystemData()->m_uiLineControllerPort);
	m_pLCNet->SetUnitNo(2 + GetSystemData()->m_iUnitNumber);
	m_pLCNet->SetMaxPanel(4);
	m_pLCNet->m_sCurModelName = GetSystemData()->m_strModelFileName;
	//110812 SJ_KJS
	m_pLCNet->SetUnitReportID(GetSystemData()->m_strReportModuelID);


#ifndef SIMULATION
	m_pLCNet->Create();
	//LCNet Start
	m_pLCNet->Start();
#endif

	CString	srgModelName[DEF_MAX_MODEL];
	int	iModelCount = 0;

	GetModelNameList(srgModelName, &iModelCount);
	MProcessProgram	 oPPID;
	for (int i = 0; i < iModelCount; i++)
	{
		oPPID.m_strPPID = srgModelName[i]; 
		m_pLCNet->SetProcessProgram(oPPID.m_strPPID, &oPPID);
	}

	registerComponentInstance(DEF_ONLINE_OBJECT_BASE, m_pLCNet);

	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createCameraCarrierComponents()
{
	int i = 0;
	SCameraCarrierRefCompList	sRefCompList;
	SCameraCarrierData			sCameraCarrierData;

	/** Camera Carrier Common Data */
	sCameraCarrierData.m_plnkSystemData				= GetSystemData();
	sCameraCarrierData.m_plnkPanelData				= GetPanelData();
	sCameraCarrierData.m_plnkPreBonderData			= GetPreBonderData();
	sCameraCarrierData.m_plnkProcessData			= GetProcessData();
	sCameraCarrierData.m_plnkTabData				= GetTabData(); //SJ_YYK 150109 Add..
//	sCameraCarrierData.m_plnkWorkSchedule			= GetWorkSchedule();
	sCameraCarrierData.m_strOffsetPosFileName		= DEF_SYSTEM_OFFSET_FILE_NAME;
	sCameraCarrierData.m_strPosFileName				= DEF_SYSTEM_POSITION_FILE_NAME;

	sCameraCarrierData.m_plnkWorkSchedule			= GetWorkSchedule(DEF_FRONT_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sCameraCarrierData.m_plnkWorkScheduleSecond		= GetWorkScheduleSecond(DEF_FRONT_GROUP);
#endif
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sRefCompList.m_plnkAxis			= m_pMotion[DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X + i];
		sRefCompList.m_plnkAxisBackupZ	= m_pMotion[DEF_AXIS_SERCOS_BACKUP1_Z + i];
		sRefCompList.m_plnkAxisExpand	= m_pMotion[DEF_AXIS_SERCOS_INSPECT_EXPAND1 + i];
		sRefCompList.m_plnkESCyl		= m_pCylinder[DEF_CYL_INSPECTION_CAM1_ES + i];

		sCameraCarrierData.m_sTeachingInfo			= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_INSPECTION_CAMERA1_X + i);
		sCameraCarrierData.m_sTeachingInfoBackupZ	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_BACKUP1_Z + i);
		sCameraCarrierData.m_sTeachingInfoExpand	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_INSPECTION_CAMERA1_EXPAND + i);
		sCameraCarrierData.m_iGroupNo				= DEF_FRONT_GROUP;
#ifdef DEF_SOURCE_SYSTEM
			//SJ_YYK 150907 Add..
#	ifndef DEF_MIRROR_
		sCameraCarrierData.m_eWorkSide = WORKING_SIDE_X_UP;

#	else
		sCameraCarrierData.m_eWorkSide = WORKING_SIDE_X_UP;
#	endif
#else
#	ifndef DEF_MIRROR_
		sCameraCarrierData.m_eWorkSide = WORKING_SIDE_Y_LEFT;
#	else
		sCameraCarrierData.m_eWorkSide = WORKING_SIDE_Y_RIGHT;
#	endif
#endif
		sCameraCarrierData.m_iCamNo1				= DEF_INSPECTION_CAMERA_1 + 2*i;
		sCameraCarrierData.m_iCamNo2				= DEF_INSPECTION_CAMERA_2 + 2*i;

		m_pCameraCarrier[i] = new MCameraCarrier(m_pObjSystemInfo->GetCommonData(DEF_CAMERA_CARRIER_OBJECT_BASE + i), sRefCompList, sCameraCarrierData);
		registerComponentInstance(DEF_CAMERA_CARRIER_OBJECT_BASE + i, m_pCameraCarrier[i]);
	}

	sCameraCarrierData.m_plnkWorkSchedule			= GetWorkSchedule(DEF_REAR_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sCameraCarrierData.m_plnkWorkScheduleSecond		= GetWorkScheduleSecond(DEF_REAR_GROUP);
#endif
	
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sRefCompList.m_plnkAxis			= m_pMotion[DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X + i];
		sRefCompList.m_plnkAxisBackupZ	= m_pMotion[DEF_AXIS_SERCOS_R_BACKUP1_Z + i];
		sRefCompList.m_plnkAxisExpand	= m_pMotion[DEF_AXIS_SERCOS_R_INSPECT_EXPAND1 + i];
		sRefCompList.m_plnkESCyl		= m_pCylinder[DEF_CYL_R_INSPECTION_CAM1_ES + i];

		sCameraCarrierData.m_sTeachingInfo			= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA1_X + i);
		sCameraCarrierData.m_sTeachingInfoBackupZ	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_BACKUP1_Z + i);
		sCameraCarrierData.m_sTeachingInfoExpand	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_INSPECTION_CAMERA1_EXPAND + i);
		sCameraCarrierData.m_iGroupNo				= DEF_REAR_GROUP;
#ifdef DEF_SOURCE_SYSTEM
		//SJ_YYK 150907 Add..
#	ifndef DEF_MIRROR_
		sCameraCarrierData.m_eWorkSide = WORKING_SIDE_X_DOWN;
#	else
		sCameraCarrierData.m_eWorkSide = WORKING_SIDE_X_DOWN;
#	endif
#else
#	ifndef DEF_MIRROR_
		sCameraCarrierData.m_eWorkSide = WORKING_SIDE_Y_RIGHT;
#	else
		sCameraCarrierData.m_eWorkSide = WORKING_SIDE_Y_LEFT;
#	endif
#endif
		sCameraCarrierData.m_iCamNo1				= DEF_R_INSPECTION_CAMERA_1 + 2*i;
		sCameraCarrierData.m_iCamNo2				= DEF_R_INSPECTION_CAMERA_2 + 2*i;


		m_pRCameraCarrier[i] = new MCameraCarrier(m_pObjSystemInfo->GetCommonData(DEF_CAMERA_CARRIER_OBJECT_BASE + i + DEF_MAX_INSPECTION_CAM_CARRIER), sRefCompList, sCameraCarrierData);
		registerComponentInstance(DEF_CAMERA_CARRIER_OBJECT_BASE + i + DEF_MAX_INSPECTION_CAM_CARRIER, m_pCameraCarrier[i]);
	}

	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createTabMounterComponents()
{
	int i = 0;
	STabMounterRefCompList	sRefCompList;
	STabMounterData			sTabMounterData;

	/** Camera Carrier Common Data */
	sTabMounterData.m_plnkSystemData		= GetSystemData();
	sTabMounterData.m_plnkPanelData			= GetPanelData();
	sTabMounterData.m_plnkPreBonderData		= GetPreBonderData();
	sTabMounterData.m_plnkProcessData		= GetProcessData();
//	sTabMounterData.m_plnkWorkSchedule		= GetWorkSchedule();
	sTabMounterData.m_plnkTabOffsetData		= GetTabOffsetData();
	sTabMounterData.m_plnkRearTabOffsetData = GetTabOffsetData_Rear(); //120208.kms
	sTabMounterData.m_strOffsetPosFileName	= DEF_SYSTEM_OFFSET_FILE_NAME;
	sTabMounterData.m_strPosFileName		= DEF_SYSTEM_POSITION_FILE_NAME;
	sRefCompList.m_plnkGantryAxis			= NULL;

	sTabMounterData.m_plnkWorkSchedule		= GetWorkSchedule(DEF_FRONT_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sTabMounterData.m_plnkWorkScheduleSecond	= GetWorkScheduleSecond(DEF_FRONT_GROUP);
#endif
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		sTabMounterData.m_iGroupNo = DEF_FRONT_GROUP;
		sTabMounterData.m_sTeachingInfoMounter = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_MOUNTER1_X + i);
		sTabMounterData.m_sTeachingInfoMounterZ = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_MOUNTER1_Z + i);
		if (0 == i)
			sTabMounterData.m_sTeachingInfoGantry = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_TABMOUNTER_GANTRY_Y);

		sRefCompList.m_plnkAxis = m_pMotion[DEF_AXIS_SERCOS_MOUNTER1_X + i];
		sRefCompList.m_plnkAxisZ = m_pMotion[DEF_AXIS_SERCOS_MOUNTER1_Z + i];
		sRefCompList.m_plnkGantryAxis = m_pMotion[DEF_AXIS_ACS_GANTRY_Y];

		sRefCompList.m_plnk1stUDCyl = m_pCylinder[DEF_CYL_TABMOUNTER1_PRESS_UD + i];
		sRefCompList.m_plnkVac = m_pVacuum[DEF_VAC_TAB_MOUNTER1 + i];

		sRefCompList.m_plnkPusherUDCyl = m_pCylinder[DEF_CYL_TABMOUNTER1_PANEL_PUSHER_UD + i];

#ifdef DEF_SOURCE_SYSTEM
		//SJ_YYK 150907 Add...
#	ifndef DEF_MIRROR_
		sTabMounterData.m_eWorkSide = WORKING_SIDE_X_UP;
#	else
		sTabMounterData.m_eWorkSide = WORKING_SIDE_X_UP;
#	endif
#else
#	ifndef DEF_MIRROR_
		sTabMounterData.m_eWorkSide = WORKING_SIDE_Y_LEFT;
#	else
		sTabMounterData.m_eWorkSide = WORKING_SIDE_Y_RIGHT;
#	endif
#endif
		/** **************************************************************************
		 *	이전/다음 TabMounter 번호 설정
		 *  ⇒ 이전/다음 구분 기준
		 *		- 자신보다 먼저 작업 위치(Prealign 위치)로 이동하는 TabMounter 가 이전 TabMounter 임.
		 *		- Gate 의 경우 TabCarrier Unload 위치에 따라 Front/Rear Group 의 TabMounter 출발 위치가 달라짐.
		 */
		if (DEF_MAX_TABMOUNTER-1 == i)
			sTabMounterData.m_iPreTabMounterNo = DEF_NONE_TABMOUNTER;
		else
			sTabMounterData.m_iPreTabMounterNo = i + 1;
		
		if (0 == i)
			sTabMounterData.m_iNextTabMounterNo = DEF_NONE_TABMOUNTER;
		else
			sTabMounterData.m_iNextTabMounterNo = i - 1;

		m_pTabMounter[i] = new MTabMounter(m_pObjSystemInfo->GetCommonData(DEF_TAB_MOUNTER_OBJECT_BASE + i), sRefCompList, sTabMounterData);
		registerComponentInstance(DEF_TAB_MOUNTER_OBJECT_BASE + i, m_pTabMounter[i]);
	}

	sTabMounterData.m_plnkWorkSchedule		= GetWorkSchedule(DEF_REAR_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sTabMounterData.m_plnkWorkScheduleSecond	= GetWorkScheduleSecond(DEF_REAR_GROUP);
#endif
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		sTabMounterData.m_iGroupNo = DEF_REAR_GROUP;
		sTabMounterData.m_sTeachingInfoMounter = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_MOUNTER1_X + i);
		sTabMounterData.m_sTeachingInfoMounterZ = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_MOUNTER1_Z + i);
		if (0 == i)
			sTabMounterData.m_sTeachingInfoGantry = m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_R_TABMOUNTER_GANTRY_Y);

		sRefCompList.m_plnkAxis = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER1_X + i];
		sRefCompList.m_plnkAxisZ = m_pMotion[DEF_AXIS_SERCOS_R_MOUNTER1_Z + i];
		sRefCompList.m_plnkGantryAxis = m_pMotion[DEF_AXIS_ACS_R_GANTRY_Y];

		sRefCompList.m_plnk1stUDCyl = m_pCylinder[DEF_CYL_R_TABMOUNTER1_PRESS_UD + i];
		sRefCompList.m_plnkVac = m_pVacuum[DEF_VAC_R_TAB_MOUNTER1 + i];

		sRefCompList.m_plnkPusherUDCyl = m_pCylinder[DEF_CYL_RTABMOUNTER1_PANEL_PUSHER_UD + i];

#ifdef DEF_SOURCE_SYSTEM
//SJ_YYK 150907 Add...
#	ifndef DEF_MIRROR_
		sTabMounterData.m_eWorkSide = WORKING_SIDE_X_DOWN;
#	else
		sTabMounterData.m_eWorkSide = WORKING_SIDE_X_DOWN;
#	endif
#else
#	ifndef DEF_MIRROR_
		sTabMounterData.m_eWorkSide = WORKING_SIDE_Y_RIGHT;
#	else
		sTabMounterData.m_eWorkSide = WORKING_SIDE_Y_LEFT;
#	endif
#endif
		/** **************************************************************************
		 *	이전/다음 TabMounter 번호 설정
		 *  ⇒ 이전/다음 구분 기준
		 *		- 자신보다 먼저 작업 위치(Prealign 위치)로 이동하는 TabMounter 가 이전 TabMounter 임.
		 *		- Gate 의 경우 TabCarrier Unload 위치에 따라 Front/Rear Group 의 TabMounter 출발 위치가 달라짐.
		 */
		if (DEF_MAX_TABMOUNTER-1 == i)
			sTabMounterData.m_iPreTabMounterNo = DEF_NONE_TABMOUNTER;
		else
			sTabMounterData.m_iPreTabMounterNo = i + 1;
		
		if (0 == i)
			sTabMounterData.m_iNextTabMounterNo = DEF_NONE_TABMOUNTER;
		else
			sTabMounterData.m_iNextTabMounterNo = i - 1;		

		m_pRTabMounter[i] = new MTabMounter(m_pObjSystemInfo->GetCommonData(DEF_TAB_MOUNTER_OBJECT_BASE + i + DEF_MAX_TABMOUNTER), sRefCompList, sTabMounterData);
		registerComponentInstance(DEF_TAB_MOUNTER_OBJECT_BASE + i + DEF_MAX_TABMOUNTER, m_pRTabMounter[i]);
	}

	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createRFIDComponents()
{
	int i = 0;
	SRFIDRefCompList	sRefCompList;
	SRFIDData			sRFIDData;

	// selsk_jdy 삭제 예정.
	//sRefCompList.m_plnkSerialPort	= m_pSerial[DEF_SERIAL_FRONT_RFID];
	sRFIDData.m_iHeadNo				= 1;

	for (int i = 0; i < DEF_MAX_RFID; i++)
	{
		if (DEF_RFID1 == i)
			sRefCompList.m_plnkSerialPort	= m_pSerial[DEF_SERIAL_FRONT_RFID];
		else // if (DEF_RFID2 == i)
			sRefCompList.m_plnkSerialPort	= m_pSerial[DEF_SERIAL_REAR_RFID];

		m_pRFID[i] = new MRFID(m_pObjSystemInfo->GetCommonData(DEF_RFID_OBJECT_BASE + i), sRefCompList, sRFIDData);
		registerComponentInstance(DEF_RFID_OBJECT_BASE + i, m_pRFID[i]);
	}
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createModelChangeComponents()
{
	SModelChangeRefCompList sRefCompList;
	SModelChangeData sModelChangeData;

	sRefCompList.m_plnkIO		= GetIOComponent();
//	sRefCompList.m_plnkESCyl	= m_pCylinder[DEF_CYL_MODEL_CHANGE_WIDTH_ADJUST_ES];
	sRefCompList.m_plnkESCyl	= NULL;
	sRefCompList.m_plnkUDCyl	= NULL;
	sRefCompList.m_plnkY		= m_pMotion[DEF_AXIS_SERCOS_MODEL_CHANGE_1];

#ifdef DEF_USE_MODELCHANGE_AIRRAIL_FB_CYL
	sRefCompList.m_plnkAirRailFB1					= m_pCylinder[DEF_CYL_MODELCHANGE_AIRRAIL_FB1];
	sRefCompList.m_plnkAirRailFB2					= m_pCylinder[DEF_CYL_MODELCHANGE_AIRRAIL_FB2];
#endif

	sModelChangeData.m_usOLockModelChangeRail		= NULL;	// PreBonder 에는 없음
	sModelChangeData.m_sPanelAlignerXYTTeachingInfo	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_STAGE_XYT);
	sModelChangeData.m_sPanelTransferXTeachingInfo	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_PANEL_TRANSFER_OUT_X);
	sModelChangeData.m_sYTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_MODEL_CHANGE_Y1);
	sModelChangeData.m_plnkSystemData				= GetSystemData();
	sModelChangeData.m_plnkPanelData				= GetPanelData();
	sModelChangeData.m_strPosFileName				= DEF_SYSTEM_POSITION_FILE_NAME;
	sModelChangeData.m_strOffsetPosFileName			= DEF_SYSTEM_OFFSET_FILE_NAME;

	m_pModelChange[DEF_FRONT_GROUP] = new MModelChange(m_pObjSystemInfo->GetCommonData(DEF_MODEL_CHANGE_OBJECT_BASE+DEF_FRONT_GROUP), sRefCompList, sModelChangeData);
	registerComponentInstance(DEF_MODEL_CHANGE_OBJECT_BASE+DEF_FRONT_GROUP, m_pModelChange[DEF_FRONT_GROUP]);


	sRefCompList.m_plnkIO		= GetIOComponent();
	sRefCompList.m_plnkESCyl	= NULL;
	sRefCompList.m_plnkUDCyl	= NULL;
	sRefCompList.m_plnkY		= m_pMotion[DEF_AXIS_SERCOS_MODEL_CHANGE_2];

	sModelChangeData.m_usOLockModelChangeRail		= NULL;	// PreBonder 에는 없음
	sModelChangeData.m_sPanelAlignerXYTTeachingInfo	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_STAGE_XYT);
	sModelChangeData.m_sPanelTransferXTeachingInfo	= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_PANEL_TRANSFER_OUT_X);
	sModelChangeData.m_sYTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_MODEL_CHANGE_Y2);
	sModelChangeData.m_plnkSystemData				= GetSystemData();
	sModelChangeData.m_plnkPanelData				= GetPanelData();
	sModelChangeData.m_strPosFileName				= DEF_SYSTEM_POSITION_FILE_NAME;
	sModelChangeData.m_strOffsetPosFileName			= DEF_SYSTEM_OFFSET_FILE_NAME;

	m_pModelChange[DEF_REAR_GROUP] = new MModelChange(m_pObjSystemInfo->GetCommonData(DEF_MODEL_CHANGE_OBJECT_BASE+DEF_REAR_GROUP), sRefCompList, sModelChangeData);
	registerComponentInstance(DEF_MODEL_CHANGE_OBJECT_BASE+DEF_REAR_GROUP, m_pModelChange[DEF_REAR_GROUP]);

	return ERR_SYSTEM_SUCCESS;
}

#ifdef DEF_USE_TURN_HANDLER
//SJ_YYK 161104 Add.
int	MPlatformOlbSystem::createTurnHandlerComponents()
{
	SHandlerRefCompList		sRefCompList;
	SHandlerData			sHandlerData;

	sRefCompList.m_plnkIO						= m_pIO;
	sRefCompList.m_plnkVac[0]					= m_pVacuum[DEF_VAC_TLHANDLER_1];
	sRefCompList.m_plnkVac[1]					= m_pVacuum[DEF_VAC_TLHANDLER_2];
	sRefCompList.m_plnkVac[2]					= m_pVacuum[DEF_VAC_TLHANDLER_3];

	sRefCompList.m_plnkXAxis					= NULL;
	sRefCompList.m_plnkYAxis					= NULL;	//@m_pMotion[DEF_AXIS_MMC_UNLOADHANDLER_Y];
	sRefCompList.m_plnkTAxis					= m_pMotion[DEF_AXIS_SERCOS_HANDLER_T];
	sRefCompList.m_plnkZAxis					= m_pMotion[DEF_AXIS_SERCOS_HANDLER_Z];
	sRefCompList.m_plnkMelsecIF					= m_pMelsecIF;

	sHandlerData.m_iHandlerType					= DEF_HANDLER_UNLOADING;
	sHandlerData.m_usOAddrIFMoveEnable			= OUT_INTERFACE_12;
	sHandlerData.m_plnkSystemData				= GetSystemData();
	sHandlerData.m_plnkPanelData				= GetPanelData();
	sHandlerData.m_strPosFileName				= DEF_SYSTEM_POSITION_FILE_NAME;
	sHandlerData.m_strOffsetPosFileName			= DEF_SYSTEM_OFFSET_FILE_NAME;	
	sHandlerData.m_sXYTTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_HANDLER_XT);
	sHandlerData.m_sZTeachingInfo				= m_pObjTeachingInfo->GetTeachingInfo(DEF_PREBONDER_UNIT_HANDLER_Z);

	m_pTurnHander = new MHandler(m_pObjSystemInfo->GetCommonData(DEF_HANDLER_OBJECT_BASE + DEF_HANDLER_UNLOADING), sRefCompList, sHandlerData);

	registerComponentInstance(DEF_HANDLER_OBJECT_BASE + DEF_HANDLER_UNLOADING, m_pTurnHander);


	return ERR_SYSTEM_SUCCESS;
}
#endif
int	MPlatformOlbSystem::createManageOpPanelComponents()
{
	m_pManageOpPanel = new MManageOpPanel(m_pObjSystemInfo->GetCommonData(DEF_CTR_MANAGE_OPPANEL_OBJECT_BASE), m_pOpPanel, m_pObjSystemData);
	registerComponentInstance(DEF_CTR_MANAGE_OPPANEL_OBJECT_BASE, m_pManageOpPanel);
	return	ERR_SYSTEM_SUCCESS;
}

int	MPlatformOlbSystem::createVisionCalibrationComponents()
{
	SVisionCalibrationRefCompList  sRefCompList;
	SVisionCalibrationData     datVisionCalibrationComponent;
	
	sRefCompList.m_plnkVision = GetVisionComponent();
	
	datVisionCalibrationComponent.m_strCalibrationFileName	= DEF_SYSTEM_VISION_CALIBRATION_DATA_FILE_NAME;
	datVisionCalibrationComponent.m_iUnitNumPerCamera		= DEF_VISION_CALIBRATION_UNIT_PER_CAM;// TabCarrier가 카메라 하나로 4개의 Unit을 인식한다. //DEF_VISION_CALIBRATION_DATA_NUM_PER_CAMERA_FOR_PRISM;
	
	m_pVisionCalibration = new MVisionCalibration(m_pObjSystemInfo->GetCommonData(DEF_CTR_VISION_CALIBRATION_OBJECT_BASE), sRefCompList, datVisionCalibrationComponent);
	registerComponentInstance(DEF_CTR_VISION_CALIBRATION_OBJECT_BASE, m_pVisionCalibration);
	return ERR_SYSTEM_SUCCESS;
}

int	MPlatformOlbSystem::createCtrlPanelAlignerComponents()
{
	SCtrlPanelAlignerRefCompList sRefCompList;
	SCtrlPanelAlignerData sCtrlPanelAlignerData;

	sRefCompList.m_plnkCtrlVisionCalibration	= GetVisionCalibrationComponent();
	sRefCompList.m_plnkVision					= GetVisionComponent();
	sRefCompList.m_plnkPanelAligner				= GetPanelAlignerComponent();

	for (int i = 0 ; i < DEF_MAX_TABMOUNTER ; i++)
	{
		sRefCompList.m_plnkStateTabMounter[i]	= GetTabMounterComponent(i);
		sRefCompList.m_plnkStateRTabMounter[i]	= GetRTabMounterComponent(i);
	}
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sRefCompList.m_plnkStateCameraCarrier[i] = GetCameraCarrierComponent(i);
		sRefCompList.m_plnkStateRCameraCarrier[i] = GetRCameraCarrierComponent(i);
	}

	sRefCompList.m_plnkCameraCarrier1			= GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
	sRefCompList.m_plnkCameraCarrier2			= GetRCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);

	sRefCompList.m_plnkModelChange1				= GetModelChangeComponent(DEF_FRONT_GROUP);
	sRefCompList.m_plnkModelChange2				= GetModelChangeComponent(DEF_REAR_GROUP);
	
	//171003 JSH.s
	for (int i = 0; i < DEF_MAX_LIGHTING; i++)
	{
		sRefCompList.m_plnkLighting[i] = GetLightingComponent(i);
	}
	//171003 JSH.e

	sCtrlPanelAlignerData.m_plnkPanelData		= GetPanelData();
	sCtrlPanelAlignerData.m_plnkPreBonderData	= GetPreBonderData();
	sCtrlPanelAlignerData.m_plnkSystemData		= GetSystemData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		sCtrlPanelAlignerData.m_plnkWorkSchedule[i]	= GetWorkSchedule(i);
#ifdef DEF_SOURCE_SYSTEM
		sCtrlPanelAlignerData.m_plnkWorkScheduleSecond[i] = GetWorkScheduleSecond(i);
#endif
	}

	//SJ_YYK 150318 Add..
	sCtrlPanelAlignerData.m_plnkAutoAlignData	= GetAutoAlignData();
	sCtrlPanelAlignerData.m_plnkTabOffset = GetTabOffsetData();
	sCtrlPanelAlignerData.m_plnkTabOffset_Rear = GetTabOffsetData_Rear();
	sCtrlPanelAlignerData.m_plnkTabData	= GetTabData();

	m_pCtrlPanelAligner = new MCtrlPanelAligner(m_pObjSystemInfo->GetCommonData(DEF_CTR_PANEL_ALIGNER_OBJECT_BASE), sRefCompList, sCtrlPanelAlignerData);
	registerComponentInstance(DEF_CTR_PANEL_ALIGNER_OBJECT_BASE, m_pCtrlPanelAligner);

	return ERR_SYSTEM_SUCCESS;
}

int	MPlatformOlbSystem::createCtrlTabFeederComponents()
{
	SCtrlTabFeederRefCompList sRefCompList;
	SCtrlTabFeederData sCtrlTabFeederData;

	sCtrlTabFeederData.m_plnkSystemData			= GetSystemData();
	sCtrlTabFeederData.m_plnkPreBonderData		= GetPreBonderData();
	sCtrlTabFeederData.m_plnkTabData			= GetTabData();
	sCtrlTabFeederData.m_plnkProductInfoData	= GetProductInfoData();
	sCtrlTabFeederData.m_plnkPanelData			= GetPanelData();
	sCtrlTabFeederData.m_plnkProcessData		= GetProcessData();

	sRefCompList.m_plnkLCNet					= GetLCNetComponent();
	sRefCompList.m_plnkTrsAutoManager           = GetTrsAutoManagerComponent();

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		sCtrlTabFeederData.m_plnkDBTabLoss		= GetDBTabLoss(i);

		sRefCompList.m_plnkTabFeeder			= GetTabFeederComponent(i);
//		sRefCompList.m_plnkTabCarrier			= GetTabCarrierComponent(i);
		sRefCompList.m_plnkRFID					= GetRFIDComponent(i);
		sRefCompList.m_plnkManageOpPanel		= GetManageOpPanelComponent();
		if ( i == DEF_TABFEEDER1)
		{
			for (int j = 0; j < DEF_MAX_TABCARRIER; j++)
				sRefCompList.m_plnkTabCarrier[j] = GetTabCarrierComponent(j);
		}
		else
		{
			for (int j = 0; j < DEF_MAX_TABCARRIER; j++)
				sRefCompList.m_plnkTabCarrier[j] = GetRTabCarrierComponent(j);
		}
		
		m_pCtrlTabFeeder[i] = new MCtrlTabFeeder(m_pObjSystemInfo->GetCommonData(DEF_CTR_TAB_FEEDER_OBJECT_BASE + i), sRefCompList, sCtrlTabFeederData);
		registerComponentInstance(DEF_CTR_TAB_FEEDER_OBJECT_BASE + i, m_pCtrlTabFeeder[i]);
	}
	
	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createCtrlTabCarrierComponents()
{
	SCtrlTabCarrierRefCompList sRefCompList;
	SCtrlTabCarrierData sCtrlTabCarrierData;
	
	sCtrlTabCarrierData.m_plnkSystemData	= GetSystemData();
	sCtrlTabCarrierData.m_plnkPreBonderData	= GetPreBonderData();
	sCtrlTabCarrierData.m_plnkTabOffsetData = GetTabOffsetData();
	sCtrlTabCarrierData.m_plnkTabOffsetData_Rear = GetTabOffsetData_Rear();

	sCtrlTabCarrierData.m_plnkDBTabLoss		= GetDBTabLoss(DEF_FRONT_GROUP);

	sRefCompList.m_plnkVision					= GetVisionComponent();
	sRefCompList.m_plnkMelsecAOCIF = GetMelsecComponent2(); //SJ_YYK 150908 Add...
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{		
		sRefCompList.m_plnkTabCarrier				= GetTabCarrierComponent(i);
//		sRefCompList.m_plnkTabFeeder				= GetTabFeederComponent(i);
		sRefCompList.m_plnkTabFeeder				= GetTabFeederComponent(DEF_TABFEEDER1);
		sRefCompList.m_plnkCtrlVisionCalibration	= GetVisionCalibrationComponent();

		//SJ_YYK 150817 Modify..
		if(i%2 == 0)
		{
			sCtrlTabCarrierData.m_iTabAlignCam1     = DEF_F_TABALIGNER_CAMERA_1;
			sCtrlTabCarrierData.m_iTabAlignCam2     = DEF_F_TABALIGNER_CAMERA_2;
		}
		else
		{
			sCtrlTabCarrierData.m_iTabAlignCam1     = DEF_F_TABALIGNER_CAMERA_3;
			sCtrlTabCarrierData.m_iTabAlignCam2     = DEF_F_TABALIGNER_CAMERA_4;
		}

		m_pCtrlTabCarrier[i] 
			= new MCtrlTabCarrier(m_pObjSystemInfo->GetCommonData(DEF_CTR_TAB_CARRIER_OBJECT_BASE + i), sRefCompList, sCtrlTabCarrierData);
		registerComponentInstance(DEF_CTR_TAB_CARRIER_OBJECT_BASE + i, m_pCtrlTabCarrier[i]);
	}

	return ERR_SYSTEM_SUCCESS;	
}

int MPlatformOlbSystem::createCtrlRTabCarrierComponents()
{
	int iInstanceNo = 0;
	SCtrlTabCarrierRefCompList sRefCompList;
	SCtrlTabCarrierData sCtrlTabCarrierData;

	sCtrlTabCarrierData.m_plnkSystemData	= GetSystemData();
	sCtrlTabCarrierData.m_plnkPreBonderData	= GetPreBonderData();
	sCtrlTabCarrierData.m_plnkTabOffsetData = GetTabOffsetData();
	sCtrlTabCarrierData.m_plnkTabOffsetData_Rear = GetTabOffsetData_Rear();

	sCtrlTabCarrierData.m_plnkDBTabLoss		= GetDBTabLoss(DEF_REAR_GROUP);

	sRefCompList.m_plnkVision					= GetVisionComponent();
	sRefCompList.m_plnkMelsecAOCIF			 = GetMelsecComponent2(); //SJ_YYK 150908 Add...
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{		
		sRefCompList.m_plnkTabCarrier				= GetRTabCarrierComponent(i);
		sRefCompList.m_plnkTabFeeder				= GetTabFeederComponent(DEF_TABFEEDER2);
		sRefCompList.m_plnkCtrlVisionCalibration	= GetVisionCalibrationComponent();

		if(i%2 == 0)
		{
			sCtrlTabCarrierData.m_iTabAlignCam1     = DEF_R_TABALIGNER_CAMERA_1;
			sCtrlTabCarrierData.m_iTabAlignCam2     = DEF_R_TABALIGNER_CAMERA_2;
		}
		else
		{
			sCtrlTabCarrierData.m_iTabAlignCam1     = DEF_R_TABALIGNER_CAMERA_3;
			sCtrlTabCarrierData.m_iTabAlignCam2     = DEF_R_TABALIGNER_CAMERA_4;
		}

//		iInstanceNo = DEF_MAX_TABCARRIER*2 - 1 - i;
		iInstanceNo = DEF_MAX_TABCARRIER + i;
		m_pCtrlRTabCarrier[i] 
			= new MCtrlTabCarrier(m_pObjSystemInfo->GetCommonData(DEF_CTR_TAB_CARRIER_OBJECT_BASE + iInstanceNo), sRefCompList, sCtrlTabCarrierData);
		registerComponentInstance(DEF_CTR_TAB_CARRIER_OBJECT_BASE + iInstanceNo, m_pCtrlRTabCarrier[i]);
	}

	return ERR_SYSTEM_SUCCESS;
}

int	MPlatformOlbSystem::createCtrlTabMounterComponents()
{
	int i = 0;
	SCtrlTabMounterRefCompList sRefCompList;
	SCtrlTabMounterData sCtrlTabMounterData;

	sCtrlTabMounterData.m_plnkSystemData = GetSystemData();
	sCtrlTabMounterData.m_plnkProcessData = GetProcessData();
//	sCtrlTabMounterData.m_plnkWorkSchedule = GetWorkSchedule();
	sCtrlTabMounterData.m_plnkTabOffsetData = GetTabOffsetData();
	//120208.kms____
	sCtrlTabMounterData.m_plnkTabOffsetData_Rear = GetTabOffsetData_Rear();
	//______________
	sCtrlTabMounterData.m_plnkPreBonderData	= GetPreBonderData();

	sRefCompList.m_plnkCtrlVisionCalibration = GetVisionCalibrationComponent();
	sRefCompList.m_plnkVision = GetVisionComponent();

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		sRefCompList.m_plnkTabCarrier[i] = GetTabCarrierComponent(i);
		sRefCompList.m_plnkRTabCarrier[i] = NULL;
	}
	sCtrlTabMounterData.m_plnkWorkSchedule = GetWorkSchedule(DEF_FRONT_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sCtrlTabMounterData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_FRONT_GROUP);
#endif
	sCtrlTabMounterData.m_plnkDBTabLoss	 = GetDBTabLoss(DEF_FRONT_GROUP);

	//Front________
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		sRefCompList.m_plnkTabMounter = GetTabMounterComponent(i);
//		sRefCompList.m_plnkTabCarrier = GetTabCarrierComponent(i);
//		sRefCompList.m_plnkRTabCarrier = NULL;
		m_pCtrlTabMounter[i] = new MCtrlTabMounter(m_pObjSystemInfo->GetCommonData(DEF_CTR_TAB_MOUNTER_OBJECT_BASE + i), sRefCompList, sCtrlTabMounterData);
		registerComponentInstance(DEF_CTR_TAB_MOUNTER_OBJECT_BASE + i, m_pCtrlTabMounter[i]);
	}

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		sRefCompList.m_plnkTabCarrier[i] = GetRTabCarrierComponent(i);
		sRefCompList.m_plnkRTabCarrier[i] = NULL;
	}
	sCtrlTabMounterData.m_plnkWorkSchedule = GetWorkSchedule(DEF_REAR_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sCtrlTabMounterData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_REAR_GROUP);
#endif
	sCtrlTabMounterData.m_plnkDBTabLoss	 = GetDBTabLoss(DEF_REAR_GROUP);
	//Rear_________
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		sRefCompList.m_plnkTabMounter = GetRTabMounterComponent(i);
//		sRefCompList.m_plnkTabCarrier = GetRTabCarrierComponent(i);
//		sRefCompList.m_plnkRTabCarrier = NULL;
		m_pCtrlRTabMounter[i] = new MCtrlTabMounter(m_pObjSystemInfo->GetCommonData(DEF_CTR_TAB_MOUNTER_OBJECT_BASE + i + DEF_MAX_TABMOUNTER), sRefCompList, sCtrlTabMounterData);
		registerComponentInstance(DEF_CTR_TAB_MOUNTER_OBJECT_BASE + i + DEF_MAX_TABMOUNTER, m_pCtrlRTabMounter[i]);
	}

	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createCtrlInspectionCameraComponents()
{
	int i = 0;
	SCtrlInspectionCameraRefCompList sRefCompList;
	SCtrlInspectionCameraData sCtrlInspectionCameraData;

	sRefCompList.m_plnkVision = GetVisionComponent();
	sRefCompList.m_plnkCtrlVisionCalibration = GetVisionCalibrationComponent();

	sCtrlInspectionCameraData.m_plnkSystemData = GetSystemData();
	sCtrlInspectionCameraData.m_plnkProcessData = GetProcessData();
	sCtrlInspectionCameraData.m_plnkPreBonderData = GetPreBonderData(); //20120216 SJ_HJG
	sCtrlInspectionCameraData.m_plnkTabData = GetTabData(); 

	for (int i = 0; i < DEF_MAX_LIGHTING; i++)
	{
		sRefCompList.m_plnkLighting[i] = GetLightingComponent(i);
	}

	sCtrlInspectionCameraData.m_plnkWorkSchedule = GetWorkSchedule(DEF_FRONT_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sCtrlInspectionCameraData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_FRONT_GROUP);
#endif
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sRefCompList.m_plnkCamCarrier = GetCameraCarrierComponent(i);
		sRefCompList.m_plnkTabMounter = GetTabMounterComponent(i);
		m_pCtrlInspectionCamera[i] = new MCtrlInspectionCamera(m_pObjSystemInfo->GetCommonData(DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE + i), sRefCompList, sCtrlInspectionCameraData);
		registerComponentInstance(DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE + i, m_pCtrlInspectionCamera[i]);
	}

	sCtrlInspectionCameraData.m_plnkWorkSchedule = GetWorkSchedule(DEF_REAR_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sCtrlInspectionCameraData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_REAR_GROUP);
#endif
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sRefCompList.m_plnkCamCarrier = GetRCameraCarrierComponent(i);
		sRefCompList.m_plnkTabMounter = GetRTabMounterComponent(i);
		m_pCtrlRInspectionCamera[i] = new MCtrlInspectionCamera(m_pObjSystemInfo->GetCommonData(DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE + i + DEF_MAX_INSPECTION_CAM_CARRIER), sRefCompList, sCtrlInspectionCameraData);
		registerComponentInstance(DEF_CTR_INSPECTION_CAMERA_OBJECT_BASE + i + DEF_MAX_INSPECTION_CAM_CARRIER, m_pCtrlRInspectionCamera[i]);
	}

	return ERR_SYSTEM_SUCCESS;	
}


#ifdef DEF_USE_TURN_HANDLER
//SJ_YYK 161114 Add..
int MPlatformOlbSystem::createCtrlTHandlerComponents()
{
	SCtrlHandlerRefCompList			sRefCompList;
	SCtrlHandlerData				sCtrlRefData;

	sCtrlRefData.m_plnkSystemData			= GetSystemData();
	sCtrlRefData.m_plnkPanelData			= GetPanelData();

	sRefCompList.m_plnkHandler				= GetTurnHandlerComponent();
	//sRefCompList.m_plnkWorkBench			= GetWorkBenchComponent();
	sRefCompList.m_plnkMelsecNet			= GetMelsecComponent();
	sRefCompList.m_plnkPanelTransferCenter	= GetPanelTransferCenterComponent();
	sRefCompList.m_plnkPanelTransfer		= GetPanelTransferComponent();

	m_pCtrlTHandler		= new MCtrlHandler(m_pObjSystemInfo->GetCommonData(DEF_CTR_HANDLER_OBJECT_BASE), sRefCompList, sCtrlRefData);
	registerComponentInstance(DEF_CTR_HANDLER_OBJECT_BASE, m_pCtrlTHandler);

	return ERR_SYSTEM_SUCCESS;
}
#endif

int	MPlatformOlbSystem::createTrsAutoManageComponents()
{
	STrsAutoManageRefCompList sRefCompList;
	STrsAutoManageData sTrsAutoManagerData;

	sRefCompList.m_plnkManageOpPanel	= GetManageOpPanelComponent();
	sRefCompList.m_plnkOpPanel			= GetOpPanelComponent();	
	sRefCompList.m_plnkMotion			= m_pMotion;
	sRefCompList.m_plnkSystemInfo		= GetSystemInfo();
	sRefCompList.m_plnkIO				= GetIOComponent();
	sRefCompList.m_plnkMelsecIF			= GetMelsecComponent();	
	sRefCompList.m_plnkMelsecAOCIF		= GetMelsecComponent2(); //SJ_YYK 150821 Add..
	sRefCompList.m_plnkLCNet			= GetLCNetComponent();
	sRefCompList.m_plnkPanelAligner		= GetPanelAlignerComponent();
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	sRefCompList.m_plnkPanelTransferCenter = GetPanelTransferCenterComponent();
#	endif 
	sRefCompList.m_plnkPanelTransfer	= GetPanelTransferComponent();
#endif
	sRefCompList.m_plnkPanelTransferOut = GetPanelTransferOutComponent();

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		sRefCompList.m_plnkTabFeeder[i] = GetTabFeederComponent(i);
		sRefCompList.m_plnkCtrlTabFeeder[i] = GetCtrlTabFeederComponent(i);
	}

	sTrsAutoManagerData.m_plnkSystemData = GetSystemData();
	sTrsAutoManagerData.m_plnkProcessData = GetProcessData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		sTrsAutoManagerData.m_plnkWorkSchedule[i] = GetWorkSchedule(i);
#ifdef DEF_SOURCE_SYSTEM
		sTrsAutoManagerData.m_plnkWorkScheduleSecond[i] = GetWorkScheduleSecond(i);
#endif
	}
	sTrsAutoManagerData.m_plnkProductInfoData = GetProductInfoData();
	sTrsAutoManagerData.m_plnkPanelData	= GetPanelData();

	for (int i = 0; i < DEF_MAX_INTERFACE_IO; i++)
	{
		sTrsAutoManagerData.m_rgusUpperIN[i]	= IN_INTERFACE_00 + i;
		sTrsAutoManagerData.m_rgusUpperOUT[i]	= OUT_INTERFACE_00 + i;
		sTrsAutoManagerData.m_rgusLowerIN[i]	= IN_INTERFACE_08 + i;
		sTrsAutoManagerData.m_rgusLowerOUT[i]	= OUT_INTERFACE_08 + i;
	}

	sTrsAutoManagerData.m_plnkAutoAlignData = GetAutoAlignData();//SJ_YYK 150318 Add..

	m_pTrsAutoManager = new MTrsAutoManager(m_pObjSystemInfo->GetCommonData(DEF_TRS_AUTOMANAGER_OBJECT_BASE), sRefCompList, sTrsAutoManagerData);
	registerComponentInstance(DEF_TRS_AUTOMANAGER_OBJECT_BASE, m_pTrsAutoManager);
	
	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createTrsJogComponents()
{
	m_pTrsJog = new MTrsJog(m_pObjSystemInfo->GetCommonData(DEF_TRS_JOG_OBJECT_BASE), m_pManageOpPanel);
	registerComponentInstance(DEF_TRS_JOG_OBJECT_BASE, m_pTrsJog);
	m_pTrsJog->AssignAutoManager(m_pTrsAutoManager);
	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createTrsPanelAlignerComponents()
{
	STrsPanelAlignerRefCompList sRefCompList;
	STrsPanelAlignerData sTrsPanelAlignerData;

	sRefCompList.m_plnkTrsAutoManager		= GetTrsAutoManagerComponent();
	sRefCompList.m_plnkCtrlPanelAligner		= GetCtrlPanelAlignerComponent();
	sRefCompList.m_plnkPanelAligner			= GetPanelAlignerComponent();
#ifdef DEF_SOURCE_SYSTEM
//@	sRefCompList.m_plnkStatePanelTransferCenter	= GetPanelTransferCenterComponent();
	sRefCompList.m_plnkStatePanelTransfer	= GetPanelTransferComponent();
#endif
	sRefCompList.m_plnkStatePanelTransferOut= GetPanelTransferOutComponent();
	sRefCompList.m_plnkIO					= GetIOComponent();
	sRefCompList.m_plnkMelsecIF				= GetMelsecComponent();
	sRefCompList.m_plnkStateTabMounter1		= GetTabMounterComponent(DEF_TABMOUNTER1);//@DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[DEF_FRONT_GROUP]);
	sRefCompList.m_plnkStateTabMounter2		= GetRTabMounterComponent(DEF_TABMOUNTER1);//@DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[DEF_REAR_GROUP]);
	sRefCompList.m_plnkModelChange1			= GetModelChangeComponent(DEF_FRONT_GROUP);
	sRefCompList.m_plnkModelChange2			= GetModelChangeComponent(DEF_REAR_GROUP);
	sRefCompList.m_plnkCameraCarrier1		= GetCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);
	sRefCompList.m_plnkCameraCarrier2		= GetRCameraCarrierComponent(DEF_INSPECTION_CARRIER_1);

	//SJ_YYK 110421 Add..Ellas
	sRefCompList.m_plnkLCNet				= GetLCNetComponent();

	sTrsPanelAlignerData.m_plnkSystemData		= GetSystemData();
	sTrsPanelAlignerData.m_plnkPanelData		= GetPanelData();
	sTrsPanelAlignerData.m_plnkPrebonderData	= GetPreBonderData();
	sTrsPanelAlignerData.m_plnkProductInfoData	= GetProductInfoData();
	sTrsPanelAlignerData.m_plnkProcessData		= GetProcessData();
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		sTrsPanelAlignerData.m_plnkWorkSchedule[i] = GetWorkSchedule(i);
#ifdef DEF_SOURCE_SYSTEM
		sTrsPanelAlignerData.m_plnkWorkScheduleSecond[i] = GetWorkScheduleSecond(i);
#endif
	}

	m_pTrsPanelAligner = new MTrsPanelAligner(m_pObjSystemInfo->GetCommonData(DEF_TRS_PANEL_ALIGNER_OBJECT_BASE), 
								sRefCompList, 
								sTrsPanelAlignerData);
	registerComponentInstance(DEF_TRS_PANEL_ALIGNER_OBJECT_BASE, m_pTrsPanelAligner);
	return ERR_SYSTEM_SUCCESS;	
}
#ifdef DEF_USE_TRANSFER_CENTER
int	MPlatformOlbSystem::createTrsPanelTransferCenterComponents()
{
	
#ifdef DEF_SOURCE_SYSTEM
	STrsPanelTransferCenterRefCompList sRefCompList;
	STrsPanelTransferCenterData sTrsPanelTransferData;

	sRefCompList.m_plnkTrsAutoManager		= GetTrsAutoManagerComponent();
	sRefCompList.m_plnkPanelTransferCenter	= GetPanelTransferCenterComponent();
	sRefCompList.m_plnkPanelAligner			= GetPanelAlignerComponent(); //170205 SJ_YSH
	sRefCompList.m_plnkStatePanelTransfer	= GetPanelTransferComponent();
#	ifdef DEF_USE_TURN_HANDLER
	sRefCompList.m_plnkStateTurnHandler		= GetTurnHandlerComponent(); //SJ_YYK 161104 Add.
#	else
	sRefCompList.m_plnkStateTurnHandler		= NULL;
#	endif
	sRefCompList.m_plnkMelsecIF				= GetMelsecComponent();
	//SJ_YYK 110421 Add.. Ellas...
	sRefCompList.m_plnkLCNet				= GetLCNetComponent();

	sTrsPanelTransferData.m_plnkSystemData	= GetSystemData();
	sTrsPanelTransferData.m_plnkPanelData	= GetPanelData();
	
	m_pTrsPanelTransferCenter = new MTrsPanelTransferCenter(m_pObjSystemInfo->GetCommonData(DEF_TRS_PANEL_TRANSFER_OBJECT_BASE+DEF_PANEL_TRANSFER_CENTER), 
								sRefCompList, 
								sTrsPanelTransferData);
	registerComponentInstance(DEF_TRS_PANEL_TRANSFER_OBJECT_BASE+DEF_PANEL_TRANSFER_CENTER, m_pTrsPanelTransferCenter);
#endif
	
	return ERR_SYSTEM_SUCCESS;	
}
#endif

int	MPlatformOlbSystem::createTrsPanelTransferComponents()
{
#ifdef DEF_SOURCE_SYSTEM
	STrsPanelTransferRefCompList sRefCompList;
	STrsPanelTransferData sTrsPanelTransferData;

	sRefCompList.m_plnkTrsAutoManager		= GetTrsAutoManagerComponent();
	sRefCompList.m_plnkPanelTransfer		= GetPanelTransferComponent();
	sRefCompList.m_plnkStatePanelAligner	= GetPanelAlignerComponent();
	sRefCompList.m_plnkMelsecIF				= GetMelsecComponent();
	sRefCompList.m_plnkLCNet				= GetLCNetComponent();

	sTrsPanelTransferData.m_plnkSystemData	= GetSystemData();
	sTrsPanelTransferData.m_plnkPanelData	= GetPanelData();
	sTrsPanelTransferData.m_uiTransferType	= DEF_PANEL_TRANSFER_IN;

	m_pTrsPanelTransfer = new MTrsPanelTransfer(m_pObjSystemInfo->GetCommonData(DEF_TRS_PANEL_TRANSFER_OBJECT_BASE+DEF_PANEL_TRANSFER_IN), 
										sRefCompList, 
										sTrsPanelTransferData);
	registerComponentInstance(DEF_TRS_PANEL_TRANSFER_OBJECT_BASE+DEF_PANEL_TRANSFER_IN, m_pTrsPanelTransfer);
#endif
	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createTrsPanelTransferOutComponents()
{
	//STrsPanelTransferRefCompList sRefCompList;
	tagSTrsPanelTransferOutRefCompList sRefCompList;
	tagSTrsPanelTransferOutData	sTrsPanelTransferData;
	//STrsPanelTransferData sTrsPanelTransferData;
	
	sRefCompList.m_plnkTrsAutoManager		= GetTrsAutoManagerComponent();
	sRefCompList.m_plnkPanelTransferOut		= GetPanelTransferOutComponent();
	sRefCompList.m_plnkStatePanelAligner	= GetPanelAlignerComponent();
	sRefCompList.m_plnkMelsecIF				= GetMelsecComponent();
	sRefCompList.m_plnkPanelTransferIn      = GetPanelTransferComponent();
	
	sTrsPanelTransferData.m_plnkSystemData	= GetSystemData();
	//sTrsPanelTransferData.m_plnkPanelData	= GetPanelData();
	//sTrsPanelTransferData.m_uiTransferType	= DEF_PANEL_TRANSFER_OUT;		
	m_pTrsPanelTransferOut = new MTrsPanelTransferOut(m_pObjSystemInfo->GetCommonData(DEF_TRS_PANEL_TRANSFER_OBJECT_BASE+DEF_PANEL_TRANSFER_OUT), 
										sRefCompList, 
										sTrsPanelTransferData);	
	registerComponentInstance(DEF_TRS_PANEL_TRANSFER_OBJECT_BASE+DEF_PANEL_TRANSFER_OUT, m_pTrsPanelTransferOut);
	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createTrsTabCarrierComponents()
{
	STrsTabCarrierRefCompList sRefCompList;
	STrsTabCarrierData sTrsTabCarrierData;


	sTrsTabCarrierData.m_plnkSystemData	= GetSystemData();
	sTrsTabCarrierData.m_plnkProcessData = GetProcessData();
	sTrsTabCarrierData.m_plnkPreBonderData = GetPreBonderData();//2009.04.04 SESL_CYJ
	sTrsTabCarrierData.m_plnkWorkSchedule = GetWorkSchedule(DEF_FRONT_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sTrsTabCarrierData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_FRONT_GROUP);
#endif
	sTrsTabCarrierData.m_plnkDBTabLoss	= GetDBTabLoss(DEF_FRONT_GROUP);

#ifdef DEF_SOURCE_SYSTEM
	//SJ_YYK 150907 Add...
#	ifndef DEF_MIRROR_
	sTrsTabCarrierData.m_eWorkSide = WORKING_SIDE_X_DOWN;
#	else
	sTrsTabCarrierData.m_eWorkSide = WORKING_SIDE_X_UP;
#	endif
#else
#	ifndef DEF_MIRROR_
		sTrsTabCarrierData.m_eWorkSide = WORKING_SIDE_Y_LEFT;
#	else
		sTrsTabCarrierData.m_eWorkSide = WORKING_SIDE_Y_RIGHT;
#	endif
#endif

	sRefCompList.m_plnkTrsAutoManager = GetTrsAutoManagerComponent();
	sRefCompList.m_plnkPanelAligner = GetPanelAlignerComponent();
	//jskim Melsec 추가..
	sRefCompList.m_plnkMelsecIF2 = GetMelsecComponent2();

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		sRefCompList.m_plnkCtrlTabCarrier = GetCtrlTabCarrierComponent(i);
		sRefCompList.m_plnkTabCarrier	  = GetTabCarrierComponent(i);
		m_pTrsTabCarrier[i] = new MTrsTabCarrier(m_pObjSystemInfo->GetCommonData(DEF_TRS_TABCARRIER_OBJECT_BASE + i), sRefCompList, sTrsTabCarrierData);
		registerComponentInstance(DEF_TRS_TABCARRIER_OBJECT_BASE + i, m_pTrsTabCarrier[i]);
	}

	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createTrsRTabCarrierComponents()
{
	int iInstanceNo = 0;
	STrsTabCarrierRefCompList sRefCompList;
	STrsTabCarrierData sTrsTabCarrierData;

	sTrsTabCarrierData.m_plnkSystemData	= GetSystemData();
	sTrsTabCarrierData.m_plnkProcessData = GetProcessData();
	sTrsTabCarrierData.m_plnkPreBonderData = GetPreBonderData();//2009.04.04 SESL_CYJ
//	sTrsTabCarrierData.m_plnkWorkSchedule = GetWorkSchedule();
	sTrsTabCarrierData.m_plnkWorkSchedule = GetWorkSchedule(DEF_REAR_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sTrsTabCarrierData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_REAR_GROUP);
#endif
	sTrsTabCarrierData.m_plnkDBTabLoss	= GetDBTabLoss(DEF_REAR_GROUP);

#ifdef DEF_SOURCE_SYSTEM	
	//SJ_YYK 150907 Add...
#	ifndef DEF_MIRROR_
	sTrsTabCarrierData.m_eWorkSide = WORKING_SIDE_X_UP;
#	else
	sTrsTabCarrierData.m_eWorkSide = WORKING_SIDE_X_DOWN;
#endif
#else
#	ifndef DEF_MIRROR_
		sTrsTabCarrierData.m_eWorkSide = WORKING_SIDE_Y_RIGHT;
#	else
		sTrsTabCarrierData.m_eWorkSide = WORKING_SIDE_Y_LEFT;
#	endif
#endif

	sRefCompList.m_plnkTrsAutoManager = GetTrsAutoManagerComponent();
	sRefCompList.m_plnkPanelAligner = GetPanelAlignerComponent();
	sRefCompList.m_plnkMelsecIF2 = GetMelsecComponent2();

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		sRefCompList.m_plnkCtrlTabCarrier = GetCtrlRTabCarrierComponent(i);
		sRefCompList.m_plnkTabCarrier = GetRTabCarrierComponent(i);

		iInstanceNo = i + DEF_MAX_TABCARRIER;//DEF_MAX_TABCARRIER*2 - 1 - i;
		m_pTrsRTabCarrier[i] = new MTrsTabCarrier(m_pObjSystemInfo->GetCommonData(DEF_TRS_TABCARRIER_OBJECT_BASE + iInstanceNo), sRefCompList, sTrsTabCarrierData);
		registerComponentInstance(DEF_TRS_TABCARRIER_OBJECT_BASE + iInstanceNo, m_pTrsRTabCarrier[i]);
	}

	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createTrsTabFeederComponents()
{
	STrsTabFeederRefCompList sRefCompList;
	STrsTabFeederData sTrsTabFeederData;

	sTrsTabFeederData.m_plnkSystemData = GetSystemData();
	sTrsTabFeederData.m_plnkPanelData = GetPanelData();
	sTrsTabFeederData.m_plnkProcessData	= GetProcessData();
	sTrsTabFeederData.m_plnkPreBonderData = GetPreBonderData();
	sTrsTabFeederData.m_plnkProductInfoData	= GetProductInfoData();

	sRefCompList.m_plnkTrsAutoManager	= GetTrsAutoManagerComponent();
	sRefCompList.m_plnkLCNet			= GetLCNetComponent();

	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		if (i == 0)
		{
			sTrsTabFeederData.m_plnkWorkSchedule = GetWorkSchedule(DEF_FRONT_GROUP);
#ifdef DEF_SOURCE_SYSTEM
			sTrsTabFeederData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_FRONT_GROUP);
#endif
		}
		else
		{
			sTrsTabFeederData.m_plnkWorkSchedule = GetWorkSchedule(DEF_REAR_GROUP);
#ifdef DEF_SOURCE_SYSTEM
			sTrsTabFeederData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_REAR_GROUP);
#endif
		}

		sRefCompList.m_plnkCtrlTabFeeder	= GetCtrlTabFeederComponent(i);
		sRefCompList.m_plnkTabFeeder		= GetTabFeederComponent(i);
		if (i == DEF_TABFEEDER1)
			sRefCompList.m_plnkCtrlOtherTabFeeder = GetCtrlTabFeederComponent(DEF_TABFEEDER2);
		else
			sRefCompList.m_plnkCtrlOtherTabFeeder = GetCtrlTabFeederComponent(DEF_TABFEEDER1);
		m_pTrsTabFeeder[i] = new MTrsTabFeeder(m_pObjSystemInfo->GetCommonData(DEF_TRS_TABFEEDER_OBJECT_BASE + i), sRefCompList, sTrsTabFeederData);
		registerComponentInstance(DEF_TRS_TABFEEDER_OBJECT_BASE + i, m_pTrsTabFeeder[i]);
	}
	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createTrsTabMounterComponents()
{
	int i = 0;
	STrsTabMounterRefCompList sRefCompList;
	STrsTabMounterData sTrsTabMounterData;

	sRefCompList.m_plnkTrsAutoManager		= GetTrsAutoManagerComponent();	
	sRefCompList.m_plnkMelsecIF				= GetMelsecComponent();

	sTrsTabMounterData.m_plnkSystemData		= GetSystemData();
	sTrsTabMounterData.m_plnkProcessData	= GetProcessData();

	sTrsTabMounterData.m_plnkWorkSchedule	= GetWorkSchedule(DEF_FRONT_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sTrsTabMounterData.m_plnkWorkScheduleSecond	= GetWorkScheduleSecond(DEF_FRONT_GROUP);
#endif
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		sRefCompList.m_plnkCtrlTabMounter	= GetCtrlTabMounterComponent(i);
		sRefCompList.m_plnkTabMounter		= GetTabMounterComponent(i);
		sRefCompList.m_plnkCameraCarrier	= GetCameraCarrierComponent(i);

		m_pTrsTabMounter[i] = new MTrsTabMounter(m_pObjSystemInfo->GetCommonData(DEF_TRS_TAB_MOUNTER_OBJECT_BASE + i), sRefCompList, sTrsTabMounterData);
		registerComponentInstance(DEF_TRS_TAB_MOUNTER_OBJECT_BASE + i, m_pTrsTabMounter[i]);
	}


	sTrsTabMounterData.m_plnkWorkSchedule	= GetWorkSchedule(DEF_REAR_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sTrsTabMounterData.m_plnkWorkScheduleSecond	= GetWorkScheduleSecond(DEF_REAR_GROUP);
#endif
	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		sRefCompList.m_plnkCtrlTabMounter	= GetCtrlRTabMounterComponent(i);
		sRefCompList.m_plnkTabMounter		= GetRTabMounterComponent(i);
		sRefCompList.m_plnkCameraCarrier	= GetRCameraCarrierComponent(i);

		m_pTrsRTabMounter[i] = new MTrsTabMounter(m_pObjSystemInfo->GetCommonData(DEF_TRS_TAB_MOUNTER_OBJECT_BASE + i + DEF_MAX_TABMOUNTER), sRefCompList, sTrsTabMounterData);
		registerComponentInstance(DEF_TRS_TAB_MOUNTER_OBJECT_BASE + i + DEF_MAX_TABMOUNTER, m_pTrsRTabMounter[i]);
	}
	return ERR_SYSTEM_SUCCESS;	
}

int	MPlatformOlbSystem::createTrsInspectionCameraComponents()
{
	int i = 0;
	STrsInspectionCameraRefCompList sRefCompList;
	STrsInspectionCameraData sTrsInspectionCameraData;

	sTrsInspectionCameraData.m_plnkSystemData = GetSystemData();
	sTrsInspectionCameraData.m_plnkPrebonderData = GetPreBonderData();
	sTrsInspectionCameraData.m_plnkProcessData = GetProcessData();

	sRefCompList.m_plnkTrsAutoManager = GetTrsAutoManagerComponent();
	sTrsInspectionCameraData.m_plnkWorkSchedule = GetWorkSchedule(DEF_FRONT_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sTrsInspectionCameraData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_FRONT_GROUP);
#endif
	for (int i = 0; i<DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
		sRefCompList.m_rgplnkStateCamCarrier[i] = GetCameraCarrierComponent(i);
	for (int i = 0; i<DEF_MAX_WORKER_PER_GROUP; i++)
		sRefCompList.m_plnkTabMounter[i] = GetTabMounterComponent(i);

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sRefCompList.m_plnkCtrlInspectCam = GetCtrlInspectionCameraComponent(i);
		sRefCompList.m_plnkCamCarrier = GetCameraCarrierComponent(i);

		m_pTrsInspectionCamera[i] = new MTrsInspectionCamera(m_pObjSystemInfo->GetCommonData(DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE + i), sRefCompList, sTrsInspectionCameraData);
		registerComponentInstance(DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE + i, m_pTrsInspectionCamera[i]);
	}


	sTrsInspectionCameraData.m_plnkWorkSchedule = GetWorkSchedule(DEF_REAR_GROUP);
#ifdef DEF_SOURCE_SYSTEM
	sTrsInspectionCameraData.m_plnkWorkScheduleSecond = GetWorkScheduleSecond(DEF_REAR_GROUP);
#endif
	for (int i = 0; i<DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
		sRefCompList.m_rgplnkStateCamCarrier[i] = GetRCameraCarrierComponent(i);
	for (int i = 0; i<DEF_MAX_WORKER_PER_GROUP; i++)
		sRefCompList.m_plnkTabMounter[i] = GetRTabMounterComponent(i);

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		sRefCompList.m_plnkCtrlInspectCam = GetCtrlRInspectionCameraComponent(i);
		sRefCompList.m_plnkCamCarrier = GetRCameraCarrierComponent(i);

		m_pTrsRInspectionCamera[i] = new MTrsInspectionCamera(m_pObjSystemInfo->GetCommonData(DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE + i + DEF_MAX_INSPECTION_CAM_CARRIER), sRefCompList, sTrsInspectionCameraData);
		registerComponentInstance(DEF_TRS_INSPECTIONCAMERA_OBJECT_BASE + i + DEF_MAX_INSPECTION_CAM_CARRIER, m_pTrsInspectionCamera[i]);
	}

	return ERR_SYSTEM_SUCCESS;	
}

#ifdef DEF_USE_TURN_HANDLER
//SJ_YYK 161104 Add..
int MPlatformOlbSystem::createTrsTHandlerComponents()
{
	int i = 0;
	STrsTHandlerRefCompList				sRefCompList;
	STrsTHandlerData					sRefDataList;

	sRefDataList.m_plnkSystemData			= GetSystemData();
	sRefDataList.m_plnkPanelData			= GetPanelData();
	sRefDataList.m_plnkProductInfoData		= GetProductInfoData();
		
	sRefCompList.m_plnkCtrlHandler			= GetCtrlTHandlerComponent();
	sRefCompList.m_plnkHandler				= GetTurnHandlerComponent();
	sRefCompList.m_plnkMelsecIF				= GetMelsecComponent();
	sRefCompList.m_plnkTrsAutoManager		= GetTrsAutoManagerComponent();
	sRefCompList.m_plnkPanelTransferCenter	= GetPanelTransferCenterComponent();
	sRefCompList.m_plnkPanelTransferIn		= GetPanelTransferComponent();
	//sRefCompList.m_plnkWorkBench			= GetWorkBenchComponent();

	m_pTrsTHandler = new MTrsTHandler(m_pObjSystemInfo->GetCommonData(DEF_TRS_HANDLER_OBJECT_BASE), sRefCompList, sRefDataList);
	registerComponentInstance(DEF_TRS_HANDLER_OBJECT_BASE, m_pTrsTHandler);

	return ERR_SYSTEM_SUCCESS;
}
#endif
/*/
#if FALSE
int MPlatformOlbSystem::createTrsLoadConveyorComponents()
{
	STrsLoadConveyorRefCompList sRefCompList;
	STrsLoadConveyorData sTrsLoadConveyorData;

	sRefCompList.m_plnkTrsAutoManager		= GetTrsAutoManagerComponent();
	sRefCompList.m_plnkLoadConveyor			= GetRollerConveyorComponent(DEF_LOAD_CONVEYOR);
	sRefCompList.m_plnkStatePanelTransfer	= GetPanelTransferComponent();
	sRefCompList.m_plnkMelsecIF				= GetMelsecComponent();
	sRefCompList.m_plnkLCNet				= GetLCNetComponent();

	sTrsLoadConveyorData.m_plnkSystemData	= GetSystemData();

	m_pTrsLoadConveyor = new MTrsLoadConveyor(m_pObjSystemInfo->GetCommonData(DEF_TRS_LOAD_CONVEYOR_OBJECT_BASE), sRefCompList, sTrsLoadConveyorData);
	registerComponentInstance(DEF_TRS_LOAD_CONVEYOR_OBJECT_BASE, m_pTrsLoadConveyor);
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::createTrsUnloadConveyorComponents()
{
	STrsUnloadConveyorRefCompList sRefCompList;
	STrsUnloadConveyorData sTrsUnloadConveyorData;

	sRefCompList.m_plnkTrsAutoManager		= GetTrsAutoManagerComponent();
	sRefCompList.m_plnkUnloadConveyor		= GetRollerConveyorComponent(DEF_UNLOAD_CONVEYOR);
	sRefCompList.m_plnkStatePanelAligner	= GetPanelAlignerComponent();
	sRefCompList.m_plnkMelsecIF				= GetMelsecComponent();

	sTrsUnloadConveyorData.m_plnkSystemData	= GetSystemData();

	m_pTrsUnloadConveyor = new MTrsUnloadConveyor(m_pObjSystemInfo->GetCommonData(DEF_TRS_UNLOAD_CONVEYOR_OBJECT_BASE), sRefCompList, sTrsUnloadConveyorData);
	registerComponentInstance(DEF_TRS_UNLOAD_CONVEYOR_OBJECT_BASE, m_pTrsUnloadConveyor);
	return ERR_SYSTEM_SUCCESS;
}
#endif
/*/

void MPlatformOlbSystem::startThreads()
{
	int i = 0;
	m_pTrsAutoManager->ThreadRun();
	m_pTrsJog->ThreadRun();
	m_pTrsPanelAligner->ThreadRun();
#ifdef DEF_SOURCE_SYSTEM
#	ifdef DEF_USE_TRANSFER_CENTER
	m_pTrsPanelTransferCenter->ThreadRun();
#	endif
	m_pTrsPanelTransfer->ThreadRun();
#endif
	m_pTrsPanelTransferOut->ThreadRun();

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_pTrsTabCarrier[i]->ThreadRun();
		m_pTrsRTabCarrier[i]->ThreadRun();
	}
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
		m_pTrsTabFeeder[i]->ThreadRun();

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		m_pTrsTabMounter[i]->ThreadRun();
		m_pTrsRTabMounter[i]->ThreadRun();
	}

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		m_pTrsInspectionCamera[i]->ThreadRun();
		m_pTrsRInspectionCamera[i]->ThreadRun();
	}
#ifdef DEF_USE_TURN_HANDLER
	m_pTrsTHandler->ThreadRun(); //SJ_YYK 161104 Add.
#endif
}

void MPlatformOlbSystem::initSystem()
{
	int i = 0;
	m_pVision->DeleteOldImageFiles(_T("..\\LogImage"));

	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		m_pTabCarrier[i]->Down(TRUE);
		m_pRTabCarrier[i]->Down(TRUE);
	}
	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
		m_pTabMounter[i]->UpPusherCyl(TRUE);
		m_pRTabMounter[i]->UpPusherCyl(TRUE);
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// 조명 값 Setting => On.
	SetLightValue(DEF_MAX_CAMERA_NO);
	LightDefult(DEF_MAX_CAMERA_NO); //171004 JSH
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	// 설비간 Interface 초기화
	m_pMelsecIF->SetLowerEqBit(eSendAble, FALSE);
	m_pMelsecIF->SetLowerEqBit(eSendStart, FALSE);
	m_pMelsecIF->SetLowerEqBit(eSendComplete, FALSE);
	m_pMelsecIF->SetLowerEqBit(eGlassCheckSensorOn, FALSE);
	m_pMelsecIF->SetLowerEqBit(eArm1FoldComplete, FALSE);
	m_pMelsecIF->SetLowerEqBit(eImmediatelyPauseRequest, FALSE);
	m_pMelsecIF->SetUpperEqBit(eReceiveAble, FALSE);
	m_pMelsecIF->SetUpperEqBit(eReceiveStart, FALSE);
	m_pMelsecIF->SetUpperEqBit(eReceiveComplete, FALSE);
	m_pMelsecIF->SetUpperEqBit(eGlassCheckSensorOn, FALSE);
	m_pMelsecIF->SetUpperEqBit(eArm1FoldComplete, FALSE);
	m_pMelsecIF->SetUpperEqBit(eImmediatelyPauseRequest, FALSE);

	//110620.KKY.Add_____
	m_pLCNet->m_CurProductPlan.SetData(&m_pObjProductInfoData->m_CurProductBatchInfo);
	for (int i=0; i<DEF_MAX_PRODUCT_PLAN_CNT; i++)
	{
		if (m_pObjProductInfoData->m_NextProductBatchInfo[i].m_sBatchID != _T(""))
		{
			MProductPlanBatchData* p = new MProductPlanBatchData;
			p->SetData(&m_pObjProductInfoData->m_NextProductBatchInfo[i]);
			m_pLCNet->m_caNextProductPlan.Add(p);
		}
		if (m_pObjProductInfoData->m_EndProductBatchInfo[i].m_sBatchID != _T(""))
		{
			MProductPlanBatchData* p = new MProductPlanBatchData;
			p->SetData(&m_pObjProductInfoData->m_EndProductBatchInfo[i]);
			m_pLCNet->m_caEndProductPlan.Add(p);
		}
	}

	int iNextCnt = m_pLCNet->m_caNextProductPlan.GetSize();
	if (iNextCnt > 0)
		m_pLCNet->m_NextProductPlan.SetData((MProductPlanBatchData*)m_pLCNet->m_caNextProductPlan.GetAt(0));

	int iEndCnt = m_pLCNet->m_caEndProductPlan.GetSize();
	if (iEndCnt > 0)
		m_pLCNet->m_EndProductPlan.SetData((MProductPlanBatchData*)m_pLCNet->m_caEndProductPlan.GetAt(iEndCnt-1));

	m_pLCNet->m_CurMaterialCodeInfo_S.CopyData(&m_pObjProductInfoData->m_MaterialCodeInfo_S[DEF_CUR_PRODUCT_PLAN]);
	m_pLCNet->m_CurMaterialCodeInfo_G.CopyData(&m_pObjProductInfoData->m_MaterialCodeInfo_G[DEF_CUR_PRODUCT_PLAN]);

	m_pLCNet->m_NextMaterialCodeInfo_S.CopyData(&m_pObjProductInfoData->m_MaterialCodeInfo_S[DEF_NEXT_PRODUCT_PLAN]);
	m_pLCNet->m_NextMaterialCodeInfo_G.CopyData(&m_pObjProductInfoData->m_MaterialCodeInfo_G[DEF_NEXT_PRODUCT_PLAN]);

	m_pLCNet->m_EndMaterialCodeInfo_S.CopyData(&m_pObjProductInfoData->m_MaterialCodeInfo_S[DEF_END_PRODUCT_PLAN]);
	m_pLCNet->m_EndMaterialCodeInfo_G.CopyData(&m_pObjProductInfoData->m_MaterialCodeInfo_G[DEF_END_PRODUCT_PLAN]);
	//___________________
}

MSystemData* MPlatformOlbSystem::GetSystemData()
{
	return m_pObjSystemData;
}

MSystemInfo* MPlatformOlbSystem::GetSystemInfo()
{
	return m_pObjSystemInfo;
}

MPanelData* MPlatformOlbSystem::GetPanelData()
{
	return m_pObjPanelData;
}

//20110223
MTowerLampData* MPlatformOlbSystem::GetTowerLampData()
{
	return m_pObjTowerLampData;
}
//___________

MPreBonderData*	MPlatformOlbSystem::GetPreBonderData()
{
	return m_pObjPreBonderData;
}

MTabData* MPlatformOlbSystem::GetTabData()
{
	return m_pObjTabData;
}

MTabOffsetData*	MPlatformOlbSystem::GetTabOffsetData()
{
	return m_pObjTabOffsetData;
}

MTabOffsetData*	MPlatformOlbSystem::GetTabOffsetData_Rear()
{
	return m_pObjTabOffsetData_Rear;
}

MProductInfoData* MPlatformOlbSystem::GetProductInfoData()
{
	return m_pObjProductInfoData;
}

MProcessData* MPlatformOlbSystem::GetProcessData()
{
	return m_pObjProcessData;
}

//SJ_YYK 150318 Add..
MAutoAlignData* MPlatformOlbSystem::GetAutoAlignData()
{
	return m_pObjAutoAlignData;
}
//______________________________*/

MWorkSchedule* MPlatformOlbSystem::GetWorkSchedule(int iWorkGroup)
{
	ASSERT(iWorkGroup < DEF_MAX_WORKER_GROUP);
	return m_pObjWorkSchedule[iWorkGroup];
}

MWorkSchedule* MPlatformOlbSystem::GetWorkScheduleSecond(int iWorkGroup)
{
#ifdef DEF_GATE_SYSTEM
	ASSERT(0);
#endif
	ASSERT(iWorkGroup < DEF_MAX_WORKER_GROUP);
	return m_pObjWorkScheduleSecond[iWorkGroup];
}

CDBTabLoss* MPlatformOlbSystem::GetDBTabLoss(int iWorkGroup)
{
	ASSERT(iWorkGroup < DEF_MAX_WORKER_GROUP);
	return m_pObjDBTabLoss[iWorkGroup];
}

IIO* MPlatformOlbSystem::GetIOComponent()
{
	return m_pIO;
}

IVision* MPlatformOlbSystem::GetVisionComponent()
{
	return m_pVision;
}

IComThread* MPlatformOlbSystem::GetSerialComponent(int iSerialIndex)
{
	return m_pSerial[iSerialIndex];
}

ICylinder* MPlatformOlbSystem::GetCylinderComponent(int iCylinderIndex)
{
	return m_pCylinder[iCylinderIndex];
}

IVacuum* MPlatformOlbSystem::GetVacuumComponent(int iIndex)
{
	return m_pVacuum[iIndex];
}

IAxis* MPlatformOlbSystem::GetMotionComponent(int iIndex)
{
	return m_pMotion[iIndex];
}

ILighting* MPlatformOlbSystem::GetLightingComponent(int iIndex)
{
	return m_pLighting[iIndex];
}

IInverter* MPlatformOlbSystem::GetInverterComponent(int iIndex)
{
	return m_pInverter[iIndex];
}

ISercosMultiAxis* MPlatformOlbSystem::GetSercosMultiMotionComponent(int iIndex)
{
	return m_pMultiSercosMotion[iIndex];
}

ISercosSetup* MPlatformOlbSystem::GetSercosSetupComponent()
{
	return m_pSercosSetup;
}

MCANSetup* MPlatformOlbSystem::GetCANSetupComponent()
{
	return m_pCANSetup;
}

MNetH* MPlatformOlbSystem::GetMelsecComponent()
{
	return m_pMelsecIF;
}

MNetH* MPlatformOlbSystem::GetMelsecComponent2()
{
	return m_pMelsecIF2;
}

IOpPanel* MPlatformOlbSystem::GetOpPanelComponent()
{
	return m_pOpPanel;
}

MPanelAligner* MPlatformOlbSystem::GetPanelAlignerComponent()
{
	return m_pPanelAligner;
}
#ifdef DEF_USE_TRANSFER_CENTER
MPanelTransfer* MPlatformOlbSystem::GetPanelTransferCenterComponent()
{
	return m_pPanelTransferCenter;
}
#endif

MPanelTransfer* MPlatformOlbSystem::GetPanelTransferComponent()
{
	return m_pPanelTransfer;
}

MPanelTransfer* MPlatformOlbSystem::GetPanelTransferOutComponent()
{
	return m_pPanelTransferOut;
}

/*/
//===================================================================
#if FALSE
MPanelTransfer*	MPlatformOlbSystem::GetPanelTransferComponent()
{
	return m_pPanelTransfer;
}
MRollerConveyor* MPlatformOlbSystem::GetRollerConveyorComponent(int iIndex)
{
	return m_pRollerConveyor[iIndex];
}

MTabAligner* MPlatformOlbSystem::GetTabAlignerComponent(int iIndex) 
{
	return m_pTabAligner[iIndex];
}
#endif
/*/

//===================================================================

MTabFeeder* MPlatformOlbSystem::GetTabFeederComponent(int iIndex)
{
	return m_pTabFeeder[iIndex];
}

MTabCarrier* MPlatformOlbSystem::GetTabCarrierComponent(int iIndex)
{
	return m_pTabCarrier[iIndex];
}

MTabCarrier* MPlatformOlbSystem::GetRTabCarrierComponent(int iIndex)
{
	return m_pRTabCarrier[iIndex];
}

MCameraCarrier* MPlatformOlbSystem::GetCameraCarrierComponent(int iIndex) 
{
	return m_pCameraCarrier[iIndex];
}

MCameraCarrier* MPlatformOlbSystem::GetRCameraCarrierComponent(int iIndex) 
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(0);
	return NULL;
#endif
	*/
	return m_pRCameraCarrier[iIndex];
}

MModelChange* MPlatformOlbSystem::GetModelChangeComponent(int iGroupID)
{
	ASSERT(iGroupID < DEF_MAX_WORKER_GROUP);
	return m_pModelChange[iGroupID];
}

MTabMounter* MPlatformOlbSystem::GetTabMounterComponent(int iIndex) 
{
	ASSERT(iIndex < DEF_MAX_TABMOUNTER);
	return m_pTabMounter[iIndex];
}

MTabMounter* MPlatformOlbSystem::GetRTabMounterComponent(int iIndex) 
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(0);
	return NULL;
#endif
	*/
	ASSERT(iIndex < DEF_MAX_TABMOUNTER);
	return m_pRTabMounter[iIndex];
}

MRFID* MPlatformOlbSystem::GetRFIDComponent(int iIndex)
{
	return m_pRFID[iIndex];
}

MLCNet*	MPlatformOlbSystem::GetLCNetComponent()
{
	return m_pLCNet;
}

//SJ_YYK 161104 Add..
#ifdef DEF_USE_TURN_HANDLER
MHandler* MPlatformOlbSystem::GetTurnHandlerComponent()
{
	return m_pTurnHander;
}
#endif

MManageOpPanel*	MPlatformOlbSystem::GetManageOpPanelComponent()
{
	return m_pManageOpPanel;
}

MVisionCalibration*	MPlatformOlbSystem::GetVisionCalibrationComponent()
{
	return m_pVisionCalibration;
}

MCtrlPanelAligner* MPlatformOlbSystem::GetCtrlPanelAlignerComponent()
{
	return m_pCtrlPanelAligner;
}

MCtrlTabCarrier* MPlatformOlbSystem::GetCtrlTabCarrierComponent(int iIndex)
{
	return m_pCtrlTabCarrier[iIndex];
}

MCtrlTabCarrier* MPlatformOlbSystem::GetCtrlRTabCarrierComponent(int iIndex)
{
	return m_pCtrlRTabCarrier[iIndex];
}

MCtrlTabFeeder*	MPlatformOlbSystem::GetCtrlTabFeederComponent(int iIndex)
{
	return m_pCtrlTabFeeder[iIndex];
}

MCtrlTabMounter* MPlatformOlbSystem::GetCtrlTabMounterComponent(int iIndex)
{
	return m_pCtrlTabMounter[iIndex];
}

MCtrlTabMounter* MPlatformOlbSystem::GetCtrlRTabMounterComponent(int iIndex)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(0);
	return NULL;
#endif
	*/
	return m_pCtrlRTabMounter[iIndex];
}

MCtrlInspectionCamera* MPlatformOlbSystem::GetCtrlInspectionCameraComponent(int iIndex)
{
	return m_pCtrlInspectionCamera[iIndex];
}

MCtrlInspectionCamera* MPlatformOlbSystem::GetCtrlRInspectionCameraComponent(int iIndex)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(0);
	return NULL;
#endif
	*/
	return m_pCtrlRInspectionCamera[iIndex];
}

#ifdef DEF_USE_TURN_HANDLER
MCtrlHandler* MPlatformOlbSystem::GetCtrlTHandlerComponent()
{
	return m_pCtrlTHandler;
}
#endif

MTrsAutoManager* MPlatformOlbSystem::GetTrsAutoManagerComponent()
{
	return m_pTrsAutoManager;
}

MTrsPanelAligner* MPlatformOlbSystem::GetTrsPanelAlignerComponent()
{
	return m_pTrsPanelAligner;
}

#ifdef DEF_USE_TRANSFER_CENTER
MTrsPanelTransferCenter* MPlatformOlbSystem::GetTrsPanelTransferCenterComponent()
{
	return m_pTrsPanelTransferCenter;
}
#endif

MTrsPanelTransfer* MPlatformOlbSystem::GetTrsPanelTransferComponent()
{
#ifdef DEF_GATE_SYSTEM
	ASSERT(0);
#endif
	return m_pTrsPanelTransfer;
}

MTrsPanelTransferOut* MPlatformOlbSystem::GetTrsPanelTransferOutComponent()
{
	return m_pTrsPanelTransferOut;
}

MTrsTabFeeder* MPlatformOlbSystem::GetTrsTabFeederComponent(int iIndex)
{
	return m_pTrsTabFeeder[iIndex];
}

MTrsTabCarrier*	MPlatformOlbSystem::GetTrsTabCarrierComponent(int iIndex)
{
	return m_pTrsTabCarrier[iIndex];
}

MTrsTabCarrier*	MPlatformOlbSystem::GetTrsRTabCarrierComponent(int iIndex)
{
	return m_pTrsRTabCarrier[iIndex];
}

MTrsTabMounter*	MPlatformOlbSystem::GetTrsTabMounterComponent(int iIndex)
{
	return m_pTrsTabMounter[iIndex];
}

MTrsTabMounter*	MPlatformOlbSystem::GetTrsRTabMounterComponent(int iIndex)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(0);
	return NULL;
#endif
	*/
	return m_pTrsRTabMounter[iIndex];
}

MTrsInspectionCamera* MPlatformOlbSystem::GetTrsInspectionCameraComponent(int iIndex)
{
	return m_pTrsInspectionCamera[iIndex];
}

MTrsInspectionCamera* MPlatformOlbSystem::GetTrsRInspectionCameraComponent(int iIndex)
{
	/*
#ifdef DEF_SOURCE_SYSTEM
	ASSERT(0);
	return NULL;
#endif
	*/
	return m_pTrsRInspectionCamera[iIndex];
}

//SJ_YYK 161104 Add..
#ifdef DEF_USE_TURN_HANDLER
MTrsTHandler* MPlatformOlbSystem::GetTrsTHandlerComponent()
{
	return m_pTrsTHandler;
}
#endif

//===================================================================
#if FALSE
MTrsPanelTransfer* MPlatformOlbSystem::GetTrsPanelTransferComponent()
{
	return m_pTrsPanelTransfer;
}

MTrsLoadConveyor* MPlatformOlbSystem::GetTrsLoadConveyorComponent()
{
	return m_pTrsLoadConveyor;
}

MTrsUnloadConveyor* MPlatformOlbSystem::GetTrsUnloadConveyorComponent()
{
	return m_pTrsUnloadConveyor;
}
#endif
//===================================================================

const EUserGroup MPlatformOlbSystem::GetCurrentUserGroup() const
{
	return m_eCurrentUserGroup;
}

void MPlatformOlbSystem::SetCurrentUserGroup(EUserGroup eUserGroup)
{
	m_eCurrentUserGroup = eUserGroup;
}

CString MPlatformOlbSystem::getCylIOAddr(int iCylinderNo)
{
	CString strCylinderSol;
	CString strCylinderUpSensor, strCylinderDownSensor;
	CString strTemp;
	unsigned short * rgusSol = m_pCylinder[iCylinderNo]->GetSolAddr();
	unsigned short * rgusUpSensor = m_pCylinder[iCylinderNo]->GetUpSensorAddr();
	unsigned short * rgusDownSensor = m_pCylinder[iCylinderNo]->GetDownSensorAddr();

	strCylinderSol.Format(_T("\n[Sol] "));
	for (int i=0; i<2; i++)
	{
		if (*(rgusSol+i) != 0)
		{
			strTemp.Empty();
			strTemp.Format(_T("%s "),Convert2IOString(*(rgusSol+i)));
			strCylinderSol += strTemp;
		}
	}

	strCylinderUpSensor.Format(_T("\n[UpSensor] "));
	for (int i = 0; i < DEF_MAX_CYLINDER_SENSOR; i++)
	{
		if (*(rgusUpSensor+i) != 0)
		{
			strTemp.Empty();
			strTemp.Format(_T("%s "),Convert2IOString(*(rgusUpSensor+i)));
			strCylinderUpSensor += strTemp;
		}
	}

	strCylinderDownSensor.Format(_T("\n[DownSensor] "));
	for (int i = 0; i < DEF_MAX_CYLINDER_SENSOR; i++)
	{
		if (*(rgusDownSensor+i) != 0)
		{
			strTemp.Empty();
			strTemp.Format(_T("%s "),Convert2IOString(*(rgusDownSensor+i)));
			strCylinderDownSensor += strTemp;
		}
	}

	return strCylinderSol + strCylinderUpSensor + strCylinderDownSensor;
}

CString MPlatformOlbSystem::getVacIOAddr(int iVacuumNo)
{
	CString strVacuumSol;
	CString strVacuumSensor;
	CString strTemp;
	unsigned short * rgusSol = m_pVacuum[iVacuumNo]->GetSolAddr();
	unsigned short * rgusSensor = m_pVacuum[iVacuumNo]->GetSensorAddr();

	strVacuumSol.Format(_T("\n[Sol] "));
	for (int i=0; i<2; i++)
	{
		if (*(rgusSol+i) != 0)
		{
			strTemp.Empty();
			strTemp.Format(_T("%s "),Convert2IOString(*(rgusSol+i)));
			strVacuumSol += strTemp;
		}
	}

	strVacuumSensor.Format(_T("\n[Sensor] "));
	for (int i = 0; i < DEF_MAX_CYLINDER_SENSOR; i++)
	{
		if (*(rgusSensor+i) != 0)
		{
			strTemp.Empty();
			strTemp.Format(_T("%s "),Convert2IOString(*(rgusSensor+i)));
			strVacuumSensor += strTemp;
		}
	}

	return strVacuumSol + strVacuumSensor;
}

// 입력 X19F, 출력 Y19F까지만 계산 가능
CString MPlatformOlbSystem::Convert2IOString(int iIOConstance)
{
	int iIOType;
	int iIOAddress; 

	iIOType = iIOConstance/1000;
	iIOAddress = iIOConstance - iIOType*1000;

	CString strIOType;
	iIOType==1 ? strIOType = "X" : strIOType = "Y";

	CString str3thDigit;
	if (iIOAddress>=160)
	{
		str3thDigit = "1";
		iIOAddress -= 160;
	}
	else
	{
		str3thDigit = "0";
	}

	CString str2ndDigit;
	int i2ndDigitCount = 0;
	while (iIOAddress>=16)
	{
		i2ndDigitCount++;
		iIOAddress -=  16;
	}
	str2ndDigit.Format(_T("%d"), i2ndDigitCount);

	CString str1stDigit;
	switch (iIOAddress)
	{
	case 15: str1stDigit = "F"; break;
	case 14: str1stDigit = "E"; break;
	case 13: str1stDigit = "D"; break;
	case 12: str1stDigit = "C"; break;
	case 11: str1stDigit = "B"; break;
	case 10: str1stDigit = "A"; break;
	default: str1stDigit.Format(_T("%d"),iIOAddress); break;
	}

	return strIOType + str3thDigit + str2ndDigit + str1stDigit;
}

short MPlatformOlbSystem::changeWorkingSetSize()
{
	DWORD dwProcessId;				/*Application program ID variable*/
	HANDLE hProcess;				/*Application program handle variable*/
	DWORD dwMinimumWorkingSetSize;	/*Minimum working set variable*/
	DWORD dwMaximumWorkingSetSize;	/*Maximum working set variable*/

	/*Obtain the application program ID*/
	dwProcessId = GetCurrentProcessId();

	/*Open the application program handle*/
	hProcess = OpenProcess(PROCESS_SET_QUOTA+PROCESS_QUERY_INFORMATION,FALSE,dwProcessId);
	if (hProcess == NULL){
		/*Error end*/
		return(ERROR);
	}

	/*Obtain the maximum working set size and minimum working set size of the application program */
	if (GetProcessWorkingSetSize(hProcess,&dwMinimumWorkingSetSize,&dwMaximumWorkingSetSize)==0){
		/*Error end*/
		CloseHandle(hProcess);
		return(ERROR);
	}

	/*Set the minimum working set size to 1MB*/
	//dwMinimumWorkingSetSize = 1 * 1024 * 1024;
	dwMinimumWorkingSetSize = 2 * 1024 * 1024;

	/*Set the maximum working set size to 3MB*/
	//dwMaximumWorkingSetSize = 3 * 1024 * 1024;
	dwMaximumWorkingSetSize = 4 * 1024 * 1024;

	/*Change the maximum working set size and minimum working set size of the application program */
	if (SetProcessWorkingSetSize(hProcess,dwMinimumWorkingSetSize,dwMaximumWorkingSetSize)==0){
		/*Error end*/
		CloseHandle(hProcess);
		return(ERROR);
	}

	/*Close the application program handle*/
	CloseHandle(hProcess);

	/*Normal return*/
	return(0);
}

int MPlatformOlbSystem::initSercos()
{
	// hongju : Sercos의 경우는 BoardInit은 프로그램 시작할때 한번만 한다..그외에는 필요 없음.
	int iResult = m_pSercosSetup->BoardInit();
	if (iResult)
	{		
		CString strMsg = m_pSercosSetup->GetErrorString();
		iResult = MyMessageBox(strMsg, _T("Error"), M_ICONERROR);
		return iResult;
	}

	iResult = m_pSercosSetup->SercosSetup();
	if (iResult)
		return iResult;

	return ERR_SYSTEM_SUCCESS;
}

BOOL MPlatformOlbSystem::IsSkipAxis(int iAxisNo)
{
	switch (iAxisNo)
	{
#	ifdef DEF_SOURCE_SYSTEM

		case DEF_AXIS_SERCOS_R_MOUNTER1_X:
		case DEF_AXIS_SERCOS_R_MOUNTER2_X:
		case DEF_AXIS_SERCOS_R_MOUNTER3_X:
		case DEF_AXIS_SERCOS_R_MOUNTER4_X:
		case DEF_AXIS_SERCOS_R_MOUNTER1_Z:
		case DEF_AXIS_SERCOS_R_MOUNTER2_Z:
		case DEF_AXIS_SERCOS_R_MOUNTER3_Z:
		case DEF_AXIS_SERCOS_R_MOUNTER4_Z:
		case DEF_AXIS_SERCOS_R_BACKUP1_Z:
		case DEF_AXIS_SERCOS_R_BACKUP2_Z:
		case DEF_AXIS_SERCOS_R_BACKUP3_Z:
		case DEF_AXIS_SERCOS_R_BACKUP4_Z:
		case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X:
		case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA3_X:
		case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T:
		case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T:
		case DEF_AXIS_SERCOS_R_INSPECT_EXPAND1:
		case DEF_AXIS_SERCOS_R_INSPECT_EXPAND2:
		case DEF_AXIS_SERCOS_R_INSPECT_EXPAND3:
		case DEF_AXIS_SERCOS_R_INSPECT_EXPAND4:
		case DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND:
		case DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2:
		case DEF_AXIS_ACS_R_GANTRY_Y:

#ifndef DEF_USE_TRANSFER_CENTER
		case DEF_AXIS_SERCOS_SPARE_76:
		case DEF_AXIS_SERCOS_SPARE_77:
#endif
#ifndef DEF_USE_TURN_HANDLER
		case DEF_AXIS_SERCOS_SPARE_78:
		case DEF_AXIS_SERCOS_SPARE_79:
#endif
#ifndef DEF_USE_TRANSFER_OUT_MULTI_AXIS
		case DEF_AXIS_SERCOS_SPARE_81:
#endif

		case DEF_AXIS_SERCOS_R_BADTAB_DETECTOR_X:
		case DEF_AXIS_SERCOS_R_FEEDER_SPROCKET1_T:
		case DEF_AXIS_SERCOS_R_FEEDER_SPROCKET2_T:
		case DEF_AXIS_SERCOS_R_TAB_PRESS_Z:
		case DEF_AXIS_SERCOS_R_FEEDER_REEL_T:
		case DEF_AXIS_SERCOS_R_MOLDING_UPDOWN_Z:
		case DEF_AXIS_SERCOS_BADTAB_DETECTOR_X:
		case DEF_AXIS_SERCOS_FEEDER_SPROCKET1_T:
		case DEF_AXIS_SERCOS_FEEDER_SPROCKET2_T:
		case DEF_AXIS_SERCOS_TAB_PRESS_Z:
		case DEF_AXIS_SERCOS_FEEDER_REEL_T:
		case DEF_AXIS_SERCOS_MOLDING_UPDOWN_Z:
			return TRUE;
			break;
#	endif
	}



	return FALSE;
}

int MPlatformOlbSystem::LightOn(int iCamNo)
{
	ASSERT(iCamNo >= DEF_CAMERA_1 && iCamNo <= DEF_MAX_CAMERA_NO);

	if (iCamNo == DEF_MAX_CAMERA_NO)
	{
		for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
		{
			if (-1 != s_rgLightIdForCam[i].x)
			m_pLighting[s_rgLightIdForCam[i].x]->LightOn();
			
			if (-1 != s_rgLightIdForCam[i].y)
				m_pLighting[s_rgLightIdForCam[i].y]->LightOn();
		}
	}
	else
	{
		if (-1 != s_rgLightIdForCam[iCamNo].x)
			m_pLighting[s_rgLightIdForCam[iCamNo].x]->LightOn();
		
		if (-1 != s_rgLightIdForCam[iCamNo].y)
			m_pLighting[s_rgLightIdForCam[iCamNo].y]->LightOn();
	}
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::LightDefult(int iCamNo)
{
	ASSERT(iCamNo >= DEF_CAMERA_1 && iCamNo <= DEF_MAX_CAMERA_NO);
	
	if (iCamNo == DEF_MAX_CAMERA_NO)
	{
		for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
		{
			if(i == DEF_INSPECTION_CAMERA_1
				|| i == DEF_INSPECTION_CAMERA_2
				|| i == DEF_INSPECTION_CAMERA_3
				|| i == DEF_INSPECTION_CAMERA_4
				|| i == DEF_INSPECTION_CAMERA_5
				|| i == DEF_INSPECTION_CAMERA_6
				|| i == DEF_INSPECTION_CAMERA_7
				|| i == DEF_INSPECTION_CAMERA_8
				)
			{
				if (-1 != s_rgLightIdForCam[i].x)
					m_pLighting[s_rgLightIdForCam[i].x]->LightOff();
			}			
			else
			{
				if (-1 != s_rgLightIdForCam[i].x)
					m_pLighting[s_rgLightIdForCam[i].x]->LightOn();
			}
			
			
			if (-1 != s_rgLightIdForCam[i].y)
				m_pLighting[s_rgLightIdForCam[i].y]->LightOn();
		}
	}
	else
	{
		if(iCamNo == DEF_INSPECTION_CAMERA_1
			|| iCamNo == DEF_INSPECTION_CAMERA_2
			|| iCamNo == DEF_INSPECTION_CAMERA_3
			|| iCamNo == DEF_INSPECTION_CAMERA_4
			|| iCamNo == DEF_INSPECTION_CAMERA_5
			|| iCamNo == DEF_INSPECTION_CAMERA_6
			|| iCamNo == DEF_INSPECTION_CAMERA_7
			|| iCamNo == DEF_INSPECTION_CAMERA_8
			)
		{
			if (-1 != s_rgLightIdForCam[iCamNo].x)
				m_pLighting[s_rgLightIdForCam[iCamNo].x]->LightOff();
		}			
		else
		{
			if (-1 != s_rgLightIdForCam[iCamNo].x)
			m_pLighting[s_rgLightIdForCam[iCamNo].x]->LightOn();
		}
		
		if (-1 != s_rgLightIdForCam[iCamNo].y)
			m_pLighting[s_rgLightIdForCam[iCamNo].y]->LightOn();
	}
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::LightOff(int iCamNo)
{
	ASSERT(iCamNo >= DEF_CAMERA_1 && iCamNo <= DEF_MAX_CAMERA_NO);

	if (iCamNo == DEF_MAX_CAMERA_NO)
	{
		for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
		{
			if (-1 != s_rgLightIdForCam[i].x)
				m_pLighting[s_rgLightIdForCam[i].x]->LightOff();
			
			if (-1 != s_rgLightIdForCam[i].y)
				m_pLighting[s_rgLightIdForCam[i].y]->LightOff();
		}
	}
	else
	{
		if (-1 != s_rgLightIdForCam[iCamNo].x)
			m_pLighting[s_rgLightIdForCam[iCamNo].x]->LightOff();
		
		if (-1 != s_rgLightIdForCam[iCamNo].y)
			m_pLighting[s_rgLightIdForCam[iCamNo].y]->LightOff();
	}
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::SetLightValue(int iCamNo)
{
	ASSERT(iCamNo >= DEF_CAMERA_1 && iCamNo <= DEF_MAX_CAMERA_NO);

	if (iCamNo == DEF_MAX_CAMERA_NO)
	{
		for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
		{
			if (-1 != s_rgLightIdForCam[i].x)
				m_pLighting[s_rgLightIdForCam[i].x]->SetValue((unsigned short)m_pObjPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[i].x]);
			
			if (-1 != s_rgLightIdForCam[i].y)
				m_pLighting[s_rgLightIdForCam[i].y]->SetValue((unsigned short)m_pObjPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[i].y]);
		}
	}
	else
	{
		if (-1 != s_rgLightIdForCam[iCamNo].x)
			m_pLighting[s_rgLightIdForCam[iCamNo].x]->SetValue((unsigned short)m_pObjPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[iCamNo].x]);
		
		if (-1 != s_rgLightIdForCam[iCamNo].y)
			m_pLighting[s_rgLightIdForCam[iCamNo].y]->SetValue((unsigned short)m_pObjPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[iCamNo].y]);
	}
	return ERR_SYSTEM_SUCCESS;
}

int MPlatformOlbSystem::GetCalibrationUnitNo(int iCamNo, int iInstanceID)
{
	/*/
	switch (iCamNo)
	{
	case DEF_F_TABALIGNER_CAMERA_1:
	case DEF_F_TABALIGNER_CAMERA_2:		
		ASSERT(iInstanceID >= 0 || iInstanceID < 4);
		//return iInstanceID;
		return 0;
		break;
	case DEF_R_TABALIGNER_CAMERA_1:
	case DEF_R_TABALIGNER_CAMERA_2:
		ASSERT(iInstanceID >= 0 || iInstanceID < 4);
		//return iInstanceID;
		return 0;
		break;

	///case DEF_PANEL_ALIGN_CAMERA_1:
	///case DEF_PANEL_ALIGN_CAMERA_2:

	case DEF_INSPECTION_CAMERA_1:
	case DEF_INSPECTION_CAMERA_2:
	case DEF_INSPECTION_CAMERA_3:
	case DEF_INSPECTION_CAMERA_4:
	case DEF_INSPECTION_CAMERA_5:
	case DEF_INSPECTION_CAMERA_6:
	case DEF_INSPECTION_CAMERA_7:
	case DEF_INSPECTION_CAMERA_8:
		return 0;		
	default:
		break;
	}
	/*/
	return 0;
}

void MPlatformOlbSystem::ReportModel()
{
	return ;

	// Loading Tab Qualtity
#ifdef DEF_SOURCE_SYSTEM
	CString strFeeder1 = "Source";
	CString strFeeder2 = "Source";
#else // DEF_GATE_SYSTEM
	CString strFeeder1 = "Gate";
	CString strFeeder2 = "Gate";
#endif

	int iXAssemble = 0;
	int iYAssemble = 0;
	
	// Assemble Quantity
	UINT i;
	for (i = 0; i < (UINT)m_pObjTabData->m_iXuTabQty; i++)
	{
		if (m_pObjPanelData->m_bUseFlag_Xu && m_pObjTabData->m_bUseXuTab[i]) 
			iXAssemble++;
	}
	for (i = 0; i < (UINT)m_pObjTabData->m_iXdTabQty; i++)
	{
		if (m_pObjPanelData->m_bUseFlag_Xd && m_pObjTabData->m_bUseXdTab[i]) 
			iXAssemble++;
	}
	for (i = 0; i < (UINT)m_pObjTabData->m_iYlTabQty; i++)
	{
		if (m_pObjPanelData->m_bUseFlag_Yl && m_pObjTabData->m_bUseYlTab[i]) 
			iYAssemble++;
	}
	for (i = 0; i < (UINT)m_pObjTabData->m_iYrTabQty; i++)
	{
		if (m_pObjPanelData->m_bUseFlag_Yr && m_pObjTabData->m_bUseYrTab[i])
			iYAssemble++;
	}

	int iTotalFeeder1, iTotalFeeder2;
	switch (m_pObjSystemData->m_eSystemType)
	{
	case SYSTEM_TYPE_SOURCE:
		iTotalFeeder1 = iTotalFeeder2 = iXAssemble;
		break;
	case SYSTEM_TYPE_GATE:
		iTotalFeeder1 = iTotalFeeder2 = iYAssemble;
		break;
	}

	m_pLCNet->ModelDataReport(m_pObjSystemData->m_strModelFileName,
								iTotalFeeder1, iTotalFeeder2,
								strFeeder1, strFeeder2);
}

int MPlatformOlbSystem::removeTabMounterTabIC(int iWorkGroupNo)
{
	int iResult = SUCCESS;
	
	int i = 0;	
	ASSERT(iWorkGroupNo > -1 && iWorkGroupNo < DEF_MAX_GROUP);
	
	double tgPos[DEF_MOUNTER_WORK_PER_GROUP];
	MPos_XYT tgPosCarrier[DEF_MAX_TABCARRIER];

	MTabMounter* pTabMounter[DEF_MOUNTER_WORK_PER_GROUP];
	MCtrlTabMounter* pCtrlTabMounter[DEF_MOUNTER_WORK_PER_GROUP];
	MTabCarrier* pTabCarrier[DEF_MAX_TABCARRIER];

	//회피 위치로 이동한다_____
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (iWorkGroupNo == DEF_FRONT_GROUP)
		{
			/*/
			pTabMounter[i] = m_pTabMounter[i];
			pCtrlTabMounter[i] = m_pCtrlTabMounter[i];
			pTabCarrier[i] = m_pTabCarrier[i];
			tgPos[i] = pTabMounter[i]->GetTargetPos(DEF_TABMOUNTER_REMOVE_POS) + (DEF_TABMOUNTER_STANDARD_GAP+2.0)*i;
			/*/
			pTabMounter[i] = m_pTabMounter[i];
			pCtrlTabMounter[i] = m_pCtrlTabMounter[i];
			pTabCarrier[i] = m_pTabCarrier[i];
		}
		else
		{
			/*/
			pTabMounter[i] = m_pTabMounter[DEF_MAX_TABMOUNTER-i-1];
			pCtrlTabMounter[i] = m_pCtrlTabMounter[DEF_MAX_TABMOUNTER-i-1];
			pTabCarrier[i] = m_pRTabCarrier[i];
//			tgPos[i] = pTabMounter[i]->GetTargetPos(DEF_TABMOUNTER_REMOVE_POS) + (DEF_TABMOUNTER_STANDARD_GAP+2.0)*(DEF_MOUNTER_WORK_PER_GROUP-i-1);
			tgPos[i] = pTabMounter[i]->GetTargetPos(DEF_TABMOUNTER_REMOVE_POS) - (DEF_TABMOUNTER_STANDARD_GAP+2.0)*i;
			/*/
			pTabMounter[i] = m_pRTabMounter[i];
			pCtrlTabMounter[i] = m_pCtrlRTabMounter[i];
			pTabCarrier[i] = m_pRTabCarrier[i];
		}
		tgPos[i] = pTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_READY_POS);
		tgPosCarrier[i] = pTabCarrier[i]->GetTargetXYTPos(DEF_TABCARRIER_READY_POS);
	}

	if (iWorkGroupNo == DEF_FRONT_GROUP)
	{
		iResult = MoveAllFrontTabMounter(tgPos, DEF_TABMOUNTER_READY_POS);	//@DEF_TABMOUNTER_REMOVE_POS);
		if (iResult)
			return iResult;
//@		iResult = MoveAllFrontTabCarrier(tgPosCarrier, DEF_TABCARRIER_READY_POS);
//@		if (iResult)
//@			return iResult;
	}
	else
	{
		iResult = MoveAllRearTabMounter(tgPos, DEF_TABMOUNTER_READY_POS);	//@DEF_TABMOUNTER_REMOVE_POS);
		if (iResult)
			return iResult;
//@		iResult = MoveAllRearTabCarrier(tgPosCarrier, DEF_TABCARRIER_READY_POS);
//@		if (iResult)
//@			return iResult;
	}

	//Remove 위치로 이동후 하강, 버리기 동작후 Ready위치로 이동한다
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (TRUE == pTabMounter[i]->IsAbsorbTabIC())
		{
			iResult = pCtrlTabMounter[i]->Remove();
			if (iResult)
				return iResult;
		}
		else
		{
			pTabMounter[i]->ReleaseTabIC();
		}
//@		iResult = pTabMounter[i]->SafeMovePos(0, DEF_TABMOUNTER_READY_POS, FALSE);
		//iResult = pTabMounter[i]->SafeMovePos(0, DEF_TABMOUNTER_ESCAPE_POS, FALSE);
		double dRemoveEscapePos = 0.0;
		dRemoveEscapePos = pTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_REMOVE_POS) - (DEF_TABMOUNTER_COLLISION_GAP + 10.0  ) * ((DEF_MOUNTER_WORK_PER_GROUP -1) - i);
			
		iResult = pTabMounter[i]->MovePos(dRemoveEscapePos, DEF_TABMOUNTER_REMOVE_POS, FALSE);		
		if (iResult)
			return iResult;
		Sleep(100);
		if (TRUE == m_pOpPanel->GetStopButtonStatus())
		{
			pTabMounter[i]->Wait4Done();
			return USER_STOP;
		}
	}
	
//@	iResult = pTabMounter[DEF_MOUNTER_WORK_PER_GROUP-1]->Wait4Done();
//@	if (iResult)
//@		return iResult;
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		iResult = pTabMounter[i]->Wait4Done();
		if (iResult)
			return iResult;
	}

//@	iResult = pTabCarrier[DEF_MOUNTER_WORK_PER_GROUP-1]->BackwardDumpBasket();
//@	if (iResult)
//@		return iResult;
	/*
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		tgPos[i] = pTabMounter[i]->GetTargetPos(DEF_TABMOUNTER_LOAD_POS);
		tgPosCarrier[i] = pTabCarrier[i]->GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS);
	}
	
	//TabMounter를 Loading위치로 이동시키고, TabCarrier를 Unloading위치로 이동시킨다.
	if (iWorkGroupNo == DEF_FRONT_GROUP)
	{
		iResult = MoveAllFrontTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
		if (iResult)
			return iResult;
		iResult = MoveAllFrontTabCarrier(tgPosCarrier, DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
			return iResult;
	}
	else
	{
		iResult = MoveAllRearTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
		if (iResult)
			return iResult;
		iResult = MoveAllRearTabCarrier(tgPosCarrier, DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
			return iResult;
	}

	//흡착되어 있는 Mounter가 없으면 동작을 멈춘다
	if (DRY_RUN_MODE == m_pObjSystemData->m_eRunMode)
		return SUCCESS;
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (pTabMounter[i]->IsAbsorbTabIC())
			break;
	}
	if (i==DEF_MOUNTER_WORK_PER_GROUP)
		return SUCCESS;

	if (TRUE == m_pOpPanel->GetStopButtonStatus())
		return USER_STOP;
	
	//TabMounter의 Tab을 Carrier 쓰레기통에 버린다
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;

		iResult = pCtrlTabMounter[i]->Remove();
		if (iResult)
			return iResult;
	}
	*/

	return SUCCESS;
}

int MPlatformOlbSystem::RemoveTabIC(int iWorkGroupNo, BOOL bRemoveTabCarrierTabIC)
{
//	if (MANUAL_MODE != m_pTrsAutoManager->GetOPMode())
//		return SUCCESS;

	int iResult = SUCCESS;
	int i = 0;
		
	BOOL bRemove = IsExistRemoveTab_TabMounter(iWorkGroupNo);

	if (bRemove)
	{
		iResult = removeTabMounterTabIC(iWorkGroupNo);
		if (iResult)
			return iResult;
	}

	if (TRUE == bRemoveTabCarrierTabIC)
	{
		if (TRUE == IsExistRemoveTab_TabCarrier(iWorkGroupNo))
		{
			iResult = LoadTabMounterTabIC(iWorkGroupNo);
			if (iResult)
				return iResult;

			iResult = removeTabMounterTabIC(iWorkGroupNo);
			if (iResult)
				return iResult;
		}
	}

	return SUCCESS;
}

int MPlatformOlbSystem::LoadTabCarrierTabIC(int iGroupNo, int iCarrierVacNo)
{
#if FALSE	//===================================
	/*/
	if (MANUAL_MODE != m_pTrsAutoManager->GetOPMode())
		return SUCCESS;

	int iCarrierLoadPosNo = -1;
	int iResult = SUCCESS;
	EWorkingSide eWorkSide = m_pObjProcessData->GetCurrentWorkingSide(iGroupNo);
	if (WORKING_SIDE_UNKNOWN == eWorkSide)
		return NOT_USE_WORKSIDE;

	BOOL bAllWork = (-1 == iCarrierVacNo) ? TRUE : FALSE;
	int iWorkCount = (TRUE == bAllWork) ? DEF_MAX_WORKER_PER_GROUP : 1;

	int iStart = -1;
	int iEnd = -1;
	int iGap = 0;
	
	if (DEF_FRONT_GROUP == iGroupNo)
	{
		iStart = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[iGroupNo];
		iEnd = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iGroupNo] - 1;
		iGap = -1;
	}
	else // if (DEF_REAR_GROUP == iGroupNo)
	{
		iStart = DEF_TABMOUNTER_GROUP_TABMOUNTER_MIN[iGroupNo];
		iEnd = DEF_TABMOUNTER_GROUP_TABMOUNTER_MAX[iGroupNo] + 1;
		iGap = 1;
	}

	iStart -= iGroupNo * DEF_MAX_WORKER_PER_GROUP;
	iEnd -= iGroupNo * DEF_MAX_WORKER_PER_GROUP;

	for (int i = iStart; i != iEnd; i += iGap)
	{
		if (TRUE == m_pObjProcessData->IsTabMounterNotSelectedForWork(eWorkSide, i + iGroupNo * DEF_MAX_WORKER_PER_GROUP))
			continue;

		if (TRUE == bAllWork)
			iCarrierVacNo = i;

		if (TRUE == m_pTabCarrier[iGroupNo]->IsAbsorbTabIC(iCarrierVacNo))
		{
			if (FALSE == bAllWork)
				break;

			continue;
		}

		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;
		
		iCarrierLoadPosNo = DEF_TABCARRIER_LOAD_POS1 + iCarrierVacNo;
		iResult = m_pCtrlTabCarrier[iGroupNo]->MoveLoadingPos(iCarrierLoadPosNo);
		if (iResult)
			return iResult;

		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;
		
		iResult = m_pTabFeeder[iGroupNo]->SafeMovePressPos(DEF_PRESS_READY_POS);
		if (iResult)
			return iResult;

		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;
		
		iResult = m_pCtrlTabFeeder[iGroupNo]->FeedTabIC();
		if (iResult)
			return iResult;

		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;
		
		iResult = m_pCtrlTabFeeder[iGroupNo]->PressTapeStep1();
		if (iResult)
			return iResult;

		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;
		
		iResult = m_pCtrlTabCarrier[iGroupNo]->LoadTabIC(iCarrierVacNo);
		if (iResult)
			return iResult;

		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;
		
		iResult = m_pCtrlTabFeeder[iGroupNo]->PressTapeStep2();
		if (iResult)
			return iResult;

		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;

		if (FALSE == m_pTabCarrier[iGroupNo]->IsDown())
		{
			iResult = m_pTabCarrier[iGroupNo]->Down();
			if (iResult)
				return iResult;
		}

		if (TRUE == m_pOpPanel->GetStopButtonStatus())
			return USER_STOP;
		
		if (FALSE == m_pTabCarrier[iGroupNo]->IsAbsorbTabIC(iCarrierVacNo))
		{
			// 317002 = TabIC Loading 동작 후 TabIC 흡착하고 있지 않음.
			return 317002 * 1000 + iGroupNo;
		}

		if (FALSE == bAllWork)
			break;
	}
	/*/

#endif	//#if FALSE	=============================

	return SUCCESS;	
}

int MPlatformOlbSystem::LoadTabMounterTabIC(int iGroupNo)
{
	int iResult = SUCCESS;
	int i = 0;	
	ASSERT(iGroupNo > -1 && iGroupNo < DEF_MAX_GROUP);
	
	MTabMounter* pTabMounter[DEF_MOUNTER_WORK_PER_GROUP];
	MCtrlTabMounter* pCtrlTabMounter[DEF_MOUNTER_WORK_PER_GROUP];
	MTabCarrier* pTabCarrier[DEF_MAX_TABCARRIER];

	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (iGroupNo == DEF_FRONT_GROUP)
		{
			pTabMounter[i] = m_pTabMounter[i];
			pCtrlTabMounter[i] = m_pCtrlTabMounter[i];
			pTabCarrier[i] = m_pTabCarrier[i];
		}
		else
		{
			/*/
			pTabMounter[i] = m_pTabMounter[DEF_MAX_TABMOUNTER-i-1];
			pCtrlTabMounter[i] = m_pCtrlTabMounter[DEF_MAX_TABMOUNTER-i-1];
			pTabCarrier[i] = m_pRTabCarrier[i];
			/*/
			pTabMounter[i] = m_pRTabMounter[i];
			pCtrlTabMounter[i] = m_pCtrlRTabMounter[i];
			pTabCarrier[i] = m_pRTabCarrier[i];
		}
	}
	double tgPos[DEF_MOUNTER_WORK_PER_GROUP];
	MPos_XYT tgPosCarrier[DEF_MAX_TABCARRIER];
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		tgPos[i] = pTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_LOAD_POS);
		tgPosCarrier[i] = pTabCarrier[i]->GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS);
	}
	if (iGroupNo == DEF_FRONT_GROUP)
	{
		iResult = MoveAllFrontTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
		if (iResult)
			return iResult;

		iResult = MoveAllFrontTabCarrier(tgPosCarrier, DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
			return iResult;		
	}
	else
	{
		iResult = MoveAllRearTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
		if (iResult)
			return iResult;

		iResult = MoveAllRearTabCarrier(tgPosCarrier, DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
			return iResult;		
	}

	int iMounterID = 0;
	int iCarrierID = 0;
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		iMounterID = i;
		iCarrierID = i;
		/*
		iResult = pTabMounter[iMounterID]->SafeMovePos(0, DEF_TABMOUNTER_LOAD_POS, FALSE);
		if (iResult)
			return iResult;
		iResult = pTabCarrier[iCarrierID]->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
			return iResult;
		iResult = pTabMounter[iMounterID]->Wait4Done();
		if (iResult)
			return iResult;
			*/

		if (FALSE == pTabMounter[iMounterID]->IsAbsorbTabIC() && TRUE == pTabCarrier[iCarrierID]->IsAbsorbTabIC())
		{
			iResult = pCtrlTabMounter[iMounterID]->LoadTabIC(iGroupNo, iCarrierID);
			if (iResult)
				return iResult;
		}
		
		/*
		iResult = pTabMounter[iMounterID]->SafeMovePos(0, DEF_TABMOUNTER_ESCAPE_POS, FALSE);
		if (iResult)
			return iResult;
//		iResult = pTabCarrier[iCarrierID]->SafeMoveXYTPos(DEF_TABCARRIER_ESCAPE_POS);
//		if (iResult)
			return iResult;
		iResult = pTabMounter[iMounterID]->Wait4Done();
		if (iResult)
			return iResult;
			*/
	}

	/*/
	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		tgPos[i] = pTabMounter[i]->GetTargetPos(0, DEF_TABMOUNTER_LOAD_POS);
		tgPosCarrier[i] = pTabCarrier[i]->GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS);
	}
	
	//TabMounter를 Loading위치로 이동시키고, TabCarrier를 Unloading위치로 이동시킨다.
	if (iGroupNo == DEF_FRONT_GROUP)
	{
		iResult = MoveAllFrontTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
		if (iResult)
			return iResult;
		iResult = MoveAllFrontTabCarrier(tgPosCarrier, DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
			return iResult;
	}
	else
	{
		iResult = MoveAllRearTabMounter(tgPos, DEF_TABMOUNTER_LOAD_POS);
		if (iResult)
			return iResult;
		iResult = MoveAllRearTabCarrier(tgPosCarrier, DEF_TABCARRIER_UNLOAD_POS);
		if (iResult)
			return iResult;
	}

	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (pTabCarrier[i]->IsAbsorbTabIC())
		{
			iResult = pCtrlTabMounter[i]->LoadTabIC(iGroupNo, i);
			if (iResult)
				return iResult;
		}
	}
	/*/
	
	return SUCCESS;
}

int MPlatformOlbSystem::PrealignTabIC(int iGroupNo, int iWorkTabNo, BOOL bSkipAfterMove, BOOL bUseBaseLineOnly)
{
	return SUCCESS;
}

int MPlatformOlbSystem::GetXOffsetPrealignTabIC(int iGroupNo, int iWorkTabNo, BOOL bSkipAfterMove)
{
	return SUCCESS;
}

CString MPlatformOlbSystem::GetOpenDoorPosition(void)
{
	BOOL bFrontDoor = FALSE;
	BOOL bRearDoor = FALSE;
	BOOL bLightCurtain = FALSE;
	BOOL bFlugOpen = FALSE;

    CString strFrontOpenDoor;
    CString strRearOpenDoor;
	CString strLightCurtain;
	CString strFlugOpen;

    strFrontOpenDoor.Empty();
    strRearOpenDoor.Empty();
	strLightCurtain.Empty();
	strFlugOpen.Empty();
 
	if (m_pIO->IsOff(IN_FRONT_DOOR1))
	{
		if (bFrontDoor)
			strFrontOpenDoor += ",";
		
		strFrontOpenDoor += " 1";
		bFrontDoor = TRUE;
	}	
	if (m_pIO->IsOff(IN_FRONT_DOOR2))
    {
		if (bFrontDoor)
			strFrontOpenDoor += ",";
		
        strFrontOpenDoor += " 2";
		bFrontDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_FRONT_DOOR3))
    {
		if (bFrontDoor)
			strFrontOpenDoor += ",";
		
        strFrontOpenDoor += " 3";
		bFrontDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_FRONT_DOOR4))
    {
		if (bFrontDoor)
			strFrontOpenDoor += ",";
		
        strFrontOpenDoor += " 4";
		bFrontDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_FRONT_DOOR5))
    {
		if (bFrontDoor)
			strFrontOpenDoor += ",";
		
        strFrontOpenDoor += " 5";
		bFrontDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_FRONT_DOOR6))
    {
		if (bFrontDoor)
			strFrontOpenDoor += ",";
		
        strFrontOpenDoor += " 6";
		bFrontDoor = TRUE;
    }


    if (m_pIO->IsOff(IN_REAR_DOOR1))
    {
		if (bRearDoor)
			strRearOpenDoor += ",";
		
        strRearOpenDoor += " 1";
		bRearDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_REAR_DOOR2))
    {
		if (bRearDoor)
			strRearOpenDoor += ",";
		
        strRearOpenDoor += " 2";
		bRearDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_REAR_DOOR3))
    {
		if (bRearDoor)
			strRearOpenDoor += ",";
		
        strRearOpenDoor += " 3";
		bRearDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_REAR_DOOR4))
    {
		if (bRearDoor)
			strRearOpenDoor += ",";
		
        strRearOpenDoor += " 4";
		bRearDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_REAR_DOOR5))
    {
		if (bRearDoor)
			strRearOpenDoor += ",";
		
        strRearOpenDoor += " 5";
		bRearDoor = TRUE;
    }
	if (m_pIO->IsOff(IN_REAR_DOOR6))
    {
		if (bRearDoor)
			strRearOpenDoor += ",";
		
        strRearOpenDoor += " 6";
		bRearDoor = TRUE;
    }

	if (bFrontDoor)
		strFrontOpenDoor = "[Front" + strFrontOpenDoor + "] ";
	
	if (bRearDoor)
		strRearOpenDoor = "[Rear" + strRearOpenDoor + "]";

	return " " + strFrontOpenDoor + strRearOpenDoor;
}

void MPlatformOlbSystem::registerComponentInstance(int iObjectID, ICommon* plnkComp)
{
	if (SUCCESS != m_pObjSystemInfo->RegisterComponent(iObjectID, plnkComp))
	{
		//생성된 Object Instance 를 등록할 수 없습니다.\r\nObject ID 를 확인하세요.
		CString strMsg;
		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_61518"), &strMsg);

		CString strID;
		strID.Format(_T(" [ID: %d]"), iObjectID);

		strMsg += strID;
		MyMessageBox(strMsg, _T("System Object Creation Error"), M_ICONERROR);
	}
}

// jdy sesl
const BOOL MPlatformOlbSystem::IsNoWorkInAutoRunMode() const
{
	int i = 0;

	if (SYSTEM_TYPE_SOURCE == m_pObjSystemData->m_eSystemType)
	{
		if (TRUE == m_pObjPanelData->m_bUseFlag_Xu)
		{
			for (int i = 0; i < m_pObjTabData->m_iXuTabQty; i++)
			{
				if (TRUE == m_pObjTabData->m_bUseXuTab[i])
					return FALSE;
			}
		}
		
		if (TRUE == m_pObjPanelData->m_bUseFlag_Xd)
		{
			for (int i = 0; i < m_pObjTabData->m_iXdTabQty; i++)
			{
				if (TRUE == m_pObjTabData->m_bUseXdTab[i])
					return FALSE;
			}
		}
	}

	if (SYSTEM_TYPE_GATE == m_pObjSystemData->m_eSystemType)
	{
		if (TRUE == m_pObjPanelData->m_bUseFlag_Yl)
		{
			for (int i = 0; i < m_pObjTabData->m_iYlTabQty; i++)
			{
				if (TRUE == m_pObjTabData->m_bUseYlTab[i])
					return FALSE;
			}
		}
		
		if (TRUE == m_pObjPanelData->m_bUseFlag_Yr)
		{
			for (int i = 0; i < m_pObjTabData->m_iYrTabQty; i++)
			{
				if (TRUE == m_pObjTabData->m_bUseYrTab[i])
					return FALSE;
			}
		}
	}
	return TRUE;
}

int MPlatformOlbSystem::Get_CarrierToTabMounter(int iWorkGroup, int iCarrierNo)
{
	int iMounterNo = 0;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (iWorkGroup == DEF_FRONT_GROUP)
	{
		iMounterNo = iCarrierNo;
	}
	else
	{
		int nToolNo[] = {7, 6, 5, 4};
		iMounterNo = nToolNo[iCarrierNo];
	}
#else
	int nToolNo[] = {3,2,1,0};
	iMounterNo = nToolNo[iCarrierNo];
#endif
	/*/
	ASSERT(iCarrierNo < DEF_MAX_TABCARRIER);
#ifdef DEF_SOURCE_SYSTEM
	int nToolNo[] = {0,1,2,3};
#else
	int nToolNo[] = {1,0};

#endif
	iMounterNo = nToolNo[iCarrierNo];
	return iMounterNo;
}

int	MPlatformOlbSystem::Get_CarrierToTabMounterOtherGroup(int iWorkGroup, int iCarrierNo)
{
	int iMounterNo = 0;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (iWorkGroup == DEF_FRONT_GROUP)
	{
		int nToolNo[] = {4, 5, 6, 7};
		iMounterNo = nToolNo[iCarrierNo];
	}
	else
	{
		int nToolNo[] = {3, 2, 1, 0};
		iMounterNo = nToolNo[iCarrierNo];		
	}
#else
	int nToolNo[] = {3,2,1,0};
	iMounterNo = nToolNo[iCarrierNo];
#endif
	/*/

	ASSERT(iCarrierNo < DEF_MAX_TABCARRIER);
#ifdef DEF_SOURCE_SYSTEM
	int nToolNo[] = {0,1,2,3};
#else
	int nToolNo[] = {1,0};
#endif
	iMounterNo = nToolNo[iCarrierNo];
	return iMounterNo;
}

int	MPlatformOlbSystem::Get_ToolToCarrier(int iToolNo)
{
	int iCarrierNo = 0;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	int nCarrierNo[] = {0, 1, 2, 3, 3, 2, 1, 0};
	iCarrierNo = nCarrierNo[iToolNo];	
#else
	iCarrierNo = iToolNo;
#endif
	/*/
	ASSERT(iToolNo < DEF_MAX_TABMOUNTER);
	iCarrierNo = iToolNo;
	return iCarrierNo;
}

int	MPlatformOlbSystem::Get_ToolToCarrierOtherGroup(int iToolNo)
{
	/*/
#ifndef DEF_SOURCE_SYSTEM
	ASSERT(0);
#endif
	int iCarrierNo = 0;
	int nCarrierNo[] = {3, 2, 1, 0, 0, 1, 2, 3};
	iCarrierNo = nCarrierNo[iToolNo];	
	return iCarrierNo;
	/*/
	ASSERT(0);
	return 0;
}

int	MPlatformOlbSystem::Get_ToolInstanceNo(int iWorkGroup, int iWorkToolNo)
{
	ASSERT(iWorkToolNo < DEF_MAX_TABMOUNTER);

	int iToolInstance = iWorkToolNo;
	/*/
#ifdef DEF_GATE_SYSTEM
	iToolInstance = iWorkToolNo;
#else
	if (iWorkGroup == DEF_FRONT_GROUP)
	{
		iToolInstance = iWorkToolNo;
	}
	else
	{
		iToolInstance = DEF_MAX_TABMOUNTER - 1 - iWorkToolNo;
	}
#endif
	/*/
	return iToolInstance;
}

//KKY____________________
int	MPlatformOlbSystem::MoveAllFrontTabCarrier(MPos_XYT tgPos[DEF_MAX_TABCARRIER], int iPosID)
{
	int iResult = SUCCESS;
	int i = 0, j = 0;
	BOOL bMoveStart[DEF_MAX_TABCARRIER] = {FALSE, FALSE};
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		if (m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->IsInMotion())
			return m_pCtrlTabCarrier[i]->generateErrorCode(317010);
		if (m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->IsInMotion())
			return m_pCtrlTabCarrier[i]->generateErrorCode(317011);
		if (m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->IsInMotion())
			return m_pCtrlTabCarrier[i]->generateErrorCode(317012);
		if (FALSE == m_pTabCarrier[i]->IsDown())
			return m_pCtrlTabCarrier[i]->generateErrorCode(317013);
	}
	while(1)
	{
		Sleep(10);
		for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
			if (FALSE == bMoveStart[i]) break;
		if (i==-1)
		{
			Sleep(500);
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				BOOL allmovedone = TRUE;
				for (int i=0; i<DEF_MAX_TABCARRIER; i++)
				{
					allmovedone &= (!m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->IsInMotion()
						&& !m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->IsInMotion()
						&& !m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->IsInMotion());
				}
				if (allmovedone)
					break;
				if (GetTickCount() - stime > 20 * 1000)
					return m_pCtrlTabCarrier[0]->generateErrorCode(317015);
			}
			return SUCCESS;
		}

		for (int j=DEF_MAX_TABCARRIER-1; j>=0; j--)
		{
			if (TRUE == bMoveStart[j]) continue;
//			if (MovingAbleFrontTabCarrier(j, tgPos[j]))
			iResult = m_pTabCarrier[j]->CheckCollisionOther(tgPos[j]);
			if (SUCCESS == iResult)
			{
				//iResult = m_pTabCarrier[j]->MoveXYTPos(tgPos[j], -1, TRUE, FALSE);
				iResult = m_pTabCarrier[j]->MoveXYTPos(tgPos[j], iPosID, FALSE, FALSE);
				if (iResult)
					return iResult;
				Sleep(100);
				bMoveStart[j] = TRUE;
				break;
			}
		}

		if (j == -1)
		{
			BOOL bFailed = FALSE;
			for (int j=DEF_MAX_TABCARRIER-1; j>=0; j--)
			{
				if (bMoveStart[j] != 1)
				{
					bFailed = TRUE;
					break;
				}
			}
			if (bFailed)
			{
				return m_pCtrlTabCarrier[0]->generateErrorCode(317014);
			}
		}
	}

	/*
	Sleep(500);

	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		iResult = m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->Wait4Done(TRUE);
		if (iResult)
			return iResult;
		iResult = m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->Wait4Done(TRUE);
		if (iResult)
			return iResult;
		iResult = m_pTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->Wait4Done(TRUE);
		if (iResult)
			return iResult;
	}
	*/

	return SUCCESS;
}

int	MPlatformOlbSystem::MoveAllRearTabCarrier(MPos_XYT tgPos[DEF_MAX_TABCARRIER], int iPosID)
{
	int iResult = SUCCESS;
	int i = 0, j = 0;
	BOOL bMoveStart[DEF_MAX_TABCARRIER] = {FALSE, FALSE};
	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		if (m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->IsInMotion())
			return m_pCtrlRTabCarrier[i]->generateErrorCode(317010);
		if (m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->IsInMotion())
			return m_pCtrlRTabCarrier[i]->generateErrorCode(317011);
		if (m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->IsInMotion())
			return m_pCtrlRTabCarrier[i]->generateErrorCode(317012);
		if (FALSE == m_pRTabCarrier[i]->IsDown())
			return m_pCtrlRTabCarrier[i]->generateErrorCode(317013);
	}
	while(1)
	{
		Sleep(10);
		for (int i=DEF_MAX_TABCARRIER-1; i>=0; i--)
			if (FALSE == bMoveStart[i]) break;
		if (i==-1)
		{
			Sleep(500);
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				BOOL allmovedone = TRUE;
				for (int i=0; i<DEF_MAX_TABCARRIER; i++)
				{
					allmovedone &= (!m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->IsInMotion()
						&& !m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->IsInMotion()
						&& !m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->IsInMotion());
				}
				if (allmovedone)
					break;
				if (GetTickCount() - stime > 20 * 1000)
					return m_pCtrlTabCarrier[0]->generateErrorCode(317015);
			}
			return SUCCESS;
		}

		//for (int j=DEF_MAX_TABCARRIER-1; j>=i; j--)
		for (int j=DEF_MAX_TABCARRIER-1; j>=0; j--)
		{
			if (bMoveStart[j] == 1) continue;
			iResult = m_pRTabCarrier[j]->CheckCollisionOther(tgPos[j]);
			if (SUCCESS == iResult)
			{
				//iResult = m_pRTabCarrier[j]->MoveXYTPos(tgPos[j], -1, TRUE, FALSE);
				iResult = m_pRTabCarrier[j]->MoveXYTPos(tgPos[j], iPosID, FALSE, FALSE);
				if (iResult)
					return iResult;
				Sleep(100);
				bMoveStart[j] = 1;
				break;
			}
		}

		if (j == -1)
		{
			BOOL bFailed = FALSE;
			for (int j=DEF_MAX_TABCARRIER-1; j>=0; j--)
			{
				if (bMoveStart[j] != 1)
				{
					bFailed = TRUE;
					break;
				}
			}
			if (bFailed)
			{
				return m_pCtrlRTabCarrier[0]->generateErrorCode(317014);
			}
		}
	}

	/*
	Sleep(500);

	for (int i=0; i<DEF_MAX_TABCARRIER; i++)
	{
		iResult = m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->Wait4Done(TRUE);
		if (iResult)
			return iResult;
		iResult = m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_Y_AXIS)->Wait4Done(TRUE);
		if (iResult)
			return iResult;
		iResult = m_pRTabCarrier[i]->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->Wait4Done(TRUE);
		if (iResult)
			return iResult;
	}
	*/
	return SUCCESS;
}

int MPlatformOlbSystem::MoveAllTabMounter(double tgPos[DEF_MOUNTER_WORK_PER_GROUP], double tgPosR[DEF_MOUNTER_WORK_PER_GROUP], int iPosID)
{
	int iResult = SUCCESS;
	int i = 0, j = 0;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	BOOL bMoveStart[DEF_MAX_TABMOUNTER] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
		if (m_pTabMounter[i]->GetMotionComponent()->IsInMotion())
			return m_pCtrlTabMounter[i]->generateErrorCode(323020);
		if (FALSE == m_pTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
		{
			//return m_pCtrlTabMounter[i]->generateErrorCode(323021);
			iResult = m_pTabMounter[i]->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS, TRUE, FALSE);
			if (iResult)
				return iResult;
		}
	}
	while(1)
	{
		Sleep(10);
		for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
			if (bMoveStart[i] != 1) break;

		if (i==DEF_MAX_TABMOUNTER)
		{
			Sleep(500);
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				BOOL allmovedone = TRUE;
				for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
				{
					allmovedone &= (!m_pTabMounter[i]->GetMotionComponent()->IsInMotion());
				}
				if (allmovedone)
					break;
				if (GetTickCount() - stime > 20 * 1000)
					return m_pCtrlTabMounter[0]->generateErrorCode(323023);
			}
			return SUCCESS;
		}
		for (int j=i; j<DEF_MAX_TABMOUNTER; j++)
		{
			if (bMoveStart[j] == 1) continue;
			if (j < DEF_MOUNTER_WORK_PER_GROUP)
			{
				iResult = m_pTabMounter[j]->CheckCollisionOtherD(tgPos[j]);
				if (SUCCESS == iResult)
				{
					//iResult = m_pTabMounter[j]->MovePos(tgPos[j], -1, TRUE, FALSE);
					iResult = m_pTabMounter[j]->MovePos(tgPos[j], -1, FALSE, FALSE);
					if (iResult)
						return iResult;
					Sleep(100);
					bMoveStart[j] = 1;
					break;
				}
			}
			else
			{
				iResult = m_pTabMounter[j]->CheckCollisionOtherD(tgPosR[DEF_MAX_TABMOUNTER-1-j]);
				if (SUCCESS == iResult)
				{
					iResult = m_pTabMounter[j]->MovePos(tgPosR[DEF_MAX_TABMOUNTER-1-j], -1, FALSE, FALSE);
					if (iResult)
						return iResult;
					Sleep(100);
					bMoveStart[j] = 1;
					break;
				}
			}
		}

		if (j == DEF_MAX_TABMOUNTER)
		{
			BOOL bFailed = FALSE;
			for (int j=0; j<DEF_MAX_TABMOUNTER; j++)
			{
				if (bMoveStart[j] != 1)
				{
					bFailed = TRUE;
					break;
				}
			}
			if (bFailed)
			{
				return m_pCtrlTabMounter[0]->generateErrorCode(323022);
			}
		}
	}
#else
	/*/

	if (iPosID == DEF_TABMOUNTER_LOAD_POS || iPosID == DEF_TABMOUNTER_READY_POS)
	{
//		m_pTabMounter[0]->SetGantryMotionEnable(TRUE, DEF_MAX_TABCARRIER-1);
//		m_pRTabMounter[0]->SetGantryMotionEnable(TRUE, DEF_MAX_TABCARRIER-1);
		m_pTabMounter[0]->SetGantryMotionEnable(TRUE);
		m_pRTabMounter[0]->SetGantryMotionEnable(TRUE);
	}

	BOOL bMoveFTabMounter = FALSE;
	BOOL bMoveRTabMounter = FALSE;

	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{		
		if (FALSE == m_pTabMounter[i]->IsInPos(tgPos[i]))
			bMoveFTabMounter = TRUE;
		if (FALSE == m_pRTabMounter[i]->IsInPos(tgPosR[i]))
			bMoveRTabMounter = TRUE;
	}

	if (FALSE == bMoveFTabMounter && FALSE == bMoveRTabMounter)
	{
//		m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
//		m_pRTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
		m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
		m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
		return SUCCESS;
	}

	BOOL bFStartMove[DEF_MAX_TABMOUNTER] = {FALSE, FALSE};
	BOOL bRStartMove[DEF_MAX_TABMOUNTER] = {FALSE, FALSE};

	for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
	{
		if (m_pTabMounter[i]->GetMotionComponent()->IsInMotion())
		{
//			m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
//			m_pRTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
			m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
			m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
			return m_pCtrlTabMounter[i]->generateErrorCode(323020);
		}
		if (FALSE == m_pTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
		{
			//return m_pCtrlTabMounter[i]->generateErrorCode(323021);
			iResult = m_pTabMounter[i]->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS, TRUE, FALSE);
			if (iResult)
			{
//				m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
//				m_pRTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
				m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
				m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
				return iResult;
			}
		}

	}

	DWORD AllSTime = GetTickCount();
	while(1)
	{
		Sleep(10);
		BOOL bAllStartMove = TRUE;
		for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
		{
			if (FALSE == bFStartMove[i] || FALSE == bRStartMove[i])
			{
				bAllStartMove = FALSE;
				break;
			}
		}

		if (TRUE == bAllStartMove)
		{
			Sleep(500);
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				BOOL allmovedone = TRUE;
				for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
				{
					allmovedone &= !m_pTabMounter[i]->GetMotionComponent()->IsInMotion();
				}
				if (allmovedone)
					break;
				if (GetTickCount() - stime > 20 * 1000)
				{
//					m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
//					m_pRTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
					m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
					m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
					return m_pCtrlTabMounter[0]->generateErrorCode(323023);
				}
			}
//			m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
//			m_pRTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
			m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
			m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
			return SUCCESS;
		}

		for (int i=0; i<DEF_MAX_TABMOUNTER; i++)
		{
			if (FALSE == bFStartMove[i])
			{
// 				iResult = m_pTabMounter[i]->CheckCollisionOtherD(tgPos[i]);
// 				if (SUCCESS == iResult)
				{
					iResult = m_pTabMounter[i]->MovePos(tgPos[i], -1, FALSE, FALSE);
					if (iResult)
					{
//						m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
//						m_pRTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
						m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
						m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
						return iResult;
					}
					Sleep(100);
					bFStartMove[i] = TRUE;
				}
			}
			if (FALSE == bRStartMove[i])
			{
				bRStartMove[i] = TRUE;

			}
		}

		if (GetTickCount() - AllSTime > 20 * 1000)
		{
//			m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
//			m_pRTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
			m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
			m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
			return m_pCtrlTabMounter[0]->generateErrorCode(323022);
		}
	}
//@ #endif
	return SUCCESS;
}

int MPlatformOlbSystem::MoveAllFrontTabMounter(double tgPos[DEF_MOUNTER_WORK_PER_GROUP], int iPosID)
{
	int iResult = SUCCESS;
	int i = 0, j = 0;
	BOOL bMoveStart[DEF_MOUNTER_WORK_PER_GROUP] = {FALSE, FALSE};
	if (iPosID == DEF_TABMOUNTER_LOAD_POS || iPosID == DEF_TABMOUNTER_READY_POS)
	{
//		m_pTabMounter[0]->SetGantryMotionEnable(TRUE, DEF_MAX_TABCARRIER-1);
		m_pTabMounter[0]->SetGantryMotionEnable(TRUE);
	}

	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (m_pTabMounter[i]->GetMotionComponent()->IsInMotion())
		{
//			m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
			m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
			return m_pCtrlTabMounter[i]->generateErrorCode(323020);
		}
		if (FALSE == m_pTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
		{
			//return m_pCtrlTabMounter[i]->generateErrorCode(323021);
			iResult = m_pTabMounter[i]->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS, TRUE, FALSE);
			if (iResult)
			{
//				m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
				m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
				return iResult;
			}
		}
	}
	DWORD sStarttime = GetTickCount();
	while(1)
	{
		Sleep(10);
		for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
			if (bMoveStart[i] != 1) break;

		if (i==DEF_MOUNTER_WORK_PER_GROUP)
		{
			Sleep(500);
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				BOOL allmovedone = TRUE;
				for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
					allmovedone &= (!m_pTabMounter[i]->GetMotionComponent()->IsInMotion());
				if (allmovedone)
					break;
				if (GetTickCount() - stime > 20 * 1000)
				{
//					m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
					m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
					return m_pCtrlTabMounter[0]->generateErrorCode(323023);
				}
			}
//			m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
			m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
			return SUCCESS;
		}

		for (int j=i; j<DEF_MOUNTER_WORK_PER_GROUP; j++)
		{
			if (bMoveStart[j] == 1) continue;

			// Doolin kwangilshin 2017. 08. 28.
			iResult = m_pTabMounter[j]->CheckCollisionOtherD(tgPos[j]);
 			// End.
			if (SUCCESS == iResult)
			{
				//iResult = m_pTabMounter[j]->MovePos(tgPos[j], -1, TRUE, FALSE);
				iResult = m_pTabMounter[j]->MovePos(tgPos[j], iPosID, FALSE, FALSE);
				if (iResult)
				{
//					m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
					m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
					return iResult;
				}
				
				Sleep(100);
				bMoveStart[j] = 1;
				break;
			}
// 			else if(iResult != 1)
// 			{
// 				return iResult;
// 			}
		}
		if (GetTickCount() - sStarttime > 30 * 1000)
		{
			//					m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
			m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
			return m_pCtrlTabMounter[0]->generateErrorCode(323022);
		}
	}

// 		if (j == DEF_MOUNTER_WORK_PER_GROUP)
// 		{
// 			BOOL bFailed = FALSE;
// 			for (int j=0; j<DEF_MOUNTER_WORK_PER_GROUP; j++)
// 			{
// 				if (bMoveStart[j] != 1)
// 				{
// 					bFailed = TRUE;
// 					break;
// 				}
// 			}
// 			if (bFailed)
// 			{
// //				m_pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
// 				m_pTabMounter[0]->SetGantryMotionEnable(FALSE);
// 				return m_pCtrlTabMounter[0]->generateErrorCode(323022);
// 			}
// 		}
// 	}

	/*
	Sleep(500);

	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		iResult = m_pTabMounter[i]->Wait4Done(TRUE);
		if (iResult)
			return iResult;
	}
	*/

	return SUCCESS;
}

int MPlatformOlbSystem::MoveAllRearTabMounter(double tgPos[DEF_MOUNTER_WORK_PER_GROUP], int iPosID)
{
	return SUCCESS;

	int iResult = SUCCESS;
	int i = 0, j = 0;
	BOOL bMoveStart[DEF_MOUNTER_WORK_PER_GROUP] = {FALSE, FALSE};
	if (iPosID == DEF_TABMOUNTER_LOAD_POS || iPosID == DEF_TABMOUNTER_READY_POS)
	{
//		pTabMounter[0]->SetGantryMotionEnable(TRUE, DEF_MAX_TABCARRIER-1);
		m_pRTabMounter[0]->SetGantryMotionEnable(TRUE);
	}

	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (m_pRTabMounter[i]->GetMotionComponent()->IsInMotion())
		{
//			pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
			m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
			return m_pCtrlRTabMounter[i]->generateErrorCode(323020);
		}
		if (FALSE == m_pRTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
		{
			//return pCtrlTabMounter[i]->generateErrorCode(323021);
			iResult = m_pRTabMounter[i]->SafeMoveZPos(DEF_TABMOUNTER_Z_MOVE_POS, TRUE, FALSE);
			if (iResult)
			{
//				pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
				m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
				return iResult;
			}
		}
	}
	while(1)
	{
		Sleep(10);
		for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
			if (bMoveStart[i] != 1) break;

		if (i==DEF_MOUNTER_WORK_PER_GROUP)
		{
			Sleep(500);
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				BOOL allmovedone = TRUE;
				for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
					allmovedone &= (!m_pRTabMounter[i]->GetMotionComponent()->IsInMotion());
				if (allmovedone)
					break;
				if (GetTickCount() - stime > 20 * 1000)
				{
//					pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
					m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
					return m_pCtrlRTabMounter[0]->generateErrorCode(323023);
				}
			}
//			pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
			m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
			return SUCCESS;
		}

		for (int j=i; j<DEF_MOUNTER_WORK_PER_GROUP; j++)
		{
			if (bMoveStart[j] == 1) continue;
// 			iResult = m_pRTabMounter[j]->CheckCollisionOtherD(tgPos[j]);
// 			if (SUCCESS == iResult)
			{
				//iResult = pTabMounter[j]->MovePos(tgPos[j], -1, TRUE, FALSE);
				iResult = m_pRTabMounter[j]->MovePos(tgPos[j], iPosID, FALSE, FALSE);
				if (iResult)
				{
//					pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
					m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
					return iResult;
				}
				Sleep(100);
				bMoveStart[j] = 1;
				break;
			}
		}

		if (j == DEF_MOUNTER_WORK_PER_GROUP)
		{
			BOOL bFailed = FALSE;
			for (int j=0; j<DEF_MOUNTER_WORK_PER_GROUP; j++)
			{
				if (bMoveStart[j] != 1)
				{
					bFailed = TRUE;
					break;
				}
			}
			if (bFailed)
			{
//				pTabMounter[0]->SetGantryMotionEnable(FALSE, DEF_MAX_TABCARRIER-1);
				m_pRTabMounter[0]->SetGantryMotionEnable(FALSE);
				return m_pCtrlRTabMounter[0]->generateErrorCode(323022);
			}
		}
	}	
	/*
	Sleep(500);

	for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		iResult = pTabMounter[i]->Wait4Done(TRUE);
		if (iResult)
			return iResult;
	}
	*/

	return SUCCESS;
}

int	MPlatformOlbSystem::MoveAllTabMounterZ(int iWorkGroup, int iPosID)
{
	MTabMounter* pTabMounter[DEF_MOUNTER_WORK_PER_GROUP];
	if (iWorkGroup == DEF_FRONT_GROUP)
	{
		for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
			pTabMounter[i] = m_pTabMounter[i];
	}
	else
	{
		for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
			pTabMounter[i] = m_pRTabMounter[i];
	}
	
	int iResult = 0;
	int i = 0;
	BOOL bCylDown = FALSE;
	if (iPosID == DEF_TABMOUNTER_Z_LOAD_POS
		|| iPosID == DEF_TABMOUNTER_Z_R_LOAD_POS
		|| iPosID == DEF_TABMOUNTER_Z_INSPECTION_POS
		|| iPosID == DEF_TABMOUNTER_Z_MOUNT_POS)
		bCylDown = TRUE;

	for (int i = 0; i < DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		if (TRUE == bCylDown)
			pTabMounter[i]->DownCyl(TRUE);
		iResult = pTabMounter[i]->SafeMoveZPos(iPosID, FALSE, FALSE);
		if (iResult)
			return iResult;
	}
	for (int i = 0; i < DEF_MOUNTER_WORK_PER_GROUP; i++)
	{
		iResult = pTabMounter[i]->Wait4DoneZ();
		if (iResult)
			return iResult;
	}
	return SUCCESS;
}

int MPlatformOlbSystem::MoveAllGroupCameraCarrier(int iWorkGroup, double tgPos[DEF_MAX_INSPECTION_CAM_CARRIER])
{
	int iResult = SUCCESS;
	int i = 0, j = 0;
	MCameraCarrier *pCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	MCtrlInspectionCamera *pCtrlCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];

	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (iWorkGroup == DEF_FRONT_GROUP)
		{
			pCamCarrier[i] = m_pCameraCarrier[i];
			pCtrlCamCarrier[i] = m_pCtrlInspectionCamera[i];
		}
		else
		{
			pCamCarrier[i] = m_pRCameraCarrier[i];
			pCtrlCamCarrier[i] = m_pCtrlRInspectionCamera[i];
		}
	}

	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{		
		if(iWorkGroup == DEF_REAR_GROUP)
		{
			if (FALSE == pCamCarrier[0]->IsInPos(tgPos[0]))
				break;
		}
		else
			if (FALSE == pCamCarrier[i]->IsInPos(tgPos[i]))
				break;

	}

	if (i == DEF_MAX_INSPECTION_CAM_CARRIER)
		return SUCCESS;

	BOOL bMoveStart[DEF_MAX_INSPECTION_CAM_CARRIER] = {FALSE, FALSE};
	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if(iWorkGroup == DEF_REAR_GROUP)
		{
			if (pCamCarrier[0]->GetMotionComponent()->IsInMotion())
				return pCtrlCamCarrier[0]->generateErrorCode(324110);
		}
		else
		{
			if (pCamCarrier[i]->GetMotionComponent()->IsInMotion())
				return pCtrlCamCarrier[i]->generateErrorCode(324110);
			if (FALSE == pCamCarrier[i]->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS))
			{
				//return m_pCtrlInspectionCamera[i]->generateErrorCode(324111);
				iResult = pCamCarrier[i]->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
				if (iResult)
					return iResult;
			}
		}
	}

	while(1)
	{
		Sleep(10);
		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			if (bMoveStart[i] != 1) break;

		if (i==DEF_MAX_INSPECTION_CAM_CARRIER)
		{
			Sleep(500);
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				BOOL allmovedone = TRUE;
				for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
				{
					if(iWorkGroup == DEF_REAR_GROUP)
						allmovedone &= (!pCamCarrier[0]->GetMotionComponent()->IsInMotion());
					else
						allmovedone &= (!pCamCarrier[i]->GetMotionComponent()->IsInMotion());
				}
				if (allmovedone)
					break;
				if (GetTickCount() - stime > 20 * 1000)
					return pCtrlCamCarrier[0]->generateErrorCode(324113);
			}
			return SUCCESS;
		}

		for (int j=i; j<DEF_MAX_INSPECTION_CAM_CARRIER; j++)
		{
			if (bMoveStart[j] == 1) continue;
//			if (MovingAbleFrontTabCarrier(j, tgPos[j]))
			if(iWorkGroup == DEF_REAR_GROUP)
			{
				if(j== 0)
				{
					iResult = pCamCarrier[0]->MovePos(tgPos[0], -1, FALSE, FALSE);
					if (iResult)
						return iResult;
					Sleep(100);
					bMoveStart[0] = 1;
					break;
				}
				else 
					bMoveStart[j] = 1;
			}
			else
			{
				iResult = pCamCarrier[j]->CheckCollisionOtherD(tgPos[j]);
				if (SUCCESS == iResult)
				{
					//iResult = pCamCarrier[j]->MovePos(tgPos[j], -1, TRUE, FALSE);
					iResult = pCamCarrier[j]->MovePos(tgPos[j], -1, FALSE, FALSE);
					if (iResult)
						return iResult;
					Sleep(100);
					bMoveStart[j] = 1;
					break;
				}
			}


		}

		if (j == DEF_MAX_INSPECTION_CAM_CARRIER)
		{
			BOOL bFailed = FALSE;
			for (int j=0; j<DEF_MAX_INSPECTION_CAM_CARRIER; j++)
			{
				if (bMoveStart[j] != 1)
				{
					bFailed = TRUE;
					break;
				}
			}
			if (bFailed)
			{
				return pCtrlCamCarrier[0]->generateErrorCode(324112);
			}
		}
	}
	
	/*
	Sleep(500);

	for (int i=0; i<DEF_MAX_CAMERACARRIER; i++)
	{
		iResult = pCamCarrier[i]->Wait4Done();
		if (iResult)
			return iResult;
	}
	*/
	return SUCCESS;
}

//Gate에서만
int	MPlatformOlbSystem::MoveAllCameraCarrier(double tgFPos[DEF_MAX_INSPECTION_CAM_CARRIER], double tgRPos[DEF_MAX_INSPECTION_CAM_CARRIER])
{
	int iResult = SUCCESS;
	int i = 0, j = 0;
//	m_pCameraCarrier[i];
//	m_pCtrlInspectionCamera[i];
//	m_pRCameraCarrier[i];
//	m_pCtrlRInspectionCamera[i];
	BOOL bMoveFCamCarrier = FALSE;
	BOOL bMoveRCamCarrier = FALSE;

	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{		
		if (FALSE == m_pCameraCarrier[i]->IsInPos(tgFPos[i]))
			bMoveFCamCarrier = TRUE;
		if (FALSE == m_pRCameraCarrier[i]->IsInPos(tgRPos[i]))
			bMoveRCamCarrier = TRUE;
	}

	if (FALSE == bMoveFCamCarrier && FALSE == bMoveRCamCarrier)
		return SUCCESS;

	BOOL bFStartMove[DEF_MAX_INSPECTION_CAM_CARRIER] = {FALSE, FALSE};
	BOOL bRStartMove[DEF_MAX_INSPECTION_CAM_CARRIER] = {FALSE, FALSE};

	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		if (m_pCameraCarrier[i]->GetMotionComponent()->IsInMotion())
			return m_pCtrlInspectionCamera[i]->generateErrorCode(324110);
		if (FALSE == m_pCameraCarrier[i]->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS))
		{
			iResult = m_pCameraCarrier[i]->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
				return iResult;
		}

		if(i==0)
		{
			if (m_pRCameraCarrier[i]->GetMotionComponent()->IsInMotion())
				return m_pCtrlRInspectionCamera[i]->generateErrorCode(324110);
			if (FALSE == m_pRCameraCarrier[i]->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS))
			{
				iResult = m_pRCameraCarrier[i]->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
				if (iResult)
					return iResult;
			}
		}
	}

	DWORD AllSTime = GetTickCount();
	while(1)
	{
		Sleep(10);
		BOOL bAllStartMove = TRUE;
		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			if (FALSE == bFStartMove[i] || FALSE == bRStartMove[i])
			{
				bAllStartMove = FALSE;
				break;
			}
		}

		if (TRUE == bAllStartMove)
		{
			Sleep(500);
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				BOOL allmovedone = TRUE;
				for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
				{
					allmovedone &= (!m_pCameraCarrier[i]->GetMotionComponent()->IsInMotion() && !m_pRCameraCarrier[0]->GetMotionComponent()->IsInMotion());
				}
				if (allmovedone)
					break;
				if (GetTickCount() - stime > 20 * 1000)
					return m_pCtrlInspectionCamera[0]->generateErrorCode(324113);
			}
			return SUCCESS;
		}

		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			if (FALSE == bFStartMove[i])
			{
				iResult = m_pCameraCarrier[i]->CheckCollisionOtherD(tgFPos[i]);
				if (SUCCESS == iResult)
				{
					iResult = m_pCameraCarrier[i]->MovePos(tgFPos[i], -1, FALSE, FALSE);
					if (iResult)
						return iResult;
					Sleep(100);
					bFStartMove[i] = TRUE;
				}
			}
			if (FALSE == bRStartMove[i])
			{
				if(i==0)
				{
					iResult = m_pRCameraCarrier[i]->MovePos(tgRPos[i], -1, FALSE, FALSE);
					if (iResult)
						return iResult;
					Sleep(100);
					bRStartMove[i] = TRUE;
				}
				else bRStartMove[i] = TRUE;
			}
		}

		if (GetTickCount() - AllSTime > 20 * 1000)
		{
			return m_pCtrlInspectionCamera[0]->generateErrorCode(324112);
		}
	}
	return SUCCESS;
}

int	MPlatformOlbSystem::MoveInspectCameraPanelAlignPos()
{
	int iResult = SUCCESS;
	double tgPos[DEF_MAX_INSPECTION_CAM_CARRIER] = {0.0, };
	double tgRPos[DEF_MAX_INSPECTION_CAM_CARRIER] = {0.0, };
	int iWorkGroup = DEF_FRONT_GROUP;

	MCameraCarrier *pCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	MCameraCarrier *pRCamCarrier[DEF_MAX_INSPECTION_CAM_CARRIER];
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		pCamCarrier[i] = GetCameraCarrierComponent(i);
		pRCamCarrier[i] = GetRCameraCarrierComponent(i);
	}		

	BOOL bMove1 = FALSE;
	BOOL bMove2 = FALSE;

	//Fidumark로 PanelAlign할때
	if (FALSE == m_pObjPreBonderData->m_bUseTabPanelAlign)
	{
		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			tgPos[i] = pCamCarrier[i]->GetTargetPos(0, DEF_CAMERACARRIER_PANEL_MARK_POS);
			tgRPos[i] = pRCamCarrier[i]->GetTargetPos(0, DEF_CAMERACARRIER_PANEL_MARK_POS);
		}

		BOOL bWait[2] = {FALSE, FALSE};
		//120210.kms_________
		//Inspection Camera 가 부상안에 있을 경우를 대비 하여 Inspection Camera 위치로 이동한 후에 
		//Model Change 가이동하도록 한다.
		iResult = MoveAllCameraCarrier(tgPos, tgRPos);
		if (iResult)
			return iResult;
		//___________________
		
		if (FALSE == m_pModelChange[0]->IsInYPos(DEF_MODEL_CHANGE_PANELALIGN_POS))
		{
			bMove1 = TRUE;
			iResult = m_pModelChange[0]->SafeMoveYPos(DEF_MODEL_CHANGE_PANELALIGN_POS, TRUE, FALSE);
			if (iResult)
				return iResult;
			bWait[0] = TRUE;
		}
		if (FALSE == m_pModelChange[1]->IsInYPos(DEF_MODEL_CHANGE_PANELALIGN_POS))
		{
			bMove2 = TRUE;
			iResult = m_pModelChange[1]->SafeMoveYPos(DEF_MODEL_CHANGE_PANELALIGN_POS, TRUE, FALSE);
			if (iResult)
				return iResult;
			bWait[1] = TRUE;
		}		
		//120210.kms_________ 시점 이동
		/*
		iResult = MoveAllCameraCarrier(tgPos, tgRPos);
		if (iResult)
			return iResult;
			*/

		for (int i=0; i<2; i++)
		{
			if (bWait[i])
			{
				iResult = m_pModelChange[i]->Wait4DoneY();
				if (iResult)
					return iResult;
			}
		}
	}
	//TabMark로 PanelAlign할때
	else
	{
//@		if (GetPanelData()->m_bUseFlag_Yl && GetPanelData()->m_bUseFlag_Yr)

		bMove1 = FALSE;
		bMove2 = FALSE;
		if (TRUE == GetUseFlagPanel(DEF_FRONT_GROUP) && TRUE == GetUseFlagPanel(DEF_REAR_GROUP))
		{
			BOOL bWait[2] = {FALSE, FALSE};
			if (FALSE == m_pModelChange[DEF_FRONT_GROUP]->IsInYPos(DEF_MODEL_CHANGE_MODEL_POS))
			{
				bMove1 = TRUE;
				iResult = m_pModelChange[DEF_FRONT_GROUP]->SafeMoveYPos(DEF_MODEL_CHANGE_MODEL_POS, TRUE, FALSE);
				if (iResult)
					return iResult;
				bWait[0] = TRUE;
			}
			if (FALSE == m_pModelChange[DEF_REAR_GROUP]->IsInYPos(DEF_MODEL_CHANGE_MODEL_POS))
			{
				bMove2 = TRUE;
				iResult = m_pModelChange[DEF_REAR_GROUP]->SafeMoveYPos(DEF_MODEL_CHANGE_MODEL_POS, TRUE, FALSE);
				if (iResult)
					return iResult;
				bWait[1] = TRUE;
			}
#ifdef DEF_SOURCE_SYSTEM
			MVarList<MInspectCarrierInfo> InspectCarrierList = m_pObjWorkScheduleSecond[DEF_FRONT_GROUP]->GetFirstInspectionCarrierList();
			MVarList<MInspectCarrierInfo> RInspectCarrierList = m_pObjWorkScheduleSecond[DEF_REAR_GROUP]->GetFirstInspectionCarrierList();
#else
			MVarList<MInspectCarrierInfo> InspectCarrierList = m_pObjWorkSchedule[DEF_FRONT_GROUP]->GetFirstInspectionCarrierList();
			MVarList<MInspectCarrierInfo> RInspectCarrierList = m_pObjWorkSchedule[DEF_REAR_GROUP]->GetFirstInspectionCarrierList();
#endif
			MInspectCarrierInfo InspectCarrierInfo;
			for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			{
				pCamCarrier[i]->ExpandShrinkForTabAlign(TRUE);
				InspectCarrierInfo = InspectCarrierList.GetInfo(i);
				tgPos[i] = pCamCarrier[i]->CalculateInspectPos(0, InspectCarrierInfo.m_dInspectPos);
				InspectCarrierInfo = RInspectCarrierList.GetInfo(i);
				tgRPos[i] = pRCamCarrier[i]->CalculateInspectPos(0, InspectCarrierInfo.m_dInspectPos);				
			}
			iResult = MoveAllCameraCarrier(tgPos, tgRPos);
			if (iResult)
				return iResult;

			for (int i=0; i<2; i++)
			{
				if (bWait[i])
				{
					iResult = m_pModelChange[i]->Wait4DoneY();
					if (iResult)
						return iResult;
				}
			}
		}
		else
		{
			BOOL bWait[2] = {FALSE, FALSE};
			if (TRUE == GetUseFlagPanel(DEF_FRONT_GROUP))
			{
				if (FALSE == m_pModelChange[DEF_FRONT_GROUP]->IsInYPos(DEF_MODEL_CHANGE_MODEL_POS))
				{
					bMove1 = TRUE;
					iResult = m_pModelChange[DEF_FRONT_GROUP]->SafeMoveYPos(DEF_MODEL_CHANGE_MODEL_POS, TRUE, FALSE);
					if (iResult)
						return iResult;
					bWait[0] = TRUE;
				}
#ifdef DEF_SOURCE_SYSTEM
				MVarList<MInspectCarrierInfo> InspectCarrierList = m_pObjWorkScheduleSecond[DEF_FRONT_GROUP]->GetFirstInspectionCarrierList();
#else
				MVarList<MInspectCarrierInfo> InspectCarrierList = m_pObjWorkSchedule[DEF_FRONT_GROUP]->GetFirstInspectionCarrierList();
#endif
				MInspectCarrierInfo InspectCarrierInfo;
				for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
				{
					pCamCarrier[i]->ExpandShrinkForTabAlign(TRUE);
					InspectCarrierInfo = InspectCarrierList.GetInfo(i);
					tgPos[i] = pCamCarrier[i]->CalculateInspectPos(0, InspectCarrierInfo.m_dInspectPos);
				}
				iWorkGroup = DEF_FRONT_GROUP;
			}	
			else if (TRUE == GetUseFlagPanel(DEF_REAR_GROUP))
			{
				if (FALSE == m_pModelChange[DEF_REAR_GROUP]->IsInYPos(DEF_MODEL_CHANGE_MODEL_POS))
				{
					bMove2 = TRUE;
					iResult = m_pModelChange[DEF_REAR_GROUP]->SafeMoveYPos(DEF_MODEL_CHANGE_MODEL_POS, TRUE, FALSE);
					if (iResult)
						return iResult;
					bWait[1] = TRUE;
				}
#ifdef DEF_SOURCE_SYSTEM
				MVarList<MInspectCarrierInfo> InspectCarrierList = m_pObjWorkScheduleSecond[DEF_REAR_GROUP]->GetFirstInspectionCarrierList();
#else
				MVarList<MInspectCarrierInfo> InspectCarrierList = m_pObjWorkSchedule[DEF_REAR_GROUP]->GetFirstInspectionCarrierList();
#endif
				MInspectCarrierInfo InspectCarrierInfo;
				for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
				{
					pRCamCarrier[i]->ExpandShrinkForTabAlign(TRUE);
					InspectCarrierInfo = InspectCarrierList.GetInfo(i);
					tgPos[i] = pRCamCarrier[i]->CalculateInspectPos(0, InspectCarrierInfo.m_dInspectPos);
				}
				iWorkGroup = DEF_REAR_GROUP;
			}
			else
				return pCamCarrier[0]->generateErrorCode(324114);

			iResult = MoveAllGroupCameraCarrier(iWorkGroup, tgPos);
			if (iResult)
				return iResult;

			for (int i=0; i<2; i++)
			{
				if (bWait[i])
				{
					iResult = m_pModelChange[i]->Wait4DoneY();
					if (iResult)
						return iResult;
				}
			}
		}
	}
	return iResult;
}
//_______________________

//101004.KKY.자동운전중 신규모델 등록_____
int MPlatformOlbSystem::CreateTmpModelData(CString strModelName)
{
	CreateTmpPanelData(strModelName);
	CreateTmpPreBonderData(strModelName);
	CreateTmpTabData(strModelName);
	return SUCCESS;
}

int MPlatformOlbSystem::CreateTmpPanelData(CString strModelName)
{
	if (m_pObjTmpPanelData)
		delete m_pObjTmpPanelData;
	
	CString	strModelFileDir;
	strModelFileDir.Format(_T("%s\\%s"), m_pObjSystemData->m_strModelFileDir, strModelName);
	m_pObjTmpPanelData	= new MPanelData(DEF_SYSTEM_PANEL_DATA_FILE_NAME, strModelFileDir);
	m_pObjTmpPanelData->ReadData();
	return SUCCESS;
}

int MPlatformOlbSystem::CreateTmpPreBonderData(CString strModelName)
{
	if (m_pObjTmpPreBonderData)
		delete m_pObjTmpPreBonderData;
	CString	strModelFileDir;
	strModelFileDir.Format(_T("%s\\%s"), m_pObjSystemData->m_strModelFileDir, strModelName);
	m_pObjTmpPreBonderData	= new MPreBonderData(DEF_SYSTEM_PRE_DATA_FILE_NAME, strModelFileDir);
	m_pObjTmpPreBonderData->ReadData();
	return SUCCESS;
}

int MPlatformOlbSystem::CreateTmpTabData(CString strModelName)
{
	if (m_pObjTmpTabData)
		delete m_pObjTmpTabData;
	CString	strModelFileDir;
	strModelFileDir.Format(_T("%s\\%s"), m_pObjSystemData->m_strModelFileDir, strModelName);
	m_pObjTmpTabData = new MTabData(DEF_SYSTEM_TAB_DATA_FILE_NAME, strModelFileDir);
	m_pObjTmpTabData->ReadData();
	return SUCCESS;
}

MPanelData* MPlatformOlbSystem::GetTmpPanelData()
{
	ASSERT(m_pObjTmpPanelData != NULL);
	return m_pObjTmpPanelData;
}

MPreBonderData* MPlatformOlbSystem::GetTmpPreBonderData()
{
	ASSERT(m_pObjTmpPreBonderData != NULL);
	return m_pObjTmpPreBonderData;
}

MTabData* MPlatformOlbSystem::GetTmpTabData()
{
	ASSERT(m_pObjTmpTabData != NULL);
	return m_pObjTmpTabData;
}
//________________________________________


//PanelAligner회전중심에서 Panel끝단까지 거리(+)
double MPlatformOlbSystem::GetDistance_StageCenterToPanelEdge_X()
{
//@	double dDist = (DEF_PANELALIGNER_DIST_CENTERTOEDGE + DEF_PANELALIGNER_DIST_PROJECT);
//@	return dDist;

	//@PanelAligner 중심과 Panel중심을 일치시킨다
#ifdef DEF_SOURCE_SYSTEM
	double dDist = m_pObjPanelData->m_dPanelSize_X/2.0 - DEF_PANELALIGNER_DIST_CENTERTOCENTER/2.0;
#else
	double dDist = m_pObjPanelData->m_dPanelSize_Y/2.0 - DEF_PANELALIGNER_DIST_CENTERTOCENTER/2.0;
#endif
	return dDist;
}

//PanelAligner회전중심에서 Panel Mark까지 거리(+)
double MPlatformOlbSystem::GetDistance_StageCenterToPanelMark_X()
{
#ifdef DEF_SOURCE_SYSTEM
	double dDist = GetDistance_StageCenterToPanelEdge_X() - m_pObjPanelData->m_dX_FiduMark_Yl;
#else
	double dDist = GetDistance_StageCenterToPanelEdge_X() - m_pObjPanelData->m_dY_FiduMark_Xu;
#endif
	return dDist;
}

//PanelAligner회전중심에서 Panel중심까지 거리(+) ---> 편심량
double MPlatformOlbSystem::GetEccentricDist_StageToPanel_X()
{
#ifdef DEF_SOURCE_SYSTEM
	double dDist = m_pObjPanelData->m_dPanelSize_X/2.0 - GetDistance_StageCenterToPanelEdge_X();
#else
	double dDist = m_pObjPanelData->m_dPanelSize_Y/2.0 - GetDistance_StageCenterToPanelEdge_X();
#endif
	return dDist;
}

//PanelAligner Plate중심에서 Panel Mark까지 거리(+)
double MPlatformOlbSystem::GetDistance_StagePlateCenterToPanelMark_X()
{
#ifdef DEF_SOURCE_SYSTEM
	double dDist = m_pObjPanelData->m_dPanelSize_X/2.0 - m_pObjPanelData->m_dX_FiduMark_Yl;
#else
	double dDist = m_pObjPanelData->m_dPanelSize_Y/2.0 - m_pObjPanelData->m_dX_FiduMark_Xu;
#endif
	return dDist;
}

//TabAligner회전중심에서 TabICMark까지 거리
double MPlatformOlbSystem::GetDistance_TabAlignerCenterToTabMarkX()
{
#ifdef DEF_SOURCE_SYSTEM
	double dTabEdgeToMark = m_pObjPreBonderData->m_dDistTabEgdeToMark_Xtab;
#else
	double dTabEdgeToMark = m_pObjPreBonderData->m_dDistTabEgdeToMark_Ytab;
#endif
	return (DEF_TABCARRIER_SHIFT_Y - dTabEdgeToMark);
}

//TabAligner회전중심에서 TabICMark까지 거리
double MPlatformOlbSystem::GetDistance_TabAlignerCenterToTabMarkY()
{
	double dDistance = m_pObjPreBonderData->m_dLead_Width_Xtab;
#ifdef DEF_GATE_SYSTEM
	double dDiatance = m_pObjPreBonderData->m_dLead_Width_Ytab;
#endif
	return (dDistance/2.0*(-1.0));
}

//FiduMark에서 지정 Tab의 중심까지의 거리 구하기
double MPlatformOlbSystem::GetXDistance_FiduMarkToTabCenter(EWorkingSide eWorkSide, int iTabNo)
{
	double dSum = 0.0;
	int i = 0;
	switch (eWorkSide)
	{
	case WORKING_SIDE_X_UP:
		for (int i=0; i<=iTabNo; i++)
			dSum += m_pObjTabData->m_dXuTabPitch[i];
		break;
	case WORKING_SIDE_X_DOWN:
		for (int i=0; i<=iTabNo; i++)
			dSum += m_pObjTabData->m_dXdTabPitch[i];
		break;
	case WORKING_SIDE_Y_LEFT:
		for (int i=0; i<=iTabNo; i++)
			dSum += m_pObjTabData->m_dYlTabPitch[i];
		break;
	case WORKING_SIDE_Y_RIGHT:
		for (int i=0; i<=iTabNo; i++)
			dSum += m_pObjTabData->m_dYrTabPitch[i];
		break;
	}
	return dSum;
}

//FiduMark에서 지정 Tab의 LeftMark까지의 거리 구하기
double MPlatformOlbSystem::GetXDistance_FiduMarkToTabMarkL(EWorkingSide eWorkSide, int iTabNo)
{
	double dTabMarkLen = 0.0;
	if (eWorkSide == WORKING_SIDE_X_UP || eWorkSide == WORKING_SIDE_X_DOWN)
		dTabMarkLen = m_pObjPreBonderData->m_dLead_Width_Xtab;
	else
		dTabMarkLen = m_pObjPreBonderData->m_dLead_Width_Ytab;

	double dDistance = GetXDistance_FiduMarkToTabCenter(eWorkSide, iTabNo) - dTabMarkLen/2.0;
	return dDistance;
}

//FiduMark에서 지정 Tab의 RightMark까지의 거리 구하기
double MPlatformOlbSystem::GetXDistance_FiduMarkToTabMarkR(EWorkingSide eWorkSide, int iTabNo)
{
	double dTabMarkLen = 0.0;
	if (eWorkSide == WORKING_SIDE_X_UP || eWorkSide == WORKING_SIDE_X_DOWN)
		dTabMarkLen = m_pObjPreBonderData->m_dLead_Width_Xtab;
	else
		dTabMarkLen = m_pObjPreBonderData->m_dLead_Width_Ytab;

	double dDistance = GetXDistance_FiduMarkToTabCenter(eWorkSide, iTabNo) + dTabMarkLen/2.0;
	return dDistance;
}

//FirstTab 중심에서 SecondTab 중심 까지의 거리 구하기
double MPlatformOlbSystem::GetXDistance_TabCenterToCenter(EWorkingSide eWorkSide, int iStartTabNo, int iLastTabNo)
{
//@	double dDistance = GetXDistance_FiduMarkToTabCenter(eWorkSide, iLastTabNo)
//@					- GetXDistance_FiduMarkToTabCenter(eWorkSide, iStartTabNo);
//@	return dDistance;
	double dSum = 0.0;
	int i = 0;
	switch (eWorkSide)
	{
	case WORKING_SIDE_X_UP:
		for (int i=iStartTabNo+1; i<=iLastTabNo; i++)
			dSum += m_pObjTabData->m_dXuTabPitch[i];
		break;
	case WORKING_SIDE_X_DOWN:
		for (int i=iStartTabNo+1; i<=iLastTabNo; i++)
			dSum += m_pObjTabData->m_dXdTabPitch[i];
		break;
	case WORKING_SIDE_Y_LEFT:
		for (int i=iStartTabNo+1; i<=iLastTabNo; i++)
			dSum += m_pObjTabData->m_dYlTabPitch[i];
		break;
	case WORKING_SIDE_Y_RIGHT:
		for (int i=iStartTabNo+1; i<=iLastTabNo; i++)
			dSum += m_pObjTabData->m_dYrTabPitch[i];
		break;
	}
	return dSum;
}

//부상길이와 작업변 길이의 차이를 구한다.
double MPlatformOlbSystem::GetDifferLen_PlateToPanelLen()
{
#ifdef DEF_SOURCE_SYSTEM
	double dMarkDist = m_pObjPanelData->m_dDistance_FiduMark_Xu;
#else
	double dMarkDist = m_pObjPanelData->m_dDistance_FiduMark_Yl;
#endif
//170826 JSh.s
	//170922 JSh.s
	MListWorkTab		m_listWorkTab;
	m_pObjWorkScheduleSecond[0]->GetOrgWorkList(&m_listWorkTab);
	MVarList<MTabInfo>	*m_pWorkList;
	int WorkCount = 0;
	int iWorkListCount = m_listWorkTab.GetCount();
	for (int i=0; i<iWorkListCount; i++)
	{
		m_pWorkList = m_listWorkTab.GetSubList(i);
		int tcnt = DEF_MAX_WORKER_PER_GROUP - m_pWorkList->GetCount();
		if(WorkCount<tcnt)
			WorkCount = tcnt;
	}
	
	double dEscapeDist = DEF_INSPECTION_STANDARD_GAP * WorkCount;
	//170922 JSh.e

	double dWorkingDist = DEF_AIR_PLATE_LENGTH + (DEF_INSPECTIOM_BONDING_CENTER_TO_ALIGN_CENTER) - dEscapeDist;
	double dDiffer = dWorkingDist - dMarkDist;
//170826 JSh.e
	return dDiffer;
}

BOOL MPlatformOlbSystem::GetUseFlagPanel(int iWorkGroup)
{
	BOOL bUseFlag = FALSE;
#ifdef DEF_SOURCE_SYSTEM
//SJ_YYK 150907 Add...
#	ifndef DEF_MIRROR_
	if (iWorkGroup == DEF_FRONT_GROUP)
		bUseFlag = GetPanelData()->m_bUseFlag_Xd;
	else
		bUseFlag = GetPanelData()->m_bUseFlag_Xu;
#	else
	if (iWorkGroup == DEF_FRONT_GROUP)
		bUseFlag = GetPanelData()->m_bUseFlag_Xu;
	else
		bUseFlag = GetPanelData()->m_bUseFlag_Xd;
#endif
#else
#	ifndef DEF_MIRROR_
	if (iWorkGroup == DEF_FRONT_GROUP)
		bUseFlag = GetPanelData()->m_bUseFlag_Yl;
	else
		bUseFlag = GetPanelData()->m_bUseFlag_Yr;
#	else
	if (iWorkGroup == DEF_FRONT_GROUP)
		bUseFlag = GetPanelData()->m_bUseFlag_Yr;
	else
		bUseFlag = GetPanelData()->m_bUseFlag_Yl;
#	endif
#endif
	return bUseFlag;
}

int MPlatformOlbSystem::GetPanelAlignGroup()
{
	int iGroup = DEF_FRONT_GROUP;
#ifdef DEF_SOURCE_SYSTEM
	iGroup = DEF_FRONT_GROUP;
#else
	if (GetUseFlagPanel(DEF_FRONT_GROUP))
		iGroup = DEF_FRONT_GROUP;
	else if (GetUseFlagPanel(DEF_REAR_GROUP))
		iGroup = DEF_REAR_GROUP;
#endif
	return iGroup;
}

BOOL MPlatformOlbSystem::IsExistRemoveTab_TabMounter(int iWorkGroup)
{
	BOOL bExist = FALSE;
	if (iWorkGroup == DEF_FRONT_GROUP)
	{
		for (int i = 0; i < DEF_MOUNTER_WORK_PER_GROUP; i++)
		{
			if (m_pTabMounter[i]->IsAbsorbTabIC())
			{
				bExist = TRUE;
				break;
			}
		}
	}
	else
	{
//		for (int i = DEF_MOUNTER_WORK_PER_GROUP; i < DEF_MAX_TABMOUNTER; i++)
		for (int i = 0; i < DEF_MOUNTER_WORK_PER_GROUP; i++)
		{
			if (m_pRTabMounter[i]->IsAbsorbTabIC())
			{
				bExist = TRUE;
				break;
			}
		}
	}
	return bExist;
}

BOOL MPlatformOlbSystem::IsExistRemoveTab_TabCarrier(int iWorkGroup)
{
	BOOL bExist = FALSE;
	if (iWorkGroup == DEF_FRONT_GROUP)
	{
		for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		{
			if (m_pTabCarrier[i]->IsAbsorbTabIC())
			{
				bExist = TRUE;
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
		{
			if (m_pRTabCarrier[i]->IsAbsorbTabIC())
			{
				bExist = TRUE;
				break;
			}
		}
	}
	return bExist;
}

// 2011.12.14 KWC ---------------------------
//Chang_
int	MPlatformOlbSystem::GetDeviceIDList(CString strModelDataDir, CString *strDeviceIDList, int *iDeviceIDCount)
{
	CString		strMsg;
	MFileUtil	util;
	CString szTextTemp1, szTextTemp2;

	*iDeviceIDCount = util.GetDirCountInDir(strModelDataDir);

	if (*iDeviceIDCount > DEF_MAX_MODEL)
	{
		//ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64505"), &szTextTemp1);	//DeviceID 전용 메세지 추가 필요
		szTextTemp2.Format(_T("[% s>% s] The number of models is greater than the system allowed.\r\n [%s > %s] 모델 갯수가 시스템 허용 갯수보다 많습니다."), *iDeviceIDCount, DEF_MAX_MODEL, *iDeviceIDCount, DEF_MAX_MODEL);
		if (MyMessageBox(szTextTemp2, _T("Manage DeviceID"), M_ICONERROR))
		{
			return	ERR_SYSTEM_MAX_COUNT_EXCEED_LIMIT;
		}
	}

	// Directory List를 읽어와서 strModelList에 등록한다.
	util.GetDirListInDir(strModelDataDir, strDeviceIDList, *iDeviceIDCount);

	return ERR_SYSTEM_SUCCESS;
}
// -----------------------------------------
//Chang_
int MPlatformOlbSystem::DeleteDeviceID(LPCTSTR pStrDeviceIDPath)
{
	MFileUtil util;
	CString	strPath,strMsg;
	CString szTextTemp1, szTextTemp2;

	CString strDeviceIDName;
	strDeviceIDName = pStrDeviceIDPath;
	strDeviceIDName = strDeviceIDName.Right(strDeviceIDName.GetLength() - strDeviceIDName.ReverseFind(_T('\\')) - 1);

	if (!_tcscmp(strDeviceIDName, MOlbSystemPre.GetTrsAutoManagerComponent()->m_strEquipModelName))
	{
		//ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_64507"), &szTextTemp1);	//메세지 추가 필요
		szTextTemp2.Format(_T("[%s] The currently selected Device ID can not be deleted.\r\n[%s] 현재 선택된 Device ID를 삭제할 수 없습니다."), strDeviceIDName, strDeviceIDName);
		if (MyMessageBox(szTextTemp2, _T("Manage DeviceID"), M_ICONERROR))
		{
			return ERR_ACF_SYSTEM_DEVICEID_DELETE_FAIL;
		}
	}

	util.DeleteFilesInDir(pStrDeviceIDPath);	// 디렉토리내 파일들을 모두 삭제 한다.
	RemoveDirectory(pStrDeviceIDPath);			// 디렉토리를 지운다.

	return ERR_SYSTEM_SUCCESS;
}

	//20121004 SJ_HJG 횡전개
void MPlatformOlbSystem::SetTabEndMarkDistance(int iWorkGroupNo, double dTabEndMarkDistance)
{
	if (iWorkGroupNo == 0)
	{
		m_dTabEndMarkDistance[0] = dTabEndMarkDistance;
		//m_dTabEndMarkDistance1 = dTabEndMarkDistance;
	}
	else
	{
		m_dTabEndMarkDistance[1] = dTabEndMarkDistance;
		//m_dTabEndMarkDistance2 = dTabEndMarkDistance;
	}
}

double MPlatformOlbSystem::GetTabEndMarkDistance(int iInstanceNo)
{
	double dTabEndMarkDistance = 0.0;
	if (iInstanceNo == 0)
	{
		dTabEndMarkDistance = m_dTabEndMarkDistance[0];
		//dTabEndMarkDistance = m_dTabEndMarkDistance1;
	}
	else
	{
		dTabEndMarkDistance = m_dTabEndMarkDistance[1];
		//dTabEndMarkDistance = m_dTabEndMarkDistance2;
	}

	return dTabEndMarkDistance;
}

//________________________________________

void MPlatformOlbSystem::AssignWorkScheduleData()
{
	int i = 0;
#ifdef DEF_SOURCE_SYSTEM
	m_pTabDataSourceFirst->CopyData(*m_pObjTabData);
	m_pTabDataSourceSecond->CopyData(*m_pObjTabData);

	BOOL bForcedModifySchedule = FALSE;


	double dOffset = GetDifferLen_PlateToPanelLen();
	double dPanelHalfSizeX = 0.0;
	double dSumXu = 0.0;
	int iTabOffsetNoXu = 0;
	int iTabOffsetNoXd = 0;
	int iMaxTabXu = 0;
	int iMaxTabXd = 0;

	if (bForcedModifySchedule == FALSE)
	{
		dPanelHalfSizeX = m_pObjPanelData->m_dPanelSize_X/2.0;
		dSumXu = 0.0;
		iTabOffsetNoXu = 0;
		iMaxTabXu = m_pObjTabData->m_iXuTabQty;
		for (int i=0; i<iMaxTabXu; i++)
		{
			dSumXu += m_pObjTabData->m_dXuTabPitch[i];
			if (dSumXu > dPanelHalfSizeX)
			{
				iTabOffsetNoXu = i;
				break;
			}
		}
		m_pTabDataSourceFirst->m_iXuTabQty = iTabOffsetNoXu;
		m_pTabDataSourceSecond->m_iXuTabQty = m_pObjTabData->m_iXuTabQty - iTabOffsetNoXu;
		for (int i=iTabOffsetNoXu; i<iMaxTabXu; i++)
		{
			if (i==iTabOffsetNoXu)
				m_pTabDataSourceSecond->m_dXuTabPitch[i-iTabOffsetNoXu] = dSumXu + dOffset;
			else
				m_pTabDataSourceSecond->m_dXuTabPitch[i-iTabOffsetNoXu] = m_pObjTabData->m_dXuTabPitch[i];//@ + dOffset;
			m_pTabDataSourceSecond->m_bUseXuTab[i-iTabOffsetNoXu] = m_pObjTabData->m_bUseXuTab[i];
		}

		double dSumXd = 0.0;
		iTabOffsetNoXd = 0;
		iMaxTabXd = m_pObjTabData->m_iXdTabQty;
		for (int i=0; i<iMaxTabXd; i++)
		{
			dSumXd += m_pObjTabData->m_dXdTabPitch[i];
			if (dSumXd > dPanelHalfSizeX)
			{
				iTabOffsetNoXd = i;
				break;
			}
		}
		m_pTabDataSourceFirst->m_iXdTabQty = iTabOffsetNoXd;
		m_pTabDataSourceSecond->m_iXdTabQty = m_pObjTabData->m_iXdTabQty - iTabOffsetNoXd;
		for (int i=iTabOffsetNoXd; i<iMaxTabXd; i++)
		{
			if (i == iTabOffsetNoXd)
				m_pTabDataSourceSecond->m_dXdTabPitch[i-iTabOffsetNoXd] = dSumXd + dOffset;
			else
				m_pTabDataSourceSecond->m_dXdTabPitch[i-iTabOffsetNoXd] = m_pObjTabData->m_dXdTabPitch[i];//@ + dOffset;
			m_pTabDataSourceSecond->m_bUseXdTab[i-iTabOffsetNoXd] = m_pObjTabData->m_bUseXdTab[i];
		}
	}
	else
	{
		MTabData TmpTabDataFirst;
		MTabData TmpTabDataSecond;
		TmpTabDataFirst.CopyData(*m_pObjTabData);
		TmpTabDataSecond.CopyData(*m_pObjTabData);

		//@[0,3,6,9] [1,4,7,10]	---> First Job
		//@[2,5,8,11]			---> Second Job
		for (int i=0; i<TmpTabDataFirst.m_iXuTabQty ; i++)
			TmpTabDataFirst.m_bUseXuTab[i] = TRUE;
		for (int i=0; i<TmpTabDataFirst.m_iXdTabQty ; i++)
			TmpTabDataFirst.m_bUseXdTab[i] = TRUE;

		TmpTabDataFirst.m_bUseXuTab[2]
			= TmpTabDataFirst.m_bUseXuTab[5]
			= TmpTabDataFirst.m_bUseXuTab[8]
			= TmpTabDataFirst.m_bUseXuTab[11] = FALSE;
		TmpTabDataFirst.m_bUseXdTab[2]
			= TmpTabDataFirst.m_bUseXdTab[5]
			= TmpTabDataFirst.m_bUseXdTab[8]
			= TmpTabDataFirst.m_bUseXdTab[11] = FALSE;

		for (int i=0; i<TmpTabDataSecond.m_iXuTabQty ; i++)
			TmpTabDataSecond.m_bUseXuTab[i] = FALSE;
		for (int i=0; i<TmpTabDataSecond.m_iXdTabQty ; i++)
			TmpTabDataSecond.m_bUseXdTab[i] = FALSE;

		TmpTabDataSecond.m_bUseXuTab[2]
			= TmpTabDataSecond.m_bUseXuTab[5]
			= TmpTabDataSecond.m_bUseXuTab[8]
			= TmpTabDataSecond.m_bUseXuTab[11] = TRUE;
		TmpTabDataSecond.m_bUseXdTab[2]
			= TmpTabDataSecond.m_bUseXdTab[5]
			= TmpTabDataSecond.m_bUseXdTab[8]
			= TmpTabDataSecond.m_bUseXdTab[11] = TRUE;


		iTabOffsetNoXu = 2;
		double dSumTo3 = 0;	//3번째 Tab까지 거리
		for (int i=0; i<=iTabOffsetNoXu; i++)
			dSumTo3 += m_pObjTabData->m_dXuTabPitch[i];

		m_pTabDataSourceFirst->m_iXuTabQty = 11;	//@(0,1,2 ... 10)
		m_pTabDataSourceSecond->m_iXuTabQty = 10;	//@(2,3,4 ... 11)

		for (int i=0; i<12; i++)
			m_pTabDataSourceFirst->m_bUseXuTab[i] = TmpTabDataFirst.m_bUseXuTab[i];

		for (int i=iTabOffsetNoXu; i<12; i++)
		{
			if (i==iTabOffsetNoXu)
				m_pTabDataSourceSecond->m_dXuTabPitch[i-iTabOffsetNoXu] = dSumTo3 + dOffset;
			else
				m_pTabDataSourceSecond->m_dXuTabPitch[i-iTabOffsetNoXu] = m_pObjTabData->m_dXuTabPitch[i];
			m_pTabDataSourceSecond->m_bUseXuTab[i-iTabOffsetNoXu] = TmpTabDataSecond.m_bUseXuTab[i];
		}

		iTabOffsetNoXd = 2;
		dSumTo3 = 0;	//3번째 Tab까지 거리
		for (int i=0; i<=iTabOffsetNoXd; i++)
			dSumTo3 += m_pObjTabData->m_dXdTabPitch[i];

		m_pTabDataSourceFirst->m_iXdTabQty = 11;	//@(0,1,2 ... 10)
		m_pTabDataSourceSecond->m_iXdTabQty = 10;	//@(2,3,4 ... 11)

		for (int i=0; i<12; i++)
			m_pTabDataSourceFirst->m_bUseXdTab[i] = TmpTabDataFirst.m_bUseXdTab[i];

		for (int i=iTabOffsetNoXd; i<12; i++)
		{
			if (i==iTabOffsetNoXd)
				m_pTabDataSourceSecond->m_dXdTabPitch[i-iTabOffsetNoXd] = dSumTo3 + dOffset;
			else
				m_pTabDataSourceSecond->m_dXdTabPitch[i-iTabOffsetNoXd] = m_pObjTabData->m_dXdTabPitch[i];
			m_pTabDataSourceSecond->m_bUseXdTab[i-iTabOffsetNoXd] = TmpTabDataSecond.m_bUseXdTab[i];
		}
	}

#endif

	SWorkScheduleRefData	WorkScheduleRefData;
	
	WorkScheduleRefData.m_plnkPanelData		= GetPanelData();
	WorkScheduleRefData.m_plnkPreBonderData	= GetPreBonderData();
	WorkScheduleRefData.m_plnkSystemData	= GetSystemData();

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
#ifdef DEF_SOURCE_SYSTEM
		WorkScheduleRefData.m_plnkTabData		= m_pTabDataSourceFirst;
	//SJ_YYK 150907 Add...
#	ifndef DEF_MIRROR_
		if (i == 0)
		{
			WorkScheduleRefData.m_iFirstTabNo	= 0;
			if (bForcedModifySchedule == FALSE)
				//WorkScheduleRefData.m_iLastTabNo	= iTabOffsetNoXd-1;
				WorkScheduleRefData.m_iLastTabNo	= iTabOffsetNoXu-1; //170127 SJ_YSH
			else
				WorkScheduleRefData.m_iLastTabNo	= 10;
		}
		else
		{
			WorkScheduleRefData.m_iFirstTabNo	= 0;
			if (bForcedModifySchedule == FALSE)
				WorkScheduleRefData.m_iLastTabNo	= iTabOffsetNoXu-1;
			else
				WorkScheduleRefData.m_iLastTabNo	= 10;
		}
#	else
		if (i == 0)
		{
			WorkScheduleRefData.m_iFirstTabNo	= 0;
			if (bForcedModifySchedule == FALSE)
				WorkScheduleRefData.m_iLastTabNo	= iTabOffsetNoXu-1;
			else
				WorkScheduleRefData.m_iLastTabNo	= 10;
		}
		else
		{
			WorkScheduleRefData.m_iFirstTabNo	= 0;
			if (bForcedModifySchedule == FALSE)
				WorkScheduleRefData.m_iLastTabNo	= iTabOffsetNoXd-1;
			else
				WorkScheduleRefData.m_iLastTabNo	= 10;
		}
#	endif

#else
#	ifndef DEF_MIRROR_
		if (i == 0)
		{
			WorkScheduleRefData.m_iFirstTabNo	= 0;
			WorkScheduleRefData.m_iLastTabNo = m_pObjTabData->m_iYlTabQty-1;
		}
		else
		{
			WorkScheduleRefData.m_iFirstTabNo	= 0;
			WorkScheduleRefData.m_iLastTabNo = m_pObjTabData->m_iYrTabQty-1;
		}
#	else
		if (i == 0)
		{
			WorkScheduleRefData.m_iFirstTabNo	= 0;
			WorkScheduleRefData.m_iLastTabNo = m_pObjTabData->m_iYrTabQty-1;
		}
		else
		{
			WorkScheduleRefData.m_iFirstTabNo	= 0;
			WorkScheduleRefData.m_iLastTabNo = m_pObjTabData->m_iYlTabQty-1;
		}
#	endif
		WorkScheduleRefData.m_plnkTabData		= GetTabData();
#endif
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Add.
		m_pObjWorkSchedule[i]->AssignData(WorkScheduleRefData);
#else
		m_pObjWorkScheduleSecond[i]->AssignData(WorkScheduleRefData);
#endif
		
	}

#ifdef DEF_SOURCE_SYSTEM
	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		WorkScheduleRefData.m_plnkTabData		= m_pTabDataSourceSecond;
	//SJ_YYK 150907 Add...
#	ifndef DEF_MIRROR_
		if (i == 0)
		{
			if (bForcedModifySchedule == FALSE)
			{
				//WorkScheduleRefData.m_iFirstTabNo	= iTabOffsetNoXd;
				WorkScheduleRefData.m_iFirstTabNo	= iTabOffsetNoXu;
				WorkScheduleRefData.m_iLastTabNo	= iMaxTabXu-1;
			}
			else
			{
				WorkScheduleRefData.m_iFirstTabNo	= 2;
				WorkScheduleRefData.m_iLastTabNo	= 11;
			}
		}
		else
		{
			if (bForcedModifySchedule == FALSE)
			{
				WorkScheduleRefData.m_iFirstTabNo	= iTabOffsetNoXu;
				WorkScheduleRefData.m_iLastTabNo	= iMaxTabXu-1;
			}
			else
			{
				WorkScheduleRefData.m_iFirstTabNo	= 2;
				WorkScheduleRefData.m_iLastTabNo	= 11;
			}
		}
#	else
		if (i == 0)
		{
			if (bForcedModifySchedule == FALSE)
			{
				WorkScheduleRefData.m_iFirstTabNo	= iTabOffsetNoXu;
				WorkScheduleRefData.m_iLastTabNo	= iMaxTabXu-1;
			}
			else
			{
				WorkScheduleRefData.m_iFirstTabNo	= 2;
				WorkScheduleRefData.m_iLastTabNo	= 11;
			}
		}
		else
		{
			if (bForcedModifySchedule == FALSE)
			{
				WorkScheduleRefData.m_iFirstTabNo	= iTabOffsetNoXd;
				WorkScheduleRefData.m_iLastTabNo	= iMaxTabXu-1;
			}
			else
			{
				WorkScheduleRefData.m_iFirstTabNo	= 2;
				WorkScheduleRefData.m_iLastTabNo	= 11;
			}
		}
#endif
#ifdef DEF_USE_REVERSE_TAB_SIDE //SJ_YYK 161228 Add.
		m_pObjWorkScheduleSecond[i]->AssignData(WorkScheduleRefData);
#else
		m_pObjWorkSchedule[i]->AssignData(WorkScheduleRefData);
#endif
	}
#endif
}
/************************************************************************/
/* Motion 제어                                                          */
/************************************************************************/
//@수정요망 - Function 모듈 확인 후 수정 필요. 
BOOL MPlatformOlbSystem::IsFunctionModuleSlaveAxis(int iAxis)
{
	switch(iAxis)
	{		
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_T:
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_T:
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_T:		
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL MPlatformOlbSystem::IsFunctionModuleAxis(int iAxis)
{
	switch(iAxis)
	{
	case DEF_AXIS_SERCOS_TABIC_CARRIER1_T:
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_T:
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_T:
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_T:

		return TRUE;
	default:
		return FALSE;
	}
}

int MPlatformOlbSystem::InitRS_NMCBoard()
{
	int iError;
	CString strTemp;

	iError = m_pRS_NMCSetup->BoardInit();
	if(iError)
	{
		MyMessageBox("NMC Board Initial Fail", "NMC Init Error", M_ICONERROR);
		return FALSE;
	}
	return ERR_SYSTEM_SUCCESS;
}

//20170830 KDW Start //Front 전체 Door 잠긴 경우 , Rear 전체 Door 잠긴 경우 따로 확인
//Return TRUE : All Lock
BOOL MPlatformOlbSystem::IsAllDoorLock(int iWorkGroup)
{
	if(iWorkGroup == DEF_FRONT_GROUP)
		//return GetIOComponent()->IsOn(IN_FRONT_DOOR_LOCK);
		return GetIOComponent()->IsOff(OUT_FRONT_DOOR_LOCK_OPEN);
	else
		//return GetIOComponent()->IsOn(IN_REAR_DOOR_LOCK);
		return GetIOComponent()->IsOff(OUT_REAR_DOOR_LOCK_OPEN);
}

//Return TRUE : All Lock
BOOL MPlatformOlbSystem::IsMaterialDoorLock(int iWorkGroup)
{
	if(iWorkGroup == DEF_FRONT_GROUP)
		return GetIOComponent()->IsOn(IN_FRONT_MATERIAL_DOOR_LOCK);
	else
		return GetIOComponent()->IsOn(IN_REAR_MATERIAL_DOOR_LOCK);
}
//20170830 KDW End //Front, Rear 따로 확인

int MPlatformOlbSystem::SetGearIn(int iAxisID)
{
	int iResult = 0;
	
	int nMasterAxis = 0 ;
	int nSlaveAxis = 0;
	int nRatioNumerator = 1;
	int nRatioDenominator = 1;
	double fAccel = 100;
	double fDecel = 100; 
	double fJerk = 0;

#ifdef DEF_1ST_EQ	
	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_CENTER_1)
	{
		nMasterAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_1]->GetNodeID();
		nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_2]->GetNodeID();
		fAccel = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_1]->GetScale() * m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_1]->GetMovingAccelerate() * 10;
		fDecel = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_1]->GetScale() * m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_1]->GetMovingAccelerate() * 10; 
		
		iResult = m_pMotionLib->EnableSyncMotion_NMC(nMasterAxis, nSlaveAxis, nRatioNumerator, nRatioDenominator, fAccel, fDecel, fJerk);
		if(iResult)
			return iResult;
	}
	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT)
	{
		nMasterAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT]->GetNodeID();
		nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT_2]->GetNodeID();
		fAccel = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT]->GetScale() * m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT]->GetMovingAccelerate() * 10;
		fDecel = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT]->GetScale() * m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT]->GetMovingAccelerate() * 10; 
		
		iResult = m_pMotionLib->EnableSyncMotion_NMC(nMasterAxis, nSlaveAxis, nRatioNumerator, nRatioDenominator, fAccel, fDecel, fJerk);
		if(iResult)
			return iResult;
	}
#endif
	
	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_IN)
	{
		nMasterAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN]->GetNodeID();
		nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN_2]->GetNodeID();
		fAccel = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN]->GetScale() * m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN]->GetMovingAccelerate() * 10;
		fDecel = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN]->GetScale() * m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN]->GetMovingAccelerate() * 10; 
		
		iResult = m_pMotionLib->EnableSyncMotion_NMC(nMasterAxis, nSlaveAxis, nRatioNumerator, nRatioDenominator, fAccel, fDecel, fJerk);
		if(iResult)
			return iResult;
	}
	return iResult;
}

int MPlatformOlbSystem::SetGearOut(int iAxisID)
{
	int iResult = 0;
	int nSlaveAxis = 0;

	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_IN)
	{
		nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN_2]->GetNodeID();
		
		iResult = m_pMotionLib->DisableSyncMotion_NMC(nSlaveAxis);
		if(iResult)
			return iResult;
	}
#ifdef DEF_1ST_EQ
	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_CENTER_1)
	{
		nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_2]->GetNodeID();
		
		iResult = m_pMotionLib->DisableSyncMotion_NMC(nSlaveAxis);
		if(iResult)
			return iResult;
	}
	
	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT)
	{
		nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT_2]->GetNodeID();
		
		iResult = m_pMotionLib->DisableSyncMotion_NMC(nSlaveAxis);
		if(iResult)
			return iResult;
	}
#endif
	return iResult;
}

BOOL MPlatformOlbSystem::IsGearIn(int iAxisID)
{
	BOOL bResult = FALSE;

	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_IN)
	{
		int nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN_2]->GetNodeID();
		
		bResult = (m_pMotionLib->IsSyncEnable_NMC(nSlaveAxis) == TRUE);
		return bResult;
	}
#ifdef DEF_1ST_EQ
	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_CENTER_1)
	{
		int nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_2]->GetNodeID();
		
		bResult = (m_pMotionLib->IsSyncEnable_NMC(nSlaveAxis) == TRUE);
		return bResult;
	}

	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT)
	{
		int nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT_2]->GetNodeID();
		
		bResult = (m_pMotionLib->IsSyncEnable_NMC(nSlaveAxis) == TRUE);
		return bResult;
	}
#endif
	return bResult;
}

BOOL MPlatformOlbSystem::IsGearOut(int iAxisID)
{
	BOOL bResult = FALSE;

	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_IN)
	{
		int nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_IN_2]->GetNodeID();
		
		bResult = (m_pMotionLib->IsSyncEnable_NMC(nSlaveAxis) == FALSE);
		return bResult;
	}
#ifdef DEF_1ST_EQ
	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_CENTER_1)
	{
		int nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_CENTER_2]->GetNodeID();
		
		bResult = (m_pMotionLib->IsSyncEnable_NMC(nSlaveAxis) == FALSE);
		return bResult;
	}

	if(iAxisID == DEF_AXIS_SERCOS_TRANSFER_OUT)
	{
		int nSlaveAxis = m_pMotion[DEF_AXIS_SERCOS_TRANSFER_OUT_2]->GetNodeID();
		
		bResult = (m_pMotionLib->IsSyncEnable_NMC(nSlaveAxis) == FALSE);
		return bResult;
	}
#endif
	return bResult;
}


/* End Of File */
