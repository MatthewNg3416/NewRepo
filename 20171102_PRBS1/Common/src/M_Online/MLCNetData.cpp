// MLCNetData.cpp: implementation of the MLCNetData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MLCNetData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MBatchInfo::MBatchInfo()
{
	m_strPLCD = "";				
	m_nOrderNo = 0;				
	m_strMakerID = "";			
	m_strProdType = "";			
	m_strProdKind = "";			
	m_nThickness = 0;			
	m_strBatchID = "";			
	m_strInputLIne = "";			
	m_strDeviceID = "";			
	m_strStepID = "";			
	m_strPPID = "";				
	m_nPlanCount = 0;			
	m_nPriority = 0;			
	m_nOutCount = 0;			
	m_nRemainCount = 0;			
	m_nWorkFlag = 0;			
	m_strFirstPanelID = "";		
	m_strCurPanelID = "";		
	m_nValidFlag = 0;			
}

MBatchInfo::~MBatchInfo()
{
}

MProductPlanBatchData::MProductPlanBatchData()
{
	m_bRunFlag			= false;
	m_sModuleID			= "";
	m_nPlanOrderID		= 0;
	m_nPriorty			= 0;
	m_sProdKind			= "";
	m_sProdType			= "";
	m_sDeviceID			= "";
	m_sStepID			= "";
	m_sPPID				= "";
	m_sBatchID			= "";
	m_nBatchSize		= 0;
	m_nBatchSizeLast	= 0;	//New Online
	m_nBatchState		= 0;
	m_sPanelMaker		= "";
	m_nPanelThickness	= 0;
	m_sFirstGlassID		= "";
	m_sCurGlassID		= "";
	m_sInputLineNo		= "";
	m_nValidFlag		= 0;
	m_nCQTY				= 0;
	m_nOQTY				= 0;
	m_nRQTY				= 0;
	m_nNQTY				= 0;
}

MProductPlanBatchData::~MProductPlanBatchData()
{

}

/**
 * PlanBatchData를 설정한다. 
 *
 * @param	*p : Prodecut Plan Batch Data Class
 */
void MProductPlanBatchData::SetData(MProductPlanBatchData *p)
{
	m_bRunFlag			= p->m_bRunFlag;
	m_sModuleID			= p->m_sModuleID;			
	m_nPlanOrderID		= p->m_nPlanOrderID;		
	m_nPriorty			= p->m_nPriorty;			
	m_sProdKind			= p->m_sProdKind;			
	m_sProdType			= p->m_sProdType;			
	m_sDeviceID			= p->m_sDeviceID;			
	m_sStepID			= p->m_sStepID;			
	m_sPPID				= p->m_sPPID;				
	m_sBatchID			= p->m_sBatchID;			
	m_nBatchSize		= p->m_nBatchSize;
	m_nBatchSizeLast	= p->m_nBatchSizeLast;	//New Online
	m_nBatchState		= p->m_nBatchState;
	m_sPanelMaker		= p->m_sPanelMaker;		
	m_nPanelThickness	= p->m_nPanelThickness;	
	m_sFirstGlassID		= p->m_sFirstGlassID;		
	m_sCurGlassID		= p->m_sCurGlassID;		
	m_sInputLineNo		= p->m_sInputLineNo;		
	m_nValidFlag		= p->m_nValidFlag;		
	m_nCQTY				= p->m_nCQTY;				
	m_nOQTY				= p->m_nOQTY;				
	m_nRQTY				= p->m_nRQTY;				
	m_nNQTY				= p->m_nNQTY;				
}

