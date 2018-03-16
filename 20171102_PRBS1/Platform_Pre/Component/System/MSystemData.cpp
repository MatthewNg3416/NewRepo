#include "stdafx.h"
#include "common.h"
#include "MSystemData.h"
#include "MyProgressWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MSystemData::MSystemData(CString strFileName) : MFileManager(strFileName)
{
#undef SYSTEM_DATA
#undef SYSTEM_DATA_ARR
#define SYSTEM_DATA(type, name) m_iDataType.push_back(name.Initlize(#name));
#define SYSTEM_DATA_ARR(type, name, arr) for(int i = 0; i<arr;i++) m_iDataType.push_back(name[i].Initlize(#name, i));
#include "info\test.inf"
#undef SYSTEM_DATA
#undef SYSTEM_DATA_ARR

	initialize();
}

MSystemData::MSystemData(CString strFileName, CString strPath) : MFileManager(strFileName, strPath)
{
	initialize();
}

void MSystemData::initialize()
{
	for (size_t i = 0; i < m_iDataType.size(); i++)
		m_iDataType.at(i)->ResetModelData();

	m_strJSHModelName.SetHardCodeData(_T("안녕ㅎㅎ"));
	m_dJSH.SetHardCodeData(0.5);
	//return;

	//111202.kms__________
	m_strCurrentDeviceID = "";

	m_strModelFileName = "";
	m_strModelFileDir = " ";
	m_strOperatorPassWord = "";	
	m_strEngineerPassWord = "";	
	m_strMakerPassWord = "";
	int i = 0, j = 0;

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		for (int j = 0; j < DEF_MAX_INSPECTION_CAM_CARRIER; j++)
		{
			m_rgdInspectionCamDistViewCenter_Expand[i][j] = 0.0;
			m_rgdInspectionCamDistViewCenter_Shrink[i][j] = 0.0;
			m_rgdInspectionCamDist_CenterToCenter[i][j] = 0.0;
		}
	}

//#ifdef DEF_SOURCE_SYSTEM
//	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
//		m_rgdInspectionCamVariableDist[i] = 0.0;
//#endif

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		m_rgdPreAlignCamDistViewCenter_Expand[i] = 0.0;
		m_rgdPrealignCamVariableDist[i] = 0.0;
		m_rgdPreAlignCamDistViewCenter_Shrink[i] = 0.0;
		m_rgdPreAlignCamDist_CenterToCenter[i] = 0.0;
	}

	m_uiMold1_TotalCount = 1;
	m_uiMold2_TotalCount = 1;
	m_uiMold1_CurCount = 1;
	m_uiMold2_CurCount = 1;
	m_uiMoldMaxCount = 1;
	m_uiMoldBlowPeriod = 1;
	m_uiMoldBlowTime = 1;

	m_iPanelAlignRetryLimit =1;
	m_dBacklash = 0.0;
	m_dAirPlateBlowOnDelay = 0.5;
	m_dPanelAlignerWorkVelocity = 600;

	m_dSprocket1_F_AutoHoming_Offset = 0.0;
	m_dSprocket1_R_AutoHoming_Offset = 0.0;
	m_dSprocket2_F_AutoHoming_Offset = 0.0;
	m_dSprocket2_R_AutoHoming_Offset = 0.0;

	m_dAlignCam1_FixedPos_X = 0.0;
	m_dAlignCam1_FixedPos_Y = 0.0;
	m_dAlignCam2_FixedPos_X = 0.0;
	m_dAlignCam2_FixedPos_Y = 0.0;
	m_dCamCarrier1_FixedPosX = 0.0;
	m_dCamCarrier2_FixedPosX = 0.0;
	m_dDistBetweenAligners = 0.0;
	m_dDist_AlignerToRotator_X = 0.0;
	m_dDist_AlignerToRotator_Y = 0.0;

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		m_dDistPosInspectCamera[i] = 0.0;
		m_dDistNegInspectCamera[i] = 0.0;

		for (int j=0; j<DEF_MAX_CAM_EXPAND; j++)
		{
			m_dData_CameraExpand[i][j] = 0.0;
		}
	}

	m_dInspectCamFixedPosWidth = 40.0;

	m_bUseAutoModelChange = FALSE;
	m_bUseMaterialDeviceID = FALSE;
	m_bUseMoldValidation = FALSE;
	// 100807. ECID _____________
	m_bUseECDataReport = FALSE;
	
	m_bUse1FeederAllMount = FALSE;
	m_bUseAlignPass = FALSE;

	m_bUseSyncMode = FALSE;
	m_bUseProductPlanValidation = FALSE;
	m_bUseTXDataCheckMode = FALSE;

	//110624______________
	for (int i = 0; i < DEF_VALIDATION_MAX_ITEM ; i++)
	{
		m_bValidationCheckSelect[i] = FALSE;
	}
	//____________________

	m_bUseDryrunFeed = FALSE;

	//110707_____
	m_nPressActionMode = 0;//0->Pin상승후 Punching, 1->동시동작
	m_nSprocket1ClutchMode = 1;//0->Feeding후 ClutchOff, 1->Punching후 ClutchOff
	//___________

	//111202.kms__________
	//	m_strCurrentDeviceID.Empty();

	m_dEmptyElapseTime = 0.0;
	//120130.kms_______
	m_bUse4MountAll = FALSE;

	m_bUseMoldAutoSearch = FALSE;

	m_bUseCheckVacuumBeforeTabLoading = FALSE; //170201 SJ_YSH
	//130424.kms________
//	m_dCalResult_F = 0.0;
//	m_dCalResult_R = 0.0;

	//130503.kms______
	for (int i = 0; i<31; i++)
	{
		m_dCalResult_F[i] = 0.0;
		m_dCalResult_R[i] = 0.0;
	}
	//________________


	m_dMoldRegCnt_F = 0.0;
	m_dMoldRegCnt_R = 0.0;
	//__________________

	//131029. SJ_YTH
	m_dTabXCheck = 0.0;
	m_dTabYCheck = 0.0;
	//__________________

	m_dMountMoveSpeed = 0.0;

}

MSystemData::~MSystemData()
{
}

int MSystemData::ReadData()
{
	for (size_t i = 0; i < m_iDataType.size(); i++)
		m_iDataType.at(i)->ReadData();
	//return true;

	BOOL	bReturn = TRUE;
	CString	strSection;
	CString	strItem;
	CString	strTemp;

	// 진행상황 Display용 //ProgWnd의 설정 및 출력	
	//CMyProgressWnd ProgWnd(NULL, _T("System Data File Loading..."));
	//ProgWnd.SetRange(0, 88);
	//ProgWnd.SetText(_T("Operation is in progress.\nWait a moment."));
	//ProgWnd.PeekAndPump();

	/*
	**********************************************************************************************
	** [Common]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), COMMON_SECTION);

	if (!GetValue(strSection, _T("CurrentDeviceID"), &m_strCurrentDeviceID))
		DISPLAY_READ_ERROR(strSection, _T("CurrentDeviceID"), bReturn);

	if (!GetValue(strSection, _T("ModelName "), &m_strModelFileName))
		DISPLAY_READ_ERROR(strSection, _T("ModelName"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection, _T("ModelDir"), &m_strModelFileDir))
		DISPLAY_READ_ERROR(strSection, _T("ModelDir"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection, _T("OperatorPassWord"), &m_strOperatorPassWord))
		DISPLAY_READ_ERROR(strSection, _T("OperatorPassWord"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection, _T("EngineerPassWord"), &m_strEngineerPassWord))
		DISPLAY_READ_ERROR(strSection, _T("EngineerPassWord"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection, _T("MakerPassWord"), &m_strMakerPassWord))
		DISPLAY_READ_ERROR(strSection, _T("MakerPassWord"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection, _T("LanguageSelect"), &m_uiLanguageSelect))
		DISPLAY_READ_ERROR(strSection, _T("LanguageSelect"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection, _T("RunMode"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("RunMode"), bReturn);
	else
	{
		if (strTemp.Compare(_T("PASS")) == 0)
			m_eRunMode = PASS_RUN_MODE;
		else if (strTemp.Compare(_T("DRY")) == 0)
			m_eRunMode = DRY_RUN_MODE;
		else if (strTemp.Compare(_T("REPAIR")) == 0)
			m_eRunMode = REPAIR_RUN_MODE;
		else
			m_eRunMode = NORMAL_RUN_MODE;
	}

	if (!GetValue(strSection, _T("VelocityMode"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("VelocityMode"), bReturn);
	else
	{
		if (strTemp.Compare(_T("Slow")) == 0)
			m_iVelocityMode = FALSE;
		else
			m_iVelocityMode = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection, _T("LineControllerIP"), &m_strLineControllerIP))
		DISPLAY_READ_ERROR(strSection, _T("LineControllerIP"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection, _T("LineControllerPort"), &m_uiLineControllerPort))
		DISPLAY_READ_ERROR(strSection, _T("LineControllerPort"), bReturn);

	//ProgWnd.StepIt();

#ifdef DEF_SOURCE_SYSTEM
	m_eSystemType = SYSTEM_TYPE_SOURCE;
#endif
	
#ifdef DEF_GATE_SYSTEM
	m_eSystemType = SYSTEM_TYPE_GATE;
	m_iUnitNumber = DEF_UNIT1;
#endif
	//ProgWnd.StepIt();

	if (m_eSystemType == SYSTEM_TYPE_SOURCE)
	{
		if (!GetValue(strSection,_T("UnitNumber"), &m_iUnitNumber))
			DISPLAY_READ_ERROR(strSection,_T("UnitNumber"), bReturn);

		if (DEF_UNIT2 != m_iUnitNumber && DEF_UNIT3 != m_iUnitNumber)
			m_iUnitNumber = DEF_UNIT3;	// 확인 필요 : 잘못된 숫자가 들어간 경우, 무조건 3번 Unit 으로 설정되어 Light Curtain 무시하는 경우 있다.
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("StopperArrivingDelay"), &m_dStopperArrivingDelay))
		DISPLAY_READ_ERROR(strSection,_T("StopperArrivingDelay"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("PanelTabDist"), &m_dPanelTabDist))
		DISPLAY_READ_ERROR(strSection,_T("PanelTabDist"), bReturn);

	//ProgWnd.StepIt();

	/*
	**********************************************************************************************
	** [Function]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), FUNCTION_SECTION);

	if (!GetValue(strSection,_T("UseAutoSearchPanel"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseAutoSearchPanel"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseAutoSearchPanel = FALSE;
		else
			m_bUseAutoSearchPanel = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UsePlateVacuum"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UsePlateVacuum"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUsePlateVacuum = FALSE;
		else
			m_bUsePlateVacuum = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UseTabReelAutoChange"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseTabReelAutoChange"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseTabReelAutoChange = FALSE;
		else
			m_bUseTabReelAutoChange = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UseMaterialPrealarm"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseMaterialPrealarm"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseMaterialPrealarm = FALSE;
		else
			m_bUseMaterialPrealarm = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UseHeaterAlarm"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseHeaterAlarm"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseHeaterAlarm = FALSE;
		else
			m_bUseHeaterAlarm = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UseBuzzer"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseBuzzer"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseBuzzer = FALSE;
		else 
			m_bUseBuzzer = TRUE;
	}
	//ProgWnd.StepIt();
#ifdef DEF_SETUP //20140306 SJ_YSH 정연수D요청
	if (!GetValue(strSection,_T("UseSafetyModule"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseSafetyModule"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bSafetySensor = FALSE;
		else 
			m_bSafetySensor = TRUE;
	}
#else
	m_bSafetySensor = TRUE;
#endif
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UseAutoSearchAreaInAfterInspection"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseAutoSearchAreaInAfterInspection"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseAutoSearchAreaInAfterInspection = FALSE;
		else 
			m_bUseAutoSearchAreaInAfterInspection = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UseInspectionAfterMountInAutoRun"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseInspectionAfterMountInAutoRun"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseInspectionAfterMountInAutoRun = FALSE;
		else 
			m_bUseInspectionAfterMountInAutoRun = TRUE;
	}
	//ProgWnd.StepIt();

// 2009.01.23 CYJ UseTestStop 기능을 UsePanelPusher로 변경.
/*	if (!GetValue(strSection,_T("UseTestStop"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseTestStop"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseTestStop = FALSE;
		else 
			m_bUseTestStop = TRUE;
	}
	//ProgWnd.StepIt();
*/

	/*/@131127.KKY.PanelPusher기능 수정______________
	if (!GetValue(strSection,_T("UsePanelPusher"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UsePanelPusher"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUsePanelPusher = FALSE;
		else 
			m_bUsePanelPusher = TRUE;
	}
	/*/
	if (!GetValue(strSection,_T("UsePanelPusher"), &m_bUsePanelPusher))
		DISPLAY_READ_ERROR(strSection,_T("UsePanelPusher"), bReturn);
	//@______________________________________________

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UsePanelValidation"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UsePanelValidation"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUsePanelValidation = FALSE;
		else
			m_bUsePanelValidation = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UsePanelAbsorbOffsetReflection"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UsePanelAbsorbOffsetReflection"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUsePanelAbsorbOffsetReflection = FALSE;
		else 
			m_bUsePanelAbsorbOffsetReflection = TRUE;
	}
	//ProgWnd.StepIt();

