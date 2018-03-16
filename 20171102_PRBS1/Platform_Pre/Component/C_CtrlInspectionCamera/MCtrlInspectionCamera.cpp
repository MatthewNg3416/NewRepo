#include "StdAfx.h"
#include <math.h>
#include "MPos_XY.h"
#include "MSystemData.h"
#include "MProcessData.h"
#include "IVision.h"
#include "MCtrlInspectionCamera.h"
#include "MCameraCarrier.h"
#include "MTabData.h"
#include "MVisionCalibration.h"
#include "MPlatformOlbSystem.h"
#include "MTrsAutoManager.h"

//20120215 SJ_HJG 추가
#include "MPreBonderData.h"
#include "ILighting.h"

//20130605 SJ_HJG
#include "MFileUtil.h"
#include "MPanelAligner.h"
#include "MTabMounter.h"

#include "common.h"

extern MPlatformOlbSystem	MOlbSystemPre;

MCtrlInspectionCamera::MCtrlInspectionCamera(SCommonAttribute commonData, SCtrlInspectionCameraRefCompList listRefComponent, SCtrlInspectionCameraData datComponent)
: ICommon(commonData)
{	
	AssignComponents(listRefComponent);
	SetData(datComponent);

//@#ifdef DEF_SOURCE_SYSTEM
//@	m_iWorkGroup = DEF_FRONT_GROUP;
//@	m_iWorkInstance = m_iInstanceNo;
//@#else
	if (m_iInstanceNo < DEF_MAX_INSPECTION_CAM_CARRIER)
	{
		m_iWorkGroup = DEF_FRONT_GROUP;
		m_iWorkInstance = m_iInstanceNo;
	}
	else
	{
		m_iWorkGroup = DEF_REAR_GROUP;
		m_iWorkInstance = m_iInstanceNo - DEF_MAX_INSPECTION_CAM_CARRIER;
	}
//@#endif
}

MCtrlInspectionCamera::~MCtrlInspectionCamera()
{
}

int MCtrlInspectionCamera::Initialize(void)
{
	return ERR_CTRL_INSPECTIONCAMERA_SUCCESS;
}

void MCtrlInspectionCamera::SetData(SCtrlInspectionCameraData datcomponent)
{
	m_plnkSystemData	= datcomponent.m_plnkSystemData;
	m_plnkProcessData	= datcomponent.m_plnkProcessData;
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_FIRST] = datcomponent.m_plnkWorkSchedule;
#ifdef DEF_SOURCE_SYSTEM
	m_plnkWorkSchedule[DEF_WORK_SCHEDULE_SECOND] = datcomponent.m_plnkWorkScheduleSecond;
#endif
	m_plnkPreBonderData = datcomponent.m_plnkPreBonderData;
	m_plnkTabData = datcomponent.m_plnkTabData;
}

void MCtrlInspectionCamera::AssignComponents(SCtrlInspectionCameraRefCompList listRefComponents)
{
	m_plnkVision				= listRefComponents.m_plnkVision;
	m_plnkCamCarrier			= listRefComponents.m_plnkCamCarrier;
	m_plnkCtrlVisionCalibration = listRefComponents.m_plnkCtrlVisionCalibration;
	m_plnkTabMounter			= listRefComponents.m_plnkTabMounter;
	//20120216 SJ_HJG
	for(int i = 0; i < DEF_MAX_LIGHTING; i++)
	{
		m_plnkLighting[i]		= listRefComponents.m_plnkLighting[i];
	}
}

//20111020 by kss
/////////////////////////////////////////////////////////
int MCtrlInspectionCamera::InspectBeforeMount(int iScheduleNo, int iTabNo, CString strGlassData)
{
	return getTabAlignOffset(iScheduleNo, iTabNo, DEF_BEFORE_MOUNT, strGlassData);
}

int MCtrlInspectionCamera::InspectAfterMount(int iScheduleNo, int iTabNo, CString strGlassData)
{
	return getTabAlignOffset(iScheduleNo, iTabNo, DEF_AFTER_MOUNT, strGlassData);
}
/////////////////////////////////////////////////////////

