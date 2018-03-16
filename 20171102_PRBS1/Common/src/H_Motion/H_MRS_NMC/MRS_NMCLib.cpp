#include "stdafx.h"

#include "MRS_NMCLib.h"
//#include "NMC_Motion.h"
#include "DefMotionError.h"
#include "MPlatformOlbSystem.h"
#include "MTickTimer.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/**
 * MRS_NMCLib.cpp : This file implements the class of RS EtherCat Motion Library.
 *				 이 파일은 RS EtherCat Motion Library를 위한 Class의 실행 코드를 구현한다.
 */

int	MRS_NMCLib::m_iObjNo = 0;

/**
 * Constructor : create and clear data
 *				 Data를 생성하고 초기화한다.
 * @stereotype constructor
 */
MRS_NMCLib::MRS_NMCLib(SCommonAttribute commonData)
:ICommon(commonData)	//** 1222
{

	m_pBoardConfig = new MMotionBoard();

	/** MRS_NMCLib 개체 수 증가 */
	m_iObjNo++;

	m_iBoardNo = 0;

	memset(chGetErrorMessage, 0, sizeof(chGetErrorMessage));
}

/**
 * Destructor : Delete inner object
 *				내부 개체를 소멸한다.
 * @stereotype destructor
 */
MRS_NMCLib::~MRS_NMCLib()
{
	BOOL bStatus;

	/** MRS_NMCLib 개체 수 감소 */
	m_iObjNo--;

	/** 마지막 개체이면 */
	if (m_iObjNo == 0)
	{
		/** Axis All Stop */
		/** Servo All Off */
		for (int i = 0; i < DEF_MAX_AXIS_NO; i++)
		{
			m_pBoardConfig->GetUseAxis(i, &bStatus);
			if (bStatus == TRUE)
			{
				SetStop(i, DEF_STOP, 1000, 0);
				SetAmpEnable(i, FALSE);
			}
		}
	}

	/** Board 구성정보 개체 소멸 */
	if (m_pBoardConfig != NULL)
		delete m_pBoardConfig;
}

