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
 * MVisionCalibration.cpp : This file implements the class of vision calibration.
 *					이 파일은 Vision Calibration Class의 실행 코드를 구현한다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#include "stdAfx.h"
#include "MyProgressWnd.h"
#include "IVision.h"
#include "MVisionCalibration.h"
#include "MVisionCalibrationData.h"
#include <math.h>
#include "MPos_XY.h"

#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/** Standard 생성자
  */
MVisionCalibration::MVisionCalibration(SCommonAttribute	commonData,
									   SVisionCalibrationRefCompList listRefComponent,
									   SVisionCalibrationData sVisionCalibrationData)
{
	// memory allocation for SDrawerRefCompList and assignment
	m_strFileName = sVisionCalibrationData.m_strCalibrationFileName;
	m_iUnitNumPerCamera = sVisionCalibrationData.m_iUnitNumPerCamera;

	AssignComponents(listRefComponent);
	
	MVisionCalibrationData calibrationData(m_strFileName);		// "Vision.dat"

	SCalibrationOperationData tmpCalibOperationData;

	int iUnit;
	for (int iCam=0; iCam < DEF_MAX_CAMERA_INSTANCE ; iCam++)
	{
		m_pData[iCam] = new SCalibrationOperationData;
		m_pData[iCam]->m_iMatrixRowCount	= 0;
		m_pData[iCam]->m_iMatrixColCount	= 0;
		m_pData[iCam]->m_dMoveWidth_X		= 0.0;
		m_pData[iCam]->m_dMoveWidth_Y		= 0.0;
		m_pData[iCam]->m_Complete_Flag		= 0;
		m_pData[iCam]->m_iMoveObjType		= 0;

		for (int i = 0; i < DEF_MAX_UNIT; i++)
			m_rgdPortingFactor[iCam][i] = NULL;

		for (iUnit = 0; iUnit < m_iUnitNumPerCamera; iUnit++)
			m_rgdPortingFactor[iCam][iUnit] = new double[DEF_FACTOR_NUMBER];

		int iResult = calibrationData.ReadCalibrationData(iCam, &tmpCalibOperationData);
		if (iResult == TRUE)
		{
			ASSERT(m_pData[iCam] != NULL);

			m_pData[iCam]->m_iMatrixRowCount	= tmpCalibOperationData.m_iMatrixRowCount;
			m_pData[iCam]->m_iMatrixColCount	= tmpCalibOperationData.m_iMatrixColCount;
			m_pData[iCam]->m_dMoveWidth_X		= tmpCalibOperationData.m_dMoveWidth_X;
			m_pData[iCam]->m_dMoveWidth_Y		= tmpCalibOperationData.m_dMoveWidth_Y;
			m_pData[iCam]->m_Complete_Flag		= tmpCalibOperationData.m_Complete_Flag;
			m_pData[iCam]->m_iMoveObjType		= tmpCalibOperationData.m_iMoveObjType;

			for (iUnit = 0; iUnit < m_iUnitNumPerCamera; iUnit++)
			{
				ASSERT(m_rgdPortingFactor[iCam] != NULL);

				calibrationData.ReadPortingFactor(iCam, m_rgdPortingFactor[iCam][iUnit], iUnit);
			}
		}
	}
}

/**
 * 소멸자
 * @stereotype destructor 
 */
MVisionCalibration::~MVisionCalibration()
{
	for (int i = 0; i < DEF_MAX_CAMERA_INSTANCE; i++)
	{
		if (m_pData[i] != NULL)
			delete m_pData[i];
		
		for (int j = 0; j < DEF_MAX_UNIT; j++)
		{
			if (m_rgdPortingFactor[i][j])
				delete []m_rgdPortingFactor[i][j];
		}
	}
}

/**
 * Vision Calibration 설정을 한다.
 *
 * @return	Error : 0 = Success, 그외=기타
 */
int MVisionCalibration::Initialize()
{
	int iResult = ERR_VCALIB_SUCCESS;

	return iResult;
}

/**
 *	해당 Camera 와 Unit 에 대한 Vision Calibration Operation Data 를 얻는다.
 * @param	iCamNo :Camera Number
 * @return  m_pData : Camera Number에 해당하는 Camera Data
 */
