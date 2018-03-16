#include "stdafx.h"
#include "common.h"
#include "MProductInfoData.h"
#include "MyProgressWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MProductInfoData::MProductInfoData(CString strFileName)	: MFileManager(strFileName)
{
	Initialize();
}

MProductInfoData::MProductInfoData(CString strFileName, CString strPath)
	: MFileManager(strFileName, strPath)
{
	Initialize();
}

MProductInfoData::~MProductInfoData()
{
}

/** Varaible 초기화 */
void MProductInfoData::Initialize()
{
	m_iPanelInfoIndex		= 0;

	m_iProductCount			= 0;
	
	// Doolin kwangilshin 2017. 09. 12.
	m_nPanelInputQty		= 0;
	m_nPanelScrapQty		= 0;
	// End

	m_iPreHandQty			= 200;
	m_iWarningQty			= 100;

	m_dEquipTact			= 0.0;

	for (int i = 0; i < DEF_MAX_QUEUE_SIZE; i++)
	{
		m_rgsPanelInfoQueue[i].m_dLineTact = 0.0;
		m_rgsPanelInfoQueue[i].m_dMachineTact = 0.0;
		m_rgsPanelInfoQueue[i].m_strPanelID.Empty();
	}

	for (int i = 0 ; i < DEF_MAX_TABFEEDER ; i++)
	{
		for (int j = 0 ; j < DEF_MAX_REEL ; j++)
			m_matTabICInfo[i][j].ResetData();

		m_matZigInfo[i].ResetData();
	}

	//110629____________
	for (int i = 0 ; i < DEF_MAX_PRODUCT_PLAN ; i++)
	{
		m_MaterialCodeInfo_S[i].ResetData();
		m_MaterialCodeInfo_G[i].ResetData();
	}

	m_CurProductBatchInfo.Clear();
	for (int i=0; i<DEF_MAX_PRODUCT_PLAN_CNT; i++)
	{
		m_NextProductBatchInfo[i].Clear();
		m_EndProductBatchInfo[i].Clear();
	}
	//__________________

	m_iPanelTurnStatus = 0;
}

/** INI 형식의 Text 파일에서 Data를 읽는다 */
BOOL MProductInfoData::ReadData()
{
	BOOL bReturn = TRUE;

	if (!GetValue("Total", "ProductQuantity", &m_iProductCount))
		DISPLAY_READ_ERROR("Total", "ProductQuantity", bReturn);
	
	// Doolin kwangilshin 2017. 09. 12.
	//
	if (!GetValue(_T("Total"), _T("PanelInputQty"), &m_nPanelInputQty))
		DISPLAY_READ_ERROR(_T("Total"), _T("PanelInputQty"), bReturn);

	if (!GetValue(_T("Total"), _T("PanelScrapQty"), &m_nPanelScrapQty))
		DISPLAY_READ_ERROR(_T("Total"), _T("PanelScrapQty"), bReturn);
	// End

	return bReturn;
}

BOOL MProductInfoData::ReadMaterialData()
{
	BOOL	bReturn = TRUE;
	CString	strSection;
	CString	strTemp;

	strSection = "Material Data";
	if (!GetValue(strSection, "m_iPreHandQty", &m_iPreHandQty))
		DISPLAY_READ_ERROR(strSection, "m_iPreHandQty", bReturn);

	if (!GetValue(strSection, "m_iWarningQty", &m_iWarningQty))
		DISPLAY_READ_ERROR(strSection, "m_iWarningQty", bReturn);

	/** TabIC Data */
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		for (int j = 0 ; j < DEF_MAX_REEL ; j++)
		{
			strSection.Format(_T("TabFeeder%d_Reel%d"), i, j);

			if (!GetValue(strSection, "m_matTabICInfo.m_sM_MAKER", &m_matTabICInfo[i][j].m_sM_MAKER))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_sM_MAKER", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_sM_BATCHID", &m_matTabICInfo[i][j].m_sM_BATCHID))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_sM_BATCHID", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_nM_TYPE", &m_matTabICInfo[i][j].m_nM_TYPE))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_nM_TYPE", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_sM_STEP", &m_matTabICInfo[i][j].m_sM_STEP))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_sM_STEP", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_sM_CODE", &m_matTabICInfo[i][j].m_sM_CODE))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_sM_CODE", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_nM_STATE", &m_matTabICInfo[i][j].m_nM_STATE))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_nM_STATE", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_nT_QTY", &m_matTabICInfo[i][j].m_nT_QTY))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_nT_QTY", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_nU_QTY", &m_matTabICInfo[i][j].m_nU_QTY))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_nU_QTY", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_nN_QTY", &m_matTabICInfo[i][j].m_nN_QTY))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_nN_QTY", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_nR_QTY", &m_matTabICInfo[i][j].m_nR_QTY))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_nR_QTY", bReturn);

			if (!GetValue(strSection, "m_matTabICInfo.m_nR_QTY", &m_matTabICInfo[i][j].m_nR_QTY))
				DISPLAY_READ_ERROR(strSection, "m_matTabICInfo.m_nR_QTY", bReturn);
		}
	}

	/** Zig Data */
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		strSection.Format(_T("Zig%d"), i);

		if (!GetValue(strSection, "m_matZigInfo.m_sM_MAKER", &m_matZigInfo[i].m_sM_MAKER))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_sM_MAKER", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_sM_BATCHID", &m_matZigInfo[i].m_sM_BATCHID))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_sM_BATCHID", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_nM_TYPE", &m_matZigInfo[i].m_nM_TYPE))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_nM_TYPE", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_sM_STEP", &m_matZigInfo[i].m_sM_STEP))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_sM_STEP", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_sM_CODE", &m_matZigInfo[i].m_sM_CODE))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_sM_CODE", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_nM_STATE", &m_matZigInfo[i].m_nM_STATE))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_nM_STATE", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_nT_QTY", &m_matZigInfo[i].m_nT_QTY))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_nT_QTY", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_nU_QTY", &m_matZigInfo[i].m_nU_QTY))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_nU_QTY", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_nN_QTY", &m_matZigInfo[i].m_nN_QTY))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_nN_QTY", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_nR_QTY", &m_matZigInfo[i].m_nR_QTY))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_nR_QTY", bReturn);

		if (!GetValue(strSection, "m_matZigInfo.m_nR_QTY", &m_matZigInfo[i].m_nR_QTY))
			DISPLAY_READ_ERROR(strSection, "m_matZigInfo.m_nR_QTY", bReturn);
	}

	return bReturn;
}

