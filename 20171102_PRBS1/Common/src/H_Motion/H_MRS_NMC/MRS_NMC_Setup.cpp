#include "stdafx.h"
#include "MPlatformOlbSystem.h"
#include "MRS_NMC_Setup.h"
#include "DefMotionValue.h"
#include "DefMotionError.h"

#include "MTickTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern MPlatformOlbSystem	MOlbSystemPre;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Constructor : initialize inner object, 내부 개체들을 초기화한다.
 * @stereotype constructor
 */	
MRS_NMCSetup::MRS_NMCSetup()
{
	m_bInitialized		= FALSE;
	m_bIsInitializing		= FALSE;
	m_bOriginReturn		= FALSE;

	memset(chGetErrorMessage, 0, sizeof(chGetErrorMessage));
}


/**
 * Destructor : Delete inner object
 *				내부 개체를 소멸한다.
 * @stereotype destructor
 */
MRS_NMCSetup::~MRS_NMCSetup()
{

}

/**
 * Board의 상태를 초기화 한다. 
 *
 * @return	Error Code : 0 = SUCCESS, 그외 = Error
 */

// Doolin kwangilshin 2017. 08. 27.
// Modify Function.
//
int MRS_NMCSetup::BoardInit()
{
#ifdef SIMULATION
	return ERR_MOTION_SUCCESS;
#endif
	
	int			iResult;
	MC_STATUS	eResult;
	
	MTickTimer	clsTimer;
	BOOL		bIsCheck = TRUE;

	if (m_csLock.Lock())
	{
		if (!m_bIsInitializing)
		{
			eResult = MC_Init();
			
			if((eResult = MC_Init()) != MC_OK)
			{
				m_csLock.Unlock();
				return MakeRS_NMC_Error(eResult);
			}

			Sleep(100);

			if ((iResult = ECatReset()) != ERR_MOTION_SUCCESS)
			{
				m_csLock.Unlock();
				return eResult;
			}
			
			clsTimer.StartTimer();
			bIsCheck = TRUE;
			while (bIsCheck)
			{
				if (IsMasterRun())
				{
					bIsCheck = FALSE;
					break;		
				}
				else if (clsTimer.MoreThan(2))
				{
					m_csLock.Unlock();
					return COMM_CONNECTION_TIMED_OUT;
				}
				
				Sleep(10);
			}

			m_bInitialized = TRUE;
			m_bIsInitializing = FALSE;
		}

		m_csLock.Unlock();
	}
	return ERR_MOTION_SUCCESS;
}

//이더캣통신 시작
int MRS_NMCSetup::ECatRun()	
{
	MC_STATUS eResult;

	if (m_csLock.Lock())
	{
		for(int i=0; i<DEF_AXIS_SERCOS_NUM_MOTION_BOARD; i++)
		{
			eResult = MC_MasterRUN(i);
			
			if(eResult != MC_OK)
			{
				m_csLock.Unlock();
				return MakeRS_NMC_Error(eResult);
			}
		}

		m_csLock.Unlock();
	}
	
	return ERR_MOTION_SUCCESS;
}

//이더캣통신 정지
int MRS_NMCSetup::ECatStop()	
{
	MC_STATUS eResult;

	if (m_csLock.Lock())
	{
		for(int i=0; i<DEF_AXIS_SERCOS_NUM_MOTION_BOARD; i++)
		{
			eResult = MC_MasterSTOP(i);
			if(eResult != MC_OK)
			{
				m_csLock.Unlock();
				return MakeRS_NMC_Error(eResult);
			}
		}
		
		m_csLock.Unlock();
	}
	return ERR_MOTION_SUCCESS;
}

//이더캣 통신을 리셋
int MRS_NMCSetup::ECatReset()	
{
	MTickTimer	clsTimer;
	BOOL		bIsCheck = TRUE;

	int			iResult;

	if (m_csLock.Lock())
	{
		if ((iResult = ECatStop()) != ERR_MOTION_SUCCESS)
		{
			m_csLock.Unlock();
			return iResult;
		}
		
		clsTimer.StartTimer();
		bIsCheck = TRUE;
		while (bIsCheck)
		{
			if (eMM_IDLE == GetMasterMode())
			{
				bIsCheck = FALSE;
				break;		
			}
			else if (clsTimer.MoreThan(2))
			{
				m_csLock.Unlock();
				return COMM_CONNECTION_TIMED_OUT;
			}
			
			Sleep(10);
		}
		
		if ((iResult = ECatRun()) != ERR_MOTION_SUCCESS)
		{
			m_csLock.Unlock();
			return iResult;
		}

		clsTimer.StartTimer();
		bIsCheck = TRUE;
		while (bIsCheck)
		{
			if (eMM_RUN == GetMasterMode())
			{
				bIsCheck = FALSE;
				break;		
			}
			else if (clsTimer.MoreThan(2))
			{
				m_csLock.Unlock();
				return COMM_CONNECTION_TIMED_OUT;
			}
			
			Sleep(10);
		}

		m_csLock.Unlock();
	}

	return ERR_MOTION_SUCCESS;
}

// 마스터가 Run 상태인지 체크
BOOL MRS_NMCSetup::IsMasterRun()
{
	UINT8	nCurMasterState = eMM_IDLE;
	BOOL	bResult			= FALSE;

	if (m_csLock.Lock())
	{
		//보드의 상태를 읽어들인다..
		MasterGetCurMode(DEF_NMC_MASTER_BOARD_ID, &nCurMasterState);
		
		if (nCurMasterState == eMM_RUN)
		{
			bResult = TRUE;
		} 

		m_csLock.Unlock();
	}

	return bResult;
}

// 마스터의 현재 상태를 읽음
int MRS_NMCSetup::GetMasterMode()
{
	UINT8 nCurMasterState = eMM_IDLE;

	if (m_csLock.Lock())
	{
		//보드의 상태를 읽어들인다..
		MasterGetCurMode(DEF_NMC_MASTER_BOARD_ID, &nCurMasterState);
		
		m_csLock.Unlock();
	}
	
	return nCurMasterState;
}

