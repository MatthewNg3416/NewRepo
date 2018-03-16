/* 
 * HW_Teaching Component
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
 * MHW_Teach.cpp : This file implements the class of MHW_Teach Component
 *					이 파일은 Vision Calibration Class의 실행 코드를 구현한다.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#include "stdAfx.h"
#include "MHW_Teach.h"
#include "MPos_XYT.h"
#include "Common.h"		// MyMessageBox를 사용하기 위함
#include "IVision.h"
#include "MPanelAligner.h"
#include "MPanelData.h"
#include <math.h>
#include "MVisionCalibration.h"

#include "MPlatformOlbSystem.h"
#include "MTabCarrier.h"
#include "MPreBonderData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern MPlatformOlbSystem MOlbSystemPre;

/**
 * Default 생성자
 * @stereotype constructor
 */

/** Standard 생성자
  */
MHW_Teach::MHW_Teach(	SCommonAttribute		commonData,
						SHW_TeachRefCompList	listRefComponents, 
						SHW_TeachData			datComponent
					)
: ICommon(commonData)
{
	CString strLogMsg;

	AssignComponents(listRefComponents);

	SetData(datComponent);

	strLogMsg.Format(_T("MHW_Teach() : ObjectID=%d OK"), m_iObjectID);
	m_plogMng->WriteLog(DEF_MLOG_NORMAL_LOG_LEVEL, strLogMsg, __FILE__, __LINE__);
}

/**
 * 소멸자
 * @stereotype destructor 
 */
MHW_Teach::~MHW_Teach()
{
}

/**
 * MHW_Teach 설정을 한다.
 *
 * @return	Error : 0 = Success, 그외=기타
 */
int MHW_Teach::Initialize()
{
	int iResult = ERR_HW_TEACH_SUCCESS;
	return iResult;
}

/** Rotate an old point to a new point
  * @param dAngle : 각도
  * @param dOldX  : X of Point 1
  * @param dOldY  : Y of Point 1
  * @param dNewX  : X of Point 2
  * @param dNewY  : Y of Point 2 
  */
void MHW_Teach::Rotation(double dAngle, double dOldX, double dOldY, double* dNewX, double* dNewY)
{
	*dNewX = cos(DEG2RAD(dAngle)) * dOldX - sin(DEG2RAD(dAngle)) * dOldY;
	*dNewY = sin(DEG2RAD(dAngle)) * dOldX + cos(DEG2RAD(dAngle)) * dOldY;
}

/** Find two centers. One is in the positive range w.r.t. the line between the first position and the second position, 
    and it is an origin of a circle that passes on the first position and the sencond position with radius r.
    The other is in the negative range w.r.t. the line between the first position and the second position, 
    and it is also an origin of a circle that passes on the first position and the sencond position with radius r.
  * @param dTAngle : 각도
  * @param dRadius : 반지름
  * @param possFirst   : First Position
  * @param posSecond  : Second Position
  * @param posCenter  : the center is in the positive range w.r.t. the line passing on the First Position and the Second Position
  */
void MHW_Teach::FindCenter(double dTAngle, 
						   double dRadius,
						   MPos_XYT posFirst,
						   MPos_XYT posSecond,
						   MPos_XYT* posCenter)
{
	MPos_XYT p1 = posFirst;        
	MPos_XYT p2 = posSecond;
	double t = DEG2RAD(dTAngle);

	posCenter->dX = ((-p2.dX + cos(t)*p1.dX - sin(t)*p1.dY)/(cos(t) - 1.0) + sin(t)/(cos(t) -1.0)*(-p2.dY + sin(t)*p1.dX + cos(t)*p1.dY)/(cos(t) - 1.0))/(1+sin(t)*sin(t)/(cos(t)-1.0)/(cos(t)-1.0));
	posCenter->dY = ((-p2.dY + sin(t)*p1.dX + cos(t)*p1.dY)/(cos(t) - 1.0) - sin(t)/(cos(t) -1.0)*(-p2.dX + cos(t)*p1.dX - sin(t)*p1.dY)/(cos(t) - 1.0))/(1+sin(t)*sin(t)/(cos(t)-1.0)/(cos(t)-1.0));
}

