/* 
 * Control TabCarrier Component
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

#include "StdAfx.h"
#include "MSystemData.h"
#include "MPreBonderData.h"
#include "MTabOffsetData.h"
//#include "IStateTabFeeder.h"
#include "MTabFeeder.h"
#include "MTabCarrier.h"
#include "MCtrlTabCarrier.h"
#include "MVision.h"
#include "MPos_XY.h"
#include "DefVision.h"
#include "MVisionCalibration.h"
#include "MPreBonderData.h"
#include "common.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"
//101124 sj_ysb
#include "IIO.h"
#include "DefIOAddrPreBonder.h"
#include "MTickTimer.h"
#include "DBTabLoss.h"
#include "IAxis.h"
#include "MTickTimer.h"


#include <math.h>

extern MPlatformOlbSystem	MOlbSystemPre;

/**
 * @stereotype constructor
 *
 * @param	commonData			: ���� Component Data
 * @param	listRefComponent	: CtrlTabCarrier Component���� ������ Component Pointer
 * @param	datComponent		: CtrlTabCarrier Component Data
 */
MCtrlTabCarrier::MCtrlTabCarrier(SCommonAttribute commonData, SCtrlTabCarrierRefCompList listRefComponent, SCtrlTabCarrierData datComponent)
: ICommon(commonData)
{
	SetData(datComponent);
	AssignComponents(listRefComponent);

	if (m_iInstanceNo < DEF_MAX_TABCARRIER)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = m_iInstanceNo-DEF_MAX_TABCARRIER;

	}

	//170906 JSH.s
	if(m_iWorkGroup == DEF_FRONT_GROUP)
	{
		if(m_iWorkInstance%2 == 0)
		{
			m_iAOCUnitNo = DEF_AOC_UNIT_NO_1;
		}
		else
		{
			m_iAOCUnitNo = DEF_AOC_UNIT_NO_2;
		}
	}
	else
	{
		if(m_iWorkInstance%2 == 0)
		{
			m_iAOCUnitNo = DEF_AOC_UNIT_NO_3;
		}
		else
		{
			m_iAOCUnitNo = DEF_AOC_UNIT_NO_4;
		}
	}
	//170906 JSH.e
}

MCtrlTabCarrier::~MCtrlTabCarrier()
{
}

/**
 * CtrlTabCarrier Component�� �ʱ�ȭ �Ѵ�.
 *
 * @return	int (0 = Success, Error Code = �� ��)
 */
int MCtrlTabCarrier::Initialize(void)
{
	return ERR_CTRL_TABCARRIER_SUCCESS;
}

/**
* MCtrlTabCarrier�� Data Parameter�� �����Ѵ�. 
* @param	datcomponent : ������ MCtrlTabCarrier�� Parameter
*/
int	MCtrlTabCarrier::SetData(SCtrlTabCarrierData datcomponent)
{
	m_plnkSystemData = datcomponent.m_plnkSystemData;
	m_plnkPreBonderData = datcomponent.m_plnkPreBonderData;
	m_plnkTabOffsetData = datcomponent.m_plnkTabOffsetData;
	m_plnkTabOffsetData_Rear = datcomponent.m_plnkTabOffsetData_Rear;
	m_iTabAlignCam1  = datcomponent.m_iTabAlignCam1;
	m_iTabAlignCam2  = datcomponent.m_iTabAlignCam2;
	m_plnkDBTabLoss		= datcomponent.m_plnkDBTabLoss;
	return ERR_CTRL_TABCARRIER_SUCCESS;
}

/**
* MCtrlTabCarrier�� Component List�� �����Ѵ�.
* @param	listRefComponents : ������ MCtrlTabCarrier�� Component List
*/
int	MCtrlTabCarrier::AssignComponents(SCtrlTabCarrierRefCompList listRefComponents)
{	
	m_plnkVision		= listRefComponents.m_plnkVision;
	m_plnkTabCarrier	= listRefComponents.m_plnkTabCarrier;
	m_plnkTabFeeder		= listRefComponents.m_plnkTabFeeder;
	m_plnkCtrlVisionCalibration = listRefComponents.m_plnkCtrlVisionCalibration;
	m_plnkMelsecAOCIF	= listRefComponents.m_plnkMelsecAOCIF; //SJ_YYK 150908 Add...
	return ERR_CTRL_TABCARRIER_SUCCESS;
}