int MCtrlInspectionCamera::getTabAlignOffset(int iScheduleNo, int iTabNo, int iTargetStatus, CString strGlassData)
{
	//const EWorkingSide eWorkSide = m_plnkCamCarrier->GetCameraCarrierWorkSide();
	const EWorkingSide eWorkSide = WORKING_SIDE_X_UP;

	if (DRY_RUN_MODE == m_plnkSystemData->m_eRunMode)
	{
		Sleep(100);

//		MPos_XY* pxyInspectionOffset = m_plnkProcessData->GetInspectionOffsetStoragePointer(eWorkSide, iTabNo);
//		pxyInspectionOffset->dX = 0.0;
//		pxyInspectionOffset->dY = 0.0;

		MPos_XYT InspectionOffset(0.0, 0.0, 0.0);
		m_plnkWorkSchedule[iScheduleNo]->SetInspectionOffset(iTabNo, InspectionOffset);

		return ERR_CTRL_INSPECTIONCAMERA_SUCCESS;
	}

	const int iCamNo = m_plnkCamCarrier->GetOwnedCameraNo1();
	const int iPairCamNo = m_plnkCamCarrier->GetOwnedCameraNo2();
	
	m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOn(); //171002 JSH

	Sleep(100); //170926 JSh 품질확인

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// parameter 유효성 검증, 사용 Data 준비(Mark 번호 etc.)
//	int iMaxTabNo = m_plnkProcessData->GetMaxTabQty(eWorkSide);

	/*/130106.KKY.Del(임시)_______
	int iMaxTabNo = m_plnkWorkSchedule[iScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP);	//110125.수정요
	if (iTabNo < 0 || iTabNo >= iMaxTabNo)
	{
		m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
		SetErrorLevel(_T("INSPECTIONCAMERA get Tab Align Offset"),1, __FILE__, __LINE__);
		// 324015 = TabIC 번호 오류. (Inspection 동작 시작 시)
		return generateErrorCode(324015);
	}
	/*/

	int rgiModel[] = { -1, -1, -1, -1 };
	if (iTargetStatus == DEF_BEFORE_MOUNT)
	{
		rgiModel[0] = DEF_PANEL_LEAD_XU_MARK1 + int(eWorkSide) * 2;
		rgiModel[1] = DEF_PANEL_LEAD_XU_MARK2 + int(eWorkSide) * 2;
		rgiModel[2] = DEF_TAB_LEAD_XU_MARK1 + int(eWorkSide) * 2;
		rgiModel[3] = DEF_TAB_LEAD_XU_MARK2 + int(eWorkSide) * 2;
	}
	else // if (iTargetStatus == DEF_AFTER_MOUNT)
	{
		rgiModel[0] = DEF_PANEL_LEAD_XU_MARK1 + int(eWorkSide) * 2;
		rgiModel[1] = DEF_PANEL_LEAD_XU_MARK2 + int(eWorkSide) * 2;
		rgiModel[2] = DEF_TAB_LEAD_XU_AFTER_MOUNT_MARK1 + int(eWorkSide) * 2;
		rgiModel[3] = DEF_TAB_LEAD_XU_AFTER_MOUNT_MARK2 + int(eWorkSide) * 2;
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Unit 상태 확인은 무시!

	// Panel Aligner 상태
	// 1. Panel 흡착
	// 2. Panel Align 완료
	// 3. Inspection 위치 or Mount 위치

	// TabMounter 상태
	// 1. TabIC 흡착
	// 2. Mount 위치 (Mounter 축)
	// 3. Mounter 높이 (DownReady)
	// 4. Backup 위치 (후진, 하강)

	// CameraCarrier 상태
	// 1. CameraCarrier 위치


	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Vision 인식
	m_plnkVision->LockCamera(iCamNo);
	if (-1 != iPairCamNo)
		m_plnkVision->LockCamera(iPairCamNo);

	m_plnkVision->ClearOverlay(iCamNo);
	if (-1 != iPairCamNo)
		m_plnkVision->ClearOverlay(iPairCamNo);
	
	int iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iCamNo);
		if (-1 != iPairCamNo)
			m_plnkVision->UnlockCamera(iPairCamNo);
		
		m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
		return iResult;
	}

	if (-1 != iPairCamNo)
	{
		iResult = m_plnkVision->Grab(iPairCamNo);
		if (iResult)
		{
			m_plnkVision->UnlockCamera(iCamNo);
			if (-1 != iPairCamNo)
				m_plnkVision->UnlockCamera(iPairCamNo);
			
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
			return iResult;
		}
	}

	// Panel Tab Lead Mark 인식
	int rgiCamNo[2] = { iCamNo, iCamNo };
	if (-1 != iPairCamNo)
		rgiCamNo[1] = iPairCamNo;
	
	for (int i = 0; i < 2; i++)
	{
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[i]);

		if (iResult)	// Mark 인식 Retry 수행.
		{
			for (int j=0; j<5; j++)
			{
				Sleep(300);
				iResult = m_plnkVision->Grab(rgiCamNo[i]);
				iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[i]);
				if (iResult == SUCCESS)
					break;
			}
		}

		//20120215 SJ_HJG
		if (iResult)	// 측광 조명 Off후 Mark 인식 Retry 수행.
		{
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff();
//@			Sleep(300);
			Sleep(500);
			iResult = m_plnkVision->Grab(rgiCamNo[i]);
			iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[i]);
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->SetValue((unsigned short)m_plnkPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[iCamNo].x]);
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOn();
			//Sleep(200);
//@			Sleep(500); //120925. SJ_YTH
			Sleep(500);
		}
		//______________________________________
		

		if (iResult)
		{
			// Error Image 저장.
			CString strInfo;
			for (int j = 0; j < 2; j++)
			{
				if (-1 == rgiCamNo[j])
					continue;

				if (iTargetStatus == DEF_BEFORE_MOUNT)
					strInfo.Format(_T("err_b_inspection_%d_%d_%d"), eWorkSide, iTabNo, j + 1);
				else // if (iTargetStatus == DEF_AFTER_MOUNT)
					strInfo.Format(_T("err_a_inspection_%d_%d_%d"), eWorkSide, iTabNo, j + 1);

				m_plnkVision->SaveImage(rgiCamNo[j], (LPCTSTR)strInfo);
			}

			// 확인 필요.
			//if (TRUE == m_plnkSystemData->m_bUsePanelLeadMarkPosPresumption)
			if (FALSE)
			{
				// 1번 측 Mark 인식 성공.
				
			}
			else
			{
				//ErrorImage MainView에 보여주기_____
				Sleep(200);
				m_plnkVision->Grab(iCamNo);
				m_plnkVision->Grab(iPairCamNo);
				//___________________________________

				m_plnkVision->UnlockCamera(iCamNo);
				if (-1 != iPairCamNo)
					m_plnkVision->UnlockCamera(iPairCamNo);
				
				m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH				
				return getErrorCode(rgiCamNo[i], iResult, 0/*Panel Mark Type*/);
			}
		}
	}

	MPos_XY rgRxy[4];
	MPos_XY rgVxy[4];	// Vision 인식 좌표 임시 저장 변수
	int iPrizmSide = 0;	// OLB 설비 좌표계 축 기준으로 작은 쪽이 0, 큰 쪽이 1 이다.
	int iCalibUnitNo = 0;
#ifdef DEF_SOURCE_SYSTEM
	iCalibUnitNo = 0;
#else
	iCalibUnitNo = 0;//110125.확인요
