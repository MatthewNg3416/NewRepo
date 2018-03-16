/* 
 * Control Handler Component
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

#include "stdAfx.h"
#include "MCtrlHandler.h"
#include "MTickTimer.h"
#include "MSystemData.h"

#include "MHandler.h"
#include "MNetH.h"
#include "IStatePanelTransfer.h"
#include "MTrsAutoManager.h"
#include "DefSystem.h"
#include "DefPanelTransfer.h"
#include "MPlatformOlbSystem.h"

extern MPlatformOlbSystem	MOlbSystemPre;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** 
 * Constructor
 * 
 * @param	SCommonAttribute commonData (공통적으로 사용할 Data)
 * @param	SCtrlHandlerRefCompList listRefComponent (참조할 Component 포인터 리스트)
 * @param	SCtrlHandlerData datComponent (자체적으로 사용할 Data)
 */
MCtrlHandler::MCtrlHandler(SCommonAttribute commonData, 
						   SCtrlHandlerRefCompList listRefComponent, 
						   SCtrlHandlerData datComponent)
: ICommon(commonData)
{
	AssignComponents(listRefComponent);
	SetData(datComponent);

//@	InitLoadStep();
//@	initUnloadStep();	// 다음 설비와 Panel을 주고 받는 내부 Step을 Clear 한다.
}

/**
 * @stereotype destructor 
 */
MCtrlHandler::~MCtrlHandler()
{
}

/****************************************************************************************************************/
/**                                                                                                            **/
/**                                     Common Interface Method Start                                          **/
/**                                                                                                            **/
/****************************************************************************************************************/

/**
 * Unload Handler를 초기화한다.
 *
 * @return	int (0 = Success, Error Code = 그 외)
 */
int MCtrlHandler::Initialize()
{
   	int iResult = ERR_CTRLHANDLER_SUCCESS;
	CString strLogMsg;
	
	/** Handler Object가 할당되어 있지 않은 경우 Error Code Return */
	if (m_plnkHandler == NULL)
	{
//@		WriteErrorLog("Initialize(): Handler Object가 할당되지 않았음");
		return generateErrorCode(311001);	// ERR_CTRLHANDLER_UNABLE_TO_USE_HANDLER
	}
	
#ifdef SIMULATION
	Sleep(500);
	return ERR_CTRLHANDLER_SUCCESS;
#endif
	
	// Handler Component의 초기화 함수 호출
//@	iResult = m_plnkHandler->Initialize();
//@	if (iResult)
//@		return iResult;
	
    return ERR_CTRLHANDLER_SUCCESS;
}

/**
 * Unload Handler Component에서 참조할 Component List를 설정한다.
 *
 * @param	SCtrlHandlerRefCompList listRefComponets (설정하고자 하는 Component 참조 List)
 * @return	void
 */
int MCtrlHandler::AssignComponents(SCtrlHandlerRefCompList listRefComponents)
{
	/*/
	m_plnkHandler				= listRefComponents.m_plnkHandler;
	m_plnkStage					= listRefComponents.m_plnkStage;
	m_plnkWorkBench			    = listRefComponents.m_plnkWorkBench;
	m_plnkInspectCameraCarrier	= listRefComponents.m_plnkInspectCameraCarrier;
	m_plnkVision				= listRefComponents.m_plnkVision;
	m_plnkCleaner				= listRefComponents.m_plnkCleaner;
	/*/
    m_plnkHandler			= listRefComponents.m_plnkHandler;
    m_plnkWorkBench			= listRefComponents.m_plnkWorkBench;
	m_plnkMelsecNet			= listRefComponents.m_plnkMelsecNet;

	m_plnkPanelTransferCenter	= listRefComponents.m_plnkPanelTransferCenter;
	m_plnkPanelTransfer			= listRefComponents.m_plnkPanelTransfer;

	return ERR_CTRLHANDLER_SUCCESS;
}

/**
 * Unload Handler 관련 Data를 설정한다.
 *
 * @param	SCtrlHandlerData datComponent (저장하고자 하는 Handler Data)
 * @return	void
 */