BOOL MProductInfoData::ReadGlassData()
{
	BOOL	bReturn = TRUE;
	//100713.KKY_____
//	CString	strSection[] = { "LOAD CONVEYOR", "PANEL TRANSFER", "PANEL ALIGNER", "UNLOAD CONVEYOR" };
	CString	strSection[] = { "TRANSFER CENTER", "TURN HANDLER", "TRANSFER IN", "PANEL ALIGNER", "TRANSFER OUT" };
	//_______________
	CString	strTemp;

	for (int i=0; i<DEF_MAX_PANEL_UNIT; i++)
	{
		if (!GetValue(strSection[i], "m_strHGlassID", &m_Glass[i].m_strHPanelID))
			DISPLAY_READ_ERROR(strSection[i], "m_strHGlassID", bReturn);

		if (!GetValue(strSection[i], "m_strEGlassID", &m_Glass[i].m_strEPanelID))
			DISPLAY_READ_ERROR(strSection[i], "m_strEGlassID", bReturn);

		if (!GetValue(strSection[i], "m_strBatchID", &m_Glass[i].m_strBatchID))
			DISPLAY_READ_ERROR(strSection[i], "m_strBatchID", bReturn);

		if (!GetValue(strSection[i], "m_strProdType", &m_Glass[i].m_strProdType))
			DISPLAY_READ_ERROR(strSection[i], "m_strProdType", bReturn);

		if (!GetValue(strSection[i], "m_strProdKind", &m_Glass[i].m_strProdKind))
			DISPLAY_READ_ERROR(strSection[i], "m_strProdKind", bReturn);

		if (!GetValue(strSection[i], "m_strDeviceID", &m_Glass[i].m_strDeviceID))
			DISPLAY_READ_ERROR(strSection[i], "m_strDeviceID", bReturn);

		if (!GetValue(strSection[i], "m_strStepID", &m_Glass[i].m_strStepID))
			DISPLAY_READ_ERROR(strSection[i], "m_strStepID", bReturn);

		if (!GetValue(strSection[i], "m_strPPID", &m_Glass[i].m_strPPID))
			DISPLAY_READ_ERROR(strSection[i], "m_strPPID", bReturn);

		if (!GetValue(strSection[i], "m_strThickness", &m_Glass[i].m_strThickness))
			DISPLAY_READ_ERROR(strSection[i], "m_strThickness", bReturn);

		if (!GetValue(strSection[i], "m_strInsFlag", &m_Glass[i].m_strInsFlag))
			DISPLAY_READ_ERROR(strSection[i], "m_strInsFlag", bReturn);

		if (!GetValue(strSection[i], "m_strPanelSize", &m_Glass[i].m_strPanelSize))
			DISPLAY_READ_ERROR(strSection[i], "m_strPanelSize", bReturn);

		if (!GetValue(strSection[i], "m_strPanelPosition", &m_Glass[i].m_strPanelPosition))
			DISPLAY_READ_ERROR(strSection[i], "m_strPanelPosition", bReturn);

		if (!GetValue(strSection[i], "m_strCount1", &m_Glass[i].m_strCount1))
			DISPLAY_READ_ERROR(strSection[i], "m_strCount1", bReturn);

		if (!GetValue(strSection[i], "m_strCount2", &m_Glass[i].m_strCount2))
			DISPLAY_READ_ERROR(strSection[i], "m_strCount2", bReturn);

		if (!GetValue(strSection[i], "m_strGrade", &m_Glass[i].m_strGrade))
			DISPLAY_READ_ERROR(strSection[i], "m_strGrade", bReturn);

		if (!GetValue(strSection[i], "m_strComment", &m_Glass[i].m_strComment))
			DISPLAY_READ_ERROR(strSection[i], "m_strComment", bReturn);

		if (!GetValue(strSection[i], "m_strCompSize", &m_Glass[i].m_strCompSize))
			DISPLAY_READ_ERROR(strSection[i], "m_strCompSize", bReturn);

		if (!GetValue(strSection[i], "m_strReadingFlag", &m_Glass[i].m_strReadingFlag))
			DISPLAY_READ_ERROR(strSection[i], "m_strReadingFlag", bReturn);

		if (!GetValue(strSection[i], "m_strPanelState", &m_Glass[i].m_strPanelState))
			DISPLAY_READ_ERROR(strSection[i], "m_strPanelState", bReturn);

		if (!GetValue(strSection[i], "m_strJudgement", &m_Glass[i].m_strJudgement))
			DISPLAY_READ_ERROR(strSection[i], "m_strJudgement", bReturn);

		if (!GetValue(strSection[i], "m_strCode", &m_Glass[i].m_strCode))
			DISPLAY_READ_ERROR(strSection[i], "m_strCode", bReturn);

		if (!GetValue(strSection[i], "m_strRunLine", &m_Glass[i].m_strRunLine))
			DISPLAY_READ_ERROR(strSection[i], "m_strRunLine", bReturn);

		if (!GetValue(strSection[i], "m_strUniqueId", &m_Glass[i].m_strUniqueId))
			DISPLAY_READ_ERROR(strSection[i], "m_strUniqueId", bReturn);

		if (!GetValue(strSection[i], "m_strPairHPanelID", &m_Glass[i].m_strPairHPanelID))
			DISPLAY_READ_ERROR(strSection[i], "m_strPairHPanelID", bReturn);

		if (!GetValue(strSection[i], "m_strPairEPanelID", &m_Glass[i].m_strPairEPanelID))
			DISPLAY_READ_ERROR(strSection[i], "m_strPairEPanelID", bReturn);

		if (!GetValue(strSection[i], "m_strPairGrade", &m_Glass[i].m_strPairGrade))
			DISPLAY_READ_ERROR(strSection[i], "m_strPairGrade", bReturn);

		if (!GetValue(strSection[i], "m_strPairUniqueId", &m_Glass[i].m_strPairUniqueId))
			DISPLAY_READ_ERROR(strSection[i], "m_strPairUniqueId", bReturn);

		if (!GetValue(strSection[i], "m_strFlowRecipe", &m_Glass[i].m_strFlowRecipe))
			DISPLAY_READ_ERROR(strSection[i], "m_strFlowRecipe", bReturn);

		if (!GetValue(strSection[i], "m_strReserved0", &m_Glass[i].m_strReserved0))
			DISPLAY_READ_ERROR(strSection[i], "m_strReserved0", bReturn);

		if (!GetValue(strSection[i], "m_strBitsSignals", &m_Glass[i].m_strBitsSignals))
			DISPLAY_READ_ERROR(strSection[i], "m_strBitsSignals", bReturn);

		if (!GetValue(strSection[i], "m_strReferData", &m_Glass[i].m_strReferData))
			DISPLAY_READ_ERROR(strSection[i], "m_strReferData", bReturn);
	}

	return bReturn;
}

