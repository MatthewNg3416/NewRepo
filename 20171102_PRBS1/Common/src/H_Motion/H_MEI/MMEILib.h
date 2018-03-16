/* 
 * Motion Component
 *
 * Copyright 2004 by Mechatronics Center, Samsung Electronics, Inc.,
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Smasung Electronics, Inc. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

/**
 * CMEILib.h : This file defines the class of MEI Motion libraries.
 *			   이 파일은 MEI Motion Library를 위한 Class를 정의한다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#ifndef MMEILIB_H
#define MMEILIB_H

#include "DefMotionStruct.h"

#include "IMotionLib.h"		
#include "MMotionBoard.h"

/**
 * This class is defined class of MEI motion libraries.
 * 이 Class는 MEI Motion Library를 위한 Class로 정의한다.
 * 
 * 본 Class는 이번 단계에서 구현하지 앟는 Class로 CMMCLib Class와 IMotionLib Interface 관계를
 * 보여주기 위해 Class 형태만 제공하는 Class이다.
 * MEI Motion Board의 사용 여부는 향후 결정하도록 한다. (T7 종료 후)
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */
class MMEILib : public IMotionLib
{

private:	// attribute

    /** Motion Board 구성 정보 
     * @link aggregation
     * @supplierCardinality 1*/
	MMotionBoard *boardConfig;

	/** MEI Motion Library Object Number */
	static	int	iObjNo;


public:		// Constructor & Destructor

	/**
	 * Constructor : create and clear data
	 *				 Data를 생성하고 초기화한다.
	 * @stereotype constructor
	 */
	MMEILib ();

	/**
	 * Destructor : Delete inner object
	 *				내부 개체를 소멸한다.
	 * @stereotype destructor
	 */
	~MMEILib ();

public:		// interface operation
	
	virtual int SetUseAxis (short siAxisID, BOOL bState);

	virtual int GetUseAxis (short siAxisID, BOOL *pbState);

	virtual int SetMotorParam (short siAxisID, SMotionAxis *pmAx, BOOL bBootOpt = FALSE);

	virtual int GetMotorParam (short siAxisID, SMotionAxis *pmAx);

	virtual int SetBoardParam (int iObjectID, 
							   SMotionBoard MotionBd,
							   int iErrorBase,
							   CString strFullFileName,
							   BYTE ucLevel, 
							   int iDays);

	virtual int GetBoardParam (SMotionBoard *pmBd);

	virtual int GetBoardNum (short *psiBdNum);

	virtual int Initialize (short siBdNum, long *plAddress);

	virtual int GetBoardInit (BOOL *pbInit);

	virtual int GetAxes (short siBdNum, short *psiAxes);

	virtual int MapAxes (short siAxNum, short *psiMapArray);

	virtual int Dwell (short siAxis, long *plDuration = NULL);

	virtual int IOTrigger (short siAxis, short siBitNo, BOOL bState);

	virtual int InCommand (short siAxis, short siType);

	virtual int AllDone (BOOL *pbStatus);

	virtual int WaitDone(short siAxis, short siMode);

	virtual int WaitForDone(short siAxis, short *psiAxis = NULL);

	virtual int SetAmpFaultEnable(short siAxis, BOOL bState);

	virtual int GetAllStatus(short siAxis, short *psiStatus, long *plStatus, double *pdStatus);

	virtual int GetAxisSource(short siAxis, short *psiState);

	virtual int GetSensorStatus(short siAxis, short siType, BOOL *pbState);

	virtual int GetAmpEnable(short siAxis, BOOL *pbState);