/**
 * 축 사용 여부 설정 (이미 사용중인 축은 사용으로 설정 불가)
 *
 * 축 사용 여부 설정은 Motor Parameter 설정과는 무관하며, 사용자에 의해 설정이 되어야 한다.
 *
 * @param	siAxisID		: 축 ID
 * @param	bState			: 사용 여부 (TRUE=사용, FALSE=미사용)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = USED AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetUseAxis(int iBoardNo, short siAxisID, BOOL bState)
{
	ErrorInfo	errorInfo;
	BOOL	bStt = 0;
	CString	strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxisID < 1) || (siAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	/** 축 사용 유무에 따라 */
	if (bState == DEF_AXIS_USE)
	{
		/** 축 사용 여부 읽기 */
		if ((errorInfo = m_pBoardConfig->GetUseAxis(siAxisID, &bStt)).IsError())
			return errorInfo;

		/** 이미 축을 사용하고 있으면 */
		if (bStt == DEF_AXIS_USE)
			return NO_ERROR_SUCESS;

		/** 축 사용하기로 설정 */
		if ((errorInfo = m_pBoardConfig->SetUseAxis(siAxisID, bState)).IsError())
			return errorInfo;
	}
	else
	{
		/** 축 사용안하기로 설정 */
		if ((errorInfo = m_pBoardConfig->SetUseAxis(siAxisID, bState)).IsError())
			return errorInfo;
	}

	m_iBoardNo = iBoardNo;

	return NO_ERROR_SUCESS;
}
/**
 * 축 사용 여부 읽기
 *
 * @param	siAxisID		: 축 ID
 * @param	*pbState		: 사용 여부 (TRUE=사용, FALSE=미사용)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetUseAxis(short siAxisID, BOOL *pbState)
{
	ErrorInfo errorInfo;
	CString	strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxisID < 1) || (siAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	/** 인수 Pointer 오류 */
	if (pbState == NULL)
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 축 사용 여부 읽기 */
	if ((errorInfo = m_pBoardConfig->GetUseAxis(siAxisID, pbState)).IsError())
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * Motor Parameter 설정 (Board 초기화 후 사용 가능)
 *
 * 전달된 SMotionAxis의 내용으로 Motion을 설정한다. (축 1개 단위로 설정)
 *
 * 1. Board가 초기화 안되어 있으면 Error Return
 * 2. 축 AMP Enable(Servo ON)이면 Disable(OFF) 후 작업 진행
 * 3. Motor 종류 설정
 * 4. Feedback Device 설정
 * 5. Loop 형태 설정
 * 6. Motor 종류에 따라
 *		6.1 속도형 Servo의 경우
 *			제어 모드와 출력 형태 설정
 *		6.2 일반 Stepper의 경우
 *			펄스 분주비와 전자기어비 설정
 *		6.3 Micro Stepper 혹은 위치형 Servo의 경우
 *			펄스 분주비(default:8)와 전자기어비(default:1.0) 설정
 * 7. 출력 펄스 형태 설정
 * 8. Encoder와 좌표 방향 설정
 * 9. AMP Enable, Fault, Reset 설정
 * 10. Home, Positive, Negative Sensor 설정
 * 11. Positive, Negative S/W Limit 설정
 * 12. In-Position 설정
 * 13. 축 AMP Enable(Servo ON)이었으면 Enable(ON) 설정
 *
 * @param	siAxisID		: 축 ID, -1=허용안됨
 * @param	mAx				: Motor Parameter
 * @param	bBootOpt		: (OPTION=FALSE) boot file에 기록할지 여부, TRUE=boot file에 기록
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = USED AXIS ID (MOTIONLIB)
 *							  xx = MOTION BOARD NO INITIALIZED (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetMotorParam(short siAxisID, SMotionAxis mAx, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	BOOL	bAmpState = FALSE;
	CString	strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxisID < 1) || (siAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109); // ERR_MLIB_INVALID_AXIS_ID

	/** Board 초기화 수행 여부 */
	if (m_pBoardConfig->GetBoardInitSuccess() != TRUE)
		return generateErrorCode(106073); // ERR_MLIB_NO_MOTION_BOARD_INITIALIZED
	
	/** AMP Enable 상태 확인 */
	if ((errorInfo = GetAmpEnable(siAxisID, &bAmpState)).IsError())
		return errorInfo;

	/** AMP Enable이면 Disable 설정 */
	if (bAmpState == DEF_ENABLE)
	{
		if ((errorInfo = SetAmpEnable(siAxisID, DEF_DISABLE)).IsError())
			return errorInfo;
	}

	/** Motor 종류 설정 */
	if ((errorInfo = SetMotorType(siAxisID, mAx.iMotorType, bBootOpt)).IsError())
		return errorInfo;

	/** Motor Feed Device와 Lop 종류 설정 */
	if ((errorInfo = SetAxisProperty(siAxisID, mAx.iFeedbackType, mAx.bLoopType, bBootOpt)).IsError())
		return errorInfo;

	/** Motor Type에 따라 */
	switch (mAx.iMotorType)
	{
	case DEF_SERVO_MOTOR :			// 속도형 Serve
		/** 속도형 Servo의 제어 모드와 출력 형태 설정 */
		if ((errorInfo = SetVServoProperty(siAxisID, mAx.bVServoControl, mAx.bVServoPolar, bBootOpt)).IsError())
			return errorInfo;

		break;
	case DEF_STEPPER :				// 일반 Stepper
		/** 펄스 분주비와 전자기어비 설정 */
		if ((errorInfo = SetRatioProperty(siAxisID, mAx.iStepperPulseR, mAx.dStepperEGear, bBootOpt)).IsError())
			return errorInfo;

		break;
	case DEF_MICRO_STEPPER :		// Micro Stepper 또는 위치형 Servo
		/** 펄스 분주비와 전자기어비 설정 */
		if ((errorInfo = SetRatioProperty(siAxisID, mAx.iStepperPulseR, mAx.dStepperEGear, bBootOpt)).IsError())
			return errorInfo;

		break;
	}

	/** 출력 펄스 형태 설정 */
	if ((errorInfo = SetStepMode(siAxisID, mAx.bPulseType, bBootOpt)).IsError())
		return errorInfo;

	/** Encoder 방향과 좌표 방향 설정 */
	if ((errorInfo = SetEncoderDirection(siAxisID, mAx.bEncoderDir, mAx.bCoordinateDir, bBootOpt)).IsError())
		return errorInfo;
	Sleep(10);
	/** AMP Enable Level 설정 */
	if ((errorInfo = SetAmpEnableLevel(siAxisID, mAx.bAmpEnableLevel, bBootOpt)).IsError())
		return errorInfo;

	/** AMP Fault Level 설정 */
	if ((errorInfo = SetAmpFaultLevel(siAxisID, mAx.bAmpFaultLevel, bBootOpt)).IsError())
		return errorInfo;

	/** AMP Fault Event 설정 */
	if ((errorInfo = SetAmpFaultEvent(siAxisID, mAx.iAmpFaultEvent, bBootOpt)).IsError())
		return errorInfo;

	/** AMP Rest Level 설정 */
	if ((errorInfo = SetAmpResetLevel(siAxisID, mAx.bAmpResetLevel, bBootOpt)).IsError())
		return errorInfo;

	/** Home Sensor Level/Event 설정 */
	if ((errorInfo = SetSensorLimit(siAxisID, DEF_HOME_SENSOR, mAx.iHomeEvent, mAx.bHomeLevel, bBootOpt)).IsError())
		return errorInfo;

	/** Positive Sensor Level/Event 설정 */
	if ((errorInfo = SetSensorLimit(siAxisID, DEF_POSITIVE_SENSOR, mAx.iPositiveEvent, mAx.bPositiveLevel, bBootOpt)).IsError())
		return errorInfo;

	/** Negative Sensor Level/Event 설정 */
	if ((errorInfo = SetSensorLimit(siAxisID, DEF_NEGATIVE_SENSOR, mAx.iNegativeEvent, mAx.bNegativeLevel, bBootOpt)).IsError())
		return errorInfo;

	/** Positive S/W Limit Level/Event 설정 */
	if ((errorInfo = SetSWLimit(siAxisID, DEF_POSITIVE_SENSOR, mAx.dPositiveSWLimit, mAx.iPositiveSWEvent, bBootOpt)).IsError())
		return errorInfo;

	/** Negative S/W Limit Level/Event 설정 */
	if ((errorInfo = SetSWLimit(siAxisID, DEF_NEGATIVE_SENSOR, mAx.dNegativeSWLimit, mAx.iNegativeSWEvent, bBootOpt)).IsError())
		return errorInfo;

	/** In-Position 위치결정값과 Level 설정 */
	if ((errorInfo = SetInPosition(siAxisID, mAx.dInpositionError, mAx.bInpositionLevel, bBootOpt)).IsError())
		return errorInfo;

	/** In-Position 사용여부 설정 설정 */
	if ((errorInfo = SetInpositionRequired(siAxisID, TRUE, bBootOpt)).IsError())
		return errorInfo;

	/** AMP Enable이었으면 Enable 설정 */
	if (bAmpState == DEF_ENABLE)
	{
		if ((errorInfo = SetAmpEnable(siAxisID, DEF_ENABLE)).IsError())
			return errorInfo;
	}

	return NO_ERROR_SUCESS;
}
/**
 * Motor Parameter 읽기
 *
 * @param	siAxisID		: 축 ID, -1=All Motor
 * @param	*pmAx			: Motor Parameter, siAxisID=-1이면 배열 구조로 구성
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = USED AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetMotorParam(short siAxisID, SMotionAxis *pmAx)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short siMotorType;
	short siFeedbackType;
	short siStepperPulseR;
	short siAmpFaultEvent;
	short siHomeEvent;
	short siPositiveEvent;
	short siNegativeEvent;
	short siPositiveSWEvent;
	short siNegativeSWEvent;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxisID < 1) || (siAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109); // ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pmAx == NULL)
		return generateErrorCode(106090); // ERR_MLIB_INVALID_POINTER

	/** Motor Parameter 읽기 */

	/** Motor 종류 읽기 */
	if ((errorInfo = GetMotorType(siAxisID, &siMotorType)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iMotorType = siMotorType;

	/** Motor Feed Device와 Lop 종류 읽기 */
	if ((errorInfo = GetAxisProperty(siAxisID, &siFeedbackType, &pmAx->bLoopType)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iFeedbackType = (int)siFeedbackType;

	/** Motor Type에 따라 */
	switch (pmAx->iMotorType)
	{
	case 0 :			// 속도형 Serve
		/** 속도형 Servo의 제어 모드와 출력 형태 읽기 */
		if ((errorInfo = GetVServoProperty(siAxisID, &pmAx->bVServoControl, &pmAx->bVServoPolar)) != NO_ERROR_SUCESS)
			return errorInfo;

		break;
	case 1 :				// 일반 Stepper
		/** 펄스 분주비와 전자기어비 읽기 */
		if ((errorInfo = GetRatioProperty(siAxisID, &siStepperPulseR, &pmAx->dStepperEGear)) != NO_ERROR_SUCESS)
			return errorInfo;
		pmAx->iStepperPulseR = (int)siStepperPulseR;

		break;
	case 2 :		// Micro Stepper 또는 위치형 Servo
		/** 펄스 분주비와 전자기어비 읽기 */
		if ((errorInfo = GetRatioProperty(siAxisID, &siStepperPulseR, &pmAx->dStepperEGear)) != NO_ERROR_SUCESS)
			return errorInfo;
		pmAx->iStepperPulseR = (int)siStepperPulseR;

		break;
	}

	/** 출력 펄스 형태 읽기 */
	if ((errorInfo = GetStepMode(siAxisID, &pmAx->bPulseType)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** Encoder 방향과 좌표 방향 읽기 */
	if ((errorInfo = GetEncoderDirection(siAxisID, &pmAx->bEncoderDir, &pmAx->bCoordinateDir)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** AMP Enable Level 읽기 */
	if ((errorInfo = GetAmpEnableLevel(siAxisID, &pmAx->bAmpEnableLevel)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** AMP Fault Level 읽기 */
	if ((errorInfo = GetAmpFaultLevel(siAxisID, &pmAx->bAmpFaultLevel)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** AMP Fault Event 읽기 */
	if ((errorInfo = GetAmpFaultEvent(siAxisID, &siAmpFaultEvent)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iAmpFaultEvent = siAmpFaultEvent;

	/** AMP Rest Level 읽기 */
	if ((errorInfo = GetAmpResetLevel(siAxisID, &pmAx->bAmpResetLevel)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** Home Sensor Level/Event 읽기 */
	if ((errorInfo = GetSensorLimit(siAxisID, 0, &siHomeEvent, &pmAx->bHomeLevel)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iHomeEvent = siHomeEvent;

	/** Positive Sensor Level/Event 읽기 */
	if ((errorInfo = GetSensorLimit(siAxisID, 1, &siPositiveEvent, &pmAx->bPositiveLevel)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iPositiveEvent = siPositiveEvent;

	/** Negative Sensor Level/Event 읽기 */
	if ((errorInfo = GetSensorLimit(siAxisID, 2, &siNegativeEvent, &pmAx->bNegativeLevel)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iNegativeEvent = siNegativeEvent;

	/** Positive S/W Limit Level/Event 읽기 */
	if ((errorInfo = GetSWLimit(siAxisID, 1, &pmAx->dPositiveSWLimit, &siPositiveSWEvent)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iPositiveSWEvent = siPositiveSWEvent;

	/** Negative S/W Limit Level/Event 읽기 */
	if ((errorInfo = GetSWLimit(siAxisID, 0, &pmAx->dNegativeSWLimit, &siNegativeSWEvent)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iNegativeSWEvent = siNegativeSWEvent;

	/** In-Position 위치결정값과 Level 읽기 */
	if ((errorInfo = GetInPosition(siAxisID, &pmAx->dInpositionError, &pmAx->bInpositionLevel)) != NO_ERROR_SUCESS)
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * Motion Board Parameter 설정
 *
 * @param	iObjectID		: Object ID
 * @param	MotionBd		: Motion Board Parameter
 * @param	iErrorBase		: Error Code Start Number
 * @param	strFullFileName	: Log File Path & File Name
 * @param	ucLevel			: Log LEvel (None, Normal, Warning, Error)
 * @param	iDays			: Log 우지 일 수
 * @return	Error Code		: 0 = SUCCESS
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetBoardParam(SMotionBoard MotionBd)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	/** Board Parameter 설정 */
	if ((errorInfo = m_pBoardConfig->SetBoardConfig(MotionBd)) != NO_ERROR_SUCESS)
		return errorInfo;


	/** Board 초기화 */
	if ((errorInfo = Initialize(MotionBd.iMaxBoardNo, MotionBd.rglAddress)) != NO_ERROR_SUCESS)
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * Motion Board Parameter 읽기
 *
 * @param	*pmBd			: Motion Board Parameter
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetBoardParam(SMotionBoard *pmBd)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	/** 인수 Pointer 오류 */
	if (pmBd == NULL)
		return generateErrorCode(106090);  // ERR_MLIB_INVALID_POINTER

	/** Board Parameter 읽기 */
	if ((errorInfo = m_pBoardConfig->GetBoardConfig(pmBd)) != NO_ERROR_SUCESS)
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * 구성된 Board 개수 읽기
 *
 * @param	*psiBdNum		: Board 구성 개수
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetBoardNum(short *psiBdNum)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 인수 Pointer 오류 */
	if (psiBdNum == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 구성된 Board 개수 읽기 */
	if ((errorInfo = m_pBoardConfig->GetBoardNum(psiBdNum)) != NO_ERROR_SUCESS)
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * Motion Board 초기화
 * 
 * 1. Motion Board 초기화가 수행된 경우
 * 	(1) Board 구성 수와 Board DPRAM Address가 같은 경우
 * 		→ Board 초기화 과정 Pass
 * 	(2) Board 구성 수가 다른 경우
 * 		→ Board 초기화 오류 처리
 * 	(3) Board DPRAM Address가 다른 경우
 * 		→ Board 초기화 오류 처리
 * 
 * 2. Motion Board 초기화가 수행되지 않은 경우
 * 	→ Board 초기화 수행
 *
 * NMC Library : MC_Init()
 * NMC Setup : 함수 구현 BoardInit()
 *
 * @param	siBdNum			: Motion Board 수 (1 ~ 4)
 * @param	*plAddress		: Board DPRAM Address (Board 수만큼)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID MOTION BOARD NUMBER (MOTIONLIB)
 *							  xx = NOT EQUAL MOTION BOARD NUMBER (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = NOT EQUAL MOTION DPRAM ADDRESS (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
//kde
ErrorInfo MRS_NMCLib::Initialize(short siBdNum, long *plAddress)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short siBdNumber;
	long  rglAddr[DEF_MAX_MOTION_BD];
	CString strLogMsg;

	/** 구성 Board 개수 읽기 */
	m_pBoardConfig->GetBoardNum(&siBdNumber, rglAddr);

	/** Board 개수 범위 오류 */
	if ((siBdNum < DEF_NON_MOTION_BD) || (siBdNum > siBdNumber))
		return generateErrorCode(106096);	// ERR_MLIB_INVALID_MOTION_BOARD_NUMBER


	/** 인수 Pointer 오류 */
	if (plAddress == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** Board 초기화가 되어 있으면 */
	if (m_pBoardConfig->GetBoardInitSuccess() == TRUE)
	{
		/** 설정되어 있는 Board 개수와 설정하려는 Board 개수가 다르면 */
		if (siBdNumber != siBdNum)
			return generateErrorCode(106072);	// ERR_MLIB_NOT_EQUAL_MOTION_BOARD_NUMBER


		for (int i = 0; i < siBdNum; i++)
		{
			if (plAddress[i] != rglAddr[i])
				return generateErrorCode(106071);	// ERR_MLIB_NOT_EQUAL_MOTION_DPRAM_ADDRESS
		}
	}
	/** Board 초기화가 안되어 있으면 */
	else
	{
		/** Board 개수, DPRAM Address 설정 */
		if ((errorInfo = m_pBoardConfig->SetBoardNum(siBdNum, plAddress)) != NO_ERROR_SUCESS)
			return errorInfo;

#ifndef SIMULATION
		errorInfo = MOlbSystemPre.InitRS_NMCBoard();
		if(errorInfo != NO_ERROR_SUCESS)
		{
			strLogMsg.Format(_T("Initialize(): BoardInit(NMC Setup Func) Fail - Error# is %d"), errorInfo);
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return errorInfo;
		}
#endif

		/** Motion Board 초기화 Flag 설정 */
		m_pBoardConfig->SetBoardInitSuccess(TRUE);
	}

	return NO_ERROR_SUCESS;
}
/**
 * Motion Board 초기화 여부 읽기
 *
 * @param	*pbInit			: Board 초기화 여부, TRUE=초기화됐음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetBoardInit(BOOL *pbInit)
{
	/** 인수 Pointer 오류 */
	if (pbInit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** Board 초기화 성공 Flag 읽기 */
	*pbInit = m_pBoardConfig->GetBoardInitSuccess();

	return NO_ERROR_SUCESS;
}

/**
	사용 안함
*/
ErrorInfo MRS_NMCLib::GetAxes(short siBdNum, short *psiAxes)
{
	CString strLogMsg;

	/** 인수 Pointer 오류 */
	if (psiAxes == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 모든 Board 선택 시 */
	if (siBdNum == DEF_ALL_MOTION_BD_ID)
	{
		*psiAxes = 1;
	}
	/** Board 1개 선택 시 */
	else
	{
		*psiAxes = 1;
	}

	return NO_ERROR_SUCESS;
}

/**
	사용 안함
 */
ErrorInfo MRS_NMCLib::MapAxes(short siAxNum, short *psiMapArray)
{
	return NO_ERROR_SUCESS;
}

/**
	사용 안함
 */
ErrorInfo MRS_NMCLib::Dwell(short siAxis, long *plDuration)
{
	return NO_ERROR_SUCESS;
}

/**
	사용 시, 구현 필
 */
ErrorInfo MRS_NMCLib::IOTrigger(short siAxis, short siBitNo, BOOL bState)
{
#ifndef SIMULATION
	//함수 구현 필요
#endif

	return NO_ERROR_SUCESS;
}

/**
 * 축 명령 수행에 대한 상태를 돌려준다.
 * 명령 수행 완료된 상태이면 SUCCESS를 Return한다.
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: 축 ID
 * @param	siType			: 조회 종류, 0=이동명령, 1=속도명령, 2=InPosition 범위내 여부
 * @return	Error Code		: 0 = SUCCESS요
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID IN COMMAND TYPE (MOTIONLIB)
 *							  xx = AXIS IS IN COMMAND (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::InCommand(short siAxis, BOOL *bState, short siType)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
	{
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	}	

	//MMC-ECAT 에서는 인포지션 관련한 모든 부분이 StandStill 상태 하나로 통합됨.
	//NMC Manager상에 보면 인포지션 범위가 펄스와 속도 두가지를 설정하도록 되어 있음.
	//속도 이동시에는 속도가 속도의 Window사이즈내에 들어와야 StandStill상태로 오게됨.
	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if(ms == MC_OK)
	{
		if(state & mcStandStill)
		{
			//모션 완료된 상태..
			*bState = FALSE;
		}
		else
		{
			//이동 명령중..
//			if(state & mcGearing)
			{
				*bState = TRUE;
			}
//			else
//			{
//				//모션 완료된 상태..
//				*bState = FALSE;
//			}
		}
	}
	else
	{
		return MakeRS_NMC_Error(ms);
	}

	return NO_ERROR_SUCESS;
}

/**
// RS-ECAT에서는 그룹모션으로 관리함. 필요시 작성 필요
*/
ErrorInfo MRS_NMCLib::AllDone (BOOL *pbStatus)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	
	/** 인수 Pointer 오류 */
	if (pbStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	// RS-ECAT에서는 그룹모션으로 관리함. 필요시 추후 협의후 작성할 필요 있음.
	*pbStatus = TRUE;

	return NO_ERROR_SUCESS;
}

/**
 * 축의 동작 완료 여부를 Return 한다.
 * 이동 및 속도 이동 완료 확인 또는 더불어 In-Position 여부까지 확인한다.
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: 축 ID
 * @return	: TRUE = AXIS_DONE
 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
 */
BOOL MRS_NMCLib::IsAxisDone(short siAxis)
{
#ifndef SIMULATION
	//MMC-ECAT 에서는 인포지션 관련한 모든 부분이 StandStill 상태 하나로 통합됨.
	UINT32 state = 0;

	MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if(state & mcStandStill)
	{
		//모션 완료된 상태..
		return TRUE;
	}
	else
	{
		//!(state | (mcConstantVelocity|mcAccelerating|mcDecelerating))
		//이동 명령중..
//		if(state & mcGearing)
		{
			return FALSE;
		}
//		else
//		{
//			return TRUE;
//		}
	}
#endif

	return TRUE;
}

/**
 * 축의 동작 완료 여부를 Return 한다.
 * 이동 및 속도 이동 완료 확인한다. InPosition 확인 안함
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: 축 ID
 * @return	: TRUE = AXIS_DONE
 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
 */

BOOL MRS_NMCLib::IsMotionDone(short siAxis)
{
#ifndef SIMULATION
	//MMC-ECAT 에서는 인포지션 관련한 모든 부분이 StandStill 상태 하나로 통합됨.
	UINT32 state = 0;

	MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if( state & mcStandStill)
	{
		//모션 완료된 상태..
		return TRUE;
	}
	else
	{
		//이동 명령중..
//		if(state & mcGearing)
		{
			return FALSE;
		}
//		else
//		{
//			return TRUE;
//		}
	}
#endif

	return TRUE;
}

/**
 * 지정 축이 동작 완료될 때까지 기다린다.
 * IsAxisDone()
 *
 * @param	siAxis			: 축 ID
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::WaitForDone(short siAxis)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	MTickTimer m_Timer;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
	{
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	}

#ifndef SIMULATION
	m_Timer.StartTimer();
	while(1)
	{
		if(IsAxisDone(siAxis) == TRUE)
		{
			break;
		}

		if(m_Timer.MoreThan(30))
		{
			return generateErrorCode(106121);
		}

		Sleep(5);
	}
#endif

	return NO_ERROR_SUCESS;
}
/**
 * 지정 축의 AMP Fault를 Clear하거나 Fault Port를 Enable 상태로 지정한다.
 *
 * NMC Library : MC_Reset()
 *
 * @param	siAxis			: 축 ID
 * @param	bState			: 지정할 상태, FALSE=Clear, TRUE=Enable
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetAmpFaultEnable(short siAxis, BOOL bState)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	/** 축 AMP Fault Reset 지정 */
	MC_STATUS ms;
	ms = MC_Reset(m_iBoardNo, siAxis);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetAmpFaultEnable(): Invalid Axis Number(AxisNo=%d is out of range)"), siAxis);
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif

	return NO_ERROR_SUCESS;
}
/**
 * 축 모든 현재 상태를 한꺼번에 읽는다.
 * psIstatus[0] = axis_source 함수값
 * psistatus[1] = in_sequence 함수값
 * psistatus[2] = get_com_velocity 함수값
 * psistatus[3] = get_act_velocity 함수값
 * psistatus[4] = motion_done 함수값
 * psistatus[5] = in_position 함수값
 * psistatus[6] = axis_done 함수값
 * 
 * plstatus[0] = get_io 함수값
 * 
 * pdstatus[0] = get_position 함수값
 * pdstatus[1] = get_command 함수값
 * pdstatus[2] = get_error 함수값
 *
 * NMC Library : axis_all_status()
 *
 * @param	siAxis			: 축 ID
 * @param	*psiState		: 상태, (axis_source, in_sequence, get_com_velocity, get_act_velocity,
 *									 motion_done, in_position, axis_done 결과값)
 * @param	*plStatus		: 상태, (get_io 결과값)
 * @param	*pdStatus		: 상태, (get_position, get_command, get_error 결과값)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::GetAllStatus(short siAxis, short *psiStatus, long *plStatus, double *pdStatus)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID


	/** 인수 Pointer 오류 */
	if (psiStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (plStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (pdStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

#ifndef SIMULATION
	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	
	if (ms != MC_OK)
	{
		strLogMsg.Format(_T("GetAllStatus(): MC_ReadAxisStatus(read all status of axes) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
	//실제 사용시에는 아래와 같이 사용함. 포함되어 있는 서보상태는 mcStandStill 를 따라서 정의내용을 찾아가면 됨.
	//if( state & mcStandStill) {;;;} else {;;;}
#else
	psiStatus[0] = 0x00;
	psiStatus[1] = 0x00;
	psiStatus[2] = 0x00;
	psiStatus[3] = 0x00;
	psiStatus[4] = 0x00;
	psiStatus[5] = 0x00;
	psiStatus[6] = 0x00;
	
	plStatus[0] = 0x00;
	
	pdStatus[0] = 0.00;
	pdStatus[1] = 0.00;
	pdStatus[2] = 0.00;
#endif
	
	return NO_ERROR_SUCESS;
}

/**
 * 축 현재 상태를 읽는다.
 *
 * 		ST_NONE					0x0000		정상동작
 * 		ST_HOME_SWITCH			0x0001		원점 센서 감지
 * 		ST_POS_LIMIT			0x0002		+ 방향 센서 감지
 * 		ST_NEG_LIMIT   			0x0004		- 방향 센서 감지
 * 		ST_AMP_FAULT			0x0008		AMP Fault 상태 발생
 * 		ST_A_LIMIT    			0x0010		가, 감속 시간이 limit보다 클 때 발생
 * 		ST_V_LIMIT  			0x0020		현재 속도가 속도 limit을 벗어날 때 발생
 * 		ST_X_NEG_LIMIT 			0x0040		- Position limit을 벗어날 때 발생
 * 		ST_X_POS_LIMIT			0x0080		+ Position limit을 벗어날 때 발생
 * 		ST_ERROR_LIMIT			0x0100		위치오차가 limit값보다 클 때 발생
 * 		ST_PC_COMMAND  			0x0200		EVENT가 발생된 상태일 때 발생
 * 		ST_OUT_OF_FRAMES    	0x0400		Frame Buffer가 Full 상태일 때 발생
 * 		ST_AMP_POWER_ONOFF  	0x0800		AMP Disable 상태일 때 발생
 * 		ST_ABS_COMM_ERROR   	0x1000		ABS Encoder 통신 Error 시 발생
 * 		ST_INPOSITION_STATUS	0x2000		??
 * 		ST_RUN_STOP_COMMAND 	0x4000 		??
 * 		ST_COLLISION_STATE  	0x8000		??
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: 축 ID
 * @param	*psiState		: 상태, 각 상태는 bit 조합으로 구성된다.
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::GetAxisSource(short siAxis, short *psiState)
{
#ifdef SIMULATION
	*psiState = 0x00;
#endif

	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (psiState == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);

	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	BOOL bELP_State			= state & mcLimitSwitchPos;
	BOOL bELN_State			= state & mcLimitSwitchNeg;
	BOOL bORG_State			= state & mcHomeAbsSwitch;
	BOOL bPowerOff_State	= !(state & mcPowerOn);
	BOOL bFault_State		= state & mcDriveFault;
	BOOL bErrorStop_State	= state & mcErrorStop;
	
	//170713 JSH.e
	/*/
	if(bFault_State)
	{
		*psiState = DEF_ST_AMP_FAULT;
		return NO_ERROR_SUCESS; 
	}
	else if(bELP_State)
	{
		*psiState = DEF_ST_POS_LIMIT;
		return NO_ERROR_SUCESS;
	}
	else if(bELN_State)
	{
		*psiState = DEF_ST_NEG_LIMIT;
		return NO_ERROR_SUCESS;
	}
	else if(bORG_State)
	{
		*psiState = DEF_ST_HOME_SWITCH;
		return NO_ERROR_SUCESS;
	}
	else if(bPowerOff_State)
	{
		*psiState = DEF_ST_AMP_POWER_ONOFF;
		return NO_ERROR_SUCESS;
	}
	else
	{
		*psiState = DEF_ST_NONE;
	}
	/*/
	short StateTemp = DEF_ST_NONE;

	if(bFault_State)
	{
		StateTemp |= DEF_ST_AMP_FAULT;
	}

	if(bErrorStop_State)
	{
		StateTemp |= DEF_ST_AMP_FAULT;
	}
	
	if(bELP_State)
	{
		StateTemp |= DEF_ST_POS_LIMIT;
	}

	if(bELN_State)
	{
		StateTemp |= DEF_ST_NEG_LIMIT;
	}
	
	if(bORG_State)
	{
		StateTemp |= DEF_ST_HOME_SWITCH;
	}
	
	if(bPowerOff_State)
	{
		StateTemp |= DEF_ST_AMP_POWER_ONOFF;
	}
	*psiState = StateTemp;
	//170713 JSH.e

	return NO_ERROR_SUCESS;
}

/**
 * 해동축의 동작중 속도명령값을 PULSE값으로 돌려준다.
 *
 * @param	siAxis			: 축 ID
 * @return	Pulse
 */
ErrorInfo MRS_NMCLib::GetComVelocity(short siAxis)
{
#ifdef SIMULATION
	return 0;
#endif

	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	//반환값이 double형임. 이 부분에 대해서 기존의 int 형 반환타입을 어떻게 할지...
	double dVel = 0.0;
	MC_ReadParameter(m_iBoardNo, siAxis, mcpCommandedVelocity, &dVel);

	return (int)dVel;
}

/**
 * 축의 센서(Home, Positive, Negative) 상태를 읽는다.
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: 축 ID
 * @param	siType			: 센서 종류, 0=Home, 1=Positive, 2=Negative
 * @param	*pbState		: 센서 상태, TRUE=Active, FALSE=No Active
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::GetSensorStatus(short siAxis, short siType, BOOL *pbState)
{
#ifdef SIMULATION
	pbState = FALSE;
	return NO_ERROR_SUCESS;
#endif

	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pbState == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	/** Sensor 상태 읽기 */
	switch (siType)
	{
	case DEF_HOME_SENSOR :		// Home Sensor

		if (state & mcHomeAbsSwitch)
		{
			*pbState = TRUE;
		}
		else
		{
			*pbState = FALSE;
		}

		break;

	case DEF_POSITIVE_SENSOR :	// Positive Sensor

		if (state & mcLimitSwitchPos)
		{
			*pbState = TRUE;
		}
		else
		{
			*pbState = FALSE;
		}

		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

		if (state & mcLimitSwitchNeg)
		{
			*pbState = TRUE;
		}
		else
		{
			*pbState = FALSE;
		}

		break;
		
	default :
		return generateErrorCode(106079);	// ERR_MLIB_INVALID_SENSOR_TYPE
	}

	return NO_ERROR_SUCESS;
}

/**
 * AMP Disable/Enable 상태를 읽는다. (Servo ON/OFF)
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: 축 ID
 * @param	*pbState		: AMP Enable 상태, TRUE=Enable, FALSE=Disable
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetAmpEnable(short siAxis, BOOL *pbState)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short	siState = 0;
	CString	strLogMsg;


	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pbState == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// Amp On 체크
	if(state & mcPowerOn)
	{
		if(state & mcErrorStop)
		{
			// Amp Fault 신호가 올라온 경우..
			*pbState = DEF_DISABLE;
		} 
		else
		{
			// 앰프가 정상적으로 ON 되어 있는 상태..
			*pbState = DEF_ENABLE;
		}
	}
	else
	{
		*pbState = DEF_DISABLE;
	}

	return NO_ERROR_SUCESS;
}

/**
 * AMP Enable의 Active Level을 읽는다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAmpEnableLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 축의 현재 Event 발생 상태를 읽는다.
 *
 *		NO_EVENT		0		Event 발생없이 정상 동작
 *		STOP_EVENT		1		stop_rate로 감속하면서 정지
 *		E_STOP_EVENT	2		e_stop_rate로 감속하면서 정지
 *		ABORT_EVENT		3		AMP disable 상태
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: 축 ID
 * @param	*psiState		: Event 내역, 0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
//int MRS_NMCLib::GetAxisState(short siAxis, short *psiState)
ErrorInfo MRS_NMCLib::GetAxisState(short siAxis, UINT32 *psiState) //170713 JSH
{
#ifdef SIMULATION
	*psiState = 0x00;
	return NO_ERROR_SUCESS;
#endif

	CString strLogMsg;


	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (psiState == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state); //170713 JSH.Mark
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	//state 가 4바이트 타입인데 기존에는 short 형 포인터로 넘어옴..
	//이 부분에 대한 추가적인 처리가 필요해 보임.
	*psiState = state;

	return NO_ERROR_SUCESS;
}

/**
 * Board의 Position Latch 여부를 읽는다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAxisLatchStatus(short siBdNum, BOOL *pbState)
{
	return NO_ERROR_SUCESS;
}

/**
 * 축에 발생된 Event를 해제하고, 다음 명령부터 실행한다.
 * Event 발생 후에는 항상 Event를 해제해 주어야 한다.
 * ABORT_EVENT 발생 시에는 Event 해제 후 AMP가 Disable 상태이므로 다시 Enable해주어야 한다.
 *
 * NMC Library : MC_Reset()
 *
 * @param	siAxis			: 축 ID
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::ClearStatus(short siAxis)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	MC_STATUS ms;
	ms = MC_Reset(m_iBoardNo, siAxis);
	if (ms != MC_OK)
	{
		strLogMsg.Format(_T("ClearStatus(): MC_Reset(clear all events) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif

	return NO_ERROR_SUCESS;
}
/**
 * 축의 Frame Buffer를 Clear한다.
 * 축별 최대 50개의 Frame의 내용을 Clear한다. 단 현재 실행중인 명령은 계속 수행된다.
 *
 * NMC Library : MC_Reset()
 *
 * @param	siAxis			: 축 ID
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::ClearFrames(short siAxis)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	MC_STATUS ms;
	ms = MC_Reset(m_iBoardNo, siAxis);
	if (ms != MC_OK)
	{
		strLogMsg.Format(_T("ClearFrames(): MC_Reset(clear frame buffers) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif

	return NO_ERROR_SUCESS;
}

/**
 * 축의 비어있는 Interpolation Frame 개수를 돌려준다.
 *
 * NMC Library : frames_interpolation()
 *
 * @param	siAxis			: 축 ID
 * @param	*psiFrameNo		: Frame 개수
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::FramesInterpolation(short siAxis, short *psiFrameNo)
{
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (psiFrameNo == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

#ifndef SIMULATION
	//지원안함. 다만 버퍼가 꽉 차있을경우 MC_ReadAxisStatus 로 상태 읽었을때 mcBufferFull 비트가 올라온다.
	//참고로 버퍼의 최대 크기는 20개로 지정되어 있음.
#endif

	return NO_ERROR_SUCESS;
}

/**
 * 축의 비어있는 Frame 개수를 돌려준다.
 *
 * NMC Library : frames_left()
 *
 * @param	siAxis			: 축 ID
 * @param	*psiFrameNo		: 비어있는 Frame 개수
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::FramesLeft(short siAxis, short *psiFrameNo)
{
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (psiFrameNo == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

#ifndef SIMULATION
	//지원안함. 다만 버퍼가 꽉 차있을경우 MC_ReadAxisStatus 로 상태 읽었을때 mcBufferFull 비트가 올라온다.
	//참고로 버퍼의 최대 크기는 20개로 지정되어 있음.
#endif

	return NO_ERROR_SUCESS;
}

/**
 * 해당 Board의 Latch 상태를 지정하고, Latch Status를 False로 만들거나, S/W적으로 Position을 Latch한다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::Latch(short siBdNum, BOOL bType, BOOL bState)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 Latch된 Position을 돌려준다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetLatchedPosition(short siAxis, double *pdPosition)
{
	*pdPosition = 0.00;

	return NO_ERROR_SUCESS;
}

/**
 * 동작중 목표위치를 재지정할 때 사용한다. (원, 원호 동작중에는 적용되지 않는다.)
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::CompensationPos(short siLen, short *psiAxes, double *pdDist, short *psiAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * Board DPRAM Address를 읽는다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetDpramAddress(short siBdNum, long *plAddr)
{
	*plAddr = 0xd8000000;

	return NO_ERROR_SUCESS;
}

/**
 * 절대치 Motor의 Type을 읽는다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAbsEncoderType(short siAxis, short *psiType)
{
	*psiType = 1;

	return NO_ERROR_SUCESS;
}

/**
 * 축의 이동 최고속도와 가,감속 구간값의 제한값을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : v()
 *
 * @param	siAxis			: 축 ID
 * @param	*pdVelocity		: 이동 최고속도, 1 ~ 2047000 coutn/sec
 * @param	*psiAccel		: 가,감속 구간값, 1 ~ 200, 10msec 단위
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetVelLimit(short siAxis, double *pdVelocity, short *psiAccel, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pdVelocity = 2047000.0;
	*psiAccel = 200;

	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pdVelocity == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (psiAccel == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	MC_STATUS ms;
	double fMaxVel;
	double fMaxAccel;

	//제한 속도
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpMaxVelocityAppl_, &fMaxVel);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);		

	//제한 가속도
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpMaxAccelerationAppl, &fMaxAccel);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	*pdVelocity = fMaxVel;
	*psiAccel = (short)fMaxAccel;

	return NO_ERROR_SUCESS;
}

/**
 * AMP Drive에 Fault 발생 상태를 읽는다.
 *
 * NMC Library : amp_fault_switch()
 *
 * @param	siAxis			: 축 ID
 * @param	*pbStatus		: AMP Fault 상태를 읽는다. TRUE=FAULT, FALSE=NORMAL
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::GetAmpFaultStatus(short siAxis, BOOL *pbStatus)
{
#ifdef SIMULATION
	*pbStatus = FALSE;
	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pbStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	//if (state & mcErrorStop)
	if (state & mcDriveFault) //170713 JSH
	{
		// Amp Fault 신호가 올라온 경우..
		*pbStatus = TRUE;
	} 
	else
	{
		// 앰프가 정상적으로 ON 되어 있는 상태..
		*pbStatus = FALSE;
	}

	return NO_ERROR_SUCESS;
}

/**
 * AMP Drive에 Fault 발생 시 동작할 Event를 읽는다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAmpFaultEvent(short siAxis, short *psiAction, BOOL bBootOpt)
{
	//Fault 발생시 무조건 Error Stop 모드로 가게 되어 있음
	*psiAction = 0x00;

	return NO_ERROR_SUCESS;
}

/**
 * AMP Enable의 Active Level을 읽는다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAmpEanbleLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt)
{
	*pbLevel = DEF_LOW;

	return NO_ERROR_SUCESS;
}

/**
 * AMP Fault의 Active Level을 읽는다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAmpFaultLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt)
{
	*pbLevel = DEF_LOW;
	
	return NO_ERROR_SUCESS;
}

/**
 * AMP Reset의 Active Level을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAmpResetLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt)
{
	*pbLevel = DEF_LOW;
	
	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 AMP Drive의 Resolution을 읽는다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAmpResolution(short siAxis, short *psiResolution, BOOL bBootOpt)
{
	*psiResolution = 2500;

	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 분주비에 대한 분자값, 분모값을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetEncoderRatio(short siAxis, short *psiRatioA, short *psiRatioB, BOOL bBootOpt)
{
	//이 부분 확인 필요함.
	*psiRatioA = 1;
	*psiRatioB = 1;

	return NO_ERROR_SUCESS;
}

/**
 * 지정 축이 회전/직선운동하는 무한회전 축인지 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : MC_ReadBoolParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	*pbStatus		: 무한회전 축 설정여부
 * @param	bType			: 운동 종류, FALSE=직선, TRUE=회전
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetEndlessAx(short siAxis, BOOL *pbStatus, BOOL bType, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pbStatus = FALSE;
	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	BOOL bStatus = 0;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pbStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER
	
	//ECAT 에서는 Module 기능으로 대체됨.
	//사용시에 로직이 맞는지 확인이 필요할것 같음.
	bool bData;
	MC_STATUS ms;
	ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpModuloAxis, &bData);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	if (bData == FALSE)
	{
		//모듈러 모드
		bStatus = TRUE;
	}
	else
	{
		//리니어 모드.
		bStatus = FALSE;
	}

	if (bStatus == FALSE)
		*pbStatus = FALSE;
	else
		*pbStatus = TRUE;

	return NO_ERROR_SUCESS;
}

/**
 * 무한회전 축의 움직이는 영역을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : MC_ReadParameter(),
 *
 * @param	siAxis			: 축 ID
 * @param	*pdRange		: 이동 영역
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetEndlessRange(short siAxis, double *pdRange, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pdRange = 0.0;
	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pdRange == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	//ECAT 에서는 Module 기능으로 대체됨.
	//사용시에 로직이 맞는지 확인이 필요할것 같음.
	double fData;
	MC_STATUS ms;
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpModuloValue, &fData);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	*pdRange = fData;

	return NO_ERROR_SUCESS;
}

/**
 * 축의 위치결정 완료값과 위치결정 시 신호 Level을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : MC_ReadParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	*pdInPosition	: 위치 결정값
 * @param	*pbLevel		: 신호 Level, TRUE=HIGH, FALSE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetInPosition(short siAxis, double *pdInPosition, BOOL *pbLevel, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pdInPosition = 10.0;
	pbLevel = DEF_LOW;

	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short siLevel = DEF_LOW;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pdInPosition == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (pbLevel == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	//인포지션 윈도우 사이즈는 지원하지만, 인포지션 센서레벨은 지원안함.
	MC_STATUS ms;
	double fInPos;
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpInPositionWindowSize, &fInPos);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	*pdInPosition = fInPos;

	// 레벨은 지원안함.
	*pbLevel = DEF_LOW;

	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 InPosition 신호 사용여부를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetInpositionRequired(short siAxis, BOOL *pbReq, BOOL bBootOpt)
{
	//지원안함. 무조건 사용하도록 되어 있음.
	*pbReq = TRUE;

	return NO_ERROR_SUCESS;
}
/**
 * 축의 위치오차 제한값과 Event를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : 
 *
 * @param	siAxis			: 축 ID
 * @param	*pdLimit		: 위치오차 제한값, 최대 35000 count
 * @param	*psiAction		: 위치오차 Event, NO EVENT, ESTOP EVENT, ABORT EVENT
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetErrorLimit(short siAxis, double *pdLimit, short *psiAction, BOOL bBootOpt)
{
#ifdef SIMULATION
	*psiAction = 0x00;

	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pdLimit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (psiAction == NULL)
	{
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER
	}

	//이 부분은 SDO접근 기능으로 대처할수 있음. 서보에 따라서 달라짐.
	//이 기능이 필요시 RSA 확인후 작성할 필요가 있음.
	*psiAction = 0x00;

	return NO_ERROR_SUCESS;
}

/**
 * 원점 복귀 시 Encoder의 C상 펄스 이용 여부를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetIndexRequired(short siAxis, BOOL *pbIndexReq, BOOL bBootOpt)
{
	// 지원안함
	// ECAT은 원점복귀 타입을 Ref으로 설정하면 자동으로 C(Z)상으로 원점복귀 동작
	*pbIndexReq = TRUE;

	return NO_ERROR_SUCESS;
}

/**
 * I/O 8점에 대한  입,출력 모드를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetIOMode(short siBdNum, BOOL *pbMode, BOOL bBootOpt)
{
	*pbMode = FALSE;
	
	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- 방향 Limit Switch Active시 동작할 Event와 신호 Level을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : MC_ReadBoolParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
 * @param	*psiLimit		: 동작할 Event
 * @param	*pbLevel		: 신호 Level, TRUE=HIGH, FALSE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetSensorLimit(short siAxis, short siType, short *psiLimit, BOOL *pbLevel, BOOL bBootOpt)
{
#ifdef SIMULATION
	pbLevel = DEF_LOW;
	*psiLimit = 0x00;

	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short siLevel = DEF_LOW;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (psiLimit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (pbLevel == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	bool bData;
	MC_STATUS ms;

	switch (siType)
	{
	case DEF_HOME_SENSOR :		// Home Sensor

		//홈센서 Active Level 취득.
		//이벤트는 무조건 Stop으로 가게됨. Stop에서 한번더 리밋방향으로 지령을 보내면 Error Stop임.
		ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpHomeActLevel, &bData);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);

		siLevel = bData;
		*psiLimit = 0x00;

		break;

	case DEF_POSITIVE_SENSOR :	// Positive Sensor

		//포지티브 리밋센서 Active Level 취득.
		//이벤트는 무조건 Stop으로 가게됨. Stop에서 한번더 리밋방향으로 지령을 보내면 Error Stop임.
		ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpHWLimitPosActLevel, &bData);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);
		siLevel = bData;
		*psiLimit = 0x00;
		
		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

		//네거티브 리밋센서 Active Level 취득.
		//이벤트는 무조건 Stop으로 가게됨. Stop에서 한번더 리밋방향으로 지령을 보내면 Error Stop임.
		ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpHWLimitNegActLevel, &bData);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);

		siLevel = bData;
		*psiLimit = 0x00;
		
		break;

	default :
		return generateErrorCode(106079);	// ERR_MLIB_INVALID_SENSOR_TYPE
	}

	if(siLevel == DEF_LOW)
		*pbLevel = DEF_LOW;
	else
		*pbLevel = DEF_HIGH;

	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- 방향 Limit Switch Active시 동작할 Event를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * @param	siAxis			: 축 ID
 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
 * @param	*psiLimit		: 동작할 Event
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetSensorEvent(short siAxis, short siType, short *psiLimit, BOOL bBootOpt)
{
#ifdef SIMULATION
	*psiLimit = 0x00;
	
	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (psiLimit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	switch (siType)
	{
	case DEF_HOME_SENSOR :		// Home Sensor

		//지원안함.
		//ECAT은 무조건 Stop으로 가게 되어 있으며, Stop상태에서 같은방향으로 더 밀면 Error Stop임.
		*psiLimit = 0x00;

		break;
	case DEF_POSITIVE_SENSOR :	// Positive Sensor

		//지원안함.
		//ECAT은 무조건 Stop으로 가게 되어 있으며, Stop상태에서 같은방향으로 더 밀면 Error Stop임.
		*psiLimit = 0x00;
		
		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

		//지원안함.
		//ECAT은 무조건 Stop으로 가게 되어 있으며, Stop상태에서 같은방향으로 더 밀면 Error Stop임.
		*psiLimit = 0x00;
		
		break;

	default :
		return generateErrorCode(106079);	// ERR_MLIB_INVALID_SENSOR_TYPE
	}

	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- 방향 Limit Switch Active시 동작할 신호 Level을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * @param	siAxis			: 축 ID
 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
 * @param	*pbLevel		: 신호 Level, TRUE=HIGH, FALSE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetSensorLevel(short siAxis, short siType, BOOL *pbLevel, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pbLevel = DEF_LOW;
	
	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short siLevel = DEF_LOW;
	

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
	{
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	}

	/** 인수 Pointer 오류 */
	if (pbLevel == NULL)
	{
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER
	}

	MC_STATUS ms;
	bool bLevel;

	switch (siType)
	{
	case DEF_HOME_SENSOR :		// Home Sensor
		
		ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpHomeActLevel, &bLevel);
		if(ms != MC_OK)
		{
			return MakeRS_NMC_Error(ms);
		}
		siLevel = bLevel;

		break;

	case DEF_POSITIVE_SENSOR :	// Positive Sensor

		ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpHWLimitPosActLevel, &bLevel);
		if(ms != MC_OK)
		{
			return MakeRS_NMC_Error(ms);
		}
		siLevel = bLevel;
		
		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

		ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpHWLimitNegActLevel, &bLevel);
		if(ms != MC_OK)
		{
			return MakeRS_NMC_Error(ms);
		}
		siLevel = bLevel;
		
		break;

	default :
		return generateErrorCode(106079);	// ERR_MLIB_INVALID_SENSOR_TYPE
	}

	if (siLevel == DEF_LOW)
		*pbLevel = DEF_LOW;
	else
		*pbLevel = DEF_HIGH;

	return NO_ERROR_SUCESS;
}

/**
 * +/- 방향으로 Motor가 이동할 수 있는 제한 위치값과 그 위치값에 도달했을 때 적용할 Event를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : MC_ReadParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	bType			: 이동 방향, TRUE=Positive, FALSE=Negative
 * @param	*pdPosition		: 제한 위치값
 * @param	*psiLimit		: 적용할 Event
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetSWLimit(short siAxis, BOOL bType, double *pdPosition, short *psiLimit, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pdPosition = 0.0;
	*psiLimit = 0;

	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pdPosition == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (psiLimit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** Positive S/W */
	if (bType == DEF_POSITIVE_SW)
	{
		//리밋시 액션에 대한 정보는 지원안함.
		MC_STATUS ms;
		double fLimit;
		ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpSWLimitPos_, &fLimit);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);

		*pdPosition = fLimit;
		*psiLimit = 0;
	}
	/** Negative S/W */
	else
	{
		//리밋시 액션에 대한 정보는 지원안함.
		MC_STATUS ms;
		double fLimit;
		ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpSWLimitNeg_, &fLimit);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);

		*pdPosition = fLimit;
		*psiLimit = 0;
	}

	return NO_ERROR_SUCESS;
}

/**
 * 해당 축이 어떤 Motor로 제어하는 축으로 지정되어 있는지 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetMotorType(short siAxis, short *psiType, BOOL bBootOpt)
{
	*psiType = DEF_SERVO_MOTOR;

	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 Feedback 장치와 Loop 형태를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetAxisProperty(short siAxis, short *psiDevice, BOOL *pbLoop, BOOL bBootOpt)
{
	*psiDevice = 0;
	*pbLoop = DEF_OPEN_LOOP;

	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 Pulse 분주비와 전자기어비를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : MC_ReadParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	*psiPgratio		: Pulse 분주비
 * @param	*pdEgratio		: 전자기어비
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetRatioProperty(short siAxis, short *psiPgratio, double *pdEgratio, BOOL bBootOpt)
{
#ifdef SIMULATION
	*psiPgratio = 8;
	*pdEgratio = 1.0;

	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (psiPgratio == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (pdEgratio == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	// ECAT 타입의 경우 스케일을 분자분모로 따로 가져가지 않고, 나눈 스케일값을 그대로 가져감.
	// 아래 함수에서 fFeedbackScaleFactor 값이 스케일(기어비)값임.
	MC_STATUS ms;
	double fFeedbackScaleFactor;
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpFeedbackScaleFactor, &fFeedbackScaleFactor);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	//이 부분 확인 필요함.
	*pdEgratio = fFeedbackScaleFactor;
	*psiPgratio = 8;

	return NO_ERROR_SUCESS;
}

/**
 * 속도형 Servo의 설정을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetVServoProperty(short siAxis, BOOL *pbControl, BOOL *pbPolar, BOOL bBootOpt)
{
	*pbControl = DEF_V_CONTROL;
	*pbPolar = DEF_UNIPOLAR;

	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 Pulse 출력 형태를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetStepMode(short siAxis, BOOL *pbMode, BOOL bBootOpt)
{
	*pbMode = DEF_SIGN_PULSE;

	return NO_ERROR_SUCESS;
}
/**
 * 지정 축의 Encoder 입력 방향과 좌표 방향을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * NMC Library : MC_ReadBoolParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	*pbEncDir		: Encoder 입력 방향, FALSE=ENCO_CW(시계방향, - Count)
 *												 TRUE =ENCO_CCW(반시계방향, + Count)
 * @param	*pbCoorDir		: 좌표방향, FALSE=CORD_CW(시계방향, + 좌표 이동)
 *										TRUE =CORD_CCW(반시계방향, - 좌표 이동)
 * @param	bBootOpt		: (OPTION=FALSE) boot file에서 읽을지 여부, TRUE=boot file에서 읽음
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetEncoderDirection(short siAxis, BOOL *pbEncDir, BOOL *pbCoorDir, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pbEncDir = DEF_CORD_CW;
	*pbCoorDir = DEF_CORD_CW;

	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short siEncDir = 0;
	short siCoorDir = 0;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	/** 인수 Pointer 오류 */
	if (pbEncDir == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 인수 Pointer 오류 */
	if (pbCoorDir == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	MC_STATUS ms;
	bool bComDir, bEncDir;
			
	//엔코더 방향 반전여부.
	ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpInvertFeedbackDir, &bEncDir);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	//지령방향 반전여부
	ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpInvertCmdDir, &bComDir);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	siEncDir = bEncDir;
	siCoorDir = bComDir;

	if (siEncDir == DEF_CORD_CW)
		*pbEncDir = DEF_CORD_CW;
	else
		*pbEncDir = DEF_CORD_CCW;

	if (siCoorDir == DEF_CORD_CW)
		*pbCoorDir = DEF_CORD_CW;
	else
		*pbCoorDir = DEF_CORD_CCW;

	return NO_ERROR_SUCESS;
}

/**
 * 지정된 축의 STOP EVENT, ESTOP EVENT 수행 시 감속 시간을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::GetStopRate(short siAxis, BOOL bType, short *psiRate, BOOL bBootOpt)
{
	*psiRate = 0;

	return NO_ERROR_SUCESS;
}

/**
 * 동기제어시 적용되는 보상 Gain값을 읽는다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetSyncGain(long *plCoeff)
{
	*plCoeff = 0;

	return NO_ERROR_SUCESS;
}
/**
 * 해당 축의 속도 또는 위치에 대한 PID & FF Gain값들을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt)
{
	*plGain = 0;

	return NO_ERROR_SUCESS;
}
/**
 * 해당 축의 적분제어 시 적분제어 모드를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetIntegration(short siAxis, BOOL bType, BOOL *pbMode, BOOL bBootOpt)
{
	*pbMode = FALSE;

	return NO_ERROR_SUCESS;
}
/**
 * 속도지령 혹은 토크 지령에 대해 Low Pass Filter 혹은 Notch Filter에 대한 Filter 값을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double *pdFilter, BOOL bBootOpt)
{
	*pdFilter = 0.0;

	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 동작 중 속도를 읽는다. (명령 값과 실제 값)
 *
 * NMC Library : MC_ReadActualVelocity(), MC_ReadParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	bType			: 속도 종류, FALSE=실제속도값, TRUE=속도명령값
 * @param	*psiPulse		: 속도의 Pulse값
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetVelocity(short siAxis, BOOL bType, double *pdPulse)
{
#ifdef SIMULATION
	*pdPulse = 0;

	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pdPulse == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	MC_STATUS ms;
	double dVel = 0.0;

	/** 실제 속도 */
	if(bType == FALSE)
	{
		ms = MC_ReadActualVelocity(m_iBoardNo, siAxis, &dVel);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);

		*pdPulse = dVel;
	}
	/** 명령 속도 */
	else
	{
		ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpCommandedVelocity, &dVel);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);

		*pdPulse = dVel;
	}

	return NO_ERROR_SUCESS;
}

/**
 * 지정된 Board의 축별 동작여부를 읽는다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetAxisRunStop(short siBdNum, short *psiState)
{
	*psiState = 0;
	return NO_ERROR_SUCESS;
}

/**
 * 축의 실제위치 및 목표위치를 읽는다.
 *
 * NMC Library : MC_ReadActualPosition(), MC_ReadParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	bCommandType	: 위치 종류, FALSE=실제위치, TRUE=목표위치
 * @param	*pdPosition		: bType=FALSE이면, 지정할 실제위치(Encoder)
 *							  bType=TRUE 이면, 지정할 목표위치(Command)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetPosition(short siAxis, BOOL bCommandType, double *pdPosition)
{//
#ifdef SIMULATION
	*pdPosition = 0.0;

	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 인수 Pointer 오류 */
	if (pdPosition == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** 실제위치(Encoder) */
	if (bCommandType == FALSE)
	{
		MC_STATUS ms;
		ms = MC_ReadActualPosition(m_iBoardNo, siAxis, pdPosition);
		if ( ms != MC_OK )
			return MakeRS_NMC_Error(ms);
	}
	/** 목표위치(Command) */
	else
	{
		MC_STATUS ms;
		ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpCommandedPosition, pdPosition);
		if(ms != MC_OK )
			return MakeRS_NMC_Error(ms);
	}

	return NO_ERROR_SUCESS;
}

/**
 * Motor의 지령치 RPM이나 실제 RPM을 읽는다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetRpm(short siAxis, BOOL bType, short *psiRpm)
{
	*psiRpm = 0;

	return NO_ERROR_SUCESS;
}

/**
 * Board별 Sampling Rate를 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetControlTimer(short siBdNum, short *psiTime, BOOL bBootOpt)
{
	*psiTime = 0;

	return NO_ERROR_SUCESS;
}

/**
 * 축의 목표위치와 실제위치의 차이값인 위치오차를 읽는다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetError(short siAxis, double *pdError)
{
	*pdError = 0.0;

	return NO_ERROR_SUCESS;
}

/**
 * 특정 축의 Encoder Feedback Data를 빠르게 읽어들일 때 사용 (50usec 주기 Update)
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetFastReadEncoder(short siAxis, BOOL *pbStatus)
{
	*pbStatus = FALSE;

	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 Analog Offset 값을 읽는다.
 * boot file 또는 실행중인 memory에서 읽을 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetAnalogOffset(short siAxis, short *psiOffset, BOOL bBootOpt)
{
	*psiOffset = 0;

	return NO_ERROR_SUCESS;
}

/**
 * 입, 출력 Port의 32bit Data를 읽는다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetIO(short siPort, BOOL bType, long *plValue)
{
	*plValue = 0;

	return NO_ERROR_SUCESS;
}

/**
 * 지정된 Analog 입/출력의 12/16bit Data 값을 읽는다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetAnalog(short siChannel, BOOL bType, short *psiValue)
{
	*psiValue = 0;

	return NO_ERROR_SUCESS;
}

/**
 * Board의 충돌방지 기능의 사용여부를 읽는다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetCollisionPreventFlag(short siBdNum, BOOL *pbMode)
{
	*pbMode = FALSE;

	return NO_ERROR_SUCESS;
}

/**
 * 동기제어 여부를 읽는다.
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	*pbState		: 지정 여부, TRUE=지정
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::GetSyncControl(short siAxis, BOOL *pbState)
{
#ifdef SIMULATION
	*pbState = FALSE;
	
	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 인수 Pointer 오류 */
	if (pbState == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	MC_STATUS ms;
	UINT32 state = 0;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if ( ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	if(state & mcGearing)
	{
		*pbState = TRUE;
	}
	else
	{
		*pbState = FALSE;
	}

	return NO_ERROR_SUCESS;
}

/**
 * Master축과 Slave축의 실제위치를 읽는다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetSyncPosition(double *pdMasterPos, double *pdSlavePos)
{
	*pdMasterPos = 0.0;
	*pdSlavePos = 0.0;

	return NO_ERROR_SUCESS;
}

/**
 * 보드별로 I/O Interrupt를 Enable/Diable하거나, I/O Interrupt 발생 시
 * STOP-EVENT나 E-STOP-EVENT를 지정축에 발생할지 여부를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::IOInterrupt(short siID, short siType, BOOL bState, BOOL bBootOpt)
{

	return NO_ERROR_SUCESS;
}
/**
 * I/O Interrupt 발생 시 PC쪽으로 Interrupt를 발생시킬지 여부를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::IOInterruptPCIRQ(short siBdNum, BOOL bState, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}
/**
 * PC Interrupt 발생 시 end of interrupt 신호를 발생시킨다.
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::IOInterruptPCIRQ_EOI(short siBdNum)
{
	return NO_ERROR_SUCESS;
}
/**
 * 지정 축의 PID 제어 여부를 지정한다.
 *
 * NMC Library : controller_idle(), controller_run()
 *
 * 지원 안
 */
ErrorInfo MRS_NMCLib::SetController(short siAxis, BOOL bState)
{
	return NO_ERROR_SUCESS;
}

/**
 * AMP Disable/Enable 상태를 설정한다.함
 *
 * NMC Library : MC_Power()
 *
 * @param	siAxis			: 축 ID
 * @param	bState			: AMP Enable 상태, TRUE=Enable
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetAmpEnable(short siAxis, BOOL bState)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	bool bSetState;
	if(bState == TRUE)
	{
		bSetState = true;
	}
	else
	{
		bSetState = false;
	}

#ifndef SIMULATION
	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_Power(m_iBoardNo, siAxis, bSetState);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetAmpEnable(): MC_Power(enable or disable AMP) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
	
	Sleep(10);

#endif
	
	return NO_ERROR_SUCESS;
}

/**
 * 축의 이동 최고속도와 가,감속 구간값의 Limit를 지정한다. (boot file에 자동 저장)
 *
 * NMC Library : MC_WriteParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	dVelocity		: 이동속도, 1 ~ 2047000 count/rev
 * @param	siAccel			: 가,감속구간값, 1 ~ 200, 10msec단위
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID VELOCITY VALUE (MOTIONLIB)
 *							  xx = INVALID ACCELERATE VALUE (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetVelLimit(short siAxis, double dVelocity, double dAccel, double dDecel)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** 이동속도 범위 오류 */
	if ((dVelocity < 0.0) || (dVelocity > DEF_VEL_LIMIT))
		return generateErrorCode(106076);	// ERR_MLIB_INVALID_VELOCITY_VALUE

	/** 이동 가,감속도 범위 오류 */
	if ((dAccel < 0) || (dAccel > DEF_ACCEL_LIMIT))
		return generateErrorCode(106112);	// ERR_MLIB_INVALID_ACCELERATE_VALUE

	/** 이동 가,감속도 범위 오류 */
	if ((dDecel < 0) || (dDecel > DEF_ACCEL_LIMIT))
		return generateErrorCode(106112);	// ERR_MLIB_INVALID_ACCELERATE_VALUE

	// **1202 -> msec 단위로 수정
	dAccel = (short)dAccel/4;
	// **050112
	if(dAccel <= 0) dAccel = 1;

	// **1202 -> msec 단위로 수정
	dDecel = (short)dDecel/4;
	// **050112
	if(dDecel <= 0) dDecel = 1;

#ifndef SIMULATION
	MC_STATUS ms;
	//Max Vel
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpMaxVelocityAppl_, dVelocity);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetVelLimit(): MC_WriteParameter(set velocity limit) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}

	//Max Acc
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpMaxAccelerationAppl, dAccel);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetVelLimit(): MC_WriteParameter(set accelerate limit) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}

	//Max Dec
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpMaxDecelerationAppl, dDecel);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetVelLimit(): MC_WriteParameter(set decelerate limit) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif

	return NO_ERROR_SUCESS;
}

/**
 * AMP Drive에 Fault 발생 시 동작할 Event를 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAmpFaultEvent(short siAxis, short siAction, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * AMP Enable의 Active Level을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAmpEnableLevel(short siAxis, BOOL bLevel, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * AMP Fault의 Active Level을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAmpFaultLevel(short siAxis, BOOL bLevel, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * AMP Reset의 Active Level을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAmpResetLevel(short siAxis, BOOL bLevel, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 AMP Drive의 Resolution을 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAmpResolution(short siAxis, short siResolution, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 분주비에 대한 분자값, 분모값을 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetEncoderRatio(short siAxis, short siRatioA, short siRatioB, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 축을 회전/직선운동하는 무한회전 축으로 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetEndlessAx(short siAxis, BOOL bStatus, short siResolution, BOOL bType, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 무한회전 축의 움직이는 영역을 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetEndlessRange(short siAxis, double dRange, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 원점 복귀 시 Encoder의 C상 펄스 이용 여부를 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetIndexRequired(short siAxis, BOOL bIndexReq, BOOL bBootOpt)
{
	//지원안함. 원점복귀 타입에 따라 C상 사용여부 설정됨.

	return NO_ERROR_SUCESS;
}

/**
 * 해당 축을 해당 Motor 종류로 제어하는 축으로 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetMotorType(short siAxis, short siType, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 Feedback 장치와 Loop 형태를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetAxisProperty(short siAxis, short siDevice, BOOL bLoop, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 분주비와 전자기어비를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetRatioProperty(short siAxis, short siPgratio, double dEgratio, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 속도형 Servo의 설정을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetVServoProperty(short siAxis, BOOL bControl, BOOL bPolar, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 Pulse 출력 형태를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetStepMode(short siAxis, BOOL bMode, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 Encoder 입력 방향과 좌표 방향을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * NMC Library : MC_WriteParameter()
 * 읽기 전용이므로 지원 안됨
 *
 * @param	siAxis			: 축 ID
 * @param	bEncDir			: Encoder 입력 방향, FALSE=ENCO_CW(시계방향, - count),
 *												 TRUE =ENCO_CCW(반시계방향, + count)
 * @param	bCoorDir		: 좌표방향, FALSE=CORD_CW(시계방향, +좌표이동),
 *										TRUE =CORD_CCW(반시계방향, -좌표이동)
 * @param	bBootOpt		: (OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetEncoderDirection(short siAxis, BOOL bEncDir, BOOL bCoorDir, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
#if 0 
	//지원안함. 읽기전용 속성으로 되어 있음.
	MC_STATUS ms;
	//엔코더 방향 반전여부.
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpInvertFeedbackDir, (double)bEncDir);
	if (ms != MC_OK)
	{
		strLogMsg.Format(_T("SetEncoderDirection(): MC_WriteParameter(set an encoder input direction and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}

	//지령방향 반전여부
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpInvertCmdDir, (double)bCoorDir);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetEncoderDirection(): MC_WriteParameter(set a coordinate direction and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif
#endif

	return NO_ERROR_SUCESS;
}

/**
 * I/O 8점에 대한  입,출력 모드를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetIOMode(short siBdNum, BOOL bMode, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 축의 위치결정 완료값과 위치결정 시 신호 Level을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * NMC Library : MC_WriteParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	dInPosition		: 위치 결정값
 * @param	bLevel			: 신호 Level, TRUE=HIGH, FALSE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetInPosition(short siAxis, double dInPosition, BOOL bLevel, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	MC_STATUS ms;
	//인포지션 사이즈 설정.
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpInPositionWindowSize, dInPosition);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetInPosition(): MC_WriteParameter(set 'in_position' value and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
	
	//위치 결정 완료 레벨설정은 지원안함.
#endif

	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 InPosition 신호 사용여부를 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetInpositionRequired(short siAxis, BOOL bReq, BOOL bBootOpt)
{
	//지원안함. 무조건 사용하도록 되어 있음.

	return NO_ERROR_SUCESS;
}

/**
 * 축의 위치오차 Limit값과 Event를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetErrorLimit(short siAxis, double dLimit, short siAction, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정된 축의 STOP EVENT, ESTOP EVENT 수행 시 감속 시간을 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원안함
 */
ErrorInfo MRS_NMCLib::SetStopRate(short siAxis, BOOL bType, short siRate, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- 방향 Limit Switch Active시 동작할 Event와 신호 Level을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * NMC Library : MC_WriteBoolParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
 * @param	siLimit			: 동작할 Event
 * @param	bLevel			: 신호 Level, TRUE=HIGH, FLASE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetSensorLimit(short siAxis, short siType, short siLimit, BOOL bLevel, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	bool bSetState;
	if(bLevel == TRUE)
	{
		bSetState = true;
	}
	else
	{
		bSetState = false;
	}
	
	MC_STATUS ms = MC_OK;

	switch (siType)
	{
	case DEF_HOME_SENSOR :		// Home Sensor

#ifndef SIMULATION
		//레벨설정은 지원하나, 이벤트는 지원안함.
		ms = MC_WriteBoolParameter(m_iBoardNo, siAxis, mcpHomeActLevel, bSetState);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSensorLimit(): MC_WriteBoolParameter(set an Active Level of home sensor and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif

		break;

	case DEF_POSITIVE_SENSOR :	// Positive Sensor

#ifndef SIMULATION
		//레벨설정은 지원하나, 이벤트는 지원안함.
		ms = MC_WriteBoolParameter(m_iBoardNo, siAxis, mcpHWLimitPosActLevel, bSetState);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSensorLimit(): MC_WriteBoolParameter(set an Active Status of positive sensor and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif

		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

#ifndef SIMULATION
		//레벨설정은 지원하나, 이벤트는 지원안함.
		ms = MC_WriteBoolParameter(m_iBoardNo, siAxis, mcpHWLimitNegActLevel, bSetState);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSensorLimit(): MC_WriteBoolParameter(set an Active Status of negative sensor and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif

		break;

	default :
		return generateErrorCode(106079);	// ERR_MLIB_INVALID_SENSOR_TYPE
	}

	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- 방향 Limit Switch Active시 동작할 Event를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetSensorEvent(short siAxis, short siType, short siLimit, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	bool bSetState;
	if(siLimit == 0)
	{
		bSetState = FALSE;
	}
	else
	{
		bSetState = TRUE;
	}
	
	MC_STATUS ms = MC_OK;

	switch (siType)
	{
	case DEF_POSITIVE_SENSOR :	// Positive Sensor

#ifndef SIMULATION
		ms = MC_WriteBoolParameter(m_iBoardNo, siAxis, mcpEnableHWLimitPos, bSetState);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSensorLevel(): MC_WriteBoolParameter(set an Active Level of positive sensor, and save in boot file) Fail[AxisID=%d]- Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif

		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

#ifndef SIMULATION
		ms = MC_WriteBoolParameter(m_iBoardNo, siAxis, mcpEnableHWLimitNeg, bSetState);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSensorLevel(): MC_WriteBoolParameter(set an Active Level of negative sensor, and save in boot file) Fail[AxisID=%d]- Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif

		break;

	default :
		return generateErrorCode(106079);	// ERR_MLIB_INVALID_SENSOR_TYPE
	}

	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- 방향 Limit Switch Active시 동작할 신호 Level을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * @param	siAxis			: 축 ID
 * @param	siType			: Sensor 종류, 0=Home, 1=Positive, 2=Negative
 * @param	bLevel			: 신호 Level, TRUE=HIGH, FLASE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetSensorLevel(short siAxis, short siType, BOOL bLevel, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	bool bSetState;
	if(bLevel == TRUE)
	{
		bSetState = true;
	}
	else
	{
		bSetState = false;
	}
	
	MC_STATUS ms = MC_OK;

	switch (siType)
	{
	case DEF_HOME_SENSOR :		// Home Sensor

#ifndef SIMULATION
		ms = MC_WriteBoolParameter(m_iBoardNo, siAxis, mcpHomeActLevel, bSetState);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSensorLevel(): MC_WriteBoolParameter(set an Active Level of home sensor, and save in boot file) Fail[AxisID=%d]- Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif

		break;

	case DEF_POSITIVE_SENSOR :	// Positive Sensor

#ifndef SIMULATION
		ms = MC_WriteBoolParameter(m_iBoardNo, siAxis, mcpHWLimitPosActLevel, bSetState);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSensorLevel(): MC_WriteBoolParameter(set an Active Level of positive sensor, and save in boot file) Fail[AxisID=%d]- Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif

		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

#ifndef SIMULATION
		ms = MC_WriteBoolParameter(m_iBoardNo, siAxis, mcpHWLimitNegActLevel, bSetState);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSensorLevel(): MC_WriteBoolParameter(set an Active Level of negative sensor, and save in boot file) Fail[AxisID=%d]- Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif

		break;

	default :
		return generateErrorCode(106079);	// ERR_MLIB_INVALID_SENSOR_TYPE
	}

	return NO_ERROR_SUCESS;
}

/**
 * +/- 방향으로 Motor가 이동할 수 있는 Limit 위치값과 그 위치값에 도달했을 때 적용될 Event를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * NMC Library : MC_WriteParameter()
 *
 * @param	siAxis			: 축 ID
 * @param	bType			: 방향, FALSE=Negative, TRUE=Positive
 * @param	dPosition		: 제한 위치값, +/-2147483647
 * @param	siLimit			: 적용될 Event
 * @param	bBootOpt		: (OPTION=FALSE) boot file에 저장 여부, TRUE=boot file에 저장
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetSWLimit(short siAxis, BOOL bType, double dPosition, short siLimit, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** Positive S/W 설정 */
	if (bType == DEF_POSITIVE_SW)
	{
#ifndef SIMULATION
		MC_STATUS ms;
		ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpSWLimitPos, motionRoundValue(dPosition));
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSWLimit(): MC_WriteParameter(set a positive S/W limit & event, and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif
	}
	/** Negative S/W 설정 */
	{
#ifndef SIMULATION
		MC_STATUS ms;
		ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpSWLimitNeg, motionRoundValue(dPosition));
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("SetSWLimit(): MC_WriteParameter(set a negative S/W limit & event, and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
#endif
	}

	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 속도 또는 위치에 대한 PID & FF Gain 값들을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 적분 제어 시 적분 제어 모드를 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetIntegration(short siAxis, BOOL bType, BOOL bMode, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 속도지령 혹은 토크 지령에 대해 Low Pass Filter 혹은 Notch Filter에 대한 Filter 값을 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double dFilter, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정된 Board의 축별 동작 여부를 설정한다.
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAxisRunStop(short siBdNum, short siState)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 I/O bit를 HIGH(1)/LOW(0) 상태로 만든다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetBit(short siBitNo, BOOL bValue)
{
	return NO_ERROR_SUCESS;
}

/**
 * 32bit의 I/O Data를 출력 Port를 통해 내보낸다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetIO(short siPort, long lValue)
{
	return NO_ERROR_SUCESS;
}

/**
 * 12/16 bit Analog 출력전압을 내보낸다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetDacOut(short siAxis, short siValue)
{
	return NO_ERROR_SUCESS;
}

/**
 * 해당 축의 Analog Offset값을 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAnalogOffset(short siAxis, short siOffset, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 축의 출력전압의 범위를 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAnalogLimit(short siAxis, short siLimit, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 축의 실제 위치 및 목표 위치를 지정한다. (AMP Disable 상태에서 수행하는게 좋다.)
 *
 * NMC Library : set_command(), set_position()
 *
 * @param	siAxis			: 축 ID
 * @param	bType			: 위치 종류, FALSE=실제위치, TRUE=목표위치
 * @param	dPosition		: bType=FALSE이면 지정할 실제위치, bType=TRUE이면 지정할 목표위치
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetPosition(short siAxis, BOOL bType, double dPosition)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	MC_STATUS ms;
	//ECAT은 지령위치와 실제위치를 동일하게 가지고감.
//	ms = MC_SetPosition(iBoardID, siAxis, motionRoundValue(dPosition / 4.0), false, mcImmediately);
	ms = MC_SetPosition(m_iBoardNo, siAxis, motionRoundValue(dPosition), false, mcImmediately);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetPosition(): set_command(set a target position) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif

	return NO_ERROR_SUCESS;
}

/**
 * 특정 축의 Encoder Feedback Data를 빠르게 읽어들일 때 사용 (50usec 주기 Update)
 * 사용 안함
*/
ErrorInfo MRS_NMCLib::SetFastReadEncoder(short siAxis, BOOL bStatus)
{
	return NO_ERROR_SUCESS;
}
/**
 * 사용자가 Motion 관련 S/W를 자체 개발하여 시스템을 동작시킬 수 있도록 지원해주는 기능
 * 사용 안함
 */
ErrorInfo MRS_NMCLib::SetInterpolation(short siLen, short *psiAxes, long *plDelt, short siFlag)
{
	return NO_ERROR_SUCESS;
}

/**
 * 충돌방지 기능을 사용할 Mastr/Slave축 및 충돌방지 거리 및 조건 (+, -, >, <)을 설정한다.
 * 사용 안함
*/
ErrorInfo MRS_NMCLib::SetCollisionPrevent(short siMasterAx, short siSlaveAx,
									BOOL bAddSub, BOOL bNonEqual, double dPosition, BOOL bState)
{
	return NO_ERROR_SUCESS;
}

/**
* 충돌방지 기능의 사용여부를 설정한다.
* 사용 안함
 */
ErrorInfo MRS_NMCLib::SetCollisionPreventFlag(short siBdNum, BOOL bMode)
{
	return NO_ERROR_SUCESS;
}

/**
 * Board DPRAM Address를 설정한다.
 * 사용 안함
*/
ErrorInfo MRS_NMCLib::SetDpramAddress(short siBdNum, long lAddr)
{
	return NO_ERROR_SUCESS;
}

/**
 * 절대치 Motor의 Type을 지정한다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAbsEncoderType(short siAxis, short siType)
{
	return NO_ERROR_SUCESS;
}

/**
 * 절대치 Motor를 설정한다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetAbsEncoder(short siAxis)
{
	return NO_ERROR_SUCESS;
}

/**
 * Servo Linear Flag 상태를 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetServoLinearFlag(short siAxis, short siFlag, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}
/**
 * 동기제어 여부를 지정한다.
 *
 * 함수 따로 만들어서 제어
 */
ErrorInfo MRS_NMCLib::SetSyncControl(BOOL bState)
{
	return NO_ERROR_SUCESS;
}

/**
 * 동기제어할 Master축과 Slave축을 지정한다.
 *
 * 따로 함수 만들어서 제어
 */
ErrorInfo MRS_NMCLib::SetSyncMapAxes(short siMasterAx, short siSlaveAx)
{
	return NO_ERROR_SUCESS;
}

/**
 * 동기제어시 적용되는 보상 Gain값을 지정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetSyncGain(short siCoeff, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * Board별 Sampling Rate를 설정한다.
 * boot file 또는 실행중인 memory에 저장할 수 있다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetControlTimer(short siBdNum, short siTime, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/*
// MMC Board Type에 따라 Sampling Time 내부 코딩
// Sampling Time을 4ms로 내부 코딩한다.
// 지원 안함
*/
ErrorInfo MRS_NMCLib::SetControlTimerForAx(short siAxis, short siTime, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 직선 동작 혹은 직선, 원, 원호등의 동작 시 속도와 가,감속도를 지정한다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetMoveSpeed(double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}
/**
 * 자동 가,감속 기능의 사용여부를 지정한다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetSplAutoOff(short siBdNum, BOOL bState)
{
	return NO_ERROR_SUCESS;
}
/**
 * 자동 가,감속 기능의 사용여부를 읽는다.
 * Library에 제공되는 함수가 없는 관계로 설정 Data에서 읽어온다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::GetSplAutoOff(short siBdNum, BOOL *pbState)
{
	return NO_ERROR_SUCESS;
}

/*
 * 축 이동 정지 (V-Stop) 이벤트를 날린다. 
 */
ErrorInfo MRS_NMCLib::SetStopEvent(short siAxis)
{
	return NO_ERROR_SUCESS;
}

/**
 * 축 이동을 정지한다.
 * 일반정지, 비상정지, 속도이동정지를 제공한다.
 *
 * NMC Library : MC_Halt()
 *
 * @param	siAxis			: 축 ID
 * @param	siType			: 정지 종류, 0=STOP, 1=ESTOP, 2=VSTOP
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID STOP TYPE (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SetStop(short siAxis, short siType, double fDecel, double fJerk)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	MTickTimer m_Timer;


	if(fDecel < 100)
	{
		fDecel = 100;
	}

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	MC_STATUS ms;
	ms = MC_Halt(m_iBoardNo, siAxis, fDecel, fJerk, mcAborting);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetStop(): MC_Halt(generate STOP Event and stop the movement of an axis) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif

	return NO_ERROR_SUCESS;
}

/**
 * 1축 속도 Profile 이동 (축 1개 단위만 이동 가능함)
 *
 * NMC Library : MC_MoveAbsolute()
 *
 * @param	siAxis			: 축 ID
 * @param	dPosition		: 이동할 위치, 혹은 상대거리
 * @param	dVelocity		: 이동 속도
 * @param	siAccel			: 이동 가속도
 * @param	siDecel			: 이동 감속도 (비대칭(t) Type만 적용)
 * @param	siType			: 이동 Type, 0=사다리꼴 속도 Profile, 절대좌표 이동
 *										 1=S-Curve 속도 Profile, 절대좌표 이동
 *										 2=사다리꼴 속도 Profile, 상대거리 이동
 *										 3=S-Curve 속도 Profile, 상대거리 이동
 *										 4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
 *										 5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
 *										 6=비대칭 사다리꼴 속도 Profile, 상대거리 이동
 *										 7=비대칭 S-Curve 속도 Profile, 상대거리 이동
 * @param	bWaitOpt		: (OPTION=FALSE) 이동 완료 대기 여부, TRUE=이동완료될때까지대기
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID MOVE TYPE (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::Move(short siAxis, double dPosition, double dVelocity, double dAccel, double dDecel, short siType, BOOL bWaitOpt, double dJerk)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	// **050112
	if(dAccel < 1) dAccel = 1;
	if(dDecel < 1) dDecel = 1;

	MC_STATUS ms = MC_OK;

	/** 이동 종류에 따라 */
	switch (siType)
	{
	case DEF_MOVE_POSITION :		// 사다리꼴 속도 Profile, 절대좌표 이동

#ifndef SIMULATION
		ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcPositiveDirection, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveAbsolute(trapezoid velocity profile, absolute coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** 이동완료 대기 */
		if(bWaitOpt == TRUE)
		{
			Sleep(10);

			serrorInfo = WaitForDone(siAxis);
			if(serrorInfo != NO_ERROR_SUCESS)
			{
				return serrorInfo;
			}
		}
#endif
		break;

	case DEF_SMOVE_POSITION :		// S-Curve 속도 Profile, 절대좌표 이동

#ifndef SIMULATION
		ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcPositiveDirection, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveAbsolutestart_move(trapezoid velocity profile, absolute coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** 이동완료 대기 */
		if(bWaitOpt == TRUE)
		{
			Sleep(10);

			serrorInfo = WaitForDone(siAxis);
			if(serrorInfo != NO_ERROR_SUCESS)
			{
				return serrorInfo;
			}
		}
#endif
		break;

	case DEF_MOVE_DISTANCE :		// 사다리꼴 속도 Profile, 상대거리 이동

#ifndef SIMULATION
		ms = MC_MoveRelative(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveRelative(trapezoid velocity profile, relative coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** 이동완료 대기 */
		if(bWaitOpt == TRUE)
		{
			Sleep(10);

			serrorInfo = WaitForDone(siAxis);
			if(serrorInfo != NO_ERROR_SUCESS)
			{
				return serrorInfo;
			}
		}
#endif		
		break;

	case DEF_SMOVE_DISTANCE :		// S-Curve 속도 Profile, 상대거리 이동

#ifndef SIMULATION
		ms = MC_MoveRelative(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveRelative(S-Curve velocity profile, relative coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** 이동완료 대기 */
		if(bWaitOpt == TRUE)
		{
			Sleep(10);

			serrorInfo = WaitForDone(siAxis);
			if(serrorInfo != NO_ERROR_SUCESS)
			{
				return serrorInfo;
			}
		}
#endif
		break;

	case DEF_TMOVE_POSITION :		// 비대칭 사다리꼴 속도 Profile, 절대좌표 이동

#ifndef SIMULATION
		ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcPositiveDirection, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveAbsolute(asymmetry trapezoid velocity profile, absolute coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** 이동완료 대기 */
		if(bWaitOpt == TRUE)
		{
			Sleep(10);

			serrorInfo = WaitForDone(siAxis);
			if(serrorInfo != NO_ERROR_SUCESS)
			{
				return serrorInfo;
			}
		}
#endif
		break;

	case DEF_TSMOVE_POSITION :		// 비대칭 S-Curve 속도 Profile, 절대좌표 이동

#ifndef SIMULATION
		ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcPositiveDirection, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveAbsolute(asymmetry S-Curve velocity profile, absolute coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** 이동완료 대기 */
		if(bWaitOpt == TRUE)
		{
			Sleep(10);

			serrorInfo = WaitForDone(siAxis);
			if(serrorInfo != NO_ERROR_SUCESS)
			{
				return serrorInfo;
			}
		}
#endif
		break;

	case DEF_TMOVE_DISTANCE :		// 비대칭 사다리꼴 속도 Profile, 상대거리 이동

#ifndef SIMULATION
		ms = MC_MoveRelative(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveRelativet_move(asymmetry trapezoid velocity profile, relative coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** 이동완료 대기 */
		if(bWaitOpt == TRUE)
		{
			Sleep(10);

			serrorInfo = WaitForDone(siAxis);
			if(serrorInfo != NO_ERROR_SUCESS)
			{
				return serrorInfo;
			}
		}
#endif
		break;

	case DEF_TSMOVE_DISTANCE :		// 비대칭 S-Curve 속도 Profile, 상대거리 이동

#ifndef SIMULATION
		ms = MC_MoveRelative(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveRelative(asymmetry S-Curve velocity profile, relative coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** 이동완료 대기 */
		if(bWaitOpt == TRUE)
		{
			Sleep(10);

			serrorInfo = WaitForDone(siAxis);
			if(serrorInfo != NO_ERROR_SUCESS)
			{
				return serrorInfo;
			}
		}
#endif
		break;

	default :
		return generateErrorCode(106092);	// ERR_MLIB_INVALID_MOVE_TYPE
	}

	return NO_ERROR_SUCESS;
}

/**
 * 다축 속도 Profile 동시 이동 (상대거리 이동은 지원하지 않는다.)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+-----+---------+
 *		| 속도 #1 | 속도 #2 | 속도 #3 | ... | 속도 #n |
 *		+---------+---------+---------+-----+---------+
 *		| 가속 #1 | 가속 #2 | 가속 #3 | ... | 가속 #n |
 *		+---------+---------+---------+-----+---------+
 *		| 감속 #1 | 감속 #2 | 감속 #3 | ... | 감속 #n |
 *		+---------+---------+---------+-----+---------+
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::MoveAll(short siLen, short *psiAxes, double *pdPosition, double *pdVelocity, short *psiAccel, short *psiDecel, short siType, BOOL bWaitOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정된 n축이 주어진 좌표값만큼 직선이동을 한다. (다른 Board의 축 사용 불가)
 *  MapAxes(), SetMoveSpeed()에서 지정된 축들이 이동한다.
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::MoveN(short siLen, double *pdPosition, short siType)
{
	return NO_ERROR_SUCESS;
}

ErrorInfo MRS_NMCLib::MoveNAx(short siLen, short *psiAxes, double *pdPosition, short siType, double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정된 그룹의 축들이 주어진 좌표값만큼 직선이동을 한다.
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| 속도 | 가속도 |
 *		+------+--------+
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::MoveNAxGr(short siGrpNum, short siLen, short *psiAxes, double *pdPosition,
						short siType, double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * 가속 후 등속 이동한다. (축 1개 단위로만 동작 가능하다.)
 *
 * NMC Library : MC_MoveVelocity()
 *
 * @param	siAxis			: 축 ID
 * @param	dVelocity		: 이동 속도
 * @param	siAccel			: 이동 가속도
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::VMove(short siAxis, double dVelocity, double dAccel, double dDecel, double dJerk)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	if(dAccel <= 1) dAccel = 1;
	if(dDecel <= 1) dDecel = 1;

#ifndef SIMULATION
	MC_STATUS ms;
	
	if(dVelocity < 0)
	{
		dVelocity = dVelocity * -1.0;

		ms = MC_MoveVelocity(m_iBoardNo, siAxis, motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcNegativeDirection, mcAborting);
		if (ms != MC_OK)
		{
			strLogMsg.Format(_T("VMove(): MC_MoveVelocity(uniform velocity after acceleration) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
	}
	else
	{
		ms = MC_MoveVelocity(m_iBoardNo, siAxis, motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcPositiveDirection, mcAborting);
		if (ms != MC_OK)
		{
			strLogMsg.Format(_T("VMove(): MC_MoveVelocity(uniform velocity after acceleration) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}
	}

	
#endif

	return NO_ERROR_SUCESS;
}

/**
 * 현재 위치에서 주어진 2/3차원 평면상의 좌표점까지 가,감속하면서 원호 CP Motion으로 이동한다.
 *  (다른 Board의 축 사용 불가)
 *  MapAxes(), SetMoveSpped()에서 지정된 축들이 이동한다.
 *
 *		|---------------> siAxNum = n <---------------|
 *		+---------+---------+---------+-----+---------+
 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+-----+---------+
 *		+-----------+-----------+------+--------+----------+
 *		| 회전중심X | 회전중심Y | 속도 | 가속도 | 회전방향 |
 *		+-----------+-----------+------+--------+----------+
 *
 * NMC Library : spl_arc_move2(), spl_arc_move3()
 *
 * @param	siAxNum			: 축 수, 2=2축, 3=3축
 * @param	dCenterX		: 회전 중심 X좌표
 * @param	dCenterY		: 회전 중심 Y좌표
 * @param	*pdPoint		: 이동할 좌표, 2축이면 2차원배열, 3축이면 3차원배열
 * @param	dVelocity		: 이동 속도 (0.0=SetMoveSpeed()에서 지정한 속도 사용)
 * @param	siAccel			: 이동 가속도 (0=SetMoveSpeed()에서 지정한 가,감속도 사용)
 * @param	bDir			: 회전방향, FALSE=CIR_CW(시계방향), TRUE=CIR_CCW(반시계방향)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SplArcMove(short siAxNum, double dCenterX, double dCenterY, double *pdPoint, double dVelocity, short siAccel, BOOL bDir)
{
	//좌표에 대한 기준과 파라미터 맞지 않음. 필요시 추후 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 현재 위치에서 주어진 좌표점까지 가,감속하면서 원호 CP Motion으로 이동한다.
 *  (다른 Board의 축 사용 가능)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+-----+---------+
 *		+-----------+-----------+------+--------+----------+
 *		| 회전중심X | 회전중심Y | 속도 | 가속도 | 회전방향 |
 *		+-----------+-----------+------+--------+----------+
 *
 * NMC Library : spl_arc_move2ax(), spl_arc_move3ax(), spl_arc_movenax()
 *
 * @param	siLen			: 축 수 (>= 2)
 * @param	*psiAxes		: 축 ID 배열 (축 수만큼 존재)
 * @param	siCenterX		: 회전 중심 X좌표
 * @param	siCenterY		: 회전 중심 Y좌표
 * @param	*pdPoint		: 이동할 좌표 (축 수 만큼 존재)
 * @param	dVelocity		: 이동 속도 (0.0=SetMoveSpeed()에서 지정한 속도 사용)
 * @param	siAccel			: 이동 가속도 (0=SetMoveSpeed()에서 지정한 가,감속도 사용)
 * @param	bDir			: 회전방향, FALSE=CIR_CW(시계방향), TRUE=CIR_CCW(반시계방향)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SplArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY, double *pdPoint, double dVelocity, short siAccel, BOOL bDir)
{
	//좌표에 대한 기준과 파라미터 맞지 않음. 필요시 추후 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 현재 위치에서 주어진 좌표점까지 가,감속하면서 원호 CP Motion으로 이동한다.
 *  (다른 Board의 축 사용 가능)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+-----+---------+
 *		+-----------+-----------+------+--------+----------+
 *		| 회전중심X | 회전중심Y | 속도 | 가속도 | 회전방향 |
 *		+-----------+-----------+------+--------+----------+
 *
 * NMC Library : spl_auto_arc_move2ax(), spl_auto_arc_move3ax(), spl_auto_arc_movenax()
 *
 * @param	siLen			: 축 수 (>= 2)
 * @param	*psiAxes		: 축 ID 배열 (축 수만큼 존재)
 * @param	siCenterX		: 회전 중심 X좌표
 * @param	siCenterY		: 회전 중심 Y좌표
 * @param	*pdPoint		: 이동할 좌표 (축 수 만큼 존재)
 * @param	dVelocity		: 이동 속도 (0.0=SetMoveSpeed()에서 지정한 속도 사용)
 * @param	siAccel			: 이동 가속도 (0=SetMoveSpeed()에서 지정한 가,감속도 사용)
 * @param	bDir			: 회전방향, FALSE=CIR_CW(시계방향), TRUE=CIR_CCW(반시계방향)
 * @param	bAutoFlag		: 자동 Auto CP, FALSE=Auto CP 안함, TRUE=Auto CP 함
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SplAutoArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY, double *pdPoint, double dVelocity, short siAccel, BOOL bDir, BOOL bAutoFlag)
{
	//좌표에 대한 기준과 파라미터 맞지 않음. 필요시 추후 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 현재 위치에서 주어진 2/3차원 평면상의 좌표점까지 가,감속하면서 직선 CP Motion으로 이동한다.
 *  (다른 Board의 축 사용 불가)
 *  MapAxes(), SetMoveSpeed()에서 지정된 축들이 이동한다.
 *
 *		|---------------> siAxNum = n <---------------|
 *		+---------+---------+---------+-----+---------+
 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| 속도 | 가속도 |
 *		+------+--------+
 *
 * NMC Library : spl_line_move1(), spl_line_move2(), spl_line_move3()
 *
 * @param	siAxNum			: 축 수, 1=1축, 2=2축, 3=3축
 * @param	*pdPoint		: 이동할 좌표, 1축이면 1차원배열, 2축이면 2차원배열, 3축이면 3차원배열
 * @param	dVelocity		: 이동 속도 (0.0=SetMoveSpeed()에서 지정한 속도 사용)
 * @param	siAccel			: 이동 가속도 (0=SetMoveSpeed()에서 지정한 가,감속도 사용)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SplLineMoveN(short siAxNum, double *pdPoint, double dVelocity, short siAccel)
{
	//좌표에 대한 기준과 파라미터 맞지 않음. 필요시 추후 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 현재 위치에서 주어진 좌표점까지 가,감속하면서 직선 CP Motion으로 이동한다.
 *  (다른 Board의 축 사용 가능)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| 속도 | 가속도 |
 *		+------+--------+
 *
 * NMC Library : spl_line_move1ax(), spl_line_move2ax(), spl_line_move3ax(), spl_line_movenax()
 *
 * @param	siLen			: 축 수 (>= 1)
 * @param	*psiAxes		: 축 ID 배열 (축 수 만큼 존재)
 * @param	*pdPoint		: 이동할 좌표 (축 수 만큼 존재)
 * @param	dVelocity		: 이동 속도 (0.0=SetMoveSpeed()에서 지정한 속도 사용)
 * @param	siAccel			: 이동 가속도 (0=SetMoveSpeed()에서 지정한 가,감속도 사용)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SplLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel)
{
	//좌표에 대한 기준과 파라미터 맞지 않음. 필요시 추후 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 현재 위치에서 주어진 좌표점까지 자동 가,감속 여부를 인자로 설정받아서 직선 CP Motion으로 이동한다.
 *  (다른 Board의 축 사용 가능)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| 축 ID#1 | 축 ID#2 | 축 ID#3 | ... | 축 ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| 속도 | 가속도 |
 *		+------+--------+
 *
 * @param	siLen			: 축 수
 * @param	*psiAxes		: 축 ID 배열
 * @param	*pdPoint		: 이동할 좌표 (축 수 만큼 존재)
 * @param	dVelocity		: 이동 속도, 0.0=기지정된 속도로 이동
 * @param	siAccel			: 이동 가속도, 0=기지정된 가속도로 이동
 * @return	Error Code		: 0 = SUCCESS, 그 외 = Error
 */
ErrorInfo MRS_NMCLib::SplAutoLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel, BOOL bAutoFlag)
{
	//좌표에 대한 기준과 파라미터 맞지 않음. 필요시 추후 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 원, 원호 이동 시 원주속도를 지정한다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SetArcDivision(double dDegree)
{
	return NO_ERROR_SUCESS;
}

/**
 * 주어진 중심에서 지정된 각도만큼 원호를 그리며 동작을 수행한다.
 *  (다른 Board의 축 사용 불가)
 *  MapAxes(), SetMoveSpeed()에서 지정된 축들이 이동한다.
 *
 * NMC Library : arc_2()
 *
 * @param	siCenterX		: 회전 중심 X좌표
 * @param	siCenterY		: 회전 중심 Y좌표
 * @param	dAngle			: 회전 각도
 * @return	Error Code		: 0 = SUCCESS
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::Arc2(double dXCenter, double dYCenter, double dAngle)
{
	//좌표 기준과, 파라미터 상이함. 필요시 추후 확인하여 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 주어진 중심에서 지정된 각도만큼 원호를 그리며 동작을 수행한다.
 * 동일 Board의 축이 아니면 Error 처리됨.
 *  (다른 Board의 축 사용 불가)
 *
 * NMC Library : arc_2ax()
 *
 * @param	siAxis1			: 축1 ID
 * @param	siAxis2			: 축2 ID
 * @param	siCenterX		: 회전 중심 X좌표
 * @param	siCenterY		: 회전 중심 Y좌표
 * @param	dAngle			: 회전 각도
 * @param	dVelocity		: 이동 속도
 * @param	siAccel			: 이동 가속도
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID AXIS ID IN COMMON BOARD (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::Arc2Ax(short siAxis1, short siAxis2, double dXCenter, double dYCenter, double dAngle, double dVelocity, short siAccel)
{
	//좌표 기준과, 파라미터 상이함. 필요시 추후 확인하여 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 주어진 사각형의 가로와 세로의 길이를 이용하여 현재위치에서 상대이동을 하면서 CP Motion으로 사각형을 그린다.
 *
 *			+--------------------+ pdPoint (X, Y)
 *			|					 |
 *			|					 |
 *			|					 |
 *			|					 |
 *			|					 |
 *			+--------------------+
 *		현재위치
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::RectMove(short siAxis1, short siAxis2, double *pdPoint, double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * 현재위치에서 주어진 위치를 경유하면서 CP Motion으로 이동한다.
 *
 *				  |----------------> siLen = n <----------------|
 *		+---------+---------+---------+---------+-----+---------+
 *		| 축 ID#1 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		| 축 ID#2 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		| 축 ID#3 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| 속도 | 가속도 |
 *		+------+--------+
 *
 * NMC Library : spl_move()
 *
 * @param	siLen			: 위치 Data 개수, 최대 30
 * @param	siAxis1			: 축1 ID
 * @param	siAxis2			: 축2 ID
 * @param	siAxis3			: 축3 ID
 * @param	*pdPointX		: X좌표 배열 (위치 Data 개수만큼 존재)
 * @param	*pdPointY		: Y좌표 배열 (위치 Data 개수만큼 존재)
 * @param	*pdPointZ		: Z좌표 배열 (위치 Data 개수만큼 존재)
 * @param	dVelocity		: 이동 속도
 * @param	siAccel			: 이동 가속도
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POSITION DATA NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SplMove(short siLen, short siAxis1, short siAxis2, short siAxis3,
					  double *pdPointX, double *pdPointY, double *pdPointZ,
					  double dVelocity, short siAccel)
{
	//좌표 기준과, 파라미터 상이함. 필요시 추후 확인하여 작성.

	return NO_ERROR_SUCESS;
}

/**
 * SplMoveX()에 필요한 위치경로를 설정한다.
 *
 *				  |----------------> siLen = n <----------------|
 *		+---------+---------+---------+---------+-----+---------+
 *		| 축 ID#1 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		| 축 ID#2 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		| 축 ID#3 | 위치 #1 | 위치 #2 | 위치 #3 | ... | 위치 #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| 속도 | 가속도 |
 *		+------+--------+
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::SplMoveData(short siSplNum, short siLen, short siAxis1, short siAxis2,
						  short siAxis3, double *pdPoint1, double *pdPoint2,
						  double *pdPoint3, double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지정 3축이 Spline Motion으로 SplMoveData()에서 지정한 위치를 경유하면서 연속 이동한다.
 *
 * NMC Library : spl_movex()
 *
 * @param	siSplNum		: Spline Motion 번호, 1 ~ 20
 * @param	siAxis1			: 축1 ID
 * @param	siAxis2			: 축2 ID
 * @param	siAxis3			: 축3 ID
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID SPLINE MOTION NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::SplMovex(short siSplNum, short siAxis1, short siAxis2, short siAxis3)
{
	//좌표 기준과, 파라미터 상이함. 필요시 추후 확인하여 작성.

	return NO_ERROR_SUCESS;
}

/**
 * 각 Board별 ROM Version을 읽는다.
 *
 * NMC Library : version_chk()
 *
 * @param	siBdNum			: Board ID (0 ~ 7)
 * @param	*psiVersion		: ROM Version, 101 => 1.01
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID MOTION BOARD ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::VersionCheck(short siBdNum, short *psiVersion)
{
	return NO_ERROR_SUCESS;
}

/**
 * 해당 Error Code의 Error Message를 반환한다.
 *
 * NMC Library : error_message(), _error_message()
 *
 * @param	siCode			: Error Code
 * @param	*pcDst			: Error Message, m_iErrorBase + ERR_MAX_ERROR_LEN(80)보다 크거나 같아야 한다.
 * @return	Error Code		: 0 = SUCCESS
 *							  그 외 = 타 함수 Return Error
 */
ErrorInfo MRS_NMCLib::ErrorMessage(int iCode, char *pcDst)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

#ifndef SIMULATION
	MC_STATUS ms;
	ms = MC_GetErrorMessage(iCode, 128, pcDst);
	if (ms != MC_OK)
	{
		strLogMsg.Format(_T("ErrorMessage(): MC_GetErrorMessage(return the error message) Fail[ErrorCode=%d] - Error# is %d"), iCode, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#else
	pcDst = "Simulation_test";
#endif

	return NO_ERROR_SUCESS;
}

/**
 * Position Compare Board를 초기화한다. (축 2를 사용할 때는 같은 Board의 축이어야 한다.)
 * 동일 Board의 축이 아니면 Error 처리됨.
 * Board 구성에 기록한다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::PositionCompareInit(short siIndexSel, short siAxis1, short siAxis2)
{
	return NO_ERROR_SUCESS;
}

/**
 * Position Compare를 설정한다. (Standard Type)
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::PositionCompare(short siIndexSel, short siIndexNum, short siBitNo, short siAxis1,
							  short siAxis2, BOOL bLatch, short siFunction, short siOutMode,
							  double dPosition, long lTime)
{
	return NO_ERROR_SUCESS;
}

/**
 * Position Compare를 설정한다. (Interval Type)
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::PositionCompareInterval(BOOL bDir, short siAxis, short siBitNo, double dStartPos, double dLimitPos,
							  long lInterval, long lTime)
{
	return NO_ERROR_SUCESS;
}

/**
 * Position Compare 동작을 할 것인지 여부를 설정한다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::PositionCompareEnable(short siBdNum, BOOL bFlag)
{
	return NO_ERROR_SUCESS;
}

/**
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::PositionCompareClear(short siBdNum, short siIndexSel)
{
	return NO_ERROR_SUCESS;
}

/**
 * Position Compare 설정된 축의 Encoder 값을 읽어낸다.
 *
 * 지원 안함
 */
ErrorInfo MRS_NMCLib::PositionCompareRead(short siIndexSel, short siAxis, double *pdPosition)
{
	return NO_ERROR_SUCESS;
}

// 반올림 Function
double MRS_NMCLib::motionRoundValue(double dVal)
{
	return (ceil(dVal + 0.5) - 1.0);
}

// NMC Library가 리턴하는 에러 코드를 System Error형태로 만든다
ErrorInfo MRS_NMCLib::MakeRS_NMC_Error(MC_STATUS mcStatus)
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
		
		ErrorMessage(mcStatus, chGetErrorMessage);
		
		iReturnErrorCode = ERR_RS_NMC_UNKNOWN;
		break;
	}

	return ((iReturnErrorCode + 106000) * 1000 )+ m_iInstanceNo;
}

//현재 축의 통신이 정상적인지 체크(펑션모듈인 경우 첫번째 축에서만 체크하면 됨)
BOOL MRS_NMCLib::IsOpMode_NMC(short siAxis)
{
	CString strLogMsg;

	UINT8 nSlaveState = 0;
	SlaveGetCurState(m_iBoardNo, siAxis, &nSlaveState );
	if (nSlaveState == eST_OP)
	{
		return TRUE;
	}

	return FALSE;
}

// 기어드(동기 제어) 설정
ErrorInfo MRS_NMCLib::EnableSyncMotion_NMC(int nMasterAxis, int nSlaveAxis, int nRatioNumerator, int nRatioDenominator, double fAccel, double fDecel, double fJerk)
{
	CString strLogMsg;
	MC_STATUS ms;
	
	ErrorInfo errorInfo;
	
	errorInfo = AmpOff_for_SyncGroup(nMasterAxis);
	if(errorInfo != SUCCESS)
		return errorInfo
	
	errorInfo = AmpOff_for_SyncGroup(nSlaveAxis);
	if(errorInfo != SUCCESS)
		return errorInfo;
	
	Sleep(100);

	errorInfo = ResetAmp_for_SyncGroup(nMasterAxis);
	if(errorInfo != SUCCESS)
		return errorInfo;

	errorInfo = ResetAmp_for_SyncGroup(nSlaveAxis);
	if(errorInfo != SUCCESS)
		return errorInfo;

	Sleep(100);

	errorInfo = AmpOn_for_SyncGroup(nMasterAxis);
	if(errorInfo != SUCCESS)
		return errorInfo;
		
	errorInfo = AmpOn_for_SyncGroup(nSlaveAxis);
	if(errorInfo != SUCCESS)
		return errorInfo;
	
	Sleep(1000);

	ms = MC_GearIn(m_iBoardNo, nMasterAxis, nSlaveAxis, nRatioNumerator, nRatioDenominator, mcSetValue, motionRoundValue(fAccel), motionRoundValue(fDecel), motionRoundValue(fJerk), mcAborting);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	Sleep(10);
	
	return NO_ERROR_SUCESS;
}

// 기어드(동기 제어) 해제
ErrorInfo MRS_NMCLib::DisableSyncMotion_NMC(int nSlaveAxis)
{
	CString strLogMsg;
	MC_STATUS ms;
		
	ms = MC_GearOut(m_iBoardNo, nSlaveAxis);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	Sleep(10);

	return NO_ERROR_SUCESS;
}

// 기어드(동기 제어)가 활성화 되어 있는지 체크. 반환값이 true일때 싱크가 걸려있는 상태임.
BOOL MRS_NMCLib::IsSyncEnable_NMC(int nSlaveAxis)
{
#ifdef SIMULATION
	return TRUE;
#endif

	CString strLogMsg;
	UINT32	nState = 0;

	MC_ReadAxisStatus(m_iBoardNo, nSlaveAxis, &nState);
	if(nState & mcGearing)
	{
		//기어드가 활성화 되어 있는상태.
		return TRUE;
	} 
	else
	{
		//기어드가 활성화 되어 있지 않은 상태.
		return FALSE;
	}
}

ErrorInfo MRS_NMCLib::HomeSetConfig(short siAxis, int iHomeMode, BOOL bDirection)
{
	//지원안함 - 읽기전용 속성으로 되어 있음
	// Revision 지원됨.
	// NMC Manager에서 설정
	
	CString strLogMsg;
	int iBoardID;
	iBoardID = GetBoardID(siAxis);
	MC_STATUS ms;
	
	// 원점 복귀 타입 설정
	ms = MC_WriteIntParameter(iBoardID, siAxis, mcpHomingType, (int)iHomeMode);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	// 원점 복귀 방향 설정
	// Direction 반대로 0 = 정방향, 1= 역방향 
	//bDirection = !bDirection; // Add

	ms = MC_WriteIntParameter(iBoardID, siAxis, mcpHomingDir, (int)bDirection);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	return NO_ERROR_SUCESS;
}

ErrorInfo MRS_NMCLib::HomeSetVelocity(short siAxis, double dFastVelocity, double dSlowVelocity, double dAccel, double dDecel, double dPosOffset)
{
	CString strLogMsg;
	
	
	MC_STATUS ms;
	
	// 원점 복귀 1차 속도 설정
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingVelocity, dFastVelocity);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	// 원점 복귀 2차 속도 설정
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingCreepVelocity, dSlowVelocity);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// 원점 복귀 가속도 설정
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingAcceleration, dAccel);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// 원점 복귀 감속도 설정
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingDeceleration, dAccel*100);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// 원점 복귀 감속도 설정
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingJerk, dDecel*1000);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// 원점 복귀 Offset 설정
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomePositionOffset, dPosOffset);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	return NO_ERROR_SUCESS;
}

ErrorInfo MRS_NMCLib::HomingStart(short siAxis)
{
	CString strLogMsg;
	MC_STATUS ms;
	UINT32 state = 0;
	
	ms = MC_Home(m_iBoardNo, siAxis, 0.0, mcAborting);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	return NO_ERROR_SUCESS;
}

ErrorInfo MRS_NMCLib::IsHoming(short siAxis, BOOL *bState)
{
	CString strLogMsg;
	MC_STATUS ms;
	UINT32 state = 0;

	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	if(state & mcHoming)
	{
		*bState = TRUE;
	}
	//170721 KMH Test 
	else if(state & mcDriveFault || state & mcErrorStop || state & mcDisabled )
	{
		*bState = FALSE;
		return MakeRS_NMC_Error(ms);
	}
	/*/
	else
	{
		*bState = FALSE;
	}
	/*/

	return NO_ERROR_SUCESS;
}

ErrorInfo MRS_NMCLib::IsHomeEnd(short siAxis, BOOL *bState)
{
	CString strLogMsg;
	MC_STATUS ms;
	UINT32 state = 0;
	
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	if(state & mcIsHomed)
	{
		*bState = TRUE;
	}
	else 
	{
		*bState = FALSE;
	}

	return NO_ERROR_SUCESS;
}

ErrorInfo MRS_NMCLib::SplAutoLineMove(short siAxis, double dPosition, double dVelocity, double dAccel, double dDecel, int iBufferMode)
{
	short serrorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	
	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	// **050112
	if(dAccel < 1) dAccel = 1;
	if(dDecel < 1) dDecel = 1;
	
	MC_BUFFER_MODE mBufferMode;
	if(iBufferMode == 0) mBufferMode = mcAborting;			// 연속 이동 중간 지점에서 가감속 적용
	if(iBufferMode == 1) mBufferMode = mcBuffered;			// 연속 이동 중 감속 다시 가속
	if(iBufferMode == 2) mBufferMode = mcBlendingLow;		// 낮은 속도 기준으로 가감속
	if(iBufferMode == 3) mBufferMode = mcBlendingPrevious;	// 이전 속도 기준으로 가감속
	if(iBufferMode == 4) mBufferMode = mcBlendingNext;		// 다음 속도 기준으로 가감속
	if(iBufferMode == 5) mBufferMode = mcBlendingHigh;		// 높은 속도 기준으로 가감속

	MC_STATUS ms;
	ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcPositiveDirection, mBufferMode);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);
		
	return NO_ERROR_SUCESS;
}

CString MRS_NMCLib::ReturnCurrentMotionError()
{
	CString strReturn = "";
	strReturn.Format(_T("%s"), chGetErrorMessage);

	memset(chGetErrorMessage, 0, sizeof(chGetErrorMessage));

	return strReturn;
}

int MRS_NMCLib::GetBoardID(int iAxisID)
{
	return DEF_NMC_BOARD_ID_1;
}

// 축의 보드 ID 반환
ErrorInfo MRS_NMCLib::AmpOn_for_SyncGroup(int iAxisID)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	
	/** 축 ID 범위 오류 */
	if((iAxisID < 1) || (iAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
#ifndef SIMULATION
	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_Power(m_iBoardNo, iAxisID, true);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("AmpOn_for_SyncGroup(): MC_Power(enable or disable AMP) Fail[AxisID=%d] - Error# is %d"), iAxisID, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif

	return NO_ERROR_SUCESS;
}

ErrorInfo MRS_NMCLib::AmpOff_for_SyncGroup(int iAxisID)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	
	
	/** 축 ID 범위 오류 */
	if((iAxisID < 1) || (iAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
#ifndef SIMULATION
	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_Power(m_iBoardNo, iAxisID, false);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("AmpOff_for_SyncGroup(): MC_Power(enable or disable AMP) Fail[AxisID=%d] - Error# is %d"), iAxisID, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif
	
	return NO_ERROR_SUCESS;
}

ErrorInfo MRS_NMCLib::ResetAmp_for_SyncGroup(int iAxisID)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	
	/** 축 ID 범위 오류 */
	if((iAxisID < 1) || (iAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
#ifndef SIMULATION
	/** 축 AMP Fault Reset 지정 */
	MC_STATUS ms;
	ms = MC_Reset(m_iBoardNo, iAxisID);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetAmpFaultEnable(): Invalid Axis Number(AxisNo=%d is out of range)"), iAxisID);
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif
	
	return NO_ERROR_SUCESS;
}
//SJ_YYK 160317 Add..
ErrorInfo MRS_NMCLib::SetOriginComplete(short siAxis)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	
	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
#ifndef SIMULATION
	/** 축 AMP Fault Reset 지정 */
	MC_STATUS ms;
	ms = MC_SetHomeFlag(m_iBoardNo, siAxis, siAxis);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetAmpFaultEnable(): Invalid Axis Number(AxisNo=%d is out of range)"), siAxis);
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif
	
	return NO_ERROR_SUCESS;
}
ErrorInfo MRS_NMCLib::GetOriginCompleteFlag(short siAxis, short *psiState)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	
	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
#ifndef SIMULATION
	/** 축 AMP Fault Reset 지정 */
	MC_STATUS ms;
	UINT32 state = 0;
	ms = MC_GetHomeFlag(m_iBoardNo, siAxis, &state);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetAmpFaultEnable(): Invalid Axis Number(AxisNo=%d is out of range)"), siAxis);
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
	
	*psiState = state;
#endif
	return NO_ERROR_SUCESS;
}
ErrorInfo MRS_NMCLib::ResetOriginComplete(short siAxis)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** 축 ID 범위 오류 */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	UINT16 iDataArray = 0;
#ifndef SIMULATION
	MC_STATUS ms;
	ms = MasterSetSDODataEcatAddr(	m_iBoardNo, siAxis, 0x5030,	0x00, 2,&respDatasize,(UINT8*)&iDataArray);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetVelLimit(): MC_WriteParameter(set velocity limit) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
#endif
	return NO_ERROR_SUCESS;
}