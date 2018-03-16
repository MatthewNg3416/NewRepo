/* 
 * TrsInspectionCamera Component
 *
 * Copyright 2005 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * MTrsInspectionCamera.cpp : Implementation of MTrsInspectionCamera component.
 */

#include "Stdafx.h"
#include <math.h>
#include "MSystemData.h"
#include "MProcessData.h"
#include "MTrsAutoManager.h"
#include "MTrsInspectionCamera.h"
#include "MCtrlInspectionCamera.h"
#include "MCameraCarrier.h"
#include "ITI_InspectionCamera.h"
#include "ITI_PanelAligner.h"
#include "ITI_TabMounter.h"
#include "IAxis.h"
#include "MPreBonderData.h"
#include "common.h"
#include "MTabMounter.h"
#include "MPlatformOlbSystem.h"
#include "IOpPanel.h"
#include "MTrsPanelAligner.h"

CRITICAL_SECTION MTrsInspectionCamera::sm_rgcsSetMoving[DEF_MAX_WORKER_GROUP];

//===================================================================
BOOL MTrsInspectionCamera::sm_bGroupWorkComplete[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsInspectionCamera::sm_bAllWorkComplete[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsInspectionCamera::sm_bAlignPosMoveComplete[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
{
#ifdef DEF_SOURCE_SYSTEM
		{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
		{FALSE, FALSE},	{FALSE, FALSE},
#endif
};
BOOL MTrsInspectionCamera::sm_bITIAllMountCompleteToPA[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsInspectionCamera::sm_bITIAllMountCompleteToPASecond[DEF_MAX_GROUP] = {FALSE, FALSE};
BOOL MTrsInspectionCamera::sm_bRemoveTab = FALSE;
BOOL MTrsInspectionCamera::sm_bRecogRemoveTab[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};

int	MTrsInspectionCamera::m_iScheduleNo[] = {0, 0};
int	MTrsInspectionCamera::m_bReceivedPressCompleteSchedule[] = { FALSE, FALSE};

BOOL MTrsInspectionCamera::m_bMovetoAlignPos[] = {TRUE, TRUE};

//@130206.KKY___________
BOOL MTrsInspectionCamera::sm_bRequestUsePanelPusher[] = {FALSE, FALSE};
BOOL MTrsInspectionCamera::sm_bInspectionComplete[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};
BOOL MTrsInspectionCamera::sm_bPusherDown[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
{
#ifdef DEF_SOURCE_SYSTEM
	{FALSE, FALSE, FALSE, FALSE},	{FALSE, FALSE, FALSE, FALSE},
#else
	{FALSE, FALSE},	{FALSE, FALSE},
#endif
};
//@_____________________
BOOL MTrsInspectionCamera::sm_MountAfterVisionLogComplete[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = {{FALSE, },	};
BOOL MTrsInspectionCamera::sm_MountAfterVisionLogEnd[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = {{FALSE, },	};
//170612 JSh.s
BOOL MTrsInspectionCamera::sm_bInspectionTabBolb[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
	{
		{FALSE, },
	};
BOOL MTrsInspectionCamera::sm_bTabBlobReady[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
	{
		{FALSE, },
	};
BOOL MTrsInspectionCamera::sm_bTabBlobStart[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
	{
		{FALSE, },
	};
//171004 JSH.s
BOOL MTrsInspectionCamera::sm_bTabBlobAfterReady[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
	{
		{FALSE, },
	}
	;
BOOL MTrsInspectionCamera::sm_bTabBlobAfterStart[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
{
	{FALSE, },
	};

BOOL MTrsInspectionCamera::sm_bTabBlobPAWaitEnd[DEF_MAX_GROUP][DEF_MAX_INSPECTION_CAM_CARRIER] = 
{
	{FALSE, },
	};
//171004 JSH.e

BOOL MTrsInspectionCamera::sm_bTabBlobWorkingToPA[DEF_MAX_GROUP] = {FALSE, };
BOOL MTrsInspectionCamera::sm_bTabBlobWorkingToTM[DEF_MAX_GROUP] = {FALSE, };
//170612 JSh.e
BOOL MTrsInspectionCamera::sm_bForceUnitAll_Initialize = FALSE; //170616 JSH
//===================================================================

extern MPlatformOlbSystem	MOlbSystemPre;

/***********************************************************************************************
/*
/* 생성자 & 소멸자
/*
/***********************************************************************************************/

MTrsInspectionCamera::MTrsInspectionCamera(SCommonAttribute commonData, STrsInspectionCameraRefCompList listRefComponents, STrsInspectionCameraData datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponents);

//@#ifdef DEF_SOURCE_SYSTEM
//@	m_iWorkGroup = DEF_FRONT_GROUP;
//@	m_iWorkInstance = m_iInstanceNo;
//@#else
	if (m_iInstanceNo < DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = m_iInstanceNo - DEF_MAX_INSPECTION_CAM_CARRIER;
	}
//@#endif

	m_bAutoInitialized = FALSE;

	m_bThreadLife = FALSE;
	m_pThread = NULL;
	m_bAutoRun = FALSE;

	initializeITI();
	initializeStep();

//KKY.Del	m_iInspectTabNo = -1;
	m_iWorkTabMounterNoInGroup = DEF_NONE_TABMOUNTER;
	m_eWorkSide = m_plnkCamCarrier->GetCameraCarrierWorkSide();
	m_iGroupNo = m_plnkCamCarrier->GetCameraCarrierGroupNo();

	m_iPreInspectCamNo = m_plnkCamCarrier->GetPreCamCarrierNo();
	m_iNextInspectCamNo = m_plnkCamCarrier->GetNextCamCarrierNo();

	if (DEF_INSPECTION_CARRIER_1 == m_iInstanceNo)
	{
		for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
			InitializeCriticalSection(&sm_rgcsSetMoving[i]);
	}
	sm_bAlignPosMoveComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_bRecogRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_bRemoveTab = FALSE;

	m_iScheduleNo[m_iWorkGroup] = 0;
	m_bMovetoAlignPos[m_iWorkGroup] = TRUE;

	//@130206.KKY___________
	m_iRetryCnt = 0;
	sm_bRequestUsePanelPusher[m_iWorkGroup] = FALSE;
	sm_bInspectionComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_bPusherDown[m_iWorkGroup][m_iWorkInstance] = FALSE;
	//@_____________________
}

MTrsInspectionCamera::~MTrsInspectionCamera()
{
	if (DEF_INSPECTION_CARRIER_1 == m_iInstanceNo)
	{
		for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
			DeleteCriticalSection(&sm_rgcsSetMoving[i]);
	}
}

/***********************************************************************************************
/*
/* Private Operation
/*
/************************************************************************************************/ 
void MTrsInspectionCamera::simulLog(CString strMsg, int iStep,	int iSleep)
{
#ifdef SIMULATION
	if (m_iPreStep == iStep)
		return;

	m_iPreStep = iStep;

	CString strLog;
	strLog.Format(_T("%s : %s(%d)\n"), GetObjectName(), strMsg, iStep);

	FILE *fp = fopen( ".\\StepLog.txt", "a");
	fprintf(fp, "%s\n", LPSTR(LPCTSTR(strLog)));
	fclose(fp);

	TRACE(strLog);
	Sleep(iSleep);
#endif	// SIMULATION
}

UINT MTrsInspectionCamera::threadFunction(LPVOID pParam)
{
	MTrsInspectionCamera* pThis = (MTrsInspectionCamera*) pParam;

	TRACE(_T("MTrsInspectionCamera Thread Run\n"));

	while (pThis->m_bThreadLife)
	{
		int i = 0;
		pThis->threadJob();
		Sleep(10);
	}

	TRACE(_T("MTrsInspectionCamera Thread Stop\n"));

	return ERR_TRS_CAMERACARRIER_SUCCESS;
}

void MTrsInspectionCamera::threadJob()
{	
	switch (m_plnkTrsAutoManager->GetOPMode())
	{
	case MANUAL_MODE:
		m_bAutoRun = FALSE;
		break;

	case AUTORUN_MODE:
		switch (m_plnkTrsAutoManager->GetOPStatus())
		{
		case ERROR_STOP:
			/* FALL THROUGH */
		case STEP_STOP:
			m_TimerError.StopTimer();
			m_bAutoRun = FALSE;
			break;

		case START_RUN:
			m_bAutoRun = TRUE;
			break;

		case CYCLE_STOP:
			break;

		case RUN:
			doRunStep();
			break;			
		}
		break;
	}
}

void MTrsInspectionCamera::doRunStep()
{
	int iResult = ERR_TRS_CAMERACARRIER_SUCCESS;
	double dTargetPos = 0.0;
	int i = 0;

	BOOL b1=FALSE;
	BOOL b2=FALSE;
	BOOL b3=FALSE;
	BOOL b4=FALSE;

	if(m_iWorkGroup == DEF_REAR_GROUP)
	{
		return;
	}
	
	switch (m_estepCurrent)
	{
	case INSPECTIONCAMERA_PANEL_IN_EQUIP_WAIT:
		simulLog("INSPECTIONCAMERA_PANEL_IN_EQUIP_WAIT", m_estepCurrent, 100);

		sm_bGroupWorkComplete[m_iWorkGroup] = FALSE;
		sm_bAllWorkComplete[m_iWorkGroup] = FALSE;
		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(INSPECTIONCAMERA__MODEL_CHANGE);
			break;
		}

		/*/
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		/*/

		/*
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady = FALSE;
			setStep(INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
		*/

		if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetUseWorkSide())
			break;

		//170612 JSH.s
		if(sm_bInspectionTabBolb[m_iWorkGroup][m_iWorkInstance] == TRUE)
		{
			sm_bInspectionTabBolb[m_iWorkGroup][m_iWorkInstance] = FALSE;
			setStep(INSPECTIONCAMERA_INSPECTION_TAB_BOLB_READY);
			break;
		}
		//170612 JSH.e

		//170912 JSh.s
		if (isGroupStarter())
		{
			if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_CurrentWorkList())
			{
				m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->PickOut_CurrentWorkList();
			}
		}
		//170912 JSh.e

		// 2009.01.13 CYJ Mech ver3 Source Inspection Cam2가 Panel Align도 하면서 추가.
		// Panel Aligner 의 Panel Align 완료 확인(Inspection 위치 이동 허가) 신호 
		if (m_plnkITI_PanelAligner->IsInspectorEnabletoInspectMove() == FALSE)
			break;
		
		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			sm_bAlignPosMoveComplete[m_iWorkGroup][i] = FALSE;
		}

		setStep(INSPECTIONCAMERA_MOVE_TO_TAB_MARK_POS);
		break;

	case INSPECTIONCAMERA_MOVE_TO_TAB_MARK_POS:
		//110420.Add___
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(INSPECTIONCAMERA__MODEL_CHANGE);
			break;
		}
		//_____________

		/*/
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		/*/

		/*
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady = FALSE;
			setStep(INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
		*/

		if (FALSE == IsReadyToWork())
			break;
		sm_bGroupWorkComplete[m_iWorkGroup] = FALSE;
		sm_bAllWorkComplete[m_iWorkGroup] = FALSE;
		simulLog("INSPECTIONCAMERA_MOVE_TO_TAB_MARK_POS", m_estepCurrent, 100);

		iResult = m_plnkCamCarrier->ExpandShrinkForTabAlign(TRUE);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		dTargetPos = m_plnkCamCarrier->CalculateInspectPos(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_dInspectPos);
		if (FALSE == m_plnkCamCarrier->IsInPos(dTargetPos))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();
			iResult = m_plnkCamCarrier->CheckCollisionOtherD(dTargetPos);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					PROCESS_ALARM(generateErrorCode(414016));
				}
				break;
			}
			m_TimerError.StopTimer();
			
			iResult = m_plnkCamCarrier->MovePos(dTargetPos, DEF_CAMERACARRIER_TAB_MARK_POS);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}

		if (TRUE == m_plnkITI_PanelAligner->IsInspectorEnabletoInspect())
		{
			if (m_InspectInfo.m_nWorkTabNo != -1)
			{
				iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_VISION_POS);
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}
//		TRACE(_T("<<<<<MTrsInspectionCamera: Move InspectionPos Complete[Instance=%d, TabNo=%d, Pos=%.3lf]>>>>>\n"), m_iInstanceNo, m_InspectInfo.m_nWorkTabNo, dTargetPos);

		setStep(INSPECTIONCAMERA_INSPECT_MARK);
		break;

	case INSPECTIONCAMERA_INSPECT_MARK:

		/*/
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		/*/

		/*
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady = FALSE;
			setStep(INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
		*/

		//NSMC KJS
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(INSPECTIONCAMERA__MODEL_CHANGE);
			break;
		}

		if (TRUE == m_plnkTrsAutoManager->IsDisplayErrorImage())
			break;

		simulLog("INSPECTIONCAMERA_INSPECT_MARK", m_estepCurrent, 100);

		//Inspection Camera inspect position
		iResult = m_plnkCamCarrier->ExpandShrinkForTabAlign();
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}

		// Panel Aligner 의 Inspection 위치 이동 완료 확인 신호 
		if (m_plnkITI_PanelAligner->IsInspectorEnabletoInspect() == FALSE)
			break;

		//@KKY.130908추가______________
		if (MOlbSystemPre.GetTrsPanelAlignerComponent()->GetWorkScheduleNo() != m_iScheduleNo[m_iWorkGroup])
			break;
		//@____________________________
		
//170922 JSh.s
//		if (m_InspectInfo.m_nWorkTabNo == -1)	//작업하지 않는 CameraCarrier일경우...
//		{
//			//@131127.KKY__________________________
//			//@130206.KKY__________________________
//@			if (TRUE == m_plnkSystemData->m_bUsePanelPusher)
//			if (m_plnkSystemData->m_bUsePanelPusher == 1)
//			//@____________________________________
//			{
//				sm_bInspectionComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;
//				sm_bPusherDown[m_iWorkGroup][m_iWorkInstance] = TRUE;
//				while(1)
//				{
//					Sleep(10);
//					if (isAllInspectionCompleteGroup())
//						break;
//					if (m_plnkTrsAutoManager->GetOPStatus() != RUN)
//						return;
//				}
//			}
//			//@____________________________________
//			setStep(INSPECTIONCAMERA_WAIT_GROUPWORK_DONE);
//			break;
//		}	
//170922 JSh.e	

//170922 JSh.s
		if (m_InspectInfo.m_nWorkTabNo != -1)	//작업하지 않는 CameraCarrier일경우...
		{
			m_iWorkTabMounterNoInGroup = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetTabMounterNoFromCurrentWorkList(m_InspectInfo.m_nWorkTabNo);
			if (m_iWorkTabMounterNoInGroup == -1)
				break;
			
			if (m_plnkITI_TabMounter[m_iWorkTabMounterNoInGroup]->GetMountTabNo() != m_InspectInfo.m_nWorkTabNo)//m_iInspectTabNo)
				break;
			
			if (m_plnkITI_TabMounter[m_iWorkTabMounterNoInGroup]->IsInspectionAbleToIC() == FALSE)
				break;
		}
//170922 JSh.e

#ifndef SIMULATION
		dTargetPos = m_plnkCamCarrier->CalculateInspectPos(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_dInspectPos);
		if (FALSE == m_plnkCamCarrier->IsInPos(dTargetPos))
		{
			//170918 JSH.s
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();
			iResult = m_plnkCamCarrier->CheckCollisionOtherD(dTargetPos);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					SetErrorLevel(_T("INSPECTION CAMERA Move Align Pos"),0, __FILE__, __LINE__);
					PROCESS_ALARM(generateErrorCode(414016));
				}
				break;
			}
			m_TimerError.StopTimer();
			//170918 JSH.e

			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
			
			iResult = m_plnkCamCarrier->MovePos(dTargetPos, DEF_CAMERACARRIER_TAB_MARK_POS);
			if (iResult)
			{
				SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
				// 414000 = Tab Inspection 작업 위치 아님.
				PROCESS_ALARM(iResult); //170916
				break;
			}
		}

		if (FALSE == m_plnkCamCarrier->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_VISION_POS))
		{
			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_VISION_POS);
			if (iResult)
			{
				SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
#endif

		m_rgbITIInspectionStartToTM = TRUE;

		// TabMounter::InspectionWaitStart 확인.
//170922 JSh.s
		if (m_InspectInfo.m_nWorkTabNo != -1)	//작업하지 않는 CameraCarrier일경우...
		{
			m_Timer4ITI.StartTimer();
			while (TRUE)
			{
				BOOL isInspectStart[DEF_MAX_WORKER_PER_GROUP];
				for (int i=0; i<DEF_MAX_WORKER_PER_GROUP; i++)
					isInspectStart[i] = m_plnkITI_TabMounter[i]->IsInspectionWaitStartToIC();
				if (m_plnkITI_TabMounter[m_iWorkTabMounterNoInGroup]->IsInspectionWaitStartToIC() == TRUE)
					break;
				
				if (FALSE == m_plnkITI_TabMounter[m_iWorkTabMounterNoInGroup]->IsInspectionAbleToIC())
				{
					return;
				}
				
				//120116.KMS________
				if(TRUE == m_plnkTrsAutoManager->GetMountAutoChangeInitStart())
					break;
				//__________________
				
				if (m_Timer4ITI.MoreThan(20))
				{
					m_rgbITIInspectionStartToTM = FALSE;
					PROCESS_ALARM(generateErrorCode(414012));
					return;
				}
				
				Sleep(10);
			}
		}
//170922 JSh.e
		//@131127.KKY__________________________
		//@130206.KKY__________________________
//@		if (TRUE == m_plnkSystemData->m_bUsePanelPusher)
		if (m_plnkSystemData->m_bUsePanelPusher == 1)
		//@____________________________________
		{
			if (TRUE == sm_bRequestUsePanelPusher[m_iWorkGroup])
			{
				m_plnkTabMounter[m_iWorkTabMounterNoInGroup]->DownPusherCyl();
				sm_bPusherDown[m_iWorkGroup][m_iWorkInstance] = TRUE;
				while(1)
				{
					Sleep(10);
					if (TRUE == isAllPusherDownGroup())
						break;
					if (m_plnkTrsAutoManager->GetOPStatus() != RUN)
					{
						m_rgbITIInspectionStartToTM = FALSE;
						m_rgbITIInspectionCompleteToTM = FALSE;
						return;
					}
				}
			}
		}
		//@_________________________

//20111020 by kss
/////////////////////////////////////////////////////////
//170922 JSh.s
		if (m_InspectInfo.m_nWorkTabNo != -1)	//작업하지 않는 CameraCarrier일경우...
		{
			iResult = m_plnkCtrlInspectCam->InspectBeforeMount(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_nRealTabNo, m_plnkTrsAutoManager->GetGlassID(DEF_PANEL_ALIGNER_PANEL));
			/////////////////////////////////////////////////////////
			if (iResult)
			{
				//@130206________________________
				if (iResult/1000 != 104030 )
				{
					//@131127.KKY__________________________
					//@			if (TRUE == m_plnkSystemData->m_bUsePanelPusher)
					if (m_plnkSystemData->m_bUsePanelPusher == 1)
						//@____________________________________
					{
						m_iRetryCnt++;
						if (m_iRetryCnt < 3)
						{
							sm_bRequestUsePanelPusher[m_iWorkGroup] = TRUE;
							m_rgbITIInspectionStartToTM = FALSE;
							m_rgbITIInspectionCompleteToTM = FALSE;
							return;
						}
					}
				}
				//@______________________________
				
				//@130206.KKY__________
				//170715_KDH 임시삭제
				//			if (FALSE == m_plnkTabMounter[m_iWorkTabMounterNoInGroup]->IsUpPusherCyl())
				//				m_plnkTabMounter[m_iWorkTabMounterNoInGroup]->UpPusherCyl();
				//_______________
				sm_bRequestUsePanelPusher[m_iWorkGroup] = FALSE;
				sm_bPusherDown[m_iWorkGroup][m_iWorkInstance] = FALSE;
				//@___________________
				
				int iCamNo = DEF_INSPECTION_CAMERA_1;
				m_rgbITIInspectionStartToTM = FALSE;
				
				switch (iResult / 1000)
				{
					// TabIC Mark 인식 실패 Error 이면 Retry!
				case 324038:	// 324008 = Inspection Vision Mark 인식 실패. [Camera 5 : TabIC Mark 인식 개수가 1 개가 아님.]
				case 324039:	// 324009 = Inspection Vision Mark 인식 실패. [Camera 6 : TabIC Mark 인식 개수가 1 개가 아님.]
				case 324040:	// 324010 = Inspection Vision Mark 인식 실패. [Camera 7 : TabIC Mark 인식 개수가 1 개가 아님.]
				case 324041:	// 324011 = Inspection Vision Mark 인식 실패. [Camera 8 : TabIC Mark 인식 개수가 1 개가 아님.]
				case 324042:	// 324012 = Inspection Vision Mark 인식 실패. [Camera 9 : TabIC Mark 인식 개수가 1 개가 아님.]
				case 324043:	// 324013 = Inspection Vision Mark 인식 실패. [Camera 10 : TabIC Mark 인식 개수가 1 개가 아님.]
				case 324044:	// 324014 = Inspection Vision Mark 인식 실패. [Camera 11 : TabIC Mark 인식 개수가 1 개가 아님.]
				case 324045:	// 324014 = Inspection Vision Mark 인식 실패. [Camera 12 : TabIC Mark 인식 개수가 1 개가 아님.]
					//				iCamNo = (iResult / 1000 - 324038) + DEF_INSPECTION_CAMERA_1;
					//				break;
				case 324054:	// 324023 = Inspection Vision Mark 인식 실패. [Camera 5 : TabIC Mark 인식률 낮음.]
				case 324055:	// 324024 = Inspection Vision Mark 인식 실패. [Camera 6 : TabIC Mark 인식률 낮음.]
				case 324056:	// 324025 = Inspection Vision Mark 인식 실패. [Camera 7 : TabIC Mark 인식률 낮음.]
				case 324057:	// 324026 = Inspection Vision Mark 인식 실패. [Camera 8 : TabIC Mark 인식률 낮음.]
				case 324058:	// 324027 = Inspection Vision Mark 인식 실패. [Camera 9 : TabIC Mark 인식률 낮음.]
				case 324059:	// 324028 = Inspection Vision Mark 인식 실패. [Camera 10 : TabIC Mark 인식률 낮음.]
				case 324060:	// 324029 = Inspection Vision Mark 인식 실패. [Camera 11 : TabIC Mark 인식률 낮음.]
				case 324061:	// 324029 = Inspection Vision Mark 인식 실패. [Camera 12 : TabIC Mark 인식률 낮음.]
					//				iCamNo = (iResult / 1000 - 324054) + DEF_INSPECTION_CAMERA_1;
					//				break;
					
					// Panel Tab Mark 인식 실패이면, Error Display 하고 이 Step 재시도!
				case 324030:	// 324001 = Inspection Vision Mark 인식 실패. [Camera 5 : Panel Mark 인식 개수가 1 개가 아님.]
				case 324031:	// 324002 = Inspection Vision Mark 인식 실패. [Camera 6 : Panel Mark 인식 개수가 1 개가 아님.]
				case 324032:	// 324003 = Inspection Vision Mark 인식 실패. [Camera 7 : Panel Mark 인식 개수가 1 개가 아님.]
				case 324033:	// 324004 = Inspection Vision Mark 인식 실패. [Camera 8 : Panel Mark 인식 개수가 1 개가 아님.]
				case 324034:	// 324005 = Inspection Vision Mark 인식 실패. [Camera 9 : Panel Mark 인식 개수가 1 개가 아님.]
				case 324035:	// 324006 = Inspection Vision Mark 인식 실패. [Camera 10 : Panel Mark 인식 개수가 1 개가 아님.]
				case 324036:	// 324007 = Inspection Vision Mark 인식 실패. [Camera 11 : Panel Mark 인식 개수가 1 개가 아님.]
				case 324037:	// 324016 = Inspection Vision Mark 인식 실패. [Camera 12 : Panel Mark 인식 개수가 1 개가 아님.]
					
				case 324046:	// 324017 = Inspection Vision Mark 인식 실패. [Camera 5 : Panel Mark 인식률 낮음.]
				case 324047:	// 324018 = Inspection Vision Mark 인식 실패. [Camera 6 : Panel Mark 인식률 낮음.]
				case 324048:	// 324019 = Inspection Vision Mark 인식 실패. [Camera 7 : Panel Mark 인식률 낮음.]
				case 324049:	// 324020 = Inspection Vision Mark 인식 실패. [Camera 8 : Panel Mark 인식률 낮음.]
				case 324050:	// 324021 = Inspection Vision Mark 인식 실패. [Camera 9 : Panel Mark 인식률 낮음.]
				case 324051:	// 324022 = Inspection Vision Mark 인식 실패. [Camera 10 : Panel Mark 인식률 낮음.]
				case 324052:	// 324022 = Inspection Vision Mark 인식 실패. [Camera 11 : Panel Mark 인식률 낮음.]
				case 324053:	// 324022 = Inspection Vision Mark 인식 실패. [Camera 12 : Panel Mark 인식률 낮음.]
					// Fall Through.
				default:
					m_rgbITIInspectionStartToTM = FALSE;
					m_rgbITIInspectionCompleteToTM = FALSE;
					//에러난 이미지를 MainView Display창에 표시하기 위해
					Sleep(200);
					//20111020 by kss
					/////////////////////////////////////////////////////////
					m_plnkCtrlInspectCam->InspectBeforeMount(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_nWorkTabNo, m_plnkTrsAutoManager->GetGlassID(DEF_PANEL_ALIGNER_PANEL));
					/////////////////////////////////////////////////////////
					PROCESS_ALARM(iResult);
					return;
				}
				
				if (FALSE == m_plnkTrsAutoManager->IsDisplayErrorImage())
				{
					m_plnkTrsAutoManager->SetDisplayErrorImage(TRUE);	
					Sleep(1000);
					::SendMessage(m_plnkTrsAutoManager->GetTitleViewHandle(),
						WM_DISP_INSPECTION_JUDGE_DLG_REQ, WPARAM(iCamNo), (LPARAM)(&iResult));				
					
					if (iResult == DEF_TABINSPECTION_RUN_STOP)
					{
						m_plnkTrsAutoManager->SetOPStatus(STEP_STOP);
						m_rgbITIInspectionStartToTM = FALSE;
						m_rgbITIInspectionCompleteToTM = FALSE;
						return;
					}
					else if (iResult == DEF_TABINSPECTION_TAB_RELOAD)
					{
						m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->SetTabSkipToCurrentWorkList(m_InspectInfo.m_nWorkTabNo);
						m_rgbITIInspectionCompleteToTM = TRUE;
						
						// TabMounter::InspectionWait 종료 확인.
						m_Timer4ITI.StartTimer();
						while (TRUE)
						{
							if (m_plnkITI_TabMounter[m_iWorkTabMounterNoInGroup]->IsInspectionWaitStartToIC() == FALSE)
								break;
							Sleep(5);
						}
						m_rgbITIInspectionStartToTM = FALSE;
						m_rgbITIInspectionCompleteToTM = FALSE;
						sm_bRemoveTab = TRUE;
						setStep(INSPECTIONCAMERA_WAIT_GROUPWORK_DONE);
						break;
					}
				}
				else
				{
					m_rgbITIInspectionStartToTM = FALSE;
					m_rgbITIInspectionCompleteToTM = FALSE;
					return;
				}			
			}
		}
//170922 JSh.e
		//@130206.모든 Inspection이 완료될때까지 기다린다_________
		sm_bInspectionComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;
		//@131127.KKY__________________________
//@		if (TRUE == m_plnkSystemData->m_bUsePanelPusher)
		if (m_plnkSystemData->m_bUsePanelPusher == 1)
		//@____________________________________
		{
			while(1)
			{
				Sleep(10);
				if (isAllInspectionCompleteGroup())
					break;
				if (m_plnkTrsAutoManager->GetOPStatus() != RUN
					|| TRUE == sm_bRequestUsePanelPusher[m_iWorkGroup])
				{
					m_rgbITIInspectionStartToTM = FALSE;
					m_rgbITIInspectionCompleteToTM = FALSE;
					return;
				}
			}
		}
		//170918 jsh.S
		else
		{
			while(1)
			{
				Sleep(20);
 				if (isAllInspectionCompleteGroup())
					break;
				if (m_plnkTrsAutoManager->GetOPStatus() != RUN)
				{
					m_rgbITIInspectionStartToTM = FALSE;
					m_rgbITIInspectionCompleteToTM = FALSE;
					return;
				}
			}
		}
		//170918 jsh.E

		//170715_KDH 임시삭제	
//		if (FALSE == m_plnkTabMounter[m_iWorkTabMounterNoInGroup]->IsUpPusherCyl())
//			m_plnkTabMounter[m_iWorkTabMounterNoInGroup]->UpPusherCyl();
		//_____________
		sm_bPusherDown[m_iWorkGroup][m_iWorkInstance] = FALSE;
		m_iRetryCnt = 0;
		//@_______________________________________________________

		m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->SetInspectionComplete(m_InspectInfo.m_nWorkTabNo);
		
		m_rgbITIInspectionCompleteToTM = TRUE;
		
		// TabMounter::InspectionWait 종료 확인.
//170922 JSh.s
		if (m_InspectInfo.m_nWorkTabNo != -1)	//작업하지 않는 CameraCarrier일경우...
		{
			m_Timer4ITI.StartTimer();
			while (TRUE)
			{
				if (m_plnkITI_TabMounter[m_iWorkTabMounterNoInGroup]->IsInspectionWaitStartToIC() == FALSE)
					break;
				//170905 JSh.s 안정화
				if (m_plnkTrsAutoManager->GetOPStatus() != RUN)
				{
					m_rgbITIInspectionStartToTM = FALSE;
					m_rgbITIInspectionCompleteToTM = FALSE;
					return;
				}
				//170905 JSh.e
				Sleep(10);
			}
		}
//170922 JSh.e
		m_rgbITIInspectionStartToTM = FALSE;
		m_rgbITIInspectionCompleteToTM = FALSE;
		
		//170905 JSH.s
		setStep(INSPECTIONCAMERA_MOVE_TO_MOUNT_POS);
		break;
		
	case INSPECTIONCAMERA_MOVE_TO_MOUNT_POS:
		//170905 JSH.e
//Inspection 동기화
		//170826 JSH.s
#ifndef SIMULATION
		dTargetPos = m_plnkCamCarrier->CalculateMountPos(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_dInspectPos);
		if (FALSE == m_plnkCamCarrier->IsInPos(dTargetPos))
		{
			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
			iResult = m_plnkCamCarrier->MovePos(dTargetPos, DEF_CAMERACARRIER_TAB_MOUNT_POS);
			if (iResult)
			{
				SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
				// 414000 = Tab Inspection 작업 위치 아님.
				PROCESS_ALARM(iResult); //170916
				break;
			}
		}
		
		if (FALSE == m_plnkCamCarrier->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_UP_POS))
		{
			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_UP_POS);
			if (iResult)
			{
				SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
				PROCESS_ALARM(iResult);
				break;
			}
		}