#endif
	if (iTargetStatus == DEF_AFTER_MOUNT)
	{
		RECT sArea;
		for (int i = 0; i < 2; i++)
		{
			// 전 Unit 에서 동일하게 OLB 설비 좌표계 기준으로 Vision 인식 좌표를 얻는다.		
			// 따라서, Unit 에 따라 dVx 가 Mounter 에 반영되어야 할 수도 있고, Aligner 에 반영되어야 할 수도 있다.
			rgVxy[i].dX = m_plnkVision->GetSearchResultX(rgiCamNo[i], rgiModel[i]);
			rgVxy[i].dY = m_plnkVision->GetSearchResultY(rgiCamNo[i], rgiModel[i]);
			if (-1 == iPairCamNo)
				iPrizmSide = i % 2;		//@미사용

			m_plnkCtrlVisionCalibration->ConvertV2R(rgiCamNo[i], iCalibUnitNo, rgVxy[i].dX, rgVxy[i].dY, rgRxy[i].dX, rgRxy[i].dY);
			
			if (TRUE == m_plnkSystemData->m_bUseAutoSearchAreaInAfterInspection)
			{
				sArea.left = (long)rgVxy[i].dX - 100;
				if (sArea.left <= 0)
					sArea.left = 1;
				
				sArea.right = (long)rgVxy[i].dX + 100;
				if (sArea.right >= 640)
					sArea.right = 639;
				
				sArea.top = (long)rgVxy[i].dY - 80;
				if (sArea.top <= 0)
					sArea.top = 1;
				
				sArea.bottom = (long)rgVxy[i].dY;
				if (sArea.bottom >= 480)
					sArea.bottom = 479;
				
				m_plnkVision->SetSearchArea(rgiCamNo[i], rgiModel[i + 2], sArea);
			}
		}
	}

	// TabIC Lead Mark 인식
	for (int i = 0; i < 2; i++)
	{
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[2 + i]);
		if (iResult)
		{
			for (int j=0; j<5; j++)
			{
				Sleep(300);
				iResult = m_plnkVision->Grab(rgiCamNo[i]);
				iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[2 + i]);
				if (iResult == SUCCESS)
					break;
			}
		}

		
		//20120215 SJ_HJG
		if (iResult)	// 동축 조명 Off후 Mark 인식 Retry 수행.
		{
			m_plnkLighting[s_rgLightIdForCam[iCamNo].y]->LightOff();
			Sleep(300);
			iResult = m_plnkVision->Grab(rgiCamNo[i]);
			iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[2 + i]);
			m_plnkLighting[s_rgLightIdForCam[iCamNo].y]->SetValue((unsigned short)m_plnkPreBonderData->m_rgiLightingValue[s_rgLightIdForCam[iCamNo].y]);
			m_plnkLighting[s_rgLightIdForCam[iCamNo].y]->LightOn();
			Sleep(200);
		}
		//______________________________________
		

		if (iResult)
		{
			// Error Image 저장.
			CString strInfo;
			for (int j = 0; j < 2; j++)
			{
				if (-1 == rgiCamNo[j])
					continue;

				if (iTargetStatus == DEF_BEFORE_MOUNT)
					strInfo.Format(_T("err_b_inspection_%d_%d_%d"), eWorkSide, iTabNo, j + 1);
				else // if (iTargetStatus == DEF_AFTER_MOUNT)
					strInfo.Format(_T("err_a_inspection_%d_%d_%d"), eWorkSide, iTabNo, j + 1);

				m_plnkVision->SaveImage(rgiCamNo[j], (LPCTSTR)strInfo);
			}

			//ErrorImage MainView에 보여주기_____
			Sleep(200);
			m_plnkVision->Grab(iCamNo);
			m_plnkVision->Grab(iPairCamNo);
			//___________________________________


			m_plnkVision->UnlockCamera(iCamNo);
			if (-1 != iPairCamNo)
				m_plnkVision->UnlockCamera(iPairCamNo);
			
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
			return getErrorCode(rgiCamNo[i], iResult, 1/*TabIC Mark Type*/);
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		if (iTargetStatus == DEF_AFTER_MOUNT && (0 == i || 1 == i))
			continue;

		// 전 Unit 에서 동일하게 OLB 설비 좌표계 기준으로 Vision 인식 좌표를 얻는다.		
		// 따라서, Unit 에 따라 dVx 가 Mounter 에 반영되어야 할 수도 있고, Aligner 에 반영되어야 할 수도 있다.
		rgVxy[i].dX = m_plnkVision->GetSearchResultX(rgiCamNo[i % 2], rgiModel[i]);
		rgVxy[i].dY = m_plnkVision->GetSearchResultY(rgiCamNo[i % 2], rgiModel[i]);
//		if (-1 == iPairCamNo)
//			iPrizmSide = i % 2;
#ifdef DEF_SOURCE_SYSTEM
		iPrizmSide = 0;		//@미사용
#else
		iPrizmSide = 1;		//@미사용
#endif
		m_plnkCtrlVisionCalibration->ConvertV2R(rgiCamNo[i % 2], iCalibUnitNo, rgVxy[i].dX, rgVxy[i].dY, rgRxy[i].dX, rgRxy[i].dY);
	}
	m_plnkVision->UnlockCamera(iCamNo);
	if (-1 != iPairCamNo)
		m_plnkVision->UnlockCamera(iPairCamNo);

	/*
	if (iTargetStatus == DEF_BEFORE_MOUNT)
	{
		MPos_XY* rgxyOldPanelMarkVisionPos
			= m_plnkProcessData->GetOldInspectionPanelMarkVisionPosStoragePointer(eWorkSide, iTabNo);	//???.왜필요할까???

		for (int i = 0; i < 2; i++)
			rgxyOldPanelMarkVisionPos[i] = rgVxy[i];
	}
	*/
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	// Vision 인식 결과 계산 (Mark 간 Center Point, AlignOffset 계산)
	
	double dTabCenterX = (rgRxy[2].dX + rgRxy[3].dX) / 2.0;
	double dTabCenterY = (rgRxy[2].dY + rgRxy[3].dY) / 2.0;

	double dPanelCenterX = (rgRxy[0].dX + rgRxy[1].dX) / 2.0;
	double dPanelCenterY = (rgRxy[0].dY + rgRxy[1].dY) / 2.0;

	double dResultX = -dTabCenterX + dPanelCenterX;
	double dResultY = -dTabCenterY + dPanelCenterY;