int MCtrlHandler::SetData(SCtrlHandlerData datComponent)
{
	m_plnkSystemData			= datComponent.m_plnkSystemData;
	m_plnkPanelData				= datComponent.m_plnkPanelData;
//@	m_plnkAcfData				= datComponent.m_plnkAcfData;
	
	return ERR_CTRLHANDLER_SUCCESS;
}

/***********************************************************************************************
/*
/* Handler Operation
/*
/************************************************************************************************/

/**
 * Unload Handler를 이용하여 Stage에서 Panel을 Loading 한다.
 * 
 * @return	int (0 = Success, Error Code = 그 외) 
 */
int MCtrlHandler::GetPanel()
{ 
	int		iResult = ERR_CTRLHANDLER_SUCCESS;
	BOOL	bStatus = 0;
	CString	strLogMsg;

#ifdef SIMULATION
	Sleep(500);
	return ERR_CTRLHANDLER_SUCCESS;
#endif

	/** Handler Object가 할당되어 있지 않은 경우 Error Code Return */
	if (m_plnkHandler == NULL)
	{
//@		WriteErrorLog("GetPanel(): Handler Object가 할당되지 않았음");
		return generateErrorCode(311001);	// ERR_CTRLHANDLER_UNABLE_TO_USE_HANDLER
	}

	/** WorkBench Object가 할당되어 있지 않은 경우 Error Code Return */
	if (m_plnkWorkBench == NULL)
	{
//@		WriteErrorLog("GetPanel(): WorkBench Object가 할당되지 않았음");
		return generateErrorCode(311002);	// ERR_CTRLHANDLER_UNABLE_TO_USE_STAGE
	}
	
	/** Auto Mode이고 Dry Run Mode가 아닐 경우 */
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		/** Stage에 Panel이 존재하는지 확인 */
		/*
		if (FALSE == m_plnkWorkBench->IsAbsorbPanel())
		{
			Sleep(300);
			if (FALSE == m_plnkWorkBench->IsAbsorbPanel())
			{
	//@			WriteErrorLog("GetPanel(): WorkBench에 Panel이 존재하지 않음");
				return generateErrorCode(311003);	// ERR_CTRLHANDLER_STAGE_PANEL_NON_EXIST
			}
		}
		*/
		if(FALSE == m_plnkPanelTransferCenter->IsPanelAbsorbed())
		{
			Sleep(300);
			if (FALSE == m_plnkPanelTransferCenter->IsPanelAbsorbed())
			{
				return generateErrorCode(311003);	// ERR_CTRLHANDLER_STAGE_PANEL_NON_EXIST
			}
		}
		// Unload Handler에 Panel이 존재하는지 확인
		if (TRUE == m_plnkHandler->IsAbsorbPanelOneMore())
		{
//@			WriteErrorLog("GetPanel(): Unload Handler에 이미 Panel이 존재함");
			return generateErrorCode(311006);	// ERR_CTRLHANDLER_HND_PANEL_EXIST
		}
	}

	/*
	if (FALSE == m_plnkWorkBench->IsInArmPos(DEF_WORKBENCH_ARM_UNLOAD_POS))
	{
		return generateErrorCode(311009);
	}
	if (FALSE == m_plnkWorkBench->IsInZPos(DEF_WORKBENCH_Z_UP_POS))
	{
		iResult = m_plnkWorkBench->SafeMoveZPos(DEF_WORKBENCH_Z_UP_POS);
		if (iResult)
			return iResult;
	}
	
  */

	if (FALSE == m_plnkPanelTransferCenter->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
	{
		Sleep(100);
		if (FALSE == m_plnkPanelTransferCenter->IsInXPos(DEF_PANEL_TRANSFER_UNLOAD_POS))
			return generateErrorCode(311009);
	}


	if (FALSE == m_plnkHandler->IsInXYTPos(DEF_HANDLER_LOAD_POS))
	{
//@		return generateErrorCode(311008);
		iResult = m_plnkHandler->SafeMoveXYTPos(DEF_HANDLER_LOAD_POS);
		if (iResult)
			return iResult;
	}

	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_DOWN_POS);
	if (iResult)
		return iResult;

	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		// 1. Handler가 Panel을 흡착한다. 
		iResult = m_plnkHandler->AbsorbPanel(FALSE);
		if (iResult)
		{
			m_plnkHandler->ReleasePanel();
			m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
//			m_plnkWorkBench->AbsorbPanel(FALSE);
			return iResult;
		}
	}