/** Camera 기준좌표로 고정좌표 찾기
 * 기 등록된 Mark를 인식하여 위치를 알아낸다. 인식할 Mark는 Xu변 또는 Xd변의 좌상단 Mark임.
 * @preconditions ExecuteCalibration()을 수행하여 CamFactor가 계산되어 있어야 한다.
 * @preconditions SetCellData()를 이용해서 Cell정보가 입력되어 있어야 한다.
 *
 * @param iStageID : 카메라 고정좌표 찾기에 사용할 Stage ID (Stage1:DEF_STAGE_1, Stage2:DEF_STAGE_2)
 * @param iCamera : 카메라 번호
 * @param iMark   : 마크 번호
 * @param pX : X 좌표
 * @param pY : Y 좌표  
 * @return	Error : 0 = Success, 그외=기타
 */
int MHW_Teach::GetCameraFixedPosition(int iStageID, 
									  int iCamNo,
									  int iMarkNo,
									  double dTAngle,
									  double* pX,
									  double* pY)
{
#ifndef DEF_PLATFORM_PREBONDER_SYSTEM

	double		dDistanceXY;
	double		dDistanceX;
	double		dDistanceY;
	double		dRadius;
	
	CString		tmpDisplay;			// Display를 위한 Temp String

	MPos_XYT	posFirstMark;		// 좌표를 저장한다.
	MPos_XYT	posSecondMark;		// 좌표를 저장한다.
	MPos_XYT	posFirst;			// 초기 좌표를 저장한다.
	MPos_XYT	posSecond;			// TAngle도 회전한후의 좌표를 저장한다.
	MPos_XYT	posMove;			// 이동할 좌표 저장
	MPos_XYT	posCenter;			// Cneter
	MPos_XYT	posTarget;			// 

	int			iResult;
	double		dPixelX;
	double		dPixelY;

	posFirst = m_plnkPanelAligner[iStageID]->GetXYTCurrentPos();
	
	/**  1번째 Mark를 인식한다.-------------------------------*/
	m_plnkVision->ClearOverlay(iCamNo);
	
	iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 1번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkVisionCalibration->ConvertV2R(iCamNo, 
											m_plnkPanelAligner[iStageID]->GetInstanceNo(),
											dPixelX, dPixelY,
											posFirstMark.dX, posFirstMark.dY);
	}
	else
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	posMove.dX =  posFirst.dX;
	posMove.dY =  posFirst.dY;
	posMove.dT =  dTAngle + posFirst.dT;

	//iResult = m_plnkStage[iStageID]->SafeMoveXYTPos(DEF_STAGE_TURN_POS);
	//if (iResult) return iResult;
	
	// Stage 축을 dTurnAgle 만큼 회전하고, XY를 움직여 Mark를 움직여 화면내에 들어오게 한다.
	dDistanceX = (m_plnkPanelData->m_dPanelSize_X / 2 - m_plnkPanelData->m_dX_FiduMark_Xu);
	dDistanceY = (m_plnkPanelData->m_dPanelSize_Y / 2 - m_plnkPanelData->m_dY_FiduMark_Xu);

	Rotation(dTAngle, dDistanceX, dDistanceY, &posMove.dX, &posMove.dY);
	
	if (dTAngle >= 0.0  &&  dTAngle <= 90.0)
	{
		posMove.dX = dDistanceX - posMove.dX;
		posMove.dY = dDistanceY - posMove.dY;
	}
	else if (dTAngle < 0.0  &&  dTAngle >= -90.0)
	{
		posMove.dX = -dDistanceX - posMove.dX;
		posMove.dY = dDistanceY + posMove.dY;
	}
	else
	{
		// 301002 = Camera 고정좌표 찾기 회전 각도가 -90 ~ 90 도 범위를 벗어납니다.
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return generateErrorCode(301002);
	}

	posMove.dX = posFirst.dX + posMove.dX;
	posMove.dY = posFirst.dY + posMove.dY;

	iResult = m_plnkPanelAligner[iStageID]->MoveXYTPos(posMove, DEF_PANEL_ALIGNER_NONE_POS);
	if (iResult)
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}
	
	/** Display Vision */
	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iCamNo);

	//if (MyMessageBox("Jog를 이용하여 Mark가 Vision 화면에 들어오게 하고 OK 버튼을 눌러 주세요.",
	//	"확인",M_ICONQUESTION |M_YESNO,"OK","취소") == IDNO)
	if (MyMessageBox(MY_MESSAGE_BOX,62401,_T("Confirmation"),M_ICONQUESTION |M_YESNO,_T("OK"),_T("Cancel")) == IDNO)
	{
		//WriteErrorLog("GetCameraFixedPosition() : User Cancel Error");
		SetErrorLevel(_T("GetCameraFixedPosition() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL;
	}

	posSecond = m_plnkPanelAligner[iStageID]->GetXYTCurrentPos();
	
	/** 2번째 Mark를 인식한다. */
	m_plnkVision->ClearOverlay(iCamNo);
	iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 2번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkVisionCalibration->ConvertV2R(iCamNo, 
											m_plnkPanelAligner[iStageID]->GetInstanceNo(),
											dPixelX, dPixelY,
											posSecondMark.dX, posSecondMark.dY);
	}
	else
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	/** 고정좌표 계산 시작  */
	posFirst	= posFirst	- posFirstMark;
	posSecond	= posSecond - posSecondMark;
    
	/** Fisrt Position에서 Second Position까지의 거리를 구한다. */
	dDistanceX	= (posFirst.dX - posSecond.dX);
	dDistanceY	= (posFirst.dY - posSecond.dY);
	dDistanceXY	= sqrt(dDistanceX * dDistanceX + dDistanceY * dDistanceY);

	/** Camera 중심에서 First (Second) Position까지의 거리를 구한다.. */
	dRadius		= dDistanceXY / (sqrt(2 * (1 - cos(DEG2RAD(dTAngle)))));

	/** First Position과 Second Position을 지나고 반지름 dRadius를 가지는 원의 중심을 구한다. */
	FindCenter(dTAngle, dRadius, posFirst, posSecond, &posCenter);
	
	*pX			= posCenter.dX;	// 카메라의 X축 좌표 리턴
	*pY			= posCenter.dY;	// 카메라의 Y축 좌표 리턴

#endif // DEF_PLATFORM_PREBONDER_SYSTEM
	return ERR_HW_TEACH_SUCCESS;
}