#endif
		//170826 JSH.e

		//170905 JSH.s	
		// TabMounter::InspectionMountReadyWait 시작 확인.
//170922 JSh.s
		if (m_InspectInfo.m_nWorkTabNo != -1 &&
			m_iWorkTabMounterNoInGroup != -1)	//작업하지 않는 CameraCarrier일경우...
		{
			while (TRUE)
			{
				if (m_plnkITI_TabMounter[m_iWorkTabMounterNoInGroup]->IsInspectionMountReadyWaitToIC() == TRUE)
					break;
				//170905 JSh.s 안정화
				if (m_plnkTrsAutoManager->GetOPStatus() != RUN)
					return;
				//170905 JSh.e
				Sleep(10);
			}
		}
//170922 JSh.e
		m_rgbITIMountReadyToTM = TRUE;
		
		// TabMounter::InspectionMountReadyWait 종료 확인.
//170922 JSh.s
		if (m_InspectInfo.m_nWorkTabNo != -1&&
			m_iWorkTabMounterNoInGroup != -1)	//작업하지 않는 CameraCarrier일경우...
		{
			while (TRUE)
			{
				if (m_plnkITI_TabMounter[m_iWorkTabMounterNoInGroup]->IsInspectionMountReadyWaitToIC() == FALSE)
					break;
				//170905 JSh.s 안정화
				if (m_plnkTrsAutoManager->GetOPStatus() != RUN)
				{
					m_rgbITIMountReadyToTM = FALSE;
					return;
				}
				//170905 JSh.e
				Sleep(10);
			}
			m_rgbITIMountReadyToTM = FALSE;
		}
