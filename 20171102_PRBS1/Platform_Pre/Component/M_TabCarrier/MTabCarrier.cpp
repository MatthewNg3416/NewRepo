/* 
 * TabCarrier Component
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
 * TabCarrier.cpp : Implementation of TabCarrier component.
 */

#include "stdafx.h"
#include "MSystemData.h"
#include "MPlatformOlbSystem.h"
#include "IAxis.h"
#include "IVacuum.h"
#include "ICylinder.h"
#include "MTabCarrier.h"
#include "MTeachingPosData.h"
#include <math.h>

#include "MPreBonderData.h"
#include "IStateTabMounter.h"
#include "DefTabMounter.h"
#include "IStateTabFeeder.h"
#include "MTrsAutoManager.h"
#include "ISercosMultiAxis.h"

#include "common.h"

extern MPlatformOlbSystem	MOlbSystemPre;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCriticalSection MTabCarrier::m_csMoveLock;

/** 
 * 생성자
 */
MTabCarrier::MTabCarrier()
{
}

/** 
 * 생성자
 *
 * @param commonData : ObjectI나 Log와 같은 Component Common Data
 * @param listRefComponent : 참조할 Component 포인터 리스트
 * @param datComponent : TabCarrier Data
 */
MTabCarrier::MTabCarrier(SCommonAttribute commonData, STabCarrierRefCompList listRefComponents, STabCarrierData datComponent)
: ICommon(commonData)
{
	int iResult = ERR_TABCARRIER_SUCCESS;

	// TABCARRIER Y축 좌표 초기화
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		m_rgsXYTFixedPos[i].Init(0.0, 0.0, 0.0);
		m_rgsXYTOffsetPos[i].Init(0.0, 0.0, 0.0);
		m_rgsXYTModelPos[i].Init(0.0, 0.0, 0.0);
	}

	for (int i = 0; i < DEF_TABCARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgsExpandFixedPos[i]	= 0.0;
		m_rgsExpandOffsetPos[i]	= 0.0;
		m_rgsExpandModelPos[i]	= 0.0;
	}

	//SJ_YYK 150109 Add...
	for (int i=0; i< DEF_TABCARRIER_Z_MAX_POS; i++)
	{
		m_rgsZFixedPos[i] = 0.0;
		m_rgsZOffsetPos[i] = 0.0;
		m_rgsZModelPos[i] = 0.0;
	}

	// Component List Assign
	iResult	= AssignComponents(listRefComponents);
	//if (iResult)
		//WriteErrorLog("MTabCarrier() : AssignComponent Error!");
		//SetErrorLevel(_T("TAB CARRIER assign Component List"),2, __FILE__, __LINE__);

	// Data Assign
	iResult = SetData(datComponent);

	if (m_iInstanceNo < DEF_MAX_TABCARRIER)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		//m_iWorkInstance = DEF_MAX_TABCARRIER*2 - 1 - m_iInstanceNo;
		m_iWorkInstance = m_iInstanceNo-DEF_MAX_TABCARRIER;
	}

	//if (iResult)
		//WriteErrorLog("MTabCarrier() : SetData Error!");
		//SetErrorLevel(_T("TAB CARRIER assign Component List"),2, __FILE__, __LINE__);

	// Data Initial
	m_bJogStats = FALSE;

	m_bMoving = FALSE;//KKY

	// Model Path
	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	// Align 변수 초기화
	m_bMarkAligned = FALSE;
	m_dValueThetaAlign = 0.0;
	m_rgAlignMarkOffset.Init(0.0, 0.0, 0.0);
	
	LoadXYTFixedPosParameter();			// 고정 좌표 Load
	LoadXYTOffsetPosParameter();		// Offset 좌표 Load
	calculateXYTModelPosParameter();	// 


	LoadExpandFixedPosParameter();
	LoadExpandOffsetPosParameter();
	calculateExpandPosParameter();

	//SJ_YYK 150109 Add...
	LoadZFixedPosParameter();
	LoadZOffsetPosParameter();
}

/**
 * 소멸자
 */
MTabCarrier::~MTabCarrier()
{
}