/** Camera 하나의 고정좌표를 기준으로 다른 Camera의 고정 좌표를 계산하기
 * 기 등록된 Mark를 인식하여 위치를 알아낸다. 인식할 Mark는 Xu변 또는 Xd변의 좌상단 Mark임.
 * @preconditions ExecuteCalibration()을 수행하여 CamFactor가 계산되어 있어야 한다.
 *
 * @param	iStageID : 카메라 간 거리 얻기에 사용할 Stage ID (Stage1:DEF_STAGE_1, Stage2:DEF_STAGE_2)
 * @param	iCamera : 카메라 번호 (첫 기준으로 잡을 Camera 번호)
 * @param	iMark : 마크 번호
 * @param	iOtherCam : 거리를 구할 카메라 번호
 * @param	iOtherMark : 거리를 구할 Cmaera의 마크 번호
 * @param	*pdX : (OPTION=NULL) 계산된 X 방향 Camera 거리, NULL이면 return하지 않음
 * @param	*pdY : (OPTION=NULL) 계산된 Y 방향 Camera 거리, NULL이면 return하지 않음
 * @param	*pdT : (OPTION=NULL) 계산된 T 방향 Camera 거리, NULL이면 return하지 않음
 * @param	*pdZ : (OPTION=NULL) 계산된 Z 방향 Camera 거리, NULL이면 return하지 않음
 * @return	Error : 0 = Success, 그외=기타
 */
