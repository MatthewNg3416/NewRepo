/* 
 * Vision Calibration Component
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
 * MVisionCalibration.h : This file defines the implements class of vision calibration component.
 *			  이 파일은 vision calibration Class에 대한 implements class를 정의한다.
 *
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */

#ifndef MVISIONCALIBRATION_H
#define MVISIONCALIBRATION_H

#include "DefSystemConfig.h"
#include "ICommon.h"

class IVision;
class MPos_XY;
#ifdef DEF_PLATFORM_PREBONDER_SYSTEM
class MTabMounter;
class MHW_Teach;
#endif

/**
 * Vision Calibration Value Define
 */

/** Max. Value */
const	int	DEF_VCALIB_MAX_DOUBLE_MOVE_POSITION	= 18;
const	int	DEF_VCALIB_MAX_CAMERA_FACTOR_NO		= 9;
const	int DEF_VCALIB_USEFUL_FACTORS			= 8; // number of useful calibration parameters

const	int	DEF_VCALIB_NONE						= -1;
const	int	DEF_VCALIB_STAGE					= 1;
const	int	DEF_VCALIB_PCBHANDLER				= 2;
const	int	DEF_VCALIB_PCBLOADER				= 3;

const	int DEF_VCALIB_NO_POSID					= -1;

const	int DEF_MAX_CAMERA_INSTANCE				= 24;
const	int DEF_MAX_UNIT						= 9;
const	int	DEF_DEFAULT_UNIT					= 0;
const	int DEF_VCALIB_MAX_MOVE_POSITION		= 9;

#define SMALLEST_DOUBLE							(1.0e-50)

/** Error Code Define */
const	int	ERR_VCALIB_SUCCESS					= 0;

typedef
/** VisionCalibration Component가 참조하는 Component List */
struct tagSVisionCalibrationRefCompList
{
	/** Vision Component */
	IVision*	m_plnkVision;

} SVisionCalibrationRefCompList;

typedef
/** VisionCalibration Component에 설정해야 될 데이타 */
struct tagSVisionCalibrationData
{
	/** Vision Calibration Data 저장 File Name */
	CString		m_strCalibrationFileName;

	/** Camera 당 Unit 개수 */
	int			m_iUnitNumPerCamera;

} SVisionCalibrationData, *pSVisionCalibrationData;

typedef
/** Calibration Operation Data */
struct tagCalibrationOperationData
{
	/** X 방향 이동 Count - 현재는 고정 사용 (3) */
	int			m_iMatrixRowCount;

	/** Y 방향 이동 Count - 현재는 고정 사용 (3) */
	int			m_iMatrixColCount;

	/** X 방향 이동 거리 */
	double		m_dMoveWidth_X;

	/** Y 방향 이동 거리 */
	double		m_dMoveWidth_Y;

	/** Calibration 완료 Flag*/
	int			m_Complete_Flag;

	/** 이동시킬 Object Type - 0:None, 1:Stage(STU), 2:PCB Handler(PPU), 3:Handler(PLU) */
	int			m_iMoveObjType;

} SCalibrationOperationData, *pSCalibrationOperationData;

/** This class is defined Vision Calibration component implements class as a abstract class.
 * 이 Class는 Vision Calibration Class에 대한 implements Class로 정의한다.
 * 
 * @author Yong-seog Pahn (dragon.pahn@samsung.com )
 * @version $Revision$
 */
class MVisionCalibration : public virtual ICommon
{
/** Attribute */
private:
	CString		m_strFileName;

	/** Camera 당 Unit 개수 */
	int			m_iUnitNumPerCamera;

	IVision*	m_plnkVision;

	/** Camera Calibration Data List */
	SCalibrationOperationData*	m_pData[DEF_MAX_CAMERA_INSTANCE];

	/** Camera Porting Factor */
	double* m_rgdPortingFactor[DEF_MAX_CAMERA_INSTANCE][DEF_MAX_UNIT];

	MHW_Teach	*m_pC_HWTeach;

private :


//	int createBasicMatrix( double* pdA );

	void transposeMatrix(double* pdA, double* pdAt, int iX, int iY);

	int inverseMatrix(double* pdA, int iN, double* pdB);

	void multiplyMatrix(double* pdA, double* pdB, double* pdC, int iX, int iY, int iZ);



/** Constructor, Destructor, 초기화 관련 */
public:

	/**
	 * Constructor
	 * @stereotype constructor 
	 */
	MVisionCalibration(SCommonAttribute	commonData,
						SVisionCalibrationRefCompList listRefComponent,
						SVisionCalibrationData sVisionCalibrationData);

	/**
	 * Destructor
	 * @stereotype destructor 
	 */
	virtual ~MVisionCalibration();

    int Initialize();

/** Interface Operation */
public:

	virtual	int	Calculate2DCalibration(int iCam, int iInstanceNo, MPos_XY* rgxyPixelPos, MPos_XY* rgxyOffsetPos, double *pdFactor);

	void ConvertV2R(int iCamNo, int iUnitNo, double dPixelX, double dPixelY, double &dRobotX, double &dRobotY);
	int ConvertR2V(int iCamNo, int iUnitNo, double dRobotX, double dRobotY, double &dPixelX, double &dPixelY);

	SCalibrationOperationData GetCalibrationData(int iCamNo);

	void SetCalibrationData(int iCamNo, SCalibrationOperationData* pCalibrationData);

	double* GetPortingFactor(int iCamNo, int iUnitNo = DEF_DEFAULT_UNIT);

	void SetPortingFactor(int iCamNo, double* pdportingFactor, int iUnitNo = DEF_DEFAULT_UNIT);

	int CalculatePortingFactorFor9PointPairs(MPos_XY* rgposImagePos, MPos_XY* rgposObjectPos, double *pdFactor);

	/***************** Common Implementation *************************************/

	int AssignComponents(SVisionCalibrationRefCompList listRefComponents);

};

#endif //MVISIONCALIBRATION_H