//int MCtrlTabCarrier::WaitTabIC(int iVacNo)
int MCtrlTabCarrier::WaitTabIC()
{
	if (NORMAL_RUN_MODE != m_plnkSystemData->m_eRunMode)
		return ERR_CTRL_TABCARRIER_SUCCESS;

	if (TRUE == m_plnkTabCarrier->IsAbsorbTabIC())
		return ERR_CTRL_TABCARRIER_SUCCESS;
	
	int iResult = ERR_CTRL_TABCARRIER_SUCCESS;
	
	if (FALSE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
	{
		iResult = m_plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
			return iResult;
		
#ifndef SIMULATION
		if (!m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
		{
			Sleep(500);
			if (!m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
			{
				SetErrorLevel(_T("TABCARRIER wait Tab IC"),1, __FILE__, __LINE__);
				// 317001 = Tab Carrier �� Load Pos �� ���� ����.		
				return generateErrorCode(317001);
			}
		}
#endif
	}
	
	iResult = m_plnkTabCarrier->Up(TRUE);
	if (iResult)
		return iResult;

	return iResult;
}

//int MCtrlTabCarrier::LoadTabIC(int iVacNo)
int MCtrlTabCarrier::LoadTabIC()
//int MCtrlTabCarrier::LoadTabIC(BOOL bFirst, BOOL bSecond)
{
	if (TRUE == m_plnkTabCarrier->IsAbsorbTabIC())
		return ERR_CTRL_TABCARRIER_SUCCESS;

	int iResult = ERR_CTRL_TABCARRIER_SUCCESS;

	if (FALSE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
	{
		if (TRUE == m_plnkTabCarrier->IsShortPitchTab())
		{
			iResult = m_plnkTabCarrier->Forward();
			if (iResult)
				return iResult;
		}
		else
		{
			iResult = m_plnkTabCarrier->Backward();
			if (iResult)
				return iResult;
		}

		iResult = m_plnkTabCarrier->Down();
		if (iResult)
			return iResult;
		
		iResult = m_plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
		if (iResult)
			return iResult;
		
#ifndef SIMULATION
		if (!m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
		{
			Sleep(100);
			if (!m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
			{
				SetErrorLevel(_T("TABCARRIER load Tab IC"),1, __FILE__, __LINE__);
				// 317001 = Tab Carrier �� Load Pos �� ���� ����.		
				return generateErrorCode(317001);
			}
		}
#endif
	}
	//_____________________*/

	if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
	{	
		iResult = m_plnkTabCarrier->AbsorbTabIC(TRUE);
		if (iResult) 
			return iResult;
	}
	//SJ_YYK 150817 Modify..


	//SJ_YYK 150817 Modify..
	//SJ_YYK 150908 Add...
	m_plnkMelsecAOCIF->SetUpperEqBit(eTurnSafetyAOC1, FALSE, m_iAOCUnitNo, TRUE);
	if(m_iWorkInstance%2 == 0)
		iResult = m_plnkTabCarrier->Up();
	else
		iResult = m_plnkTabCarrier->Up2();
	if (iResult)
	{
		m_plnkTabCarrier->Down();
		return iResult;
	}

	if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
	{	
		// ���� : TabFeeder �� Vacuum �� TabCarrier �� �����Ѵ�.
		
		
		iResult = m_plnkTabCarrier->AbsorbTabIC();
		if (iResult)
		{
			// ���� : Carrier �� TabIC ���� ���ص�, TabIC �� ���������� �ʴ´�.
			//        - Carrier Tip �� TabIC �� Press ������ �о� �ø��� �ִ�.
			//SJ_YYK 110415 Test ����...
			/*
			int iResult2 = m_plnkTabFeeder->AbsorbPress();
			if (iResult2)
			{
			m_plnkTabFeeder->ReleasePress();
			m_plnkTabCarrier->Down();
			SetErrorLevel(_T("TABCARRIER load Tab IC"),1, __FILE__, __LINE__);
			return iResult;	// ���� Error Code �� Return �Ѵ�.
			}
			
			  m_plnkTabCarrier->ReleaseTabIC();
			  
				iResult2 = m_plnkTabCarrier->Down();
				if (iResult2)
				{
				SetErrorLevel(_T("TABCARRIER load Tab IC"),1, __FILE__, __LINE__);
				return iResult;	// ���� Error Code �� Return �Ѵ�.
				}
				
				  Sleep(100);
				  
					iResult2 = m_plnkTabCarrier->Up();
					if (iResult2)
					{
					m_plnkTabCarrier->Down();
					SetErrorLevel(_T("TABCARRIER load Tab IC"),1, __FILE__, __LINE__);
					return iResult;	// ���� Error Code �� Return �Ѵ�.
					}
					
					  iResult2 = m_plnkTabCarrier->AbsorbTabIC();
					  if (iResult2)
			*/
			if (iResult)
			{
				m_plnkTabCarrier->ReleaseTabIC();
				//SJ_YYK 150817 Modify..
				if(m_iWorkInstance%2 == 0)
					m_plnkTabCarrier->Down();
				else
					m_plnkTabCarrier->Down2();
				//SJ_YYK 150908 Add...
				m_plnkMelsecAOCIF->SetUpperEqBit(eTurnSafetyAOC1, TRUE, m_iAOCUnitNo, TRUE);


				//101124 sj_ysb Ÿ�� �� Carrier ���� Error�� ���� Blow ��� �߰�....
				
				if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
				{
					E_TABVAC_ERR_FIELD eField;
					switch (m_iWorkInstance)
					{
					case 0:
						eField = eCA1;
						break;
					case 1:
						eField = eCA2;
						break;
					case 2:
						eField = eCA3;
						break;
					case 3:
						eField = eCA4;
						break;
					}
					m_plnkDBTabLoss->AppendFieldCount(eField);
				}
				
				return iResult;	// ���� Error Code �� Return �Ѵ�.
			}
		}

	}
	else
	{
		Sleep(200);
	}
	// ���� ���� �ð� ������ ���� Skip Sensor ���.
	/*iResult = m_plnkTabCarrier->Down(TRUE); 
	if (iResult) 
		return iResult;
	*/
	//TabCarrier���� �ϰ����� �������, TabCarrier��/�� Tension�� ���Ҷ� TabIC��ġ�� ���� �߻�
	//SJ_YYK 150817 Modify..
	//SJ_YYK 150912 Modify,,
	MTickTimer			m_timer;

	m_plnkMelsecAOCIF->SetUpperEqBit(eArm1FoldComplete, TRUE, m_iAOCUnitNo, TRUE);
	m_timer.StartTimer();
	while(TRUE)
	{
		Sleep(10);
		if (m_plnkMelsecAOCIF->IsOnUpperEqBit(eGlassCheckSensorOn, m_iAOCUnitNo, TRUE)) 
			break;
		
		if (FALSE == m_plnkMelsecAOCIF->IsOnUpperEqBit(eSendAble, m_iAOCUnitNo, TRUE)) {					
			return generateErrorCode(317029);
		}
		if (m_timer.MoreThan(30.0)) {
			return generateErrorCode(317029);
		}
	}
//	Sleep(100);
	Sleep(50);


	//SJ_YYK 150817 Modify..
	if(m_iWorkInstance %2 == 0)
		iResult = m_plnkTabCarrier->Down();
	else
		iResult = m_plnkTabCarrier->Down2();
	if (iResult) 
		return iResult;
	else
	{
		//SJ_YYK 150908 Add...
		m_plnkMelsecAOCIF->SetUpperEqBit(eTurnSafetyAOC1, TRUE, m_iAOCUnitNo, TRUE);
	}
	
	if (DRY_RUN_MODE != m_plnkSystemData->m_eRunMode)
	{	
		if (!m_plnkTabCarrier->IsAbsorbTabIC())
		{
			if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() != MANUAL_MODE)
			{
				E_TABVAC_ERR_FIELD eField;
				switch (m_iWorkInstance)
				{
				case 0:
					eField = eCA1D;
					break;
				case 1:
					eField = eCA2D;
					break;
				case 2:
					eField = eCA3D;
					break;
				case 3:
					eField = eCA4D;
					break;
				}
				m_plnkDBTabLoss->AppendFieldCount(eField);
			}
			m_plnkTabCarrier->ReleaseTabIC();
			return generateErrorCode(317002);
		}
	}
	
	//170201 SJ_YSH
	/*/
	double dTargetPreAlign_T;
	dTargetPreAlign_T = m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS).dT;
	if(MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
	{
		if(m_iWorkInstance == DEF_TABCARRIER2 || m_iWorkInstance == DEF_TABCARRIER4)
		{
			iResult = m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_T_AXIS)->Move(dTargetPreAlign_T);
			if (iResult)
			{
				SetErrorLevel(_T("TABCARRIER move Load Position"),1, __FILE__, __LINE__);
				return iResult;
			}
		}
	}
	/*/
	//_____________
	return iResult;
}

int MCtrlTabCarrier::MoveReadyPos()
{
	/*
	int iResult = m_plnkTabCarrier->Down();
	if (iResult)
	{
		SetErrorLevel(_T("TABCARRIER move Load Position"),1, __FILE__, __LINE__);
		return iResult;
	}*/

	int iResult = 0;
	if (TRUE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_READY_POS))
		return ERR_CTRL_TABCARRIER_SUCCESS;

	iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_READY_POS);
	if (iResult)
	{
		DWORD stime = GetTickCount();
		while(1)
		{
			Sleep(10);
			iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_READY_POS);
			if (iResult == ERR_CTRL_TABCARRIER_SUCCESS || (GetTickCount() - stime > DEF_COLLISION_WAIT_TIME))
				break;
		}
	}

	iResult = m_plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_READY_POS);
	if (iResult)
		return iResult;

	return ERR_CTRL_TABCARRIER_SUCCESS;
}

//int MCtrlTabCarrier::MoveLoadingPos(int iLoadPosNum)
int MCtrlTabCarrier::MoveLoadingPos()
{
	/*
	int iResult = m_plnkTabCarrier->Down();
	if (iResult)
	{
		SetErrorLevel(_T("TABCARRIER move Load Position"),1, __FILE__, __LINE__);
		return iResult;
	}*/

	int iResult = 0;
	if (TRUE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_LOAD_POS))
		return ERR_CTRL_TABCARRIER_SUCCESS;

//	MPos_XY posTarget = m_plnkTabCarrier->GetTargetXYPos(iLoadPosNum);
//	iResult = m_plnkTabCarrier->MoveXYPos(posTarget, iLoadPosNum);
	iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_LOAD_POS);
	if (iResult)
	{
		DWORD stime = GetTickCount();
		while(1)
		{
			Sleep(10);
			iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_LOAD_POS);
			if (iResult == ERR_CTRL_TABCARRIER_SUCCESS || (GetTickCount() - stime > DEF_COLLISION_WAIT_TIME))
				break;
		}
	}

	iResult = m_plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_LOAD_POS);
	if (iResult)
		return iResult;

	return ERR_CTRL_TABCARRIER_SUCCESS;
}

int MCtrlTabCarrier::MovePreAlignPos()
{
	/*
	int iResult = m_plnkTabCarrier->Down();
	if (iResult)
	{
		SetErrorLevel(_T("TABCARRIER move Load Position"),1, __FILE__, __LINE__);
		return iResult;
	}*/

	int iResult = 0;
	if (TRUE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_PREALIGN_POS))
		return ERR_CTRL_TABCARRIER_SUCCESS;

	iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_PREALIGN_POS);
	if (iResult)
	{
		DWORD stime = GetTickCount();
		while(1)
		{
			Sleep(10);
			iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_PREALIGN_POS);
			if (iResult == ERR_CTRL_TABCARRIER_SUCCESS || (GetTickCount() - stime > DEF_COLLISION_WAIT_TIME))
				break;
		}
	}

	iResult = m_plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_PREALIGN_POS);
	if (iResult)
		return iResult;
	Sleep(50);
	return ERR_CTRL_TABCARRIER_SUCCESS;
}