int MHW_Teach::GetCameraDistance(int iStageID,
								 int iCamNo,
								 int iMarkNo,
								 int iOtherCamNo,
								 int iOtherMarkNo,
								 double* pdX,
								 double* pdY)
{
	MPos_XYT	dRDistance;

	CString		tmpDisplay;			// Display를 위한 Temp String

	MPos_XYT	posFirstMark;		// 인식 좌표를 저장한다.
	MPos_XYT	posSecondMark;		// 인식 좌표를 저장한다.
	MPos_XYT	posFirst;		// 초기 좌표를 저장한다.
	MPos_XYT	posSecond;	// 이동한 후의 좌표를 저장한다.

	double		dPixelX;
	double		dPixelY;

	int			iResult;

	/** Display Vision */
	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iCamNo);
	
	/** Jog로 Mark를 이동하여 Vision에 들어오게 한다. */
	//if (MyMessageBox("Jog를 이용하여 Mark가 첫번째 카메라 Vision 화면에 들어오게 하고 확인 버튼을 눌러 주세요.",
	//	"확인", M_ICONQUESTION |M_YESNO, "확인", "취소") == IDNO)
	if (MyMessageBox(MY_MESSAGE_BOX,62402,_T("Confirmation"),M_ICONQUESTION |M_YESNO,_T("Confirm"),_T("Cancel")) == IDNO)
	{
		//WriteErrorLog("GetCameraDistance() : User Cancel Error");
		SetErrorLevel(_T("GetCameraDistance() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL
	}
	Sleep(1000);
	
	posFirst = m_plnkPanelAligner[iStageID]->GetXYTCurrentPos();
	
	/** 1번째 Mark를 인식한다. */
	m_plnkVision->ClearOverlay(iCamNo);
	iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 1번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkVisionCalibration->ConvertV2R(iCamNo, 
											m_plnkPanelAligner[iStageID]->GetInstanceNo(),
											dPixelX, dPixelY,
											posFirstMark.dX, posFirstMark.dY);
	}
	else
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}
		
	Sleep(500);

	/** Display Vision */
//	m_plnkVision->SelectCamera(iOtherCamNo);
	m_plnkVision->ClearOverlay(iOtherCamNo);
	m_plnkVision->LiveVideo(iOtherCamNo);
	
	/** Jog로 Mark를 이동하여 Vision에 들어오게 한다. */
	//if (MyMessageBox("Jog를 이용하여 Mark가 두번째 카메라 Vision 화면에 들어오게 하고 확인 버튼을 눌러 주세요.",
	//	"확인", M_ICONQUESTION |M_YESNO, "확인", "취소") == IDNO) 
	if (MyMessageBox(MY_MESSAGE_BOX,62403,_T("Confirmation"),M_ICONQUESTION |M_YESNO,_T("Confirm"),_T("Cancel")) == IDNO)
	{
		//WriteErrorLog("GetCameraDistance() : User Cancel Error");
		SetErrorLevel(_T("GetCameraDistance() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL
	}
	Sleep(1000);

	posSecond = m_plnkPanelAligner[iStageID]->GetXYTCurrentPos();
			
	/** 2번째 Mark를 인식한다. */
	m_plnkVision->ClearOverlay(iOtherCamNo);
	iResult = m_plnkVision->Grab(iOtherCamNo);
	if (iResult)
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	iResult = m_plnkVision->RecognitionPatternMark(iOtherCamNo, iOtherMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iOtherCamNo, iOtherMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iOtherCamNo, iOtherMarkNo);

		/** 2번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkVisionCalibration->ConvertV2R(iOtherCamNo, 
											m_plnkPanelAligner[iStageID]->GetInstanceNo(),
											dPixelX, dPixelY,
											posSecondMark.dX, posSecondMark.dY);
	}
	else
	{
		SetErrorLevel(_T("HARDWARE TEACH get Camera Fixed Position"),1, __FILE__, __LINE__);
		return iResult;
	}

	/** Camera 간 거리 구하기 */
	dRDistance = (posSecond - posSecondMark) - (posFirst - posFirstMark);
	
	/** 구한 Camera 간 거리 전달하기 */
	if (pdX != NULL)
		*pdX = dRDistance.dX;
	
	if (pdY != NULL)
		*pdY = dRDistance.dY;

	return ERR_HW_TEACH_SUCCESS;
}

/** Camera 기준좌표로 고정좌표 찾기
 * 기 등록된 Mark를 인식하여 위치를 알아낸다. 인식할 Mark는 Xu변 또는 Xd변의 좌상단 Mark임.
 * @preconditions ExecuteCalibration()을 수행하여 CamFactor가 계산되어 있어야 한다.
 * @preconditions SetCellData()를 이용해서 Cell정보가 입력되어 있어야 한다.
 *
 * @param iCamNo : 카메라 번호
 * @param iMark   : 마크 번호
 * @param pX : X 좌표
 * @param pY : Y 좌표  
 * @return	Error : 0 = Success, 그외=기타
 */
int MHW_Teach::GetTabCarrierCameraFixedPosition(int iCarrierID, 
								int iCamNo,
								int iMarkNo,
								double dTAngle,
								double* pX,
								double* pY,
								double* pCamDistX)
{
	double		dDistanceXY;
	double		dDistanceX;
	double		dDistanceY;
	double		dRadius;
	
	CString		tmpDisplay;			// Display를 위한 Temp String

	MPos_XYT	posFirstMark;		// 좌표를 저장한다.
	MPos_XYT	posSecondMark;		// 좌표를 저장한다.
	MPos_XYT	posFirst;			// 초기 좌표를 저장한다.
	MPos_XYT	posSecond;			// TAngle도 회전한후의 좌표를 저장한다.
	MPos_XYT	posMove;			// 이동할 좌표 저장
	MPos_XYT	posCenter;			// Center
	MPos_XYT	posTarget;			 

	int			iResult;
	double		dPixelX;
	double		dPixelY;

	MTabCarrier*  m_plnkTabCarrier;

	m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iCarrierID);

	posFirst = m_plnkTabCarrier->GetCurrentXYTPos();
	
	/**  1번째 Mark를 인식한다.-------------------------------*/
	m_plnkVision->Grab(iCamNo);

	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 1번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkVisionCalibration->ConvertV2R(iCamNo, 
											m_plnkTabCarrier->GetInstanceNo(),
											dPixelX, dPixelY,
											posFirstMark.dX, posFirstMark.dY);
	}
	else
	{
		return iResult;
	}


	
	/** Stage 축을 dTurnAgle만큼 회전하고, XY를 움직여 Mark를 움직여 화면내에 들어오게 한다. */
	///dDistanceX = m_plnkPreBonderData->m_dLead_Width_Xtab / 2 ;