BOOL MProductInfoData::ReadStatusData()
{
	BOOL	bReturn = TRUE;
	CString	strSection;
	strSection = "Status Data";
	if (!GetValue(strSection, "iPanelTurnStatus", &m_iPanelTurnStatus))
		DISPLAY_READ_ERROR(strSection, "iPanelTurnStatus", bReturn);

	return bReturn;
}

//110620.KKY.Add_____________
BOOL MProductInfoData::ReadProductPlan()
{
	BOOL	bReturn = TRUE;
	//110629.Modify_______________
	MProductPlanBatchData *pInfo[DEF_MAX_PRODUCT_PLAN_CNT*2+1] =
	{
		&m_CurProductBatchInfo, &m_NextProductBatchInfo[0], &m_NextProductBatchInfo[1], &m_NextProductBatchInfo[2], &m_NextProductBatchInfo[3], &m_NextProductBatchInfo[4],
								&m_EndProductBatchInfo[0], &m_EndProductBatchInfo[1], &m_EndProductBatchInfo[2], &m_EndProductBatchInfo[3], &m_EndProductBatchInfo[4]
	};

	CString	strSection[DEF_MAX_PRODUCT_PLAN_CNT*2+1] = 
	{
		_T("Current Product Plan"), _T("Next Product Plan1"), _T("Next Product Plan2"), _T("Next Product Plan3"), _T("Next Product Plan4"), _T("Next Product Plan5"), 
								_T("Ended Product Plan1"), _T("Ended Product Plan2"), _T("Ended Product Plan3"), _T("Ended Product Plan4"), _T("Ended Product Plan5")
	};
	CString	strTemp;
	int i = 0;

	for (int i=0; i<DEF_MAX_PRODUCT_PLAN_CNT*2+1; i++)
	{
		if (!GetValue(strSection[i], "m_bRunFlag", &strTemp))
			DISPLAY_READ_ERROR(strSection[i], "m_bRunFlag", bReturn);

		if (strTemp == "TRUE")		pInfo[i]->m_bRunFlag = TRUE;
		else						pInfo[i]->m_bRunFlag = FALSE;

		if (!GetValue(strSection[i], "m_sModuleID", &pInfo[i]->m_sModuleID))
			DISPLAY_READ_ERROR(strSection[i], "m_sModuleID", bReturn);

		if (!GetValue(strSection[i], "m_nPlanOrderID", &pInfo[i]->m_nPlanOrderID))
			DISPLAY_READ_ERROR(strSection[i], "m_nPlanOrderID", bReturn);

		if (!GetValue(strSection[i], "m_nPriorty", &pInfo[i]->m_nPriorty))
			DISPLAY_READ_ERROR(strSection[i], "m_nPriorty", bReturn);

		if (!GetValue(strSection[i], "m_sProdKind", &pInfo[i]->m_sProdKind))
			DISPLAY_READ_ERROR(strSection[i], "m_sProdKind", bReturn);

		if (!GetValue(strSection[i], "m_sProdType", &pInfo[i]->m_sProdType))
			DISPLAY_READ_ERROR(strSection[i], "m_sProdType", bReturn);

		if (!GetValue(strSection[i], "m_sDeviceID", &pInfo[i]->m_sDeviceID))
			DISPLAY_READ_ERROR(strSection[i], "m_sDeviceID", bReturn);

		if (!GetValue(strSection[i], "m_sStepID", &pInfo[i]->m_sStepID))
			DISPLAY_READ_ERROR(strSection[i], "m_sStepID", bReturn);

		if (!GetValue(strSection[i], "m_sPPID", &pInfo[i]->m_sPPID))
			DISPLAY_READ_ERROR(strSection[i], "m_sPPID", bReturn);

		if (!GetValue(strSection[i], "m_sBatchID", &pInfo[i]->m_sBatchID))
			DISPLAY_READ_ERROR(strSection[i], "m_sBatchID", bReturn);

		if (!GetValue(strSection[i], "m_nBatchSize", &pInfo[i]->m_nBatchSize))
			DISPLAY_READ_ERROR(strSection[i], "m_nBatchSize", bReturn);

		if (!GetValue(strSection[i], "m_nBatchState", &pInfo[i]->m_nBatchState))
			DISPLAY_READ_ERROR(strSection[i], "m_nBatchState", bReturn);

		if (!GetValue(strSection[i], "m_sPanelMaker", &pInfo[i]->m_sPanelMaker))
			DISPLAY_READ_ERROR(strSection[i], "m_sPanelMaker", bReturn);

		if (!GetValue(strSection[i], "m_nPanelThickness", &pInfo[i]->m_nPanelThickness))
			DISPLAY_READ_ERROR(strSection[i], "m_nPanelThickness", bReturn);

		if (!GetValue(strSection[i], "m_sFirstGlassID", &pInfo[i]->m_sFirstGlassID))
			DISPLAY_READ_ERROR(strSection[i], "m_sFirstGlassID", bReturn);

		if (!GetValue(strSection[i], "m_sCurGlassID", &pInfo[i]->m_sCurGlassID))
			DISPLAY_READ_ERROR(strSection[i], "m_sCurGlassID", bReturn);

		if (!GetValue(strSection[i], "m_sInputLineNo", &pInfo[i]->m_sInputLineNo))
			DISPLAY_READ_ERROR(strSection[i], "m_sInputLineNo", bReturn);

		if (!GetValue(strSection[i], "m_nValidFlag", &pInfo[i]->m_nValidFlag))
			DISPLAY_READ_ERROR(strSection[i], "m_nValidFlag", bReturn);

		if (!GetValue(strSection[i], "m_nCQTY", &pInfo[i]->m_nCQTY))
			DISPLAY_READ_ERROR(strSection[i], "m_nCQTY", bReturn);

		if (!GetValue(strSection[i], "m_nOQTY", &pInfo[i]->m_nOQTY))
			DISPLAY_READ_ERROR(strSection[i], "m_nOQTY", bReturn);

		if (!GetValue(strSection[i], "m_nRQTY", &pInfo[i]->m_nRQTY))
			DISPLAY_READ_ERROR(strSection[i], "m_nRQTY", bReturn);

		if (!GetValue(strSection[i], "m_nNQTY", &pInfo[i]->m_nNQTY))
			DISPLAY_READ_ERROR(strSection[i], "m_nNQTY", bReturn);
	}

	return bReturn;
}