int MTabCarrier::checkInterlockForXYT(MPos_XYT posTarget, int iPosID, BOOL bAuto)
{
#ifdef SIMULATION
	return ERR_TABCARRIER_SUCCESS;
#endif	

	int iResult = ERR_TABCARRIER_SUCCESS;
	MPos_XYT xytTolerance;
	xytTolerance.dX = xytTolerance.dY = 0.001;
	xytTolerance.dT = 0.001;
	if (IsInXYTPos(posTarget, &xytTolerance))
		return ERR_TABCARRIER_SUCCESS;


	//실린더 Down상태 확인
	double dLoadPosX = GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dX;
	double dCurPosX = GetCurrentXYTPos().dX;
	if (iPosID == DEF_TABCARRIER_LOAD_POS
		|| (dCurPosX < dLoadPosX-1.0 && posTarget.dX > dLoadPosX+10.0)
		|| (dCurPosX > dLoadPosX+10.0 && posTarget.dX < dLoadPosX-1.0))
	{
		if (FALSE == IsDown())
		{
			DWORD dTemp = GetTickCount();
			while(1)
			{
				Sleep(15);
				if(IsDown()) break;
//				if(GetTickCount() - dTemp > 10 * 1000)
				if(GetTickCount() - dTemp > 5 * 1000)
					return generateErrorCode(217007);	// UD Cylinder가 Down 안되어 있어 Carrier를 이동할 수 없습니다.		
			}
		}

		//170117 SJ_YSH
		if (FALSE == IsDown2())
		{
			DWORD dTemp = GetTickCount();
			while(1)
			{
				Sleep(15);
				if(IsDown()) break;
//				if(GetTickCount() - dTemp > 10 * 1000)
				if(GetTickCount() - dTemp > 5 * 1000)
					return generateErrorCode(217007);	// UD Cylinder가 Down 안되어 있어 Carrier를 이동할 수 없습니다.		
			}
		}
		//____________
	}

	//110208.Add_____
	MPos_XYT xytchkTolerance(5.0, 10.0, 10.0);
	if (IsInXYTPos(DEF_TABCARRIER_LOAD_POS, &xytchkTolerance) && !IsDown())
	{
		iResult = Down();
		if (iResult)
		{
			return iResult;
		}
	}
	//_______________

	//5pitch까지는 Carrier U/D실린더 전진한 상태로 움직이고,
	//5pitch초과는 Carrier U/D실린더 후진한 상태로 움직인다.
	if (FALSE == IsShortPitchTab() && FALSE == IsBackward())
	{
		iResult = Backward();
		if (iResult)
			return iResult;
	}
	else if (TRUE == IsShortPitchTab() && FALSE == IsForward())
	{
		iResult = Forward();
		if (iResult)
			return iResult;
	}

	BOOL bToolDangerPos = FALSE;
	BOOL bToolCheck = FALSE;

	/*/
#ifdef DEF_SOURCE_SYSTEM
	//목표위치가 Unloading위치이거나, 현재위치가 Prealign위치보다 클경우 TabMounter Up위치 확인
	if (iPosID == DEF_TABCARRIER_UNLOAD_POS || CompareCurrentPos(DEF_TABCARRIER_Y_AXIS, DEF_TABCARRIER_LOAD_POS, MORE_THAN))  ///DEF_TABCARRIER_PREALIGN_POS
	{
		for (int i = 0 ; i < DEF_MAX_WORKER_PER_GROUP ; i++)
		{
			if (m_iWorkGroup == DEF_FRONT_GROUP)
				bToolDangerPos = m_plnkStateTabMounter[i]->IsIn_FrontTabLoadPos_TabMounter(i);
			else
				bToolDangerPos = m_plnkStateTabMounter[i]->IsIn_RearTabLoadPos_TabMounter(i);

			if (m_plnkStateTabMounter[i]->CheckOrigin() != 0 || bToolDangerPos)
			{
/ *				if (m_plnkStateTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS) == FALSE)
				{
					SetErrorLevel(_T("TAB CARRIER check Interlock for XY"),2, __FILE__, __LINE__);
					// 217013 = 이동 불가. [TabMounter 가 간섭 가능 위치에서 Up 아님.]
					return generateErrorCode(217013);
				}* /
			}
		}
	}
#else
	/*/

	if (iPosID == DEF_TABCARRIER_UNLOAD_POS || CompareCurrentPos(DEF_TABCARRIER_X_AXIS, DEF_TABCARRIER_PREALIGN_POS, LESS_THAN))
	{
		double dTolerance = 100.0;
		if (TRUE == m_plnkStateTabMounter[0]->IsInGantryPos(DEF_TABMOUNTER_GANTRY_LOAD_POS, &dTolerance))
		{
			dTolerance = 2.0;
			if (FALSE == m_plnkStateTabMounter[m_iWorkInstance]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS, &dTolerance))
				// 217013 = 이동 불가. [TabMounter 가 간섭 가능 위치에서 Up 아님.]
				return generateErrorCode(217013);
		}
	}


	iResult = CheckCollisionOther(posTarget);
	if (iResult)
	{
		DWORD sTime = GetTickCount();
		while(1)
		{
			Sleep(15);
			iResult = CheckCollisionOther(posTarget);
			if (iResult == 0)
				break;
			if (GetTickCount() - sTime > 3000)
				break;
		}
		if (iResult)
			return iResult;
	}

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::checkInterlockTabMounter4MoveUnload(int iPosID)
{
	/*/
#ifndef DEF_SOURCE_SYSTEM
	ASSERT(0);
	return ERR_TABCARRIER_SUCCESS;
#endif
	/*/

#ifdef SIMULATION
	return ERR_TABCARRIER_SUCCESS;
#endif

//	if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == MANUAL_MODE)
//	{
//
//	}
	int iResult = ERR_TABCARRIER_SUCCESS;
	int i = 0;
	double dToolLoadPos = 0.0;
	int iToolNo = 0;
	//Unloading위치로 이동할 경우
///	if (iPosID == DEF_TABCARRIER_UNLOAD_POS)
	{
		if (m_iWorkGroup == DEF_FRONT_GROUP)
		{
			iToolNo = MOlbSystemPre.Get_CarrierToTabMounter(DEF_FRONT_GROUP, m_iWorkInstance);
			dToolLoadPos = m_plnkStateTabMounter[iToolNo]->GetTargetPos(0, DEF_TABMOUNTER_LOAD_POS) + DEF_TABMOUNTER_TOOL_SIZE;
			for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
			{
				if (m_plnkStateTabMounter[i]->GetCurrentPos() <= dToolLoadPos)
				{
					if (FALSE == m_plnkStateTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
						return generateErrorCode(217030);
				}
			}

			if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount)
			{
				iToolNo = MOlbSystemPre.Get_CarrierToTabMounterOtherGroup(DEF_FRONT_GROUP, m_iWorkInstance);
				dToolLoadPos = m_plnkStateTabMounter[iToolNo]->GetTargetPos(0, DEF_TABMOUNTER_R_LOAD_POS) + DEF_TABMOUNTER_TOOL_SIZE;
				for (int i=DEF_MOUNTER_WORK_PER_GROUP; i<DEF_MAX_TABMOUNTER; i++)
				{
					if (m_plnkStateTabMounter[i]->GetCurrentPos() <= dToolLoadPos)
					{
						if (FALSE == m_plnkStateTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
							return generateErrorCode(217030);
					}
				}
			}
		}
		else
		{
			iToolNo = MOlbSystemPre.Get_CarrierToTabMounter(DEF_REAR_GROUP, m_iWorkInstance);
			dToolLoadPos = m_plnkStateTabMounter[iToolNo]->GetTargetPos(0, DEF_TABMOUNTER_LOAD_POS) - DEF_TABMOUNTER_TOOL_SIZE;
			for (int i=DEF_MOUNTER_WORK_PER_GROUP; i<DEF_MAX_TABMOUNTER; i++)
			{
				if (m_plnkStateTabMounter[i]->GetCurrentPos() >= dToolLoadPos)
				{
					if (FALSE == m_plnkStateTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
						return generateErrorCode(217030);
				}
			}

			if (TRUE == m_plnkSystemData->m_bUse1FeederAllMount)
			{
				iToolNo = MOlbSystemPre.Get_CarrierToTabMounterOtherGroup(DEF_REAR_GROUP, m_iWorkInstance);
				dToolLoadPos = m_plnkStateTabMounter[iToolNo]->GetTargetPos(0, DEF_TABMOUNTER_R_LOAD_POS) - DEF_TABMOUNTER_TOOL_SIZE;
				for (int i=0; i<DEF_MOUNTER_WORK_PER_GROUP; i++)
				{
					if (m_plnkStateTabMounter[i]->GetCurrentPos() >= dToolLoadPos)
					{
						if (FALSE == m_plnkStateTabMounter[i]->IsInPosZ(DEF_TABMOUNTER_Z_MOVE_POS))
							return generateErrorCode(217030);
					}
				}				
			}
		}
	}
///	//Unloading위치에서 다른 위치로 이동할 경우
///	else
///	{
///	}

	return iResult;
}

int MTabCarrier::checkInterlockForUp()
{
	int iResult = CheckOrigin();
	if (iResult)
		return iResult;
	
	//SJ_YYK 150817 Modify...
	//SJ_YYK 150211 Modify..
	//double dLoadStartPos = GetTargetXYTPos(DEF_TABCARRIER_LOAD_START_POS).dX;
	if (FALSE == IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
	//if(GetCurrentXYTPos().dX > dLoadStartPos + 1.0)
	{
		if (FALSE == IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
		{
			BOOL isInLoadPos = FALSE;
			for (int i=0; i<5; i++)
			{
				Sleep(10);
				isInLoadPos = IsInXYTPos(DEF_TABCARRIER_LOAD_POS);
				if (isInLoadPos)
					break;
			}
			if (FALSE == isInLoadPos)
				// 217008 = Up 불가. [TabCarrier Load 위치 아님.]
				return generateErrorCode(217008);
		}
	}
	if (TRUE == IsAbsorbTabIC())
		// 217016 = TabIC 를 흡착한 채, 상승하려 함.
		return generateErrorCode(217016);

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::checkInterlockForDumpBasketFor()
{
	int iResult = ERR_TABCARRIER_SUCCESS;

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::calculateXYTModelPosParameter()
{
	//m_dLength_between_ChipCenter_and_TabEnd ---> TabIC길이값임.
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		m_rgsXYTModelPos[i].Init(0.0, 0.0, 0.0);
		if (i == DEF_TABCARRIER_LOAD_POS || i == DEF_TABCARRIER_READY_POS)
		{
			m_rgsXYTModelPos[i].dY = 0.0;
			//SJ_PJC 20170117 Add..
			double dTabWidth = 0.0;
#		ifdef DEF_SOURCE_SYSTEM
			dTabWidth = m_plnkPreBonderData->m_dLead_Width_Xtab;
#		else
			dTabWidth = m_plnkPreBonderData->m_dLead_Width_Ytab;
#		endif
			if(m_iWorkInstance == DEF_TABCARRIER1
			|| m_iWorkInstance == DEF_TABCARRIER3)
			{
				m_rgsXYTModelPos[i].dX = dTabWidth/2.0;
			}
			else
			{
				m_rgsXYTModelPos[i].dX = -dTabWidth/2.0;
			}

			if((m_iWorkInstance == DEF_TABCARRIER3 || m_iWorkInstance == DEF_TABCARRIER4) && i == DEF_TABCARRIER_READY_POS)
			{
				m_rgsXYTModelPos[i].dX = 0.0;
			}
			//_____________________

		}
		else if (i == DEF_TABCARRIER_PREALIGN_POS)
		{
			double dLeadWidth = 0.0;
#ifdef DEF_SOURCE_SYSTEM
			dLeadWidth = m_plnkPreBonderData->m_dLead_Width_Xtab;
#else
			dLeadWidth = m_plnkPreBonderData->m_dLead_Width_Ytab;
#endif
		}
		else if (i == DEF_TABCARRIER_UNLOAD_POS)
		{
		}
	}

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::calculateExpandPosParameter()
{
	for (int i = 0; i < DEF_TABCARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgsExpandModelPos[i] = 0.0;

		if(m_iWorkInstance %2 == 0)
			m_rgsExpandFixedPos[i] = m_plnkSystemData->m_dData_CameraExpand[m_iWorkGroup][DEF_CAM_TABCARRIER];
		else 
			m_rgsExpandFixedPos[i] = m_plnkSystemData->m_dData_CameraExpand[m_iWorkGroup][DEF_CAM_TABCARRIER2];

		if (i == DEF_TABCARRIER_EXPAND_WORKING_POS)
		{
#	ifdef DEF_SOURCE_SYSTEM
			if(m_iWorkInstance %2 == 0)
				m_rgsExpandModelPos[i] = m_plnkPreBonderData->m_dLead_Width_Xtab - m_plnkSystemData->m_dData_CameraExpand[m_iWorkGroup][DEF_CAM_TABCARRIER];
			else
				m_rgsExpandModelPos[i] = m_plnkPreBonderData->m_dLead_Width_Xtab - m_plnkSystemData->m_dData_CameraExpand[m_iWorkGroup][DEF_CAM_TABCARRIER2];
#	else
			if(m_iWorkInstance %2 == 0)
				m_rgsExpandModelPos[i] = m_plnkPreBonderData->m_dLead_Width_Ytab - m_plnkSystemData->m_dData_CameraExpand[m_iWorkGroup][DEF_CAM_TABCARRIER];
			else
				m_rgsExpandModelPos[i] = m_plnkPreBonderData->m_dLead_Width_Ytab - m_plnkSystemData->m_dData_CameraExpand[m_iWorkGroup][DEF_CAM_TABCARRIER2];
#	endif
		}
	}

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::SetData(STabCarrierData datcomponent)
{
	m_plnkSystemData		= datcomponent.m_plnkSystemData;
	m_plnkPreBonderData		= datcomponent.m_plnkPreBonderData;
	m_plnkTabOffsetData		= datcomponent.m_plnkTabOffsetData;
	m_sXYTTeachingInfo		= datcomponent.m_sXYTTeachingInfo;
	m_sExpandTeachingInfo	= datcomponent.m_sExpandTeachingInfo;
	m_sZTeachingInfo		= datcomponent.m_sZTeachingInfo; //SJ_YYK 150109 Add...
	m_strPosFileName		= datcomponent.m_strPosFileName;
	m_strOffsetPosFileName	= datcomponent.m_strOffsetPosFileName;
	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	m_iCamNo1				= datcomponent.m_iCamNo1;
	m_iCamNo2				= datcomponent.m_iCamNo2;

	m_iPreTabCarrierNo		= datcomponent.m_iPreTabCarrierNo;
	m_iNextTabCarrierNo		= datcomponent.m_iNextTabCarrierNo;

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::AssignComponents(STabCarrierRefCompList TabCarrierCompList)
{
	// TabCarrier X Motion Object
	m_plnkX = TabCarrierCompList.m_plnkXAxis;

	// TabCarrier Y Motion Object
	m_plnkY = TabCarrierCompList.m_plnkYAxis;

	// TabCarrier T Motion Object
	m_plnkT = TabCarrierCompList.m_plnkTAxis;

	m_plnkExpandAxis = TabCarrierCompList.m_plnkExpandAxis;

	m_plnkZAxis = TabCarrierCompList.m_plnkZAxis; //SJ_YYK 150109 Add...

	m_plnkXZAxis = TabCarrierCompList.m_plnkXZAxis; //SJ_YYK 150212 Add...

	// TabCarrier Up/Down Cylinder Object
	m_plnkUDCyl = TabCarrierCompList.m_plnkUDCyl;

	m_plnkUDCyl2 = TabCarrierCompList.m_plnkUDCyl2; //SJ_YYK 150817 Add...

	m_plnkFBCyl = TabCarrierCompList.m_plnkFBCyl;

	m_plnkCamESCyl = TabCarrierCompList.m_plnkCamESCyl;

	m_plnkDumpBasketFBCyl = NULL;

	m_plnkVac = TabCarrierCompList.m_plnkVac;

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::AssignStateCheckComponents(SStateCheckCompListForTabCarrier sCompList)
{
	for (int i = 0; i < DEF_MAX_WORKER_PER_GROUP; i++)
	{
		m_plnkStateTabMounter[i] = sCompList.m_plnkStateTabMounter[i];
	}
	for (int i = 0 ; i < DEF_MAX_TABCARRIER ; i++)
	{
		m_plnkStateTabCarrier[i] = sCompList.m_plnkStateTabCarrier[i];
	}
	m_plnkStateTabFeeder = sCompList.m_plnkStateTabFeeder;
	return ERR_TABCARRIER_SUCCESS;
}

IAxis* MTabCarrier::GetMotionComponent(int iAxisID)
{
	// Motion Component 축 ID에 따라
	switch (iAxisID)
	{
	case DEF_TABCARRIER_X_AXIS :	// TABCARRIER X축 
		return m_plnkX;

	case DEF_TABCARRIER_Y_AXIS :	// TABCARRIER Y축
		return m_plnkY;

	case DEF_TABCARRIER_T_AXIS :	// TABCARRIER T축
		return m_plnkT;

		// Doolin kwangilshin 2017. 08. 25.
		//
	case DEF_TABCARRIER_EXPAND_AXIS :	// TABCARRIER T축
		return m_plnkExpandAxis;
		// End.

	default :						// 축 ID 오류
		ASSERT(0);	//KKY
		return NULL;
	}
}

IAxis* MTabCarrier::GetExpandMotionComponent()
{
	return m_plnkExpandAxis;
}

//SJ_YYK 150109 Add..
IAxis* MTabCarrier::GetZMotionComponent()
{
	return m_plnkZAxis;
}

int MTabCarrier::ChangeModel(void)
{
	// Model Path Change
	m_strOffsetPosFilePath.Format(_T("%s\\%s"),
									m_plnkSystemData->m_strModelFileDir,
									m_plnkSystemData->m_strModelFileName);

	// Y축 Offset 좌표 Load
	int iResult = LoadXYTOffsetPosParameter();
	if (iResult)
		return iResult;

	calculateXYTModelPosParameter();

	calculateExpandPosParameter();

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::LoadXYTFixedPosParameter(void)
{
	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult;

	MTeachingPosData TabCarrierXYTFixPosDat(m_sXYTTeachingInfo, m_strPosFileName);

	bResult = TabCarrierXYTFixPosDat.ReadData();
	
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		m_rgsXYTFixedPos[i].dX = TabCarrierXYTFixPosDat.m_TeachingPos[i][DEF_TABCARRIER_X_AXIS];
		m_rgsXYTFixedPos[i].dY = TabCarrierXYTFixPosDat.m_TeachingPos[i][DEF_TABCARRIER_Y_AXIS];
		m_rgsXYTFixedPos[i].dT = TabCarrierXYTFixPosDat.m_TeachingPos[i][DEF_TABCARRIER_T_AXIS];
	}

	if (!bResult)
		return generateErrorCode(217001);	// Tab Carrier 축 고정좌표를 Load 하는데 실패 했습니다. 

	return iResult;
}

int MTabCarrier::LoadExpandFixedPosParameter(void)
{
	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult = TRUE;

	MTeachingPosData ExpandFixPosDat(m_sExpandTeachingInfo, m_strPosFileName);
//@	bResult = ExpandFixPosDat.ReadData();	
	for (int i = 0; i < DEF_TABCARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgsExpandFixedPos[i] = m_plnkSystemData->m_dData_CameraExpand[m_iWorkGroup][DEF_CAM_TABCARRIER];  //@ExpandFixPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		return generateErrorCode(217001);	// Tab Carrier 축 고정좌표를 Load 하는데 실패 했습니다. 
	return iResult;
}

//SJ_YYK 150109 Add..
int MTabCarrier::LoadZFixedPosParameter(void)
{
	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult = TRUE;

	MTeachingPosData ZFixPosDat(m_sZTeachingInfo, m_strPosFileName);

	bResult = ZFixPosDat.ReadData();
	
	for (int i = 0; i < DEF_TABCARRIER_Z_MAX_POS; i++)
	{
		m_rgsZFixedPos[i] = ZFixPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		return generateErrorCode(217001);	// Tab Carrier 축 고정좌표를 Load 하는데 실패 했습니다. 
	return iResult;
}


int MTabCarrier::LoadXYTOffsetPosParameter(void)
{
	MTeachingPosData TabCarrierXYTOffsetPosDat(m_sXYTTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	// Offset Pos
	BOOL bResult = TabCarrierXYTOffsetPosDat.ReadData();	
	
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		m_rgsXYTOffsetPos[i].dX = TabCarrierXYTOffsetPosDat.m_TeachingPos[i][DEF_TABCARRIER_X_AXIS];
		m_rgsXYTOffsetPos[i].dY = TabCarrierXYTOffsetPosDat.m_TeachingPos[i][DEF_TABCARRIER_Y_AXIS];
		m_rgsXYTOffsetPos[i].dT = TabCarrierXYTOffsetPosDat.m_TeachingPos[i][DEF_TABCARRIER_T_AXIS];
	}

	if (!bResult)
		return generateErrorCode(217002);	// Tab Carrier 축 Offset 좌표를 Load 하는데 실패 했습니다. 

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::LoadExpandOffsetPosParameter(void)
{
	MTeachingPosData ExpandOffsetPosDat(m_sExpandTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	// Offset Pos
	BOOL bResult = ExpandOffsetPosDat.ReadData();	
	
	for (int i = 0; i < DEF_TABCARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgsExpandOffsetPos[i] = ExpandOffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		return generateErrorCode(217002);	// Tab Carrier 축 Offset 좌표를 Load 하는데 실패 했습니다. 

	return ERR_TABCARRIER_SUCCESS;
}

//SJ_YYK 150109 Add..
int MTabCarrier::LoadZOffsetPosParameter(void)
{
	MTeachingPosData ZOffsetPosDat(m_sZTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	// Offset Pos
	BOOL bResult = ZOffsetPosDat.ReadData();	
	
	for (int i = 0; i < DEF_TABCARRIER_Z_MAX_POS; i++)
	{
		m_rgsZOffsetPos[i] = ZOffsetPosDat.m_TeachingPos[i][0];
	}

	if (!bResult)
		return generateErrorCode(217002);	// Tab Carrier 축 Offset 좌표를 Load 하는데 실패 했습니다. 

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::SaveXYTFixedPosParameter(MPos_XYT* rgsFixedPos)
{
	ASSERT(rgsFixedPos != NULL);

	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult;
	
	MTeachingPosData TabCarrierXYTFixPosDat(m_sXYTTeachingInfo, m_strPosFileName);
	
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		m_rgsXYTFixedPos[i].dX = rgsFixedPos[i].dX;
		m_rgsXYTFixedPos[i].dY = rgsFixedPos[i].dY;
		m_rgsXYTFixedPos[i].dT = rgsFixedPos[i].dT;

		TabCarrierXYTFixPosDat.m_TeachingPos[i][DEF_TABCARRIER_X_AXIS] = m_rgsXYTFixedPos[i].dX;
		TabCarrierXYTFixPosDat.m_TeachingPos[i][DEF_TABCARRIER_Y_AXIS] = m_rgsXYTFixedPos[i].dY;		
		TabCarrierXYTFixPosDat.m_TeachingPos[i][DEF_TABCARRIER_T_AXIS] = m_rgsXYTFixedPos[i].dT;
		TabCarrierXYTFixPosDat.m_TeachingPos[i][3] = 0.0;
	}

	bResult = TabCarrierXYTFixPosDat.WriteData();
	
	if (!bResult)
		return generateErrorCode(217003);	// Tab Carrier 축 고정 좌표를 저장하는데 실패 했습니다. 

	return iResult;
}

int MTabCarrier::SaveExpandFixedPosParameter(double* rgsFixedPos)
{
	ASSERT(rgsFixedPos != NULL);

	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult;
	
	MTeachingPosData ExpandFixPosDat(m_sExpandTeachingInfo, m_strPosFileName);
	
	for (int i = 0; i < DEF_TABCARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgsExpandFixedPos[i] = m_plnkSystemData->m_dData_CameraExpand[m_iWorkGroup][DEF_CAM_TABCARRIER]; //@rgsFixedPos[i];
		ExpandFixPosDat.m_TeachingPos[i][0] = m_rgsExpandFixedPos[i];	
	}

	bResult = ExpandFixPosDat.WriteData();
	
	if (!bResult)
		return generateErrorCode(217003);	// Tab Carrier 축 고정 좌표를 저장하는데 실패 했습니다. 

	return iResult;
}

//SJ_YYK 150109 Add...
int MTabCarrier::SaveZFixedPosParamter(double* rgsFixedPos)
{
	ASSERT(rgsFixedPos != NULL);

	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult;
	
	MTeachingPosData TabCarrierZFixPosDat(m_sZTeachingInfo, m_strPosFileName);
	
	for (int i = 0; i < DEF_TABCARRIER_Z_MAX_POS; i++)
	{
		m_rgsZFixedPos[i] = rgsFixedPos[i];
		TabCarrierZFixPosDat.m_TeachingPos[i][0] = m_rgsZFixedPos[i];
	}

	bResult = TabCarrierZFixPosDat.WriteData();
	
	if (!bResult)
		return generateErrorCode(217003);	// Tab Carrier 축 고정 좌표를 저장하는데 실패 했습니다. 

	return iResult;
}

int MTabCarrier::SaveXYTOffsetPosParameter(MPos_XYT* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult;

	MTeachingPosData TabCarrierXYTOffsetPosDat(m_sXYTTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	// Offset Pos 
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		m_rgsXYTOffsetPos[i].dX = rgsOffsetPos[i].dX;
		m_rgsXYTOffsetPos[i].dY = rgsOffsetPos[i].dY;
		m_rgsXYTOffsetPos[i].dT = rgsOffsetPos[i].dT;

		TabCarrierXYTOffsetPosDat.m_TeachingPos[i][DEF_TABCARRIER_X_AXIS] = m_rgsXYTOffsetPos[i].dX;
		TabCarrierXYTOffsetPosDat.m_TeachingPos[i][DEF_TABCARRIER_Y_AXIS] = m_rgsXYTOffsetPos[i].dY;
		TabCarrierXYTOffsetPosDat.m_TeachingPos[i][DEF_TABCARRIER_T_AXIS] = m_rgsXYTOffsetPos[i].dT;
	}

	bResult = TabCarrierXYTOffsetPosDat.WriteData();

	if (!bResult)
		return generateErrorCode(217004);	// Tab Carrier 축 Offset 좌표를 저장하는데 실패 했습니다. 
	
	return iResult;
}

int MTabCarrier::SaveExpandOffsetPosParameter(double* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult;

	MTeachingPosData ExpandOffsetPosDat(m_sExpandTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	// Offset Pos 
	for (int i = 0; i < DEF_TABCARRIER_EXPAND_MAX_POS; i++)
	{
		m_rgsExpandOffsetPos[i] = rgsOffsetPos[i];
		ExpandOffsetPosDat.m_TeachingPos[i][0] = m_rgsExpandOffsetPos[i];
	}

	bResult = ExpandOffsetPosDat.WriteData();

	if (!bResult)
		return generateErrorCode(217004);	// Tab Carrier 축 Offset 좌표를 저장하는데 실패 했습니다. 
	
	return iResult;
}

//SJ_YYK 150109 Add..
int MTabCarrier::SaveZOffsetPosParameter(double* rgsOffsetPos)
{
	ASSERT(rgsOffsetPos != NULL);

	int iResult = ERR_TABCARRIER_SUCCESS;
	BOOL bResult;

	MTeachingPosData TabCarrierZOffsetPosDat(m_sZTeachingInfo, m_strOffsetPosFileName, m_strOffsetPosFilePath);

	// Offset Pos 
	for (int i = 0; i < DEF_TABCARRIER_Z_MAX_POS; i++)
	{
		m_rgsZOffsetPos[i] = rgsOffsetPos[i];
		TabCarrierZOffsetPosDat.m_TeachingPos[i][0] = m_rgsZOffsetPos[i];
	}

	bResult = TabCarrierZOffsetPosDat.WriteData();

	if (!bResult)
		return generateErrorCode(217004);	// Tab Carrier 축 Offset 좌표를 저장하는데 실패 했습니다. 
	
	return iResult;
}

int MTabCarrier::GetXYTPosParameter(MPos_XYT* rgsFixedPos, MPos_XYT* rgsOffsetPos, MPos_XYT* rgsModelPos)
{
	for (int i = 0; i < DEF_TABCARRIER_MAX_POSITION; i++)
	{
		if (rgsFixedPos != NULL)
		{
			rgsFixedPos[i].dX = m_rgsXYTFixedPos[i].dX;
			rgsFixedPos[i].dY = m_rgsXYTFixedPos[i].dY;
			rgsFixedPos[i].dT = m_rgsXYTFixedPos[i].dT;
		}

		if (rgsOffsetPos != NULL)
		{
			rgsOffsetPos[i].dX = m_rgsXYTOffsetPos[i].dX;
			rgsOffsetPos[i].dY = m_rgsXYTOffsetPos[i].dY;
			rgsOffsetPos[i].dT = m_rgsXYTOffsetPos[i].dT;
		}

		if (rgsModelPos != NULL)
		{
			rgsModelPos[i].dX = m_rgsXYTModelPos[i].dX;
			rgsModelPos[i].dY = m_rgsXYTModelPos[i].dY;
			rgsModelPos[i].dT = m_rgsXYTModelPos[i].dT;
		}
	}

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::GetExpandPosParameter(double* rgsFixedPos, double* rgsOffsetPos, double* rgsModelPos)
{
	for (int i = 0; i < DEF_TABCARRIER_EXPAND_MAX_POS; i++)
	{
		if (rgsFixedPos != NULL)
		{
			rgsFixedPos[i] = m_rgsExpandFixedPos[i];
		}

		if (rgsOffsetPos != NULL)
		{
			rgsOffsetPos[i] = m_rgsExpandOffsetPos[i];
		}

		if (rgsModelPos != NULL)
		{
			rgsModelPos[i] = m_rgsExpandModelPos[i];
		}
	}
	return ERR_TABCARRIER_SUCCESS;
}

//SJ_YYK 150109 Add...
int MTabCarrier::GetZPosParameter(double* rgsFixedPos, double* rgsOffsetPos, double* rgsModelPos)
{
	for (int i = 0; i < DEF_TABCARRIER_Z_MAX_POS; i++)
	{
		if (rgsFixedPos != NULL)
		{
			rgsFixedPos[i] = m_rgsZFixedPos[i];
		}

		if (rgsOffsetPos != NULL)
		{
			rgsOffsetPos[i] = m_rgsZOffsetPos[i];
		}

		if (rgsModelPos != NULL)
		{
			rgsModelPos[i] = m_rgsZModelPos[i];
		}
	}
	return ERR_TABCARRIER_SUCCESS;
}

STeachingInfo MTabCarrier::GetXYTTeachingInfo()
{
	return m_sXYTTeachingInfo;
}

STeachingInfo MTabCarrier::GetExpandTeachingInfo()
{
	return m_sExpandTeachingInfo;
}

//SJ_YYK 150109 Add...
STeachingInfo MTabCarrier::GetZTeachingInfo()
{
	return m_sZTeachingInfo;
}

//int MTabCarrier::CheckVacuum(int iVacID)
int MTabCarrier::CheckVacuum()
{
//	ASSERT(iVacID >= DEF_VAC_TAB_CARRIER_1 && iVacID < DEF_MAX_WORKER_PER_GROUP);
//	return m_plnkVac[iVacID]->CompareIO();
	return m_plnkVac->CompareIO();
}

//int MTabCarrier::AbsorbTabIC(int iVacID, BOOL bSkipSensor)
int MTabCarrier::AbsorbTabIC(BOOL bSkipSensor)
{
//	ASSERT(iVacID >= DEF_VAC_TAB_CARRIER_ALL && iVacID < DEF_MAX_WORKER_PER_GROUP);
	WriteTactTimeLog("[MTabCarrier] [M] [Vacuum Absorb] [Start]");
	
	int iResult = ERR_TABCARRIER_SUCCESS;
	int i = 0;

	iResult = m_plnkVac->On(bSkipSensor);
	if (iResult)
		return iResult;

	WriteTactTimeLog("[MTabCarrier] [M] [Vacuum Absorb] [End]");
	return ERR_TABCARRIER_SUCCESS;
}

//int MTabCarrier::ReleaseTabIC(int iVacID, BOOL bSkipSensor)
int MTabCarrier::ReleaseTabIC(BOOL bSkipSensor)
{
//	ASSERT(iVacID >= DEF_VAC_TAB_CARRIER_ALL && iVacID < DEF_MAX_WORKER_PER_GROUP);
	WriteTactTimeLog("[MTabCarrier] [M] [Vacuum Release] [Start]");

	int iResult = ERR_TABCARRIER_SUCCESS;
	int i = 0;

	iResult = m_plnkVac->Off(bSkipSensor);
	if (iResult)
		return iResult;

	WriteTactTimeLog("[MTabCarrier] [M] [Vacuum Release] [End]");
	return ERR_TABCARRIER_SUCCESS;
}

//BOOL MTabCarrier::IsAbsorbTabIC(int iVacID)
BOOL MTabCarrier::IsAbsorbTabIC()
{
	return m_plnkVac->IsOn();
}

//BOOL MTabCarrier::IsReleaseTabIC(int iVacID)
BOOL MTabCarrier::IsReleaseTabIC()
{
	return m_plnkVac->IsOff();
}

int MTabCarrier::Up(BOOL bSkipSensor)
{
	//170711 JSH.s
#ifdef DEF_SETUP_NONE_IN_LINE
	return FALSE;
#endif
	//170711 JSH.e
	ASSERT(m_plnkUDCyl != NULL);

	if (TRUE == m_plnkUDCyl->IsUp())
		return ERR_TABCARRIER_SUCCESS;

	WriteTactTimeLog("[MTabCarrier] [M] [Up] [Start]");
	
	int iResult = checkInterlockForUp();
	if (iResult)
		return iResult;
	iResult = m_plnkUDCyl->Up(bSkipSensor);
	if (iResult)
		return iResult;
	
	WriteTactTimeLog("[MTabCarrier] [M] [Up] [End]");
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::Up2(BOOL bSkipSensor)
{
	//170711 JSH.s
#ifdef DEF_SETUP_NONE_IN_LINE
	return FALSE;
#endif
	//170711 JSH.e
	ASSERT(m_plnkUDCyl2 != NULL);

	if (TRUE == m_plnkUDCyl2->IsUp())
		return ERR_TABCARRIER_SUCCESS;

	WriteTactTimeLog("[MTabCarrier] [M] [Up] [Start]");
	
	int iResult = checkInterlockForUp();
	if (iResult)
		return iResult;

	iResult = m_plnkUDCyl2->Up(bSkipSensor);
	if (iResult)
		return iResult;
	
	WriteTactTimeLog("[MTabCarrier] [M] [Up] [End]");
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::Forward(BOOL bSkipSensor)
{
	return 0;

	ASSERT(m_plnkUDCyl != NULL);
	if (TRUE == m_plnkFBCyl->IsFront())
		return ERR_TABCARRIER_SUCCESS;
	
	WriteTactTimeLog("[MTabCarrier] [M] [Forward] [Start]");
	
/*	int iResult = checkInterlockForUp();
	if (iResult)
	{
		SetErrorLevel(_T("TAB CARRIER up"),2, __FILE__, __LINE__);
		return iResult;
	}
*/
	int iResult = m_plnkFBCyl->Front(bSkipSensor);
	if (iResult)
		return iResult;
	
	WriteTactTimeLog("[MTabCarrier] [M] [Forward] [End]");
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::Down(BOOL bSkipSensor)
{
	//170711 JSH.s
#ifdef DEF_SETUP_NONE_IN_LINE
	return FALSE;
#endif
	//170711 JSH.e
	int iResult;

	ASSERT(m_plnkUDCyl != NULL);

	m_plnkUDCyl->Down(TRUE);

	if (TRUE == m_plnkUDCyl->IsDown())
		return ERR_TABCARRIER_SUCCESS;


	WriteTactTimeLog("[MTabCarrier] [M] [Down] [Start]");
	iResult = m_plnkUDCyl->Down(bSkipSensor);
	if (iResult)
		return iResult;
	
	WriteTactTimeLog("[MTabCarrier] [M] [Down] [End]");
	return ERR_TABCARRIER_SUCCESS;
}
int MTabCarrier::Down2(BOOL bSkipSensor)
{
	//170711 JSH.s
#ifdef DEF_SETUP_NONE_IN_LINE
	return FALSE;
#endif
	//170711 JSH.e
	int iResult;
	ASSERT(m_plnkUDCyl2 != NULL);

	m_plnkUDCyl2->Down(TRUE);

	if (TRUE == m_plnkUDCyl2->IsDown())
		return ERR_TABCARRIER_SUCCESS;

	WriteTactTimeLog("[MTabCarrier] [M] [Down] [Start]");

	iResult = m_plnkUDCyl2->Down(bSkipSensor);
	if (iResult)
		return iResult;
	
	WriteTactTimeLog("[MTabCarrier] [M] [Down] [End]");
	return ERR_TABCARRIER_SUCCESS;
}
int MTabCarrier::Backward(BOOL bSkipSensor)
{
	return 0;
	ASSERT(m_plnkUDCyl != NULL);

	m_plnkFBCyl->Back(TRUE);

	if (TRUE == m_plnkFBCyl->IsBack())
		return ERR_TABCARRIER_SUCCESS;

	WriteTactTimeLog("[MTabCarrier] [M] [BackWard] [Start]");
	
	int iResult = m_plnkFBCyl->Back(bSkipSensor);
	if (iResult)
		return iResult;
	
	WriteTactTimeLog("[MTabCarrier] [M] [BackWard] [End]");
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::ShrinkCamera(BOOL bSkipSensor)
{
	int iResult = SafeMoveExpandPos(DEF_TABCARRIER_EXPAND_WORKING_POS, !bSkipSensor);
	if (iResult)
		return iResult;
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::ExpandCamera(BOOL bSkipSensor)
{
	int iResult = SafeMoveExpandPos(DEF_TABCARRIER_EXPAND_WORKING_POS, !bSkipSensor);
	if (iResult)
		return iResult;
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::ExpandShrinkForPreAlign(BOOL bSkipSensor)
{
	int iResult = ERR_TABCARRIER_SUCCESS;
	if (FALSE == IsInExpandPos(DEF_TABCARRIER_EXPAND_WORKING_POS))
		iResult = SafeMoveExpandPos(DEF_TABCARRIER_EXPAND_WORKING_POS, !bSkipSensor);
	return iResult;
}

BOOL MTabCarrier::IsUp(void)
{
	//170711 JSH.s
#ifdef DEF_SETUP_NONE_IN_LINE
	return TRUE;
#endif
	//170711 JSH.e

	ASSERT(m_plnkUDCyl);
	return m_plnkUDCyl->IsUp();
}
BOOL MTabCarrier::IsUp2(void)
{
	//170711 JSH.s
#ifdef DEF_SETUP_NONE_IN_LINE
	return TRUE;
#endif
	//170711 JSH.e
	ASSERT(m_plnkUDCyl2);
	return m_plnkUDCyl2->IsUp();
}
BOOL MTabCarrier::IsDown(void)
{
	//170711 JSH.s
#ifdef DEF_SETUP_NONE_IN_LINE
	return TRUE;
#endif
	//170711 JSH.e
	ASSERT(m_plnkUDCyl);

	return m_plnkUDCyl->IsDown();
}
BOOL MTabCarrier::IsDown2(void)
{
	//170711 JSH.s
#ifdef DEF_SETUP_NONE_IN_LINE
	return TRUE;
#endif
	//170711 JSH.e
	ASSERT(m_plnkUDCyl2);

	return m_plnkUDCyl2->IsDown();
}
BOOL MTabCarrier::IsForward(void)
{
	ASSERT(m_plnkFBCyl);
	return m_plnkFBCyl->IsFront();
}

BOOL MTabCarrier::IsBackward(void)
{
	ASSERT(m_plnkFBCyl);

	return m_plnkFBCyl->IsBack();
}

BOOL MTabCarrier::IsShrinkCamera()
{
	return IsInExpandPos(DEF_TABCARRIER_EXPAND_WORKING_POS);
}

BOOL MTabCarrier::IsExpandCamera()
{
	return IsInExpandPos(DEF_TABCARRIER_EXPAND_WORKING_POS);
}

int MTabCarrier::ForwardDumpBasket(BOOL bSkipSensor)
{
	int iResult = ERR_TABCARRIER_SUCCESS;
	return iResult;
}

int MTabCarrier::BackwardDumpBasket(BOOL bSkipSensor)
{
	int iResult = ERR_TABCARRIER_SUCCESS;
	return iResult;
}

BOOL MTabCarrier::IsForwardDumpBasket()
{
	return FALSE;
}

BOOL MTabCarrier::IsBackwardDumpBasket()
{
	return TRUE;
}

int MTabCarrier::SafeMoveXYTPos(int iPosID, BOOL bAuto)
{
	//101006.KKY.Remove위치로 이동은 MCtrlTabCarrier에서 Unloading위치로 이동후
	//MoveXYTPos함수를 통해서만 이동할수 있도록...
	ASSERT(iPosID < DEF_TABCARRIER_MAX_POSITION);

	if (iPosID == DEF_TABCARRIER_LOAD_POS || iPosID == DEF_TABCARRIER_READY_POS
//@#ifdef DEF_GATE_SYSTEM //20120215 SJ_HJG
		 || iPosID == DEF_TABCARRIER_ESCAPE_POS || iPosID == DEF_TABCARRIER_LOAD_START_POS //SJ_YYK 150109 Add..
//@#endif
		) 
	{
		InitAlignMarkOffset();
	}
	MPos_XYT rgsTarget = GetTargetXYTPos(iPosID);	// 목표 위치 읽기
	return MoveXYTPos(rgsTarget, iPosID, bAuto);		// 해당 위치로 이동
}

int MTabCarrier::MoveXYTPos(MPos_XYT posTarget, int iPosID, BOOL bWaitOpt, BOOL bAuto)
{
#ifdef SIMULATION
	return ERR_TABCARRIER_SUCCESS;
#endif

	//101006.KKY.Remove위치로 이동은 MCtrlTabCarrier에서 Unloading위치로 이동후
	//MoveXYTPos함수를 통해서만 이동할수 있도록...
	ASSERT(iPosID < DEF_TABCARRIER_MAX_POSITION);

	// jdy sesl
	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabCarrier] [M] [Carrier Move to PosID:%d] [Start]", iPosID);
	WriteTactTimeLog(szLogMsg);

	// Interlock 확인
	int iResult = checkInterlockForXYT(posTarget, iPosID, bAuto);
	if (iResult)
		return iResult;

	if (iPosID == DEF_TABCARRIER_LOAD_POS || iPosID == DEF_TABCARRIER_READY_POS
//@#ifdef DEF_GATE_SYSTEM //20120215 SJ_HJG
		 || iPosID == DEF_TABCARRIER_ESCAPE_POS || iPosID == DEF_TABCARRIER_LOAD_START_POS //SJ_YYK 150109 Add..
//@#endif		
		)
	{
		InitAlignMarkOffset();
	}

	//110414.Add_____
	BOOL bChkTabMounter = FALSE;
	double dTolerance = 2.0;
	DWORD sTime = 0;
	//_______________

	/*/
#ifdef DEF_TAB_SUPPLY_DIR_X

	m_csMoveLock.Lock();
	SetMovingY(TRUE, posTarget);

	//현재위치Y가 (Align-Gap/2)위치보다 작거나 같고, 목표위치Y가 (Align-Gap/2)위치보가 크거나 같을때
	//현재위치Y가 (Align+Gap/2)위치보다 크거나 같고, 목표위치Y가 (Align+Gap/2)위치보다 작거나 같을때
	//	현재위치X가 인식위치X보다 작을경우
	//		목표위치X가 인식위치X보다 작을경우
	//			X,Y,T축 동시 이동
	//		목표위치X가 인식위치X보다 클경우
	//			Y축 이동후 X,T축 동시이동
	//	현재위치X가 인식위치X보다 클경우
	//		목표위치x가 인식위치X보다 작을경우
	//			X축 목표위치로 이동후 Y,T축 동시이동
	//		목표위치X가 인식위치X보다 클경우
	//			X축 안전위치(인식위치-2.0)로 이동후 Y축 이동후 X,T축 동시이동
	//현재위치Y가 (Align-Gap/2, Align+Gap/2)사이에 있을때
	//	목표위치 X가 인식위치 X보다 작을경우
	//		X,Y,T축 동시 이동
	//	목표위치 X가 인식위치 X보가 클경우
	//		Y이동후 X,T축 동시이동
	//아니면
	//	X,T,T축 동시 이동

	BOOL bYFirstMove = FALSE;//X먼저 이동할지 결정.이동위치는 dXFirstTarget에서 지정
	double dYFirstTarget = 0.0;
	int	nMoveType = 0;//0->X먼저 이동후 X,Y이동		1->X,Y,T동시 이동

	MPos_XYT CurPosXYT = GetCurrentXYTPos();
	MPos_XYT AlignPosXYT = GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS);
	MPos_XYT TgPosXYT = posTarget;
	double	dSafeWidth = DEF_TABCARRIER_SAFE_GAP;

	bYFirstMove = FALSE;
	//X위치가 Remove를 제외한 위치보다 클경우 X부터 이동후 Y이동하게 한다
	double dYMaxPos = -10000.0;
	for (int i=0; i<DEF_TABCARRIER_MAX_POSITION; i++)
	{
		if (dYMaxPos < GetTargetXYTPos(i).dY)
			dYMaxPos = GetTargetXYTPos(i).dY;
	}
	if (CurPosXYT.dY > dYMaxPos + 0.5)
	{
		bYFirstMove = TRUE;
		dYFirstTarget = TgPosXYT.dY;
	}

	nMoveType = 1;//0->X먼저 이동후 X,Y이동		1->X,Y,T동시 이동

	if (bYFirstMove)
	{
		iResult = m_plnkY->Move(dYFirstTarget);
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			SetMovingY(FALSE, posTarget);
			m_csMoveLock.Unlock();
			SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
			return iResult;
		}
	}

	iResult = m_plnkX->StartMove(posTarget.dX);
	if (iResult != ERR_TABCARRIER_SUCCESS)
	{
		m_plnkX->Stop();
		SetMovingY(FALSE, posTarget);
		m_csMoveLock.Unlock();
		SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
		return iResult;
	}

	//Ready위치로 이동시 tacttime줄이기 위해 조금이라도 더 같이 움직이도록 Sleep을 없앰
	if (iPosID != DEF_TABCARRIER_READY_POS)
	{
		//	Sleep(50);
		Sleep(10);
	}

	m_csMoveLock.Unlock();

	if (nMoveType == 0)
	{
		Sleep(10);//110414.Add
//		iResult = m_plnkY->Wait4Done();
		sTime = GetTickCount();
		while(1)
		{
			Sleep(10);
			if (m_plnkX->IsAxisDone())
			{
				iResult = m_plnkX->Wait4Done();
				break;
			}
			if (GetTickCount() - sTime > 15*1000)
			{
				iResult = m_plnkX->Wait4Done();
				break;
			}
//@			iResult = checkInterlockTabMounter4MoveUnload(iPosID);
//@			if (iResult)
//@				break;
		}
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			m_plnkY->EStop();
			SetMovingY(FALSE, posTarget);
			SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
			return iResult;
		}
	}
	
	/ *
	//Y먼저 이동후 Delay후 X이동하게_____
	if (FALSE == IsInXPos(posTarget.dX))
		Sleep(200);
	//___________________________________
	* /

	iResult = m_plnkY->StartMove(posTarget.dY);
	if (iResult != ERR_TABCARRIER_SUCCESS)
	{
		m_plnkY->Stop();
		m_plnkX->Stop();
		SetMovingY(FALSE, posTarget);
		SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
		return iResult;
	}
	iResult = m_plnkT->StartMove(posTarget.dT);
	if (iResult != ERR_TABCARRIER_SUCCESS)
	{
		m_plnkY->Stop();
		m_plnkX->Stop();
		m_plnkT->Stop();
		SetMovingY(FALSE, posTarget);
		SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
		return iResult;
	}

	if (!bWaitOpt)
	{
		SetMovingY(FALSE, posTarget);
		return ERR_TABCARRIER_SUCCESS;
	}	

///	iResult = m_plnkY->Wait4Done(TRUE);
	Sleep(10);
	sTime = GetTickCount();
	while(1)
	{
		Sleep(10);
		if (m_plnkX->IsAxisDone())
		{
			iResult = m_plnkX->Wait4Done();
			break;
		}
		if (GetTickCount() - sTime > 15*1000)
		{
			iResult = m_plnkX->Wait4Done();
			break;
		}
//@		iResult = checkInterlockTabMounter4MoveUnload(iPosID);
//@		if (iResult)
//@			break;
	}
	if (iResult != ERR_TABCARRIER_SUCCESS)
	{
		m_plnkY->EStop();
		m_plnkX->Stop();
		m_plnkT->Stop();
		SetMovingY(FALSE, posTarget);
		SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
		return iResult;
	}
	SetMovingY(FALSE, posTarget);

	iResult = m_plnkY->Wait4Done(TRUE);
	if (iResult != ERR_TABCARRIER_SUCCESS)
	{
		m_plnkY->Stop();
		m_plnkT->Stop();
		SetMovingY(FALSE, posTarget);
		SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
		return iResult;
	}

	iResult = m_plnkT->Wait4Done();
	if (iResult != ERR_TABCARRIER_SUCCESS)
	{
		SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
		return iResult;
	}

 #else		//=====#ifdef DEF_TAB_SUPPLY_DIR_X
	/*/

	MPos_XYT CurPos = GetCurrentXYTPos();
	BOOL bMoveT = FALSE;
	if (fabs(posTarget.dT - CurPos.dT) > 5.0)
		bMoveT = TRUE;

	int iCurPosID = DEF_TABCARRIER_NO_WORKING_POS;
	MPos_XYT posTolerance(1.5, 1.5, 1.5);
	if (IsInXYTPos(GetTargetXYTPos(DEF_TABCARRIER_READY_POS), &posTolerance))
		iCurPosID = DEF_TABCARRIER_READY_POS;
	else if (IsInXYTPos(GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS), &posTolerance))
		iCurPosID = DEF_TABCARRIER_LOAD_POS;
	else if (IsInXYTPos(GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS), &posTolerance))
		iCurPosID = DEF_TABCARRIER_PREALIGN_POS;
	else if (IsInXYTPos(GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS), &posTolerance))
		iCurPosID = DEF_TABCARRIER_UNLOAD_POS;
	else if (IsInXYTPos(GetPrealignReadyPos(), &posTolerance))
		iCurPosID = DEF_TABCARRIER_LOAD_POS;
	else if (IsInXYTPos(GetUnloadingReadyPos(), &posTolerance))
		iCurPosID = DEF_TABCARRIER_UNLOAD_POS;
	else if (IsInXYTPos(GetTargetXYTPos(DEF_TABCARRIER_ESCAPE_POS), &posTolerance))
		iCurPosID = DEF_TABCARRIER_ESCAPE_POS;
	else if (IsInXYTPos(GetTargetXYTPos(DEF_TABCARRIER_LOAD_START_POS), &posTolerance)) //SJ_YYK 150109 Add..
		iCurPosID = DEF_TABCARRIER_LOAD_START_POS;

	IAxis* pAxisLong = m_plnkX;
	IAxis* pAxisShort = m_plnkY;
	double dTgShort = posTarget.dY;
	double dTgLong = posTarget.dX;
	double dSafePos = GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;


	//SJ_YYK 150211 Add..
	double dCarrierXSpeed = 0.0;
	/*
	if(MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
	{
		if(iPosID == DEF_TABCARRIER_LOAD_POS
			|| iPosID == DEF_TABCARRIER_PREALIGN_POS)
			dCarrierXSpeed = 200.0;
	}
	//___________________*/
	double dYMoveSafeZoneCarrierX = GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS).dX;

	//비정상경로로 이동시
	//	X안전위치(0.0) 이동 -> T안전위치(0.0) 이동 -> Y이동 -> T이동 -> X이동
	if (iCurPosID == DEF_TABCARRIER_NO_WORKING_POS)
	{
//@		double dXSafePos = GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dX;
		//X축 안전위치로 이동
		//170725 JSH.s
//		if (dYMoveSafeZoneCarrierX - 1.0 <= CurPos.dX
//			&& bAuto == FALSE)	//jskim Y cnr 그자리에서/..
		//170725 JSH.e
		{
			iResult = pAxisShort->Move(dSafePos);
			if (iResult != ERR_TABCARRIER_SUCCESS)
				return iResult;
		}
		//Y축 목표위치로 이동
		m_csMoveLock.Lock();
		SetMovingY(TRUE, posTarget);
		iResult = pAxisLong->StartMove(dTgLong);
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			m_csMoveLock.Unlock();
			return iResult;
		}
		m_csMoveLock.Unlock();
		Sleep(10);
		if (dYMoveSafeZoneCarrierX <= CurPos.dX - 1.0
			&& bAuto == FALSE)	//jskim Y cnr 그자리에서/..
		{
			iResult = pAxisLong->Wait4Done();
			if (iResult != ERR_TABCARRIER_SUCCESS)
			{
				pAxisLong->Stop();
				SetMovingY(FALSE, posTarget);
				return iResult;
			}
			SetMovingY(FALSE, posTarget);
		}
		//T축 목표위치로 이동
		//if(bAuto == FALSE)
		{
			iResult = m_plnkT->Move(posTarget.dT);
		}
		/*
		else
		{
			iResult = m_plnkT->StartMove(posTarget.dT);
		}
		*/

		if (iResult != ERR_TABCARRIER_SUCCESS)
			return iResult;
		//X축 목표위치로 이동
		if(bAuto == FALSE)
		{
			iResult = pAxisShort->Move(dTgShort);
		}
		else
		{
			iResult = pAxisShort->StartMove(dTgShort);
		}

		if (iResult != ERR_TABCARRIER_SUCCESS)
			return iResult;

		if (dYMoveSafeZoneCarrierX >= CurPos.dX - 1.0
			|| bAuto == TRUE)	//jskim Y cnr 그자리에서/..
		{
			iResult = pAxisLong->Wait4Done();
			if (iResult != ERR_TABCARRIER_SUCCESS)
			{
				pAxisLong->Stop();
				SetMovingY(FALSE, posTarget);
				return iResult;
			}
			SetMovingY(FALSE, posTarget);

			iResult = pAxisShort->Wait4Done();
			if (iResult != ERR_TABCARRIER_SUCCESS)
			{
				pAxisShort->Stop();
				return iResult;
			}

			iResult = m_plnkT->Wait4Done();
			if (iResult != ERR_TABCARRIER_SUCCESS)
			{
				m_plnkT->Stop();
				return iResult;
			}
		}
		memset(szLogMsg, 0x00, sizeof(szLogMsg));
		sprintf(szLogMsg, "[MTabCarrier] [M] [Carrier Move to PosID:%d] [End]", iPosID);
		WriteTactTimeLog(szLogMsg);
		return ERR_TABCARRIER_SUCCESS;
	}

	//정상경로로 이동시
	//0. T이동 -> XY동시이동
	//1. XY이동 -> 안전위치보다클때 T이동
	//2. XYT동시이동
	//3. XYT동시이동인데 Y축이 Up실린더 벗어나면 X축 이동
	//4. X축 이동후 YT이동
	int	nMoveType = 0;
	switch (iCurPosID)
	{
	case DEF_TABCARRIER_READY_POS:
	case DEF_TABCARRIER_LOAD_POS:
		switch (iPosID)
		{
			case DEF_TABCARRIER_LOAD_POS:
			case DEF_TABCARRIER_READY_POS:
				nMoveType = 2;
				break;
			case DEF_TABCARRIER_PREALIGN_POS:
			
			case DEF_TABCARRIER_UNLOAD_POS:
			case DEF_TABCARRIER_ESCAPE_POS:
				//SJ_YYK 151021 Modify..
				//nMoveType = 3;
				nMoveType = 0;
				break;
//@				nMoveType = 1;
		//		nMoveType = 3;
		//		break;
		}
		break;
	case DEF_TABCARRIER_PREALIGN_POS:
		switch (iPosID)
		{
//			case DEF_TABCARRIER_PREALIGN_POS:
//				break;
			case DEF_TABCARRIER_READY_POS:
			case DEF_TABCARRIER_LOAD_POS:
				nMoveType = 4;
				break;
			case DEF_TABCARRIER_UNLOAD_POS:
			case DEF_TABCARRIER_ESCAPE_POS:
				nMoveType = 1;
				break;
		}
		break;
	case DEF_TABCARRIER_UNLOAD_POS:
	case DEF_TABCARRIER_ESCAPE_POS:
		switch (iPosID)
		{
			case DEF_TABCARRIER_READY_POS:
			case DEF_TABCARRIER_LOAD_POS:
				nMoveType = 2;
//@				nMoveType = 4;
				break;
			case DEF_TABCARRIER_PREALIGN_POS:
				nMoveType = 0;
				break;
			case DEF_TABCARRIER_UNLOAD_POS:
			case DEF_TABCARRIER_ESCAPE_POS:
				nMoveType = 2;
				break;
		}
		break;
	default:
		break;
	}
	//0. T이동 -> XY동시이동
	if (nMoveType == 0)
	{
		/*
		if (FALSE == IsInTPos(posTarget.dT))
		{
			iResult = m_plnkT->Move(posTarget.dT);
			if (iResult)
			{
				SetErrorLevel(_T("TAB CARRIER move Position"),2, __FILE__, __LINE__);
				return iResult;
			}
		}
		*/
		//170216_KDH Tact Test중.
		if (FALSE == IsInTPos(posTarget.dT))
		{
			iResult = m_plnkT->StartMove(posTarget.dT);
			if (iResult)
				return iResult;
			//TEST
			//double dTCurPos = GetCurrentXYTPos().dT;
			
			//__________
			MTickTimer TickTimer;
			TickTimer.StartTimer();
			double dTAxisTol = 60.0;  //30도까지 안전
			double TCurPos;
			while(1)
			{
				TCurPos = GetCurrentXYTPos().dT;
				Sleep(15);
				//if (fabs(GetCurrentXYTPos().dT - posTarget.dT) < dTAxisTol || m_plnkT->IsAxisDone())
				if (fabs(TCurPos - posTarget.dT) < dTAxisTol || m_plnkT->IsAxisDone())
					break;
				if(TickTimer.MoreThan(30.0))
				{
					m_plnkT->Stop();
					return generateErrorCode(317030);
				}
			}
		}
	
		//____________________
		iResult = pAxisShort->StartMove(dTgShort);
		if (iResult)
		{
			m_plnkX->Stop();
			return iResult;
		}
		m_csMoveLock.Lock();
		SetMovingY(TRUE, posTarget);
		iResult = pAxisLong->StartMove(dTgLong);
		if (iResult)
		{
			pAxisShort->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			m_csMoveLock.Unlock();
			return iResult;
		}
//		Sleep(20);
		Sleep(10);
		m_csMoveLock.Unlock();
		iResult = pAxisShort->Wait4Done(TRUE);
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			pAxisShort->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		iResult = pAxisLong->Wait4Done();
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		SetMovingY(FALSE, posTarget);
		//170217_KDH Test..
		iResult = m_plnkT->Wait4Done();
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			m_plnkT->Stop();
			return iResult;
		}
		//_____________
	}
	//1. XY이동 -> 안전위치보다클때 T이동
	else if (nMoveType == 1)
	{
		iResult = pAxisShort->StartMove(dTgShort);
		if (iResult)
		{
			pAxisShort->Stop();
			return iResult;
		}
		m_csMoveLock.Lock();
		SetMovingY(TRUE, posTarget);
		iResult = pAxisLong->StartMove(dTgLong);
		if (iResult)
		{
			pAxisShort->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			m_csMoveLock.Unlock();
			return iResult;
		}
		m_csMoveLock.Unlock();
		Sleep(10);
//		double dSafePos = m_plnkStateTabCarrier[0]->GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS).dY;		
		double dSafePos = GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS).dX - 40.0;
		while(1)
		{
			Sleep(15);
			if (GetCurrentXYTPos().dX < dSafePos + 1.0 || m_plnkX->IsAxisDone())
				break;
		}
		iResult = m_plnkT->Move(posTarget.dT);
		if (iResult)
		{
			pAxisShort->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		iResult = pAxisShort->Wait4Done(TRUE);
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			pAxisShort->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		iResult = pAxisLong->Wait4Done();
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		SetMovingY(FALSE, posTarget);
	}
	//2. XYT동시이동
	//3. XYT동시이동인데 Y축이 Up실린더 벗어나면 X축 이동
	else if (nMoveType == 2 || nMoveType == 3)
	{
		m_csMoveLock.Lock();
		SetMovingY(TRUE, posTarget);
		//SJ_YYK 150211 Modify..
		//iResult = pAxisLong->StartMove(dTgLong);
		iResult = pAxisLong->StartMove(dTgLong,dCarrierXSpeed);
		if (iResult)
		{
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			m_csMoveLock.Unlock();
			return iResult;
		}
		m_csMoveLock.Unlock();
//		Sleep(20);
		//@KKY.130908___________
		Sleep(10);
		//@_____________________
		if (nMoveType == 3)
		{
			double dSafeX = GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dX - 50.0;
			while(1)
			{
				Sleep(15);
				if (m_plnkX->GetCurrentPosition() < dSafeX || m_plnkX->IsMotionDone())
					break;
			}
		}
		iResult = pAxisShort->StartMove(dTgShort);
		if (iResult)
		{
			pAxisShort->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		iResult = m_plnkT->StartMove(posTarget.dT);
		if (iResult)
		{
			pAxisShort->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		if (bWaitOpt)
		{
			iResult = m_plnkT->Wait4Done(TRUE);
			if (iResult)
			{
				pAxisShort->Stop();
				pAxisLong->Stop();
				SetMovingY(FALSE, posTarget);
				return iResult;
			}

			iResult = pAxisShort->Wait4Done(TRUE);
			if (iResult != ERR_TABCARRIER_SUCCESS)
			{
				pAxisShort->Stop();
				pAxisLong->Stop();
				SetMovingY(FALSE, posTarget);
				return iResult;
			}
			iResult = pAxisLong->Wait4Done();
			if (iResult != ERR_TABCARRIER_SUCCESS)
			{
				pAxisLong->Stop();
				SetMovingY(FALSE, posTarget);
				return iResult;
			}
			SetMovingY(FALSE, posTarget);
		}
	}
	//4. X축 이동후 YT이동
	else if (nMoveType == 4)
	{
		iResult = pAxisShort->Move(dTgShort);
		if (iResult)
			return iResult;
		iResult = m_plnkT->StartMove(posTarget.dT);
		if (iResult)
		{
			m_plnkT->Stop();
			return iResult;
		}
		m_csMoveLock.Lock();
		SetMovingY(TRUE, posTarget);
		iResult = pAxisLong->StartMove(dTgLong);
		if (iResult)
		{
			m_plnkT->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			m_csMoveLock.Unlock();
			return iResult;
		}
		m_csMoveLock.Unlock();
		Sleep(20);
		iResult = m_plnkT->Wait4Done(TRUE);
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			m_plnkT->Stop();
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		iResult = pAxisLong->Wait4Done();
		if (iResult != ERR_TABCARRIER_SUCCESS)
		{
			pAxisLong->Stop();
			SetMovingY(FALSE, posTarget);
			return iResult;
		}
		SetMovingY(FALSE, posTarget);
	}
//@#endif

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabCarrier] [M] [Carrier Move to PosID:%d] [End]", iPosID);
	WriteTactTimeLog(szLogMsg);

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::SafeMoveExpandPos(int iPosID, BOOL bWaitOpt, BOOL bAuto)
{
	ASSERT(iPosID < DEF_TABCARRIER_EXPAND_MAX_POS);
	double rgsTarget = GetTargetExpandPos(iPosID);	// 목표 위치 읽기
	return MoveExpandPos(rgsTarget, iPosID, bWaitOpt, bAuto);		// 해당 위치로 이동
}

//SJ_YYK 150109 Add....
int MTabCarrier::SafeMoveZPos(int iPosID, BOOL bWaitOpt, BOOL bAuto)
{
	ASSERT(iPosID < DEF_TABCARRIER_Z_MAX_POS);
	double rgsTarget = GetTargetZPos(iPosID);	// 목표 위치 읽기
	return MoveZPos(rgsTarget, iPosID, bWaitOpt, bAuto);
}

int MTabCarrier::MoveXPos(double posTarget)
{
	//@X방향 단독 이동은 Tab공급 Y방향 에서만 사용
	ASSERT(0);

	int iResult = ERR_TABCARRIER_SUCCESS;

	if (FALSE == m_plnkX->IsOriginReturn())
		return generateErrorCode(217011);	// Tab Carrier X축 원점복귀 되어 있지 않습니다.

	if (TRUE == IsInXPos(posTarget))
		return ERR_TABCARRIER_SUCCESS;

	iResult = m_plnkX->StartMove(posTarget);
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;

	iResult = m_plnkX->Wait4Done(TRUE);
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::MoveYPos(double posTarget)
{
	//@Y방향 단독 이동은 Tab공급 X방향 에서만 사용

	int iResult = ERR_TABCARRIER_SUCCESS;

	if (FALSE == m_plnkY->IsOriginReturn())
		return generateErrorCode(217006);	// Tab Carrier Y축 원점복귀 되어 있지 않습니다.

	if (TRUE == IsInYPos(posTarget))
		return ERR_TABCARRIER_SUCCESS;

	iResult = m_plnkY->StartMove(posTarget);
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;

	iResult = m_plnkY->Wait4Done(TRUE);
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;

	return ERR_TABCARRIER_SUCCESS;

}

int MTabCarrier::MoveTPos(double posTarget)
{
	int iResult = ERR_TABCARRIER_SUCCESS;
	if (FALSE == m_plnkT->IsOriginReturn())
		return generateErrorCode(217026);	// Tab Carrier X축 원점복귀 되어 있지 않습니다.

	double dTolerance = 0.001;
	if (TRUE == IsInTPos(posTarget, &dTolerance))
		return ERR_TABCARRIER_SUCCESS;

	iResult = m_plnkT->StartMove(posTarget);
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;

	iResult = m_plnkT->Wait4Done(TRUE);
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::MoveExpandPos(double posTarget, int iPosID, BOOL bWaitOpt, BOOL bAuto)
{
	int iResult = ERR_TABCARRIER_SUCCESS;

	if (FALSE == m_plnkExpandAxis->IsOriginReturn())
		return generateErrorCode(217031);	// Tab Carrier X축 원점복귀 되어 있지 않습니다.

	if (TRUE == IsInExpandPos(posTarget))
		return ERR_TABCARRIER_SUCCESS;

	iResult = m_plnkExpandAxis->StartMove(posTarget);
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;

	iResult = m_plnkExpandAxis->Wait4Done();
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;

	return ERR_TABCARRIER_SUCCESS;
}

//SJ_YYK 150109 Add...
int MTabCarrier::MoveZPos(double posTarget, int iPosID, BOOL bWaitOpt, BOOL bAuto)
{
	int iResult = ERR_TABCARRIER_SUCCESS;

	if (FALSE == m_plnkZAxis->IsOriginReturn())
		return generateErrorCode(217031);	// Tab Carrier X축 원점복귀 되어 있지 않습니다.

	if (TRUE == IsInZPos(posTarget))
		return ERR_TABCARRIER_SUCCESS;

	iResult = m_plnkZAxis->StartMove(posTarget);
	if (iResult != ERR_TABCARRIER_SUCCESS)
		return iResult;

	if(bWaitOpt == TRUE)
	{
		iResult = m_plnkZAxis->Wait4Done();
		if (iResult != ERR_TABCARRIER_SUCCESS)
			return iResult;
	}

	return ERR_TABCARRIER_SUCCESS;
}

#if FALSE	//===================================
///////////////////////////////////////////////////////////////////////////////////
// 2009.04.14 SESL_CYJ
int MTabCarrier::SafeMoveYToTurnPos(BOOL bXUseLimitSelect)
{
	MPos_XY rgsTarget = GetTargetXYPos(DEF_TABCARRIER_TURN_POS);	// 목표 위치 읽기
	return  MoveYToTurnPos(rgsTarget, bXUseLimitSelect);		// 해당 위치로 이동
}

int MTabCarrier::MoveYToTurnPos(MPos_XY posTarget, BOOL bXUseLimitSelect, BOOL bWaitOpt)
{
#ifdef SIMULATION
	return ERR_TABCARRIER_SUCCESS;
#endif

	char szLogMsg[200];

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabCarrier] [M] [Carrier Y Move to Turn Pos:%d] [Start]");
	WriteTactTimeLog(szLogMsg);

	// Interlock 확인
	int iResult = checkInterlockForYMoveToTurnPos(bXUseLimitSelect);
	if (iResult)
	{
		SetErrorLevel(_T("TAB CARRIER Y move to Turn Position"),2, __FILE__, __LINE__);
		return iResult;
	}
		
	iResult = m_plnkY->Move(posTarget.dY);
	if (iResult)
	{
		m_plnkY->Stop();
		SetErrorLevel(_T("TAB CARRIER Y move to Turn Position"),2, __FILE__, __LINE__);
		return iResult;
	}		

	memset(szLogMsg, 0x00, sizeof(szLogMsg));
	sprintf(szLogMsg, "[MTabCarrier] [M] [Carrier Y Move to Turn Pos:%d] [End]");
	WriteTactTimeLog(szLogMsg);

	return ERR_TABCARRIER_SUCCESS;
}
////////////////////////////////////////////////////////////////
#endif	//#if FALSE==============================

int MTabCarrier::CheckOrigin()
{
	// TabCarrier X Motion Check Origin
	if (m_plnkX->IsOriginReturn() == FALSE)
		return generateErrorCode(217011);	// Tab Carrier X축 원점복귀 되어 있지 않습니다.

	// TabCarrier Y Motion Check Origin
	if (m_plnkY->IsOriginReturn() == FALSE)
		return generateErrorCode(217006);	// Tab Carrier Y축 원점복귀 되어 있지 않습니다.

	// TabCarrier Y Motion Check Origin
	if (m_plnkT->IsOriginReturn() == FALSE)
		return generateErrorCode(217023);	// Tab Carrier Y축 원점복귀 되어 있지 않습니다.

	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::CheckOriginX()
{
	if (m_plnkX->IsOriginReturn() == FALSE)
		return generateErrorCode(217011);	// Tab Carrier Y축 원점복귀 되어 있지 않습니다.
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::CheckOriginY()
{
	if (m_plnkY->IsOriginReturn() == FALSE)
		return generateErrorCode(217006);	// Tab Carrier Y축 원점복귀 되어 있지 않습니다.
	return ERR_TABCARRIER_SUCCESS;
}

int MTabCarrier::CheckOriginExpand()
{
	if (m_plnkExpandAxis->IsOriginReturn() == FALSE)
		return generateErrorCode(217031);	// ExpandY축 원점복귀 되어 있지 않습니다.
	return ERR_TABCARRIER_SUCCESS;
}

//SJ_YYK 150109 Add...
int MTabCarrier::CheckOriginZ()
{
	if (m_plnkZAxis->IsOriginReturn() == FALSE)
		return generateErrorCode(217031);	// ExpandY축 원점복귀 되어 있지 않습니다.
	return ERR_TABCARRIER_SUCCESS;
}

MPos_XYT MTabCarrier::GetCurrentXYTPos(BOOL bType)
{
	ASSERT(m_plnkX != NULL && m_plnkY != NULL && m_plnkT != NULL);

	MPos_XYT sCurPos;

	sCurPos.dX = m_plnkX->GetCurrentPosition(bType);
	sCurPos.dY = m_plnkY->GetCurrentPosition(bType);
	sCurPos.dT = m_plnkT->GetCurrentPosition(bType);

	return sCurPos;
}

double MTabCarrier::GetCurrentExpandPos(BOOL bType)
{
	ASSERT(m_plnkExpandAxis != NULL);
	double dCurPos = m_plnkExpandAxis->GetCurrentPosition(bType);
	return dCurPos;
}

//SJ_YYK 150109 Add...
double MTabCarrier::GetCurrentZPos(BOOL bType)
{
	ASSERT(m_plnkZAxis != NULL);
	double dCurPos = m_plnkZAxis->GetCurrentPosition(bType);
	return dCurPos;
}

MPos_XYT MTabCarrier::GetTargetXYTPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_TABCARRIER_MAX_POSITION);
	MPos_XYT posTarget = m_rgsXYTFixedPos[iPosID] + m_rgsXYTModelPos[iPosID] + m_rgsXYTOffsetPos[iPosID];
	if (IsAligned())
	{
		//posTarget = posTarget + m_rgAlignMarkOffset;
		/*if (iPosID == DEF_TABCARRIER_UNLOAD_POS)
		{
			MPos_XYT posAlignOffset = CalculateAlignMarkOffset(posTarget);
			posTarget = posTarget + posAlignOffset;
		}*/
		//@추가____
		if (iPosID == DEF_TABCARRIER_PREALIGN_POS || iPosID == DEF_TABCARRIER_UNLOAD_POS)
		{
			MPos_XYT posAlignOffset = CalculateAlignMarkOffset(posTarget);

			//SJ_YYK 150109 Modify..
			if(iPosID == DEF_TABCARRIER_UNLOAD_POS)
			{
			}
			//_______________________________________*/
			posTarget = posTarget + posAlignOffset;
		}
	}
	return posTarget;
}

double MTabCarrier::GetTargetExpandPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_TABCARRIER_EXPAND_MAX_POS);
	double dTarget = m_rgsExpandFixedPos[iPosID] + m_rgsExpandModelPos[iPosID] + m_rgsExpandOffsetPos[iPosID];
	return dTarget;
}

//SJ_YYK 150109 Add..
double MTabCarrier::GetTargetZPos(int iPosID)
{
	ASSERT(iPosID >= 0 && iPosID < DEF_TABCARRIER_Z_MAX_POS);
	double dTarget = m_rgsZFixedPos[iPosID] + m_rgsZModelPos[iPosID] + m_rgsZOffsetPos[iPosID];
	return dTarget;
}

BOOL MTabCarrier::IsInXYTPos(int iPosID, MPos_XYT* pTolerance)
{
	MPos_XYT posCompare = GetTargetXYTPos(iPosID);

	return IsInXYTPos(posCompare, pTolerance);
}

BOOL MTabCarrier::IsInXPos(int iPosID, double* pTolerance)
{
	double posCompare = GetTargetXYTPos(iPosID).dX;
	return IsInXPos(posCompare, pTolerance);
}

BOOL MTabCarrier::IsInYPos(int iPosID, double* pTolerance)
{
	double posCompare = GetTargetXYTPos(iPosID).dY;
	return IsInYPos(posCompare, pTolerance);
}

BOOL MTabCarrier::IsInTPos(int iPosID, double* pTolerance)
{
	double posCompare = GetTargetXYTPos(iPosID).dT;
	return IsInTPos(posCompare, pTolerance);
}

BOOL MTabCarrier::IsInExpandPos(int iPosID, double* pTolerance)
{
	double posCompare = GetTargetExpandPos(iPosID);
	return IsInExpandPos(posCompare, pTolerance);
}

//SJ_YYK 150109 Add..
BOOL MTabCarrier::IsInZPos(int iPosID, double* pTolerance)
{
	double posCompare = GetTargetZPos(iPosID);
	return IsInZPos(posCompare, pTolerance);
}

BOOL MTabCarrier::IsInXYTPos(MPos_XYT sTargetPos, MPos_XYT* pTolerance)
{
	BOOL bState[DEF_TABCARRIER_AXIS_MAX_NO];

	if (pTolerance != NULL)
	{
		 /* @param	bPosOpt              : 비교할 위치 종류, FALSE=현재위치, TRUE=Motion의 목표위치
		 * @param   dTargetPosition      : 비교할 좌표값
		 * @param   *pbJudge             : 비교결과
		 * @param   *pdPermission        : (OPTION = NULL) 비교허용 오차, NULL이면 내부 Tolerance값으로 비교한다.
		 * @param   *pdDeviation         : (OPTION = NULL) 비교 차이값
		 * @return	Error Code		     : 0 = SUCCESS  그 외 = 타 함수 Return Error
		 */
		bState[DEF_TABCARRIER_X_AXIS] = m_plnkX->ComparePosition(FALSE, sTargetPos.dX, &pTolerance->dX);
		bState[DEF_TABCARRIER_Y_AXIS] = m_plnkY->ComparePosition(FALSE, sTargetPos.dY, &pTolerance->dY);
		bState[DEF_TABCARRIER_T_AXIS] = m_plnkT->ComparePosition(FALSE, sTargetPos.dT, &pTolerance->dT);
	}
	else
	{
		bState[DEF_TABCARRIER_X_AXIS] = m_plnkX->ComparePosition(FALSE, sTargetPos.dX, NULL);
		bState[DEF_TABCARRIER_Y_AXIS] = m_plnkY->ComparePosition(FALSE, sTargetPos.dY, NULL);
		bState[DEF_TABCARRIER_T_AXIS] = m_plnkT->ComparePosition(FALSE, sTargetPos.dT, NULL);
	}

	return bState[DEF_TABCARRIER_X_AXIS] && bState[DEF_TABCARRIER_Y_AXIS] && bState[DEF_TABCARRIER_T_AXIS];
}

BOOL MTabCarrier::IsInXPos(double dTargetPos, double* pTolerance)
{
//	BOOL bState;
//	if (pTolerance != NULL)
//		bState = m_plnkX->ComparePosition(FALSE, dTargetPos, pTolerance);
//	return bState;
	return m_plnkX->ComparePosition(FALSE, dTargetPos, pTolerance);
}

BOOL MTabCarrier::IsInYPos(double dTargetPos, double* pTolerance)
{
//	BOOL bState;
//	if (pTolerance != NULL)
//		bState = m_plnkY->ComparePosition(FALSE, dTargetPos, pTolerance);
//	return bState;
	return m_plnkY->ComparePosition(FALSE, dTargetPos, pTolerance);
}

BOOL MTabCarrier::IsInTPos(double dTargetPos, double* pTolerance)
{
//	BOOL bState;
//	if (pTolerance != NULL)
//		bState = m_plnkT->ComparePosition(FALSE, dTargetPos, pTolerance);
//	return bState;
	return m_plnkT->ComparePosition(FALSE, dTargetPos, pTolerance);
}

BOOL MTabCarrier::IsInExpandPos(double dTargetPos, double* pTolerance)
{
	return m_plnkExpandAxis->ComparePosition(FALSE, dTargetPos, pTolerance);
}

//SJ_YYK 150109 Add..
BOOL MTabCarrier::IsInZPos(double dTargetPos, double* pTolerance)
{
	return m_plnkZAxis->ComparePosition(FALSE, dTargetPos, pTolerance);
}

//KKY______________________________________________________
//선택한 축의 현재 위치와 ComparePosID의 위치를 비교한다.
BOOL MTabCarrier::CompareCurrentPos(int CarrierAxisID, int ComparePosID, ECompareType CompareType)
{
	IAxis* pAxis = GetMotionComponent(CarrierAxisID);
	if (!pAxis->IsOriginReturn())
		return FALSE;
	double curPos = pAxis->GetCurrentPosition(FALSE);
	double compPos = 0.0;
	BOOL (MTabCarrier::*pFunc)(int iPosID, double* pdTolerance);
	switch (CarrierAxisID)
	{
	case DEF_TABCARRIER_X_AXIS:
		compPos = GetTargetXYTPos(ComparePosID).dX;
		pFunc = &MTabCarrier::IsInXPos;
		break;
	case DEF_TABCARRIER_Y_AXIS:
		compPos = GetTargetXYTPos(ComparePosID).dY;
		pFunc = &MTabCarrier::IsInYPos;
		break;
	case DEF_TABCARRIER_T_AXIS:
		compPos = GetTargetXYTPos(ComparePosID).dT;
		pFunc = &MTabCarrier::IsInTPos;
		break;
	default:
		ASSERT(0);
		break;
	}

	BOOL bResult = FALSE;
	switch(CompareType)
	{
	case MORE_THAN:
		if (curPos > compPos)
			bResult = TRUE;
		break;
	case MORE_THAN_EQUAL:
		if (curPos > compPos || (this->*pFunc)(ComparePosID, NULL))
			bResult = TRUE;
		break;
	case EQUAL:
		if ((this->*pFunc)(ComparePosID, NULL))
			bResult = TRUE;
		break;
	case LESS_THAN_EQUAL:
		if (curPos < compPos || (this->*pFunc)(ComparePosID, NULL))
			bResult = TRUE;
		break;
	case LESS_THAN:
		if (curPos < compPos)
			bResult = TRUE;
		break;
	default:
		ASSERT(0);
	}
	return bResult;
}

//선택한 축의 현재 위치와 dComparePos의 위치를 비교한다.
BOOL MTabCarrier::CompareCurrentPos(int CarrierAxisID, double dComparePos, ECompareType CompareType)
{
	IAxis* pAxis = GetMotionComponent(CarrierAxisID);
	if (!pAxis->IsOriginReturn())
		return FALSE;
	double curPos = pAxis->GetCurrentPosition(0);
	double compPos = dComparePos;
	BOOL (MTabCarrier::*pFunc)(double dPos, double* pdTolerance);
	switch (CarrierAxisID)
	{
	case DEF_TABCARRIER_X_AXIS:
		pFunc = &MTabCarrier::IsInXPos;
		break;
	case DEF_TABCARRIER_Y_AXIS:
		pFunc = &MTabCarrier::IsInYPos;
		break;
	case DEF_TABCARRIER_T_AXIS:
		pFunc = &MTabCarrier::IsInTPos;
		break;
	default:
		ASSERT(0);
		break;
	}

	BOOL bResult = FALSE;
	switch(CompareType)
	{
	case MORE_THAN:
		if (curPos > compPos)
			bResult = TRUE;
		break;
	case MORE_THAN_EQUAL:
		if (curPos > compPos || (this->*pFunc)(compPos, NULL))
			bResult = TRUE;
		break;
	case EQUAL:
		if ((this->*pFunc)(compPos, NULL))
			bResult = TRUE;
		break;
	case LESS_THAN_EQUAL:
		if (curPos < compPos || (this->*pFunc)(compPos, NULL))
			bResult = TRUE;
		break;
	case LESS_THAN:
		if (curPos < compPos)
			bResult = TRUE;
		break;
	default:
		ASSERT(0);
	}
	return bResult;
}

//선택한 축의 목표 위치와 ComparePosID의 위치를 비교한다.
BOOL MTabCarrier::CompareTargetPos(int CarrierAxisID, int TargetPosID, int ComparePosID, ECompareType CompareType)
{
	IAxis* pAxis = GetMotionComponent(CarrierAxisID);
	if (!pAxis->IsOriginReturn())
		return FALSE;
	double tgPos = 0.0;
	double compPos = 0.0;
	switch (CarrierAxisID)
	{
	case DEF_TABCARRIER_X_AXIS:
		tgPos = GetTargetXYTPos(TargetPosID).dX;
		compPos = GetTargetXYTPos(ComparePosID).dX;
		break;
	case DEF_TABCARRIER_Y_AXIS:
		tgPos = GetTargetXYTPos(TargetPosID).dY;
		compPos = GetTargetXYTPos(ComparePosID).dY;
		break;
	case DEF_TABCARRIER_T_AXIS:
		tgPos = GetTargetXYTPos(TargetPosID).dT;
		compPos = GetTargetXYTPos(ComparePosID).dT;
		break;
	default:
		ASSERT(0);
		break;
	}

	BOOL bResult = FALSE;
	switch(CompareType)
	{
	case MORE_THAN:
		if (tgPos > compPos)
			bResult = TRUE;
		break;
	case MORE_THAN_EQUAL:
		if (tgPos >= compPos)
			bResult = TRUE;
		break;
	case EQUAL:
		if (tgPos == compPos)
			bResult = TRUE;
		break;
	case LESS_THAN_EQUAL:
		if (tgPos <= compPos)
			bResult = TRUE;
		break;
	case LESS_THAN:
		if (tgPos < compPos)
			bResult = TRUE;
		break;
	default:
		ASSERT(0);
	}
	return bResult;
}
//_________________________________________________________

int MTabCarrier::VMoveVelocity(UINT nSel, BOOL dir)
{
	int iResult = GetMotionComponent(nSel)->JogMoveFast(dir);

	m_bJogStats = TRUE;

	return iResult;
}

int MTabCarrier::VMovePitch(UINT nSel, BOOL dir)
{
	int iResult = GetMotionComponent(nSel)->JogMoveSlow(dir);

	m_bJogStats = TRUE;

	return iResult;
}

int MTabCarrier::VMoveEnd()
{
	if (m_bJogStats)
	{
		m_plnkX->Stop();
		m_plnkY->Stop();
		m_plnkT->Stop();
		m_bJogStats = FALSE;
	}
	return ERR_TABCARRIER_SUCCESS;
}

void MTabCarrier::InitAlignMarkOffset(void)
{
	m_bMarkAligned = FALSE;
	m_rgAlignMarkOffset.Init(0.0, 0.0, 0.0);
	//SJ_YYK 150109 Add...
	MOlbSystemPre.GetTrsAutoManagerComponent()->WriteTabCarrierAlignResult(m_iWorkGroup, m_iWorkInstance, 0.0);
}

MPos_XYT MTabCarrier::CalculateAlignMarkOffset(MPos_XYT posxytTarget)
{
	MPos_XYT rgdFiduMark;

	if (m_bMarkAligned)
	{
		rgdFiduMark.dT = DEG2RAD(posxytTarget.dT - m_dValueThetaAlign);	// T차이값 계산

		// X축 보정값 계산
		rgdFiduMark.dX = cos(rgdFiduMark.dT) * m_rgAlignMarkOffset.dX 
			             - sin(rgdFiduMark.dT) * m_rgAlignMarkOffset.dY; 
		
		// Y축 보정값 계산
		rgdFiduMark.dY = sin(rgdFiduMark.dT) * m_rgAlignMarkOffset.dX 
			             + cos(rgdFiduMark.dT) * m_rgAlignMarkOffset.dY; 

		// T축 보정값
		rgdFiduMark.dT = m_rgAlignMarkOffset.dT;
	}
	else
	{
		rgdFiduMark.Init(0.0,0.0,0.0);
	}

	return rgdFiduMark;
}

void MTabCarrier::SetAlignMarkOffset(MPos_XYT sPos, double dValueThetaAlign)
{
	// Align Mark Offset 설정 
	m_rgAlignMarkOffset.Init(sPos.dX, sPos.dY, sPos.dT);
	m_dValueThetaAlign = dValueThetaAlign;

	m_bMarkAligned = TRUE;
}

MPos_XYT MTabCarrier::GetAlignMarkOffset()
{
	MPos_XYT psPos;

	if (m_bMarkAligned == TRUE)
	{
		// Align Mark OFfset 값 전달
		psPos.Init(m_rgAlignMarkOffset.dX, m_rgAlignMarkOffset.dY, m_rgAlignMarkOffset.dT);
	}
	else
	{
		psPos.Init(0.0, 0.0, 0.0);	// Align Mark Offset 값 0.0 전달 
	}

	return psPos;
}

BOOL MTabCarrier::IsAligned()
{
	return m_bMarkAligned;
}

/**
 * 이동시 인근 Carrier와 충돌여부 확인하는 함수
 **/

void MTabCarrier::SetMovingY(BOOL bMove, MPos_XYT posTarget)
{
	m_posTargetMove = posTarget;
	m_bMoving = bMove;
}

int	MTabCarrier::CheckCollisionOther(int iPosID)
{
	MPos_XYT tgPos = GetTargetXYTPos(iPosID);
	return CheckCollisionOther(tgPos);
}

int MTabCarrier::CheckCollisionOther(MPos_XYT posTarget)
{
	int iResult = ERR_TABCARRIER_SUCCESS;



	m_csMoveLock.Lock();

	double dCurrentPos = GetCurrentXYTPos().dX;
	BOOL bDir = (posTarget.dX > dCurrentPos) ? TRUE /* (+) 방향 */ : FALSE /* (-) 방향 */;

	if (TRUE == bDir)
	{
		if (DEF_NONE_TABCARRIER != m_iPreTabCarrierNo)
		{
			iResult = m_plnkStateTabCarrier[m_iPreTabCarrierNo]->CheckOriginX();
			if (iResult)
			{
				m_csMoveLock.Unlock();
				return iResult;
			}
			/*
			if (!bAuto)
			{
				if (posTarget.dY > m_plnkStateTabCarrier[m_iPreTabCarrierNo]->GetCurrentXYTPos().dY - DEF_TABCARRIER_COLLISION_GAP)
				{
					SetErrorLevel(_T("TAB CARRIER check Interlock for XYT"),2, __FILE__, __LINE__);
					return generateErrorCode(217024);
				}
			}
			*/
			double dComparePos = 0.0;
			if (m_plnkStateTabCarrier[m_iPreTabCarrierNo]->IsMovingY())
				dComparePos = m_plnkStateTabCarrier[m_iPreTabCarrierNo]->GetMovingTgPos().dX;
			else
				dComparePos = m_plnkStateTabCarrier[m_iPreTabCarrierNo]->GetCurrentXYTPos().dX;

			///추후 정리 원점 복귀 Offset 다른 경우 정리 필요
			//@확인요______
//@			if (posTarget.dX > dComparePos - (DEF_TABCARRIER_COLLISION_GAP - 1.5))			
			//if (posTarget.dX > dComparePos - (DEF_TABCARRIER_STANDARD_GAP - 1.5))
			if (posTarget.dX > dComparePos - (DEF_TABCARRIER_STANDARD_GAP - 3.0))
			//@____________
			{
				m_posTargetMove.dX = posTarget.dX;
				m_csMoveLock.Unlock();
				return generateErrorCode(217024);
			}
		}
	}
	else
	{
		if (DEF_NONE_TABCARRIER != m_iNextTabCarrierNo)
		{
			iResult = m_plnkStateTabCarrier[m_iNextTabCarrierNo]->CheckOriginX();
			if (iResult)
			{
				m_csMoveLock.Unlock();
				return iResult;
			}
			/*
			if (!bAuto)
			{
				if (posTarget.dY < m_plnkStateTabCarrier[m_iNextTabCarrierNo]->GetCurrentXYTPos().dY + DEF_TABCARRIER_COLLISION_GAP)
				{
					SetErrorLevel(_T("TAB CARRIER check Interlock for XYT"),2, __FILE__, __LINE__);
					return generateErrorCode(217025);
				}
			}
			*/
			double dComparePos = 0.0;
			if (m_plnkStateTabCarrier[m_iNextTabCarrierNo]->IsMovingY())
				dComparePos = m_plnkStateTabCarrier[m_iNextTabCarrierNo]->GetMovingTgPos().dX;
			else
				dComparePos = m_plnkStateTabCarrier[m_iNextTabCarrierNo]->GetCurrentXYTPos().dX;

			//@확인요_______
//@			if (posTarget.dX < dComparePos + (DEF_TABCARRIER_COLLISION_GAP - 1.5))
			//if (posTarget.dX < dComparePos + (DEF_TABCARRIER_STANDARD_GAP - 1.5))
			if (posTarget.dX < dComparePos + (DEF_TABCARRIER_STANDARD_GAP - 4.0))
			
			//@_____________
			{
				m_csMoveLock.Unlock();
				return generateErrorCode(217025);
			}
		}
	}
	m_csMoveLock.Unlock();
	return ERR_TABCARRIER_SUCCESS;
}

BOOL MTabCarrier::IsMovingY()
{
	m_csMoveLock.Lock();
	BOOL bMove = (m_bMoving || m_plnkX->IsInMotion());
	m_csMoveLock.Unlock();
	return bMove;
}

MPos_XYT MTabCarrier::GetMovingTgPos()
{
	return m_posTargetMove;
}

void MTabCarrier::SetTabInfo(MTabInfo tabinfo)
{
	m_TabInfo = tabinfo;
}

MTabInfo MTabCarrier::GetTabInfo()
{
	return m_TabInfo;
}

//5Pitch이하 Short, 5Pitch초과 Long
BOOL MTabCarrier::IsShortPitchTab()
{
	BOOL bShort = FALSE;
//	if (m_plnkPreBonderData->m_uiTab_Pitch_Count_Xtab <= 5)
#ifdef DEF_SOURCE_SYSTEM
	if (m_plnkPreBonderData->m_dLength_between_ChipCenter_and_TabEnd < 31.0)	//101222. 30.0에서 31.0으로 변경
#else
	//150320 SJ_YSH 전종연K 요청으로 변경 (10.5 -> 11)
	//if (m_plnkPreBonderData->m_dLength_between_ChipCenter_and_TabEnd < 10.5)	//3Pitch
	if (m_plnkPreBonderData->m_dLength_between_ChipCenter_and_TabEnd < 11.0)	//3Pitch
#endif
		bShort = TRUE;
	else
		bShort = FALSE;
	return bShort;
}

//110128.Add
MPos_XYT MTabCarrier::GetPrealignReadyPos()
{
	MPos_XYT posPrealignReady;
	posPrealignReady.dX = GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS).dX + (DEF_TABCARRIER_STANDARD_GAP + 0.5);
	posPrealignReady.dY = GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dY;
	posPrealignReady.dT = GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dT;
	return posPrealignReady;
}

MPos_XYT MTabCarrier::GetUnloadingReadyPos()
{
	MPos_XYT posUnloadingReady;
	posUnloadingReady = GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS);
	return posUnloadingReady;
}

int MTabCarrier::Wait4DoneXYT()
{
	int iResult = ERR_TABCARRIER_SUCCESS;

	iResult = m_plnkT->Wait4Done(TRUE);
	if (iResult)
		return iResult;
	iResult = m_plnkY->Wait4Done(TRUE);
	if (iResult)
		return iResult;
	iResult = m_plnkX->Wait4Done();
	if (iResult)
		return iResult;

	return ERR_TABCARRIER_SUCCESS;
}
//SJ_YYK 150212 Add..
int MTabCarrier::StartSplineMove(int iMaxPoint, double *pdPosition, double *pdPosition2, double *pdVelocity, int *piAccelerate,
								BOOL bAutoFlag, BOOL bWaitOpt)
{
	int iResult = ERR_TABCARRIER_SUCCESS;

	iResult = m_plnkXZAxis->MoveSplineLine2(iMaxPoint, pdPosition, pdPosition2, pdVelocity, piAccelerate, bAutoFlag, bWaitOpt);
	if(iResult)
		return iResult;
	return ERR_TABCARRIER_SUCCESS;
}
//SJ_YYK 150223 Add..
int MTabCarrier::MoveYTPos(int iPosID, BOOL bWaitOpt, BOOL bAuto)
{
	int iResult = ERR_TABCARRIER_SUCCESS;

	MPos_XYT posTarget = GetTargetXYTPos(iPosID);

	iResult = m_plnkY->StartMove(posTarget.dY);
	if(iResult)
		return iResult;
	iResult = m_plnkT->StartMove(posTarget.dT);
	if(iResult)
		return iResult;

	if(bWaitOpt == TRUE)
	{
		iResult = m_plnkY->Wait4Done();
		if(iResult)
			return iResult;

		iResult = m_plnkT->Wait4Done();
		if(iResult)
			return iResult;
	}


	return ERR_TABCARRIER_SUCCESS;
}