//110803 SJ_KJS
//SJ_YYK 110730 Add,,
void MProductPlanBatchData::SetData2N1(MProductPlanBatchData *p)
{
	m_bRunFlag			= p->m_bRunFlag;
	m_sModuleID			= p->m_sModuleID;			
	m_nPlanOrderID		= p->m_nPlanOrderID2;		
	m_nPriorty			= p->m_nPriorty2;			
	m_sProdKind			= p->m_sProdKind2;			
	m_sProdType			= p->m_sProdType2;			
	m_sDeviceID			= p->m_sDeviceID2;			
	m_sStepID			= p->m_sStepID2;			
	m_sPPID				= p->m_sPPID2;				
	m_sBatchID			= p->m_sBatchID2;			
	m_nBatchSize		= p->m_nBatchSize2;		
	m_nBatchSizeLast	= p->m_nBatchSizeLast2;	//SJ_HJG
	m_nBatchState		= p->m_nBatchState2;
	m_sPanelMaker		= p->m_sPanelMaker2;		
	m_nPanelThickness	= p->m_nPanelThickness2;	
	m_sFirstGlassID		= p->m_sFirstGlassID2;		
	m_sCurGlassID		= p->m_sCurGlassID2;		
	m_sInputLineNo		= p->m_sInputLineNo2;		
	m_nValidFlag		= p->m_nValidFlag2;		
	m_nCQTY				= p->m_nCQTY2;				
	m_nOQTY				= p->m_nOQTY2;				
	m_nRQTY				= p->m_nRQTY2;				
	m_nNQTY				= p->m_nNQTY2;				
}
//_________________________________________


/**
 * PlanBatchData를 초기화 한다. 
 *
 */
void MProductPlanBatchData::Clear()
{
	m_bRunFlag			= false;
	m_sModuleID			= "";
	m_nPlanOrderID		= 0;
	m_nPriorty			= 0;
	m_sProdKind			= "";
	m_sProdType			= "";
	m_sDeviceID			= "";
	m_sStepID			= "";
	m_sPPID				= "";
	m_sBatchID			= "";
	m_nBatchSize		= 0;
	m_nBatchSizeLast	= 0;	//New Online
	m_nBatchState		= 0;
	m_sPanelMaker		= "";
	m_nPanelThickness	= 0;
	m_sFirstGlassID		= "";
	m_sCurGlassID		= "";
	m_sInputLineNo		= "";
	m_nValidFlag		= 0;
	m_nCQTY				= 0;
	m_nOQTY				= 0;
	m_nRQTY				= 0;
	m_nNQTY				= 0;

	//110803 SJ_KJS
	m_nPlanOrderID2		= 0;
	m_nPriorty2			= 0;
	m_sProdKind2			= "";
	m_sProdType2			= "";
	m_sDeviceID2			= "";
	m_sStepID2			= "";
	m_sPPID2				= "";
	m_sBatchID2			= "";
	m_nBatchSize2		= 0;
	m_nBatchSizeLast2	= 0;	////SJ_HJG
	m_nBatchState2		= 0;
	m_sPanelMaker2		= "";
	m_nPanelThickness2	= 0;
	m_sFirstGlassID2		= "";
	m_sCurGlassID2		= "";
	m_sInputLineNo2		= "";
	m_nValidFlag2		= 0;
	m_nCQTY2				= 0;
	m_nOQTY2			= 0;
	m_nRQTY2			= 0;
	m_nNQTY2			= 0;
	//___________________________

}

MDevice::MDevice()
{
	m_strProcessId = "";		
	m_strPartId = "";		
	m_strStepID = "";		
	m_strPPID = "";			
}
MDevice::~MDevice()
{
}

/**
 * Device Data를 설정한다. 
 *
 * @param	*p : Device에 관련된 Data Class
 */
void MDevice::CopyData(MDevice* p)
{
	m_strProcessId = p->m_strProcessId;				
	m_strPartId = p->m_strPartId;
	m_strStepID = p->m_strStepID;
	m_strPPID = p->m_strPPID;				
}


