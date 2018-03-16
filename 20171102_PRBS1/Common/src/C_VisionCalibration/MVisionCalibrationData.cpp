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

/** MVisionCalibrationData.cpp: implementation of the MVisionCalibrationData class.
 */

#include "stdafx.h"
#include "MVisionCalibrationData.h"
#include "common.h"

MVisionCalibrationData::MVisionCalibrationData(CString strFileName, CString strPath) : MFileManager(strFileName, strPath)
{
	memset(	m_iCamera_Calib_Count_X, 0, sizeof(m_iCamera_Calib_Count_X));
	memset(	m_iCamera_Calib_Count_Y, 0, sizeof(m_iCamera_Calib_Count_Y));
	memset(	m_dCamera_Move_Width_X, 0, sizeof(m_dCamera_Move_Width_X));
	memset(	m_dCamera_Move_Width_Y, 0, sizeof(m_dCamera_Move_Width_Y));

	for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
	{
		for (int j = 0; j < DEF_FACTOR_NUMBER; j++)
			m_dPortingFactor_Cam[i][j] = 0.0;
	}
}

MVisionCalibrationData::MVisionCalibrationData(CString strFileName) : MFileManager(strFileName)
{
	memset(	m_iCamera_Calib_Count_X, 0, sizeof(m_iCamera_Calib_Count_X));
	memset(	m_iCamera_Calib_Count_Y, 0, sizeof(m_iCamera_Calib_Count_Y));
	memset(	m_dCamera_Move_Width_X, 0, sizeof(m_dCamera_Move_Width_X));
	memset(	m_dCamera_Move_Width_Y, 0, sizeof(m_dCamera_Move_Width_Y));

	for (int i = 0; i < DEF_MAX_CAMERA_NO; i++)
	{
		for (int j = 0; j < DEF_FACTOR_NUMBER; j++)
			m_dPortingFactor_Cam[i][j] = 0.0;
	}
}

MVisionCalibrationData::~MVisionCalibrationData()
{
	TRACE(_T("MVisionCalibrationData Class Destruction is Completed.\n"));
}

/**
 * Calibration Data를 읽는다. 
 * 
 * @param	iCamNo	: Camera 번호 
 * @param	pCalibData	: Calibration Data
 * @return	Error : 0=SUCCESS, 그외 = Error Code
 */
int MVisionCalibrationData::ReadCalibrationData(int iCamNo, SCalibrationOperationData* pCalibData)
{
	WCHAR eMsg[200];
	WCHAR eMsg1[200];
	CString strSection;
	CString strItem;
	BOOL ret = TRUE;
	
	memset(eMsg, 0, sizeof(eMsg));
	memset(eMsg1, 0, sizeof(eMsg1));
	
	strSection.Format(_T("Camera #%d Calibration Data"), iCamNo + 1);
	
	if (!GetValue(strSection, "Camera_Calib_Count_X", &(pCalibData->m_iMatrixRowCount)))
	{
		/** 첫번째 Data 를 읽지 못하면 이 Camera 는 사용하지 않는 것으로 인식한다. */
		return FALSE;
	}

	if (!GetValue(strSection, "Camera_Calib_Count_Y", &(pCalibData->m_iMatrixColCount)))
	{
		swprintf(eMsg, _T("[%s] Camera_Calib_Count_Y 항목을 읽지 못했습니다. 다음 항목을 계속 읽겠습니까?"), strSection.GetBuffer());
		swprintf(eMsg1, _T("%s File Read Error!"), m_strFileName.GetBuffer());
		// MyMessageBox 오버로딩에 맞게 변경 요망
		// 이기호
//		if (MyMessageBox(eMsg, eMsg1, M_ICONERROR|M_YESNO) == IDNO) return FALSE;
		ret = FALSE;
	}

	if (!GetValue(strSection, "Camera_Move_Width_X", &(pCalibData->m_dMoveWidth_X)))
	{
		swprintf(eMsg, _T("[%s] Camera_Move_Width_X 항목을 읽지 못했습니다. 다음 항목을 계속 읽겠습니까?"), strSection.GetBuffer());
		swprintf(eMsg1, _T("%s File Read Error!"), m_strFileName.GetBuffer());
		// MyMessageBox 오버로딩에 맞게 변경 요망
		// 이기호
//		if (MyMessageBox(eMsg, eMsg1, M_ICONERROR|M_YESNO) == IDNO) return FALSE;
		ret = FALSE;
	}

	if (!GetValue(strSection, "Camera_Move_Width_Y", &(pCalibData->m_dMoveWidth_Y)))
	{
		swprintf(eMsg, _T("[%s] Camera_Move_Width_Y 항목을 읽지 못했습니다. 다음 항목을 계속 읽겠습니까?"), strSection.GetBuffer());
		swprintf(eMsg1, _T("%s File Read Error!"), m_strFileName.GetBuffer());
		// MyMessageBox 오버로딩에 맞게 변경 요망
		// 이기호
//		if (MyMessageBox(eMsg, eMsg1, M_ICONERROR|M_YESNO) == IDNO) return FALSE;
		ret = FALSE;
	}

	return ret;
}

/**
 * Porting Factor Data를 읽는다. 
 * 
 * @param	iCamNo	: Camera 번호 
 * @param	rgdPortingFactor	: Porting Factor Data
 * @param	iUnit	: Unit 번호 
 * @return	Error : 0=SUCCESS, 그외 = Error Code
 */