SCalibrationOperationData MVisionCalibration::GetCalibrationData(int iCamNo)
{
	return *m_pData[iCamNo];
}

/**
 * 해당 Camera 와 Unit 에 대한 Vision Calibration Operation Data 값을
 * Component 의 Member 에 설정하고, File 로 저장한다.
 *
 * @param	iCamNo :Camera Number
 * @param	pCalibrationData : 해당 Camera의 Calibration Data
 * 
 */
void MVisionCalibration::SetCalibrationData(int iCamNo, SCalibrationOperationData* pCalibrationData)
{
	m_pData[iCamNo]->m_iMatrixRowCount		= pCalibrationData->m_iMatrixRowCount;
	m_pData[iCamNo]->m_iMatrixColCount		= pCalibrationData->m_iMatrixColCount;
	m_pData[iCamNo]->m_dMoveWidth_X			= pCalibrationData->m_dMoveWidth_X;
	m_pData[iCamNo]->m_dMoveWidth_Y			= pCalibrationData->m_dMoveWidth_Y;
	m_pData[iCamNo]->m_Complete_Flag		= pCalibrationData->m_Complete_Flag;
	m_pData[iCamNo]->m_iMoveObjType			= pCalibrationData->m_iMoveObjType;

	MVisionCalibrationData calibrationData(m_strFileName);		// "Vision.dat"
	calibrationData.WriteCalibrationData(iCamNo, pCalibrationData);
}

/**
 * 해당 Camera 와 Unit 에 대한 Vision Calibration Porting Factor 를 얻어온다.
 *
 * @param	iCamNo :Camera Number
 * @param	iUnitNo : Unit Number
 * @return	m_rgdPortingFactor : 해당 Unit의 Camera에 관련된 PortingFactor Data
 */
double* MVisionCalibration::GetPortingFactor(int iCamNo, int iUnitNo)
{
	return m_rgdPortingFactor[iCamNo][iUnitNo];
}

/**
 * 해당 Camera 와 Unit 에 대한 Vision Calibration Porting Factor 값을
 * Component 의 Member 에 설정하고, File 로 저장한다.
 *
 * @param	iCamNo :Camera Number
 * @param	pdportingFactor : Vision Calibration Porting Factor 값
 * @param	iUnitNo : Unit Number
 */
void MVisionCalibration::SetPortingFactor(int iCamNo, double* pdportingFactor, int iUnitNo)
{
	for (int i = 0; i < DEF_FACTOR_NUMBER; i++)
		m_rgdPortingFactor[iCamNo][iUnitNo][i] = pdportingFactor[i];

	MVisionCalibrationData calibrationData(m_strFileName);		// "Vision.dat"
	calibrationData.WritePortingFactor(iCamNo, pdportingFactor, iUnitNo);
}

/***************** Component Operation *************************************/

/**
 *	Vision 좌표를 Robot 좌표로 Convert 한다.
 *
 * @param	iCamNo :Camera Number
 * @param	iUnitNo : Unit Number
 * @param	pdportingFactor : Vision Calibration Porting Factor 값
 * @param	dPixelX  : Pixel X 값
 * @param	dPixelY  : Pixel Y 값
 * @param	dRobotX  : Robot 좌표  X 값
 * @param	dRobotY  : Robot 좌표  Y 값
 */
void MVisionCalibration::ConvertV2R(int iCamNo, int iUnitNo, double dPixelX, double dPixelY,
									double &dRobotX, double &dRobotY)
{
	dRobotX	=	  dPixelX *	m_rgdPortingFactor[iCamNo][iUnitNo][0]
				+ dPixelY *	m_rgdPortingFactor[iCamNo][iUnitNo][1]
				+			m_rgdPortingFactor[iCamNo][iUnitNo][2];

	dRobotY	=	  dPixelX *	m_rgdPortingFactor[iCamNo][iUnitNo][3]
				+ dPixelY *	m_rgdPortingFactor[iCamNo][iUnitNo][4]
				+			m_rgdPortingFactor[iCamNo][iUnitNo][5];

	double t =	  dPixelX *	m_rgdPortingFactor[iCamNo][iUnitNo][6]
				+ dPixelY *	m_rgdPortingFactor[iCamNo][iUnitNo][7]
				+ 1.0;

	dRobotX /= t;
	dRobotY /= t;
}