MMaterial::MMaterial()
{
	m_eMatEvt = eMatNothing;

	m_sPORTID = "";			// Material Port ID
	m_sEQP_STATE = "";		// Eq State
	m_sPORT_STATE = "";		// Port State
	m_sPORT_TYPE = "";		// Port Type(Input/Output/Both)
	m_sCSTID = "";			// 자재 반송 매체 ID
	m_nMAP_STIF = 0;		// Mapping 정보
	m_nCUR_STIF = 0;		// UnLoading 시 Mapping 정보
	m_sM_BATCHID = "";		// Material Batchid
	m_sM_MAKER = "";		// Material Maker
	m_nM_KIND = 0;			// Material Kind
	m_nM_TYPE = 0;			// Material Type
	m_nM_STATE = 0;		// Material State
	m_nT_QTY = 0;			// 자재 전체 수량
	m_nU_QTY = 0;			// 사용된 수량
	m_nR_QTY = 0;			// 남은 수량
	m_nN_QTY = 0;			// NG 누적 수량
	m_nA_QTY = 0;			// Assemble  자재 수량
	m_nE_FLAG = 0;			// 자재 완료되었을 때, 자투리 인지 유무
	m_sC_CODE = "";			// Cancel Code
	m_sM_STEP = "";			// 자재 종류 구분 
	m_nS_QTY = 0;			// 자재 요청 수량
	m_sM_CODE = "";			// Materal Defect Code
}
void MMaterial::ResetData()
{
	m_eMatEvt = eMatNothing;

//	m_sPORTID = "";			// Material Port ID, ex)FEEDER1, FEEDER2
	m_sEQP_STATE = "";		// Eq State
	m_sPORT_STATE = "";		// Port State
	m_sPORT_TYPE = "";		// Port Type(Input/Output/Both)
	m_sCSTID = "";			// 자재 반송 매체 ID
	m_nMAP_STIF = 0;		// Mapping 정보
	m_nCUR_STIF = 0;		// UnLoading 시 Mapping 정보
	m_sM_BATCHID = "";		// Material Batchid
	m_sM_MAKER = "";		// Material Maker
//	m_nM_KIND = 0;			// Material Kind, ex) 13
	m_nM_TYPE = 0;			// Material Type
	m_nM_STATE = 0;		// Material State
	m_nT_QTY = 0;			// 자재 전체 수량
	m_nU_QTY = 0;			// 사용된 수량
	m_nR_QTY = 0;			// 남은 수량
	m_nN_QTY = 0;			// NG 누적 수량
	m_nA_QTY = 0;			// Assemble  자재 수량
	m_nE_FLAG = 0;			// 자재 완료되었을 때, 자투리 인지 유무
	m_sC_CODE = "";			// Cancel Code
//	m_sM_STEP = "";			// 자재 종류 구분 
	m_nS_QTY = 0;			// 자재 요청 수량
	m_sM_CODE = "";			// Materal Defect Code
}
MMaterial::~MMaterial()
{
}
/**
 * Material Data 설정한다. 
 *
 * @param	*p : Tap IC 및 금형에 관련된 Data Class
 */

void MMaterial::CopyData(MMaterial* p)
{
	m_eMatEvt = p->m_eMatEvt;

	m_sPORTID = p->m_sPORTID;			
	m_sEQP_STATE = p->m_sEQP_STATE;		
	m_sPORT_STATE = p->m_sPORT_STATE;		
	m_sPORT_TYPE = p->m_sPORT_TYPE;		
	m_sCSTID = p->m_sCSTID;			
	m_nMAP_STIF = p->m_nMAP_STIF;		
	m_nCUR_STIF = p->m_nCUR_STIF;		
	m_sM_BATCHID = p->m_sM_BATCHID;		
	m_sM_MAKER = p->m_sM_MAKER;		
	m_nM_KIND = p->m_nM_KIND;			
	m_nM_TYPE = p->m_nM_TYPE;			
	m_nM_STATE = p->m_nM_STATE;		
	m_nT_QTY = p->m_nT_QTY;			
	m_nU_QTY = p->m_nU_QTY;			
	m_nR_QTY = p->m_nR_QTY;			
	m_nN_QTY = p->m_nN_QTY;			
	m_nA_QTY = p->m_nA_QTY;			
	m_nE_FLAG = p->m_nE_FLAG;			
	m_sC_CODE = p->m_sC_CODE;			
	m_sM_STEP = p->m_sM_STEP;			
	m_nS_QTY = p->m_nS_QTY;	
	m_sM_CODE = p->m_sM_CODE;
}