/*	
	// 2. WorkBench가 Panel을 해제한다.
	iResult = m_plnkWorkBench->ReleasePanel();
	if (iResult)
		return iResult;
*/

	iResult = m_plnkPanelTransferCenter->ReleasePanel();
	if(iResult)
		return iResult;

	// jskim Up동작 하기전에 다시한번..
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		iResult = m_plnkHandler->AbsorbPanel(FALSE);
		if (iResult)
		{
			m_plnkHandler->ReleasePanel();
			m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
//			m_plnkWorkBench->AbsorbPanel(FALSE);
			return iResult;
		}
	}

	m_plnkPanelTransferCenter->DownPickUpUDCyl(TRUE);

	// 3. Handler 상승
	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
	if (iResult)
		return iResult;
	
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		// 4. Handler Vacuum 흡착 확인  
		iResult = m_plnkHandler->AbsorbPanel();
		if (iResult)	return iResult;
	}

	return ERR_CTRLHANDLER_SUCCESS;
}

/** Local Stage에서 Panel을 Loading을 Ready 한다 */
/*/
int MCtrlHandler::PutPanelReady()
{
	int iResult = ERR_CTRLHANDLER_SUCCESS;

	iResult = m_plnkHandler->SafeMoveXYTPos(DEF_HANDLER_UNLOAD_POS);
	if (iResult != ERR_CTRLHANDLER_SUCCESS)
	{
		return iResult;
	}

	if(!(m_plnkStage->IsInXYTPos(DEF_STAGE_LOAD_POS) && m_plnkStage->IsInZPos(DEF_STAGE_Z_MOVE_POS)))
	{
		WriteErrorLog("PutPanel(): WorkBench가 Loading 할 수 있는 위치가 아닙니다.");
		return generateErrorCode(311015);	// ERR_CTRLHANDLER_HND_PANEL_EXIST
	}

	return ERR_CTRLHANDLER_SUCCESS;
}
/*/

int MCtrlHandler::PutPanel()
{
	int iResult = ERR_CTRLHANDLER_SUCCESS;

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
	{
		Sleep(1000);
		return ERR_CTRLHANDLER_SUCCESS;
	}

	if (FALSE == m_plnkHandler->IsInXYTPos(DEF_HANDLER_UNLOAD_POS))
	{
		iResult = m_plnkHandler->SafeMoveXYTPos(DEF_HANDLER_UNLOAD_POS);
		if (iResult)
			return iResult;
	}

	if(FALSE == m_plnkPanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
	{
		Sleep(200);
		if(FALSE == m_plnkPanelTransfer->IsInXPos(DEF_PANEL_TRANSFER_LOAD_POS))
			return generateErrorCode(311015);
	}

	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_DOWN_POS);
	if (iResult)
	{
		return iResult;
	}

	m_plnkPanelTransfer->AbsorbPanel(TRUE);
	
	iResult = m_plnkPanelTransfer->UpPickUpUDCyl();
	if(iResult)
		return iResult;

	iResult = m_plnkPanelTransfer->AbsorbPanel();
	if(iResult)
		return iResult;

	iResult = m_plnkHandler->ReleasePanel();
	if(iResult)
		return iResult;

	//m_plnkPanelTransfer->DownPickUpUDCyl(TRUE); //161231 SJ_YSH Del...

	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
	if(iResult)
		return iResult;

	//161231 SJ_YSH Del.
	/*/
	iResult = m_plnkPanelTransfer->DownPickUpUDCyl();
	if(iResult)
		return iResult;	
	/*/

	return ERR_CTRLHANDLER_SUCCESS;


	return ERR_CTRLHANDLER_SUCCESS;
}

/*/
int MCtrlHandler::UpHandler()
{
	int iResult;
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		iResult = m_plnkHandler->ReleasePanel();
		if (iResult)
			return iResult;
	}
	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
	if (iResult)
		return iResult;

	return ERR_CTRLHANDLER_SUCCESS;
}
/*/