#ifdef DEF_SOURCE_SYSTEM
	dDistanceX = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Xtab / 2 ;
#else
	dDistanceX = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Ytab / 2 ;
#endif
	dDistanceY = MOlbSystemPre.GetDistance_TabAlignerCenterToTabMarkX();


	double dir = -1.0;
	/*__________________________
		(-X) <---------> (+X)
				Cam
				-----------------
				|				|
				|	  Panel		|
				|				|
				-----------------
	___________________________*/
	Rotation(dTAngle, dDistanceX*dir, dDistanceY, &posMove.dX, &posMove.dY);

	posMove.dX = dDistanceX*dir - posMove.dX;
	posMove.dY = dDistanceY - posMove.dY;

	posMove.dX = posFirst.dX + posMove.dX;
	posMove.dY = posFirst.dY + posMove.dY;
	posMove.dT = posFirst.dT + dTAngle;

	iResult = m_plnkTabCarrier->MoveXYTPos(posMove, DEF_TABCARRIER_NO_WORKING_POS, TRUE, FALSE);
	if (iResult) return iResult;
	
	/** Display Vision */
	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iCamNo);

//	if ( MyMessageBox(_T("Jog를 이용하여 Mark가 Vision 화면에 들어오게 하고 OK 버튼을 눌러 주세요."),
//		_T("확인"),M_ICONQUESTION |M_YESNO,_T("OK"),_T("취소")) == IDNO)
	if (MyMessageBox("Using Jog! => Let The Mark In The Vision => Press OK!",
		_T("Confirm"),M_ICONQUESTION |M_YESNO,_T("OK"),_T("Cancel")) == IDNO)
	{
		SetErrorLevel(_T("GetCameraFixedPosition() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL;
	}

	posSecond = m_plnkTabCarrier->GetCurrentXYTPos();
	
	/** 2번째 Mark를 인식한다. */
	m_plnkVision->Grab(iCamNo);
	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel 좌표 */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 2번째 마크 인식한 Offset(Robot 좌표)을 저장한다. */
		m_plnkVisionCalibration->ConvertV2R(iCamNo, 
											m_plnkTabCarrier->GetInstanceNo(),
											dPixelX, dPixelY,
											posSecondMark.dX, posSecondMark.dY);
	}
	else
	{
		return iResult;
	}

	/** 고정좌표 계산 시작  */
	posFirst	= posFirst	- posFirstMark;
	posSecond	= posSecond - posSecondMark;
    
	/** Fisrt Position에서 Second Position까지의 거리를 구한다. */
	dDistanceX	= (posFirst.dX - posSecond.dX);
	dDistanceY	= (posFirst.dY - posSecond.dY);
	dDistanceXY	= sqrt(dDistanceX * dDistanceX + dDistanceY * dDistanceY);

	/** Camera 중심에서 First (Second) Position까지의 거리를 구한다.. */
	dRadius		= dDistanceXY / (sqrt(2 * (1 - cos(DEG2RAD(dTAngle)))));

	/** First Position과 Second Position을 지나고 반지름 dRadius를 가지는 원의 중심을 구한다. */
	FindCenter(dTAngle, dRadius, posFirst, posSecond, &posCenter);
	
///#ifdef DEF_MACHINE_COAXIAL_AXIS_CAMERA
	//Camera Center를 고정좌표로 하기위해_____
	int iOtherCamNo = (iCamNo == (DEF_F_TABALIGNER_CAMERA_1||DEF_R_TABALIGNER_CAMERA_1)) ? DEF_TAB_LEAD_XU_MARK1 : DEF_TAB_LEAD_XU_MARK2 ;

	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iOtherCamNo);
	m_plnkVision->ClearOverlay(iOtherCamNo);