MEqConstant::MEqConstant()
{
	//100807.ECID__________
	m_strECID = "";
	//m_nECID = 0;
	m_strECName = "";			
	m_strECDef = "";				
	m_strECSLL = "";				
	m_strECSUL = "";				
	m_strECWLL = "";				
	m_strECWUL = "";				
}
MEqConstant::~MEqConstant()
{
}

//100807.ECID__________
void MEqConstant::ResetData()
{
	m_strECID = "";				
	m_strECName = "";			
	m_strECDef = "";				
	m_strECSLL = "";				
	m_strECSUL = "";				
	m_strECWLL = "";				
	m_strECWUL = "";				
}
//_____________________

/**
 * MEqConstant Data 설정한다. 
 *
 * @param	*p : 설비 ID 및 이름에 관련된 Data Class
 */
void MEqConstant::CopyData(MEqConstant* p)
{
	//100807.ECID__________
	m_strECID = p->m_strECID;
	//m_nECID = p->m_nECID;				
	m_strECName = p->m_strECName;
	m_strECDef = p->m_strECDef;
	m_strECSLL = p->m_strECSLL;				
	m_strECSUL = p->m_strECSUL;
	m_strECWLL = p->m_strECWLL;
	m_strECWUL = p->m_strECWUL;
}

MEqOnlineParam::MEqOnlineParam()
{
	m_nEOID = 0;			
	m_nEOMD = 0;			
	m_nEOV = 0;				
}
MEqOnlineParam::~MEqOnlineParam()
{
}

/**
 * Online Data 설정한다. 
 *
 * @param	*p : Online에 관련된 Data Class
 */
void MEqOnlineParam::CopyData(MEqOnlineParam* p)
{
	m_nEOID = p->m_nEOID;				
	m_nEOMD = p->m_nEOMD;
	m_nEOV = p->m_nEOV;
}


MStateVariable::MStateVariable()
{
	m_nSVID = 0;				
	m_strSV = "";				
	m_strSVName = "";			
}
MStateVariable::~MStateVariable()
{
}

/**
 * StateVariable Data 설정한다. 
 *
 * @param	*p : 설비 상태에 관련된 Data Class
 */
void MStateVariable::CopyData(MStateVariable* p)
{
	m_nSVID = p->m_nSVID;				
	m_strSV = p->m_strSV;
	m_strSVName = p->m_strSVName;
}


MPPBody::MPPBody()
{
	m_strPParamName = "";				
	m_strPParamValue = "";			
}
MPPBody::~MPPBody()
{
}

/**
 * StateVariable Data 설정한다. 
 *
 * @param	*p : Process 상태에 관련된 Data Class
 */
void MPPBody::CopyData(MPPBody* p)
{
	m_strPParamName = p->m_strPParamName;				
	m_strPParamValue = p->m_strPParamValue;
}


MCCode::MCCode()
{
	m_nCCode = 0;			
	m_nPPBodyQty = 0;		
	m_paPPBoby = NULL;			
}
MCCode::~MCCode()
{
	if(m_paPPBoby) {
		delete [] m_paPPBoby; m_paPPBoby = NULL;
	}
}

/**
 * PPID Data 설정한다. 
 *
 * @param	*p : PPID에 관련된 Data Class
 */
void MCCode::CopyData(MCCode* p)
{
	int i;

	m_nCCode = p->m_nCCode;				
	m_nPPBodyQty = p->m_nPPBodyQty;
	for(i=0; i<m_nPPBodyQty; i++){
		m_paPPBoby[i].CopyData(&p->m_paPPBoby[i]);
	}
}
void MCCode::SetCount(int nItemCnt)
{
	m_nPPBodyQty = nItemCnt;
	m_paPPBoby = new MPPBody[m_nPPBodyQty];
}