/*/
int MCtrlHandler::LoadPanel()
{
	if (!m_plnkHandler->IsInXPos(DEF_HANDLER_LOAD_POS))
		return generateErrorCode(311016);

	int iResult = m_plnkHandler->AbsorbPanel(TRUE);
	if (iResult)
	{
		m_plnkHandler->ReleasePanel();
//		m_plnkHandler->Down();
//		m_plnkHandler->SafeMoveYPos(DEF_HANDLER_Y_BACK_POS);
		return iResult;
	}

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
		return ERR_CTRLHANDLER_SUCCESS;
/ *
	iResult = m_plnkHandler->Up();
	if (iResult)
	{
		m_plnkHandler->ReleasePanel();
		m_plnkHandler->Down();
        //20060504 ysb
		//		m_plnkHandler->SafeMoveYPos(DEF_HANDLER_Y_BACK_POS);
	}
* /
	return iResult;
}
/*/

/***********************************************************************************************
/*
/* Internal Operation
/*
/************************************************************************************************/

/*/
void MCtrlHandler::setUnloadStep(EStepUnLoadHandOffPanel stepVal)
{
	if (stepVal != m_estepUnloadHandOff) // 현재 STEP가 다음 STEP이 같지 않을 경우에만 이전 STEP 변경
		m_estepUnloadHandOffPrevious	= m_estepUnloadHandOff;
	m_estepUnloadHandOff				= stepVal;
}

void MCtrlHandler::initUnloadStep()
{
	m_estepUnloadHandOffPrevious	= STEP_UNLOAD_HANDOFF_INIT;
	m_estepUnloadHandOff			= STEP_UNLOAD_HANDOFF_INIT;
}

void MCtrlHandler::SetLoadStep(EStepLoadHandOffPanel stepVal)
{
	if (stepVal != m_estepLoadHandOff) // 현재 STEP가 다음 STEP이 같지 않을 경우에만 이전 STEP 변경
		m_estepLoadHandOffPrevious	= m_estepLoadHandOff;
	m_estepLoadHandOff				= stepVal;
}

void MCtrlHandler::InitLoadStep()
{
	m_estepLoadHandOffPrevious	= STEP_LOAD_HANDOFF_INIT;
	m_estepLoadHandOff			= STEP_LOAD_HANDOFF_INIT;
}
/*/

int MCtrlHandler::MoveForLoading()
{
	int iResult = ERR_CTRLHANDLER_SUCCESS;
	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
	if(iResult)
		return iResult;		

	// XYT축을 이동시킨다. 
	iResult = m_plnkHandler->SafeMoveXYTPos(DEF_HANDLER_LOAD_POS);
	if(iResult)
		return iResult;		

	return ERR_CTRLHANDLER_SUCCESS;
}

int MCtrlHandler::MoveForWaiting()
{
	int iResult = ERR_CTRLHANDLER_SUCCESS;
	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
	if(iResult)
		return iResult;		

	// XYT축을 이동시킨다. 
	iResult = m_plnkHandler->SafeMoveXYTPos(DEF_HANDLER_TURN_POS);
	if(iResult)
		return iResult;		

	return ERR_CTRLHANDLER_SUCCESS;
}

int MCtrlHandler::MoveForUnloading()
{
	int iResult = ERR_CTRLHANDLER_SUCCESS;
	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
	if(iResult)
		return iResult;		
	
	// XYT축을 이동시킨다. 
	iResult = m_plnkHandler->SafeMoveXYTPos(DEF_HANDLER_UNLOAD_POS);
	if(iResult)
		return iResult;	

	return ERR_CTRLHANDLER_SUCCESS;
}

int MCtrlHandler::InitializeMelsecIF()
{
	m_plnkMelsecNet->SetLowerEqBit(eSendAble, 0);
	m_plnkMelsecNet->SetLowerEqBit(eSendStart, 0);
	m_plnkMelsecNet->SetLowerEqBit(eGlassCheckSensorOn, 0);
	m_plnkMelsecNet->SetLowerEqBit(eSendComplete, 0);

	return ERR_CTRLHANDLER_SUCCESS;
}

/**
 * ACF 부착 상태 검사 수행
 */