int MCtrlTabCarrier::MoveUnloadingPos(int nTabNo)
{
	/*
	int iResult = m_plnkTabCarrier->Down();
	if (iResult)
	{
		SetErrorLevel(_T("TABCARRIER move Unload Position"),1, __FILE__, __LINE__);
		return iResult;
	}*/

	int iResult = 0;
	MPos_XYT tgPos = m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS);
	BOOL bMoveOffset = FALSE;
	if (nTabNo >= 0 && nTabNo < DEF_MAX_TAB_XU)
	{
		double dDir = 1.0;
		if (m_iWorkGroup == DEF_FRONT_GROUP)
		{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
			tgPos.dY += m_plnkTabOffsetData->m_rgdYlTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdYlTabOffsetTabAlignerT[nTabNo];
	#else
			tgPos.dY += m_plnkTabOffsetData->m_rgdYrTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdYrTabOffsetTabAlignerT[nTabNo];
	#endif
#else
//@			tgPos.dY += m_plnkTabOffsetData->m_rgdXuTabOffsetTabAligner[nTabNo];
//@			tgPos.dT += m_plnkTabOffsetData->m_rgdXuTabOffsetTabAlignerT[nTabNo];

			tgPos.dY += m_plnkTabOffsetData->m_rgdXuTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdXuTabOffsetTabAlignerT[nTabNo];

#endif		
		}
		else
		{
#ifdef DEF_GATE_SYSTEM
	#ifndef DEF_MIRROR_
			tgPos.dY += m_plnkTabOffsetData->m_rgdYrTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdYrTabOffsetTabAlignerT[nTabNo];
	#else
			tgPos.dY += m_plnkTabOffsetData->m_rgdYlTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdYlTabOffsetTabAlignerT[nTabNo];
	#endif
#else
//@			tgPos.dY += m_plnkTabOffsetData->m_rgdXdTabOffsetTabAligner[nTabNo];
//@			tgPos.dT += m_plnkTabOffsetData->m_rgdXdTabOffsetTabAlignerT[nTabNo];
	//SJ_YYK 150907 Modify...
			tgPos.dY += m_plnkTabOffsetData->m_rgdXdTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdXdTabOffsetTabAlignerT[nTabNo];
#endif
		}
//@#endif
		bMoveOffset = TRUE;
	}

	if (FALSE == bMoveOffset)
	{
		//if (TRUE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_UNLOAD_POS))
		if (TRUE == m_plnkTabCarrier->IsInXYTPos(tgPos))
			return ERR_CTRL_TABCARRIER_SUCCESS;

		//iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_UNLOAD_POS);
		iResult = m_plnkTabCarrier->CheckCollisionOther(tgPos);
		if (iResult)
		{
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				//iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_UNLOAD_POS);
				iResult = m_plnkTabCarrier->CheckCollisionOther(tgPos);
				if (iResult == ERR_CTRL_TABCARRIER_SUCCESS || (GetTickCount() - stime > DEF_COLLISION_WAIT_TIME))
					break;
			}
		}
	}

//	iResult = m_plnkTabCarrier->SafeMoveXYTPos(DEF_TABCARRIER_UNLOAD_POS);
	iResult = m_plnkTabCarrier->MoveXYTPos(tgPos, DEF_TABCARRIER_UNLOAD_POS);
	if (iResult)
		return iResult;
	return ERR_CTRL_TABCARRIER_SUCCESS;
}

int MCtrlTabCarrier::MoveUnloading2Pos(int nTabNo)
{
	int iResult = 0;
//	MPos_XYT tgPos = m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_R_UNLOAD_POS);
	MPos_XYT tgPos = m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_UNLOAD_POS);
	BOOL bMoveOffset = FALSE;
	if (nTabNo >= 0 && nTabNo < DEF_MAX_TAB_XU)
	{
		double dDir = 1.0;
#ifdef DEF_SOURCE_SYSTEM
#	ifndef DEF_MIRROR_
		if (DEF_FRONT_GROUP == m_iWorkGroup)
			dDir = 1.0;
		else
			dDir = -1.0;
#	else
		if (DEF_FRONT_GROUP == m_iWorkGroup)
			dDir = -1.0;
		else
			dDir = 1.0;
#	endif
		if (m_iWorkGroup == DEF_FRONT_GROUP)
		{
			tgPos.dX += m_plnkTabOffsetData->m_rgdXuTabOffsetTabAligner[nTabNo];
			tgPos.dT += (m_plnkTabOffsetData->m_rgdXuTabOffsetTabAlignerT[nTabNo]*dDir);
		}
		else
		{
			tgPos.dX += m_plnkTabOffsetData_Rear->m_rgdXuTabOffsetTabAligner[nTabNo];
			tgPos.dT += (m_plnkTabOffsetData_Rear->m_rgdXuTabOffsetTabAlignerT[nTabNo]*dDir);
		}
#else
		if (m_iWorkGroup == DEF_FRONT_GROUP)
		{
#	ifndef DEF_MIRROR_
			tgPos.dY += m_plnkTabOffsetData->m_rgdYlTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdYlTabOffsetTabAlignerT[nTabNo];
#	else
			tgPos.dY += m_plnkTabOffsetData->m_rgdYrTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdYrTabOffsetTabAlignerT[nTabNo];
#	endif
		}
		else
		{
#	ifndef DEF_MIRROR_
			tgPos.dY += m_plnkTabOffsetData->m_rgdYrTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdYrTabOffsetTabAlignerT[nTabNo];
#	else
			tgPos.dY += m_plnkTabOffsetData->m_rgdYlTabOffsetTabAligner[nTabNo];
			tgPos.dT += m_plnkTabOffsetData->m_rgdYlTabOffsetTabAlignerT[nTabNo];
#	endif
		}
#endif
		bMoveOffset = TRUE;
	}

	if (FALSE == bMoveOffset)
	{
		//if (TRUE == m_plnkTabCarrier->IsInXYTPos(DEF_TABCARRIER_UNLOAD_POS))
		if (TRUE == m_plnkTabCarrier->IsInXYTPos(tgPos))
			return ERR_CTRL_TABCARRIER_SUCCESS;

		//iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_UNLOAD_POS);
		iResult = m_plnkTabCarrier->CheckCollisionOther(tgPos);
		if (iResult)
		{
			DWORD stime = GetTickCount();
			while(1)
			{
				Sleep(10);
				//iResult = m_plnkTabCarrier->CheckCollisionOther(DEF_TABCARRIER_UNLOAD_POS);
				iResult = m_plnkTabCarrier->CheckCollisionOther(tgPos);
				if (iResult == ERR_CTRL_TABCARRIER_SUCCESS || (GetTickCount() - stime > DEF_COLLISION_WAIT_TIME))
					break;
			}
		}
	}
//	iResult = m_plnkTabCarrier->MoveXYTPos(tgPos, DEF_TABCARRIER_R_UNLOAD_POS);
	iResult = m_plnkTabCarrier->MoveXYTPos(tgPos, DEF_TABCARRIER_UNLOAD_POS);
	if (iResult)
		return iResult;
	return ERR_CTRL_TABCARRIER_SUCCESS;
}

/**
 * Align�� ���� Fidu-Mark�� Position ã��
 *
 * @param	piFiduTabMark1Pos : Carrier �̵��� ù��° Mark �ν� Position Index
 * @param	piFiduTabMark2Pos : Carrier �̵��� �ι�° Mark �ν� Position Index
 * @return	MPos_XYT ()
 */