MProcessProgram::MProcessProgram()
{
	m_strPPID = "";				
	m_nCCodeQty = 0;			
	m_paCCode = NULL;				
}
MProcessProgram::~MProcessProgram()
{
	if(m_paCCode){
		delete [] m_paCCode; m_paCCode=NULL;
	}
}

/**
 * ProcessProgram Data 설정한다. 
 *
 * @param	*p : Process Program에 관련된 Data Class
 */
void MProcessProgram::CopyData(MProcessProgram* p)
{
	int i;

	m_strPPID = p->m_strPPID;				
	m_nCCodeQty = p->m_nCCodeQty;
	for(i=0; i<m_nCCodeQty; i++){
		m_paCCode[i].CopyData(&p->m_paCCode[i]);
	}
}
void MProcessProgram::SetCount(int nItemCnt)
{
	m_nCCodeQty = nItemCnt;
	m_paCCode = new MCCode[m_nCCodeQty];
}

MPanel::MPanel()
{
	m_strHPanelID       = "";
	m_strEPanelID		= "";
	m_strBatchID	    = "";
	m_strProdType		= "";
	m_strProdKind		= "";
	m_strDeviceID		= "";
	m_strStepID			= "";
	m_strPPID			= "";
	m_strThickness		= "";
	m_strInsFlag		= "";
	m_strPanelSize      = "";
	m_strPanelPosition  = "";
	m_strCount1			= "";
	m_strCount2			= "";
	m_strGrade			= "";
	m_strComment        = "";
	m_strCompSize       = "";
	m_strReadingFlag    = "";
	m_strPanelState     = "";
	m_strJudgement      = "";
	m_strCode			= "";
	m_strRunLine		= "";
	m_strUniqueId		= "";
	m_strPairHPanelID   = "";
	m_strPairEPanelID   = "";
	m_strPairGrade		= "";
	m_strPairUniqueId   = "";
	m_strFlowRecipe		= "";
	m_strReserved0      = "";
	m_strBitsSignals    = "";
	m_strReferData		= "";			
}
MPanel::~MPanel()
{
}

/**
 * Panel Data 설정한다. 
 *
 * @param	*p : Panel에 관련된 Data Class
 */
void MPanel::CopyData(MPanel* p)
{
	m_strHPanelID        = p->m_strHPanelID;     
	m_strEPanelID		 = p->m_strEPanelID;		
	m_strBatchID	     = p->m_strBatchID;	    
	m_strProdType		 = p->m_strProdType;		
	m_strProdKind		 = p->m_strProdKind;		
	m_strDeviceID		 = p->m_strDeviceID;		
	m_strStepID			 = p->m_strStepID;		
	m_strPPID			 = p->m_strPPID;			
	m_strThickness		 = p->m_strThickness;	
	m_strInsFlag		 = p->m_strInsFlag;		
	m_strPanelSize       = p->m_strPanelSize;    
	m_strPanelPosition   = p->m_strPanelPosition;
	m_strCount1			 = p->m_strCount1;		
	m_strCount2			 = p->m_strCount2;		
	m_strGrade			 = p->m_strGrade;		
	m_strComment         = p->m_strComment;      
	m_strCompSize        = p->m_strCompSize;     
	m_strReadingFlag     = p->m_strReadingFlag;  
	m_strPanelState      = p->m_strPanelState;   
	m_strJudgement       = p->m_strJudgement;    
	m_strCode			 = p->m_strCode;			
	m_strRunLine		 = p->m_strRunLine;		
	m_strUniqueId		 = p->m_strUniqueId;		
	m_strPairHPanelID    = p->m_strPairHPanelID; 
	m_strPairEPanelID    = p->m_strPairEPanelID; 
	m_strPairGrade		 = p->m_strPairGrade;	
	m_strPairUniqueId    = p->m_strPairUniqueId; 
	m_strFlowRecipe		 = p->m_strFlowRecipe;	
	m_strReserved0       = p->m_strReserved0;    
	m_strBitsSignals     = p->m_strBitsSignals;
	m_strReferData		 = p->m_strReferData;
}