/*/
int	MCtrlHandler::InspectPanel(int iCurrentJobID)
{
#ifdef SIMULATION
	return ERR_CTRLHANDLER_SUCCESS;
#endif

	int iReturn;
	BOOL bAcfInspectResult;
	int iCameraPos;
	if (!iCurrentJobID == DEF_SOURCE_INSPECT_XU_JOB)
	{
		iCameraPos = DEF_CAMERA_INSPECTION_POS_Y;
	}
	else
	{
		iCameraPos = DEF_CAMERA_INSPECTION_POS_XU;
	}
	
	if (!m_plnkInspectCameraCarrier->IsInY1Pos(iCameraPos)
		//cho delete
		/ *|| !m_plnkInspectCameraCarrier->IsInY2Pos(iCameraPos)* /
		)
	{
		iReturn = m_plnkInspectCameraCarrier->SafeY1MovePos(iCameraPos);
		if (iReturn)
		{
			WriteErrorLog("InspectPanel():Camera Carrier X Motion 동작 중 이상 발생");
			return iReturn;
		}
		//cho delete
		/ *
		iReturn = m_plnkInspectCameraCarrier->SafeY2MovePos(iCameraPos);
		if (iReturn)
		{
			WriteErrorLog("InspectPanel():Camera Carrier X Motion 동작 중 이상 발생");
			return iReturn;
		}* /
	}

	iReturn = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);  
	if (iReturn)
	{
		WriteErrorLog("InspectPanel():Z Motion 동작 중 이상 발생");
		return iReturn;
	}

	int iHandlerPos = GetHandlerPosForCurrentJob(iCurrentJobID);
	
	iReturn = m_plnkHandler->SafeMoveXYTPos(iHandlerPos);
	if (iReturn)
	{
		WriteErrorLog("InspectPanel():XYT Motion 동작 중 이상 발생");
		return iReturn;
	}

	if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE
		|| m_plnkSystemData->m_eRunMode == PASS_RUN_MODE
		|| m_plnkSystemData->m_bUseAcfInspection == FALSE)
		return ERR_CTRLHANDLER_SUCCESS;

	/ ** Blob Analysis 1 * /
	int iCameraNo, iMarkNo;
	switch(iCurrentJobID)
	{
	
	case DEF_SOURCE_INSPECT_XU_JOB:
		iCameraNo = DEF_CAMERA_INSPECTION_1;
		iMarkNo = DEF_ACF_INSPECTION_Xu;
		break;
	case DEF_GATE_INSPECT_YR_FRONT_JOB:
		iCameraNo = DEF_CAMERA_INSPECTION_1;
		iMarkNo = DEF_ACF_INSPECTION_Yr;
		break;
	case DEF_GATE_INSPECT_YR_REAR_JOB:
		iCameraNo = DEF_CAMERA_INSPECTION_1;
		iMarkNo = DEF_ACF_INSPECTION_Yr;
		break;
	case DEF_GATE_INSPECT_YL_FRONT_JOB:
		iCameraNo = DEF_CAMERA_INSPECTION_2;
		iMarkNo = DEF_ACF_INSPECTION_Yl;
		break;
	case DEF_GATE_INSPECT_YL_REAR_JOB:
		iCameraNo = DEF_CAMERA_INSPECTION_2;
		iMarkNo = DEF_ACF_INSPECTION_Yl;
		break;
	}

//	m_plnkVision->Grab(iCameraNo);
	bAcfInspectResult = TRUE;
	int iBlobResult = m_plnkVision->RecognitionBLOBPattern(iCameraNo, iMarkNo, m_plnkSystemData->m_iBlobAlgorithm, TRUE);
	
	if (iBlobResult)
	{
		/ ** Vision Error : 104021 = Blob Analsys 결과가 NG(Not Good) 입니다. * /
		if (iBlobResult/1000 == 104021)
		{
			Sleep(100);
//			m_plnkVision->Grab(iCameraNo);
			Sleep(100);
			iBlobResult = m_plnkVision->RecognitionBLOBPattern(iCameraNo, iMarkNo, m_plnkSystemData->m_iBlobAlgorithm, TRUE);
			if (iBlobResult)
			{
				WriteErrorLog("InspectPanel():ACF 부착 검사 결과가 NG 입니다.");
				bAcfInspectResult = FALSE;
				return generateErrorCode(311008+iCurrentJobID);
			}
		}
		else
			return iBlobResult;
	}
	/ ** Blob Analysis 2 * /
	if (m_plnkAcfData->m_bUseFlag_Xu && iCurrentJobID == DEF_SOURCE_INSPECT_XU_JOB)
	{
		BOOL bAcfInspectResult2 = TRUE;
		int iBlobResult2 = m_plnkVision->RecognitionBLOBPattern(DEF_CAMERA_INSPECTION_2, iMarkNo, m_plnkSystemData->m_iBlobAlgorithm);
		if (iBlobResult2)
		{
			/ ** Vision Error : 104021 = Blob Analsys 결과가 NG(Not Good) 입니다. * /
			if (iBlobResult2/1000 == 104021)
			{
				Sleep(100);
//				m_plnkVision->Grab(DEF_CAMERA_INSPECTION_2);
				Sleep(100);
				iBlobResult2 = m_plnkVision->RecognitionBLOBPattern(DEF_CAMERA_INSPECTION_2, iMarkNo, m_plnkSystemData->m_iBlobAlgorithm, TRUE);
				if (iBlobResult2)
				{
					WriteErrorLog("InspectPanel():ACF 부착 검사 결과가 NG 입니다.");
					bAcfInspectResult2 = FALSE;
					return generateErrorCode(311008+iCurrentJobID);
				}
			}
			else
				return iBlobResult2;
		}
	}

	WriteErrorLog("InspectPanel():ACF 부착 검사 결과가 성공 입니다.");
	return ERR_CTRLHANDLER_SUCCESS;
}
/*/