void MCtrlTabCarrier::getFiduTabMarkPosition(MPos_XYT &TabMarkPosition1, 
									 MPos_XYT &TabMarkPosition2, 
									 int& piFiduTabMark1Pos, 
									 int& piFiduTabMark2Pos,
									 int& piTabCamPos)
{
	double		dTabMarkDistance = 0;
	if(m_plnkTabCarrier->IsShrinkCamera())
		piTabCamPos			= DEF_TABCAMERA_SHRINK_POS_ID;   ///0
	else
		piTabCamPos			= DEF_TABCAMERA_EXPAND_POS_ID;   ///1

	/*
	if(m_plnkTabData->m_bUseXuTab)
		dTabMarkDistance = m_plnkTabData->m_dXuTabPitch;
	else if(m_plnkTabData->m_bUseXdTab)
		dTabMarkDistance = m_plnkTabData->m_dXdTabPitch;
	else if(m_plnkTabData->m_bUseYlTab)
		dTabMarkDistance = m_plnkTabData->m_dYlTabPitch;
	else
		dTabMarkDistance = m_plnkTabData->m_dYrTabPitch;
	*/

	ASSERT(m_iWorkInstance >= 0 && m_iWorkInstance < DEF_MAX_TABCARRIER);

	if (piTabCamPos == DEF_TABCAMERA_EXPAND_POS_ID)
	{
		dTabMarkDistance = m_plnkSystemData->m_rgdPreAlignCamDistViewCenter_Expand[m_iWorkGroup];
	}
	else
	{
		dTabMarkDistance = m_plnkSystemData->m_rgdPreAlignCamDistViewCenter_Shrink[m_iWorkGroup];
	}

	///�ӽ� �Ÿ�
//	dTabMarkDistance = 39.138;
#ifdef DEF_SOURCE_SYSTEM
	dTabMarkDistance = m_plnkPreBonderData->m_dLead_Width_Xtab;
#else
	dTabMarkDistance = m_plnkPreBonderData->m_dLead_Width_Ytab;
#endif

	TabMarkPosition1.dX	= - dTabMarkDistance / 2.0;
	TabMarkPosition1.dY = MOlbSystemPre.GetDistance_TabAlignerCenterToTabMarkX();
	TabMarkPosition1.dT	=  0;

	TabMarkPosition2 = TabMarkPosition1;
	TabMarkPosition2.dX += dTabMarkDistance;
	// �ν��ϴ� ��ġ�� ���� ��ġ�̴�.
	piFiduTabMark1Pos = DEF_TABCARRIER_PREALIGN_POS;  ///�ɸ��� �� ����� ��ġ 
	piFiduTabMark2Pos = DEF_TABCARRIER_PREALIGN_POS;  ///�ɸ��� �� ����� ��ġ 
}

/**
 * Camera 1��� Mark �ν� �õ�
 *
 * @param	iCameraNo : Camera ��ȣ
 * @param	iMarkNo : �ν��� Mark ��ȣ
 * @param	iPosIndex : Potision Index ��ȣ
 * @param	pResult : �ν� ���
 * @return	int (0 = Success, Error Code = �� ��)
 */
int	MCtrlTabCarrier::getTabMarkRecogPosition(
	int			iCameraNo,
	int			iMarkNo,
	int			iPosIndex,
	MPos_XYT*	pResult)
{
	int			iResult;

	/** Panel Mark �ν� 1�� �õ� */
	iResult = m_plnkVision->RecognitionPatternMark(iCameraNo, iMarkNo);
	if (iResult)		/** �ν� �����̸� */
	{
		if (iResult)
			return generateErrorCode(317021);	// Vision �ν� Score�� �����ϴ�.
	}
	
	m_plnkCtrlVisionCalibration->ConvertV2R(iCameraNo,
										0,
										//m_iWorkInstance,
										m_plnkVision->GetSearchResultX(iCameraNo, iMarkNo),
										m_plnkVision->GetSearchResultY(iCameraNo, iMarkNo),
										pResult->dX,
										pResult->dY);
	pResult->dT	= 0;	
	
	return ERR_CTRL_TABCARRIER_SUCCESS;
}

//20121004 SJ_HJG Ⱦ����
//20120826 sj_ysb
/**
 * Camera 1��� Mark �ν� �õ�
 *
 * @param	iCameraNo : Camera ��ȣ
 * @param	iMarkNo : �ν��� Mark ��ȣ
 * @param	iPosIndex : Potision Index ��ȣ
 * @param	pResult : �ν� ���
 * @return	int (0 = Success, Error Code = �� ��)
 */
int	MCtrlTabCarrier::getTabMarkRecogBlobPosition(
	int			iCameraNo,
	int			iMarkNo,
	int			iPosIndex,
	double		dRefVX,
	double		dRefVY,
	MPos_XYT*	pResult)
{
	int			iResult;
//	double		dResultY;
	
	/** Panel Mark �ν� 1�� �õ� */
	iResult = m_plnkVision->RecognitionEdgeLineY(iCameraNo, iMarkNo, FALSE, dRefVX, dRefVY);
	if (iResult)		/** �ν� �����̸� */
	{
		if (iResult)
			return generateErrorCode(317021);	// Vision �ν� Score�� �����ϴ�.
	}
	
	m_plnkCtrlVisionCalibration->ConvertV2R(iCameraNo,
										0,
										//m_iWorkInstance,
										m_plnkVision->GetSearchBlobResultX(iCameraNo, iMarkNo),
										m_plnkVision->GetSearchBlobResultY(iCameraNo, iMarkNo),
										pResult->dX,
										pResult->dY);
	pResult->dT	= 0;	

	char szTxt[128];
	memset(szTxt, 0, sizeof(szTxt));
	sprintf(szTxt, "X:%.3lf, Y:%.3lf", pResult->dX, pResult->dY);
	m_plnkVision->DrawOverlayText(iCameraNo, szTxt);
	
	return ERR_CTRL_TABCARRIER_SUCCESS;
}
//__________________*/
/**
 * TabIC Align ����
 *
 * @return	pRetAlignOffset : (OPTION=NULL) ���� Align Mark Offset
 * @return	int (0 = Success, Error Code = �� ��)
 */
int	MCtrlTabCarrier::AlignTabIC(MPos_XYT* pRetAlignOffset)
{

	MPos_XYT		MarkPosition1;			//	ù��° ��ũ Carrier �߽ɿ��������� ��ǥ
	MPos_XYT		MarkPosition2;			//	�ι�° ��ũ Carrier �߽ɿ��������� ��ǥ
	MPos_XYT		MarkOffset1;			//	Vision���� �ν��� ù��° ��ũ �ν� Offset
	MPos_XYT		MarkOffset2;			//	Vision���� �ν��� �ι�° ��ũ �ν� Offset
	MPos_XYT		ActualAlignPos;			//	���� Align Motion ��ǥ
	MPos_XYT		AlignOffsetL;
	MPos_XYT		AlignOffsetR;
	MPos_XYT		AlignOffset;			//	Align Offset;
	MPos_XYT		PriorAlignOffset;		//  Carrier�� ������ ���� AlignOffset;

	//20121004 SJ_HJG Ⱦ���� 
	//20120826 sj_ysb
	MPos_XYT		MarkSRLineOffset1;			// 	Vision���� �ν��� ù��° ��ũ �ν� SRLine Offset
	MPos_XYT		MarkSRLineOffset2;			//	Vision���� �ν��� �ι�° ��ũ �ν� SRLine Offset
	MPos_XYT		MarkTabEndOffset1;			// 	Vision���� �ν��� ù��° ��ũ �ν� TabEnd Offset
	MPos_XYT		MarkTabEndOffset2;			//	Vision���� �ν��� �ι�° ��ũ �ν� TabEnd Offset

	//int				riTabMarkNo[2];			//  Align ��ũ ��ȣ	(2��)
	int				riTabMarkNo[6];			//  Align ��ũ ��ȣ	(2��)
	int				iTabMarkPosIndex;		//	ù��° �ν��� FiduMark�� ��ǥ Index
	int				iCamCarrierAlignPos;
	int				iResult;
	int				iFiduTabMark1Pos, iFiduTabMark2Pos;

	CString strLog1, strLog2;

	pRetAlignOffset->Init(0.0, 0.0, 0.0);


	// Stage���� Offset ��ǥ�� �о�´�. (Align ��ġ�� Offset ���� �ݿ��ϱ� ���ؼ� �ʿ���)
	MPos_XYT sCarrierXYTOffsetPos[DEF_TABCARRIER_MAX_POSITION];	
	m_plnkTabCarrier->GetXYTPosParameter(NULL, sCarrierXYTOffsetPos, NULL);

#ifdef SIMULATION
	Sleep(500);
	return ERR_CTRL_TABCARRIER_SUCCESS;
#endif

	// Dry_Run ��尡 �ƴҰ�� Panel�� ���� ���� �� ���� ���� üũ
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		// Panel ���� �ȵ� ���
		if (m_plnkTabCarrier->IsAbsorbTabIC() == FALSE)
			return generateErrorCode(317020); // TAB Align�� TAB ���� ���� �߻� 	
	}

	//SJ_YYK 150817 Modify..
	if(m_iWorkInstance == 0 || m_iWorkInstance == 1)
	{
		iResult = m_plnkTabCarrier->ExpandShrinkForPreAlign();
		if (iResult)
			return iResult;
	}

