// MNetHData.cpp: implementation of the MNetHData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNetHData.h"
#include "DefSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MRobotArmStateBit::MRobotArmStateBit()
{

}
MRobotArmStateBit::~MRobotArmStateBit()
{

}
void MRobotArmStateBit::SetData(MRobotArmStateBit* p)
{
	m_nRobotAbnormal = p->m_nRobotAbnormal;
	m_nTypeOfArm = p->m_nTypeOfArm;
	m_nTypeOfStageConveyor = p->m_nTypeOfStageConveyor;
	m_nArm1Violate = p->m_nArm1Violate;
	m_nArm2Violate = p->m_nArm2Violate;
	m_nArm1FoldComplete = p->m_nArm1FoldComplete;
	m_nArm2FoldComplete = p->m_nArm2FoldComplete;
	m_nArm1GlassCheckSensor = p->m_nArm1GlassCheckSensor;
	m_nArm2GlassCheckSensor = p->m_nArm2GlassCheckSensor;
	m_nReserved0 = p->m_nReserved0;
	m_nReserved1 = p->m_nReserved1;
	m_nReserved2 = p->m_nReserved2;
	m_nManualOperation = p->m_nManualOperation;
	m_nRobotDirection = p->m_nRobotDirection;
	m_nReserved3 = p->m_nReserved3;
	m_nReserved4 = p->m_nReserved4;
}



MToUpperEqBit::MToUpperEqBit()
{

}
MToUpperEqBit::~MToUpperEqBit()
{

}

MToLowerEqBit::MToLowerEqBit()
{

}
MToLowerEqBit::~MToLowerEqBit()
{

}

MStageStateBit::MStageStateBit()
{

}
MStageStateBit::~MStageStateBit()
{

}
void MStageStateBit::SetData(MStageStateBit* p)
{
	m_nStageCVAbnormal = p->m_nStageCVAbnormal;
	m_nTypeOfArm = p->m_nTypeOfArm;
	m_nTypeOfStageConveyor = p->m_nTypeOfStageConveyor;
	m_nEmpty = p->m_nEmpty;
	m_nIdle = p->m_nIdle;
	m_nBusy = p->m_nBusy;
	m_nPinUp = p->m_nPinUp;
	m_nPinDown = p->m_nPinDown;
	m_nStopperUp = p->m_nStopperUp;
	m_nStopperDown = p->m_nStopperDown;
	m_nDoorOpen = p->m_nDoorOpen;
	m_nDoorClose = p->m_nDoorClose;
	m_nManualOperation = p->m_nManualOperation;
	m_nBodyMoving = p->m_nBodyMoving;	
	m_nGlassCheckSensorOn = p->m_nGlassCheckSensorOn;
	m_nReserved0 = p->m_nReserved0;
}

MContactStateBit::MContactStateBit()
{
	for(int i = 0; i < W_U_SIZE; i++)
	{
		m_nContactState[i] = 0;
	}
}
MContactStateBit::~MContactStateBit()
{

}
void MContactStateBit::SetData(MContactStateBit* p)
{
	for(int i = 0; i < W_U_SIZE; i++)
	{
		m_nContactState[i] = p->m_nContactState[i];
	}
}

MToUpperEqs::MToUpperEqs()
{
	m_strReadingGlassID = "";					
	m_strReadingGlassIDType = "";				
	m_strGlassType = "";	
	m_strReceiveRefuseCode = "";
	m_strReserved = "";			
	m_strCSIF = "";					
	m_strAS = "";				
	m_strAPS = "";					
}
MToUpperEqs::~MToUpperEqs()
{

}

MToLowerEqs::MToLowerEqs()
{
	m_strHPanelID= "";					
	m_strEPanelID= "";
	m_strBatchID= "";					
	m_strProdType= "";
	m_strProdKind= "";
	m_strDeviceID= "";
	m_strStepID= "";
	m_strPPID= "";
	m_strThickness= "0";
	m_strInsFlag= "";
	m_strPanelSize= "0";
	m_strPanelPosition= "";
	m_strCount1= "0";
	m_strCount2= "0";
	m_strGrade= "";
	m_strComment= "";
	m_strCompSize= "";
	m_strReadingFlag= "";
	m_strPanelState= "";
	m_strJudgement= "";
	m_strCode= "";
	m_strRunLine= "";		//Binary String
	m_strUniqueId= "";
	m_strPairHPanelID= "";
	m_strPairEPanelID= "";
	m_strPairGrade= "";
	m_strPairUniqueId= "";
	m_strFlowRecipe= "";
	m_strReserved0= "";
	m_strBitsSignals= "";
	m_strReferData= "";
	m_strCSIF= "";
	m_strAS= "";
	m_strAPS= "";
}
MToLowerEqs::~MToLowerEqs()
{

}

