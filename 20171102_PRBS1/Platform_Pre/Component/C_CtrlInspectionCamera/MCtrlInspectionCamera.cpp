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

//20120215 SJ_HJG �߰�
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

	Sleep(100); //170926 JSh ǰ��Ȯ��

	//������������������������������������������������������������
	// parameter ��ȿ�� ����, ��� Data �غ�(Mark ��ȣ etc.)
//	int iMaxTabNo = m_plnkProcessData->GetMaxTabQty(eWorkSide);

	/*/130106.KKY.Del(�ӽ�)_______
	int iMaxTabNo = m_plnkWorkSchedule[iScheduleNo]->GetMaxTabQty(WORKING_SIDE_X_UP);	//110125.������
	if (iTabNo < 0 || iTabNo >= iMaxTabNo)
	{
		m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
		SetErrorLevel(_T("INSPECTIONCAMERA get Tab Align Offset"),1, __FILE__, __LINE__);
		// 324015 = TabIC ��ȣ ����. (Inspection ���� ���� ��)
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

	//������������������������������������������������������������
	// Unit ���� Ȯ���� ����!

	// Panel Aligner ����
	// 1. Panel ����
	// 2. Panel Align �Ϸ�
	// 3. Inspection ��ġ or Mount ��ġ

	// TabMounter ����
	// 1. TabIC ����
	// 2. Mount ��ġ (Mounter ��)
	// 3. Mounter ���� (DownReady)
	// 4. Backup ��ġ (����, �ϰ�)

	// CameraCarrier ����
	// 1. CameraCarrier ��ġ


	//������������������������������������������������������������
	// Vision �ν�
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

	// Panel Tab Lead Mark �ν�
	int rgiCamNo[2] = { iCamNo, iCamNo };
	if (-1 != iPairCamNo)
		rgiCamNo[1] = iPairCamNo;
	
	for (int i = 0; i < 2; i++)
	{
		iResult = m_plnkVision->RecognitionPatternMark(rgiCamNo[i], rgiModel[i]);

		if (iResult)	// Mark �ν� Retry ����.
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
		if (iResult)	// ���� ���� Off�� Mark �ν� Retry ����.
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
			// Error Image ����.
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

			// Ȯ�� �ʿ�.
			//if (TRUE == m_plnkSystemData->m_bUsePanelLeadMarkPosPresumption)
			if (FALSE)
			{
				// 1�� �� Mark �ν� ����.
				
			}
			else
			{
				//ErrorImage MainView�� �����ֱ�_____
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
	MPos_XY rgVxy[4];	// Vision �ν� ��ǥ �ӽ� ���� ����
	int iPrizmSide = 0;	// OLB ���� ��ǥ�� �� �������� ���� ���� 0, ū ���� 1 �̴�.
	int iCalibUnitNo = 0;
#ifdef DEF_SOURCE_SYSTEM
	iCalibUnitNo = 0;
#else
	iCalibUnitNo = 0;//110125.Ȯ�ο�
#endif
	if (iTargetStatus == DEF_AFTER_MOUNT)
	{
		RECT sArea;
		for (int i = 0; i < 2; i++)
		{
			// �� Unit ���� �����ϰ� OLB ���� ��ǥ�� �������� Vision �ν� ��ǥ�� ��´�.		
			// ����, Unit �� ���� dVx �� Mounter �� �ݿ��Ǿ�� �� ���� �ְ�, Aligner �� �ݿ��Ǿ�� �� ���� �ִ�.
			rgVxy[i].dX = m_plnkVision->GetSearchResultX(rgiCamNo[i], rgiModel[i]);
			rgVxy[i].dY = m_plnkVision->GetSearchResultY(rgiCamNo[i], rgiModel[i]);
			if (-1 == iPairCamNo)
				iPrizmSide = i % 2;		//@�̻��

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

	// TabIC Lead Mark �ν�
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
		if (iResult)	// ���� ���� Off�� Mark �ν� Retry ����.
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
			// Error Image ����.
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

			//ErrorImage MainView�� �����ֱ�_____
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

		// �� Unit ���� �����ϰ� OLB ���� ��ǥ�� �������� Vision �ν� ��ǥ�� ��´�.		
		// ����, Unit �� ���� dVx �� Mounter �� �ݿ��Ǿ�� �� ���� �ְ�, Aligner �� �ݿ��Ǿ�� �� ���� �ִ�.
		rgVxy[i].dX = m_plnkVision->GetSearchResultX(rgiCamNo[i % 2], rgiModel[i]);
		rgVxy[i].dY = m_plnkVision->GetSearchResultY(rgiCamNo[i % 2], rgiModel[i]);
//		if (-1 == iPairCamNo)
//			iPrizmSide = i % 2;
#ifdef DEF_SOURCE_SYSTEM
		iPrizmSide = 0;		//@�̻��
#else
		iPrizmSide = 1;		//@�̻��
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
			= m_plnkProcessData->GetOldInspectionPanelMarkVisionPosStoragePointer(eWorkSide, iTabNo);	//???.���ʿ��ұ�???

		for (int i = 0; i < 2; i++)
			rgxyOldPanelMarkVisionPos[i] = rgVxy[i];
	}
	*/
	//������������������������������������������������������������
	// Vision �ν� ��� ��� (Mark �� Center Point, AlignOffset ���)
	
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

	// xyAlignOffset->dX �� Mounter ���� Align Offset ���� ����ȴ�.