// 전체 슬레이브의 상태가 Op모드인지 체크
BOOL MRS_NMCSetup::IsSlaveOpModeAll()
{
#ifndef SIMULATION 
	if (m_csLock.Lock())
	{
		for (int iAxis=0; iAxis< DEF_AXIS_SERCOS_MAX_AXIS; iAxis++)
		{ 
			if (!IsSlaveOpMode(iAxis))
			{
				m_csLock.Unlock();

				return FALSE;
			}
		}

		m_csLock.Unlock();
	}
#else
	return FALSE;
#endif
	return TRUE;
}

// Doolin kwangilshin 2017. 08. 27.
//


BOOL MRS_NMCSetup::IsECatInitializing()
{
	BOOL bResult = FALSE;

	if (m_csLock.Lock())
	{
		bResult = m_bIsInitializing;

		m_csLock.Unlock();
	}
	
	return bResult;
}

BOOL MRS_NMCSetup::IsSlaveOpMode(int nAxisIdx)
{
	int		iBoardID;
	int		iAxisID;
	UINT8	nSlaveState = 0;
	
	if (m_csLock.Lock())
	{
		if (MOlbSystemPre.IsFunctionModuleSlaveAxis(nAxisIdx) == FALSE
			&& MOlbSystemPre.IsSkipAxis(nAxisIdx) == FALSE 
			&& nAxisIdx != DEF_AXIS_ACS_GANTRY_Y
			&& nAxisIdx != DEF_AXIS_ACS_R_GANTRY_Y)
		{
			iBoardID	= GetBoardID(nAxisIdx);
			iAxisID		= GetAxisID(nAxisIdx);
			
			SlaveGetCurState(iBoardID, iAxisID, &nSlaveState);
			
			if (nSlaveState != eST_OP)
			{
				return FALSE;
			}
		}
	
		m_csLock.Unlock();
	}
	
	return TRUE;
}
// End.

CString MRS_NMCSetup::ReturnCurrentMotionError()
{
	CString strReturn = "";
	strReturn.Format(_T("%s"), chGetErrorMessage);
	
	memset(chGetErrorMessage, 0, sizeof(chGetErrorMessage));
	
	return strReturn;
}