#ifdef DEF_SOURCE_SYSTEM
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
		//SJ_YYK 150907 Modify...
#	ifndef DEF_MIRROR_
		riTabMarkNo[0] = DEF_TAB_LEAD_XD_MARK1;
		riTabMarkNo[1] = DEF_TAB_LEAD_XD_MARK2;
#	else
		riTabMarkNo[0] = DEF_TAB_LEAD_XU_MARK1;
		riTabMarkNo[1] = DEF_TAB_LEAD_XU_MARK2;
#	endif
	}
	else
	{
		//SJ_YYK 150907 Modify...
#	ifndef DEF_MIRROR_
		riTabMarkNo[0] = DEF_TAB_LEAD_XU_MARK1;
		riTabMarkNo[1] = DEF_TAB_LEAD_XU_MARK2;
#	else
		riTabMarkNo[0] = DEF_TAB_LEAD_XD_MARK1;
		riTabMarkNo[1] = DEF_TAB_LEAD_XD_MARK2;		
#	endif
	}
#else
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
#	ifdef DEF_MIRROR_
		riTabMarkNo[0] = DEF_TAB_LEAD_YR_MARK1;
		riTabMarkNo[1] = DEF_TAB_LEAD_YR_MARK2;
#	else
		riTabMarkNo[0] = DEF_TAB_LEAD_YL_MARK1;
		riTabMarkNo[1] = DEF_TAB_LEAD_YL_MARK2;
#	endif
	}
	else
	{
#	ifdef DEF_MIRROR_
		riTabMarkNo[0] = DEF_TAB_LEAD_YL_MARK1;
		riTabMarkNo[1] = DEF_TAB_LEAD_YL_MARK2;
#	else
		riTabMarkNo[0] = DEF_TAB_LEAD_YR_MARK1;
		riTabMarkNo[1] = DEF_TAB_LEAD_YR_MARK2;
#	endif
	}
#endif
//20121004 SJ_HJG Ⱦ����
	riTabMarkNo[2] = DEF_BLOB_SR_MARK1;
	riTabMarkNo[3] = DEF_BLOB_SR_MARK2;
	riTabMarkNo[4] = DEF_BLOB_TAB_END_MARK1;
	riTabMarkNo[5] = DEF_BLOB_TAB_END_MARK2;