int MVisionCalibration::ConvertR2V(int iCamNo, int iUnitNo, double dRobotX, double dRobotY,
									double &dPixelX, double &dPixelY)
{
	double t;
	double s[DEF_FACTOR_NUMBER];
	double r[DEF_FACTOR_NUMBER];

	for (int i = 0; i < DEF_FACTOR_NUMBER; i++)
		s[i] = m_rgdPortingFactor[iCamNo][iUnitNo][i];

	int iResult = inverseMatrix(s, 3, r);
	if (iResult)
		return iResult;

	t		= dRobotX * r[6] + dRobotY * r[7] + 1.0;
	dPixelX	= (dRobotX * r[0] + dRobotY * r[1] + r[2]) * t;
	dPixelY	= (dRobotX * r[3] + dRobotY * r[4] + r[5]) * t;

	return ERR_VCALIB_SUCCESS;
}

/**
 * 2D Calibration을 수행한다.
 * 2D Calibration을 수행하기 위해서는 Vision화면이 Display 되어야 하며
 * 해당 Stage의 조그가 Enable 되어 있어야 한다.
 * @param	iUnitID : Calibration 을 수행할 Stage 번호
 * @param	iCamera : Unit의 Camera Index (Vision의 Camera ID가 아님)
 * @param	iMark : 인식할 Mark Pattern 전호
 * @param	pdFactor :Factor Value;
 * @param	iCalibType : Calibration이 수행 종류  (DEF_CALIB_MOVE_9_POINT, DEF_CALIB_MOVE_3_POINT)
 * @return	Error : 0=SUCCESS, 그외=Error Code
 */
int MVisionCalibration::Calculate2DCalibration(int iCam, int iInstanceNo, MPos_XY* rgxyPixelPos, MPos_XY* rgxyOffsetPos, double *pdFactor)
{
	int iError;

	iError = CalculatePortingFactorFor9PointPairs(rgxyPixelPos, rgxyOffsetPos, pdFactor);
	if (iError != ERR_VCALIB_SUCCESS)
		return iError;

	/** Vision Compponent에 Porting Factor를 저장한다. */
//	SetPortingFactor(iCam, pdFactor, 0 /*m_plnkStage[iUnitID]->GetInstanceNo()*/);
	SetPortingFactor(iCam, pdFactor, iInstanceNo);
		
	return ERR_VCALIB_SUCCESS;
}

/** Basic Estimation Matrix 
 * @param	pdA : Source 행렬
 * @return	Error : 0=SUCCESS, 그외=Error Code
 */
/*
int MVisionCalibration::createBasicMatrix(double *pdA)
{
	for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
	{
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  0] = m_rgxyPixelPos[i].dX;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  1] = m_rgxyPixelPos[i].dY;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  2] = 1.0;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  3] = 0.0;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  4] = 0.0;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  5] = 0.0;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  6] = -m_rgxyPixelPos[i].dX * m_rgxyOffsetPos[i].dX;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  7] = -m_rgxyPixelPos[i].dY * m_rgxyOffsetPos[i].dX;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  8] = 0.0;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  9] = 0.0;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 10] = 0.0;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 11] = m_rgxyPixelPos[i].dX;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 12] = m_rgxyPixelPos[i].dY;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 13] = 1.0;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 14] = -m_rgxyPixelPos[i].dX * m_rgxyOffsetPos[i].dY;
		pdA[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 15] = -m_rgxyPixelPos[i].dY * m_rgxyOffsetPos[i].dY;
	}
	return ERR_VCALIB_SUCCESS;
}
*/

/*
 * 전치행렬 계산하기
 *
 *		1 2 3		1 4 7
 *		4 5 6	->	2 5 8
 *		7 8 9		3 6 9
 *
 * @param	pdA : Source 행렬
 * @param	pdAt : Result 행렬
 * @param	iX : Source 행렬 (X x Y) 중 X
 * @param	iY : Source 행렬 (X x Y) 중 Y
 */
void MVisionCalibration::transposeMatrix(double* pdA, double* pdAt, int iX, int iY)
{
	for (int i = 0; i < iX; i++)
		for (int j = 0; j < iY; j++)
			*(pdAt + (iX * j) + i) = (*(pdA + (iY * i) + j));
} 