void MPanel::ResetData()
{
	m_strHPanelID         = "";
	m_strEPanelID		  = "";
	m_strBatchID	      = "";
	m_strProdType		  = "";
	m_strProdKind		  = "";
	m_strDeviceID		  = "";
	m_strStepID			  = "";
	m_strPPID			  = "";
	m_strThickness		  = "";
	m_strInsFlag		  = "";
	m_strPanelSize        = "";
	m_strPanelPosition    = "";
	m_strCount1			  = "";
	m_strCount2			  = "";
	m_strGrade			  = "";
	m_strComment          = "";
	m_strCompSize         = "";
	m_strReadingFlag      = "";
	m_strPanelState       = "";
	m_strJudgement        = "";
	m_strCode			  = "";
	m_strRunLine		  = "";
	m_strUniqueId		  = "";
	m_strPairHPanelID     = "";
	m_strPairEPanelID     = "";
	m_strPairGrade		  = "";
	m_strPairUniqueId     = "";
	m_strFlowRecipe		  = "";
	m_strReserved0        = "";
	m_strBitsSignals      = "";
	m_strReferData		  = "";			
}

MAlarm::MAlarm()
{
	m_bAlarmSet = false;							

	m_nAlarmId = 0;			
	m_nAlarmCode = 0;		
	m_sAlarmText = "";		
}
MAlarm::~MAlarm()
{
}	

/**
 * Alram Data 설정한다. 
 *
 * @param	*p : Alram에 관련된 Data Class
 */
void MAlarm::CopyData(MAlarm* p)
{
	m_bAlarmSet = p->m_bAlarmSet;							

	m_nAlarmId = p->m_nAlarmId;			
	m_nAlarmCode = p->m_nAlarmCode;		
	m_sAlarmText = p->m_sAlarmText;		
}


MBarCodeParsingInfo::MBarCodeParsingInfo()
{
	m_sMaker = "";				// Maker

	m_nStartIndex_ID = 0;		// ID Start Position 
	m_nEndIndex_ID = 0;			// ID End Position 
	m_nStartIndex_Qty = 0;		// Qty Start Position
	m_nEndIndex_Qty = 0;		// Qty End Position
}
MBarCodeParsingInfo::~MBarCodeParsingInfo()
{
}	

/**
 * BarCord Data 설정한다. 
 *
 * @param	*p : BarCode에 관련된 Data Class
 */
void MBarCodeParsingInfo::CopyData(MBarCodeParsingInfo* p)
{
	m_sMaker = p->m_sMaker;						// Maker

	m_nStartIndex_ID = p->m_nStartIndex_ID;		// ID Start Position 
	m_nEndIndex_ID = p->m_nEndIndex_ID;			// ID End Position 
	m_nStartIndex_Qty = p->m_nStartIndex_Qty;	// Qty Start Position
	m_nEndIndex_Qty = p->m_nEndIndex_Qty;		// Qty End Position
}

//____________________________________________________________________________________________________
// 091028 SJ_Chi, Data Collection Report 추가
MProcessParamInfo::MProcessParamInfo()
{

	m_iDataCount = 0;

	//20140218 SJ_HJG
	for (int i = 0; i<500; i++)
	{
		m_rgsProcessParam[i].m_strParamName = _T("");
		m_rgsProcessParam[i].m_dSettingValue = 0;
		m_rgsProcessParam[i].m_dMeasureValue = 0;
		m_rgsProcessParam[i].m_iIonizerState = 0;
	}
//	memset(m_rgszParamName, NULL, sizeof(m_rgszParamName));
//	for (int i = 0; i<30; i++)
//	{
//		m_rgiSettingValue[i] = 0;
//		m_rgiMeasureValue[i] = 0;
//	}
}

MProcessParamInfo::~MProcessParamInfo()
{
}