int MRS_NMCSetup::MakeRS_NMC_Error(MC_STATUS mcStatus)
{
	int iReturnErrorCode;

	switch(mcStatus)
	{
	case MC_ERROR_HW_NOT_INSTALLED:
		iReturnErrorCode = ERR_RS_NMC_MC_ERROR_HW_NOT_INSTALLED;
		break;
	case MC_ERROR_DD_SEND_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_ERROR_DD_SEND_ERROR;
		break;
	case MC_ERROR_DD_READ_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_ERROR_DD_READ_ERROR;
		break;
	case MC_DD_ERROR_SEND:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_ERROR_SEND;
		break;
	case MC_DD_ERROR_RECV:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_ERROR_RECV;
		break;
	case MC_DD_OPEN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_OPEN_FAIL;
		break;
	case MC_DD_NOT_OPENED:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_NOT_OPENED;
		break;
	case MC_DD_CONN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_CONN_FAIL;
		break;
	case MC_DD_CLIENT_START_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_CLIENT_START_FAIL;
		break;
	case MC_DD_OK:
		iReturnErrorCode = ERR_RS_NMC_MC_DD_OK;
		break;
	case MC_CN_NOT_CONNECTED:
		iReturnErrorCode = ERR_RS_NMC_MC_CN_NOT_CONNECTED;
		break;
	case MC_CN_CONNECTED:
		iReturnErrorCode = ERR_RS_NMC_MC_CN_CONNECTED;
		break;
	case MC_CN_CONNERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_CN_CONNERROR;
		break;
	case MC_INVALID_SYSTEM_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_SYSTEM_STATE;
		break;
	case MC_UNSUPPORT_CMD:
		iReturnErrorCode = ERR_RS_NMC_MC_UNSUPPORT_CMD;
		break;
	case MC_INVALID_AXIS_STATE_NOT_HOMING_MODE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_NOT_HOMING_MODE;
		break;
	case MC_INVALID_PARAM:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM;
		break;
	case MC_INVALID_PARAM_1:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_1;
		break;
	case MC_INVALID_PARAM_2:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_2;
		break;
	case MC_INVALID_PARAM_3:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_3;
		break;
	case MC_INVALID_PARAM_4:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_4;
		break;
	case MC_INVALID_PARAM_5:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_5;
		break;
	case MC_INVALID_PARAM_6:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_6;
		break;
	case MC_INVALID_PARAM_7:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_7;
		break;
	case MC_INVALID_PARAM_8:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_8;
		break;
	case MC_INVALID_PARAM_9:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_9;
		break;
	case MC_INVALID_PARAM_10:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_PARAM_10;
		break;
	case MC_INVALID_SIZE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_SIZE;
		break;
	case MC_INVALID_AXIS_NUM:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_NUM;
		break;
	case MC_NOT_ENOUGH_RESOURCE:
		iReturnErrorCode = ERR_RS_NMC_MC_NOT_ENOUGH_RESOURCE;
		break;
	case MC_LIMIT_ERROR_PARAM:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM;
		break;
	case MC_LIMIT_ERROR_PARAM_1:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_1;
		break;
	case MC_LIMIT_ERROR_PARAM_2:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_2;
		break;
	case MC_LIMIT_ERROR_PARAM_3:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_3;
		break;
	case MC_LIMIT_ERROR_PARAM_4:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_4;
		break;
	case MC_LIMIT_ERROR_PARAM_5:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_5;
		break;
	case MC_LIMIT_ERROR_PARAM_6:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_6;
		break;
	case MC_LIMIT_ERROR_PARAM_7:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_7;
		break;
	case MC_LIMIT_ERROR_PARAM_8:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_8;
		break;
	case MC_LIMIT_ERROR_PARAM_9:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_9;
		break;
	case MC_LIMIT_ERROR_PARAM_10:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_ERROR_PARAM_10;
		break;
	case MC_INVALID_DEVICE_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_DEVICE_STATE;
		break;
	case MC_INVALID_DEVICE_STATE_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_DEVICE_STATE_ERROR;
		break;
	case MC_INVALID_AXIS_STATE_DISABLED:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_DISABLED;
		break;
	case MC_INVALID_AXIS_STATE_STANDSTILL:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_STANDSTILL;
		break;
	case MC_INVALID_AXIS_STATE_DISCRETE_MOTION:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_DISCRETE_MOTION;
		break;
	case MC_INVALID_AXIS_STATE_CONTINUOUS_MOTION:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_CONTINUOUS_MOTION;
		break;
	case MC_INVALID_AXIS_STATE_SYNC_MOTION:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_SYNC_MOTION;
		break;
	case MC_INVALID_AXIS_STATE_HOMING:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_HOMING;
		break;
	case MC_INVALID_AXIS_STATE_STOPPING:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_STOPPING;
		break;
	case MC_INVALID_AXIS_STATE_ERRORSTOP:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_ERRORSTOP;
		break;
	case MC_INVALID_AXIS_STATE_MODE_CHANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_STATE_MODE_CHANGE;
		break;
	case MC_INVALID_AXIS_CONFIG:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG;
		break;
	case MC_INVALID_AXIS_CONFIG_POS_LIMIT_SWITCH:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_POS_LIMIT_SWITCH;
		break;
	case MC_INVALID_AXIS_CONFIG_NEG_LIMIT_SWITCH:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_NEG_LIMIT_SWITCH;
		break;
	case MC_INVALID_AXIS_CONFIG_HOME_SWITCH:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOME_SWITCH;
		break;
	case MC_INVALID_AXIS_CONFIG_Z_PHASE_INPUT:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_Z_PHASE_INPUT;
		break;
	case MC_INVALID_AXIS_CONFIG_HOME_SENSOR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOME_SENSOR;
		break;
	case MC_INVALID_AXIS_CONFIG_MARK_PULSE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_MARK_PULSE;
		break;
	case MC_INVALID_AXIS_CONFIG_HOME_TYPE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOME_TYPE;
		break;
	case MC_INVALID_AXIS_CONFIG_HOME_FLAG_HANDLE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOME_FLAG_HANDLE;
		break;
	case MC_INVALID_AXIS_CONFIG_HOMING_MODE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_CONFIG_HOMING_MODE;
		break;
	case MC_GEARING_RULE_VIOLATION:
		iReturnErrorCode = ERR_RS_NMC_MC_GEARING_RULE_VIOLATION;
		break;
	case MC_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_LIMIT_POSITION_OVER;
		break;
	case MC_POS_HW_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_POS_HW_LIMIT_POSITION_OVER;
		break;
	case MC_NEG_HW_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_NEG_HW_LIMIT_POSITION_OVER;
		break;
	case MC_POS_SW_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_POS_SW_LIMIT_POSITION_OVER;
		break;
	case MC_NEG_SW_LIMIT_POSITION_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_NEG_SW_LIMIT_POSITION_OVER;
		break;
	case MC_INVALID_AXES_GROUP_NUM:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXES_GROUP_NUM;
		break;
	case MC_AXIS_ALREADY_ASSIGNED:
		iReturnErrorCode = ERR_RS_NMC_MC_AXIS_ALREADY_ASSIGNED;
		break;
	case MC_IDENT_ALREADY_ASSIGNED:
		iReturnErrorCode = ERR_RS_NMC_MC_IDENT_ALREADY_ASSIGNED;
		break;
	case MC_AXES_GROUP_INVALID_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_AXES_GROUP_INVALID_STATE;
		break;
	case MC_GROUP_INVALID_STATE_MOVING:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_INVALID_STATE_MOVING;
		break;
	case MC_GROUP_INVALID_STATE_HOMING:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_INVALID_STATE_HOMING;
		break;
	case MC_GROUP_INVALID_STATE_STOPPING:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_INVALID_STATE_STOPPING;
		break;
	case MC_GROUP_INVALID_STATE_ERRORSTOP:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_INVALID_STATE_ERRORSTOP;
		break;
	case MC_AXIS_IN_SINGLE_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_AXIS_IN_SINGLE_MOTION_STATE;
		break;
	case MC_1ST_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_1ST_AXIS_IN_MOTION_STATE;
		break;
	case MC_2ND_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_2ND_AXIS_IN_MOTION_STATE;
		break;
	case MC_3RD_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_3RD_AXIS_IN_MOTION_STATE;
		break;
	case MC_4TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_4TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_5TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_5TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_6TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_6TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_7TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_7TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_8TH_AXIS_IN_MOTION_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_8TH_AXIS_IN_MOTION_STATE;
		break;
	case MC_GROUP_MEMBER_EMPTY:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_MEMBER_EMPTY;
		break;
	case MC_1ST_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_1ST_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_2ND_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_2ND_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_3RD_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_3RD_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_4TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_4TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_5TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_5TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_6TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_6TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_7TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_7TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_8TH_AXIS_IN_GROUP_LIMIT_OVER:
		iReturnErrorCode = ERR_RS_NMC_MC_8TH_AXIS_IN_GROUP_LIMIT_OVER;
		break;
	case MC_GROUP_CMD_SIZE_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_CMD_SIZE_ERROR;
		break;
	case MC_GROUP_CMD_PARAMETER_SIZE_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_CMD_PARAMETER_SIZE_ERROR;
		break;
	case MC_GROUP_MEMBER_NOT_ALLOCATED_X:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_MEMBER_NOT_ALLOCATED_X;
		break;
	case MC_GROUP_MEMBER_NOT_ALLOCATED_Y:
		iReturnErrorCode = ERR_RS_NMC_MC_GROUP_MEMBER_NOT_ALLOCATED_Y;
		break;
	case MC_AXIS_IN_GROUP_MOTION:
		iReturnErrorCode = ERR_RS_NMC_MC_AXIS_IN_GROUP_MOTION;
		break;
	case MC_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_FAIL;
		break;
	case MC_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_ERROR;
		break;
	case MC_IOMAPING_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_IOMAPING_ERR;
		break;
	case MC_COMMINIT_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_COMMINIT_ERR;
		break;
	case MC_COMM_EVENT_INIT_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_COMM_EVENT_INIT_ERR;
		break;
	case MC_READ_ENI_NODE_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_READ_ENI_NODE_ERR;
		break;
	case MC_INVALID_AXIS_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_AXIS_ERR;
		break;
	case MC_INVALID_BOARD_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_BOARD_ERR;
		break;
	case MC_XML_PARSING_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_XML_PARSING_ERR;
		break;
	case MC_XML_ITEM_COUNT_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_MC_XML_ITEM_COUNT_MISMATCH;
		break;
	case MC_NO_BOARD_INSTALLED:
		iReturnErrorCode = ERR_RS_NMC_MC_NO_BOARD_INSTALLED;
		break;
	case MC_INVALID_DOWNLOAD_FILE_TYPE:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_DOWNLOAD_FILE_TYPE;
		break;
	case MC_OPEN_ENI_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_OPEN_ENI_ERR;
		break;
	case MC_FILE_OPEN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_FILE_OPEN_FAIL;
		break;
	case MC_NO_MATCHING_DOWNLOADINFORMATION:
		iReturnErrorCode = ERR_RS_NMC_MC_NO_MATCHING_DOWNLOADINFORMATION;
		break;
	case MC_NONE_OP:
		iReturnErrorCode = ERR_RS_NMC_MC_NONE_OP;
		break;
	case MC_FAIL_GEN_DOWNLOAD_FILE:
		iReturnErrorCode = ERR_RS_NMC_MC_FAIL_GEN_DOWNLOAD_FILE;
		break;
	case MC_REG_KEY_READ_FAIL:
		iReturnErrorCode = ERR_RS_NMC_MC_REG_KEY_READ_FAIL;
		break;
	case MC_NOT_ALLOWED_IN_THIS_MASTER_MODE:
		iReturnErrorCode = ERR_RS_NMC_MC_NOT_ALLOWED_IN_THIS_MASTER_MODE;
		break;
	case MC_MASTERID_OUT_OF_RANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTERID_OUT_OF_RANGE;
		break;
	case MC_BOARDNO_OUT_OF_RANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_BOARDNO_OUT_OF_RANGE;
		break;
	case MC_AXISNO_OUT_OF_RANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_AXISNO_OUT_OF_RANGE;
		break;
	case MC_BOARDCNT_OUT_OF_RANGE:
		iReturnErrorCode = ERR_RS_NMC_MC_BOARDCNT_OUT_OF_RANGE;
		break;
	case MC_RETURN_SIZE_NOT_EQUAL:
		iReturnErrorCode = ERR_RS_NMC_MC_RETURN_SIZE_NOT_EQUAL;
		break;
	case MC_MASTERID_DUPLICATION_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTERID_DUPLICATION_ERR;
		break;
	case MC_PARAM_ERROR_FILE_IS_NULL:
		iReturnErrorCode = ERR_RS_NMC_MC_PARAM_ERROR_FILE_IS_NULL;
		break;
	case MC_NO_MATCHING_BOARDID_FOUND:
		iReturnErrorCode = ERR_RS_NMC_MC_NO_MATCHING_BOARDID_FOUND;
		break;
	case MC_NOT_READY_NETWORK_CONFIGURATION:
		iReturnErrorCode = ERR_RS_NMC_MC_NOT_READY_NETWORK_CONFIGURATION;
		break;
	case MC_INVALID_MASTERID_ERR:
		iReturnErrorCode = ERR_RS_NMC_MC_INVALID_MASTERID_ERR;
		break;
	case MC_MASTER_MODE_CHANGE_NOT_ALLOWED:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTER_MODE_CHANGE_NOT_ALLOWED;
		break;
	case MC_MASTER_REQUEST_PARAM_ERROR:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTER_REQUEST_PARAM_ERROR;
		break;
	case MC_MASTER_INVALID_STATE:
		iReturnErrorCode = ERR_RS_NMC_MC_MASTER_INVALID_STATE;
		break;
	case MC_NOT_MOTION_LIBRAY_INITIALIZED:
		iReturnErrorCode = ERR_RS_NMC_MC_NOT_MOTION_LIBRAY_INITIALIZED;
		break;
	case MC_IOMANAGER_NOT_RUNNING:
		iReturnErrorCode = ERR_RS_NMC_MC_IOMANAGER_NOT_RUNNING;
		break;
	case MC_ANOTHER_PROGRAM_IS_USING_NMC_LIBRARY:
		iReturnErrorCode = ERR_RS_NMC_MC_ANOTHER_PROGRAM_IS_USING_NMC_LIBRARY;
		break;
	case MC_SLAVE_ITEM_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_MC_SLAVE_ITEM_MISMATCH;
		break;
	case MC_SLAVE_ITEM_COUNT_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_MC_SLAVE_ITEM_COUNT_MISMATCH;
		break;
	case MC_PCICIP_GEN_10:
		iReturnErrorCode = ERR_RS_NMC_MC_PCICIP_GEN_10;
		break;
	case COMM_CONNECTION_ESTABLISHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_ESTABLISHED;
		break;
	case COMM_CONN_CONFIG_FAILED_INVALID_NETWORK_PATH:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONN_CONFIG_FAILED_INVALID_NETWORK_PATH;
		break;
	case COMM_CONN_CONFIG_FAILED_NO_RESPONSE:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONN_CONFIG_FAILED_NO_RESPONSE;
		break;
	case COMM_CONN_CONFIG_FAILED_ERROR_RESPONSE:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONN_CONFIG_FAILED_ERROR_RESPONSE;
		break;
	case COMM_CONNECTION_TIMED_OUT:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_TIMED_OUT;
		break;
	case COMM_CONNECTION_CLOSED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_CLOSED;
		break;
	case COMM_INCOMING_CONNECTION_RUN_IDLE_FLAG_CHANGED:
		iReturnErrorCode = ERR_RS_NMC_COMM_INCOMING_CONNECTION_RUN_IDLE_FLAG_CHANGED;
		break;
	case COMM_ASSEMBLY_NEW_INSTANCE_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ASSEMBLY_NEW_INSTANCE_DATA;
		break;
	case COMM_ASSEMBLY_NEW_MEMBER_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ASSEMBLY_NEW_MEMBER_DATA;
		break;
	case COMM_CONNECTION_NEW_INPUT_SCANNER_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_NEW_INPUT_SCANNER_DATA;
		break;
	case COMM_CONNECTION_VERIFICATION:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_VERIFICATION;
		break;
	case COMM_CONNECTION_RECONFIGURED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_RECONFIGURED;
		break;
	case COMM_REQUEST_RESPONSE_RECEIVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_REQUEST_RESPONSE_RECEIVED;
		break;
	case COMM_REQUEST_FAILED_INVALID_NETWORK_PATH:
		iReturnErrorCode = ERR_RS_NMC_COMM_REQUEST_FAILED_INVALID_NETWORK_PATH;
		break;
	case COMM_REQUEST_TIMED_OUT:
		iReturnErrorCode = ERR_RS_NMC_COMM_REQUEST_TIMED_OUT;
		break;
	case COMM_CLIENT_OBJECT_REQUEST_RECEIVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CLIENT_OBJECT_REQUEST_RECEIVED;
		break;
	case COMM_NEW_CLASS3_RESPONSE:
		iReturnErrorCode = ERR_RS_NMC_COMM_NEW_CLASS3_RESPONSE;
		break;
	case COMM_CLIENT_PCCC_REQUEST_RECEIVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CLIENT_PCCC_REQUEST_RECEIVED;
		break;
	case COMM_NEW_LIST_IDENTITY_RESPONSE:
		iReturnErrorCode = ERR_RS_NMC_COMM_NEW_LIST_IDENTITY_RESPONSE;
		break;
	case COMM_ID_RESET:
		iReturnErrorCode = ERR_RS_NMC_COMM_ID_RESET;
		break;
	case COMM_BACKPLANE_REQUEST_RECEIVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_BACKPLANE_REQUEST_RECEIVED;
		break;
	case COMM_OUT_OF_MEMORY:
		iReturnErrorCode = ERR_RS_NMC_COMM_OUT_OF_MEMORY;
		break;
	case COMM_UNABLE_INTIALIZE_WINSOCK:
		iReturnErrorCode = ERR_RS_NMC_COMM_UNABLE_INTIALIZE_WINSOCK;
		break;
	case COMM_UNABLE_START_THREAD:
		iReturnErrorCode = ERR_RS_NMC_COMM_UNABLE_START_THREAD;
		break;
	case COMM_ERROR_USING_WINSOCK:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_USING_WINSOCK;
		break;
	case COMM_ERROR_SETTING_SOCKET_TO_NONBLOCKING:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_SETTING_SOCKET_TO_NONBLOCKING;
		break;
	case COMM_ERROR_SETTING_TIMER:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_SETTING_TIMER;
		break;
	case COMM_SESSION_COUNT_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_SESSION_COUNT_LIMIT_REACHED;
		break;
	case COMM_CONNECTION_COUNT_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CONNECTION_COUNT_LIMIT_REACHED;
		break;
	case COMM_PENDING_REQUESTS_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_PENDING_REQUESTS_LIMIT_REACHED;
		break;
	case COMM_PENDING_REQUEST_GROUPS_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_PENDING_REQUEST_GROUPS_LIMIT_REACHED;
		break;
	case COMM_ERROR_UNABLE_START_MODBUS:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_UNABLE_START_MODBUS;
		break;
	case COMM_ERROR_HW_NOT_INSTALLED:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_HW_NOT_INSTALLED;
		break;
	case COMM_ERROR_DD_SEND_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DD_SEND_ERROR;
		break;
	case COMM_ERROR_DD_READ_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DD_READ_ERROR;
		break;
	case COMM_DD_ERROR_SEND:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_ERROR_SEND;
		break;
	case COMM_DD_ERROR_RECV:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_ERROR_RECV;
		break;
	case COMM_DD_OPEN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_OPEN_FAIL;
		break;
	case COMM_DD_NOT_OPENED:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_NOT_OPENED;
		break;
	case COMM_DD_CONN_FAIL:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_CONN_FAIL;
		break;
	case COMM_DD_CLIENT_START_FAIL:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_CLIENT_START_FAIL;
		break;
	case COMM_DD_OK:
		iReturnErrorCode = ERR_RS_NMC_COMM_DD_OK;
		break;
	case COMM_CN_NOT_CONNECTED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CN_NOT_CONNECTED;
		break;
	case COMM_CN_CONNECTED:
		iReturnErrorCode = ERR_RS_NMC_COMM_CN_CONNECTED;
		break;
	case COMM_CN_CONNERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_CN_CONNERROR;
		break;
	case COMM_ERROR_SUCCESS:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_SUCCESS;
		break;
	case COMM_ERROR_FAILURE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_FAILURE;
		break;
	case COMM_EXT_ERR_DUPLICATE_FWD_OPEN:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_DUPLICATE_FWD_OPEN;
		break;
	case COMM_EXT_ERR_CLASS_TRIGGER_INVALID:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CLASS_TRIGGER_INVALID;
		break;
	case COMM_EXT_ERR_OWNERSHIP_CONFLICT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_OWNERSHIP_CONFLICT;
		break;
	case COMM_EXT_ERR_CONNECTION_NOT_FOUND:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CONNECTION_NOT_FOUND;
		break;
	case COMM_EXT_ERR_INVALID_CONN_TYPE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_CONN_TYPE;
		break;
	case COMM_EXT_ERR_INVALID_CONN_SIZE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_CONN_SIZE;
		break;
	case COMM_EXT_ERR_DEVICE_NOT_CONFIGURED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_DEVICE_NOT_CONFIGURED;
		break;
	case COMM_EXT_ERR_RPI_NOT_SUPPORTED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_RPI_NOT_SUPPORTED;
		break;
	case COMM_EXT_ERR_CONNECTION_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CONNECTION_LIMIT_REACHED;
		break;
	case COMM_EXT_ERR_VENDOR_PRODUCT_CODE_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_VENDOR_PRODUCT_CODE_MISMATCH;
		break;
	case COMM_EXT_ERR_PRODUCT_TYPE_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_PRODUCT_TYPE_MISMATCH;
		break;
	case COMM_EXT_ERR_REVISION_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_REVISION_MISMATCH;
		break;
	case COMM_EXT_ERR_INVALID_CONN_POINT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_CONN_POINT;
		break;
	case COMM_EXT_ERR_INVALID_CONFIG_FORMAT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_CONFIG_FORMAT;
		break;
	case COMM_EXT_ERR_NO_CONTROLLING_CONNECTION:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_NO_CONTROLLING_CONNECTION;
		break;
	case COMM_EXT_ERR_TARGET_CONN_LIMIT_REACHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_TARGET_CONN_LIMIT_REACHED;
		break;
	case COMM_EXT_ERR_RPI_SMALLER_THAN_INHIBIT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_RPI_SMALLER_THAN_INHIBIT;
		break;
	case COMM_EXT_ERR_CONNECTION_TIMED_OUT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CONNECTION_TIMED_OUT;
		break;
	case COMM_EXT_ERR_UNCONNECTED_SEND_TIMED_OUT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_UNCONNECTED_SEND_TIMED_OUT;
		break;
	case COMM_EXT_ERR_PARAMETER_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_PARAMETER_ERROR;
		break;
	case COMM_EXT_ERR_MESSAGE_TOO_LARGE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_MESSAGE_TOO_LARGE;
		break;
	case COMM_EXT_ERR_UNCONN_ACK_WITHOUT_REPLY:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_UNCONN_ACK_WITHOUT_REPLY;
		break;
	case COMM_EXT_ERR_NO_BUFFER_MEMORY_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_NO_BUFFER_MEMORY_AVAILABLE;
		break;
	case COMM_EXT_ERR_BANDWIDTH_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_BANDWIDTH_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_TAG_FILTERS_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_TAG_FILTERS_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_REAL_TIME_DATA_NOT_CONFIG:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_REAL_TIME_DATA_NOT_CONFIG;
		break;
	case COMM_EXT_ERR_PORT_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_PORT_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_LINK_ADDR_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_LINK_ADDR_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_INVALID_SEGMENT_TYPE_VALUE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_SEGMENT_TYPE_VALUE;
		break;
	case COMM_EXT_ERR_PATH_CONNECTION_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_PATH_CONNECTION_MISMATCH;
		break;
	case COMM_EXT_ERR_INVALID_NETWORK_SEGMENT:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_NETWORK_SEGMENT;
		break;
	case COMM_EXT_ERR_INVALID_LINK_ADDRESS:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_INVALID_LINK_ADDRESS;
		break;
	case COMM_EXT_ERR_SECOND_RESOURCES_NOT_AVAILABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_SECOND_RESOURCES_NOT_AVAILABLE;
		break;
	case COMM_EXT_ERR_CONNECTION_ALREADY_ESTABLISHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_CONNECTION_ALREADY_ESTABLISHED;
		break;
	case COMM_EXT_ERR_DIRECT_CONN_ALREADY_ESTABLISHED:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_DIRECT_CONN_ALREADY_ESTABLISHED;
		break;
	case COMM_EXT_ERR_MISC:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_MISC;
		break;
	case COMM_EXT_ERR_REDUNDANT_CONNECTION_MISMATCH:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_REDUNDANT_CONNECTION_MISMATCH;
		break;
	case COMM_EXT_ERR_NO_MORE_CONSUMER_RESOURCES:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_NO_MORE_CONSUMER_RESOURCES;
		break;
	case COMM_EXT_ERR_NO_TARGET_PATH_RESOURCES:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_NO_TARGET_PATH_RESOURCES;
		break;
	case COMM_EXT_ERR_VENDOR_SPECIFIC:
		iReturnErrorCode = ERR_RS_NMC_COMM_EXT_ERR_VENDOR_SPECIFIC;
		break;
	case COMM_ERROR_NO_RESOURCE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_NO_RESOURCE;
		break;
	case COMM_ERROR_INVALID_PARAMETER_VALUE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_PARAMETER_VALUE;
		break;
	case COMM_ERROR_INVALID_SEG_TYPE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_SEG_TYPE;
		break;
	case COMM_ERROR_INVALID_DESTINATION:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_DESTINATION;
		break;
	case COMM_ERROR_PARTIAL_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_PARTIAL_DATA;
		break;
	case COMM_ERROR_CONN_LOST:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_CONN_LOST;
		break;
	case COMM_ERROR_BAD_SERVICE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_BAD_SERVICE;
		break;
	case COMM_ERROR_BAD_ATTR_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_BAD_ATTR_DATA;
		break;
	case COMM_ERROR_ATTR_LIST_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ATTR_LIST_ERROR;
		break;
	case COMM_ERROR_ALREADY_IN_REQUESTED_MODE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ALREADY_IN_REQUESTED_MODE;
		break;
	case COMM_ERROR_OBJECT_STATE_CONFLICT:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_OBJECT_STATE_CONFLICT;
		break;
	case COMM_ERROR_OBJ_ALREADY_EXISTS:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_OBJ_ALREADY_EXISTS;
		break;
	case COMM_ERROR_ATTR_NOT_SETTABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ATTR_NOT_SETTABLE;
		break;
	case COMM_ERROR_PERMISSION_DENIED:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_PERMISSION_DENIED;
		break;
	case COMM_ERROR_DEV_IN_WRONG_STATE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DEV_IN_WRONG_STATE;
		break;
	case COMM_ERROR_REPLY_DATA_TOO_LARGE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_REPLY_DATA_TOO_LARGE;
		break;
	case COMM_ERROR_FRAGMENT_PRIMITIVE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_FRAGMENT_PRIMITIVE;
		break;
	case COMM_ERROR_NOT_ENOUGH_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_NOT_ENOUGH_DATA;
		break;
	case COMM_ERROR_ATTR_NOT_SUPPORTED:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ATTR_NOT_SUPPORTED;
		break;
	case COMM_ERROR_TOO_MUCH_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_TOO_MUCH_DATA;
		break;
	case COMM_ERROR_OBJ_DOES_NOT_EXIST:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_OBJ_DOES_NOT_EXIST;
		break;
	case COMM_ERROR_NO_FRAGMENTATION:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_NO_FRAGMENTATION;
		break;
	case COMM_ERROR_DATA_NOT_SAVED:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DATA_NOT_SAVED;
		break;
	case COMM_ERROR_DATA_WRITE_FAILURE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_DATA_WRITE_FAILURE;
		break;
	case COMM_ERROR_REQUEST_TOO_LARGE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_REQUEST_TOO_LARGE;
		break;
	case COMM_ERROR_RESPONSE_TOO_LARGE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_RESPONSE_TOO_LARGE;
		break;
	case COMM_ERROR_MISSING_LIST_DATA:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_MISSING_LIST_DATA;
		break;
	case COMM_ERROR_INVALID_LIST_STATUS:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_LIST_STATUS;
		break;
	case COMM_ERROR_SERVICE_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_SERVICE_ERROR;
		break;
	case COMM_ERROR_VENDOR_SPECIFIC:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_VENDOR_SPECIFIC;
		break;
	case COMM_ERROR_INVALID_PARAMETER:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_PARAMETER;
		break;
	case COMM_ERROR_WRITE_ONCE_FAILURE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_WRITE_ONCE_FAILURE;
		break;
	case COMM_ERROR_INVALID_REPLY:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_REPLY;
		break;
	case COMM_ERROR_BAD_KEY_IN_PATH:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_BAD_KEY_IN_PATH;
		break;
	case COMM_ERROR_BAD_PATH_SIZE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_BAD_PATH_SIZE;
		break;
	case COMM_ERROR_UNEXPECTED_ATTR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_UNEXPECTED_ATTR;
		break;
	case COMM_ERROR_INVALID_MEMBER:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_INVALID_MEMBER;
		break;
	case COMM_ERROR_MEMBER_NOT_SETTABLE:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_MEMBER_NOT_SETTABLE;
		break;
	case COMM_ERROR_UNKNOWN_MODBUS_ERROR:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_UNKNOWN_MODBUS_ERROR;
		break;
	case COMM_ERROR_HW_NOT_INSTALLED1:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_HW_NOT_INSTALLED1;
		break;
	case COMM_ERROR_ENCAP_PROTOCOL:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_ENCAP_PROTOCOL;
		break;
	case COMM_ERROR_STILL_PROCESSING:
		iReturnErrorCode = ERR_RS_NMC_COMM_ERROR_STILL_PROCESSING;
		break;
	case MC_DOWNLOAD_FAIL_DUE_TO_ANOTHER_PROGRAM_IS_RUNNING:
		iReturnErrorCode = ERR_RS_NMC_MC_DOWNLOAD_FAIL_DUE_TO_ANOTHER_PROGRAM_IS_RUNNING;
		break;
	default:

#ifndef SIMULATION
		MC_GetErrorMessage(mcStatus, 128, chGetErrorMessage);
#else
		sprintf(chGetErrorMessage, "%s", "Simulation_test");
#endif

		iReturnErrorCode = ERR_RS_NMC_UNKNOWN;
		break;
	}

	if(iReturnErrorCode != ERR_RS_NMC_UNKNOWN)
	{
		memset(chGetErrorMessage, 0, sizeof(chGetErrorMessage));
	}

	return (iReturnErrorCode + 105000) * 1000;
}