/*
 * 역행렬 계산하기
 *
 * @param	pdA : Source 행렬
 * @param	iN : Source 행렬 (N x N) 중 N
 * @param	pdB : Result 행렬
 * @return	Error : 0=SUCCESS, 그외 = Error Code
 */
int MVisionCalibration::inverseMatrix(double* pdA, int iN, double* pdB)
{
	int i, j, k, l, l1;
	double dTemp1;

	for ( i = 0; i < iN; i++) 
	{
		for (j = 0; j < iN; j++)
			 *(pdB + j + (i * iN)) = 0.;
		*(pdB + i + (i * iN)) = 1.0;
	}

	for (l = 0; l < iN; l++)
	{
		if (fabs(*(pdA + l + (l * iN))) < SMALLEST_DOUBLE)
		{
			l1 = l;

			while ((fabs(*(pdA + l + (l1 * iN))) < SMALLEST_DOUBLE) && (++l1 < iN))
				; /** NULL */

			if (l1 >= iN)
				return generateErrorCode(302001);	// ERR_VCALIB_NOT_EXIST_INVERSE_MATRIX
			
			for (j=0; j<iN; j++)
			{
				*(pdA + j + (l*iN)) += (*(pdA + j + (l1 * iN)));
				*(pdB + j + (l*iN)) += (*(pdB + j + (l1 * iN)));
			}
		}

		dTemp1 = 1.0 / (*(pdA + l + (l * iN)));

		for (j = l; j < iN; j++)
			*(pdA + j + (l * iN)) *= dTemp1;

		for (j = 0; j < iN; j++)
			*(pdB + j + (l * iN)) *= dTemp1;

		k = l + 1;

		for (i = k; i < iN; i++)
		{
			dTemp1 = *(pdA + l + (i * iN));

			for (j = l; j < iN; j++)
				*(pdA + j + (i * iN)) -= (*(pdA + j + (l * iN)) * dTemp1);

			for (j = 0; j < iN; j++)
				*(pdB + j + (i * iN)) -= (*(pdB + j + (l * iN)) * dTemp1);
		}
	}
	for (l = iN - 1; l >= 0; l--)
	{
		for (k = 1; k <= l; k++)
		{
			i = l - k;

			dTemp1 = *(pdA + l + (i * iN));

			for (j = l; j < iN; j++)
				*(pdA + j + (i * iN)) -= (*(pdA + j + (l * iN)) * dTemp1);

			for (j = 0; j < iN; j++)
				*(pdB + j + (i * iN)) -= (*(pdB + j + (l * iN)) * dTemp1);
		}
	}
	return ERR_VCALIB_SUCCESS;
} 

/*
 * 행렬 곱 계산하기
 *
 * @param	pdA : Source 행렬 1
 * @param	pdB : Source 행렬 2
 * @param	pdC : Reuslt 행렬
 * @param	iX : Source 행렬 1의 (A x B) 중 A
 * @param	iY : Source 행렬 2의 (B x C) 중 B
 * @param	iZ :Result 행렬 (A x C) 중 C
 */
