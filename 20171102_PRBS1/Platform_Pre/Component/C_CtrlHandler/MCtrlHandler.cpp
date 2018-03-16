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
 * @param	SCommonAttribute commonData (���������� ����� Data)
 * @param	SCtrlHandlerRefCompList listRefComponent (������ Component ������ ����Ʈ)
 * @param	SCtrlHandlerData datComponent (��ü������ ����� Data)
 */
MCtrlHandler::MCtrlHandler(SCommonAttribute commonData, 
						   SCtrlHandlerRefCompList listRefComponent, 
						   SCtrlHandlerData datComponent)
: ICommon(commonData)
{
	AssignComponents(listRefComponent);
	SetData(datComponent);

//@	InitLoadStep();
//@	initUnloadStep();	// ���� ����� Panel�� �ְ� �޴� ���� Step�� Clear �Ѵ�.
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
 * Unload Handler�� �ʱ�ȭ�Ѵ�.
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MCtrlHandler::Initialize()
{
   	int iResult = ERR_CTRLHANDLER_SUCCESS;
	CString strLogMsg;
	
	/** Handler Object�� �Ҵ�Ǿ� ���� ���� ��� Error Code Return */
	if (m_plnkHandler == NULL)
	{
//@		WriteErrorLog("Initialize(): Handler Object�� �Ҵ���� �ʾ���");
		return generateErrorCode(311001);	// ERR_CTRLHANDLER_UNABLE_TO_USE_HANDLER
	}
	
#ifdef SIMULATION
	Sleep(500);
	return ERR_CTRLHANDLER_SUCCESS;
#endif
	
	// Handler Component�� �ʱ�ȭ �Լ� ȣ��
//@	iResult = m_plnkHandler->Initialize();
//@	if (iResult)
//@		return iResult;
	
    return ERR_CTRLHANDLER_SUCCESS;
}

/**
 * Unload Handler Component���� ������ Component List�� �����Ѵ�.
 *
 * @param	SCtrlHandlerRefCompList listRefComponets (�����ϰ��� �ϴ� Component ���� List)
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
 * Unload Handler ���� Data�� �����Ѵ�.
 *
 * @param	SCtrlHandlerData datComponent (�����ϰ��� �ϴ� Handler Data)
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
 * Unload Handler�� �̿��Ͽ� Stage���� Panel�� Loading �Ѵ�.
 * 
 * @return	int (0 = Success, Error Code = �� ��) 
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

	/** Handler Object�� �Ҵ�Ǿ� ���� ���� ��� Error Code Return */
	if (m_plnkHandler == NULL)
	{
//@		WriteErrorLog("GetPanel(): Handler Object�� �Ҵ���� �ʾ���");
		return generateErrorCode(311001);	// ERR_CTRLHANDLER_UNABLE_TO_USE_HANDLER
	}

	/** WorkBench Object�� �Ҵ�Ǿ� ���� ���� ��� Error Code Return */
	if (m_plnkWorkBench == NULL)
	{
//@		WriteErrorLog("GetPanel(): WorkBench Object�� �Ҵ���� �ʾ���");
		return generateErrorCode(311002);	// ERR_CTRLHANDLER_UNABLE_TO_USE_STAGE
	}
	
	/** Auto Mode�̰� Dry Run Mode�� �ƴ� ��� */
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		/** Stage�� Panel�� �����ϴ��� Ȯ�� */
		/*
		if (FALSE == m_plnkWorkBench->IsAbsorbPanel())
		{
			Sleep(300);
			if (FALSE == m_plnkWorkBench->IsAbsorbPanel())
			{
	//@			WriteErrorLog("GetPanel(): WorkBench�� Panel�� �������� ����");
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
		// Unload Handler�� Panel�� �����ϴ��� Ȯ��
		if (TRUE == m_plnkHandler->IsAbsorbPanelOneMore())
		{
//@			WriteErrorLog("GetPanel(): Unload Handler�� �̹� Panel�� ������");
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
		// 1. Handler�� Panel�� �����Ѵ�. 
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
	// 2. WorkBench�� Panel�� �����Ѵ�.
	iResult = m_plnkWorkBench->ReleasePanel();
	if (iResult)
		return iResult;
*/

	iResult = m_plnkPanelTransferCenter->ReleasePanel();
	if(iResult)
		return iResult;

	// jskim Up���� �ϱ����� �ٽ��ѹ�..
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

	// 3. Handler ���
	iResult = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);
	if (iResult)
		return iResult;
	
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		// 4. Handler Vacuum ���� Ȯ��  
		iResult = m_plnkHandler->AbsorbPanel();
		if (iResult)	return iResult;
	}

	return ERR_CTRLHANDLER_SUCCESS;
}