//	if (m_iInstanceNo == 3)
//	{
//		TRACE(_T("=====TCX:%.3lf,TCY:%.3lf,	PCX:%.3lf,PCY:%.3lf,	ResultX:%.3lf, ResultY:%.3lf=====\n"),
//			dTabCenterX, dTabCenterY, dPanelCenterX, dPanelCenterY, dResultX, dResultY);
//	}

	// xyAlignOffset->dX 가 Mounter 방향 Align Offset 으로 적용된다.
//	MPos_XY xyAlignOffset;
//	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)

//@#ifdef DEF_SOURCE_SYSTEM
//@	{
//@		xyAlignOffset.dX = dResultY;	// Source Inspection Camera Calibration 은 Panel Aligner 로 수행하므로, Y 값을 Mounter 축에 반영해야 한다.
//@		xyAlignOffset.dY = dResultX;
//@	}
//	else // if (SYSTEM_TYPE_GATE == m_plnkSystemData->m_eSystemType)
//@#else
//	{
//		xyAlignOffset.dX = dResultX;
//		xyAlignOffset.dY = dResultY;
//	}
//@#endif

	//20130520 SJ_HJG
	// Tab lead mark 길이 측정
	//  TabIC Mark 와 Panel Mark의 Mark간의 간격차를 비교하여 일정값을 초과하면 에러 처리..
	double dTabXCheck = m_plnkSystemData->m_dTabXCheck;
	double tmpCellLength,tmpTabLength;
	double cur_width;
	CString strTabCheckLog;
	MPos_XY tab_mark_l, tab_mark_r, cell_mark_l, cell_mark_r;

	if(eWorkSide == WORKING_SIDE_X_UP) //Xu
		cur_width = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Xtab;
	else
		cur_width = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Ytab;

	// Tab IC 좌표값 계산
#ifdef DEF_GATE_SYSTEM
	tab_mark_l.dX = rgRxy[2].dX;
	tab_mark_l.dY = rgRxy[2].dY;
	tab_mark_r.dX = rgRxy[3].dX + cur_width;
	tab_mark_r.dY = rgRxy[3].dY;

	// Cell 좌표값 계산
	cell_mark_l.dX = rgRxy[0].dX;
	cell_mark_l.dY = rgRxy[0].dY;
	cell_mark_r.dX = rgRxy[1].dX + cur_width;
	cell_mark_r.dY = rgRxy[1].dY;
#else
	tab_mark_l.dX = rgRxy[2].dY;
	tab_mark_l.dY = rgRxy[2].dX;
	tab_mark_r.dX = rgRxy[3].dY + cur_width;
	tab_mark_r.dY = rgRxy[3].dX;

	// Cell 좌표값 계산
	cell_mark_l.dX = rgRxy[0].dY;
	cell_mark_l.dY = rgRxy[0].dX;
	cell_mark_r.dX = rgRxy[1].dY + cur_width;
	cell_mark_r.dY = rgRxy[1].dX;