void MToLowerEqs::ResetData()
{
	m_strHPanelID= "";					
	m_strEPanelID= "";
	m_strBatchID= "";					
	m_strProdType= "";
	m_strProdKind= "";
	m_strDeviceID= "";
	m_strStepID= "";
	m_strPPID= "";
	m_strThickness= "0";
	m_strInsFlag= "";
	m_strPanelSize= "0";
	m_strPanelPosition= "";
	m_strCount1= "0";
	m_strCount2= "0";
	m_strGrade= "";
	m_strComment= "";
	m_strCompSize= "";
	m_strReadingFlag= "";
	m_strPanelState= "";
	m_strJudgement= "";
	m_strCode= "";
	m_strRunLine= "";		//Binary String
	m_strUniqueId= "";
	m_strPairHPanelID= "";
	m_strPairEPanelID= "";
	m_strPairGrade= "";
	m_strPairUniqueId= "";
	m_strFlowRecipe= "";
	m_strReserved0= "";
	m_strBitsSignals= "";
	m_strReferData= "";
	m_strCSIF= "";
	m_strAS= "";
	m_strAPS= "";
}
void MToLowerEqs::CopyData(MToLowerEqs Data)
{
	m_strHPanelID			= Data.m_strHPanelID;				
	m_strEPanelID			= Data.m_strEPanelID;
	m_strBatchID			= Data.m_strBatchID;				
	m_strProdType			= Data.m_strProdType;
	m_strProdKind			= Data.m_strProdKind;
	m_strDeviceID			= Data.m_strDeviceID;
	m_strStepID				= Data.m_strStepID;
	m_strPPID				= Data.m_strPPID;
	m_strThickness			= Data.m_strThickness;
	m_strInsFlag			= Data.m_strInsFlag;
	m_strPanelSize			= Data.m_strPanelSize;
	m_strPanelPosition		= Data.m_strPanelPosition;
	m_strCount1				= Data.m_strCount1;
	m_strCount2				= Data.m_strCount2;
	m_strGrade				= Data.m_strGrade;
	m_strComment			= Data.m_strComment;
	m_strCompSize			= Data.m_strCompSize;
	m_strReadingFlag		= Data.m_strReadingFlag;
	m_strPanelState			= Data.m_strPanelState;
	m_strJudgement			= Data.m_strJudgement;
	m_strCode				= Data.m_strCode;
	m_strRunLine			= Data.m_strRunLine;
	m_strUniqueId			= Data.m_strUniqueId;
	m_strPairHPanelID		= Data.m_strPairHPanelID;
	m_strPairEPanelID		= Data.m_strPairEPanelID;
	m_strPairGrade			= Data.m_strPairGrade;
	m_strPairUniqueId		= Data.m_strPairUniqueId;
	m_strFlowRecipe			= Data.m_strFlowRecipe;
	m_strReserved0			= Data.m_strReserved0;
	m_strBitsSignals		= Data.m_strBitsSignals;
	m_strReferData			= Data.m_strReferData;
	m_strCSIF				= Data.m_strCSIF;
	m_strAS					= Data.m_strAS;
	m_strAPS				= Data.m_strAPS;
}

void MToLowerEqs::SetJobOrder()
{
	int nValue = 0;

	//nValue = atoi(m_strBitsSignals);
	nValue = _wtoi(m_strBitsSignals);
	nValue = nValue | 0x04;
	m_strBitsSignals.Format(_T("%d"), nValue);
}
void MToLowerEqs::RstJobOrder()
{
	int nValue = 0;

	//nValue = atoi(m_strBitsSignals);
	nValue = _wtoi(m_strBitsSignals);
	nValue = nValue & (0xFFFF - 0x04);
	m_strBitsSignals.Format(_T("%d"), nValue);
}

MEqStates::MEqStates()
{
}
MEqStates::~MEqStates()
{
}


MGlassSummarys::MGlassSummarys()
{
	//20111206 SJ_HJG
	/*
	m_strTGlassCount="0";					
	m_strTFTCount="0";				
	m_strCFCount="0";					
	m_strDummyCount="0";				
	m_strBPGCount="0";					
	m_strProcessID1="";				
	m_strStepID1="";					
	m_strBatchID1="";				
	m_strGlassCount1="0";					
	m_strProcessID2="";				
	m_strStepID2="";					
	m_strBatchID2="";				
	m_strGlassCount2="0";					
	m_strProcessID3="";				
	m_strStepID3="";					
	m_strBatchID3="";				
	m_strGlassCount3="0";					
	*/
	
	for(int i = 0; i < DEF_MAX_TAB; i ++)
	{
		m_strReceiveTabOffsetDataA[i]="0";
		m_strReceiveTabOffsetDataB[i]="0";
	}
}
MGlassSummarys::~MGlassSummarys()
{
}

MGlassDatas::MGlassDatas()
{
	m_strHPanelID = "";		
	m_strEPanelID = "";			
	m_strBatchID = "";		
	m_strProdType = "";		
	m_strProdKind = "";		
	m_strDeviceID = "";		
	m_strStepID = "";		
	m_strPPID = "";	
	m_strThickness = "";		
	m_strInsFlag = "";		
	m_strPanelSize = "";		
	m_strPanelPosition = "";	
	m_strCount1 = "";		
	m_strCount2 = "";		
	m_strGrade = "";
	m_strComment = "";		
	m_strCompSize = "";		
	m_strReadingFlag = "";	
	m_strPanelState = "";	
	m_strJudgement = "";		
	m_strCode = "";
	m_strRunLine = "";		
	m_strUniqueId = "";		
	m_strPairHPanelID = "";	
	m_strPairEPanelID = "";	
	m_strPairGrade = "";		
	m_strPairUniqueId = "";	
	m_strFlowRecipe = "";	
	m_strReserved0 = "";		
	m_strBitsSignals = "";	
	m_strReferData = "";
}


