/* 
 * VisionCalibration Component
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

/** MVisionCalibrationData.h: interface for the MVisionCalibrationData class.
 */

#ifndef MVISIONCALIBRATIONDATA_H
#define MVISIONCALIBRATIONDATA_H

#include "DefSystem.h"
#include "MVisionCalibration.h"
#include "MFileManager.h"

const int ERR_VISION_CALIBRATION_DATA_SUCCESS				= 0;
const int ERR_VISION_CALIBRATION_DATA_FILE_WRITE_FAILURE	= 1;

class MVisionCalibrationData : public MFileManager 
{
private:
	int		m_iCamera_Calib_Count_X[DEF_MAX_CAMERA_NO];
	int		m_iCamera_Calib_Count_Y[DEF_MAX_CAMERA_NO];
	double	m_dCamera_Move_Width_X[DEF_MAX_CAMERA_NO];
	double	m_dCamera_Move_Width_Y[DEF_MAX_CAMERA_NO];
	double	m_dPortingFactor_Cam[DEF_MAX_CAMERA_NO][DEF_FACTOR_NUMBER];

public:
	/**��ΰ� ���� ������ �����ϴ� ������*/
    MVisionCalibrationData(CString strFileName, CString strPath);
	/**��ΰ� Data ������ �����ϴ� ������*/
	MVisionCalibrationData(CString strFileName);
	MVisionCalibrationData(){};
	/** �Ҹ��� */
    ~MVisionCalibrationData();    

	/** Data�� File���� �о� ���� �Լ� */
	int ReadCalibrationData(int iCamNo, SCalibrationOperationData* pCalibData);
	int ReadPortingFactor(int iCamNo, double* rgdPortingFactor, int iUnitNo);

	/** Data�� File�� ���� ���� �Լ� */
	int WriteCalibrationData(int iCamNo, SCalibrationOperationData* pCalibData);
	int WritePortingFactor(int iCamNo, double* rgdPortingFactor, int iUnitNo);
};
#endif //MVISIONCALIBRATIONDATA_H
