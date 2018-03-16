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
 *				 �� ������ RS EtherCat Motion Library�� ���� Class�� ���� �ڵ带 �����Ѵ�.
 */

int	MRS_NMCLib::m_iObjNo = 0;

/**
 * Constructor : create and clear data
 *				 Data�� �����ϰ� �ʱ�ȭ�Ѵ�.
 * @stereotype constructor
 */
MRS_NMCLib::MRS_NMCLib(SCommonAttribute commonData)
:ICommon(commonData)	//** 1222
{

	m_pBoardConfig = new MMotionBoard();

	/** MRS_NMCLib ��ü �� ���� */
	m_iObjNo++;

	m_iBoardNo = 0;

	memset(chGetErrorMessage, 0, sizeof(chGetErrorMessage));
}

/**
 * Destructor : Delete inner object
 *				���� ��ü�� �Ҹ��Ѵ�.
 * @stereotype destructor
 */
MRS_NMCLib::~MRS_NMCLib()
{
	BOOL bStatus;

	/** MRS_NMCLib ��ü �� ���� */
	m_iObjNo--;

	/** ������ ��ü�̸� */
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

	/** Board �������� ��ü �Ҹ� */
	if (m_pBoardConfig != NULL)
		delete m_pBoardConfig;
}

/**
 * �� ��� ���� ���� (�̹� ������� ���� ������� ���� �Ұ�)
 *
 * �� ��� ���� ������ Motor Parameter �������� �����ϸ�, ����ڿ� ���� ������ �Ǿ�� �Ѵ�.
 *
 * @param	siAxisID		: �� ID
 * @param	bState			: ��� ���� (TRUE=���, FALSE=�̻��)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = USED AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetUseAxis(int iBoardNo, short siAxisID, BOOL bState)
{
	ErrorInfo	errorInfo;
	BOOL	bStt = 0;
	CString	strLogMsg;

	/** �� ID ���� ���� */
	if((siAxisID < 1) || (siAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	/** �� ��� ������ ���� */
	if (bState == DEF_AXIS_USE)
	{
		/** �� ��� ���� �б� */
		if ((errorInfo = m_pBoardConfig->GetUseAxis(siAxisID, &bStt)).IsError())
			return errorInfo;

		/** �̹� ���� ����ϰ� ������ */
		if (bStt == DEF_AXIS_USE)
			return NO_ERROR_SUCESS;

		/** �� ����ϱ�� ���� */
		if ((errorInfo = m_pBoardConfig->SetUseAxis(siAxisID, bState)).IsError())
			return errorInfo;
	}
	else
	{
		/** �� �����ϱ�� ���� */
		if ((errorInfo = m_pBoardConfig->SetUseAxis(siAxisID, bState)).IsError())
			return errorInfo;
	}

	m_iBoardNo = iBoardNo;

	return NO_ERROR_SUCESS;
}
/**
 * �� ��� ���� �б�
 *
 * @param	siAxisID		: �� ID
 * @param	*pbState		: ��� ���� (TRUE=���, FALSE=�̻��)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetUseAxis(short siAxisID, BOOL *pbState)
{
	ErrorInfo errorInfo;
	CString	strLogMsg;

	/** �� ID ���� ���� */
	if((siAxisID < 1) || (siAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	/** �μ� Pointer ���� */
	if (pbState == NULL)
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �� ��� ���� �б� */
	if ((errorInfo = m_pBoardConfig->GetUseAxis(siAxisID, pbState)).IsError())
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * Motor Parameter ���� (Board �ʱ�ȭ �� ��� ����)
 *
 * ���޵� SMotionAxis�� �������� Motion�� �����Ѵ�. (�� 1�� ������ ����)
 *
 * 1. Board�� �ʱ�ȭ �ȵǾ� ������ Error Return
 * 2. �� AMP Enable(Servo ON)�̸� Disable(OFF) �� �۾� ����
 * 3. Motor ���� ����
 * 4. Feedback Device ����
 * 5. Loop ���� ����
 * 6. Motor ������ ����
 *		6.1 �ӵ��� Servo�� ���
 *			���� ���� ��� ���� ����
 *		6.2 �Ϲ� Stepper�� ���
 *			�޽� ���ֺ�� ���ڱ��� ����
 *		6.3 Micro Stepper Ȥ�� ��ġ�� Servo�� ���
 *			�޽� ���ֺ�(default:8)�� ���ڱ���(default:1.0) ����
 * 7. ��� �޽� ���� ����
 * 8. Encoder�� ��ǥ ���� ����
 * 9. AMP Enable, Fault, Reset ����
 * 10. Home, Positive, Negative Sensor ����
 * 11. Positive, Negative S/W Limit ����
 * 12. In-Position ����
 * 13. �� AMP Enable(Servo ON)�̾����� Enable(ON) ����
 *
 * @param	siAxisID		: �� ID, -1=���ȵ�
 * @param	mAx				: Motor Parameter
 * @param	bBootOpt		: (OPTION=FALSE) boot file�� ������� ����, TRUE=boot file�� ���
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = USED AXIS ID (MOTIONLIB)
 *							  xx = MOTION BOARD NO INITIALIZED (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetMotorParam(short siAxisID, SMotionAxis mAx, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	BOOL	bAmpState = FALSE;
	CString	strLogMsg;

	/** �� ID ���� ���� */
	if((siAxisID < 1) || (siAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109); // ERR_MLIB_INVALID_AXIS_ID

	/** Board �ʱ�ȭ ���� ���� */
	if (m_pBoardConfig->GetBoardInitSuccess() != TRUE)
		return generateErrorCode(106073); // ERR_MLIB_NO_MOTION_BOARD_INITIALIZED
	
	/** AMP Enable ���� Ȯ�� */
	if ((errorInfo = GetAmpEnable(siAxisID, &bAmpState)).IsError())
		return errorInfo;

	/** AMP Enable�̸� Disable ���� */
	if (bAmpState == DEF_ENABLE)
	{
		if ((errorInfo = SetAmpEnable(siAxisID, DEF_DISABLE)).IsError())
			return errorInfo;
	}

	/** Motor ���� ���� */
	if ((errorInfo = SetMotorType(siAxisID, mAx.iMotorType, bBootOpt)).IsError())
		return errorInfo;

	/** Motor Feed Device�� Lop ���� ���� */
	if ((errorInfo = SetAxisProperty(siAxisID, mAx.iFeedbackType, mAx.bLoopType, bBootOpt)).IsError())
		return errorInfo;

	/** Motor Type�� ���� */
	switch (mAx.iMotorType)
	{
	case DEF_SERVO_MOTOR :			// �ӵ��� Serve
		/** �ӵ��� Servo�� ���� ���� ��� ���� ���� */
		if ((errorInfo = SetVServoProperty(siAxisID, mAx.bVServoControl, mAx.bVServoPolar, bBootOpt)).IsError())
			return errorInfo;

		break;
	case DEF_STEPPER :				// �Ϲ� Stepper
		/** �޽� ���ֺ�� ���ڱ��� ���� */
		if ((errorInfo = SetRatioProperty(siAxisID, mAx.iStepperPulseR, mAx.dStepperEGear, bBootOpt)).IsError())
			return errorInfo;

		break;
	case DEF_MICRO_STEPPER :		// Micro Stepper �Ǵ� ��ġ�� Servo
		/** �޽� ���ֺ�� ���ڱ��� ���� */
		if ((errorInfo = SetRatioProperty(siAxisID, mAx.iStepperPulseR, mAx.dStepperEGear, bBootOpt)).IsError())
			return errorInfo;

		break;
	}

	/** ��� �޽� ���� ���� */
	if ((errorInfo = SetStepMode(siAxisID, mAx.bPulseType, bBootOpt)).IsError())
		return errorInfo;

	/** Encoder ����� ��ǥ ���� ���� */
	if ((errorInfo = SetEncoderDirection(siAxisID, mAx.bEncoderDir, mAx.bCoordinateDir, bBootOpt)).IsError())
		return errorInfo;
	Sleep(10);
	/** AMP Enable Level ���� */
	if ((errorInfo = SetAmpEnableLevel(siAxisID, mAx.bAmpEnableLevel, bBootOpt)).IsError())
		return errorInfo;

	/** AMP Fault Level ���� */
	if ((errorInfo = SetAmpFaultLevel(siAxisID, mAx.bAmpFaultLevel, bBootOpt)).IsError())
		return errorInfo;

	/** AMP Fault Event ���� */
	if ((errorInfo = SetAmpFaultEvent(siAxisID, mAx.iAmpFaultEvent, bBootOpt)).IsError())
		return errorInfo;

	/** AMP Rest Level ���� */
	if ((errorInfo = SetAmpResetLevel(siAxisID, mAx.bAmpResetLevel, bBootOpt)).IsError())
		return errorInfo;

	/** Home Sensor Level/Event ���� */
	if ((errorInfo = SetSensorLimit(siAxisID, DEF_HOME_SENSOR, mAx.iHomeEvent, mAx.bHomeLevel, bBootOpt)).IsError())
		return errorInfo;

	/** Positive Sensor Level/Event ���� */
	if ((errorInfo = SetSensorLimit(siAxisID, DEF_POSITIVE_SENSOR, mAx.iPositiveEvent, mAx.bPositiveLevel, bBootOpt)).IsError())
		return errorInfo;

	/** Negative Sensor Level/Event ���� */
	if ((errorInfo = SetSensorLimit(siAxisID, DEF_NEGATIVE_SENSOR, mAx.iNegativeEvent, mAx.bNegativeLevel, bBootOpt)).IsError())
		return errorInfo;

	/** Positive S/W Limit Level/Event ���� */
	if ((errorInfo = SetSWLimit(siAxisID, DEF_POSITIVE_SENSOR, mAx.dPositiveSWLimit, mAx.iPositiveSWEvent, bBootOpt)).IsError())
		return errorInfo;

	/** Negative S/W Limit Level/Event ���� */
	if ((errorInfo = SetSWLimit(siAxisID, DEF_NEGATIVE_SENSOR, mAx.dNegativeSWLimit, mAx.iNegativeSWEvent, bBootOpt)).IsError())
		return errorInfo;

	/** In-Position ��ġ�������� Level ���� */
	if ((errorInfo = SetInPosition(siAxisID, mAx.dInpositionError, mAx.bInpositionLevel, bBootOpt)).IsError())
		return errorInfo;

	/** In-Position ��뿩�� ���� ���� */
	if ((errorInfo = SetInpositionRequired(siAxisID, TRUE, bBootOpt)).IsError())
		return errorInfo;

	/** AMP Enable�̾����� Enable ���� */
	if (bAmpState == DEF_ENABLE)
	{
		if ((errorInfo = SetAmpEnable(siAxisID, DEF_ENABLE)).IsError())
			return errorInfo;
	}

	return NO_ERROR_SUCESS;
}
/**
 * Motor Parameter �б�
 *
 * @param	siAxisID		: �� ID, -1=All Motor
 * @param	*pmAx			: Motor Parameter, siAxisID=-1�̸� �迭 ������ ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = USED AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
	if((siAxisID < 1) || (siAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109); // ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pmAx == NULL)
		return generateErrorCode(106090); // ERR_MLIB_INVALID_POINTER

	/** Motor Parameter �б� */

	/** Motor ���� �б� */
	if ((errorInfo = GetMotorType(siAxisID, &siMotorType)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iMotorType = siMotorType;

	/** Motor Feed Device�� Lop ���� �б� */
	if ((errorInfo = GetAxisProperty(siAxisID, &siFeedbackType, &pmAx->bLoopType)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iFeedbackType = (int)siFeedbackType;

	/** Motor Type�� ���� */
	switch (pmAx->iMotorType)
	{
	case 0 :			// �ӵ��� Serve
		/** �ӵ��� Servo�� ���� ���� ��� ���� �б� */
		if ((errorInfo = GetVServoProperty(siAxisID, &pmAx->bVServoControl, &pmAx->bVServoPolar)) != NO_ERROR_SUCESS)
			return errorInfo;

		break;
	case 1 :				// �Ϲ� Stepper
		/** �޽� ���ֺ�� ���ڱ��� �б� */
		if ((errorInfo = GetRatioProperty(siAxisID, &siStepperPulseR, &pmAx->dStepperEGear)) != NO_ERROR_SUCESS)
			return errorInfo;
		pmAx->iStepperPulseR = (int)siStepperPulseR;

		break;
	case 2 :		// Micro Stepper �Ǵ� ��ġ�� Servo
		/** �޽� ���ֺ�� ���ڱ��� �б� */
		if ((errorInfo = GetRatioProperty(siAxisID, &siStepperPulseR, &pmAx->dStepperEGear)) != NO_ERROR_SUCESS)
			return errorInfo;
		pmAx->iStepperPulseR = (int)siStepperPulseR;

		break;
	}

	/** ��� �޽� ���� �б� */
	if ((errorInfo = GetStepMode(siAxisID, &pmAx->bPulseType)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** Encoder ����� ��ǥ ���� �б� */
	if ((errorInfo = GetEncoderDirection(siAxisID, &pmAx->bEncoderDir, &pmAx->bCoordinateDir)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** AMP Enable Level �б� */
	if ((errorInfo = GetAmpEnableLevel(siAxisID, &pmAx->bAmpEnableLevel)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** AMP Fault Level �б� */
	if ((errorInfo = GetAmpFaultLevel(siAxisID, &pmAx->bAmpFaultLevel)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** AMP Fault Event �б� */
	if ((errorInfo = GetAmpFaultEvent(siAxisID, &siAmpFaultEvent)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iAmpFaultEvent = siAmpFaultEvent;

	/** AMP Rest Level �б� */
	if ((errorInfo = GetAmpResetLevel(siAxisID, &pmAx->bAmpResetLevel)) != NO_ERROR_SUCESS)
		return errorInfo;

	/** Home Sensor Level/Event �б� */
	if ((errorInfo = GetSensorLimit(siAxisID, 0, &siHomeEvent, &pmAx->bHomeLevel)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iHomeEvent = siHomeEvent;

	/** Positive Sensor Level/Event �б� */
	if ((errorInfo = GetSensorLimit(siAxisID, 1, &siPositiveEvent, &pmAx->bPositiveLevel)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iPositiveEvent = siPositiveEvent;

	/** Negative Sensor Level/Event �б� */
	if ((errorInfo = GetSensorLimit(siAxisID, 2, &siNegativeEvent, &pmAx->bNegativeLevel)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iNegativeEvent = siNegativeEvent;

	/** Positive S/W Limit Level/Event �б� */
	if ((errorInfo = GetSWLimit(siAxisID, 1, &pmAx->dPositiveSWLimit, &siPositiveSWEvent)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iPositiveSWEvent = siPositiveSWEvent;

	/** Negative S/W Limit Level/Event �б� */
	if ((errorInfo = GetSWLimit(siAxisID, 0, &pmAx->dNegativeSWLimit, &siNegativeSWEvent)) != NO_ERROR_SUCESS)
		return errorInfo;
	pmAx->iNegativeSWEvent = siNegativeSWEvent;

	/** In-Position ��ġ�������� Level �б� */
	if ((errorInfo = GetInPosition(siAxisID, &pmAx->dInpositionError, &pmAx->bInpositionLevel)) != NO_ERROR_SUCESS)
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * Motion Board Parameter ����
 *
 * @param	iObjectID		: Object ID
 * @param	MotionBd		: Motion Board Parameter
 * @param	iErrorBase		: Error Code Start Number
 * @param	strFullFileName	: Log File Path & File Name
 * @param	ucLevel			: Log LEvel (None, Normal, Warning, Error)
 * @param	iDays			: Log ���� �� ��
 * @return	Error Code		: 0 = SUCCESS
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetBoardParam(SMotionBoard MotionBd)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	/** Board Parameter ���� */
	if ((errorInfo = m_pBoardConfig->SetBoardConfig(MotionBd)) != NO_ERROR_SUCESS)
		return errorInfo;


	/** Board �ʱ�ȭ */
	if ((errorInfo = Initialize(MotionBd.iMaxBoardNo, MotionBd.rglAddress)) != NO_ERROR_SUCESS)
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * Motion Board Parameter �б�
 *
 * @param	*pmBd			: Motion Board Parameter
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetBoardParam(SMotionBoard *pmBd)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	/** �μ� Pointer ���� */
	if (pmBd == NULL)
		return generateErrorCode(106090);  // ERR_MLIB_INVALID_POINTER

	/** Board Parameter �б� */
	if ((errorInfo = m_pBoardConfig->GetBoardConfig(pmBd)) != NO_ERROR_SUCESS)
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * ������ Board ���� �б�
 *
 * @param	*psiBdNum		: Board ���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetBoardNum(short *psiBdNum)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �μ� Pointer ���� */
	if (psiBdNum == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** ������ Board ���� �б� */
	if ((errorInfo = m_pBoardConfig->GetBoardNum(psiBdNum)) != NO_ERROR_SUCESS)
		return errorInfo;

	return NO_ERROR_SUCESS;
}
/**
 * Motion Board �ʱ�ȭ
 * 
 * 1. Motion Board �ʱ�ȭ�� ����� ���
 * 	(1) Board ���� ���� Board DPRAM Address�� ���� ���
 * 		�� Board �ʱ�ȭ ���� Pass
 * 	(2) Board ���� ���� �ٸ� ���
 * 		�� Board �ʱ�ȭ ���� ó��
 * 	(3) Board DPRAM Address�� �ٸ� ���
 * 		�� Board �ʱ�ȭ ���� ó��
 * 
 * 2. Motion Board �ʱ�ȭ�� ������� ���� ���
 * 	�� Board �ʱ�ȭ ����
 *
 * NMC Library : MC_Init()
 * NMC Setup : �Լ� ���� BoardInit()
 *
 * @param	siBdNum			: Motion Board �� (1 ~ 4)
 * @param	*plAddress		: Board DPRAM Address (Board ����ŭ)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID MOTION BOARD NUMBER (MOTIONLIB)
 *							  xx = NOT EQUAL MOTION BOARD NUMBER (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = NOT EQUAL MOTION DPRAM ADDRESS (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
//kde
ErrorInfo MRS_NMCLib::Initialize(short siBdNum, long *plAddress)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short siBdNumber;
	long  rglAddr[DEF_MAX_MOTION_BD];
	CString strLogMsg;

	/** ���� Board ���� �б� */
	m_pBoardConfig->GetBoardNum(&siBdNumber, rglAddr);

	/** Board ���� ���� ���� */
	if ((siBdNum < DEF_NON_MOTION_BD) || (siBdNum > siBdNumber))
		return generateErrorCode(106096);	// ERR_MLIB_INVALID_MOTION_BOARD_NUMBER


	/** �μ� Pointer ���� */
	if (plAddress == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** Board �ʱ�ȭ�� �Ǿ� ������ */
	if (m_pBoardConfig->GetBoardInitSuccess() == TRUE)
	{
		/** �����Ǿ� �ִ� Board ������ �����Ϸ��� Board ������ �ٸ��� */
		if (siBdNumber != siBdNum)
			return generateErrorCode(106072);	// ERR_MLIB_NOT_EQUAL_MOTION_BOARD_NUMBER


		for (int i = 0; i < siBdNum; i++)
		{
			if (plAddress[i] != rglAddr[i])
				return generateErrorCode(106071);	// ERR_MLIB_NOT_EQUAL_MOTION_DPRAM_ADDRESS
		}
	}
	/** Board �ʱ�ȭ�� �ȵǾ� ������ */
	else
	{
		/** Board ����, DPRAM Address ���� */
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

		/** Motion Board �ʱ�ȭ Flag ���� */
		m_pBoardConfig->SetBoardInitSuccess(TRUE);
	}

	return NO_ERROR_SUCESS;
}
/**
 * Motion Board �ʱ�ȭ ���� �б�
 *
 * @param	*pbInit			: Board �ʱ�ȭ ����, TRUE=�ʱ�ȭ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetBoardInit(BOOL *pbInit)
{
	/** �μ� Pointer ���� */
	if (pbInit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** Board �ʱ�ȭ ���� Flag �б� */
	*pbInit = m_pBoardConfig->GetBoardInitSuccess();

	return NO_ERROR_SUCESS;
}

/**
	��� ����
*/
ErrorInfo MRS_NMCLib::GetAxes(short siBdNum, short *psiAxes)
{
	CString strLogMsg;

	/** �μ� Pointer ���� */
	if (psiAxes == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** ��� Board ���� �� */
	if (siBdNum == DEF_ALL_MOTION_BD_ID)
	{
		*psiAxes = 1;
	}
	/** Board 1�� ���� �� */
	else
	{
		*psiAxes = 1;
	}

	return NO_ERROR_SUCESS;
}

/**
	��� ����
 */
ErrorInfo MRS_NMCLib::MapAxes(short siAxNum, short *psiMapArray)
{
	return NO_ERROR_SUCESS;
}

/**
	��� ����
 */
ErrorInfo MRS_NMCLib::Dwell(short siAxis, long *plDuration)
{
	return NO_ERROR_SUCESS;
}

/**
	��� ��, ���� ��
 */
ErrorInfo MRS_NMCLib::IOTrigger(short siAxis, short siBitNo, BOOL bState)
{
#ifndef SIMULATION
	//�Լ� ���� �ʿ�
#endif

	return NO_ERROR_SUCESS;
}

/**
 * �� ��� ���࿡ ���� ���¸� �����ش�.
 * ��� ���� �Ϸ�� �����̸� SUCCESS�� Return�Ѵ�.
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: �� ID
 * @param	siType			: ��ȸ ����, 0=�̵����, 1=�ӵ����, 2=InPosition ������ ����
 * @return	Error Code		: 0 = SUCCESS��
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID IN COMMAND TYPE (MOTIONLIB)
 *							  xx = AXIS IS IN COMMAND (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::InCommand(short siAxis, BOOL *bState, short siType)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
	{
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	}	

	//MMC-ECAT ������ �������� ������ ��� �κ��� StandStill ���� �ϳ��� ���յ�.
	//NMC Manager�� ���� �������� ������ �޽��� �ӵ� �ΰ����� �����ϵ��� �Ǿ� ����.
	//�ӵ� �̵��ÿ��� �ӵ��� �ӵ��� Window������� ���;� StandStill���·� ���Ե�.
	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if(ms == MC_OK)
	{
		if(state & mcStandStill)
		{
			//��� �Ϸ�� ����..
			*bState = FALSE;
		}
		else
		{
			//�̵� �����..
//			if(state & mcGearing)
			{
				*bState = TRUE;
			}
//			else
//			{
//				//��� �Ϸ�� ����..
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
// RS-ECAT������ �׷������� ������. �ʿ�� �ۼ� �ʿ�
*/
ErrorInfo MRS_NMCLib::AllDone (BOOL *pbStatus)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	
	/** �μ� Pointer ���� */
	if (pbStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	// RS-ECAT������ �׷������� ������. �ʿ�� ���� ������ �ۼ��� �ʿ� ����.
	*pbStatus = TRUE;

	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� �Ϸ� ���θ� Return �Ѵ�.
 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ�� �Ǵ� ���Ҿ� In-Position ���α��� Ȯ���Ѵ�.
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: �� ID
 * @return	: TRUE = AXIS_DONE
 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
 */
BOOL MRS_NMCLib::IsAxisDone(short siAxis)
{
#ifndef SIMULATION
	//MMC-ECAT ������ �������� ������ ��� �κ��� StandStill ���� �ϳ��� ���յ�.
	UINT32 state = 0;

	MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if(state & mcStandStill)
	{
		//��� �Ϸ�� ����..
		return TRUE;
	}
	else
	{
		//!(state | (mcConstantVelocity|mcAccelerating|mcDecelerating))
		//�̵� �����..
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
 * ���� ���� �Ϸ� ���θ� Return �Ѵ�.
 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ���Ѵ�. InPosition Ȯ�� ����
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: �� ID
 * @return	: TRUE = AXIS_DONE
 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
 */

BOOL MRS_NMCLib::IsMotionDone(short siAxis)
{
#ifndef SIMULATION
	//MMC-ECAT ������ �������� ������ ��� �κ��� StandStill ���� �ϳ��� ���յ�.
	UINT32 state = 0;

	MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if( state & mcStandStill)
	{
		//��� �Ϸ�� ����..
		return TRUE;
	}
	else
	{
		//�̵� �����..
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
 * ���� ���� ���� �Ϸ�� ������ ��ٸ���.
 * IsAxisDone()
 *
 * @param	siAxis			: �� ID
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::WaitForDone(short siAxis)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	MTickTimer m_Timer;

	/** �� ID ���� ���� */
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
 * ���� ���� AMP Fault�� Clear�ϰų� Fault Port�� Enable ���·� �����Ѵ�.
 *
 * NMC Library : MC_Reset()
 *
 * @param	siAxis			: �� ID
 * @param	bState			: ������ ����, FALSE=Clear, TRUE=Enable
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetAmpFaultEnable(short siAxis, BOOL bState)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	/** �� AMP Fault Reset ���� */
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
 * �� ��� ���� ���¸� �Ѳ����� �д´�.
 * psIstatus[0] = axis_source �Լ���
 * psistatus[1] = in_sequence �Լ���
 * psistatus[2] = get_com_velocity �Լ���
 * psistatus[3] = get_act_velocity �Լ���
 * psistatus[4] = motion_done �Լ���
 * psistatus[5] = in_position �Լ���
 * psistatus[6] = axis_done �Լ���
 * 
 * plstatus[0] = get_io �Լ���
 * 
 * pdstatus[0] = get_position �Լ���
 * pdstatus[1] = get_command �Լ���
 * pdstatus[2] = get_error �Լ���
 *
 * NMC Library : axis_all_status()
 *
 * @param	siAxis			: �� ID
 * @param	*psiState		: ����, (axis_source, in_sequence, get_com_velocity, get_act_velocity,
 *									 motion_done, in_position, axis_done �����)
 * @param	*plStatus		: ����, (get_io �����)
 * @param	*pdStatus		: ����, (get_position, get_command, get_error �����)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::GetAllStatus(short siAxis, short *psiStatus, long *plStatus, double *pdStatus)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID


	/** �μ� Pointer ���� */
	if (psiStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
	if (plStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
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
	//���� ���ÿ��� �Ʒ��� ���� �����. ���ԵǾ� �ִ� �������´� mcStandStill �� ���� ���ǳ����� ã�ư��� ��.
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
 * �� ���� ���¸� �д´�.
 *
 * 		ST_NONE					0x0000		������
 * 		ST_HOME_SWITCH			0x0001		���� ���� ����
 * 		ST_POS_LIMIT			0x0002		+ ���� ���� ����
 * 		ST_NEG_LIMIT   			0x0004		- ���� ���� ����
 * 		ST_AMP_FAULT			0x0008		AMP Fault ���� �߻�
 * 		ST_A_LIMIT    			0x0010		��, ���� �ð��� limit���� Ŭ �� �߻�
 * 		ST_V_LIMIT  			0x0020		���� �ӵ��� �ӵ� limit�� ��� �� �߻�
 * 		ST_X_NEG_LIMIT 			0x0040		- Position limit�� ��� �� �߻�
 * 		ST_X_POS_LIMIT			0x0080		+ Position limit�� ��� �� �߻�
 * 		ST_ERROR_LIMIT			0x0100		��ġ������ limit������ Ŭ �� �߻�
 * 		ST_PC_COMMAND  			0x0200		EVENT�� �߻��� ������ �� �߻�
 * 		ST_OUT_OF_FRAMES    	0x0400		Frame Buffer�� Full ������ �� �߻�
 * 		ST_AMP_POWER_ONOFF  	0x0800		AMP Disable ������ �� �߻�
 * 		ST_ABS_COMM_ERROR   	0x1000		ABS Encoder ��� Error �� �߻�
 * 		ST_INPOSITION_STATUS	0x2000		??
 * 		ST_RUN_STOP_COMMAND 	0x4000 		??
 * 		ST_COLLISION_STATE  	0x8000		??
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: �� ID
 * @param	*psiState		: ����, �� ���´� bit �������� �����ȴ�.
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
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
 * �ص����� ������ �ӵ���ɰ��� PULSE������ �����ش�.
 *
 * @param	siAxis			: �� ID
 * @return	Pulse
 */
ErrorInfo MRS_NMCLib::GetComVelocity(short siAxis)
{
#ifdef SIMULATION
	return 0;
#endif

	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	//��ȯ���� double����. �� �κп� ���ؼ� ������ int �� ��ȯŸ���� ��� ����...
	double dVel = 0.0;
	MC_ReadParameter(m_iBoardNo, siAxis, mcpCommandedVelocity, &dVel);

	return (int)dVel;
}

/**
 * ���� ����(Home, Positive, Negative) ���¸� �д´�.
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: �� ID
 * @param	siType			: ���� ����, 0=Home, 1=Positive, 2=Negative
 * @param	*pbState		: ���� ����, TRUE=Active, FALSE=No Active
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pbState == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	/** Sensor ���� �б� */
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
 * AMP Disable/Enable ���¸� �д´�. (Servo ON/OFF)
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: �� ID
 * @param	*pbState		: AMP Enable ����, TRUE=Enable, FALSE=Disable
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetAmpEnable(short siAxis, BOOL *pbState)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short	siState = 0;
	CString	strLogMsg;


	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pbState == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// Amp On üũ
	if(state & mcPowerOn)
	{
		if(state & mcErrorStop)
		{
			// Amp Fault ��ȣ�� �ö�� ���..
			*pbState = DEF_DISABLE;
		} 
		else
		{
			// ������ ���������� ON �Ǿ� �ִ� ����..
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
 * AMP Enable�� Active Level�� �д´�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAmpEnableLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� Event �߻� ���¸� �д´�.
 *
 *		NO_EVENT		0		Event �߻����� ���� ����
 *		STOP_EVENT		1		stop_rate�� �����ϸ鼭 ����
 *		E_STOP_EVENT	2		e_stop_rate�� �����ϸ鼭 ����
 *		ABORT_EVENT		3		AMP disable ����
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	siAxis			: �� ID
 * @param	*psiState		: Event ����, 0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT
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


	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (psiState == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	UINT32 state = 0;
	MC_STATUS ms;
	ms = MC_ReadAxisStatus(m_iBoardNo, siAxis, &state); //170713 JSH.Mark
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	//state �� 4����Ʈ Ÿ���ε� �������� short �� �����ͷ� �Ѿ��..
	//�� �κп� ���� �߰����� ó���� �ʿ��� ����.
	*psiState = state;

	return NO_ERROR_SUCESS;
}

/**
 * Board�� Position Latch ���θ� �д´�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAxisLatchStatus(short siBdNum, BOOL *pbState)
{
	return NO_ERROR_SUCESS;
}

/**
 * �࿡ �߻��� Event�� �����ϰ�, ���� ��ɺ��� �����Ѵ�.
 * Event �߻� �Ŀ��� �׻� Event�� ������ �־�� �Ѵ�.
 * ABORT_EVENT �߻� �ÿ��� Event ���� �� AMP�� Disable �����̹Ƿ� �ٽ� Enable���־�� �Ѵ�.
 *
 * NMC Library : MC_Reset()
 *
 * @param	siAxis			: �� ID
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::ClearStatus(short siAxis)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
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
 * ���� Frame Buffer�� Clear�Ѵ�.
 * �ະ �ִ� 50���� Frame�� ������ Clear�Ѵ�. �� ���� �������� ����� ��� ����ȴ�.
 *
 * NMC Library : MC_Reset()
 *
 * @param	siAxis			: �� ID
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::ClearFrames(short siAxis)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** �� ID ���� ���� */
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
 * ���� ����ִ� Interpolation Frame ������ �����ش�.
 *
 * NMC Library : frames_interpolation()
 *
 * @param	siAxis			: �� ID
 * @param	*psiFrameNo		: Frame ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::FramesInterpolation(short siAxis, short *psiFrameNo)
{
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (psiFrameNo == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

#ifndef SIMULATION
	//��������. �ٸ� ���۰� �� ��������� MC_ReadAxisStatus �� ���� �о����� mcBufferFull ��Ʈ�� �ö�´�.
	//����� ������ �ִ� ũ��� 20���� �����Ǿ� ����.
#endif

	return NO_ERROR_SUCESS;
}

/**
 * ���� ����ִ� Frame ������ �����ش�.
 *
 * NMC Library : frames_left()
 *
 * @param	siAxis			: �� ID
 * @param	*psiFrameNo		: ����ִ� Frame ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 */
ErrorInfo MRS_NMCLib::FramesLeft(short siAxis, short *psiFrameNo)
{
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (psiFrameNo == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

#ifndef SIMULATION
	//��������. �ٸ� ���۰� �� ��������� MC_ReadAxisStatus �� ���� �о����� mcBufferFull ��Ʈ�� �ö�´�.
	//����� ������ �ִ� ũ��� 20���� �����Ǿ� ����.
#endif

	return NO_ERROR_SUCESS;
}

/**
 * �ش� Board�� Latch ���¸� �����ϰ�, Latch Status�� False�� ����ų�, S/W������ Position�� Latch�Ѵ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::Latch(short siBdNum, BOOL bType, BOOL bState)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� Latch�� Position�� �����ش�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetLatchedPosition(short siAxis, double *pdPosition)
{
	*pdPosition = 0.00;

	return NO_ERROR_SUCESS;
}

/**
 * ������ ��ǥ��ġ�� �������� �� ����Ѵ�. (��, ��ȣ �����߿��� ������� �ʴ´�.)
 * ��� ����
 */
ErrorInfo MRS_NMCLib::CompensationPos(short siLen, short *psiAxes, double *pdDist, short *psiAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * Board DPRAM Address�� �д´�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetDpramAddress(short siBdNum, long *plAddr)
{
	*plAddr = 0xd8000000;

	return NO_ERROR_SUCESS;
}

/**
 * ����ġ Motor�� Type�� �д´�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAbsEncoderType(short siAxis, short *psiType)
{
	*psiType = 1;

	return NO_ERROR_SUCESS;
}

/**
 * ���� �̵� �ְ�ӵ��� ��,���� �������� ���Ѱ��� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : v()
 *
 * @param	siAxis			: �� ID
 * @param	*pdVelocity		: �̵� �ְ�ӵ�, 1 ~ 2047000 coutn/sec
 * @param	*psiAccel		: ��,���� ������, 1 ~ 200, 10msec ����
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pdVelocity == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
	if (psiAccel == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	MC_STATUS ms;
	double fMaxVel;
	double fMaxAccel;

	//���� �ӵ�
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpMaxVelocityAppl_, &fMaxVel);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);		

	//���� ���ӵ�
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpMaxAccelerationAppl, &fMaxAccel);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	*pdVelocity = fMaxVel;
	*psiAccel = (short)fMaxAccel;

	return NO_ERROR_SUCESS;
}

/**
 * AMP Drive�� Fault �߻� ���¸� �д´�.
 *
 * NMC Library : amp_fault_switch()
 *
 * @param	siAxis			: �� ID
 * @param	*pbStatus		: AMP Fault ���¸� �д´�. TRUE=FAULT, FALSE=NORMAL
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


	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
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
		// Amp Fault ��ȣ�� �ö�� ���..
		*pbStatus = TRUE;
	} 
	else
	{
		// ������ ���������� ON �Ǿ� �ִ� ����..
		*pbStatus = FALSE;
	}

	return NO_ERROR_SUCESS;
}

/**
 * AMP Drive�� Fault �߻� �� ������ Event�� �д´�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAmpFaultEvent(short siAxis, short *psiAction, BOOL bBootOpt)
{
	//Fault �߻��� ������ Error Stop ���� ���� �Ǿ� ����
	*psiAction = 0x00;

	return NO_ERROR_SUCESS;
}

/**
 * AMP Enable�� Active Level�� �д´�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAmpEanbleLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt)
{
	*pbLevel = DEF_LOW;

	return NO_ERROR_SUCESS;
}

/**
 * AMP Fault�� Active Level�� �д´�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAmpFaultLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt)
{
	*pbLevel = DEF_LOW;
	
	return NO_ERROR_SUCESS;
}

/**
 * AMP Reset�� Active Level�� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAmpResetLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt)
{
	*pbLevel = DEF_LOW;
	
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� AMP Drive�� Resolution�� �д´�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAmpResolution(short siAxis, short *psiResolution, BOOL bBootOpt)
{
	*psiResolution = 2500;

	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� ���ֺ� ���� ���ڰ�, �и��� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetEncoderRatio(short siAxis, short *psiRatioA, short *psiRatioB, BOOL bBootOpt)
{
	//�� �κ� Ȯ�� �ʿ���.
	*psiRatioA = 1;
	*psiRatioB = 1;

	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� ȸ��/������ϴ� ����ȸ�� ������ �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : MC_ReadBoolParameter()
 *
 * @param	siAxis			: �� ID
 * @param	*pbStatus		: ����ȸ�� �� ��������
 * @param	bType			: � ����, FALSE=����, TRUE=ȸ��
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pbStatus == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER
	
	//ECAT ������ Module ������� ��ü��.
	//���ÿ� ������ �´��� Ȯ���� �ʿ��Ұ� ����.
	bool bData;
	MC_STATUS ms;
	ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpModuloAxis, &bData);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	if (bData == FALSE)
	{
		//��ⷯ ���
		bStatus = TRUE;
	}
	else
	{
		//���Ͼ� ���.
		bStatus = FALSE;
	}

	if (bStatus == FALSE)
		*pbStatus = FALSE;
	else
		*pbStatus = TRUE;

	return NO_ERROR_SUCESS;
}

/**
 * ����ȸ�� ���� �����̴� ������ �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : MC_ReadParameter(),
 *
 * @param	siAxis			: �� ID
 * @param	*pdRange		: �̵� ����
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetEndlessRange(short siAxis, double *pdRange, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pdRange = 0.0;
	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pdRange == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	//ECAT ������ Module ������� ��ü��.
	//���ÿ� ������ �´��� Ȯ���� �ʿ��Ұ� ����.
	double fData;
	MC_STATUS ms;
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpModuloValue, &fData);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	*pdRange = fData;

	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ġ���� �Ϸᰪ�� ��ġ���� �� ��ȣ Level�� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : MC_ReadParameter()
 *
 * @param	siAxis			: �� ID
 * @param	*pdInPosition	: ��ġ ������
 * @param	*pbLevel		: ��ȣ Level, TRUE=HIGH, FALSE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pdInPosition == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
	if (pbLevel == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	//�������� ������ ������� ����������, �������� ���������� ��������.
	MC_STATUS ms;
	double fInPos;
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpInPositionWindowSize, &fInPos);
	if(ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	*pdInPosition = fInPos;

	// ������ ��������.
	*pbLevel = DEF_LOW;

	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� InPosition ��ȣ ��뿩�θ� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetInpositionRequired(short siAxis, BOOL *pbReq, BOOL bBootOpt)
{
	//��������. ������ ����ϵ��� �Ǿ� ����.
	*pbReq = TRUE;

	return NO_ERROR_SUCESS;
}
/**
 * ���� ��ġ���� ���Ѱ��� Event�� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : 
 *
 * @param	siAxis			: �� ID
 * @param	*pdLimit		: ��ġ���� ���Ѱ�, �ִ� 35000 count
 * @param	*psiAction		: ��ġ���� Event, NO EVENT, ESTOP EVENT, ABORT EVENT
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetErrorLimit(short siAxis, double *pdLimit, short *psiAction, BOOL bBootOpt)
{
#ifdef SIMULATION
	*psiAction = 0x00;

	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pdLimit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
	if (psiAction == NULL)
	{
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER
	}

	//�� �κ��� SDO���� ������� ��ó�Ҽ� ����. ������ ���� �޶���.
	//�� ����� �ʿ�� RSA Ȯ���� �ۼ��� �ʿ䰡 ����.
	*psiAction = 0x00;

	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� �� Encoder�� C�� �޽� �̿� ���θ� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetIndexRequired(short siAxis, BOOL *pbIndexReq, BOOL bBootOpt)
{
	// ��������
	// ECAT�� �������� Ÿ���� Ref���� �����ϸ� �ڵ����� C(Z)������ �������� ����
	*pbIndexReq = TRUE;

	return NO_ERROR_SUCESS;
}

/**
 * I/O 8���� ����  ��,��� ��带 �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetIOMode(short siBdNum, BOOL *pbMode, BOOL bBootOpt)
{
	*pbMode = FALSE;
	
	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- ���� Limit Switch Active�� ������ Event�� ��ȣ Level�� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : MC_ReadBoolParameter()
 *
 * @param	siAxis			: �� ID
 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
 * @param	*psiLimit		: ������ Event
 * @param	*pbLevel		: ��ȣ Level, TRUE=HIGH, FALSE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (psiLimit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
	if (pbLevel == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	bool bData;
	MC_STATUS ms;

	switch (siType)
	{
	case DEF_HOME_SENSOR :		// Home Sensor

		//Ȩ���� Active Level ���.
		//�̺�Ʈ�� ������ Stop���� ���Ե�. Stop���� �ѹ��� ���Թ������� ������ ������ Error Stop��.
		ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpHomeActLevel, &bData);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);

		siLevel = bData;
		*psiLimit = 0x00;

		break;

	case DEF_POSITIVE_SENSOR :	// Positive Sensor

		//����Ƽ�� ���Լ��� Active Level ���.
		//�̺�Ʈ�� ������ Stop���� ���Ե�. Stop���� �ѹ��� ���Թ������� ������ ������ Error Stop��.
		ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpHWLimitPosActLevel, &bData);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);
		siLevel = bData;
		*psiLimit = 0x00;
		
		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

		//�װ�Ƽ�� ���Լ��� Active Level ���.
		//�̺�Ʈ�� ������ Stop���� ���Ե�. Stop���� �ѹ��� ���Թ������� ������ ������ Error Stop��.
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
 * Home, +/- ���� Limit Switch Active�� ������ Event�� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * @param	siAxis			: �� ID
 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
 * @param	*psiLimit		: ������ Event
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetSensorEvent(short siAxis, short siType, short *psiLimit, BOOL bBootOpt)
{
#ifdef SIMULATION
	*psiLimit = 0x00;
	
	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (psiLimit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	switch (siType)
	{
	case DEF_HOME_SENSOR :		// Home Sensor

		//��������.
		//ECAT�� ������ Stop���� ���� �Ǿ� ������, Stop���¿��� ������������ �� �и� Error Stop��.
		*psiLimit = 0x00;

		break;
	case DEF_POSITIVE_SENSOR :	// Positive Sensor

		//��������.
		//ECAT�� ������ Stop���� ���� �Ǿ� ������, Stop���¿��� ������������ �� �и� Error Stop��.
		*psiLimit = 0x00;
		
		break;

	case DEF_NEGATIVE_SENSOR :	// Negative Sensor

		//��������.
		//ECAT�� ������ Stop���� ���� �Ǿ� ������, Stop���¿��� ������������ �� �и� Error Stop��.
		*psiLimit = 0x00;
		
		break;

	default :
		return generateErrorCode(106079);	// ERR_MLIB_INVALID_SENSOR_TYPE
	}

	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- ���� Limit Switch Active�� ������ ��ȣ Level�� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * @param	siAxis			: �� ID
 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
 * @param	*pbLevel		: ��ȣ Level, TRUE=HIGH, FALSE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetSensorLevel(short siAxis, short siType, BOOL *pbLevel, BOOL bBootOpt)
{
#ifdef SIMULATION
	*pbLevel = DEF_LOW;
	
	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	short siLevel = DEF_LOW;
	

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
	{
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	}

	/** �μ� Pointer ���� */
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
 * +/- �������� Motor�� �̵��� �� �ִ� ���� ��ġ���� �� ��ġ���� �������� �� ������ Event�� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : MC_ReadParameter()
 *
 * @param	siAxis			: �� ID
 * @param	bType			: �̵� ����, TRUE=Positive, FALSE=Negative
 * @param	*pdPosition		: ���� ��ġ��
 * @param	*psiLimit		: ������ Event
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pdPosition == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
	if (psiLimit == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** Positive S/W */
	if (bType == DEF_POSITIVE_SW)
	{
		//���Խ� �׼ǿ� ���� ������ ��������.
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
		//���Խ� �׼ǿ� ���� ������ ��������.
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
 * �ش� ���� � Motor�� �����ϴ� ������ �����Ǿ� �ִ��� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetMotorType(short siAxis, short *psiType, BOOL bBootOpt)
{
	*psiType = DEF_SERVO_MOTOR;

	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� Feedback ��ġ�� Loop ���¸� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetAxisProperty(short siAxis, short *psiDevice, BOOL *pbLoop, BOOL bBootOpt)
{
	*psiDevice = 0;
	*pbLoop = DEF_OPEN_LOOP;

	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� Pulse ���ֺ�� ���ڱ��� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : MC_ReadParameter()
 *
 * @param	siAxis			: �� ID
 * @param	*psiPgratio		: Pulse ���ֺ�
 * @param	*pdEgratio		: ���ڱ���
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (psiPgratio == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
	if (pdEgratio == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	// ECAT Ÿ���� ��� �������� ���ںи�� ���� �������� �ʰ�, ���� �����ϰ��� �״�� ������.
	// �Ʒ� �Լ����� fFeedbackScaleFactor ���� ������(����)����.
	MC_STATUS ms;
	double fFeedbackScaleFactor;
	ms = MC_ReadParameter(m_iBoardNo, siAxis, mcpFeedbackScaleFactor, &fFeedbackScaleFactor);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	//�� �κ� Ȯ�� �ʿ���.
	*pdEgratio = fFeedbackScaleFactor;
	*psiPgratio = 8;

	return NO_ERROR_SUCESS;
}

/**
 * �ӵ��� Servo�� ������ �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetVServoProperty(short siAxis, BOOL *pbControl, BOOL *pbPolar, BOOL bBootOpt)
{
	*pbControl = DEF_V_CONTROL;
	*pbPolar = DEF_UNIPOLAR;

	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� Pulse ��� ���¸� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetStepMode(short siAxis, BOOL *pbMode, BOOL bBootOpt)
{
	*pbMode = DEF_SIGN_PULSE;

	return NO_ERROR_SUCESS;
}
/**
 * ���� ���� Encoder �Է� ����� ��ǥ ������ �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * NMC Library : MC_ReadBoolParameter()
 *
 * @param	siAxis			: �� ID
 * @param	*pbEncDir		: Encoder �Է� ����, FALSE=ENCO_CW(�ð����, - Count)
 *												 TRUE =ENCO_CCW(�ݽð����, + Count)
 * @param	*pbCoorDir		: ��ǥ����, FALSE=CORD_CW(�ð����, + ��ǥ �̵�)
 *										TRUE =CORD_CCW(�ݽð����, - ��ǥ �̵�)
 * @param	bBootOpt		: (OPTION=FALSE) boot file���� ������ ����, TRUE=boot file���� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	/** �μ� Pointer ���� */
	if (pbEncDir == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** �μ� Pointer ���� */
	if (pbCoorDir == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	MC_STATUS ms;
	bool bComDir, bEncDir;
			
	//���ڴ� ���� ��������.
	ms = MC_ReadBoolParameter(m_iBoardNo, siAxis, mcpInvertFeedbackDir, &bEncDir);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	//���ɹ��� ��������
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
 * ������ ���� STOP EVENT, ESTOP EVENT ���� �� ���� �ð��� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::GetStopRate(short siAxis, BOOL bType, short *psiRate, BOOL bBootOpt)
{
	*psiRate = 0;

	return NO_ERROR_SUCESS;
}

/**
 * ��������� ����Ǵ� ���� Gain���� �д´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetSyncGain(long *plCoeff)
{
	*plCoeff = 0;

	return NO_ERROR_SUCESS;
}
/**
 * �ش� ���� �ӵ� �Ǵ� ��ġ�� ���� PID & FF Gain������ �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt)
{
	*plGain = 0;

	return NO_ERROR_SUCESS;
}
/**
 * �ش� ���� �������� �� �������� ��带 �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetIntegration(short siAxis, BOOL bType, BOOL *pbMode, BOOL bBootOpt)
{
	*pbMode = FALSE;

	return NO_ERROR_SUCESS;
}
/**
 * �ӵ����� Ȥ�� ��ũ ���ɿ� ���� Low Pass Filter Ȥ�� Notch Filter�� ���� Filter ���� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double *pdFilter, BOOL bBootOpt)
{
	*pdFilter = 0.0;

	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� ���� �� �ӵ��� �д´�. (��� ���� ���� ��)
 *
 * NMC Library : MC_ReadActualVelocity(), MC_ReadParameter()
 *
 * @param	siAxis			: �� ID
 * @param	bType			: �ӵ� ����, FALSE=�����ӵ���, TRUE=�ӵ���ɰ�
 * @param	*psiPulse		: �ӵ��� Pulse��
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetVelocity(short siAxis, BOOL bType, double *pdPulse)
{
#ifdef SIMULATION
	*pdPulse = 0;

	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;


	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pdPulse == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	MC_STATUS ms;
	double dVel = 0.0;

	/** ���� �ӵ� */
	if(bType == FALSE)
	{
		ms = MC_ReadActualVelocity(m_iBoardNo, siAxis, &dVel);
		if(ms != MC_OK)
			return MakeRS_NMC_Error(ms);

		*pdPulse = dVel;
	}
	/** ��� �ӵ� */
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
 * ������ Board�� �ະ ���ۿ��θ� �д´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetAxisRunStop(short siBdNum, short *psiState)
{
	*psiState = 0;
	return NO_ERROR_SUCESS;
}

/**
 * ���� ������ġ �� ��ǥ��ġ�� �д´�.
 *
 * NMC Library : MC_ReadActualPosition(), MC_ReadParameter()
 *
 * @param	siAxis			: �� ID
 * @param	bCommandType	: ��ġ ����, FALSE=������ġ, TRUE=��ǥ��ġ
 * @param	*pdPosition		: bType=FALSE�̸�, ������ ������ġ(Encoder)
 *							  bType=TRUE �̸�, ������ ��ǥ��ġ(Command)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetPosition(short siAxis, BOOL bCommandType, double *pdPosition)
{//
#ifdef SIMULATION
	*pdPosition = 0.0;

	return NO_ERROR_SUCESS;
#endif

	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �μ� Pointer ���� */
	if (pdPosition == NULL)
		return generateErrorCode(106090);	// ERR_MLIB_INVALID_POINTER

	/** ������ġ(Encoder) */
	if (bCommandType == FALSE)
	{
		MC_STATUS ms;
		ms = MC_ReadActualPosition(m_iBoardNo, siAxis, pdPosition);
		if ( ms != MC_OK )
			return MakeRS_NMC_Error(ms);
	}
	/** ��ǥ��ġ(Command) */
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
 * Motor�� ����ġ RPM�̳� ���� RPM�� �д´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetRpm(short siAxis, BOOL bType, short *psiRpm)
{
	*psiRpm = 0;

	return NO_ERROR_SUCESS;
}

/**
 * Board�� Sampling Rate�� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetControlTimer(short siBdNum, short *psiTime, BOOL bBootOpt)
{
	*psiTime = 0;

	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ǥ��ġ�� ������ġ�� ���̰��� ��ġ������ �д´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetError(short siAxis, double *pdError)
{
	*pdError = 0.0;

	return NO_ERROR_SUCESS;
}

/**
 * Ư�� ���� Encoder Feedback Data�� ������ �о���� �� ��� (50usec �ֱ� Update)
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetFastReadEncoder(short siAxis, BOOL *pbStatus)
{
	*pbStatus = FALSE;

	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� Analog Offset ���� �д´�.
 * boot file �Ǵ� �������� memory���� ���� �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetAnalogOffset(short siAxis, short *psiOffset, BOOL bBootOpt)
{
	*psiOffset = 0;

	return NO_ERROR_SUCESS;
}

/**
 * ��, ��� Port�� 32bit Data�� �д´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetIO(short siPort, BOOL bType, long *plValue)
{
	*plValue = 0;

	return NO_ERROR_SUCESS;
}

/**
 * ������ Analog ��/����� 12/16bit Data ���� �д´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetAnalog(short siChannel, BOOL bType, short *psiValue)
{
	*psiValue = 0;

	return NO_ERROR_SUCESS;
}

/**
 * Board�� �浹���� ����� ��뿩�θ� �д´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetCollisionPreventFlag(short siBdNum, BOOL *pbMode)
{
	*pbMode = FALSE;

	return NO_ERROR_SUCESS;
}

/**
 * �������� ���θ� �д´�.
 *
 * NMC Library : MC_ReadAxisStatus()
 *
 * @param	*pbState		: ���� ����, TRUE=����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::GetSyncControl(short siAxis, BOOL *pbState)
{
#ifdef SIMULATION
	*pbState = FALSE;
	
	return NO_ERROR_SUCESS;
#endif
	
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �μ� Pointer ���� */
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
 * Master��� Slave���� ������ġ�� �д´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetSyncPosition(double *pdMasterPos, double *pdSlavePos)
{
	*pdMasterPos = 0.0;
	*pdSlavePos = 0.0;

	return NO_ERROR_SUCESS;
}

/**
 * ���庰�� I/O Interrupt�� Enable/Diable�ϰų�, I/O Interrupt �߻� ��
 * STOP-EVENT�� E-STOP-EVENT�� �����࿡ �߻����� ���θ� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::IOInterrupt(short siID, short siType, BOOL bState, BOOL bBootOpt)
{

	return NO_ERROR_SUCESS;
}
/**
 * I/O Interrupt �߻� �� PC������ Interrupt�� �߻���ų�� ���θ� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::IOInterruptPCIRQ(short siBdNum, BOOL bState, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}
/**
 * PC Interrupt �߻� �� end of interrupt ��ȣ�� �߻���Ų��.
 * ���� ����
 */
ErrorInfo MRS_NMCLib::IOInterruptPCIRQ_EOI(short siBdNum)
{
	return NO_ERROR_SUCESS;
}
/**
 * ���� ���� PID ���� ���θ� �����Ѵ�.
 *
 * NMC Library : controller_idle(), controller_run()
 *
 * ���� ��
 */
ErrorInfo MRS_NMCLib::SetController(short siAxis, BOOL bState)
{
	return NO_ERROR_SUCESS;
}

/**
 * AMP Disable/Enable ���¸� �����Ѵ�.��
 *
 * NMC Library : MC_Power()
 *
 * @param	siAxis			: �� ID
 * @param	bState			: AMP Enable ����, TRUE=Enable
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetAmpEnable(short siAxis, BOOL bState)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
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
 * ���� �̵� �ְ�ӵ��� ��,���� �������� Limit�� �����Ѵ�. (boot file�� �ڵ� ����)
 *
 * NMC Library : MC_WriteParameter()
 *
 * @param	siAxis			: �� ID
 * @param	dVelocity		: �̵��ӵ�, 1 ~ 2047000 count/rev
 * @param	siAccel			: ��,���ӱ�����, 1 ~ 200, 10msec����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID VELOCITY VALUE (MOTIONLIB)
 *							  xx = INVALID ACCELERATE VALUE (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetVelLimit(short siAxis, double dVelocity, double dAccel, double dDecel)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** �̵��ӵ� ���� ���� */
	if ((dVelocity < 0.0) || (dVelocity > DEF_VEL_LIMIT))
		return generateErrorCode(106076);	// ERR_MLIB_INVALID_VELOCITY_VALUE

	/** �̵� ��,���ӵ� ���� ���� */
	if ((dAccel < 0) || (dAccel > DEF_ACCEL_LIMIT))
		return generateErrorCode(106112);	// ERR_MLIB_INVALID_ACCELERATE_VALUE

	/** �̵� ��,���ӵ� ���� ���� */
	if ((dDecel < 0) || (dDecel > DEF_ACCEL_LIMIT))
		return generateErrorCode(106112);	// ERR_MLIB_INVALID_ACCELERATE_VALUE

	// **1202 -> msec ������ ����
	dAccel = (short)dAccel/4;
	// **050112
	if(dAccel <= 0) dAccel = 1;

	// **1202 -> msec ������ ����
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
 * AMP Drive�� Fault �߻� �� ������ Event�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAmpFaultEvent(short siAxis, short siAction, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * AMP Enable�� Active Level�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAmpEnableLevel(short siAxis, BOOL bLevel, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * AMP Fault�� Active Level�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAmpFaultLevel(short siAxis, BOOL bLevel, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * AMP Reset�� Active Level�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAmpResetLevel(short siAxis, BOOL bLevel, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� AMP Drive�� Resolution�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAmpResolution(short siAxis, short siResolution, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� ���ֺ� ���� ���ڰ�, �и��� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetEncoderRatio(short siAxis, short siRatioA, short siRatioB, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� ȸ��/������ϴ� ����ȸ�� ������ �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetEndlessAx(short siAxis, BOOL bStatus, short siResolution, BOOL bType, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ����ȸ�� ���� �����̴� ������ �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetEndlessRange(short siAxis, double dRange, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� �� Encoder�� C�� �޽� �̿� ���θ� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetIndexRequired(short siAxis, BOOL bIndexReq, BOOL bBootOpt)
{
	//��������. �������� Ÿ�Կ� ���� C�� ��뿩�� ������.

	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� �ش� Motor ������ �����ϴ� ������ �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetMotorType(short siAxis, short siType, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� Feedback ��ġ�� Loop ���¸� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetAxisProperty(short siAxis, short siDevice, BOOL bLoop, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� ���ֺ�� ���ڱ��� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetRatioProperty(short siAxis, short siPgratio, double dEgratio, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * �ӵ��� Servo�� ������ �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetVServoProperty(short siAxis, BOOL bControl, BOOL bPolar, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� Pulse ��� ���¸� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetStepMode(short siAxis, BOOL bMode, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� Encoder �Է� ����� ��ǥ ������ �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * NMC Library : MC_WriteParameter()
 * �б� �����̹Ƿ� ���� �ȵ�
 *
 * @param	siAxis			: �� ID
 * @param	bEncDir			: Encoder �Է� ����, FALSE=ENCO_CW(�ð����, - count),
 *												 TRUE =ENCO_CCW(�ݽð����, + count)
 * @param	bCoorDir		: ��ǥ����, FALSE=CORD_CW(�ð����, +��ǥ�̵�),
 *										TRUE =CORD_CCW(�ݽð����, -��ǥ�̵�)
 * @param	bBootOpt		: (OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetEncoderDirection(short siAxis, BOOL bEncDir, BOOL bCoorDir, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
#if 0 
	//��������. �б����� �Ӽ����� �Ǿ� ����.
	MC_STATUS ms;
	//���ڴ� ���� ��������.
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpInvertFeedbackDir, (double)bEncDir);
	if (ms != MC_OK)
	{
		strLogMsg.Format(_T("SetEncoderDirection(): MC_WriteParameter(set an encoder input direction and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}

	//���ɹ��� ��������
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
 * I/O 8���� ����  ��,��� ��带 �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetIOMode(short siBdNum, BOOL bMode, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ġ���� �Ϸᰪ�� ��ġ���� �� ��ȣ Level�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * NMC Library : MC_WriteParameter()
 *
 * @param	siAxis			: �� ID
 * @param	dInPosition		: ��ġ ������
 * @param	bLevel			: ��ȣ Level, TRUE=HIGH, FALSE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetInPosition(short siAxis, double dInPosition, BOOL bLevel, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	MC_STATUS ms;
	//�������� ������ ����.
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpInPositionWindowSize, dInPosition);
	if(ms != MC_OK)
	{
		strLogMsg.Format(_T("SetInPosition(): MC_WriteParameter(set 'in_position' value and save in boot file) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
		SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
		return MakeRS_NMC_Error(ms);
	}
	
	//��ġ ���� �Ϸ� ���������� ��������.
#endif

	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� InPosition ��ȣ ��뿩�θ� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetInpositionRequired(short siAxis, BOOL bReq, BOOL bBootOpt)
{
	//��������. ������ ����ϵ��� �Ǿ� ����.

	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ġ���� Limit���� Event�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetErrorLimit(short siAxis, double dLimit, short siAction, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ������ ���� STOP EVENT, ESTOP EVENT ���� �� ���� �ð��� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ��������
 */
ErrorInfo MRS_NMCLib::SetStopRate(short siAxis, BOOL bType, short siRate, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * Home, +/- ���� Limit Switch Active�� ������ Event�� ��ȣ Level�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * NMC Library : MC_WriteBoolParameter()
 *
 * @param	siAxis			: �� ID
 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
 * @param	siLimit			: ������ Event
 * @param	bLevel			: ��ȣ Level, TRUE=HIGH, FLASE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetSensorLimit(short siAxis, short siType, short siLimit, BOOL bLevel, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
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
		//���������� �����ϳ�, �̺�Ʈ�� ��������.
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
		//���������� �����ϳ�, �̺�Ʈ�� ��������.
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
		//���������� �����ϳ�, �̺�Ʈ�� ��������.
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
 * Home, +/- ���� Limit Switch Active�� ������ Event�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetSensorEvent(short siAxis, short siType, short siLimit, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
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
 * Home, +/- ���� Limit Switch Active�� ������ ��ȣ Level�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * @param	siAxis			: �� ID
 * @param	siType			: Sensor ����, 0=Home, 1=Positive, 2=Negative
 * @param	bLevel			: ��ȣ Level, TRUE=HIGH, FLASE=LOW
 * @param	bBootOpt		: (OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID SENSOR TYPE (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetSensorLevel(short siAxis, short siType, BOOL bLevel, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
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
 * +/- �������� Motor�� �̵��� �� �ִ� Limit ��ġ���� �� ��ġ���� �������� �� ����� Event�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * NMC Library : MC_WriteParameter()
 *
 * @param	siAxis			: �� ID
 * @param	bType			: ����, FALSE=Negative, TRUE=Positive
 * @param	dPosition		: ���� ��ġ��, +/-2147483647
 * @param	siLimit			: ����� Event
 * @param	bBootOpt		: (OPTION=FALSE) boot file�� ���� ����, TRUE=boot file�� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetSWLimit(short siAxis, BOOL bType, double dPosition, short siLimit, BOOL bBootOpt)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	/** Positive S/W ���� */
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
	/** Negative S/W ���� */
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
 * �ش� ���� �ӵ� �Ǵ� ��ġ�� ���� PID & FF Gain ������ �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� ���� ���� �� ���� ���� ��带 �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetIntegration(short siAxis, BOOL bType, BOOL bMode, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * �ӵ����� Ȥ�� ��ũ ���ɿ� ���� Low Pass Filter Ȥ�� Notch Filter�� ���� Filter ���� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetFilter(short siAxis, BOOL bCommandType, BOOL bFilterType, double dFilter, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ������ Board�� �ະ ���� ���θ� �����Ѵ�.
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAxisRunStop(short siBdNum, short siState)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� I/O bit�� HIGH(1)/LOW(0) ���·� �����.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetBit(short siBitNo, BOOL bValue)
{
	return NO_ERROR_SUCESS;
}

/**
 * 32bit�� I/O Data�� ��� Port�� ���� ��������.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetIO(short siPort, long lValue)
{
	return NO_ERROR_SUCESS;
}

/**
 * 12/16 bit Analog ��������� ��������.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetDacOut(short siAxis, short siValue)
{
	return NO_ERROR_SUCESS;
}

/**
 * �ش� ���� Analog Offset���� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAnalogOffset(short siAxis, short siOffset, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� ��������� ������ �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAnalogLimit(short siAxis, short siLimit, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� ��ġ �� ��ǥ ��ġ�� �����Ѵ�. (AMP Disable ���¿��� �����ϴ°� ����.)
 *
 * NMC Library : set_command(), set_position()
 *
 * @param	siAxis			: �� ID
 * @param	bType			: ��ġ ����, FALSE=������ġ, TRUE=��ǥ��ġ
 * @param	dPosition		: bType=FALSE�̸� ������ ������ġ, bType=TRUE�̸� ������ ��ǥ��ġ
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SetPosition(short siAxis, BOOL bType, double dPosition)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

#ifndef SIMULATION
	MC_STATUS ms;
	//ECAT�� ������ġ�� ������ġ�� �����ϰ� ������.
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
 * Ư�� ���� Encoder Feedback Data�� ������ �о���� �� ��� (50usec �ֱ� Update)
 * ��� ����
*/
ErrorInfo MRS_NMCLib::SetFastReadEncoder(short siAxis, BOOL bStatus)
{
	return NO_ERROR_SUCESS;
}
/**
 * ����ڰ� Motion ���� S/W�� ��ü �����Ͽ� �ý����� ���۽�ų �� �ֵ��� �������ִ� ���
 * ��� ����
 */
ErrorInfo MRS_NMCLib::SetInterpolation(short siLen, short *psiAxes, long *plDelt, short siFlag)
{
	return NO_ERROR_SUCESS;
}

/**
 * �浹���� ����� ����� Mastr/Slave�� �� �浹���� �Ÿ� �� ���� (+, -, >, <)�� �����Ѵ�.
 * ��� ����
*/
ErrorInfo MRS_NMCLib::SetCollisionPrevent(short siMasterAx, short siSlaveAx,
									BOOL bAddSub, BOOL bNonEqual, double dPosition, BOOL bState)
{
	return NO_ERROR_SUCESS;
}

/**
* �浹���� ����� ��뿩�θ� �����Ѵ�.
* ��� ����
 */
ErrorInfo MRS_NMCLib::SetCollisionPreventFlag(short siBdNum, BOOL bMode)
{
	return NO_ERROR_SUCESS;
}

/**
 * Board DPRAM Address�� �����Ѵ�.
 * ��� ����
*/
ErrorInfo MRS_NMCLib::SetDpramAddress(short siBdNum, long lAddr)
{
	return NO_ERROR_SUCESS;
}

/**
 * ����ġ Motor�� Type�� �����Ѵ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAbsEncoderType(short siAxis, short siType)
{
	return NO_ERROR_SUCESS;
}

/**
 * ����ġ Motor�� �����Ѵ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetAbsEncoder(short siAxis)
{
	return NO_ERROR_SUCESS;
}

/**
 * Servo Linear Flag ���¸� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetServoLinearFlag(short siAxis, short siFlag, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}
/**
 * �������� ���θ� �����Ѵ�.
 *
 * �Լ� ���� ���� ����
 */
ErrorInfo MRS_NMCLib::SetSyncControl(BOOL bState)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���������� Master��� Slave���� �����Ѵ�.
 *
 * ���� �Լ� ���� ����
 */
ErrorInfo MRS_NMCLib::SetSyncMapAxes(short siMasterAx, short siSlaveAx)
{
	return NO_ERROR_SUCESS;
}

/**
 * ��������� ����Ǵ� ���� Gain���� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetSyncGain(short siCoeff, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * Board�� Sampling Rate�� �����Ѵ�.
 * boot file �Ǵ� �������� memory�� ������ �� �ִ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetControlTimer(short siBdNum, short siTime, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/*
// MMC Board Type�� ���� Sampling Time ���� �ڵ�
// Sampling Time�� 4ms�� ���� �ڵ��Ѵ�.
// ���� ����
*/
ErrorInfo MRS_NMCLib::SetControlTimerForAx(short siAxis, short siTime, BOOL bBootOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ���� Ȥ�� ����, ��, ��ȣ���� ���� �� �ӵ��� ��,���ӵ��� �����Ѵ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetMoveSpeed(double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}
/**
 * �ڵ� ��,���� ����� ��뿩�θ� �����Ѵ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetSplAutoOff(short siBdNum, BOOL bState)
{
	return NO_ERROR_SUCESS;
}
/**
 * �ڵ� ��,���� ����� ��뿩�θ� �д´�.
 * Library�� �����Ǵ� �Լ��� ���� ����� ���� Data���� �о�´�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::GetSplAutoOff(short siBdNum, BOOL *pbState)
{
	return NO_ERROR_SUCESS;
}

/*
 * �� �̵� ���� (V-Stop) �̺�Ʈ�� ������. 
 */
ErrorInfo MRS_NMCLib::SetStopEvent(short siAxis)
{
	return NO_ERROR_SUCESS;
}

/**
 * �� �̵��� �����Ѵ�.
 * �Ϲ�����, �������, �ӵ��̵������� �����Ѵ�.
 *
 * NMC Library : MC_Halt()
 *
 * @param	siAxis			: �� ID
 * @param	siType			: ���� ����, 0=STOP, 1=ESTOP, 2=VSTOP
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID STOP TYPE (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
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

	/** �� ID ���� ���� */
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
 * 1�� �ӵ� Profile �̵� (�� 1�� ������ �̵� ������)
 *
 * NMC Library : MC_MoveAbsolute()
 *
 * @param	siAxis			: �� ID
 * @param	dPosition		: �̵��� ��ġ, Ȥ�� ���Ÿ�
 * @param	dVelocity		: �̵� �ӵ�
 * @param	siAccel			: �̵� ���ӵ�
 * @param	siDecel			: �̵� ���ӵ� (���Ī(t) Type�� ����)
 * @param	siType			: �̵� Type, 0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
 *										 1=S-Curve �ӵ� Profile, ������ǥ �̵�
 *										 2=��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
 *										 3=S-Curve �ӵ� Profile, ���Ÿ� �̵�
 *										 4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
 *										 5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
 *										 6=���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
 *										 7=���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�
 * @param	bWaitOpt		: (OPTION=FALSE) �̵� �Ϸ� ��� ����, TRUE=�̵��Ϸ�ɶ��������
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID MOVE TYPE (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::Move(short siAxis, double dPosition, double dVelocity, double dAccel, double dDecel, short siType, BOOL bWaitOpt, double dJerk)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID

	// **050112
	if(dAccel < 1) dAccel = 1;
	if(dDecel < 1) dDecel = 1;

	MC_STATUS ms = MC_OK;

	/** �̵� ������ ���� */
	switch (siType)
	{
	case DEF_MOVE_POSITION :		// ��ٸ��� �ӵ� Profile, ������ǥ �̵�

#ifndef SIMULATION
		ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcPositiveDirection, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveAbsolute(trapezoid velocity profile, absolute coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** �̵��Ϸ� ��� */
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

	case DEF_SMOVE_POSITION :		// S-Curve �ӵ� Profile, ������ǥ �̵�

#ifndef SIMULATION
		ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcPositiveDirection, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveAbsolutestart_move(trapezoid velocity profile, absolute coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** �̵��Ϸ� ��� */
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

	case DEF_MOVE_DISTANCE :		// ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�

#ifndef SIMULATION
		ms = MC_MoveRelative(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveRelative(trapezoid velocity profile, relative coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** �̵��Ϸ� ��� */
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

	case DEF_SMOVE_DISTANCE :		// S-Curve �ӵ� Profile, ���Ÿ� �̵�

#ifndef SIMULATION
		ms = MC_MoveRelative(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveRelative(S-Curve velocity profile, relative coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** �̵��Ϸ� ��� */
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

	case DEF_TMOVE_POSITION :		// ���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�

#ifndef SIMULATION
		ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcPositiveDirection, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveAbsolute(asymmetry trapezoid velocity profile, absolute coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** �̵��Ϸ� ��� */
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

	case DEF_TSMOVE_POSITION :		// ���Ī S-Curve �ӵ� Profile, ������ǥ �̵�

#ifndef SIMULATION
		ms = MC_MoveAbsolute(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcPositiveDirection, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveAbsolute(asymmetry S-Curve velocity profile, absolute coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** �̵��Ϸ� ��� */
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

	case DEF_TMOVE_DISTANCE :		// ���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�

#ifndef SIMULATION
		ms = MC_MoveRelative(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), 0, mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveRelativet_move(asymmetry trapezoid velocity profile, relative coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** �̵��Ϸ� ��� */
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

	case DEF_TSMOVE_DISTANCE :		// ���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�

#ifndef SIMULATION
		ms = MC_MoveRelative(m_iBoardNo, siAxis, motionRoundValue(dPosition), motionRoundValue(dVelocity), motionRoundValue(dAccel), motionRoundValue(dDecel), motionRoundValue(dJerk), mcAborting);
		if(ms != MC_OK)
		{
			strLogMsg.Format(_T("Move(): MC_MoveRelative(asymmetry S-Curve velocity profile, relative coordinates, no wait) Fail[AxisID=%d] - Error# is %d"), siAxis, MakeRS_NMC_Error(ms));
			SetErrorLevel(strLogMsg, 3, __FILE__, __LINE__);
			return MakeRS_NMC_Error(ms);
		}

		/** �̵��Ϸ� ��� */
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
 * ���� �ӵ� Profile ���� �̵� (���Ÿ� �̵��� �������� �ʴ´�.)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+-----+---------+
 *		| �ӵ� #1 | �ӵ� #2 | �ӵ� #3 | ... | �ӵ� #n |
 *		+---------+---------+---------+-----+---------+
 *		| ���� #1 | ���� #2 | ���� #3 | ... | ���� #n |
 *		+---------+---------+---------+-----+---------+
 *		| ���� #1 | ���� #2 | ���� #3 | ... | ���� #n |
 *		+---------+---------+---------+-----+---------+
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::MoveAll(short siLen, short *psiAxes, double *pdPosition, double *pdVelocity, short *psiAccel, short *psiDecel, short siType, BOOL bWaitOpt)
{
	return NO_ERROR_SUCESS;
}

/**
 * ������ n���� �־��� ��ǥ����ŭ �����̵��� �Ѵ�. (�ٸ� Board�� �� ��� �Ұ�)
 *  MapAxes(), SetMoveSpeed()���� ������ ����� �̵��Ѵ�.
 * ���� ����
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
 * ������ �׷��� ����� �־��� ��ǥ����ŭ �����̵��� �Ѵ�.
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| �ӵ� | ���ӵ� |
 *		+------+--------+
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::MoveNAxGr(short siGrpNum, short siLen, short *psiAxes, double *pdPosition,
						short siType, double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� �� ��� �̵��Ѵ�. (�� 1�� �����θ� ���� �����ϴ�.)
 *
 * NMC Library : MC_MoveVelocity()
 *
 * @param	siAxis			: �� ID
 * @param	dVelocity		: �̵� �ӵ�
 * @param	siAccel			: �̵� ���ӵ�
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::VMove(short siAxis, double dVelocity, double dAccel, double dDecel, double dJerk)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;

	/** �� ID ���� ���� */
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
 * ���� ��ġ���� �־��� 2/3���� ������ ��ǥ������ ��,�����ϸ鼭 ��ȣ CP Motion���� �̵��Ѵ�.
 *  (�ٸ� Board�� �� ��� �Ұ�)
 *  MapAxes(), SetMoveSpped()���� ������ ����� �̵��Ѵ�.
 *
 *		|---------------> siAxNum = n <---------------|
 *		+---------+---------+---------+-----+---------+
 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+-----+---------+
 *		+-----------+-----------+------+--------+----------+
 *		| ȸ���߽�X | ȸ���߽�Y | �ӵ� | ���ӵ� | ȸ������ |
 *		+-----------+-----------+------+--------+----------+
 *
 * NMC Library : spl_arc_move2(), spl_arc_move3()
 *
 * @param	siAxNum			: �� ��, 2=2��, 3=3��
 * @param	dCenterX		: ȸ�� �߽� X��ǥ
 * @param	dCenterY		: ȸ�� �߽� Y��ǥ
 * @param	*pdPoint		: �̵��� ��ǥ, 2���̸� 2�����迭, 3���̸� 3�����迭
 * @param	dVelocity		: �̵� �ӵ� (0.0=SetMoveSpeed()���� ������ �ӵ� ���)
 * @param	siAccel			: �̵� ���ӵ� (0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
 * @param	bDir			: ȸ������, FALSE=CIR_CW(�ð����), TRUE=CIR_CCW(�ݽð����)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SplArcMove(short siAxNum, double dCenterX, double dCenterY, double *pdPoint, double dVelocity, short siAccel, BOOL bDir)
{
	//��ǥ�� ���� ���ذ� �Ķ���� ���� ����. �ʿ�� ���� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ġ���� �־��� ��ǥ������ ��,�����ϸ鼭 ��ȣ CP Motion���� �̵��Ѵ�.
 *  (�ٸ� Board�� �� ��� ����)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+-----+---------+
 *		+-----------+-----------+------+--------+----------+
 *		| ȸ���߽�X | ȸ���߽�Y | �ӵ� | ���ӵ� | ȸ������ |
 *		+-----------+-----------+------+--------+----------+
 *
 * NMC Library : spl_arc_move2ax(), spl_arc_move3ax(), spl_arc_movenax()
 *
 * @param	siLen			: �� �� (>= 2)
 * @param	*psiAxes		: �� ID �迭 (�� ����ŭ ����)
 * @param	siCenterX		: ȸ�� �߽� X��ǥ
 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
 * @param	*pdPoint		: �̵��� ��ǥ (�� �� ��ŭ ����)
 * @param	dVelocity		: �̵� �ӵ� (0.0=SetMoveSpeed()���� ������ �ӵ� ���)
 * @param	siAccel			: �̵� ���ӵ� (0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
 * @param	bDir			: ȸ������, FALSE=CIR_CW(�ð����), TRUE=CIR_CCW(�ݽð����)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SplArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY, double *pdPoint, double dVelocity, short siAccel, BOOL bDir)
{
	//��ǥ�� ���� ���ذ� �Ķ���� ���� ����. �ʿ�� ���� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ġ���� �־��� ��ǥ������ ��,�����ϸ鼭 ��ȣ CP Motion���� �̵��Ѵ�.
 *  (�ٸ� Board�� �� ��� ����)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+-----+---------+
 *		+-----------+-----------+------+--------+----------+
 *		| ȸ���߽�X | ȸ���߽�Y | �ӵ� | ���ӵ� | ȸ������ |
 *		+-----------+-----------+------+--------+----------+
 *
 * NMC Library : spl_auto_arc_move2ax(), spl_auto_arc_move3ax(), spl_auto_arc_movenax()
 *
 * @param	siLen			: �� �� (>= 2)
 * @param	*psiAxes		: �� ID �迭 (�� ����ŭ ����)
 * @param	siCenterX		: ȸ�� �߽� X��ǥ
 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
 * @param	*pdPoint		: �̵��� ��ǥ (�� �� ��ŭ ����)
 * @param	dVelocity		: �̵� �ӵ� (0.0=SetMoveSpeed()���� ������ �ӵ� ���)
 * @param	siAccel			: �̵� ���ӵ� (0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
 * @param	bDir			: ȸ������, FALSE=CIR_CW(�ð����), TRUE=CIR_CCW(�ݽð����)
 * @param	bAutoFlag		: �ڵ� Auto CP, FALSE=Auto CP ����, TRUE=Auto CP ��
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SplAutoArcMoveNax(short siLen, short *psiAxes, double dCenterX, double dCenterY, double *pdPoint, double dVelocity, short siAccel, BOOL bDir, BOOL bAutoFlag)
{
	//��ǥ�� ���� ���ذ� �Ķ���� ���� ����. �ʿ�� ���� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ġ���� �־��� 2/3���� ������ ��ǥ������ ��,�����ϸ鼭 ���� CP Motion���� �̵��Ѵ�.
 *  (�ٸ� Board�� �� ��� �Ұ�)
 *  MapAxes(), SetMoveSpeed()���� ������ ����� �̵��Ѵ�.
 *
 *		|---------------> siAxNum = n <---------------|
 *		+---------+---------+---------+-----+---------+
 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| �ӵ� | ���ӵ� |
 *		+------+--------+
 *
 * NMC Library : spl_line_move1(), spl_line_move2(), spl_line_move3()
 *
 * @param	siAxNum			: �� ��, 1=1��, 2=2��, 3=3��
 * @param	*pdPoint		: �̵��� ��ǥ, 1���̸� 1�����迭, 2���̸� 2�����迭, 3���̸� 3�����迭
 * @param	dVelocity		: �̵� �ӵ� (0.0=SetMoveSpeed()���� ������ �ӵ� ���)
 * @param	siAccel			: �̵� ���ӵ� (0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SplLineMoveN(short siAxNum, double *pdPoint, double dVelocity, short siAccel)
{
	//��ǥ�� ���� ���ذ� �Ķ���� ���� ����. �ʿ�� ���� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ġ���� �־��� ��ǥ������ ��,�����ϸ鼭 ���� CP Motion���� �̵��Ѵ�.
 *  (�ٸ� Board�� �� ��� ����)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| �ӵ� | ���ӵ� |
 *		+------+--------+
 *
 * NMC Library : spl_line_move1ax(), spl_line_move2ax(), spl_line_move3ax(), spl_line_movenax()
 *
 * @param	siLen			: �� �� (>= 1)
 * @param	*psiAxes		: �� ID �迭 (�� �� ��ŭ ����)
 * @param	*pdPoint		: �̵��� ��ǥ (�� �� ��ŭ ����)
 * @param	dVelocity		: �̵� �ӵ� (0.0=SetMoveSpeed()���� ������ �ӵ� ���)
 * @param	siAccel			: �̵� ���ӵ� (0=SetMoveSpeed()���� ������ ��,���ӵ� ���)
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SplLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel)
{
	//��ǥ�� ���� ���ذ� �Ķ���� ���� ����. �ʿ�� ���� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * ���� ��ġ���� �־��� ��ǥ������ �ڵ� ��,���� ���θ� ���ڷ� �����޾Ƽ� ���� CP Motion���� �̵��Ѵ�.
 *  (�ٸ� Board�� �� ��� ����)
 *
 *		|----------------> siLen = n <----------------|
 *		+---------+---------+---------+-----+---------+
 *		| �� ID#1 | �� ID#2 | �� ID#3 | ... | �� ID#n |
 *		+---------+---------+---------+-----+---------+
 *		| ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| �ӵ� | ���ӵ� |
 *		+------+--------+
 *
 * @param	siLen			: �� ��
 * @param	*psiAxes		: �� ID �迭
 * @param	*pdPoint		: �̵��� ��ǥ (�� �� ��ŭ ����)
 * @param	dVelocity		: �̵� �ӵ�, 0.0=�������� �ӵ��� �̵�
 * @param	siAccel			: �̵� ���ӵ�, 0=�������� ���ӵ��� �̵�
 * @return	Error Code		: 0 = SUCCESS, �� �� = Error
 */
ErrorInfo MRS_NMCLib::SplAutoLineMoveNax(short siLen, short *psiAxes, double *pdPoint, double dVelocity, short siAccel, BOOL bAutoFlag)
{
	//��ǥ�� ���� ���ذ� �Ķ���� ���� ����. �ʿ�� ���� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * ��, ��ȣ �̵� �� ���ּӵ��� �����Ѵ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SetArcDivision(double dDegree)
{
	return NO_ERROR_SUCESS;
}

/**
 * �־��� �߽ɿ��� ������ ������ŭ ��ȣ�� �׸��� ������ �����Ѵ�.
 *  (�ٸ� Board�� �� ��� �Ұ�)
 *  MapAxes(), SetMoveSpeed()���� ������ ����� �̵��Ѵ�.
 *
 * NMC Library : arc_2()
 *
 * @param	siCenterX		: ȸ�� �߽� X��ǥ
 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
 * @param	dAngle			: ȸ�� ����
 * @return	Error Code		: 0 = SUCCESS
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::Arc2(double dXCenter, double dYCenter, double dAngle)
{
	//��ǥ ���ذ�, �Ķ���� ������. �ʿ�� ���� Ȯ���Ͽ� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * �־��� �߽ɿ��� ������ ������ŭ ��ȣ�� �׸��� ������ �����Ѵ�.
 * ���� Board�� ���� �ƴϸ� Error ó����.
 *  (�ٸ� Board�� �� ��� �Ұ�)
 *
 * NMC Library : arc_2ax()
 *
 * @param	siAxis1			: ��1 ID
 * @param	siAxis2			: ��2 ID
 * @param	siCenterX		: ȸ�� �߽� X��ǥ
 * @param	siCenterY		: ȸ�� �߽� Y��ǥ
 * @param	dAngle			: ȸ�� ����
 * @param	dVelocity		: �̵� �ӵ�
 * @param	siAccel			: �̵� ���ӵ�
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID AXIS ID IN COMMON BOARD (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::Arc2Ax(short siAxis1, short siAxis2, double dXCenter, double dYCenter, double dAngle, double dVelocity, short siAccel)
{
	//��ǥ ���ذ�, �Ķ���� ������. �ʿ�� ���� Ȯ���Ͽ� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * �־��� �簢���� ���ο� ������ ���̸� �̿��Ͽ� ������ġ���� ����̵��� �ϸ鼭 CP Motion���� �簢���� �׸���.
 *
 *			+--------------------+ pdPoint (X, Y)
 *			|					 |
 *			|					 |
 *			|					 |
 *			|					 |
 *			|					 |
 *			+--------------------+
 *		������ġ
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::RectMove(short siAxis1, short siAxis2, double *pdPoint, double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * ������ġ���� �־��� ��ġ�� �����ϸ鼭 CP Motion���� �̵��Ѵ�.
 *
 *				  |----------------> siLen = n <----------------|
 *		+---------+---------+---------+---------+-----+---------+
 *		| �� ID#1 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		| �� ID#2 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		| �� ID#3 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| �ӵ� | ���ӵ� |
 *		+------+--------+
 *
 * NMC Library : spl_move()
 *
 * @param	siLen			: ��ġ Data ����, �ִ� 30
 * @param	siAxis1			: ��1 ID
 * @param	siAxis2			: ��2 ID
 * @param	siAxis3			: ��3 ID
 * @param	*pdPointX		: X��ǥ �迭 (��ġ Data ������ŭ ����)
 * @param	*pdPointY		: Y��ǥ �迭 (��ġ Data ������ŭ ����)
 * @param	*pdPointZ		: Z��ǥ �迭 (��ġ Data ������ŭ ����)
 * @param	dVelocity		: �̵� �ӵ�
 * @param	siAccel			: �̵� ���ӵ�
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID POSITION DATA NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SplMove(short siLen, short siAxis1, short siAxis2, short siAxis3,
					  double *pdPointX, double *pdPointY, double *pdPointZ,
					  double dVelocity, short siAccel)
{
	//��ǥ ���ذ�, �Ķ���� ������. �ʿ�� ���� Ȯ���Ͽ� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * SplMoveX()�� �ʿ��� ��ġ��θ� �����Ѵ�.
 *
 *				  |----------------> siLen = n <----------------|
 *		+---------+---------+---------+---------+-----+---------+
 *		| �� ID#1 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		| �� ID#2 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		| �� ID#3 | ��ġ #1 | ��ġ #2 | ��ġ #3 | ... | ��ġ #n |
 *		+---------+---------+---------+---------+-----+---------+
 *		+------+--------+
 *		| �ӵ� | ���ӵ� |
 *		+------+--------+
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::SplMoveData(short siSplNum, short siLen, short siAxis1, short siAxis2,
						  short siAxis3, double *pdPoint1, double *pdPoint2,
						  double *pdPoint3, double dVelocity, short siAccel)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� 3���� Spline Motion���� SplMoveData()���� ������ ��ġ�� �����ϸ鼭 ���� �̵��Ѵ�.
 *
 * NMC Library : spl_movex()
 *
 * @param	siSplNum		: Spline Motion ��ȣ, 1 ~ 20
 * @param	siAxis1			: ��1 ID
 * @param	siAxis2			: ��2 ID
 * @param	siAxis3			: ��3 ID
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID SPLINE MOTION NUMBER (MOTIONLIB)
 *							  xx = INVALID AXIS ID (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::SplMovex(short siSplNum, short siAxis1, short siAxis2, short siAxis3)
{
	//��ǥ ���ذ�, �Ķ���� ������. �ʿ�� ���� Ȯ���Ͽ� �ۼ�.

	return NO_ERROR_SUCESS;
}

/**
 * �� Board�� ROM Version�� �д´�.
 *
 * NMC Library : version_chk()
 *
 * @param	siBdNum			: Board ID (0 ~ 7)
 * @param	*psiVersion		: ROM Version, 101 => 1.01
 * @return	Error Code		: 0 = SUCCESS
 *							  xx = INVALID MOTION BOARD ID (MOTIONLIB)
 *							  xx = INVALID POINTER (MOTIONLIB)
 *							  �� �� = Ÿ �Լ� Return Error
 */
ErrorInfo MRS_NMCLib::VersionCheck(short siBdNum, short *psiVersion)
{
	return NO_ERROR_SUCESS;
}

/**
 * �ش� Error Code�� Error Message�� ��ȯ�Ѵ�.
 *
 * NMC Library : error_message(), _error_message()
 *
 * @param	siCode			: Error Code
 * @param	*pcDst			: Error Message, m_iErrorBase + ERR_MAX_ERROR_LEN(80)���� ũ�ų� ���ƾ� �Ѵ�.
 * @return	Error Code		: 0 = SUCCESS
 *							  �� �� = Ÿ �Լ� Return Error
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
 * Position Compare Board�� �ʱ�ȭ�Ѵ�. (�� 2�� ����� ���� ���� Board�� ���̾�� �Ѵ�.)
 * ���� Board�� ���� �ƴϸ� Error ó����.
 * Board ������ ����Ѵ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::PositionCompareInit(short siIndexSel, short siAxis1, short siAxis2)
{
	return NO_ERROR_SUCESS;
}

/**
 * Position Compare�� �����Ѵ�. (Standard Type)
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::PositionCompare(short siIndexSel, short siIndexNum, short siBitNo, short siAxis1,
							  short siAxis2, BOOL bLatch, short siFunction, short siOutMode,
							  double dPosition, long lTime)
{
	return NO_ERROR_SUCESS;
}

/**
 * Position Compare�� �����Ѵ�. (Interval Type)
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::PositionCompareInterval(BOOL bDir, short siAxis, short siBitNo, double dStartPos, double dLimitPos,
							  long lInterval, long lTime)
{
	return NO_ERROR_SUCESS;
}

/**
 * Position Compare ������ �� ������ ���θ� �����Ѵ�.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::PositionCompareEnable(short siBdNum, BOOL bFlag)
{
	return NO_ERROR_SUCESS;
}

/**
 * ���� ����
 */
ErrorInfo MRS_NMCLib::PositionCompareClear(short siBdNum, short siIndexSel)
{
	return NO_ERROR_SUCESS;
}

/**
 * Position Compare ������ ���� Encoder ���� �о��.
 *
 * ���� ����
 */
ErrorInfo MRS_NMCLib::PositionCompareRead(short siIndexSel, short siAxis, double *pdPosition)
{
	return NO_ERROR_SUCESS;
}

// �ݿø� Function
double MRS_NMCLib::motionRoundValue(double dVal)
{
	return (ceil(dVal + 0.5) - 1.0);
}

// NMC Library�� �����ϴ� ���� �ڵ带 System Error���·� �����
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

//���� ���� ����� ���������� üũ(��Ǹ���� ��� ù��° �࿡���� üũ�ϸ� ��)
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

// ����(���� ����) ����
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

// ����(���� ����) ����
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

// ����(���� ����)�� Ȱ��ȭ �Ǿ� �ִ��� üũ. ��ȯ���� true�϶� ��ũ�� �ɷ��ִ� ������.
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
		//���尡 Ȱ��ȭ �Ǿ� �ִ»���.
		return TRUE;
	} 
	else
	{
		//���尡 Ȱ��ȭ �Ǿ� ���� ���� ����.
		return FALSE;
	}
}

ErrorInfo MRS_NMCLib::HomeSetConfig(short siAxis, int iHomeMode, BOOL bDirection)
{
	//�������� - �б����� �Ӽ����� �Ǿ� ����
	// Revision ������.
	// NMC Manager���� ����
	
	CString strLogMsg;
	int iBoardID;
	iBoardID = GetBoardID(siAxis);
	MC_STATUS ms;
	
	// ���� ���� Ÿ�� ����
	ms = MC_WriteIntParameter(iBoardID, siAxis, mcpHomingType, (int)iHomeMode);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	// ���� ���� ���� ����
	// Direction �ݴ�� 0 = ������, 1= ������ 
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
	
	// ���� ���� 1�� �ӵ� ����
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingVelocity, dFastVelocity);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);
	
	// ���� ���� 2�� �ӵ� ����
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingCreepVelocity, dSlowVelocity);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// ���� ���� ���ӵ� ����
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingAcceleration, dAccel);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// ���� ���� ���ӵ� ����
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingDeceleration, dAccel*100);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// ���� ���� ���ӵ� ����
	ms = MC_WriteParameter(m_iBoardNo, siAxis, mcpHomingJerk, dDecel*1000);
	if (ms != MC_OK)
		return MakeRS_NMC_Error(ms);

	// ���� ���� Offset ����
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
	
	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
	// **050112
	if(dAccel < 1) dAccel = 1;
	if(dDecel < 1) dDecel = 1;
	
	MC_BUFFER_MODE mBufferMode;
	if(iBufferMode == 0) mBufferMode = mcAborting;			// ���� �̵� �߰� �������� ������ ����
	if(iBufferMode == 1) mBufferMode = mcBuffered;			// ���� �̵� �� ���� �ٽ� ����
	if(iBufferMode == 2) mBufferMode = mcBlendingLow;		// ���� �ӵ� �������� ������
	if(iBufferMode == 3) mBufferMode = mcBlendingPrevious;	// ���� �ӵ� �������� ������
	if(iBufferMode == 4) mBufferMode = mcBlendingNext;		// ���� �ӵ� �������� ������
	if(iBufferMode == 5) mBufferMode = mcBlendingHigh;		// ���� �ӵ� �������� ������

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

// ���� ���� ID ��ȯ
ErrorInfo MRS_NMCLib::AmpOn_for_SyncGroup(int iAxisID)
{
	ErrorInfo errorInfo = NO_ERROR_SUCESS;
	CString strLogMsg;
	
	/** �� ID ���� ���� */
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
	
	
	/** �� ID ���� ���� */
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
	
	/** �� ID ���� ���� */
	if((iAxisID < 1) || (iAxisID > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
#ifndef SIMULATION
	/** �� AMP Fault Reset ���� */
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
	
	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
#ifndef SIMULATION
	/** �� AMP Fault Reset ���� */
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
	
	/** �� ID ���� ���� */
	if((siAxis < 1) || (siAxis > DEF_AXIS_NODE_MAX_NO))
		return generateErrorCode(106109);	// ERR_MLIB_INVALID_AXIS_ID
	
#ifndef SIMULATION
	/** �� AMP Fault Reset ���� */
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

	/** �� ID ���� ���� */
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