//______________*/
//____________________________________________________________
//
// Step 1. 1st Mark �ν� ��ġ�� �̵��ϱ� �Ǹ��� Ȯ�� ��� ���ǿ� ����.
//____________________________________________________________
	// Move Camera Align Position 
	/* ///�ӽ� 
	if (iResult = m_plnkTabCarrier->ExpandCamera())
	{
		if (iResult != ERR_CTRL_TABCARRIER_SUCCESS)
			return iResult;
	}
	*/

	int iAlignRetryCnt = 0;
	DWORD dStartCnt = GetTickCount();
	DWORD dEndCnt = GetTickCount();
	for(iAlignRetryCnt = 0; iAlignRetryCnt<20; iAlignRetryCnt++)
	{
		// �Ǹ��� ��ġ������ ...... Mark�� ��ġ�� ����Ѵ�.
		getFiduTabMarkPosition(MarkPosition1, MarkPosition2, iFiduTabMark1Pos, iFiduTabMark2Pos, iCamCarrierAlignPos);	
		
		///MarkPosition1		= MarkPosition1 - sCarrierXYTOffsetPos[iFiduTabMark1Pos];
		///MarkPosition2		= MarkPosition2 - sCarrierXYTOffsetPos[iFiduTabMark2Pos];
		MarkPosition1		= MarkPosition1 - sCarrierXYTOffsetPos[iFiduTabMark1Pos];
		MarkPosition2		= MarkPosition2 - sCarrierXYTOffsetPos[iFiduTabMark2Pos];
		
		// Mark ��ġ�� �̵�
		// Move 1st Mark �ν� Position
		if (FALSE == m_plnkTabCarrier->IsInXYTPos(iFiduTabMark1Pos))
		{
			iResult = m_plnkTabCarrier->SafeMoveXYTPos(iFiduTabMark1Pos);
			if (iResult != ERR_CTRL_TABCARRIER_SUCCESS)
				return iResult;
		}
		
		Sleep(100); //170926 JSh ǰ�� Ȯ�ο�

		//____________________________________________________________
		//
		// Step 2. Vision �ν� �ϱ�
		//____________________________________________________________
		//20121004 SJ_HJG Ⱦ����
		MPos_XYT iOldMarkOffset1; 
		MPos_XYT iOldMarkOffset2;
		double dSRLineOffsetDis1 = 0.0;
		double dEndOffsetDis1 = 0.0;
		double dSRLineOffsetDis2 = 0.0;
		double dEndOffsetDis2 = 0.0;
		//_______________*/
		double dRefVX = 0.0;
		double dRefVY = 0.0;
		int nFirstCam = 0;
		int nSecondCam = 0;
		
//		dEndCnt = GetTickCount();
//		TRACE(_T("\nJSH TEST iAlignRetryCnt = %d\n Time = %d\n"), iAlignRetryCnt, dEndCnt-dStartCnt);

		if ( m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			//		m_csVisionLock.Lock();
			nFirstCam = m_iTabAlignCam1; 
			m_plnkVision->LockCamera(nFirstCam);
			
			m_plnkVision->ClearOverlay(nFirstCam);
			m_plnkVision->Grab(nFirstCam);
			
			iResult	= getTabMarkRecogPosition(nFirstCam, 
				riTabMarkNo[0], 
				iFiduTabMark1Pos, 
				&MarkOffset1);	//, &iMatchRatio);
			
			//20111020 by kss
			/////////////////////////////////////////////////////////
			if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
			{
				strLog1.Format(_T("InstanceNo(%02d),t1(,%.4f,%.4f,))"),
					m_iInstanceNo, MarkOffset1.dX, MarkOffset1.dY);			
				//m_plnkVision->WriteLog(_T("..\\Log\\Carrier_log.csv"), (LPCTSTR)strLog);
			}
			
			//		m_csVisionLock.Unlock();
			iOldMarkOffset1 = MarkOffset1;		//20121004 SJ_HJG Ⱦ����
			if (iResult != ERR_CTRL_TABCARRIER_SUCCESS)	// �ڵ� �ν� ���н� �������� �ν�
			{
				for (int i=0; i<3; i++)
				{
					Sleep(200);
					m_plnkVision->Grab(nFirstCam);
					iResult	= getTabMarkRecogPosition(nFirstCam, riTabMarkNo[0], iFiduTabMark1Pos, &MarkOffset1);
					if (iResult == SUCCESS)
					{
						iOldMarkOffset1 = MarkOffset1; 	//20121004 SJ_HJG Ⱦ����
						
						break;
					}
				}
				if (iResult)
				{
					Sleep(200);
					m_plnkVision->Grab(nFirstCam);//ErrorImage MainView�� �����ֱ�...
					m_plnkVision->UnlockCamera(nFirstCam);
					CString strInfo;
					strInfo.Format(_T("getTabMarkRecogPosition %d"), riTabMarkNo[0]);
					m_plnkVision->SaveImage(nFirstCam, (LPCTSTR)strInfo);
					return	iResult;
				}
			}
			
			if (m_plnkPreBonderData->m_bSRLineAlignFunc == TRUE)
			{
				dRefVX = m_plnkVision->GetSearchResultX(nFirstCam, riTabMarkNo[0]);
				dRefVY = m_plnkVision->GetSearchResultY(nFirstCam, riTabMarkNo[0]);
				iResult	= getTabMarkRecogBlobPosition(nFirstCam, riTabMarkNo[2], iFiduTabMark1Pos, dRefVX, dRefVY, &MarkSRLineOffset1);
			}
			
			if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
			{
				dRefVX = m_plnkVision->GetSearchResultX(nFirstCam, riTabMarkNo[0]);
				dRefVY = m_plnkVision->GetSearchResultY(nFirstCam, riTabMarkNo[0]);
				iResult	= getTabMarkRecogBlobPosition(nFirstCam, riTabMarkNo[4], iFiduTabMark1Pos, dRefVX, dRefVY, &MarkTabEndOffset1);
			}
			//_________________________
			
			m_plnkVision->UnlockCamera(nFirstCam);
			MarkOffset1	= MarkOffset1 +	MarkPosition1;	// MarkOffset�� Carrier �߽ɱ����� �Ÿ��� ���Ѵ�.
		}
		
		dSRLineOffsetDis1 = MarkSRLineOffset1.dY - iOldMarkOffset1.dY;
		dEndOffsetDis1 = MarkTabEndOffset1.dY - iOldMarkOffset1.dY;
		//@	dSRLineOffsetDis1 = fabs(dSRLineOffsetDis1); 
		//@	dEndOffsetDis1 = fabs(dEndOffsetDis1); 
		
		//20120907 sj_ysb
		if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
		{
			CString strLog;
			CString strLog1;
			
			if (m_plnkPreBonderData->m_bSRLineAlignFunc == TRUE)
			{
				strLog.Format(_T("CamNum : %d MarkOffset1 : %.3f MarkSRLineOffset1 : %.3f SRLineDis1 : %.3f"),
					m_iTabAlignCam1, iOldMarkOffset1.dY, MarkSRLineOffset1.dY, dSRLineOffsetDis1);
				m_plnkVision->WriteLog(_T("..\\Log\\CarrierMarkDistance.csv"), (LPCTSTR)strLog);
			}
			
			if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
			{
				strLog1.Format(_T("CamNum : %d MarkOffset1 : %.3f MarkTabEndOffset1 : %.3f dEndDis1 : %.3f"),
					m_iTabAlignCam1, iOldMarkOffset1.dY, MarkTabEndOffset1.dY, dEndOffsetDis1);
				m_plnkVision->WriteLog(_T("..\\Log\\CarrierMarkDistance.csv"), (LPCTSTR)strLog1);
			}
			
		}
		//________________________________
		
		if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
		{
			char szResult[256];
			memset(szResult, 0, sizeof(szResult));
			sprintf(szResult, "DX=%.3lf", dEndOffsetDis1);
			m_plnkVision->DrawOverlayText(nFirstCam, szResult);
		}
		
		// ù��° Align�ϴ� ��ǥ ����
		ActualAlignPos	= m_plnkTabCarrier->GetCurrentXYTPos();
		
		double dOffX = MarkOffset1.dX;
		
//		dEndCnt = GetTickCount();
//		TRACE(_T("\nJSH TEST iAlignRetryCnt = %d\n Time = %d\n"), iAlignRetryCnt, dEndCnt-dStartCnt);

		//____________________________________________________________
		//
		// Step 3. 2nd Mark �ν� ��ġ�� �̵��ϱ�
		//____________________________________________________________
		// �ι�° Mark ��ġ�� �̵�
		// Move 2nd Mark �ν� Position
		//	iResult = m_plnkStage->SafeMoveXYTPos(iFiduMark2Pos);
		//	if (iResult != ERR_CTRLSTAGE_SUCCESS) return iResult;
		
		//____________________________________________________________
		//
		// Step 4. Vision �ν� �ϱ�
		//____________________________________________________________
		// Dry_Run�� �ƴҰ�� �ι�° ��ũ �ν�
		if ( m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
		{
			//		m_csVisionLock.Lock();
			nSecondCam = m_iTabAlignCam2;
			m_plnkVision->LockCamera(nSecondCam);
			
			m_plnkVision->ClearOverlay(nSecondCam);
			m_plnkVision->Grab(nSecondCam);
			iResult	= getTabMarkRecogPosition(nSecondCam, 
				riTabMarkNo[1], 
				iFiduTabMark2Pos, 
				&MarkOffset2);	//, &iMatchRatio);
			
			if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
			{
				CString strTempLog;
				strLog2.Format(_T("InstanceNo(%02d),t2(,%.4f,%.4f,))"),
					m_iInstanceNo, MarkOffset2.dX, MarkOffset2.dY);
				
				strTempLog.Format(_T("%s, %s"), strLog1, strLog2);
				
				m_plnkVision->WriteLog(_T("..\\Log\\Carrier_log.csv"), (LPCTSTR)strTempLog);
			}
			
			//		m_csVisionLock.Unlock();
			iOldMarkOffset2 = MarkOffset2;	//20121004 SJ_HJG Ⱦ����
			if (iResult != ERR_CTRL_TABCARRIER_SUCCESS)	// �ڵ� �ν� ���н� �������� �ν�
			{
				for (int i=0; i<3; i++)
				{
					Sleep(200);
					m_plnkVision->Grab(nSecondCam);
					iResult	= getTabMarkRecogPosition(nSecondCam, riTabMarkNo[1], iFiduTabMark2Pos, &MarkOffset2);
					if (iResult == SUCCESS)
					{
						iOldMarkOffset2 = MarkOffset2;	//20121004 SJ_HJG Ⱦ����
						
						break;
					}
				}
				if (iResult)
				{
					Sleep(200);
					m_plnkVision->Grab(nSecondCam);//ErrorImage MainView�� �����ֱ�...
					m_plnkVision->UnlockCamera(nSecondCam);
					CString strInfo;
					strInfo.Format(_T("getTabMarkRecogPosition %d"), riTabMarkNo[1]);
					m_plnkVision->SaveImage(nSecondCam, (LPCTSTR)strInfo);
					return	iResult;
				}
			}
			//20121004 SJ_HJG Ⱦ����
			// 20120826 sj_ysb
			if (m_plnkPreBonderData->m_bSRLineAlignFunc == TRUE)
			{
				dRefVX = m_plnkVision->GetSearchResultX(nSecondCam, riTabMarkNo[1]);
				dRefVY = m_plnkVision->GetSearchResultY(nSecondCam, riTabMarkNo[1]);
				iResult	= getTabMarkRecogBlobPosition(nSecondCam, riTabMarkNo[3], iFiduTabMark1Pos, dRefVX, dRefVY, &MarkSRLineOffset2);
			}
			
			if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
			{
				dRefVX = m_plnkVision->GetSearchResultX(nSecondCam, riTabMarkNo[1]);
				dRefVY = m_plnkVision->GetSearchResultY(nSecondCam, riTabMarkNo[1]);
				iResult	= getTabMarkRecogBlobPosition(nSecondCam, riTabMarkNo[5], iFiduTabMark1Pos, dRefVX, dRefVY, &MarkTabEndOffset2);
				
			}
			//___________________________
			m_plnkVision->UnlockCamera(nSecondCam);
			MarkOffset2	= MarkOffset2 +	MarkPosition2;	// MarkOffset�� Carrier �߽ɱ����� �Ÿ��� ���Ѵ�.
		}

//		dEndCnt = GetTickCount();
//		TRACE(_T("\nJSH TEST iAlignRetryCnt = %d\n Time = %d\n"), iAlignRetryCnt, dEndCnt-dStartCnt);
		
		dSRLineOffsetDis2 = MarkSRLineOffset2.dY - iOldMarkOffset2.dY;
		dEndOffsetDis2 = MarkTabEndOffset2.dY - iOldMarkOffset2.dY;
		//@	dSRLineOffsetDis2 = fabs(dSRLineOffsetDis2); 
		//@	dEndOffsetDis2 = fabs(dEndOffsetDis2); 
		
		
		if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
		{
			CString strLog;
			CString strLog1;
			
			if (m_plnkPreBonderData->m_bSRLineAlignFunc == TRUE)
			{
				strLog.Format(_T("CamNum : %d MarkOffset2 : %.3f MarkSRLineOffset2 : %.3f SRLineDis2 : %.3f"),
					m_iTabAlignCam2, iOldMarkOffset2.dY, MarkSRLineOffset2.dY, dSRLineOffsetDis2);
				m_plnkVision->WriteLog(_T("..\\Log\\CarrierMarkDistance.csv"), (LPCTSTR)strLog);
			}
			
			if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
			{
				strLog1.Format(_T("CamNum : %d MarkOffset2 : %.3f MarkTabEndOffset2 : %.3f dEndDis2 : %.3f"),
					m_iTabAlignCam2, iOldMarkOffset2.dY, MarkTabEndOffset2.dY, dEndOffsetDis2);
				m_plnkVision->WriteLog(_T("..\\Log\\CarrierMarkDistance.csv"), (LPCTSTR)strLog1);
			}
		}
		//____________________________
		
		if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
		{
			char szResult[256];
			memset(szResult, 0, sizeof(szResult));
			sprintf(szResult, "DX=%.3lf", dEndOffsetDis2);
			m_plnkVision->DrawOverlayText(nSecondCam, szResult);
		}
		
		//____________________________________________________________
		//
		// Step 5. Align Offset ��� �ϱ�
		//____________________________________________________________
		// Dry Run Mode������ ��������� �۾���
		if (m_plnkSystemData->m_eRunMode == DRY_RUN_MODE)
			return ERR_CTRL_TABCARRIER_SUCCESS;
		
		// Radian ������ Ʋ���� ���� ���
		///AlignOffset.dT = -atan((MarkOffset2.dY - MarkOffset1.dY) / (MarkOffset2.dX - MarkOffset1.dX));
		///AlignOffset.dT = atan((MarkOffset2.dX - MarkOffset1.dX) / (MarkOffset2.dY - MarkOffset1.dY));
		///�ӽ�
		
#ifdef DEF_SOURCE_SYSTEM
		AlignOffset.dT = -atan((MarkOffset2.dY - MarkOffset1.dY) / ( MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Xtab ));
#else
		AlignOffset.dT = -atan((MarkOffset2.dY - MarkOffset1.dY) / ( MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Ytab ));
#endif
		///TRACE(_T("��� ���� : %.5lf\n"), AlignOffset.dT);
		
		iTabMarkPosIndex = iFiduTabMark1Pos;
		
		// Align�� �ȵǾ��� ��� Fidu Mark ��ġ�� Offset�� ���� ����ؾ� �Ѵ�.
		// Align ��ġ�� Offset�� �ٸ� ��ġ�� ������ ��ġ�� ���� ���� �ϰ��� ���̴�.
		
		// ������ Tab  ���� ���
		//m_dMeasuredMarkDistance = _hypot((MarkOffset2.dX - MarkOffset1.dX), 
		//								(MarkOffset2.dY - MarkOffset1.dY));
		
		//Method1
		
		// Ʋ���� Theta���� ȸ�������� X,Y ��ǥ ������ ��� (ù��° �νĵ� Mark Offset�� ������ �Ѵ�.)
		///AlignOffset.dX = (cos(AlignOffset.dT) * MarkOffset1.dX) 
		///				- (sin(AlignOffset.dT) * MarkOffset1.dY);
		///AlignOffset.dY = (sin(AlignOffset.dT) * MarkOffset1.dX) 
		///				+ (cos(AlignOffset.dT) * MarkOffset1.dY);
		
		//Left Mark Align Offset
		AlignOffsetL.dX = (cos(AlignOffset.dT) * MarkOffset1.dX) 
			- (sin(AlignOffset.dT) * MarkOffset1.dY);
		AlignOffsetL.dY = (sin(AlignOffset.dT) * MarkOffset1.dX) 
			+ (cos(AlignOffset.dT) * MarkOffset1.dY);
		
		// ���� ������ XY - Panel Mark������ �Ÿ�����
		AlignOffsetL.dX = MarkPosition1.dX - AlignOffsetL.dX;
		AlignOffsetL.dY = MarkPosition1.dY - AlignOffsetL.dY;
		
		//Right Mark Align Offset
		AlignOffsetR.dX = (cos(AlignOffset.dT) * MarkOffset2.dX) 
			- (sin(AlignOffset.dT) * MarkOffset2.dY);
		AlignOffsetR.dY = (sin(AlignOffset.dT) * MarkOffset2.dX) 
			+ (cos(AlignOffset.dT) * MarkOffset2.dY);
		
		// ���� ������ XY - Panel Mark������ �Ÿ�����
		AlignOffsetR.dX = MarkPosition2.dX - AlignOffsetR.dX;
		AlignOffsetR.dY = MarkPosition2.dY - AlignOffsetR.dY;
		
		
		//Center���� Align Offset���� ���Ѵ�
		AlignOffset.dX = (AlignOffsetL.dX + AlignOffsetR.dX)/2.0;
		AlignOffset.dY = (AlignOffsetL.dY + AlignOffsetR.dY)/2.0;
		
		if (m_plnkPreBonderData->m_bSRLineAlignFunc == TRUE)
		{
		/*/
		if (dSRLineOffsetDis1 < -0.1 || dSRLineOffsetDis1 > 0.1)  // ������ Check
		{
		SetErrorLevel(_T("AlignTabIC() : SRLineMarkDistance value did allowable value (0.1) more than."),1, __FILE__, __LINE__);
		return generateErrorCode(317024);	
		}
		
		  if (dSRLineOffsetDis2 < -0.1 || dSRLineOffsetDis2 > 0.1)  // ������ Check
		  {
		  SetErrorLevel(_T("AlignTabIC() : SRLineMarkDistance value did allowable value (0.1) more than."),1, __FILE__, __LINE__);
		  return generateErrorCode(317025);	
		  }
			/*/
			
			//@____________________
			if (fabs(MarkSRLineOffset1.dY - MarkSRLineOffset2.dY) > 0.1)
				return generateErrorCode(317026);	
			
			double dAvgDist = m_plnkPreBonderData->m_dSRLineMarkDistance - (dSRLineOffsetDis1 + dSRLineOffsetDis2)/2.0;
			if (fabs(dAvgDist) > m_plnkPreBonderData->m_dToleranceSRLine)
				return generateErrorCode(317026);	
			//@____________________
			
			//201201008 SJ_HJG
#ifdef DEF_GATE_SYSTEM
			AlignOffset.dY = AlignOffset.dY + dAvgDist;
#else
			AlignOffset.dY = AlignOffset.dY + dAvgDist;
#endif
			
		}
		
		if (m_plnkPreBonderData->m_bMarkDistanceFunc == TRUE)
		{		
		/*
		//@____________________
		if (fabs(MarkTabEndOffset1.dX - MarkTabEndOffset2.dX) > 0.2)
		{
		SetErrorLevel(_T("AlignTabIC() : TabEnd Mark Y Difference is too big.(more than 0.1 mm)"), 1, __FILE__, __LINE__);
		//130131.kms____________
		//Log ���� Image Save
		//______________________
		return generateErrorCode(317027);	
		}
		//@____________________
			*/
			
			double dEndDistance = m_plnkPreBonderData->m_dTabEndMarkDistance - (dEndOffsetDis1 + dEndOffsetDis2)/2;
			
			if (fabs(dEndDistance) > m_plnkPreBonderData->m_dToleranceTabEnd)
				return generateErrorCode(317028);
			
			//@		if (fabs(dEndDistance) < 0.015)
			if (fabs(dEndDistance) <= m_plnkPreBonderData->m_dNoCareValueTabEnd)
				dEndDistance = 0.0;
			
			MOlbSystemPre.SetTabEndMarkDistance(m_iWorkGroup, dEndDistance);
		}
		else
		{
			MOlbSystemPre.SetTabEndMarkDistance(m_iWorkGroup, 0.0);
		}
		//___________________
		
		// X,Y ����� �������� T���� degree ��ǥ�� ��ȯ�Ѵ�.
		///AlignOffset.dT = RAD2DEG(AlignOffset.dT);
		double dAngle = 180 * AlignOffset.dT / PI;
		
		///TRACE(_T("��� ���� : %.5lf\n"), dAngle);
		
		// Align ��ġ�� T��ǥ Offset �� ����
		////if (!m_plnkTabCarrier->IsAligned())
		////	AlignOffset.dT += sCarrierXYTOffsetPos[iTabMarkPosIndex].dT;
		
		// T��ǥ ���� ���� üũ, �ʹ� ���� ���� ���� �ȵȴ�.	
		//@	if (dAngle < -10.0 || dAngle > 10.0)  // ������ Check
		if (dAngle < -5.0 || dAngle > 5.0)  // ������ Check
			return generateErrorCode(317022);	
		
		// Align ����� Mechanical Layer�� carrier Component�� �����Ѵ�.
		///PriorAlignOffset = m_plnkTabCarrier->GetAlignMarkOffset();			// TabCarrier ����� Align�� �о� �´�.
		///AlignOffset	= AlignOffset + PriorAlignOffset;						// Align �������� ���� �Ѵ�.
		///m_plnkTabCarrier->SetAlignMarkOffset(AlignOffset, ActualAlignPos.dT);	// Stage�� Align ������� ���� �Ѵ�.
		AlignOffset.dT = dAngle ;
		
//		dEndCnt = GetTickCount();
//		TRACE(_T("\nJSH TEST iAlignRetryCnt = %d\n Time = %d\n"), iAlignRetryCnt, dEndCnt-dStartCnt);

		if(fabs(AlignOffset.dX) < 0.02
			&& fabs(AlignOffset.dY) < 0.005
			&& fabs(AlignOffset.dT) < 0.003)
			break;
	

		PriorAlignOffset = m_plnkTabCarrier->GetAlignMarkOffset();			// TabCarrier ����� Align�� �о� �´�.
		AlignOffset	= AlignOffset + PriorAlignOffset;						// Align �������� ���� �Ѵ�.
		m_plnkTabCarrier->SetAlignMarkOffset(AlignOffset, ActualAlignPos.dT);	// Stage�� Align ������� ���� �Ѵ�.
		
		///m_plnkTabCarrier->SetAlignMarkOffset(AlignOffset, 0);
	}
	
	TRACE(_T("\n\n\nJSH TEST iAlignRetryCnt = %d\n\n\n"), iAlignRetryCnt);

	if(iAlignRetryCnt == 20)
	{
		return generateErrorCode(317031);
	}

	AlignOffset	= m_plnkTabCarrier->GetAlignMarkOffset();			// TabCarrier ����� Align�� �о� �´�

	// Align Offset ��� return
	if (pRetAlignOffset != NULL)
		pRetAlignOffset->Init(AlignOffset.dX, AlignOffset.dY, AlignOffset.dT);
	
	WriteTactTimeLog(_T(" [MTabCarrier] [M] [Align Tab IC] [End]"));

	return ERR_CTRL_TABCARRIER_SUCCESS;
}
//SJ_YYK 150213 Add...
int MCtrlTabCarrier::MoveSplineLoadPos()
{
	int iResult = ERR_CTRL_TABCARRIER_SUCCESS;

	double dXPos[4] = {m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_START_POS).dX,
						m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dX + 15.0,
						m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dX,
						m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_POS).dX};

	double dZPos[4]= {m_plnkTabCarrier->GetTargetZPos(DEF_TABCARRIER_Z_DOWN_POS),
						m_plnkTabCarrier->GetTargetZPos(DEF_TABCARRIER_Z_DOWN_POS),
						m_plnkTabCarrier->GetTargetZPos(DEF_TABCARRIER_Z_UP_POS) - 3.0,
						m_plnkTabCarrier->GetTargetZPos(DEF_TABCARRIER_Z_UP_POS),};


	double dXSplineDist = dXPos[2] - dXPos[1];
	double dZSplineDist = dZPos[2] - dZPos[1];

	double dSplineVel;

	if(fabs(dXSplineDist) > fabs(dZSplineDist))
		dSplineVel = m_plnkTabCarrier->GetZMotionComponent()->GetMovingVelocity() * (sqrt((dXSplineDist*dXSplineDist)+(dZSplineDist*dZSplineDist))/(dXSplineDist));
	else
		dSplineVel = m_plnkTabCarrier->GetZMotionComponent()->GetMovingVelocity() * (sqrt((dXSplineDist*dXSplineDist)+(dZSplineDist*dZSplineDist))/(dZSplineDist));

	dSplineVel *= 5.0;

	double dVel[4] = {m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->GetMovingVelocity() * 1.414,
						m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->GetMovingVelocity() * 1.414,	
						dSplineVel,
						m_plnkTabCarrier->GetZMotionComponent()->GetMovingVelocity() * 1.414};

	int dAcc[4] = {m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->GetMovingAccelerate(),
					m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->GetMovingAccelerate(),					
					m_plnkTabCarrier->GetZMotionComponent()->GetMovingAccelerate(),
					m_plnkTabCarrier->GetZMotionComponent()->GetMovingAccelerate(),};


	iResult = m_plnkTabCarrier->StartSplineMove(4, dXPos, dZPos, dVel, dAcc, TRUE);
	if(iResult)
		return iResult;

	return ERR_CTRL_TABCARRIER_SUCCESS;
}