//170922 JSh.e
		//170905 JSH.e

//		setStep(INSPECTIONCAMERA_PANEL_IN_EQUIP_WAIT);
		setStep(INSPECTIONCAMERA_WAIT_GROUPWORK_DONE);
		break;

	case INSPECTIONCAMERA_WAIT_GROUPWORK_DONE:
		//110420.Add___
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(INSPECTIONCAMERA__MODEL_CHANGE);
			break;
		}
		//_____________

		//버리고 다시 Inspection해야할 Tab이 있는 경우
		if (sm_bRemoveTab)
		{
			sm_bRecogRemoveTab[m_iWorkGroup][m_iWorkInstance] = TRUE;
			if (FALSE == isGroupStarter())
			{
				if (m_InspectInfo.m_nWorkTabNo != -1)
					setStep(INSPECTIONCAMERA_INSPECT_MARK);
			}
			else
			{
				for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
				{
					if (FALSE == sm_bRecogRemoveTab[m_iWorkGroup][i])
						return;
				}

				for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
					sm_bRecogRemoveTab[m_iWorkGroup][i] = FALSE;
				sm_bRemoveTab = FALSE;
				if (m_InspectInfo.m_nWorkTabNo != -1)
					setStep(INSPECTIONCAMERA_INSPECT_MARK);
			}
			break;
		}
		//170905 JSH.e
		if (isGroupStarter())
		{
			if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsPressComplete_CurrentWorkList())
			{
				Sleep(10);
				sm_bGroupWorkComplete[m_iWorkGroup] = TRUE;
			}
		}
		
		if (FALSE == sm_bGroupWorkComplete[m_iWorkGroup])
			break;
		
		setStep(INSPECTIONCAMERA_MOUNT_AFTER_VISION_LOG_REDAY);
		break;
		
	case INSPECTIONCAMERA_MOUNT_AFTER_VISION_LOG_REDAY:
		//110420.Add___
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(INSPECTIONCAMERA__MODEL_CHANGE);
			break;
		}
		//_____________

		if(m_plnkSystemData->m_eRunMode != PASS_RUN_MODE && m_plnkSystemData->m_bUseBondingAfterImgLog == TRUE) //170927 JSH
			m_FlgMountAfterVisionLog = TRUE;
		else
			m_FlgMountAfterVisionLog = FALSE;

		if(sm_MountAfterVisionLogComplete[m_iWorkGroup][m_iWorkInstance] == TRUE)
		{
			
			// 414020 = InterFace초기화가 되어 있지 않습니다. 유닛 초기화 이후 진행해 주세요
			PROCESS_ALARM(generateErrorCode(414020));
				break;
		}

		setStep(INSPECTIONCAMERA_MOUNT_AFTER_VISION_LOG);
		break;
		
	case INSPECTIONCAMERA_MOUNT_AFTER_VISION_LOG:
		//110420.Add___
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(INSPECTIONCAMERA__MODEL_CHANGE);
			break;
		}
		//_____________
		if(m_FlgMountAfterVisionLog)
		{
			m_FlgMountAfterVisionLog = FALSE;
#ifndef SIMULATION
			dTargetPos = m_plnkCamCarrier->CalculateInspectPos(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_dInspectPos);
			if (FALSE == m_plnkCamCarrier->IsInPos(dTargetPos))
			{
				iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
				if (iResult)
				{
					SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
					PROCESS_ALARM(iResult);
					break;
				}
				iResult = m_plnkCamCarrier->MovePos(dTargetPos, DEF_CAMERACARRIER_TAB_MARK_POS);
				if (iResult)
				{
					SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
					// 414002 = Tab Inspection 작업 위치 아님.
					PROCESS_ALARM(generateErrorCode(414002));
					break;
				}
			}
			
			if (FALSE == m_plnkCamCarrier->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_VISION_POS))
			{
				iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_VISION_POS);
				if (iResult)
				{
					SetErrorLevel(_T("INSPECTION CAMERA inspect Mark"),0, __FILE__, __LINE__);
					PROCESS_ALARM(iResult);
					break;
				}
			}