//	MPos_XY xyAlignOffset;
//	if (SYSTEM_TYPE_SOURCE == m_plnkSystemData->m_eSystemType)

//@#ifdef DEF_SOURCE_SYSTEM
//@	{
//@		xyAlignOffset.dX = dResultY;	// Source Inspection Camera Calibration �� Panel Aligner �� �����ϹǷ�, Y ���� Mounter �࿡ �ݿ��ؾ� �Ѵ�.
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
	// Tab lead mark ���� ����
	//  TabIC Mark �� Panel Mark�� Mark���� �������� ���Ͽ� �������� �ʰ��ϸ� ���� ó��..
	double dTabXCheck = m_plnkSystemData->m_dTabXCheck;
	double tmpCellLength,tmpTabLength;
	double cur_width;
	CString strTabCheckLog;
	MPos_XY tab_mark_l, tab_mark_r, cell_mark_l, cell_mark_r;

	if(eWorkSide == WORKING_SIDE_X_UP) //Xu
		cur_width = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Xtab;
	else
		cur_width = MOlbSystemPre.GetPreBonderData()->m_dLead_Width_Ytab;

	// Tab IC ��ǥ�� ���
#ifdef DEF_GATE_SYSTEM
	tab_mark_l.dX = rgRxy[2].dX;
	tab_mark_l.dY = rgRxy[2].dY;
	tab_mark_r.dX = rgRxy[3].dX + cur_width;
	tab_mark_r.dY = rgRxy[3].dY;

	// Cell ��ǥ�� ���
	cell_mark_l.dX = rgRxy[0].dX;
	cell_mark_l.dY = rgRxy[0].dY;
	cell_mark_r.dX = rgRxy[1].dX + cur_width;
	cell_mark_r.dY = rgRxy[1].dY;
#else
	tab_mark_l.dX = rgRxy[2].dY;
	tab_mark_l.dY = rgRxy[2].dX;
	tab_mark_r.dX = rgRxy[3].dY + cur_width;
	tab_mark_r.dY = rgRxy[3].dX;

	// Cell ��ǥ�� ���
	cell_mark_l.dX = rgRxy[0].dY;
	cell_mark_l.dY = rgRxy[0].dX;
	cell_mark_r.dX = rgRxy[1].dY + cur_width;
	cell_mark_r.dY = rgRxy[1].dX;