int MRS_NMCSetup::GetBoardID(int iAxisID)
{
	//161103 JSH.s
	/*/
	if(iAxisID < DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X)
		return DEF_NMC_BOARD_ID_1;
	else
		return DEF_NMC_BOARD_ID_2;
	/*/
	return DEF_NMC_BOARD_ID_1;
	//161103 JSH.e
}

int MRS_NMCSetup::GetAxisID(int iAxisID)
{
	int iNodeId = 0;
	
	switch(iAxisID)
	{
	case DEF_AXIS_SERCOS_STAGE_Y:
		iNodeId = 1;
		break;
	case DEF_AXIS_SERCOS_STAGE_T:
		iNodeId = 2;	
		break;
	case DEF_AXIS_SERCOS_STAGE_X:
		iNodeId = 3;
		break;
	case DEF_AXIS_SERCOS_STAGE_Z:
		iNodeId = 5;
		break;
	case DEF_AXIS_SERCOS_TRANSFER_IN:
		iNodeId = 6;
		break;
	case DEF_AXIS_SERCOS_TRANSFER_OUT:
		iNodeId = 7;
		break;
	case DEF_AXIS_SERCOS_MODEL_CHANGE_1:
		iNodeId = 8;
		break;
	case DEF_AXIS_SERCOS_MODEL_CHANGE_2:
		iNodeId = 9;
		break;
	case DEF_AXIS_SERCOS_MOUNTER1_X:
		iNodeId = 10;
		break;
	case DEF_AXIS_SERCOS_MOUNTER2_X:
		iNodeId = 11;
		break;
	case DEF_AXIS_SERCOS_MOUNTER3_X:
		iNodeId = 12;
		break;
	case DEF_AXIS_SERCOS_MOUNTER4_X:
		iNodeId = 13;
		break;
	case DEF_AXIS_SERCOS_MOUNTER1_Z:
		iNodeId = 14;
		break;
	case DEF_AXIS_SERCOS_MOUNTER2_Z:
		iNodeId = 15;
		break;
	case DEF_AXIS_SERCOS_MOUNTER3_Z:
		iNodeId = 16;
		break;
	case DEF_AXIS_SERCOS_MOUNTER4_Z:
		iNodeId = 17;
		break;
	case DEF_AXIS_SERCOS_BACKUP1_Z:
		iNodeId = 18;
		break;
	case DEF_AXIS_SERCOS_BACKUP2_Z:
		iNodeId = 19;
		break;
	case DEF_AXIS_SERCOS_BACKUP3_Z:
		iNodeId = 20;
		break;
	case DEF_AXIS_SERCOS_BACKUP4_Z:
		iNodeId = 21;
		break;
	case DEF_AXIS_SERCOS_INSPECTION_CAMERA1_X:
		iNodeId = 22;
		break;
	case DEF_AXIS_SERCOS_INSPECTION_CAMERA2_X:
		iNodeId = 23;
		break;
	case DEF_AXIS_SERCOS_INSPECTION_CAMERA3_X:
		iNodeId = 24;
		break;
	case DEF_AXIS_SERCOS_INSPECTION_CAMERA4_X:
		iNodeId = 25;
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER1_X:
		iNodeId = 26;
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER2_X:
		iNodeId = 27;
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER3_X:
		iNodeId = 28;
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER4_X:
		iNodeId = 29;
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER1_Z:
		iNodeId = 30;
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER2_Z:
		iNodeId = 31;
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER3_Z:
		iNodeId = 32;
		break;
	case DEF_AXIS_SERCOS_R_MOUNTER4_Z:
		iNodeId = 33;
		break;
	case DEF_AXIS_SERCOS_R_BACKUP1_Z:
		iNodeId = 34;
		break;
	case DEF_AXIS_SERCOS_R_BACKUP2_Z:
		iNodeId = 35;
		break;
	case DEF_AXIS_SERCOS_R_BACKUP3_Z:
		iNodeId = 36;
		break;
	case DEF_AXIS_SERCOS_R_BACKUP4_Z:
		iNodeId = 37;
		break;
	case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA1_X:
		iNodeId = 38;
		break;
	case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA2_X:
		iNodeId = 39;
		break;
	case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA3_X:
		iNodeId = 40;
		break;
	case DEF_AXIS_SERCOS_R_INSPECTION_CAMERA4_X:
		iNodeId = 41;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER1_Y:
		iNodeId = 42;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_Y:
		iNodeId = 43;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_Y:
		iNodeId = 44;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_Y:
		iNodeId = 45;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER1_X:
		iNodeId = 46;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_X:
		iNodeId = 47;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_X:
		iNodeId = 48;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_X:
		iNodeId = 49;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER1_T:
		iNodeId = 50;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER2_T:
		iNodeId = 51;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER3_T:
		iNodeId = 52;
		break;
	case DEF_AXIS_SERCOS_TABIC_CARRIER4_T:
		iNodeId = 53;
		break;
	case DEF_AXIS_SERCOS_INSPECT_EXPAND1:
		iNodeId = 54;
		break;
	case DEF_AXIS_SERCOS_INSPECT_EXPAND2:
		iNodeId = 55;
		break;
	case DEF_AXIS_SERCOS_INSPECT_EXPAND3:
		iNodeId = 56;
		break;
	case DEF_AXIS_SERCOS_INSPECT_EXPAND4:
		iNodeId = 57;
		break;
	case DEF_AXIS_SERCOS_TABCAMERA_EXPAND:
		iNodeId = 58;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_Y:
		iNodeId = 59;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_Y:
		iNodeId = 60;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_Y:
		iNodeId = 61;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_Y:
		iNodeId = 62;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_X:
		iNodeId = 63;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_X:
		iNodeId = 64;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_X:
		iNodeId = 65;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_X:
		iNodeId = 66;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER1_T:
		iNodeId = 67;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER2_T:
		iNodeId = 68;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER3_T:
		iNodeId = 69;
		break;
	case DEF_AXIS_SERCOS_TABIC_R_CARRIER4_T:
		iNodeId = 70;
		break;
	case DEF_AXIS_SERCOS_R_INSPECT_EXPAND1:
		iNodeId = 71;
		break;
	case DEF_AXIS_SERCOS_R_INSPECT_EXPAND2:
		iNodeId = 72;
		break;
	case DEF_AXIS_SERCOS_R_INSPECT_EXPAND3:
		iNodeId = 73;
		break;
	case DEF_AXIS_SERCOS_R_INSPECT_EXPAND4:
		iNodeId = 74;
		break;
	case DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND:
		iNodeId = 75;
		break;
	case DEF_AXIS_SERCOS_TABCAMERA_EXPAND2:
		iNodeId = 76;
		break;
	case DEF_AXIS_SERCOS_R_TABCAMERA_EXPAND2:
		iNodeId = 77;
		break;
#ifdef DEF_USE_TRANSFER_CENTER
	case DEF_AXIS_SERCOS_TRANSFER_CENTER_1:
		iNodeId = 78;
		break;
	case DEF_AXIS_SERCOS_TRANSFER_CENTER_2:
		iNodeId = 79;
		break;
#endif
#ifdef DEF_USE_TURN_HANDLER
	case DEF_AXIS_SERCOS_HANDLER_T:
		iNodeId = 80;
		break;
	case DEF_AXIS_SERCOS_HANDLER_Z:
		iNodeId = 81;
		break;
#endif

	case DEF_AXIS_SERCOS_TRANSFER_IN_2:
		iNodeId = 82;
		break;

#ifdef DEF_USE_TRANSFER_OUT_MULTI_AXIS
	case DEF_AXIS_SERCOS_TRANSFER_OUT_2:
		iNodeId = 83;
		break;
#endif
	}

	return iNodeId;
}