//SESL_LKH_090711 : m_bSafetySensor와 통합
/*
	if (FALSE == m_bSafetySensor)
	{
		if (!GetValue(strSection, _T("DisplayDoorOpenError"), &strTemp))
			DISPLAY_READ_ERROR(strSection, _T("DisplayDoorOpenError"), bReturn);
		else
		{
			if (strTemp.Compare(_T("FALSE")) == 0)
				m_bDisplayDoorOpenError = FALSE;
			else 
				m_bDisplayDoorOpenError = TRUE;
		}
	}
	else
	{
		m_bDisplayDoorOpenError = FALSE;
	}
	//ProgWnd.StepIt();
*/
	if (!GetValue(strSection,_T("UseVIPMode"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseVIPMode"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseVIPMode = FALSE;
		else 
			m_bUseVIPMode = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("UseMoldBlow"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("UseMoldBlow"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseMoldBlow = FALSE;
		else 
			m_bUseMoldBlow = TRUE;
	}
	//ProgWnd.StepIt();

	//20081110 by kss
	if (!GetValue(strSection, _T("UsePanelPreAlign"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UsePanelPreAlign"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUsePanelPreAlign = FALSE;
		else 
			m_bUsePanelPreAlign = TRUE;
	}
	//ProgWnd.StepIt();

	/** Auto Model Change 사용여부 */
	if (!GetValue(strSection, _T("UseAutoModelChange"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseAutoModelChange"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseAutoModelChange = FALSE;
		else 
			m_bUseAutoModelChange = TRUE;
	}
	//ProgWnd.StepIt();

	/** Material Validation 사용여부 */
	if (!GetValue(strSection, _T("UseMaterialDeviceID"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseMaterialDeviceID"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseMaterialDeviceID = FALSE;
		else 
			m_bUseMaterialDeviceID = TRUE;
	}
	//ProgWnd.StepIt();

	//100825.kms___________________
	/** Mold Validation 사용여부 */
	if (!GetValue(strSection, _T("UseMoldValidation"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseMoldValidation"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseMoldValidation = FALSE;
		else 
			m_bUseMoldValidation = TRUE;
	}

	if (!GetValue(strSection, _T("UseFeeder"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseFeeder"), bReturn);
	else
	{
		m_nUseFeeder = _ttoi(strTemp);
	}

	// 100807. ECID ___________________
	if (!GetValue(strSection, _T("UseECDataReport"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseECDataReport"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseECDataReport = FALSE;
		else 
			m_bUseECDataReport = TRUE;
	}
	//_________________________________	

	//110110.Add_____	
	if (!GetValue(strSection, _T("Use1FeederAllMount"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("Use1FeederAllMount"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUse1FeederAllMount = FALSE;
		else 
			//120116.KMS______
			m_bUse1FeederAllMount = FALSE;
//			m_bUse1FeederAllMount = TRUE;
			//________________
	}
	//110413.Add_____	
	if (!GetValue(strSection, _T("UseAlignPass"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseAlignPass"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseAlignPass = FALSE;
		else 
			m_bUseAlignPass = TRUE;
	}
	//_______________

	if (!GetValue(strSection, _T("UseSyncMode"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseSyncMode"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseSyncMode = FALSE;
		else 
			m_bUseSyncMode = TRUE;
	}

	if (!GetValue(strSection, _T("UseProductPlanValidation"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseProductPlanValidation"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseProductPlanValidation = FALSE;
		else 
			m_bUseProductPlanValidation = TRUE;
	}

	if (!GetValue(strSection, _T("UseTXDataCheckMode"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseTXDataCheckMode"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseTXDataCheckMode = FALSE;
		else 
			m_bUseTXDataCheckMode = TRUE;
	}

	//110707_____
	if (!GetValue(strSection,_T("PressActionMode"), &m_nPressActionMode))
		DISPLAY_READ_ERROR(strSection,_T("PressActionMode"), bReturn);
	if (!GetValue(strSection,_T("Sprocket1ClutchMode"), &m_nSprocket1ClutchMode))
		DISPLAY_READ_ERROR(strSection,_T("Sprocket1ClutchMode"), bReturn);
	//___________

	//120130.kms_________
#ifndef DEF_GATE_SYSTEM
	if (!GetValue(strSection, _T("Use4MountAll"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("Use4MountAll"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUse4MountAll = FALSE;
		else 
			m_bUse4MountAll = TRUE;
	}
#endif

	if (!GetValue(strSection, _T("IonizerCheck"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("IonizerCheck"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseIonizerCheck = FALSE;
		else 
			m_bUseIonizerCheck = TRUE;
	}

	if (!GetValue(strSection, _T("UseTabRemoveAuto"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseTabRemoveAuto"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseTabRemoveAuto = FALSE;
		else 
			m_bUseTabRemoveAuto = TRUE;
	}	
	//__________________

	if (!GetValue(strSection, _T("UseMoldAutoSearch"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseMoldAutoSearch"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseMoldAutoSearch = FALSE;
		else 
			m_bUseMoldAutoSearch = TRUE;
	}

	//170201 SJ_YSH
	if (!GetValue(strSection, _T("UseCheckVacuumBeforeTabLoading"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseCheckVacuumBeforeTabLoading"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseCheckVacuumBeforeTabLoading = FALSE;
		else 
			m_bUseCheckVacuumBeforeTabLoading = TRUE;
	}
	//__________________________//

	//170612 JSH.s
	if (!GetValue(strSection, _T("UseInspectionTabBolb"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseInspectionTabBolb"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseInspectionTabBolb = FALSE;
		else 
			m_bUseInspectionTabBolb = TRUE;
	}
	//170612 JSH.e

	//170718 JSH.s
	if (!GetValue(strSection, _T("m_bUse1stPanelAlign"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("m_bUse1stPanelAlign"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUse1stPanelAlign = FALSE;
		else 
			m_bUse1stPanelAlign = TRUE;
	}
	//170612 JSH.e
	
	//170927 JSH.s
	if (!GetValue(strSection, _T("UseBondingAfterImgLog"), &strTemp))
		DISPLAY_READ_ERROR(strSection, _T("UseBondingAfterImgLog"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bUseBondingAfterImgLog = FALSE;
		else 
			m_bUseBondingAfterImgLog = TRUE;
	}
	//170927 JSH.e
	
	/*
	**********************************************************************************************
	** [Material]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), MATERIAL_SECTION);

	/** Mold[1] 전체 사용 횟수 */
	if (!GetValue(strSection,_T("Mold1_TotalCount"), &m_uiMold1_TotalCount))
		DISPLAY_READ_ERROR(strSection,_T("Mold1_TotalCount"), bReturn);

	//ProgWnd.StepIt();

	/** Mold[2] 전체 사용 횟수 */
	if (!GetValue(strSection,_T("Mold2_TotalCount"), &m_uiMold2_TotalCount))
		DISPLAY_READ_ERROR(strSection,_T("Mold2_TotalCount"), bReturn);

	//ProgWnd.StepIt();

	/** Mold[1] 세정 후 사용 횟수 */
	if (!GetValue(strSection,_T("Mold1_CurCount"), &m_uiMold1_CurCount))
		DISPLAY_READ_ERROR(strSection,_T("Mold1_CurCount"), bReturn);

	//ProgWnd.StepIt();

	/** Mold[2] 세정 후 사용 횟수 */
	if (!GetValue(strSection,_T("Mold2_CurCount"), &m_uiMold1_CurCount))
		DISPLAY_READ_ERROR(strSection,_T("Mold2_CurCount"), bReturn);

	//ProgWnd.StepIt();

	/*
	**********************************************************************************************
	** [TabMounter]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), TABMOUNTER_SECTION);

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		strItem.Format(_T("ToolOffsetMounter%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdToolOffsetMounter[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("ToolOffsetTabAligner%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdToolOffsetTabAligner[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();
		
/* 삭제 예정
		strItem.Format(_T("UseMounter%d"), i + 1);
		if (!GetValue(strSection, strItem, &strTemp))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		else
		{
			if (strTemp.Compare(_T("FALSE")) == 0)
				m_rgbUseMounter[i] = FALSE;
			else
				m_rgbUseMounter[i] = TRUE;
		}
		//ProgWnd.StepIt();
*/
	}

	if (!GetValue(strSection,_T("MountRetryLimit"), &m_iMountRetryLimit))
		DISPLAY_READ_ERROR(strSection,_T("MountRetryLimit"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("BondTime"), &m_dBondTime))
		DISPLAY_READ_ERROR(strSection,_T("BondTime"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("StartMoveDelay"), &m_dStartMoveDelay))
		DISPLAY_READ_ERROR(strSection,_T("StartMoveDelay"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("2ndGroupMountStartDelay"), &m_d2ndGroupMountStartDelay))
		DISPLAY_READ_ERROR(strSection,_T("2ndGroupMountStartDelay"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("IFDelayLimit"), &m_dIFDelayLimit))
		DISPLAY_READ_ERROR(strSection,_T("IFDelayLimit"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("TabMounterStabilizationDelay"), &m_dTabMounterStabilizationDelay))
		DISPLAY_READ_ERROR(strSection,_T("TabMounterStabilizationDelay"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("MountPosTolerance"), &m_dMountPosTolerance))
		DISPLAY_READ_ERROR(strSection,_T("MountPosTolerance"), bReturn);

	//140626. SJ_YTH.
	if (!GetValue(strSection,_T("MountMoveSpeed"), &m_dMountMoveSpeed))
		DISPLAY_READ_ERROR(strSection,_T("MountMoveSpeed"), bReturn);
	//___________________________//

	//ProgWnd.StepIt();
	
	/*
	**********************************************************************************************
	** [TabFeeder]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), TABFEEDER_SECTION);

	/** 금형 Data 기록 주기 : 설정된 회수만큼의 Panel 배출 후 Data 기록. */
	if (!GetValue(strSection,_T("MoldMaxCount"), &m_uiMoldMaxCount))
		DISPLAY_READ_ERROR(strSection,_T("MoldMaxCount"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("MoldBlowPeriod"), &m_uiMoldBlowPeriod))
		DISPLAY_READ_ERROR(strSection,_T("MoldBlowPeriod"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("MoldBlowTime"), &m_uiMoldBlowTime))
		DISPLAY_READ_ERROR(strSection,_T("MoldBlowTime"), bReturn);

	//ProgWnd.StepIt();

	/** 세정 작업 후 금형 사용 제한 횟수*/
	if (!GetValue(strSection,_T("MoldLimitCountAfterCleaning"), &m_uiMoldLimitCountAfterCleaning))
		DISPLAY_READ_ERROR(strSection,_T("MoldLimitCountAfterCleaning"), bReturn);

	//ProgWnd.StepIt();

	/** 연속 Bad-Tab IC 제한 횟수*/
	if (!GetValue(strSection,_T("BadTabLimitCount"), &m_uiBadTabLimitCount))
		DISPLAY_READ_ERROR(strSection,_T("BadTabLimitCount"), bReturn);
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("Sprocket1_F_AutoHoming_Offset"), &m_dSprocket1_F_AutoHoming_Offset))
		DISPLAY_READ_ERROR(strSection,_T("Sprocket1_F_AutoHoming_Offset"), bReturn);
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("Sprocket1_R_AutoHoming_Offset"), &m_dSprocket1_R_AutoHoming_Offset))
		DISPLAY_READ_ERROR(strSection,_T("Sprocket1_R_AutoHoming_Offset"), bReturn);
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("Sprocket2_F_AutoHoming_Offset"), &m_dSprocket2_F_AutoHoming_Offset))
		DISPLAY_READ_ERROR(strSection,_T("Sprocket2_F_AutoHoming_Offset"), bReturn);
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("Sprocket2_R_AutoHoming_Offset"), &m_dSprocket2_R_AutoHoming_Offset))
		DISPLAY_READ_ERROR(strSection,_T("Sprocket2_R_AutoHoming_Offset"), bReturn);
	//ProgWnd.StepIt();


	//CString strItem;

	//130424.kms_________
	CString strCalFResult, strCalRResult;
	for (int i = 0; i < 31; i++)
	{
		strCalFResult.Format(_T("CalResult_F#%d"), i);
		if (!GetValue(strSection, strCalFResult, &m_dCalResult_F[i]))
		{
//@			DISPLAY_READ_ERROR(strSection, strCalFResult, bReturn);
			m_dCalResult_F[i] = 0.0;
		}
		strCalRResult.Format(_T("CalResult_R#%d"), i);
		if (!GetValue(strSection, strCalRResult, &m_dCalResult_R[i]))
		{
//@			DISPLAY_READ_ERROR(strSection, strCalRResult, bReturn);
			m_dCalResult_R[i] = 0.0;
		}
	}

	/*
	if (!GetValue(strSection,_T("MoldRegCnt_F"), &m_dMoldRegCnt_F))
		DISPLAY_READ_ERROR(strSection,_T("MoldRegCnt_F"), bReturn);
	if (!GetValue(strSection,_T("MoldRegCnt_R"), &m_dMoldRegCnt_R))
		DISPLAY_READ_ERROR(strSection,_T("MoldRegCnt_R"), bReturn);
		*/
	//___________________

	/*
	**********************************************************************************************
	** [TabCarrier]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), TABCARRIER_SECTION);

	/** Carrier Tab Loading Retry 횟수*/
/* 삭제 예정.
	if (!GetValue(strSection,_T("CarrierRetryCount"), &m_iCarrierRetryCount))
		DISPLAY_READ_ERROR(strSection,_T("CarrierRetryCount"), bReturn);

	//ProgWnd.StepIt();
*/
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		strItem.Format(_T("CarrierHeadCenterX_%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdCarrierHeadCenterX[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		strItem.Format(_T("RCarrierHeadCenterX_%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdRCarrierHeadCenterX[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		strItem.Format(_T("CarrierHeadCenterY_%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdCarrierHeadCenterY[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		strItem.Format(_T("RCarrierHeadCenterY_%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdRCarrierHeadCenterY[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
	}

	/*
	**********************************************************************************************
	** [Panel Aligner]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), PANELALIGNER_SECTION);

	if (!GetValue(strSection,_T("Distance4DifVel"), &m_iDistance4DifVel))
		DISPLAY_READ_ERROR(strSection,_T("Distance4DifVel"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("ActiveAlignerPosX"), &m_dActiveAlignerPosX))
		DISPLAY_READ_ERROR(strSection,_T("ActiveAlignerPosX"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("ActiveAlignerPosY"), &m_dActiveAlignerPosY))
		DISPLAY_READ_ERROR(strSection,_T("ActiveAlignerPosY"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("OriginDistBetweenAligners"), &m_dOriginDistBetweenAligners))
		DISPLAY_READ_ERROR(strSection,_T("OriginDistBetweenAligners"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("AutoSearchPanelMarkDistance"), &m_dAutoSearchPanelMarkDistance))
		DISPLAY_READ_ERROR(strSection,_T("AutoSearchPanelMarkDistance"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("PanelAlignTolerance"), &m_dPanelAlignTolerance))
		DISPLAY_READ_ERROR(strSection,_T("PanelAlignTolerance"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("PanelAlignRetryLimit"), &m_iPanelAlignRetryLimit))
		DISPLAY_READ_ERROR(strSection,_T("PanelAlignRetryLimit"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("Backlash"), &m_dBacklash))
		DISPLAY_READ_ERROR(strSection,_T("Backlash"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("AirPlateBlowOnDelay"), &m_dAirPlateBlowOnDelay))
		DISPLAY_READ_ERROR(strSection,_T("AirPlateBlowOnDelay"), bReturn);

	if (!GetValue(strSection,_T("PanelAlignerWorkVelocity"), &m_dPanelAlignerWorkVelocity))
		DISPLAY_READ_ERROR(strSection,_T("PanelAlignerWorkVelocity"), bReturn);
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("AlignCam1_FixedPos_X"), &m_dAlignCam1_FixedPos_X))
		DISPLAY_READ_ERROR(strSection,_T("AlignCam1_FixedPos_X"), bReturn);

	if (!GetValue(strSection,_T("AlignCam1_FixedPos_Y"), &m_dAlignCam1_FixedPos_Y))
		DISPLAY_READ_ERROR(strSection,_T("AlignCam1_FixedPos_Y"), bReturn);

	if (!GetValue(strSection,_T("AlignCam2_FixedPos_X"), &m_dAlignCam2_FixedPos_X))
		DISPLAY_READ_ERROR(strSection,_T("AlignCam2_FixedPos_X"), bReturn);

	if (!GetValue(strSection,_T("AlignCam2_FixedPos_Y"), &m_dAlignCam2_FixedPos_Y))
		DISPLAY_READ_ERROR(strSection,_T("AlignCam2_FixedPos_Y"), bReturn);

	if (!GetValue(strSection,_T("CamCarrier1_FixedPosX"), &m_dCamCarrier1_FixedPosX))
		DISPLAY_READ_ERROR(strSection,_T("CamCarrier1_FixedPosX"), bReturn);

	if (!GetValue(strSection,_T("CamCarrier2_FixedPosX"), &m_dCamCarrier2_FixedPosX))
		DISPLAY_READ_ERROR(strSection,_T("CamCarrier2_FixedPosX"), bReturn);

	if (!GetValue(strSection,_T("DistBetweenAligners"), &m_dDistBetweenAligners))
		DISPLAY_READ_ERROR(strSection,_T("DistBetweenAligners"), bReturn);

	/*/
#ifdef DEF_GATE_SYSTEM
	if (!GetValue(strSection,_T("Dist_AlignerToRotator_X"), &m_dDist_AlignerToRotator_X))
		DISPLAY_READ_ERROR(strSection,_T("Dist_AlignerToRotator_X"), bReturn);

	if (!GetValue(strSection,_T("Dist_AlignerToRotator_Y"), &m_dDist_AlignerToRotator_Y))
		DISPLAY_READ_ERROR(strSection,_T("Dist_AlignerToRotator_Y"), bReturn);
#endif
	/*/

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("DistPosInspectCamera%d"), i+1);
		if (!GetValue(strSection, strItem, &m_dDistPosInspectCamera[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("DistNegInspectCamera%d"), i+1);
		if (!GetValue(strSection, strItem, &m_dDistNegInspectCamera[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
	}

	if (!GetValue(strSection, _T("InspectCamFixedPosWidth"), &m_dInspectCamFixedPosWidth))
		DISPLAY_WRITE_ERROR(strSection, _T("InspectCamFixedPosWidth"), bReturn);

	/*
	**********************************************************************************************
	** [Model Change Distance]
	**********************************************************************************************
	*/
	/*
	strSection.Format(_T("%s"), MODEL_CHANGE_DIST_SECTION);
	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("ModelChangeOriginDist%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dModelChangeOriginDist[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

//		if (!GetValue(strSection,_T("DistBetweenRGantryHomeToModelChangeHome"), &m_dDistBetweenRGantryHomeToModelChangeHome))
//			DISPLAY_READ_ERROR(strSection,_T("DistBetweenRGantryHomeToModelChangeHome"), bReturn);
		strItem.Format(_T("DistBetweenGantryHomeToModelChangeHome%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dDistBetweenGantryHomeToModelChangeHome[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
	}
	*/
	m_dModelChangeOriginDist[0] = m_dAlignCam1_FixedPos_Y;
	m_dModelChangeOriginDist[1] = m_dAlignCam2_FixedPos_Y;
	strSection.Format(_T("%s"), MODEL_CHANGE_DIST_SECTION);
	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("GantryOriginDist%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dGantryOriginDist[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
	}

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("BadTabOriginDist%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dDistBadTabOriginDist[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
	}

	//131029. SJ_YTH
	strSection.Format(_T("%s"), FUNCTION_SECTION);
	if (!GetValue(strSection,_T("TabXCheck"), &m_dTabXCheck))
		DISPLAY_READ_ERROR(strSection,_T("TabXCheck"), bReturn);

	if (!GetValue(strSection,_T("TabYCheck"), &m_dTabYCheck))
		DISPLAY_READ_ERROR(strSection,_T("TabYCheck"), bReturn);
	//___________________

	//ProgWnd.StepIt();

	/*
	**********************************************************************************************
	** [TabAligner]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), TABALIGNER_SECTION);

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		strItem.Format(_T("TabCameraDist1_TabMounterGroup%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dTabCameraDist1[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabCameraDist2_TabMounterGroup%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dTabCameraDist2[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabCameraDist3_TabMounterGroup%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dTabCameraDist3[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabCameraDist4_TabMounterGroup%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dTabCameraDist4[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();
	}

	if (!GetValue(strSection,_T("TabICLeadCheck"), &m_dTabICLeadCheck))
		DISPLAY_READ_ERROR(strSection,_T("TabICLeadCheck"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("TabAlignerOperationLimit"), &m_iTabAlignerOperationLimit))
		DISPLAY_READ_ERROR(strSection,_T("TabAlignerOperationLimit"), bReturn);

	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("TabAlignerTolerance"), &m_dTabAlignerTolerance))
		DISPLAY_READ_ERROR(strSection,_T("TabAlignerTolerance"), bReturn);

	//ProgWnd.StepIt();

	for (int i = 0; i < DEF_MAX_TABALIGNER; i++)
	{
		strItem.Format(_T("UseTabAligner%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgbUseTabAligner[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabAligner%d_Base1"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdTabAlignerBase1[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabAligner%d_Base2"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdTabAlignerBase2[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabAligner%d_EtcOffset1"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdTabAlignerEtcOffset1[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabAligner%d_EtcOffset2"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdTabAlignerEtcOffset2[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();
	}

	/*
	**********************************************************************************************
	** [Camera Prism]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), CAMERA_PRISM_SECTION);
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		strItem.Format(_T("InspectionCamDistBetweenPrismViewCenterForCam%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdInspectionCamDistViewCenter_Expand[0][i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("InspectionCamDistViewCenter_Shrink%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdInspectionCamDistViewCenter_Shrink[0][i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("InspectionCamDist_CenterToCenter%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdInspectionCamDist_CenterToCenter[0][i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		
		//ProgWnd.StepIt();
	}

	for (int i = 0; i < DEF_MAX_CAM_EXPAND; i++)
	{
		strItem.Format(_T("Data_CameraExpand1_%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dData_CameraExpand[0][i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
	}


//	strSection.Format(_T("%s"), R_CAMERA_PRISM_SECTION);
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		strItem.Format(_T("RInspectionCamDistBetweenPrismViewCenterForCam%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdInspectionCamDistViewCenter_Expand[1][i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("RInspectionCamDistViewCenter_Shrink%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdInspectionCamDistViewCenter_Shrink[1][i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("RInspectionCamDist_CenterToCenter%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdInspectionCamDist_CenterToCenter[1][i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		
		//ProgWnd.StepIt();
	}

	for (int i = 0; i < DEF_MAX_CAM_EXPAND; i++)
	{
		strItem.Format(_T("Data_CameraExpand2_%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_dData_CameraExpand[1][i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
	}

//#ifdef DEF_SOURCE_SYSTEM
//	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
//	{
//		strItem.Format(_T("VariableDistForInspectionCam%d"), i + 1);
//		if (!GetValue(strSection, strItem, &m_rgdInspectionCamVariableDist[i]))
//			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
//
//		//ProgWnd.StepIt();
//	}
//#endif

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("PreAlignCamDistBetweenPrismViewCenterForCam%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdPreAlignCamDistViewCenter_Expand[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("PreAlignCamDistViewCenter_Shrink%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdPreAlignCamDistViewCenter_Shrink[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("PreAlignCamDist_CenterToCenter%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdPreAlignCamDist_CenterToCenter[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		
		//ProgWnd.StepIt();
	}

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("PrealignCamVariableDist%d"), i + 1);
		if (!GetValue(strSection, strItem, &m_rgdPrealignCamVariableDist[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		//ProgWnd.StepIt();
	}
	
	/*
	**********************************************************************************************
	** [Log]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), LOG_SECTION);

	if (!GetValue(strSection,_T("LogLevelError"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("LogLevelError"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bLogLevelError = FALSE;
		else
			m_bLogLevelError = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("LogLevelWarning"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("LogLevelWarning"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bLogLevelMonitoring = FALSE;
		else
			m_bLogLevelMonitoring = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("LogLevelNormal"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("LogLevelNormal"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bLogLevelNormal = FALSE;
		else
			m_bLogLevelNormal = TRUE;
	}
	//ProgWnd.StepIt();

	if (!GetValue(strSection,_T("LogLevelTactTime"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("LogLevelTactTime"), bReturn);
	else
	{
		// TT_Alarm...20090423 TactTime은 Default가 FALSE
		//if (strTemp.Compare(_T("FALSE")) == 0)
		//	m_bLogLevelTactTime = FALSE;
		//else
		//	m_bLogLevelTactTime = TRUE;
		m_bLogLevelTactTime = FALSE;
	}
	//ProgWnd.StepIt();
//170427 JSH.s
	if (!GetValue(strSection,_T("LogLevelEqTactTime"), &strTemp))
		DISPLAY_READ_ERROR(strSection,_T("LogLevelEqTactTime"), bReturn);
	else
	{
		if (strTemp.Compare(_T("FALSE")) == 0)
			m_bLogLevelEqTactTime = FALSE;
		else
			m_bLogLevelEqTactTime = TRUE;
	}
	//ProgWnd.StepIt();
//170427 JSH.e

	if (!GetValue(strSection,_T("LogKeepingDays"), &m_iLogKeepingDays))
		DISPLAY_READ_ERROR(strSection,_T("LogKeepingDays"), bReturn);

	//ProgWnd.StepIt();


	//100121 SJ_YYK EQ_STOP_DATA
	strSection.Format(_T("%s"), EQSTOP_SECTION);
	CString strError;
	//CString strItem;
	for( int j=0; j<DEF_STOP_MAX_ITEM ; j++)
	{
		// EqStop Code
		strError.Format(_T("StopCode#%d"), j);
		if(!GetValue(strSection, strError, &m_strStopCode[j]))
		{
			DISPLAY_READ_ERROR(strSection,_T("StopCode#%d"), bReturn);
		}
	}
	for(int j=0; j<DEF_STOP_MAX_ITEM ; j++)
	{
		// EqStop Item
		strError.Format(_T("StopItem#%d"), j);
		if(!GetValue(strSection, strError, &m_strStopItem[j]))
		{
			DISPLAY_READ_ERROR(strSection,_T("StopItem#%d"), bReturn);
		}
	}

	//110624_______________________________
	strSection.Format(_T("%s"), PRODUCT_CHECK_SECTION);
	//CString strItem;
	for( int p=0; p<DEF_VALIDATION_MAX_ITEM ; p++)
	{
		// EqStop Code
		strError.Format(_T("ValidationCheck#%d"), p);
		if(!GetValue(strSection, strError, &m_bValidationCheckSelect[p]))
		{
			DISPLAY_READ_ERROR(strSection,_T("ValidationCheck#%d"), bReturn);
		}
	}
	//_______________________________________

	//1100705.kms__________________
	//
	strSection.Format(_T("%s"), MODULEPORT_SECTION);

	if (!GetValue(strSection, _T("EQPID"), &m_strEQPID))
	{
		DISPLAY_READ_ERROR(strSection,_T("EQPID"), bReturn);
//		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_10408"), &szTextTemp1);
//		szTextTemp2.Format(_T("[%s] %s"),strSection,szTextTemp1);
//		szTitleTemp.Format(_T("[%s] File Read Error"),m_strFileName);
//		if (MyMessageBox(szTextTemp2, (LPCTSTR)szTitleTemp, M_ICONERROR|M_YESNO) == IDNO) return FALSE;
//		ret = FALSE;
	}

	for( int m=0; m<DEF_MODULE_PORT_MAX_ID ; m++)
	{
		// EqStop Code
		strError.Format(_T("ModulePortID#%d"), m);
		if(!GetValue(strSection, strError, &m_strModulePortID[m]))
		{
			DISPLAY_READ_ERROR(strSection,_T("ModulePortID#%d"), bReturn);
		}
	}

	//110812 SJ_KJS
	if(!GetValue(strSection, _T("ModuleID"), &m_strModuleID))
	{
		DISPLAY_READ_ERROR(strSection,_T("ModuleID"), bReturn);
	}

	if(!GetValue(strSection, _T("ModuleID2"), &m_strModuleID2))
	{
		DISPLAY_READ_ERROR(strSection,_T("ModuleID2"), bReturn);
	}

	if(!GetValue(strSection, _T("ModuleID3_1"), &m_strModuleID3_1))
	{
		DISPLAY_READ_ERROR(strSection,_T("ModuleID3_1"), bReturn);
	}

	if(!GetValue(strSection, _T("ModuleID3_2"), &m_strModuleID3_2))
	{
		DISPLAY_READ_ERROR(strSection,_T("ModuleID3_2"), bReturn);
	}

	if(!GetValue(strSection, _T("ModuleID3_3"), &m_strModuleID3_3))
	{
		DISPLAY_READ_ERROR(strSection,_T("ModuleID3_3"), bReturn);
	}

	if(!GetValue(strSection, _T("ModuleID3_4"), &m_strModuleID3_4))
	{
		DISPLAY_READ_ERROR(strSection,_T("ModuleID3_4"), bReturn);
	}

	if(!GetValue(strSection, _T("ReportModuelID"), &m_strReportModuelID))
	{
		DISPLAY_READ_ERROR(strSection,_T("ReportModuelID"), bReturn);
	}

	if(!GetValue(strSection, _T("ReportModuleLevel"), &m_iReportModuleLevel))
	{
		DISPLAY_READ_ERROR(strSection,_T("ReportModuleLevel"), bReturn);
	}
	//______________________________
	
	//20110914 SJ_HJG
	strSection.Format(_T("%s"), COMMON_SECTION);
	if (!GetValue(strSection, _T("EmptyElapseTime"), &m_dEmptyElapseTime))
		DISPLAY_READ_ERROR(strSection,_T("EmptyElapseTime"), bReturn);

	//20111205 sj_hjg
	if (!GetValue(strSection, _T("TabOffsetdata"), &m_dTabOffsetdata))
		DISPLAY_READ_ERROR(strSection,_T("TabOffsetdata"), bReturn);

	if (!GetValue(strSection, _T("TabOffsetAverage"), &m_dTabOffsetAverage))
		DISPLAY_READ_ERROR(strSection,_T("TabOffsetAverage"), bReturn);

	strSection.Format(_T("%s"), FUNCTION_SECTION);
	if (!GetValue(strSection, _T("UseAutoAlignSystem"), &m_bUseAutoAlignSystem))
		DISPLAY_READ_ERROR(strSection,_T("UseAutoAlignSystem"), bReturn);
	//___________________

	//130320.kms_________
	strSection.Format(_T("%s"), ERRORNOTSEND_SECTION);
	//CString strItem;
	for (int j=0; j<DEF_MAX_ERROR_NOT_SEND ; j++)
	{
		strError.Format(_T("ErrorNo#%d"), j);
		if(!GetValue(strSection, strError, &m_strErrorNo[j]))
		{
			DISPLAY_READ_ERROR(strSection,_T("ErrorNo%d"), bReturn);
		}
	}
	//___________________

	return bReturn;
}

int MSystemData::CancelChangeData()
{
	for (size_t i = 0; i < m_iDataType.size(); i++)
		m_iDataType.at(i)->CancelChange();

	return true;
}
int MSystemData::WriteData()
{
	if (!MakeBackUpFile())
	{
		//Back Up File을 만드는데 실패 했습니다. 계속 진행 하겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX,10000,_T("File Write Error"), M_ICONERROR|M_YESNO) == IDNO) 
			return FALSE;
	}

	for (size_t i = 0; i < m_iDataType.size(); i++)
		m_iDataType.at(i)->WriteData();

	//return TRUE;

	int iNumOfItems = 87;
	//CMyProgressWnd ProgWnd(NULL,_T("System Data File Saving..."));
	//ProgWnd.SetRange(0, iNumOfItems);
	//ProgWnd.SetText(_T("Operation is in progress.\nWait a moment."));
	//ProgWnd.PeekAndPump();

	CString strSection;
	CString strItem;
	CString strTemp;
	BOOL bReturn = TRUE;

	/*
	**********************************************************************************************
	** [Common]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), COMMON_SECTION);

	if (!SetValue(strSection,_T("CurrentDeviceID"), m_strCurrentDeviceID))
		DISPLAY_WRITE_ERROR(strSection,_T("CurrentDeviceID"), bReturn);

	if (!SetValue(strSection,_T("ModelName "), m_strModelFileName))
		DISPLAY_WRITE_ERROR(strSection,_T("ModelName"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("ModelDir"), m_strModelFileDir))
		DISPLAY_WRITE_ERROR(strSection,_T("ModelDir"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("OperatorPassWord"), m_strOperatorPassWord))
		DISPLAY_WRITE_ERROR(strSection,_T("OperatorPassWord"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("EngineerPassWord"), m_strEngineerPassWord))
		DISPLAY_WRITE_ERROR(strSection,_T("EngineerPassWord"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("MakerPassWord"), m_strMakerPassWord))
		DISPLAY_WRITE_ERROR(strSection,_T("MakerPassWord"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("LanguageSelect"), m_uiLanguageSelect))
		DISPLAY_WRITE_ERROR(strSection,_T("LanguageSelect"), bReturn);

	//ProgWnd.StepIt();

	if (m_eRunMode == PASS_RUN_MODE)
		strTemp.Format(_T("PASS"));
	else if (m_eRunMode == DRY_RUN_MODE)
		strTemp.Format(_T("DRY"));
	else if (m_eRunMode == REPAIR_RUN_MODE)
		strTemp.Format(_T("FALSE"));
	else
		strTemp.Format(_T("NORMAL"));
	if (!SetValue(strSection,_T("RunMode"),strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("RunMode"), bReturn);

	//ProgWnd.StepIt();

	if (m_iVelocityMode == FALSE)
		strTemp.Format(_T("Slow"));
	else
		strTemp.Format(_T("NORMAL"));
	if (!SetValue(strSection,_T("VelocityMode"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("VelocityMode"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("LineControllerIP"), m_strLineControllerIP))
		DISPLAY_WRITE_ERROR(strSection,_T("LineControllerIP"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("LineControllerPort"), m_uiLineControllerPort))
		DISPLAY_WRITE_ERROR(strSection,_T("LineControllerPort"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("UnitNumber"), m_iUnitNumber))
		DISPLAY_WRITE_ERROR(strSection,_T("UnitNumber"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("StopperArrivingDelay"), m_dStopperArrivingDelay))
		DISPLAY_WRITE_ERROR(strSection,_T("StopperArrivingDelay"), bReturn);

	//ProgWnd.StepIt();

//	if (!SetValue(strSection,_T("PanelPreAlignDelay"), m_dPanelTabDist))
	if (!SetValue(strSection,_T("PanelTabDist"), m_dPanelTabDist))
		DISPLAY_WRITE_ERROR(strSection,_T("PanelTabDist"), bReturn);

	//ProgWnd.StepIt();

	/*
	**********************************************************************************************
	** [Function]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), FUNCTION_SECTION);

	strTemp.Format((m_bUseAutoSearchPanel == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseAutoSearchPanel"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseAutoSearchPanel"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUsePlateVacuum == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UsePlateVacuum"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UsePlateVacuum"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUseTabReelAutoChange == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseTabReelAutoChange"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseTabReelAutoChange"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUseMaterialPrealarm == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseMaterialPrealarm"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseMaterialPrealarm"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUseHeaterAlarm == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseHeaterAlarm"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseHeaterAlarm"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUseBuzzer == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseBuzzer"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseBuzzer"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bSafetySensor == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseSafetyModule"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseSafetyModule"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUseAutoSearchAreaInAfterInspection == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseAutoSearchAreaInAfterInspection"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseAutoSearchAreaInAfterInspection"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUseInspectionAfterMountInAutoRun == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseInspectionAfterMountInAutoRun"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseInspectionAfterMountInAutoRun"), bReturn);

	//ProgWnd.StepIt();

// 2009.01.23 CYJ UseTestStop 기능을 UsePanelPusher로 변경.
//	strTemp.Format((m_bUseTestStop == TRUE) ? _T("TRUE") : _T("FALSE"));
//	if (!SetValue(strSection,_T("UseTestStop"), strTemp))
//		DISPLAY_WRITE_ERROR(strSection,_T("UseTestStop"), bReturn);

//	//ProgWnd.StepIt();

	/*/@131127.KKY.PanelPusher기능 수정______________
	strTemp.Format((m_bUsePanelPusher == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UsePanelPusher"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UsePanelPusher"), bReturn);
	/*/
	if (!SetValue(strSection,_T("UsePanelPusher"), m_bUsePanelPusher))
		DISPLAY_WRITE_ERROR(strSection,_T("UsePanelPusher"), bReturn);	
	//@______________________________________________

	//ProgWnd.StepIt();

	if (m_bUsePanelValidation == TRUE)
		strTemp.Format(_T("TRUE"));
	else
		strTemp.Format(_T("FALSE"));
	if (!SetValue(strSection,_T("UsePanelValidation"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UsePanelValidation"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUsePanelAbsorbOffsetReflection == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UsePanelAbsorbOffsetReflection"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UsePanelAbsorbOffsetReflection"), bReturn);

	//ProgWnd.StepIt();

//SESL_LKH_090711 : m_bSafetySensor와 통합
/*
	strTemp.Format((m_bDisplayDoorOpenError == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("DisplayDoorOpenError"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("DisplayDoorOpenError"), bReturn);

	//ProgWnd.StepIt();
*/
	strTemp.Format((m_bUseVIPMode == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseVIPMode"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseVIPMode"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bUseMoldBlow == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("UseMoldBlow"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseMoldBlow"), bReturn);

	//ProgWnd.StepIt();

	//20081110 by kss
	/** UsePanelPreAlign 사용 여부 */
	strTemp.Format((m_bUsePanelPreAlign == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, "UsePanelPreAlign", strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UsePanelPreAlign"), bReturn);

	//ProgWnd.StepIt();

	/** Auto Model Change 사용여부 */
	strTemp.Format((m_bUseAutoModelChange == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, "UseAutoModelChange", strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseAutoModelChange"), bReturn);
	
	/** Material Validation 사용여부 */
	strTemp.Format((m_bUseMaterialDeviceID == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, "UseMaterialDeviceID", strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseMaterialDeviceID"), bReturn);
	
	//100825.kms___________________
	/** Mold Validation 사용여부 */
	strTemp.Format((m_bUseMoldValidation == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, "UseMoldValidation", strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseMoldValidation"), bReturn);
	//ProgWnd.StepIt();

	if (m_nUseFeeder < 0 || m_nUseFeeder > 2)
		m_nUseFeeder = 2;
	strTemp.Format(_T("%d"), m_nUseFeeder);
	if (!SetValue(strSection, "UseFeeder", strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseFeeder"), bReturn);

	// 100807. ECID ___________________
	strTemp.Format((m_bUseECDataReport == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseECDataReport"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseECDataReport"), bReturn);
	//_________________________________

	//110110.Add_____	
	strTemp.Format((m_bUse1FeederAllMount == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("Use1FeederAllMount"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("Use1FeederAllMount"), bReturn);
	//110110.Add_____	
	strTemp.Format((m_bUseAlignPass == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseAlignPass"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseAlignPass"), bReturn);

	//Online  관련  추가
	strTemp.Format((m_bUseAlignPass == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseSyncMode"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseSyncMode"), bReturn);

	strTemp.Format((m_bUseProductPlanValidation == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseProductPlanValidation"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseProductPlanValidation"), bReturn);

	strTemp.Format((m_bUseTXDataCheckMode == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseTXDataCheckMode"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseTXDataCheckMode"), bReturn);

	//110707_____
	if (!SetValue(strSection,_T("PressActionMode"), m_nPressActionMode))
		DISPLAY_WRITE_ERROR(strSection,_T("PressActionMode"), bReturn);
	if (!SetValue(strSection,_T("Sprocket1ClutchMode"), m_nSprocket1ClutchMode))
		DISPLAY_WRITE_ERROR(strSection,_T("Sprocket1ClutchMode"), bReturn);
	//___________

	//120130.kms______
#ifndef DEF_GATE_SYSTEM
	strTemp.Format((m_bUse4MountAll == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("Use4MountAll"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("Use4MountAll"), bReturn);
#endif

	strTemp.Format((m_bUseIonizerCheck == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("IonizerCheck"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("IonizerCheck"), bReturn);
	//________________

	strTemp.Format((m_bUseTabRemoveAuto == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseTabRemoveAuto"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseTabRemoveAuto"), bReturn);


	strTemp.Format((m_bUseMoldAutoSearch == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseMoldAutoSearch"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseMoldAutoSearch"), bReturn);

	//170201 SJ_YSH
	strTemp.Format((m_bUseCheckVacuumBeforeTabLoading == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseCheckVacuumBeforeTabLoading"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseCheckVacuumBeforeTabLoading"), bReturn);	
	//__________________________//
	
	//170612 JSH.s
	strTemp.Format((m_bUseInspectionTabBolb == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseInspectionTabBolb"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseInspectionTabBolb"), bReturn);
	//170612 JSH.e

	//170718 JSH.s
	strTemp.Format((m_bUse1stPanelAlign == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("m_bUse1stPanelAlign"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("m_bUse1stPanelAlign"), bReturn);
	//170718 JSH.e

	//170927 JSH.s
	strTemp.Format((m_bUseBondingAfterImgLog == TRUE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection, _T("UseBondingAfterImgLog"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("UseBondingAfterImgLog"), bReturn);
	//170927 JSH.e

	/*
	**********************************************************************************************
	** [Material]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), MATERIAL_SECTION);

	/** Mold[1] 전체 사용 횟수 */
	if (!SetValue(strSection,_T("Mold1_TotalCount"), m_uiMold1_TotalCount))
		DISPLAY_WRITE_ERROR(strSection,_T("Mold1_TotalCount"), bReturn);

	//ProgWnd.StepIt();

	/** Mold[2] 전체 사용 횟수 */
	if (!SetValue(strSection,_T("Mold2_TotalCount"), m_uiMold2_TotalCount))
		DISPLAY_WRITE_ERROR(strSection,_T("Mold2_TotalCount"), bReturn);

	//ProgWnd.StepIt();

	/** Mold[1] 세정 후 사용 횟수 */
	if (!SetValue(strSection,_T("Mold1_CurCount"), m_uiMold1_CurCount))
		DISPLAY_WRITE_ERROR(strSection,_T("Mold1_CurCount"), bReturn);

	//ProgWnd.StepIt();

	/** Mold[2] 세정 후 사용 횟수 */
	if (!SetValue(strSection,_T("Mold2_CurCount"), m_uiMold1_CurCount))
		DISPLAY_WRITE_ERROR(strSection,_T("Mold2_CurCount"), bReturn);

	//ProgWnd.StepIt();

	/*
	**********************************************************************************************
	** [TabMounter]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), TABMOUNTER_SECTION);

	for (int i = 0; i < DEF_MAX_TABMOUNTER; i++)
	{
		strItem.Format(_T("ToolOffsetMounter%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdToolOffsetMounter[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("ToolOffsetTabAligner%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdToolOffsetTabAligner[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();
		
/* 삭제 예정
		strItem.Format(_T("UseMounter%d"), i + 1);
		strTemp.Format((m_rgbUseMounter[i] == TRUE) ? _T("TRUE") : _T("FALSE"));
		if (!SetValue(strSection, strItem, strTemp))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();
*/
	}

	if (!SetValue(strSection,_T("MountRetryLimit"), m_iMountRetryLimit))
		DISPLAY_WRITE_ERROR(strSection,_T("MountRetryLimit"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("BondTime"), m_dBondTime))
		DISPLAY_WRITE_ERROR(strSection,_T("BondTime"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("StartMoveDelay"), m_dStartMoveDelay))
		DISPLAY_WRITE_ERROR(strSection,_T("StartMoveDelay"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("2ndGroupMountStartDelay"), m_d2ndGroupMountStartDelay))
		DISPLAY_WRITE_ERROR(strSection,_T("2ndGroupMountStartDelay"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("IFDelayLimit"), m_dIFDelayLimit))
		DISPLAY_WRITE_ERROR(strSection,_T("IFDelayLimit"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("TabMounterStabilizationDelay"), m_dTabMounterStabilizationDelay))
		DISPLAY_WRITE_ERROR(strSection,_T("TabMounterStabilizationDelay"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("MountPosTolerance"), m_dMountPosTolerance))
		DISPLAY_WRITE_ERROR(strSection,_T("MountPosTolerance"), bReturn);

	//140626. SJ_YTH.
	if (!SetValue(strSection,_T("MountMoveSpeed"), m_dMountMoveSpeed))
		DISPLAY_WRITE_ERROR(strSection,_T("MountMoveSpeed"), bReturn);
	//____________________________//

	//ProgWnd.StepIt();

	/*
	**********************************************************************************************
	** [TabFeeder]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), TABFEEDER_SECTION);

	/** 금형 Data 기록 주기 : 설정된 회수만큼의 Panel 배출 후 Data 기록. */
	if (!SetValue(strSection,_T("MoldMaxCount"), m_uiMoldMaxCount))
		DISPLAY_WRITE_ERROR(strSection,_T("MoldMaxCount"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("MoldBlowPeriod"), m_uiMoldBlowPeriod))
		DISPLAY_WRITE_ERROR(strSection,_T("MoldBlowPeriod"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("MoldBlowTime"), m_uiMoldBlowTime))
		DISPLAY_WRITE_ERROR(strSection,_T("MoldBlowTime"), bReturn);

	//ProgWnd.StepIt();

	/** 세정 작업 후 금형 사용 제한 횟수*/
	if (!SetValue(strSection,_T("MoldLimitCountAfterCleaning"), m_uiMoldLimitCountAfterCleaning))
		DISPLAY_WRITE_ERROR(strSection,_T("MoldLimitCountAfterCleaning"), bReturn);

	//ProgWnd.StepIt();

	/** 연속 Bad-Tab IC 제한 횟수*/
	if (!SetValue(strSection,_T("BadTabLimitCount"), m_uiBadTabLimitCount))
		DISPLAY_WRITE_ERROR(strSection,_T("BadTabLimitCount"), bReturn);

	//ProgWnd.StepIt();

	// hongju 수정...20080825
	if (!SetValue(strSection,_T("Sprocket1_F_AutoHoming_Offset"), m_dSprocket1_F_AutoHoming_Offset))
		DISPLAY_WRITE_ERROR(strSection,_T("Sprocket1_F_AutoHoming_Offset"), bReturn);
	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("Sprocket1_R_AutoHoming_Offset"), m_dSprocket1_R_AutoHoming_Offset))
		DISPLAY_WRITE_ERROR(strSection,_T("Sprocket1_R_AutoHoming_Offset"), bReturn);
	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("Sprocket2_F_AutoHoming_Offset"), m_dSprocket2_F_AutoHoming_Offset))
		DISPLAY_WRITE_ERROR(strSection,_T("Sprocket2_F_AutoHoming_Offset"), bReturn);
	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("Sprocket2_R_AutoHoming_Offset"), m_dSprocket2_R_AutoHoming_Offset))
		DISPLAY_WRITE_ERROR(strSection,_T("Sprocket2_R_AutoHoming_Offset"), bReturn);
	//ProgWnd.StepIt();


	//130424.kms_____________
	CString strCalFResult, strCalRResult;
	for (int i = 0; i < 31; i++)
	{
		strCalFResult.Format(_T("CalResult_F#%d"), i);
		if (!SetValue(strSection, strCalFResult, m_dCalResult_F[i]))
			DISPLAY_WRITE_ERROR(strSection, strCalFResult, bReturn);

		strCalRResult.Format(_T("CalResult_R#%d"), i);
		if (!SetValue(strSection, strCalRResult, m_dCalResult_R[i]))
			DISPLAY_WRITE_ERROR(strSection, strCalRResult, bReturn);
	}

	/*
	if (!SetValue(strSection,_T("MoldRegCnt_F"), m_dMoldRegCnt_F))
		DISPLAY_WRITE_ERROR(strSection,_T("MoldRegCnt_F"), bReturn);
	if (!SetValue(strSection,_T("MoldRegCnt_R"), m_dMoldRegCnt_R))
		DISPLAY_WRITE_ERROR(strSection,_T("MoldRegCnt_R"), bReturn);
		*/
	//_______________________


	/*
	**********************************************************************************************
	** [TabCarrier]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), TABCARRIER_SECTION);

	/** Carrier Tab Loading Retry 횟수*/
/* 삭제 예정.
	if (!SetValue(strSection,_T("CarrierRetryCount"), m_iCarrierRetryCount))
		DISPLAY_WRITE_ERROR(strSection,_T("CarrierRetryCount"), bReturn);

	//ProgWnd.StepIt();
*/
	for (int i = 0; i < DEF_MAX_TABCARRIER; i++)
	{
		strItem.Format(_T("CarrierHeadCenterX_%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdCarrierHeadCenterX[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
		strItem.Format(_T("RCarrierHeadCenterX_%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdRCarrierHeadCenterX[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
		strItem.Format(_T("CarrierHeadCenterY_%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdCarrierHeadCenterY[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
		strItem.Format(_T("RCarrierHeadCenterY_%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdRCarrierHeadCenterY[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
	}

	/*
	**********************************************************************************************
	** [Panel Aligner]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), PANELALIGNER_SECTION);

	if (!SetValue(strSection,_T("Distance4DifVel"), m_iDistance4DifVel))
		DISPLAY_WRITE_ERROR(strSection,_T("Distance4DifVel"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("ActiveAlignerPosX"), m_dActiveAlignerPosX))
		DISPLAY_WRITE_ERROR(strSection,_T("ActiveAlignerPosX"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("ActiveAlignerPosY"), m_dActiveAlignerPosY))
		DISPLAY_WRITE_ERROR(strSection,_T("ActiveAlignerPosY"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("OriginDistBetweenAligners"), m_dOriginDistBetweenAligners))
		DISPLAY_WRITE_ERROR(strSection,_T("OriginDistBetweenAligners"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("AutoSearchPanelMarkDistance"), m_dAutoSearchPanelMarkDistance))
		DISPLAY_WRITE_ERROR(strSection,_T("AutoSearchPanelMarkDistance"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("PanelAlignTolerance"), m_dPanelAlignTolerance))
		DISPLAY_WRITE_ERROR(strSection,_T("PanelAlignTolerance"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("PanelAlignRetryLimit"), m_iPanelAlignRetryLimit))
		DISPLAY_WRITE_ERROR(strSection,_T("PanelAlignRetryLimit"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("Backlash"), m_dBacklash))
		DISPLAY_WRITE_ERROR(strSection,_T("Backlash"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("AirPlateBlowOnDelay"), m_dAirPlateBlowOnDelay))
		DISPLAY_WRITE_ERROR(strSection,_T("AirPlateBlowOnDelay"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("PanelAlignerWorkVelocity"), m_dPanelAlignerWorkVelocity))
		DISPLAY_WRITE_ERROR(strSection,_T("PanelAlignerWorkVelocity"), bReturn);

	//ProgWnd.StepIt();
	if (!SetValue(strSection,_T("AlignCam1_FixedPos_X"), m_dAlignCam1_FixedPos_X))
		DISPLAY_WRITE_ERROR(strSection,_T("AlignCam1_FixedPos_X"), bReturn);

	if (!SetValue(strSection,_T("AlignCam1_FixedPos_Y"), m_dAlignCam1_FixedPos_Y))
		DISPLAY_WRITE_ERROR(strSection,_T("AlignCam1_FixedPos_Y"), bReturn);

	if (!SetValue(strSection,_T("AlignCam2_FixedPos_X"), m_dAlignCam2_FixedPos_X))
		DISPLAY_WRITE_ERROR(strSection,_T("AlignCam2_FixedPos_X"), bReturn);

	if (!SetValue(strSection,_T("AlignCam2_FixedPos_Y"), m_dAlignCam2_FixedPos_Y))
		DISPLAY_WRITE_ERROR(strSection,_T("AlignCam2_FixedPos_Y"), bReturn);

	if (!SetValue(strSection,_T("CamCarrier1_FixedPosX"), m_dCamCarrier1_FixedPosX))
		DISPLAY_WRITE_ERROR(strSection,_T("CamCarrier1_FixedPosX"), bReturn);

	if (!SetValue(strSection,_T("CamCarrier2_FixedPosX"), m_dCamCarrier2_FixedPosX))
		DISPLAY_WRITE_ERROR(strSection,_T("CamCarrier2_FixedPosX"), bReturn);


	if (!SetValue(strSection,_T("DistBetweenAligners"), m_dDistBetweenAligners))
		DISPLAY_WRITE_ERROR(strSection,_T("DistBetweenAligners"), bReturn);

	/*/
#ifdef DEF_GATE_SYSTEM
	if (!SetValue(strSection,_T("Dist_AlignerToRotator_X"), m_dDist_AlignerToRotator_X))
		DISPLAY_WRITE_ERROR(strSection,_T("Dist_AlignerToRotator_X"), bReturn);

	if (!SetValue(strSection,_T("Dist_AlignerToRotator_Y"), m_dDist_AlignerToRotator_Y))
		DISPLAY_WRITE_ERROR(strSection,_T("Dist_AlignerToRotator_Y"), bReturn);
#endif
	/*/

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("DistPosInspectCamera%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dDistPosInspectCamera[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
		
		strItem.Format(_T("DistNegInspectCamera%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dDistNegInspectCamera[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
	}

	if (!SetValue(strSection, _T("InspectCamFixedPosWidth"), m_dInspectCamFixedPosWidth))
		DISPLAY_WRITE_ERROR(strSection, _T("InspectCamFixedPosWidth"), bReturn);

	/*
	**********************************************************************************************
	** [Model Change Distance]
	**********************************************************************************************
	*/
	/*
	strSection.Format(_T("%s"), MODEL_CHANGE_DIST_SECTION);
	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("ModelChangeOriginDist%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dModelChangeOriginDist[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("DistBetweenGantryHomeToModelChangeHome%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dDistBetweenGantryHomeToModelChangeHome[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
	}
	*/
	m_dModelChangeOriginDist[0] = m_dAlignCam1_FixedPos_Y;
	m_dModelChangeOriginDist[1] = m_dAlignCam2_FixedPos_Y;

	strSection.Format(_T("%s"), MODEL_CHANGE_DIST_SECTION);
	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("GantryOriginDist%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dGantryOriginDist[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
	}

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("BadTabOriginDist%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dDistBadTabOriginDist[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
	}

	//ProgWnd.StepIt();

	/*
	**********************************************************************************************
	** [TabAligner]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), TABALIGNER_SECTION);

	for (int i = 0; i < DEF_MAX_WORKER_GROUP; i++)
	{
		strItem.Format(_T("TabCameraDist1_TabMounterGroup%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dTabCameraDist1[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabCameraDist2_TabMounterGroup%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dTabCameraDist2[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabCameraDist3_TabMounterGroup%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dTabCameraDist3[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabCameraDist4_TabMounterGroup%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dTabCameraDist4[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();
	}

	if (!SetValue(strSection,_T("TabICLeadCheck"), m_dTabICLeadCheck))
		DISPLAY_WRITE_ERROR(strSection,_T("TabICLeadCheck"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("TabAlignerOperationLimit"), m_iTabAlignerOperationLimit))
		DISPLAY_WRITE_ERROR(strSection,_T("TabAlignerOperationLimit"), bReturn);

	//ProgWnd.StepIt();

	if (!SetValue(strSection,_T("TabAlignerTolerance"), m_dTabAlignerTolerance))
		DISPLAY_WRITE_ERROR(strSection,_T("TabAlignerTolerance"), bReturn);

	//ProgWnd.StepIt();

	for (int i = 0; i < DEF_MAX_TABALIGNER; i++)
	{
		strItem.Format(_T("UseTabAligner%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgbUseTabAligner[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabAligner%d_Base1"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdTabAlignerBase1[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabAligner%d_Base2"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdTabAlignerBase2[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabAligner%d_EtcOffset1"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdTabAlignerEtcOffset1[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();

		strItem.Format(_T("TabAligner%d_EtcOffset2"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdTabAlignerEtcOffset2[i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();
	}

	/*
	**********************************************************************************************
	** [Camera Prism]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), CAMERA_PRISM_SECTION);
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		strItem.Format(_T("InspectionCamDistBetweenPrismViewCenterForCam%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdInspectionCamDistViewCenter_Expand[0][i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("InspectionCamDistViewCenter_Shrink%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdInspectionCamDistViewCenter_Shrink[0][i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("InspectionCamDist_CenterToCenter%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdInspectionCamDist_CenterToCenter[0][i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
		//ProgWnd.StepIt();
	}

//	strSection.Format(_T("%s"), R_CAMERA_PRISM_SECTION);
	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
	{
		strItem.Format(_T("RInspectionCamDistBetweenPrismViewCenterForCam%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdInspectionCamDistViewCenter_Expand[1][i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("RInspectionCamDistViewCenter_Shrink%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdInspectionCamDistViewCenter_Shrink[1][i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("RInspectionCamDist_CenterToCenter%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdInspectionCamDist_CenterToCenter[1][i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
		//ProgWnd.StepIt();
	}

//#ifdef DEF_SOURCE_SYSTEM
//	for (int i = 0; i < DEF_MAX_INSPECTION_CAM_CARRIER; i++)
//	{
//		strItem.Format(_T("VariableDistForInspectionCam%d"), i + 1);
//		if (!SetValue(strSection, strItem, m_rgdInspectionCamVariableDist[i]))
//			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
//
//		//ProgWnd.StepIt();
//	}
//#endif

	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("PreAlignCamDistBetweenPrismViewCenter%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdPreAlignCamDistViewCenter_Expand[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("PreAlignCamDistViewCenter_Shrink%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdPreAlignCamDistViewCenter_Shrink[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		strItem.Format(_T("PreAlignCamDist_CenterToCenter%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdPreAlignCamDist_CenterToCenter[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);
		//ProgWnd.StepIt();
	}
	for (int i = 0; i < DEF_MAX_GROUP; i++)
	{
		strItem.Format(_T("PrealignCamVariableDist%d"), i + 1);
		if (!SetValue(strSection, strItem, m_rgdPrealignCamVariableDist[i]))
			DISPLAY_READ_ERROR(strSection, strItem, bReturn);

		//ProgWnd.StepIt();
	}

	for (int i = 0; i < DEF_MAX_CAM_EXPAND; i++)
	{
		strItem.Format(_T("Data_CameraExpand1_%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dData_CameraExpand[0][i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
	}
	for (int i = 0; i < DEF_MAX_CAM_EXPAND; i++)
	{
		strItem.Format(_T("Data_CameraExpand2_%d"), i + 1);
		if (!SetValue(strSection, strItem, m_dData_CameraExpand[1][i]))
			DISPLAY_WRITE_ERROR(strSection, strItem, bReturn);
	}


	/*
	**********************************************************************************************
	** [Log]
	**********************************************************************************************
	*/
	strSection.Format(_T("%s"), LOG_SECTION);

	strTemp.Format((m_bLogLevelError != FALSE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("LogLevelError"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("LogLevelError"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bLogLevelMonitoring != FALSE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("LogLevelWarning"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("LogLevelWarning"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bLogLevelNormal != FALSE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("LogLevelNormal"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("LogLevelNormal"), bReturn);

	//ProgWnd.StepIt();

	strTemp.Format((m_bLogLevelTactTime != FALSE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("LogLevelTactTime"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("LogLevelTactTime"), bReturn);

	//ProgWnd.StepIt();

//170427 JSH.s
	strTemp.Format((m_bLogLevelEqTactTime != FALSE) ? _T("TRUE") : _T("FALSE"));
	if (!SetValue(strSection,_T("LogLevelEqTactTime"), strTemp))
		DISPLAY_WRITE_ERROR(strSection,_T("LogLevelEqTactTime"), bReturn);
	
	//ProgWnd.StepIt();
//170427 JSH.e

	if (!SetValue(strSection,_T("LogKeepingDays"), m_iLogKeepingDays))
		DISPLAY_WRITE_ERROR(strSection,_T("LogKeepingDays"), bReturn);

	//ProgWnd.StepIt();

	//100121 SJ_YYK EQ_STOP_DATA
	CString strError;

	//100121 SJ_YYK EQ_STOP_DATA
	strSection.Format(_T("%s"), EQSTOP_SECTION);
	for(int j =0; j<DEF_STOP_MAX_ITEM;j++)
	{
		// EqStop Code
		strError.Format(_T("StopCode#%d"), j);
		if(!SetValue(strSection, strError, m_strStopCode[j]))
		{
			DISPLAY_WRITE_ERROR(strSection,_T("StopCode#%d"), bReturn);
		}
	}
	
	for (int j =0; j<DEF_STOP_MAX_ITEM;j++)
	{
		strError.Format(_T("StopItem#%d"), j);
		if(!SetValue(strSection, strError, m_strStopItem[j]))
		{
			DISPLAY_WRITE_ERROR(strSection,_T("StopItem#%d"), bReturn);
		}
	}

	//110624._______________________________	
	strSection.Format(_T("%s"), PRODUCT_CHECK_SECTION);

	//CString strItem;
	for( int p=0; p<DEF_VALIDATION_MAX_ITEM ; p++)
	{
		// EqStop Code
		strError.Format(_T("ValidationCheck#%d"), p);
		if(!SetValue(strSection, strError, m_bValidationCheckSelect[p]))
		{
			DISPLAY_READ_ERROR(strSection,_T("ValidationCheck#%d"), bReturn);
		}
	}
	//_______________________________________


	//110705.kms________________
	strSection.Format(_T("%s"), MODULEPORT_SECTION);

	if (!SetValue(strSection, "EQPID", m_strEQPID))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("EQPID"), bReturn);
//		ReadDisplayMessage(MY_MESSAGE_BOX, _T("Message_10461"), &szTextTemp1);
//		szTextTemp2.Format(_T("[%s] %s"),strSection,szTextTemp1);
//		szTitleTemp.Format(_T("[%s] File Write Error"),m_strFileName);
//		if (MyMessageBox(szTextTemp2, (LPCTSTR)szTitleTemp, M_ICONERROR|M_YESNO) == IDNO) return FALSE;
//		ret = FALSE;
	}

	for(int m =0; m<DEF_MODULE_PORT_MAX_ID;m++)
	{
		strError.Format(_T("ModulePortID#%d"), m);
		if(!SetValue(strSection, strError, m_strModulePortID[m]))
		{
			DISPLAY_WRITE_ERROR(strSection,_T("ModulePortID#%d"), bReturn);
		}
	}

	if (!SetValue(strSection, _T("ModuleID"), m_strModuleID))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("ModuleID"), bReturn);
	}

	if (!SetValue(strSection, _T("ModuleID2"), m_strModuleID2))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("ModuleID2"), bReturn);
	}

	if (!SetValue(strSection, _T("ModuleID3_1"), m_strModuleID3_1))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("ModuleID3_1"), bReturn);
	}

	if (!SetValue(strSection, _T("ModuleID3_2"), m_strModuleID3_2))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("ModuleID3_2"), bReturn);
	}

	if (!SetValue(strSection, _T("ModuleID3_3"), m_strModuleID3_3))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("ModuleID3_3"), bReturn);
	}

	if (!SetValue(strSection, _T("ModuleID3_4"), m_strModuleID3_4))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("ModuleID3_4"), bReturn);
	}

	if (!SetValue(strSection, _T("ReportModuelID"), m_strReportModuelID))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("ReportModuelID"), bReturn);
	}

	if (!SetValue(strSection, _T("ReportModuleLevel"), m_iReportModuleLevel))
	{
		DISPLAY_WRITE_ERROR(strSection,_T("ReportModuleLevel"), bReturn);
	}
	//_________________________

	//20110914 SJ_HJG
	strSection.Format(_T("%s"), COMMON_SECTION);
	if (!SetValue(strSection, _T("EmptyElapseTime"), m_dEmptyElapseTime))
		DISPLAY_WRITE_ERROR(strSection,_T("EmptyElapseTime"), bReturn);

	//20111205 sj_hjg
	if (!SetValue(strSection, _T("TabOffsetdata"), m_dTabOffsetdata))
		DISPLAY_WRITE_ERROR(strSection,_T("TabOffsetdata"), bReturn);

	if (!SetValue(strSection, _T("TabOffsetAverage"), m_dTabOffsetAverage))
		DISPLAY_WRITE_ERROR(strSection,_T("TabOffsetAverage"), bReturn);

	strSection.Format(_T("%s"), FUNCTION_SECTION);
	if (!SetValue(strSection, _T("UseAutoAlignSystem"), m_bUseAutoAlignSystem))
		DISPLAY_WRITE_ERROR(strSection,_T("UseAutoAlignSystem"), bReturn);
	//________________________

	if (!SetValue(strSection, _T("TabXCheck"), m_dTabXCheck))
		DISPLAY_WRITE_ERROR(strSection,_T("TabXCheck"), bReturn);

	if (!SetValue(strSection, _T("TabYCheck"), m_dTabYCheck))
		DISPLAY_WRITE_ERROR(strSection,_T("TabYCheck"), bReturn);

	//130320.kms_________
	strSection.Format(_T("%s"), ERRORNOTSEND_SECTION);
	//CString strItem;
	for (int j=0; j<DEF_MAX_ERROR_NOT_SEND ; j++)
	{
		strError.Format(_T("ErrorNo#%d"), j);
		if(!SetValue(strSection, strError, m_strErrorNo[j]))
		{
			DISPLAY_WRITE_ERROR(strSection,_T("ErrorNo%d"), bReturn);
		}
	}
	//___________________

	return bReturn;
}

BOOL MSystemData::WriteRunModeData()
{
	BOOL bReturn = TRUE;
	CString strTemp;

	if (m_eRunMode == PASS_RUN_MODE)
		strTemp.Format(_T("PASS"));
	else if (m_eRunMode == DRY_RUN_MODE)
		strTemp.Format(_T("DRY"));
	else if (m_eRunMode == REPAIR_RUN_MODE)
		strTemp.Format(_T("REPAIR"));
	else
		strTemp.Format(_T("NORMAL"));

	if (!SetValue(COMMON_SECTION, _T("RunMode"), strTemp))
		DISPLAY_WRITE_ERROR(COMMON_SECTION, _T("RunMode"), bReturn);

	return bReturn;
}