#endif	
			//if(m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
			{
				iResult = m_plnkCtrlInspectCam->WriteTabImageLog(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_nWorkTabNo);
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}
		
		//해당 구문 최초 실행 전 FALSE 상태여야 한다.
		sm_MountAfterVisionLogComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;

		if (isGroupStarter())
		{
			BOOL bAllComplete = TRUE;
			for(int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
				if(sm_MountAfterVisionLogComplete[m_iWorkGroup][i] == FALSE)
					bAllComplete = FALSE;
			if(bAllComplete)
				for(int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
					sm_MountAfterVisionLogEnd[m_iWorkGroup][i] = TRUE;
		}

		if(!sm_MountAfterVisionLogEnd[m_iWorkGroup][m_iWorkInstance])
			break;
		sm_MountAfterVisionLogComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_MountAfterVisionLogEnd[m_iWorkGroup][m_iWorkInstance] = FALSE;

		if (isGroupStarter())
		{
			Sleep(10);
			if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsPressComplete_AllWorkList())
			{
				m_bReceivedPressCompleteSchedule[m_iWorkGroup] = TRUE;
				m_iScheduleNo[m_iWorkGroup]++;
				
				//170719_KDH 작업할꺼 없으면 Skip Test중.
				for (int i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
				{
					MListWorkTab ListWorkTab;
					m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
					if(ListWorkTab.GetTotalWorkTabCount() > 0)
						break;
				}
				m_iScheduleNo[m_iWorkGroup] = i;
				//____________________
				if (m_iScheduleNo[m_iWorkGroup] >= DEF_MAX_WORK_SCHEDULE)
				{
					//170719_KDH 작업할꺼 없으면 Skip Test중.
					m_iScheduleNo[m_iWorkGroup] = 0;	//@
					for (int i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
					{
						MListWorkTab ListWorkTab;
						m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
						if(ListWorkTab.GetTotalWorkTabCount() > 0)
							break;			
					}
					m_iScheduleNo[m_iWorkGroup] = i;
					//_______________________
					
					m_bMovetoAlignPos[m_iWorkGroup] = TRUE;
				}
				else
				{
					m_bMovetoAlignPos[m_iWorkGroup] = FALSE;
				}
				//@__________________________
				
				sm_bAllWorkComplete[m_iWorkGroup] = TRUE;
			}
			else
			{
				sm_bAllWorkComplete[m_iWorkGroup] = FALSE;
			}
			sm_bGroupWorkComplete[m_iWorkGroup] = TRUE;
			sm_bITIAllMountCompleteToPA[m_iWorkGroup] = TRUE;
		}
		
		iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
		if (iResult)
		{
			PROCESS_ALARM(iResult);
			break;
		}
		//@130206.KKY________
		sm_bInspectionComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_bRequestUsePanelPusher[m_iWorkGroup] = FALSE;
		//@__________________
		setStep(INSPECTIONCAMERA_ALL_WORK_DONE);
		break;

	case INSPECTIONCAMERA_ALL_WORK_DONE:

		//110420.Add___
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
		{
			m_bModelChangeReady = FALSE;
			setStep(INSPECTIONCAMERA__MODEL_CHANGE);
			break;
		}
		//_____________

		/*/
		if (TRUE == is1FeederAllMounterInitStart())
		{
			m_b1FeederAllMountInitReady = FALSE;
			setStep(INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		/*/

		/*
		//120116.KMS__________
		if(m_plnkTrsAutoManager->GetMountAutoChange() == TRUE)
		{
			m_bMountAutoChangeReady = FALSE;
			setStep(INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY);
			break;
		}
		//____________________
		*/

		if (TRUE == m_plnkITI_PanelAligner->IsWorkerEnabletoWork())
			break;
		if (isGroupStarter())
			sm_bITIAllMountCompleteToPA[m_iWorkGroup] = FALSE;

		if (FALSE == sm_bAllWorkComplete[m_iWorkGroup])
		{
			setStep(INSPECTIONCAMERA_MOVE_TO_TAB_MARK_POS);
			break;
		}

		Sleep(1000);
		if (FALSE == m_plnkPrebonderData->m_bUseTabPanelAlign && m_bMovetoAlignPos[m_iWorkGroup] == TRUE)
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();
			//iResult = m_plnkCamCarrier->CheckCollisionOther(m_iScheduleNo[m_iWorkGroup], DEF_CAMERACARRIER_PANEL_MARK_POS, -1);
			iResult = m_plnkCamCarrier->CheckCollisionOther(0, DEF_CAMERACARRIER_PANEL_MARK_POS, -1);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					PROCESS_ALARM(generateErrorCode(414015));
				}
				break;
			}
			m_TimerError.StopTimer();
			
//			TRACE(_T("Panel Align 하러 갑니다.\n"));
			//iResult = m_plnkCamCarrier->SafeMovePos(m_iScheduleNo[m_iWorkGroup], DEF_CAMERACARRIER_PANEL_MARK_POS, TRUE);
			iResult = m_plnkCamCarrier->SafeMovePos(0, DEF_CAMERACARRIER_PANEL_MARK_POS, TRUE);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		else
		{
			m_InspectInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetInspectCarrierInfo_FromFirstWorkList(m_iWorkInstance);
			dTargetPos = m_plnkCamCarrier->CalculateInspectPos(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_dInspectPos);
			if (FALSE == m_plnkCamCarrier->IsInPos(dTargetPos))
			{
				if (FALSE == m_TimerError.IsTimerStarted())
					m_TimerError.StartTimer();
				iResult = m_plnkCamCarrier->CheckCollisionOtherD(dTargetPos);
				if (iResult)
				{
					if (m_TimerError.MoreThan(30.0))
					{
						m_TimerError.StopTimer();
						PROCESS_ALARM(generateErrorCode(414016));
					}
					break;
				}
				m_TimerError.StopTimer();
				
				m_plnkCamCarrier->ExpandShrinkForTabAlign(TRUE);
//				TRACE(_T("Tab Align 하러 갑니다.\n"));
			
				iResult = m_plnkCamCarrier->MovePos(dTargetPos, DEF_CAMERACARRIER_TAB_MARK_POS);
				if (iResult)
				{
					PROCESS_ALARM(iResult);
					break;
				}
			}
		}

		sm_bAlignPosMoveComplete[m_iWorkGroup][m_iWorkInstance] = TRUE;

		for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		{
			if (FALSE == sm_bAlignPosMoveComplete[m_iWorkGroup][i])
			{
				return;
			}
		}

		setStep(INSPECTIONCAMERA_PANEL_IN_EQUIP_WAIT);
		break;

		//NSMC KJS
	case INSPECTIONCAMERA__MODEL_CHANGE: simulLog("INSPECTIONCAMERA__MODEL_CHANGE", m_estepCurrent, 500);
		// Model Change 완료를 TrsAutoManager 에게 통보.
		m_bModelChangeReady = TRUE;
		
		// 확인 필요 : TrsAutoManager 에서 신호를 하나만 사용할 것인가? 두개로 사용할 것인가?
		if (TRUE == m_plnkTrsAutoManager->IsStartModelChange())
			break;
		
		m_bModelChangeReady = FALSE;
		
		setStep(INSPECTIONCAMERA_PANEL_IN_EQUIP_WAIT);
		break;
		
	case INSPECTIONCAMERA_READY_MOVE_FOR_FEEDERREADY:
		if (FALSE == m_plnkCamCarrier->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_DOWN_POS))
		{
			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}
		m_b1FeederAllMountInitReady = FALSE;
		setStep(INSPECTIONCAMERA_WAIT_1FEEDERALLMOUNT_INIT);
		break;

	case INSPECTIONCAMERA_WAIT_1FEEDERALLMOUNT_INIT:

		//120116.KMS________
		m_bMountAutoChangeReady = TRUE;
		if(TRUE == m_plnkTrsAutoManager->GetMountAutoChangeInitStart())
			break;

		if(m_plnkSystemData->m_bUse1FeederAllMount == TRUE)
		{
			m_b1FeederAllMountInitReady = TRUE;
			if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart())
			{
				break;
			}
		}
		m_bMountAutoChangeReady = FALSE;
		//_________________

		m_InspectInfo.Init();
		sm_bGroupWorkComplete[m_iWorkGroup] = FALSE;
		sm_bAllWorkComplete[m_iWorkGroup] = FALSE;
		sm_bITIAllMountCompleteToPA[m_iWorkGroup] = FALSE;
		sm_bAlignPosMoveComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_bRecogRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_bRemoveTab = FALSE;
		m_TimerError.StopTimer();
		initializeITI();
		m_b1FeederAllMountInitReady = FALSE;
		Sleep(1000);
		setStep(INSPECTIONCAMERA_PANEL_IN_EQUIP_WAIT);
		break;

		//170612 JSH.s
	case INSPECTIONCAMERA_INSPECTION_TAB_BOLB_READY:		
		sm_bTabBlobReady[m_iWorkGroup][m_iWorkInstance] = TRUE;
		
		if(isGroupStarter() == TRUE)
		{
			BOOL bAllReady = TRUE;
			for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			{
				if (sm_bTabBlobReady[m_iWorkGroup][i] == FALSE)
				{
					bAllReady = FALSE;
					break;
				}
			}

			if(bAllReady)
			{
				//170922 JSH.s
				//모든 워크 스케쥴 검사 완료
				BOOL bPreBonderMoreWork;
				bPreBonderMoreWork = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsMoreWorkForCurrentPanel();
				
				if(bPreBonderMoreWork == FALSE)
				{	
					m_iScheduleNo[m_iWorkGroup]++;
					
					//170719_KDH 작업할꺼 없으면 Skip Test중.
					int p;
					for(p = m_iScheduleNo[m_iWorkGroup]; p < DEF_MAX_WORK_SCHEDULE; p++)
					{
						MListWorkTab ListWorkTab;
						m_plnkWorkSchedule[p]->GetOrgWorkList(&ListWorkTab);
						if(ListWorkTab.GetTotalWorkTabCount() > 0)
							break;
					}
					m_iScheduleNo[m_iWorkGroup] = p;
					//____________________
				}
				//170922 JSH.e
				
				for (int j=0; j<DEF_MAX_INSPECTION_CAM_CARRIER; j++)
				{
					sm_bTabBlobStart[m_iWorkGroup][j] = TRUE;
				}					
			}
		}
		
		if(sm_bTabBlobStart[m_iWorkGroup][m_iWorkInstance] == FALSE)
			break;

		sm_bTabBlobReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_bTabBlobStart[m_iWorkGroup][m_iWorkInstance] = FALSE;

		if (m_iScheduleNo[m_iWorkGroup] >= DEF_MAX_WORK_SCHEDULE)
			setStep(INSPECTIONCAMERA_INSPECTION_TAB_BOLB_AFTER);
		else		
			setStep(INSPECTIONCAMERA_INSPECTION_TAB_BOLB_PA_WAIT);
		//_________________

		//170616 JSH.e
		break;

	case INSPECTIONCAMERA_INSPECTION_TAB_BOLB_PA_WAIT:
		
		//171004 JSH.s
		if(isGroupStarter() == TRUE)
		{
			if(m_plnkITI_PanelAligner->IsReadyInspectionBolbPos(m_iScheduleNo[m_iWorkGroup]) == FALSE)
				break;

			//170925 JSh.s
			iResult = MOlbSystemPre.GetTrsPanelAlignerComponent()->moveModelChangeWorkPos(TRUE);
			if(iResult)
			{					//다시 전체 초기화 필요!!
				sm_bForceUnitAll_Initialize = TRUE; //170616 JSH
				m_plnkTrsAutoManager->ResetUnitInitialFlag();
				
				sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
				sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
				
				PROCESS_ALARM(iResult);
				return;
			}
			//170925 JSh.e
			
			for (int j=0; j<DEF_MAX_INSPECTION_CAM_CARRIER; j++)
			{
				sm_bTabBlobPAWaitEnd[m_iWorkGroup][j] = TRUE;
			}
		}
		if(sm_bTabBlobPAWaitEnd[m_iWorkGroup][m_iWorkInstance] == FALSE)
			break;
		sm_bTabBlobPAWaitEnd[m_iWorkGroup][m_iWorkInstance] = FALSE;
		
		//171004 JSH.e

		setStep(INSPECTIONCAMERA_INSPECTION_TAB_BOLB);
		break;
		
	case INSPECTIONCAMERA_INSPECTION_TAB_BOLB:
		if(IsReadyToWork() == FALSE)
			break;

		//170616 JSH.s
		dTargetPos = m_plnkCamCarrier->CalculateInspectPos(m_iScheduleNo[m_iWorkGroup], m_InspectInfo.m_dInspectPos);
		if (FALSE == m_plnkCamCarrier->IsInPos(dTargetPos))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();
			iResult = m_plnkCamCarrier->CheckCollisionOtherD(dTargetPos);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					//다시 전체 초기화 필요!!
					sm_bForceUnitAll_Initialize = TRUE; //170616 JSH
					m_plnkTrsAutoManager->ResetUnitInitialFlag();
					
					sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
					sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
					PROCESS_ALARM(generateErrorCode(414016));
					break;
				}
				break;
			}
			m_TimerError.StopTimer();
			
			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				//다시 전체 초기화 필요!!
				sm_bForceUnitAll_Initialize = TRUE; //170616 JSH
				m_plnkTrsAutoManager->ResetUnitInitialFlag();
				
				sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
				sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
				PROCESS_ALARM(iResult);
				break;
			}
			
			iResult = m_plnkCamCarrier->MovePos(dTargetPos, DEF_CAMERACARRIER_TAB_MARK_POS);
			if (iResult)
			{
				//다시 전체 초기화 필요!!
				sm_bForceUnitAll_Initialize = TRUE; //170616 JSH
				m_plnkTrsAutoManager->ResetUnitInitialFlag();
				
				sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
				sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
				PROCESS_ALARM(iResult);
				break;
			}
		}
		
		if (FALSE == m_plnkCamCarrier->IsInPosBackup(DEF_CAMERACARRIER_BACKUP_VISION_POS))
		{
			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_VISION_POS);
			if (iResult)
			{
				//다시 전체 초기화 필요!!
				sm_bForceUnitAll_Initialize = TRUE; //170616 JSH
				m_plnkTrsAutoManager->ResetUnitInitialFlag();
				
				sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
				sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
				PROCESS_ALARM(iResult);
				break;
			}
		}
		//170616 JSH.e
		//170922 JSH.s
		if (m_InspectInfo.m_nWorkTabNo != -1)
		{
			iResult = m_plnkCtrlInspectCam->InspectTabAttached(m_InspectInfo.m_nWorkTabNo);
			if (iResult)
			{
				//다시 전체 초기화 필요!!
				sm_bForceUnitAll_Initialize = TRUE; //170616 JSH
				m_plnkTrsAutoManager->ResetUnitInitialFlag();
				
				sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
				sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
				PROCESS_ALARM(iResult);
				break;
			}

			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				//다시 전체 초기화 필요!!
				sm_bForceUnitAll_Initialize = TRUE; //170616 JSH
				m_plnkTrsAutoManager->ResetUnitInitialFlag();
				
				sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
				sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
				PROCESS_ALARM(iResult);
				break;
			}			

			m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->SetPressComplete(m_InspectInfo.m_nWorkTabNo);
		}
		else
		{
			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				//다시 전체 초기화 필요!!
				sm_bForceUnitAll_Initialize = TRUE; //170616 JSH
				m_plnkTrsAutoManager->ResetUnitInitialFlag();
				
				sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
				sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
				PROCESS_ALARM(iResult);
				break;
			}
		}
		//170922 JSH.e
		
		TRACE(_T("<<m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->SetPressComplete(m_InspectInfo.m_nWorkTabNo);>> m_iWorkGroup[%d], m_iWorkInstance[%d]\n "), m_iWorkGroup, m_iWorkInstance);
		
		setStep(INSPECTIONCAMERA_INSPECTION_TAB_BOLB_READY);
		break;

	case INSPECTIONCAMERA_INSPECTION_TAB_BOLB_AFTER:		
		//170922 JSH.s
		sm_bTabBlobAfterReady[m_iWorkGroup][m_iWorkInstance] = TRUE;
		
		if(isGroupStarter() == TRUE)
		{	
			BOOL bAllReady = TRUE;
			for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
			{
				if (sm_bTabBlobAfterReady[m_iWorkGroup][i] == FALSE)
				{
					bAllReady = FALSE;
					break;
				}
			}
			if(bAllReady == FALSE)
				break;
			
			if (m_iScheduleNo[m_iWorkGroup] >= DEF_MAX_WORK_SCHEDULE)
			{
				//170719_KDH 작업할꺼 없으면 Skip Test중.
				m_iScheduleNo[m_iWorkGroup] = 0;	//@
				for (int i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
				{
					MListWorkTab ListWorkTab;
					m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
					if(ListWorkTab.GetTotalWorkTabCount() > 0)
						break;			
				}
				m_iScheduleNo[m_iWorkGroup] = i;
				//_______________________
			}
			
			for (int j=0; j<DEF_MAX_INSPECTION_CAM_CARRIER; j++)
			{
				sm_bTabBlobAfterStart[m_iWorkGroup][j] = TRUE;
			}					
		}
		if(sm_bTabBlobAfterStart[m_iWorkGroup][m_iWorkInstance] == FALSE)
			break;
		
		sm_bTabBlobAfterReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
		sm_bTabBlobAfterStart[m_iWorkGroup][m_iWorkInstance] = FALSE;

		setStep(INSPECTIONCAMERA_INSPECTION_TAB_BOLB_DONE);
		break;
		
	case INSPECTIONCAMERA_INSPECTION_TAB_BOLB_DONE:
		//170922 JSH.e
		if (FALSE == m_plnkCamCarrier->IsInPos(0, DEF_CAMERACARRIER_PANEL_MARK_POS))
		{
			if (FALSE == m_TimerError.IsTimerStarted())
				m_TimerError.StartTimer();
			
			iResult = m_plnkCamCarrier->CheckCollisionOther(0, DEF_CAMERACARRIER_PANEL_MARK_POS, -1);
			if (iResult)
			{
				if (m_TimerError.MoreThan(30.0))
				{
					m_TimerError.StopTimer();
					PROCESS_ALARM(generateErrorCode(414016));
					break;
				}
				break;
			}
			m_TimerError.StopTimer();

			iResult = m_plnkCamCarrier->SafeMoveBackupPos(DEF_CAMERACARRIER_BACKUP_DOWN_POS);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}

			iResult = m_plnkCamCarrier->SafeMovePos(0, DEF_CAMERACARRIER_PANEL_MARK_POS);
			if (iResult)
			{
				PROCESS_ALARM(iResult);
				break;
			}
		}

		TRACE(_T("<<case INSPECTIONCAMERA_INSPECTION_TAB_BOLB_DONE:>> m_iWorkGroup[%d], m_iWorkInstance[%d]\n "), m_iWorkGroup, m_iWorkInstance);
		if(isGroupStarter() == TRUE)
		{
			sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
			sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
			//170922 JSH.s
			for(int i = 0; i<DEF_MAX_WORK_SCHEDULE; i++)
			{
				m_plnkWorkSchedule[i]->ClearWorkSchedule();
				m_plnkWorkSchedule[i]->GenerateWorkSchedule();
				m_plnkWorkSchedule[i]->SetExistWorkPanel(TRUE);
			}
			//170922 JSH.e
		}
		setStep(INSPECTIONCAMERA_PANEL_IN_EQUIP_WAIT);
		break;
		//170612 JSH.e
	default:
		break;
	}
}