/**
 * Process Parameter Data 설정한다. 
 *
 * @param	*p : 공정 관리 Parameter 관련된 Data Class
 */

void MProcessParamInfo::CopyData(MProcessParamInfo* p)
{

	m_iDataCount = p->m_iDataCount;

	for (int i = 0; i<m_iDataCount; i++)
	{
		m_rgsProcessParam[i].m_strParamName.Format(p->m_rgsProcessParam[i].m_strParamName);
		m_rgsProcessParam[i].m_dSettingValue = p->m_rgsProcessParam[i].m_dSettingValue;
		m_rgsProcessParam[i].m_dSettingValue = p->m_rgsProcessParam[i].m_dMeasureValue;
		m_rgsProcessParam[i].m_iIonizerState = p->m_rgsProcessParam[i].m_iIonizerState;
	}

//	for (int i = 0; i<m_iDataCount; i++)
//	{
//		printf(m_rgszParamName[i], p->m_rgszParamName[i]);
//		m_rgiSettingValue[i] = p->m_rgiSettingValue[i];
//		m_rgiMeasureValue[i] = p->m_rgiMeasureValue[i];
//	}
}
//___________________________________________________________________


//110620.KKY_____
//________________________________________________
MPlanRequestData::MPlanRequestData()
{
	m_sLINE_NO = "";
	m_sEQPID = "";
	m_sCarrierID = "";
	m_sDeviceID = "";
	m_nM_KIND = 0;
	m_sM_STEP = "";
}

MPlanRequestData::~MPlanRequestData()
{
}

/** 
 *	Product Plan Request Data 설정
 * @param	*p : BarCode에 관련된 Data Class
 */
void MPlanRequestData::CopyData(MPlanRequestData* p)
{
	m_sLINE_NO = p->m_sLINE_NO;
	m_sEQPID = p->m_sEQPID;
	m_sCarrierID = p->m_sCarrierID;
	m_sDeviceID = p->m_sDeviceID;
	m_nM_KIND = p->m_nM_KIND;
	m_sM_STEP = p->m_sM_STEP;
}
//________________________________________________

//________________________________________________
MMaterialCodeInfo::MMaterialCodeInfo()
{
	ResetData();
}

MMaterialCodeInfo::~MMaterialCodeInfo()
{
	m_asM_CODE.RemoveAll();
}

void MMaterialCodeInfo::CopyData(MMaterialCodeInfo* p)
{
	m_nWorkOrder = p->m_nWorkOrder;
	m_sDeviceID	= p->m_sDeviceID;
	m_nM_KIND = p->m_nM_KIND;
	m_sM_STEP = p->m_sM_STEP;
	m_asM_CODE.RemoveAll();
	int nCnt = p->m_asM_CODE.GetSize();
	for (int i=0; i<nCnt; i++)
		m_asM_CODE.Add(p->m_asM_CODE.GetAt(i));
}

void MMaterialCodeInfo::ResetData()
{
	m_nWorkOrder = 0;
	m_sDeviceID	= "";
	m_nM_KIND = eMatKind_NONE;
	m_sM_STEP = "";
	m_asM_CODE.RemoveAll();
}
//________________________________________________

////////////////////////////////////////////
MProductCodeData::MProductCodeData()
{
//	m_iUnitID = 0;
	m_strCSTID = " ";
	m_iCSTTYPE = 0;
	m_iReq_Qty = 0;
	m_strPanelID = " ";
}

MProductCodeData::~MProductCodeData()
{
}

/** 
	Product Code Data 설정 : Unit#1 - Vericode, Unit#2 - Barcode 
*/
void MProductCodeData::CopyData(MProductCodeData* p)
{
//	m_iUnitID = p->m_iUnitID;
	m_strCSTID = p->m_strCSTID;
	m_iCSTTYPE = p->m_iCSTTYPE;
	m_iReq_Qty = p->m_iReq_Qty;
	m_strPanelID = p->m_strPanelID;
}