	virtual int GetAmpEnableLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = FALSE);

	virtual int GetAxisState(short siAxis, short *psiState);

	virtual int GetAxisLatchStatus(short siBdNum, BOOL *pbState);

	virtual int ClearStatus(short siAxis);

	virtual int ClearFrames(short siAxis);

	virtual int FramesInterpolation(short siAxis, short *psiFrameNo);

	virtual int FramesLeft(short siAxis, short *psiFrameNo);

	virtual int Latch(short siBdNum, BOOL bType, BOOL bState = 0);

	virtual int GetLatchedPosition(short siAxis, double *pdPosition);

	virtual int CompensationPos(short siLen, short *psiAxes, double *pdDist, short *psiAccel);

	virtual int GetAbsEncoderType(short siAxis, short *psiType);

	virtual int GetVelLimit(short siAxis, double *pdVelocity, short *psiAccel);

	virtual int GetAmpFaultStatus(short siAxis, BOOL *pbStatus);

	virtual int GetAmpFaultEvent(short siAxis, short *psiAction, BOOL bBootOpt = 0);

	virtual int GetAmpEanbleLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = 0);

	virtual int GetAmpFaultLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = 0);

	virtual int GetAmpResetLevel(short siAxis, BOOL *pbLevel, BOOL bBootOpt = 0);

	virtual int GetAmpResolution(short siAxis, short *psiResolution, BOOL bBootOpt = 0);

	virtual int GetEncoderRatio(short siAxis, short *psiRatioA, short *psiRatioB, BOOL bBootOpt=FALSE);

	virtual int GetEndlessAx(short siAxis, BOOL *pbStatus, BOOL bType, BOOL bBootOpt = 0);

	virtual int GetEndlessRange(short siAxis, double *pdRange, BOOL bBootOpt = 0);

	virtual int GetInPosition(short siAxis, double *pdInPosition, short *psiLevel, BOOL bBootOpt=0);

	virtual int GetInpositionRequired(short siAxis, BOOL *pbReq, BOOL bBootOpt = 0);

	virtual int GetErrorLimit(short siAxis, double *pdLimit, short *psiAction, BOOL bBootOpt = 0);

	virtual int GetIndexRequired(short siAxis, BOOL *pbIndexReq, BOOL bBootOpt = 0);

	virtual int GetIOMode(short siBdNum, BOOL *pbMode, BOOL bBootOpt = 0);

	virtual int GetSensorLimit(short siAxis,
							   short siType, 
							   short *psiLimit, 
							   short *psiLevel,
							   BOOL bBootOpt = 0);

	virtual int GetSWLimit(short siAxis,
						   BOOL bType, 
						   double *pdPosition, 
						   short *psiLimit,
						   BOOL bBootOpt = 0);

	virtual int GetMotorType(short siAxis, short *psiType, BOOL bBootOpt = 0);

	virtual int GetAxisProperty(short siAxis, short *psiDevice, BOOL *pbLoop, BOOL bBootOpt = 0);

	virtual int GetRatioProperty(short siAxis, 
								 short *psiPgratio,
								 double *pdEgratio,
								 BOOL bBootOpt = 0);

	virtual int GetVServoProperty(short siAxis, BOOL *pbControl, BOOL *pbPolar, BOOL bBootOpt = 0);

	virtual int GetStepMode(short siAxis, BOOL *pbMode, BOOL bBootOpt = 0);

	virtual int GetEncoderDirection(short siAxis, BOOL *pbEncDir, BOOL *pbCoorDir, BOOL bBootOpt = 0);

	virtual int GetStopRate(short siAxis, BOOL bType, short *psiRate, BOOL bBootOpt = 0);

	virtual int GetSyncGain(short *psiCoeff);

	virtual int GetGain(short siAxis, short siVelType, long *plGain, BOOL bBootOpt = 0);

	virtual int GetIntegration(short siAxis, BOOL bType, BOOL *pbMode, BOOL bBootOpt = 0);

	virtual int GetFilter(short siAxis, 
						  BOOL bCommandType, 
						  BOOL bFilterType,
						  double *pdFilter,
						  BOOL bBootOpt=FALSE);

	virtual int GetVelocity(short siAxis, BOOL bType, short *psiPulse);

	virtual int GetAxisRunStop(short siBdNum, short *psiState);

	virtual int GetPosition(short siAxis, BOOL bType, double *pdPosition);

	virtual int GetRpm(short siAxis, short *psiRpm, BOOL bType);

	virtual int GetControlTimer(short siBdNum, short *psiTime, BOOL bBootOpt = 0);

	virtual int GetError(short siAxis, double *pError);

	virtual int GetFastReadEncoder(short siAxis, BOOL *pbStatus);

	virtual int GetAnalogOffset(short siAxis, short *psiOffset, BOOL bBootOpt = 0);

	virtual int GetIO(short siPort, BOOL bType, long *plValue);

	virtual int GetAnalog(short siChannel, BOOL bType, short *psiValue);

	virtual int GetCollisionPreventFlag(short siBdNum, BOOL *pbMode);

	virtual int GetSyncControl(BOOL *pbState);

	virtual int GetSyncPosition(double *pdMasterPos, double *pdSlavePos);

	virtual int IOInterrupt(short siID, short siType, BOOL bState, BOOL bBootOpt = 0);

	virtual int IOInterruptPCIRQ(short siBdNum, BOOL bState, BOOL bBootOpt = 0);

	virtual int IOInterruptPCIRQ_EOI(short siBdNum);

	virtual int SetController(short siAxis, BOOL bState);

	virtual int SetAmpEnable(short siAxis, BOOL bState);

	virtual int SetVelLimit(short siAxis, double dVelocity, short siAccel);

	virtual int SetAmpFaultEvent(short siAxis, short siAction, BOOL bBootOpt = 0);

	virtual int SetAmpEnableLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = 0);

	virtual int SetAmpFaultLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = 0);

	virtual int SetAmpResetLevel(short siAxis, BOOL bLevel, BOOL bBootOpt = 0);

	virtual int SetAmpResolution(short siAxis, short siResolution, BOOL bBootOpt = 0);

	virtual int SetEncoderRatio(short siAxis, short siRatioA, short siRatioB, BOOL bBootOpt=FALSE);

	virtual int SetEndlessAx(short siAxis, BOOL bStatus, short siResolution, BOOL bType,
							   BOOL bBootOpt = 0);

	virtual int SetEndlessRange(short siAxis, double dRange, BOOL bBootOpt = 0);

	virtual int SetIndexRequired(short siAxis, BOOL bIndexReq, BOOL bBootOpt = 0);

	virtual int SetMotorType(short siAxis, short siType, BOOL bBootOpt = 0);

	virtual int SetAxisProperty(short siAxis, short siDevice, BOOL bLoop, BOOL bBootOpt = 0);

	virtual int SetRatioProperty(short siAxis, short siPgratio, double dEgratio, BOOL bBootOpt = 0);

	virtual int SetVServoProperty(short siAxis, BOOL bControl, BOOL bPolar, BOOL bBootOpt = 0);

	virtual int SetStepMode(short siAxis, BOOL bMode, BOOL bBootOpt = 0);

	virtual int SetEncoderDirection(short siAxis, BOOL bEncDir, BOOL bCoorDir, BOOL bBootOpt = 0);

	virtual int SetIOMode(short siBdNum, BOOL bMode, BOOL bBootOpt = 0);

	virtual int SetInPosition(short siAxis, double dInPosition, BOOL bLevel, BOOL bBootOpt = 0);

	virtual int SetInpositionRequired(short siAxis, BOOL bReq, BOOL bBootOpt = 0);

	virtual int SetErrorLimit(short siAxis, double dLimit, short siAction, BOOL bBootOpt = 0);

	virtual int SetStopRate(short siAxis, BOOL bType, short siRate, BOOL bBootOpt = 0);

	virtual int SetSensorLimit(short siAxis,
							   short siType, 
							   short siLimit,
							   BOOL bLevel, 
							   BOOL bBootOpt = 0);

	virtual int SetSWLimit(short siAxis, 
						   BOOL bType,
						   double dPosition,
						   short siLimit,
						   BOOL bBootOpt = 0);

	virtual int SetGain(short siAxis, BOOL bVelType, long *plGain, BOOL bBootOpt = 0);

	virtual int SetIntegration(short siAxis, BOOL bType, BOOL bMode, BOOL bBootOpt = 0);

	virtual int SetFilter(short siAxis,
						  BOOL bCommandType,
						  BOOL bFilterType,
						  double dFilter,
						  BOOL bBootOpt=FALSE) = 0;

	virtual int SetAxisRunStop(short siBdNum, short siState);

	virtual int SetBit(short siBitNo, BOOL bValue);

	virtual int SetIO(short siPort, long lValue);

	virtual int SetDacOut(short siAxis, short siValue);

	virtual int SetAnalogOffset(short siAxis, short siOffset, BOOL bBootOpt = 0);

	virtual int SetAnalogLimit(short siAxis, short siLimit, BOOL bBootOpt=FALSE);

	virtual int SetPosition(short siAxis, BOOL bType, double dPosition);

	virtual int SetFastReadEncoder(short siAxis, BOOL bStatus);

	virtual int SetInterpolation(short siLen, short *psiAxes, long *plDelt, short siFlag);

	virtual int SetCollisionPrevent(short siMasterAx,
									short siSlaveAx,
									short siAddSub,
									short siNonEqual,
									double dPosition);

	virtual int SetCollisionPreventFlag(short siBdNum, BOOL bMode);

	virtual int SetDpramAddress(short siBdNum, long lAddr);

	virtual int SetAbsEncoderType(short siAxis, short siType);

	virtual int SetAbsEncoder(short siAxis);

	virtual int SetServoLinearFlag(short siAxis, short siFlag, BOOL bBootOpt=FALSE);

	virtual int SetSyncControl(BOOL bState);

	virtual int SetSyncMapAxes(short siMasterAx, short siSlaveAx);

	virtual int SetSyncGain(short siCoeff, BOOL bBootOpt=0);

	virtual int SetControlTimer(short siBdNum, short siTime, BOOL bBootOpt=0);