//	if (MyMessageBox("Jog를 이용하여 2번째 Camera Mark가 Vision 화면에 들어오게 하고 OK 버튼을 눌러 주세요.",
//		"확인",M_ICONQUESTION |M_YESNO,"OK","취소") == IDNO)
	if (MyMessageBox("Using Jog! => Let 2nd Camera Mark In The Vision => Press OK!",
		_T("Confirm"),M_ICONQUESTION |M_YESNO,_T("OK"),_T("Cancel")) == IDNO)
	{
		SetErrorLevel(_T("GetCameraFixedPosition() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL;
	}
	m_plnkVision->Grab(iOtherCamNo);
	MPos_XYT	posThirdMark;
	MPos_XYT	posThird;
	posThird = 	m_plnkTabCarrier->GetCurrentXYTPos();
	iResult = m_plnkVision->RecognitionPatternMark(iOtherCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		dPixelX = m_plnkVision->GetSearchResultX(iOtherCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iOtherCamNo, iMarkNo);
		m_plnkVisionCalibration->ConvertV2R(iOtherCamNo, 
											m_plnkTabCarrier->GetInstanceNo(),
											dPixelX, dPixelY,
											posThirdMark.dX, posThirdMark.dY);
	}
	else
		return iResult;
	posThird = posThird - posThirdMark;
	posCenter.dX += ((posThird.dX-posSecond.dX)/2.0);
	posCenter.dY += ((posThird.dY-posSecond.dY)/2.0);
	//__________________________________________________

	*pCamDistX = fabs(posThird.dX - posSecond.dX);
///#endif
	*pX			= posCenter.dX;	// 카메라의 X축 좌표 리턴
	*pY			= posCenter.dY;	// 카메라의 Y축 좌표 리턴

	return ERR_HW_TEACH_SUCCESS;
}







/***************** Common Implementation *************************************/

/**
 * Assign component linkage
 * 
 * @param	listRefComponents	: MHW_Teach 참조 Component 
 * @return	Error : 0 = Success, 그외=기타
 */
int MHW_Teach::AssignComponents(SHW_TeachRefCompList listRefComponents)
{
	for (int i = 0; i < DEF_MAX_STAGE; i++)
		m_plnkPanelAligner[i]	= listRefComponents.m_plnkPanelAligner[i];

	m_plnkVision			= listRefComponents.m_plnkVision;
	m_plnkVisionCalibration	= listRefComponents.m_plnkVisionCalibration;
	return ERR_HW_TEACH_SUCCESS;
}

/** 
 * Component Data 처리 
 *
 * @param	pData : SMHW_TeachData 구조체에 대한 포인터
 * @return	Error : 0 = Success, 그외=기타
 */
int MHW_Teach::SetData(SHW_TeachData datComponent)
{
	m_plnkPanelData	= datComponent.m_plnkPanelData;
	return ERR_HW_TEACH_SUCCESS;
}

/** 
 * Component Data 처리 
 *
 * @param	pData : SDrawerData 구조체에 대한 포인터
 * @return	Error : 0 = Success, 그외=기타
 */
int MHW_Teach::GetData(SHW_TeachData *datComponent)
{
	datComponent->m_plnkPanelData	= m_plnkPanelData;
	return ERR_HW_TEACH_SUCCESS;
}

/* End Of Code */