#endif

	tmpTabLength = _hypot(tab_mark_r.dX-tab_mark_l.dX, tab_mark_r.dY-tab_mark_l.dY);
	tmpCellLength = _hypot(cell_mark_r.dX-cell_mark_l.dX, cell_mark_r.dY-cell_mark_l.dY);

	//Gantry Mount Pos 위치 Check...
	//1).Gantry Insoection Pos과 Mount Pos 차이 값을 계산
	//2).현재 Gantry Insoection에서의 TabIC Mark와 Panel Mark의 Y 위치 차이값을 받아와서
	// 1)과 2) 의 값을 비교 하여 일정값을 초과 하면Error처리.

	double dGantryCheck = m_plnkSystemData->m_dTabYCheck;
	double dGantryInspectionPos,dGantryMountPos;	
	double dGantryPosDiff,dGantryCurPosdiff;

	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
		dGantryMountPos = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER1)->GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS);
		dGantryInspectionPos = MOlbSystemPre.GetTabMounterComponent(DEF_TABMOUNTER1)->GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS);
	}
	else
	{
		dGantryMountPos = MOlbSystemPre.GetRTabMounterComponent(DEF_TABMOUNTER1)->GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_MOUNT_POS);
		dGantryInspectionPos = MOlbSystemPre.GetRTabMounterComponent(DEF_TABMOUNTER1)->GetTargetGantryPos(DEF_TABMOUNTER_GANTRY_INSPECTION_POS);		
	}
	//Gantry Insoection Pos과 Mount Pos 차이 값을 계산 2
	dGantryPosDiff = fabs( dGantryInspectionPos - dGantryMountPos);

	//TabIC Mark와 Panel Mark의 Y위치 차
	dGantryCurPosdiff = fabs( dResultY);

	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
		strTabCheckLog.Format(_T("Front TabNo:%d Panel Mark1 X:[%0.3f] Y:[%0.3f], Panel Mark2 X:[%0.3f] Y:[%0.3f], Tab Mark1 X:[%0.3f] Y:[%0.3f], Tab Mark2 X:[%0.3f] Y:[%0.3f]"),
								iTabNo + 1,rgRxy[0].dX,rgRxy[0].dY,rgRxy[1].dX,rgRxy[1].dY,rgRxy[2].dX,rgRxy[2].dY,rgRxy[3].dX,rgRxy[3].dY);
	}
	else
	{
		strTabCheckLog.Format(_T("Rear TabNo:%d Panel Mark1 X:[%0.3f] Y:[%0.3f], Panel Mark2 X:[%0.3f] Y:[%0.3f], Tab Mark1 X:[%0.3f] Y:[%0.3f], Tab Mark2 X:[%0.3f] Y:[%0.3f]"),
								iTabNo + 1,rgRxy[0].dX,rgRxy[0].dY,rgRxy[1].dX,rgRxy[1].dY,rgRxy[2].dX,rgRxy[2].dY,rgRxy[3].dX,rgRxy[3].dY);
	}
	WriteTabCheckLog(strTabCheckLog);
	
	if (m_iWorkGroup == DEF_FRONT_GROUP)
	{
		strTabCheckLog.Format(_T("Front TabNo: %d TabIC Length:[%0.3f] Panel Length:[%0.3f] TabCheck X:[%0.3f] TabCheck Y:[%0.3f]"),iTabNo + 1,tmpTabLength,tmpCellLength,(tmpTabLength-tmpCellLength),(dGantryPosDiff - dGantryCurPosdiff) );		
	}
	else
		strTabCheckLog.Format(_T("Rear TabNo: %d TabIC Length:[%0.3f] Panel Length:[%0.3f] TabCheck X:[%0.3f] TabCheck Y:[%0.3f]"),iTabNo + 1,tmpTabLength,tmpCellLength,(tmpTabLength-tmpCellLength),(dGantryPosDiff - dGantryCurPosdiff) );

	//20130612 SJ_HJG
	MOlbSystemPre.GetTrsAutoManagerComponent()->SetMeasureProcessData(eWorkSide,iTabNo,fabs(tmpTabLength-tmpCellLength),fabs(dGantryPosDiff - dGantryCurPosdiff));
	
	if(fabs(dGantryPosDiff - dGantryCurPosdiff) > dGantryCheck)
	{
		//324220 = Inspection Vision Mark 인식 실패. [InapecCam 1 : Tab Check Y 결과값이 Limit값을 초과함.]
		
		m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
		return generateErrorCode(324220 + iCamNo - DEF_INSPECTION_CAMERA_1);
	}
	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	WriteTabCheckLog(strTabCheckLog);

	//Error 처리
	if(fabs(tmpTabLength-tmpCellLength)>dTabXCheck)
	{
		//324200 = Inspection Vision Mark 인식 실패. [InapecCam 1 : Tab Check X 결과값이 Limit값을 초과함.]
		
		m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
		return generateErrorCode(324200 + iCamNo - DEF_INSPECTION_CAMERA_1);
	}
	//_____________________________
	

	// xyAlignOffset->dX 가 Mounter 방향 Align Offset 으로 적용된다.
	MPos_XY xyAlignOffset;
		xyAlignOffset.dX = dResultX;
		xyAlignOffset.dY = dResultY;


	if (iTargetStatus == DEF_BEFORE_MOUNT)
	{
//		MPos_XY* pxyInspectionOffset = m_plnkProcessData->GetInspectionOffsetStoragePointer(eWorkSide, iTabNo);
//		pxyInspectionOffset->dX = xyAlignOffset.dX;
//		pxyInspectionOffset->dY = xyAlignOffset.dY;
		//임시 20100823 sj_ysb
		/*MPos_XYT InspectionOffset(0.0, 0.0, 0.0);
		InspectionOffset.dX = xyAlignOffset.dX;
		InspectionOffset.dY = xyAlignOffset.dY;
		m_plnkWorkSchedule->SetInspectionOffset(iTabNo, InspectionOffset);*/
		MPos_XYT InspectionOffset(0.0, 0.0, 0.0);
//@		InspectionOffset.dX =  xyAlignOffset.dY;	//확인요
//@		InspectionOffset.dY =  xyAlignOffset.dX;
		InspectionOffset.dX =  xyAlignOffset.dX;	//확인요
		InspectionOffset.dY =  xyAlignOffset.dY;
		m_plnkWorkSchedule[iScheduleNo]->SetInspectionOffset(iTabNo, InspectionOffset);
//		if (m_iInstanceNo == 3)
//		{
//			TRACE(_T("=====InspectionOffsetX:%.3lf, InspectionOffsetY:%.3lf=====\n"), InspectionOffset.dX, InspectionOffset.dY);
//		}


/*
		//@130123.KMS_____________________
		double dPanelTabMark = rgRxy[1].dX - rgRxy[0].dX;
		double dICTabMark = rgRxy[3].dX - rgRxy[2].dX;
		if(fabs(dPanelTabMark - dPanelTabMark) > m_plnkPreBonderData->m_dInspectionMarkXPermission)
		{
		
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
			return generateErrorCode(324115);
		}
*/
		double dPanelMarkDist = rgRxy[1].dX - rgRxy[0].dX;
		double dTabMarkDist = rgRxy[3].dX - rgRxy[2].dX;

//		TRACE(_T("P0:%.3lf, P1:%.3lf, T0:%.3lf, T1:%.3lf\n"), rgRxy[0].dX, rgRxy[1].dX, rgRxy[2].dX, rgRxy[3].dX);
//		TRACE(_T("Panel : %.3lf, Tab : %.3lf, Panel-Tab : %.3lf\n"), dPanelMarkDist, dTabMarkDist, dPanelMarkDist - dTabMarkDist);

		if(fabs(dPanelMarkDist - dTabMarkDist) > m_plnkSystemData->m_dPanelTabDist)
		{
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
			return generateErrorCode(324115);
		}
/*
		double dPanelMarkYDiff = rgRxy[1].dY - rgRxy[0].dY;
		double dICMarkYDiff = rgRxy[3].dY - rgRxy[2].dY;
		if (fabs(dPanelMarkYDiff - dICMarkYDiff) > m_plnkPreBonderData->m_dInspectionMarkYPermission)	//@Theta틀어짐
		{
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
			return generateErrorCode(324116);
		}
*/
		//@_______________________________
	}

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	//20111020 by kss
	/////////////////////////////////////////////////////////
	if (TRUE == m_plnkSystemData->m_bLogLevelNormal)
	{
		CString strLog;
		strLog.Format(_T("GlassId %s,IC%d,ws(%d),cam(%d),tab(%02d),p1_v(,%.3f,%.3f,),p2_v(,%.3f,%.3f,),t1_v(,%.3f,%.3f,),t2_v(,%.3f,%.3f,),p1(,%.4f,%.4f,),p2(,%.4f,%.4f,),t1(,%.4f,%.4f,),t2(,%.4f,%.4f,),t_c(,%.4f,%.4f,),p_c(,%.4f,%.4f,),offset(,%.4f,%.4f,)"),
			strGlassData, m_iInstanceNo, eWorkSide, iCamNo, iTabNo,
			rgVxy[0].dX, rgVxy[0].dY, rgVxy[1].dX, rgVxy[1].dY, rgVxy[2].dX, rgVxy[2].dY, rgVxy[3].dX, rgVxy[3].dY,
			rgRxy[0].dX, rgRxy[0].dY, rgRxy[1].dX, rgRxy[1].dY, rgRxy[2].dX, rgRxy[2].dY, rgRxy[3].dX, rgRxy[3].dY,
			dTabCenterX, dTabCenterY, dPanelCenterX, dPanelCenterY, xyAlignOffset.dX, xyAlignOffset.dY);
		
		if (iTargetStatus == DEF_BEFORE_MOUNT)
		{
			m_plnkVision->WriteLog(_T("..\\Log\\inspection_before_log.csv"), (LPCTSTR)strLog);
		}
		else // if (iTargetStatus == DEF_AFTER_MOUNT)
		{
			m_plnkVision->WriteLog(_T("..\\Log\\inspection_after_log.csv"), (LPCTSTR)strLog);
		}
	}
	/////////////////////////////////////////////////////////

	m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH

	return ERR_CTRL_INSPECTIONCAMERA_SUCCESS;
}