#endif

	tmpTabLength = _hypot(tab_mark_r.dX-tab_mark_l.dX, tab_mark_r.dY-tab_mark_l.dY);
	tmpCellLength = _hypot(cell_mark_r.dX-cell_mark_l.dX, cell_mark_r.dY-cell_mark_l.dY);

	//Gantry Mount Pos ��ġ Check...
	//1).Gantry Insoection Pos�� Mount Pos ���� ���� ���
	//2).���� Gantry Insoection������ TabIC Mark�� Panel Mark�� Y ��ġ ���̰��� �޾ƿͼ�
	// 1)�� 2) �� ���� �� �Ͽ� �������� �ʰ� �ϸ�Erroró��.

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
	//Gantry Insoection Pos�� Mount Pos ���� ���� ��� 2
	dGantryPosDiff = fabs( dGantryInspectionPos - dGantryMountPos);

	//TabIC Mark�� Panel Mark�� Y��ġ ��
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
		//324220 = Inspection Vision Mark �ν� ����. [InapecCam 1 : Tab Check Y ������� Limit���� �ʰ���.]
		
		m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
		return generateErrorCode(324220 + iCamNo - DEF_INSPECTION_CAMERA_1);
	}
	//������������������������������������������������������������
	WriteTabCheckLog(strTabCheckLog);

	//Error ó��
	if(fabs(tmpTabLength-tmpCellLength)>dTabXCheck)
	{
		//324200 = Inspection Vision Mark �ν� ����. [InapecCam 1 : Tab Check X ������� Limit���� �ʰ���.]
		
		m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
		return generateErrorCode(324200 + iCamNo - DEF_INSPECTION_CAMERA_1);
	}
	//_____________________________
	

	// xyAlignOffset->dX �� Mounter ���� Align Offset ���� ����ȴ�.
	MPos_XY xyAlignOffset;
		xyAlignOffset.dX = dResultX;
		xyAlignOffset.dY = dResultY;


	if (iTargetStatus == DEF_BEFORE_MOUNT)
	{
//		MPos_XY* pxyInspectionOffset = m_plnkProcessData->GetInspectionOffsetStoragePointer(eWorkSide, iTabNo);
//		pxyInspectionOffset->dX = xyAlignOffset.dX;
//		pxyInspectionOffset->dY = xyAlignOffset.dY;
		//�ӽ� 20100823 sj_ysb
		/*MPos_XYT InspectionOffset(0.0, 0.0, 0.0);
		InspectionOffset.dX = xyAlignOffset.dX;
		InspectionOffset.dY = xyAlignOffset.dY;
		m_plnkWorkSchedule->SetInspectionOffset(iTabNo, InspectionOffset);*/
		MPos_XYT InspectionOffset(0.0, 0.0, 0.0);
//@		InspectionOffset.dX =  xyAlignOffset.dY;	//Ȯ�ο�
//@		InspectionOffset.dY =  xyAlignOffset.dX;
		InspectionOffset.dX =  xyAlignOffset.dX;	//Ȯ�ο�
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
		if (fabs(dPanelMarkYDiff - dICMarkYDiff) > m_plnkPreBonderData->m_dInspectionMarkYPermission)	//@ThetaƲ����
		{
			m_plnkLighting[s_rgLightIdForCam[iCamNo].x]->LightOff(); //171002 JSH
			return generateErrorCode(324116);
		}
*/
		//@_______________________________
	}

	//������������������������������������������������������������
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
	if (104017 == iVisionErrorCode / 1000)	// 104017 = Pattern Matching �ν� ���� [Mark �ν� ������ 1 ���� �ƴ�.]
	{
		if (0/*Panel Mark Type*/ == iTargetMarkType)
		{
			/*
			switch (iCamNo)
			{
			case DEF_F_INSPECTION_CAMERA_1:
				// 324001 = Inspection Vision Mark �ν� ����. [Camera 5 : Panel Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324001);
				
			case DEF_F_INSPECTION_CAMERA_2:
				// 324002 = Inspection Vision Mark �ν� ����. [Camera 6 : Panel Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324002);
				
			case DEF_F_INSPECTION_CAMERA_3:
				// 324003 = Inspection Vision Mark �ν� ����. [Camera 7 : Panel Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324003);
				
			case DEF_F_INSPECTION_CAMERA_4:
				// 324004 = Inspection Vision Mark �ν� ����. [Camera 8 : Panel Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324004);
				
			case DEF_R_INSPECTION_CAMERA_1:
				// 324005 = Inspection Vision Mark �ν� ����. [Camera 10 : Panel Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324005);
				
			case DEF_R_INSPECTION_CAMERA_2:
				// 324006 = Inspection Vision Mark �ν� ����. [Camera 11 : Panel Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324006);
				
			case DEF_R_INSPECTION_CAMERA_3:
				// 324007 = Inspection Vision Mark �ν� ����. [Camera 12 : Panel Mark �ν� ������ 1 ���� �ƴ�.]
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
				// 324008 = Inspection Vision Mark �ν� ����. [Camera 5 : TabIC Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324008);
				
			case DEF_F_INSPECTION_CAMERA_2:
				// 324009 = Inspection Vision Mark �ν� ����. [Camera 6 : TabIC Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324009);
				
			case DEF_F_INSPECTION_CAMERA_3:
				// 324010 = Inspection Vision Mark �ν� ����. [Camera 7 : TabIC Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324010);
				
			case DEF_F_INSPECTION_CAMERA_4:
				// 324011 = Inspection Vision Mark �ν� ����. [Camera 8 : TabIC Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324011);
				
			case DEF_R_INSPECTION_CAMERA_1:
				// 324012 = Inspection Vision Mark �ν� ����. [Camera 10 : TabIC Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324012);
				
			case DEF_R_INSPECTION_CAMERA_2:
				// 324013 = Inspection Vision Mark �ν� ����. [Camera 11 : TabIC Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324013);
				
			case DEF_R_INSPECTION_CAMERA_3:
				// 324014 = Inspection Vision Mark �ν� ����. [Camera 12 : TabIC Mark �ν� ������ 1 ���� �ƴ�.]
				return generateErrorCode(324014);
			default:
				break;
			}
			*/
			return generateErrorCode(324050 + iCamNo - DEF_INSPECTION_CAMERA_1);
		}
	}
	else if (104018 == iVisionErrorCode / 1000)	// 104018 = Pattern Matching �νķ� ����
	{
		if (0/*Panel Mark Type*/ == iTargetMarkType)
		{
			/*
			switch (iCamNo)
			{
			case DEF_F_INSPECTION_CAMERA_1:
				// 324016 = Inspection Vision Mark �ν� ����. [Camera 5 : Panel Mark �νķ� ����.]
				return generateErrorCode(324016);
				
			case DEF_F_INSPECTION_CAMERA_2:
				// 324017 = Inspection Vision Mark �ν� ����. [Camera 6 : Panel Mark �νķ� ����.]
				return generateErrorCode(324017);
				
			case DEF_F_INSPECTION_CAMERA_3:
				// 324018 = Inspection Vision Mark �ν� ����. [Camera 7 : Panel Mark �νķ� ����.]
				return generateErrorCode(324018);
				
			case DEF_F_INSPECTION_CAMERA_4:
				// 324019 = Inspection Vision Mark �ν� ����. [Camera 8 : Panel Mark �νķ� ����.]
				return generateErrorCode(324019);
				
			case DEF_R_INSPECTION_CAMERA_1:
				// 324020 = Inspection Vision Mark �ν� ����. [Camera 10 : Panel Mark �νķ� ����.]
				return generateErrorCode(324020);
				
			case DEF_R_INSPECTION_CAMERA_2:
				// 324021 = Inspection Vision Mark �ν� ����. [Camera 11 : Panel Mark �νķ� ����.]
				return generateErrorCode(324021);
				
			case DEF_R_INSPECTION_CAMERA_3:
				// 324022 = Inspection Vision Mark �ν� ����. [Camera 12 : Panel Mark �νķ� ����.]
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
				// 324023 = Inspection Vision Mark �ν� ����. [Camera 5 : TabIC Mark �νķ� ����.]
				return generateErrorCode(324023);
				
			case DEF_F_INSPECTION_CAMERA_2:
				// 324024 = Inspection Vision Mark �ν� ����. [Camera 6 : TabIC Mark �νķ� ����.]
				return generateErrorCode(324024);
				
			case DEF_F_INSPECTION_CAMERA_3:
				// 324025 = Inspection Vision Mark �ν� ����. [Camera 7 : TabIC Mark �νķ� ����.]
				return generateErrorCode(324025);
				
			case DEF_F_INSPECTION_CAMERA_4:
				// 324026 = Inspection Vision Mark �ν� ����. [Camera 8 : TabIC Mark �νķ� ����.]
				return generateErrorCode(324026);
				
			case DEF_R_INSPECTION_CAMERA_1:
				// 324027 = Inspection Vision Mark �ν� ����. [Camera 10 : TabIC Mark �νķ� ����.]
				return generateErrorCode(324027);
				
			case DEF_R_INSPECTION_CAMERA_2:
				// 324028 = Inspection Vision Mark �ν� ����. [Camera 11 : TabIC Mark �νķ� ����.]
				return generateErrorCode(324028);
				
			case DEF_R_INSPECTION_CAMERA_3:
				// 324029 = Inspection Vision Mark �ν� ����. [Camera 12 : TabIC Mark �νķ� ����.]
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
	//KJS NSMC System ������ �Ϸ�Ǳ� ������ �α� �ۼ����� ����

	//20131029 �ӽ�...
	return FALSE;

	FILE* fsource;
	//CFileFind fn;
	MFileUtil FileUtil;
	CString fname;

	CTime tmCurr = CTime::GetCurrentTime();
 
	strMsg = tmCurr.Format(_T("[%m/%d-%H:%M:%S]")) + strMsg;
	fname.Format(_T("..\\Log\\TabCheckLog\\")); 
	fname = fname + tmCurr.Format(_T("TabCheck_%m%d-")) + _T(".csv");

	FileUtil.MakeDir(fname,FALSE); // �α� ���丮�� �������� ���� ��� Directory�� �����.

	//LKH_20090130 : Log Unicode Writing
	if ((fsource = _wfopen(fname, _T("ab+,ccs=UNICODE"))) == NULL)
	{
		//Log ������ ������ �� �߽��ϴ�.
//		MyMessageBox(MY_MESSAGE_BOX, 64004, _T("File Write Error!"), M_ICONERROR);

		return FALSE;
	}

	//���� �߻� �α� ���� ���
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
//Tab No�� �α׿�...
int MCtrlInspectionCamera::InspectTabAttached(int iTabNo)
{
	int iResult = ERR_CTRL_INSPECTIONCAMERA_SUCCESS;
	
	//170616 JSH Skip ó��
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