BOOL MProductInfoData::ReadMaterialCodeInfo()
{
	BOOL	bReturn = TRUE;
	CString	strSection[] = { "Current Product Plan", "Next Product Plan", "Ended Product Plan" };
	CString	strTemp;
	
	for (int i=0; i<DEF_MAX_PRODUCT_PLAN; i++)
	{
		if (!GetValue(strSection[i], "m_nWorkOrder", &m_MaterialCodeInfo_S[i].m_nWorkOrder))
		{
			m_MaterialCodeInfo_S[i].m_nWorkOrder = 0;
			DISPLAY_READ_ERROR(strSection[i], "m_nWorkOrder", bReturn);
		}

		if (!GetValue(strSection[i], "m_sDeviceID", &m_MaterialCodeInfo_S[i].m_sDeviceID))
			DISPLAY_READ_ERROR(strSection[i], "m_sDeviceID", bReturn);

		m_MaterialCodeInfo_G[i].m_nWorkOrder = m_MaterialCodeInfo_S[i].m_nWorkOrder;
		m_MaterialCodeInfo_G[i].m_sDeviceID = m_MaterialCodeInfo_S[i].m_sDeviceID;

		m_MaterialCodeInfo_S[i].m_nM_KIND = eMatKind_TABIC;
		m_MaterialCodeInfo_S[i].m_sM_STEP = DEF_MCODE_STEP_S;	
		for (int j=0; j<DEF_MAX_MCODE; j++)
		{
			CString strValue;
			strTemp.Format(_T("MCodeS_%d"), i);
			if (!GetValue(strSection[i], strTemp, &strValue))
				DISPLAY_READ_ERROR(strSection[i], strTemp, bReturn);

			if (FALSE == strValue.IsEmpty())
				m_MaterialCodeInfo_S[i].m_asM_CODE.Add(strValue);
			else
				break;
		}

		m_MaterialCodeInfo_G[i].m_nM_KIND = eMatKind_TABIC;
		m_MaterialCodeInfo_G[i].m_sM_STEP = DEF_MCODE_STEP_G;
		for (int j=0; j<DEF_MAX_MCODE; j++)
		{
			CString strValue;
			strTemp.Format(_T("MCodeG_%d"), i);
			if (!GetValue(strSection[i], strTemp, &strValue))
				DISPLAY_READ_ERROR(strSection[i], strTemp, bReturn);

			if (FALSE == strValue.IsEmpty())
				m_MaterialCodeInfo_G[i].m_asM_CODE.Add(strValue);
			else
				break;
		}

	}
	return bReturn;
}
//___________________________