int MVisionCalibrationData::ReadPortingFactor(int iCamNo, double* rgdPortingFactor, int iUnit)
{
	CString strSection;
	CString strItem;
	BOOL bReturn = TRUE;
	
	strSection.Format(_T("Camera #%d Calibration Data"), iCamNo + 1);

	for (int j = 0; j < DEF_FACTOR_NUMBER ; j++)
	{
		strItem.Format(_T("Porting_Factor[Unit%d]_%d"), iUnit + 1, j + 1);
		if (!GetValue(strSection, strItem, &(rgdPortingFactor[j])))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
	}
	
	return bReturn;
}

/**
 * Calibration Data를 쓴다.. 
 * 
 * @param	iCamNo	: Camera 번호 
 * @param	pCalibData	: Calibration Data
 * @return	Error : 0=SUCCESS, 그외 = Error Code
 */
int MVisionCalibrationData::WriteCalibrationData(int iCamNo, SCalibrationOperationData* pCalibData)
{
	if (!MakeBackUpFile())
	{
		//CString strMsg = ML_MSG("Back Up File을 만드는데 실패 했습니다. 계속 진행 하겠습니까?",
		//	"It was failed to make backup file. Go ahead?");
		//if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "File Write Error", M_ICONERROR|M_YESNO) == IDNO) 
		if (MyMessageBox(MY_MESSAGE_BOX,62000, _T("File Write Error"), M_ICONERROR|M_YESNO) == IDNO) 
			return FALSE;
	}

	CString strSection;
	CString strItem;
	BOOL ret = TRUE;

	strSection.Format(_T("Camera #%d Calibration Data"), iCamNo+1);
	
	if (!SetValue(strSection, "Camera_Calib_Count_X", pCalibData->m_iMatrixRowCount))
		ret = FALSE;

	if (!SetValue(strSection, "Camera_Calib_Count_Y", pCalibData->m_iMatrixColCount))
		ret = FALSE;

	if (!SetValue(strSection, "Camera_Move_Width_X", pCalibData->m_dMoveWidth_X))
		ret = FALSE;

	if (!SetValue(strSection, "Camera_Move_Width_Y", pCalibData->m_dMoveWidth_Y))
		ret = FALSE;
	
	if (!ret)
	{
		WCHAR eMsg[200];
		WCHAR eMsg1[200];
		memset(eMsg, 0, sizeof(eMsg));
		memset(eMsg1, 0, sizeof(eMsg1));
		
		swprintf(eMsg, _T("%s File is an incorrect."), m_strFileName.GetBuffer());
		swprintf(eMsg1, _T("%s File Write Error!"), m_strFileName.GetBuffer());
		// MyMessageBox 오버로딩에 맞게 변경 요망
		// 이기호
//		MyMessageBox(eMsg, eMsg1, M_ICONERROR);
		return ERR_VISION_CALIBRATION_DATA_FILE_WRITE_FAILURE;
	}

	return ERR_VISION_CALIBRATION_DATA_SUCCESS;
}

/**
 * Porting Factor Data를 쓴다.. 
 * 
 * @param	iCamNo	: Camera 번호 
 * @param	rgdPortingFactor	: Porting Factor Data
 * @param	iUnit	: Unit 번호 
 * @return	Error : 0=SUCCESS, 그외 = Error Code
 */
int MVisionCalibrationData::WritePortingFactor(int iCamNo, double* rgdPortingFactor, int iUnit)
{
	if (!MakeBackUpFile())
	{
		//CString strMsg = ML_MSG("Back Up File을 만드는데 실패 했습니다. 계속 진행 하겠습니까?",
		//	"It was failed to make backup file. Go ahead?");
		//if (MyMessageBox(strMsg.GetBuffer(strMsg.GetLength()), "File Write Error", M_ICONERROR|M_YESNO) == IDNO) 
		if (MyMessageBox(MY_MESSAGE_BOX,62000, _T("File Write Error"), M_ICONERROR|M_YESNO) == IDNO) 
			return FALSE;
	}

	CString strSection;
	CString strItem;
	BOOL ret = TRUE;

	strSection.Format(_T("Camera #%d Calibration Data"), iCamNo + 1);
	
	for (int j = 0; j < DEF_FACTOR_NUMBER ; j++)
	{
		strItem.Format(_T("Porting_Factor[Unit%d]_%d"), iUnit + 1, j + 1);
		if (!SetValue(strSection, strItem, rgdPortingFactor[j]))
			ret = FALSE;
	}

	if (!ret)
	{
		WCHAR eMsg[200];
		WCHAR eMsg1[200];
		memset(eMsg, 0, sizeof(eMsg));
		memset(eMsg1, 0, sizeof(eMsg1));
		
		swprintf(eMsg, _T("%s File is an incorrect."), m_strFileName.GetBuffer());
		swprintf(eMsg1, _T("%s File Write Error!"), m_strFileName.GetBuffer());
		// MyMessageBox 오버로딩에 맞게 변경 요망
		// 이기호
//		MyMessageBox(eMsg, eMsg1, M_ICONERROR);
		return ERR_VISION_CALIBRATION_DATA_FILE_WRITE_FAILURE;
	}

	return ERR_VISION_CALIBRATION_DATA_SUCCESS;
}