/** Local Stage���� Panel�� Loading�� Ready �Ѵ� */
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
		WriteErrorLog("PutPanel(): WorkBench�� Loading �� �� �ִ� ��ġ�� �ƴմϴ�.");
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
	if (stepVal != m_estepUnloadHandOff) // ���� STEP�� ���� STEP�� ���� ���� ��쿡�� ���� STEP ����
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
	if (stepVal != m_estepLoadHandOff) // ���� STEP�� ���� STEP�� ���� ���� ��쿡�� ���� STEP ����
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

	// XYT���� �̵���Ų��. 
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

	// XYT���� �̵���Ų��. 
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
	
	// XYT���� �̵���Ų��. 
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
 * ACF ���� ���� �˻� ����
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
			WriteErrorLog("InspectPanel():Camera Carrier X Motion ���� �� �̻� �߻�");
			return iReturn;
		}
		//cho delete
		/ *
		iReturn = m_plnkInspectCameraCarrier->SafeY2MovePos(iCameraPos);
		if (iReturn)
		{
			WriteErrorLog("InspectPanel():Camera Carrier X Motion ���� �� �̻� �߻�");
			return iReturn;
		}* /
	}

	iReturn = m_plnkHandler->SafeMoveZPos(DEF_HANDLER_Z_UP_POS);  
	if (iReturn)
	{
		WriteErrorLog("InspectPanel():Z Motion ���� �� �̻� �߻�");
		return iReturn;
	}

	int iHandlerPos = GetHandlerPosForCurrentJob(iCurrentJobID);
	
	iReturn = m_plnkHandler->SafeMoveXYTPos(iHandlerPos);
	if (iReturn)
	{
		WriteErrorLog("InspectPanel():XYT Motion ���� �� �̻� �߻�");
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
		/ ** Vision Error : 104021 = Blob Analsys ����� NG(Not Good) �Դϴ�. * /
		if (iBlobResult/1000 == 104021)
		{
			Sleep(100);
//			m_plnkVision->Grab(iCameraNo);
			Sleep(100);
			iBlobResult = m_plnkVision->RecognitionBLOBPattern(iCameraNo, iMarkNo, m_plnkSystemData->m_iBlobAlgorithm, TRUE);
			if (iBlobResult)
			{
				WriteErrorLog("InspectPanel():ACF ���� �˻� ����� NG �Դϴ�.");
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
			/ ** Vision Error : 104021 = Blob Analsys ����� NG(Not Good) �Դϴ�. * /
			if (iBlobResult2/1000 == 104021)
			{
				Sleep(100);
//				m_plnkVision->Grab(DEF_CAMERA_INSPECTION_2);
				Sleep(100);
				iBlobResult2 = m_plnkVision->RecognitionBLOBPattern(DEF_CAMERA_INSPECTION_2, iMarkNo, m_plnkSystemData->m_iBlobAlgorithm, TRUE);
				if (iBlobResult2)
				{
					WriteErrorLog("InspectPanel():ACF ���� �˻� ����� NG �Դϴ�.");
					bAcfInspectResult2 = FALSE;
					return generateErrorCode(311008+iCurrentJobID);
				}
			}
			else
				return iBlobResult2;
		}
	}

	WriteErrorLog("InspectPanel():ACF ���� �˻� ����� ���� �Դϴ�.");
	return ERR_CTRLHANDLER_SUCCESS;
}
/*/

/**
 *	�Ѱ� ���� Current Job ID �� �´� Inspect Camera Carrier Position ID �� ��ȯ�Ѵ�.
 *	@return int : Inspect Camera Carrier Position ID
 *	@param int iCurrentJobID : ���� Job ID
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
 *	�Ѱ� ���� Current Job ID �� Position Type �� �´� Handler Position ID �� ��ȯ�Ѵ�.
 *	@return int : Handler Position ID
 *	@param int iCurrentJobID : ���� Job ID
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