void MVisionCalibration::multiplyMatrix(double* pdA, double* pdB, double* pdC, int iX, int iY, int iZ)
{
	double dTemp;

	for (int i = 0; i < iX; i++)
	{
		for (int j = 0; j < iZ; j++)
		{
			dTemp = 0.0;

			for (int k = 0; k < iY; k++)				// 곱 계산
				dTemp += (*(pdA + (i * iY) + k) * (*(pdB + (k * iZ) + j)));

			*(pdC + (i * iZ)+j) = dTemp;				// 결과 행렬에 기록
		}
	}
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 9 Point Matching Calibration Calculating Function
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
int MVisionCalibration::CalculatePortingFactorFor9PointPairs(MPos_XY* rgposImagePos,
															   MPos_XY* rgposObjectPos,
															   double *pdFactor)
{
	double  t[DEF_VCALIB_MAX_DOUBLE_MOVE_POSITION * DEF_VCALIB_USEFUL_FACTORS];		// 18 X 8 : 기본 행렬
	double	tt[DEF_VCALIB_USEFUL_FACTORS * DEF_VCALIB_MAX_DOUBLE_MOVE_POSITION];	// 8 X 18 : 전치 행렬
	double	m1[DEF_VCALIB_USEFUL_FACTORS * DEF_VCALIB_USEFUL_FACTORS];				// 8 X 8  : [tt X t] 행렬곱
	double  r[DEF_VCALIB_USEFUL_FACTORS * DEF_VCALIB_USEFUL_FACTORS];				// 8 X 8  : [tt X t]의 역행렬
	double	m2[DEF_VCALIB_USEFUL_FACTORS * 1];										// 8 x 1 : [tt X p] 행렬곱
	double  p[DEF_VCALIB_MAX_DOUBLE_MOVE_POSITION];									// 18 X 1 : Pixel Data 행렬
	double  f[DEF_VCALIB_USEFUL_FACTORS + 1];										// 8 X 1  : Camera Factor 행렬

	/** Let the Basic Matrix be t(18 x 8)  */
	for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
	{
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  0] = rgposImagePos[i].dX;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  1] = rgposImagePos[i].dY;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  2] = 1.0;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  3] = 0.0;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  4] = 0.0;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  5] = 0.0;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  6] = -rgposObjectPos[i].dX * rgposImagePos[i].dX;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  7] = -rgposObjectPos[i].dX * rgposImagePos[i].dY;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  8] = 0.0;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS +  9] = 0.0;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 10] = 0.0;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 11] = rgposImagePos[i].dX;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 12] = rgposImagePos[i].dY;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 13] = 1.0;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 14] = -rgposObjectPos[i].dY * rgposImagePos[i].dX;
		t[i * 2 * DEF_VCALIB_USEFUL_FACTORS + 15] = -rgposObjectPos[i].dY * rgposImagePos[i].dY;
	}

	/** input t -> output tt(transpose matrix of t, 8 x 18)  */
	transposeMatrix(t, tt, DEF_VCALIB_MAX_DOUBLE_MOVE_POSITION, DEF_VCALIB_USEFUL_FACTORS);

	/** matrix multiplication : m1 = tt x t ([8x18] x [18x8] = [8x8]) */
	multiplyMatrix(tt, 
		           t, 
				   m1, 
				   DEF_VCALIB_USEFUL_FACTORS, 
				   DEF_VCALIB_MAX_DOUBLE_MOVE_POSITION, 
				   DEF_VCALIB_USEFUL_FACTORS);

	/** inverse matrix manipulation : r = m-1 ([8x8]) */
	int iError = inverseMatrix(m1, DEF_VCALIB_USEFUL_FACTORS, r);
	if (iError != ERR_VCALIB_SUCCESS)
		return iError;

	for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION; i++)
	{
		p[2 * i] = rgposObjectPos[i].dX;
		p[2 * i + 1] = rgposObjectPos[i].dY;
	}

	/** m2 = tt x p  ([8x18] x [18x1] = [8x1]) */
	multiplyMatrix(tt, p, m2, DEF_VCALIB_USEFUL_FACTORS, DEF_VCALIB_MAX_DOUBLE_MOVE_POSITION, 1);

/*	for (int i=0; i<DEF_VCALIB_MAX_MOVE_POSITION; i++)
	{
		p[2 * i] = m_rgxyPixelPos[i].dX;
		p[2 * i + 1] = m_rgxyPixelPos[i].dY;
	}
*/
	/** Finally we caluculate porting factor f = r x m2 = inverse(t x tt) x (tt x robot position) */
	multiplyMatrix(r, m2, f, DEF_VCALIB_USEFUL_FACTORS, DEF_VCALIB_USEFUL_FACTORS, 1);

	for (int i = 0; i < DEF_VCALIB_MAX_MOVE_POSITION - 1; i++)
		pdFactor[i] = f[i];
	
	/** 마지막 Factor값은 항상 1.0으로 한다. */
	pdFactor[DEF_VCALIB_MAX_MOVE_POSITION-1] = 1.0;
		
	return ERR_VCALIB_SUCCESS;
}

/***************** Common Implementation *************************************/

/**
 * Assign component linkage
 * 
 * @param	listRefComponents	: MVisionCalibration 참조 Component list
 * @return	Error : 0=SUCCESS, 그외 = Error Code
 */
int MVisionCalibration::AssignComponents(SVisionCalibrationRefCompList listRefComponent)
{
	m_plnkVision = listRefComponent.m_plnkVision;
	return ERR_VCALIB_SUCCESS;
}
/* End Of Code */