int MCtrlInspectionCamera::getErrorCode(int iCamNo, int iVisionErrorCode, int iTargetMarkType)
{
	if (104017 == iVisionErrorCode / 1000)	// 104017 = Pattern Matching 인식 실패 [Mark 인식 개수가 1 개가 아님.]
	{
		if (0/*Panel Mark Type*/ == iTargetMarkType)
		{
			/*
			switch (iCamNo)
			{
			case DEF_F_INSPECTION_CAMERA_1:
				// 324001 = Inspection Vision Mark 인식 실패. [Camera 5 : Panel Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324001);
				
			case DEF_F_INSPECTION_CAMERA_2:
				// 324002 = Inspection Vision Mark 인식 실패. [Camera 6 : Panel Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324002);
				
			case DEF_F_INSPECTION_CAMERA_3:
				// 324003 = Inspection Vision Mark 인식 실패. [Camera 7 : Panel Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324003);
				
			case DEF_F_INSPECTION_CAMERA_4:
				// 324004 = Inspection Vision Mark 인식 실패. [Camera 8 : Panel Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324004);
				
			case DEF_R_INSPECTION_CAMERA_1:
				// 324005 = Inspection Vision Mark 인식 실패. [Camera 10 : Panel Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324005);
				
			case DEF_R_INSPECTION_CAMERA_2:
				// 324006 = Inspection Vision Mark 인식 실패. [Camera 11 : Panel Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324006);
				
			case DEF_R_INSPECTION_CAMERA_3:
				// 324007 = Inspection Vision Mark 인식 실패. [Camera 12 : Panel Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324007);

			default:
				break;
			}
			*/
			return generateErrorCode(324030 + iCamNo - DEF_INSPECTION_CAMERA_1);
		}
		else //if (1/*TabIC Mark Type*/ == iTargetMarkType)
		{
			/*
			switch (iCamNo)
			{
			case DEF_F_INSPECTION_CAMERA_1:
				// 324008 = Inspection Vision Mark 인식 실패. [Camera 5 : TabIC Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324008);
				
			case DEF_F_INSPECTION_CAMERA_2:
				// 324009 = Inspection Vision Mark 인식 실패. [Camera 6 : TabIC Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324009);
				
			case DEF_F_INSPECTION_CAMERA_3:
				// 324010 = Inspection Vision Mark 인식 실패. [Camera 7 : TabIC Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324010);
				
			case DEF_F_INSPECTION_CAMERA_4:
				// 324011 = Inspection Vision Mark 인식 실패. [Camera 8 : TabIC Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324011);
				
			case DEF_R_INSPECTION_CAMERA_1:
				// 324012 = Inspection Vision Mark 인식 실패. [Camera 10 : TabIC Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324012);
				
			case DEF_R_INSPECTION_CAMERA_2:
				// 324013 = Inspection Vision Mark 인식 실패. [Camera 11 : TabIC Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324013);
				
			case DEF_R_INSPECTION_CAMERA_3:
				// 324014 = Inspection Vision Mark 인식 실패. [Camera 12 : TabIC Mark 인식 개수가 1 개가 아님.]
				return generateErrorCode(324014);
			default:
				break;
			}
			*/
			return generateErrorCode(324050 + iCamNo - DEF_INSPECTION_CAMERA_1);
		}
	}
	else if (104018 == iVisionErrorCode / 1000)	// 104018 = Pattern Matching 인식률 낮음
	{
		if (0/*Panel Mark Type*/ == iTargetMarkType)
		{
			/*
			switch (iCamNo)
			{
			case DEF_F_INSPECTION_CAMERA_1:
				// 324016 = Inspection Vision Mark 인식 실패. [Camera 5 : Panel Mark 인식률 낮음.]
				return generateErrorCode(324016);
				
			case DEF_F_INSPECTION_CAMERA_2:
				// 324017 = Inspection Vision Mark 인식 실패. [Camera 6 : Panel Mark 인식률 낮음.]
				return generateErrorCode(324017);
				
			case DEF_F_INSPECTION_CAMERA_3:
				// 324018 = Inspection Vision Mark 인식 실패. [Camera 7 : Panel Mark 인식률 낮음.]
				return generateErrorCode(324018);
				
			case DEF_F_INSPECTION_CAMERA_4:
				// 324019 = Inspection Vision Mark 인식 실패. [Camera 8 : Panel Mark 인식률 낮음.]
				return generateErrorCode(324019);
				
			case DEF_R_INSPECTION_CAMERA_1:
				// 324020 = Inspection Vision Mark 인식 실패. [Camera 10 : Panel Mark 인식률 낮음.]
				return generateErrorCode(324020);
				
			case DEF_R_INSPECTION_CAMERA_2:
				// 324021 = Inspection Vision Mark 인식 실패. [Camera 11 : Panel Mark 인식률 낮음.]
				return generateErrorCode(324021);
				
			case DEF_R_INSPECTION_CAMERA_3:
				// 324022 = Inspection Vision Mark 인식 실패. [Camera 12 : Panel Mark 인식률 낮음.]
				return generateErrorCode(324022);
			default:
				break;
			}
			*/
			return generateErrorCode(324070 + iCamNo - DEF_INSPECTION_CAMERA_1);
		}
		else //if (1/*TabIC Mark Type*/ == iTargetMarkType)
		{
			/*
			switch (iCamNo)
			{
			case DEF_F_INSPECTION_CAMERA_1:
				// 324023 = Inspection Vision Mark 인식 실패. [Camera 5 : TabIC Mark 인식률 낮음.]
				return generateErrorCode(324023);
				
			case DEF_F_INSPECTION_CAMERA_2:
				// 324024 = Inspection Vision Mark 인식 실패. [Camera 6 : TabIC Mark 인식률 낮음.]
				return generateErrorCode(324024);
				
			case DEF_F_INSPECTION_CAMERA_3:
				// 324025 = Inspection Vision Mark 인식 실패. [Camera 7 : TabIC Mark 인식률 낮음.]
				return generateErrorCode(324025);
				
			case DEF_F_INSPECTION_CAMERA_4:
				// 324026 = Inspection Vision Mark 인식 실패. [Camera 8 : TabIC Mark 인식률 낮음.]
				return generateErrorCode(324026);
				
			case DEF_R_INSPECTION_CAMERA_1:
				// 324027 = Inspection Vision Mark 인식 실패. [Camera 10 : TabIC Mark 인식률 낮음.]
				return generateErrorCode(324027);
				
			case DEF_R_INSPECTION_CAMERA_2:
				// 324028 = Inspection Vision Mark 인식 실패. [Camera 11 : TabIC Mark 인식률 낮음.]
				return generateErrorCode(324028);
				
			case DEF_R_INSPECTION_CAMERA_3:
				// 324029 = Inspection Vision Mark 인식 실패. [Camera 12 : TabIC Mark 인식률 낮음.]
				return generateErrorCode(324029);
			default:
				break;
			}
			*/
			return generateErrorCode(324090 + iCamNo - DEF_INSPECTION_CAMERA_1);
		}
	}
	//170612 JSh.s
	else if (iTargetMarkType == 2/*Bolb Type*/)
	{
		if(iVisionErrorCode != 0)
		{
			return generateErrorCode(324120 + iCamNo - DEF_INSPECTION_CAMERA_1);
		}
	}
	//170612 JSh.e
	return iVisionErrorCode;
}