int MCtrlTabCarrier::MoveSplinePrealignPos()
{
	int iResult = ERR_CTRL_TABCARRIER_SUCCESS;

	double dXPos[4] = {m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_LOAD_START_POS).dX - DEF_TABCARRIER_STANDARD_GAP,
						m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS).dX + 15.0,
						m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS).dX,
						m_plnkTabCarrier->GetTargetXYTPos(DEF_TABCARRIER_PREALIGN_POS).dX};

	double dZPos[4] = {m_plnkTabCarrier->GetTargetZPos(DEF_TABCARRIER_Z_DOWN_POS),
						m_plnkTabCarrier->GetTargetZPos(DEF_TABCARRIER_Z_DOWN_POS),
						m_plnkTabCarrier->GetTargetZPos(DEF_TABCARRIER_Z_UP_POS) - 3.0,
						m_plnkTabCarrier->GetTargetZPos(DEF_TABCARRIER_Z_UP_POS),};

	double dXSplineDist = dXPos[2] - dXPos[1];
	double dZSplineDist = dZPos[2] - dZPos[1];

	double dSplineVel;

	if(fabs(dXSplineDist) > fabs(dZSplineDist))
		dSplineVel = m_plnkTabCarrier->GetZMotionComponent()->GetMovingVelocity() * (sqrt((dXSplineDist*dXSplineDist)+(dZSplineDist*dZSplineDist))/(dXSplineDist));
	else
		dSplineVel = m_plnkTabCarrier->GetZMotionComponent()->GetMovingVelocity() * (sqrt((dXSplineDist*dXSplineDist)+(dZSplineDist*dZSplineDist))/(dZSplineDist));

	dSplineVel *= 5.0;

	double dVel[4] = {m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->GetMovingVelocity() * 1.414,
						m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->GetMovingVelocity() * 1.414,	
						dSplineVel,
						m_plnkTabCarrier->GetZMotionComponent()->GetMovingVelocity() * 1.414};

	int dAcc[4] = {m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->GetMovingAccelerate(),
					m_plnkTabCarrier->GetMotionComponent(DEF_TABCARRIER_X_AXIS)->GetMovingAccelerate(),					
					m_plnkTabCarrier->GetZMotionComponent()->GetMovingAccelerate(),
					m_plnkTabCarrier->GetZMotionComponent()->GetMovingAccelerate(),};

	iResult = m_plnkTabCarrier->StartSplineMove(4, dXPos, dZPos, dVel, dAcc, TRUE);
	if(iResult)
		return iResult;

	return ERR_CTRL_TABCARRIER_SUCCESS;
}
// End of Code