//#if FALSE	// MC Library Header File에 없음 (MMCWFI30.h)
//	virtual int PositionIOClear(short siPosNum = 0);
//#endif

//#if FALSE	// MC Library Header File에 없음 (MMCWFI30.h)
//	virtual int PositionIoOnoff(short siPosNum, 
//								short siIONum,
//								short siAxis,
//								double dPosition,
//								BOOL bEncFlag);
//#endif

	virtual int SetMoveSpeed(double dVelocity, short siAccel);

	virtual int SetSplAutoOff(short siBdNum, BOOL bState);

	virtual int GetSplAutoOff(short siBdNum, BOOL *pbState);

	virtual int SetStop(short siAxis, short siType);

	virtual int Move(short siAxis, 
					 double dPosition, 
					 double dVelocity,
					 short siAccel,
					 short siDecel,
					 short siType, 
					 BOOL bWaitOpt=0);

	virtual int MoveAll(short siLen, 
						short *psiAxes, 
						double *pdPosition,
						double *pdVelocity,
						short *psiAccel,
						short *psiDecel, 
						short siType, 
						BOOL bWaitOpt=0);

	virtual int MoveN(short siLen, double *pdPosition, short siType);

	virtual int MoveNAx(short siLen, 
						short *psiAxes, 
						double *pdPosition,
						short siType,
						double dVelocity, 
						short siAccel);

	virtual int MoveNAxGr(short siGrpNum, 
						  short siLen, 
						  short *psiAxes,
						  double *pdPosition,
						  short siType,
						  double dVelocity, 
						  short siAccel);

	virtual int VMove(short siAxis, double dVelocity, short siAccel);

	virtual int SplArcMove(short siAxNum, 
						   double dCenterX,
						   double dCenterY,
						   double *pdPoint,
						   double dVelocity, 
						   short siAccel,
						   BOOL bDir);

	virtual int SplArcMoveNax(short siLen,
							  short *psiAxes,
							  double dCenterX,
							  double dCenterY,
							  double *pdPoint,
							  double dVelocity, 
							  short siAccel,
							  BOOL bDir);

	virtual int SplLineMoveN(short siAxNum, double *pdPoint, double dVelocity, short siAccel);

	virtual int SplLineMoveNax(short siLen, 
							   short *psiAxes,
							   double *pdPoint,
							   double dVelocity,
							   short siAccel);

	virtual int SetArcDivision(double dDegree);

	virtual int Arc2(double dXCenter, double dYCenter, double dAngle);

	virtual int Arc2Ax(short siAxis1, 
					   short siAxis2,
					   double dXCenter, 
					   double dYCenter,
					   double dAngle,
					   double dVelocity, 
					   short siAccel);

	virtual int RectMove(short siAxis1,
						 short siAxis2,
						 double *pdPoint,
						 double dVelocity, 
						 short siAccel);

	virtual int SplMove(short siLen, 
						short siAxis1, 
						short siAxis2, 
						short siAxis3,
						double *pdPointX,
						double *pdPointY,
						double *pdPointZ,
						double dVelocity, 
						short siAccel);

	virtual int SplMoveData(short siSplNum, 
							short siLen, 
							short siAxis1,
							short siAxis2,
							short siAxis3,
							double *pdPoint1,
							double *pdPoint2,
							double *pdPoint3,
							double dVelocity,
							short siAccel);

	virtual int SplMovex(short siSplNum, short siAxis1, short siAxis2, short siAxis3);

	virtual int VersionCheck(short siBdNum, short *psiVersion);

	virtual int ErrorMessage(short siCode, char *pcDst);

	virtual int PositionCompareInit(short siIndexSel, short siAxis1, short siAxis2);

	virtual int PositionCompare(short siIndexSel,
								short siIndexNum, 
								short siBitNo, 
								short siAxis1,
								short siAxis2, 
								BOOL bLatch, 
								short siFunction, 
								short siOutMode,
								double dPosition,
								long lTime);

	virtual int PositionCompareInterval(BOOL bDir, 
										short siAxis,
										short siBitNo, 
										double dStartPos,
										double dLimitPos,
										long lInterval, 
										long lTime);

	virtual int PositionCompareEnable(short siBdNum, BOOL bFlag);

	virtual int PositionCompareClear(short siBdNum, short siIndexSel);

	virtual int PositionCompareRead(short siIndexSel, short siAxis, double *pdPosition);

};
#endif // MMEILIB_H

// End of File MMEILib.h