BOOL MProductInfoData::WriteData()
{
	if (!MakeBackUpFile())
	{
		//Back Up File을 만드는데 실패 했습니다. 계속 진행 하겠습니까?
		if (MyMessageBox(MY_MESSAGE_BOX,10000, _T("File Write Error"), M_ICONERROR|M_YESNO) == IDNO) 
			return FALSE;
	}

	BOOL bReturn = TRUE;

	if (!SetValue("Total", "ProductQuantity ", m_iProductCount, FALSE))
		bReturn = FALSE;

	// Doolin kwangilshin 2017. 09. 12.
	//
	if (!SetValue(_T("Total"), _T("PanelInputQty"), m_nPanelInputQty), FALSE)
		bReturn = FALSE;
	
	if (!SetValue(_T("Total"), _T("PanelScrapQty"), m_nPanelScrapQty), FALSE)	
		bReturn = FALSE;
	// End
	
	return bReturn;
}

BOOL MProductInfoData::WriteMaterialData()
{
	CString strSection;
	CString strItem;
	CString strTemp;
	BOOL bReturn = TRUE;

	/** Material Data */
	strSection = "Material Data";

	if (!SetValue(strSection, "m_iPreHandQty", m_iPreHandQty, FALSE))
		DISPLAY_WRITE_ERROR(strSection, "m_iPreHandQty", bReturn);

	if (!SetValue(strSection, "m_iWarningQty", m_iWarningQty, FALSE))
		DISPLAY_WRITE_ERROR(strSection, "m_iWarningQty", bReturn);

	/** TabIC Data */
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		for (int j = 0; j < DEF_MAX_REEL; j++)
		{
			strSection.Format(_T("TabFeeder%d_Reel%d"), i, j);

			if (!SetValue(strSection, "m_matTabICInfo.m_sM_MAKER", m_matTabICInfo[i][j].m_sM_MAKER, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_sM_MAKER", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_sM_BATCHID", m_matTabICInfo[i][j].m_sM_BATCHID, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_sM_BATCHID", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_nM_TYPE", m_matTabICInfo[i][j].m_nM_TYPE, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_nM_TYPE", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_sM_STEP", m_matTabICInfo[i][j].m_sM_STEP, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_sM_STEP", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_sM_CODE", m_matTabICInfo[i][j].m_sM_CODE, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_sM_CODE", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_nM_STATE", m_matTabICInfo[i][j].m_nM_STATE, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_nM_STATE", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_nT_QTY", m_matTabICInfo[i][j].m_nT_QTY, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_nT_QTY", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_nU_QTY", m_matTabICInfo[i][j].m_nU_QTY, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_nU_QTY", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_nN_QTY", m_matTabICInfo[i][j].m_nN_QTY, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_nN_QTY", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_nR_QTY", m_matTabICInfo[i][j].m_nR_QTY, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_nR_QTY", bReturn);

			if (!SetValue(strSection, "m_matTabICInfo.m_nR_QTY", m_matTabICInfo[i][j].m_nR_QTY, FALSE))
				DISPLAY_WRITE_ERROR(strSection, "m_matTabICInfo.m_nR_QTY", bReturn);
		}
	}

	/** Zig Data */
	for (int i = 0; i < DEF_MAX_TABFEEDER; i++)
	{
		strSection.Format(_T("Zig%d"),i);
		if (!SetValue(strSection, "m_matZigInfo.m_sM_MAKER", m_matZigInfo[i].m_sM_MAKER, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_sM_MAKER", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_sM_BATCHID", m_matZigInfo[i].m_sM_BATCHID, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_sM_BATCHID", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_nM_TYPE", m_matZigInfo[i].m_nM_TYPE, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_nM_TYPE", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_sM_STEP", m_matZigInfo[i].m_sM_STEP, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_sM_STEP", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_sM_CODE", m_matZigInfo[i].m_sM_CODE, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_sM_CODE", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_nM_STATE", m_matZigInfo[i].m_nM_STATE, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_nM_STATE", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_nT_QTY", m_matZigInfo[i].m_nT_QTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_nT_QTY", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_nU_QTY", m_matZigInfo[i].m_nU_QTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_nU_QTY", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_nN_QTY", m_matZigInfo[i].m_nN_QTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_nN_QTY", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_nR_QTY", m_matZigInfo[i].m_nR_QTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_nR_QTY", bReturn);

		if (!SetValue(strSection, "m_matZigInfo.m_nR_QTY", m_matZigInfo[i].m_nR_QTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection, "m_matZigInfo.m_nR_QTY", bReturn);
	}

	return bReturn;
}

BOOL MProductInfoData::WriteGlassData()
{
	BOOL	bReturn = TRUE;
	//100713.KKY_____
//	CString	strSection[] = { "LOAD CONVEYOR", "PANEL TRANSFER", "PANEL ALIGNER", "UNLOAD CONVEYOR" };
	CString	strSection[] = { "TRANSFER CENTER", "TURN HANDLER",  "TRANSFER IN", "PANEL ALIGNER", "TRANSFER OUT" };
	//_______________
	CString	strTemp;
	CString	strItem;

	for (int i=0; i<DEF_MAX_PANEL_UNIT; i++)
	{
		if (!SetValue(strSection[i], "m_strHGlassID", m_Glass[i].m_strHPanelID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strHGlassID", bReturn);

		if (!SetValue(strSection[i], "m_strEGlassID", m_Glass[i].m_strEPanelID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strEGlassID", bReturn);

		if (!SetValue(strSection[i], "m_strBatchID", m_Glass[i].m_strBatchID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strBatchID", bReturn);

		if (!SetValue(strSection[i], "m_strProdType", m_Glass[i].m_strProdType, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strProdType", bReturn);

		if (!SetValue(strSection[i], "m_strProdKind", m_Glass[i].m_strProdKind, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strProdKind", bReturn);

		if (!SetValue(strSection[i], "m_strDeviceID", m_Glass[i].m_strDeviceID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strDeviceID", bReturn);

		if (!SetValue(strSection[i], "m_strStepID", m_Glass[i].m_strStepID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strStepID", bReturn);
		
		if (!SetValue(strSection[i], "m_strPPID", m_Glass[i].m_strPPID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strPPID", bReturn);

		if (!SetValue(strSection[i], "m_strThickness", m_Glass[i].m_strThickness, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strThickness", bReturn);

		if (!SetValue(strSection[i], "m_strInsFlag", m_Glass[i].m_strInsFlag, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strInsFlag", bReturn);

		if (!SetValue(strSection[i], "m_strPanelSize", m_Glass[i].m_strPanelSize, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strPanelSize", bReturn);

		if (!SetValue(strSection[i], "m_strPanelPosition", m_Glass[i].m_strPanelPosition, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strPanelPosition", bReturn);

		if (!SetValue(strSection[i], "m_strCount1", m_Glass[i].m_strCount1, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strCount1", bReturn);

		if (!SetValue(strSection[i], "m_strCount2", m_Glass[i].m_strCount2, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strCount2", bReturn);

		if (!SetValue(strSection[i], "m_strGrade", m_Glass[i].m_strGrade, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strGrade", bReturn);

		if (!SetValue(strSection[i], "m_strComment", m_Glass[i].m_strComment, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strComment", bReturn);

		if (!SetValue(strSection[i], "m_strCompSize", m_Glass[i].m_strCompSize, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strCompSize", bReturn);

		if (!SetValue(strSection[i], "m_strReadingFlag", m_Glass[i].m_strReadingFlag, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strReadingFlag", bReturn);

		if (!SetValue(strSection[i], "m_strPanelState", m_Glass[i].m_strPanelState, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strPanelState", bReturn);

		if (!SetValue(strSection[i], "m_strJudgement", m_Glass[i].m_strJudgement, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strJudgement", bReturn);

		if (!SetValue(strSection[i], "m_strCode", m_Glass[i].m_strCode, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strCode", bReturn);

		if (!SetValue(strSection[i], "m_strRunLine", m_Glass[i].m_strRunLine, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strRunLine", bReturn);

		if (!SetValue(strSection[i], "m_strUniqueId", m_Glass[i].m_strUniqueId, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strUniqueId", bReturn);

		if (!SetValue(strSection[i], "m_strPairHPanelID", m_Glass[i].m_strPairHPanelID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strPairHPanelID", bReturn);

		if (!SetValue(strSection[i], "m_strPairEPanelID", m_Glass[i].m_strPairEPanelID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strPairEPanelID", bReturn);

		if (!SetValue(strSection[i], "m_strPairGrade", m_Glass[i].m_strPairGrade, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strPairGrade", bReturn);

		if (!SetValue(strSection[i], "m_strPairUniqueId", m_Glass[i].m_strPairUniqueId, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strPairUniqueId", bReturn);

		if (!SetValue(strSection[i], "m_strFlowRecipe", m_Glass[i].m_strFlowRecipe, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strFlowRecipe", bReturn);

		if (!SetValue(strSection[i], "m_strReserved0", m_Glass[i].m_strReserved0, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strReserved0", bReturn);

		if (!SetValue(strSection[i], "m_strBitsSignals", m_Glass[i].m_strBitsSignals, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strBitsSignals", bReturn);

		if (!SetValue(strSection[i], "m_strReferData", m_Glass[i].m_strReferData, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_strReferData", bReturn);
	}

	return bReturn;
}

BOOL MProductInfoData::WriteStatusData()
{
	BOOL	bReturn = TRUE;
	CString	strSection;
	strSection = "Status Data";
	if (!SetValue(strSection, "iPanelTurnStatus", m_iPanelTurnStatus, FALSE))
		DISPLAY_WRITE_ERROR(strSection, "iPanelTurnStatus", bReturn);

	return bReturn;
}

//110620.KKY.Add_____________
BOOL MProductInfoData::WriteProductPlan()
{
	BOOL	bReturn = TRUE;
	//110629.Modify_______________
	MProductPlanBatchData *pInfo[DEF_MAX_PRODUCT_PLAN_CNT*2+1] =
	{
		&m_CurProductBatchInfo, &m_NextProductBatchInfo[0], &m_NextProductBatchInfo[1], &m_NextProductBatchInfo[2], &m_NextProductBatchInfo[3], &m_NextProductBatchInfo[4],
								&m_EndProductBatchInfo[0], &m_EndProductBatchInfo[1], &m_EndProductBatchInfo[2], &m_EndProductBatchInfo[3], &m_EndProductBatchInfo[4]
	};

	CString	strSection[DEF_MAX_PRODUCT_PLAN_CNT*2+1] = 
	{
		_T("Current Product Plan"), _T("Next Product Plan1"), _T("Next Product Plan2"), _T("Next Product Plan3"), _T("Next Product Plan4"), _T("Next Product Plan5"), 
								_T("Ended Product Plan1"), _T("Ended Product Plan2"), _T("Ended Product Plan3"), _T("Ended Product Plan4"), _T("Ended Product Plan5")
	};
	CString	strTemp;

	for (int i=0; i<DEF_MAX_PRODUCT_PLAN_CNT*2+1; i++)
	{
		if (pInfo[i]->m_bRunFlag)	strTemp.Format(_T("TRUE"));
		else									strTemp.Format(_T("FALSE"));

		if (!SetValue(strSection[i], "m_bRunFlag", strTemp, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_bRunFlag", bReturn);

		if (!SetValue(strSection[i], "m_sModuleID", pInfo[i]->m_sModuleID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sModuleID", bReturn);

		if (!SetValue(strSection[i], "m_nPlanOrderID", pInfo[i]->m_nPlanOrderID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nPlanOrderID", bReturn);

		if (!SetValue(strSection[i], "m_nPriorty", pInfo[i]->m_nPriorty, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nPriorty", bReturn);

		if (!SetValue(strSection[i], "m_sProdKind", pInfo[i]->m_sProdKind, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sProdKind", bReturn);

		if (!SetValue(strSection[i], "m_sProdType", pInfo[i]->m_sProdType, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sProdType", bReturn);

		if (!SetValue(strSection[i], "m_sDeviceID", pInfo[i]->m_sDeviceID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sDeviceID", bReturn);

		if (!SetValue(strSection[i], "m_sStepID", pInfo[i]->m_sStepID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sStepID", bReturn);

		if (!SetValue(strSection[i], "m_sPPID", pInfo[i]->m_sPPID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sPPID", bReturn);

		if (!SetValue(strSection[i], "m_sBatchID", pInfo[i]->m_sBatchID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sBatchID", bReturn);

		if (!SetValue(strSection[i], "m_nBatchSize", pInfo[i]->m_nBatchSize, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nBatchSize", bReturn);

		if (!SetValue(strSection[i], "m_nBatchState", pInfo[i]->m_nBatchState, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nBatchState", bReturn);

		if (!SetValue(strSection[i], "m_sPanelMaker", pInfo[i]->m_sPanelMaker, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sPanelMaker", bReturn);

		if (!SetValue(strSection[i], "m_nPanelThickness", pInfo[i]->m_nPanelThickness, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nPanelThickness", bReturn);

		if (!SetValue(strSection[i], "m_sFirstGlassID", pInfo[i]->m_sFirstGlassID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sFirstGlassID", bReturn);

		if (!SetValue(strSection[i], "m_sCurGlassID", pInfo[i]->m_sCurGlassID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sCurGlassID", bReturn);

		if (!SetValue(strSection[i], "m_sInputLineNo", pInfo[i]->m_sInputLineNo, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sInputLineNo", bReturn);

		if (!SetValue(strSection[i], "m_nValidFlag", pInfo[i]->m_nValidFlag, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nValidFlag", bReturn);

		if (!SetValue(strSection[i], "m_nCQTY", pInfo[i]->m_nCQTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nCQTY", bReturn);

		if (!SetValue(strSection[i], "m_nOQTY", pInfo[i]->m_nOQTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nOQTY", bReturn);

		if (!SetValue(strSection[i], "m_nRQTY", pInfo[i]->m_nRQTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nRQTY", bReturn);

		if (!SetValue(strSection[i], "m_nNQTY", pInfo[i]->m_nNQTY, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nNQTY", bReturn);

	}

	return bReturn;
}

BOOL MProductInfoData::WriteMaterialCodeInfo()
{
	BOOL	bReturn = TRUE;
	CString	strSection[] = { "Current Product Plan", "Next Product Plan", "Ended Product Plan" };
	CString	strTemp;
	
	for (int i=0; i<DEF_MAX_PRODUCT_PLAN; i++)
	{
		if (!SetValue(strSection[i], "m_nWorkOrder", m_MaterialCodeInfo_S[i].m_nWorkOrder, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_nWorkOrder", bReturn);
	
		if (!SetValue(strSection[i], "m_sDeviceID", m_MaterialCodeInfo_S[i].m_sDeviceID, FALSE))
			DISPLAY_WRITE_ERROR(strSection[i], "m_sDeviceID", bReturn);

//		if (!SetValue(strSection[i], "m_nM_KIND", m_MaterialCodeInfo[i].m_nM_KIND, FALSE))
//			DISPLAY_WRITE_ERROR(strSection[i], "m_nM_KIND", bReturn);

//		if (!SetValue(strSection[i], "m_sM_STEP", m_MaterialCodeInfo[i].m_sM_STEP, FALSE))
//			DISPLAY_WRITE_ERROR(strSection[i], "m_sM_STEP", bReturn);

		int nCnt = m_MaterialCodeInfo_S[i].m_asM_CODE.GetSize();
		for (int j=0; j<DEF_MAX_MCODE; j++)
		{
			CString strValue;
			strTemp.Format(_T("MCodeS_%d"), i);

			if (j < nCnt)
				strValue = m_MaterialCodeInfo_S[i].m_asM_CODE.GetAt(j);

			if (!SetValue(strSection[i], strTemp, strValue, FALSE))
				DISPLAY_WRITE_ERROR(strSection[i], strTemp, bReturn);
		}
		nCnt = m_MaterialCodeInfo_G[i].m_asM_CODE.GetSize();
		for (int j=0; j<DEF_MAX_MCODE; j++)
		{
			CString strValue;
			strTemp.Format(_T("MCodeG_%d"), i);

			if (j < nCnt)
				strValue = m_MaterialCodeInfo_G[i].m_asM_CODE.GetAt(j);

			if (!SetValue(strSection[i], strTemp, strValue, FALSE))
				DISPLAY_WRITE_ERROR(strSection[i], strTemp, bReturn);
		}

	}

	return bReturn;
}
//___________________________

BOOL MProductInfoData::IncreaseProductionCount()
{
	m_iProductCount++;
	SetValue("Total", "ProductCount", m_iProductCount, FALSE);
	return TRUE;
}

void MProductInfoData::SetPanelInfo(double dLineTact, CString strPanelID, CTime timePanelIn, CTime timePanelOut)
{
	if (dLineTact == 0.0)
		return;
	//170329_KDH Machine Tacttime 남기는 코드 (Test중..),
	m_rgsPanelInfoQueue[m_iPanelInfoIndex].m_dMachineTact = m_dEquipTact;
	//________________

	m_rgsPanelInfoQueue[m_iPanelInfoIndex].m_dLineTact = dLineTact;
	m_rgsPanelInfoQueue[m_iPanelInfoIndex].m_strPanelID = strPanelID;
	m_rgsPanelInfoQueue[m_iPanelInfoIndex].m_timePanelIn = timePanelIn;
	m_rgsPanelInfoQueue[m_iPanelInfoIndex].m_timePanelOut = timePanelOut;

	m_iPanelInfoIndex++;

	if (m_iPanelInfoIndex == DEF_MAX_QUEUE_SIZE)
		m_iPanelInfoIndex = 0;
}

SPanelInfo* MProductInfoData::GetPanelInfo(int iQueueIndex)
{
	ASSERT(iQueueIndex > -1 && iQueueIndex < DEF_MAX_QUEUE_SIZE);

	return &(m_rgsPanelInfoQueue[iQueueIndex]);
}

int MProductInfoData::GetCurrentQueueIndex()
{
	return m_iPanelInfoIndex;
}

double MProductInfoData::GetMeanLineTact()
{
	int i = 0;
	double dSum = 0.0;

	for (int i = 0; i < DEF_MAX_QUEUE_SIZE; i++)
	{
		if (m_rgsPanelInfoQueue[i].m_dLineTact != 0.0)
		{
			dSum += m_rgsPanelInfoQueue[i].m_dLineTact;
		}
		else
			break;
	}

	if (0 == i)
		return 0.0;

	return dSum / static_cast<double>(i);
}

void MProductInfoData::SetPanelStatus(BOOL bTurn)
{
	m_iPanelTurnStatus = bTurn;
}

BOOL MProductInfoData::GetPanelStatus()
{
	return m_iPanelTurnStatus;
}