BOOL MCtrlInspectionCamera::WriteTabCheckLog(CString strMsg)
{
	//KJS NSMC System 생성이 완료되기 전에는 로그 작성하지 않음

	//20131029 임시...
	return FALSE;

	FILE* fsource;
	//CFileFind fn;
	MFileUtil FileUtil;
	CString fname;

	CTime tmCurr = CTime::GetCurrentTime();
 
	strMsg = tmCurr.Format(_T("[%m/%d-%H:%M:%S]")) + strMsg;
	fname.Format(_T("..\\Log\\TabCheckLog\\")); 
	fname = fname + tmCurr.Format(_T("TabCheck_%m%d-")) + _T(".csv");

	FileUtil.MakeDir(fname,FALSE); // 로깅 디렉토리가 존재하지 않을 경우 Directory를 만든다.

	//LKH_20090130 : Log Unicode Writing
	if ((fsource = _wfopen(fname, _T("ab+,ccs=UNICODE"))) == NULL)
	{
		//Log 파일을 만들지 못 했습니다.
//		MyMessageBox(MY_MESSAGE_BOX, 64004, _T("File Write Error!"), M_ICONERROR);

		return FALSE;
	}

	//현재 발생 로그 내용 기록
	fwprintf(fsource, _T("%s\r\n"), strMsg.GetBuffer());

	fclose(fsource);

	return TRUE;
}
//170926 JSh.s
int MCtrlInspectionCamera::WriteTabImageLog(int ScheduleNo, int TabNo)
{
	//170925 JSH.s
	if(TabNo == -1)
		return ERR_CTRL_INSPECTIONCAMERA_SUCCESS;
	//170925 JSH.e
	
//170926 JSh.s
	CString strPath;
	if(m_iWorkGroup == DEF_FRONT_GROUP)
		strPath.Format(_T("..\\LogImage\\F_MountAfterImage"));
	else
		strPath.Format(_T("..\\LogImage\\R_MountAfterImage"));
//170926 JSh.e

	m_plnkVision->DeleteOldImageFiles((LPCTSTR)strPath);

	const int iCamNo = m_plnkCamCarrier->GetOwnedCameraNo1();
	const int iPairCamNo = m_plnkCamCarrier->GetOwnedCameraNo2();

	int iXuTabQtyHalf = m_plnkTabData->m_iXuTabQty / 2;

	CString strInfo;
	if(ScheduleNo != DEF_WORK_SCHEDULE_SECOND)
		strInfo.Format(_T("Tab%d LogImage 1st Cam"), TabNo + 1);
	else
		strInfo.Format(_T("Tab%d LogImage 1st Cam"), TabNo + 1 + iXuTabQtyHalf);

	m_plnkVision->LockCamera(iCamNo);	
	m_plnkVision->ClearOverlay(iCamNo);

	int iResult = m_plnkVision->Grab(iCamNo);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iCamNo);
		return iResult;
	}
	m_plnkVision->SaveImage(iCamNo, (LPCTSTR)strPath, (LPCTSTR)strInfo);
	m_plnkVision->UnlockCamera(iCamNo);
	
	if(ScheduleNo != DEF_WORK_SCHEDULE_SECOND)
		strInfo.Format(_T("Tab%d LogImage 2nd Cam"), TabNo + 1);
	else
		strInfo.Format(_T("Tab%d LogImage 2nd Cam"), TabNo + 1 + iXuTabQtyHalf);

	m_plnkVision->LockCamera(iPairCamNo);	
	m_plnkVision->ClearOverlay(iPairCamNo);

	iResult = m_plnkVision->Grab(iPairCamNo);
	if (iResult)
	{
		m_plnkVision->UnlockCamera(iPairCamNo);
		return iResult;
	}
	m_plnkVision->SaveImage(iPairCamNo, (LPCTSTR)strPath, (LPCTSTR)strInfo);
	m_plnkVision->UnlockCamera(iPairCamNo);
	return ERR_CTRL_INSPECTIONCAMERA_SUCCESS;
}
//170926 JSh.e