MGlassDatas::~MGlassDatas()
{

}


void MGlassDatas::SetJobOrder()
{
	int nValue = 0;

	//nValue = atoi(m_strBitsSignals);
	nValue = _wtoi(m_strBitsSignals);
	nValue = nValue | 0x04;
	m_strBitsSignals.Format(_T("%d"), nValue);
}


void MGlassDatas::RstJobOrder()
{
	int nValue = 0;

	//nValue = atoi(m_strBitsSignals);
	nValue = _wtoi(m_strBitsSignals);
	nValue = nValue & (0xFFFF - 0x04);
	m_strBitsSignals.Format(_T("%d"), nValue);
}

void MGlassDatas::CopyData(MGlassDatas Data)
{
	m_strHPanelID			=Data.m_strHPanelID;									
	m_strEPanelID			=Data.m_strEPanelID;				
	m_strBatchID			=Data.m_strBatchID;								
	m_strProdType			=Data.m_strProdType;				
	m_strProdKind			=Data.m_strProdKind;				
	m_strDeviceID			=Data.m_strDeviceID;				
	m_strStepID				=Data.m_strStepID;					
	m_strPPID				=Data.m_strPPID;					
	m_strThickness			=Data.m_strThickness;			
	m_strInsFlag			=Data.m_strInsFlag;				
	m_strPanelSize			=Data.m_strPanelSize;			
	m_strPanelPosition		=Data.m_strPanelPosition;	
	m_strCount1				=Data.m_strCount1;					
	m_strCount2				=Data.m_strCount2;					
	m_strGrade				=Data.m_strGrade;						
	m_strComment			=Data.m_strComment;				
	m_strCompSize			=Data.m_strCompSize;				
	m_strReadingFlag		=Data.m_strReadingFlag;		
	m_strPanelState			=Data.m_strPanelState;			
	m_strJudgement			=Data.m_strJudgement;				
	m_strCode				=Data.m_strCode;						
	m_strRunLine			=Data.m_strRunLine;				
	m_strUniqueId			=Data.m_strUniqueId;				
	m_strPairHPanelID		=Data.m_strPairHPanelID;		
	m_strPairEPanelID		=Data.m_strPairEPanelID;		
	m_strPairGrade			=Data.m_strPairGrade;			
	m_strPairUniqueId		=Data.m_strPairUniqueId;		
	m_strFlowRecipe			=Data.m_strFlowRecipe;			
	m_strReserved0			=Data.m_strReserved0;			
	m_strBitsSignals		=Data.m_strBitsSignals;		
	m_strReferData			=Data.m_strReferData;			
	
	//for(int i=0; i<12; i++){ m_rgnBitsSignal[i]=Data.m_rgnBitsSignal[i]; }

}

void MGlassDatas::ResetData()
{
	m_strHPanelID			="";							
	m_strEPanelID			="";		
	m_strBatchID			="";						
	m_strProdType			="";		
	m_strProdKind			="";		
	m_strDeviceID			="";		
	m_strStepID				="";		
	m_strPPID				="";		
	m_strThickness			="";	
	m_strInsFlag			="";		
	m_strPanelSize			="";	
	m_strPanelPosition		="";
	m_strCount1				="";		
	m_strCount2				="";		
	m_strGrade				="";			
	m_strComment			="";		
	m_strCompSize			="";		
	m_strReadingFlag		="";	
	m_strPanelState			="";	
	m_strJudgement			="";		
	m_strCode				="";			
	m_strRunLine			="";		
	m_strUniqueId			="";		
	m_strPairHPanelID		="";	
	m_strPairEPanelID		="";	
	m_strPairGrade			="";	
	m_strPairUniqueId		="";	
	m_strFlowRecipe			="";	
	m_strReserved0			="";	
	m_strBitsSignals		="";	
	m_strReferData			="";	
	
	//for(int i=0; i<12; i++){ m_rgnBitsSignal[i]=0; }
}

MModelChangeWordDatas::MModelChangeWordDatas()
{
	m_strDeviceID = "";		
	m_strEQState = "";			
	m_strReplyCode = "";		
	m_strPanelWIP = "";		
}

MModelChangeWordDatas::~MModelChangeWordDatas()
{
}

void MModelChangeWordDatas::CopyData(MModelChangeWordDatas Data)
{
	m_strDeviceID = Data.m_strDeviceID;		
	m_strEQState = Data.m_strEQState;			
	m_strReplyCode = Data.m_strEQState;		
	m_strPanelWIP = Data.m_strEQState;		
}

void MModelChangeWordDatas::ResetData()
{
	m_strDeviceID = "";		
	m_strEQState = "";			
	m_strReplyCode = "";		
	m_strPanelWIP = "";		
}