void MTrsInspectionCamera::initializeITI()
{
	m_rgbITIInspectionStartToTM = FALSE;
	m_rgbITIInspectionCompleteToTM = FALSE;
	m_rgbITIMountReadyToTM = FALSE;
	if (isGroupStarter())
		sm_bITIAllMountCompleteToPA[m_iWorkGroup] = FALSE;
		
	//170616 JSH.s
	if(m_plnkSystemData->m_bUseInspectionTabBolb == FALSE)
	{
		sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
		sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
	}
	//170616 JSH.e
}

void MTrsInspectionCamera::initializeStep()
{
	m_estepPrevious	= (enumStepInspectionCamera)0;	//  이전 Step Clear
	m_estepCurrent	= (enumStepInspectionCamera)0;	//  현재 Step Clear
}

void MTrsInspectionCamera::setStep(EStepInspectionCamera estepInspectionCamera)
{
	m_estepPrevious	= m_estepCurrent;			// 이전 Step 저장
	m_estepCurrent	= estepInspectionCamera;	// 새로 Step으로 전환
}

/***********************************************************************************************
/*
/* Component 공통 제공 Interface
/*
/************************************************************************************************/ 

void MTrsInspectionCamera::AssignComponents(STrsInspectionCameraRefCompList listRefComponents)
{
	m_plnkTrsAutoManager = listRefComponents.m_plnkTrsAutoManager;
	m_plnkCtrlInspectCam = listRefComponents.m_plnkCtrlInspectCam;
	m_plnkCamCarrier = listRefComponents.m_plnkCamCarrier;

	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_PER_GROUP; i++)
		m_rgplnkStateCamCarrier[i] = listRefComponents.m_rgplnkStateCamCarrier[i];

	for (int i=0; i<DEF_MAX_WORKER_PER_GROUP; i++)
		m_plnkTabMounter[i] = listRefComponents.m_plnkTabMounter[i];
}

