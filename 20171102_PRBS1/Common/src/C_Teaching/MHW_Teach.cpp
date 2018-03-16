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
 *					�� ������ Vision Calibration Class�� ���� �ڵ带 �����Ѵ�.
 *
 * @author Yong-Seog Pahn (dragon.pahn@samsung.com)
 * @version $Revision$
 */

#include "stdAfx.h"
#include "MHW_Teach.h"
#include "MPos_XYT.h"
#include "Common.h"		// MyMessageBox�� ����ϱ� ����
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
 * Default ������
 * @stereotype constructor
 */

/** Standard ������
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
 * �Ҹ���
 * @stereotype destructor 
 */
MHW_Teach::~MHW_Teach()
{
}

/**
 * MHW_Teach ������ �Ѵ�.
 *
 * @return	Error : 0 = Success, �׿�=��Ÿ
 */
int MHW_Teach::Initialize()
{
	int iResult = ERR_HW_TEACH_SUCCESS;
	return iResult;
}

/** Rotate an old point to a new point
  * @param dAngle : ����
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
  * @param dTAngle : ����
  * @param dRadius : ������
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

/** Camera ������ǥ�� ������ǥ ã��
 * �� ��ϵ� Mark�� �ν��Ͽ� ��ġ�� �˾Ƴ���. �ν��� Mark�� Xu�� �Ǵ� Xd���� �»�� Mark��.
 * @preconditions ExecuteCalibration()�� �����Ͽ� CamFactor�� ���Ǿ� �־�� �Ѵ�.
 * @preconditions SetCellData()�� �̿��ؼ� Cell������ �ԷµǾ� �־�� �Ѵ�.
 *
 * @param iStageID : ī�޶� ������ǥ ã�⿡ ����� Stage ID (Stage1:DEF_STAGE_1, Stage2:DEF_STAGE_2)
 * @param iCamera : ī�޶� ��ȣ
 * @param iMark   : ��ũ ��ȣ
 * @param pX : X ��ǥ
 * @param pY : Y ��ǥ  
 * @return	Error : 0 = Success, �׿�=��Ÿ
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
	
	CString		tmpDisplay;			// Display�� ���� Temp String

	MPos_XYT	posFirstMark;		// ��ǥ�� �����Ѵ�.
	MPos_XYT	posSecondMark;		// ��ǥ�� �����Ѵ�.
	MPos_XYT	posFirst;			// �ʱ� ��ǥ�� �����Ѵ�.
	MPos_XYT	posSecond;			// TAngle�� ȸ�������� ��ǥ�� �����Ѵ�.
	MPos_XYT	posMove;			// �̵��� ��ǥ ����
	MPos_XYT	posCenter;			// Cneter
	MPos_XYT	posTarget;			// 

	int			iResult;
	double		dPixelX;
	double		dPixelY;

	posFirst = m_plnkPanelAligner[iStageID]->GetXYTCurrentPos();
	
	/**  1��° Mark�� �ν��Ѵ�.-------------------------------*/
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
		/** Pixel ��ǥ */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 1��° ��ũ �ν��� Offset(Robot ��ǥ)�� �����Ѵ�. */
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
	
	// Stage ���� dTurnAgle ��ŭ ȸ���ϰ�, XY�� ������ Mark�� ������ ȭ�鳻�� ������ �Ѵ�.
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
		// 301002 = Camera ������ǥ ã�� ȸ�� ������ -90 ~ 90 �� ������ ����ϴ�.
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

	//if (MyMessageBox("Jog�� �̿��Ͽ� Mark�� Vision ȭ�鿡 ������ �ϰ� OK ��ư�� ���� �ּ���.",
	//	"Ȯ��",M_ICONQUESTION |M_YESNO,"OK","���") == IDNO)
	if (MyMessageBox(MY_MESSAGE_BOX,62401,_T("Confirmation"),M_ICONQUESTION |M_YESNO,_T("OK"),_T("Cancel")) == IDNO)
	{
		//WriteErrorLog("GetCameraFixedPosition() : User Cancel Error");
		SetErrorLevel(_T("GetCameraFixedPosition() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL;
	}

	posSecond = m_plnkPanelAligner[iStageID]->GetXYTCurrentPos();
	
	/** 2��° Mark�� �ν��Ѵ�. */
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
		/** Pixel ��ǥ */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 2��° ��ũ �ν��� Offset(Robot ��ǥ)�� �����Ѵ�. */
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

	/** ������ǥ ��� ����  */
	posFirst	= posFirst	- posFirstMark;
	posSecond	= posSecond - posSecondMark;
    
	/** Fisrt Position���� Second Position������ �Ÿ��� ���Ѵ�. */
	dDistanceX	= (posFirst.dX - posSecond.dX);
	dDistanceY	= (posFirst.dY - posSecond.dY);
	dDistanceXY	= sqrt(dDistanceX * dDistanceX + dDistanceY * dDistanceY);

	/** Camera �߽ɿ��� First (Second) Position������ �Ÿ��� ���Ѵ�.. */
	dRadius		= dDistanceXY / (sqrt(2 * (1 - cos(DEG2RAD(dTAngle)))));

	/** First Position�� Second Position�� ������ ������ dRadius�� ������ ���� �߽��� ���Ѵ�. */
	FindCenter(dTAngle, dRadius, posFirst, posSecond, &posCenter);
	
	*pX			= posCenter.dX;	// ī�޶��� X�� ��ǥ ����
	*pY			= posCenter.dY;	// ī�޶��� Y�� ��ǥ ����

#endif // DEF_PLATFORM_PREBONDER_SYSTEM
	return ERR_HW_TEACH_SUCCESS;
}


/** Camera �ϳ��� ������ǥ�� �������� �ٸ� Camera�� ���� ��ǥ�� ����ϱ�
 * �� ��ϵ� Mark�� �ν��Ͽ� ��ġ�� �˾Ƴ���. �ν��� Mark�� Xu�� �Ǵ� Xd���� �»�� Mark��.
 * @preconditions ExecuteCalibration()�� �����Ͽ� CamFactor�� ���Ǿ� �־�� �Ѵ�.
 *
 * @param	iStageID : ī�޶� �� �Ÿ� ��⿡ ����� Stage ID (Stage1:DEF_STAGE_1, Stage2:DEF_STAGE_2)
 * @param	iCamera : ī�޶� ��ȣ (ù �������� ���� Camera ��ȣ)
 * @param	iMark : ��ũ ��ȣ
 * @param	iOtherCam : �Ÿ��� ���� ī�޶� ��ȣ
 * @param	iOtherMark : �Ÿ��� ���� Cmaera�� ��ũ ��ȣ
 * @param	*pdX : (OPTION=NULL) ���� X ���� Camera �Ÿ�, NULL�̸� return���� ����
 * @param	*pdY : (OPTION=NULL) ���� Y ���� Camera �Ÿ�, NULL�̸� return���� ����
 * @param	*pdT : (OPTION=NULL) ���� T ���� Camera �Ÿ�, NULL�̸� return���� ����
 * @param	*pdZ : (OPTION=NULL) ���� Z ���� Camera �Ÿ�, NULL�̸� return���� ����
 * @return	Error : 0 = Success, �׿�=��Ÿ
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

	CString		tmpDisplay;			// Display�� ���� Temp String

	MPos_XYT	posFirstMark;		// �ν� ��ǥ�� �����Ѵ�.
	MPos_XYT	posSecondMark;		// �ν� ��ǥ�� �����Ѵ�.
	MPos_XYT	posFirst;		// �ʱ� ��ǥ�� �����Ѵ�.
	MPos_XYT	posSecond;	// �̵��� ���� ��ǥ�� �����Ѵ�.

	double		dPixelX;
	double		dPixelY;

	int			iResult;

	/** Display Vision */
	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iCamNo);
	
	/** Jog�� Mark�� �̵��Ͽ� Vision�� ������ �Ѵ�. */
	//if (MyMessageBox("Jog�� �̿��Ͽ� Mark�� ù��° ī�޶� Vision ȭ�鿡 ������ �ϰ� Ȯ�� ��ư�� ���� �ּ���.",
	//	"Ȯ��", M_ICONQUESTION |M_YESNO, "Ȯ��", "���") == IDNO)
	if (MyMessageBox(MY_MESSAGE_BOX,62402,_T("Confirmation"),M_ICONQUESTION |M_YESNO,_T("Confirm"),_T("Cancel")) == IDNO)
	{
		//WriteErrorLog("GetCameraDistance() : User Cancel Error");
		SetErrorLevel(_T("GetCameraDistance() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL
	}
	Sleep(1000);
	
	posFirst = m_plnkPanelAligner[iStageID]->GetXYTCurrentPos();
	
	/** 1��° Mark�� �ν��Ѵ�. */
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
		/** Pixel ��ǥ */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 1��° ��ũ �ν��� Offset(Robot ��ǥ)�� �����Ѵ�. */
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
	
	/** Jog�� Mark�� �̵��Ͽ� Vision�� ������ �Ѵ�. */
	//if (MyMessageBox("Jog�� �̿��Ͽ� Mark�� �ι�° ī�޶� Vision ȭ�鿡 ������ �ϰ� Ȯ�� ��ư�� ���� �ּ���.",
	//	"Ȯ��", M_ICONQUESTION |M_YESNO, "Ȯ��", "���") == IDNO) 
	if (MyMessageBox(MY_MESSAGE_BOX,62403,_T("Confirmation"),M_ICONQUESTION |M_YESNO,_T("Confirm"),_T("Cancel")) == IDNO)
	{
		//WriteErrorLog("GetCameraDistance() : User Cancel Error");
		SetErrorLevel(_T("GetCameraDistance() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL
	}
	Sleep(1000);

	posSecond = m_plnkPanelAligner[iStageID]->GetXYTCurrentPos();
			
	/** 2��° Mark�� �ν��Ѵ�. */
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
		/** Pixel ��ǥ */
		dPixelX = m_plnkVision->GetSearchResultX(iOtherCamNo, iOtherMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iOtherCamNo, iOtherMarkNo);

		/** 2��° ��ũ �ν��� Offset(Robot ��ǥ)�� �����Ѵ�. */
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

	/** Camera �� �Ÿ� ���ϱ� */
	dRDistance = (posSecond - posSecondMark) - (posFirst - posFirstMark);
	
	/** ���� Camera �� �Ÿ� �����ϱ� */
	if (pdX != NULL)
		*pdX = dRDistance.dX;
	
	if (pdY != NULL)
		*pdY = dRDistance.dY;

	return ERR_HW_TEACH_SUCCESS;
}

/** Camera ������ǥ�� ������ǥ ã��
 * �� ��ϵ� Mark�� �ν��Ͽ� ��ġ�� �˾Ƴ���. �ν��� Mark�� Xu�� �Ǵ� Xd���� �»�� Mark��.
 * @preconditions ExecuteCalibration()�� �����Ͽ� CamFactor�� ���Ǿ� �־�� �Ѵ�.
 * @preconditions SetCellData()�� �̿��ؼ� Cell������ �ԷµǾ� �־�� �Ѵ�.
 *
 * @param iCamNo : ī�޶� ��ȣ
 * @param iMark   : ��ũ ��ȣ
 * @param pX : X ��ǥ
 * @param pY : Y ��ǥ  
 * @return	Error : 0 = Success, �׿�=��Ÿ
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
	
	CString		tmpDisplay;			// Display�� ���� Temp String

	MPos_XYT	posFirstMark;		// ��ǥ�� �����Ѵ�.
	MPos_XYT	posSecondMark;		// ��ǥ�� �����Ѵ�.
	MPos_XYT	posFirst;			// �ʱ� ��ǥ�� �����Ѵ�.
	MPos_XYT	posSecond;			// TAngle�� ȸ�������� ��ǥ�� �����Ѵ�.
	MPos_XYT	posMove;			// �̵��� ��ǥ ����
	MPos_XYT	posCenter;			// Center
	MPos_XYT	posTarget;			 

	int			iResult;
	double		dPixelX;
	double		dPixelY;

	MTabCarrier*  m_plnkTabCarrier;

	m_plnkTabCarrier = MOlbSystemPre.GetTabCarrierComponent(iCarrierID);

	posFirst = m_plnkTabCarrier->GetCurrentXYTPos();
	
	/**  1��° Mark�� �ν��Ѵ�.-------------------------------*/
	m_plnkVision->Grab(iCamNo);

	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel ��ǥ */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 1��° ��ũ �ν��� Offset(Robot ��ǥ)�� �����Ѵ�. */
		m_plnkVisionCalibration->ConvertV2R(iCamNo, 
											m_plnkTabCarrier->GetInstanceNo(),
											dPixelX, dPixelY,
											posFirstMark.dX, posFirstMark.dY);
	}
	else
	{
		return iResult;
	}


	
	/** Stage ���� dTurnAgle��ŭ ȸ���ϰ�, XY�� ������ Mark�� ������ ȭ�鳻�� ������ �Ѵ�. */
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

//	if ( MyMessageBox(_T("Jog�� �̿��Ͽ� Mark�� Vision ȭ�鿡 ������ �ϰ� OK ��ư�� ���� �ּ���."),
//		_T("Ȯ��"),M_ICONQUESTION |M_YESNO,_T("OK"),_T("���")) == IDNO)
	if (MyMessageBox("Using Jog! => Let The Mark In The Vision => Press OK!",
		_T("Confirm"),M_ICONQUESTION |M_YESNO,_T("OK"),_T("Cancel")) == IDNO)
	{
		SetErrorLevel(_T("GetCameraFixedPosition() : User Cancel Error"),1, __FILE__, __LINE__);
		return generateErrorCode(301001);	// ERR_HW_USER_CANEL;
	}

	posSecond = m_plnkTabCarrier->GetCurrentXYTPos();
	
	/** 2��° Mark�� �ν��Ѵ�. */
	m_plnkVision->Grab(iCamNo);
	iResult = m_plnkVision->RecognitionPatternMark(iCamNo, iMarkNo);
	if (iResult == ERR_VISION_SUCCESS)
	{
		/** Pixel ��ǥ */
		dPixelX = m_plnkVision->GetSearchResultX(iCamNo, iMarkNo);
		dPixelY = m_plnkVision->GetSearchResultY(iCamNo, iMarkNo);

		/** 2��° ��ũ �ν��� Offset(Robot ��ǥ)�� �����Ѵ�. */
		m_plnkVisionCalibration->ConvertV2R(iCamNo, 
											m_plnkTabCarrier->GetInstanceNo(),
											dPixelX, dPixelY,
											posSecondMark.dX, posSecondMark.dY);
	}
	else
	{
		return iResult;
	}

	/** ������ǥ ��� ����  */
	posFirst	= posFirst	- posFirstMark;
	posSecond	= posSecond - posSecondMark;
    
	/** Fisrt Position���� Second Position������ �Ÿ��� ���Ѵ�. */
	dDistanceX	= (posFirst.dX - posSecond.dX);
	dDistanceY	= (posFirst.dY - posSecond.dY);
	dDistanceXY	= sqrt(dDistanceX * dDistanceX + dDistanceY * dDistanceY);

	/** Camera �߽ɿ��� First (Second) Position������ �Ÿ��� ���Ѵ�.. */
	dRadius		= dDistanceXY / (sqrt(2 * (1 - cos(DEG2RAD(dTAngle)))));

	/** First Position�� Second Position�� ������ ������ dRadius�� ������ ���� �߽��� ���Ѵ�. */
	FindCenter(dTAngle, dRadius, posFirst, posSecond, &posCenter);
	
///#ifdef DEF_MACHINE_COAXIAL_AXIS_CAMERA
	//Camera Center�� ������ǥ�� �ϱ�����_____
	int iOtherCamNo = (iCamNo == (DEF_F_TABALIGNER_CAMERA_1||DEF_R_TABALIGNER_CAMERA_1)) ? DEF_TAB_LEAD_XU_MARK1 : DEF_TAB_LEAD_XU_MARK2 ;

	m_plnkVision->ClearOverlay(iCamNo);
	m_plnkVision->LiveVideo(iOtherCamNo);
	m_plnkVision->ClearOverlay(iOtherCamNo);

//	if (MyMessageBox("Jog�� �̿��Ͽ� 2��° Camera Mark�� Vision ȭ�鿡 ������ �ϰ� OK ��ư�� ���� �ּ���.",
//		"Ȯ��",M_ICONQUESTION |M_YESNO,"OK","���") == IDNO)
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
	*pX			= posCenter.dX;	// ī�޶��� X�� ��ǥ ����
	*pY			= posCenter.dY;	// ī�޶��� Y�� ��ǥ ����

	return ERR_HW_TEACH_SUCCESS;
}







/***************** Common Implementation *************************************/

/**
 * Assign component linkage
 * 
 * @param	listRefComponents	: MHW_Teach ���� Component 
 * @return	Error : 0 = Success, �׿�=��Ÿ
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
 * Component Data ó�� 
 *
 * @param	pData : SMHW_TeachData ����ü�� ���� ������
 * @return	Error : 0 = Success, �׿�=��Ÿ
 */
int MHW_Teach::SetData(SHW_TeachData datComponent)
{
	m_plnkPanelData	= datComponent.m_plnkPanelData;
	return ERR_HW_TEACH_SUCCESS;
}

/** 
 * Component Data ó�� 
 *
 * @param	pData : SDrawerData ����ü�� ���� ������
 * @return	Error : 0 = Success, �׿�=��Ÿ
 */
int MHW_Teach::GetData(SHW_TeachData *datComponent)
{
	datComponent->m_plnkPanelData	= m_plnkPanelData;
	return ERR_HW_TEACH_SUCCESS;
}

/* End Of Code */