//170922 JSh.s
//170612 JSh add
//Tab No는 로그용...
int MCtrlInspectionCamera::InspectTabAttached(int iTabNo)
{
	int iResult = ERR_CTRL_INSPECTIONCAMERA_SUCCESS;
	
	//170616 JSH Skip 처리
	if(iTabNo == -1)
		return ERR_CTRL_INSPECTIONCAMERA_SUCCESS;

	int riBlobNo;
	int iCamNo;
	
	riBlobNo = DEF_BLOB_TAB_ATTACHED1; //DEF_RESERVED13;
	iCamNo = m_plnkCamCarrier->GetOwnedCameraNo1();
	
	m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOn(); //171002 JSH
	Sleep(100);//171002 JSH
	
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		m_plnkVision->LockCamera(iCamNo);
		
		m_plnkVision->ClearOverlay(iCamNo);
		m_plnkVision->Grab(iCamNo);
		
		iResult = m_plnkVision->RecognitionBLOBPattern(iCamNo, riBlobNo);
		if (iResult)
		{
			Sleep(500);
			m_plnkVision->ClearOverlay(iCamNo);
			m_plnkVision->Grab(iCamNo);
			
			iResult = m_plnkVision->RecognitionBLOBPattern(iCamNo, riBlobNo);
			if(iResult)
			{
				if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPStatus() == RUN)
				{
					CString strImageDir, strImageFName;
					strImageFName.Format(_T("Tab%2dBolb[Cam%d]"), iTabNo,iCamNo+1);
					//170925 JSh.s
// 					if(m_iWorkGroup == DEF_FRONT_GROUP)
// 						strImageDir.Format(_T("..\\LogImage\\F_InspectionTabAttached"));
// 					else
// 						strImageDir.Format(_T("..\\LogImage\\R_InspectionTabAttached"));
// 					m_plnkVision->SaveImage(iCamNo, (LPCTSTR)strImageDir, (LPCTSTR)strImageFName);
					m_plnkVision->SaveImage(iCamNo, (LPCTSTR)strImageFName);
					//170925 JSh.e
				}
				
				m_plnkVision->UnlockCamera(iCamNo);

				m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
				return getErrorCode(iCamNo, iResult, 2/*Bolb Type*/);
			}
		}
		
		m_plnkVision->UnlockCamera(iCamNo);
	}

	riBlobNo = DEF_BLOB_TAB_ATTACHED2; //DEF_RESERVED14;
	iCamNo = m_plnkCamCarrier->GetOwnedCameraNo2();
	if (m_plnkSystemData->m_eRunMode != DRY_RUN_MODE)
	{
		m_plnkVision->LockCamera(iCamNo);
		
		m_plnkVision->ClearOverlay(iCamNo);
		m_plnkVision->Grab(iCamNo);
		
		iResult = m_plnkVision->RecognitionBLOBPattern(iCamNo, riBlobNo);
		if (iResult)
		{
			Sleep(500);
			m_plnkVision->ClearOverlay(iCamNo);
			m_plnkVision->Grab(iCamNo);
			
			iResult = m_plnkVision->RecognitionBLOBPattern(iCamNo, riBlobNo);
			if(iResult)
			{
				if (MOlbSystemPre.GetTrsAutoManagerComponent()->GetOPMode() == AUTORUN_MODE)
				{
					CString strImageDir, strImageFName;
					strImageFName.Format(_T("Tab%2dBolb[Cam%d]"), iTabNo,iCamNo+1);
					//170925 JSh.s
// 					if(m_iWorkGroup == DEF_FRONT_GROUP)
// 						strImageDir.Format(_T("..\\LogImage\\F_InspectionTabAttached"));
// 					else
// 						strImageDir.Format(_T("..\\LogImage\\R_InspectionTabAttached"));
// 					m_plnkVision->SaveImage(iCamNo, (LPCTSTR)strImageDir, (LPCTSTR)strImageFName);
					m_plnkVision->SaveImage(iCamNo, (LPCTSTR)strImageFName);
					//170925 JSh.e
				}
				
				m_plnkVision->UnlockCamera(iCamNo);
				m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
				return getErrorCode(iCamNo, iResult, 2/*Bolb Type*/);
			}
		}
		
		m_plnkVision->UnlockCamera(iCamNo);
	}

	m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
	return ERR_CTRL_INSPECTIONCAMERA_SUCCESS;
}
//170922 JSh.e

//____________