/**
 *	넘겨 받은 Current Job ID 에 맞는 Inspect Camera Carrier Position ID 를 반환한다.
 *	@return int : Inspect Camera Carrier Position ID
 *	@param int iCurrentJobID : 현재 Job ID
 */
/*/
int MCtrlHandler::GetCameraPosForCurrentJob(int iCurrentJobID)
{
	ASSERT(iCurrentJobID > DEF_NONE_JOB && iCurrentJobID < DEF_MAX_JOB);
	if (m_plnkSystemData->m_eSystemType == SYSTEM_TYPE_SOURCE
		&& m_plnkAcfData->m_bUseSecondJob == FALSE)
	{
		return DEF_CAMERA_INIT_POS;
	}
	else
		return iCurrentJobID + DEF_CAMERA_WORKING_POS_XU;
}
/*/

/**
 *	넘겨 받은 Current Job ID 와 Position Type 에 맞는 Handler Position ID 를 반환한다.
 *	@return int : Handler Position ID
 *	@param int iCurrentJobID : 현재 Job ID
 */
/*/
int MCtrlHandler::GetHandlerPosForCurrentJob(int iCurrentJobID)
{
	int iHandlerPosID;
	switch (iCurrentJobID)
	{
	case DEF_SOURCE_INSPECT_XU_JOB:		// 4
		iHandlerPosID = DEF_HANDLER_INSPECT_XU_POS;
		break;
	case DEF_SOURCE_INSPECT_XD_JOB:		// 5
		iHandlerPosID = DEF_HANDLER_INSPECT_XD_POS;
		break;
	case DEF_GATE_INSPECT_YL_FRONT_JOB:		// 4
		iHandlerPosID = DEF_HANDLER_INSPECT_YR1_POS;
		break;
	case DEF_GATE_INSPECT_YR_FRONT_JOB:		// 5
		iHandlerPosID = DEF_HANDLER_INSPECT_YF1_POS;
		break;
	case DEF_GATE_INSPECT_YL_REAR_JOB:	// 4
		iHandlerPosID = DEF_HANDLER_INSPECT_YR2_POS;
		break;
	case DEF_GATE_INSPECT_YR_REAR_JOB:		// 5
		iHandlerPosID = DEF_HANDLER_INSPECT_YF2_POS;
		break;
	default:
		iHandlerPosID = -1;
		break;
	}

	return iHandlerPosID;
}
/*/

// End Of Code