void MTrsInspectionCamera::SetData(STrsInspectionCameraData datComponent)
{
	m_plnkSystemData = datComponent.m_plnkSystemData;
	m_plnkProcessData = datComponent.m_plnkProcessData;
	m_plnkPrebonderData = datComponent.m_plnkPrebonderData;
//@	m_plnkWorkSchedule = datComponent.m_plnkWorkSchedule;
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST] = datComponent.m_plnkWorkSchedule;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND] = datComponent.m_plnkWorkScheduleSecond;
#endif
}

int MTrsInspectionCamera::Initialize()
{
	m_bAutoInitialized = FALSE;
	if(m_iWorkGroup == DEF_REAR_GROUP)
	{
		m_bAutoInitialized  = TRUE;
		return ERR_TRS_CAMERACARRIER_SUCCESS;
	}

#ifndef SIMULATION
	int iResult = m_plnkCamCarrier->CheckOrigin();
	if (iResult)
	{
		SetErrorLevel(_T("INSPECTION CAMERA assign Components"),0, __FILE__, __LINE__);
		return iResult;
	}

	// 초기화 시 Inspection Camera 는 별도 이동 없음.
#endif /* SIMULATION */

	//170719_KDH 작업할꺼 없으면 Skip Test중.
	m_iScheduleNo[m_iWorkGroup] = 0;	//@
	int i;
	for(i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
	{
		MListWorkTab ListWorkTab;
		m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
		if(ListWorkTab.GetTotalWorkTabCount() > 0)
		{
			//170920 JSH.s
			if(i != DEF_MAX_WORK_SCHEDULE -1)
			{
				if(m_plnkWorkSchedule[i]->IsPressComplete_AllWorkList())
					continue;
			}
			//170920 JSH.e

			break;			
		}
	}
	//170920 JSH.s
	if(i == DEF_MAX_WORK_SCHEDULE)
		i = 0;
	//170920 JSH.e
	m_iScheduleNo[m_iWorkGroup] = i;
	//_______________________

	//===========================================
	m_InspectInfo.Init();
	sm_bGroupWorkComplete[m_iWorkGroup] = FALSE;
	sm_bAllWorkComplete[m_iWorkGroup] = FALSE;
	sm_bITIAllMountCompleteToPA[m_iWorkGroup] = FALSE;
	sm_bAlignPosMoveComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_bRecogRemoveTab[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_bRemoveTab = FALSE;
	m_bReceivedPressCompleteSchedule[m_iWorkGroup] = FALSE;
	m_TimerError.StopTimer();
	//===========================================
	
	//@130206.KKY___________
	m_iRetryCnt = 0;
	sm_bRequestUsePanelPusher[m_iWorkGroup] = FALSE;
	sm_bInspectionComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_bPusherDown[m_iWorkGroup][m_iWorkInstance] = FALSE;
	//@_____________________
	sm_MountAfterVisionLogComplete[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_MountAfterVisionLogEnd[m_iWorkGroup][m_iWorkInstance] = FALSE;


	//170612 JSH.s
	sm_bTabBlobReady[m_iWorkGroup][m_iWorkInstance] = FALSE;
	sm_bTabBlobStart[m_iWorkGroup][m_iWorkInstance] = FALSE;
	if(m_plnkSystemData->m_bUseInspectionTabBolb == FALSE)
	{
		sm_bInspectionTabBolb[m_iWorkGroup][m_iWorkInstance] = FALSE;
	}
	//170612 JSH.e

	m_bMovetoAlignPos[m_iWorkGroup] = TRUE;
	initializeStep();
	initializeITI();

	m_bAutoInitialized = TRUE;
	return ERR_TRS_CAMERACARRIER_SUCCESS; 
}

/***********************************************************************************************
/*
/* ITI (Inter-Thread Interface)
/*
/************************************************************************************************/

void MTrsInspectionCamera::AssignITI(SInspectionCameraRefITIList sITIList)
{
	m_plnkITI_PanelAligner = sITIList.m_plnkITI_PanelAligner;
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
		m_plnkITI_TabMounter[i] = sITIList.m_plnkITI_TabMounter[i];
}

/***********************************************************************************************
/*
/* Threading
/*
/************************************************************************************************/

void MTrsInspectionCamera::ThreadRun()
{
	if (m_bThreadLife)
	{
		ThreadStop();
		Sleep(100);
	}

	m_bThreadLife = TRUE;	
	
	m_pThread = AfxBeginThread(threadFunction, this);
}

void MTrsInspectionCamera::ThreadStop()
{
	m_bThreadLife = FALSE;

	if (m_pThread != NULL)
	{
		DWORD exitcode = ::WaitForSingleObject(m_pThread->m_hThread, 10000);

		if (exitcode == WAIT_TIMEOUT)
			::TerminateThread(m_pThread->m_hThread, exitcode);

		m_pThread = NULL;
	}
}
// NSMC
BOOL MTrsInspectionCamera::IsModelChangeReady()	
{
	return m_bModelChangeReady;
}

BOOL MTrsInspectionCamera::Is1FeederAllMountInitReady()
{
	return m_b1FeederAllMountInitReady;
}

BOOL MTrsInspectionCamera::is1FeederAllMounterInitStart()
{
	BOOL bInitStart = FALSE;
	/*/
#ifdef DEF_SOURCE_SYSTEM
	if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount)
	{
		if (TRUE == m_plnkTrsAutoManager->Get1FeederAllMounterInitStart() && TRUE == m_plnkTrsAutoManager->IsAll1FeederAllMount_MountInitReady()
			&& TRUE == m_plnkTrsAutoManager->IsAll1FeederAllMount_PanelAlignerInitReady())
		{
			bInitStart = TRUE;
		}
	}
#endif
	/*/
	return bInitStart;
}


BOOL MTrsInspectionCamera::isGroupStarter()
{
	if (m_iWorkInstance == 0)
		return TRUE;
	return FALSE;
}

BOOL MTrsInspectionCamera::IsReadyToWork()
{
	BOOL bReady = FALSE;
	if (isGroupStarter())
	{
		if (FALSE == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_CurrentWorkList())
		{
			if (SUCCESS == m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->PickOut_CurrentWorkList())
			{
				int iKey = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetKeyValue_CurrentWorkList();
//				TRACE(_T("<<<<<MTrsInspectionCamera:Pickout CurrentWorkList(Key = %d)>>>>>\n"), iKey);
//				Sleep(100);
			}
		}
	}

	if (m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->IsExist_CurrentWorkList())
	{
		m_InspectInfo = m_plnkWorkSchedule[m_iScheduleNo[m_iWorkGroup]]->GetInspectCarrierInfo_FromCurrentWorkList(m_iWorkInstance);
		bReady = TRUE;
	}
	return bReady;	
}

//120116.kms__________
BOOL MTrsInspectionCamera::IsMountAutoChangeReady()
{
	return m_bMountAutoChangeReady;
}

//@130206.KKY_____________
BOOL MTrsInspectionCamera::isAllInspectionCompleteGroup()
{
	BOOL bComplete = TRUE;
	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		bComplete &= sm_bInspectionComplete[m_iWorkGroup][i];
	return bComplete;
}

BOOL MTrsInspectionCamera::isAllPusherDownGroup()
{
	BOOL bComplete = TRUE;
	for (int i=0; i<DEF_MAX_INSPECTION_CAM_CARRIER; i++)
		bComplete &= sm_bPusherDown[m_iWorkGroup][i];
	return bComplete;
}
//@_______________________
//170612 JSh.s
void MTrsInspectionCamera::SetInspectionTabBolb()
{
	if(m_plnkSystemData->m_bUseInspectionTabBolb == FALSE)
		return;
	int i = 0;

	for (int i = 0; i<DEF_MAX_GROUP; i++)
	{
		for (int j=0; j<DEF_MAX_INSPECTION_CAM_CARRIER; j++)
		{
			sm_bInspectionTabBolb[i][j] = TRUE;
		}

		sm_bTabBlobWorkingToPA[i] = TRUE;
		sm_bTabBlobWorkingToTM[i] = TRUE;
	}

	//170705 JSH.s
	
	
	//170719_KDH 작업할꺼 없으면 Skip Test중.
	m_iScheduleNo[m_iWorkGroup] = 0;	//@
	for (int i = m_iScheduleNo[m_iWorkGroup]; i < DEF_MAX_WORK_SCHEDULE; i++)
	{
		MListWorkTab ListWorkTab;
		m_plnkWorkSchedule[i]->GetOrgWorkList(&ListWorkTab);
		if(ListWorkTab.GetTotalWorkTabCount() > 0)
		{
			//170920 JSH.s
			if(i != DEF_MAX_WORK_SCHEDULE -1)
			{
				if(m_plnkWorkSchedule[i]->IsPressComplete_AllWorkList())
					continue;
			}
			//170920 JSH.e
			
			break;			
		}
	}
	//170920 JSH.s
	if(i == DEF_MAX_WORK_SCHEDULE)
		i = 0;
	//170920 JSH.e
	m_iScheduleNo[m_iWorkGroup] = i;
	//_______________________
	//170705 JSH.e

}
//170612 JSH.e

//170616 JSH.s
void MTrsInspectionCamera::ResetInspectionTabBolb()
{
	for(int i = 0; i<DEF_MAX_GROUP; i++)
	{
		for (int j=0; j<DEF_MAX_INSPECTION_CAM_CARRIER; j++)
		{
			sm_bInspectionTabBolb[i][j] = FALSE;
		}
	}

	sm_bTabBlobWorkingToPA[m_iWorkGroup] = FALSE;
	sm_bTabBlobWorkingToTM[m_iWorkGroup] = FALSE;
}
//170616 JSH.e
